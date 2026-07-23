# GRChombo Production-Adaptation Preflight

Status: dependency-source lock and isolated production variable contract
implemented. The contract is not yet imported by the live 27-slot smoke
application. No GP initial-data `BoxLoop`, hidden RHS, cleanup, constraint,
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
| Target production dimensions | `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4` | required design, compilation/runtime still unresolved |
| Chombo source/revision | not available in the inspected checkout | unresolved |
| PETSc source/build tuple | not available | unresolved |
| Docker image | historical name `grchombo/grchombo`; digest unavailable | unresolved |
| Container recipe digest | unavailable | unresolved |

The machine-readable source lock is
`run_manifests/grchombo_dependency_lock.yaml`. The read-only verifier is
`scripts/verify_grchombo_dependency.sh`. It fails on a wrong origin, wrong
commit, attached branch, or dirty checkout.

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

The manifest locks source inspection, not a full build. No fresh-machine
production reproducibility claim is permitted until the Chombo revision,
PETSc tuple where applicable, container image/recipe digests, and the target
dimension build have been locked and executed.

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
`CCZ4Vars` can own this state. The available direct header probe is also
blocked earlier by missing Chombo `parstream.H`, so no full target stock
header/build claim is made. The earlier direct GRChombo comparisons remain
matched stock-`d=3` evidence.

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
| GP initial-data `BoxLoop` | extend | add a black-string compute class later; reuse `BoxLoops`, grid traversal, and parameter plumbing |
| CCZ4 RHS | wrap | retain stock visible families and add explicitly reported hidden/cartoon increments through a thin adapter |
| Modified-cartoon derivatives and geometry | extend | use GRChombo derivative/ghost infrastructure; add only hidden representative formulas and axis policy |
| Algebraic cleanup | wrap | reuse positivity; extend determinant and `A`-trace operations for `hww/Aww` with multiplicity two |
| Constraints | wrap | retain stock orchestration/reductions; add physical-`d=4` hidden terms |
| Fixed lapse source | wrap | later add the validated field-independent lapse-only source after raw stock gauge evaluation |
| Periodic `z` ownership | reuse unchanged | Chombo domain/ghost exchange owns periodicity; project code supplies configuration only |
| Diagnostics and horizon data | reuse and extend | reuse reductions, interpolation, I/O, and AH machinery; add black-string `R_H`, minimum radius, area, and Fourier data |
| RK, AMR, MPI/OpenMP, checkpoint/restart | reuse unchanged | no project-owned replacement |

Thin adapters must preserve separately observable stock-visible and
black-string-hidden contributions. Copying the complete custom solver into
production is explicitly rejected.

## Oracle-to-production comparison seam

The first pointwise production acceptance target will be a test adapter with
the following contract:

- Input: `(x,z,r0)`, the 18 target values, analytic first and second
  derivatives in the two gridded directions, the modified-cartoon
  regularity/hidden derivative data, lapse/shift derivatives, and explicit
  formulation parameters.
- Locked formulation: CCZ4, physical `d=4`, gridded `d=2`, hidden
  multiplicity two, vacuum, `Lambda=0`, `kappa1=0.1`, `kappa2=0`,
  `kappa3=1`, and `covariantZ4=true`.
- Output: all 13 physical RHS rows in the frozen order
  `(chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,GammaX,GammaZ)`.
- Reporting: stock-visible contributions, adapted hidden/cartoon
  contributions, and final rows are recorded separately so cancellation
  cannot hide ownership errors.
- Reference: the independent
  `code/BlackStringToy/tests/Stage4AOCAnalyticFullOracle.hpp` path and its
  complete analytic full-oracle fixture. Production code must not be called
  by the oracle.
- Acceptance:
  `|a-b| <= 5e-13 + 5e-12 max(|a|,|b|)` for pointwise comparisons, plus
  component ownership, hidden-multiplicity, parity, and mutation checks.

The adapter design allows the already direct stock-visible evidence to remain
visible while the new hidden terms are assessed independently. It is not
implemented in this preflight.

## First adaptation sequence and audit checkpoints

1. **Dependency verification.** Run the source-lock verifier and resolve the
   Chombo/container tuple needed by the selected build.
2. **Target enumeration and registration.** Implement only the 18-slot enum,
   names, compile-time dimension assertions, and exact permutation/parity
   fixture.
   **Audit checkpoint A:** verify source tuple, macro values, slots, names,
   absence of visible-`y`, and no physics path before initial data.
3. **GP initializer.** Add cellwise GP values and analytic setup checks.
   **Audit checkpoint B:** compare every initialized value with the batch-4
   convention lock before any RHS adaptation.
4. **Hidden/cartoon geometry adapter.** Add derivative and geometric hidden
   families around GRChombo infrastructure.
   **Audit checkpoint C:** compare every hidden family independently with the
   frozen oracle.
5. **Hidden-aware cleanup and constraints.** Add multiplicity-two determinant,
   trace, Hamiltonian, and momentum ownership.
   **Audit checkpoint D:** execute algebraic and constraint mutations before
   adding a gauge source.
6. **Fixed lapse-source adapter.** Add the field-independent lapse-only hook.
7. **Pointwise 13-row comparison.** Exercise the complete seam specified
   above.
   **Audit checkpoint E:** require all families and final rows to pass before
   grid ownership or evolution.
8. **Periodic-`z` grid ownership.** Configure and execute the real Chombo
   periodic/ghost path.
   **Audit checkpoint F:** verify wrap, ghost exchange, refinement, and
   restart ownership.
9. **Unperturbed background evolution.** Reuse GRChombo time integration and
   AMR with the accepted adapters.
   **Audit checkpoint G:** require stationarity, constraint convergence, raw
   versus held lapse reporting, and restart agreement before perturbations.

This design-only preflight is not assigned a separate audit. Its checkpoints
are implementation stop points. The first implementation substage after
preflight is dependency verification followed by the isolated target
enumeration/registration change; GP data and all RHS work remain out of that
substage.

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

Future Chombo `Cell`/`FArrayBox` wrappers will load the 18 slots into this
structure, expose visible `(x,z)` data to stock formula families, add
black-string hidden families, and store through the same seam. For the
black-string application this replaces
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

Only the reduced Vars seam, pointwise GP contract, and analytic GP metadata
are complete. Chombo storage/`BoxLoop`, live registration, CCZ4 RHS,
hidden/cartoon geometry, cleanup/constraints, fixed lapse source, periodic
ownership, evolution, and diagnostics remain open.
