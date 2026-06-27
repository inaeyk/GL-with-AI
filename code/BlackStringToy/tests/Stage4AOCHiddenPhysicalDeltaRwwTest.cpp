#include "CartoonAwayAxisPhysicalRww.hpp"
#include "CartoonRicciBridge.hpp"
#include "Stage4AOFrozenGaugeHiddenRww.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace
{
namespace Hidden = BlackStringToy::Stage4AOFrozenGaugeHiddenRww;
namespace PhysicalRww = BlackStringToy::CartoonAwayAxisPhysicalRww;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;

using Metric = BlackStringToy::ConformalCartoonAlgebra::ConformalMetric;

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_true(const std::string &label, const bool condition)
{
    if (!condition)
    {
        fail(label, "condition was false");
    }
    std::cout << "PASS " << label << "\n";
}

void require_close(const std::string &label, const double actual,
                   const double expected, const double tolerance)
{
    const double error = std::abs(actual - expected);
    if (error > tolerance)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " error=" + std::to_string(error) +
                        " tolerance=" + std::to_string(tolerance));
    }
}

Metric metric(const double xx, const double xz, const double zz,
              const double ww)
{
    return {xx, xz, zz, ww};
}

Metric zero_metric() { return metric(0.0, 0.0, 0.0, 0.0); }

Ricci::MetricDerivatives zero_metric_derivatives()
{
    return {zero_metric(), zero_metric(), zero_metric(), zero_metric(),
            zero_metric()};
}

Ricci::ScalarDerivatives zero_scalar_derivatives()
{
    return {{0.0, 0.0}, {0.0, 0.0, 0.0}};
}

Hidden::HiddenRwwPerturbation make_hidden_mode(
    const double x, const Metric &delta_h,
    const Ricci::MetricDerivatives &delta_h_derivatives,
    const double delta_chi,
    const Ricci::ScalarDerivatives &delta_chi_derivatives)
{
    return Hidden::make_hidden_rww_perturbation(
        x, delta_h, delta_h_derivatives, delta_chi,
        delta_chi_derivatives);
}

Ricci::CartoonRicciInputs make_stage4g_inputs(
    const Hidden::HiddenRwwPerturbation &perturbation, const double epsilon)
{
    const auto &h = perturbation.delta_h();
    const auto &dh = perturbation.delta_h_derivatives();
    const auto &dchi = perturbation.delta_chi_derivatives();

    Ricci::CartoonRicciInputs inputs{};
    inputs.x = perturbation.x();
    inputs.chi = 1.0 + epsilon * perturbation.delta_chi();
    inputs.h = metric(1.0 + epsilon * h.xx, epsilon * h.xz,
                      1.0 + epsilon * h.zz, 1.0 + epsilon * h.ww);
    inputs.h_derivatives = {
        metric(epsilon * dh.dx.xx, epsilon * dh.dx.xz,
               epsilon * dh.dx.zz, epsilon * dh.dx.ww),
        metric(epsilon * dh.dz.xx, epsilon * dh.dz.xz,
               epsilon * dh.dz.zz, epsilon * dh.dz.ww),
        metric(epsilon * dh.dxx.xx, epsilon * dh.dxx.xz,
               epsilon * dh.dxx.zz, epsilon * dh.dxx.ww),
        metric(epsilon * dh.dxz.xx, epsilon * dh.dxz.xz,
               epsilon * dh.dxz.zz, epsilon * dh.dxz.ww),
        metric(epsilon * dh.dzz.xx, epsilon * dh.dzz.xz,
               epsilon * dh.dzz.zz, epsilon * dh.dzz.ww)};
    inputs.chi_derivatives = {
        {epsilon * dchi.first.dx, epsilon * dchi.first.dz},
        {epsilon * dchi.second.dxx, epsilon * dchi.second.dxz,
         epsilon * dchi.second.dzz}};
    return inputs;
}

double direct_stage4g_rww(const Hidden::HiddenRwwPerturbation &perturbation,
                          const double epsilon)
{
    const auto ricci =
        Ricci::compute_metric_derivative_ricci(make_stage4g_inputs(perturbation,
                                                                    epsilon));
    return RicciBridge::to_rhs_ricci_components(ricci).ww;
}

double split_stage4ah_rww(const Hidden::HiddenRwwPerturbation &perturbation,
                          const double epsilon)
{
    const auto &h = perturbation.delta_h();
    const auto &dh = perturbation.delta_h_derivatives();
    const auto &dchi = perturbation.delta_chi_derivatives();
    const double chi = 1.0 + epsilon * perturbation.delta_chi();

    const auto inputs = PhysicalRww::make_away_axis_physical_rww_inputs(
        perturbation.x(), 1.0 + epsilon * h.xx, epsilon * h.xz,
        1.0 + epsilon * h.zz, 1.0 + epsilon * h.ww, chi,
        epsilon * dh.dx.xx, epsilon * dh.dz.xx, epsilon * dh.dx.xz,
        epsilon * dh.dz.xz, epsilon * dh.dx.zz, epsilon * dh.dz.zz,
        epsilon * dh.dx.ww, epsilon * dh.dz.ww, epsilon * dh.dxx.ww,
        epsilon * dh.dxz.ww, epsilon * dh.dzz.ww,
        epsilon * dchi.first.dx, epsilon * dchi.first.dz,
        epsilon * dchi.second.dxx, epsilon * dchi.second.dxz,
        epsilon * dchi.second.dzz);
    return PhysicalRww::compute_away_axis_physical_rww(inputs).physical_rww();
}

double central_difference_direct(
    const Hidden::HiddenRwwPerturbation &perturbation, const double epsilon)
{
    return (direct_stage4g_rww(perturbation, epsilon) -
            direct_stage4g_rww(perturbation, -epsilon)) /
           (2.0 * epsilon);
}

double central_difference_split(
    const Hidden::HiddenRwwPerturbation &perturbation, const double epsilon)
{
    return (split_stage4ah_rww(perturbation, epsilon) -
            split_stage4ah_rww(perturbation, -epsilon)) /
           (2.0 * epsilon);
}

void check_oracle_plateau(const std::string &label,
                          const Hidden::HiddenRwwPerturbation &perturbation,
                          const double expected)
{
    const auto linear = Hidden::compute_hidden_physical_delta_rww(perturbation);
    require_close(label + " analytic formula", linear.delta_rww(), expected,
                  1.0e-12);

    const std::array<double, 5> epsilons = {1.0e-2, 1.0e-4, 1.0e-5,
                                           1.0e-6, 1.0e-7};
    double middle_direct_max = 0.0;
    double middle_split_max = 0.0;
    for (const double epsilon : epsilons)
    {
        const double direct = central_difference_direct(perturbation, epsilon);
        const double split = central_difference_split(perturbation, epsilon);
        const double direct_error = std::abs(direct - linear.delta_rww());
        const double split_error = std::abs(split - linear.delta_rww());
        std::cout << "INFO " << label << " eps=" << epsilon
                  << " direct_error=" << direct_error
                  << " split_error=" << split_error << "\n";
        if (epsilon == 1.0e-5 || epsilon == 1.0e-6)
        {
            middle_direct_max = std::max(middle_direct_max, direct_error);
            middle_split_max = std::max(middle_split_max, split_error);
        }
    }

    require_close(label + " Stage 4G epsilon plateau", middle_direct_max, 0.0,
                  1.0e-7);
    require_close(label + " Stage 4AH split epsilon plateau",
                  middle_split_max, 0.0, 1.0e-7);
}

Hidden::HiddenRwwPerturbation pure_hidden_metric_mode(const double x)
{
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.ww = 2.0 * x;
    derivatives.dxx.ww = 2.0;
    return make_hidden_mode(x, metric(0.0, 0.0, 0.0, x * x), derivatives,
                            0.0, zero_scalar_derivatives());
}

Hidden::HiddenRwwPerturbation pure_chi_mode(const double x)
{
    auto chi_derivatives = zero_scalar_derivatives();
    chi_derivatives.first.dx = 1.0;
    return make_hidden_mode(x, zero_metric(), zero_metric_derivatives(), x,
                            chi_derivatives);
}

Hidden::HiddenRwwPerturbation z_even_hidden_metric_mode(const double x,
                                                        const double z,
                                                        const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dz.ww = -k * s;
    derivatives.dzz.ww = -k * k * c;
    return make_hidden_mode(x, metric(0.0, 0.0, 0.0, c), derivatives, 0.0,
                            zero_scalar_derivatives());
}

Hidden::HiddenRwwPerturbation radial_visible_metric_mode(const double x)
{
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.xx = 2.0 * x;
    derivatives.dxx.xx = 2.0;
    derivatives.dx.zz = 1.0;
    return make_hidden_mode(x, metric(x * x, 0.0, x, 0.0), derivatives, 0.0,
                            zero_scalar_derivatives());
}

Hidden::HiddenRwwPerturbation mixed_radial_z_offdiagonal_mode(
    const double x, const double z, const double k)
{
    const double s = std::sin(k * z);
    const double c = std::cos(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.xz = s;
    derivatives.dz.xz = x * k * c;
    derivatives.dxz.xz = k * c;
    derivatives.dzz.xz = -x * k * k * s;
    return make_hidden_mode(x, metric(0.0, x * s, 0.0, 0.0), derivatives,
                            0.0, zero_scalar_derivatives());
}

void check_zero_mode()
{
    const auto zero =
        make_hidden_mode(2.0, zero_metric(), zero_metric_derivatives(), 0.0,
                         zero_scalar_derivatives());
    check_oracle_plateau("zero perturbation", zero, 0.0);
}

void check_required_perturbations()
{
    check_oracle_plateau("pure delta hww radial mode",
                         pure_hidden_metric_mode(2.0), -6.0);
    check_oracle_plateau("pure delta chi radial mode", pure_chi_mode(2.0),
                         1.0);

    const double x = 2.0;
    const double z = 0.4;
    const double k = 3.0;
    const double c = std::cos(k * z);
    check_oracle_plateau("z-dependent scalar hidden mode",
                         z_even_hidden_metric_mode(x, z, k),
                         -c / (x * x) + 0.5 * k * k * c);
    const auto plus = Hidden::compute_hidden_physical_delta_rww(
        z_even_hidden_metric_mode(x, z, k));
    const auto minus = Hidden::compute_hidden_physical_delta_rww(
        z_even_hidden_metric_mode(x, -z, k));
    require_close("scalar/even z parity compatibility", plus.delta_rww(),
                  minus.delta_rww(), 1.0e-12);

    check_oracle_plateau("radial visible metric mode",
                         radial_visible_metric_mode(2.0), 1.75);

    const double z_mixed = 0.37;
    const double k_mixed = 2.5;
    check_oracle_plateau("mixed radial-z offdiagonal mode",
                         mixed_radial_z_offdiagonal_mode(2.0, z_mixed,
                                                         k_mixed),
                         k_mixed * std::cos(k_mixed * z_mixed));
}

void check_negative_guards()
{
    const auto hww = pure_hidden_metric_mode(2.0);
    const auto hww_linear = Hidden::compute_hidden_physical_delta_rww(hww);
    const auto &h = hww.delta_h();
    const auto &dh = hww.delta_h_derivatives();
    const double x = hww.x();
    const double wrong_without_one_over_x =
        -0.5 * (dh.dxx.ww + dh.dzz.ww);
    require_true("dropping hidden 1/x terms would fail",
                 std::abs(hww_linear.delta_rww() -
                          wrong_without_one_over_x) > 1.0e-3);
    require_true("treating raw hidden Rww as trace-free would fail",
                 std::abs(hww_linear.delta_rww() -
                          0.5 * hww_linear.delta_rww()) > 1.0e-3);
    require_true("dropping (h_xx-h_ww)/x^2 would fail",
                 std::abs(hww_linear.delta_rww() -
                          ((h.xx) / (x * x) - 2.0 * dh.dx.ww / x -
                           0.5 * (dh.dxx.ww + dh.dzz.ww))) > 1.0e-3);

    const auto chi = pure_chi_mode(2.0);
    const auto &dchi = chi.delta_chi_derivatives();
    const double wrong_hidden_multiplicity =
        1.5 * dchi.first.dx / chi.x() +
        0.5 * (dchi.second.dxx + dchi.second.dzz);
    require_true("wrong conformal-factor hidden multiplicity would fail",
                 std::abs(Hidden::compute_hidden_physical_delta_rww(chi)
                              .delta_rww() -
                          wrong_hidden_multiplicity) > 1.0e-3);

    const auto mixed = mixed_radial_z_offdiagonal_mode(2.0, 0.37, 2.5);
    const auto &mixed_derivs = mixed.delta_h_derivatives();
    const double wrong_derivative_sign =
        (mixed.delta_h().xx - mixed.delta_h().ww) / (mixed.x() * mixed.x()) +
        (mixed_derivs.dx.xx - 2.0 * mixed_derivs.dz.xz -
         mixed_derivs.dx.zz) /
            (2.0 * mixed.x()) -
        2.0 * mixed_derivs.dx.ww / mixed.x() -
        0.5 * (mixed_derivs.dxx.ww + mixed_derivs.dzz.ww);
    require_true("wrong offdiagonal z-derivative sign would fail",
                 std::abs(Hidden::compute_hidden_physical_delta_rww(mixed)
                              .delta_rww() -
                          wrong_derivative_sign) > 1.0e-3);
}

void check_scope_flags_and_validation()
{
    static_assert(!std::is_aggregate<Hidden::HiddenRwwPerturbation>::value,
                  "Stage 4AO-C hidden Rww perturbation package is non-aggregate");
    static_assert(!std::is_aggregate<Hidden::HiddenPhysicalDeltaRww>::value,
                  "Stage 4AO-C hidden Rww result package is non-aggregate");

    require_true("hidden physical delta Rww is implemented",
                 Hidden::hidden_physical_delta_rww_implemented);
    require_true("visible Ricci remains unimplemented",
                 !Hidden::visible_ricci_implemented);
    require_true("trace-free A curvature source remains unimplemented",
                 !Hidden::trace_free_a_curvature_source_implemented);
    require_true("Theta Ricci scalar remains unimplemented",
                 !Hidden::theta_ricci_scalar_implemented);
    require_true("full frozen-gauge operator remains unimplemented",
                 !Hidden::full_frozen_gauge_operator_implemented);
    require_true("eigensolver remains unimplemented",
                 !Hidden::eigensolver_implemented);
    require_true("production RHS wiring remains unimplemented",
                 !Hidden::production_rhs_wiring_implemented);

    bool threw = false;
    try
    {
        auto bad_derivatives = zero_metric_derivatives();
        bad_derivatives.dx.ww = std::numeric_limits<double>::infinity();
        (void)make_hidden_mode(2.0, zero_metric(), bad_derivatives, 0.0,
                               zero_scalar_derivatives());
    }
    catch (const std::domain_error &)
    {
        threw = true;
    }
    require_true("nonfinite perturbation derivatives are rejected", threw);
}

} // namespace

int main()
{
    check_zero_mode();
    check_required_perturbations();
    check_negative_guards();
    check_scope_flags_and_validation();
    std::cout << "Stage 4AO-C hidden physical delta Rww tests passed\n";
    return 0;
}
