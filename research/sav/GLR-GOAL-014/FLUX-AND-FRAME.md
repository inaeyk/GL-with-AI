# Einstein-frame tensor flux and physical cuts

Use dimensionless orbit coordinates with $r_0=1$ inside the hierarchy. Use
the inertial Einstein-frame clock to convert back to physical time. Reducing
the Einstein action on the circle with fixed asymptotic circumference $L$
sets $G_4=G_5/L$. The mean pure-radion factors cancel in the noncompact
axial orbit densities, while the massive compact products vanish in the
finite-cut extraction under the weighted mixed estimates. The physical
mean geometry entering the hierarchy is still the fixed-mass Einstein-frame
mean. [GOAL-011 hierarchy, Eqs. (3)–(5)](../GLR-GOAL-011/HIERARCHY.md).

Let $Y$ be real and satisfy $\int Y^2=1$. For $L_\ell=\ell(\ell+1)$,
$X_A=-\varepsilon_A{}^B D_BY$ obeys $D_A X^A=0$ and $\int X^2=L_\ell$.
The curvature commutator on the unit sphere gives
$D^2 X_A=-(L_\ell-1)X_A$ and
$D^B D_A X_B=X_A$. Consequently
$D^B X_{AB}=-(L_\ell-2)X_A/2$ for $X_{AB}=D_{(A}X_{B)}$. Integrating
this divergence against $-X^A$ gives
$\int X_{AB}X^{AB}=L_\ell(L_\ell-2)/2=12$. The direct norm check expands
the symmetrized gradient and uses
$\int |DX|^2=(L_\ell-1)L_\ell$ and
$\int D_A X_B D^B X^A=-L_\ell$.

With $g_{AB}=r^2\gamma_{AB}+r C X_{AB}+O(1)$ the inertial Bondi
angular shift gives $h_u=-\lambda C/4+O(r^{-1})$. The axial metric
coefficient $h_r$ vanishes in Bondi angular gauge. The completed axial
potential has curl $F_{ur}=-\partial_r(h_u/r^2)$ because the $h_T$ part
is a gradient. This gives $F_{ur}=-\lambda C/(2r^3)+O(r^{-4})$.
Since $F_{tr}=F_{ur}$ and $Q_z=-r^4 F_{tr}$ asymptotically in the pinned
orientation, $\lim 2Q_z/(\lambda r)=C$. The dimensionless limit multiplied
by $r_0$ is the physical shear. Its $u_{\rm phys}$ derivative equals the
dimensionless $u$ derivative $N_T$. [Mädler–Winicour v3, Eq. (56)](https://arxiv.org/pdf/1609.01731v3)
· [Martel–Poisson v1, Eqs. (5.13), (6.11)–(6.12), (6.16)](https://arxiv.org/pdf/gr-qc/0502028v1).

The Bondi mass-aspect equation gives the tensor power
$\int N_{AB}N^{AB}/(32\pi G_4)$ after integrating the angular divergence
on round cuts. The coefficient on the prescribed interval follows by
multiplying the harmonic norm, the factor of two in the news square and
the time conversion $du_{\rm phys}=r_0du$,
$$
 E_{22,T}(I)=\frac{12\cdot2r_0}{32\pi G_4}\mathcal P(I).
$$
The square of the exact tensor news integrates to a nonnegative energy.
Its ordered interference coefficient can be negative. Exact $SO(3)$ of
the GL background removes the axial tensor channel without the seed.
Radion Bondi flux and massive KK flux through timelike surfaces require
their own observables. The coefficient above concerns this real harmonic
on the matched cuts. [Flanagan–Nichols v5, Eq. (2.11a)](https://arxiv.org/pdf/1510.03386v5).

## Pullback and interpretation

An axial angular gauge change adds the same gradient to the two terms in
$A_a=h_a/R^2-\partial_a(h_T/R^2)$, so the orbit curl is unchanged. An
orbit-coordinate relabeling acts by common pullback of the fields, data,
frame and integration region. A common absolute translation moves both
cut labels and both news profiles. At fixed coordinate endpoints,
$u_{\rm old}=u_{\rm new}+\epsilon^2 c$ adds
$c[N_{01}^2]_a^d/2$ to $\mathcal P$. Under a time-dependent
bookkeeping relabeling $u_{\rm old}=u+\epsilon^2 c(u)$, physical flux
per unit inertial time is pulled back with the Jacobian
$1+\epsilon^2 c'$. The two terms in the change of the energy density
are proportional to $(cN_{01}^2)'$. The pullback of the physical
endpoints cancels the corresponding boundary term. A derivative of
shear with respect to the new coordinate includes a Jacobian itself and
must be converted back to inertial news before using the flux formula.
Angle-dependent relabelings require the cut pullback generator by
generator and the pulled-back angular tensor and measure. Comparing
one coordinate harmonic on new constant-coordinate cuts can define a
different projection.

The [projection record](PHYSICAL-PROJECTION.md) separates the directed
negative homogeneous graph summand from the empirical full early
candidate. The graph summand by itself is a convention-dependent split of
the Duhamel solution. An energy coefficient for the complete physical
window requires the sum.

## Conditional nonlinear bridge

Suppose a smooth five-dimensional constrained family with the specified
jets and fixed charges has an exterior through a common set of physical
cuts. Let $\mathcal N(\epsilon,\eta)$ be its tensor news in
$Y=L^2(I)$. Suppose the solution-to-news map in this norm is even in
$\epsilon$ by half-period covariance, vanishes at $\eta=0$, has the
ordered jets, and admits uniform mixed derivative bounds
$\|\partial_\eta\partial_\epsilon^4\mathcal N\|\le M_{41}$,
$\|\partial_\eta^2\partial_\epsilon^2\mathcal N\|\le M_{22}$ and
$\|\partial_\eta^2\mathcal N(0,\eta)\|\le M_{02}$.
Taylor's formula in $\epsilon$ at fixed $\eta$ and then in $\eta$ gives
$$
 \begin{aligned}
 \left\|\frac{\mathcal N(\epsilon,\eta)-\mathcal N(0,\eta)}
 {\epsilon^2\eta}-N_{21}\right\|_Y
 &\le M_{41}\epsilon^2/24+M_{22}|\eta|/4=:e_{21},\\
 \left\|\mathcal N(0,\eta)/\eta-N_{01}\right\|_Y
 &\le M_{02}|\eta|/2=:e_{01}.
 \end{aligned}
$$
For this harmonic, the difference of the two exact energy integrals
divided by $[3r_0/(4\pi G_4)]\epsilon^2\eta^2$ differs from
$\mathcal P$ by at most
$$
 \|N_{01}\|_Y e_{21}+(\|N_{21}\|_Y+e_{21})e_{01}
 +\frac{\epsilon^2}{2}(\|N_{21}\|_Y+e_{21})^2.
$$
The last term comes from the square of the news difference. Establishing
the data family and uniform mixed-derivative trace estimates at null
infinity is open. Finite-radius Taylor differentiability alone leaves
those estimates open, and the perturbative smallness region is bounded
by $|\epsilon|e^{\Omega t}\ll1$ near the source. The theorem gives no
amplitude range or pinch-off claim.

## Nearby fixed-support seeds

Fix the graph, normalized GL coefficients, ordered data rule and cuts.
Take $X=H^4_0((a,a+\delta))$ with zero extension as a topology for seed
variations. It supplies at least the first three weak derivatives
needed in the finite-cut graph and mixed-source estimates. For bounded
ordered maps $T_0,T_2:X\to L^2(I)$ with norms $K_0,K_2$, the two
stage traces are linear in the seed. Polarization and Cauchy–Schwarz give
$$
 |\mathcal P(B+h)-\mathcal P(B)|
 \le K_0 K_2(2\|B\|_X\|h\|_X+\|h\|_X^2).
$$
If a directed benchmark margin $m_*>0$ and applicable evaluated norms
are available, any $\|h\|_X<\sqrt{\|B\|_X^2+m_* /(K_0K_2)}-\|B\|_X$
preserves the sign. The norm bound must cover the same completed graph
and strong extraction under perturbation of the packet. Here the
complete sign and usable operator constants remain open. This concerns
the ordered response within a fixed-support class.
