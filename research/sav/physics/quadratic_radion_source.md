# Closed quadratic zero-mode radion equation

Task `T-20261022-GLR-GOAL-001`, 2026-10-22. Expansion convention throughout:
\(g_5=\bar g+\epsilon h+\epsilon^2q+O(\epsilon^3)\), **no factorial**.
The book-keeping \(\epsilon\) is absorbed in \(A(t)\) below. This derivation
is conditional on a genuine on-shell growing GL eigenfunction, not a
projected numerical fixture. Independent reviews: `../verification/AUDIT_FOUNDATION.md`
and `../verification/AUDIT_SOURCE_RETARDED.md`; executable exact checks:
`../verification/check_quadratic_source.py`.

## 1. A unique first-order representative and second-order completion

Use \(\bar g=\bar g_{\mu\nu}^{\rm Sch}dx^\mu dx^\nu+dz^2\),
\(f=1-r_0/r\), \(k=2\pi/L\ne0\). In one complex Fourier sector,
\(\delta h_{zz}=2ik\xi_z\),
\(\delta h_{\mu z}=ik\xi_\mu+\bar\nabla_\mu\xi_z\).
The unique nonzero-k first-order diffeomorphism setting both to zero is

\[
 \xi_z=-\frac{h_{zz}}{2ik},\qquad
 \xi_\mu=-\frac{h_{\mu z}+\bar\nabla_\mu\xi_z}{ik}.
\]

Consequently the four-dimensional tensor

\[
 h^U_{\mu\nu}=h_{\mu\nu}
 +\frac{i}{k}(\bar\nabla_\mu h_{\nu z}+\bar\nabla_\nu h_{\mu z})
 -\frac{1}{k^2}\bar\nabla_\mu\bar\nabla_\nu h_{zz}
 \tag{1}
\]

is invariant under all first-order diffeomorphisms of that sector. This
construction uses inverse \(\partial_z\), **not** an inverse time/radial
operator; it is smooth in ingoing EF coordinates whenever the initial
perturbation is smooth there. A coordinate singularity of Schwarzschild
components is not a gauge obstruction. Fixed L excludes a nonperiodic
rescaling of z. The first-order family has no independent zero mode.

With the active pullback convention the quadratic representative is

\[
 q^U=q+\mathcal L_\xi h+\tfrac12\mathcal L_\xi^2\bar g
       +\mathcal L_\eta\bar g.\tag{2}
\]

Here \(\eta\) is the freely chosen second-order gauge generator in this
parameterization. Its contribution to \(\langle q^U_{zz}\rangle_z\) is
\(\langle2\partial_z\eta_z\rangle_z=0\) for periodic regular gauges.
Equation (2) defines the second-order gauge-invariant scalar completion
\(\varphi=\sqrt3\langle q^U_{zz}\rangle_z/4\). Residual first-order
zero-mode coordinate shifts do not affect it because \(h^U_{zz}=0\).
This is a completed perturbative scalar, not the raw average of \(\log g_{zz}\)
in an arbitrary z-dependent gauge.

The linear \(zz\) and \(\mu z\) vacuum equations in this gauge impose
\(\bar g^{\mu\nu}h^U_{\mu\nu}=0\) and
\(\bar\nabla^\mu h^U_{\mu\nu}=0\). The remaining equation is the massive
Lichnerowicz equation
\(\bar\Box h^U_{\mu\nu}+2\bar R_{\mu\alpha\nu\beta}
h_U^{\alpha\beta}-k^2h^U_{\mu\nu}=0\), with curvature convention
\(R^a{}_{bcd}=\partial_c\Gamma^a_{db}-\partial_d\Gamma^a_{cb}+\cdots\).

Write the real mode as
\(h^U_{\mu\nu}=A(t)H_{\mu\nu}(r)\cos kz\),
\(A=\epsilon p_1e^{\Omega(t-t_0)}\). In Schwarzschild components let
\(T=H_{tt}, B=H_{tr}, D=H_{rr}, K=H_{\theta\theta}/r^2\), with
\(H_{\varphi\varphi}=r^2\sin^2\theta K\).
The invariant amplitude convention for this calculation is
\(K(2r_0)/2=1\): A is the fractional sphere-radius perturbation at
fixed background areal radius \(2r_0\) in the unique unitary representative.
This definition is perturbatively invariant by (1); it is not a coordinate
chi amplitude or an apparent-horizon radius on an unspecified slicing.
It requires \(K(2r_0)\ne0\). All transfer coefficients depend on this choice.

## 2. Explicit complete scalar source

At first order the z direction is in Gaussian normal form. For an
arbitrary four-metric \(\gamma_{\mu\nu}(x,z)\) in
\(ds^2=dz^2+\gamma_{\mu\nu}dx^\mu dx^\nu\), direct Christoffels give

\[
 R_{zz}=-\frac12\operatorname{tr}(\gamma^{-1}\gamma_{,zz})
       +\frac14\operatorname{tr}(\gamma^{-1}\gamma_{,z}
                                      \gamma^{-1}\gamma_{,z}).\tag{3}
\]

This is also \(-\partial_z K_z-\operatorname{tr}K_z^2\), where
\((K_z)_{\mu\nu}=\gamma_{\mu\nu,z}/2\). Expanding the inverse through
quadratic order, \(h_{,zz}=-k^2h\), and averaging cos² and sin² to 1/2 gives

\[
 Q_{zz,0}=\left\langle\tfrac12\operatorname{tr}(h h_{,zz})
                         +\tfrac14\operatorname{tr}(h_{,z}h_{,z})\right\rangle
          =-\frac{k^2A^2}{8}\mathcal H(r),
\quad
 \mathcal H=\frac{T^2}{f^2}+f^2D^2+2K^2-2B^2.\tag{4}
\]

All indices in quadratic expressions are raised with \(\bar g\).
The negative sign of the \(tr\) contribution is essential. For a zero
Fourier second-order field, the **full linearized** five-dimensional Ricci
component is
\(R'_{zz}[q_0]=-\bar\Box_4q_{zz,0}/2\). It contains neither
\(q_{\mu\nu,0}\) nor \(q_{\mu z,0}\). Thus vacuum Einstein equations imply

\[
 \boxed{\bar\Box_4\varphi=-\frac{\sqrt3 k^2}{16}A(t)^2\mathcal H(r)}.
 \tag{5}
\]

For \(\psi=r\varphi\) and the fixed tortoise convention
\(x=r_*=r+r_0\log(r/r_0-1)\),

\[
 \boxed{[\partial_t^2-\partial_x^2+V]\psi=A(t)^2\mathcal S(r)},\qquad
 V=\frac{fr_0}{r^3},\qquad
 \mathcal S=\frac{\sqrt3 k^2rf}{16}
 \left(\frac{T^2}{f^2}+f^2D^2+2K^2-2B^2\right).\tag{6}
\]

This is an explicit radial functional of the reconstructed constrained
linear mode; there are no omitted radial derivatives: they enter through
the TT reconstruction of T,B,D,K. Its compact form follows from the z-Gauss
equation, rather than dropping metric components. A trace-only ansatz,
or retaining only the linear hzz component in a different gauge, fails.

## 3. Independent action route and constraints

Put \(ds_5^2=e^{-2\varphi/\sqrt3}\widetilde g_{\mu\nu}(x,z)dx^\mu dx^\nu
+e^{4\varphi/\sqrt3}dz^2\), with \(\varphi=O(A^2)\),
\(\widetilde g=\bar g+h^U+O(A^2)\). The spacelike-ADM bulk action contains
\(N\sqrt{-\gamma}[R[\gamma]+K_z^2-K_{z,\mu\nu}K_z^{\mu\nu}]\), modulo
the GHY divergence. The x-curvature terms reduce to
\(\sqrt{-\widetilde g}[R[\widetilde g]-2(\nabla\varphi)^2]\) plus an
x-boundary divergence. The TT z-kinetic term is
\(-\tfrac14 e^{-2\sqrt3\varphi}\operatorname{tr}(h^U_{,z})^2\).
Its cubic scalar variation yields
\(4\bar\Box\varphi+(\sqrt3/2)\langle\operatorname{tr}(h^U_{,z})^2\rangle=0\),
which is (5). In this action check \(q^E_{\mu\nu}=q^U_{\mu\nu}
+(q^U_{zz}/2)\bar g_{\mu\nu}\) is held fixed.

The remaining zero-mode equations determine the spherical Einstein-frame
metric backreaction and its mass integration constant. They do not add a
second scalar condition: the background radion is constant, and the
linearized zero-mode Einstein equations decouple from the scalar.
Their sourced compatibility follows from the expanded five-dimensional
contracted Bianchi identity **only for an on-shell first-order mode**.
The mixed source averages to zero for a standing real cosine (every
quadratic mixed term has sin cos parity); there is no forced zero-mode
electric charge in this family. This is a consistency/integrability proof,
not a claim to have integrated the metric backreaction or bounded its norm.

A useful reconstructable expression for the other quadratic Ricci sources
is obtained from the connection difference
\(C^A{}_{BC}=\frac12\bar g^{AD}(\bar\nabla_Bh_{CD}
+\bar\nabla_Ch_{BD}-\bar\nabla_Dh_{BC})\):
\[
 Q_{AB}=\bar\nabla_C(-h^C{}_D C^D{}_{AB})
       -\bar\nabla_B(-h^C{}_D C^D{}_{AC})
       +C^C{}_{CD}C^D{}_{AB}-C^C{}_{BD}C^D{}_{AC}.\tag{7}
\]
The derivative placement assumes the standard difference-of-connections
Ricci identity. Fourier average (7) supplies the remaining forced
constraints if needed; it is not an alternative substitute for (5).

Homogeneous zero modes must be separated. A regular mass shift is allowed.
A static scalar obeys \((r^2f\varphi')'=0\), so
\(\varphi=c_0+c_1\log f\). Fixed asymptotic circle sets \(c_0=0\), and
future-horizon regularity sets \(c_1=0\). An arbitrary homogeneous tension
shift cannot be added as a regular scalar solution of this background;
sourced static backreaction is a different issue. Pure zero-mode gauge
terms carry no \(q_{zz,0}^U\). These facts prevent confusing a charge/gauge
change with scalar news.

## 4. Infinity and proof limits

The admissible linear GL tail decays as \(e^{-\sqrt{k^2+\Omega^2}r}\)
times powers, so its quadratic gauge completion also vanishes at fixed
retarded time \(u=t-x\) at infinity: \(\sqrt{k^2+\Omega^2}>\Omega\).
Consequently the coefficient of \(1/\rho\) in \(\varphi\) agrees with the
Einstein-frame Bondi radion \(\Phi(u)\); second-order shifts of r or u
cannot act on a first-order zero-mode scalar because that field is zero.
This requires admissible asymptotic diffeomorphisms preserving the circle
and fixed asymptotic clock. Large changes of those conventions change the
observable's coordinate description and are not a gauge comparison here.

Equations (5)–(6) establish the complete **scalar** source. They alone do
not establish that \(\mathcal H\) is sign definite, that its outgoing
overlap is nonzero, or that all nonlinear response norms stay small at a
specified finite A. Those are separate numerical/analytic gates. In
particular the Lorentzian contraction in (4) must not be replaced by a
sum of positive squares merely to infer radiation.
