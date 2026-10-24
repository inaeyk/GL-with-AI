# GOAL-005: validated spectral input and coarse source enclosures

2026-10-23, Michael Yan's authorized research task. All lengths use r0=1,
k=pi/5, signature(-++++), no factorial in the quadratic field. The exact
historical GL operator/reconstruction are inputs, independently tested
against full 5D equations by the separate geometry audit. This is not a
new derivation of every historical equation, or an external human review.

## 1. Certified nodeless spectral connection

Let x=r+log(r-1), t=log(r-1), D=1+k²r³, f=(r-1)/r and

\[
 V=k^2f+{(6r-11)(r-1)\over r^4}
 +{18(r-1)^2\over r^4D^2}-{6(4r-5)(r-1)\over r^4D}.
\]

The Schrödinger equation is u_xx=(V+Omega²)u. With p=u_x/u,
p_t=r(V+Omega²-p²). All coefficients, pi, rational trial parameters,
exponentials and rounding errors are enclosed with Arb at160 bits.
The domain is tH=-28, R=40, match r=2; only this physical GL
configuration was used. No saved profile enters the certificate.

At the horizon u=e^{Omega x}v, v(-infinity)=1. The Volterra equation is

\[
v=1+{1\over2\Omega}\int_{-\infty}^x
(1-e^{-2\Omega(x-y)})V(y)v(y)dy.
\]

For delta=r-1, |V|<Cv delta, Cv=k²+47,
eta=Cv(deltaH+deltaH²/2)/(2Omega)<1/2. It follows that
|v-1|<=eta/(1-eta), v>0 and

\[
|p_H-\Omega|\le {C_v\delta_H(1+\delta_H)
 \over(1+2\Omega)(1-2\eta)}.
\]

This loose bound is <2.91e-11. An independently derived residual
contraction in Audit I gives the sharper Frobenius statement
pH=Omega+c deltaH+eH, c=(k²-5+6/(1+k²))/(1+2Omega),
|eH|<1.60e-24, uniform on [.06327,.06330]. The primary certificate
uses the looser bound; independence does not depend on importing that audit.

At infinity use w=e^{-qr}r^{-beta}, q=sqrt(k²+Omega²),
beta=q-k²/(2q)>0. The exact residual (H+Omega²)w/w is bounded by
CR/r² using the three rational coefficients in goal004_source_geometry.md,
which are evaluated outward in the executable. For R=40,
etaI=CR/(2q f(R)²R)<.147. The Volterra solution u=w vI obeys

\[
|v_I-1|\le{\eta_I\over1-\eta_I},\quad
|p_I+f(q+\beta/R)|\le {C_R\over2qf(R)R^2(1-2\eta_I)}.
\]

The latter radius is <.005063. It is not an asymptotic series treated
as convergent. Audit I separately proves a cubic Riccati residual bound
with radius2.74e-5 and deliberately falsifies wrong endpoint prescriptions.

For each finite step the primary constructs a Picard tube Z with
p_initial+[0,h]F(time_cell,Z) strictly inside Z. A degree11 Taylor
polynomial about the center initial value is enclosed, with remainder
|h|^12 times the order12 ODE coefficient evaluated over the whole tube
and time cell. The old radius propagates by

\[
e_{out}\le e_{in}\exp\big(\sup_{cell}[-2r p h]\big)+e_{Taylor}+e_{round}.
\]

The signed factor is essential for backwards contraction on the infinity
branch. Same-parameter mean-value flow justifies the formula even when
Omega is a ball. Endpoint log39 uncertainty is retained. Nominal steps
are <=1/32; failed Picard cells are subdivided, at most10 levels. This
is interval IVP validation, not a tolerance or convergence estimate.

All finite cells over the whole Omega bracket have finite tubes.
Together with positive endpoint v, this proves no poles/nodes. Define
F=pH(2)-pI(2). Differentiating the *linear* equation independently gives
(u² partial_Omega p)_x=2Omega u². Both endpoint boundary terms vanish,
so on this nodeless branch

\[
F'=2\Omega\left[\int_{-\infty}^{x_m}(u_H/u_H(x_m))^2dx
 +\int_{x_m}^{\infty}(u_I/u_I(x_m))^2dx\right]>0.
\]

Strict opposite signs enclose one root in
**[.06328118,.06328120]**, width2e-8. The exact lower/upper matching
balls are saved in goal005_gl_results.json; their widths are <3.95e-10.
The match centers are approximately -4.07e-8 and +4.99e-8. No global
claim about all unstable eigenvalues is made.

For u(2)=1 the sphere normalization functional is

\[
\nu=-{1\over8}-{16k^2-1\over32k^2(1+8k^2)}-{p(2)\over8k^2}.
\]

It lies in [-.077137656,-.077137624], hence is nonzero. Scaling by
1/nu gives Htheta theta/(2r²)=1 at r=2. This certifies a physical,
non-pure-gauge normalized eigenmode, not numerical chi or horizon bulge.

## 2. A coarse global regular C5 upper envelope

Define M as the maximum over four covariant SO(3) coefficients
(Hvv,Hvr,Hrr,Htheta theta) and derivatives0..5 on tau=v-r=0,
1<=r<=4, together with the four *static* profiles on r>=4 weighted
by exp((r-4)/2). The weight multiplies each derivative; it is not
differentiated at the chart boundary. Hphi phi=sin²theta Htheta theta
is handled as the round-sphere tensor. This is a mixed profile norm,
not a common-slice tetrad norm. The certified result is **M<4e29**.

To obtain amplitudes, every IVP tube bounds the whole-cell integral
of r p dt. Subtracting the integral to r=2 normalizes u(2)=1 without
floating quadrature. The left integral lies in [.96408,1.09275] and
the right integral in [25.1542,27.1790]. Their breadth is retained.
The normalized horizon coefficient u/e^{Omega x} lies in
[-27.295,-23.999]. These intervals include endpoint, parameter,
quadrature and rounding uncertainty; they are deliberately loose.

On delta<=dc=1/4096 set Y=e^{-Omega x}y, y=Du/r. Then

\[
fY''+bY'+cY=0,\quad b=2\Omega+1/r-5f/r+6f/(rD),
\quad c={\Omega(-4+6/D)\over r}+{3\over r^3}-k^2-{3\over r^3D}.
\]

Here b>1 and f'>0 are interval checked on the whole collar. The
ingoing regular-singular solution is analytic (nonresonant denominators
j(j+2Omega)); the Volterra bound supplies its zeroth norm. Differentiating
n times gives a first-order equation for Y^(n+1) with positive coefficient
b+n f'. Its integrating-factor maximum estimate supplies B_(n+1) from
the complete binomial sums in the executable. Derivatives through8 are
bounded. For the exact homogeneous EF reconstruction define

\[
Z=-{\Omega\over k^2D}[fY'+(\Omega-1/(2r^2))Y],\quad
H_{vr}=-Z'/\Omega-2Z/(\Omega r),\quad H_{vv}=Z-fH_{vr},
\]
\[
H_{\theta\theta}=rZ/\Omega-Y/(2k^2r),\quad
H_{rr}=rN/\delta,\quad N=2Z'/\Omega+2Z/(\Omega r)+Y/(k^2r^3).
\]

The regular endpoint equation gives N(1)=0 exactly. Thus
partial_r^j(N/delta)=integral_0^1 t^j N^(j+1)(1+t delta)dt,
and the derivative bound is ||N^(j+1)||/(j+1). The Taylor coefficient
factorials cancel as implemented. Multiplication by e^{Omega r}
converts to the tau slice. No subtraction of singular sampled components
is used at the horizon.

Between the collar and40, full-cell p and amplitude balls generate
derivative jets by the exact ODE, and every reconstruction coefficient
is evaluated on the full cell. This is legitimate derivative control
of an already validated exact solution, unlike assigning on-shell jets
to unvalidated sampled data. It is extremely pessimistic near dc and
causes the enormous bound. On r>=40 compactify t=1/r and factor each
static component as r u E(t,p), with E rational and regular at t=0.
The exact ODE bounds jets of E and u. Finally

\[
\sup_{r\ge40} e^{(r-4)/2}r|u|
\le {|u(40)|e^{18}\over1-2\eta_I}
 \left(40+{1\over q-1/2}\right).
\]

Audit I checks the recurrence, EF map, divided difference and tail
independently, and supplies a conversion factor <85 to a common tau=0
EF coordinate norm. Thus **||H||_(C5,EF,tau,weight)<3.4e31**.
Time/compact derivatives of total order<=5 multiply by Omega^a k^b<=1.
This is an absolute source-controlling bound, not a small C5 error about
the historical cubic, and not a claimed tetrad constant. The historical
cubic is generally not C5 across knots.

## 3. Explicit all-component source envelope

On the compact region remove e^{Omega r} from the tau coefficients;
Leibniz costs at most32. Set B=2e31, a common bound for each first-order
orbit/sphere coefficient and all needed mixed derivatives through5 in
regular EF profiles inside4 and static profiles outside4. Exterior
derivatives each carry exp(-(r-4)/2). Background orbit g3 and g3^-1
derivatives through5 are bounded by Kb=1000 in those charts. For
w=Htheta theta/r², C5(w)<=32*720 B<1e5 B.

Use exact coordinate connection coefficients with g3=g0+epsilon h:
i1=-i0 h i0, i2=i0 h i0 h i0; Gamma0=i0 Dg0/2,
Gamma1=(i0 Dh+i1 Dg0)/2, Gamma2=(i1 Dh+i2 Dg0)/2.
D denotes the three-term Christoffel derivative combination.
For componentwise C^m maxima, a product of q factors costs q^m.
There are three orbit indices. The following conservative bounds are
checked with outward/exact arithmetic, with B factors omitted:

| Quantity | Bound |
|---|---:|
| i1 C4 /B; i2 C4 /B² | 1e9;1e14 |
| Gamma0 C4; Gamma1 C4 /B; Gamma2 C4 /B² | 1e8;1e14;1e19 |
| quadratic orbit Ricci C3 /B² | 1e32 |
| quadratic warped-orbit correction C3 /B² | 1e25 |
| quadratic sphere coefficient divided r², C3 /B² | 1e33 |
| all raw Ricci coefficients C3 /(r² B²) | 2e34 |
| trace-adjusted Einstein source C3 /(r² B²) | 1e44 |

For example the Ricci bound is
6G2+36*8G0G2+18*8G1²<1e32. The warped formulas are
R5_ab=R3_ab-Hess(log W)_ab-(d log W)_a(d log W)_b/2 and
Rtheta theta=1-Box3 W/2, W=r²(1+epsilon w). These give every orbit
and sphere component. The trace estimate reserves48 for differentiated
sphere normalization. The audit independently checks these counts.

Fourier projection is exact: h_±1=H/2; the zero source is the sum
of two cross products (factor1/2), and +2 one product (factor1/4).
The common absolute envelope bounds all of them; it does not replace
their distinct values. In particular Qzz,0=-k²H²/8 and
Qzz,+2=-3k²H²/16. The conserved source is E=Q-g0 tr(Q)/2;
conservation is exact for the validated on-shell H, not an assertion
about the historical cubic. Gauge completion retains half Lie_xi² g0.

For sigma=sqrt((2Omega)²+(2k)²/4)<.65,
sup_(r>=4) r² exp(-(1-sigma)(r-4)) <(4+2/.35)²<100.
Therefore each of the seven Ricci or Einstein source coefficients has
regular compact C3 and weighted exterior C3 norm **<1e110**.
This is an absolute upper interval [0,1e110], not a sharp norm estimate
or source-error interval about the old interpolant. No positivity of
the Lorentz contraction is inferred.

The literal historical Y0 also includes third derivatives of the
master forcing. Generic Q C3 alone does not imply that requirement.
Audit II derives the physical Codazzi saving: Q_mu z is quadratic h∇h,
so its fourth radial derivative needs H5 rather than H6. That permits
an anisotropic norm (ordinary Q C3, mixed Q_mu z C4), but its complete
numerical constants and full Y0 are not asserted here.

## 4. Certified scalar-master consequence and unclosed metric lift

The exact forced source is mathfrakF=-f²r F_y/D, with F_y from the
historical sourced master, K=2k, s=2Omega, a=K². The new executable
interval-bounds every relevant rational coefficient: J=f e^{-sx}F_y
coefficients and derivatives0..2 on1..4 are <1e10; static mathfrakF
coefficients and derivatives0..1 divided by r6 on4..infinity are <1e10.
These bounds include nonzero trace/divergence factors2/a and2/K.

With Qb=1e110, on the exterior |mathfrakF|<=1e12 Qb r8 e^{-(r-4)}.
On the compact collar |mathfrakF|<=4e^{4s}Cj delta^{s+1},
Cj=7e10 Qb. Thus the whole-line L2 bound is evaluated using

\[
\|\mathfrak F\|_2^2\le {4\over3}(10^{12}Q_b)^2
 \sum_{j=0}^{16}{16\choose j}4^{16-j}{j!\over2^{j+1}}
 +64e^{8s}C_j^2{3^{2s+2}\over2s+2}.
\]

This encloses both endpoints, not a truncated grid quadrature. The
independently checked historical positive supersolution proves H_a>=0
at a>=3/2, covering the exact k. The compatible separated Green solution
then has

\[
\|\mathfrak F\|_2<10^{128},\quad
\|u_{+2}\|_2<5\,10^{129},\quad \|u_{+2}\|_\infty<2\,10^{130}.
\]

The last estimate uses sqrt[2s^-2(s^-1+(5/2)s^-2)]||F||2.
These huge bounds are now fed by an enclosed physical source. They
bound the exact point-inverse master, not a retarded metric propagator,
the old cubic response error, or M2. The signed kernel remains
-uH(x<)uI(x>)/W, W<0; both Green integrals and the forced
equal-exponential tail are retained in the exact definition.

Two bounded routes to the full metric were investigated. Direct
historical reconstruction fails the newly measured all-seven residual.
The second route, certified absolute source-to-exact-inverse estimates,
closes the scalar bounds above but does not evaluate/independently verify
the full affine EF lift constant, its differentiated compatibility error,
the zero-mode scalar and charge-fixed mass/lapse bounds in one norm,
or the family's homogeneous data. Those are not replaced by the scalar
numbers. The first remaining full-geometry inequality is an evaluated

\[
\|q_{2,2}\|_{X_0}\le C_{2,0}\|Q_{+2}\|_{Y_0},
\]

or a proved/evaluated anisotropic replacement, together with the zero
sector and finite-data terms. Existence of an effective historical
algorithm does not certify our unevaluated number. This is a bounded
partial outcome, not proof that the exact geometry cannot be enclosed.

## 5. Radiation and physical scope

No C0 overlap was newly enclosed. The historical12.30729197 remains
empirical. Even an enclosed C0 would only give the particular leading
coefficient 2 Omega_hat C0²(A2⁴-A1⁴). For finite data, homogeneous scalar
news interferes; the energy is (2/r0)||Npart+Nhom||². Particular-matched
and zero independent scalar data differ by a genuine homogeneous field.
Neither a mass integration constant nor a static tension shift cancels
that freedom. Global exact data, nonlinear propagation/trace and the
physical fourth-order news remainder remain open. No physical A*,
kinematic full-quadratic amplitude interval, or first-bulge E/M follows.
Exact SO(3) excludes tensor Bondi news but permits the massless radion;
stabilization remains a separate EFT. The failed few-mode model is not used.
