# BlackStringToy

Isolated Stage 4AO-D-E1 black-string application seam.

The application is compiled with `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and
`DEFAULT_TENSOR_DIM=4`. Direction 0 is radial `x`; direction 1 is compact and
periodic `z`. Its production state contains exactly 18 variables, including
one stored `hww/Aww` representative with hidden multiplicity applied only in
target contractions.

Live hooks:

- `initialData()` runs the validated GP initializer through `BoxLoops` and
  delegates ghost ownership to the framework;
- `specificEvalRHS()` obtains real fourth-order derivatives, expands the
  reduced state to target `d=4`, directly calls locked GRChombo for all 13
  physical rows, evaluates five moving-puncture gauge rows, and applies the
  fixed GP source once to lapse;
- `specificUpdateODE()` applies determinant normalization, weighted
  hidden-aware `A`-trace removal, and the inherited positivity policy after
  the normal update;
- `specificPostTimeStep()` emits exactly `H`, `Mx`, and `Mz` without mutating
  evolution variables.

Build the serial E1 executable with the qualified compiler tuple:

```bash
make -C code/BlackStringToy -j1 all DIM=2 DEBUG=FALSE OPT=TRUE \
  MPI=FALSE USE_HDF=FALSE CXX=g++ \
  FC=/usr/bin/x86_64-linux-gnu-gfortran-15 \
  "CSHELLCMD=/bin/csh -f -c"
```

`params_stage4ao_de1.txt` is an isolated check-only setup. A project-local
pre-include compiles locked `ChomboParameters` grid loops at
`CH_SPACEDIM=2`, then restores target tensor loops at dimension four. The
black-string boundary translation unit now uses a dimension-safe grid-loop
scope, and a real define-only fixture validates
`AMR::define -> GRAMRLevel::define -> BoundaryConditions::define` without
accessing grid directions 2 or 3. The former `RealVect m_center[i=2]`
over-index blocker is cleared. This infrastructure result does not establish
accepted radial-boundary physics. The E2 result below separately establishes
bounded level-zero time advancement; sustained evolution, AMR refinement,
MPI, extraction, expanded diagnostics, horizons, and PETSc/AHFinder support
remain unqualified. The E1 parameter file remains check-only, and the older
smoke parameter file is unchanged.

The application fixture in `tests/chombo_live_application` uses real
`FArrayBox`, `LevelData`, `BoxLoops`, fourth-order derivatives, and
`LevelData::exchange`. It checks:

- all 18 initialized and RHS slots against their pointwise owners;
- four-resolution manufactured convergence for Ricci, encoded Z, advection,
  shift terms, lapse derivatives, combined rows, and `H,Mx,Mz`;
- both global periodic-z wraps and, separately, first-, second-, and
  mixed-derivative production stencils at both sides of the internal
  two-box seam, using scalar/even and one-z fields with fourth-order
  convergence, no translation sign flip, and independent nonperiodic radial
  ghosts;
- four-resolution GP residuals for all 18 rows and `H,Mx,Mz`, including raw
  and fixed-source lapse drift;
- actual pre-output input-registration, RHS-storage, gauge/source, and
  update-hook mutations for ownership, coordinate, cleanup, source, and
  legacy-layout defects;
- worst manufactured row/component, `IntVect`, physical `(x,z)`, and parity
  provenance at every resolution.

The manufactured and GP residual measurements exclude radial-boundary cells.
They qualify the interior application seam, not the current extrapolation
policy as a physical radial boundary.

The Stage 4AO-D-E2 fixture in `tests/chombo_level_zero_evolution` now runs a
bounded, serial, single-level unperturbed GP diagnostic through real
`GRAMR`. The matched `(N_x,N_z)=(32,8),(64,16),(128,32)` sequence keeps
`L_x=8`, `L_z=2`, Courant factor `0.004`, and final time `0.004`.
Consequently `dx=dz=0.25,0.125,0.0625`, `dt=0.001,0.0005,0.00025`, and the
runs take 4, 8, and 16 steps. Compact `z` ghosts remain owned by the one
framework exchange in each logical periodic refresh. A black-string-only
diagnostic policy fills only the low/high radial strips and their
radial-periodic corners from the validated GP initializer/storage seam; it
does not exchange. This exact-background fill is diagnostic-only, not an
accepted physical perturbation boundary.

The default production level contains no lifecycle or ghost-counter storage
or counter updates. E2 enables a compile-time-only instrumentation policy.
It proves framework-exchange then radial-fill then RHS order and reports old
total versus repaired exchanges as `104 -> 24`, `200 -> 44`, and
`392 -> 84`; radial and periodic ghost errors remain exactly zero. The E2
RHS probe no longer performs a redundant prefill. A real
`BLACKSTRING_BOUNDARY_LEGACY_TENSOR_LOOP` build fails during level-zero setup
when direction 2 is attempted.

At the matched resolutions, the maximum 18-variable changes are
`3.059570617455879e-6`, `2.196566544967908e-7`, and
`1.510007325733700e-8`. The corresponding maximum sourced-lapse drifts are
`5.639184674777198e-9`, `4.093922978398723e-10`, and
`2.811939570079858e-11`. `Mx` decreases from
`6.878627068376275e-4` to `3.396766288910058e-6`; `H` decreases from
`6.910655251657349e-7` to `8.041968369010277e-9`. Constraints use
`constraint_diagnostic_cadence=1` in E2; production defaults to `0`, and the
live cadence-zero probe advances without executing the constraint loop.

Focused whole-fixture timings are `0.03 s / 19.7 MB`, `0.20 s / 20.0 MB`,
and `1.78 s / 21.7 MB`, with approximate wall times per valid-cell/RHS of
`6.89e-6`, `5.92e-6`, and `6.69e-6` seconds. The N32 duplicate-exchange
baseline also rounds to `0.03 s / 19.7 MB`; these runs are too small and
noisy for a speedup claim, while the exchange-count reduction is exact.
Sustained evolution, a physical radial boundary, AMR, MPI, perturbations,
broader diagnostics, and horizons remain unqualified.

The validated cleanup values are determinant `0.9999999999999997` and
weighted trace `-5.204170427930421e-18`.
