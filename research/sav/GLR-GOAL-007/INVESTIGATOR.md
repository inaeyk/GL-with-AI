# Axial finite-data GL coupling

The input is GOAL-005 commit 61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708, including its unitary GL mode, nonzero decaying-end coefficient, and Einstein-frame monopole equations. See the [pinned derivation](../GLR-GOAL-005/DERIVATIONS.md) and [radiation report](../GLR-GOAL-005/RADIATION.md). The local commit object of the independent baseline audit a9bfbec0efe41b881e4b12aa262f5fcfcd94c639 became visible during the investigation. A promisor blob fetch blocked direct inspection of its diff in this workstream. This derivation concerns a perturbative retarded trace for a specified pulse preparation.

## Axial equations and complete sources

Write the spherical vacuum geometry, including its GL deformation, as $g_5=g_{ab}dx^a dx^b+R^2\gamma_{AB}dy^A dy^B$ with three-dimensional base $(t,r,z)$. Work at first order in $\eta$, allowing general $\epsilon$ in the spherical coefficients. For a unit-norm scalar spherical harmonic let $X_A=-\epsilon_A{}^B D_BY$, $X_{AB}=D_{(A}X_{B)}$ and $\lambda=(\ell-1)(\ell+2)>0$. Write the axial variation as $\delta g_{aA}=h_aX_A$, $\delta g_{AB}=2h_TX_{AB}$. An axial angular vector $\Lambda X^A$ adds $R^2\partial_a\Lambda$ to $h_a$ and $R^2\Lambda$ to $h_T$. Therefore the completed variables are
\[
 A_a=h_a/R^2-\partial_a(h_T/R^2),\qquad F_{ab}=2\partial_{[a}A_{b]} .
\]
In $h_T=0$ gauge put the connection one-form $A_a X^A$ in the sphere coframe. The connection curvature term of the Einstein action has base weight $R^4$ after angular integration and norm $\int |X|^2$. The tracefree horizontal derivative of the sphere metric is $-\eta A_a(\mathcal L_X\gamma)_{AB}$. The corresponding Einstein kinetic term is $-\tfrac14 R^2 A_aA^a\int|\mathcal L_X\gamma|^2$. Its trace term vanishes by $D_AX^A=0$. Integration by parts and sphere curvature give $\int |X_{AB}|^2=(\lambda/2)\int|X|^2$. Completing the block metric with the coframe changes the metric at quadratic order in $\eta$, adding an action term proportional to the vacuum background equation. Thus the axial action is proportional to
\[
 S_{\rm ax}=\int\sqrt{|g|}\left[-\tfrac14 R^4 F_{ab}F^{ab}
                 -\tfrac{\lambda}{2}R^2 A_aA^a\right]d^3x .
\]
The resulting equations also follow from the general warped-product [Ishibashi–Kodama v2, §5.1, (5.2)–(5.3)](https://arxiv.org/pdf/1103.6148v2), with base dimension three, sphere dimension two and vacuum. The full axial equations are
\[
 \partial_b(P^{bacd}F_{cd})-\lambda Q^{ab}A_b=0,\qquad
 \partial_a(Q^{ab}A_b)=0,\qquad
 P^{bacd}=\sqrt{|g|}R^4g^{bc}g^{ad},\quad Q^{ab}=\sqrt{|g|}R^2g^{ab}.
 \tag{1}
\]
The angular equation is the divergence of the mixed equations. The component $A_z$ and the $z$ equation are included in (1).

For explicit coefficient sources write $g=g_0+\epsilon h+\epsilon^2 q$, $R=r(1+\epsilon b+\epsilon^2c)$, without factorials. The inverse coefficients are $G_0=g_0^{-1}$, $G_1=-G_0hG_0$, $G_2=G_0hG_0hG_0-G_0qG_0$. The coefficients of $\sqrt{|g|}R^s/(\sqrt{|g_0|}r^s)$ are
\[
\begin{split}
 w_{s,0}&=1,\qquad w_{s,1}=\tfrac12\operatorname{tr}(G_0h)+sb,\\
 w_{s,2}&=\tfrac12\operatorname{tr}(G_0q)
    +\tfrac18[\operatorname{tr}(G_0h)]^2-\tfrac14\operatorname{tr}[(G_0h)^2]
    +sc+\tfrac12s(s-1)b^2+\tfrac s2b\operatorname{tr}(G_0h).
\end{split}
\]
Use $P_j^{bacd}=\sqrt{|g_0|}r^4\sum_{l+i+k=j}w_{4,l}G_i^{bc}G_k^{ad}$ and $Q_j^{ab}=\sqrt{|g_0|}r^2\sum_{l+i=j}w_{2,l}G_i^{ab}$. With $E_j A=\partial_b(P_j^{bacd}(dA)_{cd})-\lambda Q_j^{ab}A_b$ and $A=\eta(A_0+\epsilon A_1+\epsilon^2 A_2)$,
\[
 E_0 A_0=0,\qquad E_0 A_1=-E_1 A_0,\qquad
 E_0 A_2=-E_1 A_1-E_2 A_0.
 \tag{2}
\]
The angular source uses the same $Q$ convolution and follows from the density divergence identity. The metric-to-$A$ reconstruction expands $R^{-2}$ at both orders. Choose first-order nonzero-$k$ unitary base coordinates and place the second-order zero harmonic in Einstein-frame spherical Bondi coordinates.

On a spacelike slice with future unit normal $n^a$ and induced metric $\gamma_\Sigma$, prescribe the tangential $A_i$ and the spatial density $\pi^i=\sqrt{\gamma_\Sigma}R^4F^{ia}n_a$. Contracting (1) with $n_a$ gives
\[
 n^a A_a=\frac{\partial_i\pi^i}{\lambda\sqrt{\gamma_\Sigma}R^2}.
 \tag{3}
\]
The normal acceleration in the spacetime divergence cancels the term from differentiating $n_a$. The tangential curl is $F_{ij}=2\partial_{[i}A_{j]}$. Tangential Proca and $dF=0$ evolve the data and preserve (3). Thus arbitrary compact smooth canonical data obey the complete axial constraints at linear order in $\eta$ on each spherical background. Hold the canonical pair fixed in the completed chart as $\epsilon$ varies. Equation (3) supplies the induced constrained mixed and cubic data.

## Characteristic calculation

Put $x=r+r_0\log(r/r_0-1)$, $u=t-x$, $v=t+x$ and $f=1-r_0/r$. Use a spacelike slice agreeing with $t=0$ in the far exterior and joining a regular horizon-penetrating slice inward. On the product background at $n=0$, $A_z=0$ and $\psi=r^3 F_{tx}/f$. With $\mathcal O=r\psi$ the $(t,x)$ equations give $A_x=-\partial_t\mathcal O/(\lambda r^2)$, $A_t=-\partial_x\mathcal O/(\lambda r^2)$ and $\pi^x=-\mathcal O$. Substituting these into $F_{tx}$ gives
\[
 (-\partial_t^2+\partial_x^2-V_{\rm RW})\psi=0,\qquad
 V_{\rm RW}=f[\ell(\ell+1)/r^2-3r_0/r^3].
 \tag{4}
\]
Choose $\psi|_\Sigma=a(u)$, $\partial_t\psi|_\Sigma=a'(u)$, where $a$ is compact with earliest edge $u_*$ and equals $(u-u_*)_+^p/p!$ near that edge for sufficiently large integer $p\ge10$. Prescribe the axial canonical pair from this outgoing pair, with $A_z=\pi^z=0$, and hold it fixed across $\epsilon$. The front computes an explicit coefficient for finite-energy data. Smooth compact preparations follow by approximation.

The curl equation follows by lowering the index in (1), solving for
$\lambda A_a=R^2 D^b F_{ba}+4R(D^bR)F_{ba}$ and applying $d$.
Closure gives $d(D^bF_{b\cdot})=\Box_g F+$ curvature terms. The extra derivatives of $R$ contribute at most first derivatives of $F$. After multiplying by $f$ the background principal operator on each component is $-4\partial_u\partial_v+f\partial_z^2$. Its first variation has $\partial_u^2$ coefficient $-h_{LL}/f$, with $L=\partial_t+\partial_x$, since $g_0^{uv}=-2/f$ and $h_{vv}=h_{LL}/4$. The first-order background is $h=e^{\Omega t}H(r)\cos(kz)$. Define the cosine-free travel-time variation
\[
 \chi(v)=\frac14\int_{v_*}^v
 \frac{e^{\Omega(u_*+\hat v)/2}(H_{tt}+2fH_{tr}+f^2H_{rr})(r(u_*,\hat v))}
      {f(r(u_*,\hat v))}\,d\hat v,\qquad v_*=-u_* .
 \tag{5}
\]
Indeed the optical equation for $U=u+\epsilon s$ gives $\partial_v s=-h_{LL}/(4f)$, with zero launch variation. For a base gauge change $\delta h=\mathcal L_\xi g_0$, the vector $L/f$ is affine and has covector $-du$. The change of the full integral including its compact dependence is $\xi^u_\Sigma-\xi^u_{\rm end}$. Transforming the launch phase and observation cut cancels the endpoint change. The chosen completion fixes the launch and kills a pure gauge compact jet. An axial pure gauge has $A=F=0$ directly.

The perturbative front calculation uses finite-order transport. Normalize all two-form components by $r^3/f$. At order $j$ in $\epsilon$ let $c_j(v,z)$ denote the coefficient of $a^{(j)}(u)$ in the normalized curl. Let $\mathcal D=-4\partial_v+B^u(v)$ where $B^u$ is the background first-derivative matrix on the front. The highest front coefficients obey
\[
 \mathcal D c_0=0,\qquad
 \mathcal D c_1=(h_{LL}/f)c_0,\qquad
 \mathcal D c_2=(h_{LL}/f)c_1.
 \tag{6}
\]
The initial $c_1,c_2$ vanish. With fixed canonical data, all $\epsilon$ jets of the initial curl are of order $(u-u_*)^p$. Their first time derivative is of order $(u-u_*)^{p-1}$ by tangential Proca and closure. In particular the latter equations differentiate $F_{ij}$ or the normal curl once, and $A_i$ already has at most one derivative of $a$. A direct second-order operator coefficient acting on the baseline field yields at most an $a'$ term after propagation. Lower derivative terms, compact derivatives, and the derivative of an operator coefficient obey the same count. The two first-order principal interactions in (6) supply the unique $a''$ term. They multiply the identity in component indices, so $c_1=-\chi\cos(kz)c_0$ and $c_2=\tfrac12\chi^2\cos^2(kz)c_0$. In the $tx$ component $c_0=1$ by (4). The mean therefore leaves $\chi^2/4$ at order $(2,1)$, and vanishes at $(1,1)$.

Let $u_*=-x(R_*)$, $q_\infty=\sqrt{k^2+\Omega^2}$ and $U_{\rm GL}=C_\infty e^{-q_\infty r}r^{-\beta}(1+O(r^{-1}))$, with $C_\infty\ne0$. The normalized GOAL-005 mode gives
\[
 H_{tt}+2fH_{tr}+f^2H_{rr}
 \sim -\frac{(q_\infty-\Omega)^2}{k^2}\frac{U_{\rm GL}}r .
\]
In (5) substitute $dv=2\,dr/f$ and $r=R_*+w$. The factor $e^{\Omega[x(R_*+w)-x(R_*)]}$ tends to $e^{\Omega w}$, and $e^{-(q_\infty-\Omega)w}$ times a polynomial majorizes the remaining scaled integrand. Hence
\[
 \chi_\infty(R_*)\sim -\frac{q_\infty-\Omega}{2k^2}
        C_\infty e^{-q_\infty R_*}R_*^{-\beta-1}.
 \tag{7}
\]
It is nonzero for sufficiently large launch radii. This uses only the asymptotic physical eigenmode and the far causal domain.

## Short-strip null trace

On $u\in[u_*,u_*+d]$, $v\ge-u$, each fixed Fourier component of the normalized background curl equation has the form
\[
 w_{uv}=B_1 w_u+B_2w_v+Cw+S,\qquad
 |B_1|+|B_2|=O(r^{-1}),\quad |C|\le K .
 \tag{8}
\]
The first derivative coefficients come from the logarithmic radial derivatives and the product metric connection. The constant compact term belongs to $C$ at fixed Fourier index. The bounds persist after finitely many $t,x$ derivatives. Choose $0<\sigma<(q_\infty-\Omega)/2$. With weight $e^{-\sigma(v-v_*)_+}$, integration of $w_u$ in $v$ from the initial slice has norm at most $d+1/\sigma$. Integration of $w_v$ in $u$ from the initial slice or the left edge has norm at most $d$. Recovering $w$ from $w_u$ in $u$ gives another factor $d$. For $R_*$ large and $d$ small, the $B_1,B_2$ terms and the $C$ term are absorbed in the norm of the first derivatives. Commuting finitely many derivatives gives the estimates successively since derivative commutators multiply lower-order unknowns. Zero data to the left of the first front give zero field there by causality.

Subtract $c_j(v,z)a^{(j)}(u)$ in the short strip. In the order-$j$ remainder the inhomogeneity is $O((u-u_*)^{p-j})$ with differentiated one-sided bounds. For the highest derivatives this follows from (6) and $h_{LL}(u,v)-h_{LL}(u_*,v)=O(u-u_*)$. At the initial line $v=-u$, subtracting the front term leaves one additional power because $v-v_*=-(u-u_*)$ and $c_1(v_*)=c_2(v_*)=0$. The same characteristic integrations keep these powers, with a gain of one power from integration in $u$. This yields an $O((u-u_*)^{p-j+1})$ remainder and the differentiated bounds through order $p-j$ in $u$. The mixed field uses the exponential weight in (8). Since $\sigma$ is below the GL decay rate in $v$, the GL-weighted mixed sources and their needed derivatives are absolutely integrable along these cuts.

For the direct mean quadratic geometry, the pure mean radion cancels from the axial zero-mode Proca densities. In a pure mean metric $R=e^{-\phi/\sqrt3}r$, $g^{ab}=e^{2\phi/\sqrt3}g_E^{ab}$ for $a,b=(u,r)$, and $g_{zz}=e^{4\phi/\sqrt3}$. Thus $\sqrt{|g|}=\sqrt{|g_E|}$ on the base, and the powers of $\phi$ cancel separately in both $P$ and $Q$ for their zero-mode two-dimensional blocks. Reflection kills the mean KK vector. For the GOAL-005 Z preparation, the projected Einstein-frame monopole stress is exponentially small on the early wedge. The radion stress starts at order $\epsilon^4$. Fix the order-$\epsilon^2$ ADM mass variation to zero and set the Bondi lapse at infinity. Integrating the order-two mass time-flux equation from $t=0$ to $t=r+O(\log r)$ at fixed large $r$ changes the limiting mass by zero, since its source is polynomial times $e^{2\Omega t-2q_\infty r}$. Equivalently the order-two Bondi mass loss vanishes. The radial mass and lapse constraints then give exponential falloff on each cut, with differentiated bounds, by integration from infinity at fixed $u$. This removes an order-two Coulomb or logarithmic clock term from the comparison. A pure $n=\pm2$ quadratic coefficient times the baseline axial $n=0$ cannot contribute to the mean. Explicit quadratic GL products decay exponentially.

For an explicit scalar source, let $J^a=-\langle E_1 A_1+E_2 A_0\rangle^a$
be the right side of the mean density equation in $(t,x,z)$, with
$J^z=0$ by compact reflection. The two product-background equations are
$\partial_x\mathcal O+\lambda r^2 A_t=J^t$ and
$-\partial_t\mathcal O-\lambda r^2 A_x=J^x$. Eliminating $A_t,A_x$
gives the causal forced equation
\[
 (-\partial_t^2+\partial_x^2-V_{\rm RW})\psi_{21}
 =r\left[\partial_t(J^x/r^2)+\partial_x(J^t/r^2)\right].
 \tag{9}
\]
Here $\mathcal O=r\psi_{21}$ in the sourced equations. The algebraic
reconstruction retains the source terms in both components. This
equation together with the Cauchy data in (3) specifies the retarded
Green-function problem. Its potential and forcing are integrable in $v$
along the early cuts. The same Volterra estimate with no exponential
weight gives uniform bounds for the zero-mode remainders and their
derivatives, and convergence of $\psi_2,\partial_u\psi_2$ as
$v\to\infty$. In particular
\[
 \psi_{01}^{\mathscr I^+}(u)=a(u)+O((u-u_*)_+^{p+1}),\qquad
 \psi_{21}^{\mathscr I^+}(u)=\frac{\chi_\infty^2}{4}a''(u)
                                 +O((u-u_*)_+^{p-1}).
 \tag{10}
\]
These are one-sided front expansions with the derivative bounds above. The clock is the Einstein-frame Bondi clock with fixed origin. An angularly constant order-two coordinate adjustment acts on the baseline waveform with at most one derivative of $a$. The same derivative count holds for the axial Bondi reconstruction. At first order the GL compact mode decays along these early null cuts.

The strip estimates and the zero-mode Volterra equation bound the trace continuously in a sufficiently high finite $C^j$ norm of the compact canonical data. Choose $p$ large for this finite derivative loss. Smooth compact approximations to $a$ and the outgoing velocity $a'$ thus have nonzero cubic news for some fixed early interval. The calculation establishes a perturbative outgoing trace. A uniform nonlinear family-to-news differentiability theorem and an exact nonlinear two-parameter constraint construction are additional problems. A later ringdown-exciting interior pulse is causally independent of this early witness. The calculation leaves the strong-field QNM residue open.

## Bondi tensor, interference and mass/time fits

Use $C_{AB}=C(u)X_{AB}$ for the coefficient of $r$ in the four-dimensional Einstein-frame angular metric. In Bondi gauge $h_r=0$ and $h_uX_A=\tfrac12D^B C_{AB}+O(r^{-1})=-\lambda C X_A/4+O(r^{-1})$. The $h_T$ contribution drops out of $dA$. Consequently $r^3F_{ur}\to-\lambda C/2$ and $r^3F_{tx}/f$ has the same limit. For a real unit-normalized harmonic,
\[
 N_{AB}=-\frac2\lambda\partial_u\psi^{\mathscr I^+}X_{AB},\qquad
 \frac{dE_{\rm tensor}}{du}=\frac1{32\pi G_4}\int N_{AB}N^{AB}\,d\Omega
   =\frac{\ell(\ell+1)}{16\pi G_4\lambda}
     |\partial_u\psi^{\mathscr I^+}|^2 .
 \tag{11}
\]
The order-$\epsilon^2\eta^2$ term is twice the cross product in the square. For the rising front on $[u_*,u_*+\delta]$, (10) gives
\[
 \int\partial_u\psi_{01}^{\mathscr I^+}\,\partial_u\psi_{21}^{\mathscr I^+}\,du
 =\frac{\chi_\infty^2\,\delta^{2p-3}}
 {4(2p-3)(p-1)!(p-3)!}+O(\delta^{2p-2}).
\]
It is positive on a sufficiently short window and stays positive under sufficiently close smoothing. The total-time cross term can depend on the preparation and window. The scalar and massive channels have separate flux observables.

A constant time shift or a comparison Schwarzschild mass shift of order $\epsilon^2$ changes the front waveform through at most $a'$ and the news through at most $a''$. The actual cubic news has a nonzero $a'''$ coefficient. The comparison respects half-period covariance, and the preparation has fixed ADM mass. To retain the template test under smoothing, take a sum of two nearby fronts launched from distinct exterior areal radii $R_i$, with the same fixed clock. The first-order outgoing front cut is $-x_M(R_i)$ for $x_M(r)=r+2G_4 M\log(r/(2G_4M)-1)$. The mass-template offsets $\partial_M[-x_M(R_i)]$ are distinct since $\partial_r x_M=(1-2G_4 M/r)^{-1}$ and $\partial_M\partial_r x_M>0$. The time-template offset is common. The two templates are therefore independent by their two front jumps. The cubic $a'''$ term has a different jump order. The three resulting distributional news functions can be paired against three compact smooth test functions to give an invertible matrix. Continuity of the traces and the mass/time template derivatives under smoothing preserves this rank. This yields compact smooth pulses for which both template fits fail.

The witness is for axial $\ell\ge2$, in particular the requested $\ell=2$, and for the specified fixed-canonical-data preparation. Independent homogeneous mixed or cubic data can change the correction. The polar amplitude and strong-field QNM residues remain separate projections.
