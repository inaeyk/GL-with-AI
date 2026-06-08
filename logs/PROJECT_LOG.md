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
