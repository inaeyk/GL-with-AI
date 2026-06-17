#include "CartoonRegularityChecks.hpp"

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

void check_matching_case_passes()
{
    const double x = 0.1;
    const double h_ww = 1.0;
    const double h_xx = h_ww + 0.25 * x * x;

    CartoonRegularityChecks::require_hxx_hww_axis_matching(x, h_xx, h_ww);
    require_close("hxx-hww matching residual",
                  CartoonRegularityChecks::hxx_hww_matching_residual(
                      x, h_xx, h_ww),
                  0.25 / h_xx);
}

void check_clear_mismatch_fails()
{
    require_domain_error("O(1) hxx-hww mismatch", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(0.1, 1.2, 1.0);
    });
}

void check_axis_rejections()
{
    require_domain_error("x = 0", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(0.0, 1.0,
                                                               1.0);
    });
    require_domain_error("negative x", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(-0.1, 1.0,
                                                               1.0);
    });
    require_domain_error("NaN x", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(
            std::numeric_limits<double>::quiet_NaN(), 1.0, 1.0);
    });
    require_domain_error("infinite x", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(
            std::numeric_limits<double>::infinity(), 1.0, 1.0);
    });
}

void check_value_rejections()
{
    require_domain_error("NaN h_xx", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(
            0.1, std::numeric_limits<double>::quiet_NaN(), 1.0);
    });
    require_domain_error("infinite h_xx", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(
            0.1, std::numeric_limits<double>::infinity(), 1.0);
    });
    require_domain_error("NaN h_ww", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(
            0.1, 1.0, std::numeric_limits<double>::quiet_NaN());
    });
    require_domain_error("infinite h_ww", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(
            0.1, 1.0, std::numeric_limits<double>::infinity());
    });
    require_domain_error("NaN tolerance", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(
            0.1, 1.0, 1.0, std::numeric_limits<double>::quiet_NaN());
    });
    require_domain_error("nonpositive tolerance", []() {
        CartoonRegularityChecks::require_hxx_hww_axis_matching(0.1, 1.0, 1.0,
                                                               0.0);
    });
}

} // namespace

int main()
{
    std::cout
        << "Stage 4Q BlackStringToy regularity matching fixture\n";
    std::cout
        << "Local pointwise guard only; this is not a proof of analytic "
           "regularity and not Stage 3I small-axis regularization.\n";

    require_false("small-axis regularization implemented",
                  CartoonRegularityChecks::
                      small_axis_regularization_implemented);
    require_false("analytic regularity proven",
                  CartoonRegularityChecks::proves_analytic_regularity);
    check_matching_case_passes();
    check_clear_mismatch_fails();
    check_axis_rejections();
    check_value_rejections();

    std::cout << "All Stage 4Q regularity-matching checks passed.\n";
    return 0;
}
