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
accepted radial-boundary physics, time evolution, AMR refinement, MPI,
extraction, diagnostics, horizons, or PETSc/AHFinder support. The E1
parameter file remains check-only, and the older smoke parameter file is
unchanged.

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

The validated cleanup values are determinant `0.9999999999999997` and
weighted trace `-5.204170427930421e-18`.
