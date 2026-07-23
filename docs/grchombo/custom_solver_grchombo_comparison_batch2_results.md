# Custom Solver versus GRChombo Comparison Batch 2

Status: executed direct source and Level-2 pointwise RHS comparison evidence.
This batch adds no production solver functionality, does not alter the custom
physical-`d=4` equations, and does not reopen stationary outer-boundary
research.

## Reproducibility and formulation lock

| Item | Inspected value | Status |
|---|---|---|
| Custom repository start | `921e639dede8a020a34b20d182f43693e757561d` (`Validate custom solver against GRChombo batch 1`) | clean worktree and index |
| GRChombo source | `37e659523830418b210acea1661dac0e00bb1b75` | clean detached HEAD; no source change |
| GRChombo RHS | `Source/CCZ4/CCZ4RHS.hpp` and `CCZ4RHS.impl.hpp`, `rhs_equation` | production implementation compiled and invoked directly |
| Geometry | `Source/CCZ4/CCZ4Geometry.hpp` | `compute_ricci` and `compute_ricci_Z` compiled and invoked directly |
| Formulation | `USE_CCZ4` | selected; an executed `USE_BSSN` negative control is distinguishable |
| Dimensions | `CH_SPACEDIM=3`, `GR_SPACEDIM=3`, `DEFAULT_TENSOR_DIM=3` | stock visible comparison dimension |
| Custom production dimensions | physical `d=4`, gridded `d=2`, hidden multiplicity `2` | unchanged; no `d=4` coefficient is compared as a `d=3` match |
| Cosmological constant | `Lambda=0` | locked zero |
| Damping | `kappa1=0.1`, `kappa2=0`, `kappa3=1`, `covariantZ4=true` | exact inspected parameter tuple; `kappa3` is inactive in the rows compared here |
| Matter | vacuum | no matter source is present in `CCZ4RHS` |
| Gauge | test-only frozen adapter; analytic lapse, shift, and derivatives supplied | gauge-output equations are not compared |
| Compiler flags | `-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror` | focused fixture and affected regressions compile cleanly |
| Acceptance rule | `abs(a-b) <= 5e-13 + 5e-12 max(abs(a),abs(b))` | unchanged predeclared Level-2 tolerance |
| Fixture | `code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch2Test.cpp` | focused batch-2 bridge |
| Fixture SHA-256 | `08d6dd9365cfbe5a068eed240490966e3d70673a084cfdaf8b98cc1b3aa77537` | final validated source |

The test-only adapter suppresses unavailable Chombo cell/derivative
infrastructure at the include boundary, subclasses the inspected
`CCZ4RHS<FrozenGauge,NoDerivatives>`, and exposes its protected
`rhs_equation`. The executed RHS body and geometry routines are the GRChombo
source checkout, not copied coefficients. A full Chombo-grid invocation
remains blocked by the unresolved Chombo/container dependency identity, but
that does not block this direct pointwise production-RHS call.

## Independent same-dimension path

The custom comparison side is a test-only generalized-`d` construction
evaluated at `d=3`. It does not modify the production `d=4` operator. It uses
general tensor operations and the custom term-family decomposition:

- form `gamma_ij=h_ij/chi`, its first and second analytic jets, and physical
  Christoffels;
- compute raw physical Ricci from the defining Christoffel expression;
- reconstruct
  `Z_over_chi^i=(hatGamma^i-tildeGamma^i)/2`,
  `Z^i=chi Z_over_chi^i`, and
  `Z_i=h_ij Z_over_chi^j`;
- compute `D_i Z_j+D_j Z_i`, its physical scalar, and its `d=3`
  trace-free projection;
- assemble each visible `chi`, `h_ij`, `K`, `Theta`, and `A_ij` family
  separately.

All deterministic conformal metric jets are positive definite, nontrivial,
and determinant-one through second order. The comparison contains off-diagonal
and mixed cases. Stock visible `y` components participate in the direct
three-dimensional calculation; custom `hww/Aww` and every hidden/cartoon
increment are excluded.

The standalone Z-index check is a source/convention comparison using a
directly computed GRChombo contracted connection. GRChombo internally
constructs `Z_over_chi^i` and `Z^i` in `CCZ4RHS.impl.hpp`; the fixture locally
reconstructs those quantities from the inspected convention and additionally
forms `Z_i`. No public callable GRChombo output exposes the three standalone
values. This limitation does not apply to the directly compiled
`compute_ricci_Z` tensor/scalar comparison below.

## Geometry and encoded-Z results

| Quantity | Classification | Maximum absolute error | Maximum normalized error | Worst absolute case/component | Result |
|---|---|---:|---:|---|---|
| raw `R^g_ij` | direct compiled GRChombo | `1.943e-16` | `1.8500888e-04` | negative case, `R_zz` | pass |
| raw `R^g` | direct compiled GRChombo | `3.053e-16` | `1.9970836e-04` | positive case, scalar | pass |
| encoded `D_iZ_j+D_jZ_i` | direct compiled GRChombo | `1.943e-16` | `2.8691300e-04` | negative case, `xz` | pass |
| encoded scalar | direct compiled GRChombo | `2.776e-16` | `1.2920294e-04` | negative case | pass |
| encoded trace-free tensor | direct compiled GRChombo | `1.943e-16` | `2.5497924e-04` | negative case, `xz` | pass |
| combined Ricci-Z tensor | direct compiled GRChombo | `1.665e-16` | `9.0765382e-05` | positive case, `zz` | pass |
| combined Ricci-Z scalar | direct compiled GRChombo | `8.327e-17` | `5.0922619e-05` | positive case | pass |
| `Z_over_chi`, `Z^i`, `Z_i` map | source/convention; local reconstruction from directly computed contracted connection | `1.110e-16` | `1.8484162e-04` | positive case, `Z_over_chi^z` | pass |

The direct difference
`compute_ricci_Z(...) - compute_ricci(...)` agrees with the independent
covariant-derivative construction. This separately demonstrates that the
selected GRChombo tensor contains one encoded-Z completion: neither omission
nor double insertion can hide behind the raw geometric Ricci.

## Visible RHS family results

The following maxima include every directly exposed stock visible component.
Zero means bitwise agreement for the exercised values.

| Row and family | Classification | Max absolute | Max normalized | Result |
|---|---|---:|---:|---|
| `chi`: lapse-`K` | direct compiled | `0` | `0` | pass |
| `chi`: shift divergence | direct compiled | `0` | `0` | pass |
| `chi`: advection | direct compiled | `0` | `0` | pass |
| `h`: `-2 alpha A_ij` | direct compiled | `0` | `0` | pass |
| `h`: advection | direct compiled | `0` | `0` | pass |
| `h`: shift stretching plus trace correction | direct compiled | `1.388e-17` | `1.766e-05` | pass |
| `h`: shift stretching and trace correction separately | source/convention only | n/a | n/a | their production sum is direct |
| `K`: Ricci-Z | direct compiled | `5.551e-17` | `2.999e-05` | pass |
| `K`: `K(K-2 Theta)` | direct compiled | `5.551e-17` | `2.854e-05` | pass |
| `K`: damping | direct compiled | `0` | `0` | pass |
| `K`: lapse Hessian | direct compiled | `0` | `0` | pass |
| `K`: advection | direct compiled | `0` | `0` | pass |
| `Theta`: one-half Ricci-Z scalar | direct compiled | `2.776e-17` | `2.361e-05` | pass |
| `Theta`: `K^2` and `A^2` | direct compiled | `0` | `0` | pass |
| `Theta`: `-K Theta` | direct compiled | `1.388e-17` | `1.708e-05` | pass |
| `Theta`: damping | direct compiled | `0` | `0` | pass |
| `Theta`: lapse-gradient/Z | direct compiled | `5.204e-18` | `9.888e-06` | pass |
| `Theta`: advection | direct compiled | `0` | `0` | pass |
| `A`: trace-free Ricci-Z | direct compiled | `1.665e-16` | `9.034e-05` | pass |
| `A`: lapse Hessian | direct compiled | `1.388e-17` | `1.513e-05` | pass |
| `A`: `(K-2 Theta)A_ij` | direct compiled | `6.939e-18` | `9.160e-06` | pass |
| `A`: quadratic `A` | direct compiled | `0` | `0` | pass |
| `A`: advection | direct compiled | `0` | `0` | pass |
| `A`: shift stretching plus trace correction | direct compiled | `1.735e-18` | `3.213e-06` | pass |
| `A`: shift stretching and trace correction separately | source/convention only | n/a | n/a | their production sum is direct |
| `A`: direct damping | direct compiled | `0` | `0` | absent as required |
| matter and `Lambda` additions | source/convention only | n/a | n/a | locked zero |

The shift pieces are classified separately as source-only because
`rhs_equation` exposes only their sum; the combined source expression is
directly executed. Likewise, zero matter and cosmological-constant terms are
confirmed from the selected vacuum source branch rather than inferred from a
subtraction of two nonzero runs.

## Combined visible rows

| Row | Maximum absolute error | Maximum normalized error | Worst absolute case/component | Result |
|---|---:|---:|---|---|
| `chi` | `5.551e-17` | `2.625e-05` | negative mixed case | pass |
| `h_ij` | `2.776e-17` | `2.803e-05` | positive mixed case, `yy` | pass |
| `K` | `1.110e-16` | `8.732e-05` | positive mixed case | pass |
| `Theta` | `5.551e-17` | `3.152e-05` | positive mixed case | pass |
| `A_ij` | `1.943e-16` | `1.521e-04` | positive mixed case, `zz` | pass |

Family isolation is acceptance evidence in its own right; the combined-row
passes are not used to excuse an incorrect family through cancellation.

## Negative controls and ownership

Every executed mutation changes nonzero active data and is detected:

| Mutation | Result |
|---|---|
| omit encoded Z | detected |
| insert encoded Z twice | detected |
| source/convention reconstruction: lower `Z` with the wrong upper vector | detected |
| source/convention reconstruction: omit the conformal factor in `Z^i` | detected |
| use `d=4` rather than `d=3` in the trace-free projection | detected |
| omit the `K` damping family | detected |
| duplicate the `Theta` Ricci family | detected |
| flip the lapse-Hessian sign | detected |
| use the `d=4` `chi` coefficient | detected |
| replace selected CCZ4 with BSSN | detected |

The wrong-lowering and missing-chi controls test the fixture's local
source/convention reconstruction, not a standalone value returned by
GRChombo. Encoded-Z omission/duplication and the trace-free-dimension control
continue to exercise the directly compiled Ricci-Z comparison.

The comparison therefore establishes visible, same-dimension equivalence for
the exercised CCZ4 families. It does not establish equivalence of complete
state arrays, `Gamma` evolution, live gauge equations, modified-cartoon
terms, or the target `d=4` production path.

## Hidden-sector gap and disposition

Stock GRChombo at the inspected commit contains no `hww/Aww` production
variables or modified-cartoon hidden contribution. Required future adaptation
still includes:

- representative `hww` and `Aww` evolution rows;
- hidden contributions to contracted connection and encoded Z;
- hidden physical Ricci and lapse Hessian;
- multiplicity-two scalar traces and `d=4` trace-free projections;
- hidden-aware determinant and trace cleanup.

The complete custom thirteen-variable frozen interior remains the oracle for
those terms. No custom production equation changed. The stationary outer
boundary remains deferred research, and no boundary, eigensolver, MOTS, or
production gate is reopened.

## Recommended batch 3

Comparison batch 3 should move from supplied analytic jets to discrete
manufactured profiles while retaining the same visible/hidden separation:

1. directly exercise GRChombo fourth-order first, second, and mixed derivative
   paths on periodic smooth profiles;
2. compare discrete visible RHS values against the analytic-jet oracle and
   verify the declared convergence order;
3. test periodic-`z` wrapping and tensor-component ownership;
4. compare stock algebraic cleanup with the visible oracle and explicitly
   demonstrate the missing hidden-weighted cleanup;
5. defer target-`d=4` and modified-cartoon production comparison until the
   dimension/source adaptation owner is implemented and reviewed.
