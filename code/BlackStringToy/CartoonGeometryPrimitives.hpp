#ifndef BLACKSTRINGTOY_CARTOONGEOMETRYPRIMITIVES_HPP
#define BLACKSTRINGTOY_CARTOONGEOMETRYPRIMITIVES_HPP

#include "CartoonSingularCombinations.hpp"

namespace BlackStringToy
{
namespace CartoonGeometryPrimitives
{
// Stage 4P local helper only. These are away-axis-only cartoon geometry
// primitives. They are not Stage 3I small-axis regularization.
//
// These helpers are not full Ricci or CCZ4 RHS formulas. Future source code
// should use named primitives like these instead of raw 1/x or 1/x^2
// expressions.
static constexpr bool small_axis_regularization_implemented = false;
static constexpr bool full_ricci_or_rhs_formula_implemented = false;

struct LocalInputs
{
    double x;
    double d_x_hww;
};

struct Primitives
{
    // This is an away-axis expression only. Near the axis, this requires the
    // expected even-parity behavior of h_ww. This helper does not implement
    // the finite axis limit.
    double dx_hww_over_x;
};

inline Primitives compute(const LocalInputs &inputs)
{
    return {CartoonSingularCombinations::first_derivative_over_x(
        inputs.d_x_hww, inputs.x)};
}

} // namespace CartoonGeometryPrimitives
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONGEOMETRYPRIMITIVES_HPP */
