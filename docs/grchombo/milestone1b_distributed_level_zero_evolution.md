# Milestone 1-B distributed level-zero evolution

Date: 2026-07-31

Source checkpoint: `ef3d80e`

Result: `M1-B PASS — distributed level-zero evolution qualified`

This report is the retained evidence for the bounded M1-B comparison.
`research_plan/stage_checklists.md` remains the current-status authority.
No production source, build file, parameter file, boundary formula, gauge,
KO implementation, or external dependency was changed.

## Fixed global problem

The four runs used one exact problem and one global box layout:

| Quantity | Value |
|---|---:|
| `CH_SPACEDIM / GR_SPACEDIM / DEFAULT_TENSOR_DIM` | `2 / 4 / 4` |
| `N_x, N_z` | `8, 32` |
| radial faces | `x=0.5` and `x=1.5` |
| physical lengths `L_x, L_z` | `1, 4` |
| `dx=dz` | `0.125` |
| `max_grid_size`, `block_factor` | `8, 4` |
| boxes | `(0:7,0:7)`, `(0:7,8:15)`, `(0:7,16:23)`, `(0:7,24:31)` |
| periodicity | radial nonperiodic; compact `z` periodic |
| ghost depth | 3 |
| KO | `ko_sigma=0.3` |
| CFL and timestep | `0.05`, `dt=0.00625` |
| RK4 steps and final time | 4, `t_f=0.025` |
| diagnostics cadence | 0 |
| state | exact GP, no perturbation |

Every 8-by-8 box spans all eight radial cells, exceeding the provisional
closure's five-cell source requirement. The only internal box seams are at
compact indices 8, 16, and 24. They are local or MPI periodic-`z` seams,
never physical radial boundaries.

The existing Chombo checkpoint path was selected because it preserves every
cell and all 18 variables without adding a fixture gather or production
instrumentation. ABI-separated optimized serial/HDF5 and MPI/HDF5
executables were built through the existing make controls. Their SHA-256
hashes were:

- serial/HDF5:
  `e229164ca9e0e2e79da558f79dcf82814b5ff9024b3acce53183d38264d485c6`;
- MPI/HDF5:
  `a99f1fb74d86745e5152a69b4539b833591cfa4bae161ca728a3564e1695aae7`.

The serial executable links `libhdf5_serial.so.310` and no MPI library. The
MPI executable links `libhdf5_openmpi.so.310` and `libmpi.so.40`.
From `code/BlackStringToy`, the reproducible isolated build commands are:

```sh
make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=TRUE \
  FC=gfortran-15 CXX=g++ XTRACONFIG=.M1B_SERIAL_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/serial \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/serial -lhdf5 -lz' \
  NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE \
  USE_HDF=TRUE FC=gfortran-15 CXX=g++ XTRACONFIG=.M1B_SERIAL_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/serial \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/serial -lhdf5 -lz'

make realclean DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=TRUE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx XTRACONFIG=.M1B_MPI_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/openmpi \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz' \
  NODEPENDS=TRUE
make -j2 Main_BlackStringToy DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE \
  USE_HDF=TRUE FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M1B_MPI_HDF \
  HDFMPIINCFLAGS=-I/usr/include/hdf5/openmpi \
  HDFMPILIBFLAGS='-L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5 -lz'
```

Because Chombo `realclean` removes application executables across
configurations, the serial executable is restored after the MPI build by
repeating only its second `make` command. Its isolated Chombo libraries
remain ABI-compatible and are not shared with the MPI configuration.

## Lifecycle and ownership

Exactly four evolutions were launched; the optional retry was not used.
Every rank reached `GRAMRLevel::define`, installed the same 18
`UserVariables`, ran `BlackStringToyLevel::initialData`, completed the
initial framework ghost refresh, performed 16 logged `evalRHS` calls and four
RK updates, wrote the final checkpoint at iteration 4 and time 0.025,
concluded, and printed `GRChombo finished.` No assertion, invalid metric,
NaN, deadlock, ABI error, or MPI finalization error occurred.

| Run | Chombo processor map for boxes 0–3 | Valid cells per rank | Global valid cells | Ownership min/max |
|---|---|---|---:|---|
| serial | `0,0,0,0` | rank 0: 256 | 256 | 1 / 1 |
| MPI-1 | `0,0,0,0` | rank 0: 256 | 256 | 1 / 1 |
| MPI-2 | `0,0,1,1` | ranks 0–1: 128 each | 256 | 1 / 1 |
| MPI-4 | `0,1,2,3` | ranks 0–3: 64 each | 256 | 1 / 1 |

The lifecycle gives one initial logical ghost refresh plus one refresh before
each of 16 RK RHS evaluations. Each participating rank therefore executes 17
framework exchanges and 17 low plus 17 high radial-policy invocations.
Because every local box spans the global radial interval, the per-rank
surface work is:

| Run | Local boxes per rank | Low/high radial box fills per rank | Outer-RHS policy calls per rank | Outer surface boxes per rank | Periodic exchange participation |
|---|---:|---:|---:|---:|---:|
| serial | 4 | 68 / 68 | 16 | 64 | 17 |
| MPI-1 | 4 | 68 / 68 | 16 | 64 | 17 |
| MPI-2 | 2 | 34 / 34 | 16 | 32 | 17 |
| MPI-4 | 1 | 17 / 17 | 16 | 16 | 17 |

These are fixture-side lifecycle/accounting results, not counters added to
the production object. Chombo owns each intralevel/periodic exchange.
`fillBdyGhosts` then fills radial strips and radial-periodic corners only.
The provisional outer valid-surface override selects only boxes whose radial
high face equals the global domain high face. No internal `z` seam can satisfy
a radial physical-boundary test.

The live RHS remains one `BoxLoops::loop` per stage. Within its cell compute,
the direct target-`d=4` evaluation is followed by one fused KO addition and
one store. The project KO helper has compile-time directions 0 and 1,
`CH_SPACEDIM==2`, 18-component coverage, and reads only the radial and compact
strides. Thus the 16 logged RHS evaluations imply 4,096 valid-cell/RHS
evaluations globally, with no second RHS or KO pass. The committed D11
direction-safety gates are reused; no direction-2/3 or unrelated fixture was
rerun.

## Full-state comparison

For each final checkpoint, Chombo stored the same four boxes, ghost depth
three, 18 components, `dx`, `dt`, iteration, and final time. The comparison
read `level_0/data:datatype=0`, selected only the valid cells using the
checkpoint boxes and offsets, and serialized them deterministically in
`(z,x,component)` order.

The common initial valid-state SHA-256 is
`9dc19dfe79f04315e29212e11df289a59cbc3142ab0ec1977a9be054557abc8f`.
The common final valid-state SHA-256 is
`b5074985695f8b226d8c9acc1d2ac2a9ef0ae234f0ed51a7fb66e6dad8a3cc49`.
These are exact field hashes, not whole-file hashes: whole checkpoint hashes
differ because processor-ownership metadata differs.

Weighted `L_2` below means
`sqrt(dx*dz*sum_cells(delta U)^2)`. All MPI results are bitwise identical to
serial, so no maximum-difference cell or variable exists.

| Field | MPI-1 `L_inf` / weighted `L_2` | MPI-2 `L_inf` / weighted `L_2` | MPI-4 `L_inf` / weighted `L_2` |
|---|---:|---:|---:|
| `chi` | 0 / 0 | 0 / 0 | 0 / 0 |
| `hxx` | 0 / 0 | 0 / 0 | 0 / 0 |
| `hxz` | 0 / 0 | 0 / 0 | 0 / 0 |
| `hzz` | 0 / 0 | 0 / 0 | 0 / 0 |
| `hww` | 0 / 0 | 0 / 0 | 0 / 0 |
| `K` | 0 / 0 | 0 / 0 | 0 / 0 |
| `Axx` | 0 / 0 | 0 / 0 | 0 / 0 |
| `Axz` | 0 / 0 | 0 / 0 | 0 / 0 |
| `Azz` | 0 / 0 | 0 / 0 | 0 / 0 |
| `Aww` | 0 / 0 | 0 / 0 | 0 / 0 |
| `Theta` | 0 / 0 | 0 / 0 | 0 / 0 |
| `GammaX` | 0 / 0 | 0 / 0 | 0 / 0 |
| `GammaZ` | 0 / 0 | 0 / 0 | 0 / 0 |
| `lapse` | 0 / 0 | 0 / 0 | 0 / 0 |
| `shiftX` | 0 / 0 | 0 / 0 | 0 / 0 |
| `shiftZ` | 0 / 0 | 0 / 0 | 0 / 0 |
| `Bx` | 0 / 0 | 0 / 0 | 0 / 0 |
| `Bz` | 0 / 0 | 0 / 0 | 0 / 0 |

The common GP-background drift gives the scale against which those zero
distribution differences are judged:

| Field | Final-minus-initial `L_inf` | weighted `L_2` |
|---|---:|---:|
| `chi` | 2.624386744587426e-5 | 2.152883825382693e-5 |
| `hxx` | 3.295939872844933e-4 | 2.435728341130529e-4 |
| `hxz` | 2.757361486720251e-18 | 3.197940578934655e-18 |
| `hzz` | 2.629899441231043e-5 | 2.155878737278276e-5 |
| `hww` | 1.516086776834902e-4 | 1.113137625925817e-4 |
| `K` | 9.542149349226925e-3 | 6.898137629652159e-3 |
| `Axx` | 5.176634667301894e-3 | 3.738703974017281e-3 |
| `Axz` | 3.009027441255248e-17 | 2.991265092422051e-17 |
| `Azz` | 2.358095966640272e-3 | 1.704271184689242e-3 |
| `Aww` | 3.211417250404525e-3 | 2.320143202016118e-3 |
| `Theta` | 2.590495899381387e-4 | 1.878469677790129e-4 |
| `GammaX` | 5.343949658981630e-3 | 3.916440025262466e-3 |
| `GammaZ` | 1.017296165686673e-17 | 1.409015106999183e-17 |
| `lapse` | 2.191461193843391e-4 | 1.585011389357621e-4 |
| `shiftX` | 7.646396094740382e-6 | 6.345959322291482e-6 |
| `shiftZ` | 1.079668796213997e-19 | 1.022935751830669e-19 |
| `Bx` | 5.265566492191282e-3 | 3.859113976299454e-3 |
| `Bz` | 1.009190862348848e-17 | 1.397039255201628e-17 |

The largest drift is `K=9.542149349226925e-3` at radial index 0 (the result
is exactly `z`-independent). For every run, the maximum conformal determinant
defect is `3.330669073875470e-16`, the maximum weighted trace-free defect is
`4.440892098500626e-16`, all valid values are finite, and spurious
`z`-dependence is exactly zero.

The retained whole final-checkpoint hashes were:

| Run | Bytes | Whole-file SHA-256 |
|---|---:|---|
| serial | 124,592 | `c61c3c2712687fe02d3a13d37287f9c7b96e6073ab7107adff32996364a72445` |
| MPI-1 | 124,200 | `5bca50142e6ff7986f64d794863f02affd8bf73b7e8147332c4ec15c7f28c5e2` |
| MPI-2 | 124,200 | `5d5f116db76826ccb568d26b58c26e255dfa748fa532de8b6d780a9ed8cae844` |
| MPI-4 | 124,200 | `3006530212e82b96601bb85b3ba603e88a51bd78db4bc6ba735d5a85350c0567` |

## Preliminary performance

Times are deliberately small and noisy. They include process startup,
initialization, four steps, and initial/final HDF5 checkpoint writes; they
are not an evolution-only scaling result.

| Run | Wall time (s) | Peak RSS per process (KiB) | Total recorded RSS (KiB) | Seconds per global valid-cell/RHS |
|---|---:|---|---:|---:|
| serial | 0.02 | `20,300` | 20,300 | 4.8828125e-6 |
| MPI-1 | 0.25 | `38,792` | 38,792 | 6.103515625e-5 |
| MPI-2 | 0.23 | `38,816; 38,764` | 77,580 | 5.615234375e-5 |
| MPI-4 | 0.23 | `38,756; 38,392; 37,968; 38,272` | 153,388 | 5.615234375e-5 |

## Decision

No distributed runtime defect or repair cycle occurred. The real Chombo path
evolves the KO-stabilized GP background through serial, MPI-1, MPI-2, and
MPI-4 with identical boxes, time policy, final time, complete ownership, and
bitwise-identical global valid fields.

`M1-B PASS — distributed level-zero evolution qualified`

M1-C may begin. This result does not replace M1-C's manufactured first,
second, mixed-derivative, KO-seam, radial-corner, or all-component seam
checks, and it is not a performance-scaling claim.
