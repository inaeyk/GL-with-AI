# Custom Solver versus GRChombo Comparison-Test Plan

Status: executable-test contract with comparison batches 1-4 completed. The
test-only bridges are authorized evidence, not production wiring. The custom
Stage 4AO-C operator is the independent oracle; GRChombo is the convention
and production authority.

## Comparison-phase disposition after dependency qualification

Comparison batches 1-4 are complete:

- Batch 1 establishes exact production/custom state correspondence,
  conformal algebra, contracted connection, and visible physical Ricci.
- Batch 2 establishes selected visible stock-`d=3` CCZ4 Ricci-Z,
  `chi,h,K,Theta,A` family and combined-row equivalence, and distinguishes the
  selected CCZ4 branch from BSSN.
- Batch 3 establishes custom second-order versus GRChombo fourth-order
  derivative convergence, manufactured visible-RHS convergence, Richardson
  continuum agreement, and visible cleanup comparison.
- Batch 4 locks
  `gamma_theta_theta=x^2`, `gamma_ww=hww/chi`, and GP `hww=1`; independently
  decomposes stock-`d=3` and target-`d=4` GP families; validates the complete
  custom analytic GP residual and constraints; and locks the fixed
  lapse-source seam plus Fourier/parity initialization convention.

This is substantial agent-capability evidence, but no final overall score is
assigned before actual evolution runs. The production target-`d=4` 13-row
pointwise seam now passes direct nonlinear comparison. It reports target
hidden-suppressed, subtraction-defined hidden increment, and full target
GRChombo results; the increment is not an independently implemented hidden
RHS. Pointwise hidden-aware cleanup, constraints, and the fixed source now
pass; live BoxLoop and periodic-ownership comparison is next.

Historical provenance remains inferred because the GRChombo-era Chombo SHA is
unavailable. The project-qualified comparison/production tuple is official
GRChombo `37e659523830418b210acea1661dac0e00bb1b75` plus official Chombo
`8684f2e000106f1abadb72642e1d15351867f98f`, detached and clean. Core DIM2
libraries, the target `2/4/4` probe, and the two stock tests pass. Container,
PETSc/AHFinder, MPI, and full-runtime qualification remain separate.

## Fixed comparison contract

Every result record must contain:

```text
test id; custom commit; GRChombo commit; Chombo/container digest;
compiler and flags; CH_SPACEDIM; GR_SPACEDIM; formulation;
kappa1,kappa2,kappa3,covariantZ4; derivative order; dx;
input fixture hash; per-component absolute/relative error; outcome.
```

The initial floating-point acceptance rule for pointwise double-precision
comparisons is

```text
abs(a-b) <= atol + rtol*max(abs(a),abs(b)),
atol=5e-13, rtol=5e-12.
```

Exact integers, slot maps, dimensions, parity tables, signs, rational
coefficients, formulation selections, and parameter values must match exactly.
Changing tolerances requires a documented numerical-error study before the
test is run, not after a failure.

For a quantity expected to vanish, report its absolute norm and compare to the
declared absolute tolerance; do not manufacture a relative error. For a family
sum, compare every family separately before comparing the sum so cancellation
cannot hide ownership errors.

## Level 1: formulas and conventions

| Test | Inputs | Compared evidence | Acceptance |
|---|---|---|---|
| L1-01 dependency manifest | Clean checkouts and build configuration | Top-level custom commit, exact GRChombo commit, container/Chombo identity, compiler flags | Every field populated and reproducible; no floating branch/tag |
| L1-02 state map | Distinct sentinel value per custom and production slot | Names, indices, tensor symmetry, hidden representative, gauge slots | Exact bijection for shared slots; hidden/gauge-only slots explicitly classified |
| L1-03 dimensions | Build manifest | `CH_SPACEDIM`, `GR_SPACEDIM`, tensor loop dimension, trace denominator, hidden multiplicity | Exact target values `2,4,4,4,2`; compilation must expose them unambiguously |
| L1-04 conformal convention | Symbolic formula table | `gamma=chi^-1 h`, chi exponent, inverse, determinant, K/A reconstruction | Exact algebra after the dimension adapter |
| L1-05 CCZ4 branch | Parameter/formulation table | `USE_CCZ4` versus `USE_BSSN`, cosmological constant, covariant Z4 | Exact selected branch; rejected BSSN K-row mutation fails |
| L1-06 signs and coefficients | Per-family coefficient ledger | K sign, Ricci signs, Z coefficient, advection/stretching, hidden multiplicity | Exact equality; every sign/multiplicity mutation fails |
| L1-07 damping/gauge | Parsed parameters | kappas, covariantZ4, lapse/shift coefficients, eta, GP source | Exact values and source ownership; no default silently substituted |
| L1-08 parity/Fourier | Even/odd variable table and compact period | Intrinsic z parity, cosine/sine mapping, `k=2 pi n/L` | Exact table and mode normalization |

## Level 2: pointwise analytic-jet RHS

Use deterministic nonzero analytic jets with positive `chi`, positive-definite
conformal metric, nonzero `hxz`, nonzero hidden perturbations, nonzero first and
second derivatives, and nonunit lapse. Avoid trace or determinant constraints
unless the test explicitly targets them. Feed both paths identical values and
derivatives; do not let either path compute its own stencil in Level 2.

| Test | Compared components/families | Acceptance |
|---|---|---|
| L2-01 conformal algebra | determinant, inverse, physical metric, K reconstruction, traces and TF projection | Every component satisfies fixed pointwise tolerance; weighted hidden trace included |
| L2-02 contracted connection/Z | visible contracted Christoffels, hidden addition, hatted Gamma, `Z_over_chi`, lower Z | Compare visible and hidden increments separately; fixed tolerance |
| L2-03 physical Ricci | `Rxx,Rxz,Rzz`, then adapted `Rww`, scalar and TF parts | Every raw component and trace meets tolerance; multiplicity mutation rejected |
| L2-04 encoded Z | raw Ricci, encoded-Z tensor increment, combined Ricci-Z | Each stage meets tolerance independently |
| L2-05 chi/metric RHS | advection, stretching, algebraic coupling, complete rows | Per-family and complete rows meet tolerance |
| L2-06 K/Theta/A RHS | Ricci, encoded Z, algebraic, damping, advection/stretching | Per-family and every output component meet tolerance; BSSN mutation fails |
| L2-07 hatted-Gamma RHS | Z/kappa, gradients, connection-A, vector Hessian, grad-div, advection | Per-family and complete x/z rows meet tolerance; parity signs exact |
| L2-08 direct nonlinear target comparison and directional diagnostic | Deterministic finite target states plus the declared central-difference epsilon sweep | Direct full target GRChombo versus the custom analytic oracle must meet the fixed tolerance for all 13 rows and is the sole numerical completion gate. The JVP sweep is reported only as a roundoff/cancellation-dominated diagnostic; no epsilon, convergence order, or locked-tolerance JVP pass is required |
| L2-09 GP background | Analytic GP values/derivatives with and without fixed lapse source | Geometric rows vanish within tolerance; unmodified lapse is `-3 lambda`; source-adjusted lapse vanishes |
| L2-10 constraint equations | Hamiltonian and momentum, raw and hidden increments | Componentwise tolerance and exact hidden multiplicity |

## Level 3: discrete manufactured profiles

Use the same physical functions on nested uniform grids. The custom harness
may keep its second-order stencil; GRChombo uses its production fourth- or
sixth-order derivative class. Compare each implementation to the analytic
profile and compare the RHS after accounting for each stencil's truncation
error.

| Test | Profiles and outputs | Acceptance |
|---|---|---|
| L3-01 radial derivatives | Polynomials through stencil exactness plus smooth nonpolynomial profiles | Custom observed order `>=1.8`; GRChombo fourth `>=3.5` or sixth `>=5.5` away from boundaries |
| L3-02 periodic z/Fourier | Single and mixed sine/cosine modes crossing periodic wrap | Expected derivative order; forbidden parity/mode leakage `<=1e-12` for exactly represented sampled modes |
| L3-03 mixed derivatives | Nonseparable smooth `f(x,z)` | Both `D_xD_z` and `D_zD_x` converge; commutator follows the declared truncation envelope |
| L3-04 manufactured full RHS | Nonzero values/first/second derivatives activating every row | Every visible advection row and each complete chi/metric/A shift RHS family is monitored separately with component and physical location; each error converges at its stencil order; ownership mutations fail |
| L3-05 algebraic cleanup | Off-determinant/off-trace profiles | Projected determinant and weighted trace residual `<=1e-12`; projection idempotent; physical slots otherwise preserved |
| L3-06 constraints | Constraint-satisfying and deliberately violating profiles | Analytic residual agreement and expected convergence; violating sign/multiplicity mutations detected |
| L3-07 GP stationarity | Uniform GP data on nested grids | Raw geometric residual converges at production order; source-adjusted lapse converges to zero without hiding raw residual |

## Level 4: physical runs

Physical runs begin only after all applicable Levels 1-3 pass and the
black-string production path replaces smoke-only hidden freezing.

| Test | Compared outputs | Acceptance |
|---|---|---|
| L4-01 unperturbed GP | State drift, constraints, determinant/trace, horizon position, `R_H` | Converges toward stationarity with refinement; no secular continuum drift over the declared validation window |
| L4-02 single small Fourier perturbation | Mode amplitude, leakage, constraints, horizon shape | Linear response scales with input amplitude; forbidden harmonics remain within declared truncation/noise envelope |
| L4-03 early growth/decay | Independent field and `R_H` amplitudes | Fitted rate stable under fit-window and resolution changes; two observables agree within combined uncertainty |
| L4-04 constraint convergence | Hamiltonian/momentum norms across resolutions | Expected production order before AMR/interpolation saturation; no hidden-sector floor |
| L4-05 horizon observables | MOTS residual, `R_H(t,z)`, minimum radius, area | Uniform solution matches analytic values; perturbed observables converge under grid and AH resolution |
| L4-06 restart/parallel invariance | Continuous versus restarted, rank/thread variants | State and diagnostics agree within roundoff/reduction-order tolerance; fitted growth uncertainty unchanged |

## First five executable comparisons

These are the first tests to implement after this documentation phase:

1. **L1-01 dependency manifest**: pin and emit the exact GRChombo, Chombo,
   container, compiler, dimension, and formulation identities.
2. **L1-02/L1-03 slot-and-dimension map**: distinct-value mapping between the
   13-state oracle, 27-slot BlackStringToy layout, and GRChombo tensor structs.
3. **L2-01 conformal algebra comparison**: determinant, inverse, trace,
   trace-free projection, and K reconstruction on deterministic nonzero data.
4. **L2-02 contracted-connection/Z comparison**: compare visible GRChombo
   reconstruction first, then the custom hidden contribution as a separate
   adapter increment.
5. **L2-03 physical-Ricci comparison**: identical analytic metric jets through
   GRChombo `compute_ricci` and the custom visible Ricci oracle, before adding
   the independently tested `Rww` production adapter.

These tests deliberately precede any time evolution, horizon solve, or growth
fit. They isolate conventions and formula ownership before numerical
infrastructure can obscure a mismatch.

## Result record template

| Field | Value |
|---|---|
| Test ID | TBD |
| Custom commit | TBD |
| GRChombo commit | TBD |
| Chombo/container identity | TBD |
| Dimensions/formulation | TBD |
| Fixture and hash | TBD |
| Declared tolerances | TBD |
| Max absolute error and component | TBD |
| Max relative error and component | TBD |
| Observed convergence order | N/A or TBD |
| Mutations executed | TBD |
| Outcome | not run / pass / fail / blocked |
| Evidence artifact | TBD |
| Follow-up owner | TBD |

## Batch 1 result records

The fixed tolerance above was used without modification. The focused fixture
is `code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch1Test.cpp`; the
detailed manifest and case definitions are in
`docs/grchombo/custom_solver_grchombo_comparison_batch1_results.md`.

| Test ID | Directness | Maximum absolute error | Maximum normalized error | Mutations | Outcome |
|---|---|---:|---:|---|---|
| L1-01 | source/environment manifest | N/A | N/A | N/A | partial: commits/compiler/dimensions pass; Chombo/container digests unresolved |
| L1-02 | compiled custom-state and stock-enum headers | exact | exact | stock-only and hidden-slot classifications | pass |
| L1-03 | compile-time assertions | exact | exact | `/4` trace denominator used against compiled `d=3` | pass; production target `d=4/2` remains a separate adapter task |
| L2-01 | direct `TensorAlgebra` | `4.441e-16` | `6.808e-05` | wrong trace dimension | pass |
| L2-02 | direct `compute_christoffel`; source-locked Z formula | `1.837e-14` | `1.036e-02` | connection sign | pass for visible terms; hidden increment custom-only |
| L2-03 | direct `CCZ4Geometry::compute_ricci` | `2.220e-16` | `1.530e-04` | `Rxz` sign | pass for same-dimension visible geometry |

For L2-03, the independent side is a test-only dimension-general
differential-geometry oracle evaluated at the compiled GRChombo dimension.
The production `d=4` custom operator was not changed. For L2-01,
`gamma=h/chi` and `Kij` reconstruction remain source/convention comparisons
because the inspected GRChombo headers expose the data convention but no
standalone callable reconstruction path.

## Batch 2 result records

The batch-2 fixture is
`code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch2Test.cpp`.
It calls the actual inspected GRChombo `CCZ4RHS::rhs_equation` and geometry
templates at stock `d=3`. The test-only custom side independently assembles
the same-dimension families; production `d=4` code is unchanged. Full results
are in
`docs/grchombo/custom_solver_grchombo_comparison_batch2_results.md`.

| Test ID | Directness | Maximum absolute error | Maximum normalized error | Mutations | Outcome |
|---|---|---:|---:|---|---|
| L2-04a raw Ricci | direct `CCZ4Geometry::compute_ricci` | `3.053e-16` including scalar | `1.9970836e-04` | raw/encoded separation controls | pass |
| L2-04b encoded Z | direct `compute_ricci_Z - compute_ricci` | `2.776e-16` including scalar | `2.8691300e-04` | omit, double, wrong TF dimension | pass |
| L1-04c standalone Z map | source/convention reconstruction using a directly computed GRChombo contracted connection | `1.110e-16` | `1.8484162e-04` | wrong lowering, missing chi | pass; no callable standalone GRChombo output exists |
| L2-04d combined Ricci-Z | direct `compute_ricci_Z` | `1.665e-16` tensor; `8.327e-17` scalar | `9.0765382e-05` | omit/double encoded Z | pass |
| L2-05 `chi/h` families | direct `CCZ4RHS::rhs_equation` | `2.776e-17` | `2.803e-05` | wrong dimension | pass; separate shift and trace subterms are source-only, their sum is direct |
| L2-06 `K` families | direct `CCZ4RHS::rhs_equation` | `5.551e-17` by family; `1.110e-16` combined | `8.732e-05` | omission, Hessian sign, BSSN branch | pass |
| L2-07 `Theta` families | direct `CCZ4RHS::rhs_equation` | `2.776e-17` by family; `5.551e-17` combined | `3.152e-05` | Ricci duplication, BSSN branch | pass |
| L2-08 visible `A` families | direct `CCZ4RHS::rhs_equation` | `1.665e-16` by family; `1.943e-16` combined | `1.521e-04` | wrong TF dimension, sign; direct damping absence | pass |
| L2-hidden | custom-only | N/A | N/A | hidden multiplicity remains covered only by custom regressions | production adaptation required |

## Batch 3 result records

The batch-3 fixtures are
`code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch3DerivativeRHSTest.cpp`
and `Stage4AOCGRChomboComparisonBatch3CleanupTest.cpp`. The derivative fixture
calls the actual inspected GRChombo fourth-order scalar kernels and actual
custom second-order helpers. Because their formal orders differ, finite-grid
values are compared to a common analytic reference and only their continuum
extrapolations are compared as equivalent values. Detailed tables are in
`docs/grchombo/custom_solver_grchombo_comparison_batch3_results.md`.

| Test ID | Directness | Custom order | GRChombo order | Finest custom / GR error | Outcome |
|---|---|---:|---:|---:|---|
| L3-01 `D_x,D_xx` | different-order convergence; both kernels direct | `1.9974-1.9975` | `3.9926-3.9933` | up to `1.919e-6` / `6.287e-10` | pass |
| L3-02 periodic `D_z,D_zz` | custom wrap direct; GR kernel direct with local analytic ghosts; Chombo periodic fill blocked | `1.9966-1.9972` | `3.9895-3.9905` | up to `2.827e-6` / `1.217e-9` | pass for kernels |
| L3-03 `D_xz`/divergence | different-order convergence; both kernels direct | `1.9957-1.9972` | `3.9912-3.9920` | up to `2.806e-6` / `1.040e-9` | pass |
| L3-04 manufactured visible RHS | direct discrete kernels plus batch-2 continuum paths; direct `rhs_equation` subtraction for complete shift families | `1.9954-1.9985` componentwise | `3.8618-4.2247` componentwise | combined `3.305e-6` / `1.219e-9` | pass; all 15 advection rows and three complete shift groups pass; extrapolated row difference unchanged at `4.920e-13` |
| L3-05 visible trace cleanup | direct `TraceARemoval`, independent matrix oracle | N/A | N/A | component max `2.776e-17`; post-trace `5.551e-17` | pass |
| L3-05 positivity cleanup | direct `PositiveChiAndAlpha` | N/A | N/A | exact declared clamps; idempotent | pass |
| L3-05 determinant cleanup | no inspected callable stock routine | N/A | N/A | N/A | source-only/absent; hidden-aware cleanup remains adaptation gap |

## Batch 4 result records

The batch-4 fixtures are
`code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch4GPBackgroundTest.cpp`
and `Stage4AOCGRChomboComparisonBatch4GaugeTest.cpp`. Full evidence is in
`docs/grchombo/custom_solver_grchombo_comparison_batch4_results.md`.

| Test ID | Directness | Result | Mutations / limitations | Outcome |
|---|---|---|---|---|
| L1-04/L1-06 GP convention | direct stock `d=3` tensor algebra/RHS plus source/convention target map | coordinate angular `x^2` is distinct from stored custom `hww=1`; custom and stock dimension-specific K/A reconstructions pass | wrong stored `hww=x^2` and reversed extrinsic sign rejected | pass |
| L1-07 gauge/source | direct `MovingPunctureGauge` raw output plus real test-only fixed-source adapter | raw lapse `-3 lambda`; adapter changes lapse only; all 20 evolved-field numerical Jacobian entries are zero | wrong sign/factor, `2K`, horizon-dependent, shift-owner, and B-owner adapters rejected | pass |
| L2-09 full GP background | actual custom-hidden-only `Stage4AOCAnalyticFullOracle` plus independent constraint evaluator | all 13 RHS rows and `H,Mx,Mz` vanish; odd-row and `Mz` injected contributions fail | stock visible `d=3` alone cannot establish full cancellation | pass for custom oracle |
| L2-09 stock/target family seam | independent family builders plus direct stock `rhs_equation` totals | stock K/A fractions reproduced; target K/A totals independently vanish; Ricci, encoded Z, lapse Hessian, and damping are separately zero | stock-K/dK, missing hidden divergence/multiplicity, wrong dimension, false Ricci/Z mutations fail | pass; no residual-negation completion |
| L3-07 GP derivatives | different-order convergence; both kernels direct | custom `D_x/D_xx` orders `2.00019/2.00018`; GR `4.00090/3.96062` | no production Chombo ghost/BoxLoop claim | pass |
| L3-07 visible GP RHS | different-order convergence | custom order `2.00006`; GR clean-plateau order `4.09948`, finest error `2.13052e-11` | full target residual still requires hidden adaptation | pass |
| L3-07 full custom GP preflight | custom second-order stencil study only | pairwise orders `1.81140,1.90359,1.95125` through `N=2048`; worst component/location retained | not used as analytic all-row evidence | pass |
| L1-08 perturbation convention | test-only convention gate | `k_n=2 pi n/L`, all frozen slots and both phase assignments checked; gauge perturbations zero | radial profile remains deliberately unlocked | pass |

The reviewed target-dimension pointwise adaptation seam now passes. Direct
locked source routines execute at target `d=4`; all 13 nonlinear rows match
the independent custom analytic oracle. Hidden-suppressed/full execution and
their subtraction remain observable. The epsilon sweep is classified only as
a roundoff/cancellation-dominated secondary diagnostic: finite-state
production/oracle discrepancies are amplified approximately as `1/epsilon`
in the quotient. Genuine Fourier `P_+`/`P_-` sector checks supplement the
primary direct nonlinear result.

The pointwise hidden-aware cleanup/constraint/source comparison also passes.
For cleanup, the target determinant and weighted trace reach roundoff and the
map is idempotent. For constraints, production directly invokes locked
`CCZ4Geometry::compute_ricci` and then evaluates the exact
`Constraints.impl.hpp` target formula
`H=R+3K^2/4-A_IJ A^IJ` plus its two visible momentum rows. The complete stock
`Constraints` method is source/convention-only at this analytic pointwise
boundary. The target total `H,Mx,Mz` is compared directly with an independent
long-double custom oracle for GP, non-trace-free flat, curved, off-diagonal,
pure-hidden, mixed, and Fourier-consistent `P_+`/`P_-` cases. The fixed source
comparison executes locked
`MovingPunctureGauge` first and verifies lapse-only ownership and zero
evolved-field dependence. Live BoxLoop wiring, periodic ghost ownership, and
GRChombo time integration remain later gates.

Both oracle defects exposed during this comparison are now active negative
controls. The old one-pass raised-Christoffel construction and the old
component-first shift-Hessian interpretation each fail the locked tolerance
on nonzero finite data; the corrected oracle passes those identical states.
