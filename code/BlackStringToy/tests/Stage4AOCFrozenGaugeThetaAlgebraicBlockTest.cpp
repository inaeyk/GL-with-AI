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

Vector make_basis_vector(const Variable variable, const double amplitude)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    values[Operator::variable_index(variable)] = amplitude;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector make_nonzero_vector()
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (std::size_t slot = 0; slot < values.size(); ++slot)
    {
        values[slot] = 0.5 + 0.4375 * static_cast<double>(slot);
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

double expected_coefficient(const Variable variable, const double lambda)
{
    const double lambda_squared = lambda * lambda;
    switch (variable)
    {
    case Variable::h_xx:
        return 49.0 * lambda_squared / 64.0;
    case Variable::h_zz:
        return 9.0 * lambda_squared / 64.0;
    case Variable::h_ww:
        return 25.0 * lambda_squared / 32.0;
    case Variable::A_xx:
        return 7.0 * lambda / 8.0;
    case Variable::A_zz:
        return 3.0 * lambda / 8.0;
    case Variable::A_ww:
        return -5.0 * lambda / 4.0;
    case Variable::K:
        return 9.0 * lambda / 8.0;
    case Variable::chi:
    case Variable::h_xz:
    case Variable::A_xz:
    case Variable::Theta:
    case Variable::hat_Gamma_x:
    case Variable::hat_Gamma_z:
        return 0.0;
    }
    return 0.0;
}

double expected_theta_output(const Vector &input, const double r0,
                             const double x)
{
    const double lambda = Operator::lambda_gp(r0, x);
    double output = 0.0;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        output += expected_coefficient(variable, lambda) * input.value(variable);
    }
    return output;
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

void check_pointwise_theta_algebraic_block()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = Operator::lambda_gp(r0, x);
    const double tolerance = 1.0e-14;

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto output =
            Operator::apply_theta_equation_algebraic_non_ricci_at_point(
                r0, x, make_basis_vector(variable, 1.0));
        require_close(std::string("Theta coefficient for ") +
                          Operator::variable_name(variable),
                      output.value(Variable::Theta),
                      expected_coefficient(variable, lambda), tolerance);
        for (const auto output_variable : Operator::frozen_gauge_state_vector)
        {
            if (output_variable != Variable::Theta)
            {
                require_close("non-Theta output is untouched",
                              output.value(output_variable), 0.0, 0.0);
            }
        }
    }

    const auto input = make_nonzero_vector();
    const auto output =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(r0, x,
                                                                    input);
    require_close("combined Theta algebraic linearization matches oracle",
                  output.value(Variable::Theta),
                  expected_theta_output(input, r0, x), tolerance);
    require_close("delta h_xz gives no Theta contribution",
                  Operator::apply_theta_equation_algebraic_non_ricci_at_point(
                      r0, x, make_basis_vector(Variable::h_xz, 1.0))
                      .value(Variable::Theta),
                  0.0, 0.0);
    require_close("delta A_xz gives no Theta contribution",
                  Operator::apply_theta_equation_algebraic_non_ricci_at_point(
                      r0, x, make_basis_vector(Variable::A_xz, 1.0))
                      .value(Variable::Theta),
                  0.0, 0.0);
}

void check_grid_theta_algebraic_block()
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
        Operator::apply_theta_equation_algebraic_non_ricci_block(grid, input);
    require_true("Theta algebraic result is partial only",
                 result.partial_operator_only());
    require_true("Theta algebraic block has no derivative boundary placeholders",
                 !result.boundary_values_are_placeholders());
    require_true("Theta algebraic block covers first grid point",
                 result.first_valid_index() == 0);
    require_true("Theta algebraic block covers last grid point",
                 result.last_valid_index() == grid.points() - 1);

    double max_error = 0.0;
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        max_error = std::max(
            max_error,
            std::abs(result.at(i).value(Variable::Theta) -
                     expected_theta_output(input[i], domain.r0(), grid.x(i))));
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            if (variable != Variable::Theta)
            {
                max_error = std::max(max_error,
                                     std::abs(result.at(i).value(variable)));
            }
        }
    }
    require_close("grid Theta algebraic block matches pointwise oracles",
                  max_error, 0.0, 1.0e-14);
}

void check_negative_guards()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = Operator::lambda_gp(r0, x);
    const double lambda_squared = lambda * lambda;

    const double hxx_to_theta =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(
            r0, x, make_basis_vector(Variable::h_xx, 1.0))
            .value(Variable::Theta);
    require_true("dropping inverse-metric variation would fail",
                 std::abs(hxx_to_theta) > 1.0e-3);

    const double hww_to_theta =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(
            r0, x, make_basis_vector(Variable::h_ww, 1.0))
            .value(Variable::Theta);
    const double wrong_no_hidden_hww = 25.0 * lambda_squared / 64.0;
    require_true("dropping hidden ww multiplicity in h_ww would fail",
                 std::abs(hww_to_theta - wrong_no_hidden_hww) > 1.0e-3);

    const double aww_to_theta =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(
            r0, x, make_basis_vector(Variable::A_ww, 1.0))
            .value(Variable::Theta);
    const double wrong_no_hidden_aww = -5.0 * lambda / 8.0;
    require_true("dropping hidden ww multiplicity in A_ww would fail",
                 std::abs(aww_to_theta - wrong_no_hidden_aww) > 1.0e-3);

    const double k_to_theta =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(
            r0, x, make_basis_vector(Variable::K, 1.0))
            .value(Variable::Theta);
    require_true("using d=3 K coefficient would fail",
                 std::abs(k_to_theta - lambda) > 1.0e-3);

    const double hxz_to_theta =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(
            r0, x, make_basis_vector(Variable::h_xz, 1.0))
            .value(Variable::Theta);
    require_true("spurious h_xz contribution would fail",
                 std::abs(hxz_to_theta - 0.25 * lambda_squared) > 1.0e-3);

    const double axz_to_theta =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(
            r0, x, make_basis_vector(Variable::A_xz, 1.0))
            .value(Variable::Theta);
    require_true("spurious A_xz contribution would fail",
                 std::abs(axz_to_theta - 0.25 * lambda) > 1.0e-3);

    const auto output =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(
            r0, x, make_nonzero_vector());
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::Theta)
        {
            require_close("non-Theta output is untouched",
                          output.value(variable), 0.0, 0.0);
        }
    }
}

void check_completion_guards()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true(
        "Theta algebraic non-Ricci block implemented",
        Operator::theta_equation_algebraic_non_ricci_block_implemented);
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
    check_pointwise_theta_algebraic_block();
    check_grid_theta_algebraic_block();
    check_negative_guards();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C Theta algebraic non-Ricci block\n";
    return 0;
}
