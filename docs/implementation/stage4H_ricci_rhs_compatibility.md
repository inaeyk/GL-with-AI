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

## Non-goals

Stage 4H does not:

- implement RHS wiring;
- call Stage 4G from evolution;
- make the Ricci helper read grid data;
- implement hidden-sector RHS terms;
- resolve Gamma-driver or damping behavior;
- implement small-`x` regularization;
- modify public GRChombo.
