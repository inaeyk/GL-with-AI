# Conformal Cartoon Translation Notes

Status: Stage 3F symbolic translation scaffold. This is documentation and
symbolic checking only. It is not GRChombo evolution code and does not
implement CCZ4 RHS source terms.

## Purpose

Stages 3D and 3E validated hidden-sector Ricci structure mostly in physical
metric variables, using

```text
gamma_ww = q(x,z).
```

Stage 3F translates that physical metric bookkeeping into the conformal
variables that GRChombo-style CCZ4 stores and evolves.

## Metric Dictionary

For the diagonal reduced physical metric,

```text
dl^2 = gamma_xx dx^2 + gamma_zz dz^2 + x^2 gamma_ww dOmega_2^2,
```

with gridded indices `A,B in {x,z}`, the conformal dictionary is

```text
gamma_xx = chi^{-1} h_xx
gamma_zz = chi^{-1} h_zz
gamma_ww = chi^{-1} hww.
```

Thus

```text
dl^2 = chi^{-1} h_xx dx^2
     + chi^{-1} h_zz dz^2
     + x^2 chi^{-1} hww dOmega_2^2.
```

Define

```text
q = gamma_ww = hww / chi.
```

The physical transverse sphere radius is

```text
R_sphere = x sqrt(gamma_ww) = x sqrt(hww / chi).
```

The angular metric component is

```text
g_theta theta = x^2 gamma_ww = x^2 hww / chi,
```

not merely `hww` and not merely `gamma_ww`. The factor `x^2` is supplied by
the spherical/cartoon reconstruction.

## Determinant Condition

The conformal split is not fully defined until the conformal determinant
condition is imposed. For the diagonal cartoon/Cartesian-like
four-spatial-dimensional metric block, the physical determinant is

```text
det gamma_4D = gamma_xx gamma_zz gamma_ww^2,
```

because the two hidden directions both contribute `gamma_ww`. Therefore

```text
chi = (det gamma_4D)^(-1/4)
    = (gamma_xx gamma_zz gamma_ww^2)^(-1/4),
```

and

```text
h_xx = chi gamma_xx
h_zz = chi gamma_zz
hww  = chi gamma_ww.
```

This gives

```text
det h_4D = h_xx h_zz hww^2 = 1.
```

This is the cartoon/Cartesian-like conformal determinant. It does not include
the spherical-coordinate Jacobian factors `x^4 sin^2(theta)`. The angular
metric component is still

```text
g_theta theta = x^2 gamma_ww = x^2 hww / chi,
```

but determinant normalization is imposed on the reduced Cartesian-like
variables with hidden multiplicity.

## Derivatives Of q

For `A in {x,z}`,

```text
partial_A q
  = chi^{-1} partial_A hww
    - chi^{-2} hww partial_A chi.
```

For `A,B in {x,z}`,

```text
partial_A partial_B q
  =
  chi^{-1} partial_A partial_B hww
  - chi^{-2} [partial_A hww partial_B chi
             + partial_B hww partial_A chi
             + hww partial_A partial_B chi]
  + 2 chi^{-3} hww partial_A chi partial_B chi.
```

The logarithmic form is often cleaner:

```text
partial_A log q = partial_A log hww - partial_A log chi.
```

Also,

```text
partial_A partial_B q / q
  =
  partial_A partial_B log q
  + partial_A log q partial_B log q.
```

## Physical Radius Function

The warped-product radius used in Stage 3E becomes

```text
f = x sqrt(q) = x sqrt(hww / chi).
```

Therefore

```text
partial_x log f
  = 1/x + 1/2 (partial_x log hww - partial_x log chi)
```

and

```text
partial_z log f
  = 1/2 (partial_z log hww - partial_z log chi).
```

The first derivatives are

```text
partial_A f = f partial_A log f.
```

These identities are the bridge from the physical-radius Ricci checks in Stage
3E to conformal variables `chi` and `hww`.

## Extrinsic Curvature Variables

For `GR_SPACEDIM = 4`, the conformal decomposition is

```text
K_AB = chi^{-1} (A_AB + h_AB K / 4)
K_ww = chi^{-1} (Aww + hww K / 4).
```

Here `K_ww` is the hidden Cartesian-like physical extrinsic-curvature component
for each hidden direction.

The full four-spatial-dimensional conformal tracelessness condition is

```text
h^{AB} A_AB + 2 Aww / hww = 0.
```

For diagonal reduced `h_AB`, this becomes

```text
A_xx / h_xx + A_zz / h_zz + 2 Aww / hww = 0.
```

Solving for the hidden component gives

```text
Aww = -hww/2 (A_xx/h_xx + A_zz/h_zz).
```

This is not a two-dimensional trace. The trace is over the full four-dimensional
physical spatial geometry, with hidden multiplicity

```text
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2.
```

## Repository Checks Added After Review

The first Stage 3F script checked the derivative identities for `q = hww/chi`,
the physical-radius identities, and the diagonal tracelessness solve. After
review, the repository script also checks:

- the cartoon/Cartesian-like determinant condition `det h_4D = 1`;
- the physical-to-conformal metric round trip;
- `K_xx`, `K_zz`, and `K_ww` reconstruction from conformal variables;
- full 4D conformal tracelessness with hidden multiplicity;
- the load-bearing `/4` denominator in the `GR_SPACEDIM = 4` decomposition.

The last check intentionally reconstructs `K_ww` with denominator `p`. It
passes for `p = 4` and fails for `p = 2` and `p = 3` under a positive numeric
substitution. This makes the silent-failure hazard explicit: using
`CH_SPACEDIM = 2` or ordinary `3+1` intuition in this decomposition gives the
wrong hidden-sector reconstruction.

## Current Run Result

Run command:

```text
python3 docs/derivations/conformal_cartoon_translation_sympy.py
```

Exact output:

```text
Stage 3F conformal cartoon translation checks
q = gamma_ww = hww / chi
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2
PASS q definition = 0
PASS partial_x q identity = 0
PASS partial_z q identity = 0
PASS partial_xx q identity = 0
PASS partial_xz q identity = 0
PASS partial_zz q identity = 0
PASS partial_x log q identity = 0
PASS partial_z log q identity = 0
PASS partial_xx q over q logarithmic identity = 0
PASS partial_xz q over q logarithmic identity = 0
PASS partial_zz q over q logarithmic identity = 0
PASS partial_x log f identity = 0
PASS partial_z log f identity = 0
PASS partial_x f identity = 0
PASS partial_z f identity = 0
PASS physical-to-conformal round trip h_xx = 0
PASS physical-to-conformal round trip h_zz = 0
PASS physical-to-conformal round trip hww = 0
PASS physical-to-conformal round trip
PASS determinant condition det h_4D = 1 = 0
PASS K_xx reconstruction with /4 = 0
PASS K_zz reconstruction with /4 = 0
PASS K_ww reconstruction with /4 = 0
PASS full-4D conformal tracelessness with hidden multiplicity = 0
PASS p=4 dimension denominator works = 0
PASS p=2 dimension denominator fails as expected = 371/24
PASS p=3 dimension denominator fails as expected = 371/72
PASS diagonal conformal tracelessness solved for Aww = 0
PASS solve(trace=0) returns Aww formula = 0
All Stage 3F conformal cartoon translation checks passed.
```

## Limitations

- Diagonal reduced metric only.
- Off-diagonal `h_xz` / `g_xz` is deferred.
- Full CCZ4 RHS terms are not implemented.
- Regularity near `x = 0` is not addressed here.
- No C++ variables are added.
