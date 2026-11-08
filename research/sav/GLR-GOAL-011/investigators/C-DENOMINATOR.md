# Independent audit of the normalized RW denominator

Scope is the final GLR-011 denominator on the existing continued outgoing sheet. I inspected `GLR-GOAL-011/checks/physical_overlap/denominator_refinement.py` and the called scalar Riccati transport, log primitive and horizon normalization. The audited script's SHA-256 is `7c3fbad2317981e36b43db435aa1cd1cbcfd66ac5ce3dedd8192d6bb138cc5b4`. No source-numerator or root-count recalculation was attempted.

## Outgoing amplitude and endpoint bound

With \(g=e^{-sx}w\), the RW equation reads \(w_{xx}-2sw_x=Vw\). For \(a=3/s\), \(w_a=1+a/r\), and \(d/dx=f\,d/dr\),
\[
(w_a)_x=-af/r^2,\qquad (w_a)_{xx}=f(2a/r^3-3a/r^4).
\]
Since \(2sa=6\), the \(r^{-2}\) terms cancel. The \(r^{-4}\) terms also cancel, giving
\[
(\partial_x^2-2s\partial_x-V)w_a=f(3-4a)/r^3.
\]
This exact residual matches the implementation.

Write \(e=w-w_a\), \(z=e_x\), \(\gamma=dx/d\rho\). Then \(z_x-2sz=Ve-f(3-4a)/r^3\). On the outward \(\pi/4\) ray, impose the decaying integral for \(z\) and \(e(\infty)=0\). The former is the negative integral of the forcing against \(\gamma(\tau)e^{2s(x(\rho)-x(\tau))}\). The latter is \(e(\rho)=-\int_\rho^\infty\gamma z\,d\tau\). With \(R=144\), let \(\gamma_b=1+1/(R-1)\), \(\delta=\Re(se^{i\pi/4})-|s|/(R-1)\), \(K=\gamma_b/(2\delta)\), \(A=(1+1/R)|3-4a|\), and \(v=(1+1/R)(6+3/R)\). For a trial cap \(|e|\le B/\rho^2\) the forcing is bounded by \((A+vB/R)/\rho^3\). Consequently
\[
|e_x|\le K(A+vB/R)/\rho^3,\qquad
|e|\le \gamma_b K(A+vB/R)/(2\rho^2).
\]
The contraction constant in \(\sup\rho^2|e|\) is \(\gamma_b K v/(2R)\). Choosing \(B=\gamma_b K A\) and checking the strict invariant-ball inequality suffices. On the full coordinate rectangle enclosing the pole disk, my separate 128-bit Arb evaluation gave \(\delta>0.39726753\), contraction \(<0.02686640\), and invariant-ball ratio \(<0.52686640\). At \(R\), the error caps are \(B/R^2<0.001027187346\) and \(|e_x|<7.464336543\times10^{-6}\). Centered Acb rectangles used in the code contain the corresponding complex disks. The bound \(|w|\ge1-|a|/R-B/R^2>0\) justifies the initialization of \(w_x/w\). The branch is the declared outgoing continuation with \(w\to1\).

## Derivative, transport and sign

The stencil \([8(D(s_c+h)-D(s_c-h))-(D(s_c+2h)-D(s_c-2h))]/(12h)\) leaves the linear Taylor coefficient and cancels the cubic. If \(M\) bounds \(|D|\) on the radius-\(R_c\) Cauchy circle and \(t=h/R_c\), bounding the remaining odd coefficients separately gives
\[
(M/R_c)\left[\frac{4t^4}{3(1-t^2)}
+\frac{(2t)^4}{3(1-4t^2)}\right].
\]
Moving the derivative from the center to radius \(d_q\) adds \((M/R_c)[(1-d_q/R_c)^{-2}-1]\). These are the code's constants. The argument uses the already validated analytic matching derivatives on the larger Cauchy disk, with no poles of either log derivative on that disk.

The outgoing Riccati variable transported from \(R\) is \(w_x/w\). The captured cells run from the far point to the match, so the primitive difference at \(1\) minus \(0\) has the required orientation. Its geometric remainder and transported log-derivative error both bound the entire directed cell. Thus the reconstruction is \(g_m=w(R)\exp[-sx_m+\int_{x(R)}^{x_m}w_x/w\,dx]\), with no missing far exponential. At the horizon, \(h=e^{sx}n\) with \(n(1)=1\) and \(n_r(1)=3/(1+2s)\), consistent with the regular series and its propagation in the same fixed tortoise convention.

For \(D=p_L-p_R\), \(p_L=h_x/h\), \(p_R=g_x/g\), the convention \(W=hg_x-h_xg\) gives \(W=-hgD\) identically near the pole. At the exact zero \(D(s_q)=0\) this yields \(W'(s_q)=-h_mg_mD'(s_q)\). The product identity for \(W'\) is specific to the zero, even though the factors are enclosed on the entire disk.

One replay with ` PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/denominator_refinement.py ` reproduced the saved output, including \(D'=[0.728\pm0.000916]+[-3.316\pm0.000783]i\), \(W'=[7.8\pm0.0560]+[-1.7\pm0.0303]i\), and \(|W'|=[8.0\pm0.0443]\) in Arb display. No discrepancy was found within this bounded audit. The certificate uses Arb objects directly for subsequent numerical bounds.
