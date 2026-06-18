#ifndef BLACKSTRINGTOY_CARTOONSOURCEFORMULAAUTHORINGGATE_HPP
#define BLACKSTRINGTOY_CARTOONSOURCEFORMULAAUTHORINGGATE_HPP

#include "CartoonRegularityGuardedSources.hpp"
#include "CartoonRhsSourceBlock.hpp"

namespace BlackStringToy
{
namespace CartoonSourceFormulaAuthoringGate
{
// Stage 4U source-formula authoring gate only. Future source formulas that
// need the regularity-sensitive (h_xx - h_ww) / x^2 ingredient must accept this
// type, or the checked RegularityGuardedGeometrySources package directly.
//
// Do not route that metric-matching-sensitive ingredient through raw
// difference_over_x2(h_xx, h_ww, x) in source formulas. The generic helper
// remains useful for generic away-axis combinations, but the source-facing
// metric-difference path must cross the Stage 4Q/4R guard.
static constexpr bool full_ricci_or_rhs_formula_implemented = false;
static constexpr bool evolution_wiring_implemented = false;
static constexpr bool grid_reads_implemented = false;

class RegularitySensitiveSourceInputs
{
  public:
    const CartoonRegularityGuardedSources::RegularityGuardedGeometrySources &
    checked_geometry() const
    {
        return m_checked_geometry;
    }

  private:
    explicit RegularitySensitiveSourceInputs(
        const CartoonRegularityGuardedSources::RegularityGuardedGeometrySources
            &checked_geometry)
        : m_checked_geometry(checked_geometry)
    {
    }

    friend RegularitySensitiveSourceInputs
    make_regularity_sensitive_source_inputs(
        const CartoonRegularityGuardedSources::
            RegularityGuardedGeometrySources &checked_geometry);

    CartoonRegularityGuardedSources::RegularityGuardedGeometrySources
        m_checked_geometry;
};

inline RegularitySensitiveSourceInputs make_regularity_sensitive_source_inputs(
    const CartoonRegularityGuardedSources::RegularityGuardedGeometrySources
        &checked_geometry)
{
    return RegularitySensitiveSourceInputs(checked_geometry);
}

inline RegularitySensitiveSourceInputs
make_regularity_sensitive_source_inputs(
    const CartoonRhsSourceBlock::LocalGuardedGeometrySourceBlockOutput
        &source_block_output)
{
    return make_regularity_sensitive_source_inputs(
        source_block_output.regularity_guarded_geometry());
}

} // namespace CartoonSourceFormulaAuthoringGate
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONSOURCEFORMULAAUTHORINGGATE_HPP */
