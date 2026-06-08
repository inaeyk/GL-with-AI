# Codex Log

## 2026-05-12

- Date: 2026-05-12
- Goal: Create initial repository scaffold.
- Files changed: Added Markdown scaffold, `.gitignore`, run manifest template, and ignored scratch directories.
- Validation: Ran `git status` and `find . -maxdepth 4 -type f | sort`.
- Review status: Needs user review before commit.

## 2026-05-13

- Date: 2026-05-13
- Goal: Shorten scaffold and remove redundant explanations.
- Files changed: Revised Markdown scaffold files; `.gitignore` unchanged.
- Validation: Pending final checks.
- Review status: Needs user review before commit.

- Date: 2026-05-13
- Goal: Record successful local GRChombo BinaryBH very-cheap smoke test under Windows WSL2.
- Files changed: `results/BinaryBH_verycheap/metadata.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation: Ran `uname -m`, `git status`, `git diff --stat`, and HDF5 path checks; confirmed no raw HDF5 files are staged or tracked.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Add reproducible Bash wrappers for the GRChombo BinaryBH very-cheap smoke-test workflow.
- Files changed: `scripts/run_binarybh_smoke.sh`, `scripts/collect_binarybh_outputs.sh`, `scripts/plot_binarybh_outputs.sh`, `scripts/clean_binarybh_smoke.sh`, `scripts/README.md`, `logs/CODEX_LOG.md`.
- Validation: Ran `chmod +x` on the scripts, `bash -n` on all four scripts, `git status`, `git diff --stat`, and HDF5 path checks.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Convert archived PDF research/setup context into concise living Markdown planning files for the GL project.
- Files changed: `research_plan/reproduction_plan.md`, `research_plan/project_outline.md`, `research_plan/radiation_profile_plan.md`, `research_plan/README.md`, `README.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation: Ran `git status`, `git diff --stat`, `find research_plan -maxdepth 3 -type f | sort`, `find docs/setup -maxdepth 3 -type f | sort`, ignored-extract checks, and HDF5 path checks.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Apply targeted cleanup to project planning docs after external review.
- Files changed: `TODO.md`, `research_plan/reproduction_plan.md`, `docs/implementation/horizon_diagnostics.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status` and `git diff --stat`.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Apply small BinaryBH smoke-test script and documentation fixes.
- Files changed: `scripts/clean_binarybh_smoke.sh`, `scripts/run_binarybh_smoke.sh`, `scripts/README.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `bash -n` on all BinaryBH smoke-test scripts, `git status`, and `git diff --stat`.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Inspect public GRChombo source and create first-pass capability/source maps.
- Files changed: `.gitignore`, `docs/grchombo/capability_map.md`, `docs/grchombo/source_map.md`, `docs/grchombo/build_notes.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran requested `git status`, ignored-status, diff-stat, docs listing, and GRChombo inspection commands.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Refine GRChombo maps after external review of dimensional/cartoon and benchmark framing.
- Files changed: `docs/grchombo/capability_map.md`, `docs/grchombo/source_map.md`, `docs/grchombo/build_notes.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, and targeted greps for approval-gate, `GR_SPACEDIM`, `gww`/`Kww`, and `ApparentHorizonFinderTest2D` language.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Update GRChombo maps with completed `GR_SPACEDIM` search summary.
- Files changed: `docs/grchombo/capability_map.md`, `docs/grchombo/source_map.md`, `docs/grchombo/build_notes.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, and targeted greps for `GR_SPACEDIM search`, `114`, and `ApparentHorizonFinderTest2D`.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Inspect and attempt to run public GRChombo `ApparentHorizonFinderTest2D`.
- Files changed: `docs/grchombo/source_map.md`, `docs/grchombo/build_notes.md`, `docs/implementation/horizon_diagnostics.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Inspected test files and upstream CI commands; attempted `make test -j 2` locally, which failed before build because `make` is unavailable; ran requested status/diff/find/grep validation.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Add concise CCZ4/cartoon variable naming reference for future `BlackStringToy` work.
- Files changed: `docs/implementation/ccz4_cartoon_variables.md`, `docs/implementation/horizon_diagnostics.md`, `docs/grchombo/capability_map.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, and targeted greps for `hww`, `GR_SPACEDIM`, and `c_K - 1`.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Create stage-by-stage gate checklists for the GL project.
- Files changed: `research_plan/stage_checklists.md`, `research_plan/README.md`, `research_plan/project_outline.md`, `TODO.md`, `docs/implementation/ccz4_cartoon_variables.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, and targeted greps for `Stage 1.5`, `c_K - 1`, and `reviewed and approved by the user`.
- Review status: Pending human review.

- Date: 2026-05-13
- Goal: Refine stage checklist gates and Stage 1.5 preflight wording before commit.
- Files changed: `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, and targeted greps for approval-gate, hazards/silent-failure, `ApparentHorizonFinderTest2D`, and `c_K - 1` language.
- Review status: Pending human review.

- Date: 2026-06-07
- Goal: Close the remaining Stage 1 documentation gaps by verifying the public `AHFunctions.hpp` `hww` access pattern and documenting the modified-cartoon silent-failure hazard before Stage 1.5.
- Files changed: `docs/grchombo/capability_map.md`, `docs/grchombo/source_map.md`, `docs/implementation/ccz4_cartoon_variables.md`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Inspected `external/GRChombo/Source/ApparentHorizonFinder/AHFunctions.hpp`; ran `git status --short`, `git diff --stat`, targeted greps for the new hazards section, `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and `c_K - 1` / `vars[c_K`; checked `git -C external/GRChombo status --short`.
- Review status: Pending human review.
