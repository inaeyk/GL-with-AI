#include "CartoonHxzParityValidation.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

namespace
{
namespace Parity = BlackStringToy::CartoonHxzParityValidation;

constexpr double tolerance = 1.0e-12;

// Stage 4AG validates only h_xz odd parity from paired two-sided samples and
// optional explicit axis data. It does not validate h_xx-h_ww=O(x^2),
// W_x=O(x), chi_x=O(x), full smoothness, finite-axis regularity, physical
// R_ww, CCZ4 RHS, or evolution.
static_assert(!std::is_aggregate<Parity::ValidatedHxzParityStencil>::value,
              "Stage 4AG validated stencil must not be an open aggregate");
static_assert(
    !std::is_default_constructible<Parity::ValidatedHxzParityStencil>::value,
    "Stage 4AG validated stencil must be minted only by its factory");

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_true(const std::string &label, const bool condition)
{
    if (!condition)
    {
        fail(label, "condition was false");
    }
    std::cout << "PASS " << label << "\n";
}

void require_close(const std::string &label, const double value,
                   const double expected)
{
    if (std::abs(value - expected) > tolerance)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void require_throws(const std::string &label, const std::function<void()> &fn)
{
    try
    {
        fn();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    fail(label, "expected std::domain_error");
}

double odd_hxz(const double x, const double a, const double b)
{
    return a * x + b * x * x * x;
}

void check_smooth_odd_data()
{
    const double a = 1.7;
    const double b = -0.4;
    const std::vector<Parity::HxzParitySample> positive = {
        {0.1, odd_hxz(0.1, a, b)}, {0.3, odd_hxz(0.3, a, b)}};
    const std::vector<Parity::HxzParitySample> negative = {
        {-0.3, odd_hxz(-0.3, a, b)}, {-0.1, odd_hxz(-0.1, a, b)}};

    const auto validated =
        Parity::validate_hxz_odd_parity(positive, negative, 0.0, tolerance);
    require_true("smooth odd pair count", validated.pair_count() == 2);
    require_true("smooth odd axis checked", validated.axis_value_checked());
    require_close("smooth odd residual", validated.max_odd_residual(), 0.0);
    require_close("smooth quotient-even residual",
                  validated.max_quotient_residual(), 0.0);
}

void check_nonconstant_even_quotient()
{
    const double a = 2.0;
    const double b = 0.75;
    const std::vector<double> radii = {0.05, 0.2, 0.45};
    std::vector<Parity::HxzParitySample> positive;
    std::vector<Parity::HxzParitySample> negative;
    for (const double radius : radii)
    {
        positive.push_back({radius, odd_hxz(radius, a, b)});
        negative.push_back({-radius, odd_hxz(-radius, a, b)});
    }

    const auto validated = Parity::validate_hxz_odd_parity(
        positive, negative, std::nullopt, tolerance);
    require_true("multiple-radius pair count", validated.pair_count() == 3);
    require_true("optional axis omitted", !validated.axis_value_checked());

    const double inner_quotient = positive.front().h_xz / positive.front().x;
    const double outer_quotient = positive.back().h_xz / positive.back().x;
    require_true("h_xz/x is allowed to vary with radius",
                 std::abs(inner_quotient - outer_quotient) > tolerance);
}

void check_even_contamination_rejects()
{
    const double x = 0.25;
    const double a = 1.0;
    const double c = 0.5;
    const auto contaminated = [=](const double coordinate) {
        return a * coordinate + c * coordinate * coordinate;
    };
    require_throws("even x^2 contamination", [&]() {
        Parity::validate_hxz_odd_parity(
            {{x, contaminated(x)}}, {{-x, contaminated(-x)}}, 0.0,
            tolerance);
    });
}

void check_axis_and_partner_rejections()
{
    require_throws("nonzero axis value", []() {
        Parity::validate_hxz_odd_parity({{0.2, 0.2}}, {{-0.2, -0.2}},
                                        1.0e-4, tolerance);
    });
    require_throws("missing negative partner", []() {
        Parity::validate_hxz_odd_parity({{0.1, 0.1}, {0.2, 0.2}},
                                        {{-0.1, -0.1}}, 0.0, tolerance);
    });
    require_throws("mismatched negative radius", []() {
        Parity::validate_hxz_odd_parity({{0.2, 0.2}}, {{-0.3, -0.3}}, 0.0,
                                        tolerance);
    });
}

void check_invalid_sample_rejections()
{
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();

    require_throws("zero positive radius", []() {
        Parity::validate_hxz_odd_parity({{0.0, 0.0}}, {{-0.1, -0.1}}, 0.0,
                                        tolerance);
    });
    require_throws("negative positive-slot radius", []() {
        Parity::validate_hxz_odd_parity({{-0.1, -0.1}}, {{-0.1, -0.1}}, 0.0,
                                        tolerance);
    });
    require_throws("positive negative-slot radius", []() {
        Parity::validate_hxz_odd_parity({{0.1, 0.1}}, {{0.1, 0.1}}, 0.0,
                                        tolerance);
    });
    require_throws("NaN coordinate", [=]() {
        Parity::validate_hxz_odd_parity({{nan, 0.1}}, {{-0.1, -0.1}}, 0.0,
                                        tolerance);
    });
    require_throws("infinite h_xz", [=]() {
        Parity::validate_hxz_odd_parity({{0.1, inf}}, {{-0.1, -0.1}}, 0.0,
                                        tolerance);
    });
    require_throws("nonfinite axis value", [=]() {
        Parity::validate_hxz_odd_parity({{0.1, 0.1}}, {{-0.1, -0.1}}, nan,
                                        tolerance);
    });
    require_throws("invalid validation tolerance", []() {
        Parity::validate_hxz_odd_parity({{0.1, 0.1}}, {{-0.1, -0.1}}, 0.0,
                                        0.0);
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4AG h_xz parity validation gate\n";
    std::cout << "Paired local parity validation only; no regularization, "
                 "physical R_ww, RHS, or evolution wiring.\n";

    check_smooth_odd_data();
    check_nonconstant_even_quotient();
    check_even_contamination_rejects();
    check_axis_and_partner_rejections();
    check_invalid_sample_rejections();

    std::cout << "All Stage 4AG h_xz parity validation checks passed.\n";
    return 0;
}
