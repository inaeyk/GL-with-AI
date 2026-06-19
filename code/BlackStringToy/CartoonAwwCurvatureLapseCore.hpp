#ifndef BLACKSTRINGTOY_CARTOONAWWCURVATURELAPSECORE_HPP
#define BLACKSTRINGTOY_CARTOONAWWCURVATURELAPSECORE_HPP

#include "CartoonAwayAxisPhysicalRww.hpp"
#include "CartoonPhysicalHiddenLapseHessian.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonAwwCurvatureLapseCore
{
namespace Lapse = CartoonPhysicalHiddenLapseHessian;
namespace PhysicalRww = CartoonAwayAxisPhysicalRww;

// Stage 4AK implements only the local away-axis geometric core
// C_ww = -D_wD_w alpha + alpha R_ww[gamma].
// The Z4 term, full 4D trace-free projection, outer chi factor, nonlinear
// A/K terms, live RHS, finite-axis support, grid reads, and evolution wiring
// are not part of this type.
static constexpr bool away_axis_only = true;
static constexpr bool z4_vector_term_implemented = false;
static constexpr bool trace_free_projection_implemented = false;
static constexpr bool chi_prefactor_implemented = false;
static constexpr bool nonlinear_AK_terms_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class AwwCurvatureLapseCore
{
  public:
    double minus_dww_alpha() const { return m_minus_dww_alpha; }
    double alpha_rww() const { return m_alpha_rww; }
    double curvature_lapse_core() const { return m_curvature_lapse_core; }

  private:
    AwwCurvatureLapseCore(const double minus_dww_alpha,
                          const double alpha_rww,
                          const double curvature_lapse_core)
        : m_minus_dww_alpha(minus_dww_alpha), m_alpha_rww(alpha_rww),
          m_curvature_lapse_core(curvature_lapse_core)
    {
    }

    friend AwwCurvatureLapseCore compute_aww_curvature_lapse_core(
        const class AwwCurvatureLapseCoreInputs &inputs);

    double m_minus_dww_alpha;
    double m_alpha_rww;
    double m_curvature_lapse_core;
};

class AwwCurvatureLapseCoreInputs
{
  private:
    AwwCurvatureLapseCoreInputs(
        const PhysicalRww::AwayAxisPhysicalRwwInputs &physical_rww_inputs,
        const Lapse::PhysicalHiddenLapseHessianInputs &lapse_hessian_inputs,
        const double alpha)
        : m_physical_rww_inputs(physical_rww_inputs),
          m_lapse_hessian_inputs(lapse_hessian_inputs), m_alpha(alpha)
    {
    }

    friend AwwCurvatureLapseCoreInputs make_aww_curvature_lapse_core_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double chi, double alpha, double d_x_h_xx, double d_z_h_xx,
        double d_x_h_xz, double d_z_h_xz, double d_x_h_zz,
        double d_z_h_zz, double d_x_h_ww, double d_z_h_ww,
        double d_xx_h_ww, double d_xz_h_ww, double d_zz_h_ww,
        double d_x_chi, double d_z_chi, double d_xx_chi,
        double d_xz_chi, double d_zz_chi, double d_x_alpha,
        double d_z_alpha);
    friend AwwCurvatureLapseCore compute_aww_curvature_lapse_core(
        const AwwCurvatureLapseCoreInputs &inputs);

    PhysicalRww::AwayAxisPhysicalRwwInputs m_physical_rww_inputs;
    Lapse::PhysicalHiddenLapseHessianInputs m_lapse_hessian_inputs;
    double m_alpha;
};

inline AwwCurvatureLapseCoreInputs make_aww_curvature_lapse_core_inputs(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double chi, const double alpha,
    const double d_x_h_xx, const double d_z_h_xx, const double d_x_h_xz,
    const double d_z_h_xz, const double d_x_h_zz, const double d_z_h_zz,
    const double d_x_h_ww, const double d_z_h_ww, const double d_xx_h_ww,
    const double d_xz_h_ww, const double d_zz_h_ww, const double d_x_chi,
    const double d_z_chi, const double d_xx_chi, const double d_xz_chi,
    const double d_zz_chi, const double d_x_alpha,
    const double d_z_alpha)
{
    // The physical-Ricci and lapse-Hessian packages are minted from this same
    // local point. No public overload accepts separately created R_ww,
    // D_wD_w alpha, determinant, or checked quotient packages.
    if (!std::isfinite(alpha))
    {
        throw std::domain_error(
            "CartoonAwwCurvatureLapseCore requires finite lapse alpha");
    }

    const auto physical_rww_inputs =
        PhysicalRww::make_away_axis_physical_rww_inputs(
            x, h_xx, h_xz, h_zz, h_ww, chi, d_x_h_xx, d_z_h_xx,
            d_x_h_xz, d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww,
            d_z_h_ww, d_xx_h_ww, d_xz_h_ww, d_zz_h_ww, d_x_chi,
            d_z_chi, d_xx_chi, d_xz_chi, d_zz_chi);
    const auto lapse_hessian_inputs =
        Lapse::make_physical_hidden_lapse_hessian_inputs(
            x, h_xx, h_xz, h_zz, h_ww, chi, d_x_h_ww, d_z_h_ww,
            d_x_chi, d_z_chi, d_x_alpha, d_z_alpha);

    return AwwCurvatureLapseCoreInputs(physical_rww_inputs,
                                       lapse_hessian_inputs, alpha);
}

inline AwwCurvatureLapseCore compute_aww_curvature_lapse_core(
    const AwwCurvatureLapseCoreInputs &inputs)
{
    const double dww_alpha =
        Lapse::compute_physical_hidden_lapse_hessian(
            inputs.m_lapse_hessian_inputs)
            .dww_alpha();
    const double physical_rww =
        PhysicalRww::compute_away_axis_physical_rww(
            inputs.m_physical_rww_inputs)
            .physical_rww();

    const double minus_dww_alpha = -dww_alpha;
    const double alpha_rww = inputs.m_alpha * physical_rww;
    const double curvature_lapse_core = minus_dww_alpha + alpha_rww;
    if (!std::isfinite(curvature_lapse_core))
    {
        throw std::domain_error(
            "CartoonAwwCurvatureLapseCore produced a nonfinite local result");
    }

    return AwwCurvatureLapseCore(minus_dww_alpha, alpha_rww,
                                 curvature_lapse_core);
}

} // namespace CartoonAwwCurvatureLapseCore
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONAWWCURVATURELAPSECORE_HPP */
