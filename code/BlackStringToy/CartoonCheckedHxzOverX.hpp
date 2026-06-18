#ifndef BLACKSTRINGTOY_CARTOONCHECKEDHXZOVERX_HPP
#define BLACKSTRINGTOY_CARTOONCHECKEDHXZOVERX_HPP

#include "CartoonAxisPolicy.hpp"
#include "CartoonSingularCombinations.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonCheckedHxzOverX
{
// Stage 4X local checked quotient only. This verifies that h_xz / x is finite
// at the supplied away-axis point and packages it for future source formulas.
// It does not prove the global parity condition h_xz(-x,z) = -h_xz(x,z), nor
// does it prove the analytic statement h_xz = O(x) for a numerical grid
// function. A true parity validation would require a two-sided parity check,
// Taylor/coefficient check, or grid-level near-axis policy. That is not
// implemented in Stage 4X.
static constexpr bool small_axis_regularization_implemented = false;
static constexpr bool proves_global_grid_parity = false;
static constexpr bool conformal_hidden_ricci_implemented = false;
static constexpr bool conformal_factor_ricci_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

struct LocalInputs
{
    double x;
    double h_xz;
};

class CheckedHxzOverXIngredient
{
  public:
    double hxz_over_x() const { return m_hxz_over_x; }

  private:
    explicit CheckedHxzOverXIngredient(const double hxz_over_x)
        : m_hxz_over_x(hxz_over_x)
    {
    }

    friend CheckedHxzOverXIngredient
    compute_checked_hxz_over_x_ingredient(const LocalInputs &inputs);

    double m_hxz_over_x;
};

inline double checked_hxz_over_x_quotient(const double x, const double h_xz)
{
    CartoonSingularCombinations::require_finite_value(h_xz, "h_xz");

    const double quotient =
        h_xz * CartoonAxisPolicy::inverse_x_away_axis(x);
    if (!std::isfinite(quotient))
    {
        throw std::domain_error(
            "CartoonCheckedHxzOverX requires finite h_xz / x");
    }
    return quotient;
}

inline void require_finite_hxz_over_x(const double x, const double h_xz)
{
    // Regularity allows h_xz = x q(x^2,z) with finite nonzero q. This local
    // check only verifies that the supplied away-axis quotient is finite; it
    // does not reject every parity violation.
    (void)checked_hxz_over_x_quotient(x, h_xz);
}

inline CheckedHxzOverXIngredient
compute_checked_hxz_over_x_ingredient(const LocalInputs &inputs)
{
    return CheckedHxzOverXIngredient(
        checked_hxz_over_x_quotient(inputs.x, inputs.h_xz));
}

} // namespace CartoonCheckedHxzOverX
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCHECKEDHXZOVERX_HPP */
