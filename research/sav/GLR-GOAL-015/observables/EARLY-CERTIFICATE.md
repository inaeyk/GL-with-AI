# Independent early strong certificate

The weak certificate in [analysis](../analysis/ADJOINT.md) avoids
strong news. Here we bound the actual early cap news in $L^2$ by a
stationary time commutation of the lower hierarchy and the exact
one-current readout in [STRONG-CAP](STRONG-CAP.md). The two proofs
share the pinned physical coefficients and the exact ordered
equations. This computation does not use the weak projected sign,
the weak candidate quadrature, a derivative of the returning pilot,
or a conforming scalar lift. The pilot supplies only the binary
ordinates of the affine comparison used by the inherited cap gate.

Write $B$ for the free scalar launch shape, $w_0$ for the exact
leading field, $h=B'+2\Omega B$, and $\Psi=(\partial_t+2\Omega)w_0$.
The exact Bernstein check in [shifted_baseline.py](shifted_baseline.py)
gives $B'\ge522B$, $B''\ge380B'$, $B'''\ge220B''$ throughout the
early window; hence $h'\ge380h$, $h''\ge220h'$. Positivity and
flatness hold on this cut. None of these monotone bounds is used
across the full packet.

Put $W_0=V/4$ and use $p=-1/r$ on $\Sigma$. The graph jets of
$\Psi$ are
$$
 \Psi_\Sigma=h,\qquad P_\Sigma=h'+\zeta,\quad Q_\Sigma=\xi,\qquad
 \zeta=-\frac{VB}{2(1-p)},\quad
 \xi=-\frac{VB}{2(1+p)} .
$$
They follow by differentiating $w_0|_\Sigma=B$ and
$(w_{0,t},w_{0,x})|_\Sigma=(B',-B')$ tangentially and using the
scalar equation to obtain the normal second derivative. With
$\ell(v)=b^{-1}(v)$ in the graph wedge and $a$ above it,
$$
 \begin{aligned}
 P&=h'+\zeta-\int_b^v W_0\Psi,\qquad
 Q=\xi(\ell)-\int_\ell^u W_0\Psi,\\
 \Psi-h&=\int_\ell^u \zeta-\int_\ell^u\int_b^v W_0\Psi,\\
 P_u-h''&=\zeta'+b' W_{0,\Sigma}h
                   -\int_b^v((W_0)_u\Psi+W_0P),\\
 Q_v&=\ell'\big(\xi'(\ell)+W_0(\ell,v)h(\ell)\big)
                   -\int_\ell^u((W_0)_v\Psi+W_0Q).
 \end{aligned}
$$
Here graph terms involving $\ell$ vanish on the flat front. Use
$\int_b^\infty W_0<1.57$, $W_0<1/4$, $|(W_0)_{u,v}|\le W_0/4$,
$|b'|<0.24$, $|\ell'|<5$, and the deliberately widened bounds
$|\zeta|\le B$, $|\xi|\le2B$, $|\zeta'|\le B+B'$ and
$|\xi'|\le2(B+B')$. For example the shear comparison gives
$|\Psi|/h\le (1+522^{-2})/(1-1.57/380)$. The other five rational
bounds follow from these ray equations and the growth ratios.
The six source weights in the order
$(\Psi_{uu}-h'',\Psi_{uv},\Psi_{vv},\Psi_u,\Psi_v,\Psi)$ divided
by $h''$ are at most
$(.00720,.00000301,.000135,.004565,.000000054,.00001202)$.
Directed integration gives $\|h''\|_2<2107.299$.

For the real two-polarization field $Y$, let
$Y_1=(\partial_t+\Omega)Y$. It solves the stationary mixed equation
with source obtained by inserting $\Psi$. Let $g=Y_t|_\Sigma$,
let a prime on $g$ now denote $d/dx$ *along the graph*, and denote
the mixed skew coefficient by $\mathsf C=4L$ (with $L$ conjugated
by the rotation when appropriate). Since $Y|_\Sigma=0$,
$$
 (1-p^2)Y_{tt}|_\Sigma=-p'_x g-2p g'+\mathsf Cg-S_m|_\Sigma .
$$
Thus the shifted graph displacement is $g$,
$Y_{1,t}|_\Sigma=Y_{tt}|_\Sigma+\Omega g$, and
$Y_{1,x}|_\Sigma=g'-pY_{1,t}|_\Sigma$. The graph source uses the
original scalar graph jets
$(w_{0,uu},w_{0,uv},w_{0,vv})=(B''+b'W_0B,-W_0B,W_0B/b')$.
The tilted graph matrix is differentiated along $\Sigma$ by
$\partial_x+p\partial_t$; the $\Omega p$ term in its coefficient
row must stay. After the rotation from [ADJOINT](../analysis/ADJOINT.md),
the incoming $X_1,P_1,Q_1$ are all nonzero. Directed upper
envelopes for their norms divided by $h''$ are
$(0.000068708,0.040170,0.101094)$. For fixed $v$ their graph
bounds are transported using monotonicity of $h'$ (not of $h''$):
a bound in units $h'(\ell)$ is at most that bound times $h''(u)/220$.

Use $A(u,v)=\int_b^v q_0(u,z)\,dz$ and
$P_{1,\mathrm{opt}}=-A h''\,e_1/4$. The same rotated mixed
system as in the weak proof gives a directed defect from source
error, graph and the potential; here the characteristic
displacement bound includes the shifted graph value,
$|X_1|/h''\le X_{1,\Sigma,*}+|P_1|_*/220$ in the graph wedge,
and the first term is zero above it. Each cell uses the supremum
of its partial phase and a positive exponential majorant for
the incoming and accumulated defects, with the exact measure
$dv=2\,dr/f$ on the last ray. This bounds the entire coupled
solution, including the other polarization. Inserting it in
Eq. (6) of [STRONG-CAP](STRONG-CAP.md) gives
$$
 \int_b^c \left|F_t-\frac{2}{\lambda}q_0 A h''\right|dv
        <0.40880\,h'' .
$$
This is an approximation to the physical $F_t$ obtained by
solving the shifted hierarchy. It is **not** the stationary
derivative of the optical current. The omitted graph phase
derivative would be needed for that different operation.

For the chosen reference $p_{\rm opt}=h'' A(u,c)^2/(4\lambda)$,
Eq. (1) in [STRONG-CAP](STRONG-CAP.md) therefore bounds the
actual $w_{2,u}(\cdot,c)-p_{\rm opt}$ using the commuted source,
uncommuted $(G-F)_c$, true $F_\Sigma$, the $H$ ray and the causal
potential term. The last is bounded using the exact uncommuted
weak shear equation with the directed full $F,H,G$ envelopes.
For the comparison $p_{\rm opt}-p_c$, a second direct radial
phase computation correlates $v=u+2x(r)$ at fixed $u$:
$$
 A(u,c)=e^{\Omega u}\int_{b(u)}^{r(u,c)}
               \frac{2}{f}e^{2\Omega x(r)}\,q_{\rm EF}(r)\,dr .
$$
We include the uncertain lower and upper slivers as optional
directed cell integrals and intersect this phase square with the
one from the characteristic slabs. It is this independent
phase evaluation, rather than reducing the mesh defect of the
pilot, that reduces the optical to affine error. The initial
slab-only comparison gave $E_c<537.681$, sufficient for the
positive sign but above the target of 400.

| Additive contribution to $\|w_{2,u}(\cdot,c)-p_c\|_2$ | Upper bound |
| --- | ---: |
| Optical reference to affine comparison | 100.181752 |
| Cap current trace | $3.59\times10^{-100}$ |
| Graph current trace | 0.270272 |
| Commuted $F$ ray | 215.363846 |
| $H$ ray | 1.779083 |
| Potential times causal shear | 6.388838 |
| Total before display rounding | 323.983790 |

The [directed run](optical_strong.txt) and [script](optical_strong.py)
pin the GOAL-014 affine SHA-256. The inherited directed physical
exterior transfer and leading test give
$|\mathcal P_{\rm early}-\langle B',p_c\rangle|<
0.024231+0.013648127E_c$. Combining with the directed affine
pairing $[7.80653,7.86816]$ gives the outward interval
$[3.3605,12.3142]$. This is a strong norm enclosure on the early
cap only. The tolerance is chosen to decide a sign; it should
not be interpreted as a precision waveform or later-window bound.
