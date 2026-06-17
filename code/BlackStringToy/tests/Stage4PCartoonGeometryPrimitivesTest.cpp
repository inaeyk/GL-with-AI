#include "CartoonGeometryPrimitives.hpp"
#include "CartoonSingularCombinations.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

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

CartoonGeometryPrimitives::LocalInputs oracle_inputs()
{
    return {2.0, 10.0, 6.0, 6.0};
}

void check_oracle_values()
{
    const auto primitives =
        CartoonGeometryPrimitives::compute(oracle_inputs());

    require_close("dx hww over x", primitives.dx_hww_over_x, 3.0);
    require_close("hxx minus hww over x^2",
                  primitives.hxx_minus_hww_over_x2, 1.0);

    require_close("dx hww agrees with Stage 4N helper",
                  primitives.dx_hww_over_x,
                  CartoonSingularCombinations::first_derivative_over_x(6.0,
                                                                       2.0));
    require_close("metric difference agrees with Stage 4N helper",
                  primitives.hxx_minus_hww_over_x2,
                  CartoonSingularCombinations::difference_over_x2(10.0, 6.0,
                                                                  2.0));
}

void check_large_finite_value_is_not_regularity_proof()
{
    // This deliberately passes. Away from the axis the primitive may return a
    // large finite value even when h_xx - h_ww does not satisfy the Stage 3I
    // matching condition h_xx - h_ww = O(x^2). That finite quotient is not a
    // regularity validation and must not be used as one.
    const auto primitives =
        CartoonGeometryPrimitives::compute({1.0e-2, 2.0, 1.0, 0.0});

    require_close("unmatched metric difference remains finite away-axis",
                  primitives.hxx_minus_hww_over_x2, 10000.0);
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

    inputs.h_xx = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN h_xx", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
    inputs.h_xx = std::numeric_limits<double>::infinity();
    require_domain_error("infinite h_xx", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
    inputs.h_ww = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN h_ww", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
    inputs.h_ww = std::numeric_limits<double>::infinity();
    require_domain_error("infinite h_ww", [&]() {
        (void)CartoonGeometryPrimitives::compute(inputs);
    });

    inputs = oracle_inputs();
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
    check_large_finite_value_is_not_regularity_proof();
    check_axis_rejections();
    check_value_rejections();

    std::cout << "All Stage 4P geometry-primitive checks passed.\n";
    return 0;
}
