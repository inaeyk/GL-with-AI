# Investigator E — adversarial reconstruction and controls

## Independence and evidence scope

This pass shares the ordered hierarchy, normalized GL ODE, scalar potential and fixed data at [GOAL-013 source SHA](https://github.com/inaeyk/GL-with-AI/commit/1e8e628f68af4cab174fcc44b81af4748a22115c). It also had the user-specified weak identity, earlier optical signs and findings from the [external GOAL-013 audit](https://github.com/inaeyk/GL-with-AI/blob/26a09054366933a197c34fa67f617af7da76e48d/research/sav/verification/GLR-GOAL-013/AUDIT.md) in the inherited task context. The work is a non-blind reconstruction of those inputs. The graph divergence and compact adjoint were re-derived before reading A's GOAL-014 treatment. The directed baseline program was implemented independently with the previously known diagonal two-scatter idea, a newly differentiated remainder, and different whole-cell range quadrature. The local graph factors were re-derived before inspecting D's calculation. The later numerical graph comparison had D's first numerical intervals visible and detected a discrepancy. The second pass includes A's, B's, C's and D's proposed results. No five-dimensional action reduction, GL spectral eigenpair proof or outer GL certificate was regenerated here.

## Independent graph and duality check

Use $J=(F-4w_v,G)$ and $\operatorname{div}_{u,v}J=H+Vw$. The graph's unnormalized outward normal is $(b',-1)$ and
\[
 J\cdot (b',-1)=b'F-G+4w_u-4(d/du)(w|_\Sigma).
\]
Thus the canonical zero-displacement jet $4w_u=G-b'F$ makes the distributional graph flux vanish. The flat front also gives no contact. Integrating the divergence over a single ray gives $(\mathcal F-4w)_u=\int_b^v(H+Vw)-G$ and the sharp test gives the upper and lower $g\mathcal F$ terms. At a seam $\phi=0$ the extra divergence is $[J]\cdot\nabla\phi\,\delta(\phi)$, where brackets denote the trace on the positive side minus the trace on the negative side. For $\phi=v-3-u/2$ a jump $[F]=1$, $[G]=[w_v]=0$ produces coefficient $-1/2$. A cell-interior differentiation does not include this measure.

A separate direct transposition starts from $P=w_u$ extended by zero below the graph. Since $w|_\Sigma=0$, $w=\mathcal I P$ with $\mathcal I$ the primitive from $a$. In distributions on a rectangle containing the physical domain,
\[
 4P_v=\partial_u F+\partial_vG-H-V\mathcal I P,
\]
where the currents are likewise zero-extended. The canonical jet is needed to cancel the graph delta in this equation. Backward propagation of $h_c$ is $h_v=\mathcal I^*(Vh/4)$, with $\mathcal I^*$ the primitive from $u$ to $d$. The operator accesses only the physical part of each ray for $u\ge \ell(v)$. At $d$, $h_v=0$. Integrating the derivatives of the extended currents against $h$ yields the terminal $F$ rays, $+\langle h_c,G_c\rangle$, and the volume $-\int(h_uF+h_vG+hH)$. Propagating the test backward from infinity has the same sign. Integrating the *exterior* $G_v$ term gives $-\langle h_c,G_c\rangle+\langle g,G_\infty\rangle$. The first term cancels exactly at the cap. Only the weak exterior route can use this cancellation without an independent cap $G$ error. The terminal $F$ ray survives, with weight $g(d)$ independent of the cap.

An exact fixture uses $b(u)=2-u/2$, $w=u(v-b)$, $F=4u$, $G=0$, $H=-Vw$ and cap $v=4$. The graph velocity $w_u=u/2$ is nonzero. Against $g=1$ on $[0,1]$ the cap overlap is $5/2$, entirely from the terminal ray. Zeroing the graph jet produces a spurious $+2u\delta(v-b)$ on the right of $4P_v$. The [symbolic check](../checks/e_controls.py) reproduces these signs. Independently, [A's two-variable polynomial control](../checks/a_adjoint.py) checks a nonzero cap $G$, nonzero potential and the seam terms.

A strong bound on $S=-F_u-G_v+H$ alone provides no bound on separate currents. For example, choose any smooth $\varphi$ supported beyond the cap and multiply $(F,G,H)=(\varphi_v,-\varphi_u,0)$ by an arbitrary constant. Then $S=0$ and the separate ray norms can be arbitrarily large. The separate current tails rely on the physical coefficient structure. The exterior constraint integral likewise cannot be inferred from the axial causal graph. A static spherical mass perturbation $\mu(r)$ that is constant inside a spherical annulus and zero beyond it is a Schwarzschild mass shift on the inside, and the linearized Einstein source of the metric is confined to the annulus. Its source is conserved by the linearized Bianchi identity. The asymptotic mass perturbation is zero, and the inner coefficient still depends on the annulus. This manufactured constraint example is separate from B's evaluated change in physical $F$ when the actual mean integral is truncated.

## Baseline derivative and terminal trace

Let $B=a_\delta$, $W_0=w_0$, $K=V/4$, and $\mathcal T$ the forward causal Volterra integral over $a\le z\le u$, $b(z)\le s\le v$, with the inner integral zero when $v<b(z)$. For each ray $A W=\int_{b(u)}^\infty K(u,v)W(u,v)\,dv$ and $J=A1$. Then $W_0=(1+\mathcal T)^{-1}B$ and
\[
 g=B'-JB+A\mathcal T B-A\mathcal T^2W_0.
\]
The kernel of $A\mathcal T$ on graph data is
\[
 L_2(u,z)=\int_{b(z)}^\infty K(u,v)\int_{b(z)}^vK(z,s)\,ds\,dv.
\]
It equals $J(z)^2/2$ at $u=z$. On the two strips $r>1.59$ and $|\partial_u\log K|\le1/4$. To see this, $\partial_x\log K=(-4r^2+9r-4)/(r^2(2r-1))$ and the two inequalities follow from $2r^3-9r^2+18r-8>0$ and $2r^3+7r^2-18r+8>0$ on $r\ge1.59$, by their values and positive derivatives there. Thus, for $L=1/4$ and $M\ge\sup J$,
\[
 |L_2(u,z)-J(z)^2/2|\le (u-z)L e^{L(u-a)}M^2/2,\qquad
 |\partial_u L_2(u,z)|\le L e^{L(u-a)}M^2/2.
\]
The diagonal replacement has zero error in its moving-endpoint derivative term. On a ray $|\mathcal T W_0|\le M e^{M(u-a)}\int_a^u B$ and $|\mathcal T^2W_0|\le M^2e^{M(u-a)}\int_a^u (u-z)B(z)\,dz$ by the positive Volterra majorant. For the derivative of $A\mathcal T^2W_0$, the graph term vanishes because $\mathcal T^2W_0=0$ there. The derivative on $K$ costs $L$, and differentiating $\mathcal T^2W_0$ contributes at most $J(u)^2\sup_v|\mathcal T W_0|/2$, from integrating the triangular double $K$ product.

Consequently the smooth continuum candidate
\[
 g_h=B'-JB+\tfrac12\int_a^u J(z)^2B(z)\,dz
\]
has pointwise error at most $(c_2+c_3)\int_a^u(u-z)B(z)\,dz$, and derivative error at most $(c_2+c_3/2)\int_a^u B(z)\,dz+Lc_3\int_a^u(u-z)B(z)\,dz$, where $c_2=L e^{L(u-a)}M^2/2$ and $c_3=M^3 e^{M(u-a)}$. The two verified $M$ values are $1.57$ and $1.60$. The code encloses the raw bump, its first two derivatives, the normalization, the diagonal integral and positive moments on dyadic cells. On flat end cells $q=1/(y(1-y))$ is at least the cell count, and $q^j e^{-q}$ decreases for $q>j$. The radius bound and ray mass bound are checked by Arb. Normalization is by the exact integral, enclosed by range sums. The numbers below are rounded outward from the finer [directed run](../checks/e_baseline_variant.txt).

| Quantity | Early | Packet |
|---|---:|---:|
| $N_{01}(d)$ | $[0.4122938,0.4128010]$ | $[0.0057401,0.0097220]$ |
| $N_{01}'(d)$ | $[158.9522,159.1479]$ | $[-0.018039,0.018039]$ |
| $\|N_{01}\|_2$ upper | $0.01365554$ | $1.0013712$ |
| $\|N_{01}'\|_2$ upper | $6.356068$ | $67.43553$ |
| $\|N_{01}-g_h\|_2$ upper | $1.70446\times10^{-10}$ | $2.77342\times10^{-4}$ |
| $\|N_{01}'-g_h'\|_2$ upper | $7.49756\times10^{-8}$ | $0.00386196$ |

The positive full-packet endpoint is a physical leading-news result. The quadrature used to compute a future scalar projection against the continuum candidate needs its own error bound. D uses a continuous dyadic primitive with a different numerical defect. Its bounds are checked against these independent, generally looser endpoint intervals.

## Distinct physical graph calculation and correction

Define the graph slope $p=T'(x)=-1/r$, $n=(1,-p)^T$, $o=(-p,1)^T$ and $q=A_0=((-B'+fB/r),B')^T/(2r)$. Let $\overline M_1$ denote the unhalved first density, including its growing time factor, and let $M_{2,\mathrm{dir}}$ already contain the quadratic cosine average. The compact graph constraint gives $A_{1x}=-p A_{1t}$ and $\lambda r^2(1-p^2)A_{1t}=n^TJ$ with $J=\lambda\overline M_1q/2$. The mixed mean is $\mathcal D_{\rm mix}=\lambda\overline M_1A_1$. Its graph time velocity is
\[
 w_{t,\rm mix}={ (o^T\overline M_1n)(n^T\overline M_1q)\over r^3(1-p^2)^2},\qquad
 w_{t,\rm dir}={2 o^TM_{2,\mathrm{dir}}q\over r(1-p^2)}.
\]
The prefactor $2/(\lambda r)$ comes from $m/r^2$, with $m=2r/\lambda$. Omitting $r^{-2}$ would introduce a separate error. The analogous mean graph contribution is $w_{t,\mathrm{mean}}=(\beta-2\mu/(rf))(2B'-fB/r)/(1-p)$, from the rank-one EF mean density. In all cases $w_u=(1+p)w_t/2$ for zero graph displacement. In terms of $\mathcal D$ this gives $w_u=o^T\mathcal D/[\lambda r(1-p)]$.

D's first program used $1+p$ in that last denominator, amplifying the two local summands by approximately $(1-p)/(1+p)\simeq4.22$. The independent run with real-axis central GL integration and a graph-aligned, compactified two-null-coordinate march flagged the discrepancy. Correcting that conversion restores agreement within the directed intervals. The central integration independently reproduces $(U_t,\Im A_{1z,t})=(21.07686714,-1.02562077)$ at $r=1.61$ in the GOAL-013 preparation. For the mean summand the empirical outer $\mu,\beta$ quadrature and GL matching are inherited GOAL-011 implementations, and the density contraction and null evolution are independently implemented. The three resolutions and contour choices give the following **empirical** projections.

| $u$ cells | Mixed local | Direct local | Fixed-mass mean | Sum |
|---:|---:|---:|---:|---:|
| 96 | $-3.41274279\times10^{-4}$ | $1.28526927\times10^{-4}$ | $3.83867617\times10^{-5}$ | $-1.74360590\times10^{-4}$ |
| 192 | $-3.40906196\times10^{-4}$ | $1.28388274\times10^{-4}$ | $3.83454895\times10^{-5}$ | $-1.74172432\times10^{-4}$ |
| 384 | $-3.40814165\times10^{-4}$ | $1.28353607\times10^{-4}$ | $3.83351702\times10^{-5}$ | $-1.74125387\times10^{-4}$ |

The `e_graph_projection.py` calculation checks only the homogeneous scalar propagation of the canonical graph velocity. The complete return also has the bulk current source. The subprojection is an exact linear decomposition in the prescribed chart and canonical preparation. It depends on the exact-curl convention if graph and bulk are separately reported. Its sign supplies no bound on the omitted bulk summand. [Independent code](../checks/e_graph_projection.py) · [Observed output](../checks/e_graph_projection.txt).

## Short-scale and frame controls

With the actual $V$ and a smooth nondecreasing $\rho(v)$ switching from zero below $v=8$ to one above $v=9$, take $w_n=\chi(u)\sin(nu)\rho(v)/n$ on $[0,1]$, $\chi=u^2(1-u)^2$, and $F_n=4(w_n)_v+\int_0^uV(z,v)w_n(z,v)\,dz$, $G_n=H_n=0$. The graph is below the switch and all required graph jets vanish. The analytic bounds are $\|w_n\|_X\le1/(16n)$ and $\|A_F\|_2\le9/(20n)$, because $r>2$ beyond the switch and $\int V\,dv\le6$. Endpoint current errors are also $O(1/n)$. But $N_n=\chi\cos(nu)+\chi'\sin(nu)/n$ has limiting squared norm $\|\chi\|_2^2/2=1/1260$. A fixed $H^1$ test pairs to zero in the limit, as the new dual bound requires. Scaling $w_n$ and currents by $\sqrt n$ and adding a baseline-test perturbation $\delta g_n=\chi\cos(nu)/\sqrt n$ makes both the weak-current error and the baseline $L^2$ error vanish while their paired error tends to $1/1260$. This strengthens the failure test for simultaneous weak-current and merely $L^2$ baseline control. The new $H^1$ test requirement excludes the sequence because $\|\delta g_n'\|_2$ grows as $\sqrt n$. No manufactured oscillation is attributed to the fixed GL solution. [Quadrature check](../checks/e_controls.txt).

The same derivative loss occurs on the lower massive terminal trace. Fix a compact patch with $v$ above the graph, $\rho\in C_c^\infty$ on that patch in $v$, and $\zeta\in C_c^\infty(\mathbb R)$ with $\zeta'(0)=1$. For the actual stationary two-component mixed operator on that compact patch choose a manufactured error $e_n=n^{-1}\zeta(n(d-u))\rho(v)e_1$, where $e_1$ is a unit polarization. Its graph and front errors vanish for large $n$. The terms $e_{uv}$ and $\mathsf C e_u$ are bounded on a strip of width $O(n^{-1})$, while $e,e_v=O(n^{-1})$. Therefore its residual tends to zero in $L^1_vL^2_u$ and $L^1_uL^2_v$ at rates $O(n^{-1/2})$ and $O(n^{-1})$, respectively. Yet $e_u(d,v)=-\rho(v)e_1$ and can couple to a nonzero transverse current weight. This example concerns residual-to-trace continuity. A physical residual estimate that uses its own terminal ray or a verified commutation excludes this defect.

For a cut relabel $u=\tilde u+\epsilon^2\xi(\tilde u)$, inertial news composes with the old label and the energy also uses the Jacobian $du/d\tilde u$. At fixed labels the coefficient of half the squared news acquires $\int(\xi N_{01}N_{01}'+\xi' N_{01}^2/2)=[\xi N_{01}^2/2]$. At physically corresponding cuts the leading energy integrated over the shifted endpoints has precisely the opposite term. The exact polynomial fixture gives $7/12$ before relabeling and $139/12$ at unchanged coordinate endpoints, with boundary correction $11$. An angular pullback must carry the tensor harmonic and sphere metric and area element with the cuts. The energy normalization follows from $\int X_A X^A=\ell(\ell+1)$ and the two integrals $\int D_A X_BD^A X^B=(\ell(\ell+1)-1)\int X_A X^A$ and $\int D_A X_BD^B X^A=-\int X_A X^A$. Their symmetrized sum gives $\int X_{AB}X^{AB}=12$ at $\ell=2$. Bondi tensor flux $\int N_{AB}N^{AB}/(32\pi G_4)$ and $du_{\mathrm{phys}}=r_0\,du$ give the stated $3r_0/(4\pi G_4)$ factor for the interference coefficient. The full angular tensor square remains nonnegative. This calculation is for the massless Einstein-frame tensor mode on fixed physical cuts.
