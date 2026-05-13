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

Do not begin `BlackStringToy` until `docs/grchombo/source_map.md` and `docs/grchombo/capability_map.md` are reviewed and approved by the user.
