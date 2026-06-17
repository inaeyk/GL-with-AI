#ifndef BLACKSTRINGTOY_CARTOONREGULARITYCHECKS_HPP
#define BLACKSTRINGTOY_CARTOONREGULARITYCHECKS_HPP

#include "CartoonAxisPolicy.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace BlackStringToy
{
namespace CartoonRegularityChecks
{
// Stage 4Q local guard only. This catches obvious pointwise violations of the
// h_xx - h_ww = O(x^2) matching condition before future near-axis use of
// (h_xx - h_ww) / x^2. It is not Stage 3I small-axis regularization and cannot
// prove analytic regularity from one local point.
static constexpr double hxx_hww_matching_tolerance = 1.0;
static constexpr bool small_axis_regularization_implemented = false;
static constexpr bool proves_analytic_regularity = false;

inline void require_finite_value(const double value, const char *label)
{
    if (!std::isfinite(value))
    {
        throw std::domain_error(
            std::string("CartoonRegularityChecks requires finite ") + label);
    }
}

inline double matching_scale(const double h_xx, const double h_ww)
{
    require_finite_value(h_xx, "h_xx");
    require_finite_value(h_ww, "h_ww");

    double scale = 1.0;
    const double abs_h_xx = std::abs(h_xx);
    const double abs_h_ww = std::abs(h_ww);
    if (abs_h_xx > scale)
    {
        scale = abs_h_xx;
    }
    if (abs_h_ww > scale)
    {
        scale = abs_h_ww;
    }
    return scale;
}

inline double hxx_hww_matching_residual(const double x, const double h_xx,
                                        const double h_ww)
{
    CartoonAxisPolicy::require_away_axis(x);

    const double scale = matching_scale(h_xx, h_ww);
    return std::abs(h_xx - h_ww) / (x * x * scale);
}

inline void require_hxx_hww_axis_matching(
    const double x, const double h_xx, const double h_ww,
    const double tolerance = hxx_hww_matching_tolerance)
{
    require_finite_value(tolerance, "hxx-hww matching tolerance");
    if (tolerance <= 0.0)
    {
        throw std::domain_error(
            "CartoonRegularityChecks requires positive matching tolerance");
    }

    const double residual = hxx_hww_matching_residual(x, h_xx, h_ww);
    if (residual > tolerance)
    {
        throw std::domain_error(
            "CartoonRegularityChecks requires h_xx - h_ww = O(x^2) "
            "before near-axis use of (h_xx - h_ww) / x^2");
    }
}

} // namespace CartoonRegularityChecks
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONREGULARITYCHECKS_HPP */
