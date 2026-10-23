# GLR-GOAL-004 — Controlled first-bulge radiation: nonlinear bounds and effective dynamics

**Task:** T-20261023-GLR-GOAL-004  
**State:** PREPARED / READY FOR OWNER-OR-COLLABORATOR LAUNCH; NOT EXECUTED  
**Scientific objective:** analytical control of gravitational radiation during the initial nonlinear bulge of a five-dimensional Gregory–Laflamme unstable black string.  
**Repository and branch:** inaeyk/GL-with-AI, research/sav-radiation.  
**Only writable directory:** research/sav/**; historical results remain intact.  
**Possible operator:** Savdeep Sethi OR Michael Yan, one designated active executor at a time.  
**Scientific collaboration:** Savdeep Sethi and Michael Yan; Pau Figueras is an advisor.  
**Default Codex model:** GPT-6.1 Sol / High, standard speed.  
**Production constraints:** DO NOT run spacetime numerical simulations, edit GRChombo/CCZ4 code, change production parameters, alter root instructions, modify the existing numerical branches, merge or launch a follow-on task automatically.

## 0. The primary challenge and a result worth obtaining

Our primary question is not whether exact SO(3) symmetry admits radiation; it does. The original unstable n=±1 GL mode nonlinearly sources the massless n=0 KK radion at second order. The strong evidence for nonzero transfer is already recorded. The challenge is to establish **how much outgoing radiation is emitted in a physically specified, finite-time, constraint-satisfying vacuum evolution**, and, ideally, how that flux relates to the **first large bulge**, not the later pinch-off cascade.

Develop the strongest mathematically controlled source-to-radiation theorem, or a constructive verified computational estimate with fully stated bounds, for an exact one-parameter 5D vacuum initial-data family and a nonzero finite amplitude interval. A target form is

$$
N_\phi(u;\epsilon)=N_{\rm ref}(u;\epsilon)+\Delta N(u;\epsilon),\qquad
\|\Delta N\|_{L^2(U)}\leq b_U(\epsilon),
$$

where N_ref includes the genuine GL-generated quadratic response and the selected independent scalar initial data, not an implicit assumption that they vanish. Prove, or conditionally bound with transparent unclosed constants,

$$
\frac{(\max\{0,\|N_{\rm ref}\|_U-b_U\})^2}{G_4}
\leq E_0(U)\leq
\frac{(\|N_{\rm ref}\|_U+b_U)^2}{G_4}.
$$

If b_U / ||N_ref||_U <= sqrt(1.1)-1, the corresponding reference-energy error is at most 10%; that is a **proposed target**, not a theorem or a preapproved production criterion.

A mathematically valid nonzero interval 0 < epsilon <= epsilon_* or 0 < A <= A_* must state the horizon-regular geometric norm, full data class, spacetime time interval, asymptotic Bondi news construction, and all source/propagation/remainder constants. If the interval does not reach first-bulge formation, say so unambiguously.

**Parallel major objective:** construct the best honest semi-analytical model of the first bulge in the exact SO(3) sector, obtaining a predicted source history and radion waveform/energy spectrum with a systematic approximation hierarchy and falsifiable uncertainty bands. This path may proceed even if the theorem remains obstructed. It is **not** permitted to convert a model fit or residual into a rigorously controlled error bound unless an independent stability estimate justifies it.

A separate extension should examine a **light stabilized radion**: in pure 5D vacuum Einstein on R^{1,3} x S1 it is massless. Stabilization requires additional dynamics. The EFT must not be fed back into the vacuum GL background/modes without a consistent matched model.

## 1. Input state and non-negotiable context

Start by inspecting the actual branch head and reading, at minimum:

- research/sav/AGENTS.md, HANDOFF.md, STATUS.md, NOTATION.md, SOURCES.md, DECISIONS.md, QUESTIONS.md;
- research/sav/goals/GLR-GOAL-001_REPORT.md, GLR-GOAL-002_REPORT.md, GLR-GOAL-003_REPORT.md;
- research/sav/verification/GOAL003_REVIEW_QUALIFICATIONS.md and negative_results.md;
- research/sav/physics/goal003_forced_master.md, goal003_master_independent.md, goal003_data_nonlinear.md and the mass/EFT note linked from GOAL-003;
- the 001/002 scalar source, retarded transfer, horizon-regular, finite-time and initial-data derivations;
- GOAL-003 actual verification ledger and both audits; original user-approved scientific boundaries in this directory;
- the selective literature review and primary references, including Gregory–Laflamme, Gubser, Collingbourne, Lehner–Pretorius and Figueras–França–Gu–Andrade.

**Do not replay completed Goals.** GOAL-001, 002 and 003 all closed MEANINGFUL_PARTIAL. GOAL-003 established a forced n=2 gauge-invariant master and affine all-seven-equation reconstruction for nonzero K, s>0; an analytic L/r0=10 nonresonance/positive-supersolution argument; effective derivative-loss regular-metric point-inverse bounds; qualitative exact GL+scalar constraint data on finite regular collars; formal third/fourth-order source recursions; a linear apparent-horizon map; and a distinct conditional stabilized-radion EFT.

But GOAL-003 **did not** establish: physical L10 source-evaluated M2; a general retarded nonlinear propagator; global asymptotically KK-flat fixed-ADM GL+radion initial data; a quantitative positive amplitude interval; a bounded fourth-order news map; a nonlinear bulge/horizon remainder; or a physical early-bulge radiation fraction. The later independent review qualified how distinct the "W versus R" source derivations really were and noted shared authorship of aspects of the inverse audit. Do not inflate that audit independence. Retain the exact prior hypotheses.

Canonical vacuum conventions: f=1-r0/r, k_n=2pi n/L, G4=G5/L, M0=r0/(2G4), real cosine amplitude and correct +/-n factors; fixed asymptotic clock, circle length and phase. The n=0 scalar at four-dimensional Bondi infinity obeys P0=(dPhi/du)^2/G4. The first-order GL normalization H^U_theta theta/(2r^2)=1 at r=2r0 defines the reference amplitude A, not the event-horizon size, not a generic chi seed.

Reference L/r0=10: Omega*r0 approximately 0.06328119; C0 approximately 12.30729, both empirically convergent but not interval-certified. Near-horizon unit-amplitude sampled regular-frame magnitude about 39.47, **not** a global norm. Existing scalar mismatch Q approximately 80.8179 is a finite-data scalar-linear diagnostic, **not** a nonlinear radiation remainder. The L10 n=2 positive-supersolution estimate H_K >=0 for (Kr0)^2>=3/2 yields an inverse at s>0, not a spectral gap at the horizon nor a uniform time-domain estimate. Keep all such distinctions.

The premise is **exact SO(3)**. Hence massless 4D tensor Bondi news vanishes, but massless KK scalar monopoles and above-threshold massive KK singlet modes survive. Nonspherical l>=2 radiation is not computed here. KK harmonic number n is distinct from angular l.

## 2. Milestone A — Turn the GOAL-003 inverse into a physical source-evaluated quadratic bound

1. Independently recheck the forced n=2 master, sourced trace/divergence, seven Einstein equations and affine metric reconstruction for the actual physical L10 GL mode. Cross-check the prior review qualifications. Do not repeat an analytic-continuation negative-k^2 check and call it a physical-mode verification.
2. Construct **rigorous endpoint envelopes or independently validated numerical enclosures** of the normalized physical L10 GL eigenfunction and its derivatives, including horizon Frobenius remainders, Schwarzschild-to-EF cancellations, exterior Volterra tails and eigenvalue error. Keep source coefficient bounds uniform under those uncertainties.
3. Insert these envelopes into the actual complete n=0 and n=2 quadratic sources, and evaluate or sharply bound their conserved weighted input norms; distinguish forced response and permitted homogeneous modes and mass/charge parameters.
4. Explicitly evaluate the GOAL-003 source-to-regular-metric estimate, its derivative loss and response profile, possibly improving the generic s^-2 resolvent bound by a source-aware Green-function estimate. Treat resonance-like equal-exponential tails, the actual source power, and nonlogarithmic forced asymptotics without a false homogeneous Robin truncation.
5. Derive a **numerical or certified M2** in a specified horizon-regular physical norm, with a complete boundary/matching/input error budget, or identify the first unbounded constant and an exact obstruction to evaluation.

**Gate A:** source-evaluated quantitative M2 or its precise missing inequality; a restatement of the known existence of C_alg is not sufficient.

## 3. Milestone B — Exact global constraint-compatible finite-time GL data

1. Extend the existing exact local horizon-crossing SO(3) GL+scalar family to an **asymptotically KK-flat global exterior initial slice**. Control differentiated radial constraints and momentum variables in weights sufficient for an ADM mass/tension limit, constraints and gauge reconstruction.
2. Fix an explicit all-orders prescription for free data, including growing GL tangent, zero independent n=2 seed unless deliberately varied, zero KK momentum, independent n=0 scalar data, and homogeneous radiative sector. Fix asymptotic circle L, phase, clock and total ADM mass; tune the charge/KID kernel with a demonstrably invertible derivative or record obstruction.
3. Establish a bounded right inverse for the linearized/global vacuum constraint map in the chosen function space and a tame implicit-function/contraction estimate for its nonlinear completion. Quantify the radius in epsilon when possible. Distinguish existence near each finite collar from global existence on a complete KK exterior.
4. Compare at least TWO explicitly admissible scalar-data prescriptions — e.g. particular-matched and zero-radion choices — without illegally switching a GL solution in time or adding two exact nonlinear metrics. Prove and quantify the resulting leading radiation/data dependence.
5. State how the data family selects all order-four homogeneous radion data: without this prescription the fourth-order news is not determined by lower-order coefficients.

**Gate B:** an exact global family (with charges and norm) or a rigorous, explicit global-constraint obstruction. A local collar family, perturbative constraint solution, or independent scalar-only family is not full closure.

## 4. Milestone C — Nonlinear evolution and a physical radiation trace

1. Work with an appropriate **gauge-fixed hyperbolic form** of the exact SO(3)-reduced 2+1 Einstein/dilaton system, including all constraints and regularity at an ingoing future horizon, rather than assuming a gauge-free dynamical energy estimate.
2. Construct a redshift/horizon-regular near-zone energy, suitable exterior weighted energy, and a radiation trace map to the n=0 Einstein-frame Bondi news. Specify null/timelike boundary treatment and conserved-charge/gravitational constraint error.
3. Prove a local nonlinear solution estimate whose constants depend transparently on the compact circumference and weighted norms; address loss of derivatives in the source-aware n=2 reconstruction with tame estimates, appropriate gauge choices or alternate variables.
4. Track nonlinearly driven growth through a window T(epsilon) and separate unstable fundamental amplification from slower/transient modes. For logarithmic time T~Omega^-1 log(A*/epsilon), justify a projected propagator with a growth constant sigma in the precise condition sigma<p Omega when such an estimate is used; do not apply fixed-time Gronwall without control of its epsilon dependence.
5. Derive a Lipschitz/differentiable map from exact data to radion news in an L2(U) or appropriate weighted trace norm, with explicit constants or a clearly conditional bound.

**Gate C:** a nonzero quantified spacetime interval, preferably growing with log(1/epsilon), and a news-trace estimate applicable to the exact family. If only local qualitative existence follows, document precisely why the radiation trace does not follow.

## 5. Milestone D — Fourth-order massless radion, physical finite-window energy and spectrum

1. Use the full ordered third- and fourth-order Einstein recursions, constraint/gauge completions, the source-evaluated q2 response and chosen exact data to construct the physical **n=0 fourth-order** radion source and outgoing retarded response. Include q3 at n=1,3 and q4 at n=0,2,4 as required by the actual hierarchy; do not assume a scalar-only quartic source.
2. Specify all homogeneous free scalar data at each order, the Bondi coordinate and areal-radius shifts, mass/tension changes and scalar-memory terms. Validate parity under z->z+L/2 for the covariance of the chosen family.
3. Obtain a bounded difference N_phys - N_ref in an actual **finite causal observation window U**. The reference news may contain free n=0 data, so retain waveform interference; the GL-only particular is a special preparation, not universal.
4. Derive a two-sided energy estimate, smooth-window spectrum and error on the integrated E_rad(U)/M0. If using a lower bound, prove b_U<||N_ref||_U and reference-norm positivity; if not, only report an upper/conditional estimate.
5. Report whether the controlled window reaches an invariantly defined onset or finite-contrast criterion for the first bulge. Do not infer a first-bulge fraction from A~1 or treat a geometric peak time in past simulations as an extracted radiative pulse.

**Gate D:** a physical bound with declared data/time/geometry and independently validated uncertainty, OR the strongest exact conditional inequality with a named missing source/propagator/remainder constant.

## 6. Milestone E — Parallel ambitious semi-analytical model of the first bulge

This track is a major independent deliverable **even if Gates A–D remain open**. It must not be misrepresented as a theorem.

1. Start from the exact SO(3) orbit action and its one local physical configuration degree. Propose a low-dimensional or matched near/far **effective source-history model** for first-bulge development, ideally using a physically defined area-radius or horizon deformation and mass/tension bookkeeping.
2. Analyze at least two candidate closures, e.g. near-threshold GL amplitude plus stable-mode memory; matched nonuniform-string/blackfold amplitude; covariant projection onto a small set of constrained modes with a nonlocal radiation kernel. Derive, do not assume, every retained coupling. State why the discarded modes are small or define a computable residual/evidence-based uncertainty instead of false control.
3. Match the weak limit to GOAL-001 C0 and GOAL-003 q2/master and the appropriate linear GL growth; make the model reproduce SO(3) and n-convolution selection, zero stationary GP/news controls, and positive outgoing scalar flux. Initial free scalar data must remain an explicit input.
4. Make **dimensionless falsifiable outputs** as functions of k r0, initial GL amplitude/preparation and radius-contrast threshold: first-bulge onset time, characteristic emission width/frequency, Phi(u), dE/du, dE/domega and cumulative E_early/M0. If coefficients are not determined, give parametric families and bounded sensitivity bands, not invented percentages.
5. Compare to existing published horizon-formation time/size information **only as a geometry benchmark**. Those papers do not supply emitted flux. Identify observationally distinct early-dominant and cascade-dominant scenarios, including possible high-frequency massive-KK transport.
6. Include a clear scope-control verdict: controlled approximation / calibrated heuristic / uncalibrated conjecture. Analyze model residuals, energy bookkeeping, near-zone backreaction and strong-field failure. Do not run 2+1 full nonlinear vacuum spacetime evolution or production GRChombo.

**Gate E:** one explicit tractable source-to-waveform model with quantitative/parametric early-radiation predictions and an adversarial physical validity assessment, or a precise demonstrated closure obstruction and reduced alternative.

## 7. Milestone F — Separate stabilized light-radion scenario and symmetry outlook

1. Keep the primary five-dimensional *vacuum* Einstein theory unchanged. In a separate four-dimensional Einstein-frame EFT include a consistent stabilizer with, for example, U(phi)=m_phi^2 phi^2/2+..., specifying action/stress exchange and assumptions needed to preserve the asymptotic circle background and the GL source. A transport-only use of the vacuum near-zone source must be explicitly labeled conditional.
2. Determine massive scalar transport through large timelike-radius worldtubes, mode thresholds, group delay, quasi-bound/backscatter/tails and finite-pulse spectral weights omega sqrt(omega^2-m_phi^2) when applicable. Never equate massive timelike flux with the massless n=0 Bondi null-infinity news; imaginary GL growth frequency is not an on-shell threshold switch.
3. Parameterize corrections by m_phi r0 AND m_phi/Omega; quantify under what assumptions the massless calculation approximates the physical early spectrum. Retain the same SO(3) selection rules; distinguish the n=2 **massive spin-2 KK gravitational channel** from the stabilized n=0 radion mass.
4. If a microscopic consistent stabilization is unknown, produce only conditional EFT propagation/scaling predictions, not an alteration of the original black string or a physical number.

**Gate F:** independently normalized massive-flux model with honest matching/validity regime and fixed-vacuum-theory separation.

## 8. Independent audits, methodological autonomy and reproducibility

- Arrange **two genuinely separate adversarial scientific reviewers**. Reviewer I: physical source-evaluated q2, regularity, global constraints, inverse/radiation-metric norms. Reviewer II: nonlinear news map, initial-data freedom/interference, finite-window energy, semi-analytical closure and stabilized-EFT distinctions. Delegate bounded tasks and do not allow an investigator to be the sole author and sole auditor of its main estimate. Disclose any shared implementation or derivational path.
- Each reviewer should independently derive at least one central identity, inspect the actual scripts/source, run exact or bounded controls, intentionally mutate wrong signs/factors/gauge terms or asymptotic branches, and report gaps. AI audits are **not** external human referees.
- Methodological autonomy is substantial: Codex may choose a better gauge, source-aware Green formulation, constrained variable, matched asymptotics, Nash–Moser/tame scheme or analytic integral representation, provided it reconciles the established observables and reports errors.
- Preserve mathematically meaningful negative results (no global ADM completion, failed source envelope, unbounded radiation trace, model breakdown, etc.) without claiming a universal physical no-go.
- At each substantive checkpoint update local scientific notes and verification ledger, but publish to the shared branch only at preauthorized checkpoints with the coordination policy. See GOAL-004_RUN_BY_SAV_OR_MICHAEL.md.
- When symbolic/numerical checks execute, retain exact commands, dependency versions, source hashes, deterministic inputs, residuals, results and counterexamples. A passed identity is not automatically a solved boundary/IVP.

### Finite research budget

Primary physical case **L/r0=10**, using existing profiles/eigenvalues wherever possible. Optional **one** comparison L/r0=8 if Gate A and the primary theory are sufficiently mature. L/r0=12 requires additional second-harmonic spectral work; treat as an explicitly unfunded stretch, not an automatic scan. No L/r0=16 production or 2-mode simulation.

Permitted computations: symbolic derivations, bounded independent exact checks, 1D linear/inhomogeneous master ODE/Green-function calculations, manufactured pulses, interval/Volterra or validated quadratures, and small explicitly identified effective-model ODE/quadrature exercises. These are *non-production mathematical verification*. Do not run numerical full 2+1 Einstein evolution, 5D CCZ4/GRChombo, MPI/AMR, cluster jobs or general parameter sweeps.

At most **three refined physical 1D configurations per newly evaluated sector and chosen k**, plus at most **two substantive repair strategies** per central failed mathematical gate before writing an obstruction/decision record. New substantial parameter families, heavier scans or further repair budgets require explicit human authorization. Reuse validated existing results; do not spend resources reproducing historical campaigns with no identified defect.

### Stopping and milestones

This is one ambitious extended Goal, not a series of automatically launched subgoals. The mathematical tracks may run concurrently within the stated scope. A failed rigorous gate **does not force abandoning** the independent semi-analytical/EFT tracks; conversely, those tracks never certify a theorem by analogy.

Stop on: production or external-branch write requirement; need for Michael's running simulation; unauthorized resources; unresolved invalid GOAL-003 equation; an irreparable constraint/gauge conflict after two bounded approaches; inability to control positive reference news while claiming a lower bound; or exhaustion of the bounded budget.

Classify separately:
- EXACT DERIVATION or INDEPENDENTLY AUDITED IDENTITY;
- CONTROLLED APPROXIMATION or CERTIFIED NUMERICAL BOUND;
- CONDITIONAL MATHEMATICAL STATEMENT;
- SEMI-ANALYTIC HEURISTIC / SPECULATIVE MODEL;
- BLOCKED / OPEN.

Final task classification is exactly one of COMPLETE_VERIFIED, MEANINGFUL_PARTIAL, BLOCKED_BY_MATHEMATICAL_OBSTRUCTION, NOT_REPRODUCIBLE, OPEN_NEEDS_APPROVAL. A strong source-evaluated M2 or global data theorem without finite-A energy is **MEANINGFUL_PARTIAL**, not completion. A quantitative first-bulge fraction without a defensible approximation/error basis is unacceptable.

## 9. Required final artifacts and human handoff

1. research/sav/goals/GLR-GOAL-004_REPORT.md: substantial scientific report with a yes/no up front for (i) positive controlled A_*, (ii) physical finite-time energy interval, (iii) actual first-bulge fraction, and (iv) independent semi-analytic predictions. Record proof dependencies, formulas, units, numerical uncertainties, model assumptions, audit verdicts, negative results, exact commits and next priorities.
2. research/sav/physics/: source-evaluated second-order metric/data construction, nonlinear radiation estimates, fourth-order source and/or near/far first-bulge model, as far as achieved.
3. research/sav/verification/: independent audits, executable reproducibility checks, outputs, failed tests and quantitative bounds/error budgets. Never suppress unsuccessful source-norm or constraint estimates.
4. Update research/sav/STATUS.md, SOURCES.md, verification/INDEX.md, verification/negative_results.md, HANDOFF.md and handoffs/MICHAEL_START_HERE.md, without altering prior scientific reports or their evidence.
5. A focused science handoff listing what Savdeep or Michael can test/measure eventually, with required invariant field normalization, initial-data preparation and finite-worldtube versus null-infinity channels. No numerical implementation or production action without separate approval.
6. On the authorized research branch, coherent checkpoint/final commits with exact provenance and verified research/sav-only diffs; no merges, force pushes, production code edits, manuscript/Overleaf work, external collaborator messages or automatic next Goal.

## Execution completion annotation — append-only, 2026-10-23

The owner explicitly launched this task with EXECUTOR: Savdeep Sethi and
PUBLICATION MODE: authorized sole writer. The preparation state at the
top is historical. Execution input was the verified clean
research/sav-radiation@2f4648c379c474ed124e3ef10b62efcd4a783c66.
The bounded investigation has terminated **MEANINGFUL_PARTIAL**.

[Final report](GLR-GOAL-004_REPORT.md),
[executed append-only milestone ledger](../verification/GLR_GOAL004_CHECKS.md),
[Audit I](../verification/AUDIT_GOAL004_GEOMETRY.md) and
[Audit II](../verification/AUDIT_GOAL004_DYNAMICS.md) record actual
derivations, commands, budget use, failures and qualifications. Physical
C5/M2 enclosure, exact global simultaneous GL data, nonlinear radiation
trace/N4 and actual first-bulge fraction remain open. Four explicit
effective waveform models and separate EFT pulse identities are delivered
with uncontrolled physical-error status. Three physical n=2 inverse
configurations and the two bounded repair approaches are exhausted;
no third approach, production action or subsequent Goal is launched.
