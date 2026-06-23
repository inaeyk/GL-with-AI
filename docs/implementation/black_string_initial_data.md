# Black-String Initial Data

Purpose: Stage 3A design note for the future 5D uniform black-string initial
data and Gregory-Laflamme perturbation in `BlackStringToy`.

Status: design only. Initial data is not implemented yet. The editable
`BlackStringToy` scaffold still inherits public `Examples/BinaryBH` behavior
only to validate the build/run workflow. It is not physical black-string
evolution and does not include compact `z`, GL perturbations, 5D variables,
modified-cartoon source terms, or `hww/Aww`.

## Stage 3A Scope

This note records the target physics and bookkeeping before source changes.
It is not a coding instruction and does not close Stage 3 implementation.

Do not implement from this note until the unresolved checklist below is reviewed
and approved. In particular, do not add `hww/Aww`, do not switch
`BlackStringToy` to `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, and do not claim that the
current scaffold evolves a 5D black string.

## Target Continuum Geometry

The target unperturbed spacetime is the five-dimensional uniform black string:
four-dimensional Schwarzschild times a compact string direction,

```text
Schwarzschild_4 x S^1_z.
```

The intended first implementation should use a horizon-penetrating
Gullstrand-Painleve-like slice rather than Schwarzschild coordinates, so that
the horizon at `x = r0` is not a coordinate singularity. A schematic GP-like
line element for the unperturbed string is

```text
ds^2 = -dt^2 + (dx + s sqrt(r0 / x) dt)^2 + x^2 dOmega_2^2 + dz^2,
```

where `s = +/- 1` encodes the ingoing/outgoing sign convention. The exact sign
and its relation to GRChombo's extrinsic-curvature convention are not fixed by
this note and must be verified before implementation.

The intended coordinates are:

- gridded directions: radial/cartoon coordinate `x` and compact string
  coordinate `z`;
- reconstructed directions: the transverse `S^2` directions handled by SO(3)
  symmetry;
- horizon representation for later diagnostics: `x = h(t,z)`.

Here `h(t,z)` is the coordinate radius/location of the apparent horizon on the
reduced grid. At time `t` and string coordinate `z`, the horizon surface is
located at `x = h(t,z)`. For the unperturbed uniform string, `h(0,z) = r0`.
During Gregory-Laflamme evolution, `h(t,z)` becomes `z`-dependent.

The target dimensional bookkeeping is:

- physical spacetime dimension: 5;
- physical spatial dimension: `GR_SPACEDIM=4`;
- Chombo gridded dimension: `CH_SPACEDIM=2`;
- hidden/reconstructed sphere directions: absorbed into modified-cartoon source
  terms and the extra variables `hww/Aww`.

The Stage 2 scaffold does not use this bookkeeping yet. Its current successful
smoke run is a non-physical 3+1D inherited scaffold validation.

## Variable Map

The future initial-data class must write the full target CCZ4 state on the
2D grid. The conformal-variable convention follows
`docs/implementation/ccz4_cartoon_variables.md`:

```text
gamma_ij = chi^{-1} h_ij
```

```text
gamma_ww = chi^{-1} hww
```

```text
K_ww = chi^{-1} (Aww + hww K / GR_SPACEDIM)
```

For `GR_SPACEDIM=4`, the conformal traceless split is therefore

```text
A_ij = chi (K_ij - gamma_ij K / 4)
Aww  = chi K_ww - hww K / 4
```

where the sign of `K_ij` still depends on the GRChombo/GP convention check.

### `chi`

For the unperturbed GP-like uniform string, the simplest target decomposition is
expected to start from a flat spatial slice in the transverse Schwarzschild
directions times `z`, so `chi = 1` before perturbation. The GL seed perturbation
is planned as a small perturbation of `chi`; this intentionally introduces small
constraint violations that CCZ4 should damp before nonlinear GL growth becomes
dominant.

### Conformal Metric `h_xx`, `h_zz`, `h_xz`

The unperturbed GP-like slice is expected to use

```text
h_xx = 1
h_zz = 1
h_xz = 0
```

when `chi = 1`, with the physical angular radius reconstructed from the hidden
sphere sector rather than represented by extra gridded coordinates.

If the perturbation is applied only to `chi`, then `h_xx`, `h_zz`, and `h_xz`
should remain the unperturbed conformal metric components at `t=0`. This choice
must be checked against the desired ADM mass behavior and constraint damping
before coding.

### Hidden Conformal Metric `hww`

`hww` is the conformal metric component for the suppressed SO(3) directions.
It is not the angular metric component `g_{theta theta}`. It is the hidden
Cartesian-like conformal metric component used by the SO(3)/modified-cartoon
reduction.

In flat transverse space, the suppressed Cartesian directions have
`gamma_ww = 1`. The angular factor `x^2` in `x^2 dOmega_2^2` is supplied by
the coordinate/cartoon reconstruction, not by setting the hidden Cartesian
component to `x^2`.

Later horizon diagnostics use the physical hidden component through

```text
gamma_ww = chi^{-1} hww
R(t,z) = h(t,z) sqrt(gamma_ww(t,h(t,z),z))
```

Here `R(t,z)` is the physical transverse `S^2` radius on the horizon. More
generally, away from the horizon,

```text
R = x sqrt(gamma_ww) = x sqrt(chi^{-1} hww)
```

For the unperturbed flat GP-like spatial slice, the target starting value is
expected to be `hww = 1` before perturbation. The actual angular line element
contains the coordinate factor `x^2 dOmega_2^2`.

Setting `hww = x^2` would double-count the angular radius and corrupt horizon
area/radius diagnostics.

The enum placement is a hard implementation hazard: the public AH code expects
`c_hww == c_K - 1`, while `Aww` is read through `c_Aww`. Do not add this
variable until the future `BlackStringToy/UserVariables.hpp` layout is reviewed.

### `K`

`K` is the trace of the physical extrinsic curvature over all four spatial
directions: the gridded `x`, gridded `z`, and two reconstructed sphere
directions. For the GP-like unperturbed string, `K` comes from the radial shift
field and is nonzero even though the spatial metric is simple.

The expected structure is:

- nonzero radial/sphere extrinsic curvature from the GP shift;
- zero or trivial string-direction contribution for the uniform unperturbed
  string;
- full trace taken with `GR_SPACEDIM=4`, not `CH_SPACEDIM=2`.

The exact formula and sign must be verified against the chosen GP sign and
GRChombo's `K_ij` convention before implementation.

For the schematic GP-like metric

```text
ds^2 = -dt^2 + (dx + s sqrt(r0 / x) dt)^2 + x^2 dOmega_2^2 + dz^2
```

the natural GP lapse and radial shift are

```text
alpha = 1
beta^x = s sqrt(r0 / x)
```

Using the convention

```text
K_ij = (1 / (2 alpha)) (D_i beta_j + D_j beta_i - partial_t gamma_ij)
```

and the time-independent GP spatial metric, the component formulas are, up to
GRChombo's possible overall `K_ij` sign convention,

```text
K_xx = -(s / 2) sqrt(r0 / x^3)
K_ww =  s       sqrt(r0 / x^3)   for each hidden transverse direction
K_zz =  0
```

Thus `K_zz = 0`, but the full trace `K` is not zero. The four-spatial-
dimensional trace is

```text
K = K^x_x + K^z_z + K^{w1}_{w1} + K^{w2}_{w2}
```

so

```text
K = (3s / 2) sqrt(r0 / x^3)
```

again up to the overall GRChombo sign convention. If GRChombo's sign convention
for `K_ij` is opposite to the convention above, all `K_ij`, `K`, and
`A_ij/Aww` signs must be flipped.

`K = gamma^{ij} K_ij` is a scalar on a fixed spatial slice, so it is invariant
under spatial coordinate changes on that slice. Its sign can still change if one
changes the GP branch, the time orientation, or the convention for the
future-pointing normal. The two signs `s = +/- 1` correspond to time-reversed
ingoing/outgoing embeddings of the same flat intrinsic spatial geometry.
Therefore the sign of `K` is fixed only after choosing both the GP branch and
GRChombo's `K_ij` convention.

### `A_xx`, `A_zz`, `A_xz`, And `Aww`

The conformal traceless extrinsic-curvature components should be derived from
the physical `K_ij` after the four-dimensional spatial trace is known:

- `A_xx`: radial traceless component;
- `A_zz`: compact-string traceless component, expected nonzero through trace
  subtraction even if physical `K_zz` is zero;
- `A_xz`: expected zero for the unperturbed uniform string and for a pure
  conformal-factor GL seed at `t=0`;
- `Aww`: hidden traceless component for the two reconstructed sphere directions.

The tracelessness condition must include the hidden directions:

```text
h^xx A_xx + h^zz A_zz + 2 h^ww Aww = 0
```

for a diagonal reduced state, up to the exact index conventions used in the
implementation. This must be converted into code using GRChombo's tensor
storage conventions, not by ad hoc component arithmetic.

### Lapse And Shift

Separate two roles before implementation:

1. the natural GP shift used to derive the geometric initial data;
2. the evolution gauge variables used to start the simulation.

The GP line element is used primarily to define a regular horizon-penetrating
initial hypersurface and to derive the geometric data `gamma_ij` and `K_ij`.
Natural GP coordinates have

```text
alpha = 1
beta^x = s sqrt(r0 / x)
```

However, the evolution gauge variables do not necessarily need to be initialized
to the natural GP shift. The older tentative reproduction-roadmap startup was

```text
alpha(0) = 1
beta^i(0) = 0
```

while keeping `gamma_ij` and nonzero `K_ij` from the GP slice. In that case the
data are GP-slice geometric data, not a fully stationary GP-coordinate
evolution.

Stage 4AO-A now locks the GL-validation background/startup family separately in
`docs/derivations/stage4AO_A_uniform_gp_background_residual.md`: frozen-GP
stationarity uses `alpha=1`, `beta^x=sqrt(r0/x)`, `beta^z=0`, and the live
moving-puncture startup keeps that GP shift with `B^i=0`, producing a
prescribed lapse startup residual in unmodified GRChombo. The 4AO-D validation
harness cancels that zeroth-order drift with the fixed, field-independent
source `S_alpha(x)=+3 sqrt(r0/x^3)`, computed from the locked analytic
background and locked `r0`, not from evolved fields. This source is validation
harness only and not production Stage 4AR/4AS wiring. Do not mix the tentative
zero-shift startup with a stationarity claim unless its own analytic residual
targets are derived.

Do not set `K_ij = 0` just because the evolution shift `beta^i` is initialized
to zero. `K_ij` is part of the geometric initial data and should not be
recomputed from the chosen gauge-startup shift.

No `z` shift is expected for the uniform string. Hidden sphere shifts are not
independent gridded fields in the reduced system.

### Gamma Variables, `Theta`, And Gauge Drivers

The conformal connection functions `Gamma^i` should be consistent with the
initial conformal metric and the reduced-coordinate convention. If the
conformal metric is flat and only `chi` is perturbed, the unperturbed `Gamma^i`
should be zero, but the perturbation and cartoon coordinate terms may produce
nontrivial reduced-source contributions. This must be derived rather than
assumed from the current BinaryBH scaffold.

Initial `Theta` should start at zero unless a reviewed CCZ4 initialization
reason says otherwise. Gauge driver variables such as `B^i` should also start
from the reviewed gauge convention, not from inherited BinaryBH assumptions.

### Initial Constraints

The unperturbed uniform string should satisfy the 4+1 vacuum Hamiltonian and
momentum constraints analytically. The GL perturbation of `chi` is expected to
introduce small constraint violations by design. Stage 3 validation should
therefore distinguish:

- unperturbed uniform-string constraint checks;
- perturbed `epsilon != 0` constraint amplitudes;
- damping behavior in short CCZ4 tests before any GL-production run.

## GL Perturbation Design

The planned seed perturbation from the reproduction roadmap is a conformal-factor
perturbation localized near the horizon:

```text
chi = 1 + epsilon sin(2 pi n z / L) exp[-(x / r0 - r0 / x)^2]
```

Planned defaults:

- `epsilon = 0.01`;
- `n = 1`;
- `r0 = 1`;
- first target case: `L / r0 = 10`.

This is only the planned perturbation. It is not implemented in
`BlackStringToy` yet. Before coding, decide whether this expression multiplies
or replaces the unperturbed `chi`, how it interacts with turduckening near the
interior, and whether additional fields must be recomputed for a consistent
conformal decomposition.

## Turduckening And Interior Handling

An interior cutoff is needed because the GP-like expressions contain powers of
`x` such as `sqrt(r0 / x)` and become singular at `x = 0`, while the numerical
grid may include or approach the cartoon axis/interior. The reproduction roadmap
uses a turduckening-style prescription: for `x < eps_cut`, evaluate initial-data
fields at `x = eps_cut`, with

```text
eps_cut = 0.1 r0
```

Quantities that need a reviewed smoothing or cutoff strategy include:

- lapse and shift if using the natural GP shift;
- `K` and all `A` components;
- `chi` and its GL perturbation;
- `hww` and any coordinate factors used in hidden-sphere reconstruction;
- Gamma variables and any derived constraint diagnostics near the interior.

The exact smoothing profile is not chosen yet. A hard clamp is simple but may
produce derivative artifacts; a smooth transition may be needed before constraint
and convergence tests are meaningful.

## Boundary And Periodicity Assumptions

The black-string domain should use compact periodic `z`:

```text
z ~ z + L
```

The radial/cartoon direction needs an outer boundary far enough away that it is
not causally connected to the string during the target evolution window. The
reproduction roadmap uses `x_outer = 256 r0`. The long-term outer boundary will
likely use a Sommerfeld-style condition or another reviewed outgoing condition.

The cartoon axis / radial-origin treatment is a separate regularity problem.
The reduced variables must remain regular as `x -> 0`, and any parity or
one-sided handling inherited from BinaryBH reflective boundaries is not a
validated black-string choice.

Inherited BinaryBH assumptions that are not appropriate long term:

- binary-puncture positions and tracking;
- puncture-centered tagging;
- noncompact Cartesian 3D domain assumptions;
- BinaryBH reflective/Sommerfeld parity choices copied from a 3+1D example;
- Weyl4 extraction settings as a default diagnostic for this reduced 5D system.

## Do Not Implement Yet Checklist

Resolve these before coding Stage 3 initial data:

- [ ] Exact coordinate convention for `x`, including whether `x` is the areal
  radial coordinate used in the horizon formula.
- [ ] Exact GP sign convention for shift and extrinsic curvature, matched to
  GRChombo's `K_ij` sign.
- [ ] Confirm with the paper/code whether to follow the roadmap choice
  `alpha(0)=1`, `beta^i(0)=0` while using GP-slice `gamma_ij` and nonzero
  `K_ij`.
- [ ] Exact `UserVariables.hpp` enum layout, including `hww/Aww` and the
  verified `c_hww == c_K - 1` AH convention.
- [ ] Whether to begin with a `DIM=3` sanity check or go directly to
  `DIM=2`, `GR_SPACEDIM=4`.
- [ ] How to pass or encode `GR_SPACEDIM=4` reliably in the editable build
  workflow.
- [ ] Validation tests for unperturbed uniform-string Hamiltonian and momentum
  constraints.
- [ ] Short-run tests showing the planned perturbation's constraint violations
  remain controlled before GL growth dominates.
- [ ] Comparison against analytic horizon radius `r0` and initial area
  `A0 = 4 pi r0^2 L`.
- [ ] Exact turduckening smoothing profile and cutoff validation.
