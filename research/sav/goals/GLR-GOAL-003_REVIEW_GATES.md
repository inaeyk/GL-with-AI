# GLR-GOAL-003 — Independent acceptance and falsification gates

> Completion annotation — 2026-10-23: GLR-GOAL-003 executed on 2026-10-22
> and completed with **MEANINGFUL_PARTIAL**. Read the [final report](GLR-GOAL-003_REPORT.md)
> and its execution ledger for actual results and open gates. The original
> acceptance rubric below is preserved as historical authorization text; READY or
> NOT LAUNCHED statements describe preparation, not current permission.
> No replay or subsequent Goal is authorized.

**PREPARED; no checks in this document have been run for GOAL-003.**
Controlling task: [GLR-GOAL-003.md](GLR-GOAL-003.md).
Baseline: research/sav-radiation@554ceb968fb3f2549a80f1b1e953810d9e79744a.
All future artifacts remain under research/sav/**. No production action.

## Evidence and claim ladder

For each item report separately: explicit derivation; independent route;
exact/manufactured checks; floating diagnostics; certified bounds; physical
conclusion. Numerical agreement does not imply a theorem. A formal
representation does not imply a bounded inverse or solved Cauchy problem.
Use existing evidence classes and preserve all negative records.

| Gate | Required evidence | Failure / partial outcome |
|---|---|---|
| A: forced n=2 master | Explicit source, second-order gauge completion and reconstruction; equivalence with all seven Einstein components, trace/divergence and source Bianchi identities; homogeneous limit recovered | Source-free TT substitution, source lift concealing an inverse, unremoved reconstruction pole or lost constraint leaves A open |
| B1: boundary response | All-component EF/tetrad horizon regularity, forced infinity expansion, evaluated Green/Volterra solution and endpoint remainders | Formal inverse, finite scalar master alone or homogeneous outer Robin condition is incomplete |
| B2: inverse estimate | Proven/certified nonresonance and explicit source-to-regular-metric norm constant, including reconstruction/derivative losses | Sampled Wronskian, numerical GL threshold and small local residual do not certify B2 |
| C1: full quadratic geometry | Charge-fixed n=0 quadratures plus n=2 response, compatible homogeneous data, global regular M2 and input errors | Repeating the radion source or bounding only one master component is incomplete |
| C2: exact simultaneous data | Exact GL-plus-scalar constraint family, growing tangent, all-orders free-data rule, boundary/charge compatibility and right-inverse estimates | Scalar-only exact family or order-two constraint satisfaction is not exact GL data |
| D: nonlinear validity | Well-posed regular evolution, constraint propagation, full metric/remainder and radiation trace estimates, quantitative positive A* and declared time domain | Unspecified existence radius, O(A^4) notation, metric component smallness or scalar Killing energy alone does not certify closure |
| E: amplitude/bulge | Fixed-convention adjoint dynamics or controlled memory kernel; invariant geometry map and continuation/error criterion | Static cubic coefficient, unbounded resummation or A~1 extrapolation is not dynamical closure; report reach separately |
| F: early radiation | Exact-family causal news, positive reference/lower bound where used, full error budget, scalar transient interference and energy balance | No first-bulge percentage or full spectrum without corresponding window/geometry/remainder control |
| S: stabilization/EFT | Separate consistent action, matching assumptions, massive monopole transport/current, domain of massless limit, preserved SO(3) rules | Adding a mass to vacuum equations, importing uncorrected GL data as exact, or treating massive flux as Bondi news fails S |
| Audit/publication | Two distinct adversarial audits with executed checks, negative tests, reproducible sources and research-only tree comparison | Self-review or intended checks are not executed independent verification |

Formal D/E algebra is allowed before B/C are quantitative; all dependent
claims remain conditional. Core COMPLETE_VERIFIED requires A, B1/B2,
C1/C2, D, F, S and audit/publication. E's bulge coverage is a separately
reported stretch achievement. No fully matched microscopic stabilizer is
required for a vacuum theorem; EFT predictions remain conditional where
matching is unresolved. A strong master/inverse result with open nonlinear
closure is MEANINGFUL_PARTIAL. Do not change this definition at the end.

## Audit I: geometry, gauge, constrained master and inverse

The reviewer should derive at least one central relation independently,
inspect the actual implementation after that derivation, and preserve
commands/versions/outputs if code is run. Check:

1. No-factorial second-order transformation, compensator Lie terms,
   real-cosine/complex Fourier halves, Lorentz contraction signs, K=2k
   and s=2Omega. The affine forced trace/divergence cannot be set to zero.
2. Both directions of master/reconstruction equivalence, all seven
   equations, all three source Bianchi identities, source conservation
   for the Einstein combination rather than Ricci Q alone, and kernel
   counting. Confirm the source lift is explicit and locally regular.
3. Horizon behavior of every reconstructed EF/tetrad component and
   derivative used in X, including cancellations of static-coordinate
   singularities. At infinity distinguish equal-exponent forcing from
   a global spectral pole; retain the actual particular tail and bound
   the residual. A divergent isolated overlap coefficient may indicate
   an invalid decomposition, not a divergent full decaying solution.
4. Wronskian sign/normalization, adjoint measure and boundary terms;
   spectral/nonresonance hypotheses; conversion of a master norm to a
   full metric norm. The point s inverse and retarded propagator are
   distinct objects. Examine continuum thresholds and derivative loss.
5. Independent n=0 angular/evolution equations, charge policy, n=2
   homogeneous data and global M2. Reproduce one relevant homogeneous
   literature/control limit without importing a different convention.

Deliberately wrong cases should include omitted trace/divergence forcing,
one lost quadratic gauge term, doubled harmonic coefficient, incorrect
Green sign, a horizon-finite master with singular reconstruction, and
omitted forced infinity tail. Demonstrate rejection in the applicable
identity/estimate; do not merely state that mutations were considered.

## Audit II: exact data, nonlinear closure, radiation and small mass

Independently check:

1. Exact simultaneous constraints, initial-data differentiability,
   Killing/charge cokernels and horizon boundary assumptions. Verify
   higher-order free data are fixed/bounded; no nonlinear superposition
   of the scalar-only family with GL. Check half-period covariance before
   excluding odd zero-mode orders.
   Half-period covariance enforces order/n parity only; the usual finite
   harmonic lists additionally set other independent free harmonics to zero.
2. A positive metric operator norm with derivatives and a nondegenerate
   horizon estimate, plus radiation trace control. Upper estimates need
   lower bounds in denominators. Track constants over a logarithmic
   instability-time window if claimed; fixed-time existence is weaker.
3. Third/fourth-order harmonic selection and all source/gauge/asymptotic
   terms needed for N4; a separate bound on remainder/higher data, not
   an assumed order symbol. Verify the amplitude/clock transformation
   of gamma and any local approximation to memory dynamics.
4. Scalar news normalization from the action/current, units and all
   G4, L, r0 and sphere factors; free/generated interference; horizon
   loss, outgoing energy and stored energy; smooth-window Parseval.
   A finite-window energy enclosure should remain meaningful near
   signal zeros, where relative error can fail.
5. EFT variation of R−2(grad phi)^2−4U, canonical normalization,
   U(0)=U'(0)=0 and U''(0)=m_phi^2. Check stabilizer stress/mixing,
   asymptotic geometry, KK retention/matching and cutoff hierarchy.
   A transport-only model with the vacuum source must carry a stated
   matching error and conditional label.
6. Exact SO(3) versus compact n selection, scalar monopole allowance,
   absence of massless tensor/vector news, and the separate massive
   gravitational n=2 sector. Verify massive scalar threshold, current,
   group velocity, tails/arrival limits and massless-limit nonuniformity.
   Pure exponential growth is not a real monochromatic emission test.

Reject deliberately omitted interference, arbitrary free order-four
scalar data hidden inside a universal K4, horizon-degenerate energy used
as a metric norm, a static gamma presented as dynamical, a potential
factor-of-two error, massive Bondi-news flux, an imposed scalar quadrupole
rule, and a threshold condition 2Omega>m_phi for exponential forcing.
Do not confuse kinematic permission with a nonzero retarded overlap.

## End-of-run questions that must receive explicit answers

- Was an explicit constraint-preserving forced master obtained? Is the
  physical reconstruction global, and is its inverse quantitatively bounded?
- Is one specified simultaneous GL data family exact? Which free data and
  charges are fixed, and are the claimed family/radiation maps differentiable?
- What finite A*, time window and news/energy error are established?
  Which constants are analytical, certified numerical, empirical or open?
- Is early energy bounded above only, or also strictly below away from
  zero? Does any result actually reach a defined first-bulge event?
- Is a local amplitude equation justified, or what kernel/data dependence
  remains? Which higher-order claims are only formal?
- What changes in the separate massive-radion EFT, what remains protected
  by symmetry, and which matching/stabilization inputs are still unknown?
- Which tests/audits actually ran? Were all published changes confined to
  research/sav/** on research/sav-radiation, with no production action?

Give precise failed inequalities and a concrete next mathematical step
for every unclosed central gate. Two unsuccessful methods do not establish
mathematical impossibility. Preserve both positive and negative results.
