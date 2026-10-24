# GLR-GOAL-005 execution report

**Terminal scientific classification: MEANINGFUL_PARTIAL.** Executed by
Michael Yan's authorized sole-writer Codex task, 2026-10-23. This completes
the bounded investigation and reporting task; the Goal's full scientific
completion gate is **not** met.

| Question | Verdict | Result and limit |
|---|---|---|
| Certified physical L10 eigenpair? | YES | Nodeless root Omega r0 in [0.06328118,0.06328120], width 2e-8; physical normalization nonzero. |
| Global regular physical C5 control? | YES, at the stated coordinate-norm scope | Mixed profile norm <4e29; common tau=0 EF coordinate norm <3.4e31. Absolute envelopes, not small errors about the old cubic or tetrad constants. |
| Complete conserved quadratic source norm? | YES for all seven component C3 envelopes; NO for full historical Y0 | n=0 and complex +2 Ricci and Einstein source bounds <1e110, with exact conservation for the validated eigenmode. Extra mixed-source derivative constants remain unevaluated. |
| Complete physical q2 / full M2? | NO | Exact source-selected +2 scalar master has F L2<1e128, u L2<5e129 and u sup<2e130. Full affine regular metric lift, zero sector and homogeneous-data bounds are missing. |
| Newly enclosed C0 or controlled finite-time radiation? | NO | Historical C0=12.30729197 remains empirical. Free scalar news and interference are not bounded here. |
| Finite-amplitude / nonlinear news-error interval? | NO | Neither a full-quadratic kinematic interval nor true nonlinear A* is supplied. |
| Physical first-bulge E/M? | NO | No exact global data family, nonlinear radiation map or invariant first-bulge relation is established. |

All numbers use r0=1, L/r0=10, k=pi/5, K=2k, s=2 Omega and
signature (-++++). The real first-order cosine amplitude is normalized
by H^U_theta theta/(2r²)=1 at r=2 in completed unitary gauge.
A is neither numerical chi nor horizon bulge. The quadratic convention
has no factorial. The +2 master bound is for one complex coefficient;
a real metric also needs its conjugate.

## 1. Certified result and independent support

The [proof and norm definitions](../physics/goal005_enclosures.md) give
the endpoint, finite-interval, reconstruction and source inequalities.
[Audit I](../verification/AUDIT_GOAL005_SPECTRAL.md) independently
derives Wronskian monotonicity and residual-contraction endpoint
estimates before inspecting the new solver, then reviews its interval
flow and physical reconstruction.
[Audit II](../verification/AUDIT_GOAL005_GEOMETRY.md) uses a direct
five-dimensional connection/Ricci implementation and separately derives
source counts, compatibility and scalar flux checks.

The primary uses outward-rounded Arb at 160 bits, degree-11 Taylor
polynomials with order-12 remainders, strict Picard tubes, signed
variational radius propagation and analytic Volterra endpoint balls.
One GL domain is used: log(r-1)=-28 to r=40, matching at r=2, maximum
logarithmic step 1/32. Bounded cell subdivision is part of validation.
Whole-parameter-interval propagation excludes Riccati poles; positive
endpoint solutions and matching establish a nodeless connection.

For F=pH(2)-pInfinity(2), the tight endpoint match balls have widths
below 3.95e-10 and strict opposite signs (centers approximately -4.07e-8
and +4.99e-8). The independently derived identity

\[
F'(\Omega)=2\Omega\left[
 \int_{-\infty}^{x_m}(u_H/u_H(x_m))^2dx+
 \int_{x_m}^{\infty}(u_I/u_I(x_m))^2dx\right]>0
\]

establishes uniqueness on this certified nodeless branch, not among all
possible GL instabilities. With u(2)=1, the physical normalization
functional is inside [-0.077137656,-0.077137624], away from zero.
[Exact saved balls](../verification/goal005_gl_results.json) retain
lower/upper endpoints rather than relying on rounded display strings.

## 2. Enclosure and error budget

| Quantity | Enclosure or allowance | Meaning |
|---|---:|---|
| Root interval | width 2e-8 | Parameter uncertainty retained throughout |
| Horizon p cutoff error | <2.91e-11 | Primary analytic Volterra allowance |
| Infinity p cutoff error at R=40 | <0.005063 | Analytic allowance contracted by validated inward flow |
| F width at each tight endpoint | <3.95e-10 | Endpoint, flow and rounding errors included |
| Independent horizon p remainder | <1.60e-24 | Audit Frobenius contraction on broader trial interval; not substituted into primary run |
| Independent cubic infinity p remainder | <2.74e-5 | Audit residual bound, independently implemented |
| Mixed regular/static C5 mode norm | [0,4e29), width 4e29 | Absolute physical-mode envelope |
| Common-slice EF coordinate C5 norm | [0,3.4e31), width 3.4e31 | Audit conversion factor <85, not a tetrad constant |
| Each n0/+2 Ricci or Einstein source C3 norm | [0,1e110), width 1e110 | All components in stated weights |
| Exact +2 scalar F L2 | [0,1e128), width 1e128 | Whole-line integral with both endpoints |
| Exact +2 scalar u L2 and sup | [0,5e129), [0,2e130) | Evaluated physical source plus positive-operator inverse |
| Historical-profile C5 error, full Y0, M2, C0 error | NOT ENCLOSED | No error bar inferred from the preceding absolute bounds |

The huge constants arise from pessimistic componentwise jets near the
small horizon collar. They do not establish smallness at a moderate
physical amplitude. Endpoint, parameter and rounding uncertainty are
propagated as balls; no empirical solver tolerance is treated as a
rigorous error. Amplitudes use whole-cell integrals, not sampled quadrature.

The mixed C5 norm covers four SO(3) coefficients and derivatives through
five: regular EF profiles on tau=v-r=0 for 1<=r<=4, static profiles with
exp((r-4)/2) weighting for r>=4. The sphere tensor is treated intrinsically.
In the horizon collar, Hrr uses an exactly vanishing numerator and a divided difference,
avoiding subtraction of separately singular static components.

Source counts use exact inverse/connection and warped-sphere formulas,
retaining Fourier factors, Lorentz signs and trace adjustment.
E=Q-gbar tr(Q)/2 is conserved because the enclosed H solves the exact
first-order equations; this is not conservation of the historical cubics.
The exterior source weight is sigma=sqrt((2 Omega)²+(2k)²/4)<0.65.
The component counts also provide the stronger raw exponential envelope
used by the scalar-master integral.

The literal historical Y0 reserves a higher master-forcing derivative.
Generic Q C3 alone is insufficient. Audit II derives a Codazzi saving:
Q_mu z is quadratic in h and its first derivatives, so Q_mu z C4 uses
H5. A suitable anisotropic replacement is possible, but its complete
constants are not evaluated. This is not a claim that H6 is necessary.

## 3. Two bounded routes to forced geometry

**Route 1: actual historical reconstruction.** The newly executed
all-seven-equation residual rejects acceptance from a small scalar ODE
residual alone. The audit uses the historical finest L10 configuration:
log horizon cutoff -18, R=65, 16001 points, rtol 2e-12, maximum step .05.
These results are empirical diagnostics, not interval enclosures.

| Diagnostic | Absolute maximum | Scaled maximum |
|---|---:|---:|
| Reconstructed Ricci residual, r=1.1..10 | 30.72869 | 0.1894123 |
| Reconstructed Ricci residual, r=1.005..30 | 3688.7901 | 6.898631 |
| Compatible local forced jets | 2.23e-16 | Local identity only |
| Independent first-order local residual | 5.28e-16 | Local identity only |

Mass-kernel controls on the first two domains are approximately 1.91e-7
and 2.23e-5. More extreme horizon samples fail that control and cannot
establish a physical singularity. The audit JSONs retain domains and data.

The cause includes separately interpolated Q and regular forcing J.
At r=2, Jinterp versus J[Qinterp] differs by about 7.359e-5 in value,
.02463 in first derivative and 6.322/4.536 in second derivatives.
Analytically differentiating the same source cubic still leaves
forced residuals .87563/.61577. The defect is therefore not solely
the final response spline. Independently implemented compatible local
jets satisfy the exact equations.

**Route 2: exact compatible source and inverse.** This avoids needing
a small error for the old interpolant. Let Qb=1e110 and Cj=7e10 Qb.
Interval-bounded rational forcing coefficients give

\[
\|F\|_2^2\le \frac43(10^{12}Q_b)^2
 \sum_{j=0}^{16}{16\choose j}4^{16-j}\frac{j!}{2^{j+1}}
 +64e^{8s}C_j^2\frac{3^{2s+2}}{2s+2}.
\]

The resulting norm upper bound is approximately 7.95048e127.
The established positive-supersolution proof for K²>=3/2 supplies the
u bounds. Audit II checks the squared estimate independently with
rational inequalities. Retaining that squared estimate matters:
F<1e128 alone would not imply the sharper u L2<5e129.
This encloses an exact separated scalar response, not a time-domain
metric propagator or full M2.

**First missing full-geometry inequality:** an explicitly evaluated,
compatibility-preserving regular affine-lift bound
\(\|q_{2,2}\|_{X_0}\le C_{2,0}\|Q_{+2}\|_{Y_0}\), or a fully proved
and evaluated anisotropic replacement, is needed, followed by the
zero-mode scalar/mass/lapse and prescribed homogeneous-data bounds.

A horizon compatibility defect leaves N(1)/(r-1); a small sampled
residual cannot bound that supremum. Exact cancellation and a bound
on differentiated N are required. Two substantive approaches were
investigated; the second closes scalar estimates but not the full gate.
This is no impossibility theorem. Unused configuration slots do not
justify an unmotivated sweep.

## 4. Radiation, initial data and next consequence

No new C0 interval is obtained; the old value remains empirical.
An eventual validated overlap could enclose the particular settled
leading coefficient

\[
\Delta E_0/M_0=2(\Omega r_0)C_0^2(A_2^4-A_1^4).
\]

Finite-data scalar energy includes
\((2/r_0)\|N_{\rm particular}+N_{\rm homogeneous}\|^2\).
Particular-matched and zero independent scalar data differ by a real
homogeneous field. Neither a mass constant nor static tension cancels
that freedom. A validated C0 overlap and specified transient bound
would be the next useful consequence for controlled *linear* finite-time
energy. Nonlinear news further needs exact global data, propagation/
trace bounds and the fourth-order remainder.

An imaginary-frequency n=2 response is not outgoing above-threshold
massive flux. Exact SO(3) excludes ordinary tensor Bondi news while
allowing the massless radion. Stabilization remains a separate EFT.
The failed GOAL-004 few-mode model is not used as calibration.

## 5. Audits, reproducibility and finite work

The reviewers are distinct AI investigators, not human referees.
Shared historical equations/conventions are disclosed. Audit I derives
endpoint/monotonicity estimates without importing the primary code,
checks manufactured linear-versus-Riccati flow and reviews physical C5.
Its 28+22 assertions are not 50 proofs. Audit II implements direct 5D
Ricci independently of the reduced W route; its historical physical
profile is shared and non-certified. Its 21 main checks, diagnostics
and independent rational-bound review have different scopes.

Rejected mutations include bad endpoints, wrong tortoise derivatives,
normalization errors, missing Fourier halves, Euclidean Lorentz
contraction, reversed Green sign, lost EF division, discarded Bianchi
compatibility, omitted half Lie-square completion, static tension as
scalar hair, and lost homogeneous-news interference. Audit records
separate exact, interval, manufactured and empirical witnesses.

See the [executed command ledger](../verification/GLR_GOAL005_CHECKS.md),
[primary failures](../verification/goal005_failures.txt),
[geometry failures](../verification/audit_goal005_geometry_failures.txt),
and [artifact manifest](../verification/goal005_manifest.json).
Dependencies: Python 3.13.13, python-flint 0.9.0 (Arb), SymPy 1.14.0,
NumPy 2.4.6, SciPy 1.18.0, mpmath 1.3.0. The historical Python version
was 3.14.8. Arb supplies validated arithmetic; ordinary floating
geometry diagnostics are not interval certificates.

One GL and one forced diagnostic configuration were used; identical
reruns and validation subcells do not add physical domains. No L8 or
other-k scan occurred. Initial Picard failure was repaired by bounded
subdivision. A source trace-product cap failed and was increased using
explicit counts; Audit II additionally required the sphere trace
derivative allowance +48. Floating selection of candidate interval
maxima was removed before acceptance, with exact/Arb cap checks and
refreshed review hashes.

## 6. Provenance and publication

Actual starting input: **0dd5be7818b2e71d28ebff2e122ac0a318a220f5**,
branch research/sav-radiation, freshly fetched and initially clean.
This is newer than the Goal's historical baseline
96339836a1281c219917f900224e7c98cf62f3f8; no reset occurred.
The manifest pins historical Git blobs and new artifact SHA256s.
The [publication record](../verification/GOAL005_PUBLICATION.md)
records the science commit and verified remote state.

The user authorized Michael as sole writer; only the primary commits
and publishes. This relies on the coordinated ownership declaration,
not an exhaustive operating-system process audit. All shared changes
are under research/sav. Historical reports, source data, contracts and
numerical history are preserved; the Goal only receives an append-only
execution annotation. Host setup and paths remain local.

No production edit, spacetime run, PR/merge, manuscript action,
collaborator message or automatic GOAL-006 occurred.
The [Michael handoff](../handoffs/MICHAEL_START_HERE.md) lists observables
for a future separately approved extraction, without a production request.
