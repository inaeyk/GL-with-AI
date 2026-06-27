#include "Stage4AOFrozenGaugeOperator.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

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

Vector make_vector(const std::array<double, Operator::frozen_gauge_state_vector.size()> &values)
{
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector make_basis_vector(const Variable variable, const double amplitude)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    values[Operator::variable_index(variable)] = amplitude;
    return make_vector(values);
}

Vector make_arbitrary_vector()
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    values[Operator::variable_index(Variable::chi)] = 1.25;
    values[Operator::variable_index(Variable::h_xx)] = 0.7;
    values[Operator::variable_index(Variable::h_xz)] = -1.3;
    values[Operator::variable_index(Variable::h_zz)] = 2.1;
    values[Operator::variable_index(Variable::h_ww)] = -0.4;
    values[Operator::variable_index(Variable::K)] = 0.9;
    values[Operator::variable_index(Variable::A_xx)] = 3.0;
    values[Operator::variable_index(Variable::A_xz)] = -4.0;
    values[Operator::variable_index(Variable::A_zz)] = -5.0;
    values[Operator::variable_index(Variable::A_ww)] = 6.0;
    values[Operator::variable_index(Variable::Theta)] = -7.0;
    values[Operator::variable_index(Variable::hat_Gamma_x)] = 8.0;
    values[Operator::variable_index(Variable::hat_Gamma_z)] = -9.0;
    return make_vector(values);
}

double expected_delta_trace_coefficient(const Variable variable,
                                        const double lambda)
{
    switch (variable)
    {
    case Variable::h_xx:
        return 7.0 * lambda / 8.0;
    case Variable::h_zz:
        return 3.0 * lambda / 8.0;
    case Variable::h_ww:
        return -5.0 * lambda / 4.0;
    case Variable::A_xx:
    case Variable::A_zz:
        return 1.0;
    case Variable::A_ww:
        return 2.0;
    case Variable::chi:
    case Variable::h_xz:
    case Variable::K:
    case Variable::A_xz:
    case Variable::Theta:
    case Variable::hat_Gamma_x:
    case Variable::hat_Gamma_z:
        return 0.0;
    }
    return 0.0;
}

double wrong_hidden_multiplicity_trace(const Vector &input, const double lambda)
{
    return input.value(Variable::A_xx) + input.value(Variable::A_zz) +
           input.value(Variable::A_ww) +
           7.0 * lambda * input.value(Variable::h_xx) / 8.0 +
           3.0 * lambda * input.value(Variable::h_zz) / 8.0 -
           5.0 * lambda * input.value(Variable::h_ww) / 8.0;
}

double no_metric_variation_trace(const Vector &input)
{
    return input.value(Variable::A_xx) + input.value(Variable::A_zz) +
           2.0 * input.value(Variable::A_ww);
}

Vector wrong_d3_projection(const Vector &input, const double trace_error)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] = input.value(variable);
    }
    values[Operator::variable_index(Variable::A_xx)] -= trace_error / 3.0;
    values[Operator::variable_index(Variable::A_zz)] -= trace_error / 3.0;
    values[Operator::variable_index(Variable::A_ww)] -= trace_error / 3.0;
    return make_vector(values);
}

void check_state_order()
{
    static_assert(!std::is_aggregate<Vector>::value,
                  "Stage 4AO-C vector package is non-aggregate");
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

void check_delta_trace_coefficients()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = Operator::lambda_gp(r0, x);
    const double tolerance = 1.0e-14;

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        require_close(std::string("delta tr A coefficient for ") +
                          Operator::variable_name(variable),
                      Operator::delta_trace_a_at_point(
                          r0, x, make_basis_vector(variable, 1.0)),
                      expected_delta_trace_coefficient(variable, lambda),
                      tolerance);
    }

    require_close("h_xz does not enter the diagonal GP trace",
                  Operator::delta_trace_a_at_point(
                      r0, x, make_basis_vector(Variable::h_xz, 3.0)),
                  0.0, 0.0);
    require_close("A_xz does not enter the diagonal GP trace",
                  Operator::delta_trace_a_at_point(
                      r0, x, make_basis_vector(Variable::A_xz, 3.0)),
                  0.0, 0.0);
}

void check_projector()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double tolerance = 1.0e-13;
    const auto input = make_arbitrary_vector();
    const double trace_error = Operator::delta_trace_a_at_point(r0, x, input);
    const auto projected =
        Operator::project_delta_a_trace_free_at_point(r0, x, input);

    require_true("arbitrary test data has nonzero trace before projection",
                 std::abs(trace_error) > 1.0e-3);
    require_close("projector removes delta tr A",
                  Operator::delta_trace_a_at_point(r0, x, projected), 0.0,
                  tolerance);

    require_close("projector subtracts trace/4 from A_xx",
                  projected.value(Variable::A_xx),
                  input.value(Variable::A_xx) - trace_error / 4.0,
                  tolerance);
    require_close("projector subtracts trace/4 from A_zz",
                  projected.value(Variable::A_zz),
                  input.value(Variable::A_zz) - trace_error / 4.0,
                  tolerance);
    require_close("projector subtracts trace/4 from A_ww",
                  projected.value(Variable::A_ww),
                  input.value(Variable::A_ww) - trace_error / 4.0,
                  tolerance);
    require_close("projector preserves A_xz", projected.value(Variable::A_xz),
                  input.value(Variable::A_xz), 0.0);

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable == Variable::A_xx || variable == Variable::A_zz ||
            variable == Variable::A_ww)
        {
            continue;
        }
        require_close("projector preserves non-diagonal-A slots",
                      projected.value(variable), input.value(variable), 0.0);
    }

    const auto projected_twice =
        Operator::project_delta_a_trace_free_at_point(r0, x, projected);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        require_close("projector is idempotent", projected_twice.value(variable),
                      projected.value(variable), tolerance);
    }

    const auto already_trace_free = projected;
    const auto unchanged = Operator::project_delta_a_trace_free_at_point(
        r0, x, already_trace_free);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        require_close("trace-free data is unchanged by the projector",
                      unchanged.value(variable),
                      already_trace_free.value(variable), tolerance);
    }
}

void check_negative_guards()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = Operator::lambda_gp(r0, x);
    const auto input = make_arbitrary_vector();
    const double trace_error = Operator::delta_trace_a_at_point(r0, x, input);

    require_true("wrong hidden multiplicity would fail",
                 std::abs(trace_error -
                          wrong_hidden_multiplicity_trace(input, lambda)) >
                     1.0e-3);
    require_true("dropping metric-variation terms would fail",
                 std::abs(trace_error - no_metric_variation_trace(input)) >
                     1.0e-3);

    const auto wrong_projected = wrong_d3_projection(input, trace_error);
    require_true("d=3/simple three-component projection would fail",
                 std::abs(Operator::delta_trace_a_at_point(r0, x,
                                                           wrong_projected)) >
                     1.0e-3);

    require_true("accidental non-A modification would fail",
                 std::abs((input.value(Variable::chi) - trace_error / 4.0) -
                          input.value(Variable::chi)) > 1.0e-3);
}

void check_completion_guards()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true(
        "trace-free delta A projector contract implemented",
        Operator::trace_free_delta_a_projector_contract_implemented);
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
    check_delta_trace_coefficients();
    check_projector();
    check_negative_guards();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C trace-free delta A projector contract\n";
    return 0;
}
