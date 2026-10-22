# GLR-GOAL-002 — From linear GL growth to controlled early radion energy

**Task ID:** T-20261022-GLR-GOAL-002  
**Status:** READY FOR CODEX — NOT YET LAUNCHED  
**Repo:** inaeyk/GL-with-AI  
**Branch:** research/sav-radiation  
**Permitted writes:** research/sav/** only  
**Scientific owners:** Savdeep Sethi and Michael Yan; Pau Figueras advises.  
**Default Codex environment:** GPT-6.1 Sol / High / standard speed.  
**Forbidden:** production GRChombo/CCZ4 evolution; MPI/AMR campaigns; modification of code, numerics, physics assumptions, production files, other branches, or simulation schedule; manuscript changes or merging.

## 0. The primary scientific result to seek

Obtain the strongest *controlled finite-amplitude prediction* for the massless Kaluza–Klein scalar radiation driven by the initial Gregory–Laflamme (GL) instability of a five-dimensional Schwarzschild black string with exact SO(3) symmetry. Begin with a single physical fundamental n=±1 unstable harmonic and regular constrained data, before the neck cascade. Go beyond the pure exponentially growing second-order particular solution by constructing and controlling:

- the full second-order n=0 and n=±2 geometric backreaction, in a horizon-regular gauge, including Einstein constraints and conserved-charge freedoms;
- admissible finite-time Cauchy data and the homogeneous scalar and gravitational transients they generate;
- quantitative validity of an invariant weak-amplitude expansion (and the first omitted terms);
- the connection between a defined physical GL amplitude, source-time evolution and asymptotic Bondi radion news;
- when justified, the cubic evolution/amplitude normal form and its relation to static nonuniform black-string perturbation theory.

The desired **primary success** is at least one nonzero, explicitly justified interval 0 < A <= A_* with a quantified error or rigorously qualified bound on outgoing radion news and integrated energy, for a *specified admissible family of initial data*. If this is beyond the available mathematical tools, deliver the exact second-order backreaction, the strongest conditional interval/energy bound, and a precise mathematical obstruction — not an invented first-bulge percentage.

The **stretch scientific prize** is a quantitatively defensible estimate for radiation during initial nonlinear bulge formation. This is not an acceptance prerequisite if the perturbative regime breaks down first. The claim that early radiation dominates the later cascade must remain a conjecture unless its numerator *and denominator* are controlled.

## 1. Reference result, notation and starting point

Read the full project contract and reports:
- research/sav/AGENTS.md, README.md, NOTATION.md, STATUS.md, SOURCES.md, DECISIONS.md, QUESTIONS.md;
- research/sav/goals/GLR-GOAL-001_REPORT.md;
- research/sav/physics/gl_linear_mode.md, quadratic_radion_source.md,
  radion_retarded_transfer.md, radion_transfer_numerics.md, so3_reduction.md,
  kk_channels.md, timescales_energy.md;
- research/sav/verification/AUDIT_FOUNDATION.md, AUDIT_SOURCE_RETARDED.md,
  INDEX.md, negative_results.md, GLR_GOAL001_CHECKS.md;
- research/sav/handoffs/MICHAEL_START_HERE.md and literature/PAST_WORK.md.
Treat AI-authored reports as results to *independently test*, not unquestioned premises.

GLR-GOAL-001 ended MEANINGFUL_PARTIAL on commit
ec39904b833e5b7f9f32b80322bee298aaac7fc7
(its core derivations entered at 7110bc2cb3c0496dd62dc97d37b34d11ec780a17).
It reports a candidate exact second-order scalar source
  Box_4 phi_0^(2) = -(sqrt(3) k^2/16) A(t)^2 H_mu nu H^mu nu,
an outgoing transfer Phi(u) = r0 C0(k) A(u)^2 for an isolated eternal-past
exponential particular response, and conditional flux
  d(E_0/M_0)/d(u/r0) = 8 (Omega r0)^2 C0^2 A^4 + O(A^6).
The estimated C0 values at L/r0=8,10,12,16 are 28.2964,12.30729,7.02554,3.32104.
These are numerically convincing *conditional* results, not certified full
continuum interval enclosures, nor physical finite-amplitude or bulge
emission fractions. Audit II retains an exactly cancelling manufactured
source; review the physical calculation critically before extending it.

Preserve signature (-++++), G4=G5/L, M0=r0/(2G4),
k_n=2 pi n/L, f=1-r0/r, tortoise r_*=r+r0 log(r/r0-1)
with the stated additive constant, and the unitary completion h^U_zA=0
for nonzero k. The GL amplitude A is normalized by
  H^U_theta theta/(2r^2)=1 at r=2r0
as the *first-order* fractional areal-sphere-radius perturbation there,
not a numerical chi seed and not the bulge amplitude at the horizon.
At second and third order any redefinition of A must be fixed physically.

The completed second-order radion is phi_0^(2)=sqrt(3)<q^U_zz>/4,
and at 4D Bondi infinity phi_0=Phi(u)/rho+..., news N_phi=dPhi/du,
power P_0=N_phi^2/G4. The vector KK exponent was corrected to
+2sqrt(3)phi for the stated metric convention in GOAL-001.
A positive coefficient in the energy law cannot by itself fix A_*.

## 2. Milestone A — Adversarial foundation and physical amplitude

1. Independently reproduce the central source/Green-function/gauge
   completion of GOAL-001 by a **different** mathematical route, or identify
   any sign, horizon regularity, gauge, omitted-field or boundary errors
   before expending higher-order effort.
2. Construct a regular tetrad/ingoing-Eddington–Finkelstein description of
   the normalized first-order GL perturbation and a scale-consistent norm
   of its physical size. Do not estimate smallness using divergent static
   Schwarzschild components.
3. Choose and document an invariant geometric amplitude definition or
   explicitly fixed gauge-plus-asymptotic-clock prescription beyond first
   order. Determine how amplitude and cubic normal-form coefficients
   transform under A -> A+c A^3 and permissible time reparametrizations.
4. Specify all boundary/charge conventions: fixed circle length L,
   asymptotic clock, ADM mass/tension, ingoing future horizon, outgoing
   infinity, and whether total ADM mass is held fixed order by order.
   Explain the static homogeneous zero modes and mass/tension adjustments.

**Gate A:** independently derivable, reproducible source/normalization,
horizon-regular state and exact initial-data/gauge conventions.
If GOAL-001 has a substantive physics defect, record it and repair only
inside research/sav/**. Do not continue with a known incorrect source.

## 3. Milestone B — Complete second-order metric response

1. Expand the *exact SO(3)-reduced Einstein action/equations* through
   second order, resolving the zero KK mode (radion + Einstein-frame
   metric/mass/tension backreaction) and n=±2 coupled gravitational sector.
   Include every independent constraint, Bianchi identity, and
   horizon/outer boundary condition. Demonstrate that the already
   calculated scalar source is consistent with the remaining equations;
   do not infer full consistency from one component without checking.
2. Separate particular forced components from independent homogeneous
   solutions, pure gauge, stationary charge data, second-harmonic
   transients and any radiative massive channels. The second-order
   forced n=2 exponential is not automatically an on-shell outgoing
   massive wave.
3. Use a horizon-regular metric/gauge and reconstruct independent
   physical invariants, with a second gauge or direct covariant
   reduction as an adversarial check. Bound radial functions near
   the horizon, through the photon/GL-potential regions and at
   spatial/null infinity; do not discard inconvenient Frobenius tails.
4. For L/r0=16 the n=2 harmonic itself lies in the linear GL-unstable
   range; independently seeded n=2 homogeneous data and the forced
   e^(2 Omega_1 t) component must be distinguished. The reference
   L/r0=10 case is cleaner. If doing a second reference case, prefer
   L/r0=12 or 8; treat L=16 as a deliberately separate two-mode stretch.

**Gate B:** explicit inhomogeneous radial equations, constraint-compatible
solution or fully evaluated Green representation for *both* sectors,
specified independent constants and boundary data, exact symbolic
residual checks and finite-domain error control when numerical ODEs are
used. A scalar-only repeat of GOAL-001 does not satisfy this milestone.

## 4. Milestone C — Prove a nonzero weakly nonlinear domain if possible

For a proposed expansion
  g = gbar + A h_1 + A^2 q_2 + A^3 q_3 + ...,
do not assume uniform accuracy merely because A << 1 at r=2r0.

1. Define explicit dimensionless, horizon-regular weighted norms and
   spatial/time regions in which the hierarchy is tested, including
   the compact circle and the far-zone matching surface.
2. Bound metric corrections A^2 q_2 relative to the first-order
   deformation and background. Ensure positivity/nondegeneracy,
   constraint preservation, asymptotic charge compatibility, and
   freedom from spurious pure-gauge large terms.
3. Determine the first omitted *even zero-mode scalar* correction at
   O(A^4), or provide a mathematically justified remainder estimate
   by a controlled contraction/energy estimate. For scalar news
   N=N_2 A^2 + N_4 A^4 +..., relative error in power is generically
   O(A^2), not a known numerical percentage until N_4 is bounded.
4. Try to give one reproducible nonzero A_* at L/r0=10, with a
   transparent tolerance such as <=10% error in a specified finite
   retarded-time-window energy. If a stronger theorem is possible,
   prefer it; a norm-independent ad hoc A_*=0.1 is **not** acceptable.
   State what region, initial-data class and observable the bound covers.
5. If no quantitative A_* is demonstrable within the budget, derive
   explicit necessary inequalities and name exactly which norm or
   loss-of-derivative/continuous-spectrum estimate obstructs closure.

**Gate C:** reproducible nonzero validity interval with quantified error,
or a transparent obstruction. No using A~1 by extrapolation.

## 5. Milestone D — Genuine finite-time constrained data and transients

1. Construct at least one clearly specified, asymptotically KK-flat,
   smooth/horizon-regular, **constraint-satisfying finite-time**
   initial-data family whose leading growing GL projection is known.
   State how the growing, decaying, gauge and independent n=0 and
   n=2 data are selected. Do **not** multiply an on-shell GL metric
   by an arbitrary time-switch and call it a vacuum solution.
2. Solve or bound the second-order n=0 scalar *retarded initial-value*
   response, showing the independent homogeneous outgoing/incoming
   component and the physical source response separately. A
   no-incoming condition alone generally does not fix independent
   outgoing scalar initial data on the Cauchy slice.
3. Develop a traceable condition under which a finite-time data family
   approaches the GOAL-001 settled exponential coefficient. Give a
   quantitative scalar transient / tail bound in an early retarded-time
   interval when feasible. Test sensitivity to at least two physically
   admissible scalar initial-data choices.
4. Establish whether there is a theorem of *non-universality*:
   E_early is not determined solely by epsilon, k and Omega without
   specifying independent scalar Cauchy data. Distinguish such a
   valid negative result from failure of the GL-driven source.
5. Obtain the windowed spectrum from actual smooth finite-time
   news rather than a sharply windowed eternal exponential whenever
   feasible. Report effects of switching/initial transients and
   causal arrival times.

**Gate D:** a specified physical initial-value problem and controlled
comparison to the particular exponential solution, or a theorem
identifying the unavoidable data dependence.

## 6. Milestone E — Third order, amplitude evolution and bulge bridge

1. Derive the cubic n=±1 forcing from q_2 and h_1. Investigate whether
   a physically meaningful adjoint/spectral projection gives a
   growth-rate correction or a normal form
     dA/dt = Omega A + gamma A^3 + ...
   under an explicitly fixed amplitude and clock. Do not infer a
   stable saturation from a sign of gamma at D=5.
2. Treat small-Omega (near marginal GL threshold) and finite-Omega
   exponential growth as potentially distinct perturbative limits.
   Check consistency with Gubser's *static* perturbative nonuniform
   string branch and Sorkin's phase-structure discussion; a static
   first-order transition does **not** fix a dynamical growth
   coefficient or provide a physical early-waveform estimate.
3. Identify any resonant secular terms, zero-mode mass/tension drifts,
   horizon boundary absorption, adjoint nonnormality or continuous
   radiation spectrum obstructing a local-in-time amplitude equation.
   If normal-form reduction fails, record a precise spectral or
   functional-analytic obstruction and propose a safer nonlocal
   equation rather than inventing gamma.
4. Define a defensible map from A to an *observable* early bulge
   distortion. Apparent-horizon formation times are slicing dependent;
   fix asymptotic time and invariant sphere radius, or use another
   explicitly geometrically specified surface. Retain the
   GOAL-001 log(epsilon) time translation only within its domain.
5. If the controlled regime reaches a clearly defined early-bulge
   event, integrate radion flux and report E_early/M0 with an
   actual error budget. Otherwise report a finite-interval weak
   energy, its exact physical meaning, and quantitative limitations.
   Do not reclassify a weak-amplitude interval as the first bulge.

**Gate E:** mathematically consistent amplitude dynamics with declared
domain/normalization, or a documented obstruction; separately honest
early-energy claim status.

## 7. Milestone F — Adversarial verification and Michael handoff

- Two independent adversarial audits, not merely an LLM rereading its
  own output: (i) full nonlinear source/constraint/gauge/horizon
  response and (ii) finite-data/energy/amplitude bounds. Each must
  run independent symbolic or numerically bounded checks of the
  relevant claims and preserve failed mutations. Record whether
  audits were independent AI agents rather than external referees.
- Independent code path or alternate-gauge identities for central
  coefficients. Include error estimates for ODE boundaries,
  Frobenius series, quadrature, zero-mode charges and data transients.
- Recheck all factors of G5, L, 4 pi, real-cosine harmonic halves,
  source/Green Wronskian sign and group velocity where needed.
- Update research/sav/STATUS.md, SOURCES.md, verification/INDEX.md,
  verification/negative_results.md and handoffs/MICHAEL_START_HERE.md.
- Produce goals/GLR-GOAL-002_REPORT.md as an honest scientific paper-like
  note, containing derivations, explicit assumptions, failures,
  evidence hierarchy, reproducible calculations, audits and next goal.
- Give Michael a **science-only** interface with the invariant
  observable, mode normalization, raw data requirements, early
  time-window selection and tests. No production code changes.

## 8. Budget, autonomy and stopping conditions

**Scientific budget:** one main physical case L/r0=10. After it passes
the scoped gates, one cross-check L/r0=8 *or* 12; reserve L/r0=16
(two unstable harmonics) for stretch/theoretical treatment only. Use
existing GLR-001 profiles/results rather than recomputing them unless
an error is shown. At most three explicitly refined 1D discretizations
per genuinely new radial solve per selected sector and two substantive
repair approaches for each central mathematical obstruction. Do not
perform a large k sweep, arbitrary dense spectral campaign, 2+1
nonlinear evolution, or horizon/GRChombo production run. Symbolic
work and bounded 1D inhomogeneous ODE/Green-function analyses are
permitted under research/sav/**; publish exact commands and outputs.

Milestone order is flexible; prioritize a meaningful finite-energy
statement. If full q_2 is too expensive, do not spend the whole
budget on one gauge; instead deliver a clear partial analytic
reconstruction, a closed formula for missing fields and a
computationally realistic next step. Never claim an error enclosure
from ODE tolerance or empirical refinement alone.

**Stop immediately on** unauthorized write/run/merge; unresolved
inconsistency in the GOAL-001 source; gauge-dependent purported
physical energy; inadmissible finite-data construction; failure of
more than two bounded repair strategies without human review; or
a need to change Michael's equations/production workflow.

**Required terminal classification:**
COMPLETE_VERIFIED / MEANINGFUL_PARTIAL /
BLOCKED_BY_MATHEMATICAL_OBSTRUCTION / NOT_REPRODUCIBLE /
OPEN_NEEDS_APPROVAL.

- COMPLETE_VERIFIED requires the prescribed data/observable and
  explicit finite-A interval or rigorously qualified energy bound,
  with independent mathematical/physical validation and full errors.
- MEANINGFUL_PARTIAL covers a correctly constrained second-order
  solution, rigorously stated non-universality/data dependence,
  or constructive amplitude/transient results lacking finite-A closure.
- BLOCKED_BY_MATHEMATICAL_OBSTRUCTION requires the precise
  obstruction and failed strategies, not a generic "too hard."
- An exact cancellation or a proved absence of a universal early
  energy fraction is a valid negative scientific result. Preserve it.
- No uncontrolled claim that first-bulge emission dominates the cascade.

## 9. End-of-task deliverables

1. Source-backed derivation(s) in research/sav/physics/, specifying
   regular gauge and boundary/charge conventions.
2. Reproducible checks, independent audits, negative results and
   machine-readable bounded outputs in research/sav/verification/.
3. A quantified energy/error table **only if** its amplitudes and
   initial data are demonstrably admissible and controlled; otherwise
   explicit conditional/obstruction tables.
4. goals/GLR-GOAL-002_REPORT.md and updated scientific handoff for Michael.
5. Exact list of commits on research/sav-radiation, no unapproved merge,
   no production simulation and clear closure classification.

## Append-only execution ledger — 2026-10-22

- Launched by explicit user instruction T-20261022-GLR-GOAL-002 in this
  chat. Input branch SHA 8d4163cd876b33752f7fb6938bb7861fe221b7c0;
  complete controlling goal/review matrix and scientific inputs read.
- Milestone A: independent quadratic connection/warped-equation recheck
  finds no substantive GOAL-001 source defect. Regular all-component EF
  limits, gauge/charge conventions and source/linear tests recorded.
- Milestone B: explicit seven-component radial generator, all-source
  Bianchi control, zero-mode Einstein metric quadratures and forced n2
  trace/divergence derived. Neither regular DAE nor covariant tensor
  route evaluates the n2 boundary inverse/global norm. Gate B NOT PASSED.
- Milestones C/D: explicit regular norm and missing inequalities; exact
  scalar-only KK constraint family; second-order GL non-universality with
  conditional exact-family extension; scalar transient Killing-energy
  inequality. P/Z data are fixed at perturbative order, not an exact
  simultaneous finite-A GL construction. Three L10 scalar configurations
  give empirical Q≈80.8179176, conditional transient-only T≈32.2843r0.
  No finite-A GL interval or physical emitted percentage certified.
- Milestone E: lower gates incomplete, so no third-order forcing/gamma
  or bulge campaign. Convention dependence checked, static/dynamic limits
  separated, no A~1 or early-dominance extrapolation.
- Milestone F: independent audit_geometry and audit_energy agents ran
  separate actual symbolic/manufactured checks and mutations, then read
  primary artifacts. They are AI scientific audits, not human referees.
  Reports/results, negative record and science-only Michael handoff saved.
- Budget: main L10 only, reused GOAL-001 profile/Omega; three scalar Green
  refinements, no GL roots/new n2 grids/spacetime evolution. Two equation/
  norm routes per central missing gate; no further repairs launched.
- Terminal MEANINGFUL_PARTIAL. Full report GLR-GOAL-002_REPORT.md and
  verification/GLR_GOAL002_CHECKS.md distinguish all failed gates.
  Scientific and closing commit refs appear in the closing provenance
  amendment/final delivery. No other paths, branches, production workflow,
  manuscripts/Overleaf, simulations, merges or subsequent Goal changed.

- Publication milestone: scientific commit **6eb617e4d22b1ca8e8641dcf1f80ca5d9c64e681**,
  tree e8f5a1e45cdb9b69bc75fd407093e9bf14283ff2; verified 31 changed/added
  research blobs, zero removals/outside changes, expected-head branch
  update without force. Closing amendment only pins provenance/pointers;
  both execution commit SHAs are supplied in final chat delivery.
