# Finite-window weak rejection tests with an adjoint energy bound

Use the exact-curl hierarchy and physical preparation in [Inputs](../PROGRAM.md).
Put \(a=u_*<0\), \(d=8.407362\), \(I=[0,d]\), \(D_c=\{a\le u\le d,\ b(u)\le v\le c\}\),
and \(V=f(6/r^2-3/r^3)>0\). The return satisfies
\[
4w_{uv}+Vw=F_u+G_v-H,\qquad
w|_\Sigma=0,\qquad 4w_u|_\Sigma=G-b'F .
\]
The front at \(a\) is flat. The finite-cut trace and exterior hypotheses
are those of [GOAL-013](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md). Write
\(N=\lim_{v\to\infty}w_u\). The physical \(G(\cdot,c)\to0\) on the full strip.

## Energy estimate for the test

Take \(g\in H^1_0(I)\), extended by zero to \([a,d]\). The exact test satisfies
\[
4(Z_c)_{uv}+VZ_c=0,\qquad Z_c(\cdot,c)=g,\qquad Z_c(d,\cdot)=0 .
\]
The backward Volterra formula and current transposition of
[GOAL-014, (A4)–(A7)](../../GLR-GOAL-014/ADJOINT-AND-ENDPOINTS.md)
apply by density. Both endpoint current terms vanish. Graph contact terms
cancel by the canonical condition, with the induced velocities included.
The finite-cap identity is
\[
 4\langle g,w_u(\cdot,c)\rangle
 =\langle g,G(\cdot,c)\rangle
   -\int_{D_c}[(Z_c)_u F+(Z_c)_v G+Z_c H]\,du\,dv . \tag{1}
\]
Since \(V_u+V_v=0\), the local backward energy identity is
\[
 \partial_v\bigl(|Z_u|^2+V|Z|^2/4\bigr)
 +\partial_u\bigl(|Z_v|^2+V|Z|^2/4\bigr)=0 . \tag{2}
\]
Every rectangle \([u,d]\times[v,c]\) starting at \((u,v)\in D_c\) is in
\(D_c\) by monotonicity of the graph. Its flux on \(u=d\) is zero and its
flux on \(v=c\) is bounded by
\[
 A_c^2=\|g'\|_2^2+\int_I V(u,c)|g(u)|^2\,du/4 .
\]
Both other fluxes are nonnegative. At fixed \(v\) choose the left
endpoint on the graph or front to bound the entire \(u\)-section. At
fixed \(u\), choose \(v=b(u)\) to bound the entire \(v\)-section. Each
directional flux is bounded by \(A_c^2\). The bound on an outgoing
\(u\)-section can restrict the cap integral to \([u,d]\).
Equation (2) also follows by contracting the Killing current with the
null normals and reversing the roles of incoming and outgoing sides.

Let \(A=\|g'\|_2\). The cap potential term tends to zero. For sources with
the physical current ray convergence, use the cap-to-infinity limit of
the Volterra test, or apply the estimate first at finite cap and take
the limit. Cauchy–Schwarz on the sections gives
\[
 \mathscr E_g(F,G,H)=
 \frac{A}{4}\left[
 \|F\|_{L^1_vL^2_u}+\|G\|_{L^1_uL^2_v}
 +2\min\bigl(\|H/\sqrt V\|_{L^1_vL^2_u},
             \|H/\sqrt V\|_{L^1_uL^2_v}\bigr)\right]. \tag{3}
\]
This bounds the absolute volume pairing in (1), with its factor of
four divided out. Use (3) for differences of physical and reconstructed
currents. The integrands use zero extensions off the physical wedge.
A finite-domain candidate can use the infinite-cap adjoint and extend
its currents by zero beyond its numerical cap. The cap contact of this
extension is then included by distributional transposition. When
pairing with finite-cap physical news, (1) includes the explicit \(G\)
trace. The strong exterior transfer is a separate estimate for a
different choice of cap data.

The constant \(A\) avoids the exponential amplification of a uniform
absolute Volterra majorant. Shorter temporal tests cost more through
\(\|g'\|_2\). A weak moment estimate alone supplies a lower test on the
mismatch. Short oscillations can carry large \(L^2\) error with small
moments.

## Propagation of lower-field errors into the moment

At one lower stage let \(e\) be the exact minus conforming candidate
with positive characteristic energy bound \(M_e\).
Set \(D_u e=e_u-\mathsf A e/2,\ D_v e=e_v+\mathsf A e/2\), where
\(\mathsf A=0\) for the baseline. Its potential weight \(K\) is given
by the [GOAL-013 energy](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md).
Write the contribution of this field to the current errors as rows
acting on \(D_u e,D_v e,K^{1/2}e/2\). The last row includes the shift
from \(\mathsf A/2\) in the corrected derivatives. The remaining
coefficient-error times candidate terms use (3) separately.

For a scalar or row \(C(u,v)\), define
\[
 \Lambda_v(C)=\int\sup_u\|C(u,v)\|\,dv,\qquad
 \Lambda_u(C)=\int\sup_v\|C(u,v)\|\,du .
\]
Let \(X\) be one of \(Z_u,Z_v,\sqrt V Z/2\) and \(Y\) one of
\(D_u e,D_v e,K^{1/2}e/2\). The two potential factors can use either
direction. If \(X,Y\) have flux estimates on the same sections,
Cauchy–Schwarz on each section gives
\[
 \left|\int X\,C Y\right|\le A M_e \Lambda_j(C),
\]
where \(j=v\) for \(u\)-section control and \(j=u\) for \(v\)-section
control. For opposite directions use weighted Cauchy–Schwarz over the
wedge with weight \(\|C\|\). This gives
\[
 \left|\int X\,C Y\right|\le
 A M_e \sqrt{\Lambda_v(C)\Lambda_u(C)} . \tag{4}
\]
For the \(H\) pairing include \(2/\sqrt V\) in \(C\). These bounds use
uncommuted lower energy and integrable coefficient rows. The returning
graph jet is built into (1). The mixed stage still needs a physical
source and trace error, including the baseline derivatives entering its
forcing, directed coefficient errors, and a conforming residual.
A numerical adjoint and volume quadrature introduce separate errors.

## Simultaneous rejection and matched-signal witnesses

If \(\|g\|_2=1\) and \(\langle g,N\rangle\in[p-E_g,p+E_g]\), then
\[
 \|N-P\|_2\ge
 \max(0,|p-\langle g,P_{\rm mid}\rangle|-E_g-\Delta_{g,P}),
 \qquad
 \Delta_{g,P}\ge \sup|\langle g,P-P_{\rm mid}\rangle| . \tag{5}
\]
The right-hand side at least \(U_P/2\) rejects the half-tolerance test.
One can choose \(g\) orthogonal to the four midpoint damped sine and
cosine functions. This removes both midpoint pairings and all
fixed-exponent residue uncertainty. The remaining exponent uncertainty
is bounded by the integral of \(u e^{(\Re s+|\delta s|)u}|g|\) times
the amplitude and frequency errors, including the prefactor \(s\).
An \(H^1_0\) test orthogonal to the leading news and prescribed nuisance
directions gives a matched-signal lower bound subject to those trace
and nuisance-model errors. Certifying a fit requires an upper bound on
the complete discrepancy.

With orthonormal tests, Bessel's inequality bounds the squared mismatch
below by the sum of squared distances to the moment intervals. Joint
optimization preserves one common template parameter vector in all
moments. Separate infima are conservative.

A floating-point candidate can help choose a test with a large apparent
margin. Freeze the test and candidate representation before directing
the adjoint, quadrature and moment errors. A decision requires the
physical lower-field and coefficient bounds in (3)–(4).

## Pullback through the mixed stage without commuted baseline data

For a smooth test with \(g(d)=g'(d)=0\), one can remove the forward mixed
solve from the formula for a weak moment. Use the infinite-cap test \(Z\)
in (1). Taper the *mixed current rows* by a smooth radial cutoff in \(v\)
which is one in the inner region and flat zero at a finite cap \(c\).
The difference from the physical moment is bounded by (3) on the removed
exterior rows. This is a weak-current truncation with a separate error.
The local and fixed-mass pieces can be paired directly with the baseline.

Write \(Y=(U_m/\sqrt D,\operatorname{Im}(\sqrt{W_m}A_z))^T\), where
\(D=r^2(\lambda+k^2r^2)\), \(W_m=\lambda r^4/(\lambda+k^2r^2)\),
\(L_c=r^4/D\), \(a_m=(\log\sqrt D)_x\), and
\(b_m=(\log\sqrt{W_m})_x\). Keep the real \(J^t,J^x,X\) with their
actual \(+k\) factors. The [two mixed rows](../../GLR-GOAL-013/SOURCE-LEDGER.md#massive-reconstruction-and-graph)
have the exact form
\[
 L_mY=\partial_t B^t+\partial_x B^x+R_m,\qquad
 B^t=\begin{pmatrix}J^x/\sqrt D\\-kL_cJ^t/\sqrt{W_m}\end{pmatrix},\quad
 B^x=\begin{pmatrix}J^t/\sqrt D\\-kL_cJ^x/\sqrt{W_m}\end{pmatrix},
\]
\[
 R_m=\begin{pmatrix}
        -a_m J^t/\sqrt D+ f\sqrt D\,X/r^4\\
        -k b_m L_cJ^x/\sqrt{W_m}
      \end{pmatrix}.
 \tag{6}
\]
These sources depend on the baseline shear and its first derivatives.
The compact source component \(J^z=0\) here follows from the pinned
compact-unitary profile. The canonical launch satisfies
\(Y|_\Sigma=0\) and \(B^t-pB^x=-(1-p^2)Y_t|_\Sigma\).
For the second row use \(L_c/\sqrt{W_m}=\sqrt{W_m}/(\lambda r^2)\).
With \(\Theta=\mathbf1_{t\ge T(x)}\) the contact of \(L_m(\Theta Y)\)
is \(-(1-p^2)Y_t\delta(t-T)\). It is exactly the contact of
\(\partial_t(\Theta B^t)+\partial_x(\Theta B^x)\). The step through
the skew first time derivative contributes no additional delta
because \(Y|_\Sigma=0\). Hence
\[
 L_m(\Theta Y)=\partial_t(\Theta B^t)+\partial_x(\Theta B^x)+\Theta R_m .
 \tag{7}
\]

The tapered, mixed-field-dependent part of the right side of (1) divided
by four has the form
\(\int (C_t\partial_tY+C_x\partial_xY+C_0Y)\,dt\,dx\).
Each row is determined by \(Z_u,Z_v,Z\), the real GL geometry and the
algebraic return reconstruction. Include the Jacobian
\(du\,dv=2\,dt\,dx\) in the rows. Keep all affine \(J,X\) terms of that
reconstruction as a separate baseline part. The graph and front values
of \(Y\) vanish, the rows vanish at the tapered cap, and at \(u=d\)
all three \(Z,Z_u,Z_v\) vanish from the test conditions. Integration
by parts therefore gives a volume functional \(\int T_m^T Y\,dt\,dx\)
with \(T_m=(C_0-\partial_t C_t-\partial_x C_x)^T\).
This integration needs enough test and coefficient regularity to
interpret \(T_m\) in the chosen adjoint energy class. Piecewise lifts
with unmatched rows must include their seam distributions.

The stationary real mixed operator is formally self-adjoint for the
bilinear spacetime pairing. Its potential is symmetric and
\((\mathsf C\partial_t)^*=\mathsf C\partial_t\) because \(\mathsf C\)
is skew and independent of \(t\). Let \(\Phi\) be the backward mixed
solution \(L_m\Phi=T_m\) with zero data on the future characteristic
faces \(u=d\) and \(v=c\). It requires no future condition on the
physical launch graph. Pairing (7) against \(\Phi\) gives the exact
tapered mixed moment
\[
 \int_{D_c}\left[-\Phi_t^T B^t-\Phi_x^T B^x+\Phi^T R_m\right]\,dt\,dx .
 \tag{8}
\]
One can also obtain (8) by Green's identity inside the wedge. The
wave boundary term on the graph is the forced normal derivative of
\(Y\), which cancels the boundary term of the mixed current divergence.
On the two future faces the adjoint trace and its tangential derivative
vanish. The exact flat front removes its boundary term.

Equation (8) transfers all derivatives of the mixed forcing and all
forward mixed-field approximation errors to a backward mixed problem
with known coefficient and test forcing. The directed inputs for this
route are the uncommuted baseline first-jet error, geometry rows and
their needed derivatives, the scalar and mixed adjoint residuals and
quadrature, and a taper-tail estimate. Its backward source contains
derivatives of the scalar test, so an energy estimate for \(Z\) alone
does not enclose it. No mixed adjoint residual for the physical
benchmark has been evaluated. The estimator is a separate way to
target a signed rejection with a finite error budget.

## Directed signed witness thresholds on the selected window

For each template take the unit test
\[
 g_P=\frac{\sin(\pi u/d)P_{\rm mid}}
 {\|\sin(\pi u/d)P_{\rm mid}\|_2}
\]
inside \(I\), zero outside. If a directed upper bound for
\(\langle g_P,N\rangle\) is at most
\(\inf_P(\langle g_P,P\rangle-\|P\|_2/2)\), Cauchy–Schwarz rejects
the half-tolerance test. For fixed exponents the objective is concave
in the residue coordinates, so its infimum over the rectangle is at a
corner. For exponent-only variation, an \(L^2\) perturbation \(e\) of
the template changes the objective by at most \(3e/2\). The real
exponential integrations with the sine and sine-square weights and the
disk perturbations are directed in [witness_boxes.py](witness_boxes.py).
They give sufficient upper bounds \(0.1702965\) for \(Q\) and
\(0.5102615\) for \(Q+S_*\).

For the clustered candidate, the respective empirical continued
moments are about \(+0.00068053\) and \(-0.02790238\). A separate
backward diamond discretization with the sampled currents and a primal
cap quadrature agree to about \(5.3\times10^{-5}\) and
\(3.9\times10^{-5}\), respectively. Both cap checks use shared
currents. The difference between cap and infinity diagnostics includes
the exterior propagation of the cap shear. The moment-error allocations
suggested by these ordinates are about \(0.1696\) and \(0.5381\).
A physical decision requires directed lower-field and coefficient errors
and an adjoint and quadrature certificate. These particular sine
tapers use (1)–(5); the integration (8) needs an additional vanishing
right-endpoint derivative or the resulting terminal mixed boundary term.
[Diagnostic source](weak_candidate.py) · [Output](weak_candidate.txt).
