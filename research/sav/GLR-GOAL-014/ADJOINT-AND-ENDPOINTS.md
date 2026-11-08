# Sharp-window adjoint for the ordered return

This analysis uses the dimensionless tortoise, time and null coordinates of the [pinned GOAL-013 source](https://github.com/inaeyk/GL-with-AI/tree/1e8e628f68af4cab174fcc44b81af4748a22115c/research/sav/GLR-GOAL-013). In particular, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $a=u_*$, and $b(u)=v_\Sigma(u)=r_\Sigma(u)$ decreases on $I=[a,d]$. The scalar potential is $V=f(6/r^2-3/r^3)$ and $W=V/4$. The exact-curl returning shear is $w=2Q_{2z}/(\lambda r)$ with $\lambda=4$, and $p_c=w_u(\cdot,c)$ denotes its cap trace. We use $p=T'(x)=-1/r$ only for the graph slope. The physical test is $g=N_{01}$ on the exact sharp window. The graph and front data are those of the fixed canonical pair in [GOAL-011](../GLR-GOAL-011/FINITE-DATA.md), with sources in [its hierarchy](../GLR-GOAL-011/HIERARCHY.md) and the tortoise conventions of [GOAL-013](../GLR-GOAL-013/FINITE-CUT-BOUNDS.md). The five-dimensional hierarchy and spectral bracket enter as pinned inputs. The duality below is derived for that hierarchy.

## Graph and distributional contacts

In the $(t,x)$ chart the returning equations give
\[
 \lambda r^2 A_{2t}=Q_{2z,x}+\mathcal D^t,\qquad
 \lambda r^2 A_{2x}=Q_{2z,t}-\mathcal D^x.
\]
The tortoise densities are $\mathcal D^t_x=f\mathcal D^t_r$ and $\mathcal D^x_x=\mathcal D^r_r$, where the subscript labels the spatial chart. The canonical zero upper data impose $A_{2x}+pA_{2t}=0$ and $Q_{2z}|_\Sigma=0$. The latter gives $Q_{2z,x}=-pQ_{2z,t}$ on the graph. Substitution into the two equations fixes
\[
 (1-p^2)Q_{2z,t}=\mathcal D^x-p\mathcal D^t .
\]
With $m=2r/\lambda$, $P=\mathcal D^x/r^2$, $Q=\mathcal D^t/r^2$, and $c_0=\lambda f r^{-4}\mathcal C$, the inherited scalar source is $m(-P_t-Q_x-c_0)$. Using $\partial_t=\partial_u+\partial_v$ and $\partial_x=-\partial_u+\partial_v$ gives $-F_u-G_v+H$ for $F=m(P-Q)$, $G=m(P+Q)$, $H=m_xQ-mc_0$. Since $w_x=-p w_t$ on $\Sigma$ and $b'=-(1+p)/(1-p)$, one obtains the identity
\[
 4w_u|_\Sigma=\frac{2m}{1-p}(P-pQ)=G-b'F. \tag{A1}
\]
In particular the graph velocity vanishes only where the corresponding current contraction vanishes.

Write $\Theta=\mathbf 1_{v\ge b(u)}$. The distributional term of $-4(\Theta w)_{uv}$ is $-4w_u|_\Sigma\delta(v-b)$, since $w|_\Sigma=0$. The term of $-(\Theta F)_u-(\Theta G)_v$ is $(b'F-G)|_\Sigma\delta(v-b)$. Equation (A1) matches them. The flat front supplies no contact at $a$. Integrating in $v$ yields
\[
 4w_u=G+\partial_u\mathcal F-\int_b^v(H+Vw)\,ds,
 \qquad \mathcal F(u,v)=\int_{b(u)}^v F(u,s)\,ds. \tag{A2}
\]
In the ordinary derivative of $\mathcal F$ the moving graph term is $-b'F|_\Sigma$. Equation (A1) cancels the entire graph contribution to (A2). With the stated ray convergence and endpoint traces, integration by parts after taking the $v\to\infty$ trace gives
\[
 4\langle g,N_{21}\rangle=[g\mathcal F_\infty]_a^d-\langle g',\mathcal F_\infty\rangle+
 \left\langle g,G_\infty-\int_b^\infty(H+Vw)\,dv\right\rangle . \tag{A3}
\]
The left endpoint is zero by flatness. The right endpoint stays on both physical windows. For example, even at the end of the compact packet, the leading news has scattered, as the [directed test certificate](checks/d_baseline_h1.txt) shows.

For a seam $\phi=0$ let $[\cdot]$ mean plus-side minus minus-side trace. Distributional differentiation of the reconstructed currents produces $(\phi_u[F]+\phi_v[G])\delta(\phi)$ in $4w_{uv}$. For a continuous piecewise smooth $w$, its matching contact is $4\phi_v[w_u]\delta(\phi)=4\phi_u[w_v]\delta(\phi)$, where equality follows from the common tangential derivative. When using undifferentiated current integrals below, their volume formula already integrates these contacts by parts. A cell-interior strong derivative calculation needs them explicitly. If lower master reconstructions themselves jump, their first distributional derivatives can also give measures *inside* the currents. Apply the estimates below to conforming lower reconstructions, or include those additional measures by a lift or by explicitly evaluating their weights. Simply sampling more interior points does not control a jump.

## Finite-cap adjoint and equivalent primal route

Let $c>b(a)$ be a constant advanced cut and $D_c=\{a\le u\le d,\ b(u)\le v\le c\}$. For a test $h\in H^1(I)$ define the backward Volterra function
\[
 Z(u,v)=h(u)-\int_u^d\int_v^c W(z,s)Z(z,s)\,ds\,dz. \tag{A4}
\]
The future rectangle stays in $D_c$ because $b$ decreases. Thus $4Z_{uv}+VZ=0$, $Z(u,c)=h(u)$, and $Z(d,v)=h(d)$. Its derivatives in the volume sense are
\[
 Z_u=h'+\int_v^c W(u,s)Z(u,s)\,ds,\qquad
 Z_v=\int_u^d W(z,v)Z(z,v)\,dz.
\]
The sharp-window pairing is
\[
 4\langle h,p_c\rangle=[h\mathcal F(\cdot,c)]_a^d+\langle h,G(\cdot,c)\rangle
     -\int_{D_c}(Z_uF+Z_vG+ZH)\,du\,dv. \tag{A5}
\]

One derivation uses the local Green identity
\[
 \partial_v(Zw_u)-\partial_u(wZ_v)=Zw_{uv}-wZ_{uv}.
\]
On $D_c$ the front and graph field traces vanish and $Z_v(d,v)=0$. Multiplying by four gives $4\langle h,p_c\rangle-\int Z|_\Sigma 4w_u|_\Sigma\,du$. Integration by parts of the right-hand side $Z(F_u+G_v-H)$ yields the endpoint and cap terms of (A5), the volume terms of (A5), and the graph contribution $\int Z|_\Sigma(b'F-G)\,du$. These graph terms match by (A1). The proof extends to piecewise currents with the distributional convention above.

A second derivation starts with (A2) and solves $w=J_s-\mathcal T w$ by the forward Volterra integral, where $J_s$ and $\mathcal T$ are [GOAL-013 Eq. (2)](../GLR-GOAL-013/FINITE-CUT-BOUNDS.md) with $v\le c$. The transpose of $\mathcal T$ on $D_c$ is $\mathcal T^*\chi(u,v)=W(u,v)\int_u^d\int_v^c\chi$. Let $\chi=WZ$. Then $(1+\mathcal T^*)\chi=Wh$, and $\int Whw=\int\chi J_s$. Swapping the three integrations in $J_s$ replaces the test weights $h',0,h$ for $F,G,H$ by $h'+\int_v^c\chi=Z_u$, $\int_u^d\chi=Z_v$, and $h-\int_u^d\int_v^c\chi=Z$ respectively. This gives (A5) from the finite-cap version of (A3). The primal calculation encloses a shear defect and tests the pairing directly. The dual calculation evaluates (A5) from the currents and requires only an adjoint defect.

## Exterior propagation of the test and cancellation at the cap

Above $c$, the flat-front shear is $w=\mathcal I w_u$, where $\mathcal I$ integrates in $u$ from $a$. The exterior equation is $P_v=-W\mathcal I P-S_2/4$, with $P=w_u$ and $S_2=-F_u-G_v+H$. If $U(v_2,v_1)$ propagates the homogeneous equation, define $h_v^{\rm test}=U(\infty,v)^*g$ and write it below as $\widehat h(u,v)$ to distinguish it from a derivative. Then
\[
 \begin{split}
 \widehat h(u,v)&=g(u)-\int_u^d\int_v^\infty W(z,s)\widehat h(z,s)\,ds\,dz,\\
 \partial_u\widehat h(u,v)&=g'(u)+\int_v^\infty W(u,s)\widehat h(u,s)\,ds,\\
 \partial_v\widehat h(u,v)&=\int_u^d W(z,v)\widehat h(z,v)\,dz.
 \end{split} \tag{A6}
\]
The terminal test trace is $\widehat h(d,v)=g(d)$ exactly. $\widehat h(a,v)$ can differ from $g(a)$, but $F(a,v)=0$ for the physical preparation. Let $h=\widehat h(\cdot,c)$ in (A5). Duhamel gives the full projection as (A5) divided by four plus $-\int_c^\infty\langle\widehat h,S_2\rangle\,dv/4$. Two ways of bounding this last piece are available.

1. **Strong exterior source route.** Use the audited bound $B_c=\|S_2\|_{L^1_vL^2_u}/4$ and the signed amplification $A_2=\exp(3D^2/(\pi^2 r_c^2))$, where $c=d+2x(r_c)$, $D=d-a$, and $r_c\ge100$. The scalar error is at most $A_2\|g\|_2 B_c$. This route retains $\langle h,G(\cdot,c)\rangle$ in (A5). It truncates the *physical source* in Duhamel. Extending the currents themselves by zero after $c$ would introduce a cap delta from $G_v$ and give a different propagation.
2. **Separate weak exterior current route.** Integrate the exterior Duhamel term by parts using (A6). Four times that term equals
\[
 \left[\int_c^\infty\widehat h F\,dv\right]_a^d
 -\int_{v\ge c}(\partial_u\widehat h F+\partial_v\widehat h G+\widehat h H)\,du\,dv
 +\langle g,G_\infty\rangle-\langle h,G(\cdot,c)\rangle. \tag{A7}
\]
The last term exactly cancels the compact cap term. With the separate physical $G_\infty=0$ estimate in [Currents and Data](CURRENTS-AND-DATA.md), one may evaluate (A5) with its cap term removed and use (A7) without its last two terms as the weak exterior error. This version does not need an independent cap $G$ trace. Its terminal $F$ ray remains. The strong source norm alone does not bound the separate currents in (A7).

To bound test transport, put $m_c=\int_c^\infty\sup_u W(u,v)\,dv=3/r_c-3/(4r_c^2)$ and $J_c(u)=\int_c^\infty W(u,v)\,dv=3/r(u,c)-3/(4r(u,c)^2)$. The equality for $m_c$ uses radial monotonicity beyond $100$. In $L^p$, $p=1,2$, use $\kappa_1=D$, $\kappa_2=2D/\pi$ for the backward primitive, and $A_1=e^{Dm_c}$, $A_2$ as above for cap amplification. Set $h_1=g-\mathcal I^*(J_c g)$. Then
\[
 \|h-h_1\|_p\le\tfrac12 \kappa_p^2 m_c^2 A_p\|g\|_p,\qquad
 \|h'-h_1'\|_p\le\tfrac12 \kappa_p m_c^2 A_p\|g\|_p. \tag{A8}
\]
Two substitutions of (A6) leave the ordered $v$-simplex of two $W$ factors, bounded by $m_c^2/2$. For the derivative, the outer primitive is removed before taking the norm. For $h_{1,h}$ formed with a baseline candidate $g_h$, test errors $e_p=\|g-g_h\|_p$ and $e'_p=\|g'-g_h'\|_p$ give
\[
 \|h-h_{1,h}\|_p\le A_p e_p+\tfrac12\kappa_p^2m_c^2 A_p\|g_h\|_p,
 \qquad
 \|h'-h_{1,h}'\|_p\le e'_p+m_c A_p e_p+\tfrac12\kappa_p m_c^2 A_p\|g_h\|_p.
 \tag{A9}
\]
The endpoint test error is exactly $|g(d)-g_h(d)|$ since the correction primitive vanishes there. The estimates preserve sharp windows throughout.

## Dual error budget

Define compact current ray errors
\[
 A_F(u)=\int_{b(u)}^c |F-F_h|\,dv,\quad
 A_H(u)=\int_{b(u)}^c |H-H_h|\,dv,\quad
 A_G(u)=\sup_{b(u)\le v\le c}|G-G_h|.
\]
Here $F_h,G_h,H_h$ denote approximate currents, independently of the cap test $h$. Weights can be integrated directly against cellwise certified errors in place of sup-ray envelopes. For simple enclosures put $M=\int_{b(d)}^\infty W(d,v)\,dv=3/b(d)-3/(4b(d)^2)$ and
\[
 S_h(u)=|h(u)|+M\int_u^d e^{M(z-u)}|h(z)|\,dz .
\]
Backward Volterra majorization gives $\sup_v|Z(u,v)|\le S_h(u)$, $\sup_v|Z_u|\le|h'|+MS_h$, and $\int_{b(u)}^c|Z_v|\,dv\le S_h(u)-|h(u)|$. For the last bound interchange the $v,z$ integrals and use $b(z)\le b(u)$ for $z\ge u$. Consequently four times the projection error from replacing the currents at fixed test is at most
\[
 \sum_{e=a,d}|h(e)|A_F(e)+\int_a^d\left[(|h'|+MS_h)A_F+S_h A_H+W_G A_G\right]du, \tag{A10}
\]
with $W_G=S_h$ in the strong exterior route and $W_G=S_h-|h|$ in the weak route. The $a$ term is absent with the true and reconstructed flat fronts. The cap term, when present, can be evaluated with a separate certified $G$ trace instead of using $A_G$. In $L^1$ the $S_h$ norm is bounded by $e^{MD}\|h\|_1$, and the norm of $S_h-|h|$ by $(e^{MD}-1)\|h\|_1$. For the test error, apply (A10) with $h-h_{1,h}$ in place of $h$ and the *absolute candidate* ray norms in place of the current errors. This follows from bilinear telescoping and (A9), and introduces no derivative of the physical returning news. In the exterior weak error (A7), the analogous majorants use $m_c$ in place of $M$ and $g$ in place of $h$.

An approximate dual $Z_h$ can be certified without differentiating the numerical reconstruction. Let $B Z=\int_u^d\int_v^c W Z$, and $r_Z=Z_h-h_{1,h}+BZ_h$. If $A_r(u)\ge\sup_v |r_Z(u,v)|$, then
\[
 \sup_v |Z[h_{1,h}]-Z_h|\le S_{A_r}(u)
 =A_r(u)+M\int_u^d e^{M(z-u)}A_r(z)\,dz .
\]
Construct the numerical weights via $h_{1,h}'+\int_v^c W Z_h$ and $\int_u^d W Z_h$. The resulting volume error, multiplied by four, is bounded by integrating $M S_{A_r}$ against the absolute candidate $F$ ray norm, $S_{A_r}$ against the $H$ ray norm, and $M\int_u^d S_{A_r}$ against the $G$ ray norm. For an exact degree-$n$ Neumann polynomial, the simpler integrated dual remainder is
\[
 \left\|\sup_v|Z[h_{1,h}]-Z_n|\right\|_1
 \le \|h_{1,h}\|_1\sum_{j=n+1}^\infty (MD)^j/j!.
\]
The two boundary terms are evaluated using $h_{1,h}$, with no $Z_h$ trace needed. Candidate quadrature, arithmetic and whole-cell reconstruction errors remain separate.

## Evaluated weights and falsification controls

For $r_c=400$, $b(d)>1.59$ on both windows. Indeed $u_\Sigma(1.59)-u_\Sigma(13/8)=0.035+2\log(125/118)\ge0.147$. Thus $M$ is bounded by $3/1.59-3/(4\cdot1.59^2)<1.6$. The [rational check](checks/a_adjoint.py) uses exact input rationals rounded outward from [the directed baseline](checks/d_baseline_h1.txt) and [the inherited $L^2$ news norm](../GLR-GOAL-013/checks/d_leading_news.txt). It bounds exponentials by a positive rational Taylor remainder and gives the following rounded-up multipliers for **uniform** compact ray errors in (A10), inclusive of the right endpoint for $F$.

| Window | $F$ | $H$ | $G$, strong exterior | $G$, weak exterior |
|---|---:|---:|---:|---:|
| Early | $0.206883890$ | $0.000202708$ | $0.000202708$ | $0.000004778$ |
| Packet | $4.927654673$ | $0.082180215$ | $0.082180215$ | $0.014275917$ |

The early endpoint alone contributes a weight $|g(d)|/4>0.1031$ to an independent terminal $F$ ray error. The uniform bound combines it with a distinct bulk error. It is often cheaper to use actual terminal and bulk weights. The [evaluated test error multipliers](checks/a_adjoint.txt) apply to the absolute candidate current norms. The strong-tail proof at the longer GOAL-013 cut applies on these earlier cap strips with the same constants. Its width and time bounds are monotone, and $r_c+0.49(v-c)\le r\le r_c+5+(v-c)/2$ holds on each shorter strip. With $B_c<1.7\times10^{-6}$ the strong scalar exterior errors are below $2.32221\times10^{-8}$ and $1.7017001\times10^{-6}$ on the early and packet windows. With the independently derived physical weak-tail bounds $A_F^{\rm tail},A_H^{\rm tail}<3.36\times10^{-15}$ and $A_G^{\rm tail}<2.74\times10^{-15}$ from [Currents and Data](CURRENTS-AND-DATA.md), the weak scalar exterior errors are below $6.948\times10^{-16}$ and $1.63462\times10^{-14}$. These are bounds for the ordered physical source under the inherited hierarchy and its GL input. They do not evaluate compact current errors.

In the exact polynomial control, the adjoint and direct news integrate to $5/3$ with nonzero induced graph velocity, terminal and cap terms and a positive potential. Dropping the terminal term gives $2/3$. A second control with a non-null seam has signed projection $-1/20$. Differentiating only the cell-interior currents and retaining no seam delta instead gives $143/640$. Both checks use exact symbolic rational arithmetic. [Check](checks/a_adjoint.py) · [Output](checks/a_adjoint.txt).

Small bulk norms cannot replace the terminal error. Use the physical scalar $V$ and a smooth exterior cut-on $\rho(v)$ with $\rho=0$ below a cap above the graph and $\rho=1$ above a larger cap. Take smooth $\psi$ supported in $[-1,1]$ with $\psi(0)=1$, and $w_n=\psi(n(d-u))\rho(v)/4$ with $F_n=4(w_n)_v+\int_a^u Vw_n\,dz$ and $G_n=H_n=0$. The primitive integrand uses $V(z,v)w_n(z,v)$. The field obeys the scalar current equation with zero incoming jets. Both the shear $X$ norm and the bulk $L^2$ norm of the $F_n$ ray size vanish as $O(n^{-1/2})$, while $\mathcal F_{n,\infty}(d)=1+O(n^{-1})$. The sharp-window pairing against a fixed smooth $g$ tends to $g(d)/4$. The terminal concentration is manufactured and illustrates the topology needed for an error certificate.

## Compact traces from uncommuted lower residuals

For an error field $e$ in the stationary mixed system or scalar special case, write the inherited equation as
\[
 -4e_{uv}-\mathsf W_e e+\mathsf C(e_u+e_v)=R,
 \qquad \mathsf W_e=K+\mathsf A^2-\mathsf A_x.
\]
Here $\mathsf C$ is skew-Hermitian and $K$ is positive on the finite diamond. On a fixed outgoing $u$ ray the transverse derivative $e_u$ obeys
\[
 (e_u)_v-\mathsf C e_u/4=(\mathsf C e_v-\mathsf W_e e-R)/4.
\]
The homogeneous matrix propagator is unitary. Use the incoming $e_u$ from the graph and the ray energy flux $\int(|e_v+\mathsf A e/2|^2+e^\dagger K e/4)dv\le M_e^2$. For an endpoint ray this yields
\[
 \sup_v |e_u(d,v)|\le |e_u(d,b(d))|+
 \frac{\|R(d,\cdot)\|_{L^1_v}}4+
 \frac{M_e}4\left(\|\mathsf C\|_{L^2_v}
    +2\|(\mathsf W_e+\mathsf C\mathsf A/2)K^{-1/2}\|_{L^2_v}\right). \tag{A11}
\]
The analogous fixed-$v$ estimate for $e_v$ uses the front (or graph) value of $e_v$, the $u$-ray residual, the flux with $e_u-\mathsf A e/2$, and $\mathsf W_e-\mathsf C\mathsf A/2$. In the scalar case $\mathsf A=\mathsf C=0$, $K=V$, and the norm in the last term of (A11) is bounded by $2\sqrt{M}$ before multiplying by its factor of two. These statements require actual residual traces and incoming graph jets on the rays, with matching derivatives or explicitly incorporated jumps. The bulk mixed-norm residual alone cannot bound its endpoint ray. For current components linear in lower field values and first jets, (A11) can avoid an extra global time commutation imposed solely for a transverse trace. This is a conditional residual-to-trace bound. Neither a physical residual trace nor a whole-cell coefficient reconstruction error is supplied by the adjoint derivation.

### Direct bound for the signed terminal-ray integral

There is a sharper bound if the target is the signed ray integral
$\Delta\mathcal F_c(d)=\int_{b(d)}^c(F-F_h)(d,v)\,dv$. Split the current difference into lower-field errors and coefficient errors acting on known fields. For one lower error write
\[
 F-F_h=B_u e_u+B_v e_v+B_0 e+\delta F_{\rm coeff},
\]
where the $B_j$ are rows in the scalar or normalized mixed variables and the decomposition is exact for that stage's linear reconstruction. Solve the terminal-ray row equation
\[
 \lambda_F'+\lambda_F\mathsf C/4=-B_u,\qquad \lambda_F(c)=0
\]
with prime here differentiating in $v$ at $u=d$. Applying the product rule to $\lambda_F e_u$ eliminates the transverse derivative from the integrated error:
\[
 \Delta\mathcal F_c(d)=\lambda_F(b)e_u(d,b)+
 \int_b^c\left[
 (B_v+\lambda_F\mathsf C/4)e_v+
 (B_0-\lambda_F\mathsf W_e/4)e-\lambda_F R/4+\delta F_{\rm coeff}
 \right]dv. \tag{A12}
\]
For $J_F=B_v+\lambda_F\mathsf C/4$ and
$K_F=B_0-\lambda_F\mathsf W_e/4-J_F\mathsf A/2$, this yields
\[
 |\Delta\mathcal F_c(d)|\le |\lambda_F(b)e_u(d,b)|+
 M_e\left(\|J_F\|_2^2+4\|K_F K^{-1/2}\|_2^2\right)^{1/2}
 +\tfrac14\int_b^c\|\lambda_F\||R|\,dv+
 \int_b^c|\delta F_{\rm coeff}|\,dv.
\]
Sum these contributions for baseline and mixed errors. The graph term can use the actual graph jet mismatch rather than a pointwise bound on the whole outgoing ray. The residual term is weighted towards the portion of the ray where the current still couples appreciably. A numerical row $\lambda_{F,h}$ can be certified by the $L^1$ norm of its ODE residual and its terminal value error, since the homogeneous row propagator is unitary. Volume weights may also act directly on the physical lower errors. Contract the current rows with $Z_u,Z_v,Z$ to write each volume contribution as $\int_{D_c}(C_u e_u+C_v e_v+C_0 e)$. Set $D_0=C_0+C_u\mathsf A/2-C_v\mathsf A/2$. The contribution is bounded by $M_e$ times
\[
 \|C_u\|_{L^1_vL^2_u}+\|C_v\|_{L^1_uL^2_v}
 +2\min(\|D_0K^{-1/2}\|_{L^1_vL^2_u},\|D_0K^{-1/2}\|_{L^1_uL^2_v}).
\]
The directional norms refer to the zero extensions on the compact diamond. This dual error route uses finite-ray and mixed-norm residuals rather than a sup-in-$v$ error in each current. In the weak exterior version it requires no additional cap trace.

For the central floating-point early candidate with $\mathcal P_h\simeq7.8181024$ and $\mathcal F_{c,h}(d)\simeq193.36257$, the terminal term alone is about $19.94309$. Conditional on a lower quadrature bound $\mathcal P_h^-$ and all nonterminal errors bounded by $E_{\rm other}$, the precise additional requirement for a positive sign is
\[
 E_{\rm terminal}<\frac{4(\mathcal P_h^- -E_{\rm other})}{\sup |g(d)|},
 \qquad E_{\rm terminal}\ge |\Delta\mathcal F_c(d)|.
\]
Any baseline endpoint or quadrature error is part of $E_{\rm other}$. Inserting the empirical center and leaving other errors at zero would allow a terminal integral error of about $75.80$, or $39.2\%$ of the terminal-ray candidate. Reserving half the margin for other errors reduces this to $37.90$. These are numerical targets and are conditional on the projected center and all other errors.

A [terminal sensitivity calculation](checks/a_terminal_sensitivity.py) evaluates the row and flux weights on the same central coefficient candidate, independently of C's physical lower time evolution. At 1440 ray panels the mixed graph-row norm, flux multiplier and maximal weighted residual multiplier $\max|\lambda_F|/4$ are $4.31146768$, $9.77615838$ and $1.07786692$. Their baseline counterparts are $13.0133660$, $14.1387921$ and $3.25334151$. A 720-panel check moves each printed quantity by less than $2\times10^{-7}$. This computation uses ordinary quadrature and a central GL and mean geometry. It supplies the scale for a future enclosure. No directed coefficient or lower residual bound follows from it. [Primary](checks/a_terminal_sensitivity.txt) · [Refinement](checks/a_terminal_sensitivity_variant.txt) · [Failures](checks/a_terminal_failures.txt).

The remaining sign gate is a finite-cap evaluation of the current pairing and a validated error in its compact volume and terminal data, including lower-field, mean, graph and GL coefficient errors. The evaluated transfer and weak-tail weights make compact sources the relevant next target. They give no sign by themselves.
