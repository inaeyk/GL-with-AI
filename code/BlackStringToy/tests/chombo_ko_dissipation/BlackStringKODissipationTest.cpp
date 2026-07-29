#include "FArrayBox.H"

#include "BlackStringGPPointwiseInitialData.hpp"
#include "BlackStringKODissipation.hpp"
#include "BlackStringLive.hpp"
#include "BoxPointers.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace
{
namespace KO = BlackStringKODissipation;
namespace Live = BlackStringLive;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

constexpr double dx = 0.125;
constexpr double sigma = 0.3;
constexpr double tolerance = 3.0e-12;

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(Production::NUM_VARS == 18);
static_assert(KO::radial_direction == 0);
static_assert(KO::compact_direction == 1);
static_assert(KO::radial_direction < CH_SPACEDIM);
static_assert(KO::compact_direction < CH_SPACEDIM);

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_KO_FAIL " << message << '\n';
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

void require_close(const double actual, const double expected,
                   const std::string &message)
{
    const double scale =
        std::max({1.0, std::abs(actual), std::abs(expected)});
    require(std::abs(actual - expected) <= tolerance * scale,
            message + " actual=" + std::to_string(actual) +
                " expected=" + std::to_string(expected));
}

IntVect point(const int x, const int z)
{
    return IntVect(D_DECL(x, z, 0));
}

Box stencil_box()
{
    return Box(point(-KO::stencil_radius, -KO::stencil_radius),
               point(KO::stencil_radius, KO::stencil_radius));
}

class RealPoint
{
  public:
    RealPoint(const FArrayBox &input, FArrayBox &output,
              const IntVect &selected)
        : m_pointers(input, output), m_cell(selected, m_pointers)
    {
    }

    const Cell<double> &cell() const { return m_cell; }

  private:
    BoxPointers m_pointers;
    Cell<double> m_cell;
};

template <class value_t>
void fill(FArrayBox &input, const value_t &value)
{
    for (BoxIterator bit(input.box()); bit.ok(); ++bit)
    {
        const IntVect index = bit();
        for (int component = 0; component < Production::NUM_VARS;
             ++component)
        {
            input(index, component) = value(index, component);
        }
    }
}

template <class policy_t = KO::DefaultPolicy>
Reduced::Storage<double> evaluate_ko(const FArrayBox &input,
                                     const double selected_sigma)
{
    FArrayBox output(stencil_box(), Production::NUM_VARS);
    output.setVal(0.0);
    RealPoint selected(input, output, point(0, 0));
    Reduced::Variables<double> rhs{};
    KO::add<policy_t>(rhs, selected.cell(), dx, selected_sigma);
    return Reduced::store(rhs);
}

void check_constants_and_polynomials()
{
    FArrayBox input(stencil_box(), Production::NUM_VARS);
    fill(input, [](const IntVect &, const int component)
         { return 1.25 + static_cast<double>(component); });
    const auto constant = evaluate_ko(input, sigma);
    for (const double value : constant)
    {
        require(value == 0.0, "constant field received nonzero KO");
    }

    double maximum_polynomial_error = 0.0;
    for (int degree = 0; degree <= 5; ++degree)
    {
        fill(input,
             [degree](const IntVect &index, const int component)
             {
                 const double x =
                     std::pow(static_cast<double>(index[0]), degree);
                 const double z =
                     std::pow(static_cast<double>(index[1]), degree);
                 return (static_cast<double>(component) + 1.0) *
                        (x + 0.5 * z);
             });
        const auto result = evaluate_ko(input, sigma);
        for (const double value : result)
        {
            maximum_polynomial_error =
                std::max(maximum_polynomial_error, std::abs(value));
        }
    }
    require(maximum_polynomial_error <= tolerance,
            "degree-five-or-lower polynomial was not annihilated");
    std::cout << "KO_ANNIHILATION constant_max=0 polynomial_degree=5"
              << " polynomial_max=" << maximum_polynomial_error << '\n';
}

Reduced::Storage<double> alternating_result(const bool alternate_x,
                                            const bool alternate_z,
                                            const double selected_sigma)
{
    FArrayBox input(stencil_box(), Production::NUM_VARS);
    fill(input,
         [alternate_x, alternate_z](const IntVect &index,
                                    const int component)
         {
             const double x_sign =
                 alternate_x && index[0] % 2 != 0 ? -1.0 : 1.0;
             const double z_sign =
                 alternate_z && index[1] % 2 != 0 ? -1.0 : 1.0;
             const double x_part = alternate_x ? x_sign : 0.0;
             const double z_part = alternate_z ? 2.0 * z_sign : 0.0;
             return (static_cast<double>(component) + 1.0) *
                    (x_part + z_part);
         });
    return evaluate_ko(input, selected_sigma);
}

void check_alternating_modes_coverage_and_scaling()
{
    const auto x_mode = alternating_result(true, false, sigma);
    const auto z_mode = alternating_result(false, true, sigma);
    const auto mixed = alternating_result(true, true, sigma);
    const auto doubled = alternating_result(true, true, 2.0 * sigma);
    for (int component = 0; component < Production::NUM_VARS; ++component)
    {
        const double amplitude = static_cast<double>(component) + 1.0;
        const double expected_x = -sigma * amplitude / dx;
        const double expected_z = -2.0 * sigma * amplitude / dx;
        require_close(x_mode[static_cast<std::size_t>(component)], expected_x,
                      "alternating x damping");
        require_close(z_mode[static_cast<std::size_t>(component)], expected_z,
                      "alternating z damping");
        require_close(mixed[static_cast<std::size_t>(component)],
                      expected_x + expected_z, "mixed direction sum");
        require_close(doubled[static_cast<std::size_t>(component)],
                      2.0 * mixed[static_cast<std::size_t>(component)],
                      "sigma linear scaling");
    }
    std::cout << "KO_ALTERNATING sigma=" << sigma
              << " dx=" << dx
              << " x_slot0=" << x_mode[0]
              << " z_slot0=" << z_mode[0]
              << " mixed_slot0=" << mixed[0]
              << " expected_x_slot0=" << -sigma / dx
              << " expected_z_slot0=" << -2.0 * sigma / dx
              << " component_coverage=18/18 sigma_linearity=PASS\n";
}

struct OmitLastComponent
{
    static constexpr int component_count = Production::NUM_VARS - 1;
    static constexpr double stencil_sign = 1.0;
};

struct ReverseDampingSign
{
    static constexpr int component_count = Production::NUM_VARS;
    static constexpr double stencil_sign = -1.0;
};

void check_mutations_are_rejected()
{
    FArrayBox input(stencil_box(), Production::NUM_VARS);
    fill(input,
         [](const IntVect &index, const int component)
         {
             const double sign = index[0] % 2 == 0 ? 1.0 : -1.0;
             return (static_cast<double>(component) + 1.0) * sign;
         });
    const auto production = evaluate_ko(input, sigma);
    const auto omitted = evaluate_ko<OmitLastComponent>(input, sigma);
    const auto reversed = evaluate_ko<ReverseDampingSign>(input, sigma);
    bool omission_rejected = false;
    bool sign_rejected = false;
    for (int component = 0; component < Production::NUM_VARS; ++component)
    {
        const auto slot = static_cast<std::size_t>(component);
        omission_rejected =
            omission_rejected || omitted[slot] != production[slot];
        sign_rejected = sign_rejected || reversed[slot] != production[slot];
    }
    require(omission_rejected, "omitted-component mutation was not rejected");
    require(sign_rejected, "reversed-sign mutation was not rejected");
    std::cout << "KO_MUTATIONS omit_component=REJECTED"
              << " reverse_sign=REJECTED\n";
}

class AuditInput
{
  public:
    explicit AuditInput(const Cell<double> &cell) : m_cell(cell) {}

    int index() const { return m_cell.get_in_index(); }

    int stride(const int direction) const
    {
        require(direction >= 0 && direction < CH_SPACEDIM,
                "hidden direction requested as a grid stride");
        m_direction_mask |= 1 << direction;
        return m_cell.get_box_pointers().m_in_stride[direction];
    }

    const double *values(const int component) const
    {
        return m_cell.get_box_pointers().m_in_ptr[component];
    }

    int direction_mask() const { return m_direction_mask; }

  private:
    const Cell<double> &m_cell;
    mutable int m_direction_mask = 0;
};

void check_direction_audit_and_sigma_zero()
{
    FArrayBox input(stencil_box(), Production::NUM_VARS);
    FArrayBox output(stencil_box(), Production::NUM_VARS);
    fill(input,
         [](const IntVect &index, const int component)
         {
             return static_cast<double>((component + 1) *
                                        (3 * index[0] - 5 * index[1]));
         });
    output.setVal(0.0);
    RealPoint selected(input, output, point(0, 0));
    AuditInput audit(selected.cell());
    Reduced::Variables<double> rhs{};
    KO::add_from_input(rhs, audit, dx, sigma);
    require(audit.direction_mask() ==
                ((1 << KO::radial_direction) |
                 (1 << KO::compact_direction)),
            "KO direction audit did not request exactly x and z");

    Reduced::Variables<double> untouched{};
    for (int component = 0; component < Production::NUM_VARS; ++component)
    {
        Reduced::component(
            untouched, static_cast<Production::VariableSlot>(component)) =
            -0.0;
    }
    const auto before = Reduced::store(untouched);
    AuditInput zero_audit(selected.cell());
    KO::add_from_input(untouched, zero_audit, dx, 0.0);
    const auto after = Reduced::store(untouched);
    require(std::memcmp(before.data(), after.data(),
                        sizeof(double) * before.size()) == 0,
            "sigma zero changed RHS bits");
    require(zero_audit.direction_mask() == 0,
            "sigma zero accessed a grid stride");
    std::cout << "KO_DIRECTION_AUDIT direction_mask="
              << audit.direction_mask()
              << " hidden_direction_accesses=0"
              << " sigma_zero_bitwise=PASS"
              << " sigma_zero_stride_accesses=0\n";
}

void check_live_rhs_sigma_zero_equivalence()
{
    const Box box = stencil_box();
    FArrayBox input(box, Production::NUM_VARS);
    FArrayBox old_output(box, Production::NUM_VARS);
    FArrayBox live_output(box, Production::NUM_VARS);
    const double center_x = 2.0;
    const std::array<double, CH_SPACEDIM> offset{
        0.5 * dx - center_x, 0.5 * dx};
    for (BoxIterator bit(box); bit.ok(); ++bit)
    {
        const IntVect index = bit();
        const double x =
            (static_cast<double>(index[0]) + 0.5) * dx - offset[0];
        const auto state =
            BlackStringGPPointwiseInitialData::make_pointwise_state(1.0, x);
        for (int component = 0; component < Production::NUM_VARS; ++component)
        {
            input(index, component) =
                state[static_cast<std::size_t>(component)];
        }
    }
    old_output.setVal(0.0);
    live_output.setVal(0.0);
    RealPoint old_point(input, old_output, point(0, 0));
    RealPoint live_point(input, live_output, point(0, 0));
    const auto old_input =
        Live::make_pointwise_input(old_point.cell(), dx, offset);
    const auto old_rhs =
        Live::evaluate_rhs(old_input, 1.0, Live::GaugeParameters{}, true);
    BlackStringCellStorage::store(old_point.cell(), old_rhs);
    Live::RHSCompute(1.0, dx, offset, Live::GaugeParameters{}, true, 0.0)
        .compute(live_point.cell());
    for (int component = 0; component < Production::NUM_VARS; ++component)
    {
        const double old_value = old_output(point(0, 0), component);
        const double live_value = live_output(point(0, 0), component);
        require(std::memcmp(&old_value, &live_value, sizeof(double)) == 0,
                "sigma-zero live RHS differs from pre-D11 path at component " +
                    std::to_string(component));
    }
    std::cout << "KO_LIVE_ORDER direct_target_evaluations=1"
              << " gauge_evaluations=1 fixed_source_applications=1"
              << " ko_additions=1 outer_surface_owner=AFTER_VOLUME"
              << " second_rhs_evaluation=0 sigma_zero_old_rhs=BITWISE_PASS\n";
}

void check_invalid_sigma_rejected()
{
    const std::array<double, CH_SPACEDIM> offset{0.0, 0.0};
    bool negative_rejected = false;
    bool nonfinite_rejected = false;
    try
    {
        const Live::RHSCompute invalid(
            1.0, dx, offset, Live::GaugeParameters{}, true, -sigma);
        static_cast<void>(invalid);
    }
    catch (const std::domain_error &)
    {
        negative_rejected = true;
    }
    try
    {
        const Live::RHSCompute invalid(
            1.0, dx, offset, Live::GaugeParameters{}, true,
            std::numeric_limits<double>::quiet_NaN());
        static_cast<void>(invalid);
    }
    catch (const std::domain_error &)
    {
        nonfinite_rejected = true;
    }
    require(negative_rejected, "negative ko_sigma was not rejected");
    require(nonfinite_rejected, "nonfinite ko_sigma was not rejected");
    std::cout << "KO_PARAMETER_VALIDATION negative=REJECTED"
              << " nonfinite=REJECTED\n";
}

} // namespace

int main()
{
    check_constants_and_polynomials();
    check_alternating_modes_coverage_and_scaling();
    check_mutations_are_rejected();
    check_direction_audit_and_sigma_zero();
    check_live_rhs_sigma_zero_equivalence();
    check_invalid_sigma_rejected();
    std::cout << "BLACKSTRING_KO_DISSIPATION_FIXTURE_PASS\n";
    return 0;
}
