# Complete perturbative Einstein hierarchy

Use signature $(-++++)$, $r_0=2M$ with $M$ geometric, $G_4=G_5/L$,
and $k=2\pi/L$. The growing first jet is $H=e^{\Omega t}H(r)\cos kz$
in the unique nonzero-mode unitary representative, with each complex
Fourier amplitude equal to half the cosine amplitude. Use the pinned
$L/r_0=10$ mode with $H_{\theta\theta}(2r_0)/(8r_0^2)=1$.
The second tangent $P$ is a real finite-data $n=0,\ell=2$ perturbation
with either angular parity. Use coefficients without factorials,
$$
g=\bar g+\epsilon H+\eta P+\epsilon^2 B+\epsilon\eta J
   +\epsilon^2\eta K+\cdots.
$$

## Angular and compact ledger

The vacuum operator is equivariant under compact translation and
$O(3)$ acting on the sphere. A product has compact index the sum of the
indices. A two-factor angular projection requires $m=m_1+m_2$,
$|\ell_1-\ell_2|\le\ell\le\ell_1+\ell_2$, and antipodal parity equal to
the product of input parities. Polar and axial parities are
$(-1)^\ell$ and $(-1)^{\ell+1}$. Covariant angular derivatives are
intertwiners. These are necessary conditions for each contraction.
Iterate Clebsch–Gordan for more factors. Tensor news requires $\ell\ge2$.

| Jet | Compact indices | Angular representation | Bondi tensor |
|---|---|---|---|
| $H$ | $\pm1$ | $0^+$ | absent |
| $P$ | $0$ | $2^p$ | ordinary radiation |
| $B$ from $HH$ | $0,\pm2$ | $0^+$ | absent |
| $J$ from $HP$ | $\pm1$ | $2^p$ | massive intermediate |
| $K$ from $HJ,BP,HHP$ | $0,\pm2$ | $2^p$ | $n=0$ permitted |

Half-circle translation implements $\epsilon\mapsto-\epsilon$ for
translation-covariant preparations. Mean jets linear in $\eta$ are
even in $\epsilon$. With $z\mapsto-z$ reflection the mean components
with exactly one compact index vanish. For axial $P$ the $K_{zz}$ mean
is forbidden by angular parity. For polar $P$ it is permitted.
At $\eta=0$, exact $SO(3)$ removes all tensor news to all orders.

## All-component source

Put $C_1(p)^a{}_{bc}=\bar g^{ad}(\bar\nabla_b p_{dc}+
\bar\nabla_c p_{db}-\bar\nabla_d p_{bc})/2$ and
$(p^\sharp)^a{}_b=\bar g^{ac}p_{cb}$. Define
$$
C_2(p,q)=-\operatorname{Sym}_{p,q}[p^\sharp C_1(q)],\qquad
C_3(p,q,s)=\operatorname{Sym}_{p,q,s}[p^\sharp q^\sharp C_1(s)] ,
$$
with averaged symmetrization. For connection differences use
$$
(DC)_{ab}=\bar\nabla_c C^c{}_{ab}-\bar\nabla_b C^c{}_{ac},\quad
(U*V)_{ab}=U^c{}_{cd}V^d{}_{ab}-U^c{}_{bd}V^d{}_{ac}.
$$
Expanding the inverse metric in the exact connection difference
gives $R_1=DC_1$ and the polarized Ricci maps
$$
Q(p,q)=DC_2(p,q)+\operatorname{Sym}_{p,q}[C_1(p)*C_1(q)],\quad
T(p,q,s)=DC_3(p,q,s)+2\operatorname{Sym}_{p,q,s}[C_1(p)*C_2(q,s)].
$$
These include derivatives of inverse factors in $D$ and all compact,
radial, angular and time components. The equations are
$$
\begin{aligned}
R_1 H&=R_1 P=0,\\
R_1 B&=-Q(H,H),\\
R_1 J&=-2Q(H,P),\\
R_1 K&=-2Q(H,J)-2Q(P,B)-3T(H,H,P).
\end{aligned}
\tag{1}
$$
Project after forming products. For example
$[Q(H,J)]_0=Q(H_+,J_-)+Q(H_-,J_+)$ and
$[T(H,H,P)]_0=2T(H_+,H_-,P)$. Only $B_0$ enters the last equation
after mean projection. The $B_{\pm2}$ fields still satisfy their own
forced equations.

If $R_1 h=-S$, then $G_1 h=-\widehat S$, where
$\widehat S_{ab}=S_{ab}-\bar g_{ab}\operatorname{tr}_5 S/2$.
Inverse-metric and prefactor variations in the full Einstein tensor
multiply lower total Ricci coefficients, which vanish by (1).
Thus $\bar\nabla^a\widehat S_{ab}=0$ by the expanded Bianchi identity.
Normal/normal and normal/tangential projections of $G_1h+\widehat S$
give the five sourced slice constraints. Equivalently expand
$R[\gamma]+(\operatorname{tr}k)^2-k_{ij}k^{ij}=0$ and
$D_j(k^j{}_i-\delta^j{}_i\operatorname{tr}k)=0$ on the same slice,
using $k_{ij}=-\mathcal L_n\gamma_{ij}/2$. Variations of the normal and
tangents multiply lower total equations. Reduced evolution with
these data and compatible gauge constraints propagates the vacuum constraints.

For nonzero wave number $\kappa=nk$ impose $h_{zA}=0$ after lower-order
completion. The linear generator is $X_z=-h_{zz}/(2i\kappa)$,
$X_\mu=-(h_{\mu z}+\bar\nabla_\mu X_z)/(i\kappa)$.
The $zz$ and $z\mu$ equations impose
$$
T_h=\operatorname{tr}_4 h=-2S_{zz}/\kappa^2,\qquad
J^h_\mu=\bar\nabla^\nu h_{\nu\mu}
=\bar\nabla_\mu T_h+2iS_{z\mu}/\kappa.
$$
The other equations use
$R_{1\mu\nu}=(-\bar\Box_4+\kappa^2)h_{\mu\nu}/2
-\bar R_{\mu\alpha\nu\beta}h^{\alpha\beta}
+\bar\nabla_{(\mu}J^h_{\nu)}-\bar\nabla_\mu\bar\nabla_\nu T_h/2$.
The trace and divergence are forced on the Cauchy solution.
At $\kappa=0$ the linear unknown splits as
$h_E=h_{\mu\nu}+\bar g_{\mu\nu}h_{zz}/2$,
$\varphi=\sqrt3 h_{zz}/4$, $A_\mu=h_{z\mu}/2$.
Then $\delta G^{(4)}[h_E]=-\widehat S_{\mu\nu}$,
$\bar\Box_4\varphi=\sqrt3 S_{zz}/2$ and
$\bar\nabla^\nu F_{\nu\mu}[A]=S_{z\mu}$.
Reflection removes the mean vector source and its homogeneous data.

## Completion and retarded response

Use a single pullback $\exp(\mathcal L_X)$ with coefficients
$X=\epsilon a+\eta b+\epsilon^2 c+\epsilon\eta d+\epsilon^2\eta e+\cdots$.
Use the same letters for Lie derivatives and $\{a,b\}=ab+ba$.
The transformed higher coefficients are
$$
\begin{aligned}
B'&=B+aH+(c+a^2/2)\bar g,\\
J'&=J+aP+bH+(d+\{a,b\}/2)\bar g,\\
K'&=K+aJ+bB+cP+dH+\tfrac12a^2P+\tfrac12\{a,b\}H\\
 &\quad+\big[e+\tfrac12\{a,d\}+\tfrac12\{b,c\}
 +\tfrac16(a^2b+aba+ba^2)\big]\bar g.
\end{aligned}
\tag{2}
$$
The nonlinear products precede the linear unitary and asymptotic Bondi
conditions. A pure-gauge test pulls back the entire family, including
its slice and data, then undoes the pullback.

In completed reflected coordinates the zero-block Einstein metric
is $g_E=(g_{zz}^{(0)})^{1/2}g_{\mu\nu}^{(0)}$ for asymptotic
matching. Its $(2,1)$ coefficient contains $B_{0,zz}P/2$ in addition
to the linear split. For a linear Schwarzschild RWZ unknown
$K_{E,\mathrm{lin}}$, project $-\widehat S_{\mu\nu}$ with the linear
conserved-source projections of Martel–Poisson. If the chosen
unknown includes $B_{0,zz}P/2$, apply $\delta G^{(4)}$ to this product
and add the result to its source. The Bondi pullback also includes
the variation of null cuts, luminosity radius and angles acting on $P$.
Fix the asymptotic time origin and angular frame and impose
$g_{rr}=g_{rA}=0$ and the areal determinant condition before taking
$\partial_u\lim_{\rho\to\infty}\rho^{-1}g_{E,AB}^{\rm TF}$.

On the Schwarzschild reference domain the projected equation is
$(-\partial_t^2+\partial_x^2-V_p)\Psi_{21}^p=s_{21}^p$, $x=r_*$.
For $\Lambda=(\ell-1)(\ell+2)/2$ and $f=1-2M/r$ the potentials are
$$
V_{\rm o}=f\left[\frac{\ell(\ell+1)}{r^2}-\frac{6M}{r^3}\right],
\qquad
V_{\rm e}=
\frac{2f[\Lambda^2(\Lambda+1)r^3+3\Lambda^2Mr^2+
9\Lambda M^2r+9M^3]}{r^3(\Lambda r+3M)^2}.
$$
Here $s_{21}^p=f\mathscr D_p[-\widehat S_{\mu\nu}]$ with
$\mathscr D_p$ the Martel–Poisson projection including its source
derivatives. Compute their source multipoles with
$8\pi T_{\mu\nu}=-\widehat S_{\mu\nu}$.
See Eqs. (4.25)–(4.27), (5.14)–(5.16) in the [source ledger](LITERATURE.md).
The solution is the homogeneous evolution of the higher master
data plus the forward Green integral from the finite slice.
The massive $J$ solution in (1) is itself a retarded constrained
solution. Future-horizon regularity and absence of an independent
incoming boundary wave follow from this Cauchy construction.
In a stationary Green representation the initial contour is retarded
with the ingoing horizon and outgoing infinity branches continued
from the upper frequency half-plane. A factor $e^{\Omega t}$ shifts
the one-sided Laplace argument from $s$ to $s-\Omega$.
A pole residue requires the corresponding finite-data transform
and does not by itself determine the finite-cut response.

A compact homogeneous RWZ wave added to $K_0$ preserves the lower
jets and the linear upper-order constraints, while changing the
outgoing coefficient. With unspecified upper data, (1) defines
a preparation-dependent operator with a free radiation contribution.
