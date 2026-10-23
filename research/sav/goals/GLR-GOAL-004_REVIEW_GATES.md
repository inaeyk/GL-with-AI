# GLR-GOAL-004 — independent review, falsification and completion rubric

**Preparation document; no checks have yet run for GOAL-004.**
Controlling task: GLR-GOAL-004.md. Authorized research-only scope:
research/sav/** on research/sav-radiation. Prior GOAL-001/002/003
execution and the GOAL003_REVIEW_QUALIFICATIONS.md remain intact.

## Non-negotiable four questions at the top of the final report

1. Does a **nonzero numerical/invariant A_*** or epsilon_* follow for an exact
   global GL-seeded initial-data family with quantified uncertainty?
2. Is there a **physical finite-time massless-radion news and energy
   enclosure**, rather than a formal coefficient or a transient-only bound?
3. Does any certified window **actually reach an invariantly specified
   first-bulge event**, and is an absolute or fractional first-bulge
   energy estimated?
4. Is there a **separate, explicitly qualified semi-analytical model**
   of the initial bulge with reproducible predictions/uncertainty bands?

Answer YES, NO or CONDITIONAL for each; list exact hypotheses. A YES on a
formal identity does not justify YES on a physical early energy fraction.

## Gate-by-gate acceptance matrix

| Gate | Minimum sufficient evidence | Misleading substitute that must fail |
|---|---|---|
| A: source-evaluated q2 | Independently enclosed *physical* normalized L10 GL mode and full n=0/n=2 sources, horizon and exterior input norms, forced regular metric and actual M2 bound | Citing existence of GOAL-003 C_alg or a generic s^-2 master bound without evaluating source constants |
| A2: source/metric inverse | Exact forced tail, properly normalized horizon/infinity Green basis, correct resonant polynomial power, all metric reconstruction derivatives and independent data | A finite cutoff Robin condition, positive discrete samples, frozen analytic-continuation rational specialization |
| B: exact global data | Exact z-dependent constraints on horizon-crossing *global KK-flat exterior* with bounded right inverse, fixed ADM mass/tension, well-defined all-orders scalar data | Finite collar family, scalar-only exact KK lift or perturbative order-two constraints |
| C: nonlinear propagation | Valid gauge-fixed redshift/exterior estimates with genuine radiation trace on a specified epsilon-dependent time window, constraint preservation and propagation norm | Point-resolvent estimate alone, scalar Killing energy alone, fixed-time Gronwall relabeled logarithmic-time control |
| D: fourth scalar news | All physical cubic/quartic sources, gauge/Bondi completion, chosen initial homogeneous news and actual R_N bound | Bare formal Q4 recurrence, setting free scalar data to zero silently or using O(A^4) without a constant |
| D2: physical radiation enclosure | Bound of true reference news plus all correction norms, two-sided E0(U)/M0, smooth spectrum and causally clean u window | Empirical C0 convergence or model fit asserted as a physical emission percentage |
| E: first-bulge analytic model | Explicit reduced equations/projection, 2 candidate closures examined, matched weak limit, predicted waveform/flux/spectrum, error proxies/uncertainty and falsifiable diagnostics | An arbitrary sigmoid or horizon-radius ansatz with no source/energy matching, a calibration to horizon times mislabeled observed radiation |
| E2: nonlinear bulge | Invariant horizon/geometry criterion and a genuine nonlinear continuation/remainder or explicit model qualification | Linear MOTS graph extrapolated to A~1 or an apparent-horizon peak called waveform arrival |
| F: stabilized radion | Consistent *separate EFT*, stress/current/threshold, physical mass regime and near/far matching assumptions | Adding m_phi to vacuum Einstein, confusing |omega|>m_phi with 2Omega>m_phi, using massless Bondi flux for massive transport |
| Audits | Two distinct scientific investigations with independent central derivations, exact/controlled checks, deliberate wrong cases and disclosed common provenance | A single author reviewing its own claim, counting shared-script reruns as independent proofs |
| Scope | Research-only fast-forward commits and no simulations, edits or merges outside agreed area | Disguised production changes or unauthorized larger numerical campaign |

**Evidence words:** PROVED, INDEPENDENTLY AUDITED IDENTITY, NUMERICALLY
ENCLOSED, EMPIRICALLY CONVERGENT, CONDITIONAL THEOREM, MODEL/HEURISTIC,
CONJECTURE, OPEN/BLOCKED. State which definition applies to each claim.
GOAL-003 review specifically warned against overstating independent
"W versus R" routes and same-investigator inverse audits; this Goal
requires independence evidenced by actual operations and derivations.

## Mandatory adversarial mathematical tests

1. **Full source check:** verify both real cosine n=0 and n=2 weights,
   gauge Lie^2 completion, Lorentzian tr-sign, forced n=2 trace/divergence,
   all seven reduced equations and all Bianchi constraints at *physical*
   positive k and at a separate exact/manufactured control. If a finite
   rational specialization has a physical radial pole, never call it a
   global physical-mode check.
2. **Horizon and asymptotics:** test regular EF/tetrad components, retained
   derivatives, source resonances, finite endpoint remainders, normalized
   Green/Wronskian signs and large-radius tails. Mutate one boundary branch
   and ensure rejection.
3. **Charges/global constraints:** test KID boundary pairing, zero KK
   momentum and ADM charge selection; check that a local solution does not
   acquire an unmeasured exterior tail. Reject perturbative linear
   superposition as an exact nonlinear data construction.
4. **Radiation/data freedom:** at fixed first-order GL amplitude construct
   at least two *allowed* free radion data sets and verify interference in
   energy. Counterexample: a source S=L_s f with f compactly supported
   can have zero outgoing overlap, even though the source is nonzero.
5. **Long-time smallness:** distinguish point-inverse norm, time-domain
   semigroup/redshift estimate, log-epsilon time and true nonlinear
   residual; test deliberately incorrect exponent/smallness conditions.
6. **Fourth order:** check the complete radion perturbative gauge
   completion and coefficient factors, the n-selection parity and the
   first uncomputed even-scalar term. An order-four independently
   prescribed free scalar datum can change N4 at fixed lower order.
7. **Finite news-energy inequality:** if N=N_ref+deltaN and b>=||deltaN||,
   confirm the lower estimate only when b<||N_ref||. Verify
   ||deltaN||/||N_ref||<=sqrt(1.1)-1 is *sufficient* for <=10% energy
   error, not necessary; retain physical units 1/G4. Use a smooth
   observation taper for any spectrum, and bound initial-data transients.
8. **Bulge-model validity:** verify source-to-news conservation and weak
   C0 limit, explore two closures, compare backreaction and discarded
   modes. Residual convergence is not a model-error certificate without
   stability. Test both early-dominant and late-high-frequency scenarios.
9. **Massive scalar:** reproduce timelike radial dispersion/current and
   the massless limit at fixed observation regime; expose noncommuting
   large-time/small-mass limits and false sharp threshold for imaginary
   GL forcing. Never mix n=2 spin-2 KK and stabilized scalar labels.

## Required adversarial review ownership

Audit I (math geometry): physical GL profile/source enclosures, n=2
and n=0 regular metric, constraint inverse and homogeneous charges.
Audit II (energy/dynamics): exact initial data, nonlinear propagator/news
trace, scalar data interference, finite-A inequality, bulge closure and
EFT distinctions.

Each audit must document:
- author/investigator role, scope and relationship to primary derivation;
- an independently obtained equation or estimator before seeing primary
  code where possible;
- actual command/version/input/output for executed tests;
- intentionally incorrect mathematical cases and observed rejection;
- specific qualified result vs missing theorem or uncertain numerical bound.

If independent agents are unavailable, record that limitation openly.
An automatic second pass that shares all proof/code assumptions is a
replication, not a mathematically independent adversarial derivation.

## Boundary, reproducibility and expense controls

- Physical L10 is primary. One optional L8 comparison *only* after the
  primary gates have meaningful progress. No new spectral k sweep.
- Three new refined 1D configurations per studied sector/k at most.
  Two substantive repair approaches per central failed estimate at most.
  Small toy/effective ODE quadratures are analytical verification,
  not permission for 2+1 Einstein numerical evolution.
- Predeclare mathematical success/failure before exploratory constants are
  adjusted. Record endpoint/source errors, homogeneous-data choice and
  explicit charge norm. Avoid false rigor from floating ODE tolerance.
- Check branch/head/status and outgoing diff before every publication
  checkpoint. Preserve previous notes/audits and historical failures.
  Report failed push/lease as failed, not published.
- No production simulations; no Michael code changes; no PR or branch
  merge; no external collaborator emails; no unapproved manuscript work.

## Stop conditions and terminal verdict

If Gate A's source evaluation fails after two bounded strategies, record
the exact inequality and continue independent Gate E/F theoretical work
within authorization. If the global constraint family fails, **no**
claim involving exact GL-seeded physical finite-A news may be made; a
conditional field-theoretic source model can still be developed.

If rigorous nonlinear trace closure remains blocked, do not repeat
ineffective spectral scans. Produce a clear mathematical obstruction
and quantitatively defined model assumptions with an explicit agenda
for independent Michael-led measurement later.

COMPLETE_VERIFIED requires a controlled exact-data/fixed-charge news
interval and full certified/qualified error account (including the
readiness of the reference). It does **not** require reaching the bulge
if that is classified separately; if the model reaches it only
heuristically, the first-bulge radiation claim is MODEL/HEURISTIC.

MEANINGFUL_PARTIAL includes a genuinely computed/verified M2, new
global-constraint theorem, useful nonlinear trace estimate, or a
reproducible physics-based semi-analytical first-bulge prediction
with transparently limited error status. Never relabel a heuristic
prediction as rigorously enclosed radiation.
