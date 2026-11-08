# A strong late-news error estimate from weak current errors and one differentiated ray

Use the canonical real axial packet and matched Einstein-frame news of [GOAL-013](../../GLR-GOAL-013/SOURCE-LEDGER.md) at `1e8e628f68af4cab174fcc44b81af4748a22115c` and the [exact-curl current split](../../GLR-GOAL-014/CURRENTS-AND-DATA.md) at `0b4bfcbb978b52626fb0816874df06055703c195`. The independent [GOAL-014 audit](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/AUDIT.md) is read-only evidence. The strong compact error in that audit was open. The results below are estimates for the pinned ordered problem. They use the certified exterior source bound as an input. They require a numerical compact error ledger for a physical news enclosure.

In units of \(r_0=1\) let \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\), \(f=1-1/r\), \(V=f(6/r^2-3/r^3)\), \(W_s=V/4\). Write \(a=-13/8-2\log(5/8)\), \(d=8.407362\), \(D=d-a\), \(I=[0,d]\) and \(I_0=[a,0]\), with \(D_0=-a\). The spacelike graph is \(v=b(u)=r_\Sigma(u)\). It is decreasing, with \(p=T'(x)=-1/r\), \(b'=-(1+p)/(1-p)\), and \(b(a)=R=13/8\). The causal strip at a null cap \(c=d+2x(r_c)\) is \(\mathcal D_c=\{a\le u\le d,\ b(u)\le v\le c\}\). Put \(\ell(v)=b^{-1}(v)\) for \(v<R\), and \(\ell(v)=a\) otherwise. The entire physical graph beyond the launch packet is flat in the axial sector. The final radius is \(b(d)=1.009021077109608\ldots\). The axial horizon collar \(r<b(d)\) is excluded by causality. The fixed-infinity mean integrals remain part of the physical coefficients.

## 1. Subtract the graph-compatible free return before applying the positive energy estimate

The full returning shear \(w=2Q_{2z}/(\lambda r)\) solves
\[
 L_{\mathrm{RW}}w=-F_u-G_v+H,\qquad L_{\mathrm{RW}}=-4\partial_u\partial_v-V,
 \qquad 4w_u|_\Sigma=G|_\Sigma-b'F|_\Sigma,
\]
with zero upper shear on the graph and a flat front at \(u=a\). Here \(F,G,H\) are the full real ordered currents, with the two mixed polarizations, their affine reconstruction, the direct term and the fixed-infinity mean. For any triple of currents with the same front compatibility define
\[
 \mathcal F(u,v)=\int_{b(u)}^v F(u,s)\,ds,\qquad
 4J_s(u,v)=\mathcal F(u,v)+\int_{\ell(v)}^u
 \left[G(z,v)-\int_{b(z)}^v H(z,s)\,ds\right]dz. \tag{1}
\]
This is the free retarded solution with the induced graph jets. Indeed its graph trace is zero and
\[
 4(J_s)_u=\partial_u\mathcal F+G-\int_b^v H,\qquad
 4(J_s)_{uv}=F_u+G_v-H.
\]
At the graph the first equality gives \(4(J_s)_u=G-b'F\), and tangential differentiation gives \((J_s)_u+b'(J_s)_v=0\). The front vanishes because the prescribed currents vanish there. This verifies the normalization and signs independently of differentiating a numerical wave solution. For instance on a straight graph with constant \(F,G\) and \(H=0\), away from any front, it gives \(4J_s=(v-b(u))F+(u-\ell(v))G\). The free bulk forcing vanishes but the physical induced velocity persists.

For continuous locally Lipschitz reconstructed currents \((F_h,G_h,H_h)\), form \(J_{s,h}\) by (1) with exact integration or bound its evaluation defects. Write the candidate shear as \(w_h=J_{s,h}+z_h\). A convenient conforming candidate for \(z_h\) is a double characteristic primitive of a continuous piecewise Lipschitz drive \((z_h)_{uv}\) with zero graph and front jets. Integrate it from the graph and the front as in the [GOAL-015 mixed lift](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/checks/COMPACT.md). With these traces the candidate is \(C^1\) through the cell interfaces. More generally retain the measured graph and front flux errors and distributional seam contributions. Define
\[
 J_e=J_s-J_{s,h},\qquad z_e=(w-J_s)-z_h,\qquad
 R_z=L_{\mathrm{RW}}z_h-VJ_{s,h}.
\]
An exact subtraction gives \(L_{\mathrm{RW}}z_e=VJ_e-R_z\). Thus the compact **scattered** news error uses undifferentiated current errors. The physical graph jets of \(w-J_s\) vanish, so no induced return graph term survives in \(z_e\). A candidate graph defect in \(z_h\), or a failure to assemble the graph currents in \(J_{s,h}\) consistently with the bulk currents, must still be carried.

For \(L_{\mathrm{RW}}e=S\) use the stationary current \(E=(|e_t|^2+|e_x|^2+V|e|^2)/2\), \(J=-\Re(e_t\overline{e_x})\), with \(E_t+J_x=-\Re(S\overline{e_t})\). The graph integrand is \(E-pJ\), and the outgoing integrands on fixed \(v,u\) are respectively \(|e_u|^2+V|e|^2/4\) and \(|e_v|^2+V|e|^2/4\). With the two characteristic mixed norms on \(\mathcal D_c\), define
\[
 \mathfrak B(S)=\tfrac12(\|S\|_{L^1_vL^2_u}+\|S\|_{L^1_uL^2_v}),\qquad
 M(S,E_{\rm in})=\frac{\mathfrak B(S)+\sqrt{\mathfrak B(S)^2+4E_{\rm in}}}{2}.
\]
The supremum of either outgoing flux square root on all subdiamonds is at most \(M\). To see this, integrate the current over a subdiamond and bound the volume term using \(e_t=e_u+e_v\) and \(dt\,dx=du\,dv/2\). The two directions pair with the corresponding fixed-ray flux, giving \(M^2\le E_{\rm in}+\mathfrak B(S)M\). In the graph-exact, front-exact case the estimate reduces to \(M\le\mathfrak B(S)\). The same argument works for the two mixed fields with \(D_x=\partial_x+\mathsf A\), potential \(K\ge0\) and anti-Hermitian stationary time coupling, as in [GOAL-013 finite-cut bounds](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md). This derivation excludes uncontrolled jumps in the energy flux or delta sources on cell faces.

For the compact cap candidate \(p_c\), put \(E_{\rm tr}=\|(w_h)_u(\cdot,c)-p_c\|_{L^2(I)}\). At the physical cap the estimate is
\[
\boxed{\begin{aligned}
 E_{c,I} \le{}& E_{\rm tr}+M(VJ_e-R_z,E_{\rm in,z})\\
 &+\tfrac14\bigg(\|\delta G(\cdot,c)\|_{L^2(I)}+\|A_H\|_{L^2(I)}
     +\left\|\partial_u\int_{b(u)}^c \delta F(u,v)\,dv\right\|_{L^2(I)}\bigg),
\end{aligned}} \tag{2}
\]
where \(\delta F=F-F_h\) and similarly for \(G,H\), and \(A_H(u)=\int_b^c|\delta H|\,dv\). The last differentiated ray equals \(\int_b^c\partial_u\delta F\,dv-b'\delta F(u,b(u))\). With exact post-packet graph currents its graph term is zero on \(I\). The compact early cut enters (2) only through undifferentiated data for \(J_e\) and the scalar residual \(R_z\). For an independent cap-news candidate obtained by a separate march, the cap trace comparison \(E_{\rm tr}\) is essential.

A piecewise current reconstruction with jumps requires care. Across \(v=\gamma(u)\), a jump \([\delta F]=\delta F_+-\delta F_-\) adds \(-\gamma'[\delta F]\) to the derivative of the ray integral. A jump of the ray integral at a fixed \(u\) gives a Dirac measure in (2). An \(L^2\) cap estimate then needs a conforming lift or proof of cancellation. Continuous piecewise Lipschitz current interpolation avoids these measures. For \(z_h\) the proposed primitive of a continuous drive has continuous gradient and hence matching energy current. Its pointwise residual can be bounded on each whole cell. None of these statements estimates the physical differences between directly interpolated current arrays and currents assembled from a conforming lower-field lift.

## 2. Constants for the scattered term

Let \(A_F(u)=\int_b^c|\delta F|\,dv\), \(j(u)=\sup_{b(u)\le v\le c}|J_e(u,v)|\), and
\[
 L_1=\int_{b(d)}^\infty\sup_{\ell(v)\le u\le d}V\,dv,\qquad
 L_2=\sup_{a\le u\le d}\left(\int_{b(u)}^\infty V^2\,dv\right)^{1/2}.
\]
The peak is at \(r_p=(9+\sqrt{17})/8>R\). For \(v\le R\) the maximum is on the graph. For \(R<v<a+2x(r_p)\) it is on \(u=a\). It is the peak for the next \(D\) units of \(v\), and for larger \(v\) is on \(u=d\). Using \(dv=2dr/f\) along each outgoing ray gives
\[
 \begin{split}
 L_1 &=\left[-6/r+9/(2r^2)-1/r^3\right]_{b(d)}^R+4\mathcal J(R)+D V(r_p),\\
 L_2^2&=2\left(12/b(d)^3-18/b(d)^4+9/b(d)^5-3/(2b(d)^6)\right),\\
 \mathcal J(r)&=3/r-3/(4r^2).
 \end{split}
\]
Here \(\mathcal J(r)\) is the integral of \(V/4\) on a null ray from radius \(r\) to infinity. Directed arithmetic gives \(L_1<12.02940\), \(L_2<1.731844\), and
\[
 \mathfrak B(VJ_e)\le \tfrac12(L_1\|j\|_2+L_2\|j\|_1)
                 < 8.625760\|j\|_2 \quad\text{if }\|j\|_2>0. \tag{3}
\]
This replaces the large exponential from an absolute shear Volterra majorant over the entire strip. The first inequality with the exact constants also covers \(j=0\).

One may estimate the \(G\) primitive without a supremum on the outgoing rays. Let \(g_2(v)=\|\delta G(\cdot,v)\|_{L^2(\ell(v),d)}\), \(v_*(v)=\sup_{\ell(v)\le u\le d}V(u,v)\), and \(B_0(u)=A_F(u)+\int_a^u A_H\). The primitive on an interval of length at most \(D\) has \(L^2\) norm at most \(k_D=2D/\pi\). The \(F,H\) parts of \(4J_e\) are bounded pointwise by \(B_0\), and its \(G\) part has \(L^2_u\) bound \(k_D g_2(v)\). Taking the same two mixed norms and applying Cauchy–Schwarz in \(u\) for the second \(G\) norm gives
\[
 \mathfrak B(VJ_e)\le \tfrac18\left[L_1\|B_0\|_2+L_2\|B_0\|_1+
 k_D\big(\|v_* g_2\|_{L^1_v}+\sqrt D\|v_* g_2\|_{L^2_v}\big)\right]. \tag{4}
\]
This form uses the derivative tangent to fixed \(v\) and the potential term from the fixed-\(v\) flux, together with the fixed-\(u\) flux for the other derivative when needed. In general fixed-\(v\) energy alone does not control \(g_2(v)\) for a current containing \(e_v\); one can evaluate the mixed \(v\) norms in (4) by Cauchy–Schwarz using fixed-\(u\) energy for that part, or directly enclose cell errors. For a coefficient envelope \(C(v)\), for example, the contribution from \(e_v+\mathsf A e/2\) satisfies \(\|C(e_v+\mathsf A e/2)\|_{L^1_vL^2_u}\le \sqrt D\|C\|_2 M\) on the full strip and \(\|C(e_v+\mathsf A e/2)\|_{L^2_{uv}}\le \sqrt D\|C\|_\infty M\). To bound \(A_F,A_H\) at fixed \(u\), similarly use either fixed-\(u\) flux or weighted Cauchy–Schwarz and fixed-\(v\) flux, depending on which derivative enters. Norms on point endpoints need their own estimate. A small empirical subcell difference does not provide any of these uniform or mixed-norm bounds.

## 3. Propagate the pure outgoing derivative directly on late rays

The only differentiated mixed state needed in \(\partial_u F\) is its pure outgoing second derivative. The other second derivative that occurs is \(Y_{uv}\), available algebraically from the mixed equation. The mixed states are \(Y=(y,Z)=(U/\sqrt D_m,\Im A_z\sqrt{W_m})\) with the stationary weights of the source ledger. Here \(D_m,W_m\) refer to those weights, separate from the window length \(D\). Use
\[
 Y_{uv}=A(P+Q)+B Y+T,\qquad P=Y_u,\quad Q=Y_v,\quad A=\mathsf C_{\mathbb R}/4,\quad B=-\mathcal V_m/4,\quad T=-S_m/4.
\]
The matrix \(A\) is skew-symmetric. An exact source mismatch and residual equation for \(e=Y-Y_h\) takes the same form with \(T_e\) including both the physical first-insertion error and the drive residual in this normalization. For \(\Pi=e_{uu}\), differentiating at fixed \(v\) and using \(Q_u=e_{uv}\) gives
\[
 \partial_v\Pi=A\Pi+A e_{uv}+(A_u+B)e_u+A_u e_v+B_u e+\partial_u T_e . \tag{5}
\]
The graph value is \(\Pi|_\Sigma=(e_u|_\Sigma)'_u-b'e_{uv}|_\Sigma\). On \(I\) the exact physical graph fields and their incoming jets vanish by the compact launch. A lift may impose this compatibility exactly. A graph drive residual still enters if its graph trace fails to vanish. The fundamental matrix of \(\partial_v U=A(u,v)U\) is orthogonal, so the exact estimate on each late ray is
\[
 |\Pi(u,v)|\le |\Pi(u,b(u))|+\int_{b(u)}^v
 |A e_{uv}+(A_u+B)e_u+A_u e_v+B_u e+\partial_u T_e|(u,s)\,ds. \tag{6}
\]
This estimate starts at the graph for each late \(u\) and does not introduce a commuted incoming front at \(u=0\). It uses the uncommuted lower-field error over the full causal strip, including the physical earlier graph. It also needs the late \(\partial_u T_e\) norm, including radial coefficient error and the residual of the reconstruction. The cap contribution of \(\Pi\) to (2) can retain the radial coefficient weight. For a known pointwise coefficient envelope \(C_\Pi(u,v)\), Tonelli gives
\[
 \int_b^c C_\Pi(u,v)|\Pi(u,v)|\,dv\le
 |\Pi_\Sigma(u)|\int_b^c C_\Pi(u,v)dv
 +\int_b^c \left(\int_s^c C_\Pi(u,v)dv\right)|\mathrm{rhs}_{\rm lower}(u,s)|ds, \tag{7}
\]
where \(\mathrm{rhs}_{\rm lower}\) is the integrand in (6). Thus late residual tolerances at large radius can be looser when the return coefficient has decayed. The expression remains a conditional inequality until the physical weighted cells are bounded.

For clarity, with \(d_k=\lambda+k^2r^2\), \(g_k=k/(2\sqrt{d_k})\) and real Fourier variables, the first-order potentials have the derivative parts
\[
 A_{1t}=J^t/D_m+(y_u-y_v-a_m y)/\sqrt{D_m}+g_k(Z_u+Z_v),\qquad
 A_{1x}=-J^x/D_m-(y_u+y_v)/\sqrt{D_m}+g_k(Z_v-Z_u-b_m Z).
\]
The coefficient of \((y_u/\sqrt{D_m}+g_k Z_u)\) in the mixed part of \(F\) is \(2r(H_{tt}+2H_{tx}+H_{xx})/f\) in the unhalved compact-unitary GL convention of the source. This follows from contracting \((-1,1)\widetilde M_1(1,-1)^T\) and is generally nonzero. It explains the required \(Y_{uu}\) term in \(\partial_u F\). The affine \(J^a\), curl and direct-plus-mean terms are still present. This extraction leaves all \(G_v\) terms in the free primitive (1) and the cap value in (2).

For a baseline scalar error \(e_0\) use \((e_0)_{uv}=-W_s e_0+r_0^{\rm def}\), where \(r_0^{\rm def}\) denotes a reconstruction defect in the \(uv\) equation. For its second derivatives there are direct ray equations
\[
 \partial_v(e_0)_{uu}=-W_s(e_0)_u-(W_s)_u e_0+(r_0^{\rm def})_u,\qquad
 \partial_u(e_0)_{vv}=-W_s(e_0)_v-(W_s)_v e_0+(r_0^{\rm def})_v.
\]
The first starts from \((e_0)_{uu}|_\Sigma=((e_0)_u|_\Sigma)'_u-b'(e_0)_{uv}|_\Sigma\). The second uses the corresponding tangential differentiation of the \(Q\) trace on the graph, with slope \(\ell'(v)\), or the zero front trace if \(v\ge R\). To estimate \(\partial_u T_e\) one can propagate \((e_0)_{uuu}\) on late outgoing rays by differentiating the first equation in \(u\). Its source is \(-W_s(e_0)_{uu}-2(W_s)_u(e_0)_u-(W_s)_{uu}e_0+(r_0^{\rm def})_{uu}\) and its graph value is the tangential derivative of \((e_0)_{uu}|_\Sigma\) minus \(b'\partial_v(e_0)_{uu}|_\Sigma\). Mixed third derivatives with one \(u\) and two \(v\) derivatives follow by differentiating the baseline wave equation in \(v\). This avoids a pure third \(v\) derivative. For the base mixed drive the first-insertion error needs second baseline derivatives across the whole strip. The late derivative additionally needs the just listed third derivatives only where they enter (5). The coefficient maps in the [source ledger](../../GLR-GOAL-013/SOURCE-LEDGER.md) determine the factors and Fourier normalization. A convenient baseline drive interpolant has continuous \(u\) derivative if \((r_0^{\rm def})_{uu}\) is to be bounded as an ordinary cell function. A merely bilinear drive with a jump in its \(u\) derivative generally requires a seam correction after this second differentiation. Differentiation of any split into an assembled source and residual must use their summed physical defect, or account for cancellation of jumps between the two.

## 4. Transfer late news while carrying earlier shear only

Let the candidate continue homogeneously with the exact scalar potential outside \(c\), using a common cap shear trace on \([a,d]\), zero at \(a\). Allow a separate numerical continuation error \(E_q\) on \(I\). For the exact solution minus that continuation, write \(e=w-w_h\) in the exterior and \(p_e=e_u\). On \(I\), \(e(u,v)=e(0,v)+\int_0^u p_e(z,v)dz\), so
\[
 (p_e)_v=-(V/4)\mathcal I_0p_e-(V/4)e(0,v)-S_2/4,\qquad
 \mathcal I_0p_e(u)=\int_0^u p_e(z)dz.
\]
The signed gain of the homogeneous late evolution is at most \(G_I=\exp(3d^2/(\pi^2r_c^2))\) for \(r_c\ge100\). Indeed
\(\Re\int_0^d \overline{p_e} V\mathcal I_0p_e=\tfrac12[V|\mathcal I_0p_e|^2]_0^d-\tfrac12\int_0^d V_u|\mathcal I_0p_e|^2\). Use the nonnegative terminal term, the norm \(\|\mathcal I_0\|=2d/\pi\), \(V_u\le 6 f/r^3\), and its ray integral. This is the [GOAL-013 cap argument](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md) restricted to the actual late window.

Let \(J_c=\mathcal J(r_c)\), \(B_c=\|S_2\|_{L^1_vL^2_u([a,d])}/4\) for \(v\ge c\), \(S_0=\|e(\cdot,c)\|_{L^2(I_0)}\), \(s_0=|e(0,c)|\) and \(k_0=2D_0/\pi\). Integrating the wave equation in \(u\) from \(a\) controls the earlier shear by
\[
 \sup_{v\ge c}\|e(\cdot,v)\|_{L^2(I_0)}\le e^{k_0J_c}(S_0+k_0B_c),
\]
with Gronwall for the integrable multiplier \(\sup V/4\). Its endpoint satisfies
\[
 \sup_{v\ge c}|e(0,v)|\le s_0+\sqrt{D_0}\,[J_c e^{k_0J_c}(S_0+k_0B_c)+B_c].
\]
This follows by evaluating the integral equation at \(u=0\) and applying Cauchy–Schwarz on \(I_0\). Hence
\[
\boxed{E_{21}\le E_q+G_I\left[E_{c,I}+B_c+\sqrt d J_c\left(s_0+\sqrt{D_0}[J_c e^{k_0J_c}(S_0+k_0B_c)+B_c]\right)\right].} \tag{8}
\]
This formulation allows a relatively inaccurate early news profile if the early shear, including the endpoint at zero, is known. From the compact free/scattered split one may use \(S_0\le \|J_e(\cdot,c)\|_{L^2(I_0)}+k_0 M\), \(s_0\le |J_e(0,c)|+\sqrt{D_0}M\), with any separate cap shear trace defect added. The endpoint \(J_e(0,c)\) requires the undifferentiated \(F\) row at zero and the preceding \(G,H\) primitives in (1). If the cap data are specified through a news candidate \(p_c\), the continuation must be initialized with its primitive from the flat front, or the difference to the compact candidate shear must be included in \(S_0,s_0\).

For \(r_c=400\), GOAL-013 bounds the complete physical \(B_c<1.7\times10^{-6}\), including mixed feedback and the fixed-infinity mean. Directed constants in (8) give the sufficient budget
\[
 E_{21}\le E_q+1.000135\big[E_{c,I}+0.0217331 s_0+0.000135262 S_0+1.731\times10^{-6}\big]. \tag{9}
\]
For a candidate using the third-degree ordered homogeneous cap approximation, the inherited full-strip analytic operator remainder is \(3.77\times10^{-8}\|p_c\|_{L^2([a,d])}\), plus directed quadrature and arithmetic. This part may still depend on the full candidate norm. It is distinct from the physical compact error. The exterior constants at \(r_c=400\) do not bound the physical source removed by a smaller cap.

## 5. A sharp failure of undifferentiated current control

Take smooth \(\chi\not\equiv0\) supported strictly within \(I\) and \(\rho\) supported in \(R<v<c\) with integral one. Use the manufactured current \(F_n=n^{-1/2}\chi(u)\sin(nu)\rho(v)\), \(G_n=H_n=0\). These are smooth canonical zero-graph and zero-front data for the scalar source and satisfy \(\|\int_b^c |F_n|dv\|_2=O(n^{-1/2})\). Nevertheless the cap derivative of the free response has leading term \(\tfrac14 n^{1/2}\chi(u)\cos(nu)\), whose \(L^2(I)\) norm grows as \(n^{1/2}\|\chi\|_2/(4\sqrt{2})\) by integration by parts in the oscillatory cross terms. Its potential-scattered correction is \(O(n^{-1/2})\) by (3) with the conforming zero candidate. Thus even a positive Regge–Wheeler potential and exact graph compatibility do not yield a late \(L^2\) trace estimate from undifferentiated current norms alone. This is an estimator counterexample for arbitrary scalar currents. It is not a freedom to modify the pinned Einstein hierarchy or physical preparation. Moving the manufactured support strictly before zero instead removes its free term from the late derivative, and the scattered late error is then \(O(n^{-1/2})\). These two placements independently test why the differentiated ray in (2) is needed on \(I\) and why its earlier counterpart is avoidable.

## Reproduction and acceptance gates

These directed constant calculations require Python 3.14.8 and python-flint 0.9.0. The Lambert function evaluates the causal final graph point with outward arithmetic. All constants use \(c=d+2x(400)\), not a cap based on the end of the seed packet.

```python
from flint import arb, ctx
ctx.prec = 128
R = arb(13)/8
a = -R-2*(R-1).log()
d = arb('8.407362')
D = d-a
b = 1+2*((-(1+d)/2).exp()/2).lambertw()
rp = (9+arb(17).sqrt())/8
V = lambda r: (1-1/r)*(6/r**2-3/r**3)
A = lambda r: -6/r+arb(9)/(2*r**2)-1/r**3
J = lambda r: 3/r-arb(3)/(4*r**2)
L1 = A(R)-A(b)+4*J(R)+D*V(rp)
L2 = (2*(12/b**3-18/b**4+9/b**5-arb(3)/(2*b**6))).sqrt()
Jc = J(arb(400))
k0 = -2*a/arb.pi()
common = d.sqrt()*Jc
small = common*(-a).sqrt()*Jc*(k0*Jc).exp()
forcing = 1+common*(-a).sqrt()*(1+Jc*k0*(k0*Jc).exp())
gain = (3*d**2/(arb.pi()**2*400**2)).exp()
assert rp > R
assert L1 < arb('12.02940') and L2 < arb('1.731844')
assert (L1+L2*D.sqrt())/2 < arb('8.625760')
assert common < arb('0.0217331') and small < arb('0.000135262')
assert gain < arb('1.000135')
assert forcing*arb('0.0000017') < arb('0.000001731')
```

Equations (1)–(9) are proved given the stated ordered equations and candidate regularity. Equation (9) is a directed numerical bound on transfer coefficients and the inherited forcing term. There is no evaluated \(E_{c,I}\) from those coefficients alone. For the inherited template pass gates, define \(E^{\rm lim}=0.151604-d_h\) or \(0.517819-d_h\) for the two respective upper tests. For the respective rejection use \(E^{\rm lim}=d_l-\Delta_P-U_P/2\). With exact-cap-compatible homogeneous continuation, a sufficient positive budget for (2) is
\[
 E_{c,I}<\frac{E^{\rm lim}-E_q}{1.000135}-0.0217331s_0-0.000135262 S_0-1.731\times10^{-6}.
\]
A nonpositive right side calls for a tighter template/candidate interval or a different comparison test. A positive right side requires four measured inputs from the actual compact candidates: the scalar conforming trace/residual, the undifferentiated \(F,G,H\) error norms and \(u=0\) endpoint, the signed late differentiated \(F\) ray (or a sufficient weighted norm as in (7)), and the early cap shear. These are separate from the template uncertainty. The [GOAL-015 directed geometry interface](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/source/ENCLOSURES.md) supplies useful real-axis coefficient and early graph cells, but its public combined GL-plus-mean interface begins at \(25/16\), and the original intended queries are restricted further. Its two-polarization conforming probe is on the first eighth of the packet. The collar from \(b(d)\) to that interface, the later physical lower-field and derivative residuals, and the complete physical cap defects require separate enclosures. No successor result or finite-amplitude nonlinear trace is assumed here.
