# Nonconstant `q(x,z)` Ricci Regression Notes

Status: Stage 3E symbolic derivation scaffold. This is documentation and
symbolic checking only. It is not GRChombo evolution code and does not
implement modified-cartoon CCZ4 source terms.

## Why Stage 3E Exists

Stage 3D added a useful constant-`q0` Ricci regression, but that case cannot
guard the derivative sector because

```text
partial_x q = partial_z q = partial_xz q = 0
```

and all second derivatives vanish. Those derivative terms are likely among the
most error-prone hidden-sector pieces in the future modified-cartoon Ricci and
CCZ4 source terms.

Stage 3E introduces a nonconstant `q(x,z)` profile with an independent
warped-product Ricci reference.

## Metric And Profile

The script uses coordinates

```text
(x, theta, phi, z)
```

and the metric

```text
dl^2 = dx^2 + dz^2 + f(x,z)^2 dOmega_2^2
```

with

```text
f(x,z) = x P(x,z)
P(x,z) = 1 + lambda x + mu z
q(x,z) = P(x,z)^2.
```

Equivalently,

```text
dl^2 = dx^2 + dz^2 + x^2 q(x,z) dOmega_2^2.
```

Here `q(x,z)` is the physical hidden/cartoon component `gamma_ww`. The angular
metric components are `g_theta theta = x^2 q` and
`g_phi phi = x^2 q sin(theta)^2`; the cartoon hidden component itself is
`gamma_ww = q`, not `x^2 q`.

This profile activates

```text
partial_x q = 2 lambda (1 + lambda x + mu z)
partial_z q = 2 mu (1 + lambda x + mu z)
partial_x partial_z q = 2 lambda mu.
```

## Warped-product Ricci Reference

For

```text
g = dx^2 + dz^2 + f(x,z)^2 dOmega_2^2
```

with a two-dimensional unit-sphere fiber and flat base coordinates `x,z`, the
independent reference formulas are

```text
R_AB = -2 f^{-1} partial_A partial_B f
```

for base indices `A,B in {x,z}` and

```text
R_theta theta = 1 - |grad f|^2 - f Delta f
R_phi phi = (1 - |grad f|^2 - f Delta f) sin(theta)^2
```

where

```text
|grad f|^2 = f_x^2 + f_z^2
Delta f = f_xx + f_zz.
```

The Ricci scalar reference is

```text
R = -4 (Delta f)/f + 2 (1 - |grad f|^2)/f^2.
```

## What The Script Asserts

`modified_cartoon_nonconstant_q_sympy.py` computes Christoffels and Ricci
directly from the metric, then asserts:

- `partial_x q`, `partial_z q`, and `partial_x partial_z q` are not
  identically zero;
- direct `R_xx`, `R_zz`, `R_xz`, `R_theta theta`, `R_phi phi`, and Ricci
  scalar match the warped-product reference formulas;
- the flat special case `lambda = 0`, `mu = 0` has vanishing Ricci tensor and
  scalar;
- the nontrivial substitution `lambda = 2`, `mu = 3` also gives zero
  direct-minus-reference differences.

## Caveat: Stage 3D Abstract Expression Is Guarded Transitively

The Stage 3E script recomputes Ricci using its own copy of symbolic engine
helper functions such as `christoffel_symbols` and `ricci_tensor`. Therefore
Stage 3E directly guards the Ricci engine on a nonconstant warped-product case
with active `q`-derivative terms.

It does not directly substitute the nonconstant profile into the exact abstract
Ricci expressions printed by the Stage 3D script. The Stage 3D abstract
expressions are validated transitively because they were produced by the same
kind of engine, and the engine has now been checked on several cases: flat
limits, the constant-`q0` cone, and a nonconstant warped product with active
derivative terms.

The residual risk is narrow: an edit or wiring mistake could corrupt the
printed or assembled abstract Stage 3D expression while leaving the engine
functions intact. This is acceptable for now, but it is recorded as a
limitation.

There are now duplicated symbolic engine helper functions across the Stage 3C,
Stage 3D, and Stage 3E scripts. This is acceptable short-term, but if the
engine logic changes, the copies could drift. A future cleanup should factor
common symbolic-geometry helpers into a shared module, for example
`docs/derivations/cartoon_geometry_common.py`, and have the derivation scripts
import it.

## Caveat: R_zz Is Zero For This Profile

For the chosen profile

```text
f = x (1 + lambda x + mu z),
```

the second `z` derivative satisfies

```text
f_zz = 0.
```

Therefore the warped-product reference gives

```text
R_zz = -2 f^{-1} f_zz = 0.
```

This does not mean the `z` derivative sector is untested. `partial_z q` is
nonzero, mixed derivative structure is active, and

```text
R_xz = -2 mu / f
```

is generally nonzero. The angular Ricci components also contain `mu^2` terms
through `|grad f|^2`, so `z` derivatives influence checked nonzero components.

The limitation is narrower: a bug that only appears in `R_zz` when `f_zz != 0`
would not be caught by this profile. A future optional refinement could use a
profile quadratic in `z`, making `f_zz != 0` and `R_zz` a nonzero target. This
is not urgent; the current Stage 3E check is still useful and non-vacuous.

## Current Run Result

Run command:

```text
python3 docs/derivations/modified_cartoon_nonconstant_q_sympy.py
```

Exact output:

```text
Stage 3E nonconstant q(x,z) Ricci regression
Metric: dl^2 = dx^2 + dz^2 + f(x,z)^2 dOmega_2^2
f = x*(1 + lambda*x + mu*z)
q = gamma_ww = (1 + lambda*x + mu*z)^2
PASS partial_x q = 2*lambda*(lambda*x + mu*z + 1)
PASS partial_z q = 2*mu*(lambda*x + mu*z + 1)
PASS partial_x partial_z q = 2*lambda*mu
PASS direct R_xx matches warped-product reference = 0
PASS direct R_zz matches warped-product reference = 0
PASS direct R_xz matches warped-product reference = 0
PASS direct R_theta theta matches warped-product reference = 0
PASS direct R_phi phi matches warped-product reference = 0
PASS direct Ricci scalar matches warped-product reference = 0
PASS flat-special R_xx = 0
PASS flat-special R_zz = 0
PASS flat-special R_xz = 0
PASS flat-special R_theta theta = 0
PASS flat-special R_phi phi = 0
PASS flat-special Ricci scalar = 0
PASS lambda=2, mu=3 R_xx direct-reference = 0
PASS lambda=2, mu=3 R_zz direct-reference = 0
PASS lambda=2, mu=3 R_xz direct-reference = 0
PASS lambda=2, mu=3 R_theta theta direct-reference = 0
PASS lambda=2, mu=3 R_phi phi direct-reference = 0
PASS lambda=2, mu=3 Ricci scalar direct-reference = 0
All Stage 3E nonconstant q(x,z) Ricci regression checks passed.
```

## What Remains Unresolved

- Off-diagonal `g_xz`.
- Non-flat base metric `a(x,z), c(x,z)`.
- Direct substitution of a nonconstant profile into the exact Stage 3D printed
  abstract Ricci expression.
- A nonconstant profile with `f_zz != 0`, making `R_zz` nonzero.
- Shared symbolic-geometry helper module for the Stage 3C/3D/3E scripts.
- Conformal variables `chi` and `hww`.
- Full CCZ4 RHS terms.
- Regularized small-`x` limits.
- Translation into C++ source-term routines.
- Unit-test fixtures.
