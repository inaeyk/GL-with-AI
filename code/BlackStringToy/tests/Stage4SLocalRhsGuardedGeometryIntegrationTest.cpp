#include "CartoonRhsSourceBlock.hpp"

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

void require_true(const std::string &label, const bool value)
{
    if (!value)
    {
        fail(label, "expected true");
    }
    std::cout << "PASS " << label << " = true\n";
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

CartoonRhsSourceBlock::LocalGuardedGeometrySourceBlockInput matching_input()
{
    const double x = 0.5;
    const double h_ww = 1.0;
    const double h_xx = h_ww + 0.25 * x * x;
    const double d_x_hww = 1.0;
    return {{x, h_xx, h_ww, d_x_hww}};
}

void check_matching_case()
{
    const auto input = matching_input();
    const auto output =
        CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    const auto direct_guarded =
        CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(
                input.guarded_geometry);

    require_close(
        "source-block dx hww over x",
        output.regularity_guarded_geometry().dx_hww_over_x(), 2.0);
    require_close(
        "source-block hxx minus hww over x^2",
        output.regularity_guarded_geometry().hxx_minus_hww_over_x2(), 0.25);
    require_close(
        "source-block matching residual",
        output.regularity_guarded_geometry().hxx_hww_matching_residual(),
        0.25 / input.guarded_geometry.h_xx);

    // Stage 4S deliberately routes the local RHS source-block layer through
    // Stage 4R. The risky metric-difference ingredient is not obtained from a
    // raw Stage 4P output path.
    require_close("source-block agrees with Stage 4R dx hww",
                  output.regularity_guarded_geometry().dx_hww_over_x(),
                  direct_guarded.dx_hww_over_x());
    require_close("source-block agrees with Stage 4R metric difference",
                  output.regularity_guarded_geometry()
                      .hxx_minus_hww_over_x2(),
                  direct_guarded.hxx_minus_hww_over_x2());
}

void check_source_block_output_is_not_forgeable()
{
    using Guarded =
        CartoonRegularityGuardedSources::RegularityGuardedGeometrySources;
    using Output =
        CartoonRhsSourceBlock::LocalGuardedGeometrySourceBlockOutput;

    static_assert(!std::is_aggregate<Guarded>::value,
                  "guarded geometry package must not be an open aggregate");
    static_assert(!std::is_constructible<Guarded, double, double, double>::value,
                  "guarded geometry package must not be directly built from "
                  "unchecked doubles");
    static_assert(!std::is_aggregate<Output>::value,
                  "source-block carry output must not be an open aggregate");
    static_assert(!std::is_constructible<Output, Guarded>::value,
                  "source-block carry output must be built by the compute "
                  "path, not by callers");

    // Former forgeable paths, intentionally left as non-compiling examples:
    // Guarded fake_guarded{1.0, 2.0, 3.0};
    // Output fake_output{fake_guarded};
    std::cout << "PASS source-block guarded output is not forgeable\n";
}

void check_mismatch_rejection()
{
    require_domain_error("source-block O(1) hxx-hww mismatch", []() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            {{0.1, 1.2, 1.0, 1.0}});
    });
}

void check_axis_rejections()
{
    auto input = matching_input();

    input.guarded_geometry.x = 0.0;
    require_domain_error("source-block x = 0", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.x = -0.5;
    require_domain_error("source-block negative x", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.x = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("source-block NaN x", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.x = std::numeric_limits<double>::infinity();
    require_domain_error("source-block infinite x", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });
}

void check_value_rejections()
{
    auto input = matching_input();

    input.guarded_geometry.h_xx = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("source-block NaN h_xx", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.h_xx = std::numeric_limits<double>::infinity();
    require_domain_error("source-block infinite h_xx", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.h_ww = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("source-block NaN h_ww", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.h_ww = std::numeric_limits<double>::infinity();
    require_domain_error("source-block infinite h_ww", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.d_x_hww =
        std::numeric_limits<double>::quiet_NaN();
    require_domain_error("source-block NaN d_x_hww", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });

    input = matching_input();
    input.guarded_geometry.d_x_hww = std::numeric_limits<double>::infinity();
    require_domain_error("source-block infinite d_x_hww", [&]() {
        (void)CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            input);
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4S BlackStringToy local RHS guarded-geometry "
                 "integration fixture\n";
    std::cout << "Local source-block integration only; this is not a full "
                 "Ricci/RHS source term and not evolution wiring.\n";

    require_false("source terms are implemented",
                  CartoonRhsSourceBlock::source_terms_implemented);
    require_false("evolution wiring is implemented",
                  CartoonRhsSourceBlock::evolution_wiring_implemented);
    require_true("regularity-guarded geometry source block is available",
                 CartoonRhsSourceBlock::
                     regularity_guarded_geometry_source_block_available);

    check_matching_case();
    check_source_block_output_is_not_forgeable();
    check_mismatch_rejection();
    check_axis_rejections();
    check_value_rejections();

    std::cout << "All Stage 4S local RHS guarded-geometry checks passed.\n";
    return 0;
}
