#include "CartoonRicciBridge.hpp"
#include "Stage4AOFrozenGaugeRicciAssembly.hpp"

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
namespace RicciAssembly = BlackStringToy::Stage4AOFrozenGaugeRicciAssembly;
namespace HiddenRww = BlackStringToy::Stage4AOFrozenGaugeHiddenRww;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;
namespace VisibleRxx = BlackStringToy::Stage4AOFrozenGaugeVisibleRxx;
namespace VisibleRxz = BlackStringToy::Stage4AOFrozenGaugeVisibleRxz;
namespace VisibleRzz = BlackStringToy::Stage4AOFrozenGaugeVisibleRzz;

using Metric = BlackStringToy::ConformalCartoonAlgebra::ConformalMetric;

struct CommonPerturbation
{
    double x;
    Metric delta_h;
    Ricci::MetricDerivatives delta_h_derivatives;
    double delta_chi;
    Ricci::ScalarDerivatives delta_chi_derivatives;
};

struct OracleAssembly
{
    double rxx;
    double rxz;
    double rzz;
    double rww;
    double trace;
    double tf_xx;
    double tf_xz;
    double tf_zz;
    double tf_ww;
};

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

RicciAssembly::TraceFreeRicciAssembly
compute_assembly(const CommonPerturbation &perturbation)
{
    const auto rxx_input = VisibleRxx::make_visible_rxx_perturbation(
        perturbation.x, perturbation.delta_h,
        perturbation.delta_h_derivatives, perturbation.delta_chi,
        perturbation.delta_chi_derivatives);
    const auto rxz_input = VisibleRxz::make_visible_rxz_perturbation(
        perturbation.x, perturbation.delta_h,
        perturbation.delta_h_derivatives, perturbation.delta_chi,
        perturbation.delta_chi_derivatives);
    const auto rzz_input = VisibleRzz::make_visible_rzz_perturbation(
        perturbation.x, perturbation.delta_h,
        perturbation.delta_h_derivatives, perturbation.delta_chi,
        perturbation.delta_chi_derivatives);
    const auto rww_input = HiddenRww::make_hidden_rww_perturbation(
        perturbation.x, perturbation.delta_h,
        perturbation.delta_h_derivatives, perturbation.delta_chi,
        perturbation.delta_chi_derivatives);

    const auto raw_components = RicciAssembly::make_raw_ricci_components(
        VisibleRxx::compute_visible_physical_delta_rxx(rxx_input),
        VisibleRxz::compute_visible_physical_delta_rxz(rxz_input),
        VisibleRzz::compute_visible_physical_delta_rzz(rzz_input),
        HiddenRww::compute_hidden_physical_delta_rww(rww_input));
    return RicciAssembly::assemble_trace_free_ricci(raw_components);
}

Ricci::CartoonRicciInputs make_stage4g_inputs(
    const CommonPerturbation &perturbation, const double epsilon)
{
    const auto &h = perturbation.delta_h;
    const auto &dh = perturbation.delta_h_derivatives;
    const auto &dchi = perturbation.delta_chi_derivatives;

    Ricci::CartoonRicciInputs inputs{};
    inputs.x = perturbation.x;
    inputs.chi = 1.0 + epsilon * perturbation.delta_chi;
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

RicciBridge::RhsRicciComponents direct_stage4g_components(
    const CommonPerturbation &perturbation, const double epsilon)
{
    const auto ricci =
        Ricci::compute_metric_derivative_ricci(make_stage4g_inputs(perturbation,
                                                                    epsilon));
    return RicciBridge::to_rhs_ricci_components(ricci);
}

OracleAssembly central_difference_oracle(
    const CommonPerturbation &perturbation, const double epsilon)
{
    const auto plus = direct_stage4g_components(perturbation, epsilon);
    const auto minus = direct_stage4g_components(perturbation, -epsilon);
    const double inv_2eps = 1.0 / (2.0 * epsilon);
    const double rxx = (plus.xx - minus.xx) * inv_2eps;
    const double rxz = (plus.xz - minus.xz) * inv_2eps;
    const double rzz = (plus.zz - minus.zz) * inv_2eps;
    const double rww = (plus.ww - minus.ww) * inv_2eps;
    const double trace = rxx + rzz + 2.0 * rww;
    return {rxx, rxz, rzz, rww, trace, rxx - 0.25 * trace, rxz,
            rzz - 0.25 * trace, rww - 0.25 * trace};
}

double max_assembly_error(
    const RicciAssembly::TraceFreeRicciAssembly &assembly,
    const OracleAssembly &oracle)
{
    double error = 0.0;
    error = std::max(error, std::abs(assembly.raw_rxx() - oracle.rxx));
    error = std::max(error, std::abs(assembly.raw_rxz() - oracle.rxz));
    error = std::max(error, std::abs(assembly.raw_rzz() - oracle.rzz));
    error = std::max(error, std::abs(assembly.raw_rww() - oracle.rww));
    error = std::max(error, std::abs(assembly.scalar_trace() - oracle.trace));
    error = std::max(error, std::abs(assembly.tf_xx() - oracle.tf_xx));
    error = std::max(error, std::abs(assembly.tf_xz() - oracle.tf_xz));
    error = std::max(error, std::abs(assembly.tf_zz() - oracle.tf_zz));
    error = std::max(error, std::abs(assembly.tf_ww() - oracle.tf_ww));
    return error;
}

void check_oracle_plateau(const std::string &label,
                          const CommonPerturbation &perturbation)
{
    const auto assembly = compute_assembly(perturbation);
    const std::array<double, 5> epsilons = {1.0e-2, 1.0e-4, 1.0e-5,
                                           1.0e-6, 1.0e-7};
    double middle_direct_max = 0.0;
    for (const double epsilon : epsilons)
    {
        const auto oracle = central_difference_oracle(perturbation, epsilon);
        const double direct_error = max_assembly_error(assembly, oracle);
        std::cout << "INFO " << label << " eps=" << epsilon
                  << " assembly_error=" << direct_error << "\n";
        if (epsilon == 1.0e-5 || epsilon == 1.0e-6)
        {
            middle_direct_max = std::max(middle_direct_max, direct_error);
        }
    }

    require_close(label + " Stage 4G epsilon plateau", middle_direct_max, 0.0,
                  2.0e-7);
}

CommonPerturbation make_common(const double x, const Metric &delta_h,
                               const Ricci::MetricDerivatives &dh,
                               const double delta_chi,
                               const Ricci::ScalarDerivatives &dchi)
{
    return {x, delta_h, dh, delta_chi, dchi};
}

CommonPerturbation zero_perturbation()
{
    return make_common(2.0, zero_metric(), zero_metric_derivatives(), 0.0,
                       zero_scalar_derivatives());
}

CommonPerturbation pure_hxx_mixed_mode(const double x, const double z,
                                       const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.xx = 2.0 * x * c;
    derivatives.dz.xx = -k * x * x * s;
    derivatives.dxx.xx = 2.0 * c;
    derivatives.dxz.xx = -2.0 * x * k * s;
    derivatives.dzz.xx = -k * k * x * x * c;
    return make_common(x, metric(x * x * c, 0.0, 0.0, 0.0), derivatives, 0.0,
                       zero_scalar_derivatives());
}

CommonPerturbation pure_hzz_mode(const double x, const double z,
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
    return make_common(x, metric(0.0, 0.0, x * x * c, 0.0), derivatives, 0.0,
                       zero_scalar_derivatives());
}

CommonPerturbation pure_hww_mode(const double x, const double z,
                                 const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.ww = 2.0 * x * c;
    derivatives.dz.ww = -k * x * x * s;
    derivatives.dxx.ww = 2.0 * c;
    derivatives.dxz.ww = -2.0 * x * k * s;
    derivatives.dzz.ww = -k * k * x * x * c;
    return make_common(x, metric(0.0, 0.0, 0.0, x * x * c), derivatives, 0.0,
                       zero_scalar_derivatives());
}

CommonPerturbation pure_hxz_mode(const double x, const double z,
                                 const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto derivatives = zero_metric_derivatives();
    derivatives.dx.xz = s;
    derivatives.dz.xz = x * k * c;
    derivatives.dxz.xz = k * c;
    derivatives.dzz.xz = -x * k * k * s;
    return make_common(x, metric(0.0, x * s, 0.0, 0.0), derivatives, 0.0,
                       zero_scalar_derivatives());
}

CommonPerturbation pure_chi_mode(const double x, const double z,
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
    return make_common(x, zero_metric(), zero_metric_derivatives(),
                       x * x * c, derivatives);
}

CommonPerturbation mixed_scalar_mode(const double x, const double z,
                                     const double k)
{
    const double c = std::cos(k * z);
    const double s = std::sin(k * z);
    auto h_derivatives = zero_metric_derivatives();
    h_derivatives.dx.xx = 2.0 * x * c;
    h_derivatives.dx.zz = -0.5 * s;
    h_derivatives.dx.ww = -1.2 * x * c;
    h_derivatives.dz.xx = -k * x * x * s;
    h_derivatives.dz.zz = -0.5 * x * k * c;
    h_derivatives.dz.ww = 1.2 * x * x * k * s;
    h_derivatives.dxx.xx = 2.0 * c;
    h_derivatives.dxx.ww = -1.2 * c;
    h_derivatives.dxz.xx = -2.0 * x * k * s;
    h_derivatives.dxz.zz = -0.5 * k * c;
    h_derivatives.dxz.ww = 2.4 * x * k * s;
    h_derivatives.dzz.xx = -k * k * x * x * c;
    h_derivatives.dzz.zz = 0.5 * x * k * k * s;
    h_derivatives.dzz.ww = 1.2 * x * x * k * k * c;

    auto chi_derivatives = zero_scalar_derivatives();
    chi_derivatives.first.dx = 0.7 * c;
    chi_derivatives.first.dz = -0.7 * x * k * s;
    chi_derivatives.second.dxz = -0.7 * k * s;
    chi_derivatives.second.dzz = -0.7 * x * k * k * c;

    return make_common(x,
                       metric(x * x * c, 0.0, -0.5 * x * s,
                              -1.2 * x * x * c),
                       h_derivatives, 0.7 * x * c, chi_derivatives);
}

void check_component_formula_identities()
{
    const auto perturbation = mixed_scalar_mode(2.0, 0.4, 3.0);
    const auto assembly = compute_assembly(perturbation);
    require_close("delta R trace formula", assembly.scalar_trace(),
                  assembly.raw_rxx() + assembly.raw_rzz() +
                      2.0 * assembly.raw_rww(),
                  1.0e-12);
    require_close("hidden multiplicity two appears in trace",
                  assembly.scalar_trace() -
                      (assembly.raw_rxx() + assembly.raw_rzz()),
                  2.0 * assembly.raw_rww(), 1.0e-12);
    require_true("R_xz is not part of scalar trace",
                 std::abs(assembly.scalar_trace() -
                          (assembly.raw_rxx() + assembly.raw_rxz() +
                           assembly.raw_rzz() + 2.0 * assembly.raw_rww())) >
                     1.0e-3);
    require_close("trace-free residual vanishes",
                  RicciAssembly::trace_free_residual(assembly), 0.0,
                  1.0e-12);
    require_close("R_xz trace-free component equals raw R_xz",
                  assembly.tf_xz(), assembly.raw_rxz(), 1.0e-12);
}

void check_required_perturbations()
{
    const double x = 2.0;
    const double z = 0.4;
    const double k = 3.0;

    check_oracle_plateau("zero perturbation", zero_perturbation());
    check_oracle_plateau("pure delta h_xx", pure_hxx_mixed_mode(x, z, k));
    check_oracle_plateau("pure delta h_zz", pure_hzz_mode(x, z, k));
    check_oracle_plateau("pure delta h_ww", pure_hww_mode(x, z, k));
    check_oracle_plateau("pure delta h_xz", pure_hxz_mode(x, z, k));
    check_oracle_plateau("pure delta chi", pure_chi_mode(x, z, k));
    check_oracle_plateau("mixed radial-z scalar mode",
                         mixed_scalar_mode(x, z, k));
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

void check_parity()
{
    constexpr double pi = 3.141592653589793238462643383279502884;
    const double x = 2.0;
    const int mode = 3;

    std::array<double, 64> trace_output = {};
    std::array<double, 64> tf_xz_output = {};
    for (std::size_t i = 0; i < trace_output.size(); ++i)
    {
        const double z = 2.0 * pi * static_cast<double>(i) /
                         static_cast<double>(trace_output.size());
        const auto assembly =
            compute_assembly(pure_hxx_mixed_mode(x, z, mode));
        trace_output[i] = assembly.scalar_trace();
        tf_xz_output[i] = assembly.tf_xz();
    }

    const double trace_allowed = std::abs(cosine_amplitude(trace_output, mode));
    const double trace_forbidden = std::abs(sine_amplitude(trace_output, mode));
    const double trace_leakage = trace_forbidden / trace_allowed;
    std::cout << "INFO Ricci trace parity scalar input allowed="
              << trace_allowed << " forbidden=" << trace_forbidden
              << " leakage=" << trace_leakage << "\n";
    require_true("Ricci trace even-sector output is nonzero",
                 trace_allowed > 1.0e-3);
    require_true("Ricci trace forbidden odd leakage is near roundoff",
                 trace_leakage < 1.0e-12);

    const double xz_allowed = std::abs(sine_amplitude(tf_xz_output, mode));
    const double xz_forbidden = std::abs(cosine_amplitude(tf_xz_output, mode));
    const double xz_leakage = xz_forbidden / xz_allowed;
    std::cout << "INFO Ricci TF_xz parity scalar input allowed="
              << xz_allowed << " forbidden=" << xz_forbidden
              << " leakage=" << xz_leakage << "\n";
    require_true("Ricci TF_xz one-z output is nonzero", xz_allowed > 1.0e-3);
    require_true("Ricci TF_xz forbidden even leakage is near roundoff",
                 xz_leakage < 1.0e-12);

    std::array<double, 64> one_z_trace_output = {};
    for (std::size_t i = 0; i < one_z_trace_output.size(); ++i)
    {
        const double z = 2.0 * pi * static_cast<double>(i) /
                         static_cast<double>(one_z_trace_output.size());
        const auto assembly = compute_assembly(pure_hxz_mode(x, z, mode));
        one_z_trace_output[i] = assembly.scalar_trace();
    }
    const double one_z_allowed =
        std::abs(cosine_amplitude(one_z_trace_output, mode));
    const double one_z_forbidden =
        std::abs(sine_amplitude(one_z_trace_output, mode));
    const double one_z_leakage = one_z_forbidden / one_z_allowed;
    std::cout << "INFO Ricci trace parity one-z input allowed="
              << one_z_allowed << " forbidden=" << one_z_forbidden
              << " leakage=" << one_z_leakage << "\n";
    require_true("one-z h_xz input produces even Ricci trace",
                 one_z_allowed > 1.0e-3);
    require_true("one-z h_xz trace forbidden leakage is near roundoff",
                 one_z_leakage < 1.0e-12);
}

void check_negative_guards()
{
    const auto assembly = compute_assembly(mixed_scalar_mode(2.0, 0.4, 3.0));
    const double no_hidden_factor_trace =
        assembly.raw_rxx() + assembly.raw_rzz() + assembly.raw_rww();
    require_true("dropping hidden factor two in Ricci trace would fail",
                 std::abs(assembly.scalar_trace() - no_hidden_factor_trace) >
                     1.0e-3);

    const double xz_in_trace =
        assembly.raw_rxx() + assembly.raw_rxz() + assembly.raw_rzz() +
        2.0 * assembly.raw_rww();
    require_true("incorrectly adding R_xz to scalar trace would fail",
                 std::abs(assembly.scalar_trace() - xz_in_trace) > 1.0e-3);

    const double d3_tf_xx = assembly.raw_rxx() - assembly.scalar_trace() / 3.0;
    require_true("using d=3 trace-free projection would fail",
                 std::abs(assembly.tf_xx() - d3_tf_xx) > 1.0e-3);

    const double premature_rxx_only_projection =
        assembly.raw_rxx() - 0.25 * assembly.raw_rxx();
    require_true("projecting R_xx before full trace assembly would fail",
                 std::abs(assembly.tf_xx() -
                          premature_rxx_only_projection) > 1.0e-3);

    require_true("assembly helper does not perform A curvature insertion",
                 !RicciAssembly::trace_free_a_curvature_source_implemented);
    require_true("assembly helper does not perform Theta Ricci insertion",
                 !RicciAssembly::theta_ricci_scalar_rhs_implemented);
    require_true("complete frozen-gauge operator remains unimplemented",
                 !RicciAssembly::full_frozen_gauge_operator_implemented);
}

void check_scope_flags_and_validation()
{
    static_assert(!std::is_aggregate<RicciAssembly::RawRicciComponents>::value,
                  "Stage 4AO-C raw Ricci components package is non-aggregate");
    static_assert(!std::is_aggregate<RicciAssembly::TraceFreeRicciAssembly>::value,
                  "Stage 4AO-C trace-free Ricci assembly package is non-aggregate");

    require_true("raw Ricci trace is implemented",
                 RicciAssembly::raw_ricci_trace_implemented);
    require_true("trace-free Ricci assembly is implemented",
                 RicciAssembly::trace_free_ricci_assembly_implemented);
    require_true("assembly namespace leaves A insertion to operator",
                 !RicciAssembly::trace_free_a_curvature_source_implemented);
    require_true("assembly namespace leaves Theta insertion to operator",
                 !RicciAssembly::theta_ricci_scalar_rhs_implemented);
    require_true("eigensolver remains unimplemented",
                 !RicciAssembly::eigensolver_implemented);
    require_true("production RHS wiring remains unimplemented",
                 !RicciAssembly::production_rhs_wiring_implemented);

    bool threw = false;
    try
    {
        auto bad_derivatives = zero_metric_derivatives();
        bad_derivatives.dxx.ww = std::numeric_limits<double>::quiet_NaN();
        (void)compute_assembly(make_common(2.0, zero_metric(), bad_derivatives,
                                           0.0,
                                           zero_scalar_derivatives()));
    }
    catch (const std::domain_error &)
    {
        threw = true;
    }
    require_true("nonfinite underlying raw Ricci data are rejected", threw);
}

} // namespace

int main()
{
    check_component_formula_identities();
    check_required_perturbations();
    check_parity();
    check_negative_guards();
    check_scope_flags_and_validation();
    std::cout << "Stage 4AO-C Ricci trace/free assembly tests passed\n";
    return 0;
}
