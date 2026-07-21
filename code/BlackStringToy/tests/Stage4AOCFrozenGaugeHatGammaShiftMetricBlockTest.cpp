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
constexpr int hidden_multiplicity = 2;

struct Input
{
    double chi = 0.0;
    double h_xx = 0.0;
    double h_xz = 0.0;
    double h_zz = 0.0;
    double h_ww = 0.0;
    double k = 0.0;
    double a_xx = 0.0;
    double a_xz = 0.0;
    double a_zz = 0.0;
    double a_ww = 0.0;
    double theta = 0.0;
    double hat_gamma_x = 0.0;
    double hat_gamma_z = 0.0;
};

struct BackgroundShiftOracle
{
    double lambda;
    double dxx_beta_x;
    double dww_beta_x;
    double dx_div_beta;
    double grad_div_prefactor;
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
    values[Operator::variable_index(Variable::chi)] = input.chi;
    values[Operator::variable_index(Variable::h_xx)] = input.h_xx;
    values[Operator::variable_index(Variable::h_xz)] = input.h_xz;
    values[Operator::variable_index(Variable::h_zz)] = input.h_zz;
    values[Operator::variable_index(Variable::h_ww)] = input.h_ww;
    values[Operator::variable_index(Variable::K)] = input.k;
    values[Operator::variable_index(Variable::A_xx)] = input.a_xx;
    values[Operator::variable_index(Variable::A_xz)] = input.a_xz;
    values[Operator::variable_index(Variable::A_zz)] = input.a_zz;
    values[Operator::variable_index(Variable::A_ww)] = input.a_ww;
    values[Operator::variable_index(Variable::Theta)] = input.theta;
    values[Operator::variable_index(Variable::hat_Gamma_x)] =
        input.hat_gamma_x;
    values[Operator::variable_index(Variable::hat_Gamma_z)] =
        input.hat_gamma_z;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector apply_vector_hessian(const double r0, const double x,
                            const Input &input)
{
    return Operator::apply_hat_gamma_vector_hessian_insertion_at_point(
        r0, x, make_state(input));
}

Vector apply_grad_div(const double r0, const double x, const Input &input)
{
    return Operator::apply_hat_gamma_grad_div_insertion_at_point(
        r0, x, make_state(input));
}

BackgroundShiftOracle independent_background_shift(const double r0,
                                                   const double x)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double dxx_beta_x = 3.0 * lambda / (4.0 * x);
    const double dww_beta_x = -3.0 * lambda / (2.0 * x);
    const double k0 = 3.0 * lambda / 2.0;
    const double dx_div_beta = -3.0 * k0 / (2.0 * x);
    const double grad_div_prefactor =
        static_cast<double>(spatial_dimension - 2) /
        static_cast<double>(spatial_dimension);
    return {lambda, dxx_beta_x, dww_beta_x, dx_div_beta,
            grad_div_prefactor};
}

std::array<double, 2> independent_vector_hessian(
    const double r0, const double x, const Input &input,
    const int hidden_copies = hidden_multiplicity)
{
    const auto background = independent_background_shift(r0, x);
    const double delta_h_uu_xx = -input.h_xx;
    const double delta_h_uu_ww = -input.h_ww;
    return {delta_h_uu_xx * background.dxx_beta_x +
                static_cast<double>(hidden_copies) * delta_h_uu_ww *
                    background.dww_beta_x,
            0.0};
}

std::array<double, 2> independent_grad_div(const double r0, const double x,
                                           const Input &input)
{
    const auto background = independent_background_shift(r0, x);
    return {background.grad_div_prefactor * (-input.h_xx) *
                background.dx_div_beta,
            background.grad_div_prefactor * (-input.h_xz) *
                background.dx_div_beta};
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

void check_separate_pure_component_oracles()
{
    const double r0 = 1.15;
    const double x = 2.1;

    Input pure_hxx;
    pure_hxx.h_xx = 0.72;
    const auto vector_hxx = apply_vector_hessian(r0, x, pure_hxx);
    const auto expected_vector_hxx =
        independent_vector_hessian(r0, x, pure_hxx);
    require_close("pure h_xx vector-Hessian x oracle",
                  vector_hxx.value(Variable::hat_Gamma_x),
                  expected_vector_hxx[0], expected_vector_hxx[0]);
    require_close("pure h_xx vector-Hessian z vanishes",
                  vector_hxx.value(Variable::hat_Gamma_z), 0.0, 0.0);

    const auto grad_hxx = apply_grad_div(r0, x, pure_hxx);
    const auto expected_grad_hxx = independent_grad_div(r0, x, pure_hxx);
    require_close("pure h_xx grad-div x oracle",
                  grad_hxx.value(Variable::hat_Gamma_x),
                  expected_grad_hxx[0], expected_grad_hxx[0]);
    require_close("pure h_xx grad-div z vanishes",
                  grad_hxx.value(Variable::hat_Gamma_z), 0.0, 0.0);

    Input pure_hww;
    pure_hww.h_ww = -0.64;
    const auto vector_hww = apply_vector_hessian(r0, x, pure_hww);
    const auto expected_vector_hww =
        independent_vector_hessian(r0, x, pure_hww);
    require_close("pure h_ww vector-Hessian x oracle",
                  vector_hww.value(Variable::hat_Gamma_x),
                  expected_vector_hww[0], expected_vector_hww[0]);
    require_close("pure h_ww vector-Hessian z vanishes",
                  vector_hww.value(Variable::hat_Gamma_z), 0.0, 0.0);
    const auto grad_hww = apply_grad_div(r0, x, pure_hww);
    require_close("pure h_ww gives no grad-div x term",
                  grad_hww.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("pure h_ww gives no grad-div z term",
                  grad_hww.value(Variable::hat_Gamma_z), 0.0, 0.0);

    Input pure_hxz;
    pure_hxz.h_xz = 0.57;
    const auto vector_hxz = apply_vector_hessian(r0, x, pure_hxz);
    require_close("pure h_xz gives no vector-Hessian x term",
                  vector_hxz.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("beta_GP^z=0 gives no vector-Hessian z term",
                  vector_hxz.value(Variable::hat_Gamma_z), 0.0, 0.0);
    const auto grad_hxz = apply_grad_div(r0, x, pure_hxz);
    const auto expected_grad_hxz = independent_grad_div(r0, x, pure_hxz);
    require_close("pure h_xz grad-div x vanishes",
                  grad_hxz.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("pure h_xz grad-div z oracle",
                  grad_hxz.value(Variable::hat_Gamma_z),
                  expected_grad_hxz[1], expected_grad_hxz[1]);

    Input pure_hzz;
    pure_hzz.h_zz = 1.3;
    const auto vector_hzz = apply_vector_hessian(r0, x, pure_hzz);
    const auto grad_hzz = apply_grad_div(r0, x, pure_hzz);
    require_close("h_zz gives no vector-Hessian x term",
                  vector_hzz.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("h_zz gives no vector-Hessian z term",
                  vector_hzz.value(Variable::hat_Gamma_z), 0.0, 0.0);
    require_close("h_zz gives no grad-div x term",
                  grad_hzz.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("h_zz gives no grad-div z term",
                  grad_hzz.value(Variable::hat_Gamma_z), 0.0, 0.0);
}

void check_mutations_and_separate_ownership()
{
    const double r0 = 0.95;
    const double x = 1.7;
    const auto background = independent_background_shift(r0, x);

    Input hww;
    hww.h_ww = 0.83;
    const auto vector_hww = apply_vector_hessian(r0, x, hww);
    const auto one_hidden = independent_vector_hessian(r0, x, hww, 1);
    require_separated("hidden multiplicity one fails",
                      vector_hww.value(Variable::hat_Gamma_x),
                      one_hidden[0]);
    require_separated("omitted h_ww vector-Hessian term fails",
                      vector_hww.value(Variable::hat_Gamma_x), 0.0);
    require_separated("wrong-sign h_ww vector-Hessian term fails",
                      vector_hww.value(Variable::hat_Gamma_x),
                      -independent_vector_hessian(r0, x, hww)[0]);

    Input hxz;
    hxz.h_xz = -0.74;
    const auto vector_hxz = apply_vector_hessian(r0, x, hxz);
    require_separated("spurious z vector-Hessian term is rejected",
                      vector_hxz.value(Variable::hat_Gamma_z),
                      -hxz.h_xz * background.dxx_beta_x);

    Input hxx;
    hxx.h_xx = 0.69;
    const auto grad_hxx = apply_grad_div(r0, x, hxx);
    const double wrong_grad_coefficient =
        background.lambda / x * hxx.h_xx;
    require_separated("wrong grad-div coefficient fails",
                      grad_hxx.value(Variable::hat_Gamma_x),
                      wrong_grad_coefficient);
    require_separated("wrong grad-div sign fails",
                      grad_hxx.value(Variable::hat_Gamma_x),
                      -independent_grad_div(r0, x, hxx)[0]);

    const auto vector_hxx = apply_vector_hessian(r0, x, hxx);
    const double duplicated_connection_a =
        independent_vector_hessian(r0, x, hxx)[0] +
        5.0 * background.lambda / (2.0 * x) * hxx.h_xx;
    require_separated("vector Hessian does not duplicate connection-A",
                      vector_hxx.value(Variable::hat_Gamma_x),
                      duplicated_connection_a);
    const double duplicated_gradient =
        independent_grad_div(r0, x, hxx)[0] -
        27.0 * background.lambda / (8.0 * x) * hxx.h_xx;
    require_separated("grad-div does not duplicate Gamma gradient block",
                      grad_hxx.value(Variable::hat_Gamma_x),
                      duplicated_gradient);
    require_separated("grad-div remains separate from vector Hessian",
                      grad_hxx.value(Variable::hat_Gamma_x),
                      independent_grad_div(r0, x, hxx)[0] +
                          independent_vector_hessian(r0, x, hxx)[0]);
}

void check_previous_blocks_and_output_scope()
{
    const double r0 = 1.0;
    const double x = 2.0;
    Input previous_only;
    previous_only.chi = 0.9;
    previous_only.k = -0.8;
    previous_only.theta = 0.7;
    previous_only.a_xx = -0.6;
    previous_only.a_xz = 0.5;
    previous_only.a_zz = -0.4;
    previous_only.a_ww = 0.3;
    previous_only.hat_gamma_x = -0.2;
    previous_only.hat_gamma_z = 0.1;
    const auto vector_output =
        apply_vector_hessian(r0, x, previous_only);
    const auto grad_output = apply_grad_div(r0, x, previous_only);
    require_close("vector Hessian does not duplicate prior Gamma x blocks",
                  vector_output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("vector Hessian does not duplicate prior Gamma z blocks",
                  vector_output.value(Variable::hat_Gamma_z), 0.0, 0.0);
    require_close("grad-div does not duplicate prior Gamma x blocks",
                  grad_output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("grad-div does not duplicate prior Gamma z blocks",
                  grad_output.value(Variable::hat_Gamma_z), 0.0, 0.0);
    require_only_gamma_outputs("vector-Hessian block", vector_output);
    require_only_gamma_outputs("grad-div block", grad_output);
}

Input parity_input(const double z)
{
    const double wave_number = 1.35;
    Input input;
    input.h_xx = 0.5 * std::cos(wave_number * z);
    input.h_ww = -0.3 * std::cos(wave_number * z);
    input.h_xz = 0.4 * std::sin(wave_number * z);
    input.h_zz = -0.2 * std::cos(wave_number * z);
    return input;
}

void check_parity_closure_and_completion_gates()
{
    const double r0 = 1.0;
    const double x = 1.8;
    const auto vector_positive =
        apply_vector_hessian(r0, x, parity_input(0.39));
    const auto vector_negative =
        apply_vector_hessian(r0, x, parity_input(-0.39));
    require_close("raw vector-Hessian x output stays even",
                  vector_positive.value(Variable::hat_Gamma_x),
                  vector_negative.value(Variable::hat_Gamma_x),
                  vector_positive.value(Variable::hat_Gamma_x));
    require_close("raw vector-Hessian z output stays zero",
                  vector_positive.value(Variable::hat_Gamma_z), 0.0, 0.0);

    const auto grad_positive = apply_grad_div(r0, x, parity_input(0.39));
    const auto grad_negative = apply_grad_div(r0, x, parity_input(-0.39));
    require_close("raw grad-div x output stays even",
                  grad_positive.value(Variable::hat_Gamma_x),
                  grad_negative.value(Variable::hat_Gamma_x),
                  grad_positive.value(Variable::hat_Gamma_x));
    require_close("raw grad-div z output stays one-z odd",
                  grad_positive.value(Variable::hat_Gamma_z),
                  -grad_negative.value(Variable::hat_Gamma_z),
                  grad_positive.value(Variable::hat_Gamma_z));
    require_only_gamma_outputs("vector-Hessian parity block", vector_positive);
    require_only_gamma_outputs("grad-div parity block", grad_positive);

    const auto contract =
        Operator::make_default_frozen_gauge_operator_contract();
    require_true("vector-Hessian family is implemented",
                 Operator::hat_gamma_vector_hessian_block_implemented);
    require_true("grad-div family is implemented",
                 Operator::hat_gamma_grad_div_block_implemented);
    require_true("surviving mathematical Gamma family inventory is closed",
                 Operator::hat_gamma_surviving_term_family_inventory_closed);
    require_true("final Gamma row assembly is now implemented",
                 Operator::hat_gamma_final_row_assembly_implemented);
    require_true("assembled Gamma row validation is now implemented",
                 Operator::hat_gamma_assembled_row_validation_implemented);
    require_true("complete Gamma RHS is now true",
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
    check_separate_pure_component_oracles();
    check_mutations_and_separate_ownership();
    check_previous_blocks_and_output_scope();
    check_parity_closure_and_completion_gates();
    std::cout << "PASS Stage 4AO-C hatted-Gamma shift metric-variation "
                 "blocks\n";
    return 0;
}
