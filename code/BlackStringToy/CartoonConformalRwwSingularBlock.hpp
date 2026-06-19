#ifndef BLACKSTRINGTOY_CARTOONCONFORMALRWWSINGULARBLOCK_HPP
#define BLACKSTRINGTOY_CARTOONCONFORMALRWWSINGULARBLOCK_HPP

#include "CartoonCheckedHxzOverX.hpp"
#include "CartoonSourceFormulaAuthoringGate.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace CartoonConformalRwwSingularBlock
{
// Stage 4Y implements only the checked singular/regularity-sensitive gradient
// sub-block in the Stage 4W conformal hidden Ricci derivation:
// G_sing = H^xx (h_xx - h_ww)/x^2 + H^xz h_xz/x^2
//        = (h_zz/D) Delta_xw - (q_xz^2/D).
// It is not full tilde R_ww, not R^chi_ww, not physical R_ww[gamma], and not
// CCZ4 RHS. It is away-axis only through the checked ingredients it consumes.
static constexpr bool full_conformal_hidden_ricci_implemented = false;
static constexpr bool conformal_factor_ricci_implemented = false;
static constexpr bool physical_ricci_implemented = false;
static constexpr bool ccz4_rhs_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

class ConformalRwwSingularGradientBlock
{
  public:
    double singular_gradient_block() const { return m_singular_gradient_block; }

  private:
    explicit ConformalRwwSingularGradientBlock(
        const double singular_gradient_block)
        : m_singular_gradient_block(singular_gradient_block)
    {
    }

    friend ConformalRwwSingularGradientBlock
    compute_conformal_rww_singular_gradient_block(
        const class ConformalRwwSingularBlockInputs &inputs);

    double m_singular_gradient_block;
};

class ConformalRwwSingularBlockInputs
{
  public:
    double h_xx() const { return m_h_xx; }
    double h_xz() const { return m_h_xz; }
    double h_zz() const { return m_h_zz; }
    double reduced_determinant() const { return m_reduced_determinant; }
    double delta_xw() const
    {
        return m_diagonal_inputs.checked_geometry().hxx_minus_hww_over_x2();
    }
    double q_xz() const { return m_hxz_over_x.hxz_over_x(); }

    const CartoonSourceFormulaAuthoringGate::RegularitySensitiveSourceInputs &
    checked_diagonal_inputs() const
    {
        return m_diagonal_inputs;
    }
    const CartoonCheckedHxzOverX::CheckedHxzOverXIngredient &
    checked_hxz_over_x() const
    {
        return m_hxz_over_x;
    }

  private:
    ConformalRwwSingularBlockInputs(
        const double h_xx, const double h_xz, const double h_zz,
        const double reduced_determinant,
        const CartoonSourceFormulaAuthoringGate::RegularitySensitiveSourceInputs
            &diagonal_inputs,
        const CartoonCheckedHxzOverX::CheckedHxzOverXIngredient &hxz_over_x)
        : m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz),
          m_reduced_determinant(reduced_determinant),
          m_diagonal_inputs(diagonal_inputs), m_hxz_over_x(hxz_over_x)
    {
    }

    friend ConformalRwwSingularBlockInputs
    make_conformal_rww_singular_block_inputs(
        double x, double h_xx, double h_xz, double h_zz, double h_ww);

    double m_h_xx;
    double m_h_xz;
    double m_h_zz;
    double m_reduced_determinant;
    CartoonSourceFormulaAuthoringGate::RegularitySensitiveSourceInputs
        m_diagonal_inputs;
    CartoonCheckedHxzOverX::CheckedHxzOverXIngredient m_hxz_over_x;
};

inline void require_finite_metric_inputs(const double h_xx, const double h_xz,
                                         const double h_zz)
{
    const double values[] = {h_xx, h_xz, h_zz};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "CartoonConformalRwwSingularBlock requires finite reduced "
                "metric entries");
        }
    }
}

inline double checked_reduced_determinant(const double h_xx, const double h_xz,
                                          const double h_zz)
{
    require_finite_metric_inputs(h_xx, h_xz, h_zz);

    const ConformalCartoonAlgebra::ConformalMetric h{h_xx, h_xz, h_zz, 1.0};
    const double determinant = ConformalCartoonAlgebra::reduced_determinant(h);
    if (!std::isfinite(determinant) ||
        determinant <= ConformalCartoonAlgebra::algebra_zero_tolerance)
    {
        throw std::domain_error(
            "CartoonConformalRwwSingularBlock requires finite positive "
            "reduced determinant");
    }
    return determinant;
}

inline ConformalRwwSingularBlockInputs
make_conformal_rww_singular_block_inputs(const double x, const double h_xx,
                                         const double h_xz, const double h_zz,
                                         const double h_ww)
{
    // The determinant inputs and singular checked ingredients are single-sourced
    // from the same local metric point by this factory. This prevents callers
    // from mixing checked Delta_xw and q_xz produced from one point with a
    // determinant built from another. After Checkpoint B, all conformal Rww
    // sub-blocks use the same reduced-metric policy: finite positive D.
    const double determinant = checked_reduced_determinant(h_xx, h_xz, h_zz);
    const auto checked_diagonal =
        CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(
                {x, h_xx, h_ww, 0.0});
    const auto diagonal_inputs =
        CartoonSourceFormulaAuthoringGate::
            make_regularity_sensitive_source_inputs(checked_diagonal);
    const auto hxz_over_x =
        CartoonCheckedHxzOverX::compute_checked_hxz_over_x_ingredient(
            {x, h_xz});

    return ConformalRwwSingularBlockInputs(
        h_xx, h_xz, h_zz, determinant, diagonal_inputs, hxz_over_x);
}

inline ConformalRwwSingularGradientBlock
compute_conformal_rww_singular_gradient_block(
    const ConformalRwwSingularBlockInputs &inputs)
{
    return ConformalRwwSingularGradientBlock(
        (inputs.h_zz() * inputs.delta_xw() - inputs.q_xz() * inputs.q_xz()) /
        inputs.reduced_determinant());
}

} // namespace CartoonConformalRwwSingularBlock
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONCONFORMALRWWSINGULARBLOCK_HPP */
