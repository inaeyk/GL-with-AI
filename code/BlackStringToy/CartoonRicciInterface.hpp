#ifndef BLACKSTRINGTOY_CARTOONRICCIINTERFACE_HPP
#define BLACKSTRINGTOY_CARTOONRICCIINTERFACE_HPP

#include "ConformalCartoonAlgebra.hpp"

#include <array>
#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonRicciBridge
{
struct RicciAccess;
}

namespace CartoonRicci
{
// Stage 4F introduced the local-value interface for a future cartoon Ricci
// helper. Stage 4G adds the first away-axis metric-derivative implementation.
//
// This is the metric-derivative cartoon Ricci interface: the future Ricci
// helper is expected to use h, chi, their first derivatives, their second
// derivatives, the reduced coordinate x, and hidden multiplicity. This follows
// the symbolic metric-derivative checks from Stages 3C-3E. It is not the
// Gamma-based GRChombo CCZ4Geometry Ricci form.
//
// Before this helper is wired into a CCZ4 RHS, a later stage must check how
// this metric-derivative Ricci output is consumed by the GRChombo-facing RHS
// code. If that path expects a Gamma-based Ricci form, the mismatch must be
// resolved before wiring. The future implementation must also apply the Stage
// 3I small-x regularity rules; carrying x in this interface does not by itself
// make the x -> 0 behavior safe.
static constexpr int hidden_multiplicity =
    ConformalCartoonAlgebra::hidden_multiplicity;
static_assert(hidden_multiplicity == 2,
              "BlackStringToy cartoon Ricci interface expects two hidden "
              "SO(3) directions");
static constexpr bool ricci_formulas_implemented = true;
static constexpr double ricci_zero_tolerance = 1.0e-14;

struct FirstDerivatives
{
    double dx;
    double dz;
};

struct SecondDerivatives
{
    double dxx;
    double dxz;
    double dzz;
};

struct ScalarDerivatives
{
    FirstDerivatives first;
    SecondDerivatives second;
};

struct MetricDerivatives
{
    ConformalCartoonAlgebra::ConformalMetric dx;
    ConformalCartoonAlgebra::ConformalMetric dz;
    ConformalCartoonAlgebra::ConformalMetric dxx;
    ConformalCartoonAlgebra::ConformalMetric dxz;
    ConformalCartoonAlgebra::ConformalMetric dzz;
};

struct CartoonRicciInputs
{
    // Reduced coordinate radius. The spherical/cartoon reconstruction supplies
    // the external x^2 factor; hww is not g_theta theta.
    double x;
    double chi;
    ConformalCartoonAlgebra::ConformalMetric h;
    ScalarDerivatives chi_derivatives;
    MetricDerivatives h_derivatives;
};

class RicciComponents
{
    friend struct CartoonRicciBridge::RicciAccess;

  public:
    RicciComponents() = default;

    RicciComponents(const double a_xx, const double a_xz, const double a_zz,
                    const double a_ww)
        : m_xx(a_xx), m_xz(a_xz), m_zz(a_zz), m_ww(a_ww)
    {
    }

  private:
    // Raw metric-derivative Ricci components are intentionally not public.
    // Future RHS-facing use must cross the Stage 4I bridge so the lower/lower
    // component, hidden-multiplicity, and trace conventions are explicit.
    double m_xx = 0.0;
    double m_xz = 0.0;
    double m_zz = 0.0;
    double m_ww = 0.0;
};

namespace detail
{
using Matrix4 = std::array<std::array<double, 4>, 4>;
using MetricDeriv1 = std::array<Matrix4, 4>;
using MetricDeriv2 = std::array<std::array<Matrix4, 4>, 4>;
using Gamma = std::array<std::array<std::array<double, 4>, 4>, 4>;
using GammaDeriv =
    std::array<std::array<std::array<std::array<double, 4>, 4>, 4>, 4>;

inline void set_symmetric(Matrix4 &matrix, const int i, const int j,
                          const double value)
{
    matrix[i][j] = value;
    matrix[j][i] = value;
}

inline ConformalCartoonAlgebra::SymmetricTensor physical_from_conformal(
    const ConformalCartoonAlgebra::ConformalMetric &h, const double chi)
{
    return {h.xx / chi, h.xz / chi, h.zz / chi, h.ww / chi};
}

inline ConformalCartoonAlgebra::SymmetricTensor first_physical_derivative(
    const ConformalCartoonAlgebra::ConformalMetric &h,
    const ConformalCartoonAlgebra::ConformalMetric &dh, const double chi,
    const double dchi)
{
    const double inv_chi = 1.0 / chi;
    const double inv_chi2 = inv_chi * inv_chi;
    return {dh.xx * inv_chi - h.xx * dchi * inv_chi2,
            dh.xz * inv_chi - h.xz * dchi * inv_chi2,
            dh.zz * inv_chi - h.zz * dchi * inv_chi2,
            dh.ww * inv_chi - h.ww * dchi * inv_chi2};
}

inline ConformalCartoonAlgebra::SymmetricTensor second_physical_derivative(
    const ConformalCartoonAlgebra::ConformalMetric &h,
    const ConformalCartoonAlgebra::ConformalMetric &dh_a,
    const ConformalCartoonAlgebra::ConformalMetric &dh_b,
    const ConformalCartoonAlgebra::ConformalMetric &d2h_ab, const double chi,
    const double dchi_a, const double dchi_b, const double d2chi_ab)
{
    const double inv_chi = 1.0 / chi;
    const double inv_chi2 = inv_chi * inv_chi;
    const double inv_chi3 = inv_chi2 * inv_chi;

    return {d2h_ab.xx * inv_chi -
                (dh_a.xx * dchi_b + dh_b.xx * dchi_a +
                 h.xx * d2chi_ab) *
                    inv_chi2 +
                2.0 * h.xx * dchi_a * dchi_b * inv_chi3,
            d2h_ab.xz * inv_chi -
                (dh_a.xz * dchi_b + dh_b.xz * dchi_a +
                 h.xz * d2chi_ab) *
                    inv_chi2 +
                2.0 * h.xz * dchi_a * dchi_b * inv_chi3,
            d2h_ab.zz * inv_chi -
                (dh_a.zz * dchi_b + dh_b.zz * dchi_a +
                 h.zz * d2chi_ab) *
                    inv_chi2 +
                2.0 * h.zz * dchi_a * dchi_b * inv_chi3,
            d2h_ab.ww * inv_chi -
                (dh_a.ww * dchi_b + dh_b.ww * dchi_a +
                 h.ww * d2chi_ab) *
                    inv_chi2 +
                2.0 * h.ww * dchi_a * dchi_b * inv_chi3};
}

inline Matrix4 inverse_at_equator(
    const ConformalCartoonAlgebra::SymmetricTensor &gamma, const double x)
{
    const double det_xz = gamma.xx * gamma.zz - gamma.xz * gamma.xz;
    const double angular = x * x * gamma.ww;
    if (det_xz <= ricci_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonRicci requires positive reduced physical determinant");
    }
    if (std::abs(angular) <= ricci_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonRicci requires nonzero angular metric component");
    }

    Matrix4 inverse{};
    inverse[0][0] = gamma.zz / det_xz;
    inverse[0][3] = -gamma.xz / det_xz;
    inverse[3][0] = inverse[0][3];
    inverse[3][3] = gamma.xx / det_xz;
    inverse[1][1] = 1.0 / angular;
    inverse[2][2] = 1.0 / angular;
    return inverse;
}

} // namespace detail

inline RicciComponents
compute_metric_derivative_ricci(const CartoonRicciInputs &inputs)
{
    if (inputs.x <= ricci_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonRicci Stage 4G helper is away-axis only and requires x > "
            "0");
    }
    if (std::abs(inputs.chi) <= ricci_zero_tolerance)
    {
        throw std::domain_error("CartoonRicci requires nonzero chi");
    }

    const auto gamma = detail::physical_from_conformal(inputs.h, inputs.chi);
    const auto gamma_x = detail::first_physical_derivative(
        inputs.h, inputs.h_derivatives.dx, inputs.chi,
        inputs.chi_derivatives.first.dx);
    const auto gamma_z = detail::first_physical_derivative(
        inputs.h, inputs.h_derivatives.dz, inputs.chi,
        inputs.chi_derivatives.first.dz);
    const auto gamma_xx = detail::second_physical_derivative(
        inputs.h, inputs.h_derivatives.dx, inputs.h_derivatives.dx,
        inputs.h_derivatives.dxx, inputs.chi,
        inputs.chi_derivatives.first.dx, inputs.chi_derivatives.first.dx,
        inputs.chi_derivatives.second.dxx);
    const auto gamma_xz = detail::second_physical_derivative(
        inputs.h, inputs.h_derivatives.dx, inputs.h_derivatives.dz,
        inputs.h_derivatives.dxz, inputs.chi,
        inputs.chi_derivatives.first.dx, inputs.chi_derivatives.first.dz,
        inputs.chi_derivatives.second.dxz);
    const auto gamma_zz = detail::second_physical_derivative(
        inputs.h, inputs.h_derivatives.dz, inputs.h_derivatives.dz,
        inputs.h_derivatives.dzz, inputs.chi,
        inputs.chi_derivatives.first.dz, inputs.chi_derivatives.first.dz,
        inputs.chi_derivatives.second.dzz);

    const double x = inputs.x;
    const double x2 = x * x;

    detail::MetricDeriv1 dg{};
    detail::MetricDeriv2 ddg{};

    detail::set_symmetric(dg[0], 0, 0, gamma_x.xx);
    detail::set_symmetric(dg[0], 0, 3, gamma_x.xz);
    detail::set_symmetric(dg[0], 3, 3, gamma_x.zz);
    dg[0][1][1] = 2.0 * x * gamma.ww + x2 * gamma_x.ww;
    dg[0][2][2] = dg[0][1][1];

    detail::set_symmetric(dg[3], 0, 0, gamma_z.xx);
    detail::set_symmetric(dg[3], 0, 3, gamma_z.xz);
    detail::set_symmetric(dg[3], 3, 3, gamma_z.zz);
    dg[3][1][1] = x2 * gamma_z.ww;
    dg[3][2][2] = dg[3][1][1];

    const auto fill_second_base =
        [&](const int a, const int b,
            const ConformalCartoonAlgebra::SymmetricTensor &d2gamma,
            const double angular_ww_second) {
            detail::set_symmetric(ddg[a][b], 0, 0, d2gamma.xx);
            detail::set_symmetric(ddg[a][b], 0, 3, d2gamma.xz);
            detail::set_symmetric(ddg[a][b], 3, 3, d2gamma.zz);
            ddg[a][b][1][1] = angular_ww_second;
            ddg[a][b][2][2] = angular_ww_second;
        };

    fill_second_base(0, 0, gamma_xx,
                     2.0 * gamma.ww + 4.0 * x * gamma_x.ww +
                         x2 * gamma_xx.ww);
    fill_second_base(0, 3, gamma_xz,
                     2.0 * x * gamma_z.ww + x2 * gamma_xz.ww);
    fill_second_base(3, 0, gamma_xz,
                     2.0 * x * gamma_z.ww + x2 * gamma_xz.ww);
    fill_second_base(3, 3, gamma_zz, x2 * gamma_zz.ww);

    // At theta = pi/2, first theta derivatives vanish, but
    // partial_theta partial_theta g_phi phi = -2 x^2 gamma_ww is essential for
    // the round two-sphere angular curvature sign.
    ddg[1][1][2][2] = -2.0 * x2 * gamma.ww;

    const auto inverse = detail::inverse_at_equator(gamma, x);

    detail::MetricDeriv1 dinv{};
    for (int a = 0; a < 4; ++a)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                double value = 0.0;
                for (int m = 0; m < 4; ++m)
                {
                    for (int n = 0; n < 4; ++n)
                    {
                        value -= inverse[i][m] * dg[a][m][n] * inverse[n][j];
                    }
                }
                dinv[a][i][j] = value;
            }
        }
    }

    detail::Gamma christoffel{};
    detail::GammaDeriv dchristoffel{};
    for (int l = 0; l < 4; ++l)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int k = 0; k < 4; ++k)
                {
                    const double bracket =
                        dg[i][j][k] + dg[j][i][k] - dg[k][i][j];
                    christoffel[l][i][j] += 0.5 * inverse[l][k] * bracket;

                    for (int a = 0; a < 4; ++a)
                    {
                        const double dbracket =
                            ddg[a][i][j][k] + ddg[a][j][i][k] -
                            ddg[a][k][i][j];
                        dchristoffel[a][l][i][j] +=
                            0.5 * (dinv[a][l][k] * bracket +
                                   inverse[l][k] * dbracket);
                    }
                }
            }
        }
    }

    detail::Matrix4 ricci{};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            double value = 0.0;
            for (int k = 0; k < 4; ++k)
            {
                value += dchristoffel[k][k][i][j];
                value -= dchristoffel[j][k][i][k];

                for (int l = 0; l < 4; ++l)
                {
                    value += christoffel[k][i][j] * christoffel[l][k][l];
                    value -= christoffel[l][i][k] * christoffel[k][j][l];
                }
            }
            ricci[i][j] = value;
        }
    }

    return RicciComponents(ricci[0][0], ricci[0][3], ricci[3][3],
                           ricci[1][1] / x2);
}

} // namespace CartoonRicci
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONRICCIINTERFACE_HPP */
