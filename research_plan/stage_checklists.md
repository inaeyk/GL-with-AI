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

## Stage 3 - 5D black-string initial data and GL perturbation

Goal: implement analytic initial data and perturbation in a controlled way.

- [x] Stage 3A: document target continuum geometry and dimensional bookkeeping.
- [x] Stage 3A: document the planned GRChombo variable map for initial data.
- [x] Stage 3A: document GL perturbation, turduckening, boundary assumptions, and unresolved decisions.
- [x] Stage 3A: create polished LaTeX physics-note source and build the PDF review artifact under `docs/physics_notes/`.
- [x] Stage 3B: document modified-cartoon hidden-direction source-term derivation and validation roadmap.
- [x] Stage 3B: document silent-failure modes for reduced 5D/SO(3) evolution.
- [x] Stage 3B: add initial LaTeX physics-note source; PDF build deferred until after Markdown review.
- [x] Stage 3B: incorporate review refinements for the `sin^2(theta)` contraction check, public-GRChombo source-term absence statement, staged CCZ4 RHS derivation plan, and hidden-direction multiplicity.
- [x] Stage 3C: create `docs/derivations/` scaffold for symbolic modified-cartoon geometry checks.
- [x] Stage 3C: add SymPy script for spherical Christoffels, GP extrinsic curvature, angular contractions, `K_ww`, and hidden-direction trace multiplicity.
- [x] Stage 3C: add prose derivation notes and optional LaTeX physics-note source; PDF build deferred.
- [x] Stage 3C: rerun the SymPy derivation script in an environment with SymPy available and record passing output.
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
- [ ] Generate and review the Stage 3B polished physics PDF after the Markdown derivation roadmap is reviewed.

Gate: do not proceed to Stage 4 or run long GL evolutions until `t=0` fields, parameter parsing, and constraints are reviewed and approved by the user. Do not treat outputs as physically meaningful until the relevant equations, variables, and diagnostics are reviewed and approved by the user.

## Stage 4 - 4+1 CCZ4 / SO(3) modified-cartoon evolution

Goal: implement the actual reduced 5D dynamics on the 2D grid.

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
