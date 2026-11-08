# E. Independent characteristic and preparation checks

This initial check used the ordered perturbative problem and the canonical data at
[the GOAL-012 source pin](https://github.com/inaeyk/GL-with-AI/tree/87fd711bf2e8117467caad1ff52c1cd344966974/research/sav/GLR-GOAL-011)
and the inherited
[Work review](https://github.com/inaeyk/GL-with-AI/tree/ae8c323e1e87abd0fff26eac75b05b701a7e7b1a/research/sav/verification/GLR-GOAL-012).
The fork exposed the older flux and rank formulas, including the prior late-packet argument, so those checks are nonblind rederivations from the current and canonical variables. The null-seam criterion, scalar ray estimate and explicit kink test below were separately derived before receipt of the GOAL-013 integrated argument.

## Energy flux and interface requirements

Use \(Lw=w_{tt}+D_x^*D_xw+Kw+B w_t=F\), \(D_x=\partial_x+\mathsf A(x)\),
with stationary \(\mathsf A^\dagger=\mathsf A\), \(B^\dagger=-B\) and \(K=K^\dagger\ge0\). The pinned mixed spatial factor is \(D_x^*D_x=-\partial_x^2+\mathsf A^2-\mathsf A_x\). The local identity
\(\partial_x(w_t^\dagger D_xw)=(D_xw_t)^\dagger D_xw-w_t^\dagger D_x^*D_xw\) yields
\[
 \partial_t \mathcal E+\partial_x\mathcal J=\Re(w_t^\dagger F),
 \qquad \mathcal E=\tfrac12(|w_t|^2+|D_xw|^2+w^\dagger K w),
 \quad \mathcal J=-\Re(w_t^\dagger D_xw).
\]
The skew term has zero real work. In \((u,v)=(t-x,t+x)\), the outward \(v=\text{constant}\) flux density in \(du\) is
\[
 \tfrac12(\mathcal E+\mathcal J)=|w_u-\mathsf A w/2|^2+w^\dagger K w/4.
\]
At the outgoing \(u=\text{constant}\) edge it is
\(\tfrac12(\mathcal E-\mathcal J)=|w_v+\mathsf A w/2|^2+w^\dagger K w/4\) in \(dv\).
The graph incoming flux is
\[
 \mathcal E-p\mathcal J
 =\tfrac12\big(|w_t+pD_xw|^2+(1-p^2)|D_xw|^2+w^\dagger K w\big),
 \qquad p=T'(x).
\]
Thus the slope degeneracy near the horizon is real, although a finite last cut intersects this graph at \(r_\Sigma(U)>1\). The causally relevant finite domain has \(r\ge r_\Sigma(U)\) since \(u_\Sigma(r)\) strictly decreases and \(x=(v-u)/2\) increases along fixed \(u\). For this characteristic construction the horizon end of the exterior is absent from the causal domain. This observation does not enclose a frequency-domain horizon matching error.

The outgoing flux through an internal null seam depends only on the field and its tangential derivative. A continuous piecewise \(C^2\) field with differentiable common trace has equal conormal energy flux across a null seam, even when its transverse derivative jumps. Equivalently \(\partial_u\partial_v\) creates no seam measure because the mixed derivative differentiates a shared tangential trace transversely. A conforming null-grid residual can consequently use ordinary whole-cell bounds. Time commutations require the same property for each commuted field. Continuity of the original field alone does not imply continuity of \(\partial_t w\). A general non-null interface requires an explicit flux, a lifting to a suitable conforming field, or a different estimate.

For a smooth or conforming error, \(|w_t|\le|w_u-\mathsf A w/2|+|w_v+\mathsf A w/2|\). Let \(M\) bound the square roots of the two directional fluxes on all truncated subdiamonds with the same graph and initial null edge. Integration, \(dt\,dx=du\,dv/2\) and Cauchy–Schwarz at fixed \(v\) and fixed \(u\) give
\[
 M^2\le E_{\rm in}+
 \tfrac12(\|F\|_{L_v^1L_u^2}+\|F\|_{L_u^1L_v^2})M.
\]
Here \(E_{\rm in}\) includes all incoming edge flux and the graph contribution. This is a volume estimate for functions, with no extension to a general surface delta in those mixed norms.

## Scalar ray bound for a crossed seam

For a scalar error on the same domain put \(P=w_u\), \(a=u_*\) and \(J(u)=\int_{v_\Sigma(u)}^\infty V(u,v)\,dv/4\). At fixed \(u\) integrate
\(P_v=(F-Vw)/4\) through cells and add each actual jump \([P]\) at a crossed interface. At fixed \(v\), the left endpoint is the early edge or the graph, and
\(w(u,v)=w_{\rm in}(u_0(v),v)+\int_{u_0(v)}^u P(s,v)ds\) for a continuous reconstruction. For a discontinuous reconstruction, the corresponding sum of field jumps must be included too. For the continuous case set
\[
 \begin{split}
 G(u)&=|P_\Sigma(u)|+\tfrac14\int |F(u,v)|dv
       +\sum_{\text{crossings at }u}|[P]|+J(u)H(u),\\
 M(u)&=\sup_v|P(u,v)|,
 \end{split}
\]
where \(H(u)\) bounds the incoming scalar value from every left endpoint with coordinate at most \(u\). Then \(M(u)\le G(u)+J(u)\int_a^u M(s)ds\). With \(j=\sup J(u)\) and \(D=U-a\), a Volterra series and the \(L^1\) norm of its convolution kernel give
\[
 \|M\|_{L^2([a,U])}\le e^{jD}\|G\|_2.
\]
The bound applies when a.e. ray has finitely many crossings and the stated \(G\) is in \(L^2\). A seam parallel to a fixed-\(u\) ray can be exceptional on a measure-zero set of \(u\). For \(\ell=2\) RW and \(r_0=1\), \(J=3/r_\Sigma-3/(4r_\Sigma^2)\le9/4\). The full-window growth bound is coarse. Its role here is an independent check on the admissibility of a seam residual and the incoming terms.

A minimal adverse example is flat, with \(T=0\), \(V=0\), zero initial value and velocity and candidate \(w=(t-|x|)_+^2\). The classical residual is zero in every open cell. At \(x=0,\ t>0\), the spatial derivative jump is \(-4t\) and the true residual is \(4t\delta(x)\). At fixed \(u>0\), \(w=v^2\) for \(v<u\) and \(w=u^2\) for \(v>u\). Thus the ray crosses a jump of \(2u\) in \(P\) and the cap norm through \(u=D\) is \(2D^{3/2}/\sqrt3\), while the exact homogeneous solution is zero. A computation that omits this interface would assign a zero error.

## Graph contact from zero extension

For the zero extension by \(\Theta=\mathbf 1_{t\ge T(x)}\), let \(h=w|_\Sigma\), \(g=w_t|_\Sigma\) and use the same \(L\). Direct distribution differentiation gives
\[
 L(\Theta w)=\Theta Lw +(1-p^2)h\,\delta'(t-T)
  +\big((1-p^2)g+2p\,h'+p'h+B h\big)\delta(t-T).
\]
Here \(h'\) is tangential and \(\delta'\) differentiates in \(t\); multiplying a field through \(\delta'\) subtracts its time derivative at the graph. In particular a vanishing independent upper position with a nonzero induced \(g\) still contributes \((1-p^2)g\,\delta\). If a strong source has the divergence \(\partial_t q+\partial_x b\), writing it as \(\partial_t(\Theta q)+\partial_x(\Theta b)\) adds \((q-p b)|_\Sigma\delta\) and requires the compensating subtraction in the weak source. Separate surface contributions at finite artificial boundaries must follow the same integration by parts.

## Why both residues fail to determine the ordered waveform

The pinned exact-curl canonical change in the mean axial sector reads \(\Delta\pi^x=\lambda r\,w/2\) and \(\Delta(A_x+pA_t)=D_\Sigma w/(2r)+pfw/(2r^2)\) with \(D_\Sigma=\partial_t+p\partial_x\). The nonzero diagonal factors and the spacelike graph give two independently variable smooth compact jets \(w|_\Sigma,D_\Sigma w|_\Sigma\). Such axial changes preserve the spherical charges to the ordered degree of interest.

For an early compact change use \(w|_\Sigma=A(u)\), \(w_v|_\Sigma=0\), \(A(u)=\int_a^u q\), and complete the displacement by a smooth taper after the last cut. These data give \(w_u|_\Sigma=q\). On the strip through \(d\), the homogeneous integral solution obeys
\[
 \sup_v|w(u,v)|\le e^{j(d-a)}\int_a^u |q|,\qquad
 |w_u(u,\infty)-q(u)|\le j e^{j(d-a)}\int_a^u |q|.
\]
This follows by integrating the homogeneous equation in \(v\) and then in \(u\), where the left intersection with the graph contributes \(A(u_0)\). The map \(q\mapsto w_u(\cdot,\infty)\) is identity plus a causal linear operator with Volterra factorial bounds. It has a bounded inverse on continuous profiles. Smooth compact data yield a dense image on the interval by smooth approximation of \(q\) vanishing near the first cut (or in \(L^2\), without an endpoint compatibility issue). Thus the early homogeneous addition can have nonzero overlap against any nonzero baseline on the strip.

A second packet placed on the graph strictly after \(d\) affects no observed cut. To show its ordinary residue has real rank two, use the Green graph current tested against \(z=e^{-s_qt}h_q(x)\). Its weights for the two independent jets are \(z|_\Sigma\) and \(-D_\Sigma z|_\Sigma\), up to the same nonzero denominator. If the range were confined to a real complex line, both weights would lie in that line throughout the late support interval. The tangential derivative \((\partial_x+p\partial_t)z\) of the first weight lies there as well. Subtract \(p\) times this derivative from \(D_\Sigma z\). This places \((1-p^2)\partial_t z=-(1-p^2)s_q z\) in the same line, which is impossible where \(z\ne0\) since \(\Im s_q\ne0\). A nontrivial homogeneous radial mode cannot vanish identically on an open interval. The late packet can therefore restore the ordinary residue after arbitrary real scaling of the early packet. Both homogeneous additions have compact graph source and an entire Cauchy numerator, and the massless inverse is regular at \(s_*\) by the inherited local certificate. They leave the translated residue unchanged. This permits either overlap sign while holding both residues fixed across the broader class. The prescribed zero independent upper canonical data exclude precisely these changes. The argument gives no sign or finite-window dominance decision for the fixed preparation.

## Sharper inhomogeneous cap stability

With \(W=\mathcal I P\) and \(W(u_*)=0\), the cap equation \(P_v=-(V W+S)/4\) admits an operator bound that uses the sign of the RW potential. At each \(v\),
\[
 \Re\langle P,VW\rangle
 =\tfrac12V(U,v)|W(U,v)|^2-\tfrac12\int_{u_*}^U V_u|W|^2\,du.
\]
Dropping the nonnegative endpoint in this identity gives
\[
 \frac{d}{dv}\|P\|_2
 \le \tfrac18\|\mathcal I\|_{2\to2}^2\|(V_u)_+\|_\infty\|P\|_2
       +\tfrac14\|S\|_2 .
\]
The same bound holds for a difference of solutions with the source replaced by its defect. It extends through zero norm by regularization. Here \(V=6/r^2-9/r^3+3/r^4\) and \(V_u=-f V_r/2\le6 f/r^3\). For \(r_c\ge4/3\), \(f/r^3\) is decreasing in \(r\). At fixed \(v\) the minimum radius on the cap strip is at \(u=U\), and there \(dv=2dr/f\). Thus the exponent is bounded by
\[
 \Lambda=\tfrac18\|\mathcal I\|_{2\to2}^2
 \int_{V_c}^{\infty}\|(V_u)_+\|_\infty\,dv
 \le \frac{3(U-u_*)^2}{\pi^2r_c^2}.
\]
This argument improves the propagated cap input and source errors to amplification \(e^\Lambda\), without replacing the stronger conditions required for the compact-domain residual.

For a degree-\(m\) homogeneous ordered iterate \(H_m(v)\) from \(P_c\), let \(H_m^{[m]}\) denote its top-degree term. The exact defect is \((V\mathcal I/4)H_m^{[m]}\), so its \(L^1_vL^2_u\) norm is bounded by \(\|P_c\|_2 K^{m+1}/(m+1)!\), where \(K=\int\|V\mathcal I/4\|\,dv\) or any upper bound. This estimate uses the simplex integral of the ordered product and has no extra absolute time-frequency fit. Consequently the degree-three homogeneous transfer at \(r_c=100\) and \(U=8.407362\) has error \(<3.761\times10^{-5}\|P_c\|_2\) and the source and input amplification is \(<1.00252\). Use \(\|P_c^{\rm candidate}\|_2\) instead when constructing the iterate from a numerical cap and including a separate cap input error. Quadrature and arithmetic errors still need their own bounds.

[Exact control source](../checks/e_adversarial.py) · [Archived output](../checks/e_adversarial.txt). The rational cap calculation uses \(D<9.092355\), \(K<0.173218\), \(\pi>3.14159\) and \(e^\Lambda\le(1-\Lambda)^{-1}\). The SymPy checks exercise a graph-only retarded solution with nonzero contact, an exact two-field skew system with a nonconstant spatial connection, and the adverse kink above. These are manufactured exact controls and do not enclose a physical compact-domain residual, a GL/KK reconstruction or \(B_c\).

For physical far-jet estimates the pinned massive spatial factor is Hermitian and its one-dimensional transport need not be unitary. The coercive potential is sufficient. If \(M_j\) bounds the characteristic energy flux of \(\partial_t^j w_m\), then on a fixed exterior \(v\)-slice, \(K\ge f(r_c)k^2=:K_c^{\min}>0\) gives
\[
 \|\partial_t^j w_m\|_2\le 2M_j/\sqrt{K_c^{\min}},\qquad
 \|D_x\partial_t^j w_m\|_2
 \le 2M_j+2M_{j+1}/\sqrt{K_c^{\min}} .
\]
The second inequality follows from \(D_xw=w_t-2(w_u-\mathsf A w/2)\). Replace the second \(x\)-derivative by the hyperbolic equation, including the full forcing, if a strong source requires it. For the scalar baseline the zero causal front and the primitive bound \(2D/\pi\) replace coercivity at large radius. These estimates allow one to bound strong exterior source terms by actual physical coefficient envelopes and commuted energies without a separate exterior mixed transfer. The envelopes and commuted energies remain numerical inputs.

## Leading-news test weights for a weak interference estimate

The \(L^2\) error for a numerical leading-news approximation alone does not bound its derivative or endpoint error. The exact leading test does have simple analytic \(H^1\) bounds. Use \(y=(u-u_*)/\delta\), \(W=w_0/\sqrt\delta\), \(g=\sqrt\delta N_{01}\), and \(M=1.61\) as in [the leading certificate](../FLUX.md). On the unit \(y\) strip let \(X\) be the \(L^2(dy)\) norm of the supremum in \(v\). The baseline Volterra equation and \(\|b'\|_2=1\) give
\[
 \|W\|_X\le e^{\delta M}/\pi,\qquad
 \|W_y\|_X\le 1+\delta M\|W\|_X .
\]
Differentiate \(g=b'-\delta A_yW\), including the derivative of the lower graph limit. The packet bounds \(|D_y|\le\delta D/4\), \(D=V/4\le2/9\), and \(|dv_\Sigma/du|\le5/21\) then give
\[
 \|g'-b''\|_2
 \le\delta\left[\tfrac{\delta M}{4}\|W\|_X+
  \tfrac{\delta(5/21)(2/9)}{\pi}+M(1+\delta M\|W\|_X)\right]<0.211 .
\]
The flat launch gives \(g(0)=0\). At the far end of the packet \(b(1)=b'(1)=0\), so the Volterra difference \(W-b\) gives \(|g(1)|\le\delta^2 M^2 e^{\delta M}/\pi<0.0145\). These bounds apply to the exact test, not an error in its \(H^1\) approximation. Using an approximate test in a weak interference formula requires corresponding \(H^1\) and endpoint errors or a separate bound on \(N_{21}\) paired with the \(L^2\) error. The physical source-current errors are also needed.
