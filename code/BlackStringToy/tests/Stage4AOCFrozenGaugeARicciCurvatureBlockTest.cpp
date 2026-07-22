#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciAssembly = BlackStringToy::Stage4AOFrozenGaugeRicciAssembly;
namespace HiddenRww = BlackStringToy::Stage4AOFrozenGaugeHiddenRww;
namespace VisibleRxx = BlackStringToy::Stage4AOFrozenGaugeVisibleRxx;
namespace VisibleRxz = BlackStringToy::Stage4AOFrozenGaugeVisibleRxz;
namespace VisibleRzz = BlackStringToy::Stage4AOFrozenGaugeVisibleRzz;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;
using ApplyResult = Operator::FrozenGaugeApplyResult;
using Metric = BlackStringToy::ConformalCartoonAlgebra::ConformalMetric;

struct CommonPerturbation
{
    double x;
    Metric delta_h;
    Ricci::MetricDerivatives delta_h_derivatives;
    double delta_chi;
    Ricci::ScalarDerivatives delta_chi_derivatives;
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

constexpr std::array<Variable, 13> expected_state_order = {
    Variable::chi,         Variable::h_xx,        Variable::h_xz,
    Variable::h_zz,        Variable::h_ww,        Variable::K,
    Variable::A_xx,        Variable::A_xz,        Variable::A_zz,
    Variable::A_ww,        Variable::Theta,       Variable::hat_Gamma_x,
    Variable::hat_Gamma_z};

Metric metric(const double xx, const double xz, const double zz,
              const double ww)
{
    return {xx, xz, zz, ww};
}

CommonPerturbation make_common_perturbation(const double x)
{
    return {
        x,
        metric(0.7, -0.4, 0.2, -0.9),
        {metric(1.3, -0.8, 0.6, -1.1),
         metric(-0.5, 1.7, -0.3, 0.9),
         metric(0.4, -1.2, 0.8, -0.7),
         metric(-1.4, 0.5, 1.1, -0.2),
         metric(0.9, -1.3, 0.7, 1.5)},
        0.1,
        {{0.6, -0.4}, {-0.7, 1.2, 0.5}}};
}

RicciAssembly::TraceFreeRicciAssembly
compute_ricci_assembly(const CommonPerturbation &perturbation)
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

void check_state_order()
{
    static_assert(!std::is_aggregate<Vector>::value,
                  "Stage 4AO-C vector package is non-aggregate");
    static_assert(!std::is_aggregate<ApplyResult>::value,
                  "Stage 4AO-C apply result is non-aggregate");
    static_assert(
        !std::is_aggregate<RicciAssembly::TraceFreeRicciAssembly>::value,
        "Stage 4AO-C Ricci assembly result is non-aggregate");
    for (std::size_t i = 0; i < expected_state_order.size(); ++i)
    {
        if (Operator::frozen_gauge_state_vector[i] != expected_state_order[i] ||
            Operator::variable_index(expected_state_order[i]) != i)
        {
            fail("exact state-vector order",
                 "slot " + std::to_string(i) + " is not locked");
        }
    }
    require_true("exact state-vector order is locked", true);
}

void require_only_a_outputs(const Vector &output, const double tolerance)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (!Operator::is_a_variable(variable))
        {
            require_close("non-A output is untouched", output.value(variable),
                          0.0, tolerance);
        }
    }
}

void check_pointwise_a_ricci_curvature_block()
{
    const double x = 2.0;
    const auto assembly = compute_ricci_assembly(make_common_perturbation(x));
    const auto output =
        Operator::apply_a_equation_ricci_curvature_insertion_at_point(
            assembly);
    constexpr double tolerance = 1.0e-14;

    require_close("A_xx receives tf_xx", output.value(Variable::A_xx),
                  assembly.tf_xx(), tolerance);
    require_close("A_xz receives tf_xz", output.value(Variable::A_xz),
                  assembly.tf_xz(), tolerance);
    require_close("A_zz receives tf_zz", output.value(Variable::A_zz),
                  assembly.tf_zz(), tolerance);
    require_close("A_ww receives tf_ww once", output.value(Variable::A_ww),
                  assembly.tf_ww(), tolerance);
    require_only_a_outputs(output, 0.0);

    require_close("inserted A-source is trace-free",
                  output.value(Variable::A_xx) +
                      output.value(Variable::A_zz) +
                      2.0 * output.value(Variable::A_ww),
                  0.0, tolerance);
    require_close("A_xz source equals raw R_xz",
                  output.value(Variable::A_xz), assembly.raw_rxz(),
                  tolerance);
    require_close("trace-free delta A convention is preserved",
                  Operator::delta_trace_a_at_point(1.0, x, output), 0.0,
                  tolerance);

    require_true("sample has nonzero Ricci trace",
                 std::abs(assembly.scalar_trace()) > 1.0e-12);
    require_true("sample has nonzero tf_ww for multiplicity guard",
                 std::abs(assembly.tf_ww()) > 1.0e-12);
    require_true("sample has nonzero raw R_xz", std::abs(assembly.raw_rxz()) >
                                                1.0e-12);
}

void check_grid_a_ricci_curvature_block()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 16);
    std::vector<RicciAssembly::TraceFreeRicciAssembly> assemblies;
    assemblies.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        assemblies.push_back(
            compute_ricci_assembly(make_common_perturbation(grid.x(i))));
    }

    const auto result =
        Operator::apply_a_equation_ricci_curvature_insertion_block(grid,
                                                                   assemblies);
    require_true("A Ricci curvature result is partial only",
                 result.partial_operator_only());
    require_true("A Ricci curvature block has no derivative boundary placeholders",
                 !result.boundary_values_are_placeholders());
    require_true("A Ricci curvature block covers first grid point",
                 result.first_valid_index() == 0);
    require_true("A Ricci curvature block covers last grid point",
                 result.last_valid_index() == grid.points() - 1);

    double max_error = 0.0;
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        max_error = std::max(max_error,
                             std::abs(result.at(i).value(Variable::A_xx) -
                                      assemblies[i].tf_xx()));
        max_error = std::max(max_error,
                             std::abs(result.at(i).value(Variable::A_xz) -
                                      assemblies[i].tf_xz()));
        max_error = std::max(max_error,
                             std::abs(result.at(i).value(Variable::A_zz) -
                                      assemblies[i].tf_zz()));
        max_error = std::max(max_error,
                             std::abs(result.at(i).value(Variable::A_ww) -
                                      assemblies[i].tf_ww()));
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            if (!Operator::is_a_variable(variable))
            {
                max_error = std::max(max_error,
                                     std::abs(result.at(i).value(variable)));
            }
        }
    }
    require_close("grid A Ricci curvature block matches pointwise oracles",
                  max_error, 0.0, 1.0e-14);
}

void check_negative_guards()
{
    const auto assembly = compute_ricci_assembly(make_common_perturbation(2.0));
    const auto output =
        Operator::apply_a_equation_ricci_curvature_insertion_at_point(
            assembly);
    const double trace = assembly.scalar_trace();

    const double wrong_sign_error =
        std::abs(output.value(Variable::A_xx) + assembly.tf_xx()) +
        std::abs(output.value(Variable::A_xz) + assembly.tf_xz()) +
        std::abs(output.value(Variable::A_zz) + assembly.tf_zz()) +
        std::abs(output.value(Variable::A_ww) + assembly.tf_ww());
    require_true("wrong sign would fail", wrong_sign_error > 1.0e-6);

    const double raw_instead_of_tf_error =
        std::abs(output.value(Variable::A_xx) - assembly.raw_rxx()) +
        std::abs(output.value(Variable::A_zz) - assembly.raw_rzz()) +
        std::abs(output.value(Variable::A_ww) - assembly.raw_rww());
    require_true("using raw Ricci instead of trace-free would fail",
                 raw_instead_of_tf_error > 1.0e-6);

    const double wrong_d3_tf_xx = assembly.raw_rxx() - trace / 3.0;
    const double wrong_d3_tf_zz = assembly.raw_rzz() - trace / 3.0;
    const double wrong_d3_tf_ww = assembly.raw_rww() - trace / 3.0;
    const double wrong_d3_error =
        std::abs(output.value(Variable::A_xx) - wrong_d3_tf_xx) +
        std::abs(output.value(Variable::A_zz) - wrong_d3_tf_zz) +
        std::abs(output.value(Variable::A_ww) - wrong_d3_tf_ww);
    require_true("using d=3 in trace-free projection would fail",
                 wrong_d3_error > 1.0e-6);

    require_true("multiplying A_ww by hidden multiplicity again would fail",
                 std::abs(output.value(Variable::A_ww) -
                          2.0 * assembly.tf_ww()) > 1.0e-6);

    require_only_a_outputs(output, 0.0);

    require_true("A RHS is complete only through the combined assembler",
                 Operator::variable_rhs_complete(Variable::A_xx) &&
                     Operator::k_theta_a_final_row_assembly_implemented);
    require_true("complete frozen-gauge operator remains false",
                 !Operator::complete_frozen_gauge_operator_implemented);
    require_true("eigensolver remains disallowed",
                 !Operator::make_default_frozen_gauge_operator_contract()
                      .eigensolver_allowed());
}

void check_completion_guards()
{
    require_true("A Ricci curvature insertion block implemented",
                 Operator::a_equation_ricci_curvature_insertion_block_implemented);
    require_true("A encoded-Z Ricci insertion implemented separately",
                 Operator::a_equation_z_ricci_contributions_implemented);
    require_true("Theta Ricci scalar insertion remains implemented",
                 Operator::theta_ricci_scalar_insertion_block_implemented);
    require_true("complete frozen-gauge operator remains false",
                 !Operator::complete_frozen_gauge_operator_implemented);
    require_true("shift-invert remains absent",
                 !Operator::shift_invert_implemented);
    require_true("threshold search remains absent",
                 !Operator::threshold_search_implemented);
}

} // namespace

int main()
{
    check_state_order();
    check_pointwise_a_ricci_curvature_block();
    check_grid_a_ricci_curvature_block();
    check_negative_guards();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C A Ricci curvature insertion block\n";
    return 0;
}
