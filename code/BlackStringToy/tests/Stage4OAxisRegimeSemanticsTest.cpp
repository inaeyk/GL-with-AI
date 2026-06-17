#include "CartoonAxisPolicy.hpp"

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

void require_true(const std::string &label, const bool value)
{
    if (!value)
    {
        fail(label, "expected true");
    }
    std::cout << "PASS " << label << " = true\n";
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

void check_only_away_axis_mode()
{
    require_true("implemented axis mode is away-axis only",
                 CartoonAxisPolicy::implemented_axis_mode ==
                     CartoonAxisPolicy::AxisMode::AwayAxisOnly);
    require_false("regularized axis is implemented",
                  CartoonAxisPolicy::regularized_axis_implemented);
}

void check_away_axis_acceptance()
{
    require_true("positive finite x is away-axis",
                 CartoonAxisPolicy::is_away_axis(2.0));
    CartoonAxisPolicy::require_away_axis(2.0);
}

void check_axis_rejections()
{
    require_domain_error("x = 0", []() {
        CartoonAxisPolicy::require_away_axis(0.0);
    });
    require_domain_error("negative x", []() {
        CartoonAxisPolicy::require_away_axis(-1.0);
    });
    require_domain_error("NaN x", []() {
        CartoonAxisPolicy::require_away_axis(
            std::numeric_limits<double>::quiet_NaN());
    });
    require_domain_error("infinite x", []() {
        CartoonAxisPolicy::require_away_axis(
            std::numeric_limits<double>::infinity());
    });
}

void check_inverse_primitives()
{
    require_close("inverse x away-axis",
                  CartoonAxisPolicy::inverse_x_away_axis(2.0), 0.5);
    require_close("inverse x^2 away-axis",
                  CartoonAxisPolicy::inverse_x2_away_axis(2.0), 0.25);

    require_domain_error("inverse x^2 rejects x = 0", []() {
        (void)CartoonAxisPolicy::inverse_x2_away_axis(0.0);
    });
    require_domain_error("inverse x^2 rejects negative x", []() {
        (void)CartoonAxisPolicy::inverse_x2_away_axis(-1.0);
    });
    require_domain_error("inverse x^2 rejects NaN x", []() {
        (void)CartoonAxisPolicy::inverse_x2_away_axis(
            std::numeric_limits<double>::quiet_NaN());
    });
    require_domain_error("inverse x^2 rejects infinite x", []() {
        (void)CartoonAxisPolicy::inverse_x2_away_axis(
            std::numeric_limits<double>::infinity());
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4O BlackStringToy axis-regime semantics fixture\n";
    std::cout << "AwayAxisOnly is the only implemented regime; no Stage 3I "
                 "small-axis regularization, clamp, or epsilon replacement is "
                 "implemented.\n";

    check_only_away_axis_mode();
    check_away_axis_acceptance();
    check_axis_rejections();
    check_inverse_primitives();

    std::cout << "All Stage 4O axis-regime checks passed.\n";
    return 0;
}
