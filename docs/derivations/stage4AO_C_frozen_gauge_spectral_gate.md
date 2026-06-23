# Stage 4AO-C Frozen-Gauge Spectral Gate

Status: blocked/incomplete. This note records why the first honest
frozen-gauge GL spectral validation harness is not yet implemented. It adds no
eigensolver, no threshold search, no production RHS wiring, no live evolution
wiring, and no Stage 4AO-D work.

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
| `K` RHS | Public `CCZ4RHS` has a visible CCZ4 `K` equation using Ricci scalar and lapse Hessian trace. | Reuse with adapter only after physical 4D Ricci scalar, hidden trace, and cartoon terms are assembled. |
| visible `A_xx,A_xz,A_zz` RHS | Stage 4AL implements local trace-free curvature/lapse source; public `CCZ4RHS` has visible nonlinear/advection terms. | Partial. Curvature/lapse local block exists, but Z4, nonlinear A/K, advection, and full operator assembly are missing. |
| hidden `A_ww` RHS | Stage 4AK implements `-D_wD_w alpha + alpha R_ww[gamma]`; Stage 4AL includes trace-free projection. | Partial local helper only. Missing Z4 term, nonlinear A/K terms, advection/cartoon vector terms, and RHS integration. |
| `Theta` RHS | Public `CCZ4RHS` has visible CCZ4 formula. Stage 4AO-A locks background residual. | Reuse with adapter only. Needs physical 4D Ricci scalar and hidden multiplicity in the same operator. |
| `hat_Gamma^x` value | Stage 4AN implements local `tilde_Gamma^x + 2 Z_over_chi^x` and hidden contraction with checked singular ingredients. | Reuse with adapter for local contraction/JVP checks. Not a RHS. |
| `hat_Gamma^x` RHS | Stage 4AM/4AO-A derive the hidden RHS cancellation on the GP background; Stage 4AO-B tests a representative `delta hww` contribution. | Missing as a complete nonlinear RHS block. Needs hidden vector-Laplacian, shift-divergence, Z4/damping, A/K/chi coupling, and full 4D multiplicity. |
| `hat_Gamma^z` RHS | Public `CCZ4RHS` has visible formula; Stage 4AO-B representative parity uses `D_z K`. | Mostly missing for modified-cartoon operator. Needs the `z` companion hidden contraction/RHS analysis and implementation. |
| Physical `R_ww[gamma]` | Stage 4AH composes Stage 4AC and 4AE; Stage 4AF identity gate exists. | Reuse with adapter for hidden Ricci source terms. Local helper only, not grid/RHS operator. |
| full physical Ricci scalar | Stage 4AI gives hidden scalar contribution `chi 2 h^ww R_ww`; Stage 4G/4I visible Ricci bridge exists. | Partial. Needs same-point assembly into `K/Theta/A` RHS operator. |

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

1. build the validation-only frozen-gauge operator wrapper and same-state-vector
   boundary-condition contract;
2. add actual-operator JVP and parity checks reusing the 4AO-B lessons;
3. add the linearized-MOTS / `delta R_H` adapter;
4. only then add an eigensolver or shift-invert adapter.

Starting with an eigensolver would be premature because there is no complete
operator or observable for it to validate.

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

Known missing pieces:

- no full repo-owned modified-cartoon RHS that combines the existing local
  curvature/lapse block with Z4 terms, nonlinear A/K terms, `chi`, metric,
  `K`, `Theta`, and `hat_Gamma` equations;
- no linearized operator assembly for the coupled frozen-gauge perturbation
  vector;
- no radial boundary-condition contract for the spectral problem;
- no shift-invert or equivalent targeted spectral method applied to the
  actual operator;
- no linearized MOTS/horizon-radius map from eigenvectors to `delta R_H`;
- no primary-source `k_c r0` convention map;
- no epsilon, radial-resolution, or boundary-location convergence for the
  actual frozen-gauge operator.

Therefore no Stage 4AO-C spectral test is added in this patch. A toy operator
or calibrated model would be misleading and is deliberately not introduced.

## Required Next Work For 4AO-C

Before Stage 4AO-C can be marked complete, the project needs a local
validation-only frozen-gauge operator harness that:

1. assembles the full coupled `D F_frozen[U_GP]` for the variables listed
   above;
2. verifies raw residual and JVP/matrix assembly without using `S_alpha` to
   hide errors;
3. checks parity-sector block diagonalization on the actual operator;
4. implements documented radial boundary conditions;
5. implements a targeted spectral method such as shift-invert;
6. maps candidate eigenvectors to the linearized horizon-radius observable;
7. demonstrates at least one unstable point, one stable point, a threshold
   estimate, radial convergence, and boundary-location convergence.

Checkpoint G remains pending until Stage 4AO-D passes.
