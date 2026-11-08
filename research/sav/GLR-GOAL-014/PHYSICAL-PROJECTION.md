# The fixed-cut physical projection

Both physical signs remain open. The early compact candidate gives
$\mathcal P\simeq 7.8181$ in ordinary floating-point arithmetic.
The terminal term in the direct weak pairing is about $+19.9431$.
It cancels substantially against the integral terms. The independently
checked directed sign of the **homogeneous graph summand** is negative.
These statements concern different parts of the same causal solution.
[Compact calculation](STRONG-NEWS-PILOT.md) · [Decision budget](ERROR-BUDGET.md).

## Directed test and its endpoints

Use $B(u)=\sqrt\delta\,b(y)$, $y=(u-a)/\delta$ and
$b=b_0/\|b_0'\|_{L^2(0,1)}$, $b_0=e^{-1/[y(1-y)]}$ with flat extension.
In this section $W=w_0/\sqrt\delta$, $K=V/4$, $A_y^v$ integrates $K$
from the graph to $v$, $A_y=A_y^\infty$, and
$J(y)=A_y1=3/r_\Sigma-3/(4r_\Sigma^2)$. The leading scalar data give
$W=b-\mathcal T W$, where
$(\mathcal T W)(y,v)=\delta\int_0^y A_z^v W\,dz$ with a zero ray
integral below the graph. The exact rescaled test is
$\widetilde g=\sqrt\delta\,g=b'-\delta A_y W$ with $g=N_{01}$.
These identities retain the graph tilt. [Baseline equations](../GLR-GOAL-013/FLUX.md).

On both packet windows $J\le M=1.61$ and
$|\partial_u\log K|\le1/4$. Use the two-scatter candidate with the
diagonal kernel $J(z)^2/2$. Its primitive is the continuous piecewise
linear integral of the exact dyadic midpoint for $J^2b_0$ on each
dyadic cell, divided by the exact $\|b_0'\|_2$. Define
$$
 A(y)=\int_0^y b(z)dz,\qquad B_1(y)=\int_0^y(y-z)b(z)dz,\qquad
 R=W-b+\mathcal T b=\mathcal T^2 W .
$$
Volterra powers give $|W-b|\le Q=\delta M e^{\delta M y}A(y)$ and
$|R|\le R_*=(\delta M)^2 e^{\delta M y}B_1(y)$.
At fixed $v$, $|R_y|\le\delta M Q$ by differentiating the upper
limit in $\mathcal T^2$. The graph value of $R$ is zero. Thus there is
no moving-endpoint contribution to $(A_y R)'$, and its absolute
contribution to $\widetilde g'$ is at most
$\delta^2 M R_*/4+\delta^2 M^2 Q$.

For the second-scatter kernel the lower radial limit is $b_\Sigma(z)$
for $z\le y$, independent of $y$. Its $y$ derivative obeys
$|\partial_y K_2(y,z)|\le (\delta/4)e^{\delta(y-z)/4}J(z)^2/2$.
At $z=y$ the candidate and true diagonal agree, so differentiation of
the outer integral creates zero diagonal defect. The resulting kernel
derivative error is at most
$\delta^3 M^2 e^{\delta y/4}A(y)/8$. The pointwise value error has
the same coefficient multiplying $B_1(y)$. For a cell midpoint
integrand $c_j$, the dyadic primitive adds the integral of
$|c_j-J^2b_0|$ to the value error and the cell's
$|c_j-J^2b_0|$ to the derivative error, in both cases with factor
$\delta^2/(2\|b_0'\|_2)$. The candidate is continuous and its
piecewise derivative is its weak derivative. The leading flat front
gives $g(a)=0$ exactly.

All quadrature bounds in the table cover whole cells. The cell
normalization uses midpoint integration with the second-derivative
remainder for $(b_0')^2$ and flat endpoint caps. To convert back to
$u$, $\|g-n_0\|_{L^2(du)}=\|\widetilde g-\widetilde n_0\|_{L^2(dy)}$
and the corresponding derivative error has an extra factor $\delta^{-1}$.

| Quantity | Early $d=a+0.015$ | Packet $d=a+0.12$ |
|---|---:|---:|
| $\|g-n_0\|_2$ upper | $2.491\times10^{-10}$ | $2.831\times10^{-4}$ |
| $\|g'-n_0'\|_2$ upper | $1.900\times10^{-7}$ | $7.343\times10^{-3}$ |
| $\|g\|_1$ upper | $0.000791631$ | $0.271373$ |
| $\|g'\|_1$ upper | $0.413693$ | $19.176120$ |
| $g(d)$ | $[0.4125472765,0.4125472991]$ | $[0.0057047207,0.0097549333]$ |
| $|g(d)-n_0(d)|$ upper | $9.709\times10^{-9}$ | $2.025107\times10^{-3}$ |

The values use [directed output](checks/d_baseline_h1.txt).
The [independent envelope](investigators/E-ADVERSARIAL.md) confirms both
positive terminal tests using a different Picard remainder and also
encloses $g'(d)$ on the early window. In particular the finite packet
support leaves a nonzero scattered test at its last cut.

### Uniform early compact baseline jets

In physical strip coordinates $B'/B\ge\kappa=522$ and
$B''/B'\ge\kappa_2=380$. For the latter put
$t_b=1/[y(1-y)]\ge64/7$ and $q=1-2y\ge3/4$. If $j=B'/B$,
then $j'/j^2=-2/(q^2t_b^2)-2/t_b$ and $B''/B'=j(1+j'/j^2)$.
The causal primitive of $B$ is at most $B/\kappa$. With
$M=1.57$ and $C=(1-M/\kappa)^{-1}$, the Volterra equation gives
$|w_0|\le CB$, $|w_0-B|\le (C-1)B$ and
$|w_{0,u}-B'|\le MCB$. Let $\alpha(v)$ be the first physical
$u$ on a fixed $v$ line. Then
$w_{0,v}=-\int_{\alpha(v)}^u K w_0\,du'$ and
$|w_{0,v}|\le K_* CB/\kappa$, with $K_*=2/9$ here denoting a
potential cap. On the tilted graph $|b_\Sigma'|<0.24$ and
$|\alpha'|<4.23$ when $\alpha(v)$ lies on the graph. At the front
$\alpha'=0$ and the field is flat. Differentiating these ray identities
gives
$$
 \begin{aligned}
 w_{0,uu}-B''&=b_\Sigma'K|_\Sigma B-\int_{b_\Sigma}^v
                   (K_u w_0+K w_{0,u})\,dv',\\
 w_{0,uv}&=-Kw_0,\\
 w_{0,vv}&=\alpha' K(\alpha,v)B(\alpha)-
             \int_\alpha^u(K_v w_0+K w_{0,v})\,du'.
 \end{aligned}
$$
Use $|K_u|,|K_v|\le K/4$ and the ray mass and pointwise caps.
The relative errors of the free outgoing candidate in
$w_0,w_{0,u},w_{0,v}$, with denominators $B,B',B'$,
are below $0.003017,0.003017,8.181\times10^{-7}$ respectively.
The corresponding errors in $w_{0,uu},w_{0,uv},w_{0,vv}$ divided
by $B''$ are below $0.004147,1.124\times10^{-6},4.740\times10^{-6}$.
These are pointwise physical compact bounds with the original lower
limit, applicable to the baseline input of the massive source. The
ratios are on the open early window and the flat front has zero
absolute errors. [Directed constants](checks/d_baseline_h1.txt).

## Evaluated homogeneous graph summand

In the exact-curl convention, split the linear scalar return into
$w=w^\Sigma+w^{\rm vol}$, where $w^\Sigma$ solves homogeneous RW
with zero graph displacement and the complete induced return graph jet.
The second term has zero displacement and velocity and the complete
strong volume source. This mathematical split leaves the total
solution fixed. In the weak-current formulation the graph data are
already represented by the currents. A separate addition of
$w^\Sigma$ to the weak solution would count the launch twice.

At the graph use $p=-1/r$, $n=(1,-p)^T$, $o=(-p,1)^T$, and
$A_0=((fB/r-B'),B')^T/(2r)$. Denote the unhalved, time-weighted first
mass matrix by $\bar M=-r^2 X\operatorname{diag}(-1,1)$ with
$X=b_0^{-1}h$. Here $b_0$ in the endomorphism is the background orbit
metric. The actual mixed source is $J=\lambda\bar M A_0/2$.
The canonical conditions give
$$
 A_1^+=n\,\frac{n^T\bar M A_0}{2r^2(1-p^2)},\qquad
 \mathcal D_{\rm mix}=\lambda\bar M A_1^+ .
$$
This uses the real graph values of the noncompact components and
includes the compact electric condition. The latter is required even
though $A_z^+|_\Sigma=0$. The direct mean matrix is
$r^2[X^2-(2d_s^2+\operatorname{tr}X^2/4)\mathrm{Id}]
\operatorname{diag}(-1,1)/2$, with $d_s=H_{\theta\theta}/(2r^2)$
including the graph time factor. The factor $1/2$ comes from the
circle average. The fixed-mass matrix is
$r^2s\begin{pmatrix}1&-1\\-1&1\end{pmatrix}$,
$s=\beta-2\mu/(rf)$. [Mean and graph inputs](CURRENTS-AND-DATA.md).

For each part the exact graph slope is
$$
 q_\Sigma=(w^\Sigma)_u\big|_\Sigma
   =\frac{(1+p)}2 (w^\Sigma)_t\big|_\Sigma
   =\frac{o^T\mathcal D}{\lambda r(1-p)}.
$$
The sign of the tilt follows from $w_x=-p w_t$ on the graph.
For the corresponding homogeneous RW solution the departure of the
news from $q_\Sigma$ is bounded pointwise by
$M\int_a^u e^{M(u-z)}|q_\Sigma(z)|\,dz$.
This follows from the exact equation
$w_u=q_\Sigma-\int_{b_\Sigma}^v K w\,dv'$
and the zero-displacement Volterra shear equation.
The test in this check uses $B'-JB$ with error bounded by
$M^2 e^{M(u-a)}\int_a^u B(z)dz$. The directed quadrature encloses
the graph currents over whole cells using the regular GL IVP across
the parameter interval and the full fixed-mass mean boxes.

| Early homogeneous graph part | Directed overlap with $N_{01}$ |
|---|---:|
| Mixed | $[-3.45616,-3.37746]\times10^{-4}$ |
| Local direct | $[1.25254,1.32106]\times10^{-4}$ |
| Fixed-mass mean | $[1.51115,6.42662]\times10^{-5}$ |
| Complete graph sum | $[-2.05250,-1.41374]\times10^{-4}$ |

The independent central IVP and compactified RW implementation gives
approximately $-1.74125\times10^{-4}$ for the sum. It caught an incorrect
graph tilt factor in the first directed implementation. The corrected
formula above and the reproduced output supersede that run.
[Corrected output](checks/d_graph_projection.txt) ·
[Retained failure](checks/d_graph_wrong_normal.txt) ·
[Independent march](checks/e_graph_projection.py).

## Physical sign gate

For a certified compact projected candidate interval $[p_-,p_+]$, the
adjoint budget gives $\mathcal P\in[p_- - E,p_+ + E]$. The early
candidate values in the compact pilot guide the required tolerance.
They supply no directed $p_-$ or $p_+$. The limiting input is a
whole-cell bound for the mixed and direct current errors on the early
strip, including the terminal ray. In particular, an outward bound for
$|\mathcal F_c(d)-\mathcal F_{h,c}(d)|$ would enter at weight
$g(d)/4\simeq0.10314$. A separate budget must cover the nonterminal
errors. [Compact adjoint and exact cut](ADJOINT-AND-ENDPOINTS.md).

The graph term is small and negative. The main cancellation in the
physical candidate is between the terminal ray and the bulk terms of
the weak formula. The empirical sign does not establish the sign of
the full constrained solution. The full packet calculation waits for
the early volume gate, and the two QNM tests wait for a physical
strong-news error on their own window.
