#ifndef BLACKSTRINGTOY_CARTOONCONFORMALRWWGRADIENTBLOCK_HPP
#define BLACKSTRINGTOY_CARTOONCONFORMALRWWGRADIENTBLOCK_HPP

#include "CartoonCheckedDxhwwOverX.hpp"
#include "CartoonCheckedHxzOverX.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonConformalRwwGradientBlock
{
// Stage 4Z implements only the first-derivative gradient sub-block in the
// Stage 4W conformal hidden Ricci decomposition:
// G_grad = -(C/D) p_W + (q_xz W_z)/D
//          - (C W_x^2 - 2 B W_x W_z + A W_z^2)/(4 W D).
// This is not the singular block, not the Hessian block, not full tilde R_ww,
// not R^chi_ww, not physical R_ww[gamma], not CCZ4 RHS, and not evolution
// wiring. It is away-axis only through the checked ingredients it consumes.
static constexpr bool singular_block_implemented_here = false;
static constexpr bool hessian_block_implemented = false;
static constexpr bool full_conformal_hidden_ricci_implemented = false;
static constexpr bool conformal_factor_ricci_implemented = false;
static constexpr bool physical_ricci_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class ConformalRwwGradientBlock
{
  public:
    double gradient_block() const { return m_gradient_block; }

  private:
    explicit ConformalRwwGradientBlock(const double gradient_block)
        : m_gradient_block(gradient_block)
    {
    }

    friend ConformalRwwGradientBlock compute_conformal_rww_gradient_block(
        const class ConformalRwwGradientBlockInputs &inputs);

    double m_gradient_block;
};

class ConformalRwwGradientBlockInputs
{
  public:
    double h_xx() const { return m_h_xx; }
    double h_xz() const { return m_h_xz; }
    double h_zz() const { return m_h_zz; }
    double h_ww() const { return m_h_ww; }
    double d_x_hww() const { return m_d_x_hww; }
    double d_z_hww() const { return m_d_z_hww; }
    double reduced_determinant() const { return m_reduced_determinant; }
    double q_xz() const { return m_hxz_over_x.hxz_over_x(); }
    double p_w() const { return m_dxhww_over_x.dxhww_over_x(); }

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

  private:
    ConformalRwwGradientBlockInputs(
        const double h_xx, const double h_xz, const double h_zz,
        const double h_ww, const double d_x_hww, const double d_z_hww,
        const double reduced_determinant,
        const CartoonCheckedHxzOverX::CheckedHxzOverXIngredient &hxz_over_x,
        const CartoonCheckedDxhwwOverX::CheckedDxhwwOverXIngredient
            &dxhww_over_x)
        : m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz), m_h_ww(h_ww),
          m_d_x_hww(d_x_hww), m_d_z_hww(d_z_hww),
          m_reduced_determinant(reduced_determinant),
          m_hxz_over_x(hxz_over_x), m_dxhww_over_x(dxhww_over_x)
    {
    }

    friend ConformalRwwGradientBlockInputs
    make_conformal_rww_gradient_block_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double d_x_hww, double d_z_hww);

    double m_h_xx;
    double m_h_xz;
    double m_h_zz;
    double m_h_ww;
    double m_d_x_hww;
    double m_d_z_hww;
    double m_reduced_determinant;
    CartoonCheckedHxzOverX::CheckedHxzOverXIngredient m_hxz_over_x;
    CartoonCheckedDxhwwOverX::CheckedDxhwwOverXIngredient m_dxhww_over_x;
};

inline void require_finite_gradient_inputs(const double h_xx,
                                           const double h_xz,
                                           const double h_zz,
                                           const double h_ww,
                                           const double d_x_hww,
                                           const double d_z_hww)
{
    const double values[] = {h_xx, h_xz, h_zz, h_ww, d_x_hww, d_z_hww};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "CartoonConformalRwwGradientBlock requires finite local "
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
            "CartoonConformalRwwGradientBlock requires finite positive "
            "reduced determinant");
    }
    return determinant;
}

inline ConformalRwwGradientBlockInputs
make_conformal_rww_gradient_block_inputs(const double x, const double h_xx,
                                         const double h_xz, const double h_zz,
                                         const double h_ww,
                                         const double d_x_hww,
                                         const double d_z_hww)
{
    // The determinant inputs and checked quotient ingredients are
    // single-sourced from the same local metric point by this factory. This
    // prevents callers from mixing q_xz, p_W, and determinant data produced
    // from different points.
    require_finite_gradient_inputs(h_xx, h_xz, h_zz, h_ww, d_x_hww,
                                   d_z_hww);
    if (h_ww <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonConformalRwwGradientBlock requires positive h_ww");
    }

    const double determinant = checked_reduced_determinant(h_xx, h_xz, h_zz);
    const auto hxz_over_x =
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {x, h_xz});
    const auto dxhww_over_x =
        CartoonCheckedDxhwwOverX::compute_checked_dxhww_over_x_ingredient(
            {x, d_x_hww});

    return ConformalRwwGradientBlockInputs(
        h_xx, h_xz, h_zz, h_ww, d_x_hww, d_z_hww, determinant, hxz_over_x,
        dxhww_over_x);
}

inline ConformalRwwGradientBlock compute_conformal_rww_gradient_block(
    const ConformalRwwGradientBlockInputs &inputs)
{
    const double numerator_quadratic =
        inputs.h_zz() * inputs.d_x_hww() * inputs.d_x_hww() -
        2.0 * inputs.h_xz() * inputs.d_x_hww() * inputs.d_z_hww() +
        inputs.h_xx() * inputs.d_z_hww() * inputs.d_z_hww();

    const double block =
        -(inputs.h_zz() * inputs.p_w()) / inputs.reduced_determinant() +
        (inputs.q_xz() * inputs.d_z_hww()) / inputs.reduced_determinant() -
        numerator_quadratic /
            (4.0 * inputs.h_ww() * inputs.reduced_determinant());

    return ConformalRwwGradientBlock(block);
}

} // namespace CartoonConformalRwwGradientBlock
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCONFORMALRWWGRADIENTBLOCK_HPP */
