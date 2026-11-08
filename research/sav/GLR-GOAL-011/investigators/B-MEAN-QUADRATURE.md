# GOAL-011 mean Einstein geometry for the contour overlap

Input conventions are those of GOAL-005
[61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708),
DERIVATIONS and the inherited physics/goal002_second_order.md (4), (5),
(7)–(10). Set $r_0=1$, $f=1-1/r$, $x=r+\log(r-1)$, $v=t+x$.
The input is the unhalved cosine profile $e^{\Omega v}\widehat H(r)\cos kz$
in the regular compact-unitary EF chart.

## Source and quadrature

Use the two-dimensional $(v,r)$ metric $b$ and $W=R^2$. At fixed $z$,
$R^{(4)}_{ij}=R[b]_{ij}-W^{-1}\nabla_i\nabla_jW+
W_{,i}W_{,j}/(2W^2)$ and $R^{(4)}_{\theta\theta}=1-\Box_bW/2$.
The module expands these expressions through order two. The
two-dimensional identity $R[b]_{ij}=b_{ij}\operatorname{Scal}[b]/2$
and $\bar b_{rr}=0$ mean that the $rr$ expansion uses only the
linear scalar curvature variation. The inverse and connection
coefficients are $\bar b^{-1}-\epsilon h^\sharp\bar b^{-1}
+\epsilon^2(h^\sharp)^2\bar b^{-1}$ and
$\bar\Gamma+\epsilon C-\epsilon^2 h^\sharp C$ in this expansion.
The four-dimensional quadratic pieces each have a factor
$\langle\cos^2 kz\rangle=1/2$.

For the spacelike compact foliation, $K_{\mu\nu}=\partial_z g_{\mu\nu}/2$.
The noncompact mean Ricci tensor receives the compact correction
$k^2(H_{\mu\rho}H^\rho{}_\nu/4-\operatorname{tr}_4(H)H_{\mu\nu}/8)$
after factoring $e^{2\Omega v}$. This comes from $2K_{\mu\rho}K^\rho{}_\nu
-KK_{\mu\nu}$, with $\langle\sin^2 kz\rangle=1/2$.
The total $z$ derivative in $R_{zz}$ averages to zero and
$-\langle\operatorname{tr}K^2\rangle=-k^2\operatorname{tr}_4(H^2)/8$.
The physical input has $\operatorname{tr}_4 H=0$. The implementation
keeps the trace term to permit general manufactured tests.

For the full mean Ricci coefficient $Q$, Einstein-frame trace reversal
gives $T_{rr}=-Q_{rr}$ and
$T^v{}_v=fQ_{rr}/2+Q_{\theta\theta}/r^2+Q_{zz}/2$.
Here the $\bar g^{vr}=1$ term of the mixed trace cancels $Q_{vr}$.
In the regular EF gauge
$p^E_{vv}=2\mu/r-2f\beta$, $p^E_{vr}=\beta$, $p^E_{rr}=0$, with
$p^E_{\theta\theta}=0$, the radial equations at fixed $v$ are
$\mu_{,r}=-r^2T^v{}_v/2$, $\beta_{,r}=rT_{rr}/2$.
Set the fixed mass and advanced lapse constants to zero at infinity.
The module integrates the two factored EF amplitudes backward
from the finite endpoint $r_{\rm match}+{\tt ray}\,{\tt length}$
with zero terminal values, along the straight ray and then inward
on the real interval. The exact ray prescription assumes a supplied
analytic GL continuation decaying in that sector. The cutoff omits a
common EF integration constant for each component. On the GL branch
the EF forcing has leading exponential $e^{-2(q_\infty+\Omega)r}$
up to powers, with $q_\infty=\sqrt{k^2+\Omega^2}$. The exposed
tail_proxy_ef is the endpoint derivative magnitude divided by
$2(q_\infty+\Omega)$, a leading-exponential diagnostic. Refining the
length and comparing at fixed radius tests the cutoff. There is no
validated tail bound or interval integrator in this module.

## Interface and controls

- gl.jet(r) returns (h, dh, d2h) with each vector ordered
  (hvv,hvr,hrr,h_theta_theta). gl.omega, gl.k and gl.rmin
  are required. All jet entries factor out $e^{\Omega v}$ and
  are analytic derivatives with respect to the EF radial coordinate.
- MeanGeometry(gl,rmatch,ray,length,tol)(r) returns the amplitudes
  (mu,beta) multiplying $e^{2\Omega t}$, via multiplication of
  the EF amplitudes by $e^{2\Omega x(r)}$. The logarithm is real on
  the physical exterior and principal along the supplied right-going
  ray. mean.jet(r) also returns the static-time radial derivatives.
  match_ef and tail_proxy_ef are in the EF-time convention.
- Running the module as a script checks the exact manufactured
  finite-end result for $W=r^2+\epsilon\cos kz$ and unperturbed
  base metric. The expected $(Q_{rr},Q_{\theta\theta},Q_{zz})=
  (2/r^6,0,-k^2/(4r^4))$ gives elementary antiderivatives. DOP853
  at tolerance $10^{-11}$ with rmatch=3, length=24 and rays $1$
  and $e^{0.35i}$ gave maximum quadrature error $1.92\times10^{-13}$.
- A separate manufactured check used
  $\widehat H=e^{-0.8r}(0.3+1/r,-0.5+0.2r,0.4-0.1r+1/r^2,0.9+0.3r^2)$
  with $\Omega=0.13$, $k=2\pi/10$. The exact 3D Christoffels,
  their derivatives from $\partial_j b^{-1}=-b^{-1}(\partial_j b)b^{-1}$,
  and the sphere warp gave the direct 5D Ricci tensor at compact
  phases $0,\pi/2$. Averaging those phases and extracting its
  $\epsilon^2$ coefficient using 24 equally spaced Cauchy samples
  at $|\epsilon|=0.02$ recovered all three coefficients with
  maximum discrepancy $1.4\times10^{-13}$ across
  $r=1.00001,1.8,3.7+0.9i$. Independent direct quadrature of the
  same manufactured forcing gave agreement within $1.6\times10^{-13}$.
  Python 3.14.8, SciPy 1.18.0, NumPy 2.4.6.

## Actual GL profile smoke test

With the companion overlap_gl.GLProfile driver from the same continuation,
construct GLProfile(2.5, exp(1j*angle), length, left_delta, tol), then
MeanGeometry(gl, 2.5, exp(1j*angle), length, tol). The joint refinements
(angle, length, left_delta, tol) were $(0,48,10^{-5},10^{-9})$,
$(0.55,48,10^{-6},10^{-10})$, and $(0.78,64,10^{-8},10^{-11})$.
Across fixed radii $1.001,1.640388203,2,2.5$, the maximum absolute
change between consecutive runs was $4.79\times10^{-8}$ then
$4.39\times10^{-10}$. On the finest run the maximum imaginary part
at those real radii was $1.16\times10^{-12}$, the GL match discrepancy
was $1.03\times10^{-12}$, and the EF endpoint tail proxy was
$(9.38\times10^{-31},1.76\times10^{-32})$. The real parts of the
static-time amplitudes on that run were

| $r$ | $\mu$ | $\beta$ |
| --- | ---: | ---: |
| 1.001 | 3.50299408 | 37.0883258 |
| 1.640388203 | 0.826780230 | 3.41494716 |
| 2 | 0.217349099 | 0.951843556 |
| 2.5 | 0.0336697632 | 0.211717425 |

At the finest inner radius $r=1+10^{-8}$ the factored EF sources
$(T^v{}_v,T_{rr})$ were approximately $(22.5752752,-1022.29646)$.
The test changes several parameters together, so these differences
are joint empirical diagnostics rather than componentwise error
bounds. No overlap integral or physical residue was evaluated.
