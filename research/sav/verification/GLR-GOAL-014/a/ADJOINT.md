# A. Causal adjoint, endpoint and cap audit

I worked from the published claims, fixed background, ordered density equations and prior Work audit. The reconstruction and manufactured checks are independent of the source scripts, with the same equations and non-blind exposure to their claimed results. The GL eigenvalue, global mode jet boxes, ordered five-dimensional reduction and absolute exterior source estimates are inherited inputs.

## Graph and sharp test

In dimensionless coordinates \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\), the graph \(v=r\) is \(t=T(x)=r-x\) with \(p=T'=-1/r\). Thus \(du_\Sigma/dx=p-1\), \(dv_\Sigma/dx=p+1\), and \(b'(u)=-(1+p)/(1-p)<0\). For the canonically prepared returning exact-curl master \(w=2Q_{2z}/(\lambda r)\), the tangential gauge-potential constraint \(A_x+pA_t=0\) and \(Q_{2z}|_\Sigma=0\) give
\[
 (1-p^2)Q_{2z,t}=\mathcal D^x-p\mathcal D^t,\qquad
 w_u|_\Sigma=\frac{\mathcal D^x-p\mathcal D^t}{\lambda r(1-p)}.
\]
The factor follows from \(w_x=-p w_t\), so \(2w_u=(1+p)w_t\). With \(F=2(\mathcal D^x-\mathcal D^t)/(\lambda r)\) and \(G=2(\mathcal D^x+\mathcal D^t)/(\lambda r)\), this is \(4w_u=G-b'F\).

In the interior \(4w_{uv}+Vw=F_u+G_v-H\). Extend by zero below the graph. Differentiation of the support function adds \(-b'F+G\) to the divergence on the graph, as a coefficient of \(\delta(v-b(u))\). The added wave contact is \(4w_u\delta(v-b(u))\). They coincide. The same reasoning at the flat front gives zero. The matched contact is the mechanism by which the weak form already accounts for the canonical launch. For a general seam \(\phi=0\), the current contact is \((\phi_u[F]+\phi_v[G])\delta(\phi)\). A continuous piecewise smooth scalar has wave contact \(4\phi_v[w_u]\delta(\phi)\) with the normal orientation fixed by \(\phi>0\). Agreement must be shown or the residual must include their difference. Derivatives of nonconforming lower fields can introduce measures in the reconstructed currents themselves.

Put \(\mathcal F(u,v)=\int_{b(u)}^vF(u,s)\,ds\). Integrating the distributional equation along \(v\) gives
\[
 4w_u=G+\partial_u\mathcal F-\int_{b(u)}^v(H+Vw)\,ds .
\]
The compact test is the Sobolev function on the closed interval, with explicit boundary traces. Equivalently, \((\mathbf 1_{[a,d]}g)'=\mathbf 1_{[a,d]}g'+g(a)\delta_a-g(d)\delta_d\). The terminal term \(+g(d)\mathcal F(d)/4\) results. The incoming contribution vanishes for this fixed preparation. Support of the seed and support of the scattered test differ.

## Transpose of propagation on nested future rectangles

For \(W=V/4\) and \(\mathcal I P(u)=\int_a^uP(z)\,dz\), the homogeneous scalar news equation above the cap is \(P_v=-W\mathcal I P\). Its forward propagator has adjoint acting backwards as \(\partial_v Z=\mathcal I^*(WZ)\) with \(\mathcal I^*f=\int_u^d f(z)\,dz\). This gives \(Z_{uv}=-WZ\) and \(Z(d,v)=g(d)\) exactly. The backward Volterra kernel is on nested rectangles since \(b\) decreases. On a finite cap \(v=c\), put \(h=Z(\cdot,c)\). Integrate
\[
 \partial_v(Zw_u)-\partial_u(wZ_v)
  = Z(w_{uv}+Ww)
\]
and then integrate the current derivatives against \(Z\). The wave and current graph fluxes match. The exact result, with the flat front, is
\[
 4\langle h,w_u(\cdot,c)\rangle
  = h(d)\mathcal F(d,c)+\langle h,G(\cdot,c)\rangle
       -\int_{D_c}(Z_uF+Z_vG+ZH).
\]
In the exterior, integration by parts against \(Z\) adds \(g(d)\int_c^\infty F(d,v)dv\), the analogous negative volume term, the term \(+\langle g,G_\infty\rangle\), and \(-\langle h,G(\cdot,c)\rangle\). The matching \(G_c\) terms cancel when the exterior uses the separate current estimates. \(G_\infty=0\) follows from the uniform fixed-\(u\) \(H^1_v\) tail control. The strong-source truncation uses \(-\int Z S_2/4\) and retains the compact \(G_c\) term. Applying the cap cancellation to this latter truncation changes the functional.

The independent symbolic fixture uses \(b=1-u,\ d=1,\ c=2,\ g=1+u\), \(Z=g+(u-1)e^{-v}\) and \(V=4e^{-v}/Z>0\). Let \(w=u^2(1-e^{-(v-b)})\), \(\chi=u^2(v-b)e^{-v}\), \(F=4w_v+\chi_v,\ G=-\chi_u,\ H=-Vw\). It has the correct graph, a nonzero exterior terminal ray and a nonzero \(G_c\). Exact direct and adjoint projections are both \(5/3\). Omitting the complete terminal term gives \(-1/3\). The spurious correction from retaining the exterior cap term alone is \((41e^2-7)e^{-4}/48\). This complements the source's finite-cap and seam fixtures by exercising the exterior signs and nontrivial cap term. See [check_adjoint.py](check_adjoint.py) and [adjoint-output.txt](adjoint-output.txt).

## Quantified transfer and residual requirements

At \(c=d+2x(r_c)\), \(r\ge r_c\), \(\int_c^\infty\sup_u W\,dv=3/r_c-3/(4r_c^2)\) for \(r_c\ge100\), using monotonicity of \(W\) in the far region. On the entire graph strip the same ray integral from \(b(u)\) is \(3/b(u)-3/(4b(u)^2)\). Its maximum \(M\) is at \(d\). Ordered integration in \(v\) gives the \(m_c^2/2\) remainder in the one-step exterior adjoint. Taking \(\partial_u\) removes the outer backward primitive and gives the derivative version without differentiating the input candidate. For the \(L^2\) homogeneous transfer, \(\Re\langle P,V\mathcal I P\rangle=[V|\mathcal I P|^2]/2-\int V_u|\mathcal I P|^2/2\). The right-endpoint flux is dissipative and \(V_u\le6f/r^3\), giving the signed exponential \(A_2=\exp(3D^2/(\pi^2r_c^2))\). This reproduces the inheritance from GOAL-013. The compact backward Volterra majorant is \(S_h(u)=|h(u)|+M\int_u^d e^{M(z-u)}|h(z)|\,dz\). It bounds \(|Z|\), gives \(|Z_u|\le|h'|+MS_h\), and gives \(\int_{b(u)}^c |Z_v|\,dv\le S_h-|h|\). These give the stated compact weights. For \(\Delta F\), the coefficient of its terminal ray is independently \(|g(d)|/4\), even if the bulk errors are integrated with different norms. The early \(F,H,G\) sup-ray weights \(0.206883890, 0.000202708,0.000004778\) use the full \(F\) weight including the endpoint. They should not be added to a second terminal bound unless the bulk \(F\) term is separately extracted.

For mixed lower errors set \(y=e_u\) at \(u=d\). The error equation is \(y_v-Cy/4=(C e_v-W_e e-R)/4\). Skew-Hermiticity implies a unitary homogeneous propagator. For signed integral of \(B_u e_u+B_v e_v+B_0e\), use the row \(\lambda'+\lambda C/4=-B_u,\ \lambda(c)=0\). Then
\[
 B_u e_u=-(\lambda y)' +\lambda(C e_v-W_e e-R)/4 .
\]
This establishes the lower-end graph term and the sign of the residual term. It is independently checked with a real two-component skew matrix and nonzero residual. Flux controls \(e_v+\mathsf A e/2\) and \(\sqrt K e/2\). The signed row needs \(\int|\lambda||R(d,v)|\,dv\), not merely volume residual norms. The central-coefficient sensitivity replay reproduced the source multipliers. It supplies no outward bound for the row or the physical residual. Directional volume flux and the signed terminal row together allow a weak certificate without differentiating return currents or requiring a full strong news trace error.

The threshold for an early proof is \(p_->E_{\rm other}+\sup |g(d)|\,E_{\rm term}/4\), where \(p_-\) is a directed compact **Bondi-test** candidate lower bound, \(E_{\rm term}\) is an outward signed terminal integral error, and \(E_{\rm other}\) covers bulk, test, dual, quadrature, coefficient and exterior errors without double counting. If \(p_-\ge7.80\), \(E_{\rm other}\le3.8\), and \(E_{\rm term}\le37\), the quoted endpoint coefficient gives a margin above \(0.18\). This is one allocation for a future validation and does not follow from the present pilot. Its cap projection uses a different empirical test from the exact transported Bondi test. Even reproducing the empirical terminal and net values leaves all three inputs to this proposed criterion open.

## Qualification

The analytic identities and majorants survive under the stipulated regularity and ordered inputs. The reported small weak exterior error is conditional on the separate physical current tails. The early projection is empirically positive while the physical sign is open. Candidate weak agreement with a discrete transpose is algebraic. Continuum certification still needs directed coefficients including the fixed-mass mean, conforming lower-field reconstruction and actual terminal-ray residual, with volume and graph errors and any seams included.
