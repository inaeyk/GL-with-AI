# Stage 4H: Ricci Helper To CCZ4 RHS Compatibility

Status: planning and compatibility decision only. This note does not wire the
Stage 4G Ricci helper into the RHS, does not change evolution, and does not
modify `external/GRChombo`.

## Purpose

Stage 4G implemented the first local, away-axis cartoon Ricci helper using the
metric-derivative form validated by Stages 3C-3E and the Stage 3G sheared-flat
gate. Its inputs are local values of `h`, `chi`, their first derivatives, their
second derivatives, the reduced coordinate `x`, and hidden multiplicity.

The current GRChombo-facing scaffold still evaluates the inherited public CCZ4
RHS. That path may not consume Ricci in the same form as the Stage 4G helper.
Stage 4H records the compatibility decision before any RHS wiring starts.

## Inspected Code Paths

Repo-owned scaffold:

- `code/BlackStringToy/BlackStringToyLevel.cpp`
- `code/BlackStringToy/BlackStringToyLevel.hpp`
- `code/BlackStringToy/TPAMR.hpp`
- `code/BlackStringToy/UserVariables.hpp`
- `code/BlackStringToy/CartoonRicciInterface.hpp`

Public GRChombo CCZ4 headers, read-only:

- `external/GRChombo/Source/CCZ4/CCZ4RHS.hpp`
- `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`
- `external/GRChombo/Source/CCZ4/CCZ4Geometry.hpp`
- `external/GRChombo/Source/CCZ4/CCZ4Vars.hpp`
- `external/GRChombo/Source/CCZ4/CCZ4UserVariables.hpp`

## Current RHS Path

`BlackStringToyLevel::specificEvalRHS` calls the inherited public
`CCZ4RHS<MovingPunctureGauge, FourthOrderDerivatives>` or
`CCZ4RHS<MovingPunctureGauge, SixthOrderDerivatives>`, depending on the
configured derivative order.

Inside public `CCZ4RHS`, the per-cell compute path loads CCZ4 variables,
computes first and second derivatives, and calls `rhs_equation`. The Ricci
quantity is produced by

```text
CCZ4Geometry::compute_ricci_Z(vars, d1, d2, h_UU, chris, Z_over_chi)
```

and is then consumed by the `A_ij`, `Theta`, and `K` RHS blocks.

The inspected `CCZ4Geometry::compute_ricci_Z` path is not the same as the
Stage 4G helper contract. It uses conformal metric derivatives, Christoffels,
`vars.Gamma`, `d1.Gamma`, and `Z_over_chi`. The comments in GRChombo describe
this as a Ricci-hat construction using the evolved hatted Gamma variable to
avoid adding terms that cancel later. In project language, this is the
Gamma-based GRChombo-facing Ricci path, not the pure local
metric-derivative cartoon Ricci path from Stage 4G.

## Compatibility Problem

Stage 4G returns local physical Ricci components

```text
R_xx, R_xz, R_zz, R_ww
```

computed from metric values and metric derivatives. Public GRChombo CCZ4
computes and consumes a Ricci object inside a larger RHS expression that also
contains the hatted connection and Z4 bookkeeping.

Therefore Stage 4G output must not be wired directly into the inherited
`CCZ4RHS` path until a repo-owned compatibility bridge states exactly:

- whether the live RHS will consume physical Ricci components or conformal
  Ricci pieces;
- where `Z^i` / `hat_Gamma^i` terms enter;
- how hidden multiplicity and `R_ww` enter scalar traces and trace-free
  projections;
- how small-`x` regularity is handled before evaluating terms involving `x`;
- whether the future RHS is a repo-owned metric-derivative RHS path or an
  adaptation of the GRChombo Gamma-based path.

## Future Route Options

### Option A: Use Stage 4G As The Repo-Owned Ricci Source

Use the Stage 4G metric-derivative helper as the source of Ricci components in
a future repo-owned modified-cartoon RHS path.

Pros:

- follows the Stage 3 symbolic checks directly;
- gives direct control over hidden `hww` and `R_ww` terms;
- makes the local test oracles from Stage 4G directly relevant to the live
  implementation.

Cons:

- does not match the inherited GRChombo Gamma-based Ricci construction;
- requires a careful repo-owned RHS compatibility layer before wiring;
- must separately account for hatted-Gamma/Z4 terms and trace-free projections.

### Option B: Adapt Cartoon Terms Into The GRChombo Gamma-Based Form

Rewrite or derive the hidden cartoon Ricci pieces in the same form expected by
GRChombo's `CCZ4Geometry` path.

Pros:

- closer to the existing public CCZ4 structure;
- may reduce the amount of repo-owned RHS replacement needed.

Cons:

- requires a new derivation and validation layer;
- Stages 3C-3E did not directly validate that Gamma-based cartoon form;
- a mismatch between hidden-sector Gamma terms and metric-derivative oracles
  could be difficult to diagnose.

### Option C: Keep Stage 4G As An Oracle Only

Use Stage 4G only as an independent local check while implementing the live RHS
in a separate GRChombo-style form.

Pros:

- keeps Stage 4G useful as a regression oracle;
- avoids prematurely committing to a metric-derivative live RHS.

Cons:

- means maintaining two Ricci paths;
- requires explicit equivalence or reference tests between the oracle and the
  live RHS path.

## Recommendation

Use Stage 4G as the checked metric-derivative Ricci source for local tests and
future repo-owned RHS experiments, but do not wire it into evolution yet. The
next stage should define a local Ricci-to-RHS contract test that consumes the
Stage 4G outputs in the same shape the future repo-owned RHS block expects,
without calling that code from evolution.

This keeps the Stage 3/4G symbolic and C++ oracles active while avoiding a
silent mix of metric-derivative Ricci with GRChombo's Gamma-based Ricci path.

## Next Stage

Recommended next stage, now represented by the Stage 4I bridge contract:

```text
Stage 4I: local Ricci-to-RHS contract test, still no evolution wiring.
```

Stage 4I defines the local data contract for feeding `R_xx`, `R_xz`, `R_zz`,
and `R_ww` toward future RHS blocks. It must not call the helper from
`CCZ4RHS`, must not write to grid data, and must not claim physical evolution
correctness.

After review, Stage 4I also makes the Stage 4G cartoon Ricci result opaque:
the raw component storage is private, and the bridge exposes a distinct
RHS-facing component type. This prevents ordinary future code from reading
`R_xx`, `R_xz`, `R_zz`, or `R_ww` directly and accidentally dropping the
off-diagonal factor or hidden `R_ww` multiplicity. Future RHS use must cross
the Stage 4I bridge or a reviewed replacement.

Stage 4J refines that boundary into a local Ricci-to-RHS contract. It defines
the bridge-approved Ricci view and local algebra inputs a future repo-owned RHS
source block may consume, but it still does not implement RHS formulas or call
evolution code.

Stage 4K then adds the local source-block skeleton that consumes the Stage 4J
contract type and names future RHS output slots. It remains an inert container
only: no physical source formulas or live evolution wiring are implemented.

Stage 4L adds the first narrow local formula block inside that source-block
layer: the 4D trace-free projection of bridge-approved Ricci components. It
uses the full trace with off-diagonal `xz` and hidden `ww` multiplicity factors
and the `/4` projection denominator, but it is still not the full CCZ4 RHS and
is not called from evolution.

Stage 4M adds a named away-axis policy object for local cartoon/RHS
expressions. It centralizes the finite `x > 0` guard and the guarded `1/x` and
`1/x^2` helpers that later source terms may need. It does not clamp `x`,
replace the axis with an epsilon, implement Stage 3I small-axis
regularization, or make axis evaluation physically valid.

Stage 4N adds guarded away-axis singular-combination helpers for local
cartoon expressions such as `d_x f / x` and `(f - g) / x^2`. These helpers
route through the Stage 4M axis policy, reject nonfinite inputs, and provide a
single local path for future singular-looking source-term factors. They do not
implement small-axis regularization or add any physical RHS term.

Stage 4O locks in the current axis-regime semantics before those singular
helpers are used by real source terms. The only implemented regime is
away-axis-only: finite positive `x` is accepted, while `x = 0`, negative `x`,
NaN, and infinity are rejected. No clamp, epsilon substitution, or axis
regularization is implemented. The guarded `1/x^2` helper is a separately
guarded away-axis primitive so any future clamped or regularized regime must
decide the `1/x` and `1/x^2` meanings independently.

Stage 4P adds the first named away-axis cartoon geometry primitives, including
`(d_x hww) / x` and `(hxx - hww) / x^2`. These primitives route through the
Stage 4N singular-combination helpers and inherit the Stage 4O away-axis-only
semantics. They are local building blocks only: no full Ricci tensor, CCZ4 RHS
source term, small-axis regularization, grid read, or evolution wiring is
implemented.

For finite axis behavior, `(hxx - hww) / x^2` additionally requires the Stage
3I matching condition `hxx - hww = O(x^2)`. Stage 4P does not check that
condition on grid data; away-axis evaluation can still return a large finite
value when matching is violated. A later regularity/matching guard must check
or construct that behavior before this primitive is used in a real source
block near the axis.

Stage 4Q adds that first local matching guard. It checks finite inputs, uses
the Stage 4O away-axis policy for `x`, and rejects clear pointwise violations
of `hxx - hww = O(x^2)` using a named residual tolerance. This prepares for
safer future use of `(hxx - hww) / x^2` near the axis, but it does not prove
analytic regularity from one point, implement finite axis limits, add a
physical RHS term, or touch evolution.

Stage 4R adds the first local source-style sub-block that combines these
pieces. It calls the Stage 4Q matching guard before packaging the Stage 4P
away-axis primitive and exposes the matching residual alongside
`(d_x hww) / x` and the guarded source-facing `(hxx - hww) / x^2`. After the
Stage 4R review patch, raw Stage 4P no longer exposes `(hxx - hww) / x^2` as
an ordinary public output; source-facing use must go through the Stage 4R
guarded path. This is still only a future source-block boundary/helper, not a
full Ricci tensor, not a CCZ4 RHS formula, not Stage 3I small-axis
regularization, and not wired into evolution.

Stage 4S connects that Stage 4R guarded package into the existing local RHS
source-block skeleton. The source-block layer now has a narrow local pathway
that carries the guarded geometry ingredients as a nested result, so
source-block consumers receive the risky `(hxx - hww) / x^2` ingredient only
after the Stage 4R helper has crossed the Stage 4Q matching guard. This still
does not implement full Ricci, a CCZ4 RHS formula, grid reads, finite
differences, or live evolution wiring.

After the Stage 4S review patch, the guarded geometry package and the
source-block carry output are checked-by-construction rather than open
aggregates. Callers can read the checked values through accessors, but they
cannot brace-initialize fake trusted values. The former public `detail` helper
for the risky metric-difference value was removed; Stage 4R now computes that
value internally only after calling the Stage 4Q guard.

Stage 4T adds the first local consumer of that checked package. It is a
diagnostic/probe consumer, not a physics formula: it accepts only the
non-forgeable Stage 4R guarded geometry package, mirrors the checked
ingredients, and reports whether the carried matching residual is within the
Stage 4Q tolerance. It does not derive or implement a Ricci sub-block, CCZ4 RHS
term, grid read, finite-difference stencil, or evolution path.

Stage 4U adds the source-formula authoring gate for this boundary. Future
source formulas that need the regularity-sensitive `(hxx - hww) / x^2`
ingredient must accept either the checked `RegularityGuardedGeometrySources`
package or the Stage 4U authoring-gate input that carries it. Direct source
formula use of `difference_over_x2(h_xx, h_ww, x)` for this metric
matching-sensitive ingredient is forbidden. The generic helper remains
available for generic away-axis combinations, but it is not the source-facing
route for this metric difference. This last rule is convention-only in the
current C++ code: the typed authoring path is enforced, but a future formula
author could still hand-write the arithmetic or call the generic helper unless
review or a later lint/CI stage catches it.

Stage 4V records the derivation-lock gap before the first real authoring-gate
consumer. No source formula was added because the existing validated pieces do
not expose a tiny, named expression that consumes only the Stage 4U checked
geometry package. Stage 4G validates the complete local metric-derivative Ricci
helper from full metric/derivative inputs, while Stage 4L consumes already
formed Ricci components for a trace-free projection. Before a Stage 4U consumer
can be physics-bearing, the project must derive or extract the precise Ricci or
RHS sub-expression, including coefficients and sign convention, that uses the
checked `(hxx - hww) / x^2` ingredient.

Stage 4W adds that derivation-lock target for the hidden-sphere Ricci sector in
`docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`. The first
serious hidden Ricci/source target is the physical CCZ4 input
`R_ww[gamma]`, because it feeds `A_ww` directly and also enters the scalar
trace used by `K`, `Theta`, and trace-free projections. The note now separates
the conformal warped-product target
`tilde{R}_ww[h] = (1 - rho D^a D_a rho - D^a rho D_a rho) / x^2`, the
`d = 4` conformal-factor correction `R^chi_ww`, and the physical input
`R_ww[gamma] = tilde{R}_ww[h] + R^chi_ww`. It records a future varying-`chi`
oracle with `chi = 1 + a x`, pins the curvature/lapse sign structure against
the Stage 3A `K_IJ` convention caveat, and keeps the Stage 4G flat,
constant-cone, and nonconstant-hidden-metric oracles as future code anchors.
It also derives the needed regularity condition from first-principles parity:
reflection about the cartoon axis requires `h_xz = O(x)`, and the expansion
exposes this through the `h^xz h_xz / x^2` term. Full `R_ww[gamma]`
implementation is therefore blocked until a checked local `h_xz / x`
ingredient exists. A stronger grid-level or two-sided validation of
`h_xz = O(x)` remains separate future work.

Stage 4X adds that source-facing checked quotient as a local `h_xz / x`
ingredient. The checked package is minted through the existing away-axis
policy, rejects invalid axis and nonfinite inputs, allows finite nonzero
quotients such as `h_xz = 3x`, and is not an open aggregate. It is a local
source ingredient for future formulas only: it does not implement
`tilde{R}_ww[h]`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS,
finite-axis regularization, grid reads, or evolution wiring, and it does not
prove global grid parity or the analytic statement `h_xz = O(x)`.

Stage 4Y adds the first real guarded formula sub-block from the conformal
hidden Ricci target. `CartoonConformalRwwSingularBlock.hpp` computes only

```text
G_sing = (h_zz / D) Delta_xw - q_xz^2 / D,
```

where `Delta_xw` is consumed through the Stage 4U checked metric-difference
package and `q_xz` is consumed through the Stage 4X checked quotient package.
After review, those checked singular ingredients and the raw determinant data
are single-sourced by a non-forgeable input package minted from one local
metric point, closing the raw/checked mismatch advisory. Stage 4Y validates
finite nonzero reduced determinant `D = h_xx h_zz - h_xz^2`, but it does not
implement full `tilde{R}_ww[h]`, `R^chi_ww`, physical
`R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, or evolution wiring.

The integrated hidden-sphere Ricci roadmap is
`docs/implementation/stage4_hidden_sphere_Rww_plan.md`. It keeps physical
`R_ww[gamma]` blocked until the conformal sub-blocks, `R^chi_ww`, a
split-vs-direct physical Ricci identity test, and true `h_xz` parity
validation are complete. It also assigns concrete future ownership for the
high-risk `hat_Gamma^x` hidden contraction and GL-growth/dispersion anchor.

Stage 4Z adds the checked local `W_x / x` ingredient and implements only the
first-derivative gradient sub-block

```text
G_grad = -(C / D) p_W + (q_xz W_z) / D
         - (C W_x^2 - 2 B W_x W_z + A W_z^2) / (4 W D).
```

The Stage 4Z inputs are single-sourced from one local metric point and consume
checked `q_xz` and checked `p_W`. This still does not implement the Hessian
block, full `tilde{R}_ww[h]`, `R^chi_ww`, physical `R_ww[gamma]`, CCZ4 RHS,
grid reads, or evolution wiring.

Stage 4AA locks the next conformal sub-block as documentation only. The
Hessian block is

```text
G^Hess_ww =
    -(sqrt(W) / x) [
        (C / D) H_xx
      - (2 B / D) H_xz
      + (A / D) H_zz
    ],
```

with `H_ab = rho_ab - Gamma^x_ab rho_x - Gamma^z_ab rho_z` and the reduced
Christoffels recorded in the Stage 4W derivation note and hidden-sphere
roadmap. The locked primary oracles are flat `0`, constant cone `0`, and
nonconstant `W = (1 + x)^2` at `x = 1` giving `G^Hess_ww = -4`. Combined with
the reviewed Stage 4Y and 4Z values for that fixture, the conformal target is
`tilde R_ww = -12`.

Stage 4AA still does not implement code. Claude Audit A verifies the distinct
nonsymmetric Hessian sample with all required derivative slots nonzero:
`G^Hess_ww = -8558 / 2883`, approximately `-2.9684356573014221`. For the same
sample, the full conformal sum is `-3576 / 961`, approximately
`-3.7211238293`, matching the independent Stage 4G conformal Ricci engine to
machine precision with residual about `4.44e-16`. This oracle is required for
Stage 4AB because it exercises off-diagonal Christoffels, `rho_xz`,
`W_z`-dependent terms, and the `(-2B/D)` contraction; the simpler oracles are
not sufficient by themselves.

Stage 4AB implements only that local conformal Hessian sub-block in
`CartoonConformalRwwHessianBlock.hpp`. It uses a single-source non-forgeable
input package for the local metric values and derivatives, rejects invalid
axis, nonpositive `W`, bad determinant, and nonfinite derivative inputs, and
does not accept precomputed determinant or Christoffel values. The fixture
includes the verified nonsymmetric Hessian oracle and a test-only sum with the
reviewed Stage 4Y/4Z sub-blocks. Full `tilde{R}_ww[h]` assembly remains
blocked until Stage 4AC.

Stage 4AC assembles local conformal `tilde{R}_ww[h]` from the reviewed
Stage 4Y singular, Stage 4Z gradient, and Stage 4AB Hessian sub-blocks. The
assembler uses a single-source non-forgeable input package and does not accept
loose block values or precomputed determinant data. Its fixture checks the
flat, constant-cone, nonconstant-`W`, and Claude-verified nonsymmetric
oracles, and directly compares the nonconstant and nonsymmetric `chi=1` cases
against the Stage 4G metric-derivative Ricci helper. This is still conformal
Ricci only; `R^chi_ww`, physical `R_ww[gamma]`, RHS, and evolution remain
future work.

Checkpoint B found one cleanup before Stage 4AD: Stage 4Y originally accepted
finite negative reduced determinant values while Stage 4Z, Stage 4AB, and
Stage 4AC required positive determinant. The singular block now uses the same
policy as the rest of the conformal `R_ww` stack: finite positive
`D = AC - B^2` is required, and `D <= 0` rejects before formula evaluation.

## Non-goals

Stage 4H does not:

- implement RHS wiring;
- call Stage 4G from evolution;
- make the Ricci helper read grid data;
- implement hidden-sector RHS terms;
- resolve Gamma-driver or damping behavior;
- implement small-`x` regularization;
- modify public GRChombo.
