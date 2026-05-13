# BinaryBH Very-Cheap Smoke Test

## Goal
Verify that the GRChombo Docker image runs locally under Windows WSL2 and produces HDF5 output.

## Environment
- Platform: Windows WSL2 Ubuntu
- Architecture: x86_64
- Docker image: grchombo/grchombo
- Example: GRChombo Examples/BinaryBH
- Parameter file: params_very_cheap.txt

## Command Summary
```bash
docker pull grchombo/grchombo
docker run -v "$(pwd):/settings" -it grchombo/grchombo
cd BinaryBH
./Main_BinaryBH3d_ch.Linux.64.mpicxx.gfortran.DEBUG.OPT.MPI.OPENMPCC.ex params_very_cheap.txt
cp -r hdf5 /settings/runs/BinaryBH_verycheap/
```

## Output
Raw HDF5 files are stored locally in:

```
runs/BinaryBH_verycheap/hdf5/
```

These files are ignored by Git.

## Status
Succeeded. HDF5 files were produced locally.
