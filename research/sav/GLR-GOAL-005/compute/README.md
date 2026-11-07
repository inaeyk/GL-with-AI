# L10 eigenvalue and early finite-slice radion checks

Input baseline `96339836a1281c219917f900224e7c98cf62f3f8`. The master,
reconstruction and source conventions come from the audited GOAL-001
equations at [7110bc2](https://github.com/inaeyk/GL-with-AI/commit/7110bc2cb3c0496dd62dc97d37b34d11ec780a17).
The Z prescription comes from GOAL-002 at
[6eb617e](https://github.com/inaeyk/GL-with-AI/commit/6eb617e4d22b1ca8e8641dcf1f80ca5d9c64e681).
See the inherited [linear mode](../../physics/gl_linear_mode.md),
[quadratic source](../../physics/quadratic_radion_source.md) and
[finite data](../../physics/goal002_finite_data_energy.md) notes.

## Results and scope

Use $r_0=1$, $k=2\pi/10$, $x=r+\log(r-1)$ and
$H_{\theta\theta}^U/(2r^2)=1$ at $r=2$. The constrained
quadratic Z scalar problem has zero scalar and normal derivative on
$v=r$ and the complete on-shell GL source. On $[-10,-8]$ we obtain

$$
0.06328117<\Omega r_0<0.06328121,\qquad
6.5671\times10^{-5}<N_2(u)<6.7662\times10^{-4},\qquad
3.5316\times10^{-7}<\mathcal K_{0,Z}<4.0171\times10^{-7}.
$$

The enclosures use Arb balls, differential comparison and interval
Riemann sums. Identifying this coefficient with that of an exact
nonlinear family requires constraint completion, selection of its Z
scalar jet and a differentiable news map. A bounded fourth-order news
correction and a quantitative nonlinear amplitude interval are separate
gates.

## Validated GL root and exterior input

Write $m$ for the master satisfying $m_{xx}=Am$, where
$A=V_{\rm GL}+\Omega^2$, and $p=m_x/m$. An independent empirical
route diagonalizes the centered second-difference Schrödinger matrix
with Dirichlet endpoints. It gives $\Omega=0.06328727024,\ 0.06328299584,\
0.06328164713$ for $(x_-,x_+,h)=(-90,30,.05),(-120,45,.025),
(-150,60,.0125)$, with nodeless sampled eigenvectors and matrix
residuals at most $2.04\times10^{-12}$. The matrix residual does
not bound continuum or truncation error.

For the interval route, the ingoing branch has
$p=\Omega+p_1\delta+O(\delta^2)$, $\delta=r-1$.
For $0<\delta\le2^{-34}$, the potential gives
$|V_{\rm GL}|<11.5\delta$ and the recurrence gives $|p_1|<12$.
The functions $\Omega\pm12\delta$ have ordered signs of
the defect $p_x-A+p^2$ and give barriers up to the left endpoint.
For $r\ge60$, the potential and $0.06<\Omega<0.07$ give
$0.6^2<A<0.65^2$. The positive decaying branch satisfies
$-.65<p<-.60$ there by comparison with the constant barriers,
using its limit $-\sqrt{k^2+\Omega^2}$.

The candidate integration proposes values and slopes at radial nodes.
Each cell uses the exact cubic Hermite polynomial $P$ defined by
those values. For $D=1+k^2r^3$ and
$Q=(k^2r^3+6r-11)D^2+18(r-1)-6(4r-5)D$, the defect is

$$
\mathcal R=P_x-A+P^2
=\frac{(r-1)(r^3D^2P_r-Q)+r^4D^2(P^2-\Omega^2)}{r^4D^2}.
$$

Evaluate the numerator as a polynomial in the local cell coordinate
in a ball containing $[0,1]$, then bound the denominator below at
the lower radius. With $e=p-P$, $e_x=-2Pe-e^2-\mathcal R$.
The left propagation uses growth at most
$\max(0,-2\inf P)+10^{-3}$ under $|e|<10^{-3}$.
The right propagation in decreasing $x$ uses damping at least
$0.83$ under $P<-.44$ and $|e|<.05$.
Each cell verifies the bootstrap using the closed constant-coefficient
scalar comparison over its tortoise length. The first 1536-cell left
grid gave an error near $8\times10^{-7}$ and did not resolve the
lower trial sign. The retained 12288 left and 8192 right cells
give errors at most $4.53\times10^{-9}$ and $2.29\times10^{-11}$
at the match for the two fixed trial parameters.

At those trial parameters, $p_H(2)-p_I(2)$ lies in
$[-9.0510,-8.1407]\times10^{-8}$ and
$[9.0686,9.9788]\times10^{-8}$. For two $p$'s the
comparison equation for their difference has source
$\Omega_2^2-\Omega_1^2$. The ingoing $p$ increases
with $\Omega$, and the decaying $p$ decreases, by their
endpoint asymptotics and comparison in the respective integration
directions. The bounded endpoint solutions enclose intermediate
solutions through $r=2$, excluding intermediate poles. Continuous
dependence of the Frobenius and decaying branches gives a nodeless
matching root, the ground state of the self-adjoint master.

The scalar certificate reruns the right defect calculation using the
whole enclosed $\Omega$ interval. Reconstructing
$H_{\theta\theta}/(r^2m)$ at $r=2$ fixes $|m(2)|$, enclosed
by $[12.96383979,12.96384013]$. The interval range of $p$
on each cell times its tortoise length encloses the corresponding
logarithmic amplitude increment. This encloses $m,m_x$ and the
quadratic scalar source, which depends only on these master variables
after on-shell cancellation. The regular-metric derivative norm
needed for the full two-harmonic source remains open.

### Linear horizon connection

The same two validated left branches enclose $p$ throughout the
matching interval in $\Omega$, with no additional candidate solve.
For the horizon coefficient of $y=(D/r)m$,

$$
 a_h=\lim_{\delta\downarrow0}\frac{y}{\delta^\Omega}
 =(1+k^2)m(2)\exp\left[-\Omega-\int_{-\infty}^{2}
 (p-\Omega)\,dx\right].
$$

The omitted tail at $\delta_L=2^{-34}$ is bounded by
$12(\delta_L+\delta_L^2/2)$ from the horizon barrier.
Enclosing the finite integral by the two validated left-cell ranges
gives $-38.5266<a_h<-38.4683$. Applying the inherited
[linear apparent-horizon formula](../../physics/goal003_data_nonlinear.md)
on the $\tau=v-r$ slice gives
$6.8950<|a_h|/[4(1+k^2)]<6.9055$ for the coefficient of
$|A(\tau)|$ in the areal bulge/neck contrast. This is a linear
slice-specific result. The finite-deformation MOTS remainder remains open.

## Exact finite-slice characteristic response

Set $s=2\Omega$, $v=t+x$, $u=t-x$,
$B=\sqrt3 k^2rf H_{\mu\nu}H^{\mu\nu}/16$, and $V=f/r^3$.
Then $\psi=r\varphi_2$ satisfies
$4\psi_{uv}=e^{s(u+v)/2}B-V\psi$. The ray at $u$ starts
at $b=b(u)$ given by $U(b)=-b-2\log(b-1)=u$ and $v=b$.
Integrating with both traces zero gives $\psi=P_0-T\psi$, or

$$
\psi(b,v)=\frac14\int_b^v dv'\int_b^{v'}db'\,
\frac{b'+1}{b'-1}
[e^{s(U(b')+v')/2}B(r)-V(r)\psi(b',v')].
$$

For the wedge $u\le c$ above the slice, changing variables to
$(t,x)$ and integrating from $t=-\log(r-1)$ to $c+x$
gives

$$
\|T\|_\infty\le\kappa(b(c))
=\frac12\int_{b(c)}^\infty\frac{c+r+2\log(r-1)}{r^3}dr
=\frac14\left[2\log\frac{b(c)}{b(c)-1}-\frac1{b(c)}\right],
\qquad
\frac14\int_{b(u)}^\infty V\,dv=\frac1{4b(u)^2}.
$$

At $c=-8$, $b=5.1525532351$ and $\kappa=0.05936485704$.
Contraction gives the bounded response. Differentiating at fixed
$v$ and taking $v\to\infty$ gives
$N_2=\tfrac14\int_b^\infty[e^{s(u+v)/2}B-V\psi]\,dv$.
The source and potential bounds justify uniform convergence on the
window. In radial variables,

$$
N_{\rm free}(u)=\frac12\int_b^\infty e^{s(u+x)}\frac{B}{f}\,dr,\qquad
\|P_0\|_{\infty,u\le c}\le\frac1{2s}\int_{b(c)}^\infty
[e^{s(c+x)}-(r-1)^{-s}]\frac{|B|}{f}\,dr.
$$

At any cut in a radius cell $[b_0,b_1]$, enclose the signed
free news using full source cells and the first partial cell,
whose width lies between zero and $b_1-b_0$.
The absolute news correction is at most
$\|P_0\|_{\infty,u\le U(b_0)}/[4b_0^2(1-\kappa(b_0))]$.
Integrating the enclosed $2N_2^2$ against the exact lengths of
the intersections of 590 retarded-time cells with $[-10,-8]$
encloses the full backscattered coefficient. The interval
also resolves positive $B$ on the covered radial cells up to
$60$. The tail bound allows either sign.

For $r\ge60$, $|m(x)|\le |m(60)|e^{-0.6(x-x_{60})}$.
The tensor reconstruction gives $|y|,|y_r|\le r^2|m|$,
$|H_{tt}/f|\le3r^2|m|$ and
$|H_{tr}|,|H_{\theta\theta}/r^2|\le2r^2|m|$ from
$|p|\le.65$, $f\ge59/60$ and $D\ge.39r^3$.
It suffices to use $|B|\le100r^5|m|^2$.
Using $r^5\le60^5e^{5(x-x_{60})/60}$ gives a direct news
tail less than $1.223\times10^{-18}$. This tail is propagated
into the potential correction using the absolute free-field bound.

## Independent floating waveform

Aligned characteristic trapezoids and a compactified tail
$p_v=-1/v$ resolve the response for the saved empirical GL profile,
with a source taper over $60<r<65$. The three grids with 225, 449 and
897 radius rows give $\mathcal K_{0,Z}=3.7343197954,\ 3.7329225443,\
3.7325750836$ in units of $10^{-7}$. The finest free and first
corrected values are $3.7743932041$ and $3.7324821159$.
The independent direct radial free quadrature agrees with the
fine mesh within relative $2.63\times10^{-5}$ at the three
check cuts. Differentiating the interpolated fine waveform
agrees with its separately integrated news to relative
$2.37\times10^{-6}$. The manufactured field $h(b)(1-b/v)^2$
has both traces zero and known news $-h'(b)(b-1)/(b+1)$.
Its maximum news error is $2.56\times10^{-5}$ on the coarse
grid and $5.73\times10^{-3}$ under a flipped potential sign.

The fine [CSV waveform and news](early_null_waveform.csv) gives
$N_2=(6.7414043\times10^{-5},2.1118822\times10^{-4},
6.5153980\times10^{-4})$ at $u=-10,-9,-8$.
For exact kernel integrals, 12 updates leave a field error at most
$\kappa^{13}\|P_0\|_\infty/(1-\kappa)$, with a further
factor $1/(4b(-8)^2)$ for news. The corresponding floating-size
news estimate $6.59\times10^{-22}$ excludes mesh and saved-profile
errors. The exact interval above supplies the continuum enclosure.

## Reproduction

Run from `research/sav` with Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0
and python-flint 0.9.0. Arb precision is 112 bits.

~~~sh
python3 GLR-GOAL-005/compute/gl_eigenvalue.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-005/compute/early_interval.py
python3 GLR-GOAL-005/compute/early_null.py
~~~

The scripts save the [root](gl_eigenvalue_results.json),
[interval](early_interval_results.json) and
[floating](early_null_results.json) evidence.
