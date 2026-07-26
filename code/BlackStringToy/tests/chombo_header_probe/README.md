# Black-string Chombo target-header probe

This fixture is a real compile, link, and run gate for the qualified Chombo
dependency. It includes Chombo `parstream.H` and `FArrayBox.H`, locked
GRChombo `Cell.hpp`, dimension/tensor/CCZ4 geometry headers, and the dedicated
black-string reduced Vars seam.

The required macros are:

```text
CH_SPACEDIM=2
GR_SPACEDIM=4
DEFAULT_TENSOR_DIM=4
```

Run it only through:

```bash
scripts/probe_grchombo_target_headers.sh
```

The script verifies both dependency source locks, compiles the `all` target,
and executes `run-only`. Merely invoking the Makefile's default usage target
is not evidence. No header stubs or copied dependency headers are allowed.

The probe accepts Chombo's existing compiler/home interfaces plus the
project's qualified Fortran and shell overrides:

```bash
CXX=/usr/bin/g++ \
CHOMBO_FC=/usr/bin/gfortran-15 \
CHOMBO_CSHELL=/bin/csh \
CHOMBO_HOME="$PWD/external/Chombo/lib" \
scripts/probe_grchombo_target_headers.sh
```

`CXX` remains the compiler variable already used by Chombo; there is no
second `CHOMBO_CXX` alias. The probe resolves the real executable behind the
`gfortran-15` symlink and passes that architecture-qualified path to Chombo,
preserving the configuration name of the qualified DIM2 archives. The helper
`lib/mk/reverse` has a locked `#!/bin/csh -f` interpreter, so setting
`CHOMBO_CSHELL` to another location does not replace the requirement that
`/bin/csh` exist.

Before invoking Make, the probe prints and validates the real compiler, shell,
Chombo-home, reverse-helper, target-header, and exact four DIM2 library paths.
Persistent dependency qualification rejects tools or Chombo homes resolved
under `/tmp` or `/var/tmp`. The normal locked path runs the dependency
verifier in `--require-build` mode.

Project and fixture sources compile with
`-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`. The locked Chombo and
GRChombo include paths alone use `-isystem`; the project contract contains no
global warning downgrade.

This probe verifies dependency headers and the reduced variable seam. The
focused storage and GP `BoxLoop` fixtures own their respective runtime
validation. This probe does not validate live initial-data dispatch, CCZ4 RHS,
cartoon geometry, cleanup, gauge source, periodic grid, or evolution.
