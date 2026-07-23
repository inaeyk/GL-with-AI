# Project Log

Chronological lab notebook.

Correction notice (2026-07-20): all earlier Stage 4AO-C K-row entries using
`A_IJ A^IJ + K^2/d` are retained only as historical records of the rejected
`USE_BSSN` branch. The current selected-CCZ4 implementation and evidence are
recorded in the 2026-07-20 correction entry.

Categories: 🔴 Physics, 🔵 Code, 🟡 Physics + Code

## 2026-05-12

Category: 🟡 Physics + Code

- Apple Silicon Mac entered the GRChombo Docker container, but the prebuilt `amd64` executable failed with `Illegal instruction`.
- GitHub Codespaces `x86_64` Docker run succeeded for the GRChombo `BinaryBH` very-cheap test.
- HDF5 output and `quick_plot` PNG plots were generated.
- `quick_plot` needed a `yt` API patch: `set_window_size` -> `set_figure_size`.
- Stable local development target: Windows PC + WSL2 + Docker.
- Repository scope: code, docs, scripts, logs, research plans, small plots, and metadata.

## 2026-05-13

Category: 🔵 Code

- GRChombo Docker image ran locally under Windows WSL2 Ubuntu.
- `BinaryBH` very-cheap example completed and produced HDF5 output in `runs/BinaryBH_verycheap/hdf5/`.

Category: 🟡 Physics + Code

- Windows WSL2 is validated as the stable local development and smoke-test platform for GRChombo BinaryBH work.

Category: 🔵 Code

- Archived the old PDF research/setup notes and created living Markdown planning files.

Category: 🟡 Physics + Code

- Clarified that `reproduction_plan.md` is the current first scientific milestone, while `project_outline.md` covers the full GW/radiation goal.

Category: 🔵 Code

- Cleaned up planning docs after external review, keeping the changes targeted to status and prerequisite wording.

Category: 🟡 Physics + Code

- `reproduction_plan.md` now explicitly depends on `project_outline.md` Stages 1-4 before Phase 1 production simulations.

Category: 🔵 Code

- Cloned and inspected public GRChombo source into ignored `external/GRChombo` and created first-pass source/capability maps.

Category: 🟡 Physics + Code

- Identified reusable public GRChombo pieces for the Pau reproduction and separated them from black-string-specific pieces likely needing development.

Category: 🟡 Physics + Code

- External review sharpened the GRChombo map around 3+1D public-main limitations, `GR_SPACEDIM`/`CH_SPACEDIM`, `gww`/`Kww`, `ApparentHorizonFinderTest2D`, and the deliberate choice to withhold Pau's code during the AI-agent benchmark stage.

Category: 🔵 Code

- Completed `GR_SPACEDIM` read-only search; found broad dimension-aware hooks in CCZ4/AH infrastructure but no public ready-to-run Pau-style driver yet.

Category: 🔵 Code

- Inspected public GRChombo `Tests/ApparentHorizonFinderTest2D`; local run attempt stopped before build because `make` and the required Chombo/PETSc build environment were unavailable.

Category: 🟡 Physics + Code

- `ApparentHorizonFinderTest2D` matters as the closest public string-like AH validation harness for the later SO(3)-reduced horizon pipeline, but it is not a full black-string evolution driver.

Category: 🟡 Physics + Code

- Recorded the CCZ4/cartoon variable naming convention and the `hww`/`Aww` enum-order hazard before starting `BlackStringToy`.

Category: 🟡 Physics + Code

- Created stage checklists to gate the transition from public-source mapping to `BlackStringToy`, cartoon CCZ4, Pau reproduction, and radiation extraction.

Category: 🟡 Physics + Code

- Tightened stage gate language to prevent premature transition from public-source mapping to `BlackStringToy` or physics simulations.

Category: 🔵 Code

- Verified from public GRChombo source that `AHFunctions.hpp` reads `hww` positionally as `c_K - 1` under `GR_SPACEDIM != CH_SPACEDIM`, while `Aww` is read via `c_Aww`.

Category: 🟡 Physics + Code

- Documented the modified-cartoon silent-failure hazard: a `CH_SPACEDIM=2`, `GR_SPACEDIM=4` run could compile and write HDF5 while still missing the absorbed-`S^2` source terms needed for physical 5D/SO(3) evolution.

## 2026-06-08

Category: 🔵 Code

- Began Stage 1.5 target-dimension compile preflight using public `Tests/ApparentHorizonFinderTest2D` as the preferred target. Initial environment checks reported Docker and `make` unavailable in that shell; this was later superseded by the verified Docker baseline below. A source-free `GR_SPACEDIM=4` compile was not attempted because the test source hard-defines `GR_SPACEDIM 2`, and external GRChombo source edits are disallowed for this preflight.

Category: 🔵 Code

- Verified the `DIM=2` `ApparentHorizonFinderTest2D` baseline build/run inside Docker; the run exits 0 but skips the AH solver because `USE_AHFINDER` is undefined.

Category: 🟡 Physics + Code

- Stage 1 source/capability maps and hazard notes were reviewed and approved by the user, clearing the gate for Stage 1.5 target-dimension compile preflight.

Category: 🟡 Physics + Code

- Re-emphasized that any future successful `CH_SPACEDIM=2`, `GR_SPACEDIM=4` compile is only a technical preflight, not evidence of physical 5D black-string correctness; modified-cartoon CCZ4 source terms and diagnostics remain required.

Category: 🔵 Code

- Attempted the Stage 1.5 scratch-copy `GR_SPACEDIM=4` compile preflight under `runs/stage1_5_preflight/GRChombo_GR4`; the copied `ApparentHorizonFinderTest2D` target built successfully with `DIM=2`, `GR_SPACEDIM=4`, and `USE_PETSC=FALSE`.

Category: 🟡 Physics + Code

- The scratch-copy compile result informs whether the target `CH_SPACEDIM=2`, `GR_SPACEDIM=4` compile path is viable, but it does not validate physical dynamics, modified-cartoon CCZ4 terms, or PETSc/AHFinder diagnostics.

Category: 🔵 Code

- Attempted Stage 1.5B CCZ4-side scratch-copy compile preflight with public `CCZ4Test`; the `DIM=2` build failed in `CCZ4Test.cpp` because the test harness is hard-coded around 3D Chombo `IntVect`/box assumptions, and the emitted compile command did not show the requested `-DGR_SPACEDIM=4` flag.

Category: 🟡 Physics + Code

- The CCZ4-side failure is a technical preflight blocker, not a physics result. It reinforces that a compile-only pass would still not validate the modified-cartoon CCZ4 source terms needed for physical `CH_SPACEDIM=2`, `GR_SPACEDIM=4` evolution.

## 2026-06-13

Category: 🔵 Code

- Began Stage 2A editable `BlackStringToy` scaffold work after user review of Stage 1 and Stage 1.5. Chose public `Examples/BinaryBH` as the origin because its very-cheap Docker smoke path was already validated in this repo.
- The Stage 2A scratch build script was manually rerun from a Docker-enabled shell. It built `Main_BlackStringToy3d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex`, completed the inherited one-step smoke run, and wrote HDF5/data outputs under ignored `runs/stage2_blackstringtoy/`.
- Hardened the Stage 2B scratch workflow so repeated runs repair root-owned Docker outputs before cleanup and after build/run completion, scoped to `runs/stage2_blackstringtoy/`.

Category: 🟡 Physics + Code

- The Stage 2A scaffold is explicitly non-physical. It inherits public 3+1D BinaryBH behavior only to validate the editable build workflow and does not implement 5D black-string initial data, modified-cartoon CCZ4 source terms, `hww/Aww`, or radiation extraction.
- Began Stage 3A design documentation for future black-string initial data. The note records the target 5D uniform string geometry, `CH_SPACEDIM=2` / `GR_SPACEDIM=4` bookkeeping, planned GL perturbation, turduckening assumptions, and unresolved choices; no implementation source was changed.
- Clarified the Stage 3A initial-data note: `h(t,z)`, `hww` cartoon meaning, GP extrinsic-curvature formulas, `K` sign convention, and the distinction between GP-slice geometric data and zero-shift gauge startup.
- Created a physics-stage PDF note workflow and Stage 3A LaTeX source under `docs/physics_notes/`; the Stage 3A PDF review artifact was built successfully at `docs/physics_notes/stage3A_black_string_initial_data.pdf`.
- Began Stage 3B design documentation for modified-cartoon source terms. The note records the spherical-to-cartoon mapping, GP extrinsic-curvature consistency check, source-term categories, validation strategy, and silent-failure modes for `CH_SPACEDIM=2`, `GR_SPACEDIM=4`; no implementation source was changed.
- Refined the Stage 3B modified-cartoon notes after review: added the `sin^2(theta)` contraction check, sharpened the public-GRChombo 5D/SO(3) source-term absence statement, split the CCZ4 RHS derivation into staged sub-steps, and clarified hidden-direction multiplicity as `GR_SPACEDIM - CH_SPACEDIM`.
- Began Stage 3C symbolic/geometric derivation tooling under `docs/derivations/`. The SymPy scaffold is designed to verify spherical Christoffels, GP extrinsic curvature, the angular contraction to `K_ww`, and `N_hidden = GR_SPACEDIM - CH_SPACEDIM`; the first run in this shell was blocked because SymPy is unavailable. No implementation source was changed.
- Recorded the successful Stage 3C SymPy run. The symbolic geometry check passes for the basic Christoffels, GP extrinsic-curvature components, angular contractions, `K_ww`, and trace multiplicity; no implementation source was changed.
- Began Stage 3D symbolic derivation tooling for nontrivial `gamma_ww = q(x,z)`. The diagonal-metric SymPy scaffold verifies hidden-sector Christoffels, flat-limit recovery, angular contraction multiplicity, `R_phi phi / sin(theta)^2 = R_theta theta`, angular Ricci scalar multiplicity, and flat Ricci vanishing; no implementation source was changed.
- Refined Stage 3D with a constant-q Ricci regression: the script now asserts `R_xx`, `R_zz`, `R_xz`, angular Ricci components, and the full Ricci scalar for `dl^2 = dx^2 + dz^2 + q0 x^2 dOmega_2^2`; no implementation source was changed.
- Documented the endorsed Stage 3D limitation: the constant-q Ricci regression does not test `q(x,z)` derivative Ricci terms, so a future non-constant-q regression is needed before relying on hidden-sector derivative source terms.

## 2026-06-14

Category: 🟡 Physics + Code

- Began Stage 3E symbolic derivation tooling for nonconstant `q(x,z)`. The new SymPy regression uses `q = (1 + lambda x + mu z)^2`, asserts active `q` derivative terms, and compares direct Ricci components/scalar against independent warped-product formulas; no implementation source was changed.
- Documented Stage 3E review caveats: the regression guards the symbolic Ricci engine transitively rather than substituting into the printed Stage 3D abstract expression directly; Stage 3C/3D/3E duplicate helper functions and should eventually share a module; and the chosen profile has `R_zz = 0` because `f_zz = 0` while still testing `z` derivatives through `partial_z q`, `R_xz`, and angular terms.
- Began Stage 3G conformal-cartoon translation tooling (Stage 3G at the time; renumbered to Stage 3F on 2026-06-14). The new SymPy check translates physical `gamma_ww=q` into `q=hww/chi`, verifies first/second derivative and radius identities, and checks the diagonal 4D conformal tracelessness relation with hidden multiplicity; no implementation source was changed.
- Strengthened Stage 3G after review (Stage 3G at the time; renumbered to Stage 3F on 2026-06-14): documented the cartoon/Cartesian-like determinant condition, added repository checks for physical/conformal round trips and `K_ij` reconstruction, and added a `/4` `GR_SPACEDIM=4` denominator guard; no implementation source was changed.
- Began current Stage 3G off-diagonal reduced-metric validation. The new derivation artifacts extend the conformal-cartoon bookkeeping to nonzero `h_xz` / `gamma_xz`, verify determinant, inverse-metric, full-4D tracelessness, `K_xz`, `/4` denominator, and diagonal-limit identities; no implementation source was changed.
- Promoted the Stage 3G sheared-flat off-diagonal Ricci gate into the repository. The new SymPy regression verifies Ricci flatness for constant `g_xz = lambda` and `x`-dependent `g_xz = x` pullback metrics using full matrix inversion; no implementation source was changed.
- Completed final Stage 3G review follow-up: added an explicit normalized off-diagonal `det h_4D = 1` guard and documented that Stage 3F is diagonal-only; no implementation source was changed.
- Began Stage 3H CCZ4 RHS block decomposition planning. The new note classifies future RHS blocks by validation route, separates exact geometry/algebra anchors from external/reference-code and convergence-dependent checks, and explicitly defers small-`x` regularity to Stage 3I and unit-test fixture design to Stage 3J; no implementation source was changed.
- Patched Stage 3H after independent review: the connection-sector blueprint now uses `hat_Gamma^A` as the evolved variable with `Z^i` encoded inside it, assigns gauge versus `hat_Gamma^A` block ownership, marks inherited versus modified-cartoon RHS term families, and records required future GL-spectrum and constraint-violation damping validation milestones; no implementation source was changed.
- Began Stage 3I small-`x` regularity documentation. The new notes and symbolic checks record parity expectations, diagonal and off-diagonal axis smoothness conditions, conformal removable-singularity limits, and defer ghost-cell/stencil/axis-boundary implementation choices; no implementation source was changed.
- Patched Stage 3I after review: explicitly distinguished the co-located cartoon axis and physical black-string singularity, clarified turduckening versus removable-axis regularity, and added an assembled `tilde_Gamma^x` / `hat_Gamma^x` small-axis guard with hidden multiplicity; no implementation source was changed.
- Began Stage 3J unit-test fixture design. The new design maps existing Stage 3C-3I symbolic gates to future C++ unit tests, RHS-block checks, reference-code comparisons, and convergence/evolution validation before source implementation; no implementation source was changed.
- Patched Stage 3J after review: added a round-`S^2` positive-curvature fixture, scoped the Schwarzschild x `S^1` anchor to slicing-aware checks, clarified tolerance policy, and separated the physical GL dispersion milestone from constraint-violation damping injection; no implementation source was changed.
- Began Stage 3K minimal C++ implementation planning. The plan recommends Stage 4A as the first safe implementation stage: repo-owned conformal-cartoon algebra helpers plus fixture tests, with variable layout audited first and full RHS/gauge/damping/turduckening/AH/radiation/external-reference comparison work deferred; no implementation source was changed.
- Patched Stage 3K after review: clarified Stage 4A as local algebra-helper plus non-grid fixtures only, deferred Ricci/small-axis/grid-wiring work to later explicit approval, required a Stage 4B automated enum/layout gate before grid wiring, and recorded that helper-green does not imply integration or cartoon-evolution correctness; no implementation source was changed.
- Clarified the convention policy for Stage 3 planning: implementation conventions target GRChombo compatibility plus internal consistency, while Pau/external implementations remain later validation references rather than convention authorities; no implementation source was changed.

Category: 🔵 Code

- Started the explicitly approved Stage 4A C++ implementation by adding `code/BlackStringToy/ConformalCartoonAlgebra.hpp` and `code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`. The helper and fixture use hard-coded local values only and do not read grid variables, enums, GRChombo `Cell`/`Vars`/`FArrayBox`, or evolution data. The standalone fixture compiled with `g++` and passed.

Category: 🟡 Physics + Code

- Stage 4A currently validates only local conformal-cartoon algebra from Stages 3F-3G: determinant, block inverse, full 4D trace/tracelessness with hidden multiplicity, `/4` denominator guards, `K_xz`/`K_ww` reconstruction, diagonal limit, and normalized determinant. This is not evidence that modified-cartoon CCZ4 evolution works.
- Patched Stage 4A after review: replaced the hard-coded `chi` exponent with `-1/physical_spatial_dim`, changed local floating-point zero guards to tolerance-based checks, added independently derived `K_xz = 41/8` and `K_ww = 43/4` fixture oracles, and renamed active roadmap terminology to Stage 4A/4B. No grid wiring, enum edits, RHS terms, or evolution code were added.
- Began Stage 4B by adding a standalone public CCZ4 baseline-layout fixture. At the time BlackStringToy still aliased the public layout; Stage 4C later replaced that alias with repo-owned hidden variables. The check records visible helper-input bindings and current public slots before `K` and `Theta`, but does not edit enums, prove hidden `hww/Aww` placement, read grid variables, or wire Stage 4A helpers to evolution.
- Patched Stage 4B after review: narrowed the claim to a public GRChombo CCZ4 baseline-layout check. Because `c_hww` and `c_Aww` do not exist yet, real hidden-placement assertions are deferred to Stage 4C inside `UserVariables.hpp`; Stage 4D is the earliest planned grid-variable handoff.
- Began Stage 4C by adding real repo-owned `hww` and `Aww` variable entries in `BlackStringToy/UserVariables.hpp`, with header-level assertions that `c_hww == c_K - 1` and `c_Aww == c_Theta - 1`. Added a non-grid placement fixture; no helper reads grid data and no RHS/evolution code was added.
- Ran the Stage 4C scratch build-validation follow-up. The full Docker/GRChombo `BlackStringToy` scaffold compile succeeds with the 27-variable layout and produces the expected executable. The inherited one-step smoke run initially exposed a 25-entry `vars_parity` mismatch, fixed by adding even parities for `hww/Aww`; after that, the smoke run reaches the first advance but aborts because `hww` and `Aww` are non-finite/uninitialized in the inherited scaffold. No grid handoff or source-term implementation was added.

## 2026-06-15

Category: 🟡 Physics + Code

- Reconciled roadmap, checklist, TODO, and build-note bookkeeping after the committed Stage 4C work. Stage 3 is recorded as complete; Stage 4A, Stage 4B, and Stage 4C are recorded as complete and committed; and Stage 4D is identified as the next narrow step to give `hww/Aww` finite scaffold values so the cheap smoke run no longer dies immediately from NaNs. This remains bookkeeping only and does not implement grid handoff, RHS terms, or physical evolution.
- Compiled companion PDFs for all current `docs/physics_notes/*.tex` sources, so Stage 3A through Stage 3K physics-note LaTeX sources now have same-name PDF review artifacts. Updated the physics-notes Makefile so `make -C docs/physics_notes all` builds all current notes.

Category: 🔵 Code

- Patched Stage 4D so the temporary `hww/Aww` finite scaffold support is explicitly smoke-only. The new `scaffold_freeze_hidden` parameter defaults off, while the cheap smoke parameter file turns it on to freeze `hww = 1.0`, `Aww = 0.0`, and inert hidden RHS slots. The cheap smoke workload was kept to `max_level = 1`; the scratch build succeeded and the smoke log ended with `GRChombo finished.`

Category: 🟡 Physics + Code

- Stage 4D still does not implement physical black-string initial data, cartoon Ricci terms, hidden-sector CCZ4 RHS terms, gauge/damping changes, small-axis regularization, finite differences, AH finding, radiation extraction, or meaningful black-string evolution. Future real hidden-sector RHS work must disable or replace the smoke-only freeze and add a loud guard against using both paths together.

Category: 🔵 Code

- Began Stage 4E by adding a default-off `scaffold_check_hidden_handoff` diagnostic. When enabled by the cheap smoke parameter file, the diagnostic reads the real scaffold grid slots for `chi`, visible conformal metric/A components, `hww`, `Aww`, and `K`, passes them to the Stage 4A local conformal-cartoon algebra helper, and checks finite determinant, inverse, trace, and `K_ij` reconstruction outputs. The helper output is not written back to the grid or used for evolution.
- The Stage 4E scratch Docker/GRChombo build completed, and the cheap smoke log ended with `GRChombo finished` with both `scaffold_freeze_hidden = 1` and `scaffold_check_hidden_handoff = 1` enabled. The wrapper still needed the known post-run scratch ownership repair outside the sandbox.

Category: 🟡 Physics + Code

- Stage 4E is a handoff sanity check only. It does not implement cartoon Ricci terms, hidden-sector RHS terms, gauge/damping changes, small-axis regularization, finite differences, real black-string initial data, or physically meaningful black-string evolution.

Category: 🔵 Code

- Patched Stage 4E after review with a standalone distinct-value mapping fixture. The test assigns different local values to `chi`, visible conformal metric/A components, `hww`, `Aww`, and `K`, verifies the helper input map, and checks an independent `K_ww = 51.375` oracle so swapped slots cannot hide behind finite symmetric smoke data.

Category: 🔵 Code

- Began Stage 4F by adding `CartoonRicciInterface.hpp` and a compile/type fixture for the future cartoon Ricci helper. The interface records local-value inputs for `x`, `chi`, reduced conformal metric components, first derivatives, second derivatives, hidden multiplicity, and future `R_xx/R_xz/R_zz/R_ww` outputs. It deliberately does not compute Ricci values, source terms, RHS terms, or evolution updates.
- Patched Stage 4F after review to make the interface contract explicit: it is the metric-derivative cartoon Ricci form validated by Stages 3C-3E, not the Gamma-based GRChombo `CCZ4Geometry` Ricci form. Later Ricci/RHS work must resolve how that output is consumed by GRChombo-facing RHS code and must still apply Stage 3I small-`x` regularity rules.
- Began Stage 4G by implementing the first local metric-derivative cartoon Ricci helper in `CartoonRicciInterface.hpp` plus a standalone Stage 4G fixture. The helper is local-value only, computes away-axis physical Ricci components from `h/chi` metric derivatives, returns `R_xx/R_xz/R_zz/R_ww`, and is not wired into grid data, RHS terms, or evolution. The fixture checks flat zero Ricci, a constant-`q` cone with `R_ww = -3/4`, a nonconstant-`q` warped-product oracle with `R_xx = -2` and `R_ww = -12`, the Stage 3G `x`-shear flat gate, and rejection of `x = 0`.
- Reviewed Stage 4G as its own implementation step. Kept the helper local and metric-derivative, clarified stale Stage 4F-era wording, and removed a small dead local assembly path without changing behavior. The Stage 4G and directly affected Stage 4F standalone fixtures still pass.
- Began Stage 4H as a planning-only compatibility pass. The new note records that BlackStringToy currently calls inherited public `CCZ4RHS`, whose `CCZ4Geometry::compute_ricci_Z` path uses evolved `Gamma`, `d1.Gamma`, Christoffels, conformal metric derivatives, and `Z_over_chi`. Stage 4G remains the checked metric-derivative Ricci source/oracle, but it must not be wired into evolution until a local Ricci-to-RHS contract resolves the convention bridge.
- Began Stage 4I by adding `CartoonRicciBridge.hpp` and a standalone bridge-contract fixture. The bridge keeps `CartoonRicci::RicciComponents` as a distinct physical lower/lower Ricci type and pins the full 4D contraction convention, including the off-diagonal factor and hidden multiplicity. The fixture checks the hard-coded `451` trace oracle, `225.5` physical scalar with `chi = 0.5`, negative sensitivity checks, and confirms no RHS wiring is implemented.
- Patched Stage 4I after review so cartoon Ricci output is harder to misuse. The raw Stage 4G component storage is now private, the Stage 4I bridge exposes a distinct RHS-facing component view, and the Stage 4G/4I fixtures access Ricci components through the bridge rather than public `ricci.xx`-style fields. No RHS wiring, grid reads, or evolution changes were added.
- Began Stage 4J by adding a local Ricci-to-RHS contract. The contract accepts only the Stage 4I bridge-approved Ricci view, records the lower/lower and hidden-multiplicity conventions for future RHS source blocks, and tests the `451` trace oracle plus `Rww`/off-diagonal omission sensitivity without implementing RHS formulas or wiring evolution.
- Patched Stage 4J after review by making the internal `RicciAccess` doorway private while preserving the public `to_rhs_ricci_components(...)` bridge path. The bridge still returns a typed by-value RHS-facing view and does not expose raw mutable Ricci storage.
- Began Stage 4K by adding a local RHS source-block skeleton. The skeleton consumes the Stage 4J contract type, exposes named inert future output fields such as `rhs_chi`, `rhs_hww`, `rhs_Aww`, and `rhs_K`, and deliberately does not implement real source formulas, grid reads, or evolution wiring.
- Began Stage 4L by adding the first local RHS formula block: a trace-free Ricci projection inside the local source-block layer. The block consumes the Stage 4J contract type and lower conformal metric values, uses the bridge-approved Ricci contraction with the full 4D trace, includes the off-diagonal and hidden `ww` multiplicity factors, and applies the `/4` projection denominator. This remains local-only and is not wired into `BlackStringToyLevel`, grid data, or live evolution.
- Patched Stage 4L after review so the projection uses the shared `ConformalCartoonAlgebra::trace_free_part` helper and named trace-denominator convention instead of local projection arithmetic. The Stage 4L boundary now reuses the away-axis contract guard and rejects `x = 0`; full small-axis regularization remains deferred.
- Began Stage 4M by adding a named away-axis policy for local cartoon/RHS expressions. The policy centralizes finite `x > 0` validation and guarded `1/x` / `1/x^2` helpers, and the Stage 4J contract boundary now uses it so Stage 4K/4L inherit the same axis decision. This does not implement Stage 3I small-axis regularization, clamp `x`, substitute epsilon, add RHS physics, or wire anything into evolution.
- Began Stage 4N by adding guarded away-axis singular-combination helpers for future cartoon source terms. The helpers route `d_x f / x` and `(f - g) / x^2` through the Stage 4M axis policy, reject nonfinite inputs, and deliberately do not implement small-axis regularization, new RHS physics, grid reads, or evolution wiring.

## 2026-06-17

Category: 🔵 Code

- Began Stage 4O by locking the current axis-regime semantics before real singular source terms use the helper path. `CartoonAxisPolicy` now exposes `AwayAxisOnly` as the only implemented regime, records that regularized-axis support is not implemented, and keeps `1/x^2` as a separately guarded away-axis primitive rather than defining it by squaring `1/x`.

Category: 🟡 Physics + Code

- Stage 4O does not implement Stage 3I small-axis regularization, clamping, epsilon replacement, new RHS source terms, grid reads, or evolution wiring. Future clamped or regularized behavior must decide the `1/x` and `1/x^2` semantics independently.

Category: 🔵 Code

- Began Stage 4P by adding named away-axis cartoon geometry primitives for `(d_x hww) / x` and `(hxx - hww) / x^2`. The helper routes through the Stage 4N singular-combination path and remains local-value-only.

Category: 🟡 Physics + Code

- Stage 4P does not implement Stage 3I small-axis regularization, a full Ricci tensor, CCZ4 RHS source terms, finite-difference stencils, grid reads, or evolution wiring.
- Patched Stage 4P after review to document the regularity precondition for `(hxx - hww) / x^2`: finite axis behavior requires `hxx - hww = O(x^2)`, and the away-axis helper does not enforce that matching condition. Future near-axis source-block use needs a separate regularity/matching guard.

Category: 🔵 Code

- Began Stage 4Q by adding `CartoonRegularityChecks.hpp`, a local pointwise guard for the `hxx - hww = O(x^2)` matching condition. The guard uses the Stage 4O away-axis policy, checks finite inputs, and rejects obvious local mismatches before future near-axis use of the Stage 4P primitive.

Category: 🟡 Physics + Code

- Stage 4Q is not Stage 3I small-axis regularization. It cannot prove analytic regularity from one point, does not construct finite axis limits, and does not add Ricci/RHS physics, grid reads, finite differences, or evolution wiring.

Category: 🔵 Code

- Began Stage 4R by adding `CartoonRegularityGuardedSources.hpp`, the first local source-style package for regularity-sensitive cartoon geometry ingredients. The helper calls the Stage 4Q matching guard before returning the Stage 4P primitives and matching residual.

Category: 🟡 Physics + Code

- Stage 4R remains local and inert. It does not implement full Ricci, full CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, finite differences, or evolution wiring.
- Patched Stage 4R after review so raw Stage 4P exposes only the low-risk `(d_x hww) / x` primitive as a public output. The regularity-sensitive `(hxx - hww) / x^2` source-facing value now appears only through the Stage 4R guarded path after the Stage 4Q matching guard has passed.

Category: 🔵 Code

- Began Stage 4S by connecting the Stage 4R guarded geometry package into the local RHS source-block skeleton. `CartoonRhsSourceBlock.hpp` now has a narrow local guarded-geometry source-block path that calls the Stage 4R helper and carries the guarded geometry package as source-block output.

Category: 🟡 Physics + Code

- Stage 4S keeps the risky `(hxx - hww) / x^2` ingredient source-facing only through the Stage 4R guarded path. It does not implement full Ricci, full CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, finite differences, or evolution wiring.

Category: 🔵 Code

- Patched Stage 4S after review so the guarded geometry package is checked-by-construction. `RegularityGuardedGeometrySources` now has private storage, accessor methods, and construction limited to the Stage 4R guarded compute path. The local RHS source-block carry output is likewise not an open aggregate, and the former Stage 4P `detail` helper for the risky metric-difference value was removed.

Category: 🟡 Physics + Code

- The patch hardens the local type boundary only. It still does not add a physical source formula, full Ricci, CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, finite differences, or evolution wiring.

Category: 🔵 Code

- Began Stage 4T by adding `CartoonGuardedSourceConsumers.hpp`, the first local consumer of the checked guarded geometry package. The consumer is explicitly a diagnostic/probe: it accepts only the non-forgeable Stage 4R guarded package and reports the checked geometry ingredients plus the Stage 4Q residual status.

Category: 🟡 Physics + Code

- Stage 4T did not add a physical source formula because no isolated Ricci/RHS sub-expression was extracted in this stage. It does not implement full Ricci, a Ricci sub-block, full CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, finite differences, or evolution wiring.

Category: 🔵 Code

- Began Stage 4U by adding `CartoonSourceFormulaAuthoringGate.hpp`, a typed authoring gate for future source formulas needing the regularity-sensitive `(hxx - hww) / x^2` ingredient. The gate carries the checked Stage 4R package and gives future formulas a local signature shape that avoids raw `h_xx`, `h_ww`, and `x` inputs for that ingredient.

Category: 🟡 Physics + Code

- Stage 4U documents the authoring rule: direct source-formula use of `difference_over_x2(h_xx, h_ww, x)` for the metric matching-sensitive `(hxx - hww) / x^2` ingredient is forbidden. The generic helper remains available for generic away-axis combinations. No Ricci/RHS formula, grid read, finite-difference stencil, small-axis regularization, or evolution wiring was added.

Category: 🟡 Physics + Code

- Patched Stage 4U after review to make the remaining limitation explicit. The typed authoring path is enforced, but a future formula author could still hand-write `(hxx - hww) / x^2` or call the generic `difference_over_x2` helper unless review or a later lint/CI stage catches it. Added a warning at the generic helper and corrected the fixture-design table so it no longer claims this bypass is mechanically caught.

Category: 🟡 Physics + Code

- Started Stage 4V as a derivation-lock check for the first real local
  source-formula consumer of the Stage 4U authoring gate. No formula code was
  added: the existing Stage 4G helper validates complete metric-derivative
  Ricci from full local metric/derivative inputs, and Stage 4L validates a
  trace-free projection after Ricci components already exist, but neither
  exposes a tiny named sub-expression that consumes only the checked Stage 4U
  geometry package. The next step is to derive or extract that exact
  sub-expression, including coefficients, sign convention, and a hard-coded
  oracle.

Category: 🟡 Physics + Code

- Started Stage 4W as a derivation-lock stage for hidden-sphere CCZ4
  contributions and the first serious `R_ww` target. Added a Markdown note
  that maps hidden contributions to the `chi`, `h_ww`, `K`/`Theta`, and
  trace-free `A_IJ` source blocks; records the warped-product `R_ww`
  expression; preserves the Stage 4G flat, constant-cone, and
  nonconstant-hidden-metric oracles; and identifies the missing
  `h_xz = O(x)` off-diagonal axis/parity guard. No formula code, full Ricci,
  CCZ4 RHS, grid reads, finite differences, small-axis regularization, or
  evolution wiring was added.

Category: 🟡 Physics + Code

- Patched Stage 4W after review to separate the conformal hidden Ricci target
  `tilde{R}_ww[h]`, the `d = 4` conformal-factor correction `R^chi_ww`, and
  the physical CCZ4 input `R_ww[gamma]`. The note now records a future
  varying-`chi` oracle, pins the curvature/lapse `A_IJ` sign against the
  Stage 3A `K_IJ` convention caveat, and derives `h_xz = O(x)` from
  first-principles reflection parity about the cartoon axis. No code or tests
  were added.

Category: 🔵 Code

- Started Stage 4X by adding `CartoonCheckedHxzOverX.hpp`, a local checked
  `h_xz / x` package for the away-axis quotient.
  The helper uses the existing away-axis policy, rejects invalid axis and
  nonfinite inputs, allows finite nonzero quotients such as `h_xz = 3x`, and
  exposes the trusted value through a non-aggregate package for future source
  consumers.

Category: 🟡 Physics + Code

- Stage 4X remains a local source ingredient only. It checks only finite
  `h_xz / x` at the supplied away-axis point. It does not implement
  `tilde{R}_ww[h]`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS,
  finite-axis regularization, grid reads, finite differences, or evolution
  wiring, and it does not prove global grid parity or `h_xz = O(x)`.

Category: 🔵 Code

- Started Stage 4Y by adding `CartoonConformalRwwSingularBlock.hpp`, the first
  guarded formula sub-block from the conformal hidden Ricci target. It computes
  only `G_sing = (h_zz / D) Delta_xw - q_xz^2 / D`, with `Delta_xw` consumed
  through the Stage 4U/4R checked package and `q_xz` consumed through the
  Stage 4X checked package. The standalone fixture passed the flat,
  constant-cone, distinct-value, checked-input boundary, pre-formula rejection,
  and determinant rejection checks.

Category: 🟡 Physics + Code

- Stage 4Y is a local away-axis conformal-Ricci sub-block only. It does not
  implement full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci,
  CCZ4 RHS, finite-axis regularization, grid reads, finite differences,
  global parity validation, or evolution wiring.

Category: 🔵 Code

- Patched Stage 4Y after review to close the raw/checked consistency hole.
  `CartoonConformalRwwSingularBlock.hpp` now exposes a non-forgeable
  `ConformalRwwSingularBlockInputs` package minted from one local metric
  point. The factory computes the checked Stage 4U `Delta_xw`, checked Stage
  4X `q_xz`, and reduced determinant from the same `x`, `h_xx`, `h_xz`,
  `h_zz`, `h_ww` input set, so the formula boundary cannot mix checked
  singular ingredients from one point with determinant data from another.

Category: 🟡 Physics + Code

- The Stage 4Y formula is unchanged:
  `G_sing = (h_zz / D) Delta_xw - q_xz^2 / D`. This patch preserves checked
  singular-input safety and same-point consistency only; it still does not add
  more of `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4
  RHS, grid reads, finite-axis regularization, global parity validation, or
  evolution wiring.

Category: 🟡 Physics + Code

- Integrated the Stage 4 hidden-sphere Ricci roadmap in
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`. The plan records the
  protected 4D cartoon conventions, hidden-sphere CCZ4 contribution map,
  conformal/physical Ricci split, Stage 4Y singular block, future `W_x/x`
  gradient block, Hessian block, `R^chi_ww` guard-stack requirement, hard
  split-vs-direct physical Ricci identity gate, true `h_xz` parity validation
  owner, `hat_Gamma^x` derivation/GL-growth anchor stages, and smoke-freeze
  removal. No code, tests, RHS wiring, or evolution path was added.

Category: 🔵 Code

- Started Stage 4Z by adding `CartoonCheckedDxhwwOverX.hpp` and
  `CartoonConformalRwwGradientBlock.hpp`. The checked quotient package mints
  the local away-axis `p_W = W_x / x` ingredient and does not claim global
  parity or finite-axis regularity. The gradient block computes only
  `G_grad = -(C/D) p_W + (q_xz W_z)/D - (C W_x^2 - 2 B W_x W_z + A W_z^2)/(4 W D)`
  from a single-source local input package that also mints checked `q_xz`
  through the Stage 4X path.

Category: 🟡 Physics + Code

- Stage 4Z is still only a conformal hidden Ricci first-derivative gradient
  sub-block. It does not implement the Stage 4Y singular block, the Hessian
  block, full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci,
  CCZ4 RHS, grid reads, finite-axis regularization, global parity validation,
  or evolution wiring.

Category: 🟡 Physics + Docs

- Stage 4AA locks the conformal hidden Ricci Hessian sub-block derivation in
  documentation only. The roadmap and Stage 4W derivation note now record
  `G^Hess_ww = -(sqrt(W)/x)[(C/D)H_xx - (2B/D)H_xz + (A/D)H_zz]`, the
  `rho` derivative formulas, and the reduced-base Christoffels needed by
  Stage 4AB.

Category: 🟡 Physics + Docs

- Stage 4AA records Hessian oracle values: flat data gives `0`, the constant
  cone gives `0`, and the flat-base nonconstant hidden metric
  `W=(1+x)^2` at `x=1` gives `G^Hess_ww=-4`. Combined with the reviewed
  Stage 4Y and 4Z values, that fixture gives `tilde R_ww=-12`. Claude Audit
  A verifies the fully distinct nonsymmetric derivative sample as
  `G^Hess_ww=-8558/2883`, with the full conformal sum `-3576/961`.

Category: 🟡 Physics + Docs

- Checkpoint A locked the nonsymmetric Hessian oracle. The oracle exercises
  off-diagonal Christoffels, `rho_xz`, `W_z`-dependent terms, and the
  `(-2B/D)` contraction; the flat, cone, and `W=(1+x)^2` oracles are not
  sufficient by themselves. Stage 4AB may now implement the Hessian block, but
  only with this verified oracle included in the test. Stage 4AA does not add
  code, tests, full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full
  Ricci, CCZ4 RHS, grid reads, or evolution wiring.

Category: 🟡 Physics + Code

- Stage 4AB implements only the local away-axis conformal Hessian sub-block in
  `CartoonConformalRwwHessianBlock.hpp`:
  `G_Hess = -(sqrt(W)/x)[(C/D)H_xx - (2B/D)H_xz + (A/D)H_zz]`. The input
  package is non-forgeable and single-sourced from one local point, including
  reduced metric values, reduced-base first derivatives, and `W` first/second
  derivatives. The factory rejects invalid axis values, nonpositive `W`,
  nonfinite inputs, and nonpositive reduced determinant.

Category: 🟡 Physics + Code

- The Stage 4AB fixture covers flat `0`, constant cone `0`, nonconstant
  `W=(1+x)^2` giving `-4`, and the Claude-verified nonsymmetric oracle
  `G_Hess=-8558/2883`. It also checks a test-only conformal sub-block sum
  `G_sing + G_grad + G_Hess = -3576/961` using the reviewed Stage 4Y/4Z
  blocks. Stage 4AB still does not assemble full `tilde R_ww`, implement
  `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, or
  evolution wiring.

Category: 🟡 Physics + Code

- Stage 4AC implements the local away-axis conformal assembly in
  `CartoonConformalRww.hpp`:
  `tilde R_ww[h] = G_sing + G_grad + G_Hess`. The non-forgeable
  `ConformalRwwInputs` package is minted from one local metric/derivative
  point and internally constructs the reviewed Stage 4Y, 4Z, and 4AB
  sub-block input packages, so callers cannot assemble conformal `R_ww` from
  loose precomputed block values or mixed local points.

Category: 🟡 Physics + Code

- The Stage 4AC fixture covers flat `0`, constant-cone `-3/4`,
  nonconstant `W=(1+x)^2` giving `-12`, and the Claude-verified nonsymmetric
  conformal value `-3576/961`. It also compares the nonconstant and
  nonsymmetric `chi=1` samples against the independent Stage 4G
  metric-derivative Ricci helper. Stage 4AC remains conformal
  `tilde R_ww[h]` only: it does not implement `R^chi_ww`, physical
  `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, finite-axis regularization,
  or evolution wiring. Checkpoint B / Claude Audit B is required before
  Stage 4AD begins the `R^chi_ww` guard-stack work.

Category: 🟡 Physics + Code

- Checkpoint B / Claude Audit B found one pre-4AD cleanup: Stage 4Y accepted
  finite negative reduced determinants while Stage 4Z, Stage 4AB, and
  Stage 4AC required positive `D = AC - B^2`. The Stage 4Y singular block now
  uses the same finite-positive determinant policy as the other conformal
  `R_ww` pieces. The formula is unchanged, and the Stage 4Y fixture now
  includes a negative-determinant rejection case.

Category: 🟡 Physics + Docs

- Stage 4AD locks the conformal-factor correction derivation for the hidden
  Ricci split `R_ww[gamma] = tilde R_ww[h] + R^chi_ww`. The documentation now
  records `D_wD_w chi`, the full conformal Laplacian with hidden multiplicity
  `(2/W)D_wD_w chi`, the reduced scalar Hessian pieces, and the conformal
  gradient norm. The Stage 4AE guard stack must add a checked local
  `chi_x/x` ingredient, reuse checked `q_xz` and checked `p_W`, and keep the
  away-axis-only, positive-`chi`, positive-`W`, positive-determinant policy.

Category: 🟡 Physics + Docs

- Stage 4AD records the `R^chi_ww` oracle set for Stage 4AE: constant `chi`
  gives zero, flat conformal data with `chi=1+a x` gives `11/144` at
  `x=2`, `a=0.1`, and a hand-derived z-dependent candidate
  `chi=1+b z` gives `-1/64` at `z=3`, `b=0.2`. The z-dependent candidate
  remains pending independent audit or replacement by a stronger off-diagonal
  oracle before Stage 4AE. No code, tests, physical Ricci assembly,
  split-vs-direct identity gate, RHS, grid reads, or evolution wiring was
  added.

Category: 🟡 Physics + Code

- Stage 4AE implements only the local away-axis conformal-factor correction
  `R^chi_ww` in `CartoonConformalFactorRww.hpp`. Its single-source input
  package validates positive `x`, `chi`, `W`, and determinant and mints
  checked `q_xz`, `p_W`, and `p_chi=chi_x/x`. The implementation retains the
  hidden multiplicity term `(2/W)D_wD_w chi`.

Category: 🟡 Physics + Code

- The Stage 4AE fixture locks constant-`chi` zero, linear-`x` `11/144`,
  linear-`z` `-1/64`, and the nonsymmetric intermediates `131/62`,
  `10430/2883`, `49/31` with final `R^chi_ww=63341/48050`. A test-only
  Stage 4G physical-minus-Stage 4AC conformal comparison passes for the
  nonsymmetric sample. This is not the general Stage 4AF hard identity gate;
  Checkpoint C / Claude Audit C is required before 4AF.

## 2026-06-19 - Stage 4AF Internal Split-Vs-Direct Identity Gate

Category: Physics + Code

- Added a local test-only hard gate for
  `tilde R_ww[h] + R^chi_ww = R_ww[gamma]`. The direct side explicitly forms
  the physical metric jet from `gamma=h/chi` through the full first/second
  product rules, then supplies it to Stage 4G with `chi=1`.
- The gate passes constant `chi`, flat linear-`x` and linear-`z` `chi`, and
  three nonsymmetric varying-`chi` points with nonzero
  `W_x,W_z,W_xx,W_xz,W_zz`. Maximum observed split/direct residual is about
  `4.44e-16`; a separate check confirms `A,B,C` second derivatives cancel
  from direct `R_ww`.
- This is internal validation, not external validation or production physical
  assembly. Stage 4AG is next and Checkpoint D remains pending before 4AH.

## 2026-06-19 - Stage 4AG Two-Sided `h_xz` Parity Gate

Category: Physics + Code

- Added `CartoonHxzParityValidation.hpp` with a factory-only validated stencil
  token. The factory accepts separate positive/negative sample lists, requires
  one-to-one opposite-radius partners, checks odd `h_xz`, checks even
  `h_xz/x`, and checks axis zero when an axis value is supplied.
- The standalone fixture accepts smooth odd `a x+b x^3` data at multiple
  radii without demanding constant `h_xz/x`, and rejects even contamination,
  nonzero axis data, partner errors, invalid coordinate signs, nonfinite data,
  and invalid tolerance.
- Tolerances are validation-only. No clamp, epsilon division, finite-axis
  regularization, physical `R_ww`, RHS, or evolution path was added.
  Checkpoint D / Claude Audit D remains required before Stage 4AH.

## 2026-06-19 - Stage 4AH Local Away-Axis Physical `R_ww`

Category: Physics + Code

- Added `CartoonAwayAxisPhysicalRww.hpp` with a non-forgeable single-source
  input package. Its factory accepts one local metric/conformal-factor jet and
  internally mints both Stage 4AC and Stage 4AE packages.
- The result exposes `conformal_part()`, `conformal_factor_part()`, and
  `physical_rww()`. Flat, cone, linear-`x`, linear-`z`, nonsymmetric direct
  Stage 4G, and invalid-input checks pass; the nonsymmetric direct residual is
  about `4.44e-16`.
- This is away-axis local assembly only. Actual-grid parity/matching, finite
  axis, grid reads, RHS, and evolution remain false. Stage 4AI is next.

## 2026-06-19 - Stage 4AI Local Physical-`R_ww` Contract

Category: Physics + Code

- Extended the Stage 4AH result with same-point `chi` and conformal
  `h^ww=1/h_ww`, retained behind its non-forgeable factory.
- Added `CartoonPhysicalRwwRhsContract.hpp`, which accepts only the Stage 4AH
  physical result and computes `2 h^ww R_ww[gamma]` plus
  `chi 2 h^ww R_ww[gamma]`.
- The standalone fixture passes flat, cone, linear-`x`, and nonsymmetric
  checks and compile-time type-boundary checks. This is not full Ricci, live
  RHS, grid validation, finite-axis support, or evolution. Checkpoint E /
  Claude Audit E is required before Stage 4AJ.

## 2026-06-19 - Stage 4AJ Physical Hidden Lapse Hessian

Category: Physics + Code

- Added `CartoonCheckedDxalphaOverX.hpp` and
  `CartoonPhysicalHiddenLapseHessian.hpp` for local physical
  `D_wD_w alpha`.
- The factory single-sources `x,A,B,C,W,chi`, first derivatives, positive
  determinant, and checked `q`, `p_W`, `p_chi`, and `p_alpha`. The formula uses
  `gamma=h/chi`; the varying-`chi` oracle catches conformal-hessian reuse.
- The standalone fixture passes constant-lapse, flat linear-`x`, flat
  linear-`z`, varying-`chi`, and nonsymmetric oracles plus direct physical
  comparisons. The source minus sign, full `A_ww` source block, RHS, grid
  reads, finite-axis support, and evolution remain future work.

## 2026-06-19 - Stage 4AK Aww Curvature/Lapse Geometric Core

Category: Physics + Code

- Added `CartoonAwwCurvatureLapseCore.hpp` for the local away-axis geometric
  core `C_ww=-D_wD_w alpha+alpha R_ww[gamma]`.
- The factory single-sources the reviewed Stage 4AH physical-Ricci package and
  Stage 4AJ physical-lapse-Hessian package from one
  metric/conformal-factor/lapse jet. The result exposes `minus_dww_alpha`,
  `alpha_rww`, and `curvature_lapse_core`.
- The standalone fixture passes flat, flat linear-lapse, constant-cone,
  varying-`chi`, nonsymmetric same-point composition, and invalid-input checks.
  Z4, trace-free projection, the outer `chi` prefactor, nonlinear A/K terms,
  RHS, grid reads, finite-axis support, and evolution remain future work.

## 2026-06-19 - Stage 4AL Trace-Free Curvature/Lapse Block

Category: Physics + Code

- Added `CartoonTraceFreeCurvatureLapseBlock.hpp` for the local away-axis
  block `source_IJ=chi(C_IJ-h_IJ C/4)` with
  `C_IJ=-D_ID_J alpha+alpha R_IJ[gamma]`.
- Visible Ricci components cross the Stage 4G/4I typed bridge, hidden `ww`
  uses Stage 4AH/4AK, and the block requires Stage 4G and Stage 4AH physical
  `R_ww` to agree before assembly. Visible lapse Hessians use the physical
  `gamma=h/chi` correction.
- The fixture passes flat, linear-lapse, constant-cone, nonsymmetric
  varying-`chi`/alpha, zero trace-free trace, and invalid-input checks. Z4
  terms, nonlinear A/K terms, full RHS, grid wiring, finite-axis support, and
  evolution remain future work. Checkpoint F / Claude Audit F is required next.

## 2026-06-19 - Checkpoint F Roadmap Follow-Up

Category: Planning

- Recorded Checkpoint F / Claude Audit F as complete for the local Stage 4AL
  trace-free curvature/lapse block.
- Split the remaining `hat_Gamma^x` path into Stage 4AM derivation/convention
  mapping, Stage 4AN local implementation/contracts, and Stage 4AO hard linear
  GL dispersion/growth-rate validation. Stage 4AO is the hard pre-evolution
  anchor because flat checks alone are insufficient; it must match radius
  convention, z-periodicity, gauge, perturbation sector, resolution, and
  measured growth variable, and Pau is not the convention authority.
- Corrected Stage 4AG status to a synthetic two-sided parity-validator
  primitive only. Stage 4AP now owns actual grid/ghost-cell regularity
  validation for `h_xz=O(x)`, `h_xx-h_ww=O(x^2)`, `W_x=O(x)`, and
  `chi_x=O(x)`.
- Added Stage 4AQ as the finite-axis source evaluation and regularized-limit
  stage, with no epsilon replacement or silent clamping. Later ownership is now
  Stage 4AR local RHS integration, Stage 4AS default-off live wiring, Stage 4AT
  smoke-freeze removal/replacement, and Stage 4AU final Stage 4 exit review.

## 2026-06-19 - Stage 4AM Hat Gamma X Derivation Lock

Category: Physics + Planning

- Added `docs/derivations/stage4AM_hatGammaX_derivation.md` to lock the
  GRChombo-facing hatted-connection convention. GRChombo evolves
  `vars.Gamma=chris.contracted+2 Z_over_chi`; the local source variable named
  `Z` is `chi Z_over_chi`, so future project types must name that translation
  explicitly.
- Derived the local hidden contraction for `tilde_Gamma^x`, including the
  `2 W^{-1} Gamma^x_ww` hidden multiplicity, the cartoon-plane
  `Gamma^x_ww` formula, the determinant-reduced cross-check under
  `D W^2=1`, and Stage 4AN oracles `0`, `-3/4`, `-1`, and `-35/961`.
- Mapped the exact GRChombo Gamma RHS terms and classified inherited,
  dimension-dependent, hidden/cartoon, gauge, and Z4/damping terms. Stage 4AM
  adds no code or tests; Stage 4AN owns local contracts and Stage 4AO remains
  the hard GL growth/dispersion gate before live RHS/evolution.

## 2026-06-19 - Stage 4AN Local Hat Gamma X Contract

Category: Physics + Code

- Added `CartoonHatGammaX.hpp` for the local away-axis contract
  `hat_Gamma^x=tilde_Gamma^x+2 Z_over_chi^x` using the GRChombo hatted
  convention.
- The factory single-sources `x,A,B,C,W`, first derivatives, and
  `Z_over_chi^x`; it obtains `Delta_xw=(A-W)/x^2` through the checked
  Stage 4U/4R path and `B/x` through the checked Stage 4X package before
  forming `Gamma^x_ww`.
- The standalone fixture passes the flat, constant-cone, determinant-one
  off-diagonal, distinct derivative, hatted-convention, rejection, and
  non-aggregate type checks. Gamma RHS, GL validation, grid regularity,
  finite-axis handling, and evolution remain future work. Stage 4AO is next.

## 2026-06-19 - Stage 4AO GL Gate Assessment

Category: Physics + Planning

- Assessed the hard Gregory-Laflamme dispersion/growth validation gate and
  recorded Outcome B: it is not yet implementable from the current repo state.
- Stage 4AN supplies only the local `hat_Gamma^x` contract. The project still
  lacks the complete coupled linearized RHS for the chosen sector, a locked
  uniform 5D black-string background/gauge convention, a primary-literature or
  internally documented GL spectrum target, and a geometric growth observable.
- No fake flat or `hat_Gamma^x`-only test was added. Stage 4AO remains
  incomplete and Checkpoint G has not passed; production RHS/evolution
  integration remains blocked pending the validation sequence.

## 2026-06-20 - Stage 4AO Four-Part GL Validation Roadmap

Category: Planning

- Replaced the broad Stage 4AO blocked-gate wording with a front-loaded
  sequence: 4AO-A background and analytic residual lock, 4AO-B discrete
  operator preflight, 4AO-C frozen-gauge spectral gate, and 4AO-D
  live-gauge/full acceptance.
- 4AO-A now owns the exact uniform ingoing-GP black-string background, `r0`,
  compact-`z`, background slicing gauge, evolution gauge-driver and
  initial-gauge startup family, `K_ij` sign, perturbation sector, geometric
  observable, continuum residual, full `hat_Gamma^x` hidden contraction, and
  analytic `1/x` cancellations.
- Recorded the hard ordering: `k_c r0 ~= 0.876` is provisional until
  primary-source convention mapping is complete; flat tests are insufficient;
  no eigensolver before 4AO-B; production Stage 4AR/4AS integration remains
  blocked until 4AO-D; Pau is not the convention authority; `hat_Gamma^x`
  alone is not the physical growth observable; and Checkpoint G passes only
  after 4AO-D.
- This was documentation-only. No code, tests, staging, or commits were added.

## 2026-06-20 - Stage 4AO Claude Audit Follow-Up

Category: Planning

- Clarified the Stage 4AO production-versus-validation boundary. Dedicated
  4AO-B/C/D validation harnesses may construct and evaluate the actual RHS,
  perform finite-difference Jacobian checks, solve frozen/live spectral
  systems, and run the seeded-eigenvector evolution bridge.
- Production integration remains blocked until 4AO-D passes: Stage 4AR
  controlled local RHS integration and Stage 4AS default-off live wiring.
- Clarified 4AO-A gauge ownership: lock the background slicing gauge plus the
  evolution gauge-driver and initial-gauge startup family, and require 4AO-C
  frozen gauge and 4AO-D live gauge to use that same pre-locked family.

## 2026-06-20 - Stage 4AO-A Uniform GP Background Lock

Category: Derivation + Planning

- Added `docs/derivations/stage4AO_A_uniform_gp_background_residual.md` for
  the 4AO-A background and analytic residual lock. The note locks the uniform
  ingoing-GP black string with `r0` as the Schwarzschild areal horizon radius,
  `z~z+L`, `k_n=2 pi n/L`, `alpha=1`,
  `beta^x=sqrt(r0/x)`, flat conformal metric, `chi=1`, and the GRChombo sign
  `partial_t gamma_IJ=-2 alpha K_IJ+L_beta gamma_IJ`.
- Recorded the frozen-GP zero-residual targets for `chi`, `h_IJ`, `K`,
  `Theta`, and the background `hat_Gamma^x` hidden contraction. The full
  hidden contraction gives `Gamma^x_ww=0`, `2 W^{-1} Gamma^x_ww=0`,
  `tilde_Gamma^x=0`, and `hat_Gamma^x=0` on the background.
- Completed the continuum residual ledger for `chi`, `h_xx`, `h_zz`,
  `h_ww`, `h_xz`, `K`, `A_xx`, `A_zz`, `A_ww`, `A_xz`, `Theta`,
  `hat_Gamma^x`, `hat_Gamma^z`, lapse, shift, B variables, Hamiltonian,
  momentum, `det(h)`, and `tr(A)`. The `hat_Gamma^x` RHS table keeps the
  hidden vector-Laplacian multiplicity explicit.
- Separated frozen-GP stationarity from the live moving-puncture gauge startup:
  the locked live gauge family has the prescribed lapse startup residual
  `partial_t alpha=-3 sqrt(r0/x^3)`. The unmodified GP state is not a full
  live-gauge equilibrium, so Stage 4AO-D uses the GRChombo moving-puncture
  family plus the fixed, field-independent validation source
  `S_alpha(x)=+3 sqrt(r0/x^3)`. It is computed from the locked analytic
  background and locked `r0`, not evolved fields; it cancels only the
  zeroth-order lapse drift and leaves the linearized GRChombo perturbation
  operator unchanged.
- Locked the GL validation radial domain to `x in [x_in,x_out]` with
  `x_in>0`, excluding the physical GP singularity at `x=0` rather than using
  an unspecified turduckening extension for 4AO-A/4AO-B.
- Clarified that the apparent horizon is not found from `alpha=0`; diagnostics
  solve the `S2 x S1` MOTS connected to `x=r0` and measure
  `R_H=h(t,z)sqrt(h_ww/chi)` at that surface.
- Added the 4AO-B raw-residual guard: background residual convergence must be
  measured with the unmodified discrete RHS, targeting zero for verified
  geometric/scalar/constraint components and `-3 sqrt(r0/x^3)` for the
  unmodified live moving-puncture lapse equation. The fixed source
  `S_alpha=+3 sqrt(r0/x^3)` is reserved for the stationary live-gauge 4AO-D
  validation harness after raw convergence is demonstrated.
- Updated the older black-string initial-data note so its tentative zero-shift
  startup is no longer confused with the Stage 4AO-A frozen-GP stationarity
  target.

## 2026-06-23 - Stage 4AO-B Discrete Operator Preflight

Category: Code + Validation

- Added `code/BlackStringToy/Stage4AOGPDiscretePreflight.hpp` and
  `code/BlackStringToy/tests/Stage4AOBDiscreteOperatorPreflightTest.cpp` as a
  local validation harness for the locked 4AO-A uniform ingoing-GP background.
- The provisional radial domain is `r0=1`, `x in [0.5,4.0]`, so
  `0<x_in<r0<x_out`.
- The raw residual convergence check evaluates the unmodified local discrete
  RHS. It does not use `S_alpha` to cancel finite-grid residuals. The
  unmodified live moving-puncture lapse equation is checked against
  `-3 sqrt(r0/x^3)`.
- The fixture also isolates the `delta hww` hidden-contraction contribution to
  `delta hat_Gamma^x`, compares a hand-derived actual-discrete-RHS
  Jacobian-vector product against finite differences, and checks cosine/sine
  parity-sector leakage.
- The standalone test passed. Recorded raw residual errors are `0.0150518`,
  `0.00428846`, and `0.00114621` on 256, 512, and 1024 intervals, with
  ratios `3.50983` and `3.74144`. The hidden-contraction error is
  `3.47146e-15`. JVP errors are `0.00183042`, `1.82974e-07`,
  `4.68507e-09`, and `0.000375647`. The original z-independent cosine/sine
  leakage number was later superseded by the z-coupled parity blocker fix
  below.
- No eigensolver, shift-invert, threshold search, production RHS integration,
  live evolution wiring, scripts, or external GRChombo changes were added.
  Stage 4AO-C remains next and Checkpoint G remains open until 4AO-D.

## 2026-06-23 - Stage 4AO-B Parity Blocker Fix

Category: Code + Validation

- Replaced the original z-independent scalar-multiplier parity check after
  Claude review found it only tested cosine/sine orthogonality.
- The Stage 4AO-B parity fixture now applies periodic finite-difference `D_z`
  and `D_zz` stencils to a representative z-coupled sub-operator:
  scalar output receives `D_z beta^z`, `h_xz`-like output receives
  `D_z beta^x`, and `hat_Gamma^z`/momentum-z-like output receives `D_z K`,
  with `D_zz` preservation terms.
- The even sector uses cosines for scalar/zero-z/two-z variables and sines for
  one-z variables; the odd sector swaps those assignments.
- The updated test passed with even-sector allowed norm `7.39609` and leakage
  `2.05279e-17`, odd-sector allowed norm `5.16184` and leakage
  `3.93873e-17`, and flipped-`beta^z` negative-guard leakage `0.79241`.
- This fixes only the 4AO-B parity blocker. It adds no eigensolver,
  shift-invert, threshold search, production RHS integration, live evolution
  wiring, scripts, or external GRChombo changes.

## 2026-06-23 - Stage 4AO-C Frozen-Gauge Spectral Gate Start

Category: Validation Planning

- Opened Stage 4AO-C as an honest blocker/status note instead of adding a
  toy spectral problem.
- The intended frozen-gauge perturbation vector is
  `delta chi`, `delta h_xx`, `delta h_xz`, `delta h_zz`, `delta h_ww`,
  `delta K`, `delta A_xx`, `delta A_xz`, `delta A_zz`, `delta A_ww`,
  `delta Theta`, `delta hat_Gamma^x`, and `delta hat_Gamma^z`, with
  `delta alpha=delta beta^i=delta B^i=0`.
- The gate is blocked because the repo does not yet contain the complete
  coupled modified-cartoon CCZ4 frozen-gauge linearized RHS, radial spectral
  boundary conditions, an actual-operator parity/JVP check, a linearized MOTS
  map to `delta R_H`, shift-invert or equivalent targeted spectral extraction,
  unstable/stable spectral points, radial and boundary convergence, or the
  primary-source `k_c r0` convention map.
- The commonly quoted `k_c r0 ~= 0.876` remains provisional. Checkpoint G
  remains pending until 4AO-D.

## 2026-06-23 - Stage 4AO-C Reuse Inventory

Category: Validation Planning

- Completed a read-only reuse inventory before any 4AO-C eigensolver/operator
  implementation.
- No SLEPc, PETSc `EPS`, project eigenvalue solver, shift-invert driver, or
  sparse GL spectral matrix assembly exists in the checkout.
- PETSc is present through the AHFinder `SNES/KSP` nonlinear MOTS solve path,
  so it is an adapter candidate for future linear solves but not a direct GL
  eigensolver.
- AHFinder has useful nonlinear MOTS pieces, especially `AHStringGeometry`,
  `ExpansionFunction`, `AHInterpolation`, and `PETScAHSolver`, but the
  symmetry-reduced `x=h(z)` geometry adapter, `R_H=h sqrt(h_ww/chi)`
  postprocessor, and linearized MOTS map `delta U -> delta R_H` are still
  missing.
- GRChombo derivative, boundary, interpolation, reduction, extraction, and
  logging helpers are reusable with adapters after the 4AO-C operator and
  boundary contract are defined. Stage 4AO-B local stencils/projections remain
  validation scaffolding, not the actual frozen-gauge spectral operator.
- The next honest 4AO-C implementation step is the validation-only
  frozen-gauge operator wrapper plus radial/z boundary-condition contract,
  followed by actual-operator JVP/parity checks and the linearized-MOTS
  adapter. Eigensolver work remains premature.

## 2026-06-23 - Stage 4AO-C Frozen-Gauge Operator Contract

Category: Code + Validation Planning

- Added `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` and
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp` as
  the validation-only foundation for the future frozen-gauge GL operator.
- The wrapper fixes the 13-variable frozen-gauge state vector:
  `chi`, `h_xx`, `h_xz`, `h_zz`, `h_ww`, `K`, `A_xx`, `A_xz`, `A_zz`,
  `A_ww`, `Theta`, `hat_Gamma^x`, and `hat_Gamma^z`.
- Gauge perturbations remain excluded:
  `delta alpha=delta beta^i=delta B^i=0`.
- The RHS inventory labels helper-only coverage versus missing
  modified-cartoon RHS blocks. No variable has a complete RHS, and the wrapper
  explicitly keeps eigensolver, shift-invert, threshold search, production RHS
  wiring, and 4AO-D live-gauge work false.
- The radial contract is `x in [x_in,x_out]` with `0<x_in<r0<x_out` and
  compact periodic `z`; the default local contract uses the 4AO-B provisional
  values `r0=1`, `x_in=0.5`, `x_out=4.0`. Boundary validation remains future
  work.
- The standalone contract test passed. No eigensolver, shift-invert,
  threshold search, `k_c r0` search, production Stage 4AR/4AS wiring, scripts,
  or external GRChombo changes were added.

## 2026-06-23 - Stage 4AO-C GP-Shift Advection Block

Category: Code + Validation Planning

- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with the
  first actual partial frozen-gauge operator block:
  `L_adv[delta u]=beta_GP^x d_x(delta u)`,
  `beta_GP^x=sqrt(r0/x)`, applied to all 13 frozen-gauge perturbation
  variables.
- Added local validation-only derivative scaffolding: second-order centered
  radial `d_x` on interior points and periodic second-order `D_z` / `D_zz`
  helpers. Boundary outputs are placeholders and boundary validation remains
  missing.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAdvectionBlockTest.cpp` and
  updated the contract fixture so only GP-shift advection is
  `implemented_now`; no frozen-gauge RHS variable is complete and eigensolver
  work remains disallowed.
- The advection fixture checks exact state-vector ordering, gauge exclusion,
  the radial advection oracle, consistent application to all variables, zero
  input, derivative scaffolding, and wrong-beta/sign/order negative guards.
  This does not add Ricci, hidden/cartoon source terms, K/A couplings,
  Theta/constraint coupling, hatted-Gamma RHS terms, a full operator, boundary
  validation, shift-invert, threshold search, production RHS wiring, or
  4AO-D live-gauge work.

## 2026-06-23 - Stage 4AO-C Tensor Shift-Stretching Block

Category: Code + Validation Planning

- Inspected GRChombo's authoritative convention in
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: the `h_ij` RHS contains
  `h_ki partial_j beta^k + h_kj partial_i beta^k
  - (2/GR_SPACEDIM) h_ij div beta`, and the `A_ij` RHS contains the analogous
  tensor-stretching terms alongside separate algebraic/curvature/nonlinear
  terms.
- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with only
  the non-advection tensor shift-stretching block for `h_IJ` and `A_IJ` on
  the locked GP background. With `lambda=sqrt(r0/x^3)`, the implemented
  coefficients are `-7 lambda/4` for `xx`, `-5 lambda/4` for `xz`,
  `-3 lambda/4` for `zz`, and hidden `+5 lambda/4` for `ww`.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTensorShiftStretchingBlockTest.cpp`.
  The fixture verifies pointwise and grid apply paths, acts only on metric
  and `A` slots, leaves scalars and hatted-Gamma slots zero, and keeps
  `complete_operator=false` and `eigensolver_allowed=false`.
- Negative guards prove that dropping hidden `h_ww/A_ww` stretching, using a
  `d=3` trace coefficient, flipping the sign of `partial_x beta^x`, or
  applying the block to scalar/hatted-Gamma slots would fail.
- Still missing: metric `-2 A_IJ` coupling, K/A algebraic dynamics, Ricci and
  curvature, Theta/constraint terms, hatted-Gamma evolution, actual full
  operator JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold crossing, unstable/stable points,
  convergence battery, and the `k_c r0` convention map.

## 2026-06-23 - Stage 4AO-C Algebraic Metric/Chi Coupling Block

Category: Code + Validation Planning

- Inspected GRChombo's authoritative convention in
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: the `h_ij` RHS contains
  `-2 alpha A_ij`, and the `chi` RHS contains
  `(2/GR_SPACEDIM) chi (alpha K - div beta)`.
- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with only the
  local algebraic frozen-gauge block
  `delta h_IJ <- -2 delta A_IJ` and `delta chi <- +(1/2) delta K`. The chi
  coefficient uses `d=4`, `alpha_GP=1`, `chi_GP=1`, and
  `K_GP=div beta_GP`.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAlgebraicCouplingBlockTest.cpp`.
  The fixture verifies the four metric outputs, the `d=4` chi coefficient,
  zero output for `K`, `Theta`, `A_IJ`, and hatted-Gamma slots from this
  block, exact state-vector ordering, and incomplete-operator/eigensolver
  guards.
- Negative guards prove that the wrong metric sign, a `-1` metric
  coefficient, a `d=3` chi coefficient, reciprocal `A_IJ <- h_IJ` coupling,
  or accidental application to `K`, `Theta`, or hatted-Gamma slots would fail.
- Still missing: K/A algebraic dynamics beyond these couplings,
  Ricci/curvature, Theta/constraint terms, hatted-Gamma evolution, actual full
  operator JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold crossing, unstable/stable points,
  convergence battery, and the `k_c r0` convention map. Stage 4AO-C remains
  incomplete.

## 2026-06-23 - Historical Stage 4AO-C K A2/K2 Block (Rejected BSSN Branch)

Category: Code + Validation Planning

- Inspected GRChombo's authoritative convention in
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: the BSSN K RHS contains
  `alpha * (tr_A2 + K^2/GR_SPACEDIM)`, with `tr_A2` built from
  `A_UU = raise_all(vars.A, h_UU)` and `compute_trace(vars.A, A_UU)`.
  `TensorAlgebra::raise_all` raises both indices with the conformal inverse.
- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with only the
  K-output algebraic linearization of `A_IJ A^IJ + K^2/d` on the locked GP
  background. The block includes direct `delta A_IJ` terms, inverse-metric
  variation from `delta h_xx`, `delta h_zz`, and `delta h_ww`, the `d=4`
  K-squared coefficient, and hidden `ww` multiplicity two.
- Added `code/BlackStringToy/tests/Stage4AOCFrozenGaugeKAlgebraicBlockTest.cpp`.
  The fixture checks the coefficients
  `(-49/32)lambda^2`, `(-9/32)lambda^2`, `(-25/16)lambda^2`,
  `(-7/4)lambda`, `(-3/4)lambda`, `(+5/2)lambda`, and `(+3/4)lambda` for
  `delta h_xx`, `delta h_zz`, `delta h_ww`, `delta A_xx`, `delta A_zz`,
  `delta A_ww`, and `delta K`, respectively.
- Negative guards prove that dropping inverse-metric variation, dropping
  hidden `ww` multiplicity, using a `d=3` K-squared coefficient, adding
  spurious `h_xz`/`A_xz` couplings, or touching non-K output slots would fail.
- Still missing: Ricci/curvature and lapse-Hessian/frozen-lapse pieces,
  remaining A-equation algebraic dynamics, Theta/constraint terms,
  hatted-Gamma evolution, actual full operator JVP/parity tests, boundary
  validation, linearized MOTS map, eigensolver/shift-invert, threshold
  crossing, unstable/stable points, convergence battery, and the `k_c r0`
  convention map. Stage 4AO-C remains incomplete.

## 2026-06-24 - Stage 4AO-C A Algebraic Non-Curvature Block

Category: Code + Validation Planning

- Inspected GRChombo's authoritative convention in
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: after `Adot_TF`, the
  `A_ij` RHS contains
  `A_ij*(alpha*(K-2Theta) - (2/GR_SPACEDIM)*divshift)`, shift stretching, and
  `-2*alpha*h_UU[k][l]*A[i][k]*A[l][j]`.
- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with only
  the A-output non-curvature algebraic linearization of
  `(K - 2Theta)A_IJ - 2 h^KL A_IK A_LJ` on the locked GP background. The
  shift-stretching and `-(2/d)A_IJ div beta` terms remain owned by the
  existing tensor shift-stretching block; the trace-free Ricci/lapse-Hessian
  curvature block remains missing.
- The implemented coefficients are:
  `A_xx <- (49/32)lambda^2 delta h_xx + 5lambda delta A_xx
  -(7/8)lambda delta K +(7/4)lambda delta Theta`,
  `A_xz <- (21/32)lambda^2 delta h_xz + 4lambda delta A_xz`,
  `A_zz <- (9/32)lambda^2 delta h_zz + 3lambda delta A_zz
  -(3/8)lambda delta K +(3/4)lambda delta Theta`, and
  `A_ww <- (25/32)lambda^2 delta h_ww - lambda delta A_ww
  +(5/8)lambda delta K -(5/4)lambda delta Theta`.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAAlgebraicBlockTest.cpp`.
  The fixture checks every coefficient by basis vector, the combined
  pointwise and grid paths, zero `A_xz` K/Theta coupling, no hidden
  multiplicity on the componentwise `A_ww` equation, and unchanged
  incomplete-operator/eigensolver guards.
- Negative guards prove that dropping inverse-metric variation, adding an
  incorrect hidden-multiplicity factor to `A_ww`, using wrong K/Theta
  coefficients, adding spurious `A_xz` K/Theta couplings, or touching non-A
  outputs would fail.
- Still missing: trace-free Ricci/lapse-Hessian curvature block for `A_IJ`,
  Ricci/curvature and lapse-Hessian/frozen-lapse pieces in other equations,
  Theta/constraint terms, hatted-Gamma evolution, actual full operator
  JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold crossing, unstable/stable points,
  convergence battery, and the `k_c r0` convention map. Stage 4AO-C remains
  incomplete.

## 2026-06-24 - Stage 4AO-C Theta Algebraic Non-Ricci Block

Category: Code + Validation Planning

- Inspected GRChombo's authoritative convention in
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: in the CCZ4 branch,
  `rhs.Theta` contains
  `0.5*lapse*(ricci.scalar - tr_A2 + ((GR_SPACEDIM-1)/GR_SPACEDIM)*K^2
  - 2*Theta*K)`, followed by separate damping, `Z dot grad lapse`, and
  cosmological terms.
- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with only
  the Theta-output non-Ricci algebraic linearization of
  `0.5*(((d-1)/d)K^2 - A_IJ A^IJ)` on the locked GP background. Ricci scalar,
  `-Theta K`, Z4 damping/constraint terms, `Z dot grad alpha`, and
  cosmological terms remain outside this substep.
- The implemented coefficients are
  `(+49/64)lambda^2 delta h_xx`,
  `(+9/64)lambda^2 delta h_zz`,
  `(+25/32)lambda^2 delta h_ww`,
  `(+7/8)lambda delta A_xx`,
  `(+3/8)lambda delta A_zz`,
  `(-5/4)lambda delta A_ww`, and
  `(+9/8)lambda delta K`.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaAlgebraicBlockTest.cpp`.
  The fixture checks every coefficient by basis vector, the combined
  pointwise and grid paths, zero `h_xz/A_xz` contribution, hidden `ww` trace
  multiplicity two, and unchanged incomplete-operator/eigensolver guards.
- Negative guards prove that dropping inverse-metric variation, dropping
  hidden `ww` multiplicity, using the `d=3` K coefficient, adding spurious
  `h_xz/A_xz` terms, or touching non-Theta outputs would fail.
- Still missing: Ricci scalar contribution to Theta, `-Theta K`, Z4
  damping/constraint terms, trace-free Ricci/lapse-Hessian curvature block for
  `A_IJ`, Ricci/curvature and lapse-Hessian/frozen-lapse pieces in other
  equations, hatted-Gamma evolution, trace-free `delta A` subspace
  enforcement/projector for the assembled operator, actual full operator
  JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold crossing, unstable/stable points,
  convergence battery, and the `k_c r0` convention map. Stage 4AO-C remains
  incomplete.

## 2026-06-24 - Stage 4AO-C Theta Minus-K Algebraic Block

Category: Code + Validation Planning

- Inspected GRChombo's authoritative convention in
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: the Theta RHS includes
  `0.5*lapse*(... - 2*Theta*K ...)`, followed by separate damping,
  `Z_dot_d1lapse`, and cosmological terms. This substep implements only the
  frozen-GP linearization of the non-damping factor
  `0.5*(-2 Theta K)=-Theta K`.
- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with the
  Theta-output-only block
  `delta Theta_RHS <- -(3 lambda/2) deltaTheta`, using
  `Theta_GP=0`, `K_GP=3 lambda/2`, and `alpha_GP=1`. No `delta K`
  contribution is present because `Theta_GP=0`.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaMinusKBlockTest.cpp`.
  The fixture checks basis-vector coefficients, combined pointwise output,
  the grid apply path, exact state-vector ordering, unchanged incomplete
  operator/eigensolver guards, and non-aggregate package status.
- Negative guards prove that the wrong sign, a `-lambda` coefficient, a
  spurious `delta K` contribution, or touching any non-Theta output would
  fail.
- Still missing: Ricci scalar contribution to Theta, Z4 damping/constraint
  terms and `kappa1/kappa2` convention lock, `Z dot grad alpha`, trace-free
  Ricci/lapse-Hessian curvature block for `A_IJ`, hatted-Gamma evolution,
  trace-free `delta A` subspace enforcement/projector, actual full-operator
  JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold crossing, unstable/stable points,
  convergence battery, and the `k_c r0` convention map. Stage 4AO-C remains
  incomplete.

## 2026-06-24 - Stage 4AO-C Trace-Free Delta A Projector Contract

Category: Code + Validation Planning

- Inspected GRChombo's trace-free convention:
  `external/GRChombo/Source/utils/TensorAlgebra.hpp` defines
  `make_trace_free(tensor, metric, inverse_metric)` by computing the trace
  with the inverse metric and subtracting `metric[i][j] trace / GR_SPACEDIM`.
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp` applies this to the
  curvature/lapse `Adot_TF` block before adding the remaining `A_ij` RHS
  pieces. This substep locks only a validation-only 4AO-C assembly contract.
- Extended `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with
  `delta_trace_a_at_point` on the locked diagonal GP background:
  `delta_tr_A = delta A_xx + delta A_zz + 2 delta A_ww
  + (7lambda/8)delta h_xx + (3lambda/8)delta h_zz
  - (5lambda/4)delta h_ww`.
- Added a projector that preserves the 13-variable state vector, preserves
  `A_xz` and non-A variables, and subtracts `delta_tr_A/4` from
  `A_xx`, `A_zz`, and `A_ww`. Hidden `ww` multiplicity two is in the trace;
  the representative `A_ww` component receives the same component subtraction
  as `A_xx` and `A_zz`.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTraceFreeDeltaAProjectorTest.cpp`.
  The fixture checks exact trace coefficients, hidden multiplicity, zero
  `h_xz/A_xz` trace contribution, trace removal, `A_xz` preservation,
  idempotence, unchanged already trace-free data, and unchanged non-A slots.
- Negative guards prove that wrong hidden multiplicity, dropping metric
  variation terms, a `d=3`/three-component projection, or accidental non-A
  modification would fail.
- Still missing: Ricci/curvature and lapse-Hessian blocks, Theta Ricci scalar,
  Z4 damping and `kappa1/kappa2` convention lock, hatted-Gamma evolution,
  actual full-operator JVP/parity tests, boundary validation, linearized MOTS
  map, eigensolver/shift-invert, threshold/stable/unstable points,
  convergence battery, and the `k_c r0` convention map. Stage 4AO-C remains
  incomplete.

## 2026-06-24 - Stage 4AO-C Ricci/Curvature Design Preflight

Category: Documentation + Validation Planning

- Inspected GRChombo's Ricci convention in
  `external/GRChombo/Source/CCZ4/CCZ4Geometry.hpp`: `compute_ricci_Z`
  combines Gamma-form conformal Ricci, conformal-factor correction, and `Z`
  terms into lower/lower physical `R_IJ[gamma]`; `compute_ricci_Z_general`
  and `compute_ricci` provide related `Z` variants. This is the
  GRChombo-facing convention authority for future operator code.
- Inventoried repo-owned Ricci/oracle pieces. Stage 4G's
  `CartoonRicci::compute_metric_derivative_ricci` is an independent
  metric-derivative physical Ricci oracle, not the GRChombo Gamma-form RHS
  path. Stage 4AC/4AE/4AH provide reviewed hidden `R_ww` nonlinear
  split/direct helpers. Stage 4AL is an oracle/structural guide for
  trace-free curvature/lapse projection, not frozen-gauge operator code.
- Locked the frozen-gauge simplification:
  `delta alpha=0`, `alpha_GP=1`, `chi_GP=1`, so
  `delta(D_I D_J alpha)=0` and the missing A-equation curvature/lapse block
  reduces to `[delta R_IJ]^TF` with the project `d=4` hidden-multiplicity
  trace convention.
- Documented the decomposition contract
  `delta R_IJ = delta tilde R_IJ[delta h] + delta R^chi_IJ[delta chi,delta h]
  + hidden/cartoon terms`, including radial derivatives, periodic-z
  derivatives, hidden `1/x` terms, conformal-factor pieces, parity behavior,
  and trace-free projection after all components are assembled.
- Oracle plan: finite-difference the nonlinear Stage 4G physical Ricci around
  the locked GP background, with epsilon plateau, and cross-check hidden `ww`
  against Stage 4AC plus Stage 4AE/4AH where appropriate. The oracle set must
  include pure `delta h_ww`, off-diagonal `delta h_xz`, conformal-factor,
  z-dependent, and trace-free-projection cases.
- Recommended first implementation target: a narrow hidden
  `delta R_ww[gamma]` block on the frozen GP background, because the reviewed
  Stage 4W-4AF path already covers the highest-risk hidden `1/x` terms and
  gives a same-point nonlinear oracle.
- No Ricci code, eigensolver, shift-invert, threshold search, MOTS map,
  4AO-D work, or production Stage 4AR/4AS wiring was added. Stage 4AO-C
  remains incomplete.

## 2026-06-24 - Stage 4AO-C Hidden Physical Delta Rww Ricci Block

Category: Code + Validation Planning

- Added validation-only
  `code/BlackStringToy/Stage4AOFrozenGaugeHiddenRww.hpp` for the raw
  lower/lower hidden physical component `delta R_ww[gamma]` on the locked
  flat GP spatial background.
- Implemented only the raw hidden component formula. It is not visible Ricci,
  not the trace-free `A_IJ` curvature source, not the Theta Ricci scalar, not
  a complete frozen-gauge operator row, and not production RHS wiring.
- Added
  `code/BlackStringToy/tests/Stage4AOCHiddenPhysicalDeltaRwwTest.cpp`. The
  fixture validates the formula by central differencing the Stage 4G
  metric-derivative physical Ricci engine as the primary oracle and the
  reviewed Stage 4AH split path as the secondary oracle.
- Perturbation coverage includes zero input, pure `delta h_ww`, pure
  `delta chi`, z-dependent scalar hidden data, radial visible-metric data, and
  mixed radial/z off-diagonal data. The `1e-5` to `1e-6` epsilon plateau
  agrees with both nonlinear oracle paths below `1e-7`.
- Negative guards show that wrong hidden multiplicity, dropping hidden `1/x`
  structures, a wrong off-diagonal z-derivative sign, or treating the raw
  component as already trace-free would fail.
- Still missing at that point: visible Ricci components starting with one-z
  `delta R_xz`, trace-free A-curvature source, Theta Ricci scalar, Z4 damping
  and kappa convention lock, hatted-Gamma evolution, actual full-operator
  JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold/stable/unstable points, convergence
  battery, and the `k_c r0` convention map. Stage 4AO-C remained incomplete.

## 2026-06-24 - Stage 4AO-C Visible Physical Delta Rxz Ricci Block

Category: Code + Validation Planning

- Added validation-only
  `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxz.hpp` for the raw
  lower/lower visible off-diagonal component `delta R_xz[gamma]` on the
  locked flat GP spatial background.
- Implemented only the raw visible component formula
  `(partial_z delta h_xx - partial_z delta h_ww)/x
  - partial_xz delta h_ww + partial_xz delta chi`.
- There is no reviewed Stage 4AC/4AE-style split path for visible `R_xz`, so
  the Stage 4G metric-derivative physical Ricci engine is the primary and sole
  independent nonlinear oracle for this substep.
- Added
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxzTest.cpp`. The
  fixture validates by central differencing Stage 4G over an epsilon sweep and
  requires the `1e-5` to `1e-6` plateau below `1e-7`.
- Perturbation coverage includes zero input, pure `delta h_xz` as a zero
  oracle, an even-sector diagonal scalar mode, a radial/z mixed hidden mode,
  and a pure conformal-factor mixed-derivative mode. The parity projection
  gives allowed sine amplitude `6` and forbidden/allowed leakage
  `3.5851e-17`.
- Negative guards show that wrong mixed-derivative sign, dropped
  conformal-factor mixed derivative, hidden multiplicity applied to visible
  `R_xz`, spurious `h_xz` contribution, or wrong parity assignment would fail.
- Still missing at that point: visible `delta R_xx` and `delta R_zz`,
  trace-free A-curvature source, Theta Ricci scalar, Z4 damping and kappa
  convention lock, hatted-Gamma evolution, actual full-operator JVP/parity
  tests, boundary validation, linearized MOTS map, eigensolver/shift-invert,
  threshold/stable/unstable points, convergence battery, and the `k_c r0`
  convention map. Stage 4AO-C remained incomplete.

## 2026-06-24 - Stage 4AO-C Visible Physical Delta Rzz Ricci Block

Category: Code + Validation Planning

- Added validation-only
  `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRzz.hpp` for the raw
  lower/lower visible diagonal component `delta R_zz[gamma]` on the locked
  flat GP spatial background.
- Implemented only the raw visible component formula with positive
  conformal-factor terms:
  `partial_xz(delta h_xz) - 0.5 partial_zz(delta h_xx)
  - 0.5 partial_xx(delta h_zz) - partial_zz(delta h_ww)
  + 2 partial_z(delta h_xz)/x - partial_x(delta h_zz)/x
  + partial_x(delta chi)/x + 0.5 partial_xx(delta chi)
  + 1.5 partial_zz(delta chi)`.
- The positive `chi` signs follow the Stage 4G / GRChombo-facing convention
  `gamma=h/chi` with lower/lower physical Ricci components.
- Added
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRzzTest.cpp`. The
  fixture validates by central differencing the Stage 4G metric-derivative
  physical Ricci engine over an epsilon sweep and requires the `1e-5` to
  `1e-6` plateau below `1e-7`.
- Perturbation coverage includes zero input, pure `delta h_zz`, one-z
  `delta h_xz` with mixed derivative and `1/x` contribution, z-dependent
  `delta h_ww`, mixed `delta chi`, z-dependent `delta h_xx`, and mixed
  radial/z behavior. Even-parity checks give forbidden/allowed leakage
  `2.00457e-17` for scalar diagonal input and `3.85494e-17` for one-z
  `h_xz` input.
- Negative guards show that wrong `partial_xz(delta h_xz)` sign, dropping
  `2 partial_z(delta h_xz)/x`, dropping `-partial_x(delta h_zz)/x`, dropping
  `-partial_zz(delta h_ww)`, flipping the positive `chi` signs, applying
  hidden multiplicity to visible `R_zz`, wrong parity, or treating raw `R_zz`
  as already trace-free would fail.
- Still missing: visible `delta R_xx`, trace-free A-curvature source, Theta
  Ricci scalar, Z4 damping and kappa convention lock, hatted-Gamma evolution,
  actual full-operator JVP/parity tests, boundary validation, linearized MOTS
  map, eigensolver/shift-invert, threshold/stable/unstable points, convergence
  battery, and the `k_c r0` convention map. Stage 4AO-C remains incomplete.

## 2026-06-24 - Stage 4AO-C Visible Physical Delta Rxx Ricci Block

Category: Code + Validation Planning

- Added validation-only
  `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxx.hpp` for the raw
  lower/lower visible radial component `delta R_xx[gamma]` on the locked flat
  GP spatial background.
- Implemented only the raw visible component formula with positive
  conformal-factor terms:
  `partial_xz(delta h_xz) - 0.5 partial_xx(delta h_zz)
  - 0.5 partial_zz(delta h_xx) + partial_x(delta h_xx)/x
  - 2 partial_x(delta h_ww)/x - partial_xx(delta h_ww)
  + partial_x(delta chi)/x + 1.5 partial_xx(delta chi)
  + 0.5 partial_zz(delta chi)`.
- The positive `chi` signs follow the Stage 4G / GRChombo-facing convention
  `gamma=h/chi` with lower/lower physical Ricci components.
- Added
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxxTest.cpp`. The
  fixture validates by central differencing the Stage 4G metric-derivative
  physical Ricci engine over an epsilon sweep and requires the `1e-5` to
  `1e-6` plateau below `1e-7`.
- Perturbation coverage includes zero input, pure `delta h_xx`, pure
  `delta h_zz`, pure `delta h_ww`, one-z `delta h_xz`, mixed `delta chi`,
  z-dependent diagonal scalar input, and mixed radial/z behavior. Even-parity
  checks give forbidden/allowed leakage `2.00457e-17` for scalar diagonal
  input and `4.85723e-17` for one-z `h_xz` input.
- Negative guards show that wrong `partial_xz(delta h_xz)` sign, dropping a
  radial `1/x` term, dropping conformal-factor terms, flipping the positive
  `chi` signs, applying hidden multiplicity to visible `R_xx`, wrong parity,
  or treating raw `R_xx` as already trace-free would fail.
- Still missing: raw Ricci trace/trace-free assembly, trace-free A-curvature
  source, Theta Ricci scalar, Z4 damping and kappa convention lock,
  hatted-Gamma evolution, actual full-operator JVP/parity tests, boundary
  validation, linearized MOTS map, eigensolver/shift-invert,
  threshold/stable/unstable points, convergence battery, and the `k_c r0`
  convention map. Stage 4AO-C remains incomplete.

## 2026-06-24 - Stage 4AO-C Ricci Trace And Trace-Free Assembly

Category: Code + Validation Planning

- Added validation-only
  `code/BlackStringToy/Stage4AOFrozenGaugeRicciAssembly.hpp` for assembling
  the raw Ricci scalar trace and trace-free Ricci tensor from the validated
  raw component result types. The helper consumes raw `delta R_xx`,
  `delta R_xz`, `delta R_zz`, and `delta R_ww` packages rather than loose raw
  doubles.
- Implemented only the frozen-GP background assembly
  `delta R = delta R_xx + delta R_zz + 2 delta R_ww`, with hidden
  multiplicity two and no `delta R_xz` scalar-trace contribution, followed by
  the `d=4` projection
  `R_xx^TF=R_xx-delta R/4`, `R_zz^TF=R_zz-delta R/4`,
  `R_ww^TF=R_ww-delta R/4`, and `R_xz^TF=R_xz`.
- Added
  `code/BlackStringToy/tests/Stage4AOCRicciTraceFreeAssemblyTest.cpp`. The
  fixture central-differences the Stage 4G nonlinear physical Ricci engine,
  independently forms the oracle trace/free components, and requires the
  `1e-5` to `1e-6` epsilon plateau below `2e-7`.
- Perturbation coverage includes zero input, pure `delta h_xx`, pure
  `delta h_zz`, pure `delta h_ww`, pure `delta h_xz`, pure `delta chi`, and a
  mixed radial/z scalar mode. Parity checks confirm even scalar trace output
  and one-z `R_xz^TF` behavior with forbidden/allowed leakage near roundoff.
- Negative guards show that dropping the hidden factor two, adding `R_xz` to
  the scalar trace, using a `d=3` projection, projecting before full raw
  assembly, or treating the block as an A-curvature/Theta insertion would
  fail.
- Still missing: insertion of `[delta R_IJ]^TF` into the `A_IJ` curvature
  source, insertion of `0.5 delta R` into the Theta Ricci scalar term, Z4
  damping and kappa convention lock, hatted-Gamma evolution, actual
  full-operator JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold/stable/unstable points, convergence
  battery, and the `k_c r0` convention map. Stage 4AO-C remains incomplete.

## 2026-06-24 - Stage 4AO-C Theta Ricci Scalar Insertion

Category: Code + Validation Planning

- Extended the validation-only frozen-gauge operator wrapper with the Theta
  Ricci scalar insertion block
  `output[Theta] += 0.5 delta R`.
- The block consumes the reviewed `TraceFreeRicciAssembly` result from
  `Stage4AOFrozenGaugeRicciAssembly.hpp` and reads `scalar_trace()`, so hidden
  `ww` multiplicity two and the absence of `R_xz` from the scalar trace remain
  owned by the raw Ricci trace assembly.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaRicciScalarBlockTest.cpp`.
  The fixture verifies pointwise and grid apply paths, positive sign, factor
  `0.5`, inherited hidden multiplicity, no `R_xz` trace contribution, no
  non-Theta output writes, and incomplete-operator/eigensolver guards.
- Updated
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp` so
  the RHS-piece inventory now marks only the Theta Ricci scalar insertion
  piece as implemented for the Theta output.
- Still missing: insertion of `[delta R_IJ]^TF` into the `A_IJ` curvature
  source, Z4 damping and kappa convention lock, hatted-Gamma evolution, actual
  full-operator JVP/parity tests, boundary validation, linearized MOTS map,
  eigensolver/shift-invert, threshold/stable/unstable points, convergence
  battery, and the `k_c r0` convention map. Stage 4AO-C remains incomplete.

## 2026-06-24 - Stage 4AO-C A Ricci Curvature Insertion

Category: Code + Validation Planning

- Extended the validation-only frozen-gauge operator wrapper with the
  `A_IJ` Ricci curvature insertion block
  `output[A_IJ] += [delta R_IJ]^TF`.
- The block consumes the reviewed `TraceFreeRicciAssembly` result from
  `Stage4AOFrozenGaugeRicciAssembly.hpp`, so the scalar trace, `d=4`
  projection, hidden `ww` trace multiplicity, and `R_xz` off-diagonal handling
  remain owned by the Ricci assembly helper.
- The frozen-gauge simplification follows the inspected GRChombo convention:
  `Adot_TF = -D_I D_J alpha + chi * alpha * R_IJ`, followed by
  `make_trace_free`. With `delta alpha=0`, `alpha_GP=1`, `chi_GP=1`,
  `R_IJ^GP=0`, and `partial_I alpha_GP=0`, the insertion is exactly
  `[delta R_IJ]^TF` with prefactor one.
- Added
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`.
  The fixture verifies pointwise and grid apply paths, no non-A output writes,
  `A_xz` equal to raw `R_xz`, trace-free source residual
  `A_xx + A_zz + 2 A_ww = 0`, no extra hidden multiplicity on representative
  `A_ww`, and incomplete-operator/eigensolver guards.
- Updated
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp` so
  the RHS-piece inventory marks the A Ricci curvature insertion implemented
  only for the `A_IJ` outputs.
- Still missing: Z4 damping and kappa convention lock, hatted-Gamma evolution,
  actual full-operator JVP/parity tests, boundary validation, linearized MOTS
  map, eigensolver/shift-invert, threshold/stable/unstable points, convergence
  battery, and the `k_c r0` convention map. Stage 4AO-C remains incomplete.

## 2026-06-25 - Stage 4AO-C Z4 Damping/Kappa Convention Lock Pass

Category: Documentation / Validation Planning

- Documented the inspected GRChombo Z4 damping convention in
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`.
- Source authority: `CCZ4_base_params_t` defines `kappa1`, `kappa2`,
  `kappa3`, and `covariantZ4`; `SimulationParametersBase` reads defaults
  `kappa1=0.1`, `kappa2=0`, `kappa3=1`, `covariantZ4=true`; the Stage 2 smoke
  params repeat those values but do not lock the GL validation convention.
- Frozen-GP targets recorded: Theta damping
  `-0.5 kappa1 (5+3 kappa2) deltaTheta`, K damping
  `-4 kappa1 (1+kappa2) deltaTheta`, and hatted-Gamma Z damping
  `(K_GP(kappa3-1)-2kappa1) delta Z_over_chi^i`.
- The hatted-Gamma target acts on
  `delta Z_over_chi^i=0.5(delta hat_Gamma^i-delta tilde_Gamma^i)`, so it
  depends on the Stage 4AM/4AN hatted convention and the future full
  hatted-Gamma evolution block.
- No kappa values were chosen and no code/tests were added. The human
  `kappa1/kappa2` convention choice and Z4 damping implementation remain
  blockers before a complete 4AO-C operator or spectrum.

## 2026-07-20 - Stage 4AO-C Selected-CCZ4 K Equation Correction

Category: Code + Validation + Documentation Reconciliation

- Superseded the 2026-06-23 K block after the committed-range audit showed
  that `A_IJ A^IJ + K^2/d` is GRChombo's `USE_BSSN` branch. Its direct
  `delta A_IJ`, inverse-metric, and `K^2/d` coefficients were removed from
  the Stage 4AO-C K row.
- Implemented the locked `USE_CCZ4` non-damping pieces only:
  `output[K] += 3 lambda input[K] - 3 lambda input[Theta]` and, through the
  validated Ricci trace assembly,
  `output[K] += delta R_xx + delta R_zz + 2 delta R_ww`.
- Replaced the old K fixture with
  `Stage4AOCFrozenGaugeKCCZ4BlockTest.cpp`. Its independent oracle
  central-differences nonlinear Stage 4G physical Ricci plus
  `K(K-2Theta)`, perturbs K and Theta independently, requires an epsilon
  plateau, and tests pure K, pure Theta, pure A, metric/chi, pure h_xz, and
  combined perturbations against both selected-CCZ4 and rejected-BSSN
  nonlinear branch oracles.
- Compiled and ran all 16 current `Stage4AOC*.cpp` fixtures with the locked
  C++17 warning flags; all passed. `git diff --check` passed, with no
  `external/GRChombo` or `scripts` diffs.
- Reconciled the Stage 4 summary, 4AO-C gate, hidden-sphere roadmap, fixture
  inventory, checklist, TODO, and logs. Theta Ricci and A trace-free Ricci
  insertions remain recorded as completed validation pieces.
- Still missing: Z/hat-Gamma-dependent Ricci, kappa damping and convention
  choice, hat-Gamma evolution, remaining coupled RHS pieces, actual-operator
  JVP/parity, boundary validation, linearized MOTS, eigensolver/shift-invert,
  threshold/stable/unstable points, convergence, and the `k_c r0` map.
  Frozen-gauge lapse-Hessian variation vanishes and locked `Lambda=0` leaves
  no cosmological K term. Stage 4AO-C remains incomplete.

## 2026-07-21 - Stage 4AO-C Main-Path CCZ4 K/Theta Damping Insertion

Category: Planning Decision + Code + Validation

- Began from clean commit `6c0b306`, which contains the audited selected-CCZ4
  K correction.
- Recorded the planning-layer decision to deliberately adopt the inherited
  GRChombo-facing main-path values `kappa1=0.1`, `kappa2=0`, `kappa3=1`, and
  `covariantZ4=true`. Codex implemented that decision without tuning the
  parameters. Zero damping is reserved for a later diagnostic comparison.
  `code/BlackStringToy/params_stage2_smoke.txt` was not changed and is not the
  convention authority.
- Added the validation-only simple damping insertion
  `output[Theta] += -0.5*kappa1*(5+3*kappa2)*input[Theta] = -0.25 input[Theta]`
  and `output[K] += -4*kappa1*(1+kappa2)*input[Theta] = -0.4 input[Theta]`.
  No `deltaK` input or other output participates. The corrected K pieces
  `+delta R + 3 lambda deltaK - 3 lambda deltaTheta` remain unchanged.
- Kept hatted-Gamma damping deferred. For `kappa3=1`, the future target is
  `delta rhs_hat_Gamma^i|Z/kappa = -0.2 delta Z_over_chi^i`; the separate
  `-(chris.contracted + 2 kappa3 Z_over_chi) * d1.shift` term belongs to the
  future complete hatted-Gamma evolution block.
- Added `Stage4AOCFrozenGaugeCCZ4DampingInsertionTest.cpp`, deriving expected
  coefficients independently from `kappa1`, `kappa2`, and `d`. The initial
  nonzero sign sentinels were replaced after audit by genuine mutation guards:
  actual coefficients must match independently derived negative damping and
  remain meaningfully separated from the corresponding positive-sign
  antidamping mutations. Further guards cover `d=3`, wrong nonzero-`kappa2`
  dependence, spurious `deltaK`, spurious hatted-Gamma writes from the
  K/Theta insertion, non-K/Theta writes, and false completion claims.
- Validation: the focused damping and operator-contract tests passed; all 17
  `Stage4AOC*.cpp` fixtures compiled with the required C++17 warning flags and
  passed. Protected-path diffs remained empty.
- Stage 4AO-C remains incomplete. Complete hatted-Gamma evolution, remaining
  coupled CCZ4/Z4 pieces, assembled-operator JVP/parity, boundaries and
  convergence, MOTS, eigensolver/shift-invert, stable/unstable threshold
  points, the full convergence battery, and the primary-source `k_c r0` map
  remain blockers. Stage 4AO-D is unstarted and Checkpoint G has not passed.

## 2026-07-21 - Stage 4AO-C Hatted-Gamma Evolution Design Preflight

Category: Documentation + Physics Design

- Began from clean damping checkpoint `62e5e9e`; used CodeGraph first and
  inspected the selected `USE_CCZ4` `CCZ4RHS::rhs.Gamma[i]` source read-only.
- Derived the complete frozen-GP `x` and `z` linearized term inventories,
  including the existing common advection ownership, metric-dependent
  background shift Hessians, K/Theta/chi gradients, connection-A terms with
  two hidden `ww` copies, `kappa3=1` shift-gradient coupling, and locked
  `-0.2 delta Z_over_chi^i` damping.
- Locked `delta Z_over_chi^i=0.5(delta hat_Gamma^i-delta tilde_Gamma^i)`.
  Reused the 4AM/4AN full `x` convention and derived the still-missing `z`
  companion, including `2 delta h_xz/x-partial_z delta h_ww` from the two
  hidden contractions.
- Recorded that the exact selected GRChombo momentum-constraint form contains
  no `d1.A`: derivatives of `delta A_IJ` are absent, and direct `delta A_IJ`
  also vanishes on the flat conformal GP background. The surviving A-sector
  dependence is `A_GP^IJ delta Gamma^i_IJ`.
- Locked `hat_Gamma^x` to the scalar/even sector and `hat_Gamma^z` to the
  one-z/opposite-parity sector. Defined nonlinear visible/4AN finite-difference
  and independent analytic hidden-block oracles with epsilon plateau,
  multiplicity mutations, parity leakage, and zero-background-residual gates.
- Recommended the first narrow implementation as a validation-only x/z
  contracted-connection and Z reconstruction helper, followed by a separate
  non-advection Z/kappa plus `kappa3` shift-gradient RHS block. No Gamma RHS,
  eigensolver, JVP assembly, boundary, MOTS, 4AO-D, or production code was
  added. Stage 4AO-C and Checkpoint G remain incomplete.

## 2026-07-21 - Stage 4AO-C Contracted Connection And Z Reconstruction

Category: Validation Helper + Tests + Documentation

- Added `Stage4AOFrozenGaugeContractedConnection.hpp`, returning only `g_x`,
  full hidden-aware `g_z`, and `Z_i=0.5(H_i-g_i)`. Hidden multiplicity two is
  explicit, and no determinant constraint is assumed.
- Added `Stage4AOCFrozenGaugeContractedConnectionTest.cpp`. It independently
  central-differences the Stage 4AN nonlinear x contraction and a test-only
  nonlinear full z contraction. The `1e-5` results differ from the analytic
  helper by about `4.1e-11` for x and `5.8e-12` for z, with stable middle
  plateaus.
- Pure `h_ww`, `h_xz`, diagonal metric, nonzero hatted-Gamma/Z, zero, and
  Fourier parity cases pass. Negative guards reject hidden multiplicity one,
  wrong derivative signs, and use of the determinant-reduced identity without
  trace certification.
- Updated the operator inventory to mark both x and z contracted-connection
  paths as reusable helpers only. Gamma damping, connection-A,
  vector-Hessian, K/Theta/chi gradients, full Gamma evolution, all completion
  gates, eigensolver access, and production wiring remain false/missing.
- Validation: the focused helper, affected operator-contract, and Stage 4AN
  full-x tests passed with the required warning flags; all 18 current
  `Stage4AOC*.cpp` fixtures compiled and passed without warnings.
  `git diff --check` passed, and protected-path plus Stage 2 smoke-parameter
  diffs remained empty.

## 2026-07-21 - Stage 4AO-C First Hatted-Gamma RHS Block

Category: Validation Operator + Tests + Documentation

- Used CodeGraph first and recovered the current uncommitted 4AO-C
  contracted-connection patch without rewriting it; no unrelated worktree
  changes were present.
- Added only the non-advection partial rows
  `delta rhs_Hx += (3 lambda/4)g_x - 0.2 Z_x + (lambda/2)H_x` and
  `delta rhs_Hz += (3 lambda/4)g_z - 0.2 Z_z`, consuming the validated
  `g_i,Z_i` helper. The common GP-shift advection block remains unchanged and
  is not duplicated.
- Added `Stage4AOCFrozenGaugeHatGammaZ4KappaBlockTest.cpp`. Its oracle derives
  `2K0/d`, `(4K0/d)(kappa3-1)-2kappa1`, and the x shift-gradient coefficient
  independently from `K0=3lambda/2`, `d=4`, `kappa1=0.1`, and `kappa3=1`.
  Pure H, pure metric/Z, parity, and output-scope cases pass; mutation guards
  reject an extra `lambda H_z/2`, positive damping sign, hidden multiplicity
  one, and duplicate advection.
- The first Gamma partial-block inventory row is implemented, but both Gamma
  variable RHS-complete flags, the full-operator flag, and eigensolver access
  remain false. Connection-A, vector/shift-Hessian, K/Theta/chi-gradient,
  remaining coupled terms, and complete row assembly are still missing.
- Validation: the focused Gamma test, operator contract, and contracted-
  connection test passed; all 19 current `Stage4AOC*.cpp` fixtures compiled
  without warnings and passed. `git diff --check` passed, and diffs under
  `external/GRChombo`, `scripts`, and `params_stage2_smoke.txt` were empty.

## 2026-07-21 - Stage 4AO-C Hatted-Gamma Gradient Block

Category: Validation Operator + Tests + Documentation

- Began from the clean committed Gamma checkpoint and used CodeGraph before
  editing.
- Added only the two hatted-Gamma K/Theta/chi-gradient rows:
  `2 dx(deltaTheta) - (3/2)dx(deltaK) - 27 lambda delta h_xx/(8x) +
  (7 lambda/2)dx(delta chi)` and the z companion with `h_xz` and
  `(3 lambda/2)dz(delta chi)`. The prior connection/Z block and common
  advection remain separate and unchanged.
- Added `Stage4AOCFrozenGaugeHatGammaGradientBlockTest.cpp`. Its oracle derives
  `-2(d-1)/d`, varies `h^{ij} partial_j K_GP` using
  `partial_x K_GP=-9 lambda/(4x)`, and derives the chi coefficients from the
  d=4 GP A background. Pure-gradient, metric-variation, parity, scope, and
  non-duplication cases pass; wrong signs, omitted metric variation, swapped
  chi coefficients, and forbidden `h_zz/h_ww` writes are rejected.
- Validation: the focused test, operator contract, prior Gamma block, and
  contracted-connection helper passed. All 20 current `Stage4AOC*.cpp`
  fixtures compiled without warnings and passed. Complete Gamma evolution,
  the full operator, and eigensolver access remain false.

## 2026-07-21 - Stage 4AO-C Hatted-Gamma Connection-A Block

Category: Validation Operator + Tests + Documentation

- Began from the clean committed Gamma-gradient checkpoint and used CodeGraph
  before editing.
- Added only `output[hat_Gamma^x] += C_x` and
  `output[hat_Gamma^z] += C_z`, with
  `C_i=2 A_GP^{IJ} delta Gamma^i_IJ`. The two hidden `ww` copies are
  explicit. No direct `delta A_IJ`, `d1.A`, common advection, Z/kappa,
  scalar-gradient, vector/shift-Hessian, or remaining shift contribution was
  added.
- Added `Stage4AOCFrozenGaugeHatGammaConnectionABlockTest.cpp`. Its analytic
  oracle reconstructs the visible and hidden linearized conformal
  Christoffels before contracting with `A_GP^xx=-7 lambda/8`,
  `A_GP^zz=-3 lambda/8`, and `A_GP^ww=5 lambda/8`. Visible, hidden,
  diagonal, parity, output-scope, and non-duplication cases pass; mutations
  reject hidden multiplicity one and wrong derivative or `1/x` signs.
- Pure `delta A_IJ` and test-only `d1.A` sentinels give zero direct output,
  as required by the vanishing GP conformal Christoffels and selected
  momentum-constraint form. Complete Gamma evolution, the full operator, and
  eigensolver access remain false.
- Validation: the focused connection-A test, operator contract, prior Gamma
  gradient and Z/kappa tests, and contracted-connection helper passed. All 21
  current `Stage4AOC*.cpp` fixtures compiled without warnings and passed;
  `git diff --check` passed and protected-path diffs remained empty.

## 2026-07-21 - Stage 4AO-C Hatted-Gamma Shift Metric Blocks

Category: Validation Operator + Tests + Documentation

- Began from the clean committed connection-A checkpoint and used CodeGraph
  before editing.
- Added two separately owned families from the selected GRChombo Gamma row.
  Vector Hessian adds `-3 lambda h_xx/(4x)+3 lambda h_ww/x` only to the x
  output and zero to z; both hidden `ww` directions are explicit in the
  derivation. Grad-div separately adds `9 lambda h_xx/(8x)` to x and
  `9 lambda h_xz/(8x)` to z.
- Added `Stage4AOCFrozenGaugeHatGammaShiftMetricBlockTest.cpp`. Its independent
  oracles derive the visible/hidden GP shift Hessians,
  `partial_x(div beta_GP)`, and `(d-2)/d`. Pure metric, parity, output-scope,
  and non-duplication cases pass; mutations reject one hidden copy,
  omitted/wrong-sign `h_ww`, a spurious z vector Hessian, and incorrect
  grad-div coefficient/sign.
- Added a Gamma term-closure ledger: no selected mathematical Gamma RHS family
  remains missing in the locked frozen-gauge specialization. Final one-time
  row assembly, background-zero and combined oracle/epsilon validation,
  assembled parity/ownership checks, and actual full-operator JVP integration
  remain missing, so all Gamma/full-operator/eigensolver gates stay false.
- Validation: the focused shift-metric test, operator contract, and all prior
  Gamma/helper tests passed. All 22 current `Stage4AOC*.cpp` fixtures compiled
  without warnings and passed.

## 2026-07-21 - Stage 4AO-C Complete Frozen-Gauge Hatted-Gamma Rows

Category: Validation Operator + Tests + Documentation

- Began from the clean committed shift-metric checkpoint and used CodeGraph
  before editing.
- Added one complete-row assembler that takes the existing common GP-advection
  output as a dependency, then calls the Z/kappa/shift-gradient,
  K/Theta/chi-gradient, connection-A, vector-Hessian, and grad-div partial
  functions exactly once. The individual family functions remain intact and
  separately testable.
- Added `Stage4AOCFrozenGaugeCompleteHatGammaRowsTest.cpp`. Its independent
  test-only nonlinear oracle constructs the visible inverse conformal metric,
  visible and modified-cartoon hidden Christoffels, contracted connection/Z,
  raised A tensor, and GP shift derivatives before evaluating the selected
  CCZ4 Gamma equation. It does not call the production partial-Gamma
  functions or copy their final reduced coefficients.
- The exact nonlinear GP residual is `x=-2.775557561563e-17`, `z=0`.
  Directed-family cases and two mixed perturbations pass the
  `1e-2,1e-4,1e-5,1e-6,1e-7` sweep with a stable `1e-5/1e-6` plateau.
  Genuinely isolated vector-Hessian x and grad-div z fixtures separately
  prove every other family is zero before checking `3 lambda a/x` and
  `9 lambda a/(8x)`, respectively.
  Omission and duplication mutations for every family, duplicate advection,
  forbidden `lambda H_z/2`, hidden multiplicity one, parity leakage, and
  non-Gamma writes are rejected.
- Opened only the complete Gamma family/assembly/validation flags and
  `variable_rhs_complete(hat_Gamma^x/z)`. Every other variable remains
  incomplete; the complete 13-variable operator and eigensolver gates remain
  false. Generic Theta/Z constraint, hidden-sphere, and hidden-evolution
  inventory entries are now `implemented_now` only for the Gamma rows.
- Validation: focused assembled-row, all individual Gamma/helper, and operator
  contract tests passed; all 23 `Stage4AOC*.cpp` fixtures compiled without
  warnings and passed. Protected paths remained untouched and nothing was
  staged or committed.

## 2026-07-22 - Stage 4AO-C Non-Gamma RHS Closure Preflight

Category: Documentation + Physics Design

- Began from clean committed complete-Gamma checkpoint `8cec8c0` and used
  CodeGraph before direct source/doc inspection.
- Traced the selected `USE_CCZ4` branches in `CCZ4RHS::rhs_equation` and the
  exact `CCZ4Geometry::compute_ricci_Z` path. The validated raw Stage 4AO-C
  Ricci blocks own geometric `delta R^g_IJ` only; they do not own the encoded-Z
  completion `q_IJ=2D_(I delta Z_J)`.
- Closed the K/Theta/A mathematical inventory without implementation. The exact
  missing terms are `+q` in K, `+q/2` in Theta, and `[q_IJ]^TF` in every A
  component, with `q_ww=2Z_x/x` and
  `q=q_xx+q_zz+2q_ww`. All remaining selected terms are already validated,
  supplied by existing helpers, implemented but unassembled, or proven zero in
  frozen gauge.
- Recorded full unsimplified row targets, ownership/duplication risks, hidden
  multiplicity, trace/projection boundaries, nonlinear and analytic oracle
  plans, parity/plateau/mutation requirements, and unresolved adapter and
  enforcement conventions.
- Recommended the first future code block as a validation-only shared
  encoded-Z Ricci-completion tensor taking a Z derivative jet and returning
  component, scalar, and trace-free outputs. No RHS code or tests were added.
  Every non-Gamma completion flag, the complete-operator gate, and eigensolver
  gate remain false; boundaries, MOTS, 4AO-D, and spectral work were untouched.

## 2026-07-22 - Stage 4AO-C Encoded-Z Ricci-Completion Tensor Helper

Category: Validation Helper + Tests + Documentation

- Preserved the uncommitted non-Gamma preflight documentation and used
  CodeGraph before direct inspection or editing.
- Added a validation-only helper that consumes an already formed lower-Z
  derivative jet and computes `q_xx=2dx z_x`,
  `q_xz=dx z_z+dz z_x`, `q_zz=2dz z_z`, and the single representative
  `q_ww=2z_x/x`. Its scalar trace alone uses the two hidden copies,
  `q=q_xx+q_zz+2q_ww`, before the four-dimensional trace-free projection.
- The helper deliberately contains no geometric Ricci, no expansion of
  `Z_i=0.5(H_i-g_i)`, no common advection, and no K/Theta/A insertion.
  Operator guards explicitly leave all three non-Gamma Z-Ricci insertion
  families, full-operator completion, and eigensolver access false.
- Added an independent analytic spherical modified-cartoon oracle. It checks
  all raw/trace/trace-free outputs, representative-versus-trace hidden
  multiplicity, weighted trace-free zero, even/one-z parity, zero Z jet, and
  mutations for hidden count, doubled representative ww, 1/x sign and
  coefficient, omitted projection, and double projection.
- Deferred the nonlinear selected-branch finite-difference oracle until
  complete K/A/Theta row assembly, where the upper/lower-Z and conformal-factor
  mapping can be validated once. The next insertion targets remain
  `K+=q`, `Theta+=q/2`, and `A_IJ+=qTF_IJ`.
- Validation: the focused helper, operator contract, Ricci/Z-adjacent K,
  Theta, A, contracted-connection, and trace-free fixtures passed. All 24
  `Stage4AOC*.cpp` tests compiled with `-Wall -Wextra -pedantic` and passed.

## 2026-07-22 - Stage 4AO-C Encoded-Z Ricci Row Insertions

Category: Validation Operator + Tests + Documentation

- Began from clean committed encoded-Z helper checkpoint `903455c` and used
  CodeGraph before direct inspection or editing.
- Added separate partial point/grid insertion blocks that consume only the
  opaque encoded-Z completion: K receives `+q`, Theta receives `+q/2`, and
  `A_xx/A_xz/A_zz/A_ww` receive the already projected `qTF_IJ` values.
- The insertion interfaces cannot accept the geometric Ricci assembly. They
  do not recompute `q_IJ`, apply a second projection, multiply representative
  `ww` by the hidden count, or write outside their owned slots. Existing
  Ricci, curvature, algebraic, damping, advection, and shift blocks are
  unchanged and unassembled with these new pieces.
- Added a focused insertion fixture covering isolated K/Theta/A ownership,
  all coefficients, weighted trace-free A output, grid application and size
  guards, scalar/even and xz/one-z parity, and mutations for wrong scalar
  weights, raw q, double projection, duplicate geometric Ricci, representative
  ww doubling, and output leakage.
- The operator inventory now marks only the encoded-Z insertion piece and its
  K/Theta/A applicability implemented. Every non-Gamma variable-completion
  flag, complete-operator gate, and eigensolver gate remains false.
- Validation: the focused insertion fixture, encoded-Z helper, operator
  contract, and affected K/Theta/A fixtures passed. All 25
  `Stage4AOC*.cpp` tests compiled with `-Wall -Wextra -pedantic` and passed.

## 2026-07-22 - Stage 4AO-C Complete Frozen-Gauge K/Theta/A Rows

Category: Validation Operator + Independent Nonlinear Oracle + Documentation

- Began from clean committed encoded-Z insertion checkpoint `e819381` and
  used CodeGraph before direct source inspection or editing.
- Added the analytic hidden-aware Z-derivative adapter. It retains all four
  derivatives of the unreduced contracted connection, including the radial
  `1/x^2` terms, hidden multiplicity two, mixed derivatives, and no conformal
  determinant assumption. Production code is not numerically differentiated.
- Added a one-time K/Theta/A assembler that consumes common advection,
  geometric Ricci, encoded-Z completion, selected algebraic terms, locked
  K/Theta damping, and A tensor shift stretching exactly once. It performs no
  second trace-free projection, writes representative `A_ww` once, and adds no
  direct A damping.
- Repaired the test-only nonlinear selected-`USE_CCZ4` split-path oracle to
  use exact analytic value/first/second jets at each nonlinear state before a
  full four-dimensional Cartesian SO(3) lift. It independently constructs physical
  Ricci, contracted conformal connection, lower Z and its covariant
  derivatives, curvature projection, algebraic terms, GP advection, and shift
  stretching without calling production partial-row functions or using
  internal spatial differences. This validates geometric Ricci plus encoded Z;
  source mapping identifies that split with selected CCZ4, but the fixture is
  not a second direct Gamma-form Ricci implementation.
- Analytic-jet GP residuals are zero except `Theta=-6.78e-21`. Both mixed
  directions pass the six-epsilon sweep `1e-2` through `1e-7`: maximum errors
  decrease by approximately 100 at each of the first two decades and then
  saturate from roundoff. No epsilon plateau is claimed.
- A genuinely isolated nonzero geometric-Ricci direction enforces
  `H=g`, `partial H=partial g`; all encoded-Z and unrelated families vanish.
  The full metric-inclusive tangent case enforces both `T=0` and
  `partial_x T=0`, has a nonzero metric RHS contribution, and gives
  `T(delta rhs)=-2.78e-17` without completing the metric rows. Curvature weighted traces,
  parity, every-family omission/duplication, hidden multiplicity, encoded-Z
  omission/doubling, representative-ww duplication, and illegal writes pass.
- The adapter fixture independently guards representative mixed and second
  derivatives in all four derivative rows, both inverse-square families, and
  hidden multiplicity with active-term mutations.
- K, Theta, and all four A completion/inventory/validation flags are true.
  Chi and metric rows, the complete operator/JVP gate, boundaries, MOTS,
  eigensolver, and 4AO-D remain false. All 27 Stage 4AO-C fixtures compile with
  warnings promoted to errors and pass. `git diff --check`, protected paths,
  smoke parameters, and the empty staged diff pass; no staging or commit was
  performed.

## 2026-07-22 - Stage 4AO-C Complete 13-Variable Frozen-Gauge Interior Operator

Category: Validation Operator + Independent Analytic-Jet Oracles + Documentation

- Began from clean committed complete-K/Theta/A checkpoint `b0ef512` and used
  CodeGraph before direct source inspection.
- Completed chi and all four conformal-metric rows by composing common GP
  advection, existing tensor shift stretching, and existing algebraic
  coupling once. The locked background identity `K_GP=div beta_GP` removes
  the apparent delta-chi coefficient. Representative hww remains one output
  component.
- Added a locked-order full interior assembler that composes the independently
  validated chi/metric, K/Theta/A, and hatted-Gamma owners. It receives common
  advection and never reimplements it or any row family.
- Added independent analytic-jet nonlinear selected-branch evaluators. The
  full oracle constructs all four Cartesian spatial directions, physical
  Ricci, encoded Z and covariant derivatives, trace-free projections,
  selected algebraic/damping terms, GP advection, tensor shift stretching,
  and hatted-Gamma shift derivatives without calling production row
  functions or copying final reduced coefficients.
- All 13 GP residuals are zero or roundoff (`max=5.42e-20`). Two mixed
  directions activate every row and major family over `epsilon=1e-2` through
  `1e-7`. Nonlinear rows converge at second order through `1e-4` before
  roundoff saturation; the exactly linear chi/metric straight-path rows stay
  at roundoff. Owner/advection omission and duplication, hidden multiplicity,
  representative hww/Aww, output-slot, weighted tangent, determinant, and
  parity mutations pass. The two explicit parity blocks have zero leakage.
- All 13 variable completion flags and the validation-only interior
  assembly/JVP/parity flags are true. Radial boundaries, the boundary-bearing
  complete operator, MOTS, eigensolver/shift-invert, threshold/convergence
  results, Stage 4AO-D, Checkpoint G, and production wiring remain incomplete.
- All 29 `Stage4AOC*.cpp` fixtures compile with warnings promoted to errors and
  pass. `git diff --check`, protected paths, smoke parameters, and the empty
  staged diff pass; no staging or commit was performed.

## 2026-07-22 - Stage 4AO-C Radial-Boundary Design Preflight

Category: Boundary Physics + Discrete Contract + Documentation

- Began from clean committed full-interior checkpoint `554de1c` and used
  CodeGraph before direct source inspection.
- Derived the selected frozen-CCZ4 radial principal blocks from the GRChombo
  source. Physical and constraint light speeds are `-beta^x +/- 1`, and the
  zero-normal-speed sectors are advected at `-beta^x`. The frozen longitudinal
  sector has a Jordan block, so the contract uses an explicit blockwise
  substitute rather than claiming a complete characteristic basis.
- For every locked `0<x_in<r0`, all sectors are pure outflow through the inner
  boundary. The contract imposes no continuum inner data, retains all 13 PDE
  rows with second-order one-sided closure, and permits only algebraic
  determinant/weighted-trace cleanup.
- For static `k>0` modes, the primary outer condition is the
  constraint-preserving asymptotic decaying subspace, whose leading radial
  profile is `exp(-kx)/x` and Robin operator is `D_x+k+1/x`. Homogeneous
  Dirichlet is reserved for boundary-systematic comparison. The `k=0` sector
  needs a separate charge/gauge asymptotic convention.
- Locked boundary-row ownership, endpoint stencil order, mixed-derivative
  closure, P+/P- separation, hidden-ww normalization, sparse generalized
  matrix layout, mutation guards, and quantitative radial/boundary-location
  acceptance criteria. No code or tests were added.
- All boundary implementation/validation, boundary-bearing complete-operator,
  MOTS, eigensolver, threshold, production, Stage 4AO-D, and Checkpoint G gates
  remain false. The next narrow target is the independently testable inner
  pure-outflow endpoint closure and reflection fixture.

## 2026-07-22 - Stage 4AO-C Inner Pure-Outflow Endpoint Operator

Category: Validation Boundary Operator + Focused Numerical Oracle

- Began from clean committed radial-boundary preflight checkpoint `6809d99`
  and used CodeGraph before direct source inspection.
- Implemented explicit second-order one-sided first/second radial derivatives
  with exposed coefficients and reach, plus Fourier-amplitude
  `D_xz=D_xD_z` in both complete parity sectors. Short grids, invalid spacing,
  and invalid Fourier inputs reject. No stored radial ghost unknown or
  extrapolated physical boundary data is present.
- Added a validation-only inner wrapper that constructs endpoint derivative
  jets and consumes the existing full 13-variable interior assembler exactly
  once. All PDE rows remain active, zero continuum boundary equations are
  added, no field is reset, representative ww outputs remain single-copy, and
  determinant/weighted-trace cleanup remains a separate idempotent operation.
- The focused fixture proves polynomial exactness and observed second-order
  convergence, rejects stencil/owner/slot/ww/row-count mutations, and gives
  zero full-sector parity leakage and reflection commutator. Its local
  endpoint-symbol reflection measure converges from `5.48e-5` to `5.11e-8`,
  below the predeclared `1e-6` target.
- All principal speeds are outward for the declared four inner-location
  ratios. Glancing and exterior-inner-boundary cases are rejected without
  claiming a complete characteristic basis for the longitudinal Jordan block.
- Only inner endpoint implementation/validation flags are true. Outer and
  complete radial-boundary work, the boundary-bearing operator, MOTS,
  eigensolver/shift-invert, threshold work, production wiring, Stage 4AO-D,
  and Checkpoint G remain closed.

## 2026-07-22 - Stage 4AO-C Outer k>0 Asymptotic Normalization Lock

Category: Boundary Physics + Projector/Matrix Design

- Began from clean committed inner-boundary checkpoint `7ceec6f` and used
  CodeGraph before direct source inspection.
- The leading stationary determinant factor classifies four physical/Z4 light
  pairs and five zero-exponential directions. The later audit found that this
  does not establish the full decaying subspace or nine annihilator rows.
- GP advection changes the asymptotics. Physical blocks have no `sqrt(x)`
  exponent but acquire `p=1+k r0/2`. Vector/scalar Z4 damping adds
  `exp[-0.1 sqrt(r0*x)]` and `exp[-0.5 sqrt(r0*x)]` plus the declared
  damping-dependent power.
- Audit correction (2026-07-23): only the scalar profile recursion was
  available. Full-vector transport through `x^-3/2`, the left nullspace,
  Jordan exclusion, and a polynomial boundary representation remain missing.
- This was documentation/design only. All outer implementation/validation,
  aggregate boundary, boundary-bearing operator, eigensolver, MOTS,
  threshold, production, Stage 4AO-D, and Checkpoint G gates remain closed.

## 2026-07-22 - Stage 4AO-C Outer Transformed-Amplitude Projector Helper

Category: Validation Boundary Basis + Analytic Asymptotic Oracle

- Began from clean committed outer-asymptotic checkpoint `c69eaac` and used
  CodeGraph before direct source inspection.
- Implemented the finite-`k>0` boundary-local map from `(U,D_xU)` to the
  physical/Z4 characteristic pairs and `J,F,G,C_h,C_A`, preserving the two
  hidden trace copies and writing each representative `ww` amplitude once.
- Audit correction (2026-07-23): the four stored columns are one-hot
  diagnostic amplitudes scaled by scalar profiles. Their scalar residual,
  rank, condition, and mixing numbers do not validate full WKB states,
  growing/Jordan rejection, parity, constraints, or endpoint rows. The
  hard-coded zero outer parity diagnostics were removed.
- Only diagnostic transform/projector helper flags are true. Actual outer
  endpoint rows,
  aggregate boundary completion, the boundary-bearing operator, eigensolver,
  MOTS, threshold, production wiring, Stage 4AO-D, and Checkpoint G remain
  closed.

## 2026-07-23 - Stage 4AO-C Outer-Boundary Audit Repair

Category: Gate Correction + Diagnostic Boundary Scaffolding

- CodeGraph showed that the purported decaying basis contained four one-hot
  transformed-amplitude columns scaled by scalar profiles. No full
  thirteen-component WKB state/derivative recursion or stationary-symbol dual
  left basis is present, so the required boundary cannot be constructed
  without inventing coefficients.
- Preserved the validated inner endpoint, complete interior operator, mirrored
  outer derivatives, and diagnostic radial row layout. Renamed the direct
  `W_b^in,J,F,G,C_h,C_A` selectors and provisional PDE projections so they do
  not own the primary outer gate.
- Removed the interpolation-at-`k={1,2,3}` coefficient extraction, exact
  quadratic-pencil claim, self-subtraction projector/Dirichlet comparison, and
  hard-coded zero outer parity diagnostics. Determinant and metric-inclusive
  A-trace cleanup remains checked on nonzero data at both endpoint locations.
- Outer implementation/validation, aggregate radial validation,
  boundary-bearing completion, and exact quadratic-pencil flags are false.
  The interior is still quadratic in `k`; no solver class is assigned to an
  absent outer operator, and no polynomial or nonlinear eigensolver work is
  authorized.

## 2026-07-23 - Stage 4AO-C Full-Vector Stationary WKB Audit

Category: Symbolic Radial System + Honest Asymptotic Blocker

- Began from clean commit `6628f49` and used CodeGraph before direct source
  inspection. No technical code, endpoint rows, nullspace implementation,
  eigensolver, MOTS work, or production wiring was added.
- Extracted the complete sparse `M0`, `M1`, and `M2` matrices of the
  stationary thirteen-row radial system. The tables retain all `1/x`,
  `1/x^2`, encoded-Z, hidden-sphere, Gamma, and higher mixed spherical/lambda
  contributions and cover both parity sectors through one sign parameter.
- Independent comparison at `q={0.31,0.73,1.11}`, `X={7,13,29}`, and both
  parity sectors gives maximum source-versus-matrix mismatch `2.44e-16`.
- Retracted the proposed universal determinant factor and generalized-chain
  obstruction. Exact rational arithmetic at `q=2/3` gives a factor
  proportional to `s^5(s^2-q^2)^3(s^2-q^2+1/200)`, which disproves the
  former fourfold light claim but is not a general-`q` factorization.
- The leading matrix is generically singular with nullity three. Its kernel
  vectors remain verified, but ordinary kernel dimension does not directly
  give geometric multiplicity for this singular problem. Higher-order
  solvability or a regularized first-order radial system is required;
  coincident leading vectors with distinct subleading series remain possible.
- Consequently `A_-`, its nine-dimensional physical annihilator, and the
  four-row PDE dual do not yet exist. No generalized-chain obstruction has
  been established.
- All outer, aggregate-boundary, boundary-bearing, pencil, eigensolver,
  `k=0`, nonzero-growth, MOTS, threshold, production, 4AO-D, and Checkpoint G
  gates remain false. The interior stays quadratic in `k`, but no boundary
  solver classification or eigensolver work is authorized.
- Project decision: freeze the custom stationary outer-boundary problem as
  deferred research. The next phase inventories custom-solver/GRChombo
  overlap, compares common formulas and numerical outputs, uses those results
  to assess AI-agent capability, and adapts missing production functionality
  directly from GRChombo.

## 2026-07-23 - Custom Solver / GRChombo Overlap-and-Gap Inventory

Category: Production Ownership + Comparison Design

- Began from clean frozen-outer-boundary checkpoint `015a035` and used
  CodeGraph before direct source inventory. This was documentation only.
- Locked the project roles: GRChombo owns production framework/conventions;
  the custom complete frozen interior, analytic nonlinear JVP, and hidden
  helpers remain independent oracles; the stationary outer boundary remains
  deferred research.
- Inspected the clean local GRChombo checkout at
  `37e659523830418b210acea1661dac0e00bb1b75`. Standard CCZ4, moving-puncture
  gauge, derivatives, generic boundaries, RK4, AMR, MPI/OpenMP,
  checkpoint/restart, parsing, reductions, interpolation, and AH/PETSc source
  are present. No black-string or modified-cartoon production implementation
  was found in this checkout.
- Added a master capability matrix covering variables/conventions, every
  frozen evolution row, gauge/background, numerical infrastructure, and all
  requested diagnostics. Each entry records locations/status, formula,
  match/gap, evidence, required comparison, acceptance, and production owner.
- Added a four-level comparison plan with fixed pre-run pointwise tolerances,
  a prioritized GRChombo adaptation backlog, and an AI-agent scorecard schema.
  No final agent score was assigned.
- First planned executable comparisons are the dependency manifest,
  slot/dimension map, conformal algebra, contracted-connection/Z, and visible
  physical-Ricci analytic jets.
- Largest blockers are the unpinned ignored dependency checkout, unavailable
  Docker/Chombo digest, missing authoritative collaboration
  modified-cartoon/black-string source, unverified PETSc-enabled AH path, and
  missing string-horizon data conventions.
- No technical source, test, boundary, eigensolver, MOTS, production evolution,
  protected path, smoke parameter, staging, or commit change was made.

## 2026-07-23 - Custom Solver / GRChombo Comparison Batch 1

Category: Direct Source Bridge + Level-2 Geometry Comparison

- Began from clean committed comparison-inventory checkpoint
  `661468ade479cf003dc5336e665dc7b70edf48c6` and used CodeGraph before direct
  source inspection.
- Inspected GRChombo at clean detached commit
  `37e659523830418b210acea1661dac0e00bb1b75`. The focused bridge compiled the
  actual `TensorAlgebra` and `CCZ4Geometry` headers with
  `CH_SPACEDIM=GR_SPACEDIM=DEFAULT_TENSOR_DIM=3`; it did not copy GRChombo
  formulas into a comparison oracle.
- Recorded the exact thirteen-state map. Shared visible variables map
  one-to-one after layout shifts; `hww/Aww` are the only frozen custom slots
  absent from stock GRChombo. The custom production oracle remains physical
  `d=4`, gridded `d=2`, hidden multiplicity two.
- Direct conformal algebra passed with maximum absolute error `4.441e-16` and
  maximum normalized error `6.808e-05`. `gamma=h/chi` and `Kij`
  reconstruction are source/convention comparisons because no standalone
  inspected GRChombo production helper exposes them.
- The visible contracted connection/Z comparison passed over diagonal,
  off-diagonal, pure-hatted-Gamma, and mixed cases with maximum absolute error
  `1.837e-14` and maximum normalized error `1.036e-02`. The hidden `ww`
  increment was separated and remains custom-only.
- Direct visible physical Ricci passed flat, diagonal-curved, off-diagonal,
  conformal-factor, and mixed determinant-one jets with maximum absolute error
  `2.220e-16` and maximum normalized error `1.530e-04`.
- Wrong trace dimension, contracted-connection sign, and `Rxz` sign mutations
  all failed as required.
- The compiler/source manifest is usable for direct header comparisons.
  Chombo and container digests remain unresolved because neither a local
  Chombo installation nor Docker is available in this shell.
- No production code, external GRChombo source, boundary condition,
  eigensolver, MOTS, production evolution, protected path, smoke parameter,
  staging, or commit was changed. The custom stationary outer boundary remains
  deferred research.

## 2026-07-23 - Custom Solver / GRChombo Comparison Batch 2

Category: Direct CCZ4 RHS + Encoded-Z Family Comparison

- Began from clean committed batch-1 checkpoint
  `921e639dede8a020a34b20d182f43693e757561d` and used CodeGraph/MCP before
  direct source inspection.
- Locked the inspected GRChombo path at clean detached commit
  `37e659523830418b210acea1661dac0e00bb1b75`: `USE_CCZ4`, stock `d=3`,
  `Lambda=0`, `kappa1=0.1`, `kappa2=0`, `kappa3=1`,
  `covariantZ4=true`, vacuum matter, analytic supplied lapse/shift, and a
  test-only frozen gauge adapter.
- Added a focused test-only bridge that invokes the actual
  `CCZ4RHS::rhs_equation`, `compute_ricci`, and `compute_ricci_Z` source
  paths. The independent side is a generalized-dimension family construction
  evaluated at `d=3`; no production `d=4` equation changed.
- Raw Ricci, encoded-Z tensor/scalar/trace-free completion, combined Ricci-Z,
  and the `Z_over_chi` upper/lower map pass. Their largest absolute error is
  `3.053e-16`.
- Every directly overlapping visible `chi`, `h`, `K`, `Theta`, and `A`
  family passes separately. Combined-row maximum absolute errors are
  `5.551e-17`, `2.776e-17`, `1.110e-16`, `5.551e-17`, and `1.943e-16`,
  respectively.
- Omitted/doubled encoded Z, wrong lowering, missing conformal factor, wrong
  trace-free dimension, family omission/duplication/sign, wrong RHS dimension,
  and BSSN branch mutations are all detected on active nonzero data.
- Separate shift-stretch and divergence-trace subterms and zero
  matter/Lambda terms remain source-only classifications. Stock GRChombo has
  no hidden `hww/Aww` production owner, so all hidden/cartoon terms and
  cleanup remain adaptation gaps.
- Batch 1 and sixteen affected custom term-family/geometry fixtures also pass
  under strict warnings. No external GRChombo, production source, frozen
  outer-boundary work, smoke parameters, staging, or commit was changed.
- Recommended batch 3 is a Level-3 discrete manufactured-profile comparison
  through real GRChombo derivative paths, not target-`d=4` production work.
