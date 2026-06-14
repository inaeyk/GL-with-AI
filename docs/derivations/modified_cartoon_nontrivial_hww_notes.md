# Nontrivial `gamma_ww` Derivation Notes

Status: Stage 3D symbolic derivation scaffold. This is documentation and
symbolic checking only. It is not GRChombo evolution code and does not
implement modified-cartoon CCZ4 source terms.

## What Stage 3D Checks

Stage 3C verified the flat GP slice geometry, including the spherical
Christoffels, the `sin(theta)^2` cancellation, the map from angular
extrinsic-curvature components to `K_ww`, and the hidden trace multiplicity.

Stage 3D extends that infrastructure to a nontrivial hidden metric component

```text
q(x,z) = gamma_ww.
```

The goal is to isolate hidden-sector Christoffel and Ricci ingredients before
any C++ implementation.

## Metric Ansatz

The intended general reduced metric may contain an off-diagonal gridded block
component `g_xz = b(x,z)`. To keep this first symbolic scaffold tractable, the
Stage 3D script uses the diagonal reduced metric

```text
dl^2 =
  a(x,z) dx^2
  + c(x,z) dz^2
  + x^2 q(x,z) dtheta^2
  + x^2 q(x,z) sin(theta)^2 dphi^2.
```

The coordinate order in the script is

```text
(x, theta, phi, z).
```

Here `q(x,z)` is the physical hidden/cartoon component `gamma_ww`. The angular
metric components are `x^2 q` and `x^2 q sin(theta)^2` because spherical
coordinates supply the external radius factor. The cartoon hidden component is
only `gamma_ww = q`; setting `gamma_ww = x^2 q` would double-count the angular
radius.

## Key Christoffel Identities

For the diagonal metric, the script computes Christoffels from the metric and
checks

```text
Gamma^x_{theta theta}
  = - (1 / (2 a)) partial_x (x^2 q)

Gamma^z_{theta theta}
  = - (1 / (2 c)) partial_z (x^2 q)

Gamma^theta_{x theta}
  = (1 / 2) partial_x log(x^2 q)
  = 1/x + (1/2) partial_x log(q)

Gamma^theta_{z theta}
  = (1 / 2) partial_z log(q)
```

and the corresponding `phi` identities:

```text
Gamma^x_{phi phi} = Gamma^x_{theta theta} sin(theta)^2
Gamma^z_{phi phi} = Gamma^z_{theta theta} sin(theta)^2
Gamma^phi_{x phi} = Gamma^theta_{x theta}
Gamma^phi_{z phi} = Gamma^theta_{z theta}.
```

## Flat Limit

With

```text
a = 1
c = 1
q = 1
```

the script verifies that the Stage 3C spherical Christoffels are recovered:

```text
Gamma^x_{theta theta} = -x
Gamma^x_{phi phi} = -x sin(theta)^2
Gamma^theta_{x theta} = 1/x
Gamma^phi_{x phi} = 1/x
Gamma^theta_{z theta} = 0
Gamma^z_{theta theta} = 0.
```

## Angular Contraction And Multiplicity

For an angular tensor with

```text
T_phi phi = T_theta theta sin(theta)^2,
```

the script verifies the structural cancellation

```text
g^phi phi T_phi phi = g^theta theta T_theta theta.
```

Thus the `theta` and `phi` sectors give two equivalent hidden contributions.
For this project,

```text
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2.
```

## Ricci Structural Checks

The script computes the Ricci tensor for the diagonal metric and checks

```text
R_phi phi / sin(theta)^2 - R_theta theta = 0
```

and the angular contribution to the Ricci scalar,

```text
g^theta theta R_theta theta + g^phi phi R_phi phi
  = 2 g^theta theta R_theta theta.
```

It also verifies the flat-space Ricci limit:

```text
R_xx = 0
R_zz = 0
R_theta theta = 0
R_phi phi = 0.
```

These are structural checks. They are not final compact CCZ4 RHS formulas.

## Constant-q Ricci Regression

The general abstract Ricci expressions are still too large to fully audit by
eye. Stage 3D therefore includes a self-guarding constant-q regression case.
For

```text
a = 1
c = 1
q = q0
```

the metric is

```text
dl^2 = dx^2 + dz^2 + q0 x^2 dOmega_2^2.
```

This is a cone-like warped angular metric. It is flat only for `q0 = 1`. The
script asserts

```text
R_xx = 0
R_zz = 0
R_xz = 0
R_theta theta = 1 - q0
R_phi phi = (1 - q0) sin(theta)^2
R = 2 (1 - q0) / (q0 x^2).
```

The existing flat Ricci limit is the `q0 = 1` special case. This regression
does not complete the full CCZ4 RHS derivation, but it guards the Ricci
computation engine against regressions.

## Known Limitation: Non-constant q Derivative Terms Are Not Regression-tested

The constant-q regression is valuable because it checks a non-flat Ricci case
and guards the non-derivative angular/cone terms. However, since `q = q0` is
constant, all `q`-derivative terms vanish:

```text
partial_x q = 0
partial_z q = 0
partial_xx q = partial_xz q = partial_zz q = 0.
```

Thus the current regression does not assert the terms involving `partial_x q`,
`partial_z q`, second derivatives of `q`, or mixed derivative structure. These
derivative terms are likely the most error-prone hidden-sector pieces for the
future modified-cartoon Ricci/source terms.

A future Stage 3E or later derivation pass should add a non-constant `q(x,z)`
profile with known analytic curvature, or another independently checked
reference case, to regression-test these derivative terms. The Stage 3D script
is intentionally not modified for this limitation note; the gap is recorded as
an explicit limitation and future task.

## Current Run Result

Run command:

```text
python3 docs/derivations/modified_cartoon_nontrivial_hww_sympy.py
```

Exact output:

```text
Stage 3D nontrivial gamma_ww symbolic checks
Metric ansatz: diagonal a(x,z), c(x,z), q(x,z)=gamma_ww
Off-diagonal g_xz=b(x,z) is deferred in this scaffold.
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2
PASS Gamma^x_{theta theta} = -x*(x*Derivative(q(x, z), x) + 2*q(x, z))/(2*a(x, z))
PASS Gamma^z_{theta theta} = -x**2*Derivative(q(x, z), z)/(2*c(x, z))
PASS Gamma^theta_{x theta} = (x*Derivative(q(x, z), x) + 2*q(x, z))/(2*x*q(x, z))
PASS Gamma^theta_{z theta} = Derivative(q(x, z), z)/(2*q(x, z))
PASS Gamma^x_{phi phi} = -x*(x*Derivative(q(x, z), x) + 2*q(x, z))*sin(theta)**2/(2*a(x, z))
PASS Gamma^z_{phi phi} = -x**2*sin(theta)**2*Derivative(q(x, z), z)/(2*c(x, z))
PASS Gamma^phi_{x phi} = (x*Derivative(q(x, z), x) + 2*q(x, z))/(2*x*q(x, z))
PASS Gamma^phi_{z phi} = Derivative(q(x, z), z)/(2*q(x, z))
PASS flat Gamma^x_{theta theta} = -x
PASS flat Gamma^x_{phi phi} = -x*sin(theta)**2
PASS flat Gamma^theta_{x theta} = 1/x
PASS flat Gamma^phi_{x phi} = 1/x
PASS flat Gamma^theta_{z theta} = 0
PASS flat Gamma^z_{theta theta} = 0
PASS angular contraction phi-theta equality = T_theta_theta(x, z)/(x**2*q(x, z))
R_theta theta = (-2*x**2*a(x, z)**2*c(x, z)*Derivative(q(x, z), (z, 2)) + x**2*a(x, z)**2*Derivative(c(x, z), z)*Derivative(q(x, z), z) - 2*x**2*a(x, z)*c(x, z)**2*Derivative(q(x, z), (x, 2)) - x**2*a(x, z)*c(x, z)*Derivative(a(x, z), z)*Derivative(q(x, z), z) - x**2*a(x, z)*c(x, z)*Derivative(c(x, z), x)*Derivative(q(x, z), x) + x**2*c(x, z)**2*Derivative(a(x, z), x)*Derivative(q(x, z), x) - 8*x*a(x, z)*c(x, z)**2*Derivative(q(x, z), x) - 2*x*a(x, z)*c(x, z)*q(x, z)*Derivative(c(x, z), x) + 2*x*c(x, z)**2*q(x, z)*Derivative(a(x, z), x) + 4*a(x, z)**2*c(x, z)**2 - 4*a(x, z)*c(x, z)**2*q(x, z))/(4*a(x, z)**2*c(x, z)**2)
R_phi phi / sin(theta)^2 = (-2*x**2*a(x, z)**2*c(x, z)*Derivative(q(x, z), (z, 2)) + x**2*a(x, z)**2*Derivative(c(x, z), z)*Derivative(q(x, z), z) - 2*x**2*a(x, z)*c(x, z)**2*Derivative(q(x, z), (x, 2)) - x**2*a(x, z)*c(x, z)*Derivative(a(x, z), z)*Derivative(q(x, z), z) - x**2*a(x, z)*c(x, z)*Derivative(c(x, z), x)*Derivative(q(x, z), x) + x**2*c(x, z)**2*Derivative(a(x, z), x)*Derivative(q(x, z), x) - 8*x*a(x, z)*c(x, z)**2*Derivative(q(x, z), x) - 2*x*a(x, z)*c(x, z)*q(x, z)*Derivative(c(x, z), x) + 2*x*c(x, z)**2*q(x, z)*Derivative(a(x, z), x) + 4*a(x, z)**2*c(x, z)**2 - 4*a(x, z)*c(x, z)**2*q(x, z))/(4*a(x, z)**2*c(x, z)**2)
PASS R_phi phi / sin(theta)^2 - R_theta theta = 0
angular Ricci scalar contribution = (-2*x**2*a(x, z)**2*c(x, z)*Derivative(q(x, z), (z, 2)) + x**2*a(x, z)**2*Derivative(c(x, z), z)*Derivative(q(x, z), z) - 2*x**2*a(x, z)*c(x, z)**2*Derivative(q(x, z), (x, 2)) - x**2*a(x, z)*c(x, z)*Derivative(a(x, z), z)*Derivative(q(x, z), z) - x**2*a(x, z)*c(x, z)*Derivative(c(x, z), x)*Derivative(q(x, z), x) + x**2*c(x, z)**2*Derivative(a(x, z), x)*Derivative(q(x, z), x) - 8*x*a(x, z)*c(x, z)**2*Derivative(q(x, z), x) - 2*x*a(x, z)*c(x, z)*q(x, z)*Derivative(c(x, z), x) + 2*x*c(x, z)**2*q(x, z)*Derivative(a(x, z), x) + 4*a(x, z)**2*c(x, z)**2 - 4*a(x, z)*c(x, z)**2*q(x, z))/(2*x**2*a(x, z)**2*c(x, z)**2*q(x, z))
PASS angular Ricci contribution equals 2 g^theta theta R_theta theta = 0
Constant-q Ricci regression: a=1, c=1, q=q0
The flat Ricci check below is the q0=1 special case.
PASS constant-q R_xx = 0
PASS constant-q R_zz = 0
PASS constant-q R_xz = 0
PASS constant-q R_theta theta = 1 - q0
PASS constant-q R_phi phi = -(q0 - 1)*sin(theta)**2
PASS constant-q Ricci scalar = -2*(q0 - 1)/(q0*x**2)
PASS flat R_xx = 0
PASS flat R_zz = 0
PASS flat R_theta theta = 0
PASS flat R_phi phi = 0
All Stage 3D nontrivial gamma_ww symbolic checks passed.
```

## What Remains Unresolved

- Off-diagonal `g_xz = b(x,z)`.
- Conformal variables `chi` and `hww`.
- Full CCZ4 RHS terms.
- Regularized small-`x` limits.
- Translation into C++ source-term routines.
- Unit-test fixtures.
