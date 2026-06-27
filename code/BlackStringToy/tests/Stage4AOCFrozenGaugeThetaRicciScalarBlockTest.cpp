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

double expected_theta_output(
    const RicciAssembly::TraceFreeRicciAssembly &assembly)
{
    return 0.5 * assembly.scalar_trace();
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

void check_pointwise_theta_ricci_scalar_block()
{
    const auto assembly = compute_ricci_assembly(make_common_perturbation(2.0));
    const auto output =
        Operator::apply_theta_ricci_scalar_insertion_at_point(assembly);
    const double tolerance = 1.0e-14;
    const double expected = expected_theta_output(assembly);

    require_close("Theta receives +0.5 delta R", output.value(Variable::Theta),
                  expected, tolerance);
    require_close("Theta Ricci uses assembled scalar trace",
                  output.value(Variable::Theta),
                  0.5 * assembly.scalar_trace(), tolerance);
    require_true("sample has nonzero hidden ww trace contribution",
                 std::abs(assembly.raw_rww()) > 1.0e-12);
    require_true("sample has nonzero raw R_xz for negative guard",
                 std::abs(assembly.raw_rxz()) > 1.0e-12);

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::Theta)
        {
            require_close("non-Theta output is untouched",
                          output.value(variable), 0.0, 0.0);
        }
    }
}

void check_grid_theta_ricci_scalar_block()
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
        Operator::apply_theta_ricci_scalar_insertion_block(grid, assemblies);
    require_true("Theta Ricci scalar result is partial only",
                 result.partial_operator_only());
    require_true(
        "Theta Ricci scalar block has no derivative boundary placeholders",
        !result.boundary_values_are_placeholders());
    require_true("Theta Ricci scalar block covers first grid point",
                 result.first_valid_index() == 0);
    require_true("Theta Ricci scalar block covers last grid point",
                 result.last_valid_index() == grid.points() - 1);

    double max_error = 0.0;
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        max_error = std::max(
            max_error,
            std::abs(result.at(i).value(Variable::Theta) -
                     expected_theta_output(assemblies[i])));
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            if (variable != Variable::Theta)
            {
                max_error = std::max(max_error,
                                     std::abs(result.at(i).value(variable)));
            }
        }
    }
    require_close("grid Theta Ricci scalar block matches pointwise oracles",
                  max_error, 0.0, 1.0e-14);
}

void check_negative_guards()
{
    const auto assembly = compute_ricci_assembly(make_common_perturbation(2.0));
    const double actual =
        Operator::apply_theta_ricci_scalar_insertion_at_point(assembly)
            .value(Variable::Theta);

    require_true("wrong sign would fail",
                 std::abs(actual + 0.5 * assembly.scalar_trace()) > 1.0e-6);
    require_true("missing factor 0.5 would fail",
                 std::abs(actual - assembly.scalar_trace()) > 1.0e-6);

    const double missing_hidden_multiplicity =
        0.5 * (assembly.raw_rxx() + assembly.raw_rzz() + assembly.raw_rww());
    require_true("dropping hidden multiplicity would fail",
                 std::abs(actual - missing_hidden_multiplicity) > 1.0e-6);

    const double spurious_rxz_trace =
        0.5 * (assembly.scalar_trace() + assembly.raw_rxz());
    require_true("incorrectly adding R_xz would fail",
                 std::abs(actual - spurious_rxz_trace) > 1.0e-6);

    const auto output =
        Operator::apply_theta_ricci_scalar_insertion_at_point(assembly);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::Theta)
        {
            require_close("touching non-Theta outputs would fail",
                          output.value(variable), 0.0, 0.0);
        }
    }

    require_true("pretending full Theta RHS complete would fail",
                 !Operator::variable_rhs_complete(Variable::Theta));
    require_true("Theta constraint/damping terms remain deferred",
                 Operator::rhs_piece_status(
                     Variable::Theta,
                     Operator::RhsPiece::theta_constraint_terms) ==
                     Operator::PieceStatus::requires_modified_cartoon_rhs);
}

void check_completion_guards()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("Theta Ricci scalar insertion block implemented",
                 Operator::theta_ricci_scalar_insertion_block_implemented);
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
    check_state_order();
    check_pointwise_theta_ricci_scalar_block();
    check_grid_theta_ricci_scalar_block();
    check_negative_guards();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C Theta Ricci scalar insertion block\n";
    return 0;
}
