# Modified-Cartoon Source Terms

Purpose: Stage 3B design note for the hidden-direction source terms needed by a
future `CH_SPACEDIM=2`, `GR_SPACEDIM=4` black-string evolution.

Status: design and validation roadmap only. No source terms are implemented.
The current `BlackStringToy` scaffold does not evolve a 5D black string. Do not
add `hww/Aww`, modify CCZ4 RHS code, or treat successful 2D compilation as
physical correctness based on this note alone.

## Stage 3B Scope

The purpose of Stage 3B is to derive, organize, and validate the algebraic
hidden-direction source terms required when two transverse sphere directions are
suppressed by SO(3) symmetry. The core target remains:

- gridded directions: `x`, `z`;
- hidden/reconstructed directions: two transverse directions, denoted
  schematically as `w1`, `w2`;
- physical spatial dimension: `GR_SPACEDIM=4`;
- Chombo grid dimension: `CH_SPACEDIM=2`.

The main risk is silent failure: a code can compile, run, and write HDF5 output
while omitting the hidden-direction algebraic terms that make the reduced
system equivalent to the intended 4+1-dimensional dynamics.

## Reduced Geometry

The transverse flat spatial geometry can be written in spherical form as

```text
dx^2 + x^2 dOmega_2^2
```

but the modified-cartoon representation does not store `g_theta theta = x^2`
as a grid variable. Instead, the hidden Cartesian-like directions have

```text
gamma_ww = 1
```

in flat transverse space. The angular factor `x^2` is supplied by the
coordinate/cartoon reconstruction. This is the same convention used in
`docs/implementation/black_string_initial_data.md`:

```text
R = x sqrt(gamma_ww) = x sqrt(chi^{-1} hww)
```

Setting `gamma_ww = x^2` or `hww = x^2` would double-count the angular radius.

## Spherical-To-Cartoon Mapping

The table below is conceptual but fixes the bookkeeping that future source-term
code must preserve.

| Spherical object | Hidden/cartoon representation |
| --- | --- |
| `g_theta theta = x^2` | `gamma_ww = 1` plus external radius factor `x^2` supplied by reconstruction |
| `g_phi phi = x^2 sin^2 theta` | second hidden direction with the same `gamma_ww = 1` contribution after symmetry contraction |
| `K_theta theta = x beta^x` | hidden Cartesian-like component `K_ww = beta^x / x` |
| `K_phi phi = x beta^x sin^2 theta` | second hidden component with the same `K_ww = beta^x / x` |
| `Gamma^x_theta theta = -x` | contributes hidden-direction algebraic source terms after contraction |
| `Gamma^theta_x theta = 1/x` | maps to explicit `1/x` source terms in reduced equations |
| angular second-derivative contractions | map to `1/x` and `1/x^2` algebraic terms |
| angular trace contribution | maps to two hidden-direction contributions because there are two suppressed directions |
| diagonal tensor trace | `T^x_x + T^z_z + 2 T^w_w` |

The factor of two is not optional. For this project the number of suppressed
spatial directions is

```text
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2
```

so diagonal traces contain `2 T^w_w`. Writing the factor this way helps avoid
hard-coding the wrong multiplicity in future variants. The two hidden
directions are equivalent here because of the imposed SO(3) spherical symmetry.

## GP Extrinsic-Curvature Check

For the unperturbed GP-like black-string slice, use the flat spatial metric

```text
dl^2 = dx^2 + x^2 dOmega_2^2 + dz^2
```

and the natural GP radial shift

```text
beta^x = s sqrt(r0 / x)
```

where `s = +/- 1` is the GP branch. The relevant spherical Christoffel symbols
are

```text
Gamma^x_theta theta = -x
Gamma^x_phi phi = -x sin^2(theta)
Gamma^theta_x theta = 1/x
Gamma^phi_x phi = 1/x
```

Using

```text
K_ij = (1 / (2 alpha)) (D_i beta_j + D_j beta_i - partial_t gamma_ij)
```

with `alpha = 1` and time-independent spatial metric gives

```text
K_xx = partial_x beta_x = -(s / 2) sqrt(r0 / x^3)
```

For the angular directions, `beta_theta = beta_phi = 0`, but covariant
derivatives include Christoffel terms:

```text
K_theta theta = x beta^x = s sqrt(r0 x)
K_phi phi = x beta^x sin^2(theta)
K_zz = 0
```

The `sin^2(theta)` factor cancels in the contracted contribution:

```text
gamma^phi phi K_phi phi
  = (1 / (x^2 sin^2(theta))) (x beta^x sin^2(theta))
  = beta^x / x

gamma^theta theta K_theta theta
  = (1 / x^2) (x beta^x)
  = beta^x / x
```

This demonstrates why the two suppressed directions contribute equally after
contraction and why the cartoon representation uses two copies of `K^w_w`.

Mapping to hidden Cartesian-like cartoon components gives one component per
suppressed direction:

```text
K_ww = K_theta theta / x^2 = beta^x / x = s sqrt(r0 / x^3)
```

and the four-spatial-dimensional trace is

```text
K = K^x_x + K^z_z + K^{w1}_{w1} + K^{w2}_{w2}
  = (3s / 2) sqrt(r0 / x^3)
```

This check is for the unperturbed `chi = 1` GP slice. It remains subject to the
overall GRChombo `K_ij` sign convention. If GRChombo uses the opposite
extrinsic-curvature sign convention, every `K_ij`, `K`, `A_ij`, and `Aww` sign
derived from this check must be flipped.

## Stage 3C/3D Symbolic Derivation Status

Stage 3C verified the flat GP cartoon geometry used above: spherical
Christoffels, the `sin^2(theta)` contraction cancellation, the map to `K_ww`,
and the trace multiplicity from
`N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2`.

Stage 3D starts the nontrivial hidden-metric derivation with
`gamma_ww = q(x,z)` for a diagonal reduced metric. The SymPy scaffold isolates
hidden-sector Christoffel identities, verifies the flat limit, checks angular
contractions, and confirms structural Ricci identities such as
`R_phi phi / sin(theta)^2 = R_theta theta` and the two-direction angular Ricci
scalar contribution.

Stage 3E adds a nonconstant `q(x,z)` regression with
`q = (1 + lambda x + mu z)^2`. It compares direct Ricci computation against
independent warped-product formulas for `dl^2 = dx^2 + dz^2 + f(x,z)^2
dOmega_2^2`, guarding the hidden-sector derivative terms that the Stage 3D
constant-`q0` regression did not test.

### Stage 3F: Conformal-variable translation

Stages 3D and 3E worked mostly in physical variables, especially
`gamma_ww = q`. Stage 3F translates that bookkeeping into conformal variables:

```text
q = gamma_ww = hww / chi.
```

This prepares the future implementation because GRChombo-style CCZ4 evolves
conformal variables such as `chi`, `h_AB`, and `hww`, not only physical
`gamma_ij`. The hidden multiplicity remains

```text
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2.
```

Stage 3F also records the diagonal conformal tracelessness relation

```text
A_xx / h_xx + A_zz / h_zz + 2 Aww / hww = 0.
```

After review, the Stage 3F repository check was strengthened to include the
cartoon/Cartesian-like determinant condition

```text
det gamma_4D = gamma_xx gamma_zz gamma_ww^2
chi = (det gamma_4D)^(-1/4)
det h_4D = h_xx h_zz hww^2 = 1.
```

This determinant condition is imposed on the reduced Cartesian-like variables
with hidden multiplicity; it does not include the spherical-coordinate factors
`x^4 sin^2(theta)`. The symbolic check now also verifies physical/conformal
round trips, `K_xx`, `K_zz`, and `K_ww` reconstruction, and a guard showing
that the `/4` denominator in the `GR_SPACEDIM = 4` extrinsic-curvature
decomposition is essential.

The actual modified-cartoon CCZ4 source-term implementation remains future
work.

These derivation artifacts are inputs to later implementation work. The actual
modified-cartoon CCZ4 source terms remain unimplemented.

## Source-Term Categories

The final CCZ4 modified-cartoon RHS formulas are not fully derived in this
note. The future implementation must account for at least the following
categories.

### Hidden Metric Variables

- `hww` as the conformal hidden Cartesian-like metric component.
- `gamma_ww = chi^{-1} hww`.
- Correct reconstruction of physical sphere radii using the external factor
  `x`, not by storing `x^2` in `hww`.

### Hidden Extrinsic Curvature

- `Aww` and physical `K_ww`.
- Reconstruction convention:

```text
K_ww = chi^{-1} (Aww + hww K / GR_SPACEDIM)
```

- Trace subtraction using `GR_SPACEDIM=4`, not `CH_SPACEDIM=2`.

### Traces And Tensor Contractions

- Every trace over physical spatial directions must include `x`, `z`, `w1`,
  and `w2`.
- For this Stage 3B target,
  `N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2`, so hidden-direction
  multiplicities must be written as two equivalent SO(3) contributions rather
  than inferred from the number of grid axes alone.
- For diagonal reduced tensors, use the trace structure

```text
T^x_x + T^z_z + 2 T^w_w
```

- A missing hidden factor of two can compile and still evolve the wrong system.

### Suppressed-Direction Derivatives

Derivatives in the suppressed directions are replaced by algebraic terms
containing powers such as

```text
1/x
1/x^2
```

These terms appear through Christoffel symbols, covariant derivatives,
connection functions, and curvature contractions. They require explicit
regularity handling near small `x`.

### Ricci Tensor And Scalar

The reduced Ricci tensor/scalar must include hidden-direction contributions.
The Stage 1 source inspection found that the required 5D/SO(3)
modified-cartoon CCZ4 source terms are absent from public GRChombo. Public
CCZ4 contains dimension-parameterized algebra, but not the project-specific
hidden-sphere source-term system required for `CH_SPACEDIM=2`,
`GR_SPACEDIM=4`. This is a statement about the black-string target in this
project, not a defect in public GRChombo's intended 3+1D use.

Therefore Stage 3B is not merely checking whether these terms exist. It is
planning the derivation, implementation, and validation of source terms that
must be added in the project-specific layer.

Detailed Ricci and CCZ4 RHS source formulas are future work and should be
derived independently before implementation.

### Gamma Variables And Gamma Constraints

The conformal connection functions and Gamma constraints must include the
hidden directions consistently. Even if only `Gamma^x` and `Gamma^z` are stored
as gridded components, their source terms can receive algebraic contributions
from the suppressed `w` directions.

### Gauge Equations

The lapse and shift evolution equations need review for hidden-direction terms.
The gauge variables may be gridded only in `x` and `z`, but their RHS can depend
on full-dimensional traces or Gamma quantities. The Stage 3A distinction
between GP-slice geometric data and zero-shift gauge startup remains active.

### Small-`x` Regularization

Terms proportional to `1/x` and `1/x^2` must be regularized near the cartoon
axis or interior cutoff. The initial-data turduckening cutoff around
`x_cut = 0.1 r0` does not automatically solve all evolution-source
regularity issues.

## Validation Strategy

Validation must be layered. A successful compile and smoke run are not enough.

### A. Symbolic Derivation

- Derive reduced Christoffels, Ricci terms, traces, and CCZ4 RHS contributions
  independently.
- Prefer a symbolic check in xAct/Mathematica or SymPy.
- Compare symbolic expressions against hand derivations and the intended
  tensor-storage conventions.
- Keep the spherical-coordinate and hidden-cartoon forms side by side until the
  mapping is audited.

### B. Unit Tests

1. Minkowski in cartoon variables:

```text
chi = 1
h_xx = h_zz = hww = 1
K_ij = 0
A_ij = 0
Aww = 0
```

Hidden source terms should vanish or reduce to regular flat-space
cancellations.

2. Uniform GP black string:

- Use the unperturbed GP fields outside the turduckening region.
- Hamiltonian and momentum constraints should vanish analytically.
- The trace and tracelessness checks should include both hidden directions.

3. Perturbed GL seed:

- Constraint violations should scale with perturbation amplitude `epsilon`.
- Compare `epsilon`, `epsilon/2`, and `2 epsilon` before nonlinear growth.

### C. Runtime Diagnostics

Track at least:

- Hamiltonian constraint;
- momentum constraints;
- CCZ4 `Theta`;
- tracelessness:

```text
h^xx A_xx + h^zz A_zz + 2 h^ww Aww = 0
```

- min/max of `hww` and `gamma_ww`;
- norms of each cartoon source-term category;
- regularity indicators near small `x`;
- horizon radius and area consistency once AH diagnostics are enabled.

### D. Convergence

Use at least three resolutions. Check:

- constraints;
- horizon radius;
- horizon area;
- Fourier modes of the horizon radius;
- source-term norms and regularity diagnostics.

Wrong source terms may converge to the wrong equations. Convergence must be
paired with comparisons against exact or unperturbed solutions, especially
Minkowski and the uniform GP black string.

## Failure Modes

Silent-failure modes to guard against:

- using `CH_SPACEDIM=2` instead of `GR_SPACEDIM=4` in traces;
- forgetting that
  `N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2`, which gives the factor
  of two for the two equivalent hidden directions;
- setting `hww = x^2` and double-counting the angular radius;
- treating `K_ww` as `K_theta theta` instead of the hidden Cartesian-like
  component;
- dropping `1/x` or `1/x^2` terms;
- failing to regularize near small `x`;
- enum layout causing AH or source-term code to read the wrong variable;
- short-run constraints looking acceptable while the solution converges to the
  wrong reduced equations.

## Do Not Implement Yet Checklist

Resolve these before coding modified-cartoon source terms:

- [ ] Future derivation sub-plan for the CCZ4 modified-cartoon RHS:
  - derive reduced metric/inverse-metric bookkeeping, including `hww` and
    `gamma_ww`;
  - derive reduced Christoffel symbols, including all `1/x` terms and regular
    limits;
  - derive hidden-direction Ricci tensor/scalar contributions;
  - derive CCZ4 conformal-metric RHS terms involving `hww`;
  - derive extrinsic-curvature RHS terms involving `Aww` and trace terms;
  - derive Gamma/connection and constraint-propagation terms;
  - derive gauge-equation terms and verify whether hidden-direction
    contributions enter;
  - validate each block against symbolic calculations and exact test data
    before coding the next block.
- [ ] Symbolic verification of Christoffels, Ricci terms, traces, and CCZ4 RHS
  source terms.
- [ ] Decision on small-`x` regularization at the axis/interior cutoff.
- [ ] Exact `UserVariables.hpp` layout for `hww/Aww`.
- [ ] Reliable `GR_SPACEDIM=4`, `CH_SPACEDIM=2` build path.
- [ ] Unit-test design for Minkowski, uniform GP string, and perturbed GL seed.
- [ ] Direct comparison to spherical-coordinate formulas.
- [ ] Convergence acceptance criteria that include exact-solution comparisons.
