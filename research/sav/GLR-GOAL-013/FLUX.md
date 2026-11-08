# Leading tensor news and finite-cut interference

The fixed canonical axial quadrupole has a certified leading-news approximation at null infinity on both packet cuts. The physical mixed-order interference signs require the corresponding returning-news projections. The optical projections below quantify the required accuracy and retain the inherited interval for the delay coefficient.

## Fixed preparation and exact baseline

Use the benchmark in [GOAL](GOAL.md) and the pinned [canonical prescription (1)](https://github.com/inaeyk/GL-with-AI/blob/87fd711bf2e8117467caad1ff52c1cd344966974/research/sav/GLR-GOAL-011/FINITE-DATA.md). In units $r_0=1$, set $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, $f=1-1/r$, $u_\Sigma(r)=r-2x(r)$, and $v_\Sigma(u)=r_\Sigma(u)=u_\Sigma^{-1}(u)$. Here $v_\Sigma$ is strictly decreasing. Write $R=13/8$, $a=u_*=u_\Sigma(R)$, $\delta=3/25$, and $y=(u-a)/\delta$. Let $b_0(y)=\exp[-1/(y(1-y))]$ inside $(0,1)$, flat and zero outside, and $b=b_0/(\int_0^1 (b_0')^2)^{1/2}$. The shear-normalized baseline $w$ obeys
$$
 w_{uv}=-D(u,v)w,\qquad D(u,v)=\tfrac14 V(r((v-u)/2)),\qquad V=3f(2r-1)/r^3,
$$
with $w|_\Sigma=\sqrt\delta\,b(y)$, $w_v|_\Sigma=0$. The trace vanishes before $a$ by causality. On each ray define
$$
 A_y F=\int_{v_\Sigma(a+\delta y)}^\infty D(a+\delta y,v)F(y,v)\,dv,
 \qquad J(y)=A_y1=\frac3{r_\Sigma}-\frac3{4r_\Sigma^2}.
$$
The identity for $J$ follows by $dv=2\,dr/f$. The independent numeric calculation verifies $r_\Sigma(a+\delta)>1.59$ and $J(y)<M=1.61$ on the full strip by monotonicity and the identity $r_\Sigma(u)=1+2W_0(e^{-(1+u)/2}/2)$, where $W_0$ is the principal Lambert function.

Integrate in $v$ first. With $W=w/\sqrt\delta$, the result at finite $v$ is
$$
 \sqrt\delta\,w_u(u,v)=b'(y)-\delta\int_{v_\Sigma(u)}^v D(u,V') W(y,V')\,dV'.
$$
At fixed $v$, the lower limit of integration in $u$ is either the causal front or the intersection with the initial graph. The latter gives the prescribed value of $w$ there. Thus $W=b-\mathcal T W$, with
$$
 (\mathcal T F)(y,v)=\delta\int_0^y\!dz\int_{v_\Sigma(a+\delta z)}^v
 D(a+\delta z,V')F(z,V')\,dV' .
$$
The inner integral is defined as zero when $v<v_\Sigma(a+\delta z)$. This uses the tilted graph exactly. Successive applications satisfy $|\mathcal T^n F|(y,v)\le(\delta M)^n\int_0^y (y-z)^{n-1}\sup_{V'}|F(z,V')|\,dz/(n-1)!$. Uniform convergence gives a bounded solution, and the same bound on powers gives uniqueness. Differentiating the integrated equation and taking $v\to\infty$ under the ray-mass bound gives the exact leading Bondi news
$$
 g(y):=\sqrt\delta\,N_{01}(a+\delta y)=b'(y)-\delta A_y W .
$$
The $L^2(dy)$ norm for $g$ equals the $L^2(du)$ norm for $N_{01}$ on the corresponding cuts. All these norms are dimensionless. In physical time the news itself has the same normalization, and its $L^2(du_{\rm phys})$ norm acquires a factor $\sqrt{r_0}$.

## A certified two-scatter approximation

Use $W=b-\mathcal T b+\sum_{j\ge2}(-\mathcal T)^j b$ in the exact trace. The second-scatter term in $g$ is
$$
 \delta A_y \mathcal T b=\delta^2\int_0^y b(z)K(y,z)\,dz,
 \qquad K(y,z)=\int_{v_\Sigma(a+\delta z)}^\infty
 D(a+\delta y,v)\int_{v_\Sigma(a+\delta z)}^v D(a+\delta z,V')\,dV'\,dv.
$$
The lower limit is the earlier ray's graph point since $v_\Sigma$ decreases. The diagonal kernel is $K(z,z)=J(z)^2/2$, the integral of $FF'$ for the cumulative ray potential $F$. For $r\ge1.59$,
$$
 \partial_x\log V=\frac1{r^2}-\frac{f(4r-3)}{r(2r-1)},\qquad
 0\le\frac{f(4r-3)}{r(2r-1)}\le\frac{2(r-1)}{r^2}\le\frac12 .
$$
The positive term obeys $1/r^2<1/2$ as well, so $|\partial_u\log D|\le1/4$ at fixed $v$. Along the segments in $K(y,z)$ the minimum radius is at or above $r_\Sigma(a+\delta y)$, which justifies this domain. Consequently
$$
 |K(y,z)-K(z,z)|\le\big(e^{\delta(y-z)/4}-1\big)\frac{J(z)^2}{2}.
$$
Summing powers of $\mathcal T$ from order two in $W$ gives
$$
 \left|g-b'+\delta Jb-\frac{\delta^2}{2}\int_0^y J(z)^2 b(z)\,dz\right|
 \le \delta^3 M^2\left(\frac{e^{\delta y/4}}8+M e^{\delta M y}\right)
       \int_0^y(y-z)b(z)\,dz . \tag{1}
$$
In the first term of (1), use $e^h-1\le he^h$. For the higher scatter terms, use $A_y1\le M$ and
$\sum_{j\ge2}(\delta M)^j (y-z)^{j-1}/(j-1)!\le(\delta M)^2(y-z)e^{\delta M y}$.

For a deterministic approximant on the dyadic mesh of spacing $h$, set $c_j$ to the exact midpoint of the Arb ball for $J^2b_0$ at the midpoint of cell $j$. Define $C_h(y)=\int_0^y \sum_j c_j\mathbf 1_{[jh,(j+1)h)}(z)\,dz$, $B=(\int (b_0')^2)^{1/2}$, and
$$
 g_h=b'-\delta Jb+\frac{\delta^2}{2B}C_h,\qquad
 n_0(u)=\delta^{-1/2}g_h((u-a)/\delta).
$$
The script bounds the quadrature error of $C_h$ by summing $h\,\sup_{\text{cell }j}|J^2b_0-c_j|$, including the current cell. The integrand ranges use monotonic ray masses and whole-cell bump jets. Accumulation of upper cell bounds for $b_0$ gives upper enclosures of the area and first Volterra moment in (1). Midpoint quadrature for $(b_0')^2$ and $(b_0'')^2$ uses a Taylor remainder $h^3\sup|F''|/24$ on each whole cell. On the two flat end cells use $|b_0^{(m)}|\le512t^8 e^{-t}$ for $0\le m\le4$ and $t=[y(1-y)]^{-1}\ge h^{-1}$. To check this bound, the four derivatives of $\log b_0$ are bounded respectively by $t^2,4t^3,18t^4,120t^5$. The fourth derivative of $b_0$ divided by $b_0$ is bounded by $(120+72+48+24+1)t^8$, with the lower orders smaller. The majorant decreases for $t\ge8$. Flat endpoint limits use the analytic extension.

The directed calculations give the following outward-rounded bounds. They retain all baseline propagation and require no finite-radius extraction.

| Quantity | Full packet $I_{\rm packet}$ | Early cut $I_{\rm early}$ |
| --- | ---: | ---: |
| $\lVert N_{01}-n_0\rVert_{L^2(du)}$, upper bound | $2.831\times10^{-4}$ | $2.491\times10^{-10}$ |
| $\lVert N_{01}\rVert_{L^2(du)}$ | $[0.99911,1.00093]$ | $[0.01359838,0.01364715]$ |
| Optical overlap against exact $N_{01}$ | $[-1377.03,-1163.36]$ | $[6.5807,7.7846]$ |
| Sufficient strict $L^2(dy)$ tolerance of scaled returning error | $16.737$ | $6.9438$ |
| Sufficient strict $L^\infty$ tolerance of scaled returning error | $21.385$ | $41.467$ |
| Physical sign | unresolved | unresolved |

The optical rows use the inherited $K_*\in[0.257,0.304]$ and the reference $K_*\delta^{-5/2}b'''(y)$. The second-scatter baseline derivation is independent of that coefficient. To evaluate the optical projection, use $\int b'b'''=[b'b'']-\int (b'')^2$ for the direct term, whole-cell signed integration for the scattering correction in $g_h$, and the pointwise error bound integrated against $|b'''|$. For $\mathfrak e=\delta^{5/2}N_{21}-K_*b'''$, the physical overlap is exactly
$$
 \mathcal P=\delta^{-2}\big(K_*\langle g,b'''\rangle+\langle g,\mathfrak e\rangle\big).
$$
Cauchy–Schwarz and the $L^1$–$L^\infty$ pairing give the table's sufficient tolerances. Direct signed projection control can succeed with a larger error orthogonal to $g$. A physical candidate $n_2$ with error $E_2$, together with $n_0$ of error $E_0$ on the same cuts, gives
$$
 \left|\mathcal P-\langle n_0,n_2\rangle\right|
 \le E_0\|n_2\|_2+E_2\|n_0\|_2+E_0E_2 .
$$
Any uncertainty from computing $\langle n_0,n_2\rangle$ belongs in addition to this cross-error.

### Direct projection gate for a sourced return

Write $4w_{uv}+Vw=-S$ for the return with zero exact-curl graph displacement and graph time derivative $j_\Sigma=w_t|_\Sigma$. For $p=T'=f-1$, tangency gives $w_x|_\Sigma=-p j_\Sigma$ and $w_u|_\Sigma=f j_\Sigma/2$. Define $H(u,v)=f j_\Sigma/2-\tfrac14\int_{v_\Sigma(u)}^v S(u,V')dV'$. The corresponding forced Volterra equation has $w_u=H-\int D w$. For an error between two sources and graph jets with the same zero displacement, take $h(u)\ge\sup_{v\ge v_\Sigma(u)}|\Delta H(u,v)|$. On either packet cut $[a,d]$, the previous Volterra powers give
$$
 |\Delta N_{21}(u)|\le h(u)+M\int_a^u e^{M(u-z)}h(z)\,dz,
$$
and hence
$$
 |\Delta\mathcal P|\le \int_a^d h(z)\left[|N_{01}(z)|+M\int_z^d e^{M(u-z)}|N_{01}(u)|\,du\right]dz.
$$
The weight follows by Fubini. This inequality targets the cut observable and includes forcing at every $v$ through the bound on $H$. It applies to smooth candidates with the stated displacement and integrable radial residuals, or to piecewise candidates once their jump contributions to $H$ are accounted for. Its physical $h$ has yet to be evaluated.

## Tensor normalization and cut dependence

Let $Y$ be real and unit normalized on the unit sphere, $X_A=-\varepsilon_A{}^B D_B Y$, $X_{AB}=D_{(A}X_{B)}$, and $L_\ell=\ell(\ell+1)$. Then $\int X^2=L_\ell$, $D^A X_A=0$, and $-D^2X_A=(L_\ell-1)X_A$ by the scalar harmonic equation and the unit sphere Ricci curvature. Directly expanding the symmetrized gradient and integrating by parts gives
$$
 \int X_{AB}X^{AB}=\tfrac12\left(\int |DX|^2+\int D_A X_B D^B X^A\right)
 =\tfrac12\big((L_\ell-1)L_\ell-L_\ell\big)=12
$$
for $\ell=2$. The second integral is $-\int \operatorname{Ric}(X,X)$. In the Bondi angular gauge the shear coefficient $C_{AB}=C X_{AB}$ gives $h_{uA}=\tfrac12 D^B C_{AB}+O(r^{-1})=-\lambda C X_A/4+O(r^{-1})$, with $\lambda=L_\ell-2=4$. The curl of the axial potential gives $F_{ur}=-\partial_r(h_u/r^2)=-\lambda C/(2r^3)+O(r^{-4})$. The pinned orientation has $Q_z=-r^4F_{tr}$ in the product limit and $F_{tr}=F_{ur}$, hence the shear is $\lim 2Q_z/(\lambda r)$. The mean-frame and extraction corrections use the pinned [Einstein-frame hierarchy (4)–(5)](https://github.com/inaeyk/GL-with-AI/blob/87fd711bf2e8117467caad1ff52c1cd344966974/research/sav/GLR-GOAL-011/HIERARCHY.md). [Bondi angular constraint, Mädler–Winicour (53)](https://arxiv.org/pdf/1609.01731v3) · [Axial master, radiative metric and flux, Martel–Poisson (5.13), (6.11)–(6.12), (6.16)](https://arxiv.org/pdf/gr-qc/0502028v1).

The tensor contribution to the Einstein-frame Bondi power is $\int N_{AB}N^{AB}/(32\pi G_4)$ with $G_4=G_5/L$, $N_{AB}=\partial_{u_{\rm phys}}C_{AB,\rm phys}$, and $C_{\rm phys}=r_0 C$. The angle divergence integrates out of the mass-aspect equation. The term linear in $\eta$ is the nonspherical axial seed response. Translating the compact coordinate by half a period changes the sign of $\epsilon$ while preserving the zero-mode observable, so the next seeded zero-mode term is $\epsilon^2\eta N_{21}$. Expanding the square and using $du_{\rm phys}=r_0\,du$ gives the coefficient of $\epsilon^2\eta^2$ in the integrated tensor energy,
$$
 E_{22,T}(I)=\frac{12r_0}{16\pi G_4}\,\mathcal P(I)=\frac{3r_0}{4\pi G_4}\,\mathcal P(I).
$$
The tensor interference coefficient is signed and the total tensor energy is nonnegative. Radion null flux and KK energy through timelike surfaces are separate observables. Strict spherical symmetry removes the seeded axial tensor channel. [Bondi mass-aspect equation, Flanagan–Nichols (2.11a)](https://arxiv.org/pdf/1510.03386v5).

A common absolute time translation simply relabels both traces and both endpoints. With an infinitesimal order-$\epsilon^2$ relabeling $u_{\rm old}=u_{\rm new}+\epsilon^2 c$, the new ordered correction at fixed coordinate labels is $N_{21}+cN_{01}'$. Its overlap changes by $c[N_{01}^2]_{\partial I}/2$. Pulling back both endpoints shifts the leading energy by the opposite amount at the same perturbative order. An angle-dependent supertranslation can change the single-mode projection on constant-coordinate cuts. The whole tensor flux on the same physical cuts is compared using the pulled-back cuts generator by generator. An actively different cut selects a different integration region.

## Reproduction

Run from `research/sav` in the pinned-base GOAL-013 branch.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-013/checks/d_leading_news.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-013/checks/d_leading_news.py --mesh-power 14 --precision 128
```

Python 3.14.8 and python-flint 0.9.0 produced the [primary 160-bit output](checks/d_leading_news.txt) and [128-bit mesh variant](checks/d_leading_news_variant.txt). The variant gives independent valid enclosures for its own candidate with wider quadrature budgets. Both use the same analytic proof and implementation. The [investigator record](investigators/D-OBSERVABLES.md) gives the template-norm check and retained adverse cases.
