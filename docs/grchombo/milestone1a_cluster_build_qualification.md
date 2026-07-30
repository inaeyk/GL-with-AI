# Milestone 1-A cluster build qualification

Date: 2026-07-30

Source checkpoint: `8c7c90a48e1dc924b0988b6ecee1b61c25d49896`

Result: `M1-A PASS — MPI/HDF5 build baseline qualified`

This report is retained build and launch evidence. Current status remains
authoritative in `research_plan/stage_checklists.md`.

## Ownership traced before building

- `Main_BlackStringToy.cpp` calls GRChombo `mainSetup`, constructs `GRAMR`,
  passes a `DefaultLevelFactory<BlackStringToyLevel>` to `setupAMRObject`,
  calls `GRAMR::run`/`conclude`, and finishes with `mainFinalize`.
- GRChombo `setupAMRObject` constructs the Chombo `ProblemDomain`, calls the
  real `AMR::define`, selects periodic directions, installs checkpoint/plot
  intervals and prefixes when `CH_USE_HDF5` is present, and calls
  `setupForNewAMRRun`.
- Chombo `AMR` owns time integration and plot/checkpoint scheduling.
  `GRAMRLevel` owns the existing Chombo-format checkpoint and plot hooks.
  BlackStringToy adds only `prePlotLevel`, which invokes its existing
  post-step diagnostic hook before a plot.
- `UserVariables.hpp` registers the project-owned 18-slot ordering. The
  existing checkpoint header writes `num_components=18` and every registered
  name. No project-owned output format is present or needed.
- `code/BlackStringToy/GNUmakefile` uses Chombo's `MPI`, `USE_HDF`,
  `MPICXX`, `HDFMPIINCFLAGS`, and `HDFMPILIBFLAGS` controls. Required Chombo
  libraries are `AMRTimeDependent`, `AMRTools`, `BoxTools`, and the
  automatically appended `BaseTools`.

No project-owned source or build-file adaptation was required.

## Environment and dependency provenance

- Host: Linux `6.18.33.2-microsoft-standard-WSL2`, x86-64.
- C++: `/usr/bin/g++`, GCC 15.2.0,
  package `g++-15=15.2.0-16ubuntu1`.
- Fortran: command `gfortran-15`, resolving to
  `/usr/bin/gfortran-15`, GCC 15.2.0,
  package `gfortran-15=15.2.0-16ubuntu1`.
  No compiler path is embedded in repository files.
- MPI: `/usr/bin/mpicxx` and `/usr/bin/mpirun`, Open MPI 5.0.10,
  packages `openmpi-bin=5.0.10-1` and
  `libopenmpi-dev=5.0.10-1`. The wrapper reports base compiler `g++`,
  compile includes under `/usr/lib/x86_64-linux-gnu/openmpi/include`, and
  link flags `-L/usr/lib/x86_64-linux-gnu/openmpi/lib -lmpi`.
- HDF5: 1.14.6 OpenMPI flavor, parallel HDF5 enabled,
  package `libhdf5-openmpi-dev=1.14.6+repack-2`.
  `h5pcc` supplies `/usr/include/hdf5/openmpi` and
  `/usr/lib/x86_64-linux-gnu/hdf5/openmpi`.
- BLAS/LAPACK: `libblas-dev=3.12.1-7ubuntu1` and
  `liblapack-dev=3.12.1-7ubuntu1`.
- GRChombo commit: `37e659523830418b210acea1661dac0e00bb1b75`.
- Chombo commit: `8684f2e000106f1abadb72642e1d15351867f98f`.

## ABI separation and exact clean-build commands

Chombo's normal configuration name distinguishes MPI from serial but does not
encode `USE_HDF`. Each matrix entry therefore used a unique `XTRACONFIG`
suffix. That suffix propagates to application object/dependency directories,
all four Chombo static-library names, and the executable name. Each entry was
real-cleaned for its exact configuration before its optimized build. No
MPI/non-MPI or HDF5-on/off object or library was shared.

From `code/BlackStringToy`:

```sh
make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=FALSE \
  FC=gfortran-15 CXX=g++ XTRACONFIG=.M1A_SERIAL_NOHDF NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE \
  USE_HDF=FALSE FC=gfortran-15 CXX=g++ \
  XTRACONFIG=.M1A_SERIAL_NOHDF

make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=FALSE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1A_MPI_NOHDF NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE \
  USE_HDF=FALSE FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1A_MPI_NOHDF

make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=TRUE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1A_MPI_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/openmpi \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz' \
  NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE \
  USE_HDF=TRUE FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1A_MPI_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/openmpi \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz'
```

Chombo `realclean` removes every application executable in the current
directory, not only the selected configuration. After all three clean builds,
the first two executables were restored from their already isolated
configuration objects/libraries with the corresponding second `make`
commands above. This does not mix ABI variants.

## Compile and link flags

The application objects use:

```text
-std=c++17 -O2 -fopenmp
-DCH_SPACEDIM=2 -DGR_SPACEDIM=4 -DDEFAULT_TENSOR_DIM=4
-DCH_Linux -DCH_USE_SETVAL -DCH_USE_COMPLEX -DCH_NAMESPACE
-DCH_USE_64 -DCH_USE_DOUBLE -DCH_USE_LAPACK -DCH_FORT_UNDERSCORE
```

MPI adds:

```text
-DCH_MPI -DMPICH_SKIP_MPICXX -ULAM_WANT_MPI2CPP -DMPI_NO_CPPBIND
```

HDF5 adds:

```text
-DCH_USE_HDF5 -I/usr/include/hdf5/openmpi
```

The locked Chombo library objects use:

```text
-march=native -O3 -std=c++14
-Wno-unused-but-set-variable -Wno-long-long -Wno-sign-compare
-Wno-deprecated -ftemplate-depth-99 -Wno-unused-local-typedefs
-Wno-literal-suffix -Wno-variadic-macros -fopenmp
```

Chombo Fortran objects use:

```text
-march=native -O3 -Wno-line-truncation -fno-second-underscore
-Wno-unused-parameter -Wno-unused-variable -fopenmp
```

Every link includes the matching configuration-specific static
`AMRTimeDependent`, `AMRTools`, `BoxTools`, and `BaseTools` libraries, then
`-lgfortran -lm -lgomp -lblas -llapack`. MPI linkage adds `libmpi.so.40`
through `mpicxx`; MPI/HDF5 linkage additionally adds
`-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz` and resolves
`libhdf5_openmpi.so.310`.

## Build matrix and executable evidence

| Configuration | Result | Executable | Bytes | SHA-256 |
|---|---|---|---:|---|
| serial, HDF5 off | build passed | `code/BlackStringToy/Main_BlackStringToy2d_ch.Linux.64.g++.gfortran-15.OPT.OPENMPCC.M1A_SERIAL_NOHDF.ex` | 1,704,312 | `f037399a4db71935e5290c3f7a6f33cb8a97e360ba0aeb25beb422786c2b46dc` |
| MPI on, HDF5 off | build passed; closure rebuild passed | `code/BlackStringToy/Main_BlackStringToy2d_ch.Linux.64.mpicxx.gfortran-15.OPT.MPI.OPENMPCC.M1A_MPI_NOHDF.ex` | 1,815,848 | initial: `111ca7a5523bd2a446e3edcab6c4905473ac9d48c86292ac2037355d9b7d6494`; closure rebuild: `0c732866912a6c0f2f3055e8d1f635e1a6b961f7de08bff81656f0632087f368` |
| MPI on, HDF5 on | build passed | `code/BlackStringToy/Main_BlackStringToy2d_ch.Linux.64.mpicxx.gfortran-15.OPT.MPI.OPENMPCC.M1A_MPI_HDF.ex` | 2,017,648 | `6a3f521cd8d1a05da31aa2eb2807b1780247d2f915e517a36c934e9467360f19` |

`readelf` confirms that the serial binary needs neither MPI nor HDF5, the
MPI/no-HDF5 binary needs `libmpi.so.40` but no HDF5 library, and the MPI/HDF5
binary needs both `libmpi.so.40` and `libhdf5_openmpi.so.310`.

## Bounded launch and output checks

All checks used exact GP, `ko_sigma=0.3`, the provisional radial boundary,
one AMR level, fourth-order derivatives, three ghosts, one OpenMP thread,
and `constraint_diagnostic_cadence=0`. Runtime overrides selected
`dx=1/8`, one step of `dt=0.00625`, and no plots. The HDF5 check enabled one
checkpoint interval.

1. **Serial one-step smoke — passed.** An 8-by-8 grid reached the real AMR
   setup (`GRAMRLevel::define`, `AMR::setupForNewAMRRun`), completed all four
   RK RHS evaluations, updated 64 valid cells, concluded, and printed
   `GRChombo finished.` No NaN, invalid metric, ABI error, or assertion was
   reported.
2. **MPI one-rank one-step smoke — passed.** Open MPI reported
   `number_procs = 1`, initialized with thread support `single`, reached the
   same AMR path, completed the step, concluded, and finalized cleanly.
3. **Initial MPI two-rank one-step smoke — rejected during startup.** Open MPI reported
   `number_procs = 2`. The load map placed boxes 0–1 on rank 0 and boxes 2–3
   on rank 1, proving both ranks participated. The attempted smallest
   decomposition split the 8-by-8 domain into four 4-by-4 boxes. Both ranks
   then aborted in `BlackStringToyLevel::initialData` while
   `fillAllEvolutionGhosts` called
   `BlackStringPerturbativeRadialBoundary::fill_solution_ghosts`.
   Chombo `Box.H:2044` rejected an index below the local box lower bound.
   The four-cell radial patch is narrower than the live one-sided radial
   closure's source footprint. This was an illegal test decomposition, not an
   application build or runtime defect. A later legal-layout retry closed the
   MPI-2 gate, as recorded below.
4. **MPI/HDF5 one-rank write and structural open — passed.** The run wrote
   initial and step-1 Chombo checkpoints. The retained step-1 evidence was
   39,528 bytes with SHA-256
   `5e66c715be611fe593f44dea2cd122720d782ed28a2636735f1f3e24eee2b87a`.
   `h5dump -H` opened it successfully and found the root header, `level_0`,
   boxes, processor ownership, and level data. The root reports
   `num_components=18` and exactly:
   `chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,GammaX,GammaZ,lapse,shiftX,shiftZ,Bx,Bz`.
   Level metadata reports `dx=0.125`, `dt=0.00625`, radial nonperiodicity,
   compact periodicity, and the 8-by-8 problem domain.

The HDF5 file itself is a generated checkpoint and is not committed; the
size, hash, complete metadata listing, and structural inspection are retained
here.

## MPI-2 retry preflight and closure

The sole retry used the prescribed legal decomposition:

- `N1=8`, `N2=16`, and `L=2` give
  `dx=L/max(N1,N2)=1/8`.
- `max_grid_size=8` and `block_factor=4` split the domain into two 8-by-8
  boxes along periodic `z`; the radial extent is not split.
- Both boxes touch both physical radial boundaries and retain eight valid
  radial cells, exceeding the provisional closure's five-cell source
  footprint.
- Two boxes permit one nonempty box per MPI rank.

The two 8-by-8 boxes span the complete radial index interval and meet only at
an internal periodic-`z` seam. Consequently, no physical radial boundary is
applied at that seam; radial-boundary work remains confined to the two global
radial edges. Both ranks own one nonempty box.

Before the launch, the optimized MPI/no-HDF5 configuration was rebuilt once
from ABI-clean objects with the exact qualified command:

```sh
make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=FALSE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1A_MPI_NOHDF NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE \
  USE_HDF=FALSE FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1A_MPI_NOHDF
```

The rebuilt executable is 1,815,848 bytes with SHA-256
`0c732866912a6c0f2f3055e8d1f635e1a6b961f7de08bff81656f0632087f368`.
Its linked-library set matches the initial qualification exactly:
`libmpi.so.40`, Fortran/OpenMP, BLAS/LAPACK, C++/C runtime libraries, and no
HDF5 library. The hash differs from the initial
`111ca7a5523bd2a446e3edcab6c4905473ac9d48c86292ac2037355d9b7d6494`
because locked Chombo `CHOMBO_VERSION.cpp` embeds the compilation
`__DATE__` and `__TIME__`; the rebuild contains
`MayDay version 3.2 built Jul 30 2026 at 20:56:00`. The build configuration,
binary size, and linked libraries are unchanged.

Exactly one two-rank launch was then run:

```sh
OMP_NUM_THREADS=1 mpirun -np 2 \
  ./Main_BlackStringToy2d_ch.Linux.64.mpicxx.gfortran-15.OPT.MPI.OPENMPCC.M1A_MPI_NOHDF.ex \
  tests/chombo_fourier_growth/params_d11_ref.txt \
  N1=8 N2=16 L=2.0 max_grid_size=8 block_factor=4 \
  stop_time=0.00625 max_steps=1 checkpoint_interval=0 plot_interval=-1 \
  constraint_diagnostic_cadence=0 verbosity=3 \
  print_progress_only_to_rank_0=1 output_path=/tmp/m1a_mpi2_legal/
```

Open MPI reported two processes. The deterministic load map assigned box 0
to rank 0 and box 1 to rank 1, and each rank reported one of two boxes.
Both ranks reached `GRAMRLevel::define`, `AMR::setupForNewAMRRun`,
`BlackStringToyLevel::initialData`, and
`GRAMRLevel::fillAllEvolutionGhosts`. Four live RK
`GRAMRLevel::evalRHS` calls completed, 128 global valid cells were updated,
the post-step and conclude paths completed, and both rank logs ended with
`GRChombo finished.` The registered state remains the qualified 18-component
`UserVariables` layout. No assertion, invalid metric, NaN, deadlock, ABI
error, or boundary-footprint failure occurred.

## Final decision and retained interruptions

All three ABI-separated builds, serial startup, MPI-1 startup, legal MPI-2
startup/one-step evolution, and MPI/HDF5 write/read-open checks pass.
Therefore:

`M1-A PASS — MPI/HDF5 build baseline qualified`

Two nontechnical interruptions remain in the evidence ledger but are not
active blockers:

1. The first MPI-2 layout created illegal four-cell radial patches, narrower
   than the provisional closure's five-cell source footprint.
2. The initially qualified MPI/no-HDF5 executable was removed before the
   legal retry, requiring the authorized clean rebuild recorded above.

M1-B may begin. Serial/MPI numerical equivalence and distributed derivative,
exchange, and boundary-seam acceptance remain explicitly deferred to M1-B
and M1-C.
