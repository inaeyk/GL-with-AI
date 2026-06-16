#ifndef BLACKSTRINGTOY_CARTOONRICCIINTERFACE_HPP
#define BLACKSTRINGTOY_CARTOONRICCIINTERFACE_HPP

#include "ConformalCartoonAlgebra.hpp"

namespace BlackStringToy
{
namespace CartoonRicci
{
// Stage 4F defines the local-value interface for a future cartoon Ricci
// helper. It intentionally does not implement Ricci formulas.
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
static constexpr bool ricci_formulas_implemented = false;

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

struct RicciComponents
{
    double xx;
    double xz;
    double zz;
    double ww;
};

} // namespace CartoonRicci
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONRICCIINTERFACE_HPP */
