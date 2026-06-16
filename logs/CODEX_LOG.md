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

- Date: 2026-06-08
- Goal: Begin Stage 1.5 target-dimension compile preflight for public GRChombo `DIM=2`, `GR_SPACEDIM=4` viability without editing external source.
- Files changed: `docs/grchombo/build_notes.md`, `docs/grchombo/source_map.md`, `research_plan/stage_checklists.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Checked Docker, `make`, main worktree status, and GRChombo worktree status; inspected `ApparentHorizonFinderTest2D/GNUmakefile`, `ApparentHorizonTest2D.cpp`, `UserVariables.hpp`, and `DimensionDefinitions.hpp`; ran requested final status/find/grep validation.
- Review status: Pending human review.

- Date: 2026-06-08
- Goal: Update Stage 1.5 documentation with the verified Docker `DIM=2` `ApparentHorizonFinderTest2D` baseline build/run result.
- Files changed: `docs/grchombo/build_notes.md`, `docs/grchombo/source_map.md`, `research_plan/stage_checklists.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, targeted greps for `USE_AHFINDER undefined`, `EXIT_CODE:0`, and `GR_SPACEDIM=4 preflight`, and checked `git -C external/GRChombo status --short`.
- Review status: Pending human review.

- Date: 2026-06-08
- Goal: Reconcile documentation before committing the Stage 1 to Stage 1.5 transition state.
- Files changed: `research_plan/project_outline.md`, `TODO.md`, `docs/grchombo/build_notes.md`, `docs/grchombo/source_map.md`, `docs/grchombo/capability_map.md`, `logs/PROJECT_LOG.md`, `CLAUDE.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, targeted greps for `Stage 1.5`, `PETSc`, `reviewed and approved by the user`, and `approval gate`, and checked `git -C external/GRChombo status --short`.
- Review status: Pending human review.

- Date: 2026-06-08
- Goal: Run the Stage 1.5 scratch-copy `GR_SPACEDIM=4` compile preflight without modifying `external/GRChombo`.
- Files changed: `docs/grchombo/build_notes.md`, `docs/grchombo/source_map.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`; ignored scratch files under `runs/stage1_5_preflight/`.
- Validation performed: Created scratch copy under `runs/stage1_5_preflight/GRChombo_GR4`, changed only the copied `ApparentHorizonTest2D.cpp` macro to `GR_SPACEDIM 4`, ran the requested Docker build, confirmed it exited 0, checked external GRChombo status, and ran the requested final status/find/grep validation.
- Review status: Pending human review.

- Date: 2026-06-08
- Goal: Begin Stage 1.5B CCZ4-side target-dimension compile preflight without modifying `external/GRChombo`.
- Files changed: `docs/grchombo/build_notes.md`, `docs/grchombo/source_map.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`; ignored scratch files under `runs/stage1_5_preflight/CCZ4_GR4/`.
- Validation performed: Inspected `CCZ4Test`, `ConstraintTest`, `CCZ4UserVariables.hpp`, and `DimensionDefinitions.hpp`; created a scratch full-GRChombo copy; ran the requested Docker build for `CCZ4Test` with `DIM=2` and `CXXCPPFLAGS="-DGR_SPACEDIM=4"`; documented the first compiler blocker and confirmed `external/GRChombo` remained clean.
- Review status: Pending human review.

## 2026-06-13

- Date: 2026-06-13
- Goal: Begin Stage 2A editable build scaffold for `BlackStringToy`.
- Files changed: `code/BlackStringToy/`, `scripts/stage2_build_blackstringtoy_scratch.sh`, `docs/grchombo/build_notes.md`, `docs/implementation/black_string_initial_data.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `bash -n scripts/stage2_build_blackstringtoy_scratch.sh`; the initial Codex run failed before scratch-copy creation because Docker was not usable from that shell. After the user manually reran `bash scripts/stage2_build_blackstringtoy_scratch.sh`, inspected `runs/stage2_blackstringtoy/GRChombo/Examples/BlackStringToy/` and confirmed the executable, `pout/pout.0` ending in `GRChombo finished.`, four HDF5 files, and inherited data outputs.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Begin Stage 2B by hardening the `BlackStringToy` scratch build workflow against root-owned Docker outputs.
- Files changed: `scripts/stage2_build_blackstringtoy_scratch.sh`, `docs/grchombo/build_notes.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `bash -n scripts/stage2_build_blackstringtoy_scratch.sh`.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Begin Stage 3A black-string initial-data design documentation without implementing source changes.
- Files changed: `docs/implementation/black_string_initial_data.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, `grep -R "Stage 3A" -n docs research_plan logs TODO.md || true`, and `grep -R "hww" -n docs/implementation/black_string_initial_data.md docs/implementation/ccz4_cartoon_variables.md || true`.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Incorporate Stage 3A review clarifications into the black-string initial-data design note without implementing source changes.
- Files changed: `docs/implementation/black_string_initial_data.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, `grep -n "h(t,z)" docs/implementation/black_string_initial_data.md || true`, `grep -n "K_xx" docs/implementation/black_string_initial_data.md || true`, `grep -n "beta" docs/implementation/black_string_initial_data.md || true`, and `git -C external/GRChombo status --short`.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Add a physics-stage PDF note workflow and Stage 3A LaTeX note.
- Files changed: `docs/physics_notes/`, `scripts/build_physics_notes.sh`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Checked for `latexmk`, `pdflatex`, `lualatex`, `xelatex`, and `tectonic`; none were available in the original Codex shell. Ran `bash -n scripts/build_physics_notes.sh`. Attempted `make -C docs/physics_notes stage3A`, which failed because `make` was unavailable in that shell. The Stage 3A PDF was later built successfully by the user at `docs/physics_notes/stage3A_black_string_initial_data.pdf`.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Record successful Stage 3A physics-note PDF build.
- Files changed: `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, `ls -lh docs/physics_notes/stage3A_black_string_initial_data.pdf`, `git check-ignore -v docs/physics_notes/stage3A_black_string_initial_data.pdf || true`, and `git -C external/GRChombo status --short`. Confirmed the PDF exists and is not ignored.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Begin Stage 3B modified-cartoon source-term derivation and validation planning without implementing source changes.
- Files changed: `docs/implementation/modified_cartoon_source_terms.md`, `docs/physics_notes/stage3B_modified_cartoon_source_terms.tex`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, targeted greps for `Stage 3B`, `modified-cartoon`, and `K_ww`, checked `git -C external/GRChombo status --short`, and confirmed no `code/BlackStringToy`, `external/GRChombo`, or script files were changed.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Incorporate Stage 3B review refinements into the modified-cartoon source-term notes without implementing source changes.
- Files changed: `docs/implementation/modified_cartoon_source_terms.md`, `docs/physics_notes/stage3B_modified_cartoon_source_terms.tex`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, greps for `sin`, `GR_SPACEDIM - CH_SPACEDIM`, and `absent from public GRChombo`, checked `git -C external/GRChombo status --short`, and confirmed no `code/BlackStringToy`, `external/GRChombo`, or script files were changed.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Begin Stage 3C symbolic/geometric derivation scaffold for modified-cartoon source terms without implementing source changes.
- Files changed: `docs/derivations/README.md`, `docs/derivations/modified_cartoon_geometry_sympy.py`, `docs/derivations/modified_cartoon_geometry_notes.md`, `docs/physics_notes/stage3C_symbolic_cartoon_geometry.tex`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, `python3 docs/derivations/modified_cartoon_geometry_sympy.py || true`, greps for `Stage 3C`, `Gamma`, and `K_ww`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`. The script reported `SymPy is required for this derivation check. Install it with: python3 -m pip install sympy`.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Record the successful Stage 3C SymPy symbolic-geometry derivation run.
- Files changed: `docs/derivations/modified_cartoon_geometry_notes.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, and `git -C external/GRChombo status --short`.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Begin Stage 3D symbolic derivation scaffold for nontrivial `gamma_ww` and hidden Ricci ingredients without implementing source changes.
- Files changed: `docs/derivations/modified_cartoon_nontrivial_hww_sympy.py`, `docs/derivations/modified_cartoon_nontrivial_hww_notes.md`, `docs/physics_notes/stage3D_nontrivial_hww_ricci.tex`, `docs/derivations/README.md`, `docs/implementation/modified_cartoon_source_terms.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, `python3 docs/derivations/modified_cartoon_nontrivial_hww_sympy.py || true`, greps for `Stage 3D`, `gamma_ww`, and `R_theta`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`. The Stage 3D symbolic checks passed.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Refine Stage 3D with constant-q Ricci regression assertions.
- Files changed: `docs/derivations/modified_cartoon_nontrivial_hww_sympy.py`, `docs/derivations/modified_cartoon_nontrivial_hww_notes.md`, `docs/physics_notes/stage3D_nontrivial_hww_ricci.tex`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `python3 docs/derivations/modified_cartoon_nontrivial_hww_sympy.py`, `git status --short`, `git diff --stat`, greps for `constant-q` and `q0`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`. The Stage 3D constant-q Ricci regression assertions passed.
- Review status: Pending human review before commit.

- Date: 2026-06-13
- Goal: Record the endorsed Stage 3D limitation that constant-q Ricci regression does not test non-constant-q derivative terms.
- Files changed: `docs/derivations/modified_cartoon_nontrivial_hww_notes.md`, `docs/physics_notes/stage3D_nontrivial_hww_ricci.tex`, `TODO.md`, `research_plan/stage_checklists.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, greps for `non-constant` and `derivative terms`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`.
- Review status: Pending human review before commit.

## 2026-06-14

- Date: 2026-06-14
- Goal: Begin Stage 3E nonconstant `q(x,z)` Ricci regression for hidden-sector derivative terms without implementing source changes.
- Files changed: `docs/derivations/modified_cartoon_nonconstant_q_sympy.py`, `docs/derivations/modified_cartoon_nonconstant_q_notes.md`, `docs/physics_notes/stage3E_nonconstant_q_regression.tex`, `docs/derivations/README.md`, `docs/implementation/modified_cartoon_source_terms.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `python3 docs/derivations/modified_cartoon_nonconstant_q_sympy.py`, `git status --short`, `git diff --stat`, greps for `Stage 3E`, `nonconstant`, and `warped-product`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`. The Stage 3E nonconstant-`q(x,z)` Ricci regression checks passed.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Document Stage 3E review caveats without changing derivation scripts or implementation code.
- Files changed: `docs/derivations/modified_cartoon_nonconstant_q_notes.md`, `docs/physics_notes/stage3E_nonconstant_q_regression.tex`, `TODO.md`, `research_plan/stage_checklists.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `git diff --stat`, greps for `transitively`, `R_zz`, and `shared module`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Begin Stage 3G conformal-cartoon translation derivation without implementing C++ source terms. Stage 3G at the time; renumbered to Stage 3F on 2026-06-14.
- Files changed: `docs/derivations/conformal_cartoon_translation_sympy.py`, `docs/derivations/conformal_cartoon_translation_notes.md`, `docs/physics_notes/stage3F_conformal_cartoon_translation.tex`, `docs/derivations/README.md`, `docs/implementation/modified_cartoon_source_terms.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `git status --short`, `git diff --stat`, greps for `Stage 3G`, `hww / chi`, and `traceless`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`. The Stage 3G derivative and tracelessness checks passed. Stage 3G at the time; renumbered to Stage 3F on 2026-06-14.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Strengthen Stage 3G after review by adding repository-owned determinant, round-trip, extrinsic-curvature reconstruction, and dimension-denominator checks. Stage 3G at the time; renumbered to Stage 3F on 2026-06-14.
- Files changed: `docs/derivations/conformal_cartoon_translation_sympy.py`, `docs/derivations/conformal_cartoon_translation_notes.md`, `docs/physics_notes/stage3F_conformal_cartoon_translation.tex`, `docs/implementation/modified_cartoon_source_terms.md`, `TODO.md`, `research_plan/stage_checklists.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `git status --short`, `git diff --stat`, greps for `det gamma_4D`, `det h_4D`, `p=4`, and `hidden multiplicity`, `git -C external/GRChombo status --short`, and targeted diffs for `code/BlackStringToy` and `scripts`. The determinant, round-trip, `K_ij` reconstruction, full-4D tracelessness, and dimension-denominator guard checks passed.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Start current Stage 3G off-diagonal reduced-metric conformal-cartoon validation without implementing C++ source terms.
- Files changed: `docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, `docs/derivations/offdiagonal_conformal_cartoon_notes.md`, `docs/physics_notes/stage3G_offdiagonal_conformal_cartoon.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `python3 docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, greps for `Stage 3G` and stale old Stage 3G conformal-cartoon filename references, and targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`. The Stage 3F diagonal checks and Stage 3G off-diagonal checks passed.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Promote the sheared-flat off-diagonal Ricci gate into the repository as a Stage 3G companion regression before Stage 3H RHS decomposition.
- Files changed: `docs/derivations/offdiagonal_ricci_flat_gate_sympy.py`, `docs/derivations/offdiagonal_ricci_flat_gate_notes.md`, `docs/derivations/offdiagonal_conformal_cartoon_notes.md`, `docs/physics_notes/stage3G_offdiagonal_conformal_cartoon.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran the Stage 3C, 3D, 3E, 3F, Stage 3G algebra, and Stage 3G sheared-flat Ricci SymPy scripts; greps for `Stage 3G` and stale old Stage 3G conformal-cartoon filename references; and targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`. All six derivation scripts passed, and protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Complete the final Stage 3G review follow-up before Stage 3H by adding an explicit normalized determinant guard and Stage 3F diagonal-only warning.
- Files changed: `docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, `docs/derivations/conformal_cartoon_translation_notes.md`, `docs/derivations/offdiagonal_conformal_cartoon_notes.md`, `docs/physics_notes/stage3G_offdiagonal_conformal_cartoon.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `python3 docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, `python3 docs/derivations/offdiagonal_ricci_flat_gate_sympy.py`, grep for stale old Stage 3G conformal-cartoon filename references, and targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`. The Stage 3F, Stage 3G algebra, and Stage 3G Ricci scripts passed.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Start Stage 3H CCZ4 RHS block decomposition planning without implementing C++ source terms.
- Files changed: `docs/derivations/ccz4_rhs_block_decomposition_notes.md`, `docs/physics_notes/stage3H_ccz4_rhs_block_decomposition.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `python3 docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, `python3 docs/derivations/offdiagonal_ricci_flat_gate_sympy.py`, greps for `Stage 3H` and stale old Stage 3G conformal-cartoon filename references, and targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`. The Stage 3F, Stage 3G algebra, and Stage 3G Ricci scripts passed; protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Patch the staged Stage 3H documentation after independent review before commit.
- Files changed: `docs/derivations/ccz4_rhs_block_decomposition_notes.md`, `docs/physics_notes/stage3H_ccz4_rhs_block_decomposition.tex`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `python3 docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, `python3 docs/derivations/offdiagonal_ricci_flat_gate_sympy.py`, greps for `Stage 3H`, stale evolved-`Z_i` connection-block wording, and stale old Stage 3G conformal-cartoon filename references, plus targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`. The Stage 3F, Stage 3G algebra, and Stage 3G Ricci scripts passed; the stale wording greps returned no hits; protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Start Stage 3I small-`x` regularity and conformal-cartoon removable-singularity planning without implementing finite differences or C++ source terms.
- Files changed: `docs/derivations/small_x_regularization_notes.md`, `docs/derivations/small_x_regularization_sympy.py`, `docs/physics_notes/stage3I_small_x_regularization.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `python3 docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, `python3 docs/derivations/offdiagonal_ricci_flat_gate_sympy.py`, `python3 docs/derivations/small_x_regularization_sympy.py`, greps for `Stage 3I` and stale old Stage 3G conformal-cartoon filename references, and targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`. The Stage 3F, Stage 3G algebra, Stage 3G Ricci, and Stage 3I small-`x` symbolic checks passed; protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Patch the staged Stage 3I documentation and symbolic guard after Claude/human review.
- Files changed: `docs/derivations/small_x_regularization_notes.md`, `docs/derivations/small_x_regularization_sympy.py`, `docs/physics_notes/stage3I_small_x_regularization.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`, `python3 docs/derivations/conformal_cartoon_translation_sympy.py`, `python3 docs/derivations/offdiagonal_conformal_cartoon_sympy.py`, `python3 docs/derivations/offdiagonal_ricci_flat_gate_sympy.py`, `python3 docs/derivations/small_x_regularization_sympy.py`, greps for `Stage 3I`, hatted/tilde Gamma connection text, cartoon-axis/black-string/turduckening language, stale old Stage 3G conformal-cartoon filename references, and targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`. The Stage 3F, Stage 3G algebra, Stage 3G Ricci, and Stage 3I small-`x` symbolic checks passed; protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Start Stage 3J unit-test fixture design before C++ implementation.
- Files changed: `docs/derivations/unit_test_fixture_design.md`, `docs/physics_notes/stage3J_unit_test_fixture_design.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; the Stage 3C, Stage 3D, Stage 3E, Stage 3F, Stage 3G algebra, Stage 3G Ricci, and Stage 3I small-`x` symbolic scripts; greps for `Stage 3J` and stale old Stage 3G conformal-cartoon filename references; targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`; and `git diff --stat` / `git diff`. All symbolic scripts passed, and protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Patch Stage 3J unit-test fixture design after Claude/human review.
- Files changed: `docs/derivations/unit_test_fixture_design.md`, `docs/physics_notes/stage3J_unit_test_fixture_design.tex`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; the Stage 3C, Stage 3D, Stage 3E, Stage 3F, Stage 3G algebra, Stage 3G Ricci, and Stage 3I small-`x` symbolic scripts using the actual repository filenames; greps for `Stage 3J`, round-sphere/Schwarzschild/tolerance/constraint-damping/GL-dispersion language, and stale old Stage 3G conformal-cartoon filename references; targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`; and `git diff --stat` / `git diff`. All symbolic scripts passed, protected paths had no diffs, and the stale filename grep returned no matches.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Start Stage 3K minimal C++ implementation planning without source edits.
- Files changed: `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/physics_notes/stage3K_minimal_cpp_implementation_plan.tex`, `docs/derivations/README.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; the Stage 3C, Stage 3D, Stage 3E, Stage 3F, Stage 3G algebra, Stage 3G Ricci, and Stage 3I small-`x` symbolic scripts; greps for `Stage 3K` and stale old Stage 3G conformal-cartoon filename references; targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`; and `git diff --stat` / `git diff`. All symbolic scripts passed, the stale filename grep returned no matches, and protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Patch Stage 3K minimal C++ implementation plan after Claude/human review.
- Files changed: `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/physics_notes/stage3K_minimal_cpp_implementation_plan.tex`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; the Stage 3C, Stage 3D, Stage 3E, Stage 3F, Stage 3G algebra, Stage 3G Ricci, and Stage 3I small-`x` symbolic scripts; greps for `Stage 3K`, enum/layout/static-assert/helper-green/cartoon-evolution wording, and stale old Stage 3G conformal-cartoon filename references; targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`; and `git diff --stat` / `git diff`. All symbolic scripts passed, the stale filename grep returned no matches, and protected paths had no diffs.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Clarify Stage 3 convention policy across planning documents.
- Files changed: `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/derivations/small_x_regularization_notes.md`, `docs/derivations/ccz4_rhs_block_decomposition_notes.md`, `docs/derivations/small_x_regularization_sympy.py`, `docs/physics_notes/stage3I_small_x_regularization.tex`, `docs/physics_notes/stage3J_unit_test_fixture_design.tex`, `docs/physics_notes/stage3K_minimal_cpp_implementation_plan.tex`, `docs/physics_notes/stage3H_ccz4_rhs_block_decomposition.tex`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; grep for `Pau`, `reference implementation`, `GRChombo/Pau`, and `confirm.*Pau`; the Stage 3C, 3D, 3E, 3F, 3G algebra, 3G Ricci, and 3I small-`x` symbolic scripts; grep for stale old Stage 3G conformal-cartoon filename references; targeted diffs for `external/GRChombo`, `code/BlackStringToy`, and `scripts`; and `git diff --stat` / staged-diff checks. All symbolic scripts passed, the stale filename grep returned no matches, protected paths had no diffs, and no files were staged.
- Review status: Pending human review before commit.

- Date: 2026-06-14
- Goal: Start the explicitly approved Stage 4A C++ implementation with local conformal-cartoon algebra helpers and non-grid fixtures only.
- Files changed: `code/BlackStringToy/ConformalCartoonAlgebra.hpp`, `code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; all Stage 3C-3I symbolic scripts; compiled the local fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp -o /tmp/blackstringtoy_stage4a_algebra_test`; ran `/tmp/blackstringtoy_stage4a_algebra_test`; grep for stale old Stage 3G conformal-cartoon filename references; targeted diffs for `external/GRChombo` and `scripts`; and `git diff --stat` / `git diff`. All symbolic scripts and the Stage 4A fixture passed. No files were staged.
- Review status: Completed and committed.

- Date: 2026-06-14
- Goal: Patch Stage 4A after review without adding grid wiring, enum edits, or source-term implementation.
- Files changed: `code/BlackStringToy/ConformalCartoonAlgebra.hpp`, `code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/physics_notes/stage3K_minimal_cpp_implementation_plan.tex`, `docs/derivations/unit_test_fixture_design.md`, `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; all Stage 3C-3I symbolic scripts; compiled the Stage 4A fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp -o /tmp/blackstringtoy_stage4a_algebra_test`; ran `/tmp/blackstringtoy_stage4a_algebra_test`; greps for retired implementation-stage terminology, Stage 4A/4B references, and stale old Stage 3G conformal-cartoon filename references; targeted diffs for `external/GRChombo` and `scripts`; and `git diff --stat` / staged-diff checks. All symbolic scripts and the Stage 4A fixture passed, retired active terminology/stale filename greps returned no matches, protected paths had no diffs, and no files were staged.
- Review status: Completed and committed.

- Date: 2026-06-14
- Goal: Start Stage 4B by adding an automatic variable-layout check before connecting Stage 4A helpers to grid data.
- Files changed: `code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; all Stage 3C-3I symbolic scripts; compiled and ran the Stage 4A local algebra fixture; compiled and ran the Stage 4B layout fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I external/GRChombo/Source/CCZ4 code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp -o /tmp/blackstringtoy_stage4b_layout_test` and `/tmp/blackstringtoy_stage4b_layout_test`; greps for retired implementation-stage terminology, Stage 4A/4B/4C references, and stale old Stage 3G conformal-cartoon filename references; protected-path diffs for `external/GRChombo` and `scripts`; and `git diff --stat` / `git diff`. All symbolic scripts, the Stage 4A fixture, and the Stage 4B fixture passed; retired active terminology/stale filename greps returned no matches; protected paths had no diffs; and no files were staged.
- Review status: Completed and committed.

- Date: 2026-06-14
- Goal: Patch Stage 4B after Claude/human review so its claims match what the current enum can actually prove.
- Files changed: `code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; all Stage 3C-3I symbolic scripts; compiled and ran the Stage 4A local algebra fixture; compiled and ran the Stage 4B public CCZ4 baseline-layout fixture; greps for retired implementation-stage terminology, Stage 4A/4B/4C/4D references, hidden-placement wording, and stale old Stage 3G conformal-cartoon filename references; protected-path diffs for `external/GRChombo`, `scripts`, and unchanged Stage 4A/UserVariables headers; and `git diff --stat` / `git diff`. All symbolic scripts, the Stage 4A fixture, and the Stage 4B fixture passed; retired terminology and stale filename greps returned no matches; protected paths had no diffs; and no files were staged.
- Review status: Completed and committed.

- Date: 2026-06-14
- Goal: Start Stage 4C by adding real repo-owned hidden `hww/Aww` variable entries with hard placement checks in `UserVariables.hpp`.
- Files changed: `code/BlackStringToy/UserVariables.hpp`, `code/BlackStringToy/tests/Stage4CVariablePlacementTest.cpp`, `code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/physics_notes/stage3K_minimal_cpp_implementation_plan.tex`, `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; all Stage 3C-3I symbolic scripts; compiled and ran the Stage 4A local algebra fixture; compiled and ran the Stage 4B public baseline fixture; compiled and ran the Stage 4C hidden-variable placement fixture; greps for retired implementation-stage terminology, Stage 4A/4B/4C/4D references, real hidden-placement assertions, stale public-alias wording, and stale old Stage 3G conformal-cartoon filename references; protected-path diffs for `external/GRChombo` and `scripts`; a grid-structure grep over Stage 4B/4C tests and `UserVariables.hpp`; and `git diff --stat` / `git diff`. All symbolic scripts and Stage 4 fixtures passed, stale wording/filename greps returned no matches, protected paths had no diffs, and no files were staged.
- Review status: Completed and committed.

- Date: 2026-06-14
- Goal: Run the Stage 4C build-validation follow-up in the full scratch Docker/GRChombo scaffold.
- Files changed: `code/BlackStringToy/params_stage2_smoke.txt`, `docs/grchombo/build_notes.md`, `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; all Stage 3C-3I symbolic scripts; compiled and ran the Stage 4A, Stage 4B, and Stage 4C standalone fixtures; attempted `bash scripts/stage2_build_blackstringtoy_scratch.sh` from the sandbox and observed Docker was unavailable there; reran the same script from an escalated Docker-capable shell; repaired root-owned scratch output with Docker-scoped `chown`; minimally updated the smoke `vars_parity` list from 25 to 27 entries for `hww/Aww`; reran the same scratch script. The full GRChombo compile succeeded and produced `Main_BlackStringToy3d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex`. The smoke command `timeout 180s ./Main_BlackStringToy3d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex params_stage2_smoke.txt` failed during first advance because `hww` and `Aww` were non-finite/uninitialized. Greps for retired implementation-stage terminology and stale old Stage 3G conformal-cartoon filename references returned no matches; protected-path diffs for `external/GRChombo` and `scripts` were clean; scratch ownership was clean after repair.
- Review status: Completed and committed.

## 2026-06-15

- Date: 2026-06-15
- Goal: Tie up Stage 4C bookkeeping before starting Stage 4D.
- Files changed: `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/grchombo/build_notes.md`, `docs/physics_notes/stage3K_minimal_cpp_implementation_plan.tex`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; all Stage 3C-3I symbolic scripts; compiled and ran the Stage 4A, Stage 4B, and Stage 4C standalone fixtures; greps for retired implementation-stage terminology, Stage 4A-4D roadmap references, stale old Stage 3G conformal-cartoon filename references, and pending-review log text; protected-path diffs for `external/GRChombo` and `scripts`; and `git diff --stat` / `git diff`. Symbolic scripts and Stage 4 fixtures passed; stale slice/stale filename greps returned no matches; pending-review hits are older historical log entries, while Stage 4A-4C entries now say completed/committed; protected paths had no diffs.
- Review status: Unstaged bookkeeping update; no commit requested.

- Date: 2026-06-15
- Goal: Compile companion PDFs for all current physics-note LaTeX sources.
- Files changed: `docs/physics_notes/*.pdf`, `docs/physics_notes/Makefile`, `TODO.md`, `research_plan/stage_checklists.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Found all `.tex` files with `find`; checked same-name PDF status; built missing PDFs with `latexmk`; removed the ignored `.build` auxiliary directory; verified every current physics-note `.tex` has a same-name `.pdf`; ran only the requested git checks for status, diff stat, `external/GRChombo`, and `scripts`.
- Review status: Unstaged documentation/build-output update; no commit requested.

- Date: 2026-06-15
- Goal: Patch Stage 4D so the temporary `hww/Aww` scaffold freeze is explicitly smoke-only.
- Files changed: `code/BlackStringToy/SimulationParameters.hpp`, `code/BlackStringToy/BlackStringToyLevel.cpp`, `code/BlackStringToy/params_stage2_smoke.txt`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/grchombo/build_notes.md`, `TODO.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: ran `git status --short`; repaired ignored scratch ownership with Docker after previous root-owned outputs; ran the documented scratch Docker/GRChombo `BlackStringToy` build/smoke workflow with `bash scripts/stage2_build_blackstringtoy_scratch.sh`; observed the fixed `timeout 180s ./Main_BlackStringToy3d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex params_stage2_smoke.txt` smoke command complete with `GRChombo finished` after reducing the cheap smoke workload to `max_level = 1`; repaired scratch ownership again after Docker output; then ran protected-path diffs for `external/GRChombo` and `scripts`, plus `git diff --stat` / `git diff`. Python scripts, grep, Stage 3 symbolic checks, and Stage 4A/4B/4C fixtures were intentionally not run for this patch.
- Review status: Unstaged Stage 4D patch; no commit requested.

- Date: 2026-06-15
- Goal: Start Stage 4E first safe grid-to-helper handoff check.
- Files changed: `code/BlackStringToy/BlackStringToyLevel.cpp`, `code/BlackStringToy/SimulationParameters.hpp`, `code/BlackStringToy/params_stage2_smoke.txt`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/grchombo/build_notes.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; ran the documented scratch Docker/GRChombo `BlackStringToy` build/smoke workflow with `bash scripts/stage2_build_blackstringtoy_scratch.sh`; observed the internal build command `make -j2 all DIM=3 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE` and smoke command `timeout 180s ./Main_BlackStringToy3d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex params_stage2_smoke.txt`; confirmed `pout/pout.0` ended with `GRChombo finished`; repaired ignored scratch ownership with Docker after the wrapper hit the known post-run `sudo chown` limitation; then ran protected-path diffs for `external/GRChombo` and `scripts`, plus `git diff --stat` / `git diff`. Python scripts, grep, Stage 3 symbolic checks, and Stage 4A/4B/4C fixtures were intentionally not run.
- Review status: Unstaged Stage 4E patch; no commit requested.

- Date: 2026-06-15
- Goal: Patch Stage 4E with a distinct-value grid-to-helper mapping test after review.
- Files changed: `code/BlackStringToy/tests/Stage4EGridToHelperMappingTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/grchombo/build_notes.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Ran `git status --short`; attempted `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4EGridToHelperMappingTest.cpp -o /tmp/blackstringtoy_stage4e_mapping_test`, which needed the GRChombo utility include path for `ArrayTools.hpp`; reran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy -I external/GRChombo/Source/utils code/BlackStringToy/tests/Stage4EGridToHelperMappingTest.cpp -o /tmp/blackstringtoy_stage4e_mapping_test`; ran `/tmp/blackstringtoy_stage4e_mapping_test`, which passed all distinct-value helper-map and `K_ww = 51.375` oracle checks. Python scripts, grep, Stage 3 symbolic checks, old Stage 4 fixtures, and the scratch smoke build were intentionally not run for this review patch.
- Review status: Unstaged Stage 4E review patch; no commit requested.

- Date: 2026-06-15
- Goal: Start Stage 4F cartoon Ricci helper interface only.
- Files changed: `code/BlackStringToy/CartoonRicciInterface.hpp`, `code/BlackStringToy/tests/Stage4FCartoonRicciInterfaceTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Compiled the standalone Stage 4F interface fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4FCartoonRicciInterfaceTest.cpp -o /tmp/blackstringtoy_stage4f_ricci_interface_test`; ran `/tmp/blackstringtoy_stage4f_ricci_interface_test`, which passed the hidden-multiplicity, no-formulas-implemented, and local-value type-instantiation checks. Python scripts, grep, old Stage 3 symbolic checks, old Stage 4 fixtures, scratch smoke builds, and LaTeX/PDF commands were intentionally not run for this interface-only stage.
- Review status: Unstaged Stage 4F patch; no commit requested.

- Date: 2026-06-15
- Goal: Patch Stage 4F after review so the cartoon Ricci interface states its Ricci form explicitly.
- Files changed: `code/BlackStringToy/CartoonRicciInterface.hpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Recompiled the standalone Stage 4F interface fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4FCartoonRicciInterfaceTest.cpp -o /tmp/blackstringtoy_stage4f_ricci_interface_test`; reran `/tmp/blackstringtoy_stage4f_ricci_interface_test`, which still passed the hidden-multiplicity, no-formulas-implemented, and local-value type-instantiation checks. Python scripts, grep, old Stage 3 symbolic checks, old Stage 4A-4E fixtures, scratch smoke builds, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4F review patch; no commit requested.

- Date: 2026-06-16
- Goal: Start Stage 4G by implementing the first local metric-derivative cartoon Ricci helper.
- Files changed: `code/BlackStringToy/CartoonRicciInterface.hpp`, `code/BlackStringToy/tests/Stage4FCartoonRicciInterfaceTest.cpp`, `code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the working tree was clean before editing. Compiled the new Stage 4G fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp -o /tmp/blackstringtoy_stage4g_ricci_test`; ran `/tmp/blackstringtoy_stage4g_ricci_test`, which passed flat zero Ricci, constant-`q` cone, nonconstant-`q`, `x`-shear flat, and `x = 0` rejection checks. Recompiled the directly affected Stage 4F fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4FCartoonRicciInterfaceTest.cpp -o /tmp/blackstringtoy_stage4f_ricci_interface_test`; reran `/tmp/blackstringtoy_stage4f_ricci_interface_test`, which passed with `ricci_formulas_implemented = true`. Python scripts, grep, old Stage 3 symbolic checks, old Stage 4A-4E fixtures, scratch smoke builds, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4G patch; no commit requested.

- Date: 2026-06-16
- Goal: Review and validate Stage 4G as its own implementation step.
- Files changed: `code/BlackStringToy/CartoonRicciInterface.hpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Reviewed the Stage 4G helper and fixture against the Stage 3C-3E/3G intent without running Python or grep. Made a minimal cleanup to clarify Stage 4G-era header/documentation wording and remove dead local assembly code. Recompiled the Stage 4G fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp -o /tmp/blackstringtoy_stage4g_ricci_test`; reran `/tmp/blackstringtoy_stage4g_ricci_test`, which passed flat zero Ricci, constant-`q` cone `R_ww=-0.75`, nonconstant-`q` `R_xx=-2` and `R_ww=-12`, `x`-shear flat, and `x = 0` rejection checks. Recompiled the directly affected Stage 4F fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4FCartoonRicciInterfaceTest.cpp -o /tmp/blackstringtoy_stage4f_ricci_interface_test`; reran `/tmp/blackstringtoy_stage4f_ricci_interface_test`, which passed with `ricci_formulas_implemented = true`. Old unrelated tests, scratch builds, Python scripts, grep, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4G review patch; no commit requested.

- Date: 2026-06-16
- Goal: Start Stage 4H by deciding how the metric-derivative Ricci helper can later connect to the CCZ4 RHS path.
- Files changed: `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Read repo-owned `BlackStringToyLevel.cpp`, `BlackStringToyLevel.hpp`, `TPAMR.hpp`, `UserVariables.hpp`, `CartoonRicciInterface.hpp`, and read-only public GRChombo `CCZ4RHS.hpp`, `CCZ4RHS.impl.hpp`, `CCZ4Geometry.hpp`, `CCZ4Vars.hpp`, and `CCZ4UserVariables.hpp` directly. Documented that BlackStringToy currently calls inherited public `CCZ4RHS`, which obtains Ricci through `CCZ4Geometry::compute_ricci_Z`; that path uses evolved `Gamma`, `d1.Gamma`, Christoffels, conformal metric derivatives, and `Z_over_chi`, so it is not the pure Stage 4G metric-derivative helper contract. Ran only `git status --short`, protected-path diffs for `external/GRChombo` and `scripts`, plus `git diff --stat` / `git diff`. Python scripts, grep, ripgrep, old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4H planning patch; no commit requested.

- Date: 2026-06-16
- Goal: Start Stage 4I by adding a typed Ricci bridge contract without RHS wiring.
- Files changed: `code/BlackStringToy/CartoonRicciBridge.hpp`, `code/BlackStringToy/tests/Stage4IRicciBridgeContractTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Compiled the standalone Stage 4I bridge fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4IRicciBridgeContractTest.cpp -o /tmp/blackstringtoy_stage4i_ricci_bridge_test`; ran `/tmp/blackstringtoy_stage4i_ricci_bridge_test`, which passed the `rhs_wiring_implemented = false` guard, the hard-coded full 4D contraction `451`, the physical scalar `225.5`, negative checks for missing off-diagonal and hidden factors, and the nonpositive-`chi` guard. Stage 4G was not rerun because this stage did not change the Stage 4G Ricci header. Python scripts, grep, ripgrep, old unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4I bridge patch; no commit requested.

- Date: 2026-06-16
- Goal: Patch Stage 4I after review so cartoon Ricci output is hard to misuse.
- Files changed: `code/BlackStringToy/CartoonRicciInterface.hpp`, `code/BlackStringToy/CartoonRicciBridge.hpp`, `code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp`, `code/BlackStringToy/tests/Stage4IRicciBridgeContractTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Compiled and ran the Stage 4I bridge contract fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4IRicciBridgeContractTest.cpp -o /tmp/blackstringtoy_stage4i_ricci_bridge_test` and `/tmp/blackstringtoy_stage4i_ricci_bridge_test`; it passed the `451` trace oracle, `225.5` scalar check, bridge-exposed component checks, negative multiplicity checks, and chi guard. Because the Ricci result type changed, also compiled and ran the Stage 4G fixture and the directly affected Stage 4F fixture; both passed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4I review patch; no commit requested.

- Date: 2026-06-16
- Goal: Start Stage 4J by defining the local Ricci-to-RHS contract without evolution wiring.
- Files changed: `code/BlackStringToy/CartoonRhsContract.hpp`, `code/BlackStringToy/tests/Stage4JRicciRhsContractTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing. Compiled the standalone Stage 4J contract fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4JRicciRhsContractTest.cpp -o /tmp/blackstringtoy_stage4j_rhs_contract_test`; ran `/tmp/blackstringtoy_stage4j_rhs_contract_test`, which passed the local no-RHS/no-evolution guards, bridge-approved component checks, hard-coded full 4D Ricci trace `451`, physical scalar `225.5`, `Rww` omission sensitivity, off-diagonal factor sensitivity, and `x = 0` rejection. Stage 4I was not rerun because the Stage 4I bridge header was not changed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4J patch; no commit requested.

- Date: 2026-06-16
- Goal: Patch Stage 4J after review by hardening the Ricci bridge doorway.
- Files changed: `code/BlackStringToy/CartoonRicciBridge.hpp`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Compiled and ran the Stage 4J contract fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4JRicciRhsContractTest.cpp -o /tmp/blackstringtoy_stage4j_rhs_contract_test` and `/tmp/blackstringtoy_stage4j_rhs_contract_test`; it passed the no-RHS/no-evolution guards, bridge-approved values, `451` trace oracle, `225.5` scalar, `Rww` omission sensitivity, off-diagonal sensitivity, and away-axis guard. Because the bridge header changed directly, also compiled and ran the Stage 4I bridge fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4IRicciBridgeContractTest.cpp -o /tmp/blackstringtoy_stage4i_ricci_bridge_test` and `/tmp/blackstringtoy_stage4i_ricci_bridge_test`; it passed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4J review patch; no commit requested.
