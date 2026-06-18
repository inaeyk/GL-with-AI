#ifndef BLACKSTRINGTOY_CARTOONGUARDEDSOURCECONSUMERS_HPP
#define BLACKSTRINGTOY_CARTOONGUARDEDSOURCECONSUMERS_HPP

#include "CartoonRegularityChecks.hpp"
#include "CartoonRegularityGuardedSources.hpp"

namespace BlackStringToy
{
namespace CartoonGuardedSourceConsumers
{
// Stage 4T diagnostic/probe consumer only. This is the first local consumer of
// the checked Stage 4R/4S guarded geometry package. It is not a Ricci tensor,
// not a CCZ4 RHS source formula, and not evolution wiring.
static constexpr bool full_ricci_or_rhs_formula_implemented = false;
static constexpr bool evolution_wiring_implemented = false;
static constexpr bool diagnostic_probe_only = true;

class GuardedGeometryConsumerProbe
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
    bool matching_residual_within_stage4q_tolerance() const
    {
        return m_matching_residual_within_stage4q_tolerance;
    }

  private:
    GuardedGeometryConsumerProbe(
        const double dx_hww_over_x,
        const double hxx_minus_hww_over_x2,
        const double hxx_hww_matching_residual,
        const bool matching_residual_within_stage4q_tolerance)
        : m_dx_hww_over_x(dx_hww_over_x),
          m_hxx_minus_hww_over_x2(hxx_minus_hww_over_x2),
          m_hxx_hww_matching_residual(hxx_hww_matching_residual),
          m_matching_residual_within_stage4q_tolerance(
              matching_residual_within_stage4q_tolerance)
    {
    }

    friend GuardedGeometryConsumerProbe
    compute_guarded_geometry_consumer_probe(
        const CartoonRegularityGuardedSources::
            RegularityGuardedGeometrySources &checked_geometry);

    double m_dx_hww_over_x;
    double m_hxx_minus_hww_over_x2;
    double m_hxx_hww_matching_residual;
    bool m_matching_residual_within_stage4q_tolerance;
};

inline GuardedGeometryConsumerProbe compute_guarded_geometry_consumer_probe(
    const CartoonRegularityGuardedSources::RegularityGuardedGeometrySources
        &checked_geometry)
{
    const double residual = checked_geometry.hxx_hww_matching_residual();
    return GuardedGeometryConsumerProbe(
        checked_geometry.dx_hww_over_x(),
        checked_geometry.hxx_minus_hww_over_x2(), residual,
        residual <= CartoonRegularityChecks::hxx_hww_matching_tolerance);
}

} // namespace CartoonGuardedSourceConsumers
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONGUARDEDSOURCECONSUMERS_HPP */
