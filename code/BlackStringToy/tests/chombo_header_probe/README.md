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
On a system where `csh` is installed outside `/bin`, set `CHOMBO_CSHELL` to
that real executable; this overrides only Chombo's command-wrapper path.
Likewise, `CHOMBO_FC` may name the qualified Fortran compiler when it is not
on `PATH`.

Project and fixture sources compile with
`-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`. The locked Chombo and
GRChombo include paths alone use `-isystem`; the project contract contains no
global warning downgrade.

This probe verifies dependency headers and the reduced variable seam. The
focused storage and GP `BoxLoop` fixtures own their respective runtime
validation. This probe does not validate live initial-data dispatch, CCZ4 RHS,
cartoon geometry, cleanup, gauge source, periodic grid, or evolution.
