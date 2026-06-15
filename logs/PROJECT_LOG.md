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
