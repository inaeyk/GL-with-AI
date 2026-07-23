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

| Priority / order | Adaptation item | GRChombo source to reuse | Project-specific work | Dependency | Acceptance / exit criterion |
|---|---|---|---|---|---|
| P0-1 | Reproducible GRChombo/Chombo build lock | Current origin, locked CI, Chombo Make infrastructure, and container tooling | Keep the tracked GRChombo commit; recover the authoritative `GRChombo/Chombo` revision/patch set and compiler/container tuple; keep PETSc separate until AHFinder | None | Metadata verifier is honest; target probe uses real Chombo headers; strict build/smoke pass; full verifier rejects every unresolved required field |
| P0-2 | Convention and slot adapter | `CCZ4Vars.hpp`, `ADMConformalVars.hpp`, `UserVariables.inc.hpp`, Tensor/VarsTools | Explicit `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4`; reviewed 18-slot black-string map with no visible-`y` slots and one multiplicity-two hidden representative | P0-1 source verification | Level 1 macro/slot/name/parity/permutation tests pass exactly |
| P0-3 | Formula comparison harness | `CCZ4RHS::rhs_equation`, `CCZ4Geometry`, gauge classes | Test-only adapter accepting supplied analytic jets and emitting per-family rows | P0-1, P0-2 | First five comparison tests execute without production evolution |
| P1-4 | Exact black-string GP initial data | GRChombo initial-data `BoxLoops` pattern and project parameter parser | Implement `alpha=1`, `beta^x=sqrt(r0/x)`, flat GP spatial metric, target K/A/Gamma/Theta, hidden slots | P0-2 | Cellwise analytic comparison, determinant/trace, and constraints pass |
| P1-5 | Fixed GP-holding lapse source | `MovingPunctureGauge` adapter pattern | Add field-independent `S_alpha=3 sqrt(r0/x^3)` with explicit validation/production policy | P1-4 | Raw lapse residual remains visible; source-adjusted GP residual vanishes; JVP unchanged |
| P1-6 | Modified-cartoon hidden-sphere production path | GRChombo CCZ4 orchestration, derivatives, BoxLoops | Adapt validated custom hidden Ricci, lapse Hessian, Gamma, algebraic, shift, and Z terms; remove smoke freeze | P0-3, P1-4 | All Level 1/2 hidden-family comparisons and ownership mutations pass |
| P1-7 | Hidden-aware algebraic cleanup and constraints | `TraceARemoval`, `PositiveChiAndAlpha`, `Constraints`, `AMRReductions` | Extend determinant/A-trace cleanup and Hamiltonian/momentum constraints with multiplicity two | P1-6 | Weighted residuals and pointwise constraints match oracle; convergence passes |
| P1-8 | Compact periodic `z` production domain | GRChombo periodic boundary/domain parameters and derivative classes | Lock `L`, `k_n=2 pi n/L`, parity conventions, radial/compact direction mapping | P0-2 | Periodic wrap and Fourier derivative tests pass at production order |
| P1-9 | Unperturbed background evolution | `GRAMR`, RK4, ghost fill, boundaries, checkpointing | Configure target grid, source, hidden RHS, diagnostics, and conservative validation window | P1-5 through P1-8 | L4-01 stationarity and constraint convergence pass; restart smoke succeeds |
| P2-10 | Fourier perturbation initialization | Initial-data BoxLoop plus periodic grid | Add normalized even/odd SO(3)-scalar perturbation families with amplitude guard | P1-9 | Linear amplitude scaling and mode/parity leakage tests pass |
| P2-11 | Fourier amplitude diagnostics | `AMRInterpolator`, reductions, `SmallDataIO` | Adapt custom cosine/sine projections to AMR-consistent sampled/integrated output | P2-10 | Synthetic and production single-mode recovery tests pass |
| P2-12 | Growth-rate extraction | `SmallDataIOReader`/project analysis output contract | Implement fit-window, uncertainty, resolution, and observable-consistency reporting | P2-11 | Synthetic exponent tests and window mutations pass before physical rates |
| P2-13 | Production convergence workflow | GRChombo AMR/restart/output machinery | Reproducible multi-resolution parameter sets and comparison tables | P1-9, P2-12 | Background, constraints, perturbations, and rate show documented convergence |
| P3-14 | String MOTS/horizon adapter | `AHFinder`, `PETScAHSolver`, `AHStringGeometry`, interpolation | Supply target variables, `S2 x S1` geometry, hidden expansion terms, PETSc configuration | P1-7, PETSc source lock | Uniform `x=r0` MOTS recovered with convergent residual; restart supported |
| P3-15 | `R_H`, minimum radius, and horizon area | AH surface data, interpolation, reductions, `SmallDataIO` | Evaluate `R_H=h sqrt(hww/chi)`, minimum over z, correct string area | P3-14 | Uniform analytic values and perturbed manufactured profiles converge |
| P3-16 | Nonlinear GL production workflow | All mature GRChombo runtime infrastructure | Parameter families, perturbation sweep, constraints/horizon monitoring, failure criteria, archival metadata | P2-13, P3-15 | Reviewed end-to-end runbook; unperturbed and small-perturbation gates pass first |
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
  target-probe mode requires a real pinned Chombo checkout; full-build mode
  cannot accept unresolved provenance.
- The minimal `2/4/4` `parstream.H`/`FArrayBox.H`/`Cell.hpp` probe is tracked
  but blocked because `external/Chombo` is absent and the authoritative
  Chombo commit is unknown. It uses no stubs.
- PETSc is not a dependency of the next pointwise GP storage wrapper. It is a
  separate later lock for `USE_AHFINDER`/P3-14.
- P0-1 remains open. Its next input is the authoritative full Chombo commit
  and patch set from the former environment, collaborators, or maintainers.
  Candidate commits may be compile-tested explicitly but are pinned only
  after the strict target probe and smoke build succeed.

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
- P1-4 is not production-complete: no Chombo `Cell`, `FArrayBox`, `BoxLoop`,
  live registration, ghost, or checkpoint path calls the new seam.
- The exact remaining integration blocker is the unresolved Chombo
  source/build tuple (`parstream.H` is unavailable locally), followed by a
  thin black-string `Cell`/`FArrayBox` wrapper and initial-data compute class.
  The wrapper must call the reduced load/store seam; it must not use stock
  `ADMConformalVars`/`VarsTools` mapping.
- Hidden/cartoon RHS, cleanup, constraints, fixed lapse source, periodic
  ownership, evolution, and diagnostics remain later backlog items.

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

1. the authoritative GRChombo/Chombo/container version tuple; the current
   nested checkout is ignored and the Docker digest is unavailable here;
2. whether the collaboration has a maintained higher-dimensional
   modified-cartoon or black-string branch not present in commit `37e6595`;
3. the authoritative production formula/source for hidden-sphere CCZ4 terms,
   if it differs from the independently derived custom oracle;
4. a PETSc-enabled GRChombo build lock—the existing 2D AH test previously
   launched only the `USE_AHFINDER`-disabled skip path;
5. the intended GRChombo string-horizon area/data conventions beyond the
   generic `AHStringGeometry` interface.
