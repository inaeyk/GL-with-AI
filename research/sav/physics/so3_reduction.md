# Exact SO(3) reduction: local action and degree of freedom count

**Status:** bulk equations, boundary reduction and Hamiltonian count
independently audited in GOAL-001; see `../verification/AUDIT_FOUNDATION.md`.
No production numerical simulation claim.

Start with `S_5=(16 pi G_5)^{-1} int sqrt(-g_5) R_5` and the
most general SO(3)-invariant warped product
`ds_5^2=g_ab(x) dx^a dx^b+R(x)^2 dOmega_2^2`,
where `x^a=(t,r,z)`, `z~z+L`, and `R>0` in a regular orbit region.
There is no invariant one-form on S^2, so no sphere/orbit-space mixed metric
component is allowed by full SO(3).

The warped-product curvature identity for a unit 2-sphere is

```text
R_5 = R_3 + 2/R^2 - 4 (Box_3 R)/R
                   - 2 (nabla R)^2/R^2.
sqrt(-g_5) = sqrt(-g_3) R^2 sin(theta).
```

The integral over S^2 contributes 4 pi, and the `-4 R Box R`
term integrates to `+4(nabla R)^2` up to boundary terms. Thus

```text
S_SO3 = (1/(4 G_5)) int dtdrdz sqrt(-g_3)
               [ R^2 R[g_3] + 2(nabla R)^2 + 2 ].
```

This exact 2+1 dilaton-gravity action retains all compact Fourier modes;
it is **not** the z-independent 4D Einstein-frame KK reduction.

The formal Euler-Lagrange variations are

```text
R R[g_3] - 2 Box_3 R = 0                      (delta R)

R^2 G_ab[g_3] + 2R(g_ab Box R - nabla_a nabla_b R)
  + g_ab[(nabla R)^2 - 1] = 0                (delta g^{ab})
```

in regular regions and with the standard boundary terms/variations.
Compare independently with the direct Ricci conditions
`R^(5)_ab=R^(3)_ab-2 nabla_a nabla_b R/R=0` and
`R^(5)_sphere=(1-(nabla R)^2-R Box R)gamma_S2=0`.
**Audit these signs and coefficients** against explicit Christoffels and
the full 5D Einstein tensor before certifying the EOM.

Local physical degree count: the spatial orbit metric has 3 configuration
components plus one scalar R. Lapse and two shifts are multipliers.
Three first-class Hamiltonian/momentum constraints remove 6 phase-space
dimensions, leaving `(2*4-2*3)/2=1` local configuration degree, away from
singular gauge charts/degenerate orbits. If lapse/shift canonical pairs
are retained instead, count six primary-plus-secondary first-class
constraints in a 14-dimensional phase space: `(14-2*6)/2=1`.
The earlier explanation mixed these two counts. 3D pure gravity has no local graviton;
coupling to R leaves a gravitational/dilaton scalar mode. This does not
classify independent global charges or boundary excitations.

For the background `g_3=Schwarzschild_2(t,r) x dz^2`, R=r,
`f=1-r0/r`. Verify the equations on this exact string.
For each compact harmonic `e^{ik_n z}`, one SO(3)-singlet physical
master degree may propagate (with special treatment of n=0 stationary
mass/tension data and exceptional constraint sectors). The n=0
far-zone degree is the 4D radion; the n!=0 sector is the SO(3)
singlet/helicity-zero part of the massive 4D KK graviton.

**Do not** mistake the reduced action's single physical local degree for
"no radiation"; one-dimensional wave motion in the radial direction
persists even under exact S^2 rotations.

For a smooth non-null orbit boundary with fixed induced metric and R,
integrating by parts the bulk term produces
`-G5^-1 int epsilon sqrt(|h|) R n.grad R`. The 5D GHY term, with
`K5=K3+2 n.grad R/R`, cancels this derivative term, leaving
`S_bdy=(2G5)^-1 int epsilon sqrt(|h|) R^2 K3`. Null/corner terms require
their own limits/terms. The audit gives explicit canonical constraints in
the regular conformal frame `g3E=R^4 g3` and verifies the scalar flux by
an independent five-dimensional TT current.

GOAL-001 results: `gl_linear_mode.md` constructs two gauge descriptions
of the same constrained master; `quadratic_radion_source.md` gives the
closed quadratic scalar source. Numerical coefficients and unresolved
finite-amplitude bounds are distinguished in the goal report.

