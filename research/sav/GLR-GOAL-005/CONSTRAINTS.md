# Global exterior vacuum constraints for the zero-radion preparation

Use $r_0=1$, $L=10$, and the horizon-crossing slice $\tau=v-r=0$ with inner leaf $r_i=3/4$. The construction solves the exact constraints on the spatial exterior including this collar inside the background horizon. Its future domain of dependence supplies the exterior development where local existence applies.

## Free data

Use the radial shift-zero chart and curvature sign of [GOAL-003, equations (1)–(3), (5), (6)](../physics/goal003_data_nonlinear.md). The leaf metric is $b^2dz^2+R^2d\omega^2$, the radial lapse is $a$, $q$ is the leaf trace of $K$, and $\eta=K(n,\partial_z)$. Prescribe
$$
 b=e^{2\epsilon^2 F/\sqrt3},\quad
 R=r\exp(\epsilon R_1/r-\epsilon^2 F/\sqrt3),\quad
 v_K=K^z{}_z=-2\epsilon^2P/\sqrt3,\quad
 d=d_0+\epsilon(\kappa_1+q_1/2),\quad \kappa=-q/2+d.
$$
Here $d_0=1/[2r^{3/2}(r+1)^{3/2}]$, $F,P$ are fixed smooth radial compactly supported functions, and subscript 1 denotes the exact growing first jet including its velocity in the regular completed frame. The inner values are $a=a_0\exp(\epsilon a_1/a_0+\lambda)$, $q=q_0+\epsilon q_1$, and $\eta=\epsilon\eta_1$. The zero-radion choice Z is $F=P=0$. Other $F,P$ change the order-two scalar Cauchy data. At order two the first-order shift has vanishing compact component and the zero Fourier component of a $z$-derivative vanishes. Thus $b,v_K$ prescribe the value and normal velocity of the completed radion. The first jet has $b_1=(v_K)_1=0$.

The local mixed constraint estimate in GOAL-003 applies on each finite collar near the background. Differentiating the locally unique system gives the growing first jet if $\lambda'(0)=0$. Reflection and translation by $L/2$ give zero compact momentum and covariance under $\epsilon\mapsto-\epsilon$, respectively, for fixed $F,P$ and even $\lambda$.

## Exact exterior equations

Outside the support of $F,P$, $b=1,\ v_K=0,\ R=r e^g$ with $g=\epsilon R_1/r$. The decaying GL branch bounds $g,d-d_0$ and every fixed finite set of derivatives exponentially, after reducing the exponent to absorb polynomial factors. For $r\ge R_m>2$ outside the support, set
$$
 U=a^{-2},\quad U_0=\frac{r}{r+1},\quad w=r(U-U_0),\quad
 Q=r^2q,\quad J=r^2\eta,\quad W=(Q-Q_0,J),\quad Q_0=r^2q_0.
$$
Define $H_*=2(1/r+g_r)$, $B=2(H_*)_r+H_*^2+|A_*|^2$, $\mathcal R_h=2r^{-2}e^{-2g}-4g_{zz}-6g_z^2$, and $Z=-Q^2/(2r^4)+2dQ/r^2-2J^2/r^4$. The equations following from the complete constraints are
$$
\begin{aligned}
 w_r &= D w_{zz}+2g_zD w_z-\frac{3}{2rH_*U^2} w_z^2
        +(1/r-B/H_*)w+\mathcal F,\qquad D=(H_*U)^{-1},\\
 \mathcal F&=r[(\mathcal R_h+Z)/H_*-U_0 B/H_*-(U_0)_r],\\
 Q_r&=aJ_z+2a_zJ+2ag_zJ-2g_rQ+2r^2(1/r+g_r)d,\\
 J_r&=aQ_z/2-a_zQ/2+ag_zQ-2g_rJ+ar^2d_z+r^2d\,a_z .
\end{aligned}
\tag{1}
$$
The drift $2g_zD w_z$ comes from the leaf Laplacian. The relation $B/H_*=1/r$ holds in the ideal exterior $g=0$, and $\mathcal F$ vanishes at the background.

## Global weighted estimate

Use periodic $H^m$, $m\ge4$, and
$$
 E=\|w\|_{H^m}^2+\tfrac12\|Q-Q_0\|_{H^m}^2+\|J\|_{H^m}^2 .
$$
On a ball $E^{1/2}\le\delta$, with $U,rH_*$ bounded above and away from zero, write $e(r)=C_m e^{-c r}$ for an integrable free-tail bound. Then $D\asymp r$, $1/r-B/H_*=O_{H^m}(|\epsilon| e(r))$, and $\|\mathcal F\|_{H^m}\le C r^{-2}\|W\|_{H^m}+|\epsilon|e(r)$. In divergence form the lapse commutator satisfies
$$
 \sum_{j\le m}\|[\partial_z^j,D]w_z\|_2
 \le C(\|w-\bar w\|_{H^m}+|\epsilon|e(r))\|w_z\|_{H^m}.
$$
Indeed $D_z=-w_z/(rH_* U^2)+O_{H^{m-1}}(|\epsilon|e(r))$, and composition and product estimates in one dimension give this bound with no derivative loss. The explicit gradient-squared term and the subtraction $D_z w_z$ in divergence form cost at most $C\|w\|_{H^m}\|w_z\|_{H^m}^2$ plus integrable free-tail terms. The diffusion absorbs them. In the momentum equations the symmetrized principal terms integrate by parts. The commutators and the $Q_0 a_z$ term cost $C r^{-1}\|w_z\|_{H^m}\|W\|_{H^m}$ on the ball, since $a_z=-(2r)^{-1}U^{-3/2}w_z$. Young's inequality costs $C r^{-3}\|W\|_{H^m}^2$ against $r\|w_z\|_{H^m}^2$. Thus
$$
 E'(r)+c_1 r\|w_z\|_{H^m}^2
 \le C[r^{-2}+e(r)]E(r)+C\epsilon^2 e(r).
\tag{2}
$$
The constants may depend on $R_m,m$, the exact normalized mode and the fixed $F,P$. Finite-collar data satisfy $E(R_m)\le C(\epsilon^2+\lambda^2)$. Gronwall closes the ball uniformly for small parameters. The local estimates continue the solution on finite subintervals with the controlled norm and coefficient lower bounds. One can start at any sufficiently high finite $m$ to obtain the finite derivative estimates below. Local tame higher-order continuation propagates smoothness on compact intervals.

For $\widetilde w=w-\bar w$, the projected form of (1) and Poincaré give
$$
 (\|\widetilde w\|_{H^m}^2)' + c_2 r\|\widetilde w\|_{H^m}^2
 \le C r^{-5}\|W\|_{H^m}^2+C\epsilon^2 e(r).
\tag{3}
$$
The kernel is bounded by $\exp[-c_2(r^2-\rho^2)/2]$. Splitting at $\rho=r/2$ gives $\widetilde w=O_{H^m}(r^{-3})$. Averaging (1) and integrating $D w_{zz}$ by parts gives $\bar w'=O(r^{-2})$ and $\bar w=w_\infty+O(r^{-1})$. Equation (1) gives $w_r=O_{H^{m-2}}(r^{-2})$. In the asymptotic Cartesian product chart this yields a spherical $1/r$ metric term, an $O(r^{-2})$ metric remainder with first derivatives $O(r^{-3})$, angular lapse harmonics $O(r^{-4})$, and $K=O(r^{-2})$ with finite square integral. The ADM surface integral gives $M/M_0=1-w_\infty$. The constrained $Q,J$ can carry persistent oscillations in $r$ and $z$. The corresponding derivatives of $K$ need only be $O(r^{-2})$. These are explicit spatial falloffs with finite mass. A stronger differentiated end class or a smooth conformal null boundary would require further estimates.

## Charge tuning and parameter regularity

At zero seed the [exact spherical family in GOAL-004, equations (2)–(3)](../physics/goal004_data_news.md) gives $\partial_\lambda M(0,0)/M_0=9/14$. To show $C^1$ of the mass, reserve two derivatives of the base solution. Varying $a$ in a momentum principal term produces $\delta a\,W_z$, with $\delta a=O(r^{-1}\delta w)$. A mass-dependent radial phase can grow logarithmically under differentiation. The local solution is differentiable at each finite radius. In the differentiated energy $\delta a\,W_z$ costs $C\delta/r$ times the derivative energy using the spare derivative and base smallness. The varied diffusion coefficient is $O(\delta w)+O(e(r))$. After integration by parts its product with $w_z$ costs an integrable coefficient by (2). The other top-order couplings absorb as before, and varying the fixed free data gives integrable forcing. Thus $E_{\rm par}'\le[C\delta/r+C h(r)]E_{\rm par}+C h(r)$, with $h$ uniformly integrable. It follows that $E_{\rm par}^{1/2}=O(r^\gamma)$ uniformly on a smaller parameter neighborhood, for some $\gamma<1$.

In the differentiated mean equation $\delta D\,w_{zz}$ is integrable by (3). The mean of $D\,\delta w_{zz}$ is estimated using $D_z=O(r^{-3})+O(e(r))$. The remaining terms are $O(r^{-2+\gamma})+O(e(r)r^\gamma)$ or better. Uniform convergence of the derivative of the mean gives the $C^1$ mass map. The implicit function theorem yields an even $\lambda(\epsilon)$ fixing $M=M_0$, with $\lambda'(0)=0$. This tuning changes the inner leaf. No independent static tension parameter is used.

For perturbative extraction on compact diamonds one can keep $\lambda=0$. This fixed-inner-leaf family is smooth in the seed at every finite radius and has the same growing first jet and specified radion second jet. The $C^1$ mass result alone does not bound higher derivatives of the tuned family in a radiation norm.
