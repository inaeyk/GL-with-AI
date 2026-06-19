#ifndef BLACKSTRINGTOY_CARTOONPHYSICALRWWRHSCONTRACT_HPP
#define BLACKSTRINGTOY_CARTOONPHYSICALRWWRHSCONTRACT_HPP

#include "CartoonAwayAxisPhysicalRww.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonPhysicalRwwRhsContract
{
namespace PhysicalRww = CartoonAwayAxisPhysicalRww;

// Stage 4AI is a local typed contract for the hidden contribution of the
// reviewed physical lower/lower R_ww[gamma]. It accepts only the Stage 4AH
// result type, so conformal tilde R_ww, R^chi_ww, or a loose raw double cannot
// cross this boundary independently.
static constexpr int hidden_multiplicity = 2;
static_assert(hidden_multiplicity == 2,
              "Stage 4AI requires two hidden SO(3) directions");

static constexpr bool away_axis_only = true;
static constexpr bool finite_axis_implemented = false;
static constexpr bool grid_hxz_parity_validated = false;
static constexpr bool grid_hxx_hww_matching_validated = false;
static constexpr bool grid_Wx_parity_validated = false;
static constexpr bool grid_chix_parity_validated = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class PhysicalRwwRhsContract
{
  public:
    double hidden_conformal_trace() const
    {
        return m_hidden_conformal_trace;
    }
    double hidden_physical_scalar_contribution() const
    {
        return m_hidden_physical_scalar_contribution;
    }

  private:
    PhysicalRwwRhsContract(
        const double hidden_conformal_trace,
        const double hidden_physical_scalar_contribution)
        : m_hidden_conformal_trace(hidden_conformal_trace),
          m_hidden_physical_scalar_contribution(
              hidden_physical_scalar_contribution)
    {
    }

    friend PhysicalRwwRhsContract compute_physical_rww_rhs_contract(
        const PhysicalRww::AwayAxisPhysicalRww &physical_rww);

    double m_hidden_conformal_trace;
    double m_hidden_physical_scalar_contribution;
};

inline PhysicalRwwRhsContract compute_physical_rww_rhs_contract(
    const PhysicalRww::AwayAxisPhysicalRww &physical_rww)
{
    const double hidden_conformal_trace =
        static_cast<double>(hidden_multiplicity) *
        physical_rww.conformal_hww_inverse() * physical_rww.physical_rww();
    const double hidden_physical_scalar_contribution =
        physical_rww.chi() * hidden_conformal_trace;

    if (!std::isfinite(hidden_conformal_trace) ||
        !std::isfinite(hidden_physical_scalar_contribution))
    {
        throw std::domain_error(
            "CartoonPhysicalRwwRhsContract produced a nonfinite local "
            "contribution");
    }

    return PhysicalRwwRhsContract(hidden_conformal_trace,
                                  hidden_physical_scalar_contribution);
}

} // namespace CartoonPhysicalRwwRhsContract
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONPHYSICALRWWRHSCONTRACT_HPP */
