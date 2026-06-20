#ifndef BLACKSTRINGTOY_CARTOONHATGAMMAX_HPP
#define BLACKSTRINGTOY_CARTOONHATGAMMAX_HPP

#include "CartoonConformalRwwSingularBlock.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonHatGammaX
{
// Stage 4AN implements the local away-axis hatted-connection contract:
// hat_Gamma^x = tilde_Gamma^x + 2 Z_over_chi^x.
// This matches GRChombo's convention
// Z_over_chi^x = 0.5 * (hat_Gamma^x - tilde_Gamma^x).
//
// This is not the Gamma RHS, not the Stage 4AO GL gate, not grid regularity
// validation, not finite-axis handling, and not evolution wiring.
static constexpr bool grchombo_hatted_convention_implemented = true;
static constexpr bool gamma_rhs_implemented = false;
static constexpr bool gl_gate_passed = false;
static constexpr bool grid_regularity_validated = false;
static constexpr bool finite_axis_handling_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class HatGammaX
{
  public:
    double base_contraction() const { return m_base_contraction; }
    double hidden_contraction() const { return m_hidden_contraction; }
    double tilde_gamma_x() const { return m_tilde_gamma_x; }
    double z_over_chi_x() const { return m_z_over_chi_x; }
    double hat_gamma_x() const { return m_hat_gamma_x; }

  private:
    HatGammaX(const double base_contraction, const double hidden_contraction,
              const double tilde_gamma_x, const double z_over_chi_x,
              const double hat_gamma_x)
        : m_base_contraction(base_contraction),
          m_hidden_contraction(hidden_contraction),
          m_tilde_gamma_x(tilde_gamma_x), m_z_over_chi_x(z_over_chi_x),
          m_hat_gamma_x(hat_gamma_x)
    {
    }

    friend HatGammaX compute_hat_gamma_x(const class HatGammaXInputs &inputs);

    double m_base_contraction;
    double m_hidden_contraction;
    double m_tilde_gamma_x;
    double m_z_over_chi_x;
    double m_hat_gamma_x;
};

class HatGammaXInputs
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
    double z_over_chi_x() const { return m_z_over_chi_x; }
    double reduced_determinant() const
    {
        return m_singular_inputs.reduced_determinant();
    }
    double delta_xw() const { return m_singular_inputs.delta_xw(); }
    double q_xz() const { return m_singular_inputs.q_xz(); }

    const CartoonConformalRwwSingularBlock::ConformalRwwSingularBlockInputs &
    checked_singular_inputs() const
    {
        return m_singular_inputs;
    }

  private:
    HatGammaXInputs(
        const double x, const double h_xx, const double h_xz,
        const double h_zz, const double h_ww, const double d_x_h_xx,
        const double d_z_h_xx, const double d_x_h_xz,
        const double d_z_h_xz, const double d_x_h_zz,
        const double d_z_h_zz, const double d_x_h_ww,
        const double d_z_h_ww, const double z_over_chi_x,
        const CartoonConformalRwwSingularBlock::ConformalRwwSingularBlockInputs
            &singular_inputs)
        : m_x(x), m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz), m_h_ww(h_ww),
          m_d_x_h_xx(d_x_h_xx), m_d_z_h_xx(d_z_h_xx),
          m_d_x_h_xz(d_x_h_xz), m_d_z_h_xz(d_z_h_xz),
          m_d_x_h_zz(d_x_h_zz), m_d_z_h_zz(d_z_h_zz),
          m_d_x_h_ww(d_x_h_ww), m_d_z_h_ww(d_z_h_ww),
          m_z_over_chi_x(z_over_chi_x), m_singular_inputs(singular_inputs)
    {
    }

    friend HatGammaXInputs make_hat_gamma_x_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double d_x_h_xx, double d_z_h_xx, double d_x_h_xz,
        double d_z_h_xz, double d_x_h_zz, double d_z_h_zz,
        double d_x_h_ww, double d_z_h_ww, double z_over_chi_x);
    friend HatGammaX compute_hat_gamma_x(const HatGammaXInputs &inputs);

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
    double m_z_over_chi_x;
    CartoonConformalRwwSingularBlock::ConformalRwwSingularBlockInputs
        m_singular_inputs;
};

inline void require_finite_hat_gamma_x_inputs(
    const double d_x_h_xx, const double d_z_h_xx, const double d_x_h_xz,
    const double d_z_h_xz, const double d_x_h_zz, const double d_z_h_zz,
    const double d_x_h_ww, const double d_z_h_ww,
    const double z_over_chi_x)
{
    const double values[] = {d_x_h_xx, d_z_h_xx, d_x_h_xz, d_z_h_xz,
                             d_x_h_zz, d_z_h_zz, d_x_h_ww, d_z_h_ww,
                             z_over_chi_x};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "CartoonHatGammaX requires finite derivative and "
                "Z_over_chi inputs");
        }
    }
}

inline HatGammaXInputs make_hat_gamma_x_inputs(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double d_x_h_xx, const double d_z_h_xx,
    const double d_x_h_xz, const double d_z_h_xz,
    const double d_x_h_zz, const double d_z_h_zz,
    const double d_x_h_ww, const double d_z_h_ww,
    const double z_over_chi_x)
{
    require_finite_hat_gamma_x_inputs(
        d_x_h_xx, d_z_h_xx, d_x_h_xz, d_z_h_xz, d_x_h_zz, d_z_h_zz,
        d_x_h_ww, d_z_h_ww, z_over_chi_x);
    if (!std::isfinite(h_ww) ||
        h_ww <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error("CartoonHatGammaX requires positive h_ww");
    }

    // The regularity-sensitive ingredients are minted by the existing checked
    // path from the same local point: Delta_xw=(A-W)/x^2 comes through the
    // Stage 4U/4R package, and q_xz=B/x comes through the Stage 4X package.
    const auto singular_inputs =
        CartoonConformalRwwSingularBlock::
            make_conformal_rww_singular_block_inputs(x, h_xx, h_xz, h_zz,
                                                     h_ww);

    return HatGammaXInputs(x, h_xx, h_xz, h_zz, h_ww, d_x_h_xx, d_z_h_xx,
                           d_x_h_xz, d_z_h_xz, d_x_h_zz, d_z_h_zz,
                           d_x_h_ww, d_z_h_ww, z_over_chi_x,
                           singular_inputs);
}

inline HatGammaX compute_hat_gamma_x(const HatGammaXInputs &inputs)
{
    const double determinant = inputs.reduced_determinant();
    const double h_xx_inverse = inputs.h_zz() / determinant;
    const double h_xz_inverse = -inputs.h_xz() / determinant;
    const double h_zz_inverse = inputs.h_xx() / determinant;

    const double gamma_x_xx =
        (inputs.h_zz() * inputs.d_x_h_xx() -
         inputs.h_xz() *
             (2.0 * inputs.d_x_h_xz() - inputs.d_z_h_xx())) /
        (2.0 * determinant);
    const double gamma_x_xz =
        (inputs.h_zz() * inputs.d_z_h_xx() -
         inputs.h_xz() * inputs.d_x_h_zz()) /
        (2.0 * determinant);
    const double gamma_x_zz =
        (inputs.h_zz() *
             (2.0 * inputs.d_z_h_xz() - inputs.d_x_h_zz()) -
         inputs.h_xz() * inputs.d_z_h_zz()) /
        (2.0 * determinant);

    const double base_contraction =
        h_xx_inverse * gamma_x_xx + 2.0 * h_xz_inverse * gamma_x_xz +
        h_zz_inverse * gamma_x_zz;

    const double a_minus_w_over_x = inputs.x() * inputs.delta_xw();
    const double b_over_x = inputs.q_xz();
    const double gamma_x_ww =
        h_xx_inverse *
            (a_minus_w_over_x - 0.5 * inputs.d_x_h_ww()) +
        h_xz_inverse * (b_over_x - 0.5 * inputs.d_z_h_ww());
    const double hidden_contraction = 2.0 * gamma_x_ww / inputs.h_ww();

    const double tilde_gamma_x = base_contraction + hidden_contraction;
    const double hat_gamma_x =
        tilde_gamma_x + 2.0 * inputs.z_over_chi_x();

    if (!std::isfinite(base_contraction) ||
        !std::isfinite(hidden_contraction) ||
        !std::isfinite(tilde_gamma_x) || !std::isfinite(hat_gamma_x))
    {
        throw std::domain_error(
            "CartoonHatGammaX produced a nonfinite local result");
    }

    return HatGammaX(base_contraction, hidden_contraction, tilde_gamma_x,
                     inputs.z_over_chi_x(), hat_gamma_x);
}

} // namespace CartoonHatGammaX
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONHATGAMMAX_HPP */
