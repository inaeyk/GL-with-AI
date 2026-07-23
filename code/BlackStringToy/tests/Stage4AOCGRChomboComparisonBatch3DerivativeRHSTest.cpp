#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

// Test-only access boundary: the scalar pointer kernels in the inspected
// GRChombo derivative class do not require a Chombo Cell.  Suppress only the
// grid wrapper includes and provide the scalar SIMD fallbacks that Cell.hpp
// normally makes visible.  The derivative formulas below are invoked from the
// real inspected header; they are not copied into this fixture.
#define CH_SPACEDIM 3
#define CELL_HPP_
template <class data_t> class Cell;
#define USERVARIABLES_HPP
#define SIMD_HPP_
#include "AlwaysInline.hpp"
template <typename data_t, typename pointer_t>
ALWAYS_INLINE pointer_t *SIMDIFY(pointer_t *pointer)
{
    return pointer;
}
template <typename data_t>
ALWAYS_INLINE data_t simd_conditional(const bool condition,
                                      const data_t &when_true,
                                      const data_t &when_false)
{
    return condition ? when_true : when_false;
}
template <typename data_t>
ALWAYS_INLINE bool simd_compare_gt(const data_t &left, const data_t &right)
{
    return left > right;
}
#include "FourthOrderDerivatives.hpp"

// Reuse the batch-2 test-only, independently validated d=3 CCZ4 family
// decomposition and direct rhs_equation bridge.  This is a single translation
// unit, so no production symbol or header is changed.
#define main stage4aoc_grchombo_comparison_batch2_embedded_main
#include "Stage4AOCGRChomboComparisonBatch2Test.cpp"
#undef main

#include "Stage4AOFrozenGaugeOperator.hpp"
#include "Stage4AOFrozenGaugeRadialBoundary.hpp"

namespace
{
constexpr double pi = 3.141592653589793238462643383279502884;
constexpr double period = 2.0 * pi;
constexpr double radial_origin = 0.4;
constexpr std::array<int, 4> refinements = {32, 64, 128, 256};
constexpr int field_count = 22;
constexpr int visible_row_count = 15;
constexpr int visible_tensor_count = 6;
constexpr int derivative_family_count = 6;
constexpr int rhs_family_count = 9;

enum RHSFamilyIndex
{
    rhs_raw_ricci,
    rhs_encoded_z,
    rhs_lapse_hessian,
    rhs_advection,
    rhs_shift_input_derivatives,
    rhs_chi_shift,
    rhs_h_shift,
    rhs_a_shift,
    rhs_combined
};

constexpr std::array<const char *, visible_row_count> visible_row_names = {
    "chi", "K",   "Theta", "hxx", "hyy", "hzz", "hxy", "hxz",
    "hyz", "Axx", "Ayy",   "Azz", "Axy", "Axz", "Ayz"};
constexpr std::array<const char *, visible_tensor_count>
    visible_tensor_names = {"xx", "yy", "zz", "xy", "xz", "yz"};
constexpr std::array<std::array<int, 2>, visible_tensor_count>
    visible_tensor_components = {
        {{{0, 0}}, {{1, 1}}, {{2, 2}}, {{0, 1}}, {{0, 2}}, {{1, 2}}}};

enum Field
{
    f_chi,
    f_K,
    f_Theta,
    f_lapse,
    f_Gamma_x,
    f_Gamma_y,
    f_Gamma_z,
    f_shift_x,
    f_shift_y,
    f_shift_z,
    f_h_xx,
    f_h_yy,
    f_h_zz,
    f_h_xy,
    f_h_xz,
    f_h_yz,
    f_A_xx,
    f_A_yy,
    f_A_zz,
    f_A_xy,
    f_A_xz,
    f_A_yz
};

struct Jet
{
    double value = 0.0;
    double dx = 0.0;
    double dz = 0.0;
    double dxx = 0.0;
    double dxz = 0.0;
    double dzz = 0.0;
};

Jet jet_add(const Jet &left, const Jet &right)
{
    return {left.value + right.value,
            left.dx + right.dx,
            left.dz + right.dz,
            left.dxx + right.dxx,
            left.dxz + right.dxz,
            left.dzz + right.dzz};
}

Jet jet_scale(const Jet &input, const double factor)
{
    return {factor * input.value, factor * input.dx, factor * input.dz,
            factor * input.dxx, factor * input.dxz, factor * input.dzz};
}

Jet jet_multiply(const Jet &left, const Jet &right)
{
    Jet out;
    out.value = left.value * right.value;
    out.dx = left.dx * right.value + left.value * right.dx;
    out.dz = left.dz * right.value + left.value * right.dz;
    out.dxx = left.dxx * right.value + 2.0 * left.dx * right.dx +
              left.value * right.dxx;
    out.dxz = left.dxz * right.value + left.dx * right.dz +
              left.dz * right.dx + left.value * right.dxz;
    out.dzz = left.dzz * right.value + 2.0 * left.dz * right.dz +
              left.value * right.dzz;
    return out;
}

Jet jet_reciprocal(const Jet &input)
{
    const double squared = input.value * input.value;
    const double cubed = squared * input.value;
    Jet out;
    out.value = 1.0 / input.value;
    out.dx = -input.dx / squared;
    out.dz = -input.dz / squared;
    out.dxx = 2.0 * input.dx * input.dx / cubed -
              input.dxx / squared;
    out.dxz = 2.0 * input.dx * input.dz / cubed -
              input.dxz / squared;
    out.dzz = 2.0 * input.dz * input.dz / cubed -
              input.dzz / squared;
    return out;
}

double base_value(const int field)
{
    constexpr std::array<double, field_count> bases = {
        0.91,  0.31, -0.08, 1.11,  0.12,  -0.05, 0.09,  0.07,
        -0.04, 0.05, 1.05,  1.02,  0.98,  0.025, -0.020, 0.018,
        0.04,  -0.03, 0.02, 0.015, -0.012, 0.010};
    return bases[static_cast<std::size_t>(field)];
}

Jet analytic_jet(const int field, const double x, const double z)
{
    // Keep the manufactured conformal metric on det(h)=1, as required by the
    // inspected CCZ4 geometry formulas.  All three off-diagonal components
    // are nonzero multi-mode fields.  Solve the full symmetric 3x3 determinant
    // identity for h_yy and propagate its complete first/second analytic jet.
    if (field == f_h_yy)
    {
        const Jet a = analytic_jet(f_h_xx, x, z);
        const Jet b = analytic_jet(f_h_xz, x, z);
        const Jet c = analytic_jet(f_h_zz, x, z);
        const Jet d = analytic_jet(f_h_xy, x, z);
        const Jet e = analytic_jet(f_h_yz, x, z);
        const Jet denominator =
            jet_add(jet_multiply(a, c),
                    jet_scale(jet_multiply(b, b), -1.0));
        Jet numerator{};
        numerator.value = 1.0;
        numerator = jet_add(
            numerator,
            jet_scale(jet_multiply(jet_multiply(d, b), e), -2.0));
        numerator =
            jet_add(numerator, jet_multiply(a, jet_multiply(e, e)));
        numerator =
            jet_add(numerator, jet_multiply(c, jet_multiply(d, d)));
        return jet_multiply(numerator, jet_reciprocal(denominator));
    }
    const double phase = 0.13 * static_cast<double>(field + 1);
    const double a = 0.0015 * static_cast<double>(1 + field % 4);
    const double b = 0.0011 * static_cast<double>(1 + field % 3);
    const double c = 0.0009 * static_cast<double>(1 + field % 5);
    const double d = 0.0007 * static_cast<double>(1 + field % 2);
    const double tx = x + phase;
    const double tz = 2.0 * z - phase;
    const double tm = x + z + 0.5 * phase;
    const double tc = 2.0 * x - z - phase;

    Jet out;
    out.value = base_value(field) + a * std::sin(tx) + b * std::cos(tz) +
                c * std::sin(tm) + d * std::cos(tc);
    out.dx = a * std::cos(tx) + c * std::cos(tm) -
             2.0 * d * std::sin(tc);
    out.dz = -2.0 * b * std::sin(tz) + c * std::cos(tm) +
             d * std::sin(tc);
    out.dxx = -a * std::sin(tx) - c * std::sin(tm) -
              4.0 * d * std::cos(tc);
    out.dxz = -c * std::sin(tm) + 2.0 * d * std::cos(tc);
    out.dzz = -4.0 * b * std::cos(tz) - c * std::sin(tm) -
              d * std::cos(tc);
    return out;
}

class FourthOrderAccess : public FourthOrderDerivatives
{
  public:
    using FourthOrderDerivatives::FourthOrderDerivatives;
    using FourthOrderDerivatives::advection_term;
};

struct DiscreteJet
{
    Jet jet;
    double advection = 0.0;
};

BlackStringToy::Stage4AOFrozenGaugeOperator::FrozenGaugePerturbationVector
filled_custom_vector(const double value)
{
    std::array<
        double,
        BlackStringToy::Stage4AOFrozenGaugeOperator::
            frozen_gauge_state_vector.size()>
        values{};
    values.fill(value);
    return BlackStringToy::Stage4AOFrozenGaugeOperator::
        make_frozen_gauge_perturbation_vector(values);
}

Jet custom_discrete_jet(const int field, const int points, const double x,
                        const double z)
{
    namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
    namespace Boundary = BlackStringToy::Stage4AOFrozenGaugeRadialBoundary;
    namespace Inner = BlackStringToy::Stage4AOFrozenGaugeInnerBoundary;
    const double spacing = period / static_cast<double>(points);

    std::vector<Operator::FrozenGaugePerturbationVector> radial;
    radial.reserve(3);
    for (int offset = -1; offset <= 1; ++offset)
    {
        radial.push_back(filled_custom_vector(
            analytic_jet(field, x + offset * spacing, z).value));
    }
    const auto radial_jet = Boundary::make_centered_derivative_jet(
        radial, 1, spacing, 0.0, Inner::FourierParitySector::P_plus);

    std::vector<double> z_values(static_cast<std::size_t>(points));
    std::vector<double> radial_dx_values(static_cast<std::size_t>(points));
    for (int j = 0; j < points; ++j)
    {
        const double zj = period * static_cast<double>(j) /
                          static_cast<double>(points);
        z_values[static_cast<std::size_t>(j)] =
            analytic_jet(field, x, zj).value;
        std::vector<Operator::FrozenGaugePerturbationVector> local_radial;
        local_radial.reserve(3);
        for (int offset = -1; offset <= 1; ++offset)
        {
            local_radial.push_back(filled_custom_vector(
                analytic_jet(field, x + offset * spacing, zj).value));
        }
        radial_dx_values[static_cast<std::size_t>(j)] =
            Boundary::make_centered_derivative_jet(
                local_radial, 1, spacing, 0.0,
                Inner::FourierParitySector::P_plus)
                .dx()
                .value_at_index(0);
    }
    int z_index = static_cast<int>(
        std::llround(z * static_cast<double>(points) / period));
    z_index %= points;
    if (z_index < 0)
    {
        z_index += points;
    }

    Jet out;
    out.value = analytic_jet(field, x, z).value;
    out.dx = radial_jet.dx().value_at_index(0);
    out.dxx = radial_jet.dxx().value_at_index(0);
    out.dz = Operator::periodic_dz(
        z_values, period, static_cast<std::size_t>(z_index));
    out.dzz = Operator::periodic_dzz(
        z_values, period, static_cast<std::size_t>(z_index));
    out.dxz = Operator::periodic_dz(
        radial_dx_values, period, static_cast<std::size_t>(z_index));
    return out;
}

Jet grchombo_discrete_jet(const int field, const int points, const double x,
                          const double z)
{
    constexpr int patch_width = 7;
    constexpr int center = 3 * patch_width + 3;
    constexpr int x_stride = patch_width;
    constexpr int z_stride = 1;
    const double spacing = period / static_cast<double>(points);
    std::array<double, patch_width * patch_width> patch{};
    for (int ix = -3; ix <= 3; ++ix)
    {
        for (int iz = -3; iz <= 3; ++iz)
        {
            patch[static_cast<std::size_t>((ix + 3) * patch_width +
                                           (iz + 3))] =
                analytic_jet(field, x + ix * spacing, z + iz * spacing)
                    .value;
        }
    }
    const FourthOrderAccess derivatives(spacing);
    Jet out;
    out.value = patch[center];
    out.dx = derivatives.diff1<double>(patch.data(), center, x_stride);
    out.dz = derivatives.diff1<double>(patch.data(), center, z_stride);
    out.dxx = derivatives.diff2<double>(patch.data(), center, x_stride);
    out.dzz = derivatives.diff2<double>(patch.data(), center, z_stride);
    out.dxz = derivatives.mixed_diff2<double>(
        patch.data(), center, x_stride, z_stride);
    return out;
}

void assign_value(AnalyticInput &input, const int field, const double value)
{
    if (field == f_chi)
        input.vars.chi = value;
    else if (field == f_K)
        input.vars.K = value;
    else if (field == f_Theta)
        input.vars.Theta = value;
    else if (field == f_lapse)
        input.vars.lapse = value;
    else if (field >= f_Gamma_x && field <= f_Gamma_z)
        input.vars.Gamma[field - f_Gamma_x] = value;
    else if (field >= f_shift_x && field <= f_shift_z)
        input.vars.shift[field - f_shift_x] = value;
    else if (field >= f_h_xx && field <= f_h_yz)
    {
        constexpr std::array<std::array<int, 2>, 6> component = {
            {{{0, 0}}, {{1, 1}}, {{2, 2}},
             {{0, 1}}, {{0, 2}}, {{1, 2}}}};
        const auto ij = component[static_cast<std::size_t>(field - f_h_xx)];
        input.vars.h[ij[0]][ij[1]] = value;
        input.vars.h[ij[1]][ij[0]] = value;
    }
    else
    {
        constexpr std::array<std::array<int, 2>, 6> component = {
            {{{0, 0}}, {{1, 1}}, {{2, 2}},
             {{0, 1}}, {{0, 2}}, {{1, 2}}}};
        const auto ij = component[static_cast<std::size_t>(field - f_A_xx)];
        input.vars.A[ij[0]][ij[1]] = value;
        input.vars.A[ij[1]][ij[0]] = value;
    }
}

void assign_derivatives(AnalyticInput &input, const int field, const Jet &jet)
{
    auto assign_vector = [&](auto &d1, auto &d2)
    {
        d1[0] = jet.dx;
        d1[1] = 0.0;
        d1[2] = jet.dz;
        d2[0][0] = jet.dxx;
        d2[0][1] = d2[1][0] = 0.0;
        d2[0][2] = d2[2][0] = jet.dxz;
        d2[1][1] = d2[1][2] = d2[2][1] = 0.0;
        d2[2][2] = jet.dzz;
    };
    if (field == f_chi)
        assign_vector(input.d1.chi, input.d2.chi);
    else if (field == f_lapse)
        assign_vector(input.d1.lapse, input.d2.lapse);
    else if (field == f_K)
    {
        input.d1.K[0] = jet.dx;
        input.d1.K[1] = 0.0;
        input.d1.K[2] = jet.dz;
    }
    else if (field == f_Theta)
    {
        input.d1.Theta[0] = jet.dx;
        input.d1.Theta[1] = 0.0;
        input.d1.Theta[2] = jet.dz;
    }
    else if (field >= f_Gamma_x && field <= f_Gamma_z)
    {
        const int component = field - f_Gamma_x;
        input.d1.Gamma[component][0] = jet.dx;
        input.d1.Gamma[component][1] = 0.0;
        input.d1.Gamma[component][2] = jet.dz;
    }
    else if (field >= f_shift_x && field <= f_shift_z)
    {
        const int component = field - f_shift_x;
        input.d1.shift[component][0] = jet.dx;
        input.d1.shift[component][1] = 0.0;
        input.d1.shift[component][2] = jet.dz;
    }
    else
    {
        constexpr std::array<std::array<int, 2>, 6> component = {
            {{{0, 0}}, {{1, 1}}, {{2, 2}},
             {{0, 1}}, {{0, 2}}, {{1, 2}}}};
        const bool is_h = field >= f_h_xx && field <= f_h_yz;
        const int first = is_h ? f_h_xx : f_A_xx;
        const auto ij = component[static_cast<std::size_t>(field - first)];
        auto assign_tensor_d1 = [&](auto &tensor)
        {
            tensor[ij[0]][ij[1]][0] = jet.dx;
            tensor[ij[0]][ij[1]][1] = 0.0;
            tensor[ij[0]][ij[1]][2] = jet.dz;
            tensor[ij[1]][ij[0]][0] = jet.dx;
            tensor[ij[1]][ij[0]][1] = 0.0;
            tensor[ij[1]][ij[0]][2] = jet.dz;
        };
        if (is_h)
        {
            assign_tensor_d1(input.d1.h);
            for (int a = 0; a < dimension; ++a)
            {
                for (int b = 0; b < dimension; ++b)
                {
                    double value = 0.0;
                    if (a == 0 && b == 0)
                        value = jet.dxx;
                    else if ((a == 0 && b == 2) ||
                             (a == 2 && b == 0))
                        value = jet.dxz;
                    else if (a == 2 && b == 2)
                        value = jet.dzz;
                    input.d2.h[ij[0]][ij[1]][a][b] = value;
                    input.d2.h[ij[1]][ij[0]][a][b] = value;
                }
            }
        }
        else
        {
            assign_tensor_d1(input.d1.A);
        }
    }
}

void assign_advection(AnalyticInput &input, const int field,
                      const double value)
{
    if (field == f_chi)
        input.advec.chi = value;
    else if (field == f_K)
        input.advec.K = value;
    else if (field == f_Theta)
        input.advec.Theta = value;
    else if (field == f_lapse)
        input.advec.lapse = value;
    else if (field >= f_Gamma_x && field <= f_Gamma_z)
        input.advec.Gamma[field - f_Gamma_x] = value;
    else if (field >= f_shift_x && field <= f_shift_z)
        input.advec.shift[field - f_shift_x] = value;
    else
    {
        constexpr std::array<std::array<int, 2>, 6> component = {
            {{{0, 0}}, {{1, 1}}, {{2, 2}},
             {{0, 1}}, {{0, 2}}, {{1, 2}}}};
        const bool is_h = field >= f_h_xx && field <= f_h_yz;
        const int first = is_h ? f_h_xx : f_A_xx;
        const auto ij = component[static_cast<std::size_t>(field - first)];
        auto &tensor = is_h ? input.advec.h : input.advec.A;
        tensor[ij[0]][ij[1]] = value;
        tensor[ij[1]][ij[0]] = value;
    }
}

AnalyticInput manufactured_input(const int points, const double x,
                                 const double z, const bool grchombo,
                                 const bool exact)
{
    AnalyticInput out = make_flat_input(
        exact ? "analytic" : (grchombo ? "grchombo" : "custom"));
    std::array<Jet, field_count> jets{};
    for (int field = 0; field < field_count; ++field)
    {
        jets[static_cast<std::size_t>(field)] =
            exact ? analytic_jet(field, x, z)
                  : (grchombo ? grchombo_discrete_jet(field, points, x, z)
                              : custom_discrete_jet(field, points, x, z));
        assign_value(out, field,
                     jets[static_cast<std::size_t>(field)].value);
        assign_derivatives(out, field,
                           jets[static_cast<std::size_t>(field)]);
    }

    const double shift_x = out.vars.shift[0];
    const double shift_z = out.vars.shift[2];
    for (int field = 0; field < field_count; ++field)
    {
        double advection = 0.0;
        if (exact || !grchombo)
        {
            const auto &jet = jets[static_cast<std::size_t>(field)];
            advection = shift_x * jet.dx + shift_z * jet.dz;
        }
        else
        {
            constexpr int patch_width = 7;
            constexpr int center = 3 * patch_width + 3;
            const double spacing = period / static_cast<double>(points);
            std::array<double, patch_width * patch_width> patch{};
            for (int ix = -3; ix <= 3; ++ix)
            {
                for (int iz = -3; iz <= 3; ++iz)
                {
                    patch[static_cast<std::size_t>((ix + 3) * patch_width +
                                                   iz + 3)] =
                        analytic_jet(field, x + ix * spacing,
                                     z + iz * spacing)
                            .value;
                }
            }
            const FourthOrderAccess derivatives(spacing);
            advection =
                derivatives.advection_term<double>(
                    patch.data(), center, shift_x, patch_width,
                    simd_compare_gt(shift_x, 0.0)) +
                derivatives.advection_term<double>(
                    patch.data(), center, shift_z, 1,
                    simd_compare_gt(shift_z, 0.0));
        }
        assign_advection(out, field, advection);
    }
    return out;
}

struct ErrorRecord
{
    double error = 0.0;
    double actual = 0.0;
    double expected = 0.0;
    double physical_z = 0.0;
    int refinement = 0;
    std::string family;
    std::string component;

    void observe(const double actual, const double expected,
                 const std::string &family_name,
                 const std::string &component_name, const int points,
                 const double z)
    {
        const double candidate = std::abs(actual - expected);
        if (candidate > error)
        {
            error = candidate;
            this->actual = actual;
            this->expected = expected;
            physical_z = z;
            refinement = points;
            family = family_name;
            component = component_name;
        }
    }

    std::string worst() const
    {
        std::ostringstream out;
        out << component << "@N" << refinement << ",z=" << physical_z;
        return out.str();
    }
};

struct LevelErrors
{
    std::array<ErrorRecord, derivative_family_count> custom_derivative;
    std::array<ErrorRecord, derivative_family_count> grchombo_derivative;
    std::array<ErrorRecord, rhs_family_count> custom_rhs;
    std::array<ErrorRecord, rhs_family_count> grchombo_rhs;
    std::array<ErrorRecord, visible_row_count> custom_advection;
    std::array<ErrorRecord, visible_row_count> grchombo_advection;
    ErrorRecord custom_chi_shift;
    ErrorRecord grchombo_chi_shift;
    std::array<ErrorRecord, visible_tensor_count> custom_h_shift;
    std::array<ErrorRecord, visible_tensor_count> grchombo_h_shift;
    std::array<ErrorRecord, visible_tensor_count> custom_a_shift;
    std::array<ErrorRecord, visible_tensor_count> grchombo_a_shift;
    ErrorRecord finite_rhs_difference;
};

std::array<double, visible_row_count> visible_rows(const Vars<double> &rhs)
{
    return {rhs.chi,
            rhs.K,
            rhs.Theta,
            rhs.h[0][0],
            rhs.h[1][1],
            rhs.h[2][2],
            rhs.h[0][1],
            rhs.h[0][2],
            rhs.h[1][2],
            rhs.A[0][0],
            rhs.A[1][1],
            rhs.A[2][2],
            rhs.A[0][1],
            rhs.A[0][2],
            rhs.A[1][2]};
}

Vars<double> custom_total(const RHSFamilies &families)
{
    Vars<double> out = zero_scalar_vars();
    out.chi = families.chi_total;
    out.K = families.k_total;
    out.Theta = families.theta_total;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out.h[i][j] = families.h_total[i][j];
            out.A[i][j] = families.a_total[i][j];
        }
    }
    return out;
}

std::array<double, visible_row_count>
advection_rows(const RHSFamilies &families)
{
    return {families.chi_advection,
            families.k_advection,
            families.theta_advection,
            families.h_advection[0][0],
            families.h_advection[1][1],
            families.h_advection[2][2],
            families.h_advection[0][1],
            families.h_advection[0][2],
            families.h_advection[1][2],
            families.a_advection[0][0],
            families.a_advection[1][1],
            families.a_advection[2][2],
            families.a_advection[0][1],
            families.a_advection[0][2],
            families.a_advection[1][2]};
}

std::array<double, visible_tensor_count>
visible_tensor_rows(const Matrix &matrix)
{
    std::array<double, visible_tensor_count> out{};
    for (std::size_t component = 0; component < out.size(); ++component)
    {
        const auto ij = visible_tensor_components[component];
        out[component] = matrix[ij[0]][ij[1]];
    }
    return out;
}

Matrix combined_h_shift(const RHSFamilies &families)
{
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
        for (int j = 0; j < dimension; ++j)
            out[i][j] = families.h_shift_stretching[i][j] +
                        families.h_trace_correction[i][j];
    return out;
}

Matrix combined_a_shift(const RHSFamilies &families)
{
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
        for (int j = 0; j < dimension; ++j)
            out[i][j] = families.a_shift_stretching[i][j] +
                        families.a_trace_correction[i][j];
    return out;
}

AnalyticInput without_shift_derivatives(AnalyticInput input)
{
    for (int i = 0; i < dimension; ++i)
        for (int j = 0; j < dimension; ++j)
            input.d1.shift[i][j] = 0.0;
    return input;
}

void observe_matrix(ErrorRecord &record, const Matrix &actual,
                    const Matrix &expected, const std::string &family,
                    const int points, const double z)
{
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            record.observe(actual[i][j], expected[i][j],
                           family,
                           "[" + std::to_string(i) + "," +
                               std::to_string(j) + "]",
                           points, z);
        }
    }
}

double matrix_max_difference(const Matrix &left, const Matrix &right)
{
    double out = 0.0;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out = std::max(out, std::abs(left[i][j] - right[i][j]));
        }
    }
    return out;
}

double observed_order(const double coarse, const double fine)
{
    return std::log(coarse / fine) / std::log(2.0);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        std::cerr << "FAIL " << message << "\n";
        std::exit(1);
    }
}

void observe_derivatives(LevelErrors &errors, const int points,
                         const double x, const double z)
{
    constexpr std::array<const char *, 6> names = {
        "Dx", "Dz", "Dxx", "Dzz", "Dxz", "divergence"};
    for (int field = 0; field < field_count; ++field)
    {
        const Jet exact = analytic_jet(field, x, z);
        const Jet custom = custom_discrete_jet(field, points, x, z);
        const Jet grchombo = grchombo_discrete_jet(field, points, x, z);
        const std::array<double, 5> expected = {
            exact.dx, exact.dz, exact.dxx, exact.dzz, exact.dxz};
        const std::array<double, 5> custom_value = {
            custom.dx, custom.dz, custom.dxx, custom.dzz, custom.dxz};
        const std::array<double, 5> grchombo_value = {
            grchombo.dx, grchombo.dz, grchombo.dxx, grchombo.dzz,
            grchombo.dxz};
        for (int derivative = 0; derivative < 5; ++derivative)
        {
            const std::string component =
                "field" + std::to_string(field);
            errors.custom_derivative[static_cast<std::size_t>(derivative)]
                .observe(custom_value[static_cast<std::size_t>(derivative)],
                         expected[static_cast<std::size_t>(derivative)],
                         names[static_cast<std::size_t>(derivative)],
                         component, points, z);
            errors.grchombo_derivative[static_cast<std::size_t>(derivative)]
                .observe(
                    grchombo_value[static_cast<std::size_t>(derivative)],
                    expected[static_cast<std::size_t>(derivative)],
                    names[static_cast<std::size_t>(derivative)], component,
                    points, z);
        }
    }
    double exact_divergence = 0.0;
    double custom_divergence = 0.0;
    double grchombo_divergence = 0.0;
    for (int component = 0; component < dimension; ++component)
    {
        const int field = f_shift_x + component;
        const Jet exact = analytic_jet(field, x, z);
        const Jet custom = custom_discrete_jet(field, points, x, z);
        const Jet grchombo = grchombo_discrete_jet(field, points, x, z);
        if (component == 0)
        {
            exact_divergence += exact.dx;
            custom_divergence += custom.dx;
            grchombo_divergence += grchombo.dx;
        }
        else if (component == 2)
        {
            exact_divergence += exact.dz;
            custom_divergence += custom.dz;
            grchombo_divergence += grchombo.dz;
        }
    }
    errors.custom_derivative[5].observe(custom_divergence, exact_divergence,
                                        "divergence", "shift", points, z);
    errors.grchombo_derivative[5].observe(
        grchombo_divergence, exact_divergence, "divergence", "shift",
        points, z);
}

void observe_rhs(LevelErrors &errors, const int points, const double x,
                 const double z, const DirectCCZ4Access &access)
{
    const AnalyticInput exact = manufactured_input(points, x, z, false, true);
    const AnalyticInput custom =
        manufactured_input(points, x, z, false, false);
    const AnalyticInput grchombo =
        manufactured_input(points, x, z, true, false);
    const GeometryPieces exact_geometry = independent_geometry_pieces(exact);
    const GeometryPieces custom_geometry =
        independent_geometry_pieces(custom);
    const GeometryPieces grchombo_geometry =
        direct_geometry_pieces(grchombo);
    const Matrix exact_hessian = physical_lapse_hessian(exact);
    const Matrix custom_hessian = physical_lapse_hessian(custom);
    const Matrix grchombo_hessian = physical_lapse_hessian(grchombo);
    const RHSFamilies exact_families = custom_d3_families(exact);
    const RHSFamilies custom_families = custom_d3_families(custom);
    const Vars<double> exact_rhs = custom_total(exact_families);
    const Vars<double> custom_rhs = custom_total(custom_families);
    const Vars<double> grchombo_rhs = direct_rhs(grchombo, access);
    const Vars<double> grchombo_rhs_without_shift =
        direct_rhs(without_shift_derivatives(grchombo), access);

    observe_matrix(errors.custom_rhs[rhs_raw_ricci],
                   custom_geometry.raw.tensor, exact_geometry.raw.tensor,
                   "raw_Ricci", points, z);
    errors.custom_rhs[rhs_raw_ricci].observe(
        custom_geometry.raw.scalar, exact_geometry.raw.scalar, "raw_Ricci",
        "scalar", points, z);
    observe_matrix(errors.grchombo_rhs[rhs_raw_ricci],
                   grchombo_geometry.raw.tensor, exact_geometry.raw.tensor,
                   "raw_Ricci", points, z);
    errors.grchombo_rhs[rhs_raw_ricci].observe(
        grchombo_geometry.raw.scalar, exact_geometry.raw.scalar,
        "raw_Ricci", "scalar", points, z);
    observe_matrix(errors.custom_rhs[rhs_encoded_z],
                   custom_geometry.encoded.tensor,
                   exact_geometry.encoded.tensor, "encoded_Z", points, z);
    observe_matrix(errors.grchombo_rhs[rhs_encoded_z],
                   grchombo_geometry.encoded.tensor,
                   exact_geometry.encoded.tensor, "encoded_Z", points, z);
    observe_matrix(errors.custom_rhs[rhs_lapse_hessian], custom_hessian,
                   exact_hessian, "lapse_Hessian", points, z);
    observe_matrix(errors.grchombo_rhs[rhs_lapse_hessian],
                   grchombo_hessian, exact_hessian, "lapse_Hessian", points,
                   z);

    const auto exact_advection = advection_rows(exact_families);
    const auto custom_advection = advection_rows(custom_families);
    const auto grchombo_advection =
        advection_rows(custom_d3_families(grchombo));
    for (int row = 0; row < visible_row_count; ++row)
    {
        const std::size_t index = static_cast<std::size_t>(row);
        errors.custom_advection[index].observe(
            custom_advection[index], exact_advection[index], "advection",
            visible_row_names[index], points, z);
        errors.grchombo_advection[index].observe(
            grchombo_advection[index], exact_advection[index], "advection",
            visible_row_names[index], points, z);
        errors.custom_rhs[rhs_advection].observe(
            custom_advection[index], exact_advection[index], "advection",
            visible_row_names[index], points, z);
        errors.grchombo_rhs[rhs_advection].observe(
            grchombo_advection[index], exact_advection[index], "advection",
            visible_row_names[index], points, z);
    }

    for (int component = 0; component < dimension; ++component)
    {
        errors.custom_rhs[rhs_shift_input_derivatives].observe(
            custom.d1.shift[component][0], exact.d1.shift[component][0],
            "shift_input_derivatives",
            "shift" + std::to_string(component) + ".Dx", points, z);
        errors.custom_rhs[rhs_shift_input_derivatives].observe(
            custom.d1.shift[component][2], exact.d1.shift[component][2],
            "shift_input_derivatives",
            "shift" + std::to_string(component) + ".Dz", points, z);
        errors.grchombo_rhs[rhs_shift_input_derivatives].observe(
            grchombo.d1.shift[component][0], exact.d1.shift[component][0],
            "shift_input_derivatives",
            "shift" + std::to_string(component) + ".Dx", points, z);
        errors.grchombo_rhs[rhs_shift_input_derivatives].observe(
            grchombo.d1.shift[component][2], exact.d1.shift[component][2],
            "shift_input_derivatives",
            "shift" + std::to_string(component) + ".Dz", points, z);
    }

    const double exact_chi_shift = exact_families.chi_shift_divergence;
    const double custom_chi_shift = custom_families.chi_shift_divergence;
    const double grchombo_chi_shift =
        grchombo_rhs.chi - grchombo_rhs_without_shift.chi;
    errors.custom_chi_shift.observe(custom_chi_shift, exact_chi_shift,
                                    "chi_shift_RHS", "chi", points, z);
    errors.grchombo_chi_shift.observe(grchombo_chi_shift, exact_chi_shift,
                                      "chi_shift_RHS", "chi", points, z);
    errors.custom_rhs[rhs_chi_shift].observe(
        custom_chi_shift, exact_chi_shift, "chi_shift_RHS", "chi", points,
        z);
    errors.grchombo_rhs[rhs_chi_shift].observe(
        grchombo_chi_shift, exact_chi_shift, "chi_shift_RHS", "chi",
        points, z);

    const auto exact_h_shift =
        visible_tensor_rows(combined_h_shift(exact_families));
    const auto custom_h_shift =
        visible_tensor_rows(combined_h_shift(custom_families));
    const auto exact_a_shift =
        visible_tensor_rows(combined_a_shift(exact_families));
    const auto custom_a_shift =
        visible_tensor_rows(combined_a_shift(custom_families));
    Matrix grchombo_h_shift_matrix{};
    Matrix grchombo_a_shift_matrix{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            grchombo_h_shift_matrix[i][j] =
                grchombo_rhs.h[i][j] -
                grchombo_rhs_without_shift.h[i][j];
            grchombo_a_shift_matrix[i][j] =
                grchombo_rhs.A[i][j] -
                grchombo_rhs_without_shift.A[i][j];
        }
    }
    const auto grchombo_h_shift =
        visible_tensor_rows(grchombo_h_shift_matrix);
    const auto grchombo_a_shift =
        visible_tensor_rows(grchombo_a_shift_matrix);
    for (std::size_t component = 0; component < visible_tensor_count;
         ++component)
    {
        const std::string name = visible_tensor_names[component];
        errors.custom_h_shift[component].observe(
            custom_h_shift[component], exact_h_shift[component],
            "h_shift_RHS", name, points, z);
        errors.grchombo_h_shift[component].observe(
            grchombo_h_shift[component], exact_h_shift[component],
            "h_shift_RHS", name, points, z);
        errors.custom_rhs[rhs_h_shift].observe(
            custom_h_shift[component], exact_h_shift[component],
            "h_shift_RHS", name, points, z);
        errors.grchombo_rhs[rhs_h_shift].observe(
            grchombo_h_shift[component], exact_h_shift[component],
            "h_shift_RHS", name, points, z);

        errors.custom_a_shift[component].observe(
            custom_a_shift[component], exact_a_shift[component],
            "A_shift_RHS", name, points, z);
        errors.grchombo_a_shift[component].observe(
            grchombo_a_shift[component], exact_a_shift[component],
            "A_shift_RHS", name, points, z);
        errors.custom_rhs[rhs_a_shift].observe(
            custom_a_shift[component], exact_a_shift[component],
            "A_shift_RHS", name, points, z);
        errors.grchombo_rhs[rhs_a_shift].observe(
            grchombo_a_shift[component], exact_a_shift[component],
            "A_shift_RHS", name, points, z);
    }

    const auto exact_rows = visible_rows(exact_rhs);
    const auto custom_rows = visible_rows(custom_rhs);
    const auto grchombo_rows = visible_rows(grchombo_rhs);
    for (int row = 0; row < visible_row_count; ++row)
    {
        errors.custom_rhs[rhs_combined].observe(
            custom_rows[static_cast<std::size_t>(row)],
            exact_rows[static_cast<std::size_t>(row)], "combined_rows",
            visible_row_names[static_cast<std::size_t>(row)], points, z);
        errors.grchombo_rhs[rhs_combined].observe(
            grchombo_rows[static_cast<std::size_t>(row)],
            exact_rows[static_cast<std::size_t>(row)], "combined_rows",
            visible_row_names[static_cast<std::size_t>(row)], points, z);
        errors.finite_rhs_difference.observe(
            custom_rows[static_cast<std::size_t>(row)],
            grchombo_rows[static_cast<std::size_t>(row)],
            "finite_difference", visible_row_names[static_cast<std::size_t>(
                                     row)],
            points, z);
    }
}

std::array<double, 4> solve4(
    std::array<std::array<double, 4>, 4> matrix,
    std::array<double, 4> right)
{
    for (int column = 0; column < 4; ++column)
    {
        int pivot = column;
        for (int row = column + 1; row < 4; ++row)
        {
            if (std::abs(matrix[row][column]) >
                std::abs(matrix[pivot][column]))
                pivot = row;
        }
        require(std::abs(matrix[pivot][column]) > 1.0e-30,
                "Richardson system is nonsingular");
        std::swap(matrix[pivot], matrix[column]);
        std::swap(right[pivot], right[column]);
        const double diagonal = matrix[column][column];
        for (int j = column; j < 4; ++j)
            matrix[column][j] /= diagonal;
        right[column] /= diagonal;
        for (int row = 0; row < 4; ++row)
        {
            if (row == column)
                continue;
            const double factor = matrix[row][column];
            for (int j = column; j < 4; ++j)
                matrix[row][j] -= factor * matrix[column][j];
            right[row] -= factor * right[column];
        }
    }
    return right;
}

double extrapolate(const std::array<double, 4> &values,
                   const std::array<int, 3> &powers)
{
    std::array<std::array<double, 4>, 4> matrix{};
    for (int level = 0; level < 4; ++level)
    {
        const double h =
            period / static_cast<double>(refinements[level]);
        matrix[level][0] = 1.0;
        for (int term = 0; term < 3; ++term)
            matrix[level][term + 1] =
                std::pow(h, powers[static_cast<std::size_t>(term)]);
    }
    return solve4(matrix, values)[0];
}

void run_negative_controls()
{
    const int points = 64;
    const double spacing = period / static_cast<double>(points);
    const double x = radial_origin + pi / 2.0;
    const double z = 0.0;
    const Jet exact = analytic_jet(f_chi, x, z);
    const Jet custom = custom_discrete_jet(f_chi, points, x, z);
    const Jet grchombo = grchombo_discrete_jet(f_chi, points, x, z);
    require(std::abs(-custom.dx - exact.dx) >
                20.0 * std::abs(custom.dx - exact.dx),
            "negative control derivative sign is detected");
    require(std::abs(0.5 * custom.dx - exact.dx) >
                20.0 * std::abs(custom.dx - exact.dx),
            "negative control grid-spacing factor is detected");
    require(std::abs(custom.dxx - exact.dxz) >
                20.0 * std::abs(custom.dxz - exact.dxz),
            "negative control mixed-derivative direction is detected");
    AnalyticInput ordered =
        manufactured_input(points, x, pi / 2.0, false, true);
    AnalyticInput wrong_mixed_slot = ordered;
    wrong_mixed_slot.d2.h[0][2][0][2] = 0.0;
    const GeometryPieces ordered_geometry =
        direct_geometry_pieces(ordered);
    const GeometryPieces wrong_mixed_geometry =
        direct_geometry_pieces(wrong_mixed_slot);
    require(
        matrix_max_difference(ordered_geometry.raw.tensor,
                              wrong_mixed_geometry.raw.tensor) > 1.0e-6,
        "negative control mixed-derivative ordering slot is detected");

    std::vector<double> periodic_values(static_cast<std::size_t>(points));
    for (int j = 0; j < points; ++j)
        periodic_values[static_cast<std::size_t>(j)] =
            analytic_jet(f_chi, x, j * spacing).value;
    const double clamped =
        (periodic_values[1] - periodic_values[0]) / (2.0 * spacing);
    require(std::abs(clamped - exact.dz) >
                20.0 * std::abs(custom.dz - exact.dz),
            "negative control periodic wrap is detected");
    require(std::abs(custom.dxx - exact.dxx) >
                20.0 * std::abs(grchombo.dxx - exact.dxx),
            "negative control wrong stencil order is detected");
    require(std::abs(-grchombo.dx - exact.dx) >
                20.0 * std::abs(grchombo.dx - exact.dx),
            "negative control mutating only GRChombo is detected");

    const AnalyticInput family_input =
        manufactured_input(points, x, pi / 2.0, false, true);
    const RHSFamilies families = custom_d3_families(family_input);
    const auto advection = advection_rows(families);
    const auto h_shift = visible_tensor_rows(combined_h_shift(families));
    const auto a_shift = visible_tensor_rows(combined_a_shift(families));
    const auto largest_nonzero = [](const auto &values,
                                    const std::size_t begin,
                                    const std::size_t end)
    {
        std::size_t selected = begin;
        for (std::size_t index = begin; index < end; ++index)
            if (std::abs(values[index]) > std::abs(values[selected]))
                selected = index;
        return values[selected];
    };
    const auto require_family_mutation =
        [](const double mutation, const double expected,
           const std::string &name)
    {
        require(std::abs(mutation - expected) > 1.0e-8,
                "negative control " + name + " is detected");
    };

    const double metric_advection =
        largest_nonzero(advection, 3, 9);
    const double a_advection =
        largest_nonzero(advection, 9, advection.size());
    require(std::abs(metric_advection) > 1.0e-6,
            "metric advection mutation uses active data");
    require(std::abs(a_advection) > 1.0e-6,
            "A advection mutation uses active data");
    require_family_mutation(0.0, metric_advection,
                            "metric-advection omission");
    require_family_mutation(2.0 * metric_advection, metric_advection,
                            "metric-advection duplication");
    require_family_mutation(-metric_advection, metric_advection,
                            "metric-advection sign");
    require_family_mutation(0.0, a_advection, "A-advection omission");
    require_family_mutation(2.0 * a_advection, a_advection,
                            "A-advection duplication");
    require_family_mutation(-a_advection, a_advection,
                            "A-advection sign");

    const double chi_shift = families.chi_shift_divergence;
    const double h_shift_value = largest_nonzero(h_shift, 0, h_shift.size());
    const double a_shift_value = largest_nonzero(a_shift, 0, a_shift.size());
    const std::array<std::pair<const char *, double>, 3> shift_groups = {
        {{"chi-shift", chi_shift},
         {"metric-shift", h_shift_value},
         {"A-shift", a_shift_value}}};
    for (const auto &group : shift_groups)
    {
        require(std::abs(group.second) > 1.0e-6,
                std::string(group.first) + " mutation uses active data");
        require_family_mutation(
            0.0, group.second, std::string(group.first) + " omission");
        require_family_mutation(
            2.0 * group.second, group.second,
            std::string(group.first) + " duplication");
        require_family_mutation(
            1.25 * group.second, group.second,
            std::string(group.first) + " coefficient");
        require_family_mutation(
            -group.second, group.second,
            std::string(group.first) + " sign");
    }
    std::cout << "PASS active derivative sign, spacing, mixed-direction, "
                 "mixed-ordering-slot, periodic-wrap, order, and one-path "
                 "mutations\n";
    std::cout << "PASS independent metric/A advection omission, duplication, "
                 "and sign mutations\n";
    std::cout << "PASS independent chi/metric/A shift-family omission, "
                 "duplication, coefficient, and sign mutations\n";
}

} // namespace

int main()
{
    const DirectCCZ4Access access(
        locked_kappa1, locked_kappa2, locked_kappa3, locked_covariant_z4,
        DirectCCZ4Access::USE_CCZ4);
    std::array<LevelErrors, refinements.size()> errors{};
    std::array<std::array<double, visible_row_count>, refinements.size()>
        custom_probe{};
    std::array<std::array<double, visible_row_count>, refinements.size()>
        grchombo_probe{};

    for (std::size_t level = 0; level < refinements.size(); ++level)
    {
        const int points = refinements[level];
        const double x = radial_origin + pi / 2.0;
        constexpr std::array<double, 4> z_points = {
            0.0, pi / 4.0, pi / 2.0, 3.0 * pi / 4.0};
        for (const double z : z_points)
        {
            observe_derivatives(errors[level], points, x, z);
            observe_rhs(errors[level], points, x, z, access);
        }
        const AnalyticInput custom =
            manufactured_input(points, x, pi / 2.0, false, false);
        const AnalyticInput grchombo =
            manufactured_input(points, x, pi / 2.0, true, false);
        custom_probe[level] =
            visible_rows(custom_total(custom_d3_families(custom)));
        grchombo_probe[level] = visible_rows(direct_rhs(grchombo, access));
    }

    constexpr std::array<const char *, 6> derivative_names = {
        "Dx", "Dz", "Dxx", "Dzz", "Dxz", "divergence"};
    constexpr std::array<const char *, rhs_family_count> rhs_names = {
        "raw_Ricci",
        "encoded_Z",
        "lapse_Hessian",
        "advection",
        "shift_input_derivatives",
        "chi_shift_RHS",
        "h_shift_RHS",
        "A_shift_RHS",
        "combined_rows"};
    std::cout << std::scientific << std::setprecision(8);
    for (std::size_t level = 0; level < refinements.size(); ++level)
    {
        for (std::size_t item = 0; item < derivative_names.size(); ++item)
        {
            std::cout << "RESULT derivative N=" << refinements[level]
                      << " class=" << derivative_names[item]
                      << " custom_error="
                      << errors[level].custom_derivative[item].error
                      << " grchombo_error="
                      << errors[level].grchombo_derivative[item].error
                      << " custom_worst="
                      << errors[level].custom_derivative[item].worst()
                      << " grchombo_worst="
                      << errors[level].grchombo_derivative[item].worst()
                      << "\n";
        }
        for (std::size_t item = 0; item < rhs_names.size(); ++item)
        {
            std::cout << "RESULT rhs N=" << refinements[level]
                      << " family=" << rhs_names[item]
                      << " custom_error="
                      << errors[level].custom_rhs[item].error
                      << " grchombo_error="
                      << errors[level].grchombo_rhs[item].error
                      << " custom_worst="
                      << errors[level].custom_rhs[item].worst()
                      << " grchombo_worst="
                      << errors[level].grchombo_rhs[item].worst() << "\n";
        }
        std::cout << "RESULT finite_difference N=" << refinements[level]
                  << " max="
                  << errors[level].finite_rhs_difference.error
                  << " worst=" << errors[level].finite_rhs_difference.worst()
                  << "\n";
        for (std::size_t row = 0; row < visible_row_count; ++row)
        {
            const auto &custom = errors[level].custom_advection[row];
            const auto &grchombo = errors[level].grchombo_advection[row];
            std::cout << "RESULT advection_row N=" << refinements[level]
                      << " row=" << visible_row_names[row]
                      << " custom_analytic=" << custom.expected
                      << " custom_discrete=" << custom.actual
                      << " custom_error=" << custom.error
                      << " custom_z=" << custom.physical_z
                      << " grchombo_analytic=" << grchombo.expected
                      << " grchombo_discrete=" << grchombo.actual
                      << " grchombo_error=" << grchombo.error
                      << " grchombo_z=" << grchombo.physical_z << "\n";
        }
        const auto print_shift =
            [&](const char *family, const char *component,
                const ErrorRecord &custom, const ErrorRecord &grchombo)
        {
            std::cout << "RESULT shift_rhs N=" << refinements[level]
                      << " family=" << family
                      << " component=" << component
                      << " custom_analytic=" << custom.expected
                      << " custom_discrete=" << custom.actual
                      << " custom_error=" << custom.error
                      << " custom_z=" << custom.physical_z
                      << " grchombo_analytic=" << grchombo.expected
                      << " grchombo_discrete=" << grchombo.actual
                      << " grchombo_error=" << grchombo.error
                      << " grchombo_z=" << grchombo.physical_z << "\n";
        };
        print_shift("chi", "chi", errors[level].custom_chi_shift,
                    errors[level].grchombo_chi_shift);
        for (std::size_t component = 0;
             component < visible_tensor_count; ++component)
        {
            print_shift("h", visible_tensor_names[component],
                        errors[level].custom_h_shift[component],
                        errors[level].grchombo_h_shift[component]);
            print_shift("A", visible_tensor_names[component],
                        errors[level].custom_a_shift[component],
                        errors[level].grchombo_a_shift[component]);
        }
    }

    for (std::size_t item = 0; item < derivative_names.size(); ++item)
    {
        const double custom_order =
            observed_order(errors.front().custom_derivative[item].error,
                           errors.back().custom_derivative[item].error) /
            3.0;
        const double grchombo_order =
            observed_order(errors.front().grchombo_derivative[item].error,
                           errors.back().grchombo_derivative[item].error) /
            3.0;
        std::cout << "RESULT derivative_order class="
                  << derivative_names[item]
                  << " custom=" << custom_order
                  << " grchombo=" << grchombo_order << "\n";
        require(std::abs(custom_order - 2.0) <= 0.25,
                std::string("custom ") + derivative_names[item] +
                    " achieves order two");
        require(std::abs(grchombo_order - 4.0) <= 0.25,
                std::string("GRChombo ") + derivative_names[item] +
                    " achieves order four");
    }
    for (std::size_t item = 0; item < rhs_names.size(); ++item)
    {
        const double custom_order =
            observed_order(errors.front().custom_rhs[item].error,
                           errors.back().custom_rhs[item].error) /
            3.0;
        const double grchombo_order =
            observed_order(errors.front().grchombo_rhs[item].error,
                           errors.back().grchombo_rhs[item].error) /
            3.0;
        std::cout << "RESULT rhs_order family=" << rhs_names[item]
                  << " custom=" << custom_order
                  << " grchombo=" << grchombo_order << "\n";
        require(std::abs(custom_order - 2.0) <= 0.25,
                std::string("custom ") + rhs_names[item] +
                    " achieves order two");
        require(std::abs(grchombo_order - 4.0) <= 0.25,
                std::string("GRChombo ") + rhs_names[item] +
                    " achieves order four");
    }
    for (std::size_t row = 0; row < visible_row_count; ++row)
    {
        const double custom_order =
            observed_order(errors.front().custom_advection[row].error,
                           errors.back().custom_advection[row].error) /
            3.0;
        const double grchombo_order =
            observed_order(errors.front().grchombo_advection[row].error,
                           errors.back().grchombo_advection[row].error) /
            3.0;
        std::cout << "RESULT advection_order row=" << visible_row_names[row]
                  << " custom=" << custom_order
                  << " grchombo=" << grchombo_order << "\n";
        require(std::abs(custom_order - 2.0) <= 0.25,
                std::string("custom advection ") + visible_row_names[row] +
                    " achieves order two");
        require(std::abs(grchombo_order - 4.0) <= 0.25,
                std::string("GRChombo advection ") +
                    visible_row_names[row] + " achieves order four");
    }
    const auto check_shift_order =
        [&](const char *family, const char *component,
            const ErrorRecord &custom_coarse, const ErrorRecord &custom_fine,
            const ErrorRecord &grchombo_coarse,
            const ErrorRecord &grchombo_fine)
    {
        const double custom_order =
            observed_order(custom_coarse.error, custom_fine.error) / 3.0;
        const double grchombo_order =
            observed_order(grchombo_coarse.error, grchombo_fine.error) / 3.0;
        std::cout << "RESULT shift_rhs_order family=" << family
                  << " component=" << component
                  << " custom=" << custom_order
                  << " grchombo=" << grchombo_order << "\n";
        require(std::abs(custom_order - 2.0) <= 0.25,
                std::string("custom ") + family + " shift " + component +
                    " achieves order two");
        require(std::abs(grchombo_order - 4.0) <= 0.25,
                std::string("GRChombo ") + family + " shift " + component +
                    " achieves order four");
    };
    check_shift_order(
        "chi", "chi", errors.front().custom_chi_shift,
        errors.back().custom_chi_shift, errors.front().grchombo_chi_shift,
        errors.back().grchombo_chi_shift);
    for (std::size_t component = 0; component < visible_tensor_count;
         ++component)
    {
        check_shift_order(
            "h", visible_tensor_names[component],
            errors.front().custom_h_shift[component],
            errors.back().custom_h_shift[component],
            errors.front().grchombo_h_shift[component],
            errors.back().grchombo_h_shift[component]);
        check_shift_order(
            "A", visible_tensor_names[component],
            errors.front().custom_a_shift[component],
            errors.back().custom_a_shift[component],
            errors.front().grchombo_a_shift[component],
            errors.back().grchombo_a_shift[component]);
    }

    ErrorRecord extrapolated_difference;
    ErrorRecord custom_extrapolated_analytic;
    ErrorRecord grchombo_extrapolated_analytic;
    const auto exact_probe = visible_rows(custom_total(custom_d3_families(
        manufactured_input(256, radial_origin + pi / 2.0, pi / 2.0,
                           false, true))));
    for (int row = 0; row < visible_row_count; ++row)
    {
        std::array<double, 4> custom_values{};
        std::array<double, 4> grchombo_values{};
        for (std::size_t level = 0; level < refinements.size(); ++level)
        {
            custom_values[level] =
                custom_probe[level][static_cast<std::size_t>(row)];
            grchombo_values[level] =
                grchombo_probe[level][static_cast<std::size_t>(row)];
        }
        const double custom_extrapolated =
            extrapolate(custom_values, {2, 4, 6});
        const double grchombo_extrapolated =
            extrapolate(grchombo_values, {4, 5, 6});
        const std::string label = "visible row " + std::to_string(row);
        std::cout << "RESULT extrapolated row=" << row
                  << " custom=" << custom_extrapolated
                  << " grchombo=" << grchombo_extrapolated
                  << " analytic="
                  << exact_probe[static_cast<std::size_t>(row)] << "\n";
        extrapolated_difference.observe(
            custom_extrapolated, grchombo_extrapolated, "extrapolated",
            visible_row_names[static_cast<std::size_t>(row)], 0,
            pi / 2.0);
        custom_extrapolated_analytic.observe(
            custom_extrapolated,
            exact_probe[static_cast<std::size_t>(row)], "extrapolated",
            visible_row_names[static_cast<std::size_t>(row)], 0,
            pi / 2.0);
        grchombo_extrapolated_analytic.observe(
            grchombo_extrapolated,
            exact_probe[static_cast<std::size_t>(row)], "extrapolated",
            visible_row_names[static_cast<std::size_t>(row)], 0,
            pi / 2.0);
        const double allowance =
            5.0e-11 +
            5.0e-10 *
                std::max(std::abs(custom_extrapolated),
                         std::abs(grchombo_extrapolated));
        require(std::abs(custom_extrapolated - grchombo_extrapolated) <=
                    allowance,
                label + " continuum extrapolations agree");
    }
    std::cout << "RESULT extrapolated custom_vs_grchombo_max="
              << extrapolated_difference.error
              << " worst=" << extrapolated_difference.worst()
              << " custom_vs_analytic_max="
              << custom_extrapolated_analytic.error
              << " grchombo_vs_analytic_max="
              << grchombo_extrapolated_analytic.error << "\n";

    run_negative_controls();
    std::cout << "PASS batch 3 direct derivative kernels and manufactured "
                 "visible-RHS convergence\n";
    return 0;
}
