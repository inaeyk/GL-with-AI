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

The first perturbative radial policy is the project-owned
`BlackStringPerturbativeRadialBoundary`. It uses GRChombo's nonperiodic
boundary layout, periodic exchange, and extrapolating RHS-ghost machinery,
but replaces stock radial formulas where the reduced state requires it.
Writing `delta U=U-U_GP`, the inner three ghost layers use degree-four
one-sided extrapolation of `delta U` from five interior points, then add the
analytic GP value at each ghost. The outer valid surface applies
`partial_t delta U=-c_out(partial_x delta U+delta U/x)` with a fourth-order
backward derivative; its solution ghosts use the same background-subtracted
extrapolation. All 18 slots, including the one-copy `hww/Aww`, are treated
componentwise. `x_in>=r0`, radial periodicity, a nonpositive outgoing speed,
and a ghost depth other than three reject.

The focused `32x8`, four-step serial smoke has `x_in=0.5<r0=1`,
`x_out=4.5`, diagnostics disabled, 17 RHS calls, 76 low/high boundary
refreshes, 17 outer radiative calls, and 20 framework periodic exchanges.
GP, scalar, and one-z cases remain finite; initial GP ghosts agree exactly
with the analytic background, and scalar/one-z initial sectors do not mix.
A manufactured outgoing `f(x-t)/x` pulse leaves the domain with remaining
ratio `1.01e-11`. Its boundary-residual reflection proxy decreases at
roughly fourth order and is smaller at `x_out=6` than at `x_out=4`.

The focused `tests/chombo_fourier_growth` fixture supplies the first bounded
Fourier growth/decay diagnostic without changing the production level. It
adds a compact `C-infinity` radial bump
`u=exp(1-1/(1-s^2))`, `s=(x-2.25)/0.75`, for `|s|<1` and zero otherwise,
with `epsilon=1e-9` and `5e-10`. Even fields use `cos(kz)` while `hxz`,
`Axz`, and
`GammaZ` use `sin(kz)`; the conformal-metric and `A` seeds are trace-free at
linear order. In particular,
`delta GammaZ=epsilon*(0.10 p' + 0.20 p/x + 0.25 k p)*sin(kz)`.
The old `1.00 p'` mutation is rejected by the live initializer audit; no
zero-`Z` interpretation is claimed here. At every cadence sample the
test-only diagnostic computes cosine and sine Fourier coefficients of
`q=0.5 log(hww/chi)` at each radial point, forms their signed radial-RMS
quadratures `Ck` and `Sk` over `1.25<=x<=3.5`, and measures
`ak=hypot(Ck,Sk)`. It also reports `atan2(Sk,Ck)`. Explicit rotations of the
quadrature pair preserve `ak` to relative error below `2.5e-16`, so a phase
rotation cannot masquerade as decay. The former cosine-only result is
superseded.

Both matched-domain sequences keep `Lz=8`, CFL `0.05`, final time `0.4`, and
sample every eight steps. The `x_out=4.5` sequence uses
`(Nx,Nz)=(24,48),(32,64),(48,96)`; the `x_out=6.5` sequence uses
`(36,48),(48,64),(72,96)`. Thus `dx=dz` is
`1/6,1/8,1/12` and `dt` is `1/120,1/160,1/240`. For
`k=pi/4`, the `[0.1,0.4]` fitted rates at either boundary are approximately
`0.3063,0.1636,0.1640`; for `k=pi/2` they are approximately
`-0.08593,-0.2287,-0.2290`. On the finest grid, moving the fit start through
`0.10,0.15,0.20` preserves the signs: the unstable slopes are
`0.1640,0.1464,0.1285`, while the stable slopes are
`-0.2290,-0.2956,-0.3379`. These are bounded sign fits, not asymptotic
eigenmode rates or threshold data.

Every resolution/domain/mode case now has an identical unperturbed control.
At final time its unseeded mode leakage lies between `8.21e-23` and
`4.14e-21`; the seeded-to-leakage ratio is at least `2.75e10`. Final
perturbed-minus-control drift is `8.37e-11` to `1.37e-10`; paired constraint
differences are `1.52e-9` to `2.42e-9` for `H`, `1.84e-10` to `1.26e-9` for
`Mx`, and `1.35e-9` to `2.96e-9` for `Mz`. These paired `Mz` values are
reported without calling them roundoff or convergent. Repeating every seeded
case at `epsilon=5e-10` changes the epsilon-normalized amplitude histories
by at most `6.62e-7`, below the declared `5e-4` tolerance, and preserves all
fitted signs. Every run is finite, scalar/one-z parity remains separated, and
every radial refresh contains one low and one high fill with no
boundary-owned periodic exchange. Diagnostics run every eight steps only in
this fixture; production still defaults to disabled.

Locked upstream `BoundaryConditions.cpp` dynamically allocates three
`std::vector<int>` component lists per boundary-driver invocation. The
boundary `Box` objects themselves are stack objects. This is upstream
per-invocation overhead, not project-introduced and not per-cell; therefore
the end-to-end boundary path is not literally allocation-free.

This remains an inexpensive provisional finite-domain boundary and first
sign diagnostic, not an exact characteristic/WKB condition or a converged
physical GL spectrum. It does not establish `k_cr_0`, long-time stability,
constraint-preserving radial data, nonlinear mode transparency, or accepted
boundary physics. Sustained evolution, AMR, MPI, horizons/AHFinder, broader
production diagnostics, and final scoring remain unqualified.

The validated cleanup values are determinant `0.9999999999999997` and
weighted trace `-5.204170427930421e-18`.
