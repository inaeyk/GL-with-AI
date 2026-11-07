# GOAL-011 independent audit E

## Independence and scope

Read-only reconstruction using the task assumptions and GOAL-007 at
0864d9d807de56cbdd6e64d215e6295508a3007f. The pinned README,
AXIAL-TRACE, REPORT and INDEPENDENT-REVIEW disclosed the earlier Proca
equation, dual normalization, upper-data freedom and qualitative pole
interpretation. A later comparison included the relevant INVESTIGATOR and
HIERARCHY excerpts. Those endpoint claims are reviewed with advance
knowledge. The quadratic action derivation, exact-density elimination and
explicit surjectivity proof for data-to-residue below are reconstructions.
No GOAL-011 investigator draft or code was read. The prior audit
46ec0208d3efdd0595813e8e2719e2319b5ee67c was unavailable to
GIT_NO_LAZY_FETCH=1 git cat-file -t. This audit is analytic and includes no
physical evolution or numerical residue.

## Axial equation from the Einstein action

Use signature \((-++++)\) and \(ds_0^2=B_{ab}(y)dy^a dy^b+
R(y)^2\gamma_{ij}d\theta^i d\theta^j\), with a smooth Lorentzian
three-dimensional orbit and a unit round two-sphere. Require the
background to solve vacuum through the spherical order used in the
linear-in-\(\eta\) expansion. For one odd harmonic \(X_i\), use
\(X_{ij}=\hat D_{(i}X_{j)}\), \(\ell\ge2\), \(\lambda=(\ell-1)(\ell+2)\),
\(\delta G_{ai}=j_a X_i\), \(\delta G_{ij}=h_2 X_{ij}\).
An angular shift \(\xi^i=\Lambda(y)X^i\) adds \(R^2d\Lambda\) to \(j\)
and \(2R^2\Lambda\) to \(h_2\), in the active pullback convention. Thus
\[
A=j/R^2-\tfrac12d(h_2/R^2),\qquad F=dA .
\]

Fix \(h_2=0\) and put
\[
ds^2=B_{ab}dy^ady^b+R^2\gamma_{ij}
(d\theta^i+A_aX^i dy^a)(d\theta^j+A_bX^j dy^b)
\]
into the Einstein action to quadratic order in \(A\). The quadratic
completion of \(G_{ab}\) here multiplies a background Einstein equation
and drops out on shell. In the horizontal frame \(e_a=\partial_a-A_aX\)
one has \([e_a,e_b]=-F_{ab}X\) and
\(K_{aij}=\tfrac12(\partial_a-\mathcal L_{A_a X})(R^2\gamma)_{ij}
=RR_a\gamma_{ij}-R^2 A_a X_{ij}\).
The scalar curvature terms after integration by parts that depend on
\(A\) are the horizontal curvature
\(-R^2|X|^2 F_{ab}F^{ab}/4\) and the fiber deformation in
\(K_aK^a-K_{aij}K^{aij}\). The \(A\) deformation is traceless so only
\(-|X_{ij}|^2 A_aA^a\) survives there. The volume factor is
\(\sqrt{|B|}R^2\sqrt{\gamma}\). Using transversality,
\(\hat\Delta X_i=-(\ell(\ell+1)-1)X_i\), and the sphere Ricci tensor,
\[
\int X_{ij}X^{ij}\,d\omega=\frac{\lambda}{2}\int X_iX^i\,d\omega .
\]
Thus the quadratic orbit action, divided by the common positive
\(\int |X|^2\), has Lagrangian
\(-R^4 F_{ab}F^{ab}/4-\lambda R^2 A_a A^a/2\). Its Euler equation is
\[
\partial_b(\sqrt{|B|}R^4F^{ba})-\lambda\sqrt{|B|}R^2 A^a=0 .
\tag{E1}
\]
Divergence gives \(\partial_a(\sqrt{|B|}R^2 A^a)=0\), which is also
the odd sphere-tensor equation by the angular gauge identity.
The normal component of (E1) is the initial Gauss constraint. On a
regular slice with future unit normal \(n\) and induced metric
\(\gamma_\Sigma\), let \(\pi^i=\sqrt{\gamma_\Sigma}R^4 F^{ib}n_b\).
Then \(n^aA_a=(\lambda\sqrt{\gamma_\Sigma}R^2)^{-1}\partial_i\pi^i\).
This follows in normal coordinates and is covariant under intrinsic
spatial changes. Fixing the smooth canonical pair on one finite slice
is one admissible preparation. Zero raw upper-order components can
violate this forced constraint. The exceptional \(\ell=1\) cannot
use the division by \(\lambda\). The general-warped-product vector
equations supply a separate source cross-check.
[Ishibashi–Kodama, arXiv:1103.6148v2, §5.1, (5.1)–(5.3)](https://arxiv.org/pdf/1103.6148v2)

## Exact mean and scalar source

Use ordinary coefficients in \(\epsilon\) after taking the linear axial
variation in \(\eta\). Write \(H^{ba cd}=\sqrt{|B|}R^4B^{bc}B^{ad}\)
and \(M^{ad}=\sqrt{|B|}R^2 B^{ad}\), with \(F_{cd}\) antisymmetric
in contractions. Let \(\langle\cdot\rangle=L^{-1}\int dz\) in the
chosen, completed periodic chart. The order-two equation is
\[
\partial_b[H_0^{ba cd}\bar F_{2,cd}+\mathcal C^{ba}]
-\lambda M_0^{ad}\bar A_{2,d}-\mathcal D^a=0,\quad
\mathcal C^{ba}=\langle H_1^{ba cd}F_{1,cd}+H_2^{ba cd}F_{0,cd}\rangle,
\quad
\mathcal D^a=\lambda\langle M_1^{ad}A_{1,d}+M_2^{ad}A_{0,d}\rangle .
\tag{E2}
\]
Here \(H_2,M_2\) contain both the direct order-two spherical
geometry and the quadratic inverse/determinant/warp contributions.
The \(H_1F_1,M_1A_1\) terms require the constrained retarded mixed
solution and its preparation. Differentiation, including \(\partial_z\),
is included by (E1). A reflection-symmetric standing GL preparation
with even noncompact baseline axial components has \(\bar A_{2,z}=0\).
Otherwise its \(n=0\) compact component has a separate equation.
Total compact derivatives drop from (E2). For a \(z\)-independent
baseline the pure second harmonic in \(H_2,M_2\) contributes zero to
(E2). The mean of two cosine factors is \(1/2\) and that of two
sine factors is \(1/2\), with derivative signs carried separately.
Opposite complex Fourier momenta reproduce these averages.

In the exterior background coordinates \((t,r,z)\) let
\(f=1-2M_{\rm g}/r\), \(dx/dr=f^{-1}\), \(x=r_*\), and orient the base
by \(dt\wedge dr\wedge dz\), where \(M_{\rm g}\) denotes the
geometric mass. Define the exact order-two component
\(q=[\langle R^4(*_B F)_z\rangle]_2
=H_0^{trcd}\bar F_{2,cd}+C\), with \(C=\mathcal C^{tr}\).
The identity with the antisymmetric density follows from the
orientation and holds even with off-diagonal base components.
Since \(\sqrt{|B_0|}=1\) in areal coordinates, (E2) gives
\[
\bar A_{2,t}=\frac{f}{\lambda r^2}(q_r+\mathcal D^t),\qquad
\bar A_{2,r}=\frac{1}{\lambda r^2f}(q_t-\mathcal D^r),\qquad
\bar F_{2,tr}=-(q-C)/r^4.
\]
Use \(F_{tr}=\partial_t A_r-\partial_r A_t\) and
\(\Psi_2=2q/(\lambda r)\) to obtain
\[
\begin{split}
L_{\rm RW}\Psi_2&=\frac{2r}{\lambda}
\left[-\partial_t(r^{-2}\mathcal D^r)
-\partial_x(fr^{-2}\mathcal D^t)-\lambda f r^{-4}C\right],\\
L_{\rm RW}&=-\partial_t^2+\partial_x^2
-f\left[\frac{\ell(\ell+1)}{r^2}-\frac{6M_{\rm g}}{r^3}\right].
\end{split}
\tag{E3}
\]
In particular the \(C\) term has a minus sign.
For \(\Psi^{\rm flat}_2=2(q-C)/(\lambda r)\), the source is the
right side of (E3) minus \(L_{\rm RW}(2C/(\lambda r))\), and its
initial pair differs accordingly. Both choices give the same
extraction after all product and initial-data terms are included.
A minimal source check sets \(\mathcal D=0\) to isolate the algebraic
\(C\) term for the exact extraction from the differentiated \(C\)
terms for the background curl.

In the pure mean metric with reflection, put
\(B_{\alpha\beta}=e^{-2\varphi/\sqrt3}b^E_{\alpha\beta}\),
\(B_{zz}=e^{4\varphi/\sqrt3}\),
\(R=e^{-\varphi/\sqrt3}r_E\). Then \(\sqrt{|B|}=\sqrt{|b^E|}\),
and the \(\varphi\) powers in both \(H^{\alpha\beta\gamma\delta}\)
and \(M^{\alpha\beta}\) cancel algebraically. This is exact for
the pure mean two-dimensional blocks. Nonzero-mode products and
the mean Einstein-frame geometry still enter. The comparison uses
the same mass and Bondi clock.

With \(\epsilon_{tr}=+1\), the product-background \(\Psi\) matches
the CPM master, since \(\tilde h_\alpha=r^2 A_\alpha\) and
\(\epsilon^{\alpha\beta}(\nabla_\alpha\tilde h_\beta
-2r_\alpha\tilde h_\beta/r)=-r^2 F_{tr}\).
The shear is \(\Psi_{\mathscr I}X_{ij}\) when nonzero compact
extraction products vanish at infinity in the fixed Bondi chart.
The original RW function is half the time derivative of CPM
in vacuum and has a distinct forced relation in the presence of a
source. For unit-normalized scalar \(Y_{\ell m}\), the axial shear
harmonic norm is \(\lambda\ell(\ell+1)/2\), hence \(12\) for
\(\ell=2\).
[Martel–Poisson, arXiv:gr-qc/0502028, (5.13)–(5.18), (6.12)](https://arxiv.org/pdf/gr-qc/0502028)

The completion is invariant under axial angular shifts and covariant
under base diffeomorphisms. Coordinate-wise \(z\) projection of
\(Q_z\) depends on the \(\epsilon\)-dependent base chart. For a pure
change generated by \(X=\chi(t,r)\cos(kz)\partial_t\) with compact
radial support and \(Q_0=q_0(t,r)\,dz\), the mean coefficient of
\(\exp(\epsilon\mathcal L_X)Q_0\) at order two is
\(\tfrac14\chi\,\partial_t(\chi\,\partial_t q_0)\,dz\), generally
nonzero. The diffeomorphism is the identity near infinity and gives
zero change in asymptotic news or residue when the metric, slice and
data all transform. This tests prescriptions that average raw
higher coefficients or hold raw data fixed under an interior gauge
change. Future-horizon regularity of (E1) is imposed in an
ingoing or horizon-penetrating chart. Factors of \(f^{-1}\) in the
exterior reconstruction are coordinate factors.

## Retarded pole and data counterexample

For a Schwarzschild-time Cauchy formulation at \(t_0=0\) write
\(p=\Psi_2|_0\), \(g=\partial_t\Psi_2|_0\). With
\(\hat q(s)=\int_0^\infty e^{-st}q(t)dt\), equation (E3) gives
\[
[\partial_x^2-s^2-V_{\rm RW}]\hat\Psi_2=\hat S-sp-g .
\tag{E4}
\]
For a different initial surface include its Cauchy surface term or
propagate to a compatible Schwarzschild-time representation.
First take \(u_H(s,x)\sim e^{sx}\) at \(x\to-\infty\) and
\(u_I(s,x)\sim e^{-sx}\) at \(x\to+\infty\) in a right half-plane.
These give the ingoing future-horizon and outgoing infinity conditions
and the kernel
\[
G_s(x,y)=u_H(s,x_<)u_I(s,x_>)/W(s),\qquad
W=u_Hu_I'-u_H'u_I ,
\]
with derivative jump \(+1\). Continue the retarded branches on a
specified sheet. A simple off-cut resonance is a simple zero
\(W(s_n)=0\) with regular nonzero normalizations. Normalize \(u_I\)
to unit outgoing amplitude. When the source-data numerator is
holomorphic at \(s_n\) after the required spatial and temporal
continuation, the outgoing CPM pole coefficient is
\[
\mathcal A_n=
\frac{N(s_n)}{W'(s_n)},\qquad
N(s)=\int_{\mathbb R}u_H(s,y)[\hat S(s,y)-sp(y)-g(y)]\,dy .
\tag{E5}
\]
For compact support the integral follows by observing beyond the
support. For extended sources it denotes the continuation of the
causal pairing when justified. The news pole is \(s_n\mathcal A_n\)
in the same Bondi clock when differentiation adds analytic endpoint
terms. An additive change of \(x\) changes \(u=t-x\) and hence the
pole coefficient. Nonzero analytic rescalings of \(u_H\) cancel from
the ratio. For \(s=-i\omega\) the Laurent residue in the complex
frequency has a Jacobian \(i\) with a corresponding inverse contour
conversion. The time-domain response also includes branch-cut and
prompt pieces.
[Leaver, Phys. Rev. D 34, 384 (1986), §II.A–B and §III](https://doi.org/10.1103/PhysRevD.34.384)

A compact real bump \(h\) in any exterior open interval where
\(u_H(s_n,\cdot)\) is not identically zero gives a counterexample to
preparation-independent residues. Add only \(\Delta p=0\),
\(\Delta g=h\) at \((2,1),n=0\), keeping (E3)'s source and all lower
jets fixed. The initial homogeneous perturbation is
\(\Delta A_t=0\), \(\Delta A_r=h/(2rf)\), \(\Delta A_z=0\), and
\(\Delta F_{tr}=0\). Reconstruction from the homogeneous master
supplies the velocities, so it is constrained and smooth for a
compact support strictly outside the horizon. Its angular
\(\ell\ge2\) and compact support preserve the stated charges through
the order considered. The residue change is
\[
\Delta\mathcal A_n=-\frac{1}{W'(s_n)}
\int u_H(s_n,y)h(y)\,dy .
\tag{E6}
\]
Choose \(h\ge0\) narrowly concentrated at a nonzero point to obtain
a nonzero result. For \({\rm Im}(s_n^2)\ne0\), the real span of values
of \(u_H(s_n,y)\) in any open interval is two-dimensional. A constant
complex phase on the interval would force its real radial profile
to vanish by taking the imaginary part of the radial ODE. Two narrow
real bumps therefore tune the complex residue arbitrarily, including
cancellation when the original residue exists. Reality tunes the
conjugate pole accordingly. Put the bumps near the potential peak
to avoid changing the GOAL-007 early far-exterior witness. Finite
speed gives earliest arrival at \(\mathscr I^+\) no earlier than
\(u=-\sup {\rm supp}\,h\) for \(t_0=0\). This is a different
upper-order preparation within the constraint-compatible family.

## Remaining gates

A pure separated factor \(e^{j\Omega t}\) shifts the one-sided
Laplace argument by \(-j\Omega\). With elapsed time from a nonzero
initial time there is also the multiplier \(e^{j\Omega t_0}\).
The actual extended GL-driven source requires a continuation to
\(s_n\) on the same sheet, including upper-order data, shifted
massive thresholds/poles, and endpoint convergence or a specified
analytic regularization. A pole of \(\hat S\) at \(s_n\) can yield
a higher pole. A fixed-\(u\) estimate on a short exterior strip
does not justify the infinite-time continuation or the radial
pairing with a continued, exponentially growing Jost function.

Even for smooth compact RW sources, adding a bump
\(\Delta S=b(t)h(x)\) entirely after the observation interval
changes (E5) by
\(\hat b(s_n)\int u_H(s_n,x)h(x)dx/W'(s_n)\) while finite speed
preserves the earlier trace. This falsifies exact residue extraction
from a finite time window in the general RW problem. A prescribed
GL switch or window defines its own observable. The nonstationary
growing GL family provides no stationary perturbed QNM eigenproblem.
A specific strong-field coefficient of the physical source remains
unevaluated.
