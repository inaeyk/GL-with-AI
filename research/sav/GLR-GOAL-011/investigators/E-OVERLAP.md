# GOAL-011 physical overlap, adverse review E

One bounded, non-blind review of the five shared overlap modules against
HIERARCHY (5), FINITE-DATA, PAIRING, the scalar Jost certificate, and C's
massive pencil. This distinguishes exact algebra from empirical evaluation.
No repository file was changed.

## Exact density and launch checks

Let \(H=b_0^{-1}h\) on the EF \(v,r\) block and \(S=h_{\theta\theta}/r^2\)
for the full cosine amplitude. Unit gauge has \(h_{za}=0\), and
\(\operatorname{tr}H=-2S\). The weights in \(M\) and in the EF curl
density are respectively
\[
(1+\epsilon S)\sqrt{\det(1+\epsilon H)},\qquad
(1+\epsilon S)^2/\sqrt{\det(1+\epsilon H)} .
\]
The first weight has zero linear term and quadratic scalar term
\(-S^2/2-\operatorname{tr}(H^2)/4\). Including the inverse metric
gives \(M_1\) proportional to \(-H\) and \(M_2\) proportional to
\(H^2-(S^2/2+\operatorname{tr}(H^2)/4)I\). The second weight has
coefficients \(3S\) and \(7S^2/2+\operatorname{tr}(H^2)/4\).
The code multiplies each of \(HJ\) and direct \(H^2P\) by \(1/2\)
for the circle average. Solving the mixed +k system with a full
cosine amplitude requires exactly this \(1/2\). The noncompact
components agree for the two signed modes and the compact
polarization changes sign. No additional factor is due. The mean
Einstein block uses weight \(+\beta I\) in \(M\), \(-\beta\) in the
curl, and no Fourier half factor because its input is already averaged.

In density coordinates \(Q_z=-r^4 F_{vr}=-r^4F_{tx}/f\).
The first curl insertion is therefore \(X^{tx}=3S Q_{0z}\).
The currents transform from \(v,r\) to \(t,x\) as
\((D^t,D^x)=(fD^v-D^r,D^r)\), including the Jacobian.
In the mixed pencil put \(u=r^4F_{tx}/f-X^{tx}\) and
\(D=r^2(\lambda+k^2r^2)\). The reconstruction signs follow from
\[
A_t=(D^t-u_x-ik r^4s_1 A_z)/D,\qquad
A_x=(-D^x-s_1u-ik r^4A_{z,x})/D .
\]
In the normalized \(Y,Z\) pencil these give the script's forcing
\(\partial_x F+G\), the storage of derivative minus \(F\), and its
regular combination \(G+s_1F\). In particular the \(\lambda f\)
and \(-ik\) contributions are retained. Az reconstruction is
consistent with the two polarizations, even though it only
contributes indirectly to the returning noncompact densities.

For the mean metric the compact \(z\)-Gauss terms give
\(-k^2\operatorname{tr}(H^2_{\rm four})/8\) in \(Q_{zz}\) and
\(+k^2 H_{\rm cov}H_{\rm mixed}/4
-k^2\operatorname{tr}(H_{\rm four})H_{\rm cov}/8\) in the other
blocks. These are already averaged. The four-dimensional warp
identities \(R_{\theta\theta}=1-\Box_b W/2\) and
\(R_{rr}=\frac12 b_{rr}\mathcal R_b-W^{-1}W_{;rr}
+(W_r/W)^2/2\), \(W=R^2\), reproduce the code's remaining terms.
Independent use of
\(\delta\mathcal R_b=D_aD_bh^{ab}-\Box\operatorname{tr}h
-\operatorname{Ric}^{ab}h_{ab}\) reproduces curvature1, including
the \(f'\Omega h_{rr}/2\) and \(f'^2h_{rr}/2\) terms. The EF
Einstein source contracts to
\(T^v{}_v=fQ_{rr}/2+Q_{\theta\theta}/r^2+Q_{zz}/2\);
the \(Q_{vr}\) terms cancel. The fixed-mass quadrature signs for
\(\mu,\beta\) follow from their radial constraints.

At the shifted baseline argument \(\sigma=s-2\Omega\), the tilted
step gives launch contacts \(+\exp(-\sigma T)a\) in the physical
spatial derivative and \(-\exp(-\sigma T)a\) in the physical time
derivative relative to the stated Green derivative. The integration
variable \(y=(u_\Sigma-u_*)/\delta\) is decreasing with radius.
The code consequently uses \(J_h={\rm full}-{\rm partial}\) and
\(J_g={\rm partial}\) inside the support, with \(\delta^{3/2}\)
from the \(u_\Sigma\) Jacobian and packet norm. Thus the source uses
the physical zero-extended \(A_0\). In the higher orders the fixed
tangential and exact electric data cancel the explicit graph terms
as in PAIRING (2). Neither the HJ nor the direct terms silently
drop a baseline contact. The empirical \(W'\) differentiates
\(W=hg_+'-h'g_+\) with unit leading factors at the two ends and
agrees in convention with the rigorous spectral certificate.
Its finite differencing and endpoint asymptotic series remain
numerical.

## Strong-weak horizon check and conditioning

With hats for the transforms in \(t\) put
\[
V=e^{-sx}\widehat{\mathcal D^v}/r^2,\quad
P=e^{-sx}\widehat{\mathcal D^r}/r^2,\quad
Q=P-fV,\quad C=e^{-sx}\widehat{\mathcal C},\quad
H_*=rh\,e^{sx}.
\]
Then \(\hat q=-e^{sx}Q\), and the strong regular integral on the
left in \(dr\) is
\((2/\lambda)\int H_*(Q_r-sV-\lambda C/r^4)\,dr\).
The weak right-side integration starts with boundary
\(-(2/\lambda)H_*Q\) at the split. In the initial script,
integration by parts after subtracting \(Q(r_{\rm left})\)
changes this boundary to
\(-(2/\lambda)H_*Q(r_{\rm left})\) and puts
\(-H_{*,r}[Q-Q(r_{\rm left})]\) inside the integral. The signs
are correct. An independent manufactured check used
\(H_*=(1+t)e^{2s(1+t+\log t)}\), \(t=r-1\),
\(Q=1+0.3i+(2-0.5i)t+0.9t^2\), \(V=0.3+0.2it\),
\(C=0.8i-0.15t\), split \(t=0.27\), and \(s\) from the literature
center. Direct strong-plus-boundary and subtracted evaluations
agreed to \(3.34\,10^{-16}\) and \(1.30\,10^{-15}\) at cutoffs
\(10^{-3}\), \(10^{-6}\). This tests the sign and endpoint term,
not the physical field accuracy.

The initial implementation's \((A_x-A_t)/f\) in the EF
reconstruction is ill-conditioned toward the horizon. The
coordinator reported drift from roughly \(0.1354-0.3708i\) at
\(10^{-5}\) to \(-0.0294-0.2080i\) at \(10^{-7}\), along with
a large near quadrature estimate. The first variant's small
split difference and exterior contour agreement cannot validate
that refinement. Preserve this negative numerical result.

A cancellation-free route follows directly from the same mixed
equations. With tildes denoting removal of \(e^{s_1x}\), the regular
EF radial component is
\[
\widetilde A_r^{EF}
=\frac{-\widetilde D^v+\partial_r\tilde u
       -ik r^4\partial_r\widetilde A_z}{D}.
\]
For a future validation one can integrate the regular \(U=(Y,Z)\)
derivatives using
\[
 f\widetilde U_{rr}+(f_r+2s_1)\widetilde U_r
 -(V_{\rm eff}/f)\widetilde U
 =\partial_r\widetilde F+(\widetilde G+s_1\widetilde F)/f .
\]
Use \(D^x+D^t=fD^v\) algebraically in the right side. This avoids
subtracting nearly equal components and removes the need to
differentiate a numerically unstable horizon extrapolant.

The updated script fits \(Q,V,C\) only on an exterior collar,
then directly integrates the strong formula against the regular
Jost series, adding the exact split boundary. Its series equation
is \((r-1)r^2 n_{rr}+(r+2sr^3)n_r-(6r-3)n=0\) for
\(h=e^{sx}n\), as required. The change \(t=t_{\rm cap}e^{-y}\)
gives \(t^{1+2s}\), with \(\Re(1+2s)>0\).
The fit, differentiation, inward extrapolation, and \(y=90\)
termination have no interval enclosure in this run.

## Alternate contour and scope

Commands ran with bytecode disabled and OPENBLAS_NUM_THREADS=1
in the shared temporary directory, with Python 3.14.8,
NumPy 2.4.6, SciPy 1.18.0. At \(R=13/8\), \(\delta=.12\),
ray length 64, horizon cutoff \(3\,10^{-6}\), and tolerance
\(10^{-10}\), the original variant produced the residues
\(0.1324938905-0.3712234296i\) at angle \(\pi/4\) and
\(0.1324793318-0.3712783255i\) at angle \(0.9\).
The ray parts agreed to about \(4\,10^{-8}\), and changing
the split from 1.22 to 1.3 changed the mixed numerator about
\(1.3\,10^{-7}\). The remaining difference was dominated
by near-horizon HJ. For deformation of the exact outer integral,
the potentially growing outgoing massive factor combines with
the GL and outgoing adjoint factors to give exponent
\(q_\infty+s+\kappa\), whose real part stays positive in the
sector from the real axis to angle \(0.9\) at the target.
The forced-tail exponent \(2q_\infty+2s-2\Omega\) is also
decaying there. The empirical contour test uses truncated
endpoints and does not bound their errors.

The updated variant used angle \(0.9\), the same length, horizon
cutoff and tolerance, cap \(0.007\), degree 7, and repeat cap
\(0.013\). It gave
\[
I=0.4259912456-3.1356504837i,\qquad
I/W'=0.1352261351-0.3706301052i,
\quad W'=7.8364909306-1.7098102027i.
\]
Repeating the cap changes the HJ numerator by
\(2.7403\,10^{-5}+2.7393\,10^{-5}i\), with the other parts
changing by less than \(4\,10^{-9}\). The tiny reported
near quadrature residual concerns integration of the fitted
functions. It is distinct from fit, endpoint, mixed-solve,
normalization, and global discretization errors.

No algebraic discrepancy was found that would cancel the
reported overlap. The value is provisional empirical evidence
for this specified preparation. The small-fit and contour
variations are not error enclosures, and the homogeneous
spectral certificate cannot certify this source numerator.
