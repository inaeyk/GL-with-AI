# Modified-Cartoon Geometry Derivation Notes

Status: Stage 3C derivation scaffold. This is documentation and symbolic
checking only. It is not GRChombo evolution code and does not implement
modified-cartoon CCZ4 source terms.

## Coordinate Setup

The SymPy script checks the unperturbed GP spatial slice in coordinates

```text
(x, theta, phi, z)
```

with four-spatial-dimensional line element

```text
dl^2 = dx^2 + x^2 dtheta^2 + x^2 sin(theta)^2 dphi^2 + dz^2.
```

This is the spherical-coordinate representation of the transverse flat
geometry plus the compact string direction. The reduced cartoon grid will later
use gridded coordinates `x,z` and hidden/reconstructed transverse directions
`w1,w2`.

## What The Script Checks

`modified_cartoon_geometry_sympy.py` computes Christoffel symbols directly from
the metric and verifies the spherical components used in the Stage 3B note:

```text
Gamma^x_{theta theta} = -x
Gamma^x_{phi phi} = -x sin(theta)^2
Gamma^theta_{x theta} = 1/x
Gamma^phi_{x phi} = 1/x
```

It then defines the natural GP radial shift

```text
beta^x = s sqrt(r0 / x)
```

with `beta_x = beta^x` for the flat radial metric, and computes

```text
K_ij = 1/2 (D_i beta_j + D_j beta_i)
```

for a time-independent spatial metric and `alpha = 1`.

The verified extrinsic-curvature components are

```text
K_xx = -(s/2) sqrt(r0 / x^3)
K_theta theta = x beta^x = s sqrt(r0 x)
K_phi phi = x beta^x sin(theta)^2
K_zz = 0
```

These checks use the sign convention stated in the Stage 3A and Stage 3B
notes. If the final GRChombo convention is opposite, all `K_ij`, `K`, `A_ij`,
and `Aww` signs must be flipped consistently.

## Cartoon Contraction Check

The spherical angular components map to equal hidden cartoon contributions
after contraction:

```text
gamma^theta theta K_theta theta
  = (1 / x^2) (x beta^x)
  = beta^x / x

gamma^phi phi K_phi phi
  = (1 / (x^2 sin(theta)^2)) (x beta^x sin(theta)^2)
  = beta^x / x
```

The explicit `sin(theta)^2` cancellation is the machine-checked reason the two
suppressed directions contribute equally in the SO(3)-symmetric reduction.
The hidden cartoon component for this check is therefore

```text
K_ww = beta^x / x.
```

## Hidden-Direction Multiplicity

For the intended reduced black-string target,

```text
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2.
```

Diagonal traces must include two equivalent hidden contributions:

```text
T^x_x + T^z_z + 2 T^w_w.
```

For the GP extrinsic-curvature check, the script verifies

```text
K = K^x_x + K^z_z + 2 K^w_w
  = (3s/2) sqrt(r0 / x^3).
```

## What This Does Not Check

This script validates only basic geometry and the GP `K_ij` formulas used by
the Stage 3A and Stage 3B notes. It does not validate:

- nontrivial `chi`, `hww`, or `gamma_ww(x,z)`;
- Ricci tensor or Ricci scalar hidden-direction contributions;
- conformal CCZ4 RHS source terms;
- gauge RHS source terms;
- small-`x` regularized limits;
- constraints for perturbed GL data;
- any C++ implementation.

## Current Run Result

Stage 3C successfully ran the script with:

```text
python3 docs/derivations/modified_cartoon_geometry_sympy.py
```

Exact output:

```text
Stage 3C modified-cartoon geometry symbolic checks
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2
PASS Gamma^x_{theta theta} = -x
PASS Gamma^x_{phi phi} = -x*sin(theta)**2
PASS Gamma^theta_{x theta} = 1/x
PASS Gamma^phi_{x phi} = 1/x
PASS K_xx = -sqrt(r0)*s/(2*x**(3/2))
PASS K_theta theta = sqrt(r0)*s*sqrt(x)
PASS K_phi phi = sqrt(r0)*s*sqrt(x)*sin(theta)**2
PASS K_zz = 0
PASS gamma^theta theta K_theta theta = sqrt(r0)*s/x**(3/2)
PASS gamma^phi phi K_phi phi = sqrt(r0)*s/x**(3/2)
PASS K_ww = sqrt(r0)*s/x**(3/2)
PASS K trace = 3*sqrt(r0)*s/(2*x**(3/2))
All Stage 3C modified-cartoon geometry checks passed.
```

Status: passed. This verifies the basic Christoffels, GP extrinsic-curvature
components, angular contractions, `K_ww`, and trace multiplicity implemented in
the Stage 3C scaffold.

## Next Derivation Targets

- Reduced inverse metric with nontrivial `hww`.
- Christoffels for general `gamma_ww(x,z)`.
- Ricci tensor/scalar hidden contributions.
- Conformal CCZ4 source terms.
- Regularized small-`x` limits.
- Conversion of symbolic results into unit-test fixtures.
