# Off-Diagonal Conformal Cartoon Notes

Status: Stage 3G symbolic validation scaffold. This is documentation and
symbolic checking only. It is not GRChombo evolution code, does not implement
CCZ4 RHS source terms, and does not establish physical correctness of an
evolution.

## Purpose

Stage 3F translated the diagonal reduced physical metric into conformal
cartoon variables. Stage 3G extends that algebra to a reduced gridded block
with nonzero `h_xz` / `gamma_xz`.

The target reduced physical metric is

```text
dl^2
  = gamma_xx dx^2
    + 2 gamma_xz dx dz
    + gamma_zz dz^2
    + x^2 gamma_ww dOmega_2^2.
```

The gridded directions are `x,z`. The two hidden transverse directions are
represented by the hidden Cartesian-like component `gamma_ww`; the external
factor `x^2` is supplied by the spherical/cartoon reconstruction and is not
stored in `gamma_ww` or `hww`.

## Determinants

For the physical reduced block,

```text
D_gamma = gamma_xx gamma_zz - gamma_xz^2.
```

With hidden multiplicity

```text
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2,
```

the cartoon/Cartesian-like physical determinant is

```text
det gamma_4D = D_gamma gamma_ww^2.
```

The conformal split uses

```text
h_ij = chi gamma_ij,
gamma_ij = chi^{-1} h_ij.
```

Thus

```text
chi = [(gamma_xx gamma_zz - gamma_xz^2) gamma_ww^2]^(-1/4).
```

For the conformal block,

```text
D_h = h_xx h_zz - h_xz^2,
det h_4D = D_h hww^2.
```

The conformal determinant condition is

```text
det h_4D = (h_xx h_zz - h_xz^2) hww^2 = 1.
```

As in Stage 3F, this is the cartoon/Cartesian-like determinant condition. It
does not include spherical-coordinate Jacobian factors from `x^2 dOmega_2^2`.

## Inverse Conformal Metric

For the off-diagonal reduced block,

```text
h^xx = h_zz / D_h
h^zz = h_xx / D_h
h^xz = -h_xz / D_h
h^ww = 1 / hww.
```

The Stage 3G script verifies that this inverse multiplies the reduced
conformal metric block to the identity.

## Full 4D Tracelessness

The conformal tracelessness condition is still full four-spatial-dimensional,
not a two-dimensional gridded trace:

```text
h^xx A_xx
  + 2 h^xz A_xz
  + h^zz A_zz
  + 2 h^ww Aww
  = 0.
```

The first factor of `2` is the off-diagonal symmetric contraction in the
`x,z` block. The second factor of `2` is the hidden-direction multiplicity.

Solving for the hidden component gives

```text
Aww
  = -hww/2 (h^xx A_xx + 2 h^xz A_xz + h^zz A_zz).
```

## Extrinsic-Curvature Translation

For `GR_SPACEDIM = 4`,

```text
K_ij = chi^{-1} (A_ij + h_ij K / 4).
```

This includes the off-diagonal component

```text
K_xz = chi^{-1} (A_xz + h_xz K / 4).
```

The trace `K` is the full 4D trace:

```text
K = gamma^xx K_xx
  + 2 gamma^xz K_xz
  + gamma^zz K_zz
  + 2 gamma^ww K_ww.
```

The denominator is `/4`, set by `GR_SPACEDIM`, not by `CH_SPACEDIM = 2` or
ordinary 3+1 intuition.

## Repository Checks

`offdiagonal_conformal_cartoon_sympy.py` verifies:

- physical determinant `D_gamma` and `det gamma_4D`;
- conformal determinant relation and determinant normalization ratio;
- inverse conformal metric components;
- full 4D tracelessness with both off-diagonal and hidden multiplicity factors;
- reconstruction of `K_xx`, `K_xz`, `K_zz`, and `K_ww` using `/4`;
- denominator guard: `/4` works, while `/2` and `/3` fail under a numeric
  substitution;
- diagonal-limit regression back to Stage 3F formulas when
  `h_xz = gamma_xz = A_xz = K_xz = 0`.

## Current Run Result

Run command:

```text
python3 docs/derivations/offdiagonal_conformal_cartoon_sympy.py
```

Exact output:

```text
Stage 3G off-diagonal conformal cartoon checks
Reduced block includes h_xz / gamma_xz
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2
PASS physical reduced-block determinant D_gamma = 0
PASS physical 4D determinant det gamma_4D = 0
PASS conformal reduced-block determinant from physical block = 0
PASS conformal 4D determinant relation = 0
PASS det h_4D normalization ratio = 0
PASS inverse h xx component = 0
PASS inverse h xz component = 0
PASS inverse h zx component = 0
PASS inverse h zz component = 0
PASS inverse h ww component = 0
PASS full-4D conformal tracelessness solved for Aww = 0
PASS solve(trace=0) returns off-diagonal Aww formula = 0
PASS K_xx reconstruction with /4 = 0
PASS K_xz reconstruction with /4 = 0
PASS K_zz reconstruction with /4 = 0
PASS K_ww reconstruction with /4 = 0
PASS A_ij definitions are full-4D tracefree = 0
PASS p=4 dimension denominator works = 0
PASS p=2 dimension denominator fails as expected = 79/4
PASS p=3 dimension denominator fails as expected = 79/12
PASS diagonal limit D_gamma = 0
PASS diagonal limit D_h = 0
PASS diagonal limit tracefree A relation = 0
PASS diagonal limit Aww solution = 0
PASS diagonal limit K trace = 0
PASS diagonal limit det h_4D = 0
All Stage 3G off-diagonal conformal cartoon checks passed.
```

## Limitations

- Algebraic conformal/cartoon translation only.
- No C++ variables or source terms are added.
- Full CCZ4 RHS terms remain future work.
- Regularity near `x = 0` is not addressed here.
- Symbolic identities do not by themselves establish physical correctness of
  an evolution.
