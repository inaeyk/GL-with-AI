# GLR-GOAL-005 — Certified GL mode, complete sourced quadratic geometry, and first controlled radiation input

**Goal ID:** T-20261023-GLR-GOAL-005  
**State:** OWNER-AUTHORIZED FOR PREPARATION; READY TO RUN; NOT EXECUTED  
**Repo/branch:** inaeyk/GL-with-AI / research/sav-radiation  
**Exclusive write scope:** research/sav/**. Do not modify other code, branch history or numerical infrastructure.  
**Eligible executor:** Savdeep Sethi OR Michael Yan, **one active branch writer only**, after confirming permissions and a clean research workspace.  
**Codex preference:** GPT-6.1 Sol / High / standard speed.  
**System:** Five-dimensional vacuum Einstein on asymptotic R^{1,3} x S^1, uniform 5D Schwarzschild string with exact transverse SO(3). No numerical full 2+1 or 5D spacetime evolution. No GRChombo/CCZ4/MPI/AMR, production campaign, cluster launch, manuscript or Overleaf action.

## 0. Big mathematical challenge — close the first genuinely quantitative obstruction

GOAL-001 obtained the gauge-completed quadratic massless radion source and its retarded coefficient C0; GOAL-002 derived complete second-order equations and an initial-data-dependent scalar energy inequality; GOAL-003 proved the forced second KK harmonic's gauge-invariant master, affine reconstruction and an effective inverse at the separated growing frequency; GOAL-004 evaluated the physical L10 source but did **not** certify its input, source or metric norm. The GOAL-004 few-mode early-bulge model is **not convergent**: the discarded relative residual changed from about 0.63 to about 0.80 and radiation energies changed nearly tenfold. This is a negative result, not a calibration of the physical first-bulge flux.

**Primary objective:** obtain a *validated, horizon-regular, globally weighted enclosure* for the physically normalized unstable GL eigenpair at L/r0=10, propagate it through the **complete conserved second-order n=0 and n=±2 Einstein source**, and solve/enclose the forced n=2 physical gravitational response, including all constraint reconstruction, endpoint remainders and nontrivial homogeneous/charge choices. Produce the first scientifically defensible **source-evaluated full quadratic metric norm M2** (or a precisely isolated mathematical obstruction with the strongest certified partial bounds).

This is the most promising missing link before a finite-amplitude news theorem. It is not merely "refine a numerical ODE"; a correct result must connect interval eigenmode control through nonlinearly coupled source derivatives to an all-component horizon-regular Einstein metric bound.

**Second ambitious objective, contingent on primary success:** give rigorous/qualified bounds on the **second-order radion retarded transfer C0** and on leading finite-time scalar energy for precisely specified homogeneous data; then determine exactly how much more is required for a nonzero physical amplitude A_* and radiation-trace bound. If feasible, push to a source-aware quantitative third/fourth-order news-error estimate. Do NOT equate a bounded separated q2 with an exact nonlinear Einstein evolution or claim a first-bulge fraction without an exact global initial-data family and controlled nonlinear remainder.

A **small stabilized radion** remains a distinct conditional KK EFT extension, NOT a modification of the primary vacuum GL equation. No additional phenomenological massive-radion sweep or ineffective two-/three-mode bulge fitting is a Goal-005 priority.

## 1. Authoritative inputs, scientific status and conventions

Before doing mathematics, read and independently challenge:

- research/sav/AGENTS.md, HANDOFF.md, STATUS.md, NOTATION.md, SOURCES.md, DECISIONS.md and verification/INDEX.md;
- completed GOAL-001/002/003/004 reports and GOAL-004 audits (especially physical source diagnostics, unclosed C5 estimate and nonconvergent fluid models);
- verification/GOAL003_REVIEW_QUALIFICATIONS.md. Do not overstate independence of the W/R route or the earlier inverse audit.
- physics/gl_linear_mode.md; physics/quadratic_radion_source.md; physics/radion_retarded_transfer.md; physics/goal003_forced_master.md; physics/goal003_master_independent.md; physics/goal004_source_geometry.md; physics/goal004_data_news.md; verification/negative_results.md.
- prior validation scripts/outputs for L10 and both separate GOAL-004 audits. Reuse as evidence *after* checking normalization/source fields. Old empirical data never becomes mathematically certified just by citing it.

Baseline execution input: research/sav-radiation@**96339836a1281c219917f900224e7c98cf62f3f8**. Verify the actual updated branch head at task launch; do NOT reset it to the baseline. Preserve all completed scientific reports and negative records.

Conventions: signature (-++++), f=1-r0/r, z~z+L, k=2pi/L, L/r0=10, G4=G5/L, M0=r0/(2G4), GL eigenfunction with real A(t)H(r)cos(kz). The *first-order* invariant normalization is H^U_theta theta/(2r^2)=1 at r=2r0 in the completed nonzero-k unitary gauge h^U_zA=0. The separated growing mode is A(t)=epsilon p1 e^{Omega (t-t0)}, with well-specified amplitude projection and coordinate clock; do not confuse A with numerical chi or the horizon bulge. For the scalar phi_0=Phi(u)/rho+..., dE0/du=(dot Phi)^2/G4 at null infinity. Massive n=2 gravity is a separate timelike-worldtube channel if it has real frequencies above 2k.

Existing **empirical, not enclosed** L10 data: k r0=2pi/10, Omega r0~0.0632811889759; C0~12.30729197; sampled n=2 source L2 ~16.64667 and truncated response norm ~113.15; sampled complex +2 regular tetrad max ~92.67, NOT full M2. DO NOT use the unverified 39.47 first-order horizon norm as a rigorous supremum. The old n=2 positive-supersolution H_K>=0 at K^2 r0^2>=3/2 (K=2k) proves the separated resolvent upper bound ||(H_K+s^2)^-1||<=s^-2 for s=2Omega>0; no time-domain semigroup estimate, horizon spectral gap or source-evaluated M2 follows.

## 2. Milestone A — Mathematically validate the physical GL eigenpair

### A1. Two-ended spectral root and branch existence

Obtain a constructive, reproducible enclosure for the *physical nodeless* positive unstable mode at k r0=2pi/10 by matching the ingoing-horizon and decaying-infinity Schrödinger/Riccati solutions, with their correct normalization at r=2r0.

The published/previous monotonicity identity has the schematic form

$$
F'(\Omega)=2\Omega\left[
\int_{-\infty}^{x_m}\left({u_H\over u_H(x_m)}\right)^2dx+
\int_{x_m}^\infty\left({u_\infty\over u_\infty(x_m)}\right)^2dx
\right]>0,
$$

for node-free normalized bases and compatible matching convention. Derive it afresh, determine its applicability (including normalization/endpoint differentiation), and use **validated** enclosure of F at two trial endpoints with a sign change and a strict root-uniqueness/positivity argument. Do not claim a global unique GL instability unless that larger assertion is independently proved; an enclosed target nodeless root suffices.

### A2. Horizon and infinity errors, not just small solver tolerances

Implement an independently checked endpoint formulation:

- future-horizon ingoing Frobenius solution, recurrence plus *explicit finite-cutoff tail/remainder* bounds (including derivative jets and proper regular EF scaling);
- exterior Coulomb/Volterra asymptotics with subleading logarithmic/power behavior, bounds on the decaying branch and its derivatives; treat an asymptotic inverse-radius expansion as *asymptotic*, not automatically convergent;
- on a finite interior interval, validated IVP/parameter differentiation/Taylor models or rigorous residual + stability estimates with outward-rounded interval arithmetic, using exact interval inputs for pi, k, Omega and matching normalization;
- positivity/no-node certification and no poles or zeros of Riccati denominators across the chosen physical domain;
- an independent representation of at least one critical identity (e.g. second-order linear master with certified fundamental matrices vs Riccati), with strict check of the derivative convention x=r_*.

The first real success metric is an interval [Omega_lo,Omega_hi] enclosing the physical root and error envelopes for the normalized u, u_x (and enough higher regular derivatives). Report the actual interval widths and whether rounding, endpoint or derivative errors dominate. Do not state "interval proof" if only mp.mpf precision, SciPy rtol, resolution convergence or a *conditional* Volterra inequality has been evaluated in floating point.

### A3. Horizon-regular physical C^5 enclosure

Reconstruct the unitary TT tensor H_AB and its EF/tetrad form. Construct a regular weighted C^5 (or compatible Sobolev with embedding) enclosure, including frame and coordinate derivatives on (i) an ingoing horizon collar, (ii) the intermediate region including r=2r0, and (iii) the whole exterior to infinity, with explicitly chosen weights compatible with the GOAL-003 source spaces. The source contains two field derivatives and the existing reconstruction loses three; the C5 requirement must be derived for the exact chosen norm rather than quoted uncritically.

Provide an inequality resembling

$$
\|H-H_{\rm app}\|_{C^5_{\rm EF,weighted}}\le \varepsilon_5
$$

with fully defined norms, decay exponents and finite bound. Near-horizon Schwarzschild component divergences cancel in the regular metric; DO NOT differentiate separate singular terms numerically and infer false infinities. State how the physical normalization and errors propagate through all components. If uniform C5 is too strong, develop an *honestly equivalent* weighted source-controlling norm or identify the exact derivative-loss obstruction.

**Gate A:** physically meaningful validated eigenpair plus global regular derivative enclosure with actual error constants; if no full enclosure, preserve certified subdomain/root bounds separately. Do not continue claiming physical source certification using mere pointwise saved-profile values.

## 3. Milestone B — The entire quadratic Einstein source, with certified conservation

With a certified first-order input, generate and bound the *complete* quadratic Einstein tensor source, not only R_zz. The project already has exact 5D→3D SO(3) geometry and connection-difference formulas. Use them explicitly for:

- z-Fourier n=0, n=±2, with real cosine vs complex Fourier factors, time exponent 2Omega, fixed background charges, and parity;
- all seven orbit/sphere equations, both independent constraint/momentum sectors, all three Bianchi identities;
- the proper conserved **Einstein** quadratic source rather than bare Ricci Q, with separate errors for numerical evaluation, integration/differentiation and on-shell residual of the physical first-order eigenpair;
- Bianchi/source regular EF cancellations, compact mixing factors, boundary/sphere curvature, and direct Schwarzschild/radion zero-mode projection.

Compute explicit certified/qualified inequalities such as

$$
\|Q[H]-Q[H_{\rm app}]\|_{Y_m}
\le C_B (2B_5\varepsilon_5+\varepsilon_5^2)
   +C_\Omega|\delta\Omega|,
$$

with every coefficient/nonlinear norm/missing derivative defined, not a symbolic placeholder or sample supremum. Choose a targeted Y_0 first (allowing the original three-derivative reserve), then attempt Y_1 or higher if useful for Einstein metric and radiation trace.

Require exact test of an independent *physical positive-k* solution/constraint identity, plus an all-component numerical forced residual with bound that covers interpolation and endpoint uncertainty. Prior local on-shell jet residuals ~1e-30 do not validate a global saved profile with O(1e-6) interpolation defects.

**Gate B:** complete conserved physical source, numerical/analytic global norm and residual/error enclosure; if partial, explain which derivative or endpoint term is still uncontrolled.

## 4. Milestone C — Solve and bound the physical forced n=2 Einstein geometry

For K=2k and s=2Omega, use the GOAL-003 **inhomogeneous** master, source-dependent affine reconstruction and nonzero forced trace/divergence. Never replace them with homogeneous TT. Construct the retarded/decaying separated boundary-value solution with:

1. two-ended horizon and infinity bases; a correct signed Wronskian/Green kernel, forced source-tail powers and polynomial factors; no spurious log-resonance or improper homogeneous Robin boundary truncation;
2. explicit exponential/weighted source-to-master envelope (including the GOAL-004 independently derived comparison |u|<=C_w w under its exact endpoint/source hypotheses) and a source-evaluated upper bound C_w rather than sampled point values;
3. all-component, all-derivative regular EF/tetrad metric reconstruction, including its Bianchi-driven numerator cancellation, divided differences N/(r-r0), charge and homogeneous modes;
4. full actual Einstein residual check on the reconstructed q2, not merely the first-order source identities. Quantify constraints, Bianchi and boundary residuals across the physical radial domain;
5. a physically stated norm M_{2,2} for the particular second harmonic, with real ±2 reconstruction and the correct factor-of-two convention, and a separate bound for allowed homogeneous n=2 data.

At the same level, bound n=0 Einstein-frame mass/lapse radial quadratures and the massless scalar particular, including mass-charge integration, scalar/free homogeneous data and Bondi-asymptotic normalization. Combine only *compatible* sectors and boundary prescriptions to obtain the **full quadratic metric M2**, including its required regular derivatives and weights. If a particular exponential response is not the one selected by finite-time data, label M2_particular and give an explicit bound on the data-selected correction, rather than hiding it.

**Gate C:** source-evaluated finite explicit M2 with verified all-seven-equation constraints/residuals, or a rigorous single-sentence obstruction naming the first missing bound. A finite-grid L2 mode value or sample maximum is insufficient.

## 5. Milestone D — Independently certify the leading massless-radion transfer and energy coefficient

Given the same validated physical H, re-evaluate the second-order n=0 radion retarded overlap and coefficient C0 with a **verified horizon and infinity normalization**, scalar Green/Wronskian sign and phase/clock convention. If successful report an interval for C0 and the conditional leading weak-energy coefficient

$$
\frac{\Delta E_0}{M_0}
=2(\Omega r_0)C_0^2(A_2^4-A_1^4)
$$

for an isolated settled exponential particular response with A<<1, with propagated interval input errors. The result is a **leading perturbative coefficient**, not a measurement of a physical finite-time first-bulge event.

Distinguish two scalar preparations: particular-matched and zero independent n=0 scalar data on a horizon-regular slice; their difference has a homogeneous scalar transient and interference in energy. If possible, use certified finite-slice scalar energy and causal Green tail to give a quantitative *linear* transient/news bound. State which true initial data are exact, perturbatively constraint compatible, locally exact or globally proven, and never claim a fixed nonzero physical early energy without controlling independent data cancellation.

**Gate D:** independently corroborated normalized C0 and a quantified conditional finite-time flux coefficient, or a transparent classification of the remaining error bound. The massive n=2 physical mode at imaginary forcing frequency is not outgoing real-frequency massive flux.

## 6. Milestone E — Push toward the first valid nonlinear interval, but do not overclaim

With full M1/M2, determine the *kinematic* smallness threshold on a chosen regular background time slice from a matrix/operator norm and elliptic/constraint bounds, not from a single component or an assumed A=0.1. A bound on a truncated metric's signature/relative correction does **not** prove a true nonlinear vacuum solution exists or has a controlled news remainder.

Explore substantial stretch objectives, gated by physical evidence:

- improve the GOAL-004 differentiated exterior momentum constraint estimate into a weighted higher-derivative global KK-flat fixed-ADM family with exact scalar preparation; exploit the proven background mass derivative 9/14 but do not treat it as the completed global map;
- derive a redshift/exterior energy and actual n=0 radiation-trace estimate for the nonlinear SO(3) 2+1 system, including compact Fourier interactions, gauge constraints, loss of derivatives and logarithmic GL growth times; quantify growth rate sigma relative to p Omega before claiming uniform residual control;
- obtain a controlled q3 or fourth-order n=0 radion source/news and actual C4 bound for a fixed all-orders data prescription; then consider a *physical* nonzero A_* and the energy enclosure via b_U and the positive reference norm;
- bound the causal dependence of a radially evolving weak GL shape source beyond the eternal exponential. If deriving an exterior retarded source-to-news operator, keep initial scalar data explicit and verify the operator on manufactured pulses.

A genuine finite A_* and an invariant first-bulge event are **stretch objectives**, not required to count a validated physical quadratic solution as an important success. If obtaining a finite A_* also requires exact global data or radiation map estimates not achieved, stop the physical conclusion there. Do NOT reuse GOAL-004's nonconvergent few-mode EOS model as if more modes automatically provide a systematic approximation. It may serve only as a negative calibration/verification test.

The stabilized radion mass in a realistic compactification is a **separate EFT** with model-dependent matching; no artificial mass in the 5D vacuum Einstein equations. Do not repeat earlier pulse-transport toy scans absent a new mathematically justified matching result. Exact SO(3) remains imposed: ordinary 4D tensor Bondi news is excluded, but the n=0 radion and above-threshold KK massive gravitational singlets remain.

## 7. Independent adversarial audits and verification design

Obtain at least TWO distinct adversarial scientific investigations:

- **Audit 1 (validated spectral analysis):** rederive independently the GL two-ended root and positivity/monotonicity arguments, certify endpoint and mode derivative bounds (including interval rounding) and use deliberate wrong matching/normalization cases to falsify false root claims. This audit should not merely import the primary Riccati solver or reuse its saved mode output.
- **Audit 2 (geometry and radiation):** independently derive/recheck the complete conserved quadratic source, nonzero-k forced trace/divergence, physical n=2 master/EF reconstruction, all equations and error propagation, zero-mode radion current/flux and homogeneous data treatment. It should use distinct symbolic/analytic pathways, re-evaluate at least one physical positive-k metric residual, and reject mutations: missing Fourier half, Euclidean tr norm, wrong Wronskian sign, lost EF division, dropped homogeneous field or arbitrary static tension.

**Independence must be evidenced.** The GOAL-003 review noted a false W-vs-R independence claim and a shared inverse author/reviewer. Do not repeat that weakness. An independent output check is useful but not a second proof if it imports the full primary equation and mode.

Document source hashes, executable tests, commands, dependencies/precision modes, interval rounding, verified vs empirical enclosures, failed approaches and negative results. AI reviewers are not human referees. Each high-priority mathematical claim needs an independent analytical or constructive check before being called PROVEN.

### Numerical/analytic budget

- **Primary: one physical L/r0=10 eigenmode**, using a new validated algorithm only as necessary. Up to **three** planned refinements/matching-window configurations (plus rigorous enclosure subdomains needed by the validator; subdomains are not separate physical k sweeps). Do not run a new GL dispersion scan.
- The same one physical K=2k and s=2Omega forced-master response, with at most **three** resolution/domain configurations; reuse existing GOAL-004 profile and output only as non-certified comparison.
- One optional L/r0=8 **stretch** after L10 primary proof/partial enclosures are settled, not a default second campaign. No L12/L16/parameter grid.
- At most **two substantive repair methods per failed critical gate**. Document the mathematical obstacle at exhaustion and proceed to independent feasible downstream analytic work, but do not silently start an unlimited sweep or relax rigor.
- Permitted: symbolic identities, regular 1D ODE/Green-function solves, validated interval/Taylor/Volterra bounds, compact manufactured controls and bounded analytically justified quadratures. No production GRChombo, MPI/AMR, numerical full 2+1/5D spacetime evolution, large distributed jobs or hidden compute campaign.
- No new unreviewed dependence on massive external libraries without recording version/provenance. If validated interval package unavailable, derive a reproducible analytic interval scheme in-scope or deliver exact conditional bounds honestly; do not relabel standard double/MP precision as certified interval.

### Execution structure, checkpoint, stop

Use substantial methodological autonomy and intermediate milestones. Do not stop after a narrowly successful algebra identity when other authorized milestones are feasible. At most two authorized meaningful **research-only checkpoints** prior to the final publication: (i) validated physical GL input/root and source, if achieved; (ii) physical n=2 reconstructed inverse and M2, if achieved. Final report is another coherent commit. The sole operator must confirm no concurrent branch writer and obey the runbook's clean checkout, verified source diff, normal fast-forward and explicit lease/reconciliation policy.

Stop when: a production/other-branch edit or full spacetime run would be required; two mathematical repair attempts fail a central enclosure; there is an unresolved substantive defect in the first-order GL or forced master; model assumptions are silently passed as vacuum Einstein; a claimed physical radiation bound lacks exact data or a true news trace; or there is no remaining in-scope, finite-budget useful research.

Use final classification EXACTLY:
COMPLETE_VERIFIED / MEANINGFUL_PARTIAL / BLOCKED_BY_MATHEMATICAL_OBSTRUCTION / NOT_REPRODUCIBLE / OPEN_NEEDS_APPROVAL.

**Completion gate:** at least an independently certified/qualified physical L10 GL eigenpair, conserved quadratic source and source-evaluated full regular second-order Einstein response with declared homogeneous data and total error enclosure, plus audits and reproducibility. A certified nonlinear A_* is a separate stretch gate and is not presumed by completion. A genuinely certified eigenpair/source without M2 is scientifically meaningful partial progress. A non-certified attempt whose constants remain unknown must stop honestly with precise estimates and negative records. Physical first-bulge percent is *not* guaranteed or required.

## 8. Required deliverables

1. research/sav/goals/GLR-GOAL-005_REPORT.md: opens with decisive YES/NO/CONDITIONAL for certified physical eigenpair; conserved source norm; complete physical q2/M2; finite-A/noise/news; first-bulge physical E/M. Explicit numerical interval widths, error budgets, units, assumptions and verdict.
2. research/sav/physics/: reproducible analytic endpoint/root/source/forced-master/metric reconstruction and conditioned radion notes, if achieved; independent checks of source and mode selection.
3. research/sav/verification/: validated scripts, exact logs/results, adversarial audits, rejected mutations, rigorous enclosures or properly qualified empirical estimates, and negative findings.
4. Update research/sav/STATUS.md, HANDOFF.md, SOURCES.md, verification/INDEX.md, verification/negative_results.md and handoffs/MICHAEL_START_HERE.md. Preserve all prior reports, contracts, source data and numerical history verbatim. No production implementation requested.
5. Scientific handoff for Michael explaining exactly what invariant near-zone/outgoing scalars and radial Fourier components to observe in a **future separately approved** extraction, with error quantities that can be compared to the analytical enclosures.
6. Document exact starting and final branch head SHA, review/diff/commit hashes, no unauthorized files or branch changes, no production simulations, no PR/merge, no automatic GOAL-006.
