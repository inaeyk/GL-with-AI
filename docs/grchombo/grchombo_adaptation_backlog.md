# GRChombo Adaptation Backlog for GL Production

Status: prioritized design backlog with comparison-batch 1-3 evidence.
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
| P0-1 | Reproducible GRChombo source/version lock | Current origin plus build/container tooling | Pin commit `37e6595...` or an approved replacement, Chombo revision, container digest, compiler flags, and local patches in a machine-readable manifest | None | Fresh checkout/build reproduces identical manifest; no ignored floating dependency |
| P0-2 | Convention and slot adapter | `CCZ4Vars.hpp`, `ADMConformalVars.hpp`, `UserVariables.inc.hpp`, Tensor/VarsTools | Explicit `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, 27-slot map, hidden representative policy | P0-1 | Level 1 slot/dimension/convention tests pass exactly |
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
  top-level source lock, and the Chombo/container digests remain unresolved.
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
