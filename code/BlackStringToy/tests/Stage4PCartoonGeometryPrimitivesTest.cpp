#include "CartoonGeometryPrimitives.hpp"
#include "CartoonSingularCombinations.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace
{
using namespace BlackStringToy;

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

template <class T, class = void> struct has_public_risky_metric_difference
    : std::false_type
{
};

template <class T>
struct has_public_risky_metric_difference<
    T, std::void_t<decltype(std::declval<T>().hxx_minus_hww_over_x2)>>
    : std::true_type
{
};

CartoonGeometryPrimitives::LocalInputs oracle_inputs()
{
    return {2.0, 6.0};
}

void check_oracle_values()
{
    const auto primitives =
        CartoonGeometryPrimitives::compute(oracle_inputs());

    require_close("dx hww over x", primitives.dx_hww_over_x, 3.0);

    require_close("dx hww agrees with Stage 4N helper",
                  primitives.dx_hww_over_x,
                  CartoonSingularCombinations::first_derivative_over_x(6.0,
                                                                       2.0));
}

void check_risky_value_is_not_public_stage4p_output()
{
    // Stage 4P remains the low-risk away-axis primitive layer. The
    // regularity-sensitive (h_xx - h_ww) / x^2 value is intentionally not a
    // public field on the raw Stage 4P output. Source-facing use must go
    // through the Stage 4R guarded path after the Stage 4Q matching guard.
    using RawStage4POutput = decltype(CartoonGeometryPrimitives::compute(
        std::declval<CartoonGeometryPrimitives::LocalInputs>()));
    require_false("raw Stage 4P exposes hxx-hww over x^2",
                  has_public_risky_metric_difference<
                      RawStage4POutput>::value);
}

void check_axis_rejections()
{
    auto inputs = oracle_inputs();

    inputs.x = 0.0;
    require_domain_error("x = 0", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
    inputs.x = -1.0;
    require_domain_error("negative x", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
    inputs.x = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN x", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
    inputs.x = std::numeric_limits<double>::infinity();
    require_domain_error("infinite x", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });
}

void check_value_rejections()
{
    auto inputs = oracle_inputs();

    inputs.d_x_hww = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN d_x_hww", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
    inputs.d_x_hww = std::numeric_limits<double>::infinity();
    require_domain_error("infinite d_x_hww", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });
}

} // namespace

int main()
{
    std::cout
        << "Stage 4P BlackStringToy cartoon geometry primitives fixture\n";
    std::cout << "Away-axis named primitives only; this is not a full Ricci/RHS "
                 "source term and not regularized-axis support.\n";

    require_false("small-axis regularization implemented",
                  CartoonGeometryPrimitives::
                      small_axis_regularization_implemented);
    require_false("full Ricci or RHS formula implemented",
                  CartoonGeometryPrimitives::
                      full_ricci_or_rhs_formula_implemented);
    check_oracle_values();
    check_risky_value_is_not_public_stage4p_output();
    check_axis_rejections();
    check_value_rejections();

    std::cout << "All Stage 4P geometry-primitive checks passed.\n";
    return 0;
}
