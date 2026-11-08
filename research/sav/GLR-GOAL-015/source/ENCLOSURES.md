# Physical GL coefficients, graph jets and the mean early subprojection

The real-axis calculation encloses the normalized GL metric jets through two radial derivatives and the fixed-infinity Einstein-frame mean on a radial interval covering both physical graph cuts and the early compact cap. In the pinned exact-curl split, the complete mean contribution on the early cut satisfies
$$
0.1953656<\mathcal P_{\mathrm{early,mean}}<0.2197755.
$$
The directed signed mean terminal ray is $3.9762381<\int F_{\rm mean}(d,v)\,dv<4.0481101$, with a positive endpoint contribution between $0.4100965$ and $0.4175093$. The two induced massive graph polarizations, including their once-commuted incoming data, are enclosed separately. The complete physical sign also depends on the mixed bulk and direct terms.

## Conventions and dependencies

Use $r_0=1$, $f=1-1/r$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $k=\pi/5$, $\lambda=4$, and $\Omega\in[0.06328118,0.06328120]$. The physical unhalved GL tensor is $e^{\Omega v}\widehat H(r)\cos kz$ in ingoing EF coordinates. The normalization is $H_{\theta\theta}(t=0,r=2)/8=1$. The mean fields have time factor $e^{2\Omega v}$, with ADM mass and lapse fixed at infinity. The canonical axial preparation is the real $\ell=2$ packet $B(u)$, beginning at $R=13/8$ on $v=r$, with width $\delta=3/25$. Write $a=-13/8-2\log(5/8)$, $d=a+3/200$, and $b(u)=r_\Sigma(u)$. The early cap is $c=d+2x(400)$, which includes radii slightly above 400 on rays $u<d$.

The pinned GOAL-014 source is [0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195) and its external audit is [84c9824f826e3517c8a1ed74d96bea061ab88584](https://github.com/inaeyk/GL-with-AI/commit/84c9824f826e3517c8a1ed74d96bea061ab88584), consulted as read-only evidence. The ordered action, density matrices, canonical graph relations and baseline controls come from [GOAL-014 currents (1)–(6)](../../GLR-GOAL-014/CURRENTS-AND-DATA.md) and the [GOAL-013 source ledger](../../GLR-GOAL-013/SOURCE-LEDGER.md) at [1e8e628f68af4cab174fcc44b81af4748a22115c](https://github.com/inaeyk/GL-with-AI/commit/1e8e628f68af4cab174fcc44b81af4748a22115c). The spectral connection and nodelessness are inherited from [GOAL-005 §1](../../physics/goal005_enclosures.md) and the [GOAL-013 B §1 exterior envelopes](../../GLR-GOAL-013/investigators/B-SOURCE.md) use that input. The GL radial normalization and EF reconstruction reuse the GOAL-011 implementation at [eb439228601a649c658ce719b503ff0bdc37a83f](https://github.com/inaeyk/GL-with-AI/commit/eb439228601a649c658ce719b503ff0bdc37a83f), as present at the pinned source. I read the prior source and review results before the reconstruction. The new real-axis right leg, off-diagonal Ricci contraction and separated mean projection are different calculations using shared physical inputs. I did not regenerate the GL eigenvalue existence theorem or the ordered vacuum reduction.

## Real GL profile enclosure

Let $U_g$ be the static master, $p_g=(U_g)_x/U_g$, and $n=e^{-\Omega x}U_g$. Then
$$
(U_g)_{xx}=(V_g+\Omega^2)U_g,\qquad
(p_g)_x=V_g+\Omega^2-p_g^2,\qquad
n_r=(p_g-\Omega)n/f .
$$
The lower leg $9/8\le r\le 5/2$ reuses the directed horizon Taylor normalization and real `CompactGL` propagation. For the right leg choose $q=\sqrt{k^2+\Omega^2}$, $\alpha=k^2/(2q)$, and $\ell=-q+\alpha f/r$. The difference $P=p_g-\ell$ satisfies $P_x=B_g-2\ell P-P^2$ where $B_g=V_g+\Omega^2-\ell_x-\ell^2$. Put $R_*=512$, $A=q-\alpha/R_*>0$, $D_*=1+k^2R_*^3$. The rational coefficients give
$$
 |B_g(r)|\le \frac{1}{r^2}
 \left[k^2+6+11/R_*+\alpha^2+\alpha(1+2/R_*)
       +18/D_*^2+6(4+5/R_*)/D_*\right],\qquad r\ge R_* .
$$
If $B_*$ is this bound at $R_*$, the backward integral for $P$ has kernel mass at most $1/(2A)$. The radius $B_*/A$ is an invariant contraction ball by the two checked quadratic inequalities in `endpoint_radius`. The resulting negative bounded $p_g$ reconstructs the decaying physical solution. The endpoint slope uncertainty is below $4.129\times10^{-5}$. The bulk right leg walks inward from $R_*$, where errors contract on the stable branch.

On each descending radial cell $r(s)=r_j+h s$, $0\le s\le1$, a cubic $P_h$ interpolates the float64 proposal values and slopes. Clear the exact rational denominators in
$$
\mathcal R=f\frac{dP_h}{dr}+2\ell P_h-B_g+P_h^2
$$
before evaluating the residual polynomial over the whole cell. For $z=P-P_h$ the error equation in $s$ is
$$
 z_s=-2x_s(\ell+P_h)z-x_s z^2-x_s\mathcal R.
$$
Within a proposed radius $\rho$, an Arb upper bound on $-2x_s(\ell+P_h)+|x_s|\rho$ gives the exponential comparison rate. The resulting scalar upper solution, initialized with the preceding endpoint error, is monotone on the cell and stays strictly below $\rho$. A first-contact argument therefore bounds the full cell and its endpoint. The small right-end error after propagation is not supplied by the numerical solve tolerance.

Integrate the exact $-q+\alpha f/r$ part analytically and the cubic $P_h$ against $dx/dr=1+1/(r-1)$. A geometric polynomial for the latter rational coefficient plus its remainder and the already certified $P-P_h$ bound encloses the log amplitude. Match the amplitude to the normalized left branch at $r=5/2$ and check overlap of the two slope enclosures there. This uses the inherited spectral matching theorem for the true $\Omega$. The overlap itself is only a cross-check. Arb and the exact GL ODE then enclose all EF jet components. In the `metric_jets` evaluation, each normalized component of each jet is exactly affine in $p_g$. A centered degree-one series in $p_g$ avoids much of the repeated dependence. The first two jet levels also intersect a radial centered value plus a mean-value derivative enclosure.

## Fixed-infinity mean and an independent mass identity

The mean source equations at fixed $v$ are
$$
 \widehat\mu_r=-\tfrac12r^2\widehat{\mathcal T}^v{}_v,\qquad
 \widehat\beta_r=\tfrac12r\widehat{\mathcal T}_{rr}.
$$
The right-hand side of each equation is a quadratic in $(n,n_r)$. Factoring $n^2$ makes it a quadratic in $p_g$, evaluated as a centered degree-two Arb series at the radial midpoint. Differentiating the physical GL radial trajectory and its source map gives a whole-cell bound on the second radial derivative of the mean integrand. On a cell of length $h$ the mean integral is enclosed by midpoint evaluation plus radius $h^3\sup|\partial_r^2(\widehat\mu_r,\widehat\beta_r)|/24$. Integrating backwards in $r$ starts at $R_*$ with two nonzero symmetric tail enclosures from GOAL-013 B §1 at $v=0$. Their radii are below $4.12\times10^{-238}$ and $8.03\times10^{-241}$. A partial-cell integral uses the directed derivative bound and the common outer endpoint. Truncating the integrals at the axial launch radius would change the physical constraint solution.

There is a second local mass formula for an exact, nonstationary, exponentially decaying GL eigenmode. At quadratic order the contracted 5D Bianchi identity and the linear vacuum equations imply background conservation of the quadratic Einstein source. Periodic averaging removes the compact divergence. The EF background is stationary and the quadratic stress is symmetric, so its $v$ component gives
$$
\partial_r(r^2\widehat{\mathcal T}^r{}_v)
=-2\Omega r^2\widehat{\mathcal T}^v{}_v.
$$
The off-diagonal source is $\widehat{\mathcal T}^r{}_v=-\widehat Q_{vv}-f\widehat Q_{vr}$ because trace reversal has zero mixed $r,v$ entry. Therefore
$$
\widehat\mu(r)=\frac{r^2\widehat{\mathcal T}^r{}_v(r)}{4\Omega}
=-\frac{r^2}{4\Omega}(\widehat Q_{vv}+f\widehat Q_{vr}).
$$
Both radial primitives vanish at infinity. A second geometric derivation uses $b=-e^{2\beta}F\,dv^2+2e^\beta dv\,dr$, $F=1-2m/r$. In spherical symmetry $G^r{}_v=-2\nabla^r\nabla_v r/r$, and $b^{rv}\Gamma^r_{vv}+b^{rr}\Gamma^r_{rv}=-F_v/2$. Hence $G^r{}_v=2m_v/r^2$ and the same mass formula follows directly. The fixed-mass identity requires $\Omega\ne0$, the on-shell GL mode and the specified vanishing asymptotic mass correction. It does not localize the lapse integral.

For a direct reconstruction of the local source let the factored EF perturbation entries be $(A,B,C,W)=(H_{vv},H_{vr},H_{rr},H_{\theta\theta})$, $L=A+fB$, $M=B+fC$. The contraction of the background $vv$ and $fvr$ rows of both the orbit metric and the sphere Hessian vanishes. The contracted quadratic 4D Ricci contribution before the circle average is
$$
\tfrac12 L\,\operatorname{Scal}[b]^{[1]}
-\frac{\mathcal H_2}{r^2}+\frac{W\mathcal H_1}{r^4}
+\frac{\Omega W(\Omega W+fW_r)}{2r^4}
-\frac{2f\Omega W^2}{r^5},
$$
where $\mathcal H_1=(\Omega-f_r/2)\Omega W+f\Omega W_r-2r C^r_{vv+fvr}$ and $\mathcal H_2=(2rL-\Omega W) C^v_{vv+fvr}+(2rM-W_r) C^r_{vv+fvr}$. Here $C^i_{jk}$ is the first connection variation, with the background $f$ inserted only after contraction. The compact contribution after averaging is $k^2 L[(B+M)/4-\operatorname{tr}_4(H)/8]$. These give [the local implementation](mean_flux.py). The radial quadrature and local formula overlap at all seven reported test radii. At $r=2.5$, the local enclosure $[0.02330000,0.02331977]$ is tighter than $[0.02272077,0.02389835]$ from radial quadrature alone. Both use the same exact GL input. The source interface intersects the two valid mass enclosures. The adversarial track separately checked the Bianchi sign and stationary Killing current argument. The first wrong Hessian sign produced disjoint intervals and is preserved in [failures](failures.txt).

At $r=b(d)$, the radial coefficients are
$$
\begin{array}{ll}
\widehat\mu\in[0.76710870,0.76739749],&
\widehat\beta\in[3.17850485,3.17899424],\\
\widehat\mu_r\in[-3.09771064,-3.09723197],&
\widehat\beta_r\in[-13.40669691,-13.40546176].
\end{array}
$$
Both radial coefficients and their derivatives have whole-cell enclosures rather than just these point intervals. The implementation uses only the pinned radial equations and the physical real decaying branch.

## Constraint-compatible graph data through one time derivative

Let $p=T'=-1/r$ be the graph tilt, distinct from $p_g$, and $D_\Sigma=\partial_x+p\partial_t$. On $v=r$, $D_\Sigma=f\,d/dr$ on all trace functions. Zero displacement and canonical tangential/electric constraints give
$$
 U_t=\frac{pJ^t-J^x}{1-p^2},\qquad
 \operatorname{Im} A_{z,t}=\frac{k(J^t-pJ^x)}{\lambda r^2(1-p^2)}.
$$
The actual first insertion uses $J=\lambda r^2\widetilde M_1 A_0/2$. Set $Y=(U/\sqrt D,\sqrt W\operatorname{Im}A_z)$, $D=r^2(\lambda+k^2r^2)$, $W=\lambda r^4/(\lambda+k^2r^2)$. The real time coupling matrix $\mathsf C$ has off-diagonal entries $-\sigma,\sigma$, where $\sigma=2k\sqrt\lambda f/(\lambda+k^2r^2)$. Let $g=Y_t|_\Sigma$ and $g'=D_\Sigma g$. The mixed operator $-Y_{tt}+Y_{xx}-V_eY+\mathsf C Y_t=S$ on the graph yields
$$
(1-p^2)Y_{tt}=\mathsf Cg-S-p'g-2pg',\qquad
Y_{tx}=g'-pY_{tt}.
$$
Indeed, differentiating $Y_x=-pg$ and $Y_t=g$ along the graph gives $Y_{xx}+pY_{tx}=-p'g-pg'$ and $Y_{tx}+pY_{tt}=g'$. In a second reconstruction differentiate $Y_u=(1+p)g/2$ and $Y_v=(1-p)g/2$ tangentially and insert $4Y_{uv}=\mathsf Cg-S$. The resulting intervals for $Y_{tt},Y_{tx}$ overlap the first calculation on each of 512 cells on both windows. The first commuted graph field has nonzero displacement $g$ and derivatives $(Y_{tt}\mp Y_{tx})/2$. The graph enclosure alone supplies no bulk residual for this commuted field.

At the early terminal graph point the raw brackets are $U_t\in[-7.805231,-7.780770]$ and $\operatorname{Im}A_{z,t}\in[0.3708072,0.3721991]$. The two components of $Y_{tt}$ lie in $[2223.45,2234.99]$ and $[-1308.48,-1302.80]$ respectively. The one-$+k$ uncommuted incoming energy is enclosed by $[0.00105915,0.00116985]$ early and $[5.46258,7.19022]$ on the packet. The packet result concerns only the incoming graph energy. These bounds use $du\,(1+p)|g|^2/2$. The integration over the real Fourier pair would double the quoted one-coefficient energy.

## Complete early mean subprojection

The rank-one inverse-density correction gives $G_{\rm mean}=0$, $F_{\rm mean}=2s_b(2w_{0,u}-fw_0/r)$, and $H_{\rm mean}=-fF_{\rm mean}/(2r)+\lambda f\beta w_0/r^2$, where $s_b=\beta-2\mu/(rf)=-\Delta b_{E,tx}/f$. Use the infinite-cap adjoint identity from [GOAL-014 (A5)–(A7)](../../GLR-GOAL-014/ADJOINT-AND-ENDPOINTS.md) in this separated linear block. It includes the mean-induced upper graph velocity through the current contact. Write $g=N_{01}$, $I_F(u)=\int_{b(u)}^\infty F_{\rm mean}(u,v)dv$, and $\widehat h$ for the transported test. Then
$$
4\mathcal P_{\mathrm{early,mean}}=g(d)I_F(d)
-\int_a^d g'I_F\,du
-\iint_{D_\infty}[(\widehat h_u-g')F_{\rm mean}+\widehat h H_{\rm mean}]\,dv\,du .
$$
For this block the signed endpoint and volume are separately small enough for a simple physical reference. Let $s_d(r)=e^{2\Omega(d+2x(r))}(\widehat\beta-2\widehat\mu/(rf))$, $A_*=\int_{b(d)}^\infty (2/f)s_d(r)dr$, and let $A_{\mathrm{abs}}$, $A_f$, $A_\beta$ bound the absolute radial moments of $s_d$, $(f/r)s_d$, and $f|\beta_d|/r^2$ with measure $2\,dr/f$. Put $A_{\mathrm{near}}=\int_{b(d)}^{b(a)}(2/f)|s_d|dr$. Changing the reference to $u<d$ scales $s_d$ by $e^{2\Omega(u-d)}$ and changes the lower limit, so the signed moment varies by at most
$$
\Delta A=(1-e^{-2\Omega(d-a)})|A_*|+A_{\mathrm{near}}.
$$
For $r\ge100$, the GOAL-013 B §1 (3) bound gives $b_*=10^9(r+1)e^{-4(r-4)/5}$, $|s_b|\le b_*/f$, and $|\beta|\le2b_*/f$. The omitted $A_{\mathrm{abs}}$ tail is at most $1.155\times10^{-22}$. The factor $f/r\le1/4$ gives the corresponding $A_f$ tail, and the $A_\beta$ tail uses $2/r^2\le2/100^2$. The moments up to 100 are evaluated on whole radial cells with the physical factored coefficients, including the true radial mass and lapse constraint integrals.

The [pinned early baseline estimates](../../GLR-GOAL-014/PHYSICAL-PROJECTION.md) give $|w_0-B|\le\varepsilon B$, $|w_{0,u}-B'|\le\varepsilon B'$, and $|w_{0,uu}-B''|\le\varepsilon_2 B''$, with $\varepsilon=0.003017$, $\varepsilon_2=0.004147$, and $B\le B'/\kappa$, $\kappa=522$. These hold on every physical ray and pass to the $g,g'$ limits by the same integrable Regge–Wheeler kernels. On the open early interval $B',B''\ge0$. Thus
$$
\begin{split}
 |I_F(u)-4A_*B'(u)|&\le c_F B'(u),\\
 \int|F_{\rm mean}|dv&\le b_F B'(u),\\
 \int|H_{\rm mean}|dv&\le b_H B'(u),\\
 c_F&=4\Delta A+4\varepsilon A_{\rm abs}+2(1+\varepsilon) A_f/\kappa,\\
 b_F&=4(1+\varepsilon) A_{\rm abs}+2(1+\varepsilon)A_f/\kappa,\\
 b_H&=b_F/8+\lambda(1+\varepsilon)A_\beta/\kappa.
\end{split}
$$
The reference is $\mathcal P_*=A_*B'(d)^2/2$ using $\int_a^d B''B'=B'(d)^2/2$. The endpoint error multiplied by four is bounded by $(c_F+\varepsilon b_F) B'(d)^2$, and the leading test derivative volume error by $(c_F+\varepsilon_2 b_F) B'(d)^2/2$. The remaining exact potential and $H$ contributions cost at most $e^{M(d-a)}\|g\|_1(M b_F+b_H)B'(d)$ in the same units, with $M=1.6$ and $\|g\|_1<0.000791631$ from the pinned directed test. The signed terminal ray itself uses the signed $A_f$ moment and the baseline errors at $d$. This argument controls the true potential and all graph and terminal contacts in this block. The direct and mixed bulk terms require separate bounds. The full packet and finite-amplitude radiation remain separate questions.

The mean error components in $\mathcal P$ units are below $0.00621549$ for the endpoint replacement, $0.00334446$ for the leading derivative volume and $0.00141666$ for the true dual correction and $H$. They enclose $\mathcal P_*$ in $[0.20634220,0.20879890]$ with a strict resulting lower margin $0.1953656$. The inherited floating mean center $0.2064653$ lies within the new directed interval. The estimate proves the sign only in the fixed exact-curl decomposition and matched physical window.


## Direct and affine early subprojections

For the direct term use the matrices of GOAL-014 (1). The mixed density reconstruction has a local part $A_{1,\mathrm{aff}}=\operatorname{diag}(1,-1)J/D$ and the local curl contribution $-6d_bX=-18d_b^2Q_{0z}$. Thus its effective mass and curl coefficients are
$$
M_{2,\mathrm{aff}}/r^2=\frac{\lambda}{2d_k}\widetilde M_1
  \operatorname{diag}(1,-1)\widetilde M_1,\qquad
\widetilde P_{2,\mathrm{aff}}=-18d_b^2,\qquad d_k=\lambda+k^2r^2 .
$$
This splitting removes the $J,X$ terms from the reconstruction of the dynamic mixed contribution without changing the mixed evolution or its graph data. The direct and affine contributions each include their return graph contact.

For $\ell=(-1,1)^T$, the coefficient of $w_{0,u}$ in $F$ is $\ell^T\widetilde M_2\ell$. Cayley–Hamilton, $\operatorname{tr}X_b=-4d_b$ and $\ell^T\eta_b\ell=0$ give
$$
\ell^T\widetilde M_{2,\mathrm{dir}}\ell
=-\frac{2d_b}{f}(H_{tt}+2H_{tx}+H_{xx}).
$$
For the affine coefficient, use $(\widetilde M_1\ell)_t=(H_{tt}+H_{tx})/f$ and $(\widetilde M_1\ell)_x=-(H_{tx}+H_{xx})/f$. The signed difference of squares contains $H_{tt}-H_{xx}=4fd_b$. Hence
$$
\ell^T\widetilde M_{2,\mathrm{aff}}\ell
=-\frac{\lambda}{d_k}\ell^T\widetilde M_{2,\mathrm{dir}}\ell .
$$
The combined outgoing derivative coefficient is $k^2r^2/d_k$ times the direct one. This cancellation applies to the specified derivative coefficient, while the other current coefficients have their own bounds. The numerical implementation intersects the matrix and reduced expressions on each radial cell. An exploratory evaluation using $\operatorname{diag}(-1,1)$ in the affine potential gave the opposite sign and is preserved in [failures](failures.txt).

In either block write $F=F_u^{(c)}w_{0,u}+F_v^{(c)}w_{0,v}+F_0^{(c)}w_0$, and similarly for $G,H$. Let $L_*$ be the signed radial moment of $F_u^{(c)}$ at $u=d$. Its variation from graph and time shift has the same bound as $\Delta A$ above. The other terms are bounded by the absolute radial moments and the inherited whole-strip estimates $|w_{0,u}-B'|\le0.003017B'$, $|w_{0,v}|\le8.181\times10^{-7}B'$ and $|w_0|\le(1+0.003017)B'/522$. The reference $I_F=L_*B'$ gives $\mathcal P_*=L_*B'(d)^2/8$ with both endpoint and volume terms retained. The exact dual $G$ term costs at most $(e^{M(d-a)}-1)\|g\|_1\sup|G|/4$ in projection units. Beyond $r=100$, the inherited coefficient bound $h\le10e^{-2(r-4)/5}$ and the density formulas imply that all three coefficient rows of either block are bounded componentwise by $100h^2$. The analytic tails are included in the directed intervals.

| Early block | Signed $F$ terminal ray | Complete physical contribution |
| --- | ---: | ---: |
| Direct | $[5.62301354,5.79987489]$ | $[0.26395169,0.32687614]$ |
| Affine | $[-4.40006944,-4.27956455]$ | $[-0.24846886,-0.20046919]$ |
| Mean, direct and affine sum | | $[0.21084844,0.34618244]$ |

The signed contributions are conditional on the fixed ordered preparation and exact-curl split. The dynamic part of the mixed return has a separate causal evolution and error budget.

## Reproduction and interface

From `research/sav` run

```sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-015/source/verify_source.py
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-015/source/local_projection.py
```

The [geometry, graph and mean output](verified-source.txt) and [direct and affine output](local-projection.txt) record Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0, 160 Arb bits, and 6016 radial cells. Float64 DOP853 values are approximation proposals at `rtol=5e-14` and `atol=1e-16` on the right. The acceptance checks use Arb residuals, the growth bracket and explicit quadrature remainders. These are bounded fixed-background calculations.

`GeometryEnclosure(refinement=2).evaluate(r_arb)` returns the factored EF $H,H_r,H_{rr}$, $(\widehat\mu,\widehat\beta)$, and $(\widehat\mu_r,\widehat\beta_r)$. Set Flint `ctx.prec>=128` and `ctx.cap>=64`. The radius interval must lie in $[25/16,512]$ and the intended compact query region is $1.59\le r\le401$. A mixed-strip implementation must multiply the factored fields by the corresponding $e^{\Omega v}$ or $e^{2\Omega v}$ and distinguish fixed-$v$ derivatives from $\partial_x|_t=\partial_v+f\partial_r|_v$. `GraphEnclosure(geometry).evaluate(u_arb)` supplies both induced graph jets and both first time commutations. These interfaces bound the exact physical coefficients and data over cells. For a separate interpolated compact candidate its mismatch requires a residual evaluated with a common local representation or an explicit whole-cell interval difference. There is no estimate here of the mixed bulk evolution defect, a mixed terminal-ray contribution, or the full strong cap news.
