# Stage 4AO-A Uniform GP Background And Analytic Residual Lock

Status: Stage 4AO-A documentation and analytic fixture lock. This note does
not start 4AO-B, add discrete operators, add Jacobians, add eigensolvers,
modify production code, or wire live RHS/evolution.

## Authoritative Conventions

The project convention remains a five-dimensional spacetime black string with
four physical spatial directions:

```text
GR_SPACEDIM = 4,
CH_SPACEDIM = 2,
grid directions = x,z,
hidden directions = two equivalent w directions.
```

The uniform background is Schwarzschild_4 times a compact string. The radius
`r0` is the Schwarzschild areal horizon radius of the four-dimensional factor:

```text
f(x) = 1 - r0 / x,
horizon at x = r0.
```

The compact string coordinate is periodic:

```text
z ~ z + L,
k_n = 2 pi n / L.
```

No default rescaling makes the period `2 pi`. Literature quantities such as
`k_c r0` must be convention-mapped in 4AO-C; the commonly quoted
`k_c r0 ~= 0.876` remains provisional until that map is locked.

The locked ingoing Gullstrand-Painleve background is

```text
ds^2 =
    -dt^2
  + (dx + sqrt(r0 / x) dt)^2
  + x^2 dOmega_2^2
  + dz^2.
```

Thus the background slicing gauge is

```text
alpha = 1,
beta^x = v = sqrt(r0 / x),
beta^z = 0.
```

The spatial metric on the GP slice is flat in the reduced/cartoon variables:

```text
gamma_xx = 1,
gamma_xz = 0,
gamma_zz = 1,
gamma_ww = 1.
```

The conformal decomposition follows the project and GRChombo convention

```text
gamma_IJ = chi^{-1} h_IJ.
```

For the unperturbed background,

```text
chi = 1,
h_xx = 1,
h_xz = 0,
h_zz = 1,
h_ww = 1.
```

GRChombo's RHS convention is

```text
partial_t gamma_IJ = -2 alpha K_IJ + L_beta gamma_IJ.
```

For the stationary GP spatial metric this gives

```text
K_IJ = (1 / (2 alpha)) L_beta gamma_IJ.
```

Define

```text
lambda = sqrt(r0 / x^3) = beta^x / x.
```

Then the background extrinsic curvature is

```text
K_xx = -lambda / 2,
K_xz = 0,
K_zz = 0,
K_ww = lambda     for each hidden direction,
K = 3 lambda / 2.
```

The conformal trace-free components are

```text
A_xx = -7 lambda / 8,
A_xz = 0,
A_zz = -3 lambda / 8,
A_ww =  5 lambda / 8.
```

The hatted connection convention is the Stage 4AM/4AN convention. GRChombo
stores `vars.Gamma[i]` as the hatted connection and reconstructs

```text
Z_over_chi[i] = 0.5 * (vars.Gamma[i] - chris.contracted[i]).
```

The unperturbed background uses

```text
Theta = 0,
Z_over_chi^x = Z_over_chi^z = 0,
hat_Gamma^x = tilde_Gamma^x,
hat_Gamma^z = tilde_Gamma^z.
```

## Gauge Startup Lock

4AO-A separates two questions.

Frozen-GP stationarity uses the background slicing gauge above as a prescribed
gauge:

```text
alpha = 1,
beta^x = sqrt(r0 / x),
beta^z = 0.
```

With that frozen gauge, the geometric CCZ4 background residual should vanish
outside the physical singular/turduckening region.

The live evolution gauge-driver family is GRChombo's moving-puncture gauge:

```text
partial_t alpha =
    lapse_advec_coeff * advec(alpha)
  - lapse_coeff * alpha^lapse_power * (K - 2 Theta),

partial_t beta^i =
    shift_advec_coeff * advec(beta^i)
  + shift_Gamma_coeff * B^i,

partial_t B^i =
    shift_advec_coeff * advec(B^i)
  - shift_advec_coeff * advec(Gamma^i)
  + partial_t Gamma^i
  - eta B^i.
```

The GRChombo default parameter family is

```text
lapse_advec_coeff = 0,
lapse_power = 1,
lapse_coeff = 2,
shift_Gamma_coeff = 0.75,
shift_advec_coeff = 0,
eta = 1.
```

The 4AO-A startup state for the validation family is

```text
alpha = 1,
beta^x = sqrt(r0 / x),
beta^z = 0,
B^x = B^z = 0.
```

With the live moving-puncture gauge equation and `Theta=0`, the lapse has the
prescribed startup residual

```text
partial_t alpha = -2 K = -3 lambda.
```

This is the unmodified moving-puncture gauge drift of the GP lapse, not a
failure of the frozen-GP geometric stationarity check. Stage 4AO-D therefore
does not use unmodified moving-puncture GP evolution as its stationary
eigenproblem.

4AO-A locks the live-gauge validation RHS as the same GRChombo
moving-puncture gauge family plus one fixed, field-independent GP-holding lapse
source:

```text
S_alpha(x) = +3 sqrt(r0 / x^3).
```

The validation lapse equation is

```text
partial_t alpha = F_alpha^GRChombo[U] + S_alpha(x),
```

with `S_alpha` computed once from the locked analytic GP background and the
locked `r0` convention. It is not recomputed from the evolving horizon,
evolved lapse, `K`, `Theta`, or any other evolved field. It is restricted to
the 4AO validation harness and is not production Stage 4AR/4AS wiring. All
other background-holding sources vanish on the verified GP background.

Because `S_alpha` is field-independent,

```text
D F_val[U_GP] = D F_GRChombo[U_GP].
```

The fixed source cancels only the zeroth-order GP lapse drift. It does not
change the linearized GRChombo perturbation operator.

4AO-B raw discrete background-residual convergence must be measured with the
unmodified discrete RHS. Its continuum target is zero for the verified
geometric, scalar, and constraint components, and
`-3 sqrt(r0 / x^3)` for the unmodified live moving-puncture lapse equation.
The fixed source `S_alpha=+3 sqrt(r0/x^3)` must not be used to cancel measured
finite-grid residuals before the raw 4AO-B convergence test. After raw
residual convergence is demonstrated, the 4AO validation harness may use the
locked source for the stationary live-gauge 4AO-D problem.

4AO-C frozen gauge and 4AO-D live gauge must use this same pre-locked gauge
family. 4AO-C freezes gauge perturbations on the same locked GP background.
4AO-D evolves gauge perturbations with the same pre-locked moving-puncture
family plus the fixed GP-holding lapse source.

## Perturbation Sector And Observable

The GL validation sector is the SO(3)-symmetric scalar sector on the hidden
sphere, depending on `x` and periodic `z` only. The first mode convention is

```text
cos(k z) or sin(k z),  k = 2 pi n / L.
```

The physical growth observable must be geometric. The preferred observable is
the horizon-radius harmonic

```text
R_H(t,z) = h(t,z) sqrt(gamma_ww(t, h(t,z), z))
         = h(t,z) sqrt(h_ww / chi).
```

`hat_Gamma^x` is gauge-dependent and is not the physical growth observable.

## Lapse, MOTS, And Growth Observable Roles

The apparent horizon is not located from the lapse. No condition such as
`alpha=0` is used in the GL validation observable.

The fixed GP-holding lapse source used by the 4AO-D live-gauge validation
harness is the analytic background function

```text
+3 sqrt(r0 / x^3).
```

It uses the locked background `r0` and is not recomputed from the evolving
horizon, evolved lapse, `K`, `Theta`, or any other evolved field. The lapse
must be monitored separately to verify that the intended gauge background
remains fixed; the lapse profile is not the GL growth observable.

At selected diagnostic output times, find the outermost `S2 x S1` MOTS
continuously connected to the unperturbed surface `x=r0` by solving

```text
Theta_+ = D_i s^i + K_ij s^i s^j - K = 0.
```

The MOTS solve uses the spatial metric and extrinsic curvature. The lapse
affects the MOTS only indirectly through the chosen slicing and the evolved
fields. Record the coordinate surface

```text
x = h(t,z),
```

then compute the physical local sphere radius at the surface:

```text
R_H(t,z) = h(t,z) sqrt(h_ww / chi) evaluated at x=h(t,z).
```

A nonlinear horizon solve is not required at every evolution timestep. Use a
diagnostic cadence sufficient to resolve the linear growth plateau, and require
the extracted growth rate to be stable when that cadence is refined. In the
spectral problem, use the linearized MOTS equation to map the eigenvector to
`delta R_H` rather than repeatedly solving a nonlinear horizon.

Frozen- and live-gauge comparisons must use the same locked slicing convention,
the same GP background, and the same horizon observable.

What is checked when:

1. Every RHS evaluation: evolve and monitor gauge and geometric variables.
2. Selected output times: solve the MOTS and measure `R_H` harmonics.
3. Spectral analysis: solve the linearized MOTS response.
4. Acceptance: compare the horizon-harmonic growth rate, not the lapse profile.

## Radial Domain Lock

4AO-A locks the validation domain as an excised exterior/interior strip with a
positive inner radius:

```text
x in [x_in, x_out],
0 < x_in < r0 < x_out,
z in [0,L), periodic.
```

No 4AO-A or 4AO-B validation harness may include the physical point `x=0`.
The GL validation path therefore excludes the physical GP singularity by an
inner boundary `x_in > 0`; it does not use an unspecified interior extension or
turduckening profile. Boundary-location convergence and the eventual numerical
choice of `x_in/r0` are 4AO-C/4AO-D validation matters, but the continuum
background residual ledger below is defined only on `x >= x_in > 0`.

This separates two issues:

- cartoon-coordinate artifacts such as `B/x`, `(A-W)/x`, `beta^x/x`, and
  hidden vector derivatives must cancel or have a stated finite-radius value in
  the continuum equations;
- the divergence of `lambda=sqrt(r0/x^3)` as `x -> 0` is the genuine GP
  physical-singularity scaling. It is not repaired by cartoon regularity, and
  it is not part of 4AO-A/4AO-B.

## Frozen-GP Continuum Residual Ledger

All targets in this section are continuum targets outside the physical
singularity and before any turduckening. They are analytic targets for 4AO-A,
not discrete tests.

The background is vacuum and constraint-satisfying:

```text
R_IJ[gamma] = 0,
D_I D_J alpha = 0,
Theta = 0,
Z_over_chi^I = 0,
Hamiltonian constraint = 0,
Momentum constraint = 0.
```

Useful derivatives are

```text
v = beta^x = x lambda,
partial_x beta^x = -lambda / 2,
partial_x partial_x beta^x = 3 lambda / (4x),
partial_w partial_w beta^x = partial_y partial_y beta^x
                           = -3 lambda / (2x)
                           for each hidden direction,
partial_x lambda = -3 lambda / (2x),
beta^x partial_x lambda = -3 lambda^2 / 2.
```

The shift divergence in four physical spatial dimensions is

```text
div beta = partial_x beta^x + 2 beta^x / x
         = -lambda / 2 + 2 lambda
         = 3 lambda / 2
         = K.
```

Therefore the conformal-factor RHS target is

```text
partial_t chi =
    beta^x partial_x chi
  + (2 / 4) chi (alpha K - div beta)
  = 0 + (1 / 2)(3 lambda / 2 - 3 lambda / 2)
  = 0.
```

The conformal metric RHS targets vanish component by component. For the
visible and hidden diagonal components:

```text
partial_t h_xx =
    -2 A_xx
  - (2 / 4) h_xx div beta
  + 2 partial_x beta^x
  = 7 lambda / 4 - 3 lambda / 4 - lambda
  = 0,

partial_t h_zz =
    -2 A_zz
  - (2 / 4) h_zz div beta
  = 3 lambda / 4 - 3 lambda / 4
  = 0,

partial_t h_ww =
    -2 A_ww
  - (2 / 4) h_ww div beta
  + 2 h_ww beta^x / x
  = -5 lambda / 4 - 3 lambda / 4 + 2 lambda
  = 0.
```

The `xz` component is zero because `A_xz=0`, `h_xz=0`, and the background
shift has no `z` component or `z` dependence.

The complete metric ledger is:

| Component | Substituted RHS | Target |
| --- | --- | --- |
| `chi` | `0 + (1/2)(K-div beta)` | `0` |
| `h_xx` | `-2A_xx -(1/2)div beta + 2 partial_x beta^x = 7lambda/4 - 3lambda/4 - lambda` | `0` |
| `h_zz` | `-2A_zz -(1/2)div beta = 3lambda/4 - 3lambda/4` | `0` |
| `h_ww` | `-2A_ww -(1/2)div beta + 2 beta^x/x = -5lambda/4 - 3lambda/4 + 2lambda` | `0` |
| `h_xz` | `-2A_xz + h_xx partial_z beta^x + h_zz partial_x beta^z = 0` | `0` |

For the trace curvature variable,

```text
K = 3 lambda / 2,
beta^x partial_x K = -K^2,
R[gamma] = 0,
D^I D_I alpha = 0,
Theta = 0.
```

Thus the non-damping part of the GRChombo CCZ4 target is

```text
partial_t K = beta^x partial_x K + alpha K^2 = 0.
```

The trace-free extrinsic-curvature geometric block
`[-D_I D_J alpha + alpha R_IJ]^{TF}` is zero on the background because
`alpha=1` and the GP spatial metric is flat. The remaining GRChombo terms are
advection, `A_IJ(K-(2/d)div beta)`, shift stretching, and the quadratic
`-2 A_IK A^K_J` term, with hidden stretching `2 A_ww beta^x/x` for `ww`.

| Component | Substituted RHS | Target |
| --- | --- | --- |
| `A_xx` | `21lambda^2/16 -21lambda^2/32 + 7lambda^2/8 -49lambda^2/32` | `0` |
| `A_zz` | `9lambda^2/16 -9lambda^2/32 -9lambda^2/32` | `0` |
| `A_ww` | `-15lambda^2/16 +15lambda^2/32 +5lambda^2/4 -25lambda^2/32` | `0` |
| `A_xz` | all advection, geometric, stretching, and quadratic terms vanish because `A_xz=0`, `partial_z beta^x=0`, `partial_x beta^z=0`, and no `z` dependence is present | `0` |

The entries are, in order:

```text
advec(A_IJ),
A_IJ (K - (2/4) div beta),
shift-stretching,
-2 A_IK A^K_J.
```

For the CCZ4 `Theta` equation,

```text
A_IJ A^IJ = 27 lambda^2 / 16,
((d - 1) / d) K^2 = (3 / 4) K^2 = 27 lambda^2 / 16,
d = 4,
```

so the Hamiltonian/Theta source combination vanishes:

```text
partial_t Theta = 0.
```

The scalar and constraint ledger is:

| Quantity | Substituted expression | Target |
| --- | --- | --- |
| `K` RHS | `beta^x partial_x K + K^2 = -9lambda^2/4 + 9lambda^2/4` | `0` |
| `Theta` RHS | `(1/2)(-A_IJ A^IJ + (3/4)K^2) = (1/2)(-27lambda^2/16 + 27lambda^2/16)` | `0` |
| Hamiltonian | `R - A_IJ A^IJ + (3/4)K^2 = 0 -27lambda^2/16 +27lambda^2/16` | `0` |
| Momentum `x` | `partial_x A_xx + 2(A_xx-A_ww)/x - (3/4)partial_x K = 21lambda/(16x) -3lambda/x +27lambda/(16x)` | `0` |
| Momentum `z` | all terms vanish by uniformity and zero `xz` components | `0` |
| `det(h)` | `(A C - B^2) W^2 = (1*1-0)1` | `1` |
| `tr(A)` | `A_xx + A_zz + 2A_ww = -7lambda/8 -3lambda/8 +10lambda/8` | `0` |

## Full `hat_Gamma^x` Hidden Contraction

Stage 4AM locked the general away-axis contraction

```text
tilde_Gamma^x =
    H^xx Gamma^x_xx
  + 2 H^xz Gamma^x_xz
  + H^zz Gamma^x_zz
  + 2 W^{-1} Gamma^x_ww,
```

with hidden Christoffel

```text
Gamma^x_ww =
    H^xx [(A - W) / x - W_x / 2]
  + H^xz [B / x - W_z / 2].
```

On the uniform GP background,

```text
A = C = W = 1,
B = 0,
D = 1,
H^xx = H^zz = 1,
H^xz = 0,
A_x = A_z = B_x = B_z = C_x = C_z = W_x = W_z = 0.
```

Therefore

```text
Gamma^x_xx = Gamma^x_xz = Gamma^x_zz = 0,
Gamma^x_ww = [(1 - 1) / x - 0] = 0,
2 W^{-1} Gamma^x_ww = 0,
tilde_Gamma^x = 0,
hat_Gamma^x = tilde_Gamma^x + 2 Z_over_chi^x = 0.
```

The determinant-reduced cross-check is also zero:

```text
(2 / x)(W^{-1} - H^xx) = (2 / x)(1 - 1) = 0.
```

This verifies that the full hidden contraction has no leftover background
cartoon singular term.

The companion `z` contraction is also zero:

```text
tilde_Gamma^z =
    H^xx Gamma^z_xx
  + 2 H^xz Gamma^z_xz
  + H^zz Gamma^z_zz
  + 2 W^{-1} Gamma^z_ww
  = 0,
hat_Gamma^z = 0.
```

## `hat_Gamma` Evolution RHS Ledger

GRChombo evolves the hatted variable:

```text
partial_t hat_Gamma^i = advec(Gamma^i) + Gammadot[i],
Gamma^i = hat_Gamma^i.
```

On the GP background,

```text
hat_Gamma^x = hat_Gamma^z = 0,
Z_over_chi^i = 0,
Theta = 0,
partial_i alpha = 0,
partial_i chi = 0,
chris.contracted^i = 0.
```

For `i=z`, every term vanishes by `z` uniformity, `beta^z=0`,
`h^{zx}=0`, and zero `z`-connection data:

```text
partial_t hat_Gamma^z = 0.
```

For `i=x`, the nonzero cancellation is:

| RHS family | Substituted value |
| --- | --- |
| `advec(Gamma^x)` | `0` |
| `(2/d) div beta (chris.contracted^x + 2 kappa3 Z_over_chi^x)` | `0` |
| Z4 damping and `K Z_over_chi^x` terms | `0` |
| `2 h^xj(alpha partial_j Theta - Theta partial_j alpha)` | `0` |
| `-2 A^xj partial_j alpha` | `0` |
| `-alpha [2(d-1)/d h^xj partial_j K]` | `-(3/2) partial_x K = +27 lambda/(8x)` |
| `-alpha d A^xj partial_j chi / chi` | `0` |
| `-(chris.contracted^j + 2 kappa3 Z_over_chi^j) partial_j beta^x` | `0` |
| `2 alpha Gamma^x_jk A^jk`, visible pieces | `0` |
| hidden `2 * [2 alpha Gamma^x_ww A^ww]` | `0` because `Gamma^x_ww=0` |
| visible `h^jk partial_j partial_k beta^x` | `partial_x partial_x beta^x = +3 lambda/(4x)` |
| hidden `h^jk partial_j partial_k beta^x` | `2 partial_w partial_w beta^x = -3 lambda/x` |
| subtotal `h^jk partial_j partial_k beta^x` | `-9 lambda/(4x)` |
| `(d-2)/d h^xj partial_j partial_k beta^k` | `(1/2)partial_x div beta = -9 lambda/(8x)` |

Therefore

```text
partial_t hat_Gamma^x =
    27 lambda / (8x)
  -  9 lambda / (4x)
  -  9 lambda / (8x)
  = 0.
```

This table is the 4AO-A continuum guard against dropping hidden multiplicity in
the `hat_Gamma^x` RHS. The two hidden copies of
`partial_w partial_w beta^x` are essential.

## Live Gauge RHS Interpretation

For the locked moving-puncture gauge family and startup state,

```text
alpha = 1,
beta^x = sqrt(r0/x),
beta^z = 0,
B^x = B^z = 0,
Theta = 0,
partial_t hat_Gamma^x = partial_t hat_Gamma^z = 0.
```

The exact live gauge RHS is:

| Variable | Substituted RHS | Target |
| --- | --- | --- |
| `lapse` | `-2 alpha (K-2Theta) = -3lambda` | prescribed nonzero startup residual |
| `shift^x` | `shift_Gamma_coeff B^x = 0` | `0` |
| `shift^z` | `shift_Gamma_coeff B^z = 0` | `0` |
| `B^x` | `partial_t Gamma^x - eta B^x = 0` | `0` |
| `B^z` | `partial_t Gamma^z - eta B^z = 0` | `0` |

The unmodified GP state is therefore not an equilibrium of the full live
moving-puncture gauge system, because the lapse residual is nonzero. That
deferred ambiguity is resolved in 4AO-A by the fixed, field-independent
GP-holding lapse source. The historical alternatives of a residual-subtracted
tangent operator, drifting-background treatment, different gauge source family,
or different stationary live-gauge background are rejected for the 4AO-D
validation path. Do not reinterpret an instantaneous Jacobian around the
unmodified live-gauge startup state as a stationary GL spectrum.

With the 4AO-D validation source included,

```text
partial_t alpha = -3 lambda + S_alpha(x) = 0,
partial_t shift^x = partial_t shift^z = 0,
partial_t B^x = partial_t B^z = 0.
```

The fixed source has no perturbative derivative, so the live-gauge linearized
operator remains the GRChombo moving-puncture perturbation operator.

## Analytic `1/x` Cancellation And Limits

| Apparent singular term | Classification on GP background |
| --- | --- |
| `(A-W)/x` in `Gamma^x_ww` | identically zero because `A=W=1` |
| `B/x` in `Gamma^x_ww` | identically zero because `B=0` |
| `(2/x)(W^{-1}-H^xx)` in determinant-reduced `tilde_Gamma^x` | identically zero because `W^{-1}=H^xx=1` |
| `W_x/x`, `chi_x/x`, `alpha_x/x` checked-quotient analogues | finite zero limits because the background values are constant |
| `2 beta^x/x` in `div beta` | genuine GP scaling `2lambda`; finite on `x>=x_in`, divergent as `x->0`, and canceled against `K` in the `chi` RHS |
| `2 h_ww beta^x/x` in `partial_t h_ww` | canceled against `-2A_ww-(1/2)h_ww div beta` |
| `2 A_ww beta^x/x` in `partial_t A_ww` | canceled against advection, trace, and quadratic `A` terms |
| `2 partial_w partial_w beta^x` in `partial_t hat_Gamma^x` | canceled with `partial_x partial_x beta^x`, `(1/2)partial_x div beta`, and the `K_x` term |
| `2(A_xx-A_ww)/x` in the momentum constraint | canceled with `partial_x A_xx` and `-(3/4)partial_x K` |
| `lambda` and `lambda/x` themselves | genuine GP physical-singularity scaling; excluded from the validation domain by `x_in>0` |

Cartoon coordinate singularities therefore cancel or vanish in the locked
background equations. The physical GP singularity at `x=0` does not cancel and
is outside the 4AO-A/4AO-B domain.

## 4AO-A Completion Status

The 4AO-A documentation prerequisites are now locked:

- exact uniform ingoing-GP background and `r0` convention;
- compact-`z` normalization and perturbation-sector convention;
- GRChombo `K_IJ` sign and conformal variable convention;
- frozen-GP continuum residual ledger for every evolved component;
- live moving-puncture gauge RHS interpretation plus the fixed
  field-independent GP-holding lapse source;
- full background `hat_Gamma^x` hidden contraction and RHS multiplicity
  cancellation;
- Hamiltonian, momentum, determinant, and trace-free constraints;
- positive-inner-radius radial domain excluding `x=0`;
- geometric growth observable.

Follow-on work is not a 4AO-A blocker but must not be confused with 4AO-A:

- 4AO-B now supplies the local discrete residual convergence, `delta hww`
  hidden-contraction, actual-discrete-RHS Jacobian-vector, and parity-sector
  preflight harness on the provisional `r0=1`, `x in [0.5,4.0]` domain;
- 4AO-C now has an intended frozen-gauge spectral-gate note, but remains
  incomplete until the coupled modified-cartoon frozen-gauge linearized RHS,
  radial spectral boundary conditions, actual-operator parity/JVP checks,
  linearized MOTS map to `delta R_H`, unstable/stable spectral points,
  threshold crossing, convergence checks, and primary-source `k_c r0`
  convention map are implemented. The `k_c r0 ~= 0.876` value remains
  provisional;
- 4AO-D owns live-gauge/full acceptance using the locked moving-puncture
  family plus fixed GP-holding lapse source;
- 4AP/4AQ still own actual grid regularity and finite-axis source evaluation.
