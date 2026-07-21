#include "Stage4AOFrozenGaugeOperator.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

constexpr double test_kappa1 = 0.1;
constexpr double test_kappa2 = 0.0;
constexpr double test_kappa3 = 1.0;
constexpr int test_spatial_dimension = 4;
constexpr double tolerance = 1.0e-14;
constexpr double meaningful_sign_separation = 1.0e-3;

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
                   const double expected, const double allowed_error = tolerance)
{
    const double error = std::abs(actual - expected);
    if (error > allowed_error)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " error=" + std::to_string(error));
    }
    std::cout << "PASS " << label << "\n";
}

double independent_theta_damping_coefficient(const double kappa1,
                                             const double kappa2,
                                             const int dimension)
{
    return -0.5 * kappa1 *
           (static_cast<double>(dimension + 1) +
            kappa2 * static_cast<double>(dimension - 1));
}

double independent_k_damping_coefficient(const double kappa1,
                                         const double kappa2,
                                         const int dimension)
{
    return -static_cast<double>(dimension) * kappa1 * (1.0 + kappa2);
}

Vector make_basis_vector(const Variable variable, const double amplitude)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    values[Operator::variable_index(variable)] = amplitude;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector make_all_nonzero_vector()
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        values[i] = 0.5 + 0.375 * static_cast<double>(i);
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

void require_only_k_theta_outputs(const std::string &label,
                                  const Vector &output)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::K && variable != Variable::Theta &&
            output.value(variable) != 0.0)
        {
            fail(label, std::string(Operator::variable_name(variable)) +
                            " was modified");
        }
    }
    std::cout << "PASS " << label << "\n";
}

void require_zero_vector(const std::string &label, const Vector &output)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (output.value(variable) != 0.0)
        {
            fail(label, std::string(Operator::variable_name(variable)) +
                            " was nonzero");
        }
    }
    std::cout << "PASS " << label << "\n";
}

void check_convention_lock()
{
    require_close("main kappa1 is inherited 0.1", Operator::main_ccz4_kappa1,
                  test_kappa1, 0.0);
    require_close("main kappa2 is inherited zero", Operator::main_ccz4_kappa2,
                  test_kappa2, 0.0);
    require_close("main kappa3 is inherited one", Operator::main_ccz4_kappa3,
                  test_kappa3, 0.0);
    require_true("main path uses covariant Z4",
                 Operator::main_ccz4_covariant_z4);
    require_true("main damping convention is locked",
                 Operator::main_ccz4_damping_convention_locked);
    require_true("zero damping is reserved for diagnostic comparison",
                 Operator::zero_damping_reserved_for_diagnostic_comparison);
    require_true("CCZ4 spatial dimension is four",
                 Operator::ccz4_spatial_dimension == test_spatial_dimension);
}

void check_pointwise_damping()
{
    const double delta_theta = 1.6;
    const double expected_theta_coefficient =
        independent_theta_damping_coefficient(
            test_kappa1, test_kappa2, test_spatial_dimension);
    const double expected_k_coefficient = independent_k_damping_coefficient(
        test_kappa1, test_kappa2, test_spatial_dimension);
    const auto output =
        Operator::apply_ccz4_k_theta_damping_insertion_at_point(
            make_basis_vector(Variable::Theta, delta_theta));

    require_close("Theta damping derives -0.25 deltaTheta",
                  output.value(Variable::Theta),
                  expected_theta_coefficient * delta_theta);
    require_close("K damping derives -0.4 deltaTheta",
                  output.value(Variable::K),
                  expected_k_coefficient * delta_theta);
    require_only_k_theta_outputs("damping touches only K and Theta", output);
    require_close("no premature hat_Gamma^x damping",
                  output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("no premature hat_Gamma^z damping",
                  output.value(Variable::hat_Gamma_z), 0.0, 0.0);

    const auto pure_k =
        Operator::apply_ccz4_k_theta_damping_insertion_at_point(
            make_basis_vector(Variable::K, 2.3));
    require_zero_vector("spurious deltaK damping contribution would fail",
                        pure_k);

    const auto pure_gamma =
        Operator::apply_ccz4_k_theta_damping_insertion_at_point(
            make_basis_vector(Variable::hat_Gamma_x, -0.8));
    require_close("hat_Gamma input does not enter simple damping block",
                  pure_gamma.value(Variable::K), 0.0, 0.0);
    require_close("hat_Gamma input does not produce Theta damping",
                  pure_gamma.value(Variable::Theta), 0.0, 0.0);

    const auto nonzero_output =
        Operator::apply_ccz4_k_theta_damping_insertion_at_point(
            make_all_nonzero_vector());
    require_only_k_theta_outputs(
        "nonzero-state damping leaves every non-K/Theta output untouched",
        nonzero_output);
}

void check_negative_guards()
{
    const auto unit_theta_output =
        Operator::apply_ccz4_k_theta_damping_insertion_at_point(
            make_basis_vector(Variable::Theta, 1.0));
    const double theta_coefficient = unit_theta_output.value(Variable::Theta);
    const double k_coefficient = unit_theta_output.value(Variable::K);
    const double expected_theta = independent_theta_damping_coefficient(
        test_kappa1, test_kappa2, test_spatial_dimension);
    const double expected_k = independent_k_damping_coefficient(
        test_kappa1, test_kappa2, test_spatial_dimension);
    const double wrong_sign_theta = -expected_theta;
    const double wrong_sign_k = -expected_k;

    require_close("Theta coefficient matches independent negative damping",
                  theta_coefficient, expected_theta);
    require_close("K coefficient matches independent negative damping",
                  k_coefficient, expected_k);
    require_true(
        "positive-sign Theta antidamping mutation would fail",
        std::abs(theta_coefficient - wrong_sign_theta) >
            meaningful_sign_separation);
    require_true("positive-sign K antidamping mutation would fail",
                 std::abs(k_coefficient - wrong_sign_k) >
                     meaningful_sign_separation);

    const double d3_theta = independent_theta_damping_coefficient(
        test_kappa1, test_kappa2, 3);
    const double d3_k = independent_k_damping_coefficient(
        test_kappa1, test_kappa2, 3);
    require_true("d=3 Theta coefficient would fail",
                 std::abs(theta_coefficient - d3_theta) > 1.0e-3);
    require_true("d=3 K coefficient would fail",
                 std::abs(k_coefficient - d3_k) > 1.0e-3);

    constexpr double diagnostic_kappa2 = 0.5;
    const double probed_theta = Operator::ccz4_theta_damping_coefficient(
        test_kappa1, diagnostic_kappa2, test_spatial_dimension);
    const double probed_k = Operator::ccz4_k_damping_coefficient(
        test_kappa1, diagnostic_kappa2, test_spatial_dimension);
    require_close("nonzero-kappa2 Theta dependence matches independent formula",
                  probed_theta,
                  independent_theta_damping_coefficient(
                      test_kappa1, diagnostic_kappa2,
                      test_spatial_dimension));
    require_close("nonzero-kappa2 K dependence matches independent formula",
                  probed_k,
                  independent_k_damping_coefficient(
                      test_kappa1, diagnostic_kappa2,
                      test_spatial_dimension));
    const double wrong_theta_kappa2 =
        -0.5 * test_kappa1 *
        (static_cast<double>(test_spatial_dimension + 1) -
         diagnostic_kappa2 *
             static_cast<double>(test_spatial_dimension - 1));
    const double wrong_k_kappa2 =
        -static_cast<double>(test_spatial_dimension) * test_kappa1 *
        (1.0 - diagnostic_kappa2);
    require_true("wrong Theta kappa2 dependence would fail",
                 std::abs(probed_theta - wrong_theta_kappa2) > 1.0e-3);
    require_true("wrong K kappa2 dependence would fail",
                 std::abs(probed_k - wrong_k_kappa2) > 1.0e-3);
}

void check_corrected_k_block_unchanged()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = Operator::lambda_gp(r0, x);

    const auto pure_k = Operator::apply_k_equation_ccz4_k_theta_at_point(
        r0, x, make_basis_vector(Variable::K, 0.8));
    require_close("corrected K block retains +3 lambda deltaK",
                  pure_k.value(Variable::K), 3.0 * lambda * 0.8);

    const auto pure_theta = Operator::apply_k_equation_ccz4_k_theta_at_point(
        r0, x, make_basis_vector(Variable::Theta, -0.65));
    require_close("corrected K block retains -3 lambda deltaTheta",
                  pure_theta.value(Variable::K),
                  -3.0 * lambda * -0.65);
    require_close("corrected K block does not absorb Theta damping",
                  pure_theta.value(Variable::Theta), 0.0, 0.0);
}

void check_grid_and_completion_guards()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 16);
    std::vector<Vector> input(grid.points(),
                              make_basis_vector(Variable::Theta, 0.7));
    const auto result =
        Operator::apply_ccz4_k_theta_damping_insertion_block(grid, input);
    require_true("damping grid result is partial only",
                 result.partial_operator_only());
    require_true("damping grid block has no boundary placeholders",
                 !result.boundary_values_are_placeholders());
    require_true("damping grid block covers all radial points",
                 result.first_valid_index() == 0 &&
                     result.last_valid_index() == grid.points() - 1);

    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("simple K/Theta damping insertion is implemented",
                 Operator::ccz4_k_theta_damping_insertion_block_implemented);
    require_true("hat_Gamma Z/kappa damping remains missing",
                 !Operator::hat_gamma_z4_kappa_damping_block_implemented);
    require_true("K RHS remains incomplete",
                 !Operator::variable_rhs_complete(Variable::K));
    require_true("Theta RHS remains incomplete",
                 !Operator::variable_rhs_complete(Variable::Theta));
    require_true("full operator remains incomplete",
                 !contract.complete_operator_implemented());
    require_true("eigensolver remains disallowed",
                 !contract.eigensolver_allowed());
}

} // namespace

int main()
{
    check_convention_lock();
    check_pointwise_damping();
    check_negative_guards();
    check_corrected_k_block_unchanged();
    check_grid_and_completion_guards();
    std::cout << "PASS Stage 4AO-C CCZ4 K/Theta damping insertion\n";
    return 0;
}
