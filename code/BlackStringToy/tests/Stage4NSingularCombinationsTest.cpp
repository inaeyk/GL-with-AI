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

void check_positive_finite_values()
{
    require_close("first derivative over x",
                  CartoonSingularCombinations::first_derivative_over_x(6.0,
                                                                        2.0),
                  3.0);
    require_close("difference over x^2",
                  CartoonSingularCombinations::difference_over_x2(10.0, 6.0,
                                                                  2.0),
                  1.0);

    require_close("first derivative agrees with axis inverse",
                  CartoonSingularCombinations::first_derivative_over_x(6.0,
                                                                        2.0),
                  6.0 * CartoonAxisPolicy::inverse_x_away_axis(2.0));
    require_close("difference agrees with axis inverse x^2",
                  CartoonSingularCombinations::difference_over_x2(10.0, 6.0,
                                                                  2.0),
                  (10.0 - 6.0) *
                      CartoonAxisPolicy::inverse_x2_away_axis(2.0));
}

void check_bad_x_rejections()
{
    require_domain_error("first derivative over x rejects x = 0", []() {
        (void)CartoonSingularCombinations::first_derivative_over_x(6.0, 0.0);
    });
    require_domain_error("difference over x^2 rejects x = 0", []() {
        (void)CartoonSingularCombinations::difference_over_x2(10.0, 6.0, 0.0);
    });

    require_domain_error("negative x", []() {
        (void)CartoonSingularCombinations::first_derivative_over_x(6.0, -1.0);
    });
    require_domain_error("NaN x", []() {
        (void)CartoonSingularCombinations::difference_over_x2(
            10.0, 6.0, std::numeric_limits<double>::quiet_NaN());
    });
    require_domain_error("infinite x", []() {
        (void)CartoonSingularCombinations::first_derivative_over_x(
            6.0, std::numeric_limits<double>::infinity());
    });
}

void check_nonfinite_value_rejections()
{
    require_domain_error("NaN derivative value", []() {
        (void)CartoonSingularCombinations::first_derivative_over_x(
            std::numeric_limits<double>::quiet_NaN(), 2.0);
    });
    require_domain_error("infinite derivative value", []() {
        (void)CartoonSingularCombinations::first_derivative_over_x(
            std::numeric_limits<double>::infinity(), 2.0);
    });
    require_domain_error("NaN difference value", []() {
        (void)CartoonSingularCombinations::difference_over_x2(
            std::numeric_limits<double>::quiet_NaN(), 6.0, 2.0);
    });
    require_domain_error("infinite difference value", []() {
        (void)CartoonSingularCombinations::difference_over_x2(
            10.0, std::numeric_limits<double>::infinity(), 2.0);
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4N BlackStringToy singular-combinations fixture\n";
    std::cout << "Away-axis singular combinations only; no Stage 3I "
                 "small-axis regularization is implemented.\n";

    check_positive_finite_values();
    check_bad_x_rejections();
    check_nonfinite_value_rejections();

    std::cout << "All Stage 4N singular-combination checks passed.\n";
    return 0;
}
