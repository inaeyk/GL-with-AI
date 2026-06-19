#ifndef BLACKSTRINGTOY_CARTOONPHYSICALHIDDENLAPSEHESSIAN_HPP
#define BLACKSTRINGTOY_CARTOONPHYSICALHIDDENLAPSEHESSIAN_HPP

#include "CartoonAxisPolicy.hpp"
#include "CartoonCheckedDxalphaOverX.hpp"
#include "CartoonCheckedDxchiOverX.hpp"
#include "CartoonCheckedDxhwwOverX.hpp"
#include "CartoonCheckedHxzOverX.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonPhysicalHiddenLapseHessian
{
namespace CheckedAlpha = CartoonCheckedDxalphaOverX;
namespace CheckedChi = CartoonCheckedDxchiOverX;
namespace CheckedHxz = CartoonCheckedHxzOverX;
namespace CheckedW = CartoonCheckedDxhwwOverX;

// Stage 4AJ implements only the local away-axis physical hidden lapse Hessian
// D_w D_w alpha. The connection is built from gamma_ij = chi^{-1} h_ij, not
// from the conformal metric alone. The CCZ4 source term sign -D_wD_w alpha,
// full A_ww source block, RHS, grid reads, finite-axis support, and evolution
// wiring remain future work.
static constexpr bool away_axis_only = true;
static constexpr bool uses_physical_metric = true;
static constexpr bool conformal_hessian_reused = false;
static constexpr bool source_minus_sign_applied = false;
static constexpr bool full_Aww_source_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class PhysicalHiddenLapseHessian
{
  public:
    double dww_alpha() const { return m_dww_alpha; }

  private:
    explicit PhysicalHiddenLapseHessian(const double dww_alpha)
        : m_dww_alpha(dww_alpha)
    {
    }

    friend PhysicalHiddenLapseHessian
    compute_physical_hidden_lapse_hessian(
        const class PhysicalHiddenLapseHessianInputs &inputs);

    double m_dww_alpha;
};

class PhysicalHiddenLapseHessianInputs
{
  private:
    PhysicalHiddenLapseHessianInputs(
        const double x, const double h_xx, const double h_xz,
        const double h_zz, const double h_ww, const double chi,
        const double d_x_h_ww, const double d_z_h_ww,
        const double d_x_chi, const double d_z_chi,
        const double d_x_alpha, const double d_z_alpha,
        const double reduced_determinant,
        const CheckedHxz::CheckedHxzOverXIngredient &hxz_over_x,
        const CheckedW::CheckedDxhwwOverXIngredient &dxhww_over_x,
        const CheckedChi::CheckedDxchiOverXIngredient &dxchi_over_x,
        const CheckedAlpha::CheckedDxalphaOverXIngredient &dxalpha_over_x)
        : m_x(x), m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz),
          m_h_ww(h_ww), m_chi(chi), m_d_x_h_ww(d_x_h_ww),
          m_d_z_h_ww(d_z_h_ww), m_d_x_chi(d_x_chi),
          m_d_z_chi(d_z_chi), m_d_x_alpha(d_x_alpha),
          m_d_z_alpha(d_z_alpha), m_reduced_determinant(reduced_determinant),
          m_hxz_over_x(hxz_over_x), m_dxhww_over_x(dxhww_over_x),
          m_dxchi_over_x(dxchi_over_x), m_dxalpha_over_x(dxalpha_over_x)
    {
    }

    friend PhysicalHiddenLapseHessianInputs
    make_physical_hidden_lapse_hessian_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double chi, double d_x_h_ww, double d_z_h_ww, double d_x_chi,
        double d_z_chi, double d_x_alpha, double d_z_alpha);
    friend PhysicalHiddenLapseHessian compute_physical_hidden_lapse_hessian(
        const PhysicalHiddenLapseHessianInputs &inputs);

    double m_x;
    double m_h_xx;
    double m_h_xz;
    double m_h_zz;
    double m_h_ww;
    double m_chi;
    double m_d_x_h_ww;
    double m_d_z_h_ww;
    double m_d_x_chi;
    double m_d_z_chi;
    double m_d_x_alpha;
    double m_d_z_alpha;
    double m_reduced_determinant;
    CheckedHxz::CheckedHxzOverXIngredient m_hxz_over_x;
    CheckedW::CheckedDxhwwOverXIngredient m_dxhww_over_x;
    CheckedChi::CheckedDxchiOverXIngredient m_dxchi_over_x;
    CheckedAlpha::CheckedDxalphaOverXIngredient m_dxalpha_over_x;
};

inline void require_finite_lapse_hessian_inputs(
    const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double chi, const double d_x_h_ww,
    const double d_z_h_ww, const double d_x_chi, const double d_z_chi,
    const double d_x_alpha, const double d_z_alpha)
{
    const double values[] = {h_xx,       h_xz,       h_zz,      h_ww,
                             chi,        d_x_h_ww,   d_z_h_ww,  d_x_chi,
                             d_z_chi,    d_x_alpha,  d_z_alpha};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "CartoonPhysicalHiddenLapseHessian requires finite local "
                "metric, conformal-factor, and lapse inputs");
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
            "CartoonPhysicalHiddenLapseHessian requires finite positive "
            "reduced determinant");
    }
    return determinant;
}

inline PhysicalHiddenLapseHessianInputs
make_physical_hidden_lapse_hessian_inputs(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double chi, const double d_x_h_ww,
    const double d_z_h_ww, const double d_x_chi, const double d_z_chi,
    const double d_x_alpha, const double d_z_alpha)
{
    // All raw metric/scalar/lapse values, determinant data, and checked
    // quotient ingredients are single-sourced from this one local point. No
    // public overload accepts loose q, p_W, p_chi, p_alpha, determinant, or a
    // precomputed conformal hidden Hessian.
    CartoonAxisPolicy::require_away_axis(x);
    require_finite_lapse_hessian_inputs(
        h_xx, h_xz, h_zz, h_ww, chi, d_x_h_ww, d_z_h_ww, d_x_chi,
        d_z_chi, d_x_alpha, d_z_alpha);
    if (h_ww <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonPhysicalHiddenLapseHessian requires positive h_ww");
    }
    if (chi <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonPhysicalHiddenLapseHessian requires positive chi");
    }

    const double determinant = checked_reduced_determinant(h_xx, h_xz, h_zz);
    const auto hxz_over_x =
        CheckedHxz::compute_checked_hxz_over_x_ingredient({x, h_xz});
    const auto dxhww_over_x =
        CheckedW::compute_checked_dxhww_over_x_ingredient({x, d_x_h_ww});
    const auto dxchi_over_x =
        CheckedChi::compute_checked_dxchi_over_x_ingredient({x, d_x_chi});
    const auto dxalpha_over_x =
        CheckedAlpha::compute_checked_dxalpha_over_x_ingredient(
            {x, d_x_alpha});

    return PhysicalHiddenLapseHessianInputs(
        x, h_xx, h_xz, h_zz, h_ww, chi, d_x_h_ww, d_z_h_ww, d_x_chi,
        d_z_chi, d_x_alpha, d_z_alpha, determinant, hxz_over_x,
        dxhww_over_x, dxchi_over_x, dxalpha_over_x);
}

inline PhysicalHiddenLapseHessian compute_physical_hidden_lapse_hessian(
    const PhysicalHiddenLapseHessianInputs &inputs)
{
    const double x = inputs.m_x;
    const double x2 = x * x;
    const double A = inputs.m_h_xx;
    const double C = inputs.m_h_zz;
    const double W = inputs.m_h_ww;
    const double chi = inputs.m_chi;
    const double D = inputs.m_reduced_determinant;
    const double q = inputs.m_hxz_over_x.hxz_over_x();
    const double p_w = inputs.m_dxhww_over_x.dxhww_over_x();
    const double p_chi = inputs.m_dxchi_over_x.dxchi_over_x();
    const double p_alpha = inputs.m_dxalpha_over_x.dxalpha_over_x();
    const double alpha_z = inputs.m_d_z_alpha;

    const double ell_x = p_w / W - p_chi / chi;
    const double ell_z = inputs.m_d_z_h_ww / W - inputs.m_d_z_chi / chi;

    const double dww_alpha =
        (W / D) * (C * p_alpha - q * alpha_z) +
        (W / (2.0 * D)) *
            (x2 * (C * ell_x * p_alpha - q * ell_x * alpha_z -
                   q * ell_z * p_alpha) +
             A * ell_z * alpha_z);

    if (!std::isfinite(dww_alpha))
    {
        throw std::domain_error(
            "CartoonPhysicalHiddenLapseHessian produced a nonfinite local "
            "result");
    }
    return PhysicalHiddenLapseHessian(dww_alpha);
}

} // namespace CartoonPhysicalHiddenLapseHessian
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONPHYSICALHIDDENLAPSEHESSIAN_HPP */
