# Finite-cut bounds

Use dimensionless \(r_0=1\), \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\). Write \(a=u_*\), \(b(u)=r_\Sigma(u)\), \(p=T'(x)=-1/r\), and \(\mathcal D=\{a\le u\le d,\ b(u)\le v\}\). The final cut \(d=8.407362\) is exact as a decimal and the packet front is \(a=-13/8-2\log(5/8)\).

## Causal domain and strong error propagation

The scalar and mixed principal parts have speed one in \(t,x\). Axial data contributing to the cuts lie at \(r\ge b(d)=1+2W_0(e^{-(1+d)/2}/2)\). The graph and the front \(u=a\) are the incoming boundaries. At \(u=a\) the exact axial fields and all jets vanish by flatness and causal support. The cap \(v=V_c\) and \(u=d\) are outgoing. The excluded axial horizon collar has zero effect. The fixed-mass mean still requires its outer constraint integral for each radius.

For \(e\) satisfying
\[
 -e_{tt}+e_{xx}-(K+\mathsf A^2-\mathsf A_x)e+\mathsf C e_t=R,
\]
with Hermitian stationary \(\mathsf A\), \(K\ge0\) stationary, and skew-Hermitian \(\mathsf C\), let \(D_x=\partial_x+\mathsf A\) and set
\[
 E=(|e_t|^2+|D_xe|^2+e^\dagger Ke)/2,\qquad J=-\Re(e_t^\dagger D_xe).
\]
The factorization \(D_x^\dagger D_x=-\partial_x^2+\mathsf A^2-\mathsf A_x\) gives \(E_t+J_x=-\Re(e_t^\dagger R)\). The graph integrand is \(E-pJ\). The null integrands on \(v=\mathrm{const}\) and \(u=\mathrm{const}\) are \(|e_u-\mathsf A e/2|^2+e^\dagger Ke/4\) and \(|e_v+\mathsf A e/2|^2+e^\dagger Ke/4\), respectively. Let \(M\) be the supremum of the square root of either outgoing flux over subdiamonds ending at arbitrary smaller \(u,v\). If \(E_{\rm in}\) includes the incoming graph and any front error flux, the two corrected null derivatives add to \(e_t\) and \(dt\,dx=du\,dv/2\). Hence
\[
 M^2\le E_{\rm in}+B M,\qquad
 B=\tfrac12(\|R\|_{L^1_vL^2_u}+\|R\|_{L^1_uL^2_v}),\qquad
 M\le \tfrac12(B+\sqrt{B^2+4E_{\rm in}}).
\]
The scalar flux directly bounds finite-cap news error. Use whole-cell envelopes for the two residual norms. A continuous reconstruction across an aligned null seam has a common tangential derivative and field, hence a common conormal flux there. For a general seam include the signed jump of the conormal flux from integrating the two cell currents separately, or lift the reconstruction to one with matching traces.

With \(W=K+\mathsf A^2-\mathsf A_x\), the first commuted graph jet follows from \(q=e|_\Sigma,\ g=e_t|_\Sigma,\ h=q'-p g=e_x|_\Sigma\), where prime differentiates graph traces in \(x\):
\[
 e_{tt}|_\Sigma=\frac{h'-p g'-Wq+\mathsf C g-R|_\Sigma}{1-p^2}.
\]
Here \(g'=e_{tx}+p e_{tt}\), \(h'=e_{xx}+p e_{tx}\). A bound for \(\partial_t e\) needs this jet and \(\partial_t R\), or their weak interface corrections. Continuity of the uncommuted candidate alone can leave a jump in its time derivative.

## Derivative-reduced interference functional

For the returning scalar \(w=w_2\), use \(m=2r/\lambda\), \(P=r^{-2}\mathcal D^x\), \(Q=r^{-2}\mathcal D^t\), and \(c=\lambda f r^{-4}\mathcal C\), with tortoise densities. Define
\[
 F=m(P-Q),\qquad G=m(P+Q),\qquad H=m_xQ-mc.
\]
Then \(L_{\rm RW}w=-F_u-G_v+H\). The canonical upper graph has \(w|_\Sigma=0\) and
\[
 4w_u|_\Sigma=G|_\Sigma-b'F|_\Sigma,\qquad b'=-\frac{1+p}{1-p}.
\]
This follows from \((1-p^2)w_t=m(P-pQ)\) and the zero graph trace. Integrating the strong equation once and differentiating the moving lower limit gives
\[
 4w_u(u,v)=G(u,v)+\partial_u\mathcal F(u,v)
 -\int_{b(u)}^v(H+Vw)(u,s)\,ds,\qquad
 \mathcal F(u,v)=\int_{b(u)}^v F(u,s)\,ds. \tag{1}
\]
Setting the upper velocities to zero with the same currents would add a spurious free-ray term to (1).

Let \(\ell(v)=a\) for \(v\ge b(a)\) and \(\ell(v)=b^{-1}(v)\) for \(b(d)\le v<b(a)\). A second integration gives \(w=J_s-\mathcal T w\), where
\[
 4J_s(u,v)=\mathcal F(u,v)+\int_{\ell(v)}^u
 \left(G(z,v)-\int_{b(z)}^v H(z,s)\,ds\right)dz,\qquad
 (\mathcal T w)(u,v)=\tfrac14\int_{\ell(v)}^u\int_{b(z)}^v V(z,s)w(z,s)\,ds\,dz . \tag{2}
\]
The integration constant vanishes at the graph or the front. Equation (2) uses reconstructed currents with first derivatives of the lower masters and no derivatives of those currents. It treats piecewise currents by weak integration. A calculation with cellwise strong derivatives instead needs their interface terms.

The exact ray mass is \(J_V(u)=\int_{b(u)}^\infty V\,dv/4=3/b(u)-3/(4b(u)^2)\). Let \(D=d-a\), \(M_V=J_V(d)\), and \(\|z\|_X=\|\sup_{v\ge b(u)}|z|\|_{L^2(du)}\). Zero-extend the ray integrals to \(u\in[a,d]\). Then \(\sup_v|\mathcal T z|\le M_V\mathcal I(\sup_v|z|)\), where \(\mathcal I\) integrates from \(a\). Young's inequality for the \(j\)th primitive gives \(\|\mathcal I^j\|_{2\to2}\le D^j/j!\), and \(\|\mathcal I\|=2D/\pi\). For candidate currents and shear, let
\[
 A_F(u)=\int_{b(u)}^\infty|F-F_h|\,dv,\quad
 A_H(u)=\int_{b(u)}^\infty|H-H_h|\,dv,\quad
 A_G(u)=\sup_{v\ge b(u)}|G-G_h|,\quad r_h=w_h-J_{s,h}+\mathcal T w_h .
\]
These norms include the true tails if the candidates extend currents by zero beyond a finite cap. The factorial resolvent yields
\[
 \|w-w_h\|_X\le C_D\left(\|r_h\|_X+
 \tfrac14[\|A_F\|_2+(2D/\pi)(\|A_G\|_2+\|A_H\|_2)]\right),\quad
 C_D=e^{M_V D}-(1-2/\pi)M_V D . \tag{3}
\]
The convergent Neumann series constructs the unique causal shear in \(X\) under these current norms.

Assume the exact news trace exists, \(G\) has its \(L^2\) limit, and the current ray envelopes control convergence including endpoint rays. For a real \(H^1\) test \(g\), integration by parts in (1) gives
\[
 4\langle g,N_{21}\rangle=
 [g\mathcal F_\infty]_a^d-\langle g',\mathcal F_\infty\rangle+
 \left\langle g,G_\infty-\int_b^\infty(H+Vw)\,dv\right\rangle. \tag{4}
\]
The same expression defines a weak pairing by passage from finite caps under the stated convergence. The candidate version of (4), after division by four, has absolute error at most
\[
 \frac{|g(d)|A_F(d)+|g(a)|A_F(a)+\|g'\|_2\|A_F\|_2+
 \|g\|_2(\|G_\infty-G_{h,\infty}\|_2+\|A_H\|_2)}4
 +M_V\|g\|_2\|w-w_h\|_X . \tag{5}
\]
At the flat front the endpoint trace vanishes. The right endpoint generally remains, even at the end of the compact launch support, because the leading news has scattered. Equations (3)–(5) allow a sign test without an \(L^2\) enclosure for \(N_{21}\). Directed evaluation gives \(C_D<1.144\) for the full packet and \(C_D<1.016\) on its first eighth. The physical current errors and shear defect in (5) still need evaluation.

## Null cap and positive-line route

For the strong scalar trace, let \(P_c=w_u(\cdot,V_c)\) and \(A(v)P=-V(\cdot,v)\mathcal I P/4\). The cap equation is \(P_v=A(v)P-S_2/4\). The cap operator norm integral \(K_c=\int_{V_c}^\infty\|A(v)\|\,dv\) and its ordered series bound the homogeneous truncation. The growth bound can use the signed operator part independently of the truncation bound. Integrating by parts with \(Z=\mathcal I P\) gives
\[
 \Re\langle P,VZ\rangle=[V|Z|^2]_a^d/2-\tfrac12\int V_u|Z|^2du.
\]
Consequently the transfer amplifies \(L^2\) initial and integrated forcing errors by at most \(\exp(3D^2/(\pi^2r_c^2))\) for \(r_c\ge100\). The estimate uses \(V_u\le 6f/r^3\), decreasing at those radii, and \(dv=2\,dr/f\) on the last ray. [Ordered transfer computation](investigators/C-COMPUTATION.md) · [Independent energy bound](investigators/E-ADVERSARIAL.md).

For the positive-line route the reviewed \(|W(s)|\ge2|s|\) is an inverse denominator bound. It supplies neither the physical numerator over an entire frequency cell nor the high-band weighted source/contact derivative bound. This route remains unclosed. The finite-rectangle and primitive estimates require no deformation through quasinormal poles.

## Quantified physical exterior term

Using the pinned physical GL interval, absolute lower-field graph/flux bounds, and the fixed-mass mean integral, the full physical exterior term \(B_c=\|S_2\|_{L^1_vL^2_u(v\ge V_c)}/4\) has the bound in [B's proof](investigators/B-SOURCE.md). At \(r_c=100\) the evaluated upper bound is of order \(2.105\times10^{46}\), so that choice fails this source-tail budget. At \(r_c=400\) the upper bound is \(1.7\times10^{-6}\), and at \(r_c=500\) it is \(7\times10^{-24}\). These are source errors from truncating the exterior forcing at those caps. They include both massive polarizations, the affine mixed terms, the direct quadratic term and the fixed-mass mean. Their derivation uses absolute energy of the physical lower fields rather than numerical candidates for those fields.

For example, set \(r_c=400\), and let \(p_c\) be a candidate for the compact cap news with \(L^2\) error \(E_c\). Let \(n_{21}\) approximate the true-potential homogeneous cap series through degree three on input \(p_c\), with a separate \(L^2\) quadrature and arithmetic error \(E_q\). Omitting the exterior source gives the budget
\[
 \|N_{21}-n_{21}\|_2
 \le 1.000158 E_c+3.77\times10^{-8}\|p_c\|_2
            +1.701\times10^{-6}+E_q .
\]
The second term uses both time ordering in \(v\) and the primitive ordering in \(u\), with \(\mu=D(6/r_c-3/(2r_c^2))/2\) and remainder bounded by \(\mu^4/[(4!)^2(1-\mu/25)]\). The first and third terms use the signed cap stability. The compact error \(E_c\), the candidate \(p_c\) and the numerical evaluation \(E_q\) for that candidate were not supplied. The \(r_c=400\) source estimate does not justify using data at \(r_c=100\) in this budget.
