#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_VISIBLE_RXZ_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_VISIBLE_RXZ_HPP

#include "CartoonRicciInterface.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeVisibleRxz
{
// Stage 4AO-C validation-only raw visible physical Ricci block:
// delta R_xz[gamma] on the locked frozen-GP background. This is the first
// one-z Ricci target. It is not delta R_xx, not delta R_zz, not the trace-free
// A curvature source, not the Theta Ricci scalar, not a complete frozen-gauge
// operator, and not production wiring.
static constexpr bool validation_only = true;
static constexpr bool visible_physical_delta_rxz_implemented = true;
static constexpr bool visible_delta_rxx_implemented = false;
static constexpr bool visible_delta_rzz_implemented = false;
static constexpr bool trace_free_a_curvature_source_implemented = false;
static constexpr bool theta_ricci_scalar_implemented = false;
static constexpr bool full_frozen_gauge_operator_implemented = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_rhs_wiring_implemented = false;

class VisibleRxzPerturbation
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
    VisibleRxzPerturbation(
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

    friend VisibleRxzPerturbation make_visible_rxz_perturbation(
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

class VisiblePhysicalDeltaRxz
{
  public:
    double metric_part() const { return m_metric_part; }
    double conformal_factor_part() const { return m_conformal_factor_part; }
    double delta_rxz() const { return m_delta_rxz; }

  private:
    VisiblePhysicalDeltaRxz(const double metric_part,
                            const double conformal_factor_part,
                            const double delta_rxz)
        : m_metric_part(metric_part),
          m_conformal_factor_part(conformal_factor_part),
          m_delta_rxz(delta_rxz)
    {
    }

    friend VisiblePhysicalDeltaRxz
    compute_visible_physical_delta_rxz(const VisibleRxzPerturbation &input);

    double m_metric_part;
    double m_conformal_factor_part;
    double m_delta_rxz;
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
                "Stage 4AO-C visible delta Rxz metric data must be finite");
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
                "Stage 4AO-C visible delta Rxz scalar derivative data must be finite");
        }
    }
}

inline VisibleRxzPerturbation make_visible_rxz_perturbation(
    const double x, const ConformalCartoonAlgebra::ConformalMetric &delta_h,
    const CartoonRicci::MetricDerivatives &delta_h_derivatives,
    const double delta_chi,
    const CartoonRicci::ScalarDerivatives &delta_chi_derivatives)
{
    if (!std::isfinite(x) || !(x > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C visible delta Rxz is away-axis only and requires x>0");
    }
    require_finite_metric(delta_h);
    require_finite_metric_derivatives(delta_h_derivatives);
    if (!std::isfinite(delta_chi))
    {
        throw std::domain_error(
            "Stage 4AO-C visible delta Rxz chi perturbation must be finite");
    }
    require_finite_scalar_derivatives(delta_chi_derivatives);

    return VisibleRxzPerturbation(x, delta_h, delta_h_derivatives, delta_chi,
                                  delta_chi_derivatives);
}

inline VisiblePhysicalDeltaRxz
compute_visible_physical_delta_rxz(const VisibleRxzPerturbation &input)
{
    const double x = input.x();
    const auto &dh = input.delta_h_derivatives();
    const auto &dchi = input.delta_chi_derivatives();

    // Linearized raw visible R_xz[gamma] around the locked flat GP spatial
    // background. The pure delta h_xz term cancels in this raw component at
    // this background; pure h_xz remains a required zero oracle because the
    // future assembled operator must still respect the one-z parity sector.
    const double metric_part =
        (dh.dz.xx - dh.dz.ww) / x - dh.dxz.ww;

    const double conformal_factor_part = dchi.second.dxz;
    const double delta_rxz = metric_part + conformal_factor_part;
    if (!std::isfinite(delta_rxz))
    {
        throw std::domain_error(
            "Stage 4AO-C visible delta Rxz produced a nonfinite result");
    }

    return VisiblePhysicalDeltaRxz(metric_part, conformal_factor_part,
                                   delta_rxz);
}

} // namespace Stage4AOFrozenGaugeVisibleRxz
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_VISIBLE_RXZ_HPP */
