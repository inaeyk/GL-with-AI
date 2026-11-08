# Strong cap news from one commuted current

Use the fixed cuts and current conventions in [INPUTS](../INPUTS.md)
and [014, Currents and Data](../../GLR-GOAL-014/CURRENTS-AND-DATA.md).
All variables are dimensionless. Write $D_c=\{a\le u\le d,\,
b(u)\le v\le c\}$, $w=w_2$ and $V=f(6/r^2-3/r^3)$. The exact
physical data and equation are
$$
 4w_{uv}=F_u+G_v-H-Vw,\qquad w|_\Sigma=0,\qquad
 4w_u|_\Sigma=G-b'F,\qquad w|_{u=a}=0.
$$
Here $\partial_t=\partial_u+\partial_v$ denotes *stationary* time
differentiation at fixed tortoise coordinate. It is not differentiation
along the initial graph.

## Exact identity

Set $P^\flat=4w_u-(G-F)$. Then
$\partial_vP^\flat=F_t-H-Vw$ and $P^\flat|_\Sigma=(1-b')F|_\Sigma$.
Thus
$$
 4w_u(\cdot,c)=(G-F)_c+(1-b')F_\Sigma+
                \int_{b(\cdot)}^c(F_t-H-Vw)\,dv.                 \tag{1}
$$
Alternatively, use the moving-limit derivative in 014 Eq. (A2) and
$\int_b^c F_u\,dv=\int_b^c F_t\,dv-F_c+F_\Sigma$. The moving
limit subtracts $b'F_\Sigma$. Both derivations give the same graph
coefficient. The $G$ trace at the cap must remain, and the physical
terminal contribution to the sharp projection is still present:
Eq. (1) is a pointwise cap-news formula, not a replacement for the
sharp-cut weak adjoint.

The new feature is an a posteriori **strong** error estimate requiring
the stationary derivative of **one** current and no separate scalar
return lift with a second-order strong residual. The $F_\Sigma$ and
$(G-F)_c$ traces and derivative $F_t$ must be genuine. A piecewise
candidate with nonzero $F$ jumps has distributional $F_t$; a formula
using only its cell derivatives omits the corresponding seam impulse
unless the trace contribution cancels or is evaluated separately.
For the estimate below take a conforming candidate $F_h$ with the
stated weak derivative and traces. An independently supplied
approximation to $F_t$ is also allowed: it must be compared with the
true derivative, not simply declared to be the candidate derivative.

## Rectangular-norm Volterra estimate

Zero extend the fields and currents below the physical graph and use
the rectangle $[a,d]\times[b(d),c]$. In the following, $F_t$ means the
interior derivative **before** zero extension. Applying $\partial_t$
to a zero-extended current instead would create an additional graph
delta. Let $\mathcal I$ integrate from $a$ in $u$, with
$\|\mathcal I\|_{2\to2}=\kappa=2(d-a)/\pi$; denote integration in
$v$ from $b(d)$ by $\mathcal J$. The exact causal equation for the
zero-extended shear is
$$
 \widetilde w=J_s-\mathcal T\widetilde w,\qquad
 4J_s=\mathcal J\widetilde F+
           \mathcal I(\widetilde G-\mathcal J\widetilde H),\qquad
 4\mathcal T=\mathcal I\mathcal J(\widetilde V\,\cdot).              \tag{2}
$$
Below the graph all terms vanish because $b$ decreases. On the graph
there is no missing derivative contact by the canonical graph row.
The exact equation is 013 Eq. (2), expressed on a rectangle.

Use $Y=L^\infty_vL^2_u$ and $U=L^1_vL^2_u$ on the zero extensions.
Set $M=\frac14\int_{b(d)}^c\|\widetilde V(\cdot,v)\|_\infty\,dv$.
The $j$th Volterra iterate in $v$ lies on an ordered simplex, so
$\|\mathcal T^j\|_{Y\to Y}\le(\kappa M)^j/j!$. For any bounded
candidate shear $w_h$ let $r_h=\widetilde w_h-J_{s,h}+\mathcal
T\widetilde w_h$; alternatively define $w_h$ by the exact candidate
Volterra solve and take $r_h=0$. With $E_F=\|\Delta F\|_U$,
$E_H=\|\Delta H\|_U$, $E_G=\|\Delta G\|_Y$ and
$E_{\rm vol}=\|r_h\|_Y$, the resolvent yields
$$
 \|\Delta w\|_Y
 \le e^{\kappa M}
 \left[E_{\rm vol}+\tfrac14\big(E_F+\kappa(E_G+E_H)\big)\right].
                                                                    \tag{3}
$$
In particular no $L^2_uL^\infty_v$ maximal estimate on the $G$
error is needed. $E_G$ measures fixed-$v$ slices, the orientation of
the lower fields' ingoing null energy flux.

Let $E_C=\|\Delta(G-F)_c\|_2$, $E_\Sigma=\|\Delta F_\Sigma\|_2$,
$E_T=\|\Delta F_t\|_U$, and $\beta=\sup|1-b'|=26/21$. Let
$E_{\rm read}$ bound the difference in $L^2(I)$ between Eq. (1)
evaluated on candidate inputs and the affine reference $p_c$; this
includes any numerical evaluation and candidate trace interpolation
error. No differentiations of the affine $p_c$ enter. Combining (1)
and (3) gives
$$
 \begin{split}
 E_c\le E_{\rm read}
    &+\tfrac14(E_C+\beta E_\Sigma+E_T+E_H)\\
    &+M e^{\kappa M}
           \left[E_{\rm vol}+\tfrac14(E_F+\kappa(E_G+E_H))\right].
 \end{split}                                                        \tag{4}
$$
This estimate is independent of the adjoint used for the weak
pairing. It still depends on the same physical lower fields. It is
conditional on whole-cell, trace, and seam inputs. In particular,
feeding the sampled current residual into (4) does not yield a
continuum upper bound.

For $c=d+2x(400)$, bound $\|\widetilde V(\cdot,v)\|_\infty$
by $e^{(d-a)/4}V(d,v)$ from $|\partial_u\log V|\le1/4$ for
$r\ge b(d)$ (014 strong pilot). Integrating the last ray gives
$$
 M\le e^{(d-a)/4}\left(\frac3{b(d)}-\frac3{4b(d)^2}
                      -\frac3{400}+\frac3{4(400)^2}\right).        \tag{5}
$$
The candidate-specific positivity test continues to require
$E_c\le570$ (target $400$), with the same exterior and leading-news
hypotheses as 014. To protect that target, the seven errors in (4)
must be enclosed for the **same** data and candidate.

## Computing the physical time derivative

Write $B=w_0$, $Y_m$ for the real normalized two-polarization mixed
field. Split $F=\mathscr F_mY_m+\mathscr F_0B$, where $\mathscr
F_m$ includes only the linear homogeneous mixed reconstruction and
$\mathscr F_0$ includes all terms linear in $B$ (in particular the
affine $J$ terms from the mixed reconstruction, the direct quadratic
geometry, and the fixed-mass mean). Both are first-order operators
in stationary coordinates. Their coefficients carry exactly
$e^{\Omega t}$ and $e^{2\Omega t}$, respectively. The mean
constraint is a fixed-$v$ radial integral and carries $e^{2\Omega
v}=e^{2\Omega t}e^{2\Omega x}$, consistent with this statement.
Consequently
$$
 F_t=\mathscr F_m(\partial_t+\Omega)Y_m+
                  \mathscr F_0(\partial_t+2\Omega)B.                \tag{6}
$$
The factor applies to the affine $J$ terms as part of $\mathscr
F_0$, not as a homogeneous mixed field. If
$L_mY_m=\mathscr S_m B$, the stationary $L_m$ commutes with
$\partial_t+\Omega$ and the first insertion $\mathscr S_m$
carries $e^{\Omega t}$. Thus
$L_m(\partial_t+\Omega)Y_m=\mathscr S_m(\partial_t+2\Omega)B$.
The two required commuted graph jets follow from the tilted graph
equations, not zero canonical data for the commuted field.
A commuted solve can use the same two-polarization operator and
source evaluator with shifted lower data; its error still needs
the correct new graph traces and conforming residual.

## Why weak current bounds cannot give a strong trace

On the actual scalar strip choose nonzero $\chi\in
C_c^\infty((a,d))$ and a smooth $\rho(v)$ vanishing below a cut
strictly above the graph and equal to 1 near $c$. Put
$w_n=n^{-1/2}\chi(u)\sin(nu)\rho(v)$ and
$$
 F_n=4(w_n)_v+\int_a^u V(z,v)w_n(z,v)\,dz,\qquad G_n=H_n=0.
$$
It satisfies the actual scalar equation and zero graph/front data.
Both the current norms $E_F,E_G,E_H$ and boundary current norms
$E_C,E_\Sigma$ tend to zero; the signed terminal $F_n(d,\cdot)$
is $O(n^{-1/2})$ too. In contrast
$\|(w_n)_u(\cdot,c)\|_2=\sqrt{n}(\|\chi\|_2/\sqrt2+o(1))$
by the oscillatory average. This rules out a bounded strong trace
map in those undifferentiated norms even with a terminal trace.
It is a manufactured error in the scalar current class, not an
allowed variation of the actual constrained GL lower fields.
It explains the $F_t$ requirement or the need for an equivalent
regularity bound using the lower hierarchy.
