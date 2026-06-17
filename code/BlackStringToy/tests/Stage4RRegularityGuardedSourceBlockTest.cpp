#include "CartoonGeometryPrimitives.hpp"
#include "CartoonRegularityGuardedSources.hpp"

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

CartoonRegularityGuardedSources::LocalInputs matching_inputs()
{
    const double x = 0.5;
    const double h_ww = 1.0;
    const double h_xx = h_ww + 0.25 * x * x;
    const double d_x_hww = 1.0;
    return {x, h_xx, h_ww, d_x_hww};
}

void check_matching_case()
{
    const auto inputs = matching_inputs();
    const auto guarded =
        CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    const auto primitives = CartoonGeometryPrimitives::compute(
        {inputs.x, inputs.d_x_hww});

    require_close("dx hww over x", guarded.dx_hww_over_x, 2.0);
    require_close("hxx minus hww over x^2",
                  guarded.hxx_minus_hww_over_x2, 0.25);
    require_close("matching residual", guarded.hxx_hww_matching_residual,
                  0.25 / inputs.h_xx);

    require_close("guarded dx hww agrees with Stage 4P",
                  guarded.dx_hww_over_x, primitives.dx_hww_over_x);

    // The regularity-sensitive metric-difference value is intentionally not
    // available from the raw Stage 4P output. The source-facing public double
    // appears here only after the Stage 4Q matching guard has accepted the
    // inputs.
}

void check_mismatch_rejection()
{
    require_domain_error("O(1) hxx-hww mismatch", []() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(
                {0.1, 1.2, 1.0, 1.0});
    });
}

void check_axis_rejections()
{
    auto inputs = matching_inputs();

    inputs.x = 0.0;
    require_domain_error("x = 0", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.x = -0.5;
    require_domain_error("negative x", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.x = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN x", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.x = std::numeric_limits<double>::infinity();
    require_domain_error("infinite x", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });
}

void check_value_rejections()
{
    auto inputs = matching_inputs();

    inputs.h_xx = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN h_xx", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.h_xx = std::numeric_limits<double>::infinity();
    require_domain_error("infinite h_xx", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.h_ww = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN h_ww", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.h_ww = std::numeric_limits<double>::infinity();
    require_domain_error("infinite h_ww", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.d_x_hww = std::numeric_limits<double>::quiet_NaN();
    require_domain_error("NaN d_x_hww", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });

    inputs = matching_inputs();
    inputs.d_x_hww = std::numeric_limits<double>::infinity();
    require_domain_error("infinite d_x_hww", [&]() {
        (void)CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(inputs);
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4R BlackStringToy regularity-guarded source-block "
                 "fixture\n";
    std::cout
        << "Local guarded source-style ingredients only; this is not a full "
           "Ricci/RHS source term and not Stage 3I regularization.\n";

    require_false("full Ricci or RHS formula implemented",
                  CartoonRegularityGuardedSources::
                      full_ricci_or_rhs_formula_implemented);
    require_false("small-axis regularization implemented",
                  CartoonRegularityGuardedSources::
                      small_axis_regularization_implemented);
    require_false("evolution wiring implemented",
                  CartoonRegularityGuardedSources::
                      evolution_wiring_implemented);

    check_matching_case();
    check_mismatch_rejection();
    check_axis_rejections();
    check_value_rejections();

    std::cout << "All Stage 4R guarded source-block checks passed.\n";
    return 0;
}
