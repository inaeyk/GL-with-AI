# Investigator B — physical source and separate weak tails

The fixed benchmark and density assembly are in [Currents and data](../CURRENTS-AND-DATA.md). The result here is an independent fixed-$u$ tail calculation conditional on the physical coefficient and flux bounds at [1e8e628f68af4cab174fcc44b81af4748a22115c](https://github.com/inaeyk/GL-with-AI/commit/1e8e628f68af4cab174fcc44b81af4748a22115c). The input envelopes are [GOAL-013 B §1 and §3](../../GLR-GOAL-013/investigators/B-SOURCE.md). The inherited Ricci expansion, mode proof and outer mean certificate were used as inputs. The weak-current inequalities below use a different argument from the inherited differentiated source tail. The companion directed early-graph calculation uses a new domain and reconstructs the return current as well as the two graph velocities.

## Fixed-$u$ energies and undifferentiated reconstruction

Let $a\le u\le d\le 8.407362$ and $v_c=d+2x(r_c)$, $r_c\ge100$. Write $w_0$ for the real leading scalar and $Y=(y,z_c)$ for the actual $+k$ normalized mixed field. Use the maxima $h$ of the absolute unhalved GL entries and their first two $t,x$ jets and $b_*$ of the absolute Einstein-frame mean entries and their first $t,x$ jets. On the exterior inherited potential bounds include $2/r^2\le V\le6/r^2$, $K\ge1/4$, $|a_m|,|b_m|\le2/r$, $|a_{m,x}|,|b_{m,x}|\le10/r^2$, $|C|\le1$ and $\|V_{\rm eff}\|\le10$. Here $V_{\rm eff}=K+\mathsf A^2-\mathsf A_x$ for the mixed system. The first two scalar time commutations and the first mixed time commutation satisfy the same respective stationary operators. On each fixed-$u$ ray and each finite exterior segment their flux controls $\sqrt K\, e/2$ and $e_v+\mathsf A e/2$ in $L^2_v$, with $K=V$, $\mathsf A=0$ for the scalar. Absolute flux square roots from GOAL-013 are bounded by $10^{13}$ for each scalar and $10^{48}$ for each mixed commutation. These bounds include graph data and forcing. Monotone passage to the full exterior ray preserves them. They apply to the rays $u=a$ and $u=d$ as well as the interior.

Define
\[
 B_0=\frac{|w_0|+|w_{0,t}|+|w_{0,tt}|}{r}+|w_{0,v}|+|w_{0,tv}|,\qquad
 B_m=|Y|+|Y_t|+|Y_v|+|Y_{tv}|.
\]
The scalar $V$ lower bound gives each of its three weighted field norms at most $\sqrt2\,10^{13}$. The other two scalar terms are each bounded by $10^{13}$. For each mixed commutation $K\ge1/4$ gives a field norm at most $4\cdot10^{48}$. Removing $\mathsf A/2$ from the derivative costs at most $4\cdot10^{48}/100$, so each $v$ derivative has norm at most $2\cdot10^{48}$. Thus uniformly in $u$,
\[
 \|B_0\|_{L^2_v}\le8\cdot10^{13},\qquad
 \|B_m\|_{L^2_v}\le12\cdot10^{48}. \tag{1}
\]
In this estimate control of $Y_x=2Y_v-Y_t$ uses the weighted field of the first time commutation on the same ray. A fixed-$v$ estimate of $Y_u$ would give a different directional norm and would not suffice for the endpoint rays.

With the physical formulas of [Currents and data](../CURRENTS-AND-DATA.md), the following exterior estimates hold pointwise, taking the maximum norm on potential entries and Euclidean norms on $S_1,Y$:

| Quantity | Bound |
|---|---:|
| $|rA_0|_{\max}$ and its $t,x,v$ derivatives separately | $10r B_0$ |
| $|rJ/D|_{\max}$ and its $v$ derivative separately | $100 h r B_0$ |
| $|S_1|$ | $10^4 h r B_0$ |
| $|rA_1|_{\max}+|\partial_v(rA_1)|_{\max}$ | $30 B_m+10^5h r B_0$ |
| $\max(|F|,|G|,|H|,|F_v|,|G_v|)$ | $10^4h B_m+10^8(h^2+b_*)r B_0$ |

The first line uses $w_x=2w_v-w_t$, $w_{tx}=2w_{tv}-w_{tt}$ and $w_{xx}=w_{tt}+Vw$. For the second and third, $\widetilde M_1$ and each of its first derivatives have entry caps $2h$ and $3h$ at $r\ge100$. The actual $J$ is $2r\widetilde M_1(rA_0)$, so $rJ/D=2\widetilde M_1(rA_0)/d_k$ with $d_k\ge4$. For each derivative in the $y$ row of (5), use the factor $2/\sqrt{d_k}\le1$ after differentiating $\widetilde M_1(rA_0)/(rd_k)$, whose common radial coefficient has relative derivative at most $3/r$. For each derivative in the $z_c$ row, $kL_c/\sqrt W=k/(2\sqrt{d_k})\le1/(2r)$ and $|\partial_x\log(rL_c)|\le5/r$. Each row contains two such derivatives and each matrix product has two entries. The remaining $X$ term obeys $|X|=|3d_b Q_{0z}|\le3h|w_0|/r$ and $\sqrt D\le3r^2$. These bounds give less than $10^3 h r B_0$ in the third line before rounding.

For the fourth line, the derivative coefficients of $rA_1$ are $1/\sqrt{d_k}\le1/2$ and $k r/(2\sqrt{d_k})\le1/2$, with first $v$ derivatives bounded by $1/r$. The lower-order terms use $a_m,b_m$. Only $Y_{xv}$ needs a PDE substitution, $Y_{xv}=Y_{tv}-2Y_{uv}$ with $4Y_{uv}=-V_{\rm eff}Y+C\sigma_1Y_t-S_1$. Thus $|Y_{xv}|\le |Y_{tv}|+5|Y|+|Y_t|/2+|S_1|/2$. This avoids an estimate of $Y_{tt}$ at each outgoing ray. The affine terms are the second line of the table.

For the last line, the direct and mean $\widetilde M_2,\widetilde P_2$ and their first $v$ derivatives have entry caps $100(h^2+b_*)$. This follows from (1) of [Currents and data](../CURRENTS-AND-DATA.md), $|X_b|_{\max}\le2h$, $|\partial_v X_b|_{\max}\le3h$, $|d_b|\le h/2$, $|\partial_v d_b|\le h$, $|Y_b|_{\max}\le2b_*$ and $|\partial_vY_b|_{\max}\le3b_*$. Each $F,G$ uses $2$ times a sum or difference of two potential rows, with two entries per row. The $v$ derivatives cost one Leibniz derivative on each factor. The same potential bounds control $m_xQ_e$ with one fewer factor of $r$. The mixed curl term in $mc$ has magnitude at most $(2f/r^3)(3h/r^2)(\sqrt D|Y|+|X|)$, and the direct and mean curl term uses $(2f/r^3)\,100(h^2+b_*)|Q_{0z}|$. Adding these terms stays within the last line, including the affine $J$ and $X$ products. The estimate for $F_v,G_v$ is used to evaluate pointwise cap bounds. No derivative of $H$ is needed.

## Tail bounds including endpoint rays

With $\ell=v-v_c$, the inherited GL and full EF mean envelopes are
\[
 h\le10e^{-2(r_c-4)/5}e^{-\ell/6},\qquad
 h^2+b_*\le[100+10^9(r_c+6)]e^{-4(r_c-4)/5}e^{-\ell/3}.
\]
The second estimate includes the infinite fixed-$v$ integration and fixed asymptotic constants in (2) of [Currents and data](../CURRENTS-AND-DATA.md). Use $r\le r_c+5+\ell/2\le(r_c+5)e^{\ell/12}$ for $r_c\ge100$. Define
\[
 a_m^*=10^5e^{-2(r_c-4)/5},\qquad
 a_0^*=10^8[100+10^9(r_c+6)](r_c+5)e^{-4(r_c-4)/5},\qquad
 D_m^*=12\cdot10^{48}a_m^*,\quad D_0^*=8\cdot10^{13}a_0^* .
\]
The last line of the table is bounded by $a_m^*e^{-\ell/6}B_m+a_0^*e^{-\ell/4}B_0$. Applying (1) and Cauchy–Schwarz gives, for every $u$ including $d$,
\[
 \int_{v_c}^\infty |F(u,v)|\,dv,\quad
 \int_{v_c}^\infty |H(u,v)|\,dv\ \le\ T_1=\sqrt3 D_m^*+\sqrt2 D_0^* . \tag{2}
\]
For either $q=F,G$, each of $\|q\|_2,\|q_v\|_2$ on the ray is bounded by $D_m^*+D_0^*$. The $H^1$ representative on the half-line tends to zero at infinity. Integrating $(|q|^2)_v$ back from infinity gives
\[
 \sup_{v\ge v_c}|F(u,v)|,\quad \sup_{v\ge v_c}|G(u,v)|\ \le\ T_\infty=\sqrt2(D_m^*+D_0^*). \tag{3}
\]
The uniformity also gives $G_\infty=0$ in $L^2_u$ by dominated convergence. If a candidate is set to zero past $v_c$, the omitted contributions to $\|A_F\|_2,\|A_H\|_2$ are each at most $\sqrt{d-a}\,T_1$ and the omitted contribution to $\|A_G\|_2$ is at most $\sqrt{d-a}\,T_\infty$. The omitted terminal integral is bounded by $T_1$. Any seam created by extending the candidate by zero must be handled by the weak pairing itself or an explicit distributional interface term if using a cellwise strong equation.

| $r_c/r_0$ | $T_1$ upper | $T_\infty$ upper |
|---:|---:|---:|
| 100 | $4.374\times10^{37}$ | $3.571\times10^{37}$ |
| 330 | $4.850\times10^{-3}$ | $3.961\times10^{-3}$ |
| 350 | $1.627\times10^{-6}$ | $1.329\times10^{-6}$ |
| 400 | $3.354\times10^{-15}$ | $2.739\times10^{-15}$ |
| 500 | $1.425\times10^{-32}$ | $1.164\times10^{-32}$ |

The [directed arithmetic](../checks/b_weak_tails.py) evaluates the table independently of the strong exterior source number. The radius $100r_0$ fails this coarse absolute budget. Using the smaller cap $350r_0$ can be considered in a weak proof with these current bounds. A strong return estimate at that cap would need its own applicable source-tail and compact trace budgets. The scalar $Vw_2$ term of the weak map extends beyond the cap, even when the currents in the candidate vanish there. Use a scalar Volterra/adjoint transfer or otherwise bound that term over the exterior.

## Independent checks and limits

- The $\widetilde M_2$ rank-one mean formula was checked by direct matrix substitution and by the null form of the EF tensor. The compact graph constraint independently fixes both velocities and agrees with the previously audited orientation.
- [Early graph checks](../checks/b_graph_mean.py) use Arb at 160/192 bits over whole cells and the inherited real trace of the contour mean. Centered quadrature corrects a dependency-inflation failure preserved in [failure output](../checks/b_graph_mean_failures.txt). A directed physical difference in $F$ confirms the effect of dropping the outer mean, and the two numerical refinements retain nonzero physical $+k$ graph launches. These are graph subcertificates, conditional on the inherited GL and geometric inputs.
- Tail formulas (2)–(3) are proved under the pinned physical energy and coefficient envelopes. The stated numbers are directed arithmetic. They include pointwise cap trace errors in $F,G$ if those currents are set to zero at the cap, and do not invoke a strong estimate for $S_2$.
- The signed interference still requires the compact physical bulk current or adjoint residual errors, in particular $\int_{b(d)}^{v_c}(F-F_h)(d,v)\,dv$, plus the scalar shear defect and test errors. The graph-current sign does not determine the terminal-ray integral or the physical sign. The exact-curl formulation retains both massive polarizations, full mean, direct source, mixed source and induced upper jets.
