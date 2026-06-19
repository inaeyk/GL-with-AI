#ifndef BLACKSTRINGTOY_CARTOONAWAYAXISPHYSICALRWW_HPP
#define BLACKSTRINGTOY_CARTOONAWAYAXISPHYSICALRWW_HPP

#include "CartoonConformalFactorRww.hpp"
#include "CartoonConformalRww.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonAwayAxisPhysicalRww
{
// Stage 4AH assembles only a local away-axis physical R_ww[gamma] value:
// R_ww[gamma] = tilde R_ww[h] + R^chi_ww.
// The single-source factory below mints both reviewed input packages from one
// metric/conformal-factor jet. It does not accept independently minted 4AC and
// 4AE packages, which could describe different local points.
static constexpr bool away_axis_only = true;
static constexpr bool finite_axis_implemented = false;
static constexpr bool grid_hxz_parity_validated = false;
static constexpr bool grid_hxx_hww_matching_validated = false;
static constexpr bool grid_Wx_parity_validated = false;
static constexpr bool grid_chix_parity_validated = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class AwayAxisPhysicalRww
{
  public:
    double conformal_part() const { return m_conformal_part; }
    double conformal_factor_part() const { return m_conformal_factor_part; }
    double physical_rww() const { return m_physical_rww; }

  private:
    AwayAxisPhysicalRww(const double conformal_part,
                        const double conformal_factor_part,
                        const double physical_rww)
        : m_conformal_part(conformal_part),
          m_conformal_factor_part(conformal_factor_part),
          m_physical_rww(physical_rww)
    {
    }

    friend AwayAxisPhysicalRww compute_away_axis_physical_rww(
        const class AwayAxisPhysicalRwwInputs &inputs);

    double m_conformal_part;
    double m_conformal_factor_part;
    double m_physical_rww;
};

class AwayAxisPhysicalRwwInputs
{
  private:
    AwayAxisPhysicalRwwInputs(
        const CartoonConformalRww::ConformalRwwInputs &conformal_inputs,
        const CartoonConformalFactorRww::ConformalFactorRwwInputs
            &conformal_factor_inputs)
        : m_conformal_inputs(conformal_inputs),
          m_conformal_factor_inputs(conformal_factor_inputs)
    {
    }

    friend AwayAxisPhysicalRwwInputs make_away_axis_physical_rww_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double chi, double d_x_h_xx, double d_z_h_xx,
        double d_x_h_xz, double d_z_h_xz, double d_x_h_zz,
        double d_z_h_zz, double d_x_h_ww, double d_z_h_ww,
        double d_xx_h_ww, double d_xz_h_ww, double d_zz_h_ww,
        double d_x_chi, double d_z_chi, double d_xx_chi,
        double d_xz_chi, double d_zz_chi);
    friend AwayAxisPhysicalRww compute_away_axis_physical_rww(
        const AwayAxisPhysicalRwwInputs &inputs);

    CartoonConformalRww::ConformalRwwInputs m_conformal_inputs;
    CartoonConformalFactorRww::ConformalFactorRwwInputs
        m_conformal_factor_inputs;
};

inline AwayAxisPhysicalRwwInputs make_away_axis_physical_rww_inputs(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double chi, const double d_x_h_xx,
    const double d_z_h_xx, const double d_x_h_xz,
    const double d_z_h_xz, const double d_x_h_zz,
    const double d_z_h_zz, const double d_x_h_ww,
    const double d_z_h_ww, const double d_xx_h_ww,
    const double d_xz_h_ww, const double d_zz_h_ww,
    const double d_x_chi, const double d_z_chi,
    const double d_xx_chi, const double d_xz_chi,
    const double d_zz_chi)
{
    const auto conformal_inputs = CartoonConformalRww::make_conformal_rww_inputs(
        x, h_xx, h_xz, h_zz, h_ww, d_x_h_xx, d_z_h_xx, d_x_h_xz,
        d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww, d_z_h_ww, d_xx_h_ww,
        d_xz_h_ww, d_zz_h_ww);
    const auto conformal_factor_inputs =
        CartoonConformalFactorRww::make_conformal_factor_rww_inputs(
            x, h_xx, h_xz, h_zz, h_ww, chi, d_x_h_xx, d_z_h_xx,
            d_x_h_xz, d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww,
            d_z_h_ww, d_x_chi, d_z_chi, d_xx_chi, d_xz_chi,
            d_zz_chi);

    return AwayAxisPhysicalRwwInputs(conformal_inputs,
                                     conformal_factor_inputs);
}

inline AwayAxisPhysicalRww compute_away_axis_physical_rww(
    const AwayAxisPhysicalRwwInputs &inputs)
{
    const double conformal_part =
        CartoonConformalRww::compute_conformal_rww(inputs.m_conformal_inputs)
            .value();
    const double conformal_factor_part =
        CartoonConformalFactorRww::compute_conformal_factor_rww(
            inputs.m_conformal_factor_inputs)
            .value();
    const double physical_rww = conformal_part + conformal_factor_part;
    if (!std::isfinite(physical_rww))
    {
        throw std::domain_error(
            "CartoonAwayAxisPhysicalRww produced a nonfinite local result");
    }
    return AwayAxisPhysicalRww(conformal_part, conformal_factor_part,
                               physical_rww);
}

} // namespace CartoonAwayAxisPhysicalRww
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONAWAYAXISPHYSICALRWW_HPP */
