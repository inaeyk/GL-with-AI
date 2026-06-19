#ifndef BLACKSTRINGTOY_CARTOONCONFORMALFACTORRWW_HPP
#define BLACKSTRINGTOY_CARTOONCONFORMALFACTORRWW_HPP

#include "CartoonAxisPolicy.hpp"
#include "CartoonCheckedDxchiOverX.hpp"
#include "CartoonCheckedDxhwwOverX.hpp"
#include "CartoonCheckedHxzOverX.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonConformalFactorRww
{
// Stage 4AE implements only the local away-axis conformal-factor correction
// R^chi_ww. It is not conformal tilde R_ww[h], not physical R_ww[gamma], not
// the Stage 4AF split-vs-direct identity gate, not full Ricci, not CCZ4 RHS,
// and not evolution wiring.
static constexpr bool conformal_factor_ricci_implemented = true;
static constexpr bool physical_ricci_implemented = false;
static constexpr bool split_vs_direct_identity_gate_implemented = false;
static constexpr bool full_ricci_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;
static constexpr int hidden_multiplicity =
    ConformalCartoonAlgebra::hidden_multiplicity;
static_assert(hidden_multiplicity == 2,
              "Stage 4AE requires two hidden SO(3) directions");

class ConformalFactorRww
{
  public:
    double value() const { return m_value; }
    double hidden_hessian() const { return m_hidden_hessian; }
    double conformal_laplacian() const { return m_conformal_laplacian; }
    double conformal_gradient_norm() const
    {
        return m_conformal_gradient_norm;
    }

  private:
    ConformalFactorRww(const double value, const double hidden_hessian,
                       const double conformal_laplacian,
                       const double conformal_gradient_norm)
        : m_value(value), m_hidden_hessian(hidden_hessian),
          m_conformal_laplacian(conformal_laplacian),
          m_conformal_gradient_norm(conformal_gradient_norm)
    {
    }

    friend ConformalFactorRww compute_conformal_factor_rww(
        const class ConformalFactorRwwInputs &inputs);

    double m_value;
    double m_hidden_hessian;
    double m_conformal_laplacian;
    double m_conformal_gradient_norm;
};

class ConformalFactorRwwInputs
{
  public:
    double x() const { return m_x; }
    double h_xx() const { return m_h_xx; }
    double h_xz() const { return m_h_xz; }
    double h_zz() const { return m_h_zz; }
    double h_ww() const { return m_h_ww; }
    double chi() const { return m_chi; }
    double d_x_h_xx() const { return m_d_x_h_xx; }
    double d_z_h_xx() const { return m_d_z_h_xx; }
    double d_x_h_xz() const { return m_d_x_h_xz; }
    double d_z_h_xz() const { return m_d_z_h_xz; }
    double d_x_h_zz() const { return m_d_x_h_zz; }
    double d_z_h_zz() const { return m_d_z_h_zz; }
    double d_x_h_ww() const { return m_d_x_h_ww; }
    double d_z_h_ww() const { return m_d_z_h_ww; }
    double d_x_chi() const { return m_d_x_chi; }
    double d_z_chi() const { return m_d_z_chi; }
    double d_xx_chi() const { return m_d_xx_chi; }
    double d_xz_chi() const { return m_d_xz_chi; }
    double d_zz_chi() const { return m_d_zz_chi; }
    double reduced_determinant() const { return m_reduced_determinant; }
    double q_xz() const { return m_hxz_over_x.hxz_over_x(); }
    double p_w() const { return m_dxhww_over_x.dxhww_over_x(); }
    double p_chi() const { return m_dxchi_over_x.dxchi_over_x(); }

    const CartoonCheckedHxzOverX::CheckedHxzOverXIngredient &
    checked_hxz_over_x() const
    {
        return m_hxz_over_x;
    }

    const CartoonCheckedDxhwwOverX::CheckedDxhwwOverXIngredient &
    checked_dxhww_over_x() const
    {
        return m_dxhww_over_x;
    }

    const CartoonCheckedDxchiOverX::CheckedDxchiOverXIngredient &
    checked_dxchi_over_x() const
    {
        return m_dxchi_over_x;
    }

  private:
    ConformalFactorRwwInputs(
        const double x, const double h_xx, const double h_xz,
        const double h_zz, const double h_ww, const double chi,
        const double d_x_h_xx, const double d_z_h_xx,
        const double d_x_h_xz, const double d_z_h_xz,
        const double d_x_h_zz, const double d_z_h_zz,
        const double d_x_h_ww, const double d_z_h_ww,
        const double d_x_chi, const double d_z_chi,
        const double d_xx_chi, const double d_xz_chi,
        const double d_zz_chi, const double reduced_determinant,
        const CartoonCheckedHxzOverX::CheckedHxzOverXIngredient &hxz_over_x,
        const CartoonCheckedDxhwwOverX::CheckedDxhwwOverXIngredient
            &dxhww_over_x,
        const CartoonCheckedDxchiOverX::CheckedDxchiOverXIngredient
            &dxchi_over_x)
        : m_x(x), m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz),
          m_h_ww(h_ww), m_chi(chi), m_d_x_h_xx(d_x_h_xx),
          m_d_z_h_xx(d_z_h_xx), m_d_x_h_xz(d_x_h_xz),
          m_d_z_h_xz(d_z_h_xz), m_d_x_h_zz(d_x_h_zz),
          m_d_z_h_zz(d_z_h_zz), m_d_x_h_ww(d_x_h_ww),
          m_d_z_h_ww(d_z_h_ww), m_d_x_chi(d_x_chi),
          m_d_z_chi(d_z_chi), m_d_xx_chi(d_xx_chi),
          m_d_xz_chi(d_xz_chi), m_d_zz_chi(d_zz_chi),
          m_reduced_determinant(reduced_determinant),
          m_hxz_over_x(hxz_over_x), m_dxhww_over_x(dxhww_over_x),
          m_dxchi_over_x(dxchi_over_x)
    {
    }

    friend ConformalFactorRwwInputs make_conformal_factor_rww_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double chi, double d_x_h_xx, double d_z_h_xx,
        double d_x_h_xz, double d_z_h_xz, double d_x_h_zz,
        double d_z_h_zz, double d_x_h_ww, double d_z_h_ww,
        double d_x_chi, double d_z_chi, double d_xx_chi,
        double d_xz_chi, double d_zz_chi);

    double m_x;
    double m_h_xx;
    double m_h_xz;
    double m_h_zz;
    double m_h_ww;
    double m_chi;
    double m_d_x_h_xx;
    double m_d_z_h_xx;
    double m_d_x_h_xz;
    double m_d_z_h_xz;
    double m_d_x_h_zz;
    double m_d_z_h_zz;
    double m_d_x_h_ww;
    double m_d_z_h_ww;
    double m_d_x_chi;
    double m_d_z_chi;
    double m_d_xx_chi;
    double m_d_xz_chi;
    double m_d_zz_chi;
    double m_reduced_determinant;
    CartoonCheckedHxzOverX::CheckedHxzOverXIngredient m_hxz_over_x;
    CartoonCheckedDxhwwOverX::CheckedDxhwwOverXIngredient m_dxhww_over_x;
    CartoonCheckedDxchiOverX::CheckedDxchiOverXIngredient m_dxchi_over_x;
};

inline void require_finite_conformal_factor_inputs(
    const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double chi, const double d_x_h_xx,
    const double d_z_h_xx, const double d_x_h_xz,
    const double d_z_h_xz, const double d_x_h_zz,
    const double d_z_h_zz, const double d_x_h_ww,
    const double d_z_h_ww, const double d_x_chi,
    const double d_z_chi, const double d_xx_chi,
    const double d_xz_chi, const double d_zz_chi)
{
    const double values[] = {
        h_xx,      h_xz,      h_zz,      h_ww,       chi,
        d_x_h_xx,  d_z_h_xx,  d_x_h_xz,  d_z_h_xz,   d_x_h_zz,
        d_z_h_zz,  d_x_h_ww,  d_z_h_ww,  d_x_chi,    d_z_chi,
        d_xx_chi,  d_xz_chi,  d_zz_chi};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "CartoonConformalFactorRww requires finite local metric, "
                "conformal-factor, and derivative inputs");
        }
    }
}

inline double checked_reduced_determinant(const double h_xx,
                                          const double h_xz,
                                          const double h_zz)
{
    const ConformalCartoonAlgebra::ConformalMetric h{h_xx, h_xz, h_zz, 1.0};
    const double determinant = ConformalCartoonAlgebra::reduced_determinant(h);
    if (!std::isfinite(determinant) ||
        determinant <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonConformalFactorRww requires finite positive reduced "
            "determinant");
    }
    return determinant;
}

inline ConformalFactorRwwInputs make_conformal_factor_rww_inputs(
    const double x, const double h_xx, const double h_xz,
    const double h_zz, const double h_ww, const double chi,
    const double d_x_h_xx, const double d_z_h_xx,
    const double d_x_h_xz, const double d_z_h_xz,
    const double d_x_h_zz, const double d_z_h_zz,
    const double d_x_h_ww, const double d_z_h_ww,
    const double d_x_chi, const double d_z_chi,
    const double d_xx_chi, const double d_xz_chi,
    const double d_zz_chi)
{
    // The determinant, base Christoffel data, and all checked quotient
    // ingredients are single-sourced from the same local metric/scalar point.
    // No source-facing overload accepts loose q_xz, p_W, p_chi, determinant,
    // or precomputed Christoffels.
    CartoonAxisPolicy::require_away_axis(x);
    require_finite_conformal_factor_inputs(
        h_xx, h_xz, h_zz, h_ww, chi, d_x_h_xx, d_z_h_xx, d_x_h_xz,
        d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww, d_z_h_ww, d_x_chi,
        d_z_chi, d_xx_chi, d_xz_chi, d_zz_chi);
    if (h_ww <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonConformalFactorRww requires positive h_ww");
    }
    if (chi <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonConformalFactorRww requires positive chi");
    }

    const double determinant = checked_reduced_determinant(h_xx, h_xz, h_zz);
    const auto hxz_over_x =
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {x, h_xz});
    const auto dxhww_over_x =
        CartoonCheckedDxhwwOverX::compute_checked_dxhww_over_x_ingredient(
            {x, d_x_h_ww});
    const auto dxchi_over_x =
        CartoonCheckedDxchiOverX::compute_checked_dxchi_over_x_ingredient(
            {x, d_x_chi});

    return ConformalFactorRwwInputs(
        x, h_xx, h_xz, h_zz, h_ww, chi, d_x_h_xx, d_z_h_xx,
        d_x_h_xz, d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww,
        d_z_h_ww, d_x_chi, d_z_chi, d_xx_chi, d_xz_chi,
        d_zz_chi, determinant, hxz_over_x, dxhww_over_x, dxchi_over_x);
}

inline ConformalFactorRww compute_conformal_factor_rww(
    const ConformalFactorRwwInputs &inputs)
{
    const double x = inputs.x();
    const double x2 = x * x;
    const double A = inputs.h_xx();
    const double B = inputs.h_xz();
    const double C = inputs.h_zz();
    const double W = inputs.h_ww();
    const double chi = inputs.chi();
    const double D = inputs.reduced_determinant();

    // Guarded Stage 4AD form. The potentially cancellation-sensitive hidden
    // 1/x structure is expressed only through checked q_xz, p_W, and p_chi.
    const double hidden_hessian =
        (C / D) *
            (W * inputs.p_chi() +
             0.5 * x2 * inputs.p_w() * inputs.p_chi()) -
        (inputs.q_xz() / D) *
            (W * inputs.d_z_chi() +
             0.5 * x2 * inputs.p_w() * inputs.d_z_chi() +
             0.5 * x2 * inputs.d_z_h_ww() * inputs.p_chi()) +
        (A / D) * 0.5 * inputs.d_z_h_ww() * inputs.d_z_chi();

    const double gamma_x_xx =
        (C * inputs.d_x_h_xx() -
         B * (2.0 * inputs.d_x_h_xz() - inputs.d_z_h_xx())) /
        (2.0 * D);
    const double gamma_z_xx =
        (-B * inputs.d_x_h_xx() +
         A * (2.0 * inputs.d_x_h_xz() - inputs.d_z_h_xx())) /
        (2.0 * D);
    const double gamma_x_xz =
        (C * inputs.d_z_h_xx() - B * inputs.d_x_h_zz()) / (2.0 * D);
    const double gamma_z_xz =
        (-B * inputs.d_z_h_xx() + A * inputs.d_x_h_zz()) / (2.0 * D);
    const double gamma_x_zz =
        (C * (2.0 * inputs.d_z_h_xz() - inputs.d_x_h_zz()) -
         B * inputs.d_z_h_zz()) /
        (2.0 * D);
    const double gamma_z_zz =
        (-B * (2.0 * inputs.d_z_h_xz() - inputs.d_x_h_zz()) +
         A * inputs.d_z_h_zz()) /
        (2.0 * D);

    const double k_xx =
        inputs.d_xx_chi() - gamma_x_xx * inputs.d_x_chi() -
        gamma_z_xx * inputs.d_z_chi();
    const double k_xz =
        inputs.d_xz_chi() - gamma_x_xz * inputs.d_x_chi() -
        gamma_z_xz * inputs.d_z_chi();
    const double k_zz =
        inputs.d_zz_chi() - gamma_x_zz * inputs.d_x_chi() -
        gamma_z_zz * inputs.d_z_chi();

    const double conformal_laplacian =
        (C * k_xx - 2.0 * B * k_xz + A * k_zz) / D +
        (static_cast<double>(hidden_multiplicity) / W) * hidden_hessian;
    const double conformal_gradient_norm =
        (C * inputs.d_x_chi() * inputs.d_x_chi() -
         2.0 * B * inputs.d_x_chi() * inputs.d_z_chi() +
         A * inputs.d_z_chi() * inputs.d_z_chi()) /
        D;
    const double value =
        hidden_hessian / chi +
        W * conformal_laplacian / (2.0 * chi) -
        W * conformal_gradient_norm / (chi * chi);

    const double outputs[] = {hidden_hessian, conformal_laplacian,
                              conformal_gradient_norm, value};
    for (const double output : outputs)
    {
        if (!std::isfinite(output))
        {
            throw std::domain_error(
                "CartoonConformalFactorRww produced a nonfinite local result");
        }
    }

    return ConformalFactorRww(value, hidden_hessian, conformal_laplacian,
                              conformal_gradient_norm);
}

} // namespace CartoonConformalFactorRww
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCONFORMALFACTORRWW_HPP */
