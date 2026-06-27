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
    return 2.0 + 0.75 * static_cast<double>(slot);
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

double locked_coefficient(const Variable variable, const double lambda)
{
    switch (variable)
    {
    case Variable::h_xx:
    case Variable::A_xx:
        return -7.0 * lambda / 4.0;
    case Variable::h_xz:
    case Variable::A_xz:
        return -5.0 * lambda / 4.0;
    case Variable::h_zz:
    case Variable::A_zz:
        return -3.0 * lambda / 4.0;
    case Variable::h_ww:
    case Variable::A_ww:
        return 5.0 * lambda / 4.0;
    case Variable::chi:
    case Variable::K:
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

void check_pointwise_tensor_stretching()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = std::sqrt(r0 / (x * x * x));
    const auto input = make_nonzero_vector();
    const auto output =
        Operator::apply_tensor_shift_stretching_at_point(r0, x, input);

    double max_error = 0.0;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const double expected =
            locked_coefficient(variable, lambda) * input.value(variable);
        const double actual = output.value(variable);
        max_error = std::max(max_error, std::abs(actual - expected));
    }
    require_close("tensor stretching coefficients match locked oracles",
                  max_error, 0.0, 2.0e-14);

    require_close("h_xx coefficient is -7 lambda / 4",
                  output.value(Variable::h_xx) / input.value(Variable::h_xx),
                  -7.0 * lambda / 4.0, 1.0e-15);
    require_close("h_xz coefficient is -5 lambda / 4",
                  output.value(Variable::h_xz) / input.value(Variable::h_xz),
                  -5.0 * lambda / 4.0, 1.0e-15);
    require_close("h_zz coefficient is -3 lambda / 4",
                  output.value(Variable::h_zz) / input.value(Variable::h_zz),
                  -3.0 * lambda / 4.0, 1.0e-15);
    require_close("h_ww hidden coefficient is +5 lambda / 4",
                  output.value(Variable::h_ww) / input.value(Variable::h_ww),
                  5.0 * lambda / 4.0, 1.0e-15);
    require_close("A_ww hidden coefficient is +5 lambda / 4",
                  output.value(Variable::A_ww) / input.value(Variable::A_ww),
                  5.0 * lambda / 4.0, 1.0e-15);

    require_close("chi gets no tensor stretching", output.value(Variable::chi),
                  0.0, 0.0);
    require_close("K gets no tensor stretching", output.value(Variable::K),
                  0.0, 0.0);
    require_close("Theta gets no tensor stretching",
                  output.value(Variable::Theta), 0.0, 0.0);
    require_close("hat_Gamma^x gets no tensor stretching",
                  output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("hat_Gamma^z gets no tensor stretching",
                  output.value(Variable::hat_Gamma_z), 0.0, 0.0);
}

void check_grid_tensor_stretching()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 16);
    std::vector<Vector> input;
    input.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        input.push_back(make_nonzero_vector());
    }

    const auto result = Operator::apply_tensor_shift_stretching_block(grid, input);
    require_true("tensor stretching result is partial only",
                 result.partial_operator_only());
    require_true("tensor stretching has no derivative boundary placeholders",
                 !result.boundary_values_are_placeholders());
    require_true("tensor stretching covers first grid point",
                 result.first_valid_index() == 0);
    require_true("tensor stretching covers last grid point",
                 result.last_valid_index() == grid.points() - 1);

    double max_error = 0.0;
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        const double lambda =
            std::sqrt(domain.r0() / (grid.x(i) * grid.x(i) * grid.x(i)));
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            const double expected =
                locked_coefficient(variable, lambda) * input[i].value(variable);
            const double actual = result.at(i).value(variable);
            max_error = std::max(max_error, std::abs(actual - expected));
        }
    }
    require_close("grid tensor stretching matches pointwise coefficients",
                  max_error, 0.0, 3.0e-14);
}

void check_negative_guards()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = std::sqrt(r0 / (x * x * x));
    const auto input = make_nonzero_vector();
    const auto output =
        Operator::apply_tensor_shift_stretching_at_point(r0, x, input);

    const double hww_without_hidden_stretching =
        (-3.0 * lambda / 4.0) * input.value(Variable::h_ww);
    const double aww_without_hidden_stretching =
        (-3.0 * lambda / 4.0) * input.value(Variable::A_ww);
    require_true("dropping hidden h_ww stretching would fail",
                 std::abs(output.value(Variable::h_ww) -
                          hww_without_hidden_stretching) > 1.0);
    require_true("dropping hidden A_ww stretching would fail",
                 std::abs(output.value(Variable::A_ww) -
                          aww_without_hidden_stretching) > 1.0);

    const double div_beta = 1.5 * lambda;
    const double wrong_trace_coefficient = 2.0 / 3.0;
    const double wrong_hzz_trace =
        -wrong_trace_coefficient * div_beta * input.value(Variable::h_zz);
    require_true("using d=3 trace coefficient would fail",
                 std::abs(output.value(Variable::h_zz) - wrong_hzz_trace) >
                     1.0e-1);

    const double wrong_sign_dx_beta_hxx =
        (lambda - 0.5 * div_beta) * input.value(Variable::h_xx);
    require_true("wrong sign for partial_x beta^x would fail",
                 std::abs(output.value(Variable::h_xx) -
                          wrong_sign_dx_beta_hxx) > 1.0);

    const double accidental_scalar_stretch =
        (-7.0 * lambda / 4.0) * input.value(Variable::chi);
    require_true("accidental scalar stretching would fail",
                 std::abs(output.value(Variable::chi) -
                          accidental_scalar_stretch) > 1.0e-1);
    const double accidental_hat_gamma_stretch =
        (-5.0 * lambda / 4.0) * input.value(Variable::hat_Gamma_z);
    require_true("accidental hat_Gamma stretching would fail",
                 std::abs(output.value(Variable::hat_Gamma_z) -
                          accidental_hat_gamma_stretch) > 1.0);
}

void check_completion_guards()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("tensor shift-stretching block implemented",
                 Operator::tensor_shift_stretching_block_implemented);
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
    check_pointwise_tensor_stretching();
    check_grid_tensor_stretching();
    check_negative_guards();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C tensor shift-stretching block\n";
    return 0;
}
