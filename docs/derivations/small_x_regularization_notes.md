# Small-x Regularization Notes

Status: Stage 3I documentation and symbolic-limit scaffold. This is not
GRChombo evolution code and does not implement finite differences, ghost-cell
rules, axis boundary conditions, or CCZ4 source terms.

## Purpose

Stage 3I records the regularity requirements near the cartoon/spherical axis
`x = 0` for the future `CH_SPACEDIM = 2`, `GR_SPACEDIM = 4` black-string
modified-cartoon implementation. Stages 3C through 3G validated geometry and
conformal-cartoon algebra, and Stage 3H decomposed the future CCZ4 RHS into
blocks. Stage 3I focuses on the apparent singular structures that appear when
the two suppressed sphere directions are reconstructed from the reduced
`x,z` grid.

The project conventions remain:

- physical spatial dimension: `GR_SPACEDIM = 4`;
- Chombo/grid dimension: `CH_SPACEDIM = 2`;
- gridded directions: `x,z`;
- hidden directions: two equivalent `w` directions;
- hidden multiplicity: `N_hidden = 4 - 2 = 2`;
- `hww` is a Cartesian-like hidden conformal metric component, not
  `g_theta theta`;
- the spherical/cartoon reconstruction supplies the external `x^2` factor;
- `g_theta theta = x^2 gamma_ww = x^2 hww / chi`;
- full traces and trace-free projections are 4D, and the conformal
  extrinsic-curvature denominator remains `/4`.

## Cartoon Axis Versus Black-String Singularity

In the reduced 5D black-string cartoon setup, two different issues are
co-located at the coordinate location `x = 0`:

- the cartoon/spherical axis is at `x = 0`;
- the unregularized black-string curvature singularity is also at `x = 0`.

These are conceptually distinct. The cartoon-axis issue is a removable
coordinate-singularity, parity, and tensor-regularity problem for smooth
SO(3)-symmetric fields. The black-string singularity is a physical curvature
singularity of the target spacetime.

Turduckening or interior regularization is intended to remove or regularize
the physical singularity from the active evolved problem. The cartoon-axis
treatment separately handles the removable singular structures created by the
spherical/cartoon reduction. Turduckening therefore addresses the fact that the
physical singularity is co-located with the cartoon axis, but it does not
replace correct axis parity and removable-singularity treatment. Conversely,
passing the small-`x` cartoon regularity checks here does not prove that the
black-string interior has been physically regularized correctly.

## Geometric Issue

Near `x = 0`, modified-cartoon formulas contain apparent singular structures
such as

```text
1/x,
1/x^2,
(gamma_xx - gamma_ww) / x^2,
(partial_x q) / x,
```

where `q` is a hidden/cartoon quantity such as `gamma_ww`. In conformal
variables, analogous combinations involve `h_xx`, `hww`, `chi`, and their
radial derivatives.

These terms can be removable singularities when smooth SO(3)-symmetric parity
and axis-matching conditions hold. They are still dangerous if evaluated
naively in numerical form. A future implementation must either use regularized
limits, appropriate axis/parity filling, or another carefully validated
strategy. Stage 3I identifies the structures and expected limits; it does not
choose the finite-difference implementation.

## Expected Parity

The following parity expectations are for smooth SO(3)-symmetric data stored as
cartoon/Cartesian-like reduced variables. They should be aligned with
GRChombo-facing project conventions and documented consistently before C++
work.

| Quantity | Expected parity in `x` | Notes |
| --- | --- | --- |
| `chi` | even | scalar-like conformal factor |
| `K` | even | scalar trace |
| `Theta` | even | scalar Z4 field |
| lapse `alpha` | even | scalar gauge variable |
| `h_xx`, `h_zz`, `hww` | even | diagonal tensor components in the reduced block and hidden sector |
| `h_xz` | odd | one radial index and one string index |
| `A_xx`, `A_zz`, `Aww` | even | diagonal trace-free extrinsic-curvature components |
| `A_xz` | odd | one radial index and one string index |
| `hat_Gamma^x` | expected odd | radial component of hatted conformal connection; confirm implementation convention |
| `hat_Gamma^z` | expected even | string component; confirm implementation convention |
| shift `beta^x` | expected odd | radial vector component |
| shift `beta^z` | expected even | string vector component |
| Gamma-driver auxiliaries | component-dependent | should follow the parity of the associated shift/connection component |

These are axis regularity expectations, not statements about physical
black-string correctness. If the production domain avoids the true axis using a
turduckening cutoff, the same formulas still matter because hidden-sector
source terms contain powers of `1/x` near the cutoff.

## Axis Smoothness Conditions

For a smooth diagonal reduced metric, the Cartesian-like radial and hidden
metric components must match at the axis:

```text
gamma_xx - gamma_ww = O(x^2).
```

This condition is the reason `(gamma_xx - gamma_ww) / x^2` can have a finite
limit. Since

```text
gamma_xx = h_xx / chi,
gamma_ww = hww / chi,
```

and `chi` should be even and finite at the axis, the conformal analogue is

```text
h_xx - hww = O(x^2).
```

For a smooth off-diagonal reduced tensor component,

```text
gamma_xz = O(x),
h_xz = O(x),
A_xz = O(x).
```

The same odd-in-`x` expectation applies to radial vector-like components such
as `beta^x` and likely `hat_Gamma^x`, subject to convention checks.

## Conformal Connection As The Highest-Risk Axis Quantity

Scalar-like variables and manifestly odd tensor components are comparatively
easy to check with generic Taylor representatives. The hatted conformal
connection, especially `hat_Gamma^x`, is higher risk because it is assembled
from contracted angular Christoffels that can carry bare `1/x` behavior.

For the local Stage 3I symbolic guard, the hidden-sector singular part of the
cartoon contracted conformal connection is represented with the documented
convention

```text
tilde_Gamma^x_singular
  = (N_hidden / x) (1 - h_xx / hww),
N_hidden = 2.
```

With the smooth-axis matching condition `h_xx - hww = O(x^2)`, this expression
is finite and odd in `x`. If the axis values of `h_xx` and `hww` do not match,
the same assembled expression exposes a generic residual `1/x` divergence. A
wrong sign or missing hidden multiplicity factor can pass generic parity tests
on individual fields while still leaving an assembled connection error.

The hatted connection uses the Stage 3H convention

```text
hat_Gamma^i = tilde_Gamma^i + 2 Z^i.
```

The symbolic check assumes regular `Z^x = O(x)` and verifies that the
corresponding `hat_Gamma^x` expression remains finite. `hat_Gamma^x` is
expected to be odd, and `hat_Gamma^z` is expected to be even, but the final
sign and full contracted-connection assembly must still be derived and
documented in the GRChombo-facing cartoon convention before C++ work.

This connection guard matters because `hat_Gamma^i` couples to the Gamma-driver
shift/gauge sector. A residual axis `1/x` error in `hat_Gamma^x` can therefore
propagate into gauge evolution even if simpler scalar and tensor parity checks
pass.

## Symbolic Limit Checks

The companion script

```text
python3 docs/derivations/small_x_regularization_sympy.py
```

uses truncated Taylor representatives:

```text
q(x,z) = q0(z) + q2(z) x^2 + O(x^4),
gamma_xz(x,z) = s1(z) x + s3(z) x^3 + O(x^5).
```

It checks representative removable-singularity limits, including

```text
lim_{x -> 0} (partial_x q) / x = 2 q2,
lim_{x -> 0} (gamma_xx - gamma_ww) / x^2 = finite,
lim_{x -> 0} gamma_xz / x = finite.
```

It also verifies that a conformal `chi` with even parity preserves the finite
limit of `(gamma_xx - gamma_ww) / x^2` when `h_xx - hww = O(x^2)`, and that
unmatched axis values expose an `x^-2` pole. The script also checks the
assembled `tilde_Gamma^x` singular part above, verifies that violating
`h_xx - hww = O(x^2)` exposes a residual `1/x` divergence, and verifies that
regular `Z^x` parity keeps `hat_Gamma^x = tilde_Gamma^x + 2 Z^x` finite.
These are local symbolic checks only; they do not derive the full CCZ4 RHS.

Current run output:

```text
Stage 3I small-x regularization symbolic checks
Taylor expansions are truncated representatives near x = 0.
PASS even scalar partial_x f / x limit = 2*f2
PASS gamma_ww-like q partial_x q / x limit = 2*q2
PASS (gamma_xx - gamma_ww) / x^2 finite limit = -gw2 + gx2
PASS conformal (gamma_xx - gamma_ww) / x^2 finite limit = (-hw2 + hx2)/chi0
PASS odd gamma_xz / x finite limit = s1
PASS odd gamma_xz radial regular combination finite limit = 2*s3
PASS conformal h_xz / x finite limit = chi0*s1
PASS odd A_xz / x finite limit = s1
PASS odd radial vector beta_x / x finite limit = s1
PASS even axial vector partial_x beta_z / x limit = 2*f2
PASS assembled cartoon tilde_Gamma^x finite under h_xx - hww = O(x^2) = 0
PASS assembled cartoon tilde_Gamma^x odd coefficient finite = 2*(hw2 - hx2)/h0
PASS assembled cartoon tilde_Gamma^x exposes 1/x divergence if matching is violated = 2 - 2*hxx0_bad/hww0_bad
PASS hat_Gamma^x finite with regular Z^x contribution = 0
PASS hat_Gamma^x odd coefficient finite = 2*(h0*z1 + hw2 - hx2)/h0
PASS hat_Gamma^z even representative partial_x / x finite = 2*z2
PASS hat_Gamma^x / hat_Gamma^z parity convention documented
PASS unmatched gamma_xx/gamma_ww axis values expose x^-2 pole
PASS diagonal matching removes x^-2 pole = 0
All Stage 3I small-x regularization checks passed.
```

## Numerical Implementation Deferred

Stage 3I deliberately does not decide:

- ghost-cell parity filling;
- one-sided or staggered stencils near `x = 0`;
- whether to evaluate analytic limits pointwise at the axis;
- how to combine turduckening cutoff logic with source-term regularization;
- finite-difference order near the cutoff;
- C++ enum layout or source-term routine structure.

Those decisions belong to Stage 3J unit-test fixture design, Stage 3K
implementation planning, or the later implementation stage after comparison
with the GRChombo-facing project conventions.

## Acceptance Criteria

Stage 3I is complete when:

- the small-`x` risks are documented;
- parity expectations are listed with uncertainty flags where needed;
- diagonal and off-diagonal smoothness conditions are recorded;
- minimal symbolic limit checks exist and pass;
- implementation details remain deferred;
- no C++ code has changed.
