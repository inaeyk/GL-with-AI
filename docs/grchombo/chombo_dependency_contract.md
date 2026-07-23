# Chombo Dependency Contract and Recovery Plan

Status: GRChombo source, Chombo fork identity, and Chombo layout are verified.
The Chombo revision, patch provenance, production compiler tuple, and
container provenance are unresolved. Therefore neither the target header
probe nor a fully reproducible build is currently available.

## Evidence classification

| Fact | Classification | Evidence |
|---|---|---|
| GRChombo remote is `https://github.com/GRTLCollaboration/GRChombo.git` | verified | clean detached local checkout and tracked source lock |
| GRChombo commit is `37e659523830418b210acea1661dac0e00bb1b75` | verified | local `HEAD` and verifier |
| GRChombo CI obtains Chombo from repository `GRChombo/Chombo` | verified | locked `gcc-tests.yml`, `clang-tests.yml`, and `intel-classic-tests.yml` |
| Canonical Chombo remote is `https://github.com/GRChombo/Chombo.git` | verified as the CI repository identity | the three locked checkout actions |
| CI Chombo checkout is pinned to a revision | false | all three checkout actions omit `ref` |
| Exact Chombo branch, commit, tag, archive digest, or patch set | unresolved | absent from locked source, manifests, and local metadata |
| Chombo checkout layout is `<root>/lib`, exported as `CHOMBO_HOME` | verified | all three locked CI workflows |
| GRChombo applications include `${CHOMBO_HOME}/mk/Make.test` | verified | application and test GNUmakefiles |
| CI installs `Make.defs.local` at `${CHOMBO_HOME}/mk/Make.defs.local` | verified | locked CI build steps |
| CI builds `AMRTimeDependent AMRTools BaseTools BoxTools` | verified | locked CI build steps |
| The minimal storage-header probe needs `BaseTools` and `BoxTools` | inferred from included Chombo/GRChombo types; must be compile-tested | `parstream.H`, `FArrayBox.H`, `Cell.hpp`, and their headers |
| `parstream.H` is supplied by Chombo, not GRChombo | verified | direct GRChombo includes and absence from the locked GRChombo tree |
| No Chombo submodule is embedded in GRChombo | verified | no `.gitmodules` and empty submodule inventory |
| Core CCZ4 / pointwise GP initialization requires PETSc | false | core headers and non-AH CI configurations |
| `USE_AHFINDER` requires PETSc headers/libraries | verified | `PETScAHSolver`, `PETScCommunicator`, and MPI Make-def flags |
| Ubuntu MPI CI discovers PETSc as `pkg-config petsc` | verified | `ubuntu-gcc.Make.defs.local` and `ubuntu-clang.Make.defs.local` |
| Exact PETSc version/revision/build | unresolved | CI installs unversioned Ubuntu 22.04 `petsc-dev`; no lock is recorded |
| Historical image name is `grchombo/grchombo` | verified as repository history | build notes and prior smoke commands |
| Historical image tag and digest | unresolved | not recorded; current Docker integration is unavailable |
| Container recipe and recipe digest | unresolved | no authoritative tracked recipe was found |
| A `docker_hpc-base-stretch.Make.defs.local` example exists | verified | locked GRChombo `InstallNotes` |
| That Make-def example identifies the historical production image | not established | it names a generic `hpc-base-stretch` base but supplies no recipe or digest |

The ignored `external/GRChombo` checkout must not be treated as carrying its
own Chombo dependency. No `external/Chombo` checkout, `CHOMBO_HOME`,
`PETSC_DIR`, or `PETSC_ARCH` is available in the current shell, and
`pkg-config` cannot resolve PETSc. The Docker command is present only as an
unavailable WSL integration shim, so it cannot recover an image ID or digest.

## Build contract recovered from the locked source

The locked GNU and Clang workflows run on Ubuntu 22.04, test gridded dimensions
two and three, and exercise MPI/non-MPI and optimized/debug variants. Their
compiler matrices are GNU 9--12 and Clang 13--15. The source README describes
GRChombo as C++14; the black-string reduced seam is tested as C++17, so its
target Make configuration must explicitly preserve C++17.

The MPI GNU configuration records:

- `PRECISION=DOUBLE`, `USE_64=TRUE`, `NAMESPACE=TRUE`;
- GNU C++ and Fortran, `mpicxx`, MPI, and OpenMP;
- serial and MPI HDF5 include/library paths;
- BLAS and LAPACK;
- `-DUSE_AHFINDER`, PETSc include flags, and PETSc link flags through
  `pkg-config petsc`.

The non-MPI GNU configuration omits AHFinder/PETSc and MPI while retaining
HDF5, BLAS, LAPACK, OpenMP, double precision, 64-bit indices, and the Chombo
namespace. The exact production choices for MPI, OpenMP, HDF5, compiler
version, and optimization flags remain decisions to lock after Chombo
recovery.

The project-local convention is:

```text
Chombo checkout: external/Chombo
CHOMBO_HOME:      external/Chombo/lib
Make definitions: external/Chombo/lib/mk/Make.defs.local
GRChombo checkout: external/GRChombo
```

This mirrors the CI relative layout without converting either ignored
checkout to a submodule.

## PETSc and AHFinder boundary

PETSc is not required for the next `Cell`/`FArrayBox` GP-initializer wrapper.
It becomes required when `USE_AHFINDER` is enabled for MOTS/horizon work.
The CI's `petsc-dev` package and `pkg-config petsc` convention are useful
build-shape evidence, but not a reproducibility lock. PETSc remote, version,
configure options, MPI ABI, and artifact digest remain unresolved.

## Verifier and compile probe

The tracked manifest is
`run_manifests/grchombo_dependency_lock.yaml`.
`scripts/verify_grchombo_dependency.sh` always verifies the locked GRChombo
origin, commit, detached state, cleanliness, and the `2/4/4` target macro
contract. If a Chombo path is present it also verifies:

- the CI-authoritative Chombo remote;
- cleanliness;
- exact revision when the manifest is pinned, or an explicitly named
  full-commit candidate during recovery;
- `${CHOMBO_HOME}/mk/Make.test`;
- a real `parstream.H`;
- compiler availability;
- PETSc through `pkg-config petsc` when requested.

Metadata-only mode reports missing dependencies without claiming a usable
build. Probe-required mode rejects an unpinned revision unless the caller
supplies the exact candidate commit. Full-build mode rejects candidate-only
evidence and all unresolved required provenance.

The test-only probe in
`code/BlackStringToy/tests/chombo_header_probe/` includes real
`parstream.H`, `FArrayBox.H`, and GRChombo `Cell.hpp`, and crosses the storage
boundary with `BlackStringReducedVars`. It compiles with
`CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4`, and C++17 through
Chombo's `Make.test`, with `-O2 -Wall -Wextra -Wpedantic -Werror`. It does not
instantiate a `BoxLoop`, initial-data physics, RHS, cleanup, source, grid, or
evolution path.

Current result:

```text
TARGET_HEADER_PROBE=BLOCKED
Reason: no Chombo checkout is available at external/Chombo, and no
authoritative Chombo revision is pinned.
```

No locally invented Chombo headers or copied stencil/formula substitutes are
accepted.

## Bounded recovery plan

Because the locked GRChombo source does not identify a Chombo revision, no
clone-and-checkout command can yet honestly name an accepted commit. Recovery
is bounded to:

1. Obtain the authoritative Chombo full commit, any patch set, compiler,
   `Make.defs.local`, PETSc tuple where AHFinder was enabled, container image
   digest, and recipe digest from the former working environment,
   collaborators, or GRChombo maintainers.
2. For each named candidate only, obtain `GRChombo/Chombo` into
   `external/Chombo`, detach at the supplied full commit, and keep it clean.

   ```bash
   git clone https://github.com/GRChombo/Chombo.git external/Chombo
   git -C external/Chombo checkout --detach \
     <AUTHORITATIVE_FULL_40_HEX_CHOMBO_COMMIT>
   git -C external/Chombo status --short
   ```

3. Run:

   ```bash
   scripts/verify_grchombo_dependency.sh \
     --metadata-only \
     --chombo-root external/Chombo

   scripts/probe_grchombo_target_headers.sh \
     --chombo-root external/Chombo \
     --candidate-revision <FULL_40_HEX_CHOMBO_COMMIT>
   ```

4. For the initializer-only non-MPI candidate, begin with the locked
   GRChombo serial template and the two probe libraries:

   ```bash
   cp \
     external/GRChombo/InstallNotes/MakeDefsLocalExamples/ubuntu-gcc-nompi.Make.defs.local \
     external/Chombo/lib/mk/Make.defs.local
   make -C external/Chombo/lib -j4 BaseTools BoxTools \
     DIM=2 MPI=FALSE OPT=TRUE DEBUG=FALSE
   ```

   A production candidate additionally builds the CI-established
   `AMRTools` and `AMRTimeDependent` targets with the reviewed
   MPI/OpenMP/HDF5 settings. Run the target header probe, a strict GRChombo
   compile, and the existing cheap smoke test in the recovered environment.
5. Pin the Chombo revision, patch set, compiler/configuration, and artifacts
   in the manifest only after the target header probe, strict compile, and
   smoke test pass. Then run `--require-probe`.
6. Recover and pin the container recipe/image digest independently. Only
   after all required fields are resolved may `--require-build` report a
   reproducible build.

Do not choose the current default branch, newest tag, or latest container as a
substitute for the missing authoritative tuple.

## Next production action

The exact blocker before the `Cell`/`FArrayBox` GP wrapper is an authoritative
full Chombo commit (plus any patch set) and a real checkout that passes the
target header probe. PETSc may remain deferred for this initializer-only
substage. After the probe passes, the next implementation is limited to a
thin `Cell` load/store wrapper and pointwise GP compute class around the
already validated reduced Vars/initializer; `BoxLoop` execution, RHS,
hidden/cartoon geometry, cleanup, lapse source, periodic ownership, and
evolution remain later gates.
