#ifndef BLACKSTRINGTOY_CARTOONSINGULARCOMBINATIONS_HPP
#define BLACKSTRINGTOY_CARTOONSINGULARCOMBINATIONS_HPP

#include "CartoonAxisPolicy.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace BlackStringToy
{
namespace CartoonSingularCombinations
{
// Stage 4N helpers only. These helpers are away-axis-only singular
// combinations. They are not Stage 3I small-axis regularization.
//
// Do not use raw 1/x or 1/x^2 in future cartoon source code; route through
// CartoonAxisPolicy or these helpers. Future regularized/clamped behavior must
// define 1/x and 1/x^2 semantics separately.
inline void require_finite_value(const double value, const char *label)
{
    if (!std::isfinite(value))
    {
        throw std::domain_error(
            std::string("CartoonSingularCombinations requires finite ") +
            label);
    }
}

inline double first_derivative_over_x(const double dx_f, const double x)
{
    require_finite_value(dx_f, "first derivative value");
    return dx_f * CartoonAxisPolicy::inverse_x_away_axis(x);
}

inline double difference_over_x2(const double f, const double g,
                                 const double x)
{
    require_finite_value(f, "difference left value");
    require_finite_value(g, "difference right value");
    return (f - g) * CartoonAxisPolicy::inverse_x2_away_axis(x);
}

} // namespace CartoonSingularCombinations
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONSINGULARCOMBINATIONS_HPP */
