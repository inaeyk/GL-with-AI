# C. Directed null-cap transfer and manufactured source-tail controls

Inputs are the [GOAL-012 source](https://github.com/inaeyk/GL-with-AI/tree/87fd711bf2e8117467caad1ff52c1cd344966974/research/sav/GLR-GOAL-012), especially its finite-time bound and investigator B's strong source, and the [independent Work audit of the cap](https://github.com/inaeyk/GL-with-AI/blob/ae8c323e1e87abd0fff26eac75b05b701a7e7b1a/research/sav/verification/GLR-GOAL-012/WINDOWS.md). This is a separate cap discretization with exposure to those proofs. All checks are fixed-background and non-production. The physical cap data and returning source density enter as unevaluated inputs.

## A second ordering in the cap evolution

Use $r_0=1$, $x=r+\log(r-1)$, $f=1-1/r$, $u=t-x$, $v=t+x$, and let $a=-13/8-2\log(5/8)$, $U=8.407362$ and $D=U-a$. The scalar wave sign is $L_{\rm RW}=-4\partial_u\partial_v-V$ with $V=f(6/r^2-3/r^3)$. A front that vanishes at $u=a$ gives $\Psi=\mathcal I_u P$ for $P=\Psi_u$, where $\mathcal I_u$ integrates from $a$. The cap $v=V_c$ lies wholly to the future of the graph, with $r_c=r((V_c-U)/2)$. In the cap rectangle set
$$
 y=(u-a)/D,\qquad z=1/r((v-U)/2),\qquad w=1-r_c z,\qquad z_c=1/r_c .
$$
Here $w$ increases to $1$ at null infinity and $dw/dv=f(z)z^2/(2z_c)$. In $L^2(du)$,
$$
 \partial_w P=-k(w,y)\mathcal I_yP+q_S,\qquad
 k=\frac{D z_c}{2z^2 f(z)}V\big(x=(v-u)/2\big),\qquad
 q_S=-\frac{z_c S}{2z^2 f(z)} .
$$
The norm of $\mathcal I_y$ on $L^2(du)=L^2(D\,dy)$ is $2/\pi$. By monotonicity of $V(r)$ for $r\ge25$,
$$
 \mu=\int_0^1\sup_y |k(w,y)|\,dw
 =\frac{D}{2}\left(\frac6{r_c}-\frac{3}{2r_c^2}\right),\qquad
 \|q_S\|_{L^1_w L^2_u}=\tfrac14\|S\|_{L^1_vL^2_u}.
$$
If the exact early front is used but a candidate front is nonzero, its effect in the residual must be carried as an extra forcing. The manufactured examples below use exactly zero front.

For $\mathcal TP=-\int_0^w k(w',y)\mathcal I_yP(w')dw'$ in $C^0_wL^2_u$, the pointwise absolute integrands in $\mathcal T^j$ are majorized by the product of the $j$ coefficient suprema times $\mathcal I_y^j |P(w_j)|$. First integrate in $y$ using the positive primitive kernel, then take the $L^2$ norm and integrate the ordered simplex in $w$. This gives
$$
 \|\mathcal T^j\|\le \frac{\mu^j}{j!}\|\mathcal I_y^j\|,\qquad
 \|\mathcal I_y^j\|\le \frac{1}{(j-1)!\sqrt{(2j-1)2j}} .
$$
The last bound is the Hilbert–Schmidt norm from integrating the squared primitive kernel $(y-y')^{j-1}/(j-1)!$ over $0\le y'\le y\le1$. It is useful for $j\ge2$. With $n=m+1\ge2$ and $\mu<n(n+1)$, a usable tail bound is
$$
 R_m(\mu)=
 \frac{\mu^n}{n!(n-1)!\sqrt{(2n-1)2n}}
 \frac{1}{1-\mu/[n(n+1)]}.
$$
The ratio of successive Hilbert–Schmidt majorants is less than $\mu/[j(j+1)]$ for $j\ge n$. Thus propagation on any subinterval has norm at most $F(\mu)=1+2\mu/\pi+R_1(\mu)$ whenever $\mu<6$. With integrable source the ordered series also gives convergence of $P$ in $L^2(du)$ as $w\uparrow1$, and convergence of the shear via $\mathcal I_u$. For $r_c=100$, directed arithmetic gives $F(\mu)<1.184411$ and $R_3(\mu)<5.157\times10^{-6}$ for the actual physical potential. The latter accounts for the degree-three series truncation before any coefficient or cap-data approximation.

## A polynomial approximation with whole-cap error

The potential at a given $y$ equals $V(x_0+b)$ where $x_0=(v-U)/2$ and $b=D(1-y)/2$. Expand in $b$ to order $h$. Define
$$
 \partial_x^j V=z^2(1-z)p_j(z),\quad
 p_0=6-3z,\quad
 p_{j+1}=-(2z-3z^2)p_j-z^2(1-z)p_j' .
$$
This follows from $\partial_x=-z^2(1-z)\partial_z$. The compactified coefficient $k_h=D z_c\sum_{j=0}^h b^j p_j(z)/(2j!)$ is a polynomial in $(w,y)$. For the exact Taylor remainder, the intermediate radius in $x_0+[0,b]$ has $0\le z'\le z$. With $p_n(z)=\sum_j p_{nj}z^j$ and $n=h+1$,
$$
 \int_0^1 \sup_y |k-k_h|\,dw \le
 \Delta_h=\frac{D z_c}{2(1-z_c)}\frac{(D/2)^n}{n!}
 \sum_j\frac{|p_{nj}|z_c^j}{j+1}.
$$
The proof bounds $(z')^2(1-z')$ by $z^2$, bounds $1/f(z)$ by $1/(1-z_c)$, and integrates each nonnegative power of $z=z_c(1-w)$. The coefficient enclosure holds across the whole cap.

For supplied cap data $p$ and source polynomial $\widehat q_S$, form the exact polynomial
$$
 P_{h,m}=\sum_{j=0}^{m}\mathcal T_h^j
 \left(p+\int_0^w\widehat q_S(w')\,dw'\right),
$$
using exact monomial primitives with Arb coefficients. A continuous reconstruction may also use polynomial pieces in $y$ if each $\mathcal I_y$ accumulates the prior cells' definite integrals as integration constants. The script implements a single global polynomial. Let $E_c=\|P(V_c)-p\|_2$, $E_q=\|q_S-\widehat q_S\|_{L^1_wL^2_u}$, $H=\|p\|_2+\|\widehat q_S\|_{L^1_wL^2_u}$, $\mu_h=\mu+\Delta_h$. Then
$$
 \|N-P_{h,m}(1)\|_2\le
 F(\mu)(E_c+E_q)
 +\left[\frac{2}{\pi}\Delta_h F(\mu)F(\mu_h)+R_m(\mu_h)\right]H .
$$
Subtract the two full cap equations and use the exact evolution bound on their difference. The mismatch in the potential contributes $(2/\pi)\Delta_h\|P_h\|_{C^0_wL^2_u}$ and $\|P_h\|\le F(\mu_h)H$. The final ordered-series tail is relative to $H$. If storing midpoint coefficients for $P_{h,m}(1)$, add $\sqrt D$ times the sum of their ball radii to the error. This is an $L^2(du)$ bound in the exact normalized coordinate $y$. It uses the actual radial potential across the entire $v\ge V_c$ exterior. Setting $\widehat q_S=0$ retains $E_q=B_c$ from the Work review.

| $r_c/r_0$ | Taylor $h$ | Picard $m$ | Upper $\Delta_h$ | Upper homogeneous factor multiplying $H$ |
| ---: | ---: | ---: | ---: | ---: |
| 25 | 4 | 5 | $3.335\times10^{-4}$ | $7.625\times10^{-4}$ |
| 100 | 0 | 3 | $1.27145\times10^{-2}$ | $1.1450\times10^{-2}$ |
| 100 | 2 | 3 | $2.730\times10^{-5}$ | $2.954\times10^{-5}$ |
| 100 | 4 | 3 | $5.918\times10^{-8}$ | $5.210\times10^{-6}$ |
| 100 | 4 | 5 | $5.918\times10^{-8}$ | $5.326\times10^{-8}$ |
| 100 | 6 | 5 | $1.292\times10^{-10}$ | $5.268\times10^{-10}$ |
| 200 | 4 | 5 | $8.751\times10^{-10}$ | $6.677\times10^{-10}$ |

These are analytically justified bounds evaluated with 96- and 160-bit Arb. The table isolates a frozen-potential limitation and the separate degree-three Picard truncation. Refining Picard order alone leaves the Taylor defect. The order-six cap coefficient and five Picard corrections give a homogeneous factor below $5.268\times10^{-10}$ at $r_c=100$. The magnitude of the unevaluated physical inputs $E_c$ and $E_q$ remains open.

## Exterior source with known retarded trace

At $r_c=100$, prescribe $P=yw^2$, $\Psi=D y^2 w^2/2$ for $w\ge0$, and zero inside the cap. Across the cap $\Psi$ and its first derivatives are continuous and the cap news vanishes. The exact compactified forcing is $q_S=2yw+k y^2w^2/2$ with the true Schwarzschild $k$. The lower front and all earlier data are zero, so the equation and uniqueness identify the retarded solution on these cuts. Its Bondi news is $y$. For the calculation of the manufactured source use Taylor order five and use order four for propagation. The difference of the two polynomial coefficients acting on the exact solution contributes at most $(\Delta_4+\Delta_5)/(2\sqrt5)$ in $L^1_wL^2(dy)$, and the approximate source has norm at most $1/\sqrt3+(\mu+\Delta_5)/(2\sqrt5)$ there. The ordered bound therefore gives an evaluated $L^2(du)$ error $<5.026\times10^{-8}$ for five Picard corrections. The directed polynomial norm of the discrepancy is $<2.064\times10^{-11}$, and the coefficient arithmetic error is $<8.054\times10^{-29}$ at 96 bits. Omitting the forcing gives an exact error $\sqrt{D/3}>1.7409$ despite the zero cap input. As a separate normalization check, when using only the endpoint-ray coefficient $k_0$ the exact power series acting on $p=y$ has coefficients $(-\mu)^j/[j!(j+1)!]$ multiplying $y^{j+1}$. The six polynomial coefficients from the transfer with $0\le j\le5$ overlap those obtained from this separate analytic formula.

## Weak cap formula for interference projections

For a signed projection, the weak cap formula can remove derivatives of the exterior returning densities. In the pinned exact-curl convention let $p_D=r^{-2}\mathcal D^x$, $q_D=r^{-2}\mathcal D^t$ and
$$
 A=\frac{r}{2\lambda}(p_D-q_D),\qquad
 B=\frac{r}{2\lambda}(p_D+q_D),\qquad
 C_0=-\frac{f q_D}{2\lambda}+\frac{f\mathcal C}{2r^3}.
$$
Indeed $-S/4=\partial_u A+\partial_v B+C_0$ by $r_u=-f/2$, $r_v=f/2$. Thus $\widetilde P=P-B$ obeys
$$
 \partial_v\widetilde P=-V\mathcal I_u\widetilde P/4
 +\partial_u A-V\mathcal I_u B/4+C_0,\qquad
 \widetilde P(V_c)=P(V_c)-B(V_c).
$$
If $B\to0$ in $L^2(du)$, the news limit is also the limit of $\widetilde P$. For a real $H^1(du)$ test $g$, let $h(v)=G(\infty,v)^*g$ where $G$ is the cap homogeneous propagator. Then
$$
 \partial_v h=\mathcal I_u^*(Vh/4),\qquad h(\infty)=g,\qquad h(v,U)=g(U).
$$
Consequently the exact weak projection is
$$
 \langle g,N\rangle=\langle h(V_c),P(V_c)-B(V_c)\rangle+
 \int_{V_c}^\infty\!\left[
     g(U)A(U,v)-h(a,v)A(a,v)
     -\langle \partial_u h,A\rangle
     +\langle h,C_0-V\mathcal I_uB/4\rangle
 \right]dv .
$$
The left-front term vanishes for the pinned flat front. The right-edge term generally survives, especially on the early packet cut. For integrable exterior norms the formula follows by integrating the strong equation and then the pairing by parts. It extends to distributional $u$ derivatives with consistently retained jumps and traces. Bounds on the dual weights are $\|h(v)\|_2\le F(\mu)\|g\|_2$ and $\|\partial_u h(v)\|_2\le\|g'\|_2+(\mu/D)F(\mu)\|g\|_2$, since $\partial_u\mathcal I_u^*$ is minus the identity. The derivative bound needs only $V$.

A manufactured flat-cap fixture makes both edge terms in the formulation observable. Take $D=3/2$, $g=y$, $\rho=dv/dw$, $\rho A=2wy^2$, $B=y(1-w)$, $P(V_c)=B(V_c)=y$, and $C_0=0$. The strong solution has $N=2y/D$. Its exact overlap is $2/3$, made of the latest-cut term $1$ and the bulk term $-1/3$. Dropping the latest-cut term reverses the sign, while dropping the shift in the cap data instead adds $1/2$. These are exact rational manufactured controls for the weak representation. Evaluating the physical projection still requires cap data and bounds on $A$, $B$, and $C_0$.

## One weak projection over the entire causal domain

For the ordered return set $\mathcal D=\{a\le u\le U,\ v\ge v_\Sigma(u)\}$, with $v_\Sigma(u)=r_\Sigma(u)$ and decreasing $v_\Sigma$. The same adjoint $Z$ can be solved on this whole domain:
$$
 Z_v(u,v)=\int_u^U (VZ/4)(u',v)\,du',\qquad
 Z(u,\infty)=g(u),\qquad Z(U,v)=g(U).
$$
For a point of $\mathcal D$, the entire interval in its right-hand side lies in $\mathcal D$. Equivalently extend $V$ by zero below the graph when estimating the backward evolution on $[a,U]$. In $\mathcal D$ the adjoint satisfies $Z_{uv}=-VZ/4$. For $W=\Psi_{21}$ and $P=W_u$, the Green identity is
$$
 \partial_v(ZP)-\partial_u(W Z_v)=-Z S/4 .
$$
The upper-cut vertical boundary in this identity vanishes by $Z_v(U,v)=0$, and the left one by $W(a,v)=0$. The ordered canonical data give $W|_\Sigma=0$. Integrating the identity therefore gives $\langle g,N\rangle=\int Z|_\Sigma P|_\Sigma\,du-\tfrac14\iint_\mathcal D ZS\,du\,dv$.

Insert $-S/4=\partial_u A+\partial_v B+C_0$. Integration by parts of the source generates the graph term $Z(v_\Sigma' A-B)|_\Sigma$. On the graph, with $p=T'(x)$ and $v_\Sigma'=(1+p)/(p-1)$, the pinned graph jet of $Q_{2z}$ gives
$$
 P|_\Sigma=\frac{r}{\lambda}\frac{p_D-pq_D}{1-p}
 =(B-v_\Sigma' A)|_\Sigma .
$$
Thus the graph terms cancel in the weak representation. Assuming the indicated trace and integrable bulk densities and $B(\cdot,v)\to0$ in $L^2$, one obtains
$$
 \langle g,N\rangle=
 g(U)\int_{v_\Sigma(U)}^\infty A(U,v)\,dv
 +\iint_\mathcal D \big(-Z_u A-Z_v B+Z C_0\big)\,du\,dv.
$$
Here the left-cut $A$ term vanishes by the flat front. If $g$ is real $H^1$ the bulk weights can be bounded using $\mu_\Sigma=(D/4)\int_{v_\Sigma(U)}^\infty\sup_{u:(u,v)\in\mathcal D}V\,dv$. The reversed ordered estimate gives $\sup_v\|Z\|_2\le F(\mu_\Sigma)\|g\|_2$ and $\sup_v\|Z_u\|_2\le\|g'\|_2+(\mu_\Sigma/D)F(\mu_\Sigma)\|g\|_2$ for $\mu_\Sigma<6$. Also $\|Z_v(v)\|_2\le (2D/\pi) F(\mu_\Sigma)\|g\|_2\sup_u V(u,v)/4$. All norms are in $du$, restricted to the available $u$ range if desired.

On the early and full packet strips the latest graph radius $r_\Sigma(U)$ is below the potential peak $r_p=(27+\sqrt{153})/24$. Bound the $v$ integral by $2[x(r_p)-x(r_\Sigma(U))]V(r_p)$ up to the peak, then use the exact radial integral from $r_p$ to infinity. The upper multipliers for $\sup\|Z\|_2/\|g\|_2$ are $1.015024$ on the early strip and $1.126439$ on the full strip. The additional coefficients multiplying $\|g\|_2$ in the $Z_u$ estimate are respectively $1.588451$ and $1.784880$. The calculation uses the exact graph inverse $r_\Sigma(U)=1+2W_0(e^{-(1+U)/2}/2)$ and directed arithmetic. The identity replaces derivatives of the returning densities by derivatives of a known scalar test solution and includes the entire causal domain. It still requires the canonical lower fields, the latest-cut density trace, and their interval errors.

In the exact flat graph control take $u\in[0,1]$, $v_\Sigma=1-u$, $g=u$, $A=C_0=0$, and $B=u(2-v)_+$. The strong solution has $P=B$, $P_\Sigma=B_\Sigma\ne0$, $W_\Sigma=0$, and $N=0$. Its graph projection is $7/12$ and its strong volume projection is $-7/12$. Dropping the graph term gives a false nonzero overlap; the correctly canceled weak form gives zero.

A separate distributional fixture on the same graph takes $B=u(2-v)$ for $1<v<2$ and $B=0$ otherwise. Then $P=B$ and $N=0$. The jump of $B$ at $v=1$ contributes $+1/3$ to the strong pairing and cancels the $-1/3$ cellwise volume term. Differentiating each cell and omitting the interface produces a false overlap. Evaluating the weak formula on the piecewise density includes the jump through distributional integration by parts.

## Reproduction and disposition

From `research/sav` in the source-pinned GOAL-013 checkout, run `python3 GLR-GOAL-013/checks/c_cap_transfer.py`. The archived [output](../checks/c_cap_transfer.txt) uses Python 3.14.8 and python-flint 0.9.0 at 96 and 160 bits. See the [failed omissions and low-order control](../checks/c_failures.txt). The numerical bounds apply on the prescribed full interval and to the manufactured scalar fixtures. Completing the physical absolute error bound requires a verified cap input from the coupled hierarchy and an evaluated exterior forcing bound or reconstruction. The weak alternative still needs an evaluated signed density projection and the associated cap input.
