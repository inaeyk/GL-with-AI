# Canonical source pairing at the continued pole

This identity gives a lower-derivative evaluation route for the numerator in [QNM-RESIDUES](QNM-RESIDUES.md). Use the exact mean curl and the fixed canonical preparation of [FINITE-DATA](FINITE-DATA.md). Let $\Theta$ indicate the future of the initial graph $t=T(x)$, and use the background tortoise coordinate $x$. For the separated adjoint master $w=e^{-st}h(s,x)$, set
$$
 \phi=rw,\qquad p=r^{-2}\mathcal D^r,\qquad q=f r^{-2}\mathcal D^t.
$$
The densities $\mathcal D^a$ and $\mathcal C$ are those of [HIERARCHY](HIERARCHY.md) in $(t,r)$. In particular the full source is
$$
 S=\frac{2r}{\lambda}\left(-p_t-q_x-\lambda f r^{-4}\mathcal C\right).
$$

## Cancellation on the launch graph

The electric data give $Q_{2z}|_\Sigma=0$. The fixed tangential potential gives $A_{2,x}+T'A_{2,t}=0$ in the product chart. Substitution of the two constrained mean equations for $A_2$ yields
$$
 (\partial_t+T'\partial_x)Q_{2z}=\mathcal D^r-T'f\mathcal D^t
 \quad\text{on }\Sigma .
$$
The graph term in (2) of [GREEN-FUNCTION](GREEN-FUNCTION.md) is therefore
$$
 -\frac2\lambda\int_\Sigma \phi(p-T'q)\,dx.
$$
Integrating $-p_t-q_x$ by parts over the future of the graph generates its opposite. Initially in the physical Laplace half-plane, with vanishing radial endpoint terms, the numerator can thus be computed as
$$
 I(s)=\frac2\lambda\int_{t\ge T(x)}
     \big[\phi_t p+\phi_x q-\lambda f r^{-4}\phi\mathcal C\big] \,dt\,dx. \tag{1}
$$
Equivalently the integrand in $dt\,dr$ is $2(a_a^\dagger\mathcal D^a+F_{tr}^\dagger\mathcal C)$, where
$a_t^\dagger=f\phi_r/(\lambda r^2)$, $a_r^\dagger=\phi_t/(f\lambda r^2)$ and $F_{tr}^\dagger=-\phi/r^4$. The adjoint pairing is bilinear. There is no independent upper-order graph term in this prescription once all exact-curl and density contributions are included.

A coordinate-density version applies at either higher order before projection. In adapted coordinates $(\tau,y)$ the electric density is $(P F)^{\tau i}$ because $n_a=-N(d\tau)_a$ and $\sqrt{|b|}=N\sqrt\gamma$. For $j=1,2$ the pullback of $A_j$ to $\Sigma$ vanishes and the exact coefficient of this electric density is zero. Thus $d(\Theta A_j)=\Theta F_j$, and the distributional equation is
$$
 E_0[\Theta A_j]^a
 =-\partial_b\left[\Theta\sum_{m=1}^j (P_m F_{j-m})^{ba}\right]
  +\lambda\Theta\sum_{m=1}^j M_m^{ab}A_{j-m,b}. \tag{2}
$$
The step derivative of the background electric coefficient cancels the step derivative of the higher-order density terms. This identity retains the compact polarization and the Gauss constraint. The baseline still uses its nonzero graph data. When reconstructing $A_0$ from derivatives of its curl after zero extension, $\Theta\partial_a Q_0=\partial_a(\Theta Q_0)-(\partial_a\Theta)Q_0$ retains that baseline correction. Equation (2) organizes the nested shifts without introducing artificial free data.

## Horizon subtraction on the continued sheet

The separated adjoint in (1) is an outgoing time-reversed test at the future horizon. In ingoing coordinates $(v,r)$, $q=f r^{-2}\mathcal D^v-p$. With $h\sim e^{sx}$, the time-transformed part of $\phi_t p+\phi_x q$ may have leading behavior proportional to $-2s e^{2sx}$ at the horizon. Consequently the unweighted bulk integral in (1) can diverge for $\operatorname{Re}s<0$ even though the strong projected pairing is convergent. The time-domain projected combination $-\partial_t p-\partial_x q$ itself has a factor $f$ for regular ingoing fields. The difference comes from integration by parts, including the radial boundary.

For an evaluation on the continued sheet choose $x_a$ below the compact launch support. Keep the strong regular ingoing pairing to its left and integrate by parts only to its right. The exact continued numerator is
$$
 \begin{split}
 I(s)={}&\int_{x<x_a,\ t\ge T(x)} w S\,dt\,dx\\
 &+\frac2\lambda\int_{x\ge x_a,\ t\ge T(x)}
       [\phi_t p+\phi_x q-\lambda f r^{-4}\phi\mathcal C] \,dt\,dx\
 &+\frac2\lambda\int_{t\ge T(x_a)} \phi(t,x_a)q(t,x_a)\,dt .
 \end{split} \tag{3}
$$
The integrals in (3) denote the continued time transforms. The sign of the left radial boundary is positive because the strong source contains $-q_x$ and the domain begins at $x_a$. In the model with transformed $p=e^{sx}p_H$ and $q=-p$, the strong combination vanishes. Its separated bulk integral has a lower-end term proportional to $p_H e^{2sx_a}$, canceled by the displayed boundary. This is a check on the sign and on the failure of dropping the radial boundary before continuation. The far boundary vanishes using the positive outer decay margin in [QNM-RESIDUES](QNM-RESIDUES.md). The fixed finite split avoids subtracting two divergent numerics. Equations (2) and (3) give an evaluable one-dimensional overlap after solving the two shifted radial problems with the actual finite launch. The [physical overlap calculation](PHYSICAL-RESIDUE.md) evaluates this split numerically, with an independent exterior strong-form check.

## Explicit transform of the baseline packet

At the first radial stage use $\sigma=s-2\Omega$. Put $a(x)=a_\delta(T(x)-x)$ and $\widehat\Psi_0(\sigma,x)=\int e^{-\sigma t}\Theta\Psi_0(t,x)\,dt$. For $T=r-x$ one has $T'=-r_0/r$ and $T''=r_0 f/r^2$. The baseline outgoing graph data have $(\Psi_0,\Psi_{0,t},\Psi_{0,x})=(a,a_\delta',-a_\delta')$ at launch. Differentiating the step gives the exact compact radial source for $(\partial_x^2-\sigma^2-V)\widehat\Psi_0$,
$$
 \mathscr S_0=e^{-\sigma T}\left[(1-T')a_x-
         \big(\sigma(1-T'^2)+T''\big)a\right].
$$
For a test $y$, integration by parts gives
$\int y\mathscr S_0\,dx=-\int e^{-\sigma T}(1-T')(y'+\sigma y)a\,dx$, with no endpoint term across the whole support. Let $b_\sigma=e^{-\sigma T}(1-T')a$ and define
$$
 J_h(x)=\int_{-\infty}^x b_\sigma(h'+\sigma h)\,dy,\qquad
 J_g(x)=\int_x^\infty b_\sigma(g_+'+\sigma g_+)\,dy ,
$$
with the radial solutions and Wronskian at $\sigma$. The endpoint terms cancel between the two pieces of the Green function, so
$$
 \widehat\Psi_0=-\frac{g_+J_h+hJ_g}{W(\sigma)},\qquad
 \widehat{\Theta\Psi_{0,x}}=-\frac{g_+'J_h+h'J_g}{W(\sigma)}+e^{-\sigma T}a .
$$
The last term follows by differentiating the split Green expression, then adding the step correction $T'e^{-\sigma T}a$. The time derivative is $\widehat{\Theta\Psi_{0,t}}=\sigma\widehat\Psi_0-e^{-\sigma T}a$. These formulas supply the Proca two-form $F_0$ and potential $A_0$ in (2) via the background reconstruction from $Q_{0z}=\lambda r\Psi_0/2$.

For example, outside the launch support toward infinity the baseline is exactly an outgoing radial mode times the compact moment $-J_h(\infty)/W(\sigma)$. For the specified normalized narrow packet,
$$
 J_h(\infty)=\delta^{3/2}e^{-\sigma T_*}(h'+\sigma h)(x_*)\int_0^1 b(y)\,dy
             +O(\delta^{5/2}),
$$
where $x_*=x(R)$, $T_*=T(x_*)$. This follows from $d(T-x)=-(1-T')dx$ and smoothness of the compact kernel. It avoids numerical differentiation of the bump and makes the first radial stage explicit for either strong-field launch radius. It does not establish the returning overlap.
