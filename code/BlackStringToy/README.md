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
`0.10,0.15,0.20` preserves those short-window signs: the `k=pi/4` slopes are
`0.1640,0.1464,0.1285`, while the `k=pi/2` slopes are
`-0.2290,-0.2956,-0.3379`. The later common turnover diagnosis supersedes
the former positive/negative physical interpretation: these are transient
fit values, not stable/unstable classifications, asymptotic eigenmode rates,
or threshold data.

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

The fixture-only `run_adaptive_k_scan.sh` extension performs a bounded
fundamental-mode sign screen without changing the production level or radial
policy. It preserves each requested `Lz=2*pi/k`, rounds `Nx,Nz` to legal
block-factor-four grids, and keeps the resulting `dx=dz` within 2.9% of the
requested `1/8`. At `x_out=4.5`, CFL `0.05`, and `t_final=0.8`, the requested
and actual values `k=0.82,0.86,0.90,0.94` have negative fits over
`[0.2,0.8]`, `[0.3,0.8]`, and `[0.4,0.8]`. The allowed lower additions
`k=0.78,0.75` also have negative late-window fits. However, every preserved
amplitude first rises and then decays after a turnover near `t=0.3-0.4`.
Those negative fits therefore do not classify any candidate as physically
stable. Leakage is
`6.82e-22` to `1.06e-20`, seeded/leakage is at least `3.42e9`, and paired
constraint differences remain finite and bounded. The authoritative runner
uses 12 evolutions and reports `AMBIGUOUS`; no positive endpoint exists, so
fine-resolution, far-boundary, and second-amplitude endpoint validation is
not invoked. The data are retained as inconclusive transient evidence. They
do not establish a provisional bracket, classify stability, or support any
conclusion about whether `k_cr*r0` lies above or below `0.75`.

The compact signed-response diagnostic uses the exact shared domain
`Lz=8`, `x_in=0.5`, `x_out=4.5`, `(Nx,Nz)=(32,64)`, `dx=dz=1/8`, and CFL
`0.05`. One control and signed `epsilon=1e-8` pairs at `k=pi/4,pi/2` run to
`t=4`; neither mode has a plateau there. Only the control and `k=pi/4` pair
are extended to `t=8`. Rolling width-`0.5` slopes over the final three
windows are `2.656,2.732,2.880`, and width-`1.0` slopes are
`2.476,2.587,2.737`, with standard errors below `0.103`, variation below
`10.1%`, and high `R2`. The signed half-amplitude pair agrees within
`3.44e-6`, while maximum even/odd contamination is `1.26e-6`. This establishes
linearity and meets the fixture's bounded
`LATE_TIME_LINEAR_INSTABILITY_DETECTED — PHYSICAL IDENTITY UNRESOLVED`
rule for `k=pi/4`;
`k=pi/2` remains `NO_MODE_PLATEAU_WITHIN_TESTED_TIME`. The `t=8` background
control drift reaches `1.97` and the linearized constraint response grows
explosively, so the instability is not identified as a GL mode. This does not
establish a physical growth rate, a negative endpoint, or any `k_cr*r0`
bracket, and the `k` scan remains suspended.

The D7 unperturbed-control matrix reuses that `t=8` control and attempts only
four additional evolutions. Exact-GP radial ghosts terminate early when the
live cleanup rejects an invalid reduced metric, so they supply no boundary
cure. The zero-coefficient gauge-parameter attempt freezes lapse and shift,
but not `B^i`: the locked moving-puncture equation retains the
`Gamma^i`-RHS coupling. It therefore does not isolate a fully frozen gauge.
At half CFL, the `0.1` drift onset remains `t=6` and final drift changes from
`1.96993` to `1.97131`. At `dx=dz=1/12`, early drift is reduced but the
`0.1` crossing moves only to `t=6.2`, final drift rises to `6.39081`, and
the final width-one slope rises to `2.56982`. Completed controls preserve
zero measured `z` span, Fourier leakage below `5.03e-16`, and algebraic
cleanup errors below `8.89e-16`. These mixed and incomplete isolations give
the bounded classification `MULTIPLE_OR_UNRESOLVED`; spatial sensitivity is
the strongest clue, not a unique root-cause identification.

D8 resolves the two incomplete D7 isolation seams without changing
production equations or boundaries. The exact-GP diagnostic finds its first
invalid metric at timestep 397, RK stage 3's predictor (`t=2.475`), in valid
inner cell `(1,0)`: `hxx` changes from `0.2191` before/after the preceding
ghost fill to `-0.03540` after the RK update, taking the determinant from
`1` to `-0.2392`. The audited fill uses Chombo exchange for pure-`z` ghosts,
then fills 384 radial ghosts and 36 radial-periodic corners in all 18
components with zero GP error and no valid-cell overwrite. Thus the abort is
not an exact-GP wiring bug.

The true frozen-gauge fixture specialization zeroes only the lapse, shift,
and `B^i` RHS rows after the unchanged direct target-`d=4` evaluation;
`Theta` and `Gamma^i` stay live. All five frozen slots have exactly zero
measured drift, but the provisional-boundary control crosses drift `0.1` at
`t=2.225`, reaches `3.4089` by `t=8`, and remains inner-boundary dominated.
The optional exact-GP/frozen-gauge combination also becomes inadmissible near
`t=2.36`. D8 therefore classifies the source as
`NEITHER_CONTROL_CURES — CORE_RADIAL_EVOLUTION_UNRESOLVED`. Neither a GL
interpretation nor resumption of the `k` scan follows.

D9 checks the inner-face placement without changing production physics. The
locked local speed envelope contains physical and Z4 speeds `1`, transverse
and longitudinal `d=4` shift-driver speeds `sqrt(0.75)` and
`sqrt(1.125)`, and the fastest lapse speed `sqrt(2)`. With
`v_+=-sqrt(r0/x)+c`, the old `x_in=0.5` face is glancing in the lapse
envelope and its first valid cell has `v_+=0.08088`; this is not strict
outflow. The matched `x_in=0.375` face has margin `0.21878`, and its medium
and fine first valid cells have margins `0.09764` and `0.13498`. This is a
conservative frozen-coefficient check, not a complete characteristic proof.

The deeper placement does not stabilize the GP control. The provisional
medium and fine runs first acquire invalid `hxx` in valid bulk cells at
`t=6.3625` and `t=6.18333`, respectively; their last sampled state drifts are
`60.0` and `94.9`, constraints and radial Nyquist content are uncontrolled,
and the finer run fails earlier. The permitted exact-GP deeper-domain control
fails at `t=0.93125`. All three ghost audits preserve all 18 valid-cell slots,
keep the three inner ghost coordinates positive, and retain zero measured
`z` span. D9 therefore returns `EXCISION_PLACEMENT_NOT_SUFFICIENT`; it does
not show that the glancing old face caused the late-time instability.

D10 stops at the required production-operator trace because it identifies a
concrete dissipation-path defect. The live `BlackStringLive::RHSCompute`
computes fourth-order `x,z` first, second, and mixed derivatives with only
the two `CH_SPACEDIM` strides, expands hidden derivatives algebraically, and
calls locked `CCZ4RHS::rhs_equation` directly. Locked GRChombo adds its
seven-point KO term only in `CCZ4RHS::compute`, after `rhs_equation`; that
wrapper is not on the black-string call path. The adapter also constructs its
otherwise inaccessible base with `sigma=0`, while the local
`SimulationParameters` derives from `ChomboParameters` and never loads or
owns `sigma`. Effective production dissipation is therefore zero with
coverage `0/18`, including the radial direction. This is not a wrong sign or
spacing in an active stencil: the correct upstream negative-semidefinite
stencil is simply never invoked. Calling the generic upstream wrapper is not
a safe shortcut because its bulk derivative/advection/dissipation overloads
use target-wide `FOR` loops to select grid strides, which would request hidden
directions 2 and 3 from a two-dimensional grid.

The rest of the traced path remains dimension-consistent: shift advection is
`beta^x partial_x U + beta^z partial_z U` for every stored slot; the
background-subtracted degree-four radial ghost closure fills three layers
from five valid cells; the outer valid surface overwrites all 18 RHS rows
with the one-sided GP-subtracted outgoing formula; and hidden-aware
determinant/trace cleanup runs after every Chombo RK update. No hidden
direction selects a grid stride. Per the diagnostic stop rule, D10 performs
zero tangent-map configurations and no nonlinear evolutions, and makes no
production repair. Its bounded classification is
`DISSIPATION_PATH_DEFECT_IDENTIFIED`; the tangent spectrum and whether the
observed instability is a bulk or closure-localized mode remain unmeasured.

D11 restores that missing term with the project-owned
`BlackStringKODissipation` helper. It reproduces the locked upstream
seven-point stencil
`[1,-6,15,-20,15,-6,1] sigma/(64 dx)`, but requests only radial stride 0
and compact stride 1 and applies it to all 18 reduced slots. It is fused into
the existing live RHS cell compute after the direct target-`d=4` CCZ4,
gauge, and fixed-source work and before the outer valid-surface override.
There is still one direct target evaluation and one volume `BoxLoop` per
stage. No hidden stride, second RHS evaluation, per-cell allocation,
logging, counter, or runtime-polymorphic dispatch is introduced. The
`sigma=0` early return is bitwise identical to the pre-D11 live RHS and
does not access input strides.

`ko_sigma` is a required, finite, nonnegative project parameter. Every real
evolution parameter file sets it explicitly; historical check fixtures use
zero. The calibration reference is `sigma_ref=0.3`, the conventional
positive value in locked GRChombo
`Examples/KerrBH/params_cheap.txt` and
`Examples/BinaryBH/params_very_cheap.txt` (the locked base default is
`0.1`). The algebraic fixture verifies degree-five annihilation, exact
negative Nyquist damping in both grid directions, mixed-mode additivity,
all-slot coverage, linear scaling, zero-sigma equivalence, and rejection of
omitted-slot and reversed-sign mutations.

On `x=[0.5,4.5]`, `L_z=8`, CFL `0.05`, and `t_f=8`, `sigma=0.3` needs no
doubling: the `dx=1/8` run remains valid with maximum drift `0.06048`,
Hamiltonian maximum `0.3036`, and radial Nyquist fraction `0.1436`; the
`dx=1/12` run improves to maximum drift `0.009710`, Hamiltonian maximum
`0.06863`, and Nyquist fraction `0.1393`. Neither crosses drift `0.1`, both
retain zero measured `z` span, and neither enters the zero-dissipation
baseline's rapid late growth. The fine run is smaller and has the lower
late drift slope (`0.09344` versus `0.12883`). The two new runs took
`26.16 s`/`11.1 MiB` and `74.64 s`/`13.4 MiB`; the medium timing is about
`4.35%` above the reused D7 zero-KO timing midpoint and is too small and
noisy for a precise kernel cost. D11 is therefore classified
`KO_PATH_RESTORED_AND_BACKGROUND_STABILIZED`.

D12 reuses the signed transient fixture at `ko_sigma=0.3` without changing
the production path. Exactly ten runs cover one control and signed
`epsilon=1e-8` pairs for `k=pi/4,pi/2` at `dx=1/8` and `1/12`, all on
`x=[0.5,4.5]`, `L_z=8`, CFL `0.05`, through `t=8`. Samples are separated by
`0.025`. The fixture forms phase-neutral odd/even Fourier responses for
`q=log(hww/chi)/2`, `hww/chi`, `K`, `Aww`, and `GammaX`, plus the three
linearized constraints and radial profiles.

Both resolutions remain interpretable and the controls reproduce D11:
neither has an invalid metric or drift-`0.1` crossing, determinant and
weighted trace stay below `9e-16`, and the fine control reduces the state
and raw Nyquist drifts. Even/odd contamination is below `3e-6`, signal to
control leakage exceeds `3e9` for `q`, and spurious harmonics remain below
`2.4e-6`. Nevertheless, no multi-field eigenmode plateau passes. The
`k=pi/4` `q` response has a late positive interval (representative
one-unit-window slopes `0.280` medium and `0.261` fine), but `K` is negative
(`-0.284,-0.150`), `Aww` is negative or unsettled
(`-0.294,+0.0718`), and `GammaX` grows much more slowly
(`0.0949,0.0323`). The `k=pi/2` response changes slope sign within the last
unit at both resolutions. Because the required physical-field compatibility
fails, there are no accepted final eigenfunction samples or maximum
location to report. D12 is bounded as `NO_CREDIBLE_MODE_PLATEAU`; it does
not identify a growing or decaying physical mode and does not support a
critical-wavenumber inference.

Locked upstream `BoundaryConditions.cpp` dynamically allocates three
`std::vector<int>` component lists per boundary-driver invocation. The
boundary `Box` objects themselves are stack objects. This is upstream
per-invocation overhead, not project-introduced and not per-cell; therefore
the end-to-end boundary path is not literally allocation-free.

This remains an inexpensive provisional finite-domain boundary and transient
diagnostic, not an exact characteristic/WKB condition or a converged
physical GL spectrum. It does not establish `k_cr_0`, long-time stability,
constraint-preserving radial data, nonlinear mode transparency, or accepted
boundary physics. Sustained evolution, AMR, MPI, horizons/AHFinder, broader
production diagnostics, and final scoring remain unqualified.

The validated cleanup values are determinant `0.9999999999999997` and
weighted trace `-5.204170427930421e-18`.
