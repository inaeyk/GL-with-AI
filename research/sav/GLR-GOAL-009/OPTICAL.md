# Full optical cuts from a vacuum radial constraint

Independent reconstruction for GOAL-009. The prior source and historical audit were available at intake. The compact cancellation below was reconstructed from the warped-product radial Ricci equation, before comparing with the coordinator's exterior approach. This is a conditional geometric theorem on a vacuum exterior. Its hypotheses are quantitative and do not assert that the exact damped GL data have already been evolved with these bounds.

## 1. Reference chart and assumptions

Write the SO(3) invariant metric as $g=q+r^2d\omega^2$, with $r=r_5$ the sphere radius, $q$ the Lorentzian three-metric in $(u,r,z)$, and $z$ of period $L$. Restrict $q$ to $z=\mathrm{const}$ to obtain the two-metric $h$. Let $b=(q^{zz})^{-1/2}$ and define
$$
 A^{ab}=r^2\langle\sqrt{-\det h}\,h^{ab}\rangle.
$$
Assume $u$ is an outgoing null label of this **effective density**, $A^{uu}=0$, constructed by a common $z$-independent recharting of an areal initial-time chart. Set $I\Subset J$, with $J$ a padded compact interval, and work on $r\ge R_1$ containing the terminal portions of all these outgoing rays. A regular finite strip can connect this region to the initial hypersurface.

Suppose the following bounds hold uniformly for small seed.

1. The metric and the effective cone have uniform nondegeneracy in this chart. Reflection gives $\langle q_{uz}\rangle=\langle q_{rz}\rangle=0$. Put $q_0=\langle q\rangle$ except reset $(q_0)_{rr}=0$, and write
   $$
   q_0=-BV\,du^2-2B\,du\,dr+b_0^2dz^2,
   \qquad B,b_0,V\asymp1.
   $$
   The $u,r$ derivatives through order five of these mean coefficients are bounded. For $B,V$ those containing at least one $r$ derivative are $O(r^{-1})$. For $b_0$ all positive-order derivatives are $O(r^{-1})$. For Bondi normalization assume $B(u,r)\to B_\infty(u)$ in $C^1(J)$, $V/B\to1$, $b_0\to1$, and $|B_\infty-1|_{C^1}\lesssim\epsilon^2$. To compare the scalar assume $b_0-1=O(r^{-1})$, which also follows from a uniform rescaled mean scalar bound in this chart.
2. Put $s=Pq=q-\langle q\rangle$, with $P$ the compact mean-free projection. Define $d_j(r)=\max_{|\alpha|\le j}\sup_{J,z}|\partial^\alpha s|$, including $r,u,z$ derivatives. A sufficient bound is
   $$
   d_5(r)\lesssim |\epsilon| r^{-1+\delta/2},
   \qquad \int_D^{2D} d_5(r)^2\,dr\lesssim\epsilon^2 D^{-2+\delta},
   \qquad 0\le\delta<1. \tag{O1}
   $$
   The constants can depend on $R_1,J$. The shell estimate in (O1) follows from a finitely commuted version of (B4) after $H^1(J)$ and $H^1(S^1)$ Sobolev bounds and $dv\asymp dr$ at fixed $u$. The pointwise estimate follows using one more local radial Sobolev derivative on a unit interval and the same shell estimate for $d_6$. In particular it uses $u$-commutations before evaluating at tilted cuts. The original (B4) as written does not itself give these higher commutations or the areal-chart bounds.
3. For the geometric initial-cut anchoring described in §5, require the half-period even, compact mean-free part of $s$ to obey (O1) with $|\epsilon|$ replaced by $\epsilon^2$, and a regular $C^2$ finite-strip continuation of the same estimates. The pinned GL linear jet is exponentially decaying along the background outgoing rays. An areal recharting made by setting $r$ equal to the invariant orbit radius must be controlled as part of the exterior proof. If a different evolution gauge has only $O(\epsilon/r)$ *normalized* angular metric fluctuations, the resulting radial displacement can be $O(\epsilon)$ and its compact derivative can cost one radial weight.

Under these conditions the physical optical chart can be constructed, with zero terminal compact optical momentum and no compact caustic. Its limiting cut clock is $C^1$-close by $O(\epsilon^2)$ after normalization and anchoring. These claims do not use an independent first-radial-moment hypothesis on the raw oscillatory $q_{rr}$.

## 2. Vacuum gives a compact antiderivative

Write $a=Pq_{rr}$, $c=Pq_{rz}$, $d=Pq_{zz}$, and $W=\partial_z^{-1}$ on the zero-mean periodic functions, with the primitive chosen to have zero mean. The effective cone condition first gives an exact estimate on the discarded mean radial coefficient. Set $w=(-\det h)^{-1/2}$. The $2\times2$ inverse formula gives $0=\langle w q_{rr}\rangle$ and hence
$$
 m:=\langle q_{rr}\rangle=-\frac{\langle(w-\langle w\rangle)a\rangle}{\langle w\rangle}.
$$
Uniform nondegeneracy and the mean-value formula for $w$ give $|m|\lesssim d_0^2$, with differentiated versions $O(d_j^2)$.

The warped-product radial vacuum equation is
$$
 0=\operatorname{Ric}(g)_{rr}=\operatorname{Ric}(q)_{rr}+2\Gamma^r_{rr}(q)/r.
$$
For $q=q_0+s+m\,dr^2$, its compact-principal linearization about $q_0$ is
$$
 b_0^{-2}\bigl(c_{rz}-\tfrac12a_{zz}-\tfrac12d_{rr}\bigr). \tag{O2}
$$
To see which terms can accompany (O2), use the coordinate Ricci principal part
$$
 \tfrac12q^{AB}\bigl(2\partial_A\partial_r q_{rB}
              -\partial_A\partial_Bq_{rr}-\partial_r^2 q_{AB}\bigr).
$$
At $q_0$, the term with $A=B=z$ is (O2), and the horizontal principal part vanishes since in two dimensions $\operatorname{Ric}(h)_{rr}=\operatorname{Scal}(h)h_{rr}/2$ and $(q_0)_{rr}=0$. Mixed inverse coefficients vanish at $q_0$. All remaining linear coefficients multiply $s,\partial s$ with size $O(r^{-1})$. For an explicit separation, let $C^A_{BC}=\delta\Gamma^A_{BC}$ and use $\delta\operatorname{Ric}(q)_{rr}=\nabla_A C^A_{rr}-\nabla_r C^A_{rA}$. The all-horizontal $C^a_{bc}$ equal the two-dimensional variations. The additional compact derivative is $\partial_z C^z_{rr}$, where $C^z_{rr}=b_0^{-2}(c_r-\Gamma^a_{rr}s_{az}-a_z/2)$, and the additional radial trace derivative is $-\partial_r C^z_{rz}$, where $C^z_{rz}=(d_r-2(b_0)_r d/b_0)/(2b_0^2)$. The remaining compact-connection terms carry a first derivative of $b_0$ or $\Gamma^a_{rr}$. In the horizontal variation the scalar curvature multiplying $a/2$ is $O(r^{-1})$. Indeed $\Gamma^u_{ru}=\Gamma^u_{rr}=0$, the other symbols with a radial lower index are $O(r^{-1})$, and the term $\partial_r\Gamma^a_{ua}$ is $O(r^{-1})$. The sphere variation has the explicit $2/r$ factor. This permits bounded pure-$u$ derivatives of $B,V$, and in particular a cut-dependent limiting $B_\infty$. The omitted nonlinear principal terms contain
$(q^{-1}-q_0^{-1})\partial^2(s+m\,dr^2)$ and $\partial^2m$. The other terms have factors of type $(\partial s)^2$, $s\partial s\,\partial q_0$, $s^2\partial^2q_0$, $s^2(\partial q_0)^2$, and the corresponding $m$-terms, or $r^{-1}(s\partial s+s^2\partial q_0)$ from the sphere. Smooth bounded inverse factors do not change the estimates. In the projected equation the zeroth-order mean $\operatorname{Ric}(g_0)_{rr}$ vanishes regardless of whether $q_0+r^2d\omega^2$ is vacuum. For $j\le3$ the remainder $F$ after (O2) obeys
$$
 \max_{|\alpha|\le j}\sup_{J,z}|\partial^\alpha F|
 \lesssim r^{-1}d_{j+1}+d_{j+2}^2.
$$
This accounting includes the full terms with two derivatives on the perturbed metric. As a sign check, for $q_0=-fdu^2-2du\,dr+dz^2$ the unprojected linear variation is exactly
$$
 c_{rz}-\tfrac12a_{zz}-\tfrac12d_{rr}
 -\tfrac12 f''a+r^{-1}(f a_r-2\partial_r s_{ur}+a_u).
$$
The sign of the last group follows from $\delta\Gamma^r_{rr}=(-2\partial_r s_{ur}+a_u+fa_r)/2$.

Apply $2b_0^2W^2$ to the projected vacuum equation. It yields the exact identity
$$
 a=E_r+G,qquad E=2Wc-W^2d_r,qquad G=2b_0^2W^2 PF. \tag{O3}
$$
The compact inversion is at fixed $u,r$. In the original nonareal chart an extra derivative of the normalized angular radius perturbation enters $E$, and an extra radius-matching problem appears. Formulating the estimate in the areal chart avoids that problem.

By (O1) and Cauchy–Schwarz, $\int_D^{2D}d_5\lesssim |\epsilon|D^{(\delta-1)/2}$, and $\int_D^{2D}r d_5^2\lesssim\epsilon^2 D^{\delta-1}$. Thus $G$ and its indicated derivatives have a finite first radial moment, whereas the raw $a$ need not have one. The $E_r$ term must be removed by a coordinate transformation or an equivalent signed estimate. Applying absolute values to that term loses the gain.

## 3. Terminal construction and compact Jacobians

Make the exact change $U=u-E/(2B)$, holding $r,z$ fixed. It is a small diffeomorphism on the padded interval. Its relevant inverse coefficient is
$$
 q^{UU}=-G/B^2-E B_r/B^3+O(d_2^2),
$$
since $q^{uu}=-a/B^2+O(d_0^2)$ and the cross term $2q^{ur}\partial_r[-E/(2B)]$ contributes $E_r/B^2-E B_r/B^3$. The estimate persists after $U,z$ derivatives through order three, with $d_5$ on the right. In particular, with primes indicating the new coordinates,
$$
 |q'^{UU}|_{C^3_{U,z}}\lesssim r^{-1}d_5+d_5^2=:\mathfrak a(r),
 \qquad |q'^{Uz}|_{C^3_{U,z}}+|q'^{rz}|_{C^3_{U,z}}\lesssim d_5.
$$
Here and below the suprema are on a fixed subinterval inside the padding. Under the change, estimates for $G,E$ can be evaluated at the old $u$ by the $J$-supremum in (O1). Define $\mathfrak b=d_5+\mathfrak a$. On any tail $r\ge R\ge R_1$,
$$
 \int_R^\infty r\mathfrak a\,dr+\int_R^\infty \mathfrak b\,dr
 \lesssim |\epsilon|R^{(\delta-1)/2}+\epsilon^2R^{\delta-1}. \tag{O4}
$$

Represent a physical cut by $U=F(r,z;\lambda)$. With $p=F_z$, the null conormal equation is
$$
 q'^{UU}-2q'^{Ur}F_r-2q'^{Uz}p+q'^{rr}F_r^2
                +2q'^{rz}F_rp+q'^{zz}p^2=0.
$$
Since $q'^{Ur}=-1/B+O(d_2)$, its small root $F_r=H(r,U,z,p)$ is smooth. On a fixed small $p$-interval $H(\cdot,0)$ and $H_p(\cdot,0)$ have $C^3_{U,z}$ bounds $C\mathfrak a,C\mathfrak b$, respectively, and $H_{pp}$ and its needed derivatives are bounded. Impose $U(T)=\lambda,z(T)=\zeta,p(T)=0$ at a large terminal radius. The contact equations are
$$
 z'=-H_p,qquad U'=H-pH_p,qquad p'=H_z+pH_U. \tag{O5}
$$
These are obtained by differentiating $F_r=H$. Conversely, the contact error $U_\zeta-pz_\zeta$ satisfies $(U_\zeta-pz_\zeta)'=H_U(U_\zeta-pz_\zeta)$, so vanishes with the terminal data. Invert $z(r;\lambda,\zeta)$ to obtain $F$, as soon as $z_\zeta$ is bounded away from zero.

Here are bounds uniform in $T$. For $A_0(r)=\int_r^T\mathfrak a(s)ds$, the integral equation and $|p'|\le C(\mathfrak a+(\mathfrak a+\mathfrak b+|p|)|p|)$ give $|p|\le C A_0$. Indeed bootstrap this estimate with a larger constant, use $\int_R^T A_0\le\int_R^T(s-R)\mathfrak a(s)ds$, and apply Gronwall with small exponent (O4). Then
$$
 |U-\lambda|\lesssim\int_r^T(\mathfrak a+p^2),\qquad
 |z-\zeta|\lesssim\int_r^T(\mathfrak b+|p|).
$$
In the variation equations the right-hand side for $\delta p$ is bounded by
$$
 C(\mathfrak a+\mathfrak b|p|+p^2)(|\delta U|+|\delta z|)
       +C(\mathfrak a+\mathfrak b+|p|)|\delta p|.
$$
If $S$ is the supremum of $|\delta U|+|\delta z|$, then $\int_R^T|\delta p|\lesssim S\int_R^T(s-R)(\mathfrak a+\mathfrak b|p|+p^2)ds$. The first moment of $\mathfrak b|p|+p^2$ is small by $\sup_{r\ge R}r|p|\lesssim\int_R^T s\mathfrak a(s)ds$. The equations for $\delta z,\delta U$ have respective $\delta p$ coefficients bounded by $C$ and $C|p|$, and the other coefficients are bounded by $C(\mathfrak b+|p|)$ and $C(\mathfrak a+p^2)$. Consequently their variations differ from terminal values by at most $CS$ times the small bound in (O4). This gives $z_\zeta=1+O(\epsilon)>0$ and a uniformly invertible change of cut labels, with analogous tail bounds by starting at any $R\ge R_1$. A second variation uses the same linear equations and a forcing quadratic in first variations. The forcing in the $p$-equation is bounded by $C(\mathfrak a+\mathfrak b|p|+p^2)+C(\mathfrak b+|p|)|\delta p|+C|\delta p|^2$ for unit terminal variations. The same first-moment estimates give bounded second variations and the needed bounds for $p_\lambda$.

These estimates also give a Cauchy limit as $T\to\infty$ in the cut and generator maps and their first variations on finite radial subintervals. Compare two terminal radii by restricting the longer solution to the shorter one $T$. Its terminal errors in $U,z$ and their first variations tend to zero by the tail norms, and its errors in $p,\delta p$ are $o(1/T)$ by (O4). In backward stability a terminal $p$ error enters $U,z$ variation estimates with at most the interval length multiplier $O(T)$, by the same Volterra estimates just given. Apply the same bounds to differences of the variation equations, using the second-variation bounds for local Lipschitz control. This proves convergence and uniqueness among small terminal solutions with $U\to\lambda,z\to\zeta$ and $r(p,p_\lambda,p_\zeta)\to0$. It proves existence of the eikonal $u_B$ after the common time normalization below. Since $q^{ur}$ stays away from zero and the covector stays close to $du$, the generator radius is strictly monotone. Positive $z_\zeta$ gives global compact injectivity on each radius (a degree-one periodic lift); the sphere factor remains positive since $r\ge R_1$.

Inverting $U=u-E/(2B)$ gives the original-chart graph $u=f(r,z;\lambda)$ with
$$
 f-\lambda=O_{C^1_{\lambda,z}}(|\epsilon|r^{-1+\delta/2}),
 \qquad f_{\lambda z}=O(|\epsilon|r^{-1+\delta/2}).
 \tag{O6}
$$
In fact $\int_r^\infty\mathfrak a\lesssim |\epsilon|r^{(\delta-3)/2}+\epsilon^2 r^{\delta-2}=o(r^{-1})$ at fixed seed, and $\int_r^\infty p^2$ is also $o(r^{-1})$ by $r^{-1}\sup_{s\ge r}s|p(s)|$ times $\int_r^\infty|p|$. Thus the slower graph correction in (O6) comes from the explicit $E$ recharting. The proof controls the graph at infinite radius and does not interchange limits in $r$ and seed.

## 4. Exact length and trace

At $r=r_5$ fixed, the exact tangent to an optical cut is $\partial_z+f_z\partial_u$. Set $P_\ell=q_{zz}+2q_{uz}f_z+q_{uu}f_z^2$ evaluated at $u=f$. Then
$$
 \ell=\int_0^L\sqrt{P_\ell}\,dz,qquad
 \ell_\lambda=\tfrac12\int_0^L P_\ell^{-1/2}
 \left[f_\lambda\bigl(q_{zz,u}+2f_zq_{uz,u}+f_z^2q_{uu,u}\bigr)
             +2(q_{uz}+q_{uu}f_z)f_{\lambda z}\right]dz . \tag{O7}
$$
This formula is coordinate invariant under compact relabeling of the cut. The Schur complement gives $q_{zz}-b^2=q_{za}h^{ab}q_{bz}$, quadratic in the compact cross terms. By (O1), (O6), and $|b_u|+|b_{uu}|\lesssim r^{-1}+d_5$,
$$
 \ell/L=\langle b(\lambda,r,z)\rangle+O_{C^1_\lambda}(|\epsilon|r^{-2+\delta/2}+\epsilon^2r^{-2+\delta}).
$$
The length difference multiplied by $r$ vanishes since $\delta<1$. The logarithm and $r_E/r\to1$ give the same field limit whenever the rescaled mean scalar has a limit. The factor $r_E/r\to1$ does not by itself control the radial derivatives of $r_E$ needed in a flux calculation. Also $\rho/r=1+O(\epsilon^2r^{-2+\delta})$ for the effective density in this areal chart. In fact $\det(\sqrt{-\det h}h^{-1})=-1$ pointwise and replacing its average by the symbol evaluated at the mean $h$ incurs an error quadratic in $P h$. Thus the field limit of the GOAL-006 effective scalar, if established, equals the physical length field limit. Its weak derivative is the geometric news after changing the time normalization. This use of equality of limiting field functions does not differentiate the $v(r,u)$ graph at fixed radius. A claim about strong convergence of fixed-radius tube derivatives still needs the separate $\partial_v\psi$ term identified by the independent audit.

## 5. Slice, clock and parity

Start with a time coordinate $\tau$ adapted to the audited initial slice, original spatial radius coordinate $r_{\rm orig}$, and compact $z$ with $\beta^z|_\Sigma=0$. The exact Z data have $\gamma_{zz}=1,\gamma_{z r_{\rm orig}}=0,K_{zz}=0$. Thus $q_{za}=0$ on $\Sigma$, and $\partial_\tau\gamma_{zz}=0$ there by the evolution of $\gamma_{zz}$, since $\partial_i\beta^z|_\Sigma=0$. The Schur formula gives $b-1=0,db=0$ exactly on $\Sigma$. Replacing $r_{\rm orig}$ by $r_5$ while holding $\tau,z$ fixed does not change $b=(g^{-1}(dz,dz))^{-1/2}$ as a scalar. The common effective rechart and both $z$-fixed optical changes preserve the same pulled-back jet. In the effective chart $\Sigma$ is still a common curve, so the free term of the scalar estimate is zero in this initial gauge. At first order the completed gauge has $b^{(0)}=1,b^{(1)}=0$; consequently the second coefficient of $b$ is unchanged by any subsequent $z$-fixed orbit rechart. In the first-order areal rechart generated by an orbit vector $\xi$, $h^{(1)}$ gains $\mathcal L_\xi \bar h$ and the compact shift gains $\partial_z\xi$. These cancel in $K_z^{(1)}=(\partial_z h^{(1)}-\mathcal L_{X^{(1)}}\bar h)/2$, leaving the completed unitary curvature and $T_z^{(1)}=0$. Since $b^{(1)}=0$, the B1 cross-current still has vanishing quadratic coefficient and the mean source agrees with the pinned unitary calculation. The explicit $q_{zz}$ length on a tilted cut can have a different finite-radius jet. Equation (O7), with the actual intersections of cuts and $\Sigma$, handles this difference. For instance, in the original slice the tangent to the intersection circle has squared length $1+\gamma_{r_{\rm orig} r_{\rm orig}}(\partial_z r_{\rm orig})^2$. A proof using a separate full-optical lapse PDE with compact coordinates transported along generators would have to transform the lapse jet again, because that last compact coordinate change alters $dz$.

The outgoing label on the common initial curve can be fixed by $u|_\Sigma=\sigma(r)=-r-2\log(r-1)$ in $r_0=1$ units. This agrees exactly with GOAL-005 at zero seed and uses invariant areal radius for the seed family. The normalized physical clock must satisfy $du_B/d\lambda=B_\infty(\lambda)$, since the asymptotic optical quotient cross coefficient before this normalization is $-B_\infty$. To see this, use the generator labels $\zeta$ from (O5). The $z$ drift and its first cut-label derivatives tend to zero by (O4), its radial derivative is $O(\mathfrak b+|p|)$, and the corresponding $u$ graph corrections and derivatives vanish by (O6) and the Hamiltonian equation. Einstein-frame scaling tends to unity. The mass logarithm is already in the effective background coordinate $\sigma$ and outgoing foliation, and the compact construction adds no logarithm. The existence and the $O(\epsilon^2)$ normalization of $B_\infty$ are explicit mean-exterior assumptions here.

One geometric additive anchor uses a chosen background cut $\lambda_*^0=\sigma(r_*)$ within the padded window. Fix the physical cut whose intersection with $\Sigma$ has circumference-weighted mean $r_5=r_*$ and set $u_B=\lambda_*^0$ on it. With the even-sector estimate in §1, this cut has $\lambda_*=\lambda_*^0+O(\epsilon^2)$. To verify the order, the correction $U-\lambda$ in (O5) is the integral of $H(r,\lambda,z,0)$ at fixed $z$ up to $O(\epsilon^2)$: $(U-\lambda,z-\zeta)=O(\epsilon)$, the $C^1$ bounds in (O4) control the change of integrand, and $p^2$ has integral $O(\epsilon^2)$. Replacing the generator label by $z$ incurs the same order. The linear part of $H(r,\lambda,z,0)$ comes from the compact mean-free terms in (O3) and the $E B_r$ correction. Its even mean-free part is $O(\epsilon^2)$ in the same norms and its compact mean is zero. The coordinate correction $E/(2B)$ has mean zero before evaluation and only $O(\epsilon^2)$ mean after evaluation. Continue through the finite strip by (O5) and the same expansion on compact intervals. Since $\sigma'$ is bounded away from zero, $u=f$ intersects $u=\sigma(r)$ transversely, and the mean areal radius of the intersection differs by $O(\epsilon^2)$ from $\sigma^{-1}(\lambda)$. The circumference measure on the intersection has density $1+O(\epsilon^2)$ in the original slice coordinates by the exact tangent formula. The derivative of the mean radius with respect to $\lambda$ is close to $(\sigma^{-1})'$ by the first variation, so the geometric anchor exists uniquely near $\lambda_*^0$. Integrating $B_\infty$ from this anchor gives $|u_B-\mathrm{id}|_{C^1(J')}\lesssim\epsilon^2$ on a smaller padded interval $J'$. A spatial-corner anchor at $u=-\infty$ would additionally need an integrable $B_\infty-1$ there.

For an exact half-translation-equivariant development, every construction above commutes with $z\mapsto z+L/2$: $r_5$, the average effective density, $W$, the terminal data and their unique solution, the generator flow, the anchor integral and $B_\infty$. Thus the physical length and clock are even under $\epsilon\mapsto-\epsilon$ at corresponding cuts. This conclusion uses actual uniqueness and estimates at the terminal boundary. Evenness alone gives no quartic remainder.

### Actual GL tilt at the background slice crossing

The pinned first jet gives a quantitative nonzero tilt correction in the fixed-$(u_B,r_5)$ circumference at the **background** crossing of a cut and $\Sigma$. In the completed static chart let $R_1=-h_{\theta\theta}/(2r)$ and $U_1=\frac12\int_r^\infty h^{(u,r)}_{rr}(u,s,z)\,ds$ be the first-order coordinate displacements in radius and optical label. The tangent shifts on a fixed cut are $\xi^r=R_1$ and $\xi^t=R_1/f-U_1$. Because $h_{zA}=0$ globally at first order and the completed Z scalar fixes $\langle q^{(2)}_{zz}\rangle=0$ at the background crossing, the quadratic circumference coefficient there is
$$
 [\ell/L]^{(2)}=\langle R_{1,z}U_{1,z}\rangle-\tfrac12 f\langle U_{1,z}^2\rangle.
$$
Write $q_*=\sqrt{k^2+\Omega^2}$ and $A_*=e^{\Omega t}U_{\rm GL}(r)$. The pinned reconstruction with $y=(1+k^2r^3)U_{\rm GL}/r$ gives $H_{tt}\sim-q_*^2U_{\rm GL}/(k^2r)$, $H_{tr}\sim\Omega q_*U_{\rm GL}/(k^2r)$, $H_{rr}\sim-\Omega^2U_{\rm GL}/(k^2r)$, and $H_{\theta\theta}/r^2\sim-U_{\rm GL}/(2r)$. Therefore $h_{rr}^{(u,r)}\sim -(q_*-\Omega)^2 A_*\cos(kz)/(k^2r)$ and $U_1\sim-(q_*-\Omega)A_*\cos(kz)/(2k^2r)$. The last integration uses $(\log |e^{\Omega t}U_{\rm GL}|)_r=-(q_*-\Omega)+O(r^{-1})$ along the background outgoing ray. Since $R_1\sim A_*\cos(kz)/4$, the leading circumference correction is $-(q_*-\Omega)A_*^2/(16r)$, strictly negative for sufficiently large $r$. This is the same result as [the coordinator's calculation](DERIVATIONS.md#4-quadratic-matching-and-the-nonzero-finite-anchor-correction). It was checked after seeing that proposed coefficient, by recomputing the pinned reconstruction and the cut tangent. The perturbed fixed-radius cut generally samples points off the exact $\Sigma$. Its Taylor coefficient at the crossing is defined using the smooth local Cauchy extension across the slice and differs from the intrinsic circumference of the actual intersection $\Sigma\cap\{u_B=\lambda\}$. This finite-radius correction decays exponentially on the early exterior and is consistent with the equality of the limiting traces.

## 6. Negative control and dependency

For $3/2<\nu<2$ take the manufactured, nonvacuum metric
$$
 q=-f_0(r)du^2-2du\,dr+e r^{-\nu}\cos(kz)dr^2+dz^2,
 \qquad f_0=1-1/r,
$$
with $e>0$ arbitrarily small. All differentiated oscillatory shell bounds of the type (O1) hold even with $\delta=0$, but the first radial moment of the raw compact forcing diverges. Finite terminal eikonal graphs with $u(T,z)=\lambda$ develop a conjugate point in backward propagation from sufficiently large $T$. At $z=p=0$ reflection fixes the generator. Let $J=\partial_\zeta z$, $P=\partial_\zeta p$. The exact small-root graph Hamiltonian obeys $H_p=0$, $H_U=0$, $-H_{pp}\asymp1$, and $H_{zz}\le-c e r^{-\nu}$ on that generator for large $r$: at $p=0$, $H=a/2+O(a^2)$ where $a=e r^{-\nu}\cos(kz)$. Thus $J'=\alpha(r)P, P'=-\beta(r)J$, with $\alpha\asymp1, \beta\ge c e r^{-\nu}$, and $J(T)=1,P(T)=0$. In backward time with independent variable $x=\int_r^T\alpha(s)ds$ the equation is $J_{xx}+(\beta/\alpha)J=0$. Sturm comparison on $T/2\le r\le T$ forces a first zero within $O(e^{-1/2}T^{\nu/2})=o(T)$ as $T\to\infty$. The metric violates the vacuum $zz$ equation already to linear order, $\delta\operatorname{Ric}(g)_{zz}=-f_0\partial_z^2a/2$. This is an obstruction to a purely absolute metric estimate for terminal Jacobians from (O1) without using field equations. It is not a caustic for the damped GL family. The vacuum identity (O3) supplies precisely the missing signed improvement. The example's effective label can be made null by an $O(a^2)$ mean radial rechart, with $u=\lambda$ on each terminal tube still equivalent to prescribing a constant shifted label there.

The conditional implication is
$$
 \text{areal mean-null vacuum exterior and commuted flux (O1)}
 \Longrightarrow\text{(O3) and terminal optical graph}
 \Longrightarrow\text{geometric scalar field trace from the mean trace}.
$$
The remaining analytical input is to construct the actual exterior with these norms (or transfer stronger norms from an evolution gauge), control the mean clock and potential, and establish the quartic forcing estimate and full five-dimensional asymptotic flux regularity. Under the stated assumptions this route removes a separate compact first-moment hypothesis. It does not establish those hypotheses for the physical initial data or turn a circle-length trace into a complete asymptotic expansion of the five-dimensional metric.

Sources used: [GOAL-005 completed gauge and Z data](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/DERIVATIONS.md), [GOAL-006 B1–B4 and optical identity](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md), and [independent audit, §§3–4](https://github.com/inaeyk/GL-with-AI/blob/c4cac8f2e2dd12cfbdf50651863d2188a75fc87c/research/sav/verification/AUDIT_GOAL006_80f030df.md).
