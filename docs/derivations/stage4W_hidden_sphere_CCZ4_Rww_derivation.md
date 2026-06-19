# Stage 4W Hidden-Sphere CCZ4 Rww Derivation Lock

Status: derivation lock only. This note does not implement `R_ww`, conformal
factor Ricci corrections, full Ricci, CCZ4 RHS terms, finite differences,
small-axis regularization, grid reads, or evolution wiring.

## Setup And Notation

The reduced black-string cartoon bookkeeping uses physical spatial dimension
`d = 4`, gridded directions `a,b in {x,z}`, and hidden multiplicity `N = 2`.
The conformal cartoon line element is

```text
d tilde{s}^2 =
    h_xx dx^2
  + 2 h_xz dx dz
  + h_zz dz^2
  + x^2 h_ww dOmega_2^2.
```

The local hidden Cartesian-like bookkeeping version is

```text
h_IJ =
[[h_xx, h_xz, 0,    0   ],
 [h_xz, h_zz, 0,    0   ],
 [0,    0,    h_ww, 0   ],
 [0,    0,    0,    h_ww]]
```

Here `h_ww` is not `g_theta theta`. The external `x^2` factor comes from the
cartoon/spherical reconstruction. Full traces and trace-free projections are
four-dimensional, with denominator `1/4`, not `1/2` or `1/3`.

## Full 4D Trace Rule

For a symmetric tensor `S_IJ`, the full conformal trace is

```text
S = h^xx S_xx + 2 h^xz S_xz + h^zz S_zz + 2 h^ww S_ww.
```

The trace-free projection is

```text
S_IJ^TF = S_IJ - (1/4) h_IJ S.
```

Therefore `S_ww` affects not only the `ww` equation but also the trace
subtraction in the `xx`, `xz`, and `zz` equations.

## CCZ4 Hidden-Sphere Contribution Map

This section is a schematic contribution map, not an implementation of the
CCZ4 equations.

The `chi` equation receives hidden cartoon contribution through the full
cartoon shift divergence:

```text
partial_I beta^I =
    partial_x beta^x + partial_z beta^z + 2 beta^x / x.
```

The `h_ww` equation has a direct hidden Lie/cartoon contribution:

```text
partial_t h_ww contains
    beta^a partial_a h_ww
  + 2 h_ww beta^x / x
  - (1/2) h_ww partial_I beta^I
  - 2 alpha A_ww.
```

The `K` and `Theta` equations use the full physical Ricci scalar:

```text
R[gamma] =
    chi (
        h^xx R_xx[gamma]
      + 2 h^xz R_xz[gamma]
      + h^zz R_zz[gamma]
      + 2 h^ww R_ww[gamma]
    ).
```

The inner parenthesized quantity is the Stage 4I/4J conformal-inverse
contraction `h^{IJ} R_IJ[gamma]`; multiplying by `chi` gives the physical
scalar `gamma^{IJ} R_IJ[gamma]`.

The `A_IJ` equation contains the curvature/lapse block

```text
S_IJ = -D_I D_J alpha + alpha (R_IJ[gamma] + D_I Z_J + D_J Z_I),
```

with source

```text
chi S_IJ^TF = chi (S_IJ - (1/4) h_IJ S).
```

Consequences:

- `R_ww[gamma]` contributes directly to the `A_ww` equation.
- `R_ww[gamma]` contributes indirectly to `A_xx`, `A_xz`, and `A_zz`
  through `S`.
- Hidden `D_w D_w alpha` also enters both `A_ww` and the trace-free
  projection.
- Hidden `A_ww` enters scalar quantities such as `A_IJ A^IJ` with
  multiplicity two.

## Conformal Hidden Ricci Target

Let the reduced conformal base metric be

```text
H_ab = [[h_xx, h_xz],
        [h_xz, h_zz]]
```

and define the effective conformal sphere radius

```text
rho = x sqrt(h_ww).
```

With `D_a` the covariant derivative of the reduced conformal base metric
`H_ab`, the conformal-metric hidden Ricci target is the warped-product
expression

```text
tilde{R}_ww[h] =
    (1 - rho D^a D_a rho - D^a rho D_a rho) / x^2.
```

This is a geometric Ricci component of the conformal metric `h_IJ`, not an
added matter source. It is not yet the full physical Ricci component entering
the CCZ4 curvature block. The physical spatial metric is

```text
gamma_IJ = chi^{-1} h_IJ,
```

so the CCZ4 Ricci input is

```text
R_ww[gamma] = tilde{R}_ww[h] + R^chi_ww.
```

## Conformal-Factor Ricci Correction

For `d = 4` spatial dimensions and `gamma_IJ = chi^{-1} h_IJ`, the Ricci split
is

```text
R_IJ[gamma] = tilde{R}_IJ[h] + R^chi_IJ,
```

where

```text
R^chi_IJ =
    (1 / chi) D_I D_J chi
  + (1 / (2 chi)) h_IJ D^K D_K chi
  - (1 / (2 chi^2)) D_I chi D_J chi
  - (1 / chi^2) h_IJ D^K chi D_K chi.
```

Here all covariant derivatives and contractions on the right-hand side use the
conformal metric `h_IJ`.

For an SO(3)-symmetric scalar `chi`, `D_w chi = 0`, but `D_w D_w chi` need not
vanish because of the hidden-sphere connection. The hidden component is

```text
R^chi_ww =
    (1 / chi) D_w D_w chi
  + (h_ww / (2 chi)) D^K D_K chi
  - (h_ww / chi^2) D^K chi D_K chi.
```

The full conformal Laplacian and gradient norm decompose as

```text
D^K D_K chi = H^ab D_a D_b chi + 2 h^ww D_w D_w chi,
```

and

```text
D^K chi D_K chi = H^ab partial_a chi partial_b chi.
```

For any SO(3)-symmetric scalar `f`, the hidden second derivative is

```text
D_w D_w f = (rho D^a rho partial_a f) / x^2.
```

This split must be cross-checked against standard BSSN/CCZ4 conformal-Ricci
formulas and modified-cartoon references, not against Pau's code. The internal
anchors are the Stage 3A initial-data note for the `K_IJ` convention caveat,
the Stage 3H CCZ4 RHS block decomposition notes, and the Stage 4H Ricci/RHS
compatibility note.

## Varying-Chi Consistency Oracle Target

Future implementation check:

```text
Compute tilde{R}_ww[h] + R^chi_ww and compare it against a direct
metric-derivative computation of R_ww[gamma] after constructing
gamma_IJ = chi^{-1} h_IJ.
```

The first suggested documentation oracle is a varying-`chi` conformally flat
case:

```text
h_xx = h_zz = h_ww = 1,
h_xz = 0,
chi = 1 + a x.
```

Then `tilde{R}_ww[h] = 0`. At `x = 2`, `a = 0.1`, and `chi = 1.2`,

```text
D_w D_w chi = a / x,
D^K D_K chi = 2 a / x,
D^K chi D_K chi = a^2,
```

so

```text
R^chi_ww = 2 a / (chi x) - a^2 / chi^2
          = 11 / 144
          approximately 0.0763888889.
```

This is a proposed future oracle only. Stage 4W does not implement it.

## Sign-Convention Lock

The curvature/lapse block in the `A_IJ` equation uses the sign structure

```text
S_IJ = -D_I D_J alpha + alpha (R_IJ[gamma] + D_I Z_J + D_J Z_I),
```

and enters through the full 4D trace-free projection

```text
chi S_IJ^TF.
```

This sign must be pinned against the published CCZ4/BSSN formulation and
internally checked against the project's own `K_IJ` convention. The internal
condition is

```text
partial_t gamma_IJ = -2 alpha K_IJ + L_beta gamma_IJ.
```

The Stage 3A initial-data note currently records the working convention

```text
K_ij = (1 / (2 alpha)) (D_i beta_j + D_j beta_i - partial_t gamma_ij)
```

and explicitly warns that all `K_ij`, `K`, `A_ij`, and `A_ww` signs must flip
if GRChombo uses the opposite convention. This sign check should be made
against `docs/physics_notes/stage3A_black_string_initial_data.tex` and the
project's BSSN/CCZ4 convention notes, not against Pau's code.

## Expanded Gradient Piece And Guarded Terms

Write `W = h_ww`. Then

```text
rho_x = sqrt(W) + x W_x / (2 sqrt(W)),
rho_z = x W_z / (2 sqrt(W)).
```

The expansion of

```text
(1 - D^a rho D_a rho) / x^2
```

contains the regularity-sensitive term

```text
h^xx (h_xx - h_ww) / x^2.
```

This is the term protected by the Stage 4Q/4R/4U checked path.

The same expansion also exposes the off-diagonal regularity-sensitive term

```text
h^xz h_xz / x^2.
```

Therefore a full safe `R_ww[gamma]` implementation requires both

```text
h_xx - h_ww = O(x^2)
```

and

```text
h_xz = O(x)
```

near the cartoon axis. The first checked source-facing path exists through
Stage 4Q/4R/4U. Stage 4X adds the local checked `h_xz / x` ingredient needed by
future away-axis formulas. A stronger grid-level or two-sided parity validation
of `h_xz = O(x)` remains a separate future validation task. Therefore full
`R_ww[gamma]` implementation is still a later formula stage, not Stage 4W or
Stage 4X.

Stage 4Y implements only the first checked singular/regularity-sensitive
gradient sub-block of this conformal target,

```text
G_sing = (h_zz / D) Delta_xw - q_xz^2 / D,
```

with `D = h_xx h_zz - h_xz^2`, `Delta_xw` minted through the Stage 4U/4R
checked metric-difference path, and `q_xz` minted through the Stage 4X checked
quotient path. After review, Stage 4Y single-sources these raw determinant
inputs and checked singular ingredients from one local metric point through a
non-forgeable input package, so callers cannot mix checked `Delta_xw` and
`q_xz` from one point with determinant data from another. This still does not
implement full `tilde{R}_ww[h]`, `R^chi_ww`, physical `R_ww[gamma]`, full
Ricci, CCZ4 RHS, grid reads, evolution wiring, finite-axis regularization, or
global axis parity validation.

## First-Principles h_xz Parity

The needed `h_xz = O(x)` condition follows from regularity and reflection
symmetry about the cartoon axis, not from an external code convention.

Under the local reflection `x -> -x` with `z` fixed,

```text
dx -> -dx,
dz -> dz.
```

The cross term

```text
2 h_xz dx dz
```

must be invariant. Therefore `h_xz` must be odd under reflection:

```text
h_xz(-x, z) = -h_xz(x, z).
```

Near the axis this means

```text
h_xz = x q(x^2, z) = O(x).
```

Since

```text
h^xz = -h_xz / (h_xx h_zz - h_xz^2),
```

regular data has `h^xz = O(x)`. The combination

```text
h^xz h_xz / x^2
```

can remain finite only with this parity behavior.

## Oracle Checks

Future implementation must preserve the Stage 4G-compatible oracles below.
For constant `chi = 1`, these are simultaneously conformal and physical Ricci
oracles.

Flat:

```text
h_xx = h_zz = h_ww = 1, h_xz = 0
=> R_ww = 0.
```

Constant cone:

```text
h_xx = h_zz = 1, h_xz = 0, h_ww = q0 = 4, x = 2
=> R_ww = -3/4 = -0.75.
```

Nonconstant hidden metric:

```text
h_xx = h_zz = 1, h_xz = 0, h_ww = (1 + x)^2, x = 1
=> R_ww = -12.
```

## Stage Conclusion

Stage 4W locks the conformal hidden Ricci target `tilde{R}_ww[h]`, the
required conformal-factor correction `R^chi_ww`, and the physical CCZ4 input
`R_ww[gamma]`. It also identifies the needed local `h_xz / x` ingredient and
the stronger unresolved `h_xz = O(x)` validation question. No
Ricci/RHS/evolution code is added.

Stage 4X provides the local checked `h_xz / x` ingredient needed by future
away-axis formulas. A stronger grid-level or two-sided parity validation of
`h_xz = O(x)` remains a separate future validation task. Stage 4X does not
implement the `h^xz h_xz / x^2` source expression, conformal hidden Ricci,
conformal-factor Ricci, physical `R_ww[gamma]`, CCZ4 RHS, or evolution wiring.
It also does not prove global parity of the numerical data.

Stage 4Y implements only the checked singular/regularity-sensitive gradient
sub-block

```text
G_sing = (h_zz / D) Delta_xw - q_xz^2 / D,
```

where `Delta_xw = (h_xx - h_ww) / x^2` is consumed through the Stage
4U/4R checked path and `q_xz = h_xz / x` is consumed through the Stage 4X
checked path. Stage 4Y is still not full `tilde{R}_ww[h]`, not `R^chi_ww`,
not physical `R_ww[gamma]`, not full Ricci, not CCZ4 RHS, and not evolution
wiring.

The integrated roadmap for the remaining hidden-sphere Ricci and CCZ4 gates is
`docs/implementation/stage4_hidden_sphere_Rww_plan.md`. It assigns concrete
future ownership for the Hessian block, `R^chi_ww`, the hard split-vs-direct
physical Ricci identity gate, true `h_xz` parity validation, `hat_Gamma^x`,
and smoke-freeze removal.

Stage 4Z implements the next conformal sub-block,

```text
G_grad =
    -(C / D) p_W
  +  (q_xz W_z) / D
  -  (C W_x^2 - 2 B W_x W_z + A W_z^2) / (4 W D),
```

with checked local `p_W = W_x / x`, checked `q_xz = B / x`, and raw
determinant data single-sourced from one local metric point. It is still not
the Hessian block, not full `tilde{R}_ww[h]`, not `R^chi_ww`, not physical
`R_ww[gamma]`, not full Ricci, not CCZ4 RHS, and not evolution wiring.

The next coding stage after Stage 4Z should remain separately reviewed before
expanding toward the physical `R_ww[gamma]` target:

```text
Future Stage: Hessian conformal hidden Ricci derivation lock, eventually
assembling tilde R_ww[h] only after each singular, first-derivative, and
Hessian piece has a reviewed formula and oracle.
```
