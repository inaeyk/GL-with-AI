# Chombo Dependency Contract

Status: **PROJECT_QUALIFIED**.

The project dependency lock is the official `GRChombo/Chombo` commit
`8684f2e000106f1abadb72642e1d15351867f98f`. It is compatible with locked
GRChombo commit `37e659523830418b210acea1661dac0e00bb1b75` under the tested
serial core configuration. This is a project qualification, not a claim that
the same Chombo SHA was used historically.

## Historical recovery result

The bounded official-metadata audit did not recover an exact log-recorded
Chombo SHA.

| Official path | Result |
|---|---|
| Locked GRChombo commit and PR | PR 272 merged at `2026-04-16T13:01:59Z` |
| Clang workflow run `24511708920` | successful; jobs checked out Chombo's default branch without an explicit `ref` |
| Intel Classic run `24511708915` | successful; same unpinned checkout convention |
| GCC run `24511708956` | failed before useful qualification; remaining jobs were cancelled |
| Public run/job log archive API | unavailable: GitHub returned HTTP 403 requiring repository administration |
| Workflow artifacts | none |
| Actions caches for the locked ref | none |
| PR-head workflow runs | none |
| Authenticated `gh api` | unavailable because `gh` and GitHub authentication were absent |

Therefore:

```text
historical_provenance.status = inferred
historical_provenance.exact_log_recorded_chombo_sha = unavailable
```

No source supports `HISTORICAL_EXACT`.

## Candidate selection

The workflows checked out the official repository named
`GRChombo/Chombo`, now canonically exposed as
`GRTLCollaboration/Chombo`. At the successful locked-GRChombo CI timestamp,
the protected default branch `main` pointed to:

```text
8684f2e000106f1abadb72642e1d15351867f98f
2023-01-17T16:46:53Z
Merge pull request #36 from GRChombo/enhancement/aarch64_2
```

There were no later default-branch commits before the 2026 workflow. This
timestamp-derived head was the first candidate. It passed every qualification
gate, so the bounded search stopped and no preceding commit was tested.

| Candidate | Classification | Selection reason | Result |
|---|---|---|---|
| `8684f2e000106f1abadb72642e1d15351867f98f` | timestamp-inferred, not historically exact | official default-branch head at successful CI time | qualified |

The ignored local checkout is `external/Chombo`, with official origin,
detached `HEAD`, and a clean tracked worktree. Neither dependency checkout is
a submodule, and neither dependency source was patched.

## Qualification configuration

The serial pattern comes from locked GRChombo's
`ubuntu-gcc-nompi.Make.defs.local`:

- `DIM=2`, `MPI=FALSE`, `DEBUG=FALSE`, `OPT=TRUE`;
- double precision, 64-bit indices, Chombo namespace;
- OpenMP enabled;
- HDF5, BLAS, and LAPACK enabled;
- no PETSc and no `USE_AHFINDER`.

The qualification machine used Ubuntu 26.04 x86-64, GNU Make 4.4.1,
GNU C++/Fortran 15.2.0, HDF5 1.14.6, and BLAS/LAPACK 3.12.1. Because the
host lacked system `csh`, Fortran, and HDF5 development packages and
passwordless package installation, official Ubuntu packages were extracted
under `/tmp` and presented read-only in a Bubblewrap mount namespace. This
changed no Chombo or GRChombo source.

Required serial DIM=2 libraries:

| Library | Build |
|---|---|
| BaseTools | pass |
| BoxTools | pass |
| AMRTools | pass |
| AMRTimeDependent | pass |

The produced archives use the
`2d_ch.Linux.64.g++.x86_64-linux-gnu-gfortran-15.OPT.OPENMPCC` configuration.
Archive hashes are not used as a portable source lock; source commits,
configuration, and executable probes are the authority.

## Target and stock compatibility matrix

| Check | Dimension/configuration | Result |
|---|---|---|
| Real target header compile and executable | `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4` | pass |
| `parstream.H` resolution | real Chombo include | pass |
| `FArrayBox.H` and Chombo link | real Chombo type/library | pass |
| GRChombo `Cell.hpp` | real locked GRChombo header | pass |
| GRChombo dimensions/tensors/CCZ4 geometry | real locked GRChombo headers | pass |
| Reduced 18-slot Vars storage crossing | project seam plus real Chombo/GRChombo boundary | pass |
| Stock `VariableStoreTest` | stock DIM=3 test | pass |
| Stock `CCZ4GeometryUnitTest` | stock DIM=3 test | pass |

The stock tests remain DIM=3 because their fixtures construct stock 3D boxes
and data. They prove real GRChombo/Chombo compile, link, `Cell`/`FArrayBox`,
tensor, and geometry compatibility. The separate target probe proves the
black-string `2/4/4` header and storage-boundary configuration. Neither test
is represented as a full application or evolution run.

Strict project warnings are enabled. With GNU 15, upstream dependency headers
need only these warning-to-error exceptions:

```text
-Wno-error=unused-parameter
-Wno-error=deprecated-copy
```

The stock DIM=3 tests additionally require
`-Wno-error=unused-variable` for their own test source. The project probe
remains `-Werror` for all other diagnostics.

## Project lock and verifier semantics

The machine-readable authority is
`run_manifests/grchombo_dependency_lock.yaml`.

```text
historical_provenance:
  status: inferred

project_dependency_lock:
  status: qualified
  chombo_commit: 8684f2e000106f1abadb72642e1d15351867f98f
```

`scripts/verify_grchombo_dependency.sh` is read-only. It rejects:

- a wrong GRChombo or Chombo origin;
- a wrong commit;
- an attached dependency branch;
- a dirty dependency checkout;
- a dimension-contract mismatch;
- missing real Chombo headers or Make infrastructure;
- missing required DIM=2 libraries in `--require-build` mode.

It reports the historical, container, and PETSc/AHFinder statuses separately.
Unknown former-container provenance no longer masks the qualified core
dependency; it also is not silently promoted to resolved.

```bash
scripts/verify_grchombo_dependency.sh --metadata-only
scripts/verify_grchombo_dependency.sh --require-probe
scripts/verify_grchombo_dependency.sh --require-build
scripts/verify_grchombo_dependency.sh --require-probe --with-petsc
```

The last command remains unavailable until PETSc is separately pinned and
installed. Candidate mode is retained only for a future recovery audit; it
cannot override the project lock.

## New-machine core setup

Obtain the two locked source trees without modifying them:

```bash
git clone https://github.com/GRTLCollaboration/GRChombo.git external/GRChombo
git -C external/GRChombo checkout --detach \
  37e659523830418b210acea1661dac0e00bb1b75

git clone https://github.com/GRChombo/Chombo.git external/Chombo
git -C external/Chombo checkout --detach \
  8684f2e000106f1abadb72642e1d15351867f98f
```

Install a C++/Fortran toolchain, `csh`, HDF5 development files, BLAS, LAPACK,
and the Perl getopt helper. Copy the locked serial pattern:

```bash
cp \
  external/GRChombo/InstallNotes/MakeDefsLocalExamples/ubuntu-gcc-nompi.Make.defs.local \
  external/Chombo/lib/mk/Make.defs.local
```

Review only machine-specific compiler and HDF5 paths, then build:

```bash
make -C external/Chombo/lib -j4 \
  BaseTools BoxTools AMRTools AMRTimeDependent \
  DIM=2 MPI=FALSE OPT=TRUE DEBUG=FALSE

scripts/verify_grchombo_dependency.sh --require-build
scripts/probe_grchombo_target_headers.sh
```

The verifier must fail if either checkout is dirty or at a different SHA.
The target probe must compile and execute; a Make usage banner is not a pass.

## Remaining provenance boundaries

The core source/build dependency is project-qualified. These remain separate:

- the exact Chombo SHA used by the former successful CI run;
- the historical `grchombo/grchombo` image tag and digest;
- the container recipe and recipe digest;
- PETSc source/version/options/artifact provenance;
- AHFinder/PETSc compile and smoke qualification;
- MPI production configuration and a full black-string evolution build.

Consequently, the project may begin the thin `Cell`/`FArrayBox` GP initializer
wrapper. This does not authorize `BoxLoop` execution, RHS/cartoon physics,
cleanup, lapse source, periodic ownership, evolution, or AHFinder work.
