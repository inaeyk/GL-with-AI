# Stage 4 Hidden-Sphere Rww Plan

Status: integrated roadmap through the local Stage 4AE implementation. This
document does not claim physical Ricci assembly, finite-difference stencils,
CCZ4 RHS terms, or evolution wiring.

## Purpose

Stage 4W through Stage 4Z established the first guarded path into the
hidden-sphere Ricci sector. This plan records the protected conventions, the
completed narrow stages, and the remaining hard gates before any physical
`R_ww[gamma]` contribution is trusted downstream.

The central rule is:

```text
R_ww[gamma] = tilde{R}_ww[h] + R^chi_ww,
gamma_IJ = chi^{-1} h_IJ.
```

Physical CCZ4 uses `R_ww[gamma]`, not `tilde{R}_ww[h]` alone.

## Protected Conventions

- Physical spatial dimension: `d = 4`.
- Grid directions: `x,z`.
- Hidden directions: two equivalent `w` directions, with
  `N_hidden = 2`.
- Conformal metric bookkeeping:

```text
h_IJ =
[[h_xx, h_xz, 0,    0   ],
 [h_xz, h_zz, 0,    0   ],
 [0,    0,    h_ww, 0   ],
 [0,    0,    0,    h_ww]]
```

- Cartoon/spherical reconstruction:

```text
d tilde{s}^2 =
    h_xx dx^2
  + 2 h_xz dx dz
  + h_zz dz^2
  + x^2 h_ww dOmega_2^2.
```

- `h_ww` is a Cartesian-like hidden conformal metric component. It is not
  `g_theta theta`; the external `x^2` factor comes from the cartoon/spherical
  reconstruction.
- Full 4D trace:

```text
S = h^xx S_xx + 2 h^xz S_xz + h^zz S_zz + 2 h^ww S_ww.
```

- Trace-free projection denominator: `1/4`.

## Hidden-Sphere CCZ4 Contribution Map

The hidden sphere enters the CCZ4 source structure through:

- trace multiplicity, such as `+ 2 h^ww (...)_ww`;
- cartoon derivative terms such as `beta^x / x`, `partial_x f / x`, and
  `D_w D_w f`;
- true hidden curvature, especially `R_ww`.

The curvature/lapse block for the `A_IJ` equation is schematically

```text
S_IJ = -D_I D_J alpha + alpha (R_IJ + D_I Z_J + D_J Z_I),
```

with source

```text
chi S_IJ^TF = chi (S_IJ - (1/4) h_IJ S).
```

Therefore `R_ww` contributes directly to the `A_ww` equation and indirectly to
`A_xx`, `A_xz`, and `A_zz` through the full 4D trace-free projection.

## Conformal And Physical Ricci Split

The split is a hard convention boundary:

```text
R_ww[gamma] = tilde{R}_ww[h] + R^chi_ww,
gamma_IJ = chi^{-1} h_IJ.
```

`tilde{R}_ww[h]` is only the conformal Ricci component. `R^chi_ww` is a
genuine, nonzero companion. Physical CCZ4 uses `R_ww[gamma]`, so any downstream
RHS or evolution path is blocked until both pieces and the split-vs-direct
identity gate below are implemented and validated.

## Conformal Hidden Ricci Decomposition

Let

```text
A = h_xx,  B = h_xz,  C = h_zz,  W = h_ww,  D = A C - B^2.
```

Then

```text
H^xx = C / D,
H^xz = -B / D,
H^zz = A / D,
rho = x sqrt(W).
```

The conformal hidden Ricci target is

```text
tilde{R}_ww[h] =
    (1 - rho D^a D_a rho - D^a rho D_a rho) / x^2.
```

For staged implementation, decompose it as

```text
tilde{R}_ww =
    G^sing_ww
  + G^grad_ww
  + G^Hess_ww.
```

Stage 4Y completed the singular/regularity-sensitive gradient block:

```text
G^sing_ww = (C / D) Delta_xw - q_xz^2 / D,
Delta_xw = (A - W) / x^2,
q_xz = B / x.
```

After review, Stage 4Y single-sources `A`, `B`, `C`, `W`, the determinant,
checked `Delta_xw`, and checked `q_xz` from one local metric point through a
non-forgeable input package.

The next gradient block is

```text
G^grad_ww =
    -(C / D) (W_x / x)
  +  (B / D) (W_z / x)
  -  (C W_x^2 - 2 B W_x W_z + A W_z^2) / (4 W D).
```

Using

```text
p_W = W_x / x,
q_xz = B / x,
```

write the guarded form as

```text
G^grad_ww =
    -(C / D) p_W
  +  (q_xz W_z) / D
  -  (C W_x^2 - 2 B W_x W_z + A W_z^2) / (4 W D).
```

Stage 4Z completed this first-derivative gradient block with a checked local
`p_W = W_x / x` ingredient and a single-source input package minted from one
local metric point. As with Stage 4X, the checked quotient is an away-axis
local token only; it does not prove global parity, finite-axis regularity, or
the analytic statement `W_x = O(x)` for grid data.

The Hessian block is

```text
G^Hess_ww =
    -(rho / x^2) H^ab (rho_ab - Gamma^c_ab[H] rho_c).
```

Stage 4AA locks this block for later implementation. With

```text
H^xx = C / D,    H^xz = -B / D,    H^zz = A / D,
```

the source-facing Hessian form is

```text
G^Hess_ww =
    -(sqrt(W) / x) [
        (C / D) H_xx
      - (2 B / D) H_xz
      + (A / D) H_zz
    ],
```

where

```text
H_ab =
    rho_ab
  - Gamma^x_ab rho_x
  - Gamma^z_ab rho_z.
```

The needed radius derivatives are

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

The reduced-base Christoffels are

```text
Gamma^x_xx = [C A_x - B(2 B_x - A_z)] / (2 D),
Gamma^z_xx = [-B A_x + A(2 B_x - A_z)] / (2 D),

Gamma^x_xz = [C A_z - B C_x] / (2 D),
Gamma^z_xz = [-B A_z + A C_x] / (2 D),

Gamma^x_zz = [C(2 B_z - C_x) - B C_z] / (2 D),
Gamma^z_zz = [-B(2 B_z - C_x) + A C_z] / (2 D).
```

Stage 4AA locks the following Hessian oracles for Stage 4AB:

- Flat: `A = C = W = 1`, `B = 0`, all derivatives zero gives
  `G^Hess_ww = 0`.
- Constant cone: `A = C = 1`, `B = 0`, `W = 4`, all derivatives zero gives
  `G^Hess_ww = 0`.
- Nonconstant hidden metric: for flat base and `W = (1 + x)^2` at `x = 1`,
  `W = 4`, `W_x = 4`, `W_xx = 2`, and
  `W_z = W_xz = W_zz = 0`. Then `rho = x(1 + x)`, `rho_xx = 2`, and
  `G^Hess_ww = -(rho / x^2) rho_xx = -4`. Combined with the reviewed
  previous sub-blocks, `G^sing_ww = -3`, `G^grad_ww = -5`, and
  `G^Hess_ww = -4`, so `tilde{R}_ww = -12`.
- Distinct nonsymmetric audit sample: `x = 2`, `A = 7`, `B = 2`, `C = 5`,
  `W = 3`, `A_x = 11`, `A_z = 13`, `B_x = 17`, `B_z = 19`, `C_x = 23`,
  `C_z = 29`, `W_x = 6`, `W_z = 4`, `W_xx = 31`, `W_xz = 37`, and
  `W_zz = 41`, with `D = 31`. This has nonzero entries in all first and
  second derivative slots needed by the block. Claude Audit A verifies
  `G^Hess_ww = -8558 / 2883`, approximately `-2.9684356573014221`.
  For the same sample, the full conformal sum is
  `G^sing_ww + G^grad_ww + G^Hess_ww = -3576 / 961`, approximately
  `-3.7211238293`, matching the independent Stage 4G conformal Ricci engine
  to machine precision with residual about `4.44e-16`.

The distinct nonsymmetric oracle is required for Stage 4AB. It exercises the
off-diagonal Christoffels, `rho_xz`, `W_z`-dependent terms, and the
`(-2 B / D)` contraction. The flat, constant-cone, and `W = (1 + x)^2`
oracles are necessary but not sufficient by themselves.

Scope: Stage 4AA is the Hessian block only. It is not full
`tilde{R}_ww[h]`, not `R^chi_ww`, not physical `R_ww[gamma]`, not full Ricci,
not CCZ4 RHS, and not evolution wiring. It is away-axis only and does not
prove global parity or finite-axis regularity.

Checkpoint A is complete for the nonsymmetric Hessian oracle. Stage 4AB may
now implement the Hessian block, but only if the Stage 4AB test includes the
verified nonsymmetric oracle above alongside the simpler flat, cone, and
nonconstant-`W` cases.

Stage 4AB implements this Hessian sub-block in
`code/BlackStringToy/CartoonConformalRwwHessianBlock.hpp`, with fixture
coverage in
`code/BlackStringToy/tests/Stage4ABConformalRwwHessianBlockTest.cpp`.
The implementation consumes a non-forgeable single-source input package minted
from one local point: `x`, `A`, `B`, `C`, `W`, the reduced-base first
derivatives, and the `W` first/second derivatives. It rejects invalid axis,
nonpositive `W`, nonfinite inputs, and nonpositive reduced determinant. It
does not accept loose raw determinant values or precomputed Christoffels.
Stage 4AB still does not assemble full `tilde{R}_ww[h]`; Stage 4AC owns that
next step.

Stage 4AC assembles the local away-axis conformal component
`tilde{R}_ww[h] = G^sing_ww + G^grad_ww + G^Hess_ww` in
`code/BlackStringToy/CartoonConformalRww.hpp`, with fixture coverage in
`code/BlackStringToy/tests/Stage4ACConformalRwwAssemblyTest.cpp`. The input
package is non-forgeable and single-sources the Stage 4Y, 4Z, and 4AB input
packages from one local metric/derivative point. The fixture checks the flat,
constant-cone, nonconstant-`W`, and Claude-verified nonsymmetric oracles,
including direct Stage 4G comparisons for the nonconstant and nonsymmetric
`chi=1` cases. Stage 4AC is conformal `tilde{R}_ww[h]` only: it does not
implement `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid
reads, or evolution wiring.

Checkpoint B / Claude Audit B reviewed the conformal assembly boundary and
found one cleanup before Stage 4AD: the reduced determinant policy must be
consistent across conformal `R_ww` pieces. The Stage 4Y singular block now
matches Stage 4Z, Stage 4AB, and Stage 4AC by requiring finite positive
`D = AC - B^2`; negative determinants are rejected before formula evaluation.

## Conformal-Factor Ricci Correction

For `gamma_IJ = chi^{-1} h_IJ` in `d = 4`,

```text
R^chi_IJ =
    (1 / chi) D_I D_J chi
  + (1 / (2 chi)) h_IJ D^K D_K chi
  - (1 / (2 chi^2)) D_I chi D_J chi
  - (1 / chi^2) h_IJ D^K chi D_K chi.
```

For the hidden component, `D_w chi = 0`, so

```text
R^chi_ww =
    (1 / chi) D_w D_w chi
  + (W / (2 chi)) D^K D_K chi
  - (W / chi^2) D^K chi D_K chi.
```

Warning: `R^chi_ww` carries its own `1/x` and `1/x^2`-type
hidden/cartoon terms through `D_w D_w chi` and the full conformal Laplacian.
Stage 4AD designed, and Stage 4AE implements, a guard stack analogous to
Stages 4P-4X. The conformal `tilde R_ww` implementation alone does not make
`R^chi_ww` safe.

Stage 4AD locks the local away-axis derivation for this conformal-factor
correction. With

```text
A = h_xx,  B = h_xz,  C = h_zz,  W = h_ww,  D = A C - B^2,
H^xx = C / D,  H^xz = -B / D,  H^zz = A / D,
rho = x sqrt(W),
```

the hidden scalar Hessian is

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

where the `2 / W` term is the hidden multiplicity `N_hidden = 2` and must not
be dropped. The reduced-base scalar Hessian pieces are

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

The Stage 4AE guard stack mints a single-source input package from one
local point and reuses the established conformal `R_ww` local policies:

- finite positive `x` through the away-axis-only policy;
- finite positive `chi`, because `R^chi_ww` divides by `chi` and `chi^2`;
- finite positive `W` and finite positive reduced determinant `D`;
- existing checked `q_xz = B / x` for the off-diagonal hidden Hessian terms;
- existing checked `p_W = W_x / x` when rewriting `rho_x` terms in guarded
  local form;
- a new checked local `p_chi = chi_x / x` ingredient for the leading
  `H^xx rho_x chi_x / x` part of `D_w D_w chi`;
- finite raw `chi_z`, `chi_xx`, `chi_xz`, and `chi_zz` values for the
  reduced-base Hessian pieces;
- no clamp, `eps_cut`, or regularized-axis substitution.

The checked `p_chi` package is a local away-axis quotient only. Like
Stage 4X and Stage 4Z, it does not prove global parity, finite-axis
regularity, or the analytic statement `chi_x = O(x)` on a numerical grid.
True grid-level parity or near-axis regularity validation remains separate
future work.

Stage 4AD records the following Stage 4AE oracles:

- Constant `chi`: all `chi` derivatives vanish, so `R^chi_ww = 0`.
- Flat conformal metric with `A = C = W = 1`, `B = 0`, and
  `chi = 1 + a x`:

```text
R^chi_ww = 2 a / (x chi) - a^2 / chi^2.
```

At `x = 2`, `a = 0.1`, `chi = 1.2`,

```text
R^chi_ww = 11 / 144 = 0.076388888888...
```

- Flat conformal metric with `A = C = W = 1`, `B = 0`, and
  `chi = 1 + b z`: hand derivation gives `D_w D_w chi = 0`,
  `D^K D_K chi = 0`, and `D^K chi D_K chi = b^2`, hence
  `R^chi_ww = -b^2 / chi^2`. For example, at `z = 3`, `b = 0.2`,
  `chi = 1.6`, this gives `R^chi_ww = -1 / 64 = -0.015625`; the Stage 4AE
  fixture now locks this value.
- The Stage 4AE nonsymmetric sample gives
  `D_wD_w chi = 131/62`, `D^KD_K chi = 10430/2883`,
  `D^K chi D_K chi = 49/31`, and
  `R^chi_ww = 63341/48050`. The fixture also checks this value against
  `R_ww[gamma] - tilde R_ww[h]` from the Stage 4G and Stage 4AC helpers.

Stage 4AD remains derivation and guard-stack design only. It does not
implement `R^chi_ww`, physical `R_ww[gamma]`, the split-vs-direct identity
gate, RHS terms, grid reads, or evolution wiring.

Stage 4AE implements the local away-axis conformal-factor correction in
`code/BlackStringToy/CartoonConformalFactorRww.hpp`. Its factory
single-sources the local metric, conformal factor, derivatives, positive
determinant, and checked `q_xz`, `p_W`, and `p_chi` ingredients. The guarded
hidden-Hessian form is

```text
D_wD_w chi =
    (C / D) [W p_chi + (x^2 / 2) p_W p_chi]
  - (q_xz / D) [
        W chi_z
      + (x^2 / 2) p_W chi_z
      + (x^2 / 2) W_z p_chi
    ]
  + (A / D) (W_z chi_z / 2).
```

The implementation retains the hidden multiplicity `(2/W)D_wD_w chi` in the
full conformal Laplacian. It is `R^chi_ww` only, not physical
`R_ww[gamma]`, not the Stage 4AF hard identity gate, not RHS, and not
evolution wiring. Checkpoint C / Claude Audit C is required before Stage 4AF.

## Hard Physical Ricci Identity Gate

Before physical `R_ww[gamma]` is trusted downstream, the project must test

```text
tilde{R}_ww[h] + R^chi_ww == R_ww[gamma]
```

against a direct physical-metric Ricci computation with
`gamma_IJ = chi^{-1} h_IJ`. The gate must include at least one varying-`chi`
oracle.

Physical `R_ww[gamma]` assembly, local RHS use, and evolution use are blocked
until this split-vs-direct identity gate passes.

## Sign Convention Gate

The internal sign convention condition is

```text
partial_t gamma_IJ = -2 alpha K_IJ + L_beta gamma_IJ.
```

The sign of the curvature/lapse block

```text
-D_I D_J alpha + alpha R_IJ
```

must be consistent with the `K_IJ` convention used in the project initial-data
derivation. The current convention anchor is
`docs/physics_notes/stage3A_black_string_initial_data.tex`; if that note is
reworked, the corresponding sign gate must be updated before RHS wiring.

## Completed Stage Summary

- Stage 0: environment and smoke baseline.
- Stage 1: public GRChombo source/capability map.
- Stage 1.5: `DIM=2` AH preflight and target-dimension compile checks.
- Stage 2: editable `BlackStringToy` scaffold and 2+1D CCZ4 cartoon planning.
- Stage 3: symbolic derivations, convention notes, and validation layers.
- Stage 4A: conformal-cartoon algebra helper.
- Stage 4B: CCZ4 layout baseline.
- Stage 4C: hidden `h_ww` / `A_ww` variables.
- Stage 4D: smoke-only hidden scaffold.
- Stage 4E: grid-to-helper handoff.
- Stage 4F: cartoon Ricci interface.
- Stage 4G: away-axis metric-derivative cartoon Ricci helper.
- Stage 4H: Ricci/RHS compatibility plan.
- Stage 4I: typed Ricci bridge.
- Stage 4J: Ricci-to-RHS contract.
- Stage 4K: local RHS source-block skeleton.
- Stage 4L: trace-free Ricci projection.
- Stage 4M: away-axis policy.
- Stage 4N: singular-combination helpers.
- Stage 4O: axis-regime semantics.
- Stage 4P: geometry primitives.
- Stage 4Q: diagonal matching guard.
- Stage 4R: regularity-guarded source package.
- Stage 4S: carry checked package into the source-block layer.
- Stage 4T: first guarded consumer.
- Stage 4U: source-formula authoring gate.
- Stage 4V: derivation gap stop.
- Stage 4W: hidden-sphere Ricci derivation lock.
- Stage 4X: checked local `h_xz / x` ingredient, not a parity proof.
- Stage 4Y: first guarded conformal `tilde{R}_ww` singular sub-block,
  single-sourced after review.
- Stage 4Z: checked local `W_x / x` ingredient and first-derivative conformal
  gradient sub-block, single-sourced from one local metric point.
- Stage 4AA: Hessian conformal sub-block derivation lock, oracle list, and
  Claude Audit A verified nonsymmetric oracle before implementation.
- Stage 4AB: local away-axis conformal Hessian sub-block implementation,
  checked by the flat, cone, nonconstant-`W`, and verified nonsymmetric
  oracles.
- Stage 4AC: local away-axis conformal `tilde{R}_ww[h]` assembly from the
  reviewed Stage 4Y, 4Z, and 4AB sub-blocks.
- Stage 4AD: `R^chi_ww` derivation lock and guard-stack design for Stage 4AE,
  including the required checked `chi_x / x` ingredient and oracle set.
- Stage 4AE: local away-axis `R^chi_ww` implementation with checked
  `chi_x/x`, single-source inputs, hidden multiplicity, locked simple and
  nonsymmetric oracles, and a test-only Stage 4G difference comparison.
- Stage 4AF: internal split-vs-direct identity gate using an explicitly
  differentiated physical metric jet, three baseline cases, and three
  nonsymmetric varying-`chi` cases with nonzero `W` second derivatives.
- Stage 4AG: paired two-sided `h_xz` odd-parity validation with optional axis
  data, one-to-one radius matching, and even `h_xz/x` checks.
- Stage 4AH: local away-axis physical `R_ww[gamma]` assembly from Stage 4AC
  plus Stage 4AE through one single-source metric/conformal-factor jet.
- Stage 4AI: typed local hidden-`ww` Ricci/RHS contribution contract consuming
  only the Stage 4AH physical result.

## Future Stage Breakdown

| Stage | Ownership | Gate |
| --- | --- | --- |
| Stage 4Z | Checked `W_x / x` ingredient and `G^grad_ww` implementation | Complete as a local away-axis sub-block; consumes checked `p_W = W_x / x`, checked `q_xz`, and single-source metric inputs; no full `tilde R_ww` claim |
| Stage 4AA | Hessian block derivation lock | Complete as documentation: `G^Hess_ww`, coefficient/sign convention, Christoffels, flat/cone/nonconstant oracles, and Claude Audit A's verified nonsymmetric oracle are recorded |
| Stage 4AB | Hessian block implementation | Complete as a local checked Hessian sub-block with determinant and away-axis guards; includes the verified nonsymmetric oracle in its test |
| Stage 4AC | Assemble conformal `tilde{R}_ww[h]` | Complete as a local away-axis conformal-only assembly of reviewed Stage 4Y/4Z/4AB sub-blocks; still not physical `R_ww[gamma]` |
| Stage 4AD | `R^chi_ww` derivation lock and guard-stack design | Complete as documentation: locks `D_wD_w chi`, the full Laplacian with hidden multiplicity, the gradient norm, the need for checked `chi_x/x`, and Stage 4AE oracles |
| Stage 4AE | Implement `R^chi_ww` | Complete as a local away-axis conformal-factor correction with checked `p_chi`, single-source inputs, hidden multiplicity, simple/nonsymmetric oracles, and one test-only Stage 4G difference comparison |
| Stage 4AF | Hard split-vs-direct physical Ricci identity gate | Complete as an internal test-only gate: explicitly construct the `gamma=h/chi` first/second derivative jet and compare the split against Stage 4G at constant `chi`, linear `chi`, and three nonsymmetric varying-`chi` points |
| Stage 4AG | True off-diagonal parity validation gate | Complete as a local paired-data gate: checks odd `h_xz`, optional axis zero, one-to-one `+x/-x` partners, and even `h_xz/x` without demanding a constant quotient |
| Stage 4AH | Assemble physical `R_ww[gamma]` | Complete only as local away-axis assembly; one factory mints both 4AC and 4AE packages, while finite-axis and actual-grid validation remain false |
| Stage 4AI | Place `R_ww` into the local Ricci/RHS contract | Complete as a typed local hidden contribution contract; accepts only the Stage 4AH physical result and adds no live RHS |
| Stage 4AJ | Hidden lapse Hessian `D_w D_w alpha` | Guard hidden/cartoon lapse terms before curvature/lapse source use |
| Stage 4AK | Local `A_ww` curvature/lapse block | Local source block only, with sign convention gate satisfied |
| Stage 4AL | `hat_Gamma^x` hidden-contraction derivation lock and GL-growth anchor | Derive hidden contribution to `tilde{Gamma}^x` and the hatted convention; define a nontrivial GL-growth/dispersion anchor |
| Stage 4AM | `hat_Gamma^x` local implementation/contract tests | Local contract tests for the hidden contraction and hatted CCZ4 convention |
| Stage 4AN | Local trace-free curvature/lapse block for all components | Include hidden trace feedback into `A_xx`, `A_xz`, `A_zz`, and `A_ww` |
| Stage 4AO | Controlled local RHS source-block integration | Integrate reviewed local blocks without evolution wiring |
| Stage 4AP | Evolution-level wiring behind explicit parameter | Default-off wiring only after local and identity gates pass |
| Stage 4AQ | Remove or replace smoke-only hidden freeze | Ensure `scaffold_freeze_hidden` cannot mask real hidden-sector RHS |
| Stage 4AR | Stage 4 exit review | Review all gates before any Stage 5/Pau diagnostic reproduction |

Stage 4AG owns true `h_xz` parity validation. Stage 4X only provides a
checked local `h_xz / x` ingredient; it does not prove global parity or the
analytic statement `h_xz = O(x)`. Without Stage 4AG, the code may have correct
away-axis local formulas while still lacking a validated global
axis-regularity assumption for `h_xz`.

Stage 4AF now passes only the internal hard identity

```text
tilde R_ww[h] + R^chi_ww = R_ww[gamma].
```

The gate constructs `gamma = h / chi` and its first and second derivatives
explicitly, feeds that already-physical jet to Stage 4G with `chi=1`, and
checks constant `chi`, flat linear-`x` and linear-`z` `chi`, and three
nonsymmetric varying-`chi` jets. All nonsymmetric cases have nonzero
`W_x,W_z,W_xx,W_xz,W_zz`; one also verifies that nonzero second derivatives
of `A,B,C` do not affect direct `R_ww`. Stage 4G is an internal project oracle,
not external validation. Stage 4AG is implemented below, and Checkpoint D
remains pending until both gates have been reviewed. Physical
`R_ww[gamma]` production assembly remains Stage 4AH.

Stage 4AG now supplies the concrete parity owner that Stage 4X lacked. The
local validator consumes positive- and negative-coordinate samples, requires
one-to-one matching radii, checks `h_xz(+x)+h_xz(-x)` and the evenness of
`h_xz/x`, and checks `h_xz(0,z)=0` when an axis value is supplied. Smooth odd
data `a x + b x^3` passes at multiple radii even though
`h_xz/x = a + b x^2` is not constant; an `x^2` contamination fails. These are
validation tolerances only, with no clamping or epsilon division. The gate
does not establish full smoothness, finite-axis regularization, or parity of
other fields. Checkpoint D is recorded below as clearing only local Stage 4AH.

Checkpoint D cleared only the narrow local Stage 4AH assembly. Stage 4AH now
computes

```text
R_ww[gamma] = tilde R_ww[h] + R^chi_ww
```

from a non-forgeable input package whose factory mints the Stage 4AC and 4AE
packages from the same local metric/conformal-factor jet. The result exposes
the two parts and their sum. This does not mean actual grid data passed the
Stage 4AG parity policy, and it does not validate grid `h_xx-h_ww` matching,
`W_x` parity, `chi_x` parity, or finite-axis behavior. No RHS, grid reads, or
evolution wiring are present.

Stage 4AI now places that reviewed physical lower/lower component into a
typed local contribution contract:

```text
hidden_conformal_trace = 2 h^ww R_ww[gamma]
                       = 2 (1 / h_ww) R_ww[gamma],

hidden_physical_scalar_contribution =
    chi * hidden_conformal_trace.
```

`CartoonAwayAxisPhysicalRww.hpp` carries the same-point `chi` and
`1 / h_ww` in its non-forgeable result. The Stage 4AI boundary accepts only
that result type, not a loose `double`, conformal `tilde R_ww`, or
`R^chi_ww`. This is the hidden `ww` contribution only, not the full Ricci
scalar, full CCZ4 RHS, grid data, finite-axis support, or evolution wiring.
Actual-grid parity/matching and `W_x`/`chi_x` regularity remain unvalidated.
Checkpoint E / Claude Audit E is required after Stage 4AI and before Stage
4AJ.

Stage 4AL owns the high-risk `hat_Gamma^x` derivation lock. It must cover

```text
tilde{Gamma}^x =
    h^ab tilde{Gamma}^x_ab
  + 2 h^ww tilde{Gamma}^x_ww,
```

and the hatted CCZ4/GRChombo convention used in this project. Flat oracles are
mostly vacuous for `hat_Gamma^x`, so the nontrivial pre-evolution anchor should
be a semi-analytic GL linear-growth/growth-rate check or documented
GL-dispersion anchor, not Pau's code. This is required before serious
evolution claims.

## Risk Register

| Risk | Current owner | Required mitigation |
| --- | --- | --- |
| Conformal vs physical Ricci confusion | Stages 4W, 4AC-4AF | Keep `tilde{R}_ww[h]`, `R^chi_ww`, and `R_ww[gamma]` separate until the identity gate passes |
| Raw/checked mismatch | Stage 4Y | Single-source input package for determinant data and checked singular ingredients |
| Inconsistent reduced determinant policy | Checkpoint B / Stage 4Y | Stage 4Y now rejects finite negative `D`; all conformal `R_ww` pieces require finite positive reduced determinant |
| False `h_xz` parity claim | Stages 4X, 4AG | Stage 4X remains a local quotient; Stage 4AG now requires paired odd data and optional explicit axis-zero validation, retained as a regression gate |
| Bypassing checked `W_x / x` | Stage 4Z and later gradient consumers | Stage 4Z provides checked `p_W = W_x / x`; later formulas must consume the checked package rather than loose raw quotient values |
| Hessian complexity | Stages 4AA-4AB | Stage 4AA locks the formula, Christoffels, and verified nonsymmetric oracle; Stage 4AB may proceed only with that oracle in its test |
| `R^chi_ww` singular hidden terms | Stages 4AD-4AE | Stage 4AE implements the dedicated checked `chi_x/x` guard stack and retains hidden multiplicity; Checkpoint C audits it before the Stage 4AF identity gate |
| Hard identity gate regression | Stage 4AF | Keep the six-point split-vs-direct fixture, explicit physical-jet product rules, nonzero `W` second derivatives, and base-second-derivative cancellation check |
| Wrong Ricci object enters the hidden RHS contract | Stage 4AI | Accept only the Stage 4AH physical `R_ww[gamma]` result; keep conformal and conformal-factor pieces outside the contract boundary |
| `hat_Gamma^x` hidden contraction | Stages 4AL-4AM | Derive hidden contraction and hatted convention; use GL-growth/dispersion anchor |
| Sign-convention consistency between initial data and RHS | Sign gate before 4AK | Check against the Stage 3A `K_IJ` convention and CCZ4 curvature/lapse sign |
| Smoke-only hidden freeze enters physics path | Stage 4AQ | Remove or replace `scaffold_freeze_hidden` before real hidden-sector RHS |
