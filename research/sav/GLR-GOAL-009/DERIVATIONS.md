# Exterior, scalar and optical derivations

All estimates below use \(r_0=1\). Restore dimensions by rescaling
\(r,t,u\) by \(r_0\) and \(\Omega,k\) by \(r_0^{-1}\).
Use coefficients of powers of the seed without factorials.

## 1. Weighted causal massive jets on the reference exterior

Write \(x=r+\log(r-1)\), \(f=1-1/r\), \(T=\partial_t\),
\(L_+=\partial_t+\partial_x\), \(u=t-x\), and
\(\tau=t+x-r\). Set \(u_A=A-2x(A)\) with \(A>2\).
On \(\mathcal W_A=\{\tau\ge0,u\le u_A\}\) one has
\[
 \tau=u+2x-r,\qquad r\ge A+c_A\tau,\qquad
 1-u\le C_A r,\qquad dt/dx|_\tau=-1/r .
\]
The first inequality follows from the inner outgoing ray, along which
\(d\tau/dr=2/f-1\). The second uses the lower bound \(\tau\ge0\).
The inner boundary is characteristic. Finite past triangles and domain
of dependence suffice to construct a linear solution on the entire
wedge from data on \(\Sigma\cap\{r\ge A\}\).

Consider the real component system
\[
 q_{tt}-q_{xx}+Vq=\mathsf A q_t+\mathsf B q_x+\mathsf Cq+S,
 \qquad V=fK^2,\qquad K\ne0 .                                      \tag{1}
\]
The background matrices are stationary and bounded by \(C_Kr^{-2}\)
with their required derivatives. Take data with finite weighted Sobolev
norms specified below and source with
\[
 \int_{\mathcal W_A}r^{p+2}(1-u)^b |T^j S|^2\,dt\,dx<\infty
\]
at the needed indices. For \(y=T^jq\), set
\(e_j=(|y_t|^2+|y_x|^2+V|y|^2)/2\), \(j_j=-(y_t,y_x)\) in
the Euclidean component norm. Stationarity gives
\(\partial_t e_j+\partial_x j_j=(y_t,\mathsf A y_t+
\mathsf B y_x+\mathsf C y+T^j S)\).
For \(p\) a nonnegative integer and \(b\ge1\), put
\(w=r^p(1-u)^b\). The exact weighted identity is
\[
 \partial_t(we_j)+\partial_x(wj_j)
 +b r^p(1-u)^{b-1}(e_j-j_j)
 =w(y_t,\mathsf A y_t+\mathsf B y_x+\mathsf C y+T^jS)
    +pf r^{p-1}(1-u)^b j_j .                                    \tag{2}
\]
Here \(e_j-j_j=(|L_+y|^2+V|y|^2)/2\). On a \(\tau\) slice the
flux is \(w(e_j+j_j/r)\,dx\), uniformly equivalent to \(we_j\,dx\).
On an outer incoming truncation \(v=t+x=\mathrm{const}\) the
outward flux is \(w(e_j+j_j)\,dx\) in absolute line measure. On any
outgoing truncation \(u=u_1\le u_A\) the outward flux is
\(w(e_j-j_j)\,dx\). Thus every artificial lateral boundary can be
discarded with the correct sign, or retained to estimate its trace.

Let \(E_{p,b,j}(\tau)\) be the weighted spacelike flux on a truncated
domain, and \(D_{p,b,j}\) the spacetime integral of the bulk in (2).
The matrix terms and the source estimate
\[
 |w(y_t,T^jS)|\le C r^{-2}we_j+Cwr^2|T^jS|^2
\]
cost at most \(\int C_K(A+c_A\tau)^{-2} E_{p,b,j}(\tau)d\tau\)
plus the finite source norm. This is integrable uniformly in the
truncation and time. The last term of (2) is zero for \(p=0\). For
\(p\ge1\), use \(y_x=L_+y-y_t\) and \(V\ge f(A)K^2\) to obtain
\[
 |j_j|\le C_K\big[(e_j-j_j)+(e_{j+1}-j_{j+1})\big] .
\]
The last term thus costs \(C_{p,b,K}(D_{p-1,b+1,j}+
D_{p-1,b+1,j+1})\). Induction in \(p\) and integrable Gronwall close
the weighted estimate without absorbing an unsigned radial multiplier
term into the same-order energy. The finite data/source list needed is
\((p-i,b+i,j+\ell)\) for \(0\le i\le p,\ 0\le\ell\le i\).
Keeping the outgoing flux before sending both truncations out gives
uniformly for all \(u_1\le u_A\)
\[
 \int_{\mathcal W_A\cap\{u=u_1\}}
 r^p(1-u_1)^b\bigl(|L_+ T^j q|^2+|T^j q|^2\bigr)\,dx
 \le C_{p,b,j,A,K}.                                             \tag{3}
\]
The constants are homogeneous quadratic in the data/source norms.
The derivation also gives their spacelike sup and weighted spacetime
bulk bounds. The boundary \(u=u_1\) is a true null hypersurface of the
**background** five-dimensional product. The flux includes the
compact mass term and is distinct from massive energy through a
timelike tube.

Time differentiation of the Cauchy jet to the finite requested order
uses (1) and the noncharacteristic slice. For instance
\(\partial_\Sigma=\partial_x-r^{-1}\partial_t\) on component
functions. The first two derivatives from the prescribed normal
velocity and tangential derivative are obtained from a uniformly
invertible two-by-two system. At higher orders the principal
coefficient for the second \(\tau\) derivative is \(1-r^{-2}\).
Further transverse derivatives on the wedge follow from (1) and
\(\partial_x=L_+-T\), using spare time commutations and differentiated
sources. A local one-dimensional Sobolev bound on \(\tau=\mathrm{const}\)
and \([x,x+1]\), which remains in the exterior, now gives pointwise
\(O(r^{-p/2})\) for \(q\) and any preassigned finite derivatives if the
weighted bounds are available at enough orders. Every claim about
large \(p\) is for a prescribed finite order and an appropriately
chosen finite GOAL-006 damping parameter.

### Application to the actual Z quadratic jet

In the first-order unitary gauge \(h_{zA}^{(1)}=0\), the compatible
\(\pm 2k\) Cauchy jet is that of the actual damped data, transformed
with the first-order gauge and then with the second-order unitary
completion. In this last completion only nonzero \(K=2k\) is inverted.
The forced trace and divergence are \(T_2=-2Q_{zz}/K^2\) and
\(J_2=dT_2+2iQ_z/K\). The normally hyperbolic equation for the tensor
has source \(2Q_4+2\nabla_{(\mu}(J_2)_{\nu)}-\nabla_\mu\nabla_\nu T_2\).
The data are those of the differentiated vacuum development on
finite diamonds, so the homogeneous subsidiary equations propagate
the trace and divergence constraints. A separated particular solution
alone would supply different data.

To obtain (1) use \(q=r\) times the static orthonormal-frame SO(3)
components of the second-harmonic tensor. Along the \((t,r)\) base the
frame connection is \(O(r^{-2})\) in the time direction and vanishes
in the radial direction. For sphere derivatives, invariant components
have no angular derivative. The orbital-sphere frame mixing is
\(O(1/r)\); its squared action contributes \(O(r^{-2})\), and the
sphere connection within the angular block cancels by isotropy.
The traced Hessian correction from the sphere adds the scalar
radial term \(2f\,\partial_r/r\) before multiplying the wave equation
by \(f\). This is removed by the rescaling \(r\), leaving the scalar
\(fr^{-3}\) potential among the \(O(r^{-2})\) terms. The
Lichnerowicz curvature term is \(O(r^{-3})\). The compact principal
term gives precisely \(fK^2\). These estimates are away from the
horizon with \(r\ge A>2\).

At every fixed \(\lambda\), half-shift covariance gives zero first
seed derivative of the even \(\pm 2k\) projection. At \(\epsilon=0\)
the projection vanishes for every \(\lambda\). Smooth compact-slab
dependence in both parameters, followed by
\(\lambda(\epsilon)=O(\epsilon^2)\), therefore identifies the same
quadratic projection as at \(\lambda=0\), without a second derivative
of the tuned mass map. The differentiated GOAL-006 (A3) bounds,
divided by \(\epsilon^2\) and passed to this local coefficient, give
arbitrarily prescribed finite spatial weights when the damping is
chosen first. A seed expansion of the coordinate normal and the
linearized finite-jet evolution gives the same weights for the
Cauchy component velocity: the linear map of the constraint data
has bounded background coefficients on \(r\ge A\), and the
quadratic coordinate products use the exponentially decaying first
jet. One may prescribe initial lapse/shift in a regular local
hyperbolic gauge with this same falloff and the completed first
jet, then transform the second nonzero mode by the unitary formulas.
Finite spare derivatives cover this gauge transformation. The same
argument applies to the mixed radial/compact derivatives on the
tilted slice.

The curvature sources of the massive tensor use finitely many
derivatives and polynomial factors times \(e^{2\Omega t}H^2\).
Writing \(t=u+x\) with \(u\le u_A\), the Schwarzschild tail of the
GL jet gives exponential rate \(\sqrt{k^2+\Omega^2}-\Omega>0\)
in \(r\), with polynomial factors harmless for every finite norm.
It follows that (3) holds for the actual causal quadratic massive
jet, including its homogeneous correction, with the requisite
differentiation. The first jet has the stronger explicit exponential
control. This proves a reference-wedge estimate for the perturbative
coefficients, while the odd remainder of the **exact** evolution
still requires a nonlinear bound.

## 2. Exact compact scalar identity and Z data

For \(n=b^{-1}(\partial_z-X)\), \(K_z=\frac12\mathcal L_n h\) on
leaves and \(T_z=\operatorname{tr}_h K_z\), normal acceleration is
\(a=-\nabla_h\log b\). With spacelike unit normal,
\(\operatorname{div}_5 a=\operatorname{div}_h a-|a|_h^2
=-\Box_h b/b\). The normal Ricci identity gives
\[
 \mathrm{Ric}_{nn}=-nT_z-K_z:K_z-b^{-1}\Box_h b .
\]
For \(D=\sqrt{-\det h}/\sin\theta\),
\(\partial_z D-\partial_a(DX^a)=bDT_z\). Combining these gives
\[
 \partial_a(Dh^{ab}\partial_b b-DX^aT_z)+\partial_z(DT_z)
 = bD(T_z^2-K_z:K_z-\mathrm{Ric}_{nn}).
\]
Periodic integration yields the vacuum mean identity. At quadratic
order the unitary jet has \(b_1=X_1=T_{z,1}=0\).
The tensor contraction is
\(\langle K_{z,1}:K_{z,1}\rangle
=k^2 e^{2\Omega t}H_{\mu\nu}H^{\mu\nu}/8\).
With \(D_0=fr^2/2\) in background double-null coordinates,
this recovers the complete GOAL-005 source and the curvature
potential \(-f/(4r^3)\).

Choose \(\beta^z=0\) on the initial slice. The actual constraints
give \(\gamma_{zz}=1,\gamma_{zi}=0\) for \(i\ne z\),
and \(K_{zz}=0\). Thus \(g_{za}=0\) initially and the ADM evolution
gives \(\partial_\tau\gamma_{zz}=0\) there. The Schur complement
\(b^2=(g^{zz})^{-1}=g_{zz}-h^{ab}g_{az}g_{bz}\) has exactly
\(b=1,\ db=0\) on \(\Sigma\). The statement survives any recharting
with the same \(z\) coordinate, including \(z\)-dependent changes
of the two orbit coordinates. In common areal coordinates and a
subsequent common mean-null recharting the mean scalar difference
has exactly zero free jet when the initial slice is pulled back
accordingly. Physical circumference on tilted cuts still includes
the separate cut tangent and must be matched explicitly.

Put \(A^{ab}=\langle Dh^{ab}\rangle\), \(B_0=\langle b\rangle\), and
\(G^a=\langle(Dh^{ab}-A^{ab})\partial_b(b-B_0)-DX^aT_z\rangle\).
In a common mean double-null chart \(A^{uv}=-\rho^2\), the
rescaling \(\psi=\sqrt3\,\rho(B_0-1)/2\) gives
\[
 \psi_{uv}=(\rho_{uv}/\rho)\psi+
 \frac{\sqrt3}{4\rho}
 \bigl[\partial_aG^a-\langle bD(T_z^2-K_z:K_z)\rangle\bigr].
\]
The GOAL-006 Volterra argument needs ray-integrable forcing and
potential bounds in this common chart, with the initial slice
identification and quadratic subtraction performed there. The
weighted result in §1 supplies arbitrarily high polynomial
integrability for the actual leading and quadratic massive
coefficients on the reference geometry. It supplies no uniform
replacement for the exact \(G^a\) or for \(\rho_{uv}/\rho\).

## 3. Nonlinear and optical routes

The relative hyperbolic attempt and the geometric characteristic
system are documented in [EXTERIOR.md](EXTERIOR.md) and the
[independent characteristic reconstruction](checks/). The exact
radial equations determine lapse, compact shift and mass aspect
up to integration functions once the scalar shape and chart are
known. The compact shift equation and its supplementary conditions
must be retained even under reflection, which only sets its mean
to zero. [OPTICAL.md](OPTICAL.md) derives the terminal comparison
and the cancellation from the \(rr\) vacuum constraint.

## 4. Quadratic matching and the nonzero finite-anchor correction

This calculation is at the level of jets of the metric on the
reference exterior. In the completed first-order gauge, use
\((u,r,z)\) with \(u=t-x(r)\). Write a normalized linear optical
label as \(U=u+\epsilon U_1+\cdots\), with \(U_1\to0\) at fixed
\(u\) as \(r\to\infty\). Since \(\bar g^{ur}=-1\) and
\(\delta g^{uu}=-h_{rr}^{(u,r)}\), the linear eikonal equation is
\[
 \partial_r U_1=-h_{rr}^{(u,r)}/2,\qquad
 U_1(u,r,z)=\tfrac12\int_r^\infty h_{rr}^{(u,r)}(u,s,z)\,ds .
\]
It and its finite derivatives converge exponentially by the exact
GL tail, and \(U_1\) is odd under half translation. This is the
first-order terminal coefficient, without an assertion about the
terminal map at finite seed.

Fix \(U=u\) and \(r_5=r\) as labels of a circle. Parameterize it
by the original \(z\). At first order its orbit coordinates in
the static chart are \(t+\epsilon \xi^t,r+\epsilon \xi^r\),
with \(\xi^r=-\delta r_5\) and \(\xi^t=f^{-1}\xi^r-U_1\).
Because the completed first-order metric has \(h_{zA}=0\),
the first length correction is zero. Pullback of \(g\) to
the circle gives the quadratic coefficient
\[
 (\ell/L)_2=
 \frac12\langle h^{(2),U}_{zz}\rangle+
 \langle \xi^r_z(U_1)_z\rangle-\frac f2\langle (U_1)_z^2\rangle . \tag{4}
\]
This follows from
\(-f(\xi^t_z)^2+f^{-1}(\xi^r_z)^2
 = 2\xi^r_z(U_1)_z-f(U_1)_z^2\).
Terms involving second-order tilt or radius change have zero
linear coefficient because \(g_{zz}^{(0)}=1\) and
\(g_{za}^{(0)}=0\). Reparameterization of the compact circle does
not affect its circumference.

For the background circle anchored at \(u=g(R)\), \(r=R\) on
\(\Sigma\), the completed Z jet makes the first term in (4) zero.
The tilted geometric circle at nonzero seed generally leaves
\(\Sigma\). The remaining terms can be evaluated on the actual
pinned GL tail. Put \(\nu=\sqrt{k^2+\Omega^2}\),
\(d=\nu-\Omega>0\), and \(Y=e^{\Omega t}U_{\rm GL}(r)\).
The leading tensor ratios in the pinned GOAL-005 waveform give
\[
 \xi^r=\tfrac14 Y\cos(kz)(1+O(r^{-1})),\qquad
 h_{rr}^{(u,r)}=-\frac{d^2}{k^2 r}Y\cos(kz)(1+O(r^{-1})).
\]
For the second equality use \(h_{rr}+2h_{tr}/f+h_{tt}/f^2\)
in static components. The ray integral has leading radial factor
\(d^{-1}\), since \(Y=e^{\Omega u}e^{-d r}
r^{\Omega-\beta}(C_\infty+O(r^{-1}))\) on fixed \(u\).
Thus \(U_1=-(d/2k^2r)Y\cos(kz)(1+O(r^{-1}))\), and at this
finite anchor
\[
 (\ell/L)_2=-\frac{d}{16R}Y^2(1+O(R^{-1}))<0
 \qquad(R\ \text{sufficiently large}).                         \tag{5}
\]
The factor \(k^2/2\) comes from averaging two sine derivatives.
Equation (5) concerns the length of the perturbed optical/areal
circle at the labels of the reference intersection. The calculation can use a short two-sided
local extension near the initial hypersurface, since the reference
labels can sample on both sides at nonzero seed. See the
[independent reconstruction](OPTICAL.md#actual-gl-tilt-at-the-background-slice-crossing).
The exact \(b\) jet on \(\Sigma\) remains zero. Assigning zero *geometric
length* data at that reference intersection would omit the
required tilt term.

On a fixed reference cut, \(r\) times both tilt terms in (4)
and their fixed-\(u\) derivatives tend to zero exponentially.
The coefficient of \(r_E\phi_{\rm len}\) is therefore the same
quadratic trace as the completed \(\psi_2\) on the reference
wedge. Multiplication by the perturbed radius and taking the log
do not affect its quadratic coefficient because the constant
and first length coefficients vanish. This establishes matching
of the quadratic coefficients and explains how a finite-radius
correction can coexist with it. It gives no error estimate uniform
in seed and radius for the exact circumference.

## 5. Conditional trace, parity and flux

For the mean-chart scalar trace let \(d_\psi=\psi_\epsilon-
\epsilon^2\psi_2\). The error forcing is
\(F_\epsilon-\epsilon^2 F_2+\epsilon^2(V_\epsilon-V_0)\psi_2\).
For an outgoing ray define \([F](u)=\int |F(u,v)|\,dv\) from the
matched Cauchy intersection. Gronwall on characteristic triangles
bounds \(\|d_\psi\|_\infty\) by the sum of the free-field sup norm
and the \(L^1_u\) ray forcing norm times
\(\exp(\|[V_\epsilon]\|_{L^1_u})\).
Integrating along outgoing rays bounds the \(L^2(I)\) derivative
error by the free derivative error, \(\|[F]\|_{L^2(I)}\), and
\(\|[V_\epsilon]\|_{L^2(I)}\|d_\psi\|_\infty\).
Uniform norms of this kind would yield the mean-chart result
when the forcing and free-field errors are quartic.

The exact half-circle translation maps the Z constraints at
\(\epsilon\) to those at \(-\epsilon\), including the mass
tuning, by covariance and uniqueness. Cauchy uniqueness gives
an isometry of the two developments wherever both are defined.
An optical construction with geometric boundary conditions and a
unique anchor inherits that covariance. The resulting circumference
is equal on matched cuts at every radius. Where the limiting
trace and its derivative exist, equality passes to those limits.
To infer a quartic remainder one needs independent uniform
estimates in the ray and trace norms. Evenness by itself allows
a cubic absolute-power remainder.

If \(u_B=u+O_{C^1}(\epsilon^2)\) on a padded interval, the
change of variables has uniformly bounded Jacobian and inverse.
Composition transports the \(L^2\) error by this bound. The
background \(n_Z\) has bounded first derivative there, so the
clock displacement and Jacobian change cost \(O(\epsilon^4)\)
when multiplying the quadratic coefficient. A corresponding
\(H^1\) comparison of geometric and mean traces is still required.

For a sufficiently regular asymptotically circle-independent end
the Einstein-frame reduction with
\(ds_5^2=e^{-2\phi/\sqrt3}ds_E^2+e^{4\phi/\sqrt3}dz^2\)
has scalar kinetic term \(-2(\nabla\phi)^2/(16\pi G_4)\).
It gives \(4\pi r_E^2 T_{uu}\to N^2/G_4\). Independently the
transverse strain has entries \((-2,-2,4)\phi/\sqrt3\), hence
squared news strain \(8N^2\). The Einstein–Hilbert flux integrated
on \(S^2\times S^1\) has coefficient
\(4\pi L/(32\pi G_5)\), giving \(N^2/G_4\).
The mean vector is absent by reflection and the trace-free
two-sphere tensor news vanishes by \(\mathrm{SO}(3)\).
These flux evaluations require asymptotic regularity of the
exact end. A bound on massive flux through outgoing cones is
a PDE estimate, separate from physical massive transport through
timelike tubes.

On dimensionless cuts let \(b_Z=\|n_Z\|_{L^2(I)}\).
Conditional on the geometric news theorem and the asymptotic
flux, the triangle inequality gives
\[
 2[\max(0,\epsilon^2 b_Z-C_I\epsilon^4)]^2
 \le E_I/M_0
 \le 2(\epsilon^2 b_Z+C_I\epsilon^4)^2.
\]
In particular the absolute error from
\(\mathcal K_Z\epsilon^4\) is bounded by
\(4b_ZC_I|\epsilon|^6+2C_I^2\epsilon^8\).
The conditional seed range is \(0<|\epsilon|^2<b_Z/C_I\)
within the exterior smallness range. On an earlier interval
\(I_A=[g(A+1),g(A)]\) with \(A\ge100\), the pinned far-tail
enclosure gives a separate positive \(b_Z\). On \([-10,-8]\)
the pinned numerical enclosure applies to the quadratic problem.

## 6. A radial normal form for the linear optical principal part

Use the local area chart and its exact radial constraints from
[checks/README.md](checks/README.md#full-optical-local-area-subsystem).
Linearize about the Schwarzschild string and restrict to the
mean-free field \(W=R\log b\). Assume enough radial decay for
the following inversions and impose zero terminal torsion.
The linear integrations give
\[
 P=-6R^{-2}\int_R^\infty W_z(s)ds,\qquad
 A=6R^{-1}\int_R^\infty W_z(s)ds
       -3\int_R^\infty W_z(s)\frac{ds}{s}.
\]
The scalar equation becomes
\[
 -2W_{uR}+W_{RR}-R^{-3}(D-1)^2W+(I-6\mathcal H)W_{zz}=0,
 \quad D=R\partial_R,\quad
 (\mathcal H h)(R)=\int_R^\infty(1/R-1/s)h(s)ds.                   \tag{6}
\]
The Schwarzschild term is the difference between
\((fW_R)_R-(f_R/R)W\) and \(W_{RR}\).
Differentiating the integral twice gives
\(\mathcal H=[D(D+1)]^{-1}\) with the terminal inverse.

For \(a=(p+1)/2>1\), \(\mathcal H\) is bounded on
\(L^2(R^p\,dR)\) on a half-line, with norm at most \(1/[a(a-1)]\).
To see this, set \(R=e^x\), multiply by \(e^{ax}\), and use
the one-sided translation kernel
\((e^s-1)e^{-as}\), \(s\ge0\). Thus \(I-6\mathcal H\) has
coercive symmetric part for \(p>5\). It is not self-adjoint
in this norm, as is seen by pairing test functions on disjoint
radial intervals in opposite orders. Coercivity of the symmetric
part alone does not justify a time-commuted mass energy.

There is an exact linear cancellation. For
\[
 F(D)=\frac{D(D-1)}{(D+2)(D+3)},\qquad Z=F(D-1)W,
\]
one has \(F(D)(I-6\mathcal H)=F(D-1)\).
Each factor \((D-\lambda)^{-1}\) is the terminal inverse
\[
 [(D-\lambda)^{-1}h](R)=-R^\lambda\int_R^\infty s^{-\lambda-1}h(s)\,ds .
\]
It is bounded in the stated weighted space when \(a+\lambda>0\).
In particular \(F(D-1)\) and its inverse are bounded for \(p>5\).
Take smooth functions with sufficient decay and derivatives
so all differentiated terms belong to their corresponding weights.
The commutation identities below follow either from this
integral or from \(D\partial_R=\partial_R(D-1)\) and
\(D R^{-j}=R^{-j}(D-j)\). Boundary terms vanish at infinity.

Apply \(F(D)\) to (6). It converts the mixed derivative to
\(-2Z_{uR}\), and the compact term to \(Z_{zz}\).
For the radial second derivative,
\(F(D-2)/F(D-1)=I-6[D(D-1)]^{-1}\), hence
\(F(D)W_{RR}=Z_{RR}-6Z/R^2\).
For the curvature correction use
\[
 \frac{F(D-3)(D-1)^2}{F(D-1)}
  =\frac{(D-3)(D-4)(D+1)(D+2)}{D(D-2)}
  =D^2-2D-11+\frac{24}{D(D-2)} .
\]
The resulting equation is
\[
 -2Z_{uR}+(fZ_R)_R+Z_{zz}
 -\left(\frac{6}{R^2}-\frac{11}{R^3}\right)Z
 -\frac{24}{R^3}[D(D-2)]^{-1}Z=0 .                             \tag{7}
\]
There is no nonlocal compact second derivative in (7).
The last term is a bounded radial integral of order zero
in the chosen weighted spaces, with an additional \(R^{-3}\)
factor. In the flat product the corresponding equation is local
with potential \(6/R^2\). This is an identity on each terminal ray.
It does not give local Cauchy data for \(Z\), since the transform
samples the whole outgoing ray, and the transformation only
linearizes the compact principal term about the fixed product.
A nonlinear estimate still needs a symmetrizer or normal form
controlling the compact derivative commutators of the radial
constraints, together with admissible terminal constants.
