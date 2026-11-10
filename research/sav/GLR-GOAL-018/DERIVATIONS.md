# Physical return, regular outgoing transport and the mixed adjoint

## Conventions and source

Use \(r_0=1\), \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\), \(f=1-1/r\), \(k=\pi/5\) and \(\lambda=4\). The causal wedge is \(a\le u\le d\), \(b(u)\le v\) with \(a=-13/8-2\log(5/8)\), \(d=8.407362\) and \(\Sigma\colon v=r=b(u)\). The graph slope in \(t,x\) is \(p=-1/r\). The support front is flat. For the finite cap set \(c=d+2x(400)\). The observable is the Einstein-frame zero-mode shear derivative \(N_{21}=\lim_{v\to\infty}w_u\), \(w=2Q_{2z}/(\lambda r)\). The curl sign follows from \(g_{uA}=D^BC_{AB}/2+O(r^{-1})\) and \(F^{\rm curl}_{ur}\sim-\lambda C/(2r^3)\) for \(C_{AB}=CX_{AB}\). The factor \(r_0\) in physical shear cancels against \(u_{\rm phys}=r_0u\). [GOAL-013 source ledger, geometry and extraction](https://github.com/inaeyk/GL-with-AI/blob/1e8e628f68af4cab174fcc44b81af4748a22115c/research/sav/GLR-GOAL-013/SOURCE-LEDGER.md) · [Martel–Poisson, Secs. V–VI](https://arxiv.org/pdf/gr-qc/0502028v1)

Use the unhalved GL cosine and the actual \(+k\) mixed amplitude. The first insertion uses half the unhalved coefficient and the return pairs that coefficient with the real part of the mixed response. In regular EF \((v,r)\) set \(g=\left(\begin{smallmatrix}0&1\\1&f\end{smallmatrix}\right)\), \(X_b=gH_{\rm GL}\), \(d_b=H_{\theta\theta}/(2r^2)\). Trace \(\mathrm{tr}X_b=-4d_b\) gives the normalized orbit density matrices
\[
 B_1=-X_b g,\qquad
 B_{\rm dir}=\tfrac12[X_b^2-(2d_b^2+\tfrac14\mathrm{tr}X_b^2)\mathbf1]g,\qquad
 B_{\rm mean}=(f\beta-2\mu/r)e_re_r^T .
\]
Here \(\mu=\frac12\int_r^\infty \rho^2\mathcal T^v{}_v\,d\rho\), \(\beta=-\frac12\int_r^\infty\rho\mathcal T_{\rho\rho}\,d\rho\) at fixed \(v\). The density variation cancels the pure radion. The mean retains the mass and lapse subtraction at infinity. With \(D=r^2(\lambda+k^2r^2)\), \(U=-Q_{1z}\) and first-insertion \(J,X\),
\[
 D A_{1r}=-J^v+U_r-ik r^4A_{1z,r},\qquad
 D A_{1v}=fJ^v-J^r-(\partial_v+f\partial_r)U-ik r^4 A_{1z,v},\qquad
 F^{\rm curl}_{1,vr}=(U+X)/r^4.
\]
Use \(A_{0r}=-(w_{0,r}+w_0/r)/(2r)\) and \(A_{0v}=((\partial_v+f\partial_r)w_0+fw_0/r)/(2r)\). The stationary two-field system for \(Y=(U/\sqrt D,\Im(\sqrt W A_{1z}))^T\), \(W=\lambda r^4/(\lambda+k^2r^2)\), is exactly the two equations in GOAL-014 Eq. (5). It has a skew time coupling and positive stationary spatial form. Its zero graph displacements come with both velocities of Eq. (6). In particular, vanishing independent compact electric data do not set the induced compact velocity to zero. [GOAL-014 Eqs. (1)–(6)](https://github.com/inaeyk/GL-with-AI/blob/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014/CURRENTS-AND-DATA.md)

For \(\Sigma_A=B_1\Re A_1+(B_{\rm dir}+B_{\rm mean})A_0\), the return currents are
\[
 F=2r(2\Sigma_A^r-f\Sigma_A^v),\quad G=2rf\Sigma_A^v,\quad
 H=\frac f{2r}(G-F)-\frac{2f}{r^3}\mathcal C .
\]
Here \(\mathcal C=-6d_b\Re(U+X)+[\frac12(14d_b^2+\mathrm{tr}X_b^2/4)-\beta]Q_{0z}\). The density transform is \(\Sigma^t=f\Sigma^v-\Sigma^r,\ \Sigma^x=\Sigma^r\), and the antisymmetric curl density is unchanged. In particular \(G_{\rm mean}=0\). The graph condition is \(w|_\Sigma=0,\ 4w_u|_\Sigma=G-b'F\), \(b'=-f/(2-f)\). It is exactly the contact needed in the zero-past extension of the return in divergence form. For the baseline the graph displacement is nonzero on the packet and is part of its own solve.

## Strong estimate in a regular radial measure

The source convention is \(4w_{uv}+Vw=F_u+G_v-H\), \(V=f(6/r^2-3/r^3)\). Write the free graph lift using \(\ell(v)=\max(a,b^{-1}(v))\), with the inverse used only for \(v<b(a)\),
\[
 4J_s(u,v)=\int_b^v F(u,s)\,ds+\int_{\ell(v)}^u\left(G(z,v)-\int_{b(z)}^v H(z,s)\,ds\right)dz.
\]
It carries the full graph jet. The remainder solves \(L_{\rm RW}(w-J_s)=VJ_s\) with homogeneous graph/front jets. A reconstruction with scalar scattered residual \(R_z\) therefore uses uncommuted energy on \(V\delta J_s-R_z\), plus the graph/front flux, seams and cap comparison defect. The free cap news difference is \((\partial_u\int_b^c\delta F\,dv+\delta G(c)-\int_b^c\delta H\,dv)/4\).

At fixed \(v\), \(\partial_u=-f\partial_r/2\); at fixed \(u\), \(dv=2\,dr/f\). With \(r_c(u)=x^{-1}((c-u)/2)\) this gives
\[
 \partial_u\int_b^c\delta F\,dv=-b'\delta F_\Sigma
 -\int_{b(u)}^{r_c(u)}(\partial_r|_v\delta F)(u+2x(r),r)\,dr.
\]
A seam \(v=\gamma(u)\) contributes \(-\gamma'[\delta F]\) if the cell derivatives in the ray omit its jump. A fixed-\(u\) jump of the primitive can give a Dirac measure. A conforming lift or exact cancellation is required for an \(L^2\) bound. The row of \(F\) multiplying \(Y_r\) in EF is \(-rh_+(D^{-1/2},k/(2\sqrt{\lambda+k^2r^2}))\), with \(h_+=4H_{vv}+4fH_{vr}+f^2H_{rr}\). This follows by contracting \((-f,2)B_1(-f/2,1)^T=-h_+/2\).

For a mixed error \(e_{uv}=A(e_u+e_v)+Be+T_e\) define \(D_+=\partial_v|_r+(f/2)\partial_r\), \(q=e_r\), \(P=D_+e\), \(a_m=A/f,\ b_m=B/f,\ \tau=T_e/f\) in this paragraph. Then \(P=e_v|_u\), \(e_u=-fq/2\) and
\[
 D_+q=(A-f'/2)q-2a_m P-2b_m e-2\tau .
\]
Differentiate and use \([\partial_r,D_+]=(f'/2)\partial_r\), \(\partial_rP=Aq-2a_mP-2b_me-2\tau\). This gives
\[
 D_+e_{rr}=(A-f')e_{rr}+\mathcal R,\quad
 \mathcal R=(A_r-f''/2-2a_m A-2b_m)q+(4a_m^2-2a_{m,r})P
 +(4a_mb_m-2b_{m,r})e+4a_m\tau-2\tau_r.
\]
The coefficient \(A\) is skew. Its orthogonal propagator times the scalar damping has norm \((f(s)/f(r))^2\), by integrating \(2 f'/f\) in \(r\). Thus an error weighted by \(C(r)=r|h_+|\sqrt{1/D+k^2/[4(\lambda+k^2r^2)]}\) has bulk kernel \(2f(s)\int_s^{r_c(u)}C(r)f(r)^{-2}\,dr\). This is bounded toward \(s=1\) for bounded \(C\) and finite cap since \(f(s)\int_s^R f^{-2}\,dr\to 1\). The analogous boundary factor is \(\int_b^{r_c}C(r)(f(b)/f(r))^2dr\). Deriving \(\partial_rP\) from the PDE avoids a separate incoming second derivative. On the graph \(e_{rr}=[(d/dr)q_\Sigma-D_+q|_\Sigma]/(1-f/2)\). For exact zero graph first jets this is \(2\tau_\Sigma/(1-f/2)\), so a trace of the summed defect is still needed. The geometry controls coefficients, while \(\tau_r\), lower-field errors and the cap defects still need enclosures. [Inherited free split and exterior budget, Eqs. (2), (8), (9)](https://github.com/inaeyk/GL-with-AI/blob/9e2fe32c35151a8a4108864622cf2aef88bb0839/research/sav/GLR-GOAL-017/analysis/STRONG-NEWS.md)

## Backward physical weak route

For \(g\) supported on \(I=[0,d]\) the backward scalar field solves \(4Z_{uv}+VZ=0\), \(Z(u,\infty)=g(u), Z(d,v)=0\). Green's identity cancels the scalar graph contact against the physical induced velocity. The exact finite-cap formula carries \(+\int g G(c)/4\). With the infinite test and currents restricted to a finite cap, transposition already carries the negative cap delta from \(\partial_v(\mathbf1_{v<c}G)\). Do not add the finite-cap term a second time. For a smooth \(v\) taper \(\chi\) vanishing near \(c\), the mixed part of the infinite-cap moment can be written \(\int (C_uY_u+C_vY_v+C_0Y)\,du\,dv\), with rows that include \(-\chi/4\) times the scalar-test coefficients. Affine terms and direct-plus-mean terms pair with the baseline separately. The removed outer currents need an independent error bound.

Where \(g\) is smoothly zero-extended, \(T_m=(C_0-\partial_u C_u-\partial_v C_v)^T\) is an ordinary backward source. A flat jet \(g=g'=0\) at both ends suffices. For sine tapers, at \(d\) one has \(Z=Z_v=0\), \(Z_u=g'(d)\). At \(0\), \([Z_u]=g'(0+)\) by the scalar PDE while \(Z,Z_v\) are continuous. If \(F_m=f_uY_u+f_vY_v+f_0Y\), the terminal row is \(-g'(d)\chi f_u/4\) and the interior drive contains \(+\chi g'(0+) f_u\,\delta(u)/4\) in \(du\,dv\).

A direct repair works with the same witnesses. Put \(B^u=B^t-B^x,\ B^v=B^t+B^x\) for the mixed forcing \(L_mY=\partial_u B^u+\partial_v B^v+R_m\), where \(B^t,B^x,R_m\) are GOAL-017 weak Eq. (6). Let \(L_m=-4\partial_u\partial_v+\mathsf C(\partial_u+\partial_v)-\mathcal V_m\), with \(\mathsf C\) skew and stationary. Solve \(L_m\Phi=T_m\) on the two sides of \(u=0\), set \(\Phi(u,c)=0\), and prescribe
\[
 4\Phi_v-\mathsf C\Phi=C_u^T\quad(u=d),\qquad
 4[\Phi]_v-\mathsf C[\Phi]=[C_u]^T\quad(u=0),
 \qquad \Phi(d,c)=[\Phi](0,c)=0.
\]
The skew coupling gives absolute face gains at most \(1/4\) times the downstream integrals of the row norms. In the bilinear Green current the \(u\)-flux is \(-2(\Phi^TY_v-\Phi_v^TY)+\Phi^T\mathsf C Y\); integrate along the face to obtain the above coefficient. The canonical graph relation \(4Y_u=b'B^u-B^v\) cancels the mixed launch contact. The front is flat, and the taper removes the mixed cap row. The volume contribution after pullback is
\[
 \int [-\Phi_u^T B^u-\Phi_v^T B^v+\Phi^T R_m]\,du\,dv
 +\int_{u=d}\Phi^T B^u\,dv-\int_{u=0}[\Phi]^T B^u\,dv .
\]
All volume derivatives here are piecewise derivatives. A hard cap instead prescribes \(4\Phi_u-\mathsf C\Phi=C_v^T\) there and adds \(\int_{v=c}\Phi^T B^v\,du\). The scalar infinite test retains all outer potential scattering. A nonzero corner term from integrating tangential derivatives cancels between adjacent faces. When the lower-field error obeys \(e_{uv}=-Ve/4+R\) with zero graph and front first jets, an opposite-direction error on \(u=u_*\) obeys \(\sup_v|e_u|\le (\int V/4\,dv)^{1/2}M_0+\int|R|\,dv\). This is a row residual requirement beyond the volume energy \(M_0\). The cap analogue for \(e_v\) starts at the flat front.

A computable derivative budget for the scalar adjoint uses \(Z=\sum_{n\ge0}(-T)^ng\), \(Th=\int_u^d\int_v^\infty Vh/4\). Rectangles lie in the wedge. The tail of order \(m\) in sup norm is bounded by \(\|g\|_\infty\sum_{n>m}(DJ_*)^n/(n!)^2\), \(D=d-a\), \(J_*=\int\sup_u V/4\,dv<3.008\). The differentiated tail in \(u\) is bounded by \(J_*\|g\|_\infty\sum_{n>m}(DJ_*)^{n-1}/[(n-1)!n!]\). The corresponding \(v\)-bound uses \(D\sup V/4\) in place of \(J_*\). The equation for the mixed derivative of the tail uses the preceding-order undifferentiated tail. Differentiate the one-direction tail integral once more for the pure second derivatives. This permits a whole-cell adjoint quadrature with no numerical differentiation of nodal fields. No such directed physical mixed-adjoint evaluation was completed here. [Inherited weak equations (1)–(8)](https://github.com/inaeyk/GL-with-AI/blob/9e2fe32c35151a8a4108864622cf2aef88bb0839/research/sav/GLR-GOAL-017/analysis/WEAK-WITNESSES.md)
