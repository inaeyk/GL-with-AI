# Milestone 1-E performance and scaling baseline

Date: 2026-07-31; focused final-state gate completed 2026-08-01

Source checkpoint: `0dad588ed01fe3ebdb512e0dae294836b30d106f`

Result: `M1-E PASS — performance and scaling baseline qualified`

This is a local engineering baseline for the committed KO-stabilized,
level-zero GP path. It is not a physical-mode, AMR, or cluster-scale result.
No production source, build file, parameter semantics, boundary, dependency,
or checkpoint format changed.

## Environment and build provenance

- Host: local WSL2, Linux `6.18.33.2-microsoft-standard-WSL2`; no scheduler
  allocation.
- CPU: Intel Core Ultra 9 275HX, one socket, 24 physical cores and 24 hardware
  threads, one exposed NUMA node; `numactl` is unavailable. Ranks 0--7 were
  bound one per distinct core, so the reported `p=1,2,4,8` data are not
  oversubscribed. This hybrid, frequency-managed laptop environment remains
  a limitation on interpreting efficiency.
- Cache: 1.1 MiB L1d, 1.5 MiB L1i, 72 MiB L2, and 36 MiB L3 as reported by
  `lscpu`.
- Memory at inventory: 16,435,032,064 bytes installed,
  13,381,390,336 bytes available, and 4,294,967,296 bytes swap.
- C++ / Fortran: GCC 15.2.0 / GFortran 15.2.0.
- MPI: Open MPI 5.0.10, `mpicxx` backed by `g++`.
- HDF5: 1.14.6 OpenMPI flavor, parallel HDF5 enabled.
- BLAS/LAPACK: Ubuntu packages 3.12.1-7ubuntu1.
- Project / GRChombo / Chombo commits:
  `0dad588ed01fe3ebdb512e0dae294836b30d106f` /
  `37e659523830418b210acea1661dac0e00bb1b75` /
  `8684f2e000106f1abadb72642e1d15351867f98f`.

Two ABI-clean optimized executables were built once and reused for all
applicable launches:

| Configuration | Bytes | SHA-256 |
|---|---:|---|
| MPI, HDF5 off | 1,815,848 | `4ba7bd911e1d5ff19bbb3470408c578858e725bcb255abc005f328aa9b0cae3f` |
| MPI, HDF5 on | 2,017,648 | `2f007b029c171890c6e5333fe997f333b2f43f4ddb87f86afc29ecf91e85e596` |

The no-HDF5 binary resolves MPI, GFortran, OpenMP, BLAS, and LAPACK and no
HDF5 library. The HDF5 binary additionally resolves
`libhdf5_openmpi.so.310`. Application C++ uses `-std=c++17 -O2 -fopenmp`;
locked Chombo libraries use `-march=native -O3 -std=c++14 -fopenmp`.
Both use `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and
`DEFAULT_TENSOR_DIM=4`.

The focused acceptance repair made one fresh optimized MPI/HDF5 build solely
to materialize final valid states. Rank 1 and rank 8 shared the same
2,017,648-byte executable, SHA-256
`f866d1a5827af5156abca0b47d4dbd3352325a938b1f31eea676d3ec96c4c7d1`,
and parameter file, SHA-256
`602501e451ab07508fb953317a787a614bc7220c1127e079f286daa060b601d8`.
The build uses the same optimization, MPI, Fortran, BLAS/LAPACK, dimension,
and production source configuration as the authoritative no-HDF5 benchmark;
HDF5 and a step-25 plot were enabled only for capture. Its timings do not
replace any baseline timing above.

The reproducible build commands were:

```sh
make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=TRUE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1E_MPI_HDF NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE \
  USE_HDF=TRUE FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1E_MPI_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/openmpi \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz'

make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=FALSE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1E_MPI_NOHDF NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE \
  USE_HDF=FALSE FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1E_MPI_NOHDF
```

## Timing policy

Every evolution launch used `OMP_NUM_THREADS=1`, `mpirun --bind-to core
--map-by core`, `ko_sigma=0.3`, the provisional radial boundary, one level,
exact GP, and `constraint_diagnostic_cadence=0`. There were 25 RK4 steps:
steps 0--4 were warm-up and steps 5--24 were timed. Each timed interval
therefore contains 20 RK steps and 80 RHS stages. No plot or checkpoint was
written in an evolution launch.

The primary time is the sum of locked `AMR.cpp`'s per-step `MPI_Wtime`
measurement, not wrapper time. Dispersion is the sample standard deviation
of the 20 step times. Full-process wrapper time, compilation, startup,
initialization, and conclude are excluded from scaling ratios.

The launch form was:

```sh
OMP_NUM_THREADS=1 mpirun --bind-to core --map-by core --report-bindings \
  --output-filename RUN/mpi -np P /usr/bin/time -v \
  blackstring_mpi_nohdf.ex params_evolution.txt [grid overrides] \
  output_path=RUN/data/
```

## Strong scaling

The fixed problem is `N_x=256`, `N_z=512`, `L=8`,
`x in [0.5,4.5]`, `z in [0,8]`, `dx=dz=1/64`, CFL 0.05,
`dt=0.00078125`, and `t_f=0.01953125`. `max_grid_size=64` makes 32
`64 x 64` boxes: four radial by eight compact boxes. The layout has both
radial and compact internal seams, every boundary box has 64 radial cells,
and the common global boxes do not change with rank count. Ownership is
exactly 32, 16, 8, and 4 boxes per rank at `p=1,2,4,8`.

There are 131,072 global valid cells and 10,485,760 timed valid-cell/RHS
evaluations in every row.

| Ranks | Timed wall (s) | RK step mean +/- SD (s) | RHS eval/s | Seconds/cell/RHS | Speedup | Efficiency |
|---:|---:|---:|---:|---:|---:|---:|
| 1 | 15.735 | 0.78675 +/- 0.01505 | 666,397 | `1.501e-6` | 1.000 | 1.000 |
| 2 | 12.538 | 0.62690 +/- 0.06838 | 836,318 | `1.196e-6` | 1.255 | 0.627 |
| 4 | 4.263 | 0.21315 +/- 0.00567 | 2,459,714 | `4.066e-7` | 3.691 | 0.923 |
| 8 | 2.752 | 0.13760 +/- 0.00627 | 3,810,233 | `2.625e-7` | 5.718 | 0.715 |

The rank-one timed interval exceeds five seconds. The two-rank dip and
four-rank jump are local-host effects consistent with dynamic frequency,
cache, and WSL2 scheduling; there is no order-of-magnitude MPI slowdown.
Across ranks, the mean step-time spread is at most 0.17%, while box counts
are exactly balanced.

## Weak scaling

The weak family fixes `N_x=128`, `N_z=128p`, `dx=dz=1/32`, and 16,384
valid cells per rank. `L_z=4p` preserves compact spacing; the radial domain
is `x in [0.5,4.5]`. `max_grid_size=64` makes `4p` boxes and exactly four
boxes per rank. CFL 0.05 gives `dt=0.0015625` and
`t_f=0.0390625` after 25 steps.

| Ranks | Grid | Timed wall (s) | RK step mean +/- SD (s) | RHS eval/s | Weak efficiency |
|---:|---:|---:|---:|---:|---:|
| 1 | `128 x 128` | 2.798 | 0.13990 +/- 0.01771 | 468,449 | 1.000 |
| 2 | `128 x 256` | 2.288 | 0.11440 +/- 0.00651 | 1,145,734 | 1.223 |
| 4 | `128 x 512` | 2.124 | 0.10620 +/- 0.00194 | 2,468,399 | 1.317 |
| 8 | `128 x 1024` | 2.370 | 0.11850 +/- 0.00312 | 4,424,371 | 1.181 |

The superlinear local values are cache/frequency effects, not a portable
claim. Rank mean-time spread remains at most 0.25%, with identical four-box
ownership per rank.

## Memory

`/usr/bin/time -v` ran inside each MPI rank. Where concurrent stderr text
made one rank's line unparsable, total RSS is conservatively estimated as
`p` times the largest readable per-rank value, as declared in the table.

| Family | Ranks | Peak per rank (MiB) | Estimated total (MiB) |
|---|---:|---:|---:|
| strong | 1 | 126.13 | 126.13 |
| strong | 2 | 76.13 | 152.26 |
| strong | 4 | 51.00 | 204.02 |
| strong | 8 | 38.32 | 306.59 |
| weak | 1 | 37.53 | 37.53 |
| weak | 2 | 37.75 | 75.50 |
| weak | 4 | 37.71 | 150.83 |
| weak | 8 | 37.97 | 303.78 |

Weak-scaling memory is essentially constant per rank. Strong-scaling local
memory falls with local cells while total memory grows because MPI/runtime
and per-rank framework state are replicated. Memory remained bounded with no
swap use.

## Communication and timer availability

The focused acceptance repair inspected both the retained output and the
locked timer source. The executed environment did not set the runtime
`CH_TIMER` variable, so `CH_TIMER_REPORT` correctly emitted no `time.table`
and no source-defined category value was retained. The timer inventory is:

| Category | Existing source timer | Retained M1-E value |
|---|---|---|
| total timed evolution | locked per-step `MPI_Wtime` around `AMR::timeStep` | available: all 20 post-warm-up step times |
| whole AMR step | `AMR::timeStep` | source-defined; no retained `CH_TIMER` value |
| level advance | `AMR::timeStep::advance`, `GRAMRLevel::advance` | source-defined; no retained `CH_TIMER` value |
| RHS | `GRAMRLevel::evalRHS` | source-defined; no retained `CH_TIMER` value |
| RK update | `GRAMRLevel::updateODE` | source-defined; no retained `CH_TIMER` value |
| whole evolution-ghost lifecycle | `GRAMRLevel::fillAllEvolutionGhosts()` | source-defined; no retained `CH_TIMER` value |
| generic Chombo exchange | `exchange+copier`, `actual_exchange`, `exchange`, `exchangeBegin`, `exchangeEnd` | source-defined; no retained `CH_TIMER` value |
| MPI waits | `MPI_Waitall` | source-defined; no retained `CH_TIMER` value |
| plot/checkpoint output | `AMR::writePlotFile`, `AMR::writeCheckpointFile`, `GRAMRLevel::writeCheckpointLevel`, and HDF5 write timers | source-defined; cached per-file spans retained instead |

The following project-level subcategories have no independent timer in the
existing framework:

- physical radial ghost fill alone:
  `UNAVAILABLE_FROM_EXISTING_FRAMEWORK_TIMERS`;
- fused KO addition alone:
  `UNAVAILABLE_FROM_EXISTING_FRAMEWORK_TIMERS`;
- outer valid-surface RHS override alone:
  `UNAVAILABLE_FROM_EXISTING_FRAMEWORK_TIMERS`;
- direct CCZ4, gauge, and fixed-source subdivisions inside the fused RHS:
  `UNAVAILABLE_FROM_EXISTING_FRAMEWORK_TIMERS`.

No production timer, counter, or per-cell instrumentation was added. This
category limitation is nonblocking: total evolution time is the valid locked
MPI wall measurement; five warm-up steps and all output are excluded; valid-
cell/RHS throughput is reported; and source inspection confirms the lean hot
path below.

## I/O cost

I/O used one two-rank, HDF5-enabled, zero-step launch at the strong grid, so
evolution did not contaminate output timing. The first attempt selected no
plot fields and correctly warned that its plot was empty. The single allowed
retry selected all 18 evolved variables and is the retained measurement.
Both files structurally open with HDF5 1.14.6 and report
`Chombo_global/SpaceDim=2`, 18 components, and the expected names. Existing
Chombo/GRChombo formats and the M1-D provenance convention were unchanged.

| Output | Bytes | MiB | Cached create-to-last-write span | Effective cached throughput | SHA-256 |
|---|---:|---:|---:|---:|---|
| plot | 18,885,600 | 18.01 | about 20 ms | about 944 MB/s | `5f77728557f258ee30f76a4a98a45c9e90e1f77159f6e413d981cde30b4b3bd2` |
| checkpoint | 22,590,504 | 21.54 | about 12 ms | about 1,883 MB/s | `06657a35d6c2c29d6d3277e2b6752970410d02a14cb99b3dfe97f848f0d7c1d7` |

The whole MPI path was 0.35 s, peak RSS was 79,604 KiB per rank (155.48 MiB
estimated total), and the combined 39.55 MiB gives a conservative whole-path
throughput of 113.0 MiB/s. Per-file spans are filesystem metadata intervals
on a cached WSL2 filesystem, not durable `fsync` timings; their very high
rates are correspondingly noisy.

## Lifecycle, numerical evidence, and hot path

Every evolution rank completed exactly 25 steps, 100 logged RHS stages,
the declared final time, and `GRChombo finished.` The global point-update
counts equal `N_x N_z x 25`, processor maps contain every box exactly once,
and no NaN, invalid metric, assertion, deadlock, or ABI error appears.

### Focused final-state evidence audit

The strong-scaling timed interval is identical at every authoritative rank
count: steps 5--24, 20 RK4 steps, 80 RHS stages, 131,072 global valid cells,
and 10,485,760 valid-cell/RHS evaluations. Every run completed step 24 and
reached `t_f=0.01953125`. The weak runs completed their declared 25 steps and
reached `t_f=0.0390625`; the retained run analysis recorded no NaN, invalid
metric, assertion, deadlock, or ABI failure. Their different compact domains
make cross-rank weak-state equality inapplicable.

The first focused audit correctly recorded that selective cleanup had removed
the original no-HDF5 strong-run final states. A human-approved budget
amendment clarified the Milestone 1 cap as **maximum 30 launches**, rather
than fewer than 30, and authorized exactly two capture launches without
retry. They reused the exact `256 x 512`, 32-box, CFL 0.05, 25-step workload:
rank 1 followed by rank 8. Both reached stored time
`0.019531250000000007` (declared `t_f=0.01953125`) at iteration 25, remained
finite, and reported `GRChombo finished.` without invalid metric, assertion,
deadlock, NaN, or ABI failure.

The fixed global layout is identical in both files: 32 `64 x 64` boxes cover
131,072 valid cells with ownership multiplicity exactly `[1,1]`. Rank 1 owns
all 32 boxes and 131,072 valid cells; rank 8 owns four boxes and 16,384 valid
cells on each of ranks 0--7. Dataset offsets
and the box sequence agree. The processor metadata intentionally differs,
which makes the whole-file hashes differ, but deterministic reconstruction
in `(z,x,component)` order gives the same valid-state SHA-256 on both runs:
`863095316ffbcf26af513b643c7bb2a1c1ed21131b07406ad068c7828a8c26ea`.
The box-major raw valid dataset is also byte-identical, SHA-256
`583924ebc15708678d02d964df8f63281830fb3c89d77f621005b6720b718c50`.

Weighted `L_2` below means
`sqrt(dx*dz*sum_valid(delta U)^2)`. Exact equality leaves no worst differing
field or cell.

| Field | rank-8 versus rank-1 `L_infinity` | weighted `L_2` |
|---|---:|---:|
| `chi` | 0 | 0 |
| `hxx` | 0 | 0 |
| `hxz` | 0 | 0 |
| `hzz` | 0 | 0 |
| `hww` | 0 | 0 |
| `K` | 0 | 0 |
| `Axx` | 0 | 0 |
| `Axz` | 0 | 0 |
| `Azz` | 0 | 0 |
| `Aww` | 0 | 0 |
| `Theta` | 0 | 0 |
| `GammaX` | 0 | 0 |
| `GammaZ` | 0 | 0 |
| `lapse` | 0 | 0 |
| `shiftX` | 0 | 0 |
| `shiftZ` | 0 | 0 |
| `Bx` | 0 | 0 |
| `Bz` | 0 | 0 |

For both rank counts the maximum conformal-determinant defect and maximum
weighted trace-free defect are each
`4.4408920985006262e-16`. The state difference is therefore exactly zero,
not merely small relative to production discretization drift. Committed M1-B
continues to supply the separate rank-1/2/4 bitwise equivalence evidence;
the new production-sized gate establishes the strongest M1-E decomposition.

Source/build inspection confirms the release hot path still has:

- one `Target::evaluate_direct` target-`d=4` CCZ4 call inside each
  `BlackStringLive::evaluate_rhs`;
- gauge rows and fixed lapse source applied to that same target result;
- one project-owned `BlackStringKODissipation::add` in the same per-cell
  `RHSCompute` BoxLoop, covering all 18 slots and only directions 0 and 1;
- the outer valid-surface override after that one volume BoxLoop;
- no second volume RHS pass or hidden-suppressed/oracle evaluation; and
- no release per-cell allocation, logging, counters, mutexes, transforms, or
  fitting. Diagnostic cadence remains default zero.

M1-E added no source or runtime instrumentation to the production object.

## Launch accounting, limitations, and recommendation

The original M1-E baseline used ten launches: four strong, four weak, the
rejected empty-plot I/O configuration, and its corrected retry. The focused
gate used exactly two more, rank 1 and rank 8, with no retry. Under the
human-approved clarification that the cap is **maximum 30 launches**, the
reconciled Milestone 1 total is exactly 30: M1-A 5, M1-B 4, M1-C 5, M1-D 4,
and M1-E 12. The amended budget is exhausted, not exceeded.

For a scheduler baseline, begin with one MPI rank per physical core,
`OMP_NUM_THREADS=1`, explicit core binding, at least four boxes per rank, and
a problem large enough to keep rank one above five seconds. The local data
favor four to eight ranks for this grid, but a real cluster must remeasure
network, NUMA, filesystem, and frequency behavior before using those values
as budgets.

The required rank counts, throughput, memory, scaling trends, separated
output launch, and production-sized rank-1/rank-8 equivalence gate all pass.
Unavailable timer subcategories remain documented as a nonblocking framework
limitation. The result is
`M1-E PASS — performance and scaling baseline qualified`. The M1-F /
Milestone 1 closure classification is
`CLUSTER_EXECUTION_BASELINE_ACCEPTED`.
