# Verification index — scientific foundation

Date: 2026-10-22. Evidence categories:
`CHECKED_SIMPLE_ALGEBRA`, `SOURCE_CONFIRMED`,
`DERIVED_UNAUDITED`, `NOT_ATTEMPTED`, `FAILED`.

## Prepared GLR-GOAL-003 review protocol — not executed

[Goal](../goals/GLR-GOAL-003.md) and
[acceptance/falsification gates](../goals/GLR-GOAL-003_REVIEW_GATES.md).
Preparation reviewed existing reports/derivations and obtained bounded
independent planning reviews of the master/inverse, nonlinear closure and
EFT/symmetry. These are document reviews, not new scientific audits or
executed GOAL-003 checks. Source/reconstruction equivalence, endpoint and
inverse bounds, exact simultaneous GL data, nonlinear radiation trace and
massive scalar transport are specified as future verification obligations.
No scientific code or simulation ran for this preparation. Publication
validation is limited to documentation links, content and Git-tree scope.
Future commands/results belong in a new GLR_GOAL003_CHECKS.md when run.

## Executed GLR-GOAL-002 checks — 2026-10-22

See GLR_GOAL002_CHECKS.md for all commands, versions, output paths and scope.

| Check / claim | Evidence | Limit |
|---|---|---|
| Independent quadratic source and all reduced sources/constraints | goal002_sector_checks.py, full exact local on-shell Bianchi and both harmonics | Analytic specialization is not a physical global GL mode |
| EF zero metric, n2 trace/divergence, full first-order horizon components | AUDIT_GOAL002_GEOMETRY.md, 17 independently implemented checks | n2 boundary inverse/global norm not solved |
| Scalar finite-slice P/Z Green mismatch | goal002_scalar_response.py/JSON, three L10 configurations, Q≈80.8179176 | Empirical, no full interval enclosure or exact GL data completion |
| Exact scalar-only horizon-crossing vacuum KK initial data | AUDIT_GOAL002_ENERGY.md, direct Hamiltonian/momentum constraints and lift | Does not superpose nonlinearly with GL |
| GL leading energy non-universality | Exact homogeneous order-two scalar freedom | Constrained perturbative theorem; exact simultaneous-family extension conditional |
| News energy inequality, regular-slice bound, amplitude convention, smooth spectrum controls | Energy audit: 18 checks, 11 rejected mutations | No physical full spectrum or finite-A error theorem |
| Fully evaluated n2 response / finite-A interval / first-bulge fraction | NOT COMPLETED | Explicit source-tail, constraint-inverse and N4/data-family obstructions |

Two distinct scientific AI agents ran the audits, not external human
referees. All equations/numerical checks are non-production. No extra
control wavelength, physical GL root, n2 grid, 5D evolution or merge.

## Executed GLR-GOAL-001 checks

`EXACT_CHECKED_AND_REVIEWED` means an exact derivation/check plus separate
AI mathematical reviewer; it is not human referee certification.

| Check | Result | Record |
|---|---|---|
| SO(3) action, GHY boundary cancellation, canonical DOF | EXACT_CHECKED_AND_REVIEWED | `AUDIT_FOUNDATION.md`, `audit_foundation_checks.py` |
| Gauge-completed radion, pure-gauge null test | EXACT_CHECKED_AND_REVIEWED | audit I; omitting half Lie² rejected |
| Full GL linear mode and two-gauge conversion | EXACT_CHECKED | `gl_linear_check.py`, `gl_linear_exact.py`, `gl_linear_verification.txt` |
| Continuum GL modes / threshold | EMPIRICAL_CONVERGENCE | `gl_linear_results.json`, `gl_linear_extended_results.json`; no interval proof |
| Complete quadratic scalar source | EXACT_CHECKED_AND_REVIEWED | `check_quadratic_source.py`, both audits, independent cubic action |
| Retarded operator, W sign, static threshold positive overlap | EXACT_CHECKED_AND_REVIEWED | `AUDIT_SOURCE_RETARDED.md`, `audit_source_checks.py` |
| Nonzero source can have exactly zero overlap | EXACT_MANUFACTURED_NEGATIVE | audit II executable; forbids source-only radiation claim |
| Four C0 numerical overlaps | STRONG_NUMERICAL_EVIDENCE | `radion_transfer.py`, `radion_transfer_results.json`; analytic scalar-tail-only brackets |
| Independent reference overlap | INDEPENDENT_NUMERICAL_AGREEMENT | `audit_source_transfer.py`, JSON; within7e-8 at L10, saved-profile pathway |
| Radion flux from 4D action and 5D TT current | EXACT_CHECKED_AND_REVIEWED | audit I; factor-two mutation rejected |
| Complete finite-k interval proof / finite-A error bound | NOT_COMPLETED | explicit obstruction in goal report; no invented emission percentage |

Executed command/version/output references are in `GLR_GOAL001_CHECKS.md`.
Four physical k values × three refinements are used; threshold separate.
No production evolutions, cluster submissions, or Michael implementation
changes. The table below is the preserved **initial** check baseline.

| Check | Status | Evidence / limitation |
|---|---|---|
| cos(kz)^2 selection n=0 and +/-2 | CHECKED_SIMPLE_ALGEBRA | SymPy 1.14.0, Python 3.13.5, `check_foundation.py`. |
| Sigmoid charge-transition energy `E/M=Q^2*r0/(3*tau)` | CHECKED_SIMPLE_ALGEBRA | Beta-function integral 1/6; not a physical GL result. |
| Static KK mass/tension charge algebra | CHECKED_SIMPLE_ALGEBRA | Solved 2 linear relations; assumes stationary asymptotic charges. |
| Figueras et al. early-burst hypothesis | SOURCE_CONFIRMED | 2210.13501v2 III.1 explicitly conjectural. |
| M2-C blocked, one GP run and 11 not attempted | SOURCE_CONFIRMED | Frozen campaign closure, raw manifest and acceptance JSON. |
| SO(3) reduction action and Euler–Lagrange equations | DERIVED_UNAUDITED | Check warped-product curvature, signs, boundary terms, Hamiltonian dof count. |
| 4D radion news normalization from 5D asymptotics | DERIVED_UNAUDITED | Check 4D EMD/Bondi flux vs 5D symplectic/Bondi law. |
| Linear GL physical eigenfunctions `Omega(k)` | NOT_ATTEMPTED | Recompute eigenfunctions and reproduce published threshold/cases. |
| Full quadratic n=0 source | NOT_ATTEMPTED | Includes gauge, constraints and all components. |
| Nonzero outgoing projection coefficient C0 | NOT_ATTEMPTED | May vanish! Require clean retarded Green-function computation. |
| Massive KK canonical energy flux | NOT_ATTEMPTED | Threshold is known; norm and physical master current are not. |
| Full numerical GRChombo/AMR waveform extraction | NOT ATTEMPTED | Explicitly prohibited as production work in this research track. |

The four checked source/algebra claims are narrow. There has been
**no independent symbolic verification of the full Einstein expansion** and
no production numerical runs. Record exact output, code version, and
proof scope before upgrading any result. Source-backed literature claims
are not automatically proofs of our equations.

## Research-level acceptance protocol

For a nontrivial new identity or coefficient: (1) analytic derivation;
(2) independent derivation in a different parameterization or gauge;
(3) machine check with exact symbolic identities and deterministic
positive/negative tests; (4) adversarial mathematical reviewer not relying
on the first derivation; (5) field normalization and dimensional checks.
Retain failed attempts and boundary-condition obstructions in separate logs.


