# Stage 4AO-C Frozen-Gauge Spectral Gate

Status: blocked/incomplete, with the validation-only operator wrapper,
boundary-condition contract, GP-shift advection block, tensor
shift-stretching block, algebraic metric/conformal-factor coupling block, and
the selected-CCZ4 K-equation `K(K-2Theta)` and physical-`delta R` pieces now
present, plus the `A_IJ`-output
non-curvature algebraic block and the Theta-output algebraic non-Ricci block.
The simple Theta `-K_GP deltaTheta` algebraic block and trace-free
`delta A` projector contract are also present. The first raw Ricci blocks,
hidden physical `delta R_ww[gamma]` and visible one-z
`delta R_xz[gamma]` plus visible diagonal `delta R_zz[gamma]` and
`delta R_xx[gamma]` on the frozen-GP background, are now implemented and
validated as standalone components. Raw Ricci trace and trace-free Ricci
assembly are also implemented as a standalone validation block. The Theta Ricci
scalar insertion `output[Theta] += 0.5 delta R` and the `A_IJ` curvature
insertion `output[A_IJ] += [delta R_IJ]^TF` now consume that assembly. This note
now locks the inherited GRChombo-facing main-path convention
`kappa1=0.1`, `kappa2=0`, `kappa3=1`, `covariantZ4=true` and implements only
the simple K/Theta damping insertions. Zero damping is reserved for a later
diagnostic comparison. The hatted-Gamma evolution design preflight now locks
both linearized rows, term ownership, hidden multiplicities, Z reconstruction,
and parity. A validation-only helper now implements both contracted
connections and reconstructs encoded Z. The first non-advection hatted-Gamma
RHS block now consumes that helper and inserts the locked Z/kappa and
`kappa3` shift-gradient terms for both visible directions. Complete
hatted-Gamma evolution remains missing.
This note records why the first honest frozen-gauge GL spectral validation
harness is not yet complete. It adds no eigensolver, shift-invert solve,
threshold search, production RHS wiring, live evolution wiring, or Stage
4AO-D work.

## Read-Only Reuse Inventory

This inventory treats the local checkout as authoritative. It inspected
repo-owned Stage 4 helper code, the local `external/GRChombo` checkout, and
the available public GRChombo support code. No eigensolver or operator code is
added here.

Files and symbols inspected for this pass:

- `code/BlackStringToy/Stage4AOGPDiscretePreflight.hpp`:
  `make_radial_grid`, `d1`, `d2`, `periodic_dz`, `periodic_dzz`,
  `compute_raw_background_residual_sample`,
  `max_jvp_error_for_epsilon`, `compute_z_coupled_parity_leakage`.
- `code/BlackStringToy/BlackStringToyLevel.cpp`:
  `specificEvalRHS`, `specificPostTimeStep`, hidden smoke-only scaffold
  writes, inherited `CCZ4RHS` call site.
- `code/BlackStringToy/CartoonAwayAxisPhysicalRww.hpp`,
  `code/BlackStringToy/CartoonTraceFreeCurvatureLapseBlock.hpp`, and
  `code/BlackStringToy/CartoonHatGammaX.hpp`: reviewed local physical
  `R_ww`, curvature/lapse, and hatted-connection helper contracts.
- `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`:
  `CCZ4RHS::compute`, `CCZ4RHS::rhs_equation`.
- `external/GRChombo/Source/CCZ4/CCZ4Geometry.hpp`:
  `compute_ricci_Z`, `compute_ricci_Z_general`, `compute_ricci`.
- `external/GRChombo/Source/ApparentHorizonFinder/AHFinder.hpp`,
  `ApparentHorizon.hpp`, `PETScAHSolver.hpp`,
  `PETScAHSolver.impl.hpp`, `PETScCommunicator.cpp`, `AHStringGeometry.hpp`,
  `AHFunctions.hpp`, `AHInterpolation.hpp`, `AHParams.hpp`,
  `AHVarsData.hpp`, and `AHDerivData.hpp`.
- `external/GRChombo/Tests/ApparentHorizonFinderTest2D/ApparentHorizonTest2D.cpp`.
- `external/GRChombo/Source/BoxUtils/FourthOrderDerivatives.hpp`,
  `external/GRChombo/Source/BoxUtils/AMRReductions.hpp`,
  `external/GRChombo/Source/GRChomboCore/BoundaryConditions.hpp`.
- `external/GRChombo/Source/AMRInterpolator/AMRInterpolator.impl.hpp`,
  `Derivative.hpp`, `InterpolationQuery.hpp`, `SurfaceExtraction.hpp`, and
  `CylindricalExtraction.hpp`.
- `external/GRChombo/Source/utils/SmallDataIO.hpp`.

### Eigensolver And Linear Algebra

No SLEPc, PETSc `EPS`, repo-owned eigenvalue solver, shift-invert spectral
driver, matrix-free eigen-operator, sparse GL matrix assembly, or Krylov
eigenvalue wrapper is present in the checkout.

PETSc is present only through the apparent-horizon nonlinear solve path:

- `PETScCommunicator` initializes/finalizes PETSc and optionally creates a
  PETSc sub-communicator.
- `PETScAHSolver` owns PETSc `DM`, `SNES`, `Vec`, and `Mat` objects.
- `PETScAHSolver::initialise` creates a 1D or 2D `DMDA`, a global surface
  vector, a residual vector, a sparse Jacobian matrix, and a `SNES` solve with
  `SNESSetFunction` / `SNESSetJacobian`.
- `PETScAHSolver::solve` calls `SNESSolve` and reports `SNES` and `KSP`
  iteration counts.
- `AHParams::PETSc_params` exposes `SNES` and `KSP` tolerances.

This is useful evidence that PETSc is already an allowed dependency for
horizon finding. It is not a GL eigensolver. The `KSP` object is an internal
linear solver inside the `SNES` horizon solve; it could inform a future PETSc
linear-solver adapter for shift-invert, but it is not reusable directly for
Stage 4AO-C spectral extraction.

The local checkout does not vendor Chombo source, and no Chombo multigrid or
linear-solver implementation is available here as a direct Stage 4AO-C
building block.

### AHFinder, MOTS, And Horizon Observable

The closest reusable MOTS machinery is the existing AHFinder path:

- `AHFinder` manages one or more `ApparentHorizon` objects and attaches an
  `AMRInterpolator<Lagrange<4>>`.
- `ApparentHorizon` stores the solved surface function, convergence state,
  residual statistics, coordinate output, and optional geometry data output.
- `PETScAHSolver` computes the nonlinear surface solve and a finite-difference
  Jacobian with respect to the surface function.
- `AHInterpolation` gathers variables and first/second derivatives on the
  candidate surface using `InterpolationQuery` and `AMRInterpolator`.
- `ExpansionFunction` computes
  `Theta_+ = D_i s^i + K_ij s^i s^j - K`, and for
  `GR_SPACEDIM != CH_SPACEDIM` adds the cartoon hidden-direction expansion
  terms using `g_hd`, `dg_hd`, and `Kww`.
- `AHStringGeometry` is a 2D string-like surface geometry with periodic
  parameter direction; the public `ApparentHorizonFinderTest2D` uses it to
  solve a sinusoidal string curve.

This path is reusable with an adapter for a nonlinear diagnostic MOTS solve,
but not directly for the Stage 4AO-C linearized horizon observable. The
project needs:

- a symmetry-reduced surface geometry whose parameter is compact `z` and whose
  solved coordinate is the radial cartoon coordinate `x=h(z)`;
- a diagnostic postprocessor for
  `R_H(t,z)=h(t,z) sqrt(h_ww/chi)` on the solved surface;
- a linearized MOTS response map `delta U -> delta h(z) -> delta R_H`, because
  Stage 4AO-C must test eigenvectors without repeatedly solving the nonlinear
  AHFinder problem inside the spectral operator;
- a check that the AH variable slots match BlackStringToy's repo-owned
  `hww/Aww` layout, not only public-GRChombo assumptions.

The existing `ChiContourFunction` and lapse-profile comments are not useful
for the GL growth observable. The horizon is not a lapse contour.

### Derivatives, Stencils, Interpolation, And Boundaries

Reusable pieces:

- `FourthOrderDerivatives` provides fourth-order centered `diff1`, `diff2`,
  `mixed_diff2`, advection, and dissipation hooks tied to GRChombo `Cell`
  input pointers and ghost zones.
- `SixthOrderDerivatives` exists in the same GRChombo `BoxUtils` family, and
  `BlackStringToyLevel::specificEvalRHS` selects fourth or sixth order based
  on `max_spatial_derivative_order`.
- `BoundaryConditions` provides periodic, static, Sommerfeld, reflective,
  extrapolating, and mixed boundary handling plus variable parity tables.
- `AMRInterpolator`, `Derivative`, `InterpolationQuery`, and `Lagrange<4>`
  provide grid interpolation and local derivative queries for diagnostic
  surfaces.
- `SurfaceExtraction` and `CylindricalExtraction` provide interpolation and
  integration on parameterized surfaces.
- Stage 4AO-B provides local validation stencils:
  second-order radial `d1/d2`, periodic `periodic_dz/periodic_dzz`, Fourier
  projection amplitudes, and a z-coupled parity leakage check.

Reuse status:

- GRChombo derivative and boundary classes are reusable with adapters if the
  4AO-C operator is built on GRChombo grid/ghost infrastructure. They are not
  directly usable in the current standalone local spectral note.
- Stage 4AO-B's small stencils are directly reusable only for validation
  scaffolding and regression tests. They must not be mistaken for the final
  actual frozen-gauge operator unless the Stage 4AO-C boundary and derivative
  contract intentionally adopts them.
- No Fourier utility, FFT wrapper, spectral basis class, or mode-decomposition
  helper exists beyond the hand-coded Stage 4AO-B cosine/sine projections.

### Frozen-Gauge CCZ4 Operator Coverage

The current live BlackStringToy RHS path is not the desired frozen-gauge
operator. `BlackStringToyLevel::specificEvalRHS` calls inherited
`CCZ4RHS<MovingPunctureGauge, FourthOrderDerivatives>` or
`CCZ4RHS<MovingPunctureGauge, SixthOrderDerivatives>`, then the Stage 4D
smoke-only scaffold may zero hidden RHS slots. This is not a repo-owned
modified-cartoon CCZ4 RHS and cannot be used as the GL operator without
adding the hidden-sector equations.

For the frozen-gauge state vector:

```text
chi;
h_xx, h_xz, h_zz, h_ww;
K;
A_xx, A_xz, A_zz, A_ww;
Theta;
hat_Gamma^x, hat_Gamma^z,
```

the current reuse map is:

| Block | Existing support | Reuse status for 4AO-C |
| --- | --- | --- |
| `chi` RHS | Public `CCZ4RHS::rhs_equation` has the visible conformal-factor equation; Stage 4AO-A/4AO-B record the GP residual target. | Reuse with adapter only. Needs modified-cartoon shift divergence and hidden multiplicity in a repo-owned operator. |
| visible `h_xx,h_xz,h_zz` RHS | Public `CCZ4RHS` has visible metric equations for `CH_SPACEDIM` components. | Reuse with adapter. Must freeze gauge perturbations and include the locked 4D trace convention. |
| hidden `h_ww` RHS | Stage 4AO-A/B derive and test representative GP residual behavior. Live code only freezes/zeros smoke hidden slots. | Missing as an actual nonlinear RHS block. Must be built before full JVP. |
| `K` RHS | Public `CCZ4RHS` has a visible CCZ4 `K` equation using Ricci scalar and lapse Hessian trace. Stage 4AO-C implements `output[K] += 3 lambda delta K - 3 lambda delta Theta`, `output[K] += delta R`, and the locked main-path damping insertion `output[K] += -0.4 deltaTheta`. | Partial. The selected `USE_CCZ4` `K(K-2Theta)` linearization, physical-`delta R`, and simple kappa damping insertion are implemented. Z/hat-Gamma-dependent Ricci contributions, remaining constraint terms, hat-Gamma evolution, and the rest of the coupled operator remain missing. Frozen-gauge lapse-Hessian variation vanishes; cosmological terms remain absent under the locked `Lambda=0` assumption. |
| visible `A_xx,A_xz,A_zz` RHS | Stage 4AL implements local trace-free curvature/lapse source; public `CCZ4RHS` has visible nonlinear/advection terms. Stage 4AO-C now inserts `[delta R_IJ]^TF` for the visible A outputs from the validated Ricci assembly. | Partial. The A_IJ trace-free Ricci curvature insertion is implemented, but the full operator remains incomplete because remaining Z4/kappa and hatted-Gamma terms, full-operator JVP/parity, boundary validation, MOTS, eigensolver/convergence, and the `k_c r0` map are still missing. |
| hidden `A_ww` RHS | Stage 4AK implements `-D_wD_w alpha + alpha R_ww[gamma]`; Stage 4AL includes trace-free projection. Stage 4AO-C now inserts the representative `[delta R_ww]^TF` component with hidden multiplicity already accounted for in the trace assembly. | Partial. The A_ww trace-free Ricci curvature insertion is implemented, but the full operator remains incomplete because remaining Z4/kappa and hatted-Gamma terms, full-operator JVP/parity, boundary validation, MOTS, eigensolver/convergence, and the `k_c r0` map are still missing. |
| `Theta` RHS | Public `CCZ4RHS` has visible CCZ4 formula. Stage 4AO-A locks background residual. Stage 4AO-C inserts `output[Theta] += 0.5 delta R` and the locked main-path damping term `output[Theta] += -0.25 deltaTheta`. | Partial. The Ricci scalar and simple kappa damping insertions are implemented. Remaining CCZ4/Z4 constraint terms and the complete hatted-Gamma evolution remain missing, along with full-operator JVP/parity, boundary validation, MOTS, eigensolver/convergence, and the `k_c r0` map. |
| `hat_Gamma^x` value | Stage 4AN implements local nonlinear `tilde_Gamma^x + 2 Z_over_chi^x`; Stage 4AO-C now implements the frozen-GP linearized `g_x` and encoded-Z reconstruction helper. | Reusable helper for local contraction/JVP checks. Not a RHS. |
| `hat_Gamma^x` RHS | Stage 4AM/4AO-A derive the hidden RHS cancellation on the GP background; Stage 4AO-B tests a representative `delta hww` contribution. Stage 4AO-C now inserts `(3 lambda/4)g_x - 0.2 Z_x + (lambda/2)H_x` without duplicating common advection. | Partial. Needs connection-A, K/Theta/chi gradients, hidden shift-Hessian/vector-Hessian terms, and complete row assembly. |
| `hat_Gamma^z` value/RHS | Public `CCZ4RHS` has the visible RHS formula; Stage 4AO-C implements the frozen-GP linearized full hidden-aware `g_z`, encoded-Z reconstruction, and `(3 lambda/4)g_z - 0.2 Z_z`. | Partial. There is deliberately no `+(lambda/2)H_z`; connection-A, K/Theta/chi gradients, vector-Hessian terms, and complete row assembly remain missing. |
| Physical `R_ww[gamma]` | Stage 4AH composes Stage 4AC and 4AE; Stage 4AF identity gate exists. | Reuse with adapter for hidden Ricci source terms. Local helper only, not grid/RHS operator. |
| full physical Ricci scalar | Stage 4AI gives hidden scalar contribution `chi 2 h^ww R_ww`; Stage 4G/4I visible Ricci bridge exists; Stage 4AO-C assembles `delta R = delta R_xx + delta R_zz + 2 delta R_ww`. | Implemented for the locked-background physical linearized trace and inserted into the K and Theta rows; the trace-free projection is inserted into A. Z/hat-Gamma-dependent Ricci contributions and the complete modified-cartoon RHS remain missing. |

Therefore a finite-difference JVP of the current live nonlinear RHS would test
the inherited public visible CCZ4 path plus smoke-hidden behavior, not the
frozen-gauge modified-cartoon GL operator. Stage 4AO-C must first build a
validation-only operator wrapper for the complete state vector.

### Diagnostics

Reusable diagnostics:

- `AMRReductions` wraps Chombo `computeNorm` and `computeSum` for AMR
  grid-level norms and integrals.
- `SmallDataIO` / `SmallDataIOReader` provide ASCII diagnostic output and
  reading.
- `ApparentHorizon` records convergence and residual statistics for nonlinear
  MOTS solves.
- `SurfaceExtraction` / `CylindricalExtraction` can extract and integrate
  fields on surfaces after an adapter selects the correct geometry.
- Stage 4AO-B has standalone norm-like max-error checks, epsilon-plateau
  reporting, and cosine/sine leakage projections.

Missing diagnostics:

- no reusable Fourier-mode decomposition utility for compact `z`;
- no spectral residual norm or eigenpair residual helper;
- no radial/boundary convergence harness for a frozen-gauge spectral operator;
- no linearized-MOTS `delta R_H` diagnostic;
- no primary-source convention-map checker for `k_c r0`.

### Recommendation Table

| Category | Items | Decision |
| --- | --- | --- |
| Reuse directly | Stage 4AO-B local periodic z stencil/projection code for small validation regressions; `SmallDataIO` for future ASCII diagnostics; Stage 4AC/4AE/4AH/4AL/4AN local formula oracles. | Directly reusable for tests and diagnostics, not sufficient for the spectral gate. |
| Reuse with adapter | PETSc communicator/KSP concepts from `PETScAHSolver`; AHFinder nonlinear MOTS solve; `AHStringGeometry` pattern; `ExpansionFunction`; `AMRInterpolator`/`Lagrange<4>`; `FourthOrderDerivatives`/`SixthOrderDerivatives`; `BoundaryConditions`; `AMRReductions`; `SurfaceExtraction`/`CylindricalExtraction`; public `CCZ4RHS` formula layout. | Useful, but each needs a 4AO-C-specific adapter and convention lock. |
| Do not use as-is | AHFinder `SNES` solve as an eigensolver; `ChiContourFunction` or lapse contours as the horizon observable; current `BlackStringToyLevel::specificEvalRHS` as the modified-cartoon operator; Stage 4AO-B representative parity/JVP harness as the actual frozen-gauge operator; `hat_Gamma^x` alone as the GL observable. | Would give false confidence if used directly. |
| Missing and must be built | Complete frozen-gauge modified-cartoon RHS operator; radial spectral boundary-condition contract; linearized-MOTS map to `delta R_H`; eigenvalue/shift-invert adapter, likely PETSc/SLEPc or a documented standalone solver; Fourier/mode utilities; primary-source `k_c r0` convention map; eigenpair residual and convergence diagnostics. | Hard blockers before Stage 4AO-C can pass. |

Next implementation order for 4AO-C should be:

1. extend the validation-only wrapper beyond the current GP-shift advection
   block into actual frozen-gauge RHS blocks;
2. add actual-operator JVP and parity checks reusing the 4AO-B lessons;
3. add the linearized-MOTS / `delta R_H` adapter;
4. only then add an eigensolver or shift-invert adapter.

Starting with an eigensolver would be premature because there is no complete
operator or observable for it to validate.

The first item is now represented by
`code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` with fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`. The
wrapper began as a contract and inventory and now also contains eleven actual
partial apply paths: `beta_GP^x d_x(delta u)` for every frozen-gauge
perturbation variable, the non-advection tensor shift-stretching terms for
`h_IJ` and `A_IJ`, and the algebraic couplings
`delta h_IJ <- -2 delta A_IJ`, `delta chi <- +delta K/2`, plus the K-output
selected-CCZ4 linearization `3 lambda delta K - 3 lambda delta Theta` and
physical-Ricci insertion `+delta R`, plus the K/Theta-output simple
locked-convention damping insertion `output[K] += -0.4 deltaTheta`,
`output[Theta] += -0.25 deltaTheta`, plus the `A_IJ`-output
linearization of `(K - 2 Theta) A_IJ - 2 h^KL A_IK A_LJ`, plus the
Theta-output algebraic linearization of
`0.5 * (((d - 1) / d) K^2 - A_IJ A^IJ)`, plus the Theta-output
`-K_GP deltaTheta` block, plus the Theta-output Ricci scalar insertion
`+0.5 delta R`, plus the `A_IJ`-output trace-free Ricci curvature insertion
`[delta R_IJ]^TF`. It also contains the structural trace-free `delta A`
projector contract for future assembled operator outputs. It records
the frozen-gauge state, excludes gauge perturbations, labels existing helper
coverage versus missing RHS blocks, and defines the radial-domain/boundary
contract. It deliberately returns
`complete_frozen_gauge_operator_implemented = false` and is not a complete
differential operator.

## Validation-Only Operator Wrapper Contract

The Stage 4AO-C wrapper state vector is exactly

```text
chi;
h_xx, h_xz, h_zz, h_ww;
K;
A_xx, A_xz, A_zz, A_ww;
Theta;
hat_Gamma^x, hat_Gamma^z.
```

Frozen gauge means the following perturbations are excluded from the operator
state:

```text
delta alpha = 0,
delta beta^x = delta beta^z = 0,
delta B^x = delta B^z = 0.
```

The wrapper distinguishes four categories:

- **contract only**: state-vector layout, gauge exclusions, inventory labels,
  and boundary contract are implemented now;
- **implemented partial blocks**: the GP-shift advection term
  `beta_GP^x d_x(delta u)` is implemented for all 13 variables, tensor
  shift-stretching is implemented for the `h_IJ` and `A_IJ` slots only, and
  the algebraic couplings `delta h_IJ <- -2 delta A_IJ` and
  `delta chi <- +delta K/2` are implemented only for their named output
  slots; the K-equation selected-CCZ4 `K(K-2Theta)` linearization and
  physical-Ricci scalar insertion are implemented only as K-output blocks;
  the locked main-path CCZ4 damping insertion is implemented only for the K
  and Theta output slots and depends only on `deltaTheta`; the first
  hatted-Gamma non-advection Z/kappa plus `kappa3` shift-gradient insertion is
  implemented only for the hatted-Gamma output slots;
  the rejected BSSN `A^2+K^2/d` terms are absent; the `A_IJ`-equation non-curvature algebraic
  linearization is implemented only for the `A_IJ` output slots; the
  Theta-equation non-Ricci algebraic linearization is implemented only for the
  `Theta` output slot; the frozen-GP `-K_GP deltaTheta` linearization is
  implemented only for the `Theta` output slot; the Theta Ricci scalar insertion
  is implemented only for the `Theta` output slot; the trace-free Ricci
  curvature insertion is implemented only for the `A_IJ` output slots;
- **helper only**: local helper layers exist and may be reused later, but they
  are not assembled into the coupled frozen-gauge RHS;
- **requires modified-cartoon RHS**: the actual operator block is missing and
  must come from the future repo-owned modified-cartoon CCZ4 RHS;
- **missing placeholder**: no sufficient helper or RHS block exists yet.

The current RHS inventory is:

| Variable | Kinematic/algebraic local pieces | Hidden/cartoon `1/x` terms | Ricci/curvature terms | `K/A` trace or trace-free terms | `Theta`/constraint terms | `hat_Gamma` hidden/evolution terms |
| --- | --- | --- | --- | --- | --- | --- |
| `chi` | GP-shift advection and `+delta K/2` implemented; tensor stretching N/A; other derivative terms helper/missing | requires modified-cartoon RHS | N/A | remaining K/chi algebraic terms require modified-cartoon RHS | N/A | N/A |
| `h_xx` | GP-shift advection, tensor stretching, and `-2 delta A_xx` implemented; other derivative terms helper/missing | requires modified-cartoon RHS | N/A | remaining metric/K/A algebraic terms require modified-cartoon RHS | N/A | N/A |
| `h_xz` | GP-shift advection, tensor stretching, and `-2 delta A_xz` implemented; other derivative terms helper/missing | requires modified-cartoon RHS | N/A | remaining metric/K/A algebraic terms require modified-cartoon RHS | N/A | N/A |
| `h_zz` | GP-shift advection, tensor stretching, and `-2 delta A_zz` implemented; other derivative terms helper/missing | requires modified-cartoon RHS | N/A | remaining metric/K/A algebraic terms require modified-cartoon RHS | N/A | N/A |
| `h_ww` | GP-shift advection, hidden tensor stretching, and `-2 delta A_ww` implemented; other derivative terms helper/missing | helper only from 4AO-A/B hidden-contraction lessons | N/A | remaining hidden metric/K/A algebraic terms require modified-cartoon RHS | N/A | N/A |
| `K` | GP-shift advection and K-output selected-CCZ4 `+3 lambda delta K - 3 lambda delta Theta` implemented; tensor stretching N/A; other derivative terms helper/missing | requires modified-cartoon RHS | physical `output[K] += delta R` implemented from `delta R_xx + delta R_zz + 2 delta R_ww`; Z/hat-Gamma-dependent Ricci pieces missing | simple main-path kappa damping `output[K] += -0.4 deltaTheta` implemented; remaining K dynamics require modified-cartoon RHS; frozen-gauge lapse Hessian vanishes; `Lambda=0` cosmological terms remain absent | direct `delta Theta` coefficients are split between `K(K-2Theta)` and the simple damping insertion; remaining selected constraint terms are missing | N/A |
| `A_xx` | GP-shift advection, tensor stretching, and A-output non-curvature algebraic linearization implemented; other derivative terms helper/missing | requires modified-cartoon RHS | raw visible physical `delta R_xx[gamma]` validation block implemented; `[delta R_xx]^TF` curvature insertion implemented | remaining A dynamics require modified-cartoon RHS | A-output block includes direct `Theta` coefficient only; full constraint terms missing | N/A |
| `A_xz` | GP-shift advection, tensor stretching, and A-output non-curvature algebraic linearization implemented; other derivative terms helper/missing | requires modified-cartoon RHS | raw visible physical `delta R_xz[gamma]` validation block implemented; `[delta R_xz]^TF` curvature insertion implemented | remaining A dynamics require modified-cartoon RHS | no direct `Theta` coefficient because `A_xz_GP=0`; full constraint terms missing | N/A |
| `A_zz` | GP-shift advection, tensor stretching, and A-output non-curvature algebraic linearization implemented; other derivative terms helper/missing | requires modified-cartoon RHS | raw visible physical `delta R_zz[gamma]` validation block implemented; `[delta R_zz]^TF` curvature insertion implemented | remaining A dynamics require modified-cartoon RHS | A-output block includes direct `Theta` coefficient only; full constraint terms missing | N/A |
| `A_ww` | GP-shift advection, hidden tensor stretching, and A-output non-curvature algebraic linearization implemented; other derivative terms helper/missing | helper only from 4AH-4AL hidden local blocks | raw hidden physical `delta R_ww[gamma]` validation block implemented; `[delta R_ww]^TF` curvature insertion implemented with no extra hidden multiplicity | remaining A dynamics require modified-cartoon RHS | A-output block includes direct `Theta` coefficient only; full constraint terms missing | N/A |
| `Theta` | GP-shift advection, Theta-output non-Ricci algebraic linearization, and `-K_GP deltaTheta` implemented; tensor stretching N/A; other derivative terms helper/missing | requires modified-cartoon RHS | `output[Theta] += 0.5 delta R` implemented from validated raw Ricci trace assembly | N/A | simple main-path kappa damping `output[Theta] += -0.25 deltaTheta` implemented; `Z dot grad alpha` has no frozen-GP linear contribution; other selected constraint terms remain missing | N/A |
| `hat_Gamma^x` | GP-shift advection implemented once in the common block; `(3 lambda/4)g_x - 0.2 Z_x + (lambda/2)H_x` implemented in a separate non-advection block | linearized `g_x` and `Z_x` reconstruction helper implemented and checked against 4AN | N/A | N/A | K/Theta/chi gradients remain missing | first Z/kappa and `kappa3` shift-gradient block implemented; connection-A, vector/shift-Hessian, and complete RHS missing |
| `hat_Gamma^z` | GP-shift advection implemented once in the common block; `(3 lambda/4)g_z - 0.2 Z_z` implemented separately, with no x-only `H` coefficient | linearized full hidden-aware `g_z` and `Z_z` reconstruction helper implemented | N/A | N/A | K/Theta/chi gradients remain missing | first Z/kappa block implemented; connection-A, vector-Hessian, and complete RHS missing |

No variable has a complete RHS in the wrapper. GP-shift advection, tensor
shift-stretching, the algebraic metric/chi couplings, the K-output selected-CCZ4
`K(K-2Theta)` linearization and physical-`delta R` insertion, and the `A_IJ`-output non-curvature algebraic
linearization, the Theta-output algebraic non-Ricci linearization, the Theta
`-K_GP deltaTheta` linearization, the Theta Ricci scalar insertion, and the
simple K/Theta kappa damping insertion, and the A_IJ trace-free Ricci curvature
insertion, and the first hatted-Gamma Z/kappa/shift-gradient insertion are
implemented operator pieces. The
raw physical `delta R_ww[gamma]`, `delta R_xz[gamma]`, `delta R_zz[gamma]`,
and `delta R_xx[gamma]` blocks are standalone Ricci validation helpers; their
raw scalar trace and trace-free Ricci projection are assembled before those
two insertion blocks consume them. The wrapper still cannot be used for full
JVP, matrix assembly, spectral extraction, or threshold searches until the
remaining hatted-Gamma connection-A, gradient, Hessian, and row-assembly
pieces, coupled CCZ4/Z4, boundary, MOTS, convergence, and convention-map
blockers are resolved.

## First Actual Operator Block: GP-Shift Advection

The implemented partial block is the frozen-background shift advection term

```text
L_adv[delta u] = beta_GP^x d_x(delta u),
beta_GP^x = sqrt(r0 / x),
```

applied slot-by-slot to the 13-variable frozen-gauge perturbation vector. It
does not include Ricci/curvature terms, hidden/cartoon `1/x` sources, K/A
couplings, Theta/constraint couplings, hatted-Gamma evolution terms, boundary
conditions, gauge perturbations, or any spectral machinery.

Derivative infrastructure in
`code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp` is deliberately local
and validation-only:

- radial `d_x` is a second-order centered finite difference on interior points
  of `x in [x_in,x_out]`;
- endpoint outputs are zero placeholders and are explicitly marked as not
  boundary validation;
- periodic `z` helpers provide second-order centered `D_z` and `D_zz` for
  shared scaffolding. The GP-shift advection block itself does not use them;
  later Ricci validation blocks exercise z derivatives without making the full
  frozen-gauge operator complete.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeAdvectionBlockTest.cpp` checks
the exact 13-slot ordering, gauge exclusion, the advection action on a
quadratic radial profile, consistent application to every variable, zero input
to zero output, non-aggregate packages, `complete_operator=false`, and
`eigensolver_allowed=false`. Negative guards show that a wrong beta profile,
wrong derivative sign, or wrong slot ordering would fail the test.

## Tensor Shift-Stretching Block

GRChombo provides the convention target in
`external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: the metric RHS adds
`h_ki partial_j beta^k + h_kj partial_i beta^k
- (2/GR_SPACEDIM) h_ij div beta`, and the `A_ij` RHS uses the analogous
tensor stretch terms alongside separate algebraic, curvature, and nonlinear
terms. This Stage 4AO-C code is only a validation-only reduced operator
piece; production code should later reuse or adapt GRChombo infrastructure
where possible.

For the locked GP background,

```text
beta^x = sqrt(r0/x),
lambda = sqrt(r0/x^3),
partial_x beta^x = -lambda/2,
beta^x/x = lambda,
div beta = partial_x beta^x + 2 beta^x/x = 3 lambda/2,
2/d = 1/2.
```

The implemented non-advection tensor-stretching coefficients are:

```text
h_xx, A_xx: -7 lambda / 4,
h_xz, A_xz: -5 lambda / 4,
h_zz, A_zz: -3 lambda / 4,
h_ww, A_ww: +5 lambda / 4.
```

The hidden `ww` coefficient includes the cartoon stretch
`+2 delta h_ww beta^x/x` or `+2 delta A_ww beta^x/x`, followed by the same
`-1/2 div beta` trace removal. Scalars `chi`, `K`, `Theta` receive no tensor
stretching. `hat_Gamma^x` and `hat_Gamma^z` shift/vector-density terms are
not part of this substep and remain owned by a future hatted-Gamma evolution
block.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeTensorShiftStretchingBlockTest.cpp`
checks these coefficients for pointwise and grid apply paths, confirms the
block acts only on the `h_IJ` and `A_IJ` slots, and keeps
`complete_operator=false` and `eigensolver_allowed=false`. Negative guards
show that dropping hidden `h_ww/A_ww` stretching, using a `d=3` trace
coefficient, flipping the sign of `partial_x beta^x`, or applying the block
to scalar/hatted-Gamma slots would fail.

## Algebraic Metric And Chi Coupling Block

GRChombo provides the convention target in
`external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: the metric RHS contains
`-2 alpha A_ij`, and the conformal-factor RHS contains
`(2/GR_SPACEDIM) chi (alpha K - div beta)`. This Stage 4AO-C code is only a
validation-only reduced operator block; production code should later reuse or
adapt GRChombo infrastructure where possible.

On the locked GP background used by the frozen-gauge validation harness,

```text
d = 4,
alpha_GP = 1,
chi_GP = 1,
K_GP = div beta_GP.
```

Therefore the implemented local linearized couplings are exactly

```text
delta h_xx <- -2 delta A_xx,
delta h_xz <- -2 delta A_xz,
delta h_zz <- -2 delta A_zz,
delta h_ww <- -2 delta A_ww,
delta chi  <- +(1/2) delta K.
```

No reciprocal `A_IJ <- h_IJ` coupling is part of this block. `K`, `Theta`,
the `A_IJ` outputs, and `hat_Gamma^x/z` receive zero from this block.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeAlgebraicCouplingBlockTest.cpp`
checks pointwise and grid apply paths, exact state-vector ordering, the four
metric outputs, the `d=4` chi coefficient, zero output for the excluded
slots, and the same incomplete-operator/eigensolver guards. Negative guards
show that the wrong sign in `-2 delta A_IJ`, a `-1` metric coefficient, a
`d=3` chi coefficient `2/3`, reciprocal `A_IJ <- h_IJ`, or accidental
application to `K`, `Theta`, or hatted-Gamma slots would fail.

## Selected-CCZ4 K-Equation Pieces

The earlier Stage 4AO-C K block followed GRChombo's `USE_BSSN` branch,
`A_IJ A^IJ + K^2/d`. That branch was wrong for this project and is explicitly
rejected. Its inverse-metric, `delta A_IJ`, and `K^2/d` coefficients have been
removed from the K row; they remain historical evidence only and are not an
implemented CCZ4 claim.

The locked formulation is `USE_CCZ4`. Separating damping, lapse-Hessian,
cosmological, advection, and Z-dependent pieces, its non-damping local target is

```text
F_K = R + K(K - 2Theta).
```

At

```text
K_GP = 3 lambda / 2,
Theta_GP = 0,
alpha_GP = 1,
delta alpha = 0,
```

the implemented algebraic linearization is exactly

```text
output[K] +=  3 lambda * input[K],
output[K] += -3 lambda * input[Theta].
```

There is no direct `delta A_IJ`, `delta h_IJ`, or `delta chi` contribution in
this piece. The separate physical-Ricci insertion is

```text
output[K] += delta R,
delta R = delta R_xx + delta R_zz + 2 delta R_ww.
```

The common GP-shift advection block already supplies
`beta_GP^x d_x(delta K)` and is not duplicated. Still separate and missing are
Z/hat-Gamma-dependent Ricci contributions, kappa damping, and hat-Gamma
evolution. The lapse-Hessian variation vanishes in frozen gauge. Cosmological
terms are absent under the still-locked `Lambda=0` assumption.

`code/BlackStringToy/tests/Stage4AOCFrozenGaugeKCCZ4BlockTest.cpp` constructs
an independent branch-sensitive oracle by central-differencing the nonlinear
`R + K(K-2Theta)` expression. `R` comes directly from the Stage 4G nonlinear
physical Ricci engine and its nonlinear physical inverse-metric trace; `K` and
`Theta` are perturbed independently around the GP background. The epsilon
sweep is `1e-2`, `1e-4`, `1e-5`, `1e-6`, and `1e-7`, with the `1e-5` to
`1e-6` plateau required below `2e-7`.

Branch-discriminating cases cover pure `delta K`, pure `delta Theta`, pure
`delta A`, metric/chi perturbations, a pure `delta h_xz` jet, and a combined
perturbation. The test also central-differences the rejected nonlinear BSSN
expression as a negative branch oracle and requires every case to distinguish
the selected CCZ4 implementation from that former row. The K RHS, coupled
operator, and eigensolver gate remain incomplete.

## A-Equation Algebraic Non-Curvature Block

GRChombo provides the convention target in
`external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: after the trace-free
curvature/lapse block `Adot_TF`, the `A_ij` RHS contains

```text
A_ij (alpha (K - 2 Theta) - (2 / d) div beta)
+ A_ki partial_j beta^k + A_kj partial_i beta^k
- 2 alpha h^kl A_ik A_lj.
```

The shift-stretching and `-(2/d) A_ij div beta` part is owned by the tensor
shift-stretching block above. This Stage 4AO-C substep implements only the
local non-curvature algebraic part

```text
(K - 2 Theta) A_IJ - 2 h^KL A_IK A_LJ,
```

linearized about the locked GP background. The mixed-index contraction raises
with the conformal inverse metric, matching GRChombo's use of `h_UU`.
The separate trace-free physical-Ricci insertion is now implemented, and the
frozen-gauge lapse-Hessian variation vanishes; neither belongs to this
non-curvature block.

On the locked GP background,

```text
alpha_GP = 1,
Theta_GP = 0,
K_GP = 3 lambda / 2,
A_xx = -7 lambda / 8,
A_xz = 0,
A_zz = -3 lambda / 8,
A_ww = +5 lambda / 8.
```

The implemented A-output coefficients are:

```text
delta A_xx_RHS +=
  (49/32) lambda^2 delta h_xx
+ 5 lambda delta A_xx
- (7/8) lambda delta K
+ (7/4) lambda delta Theta,

delta A_xz_RHS +=
  (21/32) lambda^2 delta h_xz
+ 4 lambda delta A_xz,

delta A_zz_RHS +=
  (9/32) lambda^2 delta h_zz
+ 3 lambda delta A_zz
- (3/8) lambda delta K
+ (3/4) lambda delta Theta,

delta A_ww_RHS +=
  (25/32) lambda^2 delta h_ww
- lambda delta A_ww
+ (5/8) lambda delta K
- (5/4) lambda delta Theta.
```

The `delta h_IJ` terms are the inverse-metric variation in
`h^KL A_IK A_LJ`. No linear `delta K` or `delta Theta` term appears in
`A_xz` because `A_xz_GP=0`. The componentwise `A_ww` equation is one hidden
component equation and does not receive an extra hidden-multiplicity factor in
this local block. No non-A output slot receives contributions from this
block.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeAAlgebraicBlockTest.cpp`
checks every coefficient by basis vector, verifies the combined pointwise and
grid apply paths, confirms `A_xz` has no `K` or `Theta` contribution, and
keeps `complete_operator=false` and `eigensolver_allowed=false`. Negative
guards show that dropping inverse-metric variation, incorrectly multiplying
the `A_ww` component equation by hidden multiplicity, using wrong `K` or
`Theta` coefficients, adding spurious `A_xz` couplings, or touching non-A
outputs would fail.

## Theta-Equation Algebraic Non-Ricci Block

GRChombo provides the convention target in
`external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`: in the CCZ4 branch the
Theta RHS contains

```text
0.5 alpha (
    R
  - A_IJ A^IJ
  + ((d - 1) / d) K^2
  - 2 Theta K
)
- 0.5 Theta kappa1 alpha ((d + 1) + kappa2 (d - 1))
- Z^i alpha_i
```

plus the cosmological term if configured. This Stage 4AO-C substep implements
only the non-Ricci algebraic part

```text
0.5 * (((d - 1) / d) K^2 - A_IJ A^IJ),
```

linearized about the locked GP background. The Ricci scalar, simple kappa
damping, `Z dot grad alpha`, and cosmological pieces are not included in this
algebraic block: Ricci and damping are separate implemented insertions,
`Z dot grad alpha` has no frozen-GP linear contribution, and locked
`Lambda=0` leaves no cosmological term. The simple `-Theta K` term is
implemented separately below.

On the locked GP background,

```text
alpha_GP = 1,
d = 4,
K_GP = 3 lambda / 2,
A_xx = -7 lambda / 8,
A_xz = 0,
A_zz = -3 lambda / 8,
A_ww = +5 lambda / 8.
```

The implemented Theta-output coefficients are:

```text
delta Theta_RHS +=
  (49/64) lambda^2 delta h_xx
+ ( 9/64) lambda^2 delta h_zz
+ (25/32) lambda^2 delta h_ww
+ ( 7/8 ) lambda   delta A_xx
+ ( 3/8 ) lambda   delta A_zz
- ( 5/4 ) lambda   delta A_ww
+ ( 9/8 ) lambda   delta K.
```

The `delta h_IJ` terms are the inverse-metric variation in the trace
`A_IJ A^IJ`. The `ww` terms include hidden trace multiplicity two because
`A_IJ A^IJ` is a full 4D trace. No linear `delta h_xz` or `delta A_xz`
appears because the locked GP background has diagonal `A_IJ` and `A_xz=0`.
No non-Theta output slot receives contributions from this block.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaAlgebraicBlockTest.cpp`
checks every coefficient by basis vector, verifies the combined pointwise and
grid apply paths, confirms `h_xz` and `A_xz` give zero, and keeps
`complete_operator=false` and `eigensolver_allowed=false`. Negative guards
show that dropping inverse-metric variation, dropping hidden `ww`
multiplicity, using the `d=3` K coefficient, adding spurious `h_xz/A_xz`
terms, or touching non-Theta outputs would fail.

## Theta-Equation `-K_GP deltaTheta` Block

The same GRChombo Theta RHS contains the non-damping algebraic factor

```text
0.5 * (-2 Theta K) = -Theta K.
```

This Stage 4AO-C substep implements only the frozen-GP linearization of that
factor. On the locked background,

```text
alpha_GP = 1,
Theta_GP = 0,
K_GP = 3 lambda / 2,
```

so

```text
delta(-Theta K) =
    -K_GP deltaTheta - Theta_GP deltaK
  = -(3 lambda / 2) deltaTheta.
```

No `delta K` contribution is present in this block because `Theta_GP=0`. The
now-locked kappa damping terms remain a separate insertion and are not bundled
into this simple algebraic block.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaMinusKBlockTest.cpp`
checks the coefficient `-(3/2)lambda`, confirms that `delta K` and all other
inputs give zero, verifies the grid apply path, and keeps
`complete_operator=false` and `eigensolver_allowed=false`. Negative guards
show that a wrong sign, a `-lambda` coefficient, a spurious `delta K`
contribution, or touching non-Theta outputs would fail.

## Trace-Free `delta A` Subspace And Projector Contract

GRChombo removes lower-index tensor traces with
`TensorAlgebra::make_trace_free(tensor, metric, inverse_metric)`, which
computes the trace using the supplied inverse metric and subtracts
`metric[i][j] trace / GR_SPACEDIM`. In `CCZ4RHS.impl.hpp` this is applied to
the curvature/lapse `Adot_TF` block before the remaining `A_ij` RHS pieces are
added. Initial-data paths also use the same helper when converting physical
extrinsic curvature into trace-free `A_ij`.

This Stage 4AO-C contract is validation-only. The production path should later
reuse or adapt GRChombo trace-removal infrastructure where possible. The
frozen-gauge state vector remains the existing 13 variables; no component is
removed from the vector.

On the locked diagonal GP background with hidden multiplicity two,

```text
delta_tr_A =
    delta A_xx
  + delta A_zz
  + 2 delta A_ww
  - A_xx_GP delta h_xx
  - A_zz_GP delta h_zz
  - 2 A_ww_GP delta h_ww.
```

Using

```text
A_xx_GP = -7 lambda / 8,
A_zz_GP = -3 lambda / 8,
A_ww_GP = +5 lambda / 8,
```

the locked formula is

```text
delta_tr_A =
    delta A_xx
  + delta A_zz
  + 2 delta A_ww
  + (7 lambda / 8) delta h_xx
  + (3 lambda / 8) delta h_zz
  - (5 lambda / 4) delta h_ww.
```

No `delta h_xz` or `delta A_xz` term appears because the GP conformal metric
and `A_IJ` background are diagonal.

The validation projector keeps all non-A variables and `A_xz` unchanged, and
subtracts the trace error from the diagonal A components with the GRChombo
`d=4` weights at the locked flat conformal background:

```text
delta A_xx <- delta A_xx - delta_tr_A / 4,
delta A_zz <- delta A_zz - delta_tr_A / 4,
delta A_ww <- delta A_ww - delta_tr_A / 4.
```

The representative `A_ww` component receives the same component subtraction as
`A_xx` and `A_zz`; hidden multiplicity two is already in `delta_tr_A`. This
projector is required before any assembled full-operator JVP, parity, or
spectral claim. It does not make the frozen-gauge operator complete.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeTraceFreeDeltaAProjectorTest.cpp`
checks the trace coefficients, hidden `ww` multiplicity two, zero `h_xz/A_xz`
trace contribution, trace removal on arbitrary data, `A_xz` preservation,
idempotence, and unchanged already trace-free data. Negative guards show that
wrong hidden multiplicity, dropping metric-variation terms, a `d=3` projection
weight, or accidental non-A modification would fail.

## Linearized Ricci/Curvature Design Preflight

This preflight locks the design and oracle plan for the frozen-gauge Ricci and
curvature blocks. It adds no `delta R_IJ` implementation and no spectral
machinery.

### Ricci Convention And Reuse Inventory

GRChombo's production Ricci convention is in
`external/GRChombo/Source/CCZ4/CCZ4Geometry.hpp`. `compute_ricci_Z` builds a
Gamma-form Ricci component with lower indices, combines it with the
conformal-factor correction, adds the `Z` terms, and returns

```text
R_IJ[gamma] = (ricci_chi + chi ricci_hat + z_terms) / chi.
```

`compute_ricci_Z_general` and `compute_ricci` provide related variants with
different `Z` treatment. The Stage 4AO-C frozen-gauge Ricci block must remain
compatible with this GRChombo-facing lower/lower physical Ricci convention,
even though the existing local oracle code often uses an independent
metric-derivative construction.

Reuse status:

| Piece | Decision | Notes |
| --- | --- | --- |
| `CCZ4Geometry::compute_ricci_Z`, `compute_ricci_Z_general`, `compute_ricci` | Reuse with adapter/convention map | Production convention authority for physical lower/lower Ricci and scalar; Gamma-form and hatted-Gamma/Z handling require a modified-cartoon adapter before use in the 4AO-C frozen operator. |
| Stage 4G `CartoonRicci::compute_metric_derivative_ricci` | Oracle-only / adapter candidate | Independent metric-derivative physical Ricci engine with flat, cone, nonconstant hidden-metric, and sheared-flat tests. It is away-axis local and not the GRChombo Gamma-form RHS path. Useful for finite-difference linearization or direct nonlinear oracle checks. |
| Stage 4W derivation and Stage 4Y/4Z/4AB/4AC conformal `R_ww` path | Reusable directly for hidden `tilde R_ww[h]` oracle pieces | Reviewed guarded conformal hidden Ricci decomposition with singular, gradient, Hessian, and assembly tests. It covers hidden `1/x` structure but only the `ww` conformal component. |
| Stage 4AE `R^chi_ww` and Stage 4AF split-vs-direct identity gate | Reusable directly for hidden physical `R_ww` oracle checks | Provides checked conformal-factor correction and internal physical split-vs-direct gate against Stage 4G. It is not a full visible Ricci linearization. |
| Stage 4AH/4AI physical `R_ww` assembly and contract | Helper-only for hidden component plumbing | Useful for same-point physical `R_ww` packaging and hidden trace contribution checks; Stage 4AO-C now finite-differences this split path as the secondary oracle for raw hidden `delta R_ww`. |
| Stage 4AL trace-free curvature/lapse block | Oracle-only / structural guide | Shows local trace-free curvature/lapse projection and Stage 4G-vs-4AH consistency checks. It includes physical lapse Hessians and is not frozen-gauge linearized operator code. |
| Stage 4AO-C trace-free `delta A` projector | Reusable directly for future assembled operator projection | Structural projector for the linearized trace-free A subspace; required before full-operator JVP/parity/spectral claims. |
| Stage 4AO-C raw hidden `delta R_ww[gamma]` block | Implemented standalone validation block | Validates the hidden physical lower/lower component around flat GP against Stage 4G and Stage 4AH finite-difference oracles; not visible Ricci, not trace-free projection, and not a full operator block. |
| Stage 4AO-C raw visible `delta R_xz[gamma]` block | Implemented standalone validation block | Validates the first one-z visible lower/lower Ricci component around flat GP against the Stage 4G finite-difference oracle. No Stage 4AC/4AE-style split oracle exists for visible `R_xz`, so Stage 4G is the primary and sole independent oracle for this substep. |
| Stage 4AO-C raw visible `delta R_zz[gamma]` block | Implemented standalone validation block | Validates the visible diagonal lower/lower Ricci component around flat GP against the Stage 4G finite-difference oracle. No Stage 4AC/4AE-style split oracle exists for visible `R_zz`, so Stage 4G is the primary and sole independent oracle for this substep. |
| Stage 4AO-C raw visible `delta R_xx[gamma]` block | Implemented standalone validation block | Validates the visible radial lower/lower Ricci component around flat GP against the Stage 4G finite-difference oracle. No Stage 4AC/4AE-style split oracle exists for visible `R_xx`, so Stage 4G is the primary and sole independent oracle for this substep. |
| Full frozen-gauge modified-cartoon `delta R_IJ` operator | Missing | Must still be built with radial derivatives, periodic z derivatives, hidden/cartoon `1/x` terms, conformal-factor terms, parity contracts, and boundary validation. |

### Frozen-Gauge Curvature Simplification

For Stage 4AO-C frozen gauge,

```text
delta alpha = 0,
alpha_GP = 1,
chi_GP = 1.
```

The `A_IJ` curvature/lapse source in GRChombo form is the trace-free part of

```text
-D_I D_J alpha + chi alpha R_IJ[gamma].
```

Since the GP background has constant `alpha=1` and frozen gauge perturbations
set `delta alpha=0`,

```text
delta(D_I D_J alpha)
  = D_I D_J(delta alpha)
    - delta Gamma^K_IJ partial_K alpha_GP
  = 0.
```

The first term vanishes because `delta alpha=0` in frozen gauge. The second
term vanishes because `alpha_GP=1` is constant, so
`partial_K alpha_GP=0`.

Therefore the missing frozen-gauge curvature/lapse block reduces to

```text
[delta R_IJ]^TF
```

with the project `d=4` trace convention and hidden multiplicity two. The
Stage 4AO-C trace-free `delta A` projector is a subspace contract for A
perturbations; the curvature block itself still needs the ordinary
trace-free projection of the Ricci source before it can contribute to the
future assembled operator.

### Ricci Decomposition Contract

The intended linearized Ricci decomposition is

```text
delta R_IJ =
    delta tilde R_IJ[delta h]
  + delta R^chi_IJ[delta chi, delta h]
  + hidden/cartoon terms.
```

Coverage must include visible `xx`, `xz`, `zz` and hidden `ww` lower/lower
components.

Required term classes:

- Radial derivatives: `d_x`, `d_xx`, and mixed `d_xz` contributions from
  conformal metric and conformal-factor perturbations.
- Periodic-z derivatives: `d_z`, `d_zz`, and `d_xz` contributions using the
  same z-stencil conventions as the future frozen-gauge operator.
- Hidden/cartoon `1/x` terms: hidden `ww` curvature, hidden contractions,
  `B/x`, `(A-W)/x^2`, `W_x/x`, `chi_x/x`, and related finite away-axis
  combinations. These remain away-axis only until Stage 4AP/4AQ.
- Conformal-factor terms: linearized `R^chi_IJ` terms from `chi_i`,
  `chi_ij`, the conformal Laplacian, and hidden `D_wD_w chi`, including the
  hidden multiplicity two in scalar traces.
- Parity behavior: scalar/zero-z/two-z outputs (`xx`, `zz`, `ww`, scalar
  trace pieces) remain in the even sector for even scalar input; one-z
  outputs (`xz`) carry the opposite sine/cosine parity. Hidden `B/x` and
  `chi_x/x`-type ingredients must use the checked/local quotient policy and
  still do not prove real grid regularity.
- Trace-free projection: the future `A_IJ` curvature block applies the
  `d=4` hidden-multiplicity trace-free projection after assembling
  `delta R_IJ`; it must not project individual Ricci subpieces in a way that
  hides cancellation errors unless a test explicitly proves equivalence.

### Independent Oracle Plan

The first implementation of any `delta R_IJ` block must be validated against
an oracle that is independent enough to catch signs, hidden multiplicities,
and `1/x` mistakes.

Primary oracle path:

1. Reuse Stage 4G's metric-derivative physical Ricci engine as the nonlinear
   independent oracle.
2. Build local GP background jets plus a perturbation jet.
3. Evaluate

```text
[R_IJ(U_GP + epsilon delta U) - R_IJ(U_GP - epsilon delta U)] / (2 epsilon)
```

with an epsilon plateau, using the Stage 4G engine or the Stage 4AC/4AE/4AH
hidden `R_ww` split where appropriate.

Secondary oracle path:

- For hidden `ww`, compare the finite-difference Stage 4G direct physical
  Ricci oracle against the reviewed Stage 4AC plus Stage 4AE split. Stage 4AF
  already proves this split-vs-direct identity at nonlinear sample points;
  the linearized test should inherit that same same-point product-rule
  discipline.
- For visible components, Stage 4G remains the best current local oracle.
  GRChombo `CCZ4Geometry` is the convention authority, but needs a
  modified-cartoon adapter before it can serve as the direct operator source.

The oracle set must include:

- at least one pure hidden `delta h_ww` mode to exercise `1/x`,
  `(A-W)/x^2`, and `W_x/x` structures;
- at least one off-diagonal `delta h_xz` mode to exercise `B/x`, parity, and
  `xz` coupling;
- at least one conformal-factor perturbation with nonzero `chi_x`,
  `chi_z`, and second derivatives;
- at least one mixed z-dependent mode with nonzero `d_z` and `d_zz`;
- at least one trace-free projection check after all components are assembled.

### Historical First Implementation Target

The hidden physical `delta R_ww[gamma]` block on the frozen GP background was
chosen as the first Ricci target.

Reasons:

- It has the highest hidden `1/x` exposure, so it attacks the hardest
  regularity risk early.
- Stage 4W-4AF already decomposed, implemented, and cross-checked the
  nonlinear hidden component through conformal `tilde R_ww`, `R^chi_ww`, and
  direct Stage 4G physical Ricci.
- Stage 4Y/4Z/4AB/4AC/4AE already isolate the relevant checked ingredients:
  `(A-W)/x^2`, `B/x`, `W_x/x`, and `chi_x/x`.
- A finite-difference nonlinear oracle can compare Stage 4AC+4AE and Stage 4G
  around the same GP background perturbation.

That first implementation was deliberately narrow: one hidden raw
`delta R_ww[gamma]` block plus direct finite-difference oracle fixtures. The
visible components, trace/free assembly, and A/Theta/K Ricci insertions were
implemented later as separate validation blocks. The full operator and
eigensolver gate remain incomplete.

## First Ricci Block: Hidden Physical `delta R_ww[gamma]`

The validation-only helper
`code/BlackStringToy/Stage4AOFrozenGaugeHiddenRww.hpp` implements only the
raw lower/lower hidden physical component on the locked flat GP spatial
background:

```text
delta R_ww[gamma] =
    (delta h_xx - delta h_ww) / x^2
  + (partial_x delta h_xx
     + 2 partial_z delta h_xz
     - partial_x delta h_zz) / (2x)
  - 2 partial_x delta h_ww / x
  - (partial_xx delta h_ww + partial_zz delta h_ww) / 2
  + 2 partial_x delta chi / x
  + (partial_xx delta chi + partial_zz delta chi) / 2.
```

The first four terms are the linearized Stage 4AC conformal hidden Ricci
contribution around the flat GP spatial metric. The final two
conformal-factor terms are the linearized Stage 4AE `R^chi_ww` correction
around `chi=1`.

This helper is a raw component check: it does not itself compute visible
Ricci or perform the separately implemented A/Theta/K Ricci insertions. It is
not a complete operator row or a production RHS path.

The fixture
`code/BlackStringToy/tests/Stage4AOCHiddenPhysicalDeltaRwwTest.cpp` validates
the block by central differencing:

- primary oracle: Stage 4G `CartoonRicci::compute_metric_derivative_ricci`;
- secondary oracle: the reviewed Stage 4AH split path using Stage 4AC plus
  Stage 4AE.

The epsilon sweep uses `1e-2`, `1e-4`, `1e-5`, `1e-6`, and `1e-7`, with the
stable `1e-5` to `1e-6` plateau required below `1e-7` for both oracle paths.
The tested perturbations include zero input, pure `delta h_ww`, pure
`delta chi`, z-dependent scalar hidden data, radial visible metric data, and a
mixed radial/z off-diagonal `delta h_xz` mode. Negative guards show that wrong
hidden multiplicity, dropping `1/x` structures, a wrong off-diagonal
z-derivative sign, or treating the raw component as already trace-free would
fail.

The close-second Ricci target implemented here is the visible one-z component
`delta R_xz`, because it is the next high-risk parity-mixing case after hidden
`R_ww`.

## Second Ricci Block: Visible Physical `delta R_xz[gamma]`

The validation-only helper
`code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxz.hpp` implements only the
raw lower/lower visible off-diagonal component on the locked flat GP spatial
background:

```text
delta R_xz[gamma] =
    (partial_z delta h_xx - partial_z delta h_ww) / x
  - partial_xz delta h_ww
  + partial_xz delta chi.
```

This is the first one-z Ricci target. It checks the parity-mixing sector that
hidden `delta R_ww` does not touch. The pure `delta h_xz` contribution cancels
for this raw component at the locked flat GP background, so pure `delta h_xz`
is a required zero oracle rather than a nonzero coefficient. A future
assembled operator still has to preserve the one-z parity sector for
`h_xz`, `A_xz`, `beta^z`, `B^z`, and `hat_Gamma^z`.

There is no reviewed Stage 4AC/4AE-style split path for visible `R_xz`.
Therefore the Stage 4G metric-derivative physical Ricci engine is the primary
and sole independent nonlinear oracle for this substep.

The fixture
`code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxzTest.cpp` validates
the block by central differencing Stage 4G over the same epsilon sweep
`1e-2`, `1e-4`, `1e-5`, `1e-6`, and `1e-7`, with the `1e-5` to `1e-6`
plateau required below `1e-7`. The tested perturbations include zero input,
pure `delta h_xz` as a zero oracle, an even-sector diagonal scalar
`delta h_xx` mode that sources one-z `R_xz`, a radial/z mixed hidden
`delta h_ww` mode, and a pure conformal-factor mixed derivative mode.

The one-z parity check projects the raw output from an even-sector diagonal
input onto sine/cosine components. The allowed sine amplitude is nonzero and
the forbidden cosine leakage is near roundoff. Negative guards show that a
wrong mixed-derivative sign, dropped conformal-factor mixed derivative,
incorrect hidden multiplicity on the visible component, spurious `h_xz`
contribution, or wrong parity assignment would fail. At that substep, scope
guards kept `delta R_xx`, `delta R_zz`, trace-free A-curvature source, Theta
Ricci scalar, the full frozen-gauge operator, eigensolver, and production
wiring false; `delta R_zz` is now implemented separately below.

## Third Ricci Block: Visible Physical `delta R_zz[gamma]`

The validation-only helper
`code/BlackStringToy/Stage4AOFrozenGaugeVisibleRzz.hpp` implements only the
raw lower/lower visible diagonal `zz` component on the locked flat GP spatial
background:

```text
delta R_zz[gamma] =
    partial_xz(delta h_xz)
  - 0.5 partial_zz(delta h_xx)
  - 0.5 partial_xx(delta h_zz)
  - partial_zz(delta h_ww)
  + 2 partial_z(delta h_xz) / x
  - partial_x(delta h_zz) / x
  + partial_x(delta chi) / x
  + 0.5 partial_xx(delta chi)
  + 1.5 partial_zz(delta chi).
```

The conformal-factor signs are positive because the project's Stage 4G and
GRChombo-facing convention is `gamma=h/chi` and the returned Ricci components
are lower/lower physical `R_IJ[gamma]`.

There is no reviewed Stage 4AC/4AE-style split path for visible `R_zz`.
Therefore the Stage 4G metric-derivative physical Ricci engine is the primary
and sole independent nonlinear oracle for this substep.

The fixture
`code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRzzTest.cpp` validates
the block by central differencing Stage 4G over the epsilon sweep `1e-2`,
`1e-4`, `1e-5`, `1e-6`, and `1e-7`, with the `1e-5` to `1e-6` plateau
required below `1e-7`. The tested perturbations include zero input, pure
`delta h_zz`, pure one-z `delta h_xz`, pure z-dependent `delta h_ww`, pure
mixed `delta chi`, z-dependent diagonal `delta h_xx`, and mixed radial/z
coverage.

The even-sector parity check projects raw outputs onto cosine/sine
components. Even scalar/diagonal inputs and one-z `h_xz` inputs both produce
even `R_zz` output with nonzero allowed cosine amplitude and near-roundoff
forbidden sine leakage. Negative guards show that wrong sign in
`partial_xz(delta h_xz)`, dropping `2 partial_z(delta h_xz)/x`, dropping
`-partial_x(delta h_zz)/x`, dropping `-partial_zz(delta h_ww)`, flipping the
positive `chi` signs, applying hidden multiplicity to visible `R_zz`, or
treating the raw component as already trace-free would fail.

## Fourth Ricci Block: Visible Physical `delta R_xx[gamma]`

The validation-only helper
`code/BlackStringToy/Stage4AOFrozenGaugeVisibleRxx.hpp` implements only the
raw lower/lower visible radial `xx` component on the locked flat GP spatial
background:

```text
delta R_xx[gamma] =
    partial_xz(delta h_xz)
  - 0.5 partial_xx(delta h_zz)
  - 0.5 partial_zz(delta h_xx)
  + partial_x(delta h_xx) / x
  - 2 partial_x(delta h_ww) / x
  - partial_xx(delta h_ww)
  + partial_x(delta chi) / x
  + 1.5 partial_xx(delta chi)
  + 0.5 partial_zz(delta chi).
```

The conformal-factor signs are positive because the project's Stage 4G and
GRChombo-facing convention is `gamma=h/chi` and the returned Ricci components
are lower/lower physical `R_IJ[gamma]`. This is the raw visible component:
there is no trace-free projection and it is not inserted into the future
`A_IJ` curvature source in this substep.

There is no reviewed Stage 4AC/4AE-style split path for visible `R_xx`.
Therefore the Stage 4G metric-derivative physical Ricci engine is the primary
and sole independent nonlinear oracle for this substep.

The fixture
`code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxxTest.cpp` validates
the block by central differencing Stage 4G over the epsilon sweep `1e-2`,
`1e-4`, `1e-5`, `1e-6`, and `1e-7`, with the `1e-5` to `1e-6` plateau
required below `1e-7`. The tested perturbations include zero input, pure
`delta h_xx`, pure `delta h_zz`, pure `delta h_ww`, pure one-z
`delta h_xz`, pure mixed `delta chi`, z-dependent diagonal scalar input, and
mixed radial/z coverage.

The even-sector parity check projects raw outputs onto cosine/sine
components. Even scalar/diagonal inputs and one-z `h_xz` inputs both produce
even `R_xx` output with nonzero allowed cosine amplitude and near-roundoff
forbidden sine leakage. Negative guards show that wrong sign in
`partial_xz(delta h_xz)`, dropping a radial `1/x` term, dropping the
conformal-factor terms, flipping the positive `chi` signs, applying hidden
multiplicity to visible `R_xx`, or treating the raw component as already
trace-free would fail.

## Raw Ricci Trace And Trace-Free Ricci Assembly

The validation-only helper
`code/BlackStringToy/Stage4AOFrozenGaugeRicciAssembly.hpp` consumes the four
validated raw result types for `delta R_xx`, `delta R_xz`, `delta R_zz`, and
`delta R_ww`. It does not accept loose raw component doubles.

On the locked frozen-GP background, the physical spatial metric is flat and
`R_IJ^GP = 0`, so inverse-metric variation terms do not enter the linear
scalar trace. With `h^xz_GP = 0` and hidden multiplicity two,

```text
delta R =
    delta R_xx
  + delta R_zz
  + 2 delta R_ww.
```

There is no `delta R_xz` contribution to the scalar trace at this background.

The trace-free projection uses the GRChombo/project `d=4` convention:

```text
[delta R_xx]^TF = delta R_xx - (1/4) delta R,
[delta R_zz]^TF = delta R_zz - (1/4) delta R,
[delta R_ww]^TF = delta R_ww - (1/4) delta R,
[delta R_xz]^TF = delta R_xz,
```

because `h_xz_GP = 0`. The implemented trace-free residual check is

```text
[delta R_xx]^TF + [delta R_zz]^TF + 2 [delta R_ww]^TF = 0.
```

The projection is applied only after assembling the complete raw Ricci tensor;
it does not project individual raw component subpieces and then combine them.

The fixture
`code/BlackStringToy/tests/Stage4AOCRicciTraceFreeAssemblyTest.cpp` validates
the assembly by central differencing the nonlinear Stage 4G physical Ricci
engine for all four raw components, then independently forming the oracle
trace and trace-free components. The epsilon sweep is `1e-2`, `1e-4`,
`1e-5`, `1e-6`, and `1e-7`; the combined trace/free plateau requires the
`1e-5` to `1e-6` maximum component error below `2e-7`. The test covers zero
input, pure `delta h_xx`, pure `delta h_zz`, pure `delta h_ww`, pure
`delta h_xz`, pure `delta chi`, a mixed radial/z scalar mode, and parity
cases.

Parity checks show that even-sector scalar/diagonal input gives an even
Ricci trace, that the same scalar input gives the one-z `TF_xz` parity, and
that one-z `h_xz` input gives an even Ricci trace with near-roundoff forbidden
leakage. Negative guards show that dropping the hidden factor two, adding
`R_xz` to the scalar trace, using `d=3`, projecting `R_xx` before full trace
assembly, or treating the result as an A-curvature source would fail.

## Theta Ricci Scalar Insertion

GRChombo's Theta RHS convention includes the Ricci scalar as

```text
0.5 * alpha * R.
```

On the locked frozen-GP background,

```text
alpha_GP = 1,
delta alpha = 0,
```

so the validation-only frozen-gauge insertion is

```text
delta Theta_RHS += 0.5 delta R.
```

The implemented block uses the raw Ricci trace assembly above:

```text
delta R = delta R_xx + delta R_zz + 2 delta R_ww.
```

The insertion code consumes the `TraceFreeRicciAssembly` result and reads its
assembled `scalar_trace()`. It does not recompute hidden multiplicity two, does
not add `R_xz` to the scalar trace, and does not write any non-Theta output
slot. It is still not the full Theta RHS: Z4 damping, kappa conventions,
constraint terms, and the rest of the coupled operator remain separate.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaRicciScalarBlockTest.cpp`
checks the pointwise and grid apply paths, verifies the positive
`+0.5 delta R` sign, confirms hidden `ww` multiplicity two is inherited from
the trace assembly, confirms `R_xz` does not enter, and keeps
`complete_operator=false` and `eigensolver_allowed=false`. Negative guards
show that a wrong sign, missing factor `0.5`, dropped hidden multiplicity,
spurious `R_xz` trace contribution, non-Theta output write, or a false full
Theta-RHS completion claim would fail.

## A-Equation Ricci Curvature Insertion

GRChombo's `A_IJ` RHS convention first forms

```text
Adot_TF = -D_I D_J alpha + chi * alpha * R_IJ,
```

then applies `make_trace_free` before adding the result to `rhs.A`. In frozen
gauge,

```text
delta alpha = 0,
alpha_GP = 1,
chi_GP = 1,
R_IJ^GP = 0,
partial_I alpha_GP = 0.
```

Therefore

```text
delta(D_I D_J alpha) = 0,
delta(chi * alpha * R_IJ) = delta R_IJ,
```

and the validation-only curvature insertion is exactly

```text
delta A_xx_RHS += [delta R_xx]^TF,
delta A_xz_RHS += [delta R_xz]^TF,
delta A_zz_RHS += [delta R_zz]^TF,
delta A_ww_RHS += [delta R_ww]^TF.
```

The block consumes the reviewed `TraceFreeRicciAssembly` object. It does not
recompute the scalar trace, does not write non-`A` slots, and does not multiply
the representative `A_ww` output by hidden multiplicity again. The hidden factor
already entered the scalar trace used to form `[delta R_IJ]^TF`.

The fixture
`code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`
checks the pointwise and grid apply paths, verifies that `A_xz` receives raw
`R_xz` because `h_xz^GP=0`, verifies the inserted A-source trace
`A_xx + A_zz + 2 A_ww = 0`, and keeps
`complete_operator=false` and `eigensolver_allowed=false`. Negative guards show
that a wrong sign, raw-Ricci insertion without trace-free projection, `d=3`
projection, double-counted hidden multiplicity on `A_ww`, non-`A` output write,
or false complete-A-RHS claim would fail.

This is still not the full `A_IJ` RHS and not the full frozen-gauge operator.
Z4 damping/constraint terms, `hat_Gamma` evolution, full-operator JVP/parity
tests, boundary validation, MOTS, and spectral work remain separate.

## Z4 Damping And Kappa Convention Lock

The planning-layer convention decision for the main frozen-gauge spectral
gate deliberately adopts the inherited GRChombo-facing values

```text
kappa1 = 0.1,
kappa2 = 0.0,
kappa3 = 1.0,
covariantZ4 = true.
```

Codex implements this recorded decision; it does not tune the parameters.
Zero damping is reserved for a later diagnostic comparison and is not the
main Stage 4AO-C path.

GRChombo defines the damping parameters in
`external/GRChombo/Source/CCZ4/CCZ4RHS.hpp`:

```text
kappa1, kappa2, kappa3,
covariantZ4.
```

`external/GRChombo/Source/GRChomboCore/SimulationParametersBase.hpp` reads
defaults

```text
kappa1 = 0.1,
kappa2 = 0.0,
kappa3 = 1.0,
covariantZ4 = true.
```

The inherited smoke parameter file
`code/BlackStringToy/params_stage2_smoke.txt` repeats those values, but it
remains an unchanged smoke scaffold rather than the authority for this lock.
The authority is the planning-layer decision recorded here and in the Stage
4AO-C checklist.

GRChombo computes

```text
Z_over_chi^i = 0.5 * (hat_Gamma^i - tilde_Gamma^i),
Z^i = chi * Z_over_chi^i.
```

The code stores this using `vars.Gamma` for the hatted connection and
`chris.contracted` for `tilde_Gamma`. The effective kappa factor used in the
RHS is

```text
kappa1_times_lapse =
    kappa1,         if covariantZ4 is true,
    kappa1 * alpha, otherwise.
```

On the locked frozen-GP background,

```text
alpha_GP = 1,
delta alpha = 0,
chi_GP = 1,
Theta_GP = 0,
Z_over_chi_GP = 0,
K_GP = div beta_GP = 3 lambda / 2,
lambda = sqrt(r0 / x^3),
hat_Gamma_GP = 0.
```

Thus the covariant/noncovariant `kappa1_times_lapse` distinction does not
change the frozen-gauge linearized coefficients, because `alpha_GP=1` and
`delta alpha=0`. It will matter again for live-gauge work if `delta alpha`
is included.

### Theta And K Rows

In `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`, the non-BSSN Theta row
contains

```text
-0.5 * Theta * kappa1_times_lapse
       * ((d + 1) + kappa2 * (d - 1)).
```

With `d=4`, the frozen-GP linearization is

```text
delta rhs_Theta|kappa =
    -0.5 * kappa1 * (5 + 3 kappa2) * delta Theta.
```

This is algebraic once `kappa1/kappa2` are fixed. It is separate from the
already implemented non-kappa `-Theta K` contribution, which gives

```text
delta rhs_Theta|-Theta K =
    -K_GP * delta Theta
  = -(3 lambda / 2) * delta Theta.
```

The K row contains

```text
-kappa1_times_lapse * d * (1 + kappa2) * Theta.
```

With `d=4`, the frozen-GP linearization is

```text
delta rhs_K|kappa =
    -4 * kappa1 * (1 + kappa2) * delta Theta.
```

This is also algebraic once `kappa1/kappa2` are fixed.

With the main-path values, the implemented validation-only insertion is

```text
output[Theta] += -0.25 * input[Theta],
output[K]     += -0.4  * input[Theta].
```

No `deltaK` input contributes, no other output is touched, and the corrected
CCZ4 K pieces `+delta R`, `+3 lambda deltaK`, and
`-3 lambda deltaTheta` remain separate and unchanged.

`code/BlackStringToy/tests/Stage4AOCFrozenGaugeCCZ4DampingInsertionTest.cpp`
independently derives both coefficients from `kappa1`, `kappa2`, and `d`
rather than copying `-0.25` and `-0.4` as unexplained constants. It checks
pointwise and grid apply paths, the unchanged corrected K block, and closed
K/Theta/full-operator/eigensolver gates. Genuine wrong-sign mutation guards
compare the actual coefficients with independently derived negative damping
and reject their positive-sign antidamping mutations. Further guards cover
`d=3`, wrong nonzero-`kappa2` dependence, spurious `deltaK`, a spurious
hatted-Gamma write from the separate K/Theta damping block, non-K/Theta
output writes, and false completion claims.

The Theta row also contains `-Z dot grad(alpha)`. This has no frozen-GP
linear contribution because `grad(alpha_GP)=0` and `delta alpha=0`.

### Hatted-Gamma Rows

The hatted-Gamma RHS contains the Z-dependent algebraic part

```text
(2/d) * (div beta * 2 kappa3 Z_over_chi^i
         - 2 alpha K Z_over_chi^i)
- 2 kappa1_times_lapse Z_over_chi^i.
```

With `d=4`, `alpha_GP=1`, and `div beta_GP = K_GP`, the frozen-GP
linearization is

```text
delta rhs_hat_Gamma^i|Z/kappa =
    (K_GP * (kappa3 - 1) - 2 kappa1) * delta Z_over_chi^i.
```

With the locked main-path `kappa3=1`, this reduces to the implemented partial
block target

```text
delta rhs_hat_Gamma^i|Z/kappa =
    -2 kappa1 * delta Z_over_chi^i
  = -0.2 * delta Z_over_chi^i.
```

In addition, `kappa3` appears in the shift-gradient term

```text
-(chris.contracted + 2 kappa3 Z_over_chi) * d1.shift.
```

The first partial hatted-Gamma block now includes the surviving frozen-GP
piece of that term; the remaining shift-Hessian and full-row terms are still
deferred.

However this must not be implemented as a raw `hat_Gamma` damping term. The
state variable is hatted, while the damping acts on

```text
delta Z_over_chi^i =
    0.5 * (delta hat_Gamma^i - delta tilde_Gamma^i).
```

The validation-only contracted-connection helper includes the `x` hidden
contraction locked in Stage 4AM/4AN and the full hidden-aware `z` companion.
It reconstructs Z but writes no Gamma RHS output. A separate tested operator
block now consumes its result for the narrow insertion above.

#### Hatted-Gamma Evolution Design Preflight

This subsection began as a documentation/design result. The subsequent
contracted-connection/Z helper implements the explicitly identified
prerequisite without RHS output, and the first separate hatted-Gamma block now
implements only the locked non-advection Z/kappa and `kappa3`
shift-gradient insertion. The exact selected
`USE_CCZ4` GRChombo equation is

```text
rhs.Gamma[i] = advec.Gamma[i] + Gammadot[i],

Gammadot[i] =
    (2/d) [
        divbeta (tilde_Gamma[i] + 2 kappa3 Z_over_chi[i])
      - 2 alpha K Z_over_chi[i]
    ]
  - 2 kappa1_times_lapse Z_over_chi[i]
  + 2 h^ij (alpha partial_j Theta - Theta partial_j alpha)
  - 2 A^ij partial_j alpha
  - alpha [2(d-1)/d h^ij partial_j K
           + d A^ij partial_j chi / chi]
  - (tilde_Gamma[j] + 2 kappa3 Z_over_chi[j]) partial_j beta^i
  + 2 alpha Gamma^i_jk A^jk
  + h^jk partial_j partial_k beta^i
  + (d-2)/d h^ij partial_j partial_k beta^k.
```

Here `tilde_Gamma[i]` is GRChombo's `chris.contracted[i]`, while the evolved
`vars.Gamma[i]` is hatted. The locked translation is

```text
delta Z_over_chi^i =
    0.5 * (delta hat_Gamma^i - delta tilde_Gamma^i).
```

No `d1.A` occurs in this selected GRChombo equation. The momentum-constraint
form has already replaced derivatives of `A_IJ` by the `partial_i K`,
`partial_i chi`, and connection-A terms. Therefore no derivative of
`delta A_IJ` may be invented in the adapter. At the flat conformal GP
background `Gamma^i_jk=0`, so direct algebraic `delta A_IJ` also drops out;
the surviving A-sector term is `A_GP^jk delta Gamma^i_jk`.

Use

```text
b = beta_GP^x = lambda x,
lambda = sqrt(r0/x^3),
K0 = div beta_GP = 3 lambda/2,
partial_x K0 = -9 lambda/(4x),
partial_x b = -lambda/2,
partial_xx b = 3 lambda/(4x),
partial_ww beta_GP^x = -3 lambda/(2x)  (for each hidden w),

A0_xx = -7 lambda/8,
A0_zz = -3 lambda/8,
A0_ww =  5 lambda/8.
```

No linear determinant constraint is assumed while constructing the local
connection oracle. About `h_IJ=delta_IJ`, the full modified-cartoon
contractions are

```text
g_x := delta tilde_Gamma^x
    =  0.5 partial_x delta h_xx
     - 0.5 partial_x delta h_zz
     -       partial_x delta h_ww
     +       partial_z delta h_xz
     + 2 (delta h_xx - delta h_ww)/x,

g_z := delta tilde_Gamma^z
    =        partial_x delta h_xz
     + 2 delta h_xz/x
     - 0.5 partial_z delta h_xx
     + 0.5 partial_z delta h_zz
     -       partial_z delta h_ww.
```

The factor two in each hidden contribution is the two-copy hidden
multiplicity. The `x` formula is the linearization of the implemented 4AN
full-contraction helper. The Stage 4AO-C helper now implements the `z`
companion as well. Its hidden Christoffel is

```text
delta Gamma^z_ww = delta h_xz/x - 0.5 partial_z delta h_ww,
```

and must not be inferred solely from the determinant-reduced identity unless
`delta h_xx + delta h_zz + 2 delta h_ww = 0` is separately certified.

Let

```text
H_x = delta hat_Gamma^x,  H_z = delta hat_Gamma^z,
Z_x = 0.5 (H_x - g_x),   Z_z = 0.5 (H_z - g_z).
```

The connection-A pieces, including both hidden copies, are

```text
C_x = 2 [A0_xx delta Gamma^x_xx
         + A0_zz delta Gamma^x_zz
         + 2 A0_ww delta Gamma^x_ww]
    = lambda [
        -7/8 partial_x delta h_xx
        -3/4 partial_z delta h_xz
        +3/8 partial_x delta h_zz
        -5/4 partial_x delta h_ww
        +(5/2)(delta h_xx-delta h_ww)/x],

C_z = 2 [A0_xx delta Gamma^z_xx
         + A0_zz delta Gamma^z_zz
         + 2 A0_ww delta Gamma^z_ww]
    = lambda [
        -7/4 partial_x delta h_xz
        +7/8 partial_z delta h_xx
        -3/8 partial_z delta h_zz
        -5/4 partial_z delta h_ww
        +(5/2) delta h_xz/x].
```

With `d=4`, frozen `alpha=chi=1`, `delta alpha=delta beta^i=0`,
`Theta_GP=Z_GP=tilde_Gamma_GP=0`, `kappa1=0.1`, and `kappa3=1`, the complete
linearized rows are

```text
delta rhs_hat_Gamma^x =
    b partial_x H_x                                      [common advection]
  + (K0/2) g_x - 0.2 Z_x                               [CCZ4 Z/kappa]
  + (lambda/2) H_x                                     [shift gradient]
  + 2 partial_x delta Theta
  - (3/2) partial_x delta K
  - (27 lambda/(8x)) delta h_xx                        [h^xj partial_j K0]
  + (7 lambda/2) partial_x delta chi
  + C_x                                                 [connection-A]
  - (3 lambda/(4x)) delta h_xx
  + (3 lambda/x) delta h_ww                            [shift Hessian]
  + (9 lambda/(8x)) delta h_xx,                        [grad div shift]

delta rhs_hat_Gamma^z =
    b partial_x H_z                                      [common advection]
  + (K0/2) g_z - 0.2 Z_z                               [CCZ4 Z/kappa]
  + 2 partial_z delta Theta
  - (3/2) partial_z delta K
  - (27 lambda/(8x)) delta h_xz                        [h^zj partial_j K0]
  + (3 lambda/2) partial_z delta chi
  + C_z                                                 [connection-A]
  + (9 lambda/(8x)) delta h_xz.                        [grad div shift]
```

The two shift-Hessian coefficients in the `x` row come from

```text
delta(h^jk partial_j partial_k beta_GP^x)
  = -(3 lambda/(4x)) delta h_xx
    +(3 lambda/x) delta h_ww,
```

where the `delta h_ww` coefficient includes both hidden copies. The final
metric terms in both rows are

```text
0.5 delta h^ij partial_j(div beta_GP).
```

The `kappa3=1` cancellation is explicit:

```text
(2/d)[divbeta_GP (g_i + 2 Z_i) - 2 K0 Z_i]
    = (K0/2) g_i,
```

before adding `-0.2 Z_i`. The remaining `kappa3` shift-gradient term is
`-H_j partial_j beta_GP^i`; it gives `+(lambda/2)H_x` for the `x` row and
zero for the `z` row. Hidden `j=w` values vanish on the cartoon plane and do
not supply an additional shift-gradient term.

The common GP-shift advection block already supplies
`b partial_x H_x` and `b partial_x H_z`. A future hatted-Gamma block must
exclude those terms and expose only the non-advection remainder; assembly
must add the common block exactly once.

Term ownership is locked as follows:

| Term family | Ownership | Frozen-GP disposition |
| --- | --- | --- |
| `advec.Gamma[i]` | reusable project helper | Already implemented by the common advection block; do not duplicate. |
| Exact visible GRChombo `Theta`, `K`, and `chi` gradient coefficients | reusable directly from GRChombo | Survive with `d=4`; lapse-gradient pieces vanish. |
| `delta tilde_Gamma^x` and `delta Z_over_chi^x` convention | reusable project helper | Linearized helper implemented and finite-differenced against the 4AN full contraction. |
| `delta tilde_Gamma^z` and `delta Z_over_chi^z` | reusable project helper | Full hidden-aware linearized companion implemented and finite-differenced against an independent nonlinear test contraction; the determinant-reduced identity remains only a conditional cross-check. |
| `divbeta`, background shift derivatives, and visible shift-Hessian pieces | reusable directly from the locked GP background | Survive through background coefficients even though `delta beta=0`. |
| Hidden `divbeta=partial_x beta^x+2 beta^x/x` | requires modified-cartoon adapter | Supplies `K0=3lambda/2`; its perturbation vanishes because gauge is frozen. |
| Hidden `delta Gamma^i_ww A0^ww` | requires modified-cartoon adapter | Survives with multiplicity two in `C_x` and `C_z`. |
| Hidden `partial_ww beta_GP^x` in the shift Hessian | requires modified-cartoon adapter | Survives twice in the `x` row; zero in the `z` row. |
| `kappa3=1` Z and shift-gradient terms | reusable directly from GRChombo after Z reconstruction | Z terms reduce to `(K0/2)g_i`; shift gradient gives `+(lambda/2)H_x`, zero for `z`. |
| `-2 kappa1 Z_over_chi^i` | reusable directly from GRChombo | Survives as locked `-0.2 Z_i`. |
| `partial_j delta A_IJ` | absent from selected equation | Must remain absent: GRChombo uses the momentum-constraint form. |
| Direct `delta A_IJ` in `Gamma^i_jk A^jk` | vanishes in frozen gauge | Multiplies `Gamma_GP=0`; only `A_GP^jk delta Gamma^i_jk` survives. |
| `partial_j alpha`, `delta alpha`, `delta beta`, `delta(div beta)`, and variations multiplying background `Theta`, Z, or contracted Gamma | vanishes in frozen gauge | Do not create placeholder contributions. |

The Fourier/parity contract is:

```text
hat_Gamma^x : scalar/even sector,
hat_Gamma^z : one-z/opposite-parity sector.
```

Thus even scalar inputs must not leak into the forbidden `hat_Gamma^z`
cosine sector, and one-z inputs must not leak into the forbidden
`hat_Gamma^x` sine sector beyond roundoff-scaled tolerance.

Independent validation must use two oracles. First, central-difference the
selected nonlinear GRChombo-facing Gamma expression for the visible terms and
the full 4AN `x` contraction, perturbing hatted Gamma, metric, K, Theta, chi,
and A independently. Use the established sweep `1e-2, 1e-4, 1e-5, 1e-6,
1e-7` and require a stable `1e-5`/`1e-6` middle plateau. Second, validate the
hidden `ww` connection-A and vector-Hessian pieces against a separately
derived analytic modified-cartoon block oracle, including multiplicity-one
negative mutations. The nonlinear and analytic oracles must not call the
future linearized implementation.

Required cases are zero/background, pure `H_x`, pure `H_z`, pure K, pure
Theta, pure chi, pure metric including `h_xz` and `h_ww`, pure A, and a mixed
perturbation. Pure A must have zero direct linear contribution at this GP
background; a nonzero derivative-A result is a failure. Before differencing,
the nonlinear oracle must reproduce the 4AO-A background targets: the `x`
row cancellation `27 lambda/(8x)-9 lambda/(4x)-9 lambda/(8x)=0` and an
identically zero `z` row. Each epsilon must retain that zero background
residual, and Fourier fixtures must measure allowed amplitude and forbidden
parity leakage. The local non-advection oracle and the existing common
advection oracle must also be tested separately before one-time assembly.

The first two narrow implementation targets after this preflight are now
complete:
`Stage4AOFrozenGaugeContractedConnection.hpp` returns `g_x`, `g_z`, `Z_x`,
and `Z_z` without writing any RHS output. Its fixture finite-differences the
existing 4AN `x` helper and an independent nonlinear full `z` contraction,
checks pure components, hidden multiplicity, derivative signs, optional
determinant-reduced identities, and Fourier parity. The separate operator
block implements only

```text
x: (K0/2) g_x - 0.2 Z_x + (lambda/2) H_x,
z: (K0/2) g_z - 0.2 Z_z,
```

leaving connection-A, K/Theta/chi gradients, hidden shift Hessians, and full
row assembly for separately validated blocks. Its focused fixture derives the
coefficients independently from `K0`, `d`, `kappa1`, and `kappa3`, checks pure
H and pure-metric/Z paths, rejects a spurious `+(lambda/2)H_z`, wrong damping
sign, hidden multiplicity one, and duplicate advection, and locks the two
parity sectors while all completion gates remain false.

### Other Rows

No `chi`, `h_IJ`, or `A_IJ` row has a direct `kappa1/kappa2` damping term in
the inspected GRChombo RHS. In a live moving-puncture gauge, `B^i` may inherit
the Gamma RHS through the gauge driver, but gauge perturbations are excluded
from the frozen-gauge 4AO-C state. Stage 4AO-D must revisit that inheritance
for live gauge.

### Recorded Decision And Remaining Gate

The inherited values are intentionally locked for the main validation path,
the simple K/Theta damping insertion is implemented, and the complete
hatted-Gamma term ownership and linearized target are now designed. This
resolves the parameter-choice and Gamma-design preflight gates without
claiming tuning or a complete operator. The first non-advection hatted-Gamma
Z/kappa and `kappa3` shift-gradient block is implemented; all remaining Gamma
term families and complete row assembly remain deferred. Stage 4AO-C still
may not claim a complete frozen-gauge operator or a physical spectrum.

## Boundary-Condition Contract

The radial domain type is fixed as

```text
x in [x_in, x_out],
0 < x_in < r0 < x_out,
z periodic.
```

The wrapper includes the provisional Stage 4AO-B local validation values
`r0 = 1`, `x_in = 0.5`, and `x_out = 4.0` only as a default contract instance.
They are not a final spectral-domain choice. Stage 4AO-C must still vary
`x_in` and/or `x_out` for boundary-location convergence.

Boundary ownership:

- Inner boundary: `x_in` lies inside the GP horizon and outside the physical
  singularity at `x=0`. The future operator must use a characteristic/outflow
  compatible or otherwise justified radial condition and test convergence with
  `x_in`.
- Outer boundary: the future operator must choose a documented asymptotic,
  radiative, or frozen-background condition and test convergence with `x_out`.
- Physical GL sector: compact `z` is periodic; the physical mode must have a
  nonzero linearized horizon-radius response `delta R_H`. Radial conditions
  remain a contract, not validation.
- Constraint-related variables: boundary treatment must be compatible with the
  derived CCZ4 constraint subsystem and checked in the actual-operator JVP and
  parity tests.
- Hatted Gamma variables: boundary treatment must preserve the GRChombo
  hatted convention, hidden multiplicity, and parity sector before any
  spectral use.

No boundary validation test exists yet. The contract only prevents future
work from starting an eigensolver without naming the boundary rules it is
using.

## Actual-Operator Validation Hooks

The next Stage 4AO-C checks are explicit TODOs:

- finite-difference JVP of the actual frozen-gauge operator;
- independent block-derived JVP for the same state vector;
- epsilon plateau for the actual operator or matrix assembly;
- actual-operator parity leakage test, not the representative 4AO-B
  sub-operator;
- radial-resolution convergence;
- boundary-location convergence;
- linearized MOTS map `delta U -> delta h(z) -> delta R_H`.

These hooks must pass before eigensolver work, shift-invert, threshold
crossing, or any `k_c r0` claim.

## Intended Frozen-Gauge Operator

Stage 4AO-C must eventually build the linear operator

```text
L_frozen = D F_frozen[U_GP]
```

about the locked Stage 4AO-A uniform ingoing-GP background. The background
conventions are fixed by
`docs/derivations/stage4AO_A_uniform_gp_background_residual.md`:

```text
r0 = 1 in local fixtures unless otherwise stated,
z ~ z + L,
k_n = 2 pi n / L,
x in [x_in,x_out] with x_in > 0 and x_in < r0 < x_out,
alpha = 1,
beta^x = sqrt(r0/x),
beta^z = 0,
chi = 1,
h_xx = h_zz = h_ww = 1,
h_xz = 0.
```

Frozen gauge means the perturbations

```text
delta alpha = 0,
delta beta^x = delta beta^z = 0,
delta B^x = delta B^z = 0.
```

The physical perturbation vector still needs the coupled geometric variables

```text
delta chi,
delta h_xx, delta h_xz, delta h_zz, delta h_ww,
delta K,
delta A_xx, delta A_xz, delta A_zz, delta A_ww,
delta Theta,
delta hat_Gamma^x, delta hat_Gamma^z.
```

The frozen operator must use the actual RHS linearization for these variables,
including the 4D trace, hidden multiplicity, physical Ricci, Z4 terms,
nonlinear A/K linearization about the GP extrinsic curvature, advection by the
locked shift, and the full `hat_Gamma` RHS. The Stage 4AO-D holding source
`S_alpha` is irrelevant to frozen gauge and must not be used to hide raw
residual or Jacobian errors.

## Boundary And Observable Requirements

The radial domain type remains the Stage 4AO-A exterior/interior strip:

```text
x in [x_in,x_out],
0 < x_in < r0 < x_out,
z periodic.
```

Stage 4AO-C has not yet locked the spectral radial boundary conditions. Before
an eigensolver is meaningful, the harness must specify the boundary treatment
for every included perturbation and must demonstrate boundary-location
convergence by varying `x_in` and/or `x_out`.

The growth observable is not `hat_Gamma^x`. The candidate physical mode must
have a nonzero horizon-radius harmonic under the Stage 4AO-A observable:

```text
R_H(t,z) = h(t,z) sqrt(h_ww / chi) evaluated on the MOTS.
```

For the spectral problem this requires the linearized MOTS response that maps
an eigenvector to `delta R_H`. That map is not yet implemented.

## Parity And Jacobian Lessons From 4AO-B

Stage 4AO-B now checks representative z-coupled parity with periodic `D_z` and
`D_zz` stencils. Stage 4AO-C must repeat the parity check on the actual
frozen-gauge operator, not the representative 4AO-B sub-operator.

The parity sectors under `z -> -z` are:

- even sector: scalar/zero-z/two-z variables use `cos(kz)`, while one-z
  variables use `sin(kz)`;
- odd sector: scalar/zero-z/two-z variables use `sin(kz)`, while one-z
  variables use `cos(kz)`.

Scalar/zero-z/two-z variables are

```text
chi, h_xx, h_zz, h_ww, K, A_xx, A_zz, A_ww, Theta,
alpha, beta^x, B^x, hat_Gamma^x.
```

One-z variables are

```text
h_xz, A_xz, beta^z, B^z, hat_Gamma^z.
```

Stage 4AO-C must also include an epsilon plateau for the actual frozen
operator or matrix assembly, not only the representative 4AO-B JVP harness.

## Convention Map Status

The comparison target is the primary-source Gregory-Laflamme threshold and
growth-rate data after matching:

```text
r0 convention,
z period L,
k = 2 pi n / L,
background slicing,
frozen gauge,
perturbation sector,
measured growth observable.
```

GRChombo/internal conventions are authoritative for project variables.
Literature values are comparison targets only. The commonly quoted

```text
k_c r0 ~= 0.876
```

remains provisional because the primary-source convention map has not yet been
locked in this repository.

## Current Blockers

Stage 4AO-C cannot honestly produce a threshold crossing, unstable point, or
stable point yet because the repository does not contain the complete
frozen-gauge linearized operator.

Implemented partial pieces include GP-shift advection, tensor
shift-stretching, algebraic metric/chi coupling, the selected-CCZ4 K-output
`+3 lambda delta K - 3 lambda delta Theta` block and physical-`delta R`
insertion, A-output non-curvature algebraic block, Theta non-Ricci algebraic
block, Theta `-K_GP deltaTheta`, trace-free `delta A` projector, validated raw
Ricci components, raw Ricci trace/trace-free assembly, Theta Ricci scalar
insertion, A_IJ trace-free Ricci curvature insertion, the main kappa
convention lock, simple K/Theta damping insertions, and the validation-only
x/z contracted-connection and encoded-Z reconstruction helper, plus the first
non-advection hatted-Gamma Z/kappa/shift-gradient insertion.

Remaining missing pieces:

- complete `hat_Gamma^x` and `hat_Gamma^z` evolution: connection-A,
  K/Theta/chi gradients, vector/shift-Hessian terms, remaining couplings, and
  complete row assembly;
- remaining coupled CCZ4/Z4 RHS pieces;
- actual full-operator JVP/parity tests;
- boundary validation;
- linearized MOTS map;
- eigensolver/shift-invert;
- threshold/stable/unstable points;
- convergence battery;
- `k_c r0` convention map.

Therefore no Stage 4AO-C eigensolver or spectral claim is enabled. A toy
operator or calibrated model would be misleading and is deliberately not
introduced.

## Required Next Work For 4AO-C

Before Stage 4AO-C can be marked complete, the project needs a local
validation-only frozen-gauge operator harness that:

1. extends the current advection/tensor-stretching/algebraic-coupling/K-block,
   A/Theta-algebraic blocks, Theta `-K_GP deltaTheta` block, trace-free
   `delta A` projector contract, raw Ricci component validation blocks, raw
   trace/trace-free Ricci assembly, Theta Ricci scalar insertion, and `A_IJ`
   Ricci curvature insertion by consuming the now-validated x/z
   contracted-connection/Z reconstruction helper and then adding separately
   validated hatted-Gamma RHS blocks before the full coupled
   `D F_frozen[U_GP]` for the variables listed above;
2. verifies raw residual and JVP/matrix assembly without using `S_alpha` to
   hide errors;
3. checks parity-sector block diagonalization on the actual operator;
4. validates the documented radial boundary conditions;
5. implements a targeted spectral method such as shift-invert;
6. maps candidate eigenvectors to the linearized horizon-radius observable;
7. demonstrates at least one unstable point, one stable point, a threshold
   estimate, radial convergence, and boundary-location convergence.

Checkpoint G remains pending until Stage 4AO-D passes.
