# Project Log

Chronological lab notebook.

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
