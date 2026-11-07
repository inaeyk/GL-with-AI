# Theorem and proof dependencies

Use \(r_0=1\) in estimates, the audited Schwarzschild time origin and
\(x=r+\log(r-1)\). Put \(u=t-x\), \(\tau=t+x-r\), and
\(g(R)=R-2x(R)\). For \(A>2\), the reference wedge is
\(\{\tau\ge0, u\le g(A)\}\). Its lower boundary is the audited spacelike
slice, and its inner boundary is an outgoing background characteristic.
The exact metric requires its own causal exterior boundary.
The comparison interval is compact in the interior of a padded interval.

## Established exact initial-data estimate

Fix a finite number of radial and compact derivatives and a finite
spatial weight. For the GOAL-006 damped, mass-tuned Z family, choose the
finite damping \(p\) and spare regularity to cover these orders as in
the damped estimate (A3). Let \(Y_*\) be the initial scaled metric and
momentum variables after subtracting the background and first GL jet,
and \(P_o=(1-\mathcal T)/2\) for the half-circle translation \(\mathcal T\).
For all sufficiently small \(|\epsilon|\), the far-end bounds at fixed
compact Sobolev order are
\[
 \|Y_*\|_{H_z^m}\le C\epsilon^2 r^{-p/2},\qquad
 \|P_oY_*\|_{H_z^m}\le C|\epsilon|^3r^{-p/2}.
\]
The first holds by the pinned damped construction. The second follows
by projecting its exact residual equation at fixed \(p\) and using
finite spare derivatives. Differentiating the spatial equations gives
the prescribed radial weights after allowing finite polynomial losses.
Constants and the seed interval depend on the chosen damping, radius
and derivative count. The exact compact scalar free jet vanishes in an
adapted initial gauge. See [the proof](EXTERIOR.md#1-initial-data-with-the-orders-required-by-the-massive-hierarchy).

## Established reference-wedge theorem

Fix \(A>2\), any finite number of differentiated fluxes and a finite
radial power. Choose the GOAL-006 damping parameter and available initial
regularity sufficiently large for those choices. The pinned growing
GL first jet and the actual compatible causal \(\pm 2k\) second jet
of the damped Z data extend over the full reference wedge. The first
jet is exponentially decaying along outgoing rays. For the second jet,
the rescaled orthonormal-frame tensor \(q=r h_2\) has uniformly
bounded polynomially weighted outgoing fluxes for the specified
derivatives, including both null derivatives, and weighted slice
energies. The energy includes \(f(2k)^2 |q|^2\). The result includes the
homogeneous finite-slice transient. See [derivation §1](DERIVATIONS.md#1-weighted-causal-massive-jets-on-the-reference-exterior).

## Target and dependence

For the exactly mass-tuned damped Z family with fixed damping profile
and sufficiently many finite weighted derivatives, the target is a
future development admitting reflection- and half-translation-equivariant
full optical cuts. Normalize their null lapse in the four-dimensional
Einstein frame at infinity and fix the additive constant by the
reference spatial-end comparison. The physical trace is
\(\Phi_\epsilon=\lim_{r_5\to\infty}r_E(\sqrt3/2)\log(\ell/L)\).
The target is \(\Phi_\epsilon\in H^1(I)\) and
\[
 \|\partial_{u_B}\Phi_\epsilon-\epsilon^2 n_Z\|_{L^2(I)}
 \le C_I|\epsilon|^4.
\]
The constants may depend on the damping profile and its weights,
reference mode, regularity, starting radius, clock anchor and padded
interval. They must be uniform in seed and extraction radius.

~~~text
exact damped Z data and weighted parity hierarchy [proved]
  ├─> actual linear and quadratic massive jets on reference wedge [proved]
  └─> causal relative Einstein development on an unbounded wedge [open]
        ├─> nonlinear sector estimates and commuted massive ray flux [open]
        ├─> common mean-chart potential, exact Z jet and scalar forcing [partial]
        └─> full optical eikonal, normalized clock and length matching [partial]
                  │
      uniform scalar H¹ trace and clock comparison [conditional]
                  │
      geometric news, flux and positivity [conditional]
~~~

The background wedge starts at \(r=A\) on \(\Sigma\). Its causal
past avoids the horizon and the strong-field interior. The choice
avoids relying on stability of the interior even though \(t\) is
unbounded on every reference outgoing ray.

## Sufficient exterior theorem to prove

This is a target for the exact evolution. In the background
orthonormal spatial frame on \(\Sigma\), define finite relative norms
by
\[
 \|(a,b)\|_{m,w}^2
  =\sum_{i+j\le m}\int_A^\infty\int_{S^1}
       r^{2w}\bigl(|\partial_r^i\partial_z^j(ra)|^2+
                   |\partial_r^i\partial_z^j(rb)|^2\bigr)\,dz\,dr .
\]
Here \(a=\gamma-\gamma_{\rm Schw}\), \(b=K-K_{\rm Schw}\), with
the same audited slicing used in both subtractions. The initial lapse
and shift can be chosen to match the completed first jet. Smooth
classical data with any fixed \(m,w\) and small norms of orders
\(\epsilon,\epsilon^2,\epsilon^3\) for the three spatial sectors are
available by the damped construction and the result above. The
smallness threshold is positive for the fixed finite regularity,
weight and damping. Uniformity as those parameters grow is unnecessary.

The missing theorem should choose finite \(m,w\) and \(A>2\), and
give an \(\epsilon_*>0\) for this family such that
\(0<|\epsilon|<\epsilon_*\) yields a vacuum exterior development
containing a padded early optical wedge. Its inner causal boundary
must lie in the future domain of dependence of the tail of the
audited slice, with no external incoming boundary prescription.
A relative wave construction must propagate its subsidiary
constraints and compare its cone to physical outgoing cuts.
A characteristic construction must propagate its supplementary
Einstein equations and match their constants to the Cauchy jet.
Existence and estimates should hold on finite incoming truncations
with constants uniform in their outer radii.

For a route through the common mean chart, a sufficient set of
conclusions is as follows.

- Relative coefficients are uniformly Lorentzian and regular to
  the finite commutation order, with \(r_5\) an increasing areal
  coordinate. The mean potential satisfies
  \([V_\epsilon]\in L^1\) on the whole earlier wedge and
  \(L^2\) on the padded interval, with uniform norms.
- The rescaled mean-free fields, split into first/odd,
  even and odd error, obey the GOAL-006 dyadic ray estimate
  \(\int_D^{2D}\|Z_j\|^2\,dv\le C|\epsilon|^{2j}
  (D/R(u))^\delta/R(u)\), \(j=1,2,3\), \(\delta<1\).
  \(Z_j\) includes the specified commuted normalized derivatives.
  For terminal optics use the additional commutations and areal
  comparison in [OPTICAL.md](OPTICAL.md).
- With the Z data transformed into this chart,
  \(F_\epsilon-\epsilon^2F_2+\epsilon^2(V_\epsilon-V_0)\psi_2\)
  has \(L^1_u\) and padded \(L^2_u\) absolute ray norms
  \(O(\epsilon^4)\). This needs the potential difference and
  the complete constrained forcing. The exact scalar identity
  fixes their coefficients but does not bound their magnitudes.
- The full eikonal has vanishing terminal compact momentum and
  uniformly nondegenerate generator Jacobians. The normalized
  clock and geometric circumference trace satisfy the padded
  comparison and differentiability conditions in the optical
  and scalar derivations. The flux requires decay of compact
  modes sufficient to justify the leading circle-independent
  strain and Einstein-frame asymptotics.

The alternate direct full-optical scalar route can replace the mean
potential and source estimates if it proves their ray-current
analogues, including the transformed Z jet and the limiting radial
term. This route needs the same normalized geometric cuts.
