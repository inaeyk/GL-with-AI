#include "CartoonAwayAxisPhysicalRww.hpp"
#include "CartoonRicciBridge.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace
{
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace Physical = BlackStringToy::CartoonAwayAxisPhysicalRww;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;

constexpr double tolerance = 1.0e-12;
constexpr double ricci_tolerance = 1.0e-10;

// Stage 4AH assembles a local away-axis physical R_ww value only. Synthetic
// Stage 4AG fixtures do not mean actual grid h_xz parity has been validated.
// Grid h_xx-h_ww matching, W_x and chi_x regularity, finite-axis evaluation,
// CCZ4 RHS, grid reads, and evolution wiring remain open prerequisites.
struct Sample
{
    double x;
    Algebra::ConformalMetric h;
    Ricci::MetricDerivatives h_derivatives;
    double chi;
    Ricci::ScalarDerivatives chi_derivatives;
};

static_assert(!std::is_aggregate<Physical::AwayAxisPhysicalRwwInputs>::value,
              "Stage 4AH inputs must not be an open aggregate");
static_assert(!std::is_aggregate<Physical::AwayAxisPhysicalRww>::value,
              "Stage 4AH result must not be an open aggregate");
static_assert(Physical::away_axis_only, "Stage 4AH must remain away-axis only");
static_assert(!Physical::finite_axis_implemented,
              "Stage 4AH must not claim finite-axis evaluation");
static_assert(!Physical::grid_hxz_parity_validated,
              "Stage 4AG synthetic data is not grid validation");
static_assert(!Physical::grid_hxx_hww_matching_validated,
              "Stage 4AH must not claim grid diagonal matching");
static_assert(!Physical::grid_Wx_parity_validated,
              "Stage 4AH must not claim grid W_x parity");
static_assert(!Physical::grid_chix_parity_validated,
              "Stage 4AH must not claim grid chi_x parity");
static_assert(!Physical::ccz4_rhs_implemented,
              "Stage 4AH must not claim CCZ4 RHS implementation");
static_assert(!Physical::evolution_wiring_implemented,
              "Stage 4AH must not claim evolution wiring");

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected, const double tol = tolerance)
{
    const double residual = std::abs(value - expected);
    if (residual > tol)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected) + ", residual " +
                        std::to_string(residual));
    }
    std::cout << "PASS " << label << " = " << value
              << ", residual = " << residual << "\n";
}

void require_throws(const std::string &label, const std::function<void()> &fn)
{
    try
    {
        fn();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    fail(label, "expected std::domain_error");
}

Physical::AwayAxisPhysicalRwwInputs make_inputs(const Sample &sample)
{
    return Physical::make_away_axis_physical_rww_inputs(
        sample.x, sample.h.xx, sample.h.xz, sample.h.zz, sample.h.ww,
        sample.chi, sample.h_derivatives.dx.xx,
        sample.h_derivatives.dz.xx, sample.h_derivatives.dx.xz,
        sample.h_derivatives.dz.xz, sample.h_derivatives.dx.zz,
        sample.h_derivatives.dz.zz, sample.h_derivatives.dx.ww,
        sample.h_derivatives.dz.ww, sample.h_derivatives.dxx.ww,
        sample.h_derivatives.dxz.ww, sample.h_derivatives.dzz.ww,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dz,
        sample.chi_derivatives.second.dxx,
        sample.chi_derivatives.second.dxz,
        sample.chi_derivatives.second.dzz);
}

Physical::AwayAxisPhysicalRww compute(const Sample &sample)
{
    return Physical::compute_away_axis_physical_rww(make_inputs(sample));
}

Sample flat_sample(const double chi = 1.0)
{
    Sample sample{};
    sample.x = 2.0;
    sample.h = {1.0, 0.0, 1.0, 1.0};
    sample.chi = chi;
    return sample;
}

Sample constant_cone_sample()
{
    auto sample = flat_sample();
    sample.h.ww = 4.0;
    return sample;
}

Sample nonsymmetric_sample()
{
    Sample sample{};
    sample.x = 2.0;
    sample.h = {7.0, 2.0, 5.0, 3.0};
    sample.h_derivatives.dx = {3.0, 5.0, 7.0, 6.0};
    sample.h_derivatives.dz = {4.0, 6.0, 8.0, 4.0};
    sample.h_derivatives.dxx = {0.0, 0.0, 0.0, 31.0};
    sample.h_derivatives.dxz = {0.0, 0.0, 0.0, 37.0};
    sample.h_derivatives.dzz = {0.0, 0.0, 0.0, 41.0};
    sample.chi = 5.0;
    sample.chi_derivatives.first = {3.0, 2.0};
    sample.chi_derivatives.second = {7.0, 11.0, 13.0};
    return sample;
}

Algebra::ConformalMetric scale(const Algebra::ConformalMetric &value,
                               const double factor)
{
    return {factor * value.xx, factor * value.xz, factor * value.zz,
            factor * value.ww};
}

Algebra::ConformalMetric add(const Algebra::ConformalMetric &left,
                             const Algebra::ConformalMetric &right)
{
    return {left.xx + right.xx, left.xz + right.xz, left.zz + right.zz,
            left.ww + right.ww};
}

Algebra::ConformalMetric physical_first_derivative(
    const Algebra::ConformalMetric &h, const Algebra::ConformalMetric &dh,
    const double chi, const double dchi)
{
    const double inv_chi = 1.0 / chi;
    return add(scale(dh, inv_chi),
               scale(h, -dchi * inv_chi * inv_chi));
}

Algebra::ConformalMetric physical_second_derivative(
    const Algebra::ConformalMetric &h, const Algebra::ConformalMetric &dh_a,
    const Algebra::ConformalMetric &dh_b,
    const Algebra::ConformalMetric &d2h_ab, const double chi,
    const double dchi_a, const double dchi_b, const double d2chi_ab)
{
    const double inv_chi = 1.0 / chi;
    const double inv_chi2 = inv_chi * inv_chi;
    const double inv_chi3 = inv_chi2 * inv_chi;
    auto result = scale(d2h_ab, inv_chi);
    result = add(result, scale(dh_a, -dchi_b * inv_chi2));
    result = add(result, scale(dh_b, -dchi_a * inv_chi2));
    result = add(result, scale(h, -d2chi_ab * inv_chi2));
    return add(result, scale(h, 2.0 * dchi_a * dchi_b * inv_chi3));
}

double direct_physical_rww(const Sample &sample)
{
    Ricci::CartoonRicciInputs inputs{};
    inputs.x = sample.x;
    inputs.chi = 1.0;
    inputs.h = scale(sample.h, 1.0 / sample.chi);
    inputs.h_derivatives.dx = physical_first_derivative(
        sample.h, sample.h_derivatives.dx, sample.chi,
        sample.chi_derivatives.first.dx);
    inputs.h_derivatives.dz = physical_first_derivative(
        sample.h, sample.h_derivatives.dz, sample.chi,
        sample.chi_derivatives.first.dz);
    inputs.h_derivatives.dxx = physical_second_derivative(
        sample.h, sample.h_derivatives.dx, sample.h_derivatives.dx,
        sample.h_derivatives.dxx, sample.chi,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dx,
        sample.chi_derivatives.second.dxx);
    inputs.h_derivatives.dxz = physical_second_derivative(
        sample.h, sample.h_derivatives.dx, sample.h_derivatives.dz,
        sample.h_derivatives.dxz, sample.chi,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dz,
        sample.chi_derivatives.second.dxz);
    inputs.h_derivatives.dzz = physical_second_derivative(
        sample.h, sample.h_derivatives.dz, sample.h_derivatives.dz,
        sample.h_derivatives.dzz, sample.chi,
        sample.chi_derivatives.first.dz, sample.chi_derivatives.first.dz,
        sample.chi_derivatives.second.dzz);

    return RicciBridge::to_rhs_ricci_components(
               Ricci::compute_metric_derivative_ricci(inputs))
        .ww;
}

void check_simple_oracles()
{
    const auto flat = compute(flat_sample());
    require_close("flat conformal part", flat.conformal_part(), 0.0);
    require_close("flat conformal-factor part", flat.conformal_factor_part(),
                  0.0);
    require_close("flat physical R_ww", flat.physical_rww(), 0.0);
    require_close("flat chi", flat.chi(), 1.0);
    require_close("flat conformal hww inverse",
                  flat.conformal_hww_inverse(), 1.0);

    const auto cone = compute(constant_cone_sample());
    require_close("constant cone physical R_ww", cone.physical_rww(), -0.75);
    require_close("constant cone conformal hww inverse",
                  cone.conformal_hww_inverse(), 0.25);

    auto linear_x = flat_sample(1.2);
    linear_x.chi_derivatives.first.dx = 0.1;
    const auto linear_x_result = compute(linear_x);
    require_close("linear-x chi physical R_ww",
                  linear_x_result.physical_rww(), 11.0 / 144.0);
    require_close("linear-x chi result", linear_x_result.chi(), 1.2);

    auto linear_z = flat_sample(1.6);
    linear_z.chi_derivatives.first.dz = 0.2;
    require_close("linear-z chi physical R_ww", compute(linear_z).physical_rww(),
                  -1.0 / 64.0);
}

void check_nonsymmetric_oracle()
{
    const auto sample = nonsymmetric_sample();
    const auto result = compute(sample);
    require_close("nonsymmetric assembled sum", result.physical_rww(),
                  result.conformal_part() + result.conformal_factor_part());
    require_close("nonsymmetric Stage 4G direct comparison",
                  result.physical_rww(), direct_physical_rww(sample),
                  ricci_tolerance);
}

void check_invalid_inputs_reject()
{
    auto sample = nonsymmetric_sample();
    sample.x = 0.0;
    require_throws("invalid axis", [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.chi = 0.0;
    require_throws("invalid chi", [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.h.ww = 0.0;
    require_throws("invalid h_ww", [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.h = {1.0, 2.0, 1.0, 3.0};
    require_throws("negative determinant", [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.h_derivatives.dxx.ww =
        std::numeric_limits<double>::infinity();
    require_throws("nonfinite W second derivative",
                   [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.chi_derivatives.second.dxz =
        std::numeric_limits<double>::quiet_NaN();
    require_throws("nonfinite chi second derivative",
                   [&]() { make_inputs(sample); });
}

} // namespace

int main()
{
    std::cout << "Stage 4AH local away-axis physical Rww assembly fixture\n";
    std::cout << "No finite-axis evaluation, grid validation, RHS, grid reads, "
                 "or evolution wiring.\n";

    check_simple_oracles();
    check_nonsymmetric_oracle();
    check_invalid_inputs_reject();

    std::cout << "All Stage 4AH away-axis physical Rww checks passed.\n";
    return 0;
}
