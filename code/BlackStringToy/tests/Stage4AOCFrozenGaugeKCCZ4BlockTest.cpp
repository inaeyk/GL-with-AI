#include "CartoonRicciBridge.hpp"
#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace
{
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciAssembly = BlackStringToy::Stage4AOFrozenGaugeRicciAssembly;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;
namespace HiddenRww = BlackStringToy::Stage4AOFrozenGaugeHiddenRww;
namespace VisibleRxx = BlackStringToy::Stage4AOFrozenGaugeVisibleRxx;
namespace VisibleRxz = BlackStringToy::Stage4AOFrozenGaugeVisibleRxz;
namespace VisibleRzz = BlackStringToy::Stage4AOFrozenGaugeVisibleRzz;

using Metric = Algebra::ConformalMetric;
using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

struct Perturbation
{
    double x;
    Metric delta_h;
    Ricci::MetricDerivatives delta_h_derivatives;
    double delta_chi;
    Ricci::ScalarDerivatives delta_chi_derivatives;
    double delta_k;
    double delta_theta;
    Metric delta_a;
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

Perturbation zero_perturbation(const double x)
{
    return {x, zero_metric(), zero_metric_derivatives(), 0.0,
            zero_scalar_derivatives(), 0.0, 0.0, zero_metric()};
}

Perturbation metric_chi_perturbation(const double x)
{
    return {x,
            metric(0.7, -0.4, 0.2, -0.9),
            {metric(1.3, -0.8, 0.6, -1.1),
             metric(-0.5, 1.7, -0.3, 0.9),
             metric(0.4, -1.2, 0.8, -0.7),
             metric(-1.4, 0.5, 1.1, -0.2),
             metric(0.9, -1.3, 0.7, 1.5)},
            0.1,
            {{0.6, -0.4}, {-0.7, 1.2, 0.5}},
            0.0,
            0.0,
            zero_metric()};
}

Perturbation pure_hxz_perturbation(const double x)
{
    return {x,
            metric(0.0, 0.8, 0.0, 0.0),
            {metric(0.0, -0.6, 0.0, 0.0),
             metric(0.0, 0.7, 0.0, 0.0),
             metric(0.0, 0.4, 0.0, 0.0),
             metric(0.0, -0.9, 0.0, 0.0),
             metric(0.0, 1.1, 0.0, 0.0)},
            0.0,
            zero_scalar_derivatives(),
            0.0,
            0.0,
            zero_metric()};
}

Perturbation combined_perturbation(const double x)
{
    auto perturbation = metric_chi_perturbation(x);
    perturbation.delta_k = 0.7;
    perturbation.delta_theta = -0.4;
    perturbation.delta_a = metric(0.9, -0.6, 0.2, -0.3);
    return perturbation;
}

Vector make_state_vector(const Perturbation &perturbation)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    values[Operator::variable_index(Variable::chi)] = perturbation.delta_chi;
    values[Operator::variable_index(Variable::h_xx)] = perturbation.delta_h.xx;
    values[Operator::variable_index(Variable::h_xz)] = perturbation.delta_h.xz;
    values[Operator::variable_index(Variable::h_zz)] = perturbation.delta_h.zz;
    values[Operator::variable_index(Variable::h_ww)] = perturbation.delta_h.ww;
    values[Operator::variable_index(Variable::K)] = perturbation.delta_k;
    values[Operator::variable_index(Variable::A_xx)] = perturbation.delta_a.xx;
    values[Operator::variable_index(Variable::A_xz)] = perturbation.delta_a.xz;
    values[Operator::variable_index(Variable::A_zz)] = perturbation.delta_a.zz;
    values[Operator::variable_index(Variable::A_ww)] = perturbation.delta_a.ww;
    values[Operator::variable_index(Variable::Theta)] =
        perturbation.delta_theta;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

RicciAssembly::TraceFreeRicciAssembly
compute_linear_ricci_assembly(const Perturbation &perturbation)
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

    return RicciAssembly::assemble_trace_free_ricci(
        RicciAssembly::make_raw_ricci_components(
            VisibleRxx::compute_visible_physical_delta_rxx(rxx_input),
            VisibleRxz::compute_visible_physical_delta_rxz(rxz_input),
            VisibleRzz::compute_visible_physical_delta_rzz(rzz_input),
            HiddenRww::compute_hidden_physical_delta_rww(rww_input)));
}

Ricci::CartoonRicciInputs make_stage4g_inputs(
    const Perturbation &perturbation, const double epsilon)
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

double nonlinear_physical_ricci_scalar(const Perturbation &perturbation,
                                        const double epsilon)
{
    const auto inputs = make_stage4g_inputs(perturbation, epsilon);
    const auto ricci = Ricci::compute_metric_derivative_ricci(inputs);
    return RicciBridge::make_rhs_ricci_bridge_contract(
               {ricci, Algebra::inverse(inputs.h), inputs.chi})
        .physical_scalar;
}

double nonlinear_ccz4_k_expression(const Perturbation &perturbation,
                                    const double r0, const double epsilon)
{
    const double lambda = Operator::lambda_gp(r0, perturbation.x);
    const double k = 1.5 * lambda + epsilon * perturbation.delta_k;
    const double theta = epsilon * perturbation.delta_theta;
    return nonlinear_physical_ricci_scalar(perturbation, epsilon) +
           k * (k - 2.0 * theta);
}

double central_difference_ccz4(const Perturbation &perturbation,
                               const double r0, const double epsilon)
{
    return (nonlinear_ccz4_k_expression(perturbation, r0, epsilon) -
            nonlinear_ccz4_k_expression(perturbation, r0, -epsilon)) /
           (2.0 * epsilon);
}

double conformal_a_squared(const Metric &a, const Metric &h)
{
    const auto inv = Algebra::inverse(h);
    const double a_uu_xx = inv.xx * inv.xx * a.xx +
                           2.0 * inv.xx * inv.xz * a.xz +
                           inv.xz * inv.xz * a.zz;
    const double a_uu_xz = inv.xx * inv.xz * a.xx +
                           (inv.xx * inv.zz + inv.xz * inv.xz) * a.xz +
                           inv.xz * inv.zz * a.zz;
    const double a_uu_zz = inv.xz * inv.xz * a.xx +
                           2.0 * inv.xz * inv.zz * a.xz +
                           inv.zz * inv.zz * a.zz;
    const double a_uu_ww = inv.ww * inv.ww * a.ww;
    return a.xx * a_uu_xx + 2.0 * a.xz * a_uu_xz + a.zz * a_uu_zz +
           2.0 * a.ww * a_uu_ww;
}

double nonlinear_rejected_bssn_expression(const Perturbation &perturbation,
                                           const double r0,
                                           const double epsilon)
{
    const double lambda = Operator::lambda_gp(r0, perturbation.x);
    const auto &dh = perturbation.delta_h;
    const auto &da = perturbation.delta_a;
    const Metric h = metric(1.0 + epsilon * dh.xx, epsilon * dh.xz,
                            1.0 + epsilon * dh.zz,
                            1.0 + epsilon * dh.ww);
    const Metric a = metric(-7.0 * lambda / 8.0 + epsilon * da.xx,
                            epsilon * da.xz,
                            -3.0 * lambda / 8.0 + epsilon * da.zz,
                            5.0 * lambda / 8.0 + epsilon * da.ww);
    const double k = 1.5 * lambda + epsilon * perturbation.delta_k;
    return conformal_a_squared(a, h) + 0.25 * k * k;
}

double central_difference_rejected_bssn(const Perturbation &perturbation,
                                        const double r0,
                                        const double epsilon)
{
    return (nonlinear_rejected_bssn_expression(perturbation, r0, epsilon) -
            nonlinear_rejected_bssn_expression(perturbation, r0, -epsilon)) /
           (2.0 * epsilon);
}

double implemented_k_output(const Perturbation &perturbation, const double r0)
{
    const auto state = make_state_vector(perturbation);
    const auto algebraic = Operator::apply_k_equation_ccz4_k_theta_at_point(
        r0, perturbation.x, state);
    const auto ricci =
        Operator::apply_k_equation_ricci_scalar_insertion_at_point(
            compute_linear_ricci_assembly(perturbation));
    return algebraic.value(Variable::K) + ricci.value(Variable::K);
}

void require_only_k_output(const Vector &output)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::K)
        {
            require_close("non-K output is untouched", output.value(variable),
                          0.0, 0.0);
        }
    }
}

void check_exact_ccz4_k_theta_coefficients()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = Operator::lambda_gp(r0, x);

    auto pure_k = zero_perturbation(x);
    pure_k.delta_k = 0.8;
    const auto pure_k_output = Operator::apply_k_equation_ccz4_k_theta_at_point(
        r0, x, make_state_vector(pure_k));
    require_close("pure delta K gives +3 lambda delta K",
                  pure_k_output.value(Variable::K),
                  3.0 * lambda * pure_k.delta_k, 1.0e-14);
    require_only_k_output(pure_k_output);

    auto pure_theta = zero_perturbation(x);
    pure_theta.delta_theta = -0.65;
    const auto pure_theta_output =
        Operator::apply_k_equation_ccz4_k_theta_at_point(
            r0, x, make_state_vector(pure_theta));
    require_close("pure delta Theta gives -3 lambda delta Theta",
                  pure_theta_output.value(Variable::K),
                  -3.0 * lambda * pure_theta.delta_theta, 1.0e-14);
    require_only_k_output(pure_theta_output);

    auto pure_a = zero_perturbation(x);
    pure_a.delta_a = metric(0.9, -0.6, 0.2, -0.3);
    require_close("pure delta A gives zero direct CCZ4 K contribution",
                  implemented_k_output(pure_a, r0), 0.0, 1.0e-14);
}

void check_ricci_insertion_scope()
{
    const double r0 = 1.0;
    const auto metric_case = metric_chi_perturbation(2.0);
    const auto assembly = compute_linear_ricci_assembly(metric_case);
    const auto state = make_state_vector(metric_case);
    const auto algebraic = Operator::apply_k_equation_ccz4_k_theta_at_point(
        r0, metric_case.x, state);
    const auto ricci =
        Operator::apply_k_equation_ricci_scalar_insertion_at_point(assembly);
    require_close("metric/chi has no direct K(K-2Theta) contribution",
                  algebraic.value(Variable::K), 0.0, 0.0);
    require_close("metric/chi enters K only through delta R",
                  ricci.value(Variable::K), assembly.scalar_trace(), 0.0);
    require_only_k_output(ricci);

    const auto hxz_case = pure_hxz_perturbation(2.0);
    const auto hxz_assembly = compute_linear_ricci_assembly(hxz_case);
    require_true("pure delta h_xz has a nonzero scalar-Ricci oracle",
                 std::abs(hxz_assembly.scalar_trace()) > 1.0e-6);
    require_close("pure delta h_xz enters only through scalar Ricci trace",
                  implemented_k_output(hxz_case, r0),
                  hxz_assembly.scalar_trace(), 1.0e-14);
}

void check_ccz4_oracle_plateau(const std::string &label,
                               const Perturbation &perturbation)
{
    const double r0 = 1.0;
    const double implemented = implemented_k_output(perturbation, r0);
    const std::array<double, 5> epsilons = {1.0e-2, 1.0e-4, 1.0e-5,
                                           1.0e-6, 1.0e-7};
    double middle_max_error = 0.0;
    for (const double epsilon : epsilons)
    {
        const double oracle =
            central_difference_ccz4(perturbation, r0, epsilon);
        const double error = std::abs(implemented - oracle);
        std::cout << "INFO " << label << " eps=" << epsilon
                  << " ccz4_error=" << error << "\n";
        if (epsilon == 1.0e-5 || epsilon == 1.0e-6)
        {
            middle_max_error = std::max(middle_max_error, error);
        }
    }
    require_close(label + " independent CCZ4 epsilon plateau",
                  middle_max_error, 0.0, 2.0e-7);

    const double rejected_bssn =
        central_difference_rejected_bssn(perturbation, r0, 1.0e-6);
    require_true(label + " discriminates against rejected USE_BSSN branch",
                 std::abs(implemented - rejected_bssn) > 1.0e-4);
}

void check_branch_discriminating_oracles()
{
    auto pure_k = zero_perturbation(2.0);
    pure_k.delta_k = 0.8;
    check_ccz4_oracle_plateau("pure delta K", pure_k);

    auto pure_theta = zero_perturbation(2.0);
    pure_theta.delta_theta = -0.65;
    check_ccz4_oracle_plateau("pure delta Theta", pure_theta);

    auto pure_a = zero_perturbation(2.0);
    pure_a.delta_a = metric(0.9, -0.6, 0.2, -0.3);
    check_ccz4_oracle_plateau("pure delta A", pure_a);

    check_ccz4_oracle_plateau("pure metric/chi",
                               metric_chi_perturbation(2.0));
    check_ccz4_oracle_plateau("pure delta h_xz",
                               pure_hxz_perturbation(2.0));
    check_ccz4_oracle_plateau("combined perturbation",
                               combined_perturbation(2.0));
}

void check_grid_blocks()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 16);
    std::vector<Vector> states;
    std::vector<RicciAssembly::TraceFreeRicciAssembly> assemblies;
    states.reserve(grid.points());
    assemblies.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        const auto perturbation = combined_perturbation(grid.x(i));
        states.push_back(make_state_vector(perturbation));
        assemblies.push_back(compute_linear_ricci_assembly(perturbation));
    }

    const auto algebraic =
        Operator::apply_k_equation_ccz4_k_theta_block(grid, states);
    const auto ricci =
        Operator::apply_k_equation_ricci_scalar_insertion_block(grid,
                                                                 assemblies);
    require_true("K CCZ4 grid block is partial only",
                 algebraic.partial_operator_only());
    require_true("K Ricci grid block is partial only",
                 ricci.partial_operator_only());
    require_true("K grid blocks cover all points without boundary placeholders",
                 algebraic.first_valid_index() == 0 &&
                     ricci.first_valid_index() == 0 &&
                     algebraic.last_valid_index() == grid.points() - 1 &&
                     ricci.last_valid_index() == grid.points() - 1 &&
                     !algebraic.boundary_values_are_placeholders() &&
                     !ricci.boundary_values_are_placeholders());
}

void check_completion_guards()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("K CCZ4 K/Theta block implemented",
                 Operator::k_equation_ccz4_k_theta_block_implemented);
    require_true("K physical-Ricci insertion implemented",
                 Operator::k_equation_ricci_scalar_insertion_block_implemented);
    require_true("K Z/hat-Gamma Ricci contributions remain missing",
                 !Operator::k_equation_z_ricci_contributions_implemented);
    require_true("simple K/Theta damping is a separate implemented block",
                 Operator::ccz4_k_theta_damping_insertion_block_implemented);
    require_true("first hat-Gamma Z/kappa block is implemented",
                 Operator::
                     hat_gamma_z4_kappa_shift_gradient_block_implemented);
    require_true("complete hat-Gamma RHS is implemented separately",
                 Operator::hat_gamma_rhs_block_implemented);
    require_true("hat_Gamma^x RHS is complete",
                 Operator::variable_rhs_complete(Variable::hat_Gamma_x));
    require_true("hat_Gamma^z RHS is complete",
                 Operator::variable_rhs_complete(Variable::hat_Gamma_z));
    require_true("K lapse Hessian vanishes in frozen gauge",
                 Operator::k_equation_lapse_hessian_vanishes_in_frozen_gauge);
    require_true("Lambda remains locked to zero",
                 Operator::cosmological_constant_locked_to_zero &&
                     !Operator::k_equation_cosmological_terms_implemented);
    require_true("K RHS remains incomplete",
                 !Operator::variable_rhs_complete(Variable::K));
    require_true("complete frozen-gauge operator remains false",
                 !contract.complete_operator_implemented());
    require_true("eigensolver remains disallowed",
                 !contract.eigensolver_allowed());
    require_true("shift-invert remains absent",
                 !Operator::shift_invert_implemented);
    require_true("threshold search remains absent",
                 !Operator::threshold_search_implemented);
}

} // namespace

int main()
{
    check_exact_ccz4_k_theta_coefficients();
    check_ricci_insertion_scope();
    check_branch_discriminating_oracles();
    check_grid_blocks();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C selected CCZ4 K equation pieces\n";
    return 0;
}
