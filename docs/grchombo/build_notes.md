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

`GR_SPACEDIM=4` direct-source preflight result: not attempted against `external/GRChombo`. Passing `-DGR_SPACEDIM=4` would collide with the source-level `#define GR_SPACEDIM 2` in `ApparentHorizonTest2D.cpp`; changing that external file would violate the Stage 1.5 constraint against modifying `external/GRChombo` source. The follow-up scratch-copy preflight below tests the one-line macro change in ignored `runs/` only.

First blocker: the baseline AH solve is skipped because `USE_AHFINDER` is undefined. The first target-specific `GR_SPACEDIM=4` blocker is that the preferred public test hard-defines `GR_SPACEDIM 2`, so a clean `GR_SPACEDIM=4` compile preflight is not available from this test without either a supported build-system override that supersedes the source define or a separate local copy/source edit, both of which were outside this task.

Failure classification:

- Missing tools: no for the verified Docker baseline.
- PETSc/AHFinder: the verified baseline used `USE_PETSC=FALSE`, and the substantive AH path skipped because `USE_AHFINDER` was undefined. PETSc availability in the Docker image has not been directly tested here. Enabling the full apparent-horizon solver requires `USE_AHFINDER`, which requires PETSc to link.
- `GR_SPACEDIM` guards: not reached in a `GR_SPACEDIM=4` compilation; source inspection found a hard `GR_SPACEDIM 2` definition in the test.
- Missing `hww/Aww` variables: not reached in a `GR_SPACEDIM=4` compilation.
- Build-system uncertainty: yes for `GR_SPACEDIM=4`; target-level makefile does not reveal an obvious safe override.

Source modification status: no GRChombo source files were modified, staged, or committed.

Generated outputs: the baseline build produced the ignored executable under `external/GRChombo/Tests/ApparentHorizonFinderTest2D/`. The skip-path run produced no AH stats, AH coordinates, HDF5, or checkpoint output.

Next action: resolve PETSc/AHFinder-enabled execution for the `DIM=2` test if needed, then review the scratch-copy `GR_SPACEDIM=4` compile result below before deciding whether to proceed toward Stage 2.

### Stage 1.5 GR_SPACEDIM=4 Scratch-Copy Preflight

Purpose: probe whether the public `ApparentHorizonFinderTest2D` target can compile with `CH_SPACEDIM=2` and `GR_SPACEDIM=4` after the minimal local macro change needed to avoid editing `external/GRChombo`. This is a compile-only preflight and does not validate 5D/SO(3) physical dynamics.

Scratch-copy location: `runs/stage1_5_preflight/GRChombo_GR4/`.

Exact scratch edit: in `runs/stage1_5_preflight/GRChombo_GR4/Tests/ApparentHorizonFinderTest2D/ApparentHorizonTest2D.cpp`, changed `#define GR_SPACEDIM 2` to `#define GR_SPACEDIM 4`.

Exact Docker command:

```bash
docker run --rm -v "$(pwd):/settings" grchombo/grchombo bash -lc '
cd /settings/runs/stage1_5_preflight/GRChombo_GR4/Tests/ApparentHorizonFinderTest2D
make clean DIM=2 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE || true
make -j2 all DIM=2 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE
'
```

Result: build succeeded. The scratch target produced `runs/stage1_5_preflight/GRChombo_GR4/Tests/ApparentHorizonFinderTest2D/ApparentHorizonTest2D2d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex`.

First blocker: none at compile time for this non-AHFinder `USE_PETSC=FALSE` target.

Failure/error classification:

- First meaningful compiler/build error: none; command exited 0.
- `GR_SPACEDIM == 3` guards: no compile failure triggered.
- Missing `hww`, `Aww`, `c_hww`, or `c_Aww`: no compile failure triggered.
- AH geometry assumptions: no compile failure triggered in this build.
- Cartoon/AH-relevant code reach: the build reached and compiled `ApparentHorizonTest2D.cpp` and linked the executable, but the substantive AH solver path is still not compiled/exercised because `USE_AHFINDER` is undefined and `USE_PETSC=FALSE`.

Interpretation: a scratch-copy `DIM=2`, `GR_SPACEDIM=4` compile of this public non-AHFinder 2D test is technically viable. This is a narrow compile result only. It does not test modified-cartoon CCZ4 source terms, does not exercise the PETSc-backed AH solver path, and does not establish physical correctness.

Next recommended action: human review of this Stage 1.5 result, then decide whether Stage 2 can begin or whether to first run an additional CCZ4-side `DIM=2`, `GR_SPACEDIM=4` compile probe. PETSc/AHFinder-enabled execution can be deferred until before Stage 5 unless horizon-solver integration becomes necessary earlier.

### Stage 1.5B CCZ4-Side Target-Dimension Compile Preflight

Purpose: probe the CCZ4/evolution-side compile path for `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, where the modified-cartoon silent-failure risk lives. This is still a compile preflight only and does not validate physical dynamics.

Chosen target: `external/GRChombo/Tests/CCZ4Test`.

Why this target:

- It directly includes `CCZ4RHS.hpp`.
- It uses `CCZ4UserVariables.hpp` through its `UserVariables.hpp`.
- It instantiates `CCZ4RHS<MovingPunctureGauge, FourthOrderDerivatives>` through `BoxLoops::loop(...)`.
- It is smaller and more direct than matter or full-AMR examples.

Scratch-copy location: `runs/stage1_5_preflight/CCZ4_GR4/GRChombo/`.

`GR_SPACEDIM` setup: `CCZ4Test` does not hard-define `GR_SPACEDIM`. The default comes from `Source/utils/DimensionDefinitions.hpp`, where `GR_SPACEDIM` defaults to `3` unless a compiler definition is accepted. The test uses the stock `CCZ4UserVariables.hpp`, which declares the 3D-style CCZ4 variable set.

Exact Docker command:

```bash
docker run --rm -v "$(pwd):/settings" grchombo/grchombo bash -lc '
cd /settings/runs/stage1_5_preflight/CCZ4_GR4/GRChombo/Tests/CCZ4Test
make clean DIM=2 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE || true
make -j2 all DIM=2 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE CXXCPPFLAGS="-DGR_SPACEDIM=4"
'
```

Result: build failed while compiling `CCZ4Test.cpp`.

First meaningful error:

```text
CCZ4Test.cpp:43:28: error: no matching function for call to 'Chombo::IntVect::IntVect(int, int, int)'
```

Interpretation of first blocker: the public `CCZ4Test` harness is hard-coded around a 3D Chombo grid (`IntVect(0, 0, 0)`, 3D boxes, 3D loops, and 3-component analytic data), so it is not a clean `DIM=2` compile target without scratch-source adaptation. This failure occurs before a clean target-dimension CCZ4 preflight can be established.

Additional compile observations:

- The emitted compile command for `CCZ4Test.cpp` did not include `-DGR_SPACEDIM=4`, so `CXXCPPFLAGS` was not confirmed as a working GRChombo/Chombo override path for this target.
- The same failed compile also emitted `VarsTools.hpp:45:5: error: static assertion failed: Interval has wrong size` while instantiating `CCZ4RHS<MovingPunctureGauge, FourthOrderDerivatives>`.
- The interval-size failure is consistent with stock 3D-style `CCZ4UserVariables.hpp` intervals being incompatible with a `CH_SPACEDIM=2` tensor mapping without a reduced/cartoon variable layout.
- No `GR_SPACEDIM == 3` guard was the first blocker.
- No `hww`, `Aww`, `c_hww`, or `c_Aww` symbol error was the first blocker.
- The failure reaches CCZ4 template instantiation after the test-harness errors, but it does not reach a validated `GR_SPACEDIM=4` CCZ4 compile path because the requested compiler define was not observed in the emitted compile command.

Next recommended action: human review of this Stage 1.5B result. If a deeper CCZ4 compile probe is required before Stage 2, use an explicitly scratch-only minimal target or scratch-adapted `CCZ4Test` that removes 3D Chombo harness assumptions and proves the `GR_SPACEDIM=4` define path in the emitted compiler command.

## Stage 2A Editable BlackStringToy Scaffold

Purpose: create a project-controlled GRChombo example scaffold that can be
built from tracked files without modifying `external/GRChombo`.

Candidate chosen: public `external/GRChombo/Examples/BinaryBH`.

Why this candidate:

- It is the public example already validated in this repo through the Docker
  `params_very_cheap.txt` smoke path.
- It uses standard 3+1D CCZ4 infrastructure and GRChombo AMR machinery.
- It has a cheap parameter file suitable for a scaffold smoke test.
- It avoids starting from the Stage 1.5B `CCZ4Test` path, which failed on 3D
  test-harness assumptions before establishing a clean reduced/cartoon CCZ4
  compile path.

Tracked scaffold location: `code/BlackStringToy/`.

Scratch build script: `scripts/stage2_build_blackstringtoy_scratch.sh`.

The script creates `runs/stage2_blackstringtoy/GRChombo/`, copies
`external/GRChombo` there, injects tracked `code/BlackStringToy/` as
`Examples/BlackStringToy`, and builds inside Docker with `DIM=3`,
`DEBUG=FALSE`, `OPT=TRUE`, and `USE_PETSC=FALSE`.

Build status: succeeded after the user manually reran the scratch build script
from a shell with Docker available.

Command attempted:

```bash
bash scripts/stage2_build_blackstringtoy_scratch.sh
```

Observed executable:

```text
runs/stage2_blackstringtoy/GRChombo/Examples/BlackStringToy/Main_BlackStringToy3d_ch.Linux.64.mpicxx.gfortran.OPT.MPI.OPENMPCC.ex
```

Observed smoke-run result: the inherited one-step scaffold run completed and
`pout/pout.0` ended with `GRChombo finished.` The run produced four HDF5 files
under
`runs/stage2_blackstringtoy/GRChombo/Examples/BlackStringToy/hdf5/`:

- `BlackStringToyChk_000000.3d.hdf5`
- `BlackStringToyChk_000001.3d.hdf5`
- `BlackStringToyPlot_000000.3d.hdf5`
- `BlackStringToyPlot_000001.3d.hdf5`

It also produced inherited diagnostic text outputs under
`runs/stage2_blackstringtoy/GRChombo/Examples/BlackStringToy/data/`:

- `Weyl4_mode_20.dat`
- `Weyl4_mode_21.dat`
- `Weyl4_mode_22.dat`
- `constraint_norms.dat`
- `punctures.dat`

Failure classification: none for Stage 2A scaffold build/run. The earlier
Codex run failed only because Docker was not usable from that shell; the manual
rerun validates scaffold copy, Makefile compilation, executable discovery,
parameter parsing, and a short inherited smoke run.

Interpretation: this scaffold is not physical black-string evolution. It
inherits public BinaryBH initial data and standard 3+1D CCZ4 behavior only to
validate the editable build workflow. It does not implement 5D initial data,
compact `z`, GL perturbations, modified-cartoon source terms, `hww/Aww`,
horizon diagnostics, radiation extraction, or a physical `GR_SPACEDIM=4`,
`CH_SPACEDIM=2` evolution.

## Stage 2B Scratch Workflow Hardening

Purpose: make repeated Stage 2 scratch runs safe on WSL/Docker when Docker
creates root-owned files under `runs/stage2_blackstringtoy/`.

Script updated: `scripts/stage2_build_blackstringtoy_scratch.sh`.

Ownership strategy:

- Before deleting the scratch tree, the script verifies that the target is
  exactly `runs/stage2_blackstringtoy`.
- If existing scratch files are not owned by the current user, the script runs
  `sudo chown -R "$(id -u):$(id -g)" runs/stage2_blackstringtoy` before
  cleanup.
- If the ownership fix fails or `sudo` is unavailable, the script exits with a
  manual command instead of continuing into a partial cleanup.
- After Docker build/run completion, the script runs the same ownership fix so
  generated files are left removable by the normal WSL user.

This does not change `external/GRChombo`, `code/BlackStringToy`, or any physics
behavior. Generated build and run outputs remain under
`runs/stage2_blackstringtoy/`.
