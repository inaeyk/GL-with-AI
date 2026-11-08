# Barrier launch, outgoing shear and energy

Use $r_0=1$ for field and coordinates and restore $r_0$ in flux integrals. Let $A_\delta=\sqrt\delta$ and $a_\delta=A_\delta b((u-u_*)/\delta)$ with the smooth $b$ and data of [FINITE-DATA](FINITE-DATA.md). Uniformly on $[u_*,u_*+\delta]$,
$$
 \begin{aligned}
 C_{01}&=a_\delta+O(A_\delta\delta),&
 C_{21}&=K_*a_\delta''+O(A_\delta/\delta),\\
 N_{01}&=a_\delta'+O(A_\delta),&
 N_{21}&=K_*a_\delta'''+O(A_\delta/\delta^2).
 \end{aligned} \tag{1}
$$
Here $K_*=(\chi_\infty/r_0)^2/4$, with conservative strict enclosures
$$
 0.257<K_*<0.304\quad(R/r_0=13/8),\qquad
 0.0715<K_*<0.0911\quad(R/r_0=7/4). \tag{2}
$$
The $O$ constants are uniform in width for each fixed profile and launch radius. Their values have not been evaluated to certify a particular finite width. Equation (1) evaluates the leading shape of the ordered coefficient as width decreases. Later scattering is part of the retarded functional in [GREEN-FUNCTION](GREEN-FUNCTION.md).

## Physical optical coefficient

In product $u=t-x,v=t+x$ coordinates the first principal change of the normalized dual wave operator, after multiplication by $f$, has coefficient $-e^{\Omega t}H_{LL}\cos(kz)/f$ on $\partial_u^2$, where $L=\partial_t+\partial_x$. The first phase variation with zero launch value is $-\chi(u,v)\cos(kz)$, with
$$
 \chi(u,v)=\tfrac14\int_{v_\Sigma(u)}^v e^{\Omega(u+v')/2}(H_{LL}/f)(r(u,v'))\,dv'.
$$
Since $dv=2\,dx$ and $t_\Sigma(R)=-\log(R-1)$, the limit on the first cut is
$$
 \chi_\infty=\tfrac12 e^{\Omega(t_\Sigma(R)-x(R))}
 \int_R^\infty e^{\Omega x(r)}(H_{tt}/f+2H_{tr}+fH_{rr})\,dx . \tag{3}
$$
The signed integrand changes sign. The interval check reconstructs the entire contraction from the physical eigenfunction and includes the right tail. Independent ordinary quadrature in [D](investigators/D-WAVEFORM.md) checks the central values. The large-$R$ limit of (3) with the audited decaying branch recovers GOAL-007, with the explicit launch factor $e^{\Omega t_\Sigma(R)}$.

## Uniform short-packet estimate

Eliminate the divergence from (4) of [HIERARCHY](HIERARCHY.md) to give a scalar principal symbol on each dual component. Rescale $Q$ by $r$ and normalize by $2/\lambda$. Write $w_j$ for the resulting vector coefficient and $e_z$ for the outgoing polarization. The coefficient of $\partial_u$ in the background transport annihilates $e_z$: its $n=0$ equation is exactly Regge–Wheeler, and compact derivatives at fixed Fourier number contribute lower-order terms in $u,v$. The equations have the form $w_{uv}=A w_u+B w_v+D w+s$ with $A,B=O(1/v)$ and bounded $D$ for the mixed modes. In the returning scalar $A=B=0$ and $D=-V/4$ is integrable. The first-order operator coefficients and their derivatives at fixed $u$ decay exponentially with exponent arbitrarily below $(q_\infty-\Omega)/2$ in $v$. The mean order-two geometry coefficients, after radion cancellation and mass matching, decay at twice that rate up to polynomial factors.

Integrate $w_v$ and $w$ in $u$ from the left edge or the intersection with the slice, and $w_u$ in $v$ from the slice. The $u$-integral has norm $\le\delta$. Beyond the compact initial part use the zero left edge and write $I_u$ for this integral. For $Y=w_u$,
$$
 (1-B I_u)Y_v=(A+D I_u)Y+s .
$$
Here $B I_u$ means multiplication by $B(u,v)$ after integration. For fixed $v_0$ large enough, the mixed equations on $v\ge v_0$ are bounded in a weighted sup norm $e^{-\sigma(v-v_0)}$ with $0<\sigma<(q_\infty-\Omega)/2$. Take $v_0$ large to make $A$ small and $\delta$ small to absorb $D I_u$, using $\|(1-B I_u)^{-1}\|\le(1-\delta\|B\|)^{-1}$. The compact initial portion uses the same integrals with prescribed slice values and finite-interval Gronwall. Its graph $v_\Sigma(u)$ is smooth with bounded nonzero slope near either launch radius. For the mean scalar use unweighted Volterra estimates with the integrable potential and forcing. Multiplying the weighted mixed field by the first-order GL coefficients makes that forcing integrable.

For each fixed number of $u$ commutations the source and solution bounds count one $\delta^{-1}$ per derivative. Commutators with the smooth coefficients multiply lower derivatives. Differentiating the slice endpoint terms on the compact part costs at most one $\delta^{-1}$ per derivative, by the dual electric definition, the divergence equation and the noncharacteristic graph. The outgoing baseline has $w_{0,v}|_\Sigma=0$. At orders one and two the position differs from its leading zero correction by at most $O(A_\delta)$ and the first derivatives by at most $O(A_\delta/\delta)$. These bounds use the fixed canonical pair, including its forced normal pieces.

Subtract the leading fields
$$
 w_0^{\mathrm{lead}}=e_z a_\delta,\qquad
 w_1^{\mathrm{lead}}=-e_z\chi\cos(kz)a_\delta',\qquad
 \langle w_2^{\mathrm{lead}}\rangle=e_z\tfrac{\chi^2}{4}a_\delta'' .
$$
The baseline remainder has forcing $O(A_\delta)$, integrable in $v$. At order one the transport cancels the $a_\delta''$ forcing pointwise, leaving $O(A_\delta/\delta)$ in the weighted integrated norm, including $\partial_u^2(w_0-w_0^{\mathrm{lead}})$. At order two the second insertion cancels the $a_\delta'''$ term. The direct quadratic geometry, coefficient derivatives, compact terms and first-order remainder give at most $O(A_\delta/\delta^2)$ in the integrated unweighted norm. In this last estimate, $\partial_u^2$ of the mixed remainder costs $\delta^{-2}$, $\partial_v w_1$ is obtained by integrating its equation in $u$, and $\partial_u\partial_v w_1$ is given by the equation. Differentiating that $\partial_v$ formula in $v$ bounds $\partial_v^2 w_1$ by $O(A_\delta/\delta^2)$ in the mixed weight, including differentiated slice terms. At each order the final $u$ integration gains $\delta$ for the field and the $w_u$ estimate bounds its derivative. The limit is justified by absolute integrability of the mean potential and weighted forcing. Freezing $\chi_\infty(u)$ at $u_*$ costs one further factor of $\delta$ in the leading term, including after one derivative. The moving slice endpoint also gives a derivative of the initial trace when estimating the second v derivative. The explicit boundary term and the bound on the third baseline v derivative are in [E's review](investigators/E-INTEGRATED.md). The argument therefore retains the constrained intermediate, including all lower-order terms in the error estimates.

Use the fixed ADM mass and a common inertial Bondi frame with matched origin. The short-range order-two metric permits the common asymptotic retarded clock. A smooth order-two clock change differentiates the baseline once and contributes within the subleading orders. Compact extraction products decay in the same weighted estimate. With fixed mass, the leading time and returning shear shapes $b'$ and $b''$ are orthogonal on $(0,1)$ and the subleading terms preserve independence for sufficiently small width. In a joint two-preparation comparison one can also allow a mass template. At fixed launch radii the two leading time offsets are common, while the reference mass derivatives of the launch cuts differ. For geometric mass $M$, $\partial_M\partial_r x=2/(rf^2)>0$; the derivative of the cut $r-2x$ therefore distinguishes the radii. Two $b'$ projections separate the mass and time directions and a $b''$ projection detects the returning shear. This template test compares the two preparations as a pair of independent experiments.

## Observable and signed interference

In Einstein-frame Bondi gauge $h_{uA}=-\lambda C X_A/4+O(r^{-1})$ and $C_{AB}=C X_{AB}$. The raw curl $\psi=r^3 F_{tr}$ has limit $-\lambda C/2$, so $C=\lim\Psi$ with $\Psi=2Q_z/(\lambda r)$. The harmonic has $\int X_{AB}X^{AB}d\Omega=12$ and the tensor power is
$$
 \frac{dE_T}{du}=\frac{12}{32\pi G_4}\dot C^{\,2},\qquad G_4=G_5/L .
$$
Using the raw curl in this formula increases the power by four. The large-radius orthonormal transverse strain is $C X_{AB}/r$ and the tensor news is $\dot C X_{AB}$.

On the support-length window $[u_*,u_*+\delta]$ the $\epsilon^2\eta^2$ tensor energy coefficient is
$$
 E_{22,T}=\frac{12r_0}{16\pi G_4}
 \left[-\frac{K_*}{\delta^2}\int_0^1(b'')^2dy+O(\delta^{-1})\right].
$$
This follows by integrating $b'b'''$ by parts. The packet is symmetric about $y=1/2$, so $b'$ vanishes at both endpoints of each half interval. The Dirichlet Poincaré inequality and $\int(b')^2=1$ give $\int(b'')^2\ge4\pi^2$. The squared-width-scaled magnitude of the leading dimensionless interference is therefore greater than $0.257\cdot4\pi^2$ at $13/8$. On a sufficiently early fixed fractional subwindow the leading coefficient is positive, since $b',b'''>0$ close to $y=0$. Later tails and independent homogeneous data can affect longer intervals. The leading shear difference is $\epsilon^2\eta C_{21} X_{AB}$ in the ordered series. Half-period covariance allows the next GL order $\epsilon^4\eta$ at fixed axial linearity. Terms quadratic and higher in the nonspherical seed and a uniform nonlinear remainder are outside this calculation.

The sourced mean radion is a separate scalar observable. Massive $n\ne0$ perturbations can carry energy to timelike tubes at real frequencies above $|n|k$. The mixed-field estimate here controls its product with the decaying GL mode on the null strip.
