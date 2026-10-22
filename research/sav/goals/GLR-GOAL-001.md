# GLR-GOAL-001 — Second-order radion radiation from an unstable 5D black string

**Task ID:** `T-20261022-GLR-GOAL-001`
**State:** EXECUTED 2026-10-22 — MEANINGFUL_PARTIAL; see execution ledger below
**Environment:** Codex GPT-6.1 Sol, High reasoning effort, standard speed
**Repository:** `inaeyk/GL-with-AI`
**Branch:** `research/sav-radiation`
**Writable scope:** `research/sav/**`, and nothing else
**Scientific owner:** Savdeep Sethi; collaborator Michael Yan; advisor Pau Figueras.
**Explicit prohibition:** no production 5D GRChombo evolutions, MPI/AMR
campaigns, parameter retuning, production code changes or branch merges.

## Big scientific question

In 5D vacuum Einstein gravity on R^{1,3} x S^1, with a uniform
Schwarzschild black string and an exact SO(3) singlet fundamental growing
GL perturbation, does the **complete second-order, KK-zero-mode,
gauge-invariant physical radion source** generate outgoing scalar news
at infinity? If so, calculate the normalized retarded transfer
coefficient as a function of k*r0, predict its weakly nonlinear
time-dependent flux and spectrum, and determine its constraints as an
early-bulge energy estimate. If it does not, explain the cancellation
rigorously and determine the first allowed radiative order.

The main target is a scientific result, not a plan or a code fixture.
An explicit, fully normalized source and retarded Green-function
formula with independently verified nonzero/zero projection is a
meaningful milestone; a quantitatively evaluated transfer coefficient
with uncertainty is preferred.

## Background and audited inputs

Start by reading:
- `research/sav/README.md`, `AGENTS.md`, `NOTATION.md`,
  `STATUS.md`, `SOURCES.md`, and `QUESTIONS.md`.
- `research/sav/literature/PAST_WORK.md` (source-linked; assess its gaps).
- `research/sav/physics/{so3_reduction,kk_channels,early_amplitude_flux,
   timescales_energy}.md`. Do not accept derivations without checking.
- `research/sav/verification/INDEX.md` and `negative_results.md`.
- The authoritative read-only engineering record in `main`,
  `supervised/m2b-cycle1`, frozen September M2-C. Never modify or
  import unaccepted production changes.
- Primary literature: Gregory–Laflamme 1993/1994; Kudoh 2006;
  Collingbourne 2021; Gubser 2002; Lehner–Pretorius 2010;
  Figueras–França–Gu–Andrade 2023, esp. III.1/Table 2;
  Lü–Mao–Wu 2019 for KK scalar news; Harmark–Obers for tension.

The Figueras early-burst statement is a **conjecture**, not waveform data.
The existing repository has no accepted physical GL mode or M2-C
scientific pass. Do not quote invented measured emission fractions.

## Primary target and conventions

The base solution is `f(r)=1-r0/r` with `z~z+L`; set
`k=2pi/L` for fundamental n=1. Retain explicit `G5,G4=G5/L,r0,L`.
The SO(3) exact action candidate is
```text
S=(4 G5)^{-1} int dtdrdz sqrt(-g3)
 [R^2 R[g3]+2(nabla R)^2+2].
```
Use a genuine constrained GL eigenmode with `A(t)=epsilon*p1
exp(Omega_GL t)` and a clearly *invariant* normalization of A.
Define the n=0 outgoing radion by
`phi0=Phi(u)/rho+O(rho^-2)` in 4D Einstein-frame Bondi gauge,
`dE0/du=(partial_u Phi)^2/G4` pending independent audit.

Define the potential transfer coefficient by
`Phi(u)=r0 C0(k*r0,Omega*r0) A(u)^2+...` *only* when
the retarded response has settled for a specified exponential source,
and justify any such local-in-time approximation. For general source
histories use a retarded convolution, not a guessed constant C0.

## Milestones (methodological autonomy within this target)

### A. Independent SO(3) continuum foundation and benchmarks
1. Re-derive warped-product 5D -> 3D action including sphere curvature,
   Euler–Lagrange equations, and boundary terms, from independent methods.
2. Count local physical degrees via Hamiltonian constraints; show
   distinction between 3D gravitational variables, zero-mode radion,
   and massive KK singlet. Handle n=0 exceptional mass/tension solutions.
3. Recover a gauge-invariant 1D radial master equation for the
   five-dimensional SO(3)-singlet GL mode; fix horizon regularity,
   exponential spatial decay, real growing Omega and normalization.
4. Independently reproduce the continuum threshold k_cr*r0~0.876 and
   at least one growth-rate benchmark; extend to representative
   `L/r0=8,10,12,16` if stable and affordable. Compare two gauges or
   a gauge-invariant Weyl/master invariant and record all conversions.

### B. Complete quadratic Einstein source in n=0
1. Expand the 5D/reduced Einstein equations through O(A^2) for
   n=+/-1. Produce an explicit second-order source (as radial functions
   of the first-order eigenfunction and derivatives), not only a
   schematic identity.
2. Enforce *all* equations and constraints and regular second-order
   gauge transformations; separate mass/tension shifts, pure gauge,
   stationary fields and the **physical radion**.
3. Derive a gauge-invariant second-order zero-mode master source,
   use compact Fourier convention with factor 1/2 for cos^2(kz),
   and check Bianchi/no-source identities.
4. Show whether a zero-outgoing-overlap theorem or cancellation
   holds. Test instead of assuming nonzero radiation.

### C. Retarded source-to-infinity transfer
1. Starting from the scalar KK action, derive the n=0 l=0
   Schwarzschild scalar operator (candidate `V=f*r0/r^3`).
2. Construct the horizon-ingoing and asymptotically outgoing
   Green-function basis, Wronskian and correct complex frequency
   continuation for `omega=2i Omega` with causal initial data.
   Explain the subtlety of a pure infinitely extended exponential
   and justify finite-time source switching.
3. Obtain a normalized overlap expression C0(k*r0,Omega*r0) and
   evaluate it if feasible with controlled bounded 1D ODE/quadrature
   calculations and interval/refinement/error estimates.
4. Independently derive the overlap in a second gauge or via the
   covariant reduced action and check equality after boundary terms.

### D. Physical flux, spectrum and early amplitude law
1. Derive radion Bondi news and energy flux independently from both
   4D Einstein-frame EMD and 5D asymptotic charges/symplectic current;
   validate the coefficients including sphere area and L.
2. Quantify O(A^2) amplitude, O(A^4) flux if nonzero and the
   `epsilon -> epsilon/10` onset time translation. Express
   `E0/M0` and `dE0/domega` with conventions and controlled
   amplitude windows. Distinguish initial transients.
3. Compare with FFG&A Table 2 first-bulge windows; these
   are geometric, not radiation measurements.
4. State exactly what can and cannot be inferred about emission
   for A~1, and whether extrapolation requires a new weakly nonlinear
   amplitude equation or model for temporal rollover.

### E. Independent verification and handoff
1. Produce derivation documents, exact symbolic checks and source-linked
   intermediate expressions with all field normalizations.
2. Commission/perform two genuinely independent adversarial reviews:
   (i) action/G5, boundary/gauge and mode projection, (ii) complete
   second-order source, possible cancellations and retarded flux. If
   independent auditors are unavailable, label that limitation.
3. Verify principal identities with a different calculation pathway
   and intentional sign/normalization mutations that are rejected.
4. Update `research/sav/STATUS.md`, `SOURCES.md`,
   `verification/INDEX.md`, `verification/negative_results.md`
   and `handoffs/MICHAEL_START_HERE.md` with a concrete extraction
   interface and no instruction to modify production code.
5. Write `research/sav/goals/GLR-GOAL-001_REPORT.md` with
   equations, quantitative claims, uncertainties, exact commit refs,
   audit verdicts and an honest terminal classification.

## Stretch objectives (only after primary gates)

- n=+/-2 massive KK source and thresholded energy current; this
  requires its **own canonical normalization** and finite-radius flux.
- First self-consistent weakly nonlinear amplitude equation feeding
  a nonadiabatic outgoing radion source; estimate the relation between
  scalar waveform peak and first-bulge formation.
- Compare small-kr0 blackfold effective theory and/or a controlled
  matched near/far expansion; state its D=5 errors.
- Bound later-cascade integrated energy or high-frequency tail under
  explicit scaling assumptions; no unconditional dominance claim.

## Permitted analysis / finite budgets

Symbolic tools and self-contained scripts are allowed **only inside
research/sav/**. A few reproducible 1D linear perturbation BVPs,
Green-function quadratures, convergence and manufactured tests are
allowed because they are **not production 5D simulations**.

Default target parameter set: k*r0=2pi/(8,10,12,16). Evaluate one
reference case thoroughly before broader cases. Keep quantitative
checks to four physical k values with up to three systematically
refined 1D discretizations per value; larger spectral scans,
heavy numerical evolution or substantive expansion require human
approval. Use no more than two substantive repair approaches for any
failed central mathematics/gauge/normalization gate before recording
the obstruction and moving to a review point. No unbounded branch
searches, retries, or expensive dense-spectral framework.

The scientific objective takes precedence over superficial milestone
completion. Codex may choose a different equivalent gauge,
symbolic package or Green-function method; it must document why.
Do not modify simulation assumptions or silently loosen physical
acceptance conditions to obtain a nonzero answer.

## Explicit stop and failure logic

- If the quadratic n=0 physical source cancels identically, write a
  verified **zero-amplitude result** with proof and identify higher
  order channels. That is a successful scientific negative result.
- If nonzero source exists but retarded overlap vanishes, prove and
  report that separate cancellation.
- If gauge invariance fails, do not compute a putative physical
  E/M using a gauge-dependent component; produce a mathematical
  obstruction and precise next step.
- If only formal source/integral is derivable, deliver a meaningful
  partial result with missing analytic/numeric gate clearly stated.
- If literature anticipates a result, credit the prior work and
  develop independent confirmation and new information rather than
  claiming novelty.
- If numerical BVP accuracy fails after bounded refinements, stop
  and report residuals; never invent C0 or an early radiated percentage.
- Stop on missing authority, unauthorized write path, production
  run temptation, or unresolved convention conflict.

Final classification must be one of:
`COMPLETE_VERIFIED`, `MEANINGFUL_PARTIAL`,
`BLOCKED_BY_MATHEMATICAL_OBSTRUCTION`,
`NOT_REPRODUCIBLE`, `OPEN_NEEDS_APPROVAL`.

## Minimum final artifacts

- `research/sav/physics/` rigorous derivations and, if obtained,
  gauge-invariant source and exact retarded overlap formula.
- `research/sav/verification/` executable symbolic/nonproduction
  checks, independent audits, failures and method limitations.
- `research/sav/goals/GLR-GOAL-001_REPORT.md`: concise scientific
  report with equations, status table, numbers/uncertainty (if any),
  and reproducibility.
- Updated Michael scientific handoff for feasible future extraction.
- Branch-scoped commits, clear stop verdict and next ambition.

## Append-only execution ledger

### 2026-10-22 — owner-authorized execution of T-20261022-GLR-GOAL-001

- Input research commit: `0b0b2e1ee1df64a80e4474d6909173d4682564f3`.
  Read complete controlling task/AGENTS and linked notation, evidence,
  literature, checks and Michael handoff before calculation. Historical
  failed numerical and AI notes were inputs for review, not proof.
- A: independent SO(3) action/boundary/constraint audit; exact gauge
  completion and all-component GL Ricci verification; threshold0.8761604020
  and four growing modes. Exactly four physical k ×three refinements used,
  reference L10 completed first. Precise published growth point not
  digitized; literature lower-bound and threshold comparisons passed.
- B: closed complete gauge-invariant quadratic scalar source derived:
  `Box phi0=−sqrt(3) k² A² H_mu nu H^mu nu/16`, with Fourier1/2,
  second-order completion, on-shell integrability and exceptional zero
  modes distinguished. Exact horizon cancellations preserved.
- C: explicit normalized −1/W retarded overlap; independent cubic-action
  equality. Final four positive C0 estimates in the report. Two bounded
  scalar-boundary approaches, final positive Volterra tail bracket;
  independent saved-profile L10 check agrees within7e-8. Full interval
  proof of physical overlap remains open.
- D: scalar flux checked from 4D action and independent5D TT current;
  conditional quartic energy law/windowed spectrum and onset shifts
  calculated. No controlled numerical finite-A interval, full spectrum,
  first-bulge radiated fraction or A~1 extrapolation established.
- E: two separate adversarial mathematical reviews completed, plus exact
  sign/Fourier/gauge/norm mutations and a manufactured cancellation.
  Definite prior KK-vector exponent/counting errors corrected; failures
  retained. Science-only Michael interface updated in closing deliverable.
- Stop: `MEANINGFUL_PARTIAL`; no production evolution, cluster launch,
  Michael setup change, merge, manuscript or Overleaf work. Finite analysis
  allocation reached; no automatic continuation or repair is authorized
  by this ledger. Closing report gives exact commit provenance and precise
  next mathematical gate.

