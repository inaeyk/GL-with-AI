# Quantitative bound on the axial front coefficient

This calculation uses the physical $L/r_0=10$ GL mode and the axial
wavefront construction in [INVESTIGATOR.md](INVESTIGATOR.md). Set
$r_0=1$, let $f=1-1/r$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$,
and put $\mathsf L=\partial_t+\partial_x$. The finite launch slice agrees with $t=0$ in the far exterior.
On an outgoing characteristic crossing it at $R$ we have
$t_R=0$, $t=t_R+x-x_R$ and $dv=2\,dr/f$.

Write the $n=1$ standing first jet as $h=e^{\Omega t}H(r)\cos kz$.
For the phase label that agrees with $u$ on the launch slice, the
eikonal equation at order one is
$$
-\frac4f\,\partial_v p_1-\frac{h_{\mathsf L\mathsf L}}{f^2}=0,\qquad
p_1|_\Sigma=0.
$$
Thus $p_1=-\chi(u,v)\cos kz$ with
$$
\chi(u,\infty)=\frac12\int_R^\infty e^{\Omega(t_R+x-x_R)}
     \left(\frac{H_{tt}}f+2H_{tr}+fH_{rr}\right)\frac{dr}{f}.
\tag{1}
$$
The same transport arises from differentiating the normally hyperbolic
axial curl equations. The order-one principal perturbation is
$-h_{\mathsf L\mathsf L}\partial_u^2/f$ when the background wave
operator is multiplied by $f$. Two successive interactions yield
$\chi^2\cos^2(kz)/2$ times the second derivative of the profile in
the order-two shear. The compact mean gives $\chi^2/4$.

As a gauge control, under an infinitesimal base pullback $p_1$ changes
by $X^u$ and its initial label changes by $X^u|_\Sigma$.
Both the source integral and the launch label must be transformed.
In the unique nonzero-mode unitary chart the launch phase has the
prescribed $p_1|_\Sigma=0$. In a general chart discarding the
transformed initial label can produce a spurious delay even for a
pure gauge deformation. If $H$ vanishes in the completed chart,
the delay and its square vanish for this preparation.

## Analytic envelope

Let $U$ be the pinned normalized GL master. The far-tail estimates
from [GOAL-005, Radiation](../GLR-GOAL-005/RADIATION.md) give
$$
\frac{H_{tt}/f+2H_{tr}+fH_{rr}}{U/r}
=-\frac{(q-\Omega)^2}{k^2}+E(r),\qquad |E(r)|\le28/r
$$
for $r\ge100$, where $q=\sqrt{k^2+\Omega^2}$. The bound adds the
three printed component errors $7/r$, $2(4/r)$ and $13/r$.
Use $0.62<q<0.76$, $0.06<\Omega<0.07$,
$0.394<k^2<0.395$. Then the ratio has fixed sign and magnitude
between $0.48$ and $1.53$. The same pinned comparison gives
$-0.9<(\log|U|)_x<-0.59$ in this region. On $R\le r\le R+1$,
$R/r\ge100/101$, and $r-R\le x-x_R\le(r-R)/0.99$.
Integrating only this unit interval gives the lower bound below.
For the upper bound, the complete exponential factor relative to the
launch value is at most $e^{-0.52(r-R)}$, $R/r\le1$ and $1/f\le1/0.99$.
Consequently
$$
0.14\,e^{\Omega t_R}\frac{|U(R)|}{R}
<|\chi_\infty|<
1.6\,e^{\Omega t_R}\frac{|U(R)|}{R}.
\tag{2}
$$
For $100\le R\le101$ the pinned absolute envelope
$10^{-40}<|U(R)|<3\cdot10^{-21}$ gives
$$
9\cdot10^{-44}<|\chi_\infty|<4.8\cdot10^{-23},\qquad
2\cdot10^{-87}<\chi_\infty^2/4<6\cdot10^{-46}.
\tag{3}
$$
Restore $\chi_{\rm phys}=r_0\chi$ and use a consistently dimensioned
front profile. These are loose bounds on the leading front
coefficient relative to the ordinary shear front. They quantify a
far-zone witness and provide no strong-field waveform estimate.

## News and interference at the front

Choose one real axial $\ell=2$ tensor harmonic
$X_{AB}=D_{(A}X_{B)}$, built from a unit-normalized scalar harmonic, with $\int X^2 d\Omega=12$, and set
$F_j(s)=s_+^j/j!$. Use a front with ordinary outgoing shear
$aF_j(s)$ to leading order, $s=u-u_*$ and $a\ne0$, with a smooth
cutoff away from the front and $j$ sufficiently large. In the
physical Bondi frame the leading terms are
$$
\begin{aligned}
N_{01,AB}&=a\,[F_{j-1}(s)+O(s_+^j)]X_{AB},\\
N_{21,AB}&=a\,[\tfrac14\chi_\infty^2F_{j-3}(s)+O(s_+^{j-2})]X_{AB}.
\end{aligned}
\tag{4}
$$
The fixed-frame tensor flux uses
$F_T=(32\pi G_4)^{-1}\int N_{AB}N^{AB}d\Omega$.
For the interval $[u_*,u_*+\delta]$, the coefficient of
$\epsilon^2\eta^2$ in the tensor energy is
$$
\frac{12a^2}{16\pi G_4}\,
\frac{\chi_\infty^2}{4}\,
\frac{\delta^{2j-3}}{(j-1)!(j-3)!(2j-3)}
+O(\delta^{2j-2}).
\tag{5}
$$
It is positive for sufficiently small $\delta>0$.
It remains positive for sufficiently close smooth approximations
to the two canonical finite-slice axial data, by continuity of
the finite-cut trace. The result is a local interference signal.
The sign of the energy correction integrated over a whole pulse
depends on data and on the rest of the response.

A constant time translation moves the shear front by one
derivative. At fixed launch radii a mass-family derivative has the
same singular order, plus changes of amplitude and tail. The mixed
response in (4) has two front derivatives before taking the news.
With fixed mass the single nonzero time template remains linearly
independent from it upon smoothing. To permit a mass template too,
one can use two separated fronts whose launch cuts have distinct
mass derivatives, and test independence by continuous linear
functionals on the three waveforms. See the
[independent audit](INDEPENDENT-REVIEW.md) and the
[front construction](INVESTIGATOR.md).
