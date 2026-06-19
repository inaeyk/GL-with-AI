#include "CartoonConformalRwwGradientBlock.hpp"

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

using Block = CartoonConformalRwwGradientBlock::ConformalRwwGradientBlock;
using Inputs =
    CartoonConformalRwwGradientBlock::ConformalRwwGradientBlockInputs;
using CheckedDxhwwOverX =
    CartoonCheckedDxhwwOverX::CheckedDxhwwOverXIngredient;

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

void require_false(const std::string &label, const bool value)
{
    if (value)
    {
        fail(label, "expected false");
    }
    std::cout << "PASS " << label << " = false\n";
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

Inputs make_inputs(const double x, const double h_xx, const double h_xz,
                   const double h_zz, const double h_ww,
                   const double d_x_hww, const double d_z_hww)
{
    return CartoonConformalRwwGradientBlock::
        make_conformal_rww_gradient_block_inputs(x, h_xx, h_xz, h_zz, h_ww,
                                                 d_x_hww, d_z_hww);
}

Block compute_block(const Inputs &inputs)
{
    return CartoonConformalRwwGradientBlock::
        compute_conformal_rww_gradient_block(inputs);
}

void check_type_boundary()
{
    using Function =
        decltype(&CartoonConformalRwwGradientBlock::
                     compute_conformal_rww_gradient_block);

    static_assert(!std::is_aggregate<CheckedDxhwwOverX>::value,
                  "Stage 4Z checked p_W package must not be an open aggregate");
    static_assert(!std::is_aggregate<Inputs>::value,
                  "Stage 4Z input package must not be an open aggregate");
    static_assert(!std::is_aggregate<Block>::value,
                  "Stage 4Z output must not be an open aggregate");
    static_assert(std::is_invocable<Function, const Inputs &>::value,
                  "Stage 4Z formula must accept the single-source package");
    static_assert(!std::is_constructible<Inputs, double, double, double,
                                         double, double, double, double,
                                         double, double>::value,
                  "Stage 4Z input package must not be forgeable from raw "
                  "doubles");
    static_assert(!std::is_invocable<Function, double, double, double,
                                     double, double, double, double>::value,
                  "Stage 4Z formula must not accept loose raw local values");
    static_assert(!std::is_invocable<Function, double, double, double>::value,
                  "Stage 4Z formula must not accept loose q_xz, p_W, or "
                  "determinant values");

    // Former bypass shapes, intentionally left as non-compiling examples:
    // compute_conformal_rww_gradient_block(A, B, C, W, W_x, W_z, D, q_xz,
    //                                      p_W);
    // compute_conformal_rww_gradient_block(D, q_xz, p_W);
    std::cout << "PASS Stage 4Z single-source type boundary is closed\n";
}

void check_flat_oracle()
{
    const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0);
    const auto block = compute_block(inputs);

    require_close("flat gradient block", block.gradient_block(), 0.0);
}

void check_constant_cone_oracle()
{
    const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, 4.0, 0.0, 0.0);
    const auto block = compute_block(inputs);

    require_close("constant-cone gradient block", block.gradient_block(), 0.0);
}

void check_nonconstant_hidden_metric_oracle()
{
    const auto inputs = make_inputs(1.0, 1.0, 0.0, 1.0, 4.0, 4.0, 0.0);
    const auto block = compute_block(inputs);

    require_close("nonconstant hidden metric checked p_W", inputs.p_w(), 4.0);
    require_close("nonconstant hidden metric gradient block",
                  block.gradient_block(), -5.0);
}

void check_distinct_value_oracle()
{
    const auto inputs = make_inputs(2.0, 7.0, 2.0, 5.0, 3.0, 6.0, 4.0);
    const auto block = compute_block(inputs);

    require_close("distinct-value determinant", inputs.reduced_determinant(),
                  31.0);
    require_close("distinct-value checked q_xz", inputs.q_xz(), 1.0);
    require_close("distinct-value checked p_W", inputs.p_w(), 3.0);
    require_close("distinct-value gradient block", block.gradient_block(),
                  -82.0 / 93.0);
}

void check_invalid_axis_rejections()
{
    require_domain_error("zero axis", []() {
        const auto inputs = make_inputs(0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("negative axis", []() {
        const auto inputs = make_inputs(-1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("NaN axis", []() {
        const auto inputs =
            make_inputs(std::numeric_limits<double>::quiet_NaN(), 1.0, 0.0,
                        1.0, 1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("infinite axis", []() {
        const auto inputs =
            make_inputs(std::numeric_limits<double>::infinity(), 1.0, 0.0,
                        1.0, 1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
}

void check_invalid_hww_rejections()
{
    require_domain_error("zero h_ww", []() {
        const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("negative h_ww", []() {
        const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, -1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("NaN h_ww", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0,
                        std::numeric_limits<double>::quiet_NaN(), 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("infinite h_ww", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0,
                        std::numeric_limits<double>::infinity(), 1.0, 0.0);
        (void)compute_block(inputs);
    });
}

void check_determinant_rejections()
{
    require_domain_error("zero reduced determinant", []() {
        const auto inputs = make_inputs(2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("NaN reduced metric entry", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, std::numeric_limits<double>::quiet_NaN(),
                        1.0, 1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("infinite reduced metric entry", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, std::numeric_limits<double>::infinity(), 1.0,
                        1.0, 1.0, 0.0);
        (void)compute_block(inputs);
    });
}

void check_derivative_rejections()
{
    require_domain_error("NaN d_x h_ww", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0, 1.0,
                        std::numeric_limits<double>::quiet_NaN(), 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("infinite d_x h_ww", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0, 1.0,
                        std::numeric_limits<double>::infinity(), 0.0);
        (void)compute_block(inputs);
    });
    require_domain_error("NaN d_z h_ww", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0, 1.0, 1.0,
                        std::numeric_limits<double>::quiet_NaN());
        (void)compute_block(inputs);
    });
    require_domain_error("infinite d_z h_ww", []() {
        const auto inputs =
            make_inputs(2.0, 1.0, 0.0, 1.0, 1.0, 1.0,
                        std::numeric_limits<double>::infinity());
        (void)compute_block(inputs);
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4Z BlackStringToy conformal Rww first-derivative "
                 "gradient-block fixture\n";
    std::cout << "Checked gradient sub-block only; this is not full tilde "
                 "R_ww, not R^chi_ww, not physical R_ww[gamma], not full "
                 "Ricci, not CCZ4 RHS, not evolution wiring, and remains "
                 "away-axis only.\n";
    std::cout << "The checked W_x / x package is a local quotient token only; "
                 "it does not prove global parity or finite-axis regularity.\n";

    require_false("singular block implemented here",
                  CartoonConformalRwwGradientBlock::
                      singular_block_implemented_here);
    require_false("Hessian block implemented",
                  CartoonConformalRwwGradientBlock::hessian_block_implemented);
    require_false("full conformal hidden Ricci implemented",
                  CartoonConformalRwwGradientBlock::
                      full_conformal_hidden_ricci_implemented);
    require_false("conformal-factor Ricci implemented",
                  CartoonConformalRwwGradientBlock::
                      conformal_factor_ricci_implemented);
    require_false("physical Ricci implemented",
                  CartoonConformalRwwGradientBlock::physical_ricci_implemented);
    require_false("CCZ4 RHS implemented",
                  CartoonConformalRwwGradientBlock::ccz4_rhs_implemented);
    require_false("evolution wiring implemented",
                  CartoonConformalRwwGradientBlock::evolution_wiring_implemented);

    check_type_boundary();
    check_flat_oracle();
    check_constant_cone_oracle();
    check_nonconstant_hidden_metric_oracle();
    check_distinct_value_oracle();
    check_invalid_axis_rejections();
    check_invalid_hww_rejections();
    check_determinant_rejections();
    check_derivative_rejections();

    std::cout << "All Stage 4Z conformal Rww gradient-block checks passed.\n";
    return 0;
}
