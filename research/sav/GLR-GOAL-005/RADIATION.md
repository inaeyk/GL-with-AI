# Finite-slice radion waveform and energy

Use dimensionless $r,t,x,u,v$ with $r_0=1$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $f=1-1/r$. All cuts below use this fixed additive clock. Let $k=\pi/5$, $s=2\Omega>0$ and $q=\sqrt{k^2+\Omega^2}$. The mode $H$ is real with $H_{\theta\theta}(2)/8=1$. The first jet is $\epsilon e^{\Omega t}H\cos(kz)$, so $\epsilon$ fixes the growing amplitude relative to the Schwarzschild time origin. At the reference radius on $	au=0$, $t=0$, so this is also the normalized initial sphere-radius perturbation there. See [the normalization proof](DERIVATIONS.md).

## Causal Z preparation

On the regular slice $\tau=v-r=0$, prescribe zero completed quadratic scalar value and normal derivative. The global constraint construction for these data is in [CONSTRAINTS.md](CONSTRAINTS.md). No temporal multiplier is applied to the first-order metric. In the exterior $\psi=r\phi_2$ obeys
$$
 4\psi_{uv}=e^{s(u+v)/2}\,S(r)-V(r)\psi,\qquad
 S=\frac{\sqrt3 k^2rf}{16}H_{\mu\nu}H^{\mu\nu},\qquad V=f/r^3 .
$$
Here $r=r((v-u)/2)$ and $g(R)=-R-2\log(R-1)$ is the outgoing cut from radius $R$ on the initial slice. Write $R(u)=g^{-1}(u)$. Integration along null characteristics with both null derivatives initially zero gives
$$
 \psi(u,v)=\frac14\int_{g(v)}^u d\hat u
  \int_{R(\hat u)}^v d\hat v\,[e^{s(\hat u+\hat v)/2}S-V\psi].
 \tag{1}
$$
Thus the field and news are the limiting $\psi$ and $\partial_u\psi$ at fixed $u$ as $v\to\infty$. The free-source news and the exact curvature correction are
$$
 n_0(u)=\frac12\int_{x(R(u))}^{\infty} e^{s(u+x)} S(r(x))\,dx,\qquad
 n_Z(u)=n_0(u)-\frac14\int_{R(u)}^\infty V\psi\,dv .
 \tag{2}
$$
This is a finite-slice retarded response with curvature scattering. An alternative representation subtracts the inherited regular separated solution $e^{st}(L_s^{-1}S)(x)$, $L_s=-\partial_x^2+V+s^2$, leaving a homogeneous solution with the negative of that solution's Cauchy jet. The latter gives the same Z field by uniqueness. Dropping its transient or using the separated coefficient alone would implement different data.

The potential operator in (1) on the full early wedge $u\le g(A)$ has sup norm at most
$$
 \kappa_A=\int_{-\infty}^{g(A)}\frac{du}{4R(u)^2}
 =\frac14\left[2\log\frac{A}{A-1}-\frac1A\right].
 \tag{3}
$$
The integral on one ray is $\int V\,dv/4=1/(4R^2)$, since $dv=2\,dr/f$. If the free field has sup norm $B_0$, the error after $j$ successive potential updates from the free field is at most $B_0\kappa_A^{j+1}/(1-\kappa_A)$. Its contribution to the error in (2) has the additional factor $1/(4A^2)$ when the news is evaluated from the resulting iterate. These bounds concern exact integrals. A quadrature, source interpolation or mode error requires separate control. For a forcing error, define $B_\delta=\sup_{u,v}|\mathcal T \delta S|$ and $D_\delta=\sup_u|\partial_u\mathcal T\delta S(u,\infty)|$, with $\mathcal T$ the free retarded integral including the factor $e^{st}$ when appropriate. Then the news error is at most $D_\delta+B_\delta/[4A^2(1-\kappa_A)]$.

## Strict positive early cut interval

The Schwarzschild decaying master $U_{\rm GL}$ has
$$
 U_{\rm GL}=C_\infty e^{-qr}r^{-\beta}(1+O(r^{-1})),\qquad
 \beta=(k^2+2\Omega^2)/(2q),\quad C_\infty\ne0 .
$$
Its normalized reconstruction has leading $(H_{tt},H_{tr},H_{rr},H_{\theta\theta}/r^2)/(U_{\rm GL}/r)=(-q^2/k^2,\Omega q/k^2,-\Omega^2/k^2,-1/2)$. In the contraction the time-radial block contributes $1$ and the sphere contributes $1/2$. Consequently
$$
 S\sim S_* e^{-2qr}r^{-m},\qquad S_*=\frac{3\sqrt3 k^2 C_\infty^2}{32},\qquad m=1+2\beta.
$$
An independent route uses TT at the leading flat end. With $K=H_{\theta\theta}/r^2$, TT gives $H_{tr}\sim-\Omega H_{tt}/q,\ H_{rr}\sim\Omega^2H_{tt}/q^2,\ K\sim k^2H_{tt}/(2q^2)$. This makes the contraction $6K^2$ at leading order. The entire strong-field source is outside the domain of dependence of sufficiently early cuts.

Let $n_*=(S_*/[2(2q-s)])e^{-2qR}R^{-m-s}$. The factor $R^{-s}$ is from the $\tau=0$ slice. Changing integration variable to $r=R+w$ in (2) gives $n_0/n_*\to1$ by dominated convergence, using $2q-s>0$. For a fixed early cut let $B(u)=\sup_v|\psi(u,v)|$. In the region where the source is positive, (1) gives
$$
 B(u)\le \exp(\kappa_{R(u)})\int_{-\infty}^u n_0(\hat u)\,d\hat u,\qquad
 |n_Z-n_0|\le B(u)/(4R(u)^2).
$$
It follows that the curvature correction is $O(R^{-2}n_*)$. Dominated convergence in (1)–(2) gives the radiation limit, its derivative, and $\phi_2=\Phi_2(u)/r+O(r^{-2})$ for each fixed early cut. In particular $n_Z>0$ on all sufficiently early cuts. The incoming and outgoing characteristics in (1) are fixed by the finite Cauchy slice.

For an explicit window one can use $I_{\rm far}=[g(101),g(100)]$. The endpoint inequalities in [GOAL-004](../physics/goal004_source_geometry.md#strategy-1-endpointvolterra-control-and-monotone-eigenvalue-matching) and the [normalization bounds](DERIVATIONS.md) give, for $r\ge100$,
$$
 0.62<q<0.76,\quad 0<\beta<0.5,\quad s<0.83,\quad 2q-s>0.67,\qquad
 0.90<W_\infty<1.10,\quad |W_\infty'/W_\infty|<11/r^2 ,
$$
where $W_\infty=U_{\rm GL}/(C_\infty e^{-qr}r^{-\beta})$. To see the endpoint constants, insert these $q,\beta$ bounds into the three Coulomb remainder coefficients of GOAL-004 to get $C_R<10$. Its Volterra estimate has $\eta_\infty<9/R$. Bounding the derivative at each $R=r$ gives the stated $11/r^2$ log derivative. For the tensor error use $t_r=1/r,\ a=k^2,\ d=y'/y$ with $y=(1+ar^3)U_{\rm GL}/r$. Then $|d+q|<3t_r$, $|d|<0.79$, and the component ratios divided by $U_{\rm GL}/r$ are
$$
 \frac{t_r^3/2-a}{a+t_r^3}-\frac{\Omega^2}{af}
       +\left(\frac{t_r^2}{2a}-\frac{3ft_r}{a+t_r^3}\right)d,\quad
 -\frac{\Omega}{a}\left(d-\frac{t_r^2}{2f}\right),\quad
 -\frac12-\frac{ft_r d}{a}.
$$
These are $H_{tt}/f,H_{tr},K$; $fH_{rr}=H_{tt}/f-2K$. Their errors and the radial component error are bounded by $(7,4,3,13)t_r$, respectively, compared with the limiting ratios. Squaring with the Lorentzian signs bounds the contraction error by $53t_r(U_{\rm GL}/r)^2$. This gives $S$ between $0.5$ and $2$ times its leading expression. In the free integral the remaining factors in $w$ are $(1-1/R)^{-s}[(R+w-1)/(R-1)]^s(1+w/R)^{-m}/f(R+w)$. Comparing them with exponential bounds gives $0.48<n_0/n_*<2.1$. Because $-g'(R)\le1.021$ and $n_*$ decays at least as $e^{-2qR}$, the curvature error is $<0.44\,n_*/R^2$. Thus
$$
 0.47\,n_*<n_Z<2.11\,n_* \quad (R\ge100).
 \tag{4}
$$
This is a conservative analytic enclosure for the exact order-two waveform in terms of $q,C_\infty$. An absolute but much wider coefficient interval on $I_{\rm far}$ follows without a floating profile. The comparison in [DERIVATIONS.md](DERIVATIONS.md) gives $4<|U_{\rm GL}(2)|<250$ and $-0.9<p<0$ outside 2. For $r\ge10$, $V_{\rm GL}>0.35>0.59^2$ follows from the printed potential and $a>0.394$, and the constant-exponent Wronskian comparison gives $p<-0.59$. On $100\le R\le101$, $10^{-40}<|U_{\rm GL}(R)|<3\cdot10^{-21}$. Inserting $C_\infty^2 e^{-2qR}R^{-m-s}=U_{\rm GL}(R)^2R^{-1-s}/W_\infty(R)^2$ into (4), and using $1<|I_{\rm far}|<1.021$, gives
$$
 10^{-176}<2\int_{I_{\rm far}} n_Z^2\,du<10^{-86}.
$$
These endpoints are loose comparison bounds at an exponentially weak cut. They do not give useful accuracy or bounds for the later numerical window.

## A larger early cut benchmark

With the pinned empirical L10 GL profile and the same Z slice and clock, the dimensionless observation interval $[-10,-8]$ corresponds to initial ray radii from $6.56648$ down to $5.15255$. A separate characteristic-grid calculation including curvature, with an independent free radial quadrature, gives
$$
 \mathcal K_Z([-10,-8])\simeq 3.7325\times10^{-7}.
$$
The finest three point samples for $n_Z$ at $-10,-9,-8$ are $6.7414\times10^{-5},\,2.1119\times10^{-4},\,6.5154\times10^{-4}$. See the executable, waveform CSV, exact kernel truncation bound and refinement records in [compute/](compute/). The observed finest-grid difference in $\mathcal K_Z$ is $3.47\times10^{-11}$. The point estimates use a saved empirical profile and grid quadrature. A separate outward-rounded calculation encloses the exact mode on this exterior, encloses the signed source on each radial cell, and integrates a lower and upper news bound in the physical cut coordinate. It bounds the complete curvature correction by the sup norm of the absolute free waveform divided by $4R^2(1-\kappa_R)$, and bounds the analytic source tail beyond $60r_0$. This gives the independent enclosure
$$
 3.5316\times10^{-7}<\mathcal K_Z([-10,-8])<4.0171\times10^{-7},\qquad
 6.5671\times10^{-5}<n_Z(u)<6.7662\times10^{-4}
$$
throughout this window, for the exact quadratic problem. The main width comes from interval radial cell integration and the conservative curvature bound. The integrated result does not depend on a mesh convergence assumption, or on the saved empirical GL profile. It has no nonlinear news error term. See the [enclosure code](compute/early_interval.py) and [bounds](compute/early_interval_results.json).

## Data dependence, flux and remainder

For any sufficiently early compact $I$, an initially outward homogeneous scalar pulse can change or cancel the quadratic trace on $I$. Set $\psi|_\Sigma=A(u_\Sigma)$, $\partial_u\psi|_\Sigma=A'$, $\partial_v\psi|_\Sigma=0$, with $A$ initially zero. On a slightly padded interval $J$ its news map is $A'\mapsto (1+\mathcal P)A'$. The bound from (1)–(3) is $\|\mathcal P\|\le |J|/[4R(\max J)^2(1-\kappa_{R(\max J)})]$. If this is below one, invert the Volterra map on a smooth extension of $-n_Z$ that vanishes near the left edge of $J$. The solution is smooth by the causal integral equation, and tapering $A$ to zero after $I$ cannot change the earlier trace. This yields compact $F,P$ in the constraint prescription. Their quadratic news cancels exactly on $I$, and other multiples produce the expected interference term. The initially outward prescription is a Cauchy-data statement. A condition imposed on past null infinity is a distinct scattering problem. As a check, a static threshold first jet has zero news in the matched static solution but can radiate during its Z adjustment.

Restore $r_0$ by $\Phi_{\rm phys}=\epsilon^2 r_0\Phi_2(u/r_0)+\cdots$. In the Einstein-frame normalization $T_{uu}=(\partial_u\Phi_{\rm phys})^2/(4\pi G_4r^2)+o(r^{-2})$. The 5D transverse radion polarization has entries $(-2,-2,4)\phi/\sqrt3$, squared norm $8\phi^2$. Multiplying its Einstein–Hilbert null kinetic current by $4\pi r^2 L$ gives the same flux $(\partial_u\Phi_{\rm phys})^2/G_4$. Thus $M_0=L r_0/(2G_5)$ gives $\mathcal K_Z=2\int n_Z^2\,d(u/r_0)$. The balance is an asymptotic zero-mode loss. Massive $n=2$ transport has $\omega^2>(2k)^2$ for real outgoing propagating modes and needs a timelike surface current. The imaginary-frequency particular solution by itself has no such far propagating wave. The massive transient's total energy, horizon energy and full nonlinear balance are open here.

An even exact family with a common nonlinear Bondi clock and a fourth differentiable trace satisfying $\|N_\epsilon-\epsilon^2 n_Z\|_{L^2(I)}\le C_I\epsilon^4$ would give an $O(\epsilon^6)$ energy error and strict positive energy if $C_I\epsilon^2<\|n_Z\|_{L^2(I)}$. The exact inequalities are obtained by squaring $\epsilon^2\|n_Z\|\pm C_I\epsilon^4$, with the lower bracket truncated at zero. We have the constraint family and its compact-slab Taylor coefficients. We do not have a uniform estimate $C_I$ for the actual limit, or a numerical finite-amplitude validity threshold. At fixed extraction radius and finite time the smooth fixed-inner-leaf family has a compact-region remainder. Taking the small-amplitude limit there and then taking the perturbative radiation limit recovers (2). Commuting the limits is the unresolved PDE step. A varying mass can also shift the retarded coordinate by a secular logarithm, which is one reason to distinguish fixed-mass initial data and a nonlinear Bondi clock.
