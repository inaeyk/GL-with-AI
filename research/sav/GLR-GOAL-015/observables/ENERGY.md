# Tensor energy on matched cuts

Let $C_{AB}=C X_{AB}$ be the real axial shear, with
$X_{AB}=D_{(A}X_{B)}$ and the spherical harmonic normalized by
$\int Y^2 d\Omega=1$. The exact-curl master convention gives
$C=\lim 2Q_z/(\lambda r)$ in dimensionless units. One verifies
this with the asymptotic Bondi angular shift:
$g_{uA}=D^BC_{AB}/2+O(r^{-1})$ and
$D^BX_{AB}=-\lambda X_A/2$, so in angular Bondi gauge
$F_{ur}=-\lambda C/(2r^3)+O(r^{-4})$. In the inherited orientation
$Q_z=-r^4 F_{ur}$ at leading order. The shear has physical
length $r_0C$, so its derivative with respect to $u_{\rm phys}$
equals the dimensionless news used in the calculations.
See [Mädler–Winicour v3, Eq. (56)](https://arxiv.org/pdf/1609.01731v3)
and [Martel–Poisson v1, Secs. V–VI](https://arxiv.org/pdf/gr-qc/0502028v1).

Integrating the spherical divergence against $X_A$ gives
$\int X_{AB}X^{AB}d\Omega=\ell(\ell+1)(\ell-1)(\ell+2)/2=12$.
The exact tensor power is $\int N_{AB}N^{AB}d\Omega/(32\pi G_4)$
in the inertial Einstein frame, $G_4=G_5/L$.
[Flanagan–Nichols v5, Eq. (2.11a)](https://arxiv.org/pdf/1510.03386v5).
The same factor follows from the two transverse strain
polarizations: their squares sum to half the squared news
tensor divided by the observer radius squared. Expanding
$N_T=\eta N_{01}+\epsilon^2\eta N_{21}+\cdots$ and converting
the time interval gives the matched-cut energy coefficient
$$
 E_{22,T}(I)=\frac{3r_0}{4\pi G_4}\int_I N_{01}N_{21}\,du .
$$
The negative coefficient on the full cut is compatible with
nonnegative total tensor energy. A change of clock convention
must pull back both the energy density and the cut boundaries;
a fixed-coordinate endpoint shift of order $\epsilon^2$
otherwise changes the interference by a news-squared endpoint
term. The coefficient uses the same mass, circumference,
angular harmonic and physical clock in both ordered solutions.
It is separate from the radion Bondi flux and massive KK flux.

The early and packet signs imply a stronger temporal statement:
the ordered cumulative projection is positive at $a+0.015$
and negative at $a+0.12$. The news factors are $L^2$ on this
finite interval, so the prefix integral is continuous. There is
at least one zero between those cuts, and the contribution from
the remainder of the packet is strictly negative. No crossing
time or pointwise waveform phase follows from these enclosures.

For a finite seed the energy comparison also needs a uniform
solution-to-news expansion of the fully constrained nonlinear
family. The [inherited remainder criterion](../../GLR-GOAL-014/FLUX-AND-FRAME.md#conditional-nonlinear-bridge)
would preserve the early sign if its dimensionless normalized
energy remainder were smaller than the certified early lower
margin, and the packet sign if it were smaller than the
absolute upper packet endpoint. There is no proved admissible
amplitude range or seed-neighborhood radius here. The later
ringdown interval has its own strong trace and residual
requirements.
