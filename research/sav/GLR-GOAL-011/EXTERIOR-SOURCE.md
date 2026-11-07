# Physical affine matching and mean fields

The fixed canonical packet has $R/r_0=13/8$ and width $0.12$.
Set $r_0=1$, $x=r+\log(r-1)$, $r_m=2.5$ and
$r=r_m+e^{i\pi/4}\rho$ on the outgoing leg.
Use the physical GL rate interval and the returning pole box from
[Spectral certificate](SPECTRAL-CERTIFICATE.md), and the physical launch
normalizations from [Horizon source](HORIZON-SOURCE.md).
The bounds in this note are for the actual lower-order fields over
those intervals. The two Fourier half factors in the returning source
continue to be governed by [Canonical pairing](PAIRING.md).

## Certified profiles and mean

The Riccati validators return whole-cell error bounds as well as
endpoint values. For a scalar logarithmic derivative
$u_x/u=\ell_0+\alpha f/r+p$, integrate the first two terms exactly
and integrate the central Hermite candidate for $p$ against $dx$.
On a cell $r=1+a+b\xi$, $dx/d\xi=b(1+(a+b\xi)^{-1})$.
The degree-eight geometric expansion of $(a+b\xi)^{-1}$ and
the certified error of $p$ bound the primitive. Exponentiation
gives the GL master, the outgoing shifted RW profile and the
adjoint profile on the same directed grid. The adjoint is
normalized by the continued ingoing solution at the match and
equals the physical test at the pole.

The absolute bounds on the cutoff $\rho=64$ are
$|u|<3.346\times10^{-13}$,
$|\widehat\Psi_0|<6.881\times10^{-11}$ and
$|h|<1.262\times10^{-11}$. C's weighted Riccati tail validates the
stable branch and the actual forcing beyond that cutoff.
On the real leg the regular GL amplitude $N=e^{-\Omega x}u$
uses the horizon Taylor evaluation and a separate outward
Riccati transport. Its matching value and derivative overlap
the scalar launch certificate.

In the EF mean quadrature the common source factor is $N^2$.
The remaining coefficients are enclosed by the analytic mean
value formula on Acb product boxes. The far-ray majorants on the
GL EF jets, quadratic Ricci expressions and exponential decay give
$9.054\times10^{-16}$ for either component's omitted tail.
Interval integration from infinity to $r_m$ and then down the
real leg gives
\[
  7.163<\mu_v(1+1/128)<7.468,\qquad
  74.342<\beta_v(1+1/128)<75.567 .
\]
The endpoints above conservatively round the stored real bounds. The underlying Acb quadrature also contains the small
imaginary errors of numerical contour integration. Deforming the
mean integral through the regular decaying wedge gives the real
physical mean field. The compact mean integration below
$r=1+1/16$ uses direct horizon Taylor derivatives with Cauchy
bounds.
[B's GL jet reconstruction](investigators/B-EXTERIOR-GL-JETS.md) ·
[C's far-tail bounds](investigators/C-EXTERIOR-TAIL.md) ·
[E's exterior and mean audit](investigators/E-EXTERIOR-INTERVALS.md)

## Inhomogeneous mixed match

For the normalized two-field system put
$L=J-PU$ where $P$ is the ingoing or outgoing logarithmic
derivative at $s_1=s-\Omega$. Then
\[
 L_x=-PL+G-PF,\qquad (P_L-P_R)U_*=L_R-L_L .
\]
On the outgoing leg the first insertion is proportional to
$u\widehat\Psi_0$. Factoring this product and enclosing the
remaining rational and square-root coefficients avoids the raw
interval cancellation in the source. C's backward contracting
tail bound gives $\|L_R(64)\|_2<9.788\times10^{-25}$.
This outgoing datum is the affine part. The homogeneous
outgoing data can have nonzero $U$ and $J$.

On the compact leg the left integration starts at
$r=1.0313414502474\ldots$. There the driven horizon Taylor
column supplies $U$ and $J$. Adding any regular homogeneous
column cancels out of $J-P_LU$, so the affine start is fixed.
The left shifted RW profile is normalized on the same mesh,
and the right shifted RW profile is Taylor-transported down
to the packet from $g_\sigma(r_m)=1$. Compact launch moments
use sixteen interval slices per intersecting cell. The portion
of a slice in a left or right partial integral is enclosed by
clamping its relative overlap length to $[0,1]$. The weights
include $e^{-\sigma T}(1-T')$ and the exact fixed-canonical
packet. Within the packet the reconstruction adds the explicit
$e^{-\sigma T}a$ in $\widehat{\Theta\Psi_x}$ and subtracts it
in $\widehat{\Theta\Psi_t}$. The reconstruction uses the
field and derivative directly throughout the packet. Both complete moments overlap the separately certified
scalar launch amplitudes.

For either direction a midpoint solve proposes a Hermite
candidate. Each directed subcell encloses its exact affine
residual and a Hermitian Gershgorin bound on the logarithmic
norm. The scalar comparison equation advances the Euclidean
error radius and bounds it over the subcell. This uses the
actual forcing and slopes on each subcell, with floating
solver tolerances entering only the construction of the
candidate. In units $r_0=1$ the error radii at $r_m$ are below
$0.06768$ on the left and $0.007530$ on the right. The
matched determinant excludes zero. With $D=P_L-P_R$, take
a numerical inverse candidate $A$, affine centers $c_L,c_R$
and match center $U_c$. The Euclidean error is bounded by
\[
 \frac{\|A(c_R-c_L-DU_c)\|_2+
             \|A\|_F(\epsilon_R+\epsilon_L)}
      {1-\|I-AD\|_F}.
\]
The denominator is certified positive. This retains the
Euclidean affine error data through the inverse. With two
subdivisions per directed cell the norm radius is below $0.46235$.
Eight subdivisions give left and right affine radii below $0.051354$
and $0.002372$, respectively, and a matched-state norm radius below
$0.33257$ about
$(-0.187461-0.416812i,-0.058891-0.494655i)$, with the center
rounded here. The previous independently assembled numerical mixed
solution differs from the unrounded center by $8.34\times10^{-6}$.
This comparison probes the center. The enclosure uses the residual
and interval inputs. The independent review checked
the packet, horizon initialization and preconditioning.
[E's ingoing and matching audit](investigators/E-INGOING-MATCH.md)

## Reproduction and remaining projection

From the research directory with the Python/Flint versions
listed in [Error budget](ERROR-BUDGET.md), run
```sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
python3 GLR-GOAL-011/checks/spectral_certificate.py
python3 GLR-GOAL-011/checks/physical_overlap/exterior_source.py
python3 GLR-GOAL-011/checks/physical_overlap/mean_certificate.py
python3 GLR-GOAL-011/checks/physical_overlap/ingoing_source.py
python3 GLR-GOAL-011/checks/physical_overlap/ingoing_source.py --subdivisions 8
```
[Recorded bounds](checks/exterior_source.txt) ·
[Wide-enclosure controls](checks/exterior_source_failures.txt)

The matching and mean boxes are wider than the provisional
inputs used for the conditional collar test. The remaining
projection requires controlled transport of $U$ from the
match, conversion to the horizon amplitudes, and a complete
signed exterior integral and mixed returning tail. Refining subcells
alone reduced the real-leg affine error modestly. The compact
forcing intervals retain the width of the two scalar launch moments
and the products in $G-PF$. Evaluating tighter scalar moments and
enclosing the whole affine residual about each subcell midpoint
would address those dependencies before transporting the match.
A linear adjoint of the remaining mixed projection can also retain
the correlations between its left and right uses of $U_*$.
The residue
value in [Physical residue](PHYSICAL-RESIDUE.md) remains
empirical.
