# GRChombo Adaptation Backlog for GL Production

Status: prioritized design backlog with comparison-batch 1-4 evidence.
Production must reuse mature GRChombo infrastructure and adapt only
black-string-specific physics, configuration, and diagnostics. Do not
independently rebuild RK4, AMR, MPI/OpenMP, checkpoint/restart, ghost exchange,
reductions, interpolation, or generic parameter parsing.

## Priority rules

- **P0** establishes a reproducible authority and prevents convention drift.
- **P1** creates the minimum stationary black-string production path.
- **P2** adds controlled perturbations and quantitative linear diagnostics.
- **P3** adds horizon observables and the nonlinear GL workflow.

Every item requires the applicable comparison-plan levels before production
acceptance. The frozen custom outer-boundary research is excluded.

## Post-qualification production lock

The historical Chombo SHA used by GRChombo is unavailable; its provenance is
inferred and must not be labeled `HISTORICAL_EXACT`. The production authority
is instead the project-qualified official tuple:

```text
GRChombo 37e659523830418b210acea1661dac0e00bb1b75
Chombo    8684f2e000106f1abadb72642e1d15351867f98f
```

Both checkouts are detached and clean. The four core serial DIM2 Chombo
libraries, the real `2/4/4` target probe, `VariableStoreTest`, and
`CCZ4GeometryUnitTest` pass. The core dependency build is verified. Former
container provenance and PETSc/AHFinder reproducibility remain unresolved;
MPI and the full black-string runtime are not yet qualified. Those gaps do not
block the next storage integration, while PETSc/AHFinder work remains blocked.

The custom solver is retained as a validated reference oracle and
agent-capability benchmark. Its complete 13-row frozen-gauge interior,
nonlinear JVP, parity/block gates, inner endpoint, stencil/layout scaffolding,
stationary matrix extraction, and comparison batches 1-4 are completed
reference work. The accepted outer WKB boundary, rank-four decaying basis,
rank-nine annihilator, boundary-bearing operator, eigensolver/shift-invert,
custom `k_cr_0`, and spectral MOTS map are deferred research. The original
spectral objective did not pass; it is simply nonblocking for production.

The locked production order is:

1. [complete] thin `Cell`/`FArrayBox` storage seam around the reduced 18-slot
   Vars and GP point initializer;
2. [compute/traversal complete] GP `BoxLoop` initializer, with live
   application wiring deferred;
3. [next] hidden/cartoon RHS adaptation, preserving GRChombo ownership of shared
   visible CCZ4 families;
4. complete 13-row pointwise equivalence with stock-visible, adapted-hidden,
   and total contributions reported separately;
5. hidden-aware cleanup and constraints;
6. production fixed lapse-source hook;
7. periodic `z` and ghost ownership;
8. unperturbed GP evolution;
9. perturbed Fourier-mode evolution and the first growth/threshold estimate;
10. horizon and nonlinear diagnostics after PETSc/AHFinder and observable
    conventions are qualified.

Substantive independent audits occur only after the assembled storage plus
`BoxLoop` initializer, the passing complete adapted 13-row RHS, integrated
cleanup/constraints plus fixed source, the first passing unperturbed
evolution, and the first passing perturbed growth-rate run. Documentation
consistency is folded into those audits; no per-substep audit is added.

| Priority / order | Adaptation item | GRChombo source to reuse | Project-specific work | Dependency | Acceptance / exit criterion |
|---|---|---|---|---|---|
| P0-1 | Reproducible GRChombo/Chombo core lock | Current origin, locked CI, Chombo Make infrastructure | Keep the tracked GRChombo commit and qualified official Chombo commit; disclose that historical SHA/container provenance is unresolved; keep PETSc separate until AHFinder | None | Project lock is detached/clean; four serial DIM2 libraries, real `2/4/4` target probe, and stock compile/smoke checks pass |
| P0-2 | Convention and slot adapter | `CCZ4Vars.hpp`, `ADMConformalVars.hpp`, `UserVariables.inc.hpp`, Tensor/VarsTools | Explicit `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4`; reviewed 18-slot black-string map with no visible-`y` slots and one multiplicity-two hidden representative | P0-1 source verification | Level 1 macro/slot/name/parity/permutation tests pass exactly |
| P0-3 | Formula comparison harness | `CCZ4RHS::rhs_equation`, `CCZ4Geometry`, gauge classes | Test-only adapter accepting supplied analytic jets and emitting per-family rows | P0-1, P0-2 | First five comparison tests execute without production evolution |
| P1-4a | Cell/FArrayBox storage seam | Chombo storage plus locked GRChombo `Cell` | Thin load/store wrapper around the validated 18-slot reduced Vars and GP point initializer; no physics duplication | P0-2 | Exact round trip through real storage; no `BoxLoop` or physics path |
| P1-4b | GP BoxLoop initializer | GRChombo initial-data `BoxLoops` pattern and project parameter parser | Thin compute class and isolated real DIM2 traversal; live application wiring deferred | P1-4a | Every requested point equals the existing initializer; coordinate, traversal, determinant, trace, and mutation checks pass |
| P1-5 | Modified-cartoon hidden-sphere production path | GRChombo CCZ4 orchestration, derivatives, BoxLoops | Adapt only missing hidden Ricci, lapse Hessian, Gamma, algebraic, shift, and Z terms; remove smoke freeze | P0-3, P1-4b | All Level 1/2 hidden-family comparisons and ownership mutations pass |
| P1-6 | Complete pointwise 13-row equivalence | Stock visible RHS plus hidden adapter and custom oracle | Report stock-visible, adapted-hidden, and total rows separately | P1-5 | Every family and all 13 totals pass the fixed tolerance and mutations |
| P1-7 | Hidden-aware algebraic cleanup and constraints | `TraceARemoval`, `PositiveChiAndAlpha`, `Constraints`, `AMRReductions` | Extend determinant/A-trace cleanup and Hamiltonian/momentum constraints with multiplicity two | P1-6 | Weighted residuals and pointwise constraints match oracle; convergence passes |
| P1-8 | Fixed GP-holding lapse source | `MovingPunctureGauge` adapter pattern | Add field-independent `S_alpha=3 sqrt(r0/x^3)` with explicit validation/production policy | P1-7 | Raw lapse residual remains visible; source-adjusted GP residual vanishes; JVP unchanged |
| P1-9 | Compact periodic `z` production domain | GRChombo periodic boundary/domain parameters and derivative classes | Lock `L`, `k_n=2 pi n/L`, parity conventions, radial/compact direction mapping | P1-4b | Periodic wrap, ghost ownership, and Fourier derivative tests pass at production order |
| P1-10 | Unperturbed background evolution | `GRAMR`, RK4, ghost fill, boundaries, checkpointing | Configure target grid, source, hidden RHS, diagnostics, and conservative validation window | P1-7 through P1-9 | L4-01 stationarity, constraint convergence, gauge-source validation, and restart smoke pass |
| P2-11 | Fourier perturbation initialization | Initial-data BoxLoop plus periodic grid | Add normalized even/odd SO(3)-scalar perturbation families with amplitude guard | P1-10 | Linear amplitude scaling and mode/parity leakage tests pass |
| P2-12 | Fourier amplitude diagnostics | `AMRInterpolator`, reductions, `SmallDataIO` | Adapt custom cosine/sine projections to AMR-consistent sampled/integrated output | P2-11 | Synthetic and production single-mode recovery tests pass |
| P2-13 | Growth-rate extraction | `SmallDataIOReader`/project analysis output contract | Implement fit-window, uncertainty, resolution, and observable-consistency reporting | P2-12 | Synthetic exponent tests and window mutations pass before physical rates |
| P2-14 | Production convergence workflow | GRChombo AMR/restart/output machinery | Reproducible multi-resolution parameter sets and comparison tables | P1-10, P2-13 | Background, constraints, perturbations, and rate show documented convergence |
| P3-15 | String MOTS/horizon adapter | `AHFinder`, `PETScAHSolver`, `AHStringGeometry`, interpolation | Supply target variables, `S2 x S1` geometry, hidden expansion terms, PETSc configuration | P1-7, PETSc source lock | Uniform `x=r0` MOTS recovered with convergent residual; restart supported |
| P3-16 | `R_H`, minimum radius, and horizon area | AH surface data, interpolation, reductions, `SmallDataIO` | Evaluate `R_H=h sqrt(hww/chi)`, minimum over z, correct string area | P3-15 | Uniform analytic values and perturbed manufactured profiles converge |
| P3-17 | Nonlinear GL production workflow | All mature GRChombo runtime infrastructure | Parameter families, perturbation sweep, constraints/horizon monitoring, failure criteria, archival metadata | P2-14, P3-16 | Reviewed end-to-end runbook; unperturbed and small-perturbation gates pass first |
| Deferred | Custom stationary outer boundary | None accepted | Preserve documentation/scaffolding only | Not on production path | Reopen only through a separate research decision |

## Batch-1 backlog update

- P0-1 is partially evidenced, not complete. The exact custom and GRChombo
  commits, detached-clean GRChombo status, compiler, comparison flags, and
  dimensions are recorded. The ignored checkout is not a reproducible
  top-level source lock, and the Chombo revision/container digests remain
  unresolved.
- P0-2 has an exact shared-slot map and an explicit stock-`d=3` versus
  custom-`d=4/2` distinction. It is not complete because no reviewed
  `CH_SPACEDIM=2`, `GR_SPACEDIM=4` production adapter exists.
- P0-3 now has a focused header-only direct bridge for GRChombo tensor algebra,
  contracted connection, and physical Ricci. It is deliberately test-only.
  A full analytic-jet CCZ4 RHS bridge remains future work.
- No P1, P2, or P3 production item was started. Hidden/cartoon terms remain
  custom-only pending P1-6, and the custom stationary outer boundary remains
  deferred.

The direct header bridge proves that missing Chombo/container digests need not
block source-level and directly compilable geometry comparisons. It does not
reduce the requirement to resolve those digests before production adaptation.

## Batch-2 backlog update

- P0-3 now includes a direct test-only call to the inspected
  `CCZ4RHS::rhs_equation`, in addition to the batch-1 geometry bridge. At
  matched stock `d=3`, every exercised visible `chi`, `h`, `K`, `Theta`, and
  `A` family and every combined row passes the fixed Level-2 tolerance.
- The raw-Ricci and encoded-Z split is directly evidenced. Omission, duplicate
  insertion, and wrong trace-free dimension mutations are detected against
  the directly compiled path. Wrong index conversion and missing conformal
  factor mutations instead validate the local source/convention
  reconstruction made from a directly computed contracted connection. No
  visible Ricci-Z correction is required before production adaptation.
- P0-2 remains incomplete: same-dimension equivalence is not a target-`d=4`
  production adapter. Dimension coefficients must remain explicit.
- P1-6 remains the primary physics adaptation gap. Stock source lacks
  `hww/Aww`, hidden contracted-connection/Z, hidden Ricci/lapse-Hessian,
  multiplicity-two traces, and hidden-aware determinant/trace cleanup.
- The next comparison work should be Level 3 discrete manufactured profiles
  through GRChombo derivative paths. It must reuse the mature derivative and
  periodic-grid infrastructure rather than rebuilding it.
- No P1, P2, or P3 production item was implemented by batch 2. The custom
  stationary outer boundary remains deferred and is not added to the
  production backlog.

## Batch-3 backlog update

- P0-3 now directly exercises the selected
  `FourthOrderDerivatives::{diff1,diff2,mixed_diff2,advection_term}` kernels.
  Their manufactured errors converge at order approximately four; the custom
  oracle scaffolding converges at order approximately two. Every visible
  manufactured `chi,h,K,Theta,A` row converges to the common batch-2 continuum
  result, and the independent continuum extrapolations agree to
  `4.920e-13`.
- The repaired family gate separately covers all 15 visible advection rows
  and directly isolates the complete GRChombo `chi`, metric, and `A` shift
  RHS families by paired `rhs_equation` calls. Raw shift-derivative
  convergence remains only an input-kernel diagnostic. This strengthens the
  comparison evidence but does not create production functionality.
- P1-8 remains framework-owned. The direct derivative kernels compile without
  Chombo, but the actual Chombo periodic-domain and ghost-exchange path is
  blocked by the missing local Chombo installation and unresolved
  Chombo/container digest. Do not replace it with a project-owned periodic
  production grid.
- The stock visible `TraceARemoval` and `PositiveChiAndAlpha` compute classes
  are directly callable and pass component, trace, idempotence, clamp, and
  ordering checks. Reuse them for stock visible fields.
- P1-7 remains open. No runtime conformal-metric determinant-normalization
  compute class was found in the inspected stock path, and stock trace removal
  has no `hww/Aww` or multiplicity-two owner. Hidden-aware determinant and
  trace cleanup must be adapted and compared with the custom oracle.
- The custom centered order-two derivatives remain focused regressions only.
  They are not candidates for production replacement of GRChombo's derivative,
  Chombo ghost, periodic-domain, or AMR infrastructure.
- No P1, P2, or P3 production item was implemented. The custom stationary
  outer boundary remains deferred.

## Batch-4 backlog update

- P1-4 now has test-only analytic and discrete evidence, but no production
  implementation. The coordinate angular coefficient
  `gamma_theta_theta=x^2` must enter the normalized modified-cartoon state as
  `hww=gamma_theta_theta/x^2=1`; stock visible `y` is not its replacement.
- P1-5 now has a directly compiled raw gauge reference:
  `MovingPunctureGauge` gives `partial_t alpha=-3 lambda`. The repaired
  test-only adapter changes lapse only and has zero numerical Fréchet
  derivative with respect to all 20 evolved-field coordinates. Wrong
  sign/factor, `2K`, evolving-horizon, shift-owner, and B-owner adapters fail.
  A production adapter is still absent.
- P1-6 and P1-7 remain required before stationarity can be inferred from
  GRChombo. Independent stock/target family builders now show that the
  stock-visible `d=3` K/A totals are replaced—not merely negated—by target
  K/A values, hidden shift divergence, the `2/d=1/2` trace coefficient, and
  multiplicity-two traces. This is an adaptation seam, not a stock defect.
- P1-8 remains blocked at production ownership. Direct fourth-order kernels
  converge, but Chombo `BoxLoop`, periodic ghost exchange, and the unresolved
  Chombo/container tuple have not been exercised.
- P2-10 has a locked test-only mode/parity convention and zero frozen-gauge
  perturbations. The physical radial perturbation profile remains
  deliberately unlocked, and no initializer was wired.
- Before P1-9, complete P0-1/P0-2, then P1-4 through P1-8: target-dimension
  slots, GP `BoxLoop` data, lapse-only source, hidden RHS, hidden-aware
  cleanup/constraints, periodic z ownership, and raw/held background
  diagnostics.
- No production P1/P2/P3 implementation was added. The custom stationary
  outer boundary remains deferred.

## Production-adaptation preflight update

- The authoritative inspected GRChombo source is now machine-readably locked
  in `run_manifests/grchombo_dependency_lock.yaml`; the read-only
  `scripts/verify_grchombo_dependency.sh` rejects the wrong remote, commit,
  branch state, or dirty checkout. This completes the source-lock portion of
  P0-1 only. The Chombo fork/layout is known, but its revision plus PETSc,
  Docker-image, and container-recipe digests remain unresolved, so
  fresh-build reproducibility is still open.
- The target P0-2 design is an 18-slot black-string layout for
  `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and `DEFAULT_TENSOR_DIM=4`. It stores no
  visible-`y` variables. `hww/Aww` are single representatives with
  multiplicity two in physical traces and contractions. The current 27-slot
  smoke/comparison enum is not the production contract.
- `docs/grchombo/grchombo_production_adaptation_preflight.md` locks field
  ownership, the minimal GRChombo wrap/extend boundaries, and the future
  pointwise 13-row oracle seam. No enum, initializer, RHS, cleanup,
  constraint, gauge-source, grid, or evolution implementation was added.
- The implementation order is now fixed: dependency verification; target
  enumeration/registration; GP initializer; hidden/cartoon geometry;
  hidden-aware cleanup/constraints; fixed lapse source; pointwise 13-row
  comparison; periodic-`z` ownership; unperturbed evolution. Audit
  checkpoints occur after the slot lock, initializer, hidden geometry,
  cleanup/constraints, complete pointwise comparison, periodic ownership, and
  unperturbed run. This design-only preflight does not receive a separate
  audit.
- The first implementation substage is the isolated 18-slot enumeration,
  names, compile-time dimension assertions, and permutation/parity fixture,
  after running the dependency verifier. It must not add GP values or physics
  RHS code.

## Chombo dependency audit update

- P0-1 now verifies more than the GRChombo checkout: the locked CI establishes
  the `GRChombo/Chombo` fork, `CHOMBO_HOME=<checkout>/lib`,
  `${CHOMBO_HOME}/mk/Make.test`, the Make-def installation point, and the
  four CI build targets. It does not establish a Chombo revision because every
  locked checkout action omits `ref`.
- The manifest separates GRChombo, Chombo, PETSc, and container records.
  Metadata-only verification passes the locked source and reports gaps;
  target-probe mode requires a real pinned Chombo checkout. The later
  project-qualification update below supersedes this audit's initial blocked
  result.
- The minimal `2/4/4` `parstream.H`/`FArrayBox.H`/`Cell.hpp` probe was
  initially tracked as blocked. It now passes against the qualified commit
  without stubs.
- PETSc is not a dependency of the next pointwise GP storage wrapper. It is a
  separate later lock for `USE_AHFINDER`/P3-14.
- Exact historical provenance remains open, but the core project lock is
  closed by the later strict qualification. Container and PETSc/AHFinder
  provenance remain separate.

## First production-contract substage update

- P0-2's isolated enum/registration contract is complete. It has exactly 18
  slots, one registration/checkpoint/output ordering, 13 physical plus five
  gauge variables, no visible-y fields, and explicit parity, stock-overlap,
  future-owner, storage, and hidden-multiplicity metadata.
- The old 27-slot `UserVariables.hpp` remains the live smoke/comparison
  scaffold. The new contract is not production evolution wiring and does not
  change checkpoints or outputs from the current executable.
- The target `2/4/4` macro assertions compile with inspected GRChombo
  `DimensionDefinitions.hpp` and `Tensor.hpp`. Unmodified stock
  `ADMConformalVars` and `VarsTools` cannot map the target state: their
  CH-dimension-selected three-component symmetric interval conflicts with the
  ten components required by `DEFAULT_TENSOR_DIM=4`, and two-component vector
  intervals conflict with four-component default tensors. The future
  black-string Vars/mapping adapter must own this translation.
- P1-4 GP initialization is next, but it must consume the reviewed contract
  through the dedicated production adapter seam. It must not reuse the old
  27-slot scaffold or begin RHS, cleanup, source, periodic, or evolution work.

## Reduced Vars and pointwise GP substage update

- The black-string-specific reduced Vars seam is complete for local storage.
  It groups 13 physical and five gauge variables, loads/stores the reviewed
  18-slot array exactly, and replaces the incompatible stock enum-mapping
  concept without copying the enum.
- P1-4 now has an exact pointwise GP initializer and analytic radial metadata.
  All 18 values, determinant, weighted trace, reconstructed `K_IJ`, gauge
  values, input rejection, and derivative formulas pass focused tests.
- The one-point Chombo storage portion of P1-4 is complete. A thin
  black-string adapter loads and stores all 18 slots through real
  `Cell<double>` access backed by a DIM2 `FArrayBox`; it performs no
  hidden-multiplicity expansion and writes `hww/Aww` once each.
- P1-4 compute and isolated traversal are complete. No live registration,
  ghost, or checkpoint path calls the new seam.
- The Chombo source/build blocker is resolved by the project-qualified tuple.
  The exact next substage is hidden/cartoon RHS adaptation. Live initializer
  wiring remains deferred and must eventually call the existing compute class.
- Hidden/cartoon RHS, cleanup, constraints, fixed lapse source, periodic
  ownership, evolution, and diagnostics remain later backlog items.

## Chombo project-qualification update

- P0-1 core dependency qualification is complete as
  `PROJECT_QUALIFIED`. Official Chombo commit
  `8684f2e000106f1abadb72642e1d15351867f98f` was the default-branch head at
  the successful locked-GRChombo CI timestamp. Public workflow logs,
  artifacts, caches, and PR-head runs exposed no exact historical SHA, so
  historical provenance remains `inferred`.
- The candidate builds BaseTools, BoxTools, AMRTools, and AMRTimeDependent in
  serial DIM2 mode. The real `CH_SPACEDIM=2`, `GR_SPACEDIM=4`,
  `DEFAULT_TENSOR_DIM=4` probe compiles, links, and runs with Chombo
  `parstream.H`/`FArrayBox.H`, GRChombo `Cell.hpp`, and the reduced Vars seam.
  Stock DIM3 VariableStore and CCZ4Geometry tests also pass.
- The core verifier enforces both dependency SHAs, detached-clean state,
  headers, and required libraries. Former Docker image/recipe provenance and
  PETSc/AHFinder remain explicit separate gaps; neither blocks the
  hidden/cartoon RHS adapter.
- The one-point black-string `Cell`/`FArrayBox` storage adapter is complete.
  The GP `BoxLoop` compute class and isolated real traversal are also
  complete. The next authorized substage is only hidden/cartoon RHS
  adaptation; live registration, cleanup/constraints, source, periodic
  ownership, evolution, and diagnostics remain open.

## Cell/FArrayBox storage seam result

- `BlackStringCellStorage::load(const Cell<double> &) -> Variables<double>`
  and `BlackStringCellStorage::store(const Cell<double> &,
  const Variables<double> &)` are the complete production-facing wrapper API.
  Slot iteration and component checks derive only from
  `BlackStringProductionVariables`.
- The focused fixture creates a real 18-component DIM2 `FArrayBox`, binds an
  explicit `IntVect` through Chombo `BoxPointers` and GRChombo `Cell`, and
  performs one-point access only. No traversal, ghost-ownership assumption,
  or `BoxLoop` is present.
- Distinct values round-trip exactly in production order. All 18
  component-local mutations affect only their intended slot; neighboring
  points remain unchanged; `hww` and `Aww` each occupy one stored component.
- Mutations for swapped, duplicate, omitted, off-by-one, visible-tensor
  `hww`, duplicated hidden representatives, neighboring-cell writes, the
  legacy 27-component shape, and an oversized output shape are rejected.
- Three GP states, including `(r0,x)=(1,2)`, store and reload slot-for-slot
  while preserving `hww=1`, determinant and weighted-trace identities,
  reconstructed `Kxx/Kzz/Kww`, and all lapse/shift/gauge values.

## GP BoxLoop initializer result

- `BlackStringGPInitialData::make_compute(r0, dx, origin)` is the
  production/default API and returns a compute whose default storage policy
  calls `BlackStringCellStorage::store`. `compute(Cell<double>)` evaluates the
  target cell center and calls
  `BlackStringGPPointwiseInitialData::make_pointwise_vars`.
- Direction 0 is radial `x`; direction 1 is compact `z`. The target adapter
  uses GRChombo's exact cell-center convention
  `(index + 1/2) * dx - origin[direction]`. Locked GRChombo's stock
  `Coordinates` constructor has no `2/4/4` branch, so the project adapter owns
  only this coordinate translation and no physics.
- The real DIM2 fixture traverses a 4-by-5 requested box inside a 6-by-7
  allocation. All 20 requested cells are visited exactly once, all 18 slots
  are written, and all 22 outside points retain sentinels.
- Every cell is compared with a separately recomputed pointwise GP state at
  tolerance `5e-13 + 5e-12 max(|a|,|b|)`. Maximum absolute and normalized
  errors are both zero in the focused fixture.
- Determinant, weighted trace, reconstructed `Kxx/Kzz/Kww`, Theta,
  hatted-Gamma, lapse, shift, and gauge checks pass at every point. Same
  radial indices agree exactly across compact positions and `hww=1`
  everywhere.
- A test-only storage policy wraps the real adapter and records into shared,
  mutex-protected state. It observes 20 calls in the requested box, zero
  outside calls, and the exact 18-slot ledger on every call; `hww` and `Aww`
  each occur once. The numerically exact direct-write mutation records no
  adapter call and is rejected for that reason, not by metadata.
- Active mutations also reject radial/compact direction confusion, node
  centering, ignored origin, compact-coordinate dependence, `hww=x^2`,
  omitted/duplicate/swapped writes, incomplete/double/outside traversal,
  nonpositive radial cells, and the legacy 27-slot shape.
- Repair acceptance was held pending until strict project diagnostics and
  real adapter instrumentation passed. Target fixtures now compile project
  code with `-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`; dependency
  paths alone use `-isystem`, and the project-warning negative compile passes.
- The future live call site remains `BlackStringToyLevel::initialData()` with
  `BoxLoops::loop(BlackStringGPInitialData::make_compute(r0, m_dx,
  m_p.center), ..., disable_simd())`. No live application or smoke path is
  changed in this substage.

## Explicit non-goals

Do not add backlog items to recreate:

- time integration or RK4;
- AMR hierarchy/regridding;
- MPI or OpenMP parallelism;
- checkpoint/restart or HDF5 ownership;
- generic ghost exchange;
- mature GRChombo interpolation/reduction machinery;
- a custom eigensolver, shift-invert path, or stationary outer boundary.

Use GRChombo directly for these facilities and restrict adaptations to the
target-dimension state map, black-string/cartoon physics, gauge source, setup,
and observables.

## Source-information blockers

Before P1-6 or P3-14 can be planned precisely, resolve:

1. the historical Chombo SHA and former container tuple remain unavailable;
   the core project source lock is qualified, while the Docker digest remains
   a separate production-runtime provenance gap;
2. whether the collaboration has a maintained higher-dimensional
   modified-cartoon or black-string branch not present in commit `37e6595`;
3. the authoritative production formula/source for hidden-sphere CCZ4 terms,
   if it differs from the independently derived custom oracle;
4. a PETSc-enabled GRChombo build lock—the existing 2D AH test previously
   launched only the `USE_AHFINDER`-disabled skip path;
5. the intended GRChombo string-horizon area/data conventions beyond the
   generic `AHStringGeometry` interface.
