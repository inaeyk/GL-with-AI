# Signed projection and interval arithmetic audit

I inspected the pinned source of [draft PR #13](https://github.com/inaeyk/GL-with-AI/pull/13) at `eb439228601a649c658ce719b503ff0bdc37a83f`, stacked on `0864d9d807de56cbdd6e64d215e6295508a3007f`. The target uses $r_0=1$, $x=r+\log(r-1)$, $\ell=2$, $k=\pi/5$, a normalized packet of width $3/25$ at $R=13/8$, and $u_0=0$. The audit was non-blind. I knew the intended result and read the source certificate. I reconstructed the signed boundary algebra, comparison inequalities, midpoint constant and massive asymptotic majorant, ran a separately implemented exact polynomial check, and replayed the imported mixed match. The lower geometry, equivalence to the causal transform, normalized denominator, and complete source replay are separate parts of this audit.

## Exact signed functional and match

For either endpoint family the matrix logarithmic derivative $P$ satisfies $P_x=s_1^2 I+V-P^2$. Set $L=J-PU$ and $H=G-PF$. Then $U_x=PU+L+F$ and $L_x=-PL+H$. For the $HJ$ weak integrand, ordinary differentiation gives
$$
\begin{aligned}
 a^{\mathsf T}U+b^{\mathsf T}L+c
 &=-\partial_x(z^{\mathsf T}U+w^{\mathsf T}L)+z^{\mathsf T}F+w^{\mathsf T}H+c\\
 &\quad +(z_x+P^{\mathsf T}z+a)^{\mathsf T}U
       +(w_x-P^{\mathsf T}w+z+b)^{\mathsf T}L .
\end{aligned}
$$
The two bilinear defect products admit absolute Euclidean Cauchy–Schwarz bounds. In cell coordinates $t$, the derivatives in the defects are $\partial_t$ and all nondifferentiated terms carry $x_t$. Defect integration then needs only the subcell width. The right grid goes from the far cut towards the match and the returning projection uses its negative. The left grid goes from the inner cap to the match with positive orientation.

At the match, $J$ and $U$ are common and $L_R-L_L=(P_L-P_R)U$. Combining the two branches with $w_L=w_R=q$ yields the match row $z_R-z_L+(P_L-P_R)^{\mathsf T}q$. The left collar term is $C+(z_L-z_c)^{\mathsf T}U_c+w_L^{\mathsf T}L_L$ and the far boundary is $-w_R^{\mathsf T}L_R$, since $z_R=0$ there. In the code the common $q$ and $z_L(c)$ are converted to the exact binary values supplied to the candidate solve. All Hermite pieces share their knot data and the starting endpoint displacement is asserted zero. The cancellation is an exact property of the chosen piecewise polynomial. SciPy and binary64 supply trial coefficients. Outward-rounded Acb operations enclose their residuals and endpoints.

The independent manufactured check prescribes continuous complex polynomial $U,J$, distinct noncommuting $P_L,P_R$, and piecewise polynomial $z,w$, then defines $F=U_x-J$ and $H=L_x+PL$. It integrates both sides exactly in SymPy, including nonzero collar and far terms. Both evaluations give $131/15+192i/35$. Omitting the match contribution changes the result by $3$. Conjugating only the defect covectors changes it by $-7709/630-2587i/210$.

## Directed comparison and the compact cells

For a directed parameter $t$, write an affine flow as $y_t=A y+b$. Its error against any candidate $p$ satisfies
$$
 \frac{d}{dt}\lVert y-p\rVert_2
 \le \lambda_{\max}\!\left(\frac{A+A^\dagger}{2}\right)\lVert y-p\rVert_2
      +\lVert p_t-Ap-b\rVert_2 .
$$
The inequality holds with the upper right derivative at zero error. If a normalized subcell has upper constants $a,b_*$, the comparison at its outgoing edge is $e^a E+(e^a-1)b_*/a$, with continuous limit at $a=0$. This solution is monotone since its derivative is a constant times $e^{at}$. Taking the maximum of its endpoint values bounds the whole subcell. The implemented Hermitian Gershgorin bound handles both directions and includes the complex $x_t$. When transporting $U$ outwards from the match, the code traverses each stored leg in reverse, hence the use of the numerical abscissa of $-x_tP$. For $A=\left(\begin{smallmatrix}-1&4\\0&-1\end{smallmatrix}\right)$, the evolution norm at $t=1/2$ is $1.46429$, versus the insufficient spectral shortcut $e^{-1/2}=0.60653$. The Hermitian rate is $1$ and gives the valid cap $e^{1/2}$.

For the Riccati comparison the linear error terms are left and right multiplication by the same matrix, each bounded in Frobenius norm by its Hermitian numerical abscissa. Under a trial bound $B$, the quadratic error increases the rate by at most $|x_t|B$. Strict endpoint bounds below $B$, together with the monotone comparison, close the first-exit argument. The saved whole-cell budget, usually larger than the endpoint radius, is used to enclose $P$ along a compact cell.

For an analytic integrand, Taylor's formula about the midpoint of a width-$h$ interval integrates its linear term to zero and gives
$$
 \left|\int g-hg_{\rm mid}\right|\le \frac{h^3}{24}\sup |g''|
       =\frac{h^3}{12}\sup |[v^2]g(t+v)|.
$$
The manufactured $g=t^2$, $h=1/4$ attains $1/768$. The implementation gets the jets from the exact ODE at an interval argument covering the whole subcell. It multiplies the numerator jets by $dx/dt$ and the mean jets by $dr/dt$. The complex midpoint estimate follows by the integral form of the real-parameter remainder. For defects, a first derivative enclosure times a symmetric interval of half the subcell width bounds the midpoint departure. Packet-intersecting cells use direct interval integration. The compact baseline uses interval clamping of all partial-moment slice lengths and includes the graph contact in the reconstructed one-form. Its ratio $\widehat\Psi_x/\widehat\Psi$ is used away from the packet, where a single homogeneous branch supplies it.

## Mixed match replay and input binding

I replayed this bounded non-production command at the source pin with Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6 and SciPy 1.18.0.

```sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 \
  python3 research/sav/GLR-GOAL-011/checks/physical_overlap/ingoing_source.py --subdivisions 8
```

| Quantity | Replay |
| --- | --- |
| Left affine endpoint Euclidean radius | $0.051353881353054918706$ |
| Right affine endpoint Euclidean radius | $0.002371656389504353004$ |
| Outgoing affine initial tail bound | $9.7871620875095003\times10^{-25}$ |
| First component of match center | $-0.18746108862784383598-0.41681210448350056775i$ |
| Second component of match center | $-0.05889059544736579619-0.49465461340480153580i$ |
| Match Euclidean radius | $0.33256173256260722684$ |

For $D=P_L-P_R$ and a trial inverse $A$, write $L_j=c_j+e_j$ and $U=U_c+e$. The exact equation gives $e=(I-AD)e+A(c_R-c_L-DU_c)+A(e_R-e_L)$. Bounding $\|I-AD\|$ and $\|A\|$ by their Frobenius norms gives the claimed radius, retaining Euclidean dependence in the two affine errors. The radius $0.33257$ imported by `signed_projection.py` has more than $8.26\times10^{-6}$ slack with the displayed binary center. The match replay uses a less tight lower-launch enclosure than the signed projection. Both enclose the same exact pinned launch.

**Correction for reuse.** The default certificate is bound to the pinned packet, but `signed_projection.run(subdivisions,input_builder=...)` exposes an arbitrary input builder while importing a fixed match ball and calling the pinned $W'$ calculator. A concrete counterexample to treating the ball as general is a fourfold scaling of the packet. Scale the bump normalization and both lower launch amplitudes by four while retaining the same $s,\Omega,k$. Linearity of the axial problem gives $U_{\rm new}=4U$. The original ball gives $\|U\|\le1.00860645$ and forces $\|4U\|\ge1.37386579$, strictly outside the entire original ball. The change is outside the unit-norm experiment and exercises the callable's missing binding. A reusable entry point needs an explicit immutable input contract or an input-derived match ball and denominator. Recomputing or checking the imported ball would also make the pinned run self-contained.

## Horizon and exterior closures

For the horizon Cauchy estimate, the factored strong integrand is $t_h^{2s}G(t_h)$ with $t_h=r-1$, $\operatorname{Re}(1+2s)>0$, and $G$ analytic. On a Cauchy disk of radius $r_G>c$, coefficients above degree $N$ are bounded by $M r_G^{-n}$. Integrating each with $c^{n+1+2s}/(n+1+2s)$ and replacing the magnitude of the denominator by $n+1+2\operatorname{Re}s$ yields the tail bound used by the implementation. The split subtraction is the strong-to-weak boundary evaluated at the finite cap with its own Taylor remainder. The larger $0.038$ disk for the radial derivative and the $0.034$ disk for $G$ both lie inside the stated regular mixed and mean analytic disks. The collar construction solves for the covector using the two regular homogeneous endpoint columns, then restores the driven term by its exact affine offset. The first insertion brings one factor of the GL scale and the driven intermediate brings the second factor together with the shifted baseline amplitude. For the mean collar the GL scale squares in the driven part and the matched mean values multiply the homogeneous mean profiles.

The outer massive tail uses $P=\ell I+N$, $\ell=-\kappa+\alpha j$, $j=f/r$, $f=1-1/r$, and $2\kappa\alpha=k^2$. Subtracting $\ell^2+\ell_x$ from the diagonal mass-plus-frequency term leaves
$$
  \frac{4f-k^2-\alpha^2 f^2}{r^2}-\alpha j_x .
$$
The cancellation of $1/r$ is exact. The other diagonal terms are $a^2-a_x$, $b^2+b_x$, where $a=j(2-c_d)$, $b=j(1+c_d)$, and $c_d=4/(4+k^2r^2)$. Expanding the first about $6/r^2$, using $j^2+j_x=f/r^3$ for the second, and applying $|4+k^2r^2|\ge k^2|r|^2-4$ recovers the implemented inverse-square envelope. For example, $|r(c_d)_x|\le8k^2 f_b/(d_{\rm low}^2 R^2)$, where $d_{\rm low}=k^2-4/R^2$ and $R=64$ is the ray cutoff. The massive off-diagonal is $-4ik s_1 f/(4+k^2r^2)$. The independent point stress test used an analytic derivative of the unfactored potential, sampling six ray points through $\rho=2048$, all four spectral rectangle corners plus center, and three GL rates. Its maximum $(\rho/64)^2\|Q\|_F$ was $0.0029955153642$, below the asserted $0.003090065$. The analytic envelope controls the full tail. The sample is an empirical cross-check.

The logarithmic derivative tail follows by a weighted Volterra contraction on $\|N\|_F\le m(R/\rho)^2$. The uniform exponent attenuation makes $\operatorname{Herm}(\gamma P_R)\le-d_m I$ with $\gamma=dx/d\rho$. Consequently $L_R$ propagates contractively backwards and $U_R$ forwards. The forcing estimate uses the faster source decay $d_s>d_m$, producing the sum $d_s+d_m$ for the affine tail and difference $d_s-d_m$ for the driven position cap. At the returning pole, the horizon-normalized adjoint also lies in the outgoing subspace, justifying its decaying tail envelope. At an off-pole parameter the horizon-normalized solution generally contains a growing component on that ray. The mean conversion $u=e^{\Omega x}n$ cancels the static-time factor against $|n|^2$ in its tail. Real mean matching data are recovered by analytically deforming the decaying GL mean integral to the real ray.

I found the compact adjoint and matching inequalities consistent for the pinned packet. The mean Ricci, source-reconstruction and contour analyticity assumptions enter these enclosures through the separate geometry and continuation calculations. I traced their interval interfaces and the analytic majorants in the main massive, horizon and quadrature steps. I did not derive every component of the mean Ricci tail majorant again from five-dimensional curvature. The full interval rectangle follows only when the matched lower data, compact and tail bounds, and normalized pole denominator are used together.

## Reproducing the independent controls

Run from the review directory.

```sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 checks/arithmetic_controls.py
```

The exact polynomial control uses SymPy 1.14.0. The nonnormal comparison and tail point sample use binary64 NumPy, and the bound on the match-input counterexample uses 128-bit Arb. One development assertion used strict equality between two Arb computations of the nondyadic midpoint remainder. It failed because the enclosures carry rounding radii. The retained check verifies exact rational equality in SymPy and Arb overlap separately.
