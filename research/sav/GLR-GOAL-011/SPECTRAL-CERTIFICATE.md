# Local certificate for the axial pole and the shifted pencils

Use the outgoing continuation and Wronskian conventions of [GREEN-FUNCTION](GREEN-FUNCTION.md) and [QNM-RESIDUES](QNM-RESIDUES.md). In $r_0=1$ units there is exactly one simple Regge–Wheeler zero in
$$
 |s-s_c|<5\times10^{-8},\qquad s_c=-0.177924631-0.747343369i.
$$
This is a local certificate around the literature fundamental. Its conjugate follows by reality. Throughout the closed disk and the pinned L10 growth bracket, the shifted massless matching scalar obeys $\operatorname{Im}D_0(s-2\Omega)>0.28$ and the shifted massive matching determinant obeys $\operatorname{Im}\det D_k(s-\Omega)>0.40$. These signs refer to the directed logarithmic derivatives below. Both shifts are therefore noncoincident at the exact returning pole. With $h\sim e^{sx}$ at the horizon, $g_+\sim e^{-sx}$ at infinity and $W=hg_+'-h'g_+$, the certificate gives $3.64<|W'(s_q)|<10.57$.

## Riccati boundary conditions

Write the scalar and massive systems as $Y_{xx}=(s^2 I+V)Y$. For the massive pair $V=\operatorname{diag}(V_Y,V_Z)-sC\sigma_1$ in [C's conventions](investigators/C-RESONANCE.md). Factor $Y=E N$, set $\ell=E_x/E$, and write $P=N_xN^{-1}$. Then
$$
 P_x=Q-2\ell P-P^2,\qquad Q=s^2 I+V-(\ell^2+\ell_x)I .
$$
At the horizon $\ell=s$. On the right $\ell=-s$ in the massless case. In the massive case $\ell=-\kappa+\alpha f/r$ with $\kappa=\sqrt{s^2+k^2}$ and $\alpha=k^2/(2\kappa)$. The branch is continued from $\operatorname{Re}s>0$ along the lower side of the massive threshold. In the target box it is the negative principal root. The outgoing ray is $r=2.5+e^{i\pi/4}\rho$, using the continuation of $x=r+\log(r-1)$ from the exterior. At the matching radius define $D=(\ell I+P)_{\rm left}-(\ell I+P)_{\rm right}$. Invertibility of $N$ follows by solving $N_x=PN$ with the normalized endpoint limit, then by uniqueness of the first-order system on each validated segment. A zero of the scalar $D$ or the massive $\det D$ is the corresponding pole condition.

Near the horizon use $t_h=r-1$. On the certified parameter boxes and $t_h<10^{-8}$, the rational expressions for $V$ give $\|Q\|_F\le100t_h$. For example $|s|<1$, $f\le t_h$, $|C|\le 4k t_h/(4+k^2)$, and $a,b$ in the massive potential and their $x$ derivatives are $O(t_h)$ with coefficients bounded directly using $r\ge1$ and $4+k^2r^2\ge4$. The scalar horizon coefficient is bounded by $9t_h$. On a horizon interval $t_h\le t_0$ the Riccati integral from $x=-\infty$ maps the weighted ball $\|P\|_F\le1000t_h$ into itself and is contractive. In the variable $\log t_h$ its operator norm after this weighting is bounded by
$$
 K_H=\frac{1+t_0}{1+2(1+t_0)\operatorname{Re}s}.
$$
This follows from $dx/d\log t_h=r\le1+t_0$. The code checks $K_H(100+10^6t_0)<1000$ and $2K_H10^3t_0<1$. The compact integration starts near $t_0=10^{-22}$.

On the right, for $\rho\ge R=144$, $|r|\ge\rho$ and $|dx/d\rho|\le 1+(R-1)^{-1}$. Write $w=e^{i\pi/4}$ and $\kappa=s$ in the scalar case. An upper bound for $\operatorname{Re}(d\log E/d\rho)$ is
$$
 -a_0=-\operatorname{Re}(w\kappa)+\frac{\lvert\kappa\rvert}{R-1}
              +\frac{\lvert\alpha\rvert}{R}.
$$
For both pencils the supplied bound $Q_R$ gives $\|Q\|_F\le Q_R(R/\rho)^2$. In the massive case the coefficient $k^2/r$ cancels exactly between the potential and $\ell^2$. The remaining inverse-square bounds use $|4+k^2r^2|\ge k^2|r|^2-4$. The integral from infinity, with kernel $E(y)^2/E(x)^2$, maps the weighted ball $\|P\|_F\le m(R/\rho)^2$ into itself for $m=2K_R Q_R$, $K_R=(1+(R-1)^{-1})/(2a_0)$. The checked inequality $2K_Rm<1$ makes it a contraction. The integrable $\rho^{-2}$ weight fixes the outgoing normalization for $N$. Analytic dependence on $s$ follows from uniform contraction on the compact spectral boxes and analytic ODE dependence after the endpoints. These endpoint branches agree with the retarded ones by the continuation argument in [C](investigators/C-RESONANCE.md).

## Directed compact flow and root count

SciPy generates Hermite candidate matrices $P_c$ for 4,200 cells per side. Their accuracy is unused in the certificate. For each complex affine radial cell parameterized by $\xi\in[0,1]$, the program clears the rational denominator and encloses the residual
$$
 F_c=f\partial_r P_c-Q+2\ell P_c+P_c^2
$$
using Acb polynomial coefficients and the full parameter box. With $\gamma=dx/d\xi$, the error $e=P-P_c$ satisfies
$$
 e'=-2\gamma\ell e-\gamma(P_c e+eP_c)-\gamma e^2-\gamma F_c .
$$
For the Frobenius norm, the linear growth rate is at most $-2\operatorname{Re}(\gamma\ell)+2\lambda_{\max}(\operatorname{Herm}(-\gamma P_c))$. Left and right multiplication both give this Hermitian bound. The code bounds the largest eigenvalue by the Gershgorin row estimate. Under a trial cap $b$ on the error, the quadratic term adds at most $|\gamma|b$ to the rate. A scalar exponential comparison with outward-rounded force $|\gamma|\|F_c\|_F$ encloses the whole cell if the comparison is strictly below $b$ at both endpoints. Monotonicity of the constant-rate comparison then closes the bootstrap and precludes a Riccati singularity.

The code bounds $|D_0|$ on the larger disk of radius $0.005$ via its enclosing rectangle. A centered secant with step $0.0004$ approximates $D_0'(s_c)$, with error bounded by the Acb secant radius and the odd Cauchy series from that larger disk. On $|s-s_c|=5\times10^{-8}$ the deviation from the resulting linear function is below $1.31\times10^{-8}$, while the comparison linear modulus is above $1.69\times10^{-7}$. Rouché counts one zero with multiplicity. The same Cauchy series controls $D_0'$ on the small disk. At the zero, $W'=-hg_+D_0'$. The program encloses $\log(hg_+)$ by integrating the scalar $P$ in the two directions, including the analytic tails. The exponential factors cancel at the matching point. This yields the normalized Wronskian bound above.

The shifts include the full interval $\Omega\in[0.06328118,0.06328120]$. The noncoincidence enclosures are local to the pole disk. Shifted poles elsewhere on the continuation strip may be bypassed as meromorphic singularities. The certificate supplies the pole denominator and removes local coincidence ambiguity. The [physical numerator](PHYSICAL-RESIDUE.md) has a converged numerical estimate. Its certified error and contour dominance on a time window remain open.

Run from the scoped research directory with Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6 and SciPy 1.18.0.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/spectral_certificate.py
```

[Saved output](checks/spectral_certificate.txt) · [Independent adverse review](investigators/E-SPECTRAL.md)


The [signed physical certificate](SIGNED-RESIDUE.md) subsequently
tightens the normalized $W'$ through a five-point Cauchy-controlled
derivative and an outgoing Volterra amplitude enclosure. The pole
count and shifted noncoincidences here are the spectral inputs to
that calculation.
