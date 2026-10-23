# GLR-GOAL-005 — predeclared scientific verification and falsification gates

This file is a **planning rubric**, not a record of completed checks or a
simulation authorization. Task: [GLR-GOAL-005](GLR-GOAL-005.md).
The completed GOAL-001/002/003/004 scientific results and all negative
findings remain at their original evidence classifications.

## Primary evaluation table

| Gate | Required evidence for a PASS | Explicitly insufficient substitute |
|---|---|---|
| 0 — conventions/inputs | Reproduce independent first-order GL equation, TT constraints, two-gauge map and action/flux coefficients in one consistent r0, L, time and real-cosine convention; cite exact pinned sources | Using raw sampled profile/old reported digits as theorem |
| A1 — physical L10 eigenpair | Rigorous sign bracket for F(Omega), verified root uniqueness on stated nodeless branch, certified horizon/exterior input and eigenvalue errors | SciPy rtol, floating W drift, high mp digits, visual no-node samples |
| A2 — globally regular derivative envelope | Certified, explicitly weighted C5 or derived equivalent source-controlling horizon-EF/tetrad norm over horizon/interior/infinity | Ordinary Schwarzschild components; irregular horizon subtraction; only center-grid C1 errors |
| B1 — all quadratic Einstein sources | Every n=0,±2 Fourier tensor/constraint component with full sourced Bianchi and a *source norm with error* | Only Rzz, a local exact jet specialization, or Bianchi satisfied by manually assigned derivatives |
| B2 — source positivity/overlap and uncertainty | No inferred sign from Lorentz H_AB H^AB or positive kernel; interval overlap if claimed nonzero, with source/endpoint errors | Positive sample values or fabricated zero-overlap theorem |
| C1 — physical n=2 forced Green solve | Proper horizon and infinity branches, correct Wronskian sign, forced equal-exponential tail, explicit source-selected homogeneous response | Homogeneous TT substitution or naive Robin/outgoing Sommerfeld truncation |
| C2 — full metric reconstruction and residual | Source-aware affine trace/divergence, EF divided differences and derivatives, full seven Einstein + constraints residual, explicit regular metric norm/error | Only master L2, finite tube, reconstructed point max or tiny local polynomial source residual |
| C3 — zero-mode/total M2 | n0 scalar + Einstein mass/lapse charge quadratures; consistent n2 real ± harmonics, gauge, independent data/charge selection; actual M2 | Adding selected sector sample maxima or assuming zero homogeneous fields without data |
| D — C0/flux | Recomputed radion C0 with verified scalar endpoint/Wronskian/normalization, error carried into leading coefficient and free-data transient comparison | Quoting GOAL-001 C0 to 12 digits; calling exponential-particular flux physical early burst |
| E — perturbative *kinematic* smallness | Explicit background operator-norm/signature inequality in horizon-regular space with evaluated M1/M2 | A~0.1 by intuition, or interpreting truncated metric as exact solution |
| E+ — true nonlinear radiation | Exact global data family, nonlinear redshift/exterior and Bondi trace bound, first omitted even scalar N4, nonzero physical A* and energy error | M2 alone, local collar data, weak O(A^4) notation without remainder, few-mode EOS fit |
| Overall — scientific honesty | Correct completed/partial/blocked classification, preserved negative findings, distinct two-adversary reviews, reproducible commands/output | Number of tests, agreement of shared code, or unverified success claim |

**Main target:** A1+A2+B1+C1+C2+C3 with a source-selected particular
response and a fully specified norm and errors. D is a strong additional
success gate. E+ is a stretch and does not follow automatically from
lower-order geometry. Do not hide absent finite-time/homogeneous-data
control under "complete physical M2": specify the response class.

## Adversarial reviewer I — eigenfunction and rigorous numerics

Before reading the new primary implementation, derive independently one
nontrivial endpoint/root identity and construct a distinct numerical or
analytic error bound. The reviewer must check:

- proper dimensionless conversion k=2pi/10 and the GL root Omega>0;
- Riccati vs linear fundamental matrix match, sign of F and F';
- no poles/nodes in the chosen positive branch and endpoint boundary terms;
- variation in k/Omega and normalization at r=2r0;
- regular ingoing EF/tetrad fields across r=r0, including 0th–5th jets
  or whichever derivatives the selected source norm actually demands;
- outward rounding and interval arithmetic for *propagated* IVP/root errors;
  a conditional Volterra tail bound cannot independently certify its
  double-precision inputs.

Required manufactured negative checks: incorrect horizon branch,
incorrect sign of beta or Coulomb tail, wrong radial Jacobian dx/dr=1/f,
a false Riccati denominator/node, and deliberate underestimation of
endpoint truncation. A reviewer who shares the solver's central core
must disclose that fact; tests of the same code are not a second proof.

## Adversarial reviewer II — full sourced Einstein geometry

Derive at least one source/metric equation from a different route
(e.g. direct reduced 3D Ricci, 5D connection expansion or covariant
action) and check the actual physical positive-k response. Verify:

- proper real-cosine +/- harmonics and second-harmonic complex factor;
- tt/tr/rr/angular signs in Lorentzian contraction and sourced n2
  trace/divergence; no homogeneous TT restriction on forced q22;
- the conserved quadratic **Einstein** source vs bare Ricci;
- Bianchi and full seven Einstein equations on the **actual
  reconstructed response**, not just pointwise source jets;
- EF numerator/cancellation and divided-difference derivatives near
  horizon; infinity forced tail and normalization;
- coupled n0 scalar/mass/lapse/charge data and n2 homogeneous solution;
- source-to-geometry norm with full derivative/tail/input interval errors;
- radion C0, Green Wronskian, scalar flux, first-order+second-order
  field normalization and independent scalar Cauchy data.

Reject deliberate mutations: Euclideanize H_tr squared; omit cos²
half; flip radial master Green sign; drop Bianchi source;
use a static tension integration constant as arbitrary horizon-regular
scalar hair; omit second-order gauge Lie² term; discard homogeneous
n2/radion data while calling it a general initial-value result.

A physically calibrated profile point check is useful but must
**not** be presented as full eigenfunction/source enclosure.

## Mathematics and numerical controls

A claimed enclosure must identify:
1. exact mathematical function space and weights, including EF horizon
   and exterior radial asymptotics;
2. endpoint truncation remainders (all required derivatives);
3. a **validated** finite-domain IVP/error propagation scheme, or an
   analytic stability estimate with exact residual enclosure;
4. interval root bracket with genuinely outward-rounded signs;
5. an evaluation of coefficient/source integrals with interval quadrature
   or independently justified analytic remainder bounds;
6. control of eigenpair uncertainty through Q and the Green map;
7. homogeneous boundary conditions/data, ADM mass and circle conventions;
8. source/metric full residual, constraint and Bianchi tolerances expressed
   in a norm relative to the rigorous error envelope.

Sample grid suprema, fine-medium differences, 60-digit source arithmetic,
positive local samples and Wronskian drift are **diagnostics only** unless
converted to a mathematical bound. State what part is certified, what is
empirically convergent and what remains conditional.

If a validated package is absent, report its absence and attempt one
bounded mathematical alternative; don't silently install an untracked
dependency, run unconstrained sweeps, or label an mp estimate interval.

## Explicit theorem-versus-physics separation

The leading flux law

  Delta E0/M0 = 2 Omega_hat C0²(A2^4 - A1^4)

is a conditional, separated, settled-exponential quadratic formula. It is
not automatically the physical emission for finite-time GL data, even
if all coefficients are rigorously enclosed. Free radion transients
interfere, and the order-four news comes from the full q3/q4 hierarchy
plus independent higher-order data. A numerical early-bulge percentage
requires global exact data, a controlled long-time evolution/news map,
and a mapping to a genuine nonlinear geometric bulge event. Explicitly
mark those dependencies as open unless actually proved.

M2 could establish a **kinematic** epsilon smallness interval for a
truncated metric, while the true nonlinear A_* remains unknown. Do not
call one the other.

SO(3) selection remains exact; massless four-dimensional tensor Bondi
news is excluded but radion monopole radiation is physical.
The stabilized radion may be massive only in an additional EFT and
cannot silently modify vacuum source or spin-2 KK mass.

## Budget and stop classifications

One physical L10 k, its forced K=2k sector, at most three declared
whole-problem refinement configurations per sector, optional L8 only
after main goals; two substantive repairs per failed central gate.
No physical full 2+1/5D time evolution, simulation, production changes
or indefinite numerical scan.

End with exactly COMPLETE_VERIFIED, MEANINGFUL_PARTIAL,
BLOCKED_BY_MATHEMATICAL_OBSTRUCTION, NOT_REPRODUCIBLE or
OPEN_NEEDS_APPROVAL. A certified GL root but unbounded physical
C5/source/M2 is MEANINGFUL_PARTIAL, not success on C3.
An exact proven obstruction or persistent lack of verified enclosure
should be recorded with its concrete limiting inequality, not a
universal no-go theorem absent proof. Separate mathematical and
physical report verdicts explicitly.

## Final review packet

The final report must present (a) a compact certified-vs-empirical
data table with actual interval widths, not only "pass"; (b) the
mathematical proof and code paths; (c) source and M2 norms with error
budget; (d) what distinguishes particular from finite-time
data-selected response; (e) independent reviewer observations and
mutations; (f) a specific next mathematical consequence for weak
early-radiation energy. Identify the exact branch/checkpoint SHAs and
any numerical failures; no automatic subsequent Goal.
