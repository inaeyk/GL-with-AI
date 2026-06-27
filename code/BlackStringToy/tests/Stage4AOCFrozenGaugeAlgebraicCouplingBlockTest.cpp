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

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;
using ApplyResult = Operator::FrozenGaugeApplyResult;

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

double value_for_slot(const std::size_t slot)
{
    return 1.25 + 0.5 * static_cast<double>(slot);
}

Vector make_nonzero_vector()
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (std::size_t slot = 0; slot < values.size(); ++slot)
    {
        values[slot] = value_for_slot(slot);
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

double expected_output(const Variable variable, const Vector &input)
{
    switch (variable)
    {
    case Variable::chi:
        return 0.5 * input.value(Variable::K);
    case Variable::h_xx:
        return -2.0 * input.value(Variable::A_xx);
    case Variable::h_xz:
        return -2.0 * input.value(Variable::A_xz);
    case Variable::h_zz:
        return -2.0 * input.value(Variable::A_zz);
    case Variable::h_ww:
        return -2.0 * input.value(Variable::A_ww);
    case Variable::K:
    case Variable::A_xx:
    case Variable::A_xz:
    case Variable::A_zz:
    case Variable::A_ww:
    case Variable::Theta:
    case Variable::hat_Gamma_x:
    case Variable::hat_Gamma_z:
        return 0.0;
    }
    return 0.0;
}

void check_state_order()
{
    static_assert(!std::is_aggregate<Vector>::value,
                  "Stage 4AO-C vector package is non-aggregate");
    static_assert(!std::is_aggregate<ApplyResult>::value,
                  "Stage 4AO-C apply result is non-aggregate");
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

void check_pointwise_algebraic_coupling()
{
    const auto input = make_nonzero_vector();
    const auto output =
        Operator::apply_algebraic_metric_chi_coupling_at_point(input);

    double max_error = 0.0;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        max_error =
            std::max(max_error,
                     std::abs(output.value(variable) -
                              expected_output(variable, input)));
    }
    require_close("algebraic coupling matches locked h<-A and chi<-K oracles",
                  max_error, 0.0, 0.0);

    require_close("h_xx receives -2 A_xx", output.value(Variable::h_xx),
                  -2.0 * input.value(Variable::A_xx), 0.0);
    require_close("h_xz receives -2 A_xz", output.value(Variable::h_xz),
                  -2.0 * input.value(Variable::A_xz), 0.0);
    require_close("h_zz receives -2 A_zz", output.value(Variable::h_zz),
                  -2.0 * input.value(Variable::A_zz), 0.0);
    require_close("h_ww receives -2 A_ww", output.value(Variable::h_ww),
                  -2.0 * input.value(Variable::A_ww), 0.0);
    require_close("chi receives +K/2", output.value(Variable::chi),
                  0.5 * input.value(Variable::K), 0.0);

    require_close("A_xx receives no reciprocal h_xx coupling",
                  output.value(Variable::A_xx), 0.0, 0.0);
    require_close("A_xz receives no reciprocal h_xz coupling",
                  output.value(Variable::A_xz), 0.0, 0.0);
    require_close("A_zz receives no reciprocal h_zz coupling",
                  output.value(Variable::A_zz), 0.0, 0.0);
    require_close("A_ww receives no reciprocal h_ww coupling",
                  output.value(Variable::A_ww), 0.0, 0.0);
    require_close("K output is zero in this block", output.value(Variable::K),
                  0.0, 0.0);
    require_close("Theta output is zero in this block",
                  output.value(Variable::Theta), 0.0, 0.0);
    require_close("hat_Gamma^x output is zero in this block",
                  output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("hat_Gamma^z output is zero in this block",
                  output.value(Variable::hat_Gamma_z), 0.0, 0.0);
}

void check_grid_algebraic_coupling()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 16);
    std::vector<Vector> input;
    input.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        input.push_back(make_nonzero_vector());
    }

    const auto result =
        Operator::apply_algebraic_metric_chi_coupling_block(grid, input);
    require_true("algebraic coupling result is partial only",
                 result.partial_operator_only());
    require_true("algebraic coupling has no derivative boundary placeholders",
                 !result.boundary_values_are_placeholders());
    require_true("algebraic coupling covers first grid point",
                 result.first_valid_index() == 0);
    require_true("algebraic coupling covers last grid point",
                 result.last_valid_index() == grid.points() - 1);

    double max_error = 0.0;
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            max_error =
                std::max(max_error,
                         std::abs(result.at(i).value(variable) -
                                  expected_output(variable, input[i])));
        }
    }
    require_close("grid algebraic coupling matches pointwise oracles",
                  max_error, 0.0, 0.0);
}

void check_negative_guards()
{
    const auto input = make_nonzero_vector();
    const auto output =
        Operator::apply_algebraic_metric_chi_coupling_at_point(input);

    const double wrong_sign_hxx = 2.0 * input.value(Variable::A_xx);
    require_true("wrong sign in -2 A_xx would fail",
                 std::abs(output.value(Variable::h_xx) - wrong_sign_hxx) >
                     1.0);

    const double wrong_coefficient_hww = -1.0 * input.value(Variable::A_ww);
    require_true("wrong -1 coefficient for h_ww would fail",
                 std::abs(output.value(Variable::h_ww) -
                          wrong_coefficient_hww) > 1.0);

    const double wrong_d3_chi = (2.0 / 3.0) * input.value(Variable::K);
    require_true("wrong d=3 chi coefficient would fail",
                 std::abs(output.value(Variable::chi) - wrong_d3_chi) >
                     1.0e-1);

    const double accidental_a_from_h = -2.0 * input.value(Variable::h_xz);
    require_true("accidental A_xz <- h_xz coupling would fail",
                 std::abs(output.value(Variable::A_xz) -
                          accidental_a_from_h) > 1.0);

    const double accidental_k = 0.5 * input.value(Variable::chi);
    require_true("accidental K output coupling would fail",
                 std::abs(output.value(Variable::K) - accidental_k) > 1.0e-1);

    const double accidental_theta = input.value(Variable::Theta);
    require_true("accidental Theta output coupling would fail",
                 std::abs(output.value(Variable::Theta) -
                          accidental_theta) > 1.0);

    const double accidental_hat_gamma =
        -2.0 * input.value(Variable::hat_Gamma_x);
    require_true("accidental hat_Gamma output coupling would fail",
                 std::abs(output.value(Variable::hat_Gamma_x) -
                          accidental_hat_gamma) > 1.0);
}

void check_completion_guards()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("algebraic metric/chi coupling block implemented",
                 Operator::algebraic_metric_chi_coupling_block_implemented);
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
    check_pointwise_algebraic_coupling();
    check_grid_algebraic_coupling();
    check_negative_guards();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C algebraic metric/chi coupling block\n";
    return 0;
}
