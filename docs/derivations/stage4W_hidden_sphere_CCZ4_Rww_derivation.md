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

Stage 4AD specializes this identity to the conformal factor. With

```text
A = h_xx,  B = h_xz,  C = h_zz,  W = h_ww,  D = A C - B^2,
H^xx = C / D,  H^xz = -B / D,  H^zz = A / D,
rho = x sqrt(W),
```

the hidden second derivative is

```text
D_w D_w chi =
    (rho / x^2) [
        H^xx rho_x chi_x
      + H^xz (rho_x chi_z + rho_z chi_x)
      + H^zz rho_z chi_z
    ].
```

The full conformal Laplacian is

```text
D^K D_K chi =
    (C / D) K_xx
  - (2 B / D) K_xz
  + (A / D) K_zz
  + (2 / W) D_w D_w chi,
```

where `2 / W` is the hidden multiplicity contribution `N_hidden = 2`; it must
not be dropped. The reduced-base scalar Hessian pieces are

```text
K_ab =
    chi_ab
  - Gamma^x_ab chi_x
  - Gamma^z_ab chi_z.
```

The conformal gradient norm is

```text
D^K chi D_K chi =
    (C / D) chi_x^2
  - (2 B / D) chi_x chi_z
  + (A / D) chi_z^2.
```

The Stage 4AD guard-stack design for Stage 4AE is: single-source all metric,
`chi`, and derivative values from one local point; require finite positive
`x`, `chi`, `W`, and `D`; reuse checked `q_xz = B/x` and checked
`p_W = W_x/x`; add a checked local `p_chi = chi_x/x` ingredient for the
leading hidden-Hessian term; and keep the policy away-axis only with no clamp,
epsilon substitution, global parity proof, or finite-axis regularity proof.

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

Stage 4W proposed this oracle without implementing it; Stage 4AE now locks it
in the local conformal-factor correction fixture.

Stage 4AD records, and the Stage 4AE fixture locks, the oracle set:

- Constant `chi` gives `R^chi_ww = 0`.
- The flat conformal metric with `chi = 1 + a x` gives the value above:
  `R^chi_ww = 11 / 144` at `x = 2`, `a = 0.1`, `chi = 1.2`.
- A z-dependent flat conformal candidate, `chi = 1 + b z`, gives
  `D_wD_w chi = 0`, `D^KD_K chi = 0`, and
  `D^K chi D_K chi = b^2`, hence `R^chi_ww = -b^2 / chi^2`. At `z = 3`,
  `b = 0.2`, `chi = 1.6`, this is `-1 / 64 = -0.015625`.
- The distinct nonsymmetric local jet gives
  `D_wD_w chi = 131/62`, `D^KD_K chi = 10430/2883`,
  `D^K chi D_K chi = 49/31`, and
  `R^chi_ww = 63341/48050`.

Stage 4AE implements this correction in
`code/BlackStringToy/CartoonConformalFactorRww.hpp`. The potentially
singular hidden Hessian is evaluated in the checked form

```text
D_wD_w chi =
    (C / D) [W p_chi + (x^2 / 2) p_W p_chi]
  - (q_xz / D) [
        W chi_z
      + (x^2 / 2) p_W chi_z
      + (x^2 / 2) W_z p_chi
    ]
  + (A / D) (W_z chi_z / 2),
```

with `q_xz = B/x`, `p_W = W_x/x`, and `p_chi = chi_x/x` minted by checked
local packages from the same point. The Stage 4AE fixture confirms the
nonsymmetric value against the test-only difference
`R_ww[gamma] - tilde R_ww[h]`. This is not the general Stage 4AF hard
identity gate. Checkpoint C / Claude Audit C is required before Stage 4AF.

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

Stage 4AA locks the Hessian conformal sub-block before any implementation.
Using `A = h_xx`, `B = h_xz`, `C = h_zz`, `W = h_ww`, and
`D = A C - B^2`,

```text
G^Hess_ww =
    -(sqrt(W) / x) [
        (C / D) H_xx
      - (2 B / D) H_xz
      + (A / D) H_zz
    ],

H_ab =
    rho_ab
  - Gamma^x_ab rho_x
  - Gamma^z_ab rho_z.
```

The locked derivatives are

```text
rho_x = sqrt(W) + x W_x / (2 sqrt(W)),
rho_z = x W_z / (2 sqrt(W)),

rho_xx =
    W_x / sqrt(W)
  + x W_xx / (2 sqrt(W))
  - x W_x^2 / (4 W^(3/2)),

rho_xz =
    W_z / (2 sqrt(W))
  + x W_xz / (2 sqrt(W))
  - x W_x W_z / (4 W^(3/2)),

rho_zz =
    x W_zz / (2 sqrt(W))
  - x W_z^2 / (4 W^(3/2)).
```

The reduced-base Christoffels are locked as

```text
Gamma^x_xx = [C A_x - B(2 B_x - A_z)] / (2 D),
Gamma^z_xx = [-B A_x + A(2 B_x - A_z)] / (2 D),

Gamma^x_xz = [C A_z - B C_x] / (2 D),
Gamma^z_xz = [-B A_z + A C_x] / (2 D),

Gamma^x_zz = [C(2 B_z - C_x) - B C_z] / (2 D),
Gamma^z_zz = [-B(2 B_z - C_x) + A C_z] / (2 D).
```

Stage 4AA records Hessian oracle values `0` for flat data, `0` for the
constant cone, and `-4` for the flat-base nonconstant hidden metric
`W = (1 + x)^2` at `x = 1`. In that last case the reviewed previous blocks
give `G_sing = -3` and `G_grad = -5`, so the assembled conformal target would
give `tilde R_ww = -12` after the Hessian block is implemented and reviewed.
A fully distinct nonsymmetric sample with all first and second derivative
slots nonzero is listed in
`docs/implementation/stage4_hidden_sphere_Rww_plan.md`. Claude Audit A
verifies that sample as

```text
G^Hess_ww = -8558 / 2883 = -2.9684356573014221...
```

For the same sample, the full conformal sum is

```text
G^sing_ww + G^grad_ww + G^Hess_ww =
    -3576 / 961 = -3.7211238293...
```

matching the independent Stage 4G conformal Ricci engine to machine precision
with residual about `4.44e-16`.

This distinct nonsymmetric oracle is required for Stage 4AB because it
exercises the off-diagonal Christoffels, `rho_xz`, `W_z`-dependent terms, and
the `(-2 B / D)` contraction. The flat, constant-cone, and `W=(1+x)^2`
oracles are not sufficient by themselves. Stage 4AB may now implement the
Hessian block, but only with this verified oracle included in the test.

Stage 4AA is the Hessian block only. It is not full `tilde{R}_ww[h]`, not
`R^chi_ww`, not physical `R_ww[gamma]`, not full Ricci, not CCZ4 RHS, and not
evolution wiring. It is away-axis only and does not prove global parity or
finite-axis regularity.

Stage 4AB implements the local away-axis Hessian sub-block

```text
G^Hess_ww =
    -(sqrt(W) / x) [
        (C / D) H_xx
      - (2 B / D) H_xz
      + (A / D) H_zz
    ],
```

in `code/BlackStringToy/CartoonConformalRwwHessianBlock.hpp`. The input
package is non-forgeable and single-sourced from one local metric/derivative
point, including the reduced-base first derivatives and `W` first/second
derivatives. The Stage 4AB fixture covers the flat, constant-cone,
nonconstant-`W`, and Claude-verified nonsymmetric oracles, plus invalid
axis/`W`/determinant/nonfinite-derivative rejection. Stage 4AB still does not
assemble full `tilde{R}_ww[h]`; that remains the Stage 4AC task.

Stage 4AC assembles that conformal target locally as

```text
tilde R_ww[h] = G_sing + G_grad + G_Hess.
```

The implementation lives in `code/BlackStringToy/CartoonConformalRww.hpp` and
uses a non-forgeable single-source input package that internally mints the
Stage 4Y singular, Stage 4Z gradient, and Stage 4AB Hessian inputs from one
local metric/derivative point. The fixture covers flat `0`, constant-cone
`-3/4`, nonconstant `W=(1+x)^2` giving `-12`, and the Claude-verified
nonsymmetric value `-3576/961`; it also compares the nonconstant and
nonsymmetric `chi=1` cases against the independent Stage 4G
metric-derivative Ricci helper. Stage 4AC is still conformal `tilde R_ww[h]`
only: it is not `R^chi_ww`, not physical `R_ww[gamma]`, not full Ricci, not
CCZ4 RHS, and not evolution wiring.

Checkpoint B applies one pre-4AD cleanup to the reduced determinant policy:
Stage 4Y now rejects `D <= 0` instead of accepting finite negative
determinants. The singular, gradient, Hessian, and Stage 4AC assembly paths
therefore all require finite positive `D = AC - B^2` before evaluating their
conformal `R_ww` formulas.

## Stage 4AF Internal Split-Vs-Direct Gate

Stage 4AF locks the internal away-axis identity

```text
tilde R_ww[h] + R^chi_ww = R_ww[gamma],  gamma = chi^{-1} h.
```

The direct side does not ask Stage 4G to repeat the conformal split. The test
forms the physical metric jet explicitly:

```text
gamma_i = h_i / chi - h chi_i / chi^2,

gamma_ij = h_ij / chi
         - (h_i chi_j + h_j chi_i + h chi_ij) / chi^2
         + 2 h chi_i chi_j / chi^3.
```

It then supplies `(gamma,gamma_i,gamma_ij)` to Stage 4G as the metric with
`chi=1`. The gate covers constant `chi`, flat conformal data with linear
`chi(x)` and linear `chi(z)`, and three distinct nonsymmetric varying-`chi`
jets with nonzero first derivatives of `A,B,C`, nonzero `B`, and nonzero
`W_x,W_z,W_xx,W_xz,W_zz`. A separate regression confirms that nonzero second
derivatives of `A,B,C` cancel from direct `R_ww`.

This is an internal identity gate against the project's Stage 4G engine, not
external validation and not a production physical-`R_ww` assembler. Stage
4AG still owns true parity/near-axis validation, Stage 4AH owns production
physical assembly, and RHS/evolution remain unimplemented. Stage 4AG now
passes its local gate below; Checkpoint D remains pending for joint review.

## Stage 4AG Two-Sided `h_xz` Parity Gate

Stage 4X's checked local `h_xz/x` value does not prove parity. Stage 4AG adds
the missing paired-data validation:

```text
h_xz(-x,z) = -h_xz(+x,z),
h_xz(0,z) = 0                         when axis data is supplied,
h_xz(+x,z)/(+x) = h_xz(-x,z)/(-x).
```

The validator requires finite one-to-one `+x/-x` partners and positive finite
validation tolerance. Smooth odd data `h_xz=a x+b x^3` passes at several
radii, including nonconstant even quotient `h_xz/x=a+b x^2`; adding even
`c x^2` contamination fails. The tolerance is not a clamp or regularization
parameter. This gate validates only the supplied `h_xz` stencil and does not
prove full smoothness, finite-axis regularity, diagonal matching, `W_x` or
`chi_x` parity, physical `R_ww`, RHS, or evolution. Checkpoint D / Claude
Audit D is required before Stage 4AH.

## Stage 4AH Local Away-Axis Physical Assembly

After the narrow Checkpoint D review, Stage 4AH assembles

```text
R_ww[gamma] = tilde R_ww[h] + R^chi_ww
```

through `CartoonAwayAxisPhysicalRww.hpp`. A single factory accepts one local
metric/conformal-factor jet and internally mints both the Stage 4AC conformal
input and Stage 4AE correction input; callers cannot provide independently
created packages from different points. The result exposes the conformal
part, conformal-factor part, and sum.

This is local away-axis assembly only. The Stage 4AG fixture does not establish
that actual grid data obeys `h_xz` parity. Grid `h_xx-h_ww` matching, `W_x`
parity, `chi_x` parity, finite-axis evaluation, RHS, and evolution remain
unimplemented.

## Stage 4AI Local Physical-`R_ww` Contract

Stage 4AI consumes only the non-forgeable Stage 4AH physical result and forms
the hidden `ww` contributions

```text
hidden_conformal_trace = 2 (1 / h_ww) R_ww[gamma],
hidden_physical_scalar_contribution =
    chi * hidden_conformal_trace.
```

The factor `2` is the hidden-direction multiplicity. The Stage 4AH result now
carries the same-point `chi` and conformal `h^ww = 1/h_ww`, preventing a
caller from pairing physical `R_ww[gamma]` with metric data from another
point. The contract does not accept conformal `tilde R_ww`, `R^chi_ww`, or a
loose raw `R_ww` value.

This remains local and away-axis only. It is not the full Ricci scalar, not a
full CCZ4 RHS, and not grid or evolution wiring. Actual-grid parity and
matching checks, `W_x` and `chi_x` regularity, and finite-axis evaluation
remain open. Checkpoint E / Claude Audit E is recorded as complete before
Stage 4AJ.

## Stage 4AJ Physical Hidden Lapse Hessian

Stage 4AJ implements the local physical hidden Hessian used later in the
curvature/lapse block:

```text
D_wD_w alpha =
    (rho_gamma / x^2) gamma^ab (partial_a rho_gamma)(partial_b alpha),

rho_gamma = x sqrt(W / chi),  gamma_IJ = chi^{-1} h_IJ.
```

Equivalently, the test checks

```text
D_wD_w alpha =
    (1 / (2 x^2)) gamma^ab
        partial_a(x^2 gamma_ww) partial_b alpha.
```

The guarded implementation is

```text
D_wD_w alpha =
    (W/D)(C p_alpha - q alpha_z)
  + (W/(2D)) [
      x^2(C ell_x p_alpha - q ell_x alpha_z - q ell_z p_alpha)
      + A ell_z alpha_z
    ],
```

where `q=B/x`, `p_W=W_x/x`, `p_chi=chi_x/x`,
`p_alpha=alpha_x/x`, `ell_x=p_W/W-p_chi/chi`, and
`ell_z=W_z/W-chi_z/chi`. The varying-`chi` flat oracle gives `11/80`, not the
conformal-metric value `3/20`, which guards against reusing the Stage 4AE
hidden Hessian structure unchanged.

This is still local and away-axis only. The source sign `-D_wD_w alpha` and
the full `A_ww` curvature/lapse block are Stage 4AK work. Stage 4AJ adds no
RHS, grid reads, finite-axis support, or evolution wiring.

## Stage 4AK Local Aww Curvature/Lapse Geometric Core

Stage 4AK implements only the local away-axis geometric core inside the future
hidden `A_ww` curvature/lapse source:

```text
C_ww = -D_wD_w alpha + alpha R_ww[gamma].
```

The factory mints the reviewed Stage 4AH physical `R_ww[gamma]` inputs and the
Stage 4AJ physical `D_wD_w alpha` inputs from one local
metric/conformal-factor/lapse jet. The result exposes `minus_dww_alpha`,
`alpha_rww`, and `curvature_lapse_core`.

This is not the full CCZ4 `A_ww` block. It deliberately omits the Z4 term
`2 alpha D_w Z_w`, the full 4D trace-free projection, the outer `chi`
prefactor, nonlinear A/K terms, RHS/grid/evolution wiring, and finite-axis
support.
