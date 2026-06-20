# Stage 4AM Hat Gamma X Derivation Lock

Status: derivation and GRChombo convention map only. This note adds no C++
code, no tests, no RHS wiring, and no evolution path.

## Scope

Stage 4AM locks the local away-axis convention and formula contract for the
future `hat_Gamma^x` implementation:

```text
hat_Gamma^x = tilde_Gamma^x + 2 Z^x.
```

In the GRChombo-facing code inspected below, the evolved variable named
`Gamma` is the hatted connection in CCZ4 mode. GRChombo reconstructs an
encoded upper Z quantity as

```text
Z_over_chi[i] = 0.5 * (vars.Gamma[i] - chris.contracted[i]),
Z[i] = chi * Z_over_chi[i].
```

Therefore the project shorthand `hat_Gamma^i = tilde_Gamma^i + 2 Z^i`
matches GRChombo only if `Z^i` in that shorthand means the encoded
`Z_over_chi[i]`, not the local variable named `Z[i]`. Equivalently, using the
local variable named `Z[i]`, GRChombo stores

```text
vars.Gamma[i] = chris.contracted[i] + 2 Z[i] / chi.
```

Future Stage 4AN code must make this naming explicit at its type boundary.

## GRChombo Convention Inspection

Read-only files and symbols inspected:

- `external/GRChombo/Source/CCZ4/CCZ4UserVariables.hpp`: grid variables
  `c_Gamma1`, `c_Gamma2`, `c_Gamma3`, with names `"Gamma1"`, `"Gamma2"`,
  `"Gamma3"`.
- `external/GRChombo/Source/CCZ4/BSSNVars.hpp`: `BSSNVars::VarsNoGauge`
  contains `Tensor<1, data_t> Gamma`, documented as conformal connection
  functions and mapped to `c_Gamma1...c_Gamma3`.
- `external/GRChombo/Source/CCZ4/CCZ4Vars.hpp`: CCZ4 variables inherit the
  BSSN `Gamma` vector and add `Theta`.
- `external/GRChombo/Source/utils/TensorAlgebra.hpp`:
  `compute_christoffel` builds `chris.contracted[i] =
  h_UU[j][k] chris.ULL[i][j][k]`; `compute_inverse_sym` divides by the
  conformal metric determinant; `make_trace_free` uses `GR_SPACEDIM`.
- `external/GRChombo/Source/CCZ4/GammaCalculator.hpp`:
  `GammaCalculator::compute` stores `chris.contracted` into
  `c_Gamma1...c_Gamma3`, so zero-Z initial data has
  `Gamma^i = tilde_Gamma^i`.
- `external/GRChombo/Source/CCZ4/CCZ4RHS.impl.hpp`:
  `CCZ4RHS::rhs_equation` reconstructs `Z_over_chi`, forms `Z`, computes
  `compute_ricci_Z(..., Z_over_chi)`, and evolves `rhs.Gamma[i]`.
- `external/GRChombo/Source/CCZ4/CCZ4Geometry.hpp`:
  `compute_ricci_Z` explicitly says its `ricci_hat` uses `vars.Gamma` where
  `tilde{Gamma}` would otherwise appear, and `compute_ricci_Z_general`
  recovers `Z_over_chi = 0.5 * (vars.Gamma - chris.contracted)`.

Convention found:

- Evolved symbol: `vars.Gamma[i]`, grid slots `Gamma1..Gamma3`.
- Meaning in CCZ4 mode: hatted conformal connection.
- Pure conformal contracted connection: `chris.contracted[i]`.
- Sign: plus convention,
  `vars.Gamma[i] = chris.contracted[i] + 2 Z_over_chi[i]`.
- Z storage: no independent evolved Z variable. The RHS reconstructs upper
  vector-like `Tensor<1>` objects `Z_over_chi` and `Z`; no lower-index Z is
  stored at this boundary.
- Determinant: GRChombo's inverse helper divides by the conformal metric
  determinant. The RHS does not enforce `det h = 1` locally; Stage 4's reduced
  identity below may use the project conformal determinant condition only when
  the future input contract certifies it.

## Hidden Contraction

Use

```text
A = h_xx,   B = h_xz,   C = h_zz,   W = h_ww,
D = A C - B^2,

H^xx = C / D,   H^xz = -B / D,   H^zz = A / D.
```

The general away-axis full Christoffel contraction for the conformal
connection is

```text
tilde_Gamma^x =
    H^xx Gamma^x_xx
  + 2 H^xz Gamma^x_xz
  + H^zz Gamma^x_zz
  + 2 W^{-1} Gamma^x_ww.
```

The final factor of two is the hidden multiplicity. It is independent of the
factor of two in `hat_Gamma^x = tilde_Gamma^x + 2 Z^x`.

At the cartoon plane, the hidden derivatives are

```text
partial_w h_xw = (A - W) / x,
partial_w h_zw = B / x.
```

Therefore

```text
Gamma^x_ww =
    H^xx [(A - W) / x - W_x / 2]
  + H^xz [B / x - W_z / 2].
```

The reduced-base Christoffels used in the visible part are the Stage 4AA
Christoffels:

```text
Gamma^x_xx = [C A_x - B(2 B_x - A_z)] / (2 D),
Gamma^x_xz = [C A_z - B C_x] / (2 D),
Gamma^x_zz = [C(2 B_z - C_x) - B C_z] / (2 D).
```

Combining these gives the source-facing general formula:

```text
tilde_Gamma^x =
    (C / D) Gamma^x_xx
  - (2 B / D) Gamma^x_xz
  + (A / D) Gamma^x_zz
  + (2 / W) {
        (C / D) [(A - W) / x - W_x / 2]
      - (B / D) [B / x - W_z / 2]
    }.
```

This is the formula Stage 4AN must implement first. It uses checked
regularity-sensitive ingredients from the existing stack:

- `(A - W) / x^2` through the Stage 4U/4R checked path when near-axis
  matching sensitivity matters;
- `B / x` through Stage 4X;
- `W_x / x` through Stage 4Z when the determinant-reduced identity or finite
  axis checks need it.

## Determinant-Reduced Identity

Under the project conformal determinant condition

```text
D W^2 = 1,
```

the contraction can also be written

```text
tilde_Gamma^x =
    -partial_x H^xx
    -partial_z H^xz
    + (2 / x)(W^{-1} - H^xx).
```

The companion identity is

```text
tilde_Gamma^z =
    -partial_x H^xz
    -partial_z H^zz
    - (2 / x) H^xz.
```

These reduced identities are not general formulas. They rely on `D W^2 = 1`
and must not replace the full Christoffel contraction unless a future input
package certifies the determinant condition. Stage 4AN should use them as a
cross-check oracle, not as the only implementation path.

## Regularity And Parity

Finite odd behavior of `tilde_Gamma^x` near the cartoon axis relies on

```text
A - W = O(x^2),
B = O(x),
W_x = O(x).
```

For the hatted GRChombo-facing variable, regularity also requires the encoded
upper Z contribution to satisfy

```text
Z_over_chi^x = O(x),
```

or equivalently `Z^x = O(x)` if the project shorthand names
`Z_over_chi^x` as `Z^x`. This statement assumes finite positive `chi`.

These are not grid-data validations. Stage 4AG is only a synthetic paired
`h_xz` validator primitive. Stage 4AP owns actual grid/ghost-cell validation
for `h_xz=O(x)`, `A-W=O(x^2)`, `W_x=O(x)`, and `chi_x=O(x)`. Stage 4AQ owns
finite-axis evaluation and analytic regularized limits.

## Stage 4AN Required Oracles

All oracles below set the encoded Z contribution to zero, so
`hat_Gamma^x = tilde_Gamma^x`.

1. Flat:

```text
A = C = W = 1,  B = 0, all derivatives zero
=> tilde_Gamma^x = 0.
```

2. Constant cone:

```text
A = C = 1,  B = 0,  W = 4,  x = 2, all derivatives zero.

Gamma^x_ww = (A - W) / x = -3/2,
2 W^{-1} Gamma^x_ww = -3/4,
=> tilde_Gamma^x = -3/4.
```

This case does not satisfy `D W^2 = 1`, so it is a full-contraction oracle
only.

3. Determinant-one off-diagonal case:

```text
A = 1,  B = 1,  C = 2,  W = 1,  x = 2, all derivatives zero.
D = 1,  D W^2 = 1,

Gamma^x_ww = -1/2,
2 W^{-1} Gamma^x_ww = -1,
=> tilde_Gamma^x = -1.
```

The determinant-reduced identity gives the same result:

```text
(2 / x)(W^{-1} - H^xx) = (2 / 2)(1 - 2) = -1.
```

4. Distinct derivative case:

```text
x = 2,
A = 7, B = 2, C = 5, W = 3,
A_x = 3, A_z = 4,
B_x = 5, B_z = 6,
C_x = 7, C_z = 8,
W_x = 6, W_z = 4.
```

Then

```text
D = 31,
base contraction =
    H^xx Gamma^x_xx
  + 2 H^xz Gamma^x_xz
  + H^zz Gamma^x_zz
  = 27 / 961,

hidden contraction =
    2 W^{-1} Gamma^x_ww
  = -2 / 31,

tilde_Gamma^x = -35 / 961.
```

These four values are required Stage 4AN test oracles.

## GRChombo Gamma RHS Map

`CCZ4RHS::rhs_equation` stores

```text
rhs.Gamma[i] = advec.Gamma[i] + Gammadot[i].
```

The non-advection part is, with `d = GR_SPACEDIM`,

```text
Gammadot[i] =
    (2 / d) [
        divbeta (chris.contracted[i] + 2 kappa3 Z_over_chi[i])
      - 2 alpha K Z_over_chi[i]
    ]
  - 2 kappa1_times_lapse Z_over_chi[i]

  + sum_j {
        2 h^ij (alpha partial_j Theta - Theta partial_j alpha)
      - 2 A^ij partial_j alpha
      - alpha [
            2(d-1)/d h^ij partial_j K
          + d A^ij partial_j chi / chi
        ]
      - (chris.contracted[j] + 2 kappa3 Z_over_chi[j])
            partial_j beta^i
    }

  + sum_j sum_k {
        2 alpha Gamma^i_jk A^jk
      + h^jk partial_j partial_k beta^i
      + (d-2)/d h^ij partial_j partial_k beta^k
    }.
```

`advec.Gamma[i]` is the inherited transport/advection term from the derivative
operator. Stage 4AM does not redesign it.

Term classification for the future `i=x` RHS contract:

| Code term | Class | Stage 4 hidden/cartoon issue |
| --- | --- | --- |
| `advec.Gamma[x]` | inherited GRChombo advection/gauge transport | Future cartoon vector advection must use the same hatted variable convention |
| `divshift` multiplying `chris.contracted[x] + 2 kappa3 Z_over_chi[x]` | inherited GRChombo term plus dimension-dependent `d=4` prefactor | `divshift` has hidden contribution `2 beta^x / x`; `chris.contracted[x]` contains `2 W^{-1} Gamma^x_ww` |
| `-2 alpha K Z_over_chi[x]` inside `(2/d)` | Z4 term with dimension-dependent prefactor | Uses encoded upper `Z_over_chi[x]`, not lower Z |
| `-2 kappa1_times_lapse Z_over_chi[x]` | Z4/damping term | Same encoded upper Z convention |
| `2 h^xj (alpha partial_j Theta - Theta partial_j alpha)` | Z4/Theta term | Visible `j=x,z` only for the block metric; no hidden multiplicity for `i=x` unless future off-block components are introduced |
| `-2 A^xj partial_j alpha` | inherited GRChombo lapse-gradient term | Visible contraction for the block metric |
| `-alpha 2(d-1)/d h^xj partial_j K` | dimension-dependent `d=4` term | Must use `d=4`, not grid dimension two |
| `-alpha d A^xj partial_j chi / chi` | dimension-dependent `d=4` term | Must use `d=4`; regularity of `chi_x` is owned later by Stage 4AP/4AQ |
| `-(chris.contracted[j] + 2 kappa3 Z_over_chi[j]) partial_j beta^x` | gauge/shift term plus hatted convention | `j=x,z` visible terms use contracted connections; hidden `j=w` terms require a separate cartoon vector-derivative analysis before implementation |
| `2 alpha Gamma^x_jk A^jk` | inherited GRChombo connection-A term | Hidden multiplicity-two contribution `2 * [2 alpha Gamma^x_ww A^ww]`; `Gamma^x_ww` contains `(A-W)/x`, `B/x`, `W_x` |
| `h^jk partial_j partial_k beta^x` | gauge/shift second-derivative term | Hidden multiplicity-two contribution `2 W^{-1} partial_w partial_w beta^x`; this is a `1/x`-type cartoon vector term and must be derived before Stage 4AN/4AO trust |
| `(d-2)/d h^xj partial_j partial_k beta^k` | dimension-dependent gauge/divergence term | The divergence includes hidden `2 beta^x/x`; visible derivatives of that quantity create `partial_x(beta^x/x)` and `partial_z(beta^x/x)`-type terms |

Known hidden multiplicity-two contractions and `1/x`-type terms:

- `tilde_Gamma^x` itself contains `2 W^{-1} Gamma^x_ww`.
- `Gamma^x_ww` contains `(A-W)/x`, `B/x`, and `W_x`.
- `divshift` contains `2 beta^x/x`.
- `2 alpha Gamma^x_jk A^jk` contains the two hidden `ww` copies.
- `h^jk partial_j partial_k beta^x` contains the two hidden second
  derivatives of a vector component.
- The shift-divergence second-derivative term differentiates the hidden
  `2 beta^x/x` contribution.

Stage 4AM does not implement these RHS terms. It only records the exact
GRChombo formula and the future ownership.

## Stage 4AO Hard GL Gate

Stage 4AN may implement local `hat_Gamma^x` formulas and contracts, but no
live RHS or evolution integration may proceed until Stage 4AO passes.

Stage 4AO must reproduce a semi-analytic or literature Gregory-Laflamme
threshold or growth rate after matching:

- `r0` / radius convention;
- `z` period and `k = 2 pi n / L`;
- gauge;
- perturbation sector;
- resolution;
- extraction variable.

The physical growth observable should be geometric. `hat_Gamma^x` is
gauge-dependent, so it is not itself the physical growth observable. The GL
gate validates the coupled linearized system containing `hat_Gamma^x`, not
constraint damping by itself. Flat tests are insufficient. Pau is not the
convention authority.

## Non-Goals

Stage 4AM does not implement:

- the Stage 4AN local `hat_Gamma^x` header or tests;
- the Gamma RHS;
- GL evolution or dispersion measurement;
- grid/ghost regularity validation;
- finite-axis source evaluation;
- RHS integration;
- live evolution wiring.
