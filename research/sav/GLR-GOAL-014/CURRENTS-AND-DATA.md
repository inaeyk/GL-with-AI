# Physical current assembly and graph data

All coordinates and master fields here are dimensionless in units of $r_0$. Set $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $f=1-1/r$, $\lambda=4$, $k=\pi/5$. The hypersurface is $v=r$, or $t=T(x)=r-x$ with $p=T'=-1/r$. Its earliest axial cut is $a=-13/8-2\log(5/8)$, and $b(u)=r_\Sigma(u)$ is its inverse cut map. The end of the early interval is $d=a+3/200$ with $b(d)=1.62143634172470\ldots$. The mass, asymptotic circle length and inertial Bondi cuts are those in the pinned preparation. The coefficient and data algebra below uses the [ordered axial reduction](../GLR-GOAL-011/HIERARCHY.md), [canonical preparation](../GLR-GOAL-011/FINITE-DATA.md), and [source ledger](../GLR-GOAL-013/SOURCE-LEDGER.md) at [the GOAL-013 source commit](https://github.com/inaeyk/GL-with-AI/commit/1e8e628f68af4cab174fcc44b81af4748a22115c). Its spectral and five-dimensional inputs are inherited. The new bounds concern this fixed ordered response.

## Density algebra

The axial potential is $A_a=h_a/R^2-\partial_a(h_T/R^2)$ when the angular perturbations are $h_a X_A$ and $2h_T X_{AB}$. This removes the axial angular gauge gradient, and $F_{ab}^{\rm curl}=(dA)_{ab}$. The ordered action gives
\[
 \partial_b (P F^{\rm curl})^{ba}=\lambda M^{ab} A_b,\qquad
 P^{bacd}=\sqrt{|b|}R^4 b^{bc}b^{ad},\qquad M^{ab}=\sqrt{|b|}R^2 b^{ab}.
\]
Write $Q_z=(PF^{\rm curl})^{tx}$. Its background factor is $-r^4 F^{\rm curl}_{tx}/f$. It is the *exact* density, including coefficient corrections, whose noncompact mean yields the shear master $w=2Q_z/(\lambda r)$.

Let $\eta_b=\operatorname{diag}(-1,1)$ and $X_b=\eta_b H/f$, where $H$ is the unhalved real GL orbit coefficient in $(t,x)$ including $e^{\Omega t}$. Write $d_b=H_{\theta\theta}/(2r^2)$, with the same time factor. The compact-unitary GL mode obeys $\operatorname{tr}X_b=-4d_b$. For example the EF reconstruction of $H_{rr}$ satisfies $fH_{rr}=-2(H_{vr}+H_{\theta\theta}/r^2)$, and the noncompact trace is $2H_{vr}+fH_{rr}$. Define $\widetilde M_j=M_j/r^2$ and $\widetilde P_j=P_j^{tx}/P_0^{tx}$ on the single input curl $F_{tx}^{\rm curl}$. The density coefficients needed here are
\[
\begin{split}
 \widetilde M_1&=-X_b\eta_b,\qquad \widetilde P_1=6d_b,\\
 \widetilde M_{2,{\rm dir}}&=\tfrac12[X_b^2-(2d_b^2+\tfrac14\operatorname{tr}X_b^2)I]\eta_b,\\
 \widetilde P_{2,{\rm dir}}&=\tfrac12[14d_b^2+\tfrac14\operatorname{tr}X_b^2].
\end{split} \tag{1}
\]
Indeed $R^2/r^2=1+2\epsilon d_b\cos kz$ in the direct algebra. The noncompact volume factor is $1+\epsilon\operatorname{tr}X_b\cos kz/2+\epsilon^2[(\operatorname{tr}X_b)^2/8-\operatorname{tr}X_b^2/4]\cos^2kz$. For $P^{tx}$ its reciprocal multiplies $R^4/r^4$, because the antisymmetric noncompact inverse pair contributes the inverse determinant. For $M$ the volume factor multiplies $R^2/r^2$ and $(I-\epsilon X_b\cos kz+\epsilon^2 X_b^2\cos^2kz)\eta_b$. This gives (1), including the direct average $\langle\cos^2kz\rangle=1/2$. The first insertion uses $\widetilde M_1/2,\widetilde P_1/2$ for the actual $+k$ coefficient. The mixed mean uses unhalved coefficients times the real parts of the $+k$ fields. The background second harmonic times the baseline is at $\pm2k$. There is no mean contribution from that product.

## Fixed-mass mean and its exact current reduction

In the zero-mode Einstein frame, $R=r$ and $\Delta b_{E,vv}=2\mu/r-2f\beta$, $\Delta b_{E,vr}=\beta$, $\Delta b_{E,rr}=0$. The pure radion cancels from the noncompact $M,P$ by the determinant and inverse-metric weights. With the pinned mean Einstein-source components,
\[
 \mu(v,r)=\tfrac12\int_r^\infty\rho^2\mathcal T^v{}_v(v,\rho)\,d\rho,\qquad
 \beta(v,r)=-\tfrac12\int_r^\infty\rho\mathcal T_{\rho\rho}(v,\rho)\,d\rho .\tag{2}
\]
These are fixed-$v$ constraint integrals with asymptotic mass and clock fixed. The axial domain of dependence does not truncate (2).

In $(t,x)$, the three metric entries are $2\mu/r-2f\beta,\ 2\mu/r-f\beta,\ 2\mu/r$. Let $Y_b=\eta_b\Delta b_E/f$. Then $\operatorname{tr}Y_b=2\beta$, $\widetilde P_{2,{\rm mean}}=-\beta$, and
\[
 \widetilde M_{2,{\rm mean}}=(\beta I-Y_b)\eta_b
 =s_b\begin{pmatrix}1&-1\\-1&1\end{pmatrix},\qquad
 s_b=\beta-2\mu/(rf).
\]
The same rank-one form follows from writing the EF perturbation as $2(\mu/r-f\beta)dv^2+2\beta\,dv\,dr$ and removing its inverse-density trace part. In the derivative-reduced return equation this gives
\[
 F_{\rm mean}=2s_b(2w_{0,u}-fw_0/r),\qquad G_{\rm mean}=0,\qquad
 H_{\rm mean}=-\frac{f}{2r}F_{\rm mean}+\frac{\lambda f\beta}{r^2}w_0. \tag{3}
\]
Equation (3) removes an exact current cancellation before interval evaluation. Both $F_{\rm mean}$ and $H_{\rm mean}$ retain the full exterior constraint data.

## Leading and coupled mixed fields

The leading scalar satisfies $L_{\rm RW}w_0=0$, $L_{\rm RW}=-4\partial_u\partial_v-V$, $V=f(6/r^2-3/r^3)$. On $\Sigma$, $w_0=a_\delta(u)$ and $w_{0,t}=-w_{0,x}=a_\delta'(u)$, with $a_\delta(u)=\sqrt\delta\,b_0((u-a)/\delta)$, $\delta=3/25$, and $b_0$ the normalized flat bump of the prescribed preparation. In the bulk
\[
 A_{0t}=(w_{0,x}+fw_0/r)/(2r),\qquad A_{0x}=w_{0,t}/(2r),\qquad Q_{0z}=\lambda r w_0/2 .
\]
At $+k$ define $J^a=\lambda(M_1A_0)^a_{+k}$, $J^z=0$, $X=(P_1F_0^{\rm curl})^{tx}_{+k}$, $U=-Q_{1z}$. Put $d_k=\lambda+k^2r^2$, $D=r^2d_k$, $W=\lambda r^4/d_k$ and $L_c=r^4/D$. The noncompact density rows and curl give
\[
 D A_{1t}=J^t-U_x-ik r^4 A_{1z,t},\quad
 D A_{1x}=-J^x-U_t-ik r^4 A_{1z,x},\quad F^{\rm curl}_{1,tx}=f(U+X)/r^4 .\tag{4}
\]
Normalize $Y=(y,z_c)=(U/\sqrt D,\sqrt W A_{1z})$, $a_m=(\log\sqrt D)_x$, $b_m=(\log\sqrt W)_x$, $K=f(k^2+\lambda/r^2)$ and $C=2ik\sqrt\lambda f/d_k$. Substitution in the noncompact curl and compact row cancels the other polarization's mixed second derivatives. The operator and sources are
\[
\begin{split}
 (-4\partial_u\partial_v-K-a_m^2+a_{m,x})y+Cz_{c,t}
 &=\sqrt D\{\partial_t(J^x/D)+\partial_x(J^t/D)\}+f\sqrt D X/r^4,\\
 (-4\partial_u\partial_v-K-b_m^2-b_{m,x})z_c+Cy_t
 &=\{J^z-ik[\partial_t(L_cJ^t)+\partial_x(L_cJ^x)]\}/\sqrt W .
\end{split} \tag{5}
\]
The off-diagonal matrix is skew-Hermitian, and the stationary spatial form is $(\partial_x+\mathsf A)^\dagger(\partial_x+\mathsf A)+K$ with $\mathsf A=\operatorname{diag}(a_m,-b_m)$. At $k=0$ the first potential reduces to $V$ and the time coupling vanishes.

The graph tangential constraints give $U=A_{1z}=0$, $U_x=-pU_t$, $A_{1z,x}=-pA_{1z,t}$, and $A_{1x}=-p A_{1t}$. Zero independent compact electric density gives $(1-p^2)A_{1z,t}=ik(A_{1t}+p A_{1x})$. The normal and tangential combinations of (4) then give
\[
 (1-p^2)U_t=pJ^t-J^x,\quad
 \lambda r^2(1-p^2)A_{1z,t}=ik(J^t-pJ^x),\quad
 A_{1t}+pA_{1x}=\frac{J^t-pJ^x}{\lambda r^2} . \tag{6}
\]
In the last step $D-k^2r^4=\lambda r^2$ keeps the graph normalization. Equation (6) supplies both induced time jets, including the compact one.

## Complete return and graph contact

For the second-order mean axial density use
\[
 \mathcal D^a=\lambda\langle M_1^{ab}A_{1b}+M_2^{ab}A_{0b}\rangle,\qquad
 \mathcal C=\langle P_1F_1^{\rm curl}+P_2F_0^{\rm curl}\rangle^{tx}.
\]
The mixed curl contribution uses $-\widetilde P_1\Re(U+X)$, with both terms of (4), and the direct plus mean curl uses $(\widetilde P_{2,{\rm dir}}+\widetilde P_{2,{\rm mean}})Q_{0z}$. The mixed potential terms use both polarizations from (4), including the affine terms $J^t,J^x$. With $m=2r/\lambda$, $P_e=\mathcal D^x/r^2$, $Q_e=\mathcal D^t/r^2$, $c=\lambda f\mathcal C/r^4$, the returning master obeys
\[
 F=m(P_e-Q_e),\quad G=m(P_e+Q_e),\quad H=m_xQ_e-mc,\qquad
 L_{\rm RW}w_2=-F_u-G_v+H .
\]
The reconstruction rows are $\lambda r^2A_{2t}=Q_{2z,x}+\mathcal D^t$, $\lambda r^2A_{2x}=Q_{2z,t}-\mathcal D^x$. Using $Q_{2z}=0$, $A_{2x}+pA_{2t}=0$ and the tangential derivative of $Q_{2z}$ on $\Sigma$ yields $(1-p^2)Q_{2z,t}=\mathcal D^x-p\mathcal D^t$. It follows that $4w_{2,u}|_\Sigma=G-b'F$ with $b'=-(1+p)/(1-p)$. Multiplying by $\Theta(t-T(x))$ creates the wave contact $-(1-p^2)w_{2,t}\delta(t-T)$, while converting $-m\Theta(\partial_tP_e+\partial_x Q_e)$ to divergence form creates the opposing $m(P_e-pQ_e)\delta(t-T)$. This gives the contact-free upper weak equation after extension. The baseline displacement is nonzero inside its graph support and has separate contacts.

An independent graph normalization check uses the density transformation from $(t,x)$ to $(\tau,r)$, $\tau=t+x-r$. Its Jacobian determinant is $f$, so $\mathcal D^\tau=(\mathcal D^t+(1-f)\mathcal D^x)/f$ and $\mathcal D^r=\mathcal D^x$ on the graph. With $s=1/r=-p$, the canonical formula in the preparation, $\partial_\tau Q_{2z}=\mathcal D^r+s\mathcal D^\tau/(1+s)$, becomes $(\mathcal D^x-p\mathcal D^t)/(1-p^2)$. At fixed $x$ the time derivatives $\partial_t$ and $\partial_\tau$ coincide. This tests the density factors and tilt sign using a different chart.

## Evaluated early graph subcertificate

These are directed intervals conditional on the pinned GL growth bracket, the inherited horizon normalization construction, the geometric source, and the pinned directed outer mean at $r=2.5$. The latter enters as widened real enclosures $[-0.08,0.08]$ for the factored $\mu$ and $[0,0.2]$ for the factored $\beta$, read from the [outer mean result](../GLR-GOAL-011/checks/exterior_source.txt). The time factor of the factored coefficients is $e^{2\Omega v}$. A new real-axis quadrature on whole radial cells transports the GL jet from the horizon to the early terminal graph, evaluates (6), and integrates (2) from that graph to $2.5$. It uses the inherited Taylor flow and Ricci identities. It independently changes the evaluated physical interval, early graph window and assembly, without regenerating the GL spectral proof or the outer contour enclosure.

| Quantity | Directed enclosure, rounded out |
|---|---:|
| Single $+k$ mixed graph energy on early causal strip | $[0.00106754,0.00116131]$ |
| $U_t$ at $r=b(d)$ | $[-7.80836,-7.77764]$ |
| $\Im A_{1z,t}$ at $r=b(d)$ | $[0.370660,0.372347]$ |
| Factored $\mu$ over entire early graph | $[0.6439,0.8330]$ |
| Factored $\beta$ over entire early graph | $[2.9692,3.2478]$ |
| Full $F$ at $(d,b(d))$ | $[17.915,19.680]$ |
| Full $G$ at $(d,b(d))$ | $[-6.059,-5.935]$ |
| Full $w_{2,t}$ at $(d,b(d))$ | $[-2.346,-1.674]$ |
| Error $F_{\rm true}-F_{\text{zero mean at }13/8}$ at $(d,b(d))$ | $[0.65507,2.29416]$ |

The graph energy uses $\int(1-p^2)(|U_t|^2/D+W|A_{1z,t}|^2)\,dr/(2f)$. Both velocities are nonzero despite the zero independent upper data. The near portion $[b(d),13/8]$ of the factored mean contributes $\mu\in[0.01095409,0.01096310]$ and $\beta\in[0.04736128,0.04738525]$. The finite-radius truncation test uses the same axial preparation and changes only the integration endpoint in (2). Its difference in the table uses (3) directly to preserve the common lower-field factors. The strict graph $F$ difference is a physical constraint-truncation error. The graph point in this table and the terminal-ray integral $\mathcal F_\infty(d)$ are different evaluations.

The first componentwise ball calculation could not prove the mean difference because of dependency inflation. The successful calculation centers the Ricci coefficients in $(r,k,\Omega,\Omega+fn_r/n)$ before multiplying by $n^2$. The initial failure and both precision/cell runs are preserved in [checks](checks/). The shell bound uses a verified sign for both constraint densities throughout each early graph cell.

## Remaining compact error

On the compact strip one can convert certified stationary lower-field energies to fixed-$u$ errors. At each stage, the flux on $u=\mathrm{const}$ controls $\sqrt K e/2$ and $e_v+\mathsf A e/2$. One time commutation adds control of $\sqrt K e_t/2$ and $e_{tv}+\mathsf A e_t/2$. For a given finite cap, the positive minimum of $K$ controls $e_t$ on every fixed-$u$ ray, so $e_x=2e_v-e_t$ is bounded there. Multiplying by current coefficient envelopes from (1)–(4) then controls $A_F(d)$, $A_F$ and $A_H$ using Cauchy–Schwarz along the ray. For a pointwise-in-$v$ $A_G$ bound, differentiating the reconstruction once in $v$ also needs $e_{uv}$ from the PDE. The residual in that equation must be included in the ray norm before using a trace inequality. A sharper integrated adjoint can instead use weighted volume errors plus the cap $G$ trace. Coefficient and canonical graph errors have to enter the first-order residuals of (5), and coefficient uncertainty multiplying the numerical fields enters the final current enclosure separately. The graph calculations above supply initial subcertificates. Bulk residuals of conforming candidates, their first time commutations, the compact terminal-ray $F$ integral and the scalar return shear defect remain separate quantities.

The [separate exterior weak-tail bound](investigators/B-SOURCES.md) controls all rays, including endpoints, beyond its stated cap. It uses absolute fluxes of the true lower fields and no numerical compact candidate.
