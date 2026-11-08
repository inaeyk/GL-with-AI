# Directed transport of the prepared quadratic radion

## Problem and flux

Set $r_0=1$, $f=1-1/r$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$,
$g(r)=-r-2\log(r-1)$ and $b=g^{-1}$. The fixed Z Cauchy graph is
$v=b(u)$. Both scalar Cauchy data vanish there. Write $\mu=m_\phi r_0$.
The transport comparison keeps the vacuum quadratic source $F=e^{s t}S(r)$
and data fixed while changing the scalar propagator,
\[
4\psi_{uv}+V_\mu\psi=F,\qquad
V_\mu=f/r^3+\mu^2 f,\qquad
S=\sqrt3\, k^2rf H_{\alpha\beta}H^{\alpha\beta}/16,\qquad s=2\Omega .
\]
The mode is the L10 unhalved real cosine with $H_{\theta\theta}(2)/8=1$,
in the GOAL-005 clock. The inherited bound used computationally is
$0.06328117<\Omega<0.06328121$, which also contains the tighter project
bracket. The prescribed-source model treats a nonzero $\mu$ as a
separate scalar Klein–Gordon EFT experiment on Schwarzschild.
[GOAL-005 radiation and conventions](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/RADIATION.md) · [GOAL-005 independent audit](https://github.com/inaeyk/GL-with-AI/blob/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639/research/sav/verification/AUDIT_GOAL005_FINITE_SLICE_61c7b5c2.md)

The spherical physical energy current stripped of $1/G_4$ is
\[
j=-\psi_t(\psi_x-a\psi)=(\psi_u+\psi_v)(\psi_u-\psi_v+a\psi),
\quad
e=\frac12[\psi_t^2+(\psi_x-a\psi)^2+f\mu^2\psi^2],\quad a=f/r .
\]
The tube coefficient is $K_R(\mu)=2\int_{-10}^{-8} j(u,u+2x(R))du$,
with $E/M_0=\epsilon^4 K_R$ at the stated order and $M_0=r_0/(2G_4)$.
Here $dt=du$ on a fixed tube. With the master current
$j_m=-\psi_t\psi_x$ and density $e_m=(\psi_t^2+\psi_x^2+V_\mu\psi^2)/2$,
$a_x+a^2=V_0$ gives $j=j_m+\partial_t(a\psi^2)/2$ and
$e=e_m-\partial_x(a\psi^2)/2$. Thus $\partial_t e+\partial_x j=F\psi_t$.
The finite-cut endpoint contribution in $j$ is included. On the strip
between $u_0,u_1$, the graph, and $x=x(R)$, the exact balance is
\[
\int_{u_0}^{u_1} j\,du+
\frac12\left[\int_{b(u)}^{u+2x(R)} (e-j)(u,v)\,dv\right]_{u_0}^{u_1}
=\frac12\iint_{\rm strip} F\psi_t\,du\,dv .
\]
The second term is a change in outgoing-null-surface energy. The graph
contribution is zero and the strip lies at $r>5$, outside the horizon.
This identity checks the sign, storage and source-work factors. The
certificate below bounds tube flux directly. GOAL-008's sampled
source-work balance is a separate diagnostic.
[GOAL-008 fixed-background transport](https://github.com/inaeyk/GL-with-AI/blob/4aac5ad001a9d6c63efa86640ce05837282fb73f/research/sav/GLR-GOAL-008/TRANSPORT.md) · [GOAL-008 independent audit](https://github.com/inaeyk/GL-with-AI/blob/a54aacd959711945edb12668e1d26edb1b9bde33/research/sav/verification/AUDIT_GOAL008_MASSIVE_RADION_4aac5ad0.md)

## Exact reference and full potential correction

Take $4\partial_u\partial_v+\mu^2$ as a reference, and put
$\Delta=V_\mu-\mu^2=f/r^3-\mu^2/r$. For the zero-data graph the exact
reference inverse $G_\mu$ integrates the causal wedge with factor $1/4$
and kernel
\[
J_0(\mu\sqrt{(u-U)(v-V)})
=F_0(k_\mu (u-U)(v-V)),\qquad
k_\mu=\mu^2/4,\qquad F_\nu(z)={}_0F_1(\nu+1;-z).
\]
Indeed $4\partial_u\partial_v J_0=-\mu^2 J_0$ off the characteristics,
and the unit values on the characteristic edges give the source when
differentiating the retarded integral. The kernel solves the equation
with zero data because the source is restricted to the future of the
spacelike graph. Its absolute value is at most one. The two kernel
derivatives are $-k_\mu(v-V) F_1$ and $-k_\mu(u-U) F_1$,
respectively, and $|F_1|\le1$. The exact Schwarzschild field satisfies
$\psi=G_\mu F-G_\mu(\Delta\psi)$.

Define $M(r)=S(r)e^{sx}/(2f)$ and the ray source envelope
\[
 A(U)=e^{sU}\int_{b(U)}^\infty |M|\,dr,\qquad
 D=\frac12\int_5^{r_{\max}} \left| r^{-3}-\frac{\mu^2}{r-1}\right|\,dr .
\]
In each run $r_{\max}>v_{\max}=-8+2x(R)$, hence it bounds all radii on
the causal wedge with $u\le-8$ and $v\le v_{\max}$. The lower bound $r>5$
follows from $g(5)>-8$. The one-ray norm of the residual potential is
at most $D$ since $dv=2\,dr/f$. The integral is directed using the
primitive $-1/(2r^2)-\mu^2\log(r-1)$ with its signs split. At
$\mu=0.10$ the crossing is within $[9,10]$, where the code instead uses
a sum of absolute bounds. For $\mu=0.20$ the integrand before taking
absolute value is negative throughout. Let $D_R$ use $r_{\max}=R$.

From the Volterra inequality
$\sup_v|\psi(u,v)|\le\int_{-\infty}^u[A(U)+D\sup_v|\psi(U,v)|]dU$,
\[
 |\psi(u,v)|\le J(u)=\int_{-\infty}^u e^{D(u-U)} A(U)dU .
\]
One may apply the inequality on the finite wedge, starting before its
earliest graph point where the field vanishes, and then enlarge the
positive envelope to $-\infty$. Write $J_1,J_2$ for the first and second
primitives of $J$ vanishing at $-\infty$. The error between $\psi$ and
$\psi^\star=G_\mu F$ on each tube cut is enclosed by the following three
radii, with derivatives at fixed complementary null coordinate.
\[
\begin{aligned}
 E_\psi &\le D J_1,\\
 E_u &\le D_R J+k_\mu(v_{\max}-5)D J_1,\\
 E_v &\le (\mu^2/R+R^{-3}) J_1/4+k_\mu D J_2.
\end{aligned}
\]
For the $u$ derivative the characteristic boundary term is the ray
integral through $r=R$, giving $D_RJ$. For the $v$ derivative the boundary
term stays at $r\ge R$ along the past fixed-$v$ segment. The remaining
kernel derivatives contribute the second summands. The field and
derivative residuals are therefore bounded before evaluating the signed
current. This method uses an exact reference inverse and requires no
differentiated residual of an empirical PDE solution or a fitted
interface trace.

For a cell in $u$ define $d=(u-g(r))_+$. The crude resolvent bound for
$j=0,1,2$ is
\[
 J_j\le e^{su}\int |M|e^{Dd}\frac{d^{j+1}}{(j+1)!}\,dr,\qquad J_0=J .
\]
The sharper variant uses $e^{su}\int |M|\int_0^d
e^{-(s-D)a} a^j/j!\,da\,dr$. In the convolution for the $j$th primitive
the extra factor is $e^{-Db}\le1$ for the primitive time offset $b$.
It is absent at $j=0$, where the inner integral is exact. The sharper
bound uses $s-D>0$ and is evaluated at a directed upper bound for $d$.
The omitted source tail can use the simple upper integral
$d^{j+1}/(j+1)!$ in this variant.

## Whole-cell source and waveform quadrature

The calculation calls the pinned GOAL-005 `source_integrals()`
to bound the actual radial source. The inherited floating Riccati
solution proposes only the continuous Hermite candidate. Its
differential defect is bounded on entire cells, the initial
outer-logarithmic-derivative uncertainty is propagated inward, and
$U_{\rm GL}(2)$ is normalized from the angular component. Integrating
the enclosed Riccati field also encloses the modulus of the master on
every contributing cell. The signed $S(r)$ is strictly positive on all
enclosed cells contributing to the early rays. No empirical GL table
or independently rounded waveform candidate enters this calculation.
[Inherited enclosure implementation](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/compute/early_interval.py)

For each tube point and radial source value, set
$c=v-u-2x(r)=2(x(R)-x(r))$ and $a_*=u-U$.
At $r<R$, the radial contribution to $\psi^\star$ is
\[
 e^{su}M(r)\int_0^{(u-g(r))_+} e^{-s a_*}
     F_0(k_\mu a_*(c+a_*))\,da_* .
\]
For $\psi^\star_u$ the direct contribution is
$e^{su}\int_{b(u)}^R M(r)dr$. Its remaining integrand is the same
with the kernel replaced by $-k_\mu(c+a_*)F_1$. For
$\psi^\star_v$ the kernel term uses $-k_\mu a_*F_1$ and the direct
boundary contribution is $e^{su}\int_R^v e^{-2s(x(r)-x(R))}M(r)\,dr$.
If $r>R$, the inner integral starts at $a_*=-c$ when that is less
than $u-g(r)$. The certificate treats the whole omitted radial
contribution, including this region, by an absolute source-tail bound.
All partial cells crossing the Z graph are enclosed. For the kernel
integral use the positive part of $u-g(r)$. For the direct integral
sum the complete radial masses and enclose each partial positive
mass between zero and its full-cell upper bound.

Grouping adjacent inherited radial masses for the kernel quadrature
only enlarges the interval on their common $r$ box. The bound is
valid for the whole box in $u$ as well. The hypergeometric kernels
are evaluated at point arguments using Arb and inflated across the
whole argument cell using
$|F_\nu^{(j)}(z)|\le \nu!/(\nu+j)!$ for $z\ge0$. The bound follows
from the Bessel integral representation, equivalently the normalized
Fourier transform of the positive ball projection density.
Composite Simpson integrates the inner kernel moments with fourth
derivative bounds from the chain rule, the exponential, and the
above Bessel derivative bounds. The remainder is bounded by
$(d/180)(d/n)^4\sup|\partial_{a_*}^4(\mathrm{integrand})|$
for an even number $n$ of equal panels, for each exact $u,r,s$ in
the box. The kernel moments used in the current are weighted by
$1$ or $a_*$ as required. Arb evaluates $2j$ on each whole $u$
cell after all remainder inflations, and integrates using an
interval Riemann sum. Interface uncertainty is covered by the
closed cells. The reference is evaluated by quadrature rather
than differentiated across a mesh seam.

The radial candidate cutoff is about $18$. From there to $60$,
the source tail uses the inherited enclosed radial masses.
For $r\ge60$ one has $0.6^2<V_{\rm GL}+\Omega^2<0.65^2$ by
inserting $k^2\in(0.39,0.40)$ and $\Omega<0.064$ in the pinned
rational potential. Constant-exponent Wronskian comparison
bounds the decaying mode logarithmic derivative between $-0.65$
and $-0.6$, and hence $|U(r)|\le m_{60}e^{-0.6(x-x(60))}$.
For example, using $f>0.98$ and $d_*=1+k^2r^3$, the
reconstruction factors $y=d_*/r$ and $|y'|$ before multiplying
by $U$ are bounded by $0.41r^2$ and $0.29r^2$. They give
$|H_{tt}/(f U)|<1.2/r$, $|H_{tr}/U|<0.123/r$ and
$|H_{\theta\theta}/(r^2 U)|<0.54/r$ at $r\ge60$. The
inherited bound $|S|\le100r^5|U|^2$ is more than sufficient.
Using $d\le3r$ and $r/60\le e^{(x-x(60))/60}$
then gives the analytic moments
$\int_{60}^\infty |M|\,d^j e^{Dd}\,dr$ by integrating a
decaying exponential. For the reference field the omitted
kernel term uses $\int |M|d$, the $\partial_u$ term uses
$\int |M|(1+k_\mu(v_{\max}-5)d)$, and the $\partial_v$ term
uses $\int |M|(1+k_\mu d^2/2)$, all with the prefactor $e^{su}$.
No boundary continuation beyond the ray domain is needed in
the solution. Extending an absolute error integral to infinity
only enlarges it.

An exact manufactured kernel check integrates a constant source
on the flat zero-data triangle, recovering
$(1-\cos(\mu t))/\mu^2$. Independent rational power-series
moments with a strict alternating remainder check all three
massive Simpson integrals at one manufactured cell, and the
wrong mass sign is excluded there. These are kernel and
normalization checks. The rigorous tube estimate uses the
continuum bounds above.
[Manufactured control](checks/check_kernels.py) · [Output](checks/check_kernels.txt)

## Directed tube decisions

Intervals below enclose the signed flux coefficient $K_R(\mu)$ on
$u\in[-10,-8]$. Each tabulated number includes the source uncertainty,
direct terms, kernel quadrature, the full Schwarzschild correction
and final integration error. The exact decimal endpoints and maximum
correction to $(\psi,\psi_u,\psi_v)$ are retained in the JSON.

| $R/r_0$ | $\mu=0$ | $\mu=0.10$ | $\mu=0.20$ |
|---|---:|---:|---:|
| 48 | $[3.5327523,4.1610766]\times10^{-7}$ | $[2.2879857,2.9482768]\times10^{-7}$ | $[0.1815300,1.4523583]\times10^{-7}$ |
| 24 | $[3.6251168,4.1905127]\times10^{-7}$ | $[3.0342635,3.5761848]\times10^{-7}$ | $[1.5602610,2.4262095]\times10^{-7}$ |

The minimum directed adjacent gaps at $48r_0$ are greater than
$5.84475\times10^{-8}$ and $8.35627\times10^{-8}$ respectively.
At $24r_0$ they exceed $4.89321\times10^{-9}$ and
$6.08054\times10^{-8}$. At each positive mass the lower endpoint at
$24r_0$ exceeds the upper endpoint at $48r_0$. The conservative
maximum full Schwarzschild correction to $\psi_u$ at $48r_0$ is
$6.723\times10^{-6}$, $9.467\times10^{-6}$ and
$6.652\times10^{-5}$ in increasing mass order. At $24r_0$ it is
$5.710\times10^{-6}$, $4.989\times10^{-6}$ and
$2.459\times10^{-5}$ respectively.
The massless null-flux interval from GOAL-005 is a separate
observable. The finite-cut tube decrease can reflect propagation
delay, energy stored on the strip and later backscatter or horizon
absorption. The signed tube coefficients alone give no integrated
escape fraction.

[Directed 48 result](checks/tube48.json) · [Directed 24 result](checks/tube24.json) · [Failed coarse 48 result](checks/tube48_coarse_failure.json) · [First 24 pass](checks/tube24_first_pass.json)

The initial coarse $48r_0$ run separated only $\mu=0$ from
$\mu=0.20$. Its gaps for $0$ versus $0.10$ and $0.10$ versus
$0.20$ were negative. It is retained as a failed certification
decision, rather than inferred away from sampled convergence.
The first run at $24r_0$ had a $2.04724\times10^{-9}$ overlap
for $0$ versus $0.10$ with the other two comparisons already
separated. The final pass uses smaller cells and the sharper
resolvent moment bound.

## A completed-source band on Schwarzschild

Consider $\mu=0.20$ and complete the same early source by multiplying
by a smooth monotone $\chi(u)$ equal to one through $u=-8$ and zero
for $u\ge u_1=-7.9$. For definiteness take
$\chi=\vartheta(u_1-u)/[\vartheta(u_1-u)+\vartheta(u+8)]$ with
$\vartheta(q)=e^{-1/q}$ for $q>0$ and zero otherwise. Evolve from
the same zero-data graph. In the whole-line retarded representation
the effective force is
$\mathcal F_0(t,x)={\bf1}_{u\ge g(r)}e^{st}S(r)\chi(u)$.
The source on the Cauchy future is smooth up to the graph. The
indicator implements zero initial data in the retarded representation
and contributes a time derivative jump there. Since $g(5)>u_1$,
the effective force is supported at $r>5$. Its $L^1_{t,x}$ norm is at
most
\[
 B_0=\frac{2e^{su_1}}{s}\int_5^\infty |M(r)|\,dr <0.01340253.
\]
Compare this completion with $\mathcal F_1=\mathcal F_0+g_\ell(t-12)
g_\ell(x-x(8))$, where $g_\ell$ is the normalized positive even bump
proportional to $e^{-1/(1-(z/\ell)^2)}$ for $|z|<\ell=1/32$ and
vanishes elsewhere. The pulse lies strictly after the early wedge
and graph. Hence the two completions agree on the full early wedge,
including the tube and null source domains. The additional pulse is a
declared source choice with unit $L^1_{t,x}$ amplitude. The comparison
isolates dependence on post-window source information at fixed
propagator, mass, initial data and early GL growth.

Use $\widehat{\psi}(\omega)=\int e^{i\omega t}\psi(t)\,dt$, and
flux-normalized Jost fields $f_L,f_R$ at the horizon and spatial
infinity. For $\omega>\mu$, their currents
$\operatorname{Im}(\overline f f_x)$ are $-1,+1$. Write
$W=f_L f_R'-f_L'f_R$. The right amplitude from a source
$\widehat{\mathcal F}$ is $-W^{-1}\int f_L\widehat{\mathcal F}\,dx$,
with $f_R$ replacing $f_L$ for the left amplitude. At the horizon
$f_R=\alpha\overline{f_L}+\beta f_L$, so the current gives
$|\alpha|^2-|\beta|^2=1$ and $|W|=2|\alpha|\ge2$. On a compact
frequency band strictly above $\mu$ the retarded Green formula
follows by the one-dimensional outgoing Wronskian matching.
The temporal Fourier integrals and radial pairings converge here
by the source envelopes. Outward/inward positive channel energy
over a band $B$ is
\[
 K_{\rm channel}(B)=\frac{2}{\pi}\int_B\omega |a_{\rm channel}|^2\,d\omega .
\]
The prefactor includes the negative-frequency partner for a real
field and the $M_0$ normalization. In plane-wave normalization
at infinity the integrand is $\omega\sqrt{\omega^2-\mu^2}$ times
the squared amplitude. The WKB basis used here includes the
long-range phase and normalizes to unit current instead.

For $\omega\ge4$ set $p=\sqrt{\omega^2-V_\mu}$, $dy/dx=p$ and
$y(x(8))=0$. Write $f=p^{-1/2}w$ to obtain
\[
 w_{yy}+w=Qw,\qquad
 Q=\frac{p_{xx}}{2p^3}-\frac{3p_x^2}{4p^4}.
\]
The phase at infinity includes the attractive Coulomb logarithm
$\mu^2\log r/[2\sqrt{\omega^2-\mu^2}]$ in addition to
$\sqrt{\omega^2-\mu^2}\,x$. At the horizon it approaches the
ordinary plane-wave phase up to a constant. The global bounds used
in the continuum estimate are
\[
\begin{aligned}
 V_\mu&\le27/256+\mu^2,\qquad
 p\ge p_*=\sqrt{16-27/256-\mu^2},\\
 \|V_{xx}\|_1&\le 2\mu^2/3+38/15,\\
 \|V_x\|_2^2&\le 3(\mu^4/12+9/56+16/90),\\
 \|Q\|_{L^1(dy)}&\le
 \eta=\frac{\|V_{xx}\|_1}{4p_*^3}
      +\frac{5\|V_x\|_2^2}{16p_*^5}<0.01045513.
\end{aligned}
\]
For example $V_r=\mu^2/r^2-3/r^4+4/r^5$ and $V_x=f V_r$.
The derivative bounds integrate the absolute monomial bounds for
$(f V_r)'$ and $\int f|V_r|^2 dr$. The WKB errors come from a
Volterra equation with the sine kernel, so each $w_\pm$ and its
first derivative are within $\delta_W=e^\eta-1$ of the
respective plane-wave values. The $x$-Wronskian of $f_L,f_R$
equals the $y$-Wronskian of $w_-,w_+$. Its difference from $2i$
is bounded by $2(2\delta_W+\delta_W^2)$.

The late pulse has a time Fourier modulus at least
$\cos(4.25\ell)$ on $B=[4,4.25]$. On the support of its
spatial bump $|y|\le4.25\ell$ and $p\le4.25$.
The real part of each Jost pairing with the spatial bump
is at least $\cos(4.25\ell)/\sqrt{4.25}-\delta_W/\sqrt{p_*}$.
This gives a lower amplitude bound by dividing by the
upper Wronskian bound. An upper amplitude bound for the
baseline uses $(1+\delta_W)B_0/(\sqrt{p_*}|W|_{\rm lower})$.
The pulse and baseline can interfere with either sign, so
their difference in moduli is used in the lower bound and their
sum in the upper bound. Applying these inequalities on the
entire band gives the same separate interval for either
channel as follows.

| Completion | $K_{\rm channel}([4,4.25])$ |
|---|---:|
| Baseline | $[0,7.89064\times10^{-6}]$ |
| Pulse alone | $[0.0347151,0.0439278]$ |
| Baseline plus pulse | $[0.0335298,0.0451132]$ |

The high-band energy separation exceeds $0.0335219$ in
either channel. These are quantitative prescribed-source
comparisons under one explicit completion. They demonstrate
that matching early growth, source and data leaves the
later radiative spectrum undetermined.

## Threshold and high-frequency error budgets

For the same $\mu=0.20$, the right Jost field exists at each
$\omega>\mu$ using the outgoing long-range phase. For example
one can use the $p,y$ transform beyond $r=5$ since there
$p^2=(\omega^2-\mu^2)+\mu^2/r-V_0>0$, and propagate through
the finite interior. For the left Jost field the plane-wave
Volterra integral from the horizon gives
\[
 |f_L(x)|\le \omega^{-1/2}
 \exp\left[\frac{\mu^2(r-1)+(1-r^{-2})/2}{\omega}\right]
 \le \mu^{-1/2}\exp[\mu(r-1)+1/(2\mu)].
\]
The first exponential uses the exact integral of $V_\mu$ from
the horizon in $x$. Together with $|W|\ge2$, it yields a uniform
right amplitude bound as $\omega\downarrow\mu$. The radial
source weight $e^{\mu(r-1)}$ is integrable because
$1.2-s-5/60-\mu>0$ in the analytic source tail. The pulse support
has $r\le8+\ell$. The directed bounds for $|a_R|$ are $0.472252$
for the baseline and $56.05216$ with the pulse. Thus for either
coefficient $C$ the escape on $(\mu,\mu+d)$ is at most
$C^2[(\mu+d)^2-\mu^2]/\pi$, vanishing with $d$. For $d=10^{-6}$
the upper bounds are $2.83960\times10^{-8}$ and $0.000400034$.
Subthreshold evanescent waves have zero limiting escape
current and can still contribute horizon energy.

For high frequencies, the baseline obeys the integrated
time-variation bound $4e^{su_1}\int_5^\infty |M|dr$.
This includes the initial graph jump in the whole-line source
representation. The normalized pulse has time variation at
most $2e^{1/3}/\ell$, by its unimodality and a lower bound on
its normalization integral. One integration by parts bounds
$\|\widehat{\mathcal F}\|_{L^1_x}$ by this total variation divided
by $\omega$. The WKB transfer above yields high-frequency
channel bounds $1.9610\times10^{-6}/\omega_H$ and
$1359.433/\omega_H$ on $(\omega_H,\infty)$ for baseline and
pulsed completion respectively, when $\omega_H\ge4$. A
coarse all-band bound follows independently from the work
inequality $\sqrt{2E_m}\le\int\|\mathcal F(t)\|_{L^2_x}dt$.
The code uses the weighted Cauchy–Schwarz estimate with
$1+t^2$ and the source bound $|t|\le3r$ and time support
length at most $3r$ at each $r$. It bounds the normalized
energy by $0.027546$ and $46.90542$ respectively. The remaining
precision obstacle for total escape is the curved transfer
over the intermediate frequencies. Precise total horizon
accounting also requires the subthreshold band.

[Directed band calculation](checks/completed_band.py) · [Band and tail output](checks/completed_band.json)

## Reproduction and finite runs

Run from `research/sav` using Python 3.14.8, NumPy 2.4.6,
SciPy 1.18.0 and python-flint 0.9.0 with 112-bit Arb arithmetic.
The root and input identities are recorded separately in the
main dossier. The four tube outputs retain the inherited input
digests and propagated Riccati enclosure. Commands emit compact
JSON to stdout. The fourth tube run is the second estimator
refinement after the coarse $48r_0$ attempt. A first pass
at the second tube used the already accepted $48r_0$ settings.

```sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
d=GLR-GOAL-016/transport/checks
python3 -B "$d/tube_intervals.py" --radius 48 --u-cells 16 --radial-group 12
python3 -B "$d/tube_intervals.py" --radius 48 --u-cells 128 --radial-group 3 --max-step .25
python3 -B "$d/tube_intervals.py" --radius 24 --u-cells 128 --radial-group 3 --max-step .25
python3 -B "$d/tube_intervals.py" --radius 24 --u-cells 192 --radial-group 2 --max-step .25 --sharper-resolvent
python3 -B "$d/check_kernels.py"
python3 -B "$d/completed_band.py"
```

User CPU seconds in sequence were approximately 1.39, 49.82,
43.99, 101.38, below 0.3 and 0.42. The checks are bounded
one-dimensional perturbation calculations. The reference-kernel
method is independent of the empirical GOAL-008 characteristic
march and its independent spacelike audit, while sharing the
vacuum first jet and using the pinned GOAL-005 radial proof.
