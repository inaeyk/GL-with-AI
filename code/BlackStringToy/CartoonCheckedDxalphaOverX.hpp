#ifndef BLACKSTRINGTOY_CARTOONCHECKEDDXALPHAOVERX_HPP
#define BLACKSTRINGTOY_CARTOONCHECKEDDXALPHAOVERX_HPP

#include "CartoonAxisPolicy.hpp"
#include "CartoonSingularCombinations.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonCheckedDxalphaOverX
{
// Stage 4AJ local checked quotient only. This verifies that alpha_x / x is
// finite at the supplied away-axis point and packages it for the physical
// hidden lapse Hessian. It does not prove global parity, finite-axis
// regularity, or the analytic statement alpha_x = O(x) for numerical grid
// data. A true grid-level regularity validation remains future work.
static constexpr bool small_axis_regularization_implemented = false;
static constexpr bool proves_global_grid_regularity = false;
static constexpr bool physical_hidden_lapse_hessian_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

struct LocalInputs
{
    double x;
    double d_x_alpha;
};

class CheckedDxalphaOverXIngredient
{
  public:
    double dxalpha_over_x() const { return m_dxalpha_over_x; }

  private:
    explicit CheckedDxalphaOverXIngredient(const double dxalpha_over_x)
        : m_dxalpha_over_x(dxalpha_over_x)
    {
    }

    friend CheckedDxalphaOverXIngredient
    compute_checked_dxalpha_over_x_ingredient(const LocalInputs &inputs);

    double m_dxalpha_over_x;
};

inline double checked_dxalpha_over_x_quotient(const double x,
                                              const double d_x_alpha)
{
    CartoonSingularCombinations::require_finite_value(d_x_alpha,
                                                      "d_x alpha");

    const double quotient =
        d_x_alpha * CartoonAxisPolicy::inverse_x_away_axis(x);
    if (!std::isfinite(quotient))
    {
        throw std::domain_error(
            "CartoonCheckedDxalphaOverX requires finite d_x alpha / x");
    }
    return quotient;
}

inline CheckedDxalphaOverXIngredient
compute_checked_dxalpha_over_x_ingredient(const LocalInputs &inputs)
{
    return CheckedDxalphaOverXIngredient(
        checked_dxalpha_over_x_quotient(inputs.x, inputs.d_x_alpha));
}

} // namespace CartoonCheckedDxalphaOverX
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCHECKEDDXALPHAOVERX_HPP */
