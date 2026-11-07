# Schwarzschild-relative exterior and massive estimates

The source is the exact damped Z data and the completed GL first jet in [GOAL-006 §§2–3, 5](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md), with [independent historical audit §§2–5](https://github.com/inaeyk/GL-with-AI/blob/c4cac8f2e2dd12cfbdf50651863d2188a75fc87c/research/sav/verification/AUDIT_GOAL006_80f030df.md). Set $r_0=1$ in this file. All weighted derivative counts and the damping $p$ are finite. The results on initial data and on the background causal second jet concern this family. The optical identities are exact wherever their coordinate chart exists.

## 1. Initial data with the orders required by the massive hierarchy

Write $P_o=(1-\mathcal T)/2$, $P_e=(1+\mathcal T)/2-P_0$, where $\mathcal T$ is translation by $L/2$ and $P_0$ is circle averaging. Use the scaled GOAL-006 unknowns $y=(w,Q-Q_0,J)$, their exact linear jet $y_1$, and $Y_*=y-\epsilon y_1$. The damping rule acts on the last two components of $Y_*$, including the mean of its $Q$ component. With ADM mass tuned, (A3) and its mean lapse estimate give
$$
 \|Y_*\|_{H^m_z}\le C_p\epsilon^2r^{-p/2},\qquad
 |\bar w|\le C_p\epsilon^2 r^{-1-p/2}.
$$
The first jet has vanishing mean. For sufficiently large fixed $p$ one also has
$$
 \|P_oY_*\|_{H^m_z}\le C_p|\epsilon|^3 r^{-p/2}.
 \tag{X1}
$$
Here and below constants depend on the chosen data, damping, radius and derivative counts.

To see this, substitute $y=\epsilon y_1+Y_*$ into the exact Hamiltonian and momentum equations and the control (A1). Denote the linear homogeneous operator in $Y_*$ at zero seed and background by $L_p$. In the far region where the cutoff equals one, its mean-free block on $(\zeta,X,Y)$ is
$$
\begin{aligned}
 \zeta_r&=D_0\zeta_{zz}+(d_0-Q_0/(2r^2))X
               -pQ_0a_0^{-1}r^{-3}\partial_z^{-1}Y,\\
 X_r&=a_0Y_z-2pa_0^{-1}r^{-2}\partial_z^{-1}Y,\\
 Y_r&=a_0 X_z/2-pY/r
           -\frac{a_0^3}{2r}(r^2d_0-Q_0/2)\zeta_z,
 \qquad D_0=(2U_0/r)^{-1}.
\end{aligned}
$$
For example the Hamiltonian coefficient of $X$ follows by multiplying the variation of $-Q^2/(2r^4)+2dQ/r^2$ by $r^2/2$. Its $d$-control contribution is $Q_0c_d$. The $a_z$ terms in the $J$ equation combine to $(r^2d_0-Q_0/2)a_z$. This block commutes with $P_o$.

The nonlinear right-hand side after subtracting $L_pY_*$ has three pieces. Its value at $Y_*=0$ has odd part bounded by $C_p|\epsilon|^3e^{-cr}$, since the constant and first terms cancel, the second term is half-period even, and the free GL profiles and their jets decay exponentially. The part linear in $Y_*$ has coefficients $O_p(|\epsilon| r^K e^{-cr})$ because it is the difference between the linearizations on the first-jet data and the background. The rest has an $H^m_z$ bound $C_pr^K\|Y_*\|_{H^{m+2}_z}^2$. These bounds follow from the displayed GOAL-005 constraint equations (1) and GOAL-006 control (A1). Their highest derivative of an unknown is $w_{zz}$. All undifferentiated coefficients are formed by products and smooth inverses of $U$, $rH_*$ and $\overline{a^{-1}}$, which are uniformly separated from zero. Explicit powers of $r$ are bounded by a fixed polynomial, also after differentiating in the unknowns twice. The inverse circle derivative is bounded on the mean-free Sobolev spaces. Thus a finite $K$ independent of $p$ suffices, with constants depending on $p$. Spare Sobolev derivatives in (A3) justify these tame bounds. Projection yields forcing $f_o$ in the $L_p$ equation with
$$
 \|f_o\|_{H^m_z}\le C_p(|\epsilon|^3 e^{-c' r}+\epsilon^4r^{K-p}).
$$
On the fixed starting collar, smooth dependence on $(\epsilon,\lambda)$, half-period covariance at fixed $\lambda$, and $\lambda=O(\epsilon^2)$ give odd error $O(|\epsilon|^3+|\epsilon\lambda|)$ without invoking a third derivative of the tuned family.

Set $P=\|X\|_m^2/2+\|Y\|_m^2$ and $S=(X,\partial_z^{-1}Y)_m$. In the odd block the energy $E=\|\zeta\|_m^2+P-pS/(ra_0)$ is positive and equivalent to the square norm for large $r$. The constant-in-$z$ principal pair gives $-pP/r$ in the derivative of the corrected energy by periodic integration by parts. The derivative of the correction coefficient, the $O(r^{-2})$ term in $X_r$ and the remaining cross terms without $\zeta_z$ cost $O_p(r^{-2})E$ after absorbing the $O(r^{-2}) X$ forcing in the parabolic energy. The $O(r^{-1})\zeta_z$ coupling costs $O_p(r^{-3})E$ against $c r\|\zeta_z\|_m^2$. Poincaré controls the $\zeta$ energy with room for $-p\|\zeta\|_m^2/r$. The forcing contributes $C_p\sqrt E\|f_o\|_m$. Integrating the resulting inequality for $r^{p/2}\sqrt E$ proves (X1) when $p/2>K+1$. The inequality for the square root can be justified by regularization. A prescribed finite number of radial derivatives follows from the differential equations using spare circle derivatives, at most fixed polynomial losses. Choosing $p$ beforehand gives arbitrary prescribed finite weights. This proves the three initial seed orders, with the $O(|\epsilon|)$ first jet exponentially decaying and the mean-free even field $O(\epsilon^2)$. Conversion to the physical metric and extrinsic curvature uses the same smooth local products and the background coefficients. All estimates are relative to the Schwarzschild slice, whose own $K_0$ has $r^{-2}$ decay.

### An exact zero free scalar jet

Choose evolution coordinates on $\Sigma$ with $\beta^z=0$ and any smooth time lapse and radial shift preserving the first-order completed gauge. On $\Sigma$ the induced metric has $\gamma_{zz}=1$, $\gamma_{rz}=0$ and $K_{zz}=0$. Thus $g_{az}=0$ for $a=\tau,r$ and
$$
 \partial_\tau g_{zz}=-2\alpha K_{zz}+(\mathcal L_\beta\gamma)_{zz}=0.
$$
The Lie derivative vanishes since $\beta^z$ vanishes identically on the slice, $\gamma_{rz}=0$, and $\gamma_{zz}=1$. In the compact-normal split $g_{zz}=b^2+h(X,X)$ this gives $X=0$, $b=1$ and $db=0$ on $\Sigma$. In any $z$-independent mean double-null relabeling the initial jet of $\psi=\sqrt3\rho(\langle b\rangle-1)/2$ is therefore exactly zero. This supplies the free-Jet hypothesis of the mean-chart Volterra estimate on any region where this gauge and chart exist. Full optical coordinates may be $z$-dependent and require separate transformation of the jet.

## 2. Uniform background ray estimate for the actual causal second jet

The massive source is the complete forced Lichnerowicz equation in [GOAL-006 §5](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md#5-causal-massive-second-harmonic-and-its-energy), with $K=2k$ and its forced trace and divergence. On the exterior $r\ge A>2$, use the static orthonormal frame $(f^{-1/2}\partial_t,f^{1/2}\partial_r,r^{-1}\widehat e_A)$ and multiply each independent SO(3)-invariant tensor component by $r$ to form a vector $q$. Use a positive Euclidean norm of these components. The scalar Schwarzschild wave principal part in tortoise $x$ is $f^{-1}(-\partial_t^2+\partial_x^2)+(2/r)\partial_x$ on rotational scalars. Radial rescaling eliminates its first radial derivative and contributes the potential $ff'/r$. In the radial frame, the radial connection of the static basis vanishes, the time connection is $O(r^{-2})$, and angular connections on rotationally invariant tensor components have size $O(r^{-1})$. Two angular derivatives yield zero-order terms of size $O(r^{-2})$ including the connection products and the intrinsic angular action on the invariant tensors. The wave-connection term differentiating the component scalars radially is precisely the scalar divergence term already removed by the rescaling. The Riemann action is $O(r^{-3})$. Hence
$$
 q_{tt}-q_{xx}+fK^2 q=A_t(r)q_t+A_x(r)q_x+C(r)q+S,
 \quad |A_t|+|A_x|+|C|\le C_A r^{-2}.
 \tag{X2}
$$
The coefficients are stationary, and no positivity of the Schwarzschild master potential near the horizon is used. $S$ is $-rf$ times the source components with the sign corresponding to (X2). The GL first jet and all fixed finite derivatives obey an exponential bound on $u=t-x\le g(A)$ because the time growth $e^{\Omega t}$ competes with the radial rate $\sqrt{k^2+\Omega^2}$, with positive difference. The quadratic source, including its differentiated trace and divergence terms, inherits an exponential bound on this wedge. More precisely the separated factor gives additional $e^{2\Omega u}$ on early rays.

The mean-free half-period-even second coefficient of the data is well defined. At fixed $\lambda$, the projected data vanish at zero seed because that family is spherical, and their first seed derivative vanishes by covariance under half translation. The quadratic coefficient has Fourier support only in $\pm 2k$, since the second-variation source is a product of first jets with modes $\pm k$, the linear operator is circle-independent, and the imposed inner data have no other quadratic nonzero mode. Taylor expansion at fixed $\lambda$, followed by $\lambda(\epsilon)\to0$, therefore identifies the coefficient with half the second derivative at the background at fixed $\lambda=0$. Smooth local dependence, the uniform damped bound divided by $\epsilon^2$, and spare derivatives give the weighted jet. The conversion to a regular Cauchy spacetime jet can use the relative wave gauge below, whose initial coefficient derivatives are solved algebraically from its gauge equations, the constraints and the evolution equations. At second order, $K\ne0$ permits unitary completion by division by $iK$ and a finite number of derivatives. On the far exterior all these maps preserve arbitrarily prescribed finite weights after choosing $p$ large enough. Constraint compatibility is inherited from the local exact development. The linear causal solution then extends through every finite truncation by normally hyperbolic theory, and the following estimate is uniform in the truncation.

Let $u_A=g(A)$, $\sigma=\tau$, and use $\{\sigma\ge0,\ u\le u_A\}$. On $\sigma$ slices $t=\sigma-x+r$ the slope is $-1/r$. At fixed $u$ one has $dr/d\sigma=f/(2-f)$, so the exterior lower radius is comparable to $A+\sigma$. With $e=(|q_t|^2+|q_x|^2+fK^2|q|^2)/2$, $J=-q_t\cdot q_x$, the graph energy $e-t_xJ$ is positive and equivalent to $e$. The outgoing boundary flux is $e-J=(|q_t+q_x|^2+fK^2|q|^2)/2$ with measure $dx$. An ingoing outer cutoff gives the other positive null flux. Equation (X2) yields
$$
 \partial_t e+\partial_x J=
 q_t\cdot(A_tq_t+A_xq_x+Cq+S).
$$
The homogeneous volume cost is $C(A+\sigma)^{-2}$ times slice energy, and the source cost is the slice $L^2$ norm of $S$ times the square root of that energy. The norm is integrable in $\sigma$. The integrated inequality gives uniform slice energy and total outgoing flux, with constants set by initial weighted norms and source norms. Commuting $\partial_t$ introduces no coefficient terms. The flux of $\partial_t q$ bounds the incoming derivative of $q$ via $q_x=(q_t+q_x)-q_t$. The equation recovers $q_{xx}$ from $q_{tt}$ and lower derivatives. Higher orders follow with finitely more time commutations and radial differentiation of the equation. The polynomially decaying data of any chosen finite order give $O(A^{-M})$ in the initial tail energy for any fixed $M$ with sufficiently large damping. The differentiated source also gives this tail energy scale. This gives a $\delta=0$ version of the dyadic bound for the causal quadratic jet. It is a positive auxiliary energy for the exterior tensor equation and carries no interpretation as signed physical massive flux.

A weighted extension uses $w=r^j(1-u)^a$ with $a\ge1$ and outgoing and ingoing cuts in the early wedge. Writing $W=(1-u)^a$,
$$
 \partial_t(we)+\partial_x(wJ)
 =w q_t\cdot(A_tq_t+A_xq_x+Cq+S)
  +r^jW_u(e-J)-j f r^{j-1}W q_t\cdot q_x.
$$
The negative $W_u$ term supplies a spacetime bulk for the outgoing derivative and mass. At $j=0$ it has no radial-weight error. At $j>0$ bound the radial error by $C_jr^{j-1}W(|q_t|^2+|q_t+q_x|^2)$, controlled by the lower radial weight with exponent $a+1$ and one extra time commutation using the positive mass. Thus finite induction gives arbitrarily prescribed finite radial powers in the flux of the second jet, provided the data carry the corresponding finite weights. This argument applies only to the background massive equation. The mean zero mode has no mass term controlling $q_t$ in this induction.

## 3. Relative wave gauge and its nonlinear cost

Use the stationary reference connection $D=\bar\nabla$ of the Schwarzschild string, the perturbation $h=g-\bar g$, and
$$
 \mathcal L^A{}_{BC}=\tfrac12 g^{AD}(D_Bh_{CD}+D_Ch_{BD}-D_Dh_{BC}),
 \qquad C_B=g^{CD}(D_Ch_{DB}-\tfrac12 D_Bh_{CD}).
$$
The exact connection-difference identity is
$$
 R_{AB}=D_D\mathcal L^D{}_{AB}-D_{(A}\mathcal L^D{}_{B)D}
        +\mathcal L^D{}_{DE}\mathcal L^E{}_{AB}
        -\mathcal L^D{}_{BE}\mathcal L^E{}_{AD},
$$
using vacuum of the reference metric. Subtracting $\nabla^g_{(A}C_{B)}$ gives $-g^{CD}D_CD_Dh_{AB}/2$ plus
$$
\begin{aligned}
 &\tfrac12g^{CD}([D_C,D_A]h_{BD}+[D_C,D_B]h_{AD}) \\
 &+\tfrac12(D_Cg^{CD})(D_Ah_{BD}+D_Bh_{AD}-D_Dh_{AB})
  -\tfrac12(D_{(A}g^{CD})D_{B)}h_{CD} \\
 &-(D_{(A}g^{CD})(D_Ch_{B)D}-\tfrac12D_{B)}h_{CD})
  +\mathcal L^D{}_{DE}\mathcal L^E{}_{AB}
  -\mathcal L^D{}_{BE}\mathcal L^E{}_{AD}+\mathcal L^D{}_{AB}C_D .
\end{aligned}
$$
Here $Dg^{-1}=-g^{-1}(Dh)g^{-1}$ and $[D,D]h$ is the background curvature action. This isolates a stationary curvature term and explicit quadratic connection terms. The operator's Lorentzian principal part is normally hyperbolic for small $h$. The Einstein constraints and $C=0$ on $\Sigma$ give $\nabla_n C=0$ there by the normal projections of $G_{AB}$, since tangential derivatives of $C$ vanish. Contracted Bianchi gives the homogeneous subsidiary wave $\Box_g C_B+R_B{}^A C_A=0$. Thus the reduced solution solves vacuum on its causal development. The initial lapse and shifts can be prescribed in the desired parity and completed first-order gauge, then the gauge equations determine their time derivatives through the coefficient of the timelike normal covector. The completed first GL jet satisfies the linear relative wave gauge since its four-dimensional tensor is transverse and traceless and its compact components vanish. This gauge setup permits the initial scalar conclusion in §1.

With $T=\partial_t$ the principal energy deformation is $\frac12\mathbf T^{AB}(\mathcal L_T h)_{AB}$ for a scalar component energy. Relative subtraction removes the stationary Schwarzschild contribution, including the extrinsic curvature of the $\tau$ slice. The highest time commutators have terms
$$
 \sum_{\ell=1}^j {j\choose\ell}
 (\mathcal L_T^\ell g^{AB})D_AD_B\mathcal L_T^{j-\ell}h,
$$
since $T$ preserves the reference connection. Compact derivatives give the corresponding coefficient commutators and the explicit mode-coupling term $(\partial_z g^{AB})D_AD_B h$. This includes the highest second derivatives of the mean in a massive-sector equation and vice versa. Wave gauge propagates constraints but does not supply a uniform bound for these terms on outgoing cones. Projection to mean-free coefficients does not commute with the principal symbol when the metric depends on $z$.

The proposed $\|\mathcal L_T h\|_\infty\lesssim\delta/r$ requires a choice of component gauge and may be too strong in a harmonic gauge. A perturbative zero-mode example is harmonic Einstein-frame linearized Ricci at second order in the amplitude of a scalar $\phi=a(u)/r$ on flat space. The $tt$ component solves $\Box h_{tt}=-4(a')^2/r^2$. For $q=rh_{tt}$ this gives $q_{uv}=(a')^2/r$. With smooth compactly supported $a$ and bounded past and initial terms in a padded outgoing window, $q_u=2(a')^2\log r+O(1)$ and $q_v=O(1/r)$ by the two ray integrations. Its time derivative therefore has a logarithm divided by $r$. This is a perturbative obstruction to a uniform all-component coefficient estimate by a naive Taylor argument. It gives no no-go result for optical formulations or nonlinear scalar news. A Schwarzschild-relative harmonic proof would need weak-null component control and comparison of its exterior to physical positive-mass cuts. The fixed flat-coordinate exterior in [Huneau–Stingo–Wyatt Proposition 3.1](https://arxiv.org/html/2307.15267v1) and the positive-mass four-dimensional theorem of [Hintz, Theorem 1.1](https://doi.org/10.1007/s00205-023-01931-3) have different domain or field hypotheses.

## 4. Full optical geometry and principal-energy improvement

Write the SO(3) quotient metric in a full optical chart with transported compact coordinate and increasing sphere radius $r$,
$$
 q_3=-BV\,du^2-2B\,du\,dr+b^2(dz+U\,du)^2,\qquad B,b>0.
$$
The areal $r$ and the gauge shift $U$ here are independent of the radial constraint variables above. The exact five-dimensional vacuum equations are
$$
 \operatorname{Ric}(q_3)_{ab}-2r^{-1}\nabla_a\nabla_b r=0,
 \qquad 1-|dr|^2-r\Box_{q_3}r=0.
$$
Equivalently with $\chi=\log r$ and $\widetilde q=r^4q_3$, the three-dimensional conformal equations are $\operatorname{Ric}(\widetilde q)=6\,d\chi\otimes d\chi-2e^{-6\chi}\widetilde q$, $\Box_{\widetilde q}\chi=e^{-6\chi}$. These follow from the three-dimensional conformal Ricci formula and $\Box_{q_3}\chi+2|d\chi|^2=e^{-2\chi}$ from the sphere equation. They remove the Hessian of the radius from the source of the quotient Ricci equation, at the cost of a growing conformal factor.

For the optical hypersurface equations, set $q_b=\partial_r\log b$, $P=\partial_z\log B$, and $W=b^3U_r/B$. Direct contraction gives
$$
 \frac{B_r}{B}=\frac{\partial_rq_b+q_b^2}{2/r+q_b},\qquad
 (r^2 W)_r=r^2 b[-P_r+(2/r+q_b)P],\qquad
 (rbV)_r=b B+r b_u-r\partial_z(bU).
 \tag{X3}
$$
For the first equation, the null affine tangent is $B^{-1}\partial_r$ and its principal transverse expansions are $B^{-1}/r,B^{-1}/r,B^{-1}q_b$. The vacuum null Raychaudhuri equation yields the displayed sign. For the second, $2R^{(5)}_{rz}=-(W_r+2W/r)/b-P_r+(2/r+q_b)P$ using $\Gamma^r{}_{rz}=(P-b^2U_r/B)/2$. For the third use $\sqrt{-\det q_3}=Bb$, $q_3^{rr}=V/B$, $q_3^{ur}=-1/B$, and $q_3^{rz}=U/B$ in the sphere equation. The mixed constraint also reads $(r^2(W+bP))_r=2r^2 b(2/r+q_b)P$, eliminating $P_r$ at the expense of one compact derivative.

Set $\rho_*=r\sqrt b$ pointwise and $B_*=bB/(\partial_r\rho_*)$ wherever $\partial_r\rho_*>0$. This auxiliary radius agrees with the Einstein radius on a circle-independent end. Algebra in (X3) gives
$$
 \partial_{\rho_*}\log B_*=\tfrac34\rho_*(\partial_{\rho_*}\log b)^2 .
 \tag{X4}
$$
Indeed $\partial_r\log B_*=3r q_b^2/[2(2+r q_b)]$ since $\partial_r\rho_*=\sqrt b(1+rq_b/2)$. In particular if $B_*(\infty)=1$ and the integral exists, then $\log B_*=-\tfrac34\int_{\rho_*}^\infty s(\partial_s\log b)^2ds$. No second radial derivative of $b$ is needed for this lapse bound. Vacuum angular geometry alone does not prove convergence or identify the integration constant with a normalized optical clock.

Let $L=\partial_r$, $\underline L=\partial_u-V\partial_r/2-U\partial_z$, $Z=b^{-1}\partial_z$. In the dual coframe $(du,dr+Vdu/2,dz+Udu)$ the deformation under $T=\partial_u$ is
$$
 \mathcal L_T q_3=-B V_u\,du^2-2 B_u\,du(dr+Vdu/2)
   +2bb_u(dz+Udu)^2+2b^2U_u\,du(dz+Udu).
$$
For a scalar rotationally invariant component $s$ with stress $\mathbf T=ds\otimes ds-g(|ds|^2+m^2s^2)/2$, its contraction is exactly
$$
\begin{aligned}
 \tfrac12\mathbf T^{ab}(\mathcal L_T g)_{ab}
 ={}&-\tfrac{V_u}{2B}(Ls)^2
       -\tfrac{B_u}{2B}[(Zs)^2+m^2s^2]
       -\tfrac{bU_u}{B}(Ls)(Zs) \\
    &+\tfrac{b_u}{b}\left[\tfrac{(Zs)^2-m^2s^2}{2}
                             +\tfrac{(Ls)(\underline Ls)}{B}\right].
\end{aligned}
$$
The sphere deformation is zero at fixed $r$. The $B_u$ cross term simplifies because $|ds|^2=-2(Ls)(\underline Ls)/B+(Zs)^2$. There is no pure incoming derivative square. A weight in $u$ with negative derivative gives a positive outgoing bulk proportional to $\mathbf T(L,T)/B$ when $T$ is timelike, and $\mathbf T(L,T)=B[(Zs)^2+m^2s^2]/2+V(Ls)^2/2+bU(Ls)(Zs)$. If the coefficients multiplying only good derivatives are small against this bulk, the mixed $b_u$ term costs a slice energy coefficient proportional to $b_u^2$ by Young's inequality. For $|b_u|\lesssim |\epsilon|/r$ and $r\gtrsim A+\tau$ this is integrable in time. The observation places even a logarithmic correction in $V_u$ on outgoing derivatives. It is an exact structural identity with a conditional energy consequence.

The $zz$ equation has a useful cancellation after the mixed constraint is introduced. With $C_o=W+bP$ it reads
$$
\begin{aligned}
 0={B\over b}R^{(5)}_{zz}={}&
 2(\partial_u-U\partial_z)b_r-Vb_{rr}-(V_r+2U_z)b_r\\
 &+\tfrac2r(b_u-Ub_z-Vb_r-bU_z)
    -\partial_z\big((B/b^2)C_o\big)
    +\tfrac{B}{2b}P^2-\tfrac{b^3}{2B}U_r^2 .
\end{aligned}
 \tag{X5}
$$
For example, the two contributions with differentiated compact shift and lapse before introducing $C_o$ are $-\partial_z(bU_r)$ and $-(B/b)(P_z+P^2/2-P b_z/b)$. Using $b U_r=(B/b^2)C_o-(B/b)P$ cancels $P_z$ and the term linear in $P b_z$. The remaining compact principal term involves the hypersurface variable $C_o$. Equations (X3), (X5) follow from the warped product Ricci tensor. The direct component formulas were also checked in SymPy 1.14.0 by forming the inverse metric and Christoffel symbols. This local check contains no evolving vacuum solution.

The remaining equations propagate radially. If $R^{(5)}_{rr}=R^{(5)}_{rz}=R^{(5)}_{zz}=R^{(5)}_{\theta\theta}=0$, the scalar curvature is $-2R^{(5)}_{ur}/B$, and the corresponding Einstein tensor obeys $G_{ar}=0$. Use
$$
 \nabla_a G^a{}_b
 =(Bbr^2)^{-1}\partial_a(Bbr^2G^a{}_b)-\tfrac12G^{ac}\partial_b g_{ac}
$$
on the orbit coordinates, retaining the sphere contributions in the last contraction. For $b=r$ only the latter term remains, with contraction $2(2/r+q_b)R^{(5)}_{ur}/B$. Hence $R^{(5)}_{ur}=0$ wherever the outgoing expansion is nonzero. The $z,u$ components of the Bianchi identity then give
$$
 \partial_r(br^2 R^{(5)}_{uz})=0,\qquad
 \partial_r\big(br^2(R^{(5)}_{uu}-U R^{(5)}_{uz})\big)
 =\partial_z\big(B r^2 R^{(5)}_{uz}/b\big).
 \tag{X6}
$$
The last contraction vanishes in these two equations since the contravariant Einstein tensor can then have support only in the $rr,rz,zr$ slots, and $g_{rr}=g_{rz}=0$. Prescribing $R^{(5)}_{uz}=R^{(5)}_{uu}=0$ on one transverse section propagates both along its generators. This closes the optical subsidiary calculation on an existing chart. The choice of section and its compatibility with the terminal construction and the original spacelike data require a separate existence proof.

The scalar principal symbol multiplied by $B$ is
$$
 -2\partial_u\partial_r+V\partial_r^2
       +2U\partial_r\partial_z+(B/b^2)\partial_z^2 .
$$
Time commutation in these coordinates therefore yields only outgoing or compact second derivatives of the commuted field. The $zz$ vacuum equation still couples $2b_{ur}$, $Vb_{rr}$ and $U b_{rz}$ to the compact derivative of $C_o$. Solving the null constraints for $U,V,B$ and substituting does not immediately prove a positive quasilinear evolution energy. In particular the radial integrations in (X3) carry compact derivatives, and $V$ also sees $b_u$. Bounds that use successive generic product estimates would cost compact derivatives. A sharp system estimate must retain the compact principal terms and their adjoints. The optical chart itself must be constructed with terminal compact momentum and controlled generator Jacobians. Free relative wave gauge does not permit imposing the optical gauge simultaneously by coordinate choice. Hence the scalar deformation identity cannot be substituted directly into the relative wave-gauge tensor equations to close their nonlinear estimate.

The next bounded step is to obtain a positive constrained evolution energy for the coupled $(b,W,V,B)$ optical system with finite compact derivative count, with terminal lapse and shift integration constants set by the same optical construction. Alternatively, in a relative wave gauge one must bound the bad deformation terms in null components and prove that the resulting domain and transverse commutations compare to physical optical rays. The proven spatial seed hierarchy, exact mean-chart Z jet, and background second-jet flux leave both nonlinear routes open.
