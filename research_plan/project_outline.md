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

## Stage 3: 5D Black-String Initial Data And GL Perturbation

- Goal: implement the baseline uniform black-string initial data and GL-triggering perturbation.
- Main deliverables: initial-data implementation, turduckening regularization, constraint-check outputs, short gauge-settling run.
- Current status: design/tooling in progress. Stage 3A documents the target uniform-string initial data, GL perturbation, variable map, turduckening plan, and unresolved implementation decisions. Stage 3B documents the modified-cartoon hidden-direction source-term derivation and validation roadmap. Stage 3C adds a `docs/derivations/` SymPy scaffold for machine-checking basic Christoffels, GP extrinsic curvature, angular contractions, and hidden-direction trace multiplicity; the symbolic geometry check now passes and its output is recorded in the derivation notes. Stage 3D extends the symbolic scaffold to a diagonal nontrivial `gamma_ww = q(x,z)` metric and verifies hidden-sector Christoffel identities, angular Ricci multiplicity, and flat Ricci limits. Stage 3E adds a nonconstant `q(x,z)` warped-product Ricci regression that exercises hidden-sector derivative terms. Stage 3G translates the diagonal physical cartoon variables into conformal `chi`, `h_AB`, and `hww` bookkeeping and verifies derivative/tracelessness identities. A polished Stage 3A PDF exists under `docs/physics_notes/`; initial Stage 3B, Stage 3C, Stage 3D, Stage 3E, and Stage 3G LaTeX sources have been drafted, with PDF generation deferred until after review. No Stage 3 source implementation has started.

## Stage 4: 4+1 CCZ4 / SO(3) Modified Cartoon Implementation

- Goal: evolve the 5D black-string system with the needed symmetry reduction and gauge choices.
- Main deliverables: 4+1 CCZ4/cartoon implementation notes, gauge/damping configuration, AMR strategy, regression smoke tests.
- Current status: not started.

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
