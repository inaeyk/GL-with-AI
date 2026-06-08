# Project Outline

Long-term goal: compute gravitational-wave/radiation profiles during the 5D Gregory-Laflamme transition using a reproducible GRChombo-based workflow.

For stage-by-stage gates, see [stage_checklists.md](stage_checklists.md).

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
- Current status: in progress. The baseline `DIM=2` `ApparentHorizonFinderTest2D` Docker build/run path has been reproduced. The actual `GR_SPACEDIM=4` compile preflight remains open.
- Item-level gates: see [stage_checklists.md](stage_checklists.md).

## Stage 2: Editable Build And BlackStringToy Skeleton

- Goal: create an editable project example that can be built and tested without modifying upstream GRChombo source unexpectedly.
- Main deliverables: build notes, minimal `BlackStringToy` skeleton, smoke-test parameter files, run manifest template.
- Current status: not started.

## Stage 3: 5D Black-String Initial Data And GL Perturbation

- Goal: implement the baseline uniform black-string initial data and GL-triggering perturbation.
- Main deliverables: initial-data implementation, turduckening regularization, constraint-check outputs, short gauge-settling run.
- Current status: not started.

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
