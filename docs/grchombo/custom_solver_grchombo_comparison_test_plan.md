# Custom Solver versus GRChombo Comparison-Test Plan

Status: executable-test design only. No comparison code or production wiring
is authorized by this document. The custom Stage 4AO-C operator is the
independent oracle; GRChombo is the convention and production authority.

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
| L2-08 nonlinear JVP | Two mixed perturbation directions and six central-difference epsilons | GRChombo nonlinear finite difference approaches custom analytic JVP; truncation region shows second-order epsilon convergence and best error meets fixed tolerance envelope |
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
| L3-04 manufactured full RHS | Nonzero values/first/second derivatives activating every row | Each RHS error converges at its stencil order; ownership mutations fail |
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
