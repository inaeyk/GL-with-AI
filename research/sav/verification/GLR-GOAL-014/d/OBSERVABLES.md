# D. Leading test, homogeneous graph summand, and tensor observable

This external pass uses the fixed hierarchy at
[0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195).
The GOAL-014 reported intervals, GOAL-013 leading-test construction and
source hypotheses were visible before reconstruction. The alternating
comparison below, the independent reciprocal-radius integration and the
explicit harmonic and cut fixtures use no source code. The numerical
graph interval check is a source replay and analysis of its arithmetic.
The normalized GL eigenpair, five-dimensional density reduction,
fixed-mass source and asymptotic trace inputs are inherited.

## Leading news on a sharp cut

Use the original $u,v$ with $K=V/4$, $A_u$ the ray integral against $K$,
and $J(u)=A_u1=3/r_\Sigma-3/(4r_\Sigma^2)$. For $w_0|_\Sigma=B(u)$
and $(w_0)_v|_\Sigma=0$, integration first in $v$ then in $u$ gives
$w_0=B-Tw_0$, where
\[
 (T\phi)(u,v)=\int_a^u ds\int_{b(s)}^v K(s,z)\phi(s,z)\,dz .
\]
The inner integral is zero when $v<b(s)$. The leading news is
$g=B'-A_u w_0$. This follows from the bound on ray mass and the
fixed-$v$ graph value. The baseline is flat at the first cut, so
$g(a)=0$.

There is a direct positivity argument at the last packet cut. For
$B\ge0$ define $a_n=(A_d T^n B)$. Fix the inner $n$ ordered pairs of the
integral for $T^{n+1}B$. The additional outer pair ranges over $u_1$
between $u_2$ and $u$, and $v_1$ between $v_2$ and $v$. Because $b$
decreases, $v_2\ge b(u_2)\ge b(u_1)$, so integrating out that pair
costs at most $(u-a)M$ with $M\ge\sup J$. Thus
$0\le a_{n+1}\le(d-a)M a_n$ for $n\ge1$. This argument needs an inner
pair and does not use a ratio bound starting at $n=0$. The exact
Neumann series gives
\[
 g(d)=B'(d)-J(d)B(d)+a_1-a_2+a_3-\cdots .
\]
For the pinned nonzero bump $a_1>0$ and $M(d-a)<1$ on both windows.
The alternating sum after $B'-JB$ lies in $[(1-M(d-a))a_1,a_1]$.
In particular at the full packet endpoint $B=B'=0$ and $g(d)>0$.
The strict sign comes from the scattered baseline and follows from
positivity of the kernel and the short-strip bound.

For a directed evaluation compare the kernel in $a_1$ to its
diagonal using $|\partial_u\log K|\le1/4$. Indeed
\[
 \partial_x\log K=1/r^2-f(4r-3)/(r(2r-1)),\qquad
 0\le f(4r-3)/(r(2r-1))\le 2(r-1)/r^2\le1/2
\]
and $1/r^2<1/2$ throughout the two ray domains. Then $a_1$ is bracketed
by $e^{\pm(d-a)/4}\int_a^d J(s)^2 B(s)\,ds/2$. Independent whole-cell
range sums for the bump norm and this diagonal integral give
$g(d)\in[0.411534530713,0.413566757274]$ early and
$[0.006030030100,0.007994479393]$ at the packet endpoint (rounded
outward). The first interval is deliberately looser than the source
bound. For an empirical check of the actual off-diagonal integrand,
set $q=1/r(s,v)$ so $K(s,v)\,dv=-(3-3q/2)\,dq$. The cumulative
inner ray kernel is $J(s)-3q+3q^2/4$. In the outer integral retain
$K(d,v)/K(s,v)$ and evaluate $r(d,v)$ by tortoise inversion. A
96-by-96 Gauss rule gives $a_1\simeq1.50084735181\times10^{-6}$ early
and $a_1\simeq0.00777401583224$ at the packet endpoint, both inside the
directed $a_1$ intervals. These two numbers have no directed Gauss error.

I audited the narrower D certificate by differentiating $A_uT^2w_0$.
Its lower-end term vanishes because $T^2w_0|_\Sigma=0$. In the
two-scatter diagonal replacement, the derivative of the upper
time limit gives zero error because the exact diagonal is $J^2/2$.
The step integrand has a continuous primitive and its weak derivative
has the cell error bounded in the code. The midpoint remainder for
the bump derivative norm uses $[(b_0')^2]''=2[(b_0'')^2+b_0'b_0''']$.
The endpoints can be capped by flat bounds. The change from
$\sqrt\delta g$ as a function of $y$ to $g$ as a function of $u$ leaves
the $L^2$ error unchanged and multiplies the derivative $L^2$ error by
$\delta^{-1}$. I recover the directed interpretation of the early
$1.900\times10^{-7}$ derivative error and $9.709\times10^{-9}$ endpoint
error. The early pointwise compact jets are a distinct estimate, with
$K\le 2/9$ following from $f/r^2\le4/27$. The moving graph terms for
$w_{0,uu}$ and $w_{0,vv}$ have opposite origins, respectively the
lower endpoints in the $v$ and $u$ integrations. The source caps
$|b'|<0.24$ and $|1/b'|<4.23$ hold on the early graph.

## Homogeneous graph summand

At the tilted graph $t=T(x)$, tangency of the vanishing return
displacement gives $Q_x=-pQ_t$, $p=-1/r$. Combine the return rows
$\lambda r^2 A_t=Q_x+\mathcal D^t$ and
$\lambda r^2 A_x=Q_t-\mathcal D^x$ with $A_x+pA_t=0$. The result is
\[
 Q_t=\frac{\mathcal D^x-p\mathcal D^t}{1-p^2},\qquad
 w_u|_\Sigma=\frac{\mathcal D^x-p\mathcal D^t}{\lambda r(1-p)}
\]
from $w_u=(w_t-w_x)/2$ and $w=2Q/(\lambda r)$. The opposite
denominator $1+p$ incorrectly amplifies this launch by
$(1-p)/(1+p)\simeq4.2$ on the early graph.

For the first mixed graph covector, $A_{1x}=-p A_{1t}$ and
$(1-p^2)A_{1z,t}=ik(A_{1t}+pA_{1x})$. Contract the two massive rows
with $(1,p)$ and substitute this compact electric condition.
The coefficient of $A_{1t}+p A_{1x}$ becomes
$D-k^2r^4=\lambda r^2$. Since the real growing mode has Fourier
coefficients $\bar M/2$ at $\pm k$, the mixed graph covector is
$(1,-p)^T[(1,-p)\bar M A_0]/[2r^2(1-p^2)]$. The returning mean
contraction uses $\bar M$ times this covector because there are two
conjugate cross terms. The direct density already includes
$\langle\cos^2 kz\rangle=1/2$. The fixed-mass mean contribution must
retain $\beta-2\mu/(rf)$ including the time factor $e^{2\Omega r}$ on
$v=r$. These factors agree with the independent density contractions
in the source E march. The local regular GL transport uses the pinned
Fuchsian initialization, Taylor flow with a complex-disk Cauchy tail,
and the inherited normalization and mean boxes.

With graph slope $q$, integration at fixed $v$ of the homogeneous
equation gives a free shear bounded by $\int_a^u|q(s)|ds$ and
the same positive Volterra majorant. Therefore the news differs
from $q(u)$ by at most $M\int_a^u e^{M(u-s)}|q(s)|ds$. The code
pairs the interval launch with a whole-cell enclosure of $g$ and
covers this correction. The displayed complete interval
$[-2.05250,-1.41374]\times10^{-4}$ is rounded outward from the
replayed primary enclosure and is conditional on the physical GL
and mean inputs. The E central value about $-1.74125\times10^{-4}$
corroborates the graph sign empirically with separate local GL and
RW integration, sharing the historical mean construction. This
summand is specific to the exact-curl Duhamel split. It is already
encoded in the weak problem and supplies no bound on the volume
summand.

## Tensor normalization, matched cuts, and finite seeds

On the unit sphere $X_A=-\varepsilon_A{}^B D_BY$, $\int Y^2=1$ and
$\Lambda=\ell(\ell+1)$ imply $\int X^2=\Lambda$. The unit Ricci
commutator gives
$\int |DX|^2=(\Lambda-1)\Lambda$ and
$\int (D_AX_B)(D^BX^A)=-\Lambda$. The symmetrized derivative has
norm $\Lambda(\Lambda-2)/2=12$ for $\ell=2$. The script checks
the same factor by direct integration of $Y_{20}$ in $x=\cos\theta$.

In inertial Bondi gauge, $h_u X_A=(1/2)D^B(C X_{AB})$ at leading
order. Then $h_u=-\lambda C/4$, $h_r=0$, and the gradient in the
axial potential drops from its curl. It follows that
$F_{ur}=-\lambda C/(2r^3)+O(r^{-4})$. The retarded-to-static
coordinate change preserves $F_{tr}=F_{ur}$. With pinned orientation
$Q=-r^4F_{tr}$ asymptotically, the dimensionless shear is the limit
of $2Q/(\lambda r)$. The physical shear is $r_0 C$ and
$du_{\rm phys}=r_0du$, so the news itself is dimensionless.
The fixed circle reduction sets $G_4=G_5/L$. Integrating the
tensor part of the mass-aspect flux gives the interference
coefficient $12\cdot2r_0\mathcal P/(32\pi G_4)$, with the factor
two from the cross term. Its dimensions are energy. The selected
harmonic may have a signed ordered coefficient while the exact
tensor flux integral is nonnegative. This statement assumes the
inertial Einstein-frame null trace of the ordered hierarchy.
[Bondi angular shift, Mädler–Winicour (56)](https://arxiv.org/pdf/1609.01731v3)
· [Odd master and radiative metric, Martel–Poisson (5.13), (6.11)–(6.12)](https://arxiv.org/pdf/gr-qc/0502028v1)
· [Mass-aspect flux, Flanagan–Nichols (2.11a)](https://arxiv.org/pdf/1510.03386v5).

For $u_{\rm old}=u+\epsilon^2\xi(u)$ the inertial news composes
with $u_{\rm old}$ and the energy measure picks up
$1+\epsilon^2\xi'$. The fixed-label coefficient shift is
$\int(\xi g g'+\xi'g^2/2)=[\xi g^2/2]$. Moving both endpoints to
the labels of the same physical cuts adds its negative through the
leading flux. A separate exact polynomial control yields coefficients
$-29/12$ before relabeling, $+19/12$ at held labels and $-29/12$
after matching cuts. A new inertial frame or a different harmonic
projection would require its own observable definition. A general
time-dependent change of labels is a bookkeeping pullback.

The finite-seed Taylor inequality follows in $L^2(I)$ under the
specified evenness in $\epsilon$, vanishing at $\eta=0$, common
inertial trace and uniform mixed derivative assumptions on a
parameter neighborhood. Taylor at fixed $\eta$ supplies the
$\epsilon^4\eta M_{41}/24$ error. Taylor of
$\partial_\epsilon^2\mathcal N(0,\eta)$ supplies
$\epsilon^2\eta^2 M_{22}/4$. Expanding the squared norm yields
the source's last positive term $\epsilon^2(\|N_{21}\|+e_{21})^2/2$
in the normalized energy error. A constrained nonlinear family and
differentiability of its news in the physical cuts remain open.
Likewise the conditional preparation radius follows by
Cauchy–Schwarz for bounded ordered maps, with no evaluated margin
or applicable norm constants for the full response.

## Reproduction and decision

Run `PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 checks.py`
from this directory and compare [checks.txt](checks.txt). The initial
independent range implementation returned an indeterminate normalizer
because powering a symmetric near-zero Arb interval by the generic
power operation can return `nan`. Explicit multiplication encloses
the square and preserves the outward normalization, and the final
check requires each endpoint interval to exclude zero. The source D
certificate already uses multiplication for the bump norm.

From `research/sav` at the source SHA, follow the seven D/E commands
in GOAL-014's `REPRODUCTION.md` for the directed baseline, directed
graph and E central graph runs, including listed variants. Every
stdout was byte-identical to its archived text [replay record](REPLAY.txt).
These replays are checks of the original implementations. The
independent alternating positivity and explicit normalization tests
support the subcertificates. Both complete interference signs and the
long-window template decisions still require physical return error
control. The earliest quantitative target is the missing compact
current functional, including the terminal ray, or the stronger
physical cap-news error.
