#include "CartoonHatGammaX.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace
{
using namespace BlackStringToy;

using Inputs = CartoonHatGammaX::HatGammaXInputs;
using Result = CartoonHatGammaX::HatGammaX;

constexpr double tol = 1.0e-12;

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected)
{
    const double residual = std::abs(value - expected);
    if (residual > tol)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected) + ", residual " +
                        std::to_string(residual));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void require_not_close(const std::string &label, const double value,
                       const double forbidden)
{
    const double residual = std::abs(value - forbidden);
    if (residual <= tol)
    {
        fail(label, "unexpectedly matched " + std::to_string(forbidden));
    }
    std::cout << "PASS " << label << " avoided " << forbidden << "\n";
}

template <class Function> void require_domain_error(const std::string &label,
                                                    Function &&function)
{
    try
    {
        function();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    catch (const std::exception &exception)
    {
        fail(label, std::string("wrong exception: ") + exception.what());
    }
    fail(label, "expected std::domain_error");
}

Inputs make_inputs(const double x, const double A, const double B,
                   const double C, const double W, const double A_x,
                   const double A_z, const double B_x, const double B_z,
                   const double C_x, const double C_z, const double W_x,
                   const double W_z, const double z_over_chi_x = 0.0)
{
    return CartoonHatGammaX::make_hat_gamma_x_inputs(
        x, A, B, C, W, A_x, A_z, B_x, B_z, C_x, C_z, W_x, W_z,
        z_over_chi_x);
}

Result compute(const Inputs &inputs)
{
    return CartoonHatGammaX::compute_hat_gamma_x(inputs);
}

double determinant_reduced_identity_x(const double x, const double A,
                                      const double B, const double C,
                                      const double W,
                                      const double d_x_H_xx,
                                      const double d_z_H_xz)
{
    const double D = A * C - B * B;
    const double H_xx = C / D;
    return -d_x_H_xx - d_z_H_xz + (2.0 / x) * (1.0 / W - H_xx);
}

void check_type_boundary()
{
    using Function = decltype(&CartoonHatGammaX::compute_hat_gamma_x);

    static_assert(!std::is_aggregate<Inputs>::value,
                  "Stage 4AN inputs must not be an open aggregate");
    static_assert(!std::is_aggregate<Result>::value,
                  "Stage 4AN result must not be an open aggregate");
    static_assert(std::is_invocable<Function, const Inputs &>::value,
                  "Stage 4AN formula accepts only the checked input package");
    static_assert(!std::is_constructible<Inputs, double, double, double,
                                         double, double, double, double,
                                         double, double, double, double,
                                         double, double, double>::value,
                  "Stage 4AN input package must not be forgeable from raw "
                  "doubles");
    static_assert(!std::is_invocable<Function, double, double, double,
                                     double, double, double>::value,
                  "Stage 4AN formula must not accept loose raw values");

    // Former bypass shape, intentionally left as a non-compiling example:
    // compute_hat_gamma_x(A, B, C, W, D, Delta_xw, q_xz, Z_over_chi_x);
    std::cout << "PASS Stage 4AN type boundary is closed\n";
}

void check_flat_oracle()
{
    const auto inputs =
        make_inputs(2.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0);
    const auto result = compute(inputs);

    require_close("flat base contraction", result.base_contraction(), 0.0);
    require_close("flat hidden contraction", result.hidden_contraction(), 0.0);
    require_close("flat tilde Gamma x", result.tilde_gamma_x(), 0.0);
    require_close("flat hat Gamma x", result.hat_gamma_x(), 0.0);
}

void check_constant_cone_oracle()
{
    const auto inputs =
        make_inputs(2.0, 1.0, 0.0, 1.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0);
    const auto result = compute(inputs);

    require_close("constant-cone base contraction", result.base_contraction(),
                  0.0);
    require_close("constant-cone hidden contraction",
                  result.hidden_contraction(), -3.0 / 4.0);
    require_close("constant-cone tilde Gamma x", result.tilde_gamma_x(),
                  -3.0 / 4.0);

    const double one_hidden_copy = 0.5 * result.hidden_contraction();
    require_close("constant-cone hidden multiplicity-one wrong value",
                  one_hidden_copy, -3.0 / 8.0);
    require_not_close("constant-cone rejects multiplicity-one result",
                      result.tilde_gamma_x(), one_hidden_copy);
}

void check_determinant_one_offdiagonal_oracle()
{
    const auto inputs =
        make_inputs(2.0, 1.0, 1.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                    0.0, 0.0, 0.0);
    const auto result = compute(inputs);

    require_close("determinant-one off-diagonal tilde Gamma x",
                  result.tilde_gamma_x(), -1.0);
    const double reduced_identity =
        determinant_reduced_identity_x(2.0, 1.0, 1.0, 2.0, 1.0, 0.0, 0.0);
    require_close("determinant-reduced identity comparison",
                  result.tilde_gamma_x(), reduced_identity);
}

void check_distinct_derivative_oracle()
{
    const auto inputs =
        make_inputs(2.0, 7.0, 2.0, 5.0, 3.0, 3.0, 4.0, 5.0, 6.0, 7.0,
                    8.0, 6.0, 4.0);
    const auto result = compute(inputs);

    require_close("distinct checked Delta_xw", inputs.delta_xw(), 1.0);
    require_close("distinct checked q_xz", inputs.q_xz(), 1.0);
    require_close("distinct base contraction", result.base_contraction(),
                  27.0 / 961.0);
    require_close("distinct hidden contraction", result.hidden_contraction(),
                  -2.0 / 31.0);
    require_close("distinct tilde Gamma x", result.tilde_gamma_x(),
                  -35.0 / 961.0);
}

void check_hatted_convention_oracle()
{
    const auto inputs =
        make_inputs(2.0, 7.0, 2.0, 5.0, 3.0, 3.0, 4.0, 5.0, 6.0, 7.0,
                    8.0, 6.0, 4.0, 0.5);
    const auto result = compute(inputs);

    require_close("Z_over_chi x accessor", result.z_over_chi_x(), 0.5);
    require_close("hatted Gamma x with GRChombo convention",
                  result.hat_gamma_x(), 926.0 / 961.0);
    require_close("recover Z_over_chi from hatted minus tilde",
                  0.5 * (result.hat_gamma_x() - result.tilde_gamma_x()),
                  result.z_over_chi_x());
}

void check_invalid_inputs_reject()
{
    require_domain_error("zero axis rejected", []() {
        const auto inputs = make_inputs(0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0,
                                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        (void)compute(inputs);
    });
    require_domain_error("negative axis rejected", []() {
        const auto inputs = make_inputs(-1.0, 1.0, 0.0, 1.0, 1.0, 0.0,
                                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                        0.0);
        (void)compute(inputs);
    });
    require_domain_error("zero h_ww rejected", []() {
        const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        (void)compute(inputs);
    });
    require_domain_error("negative h_ww rejected", []() {
        const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, -1.0, 0.0,
                                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                        0.0);
        (void)compute(inputs);
    });
    require_domain_error("zero determinant rejected", []() {
        const auto inputs = make_inputs(2.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0,
                                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        (void)compute(inputs);
    });
    require_domain_error("negative determinant rejected", []() {
        const auto inputs = make_inputs(2.0, 1.0, 2.0, 1.0, 1.0, 0.0, 0.0,
                                        0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        (void)compute(inputs);
    });
    require_domain_error("nonfinite derivative rejected", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0, 1.0,
                        std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0);
        (void)compute(inputs);
    });
    require_domain_error("nonfinite Z_over_chi rejected", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0,
                        std::numeric_limits<double>::infinity());
        (void)compute(inputs);
    });
}

} // namespace

int main()
{
    // Stage 4AN implements only a local away-axis hatted-convention contract:
    // hat_Gamma^x = tilde_Gamma^x + 2 Z_over_chi^x. It is not the Gamma RHS,
    // not the Stage 4AO GL gate, not grid regularity validation, not finite
    // axis handling, and not evolution wiring.
    check_type_boundary();
    check_flat_oracle();
    check_constant_cone_oracle();
    check_determinant_one_offdiagonal_oracle();
    check_distinct_derivative_oracle();
    check_hatted_convention_oracle();
    check_invalid_inputs_reject();

    std::cout << "PASS Stage 4AN hat_Gamma^x tests\n";
    return 0;
}
