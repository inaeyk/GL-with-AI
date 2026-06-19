#include "CartoonConformalRwwSingularBlock.hpp"

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

using Block =
    CartoonConformalRwwSingularBlock::ConformalRwwSingularGradientBlock;
using Inputs =
    CartoonConformalRwwSingularBlock::ConformalRwwSingularBlockInputs;

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
                   const double h_zz, const double h_ww)
{
    return CartoonConformalRwwSingularBlock::
        make_conformal_rww_singular_block_inputs(x, h_xx, h_xz, h_zz, h_ww);
}

Block compute_block(const Inputs &inputs)
{
    return CartoonConformalRwwSingularBlock::
        compute_conformal_rww_singular_gradient_block(inputs);
}

void check_type_boundary()
{
    using Function = decltype(
        &CartoonConformalRwwSingularBlock::
            compute_conformal_rww_singular_gradient_block);

    static_assert(!std::is_aggregate<Block>::value,
                  "Stage 4Y output must not be an open aggregate");
    static_assert(!std::is_aggregate<Inputs>::value,
                  "Stage 4Y input package must not be an open aggregate");
    static_assert(std::is_invocable<Function, const Inputs &>::value,
                  "Stage 4Y formula must accept the single-source package");
    static_assert(!std::is_constructible<Inputs, double, double, double,
                                         double, double, double>::value,
                  "Stage 4Y input package must not be forgeable from raw "
                  "doubles");
    static_assert(!std::is_invocable<Function, double, double, double,
                                     double, double>::value,
                  "Stage 4Y formula must not accept one loose raw point");
    static_assert(!std::is_invocable<Function, double, double, double,
                                     double, double, double, double>::value,
                  "Stage 4Y formula must not accept raw determinant plus "
                  "singular values");

    // Former bypass shapes, intentionally left as non-compiling examples:
    // compute_conformal_rww_singular_gradient_block(metric, diagonal,
    //                                               checked_hxz_over_x);
    // compute_conformal_rww_singular_gradient_block(metric, delta_xw, q_xz);
    std::cout << "PASS Stage 4Y single-source type boundary is closed\n";
}

void check_flat_oracle()
{
    const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, 1.0);
    const auto block = compute_block(inputs);

    require_close("flat singular gradient block",
                  block.singular_gradient_block(), 0.0);
}

void check_constant_cone_oracle()
{
    const auto inputs = make_inputs(2.0, 1.0, 0.0, 1.0, 4.0);
    const auto block = compute_block(inputs);

    require_close("constant-cone singular gradient block",
                  block.singular_gradient_block(), -0.75);
}

void check_distinct_value_oracle()
{
    const auto inputs = make_inputs(2.0, 7.0, 2.0, 5.0, 3.0);
    const auto block = compute_block(inputs);

    require_close("distinct-value determinant", inputs.reduced_determinant(),
                  31.0);
    require_close("distinct-value checked Delta_xw", inputs.delta_xw(), 1.0);
    require_close("distinct-value checked q_xz", inputs.q_xz(), 1.0);

    require_close("distinct-value singular gradient block",
                  block.singular_gradient_block(), 4.0 / 31.0);
}

void check_diagonal_mismatch_fails_before_formula()
{
    require_domain_error("diagonal mismatch before Stage 4Y formula", []() {
        const auto inputs = make_inputs(0.1, 1.2, 0.0, 1.0, 1.0);
        (void)compute_block(inputs);
    });
}

void check_invalid_hxz_fails_before_formula()
{
    require_domain_error("invalid h_xz / x before Stage 4Y formula", []() {
        const auto inputs = make_inputs(0.0, 1.0, 1.0, 2.0, 1.0);
        (void)compute_block(inputs);
    });
}

void check_determinant_rejections()
{
    require_domain_error("zero reduced determinant", [&]() {
        const auto inputs = make_inputs(2.0, 1.0, 1.0, 1.0, 1.0);
        (void)compute_block(inputs);
    });
    require_domain_error("negative reduced determinant", [&]() {
        // Checkpoint B unifies the reduced determinant policy across the
        // conformal Rww pieces: the Stage 4Y singular block now rejects D <= 0
        // just like the Stage 4Z gradient, Stage 4AB Hessian, and Stage 4AC
        // assembler paths.
        const auto inputs = make_inputs(2.0, 1.0, 2.0, 1.0, 1.0);
        (void)compute_block(inputs);
    });
    require_domain_error("NaN reduced metric entry", [&]() {
        const auto inputs = make_inputs(2.0, 1.0,
                                        std::numeric_limits<double>::quiet_NaN(),
                                        1.0, 1.0);
        (void)compute_block(inputs);
    });
    require_domain_error("infinite reduced metric entry", [&]() {
        const auto inputs = make_inputs(2.0, 1.0,
                                        std::numeric_limits<double>::infinity(),
                                        1.0, 1.0);
        (void)compute_block(inputs);
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4Y BlackStringToy conformal Rww singular-gradient "
                 "sub-block fixture\n";
    std::cout << "Checked singular sub-block only; this is not full tilde "
                 "R_ww, not R^chi_ww, not physical R_ww[gamma], not full "
                 "Ricci, not CCZ4 RHS, not evolution wiring, and remains "
                 "away-axis only.\n";
    std::cout << "The Stage 4Y input package is single-sourced from one local "
                 "metric point so determinant data cannot be mixed with "
                 "checked singular ingredients from another point. This does "
                 "not implement more of tilde R_ww.\n";

    require_false("full conformal hidden Ricci implemented",
                  CartoonConformalRwwSingularBlock::
                      full_conformal_hidden_ricci_implemented);
    require_false("conformal-factor Ricci implemented",
                  CartoonConformalRwwSingularBlock::
                      conformal_factor_ricci_implemented);
    require_false("physical Ricci implemented",
                  CartoonConformalRwwSingularBlock::physical_ricci_implemented);
    require_false("CCZ4 RHS implemented",
                  CartoonConformalRwwSingularBlock::ccz4_rhs_implemented);
    require_false("evolution wiring implemented",
                  CartoonConformalRwwSingularBlock::evolution_wiring_implemented);

    check_type_boundary();
    check_flat_oracle();
    check_constant_cone_oracle();
    check_distinct_value_oracle();
    check_diagonal_mismatch_fails_before_formula();
    check_invalid_hxz_fails_before_formula();
    check_determinant_rejections();

    std::cout << "All Stage 4Y conformal Rww singular-gradient checks passed.\n";
    return 0;
}
