#ifndef BLACKSTRINGTOY_CARTOONRHSCONTRACT_HPP
#define BLACKSTRINGTOY_CARTOONRHSCONTRACT_HPP

#include "CartoonAxisPolicy.hpp"
#include "CartoonRicciBridge.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonRhsContract
{
// Stage 4J local Ricci-to-RHS contract only. This header defines the shape of
// the data a future repo-owned RHS source block may receive from the Stage 4I
// bridge. It does not implement CCZ4 RHS terms, does not touch grid data, and
// is not called from evolution.
//
// Contract:
// - Ricci input must be CartoonRicciBridge::RhsRicciComponents.
// - Raw CartoonRicci::RicciComponents is intentionally not accepted here.
// - Ricci components are physical lower/lower metric-derivative cartoon
//   components R_xx, R_xz, R_zz, R_ww.
// - R_ww is the Cartesian-like hidden component, not R_theta theta.
// - hww is not g_theta theta; the external spherical/cartoon x^2 factor is
//   not stored inside hww.
// - Full 4D traces include the off-diagonal factor 2 and hidden multiplicity 2.
// - Small-axis regularization is still not implemented by this contract.
static constexpr int hidden_multiplicity =
    CartoonRicciBridge::hidden_multiplicity;
static_assert(hidden_multiplicity == 2,
              "CartoonRhsContract expects two hidden SO(3) directions");

static constexpr bool rhs_terms_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

struct RhsRicciInputs
{
    CartoonRicciBridge::RhsRicciComponents ricci_LL;
    ConformalCartoonAlgebra::InverseConformalMetric h_UU;
    double chi;
};

struct RhsLocalInputs
{
    RhsRicciInputs ricci;

    // Reduced coordinate. Future RHS terms involving explicit cartoon factors
    // must apply the Stage 3I small-x rules before using this near the axis.
    double x;
};

struct RhsRicciContractions
{
    // h^{ij} R_ij, with full 4D off-diagonal and hidden multiplicities.
    double conformal_inverse_contract;

    // gamma^{ij} R_ij = chi h^{ij} R_ij for physical lower/lower Ricci.
    double physical_scalar;
};

struct RhsLocalOutputs
{
    // Future RHS blocks may populate named source-block outputs after separate
    // derivation and review. Stage 4J deliberately leaves them inert.
    RhsRicciContractions ricci_contractions;
};

inline RhsRicciContractions
compute_ricci_contractions(const RhsRicciInputs &inputs)
{
    if (inputs.chi <= CartoonRicci::ricci_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonRhsContract requires positive chi for physical scalar");
    }

    const auto &r = inputs.ricci_LL;
    const auto &h = inputs.h_UU;
    const double conformal_contract =
        h.xx * r.xx + 2.0 * h.xz * r.xz + h.zz * r.zz +
        static_cast<double>(hidden_multiplicity) * h.ww * r.ww;

    return {conformal_contract, inputs.chi * conformal_contract};
}

inline RhsLocalOutputs make_inert_rhs_contract_outputs(
    const RhsLocalInputs &inputs)
{
    CartoonAxisPolicy::require_away_axis(inputs.x);

    return {compute_ricci_contractions(inputs.ricci)};
}

} // namespace CartoonRhsContract
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONRHSCONTRACT_HPP */
