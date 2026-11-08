# GLR-GOAL-011 specialist D audit

The optical calculation was reconstructed before exposure to GOAL-011 code or outputs. The short-packet audit was given the proposed asymptotics in advance. It is an independent check of that statement using the pinned dual-variable equations. The work uses GOAL-007 at `0864d9d807de56cbdd6e64d215e6295508a3007f` and the GOAL-005 files in the same supplied checkout. GOAL-007's `INVESTIGATOR.md` identifies its GOAL-005 input as `61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708`. The exact reconstruction formulas are in `physics/gl_linear_mode.md`. The optical numerical runs do not use project Python modules.

## Optical integral

Use dimensionless coordinates with $r_0=1$, $L=10$, $k=\pi/5$, $f=1-1/r$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$. The real standing GL jet is $h=e^{\Omega t}H\cos(kz)$ with $H_{\theta\theta}(2)=8$. The slice $v-r=0$ gives $v_\Sigma(u)=R(u)$ with $u=g(R)=R-2x(R)$ and $t_R=-\log(R-1)$. Along the outgoing background ray, $t=u+x(r)$ and $dv=2dr/f$.

With $\bar g^{uv}=-2/f$ and $h_{vv}=h(L,L)/4$, $L=\partial_t+\partial_x$, the linear eikonal equation for $p=u+\epsilon p_1$ is $-4p_{1,v}/f-h(L,L)/f^2=0$. The physical launch label gives $p_1|_\Sigma=0$ in the completed unitary chart. Write $p_1=-\chi\cos(kz)$. Then
$$
\chi(u,\infty)=\frac12\int_{R(u)}^\infty e^{\Omega[u+x(r)]}
  \frac{H_{tt}+2fH_{tr}+f^2H_{rr}}{f^2}\,dr .
$$
The sign quoted below is for $\chi$ in this convention. The eikonal perturbation on the $\cos(kz)=1$ ray has the opposite sign.

The independent SciPy calculation integrates the logarithmic derivative $d=y'/y$ and $\log|y|$ backward from the leading Coulomb branch of the pinned scalar equation $y''+Py'+(Q-\Omega^2/f^2)y=0$. Normalize with the exact $H_{\theta\theta}$ expression at 2 and reconstruct $H_{tt}$, $H_{tr}$ and $H_{rr}$. A cross-check interpolates the individual saved empirical metric components by cubic splines and uses direct cell-aware quadrature. The source forms share the pinned linear theory and normalization. These are different numerical implementations.

| $R/r_0$ | $u_*/r_0$ | Direct ODE $\chi/r_0$ | Saved-component $\chi/r_0$ | Direct ODE $\chi^2/(4r_0^2)$ |
| --- | ---: | ---: | ---: | ---: |
| $13/8$ | $-0.6849927415$ | $-1.059358022945$ | $-1.059357901555$ | $0.2805598551946$ |
| $7/4$ | $-1.1746358551$ | $-0.5703958205705$ | $-0.5703957500360$ | $0.08133784803108$ |

The three direct calculations used outer radii $30,45,65$ and DOP853 relative tolerances $2\times10^{-8},2\times10^{-10},2\times10^{-12}$, with absolute tolerances smaller by 20 and maximum radial step $0.4$. Whole-ray quadrature used absolute and relative tolerances $2\times10^{-11}$. The last refinement moves $\chi$ by less than $5\times10^{-11}$. Repeating the direct calculation at the GOAL-005 growth interval endpoints $[0.06328117,0.06328121]$ shifts the estimates by up to $3.55\times10^{-7}$ and $2.55\times10^{-7}$, respectively. These endpoint values are sensitivity samples. The finite-radius integration and the interpolated cross-check are empirical, with no interval enclosure on the strong-field segment.

The optical integrand is signed. For $R=13/8$ the signed pieces on $[R,2],[2,3],[3,5],[5,10]$ are approximately $-0.92941,-0.26508,+0.09185,+0.04182$. For $R=7/4$ they are $-0.44441,-0.25699,+0.08905,+0.04055$. The sign of the asymptotic integrand alone gives the wrong sign for both complete integrals. The chosen slice matters. Replacing it with $t=0$ at fixed launch radius multiplies $\chi$ by $(R-1)^\Omega$, giving $-1.02831413324$ and $-0.560105790946$. The analytic derivative of the integral gives $d\chi/du\simeq-1.22612836$ and $-0.79561189$.

An analytic tail check uses the GOAL-005 bounds $|U(2)|<250$, $-.9<p<0$ for $r\ge2$ and $p<-.59$ for $r\ge10$, with $p=U_x/U$. For $r\ge65$, $|d|<.95$ follows from $d=(\log(D/r))'+p/f$ where $D=1+k^2r^3$. The component ratios in `GLR-GOAL-005/RADIATION.md` then give $|H_{tt}/f+2H_{tr}+fH_{rr}|<4|U|/r$. Since $\Omega<.07$, $d x/dr\ge1$ and $u+x(65)>0$, the absolute tail is bounded above by
$$
\frac{500e^{-.59[x(65)-x(10)]+.07[u+x(65)]}}{65 f(65)(.59-.07)}.
$$
The values are below $4.61\times10^{-12}$ and $4.46\times10^{-12}$.

For a completed pure gauge GL jet, $H=0$ and $\chi=0$. In an arbitrary chart a pullback changes $p_1$ by $X^u$ and changes the launch label by its boundary value. The eikonal source integral shifts by the corresponding endpoint difference. Keeping the launch label fixed in a chart where the pullback moved it would generate a spurious optical contribution. Compact half-period averaging kills the one-insertion term and yields $\langle\cos^2(kz)\rangle/2=1/4$ for two insertions.

## Smooth packet and differentiated trace

Take $a_\delta(u)=A b((u-u_*)/\delta)$ with a fixed nonzero smooth bump $b$ supported on $[0,1]$. The outgoing zeroth-order dual master jet is $\Psi=a_\delta$, $\Psi_v=0$ on the slice. A precise sufficient preparation is the GOAL-007 dual canonical pair with only spatial $z$ components, fixed as functions of the background slice coordinates in the completed chart. The density divergence vanishes and the normal dual component is set by Gauss. The induced first derivatives have at most one derivative of the prescribed profile. This control on the mixed and cubic initial data is needed. Independent data at those orders with larger derivative scales can change the leading result.

For $Q=R^4*_B dA$ and $W=2Q/(\lambda r)$, the background $z$ polarization has zero first-derivative transport in the product geometry. The radial weight contributes $-2f\partial_x Q_z/r$ in the $Q_z$ equation multiplied by $f$. Rescaling $Q_z=rW_z\lambda/2$ cancels it. Eliminating the weighted divergence gives a remaining coupling $-2r^{-1}r_b\partial_z Q^b$ before multiplication by $f$, which is algebraic at fixed Fourier number. The $t,x$ equations also have no first-derivative transport from an input solely in the $z$ polarization. The sole enhanced derivative is thus the scalar coefficient $-h(L,L)\partial_u^2/f$ in the first variation of the principal operator multiplied by $f$. Its transport uses the same $\chi(u,v)$ as the eikonal integral, with the lower limit on the actual slice. The approximate outgoing fields before the final mean projection are
$$
W_{j,\mathrm{lead}}=e_z\frac{[-\chi(u,v)\cos(kz)]^j}{j!}\,a_\delta^{(j)}(u),
\qquad j=0,1,2.
$$
Here $e_z$ denotes the $z$ covector polarization. The factorial comes from the repeated transport integration under the no-factorial perturbation convention. It is not a coefficient convention for the metric. Differentiation in $u$ also acts on $\chi$, whose derivatives are bounded independently of $\delta$ on this strip.

Subtract these leading fields successively. At each order $j$, the only possible $A\delta^{-j-1}$ source term cancels by transport. Residual source sizes are $O(A\delta^{-j})$, with each $u$ derivative costing at most a factor $\delta^{-1}$, and each $v$ derivative at most that factor on the finite initial collar. This includes first insertions on the previous remainder, mixed derivatives on the leading term and direct order-two terms. The quadratic $n=2$ geometry times the baseline $n=0$ is absent from the final mean. In the mean, the fixed-mass Einstein-frame geometry is short range and the pure radion cancels in the axial equation. The remaining quadratic GL products are short range. These statements assume the GOAL-007 common-clock and reflection conditions and the GOAL-005 data.

For the strip estimate, integrate $W_{uv}$ in $u$ to obtain $W_v$ from its bottom value and use $W=W|_{\rm bottom}+I_u W_u$, where the bottom is the initial slice or the left edge $u=u_*$. The $u$ integration norm is at most $\delta$. The projected residual $W_j$ vanishes on the left edge. At $j=0$ its Cauchy residual data are zero. At $j=1,2$ the leading ansatz vanishes on the slice and the residual initial derivatives are at most $O(A/\delta)$. In particular at $j=1$ the difference in initial derivatives of the ansatz and actual solution contributes to the permitted $O(A)$ shear error, even when the label agrees on the slice. Smoothness of $v_\Sigma(u)$ and its inverse gives the same scaled derivative estimates on the finite initial collar. Above the collar invert the characteristic Volterra term $1-A_2 I_u$. The $O(r^{-1})$ matrix terms give polynomial growth, and the bounded massive zeroth-order term gives $\exp(C\delta v)$. Pick $\delta$ small so a fixed exponential weight on the massive remainder remains weaker than the $\exp[-(q-\Omega)v/2]$ GL falloff up to radial powers. The weighted source for the projected mean and its needed derivatives are then integrable in $v$. In the mean the scalar RW potential is $O(r^{-2})$, so Gronwall and dominated convergence give both the field and differentiated trace. A bounded fixed number of differentiated estimates follows by differentiating the characteristic identities. Differentiating the lower endpoint on the finite collar uses the compatible slice jets, and beyond the collar the lower endpoint is fixed. This gives one power of $\delta$ gain from each residual source to the field. The $u$ derivative costs $\delta^{-1}$. The statements use fixed $b,R$ and perturbative coefficients, followed by $\delta\downarrow0$.

In Bondi gauge, $h_r=0$ and $h_u=-\lambda C/4+O(r^{-1})$ in the axial vector harmonic. It follows from the curl that $F_{ur}=-\lambda C/(2r^3)+O(r^{-4})$. For orientation $dt\wedge dx\wedge dz$, $Q_z=-r^4 F_{ur}$ in the background. The mean extraction therefore gives $W_z\to C$ with $N=\partial_u C$. The pure mean radion cancels in $Q_z$ and the decaying nonzero-mode products do not contribute at the trace. Matching the Bondi clock and the mass prevents a secular mass-induced coordinate term. A smooth second-order cut shift acts with at most one profile derivative.

The resulting shear and news on $I_\delta=[u_*,u_*+\delta]$ obey
$$
\begin{aligned}
 C_{01}&=a_\delta+O(A\delta), &N_{01}&=a'_\delta+O(A),\\
 C_{21}&=\frac{\chi(u_*,\infty)^2}{4}a''_\delta+O(A/\delta),
 &N_{21}&=\frac{\chi(u_*,\infty)^2}{4}a'''_\delta+O(A/\delta^2).
\end{aligned}
$$
Freezing $\chi(u,\infty)$ at $u_*$ costs one power of $\delta$ in the stated scaled bounds. These are short-packet asymptotics on the stated cut window. The two shear remainders and their differentiated versions need the uniform strip estimate. Pointwise front orders for a fixed profile alone do not imply them.

## Interference and controls

For a unit-normalized real scalar harmonic, $\int X_A X^A=\ell(\ell+1)$ and $D^B X_{AB}=-\lambda X_A/2$. Hence $\int X_{AB}X^{AB}=\ell(\ell+1)\lambda/2=12$ at $\ell=2$. The coefficient of $\epsilon^2\eta^2$ in the Bondi tensor energy on $I_\delta$ is
$$
\begin{aligned}
 [E_T]_{22}
  &=\frac{12}{16\pi G_4}\int_{I_\delta} N_{01}N_{21}\,du\\
  &=-\frac{12A^2\chi(u_*,\infty)^2}{64\pi G_4\delta^3}
    \int_0^1(b'')^2\,ds+O(A^2\delta^{-2}).
\end{aligned}
$$
The mixed mean news $N_{11}$ vanishes by half-period parity, and the pure GL data have no tensor news by spherical symmetry. The sign follows exactly by parts using smooth compact support. With the full varying coefficient $K(u)=\chi(u,\infty)^2/4$ one has the exact principal-term identity $\int a'_\delta (K a''_\delta)'=-\int K(a''_\delta)^2$. A nontrivial compact bump has $\int (b'')^2>0$. The sign is strict for sufficiently small $\delta$ at any launch with $\chi_\infty\ne0$. The empirical optical check supports this nonzero condition at both requested radii.

For the test bump $b(s)=\exp(4-1/[s(1-s)])$ inside $(0,1)$, zero outside, the quadratures give $\int(b'')^2=425.67824355271927$ and $\int b'b'''=-425.6782435527192$. A constant time shift has leading interference proportional to $\int b'b''$, zero exactly and numerically $3.01\times10^{-19}$. The initial subwindow $[0,.16]$ gives $\int b'b'''=11.327889903892157$. On such a subwindow the boundary term $[b'b'']$ survives. This reconciles the full-support negative sign with a positive rising-front witness. The full-support statement covers the support of the launch profile in retarded time and leaves the later scattered radiation outside the window. The sign refers to a Taylor interference coefficient. The exact tensor energy square is nonnegative.

Run `python3 GLR-GOAL-011/checks/barrier_delay_independent.py` with Python 3.14.8, SciPy 1.18.0 and NumPy 2.4.6. Representative output with the actual radial splits, sensitivity samples and controls is in [the saved output](../checks/barrier_delay_independent.txt).
