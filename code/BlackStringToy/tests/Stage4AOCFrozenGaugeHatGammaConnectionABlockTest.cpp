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
constexpr int hidden_multiplicity = 2;

struct Input
{
    double h_xx = 0.0;
    double h_xz = 0.0;
    double h_zz = 0.0;
    double h_ww = 0.0;
    double a_xx = 0.0;
    double a_xz = 0.0;
    double a_zz = 0.0;
    double a_ww = 0.0;
    double hat_gamma_x = 0.0;
    double hat_gamma_z = 0.0;
    double dx_h_xx = 0.0;
    double dx_h_xz = 0.0;
    double dx_h_zz = 0.0;
    double dx_h_ww = 0.0;
    double dz_h_xx = 0.0;
    double dz_h_xz = 0.0;
    double dz_h_zz = 0.0;
    double dz_h_ww = 0.0;
    double dx_a_xx = 0.0;
    double dx_a_xz = 0.0;
    double dx_a_zz = 0.0;
    double dx_a_ww = 0.0;
    double dz_a_xx = 0.0;
    double dz_a_xz = 0.0;
    double dz_a_zz = 0.0;
    double dz_a_ww = 0.0;
    double dx_k = 0.0;
    double dz_theta = 0.0;
    double dx_chi = 0.0;
    double dx_hat_gamma_x = 0.0;
    double dx_hat_gamma_z = 0.0;
};

struct ConnectionOracle
{
    double gamma_x_xx;
    double gamma_x_zz;
    double gamma_x_ww;
    double gamma_z_xx;
    double gamma_z_zz;
    double gamma_z_ww;
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
    values[Operator::variable_index(Variable::A_xx)] = input.a_xx;
    values[Operator::variable_index(Variable::A_xz)] = input.a_xz;
    values[Operator::variable_index(Variable::A_zz)] = input.a_zz;
    values[Operator::variable_index(Variable::A_ww)] = input.a_ww;
    values[Operator::variable_index(Variable::hat_Gamma_x)] =
        input.hat_gamma_x;
    values[Operator::variable_index(Variable::hat_Gamma_z)] =
        input.hat_gamma_z;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector apply(const double r0, const double x, const Input &input)
{
    return Operator::apply_hat_gamma_connection_a_insertion_at_point(
        r0, x, make_state(input), input.dx_h_xx, input.dx_h_xz,
        input.dx_h_zz, input.dx_h_ww, input.dz_h_xx, input.dz_h_xz,
        input.dz_h_zz, input.dz_h_ww);
}

ConnectionOracle linearized_connection(const double x, const Input &input)
{
    return {
        0.5 * input.dx_h_xx,
        input.dz_h_xz - 0.5 * input.dx_h_zz,
        (input.h_xx - input.h_ww) / x - 0.5 * input.dx_h_ww,
        input.dx_h_xz - 0.5 * input.dz_h_xx,
        0.5 * input.dz_h_zz,
        input.h_xz / x - 0.5 * input.dz_h_ww};
}

std::array<double, 2> independent_oracle(const double r0, const double x,
                                         const Input &input,
                                         const int hidden_copies =
                                             hidden_multiplicity)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double a0_xx = -7.0 * lambda / 8.0;
    const double a0_zz = -3.0 * lambda / 8.0;
    const double a0_ww = 5.0 * lambda / 8.0;
    const auto connection = linearized_connection(x, input);
    return {
        2.0 * (a0_xx * connection.gamma_x_xx +
               a0_zz * connection.gamma_x_zz +
               static_cast<double>(hidden_copies) * a0_ww *
                   connection.gamma_x_ww),
        2.0 * (a0_xx * connection.gamma_z_xx +
               a0_zz * connection.gamma_z_zz +
               static_cast<double>(hidden_copies) * a0_ww *
                   connection.gamma_z_ww)};
}

void require_matches_oracle(const std::string &label, const double r0,
                            const double x, const Input &input)
{
    const auto actual = apply(r0, x, input);
    const auto expected = independent_oracle(r0, x, input);
    require_close(label + " x", actual.value(Variable::hat_Gamma_x),
                  expected[0], expected[0]);
    require_close(label + " z", actual.value(Variable::hat_Gamma_z),
                  expected[1], expected[1]);
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

void check_visible_connection_terms()
{
    const double r0 = 1.1;
    const double x = 2.2;

    Input xx;
    xx.dx_h_xx = 0.8;
    xx.dz_h_xx = -0.7;
    require_matches_oracle("visible xx connection", r0, x, xx);

    Input xz;
    xz.dx_h_xz = -0.6;
    xz.dz_h_xz = 0.9;
    require_matches_oracle("visible xz connection", r0, x, xz);

    Input zz;
    zz.dx_h_zz = 0.5;
    zz.dz_h_zz = -0.4;
    require_matches_oracle("visible zz connection", r0, x, zz);

    const auto xz_output = apply(r0, x, xz);
    const auto xz_expected = independent_oracle(r0, x, xz);
    require_separated("wrong dz(h_xz) sign fails in x row",
                      xz_output.value(Variable::hat_Gamma_x),
                      -xz_expected[0]);
    require_separated("wrong dx(h_xz) sign fails in z row",
                      xz_output.value(Variable::hat_Gamma_z),
                      -xz_expected[1]);
}

void check_hidden_and_radial_terms()
{
    const double r0 = 0.85;
    const double x = 1.6;

    Input pure_hww;
    pure_hww.h_ww = 0.7;
    pure_hww.dx_h_ww = -0.3;
    pure_hww.dz_h_ww = 0.45;
    require_matches_oracle("pure hidden h_ww", r0, x, pure_hww);
    const auto hww_output = apply(r0, x, pure_hww);
    const auto one_hidden_hww = independent_oracle(r0, x, pure_hww, 1);
    require_separated("one hidden h_ww copy fails in x row",
                      hww_output.value(Variable::hat_Gamma_x),
                      one_hidden_hww[0]);
    require_separated("one hidden h_ww copy fails in z row",
                      hww_output.value(Variable::hat_Gamma_z),
                      one_hidden_hww[1]);

    Input pure_hxz;
    pure_hxz.h_xz = -0.65;
    require_matches_oracle("pure h_xz radial connection", r0, x, pure_hxz);
    const auto hxz_output = apply(r0, x, pure_hxz);
    const double lambda = std::sqrt(r0 / (x * x * x));
    require_separated("wrong h_xz over x sign fails",
                      hxz_output.value(Variable::hat_Gamma_z),
                      -5.0 * lambda / (2.0 * x) * pure_hxz.h_xz);
    require_separated("omitted h_xz over x term fails",
                      hxz_output.value(Variable::hat_Gamma_z), 0.0);

    Input diagonal;
    diagonal.h_xx = 0.4;
    diagonal.h_ww = -0.2;
    diagonal.dx_h_xx = 0.35;
    diagonal.dx_h_zz = -0.5;
    diagonal.dx_h_ww = 0.25;
    diagonal.dz_h_xx = -0.45;
    diagonal.dz_h_zz = 0.55;
    diagonal.dz_h_ww = -0.15;
    require_matches_oracle("combined diagonal connection", r0, x, diagonal);
    const auto diagonal_output = apply(r0, x, diagonal);
    const auto diagonal_connection = linearized_connection(x, diagonal);
    const double wrong_radial_x =
        diagonal_output.value(Variable::hat_Gamma_x) -
        4.0 * (5.0 * lambda / 8.0) * diagonal_connection.gamma_x_ww +
        4.0 * (5.0 * lambda / 8.0) *
            (-(diagonal.h_xx - diagonal.h_ww) / x -
             0.5 * diagonal.dx_h_ww);
    require_separated("wrong diagonal 1/x sign fails",
                      diagonal_output.value(Variable::hat_Gamma_x),
                      wrong_radial_x);
}

void check_no_a_or_previous_block_dependence()
{
    const double r0 = 1.0;
    const double x = 2.0;
    Input excluded;
    excluded.a_xx = 0.8;
    excluded.a_xz = -0.7;
    excluded.a_zz = 0.6;
    excluded.a_ww = -0.5;
    excluded.dx_a_xx = 1.1;
    excluded.dx_a_xz = -1.2;
    excluded.dx_a_zz = 1.3;
    excluded.dx_a_ww = -1.4;
    excluded.dz_a_xx = -1.5;
    excluded.dz_a_xz = 1.6;
    excluded.dz_a_zz = -1.7;
    excluded.dz_a_ww = 1.8;
    excluded.hat_gamma_x = 0.9;
    excluded.hat_gamma_z = -0.85;
    excluded.dx_k = 0.75;
    excluded.dz_theta = -0.65;
    excluded.dx_chi = 0.55;
    excluded.dx_hat_gamma_x = -0.45;
    excluded.dx_hat_gamma_z = 0.35;
    const auto output = apply(r0, x, excluded);
    require_close("pure delta A gives zero direct x output",
                  output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("pure delta A gives zero direct z output",
                  output.value(Variable::hat_Gamma_z), 0.0, 0.0);
    require_close("d1.A does not enter x output",
                  output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("d1.A does not enter z output",
                  output.value(Variable::hat_Gamma_z), 0.0, 0.0);
    require_close("does not duplicate earlier Gamma blocks in x",
                  output.value(Variable::hat_Gamma_x), 0.0, 0.0);
    require_close("does not duplicate earlier Gamma blocks in z",
                  output.value(Variable::hat_Gamma_z), 0.0, 0.0);
    require_separated("does not duplicate hypothetical x advection",
                      output.value(Variable::hat_Gamma_x),
                      std::sqrt(r0 / x) * excluded.dx_hat_gamma_x);
    require_separated("does not duplicate hypothetical z advection",
                      output.value(Variable::hat_Gamma_z),
                      std::sqrt(r0 / x) * excluded.dx_hat_gamma_z);
    require_only_gamma_outputs("connection-A block", output);
}

Input parity_input(const double z)
{
    const double wave_number = 1.3;
    const double cosine = std::cos(wave_number * z);
    const double sine = std::sin(wave_number * z);
    Input input;
    input.h_xx = 0.4 * cosine;
    input.h_xz = -0.3 * sine;
    input.h_ww = 0.2 * cosine;
    input.dx_h_xx = -0.5 * cosine;
    input.dx_h_xz = 0.6 * sine;
    input.dx_h_zz = 0.7 * cosine;
    input.dx_h_ww = -0.8 * cosine;
    input.dz_h_xx = -0.4 * wave_number * sine;
    input.dz_h_xz = -0.3 * wave_number * cosine;
    input.dz_h_zz = 0.9 * wave_number * sine;
    input.dz_h_ww = -0.2 * wave_number * sine;
    return input;
}

void check_parity_and_completion_guards()
{
    const double r0 = 1.0;
    const double x = 1.9;
    const auto positive = apply(r0, x, parity_input(0.41));
    const auto negative = apply(r0, x, parity_input(-0.41));
    require_close("hat_Gamma^x connection-A output stays even",
                  positive.value(Variable::hat_Gamma_x),
                  negative.value(Variable::hat_Gamma_x),
                  positive.value(Variable::hat_Gamma_x));
    require_close("hat_Gamma^z connection-A output stays one-z odd",
                  positive.value(Variable::hat_Gamma_z),
                  -negative.value(Variable::hat_Gamma_z),
                  positive.value(Variable::hat_Gamma_z));
    require_only_gamma_outputs("parity connection-A block", positive);

    const auto contract =
        Operator::make_default_frozen_gauge_operator_contract();
    require_true("connection-A partial block is implemented",
                 Operator::hat_gamma_connection_a_block_implemented);
    require_true("Z/kappa Gamma block remains implemented",
                 Operator::hat_gamma_z4_kappa_shift_gradient_block_implemented);
    require_true("Gamma gradient block remains implemented",
                 Operator::hat_gamma_k_theta_chi_gradient_block_implemented);
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
    check_visible_connection_terms();
    check_hidden_and_radial_terms();
    check_no_a_or_previous_block_dependence();
    check_parity_and_completion_guards();
    std::cout << "PASS Stage 4AO-C hatted-Gamma connection-A block\n";
    return 0;
}
