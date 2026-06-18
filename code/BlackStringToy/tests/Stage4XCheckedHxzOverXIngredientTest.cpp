#include "CartoonCheckedHxzOverX.hpp"

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
using Sources =
    CartoonCheckedHxzOverX::CheckedHxzOverXIngredient;

constexpr double tol = 1.0e-12;

static_assert(!std::is_aggregate<Sources>::value,
              "checked h_xz / x ingredient must not be an open aggregate");

double dummy_future_consumer(const Sources &sources)
{
    // Future source formulas should consume this checked package, not raw
    // h_xz, x, or hand-rolled h_xz / x values. This dummy is diagnostic only:
    // it is not R_ww, not R^chi_ww, not CCZ4 RHS, and not evolution wiring.
    return sources.hxz_over_x();
}

using ConsumerPointer = decltype(&dummy_future_consumer);
static_assert(std::is_invocable<ConsumerPointer, const Sources &>::value,
              "dummy consumer must accept the checked package");
static_assert(!std::is_invocable<ConsumerPointer, double, double>::value,
              "dummy consumer must not accept raw doubles");

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

void check_regular_allowed_example()
{
    // h_xz = 3x is allowed because a finite nonzero h_xz / x is compatible
    // with odd parity. This fixture does not claim to reject all parity
    // violations; it checks only the local finite quotient and the
    // non-forgeable checked-package path.
    const CartoonCheckedHxzOverX::LocalInputs inputs{2.0, 6.0};
    const auto sources =
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            inputs);

    require_close("h_xz / x allowed quotient", sources.hxz_over_x(), 3.0);
    require_close("dummy future consumer", dummy_future_consumer(sources),
                  3.0);
    require_close("direct residual helper",
                  CartoonCheckedHxzOverX::
                      checked_hxz_over_x_quotient(2.0, 6.0),
                  3.0);
}

void check_zero_example()
{
    const auto sources =
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {2.0, 0.0});
    require_close("zero h_xz quotient", sources.hxz_over_x(), 0.0);
}

void check_axis_rejections()
{
    require_domain_error("x = 0", []() {
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {0.0, 1.0});
    });
    require_domain_error("negative x", []() {
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {-2.0, 1.0});
    });
    require_domain_error("NaN x", []() {
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {std::numeric_limits<double>::quiet_NaN(), 1.0});
    });
    require_domain_error("infinite x", []() {
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {std::numeric_limits<double>::infinity(), 1.0});
    });
}

void check_hxz_rejections()
{
    require_domain_error("NaN h_xz", []() {
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {2.0, std::numeric_limits<double>::quiet_NaN()});
    });
    require_domain_error("infinite h_xz", []() {
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {2.0, std::numeric_limits<double>::infinity()});
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4X BlackStringToy checked h_xz / x fixture\n";
    std::cout
        << "Local checked h_xz / x ingredient only. This finite-quotient "
           "check does not prove h_xz(-x,z) = -h_xz(x,z), does not prove "
           "h_xz = O(x), and is not tilde R_ww, not R^chi_ww, not full "
           "Ricci, not CCZ4 RHS, and not evolution wiring.\n";
    std::cout
        << "A true parity validation would require a two-sided parity check, "
           "Taylor/coefficient check, or grid-level near-axis policy. That is "
           "not implemented in Stage 4X.\n";

    require_false("small-axis regularization implemented",
                  CartoonCheckedHxzOverX::small_axis_regularization_implemented);
    require_false("global grid parity proven",
                  CartoonCheckedHxzOverX::proves_global_grid_parity);
    require_false("conformal hidden Ricci implemented",
                  CartoonCheckedHxzOverX::
                      conformal_hidden_ricci_implemented);
    require_false("conformal-factor Ricci implemented",
                  CartoonCheckedHxzOverX::
                      conformal_factor_ricci_implemented);
    require_false("CCZ4 RHS implemented",
                  CartoonCheckedHxzOverX::ccz4_rhs_implemented);
    require_false("evolution wiring implemented",
                  CartoonCheckedHxzOverX::evolution_wiring_implemented);

    check_regular_allowed_example();
    check_zero_example();
    check_axis_rejections();
    check_hxz_rejections();

    std::cout << "All Stage 4X checked h_xz / x ingredient checks passed.\n";
    return 0;
}
