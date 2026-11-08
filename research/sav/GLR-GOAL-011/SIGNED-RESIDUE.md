# Signed enclosure of the physical axial residue

Take the fixed-canonical outgoing packet of [Finite data](FINITE-DATA.md)
at $R/r_0=13/8$, width $delta=3/25$ and unit derivative-square bump
norm. Use $r_0=1$ inside the calculation, $x=r+\log(r-1)$, the real
unit-normalized axial quadrupole, and the fixed retarded origin $u_0=0$.
The coefficient is for the ordered $\epsilon^2\eta$ shear in units of
$r_0$. The simple fundamental pole lies within $5\times10^{-8}/r_0$
of $(-0.177924631-0.747343369i)/r_0$. Both shifted pencils are invertible
throughout the same physical parameter box.
[Physical normalization and pole certificate](SPECTRAL-CERTIFICATE.md)

The complete signed four-subdivision enclosure is
\[
 I\in[0.08101,0.76583]+i[-3.48672,-2.78474],\qquad
 A_{21,q}\in[0.08223,0.18771]+i[-0.42418,-0.31707].
\]
The same run bounds $0.32756<|A_{21,q}|<0.46386$. Its
entire far returning tail is below $1.383\times10^{-26}$.
The two-subdivision run encloses the four-subdivision result,
with residue modulus lower bound $0.26430$. The left and right
weighted defect caps decrease from $0.06131$, $0.11230$ to
$0.01670$, $0.03040$. The independent strong-form projection gives the empirical
$0.13523379-0.37062008i$ and lies within the enclosure.
The ordered physical null trace is nonzero, since its continued
transform has a nonzero simple-pole coefficient.

## Physical signed numerator

Let $s$ be the returning pole and $s_1=s-\Omega$, $\sigma=s-2\Omega$.
The [canonical pairing](PAIRING.md) removes the induced graph terms by
retaining the normal contact and exact electric density. The
[causal transform](NULL-TRANSFORM.md) identifies the continued
source-plus-data transform with the ordered outgoing null trace. Use
the exact $HJ$, $H^2P$ and $B_EP$ densities of
[Physical residue](PHYSICAL-RESIDUE.md). The shift $\sigma$ enters the
baseline and $s_1$ the massive response, while the outer Wronskian
stays at $s$.

For the mixed response on each leg let $P$ be the endpoint-selected
matrix logarithmic derivative, $U=(Y,Z)$ the normalized position,
and $L=J-PU$. Then
\[
 U_x=PU+L+F,\qquad L_x=-PL+G-PF .
\]
The weak $HJ$ integrand in $dx$ is $a^{\mathsf T}U+b^{\mathsf T}L+c$.
For any piecewise smooth, continuous candidate covectors $z,w$, define
\[
 d_z=z_x+P^{\mathsf T}z+a,\qquad
 d_w=w_x-P^{\mathsf T}w+z+b .
\]
The exact bilinear identity is
\[
 a^{\mathsf T}U+b^{\mathsf T}L+c
 =-(z^{\mathsf T}U+w^{\mathsf T}L)_x
   +z^{\mathsf T}F+w^{\mathsf T}(G-PF)+c
   +d_z^{\mathsf T}U+d_w^{\mathsf T}L .
\]
The signed compact contributions thus pair $F$ and $G-PF$ with point
candidates. The error contribution has the local norm bound
$\|d_z\|_2\|U\|_2+\|d_w\|_2\|L\|_2$. This retains the cancellation
between both uses of $U_m$.

## Matching, horizon, and tail

Use $r_m=2.5$ and the far cut $r_f=r_m+64e^{i\pi/4}$. The left cap
has $r_c-1=0.025398534331226477\ldots$. At the left cap the convergent
strong integral and its weak split boundary form an affine
functional $C-z_c^{\mathsf T}U_c$. Compute $z_c$ from the two homogeneous
horizon Taylor columns and $C$ from the driven column. Factor the GL
and baseline amplitudes out before forming these terms. The
Taylor/Cauchy test uses degree 80, a disk of radius $0.034$ for the
integrand and $0.038$ for its differentiated coefficient, inside the
existing analytic domains. The $H^2P$ and $B_EP$ collar contributions
use the same physical cap and the actual interval mean.

Use $z_R(r_f)=0$ and the same numerical point $w_L(r_m)=w_R(r_m)=q$.
Write $D=P_L-P_R$. The exact boundary correction to add to the
signed compact quadratures and their residuals is
\[
 \begin{split}
 C&+(z_L(r_c)-z_c)^{\mathsf T}U_c+w_L(r_c)^{\mathsf T}L_L(r_c)\\
  &+\big[z_R(r_m)-z_L(r_m)+D^{\mathsf T}q\big]^{\mathsf T}U_m
    -w_R(r_f)^{\mathsf T}L_R(r_f).
 \end{split}
\]
The initial covectors are rounded to the exact binary64 points used
by the candidate solver and checked at the first node. Consecutive
Hermite pieces use the same node values. The exact match uses the
same physical ball from [Exterior source](EXTERIOR-SOURCE.md) on both legs. The actual horizon
$L_L$ is the affine Taylor datum, and $L_R(r_f)$ obeys the analytic
outgoing bound. Directed scalar comparisons with Hermitian numerical
abscissae provide entire-cell bounds for $L$ and $U$ on both legs.

Within the compact launch support, interval Riemann integration
retains the launch contact and partial moments. Elsewhere the
source coefficients are analytic in each cell. Derive their jets
directly from the Riccati, scalar amplitude and mean equations with
constant coefficients enclosed over the full subcell. For a
second Taylor coefficient bound $M_2$ and subcell width $h$, the
midpoint integral error is at most $M_2h^3/12$. The first derivative
of each candidate defect bounds its departure from its midpoint.
The code propagates the physical EF mean quadrature from the
decaying ray to the real cap. At $r_m$ its actual value is real by
analytic deformation. The cutoff beyond $\rho=64$ uses a separate
analytic bound on each weak source component and the certified
absolute $U(r_f)$ bound.
[B's refined launch](investigators/B-TIGHT-LAUNCH.md) ·
[C's complete tail](investigators/C-RETURNING-TAIL.md) ·
[E's signed audit](investigators/E-SIGNED-PROJECTION.md)

The direct propagation of interval adjoint states enclosed zero.
Its near-horizon boundary term and real-leg volume term individually
had magnitudes near $60$. The residual pairing above reduced the
correlated uncertainty enough to resolve their signed sum. The
coefficient check reassembled the earlier direct EF expressions
at ten real and ray points, including inside the launch and near
the cap. The largest scaled discrepancy was $1.264\times10^{-14}$.
[Wide result and development control](checks/signed_projection_failures.txt)

## Normalized denominator

Write $D_{\rm RW}=p_L-p_R$ for the massless matching difference.
At the simple zero, $W'=-h(r_m)g_+(r_m)D_{\rm RW}'$ with unit horizon
and outgoing leading amplitudes. A centered five-point derivative
of $D_{\rm RW}$ uses step $10^{-4}$ and the Cauchy bound on the
$0.005$ disk. For $t$ the ratio of step to disk radius and $M$ the
disk bound, the truncation cap is
\[
 \frac{M}{0.005}
 \left[\frac{4t^4}{3(1-t^2)}+
             \frac{(2t)^4}{3(1-4t^2)}\right].
\]
Cauchy also bounds variation of the derivative across the certified
root disk.

For the outgoing normalization let $g_+=e^{-sx}v$ and
$v_a=1+3/(sr)$. The exact equation residual of $v_a$ is
$f(3-12/s)/r^3$. On the outer ray beyond $\rho=144$,
apply backward Volterra variation of constants to $(v-v_a)_x$
and integrate once more with $v-v_a\to0$. In the weighted ball
$\rho^2|v-v_a|\le B$ the integral is a contraction with factor
below $0.02687$. It supplies the actual outgoing amplitude and
logarithmic derivative at $144$. Directed Riccati transport and the
polynomial logarithmic primitive then give $g_+(r_m)$. The unit
$h(r_m)$ is transported from the horizon Taylor solution.
[C's denominator audit](investigators/C-DENOMINATOR.md)

## Preparation and common clock

The ordinary axial residue for the same packet is
$A_{01,q}=-J_h(s_q)/W'(s_q)$, with the unshifted compact moment
evaluated directly at the pole before any division by $W$.
In shear units the real infinitesimal common time shift has
direction $s_q A_{01,q}$. Its projection can be removed before
dividing by the Wronskian. With $K=-s_qJ_h(s_q)$, the signed
perpendicular part of $A_{21,q}$ is
\[
 \frac{\operatorname{Im}(I\overline K)}{|K||W'(s_q)|}.
\]
The unshifted moment and the outward-rounded four-subdivision numerator
give the strict interval $(-0.44590,-0.33745)$ for this component.
The ADM mass and Bondi frame are fixed by the data prescription.

On a common compact support around this packet, the residue is
linear and continuous in the axial canonical profile in the
$C^1$ norm with the same upper-data prescription. The compact
moments and explicit contacts depend boundedly on that norm.
The two noncoincident radial inverses give bounded solutions on
each finite leg and in their specified weighted ends. The collar
Taylor functional and exponentially decaying outgoing projection
then give a bounded linear scalar functional. A nonzero value
implies an open dense complement to its kernel in the linear real
packet space. On the unit derivative-square surface, perturb a kernel
packet by a small real multiple of the certified packet and normalize
again. The perturbations converge in $C^1$ and their residues are
nonzero. This gives relative openness and density on the surface. These assertions use fixed
support, GL tangent and canonical rule. Independently prescribed
upper homogeneous data alter the result as shown in
[Resonance](QNM-RESIDUES.md).

The local pole term in the continued transform has inverse
Laplace contribution $A_{21,q}e^{s_q u_{\rm phys}}$ with its
reality partner. The [finite-cut waveform](WAVEFORM.md) controls
the prompt ordered signal and the signed early interference.
A time interval of pole dominance, a finite-amplitude Bondi
remainder and the late nonlinear bulge dynamics remain open.

## Reproduction

From the scoped directory with Python 3.14.8, python-flint 0.9.0,
NumPy 2.4.6 and SciPy 1.18.0, run

```sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
python3 GLR-GOAL-011/checks/physical_overlap/tight_launch.py
python3 GLR-GOAL-011/checks/physical_overlap/return_coefficients_check.py
python3 GLR-GOAL-011/checks/physical_overlap/return_tail.py
python3 GLR-GOAL-011/checks/physical_overlap/denominator_refinement.py
python3 GLR-GOAL-011/checks/physical_overlap/signed_projection.py
python3 GLR-GOAL-011/checks/physical_overlap/signed_projection.py --subdivisions 4
python3 GLR-GOAL-011/checks/physical_overlap/clock_projection.py
```

[Saved arithmetic and physical bounds](checks/signed_projection.txt)
