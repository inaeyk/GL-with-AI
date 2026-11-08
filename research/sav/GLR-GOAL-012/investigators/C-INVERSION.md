# Physical-line inversion and finite-window pole subtraction

Use $r_0=1$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $f=1-1/r$, and $e^{-st}$ for the Laplace kernel. The outgoing trace of the exact mean curl $\Psi_2=2Q_{2z}/(\lambda r)$ is the ordered $\epsilon^2\eta$ shear coefficient in units of $r_0$, and $N$ is its $u$ derivative. The real axial quadrupole, fixed graph, bump, normalization, and retarded origin are those of GOAL-012. The input pin is eb439228601a649c658ce719b503ff0bdc37a83f. The formulas below use the full strong source (5), the exact canonical graph term, and the inherited all-time pole enclosure. [Hierarchy](../../GLR-GOAL-011/HIERARCHY.md) · [Finite data](../../GLR-GOAL-011/FINITE-DATA.md) · [Retarded pairing](../../GLR-GOAL-011/GREEN-FUNCTION.md) · [Pole enclosure](../../GLR-GOAL-011/SIGNED-RESIDUE.md)

An upper bound for the finite-window inversion error follows from three weighted norms of the returning source and graph trace and a finite band of the positive-half-plane scalar transfer. The returning denominator admits an analytic lower bound over that entire half-plane. The finite band and weighted source norms have no numerical enclosures here. The result is a conditional quantitative inversion bound for the fixed experiment, with the post-prompt dominance test still open.

## Keep the window while shortening the source history

Fix the observation interval $J=[c,d]$, with $c\ge u_* = u_\Sigma(13/8)$ and $u_\Sigma(r)=r-2x(r)$. Let $\chi(u)=1$ for $u\le d$ and $\chi=0$ for $u\ge d+w$, $w>0$. A $C^2$ choice in between is $1-10z^3+15z^4-6z^5$ with $z=(u-d)/w$. Its first and second derivative bounds are $15/(8w)$ and $10/(\sqrt 3\,w^2)$. Use $S_U=\chi(t-x)S$ in the returning scalar equation with the original exact-curl graph data. The cutoff acts after the constrained $HJ+H^2P+B_EP$ projection. Future causal curves have nondecreasing $u$, so $N_U=N$ through $d$. Compute the source from the unchanged lower-order hierarchy on cuts through $d+w$. [Finite-cut construction](../../GLR-GOAL-011/QNM-RESIDUES.md)

The auxiliary source has $u$ in $[u_*,d+w]$ and $x\ge X(d+w)$, where $X$ is the tortoise location of the graph at fixed $u$. The pinned right-end estimates give exponential decay in $x$ on this compact $u$ range for the source and the derivatives used below. Thus $S_U\in L^1_t L^2_x$. The compact graph data have finite Killing energy. For $V=f(6/r^2-3/r^3)\ge0$, the energy current is $((\Psi_t^2+\Psi_x^2+V\Psi^2)/2,-\Psi_t\Psi_x)$ and its divergence is $-S_U\Psi_t$. Its energy and flux through $v=\mathrm{const}$ are bounded by the graph energy and the work estimate $\int \|S_U(t)\|_2\|\Psi_t(t)\|_2\,dt$. In particular, flux in $du$ across this null boundary controls $|\Psi_u|^2$. The pointwise trace limit on compact cuts and Fatou give $N_U\in L^2(du)$, supported in $u\ge u_*$. The shear is its primitive from $u_*$. Both Laplace transforms are holomorphic in $\Re s>0$ for the auxiliary problem. The identity with the outgoing pairing holds first on a sufficiently right line and then by holomorphy throughout the positive half-plane. The exact unwindowed transform uses the different physical right-half-plane argument in GOAL-011. [Right-end estimates](../../GLR-GOAL-011/NULL-TRANSFORM.md)

## Returning transfer and the graph contact

Use the unit ingoing homogeneous solution $h=e^{sx}m$ on the real axis and the unit outgoing solution $g_+\sim e^{-sx}$. Write $W=hg_+'-h'g_+$ and $M=\int_{\mathbb R}V\,dx=9/2$. For $\gamma=\Re s>0$,
$$
 m(s,x)=1+\int_{-\infty}^x
       \frac{1-e^{-2s(x-y)}}{2s} V(y)m(s,y)\,dy ,
 \qquad W(s)=-2s\,m(s,+\infty).
$$
The Volterra kernel gives $\|m\|_\infty\le e^{M/|s|}$. The right limit is $1+\frac1{2s}\int_{\mathbb R}Vm\,dx$, and $m_x\to0$ there. With $a=m+m_x/(2s)$ and $b=-m_x/(2s)$, so $m=a+b$, the homogeneous equation gives
$$
 a'=\frac{V}{2s}(a+b),\qquad
 b'=-2sb-\frac{V}{2s}(a+b),\qquad
 (|a|^2-|b|^2)'=V\,\Re(1/s)|a+b|^2+4\gamma|b|^2.
$$
At the left endpoint $(a,b)\to(1,0)$. At the right $(a,b)\to(m(s,+\infty),0)$. Hence
$$
                  |W(s)|\ \ge\ 2|s| \qquad(\Re s>0).       \tag{1}
$$
This relies on the real nonnegative potential and the Jost endpoint convention. It gives a denominator bound on the positive inversion line regardless of the continued QNM and cut structure. There is also a useful uniform bound on the transfer density. Since $|a|^2-|b|^2\ge1$, $a$ has no zeros and $|b/a|<1$. The equation for $a$ gives
$$
 \partial_x\log|a|\ge
 \frac{V}{2}\left(\frac{\gamma}{|s|^2}-\frac1{|s|}\right).
$$
Integrate toward $+\infty$ and use $|a+b|\le2|a|$. Then
$$
 \left|\frac{s\,m(s,x)}{W(s)}\right|
 \le \min\left\{\frac12 e^{M/|s|},
       \exp\!\left[\frac M2\left(\frac1{|s|}-\frac{\gamma}{|s|^2}\right)\right]\right\}
 \le e^{M/(8\gamma)}.
$$
The last bound maximizes the exponent over $|s|\ge\gamma$.

The graph $t=T(x)=r(x)-x$ gives $X'=1/(f(X)-2)$, $|X'|\le1$, $|1+X'|\le1/2$, $X''=-f_x(X)(X')^3$, and $|X''|\le4/27$. In the exact-curl convention $\Psi_2|_\Sigma=0$. With the directed graph derivative $D_\Sigma=\partial_t+T'\partial_x$, the graph density in $du$ is
$$
 d_\Sigma(u)=|X'|D_\Sigma\Psi_2|_\Sigma
            =f(X)\,\Psi_{2,t}|_\Sigma .
$$
The derivative tangent to the graph annihilates the zero trace, and $|X'|(1-T'^2)=f(X)$. Compute the compact flux from the exact $\mathcal D^r-T'f\mathcal D^t$ graph identity of GOAL-011. The velocity supplies a graph contact even though the exact curl position vanishes. [Canonical pairing, Eq. (1) and launch derivation](../../GLR-GOAL-011/PAIRING.md)

For $y=u-u_*$, use the transform anchored at $u_*$, $H_U(s)=\int e^{-sy}N_U(u)\,du$, with
$$
 \begin{split}
 F_s(u)&=\int_{X(u)}^\infty m(s,x)S_U(u+x,x)\,dx
                         - m(s,X(u))d_\Sigma(u),\\
 H_U(s)&=\frac{s}{W(s)}\int e^{-sy}F_s(u)\,du.             \tag{2}
 \end{split}
$$
Equation (2) uses the strong source and graph contact once. In the free case $m=1,W=-2s$, it gives $N_U=-F_s/2$ with $F_s$ independent of $s$. The same sign follows by differentiating the free retarded Green function at null infinity. For a separate graph-only check, the free field is $P(u)+Q(v)$. With zero displacement on the graph, $P'+v_\Sigma'Q'=0$ and $\Psi_t=P'+Q'$, where $v_\Sigma'=f/(f-2)$. Thus the outgoing graph contribution is $P'=f\Psi_t/2=d_\Sigma/2$, the contact sign in (2). On $\Re s>0$, all endpoint estimates above are locally uniform and the source and contact give holomorphic pairings.

## A finite-band error bound

Before differentiating (2) in $u$, set $x=X(u)+\ell$ with $\ell\ge0$. The derivative along the resulting source integrand is
$$
 D=(1+X')\partial_t+X'\partial_x
$$
with coefficients differentiated when $D$ is iterated. Set, for $j=0,1,2$,
$$
 A_j(\gamma)=\int e^{-\gamma y}
   \left[\int_0^\infty
       |D^j S_U(u+X(u)+\ell,X(u)+\ell)|\,d\ell
         +|d_\Sigma^{(j)}(u)|\right]du.                 \tag{3}
$$
In particular $D u=1$, and the derivatives of the cutoff in (3) have the explicit bounds above. The launch jets are flat at the initial front, and the cutoff has vanishing first two derivatives at its terminal endpoint. The graph flux is smooth and compact. These facts and the right-end derivative bounds justify twice integrating by parts in $u$.

For $|\Im s|\ge B>0$ set
$$
 \begin{split}
 m_0 &= \exp\!\left[\frac{M}{\sqrt{\gamma^2+B^2}}\right],\\
 m_1 &= \frac{\|V\|_\infty m_0}{2\gamma},\qquad
 m_2 = \frac{\|V_x\|_\infty m_0+\|V\|_\infty m_1}{2\gamma}.
 \end{split}
$$
These are uniform bounds on $m,m_x,m_{xx}$. In fact $m_x(x)=\int_0^\infty e^{-2sz}(Vm)(x-z)\,dz$, so differentiating this expression replaces $Vm$ by $(Vm)_x$ without a large $|s|$ prefactor. One may use $\|V\|_\infty\le8/9$ and $\|V_x\|_\infty\le4/9$. The second bound follows by expanding $fV_r$ into its opposite-sign terms $ff_r(6/r^2-3/r^3)$ and $f^2(-12/r^3+9/r^4)$. Their absolute maxima are bounded by $4/9$ and $1296/3125$, respectively. Applying two $u$ derivatives to (2), using (1), gives
$$
 |H_U(\gamma+i\omega)|\le\frac{K}{|\gamma+i\omega|^2},\qquad
 K=\frac12\left[m_0 A_2+2m_1 A_1+
                       (m_2+4m_1/27) A_0\right],
 \qquad |\omega|\ge B.                                   \tag{4}
$$

Partition the retained band into cells of width $h_j$, centered at $\omega_j$, with point approximants $z_j$ and cellwise caps
$\sup_{\omega\ {\rm in\ cell}\ j}|H_U(\gamma+i\omega)-z_j|\le e_j$. Use conjugate-symmetric cells for the real trace. Invert the piecewise constant approximant exactly in $\omega$,
$$
 \widetilde N(u)=\frac{e^{\gamma y}}{2\pi}
   \sum_j h_jz_j e^{i\omega_j y}\operatorname{sinc}(h_jy/2).
$$
Plancherel gives the explicit finite-window bound
$$
 \boxed{\quad
 \|N-\widetilde N\|_{L^2(J)}
  \ \le\ e^{\gamma(d-u_*)}
       \sqrt{\frac{\sum_j h_j e_j^2}{2\pi}
                          +\frac{K^2}{3\pi B^3}}\ .\quad}     \tag{5}
$$
The squared contributions add because their weighted Fourier supports are disjoint. A uniform absolute error is also bounded by $e^{\gamma(d-u_*)}[\sum_j h_j e_j/(2\pi)+K/(\pi B)]$. The inversion stays entirely in the positive half-plane and retains continuous spectral contributions.

A way to cap the cell error is to add a rigorous center-point evaluation error to $h_j D_\omega/2$. The auxiliary transform is analytic on the circle of radius $\gamma/2$ around any center on the line. The uniform transfer-density bound and (2) bound its modulus there by $e^{M/(4\gamma)} A_0(\gamma/2)$, since $y\ge0$. Cauchy's estimate gives $D_\omega\le 2 e^{M/(4\gamma)} A_0(\gamma/2)/\gamma$ on the whole line. This is a finite but potentially loose fallback. Parameter cells in the radial calculation can instead supply $e_j$ directly. Center-point errors must include source enclosure over the physical GL input interval, graph contact, radial endpoints, and complex integration. An $L^1(e^{-\gamma y}du\,dx)$ source error $a$ alone adds at most $a e^{M/|s|}/2$ to the transform error with the exact RW kernel. The same weighted bound applies to the omitted source tail if it is enclosed using the uniform right-cut decay. Causal truncation after $d+w$ makes the all-time GL growth irrelevant to (3).

## Certified denominator for a dominance decision

The inherited physical, unwindowed $s_q,A_{21,q}$ pair defines $P(u)=2\Re(s_q A_{21,q} e^{s_qu})$ on $J$. For a subtraction on the analytic sheet one may multiply $P$ by a Heaviside function supported from $u_b\le c$, and use its transform
$$
 \frac{s_q A_{21,q}\,e^{-(s-s_q)u_b}}{s-s_q}
  +\frac{\overline{s_q A_{21,q}}\,e^{-(s-\bar s_q)u_b}}{s-\bar s_q}.
$$
It cancels the physical news poles and yields the displayed $P$ on $J$ independently of $u_b$. This news-first convention avoids an onset delta from differentiating a switched shear term. The auxiliary transfer used for (5) generally has a different continued numerator and hence different residues. It supplies $N$ on $J$ for comparison to the physical $P$. [Unwindowed pole subtraction](../../GLR-GOAL-011/NULL-TRANSFORM.md)

Write $\alpha=\Re s_q<0$, $C=s_q A_{21,q}e^{s_qc}$, and $T=d-c$. The exact Gram norm is
$$
 \|P\|_{L^2(J)}^2
 =2|C|^2\frac{e^{2\alpha T}-1}{2\alpha}
   +2\Re\!\left[C^2\frac{e^{2s_qT}-1}{2s_q}\right].            \tag{6}
$$
If $T\ge 8.40736132$, it contains one exact fundamental period. Applying (6) to that period yields the phase-independent bound
$$
 \|P\|_{L^2(J)}
 \ge |s_q A_{21,q}|e^{\alpha c}
       \sqrt{(1-e^{-2|\alpha|(2\pi/|\Im s_q|)})
                    (|\alpha|^{-1}-|s_q|^{-1})}.
$$
The enclosed $s_q$ disk and $A_{21,q}$ rectangle give $0.50965 e^{-0.177924681c}$ as a strict lower bound for $c\ge0$. This is in the ordered news units and dimensionless $du$. For $c<0$ use the upper endpoint of $\alpha$ in the exponential instead. The full physical signed rectangle is used.

Write $E$ for (5) and $L,U$ for an enclosed lower and upper norm of $P$. If the numerically enclosed supremum of $\|\widetilde N-P\|_{L^2(J)}$ over the inherited parameter box plus $E$ is less than $\rho L$, the dominance test passes. If the corresponding infimum minus $E$ is at least $\rho U$, the tested threshold fails on $J$. Both tests also need a time-quadrature enclosure for the finite expressions they use. The same $\widetilde N$ includes prompt and delayed waves, any shifted features, and all continuous spectral contributions to the finite-window waveform. Equation (6) supplies the pole norm, while the remainder estimate still requires (5).

## Checks, exposure, and open gates

From the scoped research directory, run the bounded non-production controls.

~~~sh
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-012/checks/c_inversion.py
~~~

[Source](../checks/c_inversion.py) · [Representative output and versions](../checks/c_inversion.txt)

- The Gram lower factor and upper period use 96-bit outward-rounded Arb balls for the inherited real projections of the complex pole disk. Direct time quadrature at a fixed point of the box separately reproduces the Gram formula.
- A unit square barrier with an exact homogeneous solution verifies the sign and endpoint flux identity at four complex frequencies. This is a manufactured check of (1) with no GL input.
- A free manufactured source with $F(u)=\sin^3(\pi u)\mathbf 1_{[0,1]}(u)$ has direct news $-F/2$ and an analytic transform from $\sin^3 z=(3\sin z-\sin 3z)/4$. The weighted inversion at $B=8,16,32$ has the recorded empirical error beneath the analytic truncation cap. The check applies to the free problem.
- The calculation uses inherited fixed-background equations, matching and signed residue outputs. Its physical-line estimates provide an analytic route independent of a continued-strip contour estimate or a direct numerical reconstruction of the full causal waveform. They were developed with exposure to GOAL-011 and without a separate blind audit. Runtime model/effort and speed are unverified.
- A continued off-pole projection must retain the actual ingoing $h(s,x)$ at the right. The GOAL-011 empirical routine's proportional outgoing test on the ray is appropriate at its returning zero only. For $s_*=s_q+2\Omega$, the compact baseline numerator divided by $W'(s_q)$ fixes the rank-one shifted baseline residue, and that residue supplies the inhomogeneous massive and returning stages if their inverses exist there. An entire graph contact contributes no new $1/(s-s_*)$ term through a regular inverse. The coordinator owns the separate shifted feature evaluation.
- Missing physical inputs to (5) are evaluated bounds on $A_0,A_1,A_2$ and the low-band cells for the complete physical source with its compact flux. These depend on the constrained mixed solution, the mean and GL profiles, and their finite-cut derivative and spatial tail errors. The GOAL-011 source enclosures apply at the fundamental pole. A finite-amplitude remainder under secular GL growth requires a separate estimate.
