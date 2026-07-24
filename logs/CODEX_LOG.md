# Codex Log

Correction notice (2026-07-20): every earlier Stage 4AO-C claim below that
lists the K-row `A_IJ A^IJ + K^2/d` or calls it an `A^2/K^2` CCZ4 piece is
historical and superseded. That expression is the rejected `USE_BSSN` branch;
the current selected-CCZ4 implementation and evidence are recorded in the
2026-07-20 entry.

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

- Date: 2026-06-19
- Goal: Add the Stage 4AG true two-sided `h_xz` parity validation gate.
- Files changed: `code/BlackStringToy/CartoonHxzParityValidation.hpp`, `code/BlackStringToy/tests/Stage4AGHxzParityValidationGateTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4AF commit `9454af7` and a clean worktree. Used CodeGraph/MCP first to inspect existing checked-package style, Stage 4X quotient context, and nearby validation fixtures. Added a factory-only validated stencil that checks finite paired data, positive/negative coordinate slots, one-to-one opposite-radius matches, optional axis zero, odd `h_xz` residuals, and even `h_xz/x` residuals. The fixture accepts smooth odd data and a radius-varying even quotient, rejects the required contamination/axis/partner/invalid-input cases, and passes its standalone compile/run command. No older tests were run because no existing headers changed. No Python, grep, ripgrep, scratch, or smoke commands were run.
- Review status: Unstaged Stage 4AG parity-gate patch; Checkpoint D / Claude Audit D required before Stage 4AH; no commit requested.

- Date: 2026-06-19
- Goal: Add Stage 4AH local away-axis physical `R_ww[gamma]` assembly.
- Files changed: `code/BlackStringToy/CartoonAwayAxisPhysicalRww.hpp`, `code/BlackStringToy/tests/Stage4AHAwayAxisPhysicalRwwAssemblyTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4AG commit `ea013fb` and a clean worktree. Used CodeGraph/MCP first to inspect the Stage 4AC/4AE factories and Stage 4AF direct physical-metric comparison. Added a private single-source input package whose factory mints both reviewed downstream packages from one local jet, plus a private result exposing the two parts and sum. The standalone fixture passes flat `0`, cone `-3/4`, linear-`x` `11/144`, linear-`z` `-1/64`, nonsymmetric assembly/direct Stage 4G comparison with residual about `4.44e-16`, non-aggregate type checks, and underlying invalid-input rejection. Only the Stage 4AH test was run; no older tests, Python, grep, ripgrep, scratch, or smoke commands were run.
- Review status: Unstaged local/test-only Stage 4AH assembly; Stage 4AI next; no commit requested.

- Date: 2026-06-19
- Goal: Add Stage 4AI local physical-`R_ww` Ricci/RHS contract.
- Files changed: `code/BlackStringToy/CartoonAwayAxisPhysicalRww.hpp`, `code/BlackStringToy/CartoonPhysicalRwwRhsContract.hpp`, `code/BlackStringToy/tests/Stage4AHAwayAxisPhysicalRwwAssemblyTest.cpp`, `code/BlackStringToy/tests/Stage4AIPhysicalRwwRhsContractTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed Stage 4AH commit `d7a1b25` and a clean worktree. Used CodeGraph/MCP first to inspect the Stage 4AH result and Stage 4J Ricci/RHS contraction convention. Extended the Stage 4AH result with same-point `chi` and conformal `h^ww=1/h_ww`, then added a non-forgeable Stage 4AI contract accepting only that result. The contract computes `hidden_conformal_trace=2 h^ww R_ww[gamma]` and `hidden_physical_scalar_contribution=chi hidden_conformal_trace`. The Stage 4AI and directly affected Stage 4AH fixtures compiled and passed. No Python, grep, ripgrep, unrelated tests, scratch builds, smoke runs, staging, or commits were used.
- Review status: Unstaged local/test-only Stage 4AI contract; Checkpoint E / Claude Audit E required before Stage 4AJ; no commit requested.

- Date: 2026-06-19
- Goal: Add Stage 4AJ local physical hidden lapse Hessian `D_wD_w alpha`.
- Files changed: `code/BlackStringToy/CartoonCheckedDxalphaOverX.hpp`, `code/BlackStringToy/CartoonPhysicalHiddenLapseHessian.hpp`, `code/BlackStringToy/tests/Stage4AJPhysicalHiddenLapseHessianTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed a clean worktree before editing. Used CodeGraph/MCP first to inspect checked quotient patterns and the Stage 4AE single-source physical/conformal-factor formula style. Added checked `p_alpha=alpha_x/x`, a non-forgeable single-source physical hidden-lapse Hessian input package, and the guarded formula for `D_wD_w alpha` using `gamma=h/chi`. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AJPhysicalHiddenLapseHessianTest.cpp -o /tmp/blackstringtoy_stage4aj_physical_hidden_lapse_hessian_test` then `/tmp/blackstringtoy_stage4aj_physical_hidden_lapse_hessian_test`; it passed. No Python, grep, ripgrep, unrelated tests, scratch builds, smoke runs, staging, or commits were used.
- Review status: Unstaged local/test-only Stage 4AJ implementation; Stage 4AK next; no commit requested.

- Date: 2026-06-19
- Goal: Add Stage 4AK local hidden `A_ww` curvature/lapse geometric core.
- Files changed: `code/BlackStringToy/CartoonAwwCurvatureLapseCore.hpp`, `code/BlackStringToy/tests/Stage4AKAwwCurvatureLapseCoreTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed a clean worktree before editing. Used CodeGraph/MCP first to inspect Stage 4AH physical `R_ww[gamma]`, Stage 4AJ physical `D_wD_w alpha`, and adjacent fixture conventions. Added a non-forgeable single-source package that mints both reviewed packages from one local jet and computes `curvature_lapse_core=-D_wD_w alpha+alpha R_ww[gamma]`. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AKAwwCurvatureLapseCoreTest.cpp -o /tmp/blackstringtoy_stage4ak_aww_curvature_lapse_core_test` then `/tmp/blackstringtoy_stage4ak_aww_curvature_lapse_core_test`; it passed. No Python, grep, ripgrep, unrelated tests, scratch builds, smoke runs, staging, or commits were used.
- Review status: Unstaged local/test-only Stage 4AK implementation; Stage 4AL next; no commit requested.

- Date: 2026-06-19
- Goal: Add Stage 4AL local trace-free curvature/lapse block.
- Files changed: `code/BlackStringToy/CartoonTraceFreeCurvatureLapseBlock.hpp`, `code/BlackStringToy/tests/Stage4ALTraceFreeCurvatureLapseBlockTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed a clean worktree before editing. Used CodeGraph/MCP first to inspect Stage 4G/4I Ricci bridge, Stage 4AH physical `R_ww`, Stage 4AK hidden core, and Stage 4A/4L trace-free helpers. Added a non-forgeable single-source package for `source_IJ=chi(C_IJ-h_IJ C/4)`, with Stage 4G-vs-4AH `R_ww` agreement before assembly. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4ALTraceFreeCurvatureLapseBlockTest.cpp -o /tmp/blackstringtoy_stage4al_trace_free_curvature_lapse_test` then `/tmp/blackstringtoy_stage4al_trace_free_curvature_lapse_test`; it passed. No Python, grep, ripgrep, unrelated tests, scratch builds, smoke runs, staging, or commits were used.
- Review status: Unstaged local/test-only Stage 4AL implementation; Checkpoint F / Claude Audit F required before further source/RHS integration; no commit requested.

- Date: 2026-06-19
- Goal: Apply the Checkpoint-F roadmap follow-up before Stage 4AM.
- Files changed: `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to locate the Stage 4 roadmap/checklist/log docs. Updated documentation only: marked Stage 4AL and Checkpoint F complete, split the remaining `hat_Gamma^x` work into Stage 4AM derivation/convention mapping, Stage 4AN local implementation/contracts, and Stage 4AO hard GL dispersion/growth-rate validation with the GL anchor's radius convention, z-periodicity, gauge, perturbation sector, resolution, and measured growth variable called out, corrected Stage 4AG to a synthetic paired-data validator only, added Stage 4AP actual grid/ghost-cell regularity validation, added Stage 4AQ finite-axis source evaluation, shifted local RHS/evolution/freeze/exit stages to 4AR-4AU, and added Checkpoints G, H, and I. No C++ code or tests were added or run. No Python scripts, grep, ripgrep, scratch builds, smoke runs, staging, or commits were used.
- Review status: Documentation-only roadmap/checklist patch; no new future stage marked complete.

- Date: 2026-06-19
- Goal: Stage 4AM derivation lock for `hat_Gamma^x` hidden contraction and GRChombo convention map.
- Files changed: `docs/derivations/stage4AM_hatGammaX_derivation.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing. Used CodeGraph/MCP first to inspect GRChombo `CCZ4RHS`, `BSSNVars`, `CCZ4Vars`, `GammaCalculator`, `TensorAlgebra`, `CCZ4Geometry`, and the Stage 4 roadmap docs, then used direct read-only file reads for the unindexed `CCZ4UserVariables.hpp` enum and exact RHS/convention snippets. Added documentation only: locked `vars.Gamma=chris.contracted+2 Z_over_chi`, the `Z=chi Z_over_chi` translation, the hidden `tilde_Gamma^x` contraction, determinant-reduced cross-check caveat, required Stage 4AN oracles, Gamma RHS term classification, and the non-optional Stage 4AO GL gate. No C++ code or tests were added or run. No Python scripts, grep, ripgrep, scratch builds, smoke runs, staging, or commits were used.
- Review status: Stage 4AM complete as derivation-only; Stage 4AN next; Stage 4AO remains incomplete and hard-gated.

- Date: 2026-06-19
- Goal: Stage 4AN local `hat_Gamma^x` implementation and contract tests.
- Files changed: `code/BlackStringToy/CartoonHatGammaX.hpp`, `code/BlackStringToy/tests/Stage4ANHatGammaXTest.cpp`, `docs/derivations/stage4AM_hatGammaX_derivation.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the worktree was clean before editing. Used CodeGraph/MCP first to inspect the Stage 4AM derivation, Stage 4Y checked `Delta_xw` package, Stage 4X checked `B/x` package, and adjacent single-source formula patterns. Added `CartoonHatGammaX.hpp` with a non-forgeable single-source input package, checked singular ingredient consumption, reduced Christoffels, hidden multiplicity, and GRChombo `Z_over_chi` hatted convention. Added a standalone fixture covering flat `0`, constant cone `-3/4`, determinant-one off-diagonal `-1`, distinct derivative base `27/961`, hidden `-2/31`, tilde `-35/961`, hatted `926/961`, invalid-input rejection, and non-aggregate type checks. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4ANHatGammaXTest.cpp -o /tmp/blackstringtoy_stage4an_hat_gamma_x_test` then `/tmp/blackstringtoy_stage4an_hat_gamma_x_test`; it passed. No Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, staging, or commits were used.
- Review status: Stage 4AN local contract complete; Stage 4AO hard GL gate next and incomplete.

- Date: 2026-06-19
- Goal: Stage 4AO hard GL growth/dispersion validation-gate assessment.
- Files changed: `docs/derivations/stage4AM_hatGammaX_derivation.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed a clean worktree before editing. Used CodeGraph/MCP first to inspect Stage 4AM, Stage 4AN, RHS source-block coverage, and roadmap context, then direct file reads for Stage 3H planning, current live RHS scaffold behavior, initial-data/GL perturbation notes, and checklists. Outcome B was recorded: the repo does not yet contain a complete coupled linearized RHS, a locked uniform 5D black-string background/gauge convention, a project-convention GL spectrum target, or a geometric growth observable. No C++ code or tests were added or run. No Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, staging, or commits were used.
- Review status: Stage 4AO remains incomplete; Checkpoint G has not passed.

- Date: 2026-06-20
- Goal: Patch the Stage 4AO roadmap to use the revised four-part GL validation program.
- Files changed: `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `research_plan/stage_checklists.md`, `TODO.md`, `research_plan/project_outline.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/derivations/stage4AM_hatGammaX_derivation.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Confirmed the only pre-existing modifications were related Stage 4AO documentation files before editing. Used CodeGraph/MCP first to inspect Stage 4AO-adjacent roadmap/source context, then direct file reads for the roadmap/checklist/TODO/outline/fixture sections. Updated documentation only: Stage 4AO-A is the background and analytic residual lock for the exact uniform ingoing-GP black string, Stage 4AO-B is the discrete operator preflight, Stage 4AO-C is the frozen-gauge spectral gate, and Stage 4AO-D is live-gauge/full acceptance. Recorded the provisional `k_c r0 ~= 0.876`, no-eigensolver-before-4AO-B, production-Stage-4AR/4AS-blocked-until-4AO-D, geometric-observable, and Checkpoint-G-after-4AO-D rules. No C++ code or tests were added or run. No Python scripts, grep, ripgrep, staging, or commits were used.
- Review status: Stage 4AO-A through 4AO-D remain incomplete; Checkpoint G passes only after 4AO-D.

- Date: 2026-06-20
- Goal: Apply the Claude Stage 4AO audit documentation follow-up.
- Files changed: `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `research_plan/stage_checklists.md`, `TODO.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/derivations/stage4AM_hatGammaX_derivation.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `research_plan/project_outline.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first. Updated documentation only to distinguish Stage 4AO validation harnesses from production integration. Dedicated 4AO-B/C/D harnesses may evaluate actual RHS/Jacobians, solve frozen/live spectral systems, and run the seeded-eigenvector bridge; production Stage 4AR/4AS integration remains blocked until 4AO-D. Clarified 4AO-A gauge ownership for background slicing, evolution gauge-driver, and initial-gauge startup family. No C++ code, tests, scripts, `.tex`, or external files were changed. No Python scripts, grep, ripgrep, staging, or commits were used.
- Review status: Stage 4AO-A through 4AO-D remain incomplete; Checkpoint G passes only after 4AO-D.

- Date: 2026-06-20
- Goal: Complete Stage 4AO-A uniform GP background and analytic residual lock.
- Files changed: `docs/derivations/stage4AO_A_uniform_gp_background_residual.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/implementation/black_string_initial_data.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/derivations/stage4AM_hatGammaX_derivation.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo `CCZ4RHS`, `ADMConformalVars`, `BSSNVars`, `CCZ4Vars`, and `MovingPunctureGauge`, then direct reads for unindexed Stage 3/4 Markdown notes. Added documentation only: locked the uniform ingoing-GP background, compact-`z` normalization, GRChombo `K_IJ` sign, frozen-GP zero-residual targets, moving-puncture lapse startup residual, fixed field-independent GP-holding lapse source, full background `hat_Gamma^x` hidden contraction, complete component residual ledger, horizon-observable definition, positive-inner-radius domain, and analytic `1/x` cancellation table. No code, tests, scripts, `.tex`, external files, Python scripts, grep, ripgrep, staging, or commits were used.
- Review status: Stage 4AO-A documentation lock is complete; Stage 4AO-B remains next and unstarted, Checkpoint G still requires 4AO-D, and live-gauge validation is locked to the GRChombo moving-puncture family plus `S_alpha(x)=+3 sqrt(r0/x^3)` for the validation harness.

- Date: 2026-06-23
- Goal: Add final 4AO-A/4AO-B raw background-residual guard.
- Files changed: `docs/derivations/stage4AO_A_uniform_gp_background_residual.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the GRChombo moving-puncture `rhs_gauge` lapse equation. Updated documentation only: 4AO-B raw background-residual convergence must use the unmodified discrete RHS with target zero for verified geometric/scalar/constraint components and `-3 sqrt(r0/x^3)` for the unmodified live moving-puncture lapse equation. The fixed source `S_alpha=+3 sqrt(r0/x^3)` remains reserved for the stationary live-gauge 4AO-D validation harness after raw convergence is shown. No code, tests, scripts, `.tex`, external files, Python scripts, grep, ripgrep, staging, or commits were used.
- Review status: 4AO-A remains complete; 4AO-B remains next and unstarted.

- Date: 2026-06-23
- Goal: Begin and complete Stage 4AO-B discrete operator preflight without starting eigensolvers or threshold searches.
- Files changed: `code/BlackStringToy/Stage4AOGPDiscretePreflight.hpp`, `code/BlackStringToy/tests/Stage4AOBDiscreteOperatorPreflightTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/derivations/stage4AM_hatGammaX_derivation.md`, `docs/derivations/stage4AO_A_uniform_gp_background_residual.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the Stage 4AN `hat_Gamma^x` fixture style and the new 4AO-B harness context. Added a local 4AO-B harness for the locked GP background with provisional domain `r0=1`, `x in [0.5,4.0]`. It evaluates raw unmodified-RHS residual convergence without `S_alpha`, checks the live moving-puncture lapse target `-3 sqrt(r0/x^3)`, isolates the `delta hww` hidden-contraction contribution to `delta hat_Gamma^x`, compares a hand-derived actual-discrete-RHS Jacobian-vector product against finite differences, and originally included a z-independent scalar parity check later superseded by the parity blocker fix below. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AOBDiscreteOperatorPreflightTest.cpp -o /tmp/blackstringtoy_stage4aob_discrete_operator_preflight_test` then `/tmp/blackstringtoy_stage4aob_discrete_operator_preflight_test`; it passed. Raw residual errors were `0.0150518`, `0.00428846`, `0.00114621`; convergence ratios were `3.50983`, `3.74144`; hidden-contraction error was `3.47146e-15`; JVP errors were `0.00183042`, `1.82974e-07`, `4.68507e-09`, `0.000375647`. No Python scripts, grep, ripgrep, unrelated tests, scratch builds, smoke runs, scripts, external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-B local preflight complete; Stage 4AO-C remains next and unstarted; Checkpoint G remains open until 4AO-D.

- Date: 2026-06-23
- Goal: Fix only the Stage 4AO-B parity blocker found by Claude review.
- Files changed: `code/BlackStringToy/Stage4AOGPDiscretePreflight.hpp`, `code/BlackStringToy/tests/Stage4AOBDiscreteOperatorPreflightTest.cpp`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/derivations/stage4AM_hatGammaX_derivation.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the Stage 4AO-B harness and fixture. Replaced the z-independent scalar parity check with a periodic z-coupled finite-difference sub-operator using `D_z beta^z` into a scalar output, `D_z beta^x` into an `h_xz`-like one-z output, `D_z K` into a `hat_Gamma^z`/momentum-z-like one-z output, and `D_zz` preservation terms. The even sector uses cosines for scalar/zero-z/two-z variables and sines for one-z variables; the odd sector swaps those assignments. Reran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AOBDiscreteOperatorPreflightTest.cpp -o /tmp/blackstringtoy_stage4aob_discrete_operator_preflight_test` then `/tmp/blackstringtoy_stage4aob_discrete_operator_preflight_test`; it passed. Even-sector allowed norm was `7.39609` with leakage `2.05279e-17`; odd-sector allowed norm was `5.16184` with leakage `3.93873e-17`; flipped-`beta^z` negative-guard leakage was `0.79241`. No Python scripts, grep, ripgrep, unrelated tests, eigensolvers, threshold searches, scratch builds, smoke runs, scripts, external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-B parity blocker fixed; Stage 4AO-B remains complete as a local preflight; Stage 4AO-C remains unstarted.

- Date: 2026-06-23
- Goal: Begin Stage 4AO-C frozen-gauge spectral gate without adding a fake spectral harness.
- Files changed: `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/derivations/stage4AM_hatGammaX_derivation.md`, `docs/derivations/stage4AO_A_uniform_gp_background_residual.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `research_plan/stage_checklists.md`, `research_plan/project_outline.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect Stage 4AO-B, Stage 4AN, local Ricci/curvature-lapse blocks, and GRChombo-style CCZ4 RHS context. Outcome: the repo does not yet contain a complete coupled modified-cartoon CCZ4 frozen-gauge linearized RHS, radial spectral boundary conditions, actual-operator parity/JVP checks, a linearized MOTS map to `delta R_H`, shift-invert or equivalent targeted spectral extraction, unstable/stable points, radial/boundary convergence, or a primary-source `k_c r0` convention map. Added documentation only recording the intended frozen-gauge perturbation vector and blockers. No C++ code or tests, eigensolvers, threshold searches, Python scripts, grep, ripgrep, scripts, external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-C is started but blocked/incomplete; Checkpoint G remains pending until 4AO-D.

- Date: 2026-06-23
- Goal: Continue Stage 4AO-C with a read-only reuse inventory before any eigensolver/operator implementation.
- Files changed: `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect PETSc/AHFinder, solver, derivative, interpolation, boundary, local RHS-helper, and Stage 4AO-B context, then direct read-only file reads of the relevant project and `external/GRChombo` headers/implementations. Recorded that no SLEPc/PETSc `EPS`/project eigensolver exists; PETSc is present only through AHFinder `SNES/KSP`; AHFinder can be adapted for nonlinear MOTS diagnostics but lacks the `R_H` postprocessor and linearized MOTS response; GRChombo derivative/boundary/interpolation/reduction/logging utilities are adapter candidates; the current BlackStringToy RHS is inherited public CCZ4 plus smoke-hidden behavior, not the modified-cartoon frozen-gauge GL operator. No C++ code or tests, eigensolver, shift-invert, threshold search, Python scripts, grep, ripgrep, scripts, external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-C remains blocked/incomplete. Next recommended work is the validation-only frozen-gauge operator wrapper plus boundary-condition contract, then actual-operator JVP/parity checks and the linearized-MOTS adapter before any eigensolver adapter.

- Date: 2026-06-23
- Goal: Add the Stage 4AO-C validation-only frozen-gauge operator wrapper and boundary-condition contract.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`, `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`, `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`, `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, `docs/derivations/unit_test_fixture_design.md`, `docs/implementation/minimal_cpp_implementation_plan.md`, `docs/implementation/stage4H_ricci_rhs_compatibility.md`, `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the Stage 4AO-B harness, Stage 4AN hatted-connection context, and the existing Stage 4AO-C reuse inventory. Added a contract-only wrapper for the frozen-gauge perturbation vector, explicit gauge perturbation exclusions, per-variable RHS inventory labels, the `0<x_in<r0<x_out` radial boundary contract, and TODO hooks for actual-operator JVP/parity, convergence, boundary-location, and linearized-MOTS checks. Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp -o /tmp/blackstringtoy_stage4aoc_frozen_gauge_operator_contract_test` then `/tmp/blackstringtoy_stage4aoc_frozen_gauge_operator_contract_test`; it passed. No eigensolver, shift-invert, threshold search, `k_c r0` search, Python scripts, grep, ripgrep, unrelated tests, scripts, external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-C remains incomplete. The wrapper/boundary-contract substep is complete, but the full frozen-gauge operator, actual-operator JVP/parity tests, boundary validation, linearized MOTS map, spectral method, threshold/stable/unstable points, and convergence battery remain blockers before eigensolver work.

- Date: 2026-06-23
- Goal: Begin the next Stage 4AO-C substep by adding the first actual
  frozen-gauge operator block, GP-shift advection, without starting
  eigensolver work.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAdvectionBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the existing
  Stage 4AO-C wrapper, contract fixture, and related 4AO-B/4AO-C context.
  Added a validation-only matrix-free apply path for
  `beta_GP^x d_x(delta u)`, with `beta_GP^x=sqrt(r0/x)`, on all 13
  frozen-gauge variables. Added interior second-order centered radial `d_x`
  and periodic second-order `D_z`/`D_zz` scaffolding; endpoints remain
  placeholder outputs and boundary validation is still missing. Compiled and
  ran `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy
  code/BlackStringToy/tests/Stage4AOCFrozenGaugeAdvectionBlockTest.cpp -o
  /tmp/blackstringtoy_stage4aoc_frozen_gauge_advection_block_test` then
  `/tmp/blackstringtoy_stage4aoc_frozen_gauge_advection_block_test`; it
  passed, including wrong-beta, wrong-sign, and wrong-order negative guards.
  Also compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I
  code/BlackStringToy
  code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp -o
  /tmp/blackstringtoy_stage4aoc_frozen_gauge_operator_contract_test` then
  `/tmp/blackstringtoy_stage4aoc_frozen_gauge_operator_contract_test`; it
  passed. No Python scripts, grep, ripgrep, unrelated tests, eigensolvers,
  shift-invert, threshold searches, `k_c r0` searches, 4AO-D work, production
  RHS/evolution wiring, scripts, external GRChombo changes, staging, or
  commits were used.
- Review status: Stage 4AO-C remains incomplete. Only GP-shift advection is
  implemented; the full frozen-gauge modified-cartoon CCZ4 operator,
  actual-operator JVP/parity checks, boundary validation, linearized MOTS map,
  spectral method, threshold/stable/unstable points, and convergence battery
  remain blockers.

- Date: 2026-06-23
- Goal: Add the next Stage 4AO-C partial operator block: tensor
  shift-stretching for conformal metric and trace-free tensor perturbations.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTensorShiftStretchingBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the GRChombo
  `CCZ4RHS.impl.hpp` shift/stretching convention and the existing Stage 4AO-C
  wrapper/tests. Added an explicit `tensor_shift_stretching` RHS inventory
  category and a validation-only apply path for the non-advection tensor
  stretch terms. On the locked GP background, the implemented coefficients
  are `-7 lambda/4` for `h_xx/A_xx`, `-5 lambda/4` for `h_xz/A_xz`,
  `-3 lambda/4` for `h_zz/A_zz`, and hidden `+5 lambda/4` for `h_ww/A_ww`.
  Scalars and hatted-Gamma slots get zero from this block. Compiled and ran
  `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy
  code/BlackStringToy/tests/Stage4AOCFrozenGaugeTensorShiftStretchingBlockTest.cpp
  -o /tmp/blackstringtoy_stage4aoc_frozen_gauge_tensor_shift_stretching_block_test`
  then
  `/tmp/blackstringtoy_stage4aoc_frozen_gauge_tensor_shift_stretching_block_test`;
  it passed, including guards for hidden stretching, d=4 trace coefficient,
  `partial_x beta^x` sign, and scalar/hatted-Gamma exclusion. Also reran the
  Stage 4AO-C contract and advection fixtures because shared inventory/apply
  types changed; both passed. No Python scripts, grep, ripgrep, unrelated
  tests, eigensolvers, shift-invert, threshold searches, `k_c r0` searches,
  4AO-D work, production RHS/evolution wiring, scripts, external GRChombo
  changes, staging, or commits were used.
- Review status: Stage 4AO-C remains incomplete. Implemented pieces are now
  GP-shift advection plus tensor shift-stretching only; metric `-2 A_IJ`
  coupling, K/A algebraic dynamics, Ricci/curvature, Theta/constraint terms,
  hatted-Gamma evolution, full-operator JVP/parity tests, boundary validation,
  linearized MOTS map, spectral method, threshold/stable/unstable points, and
  convergence battery remain blockers.

- Date: 2026-06-23
- Goal: Add the next Stage 4AO-C partial operator block: algebraic
  metric/conformal-factor couplings.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAlgebraicCouplingBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the GRChombo
  `CCZ4RHS.impl.hpp` conventions for the `-2 alpha A_ij` metric term and
  `(2/GR_SPACEDIM) chi (alpha K - div beta)` chi term. Added an explicit
  `algebraic_metric_chi_coupling` RHS inventory category and a
  validation-only apply path for
  `delta h_IJ <- -2 delta A_IJ`, `delta chi <- +(1/2) delta K`, using `d=4`,
  `alpha_GP=1`, `chi_GP=1`, and `K_GP=div beta_GP`. Compiled and ran
  `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy
  code/BlackStringToy/tests/Stage4AOCFrozenGaugeAlgebraicCouplingBlockTest.cpp
  -o /tmp/blackstringtoy_stage4aoc_frozen_gauge_algebraic_coupling_block_test`
  then
  `/tmp/blackstringtoy_stage4aoc_frozen_gauge_algebraic_coupling_block_test`;
  it passed, including negative guards for wrong metric sign/coefficient,
  wrong `d=3` chi coefficient, reciprocal `A_IJ <- h_IJ`, and accidental
  `K`/`Theta`/hatted-Gamma outputs. Also reran the Stage 4AO-C contract,
  advection, and tensor shift-stretching fixtures because the shared wrapper
  and inventory changed; all passed. No Python scripts, grep, ripgrep,
  unrelated tests, eigensolvers, shift-invert, threshold searches, `k_c r0`
  searches, 4AO-D work, production RHS/evolution wiring, scripts, external
  GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-C remains incomplete. Implemented pieces are now
  GP-shift advection, tensor shift-stretching, and algebraic metric/chi
  coupling only; remaining K/A algebraic dynamics, Ricci/curvature,
  Theta/constraint terms, hatted-Gamma evolution, full-operator JVP/parity
  tests, boundary validation, linearized MOTS map, spectral method,
  threshold/stable/unstable points, and convergence battery remain blockers.

- Date: 2026-06-23
- Goal: Historical, superseded attempt to add a Stage 4AO-C K-equation
  `A^2/K^2` linearization. The 2026-07-20 correction identifies this as the
  rejected `USE_BSSN` branch, not the locked project `USE_CCZ4` row.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeKAlgebraicBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo
  `CCZ4RHS.impl.hpp`, which forms `tr_A2` from
  `A_UU=raise_all(vars.A,h_UU)` and `compute_trace(vars.A,A_UU)`, then uses
  `lapse*(tr_A2+K^2/GR_SPACEDIM)` in the BSSN K RHS. A direct read of
  `TensorAlgebra.hpp` confirmed `raise_all` raises both indices with the
  conformal inverse. Added an explicit `k_equation_algebraic_a2_k2` RHS
  inventory category and a validation-only K-output apply path for the locked
  GP-background linearization, including inverse-metric variation and hidden
  `ww` multiplicity. Compiled and ran `g++ -std=c++17 -Wall -Wextra
  -pedantic -I code/BlackStringToy
  code/BlackStringToy/tests/Stage4AOCFrozenGaugeKAlgebraicBlockTest.cpp -o
  /tmp/blackstringtoy_stage4aoc_frozen_gauge_k_algebraic_block_test` then
  `/tmp/blackstringtoy_stage4aoc_frozen_gauge_k_algebraic_block_test`; it
  passed, including negative guards for missing inverse-metric variation,
  missing hidden multiplicity, wrong `d=3` K-squared coefficient, spurious
  `h_xz`/`A_xz` terms, and non-K outputs. Also reran the Stage 4AO-C contract
  fixture because the shared wrapper and inventory changed; it passed. No
  Python scripts, grep, ripgrep, unrelated tests, eigensolvers, shift-invert,
  threshold searches, `k_c r0` searches, 4AO-D work, production RHS/evolution
  wiring, scripts, external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-C remains incomplete. Implemented pieces are now
  GP-shift advection, tensor shift-stretching, algebraic metric/chi coupling,
  and K-output `A^2/K^2` algebraic linearization only. Ricci/curvature and
  lapse-Hessian/frozen-lapse pieces, remaining A-equation algebraic dynamics,
  Theta/constraint terms, hatted-Gamma evolution, full-operator JVP/parity
  tests, boundary validation, linearized MOTS map, spectral method,
  threshold/stable/unstable points, and convergence battery remain blockers.

- Date: 2026-06-24
- Goal: Add the next Stage 4AO-C partial operator block: A-equation
  algebraic non-curvature linearization.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAAlgebraicBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo
  `CCZ4RHS.impl.hpp`, which sets
  `rhs.A[i][j]=advec.A[i][j]+Adot_TF[i][j]+A_ij*(alpha*(K-2Theta)
  -(2/GR_SPACEDIM)*divshift)`, then adds shift stretching and
  `-2*alpha*h_UU[k][l]*A[i][k]*A[l][j]`. Added an explicit
  `a_equation_algebraic_non_curvature` RHS inventory category and a
  validation-only A-output apply path for
  `(K - 2Theta)A_IJ - 2 h^KL A_IK A_LJ` on the locked GP background. The
  block includes conformal inverse-metric variation, direct K/Theta
  coefficients where `A_IJ_GP` is nonzero, zero `A_xz` K/Theta coupling, and
  no hidden multiplicity factor on the componentwise `A_ww` equation.
  Compiled and ran `g++ -std=c++17 -Wall -Wextra -pedantic -I
  code/BlackStringToy
  code/BlackStringToy/tests/Stage4AOCFrozenGaugeAAlgebraicBlockTest.cpp -o
  /tmp/blackstringtoy_stage4aoc_frozen_gauge_a_algebraic_block_test` then
  `/tmp/blackstringtoy_stage4aoc_frozen_gauge_a_algebraic_block_test`; it
  passed, including negative guards for missing inverse-metric variation,
  wrong hidden multiplicity on `A_ww`, wrong K/Theta coefficients, spurious
  `A_xz` K/Theta couplings, and non-A outputs. Also reran the Stage 4AO-C
  contract, K algebraic, algebraic metric/chi coupling, tensor
  shift-stretching, and GP-shift advection fixtures because the shared wrapper
  and inventory changed; all passed. No Python scripts, grep, ripgrep,
  unrelated tests, eigensolvers, shift-invert, threshold searches,
  `k_c r0` searches, 4AO-D work, production RHS/evolution wiring, scripts,
  external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-C remains incomplete. Implemented pieces are now
  GP-shift advection, tensor shift-stretching, algebraic metric/chi coupling,
  K-output `A^2/K^2` algebraic linearization, and A-output non-curvature
  algebraic linearization only. The trace-free Ricci/lapse-Hessian curvature
  block for `A_IJ`, Ricci/curvature and lapse-Hessian/frozen-lapse pieces in
  other equations, Theta/constraint terms, hatted-Gamma evolution,
  full-operator JVP/parity tests, boundary validation, linearized MOTS map,
  spectral method, threshold/stable/unstable points, convergence battery, and
  `k_c r0` convention map remain blockers.

- Date: 2026-06-24
- Goal: Add the next Stage 4AO-C partial operator block: Theta-equation
  algebraic non-Ricci linearization.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaAlgebraicBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo
  `CCZ4RHS.impl.hpp`, then directly read the Theta RHS range. The CCZ4 branch
  uses `0.5*lapse*(ricci.scalar - tr_A2
  + ((GR_SPACEDIM-1)/GR_SPACEDIM)*K^2 - 2*Theta*K)` plus separate damping,
  `Z_dot_d1lapse`, and cosmological terms. Added an explicit
  `theta_equation_algebraic_non_ricci` RHS inventory category and a
  validation-only Theta-output apply path for
  `0.5*(((d-1)/d)K^2 - A_IJ A^IJ)` on the locked GP background. The block
  includes conformal inverse-metric variation, hidden `ww` trace multiplicity
  two, zero `h_xz/A_xz` contribution, and the `d=4` K coefficient. Ricci
  scalar, `-Theta K`, Z4 damping/constraint terms, `Z dot grad alpha`, and
  cosmological terms remain missing. Compiled and ran `g++ -std=c++17 -Wall
  -Wextra -pedantic -I code/BlackStringToy
  code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaAlgebraicBlockTest.cpp
  -o /tmp/blackstringtoy_stage4aoc_frozen_gauge_theta_algebraic_block_test`
  then `/tmp/blackstringtoy_stage4aoc_frozen_gauge_theta_algebraic_block_test`;
  it passed, including negative guards for missing inverse-metric variation,
  missing hidden multiplicity, wrong `d=3` K coefficient, spurious
  `h_xz/A_xz` terms, and non-Theta outputs. Also reran the Stage 4AO-C
  contract, A algebraic, K algebraic, algebraic metric/chi coupling, tensor
  shift-stretching, and GP-shift advection fixtures because the shared wrapper
  and inventory changed; all passed. No Python scripts, grep, ripgrep,
  unrelated tests, eigensolvers, shift-invert, threshold searches,
  `k_c r0` searches, 4AO-D work, production RHS/evolution wiring, scripts,
  external GRChombo changes, staging, or commits were used.
- Review status: Stage 4AO-C remains incomplete. Implemented pieces are now
  GP-shift advection, tensor shift-stretching, algebraic metric/chi coupling,
  K-output `A^2/K^2` algebraic linearization, A-output non-curvature
  algebraic linearization, and Theta-output non-Ricci algebraic linearization
  only. Ricci scalar contribution to Theta, Z4 damping/constraint terms,
  trace-free Ricci/lapse-Hessian curvature block for `A_IJ`, Ricci/curvature
  and lapse-Hessian/frozen-lapse pieces in other equations, hatted-Gamma
  evolution, trace-free `delta A` subspace enforcement/projector,
  full-operator JVP/parity tests, boundary validation, linearized MOTS map,
  spectral method, threshold/stable/unstable points, convergence battery, and
  `k_c r0` convention map remain blockers.

- Date: 2026-06-24
- Goal: Add the next Stage 4AO-C partial operator block: Theta
  `-K_GP deltaTheta` algebraic linearization.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaMinusKBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the GRChombo
  `CCZ4RHS.impl.hpp` Theta RHS convention. The CCZ4 branch contains
  `0.5*lapse*(... - 2*Theta*K ...)` with damping and `Z_dot_d1lapse`
  outside this simple algebraic factor. Added an explicit
  `theta_equation_minus_k_delta_theta` RHS inventory category and a
  validation-only Theta-output apply path for
  `delta Theta_RHS <- -(3 lambda/2) deltaTheta`. Compiled and ran the new
  Stage 4AO-C Theta minus-K fixture, then reran the Stage 4AO-C contract and
  existing partial-block fixtures because the shared wrapper and inventory
  changed; all passed.
- Review status: Stage 4AO-C remains incomplete. Implemented pieces are now
  GP-shift advection, tensor shift-stretching, algebraic metric/chi coupling,
  K-output `A^2/K^2`, A-output non-curvature algebraic, Theta-output
  non-Ricci algebraic, and Theta-output `-K_GP deltaTheta` blocks only.
  Ricci scalar contribution to Theta, Z4 damping/constraint terms and
  `kappa1/kappa2` convention lock, trace-free Ricci/lapse-Hessian curvature
  block for `A_IJ`, hatted-Gamma evolution, full-operator JVP/parity tests,
  boundary validation, linearized MOTS map, spectral method,
  threshold/stable/unstable points, convergence battery, and the `k_c r0`
  convention map remain blockers.

- Date: 2026-06-24
- Goal: Add the next Stage 4AO-C structural contract: trace-free `delta A`
  subspace/projector handling before full-operator assembly.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTraceFreeDeltaAProjectorTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo
  `TensorAlgebra::make_trace_free` and the `CCZ4RHS.impl.hpp` use on
  `Adot_TF`. Added `delta_trace_a_at_point` and a validation-only projector
  that subtracts `delta_tr_A/4` from `A_xx`, `A_zz`, and `A_ww` while keeping
  the 13-variable frozen-gauge state unchanged. Compiled and ran the new
  trace-free projector fixture, then reran the Stage 4AO-C contract and
  existing partial-block fixtures because the shared wrapper changed; all
  passed.
- Review status: Stage 4AO-C remains incomplete. The projector is required
  before any assembled full-operator JVP/parity/spectral claim, but it does
  not implement Ricci/curvature, lapse-Hessian, Theta Ricci scalar, Z4
  damping, hatted-Gamma evolution, boundary validation, MOTS mapping,
  eigensolvers, threshold searches, convergence batteries, or `k_c r0`
  convention mapping.

- Date: 2026-06-24
- Goal: Begin the next Stage 4AO-C substep: linearized Ricci/curvature design
  preflight before implementing any `delta R_IJ` component.
- Files changed: `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `research_plan/stage_checklists.md`, `research_plan/project_outline.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo
  `CCZ4Geometry` and the repo-owned Ricci/helper layers. Directly inspected
  the relevant GRChombo `CCZ4Geometry`, `CCZ4RHS`, Stage 4G, Stage 4AC,
  Stage 4AE, and Stage 4AL artifacts. Documented the reuse table, frozen-gauge
  curvature simplification, Ricci decomposition contract, oracle plan, and
  recommended first `delta R_ww[gamma]` target.
- Review status: Documentation/design only. No Ricci code, tests, eigensolver,
  shift-invert, threshold search, `k_c r0` search, MOTS map, 4AO-D work,
  production RHS/evolution wiring, scripts, external GRChombo changes,
  staging, or commits were used. Stage 4AO-C remains incomplete.

- Date: 2026-06-24
- Goal: Begin the first Stage 4AO-C Ricci implementation substep: raw hidden
  physical `delta R_ww[gamma]` on the locked frozen-GP background only.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeHiddenRww.hpp`,
  `code/BlackStringToy/tests/Stage4AOCHiddenPhysicalDeltaRwwTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`,
  `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the Stage 4G
  metric-derivative physical Ricci path, Stage 4AC/4AE/4AH hidden `R_ww`
  split/direct helpers, and GRChombo's lower/lower physical Ricci convention.
  Added the frozen-GP linearized raw hidden component formula and a fixture
  that central-differences both the Stage 4G direct oracle and Stage 4AH split
  oracle over an epsilon sweep. Covered zero perturbation, pure
  `delta h_ww`, pure `delta chi`, z-dependent scalar hidden data, radial
  visible-metric data, and mixed radial/z off-diagonal data. The stable
  `1e-5` to `1e-6` plateau passed below `1e-7` for both oracle paths.
- Review status: Stage 4AO-C remains incomplete. This implements only raw
  hidden `delta R_ww[gamma]`; visible Ricci components, trace-free
  A-curvature source, Theta Ricci scalar, full operator JVP/parity tests,
  boundary validation, MOTS map, eigensolver/shift-invert, threshold search,
  convergence battery, `k_c r0` map, 4AO-D, and production wiring remain
  missing.

- Date: 2026-06-24
- Goal: Begin the next Stage 4AO-C Ricci implementation substep: raw visible
  physical `delta R_xz[gamma]` on the locked frozen-GP background only.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxz.hpp`,
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxzTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`,
  `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect Stage 4G's
  metric-derivative physical Ricci path, the Stage 4AO-C Ricci preflight, and
  GRChombo's lower/lower physical Ricci convention. Added a validation-only
  raw `delta R_xz` helper. The Stage 4G finite-difference physical Ricci
  engine is the primary and sole independent oracle for this visible component
  because there is no Stage 4AC/4AE split path for visible `R_xz`. The fixture
  covers zero input, pure `delta h_xz` as a zero oracle, an even-sector
  diagonal scalar mode, a radial/z mixed hidden mode, and a pure
  conformal-factor mixed-derivative mode. The stable `1e-5` to `1e-6` epsilon
  plateau passed below `1e-7`; the one-z parity projection produced allowed
  sine amplitude `6` and forbidden/allowed leakage `3.5851e-17`.
- Review status: Stage 4AO-C remains incomplete. This implements only raw
  visible `delta R_xz[gamma]`; `delta R_xx`, `delta R_zz`, trace-free
  A-curvature source, Theta Ricci scalar, full operator JVP/parity tests,
  boundary validation, MOTS map, eigensolver/shift-invert, threshold search,
  convergence battery, `k_c r0` map, 4AO-D, and production wiring remain
  missing.

- Date: 2026-06-24
- Goal: Resume the Stage 4AO-C raw visible physical `delta R_zz[gamma]`
  implementation substep after resolving the conformal-factor sign convention.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRzz.hpp`,
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRzzTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`,
  `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect Stage 4G's
  metric-derivative physical Ricci path, `CartoonRicciBridge` lower/lower
  access, GRChombo `CCZ4Geometry::compute_ricci_Z`, and the existing raw
  `delta R_ww` / `delta R_xz` helper styles. Implemented raw visible
  `delta R_zz[gamma]` only. Because Stage 4G uses `gamma=h/chi`, the
  conformal-factor terms have positive signs. The Stage 4G finite-difference
  physical Ricci engine is the sole independent oracle for this visible
  component. The fixture covers zero input, pure `delta h_zz`, one-z
  `delta h_xz`, z-dependent `delta h_ww`, mixed `delta chi`, z-dependent
  `delta h_xx`, and mixed radial/z behavior. The `1e-5` to `1e-6` epsilon
  plateau passed below `1e-7`; even-parity checks gave forbidden/allowed
  leakage `2.00457e-17` for scalar diagonal input and `3.85494e-17` for
  one-z `h_xz` input.
- Review status: Stage 4AO-C remains incomplete. This implements only raw
  visible `delta R_zz[gamma]`; `delta R_xx`, trace-free A-curvature source,
  Theta Ricci scalar, full operator JVP/parity tests, boundary validation,
  MOTS map, eigensolver/shift-invert, threshold search, convergence battery,
  `k_c r0` map, 4AO-D, and production wiring remain missing.

- Date: 2026-06-24
- Goal: Begin the next Stage 4AO-C Ricci implementation substep: raw visible
  physical `delta R_xx[gamma]` on the locked frozen-GP background only.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxx.hpp`,
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxxTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `research_plan/project_outline.md`, `research_plan/stage_checklists.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect Stage 4G's
  metric-derivative physical Ricci path, `CartoonRicciBridge` lower/lower
  access, GRChombo `CCZ4Geometry` lower/lower Ricci convention, and the
  existing raw `delta R_ww`, `delta R_xz`, and `delta R_zz` helper styles.
  Implemented raw visible `delta R_xx[gamma]` only:
  `partial_xz(delta h_xz) - 0.5 partial_xx(delta h_zz)
  - 0.5 partial_zz(delta h_xx) + partial_x(delta h_xx)/x
  - 2 partial_x(delta h_ww)/x - partial_xx(delta h_ww)
  + partial_x(delta chi)/x + 1.5 partial_xx(delta chi)
  + 0.5 partial_zz(delta chi)`. The positive `chi` signs follow
  `gamma=h/chi`.
- Test result: compiled and ran
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxxTest.cpp`.
  The Stage 4G finite-difference physical Ricci engine is the sole independent
  oracle for this visible component. The `1e-5` to `1e-6` epsilon plateau
  passed below `1e-7` across zero input, pure `delta h_xx`, pure
  `delta h_zz`, pure `delta h_ww`, one-z `delta h_xz`, mixed `delta chi`,
  z-dependent diagonal scalar input, and mixed radial/z cases. Even-parity
  checks gave forbidden/allowed leakage `2.00457e-17` for scalar diagonal
  input and `4.85723e-17` for one-z `h_xz` input.
- Review status: Stage 4AO-C remains incomplete. This implements only raw
  visible `delta R_xx[gamma]`; raw Ricci trace/trace-free assembly,
  trace-free A-curvature source, Theta Ricci scalar, full operator JVP/parity
  tests, boundary validation, MOTS map, eigensolver/shift-invert, threshold
  search, convergence battery, `k_c r0` map, 4AO-D, and production wiring
  remain missing.

- Date: 2026-06-24
- Goal: Begin the next Stage 4AO-C Ricci substep: raw Ricci trace and
  trace-free assembly from the validated raw `delta R_xx`, `delta R_xz`,
  `delta R_zz`, and `delta R_ww` blocks.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeRicciAssembly.hpp`,
  `code/BlackStringToy/tests/Stage4AOCRicciTraceFreeAssemblyTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `research_plan/project_outline.md`, `research_plan/stage_checklists.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo
  trace-free convention, `CartoonRicciBridge` lower/lower physical Ricci
  access, the Stage 4AO-C trace-free `delta A` convention, and the raw
  `delta R_ww`, `delta R_xz`, `delta R_zz`, and `delta R_xx` helpers. Added
  the standalone trace assembly `delta R=delta R_xx+delta R_zz+2delta R_ww`
  and `d=4` trace-free projection after full raw assembly. The fixture
  central-differences the Stage 4G nonlinear Ricci engine over an epsilon
  sweep and validates zero, pure component, pure `chi`, mixed scalar, and
  parity cases. The stable middle epsilon plateau passed below `2e-7`.
- Review status: Stage 4AO-C remains incomplete. This substep does not insert
  `[delta R_IJ]^TF` into the `A_IJ` curvature source, does not insert
  `0.5 delta R` into the Theta Ricci scalar term, and does not add full
  operator JVP/parity tests, boundary validation, MOTS map, eigensolver,
  threshold search, convergence battery, 4AO-D, or production wiring.

- Date: 2026-06-24
- Goal: Begin the next Stage 4AO-C substep: insert the Ricci scalar
  contribution `0.5 delta R` into the validation-only frozen-gauge Theta RHS.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaRicciScalarBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `research_plan/project_outline.md`, `research_plan/stage_checklists.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect the
  GRChombo-facing Theta convention, the raw Ricci trace/free assembly helper,
  and the existing Theta algebraic/minus-K blocks. Added a partial operator
  block that accepts only the `TraceFreeRicciAssembly` result and writes
  `output[Theta] += 0.5 * scalar_trace()`. The focused test verifies the
  positive sign, factor `0.5`, hidden `ww` multiplicity inherited from the
  trace assembly, no `R_xz` trace contribution, no non-Theta output writes,
  and incomplete-operator/eigensolver guards. The operator contract test now
  records the Theta Ricci insertion as implemented only for the Theta output.
- Review status: Stage 4AO-C remains incomplete. This substep does not insert
  `[delta R_IJ]^TF` into the `A_IJ` curvature source, does not implement Z4
  damping/kappa terms, hatted-Gamma evolution, actual full-operator JVP/parity
  tests, boundary validation, MOTS map, eigensolver, threshold search,
  convergence battery, 4AO-D, or production wiring.

- Date: 2026-06-24
- Goal: Begin the next Stage 4AO-C substep: insert
  `[delta R_IJ]^TF` into the validation-only frozen-gauge `A_IJ` curvature
  source.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`,
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`,
  `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `docs/derivations/unit_test_fixture_design.md`,
  `docs/implementation/minimal_cpp_implementation_plan.md`,
  `docs/implementation/stage4H_ricci_rhs_compatibility.md`,
  `docs/implementation/stage4_hidden_sphere_Rww_plan.md`,
  `research_plan/project_outline.md`, `research_plan/stage_checklists.md`,
  `TODO.md`, `logs/PROJECT_LOG.md`, `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect GRChombo's
  `CCZ4RHS` A-equation convention and the existing Stage 4AO-C Ricci assembly
  helper. Added a partial operator block that accepts only
  `TraceFreeRicciAssembly` and writes `tf_xx`, `tf_xz`, `tf_zz`, and `tf_ww`
  into the corresponding `A_IJ` output slots. In frozen gauge,
  `delta(D_I D_J alpha)=0` and `delta(chi alpha R_IJ)=delta R_IJ`, so the
  insertion has prefactor one.
- Test result: compiled and ran
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`
  and `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`.
  The focused test checks pointwise/grid insertion, no non-A output writes,
  trace-free source residual, `A_xz` equal to raw `R_xz`, no double hidden
  multiplicity on `A_ww`, and negative guards for wrong sign, raw-Ricci
  insertion, `d=3` projection, double `A_ww`, and false completion.
- Review status: Stage 4AO-C remains incomplete. This substep does not
  implement Z4 damping/kappa terms, hatted-Gamma evolution, actual
  full-operator JVP/parity tests, boundary validation, MOTS map, eigensolver,
  threshold search, convergence battery, 4AO-D, or production wiring.

- Date: 2026-06-25
- Goal: Begin the Stage 4AO-C Z4 damping / kappa convention-lock pass.
- Files changed: `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`,
  `research_plan/stage_checklists.md`, `TODO.md`, `logs/PROJECT_LOG.md`,
  `logs/CODEX_LOG.md`.
- Validation performed: Used CodeGraph/MCP first to inspect
  `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`,
  `CCZ4RHS.hpp`, `SimulationParametersBase.hpp`, `MovingPunctureGauge.hpp`,
  `IntegratedMovingPunctureGauge.hpp`, `CCZ4Geometry.hpp`, and the local
  Stage 2 smoke parameters. Documented the exact GRChombo kappa terms and
  frozen-GP linearized targets.
- Review status: Documentation/derivation only. No kappa values were chosen,
  no code/tests were added, and Stage 4AO-C remains incomplete until the human
  team locks the `kappa1/kappa2` convention and the remaining damping,
  hatted-Gamma, full-operator, boundary, MOTS, spectral, convergence, and
  `k_c r0` gates are implemented.

- Date: 2026-07-20
- Goal: Correct the Stage 4AO-C K equation to the locked `USE_CCZ4`
  formulation.
- Files changed: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`;
  replaced `code/BlackStringToy/tests/Stage4AOCFrozenGaugeKAlgebraicBlockTest.cpp`
  with `code/BlackStringToy/tests/Stage4AOCFrozenGaugeKCCZ4BlockTest.cpp`;
  updated the Stage 4AO-C gate, fixture inventory, hidden-sphere roadmap,
  checklist, TODO, and project/Codex logs.
- Correction: removed the former BSSN K row `A_IJ A^IJ + K^2/d`, including
  all direct `delta A_IJ` and inverse-metric coefficients. Implemented only
  `output[K] += 3 lambda input[K] - 3 lambda input[Theta]` and the separate
  physical-Ricci insertion
  `output[K] += delta R_xx + delta R_zz + 2 delta R_ww`.
- Validation performed: Used CodeGraph first. The replacement fixture
  central-differences nonlinear Stage 4G physical Ricci plus independently
  perturbed `K(K-2Theta)` over epsilons `1e-2`, `1e-4`, `1e-5`, `1e-6`, and
  `1e-7`; the `1e-5`/`1e-6` plateau tolerance is `2e-7`. Pure `delta K`,
  pure `delta Theta`, pure `delta A`, metric/chi, pure `delta h_xz`, and
  combined cases all discriminate against a separately finite-differenced
  nonlinear BSSN oracle.
- Final validation: compiled and ran all 16 current `Stage4AOC*.cpp` fixtures
  with `g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy`; all
  passed. `git diff --check` passed, and diffs under `external/GRChombo` and
  `scripts` were empty.
- Review status: Only the selected-CCZ4 K/Theta and physical-`delta R` pieces
  are implemented. Z/hat-Gamma-dependent Ricci, kappa damping, hat-Gamma
  evolution, actual full-operator JVP/parity, boundaries, MOTS, eigensolver,
  threshold, and convergence remain blockers. Frozen-gauge lapse-Hessian
  variation vanishes and locked `Lambda=0` leaves no cosmological term.
  Stage 4AO-C remains incomplete and the eigensolver remains disallowed.

- Date: 2026-07-21
- Goal: Begin the Stage 4AO-C selected-CCZ4 damping insertion substep.
- Initial state: clean `HEAD` at `6c0b306`, containing the audited CCZ4 K
  correction; no unrelated changes.
- Convention decision: implement the planning-layer lock
  `kappa1=0.1`, `kappa2=0`, `kappa3=1`, `covariantZ4=true` for the main
  frozen-gauge validation path. Zero damping is reserved for a later
  diagnostic comparison; no tuning was performed and the Stage 2 smoke
  parameter file was not modified.
- Implementation: added only the simple K/Theta damping insertion
  `output[Theta] += -0.25 input[Theta]` and
  `output[K] += -0.4 input[Theta]`, derived from the general `d=4` GRChombo
  coefficients. No `deltaK`, hatted-Gamma, or other output contribution was
  added. The corrected `+delta R + 3 lambda deltaK - 3 lambda deltaTheta` K
  pieces remain unchanged.
- Deferred target: complete hatted-Gamma evolution, including
  `delta rhs_hat_Gamma^i|Z/kappa = -0.2 delta Z_over_chi^i` at `kappa3=1`
  and the `kappa3` shift-gradient term
  `-(chris.contracted + 2 kappa3 Z_over_chi) * d1.shift`.
- Tests: added an independent coefficient/negative-guard fixture, updated the
  operator inventory contract, and compiled/reran all 17 Stage 4AO-C tests;
  all passed with the requested warning flags. `git diff --check` and the
  protected-path checks passed.
- Review status: K, Theta, and the full frozen-gauge operator remain
  incomplete; the eigensolver remains disallowed. Stage 4AO-C, 4AO-D, and
  Checkpoint G remain incomplete.

- Date: 2026-07-21
- Goal: Begin the Stage 4AO-C hatted-Gamma evolution design preflight.
- Initial state: clean committed damping checkpoint `62e5e9e`; CodeGraph used
  before direct source inspection.
- Result: mapped the exact selected GRChombo Gamma equation and derived both
  frozen-GP linearized rows, including hidden multiplicity two, Z reconstruction,
  locked damping, `kappa3` shift-gradient coupling, parity, and one-time common
  advection ownership. The selected momentum-constraint form contains no
  derivative-`A_IJ` term; the design records this explicitly.
- Validation design: independent nonlinear visible/4AN finite differences plus
  analytic hidden connection-A/vector-Hessian oracles, epsilon plateau, parity
  leakage, mutation guards, and zero background residual.
- Next narrow target: x/z contracted-connection and Z reconstruction helper,
  then the non-advection Z/kappa plus `kappa3` shift-gradient block. No code,
  tests, eigensolver/JVP/boundary/MOTS/4AO-D/production wiring was added.

- Date: 2026-07-21
- Goal: Implement the Stage 4AO-C contracted-connection/Z reconstruction
  helper without adding Gamma RHS output.
- Implementation: added explicit hidden-multiplicity-two `g_x`, full `g_z`,
  and `Z_i=0.5(H_i-g_i)` in a validation-only non-RHS helper. No determinant
  constraint is assumed.
- Validation: the focused fixture checks nonlinear central-difference x/z
  oracles, stable epsilon plateaus, pure metric and Z cases, parity, hidden
  multiplicity, derivative signs, optional determinant-reduced identities,
  zero input, and closed completion/eigensolver gates.
- Test result: focused helper, operator contract, and Stage 4AN tests passed;
  all 18 current Stage 4AO-C fixtures compiled and passed without warnings.
  Diff and protected-path checks passed.
- Remaining Gamma work: non-advection Z/kappa and `kappa3` shift-gradient,
  connection-A, vector-Hessian, K/Theta/chi gradients, complete row assembly,
  and full-operator validation.

- Date: 2026-07-21
- Goal: Implement the first Stage 4AO-C hatted-Gamma RHS block without
  duplicating common advection or adding later Gamma families.
- Recovery: CodeGraph was used first. The worktree contained only the current
  4AO-C design/helper patch and related documentation; it was preserved.
- Implementation: added
  `output[hat_Gamma^x] += (3 lambda/4)g_x - 0.2 Z_x + (lambda/2)H_x` and
  `output[hat_Gamma^z] += (3 lambda/4)g_z - 0.2 Z_z`, consuming the validated
  contracted-connection/Z helper. No K/Theta/chi gradients, connection-A,
  vector/shift-Hessian, remaining shift terms, or duplicate GP advection were
  added.
- Validation design: the focused fixture independently derives the
  coefficients from `K0`, `d`, `kappa1`, and `kappa3`; checks pure H, pure
  metric/Z, output scope, and even/one-z parity; and rejects positive-sign
  antidamping, hidden multiplicity one, spurious `lambda H_z/2`, duplicate
  advection, and premature completion/eigensolver claims.
- Status: this is an implemented partial Gamma block, not complete Gamma
  evolution. Stage 4AO-C, 4AO-D, and Checkpoint G remain incomplete.
- Result: focused, operator-contract, and contracted-connection tests passed;
  all 19 current Stage 4AO-C fixtures passed with the required warning flags.
  Whitespace and protected-path checks passed, and the Stage 2 smoke parameter
  file remains unchanged.

- Date: 2026-07-21
- Goal: Implement only the Stage 4AO-C hatted-Gamma K/Theta/chi-gradient
  block.
- Initial state: clean committed Gamma checkpoint; CodeGraph used first.
- Implementation: added the selected d=4 x/z gradient rows, including the
  `-27 lambda delta h_xx/(8x)` and `-27 lambda delta h_xz/(8x)` inverse-metric
  variations of the GP K gradient and the distinct `7 lambda/2` versus
  `3 lambda/2` chi coefficients. Only hatted-Gamma outputs are written.
- Tests: the independent focused oracle covers pure K, Theta, chi, `h_xx`,
  `h_xz`, excluded `h_zz/h_ww`, wrong signs, omitted metric variation,
  distinct chi coefficients, parity, output scope, and no duplication of
  prior Gamma/advection blocks. Focused and prior Gamma/helper/contract tests
  passed; all 20 Stage 4AO-C fixtures passed without warnings.
- Remaining: connection-A, vector/shift-Hessian terms, remaining coupled
  terms, and final Gamma row assembly. Stage 4AO-C, 4AO-D, and Checkpoint G
  remain incomplete.

- Date: 2026-07-21
- Goal: Implement only the Stage 4AO-C hatted-Gamma connection-A block.
- Initial state: clean committed Gamma-gradient checkpoint; CodeGraph used
  first.
- Implementation: added `C_x` and `C_z` only to the two hatted-Gamma outputs,
  using the reviewed `2 A_GP^{IJ} delta Gamma^i_IJ` formulas with explicit
  hidden multiplicity two. Direct `delta A_IJ`, `d1.A`, common advection,
  earlier Gamma blocks, and later Hessian/shift families remain absent.
- Tests: the independent analytic oracle builds each visible/hidden
  linearized Christoffel before contracting with the GP A tensor. It covers
  visible `xx/xz/zz`, both `ww` copies, pure `h_ww`, pure `h_xz`, diagonal
  data, parity, output scope, pure/differentiated A exclusions, and no
  duplication. Wrong hidden multiplicity, derivative signs, and `1/x`
  coefficients are rejected.
- Remaining: vector/shift-Hessian terms, remaining coupled terms, and final
  Gamma row assembly. Stage 4AO-C, 4AO-D, and Checkpoint G remain incomplete.
- Result: focused, operator-contract, prior Gamma/helper, and all 21 Stage
  4AO-C fixtures passed without warnings. Whitespace and protected-path checks
  passed; nothing was staged or committed.

- Date: 2026-07-21
- Goal: Implement the separate Stage 4AO-C hatted-Gamma vector-Hessian and
  grad-div metric-variation blocks.
- Initial state: clean committed connection-A checkpoint; CodeGraph used
  first.
- Implementation: added the x-only vector Hessian
  `-3 lambda h_xx/(4x)+3 lambda h_ww/x`, with two hidden directions, and the
  separate grad-div pair `9 lambda h_xx/(8x)`,
  `9 lambda h_xz/(8x)`. The z vector Hessian remains exactly zero because
  `beta_GP^z=0`; previous Gamma families are unchanged and unassembled.
- Tests: independent background-shift Hessian and grad-div oracles cover pure
  `h_xx/h_ww/h_xz`, excluded `h_zz`, hidden multiplicity, sign/coefficient
  mutations, raw parity, output scope, prior-family non-duplication, and
  closed completion/eigensolver gates.
- Closure: the locked frozen-gauge mathematical Gamma family inventory is now
  closed. Final row assembly and combined background/oracle/epsilon/parity
  validation remain missing, so Gamma RHS completion remains false.
- Result: focused, operator-contract, prior Gamma/helper, and all 22 Stage
  4AO-C fixtures passed without warnings. Final whitespace and protected-path
  checks passed; nothing was staged or committed.

- Date: 2026-07-21
- Goal: Assemble and independently validate the complete frozen-gauge
  hatted-Gamma x/z rows.
- Initial state: clean committed shift-metric checkpoint; CodeGraph used
  first.
- Implementation: added an assembler that consumes the existing common
  advection output and the five existing non-advection partial functions once
  each. No advection or family formula was reimplemented in the assembler.
- Oracle: added a direct nonlinear selected-CCZ4/modified-cartoon test oracle
  with nonlinear inverse metric, visible/hidden Christoffels, contracted
  connection/Z reconstruction, raised A, GP shift derivatives, and two hidden
  directions. The GP residual is `(-2.775557561563e-17,0)`; directed-family
  and mixed cases pass the five-epsilon sweep and stable `1e-5/1e-6` plateau.
  Isolated vector-Hessian x and grad-div z cases make every other family zero
  before checking their nonzero targets.
- Mutations: every-family omission/duplication, duplicate common advection,
  spurious `lambda H_z/2`, hidden multiplicity one, parity leakage, and output
  leakage are rejected.
- Flags: complete Gamma family/assembly/validation and only the two Gamma
  variable-completion flags are true. The full 13-variable operator and
  eigensolver remain false. Generic Gamma Theta/Z and hidden modified-cartoon
  inventory entries are classified `implemented_now`; non-Gamma rows are
  unchanged.
- Result: focused and individual Gamma/helper/contract tests passed; all 23
  Stage 4AO-C fixtures compiled without warnings and passed. Protected paths
  remained untouched; nothing was staged or committed.

- Date: 2026-07-22
- Goal: Begin the documentation-only Stage 4AO-C non-Gamma RHS closure
  preflight from the committed complete-Gamma checkpoint.
- Initial state: `HEAD=8cec8c0`, worktree clean; CodeGraph used before direct
  inspection.
- Source audit: followed `CCZ4RHS.impl.hpp` through the selected non-BSSN K and
  Theta branches, the A curvature/trace-free block, and
  `CCZ4Geometry::compute_ricci_Z`. Distinguished existing metric-derived
  `delta R^g_IJ` from the missing `q_IJ=2D_(I delta Z_J)` completion.
- Result: documented exact remaining terms (`K:+q`, `Theta:+q/2`,
  `A_IJ:[q_IJ]^TF`), full row formulas without combining owners, hidden `ww`
  multiplicity, duplication hazards, oracle/mutation plan, a narrow first
  implementation recommendation, and unresolved convention questions.
- Scope: docs/checklist/TODO/logs only. No code, tests, flags, row assembly,
  boundaries, MOTS, eigensolver, or 4AO-D work; all non-Gamma and global gates
  remain false.

- Date: 2026-07-22
- Goal: Implement only the Stage 4AO-C encoded-Z Ricci-completion tensor
  helper while preserving the uncommitted closure preflight.
- Implementation: added an opaque checked Z-derivative jet and read-only
  result for `q_xx`, `q_xz`, `q_zz`, representative `q_ww`, the
  two-hidden-copy trace, and `d=4` trace-free projection. No raw/geometric
  Ricci, Z reconstruction derivatives, row weights, or RHS writes enter the
  helper.
- Oracle: an independent spherical modified-cartoon construction derives both
  hidden angular covariant components before converting one representative
  `ww` output and using both copies in the trace. Component, trace-free,
  parity, zero, input-guard, and omission/duplication mutation checks pass.
- Gates: helper availability is true, but K/Theta/A Z-Ricci insertions,
  every non-Gamma row-completion flag, the full operator, and eigensolver stay
  false. The nonlinear selected-branch finite-difference oracle is deferred
  to complete non-Gamma row assembly.
- Result: the focused helper and operator contract passed, as did all 24
  Stage 4AO-C fixtures under `-Wall -Wextra -pedantic`. No row assembler,
  boundary, MOTS, eigensolver, 4AO-D, staging, or commit action was added.

- Date: 2026-07-22
- Goal: Insert the validated encoded-Z Ricci completion into only the K,
  Theta, and A output slots.
- Initial state: clean `HEAD=903455c` encoded-Z helper checkpoint; CodeGraph
  used first.
- Implementation: added three typed point/grid partial blocks consuming
  `CompletionTensor` and writing `K=q`, `Theta=q/2`, and
  `A_IJ=qTF_IJ`. The representative ww output is written once. No geometric
  Ricci input, second projection, or complete-row summation is available at
  this interface.
- Tests: isolated ownership, all six values, weighted A trace, parity, grid
  coverage, invalid sizes, and coefficient/raw-q/double-projection/
  geometric-Ricci/ww/output-scope mutations pass in the focused fixture.
- Gates: K/Theta/A encoded-Z insertion availability is true, but all
  non-Gamma variable completion, full-operator, and eigensolver gates stay
  false.
- Result: focused, helper, contract, affected K/Theta/A, and all 25 Stage
  4AO-C fixtures passed under `-Wall -Wextra -pedantic`. No complete row,
  boundary, MOTS, eigensolver, 4AO-D, staging, or commit action was added.

- Date: 2026-07-22
- Goal: Complete and independently validate the Stage 4AO-C frozen-gauge K,
  Theta, and four A rows from the clean encoded-Z insertion checkpoint.
- Adapter: analytically differentiated the unreduced hidden-aware `g_x/g_z`
  formulas, including mixed second derivatives, two hidden copies, and every
  `1/x^2` term, then formed `partial_a Z_i=(partial_a H_i-partial_a g_i)/2`.
  The adapter is separately testable, assumes no determinant constraint, and
  does not numerically differentiate production code.
- Assembly: added each named family once: common advection; geometric Ricci;
  encoded-Z Ricci; selected K/Theta/A algebraic terms; locked K/Theta damping;
  and A shift stretching. Existing curvature projections are consumed without
  re-projection; representative ww is written once; direct A damping is absent.
- Oracle repair: supplies exact analytic value/first/second jets at each
  nonlinear `U_GP +/- epsilon delta U` state, then lifts them to a
  four-dimensional Cartesian metric and constructs physical Ricci,
  `Z_over_chi`, lower Z, covariant Z derivatives, trace-free curvature, GP
  advection, tensor shift stretching, and selected CCZ4 algebraic terms.
  There is no internal spatial differencing and epsilon changes no unrelated
  resolution. Analytic-jet GP residuals are zero except
  `Theta=-6.78e-21`. Both mixed directions show approximately 100-fold error
  reduction from `1e-2` to `1e-3` and from `1e-3` to `1e-4`, followed by
  roundoff saturation through `1e-7`; no plateau is claimed.
- Isolation/trace repair: a nonzero geometric-Ricci direction with
  `H=g` and `partial H=partial g` has zero encoded Z and zero unrelated
  families. The metric-inclusive tangent case has `T=3.47e-18`,
  `partial_x T=0`, nonzero metric contribution `-1.354838562373e-1`, and
  `T(delta rhs)=-2.78e-17`; metric rows remain incomplete.
- Mutations: rejected omission and duplication of every family, encoded-Z
  omission/double insertion, hidden multiplicity one, doubled representative
  ww, parity leakage, and illegal writes. Adapter guards now independently
  reject representative mixed/second-derivative omissions in every row,
  both required inverse-square terms, and hidden multiplicity.
- Claim boundary: the oracle validates the geometric-Ricci-plus-Z split path;
  source mapping identifies it with selected CCZ4. It does not independently
  implement the direct Gamma-form Ricci.
- Gates: K, Theta, and all four A rows are complete. Chi and metric rows,
  complete operator/JVP, boundaries, MOTS, eigensolver, and 4AO-D stay false.
  All 27 `Stage4AOC*.cpp` fixtures compile under
  `-Wall -Wextra -Wpedantic -Werror` with no warnings and pass. The focused
  adapter and complete-row fixtures, helper/insertion fixtures, all relevant
  family/projector/Ricci fixtures, and operator contract are included in that
  suite. `git diff --check` passes; protected production paths and smoke
  parameters are clean; the staged diff is empty. No staging or commit was
  performed.

- Date: 2026-07-22
- Goal: Complete the remaining Stage 4AO-C frozen-gauge chi/metric rows and
  assemble and validate the full 13-variable interior operator.
- Initial state: clean committed complete-K/Theta/A checkpoint `b0ef512`;
  CodeGraph used first.
- Chi/metric rows: assembled common advection, existing tensor shift
  stretching, and existing algebraic coupling exactly once. The final rows
  are `chi=Adv+deltaK/2` and
  `hxx/hxz/hzz/hww=Adv+(-7,-5,-3,+5)lambda delta h/4-2delta A`.
  `K_GP=div beta_GP` removes the possible delta-chi coefficient, and the
  representative ww output is written once.
- Oracles: added an independent analytic-jet nonlinear chi/metric evaluator
  and a full selected-branch evaluator for all 13 rows. The latter constructs
  a four-dimensional modified-cartoon metric, physical Ricci, encoded Z and
  its covariant derivatives, algebraic/damping families, GP advection,
  shift stretching, and Gamma shift-Hessian terms without calling production
  partial/full rows or encoding final linear coefficients.
- Evidence: all 13 GP residuals are zero or below `5.42e-20`. Two mixed
  directions cover `1e-2` through `1e-7`; nonlinear rows show approximately
  100-fold error reduction through `1e-4` followed by roundoff saturation,
  while the straight-path chi/metric equations are exact-linear at roundoff.
  Pure/mixed, family/owner omission and duplication, common-advection,
  hidden/representative-ww, output ownership, determinant and metric/A
  tangent, and parity mutations pass. The explicit parity blocks have zero
  forbidden leakage and zero reflection commutator.
- Gates: all 13 variable RHS-completion flags and only the full frozen-gauge
  interior assembly/JVP/parity flags are true. The boundary-bearing complete
  operator, radial-boundary acceptance, MOTS, eigensolver/shift-invert,
  threshold results, 4AO-D, and production wiring remain false.
- Validation: all 29 `Stage4AOC*.cpp` fixtures compile under
  `-Wall -Wextra -Wpedantic -Werror` without warnings and pass. The focused
  chi/metric and full-interior tests, operator contract, and every affected
  prior family/helper fixture are included. `git diff --check`, protected
  paths, smoke parameters, and the empty staged diff pass; nothing was staged
  or committed.

- Date: 2026-07-22
- Goal: Begin the Stage 4AO-C radial-boundary design preflight from the clean
  committed full-interior checkpoint `554de1c`; documentation/design only.
- Source trace: CodeGraph was used first. The selected external GRChombo
  `CCZ4RHS::rhs_equation` and `CCZ4Geometry::compute_ricci_Z` branch was then
  traced at external commit `37e6595`. Physical and Z4 radial light blocks
  have fixed-boundary speeds `1-sqrt(r0/x)` and `-1-sqrt(r0/x)`; advected and
  algebraic blocks have speed `-sqrt(r0/x)`. The frozen longitudinal block is
  Jordan rather than a complete characteristic pair.
- Inner contract: every principal sector exits at decreasing x whenever
  `0<x_in<r0`; impose no continuum data. Retain all 13 PDE rows with matching
  second-order one-sided radial derivatives. Only local determinant and
  metric/A tangent projection is allowed. Packet reflection, discrete-symbol,
  parity, and stencil convergence criteria are fixed.
- Outer contract: for static `exp(ikz)`, `k>0`, use the independently derived
  decaying asymptotic block with leading `f~exp(-kx)/x` and
  `f_x+(k+1/x)f=0`, together with homogeneous incoming Z4 constraint data.
  Homogeneous Dirichlet is only a boundary-systematic alternative. The `k=0`
  charge/gauge sector remains a separate diagnostic.
- Discrete/gate contract: boundary-local rows remain block diagonal in P+/P-,
  representative ww is written once, and continuum boundary rows form a
  sparse generalized pencil. Implementation, independent validation,
  boundary-bearing completion, and eigensolver permission remain distinct.
  All boundary, MOTS, eigensolver, threshold, production, 4AO-D, and
  Checkpoint G gates stay false. The first implementation target is the inner
  pure-outflow endpoint derivative/packet-reflection helper.

- Date: 2026-07-22
- Goal: Implement and independently validate the Stage 4AO-C inner no-data
  pure-outflow endpoint operator from clean checkpoint `6809d99`.
- Implementation: added explicit `(-3,4,-1)/(2dx)` and
  `(2,-5,4,-1)/dx^2` coefficient/reach helpers, the two-sector Fourier-
  amplitude rule `D_z=-s p k`, and `D_xz=D_xD_z`. No radial ghost unknown,
  physical extrapolation, reset, or continuum boundary equation exists.
- Full rows: the endpoint wrapper constructs the Ricci, encoded-Z,
  connection, and common-advection inputs from the one-sided jet and calls the
  existing complete interior assembler once. All 13 PDE rows remain active;
  representative hww/Aww are written once; algebraic cleanup is separate.
- Evidence: polynomial exactness and smooth manufactured second-order
  convergence pass. The local outgoing-normal-mode endpoint-symbol reflection
  measure decreases from `5.482906556595e-5` at `dx=1/64` to
  `5.107069272296e-8` at `dx=1/2048`. Both Fourier sectors have zero leakage
  and zero reflection commutator. Owner omission/duplication, stencil, reach,
  centered-stencil, reset, ww, and grid-size mutations are rejected.
- Causality: `x_in/r0={0.35,0.50,0.65,0.80}` gives negative `c_+`, `c_-`, and
  `c_0`; the horizon is glancing and rejected, and `x_in>r0` is invalid for
  the no-data contract. No complete diagonalization of the longitudinal
  Jordan block is claimed.
- Gates: only the inner endpoint helper and inner pure-outflow validation
  flags are newly true. Outer, aggregate radial-boundary, boundary-bearing
  complete-operator, MOTS, eigensolver/shift-invert, threshold, production,
  4AO-D, and Checkpoint G gates remain false.

- Date: 2026-07-22
- Goal: Complete the Stage 4AO-C outer `k>0` asymptotic-boundary
  normalization lock from clean inner-boundary checkpoint `7ceec6f`;
  documentation/design only.
- Historical claim, subsequently rejected: this pass proposed the factor
  `s^5(s^2-k^2)^4` and inferred four light pairs plus five
  advected/algebraic/Jordan amplitudes. The 2026-07-23 exact-rational
  `q=2/3` counterexample disproved that universal factorization, so the
  inferred dimensions are not accepted results.
- Audit correction (2026-07-23): only scalar profiles with
  `gamma={0,0,0.1,0.5}` and powers
  `p^-=1+k r0/2+gamma^2 r0/(8k)` were derived. Full-vector
  projector/eigenvector transport, Jordan exclusion, endpoint replacement,
  and a polynomial boundary representation were not established. The direct
  selectors are diagnostic only.
- Status: no boundary code or tests were added. Outer implementation and
  validation, aggregate radial-boundary completion, boundary-bearing
  complete operator, eigensolver/shift-invert, MOTS, threshold, production,
  4AO-D, and Checkpoint G gates remain false.

- Date: 2026-07-22
- Goal: Implement the Stage 4AO-C validation-only outer `k>0` transformed-
  amplitude and decaying-subspace projector helper from clean checkpoint
  `c69eaac`; CodeGraph used first.
- Transformation: maps `Y=(U,D_xU)` to the locked four `W^{out/in}` pairs and
  `J,F,G,C_h,C_A` through physical `delta gamma`, `d`, `p`, and reconstructed
  lower `z`. Hidden multiplicity two occurs only in traces; representative
  `ww` is read once. Zero/nonfinite `k` and invalid/nonfinite boundary inputs
  reject.
- Audit correction (2026-07-23): the four columns are one-hot diagnostic
  amplitudes scaled by scalar profiles. Their rank, condition, mixing, and
  scalar-residual numbers do not validate full WKB states, growing/Jordan
  rejection, parity, or constraints. Literal-zero parity diagnostics were
  removed.
- Gates: only diagnostic transform/projector helper flags are true. No
  accepted endpoint PDE row or pencil row exists. Outer endpoint
  implementation/validation, aggregate boundary, boundary-bearing operator,
  eigensolver, MOTS, threshold, production, 4AO-D, and Checkpoint G remain
  false.

- Date: 2026-07-23
- Goal: Audit and repair the uncommitted Stage 4AO-C outer-boundary patch;
  CodeGraph first.
- Finding: the repository contains a scalar master-profile recursion but no
  full thirteen-component stationary-symbol right-vector recursion through
  `x^-3/2` and no compatible left/dual basis. The four stored columns were
  one-hot nominal outgoing amplitudes multiplied by scalar profiles, so
  projector normalization erased `p_b^-`, `gamma_b`, all retained `u_j`, the
  radial logarithmic derivative, and any absent relative mixing.
- Repair: retained the validated inner endpoint, complete interior operator,
  mirrored outer stencils, and useful `13+0/13+0/4+9` row-layout scaffolding.
  Relabeled the direct nine selectors and four provisional PDE projections as
  diagnostic characteristic scaffolding. Removed the `k={1,2,3}` interpolation
  path, exact quadratic-pencil claim, self-subtraction Dirichlet comparison,
  and literal-zero outer parity diagnostics.
- Gates: outer implementation, outer validation, aggregate radial validation,
  boundary-bearing completion, and exact quadratic-pencil representation are
  false. Eigensolver/shift-invert, `k=0`, nonzero-growth asymptotics, MOTS,
  threshold, production, 4AO-D, and Checkpoint G remain false.
- Solver status: the interior remains quadratic in `k`, but no boundary
  operator or solver-class representation has been derived. No polynomial or
  nonlinear eigensolver work is authorized.

- Date: 2026-07-23
- Goal: Derive the missing Stage 4AO-C full-vector stationary WKB basis from
  the committed corrected-boundary checkpoint; symbolic/design only.
- Start state: Used CodeGraph first. The worktree and index were clean at
  commit `6628f49` (`Correct Stage 4AO-C outer boundary status`).
- Derivation: Extracted the exact sparse radial matrices
  `M0 U+M1 D_x U+M2 D_xx U=0` directly from all thirteen validated interior
  rows, including spherical, encoded-Z, hidden-sphere, and Gamma terms.
  Independent comparison at `q={0.31,0.73,1.11}`, `X={7,13,29}`, and both
  parity sectors gives maximum source-versus-matrix mismatch `2.44e-16`.
- Retracted conclusions: this pass incorrectly claimed the universal
  `s^5(s^2-k^2)^4` factorization and inferred one light plus two zero-sector
  generalized chains from nullity three. Exact rational arithmetic at
  `q=2/3` instead gives a factor proportional to
  `s^5(s^2-q^2)^3(s^2-q^2+1/200)`, disproving the universal claim. Ordinary
  kernel nullity of this singular leading matrix does not classify the full
  asymptotic branches, and coincident leading vectors with distinct
  subleading series have not been excluded.
- Decision: Did not invent `gamma`, `p`, or componentwise `u0..u3` values.
  Superseded the scalar `gamma={0,0,0.1,0.5}` and power formulas as WKB
  claims, documented the unresolved higher-order classification, and kept every outer
  and downstream gate false. No endpoint rows, nullspaces, eigensolvers,
  production wiring, MOTS work, technical code, staging, or commit were added.
- Solver status: the interior remains quadratic in `k`; no polynomial,
  rational, or nonlinear classification exists for an absent outer operator,
  and no eigensolver work is authorized.
- Final status: the complete thirteen-variable interior and inner pure-outflow
  endpoint remain validated. Mirrored outer stencils and row-layout code are
  diagnostic scaffolding only; no outer condition, rank-four decaying basis,
  or rank-nine physical annihilator exists. Every outer and downstream gate
  remains false.
- Project decision: freeze the custom stationary outer-boundary problem as
  deferred research. Next inventory custom-solver/GRChombo overlap, compare
  shared formulas and numerical outputs, use the evidence to assess AI-agent
  capability, and adapt missing production functionality directly from
  GRChombo.

- Date: 2026-07-23
- Goal: Create the custom-solver versus GRChombo overlap/gap checklist,
  comparison hierarchy, production adaptation backlog, and AI-agent evidence
  scorecard; documentation/inventory only.
- Start state: Used CodeGraph first. The worktree and index were clean at
  commit `015a035` (`Freeze unresolved Stage 4AO-C outer boundary`).
- Authority map: GRChombo is the production/convention authority; the complete
  custom thirteen-row frozen interior, nonlinear JVP oracle, and hidden helpers
  are retained as independent references. Mature GRChombo RK4, AMR,
  MPI/OpenMP, restart, ghost, parsing, reduction, interpolation, and AH
  infrastructure must be reused rather than rebuilt.
- Source finding: the clean ignored GRChombo checkout is at upstream commit
  `37e659523830418b210acea1661dac0e00bb1b75`. It contains standard CCZ4,
  gauge, derivative, boundary, AMR, and AH infrastructure but no black-string
  or modified-cartoon production source. Docker was unavailable in this shell,
  so the previously used image digest and Chombo identity remain unrecorded.
- Outputs: added the master capability/ownership checklist, four-level
  comparison plan with fixed Level-2 tolerances, prioritized adaptation
  backlog, and scorecard template without a final score. Updated the roadmap,
  stage checklist, TODO, and current logs only.
- Production sequence: pin dependencies; map slots/dimensions; compare
  conformal algebra, contracted connection/Z, and visible Ricci; then adapt GP
  initial data/source, hidden production terms/cleanup, periodic z, background
  evolution, perturbation/growth diagnostics, and string-horizon observables.
- Scope: no code, tests, boundary work, eigensolver, MOTS, or production
  evolution was implemented. The custom outer-boundary problem remains
  deferred and every associated gate remains false.

- Date: 2026-07-23
- Goal: Execute custom Stage 4AO-C versus GRChombo comparison batch 1 from the
  committed inventory checkpoint; no production adaptation or boundary work.
- Start state: Used CodeGraph/MCP first. The worktree and index were clean at
  `661468ade479cf003dc5336e665dc7b70edf48c6`. The ignored GRChombo checkout
  was clean, detached, and exactly at
  `37e659523830418b210acea1661dac0e00bb1b75`.
- Bridge: Added one focused test-only fixture that directly includes and calls
  GRChombo `TensorAlgebra::compute_inverse_sym`,
  `compute_determinant_sym`, `compute_trace`, `make_trace_free`,
  `compute_christoffel`, and `CCZ4Geometry::compute_ricci`. The independent
  Ricci side uses the general metric/Christoffel definition and does not call
  or copy the GRChombo Ricci path.
- Dimensions: Compiled the inspected stock source explicitly at
  `CH_SPACEDIM=3`, `GR_SPACEDIM=3`, `DEFAULT_TENSOR_DIM=3`. Kept the custom
  production assertions at physical `d=4`, gridded `d=2`, hidden
  multiplicity two; no cross-dimension coefficient was labeled a match.
- State map: All thirteen frozen slots map exactly to stock variables except
  the custom hidden representatives `hww/Aww`. Stock visible-y and gauge slots
  are explicitly classified.
- Results: conformal algebra maximum absolute/normalized errors are
  `4.441e-16` / `6.808e-05`; contracted connection/Z are
  `1.837e-14` / `1.036e-02`; visible physical Ricci are
  `2.220e-16` / `1.530e-04`. All meet the unchanged
  `5e-13 + 5e-12*scale` rule.
- Negative controls: wrong trace dimension, connection sign, and Ricci
  off-diagonal sign were all detected.
- Directness: executable tensor algebra, contracted connection, and Ricci are
  direct compiled GRChombo comparisons. `gamma=h/chi` and `Kij`
  reconstruction are source/convention only. Hidden `ww` connection/Ricci
  terms are custom-only pending production adaptation.
- Reproducibility gap: compiler, flags, commits, status, and dimensions are
  recorded. Docker is unavailable and no local Chombo installation is
  present, so their exact digests remain unresolved without blocking the
  header-only comparisons.
- Scope: no production solver source, external GRChombo, outer-boundary work,
  eigensolver, MOTS, smoke parameter, staging, or commit was changed. No final
  agent score was assigned.

- Date: 2026-07-23
- Goal: Execute custom-solver/GRChombo comparison batch 2 for the directly
  overlapping visible stock-`d=3` CCZ4 RHS families, with raw Ricci and
  encoded Z separated.
- Start state: Used CodeGraph/MCP first. The worktree and index were clean at
  committed batch-1 checkpoint
  `921e639dede8a020a34b20d182f43693e757561d`; ignored GRChombo was clean and
  detached at `37e659523830418b210acea1661dac0e00bb1b75`.
- Direct bridge: The focused fixture compiles and invokes the inspected
  `CCZ4RHS::rhs_equation`, `CCZ4Geometry::compute_ricci`, and
  `compute_ricci_Z`. Only unavailable Chombo cell/derivative infrastructure
  and gauge outputs are replaced at the test boundary; no final RHS formula is
  copied as the direct side.
- Formulation: `USE_CCZ4`, `d=3`, `Lambda=0`, `kappa1=0.1`, `kappa2=0`,
  `kappa3=1`, `covariantZ4=true`, vacuum matter, supplied analytic
  lapse/shift. The custom production path remains physical `d=4`, gridded
  `d=2`, hidden multiplicity two.
- Geometry: raw, encoded-Z, and combined tensor/scalar results all pass the
  unchanged `5e-13 + 5e-12*scale` rule. The largest absolute geometry error is
  `3.053e-16`.
- RHS: Every exercised visible `chi/h/K/Theta/A` family passes independently;
  the combined rows pass with maxima from `2.776e-17` to `1.943e-16`.
  Direct `A` damping is absent as required.
- Controls: all ten omission, duplication, index/conformal-factor,
  trace-dimension, sign, RHS-dimension, and formulation mutations are
  detected.
- Audit behavior: the direct bridge exposed two mistakes in the unaccepted
  draft oracle—lowering Z before every upper component was constructed and
  failing to subtract active conformal Ricci in the Hessian isolation. Both
  were corrected before evidence was recorded; no custom-production or
  GRChombo defect was established.
- Scope: hidden `ww` terms remain custom-only pending adaptation. No production
  code, external GRChombo, boundary condition, eigensolver, MOTS, smoke
  parameter, staging, or commit changed. No final agent score was assigned.

- Date: 2026-07-23
- Goal: Execute comparison batch 3 for discrete derivatives, manufactured
  visible-RHS convergence, and visible algebraic cleanup without production
  adaptation.
- Start state: Used CodeGraph/MCP first. The worktree and index were clean at
  committed batch-2 checkpoint
  `571b142ab2bd8c14abb967eb259f2ca202ec9d22`; ignored GRChombo remained clean
  and detached at `37e659523830418b210acea1661dac0e00bb1b75`.
- Direct paths: Invoked actual custom centered order-two helpers and actual
  GRChombo fourth-order first/second/mixed and biased-advection kernels. The
  fixture supplies a local analytic periodic ghost patch because full Chombo
  grid infrastructure is unavailable; it does not label that patch as
  production periodic handling.
- Derivatives: All six derivative/divergence classes converge at
  `1.9957-1.9975` custom and `3.9895-3.9933` GRChombo over four refinements.
- Discrete RHS: Raw Ricci, encoded Z, lapse Hessian, advection, shift
  derivatives, and complete visible rows converge independently at expected
  orders. Four-level continuum extrapolations agree to
  `4.91998803e-13`.
- Cleanup: Direct actual `TraceARemoval` and `PositiveChiAndAlpha` checks pass
  against independent same-dimension algebra. No runtime stock determinant
  normalization or hidden-aware cleanup owner was found.
- Controls: Active sign, spacing, mixed-direction, periodic-wrap, stencil
  order, one-path, wrong-dimension, omission, positivity, and hidden
  multiplicity controls fail as required. Determinant-exponent controls are
  explicitly local/source-only because no callable stock output exists.
- Scope: No custom production equation, external GRChombo, smoke parameter,
  outer boundary, eigensolver, MOTS, production evolution, staging, or commit
  changed. No final overall agent score was assigned.

- Date: 2026-07-23
- Goal: Execute comparison batch 4 at the GP black-string initial-data,
  background-residual, and fixed-lapse-source convention seam without
  production wiring.
- Start state: Used CodeGraph/MCP first. The worktree and index were clean at
  committed batch-3 checkpoint
  `debafd929377498eaa22c84fdf8c84409f7c3cee`; ignored GRChombo was clean and
  detached at `37e659523830418b210acea1661dac0e00bb1b75`.
- Direct paths: The two focused fixtures invoke inspected GRChombo tensor
  algebra, `CCZ4RHS::rhs_equation`, `FourthOrderDerivatives`, and
  `MovingPunctureGauge` templates. The independent custom side constructs GP
  jets analytically, calls `make_centered_derivative_jet`, and calls the
  validated full physical-`d=4` preflight/oracle.
- Convention: The angular coordinate component is
  `gamma_theta_theta=x^2`; the custom modified-cartoon slot is its normalized
  Cartesian representative `hww=1`. Stock visible `y` is not hidden `ww`.
  Dimension-specific `d=3` and `d=4` K/A coefficients are reported
  separately.
- Residuals: The repaired fixture invokes the actual complete analytic oracle
  for all 13 rows and separately evaluates `H,Mx,Mz`. Independent stock and
  target family paths reproduce the exact K/A fractions and target zeroes
  without residual negation. The discrete preflight is separately classified
  and reaches pairwise orders `1.81140120`, `1.90359372`, and `1.95125006`
  through `N=2048`, with worst row/location retained.
- Gauge: Direct stock lapse is `-3 lambda`; a real test-only `+3 lambda`
  adapter writes lapse only and has zero numerical Jacobian for all 20
  evolved-field coordinates. Wrong `2K`, evolving-horizon, sign/factor,
  shift-owner, and B-owner adapters are detected.
- Discrete setup: Custom first/second derivatives converge at
  `2.00018683/2.00017965`, GRChombo at
  `4.00089876/3.96062158`; reduced visible RHS orders are
  `2.00005826/4.09948021` on their clean regimes.
- Perturbations: `k_n=2 pi n/L`, both sine/cosine parity assignments, all
  thirteen frozen slots, and zero gauge perturbations pass. No physical
  radial profile was selected.
- Scope: No production initial data, hidden evolution, gauge source, time
  integration, external GRChombo, smoke parameter, outer-boundary research,
  staging, or commit changed. No final overall agent score was assigned.

- Date: 2026-07-23
- Goal: Begin the GRChombo production-adaptation preflight without production
  physics or evolution changes.
- Start state: Used CodeGraph/MCP first. The top-level worktree and index were
  clean at committed batch-4 checkpoint
  `fc15a936dbaa149edbd68c80d4f921d98aa13eec`; ignored GRChombo was clean and
  detached at `37e659523830418b210acea1661dac0e00bb1b75`.
- Dependency lock: Added a tracked source manifest and read-only verifier for
  the exact GRChombo remote and commit. The verifier rejects wrong origin,
  wrong commit, attached branch, and dirty state. Chombo, PETSc, Docker-image,
  and container-recipe digests remain explicitly unresolved.
- Ownership: Locked a future 18-slot state under `CH_SPACEDIM=2`,
  `GR_SPACEDIM=4`, and `DEFAULT_TENSOR_DIM=4`. There are no visible-`y`
  slots; one `hww/Aww` representative carries multiplicity two. The GP
  storage convention remains `gamma_theta_theta=x^2`,
  `gamma_ww=hww/chi`, and `hww=1`.
- Architecture: Classified the example enum/names as black-string
  replacements, CCZ4/cleanup/constraints/gauge as thin wrappers, hidden
  cartoon geometry as the extension, and GRChombo runtime infrastructure as
  unchanged reuse. Defined a future analytic-jet adapter returning all 13
  physical RHS rows with visible/hidden/total reports against the frozen
  custom oracle.
- Scope: Documentation, manifest, and verification tooling only. No
  production source, test equation, external dependency, smoke parameter,
  stationary boundary research, staging, or commit was changed.

- Date: 2026-07-23
- Goal: Implement dependency verification and the first isolated
  black-string production variable contract.
- Start state: Used CodeGraph/MCP first. The top-level worktree and index were
  clean at committed preflight checkpoint
  `4e76df0a38da8efcda5b5238c75b54ee592ea910`; the verifier accepted clean
  detached GRChombo commit `37e659523830418b210acea1661dac0e00bb1b75`.
- Contract: Added exactly 18 contiguous slots and one aliased
  registration/checkpoint/output name array. Per-slot metadata records
  physical/gauge category, scalar/one-z/gauge parity, storage and hidden
  multiplicity, stock overlap, and initial-data/RHS/cleanup/diagnostic owners.
- Validation: The focused fixture compiles under C++17 strict warnings with
  `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and `DEFAULT_TENSOR_DIM=4`. It passes the
  13+5 split, exact one-z fields, no-visible-y, GP storage, and hidden
  trace-copy checks; all nine requested mutations fail.
- Stock seam: `DimensionDefinitions.hpp` and `Tensor.hpp` accept the target
  tuple. Unmodified stock symmetric mapping selects three gridded slots but
  requires ten physical tensor components, while vector mapping selects two
  slots for four physical components. A dedicated Vars adapter is required.
  Full stock-header compilation is independently blocked by missing Chombo
  `parstream.H`.
- Scope: The old 27-slot smoke/comparison `UserVariables.hpp`, external
  GRChombo, parameters, initial data, BoxLoops, RHS, cleanup, constraints,
  source, periodic ownership, diagnostics, and evolution were not changed.

- Date: 2026-07-23
- Goal: Implement the reduced production Vars seam, exact GP pointwise
  initializer, and analytic GP radial metadata without Chombo wiring.
- Start state: Used CodeGraph/MCP first. The worktree and index were clean at
  committed 18-slot checkpoint
  `fca3b899feacb2290887089b398d9f2836980ae8`; the dependency verifier
  accepted clean detached GRChombo
  `37e659523830418b210acea1661dac0e00bb1b75`.
- Adapter: Added storage-agnostic reduced tensors/vectors, physical/gauge
  groups, const/mutable access, and exact load/store using only
  `BlackStringProductionVariables.hpp`. The adapter documents the future
  `Cell`/`FArrayBox` boundary and replacement of incompatible stock enum
  mapping.
- Initializer: Added all 18 locked GP values plus analytic radial jets for
  `beta^x`, `lambda`, `K`, `Axx`, `Axz`, `Azz`, and `Aww`. Stored `hww=1`
  remains distinct from coordinate `gamma_theta_theta=x^2`.
- Validation: Strict-warning Vars and GP fixtures pass three point cases,
  exact round trips, local mutable writes, determinant/trace/reconstruction,
  gauge ownership, central-difference order, roundoff turnover, input
  rejection, and all requested mutations.
- Scope: No external source, live registration, Chombo storage, BoxLoop, RHS,
  hidden geometry, cleanup, constraint, fixed source, periodic ownership,
  evolution, diagnostic, smoke parameter, staging, or commit changed.

- Date: 2026-07-23
- Goal: Resolve as much of the Chombo build contract as the locked source and
  repository-owned evidence permit, without downloading dependencies or
  implementing production physics.
- Start state: Used CodeGraph/MCP first. The worktree and index were clean at
  reduced-Vars/GP checkpoint
  `dc4981ec3a1f26f1d90ba1252e934ab77c1a9ede`; the original dependency
  verifier accepted clean detached GRChombo
  `37e659523830418b210acea1661dac0e00bb1b75`.
- Recovered contract: Locked CI checks out `GRChombo/Chombo` into `Chombo`,
  exports `CHOMBO_HOME=Chombo/lib`, installs `lib/mk/Make.defs.local`, and
  builds `AMRTimeDependent`, `AMRTools`, `BaseTools`, and `BoxTools`.
  Checkout `ref` is absent in all three workflows, so no Chombo revision or
  patch set was invented.
- PETSc/container: Core and the pointwise GP wrapper do not require PETSc;
  `USE_AHFINDER` does. CI uses unpinned Ubuntu `petsc-dev` and
  `pkg-config petsc`. The historical image name is known, but its tag, digest,
  and recipe digest are not. Current Chombo/PETSc/Docker facilities are
  unavailable.
- Tooling: Split the manifest into GRChombo/Chombo/PETSc/container records;
  added honest metadata/probe/build verification modes and exact candidate
  checking; added a real-header `2/4/4` probe using Chombo `Make.test`,
  `parstream.H`, `FArrayBox.H`, GRChombo `Cell.hpp`, and the reduced Vars
  seam.
- Validation: Shell syntax and metadata verification pass. The compile probe
  returns its documented blocked status 2 because `external/Chombo` and an
  authoritative Chombo revision are absent. No stubs, downloads, dependency
  edits, production physics, staging, or commit were introduced.

- Date: 2026-07-23
- Goal: Recover or qualify and pin the Chombo revision for the locked
  GRChombo adaptation.
- Historical recovery: Used CodeGraph/MCP first, then exhausted official
  workflow-run, PR, job-log, artifact, and cache metadata for locked
  GRChombo commit `37e659523830418b210acea1661dac0e00bb1b75`. Public log
  archives require repository administration; no artifact, cache, PR-head
  run, or authenticated `gh` path exposed the Chombo SHA. Historical
  provenance is therefore `inferred`, not exact.
- Candidate: Selected only official Chombo
  `8684f2e000106f1abadb72642e1d15351867f98f`, the default-branch head at the
  successful 2026-04-16 GRChombo CI timestamp. The ignored checkout remained
  detached and clean; no dependency source was patched.
- Qualification: Built serial DIM2 BaseTools, BoxTools, AMRTools, and
  AMRTimeDependent with GNU C++/Fortran 15.2.0. Corrected the target probe so
  it compiles `all` and executes `run-only`; real Chombo
  `parstream.H`/`FArrayBox.H`, GRChombo `Cell.hpp`, tensors/geometry, and the
  reduced Vars seam pass at `CH_SPACEDIM=2`, `GR_SPACEDIM=4`,
  `DEFAULT_TENSOR_DIM=4`. Stock DIM3 VariableStore and CCZ4Geometry tests
  also compile and pass.
- Result: Pinned the candidate as `PROJECT_QUALIFIED`, distinct from
  historical provenance. The verifier enforces both clean detached source
  locks and the four DIM2 libraries while continuing to report the former
  container and PETSc/AHFinder tuples unresolved. The thin
  `Cell`/`FArrayBox` GP storage wrapper is now authorized; no production
  physics, `BoxLoop`, evolution, dependency source, smoke parameter, staging,
  or commit changed.

- Date: 2026-07-24
- Goal: Reconcile the roadmap, authoritative checklist, TODO, GRChombo
  adaptation backlog, comparison plan, scorecard, dependency/preflight notes,
  and current logs after Chombo qualification.
- Start state: Used CodeGraph/MCP first and confirmed a clean worktree at
  committed qualification checkpoint `816a555`. Both official dependency
  checkouts were detached, clean, and at GRChombo
  `37e659523830418b210acea1661dac0e00bb1b75` and Chombo
  `8684f2e000106f1abadb72642e1d15351867f98f`.
- Dependency disposition: Historical Chombo provenance remains unavailable
  and inferred. The project core tuple is qualified; former container,
  PETSc/AHFinder, MPI, and full black-string runtime status remain separate.
- Roadmap disposition: Retained the custom solver as the validated reference
  oracle and capability benchmark, selected GRChombo as production owner, and
  froze unresolved outer spectral work as deferred and nonblocking without
  marking the original gate passed. Stage 4AO-D and Checkpoint G remain
  incomplete.
- Production lock: Recorded the ten-step active sequence and five substantive
  audit checkpoints. The exact next implementation substage is only the thin
  `Cell`/`FArrayBox` storage wrapper around the validated 18-slot reduced Vars
  and GP point initializer.
- Scope: Documentation/status only. No Cell/FArrayBox wrapper, BoxLoop, RHS,
  gauge source, periodicity, evolution, diagnostics, AHFinder, code, tests,
  production source parameters, smoke parameters, staging, or commit was
  changed.
- Validation: Ran the strict dependency verifier and real target-header
  probe. Because this shell lacked the already-qualified Fortran/csh/link
  tools, downloaded and extracted them under `/tmp` only; no system package
  was installed. The verifier returned `CORE_DEPENDENCY_BUILD=VERIFIED`, the
  probe returned `TARGET_HEADER_PROBE=PASS`, and probe-generated artifacts
  were cleaned before the final status check.

- Date: 2026-07-24
- Goal: Implement only the black-string real `Cell`/`FArrayBox` storage seam
  around the validated 18-slot reduced Vars contract.
- Start/gate: Used CodeGraph/MCP first and confirmed clean checkpoint
  `450624c`. Inspection of the verifier help, manifest, and dependency
  contract established `scripts/verify_grchombo_dependency.sh
  --require-build` as the strongest supported core qualification mode; no
  unsupported `--strict` alias was added or documented. The verifier and
  target-header probe both passed with the qualified temporary compiler
  support under `/tmp`.
- Implementation: Added `BlackStringCellStorage.hpp` with const `load` and
  mutable-cell `store` operations, exact 18-component guards, production-enum
  slot iteration, and no hidden multiplicity expansion. Added only a
  test-local `UserVariables.hpp` boundary and real Chombo DIM2 fixture using
  `FArrayBox`, explicit `IntVect`, `BoxPointers`, and `Cell`.
- Evidence: Exact distinct-value and GP round trips pass. All 18
  component-local mutations preserve ownership; neighbors remain unchanged;
  `hww/Aww` each write once. Swapped, duplicate, omitted, off-by-one,
  visible-tensor `hww`, doubled hidden, neighbor-write, legacy 27-component,
  and oversized-output mutations are detected.
- Scope: No `BoxLoop`, live application registration, RHS, cleanup,
  constraints, source, periodicity, evolution, diagnostics, AHFinder,
  external dependency source, production physics, or smoke parameters were
  changed. The exact next implementation is the GP `BoxLoop` initializer
  that delegates each point to the existing initializer and storage seam.
