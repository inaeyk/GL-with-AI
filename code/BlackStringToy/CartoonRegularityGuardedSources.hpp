#ifndef BLACKSTRINGTOY_CARTOONREGULARITYGUARDEDSOURCES_HPP
#define BLACKSTRINGTOY_CARTOONREGULARITYGUARDEDSOURCES_HPP

#include "CartoonGeometryPrimitives.hpp"
#include "CartoonRegularityChecks.hpp"
#include "CartoonSingularCombinations.hpp"

namespace BlackStringToy
{
namespace CartoonRegularityGuardedSources
{
// Stage 4R local source-style sub-block only. This packages away-axis cartoon
// geometry ingredients after the Stage 4Q matching guard has accepted them.
// It is not a full Ricci tensor, not a CCZ4 RHS formula, and not Stage 3I
// small-axis regularization.
static constexpr bool full_ricci_or_rhs_formula_implemented = false;
static constexpr bool small_axis_regularization_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

struct LocalInputs
{
    double x;
    double h_xx;
    double h_ww;
    double d_x_hww;
};

class RegularityGuardedGeometrySources
{
  public:
    double dx_hww_over_x() const { return m_dx_hww_over_x; }
    double hxx_minus_hww_over_x2() const
    {
        return m_hxx_minus_hww_over_x2;
    }
    double hxx_hww_matching_residual() const
    {
        return m_hxx_hww_matching_residual;
    }

  private:
    RegularityGuardedGeometrySources(
        const double dx_hww_over_x,
        const double hxx_minus_hww_over_x2,
        const double hxx_hww_matching_residual)
        : m_dx_hww_over_x(dx_hww_over_x),
          m_hxx_minus_hww_over_x2(hxx_minus_hww_over_x2),
          m_hxx_hww_matching_residual(hxx_hww_matching_residual)
    {
    }

    friend RegularityGuardedGeometrySources
    compute_away_axis_regularly_matched_geometry_sources(
        const LocalInputs &inputs);

    double m_dx_hww_over_x;
    double m_hxx_minus_hww_over_x2;
    double m_hxx_hww_matching_residual;
};

inline RegularityGuardedGeometrySources
compute_away_axis_regularly_matched_geometry_sources(
    const LocalInputs &inputs)
{
    CartoonRegularityChecks::require_hxx_hww_axis_matching(
        inputs.x, inputs.h_xx, inputs.h_ww);

    const auto primitives = CartoonGeometryPrimitives::compute(
        {inputs.x, inputs.d_x_hww});
    const double guarded_hxx_minus_hww_over_x2 =
        CartoonSingularCombinations::difference_over_x2(
            inputs.h_xx, inputs.h_ww, inputs.x);
    const double residual =
        CartoonRegularityChecks::hxx_hww_matching_residual(
            inputs.x, inputs.h_xx, inputs.h_ww);

    return RegularityGuardedGeometrySources(
        primitives.dx_hww_over_x, guarded_hxx_minus_hww_over_x2, residual);
}

} // namespace CartoonRegularityGuardedSources
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONREGULARITYGUARDEDSOURCES_HPP */
