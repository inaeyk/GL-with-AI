#ifndef BLACKSTRINGTOY_CARTOONCHECKEDDXHWWOVERX_HPP
#define BLACKSTRINGTOY_CARTOONCHECKEDDXHWWOVERX_HPP

#include "CartoonAxisPolicy.hpp"
#include "CartoonSingularCombinations.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonCheckedDxhwwOverX
{
// Stage 4Z local checked quotient only. This verifies that W_x / x, with
// W = h_ww, is finite at the supplied away-axis point and packages it for
// future conformal hidden Ricci formulas. It does not prove global parity,
// finite-axis regularity, or the analytic statement W_x = O(x) for numerical
// grid data. A true grid-level regularity validation remains future work.
static constexpr bool small_axis_regularization_implemented = false;
static constexpr bool proves_global_grid_regularity = false;
static constexpr bool conformal_hidden_ricci_implemented = false;
static constexpr bool conformal_factor_ricci_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

struct LocalInputs
{
    double x;
    double d_x_hww;
};

class CheckedDxhwwOverXIngredient
{
  public:
    double dxhww_over_x() const { return m_dxhww_over_x; }

  private:
    explicit CheckedDxhwwOverXIngredient(const double dxhww_over_x)
        : m_dxhww_over_x(dxhww_over_x)
    {
    }

    friend CheckedDxhwwOverXIngredient
    compute_checked_dxhww_over_x_ingredient(const LocalInputs &inputs);

    double m_dxhww_over_x;
};

inline double checked_dxhww_over_x_quotient(const double x,
                                            const double d_x_hww)
{
    CartoonSingularCombinations::require_finite_value(d_x_hww, "d_x hww");

    const double quotient =
        d_x_hww * CartoonAxisPolicy::inverse_x_away_axis(x);
    if (!std::isfinite(quotient))
    {
        throw std::domain_error(
            "CartoonCheckedDxhwwOverX requires finite d_x hww / x");
    }
    return quotient;
}

inline CheckedDxhwwOverXIngredient
compute_checked_dxhww_over_x_ingredient(const LocalInputs &inputs)
{
    return CheckedDxhwwOverXIngredient(
        checked_dxhww_over_x_quotient(inputs.x, inputs.d_x_hww));
}

} // namespace CartoonCheckedDxhwwOverX
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCHECKEDDXHWWOVERX_HPP */
