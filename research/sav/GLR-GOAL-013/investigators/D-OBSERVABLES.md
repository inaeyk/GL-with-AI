# Agent D — leading news, flux and finite-window observables

## Scope and independence

Agent D reconstructed the scalar launch-to-Bondi map and independently derived the angular normalization. The source was GOAL-012 at commit `87fd711bf2e8117467caad1ff52c1cd344966974`. I read the fixed canonical pair and its induced data in GOAL-011 FINITE-DATA, the GOAL-012 optical and template records, and the GOAL-012 Work audit windows and quadrature at `ae8c323e1e87abd0fff26eac75b05b701a7e7b1a`. The optical signs, first-scatter estimates, inherited delay and residue boxes were available in advance. The investigation is a non-blind independent derivation of the second-scatter kernel and a new directed implementation without historical code imports. The two mesh runs share the same proof and implementation. The scientific task stopped after establishing the baseline certificate, flux dictionary and QNM test inputs. The physical source evaluation is a separate gate. [Pinned source](https://github.com/inaeyk/GL-with-AI/commit/87fd711bf2e8117467caad1ff52c1cd344966974) · [Pinned Work audit](https://github.com/inaeyk/GL-with-AI/commit/ae8c323e1e87abd0fff26eac75b05b701a7e7b1a).

## Evidence and bounds

- **Analytic and directed**. The two-scatter candidate includes the complete leading-order backscatter up to the evaluated error, with the original graph and without cutting off the ray. The leading-news errors are below $2.831\times10^{-4}$ and $2.491\times10^{-10}$ in dimensionless $L^2(du)$ on the packet and early cuts. [Derivation](../FLUX.md) · [Directed output](../checks/d_leading_news.txt).
- **Analytic and directed, conditional optical reference**. The exact leading waveform projects negatively against the inherited optical return on the packet and positively on the early cut. The required strict tolerances on the scaled return error are $16.737$ and $6.9438$ in $L^2(dy)$. The ordered physical return projections, and therefore the two interference signs, are unresolved. [Flux and direct projection bound](../FLUX.md).
- **Analytic**. The real unit-normalized axial tensor harmonic has squared norm $12$ by direct integrated derivative contraction. In the adopted Einstein-frame Bondi units, the signed tensor energy coefficient is $3r_0\mathcal P/(4\pi G_4)$. The common order-$\epsilon^2$ clock shift acts at fixed labels by an endpoint term. Corresponding physical intervals cancel it with the endpoint displacements of the leading energy. [Normalization and cut transformations](../FLUX.md).
- **Directed template geometry**. Both central-window denominator norms are bounded away from zero with the inherited pole frequency and shear-residue boxes. These calculations do not constrain the physical waveform numerator. [Code](../checks/d_template_norms.py) · [Output](../checks/d_template_norms.txt).

## Template uncertainty and the decision rule

Set $I=[0,8.407362]$. For the pair $q_{s,A}=2\Re(sA e^{su})$, define $J_I(\zeta)=\int_I e^{\zeta u}\,du$, $z=sA$, $w=tB$. Direct multiplication gives the real cross Gram identity
$$
 \langle q_{s,A},q_{t,B}\rangle
 =2\Re\big[z w J_I(s+t)+z\bar w J_I(s+\bar t)\big].
$$
A perturbation in $A$ of complex modulus at most $r_A$ yields an $L^2$ error at fixed $s$ of at most $r_A |s|\sqrt{2[J_I(2\Re s)+|J_I(2s)|]}$. For an additional frequency change of modulus at most $r_s$, use the segment derivative $\partial_s(se^{su})=e^{su}(1+su)$ and $\Re s+r_s<0$. An upper bound for its additional norm error is $2(|A|+r_A)r_s[1+(|s|+r_s)|I|]\sqrt{|I|}$. The source disk is contained in the radius used for the source frequency and the translated radius also includes $2\Omega$ uncertainty. Bounds remain valid for the correlated frequency pair since each bound covers the entire corresponding box.

The directed central-window midpoint inner product of $Q$ and $S_*$ is approximately $0.6192162577$. The midpoint template norms and uncertainty radii are enclosed by the table. The norm lower and upper endpoints use the triangle inequality.

| Template | Midpoint norm | Error radius upper bound | Template norm enclosure |
| --- | ---: | ---: | ---: |
| $Q$ | $[0.73555152,0.73555153]$ | $0.148061$ | $[0.58749,0.88362]$ |
| $Q+S_*$ | $[1.75506016,1.75506018]$ | $0.239806$ | $[1.51525,1.99487]$ |

One can replace the first norm enclosure by the Work audit's tighter bound $[0.59933,0.87548]$. If $\lVert n_{21}-P_{\rm mid}\rVert\in[d_l,d_h]$, $\lVert N_{21}-n_{21}\rVert\le E$, $\lVert P-P_{\rm mid}\rVert\le e_P$, and $\lVert P\rVert\in[L_P,U_P]$, the decision enclosure is
$$
 D(P)\in\left[\frac{\max(0,d_l-E-e_P)}{U_P},\frac{d_h+E+e_P}{L_P}\right].
$$
All functions in the comparison use the fixed physical clock and the same onset at or before the comparison window. Switching the news directly avoids an impulsive derivative of a switched shear. An upper endpoint strictly below $1/2$ passes. A lower endpoint at least $1/2$ rejects. Finite-window data for the two numerators remain unresolved.

## Adverse checks and limits

- In the free limit $D=0$, the ray mass, second-scatter term and all higher scatter terms vanish and the launched derivative is the exact news. The positive second-scatter correction follows from the two negative signs in the differentiated trace and first shear correction.
- A constant positive null potential on a finite slab with characteristic data $w(u,0)=w(0,v)=1$ provides an exact second-scatter sign control. The solution of $w_{uv}=-D w$ is $J_0(2\sqrt{Duv})$ by its power series. At the far edge $v=V$ the first two news terms are $-DV+(DV)^2u/2$. The diagonal kernel reduces to $(DV)^2/2$, confirming the factor and sign.
- The initial directed script produced a failed normalization guard. Arb exponentiation of a signed ball crossing zero can return `nan` even for integer powers through the Python binding. Replacing signed powers by directed products corrected the failure. The retained diagnostic is separate. [Failure record](../checks/d_failures.txt).
- The lower-precision mesh variant gives valid bounds for its own candidate. The full error changes from about $2.8304\times10^{-4}$ to $2.8362\times10^{-4}$ and remains dominated by the analytic kernel approximation and higher-scatter term. These runs evaluate whole cells. Agreement is a consistency check. [Variant output](../checks/d_leading_news_variant.txt).
- Both pole residues allow large changes of either signed overlap under changes of independent canonical upper data. The known causal counterexample varies the preparation and leaves the residues fixed by a later packet. There is no inferred freedom in the fixed preparation and no pole-only physical sign decision. [Work construction](https://github.com/inaeyk/GL-with-AI/blob/ae8c323e1e87abd0fff26eac75b05b701a7e7b1a/research/sav/verification/GLR-GOAL-012/WINDOWS.md).

## Reproduction

Run the following from `research/sav` with Python 3.14.8 and python-flint 0.9.0.

~~~
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-013/checks/d_leading_news.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-013/checks/d_leading_news.py --mesh-power 14 --precision 128
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-013/checks/d_template_norms.py
~~~
