# C. Compact fixed-background pilot and the strong-news gate

I reviewed the [GOAL-014 source](https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195), including [the pilot and its gate](../../GLR-GOAL-014/STRONG-NEWS-PILOT.md), [the complete budget](../../GLR-GOAL-014/ERROR-BUDGET.md), [C's source record](../../GLR-GOAL-014/investigators/C-NUMERICS.md), the compact, cap and graph scripts and the source outputs. I traced their imports through the GOAL-011 physical-overlap code (GL real/radial continuation, mean constraint solver, normalized bump, horizon-series/Taylor flow and EF jets) and the GOAL-013 directed graph helper. For the physical source conventions I used the [pinned ordered equations](../../GLR-GOAL-013/SOURCE-LEDGER.md). I used the [GOAL-013 finite-cut analysis](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md), [cap calculation](../../GLR-GOAL-013/investigators/C-COMPUTATION.md) and [separate Work audit](https://github.com/inaeyk/GL-with-AI/blob/26a09054366933a197c34fa67f617af7da76e48d/research/sav/verification/GLR-GOAL-013/AUDIT.md) as read-only dependencies. The source assertions, results and comparisons were visible before the check. I separately derived the scalar cap/work estimates below and constructed the nodal and seam controls. The script executions reuse the source code. All replayed runs are the documented bounded fixed-background calculations.

## Finite-cut estimate from the scalar equation

Use $r_0=1$, $D_u=d-a=3/200$ and $v_c=d+2x(400)$. For the physical return master $w_2$, the flat front implies $w_2=\mathcal I P$ on the cap rectangle, where $P=(w_2)_u$ and $\mathcal I P(u)=\int_a^u P(s)\,ds$. Thus
\[
 P_v=-\frac V4\mathcal I P-\frac{S_{21}}4 .
\]
The primitive has norm $2D_u/\pi$ on $L^2(du)$. One obtains this norm by the lowest mixed Dirichlet–Neumann eigenvalue for the primitive's derivative. Beyond the cap the RW potential decreases with $r$. The integral of $\sup_u V/4$ in $dv$ equals its integral on the $u=d$ ray. There $dv=2\,dr/f$ and $V=f(6/r^2-3/r^3)$, so the integral is $3/400-3/(4\cdot400^2)$. If $T(v_2,v_1)$ is the homogeneous propagator, its ordered series gives
\[
 \|T(\infty,v_c)\|\le e^\alpha,\qquad
 \|T(\infty,v_c)-1\|\le e^\alpha-1,\qquad
 \alpha=\frac{2D_u}{\pi}\left(\frac3{400}-\frac3{4\cdot400^2}\right).
\]
Every exterior subinterval satisfies the same amplification bound. Numerically $\alpha\simeq7.1574962\times10^{-5}$. The cap trace has $L^2$ convergence with the integrable coefficient and forcing. The inherited strong tail bound $B_c=\|S_{21}\|_{L^1_v L^2_u}/4<1.7\times10^{-6}$ for the $400r_0$ exterior gives a forced-news contribution below $1.701\times10^{-6}$ after amplification. The stronger signed transfer in GOAL-013 is also available. The ordinary norm estimate here suffices and controls the homogeneous change from the identity as well.

Let $p_c$ be the exact affine interpolant of the replayed calculation's binary strong cap ordinates, on exact $u_j=a+jD_u/192$. With $E_c=\|P(\cdot,v_c)-p_c\|_2$, Duhamel and the triangle inequality give
\[
 \|N_{21}-p_c\|_2\le e^\alpha E_c+(e^\alpha-1)\|p_c\|_2+1.701\times10^{-6}.
\]
Consequently, with $\phi=a_\delta$ and $E_0=\|N_{01}-\phi'\|_2$,
\[
 |\langle N_{01},N_{21}\rangle-\langle\phi',p_c\rangle|
 \le E_0\|p_c\|_2+\|N_{01}\|_2
       \left[e^\alpha E_c+(e^\alpha-1)\|p_c\|_2+1.701\times10^{-6}\right].
\]
This telescoping uses the bound on the **true** baseline norm. In particular it has no missing product of the two news errors. With the rounded certified inputs, direct rational upper arithmetic verifies the source's bound $0.024231+0.013648127E_c$. The rounded lower reference $7.80653$ leaves a strict margin of $0.00286661$ at $E_c=570$. That tolerance is about 90% of the candidate's norm. It suffices for this one signed test, with limited margin at the threshold. It supplies neither a waveform-accuracy claim nor a long-window template test.

## The evaluated lower-order defects

The free baseline lift $\phi(u)$ matches both uncommuted graph data. For the scalar field, $L_{\rm RW}\phi=-V\phi$. For its stationary time derivative, $L_{\rm RW}\phi'=-V\phi'$. Differentiating the leading graph data tangentially in $x$ gives
\[
 w_{0,tt}+p w_{0,tx}=(1-p)\phi'',\qquad
 w_{0,tx}+p w_{0,xx}=-(1-p)\phi'' .
\]
Together with $w_{0,xx}=w_{0,tt}+Vw_0$, these give the reported once-commuted displacement (exact) and velocity defect of magnitude $V\phi/(1-p^2)$. For a zero-displacement graph error $e_x=-p e_t$ and the graph energy in $dx$ is $(1-p^2)|e_t|^2/2$. Changing to $du$ yields $(1+p)|e_t|^2/2$. This checks the factors in the computed commuted graph energy. The mixed version has the same graph prefactor since the stationary connection and mass terms multiply the zero displacement, followed by the exact weights $1/D$ and $W$ for the two raw jets. The conjugate $-k$ mode contributes equal energy in this convention.

For a free residual $Vh(u)$, the first mixed work norm is at most $\|h\|_2\int\sup_u V\,dv$. The log derivative bound $|\partial_u\log V|\le1/4$ on the physical early strip gives the source's $L_1=4J_b e^{D_u/4}$. The second mixed norm is at most $\|h\|_1 \sup_u\left(\int_{b(u)}^\infty V^2\,dv\right)^{1/2}$. Direct radial integration of $2f(6/r^2-3/r^3)^2$ gives the stated $L_2$. This re-derives the graph-exact baseline work estimates. The sharper uncommuted trace comparison follows from
\[
 P_0(u,\infty)-\phi'(u)=-\tfrac14\int_{b(u)}^\infty Vw_0\,dv,\qquad
 \|w_0\|_{L^2_uL^\infty_v}\le e^{J_bD_u}\|\phi\|_2 .
\]
The upper mixed graph energy $4.10\times10^{-7}$ is for the candidate defined by the two binary raw jets interpolated in exact $u$, with the exact stationary rescaling and a compatible zero displacement. It is directed under the inherited GL data and the ordered graph identities. The checked defect does not give a commuted mixed graph or any mixed volume error.

## What the pilot does and where the proof stops

The weak shear march, derivative of that shear at the cap, and differentiated-current strong march have different scalar derivative treatments. They share the baseline grid and the same sampled mixed fields, direct geometry and fixed-mass mean. The discrete transpose applies the same discrete operators, so equality checks the linear algebra only. The mean is integrated with a finite outer endpoint and the candidate uses a central growth rate and a numerical GL matching splice. Its small finite-end proxy and log-derivative mismatch provide empirical diagnostics. The archived fine run's SciPy error-estimator warning recurred, with successful solver exit. The independent interval graph calculation deliberately uses different directed GL inputs for the exact side of its graph comparison. The three meshes and caps vary together. Their spread does not separate resolution, changing cut or shared coefficient errors.

For a conforming return lift $\widehat w$ let $e=w_2-\widehat w$ and let $R$ be its distributional residual against the **true** coupled source. The conserved scalar energy has $E=(|e_t|^2+|e_x|^2+V|e|^2)/2$ and $J=-e_t e_x$. Its two outgoing null flux densities are $|e_u|^2+V|e|^2/4$ on constant $v$ and $|e_v|^2+V|e|^2/4$ on constant $u$. Let $M$ bound the square roots of both fluxes over all truncations. Work on a diamond uses $e_t=e_u+e_v$ and $dt\,dx=du\,dv/2$, so
\[
 M^2\le E_{\rm in}
 +\tfrac12\left(\|R\|_{L^1_vL^2_u}+\|R\|_{L^1_uL^2_v}\right)M .
\]
The positive root plus $\|(\widehat w)_u(\cdot,v_c)-p_c\|_2$ is a valid upper bound for $E_c$. The front of $\widehat w$ must be exact or its incoming flux must be added to $E_{\rm in}$. The strong pilot supplies no conforming lift with evaluated cap matching, returning incoming flux, and both mixed norms for $R$. Using the numerically generated source in place of the true one leaves $-\partial_u(F-F_h)-\partial_v(G-G_h)+(H-H_h)$, including derivatives of mixed and leading reconstruction errors and coefficient errors. Those bounds need whole-cell data, propagation through the lower hierarchy, and seam compatibility or a separate measured seam term. They cannot be inferred from the graph error alone.

Two controls make the missing checks concrete.

- **Null seams after commutation.** In a compact strip let $e=(u-u_0)_+\rho(v)$, with $\rho$ smooth and zero near the graph. On the null seam $u=u_0$ the field and $e_v$ are continuous. Hence $-4e_{uv}-Ve$ has a piecewise bulk density with no seam measure. The stationary time derivative $\partial_t e$ jumps by $\rho(v)$, so $L_{\rm RW}\partial_t e$ contains $-4\rho'(v)\delta(u-u_0)$. A certificate for the uncommuted lift cannot be reused for its commuted error without proving trace compatibility or separately controlling this term.
- **Subcell cap news invisible to nodal equations.** On an evenly spaced $N$-panel $u$ grid let $\chi$ be smooth, compactly supported in $(a,d)$ and equal to one on the middle half of the interval. Let $\rho$ vanish below a cap above the graph and equal one near $v_c$. Add
  \[
   e_N(u,v)=\frac{D_u}{2\pi N}
   \left(1-\cos\frac{2\pi N(u-a)}{D_u}\right)\chi(u)\rho(v).
  \]
  At every $u$ grid line, the field, both first derivatives, and $L_{\rm RW}e_N$ vanish. The $L^\infty$ shear error shrinks as $N^{-1}$. The cap derivative on the middle half is $\sin(2\pi N(u-a)/D_u)$, so $\|(e_N)_u(\cdot,v_c)\|_2\ge\sqrt{D_u}/2$ at every $N$. Scaling the perturbation scales the invisible error. This is a manufactured residual using the true scalar potential and flat graph and front, with no assertion that it is a physical coupled solution. It shows precisely why pointwise nodal defect agreement and agreement between derivative treatments of shared arrays cannot provide a continuum residual-to-news bound.

## Disposition and next calculation

The early affine candidate and its arithmetic are new compared with GOAL-013. The GOAL-013 physical compact error persists, as do the open packet sign, later returning waveform, both QNM comparisons and the nonlinear trace bridge. Weak control of the early scalar projection does not bound the strong news or cover the long window $[0,8.407362]$. The strong route would require the three residual terms above on the actual early cap and a separate budget on any extended interval.

For the next sign test, choose a conforming coupled mixed lift at $r_c=400$ with the directed early graph traces. Enclose all compact coefficients and the fixed-mass integrals. Certify whole-cell residuals and the physical $F$ ray at $u=d$, then combine the signed terminal and weighted bulk errors with directed candidate quadrature. This route targets the cancellation between the empirical terminal contribution $+19.94308724$ and the remaining contribution near $-12.12498483$. The gate remains an inequality against a directed lower candidate overlap, so the float64 central value should not be used as the lower endpoint of the proof. If pursuing the independent strong route, match the affine cap data to the conforming return lift and enforce the exact front or add its flux explicitly. In either route, save the actual binary candidate ordinates' digest when publishing a candidate-specific margin.

[c-replay.py](c-replay.py) reproduces all seven documented C commands, compares their stdout with the source and separately evaluates the rational gate margin. Run the following command from the pinned tree's research/sav directory.

~~~sh
PYTHONDONTWRITEBYTECODE=1 python3 verification/GLR-GOAL-014/c-replay.py
~~~

The audit output is [c-replay.txt](c-replay.txt). The strong candidate binary array is generated in scratch. Its hash records the actually checked ordinates. The replay confirms the numerical archive and candidate arithmetic under the listed dependencies. The manufactured controls and the separate energy/transfer derivations are independent of those source runs.
