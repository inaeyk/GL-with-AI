# Independent transport and consistency review

Role is `verification`. I reconstructed the stress current from the covariant scalar action, the radial transfer from its jump and asymptotic conditions, and the flat-product matter constraints directly from Einstein's equations. I inspected the local GOAL-005 source at [61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708), especially [RADIATION.md](../GLR-GOAL-005/RADIATION.md), [VERIFICATION.md](../GLR-GOAL-005/VERIFICATION.md), [NOTATION.md](../NOTATION.md) and the [vacuum source derivation](../physics/quadratic_radion_source.md). The GOAL-005 source, normalization and benchmark are shared inputs, so the assessment is an independent derivation of the massive extension and a comparison with the given massless result. The parent task was visible and a winding candidate appeared in its historical agent brief. No GOAL-008 draft or candidate solution from another specialist was consulted before reconstruction. The separate pinned Work audit was not part of the local inputs I inspected. These are analytic checks and bounded wrong-case tests, with no production or nonlinear simulation.

## 1. Scalar action, source and physical current

The GOAL-005 completed, dimensionless geometric radion is $\phi=\sqrt3\langle q_{zz}^{U}\rangle/4$, with the perturbative $\epsilon^2$ suppressed below. Set $c=(4\pi G_4)^{-1}$ and $\varphi=\sqrt c\,\phi$. An action with a prescribed scalar driving function $\sigma$ is
$$
S_{\rm I}=\int\sqrt{-g}\left[-\frac12(\nabla\varphi)^2-\frac12 m^2\varphi^2+\sqrt c\,\sigma\varphi\right].
$$
Variation gives $(\Box-m^2)\phi=-\sigma$. A separate $z$-Gaussian source check uses $K^\mu{}_\nu=\tfrac12(\gamma^{-1}\partial_z\gamma)^\mu{}_\nu$. The compact average of $-\partial_z \operatorname{tr}K$ vanishes, leaving $\langle R_{zz}^{(2)}[h]\rangle=-\tfrac14\langle \operatorname{tr}(h_{,z}^2)\rangle=-k^2 e^{st}H_{\mu\nu}H^{\mu\nu}/8$. The linear zero-mode Ricci component is $-\tfrac12\Box q^U_{zz,0}$. Therefore $\sigma=\sqrt3 k^2 e^{st}H_{\mu\nu}H^{\mu\nu}/16$, $s=2\Omega$. This sign follows from the completed vacuum source, then the stabilization is introduced into the scalar operator alone. In a flat local frame with only a time-radial component $H_{tr}=b$, the contraction is $-2b^2$, rejecting a Euclidean source. Replacing the real cosine by one unsummed complex harmonic would incorrectly eliminate the $n=0$ drive. Keeping only a raw averaged $g_{zz}$ in a generic gauge would omit the prescribed completion.

Let $x=r+r_0\log(r/r_0-1)$, $f=1-r_0/r$, $a=f/r$, $\psi=r\phi$ and $F=rf\sigma$. Directly using the spherical d'Alembertian gives
$$
(\partial_t^2-\partial_x^2+V_m)\psi=F,\qquad V_m=f(m^2+r_0/r^3).
$$
The scalar stress excluding the prescribed drive is
$$
T_{\mu\nu}=c\left[\partial_\mu\phi\partial_\nu\phi
-\frac12g_{\mu\nu}\big((\nabla\phi)^2+m^2\phi^2\big)\right].
$$
For $\xi=\partial_t$, $J^\mu=-T^\mu{}_\nu\xi^\nu$, $\nabla_\mu J^\mu=c\sigma\partial_t\phi$. The outward flux per Killing time at a worldtube and the shell Killing energy on a constant-$t$ slice are
$$
 P_R=-\frac1{G_4}\psi_t(\psi_x-a\psi)\big|_R,\qquad
 E_{[x_a,x_b]}=\frac1{2G_4}\int_{x_a}^{x_b}
  [\psi_t^2+(\psi_x-a\psi)^2+fm^2\psi^2]\,dx .
$$
Consequently $\dot E+P_b-P_a=G_4^{-1}\int F\psi_t\,dx$. On the future horizon an ingoing profile has $P=-G_4^{-1}(\partial_v\psi)^2$. For a horizon-penetrating initial slice the covariant balance uses the energy crossing that slice and the horizon rather than evaluating finite Schwarzschild $t$ at the horizon. The reduced master energy has $\psi_x^2+V_m\psi^2$ in place of the last two terms and differs by a surface term, $E_{\rm physical}=E_{\rm master}-[a\psi^2]_{x_a}^{x_b}/(2G_4)$, because $a_x+a^2=fr_0/r^3$. Its flux is $-G_4^{-1}\psi_t\psi_x$. The omitted term affects finite windows. The work on the right is prescribed energy exchange in Level I. A physical GL energy budget also needs the source-sector reaction and any stabilizer energy.

## 2. Retarded transfer and the threshold

Use $\widetilde\psi(\omega)=\int dt\,e^{i\omega t}\psi(t)$ and the analytic branch $p=\sqrt{\omega^2-m^2}$ with $\operatorname{Im}p>0$ for $\operatorname{Im}\omega>0$ and $p\sim\omega$ there. At the horizon choose $h_H\sim e^{-i\omega x}$. At infinity $V_m=m^2-m^2r_0/r+O(r^{-3})$ and the outgoing branch for positive real $\omega>m$ satisfies
$$
h_I\sim \exp\!\left[i\left(px+\frac{m^2r_0}{2p}\log(r/r_0)\right)\right].
$$
The coefficient of $\log(r/r_0)$ follows by substituting $e^{ipx}r^{i\alpha}$ and canceling the $1/r$ term. Negative frequencies give complex conjugation, with $p<0$. On the open interval $|\omega|<m$, $p=i\sqrt{m^2-\omega^2}$ and the infinity solution is the real decaying branch up to normalization, with a Coulomb power multiplying its exponential. The inverse of $-\partial_x^2+V_m-\omega^2$ has kernel $-h_H(x_<)h_I(x_>)/W$, with $W=h_Hh_I'-h_H'h_I$. The minus sign gives the derivative jump $-1$. In flat space $h_H=e^{-ipx}$, $h_I=e^{ipx}$ and the kernel reduces to $i e^{ip|x-x'|}/(2p)$. Changing the sign of $p$ in the upper half plane gives an exponentially growing boundary and fails the retarded prescription. For finite-slice Z data the same kernel can be used on the source multiplied by the characteristic function of the future of the initial slice in a global extension by zero, or implemented directly via the characteristic Cauchy integral. With unrestricted exponential forcing the Fourier transform on the real line is unavailable. Use a Laplace contour above the growth pole or specify a future completion.

For the constant-potential flat check on the whole line, causal support and the Lorentz-invariant interior variable $\ell=\sqrt{\Delta t^2-\Delta x^2}$ reduce the homogeneous equation inside the cone to $b''+\ell^{-1}b'+m^2b=0$. The regular solution with $b(0)=1$ is $J_0(m\ell)$, so
$$
G_{\rm ret}^{1+1}(\Delta t,\Delta x)=\frac12\Theta(\Delta t-|\Delta x|)\,J_0(m\ell).
$$
The factor $1/2$ gives the same light-cone distributional jump as the massless fundamental solution. This shows that the massive causal front and the arrival of the finite-frequency envelope are distinct. The constant-potential solution is a diagnostic, while the Schwarzschild horizon has $V_m\to0$ and uses $e^{-i\omega x}$.

For an integrable real pulse in a homogeneous extraction exterior with no incoming infinity wave, Parseval and the exact radial Wronskian give
$$
 \int_{\mathbb R} P_R\,dt=\frac1{\pi G_4}
    \int_m^\infty \omega p\,|\mathcal A(\omega)|^2\,d\omega,\qquad
 \widetilde\psi(\omega,x)=\mathcal A(\omega)h_I(\omega,x).
$$
The physical boundary correction integrates to zero with vanishing endpoint data. For a noncompact source this formula at infinity requires a justified limit of the exterior forcing contribution. A single real subthreshold decaying mode has zero spectral current. Superpositions and locally forced fields can have nonzero instantaneous signed tube flux, and flux through a finite tube may later return. Positive net escape is an asymptotic observable with an appropriate time integration.

Strict $\omega^2>m^2$ gives ordinary scattering states with nonzero limiting group velocity $p/\omega$. Exactly at $\omega=m>0$ the formal equation has local squared radial wavenumber $m^2r_0/r$, so it can have WKB solutions proportional to $r^{1/4}e^{\pm2im\sqrt{r_0r}}$ and zero limiting speed. The boundary $\omega=m$ is a branch endpoint of measure zero for finite-energy smooth pulses. Flat constant-coefficient evanescence should not be extrapolated to the exact Schwarzschild threshold. An imaginary $\omega=is$ gives $p=i\sqrt{s^2+m^2}$ and a spatially screened homogeneous response, for every $s>0$. The forced profile can inherit a slower radial decay from the extended GL source. Neither profile carries real on-shell outgoing flux merely because $s>m$.

The potential shape supplies an independent quasi-bound diagnostic. With $y=r/r_0$ and $\mu=mr_0$,
$$
 r_0^2V_m=(1-y^{-1})(\mu^2+y^{-3}),\qquad
 \frac{d(r_0^2V_m)}{dy}=\frac{\mu^2 y^3-3y+4}{y^5}.
$$
For $0<\mu<1/2$ there are an exterior maximum and outer minimum, coalescing at $y=2$ for $\mu=1/2$. For small $\mu$ they are near $4/3$ and $\sqrt3/\mu$. This is a trapping diagnostic, not a pole or lifetime calculation. A real-frequency decaying mode cannot simultaneously be regular ingoing with nonzero horizon amplitude, since its Wronskian current vanishes at infinity but is $-\omega|A_H|^2$ at the horizon. Positive energy similarly excludes regular static decaying and exponentially growing finite-energy modes. Resonances can leak into the horizon. Branch-cut tails are distinct from damped resonances. The Schwarzschild intermediate and asymptotic oscillatory rates $t^{-3/2}$ for the monopole and $t^{-5/6}$, respectively, apply to the corresponding late-time regimes of transients. The continuously growing forced solution does not satisfy such a late-time decay law. [Koyama–Tomimatsu v2, Sections III.2–III.3 and Eqs. (33), (36)](https://arxiv.org/html/gr-qc/0103086v2)

## 3. Finite-slice massless recovery and a joint early-cut estimate

For this subsection use $r_0=1$, $u=t-x$, $v=t+x$, $\tau=v-r$ and $g(R)=-R-2\log(R-1)$, exactly as in GOAL-005. Write $R(u)=g^{-1}(u)$, $\mathcal T$ for its free retarded double integral with prefactor $1/4$, and $V_0=f/r^3$. The massive Z field is $\psi_m=\mathcal T[F-(V_0+\mu^2 f)\psi_m]$ with zero scalar Cauchy pair on $\tau=0$. At $\mu=0$ the exact GOAL-005 equation follows with its clock, source, curvature term and Cauchy data. Uniqueness excludes an extra free pulse in this comparison. This analytic recovery inherits the interval $3.5316\times10^{-7}<2\int_{-10}^{-8}(\partial_u\psi_0(u,\infty))^2du<4.0171\times10^{-7}$ for the normalized Z waveform. I did not rerun the baseline interval code.

For a useful mass-continuity estimate, truncate at $v_*\ge A$, use $u\le u_1=g(A)$, and set
$$
 \|\psi\|_\gamma=\sup_{u\le u_1,\ R(u)\le v\le v_*}e^{\gamma(u_1-u)}|\psi(u,v)|,\quad
 \kappa_A=\tfrac14[2\log(A/(A-1))-1/A],\quad
 \eta=\frac{\mu^2(v_*-A)}{4\gamma}.
$$
For $0<\gamma<2q$ with $q=\sqrt{k^2+\Omega^2}$, the free-source norm is finite uniformly in $v_*$ by the GOAL-005 early-cut asymptotics. In detail, its far early bound is a polynomial in $R$ times $e^{-2qR}$ and $R\sim-u+O(\log|u|)$ as $u\to-\infty$. On the finite remaining interval $u\le u_1$, the integrals converge as $v_*\to\infty$ since $2q>s$. Integration of $V_0$ in the interior null variable gives $1/(4R(u)^2)$ including its prefactor, as in the GOAL-005 curvature bound. The decreasing weight on each Volterra integral cannot increase that operator norm $\kappa_A$. For the mass term bound $f\le1$, the $v$ range by $v_*-A$, and integrate $e^{-\gamma(u-\hat u)}$ over $\hat u\le u$ to get $\eta$. If $\kappa_A+\eta<1$, subtraction and inversion imply
$$
 \|\psi_m-\psi_0\|_\gamma
   \le \frac{\eta}{1-\kappa_A-\eta}\|\psi_0\|_\gamma .
$$
Writing $\delta=\psi_m-\psi_0$, differentiation on an outgoing ray also gives
$$
 \|\partial_u\delta\|_\gamma
 \le\left[\frac1{4A^2}+\frac{\mu^2(v_*-A)}4\right]\|\delta\|_\gamma
       +\frac{\mu^2(v_*-A)}4\|\psi_0\|_\gamma.
$$
For $\partial_v\delta$ integrate the outer $u$ variable along the incoming ray and use $\sup_{r\ge A} V_0\le A^{-3}$, obtaining
$$
 \|\partial_v\delta\|_\gamma
 \le\frac{A^{-3}+\mu^2}{4\gamma}\|\delta\|_\gamma+\frac{\mu^2}{4\gamma}\|\psi_0\|_\gamma.
$$
The endpoint terms vanish where the characteristic domain has zero extent. At the worldtube $R_{\rm obs}$ on a fixed early window ending at $u_1$, take $v_*=u_1+2x(R_{\rm obs})$. These estimates justify massless recovery on each finite tube and, in the joint regime $\mu^2 R_{\rm obs}/r_0\to0$ with $R_{\rm obs}/r_0\to\infty$, recovery of the massless early-cut waveform derivatives and quadratic energy. The tube factors are $\partial_t=\partial_u+\partial_v$ and $\partial_x=-\partial_u+\partial_v$, and the $a\psi$ term vanishes at infinity. For the massless field $\partial_v\psi_0\to0$ along the fixed window by differentiating the characteristic formula and using the far source, its weight, and the integrable curvature. Restoring units, the news in the physical field is $\epsilon^2\partial_u\psi$ with dimensionless $u$, and $M_0=r_0/(2G_4)$, giving the factor of two in the dimensionless energy.

A bounded scalar arithmetic check in Python 3.14.8 solved $g(A)=-8$ by bisection and evaluated the norm multiplier $\eta/(1-\kappa_A-\eta)$ with $\gamma=1$. It returned $A=5.152553235120$ and $\kappa_A=0.059364857039$. At $R_{\rm obs}/r_0=20$ and $\mu=0.05,0.1,0.2$ the multipliers were $0.022235123$, $0.095297336$, $0.533797895$. At $R_{\rm obs}/r_0=50$ and $\mu=0.05,0.1$ they were $0.067095919$, $0.336020487$. For $R_{\rm obs}/r_0=50,\ \mu=0.2$, $\eta=0.946310874$ and the sufficient contraction test fails. This records rounding-level evaluations of an explicit analytic bound, with no numerical waveform simulation. Failure of this bound gives no instability or nonexistence conclusion.

At fixed nonzero $\mu$, $\int_{R(u)}^\infty \mu^2 f\,dv$ diverges. The curvature Neumann bound therefore cannot be applied on the untruncated wedge for the massive operator. A finite-energy smooth outgoing packet has no ordinary massive radiation field at fixed null cut. One way to see the failure of uniformity is to take a smooth compact spectral band strictly above threshold. Its phase along fixed $u$ contains $(p-\omega)x$ and the $O(\log r)$ Coulomb phase. On the band $\partial_\omega(p-\omega)=\omega/p-1>0$, and integration by parts makes the field and its derivative tend to zero at fixed $u$. At $m=0$ that leading phase vanishes and the packet has a nonzero null waveform. Extending the argument to arbitrary data requires integrable, controlled frequency endpoint and high-frequency tails. A monochromatic solution or singular front is outside this pulse argument. The phase accumulation $-(m^2/2\omega)x$ and delay $x(\omega/p-1)$ make the order of limits physical. For massive waves the correct observable captures the pulse at times $t\simeq R/v_g$ and eventually integrates the net large-tube flux, which is different from taking a fixed retarded-cut flux. [Sussman v2, Section 1.1 and Theorem 1](https://arxiv.org/html/2305.01119v2)

For the frequency of the forcing itself, the finite-slice preparation starts at $t_\Sigma(r)=r-x$, not at a common $t$. For a diagnostic that stops the drive at $T$ and uses $\epsilon$ normalized at that time, the radial source transform for $t_\Sigma<T$ is
$$
 \widetilde F_T(\omega,r)=S(r)\frac{e^{i\omega T}}{s+i\omega}
    [1-e^{-(s+i\omega)(T-t_\Sigma(r))}],
$$
where $S(r)=rf\sigma e^{-st}$ as in the pinned waveform. The step is a model and contributes an artificial ultraviolet tail. At each radius a long interval gives a Lorentzian factor $1/(s^2+\omega^2)$ in source power. The retarded spatial overlap and outgoing weight $\omega p$ still enter the energy, and smoothing the completion changes the high-frequency population. Source power is also not the spectrum of the observed waveform on a finite null cut. The denominator contains no real emission threshold at $s$.

## 4. Stabilization obstruction and mode selection

An unwarped product of any Ricci-flat four-metric with a constant-radius circle is Ricci flat. If five-dimensional Einstein gravity has a cosmological term plus matter, its field equations require total effective stress $T^{\rm tot}_{AB}=T_{AB}-(\Lambda_5/8\pi G_5)g_{AB}=0$ pointwise. In particular, for a compact-directed null vector $N^A=(u^\mu,n^z)$ with $u$ unit timelike and $n$ unit compact-spacelike, $T_{AB}N^AN^B=0$ independently of $\Lambda_5$. Canonical winding matter $-\tfrac12(\partial\theta)^2-U$ gives the strictly positive $(N\cdot\partial\theta)^2$ for nonzero winding, so no tuning of $\Lambda_5$ or $U$ makes this product solve the equations. The same obstruction applies to canonical positive kinetic sectors with strictly positive compact-null contraction and no offsetting stress. The equality test alone is insufficient for general matter, warped circles, derivative and curvature interactions or nonlocal/quantum stresses.

For a shift-symmetric action $P(X)$ with $X=g^{AB}\partial_A\theta\partial_B\theta$ and winding $\theta=wz$, its stress is $P g_{AB}-2P_X\partial_A\theta\partial_B\theta$. Absorbing any cosmological constant into $P$, the exact product requires $P(X_0)=P_X(X_0)=0$, $X_0=w^2$. At constant radion the reduced potential is
$$
 V_E(\phi)=-L e^{-2\phi/\sqrt3}P(X_0e^{-4\phi/\sqrt3}),\qquad
 m_\varphi^2=-\frac{64\pi G_5}{3}X_0^2P_{XX}(X_0).
$$
Negative $P_{XX}$ appears to stabilize the homogeneous radius while giving zero background stress. The quadratic five-dimensional matter term is only $\tfrac12 P_{XX}(2w\partial_z\delta\theta-w^2h_{zz})^2$, with no four-dimensional time kinetic term for the winding fluctuation. The linear winding equation sets $\partial_z\delta X=0$, hence $\delta X_n=0$ for each $n\ne0$. The unbroken quadratic vector can lack the nonlinear longitudinal kinetic description. An apparent untouched GL branch at this degenerate point is insufficient for a controlled weakly coupled stabilization. The principal scalar symbol is $P_X g^{AB}+2P_{XX}\partial^A\theta\partial^B\theta$. In a local frame with only a compact background gradient, it has one compact derivative and no time derivative at the tuned point. With $P_{XX}(X_0)<0$, just below $X_0$ the time kinetic coefficient $-P_X$ is negative and the compact and noncompact principal spatial coefficients have opposite signs. Thus the minimal $P(X)$ prescription lacks an open, healthy perturbation neighborhood of that point. This diagnoses the two-derivative action as written. Extra operators or degrees of freedom would require a fresh constraint and principal-symbol analysis. Positive reduced curvature of the formal potential is insufficient to pass the higher-dimensional health gate. This check does not assume that a general saturation configuration must be degenerate.

With unbroken circle isometry and the pinned Einstein-frame convention, $F_c=F/\sqrt{4\pi G_4}$ and $M_4=(8\pi G_4)^{-1/2}$ give $-\tfrac14e^{\sqrt6\,\varphi/M_4}F_c^2$. Thus the minimal action contains the cubic $\varphi F_c^2$ and the kinetic and mass stress vertices $h\varphi^2$ for the zero-mode tensor. A tuned potential with $V(0)=V'(0)=0$ supplies no minimal vertex with one radion and two gravitons about the flat vacuum. Other operators can change that statement in a specified theory. At constant radion the vacuum gravitational KK mass squared is $k_n^2e^{-2\sqrt3\phi}$, so its variation allows a radion with a pair of opposite KK charges. Circle-translation selection is conditional on the symmetry of the stabilizing background, including a possible diagonal symmetry in a winding model. A pure zero-mode classical solution cannot create a nonzero charged mode from exact zero initial charged data in a translation-invariant evolution. Existing GL modes can participate in nonlinear mixing. A quantum one-particle decay into a KK pair needs $m\ge 2M_n$ for the physical asymptotic KK mass $M_n$ and a nonzero matrix element. The vacuum KK mass is $|k_n|$. The quantum process is separate from classical seeded conversion or parametric amplification.

Exact transverse $SO(3)$ forces any invariant symmetric tensor on a sphere tangent plane to be proportional to its metric, since the stabilizer $SO(2)$ rotates that plane. Its tracefree part vanishes, so the massless tensor Bondi news vanishes in every invariant frame, independent of nonlinear order. This leaves the angular monopole scalar and potentially the total-angular-momentum-zero sector of a massive KK spin-2 mode. The massless vector has only radial electric and possibly monopole magnetic fields in the invariant sector, for which $T^r{}_t=0$. In addition, a reflection-symmetric GL standing cosine cannot source the reflection-odd metric vector zero mode when the full action, background and data preserve that reflection. The stabilizing matter can affect this argument if it breaks the reflection, changes the vector spectrum, or changes the compact symmetry itself.

## Review gate decisions

- **Derived and checked:** scalar normalization, signed completed source, radial operator, upper-half-plane prescription, current and exact prescribed-work identity. The threshold tests, boundary term and Wronskian constrain their domains.
- **Analytic comparison:** the massive equation tends to the pinned Z solution at finite extraction radius and in the stated joint early-cut regime. The pinned numerical enclosure remains baseline evidence. The same massless curvature argument on the full untruncated wedge fails for fixed $\mu>0$.
- **Model-dependent:** a whole-pulse escape fraction needs both the full retarded transfer and a specified future source history. The source-selected imaginary-frequency solution, an early-cut spectrum and the canonical energy of a prepared pulse are distinct.
- **Proved in the specified class:** ordinary positive-kinetic winding plus a cosmological constant cannot realize a Ricci-flat unwarped product. The degenerate $P(X)$ action gives an exact background and formal radial mass while leaving the weakly coupled matter perturbation gate open. The argument allows stabilizers outside that class.
- **Open:** a stable matched theory's quantitative GL threshold or growth shift, nonlinear first bulge, and complete physical radiation energy budget. The fixed-source scalar equation and exact $SO(3)$ obstruction do not determine these quantities.
