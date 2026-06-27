#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_VISIBLE_RXX_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_VISIBLE_RXX_HPP

#include "CartoonRicciInterface.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeVisibleRxx
{
// Stage 4AO-C validation-only raw visible physical Ricci block:
// delta R_xx[gamma] on the locked frozen-GP background. This is not
// trace/trace-free Ricci assembly, not the trace-free A curvature source, not
// the Theta Ricci scalar, not a complete frozen-gauge operator, and not
// production wiring.
static constexpr bool validation_only = true;
static constexpr bool visible_physical_delta_rxx_implemented = true;
static constexpr bool trace_free_ricci_assembly_implemented = false;
static constexpr bool trace_free_a_curvature_source_implemented = false;
static constexpr bool theta_ricci_scalar_implemented = false;
static constexpr bool full_frozen_gauge_operator_implemented = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_rhs_wiring_implemented = false;

class VisibleRxxPerturbation
{
  public:
    double x() const { return m_x; }
    const ConformalCartoonAlgebra::ConformalMetric &delta_h() const
    {
        return m_delta_h;
    }
    const CartoonRicci::MetricDerivatives &delta_h_derivatives() const
    {
        return m_delta_h_derivatives;
    }
    double delta_chi() const { return m_delta_chi; }
    const CartoonRicci::ScalarDerivatives &delta_chi_derivatives() const
    {
        return m_delta_chi_derivatives;
    }

  private:
    VisibleRxxPerturbation(
        const double x,
        const ConformalCartoonAlgebra::ConformalMetric &delta_h,
        const CartoonRicci::MetricDerivatives &delta_h_derivatives,
        const double delta_chi,
        const CartoonRicci::ScalarDerivatives &delta_chi_derivatives)
        : m_x(x), m_delta_h(delta_h),
          m_delta_h_derivatives(delta_h_derivatives), m_delta_chi(delta_chi),
          m_delta_chi_derivatives(delta_chi_derivatives)
    {
    }

    friend VisibleRxxPerturbation make_visible_rxx_perturbation(
        double x, const ConformalCartoonAlgebra::ConformalMetric &delta_h,
        const CartoonRicci::MetricDerivatives &delta_h_derivatives,
        double delta_chi,
        const CartoonRicci::ScalarDerivatives &delta_chi_derivatives);

    double m_x;
    ConformalCartoonAlgebra::ConformalMetric m_delta_h;
    CartoonRicci::MetricDerivatives m_delta_h_derivatives;
    double m_delta_chi;
    CartoonRicci::ScalarDerivatives m_delta_chi_derivatives;
};

class VisiblePhysicalDeltaRxx
{
  public:
    double metric_part() const { return m_metric_part; }
    double conformal_factor_part() const { return m_conformal_factor_part; }
    double delta_rxx() const { return m_delta_rxx; }

  private:
    VisiblePhysicalDeltaRxx(const double metric_part,
                            const double conformal_factor_part,
                            const double delta_rxx)
        : m_metric_part(metric_part),
          m_conformal_factor_part(conformal_factor_part),
          m_delta_rxx(delta_rxx)
    {
    }

    friend VisiblePhysicalDeltaRxx
    compute_visible_physical_delta_rxx(const VisibleRxxPerturbation &input);

    double m_metric_part;
    double m_conformal_factor_part;
    double m_delta_rxx;
};

inline void require_finite_metric(
    const ConformalCartoonAlgebra::ConformalMetric &metric)
{
    const double values[] = {metric.xx, metric.xz, metric.zz, metric.ww};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C visible delta Rxx metric data must be finite");
        }
    }
}

inline void require_finite_metric_derivatives(
    const CartoonRicci::MetricDerivatives &derivatives)
{
    require_finite_metric(derivatives.dx);
    require_finite_metric(derivatives.dz);
    require_finite_metric(derivatives.dxx);
    require_finite_metric(derivatives.dxz);
    require_finite_metric(derivatives.dzz);
}

inline void require_finite_scalar_derivatives(
    const CartoonRicci::ScalarDerivatives &derivatives)
{
    const double values[] = {
        derivatives.first.dx,   derivatives.first.dz,
        derivatives.second.dxx, derivatives.second.dxz,
        derivatives.second.dzz};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C visible delta Rxx scalar derivative data must be finite");
        }
    }
}

inline VisibleRxxPerturbation make_visible_rxx_perturbation(
    const double x, const ConformalCartoonAlgebra::ConformalMetric &delta_h,
    const CartoonRicci::MetricDerivatives &delta_h_derivatives,
    const double delta_chi,
    const CartoonRicci::ScalarDerivatives &delta_chi_derivatives)
{
    if (!std::isfinite(x) || !(x > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C visible delta Rxx is away-axis only and requires x>0");
    }
    require_finite_metric(delta_h);
    require_finite_metric_derivatives(delta_h_derivatives);
    if (!std::isfinite(delta_chi))
    {
        throw std::domain_error(
            "Stage 4AO-C visible delta Rxx chi perturbation must be finite");
    }
    require_finite_scalar_derivatives(delta_chi_derivatives);

    return VisibleRxxPerturbation(x, delta_h, delta_h_derivatives, delta_chi,
                                  delta_chi_derivatives);
}

inline VisiblePhysicalDeltaRxx
compute_visible_physical_delta_rxx(const VisibleRxxPerturbation &input)
{
    const double x = input.x();
    const auto &dh = input.delta_h_derivatives();
    const auto &dchi = input.delta_chi_derivatives();

    // Linearized raw visible R_xx[gamma] around the locked flat GP spatial
    // background. Stage 4G uses gamma=h/chi and returns lower/lower physical
    // Ricci, so the conformal-factor contribution below has positive signs.
    // This raw component is not trace-free projected.
    const double metric_part =
        dh.dxz.xz - 0.5 * dh.dxx.zz - 0.5 * dh.dzz.xx +
        dh.dx.xx / x - 2.0 * dh.dx.ww / x - dh.dxx.ww;

    const double conformal_factor_part =
        dchi.first.dx / x + 1.5 * dchi.second.dxx +
        0.5 * dchi.second.dzz;
    const double delta_rxx = metric_part + conformal_factor_part;
    if (!std::isfinite(delta_rxx))
    {
        throw std::domain_error(
            "Stage 4AO-C visible delta Rxx produced a nonfinite result");
    }

    return VisiblePhysicalDeltaRxx(metric_part, conformal_factor_part,
                                   delta_rxx);
}

} // namespace Stage4AOFrozenGaugeVisibleRxx
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_VISIBLE_RXX_HPP */
