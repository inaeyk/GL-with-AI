# Stage Checklists

These checklists are gatekeeping tools. They do not replace the reproduction plan or project outline. Each stage should be reviewed before moving to the next. For code-generating agents, completion requires human review and approval, not just self-review.

Future physics or physics-design stages should include a polished physics-stage PDF note under `docs/physics_notes/` when they contain substantive physics reasoning. Build-only and infrastructure stages do not need full physics PDFs unless they include nontrivial physics design.

## Stage 0 - Infrastructure and reproducibility

Goal: establish local/remote development, repo hygiene, logs, smoke tests, and reproducible scripts.

- [x] Create GitHub repo and concise scaffold.
- [x] Set up WSL2 Ubuntu on Windows PC.
- [x] Authenticate GitHub access from WSL.
- [x] Install/use Docker Desktop with WSL2 backend.
- [x] Run GRChombo BinaryBH very-cheap Docker smoke test locally.
- [x] Confirm raw HDF5 output is stored under ignored `runs/`.
- [x] Create project logs and Codex logs.
- [x] Add smoke-test scripts.
- [ ] Confirm one-command smoke-test script works from a clean local state.
- [ ] Confirm plotting workflow is either implemented or explicitly deferred.

Gate: do not begin C++ implementation until the repo is clean, raw outputs are ignored, and smoke-test scripts are reviewed and approved by the user.

## Stage 1 - Public GRChombo capability/source map

Goal: understand what public GRChombo already provides before editing source code.

- [x] Clone public GRChombo into ignored `external/GRChombo`.
- [x] Create capability map.
- [x] Create source map.
- [x] Identify CCZ4, gauge, AMR, output, initial-data, and AH-finder infrastructure.
- [x] Identify `GR_SPACEDIM` / `CH_SPACEDIM` distinction.
- [x] Identify `hww/Aww` and `g_ww/K_ww` naming issues.
- [x] Inspect `ApparentHorizonFinderTest2D`.
- [x] Record that public CCZ4 appears dimension-parameterized but not fully cartoon-source-term aware.
- [x] Add explicit hazards/silent-failure section in `docs/grchombo/capability_map.md`.
- [x] Verify exact `AHFunctions.hpp` enum-order assumption around `hww` access: verified from public source that under `GR_SPACEDIM != CH_SPACEDIM`, `hww` is read positionally as `c_K - 1`, while `Aww` is read via `c_Aww`.
- [x] Decide whether to run a `CH_SPACEDIM=2`, `GR_SPACEDIM=4` compile preflight.

Gate: do not proceed to Stage 1.5 or create `BlackStringToy` until the source/capability maps and hazard notes are reviewed and approved by the user.

## Stage 1.5 - Target-dimension compile preflight

Goal: check whether the intended compile configuration `CH_SPACEDIM=2`, `GR_SPACEDIM=4` is technically viable before physics implementation.

- [x] Identify the smallest public test/example suitable for compile preflight.
- [x] Preferred first preflight target: adapt or reuse `Tests/ApparentHorizonFinderTest2D` as a controlled `DIM=2`, `GR_SPACEDIM=4` compile experiment, if build tooling allows.
- [x] Reproduce the baseline `DIM=2` Docker build/run path for `Tests/ApparentHorizonFinderTest2D`.
- [ ] If that target is unsuitable, choose the smallest public test/example that probes the `GR_SPACEDIM != CH_SPACEDIM` path.
- [x] Attempt `DIM=2` build with `GR_SPACEDIM=4` using controlled compiler flags or minimal local copy.
- [x] Record which files compile and which fail.
- [x] Check whether `AHSphericalGeometry` guards or other `GR_SPACEDIM==3` assumptions trigger.
- [x] Check whether `hww/Aww` enum variables are required for compilation.
- [x] Attempt an additional CCZ4-side `DIM=2`, `GR_SPACEDIM=4` scratch-copy compile probe.
- [x] Document that public `CCZ4Test` is blocked as a clean target by 3D test-harness assumptions and unconfirmed `GR_SPACEDIM=4` flag propagation.
- [x] Document blockers in `docs/grchombo/build_notes.md`.
- [x] Record that a successful compile is not evidence of physical correctness.

Gate: do not proceed to Stage 2 until this stage is reviewed and approved by the user. Do not treat any `GR_SPACEDIM=4` run as physically meaningful until the relevant equations, variables, and diagnostics are reviewed and approved by the user.

## Stage 2 - Editable build and BlackStringToy skeleton

Goal: create an editable project-specific example while making no physics claims.

- [x] Stage 2A: choose strategy: copy/adapt a minimal public example into tracked project code.
- [x] Stage 2A: use public `Examples/BinaryBH` as the scaffold origin because its very-cheap Docker smoke path was already validated in this repo.
- [x] Stage 2A: create `BlackStringToy` scaffold only after Stage 1.5 user review.
- [x] Stage 2A: add a scratch build script that copies GRChombo under ignored `runs/` and injects tracked `code/BlackStringToy/`.
- [x] Preserve known-working build/run behavior initially.
- [x] Rename files/classes minimally.
- [x] Build successfully.
- [x] Run a smoke test.
- [x] Confirm inherited parameter parsing and output writing still work for the scaffold smoke run.
- [x] Record exactly what was inherited from public examples.
- [x] Clearly label the skeleton as "not physical black-string evolution."

Gate: do not proceed to Stage 3 or add black-string initial data until the skeleton builds/runs and the diff is reviewed and approved by the user. Do not treat skeleton outputs as physically meaningful.

## Stage 3 - 5D black-string initial data plus modified-cartoon geometry derivation/validation

Goal: design the analytic initial data and perturbation, and validate the modified-cartoon geometry/source-term bookkeeping before implementation.

Status: complete. Remaining unchecked items in this section are carry-forward
validation or implementation prerequisites, not blockers to the already
reviewed Stage 4A-4C setup work.

- [x] Stage 3A: document target continuum geometry and dimensional bookkeeping.
- [x] Stage 3A: document the planned GRChombo variable map for initial data.
- [x] Stage 3A: document GL perturbation, turduckening, boundary assumptions, and unresolved decisions.
- [x] Stage 3A: create polished LaTeX physics-note source and build the PDF review artifact under `docs/physics_notes/`.
- [x] Stage 3B: document modified-cartoon hidden-direction source-term derivation and validation roadmap.
- [x] Stage 3B: document silent-failure modes for reduced 5D/SO(3) evolution.
- [x] Stage 3B: add LaTeX physics-note source and compiled PDF review artifact.
- [x] Stage 3B: incorporate review refinements for the `sin^2(theta)` contraction check, public-GRChombo source-term absence statement, staged CCZ4 RHS derivation plan, and hidden-direction multiplicity.
- [x] Stage 3C: create `docs/derivations/` scaffold for symbolic modified-cartoon geometry checks.
- [x] Stage 3C: add SymPy script for spherical Christoffels, GP extrinsic curvature, angular contractions, `K_ww`, and hidden-direction trace multiplicity.
- [x] Stage 3C: add prose derivation notes, LaTeX physics-note source, and compiled PDF review artifact.
- [x] Stage 3C: rerun the SymPy derivation script in an environment with SymPy available and record passing output.
- [x] Stage 3D: add SymPy scaffold for diagonal nontrivial `gamma_ww = q(x,z)` hidden-sector geometry.
- [x] Stage 3D: verify hidden-sector Christoffel identities, flat limit, angular contraction multiplicity, Ricci angular equality, and flat Ricci limit.
- [x] Stage 3D: add constant-q Ricci regression assertions for `R_xx`, `R_zz`, `R_xz`, angular Ricci components, and Ricci scalar.
- [x] Stage 3D: add prose derivation notes, LaTeX physics-note source, and compiled PDF review artifact.
- [x] Stage 3E: add nonconstant `q(x,z)` Ricci regression against warped-product reference formulas.
- [x] Stage 3E: assert `partial_x q`, `partial_z q`, and `partial_x partial_z q` are active for the chosen profile.
- [x] Stage 3E: add prose derivation notes, LaTeX physics-note source, and compiled PDF review artifact.
- [x] Stage 3F: translate diagonal physical `gamma_ww=q` bookkeeping into conformal `q=hww/chi` variables.
- [x] Stage 3F: verify `q` derivative identities, physical radius derivatives, and diagonal conformal tracelessness with SymPy.
- [x] Stage 3F: strengthen the SymPy checks with the determinant condition, physical/conformal round trip, `K_ij` reconstruction, hidden-multiplicity tracelessness, and `/4` dimension-denominator guard.
- [x] Stage 3F: add prose derivation notes, LaTeX physics-note source, and compiled PDF review artifact.
- [ ] Carry forward: document how the project enforces the cartoon/conformal determinant condition and trace-free constraints in the GRChombo-facing variable set, especially hidden `hww` participation in `det h = 1` and trace-free `A` enforcement.
- [x] Stage 3G: document the off-diagonal reduced metric `h_xz` / `gamma_xz` conformal-cartoon extension.
- [x] Stage 3G: add SymPy checks for the off-diagonal determinant, inverse metric, full-4D tracelessness, `K_xz` reconstruction, `/4` denominator guard, and diagonal Stage 3F limit.
- [x] Stage 3G: add sheared-flat off-diagonal Ricci regression for constant `g_xz = lambda` and `x`-dependent `g_xz = x` metrics.
- [x] Stage 3G: add final review follow-up for explicit normalized `det h_4D = 1` guard and Stage 3F diagonal-only warning.
- [x] Stage 3G: add prose derivation notes, LaTeX physics-note source, and compiled PDF review artifact.
- [x] Stage 3H: decompose the future CCZ4 RHS into checked source-term blocks before C++ implementation.
- [x] Stage 3H: separate exact-checkable RHS blocks from blocks requiring external/reference-code comparison or convergence evidence.
- [x] Stage 3H: use `hat_Gamma^A`, not `Z_i`, as the future evolved connection-sector blueprint while keeping `Z^i` as the encoded constraint vector.
- [x] Stage 3H: tag RHS blocks as inherited, modified-cartoon additions, mixed, or reference/convergence dependent.
- [x] Stage 3H: document required future GL-spectrum and constraint-violation damping validation milestones.
- [x] Stage 3H: document that small-`x` regularity is deferred to Stage 3I and unit-test fixture design is deferred to Stage 3J.
- [x] Stage 3I: document regularity and small-`x` treatment for conformal cartoon variables.
- [x] Stage 3I: list expected parity for scalar, metric, extrinsic-curvature, hatted-connection, and shift/gauge variables with uncertainty flags where needed.
- [x] Stage 3I: record diagonal matching `gamma_xx - gamma_ww = O(x^2)` and off-diagonal `gamma_xz = O(x)` smoothness expectations.
- [x] Stage 3I: add symbolic removable-singularity checks for `(partial_x q)/x`, `(gamma_xx - gamma_ww)/x^2`, conformal matching, and odd off-diagonal components.
- [x] Stage 3I: explicitly distinguish cartoon-axis regularity from the co-located physical black-string singularity and turduckening/interior regularization.
- [x] Stage 3I: add targeted assembled `tilde_Gamma^x` / `hat_Gamma^x` small-axis guard with hidden multiplicity.
- [x] Stage 3I: defer ghost-cell, stencil, axis-boundary, and finite-difference implementation choices.
- [x] Stage 3J: design unit-test fixtures before adding C++ source terms.
- [x] Stage 3J: map the validated Stage 3C-3I symbolic gates to future C++ unit-test, RHS-block, reference-comparison, and convergence/evolution fixture roles.
- [x] Stage 3J: identify required pre-C++ gates, including symbolic-script pass status, determinant/trace conventions, `hat_Gamma^A` convention, small-`x` risk documentation, and unresolved GRChombo-facing convention choices.
- [x] Stage 3J: separate exact-checkable fixtures from tolerance-based, reference-based, and convergence-based checks.
- [x] Stage 3J: incorporate review refinements for the round-`S^2` positive-curvature fixture, slicing-scoped Schwarzschild x `S^1` anchor, tolerance policy, and separated GL versus constraint-damping milestones.
- [x] Stage 3K: create the minimal C++ implementation plan after Stage 3J review.
- [x] Stage 3K: choose Stage 4A as the first safe implementation stage: repo-owned conformal-cartoon algebra helpers plus fixture tests, without connecting to time evolution.
- [x] Stage 3K: map likely repo-owned implementation targets and validation gates without editing code.
- [x] Stage 3K: explicitly defer full CCZ4 RHS, gauge/Gamma-driver, constraint damping, turduckening, AH finding, radiation extraction, production runs, and external/reference comparison.
- [x] Stage 3K: clarify Stage 4A is local algebra-helper plus non-grid fixtures only; Ricci, small-axis, grid wiring, RHS, gauge, damping, finite-difference, and evolution work require later explicit approval.
- [x] Stage 3K: require an automated enum/layout `static_assert` or unit-layout gate before any helper is wired to grid variables.
- [x] Stage 3K: record that green algebra-helper fixtures do not prove integration, grid-slot wiring, cartoon evolution, or CCZ4 correctness.
- [x] Stage 3K: document that GRChombo-facing compatibility plus internal consistency is the convention authority, while external/Pau implementations are later validation references only.
- [x] Review Stage 3K plan enough to explicitly approve Stage 4A local C++ source edits.
- [x] Stage 4A: add local conformal-cartoon algebra helper and non-grid fixture for determinant, inverse, full 4D trace/tracelessness, `/4` guards, `K_ij` reconstruction, diagonal limit, and normalized determinant checks.
- [x] Stage 4A: patch review items for dimension-aware `chi` power, tolerance-based floating-point guards, independent `K_ij` oracle values, and Stage 4A/4B terminology.
- [x] Review Stage 4A helper and fixture output enough to approve Stage 4B layout-check work.
- [x] Stage 4B: add automatic public CCZ4 baseline-layout fixture for current public component order and visible helper-input bindings without enum edits or grid reads.
- [x] Stage 4B review follow-up: clarify that real `hww/Aww` placement is not yet enforced because the repo-owned enum symbols do not exist.
- [x] Review Stage 4B public baseline-layout fixture enough to approve Stage 4C hidden enum work.
- [x] Stage 4C: add repo-owned `hww/Aww` enum names and real header-level placement assertions in `UserVariables.hpp`.
- [x] Stage 4C: add a non-grid placement fixture proving `c_hww`, `c_Aww`, their names, and helper-map component slots.
- [x] Stage 4C: rerun the scratch Docker/GRChombo scaffold build; compile succeeds with the 27-variable layout.
- [x] Stage 4C: record that the inherited smoke run reaches runtime but fails with non-finite `hww/Aww` until a later approved stage initializes or hands off the hidden variables.
- [x] Review Stage 4C hidden-variable placement before any helper reads live grid components.
- [x] Stage 4D: add finite smoke-only scaffold support for `hww/Aww` so the cheap smoke run no longer dies immediately from NaNs.
- [x] Stage 4D: guard the temporary `hww/Aww` freeze behind the default-off `scaffold_freeze_hidden` parameter, enabled only by the cheap smoke file.
- [x] Stage 4D: keep the freeze narrow; do not claim physical evolution correctness or implement cartoon Ricci/RHS terms.
- [ ] Future hidden-sector RHS work: disable or replace the Stage 4D smoke-only freeze and add a loud guard against using both paths together.
- [ ] Align Stage 3I parity and axis-filling conventions with GRChombo-facing project conventions before C++ source-term work.
- [ ] Derive and document the Stage 3I `tilde_Gamma^x` sign and full `hat_Gamma^A` contracted-connection convention in the GRChombo-facing cartoon extension.
- [ ] Required future RHS validation: reproduce the linear Gregory-Laflamme threshold/growth spectrum after matching radius convention, periodicity, perturbation sector, gauge, and extraction variable.
- [ ] Required future constraint-damping validation: inject linearized constraint violations and verify `Theta` / encoded-`Z^i` damping signs, `kappa_1`, `kappa_2`, hidden multiplicity, and `/4` bookkeeping.
- [ ] Add optional nonconstant profile with `f_zz != 0` so `R_zz` is a nonzero regression target.
- [ ] Factor duplicated Stage 3C/3D/3E symbolic geometry helpers into a shared module.
- [ ] Write or verify formulas for 5D black-string initial data in chosen coordinates.
- [ ] Decide exact variables to initialize: `chi`, `h_ij`, `hww`, `K`, `A_ij`, `Aww`, lapse, shift, `B`, `Gamma`, `Theta`.
- [ ] Verify `hww/Aww` enum placement before implementation.
- [ ] Implement parameters: `r0`, `L`, `epsilon`, `n`, `eps_cut`.
- [ ] Implement turduckening cutoff.
- [ ] Implement GL perturbation in `chi`.
- [ ] Run `t=0` or very short tests only.
- [ ] Check periodicity in `z`.
- [ ] Check smoothness outside cutoff.
- [ ] Check constraints and gauge quantities qualitatively.
- [x] Compile companion PDFs for all current physics-note LaTeX sources under `docs/physics_notes/`.

Gate status: Stage 3 design/tooling is complete and Stage 4A-4C setup work has
begun under explicit approval. Do not run long GL evolutions or treat outputs
as physically meaningful until `t=0` fields, parameter parsing, constraints,
modified-cartoon source terms, and diagnostics are reviewed and approved by the
user.

## Stage 4 - 4+1 CCZ4 / SO(3) modified-cartoon evolution

Goal: implement the actual reduced 5D dynamics on the 2D grid.

- [x] Stage 4A: local conformal-cartoon algebra helpers and non-grid tests.
- [x] Stage 4B: public GRChombo CCZ4 baseline-layout check.
- [x] Stage 4C: real `hww/Aww` variable entries, header-level placement checks, and non-grid placement fixture.
- [x] Stage 4C: full scratch Docker/GRChombo `BlackStringToy` scaffold compile succeeds with the 27-variable layout.
- [x] Stage 4C: cheap inherited smoke run result recorded accurately; it reaches runtime but fails because `hww/Aww` are not initialized or handed off yet.
- [x] Stage 4D: finite smoke-only scaffold support for `hww/Aww` so the cheap smoke run no longer dies immediately from NaNs.
- [x] Stage 4D: guard the temporary freeze with the default-off `scaffold_freeze_hidden` parameter, enabled only in the cheap smoke parameter file.
- [x] Stage 4D: do not claim physical evolution correctness; do not implement cartoon Ricci, full RHS, gauge/damping, finite differences, initial data, AH finding, or radiation extraction.
- [x] Stage 4E: add the first guarded grid-to-helper handoff diagnostic for `chi`, visible conformal metric/A components, `hww`, `Aww`, and `K`.
- [x] Stage 4E: keep the diagnostic check-only; helper outputs must not feed back into evolution or RHS terms.
- [x] Stage 4E: add a distinct-value standalone mapping fixture to catch swapped helper input slots that could remain finite in the cheap smoke data.
- [x] Stage 4F: add a local-value cartoon Ricci helper interface and compile-only/type fixture.
- [x] Stage 4F: document that the interface is the metric-derivative Ricci form from Stages 3C-3E, not the Gamma-based GRChombo Ricci form.
- [x] Stage 4F: do not implement Ricci formulas, source terms, RHS terms, or evolution wiring.
- [x] Stage 4G: implement the first local metric-derivative cartoon Ricci helper for away-axis inputs only.
- [x] Stage 4G: add flat, constant-`q`, nonconstant-`q`, sheared-flat, and `x = 0` rejection fixture checks.
- [x] Stage 4G: keep the Ricci helper local-value only; do not wire it into grid data, RHS terms, or evolution.
- [x] Stage 4H: document the compatibility issue between Stage 4G metric-derivative Ricci and GRChombo's Gamma-based `CCZ4Geometry::compute_ricci_Z` path.
- [x] Stage 4H: recommend keeping Stage 4G as a checked local Ricci source/oracle until a local Ricci-to-RHS contract exists.
- [x] Stage 4I: add a typed Ricci bridge contract before any Ricci output is wired into evolution.
- [x] Stage 4I: test the full 4D bridge contraction with the hard-coded `451` oracle and hidden/off-diagonal sensitivity checks.
- [x] Stage 4I review follow-up: make raw cartoon Ricci component storage private and require bridge exposure for RHS-facing access.
- [x] Stage 4I: keep the bridge local; do not call RHS, read grid data, or wire Ricci into evolution.
- [x] Stage 4J: define the local Ricci-to-RHS contract using bridge-approved Ricci data only.
- [x] Stage 4J: test the contract with the hard-coded `451` trace oracle, `Rww` omission sensitivity, off-diagonal sensitivity, and away-axis guard.
- [x] Stage 4J: keep the contract local; do not implement RHS formulas, read grid data, or wire evolution.
- [x] Stage 4K: add a local RHS source-block skeleton that consumes the Stage 4J contract type.
- [x] Stage 4K: test the skeleton's named inert output fields without implementing real RHS formulas.
- [x] Stage 4K: keep the skeleton local; do not call `BlackStringToyLevel`, read grid data, or wire evolution.
- [x] Stage 4L: implement the first local RHS formula block, limited to trace-free Ricci projection.
- [x] Stage 4L: test the full 4D trace, hidden `ww` multiplicity, shared `/4` denominator convention, zero-trace projection behavior, and `x = 0` rejection.
- [x] Stage 4L: keep the projection local; do not add live RHS wiring, grid reads, gauge/damping, or evolution changes.
- [x] Stage 4M: add a named away-axis policy for local cartoon/RHS expressions.
- [x] Stage 4M: centralize finite `x > 0` validation and guarded `1/x` / `1/x^2` helpers before adding future singular-looking source terms.
- [x] Stage 4M: keep Stage 3I small-axis regularization deferred; do not clamp `x`, substitute epsilon, or claim axis-valid evaluation.
- [x] Stage 4N: add guarded away-axis singular-combination helpers for future cartoon source terms.
- [x] Stage 4N: test `d_x f / x`, `(f - g) / x^2`, nonfinite-value rejection, and zero/negative/nonfinite `x` rejection.
- [x] Stage 4N: do not implement small-axis regularization, new physical RHS terms, grid reads, or evolution wiring.
- [x] Stage 4O: lock current axis-regime semantics as `AwayAxisOnly`.
- [x] Stage 4O: keep `1/x^2` as a separately guarded away-axis primitive so future clamp or regularized behavior must define it independently from `1/x`.
- [x] Stage 4O: do not implement small-axis regularization, clamping, epsilon replacement, new source terms, grid reads, or evolution wiring.
- [x] Stage 4P: add named away-axis cartoon geometry primitives for `(d_x hww) / x` and `(hxx - hww) / x^2`.
- [x] Stage 4P: route the geometry primitives through Stage 4N guarded singular-combination helpers and Stage 4O away-axis semantics.
- [x] Stage 4P review patch: document that finite axis behavior of `(hxx - hww) / x^2` requires `hxx - hww = O(x^2)`, which Stage 4P does not enforce.
- [x] Stage 4P: do not implement full Ricci, CCZ4 RHS terms, small-axis regularization, grid reads, or evolution wiring.
- [x] Stage 4Q: add a local pointwise matching guard for `hxx - hww = O(x^2)`.
- [x] Stage 4Q: test that a matching sample passes, a clear `O(1)` mismatch fails, and invalid/nonfinite inputs reject.
- [x] Stage 4Q: document that the guard does not prove analytic regularity, construct finite axis limits, add RHS physics, read grid data, or wire evolution.
- [x] Stage 4R: add the first regularity-guarded local cartoon source sub-block.
- [x] Stage 4R: combine Stage 4P primitives with the Stage 4Q matching guard and Stage 4O away-axis semantics.
- [x] Stage 4R review patch: make Stage 4R the normal source-facing path for `(hxx - hww) / x^2`; raw Stage 4P no longer exposes that risky value as a public output.
- [x] Stage 4R: do not implement full Ricci, full CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, or evolution wiring.
- [ ] Future hidden-sector RHS work: disable or replace the Stage 4D smoke-only freeze and add a loud guard against using both paths together.
- [ ] Derive/list required modified-cartoon source terms beyond the Stage 3B roadmap.
- [ ] Identify where `CCZ4RHS` needs modification.
- [ ] Implement `hww/Aww` evolution equations.
- [ ] Ensure `K` trace and `A` tracelessness use `GR_SPACEDIM=4`.
- [ ] Add regularity handling near the cartoon axis if needed.
- [ ] Test stationary or near-stationary uniform black string.
- [ ] Monitor Hamiltonian/momentum constraints.
- [ ] Check convergence on a small non-production grid.
- [ ] Explicitly document silent-failure checks.

Gate: do not proceed to Stage 5 or start Pau diagnostic reproduction until cartoon CCZ4 implementation is verified on controlled tests and reviewed and approved by the user. Do not treat outputs as physically meaningful until the relevant equations, variables, and diagnostics are reviewed and approved by the user.

## Stage 5 - Pau diagnostic reproduction

Goal: reproduce the diagnostic backbone of the 5D GL black-string paper.

- [ ] Resolve PETSc/AH-finder environment.
- [ ] Configure baseline `L/r0=10` run.
- [ ] Implement horizon output `x=h(t,z)`.
- [ ] Output `gamma_xx`, `gamma_xz`, `gamma_zz`, `gamma_ww` or equivalent reconstructed quantities.
- [ ] Compute `A/A0`.
- [ ] Compute `R_min/r0`.
- [ ] Compute pinch-off fits.
- [ ] Compute Fourier coefficients on string segments.
- [ ] Track generation formation times/properties.
- [ ] Compare `L/r0=10` against target paper diagnostics.

Gate: do not proceed to Stage 6 or run the full `L` sweep until the `L=10` medium run produces credible horizon diagnostics and is reviewed and approved by the user.

## Stage 6 - Convergence and validation

Goal: establish that the reproduced diagnostics are not numerical artifacts.

- [ ] Define low/medium/high resolution set.
- [ ] Run `L10_low`, `L10_med`, `L10_high`.
- [ ] Compare `A/A0` convergence.
- [ ] Compare `R_min` convergence.
- [ ] Compare pinch-off parameters.
- [ ] Compare Fourier coefficients.
- [ ] Compare generation times/properties.
- [ ] Track horizon finder residuals and estimator disagreements.
- [ ] Document systematic uncertainties.

Gate: do not proceed to Stage 7 or claim reproduction complete until convergence/validation criteria are documented and reviewed and approved by the user.

## Stage 7 - Radiation/wave-extraction design

Goal: design radiation extraction after the horizon dynamics are robust.

- [ ] Decide radiation observable appropriate for 5D/SO(3)-reduced spacetime.
- [ ] Review whether public Weyl4 machinery is usable or purely 3+1D.
- [ ] Decide extraction surfaces/large-radius diagnostics.
- [ ] Identify gauge issues and asymptotic normalization.
- [ ] Define waveform, spectrum, energy by mode, and angular-distribution observables.
- [ ] Decide how Pau's collaboration code enters this phase.

Gate: do not proceed to Stage 8 or implement radiation extraction until the formalism and validation tests are specified and reviewed and approved by the user.

## Stage 8 - Radiation computation

Goal: compute waveform/radiation diagnostics from validated simulations.

- [ ] Implement extraction code.
- [ ] Test on controlled solutions or perturbative benchmarks if available.
- [ ] Run extraction on reproduced `L=10` case.
- [ ] Decompose by modes/harmonics.
- [ ] Compute spectrum.
- [ ] Estimate radiated energy.
- [ ] Compare radiation windows to GL generation times and Fourier-mode evolution.

Gate: do not proceed to Stage 9 or interpret radiation results physically until extraction is benchmarked, convergence-tested, and reviewed and approved by the user.

## Stage 9 - Physics interpretation and writeup

Goal: turn validated simulations and radiation diagnostics into scientific conclusions.

- [ ] Summarize reproduction agreement/mismatches.
- [ ] Summarize radiation observables.
- [ ] Identify robust signatures of higher-dimensional GL transition.
- [ ] Quantify numerical/systematic uncertainty.
- [ ] Compare to PBH/higher-dimensional phenomenology motivation.
- [ ] Draft internal note/paper/thesis section.
