# D — fixed-width tensor interference against the exact causal baseline

## Inputs and exposure

The source checkout was at `eb439228601a649c658ce719b503ff0bdc37a83f` with unchanged `FINITE-DATA.md`, `HIERARCHY.md` and `WAVEFORM.md`. I inspected those files, the earlier D and E short-strip calculations, `PAIRING.md`, `NULL-TRANSFORM.md`, and the supplied read-only Work audit waveform file. The locally committed audit file has blob `c4c2adbfc8d5448f29febe3cffa74cc71d2a4101` at `e45da1098bfd4b1ff7e154a166408774dac3d9d0`. The coordinator checked that this blob matches the GitHub contents listing at the Goal’s audit pin `75cb3ff3feef98164a65cf14105be6df77ea809c`. The two known short-width signs, their proposed early interval, the optical coefficient and the nonzero pole enclosure were already disclosed. This is a non-blind reconstruction with a new causal baseline bound and independent Arb quadrature. The script imports no project modules. [Pinned source](https://github.com/inaeyk/GL-with-AI/commit/eb439228601a649c658ce719b503ff0bdc37a83f), [pinned Work waveform file](https://github.com/inaeyk/GL-with-AI/blob/75cb3ff3feef98164a65cf14105be6df77ea809c/research/sav/verification/GLR-GOAL-011/WAVEFORM.md).

Use the pinned real unit-normalized $Y_{2m}$, signature $(-++++)$, $L/r_0=10$, $k r_0=\pi/5$, and the real standing growing GL mode with $H_{\theta\theta}(2r_0)/(8r_0^2)=1$. The expansion uses ordinary coefficients with axial order linear in $\eta$. Fix the ADM mass, Bondi frame, retarded origin and canonical data in [FINITE-DATA](../../GLR-GOAL-011/FINITE-DATA.md), including its induced higher-order graph derivatives. All coordinates and scalar shear below are in units of $r_0$ unless a physical subscript is shown. Put $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $f=1-1/r$, and $u_*=R-2x(R)$ for $R=13/8$. The bump is $b(y)=c_b\exp[-1/(y(1-y))]$ inside $(0,1)$, flat and zero outside, with $\int (b')^2dy=1$. Use $y=(u-u_*)/\delta$, $a_\delta(u)=\sqrt{\delta}\,b(y)$ and $\delta=3/25$. The two intervals are
\[
 I_{[0,1]}=[u_*,u_*+0.12],\qquad I_{[0,1/8]}=[u_*,u_*+0.015],
 \qquad u_*=-0.68499274150852889\ldots .
\]
The endpoints in the fixed clock are approximately $-0.5649927415$ and $-0.6699927415$, respectively. Here the origin $u=0$ is the reference for the reported residue, while the auxiliary lower bound called $u_0$ in `NULL-TRANSFORM.md` is chosen strictly before support. Moving the first cut to zero would rescale a residue quoted in the old clock.

## Shear and interference normalization

On the unit sphere $X_A=-\varepsilon_A{}^B D_BY$ and $X_{AB}=D_{(A}X_{B)}$. Commuting derivatives with the unit sphere Ricci tensor and using $D^A X_A=0$ gives $D^B X_{AB}=-(\lambda/2)X_A$, with $\lambda=(\ell-1)(\ell+2)=4$. Integration by parts gives $\int |X|^2=\ell(\ell+1)=6$ and $\int |X_{AB}|^2=(\lambda/2)6=12$. In the Einstein-frame Bondi gauge $h_{rA}=0$ and $C_{AB}=C X_{AB}$. The leading angular constraint $h_{uA}=\tfrac12 D^B C_{AB}+O(r^{-1})$ gives $h_{uA}=-(\lambda/4)C X_A+O(r^{-1})$. The gradient in the axial invariant potential $A$ cancels in $F=dA$. Thus $F_{ur}=-\partial_r(h_u/r^2)=-\lambda C/(2r^3)+O(r^{-4})$. With the pinned orientation $dt\wedge dr\wedge dz$, $Q_z=-r^4 F_{tr}$ in the product limit, and $F_{tr}=F_{ur}$. The shear is $\lim 2Q_z/(\lambda r)$, and the news is its $u$ derivative on finite cuts. The radion cancellation in the mean curl and the vanishing nonzero-mode extraction products on these cuts use [HIERARCHY (4)–(5)](../../GLR-GOAL-011/HIERARCHY.md) and the inherited right-end estimates. The orthonormal radiative strain is $C_{\rm phys}X_{\hat A\hat B}/r_{\rm phys}$. [Bondi angular constraint, Mädler–Winicour v3 (53)](https://arxiv.org/pdf/1609.01731v3), [odd master and radiative metric, Martel–Poisson v1 (5.13), (6.11)–(6.12), and averaged flux (6.16)](https://arxiv.org/pdf/gr-qc/0502028v1).

With $N_{AB}=\partial_{u_{\rm phys}}C_{AB,{\rm phys}}$ the instantaneous tensor power is $\int N_{AB}N^{AB}d\Omega/(32\pi G_4)$, where $G_4=G_5/L$. This equals $12(\dot C_{\rm phys})^2/(32\pi G_4)$. The tensor mass-aspect equation has gravitational term $-N_{AB}N^{AB}/8$, with the angular divergence integrating to zero, and conversion of the geometric mass to energy contributes $1/G_4$. The corresponding term in Mädler–Winicour v3 (58) uses half of this tensor news. [Flanagan–Nichols v5 (2.4), (2.11a)](https://arxiv.org/pdf/1510.03386v5), [Mädler–Winicour v3 (58)](https://arxiv.org/pdf/1609.01731v3).

Using $r^3F_{tr}$ as the shear multiplies the power by four. The pinned half-period symmetry eliminates the mean $\epsilon\eta$ news. Expanding the remaining $\eta N_{01}+\epsilon^2\eta N_{21}+\cdots$ gives the coefficient
\[
 E_{22,T}(J)=\frac{12r_0}{16\pi G_4}P(J),\qquad
 P(J)=\int_{I_J}N_{01}(u)N_{21}(u)\,du.
\]
This is an interference coefficient in the tensor power. The full tensor power is nonnegative. The radion gives a separate Einstein-frame scalar contribution to the mass loss, and massive KK transport refers to timelike surfaces. The comparison here uses ordered coefficients with $\delta$ fixed before the seed limit. A real order-$\epsilon^2$ clock shift $C_{21}\mapsto C_{21}+c C_{01}'$ changes $P$ at fixed coordinate endpoints by $c[N_{01}^2]_{\partial I_J}/2$. The fixed physical cuts and matched clock therefore matter to the finite-width sign.

## Exact Regge–Wheeler baseline on the strip

In the product limit the canonical pair gives $w|_\Sigma=a_\delta$, $w_v|_\Sigma=0$ for the shear-normalized master $w=\Psi_0$. Its equation is
\[
 w_{uv}=-D(u,v)w,\qquad D(u,v)=\tfrac14 V_{\rm RW}(r((v-u)/2)),
 \qquad V_{\rm RW}=f(6/r^2-3/r^3)\ge0.
\]
The right of the front $u=u_*$ is causally sourced by the prescribed graph alone. On each ray $v\ge v_\Sigma(u)=r_\Sigma(u)$, where $r_\Sigma$ inverts $r-2x(r)$, integrating in $v$ gives
\[
 w_u(u,v)=a_\delta'(u)-\int_{v_\Sigma(u)}^v D(u,V)w(u,V)dV.
\]
At fixed $v$, the lower endpoint for the $u$ integral is $u_*$ or $v_\Sigma^{-1}(v)$, whichever lies in the domain first. At the latter $w=a_\delta$ and at the front both fields vanish. Therefore $w=a_\delta-\mathcal T w$, where $\mathcal T$ integrates the displayed $D w$ integral in $u$, assigning zero to its integrand when $v<v_\Sigma(u)$. The ray mass is explicit since $dv=2dr/f$,
\[
 J(u)=\int_{v_\Sigma(u)}^\infty D(u,v)dv
 =\frac{3}{r_\Sigma(u)}-\frac{3}{4r_\Sigma(u)^2}.
\]
On the full strip $r_\Sigma\in(1.59,1.625]$, $J<M=1.61$, and $0<J'(u)<L_J=0.30$ because
\[
 J'(u)=\frac{3(2r_\Sigma-1)(r_\Sigma-1)}{2r_\Sigma^3(r_\Sigma+1)}.
\]
The Volterra operator has iterates bounded by $(M(u-u_*))^n/n!$ in sup norm. In particular the Neumann series gives the unique bounded solution on the full strip and, bounding each integrated bump by a simplex integral,
\[
 \frac{|w-a_\delta|}{\sqrt{\delta}}\le
 \delta M e^{\delta M y}\int_0^y b(s)\,ds.
\]
This upper bound follows by summing $(M\delta)^n \int_0^y (y-s)^{n-1}b(s)ds/(n-1)!$ for $n\ge1$. The upper radial endpoint can be sent to infinity in $w_u$ by the ray mass bound. The limit in $w$ follows from integrating $w_u$ along $u$ and dominated convergence. The result is the exact causal differentiated trace $N_{01}=a_\delta'-\int D w\,dv$ on the two cuts. This proof retains the tilted launch and the backscattered baseline.

Scale $g(y)=\sqrt{\delta}\,N_{01}(u_*+\delta y)$. A frozen-ray first iterate gives $g_1=b'-\delta J(u_*)b$ and the explicit pointwise remainder
\[
 |g-g_1|\le E(y)=\delta^2\left[yL_J b(y)+M^2 e^{\delta My}\int_0^y b(s)ds\right]. \tag{1}
\]
The first term bounds the variation of the ray mass. The second uses the bound on $w-a_\delta$. On the early interval $b$ is nonnegative and increasing, so $|w-a_\delta|/\sqrt\delta\le\delta M y e^{\delta My} b(y)<b(y)$ for $0<y\le1/8$. Hence $w>0$ and $w=a_\delta-\mathcal T w\le a_\delta$ there. The exact $g$ then obeys $b'-\delta M b\le g\le b'$. Since $b'/b=(1-2y)/[y(1-y)]^2\ge (3/4)(64/7)^2$ there, $g$ is strictly positive. The free limit $V_{\rm RW}=0$ gives $g=b'$ and saturates zero baseline error. Also $\int_0^1 b b'''dy=0$ by flatness, so the frozen-ray first-iterate correction to the full optical projection cancels exactly.

## Finite-width thresholds

The first outgoing eikonal perturbation is $-\chi\cos(kz)$ with $\partial_v\chi=e^{\Omega t}H_{LL}/(4f)$ and zero graph value, from $\delta b^{uu}=-e^{\Omega t}H_{LL}\cos(kz)/f^2$. Squaring the delay yields the Taylor factor $1/2$ and compact factor $\langle\cos^2(kz)\rangle=1/2$. Thus the frozen leading coefficient is $K=\chi_\infty(u_*)^2/4\in(0.257,0.304)$ from the pinned optical enclosure, and the leading returning news is $K\delta^{-5/2}b'''$. For $p=y(1-y)$ and $q=1-2y$, $b'''=b q(1-10p+24p^2-12p^3)/p^6$. The cubic is decreasing up to $7/64$ and positive there, so $b'''>0$ on the open early interval. On the full interval $\int_0^1 b'b'''=-\int_0^1 (b'')^2<0$ by integration by parts. These are leading signs as $\delta$ decreases. Using the exact $g$ with this optical reference makes the following comparison at $\delta=0.12$.

Define the unknown scaled returning residual $\mathfrak e=\delta^{5/2}N_{21}-K b'''$. The decomposition
\[
 P(J)=\delta^{-2}\left(\langle g,K b'''\rangle_J+\langle g,\mathfrak e\rangle_J\right)
\]
is exact. The adverse projected correction needed to reach zero is $-\langle g,K b'''\rangle_J$. The minimum unrestricted $L^2(J,dy)$ norm of such a correction is $T_2=|\langle g,K b'''\rangle_J|/\|g\|_{2,J}$ by Cauchy–Schwarz, attained by a multiple of $g$. The sharp $L^\infty$ threshold is $T_\infty=|\langle g,K b'''\rangle_J|/\|g\|_{1,J}$, with extremizer proportional to $\operatorname{sgn} g$ or smooth approximants. These statements concern the functional on arbitrary residuals. They impose no waveform shape on the physical correction. In the short-width error convention $|N_{21}-K a_\delta'''|\le B_2\sqrt{\delta}/\delta^2$, one has $\|\mathfrak e\|_\infty\le\delta B_2$.

The directed replay encloses the following values with outward rounding. The interval for the sharp threshold reflects the allowed $K$ and exact baseline uncertainty. A strict residual norm below the tabulated lower endpoint suffices for the corresponding physical sign.

| Quantity | Full $J=[0,1]$ | Early $J=[0,1/8]$ |
| --- | ---: | ---: |
| Pure bump projection $\int_J b'b'''dy$ | $[-65.452,-64.992]$ | $[0.36719,0.37213]$ |
| Exact baseline projection $\int_J g b'''dy$ | $[-65.893,-64.552]$ | $[0.36627,0.37121]$ |
| Leading $P$ with unscattered baseline | $[-1382,-1159]$ | $[6.54,7.86]$ |
| Optical $P$ with exact baseline | $[-1392,-1149]$ | $[6.52,7.84]$ |
| Sharp threshold $T_2$ for $\mathfrak e$ | $[16.45,20.14]$ | $[6.88,8.31]$ |
| Sharp threshold $T_\infty$ for $\mathfrak e$ | $[21.01,25.73]$ | $[41.11,49.61]$ |
| Sharp sup threshold $T_\infty/\delta$ for $B_2$ | $[175.14,214.37]$ | $[342.62,413.37]$ |

The exact-baseline reference is enclosed without sampling $w$ or cutting off the outer ray. The physical $\mathfrak e$ includes the full mixed insertion, constrained second-order source, finite-graph derivatives and all residual transport including the variation of $\chi_\infty$ across the packet. A bound on its signed projection is enough. No such bound follows from the pinned local pole enclosure.

## Negative control and remaining gate

A smooth causal shear addition $\Delta C_{21}=-2K(\delta/\delta_0)a_\delta''$ supported on the original packet interval, with fixed $\delta_0=0.12$, lies within both inherited short-width remainder orders. At $\delta_0$ it flips both exact-baseline optical signs when added to the optical reference. Its Laplace transform is entire. The same compact addition, with an arbitrary real factor in place of $-2$, can move the two signed projections of an actual trace while leaving its continued pole residue unchanged. In particular the limiting signs for large positive and negative factors are opposite. This is a counterexample to inferring finite-width flux signs from the residue and the scale orders alone, within manufactured traces. It changes the waveform and does not purport to solve the fixed-canonical physical equations.

The normalization, baseline Volterra bounds and sharp threshold formulas are analytic consequences of the pinned ordered model. The table is a directed finite-width enclosure for the optical projection against the physical baseline, conditional on the inherited $K$ box. The leading physical short-width expansion is inherited. The physical signs at $0.12$ remain unresolved until the returning residual or its signed projections are enclosed. No finite-amplitude Bondi remainder estimate enters this result.

## Reproduction

From `research/sav`, run `PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-012/checks/d_flux.py`. The saved [output](../checks/d_flux.txt) uses Python 3.14.8, python-flint 0.9.0, 128-bit Arb and 32768 uniform cells. The script works on the raw bump and normalizes by the enclosed derivative-square integral. A cell enclosure of $b$ also bounds the cumulative integral in (1). The two flat endpoint cells use the analytic bound $|\partial_y^j b_{\rm raw}|\le 6t^6e^{-t}$ for $j\le3$, $t=[y(1-y)]^{-1}\ge32768$, decreasing on this tail. The exact-baseline projection is enclosed by a signed interval sum for $g_1 b'''$ plus $\int E|b'''|$. The norm enclosures use the reverse triangle inequality and $\|E\|_p$. The initial interval implementation divided by raw powers of the interval $p$ and produced `nan` near the flat ends because those power enclosures included zero. Forming the positive reciprocal first and then taking its powers resolves the artificial singularity. All calculations are non-production.
