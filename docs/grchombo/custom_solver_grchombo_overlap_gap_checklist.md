# Custom Solver versus GRChombo Overlap-and-Gap Checklist

Status: inventory contract with comparison batches 1-4 evidence. The inventory
starts from the clean frozen-outer-boundary checkpoint `015a035`; batch 1
starts from the clean committed inventory checkpoint
`661468ade479cf003dc5336e665dc7b70edf48c6`. GRChombo is the production
framework and convention authority; the Stage 4AO-C custom implementation is
an independent reference/oracle. The unresolved stationary outer boundary is
deferred research and is not on the production critical path.

## Source authority and status vocabulary

The local GRChombo checkout is clean at commit
`37e659523830418b210acea1661dac0e00bb1b75` (2026-04-16, upstream origin
`GRTLCollaboration/GRChombo`). It is an ignored nested checkout, not a pinned
top-level submodule. The first adaptation task must make this source lock
reproducible. The public `grchombo/grchombo` image was previously used, but its
digest cannot be read in the current shell because Docker is unavailable.

Status terms:

- **validated oracle**: independent local tests validate the custom formula,
  but it is not production wiring;
- **production framework**: mature GRChombo functionality available for
  direct reuse;
- **adapter required**: GRChombo supplies the infrastructure or visible
  formula, but black-string/cartoon integration is absent;
- **absent**: no implementation was found in the inspected checkout;
- **diagnostic only**: useful scaffolding with no production acceptance gate.

Production-owner values are exactly: **reuse GRChombo**, **adapt GRChombo**,
**retain custom oracle**, and **custom-only deferred research**.

## Variables and conventions

| Capability | Custom implementation and status | GRChombo implementation and status | Formula or convention | Match / mismatch | Existing evidence | Required comparison and acceptance | Production owner |
|---|---|---|---|---|---|---|---|
| State variables and ordering | `Stage4AOFrozenGaugeOperator.hpp`: validated 13-slot frozen state; `UserVariables.hpp`: actual 27-slot scaffold including `hww/Aww` | `Source/CCZ4/CCZ4UserVariables.hpp`, `CCZ4Vars.hpp`, `BSSNVars.hpp`: stock 25-slot 3D CCZ4 mapping | Custom oracle order is `chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,Gx,Gz`; production also needs lapse, shift, and B | Eleven frozen variables have documented stock correspondences; hidden, visible-y, and gauge ownership remains distinct; complete arrays are not identical | Repaired batch-1 fixture includes both real enum headers, proves the custom `[0,27)` permutation, and rejects slot-swap/hidden/duplicate/omission mutations | Preserve the exact 27-slot gate and ownership separation; any future adapter round trip must cover each shared and hidden owner once | adapt GRChombo |
| Spacetime/spatial dimensions | `ConformalCartoonAlgebra.hpp`: spacetime 5D, physical spatial `d=4`, gridded `CH_SPACEDIM=2`, hidden multiplicity 2 | `DimensionDefinitions.hpp`: `GR_SPACEDIM` defaults 3 and tensor loops default to `CH_SPACEDIM`; stock examples are 3D | Project target is `GR_SPACEDIM=4`, `CH_SPACEDIM=2` | Target convention is not the stock production instantiation | Stage 1.5 scratch compile notes; custom dimension assertions | Compile a pinned minimal production target with both dimensions visible in emitted flags and runtime manifest; all tensor intervals must map without static-assert failures | adapt GRChombo |
| Conformal metric/factor | `ConformalCartoonAlgebra.hpp`, Stage 4AO-A note: `gamma_IJ=chi^-1 h_IJ`, `chi=det(gamma)^(-1/4)` | `ADMConformalVars.hpp`, `CCZ4RHS.impl.hpp` | Same conformal decomposition, with dimension-dependent exponent/trace | Formula family matches; dimension must be explicit | Stage 4A algebra fixture and nonlinear JVP oracle | Random positive-definite metric comparison after dimension adapter; determinant, inverse, and reconstructed physical metric within `5e-13 + 5e-12 scale` | retain custom oracle |
| `K` definition/sign | Stage 4AO-A: `partial_t gamma=-2 alpha K+L_beta gamma`; GP `K=3 lambda/2` | `ADMConformalVars.hpp`, `CCZ4RHS.impl.hpp` use the same evolution sign | `K=gamma^IJ K_IJ` | Known match for selected branch | GP analytic residual and K-row fixtures | Formula manifest plus GP pointwise test; exact coefficients and pointwise tolerance must pass | retain custom oracle |
| `A_IJ` definition | `ConformalCartoonAlgebra.hpp`: `A_IJ=chi(K_IJ-gamma_IJ K/4)` | `ADMConformalVars.hpp`: `A=chi(K_ij)^TF`, with denominator `GR_SPACEDIM` | Trace-free denominator is physical dimension | Same family; stock runtime denominator is 3, target is 4 | Stage 4A/4AL and trace-free projector fixtures | Compare reconstruction and trace removal under an explicit `d=4` adapter; weighted trace including `2 Aww` below `5e-13` | adapt GRChombo |
| `Theta` | 13-state oracle and selected CCZ4 rows in `Stage4AOFrozenGaugeOperator.hpp` | `CCZ4Vars.hpp`, `CCZ4RHS.impl.hpp` | CCZ4 Hamiltonian-constraint variable | Same selected-CCZ4 role; production hidden terms absent | Theta algebraic, Ricci, minus-K, damping, and complete-row fixtures | Pointwise analytic-jet comparison of every Theta family; Level-2 tolerance | retain custom oracle |
| Hatted Gamma | `CartoonHatGammaX.hpp`, `Stage4AOFrozenGaugeContractedConnection.hpp` | `BSSNVars.hpp`, `CCZ4Geometry.hpp`, `CCZ4RHS.impl.hpp` store `Gamma[i]` as hatted connection in CCZ4 | `hatGamma^i=tildeGamma^i+2 Z_over_chi^i` | Visible convention matches; hidden contraction is custom | Stage 4AN and Stage 4AO-C Gamma fixtures | Compare contracted visible contribution first, then add hidden adapter; every term and parity sign must meet Level-2 tolerance | retain custom oracle |
| `Z^i`, `Z_i`, `Z_over_chi` | Contracted-connection and Z-derivative adapters reconstruct upper/lower forms | `CCZ4Geometry::compute_ricci_Z[_general]` reconstructs `Z_over_chi=0.5(Gamma-chris.contracted)` | Lowering must use the conformal/physical map consistently; no double chi | Core reconstruction matches; hidden lowering has no stock counterpart | Contracted-connection, encoded-Z tensor, derivative, insertion tests | Identical analytic jet with visible hidden terms suppressed, then target adapter enabled; reconstructed vectors and Ricci-Z split meet Level-2 tolerance | retain custom oracle |
| Hidden `ww` representative/multiplicity | `hww/Aww` representative with multiplicity 2 in `ConformalCartoonAlgebra.hpp` and cartoon helpers | Stock GRChombo checkout has no `ww` variable or modified-cartoon source path; BlackStringToy scaffold only adds inert slots | Full traces contain `2 h^ww T_ww`; component equations write `ww` once | Production gap | Hidden multiplicity and representative-write mutation tests across Stage 4A-4AO-C | Adapter tests must distinguish one representative write from two trace copies and reject multiplicity 1 or representative duplication | adapt GRChombo |
| Extrinsic-curvature sign | Locked in Stage 4AO-A and all custom row derivations | GRChombo CCZ4 evolution convention | `partial_t gamma_ij=-2 alpha K_ij+L_beta gamma_ij` | Known match | Background residual and nonlinear JVP tests | Include sign mutation in Level 1 and GP pointwise RHS; mutation must fail | retain custom oracle |
| Determinant convention | `det_4 h=(hxx hzz-hxz^2) hww^2`; tangent cleanup custom-only | No runtime conformal-metric determinant-normalization compute class was found in the inspected stock path; initial-data helpers do use determinant formulas | Target `det h=1` in four physical spatial dimensions | Formula convention is known, but no callable stock cleanup output or hidden enforcement exists | Stage 4A determinant and Stage 4AO-C tangent tests; batch-3 source inventory | Nonzero admissible data, exact hidden determinant, idempotence, and no slot loss after a reviewed adapter; Level-2 tolerance | adapt GRChombo |
| Trace-free convention | Weighted custom trace includes two hidden copies | `TraceARemoval.hpp` uses stock tensor inverse/loop | `tr_h A=0`, denominator `d` | Stock BlackStringToy call ignores `Aww`; production mismatch | Trace-free projector and metric-inclusive tangent fixtures | Replace/extend production cleanup, then compare to oracle on nonzero data; weighted trace below `1e-12` | adapt GRChombo |

## Evolution equations

| Capability | Custom implementation and status | GRChombo implementation and status | Formula or convention | Match / mismatch | Existing evidence | Required comparison and acceptance | Production owner |
|---|---|---|---|---|---|---|---|
| `chi` RHS | Complete frozen row in `Stage4AOFrozenGaugeOperator.hpp` | `CCZ4RHS.impl.hpp` nonlinear production equation | Advection plus `(2/d)chi(alpha K-div beta)` | Same family; custom is `d=4` linearization | Complete chi/metric and full-interior JVP fixtures | Pointwise nonlinear finite-difference JVP against GRChombo with explicit `d=4`; Level-2 tolerance | retain custom oracle |
| `hxx,hxz,hzz` RHS | Complete frozen visible rows | `CCZ4RHS.impl.hpp` nonlinear visible rows | Advection, shift stretching, `-2 alpha A_ij` | Direct overlap after dimension/gauge lock | Shift, algebraic, complete-row, full-JVP tests | Per-component family-isolation and mixed-jet JVP; Level-2 tolerance | retain custom oracle |
| `hww` RHS | Complete representative frozen row with cartoon shift term | Absent in stock GRChombo; scaffold freezes it | Same tensor family plus hidden-direction `beta^x/x` geometry | Production gap | Hidden row, ownership, parity, JVP tests | Adapt production row and compare every family plus representative ownership | adapt GRChombo |
| `K` RHS | Complete selected-CCZ4 frozen row | `CCZ4RHS.impl.hpp` has CCZ4/BSSN switch | Selected `USE_CCZ4`; rejected `A^2+K^2/d` K block belongs to BSSN branch | Known historical branch mismatch corrected | K CCZ4, Ricci, damping, complete-row tests | Formula branch manifest must reject BSSN mutation; mixed analytic jets meet Level-2 tolerance | retain custom oracle |
| Visible `A` RHS | Complete `Axx,Axz,Azz` rows | `CCZ4RHS.impl.hpp` nonlinear visible equations | TF curvature/lapse, algebraic terms, advection/stretching | Direct overlap after `d=4` adapter | A algebraic, Ricci, complete-row and JVP fixtures | Isolate curvature, algebraic, and shift families; componentwise Level-2 tolerance | retain custom oracle |
| `Aww` RHS | Complete representative frozen row | Absent in stock production; scaffold freezes it | Hidden Hessian/Ricci, TF projection, algebraic and shift terms | Production gap | Stage 4AJ-4AL and 4AO-C ownership tests | Adapt full row, compare single write and weighted trace; Level-2 tolerance | adapt GRChombo |
| `Theta` RHS | Complete frozen row | `CCZ4RHS.impl.hpp` nonlinear CCZ4 row | `R/2`, algebraic constraint terms, `-K Theta`, damping | Visible overlap; hidden scalar trace absent upstream | Family tests and nonlinear JVP | Compare visible part, then hidden scalar completion; all rows Level-2 tolerance | retain custom oracle |
| `hatGamma^x,z` RHS | Complete frozen rows including hidden contractions | `CCZ4RHS.impl.hpp` nonlinear visible rows | K/Theta/chi gradients, connection-A, vector Hessian, grad-div, Z damping | Visible overlap; hidden x terms custom | Five family fixtures, complete Gamma, full JVP/parity | Family-by-family analytic jets, then full mixed direction; Level-2 tolerance and forbidden parity below `1e-13` | retain custom oracle |
| Physical Ricci | Visible custom `Rxx,Rxz,Rzz`, hidden `Rww`, trace/TF assembly | `CCZ4Geometry::compute_ricci[_Z]` visible tensor path | `R=Rxx+Rzz+2Rww` in target reduction | Visible formulas comparable; hidden terms absent upstream | Independent component and split-vs-direct fixtures | Same analytic metric jets; visible match first, hidden adapter separately; Level-2 tolerance | retain custom oracle |
| Encoded-Z Ricci | Custom tensor helper, derivative adapter, row insertions | `CCZ4Geometry::compute_ricci_Z` and `_general` | `Z_over_chi=(hatGamma-tildeGamma)/2`; selected `D_(i Z_j)` coefficient | Formula family overlaps | Z completion/adapter/insertion mutation tests | Compare raw physical Ricci, encoded-Z increment, and sum separately | retain custom oracle |
| Damping and kappas | Locked custom `kappa1=.1,kappa2=0,kappa3=1,covariantZ4=true` | `CCZ4_base_params_t`, `CCZ4RHS.impl.hpp`, parameter parser | Covariant Z4 replaces `kappa1` by `kappa1/lapse` | Parameter convention matches intended branch | Dedicated damping and contract tests | Parse/manifest equality plus nonunit-lapse pointwise comparison; exact parameters and Level-2 RHS tolerance | reuse GRChombo |
| Advection | Frozen GP `beta^x d_x` custom block | GRChombo derivative policy and CCZ4 advection | Sign follows `+ beta^i partial_i` RHS convention | Known match at formula level | Advection fixture and GP residual | Identical supplied derivatives eliminate stencil differences; exact coefficient/sign and Level-2 tolerance | reuse GRChombo |
| Shift stretching | Custom scalar/tensor blocks | `CCZ4RHS.impl.hpp` | Tensor Lie/stretch terms with `2/d div beta` trace removal | Same family, dimension differs | Tensor stretching and complete-row tests | Isolated affine shift jet in `d=4`; exact coefficient table | retain custom oracle |
| Hidden-sphere/cartoon terms | Extensive `Cartoon*.hpp` and frozen-Gauge hidden helpers; validation-only | No modified-cartoon or black-string source found in inspected GRChombo commit | Two hidden copies, checked `1/x` and `1/x^2` limits, representative `ww` | Largest physics production gap | Stage 4A-4AN and 4AO-C hidden mutation battery | Adapt into production and compare analytic jets, axis limits, parity, and manufactured profiles | adapt GRChombo |

## Gauge and background

| Capability | Custom implementation and status | GRChombo implementation and status | Formula or convention | Match / mismatch | Existing evidence | Required comparison and acceptance | Production owner |
|---|---|---|---|---|---|---|---|
| GP lapse/shift | Stage 4AO-A derivation and `Stage4AOGPDiscretePreflight.hpp`; validated oracle only | Generic lapse/shift fields and gauge hooks, no GP black-string initializer found | `alpha=1`, `beta^x=sqrt(r0/x)`, `beta^z=0` | Convention matches; production data owner remains absent | Batch 4 directly exercises stock gauge/RHS and derivative paths against independently constructed analytic GP jets | Production `BoxLoop` initializer values/derivatives at random radii; absolute error `<=5e-14` | adapt GRChombo |
| Fixed GP-holding lapse source | Documented Stage 4AO-A source and batch-4 test-only adapter; not production code | Direct `MovingPunctureGauge.hpp` gives raw `-3 lambda`; no fixed source exists | `S_alpha=+3 sqrt(r0/x^3)`, locked-background and field independent | Raw stock gauge convention matches; production adapter absent | Batch 4 proves exact cancellation, zero linearization, lapse-only ownership, and rejects six active source mutations | Add through a reviewed gauge adapter; raw residual remains visible and source-adjusted continuum residual vanishes | adapt GRChombo |
| Frozen-gauge operator | Complete 13-variable custom oracle | GRChombo provides nonlinear live fields, not a frozen linear operator | `delta alpha=delta beta=delta B=0` | Custom-only reference role | Full interior nonlinear JVP and parity | Compare custom JVP to finite differences of adapted GRChombo nonlinear RHS | retain custom oracle |
| Live moving-puncture gauge | Only convention/startup documentation; no custom live operator | `MovingPunctureGauge.hpp` production implementation | Bona-Masso lapse and Gamma-driver shift | GRChombo-only production capability | Inherited BinaryBH smoke only | GP startup and perturbed gauge RHS comparison after source adapter; Level-2 tolerance | reuse GRChombo |
| Background residuals | Analytic zero-residual oracle and second-order discrete custom preflight | Direct stock-visible `d=3` CCZ4 RHS is callable, but lacks target hidden/dimension completion | Full `d=4` GP geometric residual zero; unmodified lapse residual `-3 lambda` | Custom full result passes; reduced stock-visible nonzero rows are classified as omitted hidden/dimension completion, not errors | Batch 4: custom preflight order `1.8575`; direct stock reduced residual ledger; direct gauge drift | Repeat through the adapted target `d=4/2` production state and require source-held convergence to zero | retain custom oracle |
| Black-string initial data | Explicit formulas and analytic jets validated in batch 4; not wired | Current BlackStringToy uses inherited BinaryBH data; stock tensor conventions directly exercised | Coordinate `gamma_theta_theta=x^2`; custom stored `hww=gamma_theta_theta/x^2=1`; uniform GP compact string | Convention seam established; production initializer absent | Batch 4 determinant, weighted trace, K/A reconstruction, state ownership, analytic-jet, and wrong-`hww` mutation gates | Production cellwise comparison plus hidden-aware determinant/trace/constraint checks | adapt GRChombo |
| Perturbation/Fourier convention | Test-only all-13-slot cosine/sine assignment; radial profile deliberately unlocked | GRChombo supplies periodic grids but no project Fourier initializer | `k_n=2 pi n/L`; scalar/even cosine and one-z sine in `P+`, swapped in `P-`; frozen gauge unperturbed | Convention lock only | Batch 4 checks mode/amplitude, all state slots, parity classes, and zero gauge perturbations | Initialize both parities on production periodic grid; projection leakage `<=1e-12`, mode amplitude exact to discretization | adapt GRChombo |

## Numerical infrastructure

| Capability | Custom implementation and status | GRChombo implementation and status | Formula or convention | Match / mismatch | Existing evidence | Required comparison and acceptance | Production owner |
|---|---|---|---|---|---|---|---|
| Radial derivatives | Direct custom centered order-two validation helpers | Selected `FourthOrderDerivatives.hpp` direct kernels; smoke locks order four | Different formal orders | Same continuum derivatives; finite-grid values intentionally differ | Batch 3: custom observed `1.9974-1.9975`, GRChombo `3.9926-3.9933`; sign/spacing/order mutations detected | Preserve manufactured convergence gate; production owns fourth-order path | reuse GRChombo |
| `z` derivatives/periodicity | Direct second-order modulo-wrap helper | Direct fourth-order kernels; periodic Chombo domain/ghost fill unavailable without Chombo | `z~z+L`, endpoint wrap owned by framework | Kernel continuum result matches; full production periodic ownership remains blocked | Batch 3: custom `1.9966-1.9972`, GRChombo `3.9895-3.9905`; periodic-wrap mutation detected | Repeat through actual Chombo periodic domain after dependency lock; do not call local analytic ghosts production evidence | reuse GRChombo |
| Mixed derivatives | Direct composition of custom radial and periodic helpers | Direct GRChombo `mixed_diff2` tensor-product kernel | Mixed derivatives on smooth tensor-product data | Same continuum result, order two versus four | Batch 3: `D_xz` custom `1.9957`, GRChombo `3.9912`; wrong-direction mutation detected | Preserve convergence and later exercise production Cell/ghost strides | reuse GRChombo |
| Endpoint stencils | Validated second-order inner one-sided helper; mirrored outer stencil diagnostic | `BoundaryConditions.hpp` generic static/Sommerfeld/reflective/extrapolating/mixed BC support | No accepted custom outer physics | Inner helper is oracle only; GRChombo outer policy undecided | Inner boundary exactness/convergence; outer scaffold tests | Retain inner regression; choose production domain/BC separately. No custom outer acceptance test authorized | custom-only deferred research |
| Outer boundary | Diagnostic row layout and rejected WKB projector only | Generic GRChombo BC infrastructure, no GL-specific asymptotic condition | Unresolved custom research | Neither side contains an accepted GL outer condition | Gate corrections and failed-boundary audit | None on production critical path; defer unless physical domain demands a custom condition | custom-only deferred research |
| Time integration | None in custom solver | `GRAMRLevel.cpp` uses `RK4LevelAdvance` | Mature RK4 AMR stepping | GRChombo-only | BinaryBH-derived one-step smoke | Standard framework convergence/smoke; do not rebuild | reuse GRChombo |
| Algebraic cleanup | Hidden-aware custom determinant/A-trace test projectors; test-only same-dimension matrix oracle | Direct `TraceARemoval.hpp` and `PositiveChiAndAlpha.hpp`; no runtime determinant-normalization compute class found; stock trace ignores hidden slots | Target cleanup must include `ww` multiplicity | Visible trace/positivity routines pass directly; determinant and hidden cleanup remain gaps | Batch 3: component max `2.776e-17`, post-trace `5.551e-17`, idempotence `1.388e-17`, exact positivity clamps | Reuse visible routines; adapt hidden determinant/trace owner and compare weighted residual `<=1e-12` | adapt GRChombo |
| AMR | No custom implementation | `GRAMR`, `GRAMRLevel`, Chombo AMR | Framework-owned refinement, interpolation, reflux/ghost operations | GRChombo-only | BinaryBH scaffold build/run | Reuse with problem-specific tagging convergence; do not independently rebuild | reuse GRChombo |
| Grid/ghost ownership | Custom vectors/grids only in validation harness | `GRLevelData`, `GRAMRLevel::fillAllGhosts`, boundary handlers | Production ghost width follows derivative order and BC | GRChombo-only | Scratch smoke and layout checks | Document required ghosts for 4th/6th order and hidden variables; fill/BC smoke passes without undefined values | reuse GRChombo |
| MPI/OpenMP | None | Chombo MPI plus BoxLoops/OpenMP/SIMD; AH has MPI subcommunicator | Framework-owned parallel decomposition | GRChombo-only | Public examples and prior MPI AH test launch | Rank/thread reproducibility for norms and diagnostics; no independent implementation | reuse GRChombo |
| Checkpoint/restart | None | Chombo/GRAMR checkpoint hooks and restart parameters | Framework-owned HDF5 state | GRChombo-only | Inherited smoke produced checkpoint files; restart not black-string validated | Restart equivalence at fixed time: state/norm differences within roundoff or documented reduction tolerance | reuse GRChombo |
| Parameter parsing | Minimal custom constants in validation code | `GRParmParse`, `SimulationParametersBase`, project `SimulationParameters.hpp` | One authoritative parsed parameter manifest | Current project parser is BinaryBH-derived and lacks black-string parameters | Parameter-only smoke | Add required black-string parameters through GRChombo parser; `just_check_params` and manifest round trip must pass | adapt GRChombo |

## Diagnostics and observables

| Capability | Custom implementation and status | GRChombo implementation and status | Formula or convention | Match / mismatch | Existing evidence | Required comparison and acceptance | Production owner |
|---|---|---|---|---|---|---|---|
| Constraints | Analytic GP Hamiltonian/momentum and tangent checks | `Constraints.hpp/.impl.hpp`, `AMRReductions`; stock dimension path | Hidden-aware `H`, `M_x`, `M_z` required | Visible overlap; hidden production completion absent | GP residual, trace, and JVP fixtures | Pointwise hidden-aware constraints then AMR norms; Level-2 tolerance and expected convergence order | adapt GRChombo |
| Apparent horizon/MOTS | Equation and target geometry documented; no solver | `AHFinder`, `PETScAHSolver`, interpolation, `AHStringGeometry` | Outermost `S2 x S1` MOTS connected to `x=r0` | Generic machinery exists; project geometry/variables and PETSc path unvalidated | Non-PETSc AH test compiled/launched but solver skipped | PETSc-enabled string-horizon baseline, then uniform `x=r0` recovery within grid/interpolation error | adapt GRChombo |
| `R_H(t,z)` | Defined only: `h sqrt(hww/chi)` on MOTS | Generic AH surface/interpolation output, no project observable | Geometric sphere radius, not lapse contour | Project-specific adapter absent | Design review only | Uniform background gives `R_H=r0`; perturbed evaluation converges under grid/AH refinement | adapt GRChombo |
| Minimum string radius | Defined as downstream need, not implemented | Generic reductions/interpolation can support it | `min_z R_H(t,z)` | Absent project diagnostic | None | Manufactured `R_H` profile exact minimum plus MPI-invariant reduction | adapt GRChombo |
| Fourier amplitudes | Custom standalone cosine/sine projections in Stage 4AO-B/C | No dedicated project Fourier utility; periodic grids and SmallDataIO available | `k_n=2 pi n/L`; parity-aware phases | Custom-only focused oracle | Parity/leakage tests | Analytic single/mixed-mode recovery; amplitude/phase error `<=1e-12` on exact sampled modes | retain custom oracle |
| Growth-rate extraction | Not implemented; design target only | No project-specific fitter in GRChombo | Fit `log |A_n(t)|` only in verified linear window | Gap | None | Synthetic exponential with noise/window mutations, then resolution-consistent physical rate | adapt GRChombo |
| Horizon area | Not implemented | AH framework provides geometry/data hooks; exact string-area adapter not identified | Must use `S2 x S1` induced area with hidden multiplicity | Source-specific adapter information missing | None | Uniform analytic area and refinement convergence before physical use | adapt GRChombo |
| Convergence measurements | Extensive standalone custom fixture norms/orders | AMR reductions and multi-resolution production runs | Compare continuum quantities at common times/locations | Complementary capabilities | Many Stage 4 fixtures; inherited smoke | Automated three-resolution tables for RHS, constraints, background, horizon, and growth | reuse GRChombo |
| Generation/formation observables | No accepted definition or implementation | No black-string-specific implementation found | Must be defined geometrically before coding | Gap on both sides | None | Design review must define observable, units, event criterion, and convergence target | adapt GRChombo |

## Overlap summary and disposition

Directly comparable after a thin data adapter: conformal algebra, visible
CCZ4 chi/metric/K/A/Theta/Gamma families, Ricci/Z reconstruction, damping,
advection, shift stretching, moving-puncture gauge, derivative operators, and
constraints. The custom implementation supplies independent formula and JVP
oracles; GRChombo supplies the nonlinear production execution framework.

Custom-only validated knowledge: physical-`d=4` hidden multiplicity,
representative `ww` formulas, frozen-GP background/JVP references, and focused
parity/mutation fixtures. GRChombo-only mature infrastructure: RK4, AMR,
ghost exchange, generic boundaries, MPI/OpenMP, checkpoint/restart, parameter
parsing, reductions, interpolation, and AH/PETSc machinery.

Largest gaps are the reproducible dependency lock, exact production GP initial
data, fixed lapse-source adapter, target-dimension modified-cartoon RHS and
cleanup, compact-periodic-z production setup, black-string constraints,
Fourier/growth diagnostics, and string-horizon observables. Stock GRChombo at
the inspected commit does not contain a black-string or modified-cartoon
implementation; an authoritative source/branch for those formulas has not
been supplied.

## Custom-code disposition

| Custom component | Disposition | Reason / exit condition |
|---|---|---|
| Complete 13-variable interior operator | **retain as trusted oracle** | Complete independent frozen-gauge reference; retain after production equivalence for regression |
| Analytic nonlinear JVP oracle | **retain as trusted oracle** | Best independent componentwise comparison target; never call production helpers |
| Hidden-sphere helpers | **retain as trusted oracle** and **focused regression tests** | Preserve checked multiplicity, axis, parity, and representative ownership while adapting production |
| Inner boundary helper | **retain as focused regression test** | Validates the custom radial formulation only; do not promote to GRChombo production ownership |
| Diagnostic outer scaffolding | **freeze as deferred research** | Useful row-layout history but no accepted outer condition |
| Rejected one-hot WKB projector | **quarantine because validation failed** | Must not feed production or gates; eventually remove after archival needs and GRChombo equivalence review |
| Stationary asymptotic matrix extraction | **retain as focused regression test** | Faithfully reproduces the validated interior operator; it is not a boundary derivation |
| BinaryBH-derived smoke scaffold | **eventually remove after GRChombo equivalence is established** | Retain only until the real black-string production path supersedes smoke-only hidden freezing |

## Executed comparison batch 1

The dependency manifest, repaired custom 27-slot/shared-variable map,
same-dimension conformal algebra, visible contracted connection/encoded-Z
reconstruction, and visible physical Ricci comparisons were executed in
`Stage4AOCGRChomboComparisonBatch1Test.cpp`. The bridge directly includes the
inspected GRChombo `TensorAlgebra` and `CCZ4Geometry` headers; it does not copy
their formulas into a second object labeled as GRChombo.

The original layout gate did not include the real custom `UserVariables.hpp`
and could not prove numeric custom slots. Its exact-map claim is retracted.
The repaired compiled gate checks the actual 27-slot custom enum, the
namespaced stock 25-slot enum, and the frozen thirteen-variable order.

| Capability | Updated status | Batch-1 evidence |
|---|---|---|
| Dependency/source manifest | partial | custom and GRChombo commits, detached clean GRChombo status, compiler, flags, and dimensions recorded; Chombo and container digests unresolved |
| State/order map | repaired exact custom layout and correspondence | all 27 custom slots form the unique complete `[0,27)` permutation; eleven frozen variables have the documented stock correspondence; `hww/Aww` are hidden-only; seven visible-y and seven gauge slots remain separate |
| Dimension map | explicit translation | direct fixture is stock `d=3`; custom production stays physical `d=4`, gridded `d=2`, hidden multiplicity two |
| Conformal algebra | direct compiled tolerance match for executable identities | maximum absolute error `4.441e-16`; maximum normalized error `6.808e-05` |
| `gamma=h/chi`, `Kij` reconstruction | source/convention match only | no standalone inspected GRChombo production function exists for a direct call |
| Visible contracted connection/Z | direct compiled tolerance match for connection; source-locked Z reconstruction | maximum absolute error `1.837e-14`; maximum normalized error `1.036e-02` |
| Hidden connection/Z | custom-only | separated analytic increment; no stock GRChombo owner |
| Visible physical Ricci | direct compiled tolerance match | maximum absolute error `2.220e-16`; maximum normalized error `1.530e-04` |
| Hidden Ricci/cartoon | custom-only pending adaptation | deliberately excluded from the direct stock comparison |

The repaired mapping gate rejects swapped `h11/h13`, hidden-to-visible-y
assignments for both `hww` and `Aww`, a duplicated shared slot, and an omitted
shared slot. Wrong trace dimension, contracted-connection sign, and Ricci
off-diagonal sign mutations also fail. This proves the custom layout and the
eleven documented correspondences, not identical complete custom and stock
state arrays. Full evidence, cases, directness classifications, and blockers
are in
`docs/grchombo/custom_solver_grchombo_comparison_batch1_results.md`.

## Executed comparison batch 2

Batch 2 directly compiles and invokes the inspected GRChombo
`CCZ4RHS::rhs_equation`, `CCZ4Geometry::compute_ricci`, and
`compute_ricci_Z` at stock `d=3`. Its independent side is a test-only
generalized-dimension custom term-family path evaluated at `d=3`; the
production physical-`d=4`, gridded-`d=2`, hidden-multiplicity-two operator is
unchanged.

| Capability | Updated status | Batch-2 evidence |
|---|---|---|
| Formulation/parameter lock | direct source lock | `USE_CCZ4`, `d=3`, `Lambda=0`, `kappa1=0.1`, `kappa2=0`, `kappa3=1`, `covariantZ4=true`, vacuum matter, supplied analytic lapse/shift, frozen gauge outputs |
| Raw visible physical Ricci | direct compiled tolerance match | tensor/scalar maximum absolute errors `1.943e-16` / `3.053e-16` |
| Visible encoded-Z completion | direct compiled tolerance match | `compute_ricci_Z-compute_ricci` tensor/scalar/TF maximum absolute errors `1.943e-16` / `2.776e-16` / `1.943e-16` |
| Standalone `Z_over_chi`, `Z^i`, and `Z_i` map | source/convention comparison | uses a directly computed GRChombo contracted connection; `Z_over_chi` and `Z^i` are reconstructed locally from `CCZ4RHS.impl.hpp`, and the fixture additionally lowers to `Z_i`; maximum absolute discrepancy `1.110e-16` |
| Combined visible Ricci-Z | direct compiled tolerance match | tensor/scalar maximum absolute errors `1.665e-16` / `8.327e-17` |
| `chi` RHS | direct compiled family and combined match | lapse-`K`, divergence, and advection pass separately; combined maximum absolute error `5.551e-17` |
| Visible `h_ij` RHS | direct compiled family and combined match | algebraic and advection families pass; shift stretching plus trace correction passes; combined maximum absolute error `2.776e-17` |
| `K` RHS | direct compiled family and combined match | Ricci-Z, nonlinear, damping, lapse-Hessian, and advection families pass; combined maximum absolute error `1.110e-16` |
| `Theta` RHS | direct compiled family and combined match | Ricci-Z, `K^2/A^2`, `-K Theta`, damping, lapse-gradient/Z, and advection pass; combined maximum absolute error `5.551e-17` |
| Visible `A_ij` RHS | direct compiled family and combined match | Ricci-Z, lapse Hessian, linear, quadratic, advection, and combined shift families pass; no direct `A` damping is present; combined maximum absolute error `1.943e-16` |
| Separate shift-stretch/trace pieces | source/convention only | production exposes their sum; that sum is directly executed and passes |
| Matter and `Lambda` additions | source/convention only | selected vacuum branch and `Lambda=0` are locked |
| Hidden `ww` RHS/Ricci/Z/cleanup | custom-hidden-only pending adaptation | deliberately excluded; stock GRChombo has no representative slots or modified-cartoon owner |

Omission or duplication of encoded Z, wrong trace-free dimension, family
omission/duplication/sign, wrong RHS dimension, and the BSSN branch are
detected against direct compiled results. Wrong Z lowering and a missing
conformal factor are separately detected mutations of the local
source/convention reconstruction.
This evidence upgrades the visible same-dimension RHS overlap from
“comparison planned” to “directly equivalent for the exercised families.” It
does not validate target-`d=4` production or hidden/cartoon equations. Detailed
cases, maxima, directness classifications, and blockers are in
`docs/grchombo/custom_solver_grchombo_comparison_batch2_results.md`.

## Executed comparison batch 3

Batch 3 began from clean checkpoint
`571b142ab2bd8c14abb967eb259f2ca202ec9d22`. It directly invokes the actual
custom order-two interior derivative helpers and actual GRChombo order-four
scalar derivative kernels. Because the orders differ, the comparison is
convergence-only at finite resolution. Across `N=32,64,128,256`, every
first, second, mixed, and divergence class achieves its documented order
within `0.25`.

| Capability | Updated status | Batch-3 evidence |
|---|---|---|
| Custom interior derivatives | direct custom helper, focused oracle only | first/second/mixed/divergence orders `1.9957-1.9975` |
| GRChombo derivative kernels | direct compiled GRChombo | first/second/mixed/divergence orders `3.9895-3.9933` |
| Periodic ownership | partial | custom modulo wrap direct; GR kernel direct on local analytic ghosts; Chombo periodic ghost exchange blocked |
| Manufactured raw Ricci / encoded Z | different-order convergence | custom orders `1.9969-1.9972`; GRChombo `3.9917-3.9951` |
| Manufactured lapse Hessian | different-order convergence | unchanged: custom `1.9954`, GRChombo `3.9925` |
| All 15 visible advection rows | componentwise different-order convergence | every row passes; aggregate custom `1.9970`, GRChombo `4.0246`; worst `hyy` at `z=pi/2`; metric/A omission, duplication, and sign mutations fail independently |
| Raw shift derivative inputs | input-kernel diagnostic only | custom `1.9965`, GRChombo `3.9914`; not labeled RHS-family evidence |
| Complete `chi`, `h_ij`, and `A_ij` shift RHS families | direct compiled GRChombo family isolation versus independent custom combination | aggregate orders `1.9972/3.9920`, `1.9968/3.9916`, and `1.9966/3.9915`; every component and location retained; omission/duplication/coefficient/sign mutations fail |
| Combined visible rows | common continuum result | custom order `1.9968`, GRChombo `3.9879`; extrapolated maximum difference `4.91998803e-13` |
| Visible trace removal | direct compiled GRChombo versus independent matrix oracle | component max `2.77555756e-17`, post-trace `5.55111512e-17`, idempotence `1.38777878e-17` |
| Chi/lapse positivity | direct compiled GRChombo | exact declared clamps and idempotence pass |
| Determinant normalization | source-only/absent | no runtime stock compute class found; local exponent controls are not direct evidence |
| Hidden-aware cleanup | adaptation gap | stock path has no `hww/Aww` or multiplicity-two owner |

The direct derivative kernels compile without Chombo, but full Chombo periodic
domain and ghost exchange remain blocked by the missing local installation and
unresolved Chombo/container digest. The locally populated analytic ghost patch
is direct-kernel evidence only, not production-domain evidence. Detailed
tables and mutations are in
`docs/grchombo/custom_solver_grchombo_comparison_batch3_results.md`.

## Executed comparison batch 4

Batch 4 began from clean checkpoint
`debafd929377498eaa22c84fdf8c84409f7c3cee`. It directly invokes the
inspected stock `d=3` tensor algebra, contracted-connection/CCZ4 RHS,
fourth-order derivative, and moving-puncture gauge paths. The custom side
independently constructs analytic GP jets, calls
`make_centered_derivative_jet`, and calls the validated full `d=4` background
oracle.

| Capability | Updated status | Batch-4 evidence |
|---|---|---|
| GP metric/extrinsic convention | direct stock sign plus source/convention target gate | coordinate `gamma_theta_theta=x^2`, but custom normalized `hww=1`; storing `x^2` in the `hww` slot and reversing the stock extrinsic sign are rejected |
| Custom full `d=4` K/A reconstruction | independently validated oracle | `K=3 lambda/2`, `Axx=-7 lambda/8`, `Azz=-3 lambda/8`, `Aww=5 lambda/8`; determinant one and weighted trace zero |
| Stock-visible `d=3` seam | direct compiled stock algebra | ordinary visible `y` remains separate; `K=-lambda/2`, `Axx=-lambda/3`, `Ayy=Azz=lambda/6` for the honest reduced tensor |
| Full background residual | custom-hidden-only | actual `Stage4AOCAnalyticFullOracle` evaluates all 13 rows; a separate full-geometry evaluator checks `H,Mx,Mz`; all are zero within `5.5e-20` |
| Stock/target GP family split | independent test-only decomposition plus direct stock totals | stock K/A families reproduce `1/8,1/12,-1/24,-1/24`; independently evaluated target K/A families vanish through target K/A, hidden shift divergence, `2/d=1/2`, and multiplicity two—not residual negation |
| Discrete GP residual | separate second-order stencil evidence | `N=256,512,1024,2048` pairwise orders `1.8114,1.9036,1.9513`; worst row is K with retained radial location |
| Moving-puncture gauge | direct compiled GRChombo | unmodified lapse is `-3 lambda`; shift and `B` sources are zero for locked inputs |
| Fixed lapse source | validated test-only adapter plus direct raw gauge result | adapter changes lapse only; all 20 evolved-field Jacobian entries are zero; wrong `2K`, horizon-dependent, sign/factor, shift-owner, and B-owner adapters fail |
| GP derivatives/discrete visible RHS | different-order convergence | custom order two; GRChombo order four through its clean plateau; four refinements executed |
| Fourier/parity setup | convention gate only | `k_n=2 pi n/L`, all thirteen frozen slots classified, one-z phase swap checked, gauge perturbations zero, radial profile unlocked |

Production `BoxLoop` initial data, target-dimension hidden evolution,
hidden-aware cleanup/constraints, Chombo periodic ghost ownership, and the
fixed-source production adapter remain open adaptation tasks. Detailed tables
and classifications are in
`docs/grchombo/custom_solver_grchombo_comparison_batch4_results.md`.

## Production-adaptation preflight

The inspected GRChombo source authority is now represented by
`run_manifests/grchombo_dependency_lock.yaml` and checked by
`scripts/verify_grchombo_dependency.sh`. The verified lock covers the origin,
commit `37e659523830418b210acea1661dac0e00bb1b75`, and detached-clean checkout.
It does not cover Chombo, PETSc, Docker-image, or container-recipe digests;
production build reproducibility therefore remains unresolved.

The approved future production ownership differs from the batch-1 27-slot
comparison map. It is a compact 18-slot black-string state under
`CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and `DEFAULT_TENSOR_DIM=4`:

| Slot range | Owner | Status |
|---|---|---|
| `0..3` | `chi,hxx,hxz,hzz` | stock-visible identities wrapped at target dimensions |
| `4` | `hww` | custom hidden representative; multiplicity two |
| `5..8` | `K,Axx,Axz,Azz` | stock-visible families plus target-dimension/hidden completion |
| `9` | `Aww` | custom hidden representative; multiplicity two |
| `10..12` | `Theta,GammaX,GammaZ` | stock CCZ4 families plus hidden completion |
| `13..17` | `lapse,shiftX,shiftZ,Bx,Bz` | stock gauge ownership; future lapse-only source wrapper |

Visible-`y` tensor, Gamma, shift, and `B` slots are deliberately absent and
must not be identified with `ww`. The GP convention remains
`gamma_theta_theta=x^2`, `gamma_ww=hww/chi`, and stored `hww=1`.

The minimum production seam is thin: replace only the black-string enum and
names; wrap stock CCZ4 RHS, cleanup, constraints, and gauge; extend only
hidden/cartoon physics; reuse Chombo periodic grids, ghost exchange, RK, AMR,
parallelism, checkpointing, reductions, interpolation, and AH infrastructure.
The first future pointwise gate accepts identical values and derivative jets
and returns all 13 physical RHS rows, reporting stock-visible, hidden-adapted,
and total families separately against
`Stage4AOCAnalyticFullOracle.hpp`. The full contract and staged audit
checkpoints are in
`docs/grchombo/grchombo_production_adaptation_preflight.md`.

### First production-contract substage

`BlackStringProductionVariables.hpp` now implements the isolated 18-slot
enum, a single aliased registration/checkpoint/output name table, and
per-slot ownership metadata. The focused strict-warning fixture verifies the
13-physical/5-gauge split, exact parity, storage-one/physical-two `ww`
multiplicity, absent visible-y fields, and the GP `hww=1` convention. All
requested layout, name, multiplicity, convention, and dimension mutations
are rejected.

This is contract evidence only. The 27-slot live smoke scaffold is unchanged
and is not imported by the focused fixture. Stock `Tensor.hpp` accepts the
`CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4` tuple, but stock
`ADMConformalVars`/`VarsTools` mapping is not compatible: the selected visible
symmetric interval has three slots while the default physical tensor requires
ten, and the selected vector interval has two slots while the default vector
has four components. A black-string Vars/mapping adapter remains required;
the full stock-header probe is additionally blocked by missing Chombo
`parstream.H`.

### Reduced Vars and GP pointwise seam

The production side now has an isolated `std::array<T,18>` mapping seam with
explicit `{xx,xz,zz,ww}` tensors and `{x,z}` vectors. Load/store uses the
reviewed production enum directly, round trips all 18 slots exactly, and
writes each hidden representative once. A separate contraction helper applies
hidden multiplicity two. No stock visible-y or legacy 27-slot owner enters
the seam.

The exact GP point initializer now supplies all 18 values and separate
analytic radial jets for `beta^x`, `lambda`, `K`, and `A_IJ`. At `r0=1,x=2`
it gives `lambda=0.3535533905932738`, `beta^x=0.7071067811865476`,
unit conformal determinant, zero weighted `A` trace, and reconstructed
`Kxx=-lambda/2`, `Kzz=0`, `Kww=lambda`. Three point cases and independent
central-difference checks pass; fine epsilon data exhibit the expected
roundoff turnover.

This adds no direct compiled Chombo storage evidence. The next Chombo-facing
step remains blocked by the missing Chombo source/build tuple and must wrap
the reduced seam at the `Cell`/`FArrayBox` boundary rather than reintroducing
stock enum mapping.
