# Nonmean compact principal symbol, mass and a weak trace criterion

## Geometric trace and averaged charge

Use \(r_0=1,\ L=10,\ k=\pi/5\), the normalized unhalved GL cosine at \(r=2\), and \(I=[-10,-8]\). The Z preparation has zero completed quadratic free radion jet. The separately mass-tuned damped family is the candidate exact family. The original family permits differentiated oscillatory \(O(r^{-2})\) momentum tails. Its equal quadratic waveform does not transfer the damped spatial weights to the original data. Both fix circumference and ADM mass. [GOAL-005 constraints](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/CONSTRAINTS.md) · [GOAL-006 damped family](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/REPORT.md)

The optical area chart has \(R=r_5e^{q/2}\), \(b=e^q\), \(\beta=\log(Bb)\) and metric
\[
 ds_5^2=-BVdu^2-2Bdu\,dR-2BAdu\,dz+b^2dz^2+r_5^2d\omega^2 .
\]
At invariant sphere radius \(r_5=s\), \(R=s e^{q(u,R,z)/2}\). The tangent \(R_z\) drops out of the length since \(g_{RR}=g_{Rz}=0\). Thus \(\ell/L=\langle e^{q(u,R(u,s,z),z)}\rangle_z\), \(\phi_\ell=(\sqrt3/2)\log(\ell/L)\), \(R_\ell=s\sqrt{\ell/L}\) in the fixed fibration. To compare with local-area extraction, put \(q_\theta=\bar q+\theta\tilde q,\ \rho_\theta=\log(R_\theta/s),\ D=R\partial_R\). Differentiating \(2\rho_\theta=q_\theta(u,se^{\rho_\theta},z)\) gives
\[
 \dot\rho_\theta=\frac{\tilde q}{2-Dq_\theta},\qquad
 \ddot\rho_\theta=\frac{2D\tilde q\,\dot\rho_\theta+D^2q_\theta\,\dot\rho_\theta^2}{2-Dq_\theta}.
\]
With denominator bounded away from zero and \(\partial_u^iD^j q_\theta\) uniformly bounded for \(i\le1,j\le3\), the first variation of \(\log\langle e^{2\rho_\theta}\rangle\) at zero vanishes by mean zero at common \(R_*=R_0\). If \(\delta_s\) bounds \(\partial_u^iD^j\tilde q\) for \(i\le1,j\le2\) on the shell, then \(\log(\ell/L)=\bar q(u,R_*)+O_{C^1_u}(\delta_s^2)\). With \(s\delta_s^2\to0\) and a differentiated limit \(R\bar q\to\alpha\) with compatible radial remainder, \(\Phi=\sqrt3\alpha/2\).

A circle-independent terminal \(\beta_\infty\) sets \(du_B=e^{\beta_\infty}du\), with separate geometric additive origin. The lapse constraint gives \(\beta_\infty=\langle\beta(R_*)\rangle+\tfrac34\int_{R_*}^\infty s\langle q_s^2\rangle ds\). In the circle-independent reduction \(R_u=Rq_u/(2-Rq_R)\), \(B_5=B R_{r_5}\), \(V_5=(V+2R_u)/R_{r_5}\), so \(V_5/e^{\beta_\infty}\to1+\partial_{u_B}\alpha\), whereas normalized local-area \(V\to1\).

In the normalized chart let \(E=e^{\beta-3q}\), \(Q=q_z-Aq_R\), \(\zeta=A/R\), \(D_*=P/2+2\zeta\). The optical angular equation is
\[
 (RV)_R=e^\beta+R^2E[(\partial_z-A\partial_R)D_*-QD_*-(P+2\zeta-Q)^2/4-2(\zeta+Q/2)^2].
\]
Add \((R^2EAD_*)_R\), average over \(z\), and use \((EA)_R=E(P+\beta_z-3q_z)\). Periodic integration by parts cancels \(E\partial_z D_*\) against \((\beta_z-3q_z)ED_*\). The exact result is
\[
 \partial_R\langle RV+R^2EAD_*\rangle=\langle e^\beta+R^2E\Xi\rangle,\quad
 \Xi=P^2/4+2P\zeta+\zeta^2-3\zeta Q-3Q^2/4 .
\]
Since \(|\Xi|\le(5/4)P^2+(7/2)\zeta^2+(9/4)Q^2\), bounded \(E\) and integrable \(s^2\langle q_s^2+P^2+\zeta^2+Q^2\rangle\) imply a finite limit of \(m_{\rm av}=(R-\langle RV+R^2EAD_*\rangle)/2\). The tail error is at most \(\frac38\int_R^\infty s^2\langle q_s^2\rangle ds+\frac12\|E\|_\infty\int_R^\infty s^2\langle|\Xi|\rangle ds\), by \(1-e^\beta\le-\beta\) and Tonelli. This charge estimate uses first derivatives. Its identification with the Bondi mass requires vanishing boundary correction and a compatible product expansion.

For circle-independent \(q,\beta\) and \(A=0\), the \(z\)-leaf Codazzi equation gives \(\operatorname{Ric}_{uz}=\frac12[V_{Rz}+(2/R+\beta_R-3q_R)V_z]\). It follows from \(K^R{}_u=V_z/(2b)\), zero trace of \(K\) and leaf density \(Br_5^2\sin\vartheta\). If the compact \(q,\beta,A\) and required scaled derivatives are \(o(R^{-2})\) and \(V=1+C_V(u,z)/R+o(R^{-1})\) with differentiated remainder, the compact error in this expression is \(o(R^{-2})\). Then exact vacuum forces \((C_V)_z=0\). The mean needs physical Cauchy matching and the supplementary \(\operatorname{Ric}_{uu}\) equation. Outgoing Vaidya times a circle satisfies the main radial equations with an arbitrary mean mass function.

If the averaged orbit metric has \(o(R^{-2})\) five-dimensional Ricci defect and the length field has the differentiated limit, its Einstein-frame \(\phi=\sqrt3\bar q/2\), \(m=R(1-e^{-\bar\beta}\bar V)/2\) obey
\[
 m_{u_B}=R^2e^{-\bar\beta}\phi_{u_B}(-\phi_{u_B}+\bar V\phi_R)+o(1)\to-N^2.
\]
The spherical mass identity contracts the mixed \(u_B{}^R\) equation, which removes the trace term of the conformal Ricci identity. A sufficient regime is \(q_z=O(R^{-2-\gamma}),q_{Rz}=O(R^{-3-\gamma})\), \(\gamma>2\), with differentiated \(u,z,R\) bounds, zero terminal compact torsion and shift, and the above supplementary condition. The radial constraints give \(P=O(R^{-2-\gamma}), A=O(R^{-1-\gamma})\), and after eliminating \((C_V)_z\) the compact \(V\) is \(O(R^{-\gamma})\). These differentiated tails yield the stated mean Ricci defect by the warped curvature formula. Integrating at fixed radius gives \(E_I/M_0=2\int_I N^2du_B\) in dimensionless units. The initial damped weights alone do not give these evolved tails. [Optical hierarchy and supplementary equations](https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/vacuum/TRACE.md) · [GOAL-009 product-end conditions](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/checks/README.md)
