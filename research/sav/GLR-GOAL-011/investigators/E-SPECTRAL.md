# GOAL-011 spectral gate, adverse review E

Non-blind code and argument audit of the final 5e-8-radius version of
glr011-spectral-cert.py (SHA-256
f74f7ff53114c492c331df6fc20fd52dffd65a3decd6591360e30222ef9b0f49)
against HIERARCHY (5) and C's massive pencil. This was one analytic
audit and an exact replay of the shared code, with a separate branch
negative control. It was not an independent numerical spectrum search.

## Operator and sheet

Write the two homogeneous equations as
\(U_{xx}=B(s,x)U\), \(B=s^2+\operatorname{diag}(V_Y,V_Z)-sC\sigma_1\)
for \(n=+1\). Set \(U_xU^{-1}=\ell I+P\); the residual system is
\(P_x=Q-2\ell P-P^2\), \(Q=B-(\ell^2+\ell_x)I\).
For \(d=4+k^2r^2\), the logarithmic derivatives in the two diagonal
potentials are \(a=(r-1)(d+k^2r^2)/(r^2d)\) and
\(b=(r-1)(2d-k^2r^2)/(r^2d)\). Differentiating with \(f\,d/dr\)
gives the script's ax and bx after multiplying by \(r^4d^2\).
The massive off diagonal is \(-4iskf/d\) and matches the script.
The right subtraction uses \(\ell=-\kappa+\alpha f/r\),
\(\alpha=k^2/(2\kappa)\). The cancellation
\(-k^2/r+2\kappa\alpha f/r=-k^2/r^2\) yields the script's
right base, including the negative term
\(-\alpha(r-1)(2-r)\) in its numerator. The scalar residual
numerator is \((r-1)(6r-3)\), as required by RW.

The physical massive root has \(\kappa=\sqrt{s_1^2+k^2}\) with
\(\Re\kappa>0\) for \(\Re s_1>0\). At fixed \(\Im s_1<-k\) it
continues across the negative-real argument of that square root
from below to above. Thus the negative of the principal square
root is the correct local branch when \(\Re s_1<0\). The ray
\(r=2.5+e^{i\pi/4}\rho\) stays in the analytic exterior region
for \(r,r-1,d,\log(r-1)\). Near the target
\(\Re(\kappa e^{i\pi/4})=0.086992\) within \(3.81\,10^{-7}\)
on the script's interval. The flipped branch fails the strict
attenuation test. C's far-contour construction gives the
connection to the outgoing branch on an overlapping right
half-plane. The matching certificate itself is local.

## Tail and compact interval error

At the horizon with \(t=r-1\), \(dx/d\log t=1+t\).
The homogeneous ingoing factor is \(e^{s x}\), so
\(Q=V\) or \(\operatorname{diag}V-sC\sigma_1\).
The rational formula directly gives \(\|Q\|_F<100t\) on
\(0<t<10^{-8}\), using \(|s|<1\): \(f\le t\),
\(|a|,|b|<2t\), \(|da/dr|,|db/dr|<10\), and
\(|sC|<t\). The scalar estimate is smaller.
The weighted Volterra kernel has norm bounded by
\((1+t_0)/(1+2\Re s(1+t_0))\) on \(\|P/t\|_\infty\).
The asserted invariant radius and Lipschitz inequality therefore
give the ingoing Riccati solution and the claimed integral bound.

For \(\rho\ge R=144\), \(|r|,|r-1|\ge\rho\),
\(|dx/d\rho|\le1+1/(R-1)\).
The real lower bound for \(-\ell\,dx/d\rho\) in the code follows
from \(dx/dr=1+1/(r-1)\). The terms in the right residual obey the
stated bound \(q(R/\rho)^2\). In the derivative part write
\(z=k^2r^2/d\) and \(m=(1-4/(k^2R^2))^{-1}\).
Then \(|z|\le m\), and
\(|dz/dr|\le8m^2/(k^2|r|^3)\le2m^2/|r|\).
This supplies the potentially delicate derivative term in the
tail bound. The backward Volterra kernel therefore has norm at
most the code's kernel in the weighted \(P=O((R/\rho)^2)\)
space. A ball of radius \(2\,\mathrm{kernel}\,q\) is invariant
with contraction constant \(2\,\mathrm{kernel}\,\mathrm{radius}<1\).
The integrated tail bound follows from the same weight.
Integrating \(U_x=(\ell I+P)U\) recovers invertible branch matrices
at all finite radii by Liouville's formula, with the normalized
asymptotic behavior. Uniform contraction and analytic ODE
dependence give holomorphic matching data on the indicated
parameter sets.

The candidate ODE samples are untrusted proposal data. Exact
binary values define a piecewise Hermite polynomial, and Acb
encloses the rational defect including the signed step
\(X=dx/d\theta\). If \(E\) is the Riccati error, the linear
matrix terms are left and right multiplication by \(-X p\),
plus \(-2X\ell E\). In Frobenius norm their logarithmic-norm
bound is \(\Re(-2X\ell)+2\mu_2(-Xp)\). Right multiplication
has the same numerical abscissa by transposition of the
Hermitian part. Gershgorin gives the script's bound. The
quadratic term is bounded by \(|X|\|E\|_F^2\). On the proposed
bootstrap ball the scalar differential majorant is affine
with constant coefficients. Its solution is monotone, so the
start and end tests bound the entire cell. This works also
for a complex inward step. The full-cell integrand
enclosure, plus the integrated error \(\sup|X|\) times the
bootstrap radius, correctly encloses the normalization
integrals. For portability it would be useful to assert that
the floating mesh endpoints are the match radius and start
within the analytic tail bound. Both conditions hold in the
executed environment; the left match endpoint is exactly
the binary value 1.5 in \(r-1\).

## Count, normalization, and local noncoincidence

The actual scalar Jost slopes are holomorphic on the Cauchy
disk since the validated Riccati graphs have no finite
blow-up on its enclosing square. If \(M\) bounds \(|D|\) on
radius \(R\), the centered difference at step \(h\) differs
from \(D'(s_c)\) by at most
\(M h^2/[R^3(1-(h/R)^2)]\).
The linear comparison bound on radius \(r\) is then the
center residual, slope uncertainty times \(r\), and the
Cauchy tail \(M(r/R)^2/(1-r/R)\). Rouché gives exactly
one zero with multiplicity one. For \(D=h'/h-g'/g\) and
\(W=hg'-h'g\), the normalization is
\(W'(s_q)=-h(s_q)g(s_q)D'(s_q)\).
The baseline factors \(e^{s x}\) and \(e^{-s x}\) cancel
in \(hg\). The horizon correction is integrated from
the left endpoint, and the infinity correction from
the right endpoint inward. Their signs are consistent
with the common specified \(x\) and unit Jost factors.

Run from the shared working directory, with bytecode disabled:
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/spectral_certificate.py
piped through diff -u GLR-GOAL-011/checks/spectral_certificate.txt -
with pipefail gave exit 0 and no differences. Python 3.14.8,
python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0. The Rouché
bound is \(1.310\,10^{-8}<1.697\,10^{-7}\) on radius
\(5\,10^{-8}\). Conservative \(|W'|\) bounds are 3.64 and
10.57. Over the exact pole enclosure and pinned GL interval
the massless shift has \(\Im D>0.28\) and the massive shift
has \(\Im\det D>0.40\). Matching a graph difference is
equivalent to matching the underlying full subspaces.
These are exclusions for the exact shifts of this local
pole, including the opposite KK sign by the constant
polarization conjugacy. They do not exclude poles along
a meromorphic continuation route, or globally order the
RW spectrum by damping. Describe the root as the isolated
simple pole in the disk around the literature fundamental.
No physical source overlap or pole dominance follows from
this homogeneous certification.
