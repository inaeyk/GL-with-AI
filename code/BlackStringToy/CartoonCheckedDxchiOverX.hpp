#ifndef BLACKSTRINGTOY_CARTOONCHECKEDDXCHIOVERX_HPP
#define BLACKSTRINGTOY_CARTOONCHECKEDDXCHIOVERX_HPP

#include "CartoonAxisPolicy.hpp"
#include "CartoonSingularCombinations.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonCheckedDxchiOverX
{
// Stage 4AE local checked quotient only. This verifies that chi_x / x is
// finite at the supplied away-axis point and packages it for the
// conformal-factor hidden Ricci correction. It does not prove global parity,
// finite-axis regularity, or the analytic statement chi_x = O(x) for
// numerical grid data. A true grid-level regularity validation remains future
// work.
static constexpr bool small_axis_regularization_implemented = false;
static constexpr bool proves_global_grid_regularity = false;
static constexpr bool physical_ricci_implemented = false;
static constexpr bool split_vs_direct_identity_gate_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

struct LocalInputs
{
    double x;
    double d_x_chi;
};

class CheckedDxchiOverXIngredient
{
  public:
    double dxchi_over_x() const { return m_dxchi_over_x; }

  private:
    explicit CheckedDxchiOverXIngredient(const double dxchi_over_x)
        : m_dxchi_over_x(dxchi_over_x)
    {
    }

    friend CheckedDxchiOverXIngredient
    compute_checked_dxchi_over_x_ingredient(const LocalInputs &inputs);

    double m_dxchi_over_x;
};

inline double checked_dxchi_over_x_quotient(const double x,
                                            const double d_x_chi)
{
    CartoonSingularCombinations::require_finite_value(d_x_chi, "d_x chi");

    const double quotient =
        d_x_chi * CartoonAxisPolicy::inverse_x_away_axis(x);
    if (!std::isfinite(quotient))
    {
        throw std::domain_error(
            "CartoonCheckedDxchiOverX requires finite d_x chi / x");
    }
    return quotient;
}

inline CheckedDxchiOverXIngredient
compute_checked_dxchi_over_x_ingredient(const LocalInputs &inputs)
{
    return CheckedDxchiOverXIngredient(
        checked_dxchi_over_x_quotient(inputs.x, inputs.d_x_chi));
}

} // namespace CartoonCheckedDxchiOverX
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCHECKEDDXCHIOVERX_HPP */
