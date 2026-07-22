#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_Z_DERIVATIVE_ADAPTER_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_Z_DERIVATIVE_ADAPTER_HPP

#include "CartoonRicciInterface.hpp"
#include "Stage4AOFrozenGaugeContractedConnection.hpp"
#include "Stage4AOFrozenGaugeZRicciCompletion.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeZDerivativeAdapter
{
namespace Connection = Stage4AOFrozenGaugeContractedConnection;
namespace Completion = Stage4AOFrozenGaugeZRicciCompletion;
namespace Ricci = CartoonRicci;

static constexpr bool validation_only = true;
static constexpr bool modified_cartoon_derivative_adapter_implemented = true;
static constexpr bool determinant_constraint_assumed = false;
static constexpr int hidden_multiplicity = 2;
static constexpr bool numerical_production_differentiation_implemented = false;
static constexpr bool complete_non_gamma_rows_implemented = false;
static constexpr bool full_operator_implemented = false;
static constexpr bool eigensolver_implemented = false;

class DerivativeAdapterInput
{
  public:
    double x() const { return m_x; }
    const ConformalCartoonAlgebra::ConformalMetric &h() const { return m_h; }
    const Ricci::MetricDerivatives &derivatives() const
    {
        return m_derivatives;
    }
    double hat_gamma_x() const { return m_hat_gamma_x; }
    double hat_gamma_z() const { return m_hat_gamma_z; }
    double dx_hat_gamma_x() const { return m_dx_hat_gamma_x; }
    double dz_hat_gamma_x() const { return m_dz_hat_gamma_x; }
    double dx_hat_gamma_z() const { return m_dx_hat_gamma_z; }
    double dz_hat_gamma_z() const { return m_dz_hat_gamma_z; }

  private:
    DerivativeAdapterInput(
        const double a_x,
        const ConformalCartoonAlgebra::ConformalMetric &a_h,
        const Ricci::MetricDerivatives &a_derivatives,
        const double a_hat_gamma_x, const double a_hat_gamma_z,
        const double a_dx_hat_gamma_x, const double a_dz_hat_gamma_x,
        const double a_dx_hat_gamma_z, const double a_dz_hat_gamma_z)
        : m_x(a_x), m_h(a_h), m_derivatives(a_derivatives),
          m_hat_gamma_x(a_hat_gamma_x), m_hat_gamma_z(a_hat_gamma_z),
          m_dx_hat_gamma_x(a_dx_hat_gamma_x),
          m_dz_hat_gamma_x(a_dz_hat_gamma_x),
          m_dx_hat_gamma_z(a_dx_hat_gamma_z),
          m_dz_hat_gamma_z(a_dz_hat_gamma_z)
    {
    }

    double m_x;
    ConformalCartoonAlgebra::ConformalMetric m_h;
    Ricci::MetricDerivatives m_derivatives;
    double m_hat_gamma_x;
    double m_hat_gamma_z;
    double m_dx_hat_gamma_x;
    double m_dz_hat_gamma_x;
    double m_dx_hat_gamma_z;
    double m_dz_hat_gamma_z;

    friend DerivativeAdapterInput make_derivative_adapter_input(
        double, const ConformalCartoonAlgebra::ConformalMetric &,
        const Ricci::MetricDerivatives &, double, double, double, double,
        double, double);
};

inline DerivativeAdapterInput make_derivative_adapter_input(
    const double x, const ConformalCartoonAlgebra::ConformalMetric &h,
    const Ricci::MetricDerivatives &derivatives, const double hat_gamma_x,
    const double hat_gamma_z, const double dx_hat_gamma_x,
    const double dz_hat_gamma_x, const double dx_hat_gamma_z,
    const double dz_hat_gamma_z)
{
    const ConformalCartoonAlgebra::ConformalMetric metric_values[] = {
        h, derivatives.dx, derivatives.dz, derivatives.dxx, derivatives.dxz,
        derivatives.dzz};
    for (const auto &metric : metric_values)
    {
        const double values[] = {metric.xx, metric.xz, metric.zz, metric.ww};
        for (const double value : values)
        {
            if (!std::isfinite(value))
            {
                throw std::domain_error(
                    "Stage 4AO-C Z derivative adapter requires finite metric jets");
            }
        }
    }
    const double gamma_values[] = {
        x, hat_gamma_x, hat_gamma_z, dx_hat_gamma_x, dz_hat_gamma_x,
        dx_hat_gamma_z, dz_hat_gamma_z};
    for (const double value : gamma_values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C Z derivative adapter requires finite Gamma jets");
        }
    }
    if (!(x > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C Z derivative adapter requires x>0");
    }
    return DerivativeAdapterInput(
        x, h, derivatives, hat_gamma_x, hat_gamma_z, dx_hat_gamma_x,
        dz_hat_gamma_x, dx_hat_gamma_z, dz_hat_gamma_z);
}

class DerivativeAdapterResult
{
  public:
    double g_x() const { return m_g_x; }
    double g_z() const { return m_g_z; }
    double z_x() const { return m_z_x; }
    double z_z() const { return m_z_z; }
    double dx_g_x() const { return m_dx_g_x; }
    double dz_g_x() const { return m_dz_g_x; }
    double dx_g_z() const { return m_dx_g_z; }
    double dz_g_z() const { return m_dz_g_z; }
    double dx_z_x() const { return m_dx_z_x; }
    double dz_z_x() const { return m_dz_z_x; }
    double dx_z_z() const { return m_dx_z_z; }
    double dz_z_z() const { return m_dz_z_z; }

  private:
    DerivativeAdapterResult(
        const double a_g_x, const double a_g_z, const double a_z_x,
        const double a_z_z, const double a_dx_g_x, const double a_dz_g_x,
        const double a_dx_g_z, const double a_dz_g_z,
        const double a_dx_z_x, const double a_dz_z_x,
        const double a_dx_z_z, const double a_dz_z_z)
        : m_g_x(a_g_x), m_g_z(a_g_z), m_z_x(a_z_x), m_z_z(a_z_z),
          m_dx_g_x(a_dx_g_x), m_dz_g_x(a_dz_g_x),
          m_dx_g_z(a_dx_g_z), m_dz_g_z(a_dz_g_z),
          m_dx_z_x(a_dx_z_x), m_dz_z_x(a_dz_z_x),
          m_dx_z_z(a_dx_z_z), m_dz_z_z(a_dz_z_z)
    {
    }

    double m_g_x;
    double m_g_z;
    double m_z_x;
    double m_z_z;
    double m_dx_g_x;
    double m_dz_g_x;
    double m_dx_g_z;
    double m_dz_g_z;
    double m_dx_z_x;
    double m_dz_z_x;
    double m_dx_z_z;
    double m_dz_z_z;

    friend DerivativeAdapterResult
    compute_derivative_adapter(const DerivativeAdapterInput &);
};

inline DerivativeAdapterResult
compute_derivative_adapter(const DerivativeAdapterInput &input)
{
    const auto &h = input.h();
    const auto &d = input.derivatives();
    const auto connection_input = Connection::make_perturbation_jet(
        input.x(), h.xx, h.xz, h.zz, h.ww, d.dx.xx, d.dx.xz, d.dx.zz,
        d.dx.ww, d.dz.xx, d.dz.xz, d.dz.zz, d.dz.ww,
        input.hat_gamma_x(), input.hat_gamma_z());
    const auto connection =
        Connection::compute_contracted_connection_and_z(connection_input);

    const double x = input.x();
    const double x_squared = x * x;
    const double hidden = static_cast<double>(hidden_multiplicity);

    const double dx_g_x =
        0.5 * d.dxx.xx - 0.5 * d.dxx.zz - d.dxx.ww + d.dxz.xz +
        hidden * ((d.dx.xx - d.dx.ww) / x -
                  (h.xx - h.ww) / x_squared);
    const double dz_g_x =
        0.5 * d.dxz.xx - 0.5 * d.dxz.zz - d.dxz.ww + d.dzz.xz +
        hidden * (d.dz.xx - d.dz.ww) / x;
    const double dx_g_z =
        d.dxx.xz +
        hidden * (d.dx.xz / x - h.xz / x_squared) -
        0.5 * d.dxz.xx + 0.5 * d.dxz.zz - d.dxz.ww;
    const double dz_g_z =
        d.dxz.xz + hidden * d.dz.xz / x - 0.5 * d.dzz.xx +
        0.5 * d.dzz.zz - d.dzz.ww;

    const double dx_z_x = 0.5 * (input.dx_hat_gamma_x() - dx_g_x);
    const double dz_z_x = 0.5 * (input.dz_hat_gamma_x() - dz_g_x);
    const double dx_z_z = 0.5 * (input.dx_hat_gamma_z() - dx_g_z);
    const double dz_z_z = 0.5 * (input.dz_hat_gamma_z() - dz_g_z);

    const double values[] = {
        connection.g_x(), connection.g_z(), connection.z_x(),
        connection.z_z(), dx_g_x, dz_g_x, dx_g_z, dz_g_z, dx_z_x,
        dz_z_x, dx_z_z, dz_z_z};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::overflow_error(
                "Stage 4AO-C Z derivative adapter produced non-finite output");
        }
    }

    return DerivativeAdapterResult(
        connection.g_x(), connection.g_z(), connection.z_x(),
        connection.z_z(), dx_g_x, dz_g_x, dx_g_z, dz_g_z, dx_z_x,
        dz_z_x, dx_z_z, dz_z_z);
}

inline Completion::CompletionTensor make_encoded_z_ricci_completion(
    const double x, const DerivativeAdapterResult &adapter)
{
    return Completion::compute(Completion::make_encoded_z_derivative_jet(
        x, adapter.z_x(), adapter.z_z(), adapter.dx_z_x(),
        adapter.dz_z_x(), adapter.dx_z_z(), adapter.dz_z_z()));
}
} // namespace Stage4AOFrozenGaugeZDerivativeAdapter
} // namespace BlackStringToy

#endif
