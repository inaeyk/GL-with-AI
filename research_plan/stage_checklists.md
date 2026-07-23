# Stage Checklists

These checklists are gatekeeping tools and the current status/checkpoint
authority for the project. They do not replace the reproduction plan or project
outline. Each stage should be reviewed before moving to the next. For
code-generating agents, completion requires human review and approval, not just
self-review.

Future physics or physics-design stages should include a polished physics-stage PDF note under `docs/physics_notes/` when they contain substantive physics reasoning. Build-only and infrastructure stages do not need full physics PDFs unless they include nontrivial physics design.

Global guardrails: compilation alone is not physical validation; local
validation harnesses are not production Stage 4AR/4AS wiring; no physical GL
evolution correctness, Pau diagnostic reproduction, or radiation extraction is
claimed yet. GRChombo/internal consistency is the convention authority, not
Pau. Checkpoint G passes only after 4AO-D; Checkpoint H follows 4AP-4AR before
live evolution; Checkpoint I follows 4AS-4AU before any Stage 5/Pau diagnostic
reproduction.

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
- [x] Confirm plotting workflow is either implemented or explicitly deferred.

Evidence: `README.md`; `.gitignore`; `scripts/run_binarybh_smoke.sh`;
`scripts/collect_binarybh_outputs.sh`; `scripts/plot_binarybh_outputs.sh`;
`scripts/clean_binarybh_smoke.sh`; `results/BinaryBH_verycheap/metadata.md`;
`results/BinaryBH_verycheap/plots/README.md`; `logs/PROJECT_LOG.md`;
`logs/CODEX_LOG.md`.

Open: clean-state one-command smoke-script proof remains unchecked; owner:
future infrastructure cleanup before relying on it as a release-quality
workflow.

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

Evidence: `external/GRChombo/`; `docs/grchombo/capability_map.md`;
`docs/grchombo/source_map.md`; `docs/grchombo/build_notes.md`;
`docs/implementation/ccz4_cartoon_variables.md`; `logs/PROJECT_LOG.md`;
`logs/CODEX_LOG.md`.

Gate: do not proceed to Stage 1.5 or create `BlackStringToy` until the source/capability maps and hazard notes are reviewed and approved by the user.

## Stage 1.5 - Target-dimension compile preflight

Goal: check whether the intended compile configuration `CH_SPACEDIM=2`, `GR_SPACEDIM=4` is technically viable before physics implementation.

- [x] Identify the smallest public test/example suitable for compile preflight.
- [x] Preferred first preflight target: adapt or reuse `Tests/ApparentHorizonFinderTest2D` as a controlled `DIM=2`, `GR_SPACEDIM=4` compile experiment, if build tooling allows.
- [x] Reproduce the baseline `DIM=2` Docker build/run path for `Tests/ApparentHorizonFinderTest2D`.
- [ ] Superseded conditional branch: if `ApparentHorizonFinderTest2D` had been unsuitable, choose the smallest public test/example that probes the `GR_SPACEDIM != CH_SPACEDIM` path. Replacement decision: use the AH-side scratch-copy preflight and separately document the CCZ4-side scratch-copy blocker.
- [x] Attempt `DIM=2` build with `GR_SPACEDIM=4` using controlled compiler flags or minimal local copy.
- [x] Record which files compile and which fail.
- [x] Check whether `AHSphericalGeometry` guards or other `GR_SPACEDIM==3` assumptions trigger.
- [x] Check whether `hww/Aww` enum variables are required for compilation.
- [x] Attempt an additional CCZ4-side `DIM=2`, `GR_SPACEDIM=4` scratch-copy compile probe.
- [x] Document that public `CCZ4Test` is blocked as a clean target by 3D test-harness assumptions and unconfirmed `GR_SPACEDIM=4` flag propagation.
- [x] Document blockers in `docs/grchombo/build_notes.md`.
- [x] Record that a successful compile is not evidence of physical correctness.

Evidence: `docs/grchombo/build_notes.md`; `docs/grchombo/source_map.md`;
`external/GRChombo/Tests/ApparentHorizonFinderTest2D/`; ignored scratch output
under `runs/stage1_5_preflight/`; `logs/PROJECT_LOG.md`; `logs/CODEX_LOG.md`.

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

Evidence: `code/BlackStringToy/`; `scripts/stage2_build_blackstringtoy_scratch.sh`;
`docs/grchombo/build_notes.md`; `docs/implementation/black_string_initial_data.md`;
`results/BlackStringToy/metadata.md`; `logs/PROJECT_LOG.md`;
`logs/CODEX_LOG.md`.

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

Evidence for checked Stage 3A-3K design/tooling items:
`docs/implementation/black_string_initial_data.md`;
`docs/implementation/modified_cartoon_source_terms.md`;
`docs/derivations/modified_cartoon_geometry_notes.md`;
`docs/derivations/modified_cartoon_geometry_sympy.py`;
`docs/derivations/modified_cartoon_nontrivial_hww_notes.md`;
`docs/derivations/modified_cartoon_nontrivial_hww_sympy.py`;
`docs/derivations/modified_cartoon_nonconstant_q_notes.md`;
`docs/derivations/modified_cartoon_nonconstant_q_sympy.py`;
`docs/derivations/conformal_cartoon_translation_notes.md`;
`docs/derivations/conformal_cartoon_translation_sympy.py`;
`docs/derivations/offdiagonal_conformal_cartoon_notes.md`;
`docs/derivations/offdiagonal_conformal_cartoon_sympy.py`;
`docs/derivations/offdiagonal_ricci_flat_gate_notes.md`;
`docs/derivations/offdiagonal_ricci_flat_gate_sympy.py`;
`docs/derivations/ccz4_rhs_block_decomposition_notes.md`;
`docs/derivations/small_x_regularization_notes.md`;
`docs/derivations/small_x_regularization_sympy.py`;
`docs/derivations/unit_test_fixture_design.md`;
`docs/implementation/minimal_cpp_implementation_plan.md`;
`docs/physics_notes/stage3A_black_string_initial_data.pdf` through
`docs/physics_notes/stage3K_minimal_cpp_implementation_plan.pdf`;
`logs/PROJECT_LOG.md`; `logs/CODEX_LOG.md`.

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
- [ ] Future hidden-sector RHS work: disable or replace the Stage 4D smoke-only freeze and add a loud guard against using both paths together. Owner: Stage 4AT for removal/replacement before physics mode.
- [ ] Align Stage 3I parity and axis-filling conventions with GRChombo-facing project conventions before C++ source-term work. Partially covered by Stage 4AG synthetic `h_xz` validator; actual owner is Stage 4AP grid/ghost regularity validation and Stage 4AQ finite-axis treatment.
- [x] Derive and document the Stage 3I `tilde_Gamma^x` sign and full `hat_Gamma^A` contracted-connection convention in the GRChombo-facing cartoon extension. Solved later by Stage 4AM/4AN.
- [ ] Required future RHS validation: reproduce the linear Gregory-Laflamme threshold/growth spectrum after matching radius convention, periodicity, perturbation sector, gauge, and extraction variable.
- [ ] Required future constraint-damping validation: inject linearized constraint violations and verify `Theta` / encoded-`Z^i` damping signs, `kappa_1`, `kappa_2`, hidden multiplicity, and `/4` bookkeeping.
- [ ] Add optional nonconstant profile with `f_zz != 0` so `R_zz` is a nonzero regression target.
- [ ] Factor duplicated Stage 3C/3D/3E symbolic geometry helpers into a shared module.
- [ ] Write or verify formulas for 5D black-string initial data in chosen coordinates.
- [ ] Decide exact variables to initialize: `chi`, `h_ij`, `hww`, `K`, `A_ij`, `Aww`, lapse, shift, `B`, `Gamma`, `Theta`.
- [x] Verify `hww/Aww` enum placement before implementation. Solved later by Stage 4C.
- [ ] Implement parameters: `r0`, `L`, `epsilon`, `n`; `eps_cut` is superseded by the no-clamp/no-epsilon policy in Stage 4O/4AQ.
- [ ] Implement turduckening cutoff.
- [ ] Implement GL perturbation in `chi`.
- [ ] Run `t=0` or very short tests only.
- [ ] Check periodicity in `z`.
- [ ] Check smoothness outside cutoff.
- [ ] Check constraints and gauge quantities qualitatively.
- [x] Compile companion PDFs for all current physics-note LaTeX sources under `docs/physics_notes/`.

Evidence for Stage 3 items solved by later Stage 4 work:
`code/BlackStringToy/ConformalCartoonAlgebra.hpp`;
`code/BlackStringToy/UserVariables.hpp`;
`code/BlackStringToy/CartoonHatGammaX.hpp`;
`code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`;
`code/BlackStringToy/tests/Stage4CVariablePlacementTest.cpp`;
`code/BlackStringToy/tests/Stage4ANHatGammaXTest.cpp`;
`docs/derivations/stage4AM_hatGammaX_derivation.md`;
`logs/PROJECT_LOG.md`; `logs/CODEX_LOG.md`.

Remaining Stage 3 carry-forward items: actual black-string initial-data
implementation, compact-`z` parameter plumbing, GL perturbation controls,
turduckening, grid-level parity/axis handling, optional symbolic cleanup, and
production constraint/gauge checks are still future work before physical runs.

Gate status: Stage 3 design/tooling is complete and Stage 4A-4C setup work has
begun under explicit approval. Do not run long GL evolutions or treat outputs
as physically meaningful until `t=0` fields, parameter parsing, constraints,
modified-cartoon source terms, and diagnostics are reviewed and approved by the
user.

## Stage 4 - 4+1 CCZ4 / SO(3) modified-cartoon evolution

Goal: implement the actual reduced 5D dynamics on the 2D grid.

Status authority summary: Stage 4A-4AL are complete as local/test-only
building blocks and Checkpoint F has passed. Stage 4AM and 4AN are complete.
Stage 4AO-A and 4AO-B are complete as validation locks/harnesses. Stage 4AO-C
now has all 13 frozen-gauge interior rows, their one-time complete-row owners,
and the full validation-only interior assembly/JVP/parity gates complete. The
analytic selected-branch fixtures cover physical Ricci, encoded Z, hidden
multiplicity, tangent identities, family ownership, and both parity sectors.
The inner endpoint is validated. Mirrored outer stencils and diagnostic radial
row-layout scaffolding are preserved, but the full thirteen-component WKB
recursion, its left nullspace, and a compatible retained-PDE dual are missing.
The explicit stationary audit now records every asymptotic radial matrix and
independently matches the source operator to `2.44e-16` over the documented
multi-`q`, multi-`X`, two-parity comparison. The former universal fourfold
light determinant factor is disproved by an exact-rational `q=2/3`
counterexample. The singular leading matrix has nullity three, but neither a
generalized-chain obstruction nor the full branch classification has been
established.
Outer and aggregate boundary validation, the boundary-bearing operator, exact
quadratic-pencil representation, MOTS mapping, eigensolver/shift-invert,
threshold results, and the primary-source convention map are still blocked.
Stage 4AO-D is unstarted, and Checkpoint G has not passed.

Project decision: freeze the custom stationary outer-boundary problem as
deferred research. The next phase inventories custom-solver/GRChombo overlap,
compares overlapping formulas and numerical outputs, uses the comparison to
assess AI-agent capability, and adapts missing production functionality
directly from GRChombo. This decision does not reopen any Stage 4AO-C gate.

The overlap/gap inventory is complete as a documentation contract. Comparison
batch 1 now supplies an exact state map and direct compiled stock-`d=3`
evidence for conformal algebra, visible contracted connection/Z, and visible
physical Ricci. The custom production oracle remains `d=4/2` with hidden
multiplicity two; no production adaptation has started. Chombo and container
digests remain unresolved.

- [x] Stage 4A: local conformal-cartoon algebra helpers and non-grid tests.
- [x] Stage 4B: public GRChombo CCZ4 baseline-layout check.
- [x] Stage 4C: real `hww/Aww` variable entries, header-level placement checks, and non-grid placement fixture.
- [x] Stage 4C: full scratch Docker/GRChombo `BlackStringToy` scaffold compile succeeds with the 27-variable layout.
- [x] Custom-solver/GRChombo overlap-and-gap inventory: classify variables/conventions, all thirteen frozen interior rows, gauge/background, numerical infrastructure, diagnostics, direct-comparison opportunities, production ownership, and custom-code disposition. GRChombo is the production authority; the custom interior/JVP/hidden helpers remain independent oracles; the outer boundary remains deferred. Evidence: `docs/grchombo/custom_solver_grchombo_overlap_gap_checklist.md`.
- [x] Comparison/adaptation planning: define Level 1 formula/convention, Level 2 analytic-jet RHS, Level 3 manufactured-profile, and Level 4 physical-run tests with pre-run tolerances; prioritize source locking, GP initial data/source, hidden production terms, periodic z, background evolution, perturbations, growth, horizon observables, and nonlinear workflow; define the agent scorecard without an overall score. Evidence: `docs/grchombo/custom_solver_grchombo_comparison_test_plan.md`, `docs/grchombo/grchombo_adaptation_backlog.md`, `docs/grchombo/agent_capability_scorecard.md`.
- [ ] GRChombo production-adaptation P0: pin the exact GRChombo, Chombo, container, compiler, dimension, and formulation manifest. The ignored local GRChombo checkout is currently at `37e6595`; the current shell cannot inspect the Docker digest, and no authoritative modified-cartoon/black-string upstream branch has been supplied.
- [x] First executable comparison tranche: exact dependency/source fields available in the shell, slot/dimension map, conformal algebra, contracted-connection/Z, and visible-Ricci analytic jets. The focused bridge calls GRChombo `TensorAlgebra` and `CCZ4Geometry` directly at stock `d=3`; all three numerical families pass the predeclared Level-2 tolerance and all dimension/sign mutations fail. Chombo/container digests remain a P0 gap, hidden terms remain custom-only, and no production evolution or physical diagnostic was started. Evidence: `docs/grchombo/custom_solver_grchombo_comparison_batch1_results.md`.
- [x] Comparison batch 2: directly invoke the inspected GRChombo `CCZ4RHS::rhs_equation`, raw Ricci, and Ricci-Z paths at stock `d=3`; independently assemble raw/encoded/combined geometry and every visible `chi,h,K,Theta,A` term family at the same dimension. All families and combined rows pass the unchanged Level-2 tolerance; omission, duplication, index/conformal-factor, trace-dimension, sign, RHS-dimension, and BSSN mutations fail. The production custom `d=4/2` operator is unchanged; hidden `ww` terms remain an adaptation gap. Evidence: `docs/grchombo/custom_solver_grchombo_comparison_batch2_results.md`.
- [x] Comparison batch 3: real custom and GRChombo derivative paths converge at their documented orders; all 15 visible advection rows and the complete `chi`, metric, and `A` shift RHS families have independent component/location-aware monitors; visible cleanup passes. Production Chombo periodic ownership/ghost exchange remains blocked, hidden-weighted cleanup remains pending, and batch 4 is next. Do not begin target-`d=4` or modified-cartoon production comparison before the reviewed adapter exists.
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

Evidence for checked Stage 4A-4N local C++ work:
`code/BlackStringToy/ConformalCartoonAlgebra.hpp`;
`code/BlackStringToy/UserVariables.hpp`;
`code/BlackStringToy/CartoonRicciInterface.hpp`;
`code/BlackStringToy/CartoonRicciBridge.hpp`;
`code/BlackStringToy/CartoonRhsContract.hpp`;
`code/BlackStringToy/CartoonRhsSourceBlock.hpp`;
`code/BlackStringToy/CartoonAxisPolicy.hpp`;
`code/BlackStringToy/CartoonSingularCombinations.hpp`;
`code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`;
`code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp`;
`code/BlackStringToy/tests/Stage4CVariablePlacementTest.cpp`;
`code/BlackStringToy/tests/Stage4EGridToHelperMappingTest.cpp`;
`code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp`;
`code/BlackStringToy/tests/Stage4LRicciTraceFreeSourceTest.cpp`;
`code/BlackStringToy/tests/Stage4NSingularCombinationsTest.cpp`;
`docs/implementation/stage4H_ricci_rhs_compatibility.md`;
`logs/PROJECT_LOG.md`; `logs/CODEX_LOG.md`.

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
- [x] Stage 4S: connect the Stage 4R guarded geometry package into the local RHS source-block skeleton.
- [x] Stage 4S: keep the risky `(hxx - hww) / x^2` ingredient source-facing only through the Stage 4R guarded path.
- [x] Stage 4S review patch: make the guarded package and source-block carry output checked-by-construction, not open aggregates.
- [x] Stage 4S: do not implement full Ricci, full CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, or evolution wiring.
- [x] Stage 4T: add the first local guarded source-consumer diagnostic/probe.
- [x] Stage 4T: require the checked Stage 4R/4S package type and reject raw-double consumer call shapes by type.
- [x] Stage 4T: do not implement full Ricci, a Ricci sub-block, full CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, or evolution wiring.
- [x] Stage 4U: add a source-formula authoring gate for regularity-sensitive geometry.
- [x] Stage 4U: require future source formulas needing `(hxx - hww) / x^2` to consume the checked package or authoring-gate input.
- [x] Stage 4U: document that direct source-formula use of `difference_over_x2(h_xx, h_ww, x)` for this metric difference is forbidden but remains convention-only without a later lint/CI gate.
- [x] Stage 4U: do not implement full Ricci, full CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, or evolution wiring.

Evidence for checked Stage 4O-4U guarded-source path:
`code/BlackStringToy/CartoonGeometryPrimitives.hpp`;
`code/BlackStringToy/CartoonRegularityChecks.hpp`;
`code/BlackStringToy/CartoonRegularityGuardedSources.hpp`;
`code/BlackStringToy/CartoonGuardedSourceConsumers.hpp`;
`code/BlackStringToy/CartoonSourceFormulaAuthoringGate.hpp`;
`code/BlackStringToy/tests/Stage4OAxisRegimeSemanticsTest.cpp`;
`code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp`;
`code/BlackStringToy/tests/Stage4QRegularityMatchingTest.cpp`;
`code/BlackStringToy/tests/Stage4RRegularityGuardedSourceBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4USourceFormulaAuthoringGateTest.cpp`;
`logs/PROJECT_LOG.md`; `logs/CODEX_LOG.md`.

- [x] Stage 4V: intentionally add no formula because no tiny authoring-gate consumer is yet derivation-locked to Stage 4G, Stage 4L, or a Stage 3/4 note.
- [x] Stage 4V: document that the next step is to derive or extract the exact Ricci/RHS sub-expression, coefficient, sign convention, and hard-coded oracle before coding a real consumer.
- [x] Stage 4W: add a Markdown derivation note locking the hidden-sphere CCZ4 contribution map and `R_ww` as the first serious hidden Ricci target.
- [x] Stage 4W review patch: distinguish conformal `tilde{R}_ww[h]`, conformal-factor `R^chi_ww`, and physical `R_ww[gamma]`.
- [x] Stage 4W review patch: document the future varying-`chi` consistency oracle and the `A_IJ` curvature/lapse sign-convention lock against the Stage 3A `K_IJ` caveat.
- [x] Stage 4W review patch: derive `h_xz = O(x)` from first-principles reflection parity about the cartoon axis.
- [x] Stage 4W: document the Stage 4G-compatible flat, constant-cone, and nonconstant-hidden-metric `R_ww` oracles.
- [x] Stage 4W: block `R_ww` implementation until the local checked off-diagonal `h_xz / x` ingredient exists; stronger `h_xz = O(x)` validation remains future work.
- [x] Stage 4W: do not add formula code, full Ricci, CCZ4 RHS, Stage 3I finite-axis regularization, grid reads, or evolution wiring.
- [x] Stage 4X: add the checked local off-diagonal `h_xz / x` ingredient before implementing `R_ww`.
- [x] Stage 4X: expose a local checked `h_xz / x` package through `CartoonCheckedHxzOverX.hpp`.
- [x] Stage 4X: allow finite nonzero `h_xz / x`, reject invalid axis and nonfinite inputs, keep the package non-forgeable, and do not claim to prove global parity or `h_xz = O(x)`.
- [x] Stage 4X: do not implement `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, or evolution wiring.
- [x] Stage 4Y: add the first guarded formula sub-block for the conformal hidden Ricci target, `G_sing = (h_zz / D) Delta_xw - q_xz^2 / D`.
- [x] Stage 4Y: consume `Delta_xw` only through the Stage 4U/4R checked path and `q_xz` only through the Stage 4X checked path.
- [x] Stage 4Y review patch: single-source checked `Delta_xw`, checked `q_xz`, and raw determinant data from one local metric point through a non-forgeable input package.
- [x] Stage 4Y: test flat, constant-cone, and distinct-value oracles plus checked-input and determinant rejection cases.
- [x] Stage 4Y: do not implement full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, or evolution wiring.
- [x] Stage 4 hidden-sphere Ricci roadmap: integrate the completed Stage 4W-4Y path and the future hard gates in `docs/implementation/stage4_hidden_sphere_Rww_plan.md`.
- [x] Stage 4Z: add a checked `W_x / x` ingredient and implement only the reviewed `G^grad_ww` conformal hidden Ricci gradient block.
- [x] Stage 4Z: test flat, constant-cone, nonconstant-hidden-metric, and distinct-value oracles plus invalid axis, `W`, determinant, and derivative rejections.
- [x] Stage 4Z: do not implement the Stage 4Y singular block, Hessian block, full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, or evolution wiring.
- [x] Stage 4AA: lock the Hessian block derivation, coefficients, sign convention, reduced Christoffels, and primary oracles before code.
- [x] Stage 4AA: record flat `0`, constant-cone `0`, nonconstant `W=(1+x)^2` Hessian `-4`, and combined conformal `tilde R_ww=-12` oracle values.
- [x] Stage 4AA: record Claude Audit A's verified nonsymmetric derivative sample, `G^Hess_ww=-8558/2883`, and full conformal sum `-3576/961` with Stage 4G residual about `4.44e-16`.
- [x] Stage 4AA: record that the nonsymmetric oracle exercises off-diagonal Christoffels, `rho_xz`, `W_z` terms, and the `(-2B/D)` contraction, which the simpler oracles do not cover.
- [x] Stage 4AB: implement only the reviewed conformal Hessian sub-block with a single-source input package and local away-axis/determinant guards.
- [x] Stage 4AB: test flat, constant-cone, nonconstant-`W`, and verified nonsymmetric Hessian oracles plus the test-only conformal sum.
- [x] Stage 4AB: reject invalid axis, invalid `W`, bad determinant, and nonfinite derivative inputs, and keep the input package non-aggregate.
- [x] Stage 4AB: do not assemble full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, or evolution wiring.
- [x] Stage 4AC: assemble conformal `tilde R_ww[h]` from reviewed Stage 4Y, 4Z, and 4AB sub-blocks through a single-source input package.
- [x] Stage 4AC: test flat `0`, constant-cone `-3/4`, nonconstant-`W` `-12`, and verified nonsymmetric `-3576/961` conformal oracles.
- [x] Stage 4AC: include direct Stage 4G `chi=1` comparisons for the nonconstant and nonsymmetric conformal samples.
- [x] Stage 4AC: do not implement `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, or evolution wiring.
- [x] Checkpoint B / Claude Audit B cleanup: unify reduced determinant policy so Stage 4Y, 4Z, 4AB, and 4AC all require finite positive `D=AC-B^2`.
- [x] Stage 4Y Checkpoint B patch: add a negative-determinant rejection case for the singular block without changing the formula.
- [x] Stage 4AD: derive `R^chi_ww` and design its own hidden/cartoon singular guard stack before implementation.
- [x] Stage 4AD: lock `D_wD_w chi`, the full conformal Laplacian with hidden multiplicity `2/W`, the reduced scalar Hessian pieces, and the conformal gradient norm.
- [x] Stage 4AD: require Stage 4AE to add a checked local `chi_x/x` ingredient and reuse checked `q_xz`, checked `p_W`, away-axis-only semantics, and positive `chi`, `W`, and determinant guards.
- [x] Stage 4AD: record constant-`chi` zero, flat `chi=1+a x` `11/144`, and z-dependent `chi=1+b z` `-1/64` oracle candidates; Stage 4AE locks all three and adds the nonsymmetric oracle.
- [x] Stage 4AD: do not implement `R^chi_ww`, physical `R_ww[gamma]`, the split-vs-direct identity gate, RHS, grid reads, or evolution wiring.
- [x] Stage 4AE: implement only the reviewed local away-axis `R^chi_ww` correction through a single-source non-forgeable input package.
- [x] Stage 4AE: add checked `p_chi=chi_x/x`, reuse checked `q_xz` and `p_W`, and retain hidden multiplicity `(2/W)D_wD_w chi`.
- [x] Stage 4AE: test constant `chi`, linear-`x`, linear-`z`, and nonsymmetric oracles plus invalid input rejection and a test-only Stage 4G difference comparison.
- [x] Checkpoint C / Claude Audit C: review Stage 4AE before Stage 4AF; require several nonsymmetric varying-`chi` points with nonzero `W` second derivatives rather than reusing only the Stage 4AE single-point comparison.
- [x] Stage 4AF: pass the internal hard identity gate `tilde R_ww + R^chi_ww == R_ww[gamma]` at constant-`chi`, linear-`x`, linear-`z`, and three nonsymmetric varying-`chi` points.
- [x] Stage 4AF: construct `gamma=h/chi` and its first/second derivative jet explicitly by product rules, then feed that already-physical jet to Stage 4G with `chi=1`.
- [x] Stage 4AF: verify nonzero conformal base-metric second derivatives cancel from direct `R_ww`, and retain invalid-input rejection through the Stage 4AC/4AE factories.
- [x] Stage 4AG: add a synthetic paired two-sided `h_xz` parity-validator primitive using hand-built near-axis samples; this is not actual grid-data parity validation.
- [x] Stage 4AG: validate `h_xz(-x,z) = -h_xz(x,z)` from one-to-one paired two-sided samples and validate `h_xz(0,z)=0` when explicit axis data is supplied.
- [x] Stage 4AG: verify `h_xz/x` is even across each pair while allowing the quotient to vary with radius, and reject even contamination, missing partners, invalid coordinates, and nonfinite data.
- [x] Stage 4AG: keep tolerances validation-only; do not clamp, replace divisions by epsilon, or claim full smoothness/finite-axis regularity.
- [x] Checkpoint D: review Stage 4AF and Stage 4AG together before permitting local-only Stage 4AH assembly; do not interpret the synthetic parity fixture as actual grid validation.
- [x] Stage 4AH: assemble local away-axis physical `R_ww[gamma]` from Stage 4AC plus Stage 4AE through a factory that single-sources both packages from one metric/conformal-factor jet.
- [x] Stage 4AH: expose conformal, conformal-factor, and physical parts while keeping finite-axis, grid parity/matching, RHS, and evolution flags false.
- [x] Stage 4AH: test flat, constant-cone, linear-`x`/linear-`z` conformal-factor, nonsymmetric direct-Stage-4G, and invalid-input cases.
- [x] Stage 4AI: place physical `R_ww[gamma]` into a typed local hidden Ricci/RHS contribution contract that accepts only the Stage 4AH result.
- [x] Stage 4AI: compute `2 h^ww R_ww[gamma]` and `chi 2 h^ww R_ww[gamma]` with hidden multiplicity two and same-point `chi` / `1/h_ww` data.
- [x] Stage 4AI: test flat, cone, linear-`x`, and nonsymmetric cases, non-aggregate result types, and rejection of a loose raw `R_ww` call shape.
- [x] Checkpoint E / Claude Audit E: review the Stage 4AI physical-object boundary, hidden multiplicity, and conformal-versus-physical contraction factors before Stage 4AJ.
- [x] Stage 4AJ: implement the local physical hidden lapse Hessian `D_wD_w alpha` using `gamma=h/chi`, not the conformal metric alone.
- [x] Stage 4AJ: add checked `p_alpha=alpha_x/x`, single-source it with checked `q`, `p_W`, `p_chi`, and positive `x`, `W`, `chi`, and determinant guards.
- [x] Stage 4AJ: test constant-lapse, flat linear-`x`, flat linear-`z`, varying-`chi`, and nonsymmetric oracles plus direct physical-form comparisons and invalid-input rejection.
- [x] Stage 4AJ: keep the source minus sign, full `A_ww` source block, RHS, grid reads, finite-axis support, and evolution wiring unimplemented.
- [x] Stage 4AK: implement the local hidden `A_ww` curvature/lapse geometric core `C_ww=-D_wD_w alpha+alpha R_ww[gamma]` from same-point Stage 4AH and Stage 4AJ packages.
- [x] Stage 4AK: expose `minus_dww_alpha`, `alpha_rww`, and `curvature_lapse_core` while keeping Z4, trace-free projection, outer `chi`, nonlinear A/K terms, RHS, and evolution false.
- [x] Stage 4AK: test flat, linear-alpha, constant-cone, varying-`chi`, nonsymmetric same-point composition, non-aggregate types, and invalid-input rejection.
- [x] Stage 4AL: implement `C_IJ=-D_ID_J alpha+alpha R_IJ[gamma]` for `xx,xz,zz,ww`, then compute the full 4D trace-free projection and outer `chi` source.
- [x] Stage 4AL: use Stage 4G/4I physical Ricci for visible components, Stage 4AH/4AK for hidden `ww`, and require Stage 4G and Stage 4AH `R_ww` agreement before assembly.
- [x] Stage 4AL: test flat, linear-lapse, constant-cone, nonsymmetric varying-`chi`/alpha, trace-free zero-trace, non-aggregate types, and invalid-input rejection.
- [x] Stage 4AL: keep Z4 terms, nonlinear A/K terms, full CCZ4 RHS, grid wiring, and evolution unimplemented.
- [x] Checkpoint F / Claude Audit F: review the Stage 4AL physical Hessian correction, `R_ww` agreement gate, trace-free projection, and nonsymmetric oracle before further source/RHS integration.
- [x] Stage 4AM: lock the `hat_Gamma^x` hidden-contraction derivation and map the GRChombo hatted-conformal-connection convention used by this project.
- [x] Stage 4AM: record that GRChombo evolves `vars.Gamma = chris.contracted + 2 Z_over_chi`, not a separately stored lower-index Z variable, and require future project code to name the encoded Z convention explicitly.
- [x] Stage 4AM: lock required Stage 4AN oracles `0`, `-3/4`, `-1`, and `-35/961`, plus the Gamma RHS term classification and GL hard-gate non-goals.
- [x] Stage 4AN: add local `hat_Gamma^x` implementation and contract tests using the Stage 4AM oracles.
- [x] Stage 4AN: compute `tilde_Gamma^x` from the full visible plus hidden Christoffel contraction, build `(A-W)/x` as `x Delta_xw` through the checked Stage 4U/4R path, and obtain `B/x` through the checked Stage 4X package.
- [x] Stage 4AN: expose base contraction, hidden contraction, `tilde_Gamma^x`, `Z_over_chi^x`, and `hat_Gamma^x`, and keep Gamma RHS, GL validation, grid regularity, finite-axis handling, and evolution unwired.

Evidence for checked Stage 4V-4AN hidden-sphere Ricci, physical `R_ww`,
trace-free curvature/lapse, and `hat_Gamma^x` work:
`docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`;
`docs/implementation/stage4_hidden_sphere_Rww_plan.md`;
`docs/derivations/stage4AM_hatGammaX_derivation.md`;
`code/BlackStringToy/CartoonCheckedHxzOverX.hpp`;
`code/BlackStringToy/CartoonCheckedDxhwwOverX.hpp`;
`code/BlackStringToy/CartoonConformalRww.hpp`;
`code/BlackStringToy/CartoonConformalFactorRww.hpp`;
`code/BlackStringToy/CartoonAwayAxisPhysicalRww.hpp`;
`code/BlackStringToy/CartoonPhysicalRwwRhsContract.hpp`;
`code/BlackStringToy/CartoonPhysicalHiddenLapseHessian.hpp`;
`code/BlackStringToy/CartoonAwwCurvatureLapseCore.hpp`;
`code/BlackStringToy/CartoonTraceFreeCurvatureLapseBlock.hpp`;
`code/BlackStringToy/CartoonHatGammaX.hpp`;
`code/BlackStringToy/tests/Stage4ACConformalRwwAssemblyTest.cpp`;
`code/BlackStringToy/tests/Stage4AFSplitVsDirectPhysicalRwwGateTest.cpp`;
`code/BlackStringToy/tests/Stage4ALTraceFreeCurvatureLapseBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4ANHatGammaXTest.cpp`;
`logs/PROJECT_LOG.md`; `logs/CODEX_LOG.md`.

- [x] Stage 4AO-A: background and analytic residual lock. Lock the exact uniform ingoing-GP black-string background, `r0`, compact-`z` period, background slicing gauge, evolution gauge-driver and initial-gauge startup family, `K_ij` sign, perturbation sector, and geometric observable; evaluate the continuum background residual; include the full `hat_Gamma^x` hidden contraction; verify `1/x` cancellations analytically; require 4AO-C frozen gauge and Stage 4AO-D live gauge to use that same pre-locked gauge family; no discrete or spectral work before this passes.
  - [x] `docs/derivations/stage4AO_A_uniform_gp_background_residual.md` locks the uniform ingoing-GP background, `z~z+L`, `k_n=2 pi n/L`, GRChombo `K_IJ` sign, frozen-GP zero-residual targets, moving-puncture lapse startup residual, and background `hat_Gamma^x` hidden contraction.
  - [x] Componentwise `A_IJ` residuals, full `hat_Gamma^x` RHS residual, fixed field-independent GP-holding lapse source `S_alpha(x)=+3 sqrt(r0/x^3)`, horizon-observable lock, positive-inner-radius domain, constraints, and `1/x` cancellation ledger are locked.
- [x] Stage 4AO-B: discrete operator preflight. The local harness demonstrates raw unmodified-RHS background residual convergence on the provisional `r0=1`, `x in [0.5,4.0]` domain, with target zero for verified geometric/scalar/constraint components and `-3 sqrt(r0/x^3)` for the unmodified live moving-puncture lapse equation; it does not use `S_alpha=+3 sqrt(r0/x^3)` to cancel measured finite-grid residuals before this raw convergence test. It also checks `delta hww` hidden-contraction isolation for `delta hat_Gamma^x`, hand-derived actual-discrete-RHS Jacobian-vector agreement, and z-coupled periodic-stencil parity-sector block diagonalization with a flipped-parity negative guard. No eigensolver or threshold search was added.
- [ ] Stage 4AO-C: the frozen-gauge spectral gate remains incomplete. All 13 interior rows and the inner pure-outflow endpoint pass. The outer one-hot transformed-amplitude selectors and scalar profiles are diagnostic only; full 13-component WKB jets, the rank-four mapped basis, nine-dimensional left nullspace, and compatible four-row PDE dual are missing. Outer implementation/validation, aggregate radial validation, boundary-bearing completion, exact quadratic-pencil representation, eigensolver, and downstream gates remain false.
- [x] Stage 4AO-C reuse inventory: inspect existing project/GRChombo/AHFinder/PETSc/stencil/diagnostic pieces before any eigensolver or operator implementation. Conclusion: PETSc exists through AHFinder `SNES/KSP`, but no ready project/SLEPc/PETSc eigensolver exists; AHFinder is adapter-useful for nonlinear MOTS diagnostics; the linearized MOTS map `delta U -> delta R_H` is missing; GRChombo derivative, boundary, interpolation, reduction, extraction, and logging utilities are adapter candidates; Stage 4AO-B stencil/projection code is validation scaffolding, not the full 4AO-C operator; next technical step is the frozen-gauge operator wrapper plus boundary-condition contract, not eigensolver work.
- [x] Stage 4AO-C wrapper/boundary-contract substep: add a validation-only frozen-gauge operator wrapper for the 13-variable state, exclude gauge perturbations, inventory implemented/reusable/missing RHS blocks, and define the `0<x_in<r0<x_out` radial boundary-condition contract. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This is not a complete operator and does not allow eigensolver work.
- [x] Stage 4AO-C first actual operator block: add the validation-only matrix-free GP-shift advection apply path `beta_GP^x d_x(delta u)`, with `beta_GP^x=sqrt(r0/x)`, for all 13 frozen-gauge perturbation variables. Add interior radial `d_x` and periodic `D_z/D_zz` scaffolding, endpoint-placeholder status, order/gauge-exclusion checks, zero-input checks, and wrong-beta/sign/order negative guards. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAdvectionBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This is still not a complete operator and does not allow eigensolver work.
- [x] Stage 4AO-C tensor shift-stretching substep: implement the non-advection GP-shift Lie derivative tensor-stretching block for `h_xx,h_xz,h_zz,h_ww,A_xx,A_xz,A_zz,A_ww`, with coefficients `-7 lambda/4`, `-5 lambda/4`, `-3 lambda/4`, and hidden `+5 lambda/4`, using the GRChombo `CCZ4RHS` convention and `d=4` trace-removal coefficient `1/2`. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTensorShiftStretchingBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. Scalars and hatted-Gamma slots are intentionally untouched, and this is still not a complete operator or eigensolver gate.
- [x] Stage 4AO-C algebraic metric/conformal-factor coupling substep: implement only `delta h_IJ <- -2 delta A_IJ` and `delta chi <- +(1/2) delta K` in the validation-only frozen-gauge wrapper, using the GRChombo `-2 alpha A_ij` and `(2/d) chi(alpha K-div beta)` convention with `d=4`, `alpha_GP=1`, `chi_GP=1`, and `K_GP=div beta_GP`. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAlgebraicCouplingBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This local block does not implement a reciprocal `A_IJ <- h_IJ`, remaining K/A dynamics, constraints, hatted-Gamma evolution, the full operator, or an eigensolver gate; the globally completed Ricci insertions are separate operator blocks.
- [ ] Stage 4AO-C rejected K-equation `A^2/K^2` substep: superseded after the committed-range audit showed that `alpha(A_IJ A^IJ + K^2/d)` belongs to GRChombo's `USE_BSSN` branch, not the locked project `USE_CCZ4` formulation. The former inverse-metric, `delta A_IJ`, and `K^2/d` coefficients were removed from the K row. Historical evidence remains in the logs only and is not an implemented CCZ4 claim.
- [x] Stage 4AO-C selected-CCZ4 K/Theta substep: implement only the K-output linearization of `K(K-2Theta)` at `K_GP=3 lambda/2`, `Theta_GP=0`, `alpha_GP=1`, and `delta alpha=0`, giving `output[K] += 3 lambda input[K] - 3 lambda input[Theta]`. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeKCCZ4BlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. The common GP-shift advection block remains separate.
- [x] Stage 4AO-C K physical-Ricci insertion substep: consume the validated Ricci trace assembly and add only `output[K] += delta R`, with `delta R = delta R_xx + delta R_zz + 2 delta R_ww`. The independent oracle central-differences nonlinear Stage 4G physical Ricci plus `K(K-2Theta)` and discriminates every required case from the rejected BSSN branch. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeKCCZ4BlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `code/BlackStringToy/tests/Stage4AOCRicciTraceFreeAssemblyTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This block remains the sole geometric-Ricci owner; the later encoded-Z and complete-row items supply the separately owned completion. Frozen-gauge lapse-Hessian variation vanishes; locked `Lambda=0` leaves no cosmological term.
- [x] Stage 4AO-C A-equation algebraic non-curvature substep: implement only the `A_IJ`-output linearization of `(K - 2 Theta)A_IJ - 2 h^KL A_IK A_LJ` about the locked GP background, including conformal inverse-metric variation, direct `K`/`Theta` coefficients where `A_IJ_GP` is nonzero, no spurious `A_xz` `K`/`Theta` coupling, and no hidden multiplicity factor on the componentwise `A_ww` equation. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAAlgebraicBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This remains a separately tested family; the later complete-row item combines it exactly once with advection, stretching, geometric Ricci, and encoded Z. The complete operator and eigensolver remain blocked by chi/metric rows and later gates.
- [x] Stage 4AO-C Theta-equation algebraic non-Ricci substep: implement only the Theta-output linearization of `0.5 * (((d - 1) / d)K^2 - A_IJ A^IJ)` about the locked GP background, including inverse-metric variation, hidden `ww` trace multiplicity two, and the `d=4` K coefficient. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaAlgebraicBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This remains a separately tested family; the later complete-row item supplies the completed Theta status. Full-operator and eigensolver gates remain missing.
- [x] Stage 4AO-C Theta `-K_GP deltaTheta` algebraic substep: implement only the Theta-output frozen-GP linearization of GRChombo's `0.5*(-2 Theta K)=-Theta K`, giving `delta Theta_RHS <- -(3 lambda/2) deltaTheta`; no `delta K` term is present because `Theta_GP=0`. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaMinusKBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This remains a separate family consumed by the later complete Theta assembler; full-operator and eigensolver gates remain missing.
- [x] Stage 4AO-C trace-free `delta A` subspace/projector contract: compute the locked linearized trace with hidden multiplicity two, `delta_tr_A = delta A_xx + delta A_zz + 2 delta A_ww + (7lambda/8)delta h_xx + (3lambda/8)delta h_zz - (5lambda/4)delta h_ww`, and provide a validation-only projector that subtracts `delta_tr_A/4` from `A_xx`, `A_zz`, and `A_ww` while preserving `A_xz` and non-A variables. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTraceFreeDeltaAProjectorTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This is required before any full-operator JVP/parity/spectral claim, but it does not make 4AO-C complete or production trace handling final.
- [x] Stage 4AO-C Ricci/curvature design preflight: document GRChombo's `CCZ4Geometry` physical lower/lower Ricci convention, the frozen-gauge simplification `delta(D_I D_J alpha)=0`, the decomposition `delta R_IJ = delta tilde R_IJ + delta R^chi_IJ + hidden/cartoon terms`, the oracle plan using Stage 4G finite-differenced nonlinear Ricci and the reviewed 4AC/4AE/4AH hidden `R_ww` split, and the recommendation to implement hidden `delta R_ww[gamma]` first. Evidence: `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; `external/GRChombo/Source/CCZ4/CCZ4Geometry.hpp`; `code/BlackStringToy/CartoonRicciInterface.hpp`; `code/BlackStringToy/CartoonConformalRww.hpp`; `code/BlackStringToy/CartoonConformalFactorRww.hpp`; logs. No `delta R_IJ` code is implemented by this preflight.
- [x] Stage 4AO-C hidden physical `delta R_ww[gamma]` Ricci substep: implement only the raw lower/lower hidden component on the locked frozen-GP background, validate it against finite differences of the Stage 4G metric-derivative physical Ricci engine and the reviewed Stage 4AH split path, and cover pure `delta h_ww`, pure `delta chi`, z-dependent scalar, radial visible-metric, and mixed radial/z off-diagonal perturbations with epsilon-plateau checks. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeHiddenRww.hpp`; `code/BlackStringToy/tests/Stage4AOCHiddenPhysicalDeltaRwwTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This is not visible Ricci, not trace-free projection, not Theta Ricci scalar, and not a complete operator.
- [x] Stage 4AO-C visible physical `delta R_xz[gamma]` Ricci substep: implement only the raw lower/lower visible off-diagonal component on the locked frozen-GP background, validate it against finite differences of the Stage 4G metric-derivative physical Ricci engine, and cover zero input, pure `delta h_xz` as a zero oracle, even-sector diagonal scalar input, radial/z mixed hidden input, and pure conformal-factor mixed derivative input. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxz.hpp`; `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxzTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This is the first one-z Ricci target and validates parity mixing; it is not `delta R_xx`, not `delta R_zz`, not trace-free projection, not Theta Ricci scalar, and not a complete operator.
- [x] Stage 4AO-C visible physical `delta R_zz[gamma]` Ricci substep: implement only the raw lower/lower visible diagonal `zz` component on the locked frozen-GP background, validate it against finite differences of the Stage 4G metric-derivative physical Ricci engine, and cover zero input, pure `delta h_zz`, one-z `delta h_xz`, z-dependent `delta h_ww`, mixed `delta chi`, z-dependent `delta h_xx`, and mixed radial/z cases with even-parity checks. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRzz.hpp`; `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRzzTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This uses the positive conformal-factor signs from `gamma=h/chi`; it is not `delta R_xx`, not trace-free projection, not Theta Ricci scalar, and not a complete operator.
- [x] Stage 4AO-C visible physical `delta R_xx[gamma]` Ricci substep: implement only the raw lower/lower visible radial `xx` component on the locked frozen-GP background, validate it against finite differences of the Stage 4G metric-derivative physical Ricci engine, and cover zero input, pure `delta h_xx`, pure `delta h_zz`, pure `delta h_ww`, one-z `delta h_xz`, mixed `delta chi`, z-dependent diagonal scalar input, and mixed radial/z cases with even-parity checks. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxx.hpp`; `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxxTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This uses the positive conformal-factor signs from `gamma=h/chi`; it is not raw Ricci trace/trace-free assembly, not trace-free A-curvature source, not Theta Ricci scalar, and not a complete operator.
- [x] Stage 4AO-C raw Ricci trace / trace-free assembly substep: consume the validated raw `delta R_xx`, `delta R_xz`, `delta R_zz`, and `delta R_ww` result types, assemble `delta R = delta R_xx + delta R_zz + 2 delta R_ww`, and project with the `d=4` trace-free convention after full raw assembly. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeRicciAssembly.hpp`; `code/BlackStringToy/tests/Stage4AOCRicciTraceFreeAssemblyTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This is not insertion into the `A_IJ` curvature source and not a complete operator.
- [x] Stage 4AO-C Theta Ricci scalar insertion substep: consume the validated Ricci trace assembly and add only `output[Theta] += 0.5 delta R`, with hidden multiplicity inherited from `delta R = delta R_xx + delta R_zz + 2 delta R_ww` and no `R_xz` scalar-trace contribution. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaRicciScalarBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This is not insertion of `[delta R_IJ]^TF` into the `A_IJ` curvature source, not Z4 damping/kappa handling, not a complete Theta RHS, and not a complete operator.
- [x] Stage 4AO-C A-equation Ricci curvature insertion substep: consume the validated trace-free Ricci assembly and add only `output[A_xx] += [delta R_xx]^TF`, `output[A_xz] += [delta R_xz]^TF`, `output[A_zz] += [delta R_zz]^TF`, and `output[A_ww] += [delta R_ww]^TF`, with no extra hidden multiplicity on the representative `A_ww` component. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. This uses the frozen-gauge simplification `delta(D_I D_J alpha)=0` and `delta(chi alpha R_IJ)=delta R_IJ`, but it is not Z4 damping/kappa handling, not a complete A RHS, and not a complete operator.
- [x] Stage 4AO-C Z4 damping/kappa convention lock: deliberately adopt the inherited GRChombo-facing main-path values `kappa1=0.1`, `kappa2=0`, `kappa3=1`, and `covariantZ4=true`; reserve zero damping for a later diagnostic comparison. This is a planning-layer decision implemented by Codex, not parameter tuning. Evidence: `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; logs. `code/BlackStringToy/params_stage2_smoke.txt` remains unchanged and is not the convention authority.
- [x] Stage 4AO-C simple K/Theta damping insertion substep: with `d=4`, implement only `output[Theta] += -0.5*kappa1*(5+3*kappa2)*input[Theta] = -0.25 input[Theta]` and `output[K] += -4*kappa1*(1+kappa2)*input[Theta] = -0.4 input[Theta]`. No `deltaK` input or other output participates. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeCCZ4DampingInsertionTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. The corrected K pieces remain unchanged, and this separate K/Theta damping block writes no hatted-Gamma output. The complete hatted-Gamma rows are now assembled and independently validated from their separate families.
- [x] Stage 4AO-C hatted-Gamma evolution design preflight: map the exact selected `USE_CCZ4` GRChombo `rhs.Gamma[i]` equation; derive both frozen-GP linearized rows with common advection ownership, `d=4` K/Theta/chi gradients, background shift-gradient and shift-Hessian terms, hidden multiplicity-two connection-A/vector-Hessian terms, `delta Z_over_chi^i=0.5(delta hat_Gamma^i-delta tilde_Gamma^i)`, locked damping, and parity. The selected momentum-constraint form has no `d1.A`. Evidence: derivation, GRChombo source, and logs. The subsequent helper, partial blocks, complete assembler, and nonlinear oracle implement and validate both frozen-gauge Gamma rows. Later checklist items close the other rows and full interior assembly/JVP/parity; radial-boundary validation, the boundary-bearing complete-operator gate, and eigensolver remain missing.
- [x] Stage 4AO-C contracted-connection/Z reconstruction helper: implement validation-only `g_x`, full hidden-aware `g_z`, and `Z_i=0.5(H_i-g_i)` without assuming the determinant constraint or writing RHS output. Evidence: helper/tests/docs/logs. The helper remains non-RHS; the complete Gamma assembler consumes its downstream partial block. Full interior assembly/JVP/parity is complete; radial-boundary validation, the boundary-bearing complete-operator gate, and eigensolver access remain missing.
- [x] Stage 4AO-C first hatted-Gamma Z/kappa and `kappa3` shift-gradient insertion: consume the validated `g_i,Z_i` helper and add only `output[hat_Gamma^x] += (3 lambda/4)g_x - 0.2 Z_x + (lambda/2)H_x` and `output[hat_Gamma^z] += (3 lambda/4)g_z - 0.2 Z_z`. The common GP-shift advection block remains the sole owner of `beta_GP^x partial_x H_i`; there is no `+(lambda/2)H_z`. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeHatGammaZ4KappaBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. The fixture independently derives coefficients from `K0=3 lambda/2`, `d=4`, `kappa1=0.1`, and `kappa3=1`, and rejects wrong damping sign, hidden multiplicity one, z-direction H coupling, duplicate advection, parity leakage, and false narrow-block completion. All surviving Gamma families and the complete assembler are implemented and validated. Full interior assembly/JVP/parity is complete; radial-boundary validation, the boundary-bearing complete-operator gate, and eigensolver remain missing.
- [x] Stage 4AO-C hatted-Gamma K/Theta/chi-gradient insertion: add only `output[hat_Gamma^x] += 2 dx(deltaTheta) - (3/2)dx(deltaK) - 27 lambda delta h_xx/(8x) + (7 lambda/2)dx(delta chi)` and `output[hat_Gamma^z] += 2 dz(deltaTheta) - (3/2)dz(deltaK) - 27 lambda delta h_xz/(8x) + (3 lambda/2)dz(delta chi)`. The metric terms are the inverse-metric variation of `h^{ij} partial_j K_GP` with `partial_x K_GP=-9 lambda/(4x)`. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeHatGammaGradientBlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. The focused oracle derives all coefficients independently from the selected d=4 equation and rejects wrong signs, omitted metric variation, swapped chi coefficients, parity leakage, non-Gamma writes, and duplicated earlier Gamma/advection terms. All surviving Gamma families and the complete assembler are implemented and validated; the later K/Theta/A item records the expanded row-completion scope. Full interior assembly/JVP/parity is complete; the radial-boundary and boundary-bearing complete-operator gates and eigensolver remain closed.
- [x] Stage 4AO-C hatted-Gamma connection-A insertion: add only `output[hat_Gamma^x] += C_x` and `output[hat_Gamma^z] += C_z`, where `C_i=2 A_{GP}^{IJ} delta Gamma^i_{IJ}` and the two hidden `ww` copies are explicit. The selected momentum-constraint form has no `d1.A`, while direct `delta A_IJ` vanishes because the background conformal Christoffels are zero. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeHatGammaConnectionABlockTest.cpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`; `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`; logs. The independent oracle reconstructs each visible and hidden linearized conformal Christoffel and contracts it with `A_GP^xx=-7 lambda/8`, `A_GP^zz=-3 lambda/8`, `A_GP^ww=5 lambda/8`; it rejects hidden multiplicity one, wrong derivative and `1/x` signs, direct/differentiated `delta A`, parity leakage, non-Gamma writes, and duplicated prior Gamma/advection blocks. Vector-Hessian, grad-div, and complete row assembly/validation are implemented. Full interior assembly/JVP/parity is complete; radial-boundary validation, the boundary-bearing complete-operator gate, and eigensolver remain closed.
- [x] Stage 4AO-C hatted-Gamma vector-Hessian and grad-div metric-variation insertion: keep both selected-GRChombo families separate and independently tested. Vector Hessian adds `-3 lambda delta h_xx/(4x)+3 lambda delta h_ww/x` only to x; grad-div adds `9 lambda delta h_xx/(8x)` and `9 lambda delta h_xz/(8x)`. These blocks remain separately testable and are now consumed once by the complete Gamma assembler.
- [x] Stage 4AO-C complete frozen-gauge hatted-Gamma rows: consume the existing common-advection output as a dependency, then sum the Z/kappa/shift-gradient, K/Theta/chi-gradient, connection-A, vector-Hessian, and grad-div family outputs exactly once without replacing their functions by a simplified formula. Evidence: `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`; `code/BlackStringToy/tests/Stage4AOCFrozenGaugeCompleteHatGammaRowsTest.cpp`; individual Gamma/helper tests; operator contract; docs/logs. The test-only nonlinear oracle directly evaluates the selected CCZ4 equation using a nonlinear inverse metric, visible/hidden Christoffels, contracted connection/Z reconstruction, raised A tensor, two hidden copies, and GP shift derivatives. The exact background residual is `x=-2.78e-17`, `z=0`; directed-family and multiple mixed perturbations pass the `1e-2,1e-4,1e-5,1e-6,1e-7` sweep with a stable `1e-5/1e-6` plateau. Genuinely isolated vector-Hessian x and grad-div z fixtures assert every partial family separately before their nonzero assembled targets. Every family omission/duplication, duplicate advection, spurious `lambda H_z/2`, hidden multiplicity, parity leakage, and non-Gamma write mutation is rejected. Gamma assembly/validation/family flags and `variable_rhs_complete(hat_Gamma^x/z)` are true; the generic Gamma Theta/Z and hidden-evolution classifications are `implemented_now`. At this checkpoint every non-Gamma variable-completion flag was false; the full-operator and eigensolver gates remain false.
- [x] Stage 4AO-C non-Gamma RHS closure preflight: from clean committed checkpoint `8cec8c0`, trace `CCZ4RHS::rhs_equation` and `CCZ4Geometry::compute_ricci_Z` for K, Theta, and `A_xx/A_xz/A_zz/A_ww`; record the term ledger and complete unsimplified targets. Existing raw Ricci owns only `delta R^g_IJ`; the selected branch additionally requires `q_xx=2dx Z_x`, `q_xz=dx Z_z+dz Z_x`, `q_zz=2dz Z_z`, `q_ww=2Z_x/x`, and `q=q_xx+q_zz+2q_ww`. Exact row insertions are K `+q`, Theta `+q/2`, and A `+[q_IJ]^TF`. Common advection and A shift stretching are implemented but unassembled; lapse-Hessian, `-Z dot grad(alpha)`, `A_xz` Theta coupling, and locked-`Lambda=0` terms vanish. The preflight added no code/tests and left every non-Gamma/full/eigensolver gate false.
- [x] Stage 4AO-C encoded-Z Ricci-completion tensor helper: add a validation-only opaque helper taking `(x,z_x,z_z,dx z_x,dz z_x,dx z_z,dz z_z)` and returning `q_xx`, `q_xz`, `q_zz`, the single representative `q_ww=2z_x/x`, `q=q_xx+q_zz+2q_ww`, and the `d=4` trace-free components. The independent analytic spherical modified-cartoon oracle checks every output, the two-copy trace but single-copy representative, weighted TF trace, even/one-z parity, zero jet, and hidden/projection mutations. The helper does not own geometric Ricci or derivatives of `Z_i=0.5(H_i-g_i)` and is not itself a row insertion. Its checkpoint deferred the nonlinear selected-branch finite-difference oracle and left every non-Gamma/full/eigensolver gate false; the later complete-row item records their current status.
- [x] Stage 4AO-C encoded-Z Ricci row insertions: consume the existing opaque completion and add only K `+=q`, Theta `+=q/2`, and `A_xx/A_xz/A_zz/A_ww += qTF_IJ` in separate partial point/grid blocks. The focused fixture checks isolated ownership, every coefficient, weighted trace-free A output, representative ww once, scalar/even and xz/one-z parity, size guards, and mutations for wrong weights, raw q, double projection, duplicated geometric Ricci, hidden doubling, and illegal writes. These blocks remain partial; at their checkpoint every non-Gamma variable-completion, full-operator, and eigensolver gate was false.
- [x] Stage 4AO-C encoded-Z derivative adapter: analytically differentiate the unreduced hidden-aware contracted-connection formulas without a determinant constraint, retaining the two hidden copies and every derivative of `1/x`, including the `1/x^2` terms. The separately testable adapter supplies `dx/dz Z_x` and `dx/dz Z_z` to the existing encoded-Z tensor helper and never numerically differentiates production code. Its independent full modified-cartoon contraction oracle checks mixed derivatives, parity, determinant independence, and sign/coefficient/multiplicity mutations.
- [x] Stage 4AO-C complete frozen-gauge K/Theta/A rows: consume common GP advection, geometric Ricci, encoded-Z completion, selected algebraic terms, locked K/Theta damping, and A shift stretching exactly once. Curvature is projected only in its existing geometric and encoded-Z owners; the complete assembler performs no second projection, representative `A_ww` is written once, and no direct A damping is introduced. A test-only nonlinear four-dimensional Cartesian analytic-jet oracle independently constructs physical Ricci, `Z_over_chi`, physical lower `Z_i=h_ij Z_over_chi^j`, covariant `D_i Z_j`, selected CCZ4 algebraic terms, trace-free curvature, GP advection, and tensor shift stretching. It validates the geometric-Ricci-plus-Z split path; source mapping identifies that path with selected CCZ4, and no independent direct Gamma-form Ricci is claimed. Analytic-jet GP residuals are zero except `Theta=-6.78e-21`. Two mixed directions pass the six-point `1e-2` through `1e-7` sweep with approximately 100-fold error reduction over the first two decades followed by roundoff saturation. A genuinely isolated nonzero geometric-Ricci direction has `Z=partial Z=0` and every unrelated family zero. The full metric-inclusive tangent case enforces `T=partial_x T=0`, has a nonzero metric contribution, and preserves `T(delta rhs)` at roundoff without completing metric rows. Every family omission/duplication, encoded-Z omission/double insertion, hidden multiplicity one, representative-ww duplication, adapter mixed/second/inverse-square mutation, parity leakage, and illegal write mutation is rejected. K, Theta, and `A_xx/A_xz/A_zz/A_ww` completion/inventory/validation flags are true. At that checkpoint chi/metric and full-interior gates remained false; the following two checklist items record their completed status. Boundaries, MOTS, eigensolver, and 4AO-D remain false.
- [x] Stage 4AO-C complete frozen-gauge chi/metric rows: assemble common GP advection, the existing tensor shift-stretching owner, and the existing `chi<-K/2`, `h_IJ<-2A_IJ` algebraic owner exactly once. The final rows are `chi=Adv+delta K/2` and `hxx/hxz/hzz/hww=Adv+(-7,-5,-3,+5)lambda delta h/4-2delta A`. The locked `K_GP=div beta_GP` identity removes the potential `delta chi` coefficient, and representative hww is written once. An independent analytic-jet nonlinear oracle covers background, pure/mixed directions, six epsilons, parity, determinant/tangent consistency, family mutations, ww ownership, and illegal writes. The chi and four metric variable-completion flags are true.
- [x] Stage 4AO-C full 13-variable interior assembly/JVP/parity: compose the chi/metric, K/Theta/A, and hatted-Gamma complete-row owners in the locked state order, consuming the supplied common-advection vector without recomputation. A test-only analytic-jet selected-branch evaluator independently constructs all 13 nonlinear rows. All GP residuals are zero or below `5.5e-20`; two mixed directions activate every row/family and pass `1e-2` through `1e-7`, with second-order convergence for nonlinear rows through `1e-4` and roundoff-level exact-linear chi/metric rows. Complete-row/advection omission and duplication, hidden/representative-ww, slot ownership, weighted tangent identities, and both explicit parity sectors pass with zero forbidden leakage. All 13 variable flags and only the interior assembly/JVP/parity gates are true; radial boundaries, boundary-bearing complete operator, MOTS, eigensolver, threshold work, and 4AO-D remain false.
- [x] Stage 4AO-C radial-boundary design preflight: trace the selected `USE_CCZ4` radial principal symbol and lock a blockwise physical/Z4 characteristic substitute. The light sectors have fixed-boundary speeds `1-sqrt(r0/x)` and `-1-sqrt(r0/x)`; shift-advected and frozen-longitudinal sectors have speed `-sqrt(r0/x)`. The frozen longitudinal sector contains a zero-normal-speed Jordan block, so no false complete characteristic basis is claimed. Every sector is outflow at `0<x_in<r0`; no inner continuum data are allowed, only second-order one-sided `D_x/D_xx/D_xz` closure and local determinant/weighted-trace cleanup. Its provisional scalar outer law and generalized layout remain diagnostic; no rank-nine WKB projector or polynomial pencil is locked. `k=0` remains a separate charge/gauge diagnostic. Every outer and downstream gate remains false.
- [x] Stage 4AO-C inner pure-outflow endpoint operator: implement explicit second-order one-sided `D_x` and `D_xx` coefficient/reach contracts and Fourier-amplitude `D_xz=D_xD_z` for both full parity sectors, with no stored radial ghost unknown or continuum data. The validation-only wrapper constructs endpoint jets, consumes the existing complete 13-variable interior assembler once, retains all 13 PDE rows, adds zero boundary equations, performs no reset/extrapolation, and keeps determinant/weighted-trace cleanup separate. Exact polynomial tests, smooth-profile order above `1.8`, wrong-sign/coefficient/reach/centered mutations, exact row-owner and representative-ww ownership, both parity sectors, zero leakage/commutator, and cleanup separation pass. At `x_in/r0={0.35,0.50,0.65,0.80}` all `c_+=1-v`, `c_-=-1-v`, and `c_0=-v` are outward; `x_in=r0` is glancing and rejected, and `x_in>r0` is invalid. The endpoint-symbol reflection measure decreases from `5.48e-5` at `dx=1/64` to `5.11e-8` at `dx=1/2048`. Only the inner helper/validation flags are true; outer, aggregate boundary, boundary-bearing complete-operator, MOTS, eigensolver, threshold, production, 4AO-D, and Checkpoint G gates remain false.
- [x] Stage 4AO-C outer `k>0` leading-asymptotic diagnostic: historical scalar profile powers and logarithmic derivatives are retained only as a rejected diagnostic record. The full stationary audit supersedes their coefficients. Its exact-rational `q=2/3` counterexample disproves the formerly claimed universal `s^5(s^2-k^2)^4` light factorization. Direct `W_b^in,J,F,G,C_h,C_A` conditions are diagnostic characteristic labels only; they are not the primary boundary condition. All outer and downstream gates remain false.
- [x] Stage 4AO-C stationary-symbol documentation audit: extract the complete sparse `M0,M1,M2` radial system, including `1/x`, `1/x^2`, encoded-Z, hidden-sphere, and Gamma terms, in both parity sectors. Independent comparison at `q={0.31,0.73,1.11}`, `X={7,13,29}`, and both parities has maximum source-versus-matrix mismatch `2.44e-16`. The listed right/left vectors are verified kernel vectors of a generically singular leading symbol with nullity three; that nullity neither establishes generalized chains nor excludes coincident leading vectors with distinct subleading series. A rank-four `A_-`, nine-row physical annihilator, and four-row PDE dual have not been derived. No endpoint or solver work is authorized; all outer and downstream gates remain false. Evidence: `docs/derivations/stage4AO_C_stationary_wkb_basis.md`.
- [x] Stage 4AO-C diagnostic outer transform/projector helper: implement the validation-only `Y=(U,D_xU)` characteristic map with hidden multiplicity two and single representative `ww` ownership, plus scalar profile diagnostics. The stored four columns are explicitly one-hot in the nominal outgoing amplitudes; their orthogonal complement tests only the selector-coordinate algebra. It does not validate decaying/growing WKB jets, Jordan exclusion, parity, constraints, or endpoint ownership. Diagnostic helper flags alone are true; all accepted outer and downstream gates remain false.
- [ ] Stage 4AO-C corrected `k>0` outer boundary: frozen as deferred research. The rejected patch added valid mirrored second-order outer derivatives and useful `13+0/13+0/4+9` radial row-layout scaffolding, but its one-hot scalar-profile columns do not define a physical WKB boundary. No accepted determinant factorization, branch classification, rank-four decaying basis, nine-row physical annihilator, or retained-PDE dual exists. Higher-order solvability or an independently justified regularized first-order radial system would be required before reconsidering the boundary. The nine direct characteristic selectors and four amplitude-coordinate PDE projections remain diagnostic only. No polynomial or nonlinear eigensolver work is authorized.
- [ ] Stage 4AO-D: live-gauge/full acceptance. Confirm the physical GL eigenvalue agrees with frozen gauge, separate physical/gauge/constraint modes, check constraint-subsystem decay against the derived CCZ4 formulation, check inner-boundary characteristics, seed the eigenvector in time evolution and reproduce the spectral growth rate, and run the full convergence battery.
- [ ] Stage 4AO hard rules: flat tests alone are insufficient; dedicated 4AO-B/C/D validation harnesses may construct and evaluate the actual RHS, perform finite-difference Jacobian checks, solve the frozen/live spectral systems, and run the seeded-eigenvector evolution bridge; production Stage 4AR/4AS integration remains blocked until 4AO-D; Checkpoint G passes only after 4AO-D; Pau is not the convention authority; the physical growth observable must be geometric and not `hat_Gamma^x` alone.
- [ ] Checkpoint G / Claude Audit G: review Stages 4AM-4AO-D. Checkpoint G passes only after 4AO-D, and Pau is not the convention authority.
- [ ] Stage 4AP: validate actual grid-level or ghost-cell regularity for `h_xz=O(x)`, `h_xx-h_ww=O(x^2)`, `W_x=O(x)`, and `chi_x=O(x)` using real grid or ghost data, not only hand-built polynomial fixtures.
- [ ] Stage 4AQ: implement finite-axis source evaluation and regularized limits with explicit analytic parity/limit treatment; do not use epsilon replacement or silent clamping, and keep turduckening of the physical singularity distinct from cartoon-axis regularity.
- [ ] Stage 4AR: integrate reviewed local RHS source blocks without live evolution wiring.
- [ ] Checkpoint H / Claude Audit H: review Stages 4AP-4AR before live evolution; hard blockers include real grid regularity validation, finite-axis source evaluation, and no smoke-only hidden freeze in the physics mode.
- [ ] Stage 4AS: add live evolution wiring only behind an explicit default-off parameter after Checkpoint H.
- [ ] Stage 4AT: remove or replace the smoke-only hidden freeze before any real hidden-sector physics path is used.
- [ ] Stage 4AU: perform the final Stage 4 exit review before any Stage 5/Pau diagnostic reproduction.
- [ ] Checkpoint I / Claude Audit I: final review of Stages 4AS-4AU, including default-off evolution controls and removal/replacement of the smoke-only hidden freeze in physics mode.
- [ ] Superseded duplicate: future hidden-sector RHS work must disable or replace the Stage 4D smoke-only freeze. Replacement owner is Stage 4AT.
- [ ] Derive/list remaining modified-cartoon source terms beyond local Stage 4AL. Owner: Stage 4AR controlled local RHS integration after 4AO-D/4AP/4AQ blockers.
- [ ] Identify exact production `CCZ4RHS` modification seam. Partial evidence in Stage 4H; production owner: Stage 4AR.
- [ ] Implement `hww/Aww` evolution equations. Owner: Stage 4AR/4AS after 4AO-D, 4AP, and 4AQ.
- [ ] Ensure production `K` trace and `A` tracelessness use `GR_SPACEDIM=4` including hidden components. Local evidence exists through Stage 4A/4L/4AL; production owner: Stage 4AR/4AS.
- [ ] Add real grid regularity handling near the cartoon axis. Owner: Stage 4AP/4AQ.
- [ ] Test stationary or near-stationary uniform black string with the actual coupled operator. Partial analytic/discrete evidence in 4AO-A/B; full owner: 4AO-C/4AO-D and later Stage 4AR validation.
- [ ] Monitor Hamiltonian/momentum constraints in the modified-cartoon path. Owner: 4AO-D constraint-subsystem check and Stage 4AR/4AS diagnostics.
- [ ] Check convergence on a small non-production grid. Owner: Stage 4AO-C/4AO-D validation harnesses, then Stage 4AR/4AS production wiring checks.
- [ ] Explicitly document silent-failure checks for production physics mode. Partial evidence in Stage 1, Stage 4D, and Stage 4AO notes; final owner: Stage 4AU exit review.

Evidence for checked Stage 4AO-A/B and 4AO-C inventory items:
`docs/derivations/stage4AO_A_uniform_gp_background_residual.md`;
`docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`;
`code/BlackStringToy/Stage4AOGPDiscretePreflight.hpp`;
`code/BlackStringToy/tests/Stage4AOBDiscreteOperatorPreflightTest.cpp`;
`code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`;
`code/BlackStringToy/Stage4AOFrozenGaugeZDerivativeAdapter.hpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeZDerivativeAdapterTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeCompleteKThetaARowsTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeAdvectionBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeTensorShiftStretchingBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeAlgebraicCouplingBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeKCCZ4BlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeAAlgebraicBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaAlgebraicBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaMinusKBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeTraceFreeDeltaAProjectorTest.cpp`;
`code/BlackStringToy/Stage4AOFrozenGaugeHiddenRww.hpp`;
`code/BlackStringToy/tests/Stage4AOCHiddenPhysicalDeltaRwwTest.cpp`;
`code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxz.hpp`;
`code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxzTest.cpp`;
`code/BlackStringToy/Stage4AOFrozenGaugeVisibleRzz.hpp`;
`code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRzzTest.cpp`;
`code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxx.hpp`;
`code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxxTest.cpp`;
`code/BlackStringToy/Stage4AOFrozenGaugeRicciAssembly.hpp`;
`code/BlackStringToy/tests/Stage4AOCRicciTraceFreeAssemblyTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaRicciScalarBlockTest.cpp`;
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`;
`docs/derivations/stage4AM_hatGammaX_derivation.md`;
`docs/implementation/stage4_hidden_sphere_Rww_plan.md`;
`logs/PROJECT_LOG.md`; `logs/CODEX_LOG.md`.

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

All unchecked Stage 5 items remain missing. Owner: Stage 5 after Stage 4AU
exit review. Stage 4AO-C's reuse inventory confirms AHFinder/PETSc pieces are
adapter candidates only; no Pau diagnostic reproduction is started.

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

All unchecked Stage 6 items remain missing. Owner: Stage 6 after credible Stage
5 diagnostics exist.

Gate: do not proceed to Stage 7 or claim reproduction complete until convergence/validation criteria are documented and reviewed and approved by the user.

## Stage 7 - Radiation/wave-extraction design

Goal: design radiation extraction after the horizon dynamics are robust.

- [ ] Decide radiation observable appropriate for 5D/SO(3)-reduced spacetime.
- [ ] Review whether public Weyl4 machinery is usable or purely 3+1D.
- [ ] Decide extraction surfaces/large-radius diagnostics.
- [ ] Identify gauge issues and asymptotic normalization.
- [ ] Define waveform, spectrum, energy by mode, and angular-distribution observables.
- [ ] Decide how Pau's collaboration code enters this phase.

All unchecked Stage 7 items remain missing. Owner: Stage 7 after Stage 5/6
horizon diagnostics are validated. Radiation extraction is not started.

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

All unchecked Stage 8 items remain missing. Owner: Stage 8 after Stage 7
formalism and validation tests are reviewed. No radiation computation is
started.

Gate: do not proceed to Stage 9 or interpret radiation results physically until extraction is benchmarked, convergence-tested, and reviewed and approved by the user.

## Stage 9 - Physics interpretation and writeup

Goal: turn validated simulations and radiation diagnostics into scientific conclusions.

- [ ] Summarize reproduction agreement/mismatches.
- [ ] Summarize radiation observables.
- [ ] Identify robust signatures of higher-dimensional GL transition.
- [ ] Quantify numerical/systematic uncertainty.
- [ ] Compare to PBH/higher-dimensional phenomenology motivation.
- [ ] Draft internal note/paper/thesis section.

All unchecked Stage 9 items remain missing. Owner: Stage 9 after validated
simulation and radiation diagnostics exist.

## Custom solver / GRChombo comparison batches

- [x] Batch 1: lock source/state/dimension mapping and compare directly
  callable conformal algebra, visible contracted connection, and physical
  Ricci at matched stock `d=3`.
- [x] Batch 2: compare raw Ricci, encoded Z, and every directly overlapping
  visible `chi,h,K,Theta,A` RHS family and combined row at matched `d=3`.
- [x] Batch 3: invoke actual custom order-two and GRChombo order-four
  derivative kernels on periodic manufactured profiles; verify all derivative
  orders, every one of the 15 visible advection rows, complete chi/metric/A
  shift RHS families, continuum row agreement to `4.91998803e-13`, and direct
  visible trace/positivity cleanup. Raw shift derivatives are input-kernel
  diagnostics, not isolated RHS-family evidence.
- [x] Batch 4: directly exercise stock `d=3` tensor/RHS/fourth-order
  derivative/moving-puncture-gauge paths at the GP setup seam; compare with
  independent stock/target term families and the actual full custom
  physical-`d=4` 13-row oracle plus `H,Mx,Mz`; distinguish coordinate
  `gamma_theta_theta=x^2` from stored `hww=1`; verify raw `-3 lambda`, a real
  lapse-only test adapter with zero 20-field Jacobian, setup convergence,
  mutation sensitivity, and the frozen-gauge Fourier/parity convention.
- [ ] Resolve the Chombo/container digest and execute the actual Chombo
  periodic-domain/ghost-fill path. The batch-3 local analytic ghost patch is
  direct kernel evidence only.
- [ ] Adapt and compare hidden-aware `d=4` determinant/A-trace cleanup; stock
  visible cleanup has no `hww/Aww` or multiplicity-two owner.
- [ ] Begin the first production adaptation seam only after provenance is
  pinned: target `d=4/2` state ownership, GP `BoxLoop` initial data,
  hidden/cartoon completion, lapse-only fixed source, hidden-aware cleanup and
  constraints, and compact-z ghost ownership before unperturbed evolution.
- [x] Production-adaptation preflight: lock the inspected GRChombo
  origin/commit in a tracked manifest; add a read-only wrong-commit/dirty-state
  verifier; lock the target 18-slot `d=4/2` state with no visible-`y` slots;
  define thin GRChombo adaptation points, the 13-row oracle seam, and staged
  audit checkpoints. This is design/dependency-source work only.
- [x] First production-contract substage: after dependency verification, add
  an isolated black-string 18-slot enum/name/metadata contract, assert
  `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and `DEFAULT_TENSOR_DIM=4`, and pass exact
  slot/name/parity/permutation/multiplicity and mutation tests. The old
  27-slot smoke scaffold remains live and unchanged; no GP or RHS path was
  added.
- [ ] Next production substage: implement GP initial-data values against the
  reviewed 18-slot contract and a dedicated black-string Vars mapping seam.
  Do not begin hidden RHS, cleanup/constraints, lapse source, periodic
  ownership, evolution, or diagnostics. The missing Chombo source/build tuple
  remains an execution blocker for an actual `BoxLoop`.
- [ ] Resolve and execute the complete Chombo/PETSc/container build tuple.
  The GRChombo source is locked, but full build reproducibility is not.
