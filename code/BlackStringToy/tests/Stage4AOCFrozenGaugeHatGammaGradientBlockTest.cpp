#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

constexpr double tolerance = 2.0e-13;
constexpr int spatial_dimension = 4;

struct Input
{
    double h_xx = 0.0;
    double h_xz = 0.0;
    double h_zz = 0.0;
    double h_ww = 0.0;
    double hat_gamma_x = 0.0;
    double hat_gamma_z = 0.0;
    double dx_k = 0.0;
    double dz_k = 0.0;
    double dx_theta = 0.0;
    double dz_theta = 0.0;
    double dx_chi = 0.0;
    double dz_chi = 0.0;
};

struct Coefficients
{
    double theta;
    double k;
    double metric;
    double chi_x;
    double chi_z;
};

[[noreturn]] void fail(const std::string &label, const std::string &details)
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
                   const double expected, const double scale = 1.0)
{
    const double allowed = tolerance * std::max(1.0, std::abs(scale));
    if (std::abs(actual - expected) > allowed)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected));
    }
    std::cout << "PASS " << label << "\n";
}

void require_separated(const std::string &label, const double actual,
                       const double mutation)
{
    if (std::abs(actual - mutation) < 1.0e-5)
    {
        fail(label, "mutation was not separated from the implementation");
    }
    std::cout << "PASS " << label << "\n";
}

Vector make_state(const Input &input)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    values[Operator::variable_index(Variable::h_xx)] = input.h_xx;
    values[Operator::variable_index(Variable::h_xz)] = input.h_xz;
    values[Operator::variable_index(Variable::h_zz)] = input.h_zz;
    values[Operator::variable_index(Variable::h_ww)] = input.h_ww;
    values[Operator::variable_index(Variable::hat_Gamma_x)] =
        input.hat_gamma_x;
    values[Operator::variable_index(Variable::hat_Gamma_z)] =
        input.hat_gamma_z;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector apply(const double r0, const double x, const Input &input)
{
    return Operator::apply_hat_gamma_k_theta_chi_gradient_insertion_at_point(
        r0, x, make_state(input), input.dx_k, input.dz_k, input.dx_theta,
        input.dz_theta, input.dx_chi, input.dz_chi);
}

Coefficients independent_coefficients(const double r0, const double x)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double theta = 2.0;
    const double k = -2.0 * static_cast<double>(spatial_dimension - 1) /
                     static_cast<double>(spatial_dimension);
    const double dx_k0 = -9.0 * lambda / (4.0 * x);

    // From k*h^{ij} partial_j(delta K) plus its inverse-metric variation:
    // delta h^{ix}=-delta h_ix on the flat conformal GP background.
    const double metric = k * (-1.0) * dx_k0;
    const double a0_xx = -7.0 * lambda / 8.0;
    const double a0_zz = -3.0 * lambda / 8.0;
    const double chi_x = -static_cast<double>(spatial_dimension) * a0_xx;
    const double chi_z = -static_cast<double>(spatial_dimension) * a0_zz;
    return {theta, k, metric, chi_x, chi_z};
}

void require_only_gamma_outputs(const std::string &label,
                                const Vector &output)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable == Variable::hat_Gamma_x ||
            variable == Variable::hat_Gamma_z)
        {
            continue;
        }
        require_close(label + " leaves " + Operator::variable_name(variable) +
                          " untouched",
                      output.value(variable), 0.0, 0.0);
    }
}

void check_pure_scalar_gradients()
{
    const double r0 = 1.2;
    const double x = 2.3;
    const auto coefficients = independent_coefficients(r0, x);

    Input pure_k;
    pure_k.dx_k = 0.7;
    pure_k.dz_k = -0.4;
    const auto k_output = apply(r0, x, pure_k);
    require_close("pure delta K x gradient", k_output.value(Variable::hat_Gamma_x),
                  coefficients.k * pure_k.dx_k, coefficients.k);
    require_close("pure delta K z gradient", k_output.value(Variable::hat_Gamma_z),
                  coefficients.k * pure_k.dz_k, coefficients.k);
    require_separated("wrong K-gradient sign fails in x",
                      k_output.value(Variable::hat_Gamma_x),
                      -coefficients.k * pure_k.dx_k);

    Input pure_theta;
    pure_theta.dx_theta = -0.6;
    pure_theta.dz_theta = 0.9;
    const auto theta_output = apply(r0, x, pure_theta);
    require_close("pure delta Theta x gradient",
                  theta_output.value(Variable::hat_Gamma_x),
                  coefficients.theta * pure_theta.dx_theta,
                  coefficients.theta);
    require_close("pure delta Theta z gradient",
                  theta_output.value(Variable::hat_Gamma_z),
                  coefficients.theta * pure_theta.dz_theta,
                  coefficients.theta);
    require_separated("wrong Theta-gradient sign fails in z",
                      theta_output.value(Variable::hat_Gamma_z),
                      -coefficients.theta * pure_theta.dz_theta);

    Input pure_chi;
    pure_chi.dx_chi = 0.8;
    pure_chi.dz_chi = 0.8;
    const auto chi_output = apply(r0, x, pure_chi);
    require_close("pure delta chi x gradient has 7 lambda/2",
                  chi_output.value(Variable::hat_Gamma_x),
                  coefficients.chi_x * pure_chi.dx_chi,
                  coefficients.chi_x);
    require_close("pure delta chi z gradient has 3 lambda/2",
                  chi_output.value(Variable::hat_Gamma_z),
                  coefficients.chi_z * pure_chi.dz_chi,
                  coefficients.chi_z);
    require_separated("x chi coefficient is not the z coefficient",
                      chi_output.value(Variable::hat_Gamma_x),
                      coefficients.chi_z * pure_chi.dx_chi);
    require_separated("z chi coefficient is not the x coefficient",
                      chi_output.value(Variable::hat_Gamma_z),
                      coefficients.chi_x * pure_chi.dz_chi);
    require_separated("wrong chi-gradient sign fails",
                      chi_output.value(Variable::hat_Gamma_x),
                      -coefficients.chi_x * pure_chi.dx_chi);
    require_only_gamma_outputs("scalar-gradient block", chi_output);
}

void check_background_k_gradient_metric_variation()
{
    const double r0 = 0.9;
    const double x = 1.7;
    const auto coefficients = independent_coefficients(r0, x);

    Input pure_hxx;
    pure_hxx.h_xx = 0.65;
    const auto x_output = apply(r0, x, pure_hxx);
    require_close("pure delta h_xx background-K-gradient variation",
                  x_output.value(Variable::hat_Gamma_x),
                  coefficients.metric * pure_hxx.h_xx,
                  coefficients.metric);
    require_close("pure delta h_xx does not enter z row",
                  x_output.value(Variable::hat_Gamma_z), 0.0, 0.0);
    require_separated("omitting background-K-gradient h_xx term fails",
                      x_output.value(Variable::hat_Gamma_x), 0.0);
    require_separated("wrong h_xx metric-variation sign fails",
                      x_output.value(Variable::hat_Gamma_x),
                      -coefficients.metric * pure_hxx.h_xx);

    Input pure_hxz;
    pure_hxz.h_xz = -0.55;
    const auto z_output = apply(r0, x, pure_hxz);
    require_close("pure delta h_xz background-K-gradient variation",
                  z_output.value(Variable::hat_Gamma_z),
                  coefficients.metric * pure_hxz.h_xz,
                  coefficients.metric);
    require_close("pure delta h_xz does not enter x row",
                  z_output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_separated("omitting background-K-gradient h_xz term fails",
                      z_output.value(Variable::hat_Gamma_z), 0.0);

    Input excluded_diagonal;
    excluded_diagonal.h_zz = 1.2;
    excluded_diagonal.h_ww = -0.8;
    const auto excluded_output = apply(r0, x, excluded_diagonal);
    require_close("delta h_zz gives no x-row contribution",
                  excluded_output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("delta h_ww gives no z-row contribution",
                  excluded_output.value(Variable::hat_Gamma_z), 0.0, 0.0);
}

Input parity_input(const double z)
{
    const double wave_number = 1.4;
    const double cosine = std::cos(wave_number * z);
    const double sine = std::sin(wave_number * z);
    Input input;
    input.h_xx = 0.3 * cosine;
    input.h_xz = -0.2 * sine;
    input.dx_k = 0.4 * cosine;
    input.dz_k = -0.5 * sine;
    input.dx_theta = -0.6 * cosine;
    input.dz_theta = 0.7 * sine;
    input.dx_chi = 0.8 * cosine;
    input.dz_chi = -0.9 * sine;
    return input;
}

void check_parity_scope_and_no_duplication()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const auto positive = apply(r0, x, parity_input(0.37));
    const auto negative = apply(r0, x, parity_input(-0.37));
    require_close("hat_Gamma^x gradient output stays even",
                  positive.value(Variable::hat_Gamma_x),
                  negative.value(Variable::hat_Gamma_x),
                  positive.value(Variable::hat_Gamma_x));
    require_close("hat_Gamma^z gradient output stays one-z odd",
                  positive.value(Variable::hat_Gamma_z),
                  -negative.value(Variable::hat_Gamma_z),
                  positive.value(Variable::hat_Gamma_z));
    require_only_gamma_outputs("parity gradient block", positive);

    Input earlier_block_only;
    earlier_block_only.hat_gamma_x = 0.9;
    earlier_block_only.hat_gamma_z = -0.7;
    const auto isolated = apply(r0, x, earlier_block_only);
    require_close("does not duplicate earlier Gamma x block",
                  isolated.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("does not duplicate earlier Gamma z block",
                  isolated.value(Variable::hat_Gamma_z), 0.0, 0.0);
    const double beta_gp = std::sqrt(r0 / x);
    require_separated("does not duplicate hypothetical x advection",
                      isolated.value(Variable::hat_Gamma_x), beta_gp * 0.8);
    require_separated("does not duplicate hypothetical z advection",
                      isolated.value(Variable::hat_Gamma_z), beta_gp * -0.6);

    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("Gamma gradient partial block is implemented",
                 Operator::hat_gamma_k_theta_chi_gradient_block_implemented);
    require_true("earlier Gamma partial block remains implemented",
                 Operator::
                     hat_gamma_z4_kappa_shift_gradient_block_implemented);
    require_true("assembled Gamma RHS is now complete",
                 Operator::hat_gamma_rhs_block_implemented);
    require_true("hat_Gamma^x RHS is now complete",
                 Operator::variable_rhs_complete(Variable::hat_Gamma_x));
    require_true("hat_Gamma^z RHS is now complete",
                 Operator::variable_rhs_complete(Variable::hat_Gamma_z));
    require_true("full operator remains incomplete",
                 !contract.complete_operator_implemented());
    require_true("eigensolver remains disallowed",
                 !contract.eigensolver_allowed());
}

} // namespace

int main()
{
    check_pure_scalar_gradients();
    check_background_k_gradient_metric_variation();
    check_parity_scope_and_no_duplication();
    std::cout << "PASS Stage 4AO-C hatted-Gamma K/Theta/chi gradient block\n";
    return 0;
}
