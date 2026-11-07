# Audit derivations and failure tests

Throughout, the five-dimensional signature is $(-++++)$, $z\sim z+L$, $L/r_0=10$, and $G_4=G_5/L$. The perturbation convention is $g=\bar g+\epsilon h_1+\epsilon^2h_2+\cdots$. The completed $n=\pm1$ tensor has $h_{zA}=0$ and $H_{\theta\theta}(2r_0)/(8r_0^2)=1$, with actual growing normal velocity on $\tau=v-r=0$. We set $r_0=1$ in differential identities unless otherwise noted.

## 1. The areal normalization conflict

### Exact angular vacuum identity

After constructing full optical cuts, transport the compact generator label and use increasing sphere radius $r=r_5$. Write the quotient metric and angular factor as
$$
q=-BV\,d\lambda^2-2B\,d\lambda\,dr+b^2(dz+U\,d\lambda)^2,\qquad g_5=q+r^2d\omega^2.
$$
Here $\lambda$ is the terminal cut label and $U$ the shift. They are distinct from the temporary eikonal rechart in the source. The inverse metric gives $q^{rr}=V/B$, $q^{\lambda r}=-1/B$, $q^{zr}=U/B$ and $\sqrt{-\det q}=Bb$. The warped-product sphere equation is
$$
0=1-\frac VB-\frac r{Bb}[-b_\lambda+(bV)_r+(bU)_z].
$$
Multiplication by $Bb$ yields $(rbV)_r=Bb+rb_\lambda-r(bU)_z$. The compact average uses periodicity, and at fixed $(\lambda,r)$ it has $\langle b\rangle=\ell/L$ exactly. Thus
$$
\partial_r(r\langle bV\rangle)=\langle Bb\rangle+
\partial_\lambda\!\left[r(\langle b\rangle-1)\right]. \tag{A}
$$
This is the angular equation in the source's separate optical areal formulation. The estimates for the terminal transformation make the graph derivatives approach their background values and the compact generator reparametrization approach the identity. Consequently the leading $B$ and $V$ limits in this chart equal those in the effective areal chart when the latter limits exist. In particular $b\to1$ and $\langle Bb\rangle\to B_\infty$. [Pinned sphere equation](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/EXTERIOR.md#L123) · [Pinned graph estimates](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/OPTICAL.md#L114)

Let $\Phi(\lambda)$ denote the limiting geometric length field before the cut-label change. If this field converges distributionally in the cut label and the rescaled trace is bounded as assumed in the comparison, then $r(\langle b\rangle-1)\to2\Phi/\sqrt3$. Integrate (A) over $[R,2R]$, divide by $R$, and test in $\lambda$. The left-hand side tends to $V_\infty$, and the right-hand side tends to $B_\infty+2\partial_\lambda\Phi/\sqrt3$. This uses the field trace and a radial Cesàro average. It avoids differentiating the limit of fixed-radius tube data. With $T'(\lambda)=du_B/d\lambda=B_\infty$ and $N_\epsilon(T(\lambda))=\partial_\lambda\Phi/B_\infty$,
$$
\boxed{\frac{V_\infty(\lambda)}{B_\infty(\lambda)}
 =1+\frac{2}{\sqrt3}N_\epsilon(T(\lambda)).} \tag{B}
$$
In particular the stated $V/B\to1$ together with a length trace gives zero news on the interval. It cannot be used as a sufficient hypothesis for nonzero Z news in the same chart.

### Einstein-radius check and scope of the correction

On a sufficiently regular asymptotically circle-independent end, $g_5=e^{-2\phi/\sqrt3}g_E+e^{4\phi/\sqrt3}dz^2$ to the required leading order. Hence $\rho=r_E=e^{\phi/\sqrt3}r_5$, and $\phi=\Phi(u_B)/\rho+o(\rho^{-1})$ gives
$$
\rho=r_5+\Phi(u_B)/\sqrt3+o(1).
$$
With differentiated Bondi asymptotics, $d\rho=dr_5+(N_\epsilon/\sqrt3)du_B+o(1)$. The leading Einstein metric $-du_B^2-2du_B\,d\rho$ therefore becomes $-(1+2N_\epsilon/\sqrt3)du_B^2-2du_B\,dr_5$ in the five-dimensional sphere radius. The cross coefficient normalizes time, while the leading time-time coefficient receives the order-news term. This also checks the sign in (B). In the local-area variable $R=r_5\sqrt b$, which agrees asymptotically with $\rho$, the usual unit limit on the normalized coefficient is consistent.

The compact antiderivative, small-root Hamiltonian and finite anchor of the source use uniform comparability and derivative bounds on $V$, with the clock using $B_\infty$. Their estimates do not use the numerical limit $V/B=1$. Drop it from the areal optical construction. If a flux theorem is intended, separately specify a differentiated Einstein-frame Bondi expansion in $\rho$, and derive (B) when an areal limit is needed. This qualification leaves the initial-data and fixed-background results intact. [Pinned optical conditions](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/OPTICAL.md#L13)

## 2. Exact damped data and the odd remainder

On the four-dimensional spatial slice use $\gamma=a^2dr^2+dz^2+\mathcal R^2d\omega^2$, with $U=a^{-2}$ in this subsection, $H=\mathcal R_r/\mathcal R$ and $g=\log\mathcal R$. The radial-compact block of mixed extrinsic curvature has entries $(\kappa,\eta/a;a\eta,0)$, the angular eigenvalues are $q/2,q/2$, and $d=\kappa+q/2$. The warped scalar curvature follows from $R_\gamma=R_{a^2dr^2+dz^2}-4\mathcal R^{-1}\Delta\mathcal R-2\mathcal R^{-2}|d\mathcal R|^2+2/\mathcal R^2$. The extrinsic contraction is $(\operatorname{tr}K)^2-\operatorname{tr}K^2=-q^2/2+2dq-2\eta^2$. Taking the two momentum divergences gives
$$
q_r=a\eta_z+2a_z\eta+2ag_z\eta+2H(d-q),\quad
\eta_r=a(d_z+q_z/2+g_zq)+(d-q/2)a_z-2H\eta .
$$
The independent initial-data checker evaluates these constraints directly and rejects a wrong radial momentum coefficient.

The GOAL-006 feedback applied to the residual momentum variables gives $\Delta J_r=-pY/r$ in the oscillatory far block and $\overline{c_d}=-p\overline X/(2r^2)$ on the mean block. For the oscillatory linear variables $(\zeta,X,Y)$ the principal part in the source has a parabolic $\zeta$ equation with diffusion $\asymp r$ and the pair $X_r=a_0Y_z$, $Y_r=(a_0/2)X_z-pY/r$. Using the real $H^m(S^1)$ inner product and mean-zero primitive, define $P=\|X\|_m^2/2+\|Y\|_m^2$ and $S=(X,\partial_z^{-1}Y)_m$. Direct integration by parts gives $P_r=-2p\|Y\|_m^2/r$ and $S_r=a_0(\|X\|_m^2/2-\|Y\|_m^2)-pS/r$ for these principal terms. Thus $P-pS/(ra_0)$ gives $-pP/r$ up to lower orders. Changing the sign loses this damping. Adding $\|\zeta\|_m^2$ yields a coercive energy at sufficiently large $r$. The off-diagonal $\zeta_z$ term has coefficient $O(r^{-1})$ and costs $O(r^{-3})E$ against $r\|\zeta_z\|_m^2$. The other displayed couplings cost $O_p(r^{-2})E$ after using compact Poincaré.

At a fixed collar the untuned solutions are smooth in $(\epsilon,\lambda)$, with $P_o$-covariance. On the spherical family $w_\infty=r_i^2(e^{-2\lambda}-1)/(1+r_i)$ and $M/M_0=1-w_\infty$. At $r_i=3/4$ the mass derivative is $9/14$. The pinned integrable mean equation and the fixed-collar Taylor estimate yield $M(\epsilon,0)-M_0=O(\epsilon^2)$, so the $C^1$ charge map and nonzero derivative give $\lambda=O(\epsilon^2)$. Subtracting the background GL first jet then gives $O(|\epsilon|^3+|\epsilon\lambda|)$ at the collar. Even $C^1$ alone permits $|\epsilon|^{1+\alpha}$, $0<\alpha<1$, and odd $C^1$ alone permits $\epsilon|\epsilon|$. Those regularities alone would be insufficient for the two respective order claims.

Subtract the background linearized operator from the exact far equations for $Y_*=y-\epsilon y_1$. Pure-first-jet quadratic terms are half-period even. Coefficient variations linear in $Y_*$ carry the exponentially decaying GL jet. Quadratic terms in $Y_*$ are bounded by $C_pr^K\|Y_*\|_{H^{m+2}}^2$ using the uniform positive inverse factors in the constraint equations, with $K$ finite for the selected derivatives. The inherited (A3) bound gives the odd forcing estimate
$$
\|f_o\|_m\lesssim_p |\epsilon|^3e^{-cr}+\epsilon^4r^{K-p}.
$$
The weighted energy inequality for $r^{p/2}\sqrt E$ has integrable $O_p(r^{-2})$ self-coupling and integrable weighted forcing when $p/2>K+1$. It gives $\|P_o Y_*\|_m=O(|\epsilon|^3r^{-p/2})$. Radial differentiations use the equations and a finite reserve of compact derivatives and weights. This is a far-end result for the chosen finite damping and regularity. It inherits the existence and mass-map steps of GOAL-006. [GOAL-006 damping derivation](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md#2-route-a-damping-with-the-exact-z-leaf-data) · [GOAL-009 remainder](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/EXTERIOR.md#1-initial-data-with-the-orders-required-by-the-massive-hierarchy)

For the Z jet, set $\beta^z=0$ on the whole initial slice. Then $\gamma_{zz}=1$, $\gamma_{rz}=0$ and $K_{zz}=0$ give $g_{za}=0$ on the slice and $\partial_\tau g_{zz}=-2\alpha K_{zz}+(\mathcal L_\beta\gamma)_{zz}=0$. The Schur complement $b^2=g_{zz}-g_{za}h^{ab}g_{bz}$ gives $b=1$ and $db=0$. This is invariant under changes keeping the compact coordinate $z$. For the averaged scalar one must still use a common initial curve, as obtained by an areal rechart keeping $\tau,z$ followed by a common mean-null rechart. A compact coordinate transported in a new optical construction can change this lapse jet. Circumference uses the cut tangent in any coordinate.

## 3. Causal massive tensor and weighted background estimate

For the $K=2k$ Fourier mode, let $Q_{AB}$ be the full quadratic Ricci coefficient in the completed first-order gauge. Nonzero $K$ allows completion of $h_2$ to $h_{2,zA}=0$ by an algebraic inversion in $z$ and an orbit derivative. For $E=\delta\operatorname{Ric}[h_2]+Q$, its $zz$ and $z\mu$ components give $T=-2Q_{zz}/K^2$ and $J=dT+2iQ_z/K$, where $T$ and $J$ are the prescribed trace and divergence. The source for the wave operator $\Box_4+2\bar{\operatorname{Riem}}-K^2$ on the remaining tensor is $2Q_4+2\nabla_{(\mu}J_{\nu)}-\nabla_\mu\nabla_\nu T$. The contracted Bianchi equation for $Q$ follows from the first-order vacuum equations and fixes the trace of that source to $(\Box_4-K^2)T$. Once the trace defect is zero, with divergence defect $C$ one has $E_{\mu\nu}=\nabla_{(\mu}C_{\nu)}$, $E_{z\mu}=iK C_\mu/2$, $E_{zz}=0$. The residual Bianchi identity then gives $(\Box_4-K^2)C=0$. The vanishing defect Cauchy jets come from differentiating the locally constrained vacuum development. This mechanism preserves the complete forced system and its finite-slice homogeneous response.

The rescaled static orthonormal components $q=r h_2$ satisfy $q_{tt}-q_{xx}+fK^2q=\mathsf A q_t+\mathsf B q_x+\mathsf C q+S$, with stationary $O(r^{-2})$ lower coefficients. The rescaling removes the scalar spherical radial divergence, the connection terms give at most $O(r^{-2})$ in the frame, and curvature is $O(r^{-3})$. The true quadratic forcing uses the GL tail $e^{\Omega t-\sqrt{k^2+\Omega^2}\,r}$ with its polynomial factors. On $u=t-x$ fixed its exponential rate per first-order factor is $\sqrt{k^2+\Omega^2}-\Omega\simeq0.568216$ in $r_0=1$ units. Early $u$ gives an additional $e^{\Omega u}$ per factor. The exact $n=\pm2$ initial coefficient inherits the finite weighted data bounds by dividing (A3) by $\epsilon^2$ and using the fixed-$\lambda$ Taylor coefficient. A separated particular solution would in general alter those data.

For $y=\partial_t^j q$, $e_j=(|y_t|^2+|y_x|^2+fK^2|y|^2)/2$, $j_j=-y_t\cdot y_x$, the $\tau$-graph slope is $-1/r$ and graph flux $e_j+j_j/r>0$ for nonzero energy. The outward outgoing and incoming flux densities are $e_j-j_j$ and $e_j+j_j$. On a subwedge $u\le u_1$ one has $r\ge R(u_1)+c_A\tau$, with $R$ the slice intersection. The $O(r^{-2})$ cost in time is integrable. With $w=r^p(1-u)^b$, differentiating gives a positive bulk $b r^p(1-u)^{b-1}(e_j-j_j)$ and the unsigned term $pf r^{p-1}(1-u)^b j_j$ on the source side. Since
$$
e_j-j_j=\tfrac12(|y_t+y_x|^2+fK^2|y|^2),\qquad
|j_j|\lesssim (e_j-j_j)+(e_{j+1}-j_{j+1}),
$$
the unsigned term is controlled by the $(p-1,b+1)$ bulk with one extra time derivative. Weighted Cauchy–Schwarz costs $\int wr^2|\partial_t^jS|^2\,dt\,dx$ and another $O(r^{-2})$ energy factor. Finite induction in $p$, integrable Gronwall and nonnegative null boundaries give the uniform estimate through arbitrary finite truncations. One further time commutation controls the incoming derivative, and the equation gives higher mixed derivatives. On $D\le r\le2D$ the trace of $T^jq$ is bounded in squared $L^2(dx)$ by $C D^{-p}R(u)^{-b}$. The positive $fK^2$ mass here is independent of any claim about sign of the scalar master potential near the horizon. [Pinned weighted identity and induction](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/DERIVATIONS.md#L24)

Three independent controls isolate proof hypotheses.

- At $K=0$ the free $q=a(u)$ with compact smooth early support has finite initial weighted norms and $L_+q=0$. It has divergent weighted field trace on any ray where $a(u)\ne0$.
- For a complex massive plane wave let $\omega/K=(s+s^{-1})/2$, $p_x/K=(s-s^{-1})/2$. The transverse time-derivative density divided by $|L_+q|^2+K^2|q|^2$ is $(s^2+1)/4$. Frequency-uniform transverse control requires the spare derivative or another argument.
- For a slightly widened local cone $g=-dt^2+a^2dx^2+dz^2$, take $a=99/100$, $K=1$, $\omega=41/9$ and $p_x=40/9$. The exact real wave $\cos(\omega t-a p_x x+z)$ has $\omega^2-p_x^2=1$. At a zero of the field the local component energy current has $e-j=(\omega^2+p_x^2)/2-\omega p_x/a=-3181/16038$ at the background boundary $u=t-x$. Thus arbitrarily small principal perturbations do not preserve that boundary sign. This is a principal-symbol test, separate from the physical vacuum family.

The exact reduced Einstein equation introduces coefficient commutators $[\mathcal L_T^j,g^{AB}]D_AD_Bh$ and the compact projection of $g^{AB}D_AD_Bh$. The fixed background time-commutation and weights cannot control these terms or the physical cone deformation. The frozen estimate therefore supplies an actual perturbative coefficient on the whole reference wedge, while uniform seed dependence of the exact development remains open.


## 4. Compact scalar source, optical identities and finite-anchor tilt

For the compact-normal split $g_5=h_{ab}(dx^a+X^a dz)(dx^b+X^b dz)+b^2 dz^2$, let $n=b^{-1}(\partial_z-X)$, $K_z=\mathcal L_nh/2$, $T_z=\operatorname{tr}K_z$ and $D=\sqrt{-\det h}/\sin\theta$. Normal acceleration is $-\nabla_h\log b$. Its five-dimensional divergence equals $-b^{-1}\Box_h b$, including the acceleration-square correction. The contracted Ricci identity is $\mathrm{Ric}_{nn}=-n(T_z)-K_z:K_z-b^{-1}\Box_h b$. The density identity $\partial_z D-\partial_a(DX^a)=bDT_z$ then gives
$$
\partial_a(Dh^{ab}\partial_b b-DX^aT_z)+\partial_z(DT_z)
=bD(T_z^2-K_z:K_z-\mathrm{Ric}_{nn}).
$$
Averaging uses periodicity. At first order the completed gauge has $b_1=X_1=T_{z,1}=0$. The harmonic product gives $\langle K_{z,1}:K_{z,1}\rangle=(k^2/8)e^{2\Omega t}H_{\mu\nu}H^{\mu\nu}$, with the $1/8$ from the derivative of the metric divided by two and averaging $\sin^2$. The contraction is Lorentzian and its sign cannot be inferred from a Euclidean norm. Source projection and curvature scattering are both needed for the physical quadratic waveform.

In the common mean-null chart $A^{uv}=-\rho^2$, with $A^{ab}=\langle Dh^{ab}\rangle$, set $\psi=\sqrt3\,\rho(\langle b\rangle-1)/2$. The divergence reduces exactly to
$$
\psi_{uv}=(\rho_{uv}/\rho)\psi+
\frac{\sqrt3}{4\rho}\big[\partial_a G^a-\langle bD(T_z^2-K_z:K_z)\rangle\big],
$$
where $G^a=\langle(Dh^{ab}-A^{ab})\partial_b(b-\langle b\rangle)-DX^aT_z\rangle$. On the reference background $\rho=r$, $r_u=-f/2$, $r_v=f/2$, so $\rho_{uv}/\rho=-f/(4r^3)$ and the source is $F_2=\sqrt3\, k^2rf e^{2\Omega t} H:H/64$. The absolute potential ray integral is $1/(4R(u)^2)$ since $dv=2dr/f$. Integrating in $u$ from $-\infty$ to $g(A)=-A-2\log(A-1)$ gives $\tfrac14[2\log(A/(A-1))-1/A]$. The boundary data are the completed zero jet on the finite tilted slice. This reconstructs the Volterra problem underlying the inherited enclosure. [Pinned GOAL-005 waveform](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/RADIATION.md)

For the error $d_\psi=\psi_\epsilon-\epsilon^2\psi_2$, the forcing is $F_\epsilon-\epsilon^2F_2+\epsilon^2(V_\epsilon-V_0)\psi_2$, with $V_\epsilon=\rho_{uv}/\rho$ in this paragraph. Integrating in $v$ then $u$ bounds $\|d_\psi\|_\infty$ by the data and $L^1_u$ absolute ray forcing multiplied by $\exp(\|[V_\epsilon]\|_{L^1_u})$, where $[F]=\int|F|dv$. One integration along each outgoing ray bounds the $L^2_u$ derivative by its free data, $\|[F]\|_{L^2_u}$ and $\|[V_\epsilon]\|_{L^2_u}\|d_\psi\|_\infty$. The needed quartic forcing and potential comparison concern the exact mean geometry and compact modes. The fixed-background estimate controls their leading jets only. An equivariant half-translation maps the two exact developments and their geometric cuts when uniqueness and normalization are available. Evenness alone allows an $|\epsilon|^3$ remainder in the limiting trace.

### Signed eikonal improvement and local optical evolution

With the true sphere radius held fixed, linearize the warped $rr$ Ricci equation around $q_0=-fdu^2-2du\,dr+dz^2$. Write $a=Pq_{rr}$, $c=Pq_{rz}$, $d=Pq_{zz}$ for the compact mean-free components. Direct variation yields
$$
\delta R_{rr}=c_{rz}-\tfrac12 a_{zz}-\tfrac12 d_{rr}-\tfrac12 f''a+
 r^{-1}(fa_r+a_u-2\partial_r s_{ur}).
$$
The effective null-density condition removes the linear mean of $q_{rr}$. On the controlled nonlinear end the remaining terms in the projected Ricci equation are bounded by $O(r^{-1}d_{j+1}+d_{j+2}^2)$ with the source's commuted size $d_j$. With the zero-mean primitive $\partial_z^{-1}$, solve for $a=E_r+G$, $E=2\partial_z^{-1}c-\partial_z^{-2}d_r$. The (O1) dyadic bounds imply $\int_D^{2D}d_5\,dr\lesssim|\epsilon|D^{(\delta-1)/2}$ and $\int_D^{2D}r d_5^2\,dr\lesssim\epsilon^2D^{\delta-1}$. Thus the first radial moment of $G$ is integrable for $\delta<1$. The signed $E_r$ rechart $u\mapsto u-E/(2B)$ leaves the small inverse-null defect $-G/B^2-E B_r/B^3+O(d_2^2)$. Its first moment and the integrability of the compact transport coefficients bound the backward characteristic Jacobian. The nonvacuum $a=\epsilon r^{-\nu}\cos(kz)$, $3/2<\nu<2$, satisfies the raw shell test with $\delta=0$ but lacks that first moment. A reflected characteristic obeys an oscillator comparison with a zero Jacobian within $O(\epsilon^{-1/2}T^{\nu/2})=o(T)$ from a remote terminal section $T$. The control isolates the signed vacuum gain. [Pinned terminal proof and negative control](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/OPTICAL.md#L157)

Independently, the true-radius optical linear equations for $W=r\,\delta\log b$ and $C=U_r+(\delta\log B)_z$ give $\delta\log B=W_r/2-W/r$ with zero terminal lapse correction, and $(r^2C)_r=2rW_{rz}-4W_z$. Changing to $R=r\sqrt b$ gives local-area variables $A=-U-W_z/2$, $P=-C+2W_z/r$ at linear order, hence $(R^2P)_R=6W_z$. The compact part of the scalar equation becomes $W_{zz}-2A_z-RP_z$. With zero terminal torsion and shift, the integrations give
$$
P=-6R^{-2}\int_R^\infty W_z(s)\,ds,\quad
A=6R^{-1}\int_R^\infty W_z(s)\,ds-3\int_R^\infty W_z(s)\,\frac{ds}{s}.
$$
Substitution gives $(I-6[D(D+1)]^{-1})W_{zz}$ for $D=R\partial_R$. With $F(D)=D(D-1)/[(D+2)(D+3)]$ and $Z=F(D-1)W$, the exact rational identities for the compact, radial and mass-curvature terms give
$$
-2Z_{uR}+(fZ_R)_R+Z_{zz}
-(6/R^2-11/R^3)Z-(24/R^3)[D(D-2)]^{-1}Z=0.
$$
The factor $(D-\lambda)^{-1}$ is the terminal integral $-R^\lambda\int_R^\infty s^{-\lambda-1}h(s)\,ds$, bounded on $L^2(R^p dR)$ for $(p+1)/2+\lambda>0$. Both $F(D-1)$ and its inverse are bounded for $p>5$. The compact operator without the transform kills the radial power $R^{-3}$, outside that space. The symmetric part of a coercive compact coefficient does not itself give a commuted energy for the non-self-adjoint terminal operator. The transformed local principal part still requires estimates on variable coefficients and compatible terminal constants. [Pinned operator proof](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/DERIVATIONS.md#L380)

After imposing $R_{rr}=R_{rz}=R_{zz}=R_{\theta\theta}=0$, the radial Bianchi component forces $R_{ur}=0$ where the outgoing expansion is nonzero. With $Y=R_{uz}$ and $Z_0=R_{uu}$, the others give $(br^2Y)_r=0$ and $[br^2(Z_0-UY)]_r=\partial_z(Br^2Y/b)$. These fix propagation given supplementary boundary data. Exact Vaidya product data $b=B=1$, $U=0$, $V=1-\mu(u)/r$ solve the four main equations but have $R_{uu}=-\mu'(u)/r^2$. Thus even $O(r^{-2})$ decay and normalized leading metric are insufficient for the supplementary equations. Zero residual on one section or the appropriate $o(r^{-2})$ decay suffices in the displayed propagation system. The radial hierarchy also leaves the $1/R$ mass aspect and torsion constants. End compatibility cannot be imposed freely alongside the Cauchy constraints. [Pinned supplementary system](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/EXTERIOR.md#L170)

### Length and matching

For an optical graph $u=f(r,z;\lambda)$, the circumference uses $q_{zz}+2q_{uz}f_z+q_{uu}f_z^2$ under the square root. A periodic coordinate change on a flat product $z_{\rm old}=z+\epsilon a(u)r^{-1/2}\sin(kz)/k$ preserves circumference. Its raw metric average is $1+\epsilon^2a(u)^2/(2r)$ and can manufacture a false scalar coefficient even in the limit. This example lies outside the stronger (O1) class. In that class the Schur correction is quadratic and the graph displacement gives $r$ times the difference of $\ell/L$ and the averaged compact lapse tending to zero in $C^1_\lambda$. This matches limiting fields, leaving strong fixed-tube derivative convergence as a separate gate.

For a completed first-order GL jet, $U_1=\frac12\int_r^\infty h_{rr}^{(u,r)}(u,s,z)\,ds$. At fixed full optical label and fixed invariant radius the first-order displacements obey $\xi^r=-\delta r_5$, $\xi^t=f^{-1}\xi^r-U_1$. The background contribution to the tangent square is $2\xi^r_z(U_1)_z-f(U_1)_z^2$, leading to
$$
(\ell/L)_2=\tfrac12\langle h_{2,zz}\rangle+
\langle \xi^r_z(U_1)_z\rangle-\tfrac f2\langle(U_1)_z^2\rangle .
$$
Using the pinned GL tail with $Y=e^{\Omega t}U_{\rm GL}$ and $\Delta=\sqrt{k^2+\Omega^2}-\Omega$, the leading terms are $\xi^r=Y\cos(kz)/4$ and $U_1=-\Delta Y\cos(kz)/(2k^2r)$. The mixed term is $-\Delta Y^2/(16r)$ to leading order. The square term is lower by one radial power. The first term vanishes at the background crossing of the Z slice. The sign is therefore negative for sufficiently early crossings. The calculation samples a local smooth extension of the slice and does not equal the intrinsic intersection length on $\Sigma$. Both tilt terms vanish exponentially after rescaling by $r$ on fixed outgoing rays. This is a quadratic coefficient comparison, with no uniform Taylor error at infinity for the exact family. [Pinned tail matching](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/DERIVATIONS.md#L232)

## 5. Flux, conservation and the error estimate

The circle-independent reduced action is $(16\pi G_4)^{-1}\int\sqrt{-g_E}[R_E-2|\nabla\phi|^2]$ when the mean vector is absent. It gives $T_{uu}\sim N_\epsilon^2/(4\pi G_4\rho^2)$. For spherical Einstein-frame Bondi asymptotics, the mass equation gives $dM_B/du_B=-N_\epsilon^2/G_4$, since $T_{u_B}{}^\rho\sim-T_{u_Bu_B}$. Independently the five-dimensional transverse strain entries are $(-2,-2,4)\phi/\sqrt3$, with vanishing trace and squared news coefficient $8N_\epsilon^2$. The area factor $4\pi L$ and gravitational coefficient $1/(32\pi G_5)$ again give $N_\epsilon^2/G_4$. Exact $\mathrm{SO}(3)$ removes the trace-free sphere tensor and compact reflection removes the mean vector. Both flux routes require the appropriate differentiated circle-independent end, and length-field equality alone is insufficient. [Primary higher-dimensional news and flux, (4.46), (5.28) and §6.4](https://arxiv.org/html/2109.11599v3)

If $u_B=T(u)=u+O_{C^1}(\epsilon^2)$ on a padded interval, $N_B(T(u))=\partial_u\Phi/T'$. The Jacobian is bounded in both directions, and the shift of the inherited smooth $n_Z$ times $\epsilon^2$ is $O(\epsilon^4)$ by its first derivative bound. The time-dependent mass inserted into a static tortoise would instead induce a secular $O(\epsilon^2\log r)$ comparison error. On a finite tube $v=v_R(u)$ the derivative is $\partial_u+v_R'\partial_v$. The free tail $\epsilon^4\sin(v^2)/v$ has zero field and fixed-$v$ derivative limits but nonvanishing tube-derivative norm at fixed nonzero seed. A uniform radius estimate is essential.

Writing $\hat u=u_B/r_0$, $M_0=r_0/(2G_4)$ and $b_Z=\|n_Z\|_{L^2(d\hat u)}$, one has $E_I/M_0=2\|N_\epsilon\|^2_{L^2(d\hat u)}$ under the flux law. If the target error holds in these units, then
$$
2[\max(0,\epsilon^2 b_Z-C_I\epsilon^4)]^2
\le E_I/M_0\le2(\epsilon^2b_Z+C_I\epsilon^4)^2,\qquad
|E_I/M_0-\epsilon^4\mathcal K_Z|\le4b_Z C_I|\epsilon|^6+2C_I^2\epsilon^8 .
$$
Positivity holds conditionally for $0<\epsilon^2<b_Z/C_I$ within the proven exterior range. The coefficient $\mathcal K_Z=2b_Z^2$ is specific to the cut window and preparation. The pinned interval output encloses it on $[-10,-8]$ and reports an absolute potential correction to news below $7.692\times10^{-6}$ and source tail below $1.223\times10^{-18}$. These are quadratic certificate errors, distinct from $C_I$ for the exact family. [Pinned interval output](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/compute/early_interval_results.json) · [Prior independent interval audit](https://github.com/inaeyk/GL-with-AI/blob/a9bfbec0efe41b881e4b12aa262f5fcd94c639/research/sav/verification/AUDIT_GOAL005_FINITE_SLICE_61c7b5c2.md#4-causal-waveform-cancellation-and-energy)

For a nonzero KK harmonic $m_n=|2\pi n/L|$, real-frequency outgoing propagation requires $p_r^2=\omega^2-m_n^2>0$ and has group velocity $p_r/\omega<1$. Subthreshold fields are evanescent. An exponentially growing GL jet is not a real-frequency propagating polarization. The massive physical channel is a timelike-tube flux with signed work, storage and horizon terms for a forced field. Its wave-zone radiation requires projection onto outgoing real-frequency polarizations. Suppression at null infinity for a broadband massive field needs differentiated estimates, as $p_r/\omega$ approaches one at high frequency. The auxiliary characteristic flux in §3 is positive for an energy estimate and has a different role from any physical escaping massive flux. [Pinned prior signed balance and wave-zone qualifications](https://github.com/inaeyk/GL-with-AI/blob/c4cac8f2e2dd12cfbdf50651863d2188a75fc87c/research/sav/verification/AUDIT_GOAL006_80f030df.md#5-causal-massive-response-and-timelike-accounting)
