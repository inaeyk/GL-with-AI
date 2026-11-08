# Vacuum trace on the early optical wedge

The linear terminal transform of GOAL-009 does not remove the top compact derivative about a radiating mean radion. The leading additional nonlocal term is computed below. A bounded rational correction removes that term to first order in the mean amplitude and leading order in inverse radius. The resulting equation still needs a finite-derivative estimate for the full optical principal part and boundary matching to the prepared Cauchy data. The finite-seed trace remains conditional.

## Inputs and conventions

All estimates use $r_0=1$. The background is the vacuum Schwarzschild string with $L=10$, $k=\pi/5$, $f=1-1/r$, $x=r+\log(r-1)$, $u=t-x$ and $\tau=v-r$. The mass-tuned Z data fix $G_4=G_5/L$ and $M_0=r_0/(2G_4)$. The normalized first jet is $e^{\Omega t} H(r)\cos(kz)$ with $H_{\theta\theta}(2)/8=1$. The completed scalar datum and its normal velocity vanish at order two. The reference outgoing cut from the initial sphere at $r$ is $u=g(r)=-r-2\log(r-1)$. The quadratic trace coefficient on $I=[-10,-8]$ is inherited from the directed Schwarzschild calculation. [GOAL-005 constraints and radiation, commit 61c7b5c2](https://github.com/inaeyk/GL-with-AI/tree/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005) · [GOAL-005 Work audit, commit a9bfbec0](https://github.com/inaeyk/GL-with-AI/blob/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639/research/sav/verification/AUDIT_GOAL005_FINITE_SLICE_61c7b5c2.md)

The inherited optical equations and background weighted energy are from GOAL-009. Its initial weights refer to the damped GOAL-006 Z family with the damping chosen before fixing the family. They cannot be applied to the original GOAL-005 spatial data by identifying the free scalar jets. In the undamped spatial equations the leading nonzero Fourier momentum block is $X_r=Y_z$, $Y_r=X_z/2$. It admits constant-envelope radial oscillations. The proven undamped class permits $K=O(r^{-2})$ with differentiated oscillatory tails. Arbitrarily large weighted momentum norms require the separate damped construction or additional cancellation in those tails. Both preparations select the same order-two completed scalar source and zero free scalar jet, but the different higher data enter the nonlinear estimate and the quadratic massive homogeneous transient. [GOAL-006, commit 80f030df](https://github.com/inaeyk/GL-with-AI/tree/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006) · [GOAL-009, commit 30b53485](https://github.com/inaeyk/GL-with-AI/tree/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009) · [GOAL-009 Work audit, commit 844d1ccd](https://github.com/inaeyk/GL-with-AI/tree/844d1ccdd5e507392f4ace02debf08da34d543d0/research/sav/verification/GLR-GOAL-009)

On the fixed background the first jet decays like $\exp[-(\sqrt{k^2+\Omega^2}-\Omega)r]$ times a radial power along each early ray. For the damped family the compatible forced second massive jet has arbitrarily prescribed **finite** weighted cone norms, with an additional time commutation used to bound the incoming derivative. The cone energy includes the positive far-zone $f(2k)^2$ term. It is an auxiliary fixed-operator estimate. The exact Einstein principal part couples compact projections and does not commute with the background time derivative. The high compact frequencies used below probe an estimate for that nonlinear principal part, even though the actual first and second jets have fixed harmonics.

## 1. Compact principal part about a circle-independent optical mean

Use the local area chart of GOAL-009, $R=r\sqrt b$, with $b=e^q$ and $g_{RR}=g_{Rz}=0$. The metric, lapse and scaled field are
\[
 g_5=-BV\,du^2-2B\,du\,dR-2BA\,du\,dz+b^2\,dz^2+r^2d\omega^2,
 \qquad \beta=\log(Bb),\qquad W=Rq.
\]
The compact normal is $n=b^{-1}(\partial_z-A\partial_R)$. Write $P=A_R+A(\beta_R-3q_R)-\beta_z+3q_z$ and $Q=q_z-Aq_R$. For the mixed trace $T$ of the compact extrinsic curvature, its angular eigenvalue $\varpi$ and its other repeated eigenvalue $U$, the combinations needed here are
\[
 T=(-P+Q-2A/R)/b,\quad
 T/2+\varpi=(-P/2-2A/R)/b,\quad
 U=(-P+2Q)/(2b),\quad \varpi=(-A/R-Q/2)/b .
\]
The GOAL-009 equations are
\[
\begin{split}
 \beta_R&=3Rq_R^2/4,\\
 (R^2P)_R&=6Rq_z-4R\beta_z+3R^2q_Rq_z,\\
 (e^{\beta-3q}A)_R&=e^{\beta-3q}(P+\beta_z-3q_z),\\
 (RV)_R&=e^\beta\{1-(R^2/b)[n(T/2+\varpi)+T^2/4+2\varpi^2]\},\\
 0&=-2W_{uR}+(VW_R)_R-(V_R/R)W
       +R e^\beta b^{-1}[n(T)+2(U^2+\varpi^2)] .
\end{split} \tag{1}
\]
Here $u,R,z$ derivatives refer to this optical chart. Equations (1) are local identities in an existing smooth chart. [Pinned characteristic equations](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/checks/README.md#full-optical-local-area-subsystem)

Linearize (1) at any smooth circle-independent optical mean $\bar W$ with $\bar P=\bar A=0$. Let $w$ denote the scalar variation, and use a subscript $1$ on variations of coefficients. The lapse and two shift equations become
\[
\begin{split}
 (\beta_1)_R&=\tfrac32 R\bar q_R (w/R)_R,\\
 (R^2P_1)_R&=(6R+3R^2\bar q_R)(w/R)_z-4R(\beta_1)_z,\\
 (e^{\bar\beta-3\bar q}A_1)_R
     &=e^{\bar\beta-3\bar q}[P_1+(\beta_1)_z-3(w/R)_z].
\end{split} \tag{2}
\]
The two-compact-derivative contribution to the scalar equation, including the potential response, is
\[
 R e^{\bar\beta-3\bar q}\partial_z
       [(w/R)_z-P_1-(\bar q_R+2/R)A_1]
 +(V_1\bar W_R)_R-\frac{(V_1)_R}{R}\bar W ,
 \qquad
 (RV_1)_R\big|_{\partial_z^2}
     =R^2 e^{\bar\beta-3\bar q}\partial_z(P_1/2+2A_1/R).
 \tag{3}
\]
In (3) the potential terms use only the two-compact-derivative part of $V_1$. Terms in the differentiated scalar equation with no compact derivative are omitted. In particular, the angular equation contributes at top compact order even if $\bar W_R=0$.

### Leading mean model

Use $\bar W=\alpha(u)$ at leading order, hence $\bar q=\alpha/R$ and $\bar\beta=O(\alpha^2/R^2)$ with zero terminal lapse. Retain only terms linear in $\alpha$. The leading calculation also applies to a mean with $\bar W=\alpha(u)+O(R^{-1})$, with differentiated radial decay, as the displayed $R^{-1}\alpha$ compact symbol. The $O(R^{-1})$ term in $\bar W$ adds subleading radial powers in (2)–(3). This is an asymptotic coefficient computation. It neither assumes $\bar W=\alpha$ is an exact Schwarzschild scalar solution nor bounds the higher-order terms for the exact Z development.

Use the terminal inverses with $\beta_1(\infty)=A_1(\infty)=\lim R^2P_1=0$ and zero terminal nonzero-harmonic $1/R$ integration function in $V_1$. This specifies the operator part of the linearization. The arbitrary integration functions in the general problem are separate boundary data. For $w=R^{-s}g(z)$, $\Re s>3$, equations (2) give
\[
\begin{split}
 \beta_1 &= -\frac{3(s+1)}{2(s+2)}\,\alpha R^{-s-2}g,\\
 P_1 &=\left[-\frac{6}{s-1}R^{-s-1}
            -\frac{3}{s+2}\alpha R^{-s-2}\right]g_z,\\
 A_1 &= [d_0(s) R^{-s}+d_1(s)\alpha R^{-s-1}]g_z,\qquad
 d_0=\frac{3(s+1)}{s(s-1)},\qquad
 d_1=\frac{3(s+3)/(2(s+2))+3d_0}{s+1}.
\end{split} \tag{4}
\]
Terms quadratic in $\alpha$ are suppressed. At $\alpha=0$ the relevant $V_1$ in (3) is
\[
 V_1=-\frac{3(s+2)}{s(s-1)(s-2)}\,R^{1-s}g_{zz}.
\]
The two-compact-derivative coefficient in (3), in units of $R^{-s}g_{zz}$, is $c_0(s)+(\alpha/R)c_1(s)$ with
\[
 \begin{split}
 c_0(s)&=1-\frac{6}{s(s-1)},\\
 c_1(s)&=\frac{3}{s+2}+d_0-2d_1-3c_0-\frac{3(s+2)}{s(s-2)}\\
       &=-3-\frac{12(s^2+2)}{(s-2)(s-1)(s+1)(s+2)} .
 \end{split} \tag{5}
\]
The last term on the first line for $c_1$ is the $-(V_1)_R\bar W/R$ contribution. For example, at $s=4$ it is $-9/4$. Dropping it changes the symbol even in the leading mean model. The Schwarzschild radial potential does not enter (5).

**Check by direct curvature.** The accompanying script reconstructs the three-dimensional quotient Christoffels from the metric and then adds $-2r^{-1}\nabla_i\nabla_jr$ to its Ricci tensor. It uses a two-parameter jet ring with factors $\alpha$ and $\eta e^{i k_m z}$ and computes $\operatorname{Ric}_{RR}$, $\operatorname{Ric}_{Rz}$, the angular-plus-normal combination and $-R e^\beta b^{-1}\operatorname{Ric}_{nn}$. The differentiated principal coefficients agree symbolically with (5) and at $s=4,5,6$. Omitting the potential variation in the metric fails the angular equation and changes all three tested values. The curvature calculation is a separate algebraic route with the same candidate jets (4). It checks those jets in the radial constraints, but shares the terminal power ansatz and does not check evolving data.

## 2. What the frozen normal form leaves and a leading repair

Put $D=R\partial_R$, and use the terminal radial domain of GOAL-009 with $L^2(R^p\,dR)$, $p>5$, on $R\ge R_*>2$. A rational function of $D$ acts on $R^{-s}$ by evaluation at $D=-s$. The statements for general decaying radial functions follow by composing terminal resolvents. For instance $(s+c)^{-1}$ corresponds to $h(R)\mapsto\int_0^\infty e^{-ct}h(Re^t)\,dt$, bounded in this weighted space by $1/(\sigma+c)$ when $\sigma=(p+1)/2>3$ and $\sigma+c>0$. This operator convention fixes the Mellin line and excludes the nondecaying integration functions. It can be used for smooth compactly supported tests on a finite ray when they vanish near the terminal section. Generic finite terminal data produce boundary terms.

The inherited transform is $Z=F(D-1)w$ after left multiplication of the equation by $F(D)$, with $F(D)=D(D-1)/[(D+2)(D+3)]$. It exactly removes the $c_0$ multiplier via $F(D)c_0(-D)=F(D-1)$. The next term is
\[
 F(D)\,\frac{\alpha}{R}c_1(-D)\partial_z^2 w
     =\frac{\alpha}{R} c_1(-D)\partial_z^2 Z ,
 \tag{6}
\]
since $F(D)R^{-1}=R^{-1}F(D-1)$. The local compact speed correction is $-3\alpha/R$. The remainder $K(s)=c_1(s)+3$ is strictly nonlocal. Its terminal convolution kernel at logarithmic displacement $t>0$ is
\[
 k(t)=6(e^{-2t}-e^{-t}+e^t-e^{2t})<0.
\]
It is bounded but not self-adjoint in the weighted radial norm. Pick smooth nonnegative inner and outer bumps with disjoint supports inside a shell at $R_*$, with the outer one to the right of the inner one. Only the pairing with $K$ acting on the outer bump is nonzero. Consequently, even allowing a symmetric nonlocal adjustment of the compact mass quadratic form leaves a skew principal contribution.

A more quantitative negative control takes $Z$ to be the outer bump times $\cos(k_m z)/k_m$ and $Z_u$ the inner bump times $\cos(k_m z)$, $k_m=2\pi m/L$. Normalize the radial bumps in $L^2(R^p\,dR)$ and use fixed profiles in $R/R_*$. The ordinary same-order time-current energy stays bounded in $m$, since it controls $Z_u,Z_R,Z_z$. The pairing of the compact remainder with the incoming derivative has magnitude a positive profile constant times $|\alpha|\,|k_m|/R_*$. Including a fixed number of $z$ commutations gives the same relative loss by rescaling the test amplitudes by $|k_m|^{-j}$. Thus this term cannot be put on the forcing side of a same-order positive current estimate with a mode-independent bound. This is a counterexample for the proposed **source estimate**, using test profiles. It is not a vacuum solution, a failure of the corrected coupled operator, or a nonexistence result for the physical trace. The outgoing null flux of the optical scalar current controls $\partial_R Z$ and $Z_z$. The problem in this forcing estimate is their coupling to the incoming derivative through $Z_u$, even though $g^{uu}=0$ in the optical chart.

There is a first correction. Define
\[
 b_*(s)=\frac{3}{s-1}-\frac3s+\frac{3}{s+1}-\frac{3}{s+2},
 \quad G(s)=s b_*(s),\quad H(s)=s b_*(s-1).
\]
Then $s[b_*(s)-b_*(s-1)]=K(s)$. Apply the left multiplier $1+(\alpha/R)H(-D)$ and set
\[
 Z_*=[1+(\alpha/R)G(-D)]Z .
\]
The identities $H-G=-K$ and $sH(s+1)=(s+1)G(s)$ imply that the principal mixed derivative is preserved through first order in $\alpha$. At that order the compact part is exactly $(1-3\alpha/R)\partial_z^2 Z_*$. The mixed term also produces a term proportional to $\alpha_u R^{-2}(D-1)G(-D)Z$, which is radial order zero because $G(s)=O(1/s)$. On the stated radial space the correction is bounded. For example
\[
 \|G(-D)\|\le \frac{3}{\sigma-1}+\frac3{\sigma+1}+\frac6{\sigma+2},
 \qquad
 \|H(-D)\|\le \frac6{\sigma-2}+\frac3{\sigma-1}+\frac3{\sigma+1}.
\]
Each transformation is invertible by a Neumann series when $|\alpha|/R_*$ times its displayed bound is below one. The flat zero-mean limit gives the inherited $F$ transform.

This repair treats the leading compact symbol linearized about a circle-independent mean. Terms with additional radial decay can still carry two compact derivatives. Smallness of a coefficient times an unbounded frequency is insufficient for a same-order estimate. The exact $z$-dependent optical principal part, the resulting commuted radial terms and the terminal data remain outside this calculation. The transformed field depends on the full outgoing ray, so its Cauchy datum cannot be set by a local initial Taylor coefficient.

## 3. Constraints, clock and the conditional news estimate

**Supplementary data.** If (1) holds then $G_{aR}=0$ and the $R$ Bianchi component enforces $\operatorname{Ric}_{uR}=0$ where $R>0$. The other two identities are
\[
 \partial_R(R^2\operatorname{Ric}_{uz})=0,\qquad
 \partial_R[R^2\operatorname{Ric}_{uu}+(B/b^2)R^2 A\operatorname{Ric}_{uz}]
 =\partial_z[(B/b^2)R^2\operatorname{Ric}_{uz}].
\]
Vanishing of both supplementary constraints on the actual intersecting Cauchy section would propagate along each ray. Their mere $O(R^{-2})$ decay does not set the integration functions to zero. For example the spherical product of outgoing Vaidya with the circle, $b=B=1$, $A=0$, $V=1-2m(u)/R$, solves (1) for arbitrary $m(u)$ and has $\operatorname{Ric}_{uu}=-2m'(u)/R^2$. The angular radial integration function must be matched to the physical mass aspect. Fixed initial ADM mass only fixes the spatial-end subtraction. The torsion-free terminal prescription in §1 has to agree with the actual Cauchy value of $R^2P$ plus its radial source integral. [GOAL-009 supplementary identities and terminal charges](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/checks/README.md#necessary-compatibility-at-a-product-end)

**Radius and lapse.** With a circle-independent leading end, $R=r_5+\alpha/2+o(1)$ and $\Phi_\epsilon=\sqrt3\,\alpha/2$. Under a $C^1$-compatible coordinate change, $dr_5=dR-(\alpha_u/2)du$ at leading order. The potential in the Einstein area chart is $V_5-\alpha_u+o(1)$ in the same clock. If $du_B=B_\infty du$, then normalized Einstein asymptotics require $V_5/B_\infty\to 1+\partial_{u_B}\alpha$. This recovers the Work audit correction $1+2N_\epsilon/\sqrt3$. Setting $V_5/B_\infty\to1$ at five-dimensional sphere radius would force zero news. In the local area chart, with a terminal lapse fixed, (1) instead yields $\beta=-\tfrac34\int_R^\infty s q_s^2\,ds$ in the normalized clock. This estimate presupposes convergence and a compatible normalization. It does not determine the integration constant of the clock. At a finite reference intersection the full optical cut can tilt off $\tau=0$. The order-two length correction in GOAL-009, DERIVATIONS §4, survives there although the completed scalar jet on the actual slice vanishes. The tilt disappears exponentially in the order-two trace. This does not give a uniform finite-seed cut comparison. [GOAL-009 Work audit, §1](https://github.com/inaeyk/GL-with-AI/blob/844d1ccdd5e507392f4ace02debf08da34d543d0/research/sav/verification/GLR-GOAL-009/DERIVATIONS.md#1-the-areal-normalization-conflict)

**Harmonic logarithm.** In the flat asymptotic Einstein-scalar model take the exact free spherical profile $\phi=A_0(u)/r$ with compactly supported smooth $A_0$. Linear harmonic Einstein response to its scalar stress has $\Box h_{tt}=-4(A_0')^2/r^2$. For $Y=r h_{tt}$ this is $Y_{uv}=(A_0')^2/r$. The fixed-$u$ radial integration gives $Y_u=2(A_0')^2\log r+O(1)$. For bounded incoming terms and a fixed padded window, $Y_v=O(r^{-1})$, so $\partial_t h_{tt}$ has a $\log r/r$ contribution. For radion amplitude $O(\epsilon^2)$ this starts at $O(\epsilon^4)$. The optical condition $g^{uu}=0$ removes the pure incoming derivative square from the deformation identity, rather than enforcing an all-component $C/r$ bound on the harmonic metric. The obstruction in (6) arises already at order $O(\epsilon^2/R)$ in the compact operator linearization. These are different tests of the two estimators.

**Sufficient bound still missing.** One route is a positive finite-derivative current for the exact constrained optical evolution, or a normal form that reduces the full two-compact-derivative principal part to an energy-compatible operator. Its incoming current must control the remainder left after (6), with truncation-independent constants and matched initial and terminal terms. This has to provide the commuted massive ray estimates and a nonlinear mean forcing control. For a mean chart with scalar equation $4\psi_{uv}+V_\epsilon\psi=F_\epsilon$, define $[F](u)=\int |F(u,v)|\,dv$ from the matched initial section. The relevant forcing difference is
\[
 \mathscr R=F_\epsilon-\epsilon^2 F_2+\epsilon^2(V_0-V_\epsilon)\psi_2.
\]
The sign here is for the potential on the left in the displayed scalar equation. If the free sup-field and padded $L^2$ derivative errors are $O(\epsilon^4)$ and $[\mathscr R]$ is $O(\epsilon^4)$ in $L^1_u$ on the past wedge and $L^2_u$ on a padded observation interval, Volterra iteration gives the sup field error and $L^2$ fixed-$v$ derivative error. The coefficients in this estimate are bounded by $\exp(\|[V_\epsilon]\|_1/4)$ and $\|[V_\epsilon]\|_2/4$. A separate tail estimate controls the incoming derivative along physical tubes and justifies the limiting differentiated trace. Half-translation covariance makes the matched mean trace even. For example $\epsilon^2 f+|\epsilon|^3g$ is also even and has quadratic coefficient $f$. Covariance does not supply the quartic error without the norm estimates.

If the geometric trace comparison and the normalized clock $u_B=u+O_{C^1}(\epsilon^2)$ are proved on a padded interval, the clock composition preserves a quartic error for the smooth $n_Z$. In fact with $d=u_B'(u)$, the normalized derivative is $N_\epsilon(u_B)=\partial_u\Phi_\epsilon(u)/d$. The error from changing the quadratic coefficient and Jacobian is bounded by $\epsilon^2\|n_Z(u)-d(u)n_Z(u_B(u))\|_2$ times $(\inf d)^{-1/2}$. The remaining error transforms with the same Jacobian factor.

For a geometric news bound $\|N_\epsilon-\epsilon^2 n_Z\|_{L^2(I)}\le C_I|\epsilon|^4$ and the regular Einstein-frame flux, set $b_Z=\|n_Z\|_2$. Then
\[
 2[\max(0,\epsilon^2 b_Z-C_I\epsilon^4)]^2
 \le E_I/M_0\le
 2(\epsilon^2 b_Z+C_I\epsilon^4)^2,\qquad
 |E_I/M_0-\mathcal K_Z\epsilon^4|\le4b_Z C_I|\epsilon|^6+2 C_I^2|\epsilon|^8.
\]
In the inherited window $b_Z\in(\sqrt{3.5316\times10^{-7}/2},\sqrt{4.0171\times10^{-7}/2})$. The lower endpoint is greater than $4.20\times10^{-4}$. A sufficient positivity threshold is $C_I\epsilon^2<4.20\times10^{-4}$ within the eventual exterior smallness regime. The present analysis does not assign $C_I$, an exterior seed threshold, or a finite-amplitude energy. It identifies one leading normal-form repair and the remaining coercive and geometric inputs needed to assign them.

## Reproduction and scope

From the scoped research directory run
```sh
env OPENBLAS_NUM_THREADS=1 python3 -B GLR-GOAL-016/vacuum/check_optical_principal.py
```
The check uses Python 3.14.8 and SymPy 1.14.0 with exact rationals and no floating integration. The archived output lists the signed test coefficients and the rational cancellation. The inputs are the explicitly defined terminal power jets. No evolution or production model is used.
