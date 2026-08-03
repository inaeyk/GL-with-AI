# GRChombo Adaptation Backlog for GL Production

Status: historical implementation backlog with comparison-batch 1-4 and
D1–D16 evidence. Current status and execution order are owned exclusively by
`research_plan/stage_checklists.md`.
Production must reuse mature GRChombo infrastructure and adapt only
black-string-specific physics, configuration, and diagnostics. Do not
independently rebuild RK4, AMR, MPI/OpenMP, checkpoint/restart, ghost exchange,
reductions, interpolation, or generic parameter parsing.

Stage 4AO-D-E1 is complete at the live application seam. The target-owned
files register 18 slots, reuse the validated GP compute class, obtain
derivatives from GRChombo's fourth-order operator, expand reduced data to
target `d=4`, call locked GRChombo directly, own the five gauge rows and one
fixed lapse source, clean only after update, expose observational `H,Mx,Mz`,
and configure direction 1 periodic through Chombo. The focused
`CH_SPACEDIM` grid-infrastructure adapter and define-only gate are complete.
E2 also completes a bounded serial level-zero GP diagnostic and matched-domain
convergence with exact background-preserving radial ghost data. A separate
minimal GP-subtracted extrapolation/Sommerfeld policy now enables the first
small perturbative boundary smoke. A fixture-only compact Fourier initializer,
cadenced projection, and two-boundary/three-resolution short evolution now
supplies transient Fourier evidence; the former stable/unstable short-window
interpretation is superseded. D9 finds that the old inner face is not
strictly outside the conservative fastest lapse envelope, but deeper
excision fails to stabilize either matched provisional-boundary resolution
or the optional exact-GP control. D10 then finds that the live direct
`rhs_equation` path bypasses the only locked upstream call that adds KO
dissipation; effective coverage is `0/18`, so the tangent probe stops before
numerical iteration with `DISSIPATION_PATH_DEFECT_IDENTIFIED`. Sustained
evolution is subsequently stabilized by the D11 project-owned
grid-dimension-safe KO helper. D12 still finds no compatible multi-field
time-domain plateau. D13 then applies the actual KO-stabilized one-step map
matrix-free at `k=pi/4,pi/2`: epsilon and Fourier-sector checks pass, but
independent seeds have cross-profile overlaps only `0.5846` and `0.5572` and
do not share rates or norm/constraint behavior. Its bounded result is
`NONNORMAL_OR_NO_CONVERGED_MODE`. D14 then builds the complete two
fixture-only `576x576` Fourier-block spectra through the existing
Chombo/LAPACK link. The only resolved positive spectral leader is
inner-boundary/constraint dominated, the other block is clustered near
neutral with large constraint response, and no preliminary physical candidate
passes. Its bounded result is
`CONSTRAINT_GAUGE_OR_BOUNDARY_SPECTRUM_DOMINATES`. Physical radial-boundary
acceptance, a converged threshold, refinement, MPI, broader diagnostics, and
horizons remain open. D15 finds no shared bulk candidate after gauge/boundary
sector isolation. D16 then finds that the instantaneous constraint nullspaces
are not sufficiently invariant under the live tangent map. The physical-mode
investigation is therefore closed inconclusive: the KO-stabilized serial
production core is functional, no credible boundary-independent,
constraint-compatible GL mode has been identified, and further fixture-only
spectral projection is stopped. The D12–D16 spectral investigation is closed
inconclusive; no automatic return to Fourier scans, dense spectra, or
constraint-nullspace projection is authorized.

The active deliverables are now:

1. cluster execution baseline;
2. one explicit physical radial/constraint strategy;
3. linear GL acceptance;
4. nonlinear and observable readiness; and
5. scientific release.

No D17 or automatic D-series continuation is active. Historical priority and
stage labels below are provenance only; their unresolved deliverables are
mapped into the five milestones.

## Active Milestone 1 delivery plan

M1-A through M1-E are complete.
`M1-E PASS — performance and scaling baseline qualified`, and M1-F /
Milestone 1 is `CLUSTER_EXECUTION_BASELINE_ACCEPTED`. The production-sized
rank-1/rank-8 final states are available and bitwise exact, with common
canonical state hash
`863095316ffbcf26af513b643c7bb2a1c1ed21131b07406ad068c7828a8c26ea`.
Timer-category limitations remain nonblocking and are labeled
`UNAVAILABLE_FROM_EXISTING_FRAMEWORK_TIMERS`. This accepts only the stated
engineering baseline: G-Engineering is passed, while G-Physics and overall
Checkpoint G remain blocked. The frozen workload uses
`CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4`, the committed
18-variable production core, `ko_sigma=0.3`, exact GP, periodic compact `z`,
the provisional radial boundary, one AMR level, no perturbation/AHFinder, and
default-off diagnostics. Production physics changes only for a directly
demonstrated MPI/HDF5 defect.

| Stage | Deliverable | Exit evidence |
|---|---|---|
| M1-A | Optimized serial/no-HDF5, MPI/no-HDF5, and MPI/HDF5 builds | Compiler/MPI/HDF5/Fortran provenance, dependency commits, flags, executable hashes, clean-build commands, startup checks, and the applicable HDF5 check |
| M1-B | Serial, MPI-1, MPI-2, and MPI-4 matched level-zero runs where available | One domain/global resolution; all 18 fields and global valid-cell ownership; field/constraint/drift equivalence; no duplicate RHS/ghost work or hidden-direction access |
| M1-C | Distributed ghost and boundary qualification | Radial/compact seams, periodic rank exchange, first/second/mixed derivatives, KO, radial ownership, corners, and all 18 components |
| M1-D | Existing-format HDF5/checkpoint/restart | Valid plot/checkpoint files, all fields and metadata restored, continuous/restarted equivalence, no reinitialization, and output provenance |
| M1-E | Performance and scaling | RK time, valid-cell RHS/s, per-rank and total memory, exposed communication time, output costs and sizes, strong scaling, and weak scaling |
| M1-F | Closure decision | `CLUSTER_EXECUTION_BASELINE_ACCEPTED`, `CLUSTER_EXECUTION_BASELINE_PARTIAL`, or `CLUSTER_EXECUTION_BASELINE_BLOCKED` |

Acceptance requires correct MPI evolution, distributed ghost/physical-boundary
ownership, serial/MPI equivalence, HDF5, checkpoint/restart, scaling, and
memory. AMR refinement, AHFinder, physical GL identification, `k_cr r0`, and
pinch-off are outside Milestone 1.

Audit M1-A through M1-C together after M1-C; do not audit M1-A alone. The
milestone-closing audit after M1-E is complete. Each substage permits at most
two repair cycles,
and the milestone stops for review after 30 numerical launches. Reuse Chombo
MPI ownership/exchange and existing HDF5/checkpoint formats; add no new MPI
abstraction. Use delta tests and committed serial/KO evidence. Full suites are
reserved for shared production-core changes. Production budgets remain one
target-`d=4` RHS per cell/stage, one fused KO addition, no per-cell allocation/
logging/counters/mutexes, default-off diagnostics, and separately measured
output cost.

## Active Milestone 2 production plan

Current status remains owned by `research_plan/stage_checklists.md`.
Milestone 1 is `CLUSTER_EXECUTION_BASELINE_ACCEPTED`; G-Engineering is passed;
G-Physics and overall Checkpoint G remain blocked. M2-A is blocked after its
only design-repair pass and awaits focused audit plus human roadmap review;
no production implementation has begun.

The selected production strategy is **Characteristic-aware radial boundaries
plus discretely constraint-compatible initial perturbations**. Its objective
is to replace the provisional physical setup with one production strategy
capable of supporting a boundary-independent, constraint-controlled linear GL
perturbation. It does not attempt a full `k_cr r0` scan, nonlinear evolution,
AMR, apparent horizons, or radiation extraction.

### M2-A — Design and convention lock

Before implementation, document the exact 18-variable live state and gauge
parameters; derive the frozen-coefficient radial principal speeds and
characteristic fields from the actual live CCZ4/gauge system around GP;
classify incoming/outgoing ownership at both faces; specify the discrete
constraint-correction operator and boundary/seed interfaces; and state cost,
limitations, failures, and fallbacks.

The boundary design must impose data only on incoming characteristic fields,
preserve analytic GP to truncation order, retain periodic `z`, and remain
compatible with the fourth-order interior and fused KO path. Inner excision
requires strict outflow of the complete relevant characteristic set; an
`x_in < r0` location is not sufficient. Otherwise the design supplies incoming
inner data. The outer treatment uses background-subtracted radiative data for
incoming physical/gauge/constraint characteristics and does not separately
Sommerfeld-evolve every stored component. Work remains proportional to radial
surface cells, without a second volume RHS pass, per-cell allocation/logging,
or runtime polymorphism.

The perturbation design starts from a smooth parity-compatible single-`k`
seed, then solves or corrects it using live discrete variables, derivatives,
domain, and boundaries. It must control the linearized Hamiltonian, momentum,
determinant, trace-free, and validated Gamma/Z conditions to a declared
tolerance. Correction is initial-data preparation only; the evolution must
not run inside an algebraically projected constraint nullspace. Preserve
amplitude linearity, `+epsilon/-epsilon`, one robust bulk observable, and
multiple physical acceptance fields. If no validated Gamma/Z helper exists,
validate one or explicitly exclude that condition from the claim.

Current classification: `M2-A DESIGN BLOCKED — the complete locked live
first-order radial principal matrix has a determinant/weighted-trace Jordan
block at coordinate speed -beta^x (algebraic multiplicity two, geometric
multiplicity one), so no complete characteristic transform or inverse exists
for either radial normal`. The exact matrix and hard stop are in the repaired
M2-A documents. The former QR reconstruction and SAT fallback are removed;
no seed or boundary implementation is authorized. M2-A used zero numerical
launches. Focused audit and human roadmap review are required.

### M2-B — Production implementation

Use the smallest possible production diff. Gates require finite stable exact
GP, no physical-boundary work at MPI seams, rank independence, intact
checkpoint/restart, one full target-`d=4` RHS plus one fused KO addition per
cell/stage, surface-only boundary cost, and default-off diagnostics. Focused
tests cover characteristic ownership, GP preservation, manufactured incoming
and outgoing waves, corrected-seed residuals, all 18 slots/parity, and only
the MPI/restart regressions touched by shared changes. The initial M2-B and
focused-test budget is at most 10 launches, followed by one audit.

### M2-C — Scientific acceptance

Use only one likely unstable low-`k` case, one high-`k` control, one
unperturbed GP control, two resolutions, two outer-boundary locations, and two
amplitudes where needed, within 12 launches. Require one common exponential
rate across several nonzero physical fields, a phase-neutral Fourier
amplitude, stable radial profile, amplitude linearity, controlled linearized
constraints, boundary/resolution robustness, and no growing boundary-localized
or grid-scale contamination. Do not scan another `k` before M2-C passes. A
milestone-closing audit follows.

At most two production repair cycles may follow the first M2-B
implementation, each with at most six launches. Each records the failed gate,
hypothesis, exact production change, bounded run count, and success/stop
criteria. Two failed cycles stop for human review. Failure cannot trigger a
new spectral framework, dense eigensolver campaign, open-ended sweep, named
stage per diagnostic, or fixture-only analysis without a production decision.
The hard cap is 34 launches. Documentation-only corrections require no second
audit.

Performance is compared with accepted M1. Retain one full target-`d=4` RHS,
one fused KO addition, no second volume pass, no production spectral/fitting
code, no per-cell allocation/logging/counters/mutexes/virtual dispatch,
`O(N_z)` boundary work, and one-time constraint correction outside the hot
path. Any slowdown above 15% requires explanation and approval before closure.

Closure returns exactly one of
`PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_ACCEPTED`,
`PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_PARTIAL`, or
`PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_BLOCKED`. `ACCEPTED` requires
validated characteristic-aware radial boundaries, a discretely
constraint-compatible perturbation, stable GP, and at least one boundary- and
resolution-robust linear result with compatible physical-field rates and
controlled constraints. A final `k_cr r0` bracket is not required.

## Historical priority rules

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

The historical locked production order was:

1. [complete] thin `Cell`/`FArrayBox` storage seam around the reduced 18-slot
   Vars and GP point initializer;
2. [complete, live] GP `BoxLoop` initializer in the black-string
   `initialData()` path;
3. [complete] reduced `(2+2)` to full target-`d=4` pointwise expansion and
   direct GRChombo RHS evaluation;
4. [complete] direct nonlinear 13-row equivalence with target
   hidden-suppressed, full-minus-suppressed increment, and full target values
   reported separately;
5. [complete] hidden-aware cleanup and constraints;
6. [complete, pointwise] production-style fixed lapse-source hook;
7. [complete] live BoxLoop RHS/cleanup/source wiring and periodic direction-1
   domain/ghost ownership;
8. [complete] dimension-correct GRAMR/boundary setup and a define-only `2/4/4`
   gate;
9. [bounded diagnostic and matched-domain convergence complete] 4/8/16-step
   unperturbed GP evolution on one fixed physical domain with an
   exact-background radial ghost policy;
10. [provisional smoke complete] GP-subtracted inner extrapolation and outer
    Sommerfeld for the first scalar/one-z perturbed level-zero run;
11. [first bounded sign diagnostic complete; threshold open] perturbed
    Fourier-mode evolution at two wavenumbers, three resolutions, and two
    outer-boundary locations;
12. horizon and nonlinear diagnostics after PETSc/AHFinder and observable
    conventions are qualified.

Current workflow is deliverable-based. Diagnostics must state a hypothesis,
decision, run budget, and production action for every result. At most two
failed repair cycles are allowed per subsystem before architecture review.
Validation is delta-based and reuses committed evidence. Independent audits
are reserved for production equations, boundaries, gauge, AMR/MPI, horizons,
scientific acceptance, and milestone closure; documentation-only repairs
require no second audit.

## `2/4/4` GRAMR dimensional-blocker audit

This is a source/design audit, not an implementation or evolution result. The
target split is:

```text
CH_SPACEDIM=2
GR_SPACEDIM=4
DEFAULT_TENSOR_DIM=4
```

`FOR` and `FOR1` expand to `DEFAULT_TENSOR_DIM`. They are valid for physical
tensor indices, but never for Chombo grid directions or storage whose extent
is `CH_SPACEDIM`.

### Observed failure and setup path

The observed runtime path is

```text
AMR::define
  -> GRAMRLevel::define(ProblemDomain)
  -> BoundaryConditions::define
  -> RealVect::operator[]
```

`GRAMRLevel.cpp:51-53` passes the two-component parameter center to
`BoundaryConditions::define`. At `BoundaryConditions.cpp:235`,
`FOR(i) { m_center[i] = a_center[i]; }` executes four iterations even though
both `RealVect m_center` and `std::array<double, CH_SPACEDIM> a_center` have
two components. The first trapped access is `i=2`. This is the first observed
failing site, not the complete adaptation surface. Boundary-parameter setters
at lines 44, 53, and 89 have the same unchecked array mismatch and can corrupt
state earlier without producing the same `RealVect` assertion.

The setup operations in `SetupFunctions.hpp:143-156` are correctly
grid-dimensional: `IntVect`, `Box`, and `ProblemDomain` use Chombo dimensions,
and periodicity is set with `dir < SpaceDim`. `GRAMRLevel::contains` and the
checkpoint/plot periodicity loops likewise use `CH_SPACEDIM` or `SpaceDim`.
They are valid gridded loops and need no target-tensor widening.

### Mismatch inventory

| Area and source location | Classification | `2/4/4` finding | Black-string relevance |
|---|---|---|---|
| `ChomboParameters.hpp:240,329,353,421` | Incompatible mixed use; intended gridded loop | `FOR` indexes `std::array<...,CH_SPACEDIM>` and `IntVect`. | Immediate parameter parsing/checking. E1's project-local pre-include scopes this one header to two directions, but that does not adapt out-of-line runtime code. |
| `BoundaryConditions.cpp:44,53,89` | Incompatible mixed use; intended gridded loop | Boundary periodic/low/high arrays have two entries but `FOR` executes four times. | Immediate and potentially earlier than the trapped define access. |
| `BoundaryConditions.cpp:235` | Incompatible mixed use; intended gridded loop | Four writes target a two-component `RealVect` and center array. | First observed trapped site in `GRAMRLevel::define`. |
| `BoundaryConditions.cpp:324,415,439,471` | Incompatible mixed use; intended gridded loop | Boundary reporting and RHS/solution/diagnostic direction dispatch index two-entry boundary arrays. | Immediate after define and during ghost/boundary fills. |
| `BoundaryConditions.cpp:604,613` | Incompatible mixed use; intended gridded loop | Sommerfeld radius and derivative stencils index `RealVect`, `IntVect`, and grid strides. | The provisional adapter reuses the stock grown layout but supplies its own radial-x GP-subtracted Sommerfeld surface formula. |
| `BoundaryConditions.cpp:699-756,716,742` | Incompatible mixed use; intended gridded loop | Extrapolation clamps two-dimensional `IntVect`s with four-direction `FOR`. Its radius calls use the coordinate wrapper. | The provisional adapter uses only stock RHS-ghost extrapolation; project-owned solution ghosts never use the stock `sqrt(x^2+z^2)` radius. |
| `BoundaryConditions.cpp:800,854,929,1027,1074,1114` | Incompatible mixed use; intended gridded loop | Copy, coarse/fine interpolation, boundary-box growth, and `ProblemDomain` growth all traverse Chombo directions with tensor bounds. | Copy/fill is level-zero downstream; interpolation and grown-grid paths become relevant with AMR or Sommerfeld/mixed boundaries. |
| `utils/Coordinates.hpp:38-49,73-103` | Incompatible target coordinate wrapper | Stock code has branches for `3/3`, `2/3` Cartoon, and `2/2`, but not `CH_SPACEDIM=2,DEFAULT_TENSOR_DIM=4`. Its static CH2 radius treats both grid directions as a Euclidean plane. | The application-local wrapper supplies `(x,z)` and prevents fake hidden coordinates, but its generic radius is still not a black-string radial-boundary policy. |
| `FourthOrderDerivatives.hpp:77,249,349,405` and `SixthOrderDerivatives.hpp:80,280,386,470` | Incompatible mixed use; D10 exposes a live omission | Bulk first/second derivative, advection, and dissipation overloads return physical tensors but index `m_in_stride[CH_SPACEDIM]` with `FOR`. Only grid directions may select a stride; hidden derivatives require target-specific expansion. | E1's explicit direction-0/1 derivative kernels and algebraic hidden jets avoid hidden grid access. D10 confirms, however, that direct `rhs_equation` evaluation also bypasses `CCZ4RHS::compute` and therefore its `add_dissipation` call. Effective live KO coverage is `0/18`; a future repair needs a thin `CH_SPACEDIM` dissipation adapter rather than the unsafe generic overload. Sixth order is disabled. |
| `ChiTaggingCriterion.hpp:27,30`, `PhiAndKTaggingCriterion.hpp:30-37`, and `ComputeModGrad.hpp:31-39` | Incompatible mixed use; intended gridded norm | The criteria request derivative directions 2 and 3 from two grid strides and contract them as if they were gridded. | Not on E1: the black-string level owns a zero criterion. Must remain unavailable unless adapted. |
| `ChiExtractionTaggingCriterion.hpp:59-63`, `ChiPunctureExtractionTaggingCriterion.hpp:69-73`, and `ChiAndPhiTaggingCriterion.hpp:50-62` | Incompatible mixed use | They call the unsafe bulk second-derivative overload and contract all tensor directions as grid Hessian directions. Coordinate-radius tagging also assumes stock spherical/cartoon geometry. | Extraction and puncture tagging are disabled and irrelevant to the first unperturbed level-zero path. |
| `SimulationParametersBase.hpp:258-268` | Incompatible mixed use; intended gridded loop | Extraction-center validation indexes `CH_SPACEDIM` center/domain arrays with `FOR`. | Dormant while extraction is disabled; required before extraction qualification. |
| `AMRInterpolator.impl.hpp:810-818,838-849` | Incompatible mixed use; intended gridded loop | Reflective setup and parity traverse `IntVect`, query-coordinate, boundary, and corner arrays with tensor bounds. | Dormant for the define-only and first level-zero path; required before interpolation/extraction or reflective interpolation. |
| `SurfaceExtraction.impl.hpp:43,57,154` | Incompatible mixed use; intended gridded loop | A four-direction loop resizes/indexes `m_interp_coords`, whose extent is `CH_SPACEDIM`, and submits nonexistent coordinate directions to `InterpolationQuery`. | Extraction is disabled. Hidden coordinates must never be invented to satisfy this loop. |
| `SphericalGeometry.hpp:97-110` and `CylindricalGeometry.hpp:70-83` | Irrelevant to the current black-string path, and not a qualified target geometry | Stock CH2 spherical extraction is the one-hidden-direction Cartoon geometry; cylindrical geometry contains a `center[2]` branch. Neither represents the `(x,z)` black-string grid plus two suppressed sphere directions. | Deferred with extraction/horizon work; do not use fake hidden query coordinates as a shortcut. |
| `Constraints.impl.hpp:58-73` and `NewConstraints.impl.hpp:72-102` | Incompatible mixed use | A derivative-index array is declared `covd_A[CH_SPACEDIM]` and then indexed by physical `FOR`; the compute path also calls unsafe bulk derivatives. Merely shortening every tensor contraction to two would lose required hidden contributions. | Stock constraint compute is not usable. E1's validated target expansion and observational `H,Mx,Mz` compute remain the owner. |
| `ADMQuantities.hpp:61-121` | Incompatible target diagnostic | Bulk derivatives are unsafe, a three-coordinate vector initializes a four-component tensor, and a fixed three-dimensional Levi-Civita symbol is indexed by tensor-wide loops. | ADM mass/angular momentum diagnostics are not enabled or qualified. |
| `Weyl4.impl.hpp` | Incompatible target diagnostic | Bulk derivatives are unsafe and the null tetrad/electric-magnetic construction is explicitly three-spatial-dimensional while tensor loops are four-dimensional. | Radiation diagnostics are deferred and irrelevant to the first unperturbed path. |
| `NanCheck.hpp:64` | Irrelevant to the blocker; coordinate semantics still restricted | It does not perform a tensor-wide grid loop, but optional coordinate reporting inherits the selected coordinate adapter's meaning. | It may report `(x,z)` safely through the application adapter; it is not a radial-physics owner. |
| `AHFinder.impl.hpp:293,377`, `ApparentHorizon.impl.hpp:204,221,256,362,493,498,835`, and `AHFunctions.hpp:158,166,214,218,247` | Irrelevant to the current path and incompatible where instantiated | Center arrays and explicitly `CH_SPACEDIM` tensors are traversed by `FOR`; the code also contains separate higher-dimensional assumptions requiring a dedicated string-horizon design. | `USE_AHFINDER`/PETSc and horizons remain deferred. This audit does not qualify them. |
| `BoxLoops.impl.hpp`, `GRLevelData.cpp`, `AMRInterpolator`'s explicit `i < CH_SPACEDIM` loops, `Derivative.hpp`, and `SimpleArrayBox` | Valid gridded loop | These use `CH_SPACEDIM`, `SpaceDim`, `D_DECL`, or an asserted dimension bounded by `CH_SPACEDIM`. | Reuse unchanged. |
| `VarsTools.hpp`, `TensorAlgebra.hpp`, `CoordinateTransformations.hpp`, `CCZ4Geometry.hpp`, `CCZ4RHS.impl.hpp`, and moving-puncture gauge classes | Valid physical-tensor loop | These operate on `Tensor<...,DEFAULT_TENSOR_DIM>` and may use `FOR`. `GR_SPACEDIM` remains the equation coefficient owner. | Reuse unchanged through the validated target-`d=4` pointwise seam. |
| Stock black-hole/cosmology initial data, puncture tracking, and stock application tagging choices | Irrelevant to the black-string application | They are application-specific, often CH3-guarded, and are not selected by the isolated black-string factory. | Do not adapt or alter them for this stage. |

This inventory distinguishes source-level reachability from current
instantiation. It does not claim that fixing the first line, or even compiling
all listed templates, proves the full adaptation complete.

### Minimal adaptation architecture

1. Add a project-owned grid-loop scope for the black-string build. Compile the
   locked `BoundaryConditions` translation unit with `FOR` bounded by
   `CH_SPACEDIM`, using a target-specific object flag/include wrapper, and
   restore `DEFAULT_TENSOR_DIM` for every physics translation unit. All
   audited `FOR` uses in that boundary translation unit are grid operations,
   so this is narrower than changing global tensor semantics or copying
   boundary formulas.
2. Retain the existing project-local `ChomboParameters` scope, but add a
   compile/runtime guard proving it affects only that header. Do not hard-code
   hidden center or `IntVect` components.
3. Keep the live RHS on explicit direction-0/1 fourth-order kernels. Add a
   negative compile/runtime policy gate against the unsafe bulk derivative,
   generic tagging, stock constraint, ADM, and Weyl paths in the black-string
   target. Hidden derivatives remain owned only by the validated target
   expansion.
4. Add a define-only level-zero gate before any setup/advance call. This
   isolates domain and boundary-object construction from initial data,
   radial ghost filling, and evolution.
5. Adapt interpolation/extraction only when those features enter scope.
   Their query coordinate count must stay `CH_SPACEDIM`; target hidden
   directions are tensor geometry, not extra grid coordinates.
6. Treat radial-boundary semantics separately. Dimension-correct loops make
   memory access safe, but do not validate the current `sqrt(x^2+z^2)`
   extrapolation radius. No custom outer-boundary research or physical
   acceptance is reopened by this adapter.

If the build system cannot apply a target-specific object flag without
changing other applications, use a black-string-only wrapper translation unit
and exclude the ordinary boundary object only from this executable. A broad
global `FOR=CH_SPACEDIM` override is forbidden because it would truncate the
accepted four-dimensional CCZ4 tensor equations.

### Focused regression sequence

1. A `2/4/4` define-only fixture must execute `AMR::define` through the real
   black-string factory and return successfully without calling
   `setupForNewAMRRun`, `initialData`, or `advance`.
2. Inspect the created `ProblemDomain`: direction 0 is nonperiodic radial,
   direction 1 is periodic compact, both extents equal the requested grid,
   and no direction 2/3 grid query exists.
3. Run the define fixture with Chombo assertions plus address/undefined
   behavior sanitizers. Add canaries around parameter center/boundary arrays
   so the silent setter overruns are independently detected.
4. Compile and run the same define-only fixture at stock
   `CH_SPACEDIM=GR_SPACEDIM=DEFAULT_TENSOR_DIM=3`, then run the existing
   BinaryBH smoke gate. Its domain, center, periodicity, and output
   registration must remain unchanged.
5. Add a coordinate-ownership fixture proving every `RealVect`, `IntVect`,
   `ProblemDomain`, stencil stride, and interpolation query direction is less
   than `CH_SPACEDIM`. Requests for hidden grid coordinates must fail; the
   existing target expansion must still supply both hidden tensor copies.
6. Verify the project dependency lock before and after: GRChombo and Chombo
   stay at the pinned detached commits and clean, while all adapters/tests are
   project-owned.

The define repair and E2 radial-ghost gate now pass bounded serial level-zero
evolution and matched-domain convergence on
`(N_x,N_z)=(32,8),(64,16),(128,32)`. The fixed domain is
`L_x=8,L_z=2`, the Courant factor is `0.004`, and all resolutions end at
`t=0.004` after 4/8/16 steps. The policy analytically preserves the exact GP
background at both radial strips and their radial-periodic corners while
pure-z ghosts remain framework-owned. It deliberately does not qualify a
physical radial boundary, sustained evolution, AMR, MPI, perturbations,
broader diagnostics, or horizons.

The subsequent provisional boundary fixture supplies the minimum physical
policy for early perturbative runs. It requires `x_in<r0`, extrapolates every
stored `delta U` at the inner side without imposing incoming data, and uses a
componentwise GP-subtracted Sommerfeld condition at the outer radial surface.
It reuses GRChombo's boundary layout/RHS-ghost infrastructure but bypasses the
stock constant-asymptote, Euclidean-radius formula. This clears only a small
serial smoke, not sustained or AMR/MPI qualification.

The repaired Fourier fixture keeps all initializer coefficients, amplitude
sampling, fits, controls, mutations, and instrumentation test-only. Its
corrected sine seed is
`delta GammaZ=epsilon*(0.10 p' + 0.20 p/x + 0.25 k p) sin(kz)`; the former
`1.00 p'` coefficient is an expected-failure mutation. No zero-`Z` seed claim
is made. On `Lz=8`, it evolves `k=pi/4` and `k=pi/2` to `t=0.4` with CFL
`0.05` on matched `dx=1/6,1/8,1/12` sequences at `x_out=4.5` and `6.5`.
Both quadratures of `q=0.5 log(hww/chi)` enter the phase-neutral radial-RMS
amplitude, superseding the cosine-only result. Fits over `[0.1,0.4]`,
`[0.15,0.4]`, and `[0.2,0.4]` retain positive and negative signs respectively
at every resolution, boundary, and at `epsilon=1e-9,5e-10`; the largest
epsilon-normalized history difference is `6.62e-7` under a `5e-4` tolerance.
Paired unperturbed leakage is at most `4.14e-21`, and the seeded/leakage ratio
is at least `2.75e10`. Paired constraint differences are reported directly;
`Mz` is not labeled roundoff or convergent. The common turnover diagnosis
supersedes the physical positive/negative interpretation: these are
short-window transient fits, not stable/unstable classifications, a
threshold, an asymptotic rate, sustained evolution, or physical-boundary
qualification.

The fixture-only adaptive extension screens the fundamental mode at requested
and actual `k=0.82,0.86,0.90,0.94`, then adds `0.78,0.75` because the initial
set contains no sign change. Legal block-factor-four grids preserve each
requested physical `Lz=2*pi/k`; actual `dx=dz` differs from the requested
`1/8` by at most 2.9%. At `x_out=4.5`, CFL `0.05`, and `t_final=0.8`, all six
candidates have three negative fits satisfying `|Omega|>=3 SE`, while seeded
amplitudes remain at least `3.42e9` above paired-control leakage. The
authoritative scan consumes 12 evolutions and returns `AMBIGUOUS`. With no
positive endpoint, endpoint-only resolution, outer-boundary, and
second-amplitude runs are correctly skipped. No numerical bracket,
interpolated threshold, `k_cr_0`, or checkpoint promotion is claimed. All six
preserved histories rise initially and turn over near `t=0.3-0.4` before
decaying, so their late negative fits are inconclusive transient evidence,
not physical stability. They support no conclusion about whether `k_cr*r0`
is above or below `0.75`.

The exact-domain antisymmetric diagnostic then runs one control and signed
`epsilon=1e-8` pairs at `k=pi/4,pi/2` on `Lz=8`, `0.5<=x<=4.5`,
`(Nx,Nz)=(32,64)`, `dx=dz=1/8`, and CFL `0.05`. Neither mode meets the
rolling-slope plateau rules by `t=4`. The permitted control and `k=pi/4`
extension to `t=8` yields final width-`0.5` slopes
`2.656,2.732,2.880` and width-`1.0` slopes `2.476,2.587,2.737`, with
variation below `10.1%`, slope errors smaller than the slopes, maximum
even/odd contamination `1.26e-6`, and no noise-floor crossing. A signed
`epsilon=5e-9` pair agrees within `3.44e-6`. This establishes linearity and
is reclassified as
`LATE_TIME_LINEAR_INSTABILITY_DETECTED — PHYSICAL IDENTITY UNRESOLVED` for
`k=pi/4`; `k=pi/2` is
`NO_MODE_PLATEAU_WITHIN_TESTED_TIME`. Background control drift reaches
`1.97` at `t=8`, and the linearized constraint response grows explosively.
Those failures prevent identification as a GL mode. No negative plateau,
critical bracket, asymptotic rate, or physical-boundary qualification
follows, and the `k` scan remains suspended.

D7 reuses the existing `t=8` unperturbed series and consumes its full budget
of four new controls. The exact-GP radial-ghost attempt terminates on an
invalid reduced metric before it can test a cure. The available
zero-coefficient gauge parameters freeze lapse and shift but not `B^i`,
because the locked gauge RHS retains the `Gamma^i`-RHS coupling; this is not a
complete frozen-gauge isolation. Half CFL reproduces the baseline onset and
final drift to a ratio of `1.0007`. Fine `dx=dz=1/12` reduces early drift but
raises the final drift ratio to `3.244` and steepens late growth, showing
spatial sensitivity without convergence or a cure. All completed controls
remain `z` independent, with maximum Fourier content `5.03e-16`, and retain
machine-scale determinant/weighted-trace cleanup. The allowed classification
is `MULTIPLE_OR_UNRESOLVED`; spatial-discretization sensitivity is the
strongest surviving clue, not a unique diagnosis.

D8 replaces the incomplete D7 isolation tests with fixture-only controls.
The exact-GP abort occurs at timestep 397, RK stage 3's predictor
(`t=2.475`), when valid inner cell `(1,0)` acquires negative `hxx` and
determinant during the RK update. The preceding Chombo-exchange/radial-fill
sequence preserves valid cells, fills all 18 radial/corner components with
zero GP error, and leaves no invalid ghost metric. No exact-GP seam repair is
warranted. A compile-time fixture RHS policy then fixes lapse, shift, and
`B^i` exactly while retaining live `Theta` and `Gamma^i`; the drift `0.1`
crossing nevertheless moves to `t=2.225` and final drift reaches `3.4089`.
The optional exact-GP/frozen-gauge combination also fails near `t=2.36`.
The bounded classification is
`NEITHER_CONTROL_CURES — CORE_RADIAL_EVOLUTION_UNRESOLVED`; production
equations and boundaries remain unchanged and the `k` scan remains
suspended.

D9 inspects the locked live-gauge and CCZ4 principal terms. The conservative
local envelope contains `c=1` physical/constraint sectors,
`c=sqrt(0.75),sqrt(1.125)` transverse/longitudinal shift sectors, and fastest
`c_max=sqrt(2)` lapse propagation. The old `x_in=0.5` face is glancing in
that envelope and its first valid cell has an incoming `v_+`; the matched
`x_in=0.375` domain has strict margin at its face and first valid cells. This
is not a complete principal-system diagonalization. Nevertheless, medium and
fine provisional controls lose metric admissibility at `t=6.3625` and
`6.18333`, with exploding constraints and radial Nyquist content; the fine
run fails earlier. The allowed exact-GP control fails at `t=0.93125`. Ghost
audits preserve all 18 slots, do not overwrite valid cells, and keep all
three radial ghost coordinates positive. The bounded classification is
`EXCISION_PLACEMENT_NOT_SUFFICIENT`; production remains unchanged and the
`k` scan stays suspended.

D10 traces the complete radial semidiscrete path before constructing the
requested tangent map. `BlackStringLive::make_pointwise_input` applies the
locked fourth-order centered `d1`, `d2`, and mixed kernels using only the
explicit `x` and `z` strides for every stored slot. The target expansion
synthesizes the two hidden directions algebraically, and all physical
target-`d=4` loops remain tensor-only. Shift advection covers all 18 slots as
`beta^x d_x U + beta^z d_z U`. The provisional policy fills three radial
ghost layers with five-point background-subtracted extrapolation; after the
interior RHS the outer valid surface receives the all-component
GP-subtracted backward outgoing RHS. Hidden-aware cleanup follows every RK
update.

The same trace finds the decisive omission. Locked
`CCZ4RHS::compute` calls `rhs_equation` and then
`FourthOrderDerivatives::add_dissipation`, but the live adapter calls
`rhs_equation` directly. Its base is constructed with `sigma=0`, and the
project parameter class does not inherit `SimulationParametersBase`, load
`sigma`, or expose a dissipation owner. Thus no KO term reaches any of the 18
RHS rows in either grid direction. The upstream seven-point operator itself
has the expected negative Nyquist sign, `1/dx` scaling, and component-wide
mapping; it is inactive, and its generic `FOR` stride loop is unsafe for
hidden directions on this `CH_SPACEDIM=2` grid. D10 consequently obeys the
defect stop: zero tangent configurations and zero evolutions, no production
change, and classification `DISSIPATION_PATH_DEFECT_IDENTIFIED`. A future
separately authorized repair must establish a grid-dimension-safe
dissipation owner before tangent-mode identification resumes.

D11 completes that separately authorized repair without changing locked
GRChombo/Chombo. `BlackStringKODissipation` owns the upstream-equivalent
seven-point normalization and damping sign, but its fixed loop covers the 18
project slots and its direction set is exactly the two Chombo grid strides
`x,z`. The addition is fused into the existing RHS cell compute, between the
fixed lapse source and outer valid-surface override. Required project
parameter `ko_sigma` is finite and nonnegative; real evolution files set it
explicitly. The reference `0.3` is sourced from the locked KerrBH cheap and
BinaryBH very-cheap example parameters. Algebraic and live zero-sigma gates
pass, and positive-sigma GP controls through `t=8` at `dx=1/8,1/12` remain
valid, avoid drift `0.1`, control radial Nyquist content, and improve on
refinement. The repair is classified
`KO_PATH_RESTORED_AND_BACKGROUND_STABILIZED`. This clears the KO ownership
item only; physical radial-boundary acceptance, renewed perturbation work,
sustained evolution, AMR/MPI, and horizons remain open.

D12 leaves that production repair untouched and adds only fixture-side
signed Fourier analysis. Ten bounded evolutions at two resolutions confirm
that the KO-stabilized GP controls remain valid through `t=8`. The signed
`q`, `hww/chi`, `K`, `Aww`, and `GammaX` responses are linear by odd/even
subtraction and far above control leakage, but they do not share a credible
late slope: the apparent positive `k=pi/4` `q` interval is not reproduced by
the other physical fields, and `k=pi/2` still turns over. D12 therefore
records `NO_CREDIBLE_MODE_PLATEAU`, with no accepted radial eigenfunction
and no critical-wavenumber claim. The physical radial boundary and sustained
mode-identification items remain open.

D13 also leaves the production path untouched. Its fixture-only centered
tangent action executes the actual periodic/radial ghost lifecycle, direct
target-`d=4` RHS, live gauge, fixed source, KO, outer override, RK updates,
and cleanup, then projects back to one Fourier parity sector. Across three
seeds per requested wavenumber, local self-overlap is not confirmed by
independent-seed convergence. Epsilon-halving agrees, but cross-seed profiles,
raw/field-scaled rates, component amplification, and normalized constraints
fail the physical-mode gate. The Fourier scan therefore remains suspended
under `NONNORMAL_OR_NO_CONVERGED_MODE`.

D14 keeps the same production lock and replaces seed iteration with complete
fixture-only reduced matrices. One cosine/sine coefficient per radial
cell/slot produces `n=576` for each requested wavenumber. Field scaling is a
similarity transform, and comparison with the unscaled diagonalization
confirms eigenvalue identity. Twelve epsilon-halving columns, Fourier
roundtrip, dense-versus-direct action, finite-entry, residual, parity, and
harmonic gates pass. The `k=pi/4` spectral leader is a real
`Re(Omega)=0.0486917` mode, but it peaks at the first valid cell with boundary
fraction `0.820` and normalized constraint norm `0.501`. Correctly
transforming LAPACK's left vector by `D^{-1}` changes its physical-coordinate
conditioning proxy from the superseded scaled-coordinate `1522.43` to
`308.63`; the `k=pi/2` leader changes from `12.03` to `4.56`. The latter
leader and neighbors are unresolved near neutral, clustered, and constraint
heavy. Their corrected proxies do not establish severe leader
nonorthogonality, so conditioning alone no longer explains D13's local
alignments. No candidate passes the
preliminary physical filter, the optional fine matrix is skipped, and the
classification is `CONSTRAINT_GAUGE_OR_BOUNDARY_SPECTRUM_DOMINATES`.

| Priority / order | Adaptation item | GRChombo source to reuse | Project-specific work | Dependency | Acceptance / exit criterion |
|---|---|---|---|---|---|
| P0-1 | Reproducible GRChombo/Chombo core lock | Current origin, locked CI, Chombo Make infrastructure | Keep the tracked GRChombo commit and qualified official Chombo commit; disclose that historical SHA/container provenance is unresolved; keep PETSc separate until AHFinder | None | Project lock is detached/clean; four serial DIM2 libraries, real `2/4/4` target probe, and stock compile/smoke checks pass |
| P0-2 | Convention and slot adapter | `CCZ4Vars.hpp`, `ADMConformalVars.hpp`, `UserVariables.inc.hpp`, Tensor/VarsTools | Explicit `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4`; reviewed 18-slot black-string map with no visible-`y` slots and one multiplicity-two hidden representative | P0-1 source verification | Level 1 macro/slot/name/parity/permutation tests pass exactly |
| P0-3 | Formula comparison harness | `CCZ4RHS::rhs_equation`, `CCZ4Geometry`, gauge classes | Test-only adapter accepting supplied analytic jets and emitting per-family rows | P0-1, P0-2 | First five comparison tests execute without production evolution |
| P1-4a | Cell/FArrayBox storage seam | Chombo storage plus locked GRChombo `Cell` | Thin load/store wrapper around the validated 18-slot reduced Vars and GP point initializer; no physics duplication | P0-2 | Exact round trip through real storage; no `BoxLoop` or physics path |
| P1-4b | GP BoxLoop initializer (live complete) | GRChombo initial-data `BoxLoops` pattern and project parameter parser | Thin compute class used by the isolated live application | P1-4a | Every requested point equals the existing initializer; coordinate, traversal, determinant, trace, and mutation checks pass |
| P1-5 | Modified-cartoon target-input pointwise production path (complete) | Direct locked target-`d=4` `CCZ4RHS::rhs_equation` and `CCZ4Geometry`; no BoxLoop | Expand the reviewed reduced state/jets to `(x,z,w1,w2)` and call locked source; do not independently rebuild hidden CCZ4 families | P0-3, P1-4b | Full and hidden-suppressed evaluations execute; real target-input mutations are rejected |
| P1-6 | Complete pointwise 13-row equivalence (complete) | Full target GRChombo RHS, target hidden-suppressed comparison, and custom oracle | Report `target_shared_hidden_suppressed`, subtraction-defined `hidden_increment_decomposition`, and `target_full_grchombo` | P1-5 | Direct nonlinear comparison passes every physical row and is the sole numerical completion gate; the JVP sweep is only a roundoff/cancellation diagnostic; genuine `P_+`/`P_-` checks pass |
| P1-7 | Hidden-aware algebraic cleanup and constraints (pointwise complete) | Direct locked `CCZ4Geometry::compute_ricci`, exact `Constraints.impl.hpp` source convention, visible `TraceARemoval` comparison, accepted target expansion | Extend determinant/A-trace cleanup and exact `R+3K^2/4-A_IJ A^IJ` Hamiltonian/two visible momentum constraints with multiplicity two | P1-6 | Non-trace-free, curved, hidden, off-diagonal, mixed, and true sector data match the independent long-double oracle; active production/reduction mutations fail |
| P1-8 | Fixed GP-holding lapse source (pointwise complete) | Direct locked `MovingPunctureGauge` | Add field-independent `S_alpha=3 sqrt(r0/x^3)` after raw gauge evaluation | P1-7 | Raw lapse is `-3 lambda`, source-adjusted GP lapse vanishes, shift/B are untouched, and the source has zero evolved-field derivative |
| P1-9 | Compact periodic `z` production domain (E1 complete) | GRChombo periodic boundary/domain parameters, `LevelData::exchange`, and derivative classes | Lock direction 0 radial/direction 1 compact; use real ghost ownership with no translation sign flip | P1-4b | Both seam wraps, multi-box exchange, scalar/one-`z` fourth-order convergence, and nonperiodic radial ghosts pass |
| P1-9a | `2/4/4` GRAMR grid-dimension adapter (define complete) | `SetupFunctions`, `GRAMRLevel`, `BoundaryConditions`, and Chombo `ProblemDomain`/grid types | Scope grid loops to `CH_SPACEDIM` only in black-string infrastructure; forbid fake hidden coordinates and generic bulk derivative paths | P1-9 | Real `GRAMRLevel::define` succeeds under checked access; radial/periodic ownership is exact; stock DIM3 is unchanged; dependencies stay clean |
| P1-10 | Unperturbed background evolution (E2 bounded diagnostic and matched-domain convergence complete; D11 KO path restored and bounded background stabilized) | `GRAMR`, RK4, ghost fill, boundaries, checkpointing, and grid-safe KO kernels | Configure target grid, source, hidden RHS, diagnostics, conservative validation window, and explicit dissipation ownership | P1-7 through P1-9a | D11's reviewed `CH_SPACEDIM` KO repair passes algebraic/live gates and the focused `t=8` controls; sustained qualification still requires an accepted physical radial boundary and restart smoke |
| P2-11 | Fourier perturbation initialization (first level-zero fixture complete; production family open) | Initial-data BoxLoop plus periodic grid | Test-only normalized compact even/odd SO(3)-scalar seed with the one-z slots on sine | P1-10 | First parity leakage and bounded linear-amplitude run pass; production parameter family remains open |
| P2-12 | Fourier amplitude diagnostics (first level-zero fixture complete; AMR output open) | Level-zero storage and reductions | Test-only cosine/sine radial-RMS quadratures of `0.5 log(hww/chi)`, phase-neutral amplitude, and paired controls at cadence eight | P2-11 | Quadrature rotation invariance, leakage, and two-epsilon normalized histories pass; AMR-consistent persistent output remains open |
| P2-13 | Growth-rate extraction (transient diagnosis complete; physical rate open) | Project fixture analysis | Signed odd/even response, rolling width-`0.5/1.0` log slopes, linearity, constraint, and budgeted transient reporting | P2-12 | `k=pi/4` has a linear late-time instability whose physical identity is unresolved; exploding constraints/control drift forbid a GL identification, D9 shows deeper excision is insufficient, and D10 identifies missing live KO dissipation before modal isolation; no negative plateau, bracket, asymptotic rate, or physical-rate acceptance follows |
| P2-13a | Matrix-free KO-stabilized mode extraction (bounded D13 probe complete; accepted mode open) | Actual level-zero one-step map plus fixture-only signed projection | Three independent parity-compatible seeds, two epsilon values, raw/scaled norms, profiles, component fractions, and linearized constraints at `k=pi/4,pi/2` | P2-13 and D11 | No independent seed pair converges to one profile/rate; `NONNORMAL_OR_NO_CONVERGED_MODE`, with no GL identity or threshold inference |
| P2-13b | Reduced KO-stabilized Fourier spectrum (bounded D14 extraction complete; accepted mode open) | Existing Chombo-linked LAPACK `dgeev` plus D13 tangent action | Two fixture-only `576x576` field-scaled similarity transforms; complete complex spectra, physical left/right residuals and conditioning proxies, profiles, all-field fractions, constraints, boundary and Nyquist ranks | P2-13a | Boundary/constraint spectral radius and an unresolved clustered block reject the candidates; corrected conditioning does not alone explain D13; `CONSTRAINT_GAUGE_OR_BOUNDARY_SPECTRUM_DOMINATES`, with no physical-mode or threshold inference |
| P2-13c | `k=pi/4` reduced-spectrum sector isolation (bounded D15 complete; source and accepted mode open) | D14 tangent/LAPACK fixture plus existing frozen-gauge and exact-GP ghost seams | Reused D14 baseline hash, native `416x416` frozen-gauge operator, and `576x576` exact-GP-radial-ghost operator retaining the provisional outer RHS override; artifact-first ranked records and independently parsed profile matching | P2-13b | Both corrected variants remain boundary/constraint heavy and contain no individually bulk-eligible candidate; `NO_BULK_PHYSICAL_CANDIDATE_AFTER_SECTOR_ISOLATION`, with no sole-source, GL-mode, or threshold inference |
| P2-13d | Constraint-aware `k=pi/4` reduced spectrum (bounded D16 complete; invariant constrained dynamics open) | D14/D15 tangent matrix plus existing live constraint and conformal-algebra evaluators and Chombo-linked LAPACK `dgesvd` | One `576x576` map and all-cell/interior signed constraint maps; orthonormal SVD nullspaces, invariance leakage, projected spectra, and direct full-map residuals | P2-13c | Both nullspaces are algebraically accurate but dynamically non-invariant (`eta=7.01e-2,5.68e-2`), and projected leaders fail direct residual gates; `CONSTRAINT_NULLSPACE_NOT_INVARIANT`, with no projected-rate, GL-mode, or threshold inference |
| P2-14 (historical label; Milestone 5) | Production convergence workflow | GRChombo AMR/restart/output machinery | Reproducible multi-resolution parameter sets and comparison tables | P1-10, P2-13 | Background, constraints, perturbations, and rate show documented convergence |
| P3-15 (historical label; Milestone 4) | String MOTS/horizon adapter | `AHFinder`, `PETScAHSolver`, `AHStringGeometry`, interpolation | Supply target variables, `S2 x S1` geometry, hidden expansion terms, PETSc configuration | P1-7, PETSc source lock | Uniform `x=r0` MOTS recovered with convergent residual; restart supported |
| P3-16 (historical label; Milestone 4) | `R_H`, minimum radius, and horizon area | AH surface data, interpolation, reductions, `SmallDataIO` | Evaluate `R_H=h sqrt(hww/chi)`, minimum over z, correct string area | P3-15 | Uniform analytic values and perturbed manufactured profiles converge |
| P3-17 (historical backlog ID, not D17; Milestones 4–5) | Nonlinear GL production workflow | All mature GRChombo runtime infrastructure | Parameter families, perturbation sweep, constraints/horizon monitoring, failure criteria, archival metadata | P2-14, P3-16 | Reviewed end-to-end runbook; unperturbed and small-perturbation gates pass first |
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
  multiplicity two in physical traces and contractions. E1 now uses this
  contract live; the historical 27-slot shape is rejected.
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
- Historical note: before E1, the old 27-slot `UserVariables.hpp` remained
  the live smoke/comparison scaffold. E1 supersedes that state with the
  isolated 18-slot application and leaves the old shape only as a negative
  control.
- The target `2/4/4` macro assertions compile with inspected GRChombo
  `DimensionDefinitions.hpp` and `Tensor.hpp`. Unmodified stock
  `ADMConformalVars` and `VarsTools` cannot map the target state: their
  CH-dimension-selected three-component symmetric interval conflicts with the
  ten components required by `DEFAULT_TENSOR_DIM=4`, and two-component vector
  intervals conflict with four-component default tensors. The future
  black-string Vars/mapping adapter must own this translation.
- P1-4 GP initialization subsequently consumed the reviewed contract through
  the dedicated adapter, and E1 assembled the remaining live seam without
  reusing the old 27-slot scaffold.

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
  The reduced-to-full target-`d=4` input seam, direct locked GRChombo
  pointwise RHS, and complete nonlinear 13-row equivalence are now complete.
  Live initializer/RHS wiring remains deferred.
- Pointwise hidden-aware cleanup, constraints, and the fixed lapse source are
  complete. Live BoxLoop wiring, periodic ownership, evolution, and
  diagnostics remain later backlog items.

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
  complete. The direct target-`d=4` pointwise RHS and 13-row equivalence are
  also complete. Pointwise hidden-aware cleanup/constraints and the fixed
  lapse source are complete. The next authorized substage is live
  BoxLoop RHS/cleanup/source wiring plus periodic ownership; evolution and
  diagnostics remain open.

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
- The formerly future live call site is implemented in
  `BlackStringToyLevel::initialData()` through
  `BoxLoops::loop(BlackStringGPInitialData::make_compute(...))`. E1 validates
  the live application seam; E2 validates bounded serial level-zero
  evolution and matched-domain convergence. The exact GP radial fill remains
  diagnostic-only. A separate provisional GP-subtracted physical boundary now
  passes the first perturbative smoke; sustained and boundary-systematic
  qualification remain next.

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

## Target pointwise hidden/cartoon RHS result

- P1-5 and P1-6 are complete at the pointwise seam. The adapter consumes the
  validated reduced 18-slot state, supplied gridded first/second jets,
  lapse/shift jets, `x>0`, and the locked CCZ4 tuple.
- Target indices are `0=x`, `1=z`, `2=w1`, `3=w2`. One stored `hww/Aww`
  representative expands to two hidden diagonal tensor entries only inside
  target contractions; outputs retain one representative row.
- Locked `CCZ4RHS::rhs_equation`, raw Ricci, and encoded-Z routines execute
  directly on the full target expansion. The hidden-suppressed path executes
  the same locked source with its hidden-sensitive inputs suppressed. The
  reported `hidden_increment_decomposition` is defined as
  `target_full_grchombo - target_shared_hidden_suppressed`; it measures the
  hidden-sensitive increment and is not an independently implemented hidden
  Ricci, encoded-Z, shift, Gamma, or coefficient-correction RHS.
- Exact GP totals are roundoff zero with stored `hww=1`. Diagonal,
  off-diagonal, hidden-`ww`, encoded-Z, and mixed finite states pass direct
  nonlinear comparison for all 13 rows under the locked tolerance. A declared
  epsilon-sweep JVP is retained only as a roundoff/cancellation-dominated
  secondary diagnostic and is neither convergence nor completion evidence.
  Fourier-consistent
  `P_+` and `P_-` jets pass phase, reflection-commutator, forbidden-leakage,
  and nonzero-allowed-output checks.
- Expansion/derivative test policies mutate the actual input supplied to
  GRChombo. Hidden multiplicity, representative `1/x^2`, `hww=x^2`, omitted
  or duplicated cartoon input, encoded-Z/Gamma, representative evolution, and
  target-coefficient mutations are rejected. Output representative doubling
  is labeled only as a reporting-layer mutation.
- An independently coded hidden-family production RHS remains deliberately
  absent. The custom analytic solver remains the independent complete-row
  oracle.
- The old one-pass Christoffel construction and old shift-Hessian index order
  are retained only as test policies. Each fails on active finite data while
  the corrected oracle passes the same direct-GRChombo comparison.
- Pointwise P1-7/P1-8 are complete. Cleanup uses
  `det(h)=hww^2(hxx*hzz-hxz^2)` and the target one-quarter weighted trace
  projection. Constraints call locked `CCZ4Geometry::compute_ricci` directly
  and use `H=R+3K^2/4-A_IJ A^IJ` without reconstructing `K_IJ`; the remaining
  formula layer is classified as source/convention implementation. Constraint
  outputs are `target_hidden_suppressed`,
  `hidden_sensitive_increment`, and `target_total`; the increment is defined
  by subtraction. The complete target result matches the independent
  long-double analytic path for `H,Mx,Mz`, including genuine reflection and
  forbidden-sector tests for both Fourier sectors.
- No live application or BoxLoop RHS/cleanup/source path changed. Periodic
  ghosts, evolution, diagnostics, and horizons remain absent. The exact next
  active substage is live BoxLoop RHS/cleanup/source wiring plus periodic-`z`
  ownership.

## Stage 4AO-D-E1 live-integration supersession

The historical pointwise-only status immediately above is superseded by the
isolated E1 application integration:

- live GP initialization, live stencil-driven target-`d=4` RHS, post-update
  cleanup, `H,Mx,Mz`, fixed lapse source, and periodic-z ownership are
  complete;
- exact live comparisons cover 72 initialized cells and 9 RHS cells across
  all 18 slots with zero measured mismatch;
- manufactured `N=8,16,32,64` fixtures pass fourth-order gates for Ricci,
  encoded Z, advection, shift terms, lapse derivatives, combined rows, and
  constraints;
- GP `N=32,64,128,256` fixtures report all 18 rows and three constraints;
  the maximum RHS residual decreases from `1.032981011468426e-4` to
  `7.543271873799995e-8`, and the fixed-source lapse residual is zero;
- low and high z wraps are checked separately; real production first-,
  second-, and mixed-derivative stencils cross both sides of the internal
  two-box seam at four resolutions with fourth-order convergence, no one-z
  sign flip, and independently owned radial ghosts;
- manufactured tables retain worst row/component, `IntVect`, physical
  `(x,z)`, and parity for every family and resolution;
- live mutation coverage means only executed pre-output registration,
  storage, source, and update-hook policy mutations. Cleanup remains
  determinant `0.9999999999999997` and weighted trace
  `-5.204170427930421e-18`.

Bounded serial level-zero GP evolution and matched-domain convergence are now
implemented and validated. The minimal physical radial-boundary smoke and the
bounded Fourier transient diagnostic are also implemented. The former
short-window stable/unstable interpretation is superseded.
Sustained evolution, physical radial-boundary acceptance, a converged
`k_cr_0`, AMR/MPI, broader diagnostics, horizons/PETSc, and final scoring
remain open under Milestones 1–5. The historical Stage 4AO-D investigation is
closed inconclusive; G-Engineering passes, while G-Physics and overall
Checkpoint G remain blocked.
Dimension-safe
`AMR::define -> GRAMRLevel::define -> BoundaryConditions::define` is
implemented and validated, and the former
`CH_SPACEDIM=2` `RealVect m_center[i=2]` over-index blocker is cleared.
The provisional radial policy now passes the small perturbative smoke and a
short two-boundary Fourier sign matrix, but it is not physically accepted.
Sustained evolution, converged growth/threshold extraction, AMR refinement,
MPI, broader diagnostics, and AHFinder remain incomplete. E1 stays the
application-seam gate; E2 is the bounded unperturbed level-zero evolution
gate.
