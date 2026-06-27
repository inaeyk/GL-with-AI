#include "CartoonRicciBridge.hpp"
#include "Stage4AOFrozenGaugeVisibleRzz.hpp"

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
namespace Visible = BlackStringToy::Stage4AOFrozenGaugeVisibleRzz;

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

Visible::VisibleRzzPerturbation make_visible_mode(
    const double x, const Metric &delta_h,
    const Ricci::MetricDerivatives &delta_h_derivatives,
    const double delta_chi,
    const Ricci::ScalarDerivatives &delta_chi_derivatives)
{
    return Visible::make_visible_rzz_perturbation(
        x, delta_h, delta_h_derivatives, delta_chi,
        delta_chi_derivatives);
}

Ricci::CartoonRicciInputs make_stage4g_inputs(
    const Visible::VisibleRzzPerturbation &perturbation, const double epsilon)
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

double direct_stage4g_rzz(const Visible::VisibleRzzPerturbation &perturbation,
                          const double epsilon)
{
    const auto ricci =
        Ricci::compute_metric_derivative_ricci(make_stage4g_inputs(perturbation,
                                                                    epsilon));
    return RicciBridge::to_rhs_ricci_components(ricci).zz;
}

double central_difference_direct(
    const Visible::VisibleRzzPerturbation &perturbation, const double epsilon)
{
    return (direct_stage4g_rzz(perturbation, epsilon) -
            direct_stage4g_rzz(perturbation, -epsilon)) /
           (2.0 * epsilon);
}

void check_oracle_plateau(const std::string &label,
                          const Visible::VisibleRzzPerturbation &perturbation,
                          const double expected)
{
    const auto linear = Visible::compute_visible_physical_delta_rzz(perturbation);
    require_close(label + " analytic formula", linear.delta_rzz(), expected,
                  1.0e-12);

    const std::array<double, 5> epsilons = {1.0e-2, 1.0e-4, 1.0e-5,
                                           1.0e-6, 1.0e-7};
    double middle_direct_max = 0.0;
    for (const double epsilon : epsilons)
    {
        const double direct = central_difference_direct(perturbation, epsilon);
        const double direct_error = std::abs(direct - linear.delta_rzz());
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

Visible::VisibleRzzPerturbation pure_hzz_mode(const double x, const double z,
                                              const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.zz = 2.0 * x * c;
    derivatives.dz.zz = -k * x * x * s;
    derivatives.dxx.zz = 2.0 * c;
    derivatives.dxz.zz = -2.0 * x * k * s;
    derivatives.dzz.zz = -k * k * x * x * c;
    return make_visible_mode(x, metric(0.0, 0.0, x * x * c, 0.0),
                             derivatives, 0.0, zero_scalar_derivatives());
}

Visible::VisibleRzzPerturbation pure_hxz_mode(const double x, const double z,
                                              const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.xz = s;
    derivatives.dz.xz = x * k * c;
    derivatives.dxz.xz = k * c;
    derivatives.dzz.xz = -x * k * k * s;
    return make_visible_mode(x, metric(0.0, x * s, 0.0, 0.0), derivatives,
                             0.0, zero_scalar_derivatives());
}

Visible::VisibleRzzPerturbation pure_hww_z_mode(const double x,
                                                const double z,
                                                const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dz.ww = -k * s;
    derivatives.dzz.ww = -k * k * c;
    return make_visible_mode(x, metric(0.0, 0.0, 0.0, c), derivatives, 0.0,
                             zero_scalar_derivatives());
}

Visible::VisibleRzzPerturbation pure_chi_mode(const double x, const double z,
                                              const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_scalar_derivatives();
    derivatives.first.dx = 2.0 * x * c;
    derivatives.first.dz = -k * x * x * s;
    derivatives.second.dxx = 2.0 * c;
    derivatives.second.dxz = -2.0 * x * k * s;
    derivatives.second.dzz = -k * k * x * x * c;
    return make_visible_mode(x, zero_metric(), zero_metric_derivatives(),
                             x * x * c, derivatives);
}

Visible::VisibleRzzPerturbation pure_hxx_z_mode(const double x,
                                                const double z,
                                                const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dz.xx = -k * s;
    derivatives.dzz.xx = -k * k * c;
    return make_visible_mode(x, metric(c, 0.0, 0.0, 0.0), derivatives, 0.0,
                             zero_scalar_derivatives());
}

void check_zero_and_required_perturbations()
{
    const auto zero =
        make_visible_mode(2.0, zero_metric(), zero_metric_derivatives(), 0.0,
                          zero_scalar_derivatives());
    check_oracle_plateau("zero perturbation", zero, 0.0);

    const double x = 2.0;
    const double z = 0.4;
    const double k = 3.0;
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);

    check_oracle_plateau("pure delta h_zz mixed radial-z mode",
                         pure_hzz_mode(x, z, k), -3.0 * c);
    check_oracle_plateau("pure delta h_xz one-z mode",
                         pure_hxz_mode(x, z, k), 3.0 * k * c);
    check_oracle_plateau("pure delta h_ww z-dependent mode",
                         pure_hww_z_mode(x, z, k), k * k * c);
    check_oracle_plateau("pure delta chi mixed mode", pure_chi_mode(x, z, k),
                         3.0 * c - 1.5 * k * k * x * x * c);
    check_oracle_plateau("pure delta h_xx z-dependent mode",
                         pure_hxx_z_mode(x, z, k), 0.5 * k * k * c);

    (void)s;
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

void check_even_parity_sector()
{
    constexpr double pi = 3.141592653589793238462643383279502884;
    const double x = 2.0;
    const int mode = 3;

    for (const char *label : {"scalar diagonal", "one-z h_xz"})
    {
        std::array<double, 64> output = {};
        for (std::size_t i = 0; i < output.size(); ++i)
        {
            const double z = 2.0 * pi * static_cast<double>(i) /
                             static_cast<double>(output.size());
            const auto perturbation =
                std::string(label) == "one-z h_xz"
                    ? pure_hxz_mode(x, z, static_cast<double>(mode))
                    : pure_hxx_z_mode(x, z, static_cast<double>(mode));
            output[i] =
                Visible::compute_visible_physical_delta_rzz(perturbation)
                    .delta_rzz();
        }

        const double allowed = std::abs(cosine_amplitude(output, mode));
        const double forbidden = std::abs(sine_amplitude(output, mode));
        const double leakage = forbidden / allowed;
        std::cout << "INFO visible Rzz parity " << label
                  << " allowed=" << allowed << " forbidden=" << forbidden
                  << " leakage=" << leakage << "\n";

        require_true(std::string(label) + " even output is nonzero",
                     allowed > 1.0e-3);
        require_true(std::string(label) +
                         " forbidden odd leakage is near roundoff",
                     leakage < 1.0e-12);
        require_true(std::string(label) + " wrong parity assignment would fail",
                     forbidden < 1.0e-12 && allowed > 1.0e-3);
    }
}

void check_negative_guards()
{
    const auto hxz = pure_hxz_mode(2.0, 0.4, 3.0);
    const auto hxz_linear = Visible::compute_visible_physical_delta_rzz(hxz);
    const auto &hxz_derivs = hxz.delta_h_derivatives();
    const double wrong_hxz_sign =
        -hxz_derivs.dxz.xz + 2.0 * hxz_derivs.dz.xz / hxz.x();
    require_true("wrong partial_xz(delta h_xz) sign would fail",
                 std::abs(hxz_linear.delta_rzz() - wrong_hxz_sign) >
                     1.0e-3);
    const double without_hxz_one_over_x = hxz_derivs.dxz.xz;
    require_true("dropping 2 partial_z(delta h_xz)/x would fail",
                 std::abs(hxz_linear.delta_rzz() -
                          without_hxz_one_over_x) > 1.0e-3);

    const auto hzz = pure_hzz_mode(2.0, 0.4, 3.0);
    const auto hzz_linear = Visible::compute_visible_physical_delta_rzz(hzz);
    const auto &hzz_derivs = hzz.delta_h_derivatives();
    const double without_hzz_one_over_x =
        hzz_derivs.dxz.xz - 0.5 * hzz_derivs.dzz.xx -
        0.5 * hzz_derivs.dxx.zz - hzz_derivs.dzz.ww;
    require_true("dropping -partial_x(delta h_zz)/x would fail",
                 std::abs(hzz_linear.delta_rzz() - without_hzz_one_over_x) >
                     1.0e-3);

    const auto hww = pure_hww_z_mode(2.0, 0.4, 3.0);
    require_true("dropping -partial_zz(delta h_ww) would fail",
                 std::abs(Visible::compute_visible_physical_delta_rzz(hww)
                              .delta_rzz()) > 1.0e-3);

    const auto chi = pure_chi_mode(2.0, 0.4, 3.0);
    const auto chi_linear = Visible::compute_visible_physical_delta_rzz(chi);
    const auto &dchi = chi.delta_chi_derivatives();
    const double flipped_chi_sign =
        -(dchi.first.dx / chi.x() + 0.5 * dchi.second.dxx +
          1.5 * dchi.second.dzz);
    require_true("flipping positive chi-term signs would fail",
                 std::abs(chi_linear.delta_rzz() - flipped_chi_sign) >
                     1.0e-3);

    require_true("hidden multiplicity on visible R_zz would fail",
                 std::abs(2.0 * hzz_linear.delta_rzz() -
                          hzz_linear.delta_rzz()) > 1.0e-3);
    require_true("treating raw R_zz as trace-free would fail",
                 std::abs(hzz_linear.delta_rzz() -
                          0.5 * hzz_linear.delta_rzz()) > 1.0e-3);
}

void check_scope_flags_and_validation()
{
    static_assert(!std::is_aggregate<Visible::VisibleRzzPerturbation>::value,
                  "Stage 4AO-C visible Rzz perturbation package is non-aggregate");
    static_assert(!std::is_aggregate<Visible::VisiblePhysicalDeltaRzz>::value,
                  "Stage 4AO-C visible Rzz result package is non-aggregate");

    require_true("visible physical delta R_zz is implemented",
                 Visible::visible_physical_delta_rzz_implemented);
    require_true("delta R_xx remains unimplemented",
                 !Visible::visible_delta_rxx_implemented);
    require_true("trace-free A curvature source remains unimplemented",
                 !Visible::trace_free_a_curvature_source_implemented);
    require_true("Theta Ricci scalar remains unimplemented",
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
        bad_derivatives.dzz.ww = std::numeric_limits<double>::infinity();
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
    check_even_parity_sector();
    check_negative_guards();
    check_scope_flags_and_validation();
    std::cout << "Stage 4AO-C visible physical delta R_zz tests passed\n";
    return 0;
}
