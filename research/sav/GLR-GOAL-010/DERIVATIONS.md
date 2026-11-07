# Forced evolution and separated comparison

Work in the regular EF chart with \(r_0=1\), \(f=1-1/r\), \(a=K^2\), \(D=1+ar^3\). The background curvature convention has \((\overline{\mathrm{Riem}}q)_{\mu\nu}=\bar R_{\mu\rho\nu\sigma}q^{\rho\sigma}\), with \(R^\rho{}_{\sigma\mu\nu}\) starting with \(\partial_\mu\Gamma^\rho_{\nu\sigma}-\partial_\nu\Gamma^\rho_{\mu\sigma}\). Greek indices are four-dimensional. Use the complex \(+K\) coefficient, with the conjugate supplying \(-K\).

## Tensor system

At nonzero \(K\), the local gauge transformation in [Cauchy data](CAUCHY_DATA.md) sets \(q_{zA}=0\). The \(zz,z\mu\) Ricci blocks fix \(T=-2Q_{zz}/a\) and \(J_\nu=\bar\nabla_\nu T+2iQ_{z\nu}/K\). The remaining system is
\[
 \mathcal D_K q=2Q_4+2\bar\nabla_{(\mu}J_{\nu)}-\bar\nabla_\mu\bar\nabla_\nu T=F,\qquad
 \mathcal D_K=\bar\Box+2\overline{\mathrm{Riem}}-a .
\]
On the Ricci-flat background, trace and divergence commute with this Lichnerowicz operator into the scalar and covector operators \(\bar\Box-a\). Source Bianchi gives \(\operatorname{tr}_4 F=(\bar\Box-a)T\) and \(\bar\nabla\cdot F=(\bar\Box-a)J\). The trace and divergence defects therefore obey homogeneous equations with zero compatible Cauchy data. This is a normally hyperbolic IVP with a regular horizon-crossing time \(\tau=v-r\), \(\bar g^{\tau\tau}=-(2-f)\). Smooth compatible data determine a unique solution on its future domain of dependence. [Pinned tensor derivation](../GLR-GOAL-006/DERIVATIONS.md#5-causal-massive-second-harmonic-and-its-energy) · [Normally hyperbolic Cauchy theory](https://arxiv.org/html/1310.0738v2)

## Regular time-domain master and inverse

Set \(S=q_{\theta\theta}\), \(j_\nu=2iQ_{z\nu}/K\), and use dots for \(\partial_v\), primes for \(\partial_r|_v\). The regular master \(U=2ar[(r-1)q_{vr}+rq_{vv}-\dot S]\) is \(\partial_t y\) in the historical separated notation. Its source is
\[
 \mathscr J=2ar^2 f(j_v'-\dot j_r)-4ar\dot Q_{\theta\theta}-a\dot T
 +\frac{4ar^2(ar^3-2)}D(f Q_{vr}+Q_{vv})
 -\frac{2a[ar^3(2r-3)-4r+3]}D j_v .
\]
It obeys
\[
 \mathcal L_{\rm EF}U=\mathscr J,\qquad
 \mathcal L_{\rm EF}=f\partial_r^2+2\partial_{vr}+fP\partial_r
                  +\frac{-4+6/D}{r}\partial_v+fQ_h ,
\]
where \(P=(r-1)^{-1}-5/r+6/(rD)\) and
\(Q_h=3/[r^2(r-1)]-ar/(r-1)-3/[r^2(r-1)D]\). Both \(fP\) and \(fQ_h\) extend smoothly to the horizon.

Define
\[
 \begin{aligned}
 M&=-\frac{fU'+\dot U}{aD}+\frac{U}{2aDr^2}
     +\frac{2r^2}{D}\big[f(j_v-rQ_{vr})-rQ_{vv}\big],\\
 L&=M'+2M/r-j_v-\dot T .
 \end{aligned}
\]
Then the reconstruction is
\[
 \dot S=rM-\frac{U}{2ar},\quad
 \dot q_{vv}=fL+\dot M,\quad
 \dot q_{vr}=-L,\quad
 \dot q_{rr}=-\frac{U''}{aD}
       +\frac{2r^2(j_v'+\dot j_r)+4rj_v-2r^3\dot Q_{rr}}{D}.
\]
The last formula follows from the trace, the scalar equation, and the \(v\) source Bianchi relation. In particular the \(1/f\) in the trace reconstruction cancels identically. The independent [arbitrary-function check](checks/check_master_time_domain.py) manufactures \(Q=-R^{(1)}[q]\) from a covariant connection variation and the warped-sphere Ricci tensor. It tests the master identity, \(M=fq_{vr}+q_{vv}\), and the four inverse identities without a temporal frequency assumption. With the compatible tensor IVP as existence and uniqueness input, its induced scalar data determine \(U\) by scalar uniqueness, and integrating the four derivatives from the actual initial \(q\) recovers the tensor. Static integration constants are thus fixed by the data. For the separated forced solution alone, division by the fixed positive growth rate \(s=2\Omega\) recovers \(q\) directly.

## Alternative affine master retaining the static sector algebraically

The literature workstream derived another regular master, which equals the old homogeneous master when \(Q=0\),
\[
 Y=-4r q_{vv}-2(ar^3-2r+3)S/r^2+4(f\partial_r+\partial_v)S-4rfT.
\]
At a nonzero separated time parameter \(s\), it equals \(y+4r(r-1)Q_{tr}^{\mathrm{static}}/s\). The source \(t\) Bianchi equation cancels the inverse time parameter. Set \(Z_Q=\operatorname{tr}_5Q\). In EF form,
\[
 \begin{split}
 \mathcal L_{\rm EF}Y=\mathcal B={}&-2ar^2(fj_r+j_v)-4arQ_{\theta\theta}-aT
 +2r^2(f\partial_r+\partial_v)Z_Q+4r^2(Q_{vv}'-\dot Q_{vr})\\
 &-\frac{2[ar^3(2r-3)-4r+3]}{D}Z_Q-\frac{8r(ar^3-2)}D Q_{vv}.
 \end{split}
\]
Define \(N=fZ_Q+2Q_{vv}\), \(C_S=2r^3N/(aD)\), \(A_0=(ar^3-2r+3)/(2r^3)\), \(G=f(1/2-ar^3)/D^2\) and \(E=(r-1)/(2ar^3D)-3(r-1)^2/D^2\). The homogeneous recipes on an arbitrary \(Y\) are
\[
 \begin{aligned}
 R_S&=-r^2Y/(2D)-(r-1)Y'/(aD)-r\dot Y/(aD),\\
 R_{vv}&=GY+E(Y'+\dot Y/f)-\ddot Y/(aD),\\
 R_{vr}&=-\dot Y'/(aD)+3r^2\dot Y/D^2-GY/f-EY'/f,\qquad
 R_{rr}=-Y''/(aD).
 \end{aligned}
\]
The affine inverse adds
\[
\begin{aligned}
 S&=R_S+C_S,\\
 q_{vv}&=R_{vv}-f\mathcal B/(aD)-A_0 C_S+(f\partial_r+\partial_v)C_S/r-fT,\\
 q_{vr}&=R_{vr}-2Q_{vr}/a+Z_Q/a-2rN/(aD)+\mathcal B/(aD)-C_S'/r+T,\\
 q_{rr}&=R_{rr}+\frac{2(2r-1-ar^3)Q_{rr}+2r(r-1)Q_{rr}'+4Q_{\theta\theta}'+4rQ_{zz}+2r^2Q_{zz}'}{aD}
                     +2r^2j_r/D .
\end{aligned}
\]
Here \(G/f\) and \(E/f\) are smooth, and all derivatives hold \(v\) fixed except the dots. The independent [coordinate-variation implementation](checks/check_master_affine.py) verifies the left inverse and forced equation with \(Q=-R^{(1)}[q]\) and arbitrary radial tensor functions. Its identities are polynomial in the separated parameter, so the off-shell left inverse extends in time, including the static sector. Equivalence for the compatible physical data follows from the full tensor and scalar IVP uniqueness. The time-domain and affine checks use related geometric identities and distinct implementations.

## Finite-slice Green function

Let \(x=r+\log(r-1)\), \(\phi=x-r\), \(b=\phi_x=1-f\), \(\tau=t+\phi\). For \(\psi=rU/D\) the static-slice equation is \((\partial_t^2+H_K)\psi=\mathfrak F\), where \(\mathfrak F=-fr\mathscr J/D=e^{s\tau}\mathfrak F_0(r)\), \(H_K=-\partial_x^2+V_K\), and
\[
 V_K=af+\frac{(6r-11)(r-1)}{r^4}
 +\frac{18(r-1)^2}{r^4D^2}-\frac{6(4r-5)(r-1)}{r^4D}.
\]
Use \(\psi_0=\psi|_\Sigma,\ \psi_1=\partial_\tau\psi|_\Sigma\) induced by the actual Cauchy jet. The second time derivative needed in \(\psi_1\) follows from the tensor wave equation. Put \(A=1-b^2,\ B=2b\partial_x+b_x\), with radial derivatives here at fixed \(\tau\). The one-sided Laplace transform obeys
\[
 (H_K-\lambda B+\lambda^2 A)\hat\psi
  =\mathfrak F_0/(\lambda-s)+A(\lambda\psi_0+\psi_1)-B\psi_0 .
\]
Conjugation by \(e^{\lambda\phi}\) gives \(H_K+\lambda^2\). With \(W=u_Hu_I'-u_H'u_I\) in \(x\), the Green kernel of this Schrödinger operator is \(-u_H(x_<)u_I(x_>)/W\) with derivative jump \(-1\), where \(u_H\) is ingoing and \(u_I\) decaying on the Laplace contour. This uses sufficiently large \(\Re\lambda>s\) for the initial transform. The \(\tau\)-kernel additionally multiplies by \(e^{-\lambda\phi(x)}e^{\lambda\phi(y)}\). Direct evolution and this transform agree by uniqueness with both initial terms. At real frequencies the growing pole is subtracted before interpreting the homogeneous scattering amplitude.

The separated particular solution plus the homogeneous solution of the *difference* jet yields the causal field. For matched data \(\psi_1=s\psi_0\), \((H_K-sB+s^2A)\psi_0=\mathfrak F_0\), the RHS is \((H_K-\lambda B+\lambda^2A)\psi_0/(\lambda-s)\), so the resolvent leaves only the growth pole. The manufactured oscillator \(y''+\omega_0^2y=F_0e^{st}\), with zero position and velocity, gives \(F_0[e^{st}-\cos(\omega_0t)-(s/\omega_0)\sin(\omega_0t)]/(s^2+\omega_0^2)\). Both checks are in [causal controls](checks/check_causal_controls.py).

## Asymptotic mismatch for both Z families

Use the pinned GOAL-003 non-derivative separated master \(y=(D/r)u_2\), growth rate \(s=2\Omega\), and first master \(u_1=c_\infty e^{-\alpha r}r^{-\beta_1}(1+O(r^{-1}))\), with \(\alpha=\sqrt{k^2+\Omega^2}\), \(\beta_1=(k^2+2\Omega^2)/(2\alpha)\). The separated force is \(-7\alpha u_1^2/(16r^2)+O(u_1^2/r^3)\). The regular ingoing and decaying separated inverse exists at \(s>0\) by the nonnegative quadratic form of \(H_K\). Normalize \(u_I\sim e^{-2\alpha r}r^{-2\beta_1}\). Writing \(u_2=u_I w\) gives \(-(u_I^2w_x)_x=u_I\mathfrak F_{\rm sep}\). The absence of the growing spatial branch gives \(u_I^2w_x\to0\). Integrating from infinity gives \(w_x=-7c_\infty^2/(64r^2)+O(r^{-3})\), hence
\[
 u_{2,\rm sep}=C u_I+u_I\left(\frac{7c_\infty^2}{64r}+O(r^{-2})\right).
\]
The angular reconstruction is \(S_{\rm sep}/e^{st}=-r u_2/2+O(u_2+u_1^2/r)\), using the historical static \(t\). For each Z preparation the unitary gauge fixes the slice. Its angular datum is \(S_Z=e^{st}H_{\theta\theta}^2/(8r^2)\sim e^{st}u_1^2/32\). The same \(\tau=0\) time factor appears on both sides. If \(C\ne0\), \(S_{\rm sep}\) has one extra power of \(r\). If \(C=0\), \(S_{\rm sep}\sim -7e^{st}u_1^2/128\), with \(S_Z-S_{\rm sep}\sim 11e^{st}u_1^2/128\). Thus the compatible Cauchy correction is nonzero for both families, independently of the value of the unknown overlap \(C\). No outgoing scattering inference is required for the proof.

The broad compatible class admits the separated jet itself. Replacing only \(n=\pm2\) leaves the first GL and quadratic zero-mode radion jets fixed. It cancels the homogeneous oscillatory correction, which proves a zero universal lower bound on the escaped oscillatory component over that class. This argument does not give exact nonlinear integrability of that broader jet or membership in either Z family.

[GOAL-003 forced master and tail, Eqs. (5), (8a), (8b)](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/physics/goal003_forced_master.md) · [Pinned positive form and regular inverse](../physics/goal003_master_independent.md)
