#ifndef BLACKSTRINGTOY_CARTOONRHSSOURCEBLOCK_HPP
#define BLACKSTRINGTOY_CARTOONRHSSOURCEBLOCK_HPP

#include "CartoonRegularityGuardedSources.hpp"
#include "CartoonRhsContract.hpp"

namespace BlackStringToy
{
namespace CartoonRhsSourceBlock
{
// Stage 4K local RHS source-block skeleton plus Stage 4L's first local formula
// block. This is the future container shape for repo-owned modified-cartoon
// RHS source terms. It is not called from BlackStringToyLevel, does not read
// grid data, and does not implement the full physical RHS.
//
// The source block consumes the Stage 4J contract type. Raw
// CartoonRicci::RicciComponents is intentionally not accepted here.
// `source_terms_implemented` remains false because the live CCZ4/cartoon RHS is
// still not implemented; Stage 4L only adds the local Ricci trace-free
// projection helper below.
static constexpr bool source_terms_implemented = false;
static constexpr bool evolution_wiring_implemented = false;
static constexpr bool trace_free_ricci_projection_implemented = true;
static constexpr bool regularity_guarded_geometry_source_block_available = true;

struct TraceFreeRicci
{
    double xx;
    double xz;
    double zz;
    double ww;
    double trace;
};

struct LocalSourceBlockInput
{
    CartoonRhsContract::RhsLocalInputs rhs_contract;
    ConformalCartoonAlgebra::ConformalMetric h_LL;
};

struct LocalSourceBlockOutput
{
    // Named future RHS pieces. Stage 4K's explicit inert test path sets these
    // to zero only to verify the container and contract boundary. These zeros
    // are not physical source terms.
    double rhs_chi;
    double rhs_hww;
    double rhs_Aww;
    double rhs_K;

    CartoonRhsContract::RhsRicciContractions ricci_contractions;
    bool inert_test_output;
};

struct LocalGuardedGeometrySourceBlockInput
{
    CartoonRegularityGuardedSources::LocalInputs guarded_geometry;
};

class LocalGuardedGeometrySourceBlockOutput
{
  public:
    const CartoonRegularityGuardedSources::RegularityGuardedGeometrySources &
    regularity_guarded_geometry() const
    {
        return m_regularity_guarded_geometry;
    }

  private:
    // Stage 4S carries the Stage 4R guarded package through the local
    // source-block layer. The regularity-sensitive metric-difference value is
    // source-facing only because Stage 4R has already called the Stage 4Q
    // matching guard. This is still not a full Ricci or CCZ4 RHS formula.
    explicit LocalGuardedGeometrySourceBlockOutput(
        const CartoonRegularityGuardedSources::RegularityGuardedGeometrySources
            &regularity_guarded_geometry)
        : m_regularity_guarded_geometry(regularity_guarded_geometry)
    {
    }

    friend LocalGuardedGeometrySourceBlockOutput
    compute_local_guarded_geometry_source_block(
        const LocalGuardedGeometrySourceBlockInput &input);

    CartoonRegularityGuardedSources::RegularityGuardedGeometrySources
        m_regularity_guarded_geometry;
};

inline LocalSourceBlockOutput make_empty_source_block_for_contract_test(
    const LocalSourceBlockInput &input)
{
    const auto contract_outputs =
        CartoonRhsContract::make_inert_rhs_contract_outputs(
            input.rhs_contract);

    return {0.0, 0.0, 0.0, 0.0,
            contract_outputs.ricci_contractions, true};
}

inline LocalGuardedGeometrySourceBlockOutput
compute_local_guarded_geometry_source_block(
    const LocalGuardedGeometrySourceBlockInput &input)
{
    return LocalGuardedGeometrySourceBlockOutput(
        CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(
                input.guarded_geometry));
}

inline TraceFreeRicci compute_ricci_trace_free_source_block(
    const LocalSourceBlockInput &input)
{
    CartoonRhsContract::make_inert_rhs_contract_outputs(input.rhs_contract);
    const auto &ricci_input = input.rhs_contract.ricci;
    const auto &ricci = ricci_input.ricci_LL;
    const auto &h_UU = ricci_input.h_UU;
    const auto &h_LL = input.h_LL;
    ConformalCartoonAlgebra::require_inverse_metric_consistency(h_LL, h_UU);

    // Local trace-free Ricci projection only:
    // R_ij^TF = R_ij - (1 / GR_SPACEDIM) h_ij h^{kl} R_kl.
    // The /4 denominator comes from the shared Stage 4A convention constant.
    // This is not the full CCZ4 RHS.
    const ConformalCartoonAlgebra::SymmetricTensor ricci_tensor{
        ricci.xx, ricci.xz, ricci.zz, ricci.ww};
    const auto trace_free =
        ConformalCartoonAlgebra::trace_free_part(ricci_tensor, h_LL, h_UU);

    return {trace_free.xx, trace_free.xz, trace_free.zz, trace_free.ww,
            ConformalCartoonAlgebra::trace(trace_free, h_UU)};
}

} // namespace CartoonRhsSourceBlock
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONRHSSOURCEBLOCK_HPP */
