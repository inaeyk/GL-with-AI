# GOAL-011 horizon and launch interval code audit E

A bounded, non-blind comparison of horizon_series.py and
certified_launch.py with the independent EF equations in
glr011-horizon-E.md. Read the matching driver and mean source
dependencies to identify which parameters are enclosed.
No repository files were edited.

## EF coefficients and exact removable powers

The code orders the unknowns as \((U,z,a,j)\), a permutation
of \((U,j,a,z)\) in the independent derivation. In this order
all the entries of \(t_h w'=B(t_h)w+b(t_h)\) agree, including
\(B_{Ua}=-r^3(\lambda+k^2r^2)\),
\(B_{az} = 0\),
\(B_{aU}=-s_1^2/(\lambda r)-t_h/r^4\), and the \(Y^r\)
term in the compact-potential row. Here
\(U=-p^{vr}-X^{vr}\). The initialization implements
\(G(0)=K(0)=0\), with the inhomogeneous \(Y^r_0\) included.
The conversion to \(Y,Z\) and the derivative-minus-source
variables follows from \(Y=U/\sqrt D\),
\(Z=\sqrt W A_z\) and \(p^{xz}=p^{rz}=j\) as a density.
The extra \(-s_1Y,-s_1Z\) in the factored derivative slots
is consistent with the numerical solver's removal of
\(e^{s_1x}\).

For the mixed radial potential the numerator divided by \(t_h\)
is \(-rG\). Its exact zero follows from the algebraic horizon
constraint, not a ball containing zero. The GL EF \(vr,rr\)
division is also exact. A separate symbolic substitution in
the reconstructed GL components gave zero for both constant
numerator coefficients. For the \(rr\) numerator its linear
coefficient is
\[
 -\frac{(a^2-4a+1)N_0-(a+1)(1+2\Omega)N_1}{a(a+1)},
 \qquad a=k^2 .
\]
It vanishes by
\(N_1=(a-5+6/(1+a))N_0/(1+2\Omega)\).
Thus the 1- and 2-power shifts remove true zeros.
The contains(0) assertions are sanity checks, while the
analytic identities authorize the divisions. The same
reasoning uses \(B_0^2=-\nu B_0\) and exact leading
compatibility for fuchsian. No exact cancellation rests
solely on an interval containing zero.

## Weighted tails and split boundary

The Taylor inverse is \(I/n+B_0/[n(n+\nu)]\).
The code bounds its norm for \(n>N\) by the largest displayed
positive denominator estimate at \(n=N+1\).
Its residual tail includes precisely the matrix coefficients
of order \(>N-j\) multiplying \(w_j\), and the entire force
tail. The Cauchy radii for the matrix and force supply their
weighted coefficient remainders. The tail-to-tail operator
has norm at most the recorded contraction. The resulting
\(\ell^1_\rho\) error scales by \((r/\rho)^{N+1}\) on a
smaller disk. Derivative bounds use a Cauchy estimate for
the analytic error on an intermediate circle. Bounds after
removable division use the boundary circle and maximum
modulus. The scalar horizon equations for GL and RW use
state \((N,N_r)\), so their indicial parameter in the
first-order form is \(1+2s\) or \(1+2\Omega\), as coded.

The local collar integral implements
\((2/\lambda)\int_0^c t_h^{2s}
(1+t_h)n_s e^{2s(1+t_h)}
[Q_r-sV-\lambda C/r^4]dt_h\)
and subtracts
\((2/\lambda)c^{2s}(1+c)n_s e^{2s(1+c)}Q(c)\).
Both signs agree with the independent EF strong/weak pairing.
For \(Q_r\) the disk 0.03 bounds the function and Cauchy
controls the derivative on radius 0.018. The coefficient
tail and termwise denominator estimate for the integral are
consistent with \(\Re(1+2s)>0\). The boundary evaluation has
its own Cauchy tail. The cap may be \(1/128\) or \(3/256\),
both inside the smaller disk.

## Launch bounds and exact scope

The compact transport recurrence is ordinary Taylor flow.
The analytic sup bound
\(\|w(0)\|_\infty\exp(R\sup\|A\|_\infty)\)
follows along the radial line inside each complex disk, then
Cauchy bounds the discarded coefficients. The interval
rectangles enclose the relevant disks. The GL scale imposes
\(H_{\theta\theta}(2)=8\); \(x(2)=2\), so the time factoring
at that point is precisely \(e^{2\Omega}\). This validates
the horizon normalization and compact transport to \(r=2.5\)
conditional on the pinned physical GL eigenvalue bracket.
It does not independently reprove GL eigenvalue existence.

The derivative-square bump norm integrates the symmetric
half interval by interval rectangles. The omitted bound at
\(y\le 1/128\) uses
\(e^8(1-a)^{-4}y^{-4}e^{-2/y}\), increasing there. The factor
2 for symmetry and the reciprocal square root are correct.
For the launch quadrature, \(dx=dr/f\), \(T'=-1/r\), and the
radial mesh is inward, giving positive weight
\(-\Delta r(1+1/r)/f\). A support extension to \(r=1.595\)
contains the entire bump. The interval bump handles cells
touching flat endpoints by its true one-sided range.
The horizon amplitude is \(-J_g/W\), and the outer
amplitude is \(-J_h/W\) multiplying the explicitly
normalized \(g(2.5)=1\). The right Jost slope input is
the previously validated one. This confirms both compact
launch moment enclosures and the stated normalization.

Latest replay used Python 3.14.8, flint 0.9.0, bytecode off,
OPENBLAS_NUM_THREADS=1, and pipefail. Both
python3 certified_launch.py and
python3 validated_horizon.py --certified-lower
matched their stored outputs byte for byte with diff -u.
The no-flag horizon run matched the numerical prefix but
omits the certified-lower appendix, as expected. The extra
SymPy coefficient check above was independent of the
shared implementation. The final launch output includes
GL scale about \(-25.908\), and launch amplitudes near
\(-0.0071+0.0067i\) and \(-0.0065+0.0109i\), with the
printed interval radii.

The driver's mixed matching amplitude boxes with half-width
0.0002 and EF mean value boxes with half-width 0.002 are
assumed around empirical values. The certified-lower
collar output is conditional on those inclusions. Neither
the small local Cauchy tails nor the small numerical
matching residual validates those boxes or the exterior
quadratures. The fit-free residue \(0.13523245-0.37062029i\)
uses midpoint arithmetic and empirical global data. It is
not an interval enclosure of the physical residue.
No sign, weighting, or removable-division error was found
in the audited local and compact launch calculations.
