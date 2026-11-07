# Original and damped Z Cauchy data

Use \(r_0=1\) and \(\tau=v-r=0\). Subscripts 1 and 2 below are the \(\epsilon e^{ikz}\) and \(\epsilon^2e^{iKz}\) coefficients. Let \(a_0^2=1+1/r\), \(U_0=a_0^{-2}\), \(P=r^2q_{\rm leaf}\), \(J=r^2\eta\), \(W=r(a^{-2}-U_0)\), \(d=\kappa+q_{\rm leaf}/2\). Here \(q_{\rm leaf}\) is the trace of the leaf extrinsic curvature, \(K_{rz}=a\eta\), and \(K_{\theta\theta}=e^{2g}P/2\). The Z free data have \(\gamma_{zz}=1\), \(\gamma_{zr}=0\), \(K_{zz}=0\), and sphere radius \(r e^g\) with \(g\) pure first harmonic. The background values are \(P_0=2\sqrt{U_0}\), \(J_0=W_0=0\), and \(d_0=1/[2r^{3/2}(r+1)^{3/2}]\).

On the slice set \(g_1=h^+_{\theta\theta}/(2r^2)\), \(A_1=a_1=h^+_{rr}/(2a_0)\), \(W_1=-2rA_1/a_0^3\). Obtain \(P_1,J_1,d_1\) from \(K_{ij}=-(\partial_\tau-\mathcal L_\beta)\gamma_{ij}/(2N)\) with the actual GL velocity \(\partial_\tau h^+_{ij}=\Omega h^+_{ij}\). This uses the completed first jet in the \((\tau,r)\) chart, including the EF-to-slice tensor transformation.

Let \(\delta=p_{\rm d}\chi(r)\). Original Z has \(\delta=0\). Damped Z has \(p_{\rm d}>0\) with \(\chi=0\) on the inner collar and \(\chi=1\) sufficiently far out. A proposed fixed representative uses \(p_{\rm d}=8\) and \(\chi(r)=b(r-2)/[b(r-2)+b(3-r)]\), with \(b(x)=e^{-1/x}\) for \(x>0\) and zero otherwise. GOAL-006 defines a family. No particular cutoff was used in a numerical Cauchy run.

At order two,
\[
 d_2=-\frac{\delta J_2}{iK a_0r^3},\qquad
 A_2=a_2=-\frac{a_0^3 W_2}{2r}+\frac{3a_0^5W_1^2}{8r^2},\qquad
 (W_2,P_2,J_2)(r_i)=(2r_i A_1^2/a_0^4,0,0),\quad r_i=3/4 .
\]
The damping mean correction and the \(O(\epsilon^2)\) ADM mass adjustment contribute only to the zero harmonic at this order. Finite-collar smooth dependence in the original parameters suffices for the \(+2\) projection.

## Radial system

Define the following functions of \(r\).
\[
\begin{aligned}
 H_0&=2/r, & H_1&=2g_1',&
 B_0&=2/r^2,& B_1&=4g_1''+12g_1'/r,& B_2&=6(g_1')^2,\\
 D_0&=(H_0U_0)^{-1},&
 D_1&=-D_0(H_1/H_0+W_1/(rU_0)),\\
 Z_1&=(2d_0/r^2-P_0/r^4)P_1+2d_1P_0/r^2,\\
 Z_2&=(2d_0/r^2-P_0/r^4)P_2+2d_2P_0/r^2-P_1^2/(2r^4)
                +2d_1P_1/r^2-2J_1^2/r^4,\\
 C_1&=-4g_1/r^2+4k^2g_1+Z_1-U_0B_1-H_1U_0',&
 C_2&=4g_1^2/r^2+6k^2g_1^2+Z_2-U_0B_2 .
\end{aligned}
\]
Then
\[
\begin{aligned}
 W_2'={}&-K^2D_0W_2-k^2D_1W_1+2D_0(ikg_1)(ikW_1)
 -\frac{3(ikW_1)^2}{2rH_0U_0^2}
 +\left(-\frac{B_1}{H_0}+\frac{B_0H_1}{H_0^2}\right)W_1
 +\frac r{H_0}(C_2-H_1C_1/H_0),\\
 P_2'={}&iKa_0J_2+2rd_2+3ik A_1 J_1+2ika_0g_1J_1-2g_1'P_1+2r^2g_1'd_1,\\
 J_2'={}&iK\left(a_0P_2/2+(r^2d_0-P_0/2)A_2\right)-\delta J_2/r
 +ikg_1(a_0P_1+A_1P_0)-2g_1'J_1+iKr^2 A_1 d_1 .
\end{aligned}
\]
This is a regular linear inhomogeneous IVP on \(r\ge r_i\), including the horizon. The [independent expansion check](checks/check_spatial_ode.py) at \(r=2\) uses arbitrary symbolic jets. It rejects an omitted lapse drift, an incorrect momentum product factor, and reversed damping.

## Unit-gauge spacetime lift

The induced metric coefficients are \(q_{rr}=2a_0A_2+A_1^2\), \(q_{\theta\theta}=2r^2g_1^2\), \(q_{zz}=q_{zr}=0\). Obtain the second fundamental form by expanding \(K_{rr}=a^2(d-P/(2r^2))\), \(K_{rz}=aJ/r^2\), \(K_{\theta\theta}=e^{2g}P/2\), \(K_{zz}=0\).

Fix the first-order gauge of a local vacuum development to the audited completed GL mode. Choose the raw second-order shift with \(\beta^z=0\) on the slice. Then \(q_{zA}|_\Sigma=\partial_\tau q_{zz}|_\Sigma=0\). The unitary transformation \(\xi_z=-q_{zz}/(2iK)\), \(\xi_\mu=-(q_{z\mu}+\partial_\mu\xi_z)/(iK)\) fixes the slice pointwise and preserves its induced metric and second fundamental form. Equivalently, for an arbitrary raw shift the exact Z conditions imply \(\partial_\tau q_{zz}=2\partial_zq_{z\tau}\) there, yielding the same vanishing \(\xi|_\Sigma\).

Write \(b_j^\beta=(\beta^r)_j\), \(N_j\) for the lapse coefficients, and \(\eta_j=J_j/r^2\). With \(b_0^\beta=1/(r+1)\) and \(N_0=a_0^{-1}\),
\[
 b_1^\beta=(h^+_{\tau r}-b_0^\beta h^+_{rr})/a_0^2,\quad
 N_1=(-h^+_{\tau\tau}+2b_0^\beta h^+_{\tau r}-(b_0^\beta)^2 h^+_{rr})/(2N_0).
\]
Set \(q_S=q_{rr}/a_0^2+2q_{\theta\theta}/r^2\), \(T=-2Q_{zz}/K^2\). The shift, lapse and time components follow from
\[
\begin{aligned}
 iK a_0^2 b_2^\beta &=2N_0(a_0\eta_2+A_1\eta_1)+2N_1 a_0\eta_1-2a_0A_1 ik b_1^\beta,\\
 2N_0N_2&=N_0^2(T-q_S)-N_1^2+a_0^2(b_1^\beta)^2,\\
 q_{\tau r}&=a_0^2b_2^\beta+2a_0A_1 b_1^\beta+b_0^\beta q_{rr},\\
 q_{\tau\tau}&=2b_0^\beta q_{\tau r}-(b_0^\beta)^2q_{rr}+N_0^2(q_S-T).
\end{aligned}
\]
Expand \(\partial_\tau\gamma=\mathcal L_\beta\gamma-2NK\) to obtain \(\partial_\tau q_{ij}\). Use the \(r,\tau\) components of \(\bar\nabla^\mu q_{\mu\nu}=\bar\nabla_\nu T+2iQ_{z\nu}/K\) sequentially to find \(\partial_\tau q_{\tau r}\) and \(\partial_\tau q_{\tau\tau}\), with coefficient \(\bar g^{\tau\tau}=-a_0^2\) and known remaining derivatives. The normal derivative is \(\bar\nabla_{\bar n}q\), \(\bar n=N_0^{-1}(\partial_\tau-b_0^\beta\partial_r)\). Existence of the local development and its unitary completion supplies compatibility of both defects and their normal derivatives. A second route computes the raw metric through three time derivatives using the two ADM evolution equations, then differentiates the unitary gauge transformation. These are algebraic regular lifts of compatible data.

At \(r=2\), \(t=0\) on the slice, \(h^+_{\theta\theta}=4\), \(g_1=1/2\), and \(q^U_{\theta\theta}=2\) for both preparations. The point source yields \(1.43908<T(0,2)<1.43917\). These are individual Cauchy components. Full numerical radial data and normal-velocity arrays were never produced. Their complete analytic reconstruction is specified above.

## Distinct preparation data

Let \(s_1=h^+_{\theta\theta}/r^2\), \(\alpha=\sqrt{k^2+\Omega^2}\), and \(\upsilon=\Omega^2/k^2\). The audited mode gives \(s_1'/s_1=-\alpha+O(r^{-1})\) and \(g_1\sim s_1/2,\ A_1\sim\upsilon s_1,\ P_1\sim-\Omega r^2s_1,\ J_1\sim-i\Omega\alpha r^2s_1/k,\ d_1\sim\Omega(3/2+\upsilon)s_1\). If original \(J_2\) vanished on a far half-line, the \(P_2\) equation would give \(P_2'\sim\Omega\alpha(2\upsilon-3/2)r^2s_1^2\). The stable \(W_2\) equation gives \(W_2\to0\), and then \(J_2'=0\) forces the finite limit of \(P_2\) to vanish. Integrating back gives \(P_2\sim\Omega(3/4-\upsilon)r^2s_1^2\), \(W_2=O(r^2s_1^2)\), \(A_2=O(rs_1^2)\). The \(J_2'\) equation divided by \(ik\Omega r^2s_1^2\) tends to \(-3/4+\upsilon+2\upsilon^2\ne0\), a contradiction. Equality of damped and original jets would force \(\delta J_2=0\) by subtraction of their \(J_2\) equations. Thus every admissible nontrivial damping changes the massive jet. Its outgoing projection still needs evaluation.

[GOAL-005 exact constraints](../GLR-GOAL-005/CONSTRAINTS.md) · [GOAL-006 damping and decay](../GLR-GOAL-006/DERIVATIONS.md#2-route-a-damping-with-the-exact-z-leaf-data)
