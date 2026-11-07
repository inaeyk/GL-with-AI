# Exact local and manufactured verification

The source baseline is `61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708`.
These implementations use the conventions in [NOTATION.md](../NOTATION.md).
They use neither the independent audit commit
`a9bfbec0efe41b881e4b12aa262f5fcfcd94c639` nor a sampled GL eigenfunction as
computational input. Run from `research/sav/` with Python 3.14.8 and SymPy 1.14.0.

```sh
python3 GLR-GOAL-007/checks/curvature.py
python3 GLR-GOAL-007/checks/radiation.py
```

The saved [curvature output](checks/curvature.txt) and
[radiation output](checks/radiation.txt) use exact rational and symbolic
arithmetic throughout. No evolution, fit, finite differences or numerical
tolerances enter these checks.

## Einstein coefficient extraction and gauge completion

Use $r_0=1$, $x=\cos\theta$, $f=1-1/r$ and a circle coordinate of period
$2\pi$. Take the Schwarzschild string metric $g_0$ and write

$$
g=g_0+\epsilon H+\eta P+\epsilon^2 Q+\epsilon\eta V+\epsilon^2\eta W,
\qquad E(g)=\mathrm{Ein}(g).
$$

No factorials enter the metric coefficients. If $D^kE$ denotes the
unscaled Fréchet derivative at $g_0$, the tested identities are

$$
\begin{aligned}
[E]_{11}&=DE[V]+D^2E[H,P],\\
[E]_{21}&=DE[W]+D^2E[H,V]+D^2E[P,Q]+\tfrac12D^3E[H,H,P].
\end{aligned}
$$

[curvature.py](checks/curvature.py) computes the jets of all 25 Einstein
components by exact coefficient convolution in the coordinate formulas for
$g^{-1}$, $\partial g^{-1}$, $\Gamma$, $\partial\Gamma$, $R_{ab}$ and its trace.
Only terms in $0\le i\le2$, $0\le j\le1$ are needed to recover $[E]_{ij}$.
The point is $r=2$, $x=1/3$, $\cos z=3/5$, $\sin z=4/5$.

The reference calculation uses the exact static lapse-warp formula.
The manufactured metric has $g_{tt}=-f(1+U)$, where $U$ has all five
independent polynomial parameter coefficients and the spatial metric $b$ is
unchanged. The chosen radial profiles in the source multiply
$\cos z$, $P_2(x)$, $\cos^2z$, $P_2(x)\cos z$, and
$P_2(x)\cos^2z$, respectively. For $L^2=f(1+U)$, set

$$
Q_{ab}^{\rm lapse}=\frac{D_aD_b L}{L}
=\frac{D_aD_b(L^2)}{2L^2}
-\frac{D_a(L^2)D_b(L^2)}{4L^4}.
$$

The spatial block of $E(g)$ is $E(b)_{ab}-Q_{ab}^{\rm lapse}
+b_{ab}\,\mathrm{tr}_b Q^{\rm lapse}$. The $tt$ block is
$-g_{tt}R[b]/2=0$ and the mixed time-space block is zero. Direct
parameter differentiation of the unexpanded reference agrees with the
coordinate extraction for all five nonbackground orders, hence 125
component comparisons. The $rz$ components are $1/15$ at $(1,1)$ and
$-7/100$ at $(2,1)$. At $(2,1)$ the linear, $HV$, $PQ$ and $HHP$
terms give $-3/50,-3/25,1/50,9/100$, respectively. Replacing the
$\tfrac12D^3E[H,H,P]$ term by $D^3E[H,H,P]$ produces the incorrect
$rz$ component $1/50$.

The finite diffeomorphism control pulls back the exact string by

$$
r'=r+\epsilon(1+r)\cos z+\eta(r-1)P_2(x),\qquad
(t',x',\phi',z')=(t,x,\phi,z).
$$

This is a local change of coordinates. Smooth radial cutoffs equal to one
near the test point make it the identity near the ends of the exterior
and leave the jets unchanged. Exact pullback differentiation gives zero
for all retained Einstein coefficients. Omitting only the $(2,1)$
metric completion gives $[E_{rz}]_{21}=233/200$ even though every lower
order still vanishes. The compactly supported pullback has identically
zero tensor news in the fixed asymptotic frame.

The flat mixed Bianchi control uses two distinct on-shell exponential jets
with covectors $(3,5,0,0,4i)$ and $(2,0,2,0,0)$ and real transverse,
trace-free polarizations, evaluated at the coordinate origin. The compact
charges are $+1$ and $0$ for a flat test circle of radius $1/4$.
The mixed coefficient is nonzero and its divergence with respect to the
background vanishes exactly. Changing the second time component from 2 to
3 while keeping its polarization puts that wave off shell. The resulting
background divergence of the mixed coefficient is
$(15/4,25/4,0,0,5i)$. This probes the requirement that the lower-order
equations hold before imposing background conservation of the mixed source.
The flat exponential jets are local algebraic tests.

## Independent axial reduction check

For a general axisymmetric axial perturbation of an $SO(3)$-warped
five-metric, use the azimuthal circle decomposition

$$
ds_5^2=b_{ij}dy^i dy^j+\rho^2(d\phi+B_i dy^i)^2,\qquad
b=g_3+\frac{R^2dx^2}{1-x^2},\quad \rho=R\sqrt{1-x^2}.
$$

The axial RW gauge is $B_x=0$, $B_a=A_a P_\ell'(x)$, with orbit index
$a$ on $g_3$. The azimuthal Einstein action term is
$-\sqrt{-b}\rho^3(dB)_{ij}(dB)^{ij}/4$, so the axial equation is
$\partial_i[\sqrt{-b}\rho^3(dB)^{ij}]=0$. The angular factorization uses
$\sqrt{-b}\rho^3=\sqrt{-g_3}R^4(1-x^2)$ and

$$
\partial_x[(1-x^2)^2 P_\ell''(x)]
=-\lambda(1-x^2)P_\ell'(x),\qquad \lambda=\ell(\ell+1)-2.
$$

The resulting orbit equation and the angular constraint are

$$
D_b(R^4F^{ba})-\lambda R^2A^a=0,\qquad
D_a(R^2A^a)=0,\qquad F=dA.
$$

The $m=0$ calculation fixes the coefficients for all $m$ by rotation
covariance. With the Schwarzschild-string orbit and a manufactured
$\ell=2$ covector depending on $t,r,z$, the direct five-dimensional
Einstein coefficient satisfies

$$
\begin{aligned}
\delta E_{a\phi}&=-\frac{(1-x^2)P_2'}{2R^2}\,g_{ac}
\left[D_b(R^4F^{bc})-4R^2 A^c\right],\\
\delta E_{x\phi}&=\tfrac12(1-x^2)P_2''\,D_a(R^2 A^a).
\end{aligned}
$$

The angular constraint component is $-64/5$ at the test point. Removing
the Proca mass term fails the orbit-component comparison. The action
calculation provides the general orbit equation. The exact point
comparison tests its component, sign and constraint factors on the
Schwarzschild orbit.

## KK projection and tensor flux

[radiation.py](checks/radiation.py) implements the Fourier convention with
$H_{\pm1}=1/2$ for $H=\cos z$. Pairing $H$ with the aligned mixed response
gives a zero-mode factor $1/2$. A pure $n=\pm2$ quadratic field times an
$n=0$ tensor has zero $n=0$ coefficient. A sine response paired with a cosine
seed also has zero average in the tested undifferentiated product.
Derivatives can change sine/cosine parity in individual factors while
preserving the KK charge. The reflection rule applies to the full product.

For unit-normalized $Y_{20}$, direct sphere integration gives

$$
\int |T_{AB}^{\rm even}|^2d\Omega
=\int |T_{AB}^{\rm odd}|^2d\Omega=12,
\qquad \int T_{AB}^{\rm even}T^{AB}_{\rm odd}\,d\Omega=0,
$$

where $T_{AB}^{\rm even}$ is the trace-free sphere Hessian and
$T_{AB}^{\rm odd}=\epsilon_{(A}{}^C T_{B)C}^{\rm even}$. The
$\ell=0,1$ versions vanish. In an orthonormal sphere dyad,
$N_{AB}N^{AB}=2(N_+^2+N_\times^2)$. Thus with $N_{AB}=\partial_uC_{AB}$
and $C_{AB}=\Psi T_{AB}$, the tensor Bondi flux
$\int N_{AB}N^{AB}d\Omega/(32\pi G_4)$ gives the single real-mode
coefficient $12|\partial_u\Psi|^2/(32\pi G_4)$, with $G_4=G_5/L$ for
circle length $L$.

## Retarded finite-data control

The flat radial $\ell=2$ master operator
$P=\partial_t^2-\partial_r^2+6/r^2$ admits an outgoing solution

$$
\Psi_h=F''(t-r)+3F'(t-r)/r+3F(t-r)/r^2,
$$

where $F=[(u+2)(u+1)]^6$ on $[-2,-1]$ and zero elsewhere. Its compact
outgoing shell starts away from the origin and has finite energy. A time
cutoff $a(t)=6t^5-15t^4+10t^3$ for $0\le t\le1$, continued by zero and
one, produces the compactly supported source
$P(a\Psi_h)=a''\Psi_h+2a'\partial_t\Psi_h$. The solution $a\Psi_h$ has
zero initial value and velocity, and its outgoing shear coefficient at
infinity is $F''(u)$. Dropping the near-zone terms fails the wave
equation. Adding $\Psi_h$ preserves the source and fails the prescribed
zero initial pair.

The single-polarization energy is $450/(323323\pi G_4)$ in these
dimensionless pulse units. For $n=F'''$, the constant time-shift tangent
is proportional to $n'$. The full-pulse interference $\int nn' du$
is zero and its value on $[-2,-7/4]$ is $164025/2147483648$. A restricted
cut window alone is therefore sensitive to a clock shift even in this
compact example.

These checks verify local source combinatorics, a finite diffeomorphism,
on-shell mixed source conservation, flux factors and a manufactured
retarded prescription. They supply no physical GL–tensor overlap,
Schwarzschild null-source bound, or nonlinear family-to-news estimate.

## Far-front comparison arithmetic

Run python3 GLR-GOAL-007/checks/front_bounds.py to check the rational
inequalities in [WITNESS-BOUNDS.md](WITNESS-BOUNDS.md). The script
inherits the pinned physical master envelope. It checks the sign
margin, the exponential integral comparison and the resulting
delay-variance bounds. See [saved output](checks/front_bounds.txt).
