# GRChombo Build Notes

Purpose: confirmed build, Docker, and platform notes.

Status: first local smoke test complete; editable rebuild not yet verified.

## Confirmed

- The public `grchombo/grchombo` Docker image ran locally under Windows WSL2 Ubuntu.
- The `Examples/BinaryBH` very-cheap smoke test completed and produced HDF5 output under `runs/BinaryBH_verycheap/hdf5/`.
- Helper scripts now wrap the smoke-test flow:
  - `scripts/run_binarybh_smoke.sh`
  - `scripts/collect_binarybh_outputs.sh`
  - `scripts/plot_binarybh_outputs.sh` placeholder
  - `scripts/clean_binarybh_smoke.sh`
- Upstream GRChombo source is inspected locally under `external/GRChombo`; this directory is ignored and must not be committed.

## Next Build Milestone

- Perform an editable rebuild test of a small GRChombo example from source.
- Stage 1 action: run or inspect `external/GRChombo/Tests/ApparentHorizonFinderTest2D/`, because it explicitly defines `GR_SPACEDIM 2` and is likely the best public validation harness for lower-dimensional/string-like AH behavior.
- Confirm the local `CHOMBO_HOME`, compiler, HDF5, MPI/OpenMP, and optional PETSc/AH finder settings.
- Verify the executable naming convention produced by the editable build.

## AHFinderTest2D Run Status

- The upstream command appears to be `make test` followed by `make run` in `external/GRChombo/Tests/ApparentHorizonFinderTest2D`, with `DIM=2` and AH finder/PETSc support configured through Chombo `Make.defs.local`.
- The baseline `DIM=2` build/run path was later verified inside the public `grchombo/grchombo` Docker image.
- The verified baseline used `USE_PETSC=FALSE`.
- The Docker build produced `external/GRChombo/Tests/ApparentHorizonFinderTest2D/ApparentHorizonTest2D2d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex`.
- `make run` initially failed because OpenMPI refuses to run as root in Docker. The makefile hardcodes `mpirun -np 2 ./ApparentHorizonTest2D2d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex -q ApparentHorizonTest2D.inputs`.
- A manual run with `mpirun --allow-run-as-root -np 2` exited with `EXIT_CODE:0`, but printed `ApparentHorizon2D test skipped (USE_AHFINDER undefined).` once per MPI rank.
- No `stats_AH*`, `coords_AH*`, HDF5, or checkpoint files were produced in that skip path.
- Current status: baseline `DIM=2` compile and executable launch work. PETSc availability in the Docker image has not been directly tested here. The actual AH solver path is skipped because `USE_AHFINDER` is undefined; enabling the full apparent-horizon solver requires `USE_AHFINDER`, which requires PETSc to link.
- Scope note: PETSc/AHFinder-enabled execution is required before Stage 5 diagnostic reproduction, but it does not block Stages 2-4.

Do not begin `BlackStringToy` until the Stage 1.5 result is reviewed and approved by the user.

## Stage 1.5 Target-Dimension Compile Preflight

Purpose: test whether public GRChombo can compile controlled targets with `CH_SPACEDIM=2` and `GR_SPACEDIM=4` before any black-string physics implementation. This is a compile preflight only; even a successful compile would not show physical correctness because the 5D/SO(3) modified-cartoon CCZ4 source terms are still required.

Tested target: `external/GRChombo/Tests/ApparentHorizonFinderTest2D`.

Why this target:

- It is a public `DIM=2` test; its `GNUmakefile` sets `DIM = 2`.
- It uses `AHStringGeometry` through `UserVariables.hpp`.
- It is close to the horizon/string-geometry side of the project.
- It is not a CCZ4/cartoon evolution test.

Commands attempted or inspected:

- Baseline build:
  `docker run --rm -v "$(pwd):/settings" grchombo/grchombo bash -lc 'cd /settings/external/GRChombo/Tests/ApparentHorizonFinderTest2D && make -j2 all DIM=2 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE'`
- Initial run path: `make run` failed because OpenMPI refuses to run as root inside Docker. The target hardcodes `mpirun -np 2 ./ApparentHorizonTest2D2d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex -q ApparentHorizonTest2D.inputs`.
- Manual run:
  `docker run --rm -v "$(pwd):/settings" grchombo/grchombo bash -lc 'cd /settings/external/GRChombo/Tests/ApparentHorizonFinderTest2D && timeout 60s mpirun --allow-run-as-root -np 2 ./ApparentHorizonTest2D2d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex -q ApparentHorizonTest2D.inputs; echo EXIT_CODE:$?'`
- Inspected `external/GRChombo/Tests/ApparentHorizonFinderTest2D/GNUmakefile`: it sets `DIM = 2` and delegates to `$(CHOMBO_HOME)/mk/Make.test`; no local target-level compiler flag hook is obvious in this file.
- Inspected `external/GRChombo/Tests/ApparentHorizonFinderTest2D/ApparentHorizonTest2D.cpp`: it hard-defines `GR_SPACEDIM 2` before the GRChombo includes.

Baseline `DIM=2` build result: reproduced in Docker. The build compiled the `DIM=2` Chombo/GRChombo path and produced `ApparentHorizonTest2D2d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex`.

Baseline run result: reproduced with a manual OpenMPI root override. The executable exited with `EXIT_CODE:0`, but the substantive AH solver path did not run because `USE_AHFINDER` was undefined. The run printed `ApparentHorizon2D test skipped (USE_AHFINDER undefined).` once per MPI rank and produced no `stats_AH*`, `coords_AH*`, HDF5, or checkpoint output.

`GR_SPACEDIM=4` preflight result: not attempted as a compile. Passing `-DGR_SPACEDIM=4` would collide with the source-level `#define GR_SPACEDIM 2` in `ApparentHorizonTest2D.cpp`; changing that file would violate the Stage 1.5 constraint against modifying `external/GRChombo` source. The correct flag path or source-free override path needs investigation in a configured Chombo build environment.

GR_SPACEDIM=4 preflight remains open.

First blocker: the baseline AH solve is skipped because `USE_AHFINDER` is undefined. The first target-specific `GR_SPACEDIM=4` blocker is that the preferred public test hard-defines `GR_SPACEDIM 2`, so a clean `GR_SPACEDIM=4` compile preflight is not available from this test without either a supported build-system override that supersedes the source define or a separate local copy/source edit, both of which were outside this task.

Failure classification:

- Missing tools: no for the verified Docker baseline.
- PETSc/AHFinder: the verified baseline used `USE_PETSC=FALSE`, and the substantive AH path skipped because `USE_AHFINDER` was undefined. PETSc availability in the Docker image has not been directly tested here. Enabling the full apparent-horizon solver requires `USE_AHFINDER`, which requires PETSc to link.
- `GR_SPACEDIM` guards: not reached in a `GR_SPACEDIM=4` compilation; source inspection found a hard `GR_SPACEDIM 2` definition in the test.
- Missing `hww/Aww` variables: not reached in a `GR_SPACEDIM=4` compilation.
- Build-system uncertainty: yes for `GR_SPACEDIM=4`; target-level makefile does not reveal an obvious safe override.

Source modification status: no GRChombo source files were modified, staged, or committed.

Generated outputs: the baseline build produced the ignored executable under `external/GRChombo/Tests/ApparentHorizonFinderTest2D/`. The skip-path run produced no AH stats, AH coordinates, HDF5, or checkpoint output.

Next action: resolve PETSc/AHFinder-enabled execution for the `DIM=2` test if needed, then investigate a source-free way to compile a `DIM=2`, `GR_SPACEDIM=4` AH target. If the only route is editing or copying the public test source, stop for review before doing so.
