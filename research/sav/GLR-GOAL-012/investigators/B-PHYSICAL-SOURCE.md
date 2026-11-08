# B. Causal physical source, fixed canonical data and a news error bound

This is a fixed-background calculation in the ordered axial hierarchy, linear in the real seed $\eta$. It uses the $L/r_0=10$ normalized growing GL mode, $r_0=2G_4 M_{\rm ADM}$, and the single $R/r_0=13/8$, $\delta=0.12$ packet. There is no finite-seed remainder claim. The main new results are the explicit induced velocities in the massive two-field preparation, a positive energy estimate on a truncated null diamond for both massless and massive stages, and a computable scalar continuation from a finite characteristic to the Bondi cut. These are bounds in terms of residual and coefficient envelopes, not an evaluated error bar for a physical candidate waveform.

## Causal domain and coefficients

Use $f=1-r_0/r$, $x=r+r_0\log(r/r_0-1)$, $t=v-x$, $u=t-x$, and $p(x)=T'(x)=-r_0/r$ for the graph $t=T(x)=r-x$. Let $a=u_*=u_\Sigma(R)$ and $b(u)=r_\Sigma(u)$, where $u_\Sigma=r-2x$, so $b'(u)=-f/(2-f)$. The graph is spacelike on $r>r_0$. For fixed $U>a$ and $V_c\ge b(a)$ define
$$
 D(U,V_c)=\{a\le u\le U,\quad b(u)\le v\le V_c\}.
$$
The $u=a$ side is zero, including all differentiated traces; the flat bump vanishes to all orders at its edge. The first possible nonzero Bondi cut is $a$, and the waveform is zero before it. This uses local finite propagation of the triangular systems below, since every axial source is linear in already causal lower axial fields. Spherical constraints may sample a wider region, but their solution is a prescribed coefficient, not axial radiation data outside the diamond. The past of a point in the domain has no dependence on axial data at $r<b(U)$; $x$ there is bounded below, and the exterior of the black string is enough. There is no artificial ingoing boundary condition to impose in this finite diamond. The $v=V_c$ cap and the $u=U$ side are outgoing boundaries for the estimate. For $r_0=1$, $a=-0.6849927415\ldots$; a window ending one period after $a+0.12$ has $U\simeq7.84237$ and $b(U)\simeq1.0119484$ ($f\simeq0.0118073$). Later choices of $U$ sample closer to the horizon, so constants need not be uniform in $U$.

Use $\lambda=4$ and the exact axial densities from [GOAL-011 HIERARCHY](../../GLR-GOAL-011/HIERARCHY.md), (1)–(5). In particular the exact gauge-invariant one-form $A$ is formed from the completed metric before taking coefficients. Expand $P_j,M_j$ with the inverse metric and weight formulas in (1)–(2), and retain both $P_1F_1,M_1 A_1$ and $P_2F_0,M_2 A_0$ in the mean. The latter contain the direct quadratic change and the spherical Einstein-frame mass and lapse, not just a square of the optical phase. On the real domain the regular ingoing EF representative has
$$
 \mu(v,r)=\tfrac12\int_r^\infty \rho^2\mathcal T^v{}_v(v,\rho)\,d\rho,
 \qquad \beta(v,r)=-\tfrac12\int_r^\infty \rho\mathcal T_{\rho\rho}(v,\rho)\,d\rho,
$$
with $\mathcal T$ the trace-reversed mean quadratic GL Ricci coefficient specified in HIERARCHY (3). The zero integration constants at fixed $v$ set the mass perturbation to zero and normalize the clock. The GL profile is future regular and decays as $e^{\Omega t-q x}$ up to powers at the right end, $q=\sqrt{k^2+\Omega^2}$. It gives the same time factor $e^{2\Omega v}$ in both EF integrals. Keeping $g_{E,vv}^{(2)}=2\mu/r-2f\beta$, $g_{E,vr}^{(2)}=\beta$ includes the mean curvature and its derivatives. The pure mean radion drops from the projected noncompact $P,M$ and exact $Q_z$; its sphere-radius and orbit conformal weights cancel as in HIERARCHY. The second harmonic of the spherical geometry does not return at this order. This elimination does not identify the radion with tensor radiation.

The derivation below uses the actual $+k$ Fourier coefficient, so the first GL insertion carries the factor $1/2$. Add the conjugate coefficient for $-k$. Equivalently one may evolve the full complex cosine amplitude with the factor removed and use the factors of $1/2$ in the mean products. Never apply both conventions in the same product. In the vanishing GL limit all sources and induced upper velocities below vanish; no independent higher canonical data remain to excite the upper equations.

## Exact-curl massive evolution and all induced velocities

The baseline $w_0=\Psi_0=2Q_{0z}/(\lambda r)$ satisfies $L_{\rm RW}w_0=0$, with $w_0|_\Sigma=a_\delta(u)$ and $(w_{0,t},w_{0,x})|_\Sigma=(a_\delta',-a_\delta')$. Reconstruct $A_0,F_0$ from this exact curl and the two massless axial density equations. For the mixed equations in the product tortoise chart define the supplied densities $J^a=\lambda(M_1 A_0)^a_{+k}$ and $X=(P_1 F_0)^{tx}_{+k}$. Compact-unitary $H_{za}=0$ and the baseline's zero compact curl imply that $X$ is the only supplied curl correction and that $J^z=0$. Let $U_m=-P^{tx}_{\rm total}=-Q_{1z}=r^4F_{1,tx}/f-X$, and keep the compact potential $A_z$ (the mixed Fourier coefficient). Put
$$
 d_k=\lambda+k^2r^2,\quad D=r^2d_k,\quad L_c=r^4/D,\quad W_c=\lambda r^4/d_k,
 \quad y=U_m/\sqrt D,\quad z_c=\sqrt{W_c}A_z.
$$
The $t,x$ density equations give the reconstruction
$$
 A_t=\frac{J^t-U_{m,x}-ik r^4 A_{z,t}}{D},\qquad
 A_x=\frac{-J^x-U_{m,t}-ik r^4 A_{z,x}}{D},\qquad
 F_{1,tx}=\frac f{r^4}(U_m+X).
$$
The curl definition and compact density equation reduce to
$$
\begin{split}
(-\partial_t^2+\partial_x^2-V_y)y+C\partial_t z_c
 &=\sqrt D[\partial_t(J^x/D)+\partial_x(J^t/D)]+f\sqrt D X/r^4,\\
(-\partial_t^2+\partial_x^2-V_z)z_c+C\partial_t y
 &=[J^z-ik\{\partial_t(L_c J^t)+\partial_x(L_c J^x)\}]/\sqrt{W_c},\\
 K&=f(k^2+\lambda/r^2),\quad a_m=(\log\sqrt D)_x,\quad b_m=(\log\sqrt{W_c})_x,\\
 V_y&=K+a_m^2-a_{m,x},\qquad V_z=K+b_m^2+b_{m,x},\qquad C=2ik\sqrt\lambda f/d_k.
\end{split}
$$
For example in the curl, the two terms in $A_{z,tx}$ cancel and leave $ik L_{c,x}A_{z,t}$ before rescaling; on moving the background part of $f(U_m+X)/r^4$ to the left one obtains the displayed positive $X$ source. In the compact equation the same cancellation leaves the other time-derivative coupling. Substitution back gives all three original density equations, so taking their divergence imposes the original sourced angular constraint. There is no freely propagated fourth component. Setting $k=0$ in this algebra gives a decoupled compact equation and $V_y=V_{\rm RW}$ in the noncompact row, with $y=-\sqrt\lambda\Psi/2$ for the exact curl. The known mixed background-curl formulation uses $y+X/\sqrt D$ and a correspondingly different source and data.

On the launch graph use the canonical density with future normal $n_a\parallel(-1,p,0)$. A graph density in the $(x,z)$ slice coordinates has $\pi^x=\pi^r$ under the radial change. At mixed order
$$
 \pi^x=-U_m,\qquad \pi^z=-r^4[(A_{z,t}+pA_{z,x})-ik(A_t+pA_x)] .
$$
The second equality has no explicit curl correction in the compact slot, since $P_1 F_0$ has no such component. The fixed original canonical pair therefore implies $U_m=A_z=0$, $A_x+pA_t=0$ and $\pi^z=0$ on $\Sigma$, but not zero transverse velocities. Differentiate the two zero position traces tangentially, so $U_{m,x}=-pU_{m,t}$ and $A_{z,x}=-p A_{z,t}$. Combining the two reconstruction equations with the first tangential condition gives $U_{m,t}$. Combining them with $A_{z,t}+pA_{z,x}=ik(A_t+pA_x)$ reduces $D$ to $\lambda r^2$. Thus
$$
 U_{m,t}|_\Sigma=\frac{pJ^t-J^x}{1-p^2},\qquad
 A_{z,t}|_\Sigma=\frac{ik(J^t-pJ^x)}{\lambda r^2(1-p^2)} . \tag{B1}
$$
The velocities for $y,z_c$ are obtained by the same stationary rescalings. A check on the normal constraint is that $A_t+pA_x=(J^t-pJ^x)/(\lambda r^2)$ on $\Sigma$. In particular $\pi^z=0$ does not allow setting the derivative of the compact potential to zero unless this specific current combination vanishes.

For the returning scalar $w_2=2Q_{2z}/(\lambda r)$, put $\mathcal C=\langle P_1F_1+P_2F_0\rangle^{tx}$ and $\mathcal D^a=\lambda\langle M_1^{ab}A_{1b}+M_2^{ab}A_{0b}\rangle$. The tortoise density has $\mathcal D^x=\mathcal D^r_{(t,r)}$ and $\mathcal D^t=f\mathcal D^t_{(t,r)}$. Then
$$
 L_{\rm RW} w_2=S_2=\frac{2r}{\lambda}\left[-\partial_t(r^{-2}\mathcal D^x)-\partial_x(r^{-2}\mathcal D^t)-\lambda f r^{-4}\mathcal C\right],\qquad V_{\rm RW}=f(6/r^2-3r_0/r^3).
$$
The positions are $Q_{2z}|_\Sigma=0$ and the fixed tangential condition gives
$$
 \partial_t Q_{2z}|_\Sigma=\frac{\mathcal D^x-p\mathcal D^t}{1-p^2}
 =\mathcal D^r_{(\tau,r)}+\frac{r_0/r}{1+r_0/r}\mathcal D^\tau_{(\tau,r)}. \tag{B2}
$$
Here the last density transformation uses $\mathcal D^t=f\mathcal D^\tau_{(\tau,r)}+p\mathcal D^r_{(\tau,r)}$. The expression for $\partial_t w_2$ follows by rescaling. For a numerically better conditioned source assembly near the horizon let $P_e=r^{-2}\mathcal D^r_{(v,r)}$, $V_e=r^{-2}\mathcal D^v_{(v,r)}$, so the two Schwarzschild derivatives in the strong source combine to $f[\partial_r(P_e-fV_e)-\partial_v V_e]$. This factorization uses regular EF derivatives at fixed $v$; the separate large terms may not be bounded independently without worsening the enclosure. In (B2), $\mathcal D$ is evaluated with the constrained mixed field on the graph, not with all its components set to zero.

The physical trace is $C_{21}=\lim_{v\to\infty} w_2(u,v)$ and $N_{21}=\partial_u C_{21}$ in the pinned frame (units $r_0$ for shear, dimensionless news). Tensor normalization is that of [GOAL-011 WAVEFORM](../../GLR-GOAL-011/WAVEFORM.md): $C_{AB}=C X_{AB}$ and $dE_T/du=12\dot C^2/(32\pi G_4)$. The compact products in the Einstein-frame extraction vanish on fixed cuts; the mass/lapse constants set above remove a spurious asymptotic cut shift. The graph velocities (B1)–(B2) incorporate the upper contacts in an untransformed evolution. If one uses a step extension and a Laplace transform, they must be included as graph distributions along with the derivative commutators of the lower fields. Replacing $w_2$ by the background-only curl subtracts $2\mathcal C/(\lambda r)$ from the master, its entire graph jet and its RW operator from the equation; it must not simply remove $\mathcal C$ from the strong source. For a decaying local extraction change $Z$, the simultaneous change by $L_{\rm RW}Z$ and the graph jet of $Z$ produces exactly $Z$ in the retarded solution and zero null trace. This is a negative control on an apparent pole caused by separating contacts and bulk.

## Positive characteristic energy and residual certification

For a direct characteristic implementation put $P=w_u$, $Q=w_v$ and
$\mathscr F=(-[K+\mathsf A^2-\mathsf A_x]w+\mathsf C(P+Q)-S)/4$.
Let $\ell(v)=b^{-1}(v)$ below $b(a)$ and $a$ at or above $b(a)$.
Then
$$
 P(u,v)=P_\Sigma(u)+\int_{b(u)}^v\mathscr F(u,v')dv',\qquad
 Q(u,v)=Q_L(v)+\int_{\ell(v)}^u\mathscr F(u',v)du',\qquad
 w(u,v)=w_L(v)+\int_{\ell(v)}^uP(u',v)du'.
$$
The left values are $Q_L=Q_\Sigma(\ell(v))$, $w_L=w_\Sigma(\ell(v))$ on the graph and zero beyond $b(a)$. The graph relation $w_\Sigma'=P_\Sigma+b'Q_\Sigma$ makes the two derivative reconstructions compatible. The integral map is a contraction on sufficiently small characteristic cells by boundedness of the coefficients and the lengths of the two integrals. Marching the cells in causal order constructs the solution on each compact diamond, taking the lower fields before the upper sources. This is a time-domain construction with no condition at a finite right radius or on a rotated ray.

The following estimate is exact for either scalar RW equation or the two-field massive equation above. Write the pencil as $-w_{tt}+w_{xx}-[K+\mathsf A^2-\mathsf A_x]w+\mathsf Cw_t=S$, with $\mathsf A=0,K=V_{\rm RW},\mathsf C=0$ for RW, and $\mathsf A=\mathrm{diag}(a_m,-b_m),\mathsf C=C\sigma_1$ for the massive equation. The coupling $\mathsf C$ is anti-Hermitian. With $D_x=\partial_x+\mathsf A$, define
$$
 \mathcal E=\tfrac12(|w_t|^2+|D_xw|^2+K|w|^2),\qquad
 \mathcal J=-\Re(w_t^\dagger D_xw).
$$
Stationarity and $D_x^*D_x=-\partial_x^2+\mathsf A^2-\mathsf A_x$ give $\partial_t\mathcal E+\partial_x\mathcal J=-\Re(w_t^\dagger S)$ pointwise. The graph energy is $\int_\Sigma(\mathcal E-p\mathcal J)dx\ge0$. With
$$
 \xi_u=w_u-\mathsf Aw/2,\qquad \xi_v=w_v+\mathsf Aw/2,
$$
the outgoing fluxes on fixed $v$ and fixed $u$ are respectively $F_v=\int(|\xi_u|^2+K|w|^2/4)du$ and $F_u=\int(|\xi_v|^2+K|w|^2/4)dv$. The factors follow from $dt\,dx=du\,dv/2$ and $w_t=\xi_u+\xi_v$. The identities hold also on all subdiamonds ending at smaller $u,v$. If $v<b(a)$, the left corner of the subdiamond is the intersection with the graph and there is no extra left side. The free RW packet has graph kinetic energy exactly $\int(a_\delta')^2du=1$ because $w_x=-w_t$ on the graph; this checks the tilt sign.

For an error field $e=w-\widetilde w$, let $E_\Sigma$ be its graph energy; add any incoming error flux on $u=a$ if the reconstruction does not set it identically to zero. Let $R$ be the error equation's volume source, including the differential residual of the smooth candidate and upstream source errors, and extend $R$ by zero outside the domain in the mixed norms below. Set
$$
 B=\frac12(\|R\|_{L^1_vL^2_u}+\|R\|_{L^1_uL^2_v}),\qquad
 M=\max\left(\sup_v F_v[e]^{1/2},\sup_u F_u[e]^{1/2}\right).
$$
Integrating the energy law on each subdiamond gives $M^2\le E_\Sigma+B M$, because $\int|\xi_uR|du\le M\|R(\cdot,v)\|_2$ and similarly for $\xi_v$ at fixed $u$. Thus
$$
 M\le \frac{B+\sqrt{B^2+4 E_\Sigma}}2\le \sqrt{E_\Sigma}+B. \tag{B3}
$$
This gives a bound on the $L^2_u$ finite-cap news error $e_u(\cdot,V_c)$ for RW. If the residual norms are finite to $v=\infty$ and the trace limit is justified, it also bounds the Bondi news error. Piecewise polynomial candidates need either adequate smoothness for the claimed commutations or their interface residuals treated in the energy identity; grid convergence and local step tolerances alone do not supply $B$.

For clarity, one explicit way to propagate coefficient and lower-field error in $B$ is the following. For $c(v)\ge\sup_u|c(u,v)|$, $d=U-a$, and directional jets of any $e$ controlled by (B3), zero-extend across the graph. Direct Cauchy–Schwarz gives
$$
\begin{array}{c|cc}
 &L^1_vL^2_u&L^1_uL^2_v\\\hline
 c\xi_u & M\|c\|_1 & \sqrt d\,M\|c\|_2\\
 c\xi_v & \sqrt d\,M\|c\|_2 & d M\|c\|_\infty
\end{array}
$$
The position $\sqrt K e/2$ is controlled in both directional fluxes; use either row or the smaller applicable bound. Multiplication by $\mathsf A$ is included by its corresponding coefficient divided by $\sqrt K$; $K>0$ throughout the fixed diamond and polynomial losses at the outer end are harmless in GL products. Time derivatives commute with each background wave system. Fluxes for $e$ and $\partial_t e$ bound the mixed field and the time, mixed and pure spatial second derivatives in the return source, replacing $e_{xx}$ by its equation with the source residual included. For the baseline, also estimate $\partial_t^2 e_0$ when bounding $\partial_t S_1$, then use the same equation to remove higher spatial derivatives. Pure time derivatives of a given solution are the sum of the two directional jets at the preceding commutation order. Graph energies for these commutations must be obtained from the actual commuted data, not by setting them to zero. In particular, writing $\phi=w(T(x),x)$, $\psi=w_t(T(x),x)$ and $V=K+\mathsf A^2-\mathsf A_x$,
$$
 (1-p^2)w_{tt}|_\Sigma=\phi''-2p\psi'-p'\psi -V\phi+\mathsf C\psi-S|_\Sigma ,
$$
where the primes on graph traces are total $x$ derivatives. This follows by eliminating $w_{tx}$ between the first graph derivative of $\psi$ and the second of $\phi$. Higher time jets follow by applying the same rule to the time-commuted problem. For uncertain coefficients, decompose a source difference into exact coefficients on the error and coefficient errors on the candidate, and include both terms. A sum of interval whole-cell bounds for $c$ and the remaining differential residuals then supplies the mixed norms. On the outer tail, coefficient envelopes can be integrated analytically using the GL decay $e^{-(q-\Omega)x}$ with any strict loss to absorb powers; $q-\Omega\simeq0.568216/r_0$. The direct and fixed-mass mean products have twice this exponent before polynomial losses. This method needs real-radius coefficient and derivative bounds, including the EF mean tail and its $v$ derivatives, not just the complex overlap's final residue error.

## Finite-cap to null infinity

At $v\ge V_c\ge b(a)$, $w_2(a,v)=0$, so $w_2(\cdot,v)=I_u P(\cdot,v)$ with $P=w_{2,u}$ and $I_u P(u)=\int_a^u P(u')du'$. Its operator norm on $L^2[a,U]$ is at most $d$. The characteristic equation is $P_v=-(V_{\rm RW}I_uP+S_2)/4$. Let $r_c(u)=r((V_c-u)/2)$ and take $r_c(U)$ beyond the potential peak. Define
$$
 K_c=\frac d4\int_{V_c}^\infty \sup_{a\le u\le U} V_{\rm RW}(u,v)dv
 =\frac d2\left[\frac6{r_c(U)}-\frac{3r_0}{2r_c(U)^2}\right],\qquad
 B_c=\frac14\|S_2\|_{L^1_vL^2_u(v\ge V_c)}.
$$
The exact integral uses $dv=2dx$, $dx=dr/f$ along $u=U$. The absolute source bound $B_c$ is obtainable from the lower-stage energies with the preceding product estimate applied to $v\ge V_c$; this does not assume that the massive waveform is outgoing on the cap. The resulting source norm is finite, and the same argument provides a priori lower energy bounds from the exact graph energies, the baseline homogeneous equation and the mixed source. Scalar Volterra evolution now proves $P(v)$ converges in $L^2_u$ and gives
$$
 \|N_{21}-P(V_c)\|_2\le(e^{K_c}-1)\|P(V_c)\|_2+e^{K_c}B_c . \tag{B4}
$$
Integrating from the zero left edge yields the shear and an error controlled by the same bound times the norm of $I_u$. News at fixed $v$ uses $P$, not the finite-radius $w_t$; using $w_t$ would need a separate $w_v$ correction. The baseline obeys the same formula with $B_c=0$.

A sharper cap correction uses the homogeneous potential only. Let $\mathscr K P(u,v)=-\tfrac14\int_{V_c}^v V_{\rm RW}(u,v') I_uP(u,v')dv'$. The ordered $v'$ integrals give $\|\mathscr K^j\|\le K_c^j/j!$ in the norm $\sup_v\|P(\cdot,v)\|_2$. Starting from $P(V_c)$, approximate $N_{21}$ by $\sum_{j=0}^m\mathscr K^jP(V_c)$ at $v=\infty$, enclosing quadrature separately. The neglected potential terms are bounded by $[e^{K_c}-\sum_{j=0}^m K_c^j/j!]\|P(V_c)\|_2$; the omitted source is still bounded by $e^{K_c}B_c$. The first correction is explicit, $-\tfrac12[6/r_c(u)-3r_0/(2r_c(u)^2)]I_u P(V_c)$. An initial cap error from (B3) is amplified by at most $e^{K_c}$. In the example window above, $K_c\simeq0.25518$ for $r_c(U)=100r_0$, so three corrections bound the pure potential remainder by about $2\times10^{-4}$ of the cap news norm. This is a check of geometry and the bound's scale, not a physical waveform result. All the cap formulas reduce to exact free transport when $V_{\rm RW}=S_2=0$.

## Slower shifted terms and what can cancel them

Let $s_a=s_q+2\Omega$ (in $r_0=1$ units, approximately $-0.05136225-0.74734337i$). The candidate inherits the baseline pole at $s_2=s_a-2\Omega=s_q$. Suppose the returning RW outgoing inverse at $s_a$ and the massive outgoing inverse at $s_a-\Omega=s_q+\Omega$ are holomorphic on the sheet under consideration. Then the singular part of the returning transfer at $s_a$ is obtained by applying the exact nested mixed insertion and direct quadratic/mean insertion to the baseline radial residue, followed by the returning outgoing inverse and outgoing trace. Indeed, all compact graph contacts due to changes of independent upper radiative data are entire inputs and pass through inverses holomorphic at those two arguments. The forced contacts for the fixed physical data must still be retained in the complete trace; this analyticity argument only isolates the singular coefficient at the specified point. If the physical shifted coefficient is nonzero, adding compact free upper data cannot cancel it under the two holomorphy assumptions. Changing the baseline packet may change or remove the baseline residue itself. Neither freedom is part of the experiment.

The necessary asymptotic overlap margins appear favorable at this point. In units $r_0=1$, the outgoing continuation has $\kappa(s_q+\Omega)\simeq-0.1968904-0.4351559i$. The real-axis inner outgoing integrability margin $q+\Re(s_q+\kappa)$ is about $0.25668$; the outer mixed margin $q+\Re\kappa-|\Re s_a|$ is about $0.38324$; and the direct margin $2(q-\Omega+\Re s_a)$ is about $1.03371$. The corresponding horizon margins $1+2\Re(s_q+\Omega)$ and $1+2\Re s_a$ are positive. These are endpoint checks at rounded parameters using the rates in GOAL-011 C-RESONANCE, not homogeneous inverse tests or a continuation certificate. In particular, the GOAL-011 noncoincidence check at $s_q-\Omega$ does not test $s_q+\Omega$. There is no proof here that the mixed and direct source terms fail to cancel, and no shifted amplitude is asserted.

An order-two constant clock comparison adds a real multiple of $\partial_u C_{01}$, with the unshifted baseline pole at $s_q$. It cannot cancel a different isolated pole of the fixed trace at $s_a$. The actual frame and origin are fixed. A clock proportional to $e^{2\Omega u}$ would shift that frequency but is not the common inertial Bondi clock comparison. Decaying local extraction terms have zero physical trace when all graph and volume changes are carried together. For the massive thresholds at $s=\Omega\pm ik$ and the shifted baseline cut, the analytic-contact argument for $s_a$ does not apply at a singular inverse: upper mixed data can change the threshold excitation and the actual physical source may cancel it. A frequency shift alone does not establish any slower physical signal.

A declared pole subtraction can be tested from the causal waveform without resolving every other singularity. Fix its onset at the causal edge $a$ and use the news transform's pole numerator $s_q A_{21,q}$ and its conjugate. On any interval strictly after the edge this gives the stipulated $N_q=2\Re(s_q A_{21,q}e^{s_qu})$. Differentiating a *step-truncated shear pole* instead includes an onset delta and transfers a matching contact to the shear remainder's derivative. It cannot be treated as an $L^2$ news term. For a computed news approximation $n$, an error $\Delta$ from (B3)–(B4), and a reference pole news $n_q$ with $L^2$ error $\Delta_q$, one obtains
$$
 \|N_{\rm rest}\|_2\le\|n-n_q\|_2+\Delta+\Delta_q,
 \qquad \|N_q\|_2\ge \|n_q\|_2-\Delta_q,
$$
and the opposite triangle inequalities test failure on the stated interval. This comparison measures the remainder of a specified subtraction rather than inferring spectral content from a fit. For tensor interference one can use the same enclosures: if $\|N_j-n_j\|_2\le\Delta_j$ on the given cut interval for $j=0,2$, then the error in the dimensionless interference integral $\int n_0 n_2\,du$ is at most $\Delta_0\|n_2\|_2+\Delta_2\|n_0\|_2+\Delta_0\Delta_2$. The physical factor is $12/(16\pi G_4)$, with an additional $r_0$ if $u$ is dimensionless. This is not the total tensor power or a scalar/KK flux bound.

## Evidence and remaining work

- **Derived for the ordered hierarchy:** the exact-curl mixed/mean equations, induced graph data (B1)–(B2), causal edge and finite-diamond domain, positive error and cap estimates (B3)–(B4), and the conditional noncancellation statement for graph changes at an isolated shifted baseline pole with holomorphic upper inverses. The algebra uses the pinned field equations and does not independently certify the GL spectral input.
- **Numerical arithmetic only:** example cut/radius, endpoint margins and cap-tail estimate were evaluated in Python 3.14.8 using `math.log`, bisection of $-r-2\log(r-1)=U$, `cmath.sqrt` with the outgoing branch, and the displayed power-series remainder. At $r_c(U)=100r_0$ the latter is $0.00018609354\ldots$ for $m=3$. No time evolution, continuum residual enclosure or new frequency inverse was run by B.
- **Unresolved by this derivation:** real-radius interval envelopes for the physical time-dependent reconstruction and derivatives on all cells, actual smooth wave candidates and their residual/graph quadrature, and evaluated cap source norms. Without these inputs the estimates give no numerical tolerance or dominance statement. For the shifted signal, separately enclose the two homogeneous inverse gates, the nested residue overlap, and any near-threshold cancellations on the physical sheet. The finite-seed and nonlinear Bondi error are outside these linear ordered estimates.

Inputs inspected at the worktree pin [`eb439228601a649c658ce719b503ff0bdc37a83f`](https://github.com/inaeyk/GL-with-AI/tree/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011): HIERARCHY, FINITE-DATA, NULL-TRANSFORM, PAIRING, PHYSICAL-RESIDUE, WAVEFORM, EXTERIOR-SOURCE, and investigators C-RESONANCE, C-MIXED-QUADRATURE and B-MEAN-QUADRATURE. Also read the scoped AGENTS.md, Goal 012 contract, NOTATION.md, GOAL003_REVIEW_QUALIFICATIONS.md, and the current sections/index of HANDOFF.md, STATUS.md and verification/INDEX.md. The asymptotic identification of the CPM master with the shear and its tensor power agrees with [Martel–Poisson, arXiv:gr-qc/0502028v1, Eqs. (6.11)–(6.12), (6.16)](https://arxiv.org/pdf/gr-qc/0502028v1), inspected here; the normalization and gauge completion for the KK projection are supplied by the pinned project source. The derivation is an independent causal route given those inputs, with exposure to prior frequency-domain formulas and no independent audit of this note yet.
