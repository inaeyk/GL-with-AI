# Physical mode, complete quadratic equations and checks

## Regular physical GL mode

Write the metric $g=\bar g+\epsilon h+\epsilon^2 q^{(2)}+\cdots$ without a factorial. A nonzero compact Fourier mode can be completed uniquely to $h_{zA}=0$ by
$$
 X_z=-h_{zz}/(2ik),\qquad X_\mu=-(h_{\mu z}+\partial_\mu X_z)/(ik)
$$
and the complex conjugate. On the product background the $zA$ equations give $H^\mu{}_\mu=\bar\nabla^\mu H_{\mu\nu}=0$. The remaining linear equation is $(\bar\Box+2\overline{\mathrm{Riem}}-k^2)H=0$. The TT reconstruction and explicit regular EF cancellations are given in [the pinned linear derivation](../physics/gl_linear_mode.md). We use its master $y=(1+k^2r^3)U_{\rm GL}/r$. The Schrödinger equation is $-U_{xx}+V_{\rm GL}U=-\Omega^2 U$, where $x=r+\log(r-1)$, with the $e^{\Omega x}$ future-horizon branch and decaying Coulomb branch at infinity. The metric time factor combines with the horizon branch to give $e^{\Omega v}$ times a smooth radial tensor. At $k=\pi/5$, Collingbourne's existence theorem and regular mode construction apply by periodizing the real cosine. Its lower bound is $\Omega>1/(20\sqrt{10})$. [Collingbourne, Theorem 1.1 and §4](https://arxiv.org/html/2007.08441v1)

### Exact normalization test

Put $a=k^2\in[197/500,79/200]$, $t_r=1/r$, $D=(a+t_r^3)^2$. The polynomial
$$
 P=(1-t_r)[(a+(6-11t_r)t_r^2)D+18(1-t_r)t_r^8-6(4-5t_r)t_r^5(a+t_r^3)]
$$
equals $DV_{\rm GL}$. The rational bivariate Bernstein certificate in [check_identities.py](check_identities.py) has positive minima for $P+0.17D$ on $0\le t_r\le1$, $P$ on $0\le t_r\le1/2$, and $[(0.9-0.4t_r)^2-0.4(1-t_r)t_r^2-0.17]D-P$ on $0\le t_r\le1/2$. The Schrödinger energy identity gives $\Omega^2<0.17$. Since $V_{\rm GL}>0$ for $r\ge2$, integrating the decaying equation from a putative zero excludes exterior zeros. Choose the sign of $U$ temporarily positive. The comparison function $b_*(x)=\exp[-\int(0.9-0.4/r)\,dx]$ satisfies $b_*''/b_*>V_{\rm GL}+\Omega^2$ there. The Wronskian $U b_*'-U'b_*$ is increasing to zero. Thus $p=U'/U>-(0.9-0.4/r)$, in particular $p(2)>-0.7$. Also $p<0$ outside 2 by $U''>0$ and decay. The radial logarithmic derivative of $y$ at $r=2$ is $12a/(1+8a)-1/2+2p(2)>-2a$. From the angular reconstruction $K=H_{\theta\theta}/r^2$, $|K(2)/(U(2)/2)|$ is between $0.016$ and $0.91$. Hence $K(2)=2$ is an admissible, gauge-completed physical normalization, with $4<|U(2)|<250$. This exact argument is independent of a fitted numerical mode.

### Growth rate enclosure

The computational check uses independent Dirichlet finite differences and a two-sided outward-rounded Riccati enclosure. The differential defect of a piecewise Hermite candidate is bounded on every cell, with analytic barriers at $1+2^{-34}$ and at 60. The comparison equations propagate defects to $r=2$. At the exact decimal growth endpoints $0.06328117$ and $0.06328121$, the mismatch $p_H-p_\infty$ lies in $[-9.052,-8.140]\times10^{-8}$ and $[9.068,9.979]\times10^{-8}$, respectively. The endpoint branches are nodeless and comparison keeps the intervening shooting functions finite. The crossing gives a physical decaying eigenmode. The nodeless eigenmode is the unique ground eigenfunction of this Schrödinger problem. The interval is for $\Omega r_0$ at L10. This reconstruction independently confirms the narrower $[0.06328118,0.06328120]$ certified by the October GOAL-005 execution using 160-bit Taylor and Picard flow. That prior investigation supplies the absolute mixed-coordinate $C^5<4\times10^{29}$ envelope and the common-slice EF envelope $<3.4\times10^{31}$. The new waveform enclosure uses its own lower-derivative right branch. See the [prior enclosure](../physics/goal005_enclosures.md). See [the code and actual output](compute/).

## Complete quadratic sources

Use $A_{\mu\nu}=e^{\Omega t}H_{\mu\nu}/2$ for the $+k$ complex coefficient, and $p_2=2k$. At first order $h=A e^{ikz}+\overline A e^{-ikz}$. Gaussian $z$-foliation at this order gives $K_z{}_{\mu\nu}=h_{\mu\nu,z}/2$ for the second fundamental form of $z$ leaves. Expanding $-\partial_z\mathrm{tr}(K_z)-\mathrm{tr}(K_z^2)$ yields
$$
 Q_{zz}=\tfrac14\mathrm{tr}(h_z^2)+\tfrac12\mathrm{tr}(hh_{zz}^{\rm diff}),\qquad
 \langle Q_{zz}\rangle=-\frac{k^2}{8}e^{2\Omega t}H_{\mu\nu}H^{\mu\nu}.
$$
Here $h_z,h_{zz}^{\rm diff}$ mean compact derivatives of the four-tensor, not metric components. With $C^\rho_{\mu\nu}=\delta\Gamma[A]^\rho_{\mu\nu}$, $D^\rho_{\mu\nu}=-A^{\rho\sigma}C_{\sigma\mu\nu}$ (the index on $C$ lowered by the background), the complete $+2k$ sources are
$$
\begin{aligned}
 Q_{\mu\nu}&=\bar\nabla_\rho D^\rho_{\mu\nu}-\bar\nabla_\nu D^\rho_{\mu\rho}
 + C^\rho_{\rho\lambda}C^\lambda_{\mu\nu}-C^\rho_{\nu\lambda}C^\lambda_{\mu\rho}
 -\tfrac{k^2}{2}A_{\mu\rho}A^\rho{}_\nu,\\
 Q_{zz}&=-3k^2\operatorname{tr}(A^2)/4,\qquad
 Q_{z\mu}=ik[-A^{\nu\rho}\bar\nabla_\nu A_{\rho\mu}/2
               +3\partial_\mu\operatorname{tr}(A^2)/8].
\end{aligned}
$$
These are coefficients of the Ricci tensor, so the corresponding Einstein source subtracts half the five-dimensional trace. Mixed $\pm k$ products give the zero harmonic. Explicit component formulas and independent coordinate checks for both harmonics are in [GOAL-002](../physics/goal002_second_order.md). The complete contractions include the off-diagonal time–radial terms and depend on the on-shell TT conditions.

At order two the compact gauge completion replaces $q^{(2)}$ by $q^{(2)}+\mathcal L_Xh+\mathcal L_X^2\bar g/2$. The mean scalar is $\phi_2=(\sqrt3/4)\langle q^{(2),U}_{zz}\rangle$. A first-order pure gauge with its accompanying quadratic Lie term cancels under $X\mapsto-X$; an independent second-order Lie term has zero mean $zz$ component. There is no radion first jet. For the mean sector, $R^{(1)}_{zz}=-\bar\Box \langle q^{(2),U}_{zz}\rangle/2$, so $\bar\Box\phi_2=(\sqrt3/2)\langle Q_{zz}\rangle$. In Einstein frame $m_{\mu\nu}=\langle q^{(2),U}_{\mu\nu}\rangle+\langle q^{(2),U}_{zz}\rangle\bar g_{\mu\nu}/2$. Its linear Einstein source is $-\langle Q_{\mu\nu}\rangle+\bar g_{\mu\nu}\langle\operatorname{tr}_5 Q\rangle/2$, independent of the free scalar jet. Reflection kills the forced mean vector. The regular EF mass/lapse quadratures use the charge and clock constants in [GOAL-002](../physics/goal002_second_order.md).

### Massive sector and compatibility

In second-harmonic unitary gauge let $T=\operatorname{tr}_4 q^{(2)}$, $J_\nu=\bar\nabla^\mu q^{(2)}_{\mu\nu}$. Then
$$
 T=-2Q_{zz}/p_2^2,\qquad J_\nu=\partial_\nu T+2iQ_{z\nu}/p_2,\qquad
 (\bar\Box+2\overline{\mathrm{Riem}}-p_2^2)q^{(2)}_{\mu\nu}
 =2Q_{\mu\nu}+\bar\nabla_\mu J_\nu+\bar\nabla_\nu J_\mu-\bar\nabla_\mu\bar\nabla_\nu T.
$$
On shell,
$$
 \bar\nabla^\mu Q_{\mu\nu}+ip_2Q_{z\nu}=\tfrac12\partial_\nu(\operatorname{tr}_4Q+Q_{zz}),\qquad
 \bar\nabla^\mu Q_{z\mu}=\tfrac{ip_2}{2}(\operatorname{tr}_4Q-Q_{zz}).
$$
Using the Ricci-flat divergence and trace identities for the Lichnerowicz operator, the difference between the imposed trace/divergence and the actual ones propagates by homogeneous massive wave equations. Compatible Cauchy data set these differences to zero. One can equivalently use the pinned [forced master and affine lift](../physics/goal003_forced_master.md) for the separated particular piece, with its source trace and divergence. Its bounded point inverse does not propagate the homogeneous Cauchy response. Exact initial constraints in the present family fix the full quadratic data once the free functions and inner conditions are given. Differentiating the regular local vacuum evolution supplies the causal constrained quadratic system on compact diamonds. The required global weighted $M_2$ for the full transient remains open. The prior execution bounds all seven zero and second harmonic Ricci and Einstein $C^3$ envelopes by $10^{110}$ in its stated weights. It encloses the separated second harmonic scalar master but leaves the full mixed source derivative norm and regular affine metric lift open. Its sampled reconstructed metric fails the all-component residual gate. See the [prior report](../goals/GLR-GOAL-005_REPORT.md) and [geometry audit](../verification/AUDIT_GOAL005_GEOMETRY.md).

The independent verifier derived the Gaussian formulas before reading the inherited source notes. The flat TT exponential in [check_identities.py](check_identities.py) checks all Ricci blocks and the quadratic Bianchi identity directly from the connections. A wrong second-harmonic $zz$ factor is rejected. This is an algebraic control on the equations and Fourier factors, separate from the physical L10 computation.
