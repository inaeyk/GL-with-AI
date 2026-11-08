# GOAL-011 exterior and mean enclosures, adverse audit E

Non-blind one-pass audit of exterior_profiles.py, exterior_source.py,
mean_certificate.py, enclosure_math.py, outer_tail.py and the parent
spectral_certificate.py in the specified temporary tree. I also read the
GL jet helper used by the mean and source. This compares the code to the
previous independent EF, launch and spectral derivations. Replays use the
shared implementation and do not constitute an independent numerical solve.

## Normalization and actual source

For the outward scalar profiles
\(u_x/u=\ell+P\), \(\ell=-q+\alpha f/r\).
The analytic factor from the match is
\(\exp[-q(x-x_m)+\alpha\log(r/r_m)]\).
The cells were validated from large to small ray parameter; the reverse
list starts at the match but keeps each negative cell step.
Thus the outward integrals are \(J(t)-J(1)\) within a cell
and \(J(0)-J(1)\) for a full traversed cell, as implemented.
For CompactGL the cells already point outwards and integrate \(J(t)-J(0)\).
The primitive uses \(dx/dt=\Delta r(1+1/(r-1))\) and expands the
reciprocal in \(-\Delta r\,t/(r_{\rm start}-1)\).
The omitted geometric tail times the polynomial coefficient norm,
plus the Riccati residual radius times the tortoise derivative bound,
is a valid uniform integrand bound. Length factors in the local
primitive bounds are correct. Exponentiation with the supplied
matching values is therefore an enclosure of the normalized solution.
For the adjoint test the outgoing profile normalized to the ingoing
matching value is the physical test at the exact certified pole.
At off-pole parameters this defines an auxiliary outgoing extension,
which can enclose the physical value at the pole but need not be
the full ingoing test there.

The GL Schrödinger master \(u=e^{\Omega x}N\) is the static-time
amplitude. In forcing_coefficients, njet with \(N=1\) and
\(N_r=(p-\Omega)/f\) gives EF-factorized coefficients. Multiplying
by \(u\), rather than by \(N\), restores the \(e^{\Omega x}\)
required in the static-time source. Likewise the baseline unit
amplitudes \((A_v,A_r^{EF})=((f+rp_\psi)/(2r^2),
[\sigma/(2r)-A_v]/f)\) are multiplied by the physical
\(\Psi(\sigma,r)\). The source is therefore in actual
physical units \(u\Psi\). On the support-free outgoing leg the
mixed source \(X^{tx}=6r S u\Psi\) and density transformation
\((Y^t,Y^x)=(fY^v-Y^r,Y^r)\) agree with the completed
first insertion. Its \(F,G\) are the terms of
\(U_x=J+F,\ J_x=(s_1^2+V_{\rm eff})U+G\).
There is no returning Fourier half factor at this first stage.
The source vanishes if either actual profile amplitude vanishes.

The centered enclosure uses the holomorphic mean value identity
along the straight line in the product of convex Acb boxes.
For the actual calls the rational denominators are regular and
the chosen square root is on the analytic branch with
\(\operatorname{Re}(\lambda+k^2r^2)>0\).
The dual partials bound each derivative on the full input box.
The inclusion therefore bounds the actual source, without
assuming the sampled center is accurate. The GL/RW/massive
outgoing arrays share the exact same NumPy grid formula and
insert the identical knot 64 in the audited code. An explicit
assertion of cell endpoint equality, in addition to equal
length, would make that dependency harder to break.

## Directed affine solve and analytic endpoint

Let \(P_R\) be the full outgoing matrix logarithmic derivative.
For \(\chi=J-P_RU\), direct differentiation gives
\(\chi_x=-P_R\chi+G-P_RF\). The affine code encloses the
Hermite candidate's residual in precisely this equation,
including the signed \(dx/dt\), the full source, and the
actual interval \(P_R\). A subcell of width \(1/m\) divides
both the logarithmic norm and residual forcing by \(m\).
The Hermitian Gershgorin bound supplies the Euclidean
logarithmic norm, so the constant scalar comparison solution
bounds the end error. The candidate's splines and DOP853
tolerance only propose the test polynomial. The proof uses
the analytic endpoint bound and outward-rounded residual,
independently of their empirical accuracy.
The dense candidate is initialized to exact zero at FAR
in this SciPy run. An explicit assertion of that equality,
or adding its actual norm to the initial error, would make
this assumption visible at the call site.

The R=64 Riccati tail has the weighted \(P=O(\rho^{-2})\)
ball and the strict invariant/contraction checks.
For the two massive corrections set \(j=f/r\), \(c=4/d\).
Then \(a=(2-c)j,\ b=(1+c)j\). The use of
\(j^2+j_x=f/r^3\) in the second diagonal and extraction
of the \(6/r^2\) term in the first are exact. The common
residual coefficient \(4-k^2-\alpha^2+\alpha\) follows
by subtracting the Coulomb factor before bounding absolute
values. The source caps have the correct \(\rho^{-2}\)
and \(\rho^{-1}\) component falloffs. Along the positive
ray direction the full outgoing matrix has Hermitian
bound \(-d_m I\) after multiplying by \(dx/d\rho\).
The backward homogeneous propagator for \(-P_R\) therefore
contracts at rate \(d_m\). The integral cap for \(\chi\)
with zero limit at the far end has denominator
\(d_m+d_{\rm GL}+d_{\rm RW}\), with the extra \(R/\rho\)
factor as coded. This is an affine boundary enclosure at the
match, with no mixed inward matching claim.

## Mean quadrature and explicit constants

The mean integrand is in ingoing EF units, so the scalar
factor is \(N^2=(e^{-\Omega x}u)^2\). It is not the square
of the static-time master. The coefficients in mean_rhs
are \(\mu'_v=-r^2 T^v{}_v/2\), \(\beta'_v=rT_{rr}/2\)
after removing the common \(e^{2\Omega v}\). On the right
each negative step contributes to \(\int_\infty^{r_m}\).
The compact mean integral is in the opposite direction,
\(\int_{r_{\rm cap}}^{r_m}\), so it is subtracted.
The large-arc deformation for the mean preserves the real
solution because the GL decaying factors continue to decay
through the wedge, and the EF mean source is analytic there.

I independently checked the coarse componentwise bound
in mean_tail. If \(B=64|r||N|\) bounds each metric component
and its first two EF radial derivatives at \(|r|\ge64\),
then with \(|f|<1.016,\ |\Omega|<.064,\ |k|<.63\),
the four connection terms entering mean_series.py are
bounded by \(0.04 B,0.62 B,0.55 B,1.55 B\), respectively.
The first Hessian terms are bounded by \(3.2|r|B\) and
the needed second Hessian terms by \(9|r|B^2\).
The scalar curvature variation is below \(1.2B\).
Substituting gives the loose component bounds
\(|Q_{rr}|<2B^2,\ |Q_{\theta\theta}|/|r|^2<2B^2,\ |Q_{zz}|<B^2\).
Both \(|T^v{}_v|\) and \(|T_{rr}|\) are therefore well
below \(128 B^2\). The unit-disk GL first-order norm check
justifies the factor \(e\) in the metric sup bound and the
first/second derivative factors from Cauchy. The derivative
bound for \(N\) at the disk center uses
\((p_{\rm cap}+\Omega)/|f|<1\).
The algebraic ftr, vr and sphere coefficients in the disk
bounds agree with the explicit GL jet helper.
Finally \(r^2(128(64|r||N|)^2)/2\) gives the coefficient
\(64^3|r|^4|N|^2\), and
\((2.5+\rho)^4\le(2.5+64)^4e^{4(\rho-64)/(2.5+64)}\).
This verifies the stated decay denominator \(2\,\mathrm{rate}
-4/(2.5+64)\) and the explicit tail constant. The beta
derivative is smaller by the unused factor \(|r|^{-1}\).

## Replay and scope

Both replays returned exact matches to stored outputs,
using bytecode disabled, OPENBLAS_NUM_THREADS=1 and pipefail:
- python3 exterior_source.py piped through diff -u against
  glr011-exterior-affine-factored.txt.
- python3 mean_certificate.py piped through diff -u against
  glr011-exterior-mean-factored.txt.

The outgoing affine tail cap was \(9.788\,10^{-25}\)
rounded up and the match-error norm cap \(0.007539\)
rounded up. The mean tail cap for either component was
\(9.054\,10^{-16}\) rounded up. The mean intervals are
considerably wider than the earlier assumed narrow boxes.
No coefficient, orientation, source-unit or candidate-
independence error was found in these right/mean enclosures.
This audit excludes the new left source and the final
physical mixed matching and overlap integration.
