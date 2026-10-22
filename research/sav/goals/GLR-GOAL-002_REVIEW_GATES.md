# GLR-GOAL-002 — predeclared acceptance and adversarial-review gates

This is the **scientific evaluation rubric**, not permission to run production
simulations or modify numerical formulations.

## What would count as a new mathematical result

| Claim | Minimum independent evidence | Failure mode to detect |
|---|---|---|
| Full n=0 and n=2 backreaction | Derived EOM, independent constraints and Bianchi checks, regular boundaries | Solving only R_zz while ignoring metric constraints |
| Physical nonlinear amplitude | Invariant projection or fully specified gauge and asymptotic clock | Hidden A -> A+c A^3 redefinition changing gamma |
| Horizon regularity | EF/tetrad curvature and bounded physical norm | Apparent Schwarzschild component divergence or cancellation loss |
| Valid finite A interval | Explicit norm and certified/qualified remainder bound | Choosing A=0.1 on intuition or ODE tolerances alone |
| Physical finite-time data | Constraint-satisfying initial slice and specified independent scalar data | Arbitrary temporal switching of linear GL eigenmode |
| Transient separation | Bounded retarded/homogeneous components, causal early window | Mistaking outgoing scalar Cauchy transient for GL-produced radiation |
| Cubic amplitude equation | Projection, adjoint, gauge and radiation continuum control | Equating static Gubser coefficient with dynamical gamma |
| Early radiated fraction | Finite-time news integral with initial-data, amplitude and boundary errors | Extrapolating the settled exponential response to A~1 |
| Early dominates cascade | Separate late-time emission bound or measurement | Assuming radius^2 suppression when multiplicities grow |

## Controls that must be attempted

1. Translate z -> z+L/2 for single-harmonic initial data; zero-mode
   observables must be even in the fundamental amplitude. Check
   nonlinear mode projection and Fourier factors.
2. Change gauge (regular horizon-penetrating vs independent
   reduction) and show unchanged completed scalar news.
3. Distinguish variation of zero-mode mass/tension from scalar
   outgoing news; independent homogeneous data cannot be dropped
   without justified initial/boundary conditions.
4. Positivity/nondegeneracy and constraints at each perturbative order.
5. Manufacture nontrivial pure gauge and a scalar source with
   zero outgoing Green-function overlap to ensure source != flux.
6. Compare a finite-time physical initial data family with at least
   two allowed independent zero-mode scalar-data choices.
7. For n=2, classify below-threshold forced response vs genuinely
   propagating massive radiation; avoid an infrared/large-r Sommerfeld
   interpretation at finite frequency.
8. If evaluating L=16, account for the independently unstable n=2 GL mode.

## Requested quantitative quality

Preselect a reference in which the first-order invariant amplitude is
measured at r=2r0, and a future-regular surface/domain. Report explicit
A_max and Cauchy-data class if one exists. Target <=10% fractional
uncertainty in the *integrated finite-interval radion energy*, including
truncation/remainder, transient, radial ODE/quadrature, gauge extraction
and boundary contributions. This numerical target is a research acceptance
proposal, **not** an approved Michael simulation tolerance.

If a mathematically rigorous bound cannot be obtained, provide a
controlled analytical asymptotic estimate with named constants and
the sharpest feasible empirical convergence evidence; mark it
CONDITIONAL, not VERIFIED. Record every failed closure and any
unbounded constant. If only non-universality is proved, report the
class of admissible initial data for which different independent
zero-mode waves change E_early at fixed growing-mode amplitude.

## Two adversarial audits

Audit 1: source/metric/gauge/constraints/horizon and conserved charges.
Audit 2: physical initial-value problem, energy norms, scalar transients,
first omitted order and claimed amplitude/radiated-energy interval.

Each audit should independently derive or reproduce the specific identity,
include deliberate errors that the tests reject, inspect actual scripts,
and distinguish numerical convergence from rigorous enclosures.
Codex may select distinct agents and methods. No external human review
is implied merely by two AI audits.

## Reports and conclusion

The report must open with a binary answer to:
(1) Was a nonzero finite-A *controlled* interval established?
(2) Was an actual first-bulge radiated fraction established?

Then give the strongest theorem/computation, limitations, artifacts,
numerical/analytic error classes, audit verdicts, exact source SHAs and
one concrete next strategy. Preserve prior failed D16/M2-C evidence.
