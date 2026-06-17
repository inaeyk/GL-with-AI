#include "CartoonAxisPolicy.hpp"
#include "CartoonRhsSourceBlock.hpp"

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

CartoonRhsSourceBlock::LocalSourceBlockInput make_source_input(const double x)
{
    const auto ricci = CartoonRicciBridge::to_rhs_ricci_components(
        CartoonRicci::RicciComponents(2.0, 3.0, 5.0, 7.0));
    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{1.0, 0.0, 1.0,
                                                               1.0};
    const ConformalCartoonAlgebra::ConformalMetric h_LL{1.0, 0.0, 1.0, 1.0};
    return {{{ricci, h_UU, 0.5}, x}, h_LL};
}

void check_axis_policy_values()
{
    require_true("positive finite x is away-axis",
                 CartoonAxisPolicy::is_away_axis(2.0));
    CartoonAxisPolicy::require_away_axis(2.0);

    require_false("x = 0 is away-axis",
                  CartoonAxisPolicy::is_away_axis(0.0));
    require_false("negative x is away-axis",
                  CartoonAxisPolicy::is_away_axis(-1.0));
    require_false("NaN x is away-axis",
                  CartoonAxisPolicy::is_away_axis(
                      std::numeric_limits<double>::quiet_NaN()));
    require_false("infinite x is away-axis",
                  CartoonAxisPolicy::is_away_axis(
                      std::numeric_limits<double>::infinity()));

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

    require_close("inverse x away-axis",
                  CartoonAxisPolicy::inverse_x_away_axis(2.0), 0.5);
    require_close("inverse x^2 away-axis",
                  CartoonAxisPolicy::inverse_x2_away_axis(2.0), 0.25);
}

void check_stage4l_boundary_still_rejects_axis()
{
    auto source_input = make_source_input(0.0);
    require_domain_error("Stage 4L source-block axis boundary",
                         [&source_input]() {
                             (void)CartoonRhsSourceBlock::
                                 compute_ricci_trace_free_source_block(
                                     source_input);
                         });
}

} // namespace

int main()
{
    std::cout << "Stage 4M BlackStringToy cartoon axis-policy fixture\n";
    std::cout << "Away-axis policy only; no Stage 3I small-axis "
                 "regularization is implemented.\n";

    check_axis_policy_values();
    check_stage4l_boundary_still_rejects_axis();

    std::cout << "All Stage 4M axis-policy checks passed.\n";
    return 0;
}
