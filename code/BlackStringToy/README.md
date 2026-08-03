# BlackStringToy

Current status authority is
`research_plan/stage_checklists.md`. After D16, completed work is consolidated
into mathematical/dependency foundation, production-core adaptation,
production stabilization, and a physical-mode investigation closed
inconclusive.

The KO-stabilized serial production core is functional. No credible
boundary-independent, constraint-compatible GL mode has been identified.
D16 shows that the instantaneous constraint nullspace is not sufficiently
invariant under the live tangent map, so further fixture-only spectral
projection is stopped.

Active work is deliverable-based:

1. cluster execution baseline;
2. one explicit physical radial/constraint strategy;
3. linear GL acceptance;
4. nonlinear and observable readiness; and
5. scientific release.

G-Engineering passed with committed KO stabilization (`b63ad39`).
G-Physics and overall Checkpoint G remain blocked. No D17 or automatic
D-series continuation is active. The E1–D16 descriptions below are retained
as the historical/diagnostic ledger, not as a current execution sequence.

Production workflow budgets remain one direct target-`d=4` RHS evaluation per
cell/stage, no test instrumentation in the release layout, no per-cell
allocation or logging, configurable diagnostic cadence, and runtime/memory/
scaling measurement at every production milestone. New diagnostics must
enable a named production decision; no new eigensolver or dense spectral
framework is allowed without explicit human approval.

## Milestone 1 — Cluster execution baseline

Milestone 1 has completed M1-A through M1-E.
`M1-E PASS — performance and scaling baseline qualified`, and M1-F /
Milestone 1 is `CLUSTER_EXECUTION_BASELINE_ACCEPTED`. The production-sized
rank-1/rank-8 final states are available and bitwise exact, with common
canonical state hash
`863095316ffbcf26af513b643c7bb2a1c1ed21131b07406ad068c7828a8c26ea`.
Timer-category limitations remain nonblocking and are labeled
`UNAVAILABLE_FROM_EXISTING_FRAMEWORK_TIMERS`. This accepts only the stated
engineering baseline: G-Engineering is passed, while G-Physics and overall
Checkpoint G remain blocked.
Its frozen workload is the committed 18-variable core with
`CH_SPACEDIM=2`, `GR_SPACEDIM=4`,
`DEFAULT_TENSOR_DIM=4`, exact GP, `ko_sigma=0.3`, periodic compact `z`, the
provisional radial boundary, one AMR level, no perturbation/AHFinder, and
diagnostics disabled unless an acceptance test requires them. Production
physics remains frozen unless a demonstrated MPI/HDF5 defect requires repair.

- **M1-A:** reproduce optimized serial/no-HDF5, MPI/no-HDF5, and MPI/HDF5
  builds with toolchain/dependency/flag provenance, executable hashes, startup
  and HDF5 checks, and clean-build commands.
- **M1-B:** compare matched serial, MPI-1, MPI-2, and MPI-4 level-zero
  evolution where available, including all 18 fields, ownership, constraints,
  drift, duplicate-work, and hidden-direction gates.
- **M1-C:** validate distributed seams, periodic rank exchange, derivative
  and KO stencils, radial ownership, radial-periodic corners, and every
  component. Audit M1-A through M1-C only after M1-C.
- **M1-D:** validate existing-format plots/checkpoints and restart equivalence
  without reinitialization.
- **M1-E:** measured RK/RHS throughput, memory, output cost and size, and
  strong/weak scaling; the rank-1/rank-8 final-state gate is bitwise exact,
  and unavailable framework-timer subcategories are nonblocking.
- **M1-F:** `CLUSTER_EXECUTION_BASELINE_ACCEPTED`; acceptance covers MPI
  evolution/ownership/equivalence, HDF5, restart, scaling, and memory.

The milestone allows two repair cycles per stage and 30 numerical launches
before review. It excludes AMR refinement, horizons, Fourier/spectral/physical
mode work, and new MPI/checkpoint abstractions. It uses delta tests and
committed serial/KO evidence. The milestone-closing audit of M1-A through
M1-E is complete. See
`docs/grchombo/milestone1e_performance_scaling.md` for the M1-E evidence.

## Historical implementation and diagnostic ledger

The implementation began as the isolated Stage 4AO-D-E1 black-string
application seam.

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

D13 removes dependence on natural seed settling with a fixture-only,
matrix-free action of the real one-step map. For each of `k=pi/4` and
`k=pi/2`, the action resets valid cells to `U_GP +/- epsilon v`, executes the
unchanged level-zero `GRAMRLevel::advance` lifecycle (periodic exchange,
project radial ghosts, four-stage RK, direct target-`d=4` RHS, live gauge,
fixed lapse source, `ko_sigma=0.3`, outer surface override, and cleanup), and
Fourier-projects the signed difference back into the requested parity
sector. Three deterministic starting vectors cover the corrected D12 seed,
a smooth bulk seed, and an inner-localized seed. The main action uses
`epsilon=1e-8`; every terminated sequence is repeated at `5e-9`.

Some sequences satisfy the local five-iterate overlap/rate test, but this is
not independent-seed eigensystem convergence. At `k=pi/4`, the D12 and smooth
seeds terminate at `(Omega,overlap)=(1.777,0.99921)` and
`(1.962,0.99913)`, yet their cross-profile overlap is only `0.5846` and
their rates differ by `9.41%`. At `k=pi/2`, the corresponding values are
`(1.588,0.99915)` and `(1.920,0.99918)`, but cross overlap is only `0.5572`
and the rates differ by `17.3%`. Both inner-localized sequences reach the
78-iteration cap, turn through positive and negative inferred rates, and
finish concentrated at the first radial cell with about `60-63%` of their
energy in the first/last three-cell bands. The other profiles peak at
`x=1.1875` or `2.3125`, so they are also mutually distinct.

The centered responses agree between epsilon values to below `9e-8` in
normalized profile and `2e-8` in amplification; harmonic leakage is below
`7e-16`, parity leakage is below `6e-9`, and radial Nyquist fractions are
below `0.009`. These establish a clean linear single-Fourier-sector action,
not a physical mode. Raw versus GP-field-scaled norm rate estimates differ
by `1.63-45.1%`, normalized Hamiltonian response reaches `0.808-4.11`, and
the nonzero component amplifications are not common. No pair of independent
seeds passes the required profile/rate gate. D13 is therefore classified
`NONNORMAL_OR_NO_CONVERGED_MODE`; it does not identify either candidate as
the physical GL mode and supplies no `k_cr r0` inference.

D14 resolves the D13 ambiguity with two fixture-only dense reductions of the
same signed one-step map. Locked Chombo already enables LAPACK and links the
system BLAS/LAPACK libraries, so the fixture calls `dgeev` without a new
dependency. Each `k=pi/4,pi/2` block has one parity-compatible Fourier
coefficient for every radial cell and evolved slot: `18*32=576` real
coordinates and a `576x576` nonsymmetric map. The field-scaled map is the
similarity transform `M_s=D^{-1} M D`. D14 transforms LAPACK's right and
left eigenvectors as `v=D v_s` and `w=D^{-1} w_s`; the reported
physical-coordinate eigenvalue-conditioning proxy is
`||w|| ||v|| / |w_s^dagger v_s|`. Diagonalizing the corresponding unscaled
map confirms both transformed vectors for the leading modes, while
degenerate scale-sensitive profiles remain rejected.

All 1,152 basis columns use the validated D13 lifecycle. Twelve representative
columns agree between `epsilon=1e-8` and `5e-9` within `2.32e-7`; independent
dense-versus-matrix-free vectors agree within `7.39e-7`. Fourier round trips
are below `3.3e-16`, forbidden-parity leakage is below `8.3e-8`, and retained
harmonic leakage is below `8.2e-15`. The fixture emits the complete complex
spectrum and separately ranks growth, constraint response, and boundary
localization.

At `k=pi/4`, the largest-growth eigenvalue is real:
`lambda=1.000304369575`, `Re(Omega)=0.0486917`. Its direct matrix-free
residual is `6.21e-8`, but `82.0%` of its energy lies in the first/last three
radial cells, it peaks at the first valid cell, its normalized combined
constraint response is `0.501`, and its corrected physical-coordinate
conditioning proxy is `308.63` (the superseded scaled-coordinate value was
`1522.43`). At `k=pi/2`, the largest-growth value is unresolved-near-neutral
(`Re(Omega)=4.60e-6`) and has combined constraint response `0.748`. The next
leading rates form a cluster with gap `6.73e-6`; their corrected proxies are
`4.56,9.29,9.29,24.23,24.47,22.80` rather than evidence of severe
leader nonorthogonality. Spectrum-wide maxima are `1.34e4` and `1.53e4`,
down from the superseded `1.24e5` and `1.31e5`, but occur outside the
accepted leading physical candidates. A conservative
`|Re(Omega)|>=2e-4` resolution gate follows from the measured direct-action
error divided by `dt`; it prevents numerical-zero map eigenvalues and
unresolved neutral signs from being mislabeled as growth or decay.

No medium candidate passes the complete residual, resolvability, boundary,
Nyquist, parity, multi-field, constraint, and scale-identity filter, so the
optional fine matrix is not constructed. D14 is classified
`CONSTRAINT_GAUGE_OR_BOUNDARY_SPECTRUM_DOMINATES`. D13's local alignments are
not explained by the corrected conditioning proxy alone. The supported
interpretation is limited to a boundary/constraint spectral leader plus an
unresolved clustered `k=pi/2` block, not convergence to an accepted physical
mode. No GL identity or `k_cr r0` inference follows, and the Fourier scan
remains suspended.

D15 isolates the `k=pi/4` reduced spectrum without changing production code.
The committed D14 baseline is reused from hash `0x73c003dd70b673c6` and is
not reconstructed. The repaired frozen-gauge operator is natively
`13*32=416`: its basis, projection, dense storage, and LAPACK solve all omit
lapse, shift, and `B^i`, while `Theta` and both `hat Gamma` fields remain
live. A mutation restoring an embedded `576x576` operator is rejected. The
exact-GP-ghost operator remains `18*32=576`; only radial tangent ghost
perturbations are zeroed, Chombo still owns pure-periodic-z exchange, and the
same provisional outer valid-surface RHS override remains active. Its ghost
policy changes ghost values by `3.50e-7`, while all `8,808` RHS invocations
retain the outer override. The two new matrix hashes are
`0xc435f8701a968876` (frozen) and `0x220e75be6cf311f7` (exact GP).

The D14 leader is not preserved by either isolation. Frozen gauge replaces it
with a real mode having `Re(Omega)=0.19427`, boundary fraction `0.9158`,
combined normalized constraints `0.6795`, and its maximum at the first
radial cell. Exact-GP ghosts replace it with a real mode having
`Re(Omega)=0.97809`, boundary fraction `0.8743`, constraint measure `8.782`,
and its maximum in the inner three-cell band. The smallest-boundary
exact-GP pair reaches `0.1278`, but its constraint measure is `0.721`, still
more than an order of magnitude above the `0.0500752` bulk gate.

Artifact rejection precedes ranking and matching. Each variant derives its
eigenvalue floor as ten times the larger epsilon-halving or dense/direct
discrepancy: `3.09e-6` frozen and `4.10e-6` exact GP. Eigenvalues below that
floor are never passed through `log(lambda)/dt`; all candidates also require
matrix-free residual below `1e-7`, forbidden parity below `1e-5`, and Fourier
round-trip/unwanted-harmonic defect below `1e-7`. The fixture emits a reason
for all 408 rejected candidates. The independent parser reconstructs the
three rankings from retained records and recomputes cross-variant matching
from emitted 18-field profile coefficients, without reading the fixture's
boolean. Neither corrected variant has even one individually bulk-eligible
candidate, so no pair can meet the additional profile-overlap `>0.90`, rate
agreement within `20%`, and radial-maximum separation within two cells.
D15 is therefore classified
`NO_BULK_PHYSICAL_CANDIDATE_AFTER_SECTOR_ISOLATION`. The evidence supports an
unresolved boundary/constraint-dominated spectrum; it does not isolate live
gauge or the provisional closure as the sole source and does not identify a
GL mode.

The two new matrices use 1,882 tangent actions (3,764 signed live steps) in
`139.63 s` fixture time (`144.19 s` including the strict rebuild), with
`354,952 KiB` process peak RSS. Dense storage is `1,384,448` bytes for
`416x416` and `2,654,208` bytes for `576x576`. Twelve epsilon-halving columns
per new matrix agree within `1.58e-7`; dense/direct probes agree within
`4.11e-7`; construction parity and harmonic leakage stay below `5.85e-8`
and `3.13e-15`. The fixture changes no production code:
the hot path remains one direct target-`d=4` RHS, one fused two-grid-direction
KO addition, and no second RHS evaluation.

D16 constructs the `k=pi/4` baseline tangent map once and reproduces the
committed D14/D15 hash `0x73c003dd70b673c6`. Its constraint map uses only
existing evaluators: `H`, `M_x`, `M_z`, `det(h)-1`, and the weighted
trace-free constraint. No validated live helper exists for an encoded
`Z/hat_Gamma` consistency row, so D16 omits that optional row instead of
introducing a new formula.

The all-cell map is `160x576`, has hash `0x467857d44d1b516a`, rank `160`,
and nullity `416`; the map excluding the first and last three radial cells is
`130x576`, has hash `0x0c67ddd102ae9f11`, rank `130`, and nullity `446`.
Their epsilon-halving discrepancies are `1.64e-8` and `9.32e-9`, and
`||CN||/||N||` is `7.48e-14` and `4.86e-14`.

Neither nullspace is invariant under the live map. The leakage measures are
`eta=7.01e-2` (all cells) and `5.68e-2` (interior only), both far above the
declared `1e-3` gate. The apparent projected leaders have
`Re(Omega)=88.65` and `8.02`, but their direct full-map residuals are
`1.49e-1` and `2.02e-2`; even the lowest-constraint representatives have
direct residuals `5.90e-4` and `3.90e-3`. Their cross-projection profile
overlap is only `0.210`. Consequently these compressed eigenvalues are not
interpreted as full-map modes, and D16 is classified
`CONSTRAINT_NULLSPACE_NOT_INVARIANT`.

D16 used 576 tangent-matrix basis actions, 12 epsilon-validation tangent
actions, one random dense/direct tangent probe, and five candidate full-map
residual tangent actions: one each for the real all-cell leader and real
all-cell lowest-constraint candidate, two signed component actions for the
complex interior leader, and one for the real interior lowest-constraint
candidate. Thus `576+12+1+5=594` tangent actions correspond to 1,188 signed
live steps. Constraint accounting remains `576+12=588` actions and 1,176
signed constraint evaluations. The run took `75.61 s` and peaked at
`49,048 KiB` RSS. The fixture and independent retained-evidence parser agree.
Production remains unchanged, the Fourier scan remains suspended, and no
GL-mode or threshold claim follows. A focused read-only audit passed the
construction and classification. It noted that the inherited D15 progress
record labels the layout `exact_gp_ghosts`, although D13 preparation and the
baseline hash prove that D16 used the provisional baseline; this is a
display-label defect only.
The retained cross-projection overlap is for the leaders. The corresponding
lowest-constraint overlap was not retained, and the one-matrix cap forbids a
reconstruction solely to recover it; this cannot affect the result because
both nullspaces fail the invariance gate and all four reported candidates
fail the direct `1e-6` full-map residual gate.

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
