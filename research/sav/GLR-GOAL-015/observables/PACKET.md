# Full packet: signed subprincipal weak estimate

Use the actual cut $[a,a+3/25]$ and cap $c=d+2x(400)$. This is a
different strip and a separate error budget. In particular $B'$ and
$B''$ have interior zeros, so the relative early jet estimates
cannot be extended. The [compact packet pilots](../checks/COMPACT.md)
give a reproducible negative candidate near $-1455.615$. Their
directed candidate intervals and strong sufficient tolerance do
not bound the physical compact mismatch. The present proof does
not use the packet pilot, its derivative arrays or its sign.

Use the exact rotation, graph data and current rows of
[ADJOINT](../analysis/ADJOINT.md). Let $D=3/25$, $\kappa=2D/\pi$.
The two estimates below use $L^p(du)$ on each fixed-$v$ slice with
zero extension below the graph, for $p=2,\infty$. The primitive
constant is $k_2=\kappa$, $k_\infty=D$. Define $\|B\|_{p}$ and the
corresponding first and second derivative norms by the pinned
packet (directed quadrature for $p=2$, cell suprema for $p=\infty$).
In particular $\|B'\|_2=1$ and $\|B''\|_2^2>4403.4178$.

## Baseline and source split

Write $W=V/4$ and $J(u,v)=\int_b^v W\,ds$. For fixed $u$ this is
$3/b-3/(4b^2)-3/r+3/(4r^2)$. We use $W\le1/4$,
$|\partial_{u,v}W|\le W/4$ and $M=1.60$ as an upper bound on
$\int\sup_u W\,dv$, including infinity. To justify the orientation
of this last bound, bound the part with last-ray radius below $2$
by $e^{D/4}$ times the last ray and use monotonicity above $2$.
With $J_\infty(s)=3/s-3/(4s^2)$ and $r_{\rm cut}=1.596<b(d)$,
$J_\infty(r_{\rm cut})+(e^{D/4}-1)
[J_\infty(r_{\rm cut})-J_\infty(2)]<1.60$.
The same $M$ bounds every fixed-$u$ ray integral.

The exact scalar graph gives $w_0=B$, $(w_0)_u=B'$,
$(w_0)_v=0$ there. The weak characteristic scalar estimate gives
$Y_0=\sup_v\|w_0(\cdot,v)\|_p\le e^{k_pM}\|B\|_p$.
The two null integrals give $\|(w_0)_u-B'\|\le MY_0$ and
$\|(w_0)_v\|\le k_pY_0/4$ on each slice (the differences are
restricted to the physical domain). Differentiating the moving
lower limit on the outgoing ray gives
$$
 (w_0)_{uu}-(B''-JB')
 = b'W_\Sigma B-\int_b^v \big(W_u w_0+W((w_0)_u-B')\big)\,ds .
$$
Its norm is bounded by
$|b'|_* \|B\|_p/4+M(Y_0/4+MY_0)$. The mixed derivative is
$-Ww_0$. For $(w_0)_{vv}$ there is the graph term
$\ell'W(\ell,v)B(\ell)$ in the wedge and the volume derivative
$-\int_\ell^u(W_vw_0+W(w_0)_v)$. The graph term has norm at most
$5\sup|B|D^{1/p}/4$ and is zero above the wedge. These formulas
also bound $g'-(B''-J_\infty(b)B')$ at infinity; the independently
enclosed $g(d)$ and $\|g\|_2$ enter the endpoint and dual estimates.

Let $e_1=(1,0)^{\mathsf T}$ and $A=\int_b^v q_0\,ds$. In the
rotated mixed equation split the true source as
$$
 R^{\mathsf T}S=q_0e_1 B''+S_1B'+\mathcal R_S,\qquad
 S_1=R^{\mathsf T}S_u^{\rm coeff}-q_0 J e_1 .
$$
The superscript means the coefficient of $(w_0)_u$ in the source,
not a derivative of the source. All six remainder weights are
bounded by the scalar estimates above. Define
$$
 X_0=-Ae_1B'/4,\quad Q_0=-q_0e_1B'/4,\qquad
 P_0=-Ae_1B''/4+C B',
 \quad C=C_{\Sigma,1}+\int_b^v(-M_+X_0/B'-S_1/4)\,ds ,
$$
where the quotients are the explicitly given coefficients, also
at zeros of $B'$. The incoming $C_{\Sigma,1}$ is the signed
coefficient of $B'$ in $P_\Sigma$; the remaining $B$ graph term
stays in the error.

## Coupled residual and signed projection

The exact outgoing error satisfies
$(P-P_0)_v=-M_+(X-X_0)-\mathcal R_S/4$.
The fixed-$v$ equation for $X-X_0$ has homogeneous part
$2L(X-X_0)$ and forcing
$$
 P-P_0+\left(C+\frac{A_u e_1-2A L e_1}{4}\right)B',
 \qquad A_u=\Omega A-q_0+(1-b')q_{0,\Sigma}.
$$
It has zero incoming displacement because $A=0$ on the graph
and $B'=0$ at the front. The analogous equation for $Q-Q_0$ has
forcing $-M_-(X-X_0)-\mathcal R_S/4+
[(q_{0,u}e_1-2q_0Le_1-S_1)/4-M_-X_0/B']B'$. The nonzero wedge
graph discrepancy $Q_\Sigma-Q_0|_\Sigma$ is retained. Skew
transport is unitary and the primitive bound applies to the
norm of the integrated forcing. At fixed $v$, $q_{0,u}$ is
$-(f/2)e^{\Omega v}$ times the radial derivative of the
factored coefficient. Whole-cell upper envelopes for these
relations give the one-dimensional positive bootstrap for
$\sup\|P-P_0\|$ in each radial slab; its exponential factor is
$e^{\|M_+\|k_p\,\Delta v}$. The accumulated error at the end of
a slab uses its full measure and that supremum. The intervals
for the partial integrals $A,C$ include zero partial measure
and the whole cell. Cells meeting the graph wedge include
optional full measure, so they cover every active subray.

The **signed** outgoing primitive of the $F$ current then has
the form
$$
 \int_b^c F\,dv=\frac{A(u,c)^2}{\lambda}B''(u)+C_F(u)B'(u)
                         +\mathcal R_F(u).
$$
The leading coefficient follows from the exact first current
row $-8q_0 e_1^{\mathsf T}/\lambda$. The signed $C_F$ is the
integral of the current on the explicit $B'$ coefficients of
$P_0,Q_0,X_0$ and the baseline $F$ row. The residual bound
uses the exact remaining mixed and baseline errors, without
sampling derivatives. We enclose the range of $C_F$ by one
interval over the full width. Its midpoint contributes
zero to $\int B'' C_F B'$ because $[B'^2]_a^d=0$. Only the
interval radius pays the leading $B''$ work, while the full
coefficient pays the smaller error $g'-B''$. The same
constant-subtraction identity is applied to
$[A(u,c)^2/\lambda]J_\infty(b(u))B'B''$ in the baseline test.
The terminal ray remains: $B'(d)=B''(d)=0$, but $g(d)>0$
and $\mathcal R_F(d)$ is bounded separately by the supremum
calculation.

Use the actual infinity test $Z$ from the sharp-cut adjoint.
The weak exterior cancels the compact cap $G$ contact;
its remaining physical error is below $1.63462\times10^{-14}$.
On the full strip
$\|\sup_v|Z|\|_2\le e^{MD}\|g\|_2$ and
$\|\sup_v|Z_u-g'|\|_2\le M e^{MD}\|g\|_2$.
For $G$, use $\|Z_v(\cdot,v)\|_2\le
\kappa e^{MD}\|g\|_2\sup_u W(u,v)$, with zero extension.
These bounds multiply the directed uncommuted
$L^1_vL^2_u$ current envelopes. The sign test uses all
three currents and the distinct terminal contribution.

## Numerical disposition

The [first directed attempt](packet_weak_first.txt) bounded
the entire nonprincipal $F$ by absolute value. It enclosed
the physical result across zero, with $F$ work above
$5533$ in the budget for $4\mathcal P$. The initial assertion
also assumed the wrong sign for $A$ itself; its square is
the invariant relevant quantity. The [failed run](packet_phase_sign_failure.txt)
is retained with the local checkout prefix removed. Correcting
that assertion did not change the interval algorithm.

The [signed refinement](packet_signed.py) reduces the remaining
$F$ ray mismatch to $\|\mathcal R_F\|_2<8.813$ and
$|\mathcal R_F(d)|<45.531$. The $4\mathcal P$ negative optical
term has magnitude above $4393.8267$; all adverse terms together
are below $3593.461$. The result rounds outward to
$$
             -2758.258<\mathcal P_{\rm packet}<-200.091 .
$$
This is a sign enclosure for the complete physical *ordered*
response. The large width reflects the conservative signed
coefficient, dual and residual bounds. It does not provide a
strong packet waveform bound. In particular the independent
packet strong gate $E_c\le1451$ is unevaluated. See the
[directed output](packet_signed.txt) and [adversarial audit](../verification/PACKET.md).
