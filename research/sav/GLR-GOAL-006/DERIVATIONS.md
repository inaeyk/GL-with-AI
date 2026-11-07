# Derivations

All radial estimates use $r_0=1$, $L=10$, $k=pi/5$, and the fixed normalization $H_{\theta\theta}(2)/8=1$. A superscript $(j)$ means a coefficient of $\epsilon^j$ without a factorial. Set $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, and $\Sigma=\{v=r\}$. The background ADM mass is $M_0=1/(2G_4)$, $G_4=G_5/L$. Brackets are the normalized coordinate circle average in the displayed chart. Source equations and data are from [GOAL-005](../GLR-GOAL-005/CONSTRAINTS.md), pinned at [61c7b5c](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708). See the [independent Work review](https://github.com/inaeyk/GL-with-AI/commit/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639), especially §§3–6.

## 1. Original constraint family and the tuned mass rate

On $\Sigma$ use $\gamma=a^2dr^2+dz^2+r^2e^{2g}d\omega^2$ with $g$ an exponentially decaying pure first harmonic of order $\epsilon$. Let $U=a^{-2}$, $U_0=r/(r+1)$, $w=r(U-U_0)$, $Q=r^2q$, $J=r^2\eta$, $W=(Q-Q_0,J)$. Put $d=\kappa+q/2$, $d_0=[2r^{3/2}(r+1)^{3/2}]^{-1}$, $v_K=K^z{}_z$. In Z, $v_K=0$ and $d_*=d_0+\epsilon d_1$. The remaining exact free data and inner leaf at $r_i=3/4$ are as in the pinned construction. First harmonic quantities and finitely many of their derivatives obey $C e^{-cr}$ bounds after reducing $c>0$.

At $\lambda=0$, the original radial energy gives $\|w,W\|_{H^m}\lesssim |\epsilon|$, $\|w-\bar w\|_{H^m}\lesssim |\epsilon|r^{-3}$, so $a_z=O_{H^{m-1}}(\epsilon r^{-4})$. Finite-collar smooth dependence gives zero averaged first jet. In (1) of the pinned constraints, integration by parts gives $\overline{aJ_z+2a_zJ}=\overline{a_zJ}$. The terms linear in $g_r$ or $d-d_0$ with background coefficients vanish. Thus
$$
 |\partial_r\overline{Q-Q_0}|\lesssim \epsilon^2(r^{-4}+e^{-cr}),\qquad
 |\overline{Q-Q_0}|\lesssim\epsilon^2.
$$
With $g=0$ the exact matter difference in the mean $w$ equation is the mean of
$$
 \left(d_0-\frac{Q_0}{2r^2}\right)(Q-Q_0)
 -\frac{(Q-Q_0)^2}{4r^2}-\frac{J^2}{r^2}.
$$
Terms linear in the free first harmonic from the other coefficients average to zero. The averaged diffusion is $-\overline{D_z w_z}$, and it and the gradient term cost $O(\epsilon^2(r^{-6}+e^{-cr}))$. Consequently $\bar w'=O(\epsilon^2(r^{-2}+e^{-cr}))$ and $w_\infty(\epsilon,0)=O(\epsilon^2)$.

At zero seed $Q=Q_0$, $J=0$ and $w_\infty=r_i^2(e^{-2\lambda}-1)/(r_i+1)$. This follows by solving $w_r=0$ and evaluating $a=a_0e^\lambda$ at the inner leaf. The ADM integral of the radial $1/r$ coefficient gives $M/M_0=1-w_\infty$. Thus $\partial_\lambda(M/M_0)(0,0)=2r_i^2/(r_i+1)=9/14$. The pinned $C^1$ charge argument (a small parameter-variation loss $r^\gamma$, $\gamma<1$, in an integrable $r^{-2+\gamma}$ mean equation) and the mean value theorem give $|\lambda(\epsilon)|\lesssim\epsilon^2$. Evenness of a $C^1$ map alone would allow $|\epsilon|^{1+\alpha}$, $0<\alpha<1$. Compact-slab mean scalar estimates can use a two-parameter smooth development, even in $\epsilon$ at each $\lambda$. Its zero-seed scalar vanishes for all $\lambda$ and $\epsilon^2$ times a smooth coefficient changes by $O(\epsilon^2|\lambda|)=O(\epsilon^4)$. No radius-uniform estimate follows from this argument.

### A tail in the original constraint class

Set $F_\pm=(Q-Q_0)/\sqrt2\pm J$, $s'=\bar a/\sqrt2$. The original equations give
$$
 \partial_r F_\pm=\pm s'\partial_z F_\pm+O_{H^{m-1}}(r^{-4}+e^{-cr})
$$
on the small ball. The error follows from $a-\bar a,a_z=O_{H^{m-1}}(r^{-4})$ and exponential free terms. Thus $F_\pm(r,z\mp s(r))$ converge in $H^{m-1}$ with $O(r^{-3})$ error. In the pinned chart
$$
 s(r)=\frac{1}{\sqrt2}\left(r+\frac{1-w_\infty}{2}\log r\right)+O(1).
$$
The logarithmic coefficient in this **spatial constraint phase** is fixed by ADM tuning. It is distinct from the spacetime optical logarithm.

For ideal exterior linearization about the background, the Fourier amplitudes at $k_n\ne0$ satisfy
$$
 w'=-\tfrac12 k_n^2(r+1)w-
 \frac{2r+1}{2r^{3/2}(r+1)^{3/2}}\delta Q,\quad
 \delta Q'=ik_n a_0\delta J,\quad
 \delta J'=ik_n\left(\tfrac12a_0\delta Q+\frac{2r+1}{4r^2}w\right).
$$
Choosing $w(A)=0$ and a nonzero reflection-symmetric oscillator at $A$ gives a nonzero demodulated limit for sufficiently large $A$. In fact the oscillator correction relative to its supremum is at most $3\sqrt2/(4|k_n| A^3)$. Integrate the damped $w$ equation, use $D\ge r/2$, $|C|\le3/(2r^2)$ for the forcing coefficient and $|B|\le3/(4r)$ for the $w$ coupling, and exchange the ordered radial integrations. If the resulting bound is below $1/2$, the correction is smaller than the initial oscillator amplitude. See [reproduction](REPRODUCIBILITY.md). Any nonzero limiting momentum profile gives a positive asymptotic circle norm of the pair, since the two translates preserve their individual norms and the diagonalizing map is unitary in the symmetrized norm. The weight $r^{1+2\delta}$ on the square integral of $K$ then costs $\int r^{-1+2\delta}dr$ even for $\delta=0$. These examples establish failure of an implication from the original estimate to stronger weights. The actual profile for the pinned nonlinear Z family has not been evaluated. An exact end-only example follows by starting the ideal nonlinear equations at $A$ with $w=0$, $Q-Q_0=\sqrt2\alpha\cos(k_nz)$ and $J=0$. The pinned energy bounds give an integrable majorant $C r^{-4}$ for the translated forcing divided by $|\alpha|$. Finite-radius differentiability and dominated convergence then show that the limiting profile divided by $\alpha$ converges to the nonzero linear transfer profile. This example has no specified inner GL collar.

## 2. Route A. Damping with the exact Z leaf data

The constraint ansatz can be changed in the exterior while keeping $b=1$, $v_K=0$, the same $g$, the inner leaf, and the first jet. It selects a different higher-order massive preparation. Define $Q_1,J_1,w_1$ as first jets of the **scaled** unknowns. Fix $p>0$ and a smooth radial cutoff $\chi$ equal to zero on a collar and to one in the far exterior. Set
$$
 X=Q-Q_0-\epsilon Q_1,\qquad Y=J-\epsilon J_1,\qquad
 d=d_*+c_d,
$$
$$
 c_d=a^{-1}\left[-\frac{p\chi}{r^3}\partial_z^{-1}Y+b_0(r)\right],\qquad
 b_0=\frac{-p\chi\bar X/(2r^2)+(p\chi/r^3)\overline{a^{-1}\partial_z^{-1}Y}}{\overline{a^{-1}}}.
 \tag{A1}
$$
The inverse derivative is on zero-mean functions and has zero-mean output. Reflection makes $Y$ odd, so it lies in the domain, and $c_d$ is even. The control is equivariant under the half-period shift and seed sign. It has no first jet at $(\epsilon,\lambda)=(0,0)$. At zero seed all members of the spherical $\lambda$ family have $X=Y=0$, so the original mass derivative survives.

The pinned Hamiltonian and momentum equations give the exact changes
$$
 \Delta Z=2c_d q,\qquad
 \Delta Q_r=2r^2(1/r+g_r)c_d,\qquad
 \Delta J_r=r^2\partial_z(a c_d)=-p\chi Y/r.
 \tag{A2}
$$
In particular there is no new derivative in the principal symbol. On bounded collars, the inverse derivative and averaging are smooth bounded maps in the same $H^m$ spaces. Substitution of (A2) into the local parabolic/symmetric-hyperbolic estimates gives local existence, uniqueness and smooth finite-collar dependence as in the pinned construction. Since $\chi=0$ on the inner collar, its constraint data are the original ones at the same $\lambda$. The new tuned $\lambda$ can move the inner lapse at order $\epsilon^2$.

Here are the far-end estimates for a fixed finite $p$ and $m\ge4$. Write $\widetilde f=f-\bar f$ and $e(r)=Ce^{-cr}$, with $c$ reduced as necessary. On the original small ball, (A1) gives
$$
 \bar c_d=-p\chi\bar X/(2r^2),\qquad
 \|\widetilde c_d\|_m\lesssim_p r^{-3}
 (\|Y\|_m+|\bar X|\|\widetilde w\|_m).
$$
The $Q_r$ change is $-p\chi\bar X/r$ plus $O_{H^m,p}(r^{-2}(\|W\|_m+|\epsilon|e))$. The Hamiltonian change $2r c_d q/H_*$ has the same $r^{-2}$ bound, since $Q$ is bounded. The mean $Q$ and the $J$ damping give favorable energy signs after writing $Q-Q_0=X+\epsilon Q_1$ and $J=Y+\epsilon J_1$. Their first-jet cross terms cost an integrable exponential times the energy and $\epsilon^2$. All principal and parabolic commutators retain the estimates of the original system. Gronwall and the projected lapse heat kernel therefore give $\|w,W\|_m\lesssim_p |\epsilon|+|\lambda|$, $\widetilde w=O_{H^m}((|\epsilon|+|\lambda|)r^{-3})$, and $\bar a'=O(r^{-2})$. Start at spare $z$ regularity for any fixed derivative count.

For the improved bound let $\zeta=\widetilde w-\epsilon w_1$ and set $P=\frac12\|\widetilde X\|_m^2+\|Y\|_m^2$, $S_c=(\widetilde X,\partial_z^{-1}Y)_m$. In the far region with $\chi=1$ the principal constant-in-$z$ terms give
$$
 P'\big|_{\rm osc}=-2p\|Y\|_m^2/r,\qquad
 S_c'\big|_{\rm osc}=\bar a(\tfrac12\|\widetilde X\|_m^2-\|Y\|_m^2)-p S_c/r.
$$
The sign follows from $(Y_z,\partial_z^{-1}Y)_m=-\|Y\|_m^2$ and $\partial_z^{-1}\widetilde X_z=\widetilde X$. Thus $P-p S_c/(r\bar a)$ is equivalent to $P$ for large $r$, and its principal derivative is $-pP/r+O_p(r^{-2})P$. The variable-coefficient remainder is bounded without derivative loss. In the first factor of $S_c'$ integrate the $Y_z$ term by parts against $\partial_z^{-1}Y$. In the second use $\partial_z^{-1}P_0(a X_z)=P_0(a X)-\partial_z^{-1}P_0(a_z X)$, with $P_0$ the mean-zero projection. Here $a-\bar a=O_{H^m}(r^{-4})$ and the $a_z$ costs at top order are absorbed by the parabolic term.

In the subtracted system the error at $\zeta=X=Y=0$ is $O_{H^m}((\epsilon^2+|\epsilon\lambda|)e(r))$. The reason is that at $\epsilon=0$ the spherical $w$ integration constant drops out of the projected equation, and its product with a first jet is exponentially small. The original top coupling of size $C r^{-1}\|\zeta_z\|_m(\|X\|_m+\|Y\|_m)$ costs $Cr^{-3}(\|X\|_m^2+\|Y\|_m^2)$ against $cr\|\zeta_z\|_m^2$. The projected Hamiltonian drive is $O_{H^m,p}(r^{-2}(\|X\|_m+\|Y\|_m))$ plus the exponential error. Using Poincaré for $\zeta$ and mean damping for $\bar X$ gives, with $T=\|\zeta\|_m^2+|\bar X|^2/2+P-pS_c/(r\bar a)$,
$$
 T'\le\left[-\frac{p}{r}+C_p r^{-2}+C e(r)\right]T
      +C(\epsilon^2+|\epsilon\lambda|)^2 e(r).
 \tag{A3}
$$
An exponential coefficient can absorb the mixed exponential forcing by Young's inequality. At the fixed start of the far estimate, smooth dependence, the exact first jet at $\lambda=0$, and $X=Y=\zeta=0$ on the zero-seed family give $\sqrt T\lesssim \epsilon^2+|\epsilon\lambda|$. Multiplying (A3) by $r^p$ and integrating proves
$$
 \|\zeta,X,Y\|_m\lesssim_p (\epsilon^2+|\epsilon\lambda|)r^{-p/2}.
$$
The mean lapse equation gives $|\bar w'|\lesssim_p (\epsilon^2+|\epsilon\lambda|)(r^{-2-p/2}+e(r))$. In this estimate the term linear in the mean remainder has coefficient $O(r^{-2})$ and the purely spherical $w$ constant cancels exactly. The integrated $C^1$ mass argument still applies. In parameter variations (A2) has the same bounded lower-order maps and favorable leading damping. The only nonintegrable undamped cost needed is the original small $r^{-1}$ variation of the momentum speed multiplying a bounded spare derivative, so the $r^\gamma$ loss remains available on a small neighborhood. This yields $\lambda=O(\epsilon^2)$ and, at tuned mass, $\bar w=O_p(\epsilon^2r^{-1-p/2})$.

Each fixed number of radial derivatives follows from the equations using spare $z$ derivatives. A crude bound loses at most a fixed power of $r$ per derivative from the parabolic coefficient, so choose $p$ larger before differentiating. For any requested finite weight and derivative order one obtains a smooth constraint end with that order of decay for the **perturbation relative to the original Schwarzschild string slice**. The background has spherical $K_0=O(r^{-2})$ on $\tau=0$. The construction therefore gives neither absolute strong $K$ falloff on this slicing nor a plug-in use of flat-KK stability. Each $p$ gives its own family and smallness range. No uniform claim as $p\to\infty$ is needed.

## 3. Route B. Exact scalar equation and a trace estimate

Use a chart with periodic compact coordinate, no angular mixing and spacelike unit normal to the $z$ leaves. Write
$$
 g_5=b^2dz^2+h_{\mu\nu}(dx^\mu+X^\mu dz)(dx^\nu+X^\nu dz),\quad
 K_z=\frac{\partial_z h-\mathcal L_X h}{2b},\quad T_z=\operatorname{tr}_h K_z .
$$
Here $X$ is a spacetime shift for this **compact** foliation. It is distinct from the radial momentum remainder in §2. Put $D=\sqrt{-\det h}/\sin\theta$, so $D$ includes the squared sphere radius. For the two nonangular leaf coordinates use indices $a,b'$. The normal-normal Ricci equation is
$$
 0=-b^{-1}(\partial_z-X)T_z-K_z:K_z-b^{-1}\Box_h b .
$$
Use $\partial_z D-\partial_a(DX^a)=bD T_z$ and integrate the compact derivative. The exact result is
$$
 \partial_a\langle D h^{ab'}\partial_{b'}b-DX^aT_z\rangle
       =\langle bD(T_z^2-K_z:K_z)\rangle=:\mathcal S.
 \tag{B1}
$$
This is a divergence identity in the chart. It uses the spacelike lapse $b$, while the circle metric is $g_{zz}=b^2+h(X,X)$.

Let $A^{ab'}=\langle D h^{ab'}\rangle$, $B_0=\langle b\rangle$ and
$$
 G^a=\langle(Dh^{ab'}-A^{ab'})\partial_{b'}(b-B_0)-DX^aT_z\rangle .
$$
Suppose $A$ is Lorentzian and has a global two-dimensional double-null chart $(u,v)$ on the early wedge. The residual freedom can label the outgoing and incoming rays by their original $\Sigma$ labels. In these coordinates $A^{uv}=-\sqrt{-\det A}=-\rho^2$, and $A^{uu}=A^{vv}=0$. The density transformation makes $\rho$ a scalar on the quotient. Then $\psi=\sqrt3\rho(B_0-1)/2$ obeys
$$
 \psi_{uv}=V_c\psi+F,qquad V_c=\rho_{uv}/\rho,\qquad
 F=\frac{\sqrt3}{4\rho}(\partial_aG^a-\mathcal S).
 \tag{B2}
$$
At zero seed $\rho=r$, $V_c=-f/(4r^3)$. In a chart with the completed first jet $h_{zA}^{(1)}=0$, $B_0^{(1)}=0$, $X^{(1)}=0$, and $T_z^{(1)}=0$. Thus $G$ has zero second coefficient and $\mathcal S^{(2)}=-fr^2 k^2 e^{2\Omega t}H_{\mu\nu}H^{\mu\nu}/16$. Equation (B2) gives exactly the pinned Z scalar source. The $O(\epsilon^2)$ identification of the mean null labels has no effect on that quadratic source. The chart comparison and optical identification at finite seed are separate hypotheses below.

### A ray norm and the trace lemma

Identify the parameter-dependent mean charts by the fixed labels on $\Sigma$, so $v=v_\Sigma(u)=R(u)$ on the initial curve, $R(u)=g_\Sigma^{-1}(u)$ with $g_\Sigma(r)=-r-2\log(r-1)$. Work on $u\le u_*$ containing a padded benchmark $I'$. Define
$$
 [F](u)=\int_{v_\Sigma(u)}^\infty |F(u,v)|\,dv,qquad
 \|F\|_{\mathcal R}=\|[F]\|_{L^1(( -\infty,u_*])}+\|[F]\|_{L^2(I')}.
$$
Suppose $\|V_{c,\epsilon}\|_{\mathcal R}$ is uniformly bounded and $\|V_{c,\epsilon}-V_{c,0}\|_{\mathcal R}\le C\epsilon^2$. Let $H_\epsilon$ be the solution of $H_{uv}=0$ with the initial jet of $\psi_\epsilon$. Require $\|H_\epsilon-\epsilon^2 H_2\|_\infty+\|\partial_u(H_\epsilon-\epsilon^2 H_2)\|_{L^2(I')}\le C\epsilon^4$, and assume $H_\epsilon$ has a field limit at $v=\infty$. The latter follows, for example, if $\psi_\Sigma$ has a limit at the earlier end and $\partial_u\psi|_\Sigma$ is integrable there. Here $H_2=0$ in Z. If $\|F_\epsilon-\epsilon^2 F_2\|_{\mathcal R}\le C\epsilon^4$, then the mean-chart news exists and satisfies the quartic bound in $L^2(I')$.

Indeed subtract $\epsilon^2\psi_2$ from $\psi_\epsilon$, set $d=\psi_\epsilon-\epsilon^2\psi_2$ and $E=F_\epsilon-\epsilon^2 F_2+\epsilon^2(V_{c,\epsilon}-V_{c,0})\psi_2$. The pinned Volterra estimate bounds $\psi_2$. Integrate $d_{uv}=V_{c,\epsilon}d+E$ in the initial characteristic triangle. With $a(u)=[V_{c,\epsilon}](u)$, the supremum of $|d|$ along each ray obeys
$$
 \|d\|_\infty\le e^{\|a\|_1}
 (\|H_\epsilon-\epsilon^2H_2\|_\infty+\|[E]\|_1).
$$
All estimates hold uniformly on finite truncated triangles first. Dominated convergence then gives the field trace by the double integral and the $L^2(I')$ limit of $\partial_u d$ by the single ray integral. The derivative of the field trace is that $L^2$ limit in distributions. The news bound is
$$
 \|\partial_u d(\cdot,\infty)\|_2\le
 \|\partial_u(H_\epsilon-\epsilon^2H_2)\|_2+\|[E]\|_2
 +\|a\|_2\|d\|_\infty .
 \tag{B3}
$$
The lemma requires no interchange of a seed derivative and an unbounded-radius limit. It also gives a uniform bound on finite-radius derivatives in the same chart. To use these coordinates for an exact development, one must establish existence on the whole wedge, the hypotheses of the lemma, and physical matching of the chart.

### Sufficient massive flux bounds

A ray estimate exploits the extra $1/r$ in the rescaled mean source. In the mean chart pull all variables back to the same labels and use common regular component frames, and let $Z$ denote the collection of $r$ times the oscillatory parts of the metric variables in (B1), including the derivatives through order two needed for $\partial G$. Circle reflection gives $\bar X^a=0$. Smooth mean coefficient fields and their derivatives must remain bounded and differ from their background values by $O(\epsilon^2)$ in the normalized frames. Require $\rho/r=1+O(\epsilon^2)$ and $r\asymp R(u)+v-v_\Sigma(u)$ on the wedge, with $r=r((v-u)/2)$ the background radius at the fixed labels. Split $Z$ into its half-period odd part $Z_o$ and its mean-free half-period even part $Z_e$. On each dyadic radial segment $D\le r\le2D$, $D\ge R=R(u)$, one sufficient hypothesis is
$$
 \int_{D\le r\le 2D}\|Z_o\|_{L^2_z}^2,dv\lesssim \epsilon^2\frac{(D/R)^\delta}{R},\quad
 \int_{D\le r\le 2D}\|Z_e\|_{L^2_z}^2,dv\lesssim \epsilon^4\frac{(D/R)^\delta}{R},\quad
 \int_{D\le r\le 2D}\|Z_o-\epsilon Z_1\|_{L^2_z}^2,dv\lesssim \epsilon^6\frac{(D/R)^\delta}{R}
 \tag{B4}
$$
for a uniform $0\le\delta<1$. Also require pointwise bounded normalized derivatives with order $O(|\epsilon|)$ and $O(\epsilon^2)$ for odd and even oscillatory variables, respectively. The GL first jet obeys the analogous unit-size flux estimate with exponential room.

In (B1) the rescaled quadratic and differentiated cross-current terms are sums of $r^{-1}$ times products of two such rescaled variables with bounded coefficients. Both factors are mean-free, since $T_z$ has no linear mean at a circle-independent background, $X$ is odd under circle reflection, and the first factor of the other cross current is projected. Equivalently, expand in oscillatory variables around the actual mean and average. At quadratic order a product of opposite half-period parities averages to zero, so the error is controlled by odd times odd-remainder and even times even. Cubic products of three odd factors also average to zero, and terms with an even factor use one pointwise factor. Quartic and higher terms are bounded by two flux factors and two pointwise factors. This yields the same bound for all terms in $F_\epsilon-\epsilon^2 F_2$ by circle and ray Cauchy–Schwarz. A dyadic segment costs $C\epsilon^4(D/R)^\delta/(D R)$, and the sum is $C\epsilon^4/R(u)^2$. The bound belongs to $L^1$ on the earlier wedge and $L^2$ on $I'$. A similar estimate gives absolute ray convergence for $F_\epsilon$. The oscillatory flux hypothesis can be weakened to product bounds in the ray norm. (B4) gives a concrete sufficient differentiated energy target for the actual spatial tail.

For comparison, the massive model $4q_{uv}+V(r)q=F_m$ with static $V\ge c>0$ on the far exterior has an outgoing energy flux controlling $\int(|q_v|^2+V|q|^2)dv$, by the $\partial_t$ energy identity. Commute $\partial_t$ to control $q_u=\partial_t q-q_v$ on that ray. On a spacelike initial tail of slope bounded away from null, $q$ and its requisite derivatives $O(r^{-1})$ cost only $O(R^{-1})$ initial energy. The corresponding compact flat model has $V=-\partial_z^2$ and circle Poincaré controls the mean-free field. For the true system the commuted equations include deformation, connection, potential and quasilinear principal-coefficient terms. In an energy proof one must bound those on the evolving cones. For example, a positive symmetrized energy with a deformation bound $a(t)\le\delta/(1+t)$ and integrable source work gives an energy growth factor at most $(1+t)^\delta$ by Gronwall. Relative to initial radius $R$, the relevant bound is $\delta/(R+t)$, with an initial tail energy $O(R^{-1})$, to obtain (B4). The flat flux is a proof of the gain in the model, not a bound on the nonlinear Einstein deformation terms. A single undifferentiated null flux does not control the transverse derivative $q_u$.

## 4. Optical clock and geometric news

Let $r_5$ be the invariant radius of the $\mathrm{SO}(3)$ orbit. With full optical $u_B$ and a spacelike cut $\{u_B,r_5\}$ of the three-dimensional orbit quotient, its circumference $\ell$ is independent of compact coordinate reparametrizations. Define
$$
 \phi_{\rm len}=\frac{\sqrt3}{2}\log(\ell/L),\qquad
 N_\epsilon=\partial_{u_B}\lim_{r_5\to\infty} r_E\phi_{\rm len},
$$
where $r_E/r_5\to1$ in the normalized product end. The normalization agrees with $g_{zz}=e^{4\phi/\sqrt3}$ in the circle-independent case. A periodic flat pullback $z=\zeta+\epsilon\chi(\zeta)$ leaves $\ell=L$ while the raw metric average gains $\epsilon^2\langle\chi'^2\rangle$.

To apply (B3), require that the mean chart cuts approach the physical optical cuts and that the circumference and areal-radius replacements in $\psi$ have vanishing $H^1(I')$ error in the limit. For the shift correction on common-coordinate cuts it suffices that $\rho\,\partial_u^j\langle |X|^2\rangle=o(1)$, $j=0,1$, with bounded coefficients and derivatives. One also needs cut comparison for possible $z$-dependent shifts. A normalized and anchored comparison $u_B=\mathcal T_\epsilon(u)$ with $\|\mathcal T_\epsilon-\mathrm{id}\|_{C^1(I')}\lesssim\epsilon^2$ then suffices. The chain rule and change of variables preserve (B3), and composing the smooth $\epsilon^2 n_Z$ with this reparametrization costs $O(\epsilon^4)$. The effective $A$ chart from (B1) does not furnish the five-dimensional eikonal.

At fixed static $t,r$, Schwarzschild comparison gives $u_{M+\Delta M}-u_M=-2G_4\Delta M\log r+O(1)$. In outgoing background Eddington coordinates the first variation satisfies $\partial_r\delta u=-h_{rr}/2$, with $h_{rr}=4G_4\Delta M/(rf^2)$ for the static mass variation. Fixing ADM mass removes this logarithm. In outgoing Bondi gauge $h_{rr}=0$ even when $m_B(u)$ varies through $h_{uu}$. Substituting $m_B(u)$ into a static tortoise can give a clock Jacobian $1+2G_4m_B'(u)\log r$ with a zero at arbitrarily large radius. Uniform normalization must come from the eikonal and gauge equations.

Two explicit negative controls separate the issues.

- $\epsilon^2 n(u+\epsilon^2\log r)$, with nonzero smooth compactly supported $n$, has the desired Taylor coefficient on every finite tube, but its infinite-radius limit at fixed nonzero seed is zero.
- On a flat product, a genuine outgoing eikonal $u=t-S(r,z)$ with finite-tube data $S(A,z)=A+a(z)$ obeys $S_r=\sqrt{1-S_z^2}$. Put $p=S_z$. Its characteristics are $z=z_0+(r-A)p_0/\sqrt{1-p_0^2}$, $p_0=a'(z_0)$. The Jacobian $1+(r-A)a''(z_0)/(1-p_0^2)^{3/2}$ vanishes for a nonconstant periodic $a$ at some $r>A$. On the generators $t-r=a(z_0)+(r-A)[(1-p_0^2)^{-1/2}-1]$. An arbitrarily small tilt can produce a caustic and secular drift. Physical comparison needs asymptotically zero compact optical momentum or an equivalent controlled eikonal construction.

### Independent optical identity

The verifier started with full optical $u$, areal $r=r_5$, and $z$ transported along generators. Then $g_{rr}=g_{rz}=0$, $X=A_o\partial_r$, $h=-B V_o\,du^2-2B\,du\,dr+r^2d\omega^2$, and $b=\sqrt{g_{zz}}$. Put $H=(B_z-A_o B_r)/B-(A_o)_r$. Direct contraction gives
$$
 T_z=(H-2A_o/r)/b,\qquad K_z:K_z=(H^2/2+2A_o^2/r^2)/b^2.
$$
The $uu$ component of $K_z$ drops out because $(K_z)_{rr}=0$. With $q_b=r(b-1)$ the exact equation is
$$
 -2(q_b)_{ur}+\partial_r(V_o(q_b)_r)-(V_o)_r q_b/r
 =rB[-\partial_zT_z+A_o\partial_rT_z-b K_z:K_z].
$$
The mean ray law follows by integration by parts in $z$:
$$
 \partial_r\left\langle(q_b)_u-\tfrac12 V_o(q_b)_r\right\rangle
 =-\tfrac12\left(\frac{\langle (V_o)_r q_b\rangle}{r}
       +r\langle B_zT_z+B A_o\partial_rT_z-Bb K_z:K_z\rangle\right).
$$
Only compact and outgoing derivatives occur in the quadratic source. This suggests a route to weaker flux hypotheses than (B4). It assumes existence of the full optical chart. Transformed initial cuts can differ from the original constraint leaves, and the completed Z jet must be carried through that transformation.

## 5. Causal massive second harmonic and its energy

Write $K=2k$ and take the $+K$ complex mode with its conjugate understood. Let $Q_{AB}$ be the full quadratic **Ricci** source. In second-harmonic unitary gauge $q_{zA}=0$, the trace and divergence of $q_{\mu\nu}$ are forced:
$$
 T=-2Q_{zz}/K^2,\qquad J_\nu=\partial_\nu T+2iQ_{z\nu}/K.
$$
The causal tensor equation is
$$
 \mathcal D_K q_{\mu\nu}=F_{\mu\nu},\quad
 \mathcal D_K=\bar\Box+2\overline{\mathrm{Riem}}-K^2,\quad
 F=2Q_{\mu\nu}+\bar\nabla_\mu J_\nu+\bar\nabla_\nu J_\mu-\bar\nabla_\mu\bar\nabla_\nu T .
$$
The on-shell five-dimensional source Bianchi identity gives $\bar\nabla\cdot J=\bar\Box T-\operatorname{tr}_4Q+Q_{zz}$. Consequently $\operatorname{tr}_4 F=(\bar\Box-K^2)T$. Taking a divergence, the gradient terms cancel by the other Bianchi component and $\bar\nabla^\mu F_{\mu\nu}=(\bar\Box-K^2)J_\nu$ on this Ricci-flat background. Trace and divergence commute with the Lichnerowicz operator to the corresponding scalar and covector wave operators. Both constraint defects therefore satisfy homogeneous massive equations. Compatible Cauchy data set the defects and their normal derivatives to zero. The actual data are the completed second jet of the chosen exact constraint family in a regular local vacuum development. Changing §2's feedback can change these data.

The background ingoing coordinates are regular at the future horizon and $\tau=v-r$ is a regular spacelike time on $r\ge r_i$. Unitary completion uses algebraic division by $iK$ and regular covariant derivatives, without division by $f$ or a temporal frequency. The source is smooth there because the first mode is future-regular. The normally hyperbolic tensor problem has a causal solution in the future domain of dependence of the collar and end. The background horizon is an interior characteristic surface in this formulation. Any separated particular solution used for comparison must be complemented by the homogeneous solution with the **difference** of the actual and particular Cauchy jets. This difference generally has nonzero data. The causal evolution is defined directly even at zero temporal frequency. [Normally hyperbolic operators and causal support, Bär v2, Example 3.3 and Theorem 3.8](https://arxiv.org/html/1310.0738v2)

A finite-time Sobolev bound can be obtained in a smooth regular frame, with a positive bundle norm on the components. On $0\le\tau\le T_*$, the principal wave energy on $\tau$ slices is uniformly coercive on $r\ge r_i$ using the regular background lapse and spatial geometry. Commute a fixed finite number of regular derivatives, regard frame connections and curvature as lower-order coefficients, and apply the stress identity and Cauchy–Schwarz. This gives
$$
 \mathcal E_j^{1/2}(\tau)\le C_j e^{C_j\tau}
 \left(\mathcal E_j^{1/2}(0)+\int_0^\tau\|F(s)\|_{H^j}\,ds\right).
$$
It can first be proved on domains with causal side boundaries, whose outward energy has nonnegative sign, then exhausted to the end. Use product Cartesian frames near infinity and a regular frame atlas on the inner collar. The $O(r^{-2})$ differentiated spatial tails are square integrable in the measure $r^2 dr\,dz$. Finite initial norms are required also for the completed spacetime jet in the chosen regular gauge. On finite $\tau$ slabs the first GL mode decays exponentially at infinity, and the smoothly reconstructed source has finite norms at each finite derivative order available from the mode and data. Fixed retarded cuts reach unbounded $\tau$ as $r\to\infty$.

For exact signed accounting use the Einstein linearization $\mathcal E=G^{(1)}$ on the five-dimensional product, and $Q_E=Q-\bar g\operatorname{tr}_5 Q/2$. Define a Lagrange current by
$$
 \bar\nabla_a\mathfrak W^a(p,q)=p:\mathcal E[q]-q:\mathcal E[p].
$$
This follows from formal self-adjointness of the second variation of the Einstein–Hilbert action on a vacuum background. Use the real $\pm K$ pair $q$ and the background Killing field $T_0=\partial_t$ to obtain
$$
 \bar\nabla_a\mathfrak W^a(q,\mathcal L_{T_0}q)
 =Q_E:\mathcal L_{T_0}q-q:\mathcal L_{T_0}Q_E .
$$
Integration gives slice storage change plus signed tube and horizon currents equal to the integral of source work in this convention. Boundary terms, gauge and the action normalization must be fixed for a physical energy extraction. The balance includes the forced non-TT field and gives no pointwise positivity for the finite-tube or horizon terms.

In the asymptotic wave zone a source-free TT outgoing polarization at real $|\omega|>K$ has $p=\sqrt{\omega^2-K^2}$ for positive frequency and outgoing phase $p x+(K^2/2p)\log r+O(1)$ in $r_0=1$ units. Its radial derivative tends to $p$. The leading flux for the real TT field is $4\pi r^2/(32\pi G_4)$ times $\langle-\partial_t q^{\rm TT}:\partial_r q^{\rm TT}\rangle$, with the physical field normalization and both compact modes included. For one $+K$ coefficient $P e^{-i\omega t+ipx+\cdots}/r$ plus its conjugate the leading expression is $\omega p\,P:\bar P/(4G_4)$. The polarization contraction is positive in the timelike four-momentum rest frame. The threshold formula presumes regular fixed amplitude and an outgoing propagating component. A finite Fourier window, a near-field piece, and the imaginary-frequency separated particular solution do not give this flux by substitution. A full causal affine lift with quantitative source/data constants and an evaluated timelike energy remain open. [Brito–Cardoso–Pani v3, §IV](https://arxiv.org/html/1304.6725v3) · [Ould El Hadj v2, §V.2.1 and (70)](https://arxiv.org/html/2411.16538v2)
