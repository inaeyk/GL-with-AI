#include "CartoonRicciBridge.hpp"
#include "Stage4AOFrozenGaugeVisibleRxz.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace
{
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;
namespace Visible = BlackStringToy::Stage4AOFrozenGaugeVisibleRxz;

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

Visible::VisibleRxzPerturbation make_visible_mode(
    const double x, const Metric &delta_h,
    const Ricci::MetricDerivatives &delta_h_derivatives,
    const double delta_chi,
    const Ricci::ScalarDerivatives &delta_chi_derivatives)
{
    return Visible::make_visible_rxz_perturbation(
        x, delta_h, delta_h_derivatives, delta_chi,
        delta_chi_derivatives);
}

Ricci::CartoonRicciInputs make_stage4g_inputs(
    const Visible::VisibleRxzPerturbation &perturbation, const double epsilon)
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

double direct_stage4g_rxz(const Visible::VisibleRxzPerturbation &perturbation,
                          const double epsilon)
{
    const auto ricci =
        Ricci::compute_metric_derivative_ricci(make_stage4g_inputs(perturbation,
                                                                    epsilon));
    return RicciBridge::to_rhs_ricci_components(ricci).xz;
}

double central_difference_direct(
    const Visible::VisibleRxzPerturbation &perturbation, const double epsilon)
{
    return (direct_stage4g_rxz(perturbation, epsilon) -
            direct_stage4g_rxz(perturbation, -epsilon)) /
           (2.0 * epsilon);
}

void check_oracle_plateau(const std::string &label,
                          const Visible::VisibleRxzPerturbation &perturbation,
                          const double expected)
{
    const auto linear = Visible::compute_visible_physical_delta_rxz(perturbation);
    require_close(label + " analytic formula", linear.delta_rxz(), expected,
                  1.0e-12);

    const std::array<double, 5> epsilons = {1.0e-2, 1.0e-4, 1.0e-5,
                                           1.0e-6, 1.0e-7};
    double middle_direct_max = 0.0;
    for (const double epsilon : epsilons)
    {
        const double direct = central_difference_direct(perturbation, epsilon);
        const double direct_error = std::abs(direct - linear.delta_rxz());
        std::cout << "INFO " << label << " eps=" << epsilon
                  << " direct_error=" << direct_error << "\n";
        if (epsilon == 1.0e-5 || epsilon == 1.0e-6)
        {
            middle_direct_max = std::max(middle_direct_max, direct_error);
        }
    }

    require_close(label + " Stage 4G epsilon plateau", middle_direct_max, 0.0,
                  1.0e-7);
}

Visible::VisibleRxzPerturbation pure_offdiagonal_hxz_mode(
    const double x, const double z, const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.xz = c;
    derivatives.dz.xz = -x * k * s;
    derivatives.dxz.xz = -k * s;
    derivatives.dzz.xz = -x * k * k * c;
    return make_visible_mode(x, metric(0.0, x * c, 0.0, 0.0), derivatives,
                             0.0, zero_scalar_derivatives());
}

Visible::VisibleRxzPerturbation even_diagonal_hxx_mode(
    const double x, const double z, const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.xx = 2.0 * x * c;
    derivatives.dz.xx = -k * x * x * s;
    derivatives.dxx.xx = 2.0 * c;
    derivatives.dxz.xx = -2.0 * x * k * s;
    derivatives.dzz.xx = -k * k * x * x * c;
    return make_visible_mode(x, metric(x * x * c, 0.0, 0.0, 0.0),
                             derivatives, 0.0, zero_scalar_derivatives());
}

Visible::VisibleRxzPerturbation mixed_hidden_hww_mode(
    const double x, const double z, const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.ww = 2.0 * x * s;
    derivatives.dz.ww = k * x * x * c;
    derivatives.dxx.ww = 2.0 * s;
    derivatives.dxz.ww = 2.0 * x * k * c;
    derivatives.dzz.ww = -k * k * x * x * s;
    return make_visible_mode(x, metric(0.0, 0.0, 0.0, x * x * s),
                             derivatives, 0.0, zero_scalar_derivatives());
}

Visible::VisibleRxzPerturbation pure_chi_even_mode(const double x,
                                                   const double z,
                                                   const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_scalar_derivatives();
    derivatives.first.dx = c;
    derivatives.first.dz = -x * k * s;
    derivatives.second.dxx = 0.0;
    derivatives.second.dxz = -k * s;
    derivatives.second.dzz = -x * k * k * c;
    return make_visible_mode(x, zero_metric(), zero_metric_derivatives(),
                             x * c, derivatives);
}

void check_zero_and_required_perturbations()
{
    const auto zero =
        make_visible_mode(2.0, zero_metric(), zero_metric_derivatives(), 0.0,
                          zero_scalar_derivatives());
    check_oracle_plateau("zero perturbation", zero, 0.0);

    check_oracle_plateau("pure delta h_xz zero oracle",
                         pure_offdiagonal_hxz_mode(2.0, 0.37, 2.5), 0.0);

    const double x = 2.0;
    const double z = 0.4;
    const double k = 3.0;
    check_oracle_plateau("even diagonal scalar h_xx mode",
                         even_diagonal_hxx_mode(x, z, k),
                         -k * x * std::sin(k * z));

    const double z_mixed = 0.37;
    const double k_mixed = 2.5;
    check_oracle_plateau("mixed radial-z hidden h_ww mode",
                         mixed_hidden_hww_mode(x, z_mixed, k_mixed),
                         -3.0 * x * k_mixed *
                             std::cos(k_mixed * z_mixed));

    check_oracle_plateau("pure delta chi mixed derivative mode",
                         pure_chi_even_mode(x, z, k),
                         -k * std::sin(k * z));
}

double sine_amplitude(const std::array<double, 64> &values, const int mode)
{
    constexpr double pi = 3.141592653589793238462643383279502884;
    double sum = 0.0;
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        const double z = 2.0 * pi * static_cast<double>(i) /
                         static_cast<double>(values.size());
        sum += values[i] * std::sin(static_cast<double>(mode) * z);
    }
    return 2.0 * sum / static_cast<double>(values.size());
}

double cosine_amplitude(const std::array<double, 64> &values, const int mode)
{
    constexpr double pi = 3.141592653589793238462643383279502884;
    double sum = 0.0;
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        const double z = 2.0 * pi * static_cast<double>(i) /
                         static_cast<double>(values.size());
        sum += values[i] * std::cos(static_cast<double>(mode) * z);
    }
    return 2.0 * sum / static_cast<double>(values.size());
}

void check_one_z_parity_sector()
{
    constexpr double pi = 3.141592653589793238462643383279502884;
    const double x = 2.0;
    const int mode = 3;
    std::array<double, 64> output = {};

    for (std::size_t i = 0; i < output.size(); ++i)
    {
        const double z = 2.0 * pi * static_cast<double>(i) /
                         static_cast<double>(output.size());
        output[i] = Visible::compute_visible_physical_delta_rxz(
                        even_diagonal_hxx_mode(x, z, static_cast<double>(mode)))
                        .delta_rxz();
    }

    const double allowed = std::abs(sine_amplitude(output, mode));
    const double forbidden = std::abs(cosine_amplitude(output, mode));
    const double leakage = forbidden / allowed;
    std::cout << "INFO visible Rxz parity allowed=" << allowed
              << " forbidden=" << forbidden << " leakage=" << leakage << "\n";

    require_true("one-z allowed sine output is nonzero", allowed > 1.0e-3);
    require_true("one-z forbidden cosine leakage is near roundoff",
                 leakage < 1.0e-12);
    require_true("wrong parity assignment would fail",
                 forbidden < 1.0e-12 && allowed > 1.0e-3);

    const auto plus = Visible::compute_visible_physical_delta_rxz(
        even_diagonal_hxx_mode(x, 0.41, static_cast<double>(mode)));
    const auto minus = Visible::compute_visible_physical_delta_rxz(
        even_diagonal_hxx_mode(x, -0.41, static_cast<double>(mode)));
    require_close("even scalar input gives odd R_xz output", plus.delta_rxz(),
                  -minus.delta_rxz(), 1.0e-12);
}

void check_negative_guards()
{
    const auto hidden = mixed_hidden_hww_mode(2.0, 0.37, 2.5);
    const auto hidden_linear =
        Visible::compute_visible_physical_delta_rxz(hidden);
    const auto &hidden_derivs = hidden.delta_h_derivatives();
    const double wrong_mixed_derivative_sign =
        (hidden_derivs.dz.xx - hidden_derivs.dz.ww) / hidden.x() +
        hidden_derivs.dxz.ww +
        hidden.delta_chi_derivatives().second.dxz;
    require_true("wrong mixed-derivative sign would fail",
                 std::abs(hidden_linear.delta_rxz() -
                          wrong_mixed_derivative_sign) > 1.0e-3);

    const auto chi = pure_chi_even_mode(2.0, 0.4, 3.0);
    require_true("dropping conformal-factor mixed derivative would fail",
                 std::abs(Visible::compute_visible_physical_delta_rxz(chi)
                              .delta_rxz()) > 1.0e-3);

    require_true("hidden multiplicity on visible R_xz would fail",
                 std::abs(2.0 * hidden_linear.delta_rxz() -
                          hidden_linear.delta_rxz()) > 1.0e-3);

    const auto hxz = pure_offdiagonal_hxz_mode(2.0, 0.37, 2.5);
    const auto hxz_linear = Visible::compute_visible_physical_delta_rxz(hxz);
    const double spurious_hxz_term =
        hxz.delta_h_derivatives().dz.xz / hxz.x();
    require_close("pure h_xz is a zero oracle", hxz_linear.delta_rxz(), 0.0,
                  1.0e-12);
    require_true("spurious h_xz contribution would fail",
                 std::abs(spurious_hxz_term - hxz_linear.delta_rxz()) >
                     1.0e-3);
}

void check_scope_flags_and_validation()
{
    static_assert(!std::is_aggregate<Visible::VisibleRxzPerturbation>::value,
                  "Stage 4AO-C visible Rxz perturbation package is non-aggregate");
    static_assert(!std::is_aggregate<Visible::VisiblePhysicalDeltaRxz>::value,
                  "Stage 4AO-C visible Rxz result package is non-aggregate");

    require_true("visible physical delta R_xz is implemented",
                 Visible::visible_physical_delta_rxz_implemented);
    require_true("visible Rxz helper does not implement delta R_xx",
                 !Visible::visible_delta_rxx_implemented);
    require_true("visible Rxz helper does not implement delta R_zz",
                 !Visible::visible_delta_rzz_implemented);
    require_true("visible Rxz helper does not perform A curvature insertion",
                 !Visible::trace_free_a_curvature_source_implemented);
    require_true("visible Rxz helper does not perform Theta Ricci insertion",
                 !Visible::theta_ricci_scalar_implemented);
    require_true("full frozen-gauge operator remains unimplemented",
                 !Visible::full_frozen_gauge_operator_implemented);
    require_true("eigensolver remains unimplemented",
                 !Visible::eigensolver_implemented);
    require_true("production RHS wiring remains unimplemented",
                 !Visible::production_rhs_wiring_implemented);

    bool threw = false;
    try
    {
        auto bad_derivatives = zero_metric_derivatives();
        bad_derivatives.dxz.ww = std::numeric_limits<double>::quiet_NaN();
        (void)make_visible_mode(2.0, zero_metric(), bad_derivatives, 0.0,
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
    check_zero_and_required_perturbations();
    check_one_z_parity_sector();
    check_negative_guards();
    check_scope_flags_and_validation();
    std::cout << "Stage 4AO-C visible physical delta R_xz tests passed\n";
    return 0;
}
