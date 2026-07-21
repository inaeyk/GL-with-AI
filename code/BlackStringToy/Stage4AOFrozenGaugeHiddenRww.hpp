#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_HIDDEN_RWW_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_HIDDEN_RWW_HPP

#include "CartoonRicciInterface.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeHiddenRww
{
// Stage 4AO-C validation-only raw hidden physical Ricci block:
// delta R_ww[gamma] on the locked frozen-GP background. This helper itself
// does not compute visible Ricci or perform the separately implemented A,
// Theta, and K Ricci insertions. It is not a complete operator or production
// wiring.
static constexpr bool validation_only = true;
static constexpr bool hidden_physical_delta_rww_implemented = true;
static constexpr bool visible_ricci_implemented = false;
static constexpr bool trace_free_a_curvature_source_implemented = false;
static constexpr bool theta_ricci_scalar_implemented = false;
static constexpr bool full_frozen_gauge_operator_implemented = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_rhs_wiring_implemented = false;

class HiddenRwwPerturbation
{
  public:
    double x() const { return m_x; }
    const ConformalCartoonAlgebra::ConformalMetric &
    delta_h() const
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
    HiddenRwwPerturbation(
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

    friend HiddenRwwPerturbation make_hidden_rww_perturbation(
        double x,
        const ConformalCartoonAlgebra::ConformalMetric &delta_h,
        const CartoonRicci::MetricDerivatives &delta_h_derivatives,
        double delta_chi,
        const CartoonRicci::ScalarDerivatives &delta_chi_derivatives);

    double m_x;
    ConformalCartoonAlgebra::ConformalMetric m_delta_h;
    CartoonRicci::MetricDerivatives m_delta_h_derivatives;
    double m_delta_chi;
    CartoonRicci::ScalarDerivatives m_delta_chi_derivatives;
};

class HiddenPhysicalDeltaRww
{
  public:
    double conformal_part() const { return m_conformal_part; }
    double conformal_factor_part() const { return m_conformal_factor_part; }
    double delta_rww() const { return m_delta_rww; }

  private:
    HiddenPhysicalDeltaRww(const double conformal_part,
                           const double conformal_factor_part,
                           const double delta_rww)
        : m_conformal_part(conformal_part),
          m_conformal_factor_part(conformal_factor_part),
          m_delta_rww(delta_rww)
    {
    }

    friend HiddenPhysicalDeltaRww
    compute_hidden_physical_delta_rww(const HiddenRwwPerturbation &input);

    double m_conformal_part;
    double m_conformal_factor_part;
    double m_delta_rww;
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
                "Stage 4AO-C hidden delta Rww metric data must be finite");
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
                "Stage 4AO-C hidden delta Rww scalar derivative data must be finite");
        }
    }
}

inline HiddenRwwPerturbation make_hidden_rww_perturbation(
    const double x,
    const ConformalCartoonAlgebra::ConformalMetric &delta_h,
    const CartoonRicci::MetricDerivatives &delta_h_derivatives,
    const double delta_chi,
    const CartoonRicci::ScalarDerivatives &delta_chi_derivatives)
{
    if (!std::isfinite(x) || !(x > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C hidden delta Rww is away-axis only and requires x>0");
    }
    require_finite_metric(delta_h);
    require_finite_metric_derivatives(delta_h_derivatives);
    if (!std::isfinite(delta_chi))
    {
        throw std::domain_error(
            "Stage 4AO-C hidden delta Rww chi perturbation must be finite");
    }
    require_finite_scalar_derivatives(delta_chi_derivatives);

    return HiddenRwwPerturbation(x, delta_h, delta_h_derivatives, delta_chi,
                                 delta_chi_derivatives);
}

inline HiddenPhysicalDeltaRww
compute_hidden_physical_delta_rww(const HiddenRwwPerturbation &input)
{
    const double x = input.x();
    const double x2 = x * x;
    const auto &h = input.delta_h();
    const auto &dh = input.delta_h_derivatives();
    const auto &dchi = input.delta_chi_derivatives();

    // Linearized Stage 4AC conformal hidden Ricci around the flat GP spatial
    // background. The terms expose the checked-singular structures from the
    // nonlinear reviewed path: (h_xx-h_ww)/x^2, h_xz/x, and W_x/x.
    const double conformal_part =
        (h.xx - h.ww) / x2 +
        (dh.dx.xx + 2.0 * dh.dz.xz - dh.dx.zz) / (2.0 * x) -
        2.0 * dh.dx.ww / x - 0.5 * (dh.dxx.ww + dh.dzz.ww);

    // Linearized Stage 4AE conformal-factor correction around chi=1.
    const double conformal_factor_part =
        2.0 * dchi.first.dx / x +
        0.5 * (dchi.second.dxx + dchi.second.dzz);

    const double delta_rww = conformal_part + conformal_factor_part;
    if (!std::isfinite(delta_rww))
    {
        throw std::domain_error(
            "Stage 4AO-C hidden delta Rww produced a nonfinite result");
    }

    return HiddenPhysicalDeltaRww(conformal_part, conformal_factor_part,
                                  delta_rww);
}

} // namespace Stage4AOFrozenGaugeHiddenRww
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_HIDDEN_RWW_HPP */
