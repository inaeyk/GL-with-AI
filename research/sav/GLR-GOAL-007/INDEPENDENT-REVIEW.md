# Independent Schwarzschild verification

The initial reconstruction used the stated background and first jets and
GOAL-005's REPORT, DERIVATIONS and RADIATION from source
61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708. The prior audit object
a9bfbec0efe41b881e4b12aa262f5fcfcd94c639 was unavailable in the local
store during reconstruction. No primary derivation or investigator result
was supplied. These are analytic checks with no measured GL–tensor
waveform.

## Complete source and constraints

Use ordinary Taylor coefficients \(G=\bar G+\epsilon a+\eta b+
\epsilon^2 c+\epsilon\eta d+\epsilon^2\eta e+\cdots\).
For the full Einstein tensor let \(L=E'_0\), \(Q=E''_0/2!\),
and \(T=E'''_0/3!\) be symmetric multilinear maps. Then
\[
 La=Lb=0,\quad Lc=-Q(a,a),\quad Ld=-2Q(a,b),\quad
 Le=-2Q(a,d)-2Q(b,c)-3T(a,a,b).
 \tag{V1}
\]
Independently expand the connection difference
\[
 C^A{}_{BC}=\tfrac12 G^{AD}
  (\bar\nabla_B h_{DC}+\bar\nabla_C h_{DB}-\bar\nabla_D h_{BC}),
 \qquad h=G-\bar G ,
\]
\[
 R_{BC}=\bar R_{BC}+\bar\nabla_A C^A{}_{BC}-\bar\nabla_C C^A{}_{BA}
  +C^A{}_{AD} C^D{}_{BC}-C^A{}_{CD} C^D{}_{BA}.
 \tag{V2}
\]
Use the ordered inverse series
\(G^{-1}=\bar G^{-1}-\bar G^{-1}h\bar G^{-1}
+\bar G^{-1}h\bar G^{-1}h\bar G^{-1}+\cdots\) for the connection
and Einstein trace. The quadratic connection products at \((2,1)\)
use \((10,11),(11,10),(01,20),(20,01)\). This is an all-component
recipe with the same combinatoric factors as (V1).

The background divergence of each Einstein source vanishes if all
contributing lower orders solve their equations, by the expanded
contracted Bianchi identity. The normal projections
\(E_{AB}n^An^B\) and \(E_{AB}n^A q^B{}_i\) must hold on the initial
slice with the forced coefficients. Corrections to the normal and
projector multiply lower Einstein coefficients that already vanish.
Bianchi propagation leaves the free radiative pairs undetermined.
Zero finite-slice data for every raw metric component of a reduced
evolution generally violate the sourced constraints.

Circle momenta add even when derivatives act on the harmonics. Angular
products lie in the Clebsch–Gordan tensor product and preserve the
product of parity eigenvalues. A scalar rotational monopole times a
single \(\ell=2\) representation preserves its \(\ell,m,\) and parity.
The circle sectors at \((2,0),(1,1),(2,1)\) are
\(\{0,\pm2\},\{\pm1\},\{0,\pm2\}\). The zero projection keeps the
\(1+(-1)\) channel and kills \(1+1\). Half-period translation acts as
\(\epsilon\mapsto-\epsilon\) on an equivariantly prepared family, so
odd powers of \(\epsilon\) vanish in its zero mode. If a particular
\((2,1)\) coefficient vanishes, \((4,1)\) is the next symmetry-allowed
order at fixed first power of \(\eta\). Exact rotational symmetry at
\(\eta=0\) excludes tracefree rank-two news. The \(\ell=0,1\) tracefree
rank-two sphere harmonics vanish identically.

## Nonlinear gauge control

For a formal generator \(X=\sum\epsilon^i\eta^jX_{ij}\) extract the
coefficients of \(\exp(\mathcal L_X)G\) without commuting Lie
derivatives. At \((1,1)\) the addition to \(d\) is
\[
 \mathcal L_{X_{11}}\bar G+\mathcal L_{X_{10}}b+
 \mathcal L_{X_{01}}a+
 \tfrac12(\mathcal L_{X_{10}}\mathcal L_{X_{01}}+
 \mathcal L_{X_{01}}\mathcal L_{X_{10}})\bar G .
\]
At \((2,1)\) this likewise takes the ordered Lie words acting on
\(\bar G,a,b,c,d\) and totaling the target degree. A linear
RW/Zerilli invariant of raw \(e\) only removes the single
\(\mathcal L_{X_{21}}\bar G\) contribution. As a negative control
use \(G=\exp(\epsilon\mathcal L_X)(\bar G+\eta b)\) with periodic
\(X\) vanishing near infinity. Then \(a=\mathcal L_X\bar G\),
\(d=\mathcal L_X b\), \(c=\tfrac12\mathcal L_X^2\bar G\),
\(e=\tfrac12\mathcal L_X^2b\). The complete mixed news vanishes in
the same Bondi frame even if raw metric pieces and sources are
nonzero. Transform the initial slice and all data together in this
test.

Circle reflection of the standing mode removes zero harmonics with
one \(z\) index. In a decoupled asymptotic region the Einstein frame is
\(g^E_{\mu\nu}=G_{zz}^{1/2}(G_{\mu\nu}-G_{\mu z}G_{\nu z}/G_{zz})\)
with \(\varphi=\sqrt3\log(G_{zz})/4\). KK products must enter before
the circle average or be shown to decay sufficiently to commute
those operations in the extraction limit. The zero-mode scalar
stress starts at \(\epsilon^4\) since the leading scalar is
\(O(\epsilon^2)\). Its news alone cannot source the \((2,1)\)
tensor coefficient through that stress.

## Exact axial hierarchy

A different implementation uses the exact axial sector at first
order in \(\eta\) on the angularly round GL geometry
\[
 G(\epsilon,0)=B_{ab}(\epsilon,y)dy^a dy^b+R(\epsilon,y)^2d\Omega^2,
 \qquad y^a=(t,r,z).
\]
Assume the background vacuum equations through \(\epsilon^2\).
For \(X_A=-\varepsilon_A{}^B D_BY_{\ell m}\) and
\(X_{AB}=D_{(A}X_{B)}\), write \(\delta G_{aA}=j_aX_A\),
\(\delta G_{AB}=h_2X_{AB}\). The angular shift
\(\xi^A=f(y)X^A\) adds \(R^2\partial_af\) to \(j_a\) and
\(2R^2f\) to \(h_2\). The exact linear-in-\(\eta\) completion is
\[
 A_a=j_a/R^2-\tfrac12\partial_a(h_2/R^2),\qquad F=dA .
 \tag{V3}
\]
Base coordinate changes act by pullback on \(A,B,R\). For
\(\lambda=(\ell-1)(\ell+2)>0\), the vacuum equations are
\[
 \partial_b(\mathcal D^{ba\,cd}F_{cd})-\lambda\mathcal M^{ab}A_b=0,
 \quad \mathcal D^{ba\,cd}=\sqrt{|B|}R^4B^{bc}B^{ad},\quad
 \mathcal M^{ab}=\sqrt{|B|}R^2B^{ab}.
 \tag{V4}
\]
This follows by substituting the vector-type invariant \(F_a/R=A_a\)
in the general warped-product equations, with base dimension three
and sphere dimension two. The partial divergence gives the angular
constraint \(\partial_a(\mathcal M^{ab}A_b)=0\) by antisymmetry of the
density contracted with \(F\). The time component of (V4) is the
Proca Gauss constraint. The angular classification “vector-type”
contains ordinary four-dimensional odd tensor radiation in its
zero-KK branch. [Kodama–Ishibashi–Seto §II.B.2, (31)–(36)](https://arxiv.org/pdf/hep-th/0004160v3)

Expand \(B=\bar B+\epsilon B_1+\epsilon^2B_2\),
\(R=r+\epsilon R_1+\epsilon^2R_2\), and
\(A=A_0+\epsilon A_1+\epsilon^2 A_2\).
Let \(\mathcal L_j A=\partial_b(\mathcal D_j^{ba\,cd}(dA)_{cd})
-\lambda\mathcal M_j^{ab}A_b\). The complete odd mixed and cubic equations are
\[
 \mathcal L_0 A_1=-\mathcal L_1 A_0,\qquad
 \mathcal L_0 P_0 A_2=-P_0(\mathcal L_2 A_0+\mathcal L_1 A_1).
 \tag{V5}
\]
For the explicit matrices use \(H_j=\bar B^{-1}B_j\) and
\[
 {\sqrt{|B|}\over\sqrt{|\bar B|}}=1+\tfrac12\epsilon\operatorname{tr}H_1+
 \epsilon^2[\tfrac12\operatorname{tr}H_2-\tfrac14\operatorname{tr}(H_1^2)
 +\tfrac18(\operatorname{tr}H_1)^2],
\]
\[
 B^{-1}=[I-\epsilon H_1+\epsilon^2(H_1^2-H_2)]\bar B^{-1},
 \quad R^j=r^j[1+j\epsilon R_1/r+
 \epsilon^2(jR_2/r+\tfrac12j(j-1)R_1^2/r^2)] .
\]
These include the forced divergence and all gauge-completion products
in (V3). Only \(P_0(B_2,R_2)\) enters \(P_0\mathcal L_2 A_0\).
The driven \(n=\pm1\) system retains the compact component. Reflection
removes \(P_0 A_{2,z}\).

For a source \(S^a\) on the right of the projected Proca equation,
set \(f=1-2M/r,\ U=r^4F_{tr}\) and orient the two-dimensional base
by \(dt\wedge dr\). Its two components give
\[
 A_t={f\over\lambda r^2}(S^t-U_r),\qquad
 A_r=-{1\over\lambda r^2f}(S^r+U_t).
\]
Taking the curl and setting \(\Psi=-2U/(\lambda r)\) yields
\[
 [-f^{-1}\partial_t^2+\partial_r(f\partial_r)
 -\ell(\ell+1)/r^2+6M/r^3]\Psi
 =-{2\over\lambda}\big[(rf)^{-1}\partial_t S^r
   +r\partial_r(fS^t/r^2)\big].
 \tag{V6}
\]
Here \(S=-P_0(\mathcal L_2 A_0+\mathcal L_1 A_1)\) for the cubic
problem and zero at linear order. The reconstruction returns
both component equations and the associated divergence. One uses
a regular horizon chart or the tortoise equation obtained by
multiplying (V6) by \(f\) for evolution. The background potential and
CPM normalization agree with [Martel–Poisson §V, (5.13)–(5.18)](https://arxiv.org/pdf/gr-qc/0502028v1).

## Retarded trace, flux and free data

With finite compatible data \(d\) the solution is
\(\Psi=\mathcal U_\Sigma d+G_{\rm ret}F\), where \(F\) is the right
side of (V6) times \(f\) in tortoise coordinates. The mixed
response in (V5) is a constrained matrix hyperbolic evolution
from a finite slice. These equations specify the exact iterated
retarded functional once the prepared \(B_2,R_2\) and higher-order
radiative data are supplied. An isolated QNM does not provide the
finite-energy initial data. Laplace shifts from the growing GL
factors leave finite-slice boundary terms in the transform.

In a regular Bondi end, with source decay sufficient for the
asymptotic constraints and differentiated trace, the completed master
limits are the shear coefficients in the same fixed Bondi frame.
For \(g^E_{AB}=r^2q_{AB}+rC_{AB}+O(1)\) use
\(C_{AB}=\sum(\Psi^{\rm e}_{\mathscr I}Y_{AB}+
\Psi^{\rm o}_{\mathscr I}X_{AB})\) and \(N_{AB}=\partial_u C_{AB}\).
The raw perturbative trace at fixed Schwarzschild cuts can fail
even for a constant mass shift \(\delta M\). Its physical time
is \(u_B=u_0-\epsilon^2\delta M\,\partial_M r_*+\cdots\), with
\(\partial_M r_*\sim 2\log r\). Comparing matched Bondi cuts
removes this logarithm. Changing the mass family also changes
the physical linear response by \(\delta M\,\partial_M N_{01}\)
with matched data. A constant clock-origin shift gives a multiple
of \(\partial_uN_{01}\). A common Bondi frame is needed since BMS
changes may also mix multipoles.

With \(G_4=G_5/L\) and unit-normalized scalar harmonics,
\(\|Y_{AB}\|^2=\|X_{AB}\|^2=(\ell-1)\ell(\ell+1)(\ell+2)/2\).
Integrating the scalar Hessian squared and using the unit sphere
Ricci tensor gives this norm. For \(\ell=2\) it is 12. The
tensor flux is \(\int N_{AB}N^{AB}d\Omega/(32\pi G_4)\). The
interference coefficient at \(\epsilon^2\eta^2\) is therefore
\[
 {1\over16\pi G_4}\int N_{01}^{AB}N_{21,AB}d\Omega
 ={24\over32\pi G_4}\sum_{m,p}
   \operatorname{Re}(\dot\Psi_{01,\mathscr I}^{p*}\dot\Psi_{21,\mathscr I}^p).
 \tag{V7}
\]
Its sign is data-dependent. A nonzero \(N_{21}\) can have zero
interference on a specified interval. The radion and massive flux
are separate. [Martel–Poisson §VI, (6.12)–(6.16)](https://arxiv.org/pdf/gr-qc/0502028v1)

At fixed first-order data, ADM mass, circle length and frame, any
smooth compact \(n=0,\ell=2\) homogeneous master datum may still
be added to \(e\). Vacuum reconstruction satisfies the target
constraints and changes no lower coefficient or linear monopole
charge. Its outgoing traces are not confined to the span of the
mass and time derivatives. For example take outward compact
packets supported far from the horizon with distinct earliest
arrival cuts. Over each fixed-width early window, the RW
characteristic integral differs from the flat response by
\(O(R^{-1})\) at initial radius at least \(R\). This follows from
the integrated \(O(r^{-2})\) potential in the characteristic
diamond and Volterra iteration. Smooth derivative data give the
same estimate for the news. Finite propagation gives exact zeros
before each packet's first arrival. Nonzero responses in their
respective first windows yield arbitrarily many linearly
independent traces. This free-data obstruction survives removing
the two nuisance derivatives. Compatible free data at \((1,1)\)
can additionally affect the mixed response through later conversion.

This obstruction is proved for formal constrained jets. Integrating a
given jet to an exact global family and differentiating the exact
radiation map at fixed Bondi cuts are further steps. A retarded
propagator specifies source causality. A prescription for the free
radiative pairs is necessary to isolate its induced waveform.
The obstruction does not give the sign or vanishing of a specified
retarded source projection.

## Exact nonzero local control on the physical GL input

The weighted divergence in (V4) is
\(\nabla_B^a A_a=-2(\nabla_B^a\log R)A_a\).
Using this in the vector equation gives a covector wave system with
principal operator \(B^{ab}\partial_a\partial_b\) on each component.
The eliminated divergence contributes only first derivatives of \(A\).
Commuting the covariant derivatives produces an algebraic base
curvature term. The remaining terms from \(R^4\) and \(\lambda\) have
at most one derivative of \(A\). This is a consequence of the full
constraint system, so it can be used as a necessary test of the
claim that the mixed axial response is identically zero.

In the unique first-order nonzero-KK unitary representative, take
\(x=r_*\), \(u=t-x\) and \(L=\partial_t+f\partial_r\). In the source
baseline use the displayed GL master reconstruction in
[gl_linear_mode.md](../physics/gl_linear_mode.md), with
\(D=1+k^2r^3\), \(y/D=U_{\rm GL}/r\), and
\(q=(k^2+\Omega^2)^{1/2}\). Since \(y'/y=-q+O(r^{-1})\), it gives
\[
 (H_{tt},H_{tr},H_{rr})=
 {y\over k^2D}\left((-q^2,\Omega q,-\Omega^2)+O(r^{-1})\right),
 \qquad H(L,L)={y\over k^2D}\left(-(q-\Omega)^2+O(r^{-1})\right).
 \tag{V8}
\]
The nontrivial decaying physical mode thus gives \(H(L,L)\ne0\)
at sufficiently large radius, since \(k>0\). This statement is a
leading-asymptotic sign result, with no numerical threshold radius
asserted.

At a fixed sufficiently far point on a finite initial slice choose
compact odd CPM data with a nonzero cubic Taylor jet of \(\Psi\) in
\(x\), lower jets zero, and outgoing velocity
\(\Psi_t=-\Psi_x\) locally. One can use a static-time slice in this
neighborhood and extend it to a regular slice through the horizon.
The source-free RW equation gives zero first and second spacetime
jets of \(\Psi\) at that point, and its third derivative tensor is
proportional to \(du\otimes du\otimes du\). This follows directly by
differentiating the wave equation because all potential terms there
involve lower vanishing jets. Reconstruction by (V6) gives
\(A_0=\partial A_0=0\) at the point and
\(\partial_a\partial_b A_{0,c}\) proportional to \(u_a u_b u_c\),
with nonzero coefficient for \(c=t\). Explicitly its first-jet
differential part is
\[
 A_{0,t}={1\over 2r}\partial_x\Psi+{f\over2r^2}\Psi,\qquad
 A_{0,r}={1\over2rf}\partial_t\Psi .
\]
Only the variation of the principal symbol survives in the mixed
covector wave equation there. Its contraction is proportional to
\[
 [\delta B^{ab}(du)_a(du)_b]\, (du)_c
 =-f^{-2}e^{\Omega t}\cos(kz)\,H(L,L)(du)_c ,
 \tag{V9}
\]
which is nonzero in the \(n=\pm1\) sector. Thus these finite
ordinary tensor data force a genuine mixed angularly gauge-invariant
axial response in the GL unitary frame. A purely angular odd
gauge perturbation has \(A=0\) exactly and fails this control. A
pure \(n\ne0\) GL gauge generator has vanishing completed \(H\)
and also fails the control. This is a local source evaluation on
the physical GL tail and neither a direct \(n=0\) cubic source
evaluation nor a nonzero outgoing Green-function projection.

A possible continuation is an outgoing geometric-optics packet
with its launch phase anchored on the initial slice. The first
eikonal variation is a line integral of \(H(L,L)\) along the
unperturbed outgoing ray. Its circle mean vanishes, while the
mean of its square is positive on sufficiently far rays by
(V8). In the high-frequency expansion at a *fixed finite
extraction radius*, the mean-square term has an extra power of
frequency compared with the second eikonal variation. This does
not control the differentiated zero-mode radiation limit. A proof
must bound the contribution of the massive intermediate field in
the infinite characteristic wedge and the asymptotic completion
in the same clock before applying this power count at null
infinity.

## Non-blind follow-up audit of the early outgoing front

After the preceding independent reconstruction, the coordinator disclosed
the proposed finite-slice axial front proof, including its leading
coefficients \(-\chi\) and \(\langle\chi^2\rangle/2\), the fixed-mass
short-range direct term, and the use of smooth approximation. This
follow-up tests that specific proof. It is independent checking, with
advance knowledge of its target.

**Axial reduction and initial constraints.** Eliminate the weighted
divergence from (V4) on the full warped product. The resulting second-order
system for \(A_a\) has scalar principal symbol \(B^{ab}\zeta_a\zeta_b\).
The eliminated divergence gives \(2\nabla_a[(\nabla^b\log R)A_b]\),
and the \(R^4\) weight gives \(4(\nabla^b\log R)(\nabla_bA_a-\nabla_aA_b)\).
Thus the variation of the principal symbol commutes with the background
matrix transport on any axial polarization. With \(u=t-x,v=t+x\),
\(\bar B=-f\,du\,dv+dz^2\) and \(\delta B^{uu}=-H(L,L)/f^2\) times the
GL time and Fourier factor. The sign in the leading transport is
\(-\chi\), where
\[
 \chi(u_*,v,z)=\int_{v_*}^v
 {e^{\Omega t}\cos(kz)H(L,L)(r)\over4f(r)}\,dv'.
\]
The leading double insertion is \(+\chi^2/2\). It is essential to
use the weighted constraint before reading off the principal symbol.

As free axial data one can prescribe \(A_i\) and the electric density
\(\mathcal D^{it\,cd}F_{cd}\) on the initial slice in the matched chart.
The Gauss equation determines \(A_t\) algebraically from the spatial
divergence of this density and from \(A_i\), since
\(\mathcal M^{tt}\ne0\). The remaining first time derivatives follow
from \(F_{ti}\) and the weighted divergence. Fixing these spatial
pairs as functions of \(\epsilon\) makes their induced temporal
completion at most as singular as the original \(A\), with velocities
at most one derivative rougher. Consequently the initial terms in the
order-one and order-two solutions do not generate the more singular
scattered fronts. A null line \(v=v_*\) through the launch point
instead of the spacelike slice adds a local triangle of length
\(O(u-u_*)\) in \(v\), so this change of initialization does not
generate either highest front coefficient.

**Decay and the differentiated trace.** In the fixed Bondi-mass
mean sector the Einstein-frame spherical radial constraints have an
exponential source on a sufficiently early bounded \(u\)-strip:
the first GL field is bounded there by a power of \(r\) times
\(\exp[-(q-\Omega)r]\), and its quadratic source by its square times
a power. The linear spherical constraints integrate from the normalized
lapse and mass at infinity to short-range coefficients. This argument
uses the mean metric in Einstein-frame areal/Bondi coordinates.
For the odd projected equation, the radion cancels algebraically:
if \(B_5=e^{-2\varphi/\sqrt3}B_E+e^{4\varphi/\sqrt3}dz^2\)
and \(R_5=e^{-\varphi/\sqrt3}r_E\), then
\(\sqrt{|B_5|}=\sqrt{|B_E|}\), while the inverse factors cancel the
radion factors in both \(\mathcal D^{ba\,cd}\) and \(\mathcal M^{ab}\)
for two-dimensional base indices. This is an exact zero-mode
vacuum reduction. At order \((2,1)\) the remaining nonzero-mode
products belong to the short-range scattering terms.

For the short-strip characteristic estimate, use \(W_a=r A_a\),
\(J=\partial_u W\), \(I J=\int_{u_*}^u J\,du'\). For a coefficient
equation in fixed Fourier sector the reduced wave system has the
form \(\partial_v J=M J+N I\partial_vJ+P I J+S\),
where \(M,N=O(r^{-1})\) and \(P=O(1)\), including the constant
mass term for nonzero KK number. The strip width \(\delta\) bounds
the norm of \(I\). Inversion of \(1-NI\) on the strip and Gronwall
give at most polynomial times \(\exp(C\delta v)\) growth, with
the same exponential rate after fixed-order differentiation by
induction over lower derivatives. Choosing \(\delta\) sufficiently
small relative to \(q-\Omega\) makes the rescattered GL sources
integrable with all the finitely many derivatives needed for
the front comparison. The one-sided derivatives of the
polynomial front may be taken from \(u>u_*\).

There is a stronger estimate for the projected massless trace.
Reflection removes \(W_z\) at \(n=0\). In the background reduced
wave equation multiplied by \(f\), the only \(O(r^{-1})\)
derivatives in the two-component \(W=(W_t,W_x)\) system are
\((2f/r)(\partial_x W_t-\partial_tW_x)\) in the \(t\) equation.
In the \(x\) equation the \(2(\nabla_x[(\nabla^b\log R)A_b])\)
term cancels the first derivative from rescaling by \(r\).
All other lower-order coefficients are \(O(r^{-2})\) or better.
For \(a=f/(2r)\), this gives
\[
 \partial_vJ_t=-a(J_t+J_x)+a I(\partial_vJ_t-\partial_vJ_x)
  +O(r^{-2})(J,IJ,I\partial_vJ)+S_t ,
 \qquad
 \partial_vJ_x=O(r^{-2})(J,IJ,I\partial_vJ)+S_x.
\]
The \(a I\) terms give an \(O(\delta r^{-2})\) perturbation of
the \(-a\) term after Volterra inversion. Hence \(J_t+J_x\)
has a nonpositive damping term and \(J_x\) has an integrable
\(v\) derivative whenever the forcing is short-range.
This closes uniform sup bounds by an integrable-coefficient
Gronwall inequality. A fixed number of \(u\) derivatives obey
the same estimate by induction: \(u\) derivatives of \(a\) are
\(O(r^{-2})\), and derivatives on \(I\partial_vJ\) are lower
jets already controlled, multiplied by \(a\).
It follows that the trace of \(W_x\) exists with the required
finite derivative control. Its connection with news is exact:
\[
 W_x=r f A_r=\tfrac12\partial_t\Psi-\frac{S^r_{\rm Proca}}{\lambda r}.
\]
The last term has vanishing trace for the short-range sources.
The ordinary shear normalization in (V7) then identifies
\(2W_x|_{\mathscr I}\) with the perturbative news coefficient.
This route uses the common Bondi clock and avoids inferring
the massless trace from a massive bound.

**Front orders and smoothing.** For a first-order CPM front
\(F_m=(u-u_*)_+^m/m!\), \(W_x\) has leading front \(F_{m-1}/2\)
and \(W_t\) its negative. This polarization vector is exactly
transported at leading order by the Schwarzschild system, by
the source-free CPM reconstruction. The first symbol insertion
gives the vector times \(-\chi F_{m-2}\), and its repetition gives
the vector times \(\chi^2 F_{m-3}/2\). Derivatives with at most one
\(u\) derivative on an input and a single direct order-two insertion
are at least one front order smoother than the double term.
The integrable differentiated estimates above preserve the
ordering at the trace. A sufficiently far front has
\(\langle\chi_\infty^2\rangle>0\) by (V8). The two news terms
therefore have a positive product on some sufficiently short
right neighborhood of the front. This gives positive integrated
tensor interference on an interior subinterval for these data.

The front datum is compactly cut off behind the front, and \(m\)
can be chosen large enough for convergence after all finite
derivative losses in this construction. One-sided smoothing
preserves causal support of the approximations. The finite
wedge and uniform short-range estimates give continuity of the
trace on fixed cuts in the corresponding finite regularity
norm. Nonzero news and nonzero interference on a fixed interval
then persist for sufficiently close smooth data. A smooth constant
clock variation or matched constant mass variation of the
unscattered front produces at worst \(F_{m-2}\) in its news,
and cannot cancel the \(F_{m-3}\) coefficient. To make the
nonabsorption statement open under smoothing, either keep
mass fixed and use rank two for the waveform and its clock
variation, or arrange independent mass and clock directions
with two compact fronts at different launch areal radii.
For the second option, at fixed launch time the two values
\(-\partial_M r_*(M,r)\) differ, so the front coefficients
of the mass and time derivatives are linearly independent.
Earlier smooth tails do not affect the later front
singularity. Once the nuisance directions are independent,
choose finitely many continuous trace functionals with
nonzero rank determinant and preserve it under approximation.

These arguments concern a specified early, far exterior
finite-slice preparation. They do not evaluate the amplitude
of strong-field QNM residues, and they do not assert
uniform differentiability of the full exact nonlinear
Einstein radiation map. The direct term, the mass control,
the Gauss completion and the trace estimate are required
for the stated nonzero result.
