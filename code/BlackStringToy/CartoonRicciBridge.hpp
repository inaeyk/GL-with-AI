#ifndef BLACKSTRINGTOY_CARTOONRICCIBRIDGE_HPP
#define BLACKSTRINGTOY_CARTOONRICCIBRIDGE_HPP

#include "CartoonRicciInterface.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonRicciBridge
{
// Stage 4I bridge contract only. This is the named crossing point from the
// Stage 4G metric-derivative cartoon Ricci helper toward any future
// RHS-facing code. It does not wire Ricci into the RHS or evolution.
//
// Convention pinned here:
// - input Ricci type is CartoonRicci::RicciComponents, not a generic tensor;
// - raw cartoon Ricci fields are private to prevent bypassing this bridge;
// - components are physical lower/lower components R_xx, R_xz, R_zz, R_ww;
// - R_ww is the Cartesian-like hidden component, not R_theta theta;
// - signs follow the Stage 4G metric-derivative Ricci helper;
// - full 4D traces include off-diagonal factor 2 and hidden multiplicity 2.
//
// Future RHS code must cross through this bridge or a reviewed replacement.
// Directly treating Stage 4G Ricci components as GRChombo CCZ4Geometry Ricci
// data is not allowed by this contract.
static constexpr int hidden_multiplicity =
    ConformalCartoonAlgebra::hidden_multiplicity;
static_assert(hidden_multiplicity == 2,
              "CartoonRicciBridge expects two hidden SO(3) directions");

static constexpr bool rhs_wiring_implemented = false;

struct RhsRicciComponents
{
    double xx;
    double xz;
    double zz;
    double ww;
};

class RicciAccess
{
  private:
    friend RhsRicciComponents
    to_rhs_ricci_components(const CartoonRicci::RicciComponents &ricci_LL);

    // Internal bridge-only doorway. Keep this returning a typed by-value view.
    // Do not change it to return raw doubles, references, pointers, or mutable
    // aliases to CartoonRicci::RicciComponents storage.
    static RhsRicciComponents
    expose_for_bridge(const CartoonRicci::RicciComponents &ricci_LL)
    {
        return {ricci_LL.m_xx, ricci_LL.m_xz, ricci_LL.m_zz,
                ricci_LL.m_ww};
    }
};

inline RhsRicciComponents
to_rhs_ricci_components(const CartoonRicci::RicciComponents &ricci_LL)
{
    return RicciAccess::expose_for_bridge(ricci_LL);
}

struct RhsRicciBridgeInput
{
    CartoonRicci::RicciComponents physical_LL;
    ConformalCartoonAlgebra::InverseConformalMetric h_UU;
    double chi;
};

struct RhsRicciBridgeContractions
{
    // h^{ij} R_ij, with the full 4D off-diagonal and hidden multiplicities.
    double conformal_inverse_contract;

    // gamma^{ij} R_ij = chi h^{ij} R_ij for physical lower/lower Ricci.
    double physical_scalar;
};

inline double conformal_inverse_contract_4d(
    const CartoonRicci::RicciComponents &ricci_LL,
    const ConformalCartoonAlgebra::InverseConformalMetric &h_UU)
{
    const auto rhs_ricci = to_rhs_ricci_components(ricci_LL);
    return h_UU.xx * rhs_ricci.xx + 2.0 * h_UU.xz * rhs_ricci.xz +
           h_UU.zz * rhs_ricci.zz +
           static_cast<double>(hidden_multiplicity) * h_UU.ww *
               rhs_ricci.ww;
}

inline RhsRicciBridgeContractions
make_rhs_ricci_bridge_contract(const RhsRicciBridgeInput &input)
{
    if (input.chi <= CartoonRicci::ricci_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonRicciBridge requires positive chi for physical scalar");
    }

    const double conformal_contract =
        conformal_inverse_contract_4d(input.physical_LL, input.h_UU);
    return {conformal_contract, input.chi * conformal_contract};
}

} // namespace CartoonRicciBridge
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONRICCIBRIDGE_HPP */
