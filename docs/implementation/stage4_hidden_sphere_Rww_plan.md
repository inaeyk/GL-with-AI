# Stage 4 Hidden-Sphere Rww Plan

Status: roadmap and derivation integration only. This document does not add
C++ code, tests, finite-difference stencils, full Ricci, CCZ4 RHS terms, or
evolution wiring.

## Purpose

Stage 4W through Stage 4Y established the first guarded path into the
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

The Hessian block is

```text
G^Hess_ww =
    -(rho / x^2) H^ab (rho_ab - Gamma^c_ab[H] rho_c).
```

This block requires its own derivation lock before implementation because it
mixes second derivatives, reduced Christoffels, and singular-looking factors.

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
Stage 4AD must design a guard stack analogous to Stages 4P-4X before
implementation. The Stage 4Y conformal block does not make `R^chi_ww` safe.

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

## Future Stage Breakdown

| Stage | Ownership | Gate |
| --- | --- | --- |
| Stage 4Z | Checked `W_x / x` ingredient and `G^grad_ww` implementation | Consumes checked `p_W = W_x / x`, checked `q_xz`, and single-source metric inputs; no full `tilde R_ww` claim |
| Stage 4AA | Hessian block derivation lock | Derive `G^Hess_ww`, coefficient/sign convention, and hard-coded oracles before code |
| Stage 4AB | Hessian block implementation | Local checked Hessian sub-block only, with determinant and away-axis guards |
| Stage 4AC | Assemble conformal `tilde{R}_ww[h]` | Combine reviewed conformal sub-blocks only; still not physical `R_ww[gamma]` |
| Stage 4AD | `R^chi_ww` derivation lock and guard-stack design | Design guards for `D_w D_w chi`, full conformal Laplacian, and hidden/cartoon singular terms |
| Stage 4AE | Implement `R^chi_ww` | Local conformal-factor Ricci correction only |
| Stage 4AF | Hard split-vs-direct physical Ricci identity gate | Validate `tilde{R}_ww + R^chi_ww == R_ww[gamma]` against direct physical Ricci, including varying `chi` |
| Stage 4AG | True off-diagonal parity validation gate | Require a two-sided check `h_xz(-x,z) = -h_xz(x,z)`, a Taylor/coefficient check, or a grid-level near-axis policy with documented tolerance |
| Stage 4AH | Assemble physical `R_ww[gamma]` | Blocked until Stages 4AF and 4AG pass |
| Stage 4AI | Place `R_ww` into the local Ricci/RHS contract | Local contract only; no live RHS |
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
| False `h_xz` parity claim | Stages 4X, 4AG | Keep Stage 4X scoped to local `h_xz / x`; add true parity validation in Stage 4AG |
| Missing `W_x / x` guard | Stage 4Z | Add checked `p_W = W_x / x` before `G^grad_ww` |
| Hessian complexity | Stages 4AA-4AB | Derivation lock before implementation |
| `R^chi_ww` singular hidden terms | Stages 4AD-4AE | Design a dedicated guard stack for `D_w D_w chi` and full Laplacian terms |
| Hard identity gate not yet passed | Stage 4AF | Direct physical Ricci comparison with varying `chi` |
| `hat_Gamma^x` hidden contraction | Stages 4AL-4AM | Derive hidden contraction and hatted convention; use GL-growth/dispersion anchor |
| Sign-convention consistency between initial data and RHS | Sign gate before 4AK | Check against the Stage 3A `K_IJ` convention and CCZ4 curvature/lapse sign |
| Smoke-only hidden freeze enters physics path | Stage 4AQ | Remove or replace `scaffold_freeze_hidden` before real hidden-sector RHS |

