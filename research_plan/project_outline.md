# Project Outline

Long-term goal: compute gravitational-wave/radiation profiles during the 5D Gregory-Laflamme transition using a reproducible GRChombo-based workflow.

For stage-by-stage gates, see [stage_checklists.md](stage_checklists.md).
Physics and physics-design stages also produce polished review notes under
`docs/physics_notes/` when they contain substantive physics reasoning.

## Stage 0: Infrastructure And Reproducibility

- Goal: establish a stable local development and smoke-test environment.
- Main deliverables: repo scaffold, Git ignore policy, smoke-test scripts, metadata logs, archived PDF snapshots, and local Docker validation.
- Current status: essentially complete. Infrastructure, repo scaffold, local WSL/Docker smoke test, logs, and smoke-test scripts are in place.

## Stage 1: Public GRChombo Capability/Source Map

- Goal: map existing public GRChombo examples, formulation code, AMR patterns, and diagnostics relevant to a 5D black-string implementation.
- Main deliverables: capability map, source-entry notes, reusable implementation checklist.
- Current status: complete and user-approved. The public GRChombo capability/source mapping and hazard documentation have been reviewed and approved by the user.
- Note: this public-source exercise is an AI-agent capability benchmark. Pau's original code is available through collaboration but intentionally not used yet. The intended evolution keeps SO(3) symmetry and is effectively 2+1D, with the S^2 reconstructed by symmetry.
- Later collaboration-code phase: compare against or build on Pau's actual code for radiation extraction after the public-source benchmark stage is complete.

## Stage 1.5: Target-Dimension Compile Preflight

- Goal: check whether the intended compile configuration `CH_SPACEDIM=2`, `GR_SPACEDIM=4` is technically viable before physics implementation. This represents a 2+1D computational grid for 4+1D physics, with SO(3) symmetry of the transverse `S^2` absorbed into modified-cartoon source terms.
- Main deliverables: documented baseline `DIM=2` build/run result, documented `GR_SPACEDIM=4` compile preflight result or blocker, and updated stage gate status.
- Current status: complete and user-reviewed for the purpose of starting Stage 2A. The baseline `DIM=2` `ApparentHorizonFinderTest2D` Docker build/run path has been reproduced, and an AH-side scratch-copy `GR_SPACEDIM=4` compile preflight succeeded with `USE_PETSC=FALSE`. A CCZ4-side `CCZ4Test` scratch-copy probe failed on 3D test-harness assumptions before establishing a clean `GR_SPACEDIM=4` CCZ4 compile path. PETSc/AHFinder-enabled execution remains unresolved and can be deferred unless horizon-solver integration becomes necessary earlier.
- Item-level gates: see [stage_checklists.md](stage_checklists.md).

## Stage 2: Editable Build And BlackStringToy Skeleton

- Goal: create an editable project example that can be built and tested without modifying upstream GRChombo source unexpectedly.
- Main deliverables: build notes, minimal `BlackStringToy` skeleton, smoke-test parameter files, run manifest template.
- Current status: in progress. Stage 2A has produced an editable `BlackStringToy` scaffold derived from the public `Examples/BinaryBH` smoke-test path. The scratch build and one-step smoke run succeeded under `runs/stage2_blackstringtoy/`. This scaffold is only for build/run workflow validation and is not physical black-string evolution.

## Stage 3: 5D Black-String Initial Data Plus Modified-Cartoon Geometry Derivation/Validation

- Goal: design the baseline uniform black-string initial data, GL-triggering perturbation, and reduced modified-cartoon geometry/source-term bookkeeping before implementation.
- Main deliverables: initial-data design notes, modified-cartoon source-term and geometry derivation notes, symbolic validation scripts, polished physics notes, and validation gates for later implementation.
- Current status: complete. Stage 3A documents the target uniform-string initial data, GL perturbation, variable map, turduckening plan, and unresolved implementation decisions. Stage 3B documents the modified-cartoon hidden-direction source-term derivation and validation roadmap. Stages 3C-3E add symbolic geometry and Ricci gates for cartoon reductions, hidden-sector multiplicity, and nonconstant `gamma_ww=q(x,z)` derivative terms. Stage 3F translates the diagonal physical cartoon variables into conformal `chi`, `h_AB`, and `hww` bookkeeping. Stage 3G extends the conformal-cartoon bookkeeping to an off-diagonal reduced metric with `h_xz` / `gamma_xz` and adds sheared-flat off-diagonal Ricci gates. Stage 3H decomposes the future CCZ4 RHS into separately checkable blocks. Stage 3I documents small-`x` parity, axis matching, the distinction between cartoon-axis regularity and the co-located physical singularity, removable-singularity limits, and the assembled `tilde_Gamma^x` / `hat_Gamma^x` connection guard. Stage 3J designs the unit-test fixture layer. Stage 3K defines Stage 4A as the minimal first C++ implementation stage, with GRChombo-facing convention authority documented and full RHS, gauge, damping, turduckening, AH, radiation, production runs, and external/reference comparison deferred.

## Stage 4: 4+1 CCZ4 / SO(3) Modified Cartoon Implementation

- Goal: evolve the 5D black-string system with the needed symmetry reduction and gauge choices.
- Main deliverables: 4+1 CCZ4/cartoon implementation notes, gauge/damping configuration, AMR strategy, regression smoke tests.
- Current status: in progress through narrow, reviewed setup stages. Stage 4A is complete and committed: it adds local-value-only conformal-cartoon algebra helpers and standalone non-grid tests. Stage 4B is complete and committed: it adds a public GRChombo CCZ4 baseline-layout check. Stage 4C is complete and committed: it adds real repo-owned `hww/Aww` variables, header-level placement assertions, and a non-grid placement fixture. The full scratch Docker/GRChombo `BlackStringToy` scaffold now compiles with the 27-variable layout. Stage 4D adds only smoke-only finite scaffold support for `hww/Aww`: the default-off `scaffold_freeze_hidden` parameter is enabled by the cheap smoke file to freeze `hww = 1.0`, `Aww = 0.0`, and inert hidden RHS slots. Stage 4E is the first safe grid-to-helper handoff diagnostic: a default-off `scaffold_check_hidden_handoff` path reads the actual scaffold grid slots into the Stage 4A local algebra helper and checks finite determinant/inverse/trace/`K_ij` outputs without using those outputs for evolution; a standalone distinct-value mapping fixture catches swapped helper input slots that could remain finite in the symmetric smoke data. Stage 4F added a local-value metric-derivative cartoon Ricci helper interface and compile/type fixture. Stage 4G implements the first local away-axis metric-derivative cartoon Ricci helper with flat, constant-`q`, nonconstant-`q`, sheared-flat, and `x = 0` rejection checks. Stage 4H documents that the inherited GRChombo CCZ4 RHS uses a Gamma-based `CCZ4Geometry::compute_ricci_Z` path. Stage 4I adds a typed bridge contract with a `451` full-4D trace oracle so future RHS-facing code has a named convention boundary; after review, raw cartoon Ricci component storage is private and bridge exposure is the normal crossing point. Stage 4J defines the first local Ricci-to-RHS contract using only bridge-approved Ricci data and a standalone `451` trace-oracle fixture. Stage 4K adds a local RHS source-block skeleton that consumes the Stage 4J contract and exposes named inert output fields without implementing source formulas. Stage 4L implements only the local trace-free Ricci projection block from bridge-approved Ricci data, including the full 4D trace, hidden `ww` multiplicity, and `/4` projection denominator; it is still not wired into evolution. Stage 4M adds a named away-axis policy for finite `x > 0` checks and guarded `1/x` / `1/x^2` helpers, while still deferring true Stage 3I small-axis regularization. Stage 4N adds guarded away-axis singular-combination helpers for future `d_x f / x` and `(f - g) / x^2` factors without adding physical RHS terms. Stage 4O locks the current axis semantics as away-axis-only, with no clamp, epsilon replacement, or regularized-axis mode, and keeps `1/x^2` as a separately guarded primitive so future clamped/regularized behavior must define it independently from `1/x`. Stage 4P now exposes only the low-risk `(d_x hww) / x` away-axis primitive as a public raw output; the risky `(hxx - hww) / x^2` ingredient is source-facing only through the Stage 4R guarded path. Stage 4Q adds a local pointwise guard that catches clear violations of that matching condition before future near-axis source-block use, but still does not prove analytic regularity or implement finite axis limits. Stage 4R adds the first regularity-guarded local source-style sub-block: it calls the Stage 4Q guard before packaging the Stage 4P ingredient and the guarded metric-difference value. Stage 4S connects that Stage 4R package into the local RHS source-block skeleton so guarded geometry ingredients can be carried through the source-block layer without exposing raw Stage 4P metric-difference output; after review, the guarded package and carry output are checked-by-construction rather than open aggregates. Stage 4T adds the first guarded consumer as a diagnostic/probe: it accepts only the checked package type and does not implement Ricci, RHS physics, grid reads, or evolution wiring. Stage 4U adds the authoring gate requiring future formulas that need `(hxx - hww) / x^2` to consume the checked package or gate input, not raw `h_xx`, `h_ww`, and `x`; after review, the remaining generic-helper or hand-written bypass is documented as convention-only until a future review/lint gate exists. Stage 4V deliberately adds no source formula because no tiny authoring-gate consumer is yet derivation-locked to an existing validated Ricci/RHS expression; the next step is to extract the exact sub-expression, coefficient, sign convention, and oracle before coding. Stages 4D-4V must not be treated as physical evolution correctness or full cartoon Ricci/RHS integration.

## Stage 5: Pau Diagnostic Reproduction

- Goal: reproduce the first scientific milestone: Pau / Figueras-Andrade-Franca-Gu style 5D black-string horizon diagnostics.
- Main deliverables: `L10_med` reproduction, horizon-analysis pipeline, multi-`L` sweep, generation tables, Fourier diagnostics, reproduction memo.
- Current status: not started.

## Stage 6: Convergence And Validation

- Goal: establish that the reproduction diagnostics are numerically robust.
- Main deliverables: `L10_low/med/high` campaign, area convergence, `R_min` convergence, pinch-off fit stability, Fourier/generation-time convergence notes.
- Current status: not started.

## Stage 7: Radiation/Wave-Extraction Design

- Goal: choose and validate a radiation-extraction strategy appropriate for the 5D GL transition.
- Main deliverables: extraction design memo, observable definitions, boundary/large-radius requirements, validation tests.
- Current status: not started.

## Stage 8: Waveform/Spectrum/Radiated-Energy/Angular-Distribution Computation

- Goal: compute the radiation observables from validated simulations.
- Main deliverables: waveform pipeline, spectra, radiated energy by mode, angular distribution, dependence on generation times and Fourier modes.
- Current status: not started.

## Stage 9: Physics Interpretation And Paper/Thesis Writeup

- Goal: interpret the numerical results and prepare publishable documentation.
- Main deliverables: physics narrative, figures/tables, uncertainty budget, paper or thesis chapters.
- Current status: not started.
