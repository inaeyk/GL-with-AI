# GOAL-011 high-order launch certificate

The exact preparation is inherited from
`e385f928cd25e508128a2ae01f40fde6f71ff300`,
GLR-GOAL-011/FINITE-DATA.md and
checks/physical_overlap/certified_launch.py. Use $r_0=1$, $R=13/8$,
$\delta=3/25$, $u_\Sigma(r)=-r-2\log(r-1)$, $T(r)=-\log(r-1)$,
$\sigma=s-2\Omega$ on the full inherited pole box, and the same
ingoing $h$ and outgoing $g$ with $g(2.5)=1$. Define
$B(y)=\exp(4-1/[y(1-y)])$ on $(0,1)$, extended by zero, and
$y(r)=(u_\Sigma(r)-u_\Sigma(R))/\delta$. Write $N$ for the
derivative-square normalization of $B$ so the physical packet is
$\sqrt\delta\,N B(y)$. The two full launch moments are
$$
 J_F = N\sqrt{\delta}
 \int_{1.595}^{R} e^{-\sigma T(r)}\frac{r+1}{r-1}
 B(y(r))\bigl(F_x(r)+\sigma F(r)\bigr)\,dr,\qquad F=h,g .
$$
The coefficient formulas are $C_-=-J_g/W$, $C_+=-J_h/W$ and
$W=h(2.5)g_x(2.5)-h_x(2.5)$. The GL scale, frequency interval,
outgoing branch, and validated radial field transport are reused
from the pin.

## Enclosure

For a real cell with center $c$ and halfwidth $d$, expand the
analytic density as $\sum_{n\ge0}a_n(r-c)^n$. If its modulus is
at most $M$ on the disk of radius $3d$, the centered integral
retains the even terms through degree 22. The remaining even
terms starting at $n=24$ are bounded in modulus by
$$
 \frac{2d\,M\,3^{-24}}{25(1-3^{-2})}.
$$
Acb Taylor arithmetic encloses the retained coefficients. On
a disk mapped to $z=x+iy$ with $0<x_-\le x\le x_+<1$ and
$|y|\le Y$, bound the complex bump via
$$
 \operatorname{Re}\left(\frac1z+\frac1{1-z}\right)
 \ge \frac{x_-}{x_+^2+Y^2}
       +\frac{1-x_+}{(1-x_-)^2+Y^2}.
$$
For the derivative factor use
$|1/z+1/(1-z)|\le 1/x_-+1/(1-x_+)$. Direct complex interval
evaluation of the exponential norm on the initial broad boxes
failed near the ends because of reciprocal wrapping. The
real-part bound above makes the same Cauchy disks usable.

The bump normalization uses 128 cells on $[1/128,1/2]$ and
symmetry. For $a=1/128$, the omitted derivative-square integral
on $[0,a]$ is bounded by $a^{-3}(1-a)^{-4}e^{8-2/a}$ because
$y^{-4}e^{-2/y}$ is increasing there. The radial integral uses
four inherited Taylor-flow patches with 64 cells on each
intersection with $[1.59714,1.62478]$. The polynomial density
is integrated by the same Cauchy rule. The difference between
the polynomial and the true $(F,F_x)$ is added using the
inherited field Taylor tails at the far cell endpoint. The
remaining support in $[1.595,R]$ has either $y$ or $1-y$
below $1/120$, so $B\le e^{4-120}$ there. A real-interval
operator norm and Gronwall from $R$ bound $(F,F_x)$ on these
tails. All changes concern enclosure of the same full packet.

For the final quotient let $J_0,W_0$ be the rectangular midpoints,
$\Delta_J,\Delta_W$ the Euclidean radii of the input rectangles,
and $C_0=-J_0/W_0$. The identity
$W(C-C_0)=-(J-J_0)-C_0(W-W_0)$ gives the disk radius
$(\Delta_J+|C_0|\Delta_W)/(|W_0|-\Delta_W)$ with positive lower
denominator. The API returns an Acb rectangle enclosing that
disk, including roundoff at the midpoint.

## Output and replay

| Quantity | Center (rounded) | Certified disk radius bound |
| --- | --- | ---: |
| $N$ | $0.39143076774757091$ | $5.994\times10^{-14}$ |
| $C_-$ | $-0.00713967733351522+0.00673851410315148i$ | $8.06834\times10^{-7}$ |
| $C_+$ | $-0.00653742934645625+0.01090364455104770i$ | $1.17257\times10^{-6}$ |

The returned launch rectangles have Euclidean radii below
$1.14104\times10^{-6}$ and $1.65826\times10^{-6}$. The summed
quadrature and true-field Taylor remainder before multiplication
by $N$ is below $8.248\times10^{-15}$. The omitted flat support
bound before that multiplication is below $4.164\times10^{-52}$.
The inherited Wronskian disk radius is below $2.721\times10^{-5}$.
With a point Wronskian the moment widths alone would contribute
about $7.55\times10^{-8}$ and $2.26\times10^{-7}$, respectively,
to the quotient bounds. The current limiting uncertainty therefore
comes from the inherited Wronskian and field balls.

From the scoped research directory, run
`PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/tight_launch.py`.
The module uses python-flint 0.9.0 with 128-bit precision and
series cap 80. Its script asserts both launch radius targets.

The integration entry is inputs(), which returns the same
certified_launch.Inputs dataclass. Calling inputs(diagnostics={})
also reports the branch slope, full moments, and error terms.
The new enclosures lie inside the older launch balls. This is a
certified lower-input refinement conditional on the inherited
spectral and transport certificates.
