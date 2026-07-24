# Black-string GP BoxLoop fixture

This fixture executes `BlackStringGPInitialData::Compute` through real
GRChombo `BoxLoops` over a multi-point DIM2 Chombo `Box` and writes a real
18-component `FArrayBox`.

The production `make_compute(...)` API uses a default policy that delegates
to `BlackStringCellStorage::store`. This fixture injects a test-only wrapper
around that same call and records adapter invocations, target `IntVect`s, and
the exact slot ledger in shared mutex-protected state. There is no global
production counter.

The coordinate contract is direction 0 = radial `x` and direction 1 = compact
`z`, with the GRChombo cell-center convention

```text
coordinate[direction] =
    (integer_index[direction] + 1/2) * dx - origin[direction].
```

The origin is the level-wide offset normally supplied by the application
through its `m_p.center`-style coordinate interface, not a box-local lower
bound. Each AMR level supplies its own `dx` with the same physical origin.

The fixture does not wire the live application, fill periodic ghosts, evaluate
an RHS, perform cleanup, evolve data, or compute diagnostics.

Project and fixture sources compile with
`-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`. Locked Chombo and GRChombo
header paths alone are passed with `-isystem`.
