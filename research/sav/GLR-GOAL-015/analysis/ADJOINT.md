# Rotated weak certificate for the early interference

For the fixed ordered benchmark, the early physical projection belongs to $[1.70019,13.47799]$. The proof combines the exact sharp-cut scalar dual with a directed optical proxy and a whole-cell bound on its finite-width error in the complete two-polarization hierarchy. The proxy retains the leading part of the $F$ current. The estimate includes the omitted $G,H$, the fixed-mass mean, both mixed polarizations and the canonical graph jets.

The frozen inputs are the [GOAL-014 source](https://github.com/inaeyk/GL-with-AI/tree/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014), the [GOAL-014 audit](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/AUDIT.md), and the preceding source and audit links identified there. The principal optical coefficient uses [GOAL-011 Eq. (3)](https://github.com/inaeyk/GL-with-AI/blob/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011/WAVEFORM.md) and its [archived interval](https://github.com/inaeyk/GL-with-AI/blob/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011/checks/barrier_delay.txt). The exact-curl equations, sharp-cut formula, compact leading jet bounds and weak exterior enclosure are inherited. The new calculation consists of the rotated transport, matched current and source identity, and explicit estimate on the physical finite-width remainder. The initial derivation had access to GOAL-014 and its audit. The directed calculation uses no sampled compact-pilot lower fields.

## Sharp cut, graph, and cap

Set $r_0=1$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $f=1-1/r$, $R=13/8$, $a=R-2x(R)$, $d=a+3/200$, and $c=d+2x(400)$. The graph is $v=b(u)$ with $u=b-2x(b)$. Its $t=T(x)$ slope is $p=-1/r$, so $b'=-(1+p)/(1-p)<0$. Set $V=f(6/r^2-3/r^3)$, $W_0=V/4$ and $g=N_{01}$. The ordered return $w$ on $D_c=\{a\le u\le d,\ b(u)\le v\le c\}$ has
$$
 -4w_{uv}-Vw=-F_u-G_v+H,\qquad
 w|_\Sigma=0,\qquad 4w_u|_\Sigma=G-b'F .
$$
The graph jet follows by inserting exact-curl tangency $Q_x=-pQ_t$ in the two noncompact constraints and setting the tangential upper canonical potential to zero. Zero extension below the graph produces $-4w_u|_\Sigma\delta(v-b)$ on the left and $(b'F-G)|_\Sigma\delta(v-b)$ on the right. They agree. The flat front supplies no current contact.

The backward scalar dual on the whole strip is
$$
 Z(u,v)=g(u)-\int_u^d dz\int_v^\infty ds\,W_0(z,s)Z(z,s).
$$
Its traces are $Z(d,v)=g(d)$ and $Z(u,\infty)=g(u)$. It satisfies $Z_{uv}=-W_0 Z$ and $Z_u-g'=\int_v^\infty W_0(u,s)Z(u,s)\,ds$. The future rectangle stays above the graph. Apply $\partial_v(Zw_u)-\partial_u(wZ_v)=Zw_{uv}-wZ_{uv}$ and integrate the current divergence by parts on $D_c$. The graph terms cancel by the jet and the compact cap term is $\int Z(\cdot,c)G(\cdot,c)\,du$. Weak integration over the exterior introduces the opposite cap term. The separate weak exterior bound and $G(\cdot,\infty)=0$ give
$$
 4\mathcal P
 =g(d)\int_{b(d)}^c F(d,v)\,dv
   -\int_{D_c}(Z_uF+Z_vG+ZH)\,du\,dv + 4\mathcal P_{\rm ext},
 \qquad |\mathcal P_{\rm ext}|<6.948\,10^{-16}. \tag{1}
$$
The terminal contact has coefficient $g(d)$ at every radius, including the exterior. It also follows from differentiating the sharp test $\mathbf 1_{[a,d]}g$ distributionally. The exterior bound is for separate physical weak currents under the pinned GL and ordered flux envelopes. A truncation using the strong differentiated source retains the compact cap term.

## Exact rotation and optical primitive

Set $k=\pi/5$, $\lambda=4$, $d_k=\lambda+k^2r^2$, $D=r^2d_k$, $W=\lambda r^4/d_k$. The real normalized $+k$ field is $Y=(U/\sqrt D,\sqrt W\,\Im A_{1z})^T$. In this real system the time coupling is $4L$, where
$$
 L=\ell_0J_0,\qquad
 J_0=\begin{pmatrix}0&-1\\1&0\end{pmatrix},\qquad
 \ell_0=k\sqrt\lambda f/(2d_k).
$$
Use [GOAL-014 CURRENTS-AND-DATA](../../GLR-GOAL-014/CURRENTS-AND-DATA.md), Eqs. (4)–(6), with $a_m=(\log\sqrt D)_x$, $b_m=(\log\sqrt W)_x$, $K=f(k^2+\lambda/r^2)$. The exact mixed equation is
$$
 -4Y_{uv}-\mathsf VY+4L(Y_u+Y_v)=S,\qquad
 \mathsf V=\operatorname{diag}(K+a_m^2-a_{m,x},\ K+b_m^2+b_{m,x}).
$$
Let $\theta=\arctan(kr/\sqrt\lambda)$ and $O=\exp(\theta J_0)$. Since $r_v=f/2$ and $r_u=-f/2$, $O_v=OL$ and $O_u=-OL$. For $X=O^TY$, $P=X_u-2LX$, $Q=X_v$, one has
$$
 \begin{aligned}
 X_u &=2LX+P, &
 P_v&=-M_+X-\tfrac14 O^TS,\\
 Q_u&=2LQ-M_-X-\tfrac14 O^TS,&
 M_\pm&=O^T\mathsf VO/4\ \pm L_v-L^2 .
 \end{aligned}\tag{2}
$$
Indeed, $O^TY_{uv}=X_{uv}+LX_u-LX_v-(L_v+L^2)X$ and $Y_t=OX_t$. Rotation makes the fixed-$u$ equation for $P$ a primitive. The fixed-$v$ equations for $X,Q$ have unitary integrating factors. On the graph $X=0$, $P=O^TY_u$, $Q=O^TY_v$, with the canonical nonzero induced jets. The incoming fixed-$v$ end is either the graph or the flat front. The real system uses the actual $+k$ coefficient and the reality relation at $-k$.

Write $B=\sqrt\delta\,b_0((u-a)/\delta)/\|b_0'\|_2$ with $\delta=3/25$ and $b_0(y)=\exp[-1/(y(1-y))]$ on the support. Let $M_1$ be the unhalved first mass density divided by $r^2$, $\ell=M_1[x,:]-M_1[t,:]$ its row difference, $\alpha=1/\sqrt D$ and $\beta=k/\sqrt{\lambda d_k}$. The source $J=\lambda r^2M_1 A_0/2$ is the $+k$ coefficient. From $A_0=(w_v-w_u+fw/r,\ w_u+w_v)^T/(2r)$ for the baseline, the coefficient of $B''$ in $\partial_u(J^x-J^t)$ at $w=B$ is $j=\lambda r(\ell_x-\ell_t)/4$. In the two mixed source equations $\partial_u(J^x-J^t)$ multiplies $(\alpha,\beta)^T=Oe_1/(\sqrt\lambda r)$. Consequently
$$
 O^TS_{\rm opt}=q_0 B''e_1,\qquad
 q_0=\frac{\sqrt\lambda}{4}(\ell_x-\ell_t)
      =-\frac{\sqrt\lambda}{4}\frac{H_{LL}}f,\qquad
 H_{LL}=H_{tt}+2H_{tx}+H_{xx}. \tag{3}
$$
The metric coefficient includes the physical time factor, $e^{\Omega v}$ in EF. The mixed term in $F$ is $2r\ell A_1$, whose row on $Y_u$ is $2r(\ell_t-\ell_x)(\alpha,\beta)$. Its row on $P$ is therefore $(-8q_0/\lambda,0)$.

Define $A(u,v)=\int_{b(u)}^v q_0(u,s)ds$, $P_\star=-A B''e_1/4$, $F_\star=2q_0 A B''/\lambda$, and $G_\star=H_\star=0$. Then
$$
 \int_{b(u)}^v F_\star(u,s)ds
 =B''(u)A(u,v)^2/\lambda
 =B''(u)\chi(u,v)^2,\qquad \chi=-A/\sqrt\lambda . \tag{4}
$$
This gives the inherited leading factor $\chi_\infty^2/4$ after the $1/4$ in (1). The reference approximates a current and an outgoing component. Equation (2), with the full source and graph jets, governs the physical fields.

## Whole-cell finite-width error

On $[a,d]$ the inherited baseline bounds give $B'\ge522B$, $B''\ge380B'$, with all jets flat at $a$. The six slots $w_{0,uu}-B'',w_{0,uv},w_{0,vv},w_{0,u},w_{0,v},w_0$, divided by $B''$, have absolute bounds
$$
 \boldsymbol e=\left(0.004147,\ 1.124\,10^{-6},\ 4.740\,10^{-6},\
 1.003017/380,\ 8.181\,10^{-7}/380,\ 1.003017/(380\cdot522)\right).
 \tag{5}
$$
See [GOAL-014's pointwise jet bounds](../../GLR-GOAL-014/PHYSICAL-PROJECTION.md). Let $s_i(v)$ bound the Euclidean norm of the source coefficient for each slot and $s_E=\boldsymbol s\cdot\boldsymbol e$. The first coefficient norm is exactly $|q_0|$ by (3). The remaining rows follow from
$$
 S=(\alpha,\beta)^T\partial_u(J^x-J^t)
    +(\alpha,-\beta)^T\partial_v(J^x+J^t)
    +\begin{pmatrix}
      -2a_m\alpha J^t+f\sqrt D X_{\rm curl}/r^4\\
      -\beta(\log(r^4/D))_x J^x
     \end{pmatrix},
$$
with $X_{\rm curl}=3d_b Q_{0z}$, $d_b=H_{\theta\theta}/(2r^2)$, $Q_{0z}=\lambda r w_0/2$.

If $\Pi(v)$ bounds $|P(u,v)|/B''(u)$ on its active $u$ range, (2) gives $|X(u,v)|\le \Pi(v)B'(u)$. This uses the unitary fixed-$v$ integral and $\int_{u_{\rm in}(v)}^u B''\,du'\le B'(u)$. For a common $m(v)\ge\max(\|M_+\|,\|M_-\|)$, graph bounds $p_g,q_g$ in units $B'$, and $b_{\min}=b(d)$, it follows that
$$
 \begin{aligned}
 |P-P_\star|/B'' &\le p_g/380+
     \int_{b_{\min}}^v(s_E/4+m\Pi/380)\,ds,\\
 |Q|/B'' &\le [q_g\mathbf 1_{v\le b(a)}
          +(s_E+s_1)/4 +m\Pi/380]/380 .
 \end{aligned}\tag{6}
$$
The graph $Q$ term evaluates $B'$ at an earlier $u$ and is bounded by the target value of $B'$. For the matrix norm use $m=\max_j|\mathsf V_{jj}|/4+|\ell_{0,v}|+\ell_0^2$. The Volterra majorant for $\Pi$ is $p_\star+p_g/380+\int(s_E/4+m\Pi/380)$, with $p_\star\ge |A|/4$. On a cell of length $\Delta v$, take cell maxima of $p_\star,s_E$ and the previous cumulative error, forming an inhomogeneous upper bound $a_j$. Then $\Pi_j\le a_j\exp(m_j\Delta v/380)$ by scalar Grönwall. Update the two cumulative integrals with these outward cell maxima. This covers the entire physical terminal characteristic and volume.

The executable forms the mixed potential by
$$
 A_1=D_uOP+D_vOQ+[(D_u+D_v)OL+D_0O]X+(J^t,-J^x)^T/D,
 \qquad
 D_u=\begin{pmatrix}\alpha&\beta\\-\alpha&-\beta\end{pmatrix},
 \quad D_v=\begin{pmatrix}-\alpha&\beta\\-\alpha&\beta\end{pmatrix},
 \quad D_0=\operatorname{diag}(-\alpha a_m,-\beta b_m).
$$
The current assembly follows GOAL-014 and uses the rank-one exact mean and its $-\beta_{\rm mean}Q_{0z}$ curl with fixed mass and lapse at infinity. Coefficients are bounded on full cells by [the directed source geometry](../source/source_geometry.py). The $F$ $P$-slot uses $|P-P_\star|$ with row norm $8|q_0|/\lambda$. Other mixed slots use the true $P,Q,X$ bounds, and all baseline slots use (5).

The coefficient mesh uses $s=r(d,v)$. On $[s_0,s_1]$, all physical radii lie in $[s_0,s_1+(d-a)/2]$ by $r_u=-f/2$. The first $s$ is $81/50<b(d)$, so lower limits may be included with zero integrand. The signed phase interval advances by the cell box $q_0\Delta v$ with union with zero wherever any part can be below the graph. Within each cell the same union encloses a partial primitive. The proof bounds the exact physical solution by its characteristic integrals with the induced data. It uses no derivatives of sampled numerical fields and no piecewise field seams.

## Directed overlap and reproduction

For the tighter signed endpoint and bulk reference, use the inherited $\chi_\infty(a)$ interval. The entire $r\ge60$ optical tail is bounded by $7.187\,10^{-16}$ before multiplication by the launch factor, which is below one. Thus widening the interval by $10^{-14}$ encloses $\chi_c(a)$. Since $\partial_t q_0=\Omega q_0$,
$$
 \partial_u\chi_c=\Omega\chi_c+
 \frac{q_0(u,c)-(1-b')q_0(u,b(u))}{\sqrt\lambda}. \tag{7}
$$
The drive lies strictly below zero by directed graph and cap boxes. For distance $\ell\in[0,d-a]$ the constant-coefficient Duhamel kernel lies between $\ell$ and $\ell e^{\Omega(d-a)}$. The interval calculation uses this at $d$ and uniformly over the strip to bound the two squared phases.

For $g'=\lim w_{0,uu}$, the inherited estimate gives $0.995853B''\le g'\le1.004147B''$. Differentiation at infinity follows from the integrable ray potential and its $u$ derivative in the baseline ray formulas. Also $|g|\le1.003017B'$. Take $M=1.57$ bounding $\int W_0 dv$ on every ray, so $\sup|Z|\le e^{M(d-a)}\sup |g|$. Let $I_F,I_G,I_H$ denote the integrals of the uniform $u$ envelopes for $|F-F_\star|,|G|,|H|$ divided by $B''$. Define $I_{|F|},I_{WG}$ analogously for $|F|$ and $(\sup_u W_0)|G|$. In (1) subtract the signed reference $g(d)B''(d)\chi_c(d)^2-\int g'B''\chi_c^2du$. The remainder in $4\mathcal P$ is bounded by
$$
 |g(d)|B''(d)I_F+1.004147 I_F\int_a^d (B'')^2du
 +B'(d)\sup|Z|[MI_{|F|}+(d-a)I_{WG}+I_H]+4E_{\rm ext}. \tag{8}
$$
This uses $|Z_u-g'|\le M\sup|Z|$ and $|Z_v|\le (d-a)\sup_u W_0\sup|Z|$. The remaining $u$ integral of $B''$ equals $B'(d)$. For the signed bulk reference use the bounds on $g'/B''$, the squared phase, and the integral of $(B'')^2$. The terminal reference uses (7) and the inherited directed $g(d)$ interval. The bump norm is inherited, and the integral of $(B'')^2$ uses direct Arb cells on $[0,1/8]$. The first $32$ of $4096$ cells are handled together by the analytic bound $512t^8e^{-t}$ for $t=[y(1-y)]^{-1}$ from the GOAL-013 baseline certificate.

The primary directed replay gives $I_F<0.104521$, $I_G<11.037541$, $I_H<1.116639$, $I_{|F|}<2.493484$, $I_{WG}<1.301875$. The optical terminal lower in $4\mathcal P$ exceeds $70.17398$. The upper optical bulk subtraction is below $51.33608$. The terminal and bulk $F$ defect budgets in the same units are below $6.88610,4.26297$, with the remaining compact dual error below $0.88810$. The terminal primitive error is less than $16.692$, below the target $37$. The code evaluates the inequalities before decimal rounding. The resulting ordered Einstein-frame tensor energy coefficient lies in $[1.70019,13.47799]\,3r_0/(4\pi G_4)$ with $G_4=G_5/L$. A uniform finite-seed energy assertion also needs a nonlinear trace estimate.

Run from the scoped research directory:

    PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-015/analysis/early_optical_certificate.py

The [primary replay](early_optical_certificate.txt) records the bounds. The [floating diagnostic](optical_budget_pilot.py) estimated coefficient scales from the inherited central geometry and supplies no directed number. The first directed attempt on the first few bump cells divided by a dependency-inflated enclosure containing zero. It returned a nonfinite seed integral and failed the positivity assertion. The whole endpoint cap above and the signed phase transfer (7) replace that attempt. The coarse global rectangle accumulation for the final squared phase alone was too wide, near $[1\pm0.645]$, although it suffices for the intermediate $|P_\star|$ bounds. For a narrow terminal layer the bulk mixed work norms can tend to zero while the outgoing derivative trace remains fixed. Equation (6) uses the entire characteristic source envelope on $u=d$ and addresses that obstruction.

The finite-width result applies to the pinned GL and source enclosures, matched physical cuts and ordered hierarchy. The numerical weak candidate near $7.818$ is a separate calculation. The early estimate does not extend by monotonicity to the full packet because the $B',B''$ jet weights used in (5)–(6) cease to be positive there.
