# GRChombo Production-Adaptation Preflight

Status: dependency source/contract audit, isolated production variable
contract, reduced Vars seam, pointwise GP initializer, real storage seam, and
isolated real DIM2 GP `BoxLoop` initializer implemented. Locked
GRChombo and the official Chombo project dependency are verified. The Chombo
lock is `PROJECT_QUALIFIED`, not historically exact; the real target `2/4/4`
header probe passes. Former-container and PETSc/AHFinder provenance remain
unresolved. The contract is not imported by the live 27-slot smoke
application. No live GP initial-data call, hidden RHS, cleanup, constraint,
gauge-source, periodic grid, diagnostic, or evolution path is implemented.

GRChombo is the production framework and convention authority. The frozen
custom physical-`d=4` implementation remains an independent pointwise oracle.
The custom stationary outer-boundary problem remains deferred research and is
not on the production path.

## Dependency lock and provenance boundary

The authoritative inspected source tuple is:

| Field | Locked or observed value | Status |
|---|---|---|
| Remote | `https://github.com/GRTLCollaboration/GRChombo.git` | verified |
| Commit | `37e659523830418b210acea1661dac0e00bb1b75` | verified |
| Checkout state | detached `HEAD`, clean tracked and untracked status | verified |
| Inspection compiler | GNU C++ `15.2.0` | environment observation, not a portable toolchain lock |
| Comparison compile contract | C++17, `-O2 -Wall -Wextra -Wpedantic -Werror` | verified for focused comparison fixtures |
| Direct-comparison dimensions | `CH_SPACEDIM=3`, `GR_SPACEDIM=3`, `DEFAULT_TENSOR_DIM=3` | verified comparison configuration |
| Target production dimensions | `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4` | real header/storage-boundary compile and run verified |
| Chombo repository | `https://github.com/GRChombo/Chombo.git` | verified from locked CI |
| Chombo layout | checkout root with `CHOMBO_HOME=<root>/lib`; `mk/Make.test` | verified from locked CI/GNUmakefiles |
| Historical Chombo revision | CI checkout omits `ref`; public logs/artifacts expose no SHA | unavailable; timestamp inference only |
| Project Chombo lock | `8684f2e000106f1abadb72642e1d15351867f98f`, patch set `none` | official candidate qualified by libraries, target probe, and stock tests |
| PETSc role | optional for core/GP wrapper; required by `USE_AHFINDER` | verified |
| PETSc source/build tuple | Ubuntu CI uses unpinned `petsc-dev` and `pkg-config petsc` | unresolved |
| Docker image | historical name `grchombo/grchombo`; digest unavailable | unresolved |
| Container recipe digest | unavailable | unresolved |

The machine-readable dependency lock is
`run_manifests/grchombo_dependency_lock.yaml`. The read-only verifier is
`scripts/verify_grchombo_dependency.sh`. It fails on a wrong origin, wrong
commit, attached branch, or dirty checkout. When Chombo is present it also
checks its fork, cleanliness, real `parstream.H`, `mk/Make.test`, and exact
revision if pinned. Metadata-only, target-probe, candidate-recovery,
PETSc-requested, and full-build modes are documented in
`docs/grchombo/chombo_dependency_contract.md`.

On a new machine, obtain and verify the inspected source with:

```bash
git clone https://github.com/GRTLCollaboration/GRChombo.git external/GRChombo
git -C external/GRChombo checkout --detach 37e659523830418b210acea1661dac0e00bb1b75
git -C external/GRChombo status --short
bash scripts/verify_grchombo_dependency.sh
```

The repository intentionally ignores `external/`. This preflight therefore
uses a tracked manifest and verifier rather than silently converting the
nested checkout to a submodule. A future submodule migration, if desired,
must be a separate reviewed change after the complete
GRChombo/Chombo/PETSc/container tuple is known; its migration plan must cover
the ignore rule, `.gitmodules`, CI/bootstrap commands, and preservation of
local checkouts.

The locked workflows clone `GRChombo/Chombo` without a `ref`; consequently
they verify the fork but do not identify the historical revision. Official
public run/log/artifact/cache routes were exhausted without an exact SHA.
The default-branch head at the successful workflow timestamp,
`8684f2e000106f1abadb72642e1d15351867f98f`, was qualified as the project's
dependency: all four required serial DIM=2 libraries build, the real target
probe compiles/links/runs, and stock variable-store and CCZ4-geometry tests
pass. PETSc and usable Docker integration remain unavailable. This authorizes
the thin `Cell`/`FArrayBox` storage wrapper, but not periodic ownership,
evolution, AHFinder, or a claim of historical exactness.

## Compile-time dimension contract

`DimensionDefinitions.hpp` defaults `GR_SPACEDIM` to three and
`DEFAULT_TENSOR_DIM` to `CH_SPACEDIM`. The black-string target must therefore
set all three macros explicitly:

```text
CH_SPACEDIM=2
GR_SPACEDIM=4
DEFAULT_TENSOR_DIM=4
```

Their meanings are:

- `CH_SPACEDIM=2`: Chombo grids and derivatives own only `(x,z)`.
- `GR_SPACEDIM=4`: physical spatial traces and CCZ4 coefficients use four
  spatial dimensions.
- `DEFAULT_TENSOR_DIM=4`: tensor loops that represent physical geometry
  cannot silently truncate to the two gridded directions.

The isolated `BlackStringProductionVariables.hpp` contract and focused
fixture now compile with this exact tuple while including the inspected
GRChombo `DimensionDefinitions.hpp` and `Tensor.hpp`. A compile with
`CH_SPACEDIM=3` fails the production static assertion as required.

This does not establish that the unmodified stock CCZ4 variable mapper accepts
the target layout. It does not:

- `ADMConformalVars.hpp` maps a symmetric tensor through
  `GRInterval<c_h11,D_SELECT(,c_h22,c_h33)>`. With `CH_SPACEDIM=2`, that
  interval contains three visible components.
- `VarsTools::define_symmetric_enum_mapping` requires the interval size to be
  `DEFAULT_TENSOR_DIM*(DEFAULT_TENSOR_DIM+1)/2`, which is ten when
  `DEFAULT_TENSOR_DIM=4`.
- Stock vector intervals contain two gridded components, while stock
  `Tensor<1>` members default to four components under the target macro tuple.

Therefore a black-string Vars/mapping adapter is required before stock
`CCZ4Vars` can own this state. The direct header probe now resolves real
Chombo `parstream.H` and `FArrayBox.H`, locked GRChombo `Cell.hpp`, and the
target reduced storage boundary. It proves header/build compatibility, not
that the generic stock variable mapper accepts the reduced state. The earlier
direct GRChombo comparisons remain matched stock-`d=3` evidence.

## Target black-string state

The production black-string example will replace only its own user-variable
enumeration and names. It will not reinterpret the current stock visible
`y` component as hidden `ww`. The intended compact layout has 18 slots:

| Slot | Field | Geometric meaning | `z -> -z` parity | Storage / physical multiplicity | Initial-data owner | RHS owner | Cleanup owner | Diagnostic owner | Stock equivalent / status |
|---:|---|---|---|---:|---|---|---|---|---|
| 0 | `chi` | conformal factor | even | 1 | GP initializer | wrapped stock CCZ4 | stock positivity plus hidden determinant adapter | 13-row seam, constraints | stock `c_chi`; shared |
| 1 | `hxx` | conformal metric `h_xx` | even | 1 | GP initializer | wrapped stock visible metric RHS | hidden-aware determinant normalization | 13-row seam | stock `h[0][0]`; shared |
| 2 | `hxz` | conformal metric `h_xz` | odd | 1 | GP initializer | wrapped stock visible metric RHS | hidden-aware determinant normalization | parity and 13-row seam | stock `h[0][1]` in the target 2-grid layout; shared |
| 3 | `hzz` | conformal metric `h_zz` | even | 1 | GP initializer | wrapped stock visible metric RHS | hidden-aware determinant normalization | 13-row seam | stock `h[1][1]` in the target 2-grid layout; shared |
| 4 | `hww` | one stored representative for each of two equivalent hidden directions | even | 1 / 2 | GP initializer | hidden/cartoon adapter | hidden-aware determinant normalization | hidden-family report and 13-row seam | no stock slot; adapt |
| 5 | `K` | physical four-dimensional trace of `K_IJ` | even | 1 | GP initializer | wrapped stock scalar RHS with hidden completion | none | 13-row seam, constraints | stock `K` name; dimension-adapted |
| 6 | `Axx` | conformal trace-free `A_xx` | even | 1 | GP initializer | wrapped stock visible tensor RHS | hidden-aware trace removal | 13-row seam | stock visible component; shared |
| 7 | `Axz` | conformal trace-free `A_xz` | odd | 1 | GP initializer | wrapped stock visible tensor RHS | hidden-aware trace removal | parity and 13-row seam | stock visible component in target grid; shared |
| 8 | `Azz` | conformal trace-free `A_zz` | even | 1 | GP initializer | wrapped stock visible tensor RHS | hidden-aware trace removal | 13-row seam | stock visible component in target grid; shared |
| 9 | `Aww` | one stored trace-free representative for two equivalent hidden directions | even | 1 / 2 | GP initializer | hidden/cartoon adapter | hidden-aware trace removal | hidden-family report and 13-row seam | no stock slot; adapt |
| 10 | `Theta` | CCZ4 scalar constraint variable | even | 1 | GP initializer | wrapped stock CCZ4 with hidden completion | none | 13-row seam, constraints | stock `Theta`; shared |
| 11 | `GammaX` | hatted conformal connection `hat_Gamma^x` | even | 1 | GP initializer | wrapped visible plus hidden/cartoon connection RHS | none | 13-row seam, constraints | stock `Gamma[0]`; shared with hidden completion |
| 12 | `GammaZ` | hatted conformal connection `hat_Gamma^z` | odd | 1 | GP initializer | wrapped visible plus hidden/cartoon connection RHS | none | parity and 13-row seam | stock `Gamma[1]` in target grid; shared with hidden completion |
| 13 | `lapse` | `alpha` | even | 1 | GP initializer | stock moving-puncture gauge plus future lapse-only source adapter | stock positivity | gauge/background diagnostics | stock lapse; reuse/wrap |
| 14 | `shiftX` | `beta^x` | even | 1 | GP initializer | stock moving-puncture gauge | none | gauge/background diagnostics | stock shift component; reuse |
| 15 | `shiftZ` | `beta^z` | odd | 1 | GP initializer | stock moving-puncture gauge | none | parity/gauge diagnostics | stock shift component in target grid; reuse |
| 16 | `Bx` | moving-puncture auxiliary `B^x` | even | 1 | GP initializer | stock moving-puncture gauge | none | gauge diagnostics | stock `B[0]`; reuse |
| 17 | `Bz` | moving-puncture auxiliary `B^z` | odd | 1 | GP initializer | stock moving-puncture gauge | none | parity/gauge diagnostics | stock `B[1]` in target grid; reuse |

Slots for visible `y`, `h_xy`, `h_yy`, `h_yz`, `A_xy`, `A_yy`, `A_yz`,
`Gamma^y`, `beta^y`, and `B^y` are deliberately absent. The two equivalent
hidden directions are represented once and enter physical traces,
determinants, contractions, and constraints with multiplicity two.

The current 27-slot `BlackStringToy/UserVariables.hpp` remains comparison and
smoke scaffolding. It contains stock-visible-`y` slots and must not be cited
as the approved production layout. The isolated production source of truth is
`code/BlackStringToy/BlackStringProductionVariables.hpp`; importing it into a
live application remains a later integration change.

The GP storage convention is locked as

```text
gamma_theta_theta = x^2
gamma_ww = hww / chi
hww = 1                 on the GP background
```

Here `gamma_theta_theta` is the angular coordinate component, whereas
`gamma_ww` is the normalized hidden Cartesian representative. They are not
the same stored quantity.

## Minimum GRChombo adaptation points

| Adaptation point | Decision | Production ownership boundary |
|---|---|---|
| User-variable enumeration | replace only for black-string variables | introduce the reviewed 18-slot enum; do not modify generic GRChombo enums |
| Variable registration and names | replace only for black-string variables | register exactly the 18 names and parity metadata in the example |
| GP initial-data `BoxLoop` | extend | isolated black-string compute class and real traversal complete; live call remains deferred |
| CCZ4 RHS | wrap | pointwise target adapter complete: expand reduced inputs, directly call locked full target-`d=4` GRChombo, execute a hidden-suppressed comparison, and report their subtraction; live BoxLoop wiring deferred |
| Modified-cartoon derivatives and geometry | extend | use GRChombo derivative/ghost infrastructure; add only hidden representative formulas and axis policy |
| Algebraic cleanup | wrap | reuse positivity; extend determinant and `A`-trace operations for `hww/Aww` with multiplicity two |
| Constraints | wrap | retain stock orchestration/reductions; add physical-`d=4` hidden terms |
| Fixed lapse source | wrap | later add the validated field-independent lapse-only source after raw stock gauge evaluation |
| Periodic `z` ownership | reuse unchanged | Chombo domain/ghost exchange owns periodicity; project code supplies configuration only |
| Diagnostics and horizon data | reuse and extend | reuse reductions, interpolation, I/O, and AH machinery; add black-string `R_H`, minimum radius, area, and Fourier data |
| RK, AMR, MPI/OpenMP, checkpoint/restart | reuse unchanged | no project-owned replacement |

Thin adapters must preserve separately observable target hidden-suppressed and
full target evaluations. Their subtraction is decomposition evidence only.
Copying the complete custom solver or independently rebuilding complete hidden
CCZ4 families in production is explicitly rejected.

## Oracle-to-production comparison seam

The implemented pointwise production acceptance adapter has the following
contract:

- Input: `(x,z,r0)`, the 18 target values, analytic first and second
  derivatives in the two gridded directions, the modified-cartoon
  regularity/hidden derivative data, lapse/shift derivatives, and explicit
  formulation parameters.
- Locked formulation: CCZ4, physical `d=4`, gridded `d=2`, hidden
  multiplicity two, vacuum, `Lambda=0`, `kappa1=0.1`, `kappa2=0`,
  `kappa3=1`, and `covariantZ4=true`.
- Output: all 13 physical RHS rows in the frozen order
  `(chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,GammaX,GammaZ)`.
- Reporting: `target_shared_hidden_suppressed`,
  `hidden_increment_decomposition`, and `target_full_grchombo` are recorded
  separately. The increment is exactly full minus suppressed; it exposes
  cancellation size without claiming an independently coded hidden family.
- Reference: the independent
  `code/BlackStringToy/tests/Stage4AOCAnalyticFullOracle.hpp` path and its
  complete analytic full-oracle fixture. Production code must not be called
  by the oracle.
- Acceptance:
  `|a-b| <= 5e-13 + 5e-12 max(|a|,|b|)` for pointwise comparisons, plus
  component ownership, hidden-multiplicity, genuine Fourier-sector, and
  target-input mutation checks.

Both target paths execute locked GRChombo source directly. The independent
custom oracle evaluates the same finite nonlinear state/jets without calling
the production adapter. The direct nonlinear comparison is the sole 13-row
numerical completion gate. The JVP epsilon sweep is a
roundoff/cancellation-dominated secondary diagnostic only; it is not
convergence evidence.

## Locked production sequence and audit checkpoints

The source locks, 18-slot contract, exact metadata, reduced Vars seam, GP point
initializer, analytic GP derivative metadata, real one-point
`Cell`/`FArrayBox` seam, and GP storage round trip are already complete. The
real DIM2 GP `BoxLoop` compute/traversal and coordinate contract are also
complete. The active implementation sequence is now:

1. **Cell/FArrayBox storage seam (complete).** The thin wrapper around the
   validated 18-slot reduced Vars duplicates no physics.
2. **GP BoxLoop initializer (compute/traversal complete).** Live application
   wiring remains deferred.
3. **Target-input and RHS adaptation (pointwise complete).** Expand the
   reduced `(2+2)` value/derivative data to full target `d=4` and call locked
   GRChombo directly. No complete hidden family is independently reimplemented.
4. **Pointwise complete 13-row equivalence (complete).** Report target
   hidden-suppressed, subtraction-defined hidden increment, and full target
   values; compare the full target result directly with the custom oracle.
5. **Hidden-aware cleanup and constraints (next).**
6. **Production fixed lapse-source hook.**
7. **Periodic `z` and ghost ownership.**
8. **Unperturbed GP evolution.** Require background stationarity, constraint
   convergence, and gauge-source validation.
9. **Perturbed Fourier-mode evolution.** Extract growth/decay and the first GL
   threshold estimate.
10. **Horizon and nonlinear diagnostics.** Begin only after PETSc/AHFinder and
    observable conventions are qualified.

Avoid per-substep audits. Require substantive independent audits only after:

- the first real `Cell`/`FArrayBox` plus `BoxLoop` GP initializer is
  assembled;
- the direct full target-`d=4` 13-row nonlinear comparison passes;
- hidden cleanup/constraints and the fixed source are integrated;
- the first unperturbed production evolution passes; and
- the first perturbed GL growth-rate run passes.

Fold documentation consistency into those technical audits. The exact next
implementation substage is item 5 only; it excludes live RHS wiring, source,
periodicity, evolution, diagnostics, and AHFinder.

## Enumeration/registration substage result

The first implementation substage passes:

- the dependency verifier accepts the clean detached locked checkout;
- exactly 18 contiguous, unique slots are defined in the reviewed order;
- one registration array owns variable, checkpoint, and output names;
- metadata records physical/gauge category, scalar/one-z/gauge parity,
  storage and hidden multiplicity, stock overlap, and future owners;
- the split is 13 physical plus five gauge fields;
- `hxz`, `Axz`, and `GammaZ` are exactly the physical one-z fields;
- `shiftZ` and `Bz` retain one-z gauge parity;
- all visible-y names are forbidden;
- `hww/Aww` have storage multiplicity one and physical multiplicity two;
- `gamma_theta_theta=x^2`, `gamma_ww=hww/chi`, and stored GP `hww=1` pass;
- swapped, duplicate, omitted, visible-y, `hww=x^2`, multiplicity-one,
  added-field, registration-order, and wrong-dimension mutations fail.

Only the contract is complete. The old 27-slot scaffold, GP initialization,
all equation and cleanup owners, periodic grid, diagnostics, and evolution
remain unchanged.

## Reduced Vars and exact GP pointwise substage

The dedicated reduced seam is implemented in
`code/BlackStringToy/BlackStringReducedVars.hpp`. It defines:

- `ReducedSymmetricTensor<T> = {xx,xz,zz,ww}`;
- `ReducedVisibleVector<T> = {x,z}`;
- `PhysicalVariables<T>` for
  `chi,h,K,A,Theta,hat_Gamma`;
- `GaugeVariables<T>` for `lapse,shift,B`;
- `Variables<T>` grouping the physical and gauge structures;
- `Storage<T> = std::array<T,18>`;
- explicit const/mutable component access and exact load/store operations.

All slot ownership comes from `BlackStringProductionVariables.hpp`; the enum
is not copied. Storage writes one `ww` representative. The optional physical
diagonal contraction counts that representative twice. There is no `y` or
`yy` member and the legacy 27-slot header is not imported.

`BlackStringCellStorage.hpp` now loads and stores the 18 slots between this
structure and a real `Cell<double>`-compatible source. Future production
adapters will expose visible `(x,z)` data to stock formula families and add
black-string hidden families. For the black-string application the reduced
Vars seam replaces
`ADMConformalVars::enum_mapping`, `BSSNVars::enum_mapping`,
`CCZ4Vars::enum_mapping`, and their
`VarsTools::define_*_enum_mapping` calls. It does not replace Chombo storage,
`BoxLoops`, derivatives, time integration, or AMR.

The exact point initializer and analytic setup metadata are implemented in
`code/BlackStringToy/BlackStringGPPointwiseInitialData.hpp`. For finite
`r0>0,x>0`, it returns:

```text
chi=1
h={xx=1,xz=0,zz=1,ww=1}
K=3 lambda/2
A={xx=-7 lambda/8,xz=0,zz=-3 lambda/8,ww=5 lambda/8}
Theta=0
hat_Gamma={x=0,z=0}
lapse=1
shift={x=sqrt(r0/x),z=0}
B={x=0,z=0}
lambda=sqrt(r0/x^3)
```

Nonfinite, nonpositive-`r0`, and nonpositive-`x` inputs are rejected. The
initializer stores `hww=1`; `gamma_theta_theta=x^2` is separate from
`gamma_ww=hww/chi=1`.

The analytic metadata supplies value, first radial derivative, and second
radial derivative for `beta^x`, `lambda`, `K`, and every reduced `A`
component. It uses

```text
beta_x'   = -beta_x/(2x)
beta_x''  = 3 beta_x/(4x^2)
lambda'   = -3 lambda/(2x)
lambda''  = 15 lambda/(4x^2)
```

and constant multiples for `K,Axx,Azz,Aww`; the `Axz` jet is identically
zero. This is analytic metadata only and performs no grid differentiation.

The focused fixtures pass exact 18-slot round trips, component-local mutable
writes, representative storage, 13+5 grouping/parity, three GP sample points,
determinant one, weighted `A` trace zero, physical `K_IJ` reconstruction,
gauge ownership, analytic derivative convergence, and roundoff turnover.
They reject swapped mappings, visible-y aliases, hidden multiplicity one,
`hww=x^2`, wrong `K` sign, wrong `Aww`, missing gauge initialization, and
field- or horizon-dependent initializer mutations.

The reduced Vars seam, pointwise GP contract, analytic GP metadata, real
one-point Chombo storage seam, GP storage round trip, and isolated real DIM2
GP `BoxLoop` traversal are complete. The pointwise target CCZ4 RHS and
hidden/cartoon geometry adapter are also complete. Live registration/BoxLoop
RHS wiring, cleanup/constraints, fixed lapse source, periodic ownership,
evolution, and diagnostics remain open.

## Cell/FArrayBox storage seam result

`code/BlackStringToy/BlackStringCellStorage.hpp` is intentionally thin:

- `load(const Cell<double> &)` checks exactly 18 components and loads them
  into `BlackStringReducedVars::Variables<double>`;
- `store(const Cell<double> &, const Variables<double> &)` checks exactly 18
  components and stores them in production-enum order;
- both operations iterate the production enum as their sole slot authority;
- storage never applies hidden multiplicity and writes one `hww` and one
  `Aww` representative.

The test-only real-grid access path is
`FArrayBox -> BoxPointers -> Cell<double> -> BlackStringCellStorage`.
It uses an explicit nonzero DIM2 `IntVect` and performs no grid traversal.
The focused fixture proves exact 18-slot round trips, all component-local
writes, unchanged neighbors, single-representative hidden storage, legacy
27-component rejection, and three pointwise GP storage comparisons including
`(r0,x)=(1,2)`. The following isolated GP `BoxLoop` substage reuses this
storage adapter and pointwise initializer; live application wiring remains
deferred.

## GP BoxLoop initializer result

`code/BlackStringToy/BlackStringGPInitialData.hpp` defines:

- `BlackStringGPInitialData::make_compute(r0, dx, origin)`, whose default
  policy delegates to `BlackStringCellStorage::store`;
- `compute(Cell<double>)`, compatible with real GRChombo `BoxLoops` when
  `disable_simd()` is selected;
- a test-injectable storage-policy seam that does not add a production
  counter.

Direction 0 is radial `x` and direction 1 is compact `z`. Cell centers follow
`coordinate=(index+1/2)dx-origin[direction]`. Locked GRChombo's generic
`Coordinates` constructor lacks a `CH_SPACEDIM=2`,
`DEFAULT_TENSOR_DIM=4` branch, so the target adapter reproduces that exact
GRChombo convention without owning any GP formula. Nonfinite inputs and
nonpositive radial cells are rejected.

The production compute path calls only
`BlackStringGPPointwiseInitialData::make_pointwise_vars(r0,x)` and
`BlackStringCellStorage::store(cell,vars)`. The compact coordinate is absent
from the physics call.

The focused real-Chombo fixture runs over 20 requested cells inside a
42-point allocation. It records 20 exact single visits, leaves all 22 outside
points unchanged, writes all 18 slots, and reports zero maximum absolute and
normalized oracle error. All per-point algebraic and gauge checks pass.
Coordinate, storage, component, traversal, nonpositive-radius, and legacy
shape mutations fail independently.

Storage-adapter ownership is observed rather than claimed. A test policy
wraps the real adapter and records, in shared mutex-protected state, one call
and the exact 18-slot ledger for every requested `IntVect`; no outside call is
recorded and `hww/Aww` each occur once. The direct-write mutation preserves
the complete numerical oracle but records zero adapter calls, so that guard
fails independently without a Boolean claim.

The checkpoint repair was held pending until this instrumentation and the
strict warning gate passed. Fixture/project code now uses
`-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`; only locked Chombo and
GRChombo include paths use `-isystem`. No global `-Wno-error` downgrade is
part of the project contract, and a project-owned unused-parameter negative
compile remains fatal.

The future live call site is `BlackStringToyLevel::initialData()`, using
`BoxLoops::loop(make_compute(r0,m_dx,m_p.center),...)` with SIMD disabled.
That file, its parameters, and all smoke applications remain unchanged.

## Target pointwise hidden/cartoon RHS acceptance

`BlackStringTargetCCZ4Pointwise::evaluate(const Input&)` now owns the narrow
production-style pointwise boundary. It expands one reduced hidden
representative into target indices `(x,z,w1,w2)=(0,1,2,3)`, supplies the
complete modified-cartoon derivative identities to directly compiled locked
GRChombo source routines, and returns exactly 13 physical rows.

The result separates `target_shared_hidden_suppressed`,
`hidden_increment_decomposition`, and `target_full_grchombo`. The middle
quantity is defined only by subtracting the first from the third; it is
decomposition evidence, not independent ownership of hidden Ricci, encoded
Z, shift, Gamma, or coefficient corrections. Directly isolated geometric
Ricci, encoded-Z, lapse-derivative, and damping evidence is retained.

The independent custom oracle passes direct nonlinear comparison for
diagonal, off-diagonal, pure hidden, pure encoded-Z, and mixed finite states.
The epsilon-sweep JVP is classified as roundoff/cancellation dominated and is
secondary diagnostic evidence only. Fourier-consistent `P_+` and
`P_-` perturbation jets pass reflection and forbidden-sector leakage checks at
several phases. Exact GP full-target totals vanish to roundoff with `hww=1`;
nonzero suppressed/increment cancellation legs remain visible. In the
hidden-suppressed target-`d=4` K row,
`beta^x d_x K = -9 lambda^2/4` and `K^2 = +9 lambda^2/4`, so that leg is
approximately zero; this is distinct from the older stock-`d=3` mapping with
residual `1/8`.

Test-only expansion policies alter the actual target data passed to GRChombo
and reject multiplicity, representative `1/x^2`, cartoon, encoded-Z/Gamma,
representative evolution, and coefficient defects. An independently coded
hidden-family RHS gate is intentionally absent/false.

The independent oracle also carries active regression policies for both
defects found by the direct nonlinear gate. A one-pass raised-Christoffel
construction fails on active off-diagonal data while the corrected two-pass
construction passes the same state. A shift-Hessian policy with distinct
nonzero derivative-derivative-component and former component-first entries
likewise rejects the former index order while the corrected order passes.

This acceptance changes no application, BoxLoop, cleanup, constraint, source,
periodic, evolution, diagnostic, or horizon path. Hidden-aware cleanup and
constraints are next.
