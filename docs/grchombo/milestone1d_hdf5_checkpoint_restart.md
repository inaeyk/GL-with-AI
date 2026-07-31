# Milestone 1-D HDF5, checkpoint, and restart qualification

Date: 2026-07-31

Source checkpoint: `191dae42158f1f1f79514fea7d1d7008c70faaa5`

Result: `M1-D PASS — HDF5 checkpoint and restart qualified`

This report is the retained evidence for the bounded M1-D qualification.
`research_plan/stage_checklists.md` remains the current-status authority. No
production source, test, build file, parameter-file semantics, boundary
formula, gauge, KO implementation, MPI ownership rule, or external dependency
was changed.

## Existing path and fixed configuration

CodeGraph and the locked sources establish this ownership:

- `Main_BlackStringToy.cpp` constructs `GRAMR`, passes the real
  `DefaultLevelFactory<BlackStringToyLevel>` to `setupAMRObject`, and calls
  Chombo `AMR::run` and `AMR::conclude`.
- `setupAMRObject` selects `setupForNewAMRRun` or opens `restart_file` with
  `HDF5Handle::OPEN_RDONLY` and calls `AMR::setupForRestart`.
- Chombo `AMR` owns checkpoint/plot scheduling and the root time, iteration,
  and level metadata. `GRAMRLevel` owns the existing-format 18-field header
  and per-level state read/write. No custom format or validator was added.
- A valid restart level is read through `readCheckpointHeader` and
  `readCheckpointLevel`; `AMR::setupForRestart` calls `initialData` only for
  undefined levels above the restored finest level. The one restored level
  therefore bypasses the GP initializer.

All successful paths used the same executable, two MPI ranks, and:

| Quantity | Value |
|---|---:|
| `CH_SPACEDIM / GR_SPACEDIM / DEFAULT_TENSOR_DIM` | `2 / 4 / 4` |
| `N_x, N_z` | `8, 16` |
| physical lengths `L_x, L_z` | `1, 2` |
| `dx=dz` | `0.125` |
| boxes | `(0:7,0:7)` on rank 0; `(0:7,8:15)` on rank 1 |
| periodicity | radial nonperiodic; compact `z` periodic |
| AMR levels / ghost depth | `1 / 3` |
| GP / radial policy | exact GP / provisional physical radial boundary |
| KO / CFL / `dt` | `0.3 / 0.05 / 0.00625` |
| diagnostics / perturbation / AHFinder | `0 / none / disabled` |
| `t_mid`, `t_f` | `0.025`, `0.05` |
| continuous / split steps | `8 / 4+4` complete RK4 steps |

Both boxes retain all eight radial cells and split only along periodic `z`.
The global physical problem, decomposition, rank ownership, and evolution
parameters are identical between the continuous and split paths.

## Executable and reproducible commands

The optimized MPI/HDF5 configuration was built from ABI-clean objects with a
dedicated suffix:

```sh
cd code/BlackStringToy
make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=TRUE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx XTRACONFIG=.M1D_MPI_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/openmpi \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz' \
  NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE \
  USE_HDF=TRUE FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1D_MPI_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/openmpi \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz'
```

The executable is
`code/BlackStringToy/Main_BlackStringToy2d_ch.Linux.64.mpicxx.gfortran-15.OPT.MPI.OPENMPCC.M1D_MPI_HDF.ex`,
2,017,648 bytes, SHA-256
`4288e1213976841b7a113c37eb30a4a5a4dc35e5101b56768fc4b738b6b3464d`.
It links `libhdf5_openmpi.so.310`, `libmpi.so.40`, Fortran/OpenMP,
BLAS/LAPACK, and the C/C++ runtimes.

The three successful role commands were the same command shape:

```sh
OMP_NUM_THREADS=1 mpirun -np 2 bash -c \
  'exec /usr/bin/time -v -o <role>/time.${OMPI_COMM_WORLD_RANK}.txt \
  <absolute-M1D-executable> <absolute-role-parameter-file>'
```

The continuous file has `stop_time=0.05,max_steps=8`; the segment has
`stop_time=0.025,max_steps=4`; the restart has
`stop_time=0.05,max_steps=8` and the explicit absolute
`restart_file=/home/inaeyk/researchrepo/GL-with-AI/.m1d-work/segment/hdf5/m1d_mid_chk_000004.2d.hdf5`.
The restart
was launched with `.m1d-work/restart_cwd` as its working directory, distinct
from both the checkpoint directory and the parameter-file directory.
Parameter SHA-256 values are:

- continuous:
  `e811fe4d9749cee260f4ce955cdcaa2af864d5d143aee4abacf42f3fa1bd36d7`;
- checkpoint segment:
  `8c326a273a155b8d6a6df976034e090caddbbbfa39d22ceb90628d2c59cff4c4`;
- restart:
  `b6c9a0ba42ce1861534fb804a1e1a44555b2dd8a475813857175c9aad82b69e4`.

Their physics, grid, timestep, KO, gauge, boundary, and diagnostics entries
are identical. Only role-specific stop/step limits, output names/paths, plot
schedule, and `restart_file` differ. The complete provenance sidecar is
`docs/grchombo/milestone1d_provenance_manifest.json`.

## Checkpoint, restart, and plot evidence

The segment wrote
`m1d_mid_chk_000004.2d.hdf5` at iteration 4 and time 0.025. The successful
restart log reports:

- `AMR::restart`, `read cur_step = 4`, and `read cur_time = 0.025`;
- two restored boxes with the original global layout and rank map;
- no `BlackStringToyLevel::initialData` call on either rank;
- four RHS evaluations per restarted RK step, steps 4 through 7;
- final iteration 8, time 0.05, and `GRChombo finished.` on both ranks.

The checkpoint header and read gates enforce `num_components=18` and the exact
order
`chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,GammaX,GammaZ,lapse,shiftX,shiftZ,Bx,Bz`.
The level stores `dx=0.125`, `dt=0.00625`, time, the `(0:7,0:15)` problem
domain, radial/compact periodicity `0/1`, both boxes, processor metadata, and
the complete state. On restart, locked GRChombo intentionally recomputes
`dt` from the parameter-side CFL policy instead of trusting the stored value;
the recomputed and stored values are both exactly 0.00625.

The nonperiodic radial domain causes the existing checkpoint writer to retain
three ghost cells. This is an upstream format choice, not a project format.
Before every live restarted RHS, the normal lifecycle performs Chombo
intralevel/periodic exchange and then the project radial-only fill, so stored
ghost values are refreshed under the same ownership as a new run. Gauge
fields, `Theta`, and both Gamma fields are ordinary members of the 18 stored
slots and are not reset. KO is stateless and resumes with the unchanged
`ko_sigma=0.3`.

The final plot is a distinct existing-format file. It contains the same two
valid boxes and all 18 registered names, but has zero output ghosts and a
2,304-double state dataset. A checkpoint has three ghosts and a
7,056-double dataset. Both `h5ls -r` and `h5dump -H` open the files and expose
the expected root, `Chombo_global`, `level_0`, boxes, processors, offsets,
attributes, and data objects.

| Retained output role | Bytes | Whole-file SHA-256 |
|---|---:|---|
| continuous final checkpoint | 67,752 | `271222da68d22d9339992466054965fc65195fe8759ad7545258951634b82ddf` |
| midpoint restart checkpoint | 67,752 | `88ef0a6143eef945f3fc05b46462e8f5106089c1a6c2d255076e335fc5f23c02` |
| restarted final checkpoint | 67,752 | `321a7463ef5d7c4f773a571beb68212ac0dbf79783e1ddfa068762988971637f` |
| final plot | 29,664 | `ae9a6b894414516bfc606e521780a0d8b12a5ab73ea37f6b9e20c72ef6923430` |

All HDF5 files were created on 2026-07-31 in timezone `+08:00`; exact creation
times are retained in the sidecar.

## Continuous-versus-restarted equivalence

The comparison read the complete HDF datasets, used the stored boxes,
offsets, and ghost vector, selected each global valid cell exactly once, and
serialized deterministic `(z,x,component)` ordering. The continuous and
restarted valid-state SHA-256 is identically
`578829dffdf2b382917fd9950df2a566722d5fdd679ed1b302bebd2016ce8177`.
The initial valid-state SHA-256 used for the drift comparison is
`67d5e58be9e94e5fe770a06ecf3faa577d80bb8d144d5847702f55e637d69e2d`.

Weighted `L_2` means `sqrt(dx*dz*sum_valid(delta U)^2)`.

| Field | restart difference `L_inf` | restart difference weighted `L_2` | final GP drift `L_inf` | final GP drift weighted `L_2` |
|---|---:|---:|---:|---:|
| `chi` | 0 | 0 | 2.885233429683609e-5 | 1.544759624165498e-5 |
| `hxx` | 0 | 0 | 6.802800888237037e-4 | 3.551369645404414e-4 |
| `hxz` | 0 | 0 | 2.230423479629747e-18 | 1.880376730110331e-18 |
| `hzz` | 0 | 0 | 2.854216461756209e-5 | 1.529512557456109e-5 |
| `hww` | 0 | 0 | 3.542325250928124e-4 | 1.836295159075149e-4 |
| `K` | 0 | 0 | 1.844962626942737e-2 | 9.441335330452900e-3 |
| `Axx` | 0 | 0 | 8.207404790060213e-3 | 4.188638285153268e-3 |
| `Axz` | 0 | 0 | 4.290296896894569e-17 | 3.547115731469420e-17 |
| `Azz` | 0 | 0 | 4.623818493937781e-3 | 2.365623977602828e-3 |
| `Aww` | 0 | 0 | 5.168268483666028e-3 | 2.640901105938695e-3 |
| `Theta` | 0 | 0 | 8.844535374611875e-4 | 4.517526676146053e-4 |
| `GammaX` | 0 | 0 | 5.926274023576567e-3 | 3.116934200011368e-3 |
| `GammaZ` | 0 | 0 | 2.795207064670250e-17 | 1.771704948378604e-17 |
| `lapse` | 0 | 0 | 7.811959361959664e-4 | 4.000880854496304e-4 |
| `shiftX` | 0 | 0 | 6.516668146994142e-5 | 3.509909086853526e-5 |
| `shiftZ` | 0 | 0 | 3.328051612049500e-19 | 2.708315678660517e-19 |
| `Bx` | 0 | 0 | 5.703009170955488e-3 | 3.001032538904613e-3 |
| `Bz` | 0 | 0 | 2.763436473292637e-17 | 1.741350797215623e-17 |

The global restart differences are exactly zero, so no worst differing field
or cell exists. Continuous and restarted maximum determinant defects are both
`4.440892098500626e-16`; weighted trace-free defects are both
`2.220446049250313e-16`. Every valid value is finite. The largest short-run GP
drift is `K=1.844962626942737e-2`; the zero restart difference is therefore
also far below the discretization drift.

## Restart safety and existing limitations

- A missing path is rejected in locked `ChomboParameters` by an `access(...,
  R_OK)` parameter gate before `setupForRestart`. A direct structural check
  also returned nonzero with `unable to open file`.
- A 64-byte truncated copy returned nonzero from the same HDF5 1.14.6 open
  layer used by `HDF5Handle::OPEN_RDONLY`; it was not treated as a checkpoint.
- Locked `GRAMRLevel::readCheckpointHeader` rejects a component-count
  mismatch and missing or mismatched component names. The checkpoint also
  records `Chombo_global/SpaceDim=2`; incompatible typed box/data metadata
  cannot be read as this 2D layout.
- No new validator was built, and no checkpoint was mutated merely to retest
  these existing source-enforced gates. The launch cap was spent on the three
  required roles plus the output-directory retry described below.
- The upstream checkpoint does not embed the full project parameter file,
  executable/dependency hashes, compiler versions, or role. The retained
  sidecar supplies these. Parameter consistency is therefore proven by the
  sidecar and parameter hashes, not by unsupported upstream attributes.
- Rank-count-changing restart and AMR-refined restart remain outside M1-D.

The first restart invocation successfully loaded and evolved the state but
aborted in `AMR::conclude` when creating its final checkpoint because locked
`setupAMRObject` creates `hdf5_subpath` only for a new run, not a restart.
Precreating the declared restart output directory satisfied that documented
filesystem precondition; the fourth and final launch then passed. No code,
parameter, format, state, or numerical repair was made.

## I/O and memory baseline

These tiny timings are noisy. Wrapper wall time includes MPI startup,
initialization or restart, evolution, and output.

| Path | Wall time | Peak RSS by rank (KiB) | RK steps |
|---|---:|---:|---:|
| continuous | 0.29 s | `38,696; 38,720` | 8 |
| checkpoint segment | 0.23 s | `37,800; 37,640` | 4 |
| restarted continuation | 0.25 s | `38,452; 38,388` | 4 |

The optimized build does not emit a separable `setupForRestart` timer.
Consequently, the honest restart-load record is: less than the 0.25 s whole
restart-path wall time, with a standalone structural `h5dump -H` open below
the tool's 0.01 s display resolution. It is not misreported as an isolated
load measurement.

Filesystem birth-to-last-write spans provide approximate cached-write
baselines, not durable `fsync` costs:

| Output | Approximate span |
|---|---:|
| midpoint checkpoint | 2.519 ms |
| continuous final checkpoint | 0.276 ms |
| restarted final checkpoint | 0.704 ms |
| final plot | 0.594 ms |

Checkpoint and plot structural read-opens were both below the 0.01 s display
resolution. M1-E retains responsibility for production-scale and
instrumented I/O performance.

## Environment provenance and decision

- host: Linux `6.18.33.2-microsoft-standard-WSL2`, x86-64;
- C++/Fortran: GCC/GFortran 15.2.0;
- MPI: Open MPI 5.0.10;
- HDF5: 1.14.6 OpenMPI flavor, parallel HDF5 enabled;
- BLAS/LAPACK packages: 3.12.1-7ubuntu1;
- project / GRChombo / Chombo:
  `191dae42158f1f1f79514fea7d1d7008c70faaa5` /
  `37e659523830418b210acea1661dac0e00bb1b75` /
  `8684f2e000106f1abadb72642e1d15351867f98f`.

The real two-rank Chombo/GRChombo path writes readable existing-format
checkpoints and plots, resumes without reinitializing valid state, restores
the full 18-slot layout and timestep policy, refreshes ghosts normally, and
reproduces the uninterrupted final global valid state bitwise.

`M1-D PASS — HDF5 checkpoint and restart qualified`

M1-E may begin. This result is not an AMR-refinement, rank-count-changing
restart, scaling, or physical-GL claim.
