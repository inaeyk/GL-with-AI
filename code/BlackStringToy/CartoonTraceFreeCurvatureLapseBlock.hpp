#ifndef BLACKSTRINGTOY_CARTOONTRACEFREECURVATURELAPSEBLOCK_HPP
#define BLACKSTRINGTOY_CARTOONTRACEFREECURVATURELAPSEBLOCK_HPP

#include "CartoonAwwCurvatureLapseCore.hpp"
#include "CartoonAwayAxisPhysicalRww.hpp"
#include "CartoonAxisPolicy.hpp"
#include "CartoonRicciBridge.hpp"
#include "CartoonRicciInterface.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonTraceFreeCurvatureLapseBlock
{
namespace AwwCore = CartoonAwwCurvatureLapseCore;
namespace PhysicalRww = CartoonAwayAxisPhysicalRww;
namespace Ricci = CartoonRicci;
namespace RicciBridge = CartoonRicciBridge;

// Stage 4AL implements only the local away-axis trace-free curvature/lapse
// geometry source chi [C_ij - (1/4) h_ij h^kl C_kl], where
// C_ij = -D_iD_j alpha + alpha R_ij[gamma]. Z4 terms, nonlinear A/K terms,
// full CCZ4 RHS, grid reads, and evolution wiring are not implemented here.
static constexpr bool away_axis_only = true;
static constexpr bool z4_terms_implemented = false;
static constexpr bool nonlinear_terms_implemented = false;
static constexpr bool full_ccz4_rhs_implemented = false;
static constexpr bool grid_wiring_implemented = false;
static constexpr bool evolution_wiring_implemented = false;
static constexpr double rww_agreement_tolerance = 1.0e-10;

struct ScalarJet
{
    double value;
    double dx;
    double dz;
    double dxx;
    double dxz;
    double dzz;
};

class TraceFreeCurvatureLapseBlock
{
  public:
    double core_xx() const { return m_core.xx; }
    double core_xz() const { return m_core.xz; }
    double core_zz() const { return m_core.zz; }
    double core_ww() const { return m_core.ww; }
    double full_trace() const { return m_full_trace; }
    double tf_xx() const { return m_trace_free.xx; }
    double tf_xz() const { return m_trace_free.xz; }
    double tf_zz() const { return m_trace_free.zz; }
    double tf_ww() const { return m_trace_free.ww; }
    double chi_tf_xx() const { return m_chi_trace_free.xx; }
    double chi_tf_xz() const { return m_chi_trace_free.xz; }
    double chi_tf_zz() const { return m_chi_trace_free.zz; }
    double chi_tf_ww() const { return m_chi_trace_free.ww; }

  private:
    TraceFreeCurvatureLapseBlock(
        const ConformalCartoonAlgebra::SymmetricTensor &core,
        const double full_trace,
        const ConformalCartoonAlgebra::SymmetricTensor &trace_free,
        const ConformalCartoonAlgebra::SymmetricTensor &chi_trace_free)
        : m_core(core), m_full_trace(full_trace), m_trace_free(trace_free),
          m_chi_trace_free(chi_trace_free)
    {
    }

    friend TraceFreeCurvatureLapseBlock
    compute_trace_free_curvature_lapse_block(
        const class TraceFreeCurvatureLapseBlockInputs &inputs);

    ConformalCartoonAlgebra::SymmetricTensor m_core;
    double m_full_trace;
    ConformalCartoonAlgebra::SymmetricTensor m_trace_free;
    ConformalCartoonAlgebra::SymmetricTensor m_chi_trace_free;
};

class TraceFreeCurvatureLapseBlockInputs
{
  public:
    const Ricci::CartoonRicciInputs &ricci_inputs() const
    {
        return m_ricci_inputs;
    }
    const PhysicalRww::AwayAxisPhysicalRwwInputs &physical_rww_inputs() const
    {
        return m_physical_rww_inputs;
    }
    const AwwCore::AwwCurvatureLapseCoreInputs &aww_core_inputs() const
    {
        return m_aww_core_inputs;
    }
    const ConformalCartoonAlgebra::ConformalMetric &h_LL() const
    {
        return m_h_LL;
    }
    const ConformalCartoonAlgebra::InverseConformalMetric &h_UU() const
    {
        return m_h_UU;
    }
    double chi() const { return m_chi; }
    const ScalarJet &alpha() const { return m_alpha; }

  private:
    TraceFreeCurvatureLapseBlockInputs(
        const Ricci::CartoonRicciInputs &ricci_inputs,
        const PhysicalRww::AwayAxisPhysicalRwwInputs &physical_rww_inputs,
        const AwwCore::AwwCurvatureLapseCoreInputs &aww_core_inputs,
        const ConformalCartoonAlgebra::ConformalMetric &h_LL,
        const ConformalCartoonAlgebra::InverseConformalMetric &h_UU,
        const double chi, const ScalarJet &alpha)
        : m_ricci_inputs(ricci_inputs), m_physical_rww_inputs(physical_rww_inputs),
          m_aww_core_inputs(aww_core_inputs), m_h_LL(h_LL), m_h_UU(h_UU),
          m_chi(chi), m_alpha(alpha)
    {
    }

    friend TraceFreeCurvatureLapseBlockInputs
    make_trace_free_curvature_lapse_block_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double chi, double alpha, double d_x_h_xx, double d_z_h_xx,
        double d_x_h_xz, double d_z_h_xz, double d_x_h_zz,
        double d_z_h_zz, double d_x_h_ww, double d_z_h_ww,
        double d_xx_h_xx, double d_xx_h_xz, double d_xx_h_zz,
        double d_xx_h_ww, double d_xz_h_xx, double d_xz_h_xz,
        double d_xz_h_zz, double d_xz_h_ww, double d_zz_h_xx,
        double d_zz_h_xz, double d_zz_h_zz, double d_zz_h_ww,
        double d_x_chi, double d_z_chi, double d_xx_chi,
        double d_xz_chi, double d_zz_chi, double d_x_alpha,
        double d_z_alpha, double d_xx_alpha, double d_xz_alpha,
        double d_zz_alpha);
    friend TraceFreeCurvatureLapseBlock
    compute_trace_free_curvature_lapse_block(
        const TraceFreeCurvatureLapseBlockInputs &inputs);

    Ricci::CartoonRicciInputs m_ricci_inputs;
    PhysicalRww::AwayAxisPhysicalRwwInputs m_physical_rww_inputs;
    AwwCore::AwwCurvatureLapseCoreInputs m_aww_core_inputs;
    ConformalCartoonAlgebra::ConformalMetric m_h_LL;
    ConformalCartoonAlgebra::InverseConformalMetric m_h_UU;
    double m_chi;
    ScalarJet m_alpha;
};

inline void require_finite_trace_free_block_inputs(
    const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double chi, const double alpha,
    const Ricci::MetricDerivatives &h_derivatives,
    const Ricci::ScalarDerivatives &chi_derivatives, const ScalarJet &alpha_jet)
{
    const double values[] = {
        h_xx,
        h_xz,
        h_zz,
        h_ww,
        chi,
        alpha,
        h_derivatives.dx.xx,
        h_derivatives.dx.xz,
        h_derivatives.dx.zz,
        h_derivatives.dx.ww,
        h_derivatives.dz.xx,
        h_derivatives.dz.xz,
        h_derivatives.dz.zz,
        h_derivatives.dz.ww,
        h_derivatives.dxx.xx,
        h_derivatives.dxx.xz,
        h_derivatives.dxx.zz,
        h_derivatives.dxx.ww,
        h_derivatives.dxz.xx,
        h_derivatives.dxz.xz,
        h_derivatives.dxz.zz,
        h_derivatives.dxz.ww,
        h_derivatives.dzz.xx,
        h_derivatives.dzz.xz,
        h_derivatives.dzz.zz,
        h_derivatives.dzz.ww,
        chi_derivatives.first.dx,
        chi_derivatives.first.dz,
        chi_derivatives.second.dxx,
        chi_derivatives.second.dxz,
        chi_derivatives.second.dzz,
        alpha_jet.dx,
        alpha_jet.dz,
        alpha_jet.dxx,
        alpha_jet.dxz,
        alpha_jet.dzz};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "CartoonTraceFreeCurvatureLapseBlock requires finite local "
                "metric, conformal-factor, and lapse jet inputs");
        }
    }
}

inline double checked_positive_reduced_determinant(
    const ConformalCartoonAlgebra::ConformalMetric &h_LL)
{
    const double determinant =
        ConformalCartoonAlgebra::reduced_determinant(h_LL);
    if (!std::isfinite(determinant) ||
        determinant <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonTraceFreeCurvatureLapseBlock requires finite positive "
            "reduced determinant");
    }
    return determinant;
}

inline ConformalCartoonAlgebra::InverseConformalMetric
checked_inverse_metric(const ConformalCartoonAlgebra::ConformalMetric &h_LL)
{
    checked_positive_reduced_determinant(h_LL);
    if (h_LL.ww <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonTraceFreeCurvatureLapseBlock requires positive h_ww");
    }

    const auto h_UU = ConformalCartoonAlgebra::inverse(h_LL);
    ConformalCartoonAlgebra::require_inverse_metric_consistency(h_LL, h_UU);
    return h_UU;
}

inline TraceFreeCurvatureLapseBlockInputs
make_trace_free_curvature_lapse_block_inputs(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double chi, const double alpha,
    const double d_x_h_xx, const double d_z_h_xx, const double d_x_h_xz,
    const double d_z_h_xz, const double d_x_h_zz, const double d_z_h_zz,
    const double d_x_h_ww, const double d_z_h_ww, const double d_xx_h_xx,
    const double d_xx_h_xz, const double d_xx_h_zz,
    const double d_xx_h_ww, const double d_xz_h_xx,
    const double d_xz_h_xz, const double d_xz_h_zz,
    const double d_xz_h_ww, const double d_zz_h_xx,
    const double d_zz_h_xz, const double d_zz_h_zz,
    const double d_zz_h_ww, const double d_x_chi, const double d_z_chi,
    const double d_xx_chi, const double d_xz_chi,
    const double d_zz_chi, const double d_x_alpha,
    const double d_z_alpha, const double d_xx_alpha,
    const double d_xz_alpha, const double d_zz_alpha)
{
    // The Ricci, hidden-lapse, inverse-metric, trace, and trace-free data are
    // all derived from this one local jet. No public API accepts loose Ricci,
    // Hessian, inverse-metric, or trace inputs.
    CartoonAxisPolicy::require_away_axis(x);
    const ConformalCartoonAlgebra::ConformalMetric h_LL{h_xx, h_xz, h_zz,
                                                        h_ww};
    const Ricci::MetricDerivatives h_derivatives{
        {d_x_h_xx, d_x_h_xz, d_x_h_zz, d_x_h_ww},
        {d_z_h_xx, d_z_h_xz, d_z_h_zz, d_z_h_ww},
        {d_xx_h_xx, d_xx_h_xz, d_xx_h_zz, d_xx_h_ww},
        {d_xz_h_xx, d_xz_h_xz, d_xz_h_zz, d_xz_h_ww},
        {d_zz_h_xx, d_zz_h_xz, d_zz_h_zz, d_zz_h_ww}};
    const Ricci::ScalarDerivatives chi_derivatives{
        {d_x_chi, d_z_chi}, {d_xx_chi, d_xz_chi, d_zz_chi}};
    const ScalarJet alpha_jet{alpha,       d_x_alpha,  d_z_alpha,
                              d_xx_alpha, d_xz_alpha, d_zz_alpha};

    require_finite_trace_free_block_inputs(
        h_xx, h_xz, h_zz, h_ww, chi, alpha, h_derivatives, chi_derivatives,
        alpha_jet);
    if (chi <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonTraceFreeCurvatureLapseBlock requires positive chi");
    }

    const auto h_UU = checked_inverse_metric(h_LL);
    const Ricci::CartoonRicciInputs ricci_inputs{x, chi, h_LL,
                                                 chi_derivatives,
                                                 h_derivatives};
    const auto physical_rww_inputs =
        PhysicalRww::make_away_axis_physical_rww_inputs(
            x, h_xx, h_xz, h_zz, h_ww, chi, d_x_h_xx, d_z_h_xx,
            d_x_h_xz, d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww,
            d_z_h_ww, d_xx_h_ww, d_xz_h_ww, d_zz_h_ww, d_x_chi,
            d_z_chi, d_xx_chi, d_xz_chi, d_zz_chi);
    const auto aww_core_inputs = AwwCore::make_aww_curvature_lapse_core_inputs(
        x, h_xx, h_xz, h_zz, h_ww, chi, alpha, d_x_h_xx, d_z_h_xx,
        d_x_h_xz, d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww, d_z_h_ww,
        d_xx_h_ww, d_xz_h_ww, d_zz_h_ww, d_x_chi, d_z_chi, d_xx_chi,
        d_xz_chi, d_zz_chi, d_x_alpha, d_z_alpha);

    return TraceFreeCurvatureLapseBlockInputs(
        ricci_inputs, physical_rww_inputs, aww_core_inputs, h_LL, h_UU, chi,
        alpha_jet);
}

inline double gamma_x_xx(const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto &h = inputs.h_LL();
    const auto &dh = inputs.ricci_inputs().h_derivatives;
    const double D = ConformalCartoonAlgebra::reduced_determinant(h);
    return (h.zz * dh.dx.xx - h.xz * (2.0 * dh.dx.xz - dh.dz.xx)) /
           (2.0 * D);
}

inline double gamma_z_xx(const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto &h = inputs.h_LL();
    const auto &dh = inputs.ricci_inputs().h_derivatives;
    const double D = ConformalCartoonAlgebra::reduced_determinant(h);
    return (-h.xz * dh.dx.xx + h.xx * (2.0 * dh.dx.xz - dh.dz.xx)) /
           (2.0 * D);
}

inline double gamma_x_xz(const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto &h = inputs.h_LL();
    const auto &dh = inputs.ricci_inputs().h_derivatives;
    const double D = ConformalCartoonAlgebra::reduced_determinant(h);
    return (h.zz * dh.dz.xx - h.xz * dh.dx.zz) / (2.0 * D);
}

inline double gamma_z_xz(const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto &h = inputs.h_LL();
    const auto &dh = inputs.ricci_inputs().h_derivatives;
    const double D = ConformalCartoonAlgebra::reduced_determinant(h);
    return (-h.xz * dh.dz.xx + h.xx * dh.dx.zz) / (2.0 * D);
}

inline double gamma_x_zz(const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto &h = inputs.h_LL();
    const auto &dh = inputs.ricci_inputs().h_derivatives;
    const double D = ConformalCartoonAlgebra::reduced_determinant(h);
    return (h.zz * (2.0 * dh.dz.xz - dh.dx.zz) - h.xz * dh.dz.zz) /
           (2.0 * D);
}

inline double gamma_z_zz(const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto &h = inputs.h_LL();
    const auto &dh = inputs.ricci_inputs().h_derivatives;
    const double D = ConformalCartoonAlgebra::reduced_determinant(h);
    return (-h.xz * (2.0 * dh.dz.xz - dh.dx.zz) + h.xx * dh.dz.zz) /
           (2.0 * D);
}

inline ConformalCartoonAlgebra::SymmetricTensor
physical_visible_lapse_hessian(
    const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto &alpha = inputs.alpha();
    const auto &chi = inputs.ricci_inputs().chi_derivatives.first;
    const auto &h = inputs.h_LL();
    const auto &h_UU = inputs.h_UU();

    const double tilde_xx =
        alpha.dxx - gamma_x_xx(inputs) * alpha.dx -
        gamma_z_xx(inputs) * alpha.dz;
    const double tilde_xz =
        alpha.dxz - gamma_x_xz(inputs) * alpha.dx -
        gamma_z_xz(inputs) * alpha.dz;
    const double tilde_zz =
        alpha.dzz - gamma_x_zz(inputs) * alpha.dx -
        gamma_z_zz(inputs) * alpha.dz;

    const double chi_alpha_contract =
        h_UU.xx * chi.dx * alpha.dx +
        h_UU.xz * (chi.dx * alpha.dz + chi.dz * alpha.dx) +
        h_UU.zz * chi.dz * alpha.dz;
    const double conformal_factor = 1.0 / (2.0 * inputs.chi());

    return {tilde_xx +
                conformal_factor *
                    (2.0 * chi.dx * alpha.dx -
                     h.xx * chi_alpha_contract),
            tilde_xz +
                conformal_factor *
                    (chi.dx * alpha.dz + chi.dz * alpha.dx -
                     h.xz * chi_alpha_contract),
            tilde_zz +
                conformal_factor *
                    (2.0 * chi.dz * alpha.dz -
                     h.zz * chi_alpha_contract),
            0.0};
}

inline void require_stage4g_stage4ah_rww_agreement(
    const RicciBridge::RhsRicciComponents &stage4g_ricci,
    const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const double stage4ah_rww =
        PhysicalRww::compute_away_axis_physical_rww(
            inputs.physical_rww_inputs())
            .physical_rww();
    const double residual = std::abs(stage4g_ricci.ww - stage4ah_rww);
    if (residual > rww_agreement_tolerance)
    {
        throw std::domain_error(
            "CartoonTraceFreeCurvatureLapseBlock requires Stage 4G and Stage "
            "4AH physical R_ww to agree before trace-free assembly");
    }
}

inline void require_finite_result(
    const ConformalCartoonAlgebra::SymmetricTensor &tensor,
    const char *description)
{
    const double values[] = {tensor.xx, tensor.xz, tensor.zz, tensor.ww};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(description);
        }
    }
}

inline TraceFreeCurvatureLapseBlock
compute_trace_free_curvature_lapse_block(
    const TraceFreeCurvatureLapseBlockInputs &inputs)
{
    const auto ricci = RicciBridge::to_rhs_ricci_components(
        Ricci::compute_metric_derivative_ricci(inputs.m_ricci_inputs));
    require_stage4g_stage4ah_rww_agreement(ricci, inputs);

    const auto visible_hessian = physical_visible_lapse_hessian(inputs);
    const auto aww_core =
        AwwCore::compute_aww_curvature_lapse_core(inputs.m_aww_core_inputs);

    const ConformalCartoonAlgebra::SymmetricTensor core{
        -visible_hessian.xx + inputs.m_alpha.value * ricci.xx,
        -visible_hessian.xz + inputs.m_alpha.value * ricci.xz,
        -visible_hessian.zz + inputs.m_alpha.value * ricci.zz,
        aww_core.curvature_lapse_core()};
    require_finite_result(
        core,
        "CartoonTraceFreeCurvatureLapseBlock produced nonfinite core values");

    const double full_trace = ConformalCartoonAlgebra::trace(core, inputs.m_h_UU);
    if (!std::isfinite(full_trace))
    {
        throw std::domain_error(
            "CartoonTraceFreeCurvatureLapseBlock produced nonfinite trace");
    }

    const auto trace_free = ConformalCartoonAlgebra::trace_free_part(
        core, inputs.m_h_LL, inputs.m_h_UU);
    require_finite_result(
        trace_free,
        "CartoonTraceFreeCurvatureLapseBlock produced nonfinite trace-free "
        "values");

    const ConformalCartoonAlgebra::SymmetricTensor chi_trace_free{
        inputs.m_chi * trace_free.xx, inputs.m_chi * trace_free.xz,
        inputs.m_chi * trace_free.zz, inputs.m_chi * trace_free.ww};
    require_finite_result(
        chi_trace_free,
        "CartoonTraceFreeCurvatureLapseBlock produced nonfinite source values");

    return TraceFreeCurvatureLapseBlock(core, full_trace, trace_free,
                                        chi_trace_free);
}

} // namespace CartoonTraceFreeCurvatureLapseBlock
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONTRACEFREECURVATURELAPSEBLOCK_HPP */
