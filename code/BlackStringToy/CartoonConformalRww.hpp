#ifndef BLACKSTRINGTOY_CARTOONCONFORMALRWW_HPP
#define BLACKSTRINGTOY_CARTOONCONFORMALRWW_HPP

#include "CartoonConformalRwwGradientBlock.hpp"
#include "CartoonConformalRwwHessianBlock.hpp"
#include "CartoonConformalRwwSingularBlock.hpp"

namespace BlackStringToy
{
namespace CartoonConformalRww
{
// Stage 4AC assembles only the local away-axis conformal hidden Ricci
// component tilde R_ww[h] from reviewed Stage 4Y/4Z/4AB sub-blocks:
// tilde R_ww = G_sing + G_grad + G_Hess.
// This is not R^chi_ww, not physical R_ww[gamma], not full Ricci, not CCZ4
// RHS, and not evolution wiring.
static constexpr bool conformal_hidden_ricci_implemented = true;
static constexpr bool conformal_factor_ricci_implemented = false;
static constexpr bool physical_ricci_implemented = false;
static constexpr bool full_ricci_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class ConformalRww
{
  public:
    double value() const { return m_value; }
    double singular_block() const { return m_singular_block; }
    double gradient_block() const { return m_gradient_block; }
    double hessian_block() const { return m_hessian_block; }

  private:
    ConformalRww(const double value, const double singular_block,
                 const double gradient_block, const double hessian_block)
        : m_value(value), m_singular_block(singular_block),
          m_gradient_block(gradient_block), m_hessian_block(hessian_block)
    {
    }

    friend ConformalRww compute_conformal_rww(
        const class ConformalRwwInputs &inputs);

    double m_value;
    double m_singular_block;
    double m_gradient_block;
    double m_hessian_block;
};

class ConformalRwwInputs
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

    const CartoonConformalRwwSingularBlock::ConformalRwwSingularBlockInputs &
    singular_inputs() const
    {
        return m_singular_inputs;
    }

    const CartoonConformalRwwGradientBlock::ConformalRwwGradientBlockInputs &
    gradient_inputs() const
    {
        return m_gradient_inputs;
    }

    const CartoonConformalRwwHessianBlock::ConformalRwwHessianBlockInputs &
    hessian_inputs() const
    {
        return m_hessian_inputs;
    }

  private:
    ConformalRwwInputs(
        const double x, const double h_xx, const double h_xz,
        const double h_zz, const double h_ww, const double d_x_h_xx,
        const double d_z_h_xx, const double d_x_h_xz,
        const double d_z_h_xz, const double d_x_h_zz,
        const double d_z_h_zz, const double d_x_h_ww,
        const double d_z_h_ww, const double d_xx_h_ww,
        const double d_xz_h_ww, const double d_zz_h_ww,
        const CartoonConformalRwwSingularBlock::
            ConformalRwwSingularBlockInputs &singular_inputs,
        const CartoonConformalRwwGradientBlock::
            ConformalRwwGradientBlockInputs &gradient_inputs,
        const CartoonConformalRwwHessianBlock::
            ConformalRwwHessianBlockInputs &hessian_inputs)
        : m_x(x), m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz),
          m_h_ww(h_ww), m_d_x_h_xx(d_x_h_xx), m_d_z_h_xx(d_z_h_xx),
          m_d_x_h_xz(d_x_h_xz), m_d_z_h_xz(d_z_h_xz),
          m_d_x_h_zz(d_x_h_zz), m_d_z_h_zz(d_z_h_zz),
          m_d_x_h_ww(d_x_h_ww), m_d_z_h_ww(d_z_h_ww),
          m_d_xx_h_ww(d_xx_h_ww), m_d_xz_h_ww(d_xz_h_ww),
          m_d_zz_h_ww(d_zz_h_ww), m_singular_inputs(singular_inputs),
          m_gradient_inputs(gradient_inputs), m_hessian_inputs(hessian_inputs)
    {
    }

    friend ConformalRwwInputs make_conformal_rww_inputs(
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
    CartoonConformalRwwSingularBlock::ConformalRwwSingularBlockInputs
        m_singular_inputs;
    CartoonConformalRwwGradientBlock::ConformalRwwGradientBlockInputs
        m_gradient_inputs;
    CartoonConformalRwwHessianBlock::ConformalRwwHessianBlockInputs
        m_hessian_inputs;
};

inline ConformalRwwInputs make_conformal_rww_inputs(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double d_x_h_xx, const double d_z_h_xx,
    const double d_x_h_xz, const double d_z_h_xz, const double d_x_h_zz,
    const double d_z_h_zz, const double d_x_h_ww, const double d_z_h_ww,
    const double d_xx_h_ww, const double d_xz_h_ww,
    const double d_zz_h_ww)
{
    // Single-source assembly gate: each reviewed sub-block input is minted from
    // the same local metric/derivative point. No source-facing overload accepts
    // loose raw block values, raw determinant values, or precomputed
    // Christoffels.
    const auto singular_inputs =
        CartoonConformalRwwSingularBlock::
            make_conformal_rww_singular_block_inputs(x, h_xx, h_xz, h_zz,
                                                     h_ww);
    const auto gradient_inputs =
        CartoonConformalRwwGradientBlock::
            make_conformal_rww_gradient_block_inputs(x, h_xx, h_xz, h_zz,
                                                     h_ww, d_x_h_ww,
                                                     d_z_h_ww);
    const auto hessian_inputs =
        CartoonConformalRwwHessianBlock::
            make_conformal_rww_hessian_block_inputs(
                x, h_xx, h_xz, h_zz, h_ww, d_x_h_xx, d_z_h_xx, d_x_h_xz,
                d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww, d_z_h_ww,
                d_xx_h_ww, d_xz_h_ww, d_zz_h_ww);

    return ConformalRwwInputs(
        x, h_xx, h_xz, h_zz, h_ww, d_x_h_xx, d_z_h_xx, d_x_h_xz,
        d_z_h_xz, d_x_h_zz, d_z_h_zz, d_x_h_ww, d_z_h_ww, d_xx_h_ww,
        d_xz_h_ww, d_zz_h_ww, singular_inputs, gradient_inputs,
        hessian_inputs);
}

inline ConformalRww compute_conformal_rww(const ConformalRwwInputs &inputs)
{
    const double singular =
        CartoonConformalRwwSingularBlock::
            compute_conformal_rww_singular_gradient_block(
                inputs.singular_inputs())
                .singular_gradient_block();
    const double gradient =
        CartoonConformalRwwGradientBlock::compute_conformal_rww_gradient_block(
            inputs.gradient_inputs())
            .gradient_block();
    const double hessian =
        CartoonConformalRwwHessianBlock::compute_conformal_rww_hessian_block(
            inputs.hessian_inputs())
            .hessian_block();

    return ConformalRww(singular + gradient + hessian, singular, gradient,
                        hessian);
}

} // namespace CartoonConformalRww
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCONFORMALRWW_HPP */
