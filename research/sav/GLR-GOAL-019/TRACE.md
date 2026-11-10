# Conditional truncation-uniform weak comparison

Suppose a common outgoing chart gives the exact and quadratic scaled length fields. For \(d=\psi_\epsilon-\epsilon^2\psi_2\) their difference is \(d_{uv}=a\,d+\partial_u A+\partial_v B+C\). Before taking the difference use \(A=\sqrt3 G^u/(4\rho)\), \(B=\sqrt3 G^v/(4\rho)\), \(C=\sqrt3[-\mathcal S+(\rho_u/\rho)G^u+(\rho_v/\rho)G^v]/(4\rho)\), with \(\rho\) the mean area radius. Include \(\epsilon^2(a_\epsilon-a_0)\psi_2\) in \(C\). On the matched graph \(v=b(u)\) let \(h\) be the free difference-data and front lift. Put \(L_\Sigma=b'A|_\Sigma-B|_\Sigma\), \(\mathcal A=\int_b^v A,\ \mathcal B=\int_{\ell(v)}^u B,\ \mathcal C=\int_b^v C\) where \(\ell\) is the graph or front endpoint. Integrating first in \(v\) and then \(u\) gives
\[
 d=h+\mathcal A+\mathcal B+\int_{\ell(v)}^u
 [L_\Sigma(z)+\mathcal C(z,v)+\int_{b(z)}^v a(z,s)d(z,s)\,ds]dz.
\]
The adapted Z data can give \(h=L_\Sigma=0\) when matched exactly. For \(\nu(u)=\int_b^\infty |a|dv\), Volterra gives the cap-independent estimate
\[
 \|d\|_\infty\le e^{\|\nu\|_1}
 (\|h\|_\infty+\|\mathcal A\|_\infty+\|\mathcal B\|_\infty+
 \|L_\Sigma\|_1+\|\sup_v|\mathcal C|\|_1).
\]
With convergence of primitives and dominated tails it bounds the limiting field without differentiating the fluctuation product in \(\partial_u A\). A bounded \(\|\partial_u\mathcal A(\cdot,v)\|_2\) plus the differentiated graph and other limiting terms gives \(H^1\) of the field by weak compactness. A small strong news error needs a small derivative estimate.

A sufficient tail route uses normalized first jets \(Z\) and undifferentiated oscillatory fields \(Z^{(0)}\) in regular charts. The GOAL-006 source structure gives \(|A|+|C_{\rm source}|\lesssim r^{-1}\|Z\|_{L^2_z}^2\), \(|B|\lesssim r^{-1}\|Z^{(0)}\|_{L^2_z}\|Z\|_{L^2_z}\). At quadratic order \(G_2=0\) by the pinned first jet. If exact first-jet dyadic fluxes are \(O(\epsilon^2(D/R)^\delta/R)\) on shells at \(D\) on rays launched at \(R\), \(0\le\delta<1\), the absolute \(A,C_{\rm source}\) ray integrals have geometric tails. With uniform scaled first jets \(O(\epsilon)\), unit-\(v\) Sobolev on the same flux controls the undifferentiated factor on the cap by \(O(|\epsilon|v^{\delta/2}R^{-(1+\delta)/2})\). For \(r\asymp v\) and bounded \(|du/dR|\), integrating \(R^{-(1+\delta)/2}\) from the inner launch to \(v\) gives \(\mathcal B=O(\epsilon^2 v^{-1/2}/(1-\delta))\). Compact first-jet convergence and these uniform tails suffice for \(o(\epsilon^2)\) source primitives. The potential product needs \(\|\int |(a_\epsilon-a_0)\psi_2|dv\|_{L^1_u}=o(1)\). These are sufficient exact exterior hypotheses. Propagation from damped Z remains open. [GOAL-006 divergence source and causal mean equation](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md#3-route-b-exact-scalar-equation-and-a-trace-estimate)

For the weak witness let \(T=u_B(u)\) be increasing bi-Lipschitz on padded cuts with \(0<m\le T'\le M\), \(T^{-1}(I)\) contained there and \(\delta_T=\|T-\mathrm{id}\|_\infty\). For real \(g\in C_c^1(I)\), \(\|g\|_2=1\), the Jacobian cancels in the news pairing. Integration by parts against the geometric length error gives
\[
 |\langle g,N_B\rangle-\epsilon^2\langle g,n_Z\rangle|
 \le\sqrt M\|g'\|_2\|d_\infty\|_{L^2(\mathrm{padded})}
 +\epsilon^2\delta_T\|g'\|_\infty\|n_Z\|_{L^1(\mathrm{padded})}.
\]
The field error includes the invariant-cut comparison. A uniform \(o(\epsilon^2)\) field error and \(\delta_T\to0\) suffice for a nonzero distributional derivative. If the physical news belongs to \(L^2\) and the flux law holds, choose \(g\) with \(\langle g,n_Z\rangle\ge\|n_Z\|_2/2\). For total error at most \(\epsilon^2\|n_Z\|_2/4\), exact energy is at least \(2(\epsilon^2\|n_Z\|_2/4)^2M_0>0\). The weak comparison needs no convergence of \(T'\) to one. Strong comparison requires Jacobian control.

A strong estimate \(\|N_B-\epsilon^2 n_Z\|_2\le C|\epsilon|^{2+\gamma}\), \(\gamma>0\), gives positivity if \(C|\epsilon|^\gamma<4.20\cdot10^{-4}\) within the exterior smallness regime, using the audited \(\mathcal K_Z=2\|n_Z\|_2^2\). Quartic remainder corresponds to \(\gamma=2\). Half-circle translation gives evenness for an equivariantly selected exact constrained family in a matched clock. Evenness admits \(|\epsilon|^3g\). A quartic Taylor remainder requires uniform fourth derivatives in the physical trace norm or an equivalent estimate. Uniform preparation stability also requires higher norm bounds for the allowed homogeneous data.
