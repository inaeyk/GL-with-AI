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

- Date: 2026-06-16
- Goal: Start Stage 4K by adding a local RHS source-block skeleton without live evolution wiring.
- Files changed: `code/BlackStringToy/CartoonRhsSourceBlock.hpp`, `code/BlackStringToy/tests/Stage4KLocalRhsSourceBlockTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing. Compiled the standalone Stage 4K skeleton fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4KLocalRhsSourceBlockTest.cpp -o /tmp/blackstringtoy_stage4k_source_block_test`; ran `/tmp/blackstringtoy_stage4k_source_block_test`, which passed the no-source/no-evolution guards, explicit inert-output flag, finite named output fields, carried `451` Ricci trace, and `225.5` scalar checks. Stage 4J was not rerun because the Stage 4J contract header was not changed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4K patch; no commit requested.

- Date: 2026-06-16
- Goal: Start Stage 4L by implementing the first local RHS formula block: trace-free Ricci projection only.
- Files changed: `code/BlackStringToy/CartoonRhsSourceBlock.hpp`, `code/BlackStringToy/tests/Stage4KLocalRhsSourceBlockTest.cpp`, `code/BlackStringToy/tests/Stage4LRicciTraceFreeSourceTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing. After review, patched the block to call `ConformalCartoonAlgebra::trace_free_part` with the shared `trace_denominator` convention and to reuse the Stage 4J away-axis validation so `x = 0` is rejected. Compiled the standalone Stage 4L trace-free Ricci fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4LRicciTraceFreeSourceTest.cpp -o /tmp/blackstringtoy_stage4l_ricci_tf_test`; ran `/tmp/blackstringtoy_stage4l_ricci_tf_test`, which passed the hard-coded `451` trace, `R_xx^TF=-110.75`, `R_xz^TF=3`, `R_zz^TF=-107.75`, `R_ww^TF=-105.75`, hidden-factor, `Rww` omission, `/3` denominator negative checks, zero full-4D trace for a consistent diagonal metric, and `x = 0` rejection. Because `CartoonRhsSourceBlock.hpp` changed directly, also compiled and ran the Stage 4K skeleton fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4KLocalRhsSourceBlockTest.cpp -o /tmp/blackstringtoy_stage4k_source_block_test` and `/tmp/blackstringtoy_stage4k_source_block_test`; it still passed. Because `ConformalCartoonAlgebra.hpp` changed directly, compiled and ran the Stage 4A algebra fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp -o /tmp/blackstringtoy_stage4a_algebra_test` and `/tmp/blackstringtoy_stage4a_algebra_test`; it still passed. Stage 4J was not rerun because the Stage 4J contract header was not changed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4L patch; no commit requested.

- Date: 2026-06-16
- Goal: Start Stage 4M by defining an explicit away-axis policy object.
- Files changed: `code/BlackStringToy/CartoonAxisPolicy.hpp`, `code/BlackStringToy/CartoonRhsContract.hpp`, `code/BlackStringToy/tests/Stage4MAxisPolicyTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing. Compiled the standalone Stage 4M axis-policy fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4MAxisPolicyTest.cpp -o /tmp/blackstringtoy_stage4m_axis_policy_test`; ran `/tmp/blackstringtoy_stage4m_axis_policy_test`, which passed finite-positive acceptance, zero/negative/NaN/infinity rejection, guarded `1/x = 0.5`, guarded `1/x^2 = 0.25`, and the Stage 4L `x = 0` boundary check. Because `CartoonRhsContract.hpp` changed, also compiled and ran the Stage 4J, Stage 4K, and Stage 4L fixtures with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4JRicciRhsContractTest.cpp -o /tmp/blackstringtoy_stage4j_rhs_contract_test` and `/tmp/blackstringtoy_stage4j_rhs_contract_test`, `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4KLocalRhsSourceBlockTest.cpp -o /tmp/blackstringtoy_stage4k_source_block_test` and `/tmp/blackstringtoy_stage4k_source_block_test`, and `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4LRicciTraceFreeSourceTest.cpp -o /tmp/blackstringtoy_stage4l_ricci_tf_test` and `/tmp/blackstringtoy_stage4l_ricci_tf_test`; all passed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4M patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4N by adding guarded away-axis singular-combination helpers for future cartoon source terms.
- Files changed: `code/BlackStringToy/CartoonSingularCombinations.hpp`, `code/BlackStringToy/tests/Stage4NSingularCombinationsTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4M was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonAxisPolicy.hpp`, nearby local helper headers, and source-block context. Compiled the standalone Stage 4N singular-combinations fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4NSingularCombinationsTest.cpp -o /tmp/blackstringtoy_stage4n_singular_combinations_test`; ran `/tmp/blackstringtoy_stage4n_singular_combinations_test`, which passed positive finite `d_x f / x` and `(f - g) / x^2` checks, zero/negative/NaN/infinite `x` rejection, nonfinite value rejection, and agreement with Stage 4M inverse helpers. `CartoonAxisPolicy.hpp` was not changed, so Stage 4M was not rerun. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4N patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4O by settling away-axis-only regime semantics before real singular source terms are added.
- Files changed: `code/BlackStringToy/CartoonAxisPolicy.hpp`, `code/BlackStringToy/CartoonSingularCombinations.hpp`, `code/BlackStringToy/tests/Stage4OAxisRegimeSemanticsTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4N was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonAxisPolicy`, `CartoonSingularCombinations`, and the Stage 4M/4N fixtures. Compiled the standalone Stage 4O fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4OAxisRegimeSemanticsTest.cpp -o /tmp/blackstringtoy_stage4o_axis_regime_test`; ran `/tmp/blackstringtoy_stage4o_axis_regime_test`, which passed the `AwayAxisOnly` implemented-mode check, regularized-axis-not-implemented check, finite-positive acceptance, zero/negative/NaN/infinity rejection, guarded `1/x = 0.5`, guarded `1/x^2 = 0.25`, and invalid-input rejection for `1/x^2`. Because `CartoonAxisPolicy.hpp` changed, reran Stage 4M with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4MAxisPolicyTest.cpp -o /tmp/blackstringtoy_stage4m_axis_policy_test` and `/tmp/blackstringtoy_stage4m_axis_policy_test`; it passed. Because `CartoonSingularCombinations.hpp` changed, reran Stage 4N with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4NSingularCombinationsTest.cpp -o /tmp/blackstringtoy_stage4n_singular_combinations_test` and `/tmp/blackstringtoy_stage4n_singular_combinations_test`; it passed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4O patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4P by adding first local cartoon singular-geometry primitives without full RHS or evolution wiring.
- Files changed: `code/BlackStringToy/CartoonGeometryPrimitives.hpp`, `code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4O was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonAxisPolicy`, `CartoonSingularCombinations`, `CartoonRhsContract`, and nearby Stage 4N/4O test style. Added local-value-only primitives for `(d_x hww) / x` and `(hxx - hww) / x^2` that route through Stage 4N helpers. Compiled the standalone Stage 4P fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp -o /tmp/blackstringtoy_stage4p_geometry_primitives_test`; ran `/tmp/blackstringtoy_stage4p_geometry_primitives_test`, which passed the no-regularization/no-full-Ricci-RHS guards, oracle values `3` and `1`, agreement with Stage 4N helpers, zero/negative/NaN/infinite `x` rejection, and nonfinite `h_xx`, `h_ww`, and `d_x_hww` rejection. `CartoonSingularCombinations.hpp` and `CartoonAxisPolicy.hpp` were not changed, so Stage 4N, Stage 4O, and Stage 4M were not rerun. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4P patch; no commit requested.

- Date: 2026-06-17
- Goal: Patch Stage 4P after review to document the regularity precondition for `(hxx - hww) / x^2`.
- Files changed: `code/BlackStringToy/CartoonGeometryPrimitives.hpp`, `code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP to inspect the Stage 4P primitive header and surrounding helper context. Added comments documenting that `(d_x hww) / x` requires expected even-parity behavior of `h_ww`, while `(hxx - hww) / x^2` requires the Stage 3I matching condition `hxx - hww = O(x^2)` for finite axis behavior and Stage 4P does not enforce it. Added a non-failing Stage 4P fixture check showing an unmatched away-axis sample returns a large finite value, which is not a regularity validation. Compiled the Stage 4P fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp -o /tmp/blackstringtoy_stage4p_geometry_primitives_test`; ran `/tmp/blackstringtoy_stage4p_geometry_primitives_test`, which passed. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4P review patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4Q by adding a local regularity/matching guard for `hxx - hww = O(x^2)`.
- Files changed: `code/BlackStringToy/CartoonRegularityChecks.hpp`, `code/BlackStringToy/tests/Stage4QRegularityMatchingTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4P was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonGeometryPrimitives`, `CartoonAxisPolicy`, and related helper context. Added `CartoonRegularityChecks.hpp` with a named pointwise residual tolerance for the `hxx - hww = O(x^2)` matching condition, finite input checks, and Stage 4O away-axis policy enforcement. Compiled the standalone Stage 4Q fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4QRegularityMatchingTest.cpp -o /tmp/blackstringtoy_stage4q_regularity_matching_test`; ran `/tmp/blackstringtoy_stage4q_regularity_matching_test`, which passed the matching sample, obvious mismatch rejection, invalid `x`, nonfinite metric values, bad tolerance checks, and no-regularization/no-analytic-proof guards. `CartoonGeometryPrimitives.hpp` and `CartoonAxisPolicy.hpp` were not changed, so Stage 4P, Stage 4O, and Stage 4M were not rerun. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4Q patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4R by adding the first regularity-guarded local cartoon source sub-block.
- Files changed: `code/BlackStringToy/CartoonRegularityGuardedSources.hpp`, `code/BlackStringToy/tests/Stage4RRegularityGuardedSourceBlockTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4Q was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonRegularityChecks`, `CartoonGeometryPrimitives`, `CartoonRhsSourceBlock`, and nearby fixture/source-block style. Added `CartoonRegularityGuardedSources.hpp`, which calls the Stage 4Q matching guard before packaging Stage 4P primitives and the matching residual. Compiled the standalone Stage 4R fixture with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4RRegularityGuardedSourceBlockTest.cpp -o /tmp/blackstringtoy_stage4r_guarded_source_block_test`; ran `/tmp/blackstringtoy_stage4r_guarded_source_block_test`, which passed the matching case, agreement with Stage 4P, mismatch rejection, invalid input rejection, and no-full-Ricci/no-regularization/no-evolution guards. `CartoonGeometryPrimitives.hpp` and `CartoonRegularityChecks.hpp` were not changed, so Stage 4P and Stage 4Q were not rerun. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4R patch; no commit requested.

- Date: 2026-06-17
- Goal: Patch Stage 4R after review so the guarded path is the required source-facing path for `(hxx - hww) / x^2`.
- Files changed: `code/BlackStringToy/CartoonGeometryPrimitives.hpp`, `code/BlackStringToy/CartoonRegularityChecks.hpp`, `code/BlackStringToy/CartoonRegularityGuardedSources.hpp`, `code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp`, `code/BlackStringToy/tests/Stage4QRegularityMatchingTest.cpp`, `code/BlackStringToy/tests/Stage4RRegularityGuardedSourceBlockTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP to inspect `CartoonGeometryPrimitives`, `CartoonRegularityChecks`, `CartoonRegularityGuardedSources`, and the Stage 4P/4Q/4R tests. Changed the public Stage 4P compute output so it exposes only the low-risk `(d_x hww) / x` primitive; the regularity-sensitive `(hxx - hww) / x^2` value is now source-facing only through the Stage 4R guarded path after the Stage 4Q matching guard. Routed the Stage 4Q residual through `CartoonAxisPolicy::inverse_x2_away_axis(x)`. Recompiled and ran Stage 4P, Stage 4Q, and Stage 4R with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp -o /tmp/blackstringtoy_stage4p_geometry_primitives_test` then `/tmp/blackstringtoy_stage4p_geometry_primitives_test`, `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4QRegularityMatchingTest.cpp -o /tmp/blackstringtoy_stage4q_regularity_matching_test` then `/tmp/blackstringtoy_stage4q_regularity_matching_test`, and `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4RRegularityGuardedSourceBlockTest.cpp -o /tmp/blackstringtoy_stage4r_guarded_source_block_test` then `/tmp/blackstringtoy_stage4r_guarded_source_block_test`; all passed. `CartoonAxisPolicy.hpp` was not changed, so Stage 4O and Stage 4M were not rerun. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4R review patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4S by connecting the regularity-guarded source sub-block into the local RHS source-block skeleton.
- Files changed: `code/BlackStringToy/CartoonRhsSourceBlock.hpp`, `code/BlackStringToy/tests/Stage4SLocalRhsGuardedGeometryIntegrationTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4R was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonRhsSourceBlock`, `CartoonRegularityGuardedSources`, and the Stage 4K/4L/4R fixture shapes. Added a local RHS guarded-geometry source-block path that calls the Stage 4R helper and carries its guarded geometry package through the source-block layer without exposing raw Stage 4P metric-difference output. Directly affected tests were compiled and run separately. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4S patch; no commit requested.

- Date: 2026-06-17
- Goal: Patch Stage 4S after review so the guarded geometry package is checked-by-construction.
- Files changed: `code/BlackStringToy/CartoonGeometryPrimitives.hpp`, `code/BlackStringToy/CartoonRegularityGuardedSources.hpp`, `code/BlackStringToy/CartoonRhsSourceBlock.hpp`, `code/BlackStringToy/tests/Stage4RRegularityGuardedSourceBlockTest.cpp`, `code/BlackStringToy/tests/Stage4SLocalRhsGuardedGeometryIntegrationTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP to inspect `CartoonGeometryPrimitives`, `CartoonRegularityChecks`, `CartoonRegularityGuardedSources`, `CartoonRhsSourceBlock`, and the Stage 4P/4Q/4R/4S tests. Made `RegularityGuardedGeometrySources` non-aggregate with private storage, const accessors, and construction only from the Stage 4R guarded compute path. Made the Stage 4S carry output non-aggregate and compute-path constructed. Removed the public Stage 4P `detail` helper for the risky metric-difference value; Stage 4R computes it internally through the guarded Stage 4N combination after the Stage 4Q guard. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4S review patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4T by adding the first guarded consumer of the checked cartoon geometry package.
- Files changed: `code/BlackStringToy/CartoonGuardedSourceConsumers.hpp`, `code/BlackStringToy/tests/Stage4TGuardedSourceConsumerTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4S was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonRegularityGuardedSources`, `CartoonRhsSourceBlock`, `CartoonRicciInterface`, and the Stage 4G/4S tests. No isolated physical Ricci/RHS sub-expression was extracted, so Stage 4T implemented a diagnostic/probe consumer rather than a physics formula. The consumer accepts only the checked Stage 4R package and reports checked values plus Stage 4Q residual status. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4T patch; no commit requested.

- Date: 2026-06-17
- Goal: Start Stage 4U by adding a source-formula authoring gate for regularity-sensitive geometry.
- Files changed: `code/BlackStringToy/CartoonSourceFormulaAuthoringGate.hpp`, `code/BlackStringToy/tests/Stage4USourceFormulaAuthoringGateTest.cpp`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4T was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `RegularityGuardedGeometrySources`, `CartoonRhsSourceBlock`, `CartoonGuardedSourceConsumers`, and related Stage 4S/4T fixture shape. Added a non-aggregate `RegularitySensitiveSourceInputs` authoring-gate type that carries the checked Stage 4R geometry package and can be built from the Stage 4S carried package or directly from the Stage 4R checked package. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4U patch; no commit requested.

- Date: 2026-06-18
- Goal: Patch Stage 4U after review to strengthen the convention warning and remove a documentation overclaim.
- Files changed: `code/BlackStringToy/CartoonSingularCombinations.hpp`, `docs/derivations/unit_test_fixture_design.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP to locate `CartoonSingularCombinations::difference_over_x2`, the Stage 4U authoring gate, the Stage 4U fixture, and the fixture-design documentation. Added a warning at `difference_over_x2` that source formulas must use the checked Stage 4U/RegularityGuardedGeometrySources path for `(hxx - hww) / x^2`. Corrected the Stage 4U fixture-design row so generic-helper or hand-written recomputation is documented as convention-only rather than mechanically caught. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4U review patch; no commit requested.

- Date: 2026-06-18
- Goal: Start Stage 4V by checking whether a first real local source-formula consumer of the Stage 4U authoring gate is derivation-locked.
- Files changed: `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4U was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect the Stage 4U authoring gate, Stage 4R guarded package, Stage 4S carry path, Stage 4T diagnostic consumer, and the Stage 4G/4L validated helper context. Chose Outcome B: no formula code was added because no tiny physics-bearing expression that consumes only the Stage 4U checked package is currently extracted from the validated Stage 4G or Stage 4L work. The next required step is to derive or isolate the exact Ricci/RHS sub-expression, coefficient/sign convention, and oracle before coding. Python scripts, grep, ripgrep, tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4V documentation patch; no commit requested.

- Date: 2026-06-18
- Goal: Start Stage 4W by locking the hidden-sphere CCZ4 contribution map and `R_ww` source target before implementation.
- Files changed: `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4V was committed and the worktree was clean before editing. Used CodeGraph/MCP to inspect `CartoonRicciInterface::compute_metric_derivative_ricci`, the Stage 4G `R_ww` oracle context, `RegularityGuardedGeometrySources`, and the Stage 4U authoring-gate boundary. Added documentation only: the new note records hidden-sphere CCZ4 contributions, the warped-product `R_ww` target, Stage 4G-compatible flat/cone/nonconstant oracles, and the missing `h_xz = O(x)` guard that blocks `R_ww` implementation. No tests were run because no code headers changed. Python scripts, grep, ripgrep, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4W documentation patch; no commit requested.

- Date: 2026-06-18
- Goal: Patch Stage 4W after review to add the conformal/physical Ricci split, sign-convention checks, and first-principles `h_xz = O(x)` parity derivation.
- Files changed: `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP to inspect the Stage 4G Ricci context and then read the untracked Stage 4W note, Stage 3A initial-data convention note, and Stage 4 compatibility docs directly. Updated documentation only: `tilde{R}_ww[h]`, `R^chi_ww`, and `R_ww[gamma]` are now distinct; the varying-`chi` oracle is documented; the `A_IJ` curvature/lapse sign is tied to the Stage 3A `K_IJ` convention caveat; and `h_xz = O(x)` is derived from reflection parity. No C++ code, tests, Python scripts, grep, ripgrep, scratch builds, smoke runs, or LaTeX/PDF commands were run.
- Review status: Unstaged Stage 4W review patch; no commit requested.

- Date: 2026-06-18
- Goal: Start Stage 4X by adding the checked local `h_xz / x` ingredient.
- Files changed: `code/BlackStringToy/CartoonCheckedHxzOverX.hpp`, `code/BlackStringToy/tests/Stage4XCheckedHxzOverXIngredientTest.cpp`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4W was committed and the worktree was clean before editing. Used CodeGraph/MCP to locate the existing away-axis policy, singular-combination helpers, Stage 4Q matching guard, Stage 4U authoring-gate context, and Stage 4W derivation note. Added a local checked `h_xz / x` package that uses the Stage 4M away-axis policy, rejects invalid axis and nonfinite inputs, allows finite nonzero quotients, and remains non-aggregate. The standalone Stage 4X fixture checks the accepted examples, rejection cases, and dummy checked-package consumer shape. This is not a global parity proof or proof of `h_xz = O(x)`.
- Review status: Unstaged Stage 4X patch; no commit requested.

- Date: 2026-06-18
- Goal: Start Stage 4Y by adding the first guarded conformal `R_ww` singular-gradient sub-block.
- Files changed: `code/BlackStringToy/CartoonConformalRwwSingularBlock.hpp`, `code/BlackStringToy/tests/Stage4YConformalRwwSingularBlockTest.cpp`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4X was committed and the worktree was clean before editing. Used CodeGraph/MCP to locate the Stage 4U checked diagonal package, Stage 4X checked quotient package, determinant helper, and nearby fixture style. Added `G_sing = (h_zz / D) Delta_xw - q_xz^2 / D` as a local checked sub-block consuming Stage 4U and Stage 4X checked ingredients. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4YConformalRwwSingularBlockTest.cpp -o /tmp/blackstringtoy_stage4y_conformal_rww_singular_block_test` then `/tmp/blackstringtoy_stage4y_conformal_rww_singular_block_test`; it passed. Stage 4U, Stage 4X, Stage 4M, and Stage 4N headers were not changed, so their tests were not rerun. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4Y patch; no commit requested.

- Date: 2026-06-18
- Goal: Patch Stage 4Y after review to single-source raw determinant data and checked singular ingredients.
- Files changed: `code/BlackStringToy/CartoonConformalRwwSingularBlock.hpp`, `code/BlackStringToy/tests/Stage4YConformalRwwSingularBlockTest.cpp`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4Y was uncommitted and Stage 4X was the current committed state. Used CodeGraph/MCP first to locate the Stage 4Y formula/test, Stage 4U checked diagonal package, Stage 4R/4S guarded path, Stage 4X checked quotient package, and relevant docs/logs. Replaced the loose Stage 4Y formula boundary with non-forgeable `ConformalRwwSingularBlockInputs` minted by a one-local-point factory. The formula still computes `G_sing = (h_zz / D) Delta_xw - q_xz^2 / D`, but the factory now computes checked `Delta_xw`, checked `q_xz`, and determinant data from the same `x`, `h_xx`, `h_xz`, `h_zz`, `h_ww` inputs. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4YConformalRwwSingularBlockTest.cpp -o /tmp/blackstringtoy_stage4y_conformal_rww_singular_block_test` then `/tmp/blackstringtoy_stage4y_conformal_rww_singular_block_test`; it passed. Stage 4U and Stage 4X headers were not changed, so their tests were not rerun. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4Y review patch; no commit requested.

- Date: 2026-06-18
- Goal: Integrate the Stage 4 hidden-sphere Ricci plan into project roadmaps and checklists.
- Files changed: `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing. Used CodeGraph/MCP first to locate the current Stage 4Y implementation, Stage 4W/4X/4Y context, and roadmap files. Added a Markdown-only hidden-sphere Ricci plan with the protected conventions, conformal/physical split, Stage 4Y completed block, future `W_x/x`, Hessian, `R^chi_ww`, split-vs-direct physical Ricci identity, true `h_xz` parity validation, `hat_Gamma^x` GL-growth anchor, sign-convention, completed-stage summary, Stage 4Z-4AR breakdown, and risk register. Updated TODO and checklists to mark completed Stage 4E-4Y review items only where docs/logs show completed work, and added concrete remaining gates. No C++ code, tests, Python scripts, grep, ripgrep, scratch builds, smoke runs, or LaTeX/PDF commands were run.
- Review status: Unstaged documentation-only roadmap integration; no commit requested.

- Date: 2026-06-18
- Goal: Start Stage 4Z by adding the checked `W_x / x` ingredient and conformal `R_ww` first-derivative gradient block.
- Files changed: `code/BlackStringToy/CartoonCheckedDxhwwOverX.hpp`, `code/BlackStringToy/CartoonConformalRwwGradientBlock.hpp`, `code/BlackStringToy/tests/Stage4ZConformalRwwGradientBlockTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the Stage 4 hidden-sphere roadmap integration and Stage 4Y were committed, and confirmed the worktree was clean before editing. Used CodeGraph/MCP first to inspect the Stage 4Y single-source pattern, Stage 4X checked quotient, and Stage 4M/4N away-axis helper style. Added a non-forgeable checked local `d_x hww / x` package and a non-forgeable single-source `ConformalRwwGradientBlockInputs` package. Implemented only `G_grad = -(C/D) p_W + (q_xz W_z)/D - (C W_x^2 - 2 B W_x W_z + A W_z^2)/(4 W D)`. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4ZConformalRwwGradientBlockTest.cpp -o /tmp/blackstringtoy_stage4z_conformal_rww_gradient_block_test` then `/tmp/blackstringtoy_stage4z_conformal_rww_gradient_block_test`; it passed. Stage 4X, Stage 4Y, Stage 4M, and Stage 4N headers were not changed, so their tests were not rerun. Python scripts, grep, ripgrep, unrelated old tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4Z patch; no commit requested.

- Date: 2026-06-18
- Goal: Start Stage 4AA by locking the Hessian conformal hidden Ricci derivation before implementation.
- Files changed: `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing and used CodeGraph/MCP first to inspect the Stage 4Z conformal gradient block, Stage 4Y singular block context, and hidden-sphere Ricci roadmap files. Updated documentation only: recorded `G^Hess_ww = -(sqrt(W)/x)[(C/D)H_xx - (2B/D)H_xz + (A/D)H_zz]`, `H_ab = rho_ab - Gamma^x_ab rho_x - Gamma^z_ab rho_z`, the `rho` derivative formulas, reduced-base Christoffels, and Hessian oracles. The locked oracles are flat `0`, constant cone `0`, nonconstant `W=(1+x)^2` Hessian `-4`, combined `tilde R_ww=-12` for that fixture, and Claude Audit A's verified nonsymmetric oracle `G^Hess_ww=-8558/2883` with full conformal sum `-3576/961`. That nonsymmetric oracle exercises off-diagonal Christoffels, `rho_xz`, `W_z` terms, and the `(-2B/D)` contraction and is required in the Stage 4AB test. No C++ code, tests, Python scripts, grep, ripgrep, scratch builds, smoke runs, or LaTeX/PDF commands were run.
- Review status: Unstaged Stage 4AA documentation patch; no commit requested.

- Date: 2026-06-19
- Goal: Start Stage 4AB by implementing the conformal hidden Ricci Hessian sub-block.
- Files changed: `code/BlackStringToy/CartoonConformalRwwHessianBlock.hpp`, `code/BlackStringToy/tests/Stage4ABConformalRwwHessianBlockTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4AA was committed and the worktree was clean before editing. Used CodeGraph/MCP first to inspect the Stage 4Y/4Z formula patterns and non-forgeable input package style. Added a local Hessian-only formula header with a single-source `ConformalRwwHessianBlockInputs` package, finite/away-axis/positive-`W`/positive-determinant validation, Stage 4AA rho derivatives, and Stage 4AA reduced-base Christoffels. Added a standalone fixture checking flat `0`, constant cone `0`, nonconstant `W=(1+x)^2` `-4`, Claude-verified nonsymmetric `-8558/2883`, a test-only Stage 4Y+4Z+4AB sum `-3576/961`, invalid input rejection, and non-aggregate input shape. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4ABConformalRwwHessianBlockTest.cpp -o /tmp/blackstringtoy_stage4ab_conformal_rww_hessian_block_test` then `/tmp/blackstringtoy_stage4ab_conformal_rww_hessian_block_test`; it passed. Stage 4Y and Stage 4Z headers were included for a test-only cross-check but not changed, so their tests were not rerun. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4AB patch; no commit requested.

- Date: 2026-06-19
- Goal: Start Stage 4AC by assembling full conformal `tilde R_ww[h]` from the reviewed local sub-blocks.
- Files changed: `code/BlackStringToy/CartoonConformalRww.hpp`, `code/BlackStringToy/tests/Stage4ACConformalRwwAssemblyTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4AB was committed and the worktree was clean before editing. Used CodeGraph/MCP first to inspect the Stage 4Y singular block, Stage 4Z gradient block, Stage 4AB Hessian block, and Stage 4G Ricci helper. Added `CartoonConformalRww.hpp` with a non-forgeable single-source `ConformalRwwInputs` package that internally mints the Stage 4Y, 4Z, and 4AB sub-block inputs from one local metric/derivative point. Implemented only `tilde R_ww[h] = G_sing + G_grad + G_Hess`. Added a standalone fixture checking flat `0`, constant-cone `-3/4`, nonconstant `W=(1+x)^2` `-12`, Claude-verified nonsymmetric `-3576/961`, invalid input rejection through the underlying factories, and non-aggregate input shape. The fixture includes direct Stage 4G `chi=1` comparisons for the nonconstant and nonsymmetric samples. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4ACConformalRwwAssemblyTest.cpp -o /tmp/blackstringtoy_stage4ac_conformal_rww_assembly_test` then `/tmp/blackstringtoy_stage4ac_conformal_rww_assembly_test`; it passed. Older Stage 4Y/4Z/4AB tests were not rerun because their headers were included but not changed. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4AC patch; no commit requested.

- Date: 2026-06-19
- Goal: Apply Checkpoint B determinant-policy cleanup before Stage 4AD.
- Files changed: `code/BlackStringToy/CartoonConformalRwwSingularBlock.hpp`, `code/BlackStringToy/tests/Stage4YConformalRwwSingularBlockTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Started from the current uncommitted Stage 4AC tree and used CodeGraph/MCP first to inspect the Stage 4Y singular block, Stage 4Z/4AB positive determinant policy, and Stage 4AC assembly path. Changed the Stage 4Y reduced determinant gate from finite nonzero to finite positive, matching the rest of the conformal `R_ww` stack. Added a Stage 4Y negative-determinant rejection case. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4YConformalRwwSingularBlockTest.cpp -o /tmp/blackstringtoy_stage4y_conformal_rww_singular_block_test` then `/tmp/blackstringtoy_stage4y_conformal_rww_singular_block_test`; it passed. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4ACConformalRwwAssemblyTest.cpp -o /tmp/blackstringtoy_stage4ac_conformal_rww_assembly_test` then `/tmp/blackstringtoy_stage4ac_conformal_rww_assembly_test`; it passed. Stage 4Z and Stage 4AB headers were not changed, so their tests were not rerun. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Checkpoint B cleanup patch; no commit requested.

- Date: 2026-06-19
- Goal: Stage 4AD documentation-only derivation lock for `R^chi_ww` and its guard stack.
- Files changed: `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the tree was clean before editing. Used CodeGraph/MCP first to locate the hidden-sphere Ricci roadmap, Stage 4W derivation note, Stage 4H compatibility note, fixture-design table, checklist, TODO, and logs. Updated documentation only: locked `R^chi_ww`, `D_wD_w chi`, the full conformal Laplacian with hidden multiplicity `(2/W)D_wD_w chi`, the reduced scalar Hessian pieces, and the conformal gradient norm. Designed the Stage 4AE guard stack around a new checked local `chi_x/x` ingredient plus existing checked `q_xz`, checked `p_W`, away-axis-only policy, positive `chi`, positive `W`, and positive determinant guards. Recorded constant-`chi` zero, flat `chi=1+a x` `11/144`, and pending z-dependent `chi=1+b z` `-1/64` oracle values. No C++ code changed, so no C++ tests were run. Python scripts, grep, ripgrep, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4AD documentation patch; no commit requested.

- Date: 2026-06-19
- Goal: Start Stage 4AE by implementing the local conformal-factor hidden Ricci correction `R^chi_ww`.
- Files changed: `code/BlackStringToy/CartoonCheckedDxchiOverX.hpp`, `code/BlackStringToy/CartoonConformalFactorRww.hpp`, `code/BlackStringToy/tests/Stage4AEConformalFactorRwwTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4AD commit `3df625e` and a clean worktree. Used CodeGraph/MCP first to inspect the Stage 4X/4Z checked quotient patterns, Stage 4AB Christoffels, Stage 4AC assembly fixture, and Stage 4G Ricci interface. Added checked local `chi_x/x`, a single-source non-forgeable `R^chi_ww` input package, the guarded `D_wD_w chi` formula, full conformal Laplacian with hidden multiplicity `2/W`, gradient norm, and local correction. The standalone fixture checks constant `chi` zero, linear-`x` `11/144`, linear-`z` `-1/64`, nonsymmetric intermediates/final `63341/48050`, invalid input rejection, non-aggregate package shape, and one test-only Stage 4G physical-minus-Stage 4AC conformal comparison. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AEConformalFactorRwwTest.cpp -o /tmp/blackstringtoy_stage4ae_conformal_factor_rww_test` then `/tmp/blackstringtoy_stage4ae_conformal_factor_rww_test`; it passed. No older tests were rerun because existing headers were included but not changed. Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, and LaTeX/PDF commands were intentionally not run.
- Review status: Unstaged Stage 4AE patch; Checkpoint C / Claude Audit C required before Stage 4AF; no commit requested.

- Date: 2026-06-19
- Goal: Add the Stage 4AF internal hard split-vs-direct physical `R_ww` identity gate.
- Files changed: `code/BlackStringToy/tests/Stage4AFSplitVsDirectPhysicalRwwGateTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4AE commit `595942d` and a clean worktree. Used CodeGraph/MCP first to inspect Stage 4AC, Stage 4AE, the Stage 4G direct Ricci interface, and its physical-derivative helpers. The test independently constructs `gamma=h/chi` and its first/second derivative jet, feeds that already-physical jet to Stage 4G with `chi=1`, and compares against Stage 4AC plus Stage 4AE. Constant `chi`, flat linear-`x`, flat linear-`z`, and three nonsymmetric varying-`chi` identity points pass at `1e-10`; all nonsymmetric points have nonzero `W` second derivatives, and a separate check confirms cancellation of `A,B,C` second derivatives. Invalid inputs still reject through 4AC/4AE factories. Only the Stage 4AF test was compiled and run; no Python, grep, ripgrep, scratch, smoke, or unrelated test commands were run.
- Review status: Unstaged Stage 4AF internal identity-gate patch; Stage 4AG and Checkpoint D remain pending; no commit requested.
