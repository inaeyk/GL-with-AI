# Strong news on the early null strip

The calculation uses the fixed ordered hierarchy in units $r_0=1$ and covers
the first eighth of the packet. The candidate cap is $v_c=d+2x(400)$,
with minimum cap radius $400r_0$. The compact error in the true return
news remains open.

## Physical compact candidate

On the graph $v=b(u)$, let $\phi=a_\delta(u)$ and $p=T'=-1/r$.
The leading Regge–Wheeler field and its stationary time commutation use
$$
 w_0|_\Sigma=\phi,\quad (w_{0,t},w_{0,x})|_\Sigma=(\phi',-\phi'),\qquad
 w_{0,tt}|_\Sigma=\phi''-\frac{V\phi}{1-p^2},\quad
 w_{0,tx}|_\Sigma=-\phi''+\frac{pV\phi}{1-p^2}.
$$
The latter follows from $w_{0,xx}=w_{0,tt}+Vw_0$ and tangential
differentiation of the first two jets. The commuted displacement and
normal derivative are separate inputs.

Let $\boldsymbol w=(y,Z)=(U/\sqrt D,\Im z_c)$ for the $+k$ mode.
The real mixed skew matrix $C_{\mathbb R}$ has off-diagonal entries
$(-c,c)$, where $c=2k\sqrt\lambda f/(\lambda+k^2r^2)$. The mixed
equations and source $S_m$ are the two time-domain rows of the pinned
[GOAL-013 source ledger](../GLR-GOAL-013/SOURCE-LEDGER.md).
For a field $w$ with null derivative pair $(P,Q)=(w_u,w_v)$ define
$$
 K_{\rm drive}=\frac{C_{\mathbb R}(P+Q)-Ww-S}{4}.
$$
The characteristic updates integrate $P_v=K_{\rm drive}$ at fixed $u$
and $Q_u=K_{\rm drive}$ at fixed $v$. The incoming values are the canonical
graph jets or the flat front, and $w=\phi_\Sigma+\int_{b(u)}^v Q(u,s)\,ds$.
This directly transports both mixed derivatives used to reconstruct
$A_{1t},A_{1x}$. The compact-unitary GL profile uses a horizon-regular
real solution up to $r=2.5$ and the decaying Riccati branch beyond that
radius. The fixed-mass mean uses both EF constraint integrals, integrated
from the finite numerical exterior end. That end is at $430r_0$ for
the $400r_0$ cap candidate. The far-end numerical proxy is empirical.
The candidate's physical coefficient error on the whole strip remains
a separate budget item.

The derivative-reduced returning march uses $w=J_s-\mathcal T w$ and
retains the cut term. A separate strong march forms $S=-F_u-G_v+H$ by
directional differentiation of the current arrays and uses the canonical
graph launch $4w_u|_\Sigma=G-b'F$ and $w_v|_\Sigma=-w_u/b'$. The cap
candidate $p_c$ is the continuous piecewise affine interpolant of the
binary strong cap ordinates at the **exact** abscissae
$u=a+j(d-a)/192$. Directed arithmetic evaluates its norm and pairing
with the free profile. A separate empirical exterior continuation
integrates the potential-only scalar cap equation in $1/r(d,v)$, with
its regular infinity endpoint.

The $192\times720$ Simpson run at $r_c=400$ gives these float64 diagnostics.

| Compact candidate calculation | Value |
| --- | ---: |
| Weak projection at the cap | $+7.81810240688$ |
| Shear-spline differentiation and direct projection | $+7.81810227383$ |
| Separate differentiated-current strong march | $+7.81810227291$ |
| Discrete transpose of the weak shear march | $+7.81810240688$ |
| Potential-only continuation of strong candidate | $+7.81781765437$ |
| Strong versus weak shear-derivative cap news $L^2$ difference | $7.50\times10^{-7}$ |
| Mixed, local quadratic, fixed-mass mean cap projections | $+7.31750024,\ +0.29413690,\ +0.20646527$ |
| Terminal-cut term | $+19.94308724$ |

The first three evaluations treat the derivatives differently and share
the lower fields, GL IVP, mean and grid. The discrete transpose checks
the linear algebra of the same discrete Volterra equation. Their agreement
leaves common coefficient, lower-field, grid and truncation errors open.

## Directed baseline, mixed graph and conditional strong gate

The graph-exact physical baseline comparison $w_{0,h}=a_\delta(u)$ has
uncommuted residual $R_0=-Va_\delta$ and time-commuted residual
$-Va_\delta'$. For $D_u=d-a$ and $r_b=b(d)$ set
$$
 J_b=3/r_b-3/(4r_b^2),\qquad L_1=4J_b e^{D_u/4},\qquad
 L_2=\sqrt{2(12/r_b^3-18/r_b^4+9/r_b^5-3/(2r_b^6))}.
$$
The first bounds $\int\sup_u V(u,v)\,dv$ using
$\lvert\partial_u\log V\rvert\le1/4$ for $r\ge r_b$. Indeed
$(\log V)_x=1/r^2-f(4r-3)/(r(2r-1))$, where the first term is at
most $1/r_b^2<1/2$ and the subtracted term is at most $2f/r\le1/2$.
The second bound integrates $V^2\,dv$ on the ray with minimum initial
radius. The mixed norm work bound is
$B(h)=(L_1\|h\|_2+L_2\|h\|_1)/2$, valid on every cap. The
once-commuted graph displacement defect vanishes and its time
derivative defect has magnitude $V\phi/(1-p^2)$. The graph measure
$\lvert dx/du\rvert=1/(1-p)$ converts its energy to
$\int (1+p)(V\phi/(1-p^2))^2\,du/2$. Whole-cell bump quadrature gives:

| Directed baseline component | Upper bound |
| --- | ---: |
| Uncommuted outgoing null error flux square root | $7.36904\times10^{-5}$ |
| Once-commuted incoming graph energy | $9.927\times10^{-11}$ |
| Once-commuted outgoing null error flux square root | $0.0436057$ |
| $E_0=\|N_{01}-a_\delta'\|_2$ from scalar Volterra | $3.71512\times10^{-5}$ |

The last row uses $\|w_0\|_X\le e^{J_bD_u}\|\phi\|_2$ and
$N_{01}-\phi'=-\int_b^\infty Vw_0\,dv/4$ and is independent of
the empirical baseline mesh.

For the **uncommuted mixed graph**, define the exact candidate lift
by affinely interpolating the two binary raw graph ordinates
$(U_t,\Im A_{z,t})$ from the strong march as functions of
$(u-a)/D_u$, followed by exact stationary rescaling to $(y_t,Z_t)$.
Its displacement and graph tangential derivative vanish. Comparison
in 768 complete graph cells with directed horizon-normalized GL jets
over the inherited spectral box gives
$$
 \sqrt{E_{{\rm in},m}}<6.403\times10^{-4},\qquad E_{{\rm in},m}<4.10\times10^{-7}.
$$
The single $+k$ energy integrand in $dr$ is
$(1-p^2)(|\delta U_t|^2/D+W|\delta\Im A_{z,t}|^2)/(2f)$.
The conjugate is fixed by reality. The maximum raw jet error bounds
are $0.129620$ and $0.007070$. The same rounded bounds hold with
160 and 192 bits. This evaluates the graph term for a lift with
these traces. A conforming volume reconstruction with the same traces
is still required.

The specific affine cap candidate has the Arb enclosures
$$
 635.49461514<\|p_c\|_2<635.49461516,\qquad
 7.80653319<\langle a_\delta',p_c\rangle<7.86815471 .
$$
For the true cap trace $P_c$, define $E_c=\|P_c-p_c\|_2$. The
ordinary $L^2$ positive Volterra factor is $e^\alpha$, where
$$
 \alpha=\frac{2D_u}{\pi}\left(\frac{3}{400}-\frac{3}{4\cdot400^2}\right).
$$
The same integrated operator bound gives $\|T_{\rm ext}-1\|\le
e^\alpha-1$ for the homogeneous transfer. Use the audited
$1.701\times10^{-6}$ upper bound on the exterior forcing contribution
and the GOAL-013 bound $\|N_{01}\|_2<0.01364715$ on the early strip.
Then
$$
\begin{aligned}
 \left|\mathcal P_{\rm early}-\langle a_\delta',p_c\rangle\right|
 &\le E_0\|p_c\|_2+
   \|N_{01}\|_2\bigl[e^\alpha E_c+(e^\alpha-1)\|p_c\|_2+
                             1.701\times10^{-6}\bigr]\\
 &<0.02423013+0.01364812684 E_c .
\end{aligned}
$$
In particular $E_c\le570$ suffices for the positive early sign for
this candidate. The comparison uses the untransferred cap candidate
and the analytical transfer estimate, avoiding an exterior quadrature
error term.

## Unclosed compact defect

The strong budget requires a conforming volume reconstruction of
$w_{21,h}$ matching $p_c$ to known trace error, an incoming graph
defect $E_{{\rm in},2}$ and an integrated strong residual
$$
 B_2=\tfrac12\left(\|L_{\rm RW}w_{21,h}-S_{21}\|_{L^1_vL^2_u}
                  +\|L_{\rm RW}w_{21,h}-S_{21}\|_{L^1_uL^2_v}\right).
$$
The sufficient criterion above requires
$(B_2+\sqrt{B_2^2+4E_{{\rm in},2}})/2$ plus the cap trace defect
to be at most $570$. The array solution with directional cubic
differentiation supplies neither a verified whole-cell $B_2$ nor a
returning graph defect for a conforming volume lift. In particular
$-\partial_u(F-F_h)-\partial_v(G-G_h)+(H-H_h)$ contains the first
derivatives of the mixed reconstruction error. The uncommuted
mixed graph energy and empirical second-derivative samples do not
control these. A commuted mixed graph and volume residual, or an
estimate that exploits conservation before taking these derivatives,
is needed. A jump of the conormal derivative at a non-null
interpolation seam contributes to the wave residual. Differentiating
a continuous null-aligned interpolant for the time-commuted equation
can also introduce a seam term. The candidate-specific target and
some lower-level terms are bounded. The physical compact $E_c$,
both interference signs and the later QNM window remain open.
