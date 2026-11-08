# Independent scientific audit of GLR-GOAL-011

| Item | Pinned identity |
| --- | --- |
| Original Codex draft | [inaeyk/GL-with-AI PR #13](https://github.com/inaeyk/GL-with-AI/pull/13) |
| Audited source | [eb439228601a649c658ce719b503ff0bdc37a83f](https://github.com/inaeyk/GL-with-AI/commit/eb439228601a649c658ce719b503ff0bdc37a83f) on codex/glr-goal-011-qnm-residue |
| GOAL-007 stack base | [0864d9d807de56cbdd6e64d215e6295508a3007f](https://github.com/inaeyk/GL-with-AI/commit/0864d9d807de56cbdd6e64d215e6295508a3007f) |
| Previous independent comparison | [GOAL-007 review PR #8 at 46ec0208d3efdd0595813e8e2719e2319b5ee67c](https://github.com/inaeyk/GL-with-AI/commit/46ec0208d3efdd0595813e8e2719e2319b5ee67c) |

## Scientific scope and status

The audit supports **Outcome A for the ordered perturbative response**. The signed interval calculation reproduced the source rectangle
$$
 A_{21,q}\in[0.08223,0.18771]+i[-0.42418,-0.31707].
$$
The result applies to the local fundamental pole of the continued causal null trace. The independent calculations, reproduction and stated qualifications below found no blocking discrepancy for the pinned preparation.

The experiment uses the normalized growing vacuum GL tangent at $L/r_0=10$, the completed graph $\tau=v-r=0$, a real unit-normalized axial quadrupole, and the fixed-canonical packet launched at $R/r_0=13/8$, $\delta=0.12$. The observed quantity is the Einstein-frame Bondi shear at order $\epsilon^2\eta$, with coefficient in units $r_0$ and retarded-time origin zero. The fixed data and retarded boundary conditions determine the coefficient. The GOAL-005 GL bracket and regular decaying mode are inherited inputs from the pinned scientific stack.

| Claim | Audit evidence |
| --- | --- |
| 5D reduction, mean and data | Independent warped-product derivation, constrained graph and electric-density calculation, manufactured quadratic Ricci comparison, and two-field massive reduction. [Geometry](GEOMETRY.md) |
| Causal Laplace exchange, continuation and pole | Independent characteristic estimates, fixed-ray continuation with horizon subtraction, separate continued fraction and Jost checks. [Continuation](CONTINUATION.md) |
| Signed residue enclosure | Pinned interval replays and independently reconstructed directed bounds, matching identity and quotient arithmetic. [Arithmetic](ARITHMETIC.md) · [Reproductions](REPRODUCTION.md) |
| Strong-form estimate | The pinned degree-42 strong integration evaluates a separate pairing on the same fields as the weak numerical calculation. The replay gives $0.13523378825882212-0.3706200776960893i$. The estimate is empirical. |
| Common clock | The real clock direction is $s_q A_{01,q}$. The pinned interval replay gives the signed perpendicular component in $(-0.44590,-0.33745)$. A separate direct solve yields $-0.391646838$ empirically. |
| Prompt waveform and flux | Independent characteristic, shear and flux derivations. The optical coefficient enclosure at $13/8$ is $(0.257,0.304)$. The signs are asymptotic statements for sufficiently small width. [Waveform](WAVEFORM.md) |

This was a separate, non-blind audit. The source pin, expected values and original methods were available to the reviewers. Independent derivations and newly written checks are identified separately from source replays and inherited premises. The external audit gives scientific evidence for human review.

## Returning pole and normalization

Use $r_0=1$, $x=r+\log(r-1)$, and the convention
$$
  h\sim e^{sx}\quad(x\to-\infty),\qquad
  g_+\sim e^{-sx}\quad(x\to+\infty),\qquad
  W=hg_{+,x}-h_xg_+.
$$
The contour fixes the outgoing germ after continuation. The Green kernel $h(x_<)g_+(x_>)/W$ has derivative jump $+1$ for $\partial_x^2-s^2-V$. Write $D=h_x/h-g_{+,x}/g_+$. On the disk where both logarithmic derivatives exist, $W=-hg_+D$ and hence $W'(s_q)=-h(r_m)g_+(r_m)D'(s_q)$. The product formula for the derivative uses $D(s_q)=0$.

The Riccati enclosure first proves that both normalized solutions remain invertible at the matching point across the larger disk. A Cauchy bound $M$ on radius $R_c=0.005$ gives $|d_n|\le M/R_c^n$ for the coefficients of $D(s_c+z)$. Subtracting the linear comparator and summing the geometric tail controls the Rouché error. The replay gives an error upper bound $1.30921\times10^{-8}$ versus comparator modulus $1.69729\times10^{-7}$. It counts one simple zero within $5\times10^{-8}$ of $-0.177924631-0.747343369i$. The shifted massless and massive tests respectively give $\Im D(s-2\Omega)>0.28$ and $\Im\det D_k(s-\Omega)>0.40$ on that disk with the full pinned growth bracket.

The centered five-point difference for $D'$ cancels the cubic term. If $t=h_{\rm step}/R_c$, the remaining odd terms are bounded by
$$
 \frac{M}{R_c}\left[\frac{4t^4}{3(1-t^2)}+
                        \frac{(2t)^4}{3(1-4t^2)}\right].
$$
The derivative's variation over the root disk of radius $d$ adds at most $(M/R_c)[(1-d/R_c)^{-2}-1]$. Both use bounds on analytic functions on the larger disk rather than a difference of approximate root locations.

For the normalization, let $g_+=e^{-sx}v$ and $v_a=1+3/(sr)$. Direct substitution into $v_{xx}-2sv_x-Vv=0$ yields the residual $f(3-12/s)/r^3$. On the outward ray write $\gamma=dx/d\rho$, $|\gamma|\le B_\gamma$, and $\Re(s\gamma)\ge b>0$ beyond $R=144$. If $|v-v_a|\le B/\rho^2$, $|V|\le V_0/\rho^2$ and the residual is at most $C/\rho^3$, integrating the first-order equation for $(v-v_a)_x$ backward gives
$$
 |(v-v_a)_x|\le
 \frac{B_\gamma}{2b}(C+V_0 B/R)\rho^{-3},\qquad
 |v-v_a|\le
 \frac{B_\gamma^2}{4b}(C+V_0 B/R)\rho^{-2}.
$$
Thus the contraction constant in the weighted amplitude norm is $B_\gamma^2 V_0/(4bR)<0.02687$. The bounded logarithmic derivative integrated from $R$ to $r_m$ preserves the unit amplitude after accounting for $v(R)$. The unit horizon amplitude comes from its regular Frobenius solution. The refined interval denominator is contained in
$$
 [7.81873470886,7.85596024942]+i[-1.73024927337,-1.69302372704].
$$
The broader Rouché replay gives $3.64<|W'(s_q)|<10.57$.

The separate [Jost check](checks/rw_jost_check.py) integrates the two factored amplitude equations directly. If $y=e^{\pm sx}v$, then $f v_{rr}+(f_r\pm2s)v_r-(6/r^2-3/r^3)v=0$. At the horizon it uses the power series of
$$
 (r-1)r^2 v_{rr}+(r+2sr^3)v_r-(6r-3)v=0,\quad v(1)=1.
$$
At infinity the minus equation with a truncated inverse-radius series uses $a_0=1$ and
$$
 2sn a_n=[6-(n-1)n]a_{n-1}+[n(n-2)-3]a_{n-2}.
$$
Varying the ray angle, far cap, match point and frequency difference step gives $s_q\simeq-0.1779246313779-0.7473433688361i$ and $W'(s_q)\simeq7.8364909381-1.7098101933i$. This comparison is empirical. The continued fraction provides a second independent route to the root. Identification with the literature fundamental uses the Schwarzschild benchmark in $2M=1$ units. The disk certificate is a local root count. [Leaver (1985), §2 and Table 1](https://doi.org/10.1098/rspa.1985.0119)

## Quotient and real clock direction

For the fixed graph $T=r-x$, the outgoing baseline position is $a_\delta(T-x)$ and its time derivative is $a_\delta'(T-x)$. The compact graph moment is
$$
 J_h(s)=\int e^{-sT}(1-T')(h_x+s h)a_\delta(T-x)\,dx .
$$
Changing variables to $u=T-x$ reverses the integration orientation and cancels the factor $1-T'$. This gives a quadrature over the unit bump with no differentiation of its endpoints. In the direct Jost calculation, the same moment gives $J_h=0.0032546969784-0.0061081545001i$ and $A_{01,q}=-J_h/W'\simeq-0.00055878952383+0.00065753033047i$.

At order $\epsilon^2$, changing the common retarded-time origin by a real amount adds a real multiple of $s_q A_{01,q}$ to $A_{21,q}$. The sign of the multiple depends on the direction assigned to the coordinate translation. The oriented component is
$$
 {\Im\big(A_{21,q}\overline{s_q A_{01,q}}\big)\over |s_q A_{01,q}|}
 ={\Im(I\overline K)\over |K||W'|},\qquad K=-s_q J_h(s_q).
$$
Both denominators are bounded away from zero. The same $W'$ cancels its phase between the numerator and the baseline clock direction. A complex clock parameter would allow arbitrary cancellation, whereas the physical time shift is real. The fixed mass and Bondi frame specify the comparisons to which this statement applies.

With the published signed numerator rectangle and the outward-rounded denominator above, exact rational corner evaluation independently gives
$$
 A_{21,q}\in[0.08285,0.18746]+i[-0.42383,-0.31759],
$$
inside the advertised rectangle. For $I=x-id$, $W'=a-ic$ all four variables are positive on their respective boxes. Each component of the quotient is affine in $x,d$ at fixed $a,c$. The signs of the real derivatives in $a,c$ are $-,+$, and the imaginary derivatives are $+,+$. In particular $d(a^2-c^2)-2acx$ is positive throughout by a rational lower bound using the box edges. Hence corner evaluation encloses the entire box. This check starts from the replayed input boxes. [Exact rational check](checks/rectangle_check.py)

## Boundaries and corrections

- Outcome A concerns the ordered response at the fixed preparation. Independent upper-order canonical data can change the residue. A pair of localized real data perturbations with complex responses of distinct phase spans the residue plane. A perturbation with later launch support also gives a control that leaves the earlier signal interval intact.
- The nonzero local residue identifies a pole of the continued causal transform. A lower bound on an observable ringdown time window would also require control of the rest of the inverse transform. Finite-time pole dominance remains open.
- The flux claims hold in the short-width limit on the ordered waveform. The dossier supplies no numerical width threshold and no flux sign at $\delta=0.12$ follows from the short-width estimate. The finite-amplitude Bondi expansion and prolonged GL growth remain open.
- The strong-form point shares the computed lower fields, normalized spectral denominator and a near-horizon fit with the weak empirical calculation. It is an independent exterior projection check with these shared inputs.
- Early rows of the source error budget describe older checkpoints. Read the complete signed row and final certificate for the final numerator claim. The arithmetic audit identifies an input-injection hazard outside the fixed default script's domain. [Source error budget](../../GLR-GOAL-011/ERROR-BUDGET.md) · [Arithmetic](ARITHMETIC.md)
- The product-slice metric in FINITE-DATA has comma punctuation in place of tensor-product spacing. The intended components are $b_{\tau\tau}=-f$, $b_{\tau r}=r_0/r$, and $b_{rr}=1+r_0/r$. The constrained derivation uses those components. [Source finite data](../../GLR-GOAL-011/FINITE-DATA.md)

All computations were non-production analytic or bounded radial checks. This review adds a dedicated dossier and reproducible checks. The audited branches and production files were treated as immutable inputs.
