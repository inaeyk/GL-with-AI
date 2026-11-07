# Specialist A — axial Einstein source

Inputs were GOAL-007 at 0864d9d807de56cbdd6e64d215e6295508a3007f and the necessary GOAL-005 unitary GL and Einstein-frame monopole at that tree, including physics/goal002_second_order.md. The direct connection derivation is a reconstruction with prior exposure to the pinned equation and no exposure to GOAL-009/010 or another new derivation. Primary checks used Kodama–Ishibashi–Seto v3, §II.B.2, (24)–(36), and Martel–Poisson v1, §V (5.13)–(5.16) and §VI (6.11)–(6.12). [KI–Seto](https://arxiv.org/pdf/hep-th/0004160v3) [Martel–Poisson](https://arxiv.org/pdf/gr-qc/0502028v1)

## Exact mixed and angular Ricci

Take the spherical metric $B_{ab}dy^ady^b+R^2\gamma_{ij}dx^idx^j$ with Lorentzian orbit $(t,r,z)$, positive $R$, signature $(-++++)$, and unit two-sphere $\gamma$. Define $p=d\log R$, $\mathfrak s=1-R^2(D\cdot p+2p^2)$ and note that the angular background Ricci is $\mathfrak s\gamma_{ij}$. Use a real coexact harmonic $V_i$, $T_{ij}=\hat D_{(i}V_{j)}$, $\hat D_j(dV)_i{}^j=\ell(\ell+1)V_i$, and $\lambda=(\ell-1)(\ell+2)=4$ at $\ell=2$. Write $h_{ai}=j_aV_i$ and $h_{ij}=2j_TT_{ij}$. The active angular shift $\xi^i=\Lambda V^i$ adds $R^2\partial_a\Lambda$ to $j_a$ and $R^2\Lambda$ to $j_T$. Set $A_a=j_a/R^2-\partial_a(j_T/R^2)$, $F=dA$, and calculate in $j_T=0$ gauge.

The nonzero types of $C^A{}_{BC}=\delta\Gamma^A{}_{BC}$, up to lower-slot symmetry, are
\[
\begin{aligned}
 C^a{}_{bi}&=R^2(\tfrac12F_b{}^a-p^aA_b)V_i,&
 C^i{}_{ab}&=(D_{(a}A_{b)}+p_a A_b+p_b A_a)V^i,\\
 C^a{}_{ij}&=R^2A^aT_{ij},&
 C^i{}_{aj}&=\tfrac12 A_a(dV)_j{}^i,\\
 C^i{}_{jk}&=R^2(p\cdot A)\gamma_{jk}V^i.
\end{aligned}
\]
The trace vanishes. With $M^b{}_a=R^2(\tfrac12F_a{}^b-p^b A_a)$, the base derivative in mixed Ricci gives $(D_bM^b{}_a-p_bM^b{}_a)V_i$. The angular derivative gives $V_i$ times the sum of $\ell(\ell+1)A_a/2$, $2p_bM^b{}_a$, $-R^2p_a(p\cdot A)$ and $R^2p^b(D_{(a}A_{b)}+p_a A_b+p_b A_a)$. Therefore
\[
 \delta R_{ai}=\left[-\frac{D_b(R^4F^b{}_a)}{2R^2}
                  +\frac{\lambda}2 A_a+\mathfrak s A_a\right]V_i,\quad
 \delta R_{ij}=D_a(R^2A^a)T_{ij},\qquad \delta R_{ab}=0.                 \tag{1}
\]
The base and angular contractions in $\delta R_{ij}$ give $R^2D\cdot A$ and $2R^2p\cdot A$, respectively, with the $C^i{}_{aj}$ terms cancelling by angular antisymmetry. All terms in $\delta R_{ab}$ are proportional to $\hat D_iV^i$. Horizontal mixed Ricci uses $\partial_a-\eta A_a V^i\partial_i$ and removes $\mathfrak s A_a$ off shell. Spherical vacuum sets $\mathfrak s=0$ through the required orders. The change of the raw Ricci in passing to angular gauge is the Lie derivative of the lower spherical Ricci, which vanishes through those orders.

Thus the complete axial Einstein equation is
\[
 (\mathcal L A)^a=\partial_b(\mathcal D^{ba\,cd}F_{cd})-\lambda\mathcal M^{ab}A_b=0,\quad
 \mathcal D^{ba\,cd}=\sqrt{|B|}R^4 B^{bc}B^{ad},\quad
 \mathcal M^{ab}=\sqrt{|B|}R^2 B^{ab}.                                   \tag{2}
\]
Taking its divergence gives the angular equation $\partial_a(\mathcal M^{ab}A_b)=0$. This also agrees with KI–Seto with their $F_a=RA_a$, $n=2$, angular eigenvalue $k_V^2=\ell(\ell+1)-1$. On a spacelike slice prescribe spatial $A_i$ and $\pi^i=\sqrt{\gamma_\Sigma}R^4F^{ia}n_a$. Gauss gives $n\cdot A=\partial_i\pi^i/(\lambda\sqrt{\gamma_\Sigma}R^2)$. The electric definition and weighted divergence determine the first time derivatives. Holding the spatial pair fixed as $\epsilon$ varies still induces higher-order constrained temporal and velocity data. The $\ell=1$ Killing limit has $T_{ij}=0$, $\lambda=0$ and a Maxwell horizontal equation.

## Cubic source

Let $b=-fdt^2+dr^2/f+dz^2$, $f=1-r_0/r$, $r_0=2M$, $z\sim z+L$, $L/r_0=10$, $k=2\pi/L$. The unitary GL tangent $H=e^{\Omega t}H(r)\cos kz$ has $H_{zA}=0$ and radial normalization $H_{\theta\theta}(2r_0)/(8r_0^2)=1$. Each Fourier coefficient is half the real cosine coefficient. Without factorials expand
\[
 B=b+\epsilon H_B+\epsilon^2 B_2,\quad
 R^2=r^2(1+\epsilon s+\epsilon^2\sigma),\quad
 X^a{}_b=b^{ac}(H_B)_{cb},\quad Y^a{}_b=b^{ac}(B_2)_{cb}.
\]
Here $s=H_{\theta\theta}/r^2$, $\sigma=B_{\theta\theta}^{(2)}/r^2$ and the unitary vacuum $zz$ equation gives $\operatorname{tr}X/2=-s$. With $d_2=\operatorname{tr}Y/2-\operatorname{tr}X^2/4+(\operatorname{tr}X)^2/8$, inverse, determinant and radius expansion yields
\[
\begin{aligned}
 \mathcal M_1&=-X\mathcal M_0,\\
 \mathcal M_2&=[(d_2+\sigma-s^2)I+X^2-Y]\mathcal M_0,\\
 \mathcal D_1&=[sI-X\oplus X]\mathcal D_0,\\
 \mathcal D_2&=[(d_2+2\sigma-s^2)I-s(X\oplus X)
              +(X^2-Y)\oplus(X^2-Y)+X\otimes X]\mathcal D_0.
\end{aligned}                                                            \tag{3}
\]
The slot sum is denoted $\oplus$, and $\otimes$ acts on both raised slots. Let $A_j$ be the coefficients of $\epsilon^j\eta$ for $j=0,1,2$, and angle brackets the circle mean. Then
\[
 \mathcal L_0 A_0=0,\qquad \mathcal L_0 A_1=-\mathcal L_1 A_0,\qquad
 \mathcal L_0\langle A_2\rangle=S=-\langle\mathcal L_2 A_0+\mathcal L_1 A_1\rangle .
                                                                            \tag{4}
\]
The $B_2$ contribution uses only $B_{2,0}$. The $HA_1$ convolution has both opposite-sign Fourier pairs. Reflection kills mean $A_z$. Although $(\mathcal L_1 A_0)^z=0$ in unitary GL gauge, the massive $z$ equation couples to the divergence of $A_{1,a}$ through $F_{az}$ and the weighted constraint. Imposing $j_z=0$ by angular gauge would still leave $-\partial_z(j_T/R^2)$ in $A_z$.

For tangential Greek slots in $(t,r)$ write $F_j^{\beta\alpha}=b^{\beta\mu}b^{\alpha\nu}(dA_j)_{\mu\nu}$. Let $m=Y_{2,0}+\tfrac12Y_{zz,0}I$, with $Y_{2,0}$ the mean noncompact endomorphism, and $\sigma_E=\langle\sigma\rangle+Y_{zz,0}/2$. Set
\[
\begin{aligned}
 W&=2\sigma_E-\tfrac12\operatorname{tr}m+
        \left\langle\tfrac14\operatorname{tr}X^2+\tfrac72s^2\right\rangle,\\
 Z&=(\sigma_E+\tfrac12\operatorname{tr}m)I-m+
    \left\langle X^2-(\tfrac14\operatorname{tr}X^2+\tfrac12s^2)I\right\rangle .
\end{aligned}
\]
Then the mean current in Schwarzschild coordinates is
\[
 S^\alpha=-\partial_\beta\!\left[r^4\!
      \left(W F_0^{\beta\alpha}+3\langle sF_1^{\beta\alpha}\rangle\right)\right]
 +\lambda r^2\!\left[ Z^\alpha{}_\gamma b^{\gamma\delta}A_{0,\delta}
       -\langle X^\alpha{}_\gamma b^{\gamma\delta}A_{1,\delta}\rangle\right].
                                                                            \tag{5}
\]
On tangential 2-forms the slot sum is $\operatorname{tr}X$. A determinant check is $(1+\epsilon s)^2/\sqrt{\det(I+\epsilon X)}=1+3\epsilon s+\epsilon^2(\operatorname{tr}X^2/4+7s^2/2)+O(\epsilon^3)$. The compact density derivative integrates to zero. Only tangential components of $A_1$ occur explicitly after solving the full massive system.

Let $C^P,C^J,C^K$ denote the pairs $(j_a,j_T)/r^2$ for the respective raw metric coefficients, and $\mathscr I(C)_a=C_a-\partial_a C_T$. Then
\[
 A_0=\mathscr I(C^P),\quad A_1=\mathscr I(C^J-sC^P),\quad
 A_2=\mathscr I(C^K-sC^J+(s^2-\sigma)C^P).                                \tag{6}
\]
The derivatives act on the products. The vector coefficient of the raw Ricci source $2Q(H,J)+2Q(P,B)+3T(H,H,P)$ is $-b_{ab}/(2r^2\sqrt{|b|})$ times $\mathcal L_1A_1+\mathcal L_2A_0+\mathcal L_0\mathscr I(-sC^J+(s^2-\sigma)C^P)$. Lower equations kill the terms from the overall conversion factor.

## Einstein frame and retarded projection

Reflection kills the mean KK vector. In the pure mean KK ansatz the orbit determinants are equal, the noncompact inverse scales by $e^{2\phi/\sqrt3}$ and $R_5=e^{-\phi/\sqrt3}R_E$. The radion factors cancel separately in both tangential densities. For a pure radion $m=\sigma_E=0$. The actual monopole is sourced by $\mathcal T_{\mu\nu}=-\langle Q_{\mu\nu}\rangle+\bar g_{\mu\nu}\langle\operatorname{tr}_5Q\rangle/2$, conserved by the lower vacuum equations. In ingoing EF areal gauge, the covariant metric perturbation is $m_{vv}=-2f\beta+2\mu/r$, $m_{vr}=\beta$, $m_{rr}=0$, $\sigma_E=0$. The pinned quadratures are
\[
 \beta=-\tfrac12\int_r^\infty R\mathcal T_{rr}(v,R)\,dR,\qquad
 \mu=\delta m+\tfrac12\int_r^\infty R^2\mathcal T^v{}_v(v,R)\,dR,
 \qquad \mu_{,v}=r^2\mathcal T^r{}_v/2.
\]
Fixed ADM mass gives $\delta m=0$. The lapse is normalized at infinity and convergence at fixed ingoing $v$ uses spatial localization. Null extraction uses a matched outgoing chart.

Algebraic KK projection in the completed chart gives mean noncompact axial metric coefficient $K_0+B_{zz,0}^{(2)}P/2$, including the angular tensor. The projected radius contains the same conformal factor, hence
\[
 (A_2^E)_\alpha=\mathscr I(\langle C^K\rangle-\langle\sigma\rangle C^P)_\alpha
 =\langle A_{2,\alpha}\rangle+
   \mathscr I(\langle sC^J\rangle-\langle s^2\rangle C^P)_\alpha.           \tag{7}
\]
A news comparison needs differentiated decay of these cross terms and common Bondi cuts. A change of order-two mean orbit coordinates induces the Lie derivative of $A_0$ and the corresponding pullback of the unperturbed waveform. A mass shift can cause a logarithmic retarded-time shift at fixed Schwarzschild cuts.

With $U=r^4(d\langle A_2\rangle)_{tr}$, $\Psi=-2U/(\lambda r)$, orientation $dt\wedge dr$, and $x=r+r_0\log(r/r_0-1)$, the components of (4) give $A_{2,t}^{(0)}=f(S^t-U_r)/(\lambda r^2)$ and $A_{2,r}^{(0)}=-(S^r+U_t)/(\lambda r^2f)$. Their curl gives
\[
 [-\partial_t^2+\partial_x^2-f(\ell(\ell+1)/r^2-6M/r^3)]\Psi
 =-\frac{2}{\lambda}\left[\frac{\partial_t S^r}{r}
                 +rf\,\partial_r(f S^t/r^2)\right].                       \tag{8}
\]
The background dual $Q_z=r^4(*dA)_z=-U$ with $dt\wedge dr\wedge dz$ confirms the sign. Equation (8) takes horizon-regular compatible finite data and no added incoming outer solution. The volume integral combines with the evolution of the prepared data. Given short-range and trace bounds, the CPM limit multiplies $T_{ij}$ in the shear and its $u$ derivative gives news.

## Evidence and open gates

- Proved locally or algebraically at fixed order under the stated assumptions: (1)–(8). Checks include the angular constraint, Killing limit, determinant identity, conformal radion cancellation and source-free Schwarzschild CPM potential. No numerical run was needed.
- Exact negative control: $S^\alpha=-\lambda r^2 b^{\alpha\beta}\partial_\beta g$ with smooth compact $g$ gives zero master forcing in (8) and the forced solution $A=dg$. A nonzero current gives no lower bound on the master projection.
- Formal under further analytic inputs: the retarded trace of the complete prepared data and sources. Remaining gates include pole-domain continuation and evaluation or exclusion of the specified residue, possible cancellation with initial and pullback terms, and exact nonlinear differentiability at the Bondi end.
