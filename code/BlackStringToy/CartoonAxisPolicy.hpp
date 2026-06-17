#ifndef BLACKSTRINGTOY_CARTOONAXISPOLICY_HPP
#define BLACKSTRINGTOY_CARTOONAXISPOLICY_HPP

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonAxisPolicy
{
// Stage 4M/4O policy only. Local cartoon/RHS expressions with explicit x
// factors must cross this guard before evaluating away-axis-only 1/x or 1/x^2
// terms. True Stage 3I small-axis regularization is not implemented here.
enum class AxisMode
{
    AwayAxisOnly
};

static constexpr AxisMode implemented_axis_mode = AxisMode::AwayAxisOnly;
static constexpr bool regularized_axis_implemented = false;

inline bool is_away_axis(const double x)
{
    return std::isfinite(x) && x > 0.0;
}

inline void require_away_axis(const double x)
{
    if (!is_away_axis(x))
    {
        throw std::domain_error(
            "CartoonAxisPolicy AwayAxisOnly requires finite x > 0; "
            "Stage 3I small-axis regularization is not implemented");
    }
}

inline double inverse_x_away_axis(const double x)
{
    require_away_axis(x);
    return 1.0 / x;
}

inline double inverse_x2_away_axis(const double x)
{
    require_away_axis(x);

    // Away-axis this is mathematically (1/x)^2. Keep this as a separately
    // guarded primitive: if a later stage adds clamped or regularized
    // behavior, it must define the 1/x^2 semantics independently rather than
    // inheriting a clamped 1/x by squaring it.
    return 1.0 / (x * x);
}

} // namespace CartoonAxisPolicy
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONAXISPOLICY_HPP */
