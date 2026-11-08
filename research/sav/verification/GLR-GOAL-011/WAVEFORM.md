# Short-packet waveform, tensor flux and common clock

Source [draft PR #13](https://github.com/inaeyk/GL-with-AI/pull/13) at
[eb439228601a649c658ce719b503ff0bdc37a83f](https://github.com/inaeyk/GL-with-AI/commit/eb439228601a649c658ce719b503ff0bdc37a83f),
stacked on
[0864d9d807de56cbdd6e64d215e6295508a3007f](https://github.com/inaeyk/GL-with-AI/commit/0864d9d807de56cbdd6e64d215e6295508a3007f).
The conclusion and source formulas were known to the auditor. This is a
non-blind analytic reconstruction and adversarial check. The two optical
runs replay different pinned implementations. The normalized-bump
calculation below uses no repository code. All runs are non-production.

## Optical transport and initial data

Use $r_0=1$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$,
$f=1-1/r$, $k=\pi/5$, and the real GL coefficient
$h=e^{\Omega t}H(r)\cos(kz)$ with $H_{\theta\theta}(2)=8$.
A prime on the packet is a dimensionless $u$ derivative. Put
$w=2Q/(\lambda r)$ for the orbit covector, where $\lambda=4$.
The graph is $v=v_\Sigma(u)$ with $u_\Sigma(r)=r-2x(r)$.
Its slope is $-f/(1+1/r)$, bounded away from zero and from null
degeneracy in either launch neighborhood.

The dual curl system has principal symbol $b^{ab}\xi_a\xi_b$ on each
component after eliminating its weighted divergence. For example, set
$l_a=\partial_a\log R$ and expand
$\nabla_b[R^{-2}(dQ)^b{}_a]=\lambda R^{-4}Q_a$.
The constraint $\nabla_a Q^a=4l_aQ^a$ cancels the second derivative
of the divergence. The remaining derivative terms besides the
covariant wave operator are
$-2l^b\nabla_bQ_a-2l^b\nabla_aQ_b$.
The product connection has no $z$ indices and $l^z=0$ on the
background. A term feeding noncompact components from $Q_z$ therefore
cannot have a $u$ or $v$ derivative. In the $z$ equation the radial
first derivative on $Q_z$ cancels after division by $r$, and the
remaining off-diagonal $\partial_z Q_b$ terms are zero order after
Fourier projection. Thus the background $u$-transport matrix
annihilates $e_z$ even for the massive Fourier sector. This
decoupling is a statement about the leading derivative only. The
lower-order mixed equations and compact polarization still enter.

Write $L=\partial_t+\partial_x$, so $h_{vv}=h(L,L)/4$ and
$b_0^{uv}=-2/f$. The outgoing inverse metric variation is
$\delta b^{uu}=-(b_0^{uv})^2h_{vv}=-h(L,L)/f^2$. Multiplying the
wave system by $f$ gives principal terms $-4\partial_u\partial_v$
at order zero and $-h(L,L)\partial_u^2/f$ at order one.
The first eikonal perturbation of the outgoing phase is equivalently
$p_1=-\chi\cos(kz)$, with
\[
 \partial_v\chi=\frac{e^{\Omega(u+v)/2}H_{LL}}{4f},
 \qquad \chi(u,v_\Sigma(u))=0 .
\]
The zero launch phase refers to the fixed graph and packet label in
the completed coordinates. A pure orbit pullback changes the phase
by the displacement of those coordinates, including the endpoints.
For instance the pure-pullback contribution to $h_{vv}$ is
$-f\partial_v\xi^u$, hence the eikonal shift is $\xi^u$ when the
launch label is pulled back as well.

The dual initial position is fixed by $Q_z=\pi^r$, $Q_r=-\pi^z$
and by dual Gauss. The latter has zero numerator for
$\widetilde\pi^r=\lambda A_z=0$ and
$\widetilde\pi^z=-\lambda A_r$ independent of $z$, so $Q_n=0$.
These data give leading upper position zero. Expressing the normal
condition in coordinate components can at most add coefficients
times $a_\delta$. The initial first derivatives come from the
dual electric and divergence relations, and have size
$O(A_\delta/\delta)$ at the upper orders. Here
$A_\delta=\sqrt\delta$ and $a_\delta=A_\delta b((u-u_*)/\delta)$.
The electric relation contains the prescribed $A_r$, which is
linear in $a_\delta$ and $a_\delta'$. The divergence solves the
normal derivative of $Q_n$ in terms of first tangential
derivatives of the fixed position and lower terms. Neither
relation forces an upper derivative of size
$A_\delta/\delta^2$ on the graph. This derivative count would
fail for additional homogeneous data with a different packet
scaling. At the baseline, $w_{0z}=a_\delta$ and
$w_{0z,v}=0$ on the graph.

The leading mixed equation is
$w_{1,uv}=-e_z\chi_v\cos(kz)a_\delta''$, giving
$w_1^{\rm lead}=-e_z\chi\cos(kz)a_\delta'$. At returning order the
insertion on this field gives
$\langle w_{2,uv}\rangle=e_z\chi\chi_v a_\delta'''/2$ at highest
packet derivative. Direct quadratic metric coefficients have only
two derivatives of $a_\delta$. Thus the leading mean is
$e_z\chi^2 a_\delta''/4$. Independently, averaging
$a_\delta(u-\epsilon\chi\cos(kz))$ to second order gives the same
coefficient from the Taylor factor $1/2$ and
$\langle\cos^2(kz)\rangle=1/2$. This argument recovers the leading
term from the phase while the transport estimates control amplitude,
compact propagation, launch derivatives and direct second variation.

Since $dv=2dx$, the limiting optical coefficient on a ray
launched at $R$ is
\[
 \chi_\infty(u_\Sigma(R))=
 \frac12\int_R^\infty e^{\Omega[u_\Sigma(R)+x(r)]}
       (H_{tt}/f+2H_{tr}+fH_{rr})\,dx,\qquad
 K_*=\chi_\infty(u_*)^2/4 .
\]
Here $H_{LL}=H_{tt}+2fH_{tr}+f^2H_{rr}$. The graph's
$t_\Sigma(R)=-\log(R-1)$ is included in the exponent via
$u_\Sigma(R)=t_\Sigma(R)-x(R)$. The integrand has contributions
of both signs. Dropping the signed outer contribution or using a
$t=0$ launch changes $K_*$. In physical units
$K_*=(\chi_{\infty,\mathrm{phys}}/r_0)^2/4$.

## Width-uniform derivative check

All estimates concern the ordered jets on the first fixed fractional
packet strip. Its minimum radius stays above $1$ for sufficiently small
$\delta$. On this strip the background mixed system takes the form
\[
 w_{uv}=\mathcal F=A w_u+B w_v+D w+s,\qquad
 A,B=O(v^{-1}),\quad D=O(1)
\]
at large $v$, with the stated bounds also for the finitely many
coefficient derivatives used here. The mass term belongs to $D$.
For the mean $z$ scalar $A=B=0$, $D=-V_{\rm RW}/4=O(v^{-2})$.
The first GL coefficients and their derivatives decay exponentially
at any fixed rate below $(q_\infty-\Omega)/2$ in $v$. The mean
Einstein geometry has twice this rate, up to polynomial factors,
by the fixed-$v$ sourced quadratures and the fixed mass and clock
conditions. Pure mean radion terms cancel in the relevant equation.

Put $g=w|_\Sigma$, $h=w_v|_\Sigma$,
$a(v)=\max(u_*,v_\Sigma^{-1}(v))$ on the early triangle,
and $Y=w_u$. The exact integrations there are
\[
 Y(u,v)=g'(u)-v_\Sigma'(u)h(u)+\int_{v_\Sigma(u)}^v\mathcal F(u,V)dV,
 \quad
 w_v=h(a(v))+\int_{a(v)}^u\mathcal F(U,v)dU,
 \quad
 w=g(a(v))+\int_{a(v)}^u Y(U,v)dU .
\]
Above the triangle the lower $u$ endpoint is constant and the
trace there vanishes by finite propagation and bump flatness.
The first integrations bound the compact part by the data and
forcing with Gronwall and an integral of length at most $\delta$.
Above the triangle,
\[
 (1-B I_u)Y_v=(A+D I_u)Y+s,\qquad
 I_u\phi=\int_{u_*}^u\phi(U,v)dU .
\]
With $\|I_u\|\le\delta$, $1-BI_u$ is uniformly invertible
for sufficiently small width. At large $v$ the operator norm on
$Y$ is at most
$(\|A\|+\delta\|D\|)/(1-\delta\|B\|)$. For any fixed
$0<\sigma<(q_\infty-\Omega)/2$, first taking a fixed large
$v_0$ and then small width makes this less than $\sigma$. This
bounds mixed $Y$ by $O(e^{\sigma(v-v_0)})$ times the specified
packet scale. The mixed field alone may grow in this estimate.
The product with first GL coefficients decays and is integrable.
For the mean, the $v^{-2}$ potential allows an unweighted
estimate through null infinity.

The background scalar source of the remainder after subtraction of
$a_\delta$ is $-V_{\rm RW}a_\delta/4$. Integrating for $Y$ and then
$w$ gives bounds $O(A_\delta)$ and $O(A_\delta\delta)$,
respectively. Commuting up to two more $u$ derivatives on the
$Y$ equation costs at most $\delta^{-1}$ per derivative, including
the differentiated lower limit $v_\Sigma(u)$. In particular the
baseline remainder's $uu$ and $uuu$ derivatives are bounded by
$O(A_\delta/\delta)$ and $O(A_\delta/\delta^2)$.
Baseline $w_{0,vv}$ comes from differentiating the
$u$ integral of $D w_0$ and is $O(A_\delta)$. Its third $v$
derivative has a moving-endpoint term with one tangential derivative
of $D w_0$ and is $O(A_\delta/\delta)$. Mixed $uv$ derivatives
of the baseline follow directly from its scalar equation.

After subtracting $w_1^{\rm lead}$ the uncancelled mixed forcing is
$O(A_\delta/\delta)$ in the integrated weighted norm, with one
$u$ derivative costing a factor $\delta^{-1}$. This uses the
baseline $uu$ and $uuu$ remainder bounds and $A e_z=0$.
On the slice the mixed remainder has value $O(A_\delta)$ and
first derivatives $O(A_\delta/\delta)$ including those of the
subtracted lead, since $\chi|_\Sigma=0$.
Consequently the mixed remainder is $O(A_\delta)$ with
$uu$ derivative $O(A_\delta/\delta^2)$ in the weight.
One also needs an estimate for the total $w_{1,vv}$ in the
second insertion. Differentiating the early triangle formula gives
the boundary term
\[
 a'(v)\big[h_1'(a(v))-\mathcal F_1(a(v),v)\big].
\]
Both terms in brackets are $O(A_\delta/\delta^2)$.
The $u$ integral contains $\partial_v s_1$ with at most
$O(A_\delta/\delta^2)$ before its decaying first-order coefficient
is counted, since $w_{0,uuv}$, $w_{0,uvv}$ and $w_{0,vvv}$ follow
from the baseline estimates just given. The differentiated
$BI_u$ term containing $w_{1,vv}$ is again absorbed. Above the
triangle the boundary term vanishes. This controls $w_{1,vv}$
at $O(A_\delta/\delta^2)$ with the mixed weight. The differentiated
slice term is necessary for this estimate.

The returning leading term has vanishing initial value and first
derivative on the graph. Subtract it to cancel the $a_\delta'''$
forcing. The remaining compact derivative, transport, explicit
quadratic metric and mixed-remainder terms have integrated size
$O(A_\delta/\delta^2)$. Thus the mean remainder $w_{2,u}$ is
$O(A_\delta/\delta^2)$ and its value $O(A_\delta/\delta)$.
Absolute integrability of the mean forcing and potential gives the
limits of the field and its first derivative. On the fixed strip
$\chi_\infty(u)^2/4=K_*+O(\delta)$, with bounded first derivative,
so freezing at the first launch cut preserves these estimates.
This reconstructs the four asymptotic sizes in WAVEFORM (1).
The reasoning establishes the scale bounds and gives no numerical
value for their constants.

## Shear, tensor flux and signs

Let $Y_{2m}$ be real with unit sphere norm,
$X_A=-\varepsilon_A{}^BD_BY$, $X_{AB}=D_{(A}X_{B)}$.
Then $\int |X|^2=6$ and
$D^B X_{AB}=-(\lambda/2)X_A$, giving
$\int |X_{AB}|^2=12$ by integration by parts. In the
Einstein-frame Bondi expansion let $h_{rA}=0$,
$h_T=rC/2+O(1)$, and
$h_{uA}=U X_A+O(r^{-1})$.
The leading angular Bondi constraint gives
$U X_A=\tfrac12 D^B(C X_{AB})$, so $U=-\lambda C/4$.
The $h_T$ gradient drops out of $F$, giving
$F_{ur}=-\partial_r(h_u/r^2)=2U/r^3+O(r^{-4})$.
Since $F_{ur}=F_{tr}$ and $Q_z=-r^4F_{tr}$ in the product
limit, $\lim 2Q_z/(\lambda r)=C$. An independent check is the
retarded $u$ Proca equation $\partial_r Q_z=\lambda r^2 A_r$
at leading order. Here $A_r=C/(2r^2)+O(r^{-3})$.
The mean backreaction and compact products vanish fast enough
at null infinity on the cut strip for both checks. The
normalization agrees with Martel–Poisson (5.13), (6.11)–(6.12) and
the tensor power factor in (6.16).
[Martel–Poisson, version 1](https://arxiv.org/pdf/gr-qc/0502028v1)

The leading orthonormal transverse metric strain is
$C_{\rm phys} X_{\hat A\hat B}/r_{\rm phys}$. The physical
Bondi tensor power is
\[
 \frac{dE_T}{du_{\rm phys}}=
   \frac{12}{32\pi G_4}\left(\frac{dC_{\rm phys}}{du_{\rm phys}}\right)^2,
 \qquad G_4=G_5/L .
\]
Using the raw curl as $C_{\rm phys}$ multiplies this result by
four for $\ell=2$. The background spherical news is zero and
half-period symmetry removes the mixed mean shear. Therefore the
ordered $\epsilon^2\eta^2$ tensor energy on the fractional interval
$J\subset[0,1]$ is
\[
 E_{22,T}(J)=\frac{12 r_0}{16\pi G_4}
   \left[K_*\delta^{-2}\int_J b'b'''dy+O(\delta^{-1})\right].
\]
This expression is a tensor interference coefficient. The total
tensor power is quadratic and nonnegative. Radion and timelike
massive fluxes are different observables.

The full interval integral equals $-\int_0^1(b'')^2$ by
integration by parts and bump flatness. Since $b'$ vanishes
at $0,1/2,1$, two Dirichlet Poincaré inequalities give
$\int_0^1 (b'')^2\ge 4\pi^2$ for the adopted normalization.
There is also a concrete early interval with positive leading
coefficient. For $p=y(1-y)$, $q=1-2y$, differentiation gives
\[
 \frac{b'}b=\frac q{p^2},\qquad
 \frac{b'''}b=\frac q{p^6}(1-10p+24p^2-12p^3).
\]
The cubic decreases on $[0,7/64]$ and has value
$11643/65536>0$ at the upper endpoint. Thus $J=[0,1/8]$
gives a strictly positive leading integral. The same argument gives
pointwise leading positivity in the open early interval, and
positivity of the ordered integrand on a closed interior fractional
subinterval for sufficiently small width. The flat start prevents
a pointwise conclusion there from absolute bounds alone.

For explicit threshold bookkeeping, suppose the news errors obey
$\|N_{01}-a_\delta'\|_\infty\le B_0 A_\delta$ and
$\|N_{21}-K_*a_\delta'''\|_\infty\le B_2 A_\delta/\delta^2$.
The absolute integrated interference error in the square bracket
is bounded by
\[
 \delta^{-1}\big[B_2\|b'\|_{1,J}
     +K_*B_0\|b'''\|_{1,J}\big]+B_0B_2|J|.
\]
The sign follows once this is smaller than the magnitude of the
leading term. Since $B_0,B_2$ have no numerical bounds in the
source, this gives an existence statement for sufficiently small
width. An algebraic control illustrates the limitation. Set
$C_{01}=a_\delta$ and
$C_{21}=(K_*-2K_*\delta/\delta_0)a_\delta''$ for a fixed
$\delta_0>0$. These traces satisfy all four short-width orders with
finite uniform constants and flip both signs at $\delta=\delta_0$.
This is a counterexample to deducing a numerical threshold from the
order symbols alone. It uses manufactured traces, with no claim of
realizing them by the fixed physical equations. In particular the
residue width $\delta_0=0.12$ has no flux-sign certification here.

## Common clock and mass

At fixed ADM mass and in the fixed Bondi frame, an order-two real
common clock perturbation acts as $C_{21}\mapsto C_{21}+c C_{01}'$,
with the sign of $c$ determined by which time variable is held fixed.
The packet is initially zero and smooth, so the Laplace derivative
multiplies the baseline pole by $s_q$ without an extra pole from
the time boundary. Write $D=s_q A_{01,q}$. For nonzero $D$ the
oriented perpendicular component is
$\operatorname{Im}(A_{21,q}\overline D)/|D|$, invariant under
$A_{21,q}\mapsto A_{21,q}+cD$.
With $A_{21,q}=I/W'$ and $D=-s_qJ_h/W'=K/W'$ this gives
$\operatorname{Im}(I\overline K)/(|K||W'|)$ with the sign in
SIGNED-RESIDUE. The sign refers to the chosen complex pole, reversing
on its conjugate. A common finite change of zeroth-order time
origin rescales this signed component by a positive factor from
the damping, so its numerical value is specified at the pinned origin.
The projection is a quotient by a real one-dimensional direction
with the ADM mass and frame fixed.

At leading short width the time template has shape $b'$ and the
returning shear shape $b''$. Their inner product is zero. For the
paired comparison, let $R_1,R_2$ be fixed physical launch radii
for the variation of the reference Schwarzschild mass $M$.
Since $\partial_M\partial_r x=2/(rf^2)$ with $f=1-2M/r$,
the mass derivatives of the two launch cuts $R_i-2x_M(R_i)$
differ by $-4\int_{R_1}^{R_2}(rf^2)^{-1}dr\ne0$.
An additive mass-dependent clock cancels in that difference.
The two independent $b'$ projections resolve the mass and time
templates in this formal joint comparison. A $b''$ projection
then detects the leading return. This argument applies to the
specified pair and does not assign a mass-invariant residue to a
single experiment with varying mass.

## Reproduction

With Python 3.14.8, SciPy 1.18.0, NumPy 2.4.6 and python-flint
0.9.0, from the pinned source GOAL-011 directory run

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
python3 checks/barrier_delay.py
python3 checks/barrier_delay_independent.py
python3 checks/physical_overlap/clock_projection.py
~~~

The first run reproduced the following intervals (endpoints here
rounded outward), conditional on the pinned GL branch.

| $R/r_0$ | $\chi_\infty/r_0$ | $K_*$ | ODE $K_*$ at endpoint 65 |
|---|---|---|---|
| $13/8$ | $[-1.1026059310,-1.0161239370]$ | $[0.2571920718,0.3039349600]$ | $0.2805598551946$ |
| $7/4$ | $[-0.6036170333,-0.5371805953]$ | $[0.0715890229,0.0910883808]$ | $0.08133784803108$ |

The weighted tail cap was $7.187\times10^{-16}$ rounded up.
The second run used the pinned independent ordinary differential
equation and signed quadrature implementation. At $13/8$ the
first two bins $[R,2]$ and $[2,3]$ contributed
$-0.92941178395$ and $-0.26507545268$, while $[3,5]$ and $[5,10]$
contributed $+0.09185441626$ and $+0.04182207446$. The effect of
using $t=0$ instead of the pinned graph was
$\chi=-1.02831413324$ instead of $-1.059358022945$ at $13/8$.
These are numerical controls on the optical coefficient.

The third run reproduced the interval
$[-0.44589263154194,-0.33745293738320]$ (rounded outward)
for the signed perpendicular component. For a separate coarse sign
test, the reported ordinary residue is in
$[-0.000565,-0.000555]+i[0.000653,0.000667]$. The pole has both
real and imaginary parts negative, so $D=s_q A_{01,q}$ has
positive real part. Its imaginary part is positive since
$0.7473(0.000555)>0.178(0.000667)$. With
$\operatorname{Re}A_{21,q}>0$ and
$\operatorname{Im}A_{21,q}<0$ in the reported signed enclosure,
$\operatorname{Im}(A_{21,q}\overline D)<0$ follows from quadrant
information alone. This checks the projection sign conditional on
the two residue enclosures.

Independent ordinary high-precision quadrature using mpmath 1.3.0 with
numerical differentiation of the normalized profile gives
$c_b=21.3713957850707$, $\int(b'')^2=65.2215886760861$,
$\int b'b'''=-65.2215886760861$, and
$\int_0^{1/8} b'b'''=0.369656467484162$.
The following reproduces these numbers. The signs above use the
analytic identities and bounds.

~~~python
import mpmath as m

m.mp.dps = 45
def bump(y):
    return m.exp(-1 / (y * (1 - y))) if 0 < y < 1 else m.mpf(0)

def product(j, k, y):
    return m.diff(bump, y, j) * m.diff(bump, y, k) if 0 < y < 1 else m.mpf(0)

cuts = [0, m.mpf(1) / 8, m.mpf(1) / 2, m.mpf(7) / 8, 1]
norm2 = m.quad(lambda y: product(1, 1, y), cuts)
print(1 / m.sqrt(norm2))
for j, k in ((2, 2), (1, 3), (1, 2)):
    print(m.quad(lambda y: product(j, k, y), cuts) / norm2)
print(m.quad(lambda y: product(1, 3, y), cuts[:2]) / norm2)
~~~

The checks establish the analytic leading shape and flux signs in
the controlled short-width limit of the ordered response.
The certified residue and common-clock projection at the specified
width are separate from this expansion. Finite-width tensor
interference at that width, finite-time pole dominance and the
finite-amplitude Bondi remainder remain unresolved by these results.
