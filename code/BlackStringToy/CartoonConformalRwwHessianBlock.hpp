#ifndef BLACKSTRINGTOY_CARTOONCONFORMALRWWHESSIANBLOCK_HPP
#define BLACKSTRINGTOY_CARTOONCONFORMALRWWHESSIANBLOCK_HPP

#include "CartoonAxisPolicy.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonConformalRwwHessianBlock
{
// Stage 4AB implements only the Hessian sub-block in the Stage 4W conformal
// hidden Ricci decomposition:
// G_Hess = -(sqrt(W)/x) [(C/D) H_xx - (2B/D) H_xz + (A/D) H_zz].
// This is not the singular block, not the first-derivative gradient block, not
// full tilde R_ww, not R^chi_ww, not physical R_ww[gamma], not CCZ4 RHS, and
// not evolution wiring. It is local and away-axis only.
static constexpr bool singular_block_implemented_here = false;
static constexpr bool gradient_block_implemented_here = false;
static constexpr bool full_conformal_hidden_ricci_implemented = false;
static constexpr bool conformal_factor_ricci_implemented = false;
static constexpr bool physical_ricci_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class ConformalRwwHessianBlock
{
  public:
    double hessian_block() const { return m_hessian_block; }

  private:
    explicit ConformalRwwHessianBlock(const double hessian_block)
        : m_hessian_block(hessian_block)
    {
    }

    friend ConformalRwwHessianBlock compute_conformal_rww_hessian_block(
        const class ConformalRwwHessianBlockInputs &inputs);

    double m_hessian_block;
};

class ConformalRwwHessianBlockInputs
{
  public:
    double x() const { return m_x; }
    double h_xx() const { return m_h_xx; }
    double h_xz() const { return m_h_xz; }
    double h_zz() const { return m_h_zz; }
    double h_ww() const { return m_h_ww; }
    double d_x_h_xx() const { return m_d_x_h_xx; }
    double d_z_h_xx() const { return m_d_z_h_xx; }
    double d_x_h_xz() const { return m_d_x_h_xz; }
    double d_z_h_xz() const { return m_d_z_h_xz; }
    double d_x_h_zz() const { return m_d_x_h_zz; }
    double d_z_h_zz() const { return m_d_z_h_zz; }
    double d_x_h_ww() const { return m_d_x_h_ww; }
    double d_z_h_ww() const { return m_d_z_h_ww; }
    double d_xx_h_ww() const { return m_d_xx_h_ww; }
    double d_xz_h_ww() const { return m_d_xz_h_ww; }
    double d_zz_h_ww() const { return m_d_zz_h_ww; }
    double reduced_determinant() const { return m_reduced_determinant; }

  private:
    ConformalRwwHessianBlockInputs(
        const double x, const double h_xx, const double h_xz,
        const double h_zz, const double h_ww, const double d_x_h_xx,
        const double d_z_h_xx, const double d_x_h_xz,
        const double d_z_h_xz, const double d_x_h_zz,
        const double d_z_h_zz, const double d_x_h_ww,
        const double d_z_h_ww, const double d_xx_h_ww,
        const double d_xz_h_ww, const double d_zz_h_ww,
        const double reduced_determinant)
        : m_x(x), m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz),
          m_h_ww(h_ww), m_d_x_h_xx(d_x_h_xx), m_d_z_h_xx(d_z_h_xx),
          m_d_x_h_xz(d_x_h_xz), m_d_z_h_xz(d_z_h_xz),
          m_d_x_h_zz(d_x_h_zz), m_d_z_h_zz(d_z_h_zz),
          m_d_x_h_ww(d_x_h_ww), m_d_z_h_ww(d_z_h_ww),
          m_d_xx_h_ww(d_xx_h_ww), m_d_xz_h_ww(d_xz_h_ww),
          m_d_zz_h_ww(d_zz_h_ww),
          m_reduced_determinant(reduced_determinant)
    {
    }

    friend ConformalRwwHessianBlockInputs
    make_conformal_rww_hessian_block_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double d_x_h_xx, double d_z_h_xx, double d_x_h_xz,
        double d_z_h_xz, double d_x_h_zz, double d_z_h_zz,
        double d_x_h_ww, double d_z_h_ww, double d_xx_h_ww,
        double d_xz_h_ww, double d_zz_h_ww);

    double m_x;
    double m_h_xx;
    double m_h_xz;
    double m_h_zz;
    double m_h_ww;
    double m_d_x_h_xx;
    double m_d_z_h_xx;
    double m_d_x_h_xz;
    double m_d_z_h_xz;
    double m_d_x_h_zz;
    double m_d_z_h_zz;
    double m_d_x_h_ww;
    double m_d_z_h_ww;
    double m_d_xx_h_ww;
    double m_d_xz_h_ww;
    double m_d_zz_h_ww;
    double m_reduced_determinant;
};

inline void require_finite_hessian_inputs(
    const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double d_x_h_xx, const double d_z_h_xx,
    const double d_x_h_xz, const double d_z_h_xz, const double d_x_h_zz,
    const double d_z_h_zz, const double d_x_h_ww, const double d_z_h_ww,
    const double d_xx_h_ww, const double d_xz_h_ww,
    const double d_zz_h_ww)
{
    const double values[] = {h_xx,      h_xz,      h_zz,      h_ww,
                             d_x_h_xx,  d_z_h_xx,  d_x_h_xz,  d_z_h_xz,
                             d_x_h_zz,  d_z_h_zz,  d_x_h_ww,  d_z_h_ww,
                             d_xx_h_ww, d_xz_h_ww, d_zz_h_ww};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "CartoonConformalRwwHessianBlock requires finite local "
                "metric and derivative inputs");
        }
    }
}

inline double checked_reduced_determinant(const double h_xx, const double h_xz,
                                          const double h_zz)
{
    const ConformalCartoonAlgebra::ConformalMetric h{h_xx, h_xz, h_zz, 1.0};
    const double determinant = ConformalCartoonAlgebra::reduced_determinant(h);
    if (!std::isfinite(determinant) ||
        determinant <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonConformalRwwHessianBlock requires finite positive "
            "reduced determinant");
    }
    return determinant;
}

inline ConformalRwwHessianBlockInputs
make_conformal_rww_hessian_block_inputs(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double d_x_h_xx, const double d_z_h_xx,
    const double d_x_h_xz, const double d_z_h_xz, const double d_x_h_zz,
    const double d_z_h_zz, const double d_x_h_ww, const double d_z_h_ww,
    const double d_xx_h_ww, const double d_xz_h_ww,
    const double d_zz_h_ww)
{
    // The metric values, determinant, base Christoffel ingredients, and rho
    // derivatives are single-sourced from one local point by this factory. No
    // public API accepts loose determinant or precomputed Christoffel inputs.
    CartoonAxisPolicy::require_away_axis(x);
    require_finite_hessian_inputs(h_xx, h_xz, h_zz, h_ww, d_x_h_xx,
                                  d_z_h_xx, d_x_h_xz, d_z_h_xz, d_x_h_zz,
                                  d_z_h_zz, d_x_h_ww, d_z_h_ww, d_xx_h_ww,
                                  d_xz_h_ww, d_zz_h_ww);
    if (h_ww <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonConformalRwwHessianBlock requires positive h_ww");
    }

    const double determinant = checked_reduced_determinant(h_xx, h_xz, h_zz);

    return ConformalRwwHessianBlockInputs(
        x, h_xx, h_xz, h_zz, h_ww, d_x_h_xx, d_z_h_xx, d_x_h_xz,
        d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww, d_z_h_ww, d_xx_h_ww,
        d_xz_h_ww, d_zz_h_ww, determinant);
}

inline ConformalRwwHessianBlock compute_conformal_rww_hessian_block(
    const ConformalRwwHessianBlockInputs &inputs)
{
    const double x = inputs.x();
    const double A = inputs.h_xx();
    const double B = inputs.h_xz();
    const double C = inputs.h_zz();
    const double W = inputs.h_ww();
    const double D = inputs.reduced_determinant();

    const double sqrt_w = std::sqrt(W);
    const double w_three_halves = W * sqrt_w;

    const double rho_x =
        sqrt_w + x * inputs.d_x_h_ww() / (2.0 * sqrt_w);
    const double rho_z = x * inputs.d_z_h_ww() / (2.0 * sqrt_w);

    const double rho_xx =
        inputs.d_x_h_ww() / sqrt_w +
        x * inputs.d_xx_h_ww() / (2.0 * sqrt_w) -
        x * inputs.d_x_h_ww() * inputs.d_x_h_ww() /
            (4.0 * w_three_halves);
    const double rho_xz =
        inputs.d_z_h_ww() / (2.0 * sqrt_w) +
        x * inputs.d_xz_h_ww() / (2.0 * sqrt_w) -
        x * inputs.d_x_h_ww() * inputs.d_z_h_ww() /
            (4.0 * w_three_halves);
    const double rho_zz =
        x * inputs.d_zz_h_ww() / (2.0 * sqrt_w) -
        x * inputs.d_z_h_ww() * inputs.d_z_h_ww() /
            (4.0 * w_three_halves);

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

    const double hessian_xx =
        rho_xx - gamma_x_xx * rho_x - gamma_z_xx * rho_z;
    const double hessian_xz =
        rho_xz - gamma_x_xz * rho_x - gamma_z_xz * rho_z;
    const double hessian_zz =
        rho_zz - gamma_x_zz * rho_x - gamma_z_zz * rho_z;

    const double contraction =
        (C * hessian_xx - 2.0 * B * hessian_xz + A * hessian_zz) / D;
    const double block = -(sqrt_w / x) * contraction;

    return ConformalRwwHessianBlock(block);
}

} // namespace CartoonConformalRwwHessianBlock
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCONFORMALRWWHESSIANBLOCK_HPP */
