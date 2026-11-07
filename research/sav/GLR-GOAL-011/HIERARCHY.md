# Exact constrained axial hierarchy

Use signature $(-++++)$, $x=r_*=r+r_0\log(r/r_0-1)$, $k=2\pi/L$, and no factorials in the metric coefficients. All axial equations below are linear in $\eta$. The round spherical metric solves vacuum through $\epsilon^2$. The same fixed completed coordinates are used in the source and the data.

## Reduction and all source terms

On the three-dimensional orbit $(t,r,z)$ write the round metric as $b_{ab}+R^2\gamma_{AB}$. For a real unit-normalized $Y_{2m}$ use $X_A=-\varepsilon_A{}^B D_BY$, $X_{AB}=D_{(A}X_{B)}$, and $\lambda=(\ell-1)(\ell+2)=4$. If the axial coefficients of the metric are $h_a X_A$ and $2h_T X_{AB}$, define $A_a=h_a/R^2-\partial_a(h_T/R^2)$ and $F=dA$. The angular gauge generator $\Lambda X^A$ adds $R^2\partial_a\Lambda$ and $R^2\Lambda$ to $h_a,h_T$, respectively. Orbit gauge acts by pullback.

Set
$$
 P^{bacd}=\sqrt{|b|}R^4 b^{bc}b^{ad},\quad M^{ab}=\sqrt{|b|}R^2 b^{ab},\quad
 E[A]^a=\partial_b(P^{bacd}F_{cd})-\lambda M^{ab}A_b. \tag{1}
$$
The mixed angular-orbit Einstein equation gives $E=0$. The axial angular equation is $\partial_a(M^{ab}A_b)=0$, which also follows from the divergence of (1). There are no other axial components. The connection calculation in [A](investigators/A-GEOMETRY.md) and second variation of the Einstein action in [E](investigators/E-ADVERSARIAL.md) are separate routes. In the action the sphere vector curvature term is $-R^4F^2/4$ and the tracefree sphere deformation term is $-\lambda R^2 A^2/2$, after dividing by $\int |X|^2$. Terms proportional to the spherical Ricci tensor vanish through the target order.

For reproducible coefficient extraction write $b=b_0+\epsilon h+\epsilon^2 b_2$ and $R=r(1+\epsilon d+\epsilon^2 e)$. Let $G_0=b_0^{-1}$, $G_1=-G_0hG_0$, $G_2=G_0hG_0hG_0-G_0b_2G_0$, $X=G_0h$, $Y=G_0b_2$. With $s=2,4$ the two weights in (1) use
$$
 w_{s,0}=1,\quad w_{s,1}=\tfrac12\operatorname{tr}X+sd,\quad
 w_{s,2}=\tfrac12\operatorname{tr}Y-\tfrac14\operatorname{tr}X^2
    +\tfrac18(\operatorname{tr}X)^2+se+\tfrac{s(s-1)}2d^2+\tfrac{s}2d\operatorname{tr}X.
$$
Use $P_j=\sqrt{|b_0|}r^4\sum_{l+i+k=j}w_{4,l}G_i^{bc}G_k^{ad}$, $M_j=\sqrt{|b_0|}r^2\sum_{l+i=j}w_{2,l}G_i^{ab}$. For $A=\eta(A_0+\epsilon A_1+\epsilon^2 A_2)$ the system is
$$
 E_0A_0=0,\qquad E_0A_1=-E_1A_0,\qquad
 E_0\langle A_2\rangle=-\langle E_1A_1+E_2A_0\rangle. \tag{2}
$$
Here $E_j$ uses $P_j,M_j$ and angle brackets average the circle. Two insertions are represented by $E_1A_1$, the terms linear in $b_2,e$ in $E_2$ are the spherical backreaction, and the rest of $E_2$ are the direct quadratic operator variation. The conversion $A=h/R^2-d(h_T/R^2)$ contains both first- and second-order products with the raw axial metric coefficients. Pull back the slice and metric together if changing orbit gauge.

The all-component connection difference is $C^A{}_{BC}=g^{AD}(\bar\nabla_B\delta g_{DC}+\bar\nabla_C\delta g_{DB}-\bar\nabla_D\delta g_{BC})/2$. Insert the geometric series for $g^{-1}$ in $\delta\mathrm{Ric}_{BC}=\bar\nabla_A C^A{}_{BC}-\bar\nabla_C C^A{}_{BA}+C^A{}_{AD}C^D{}_{BC}-C^A{}_{CD}C^D{}_{BA}$. In raw variables the coefficient is exactly $\mathcal L K+2Q(H,J)+2Q(B,P)+3T(H,H,P)$. The axial reduction is its completed form evaluated on the lower spherical vacuum equations. The sourced angular divergence follows also by coefficient extraction in the Bianchi identity. Circle reflection makes noncompact components even and compact components odd. At mixed order there are $n=\pm1$ components of $A_1$, including its compact polarization and its divergence equation. At returning order the mean compact component vanishes. The $n=\pm2$ part of $B$ contributes only to $n=\pm2$ of $K$. Half-period translation excludes odd GL orders from the mean for this data prescription. Axial parity and $\ell=2$ are preserved by the spherical background.

## Spherical vacuum backreaction in the axial projection

Take $H$ to be the GOAL-005 future-regular nonzero-$k$ unitary mode, $H_{zA}=0$, with $H_{\theta\theta}/(2r^2)=1$ at $2r_0$ and the real amplitude $e^{\Omega t}\cos kz$. Define the Einstein-frame monopole $m_{\mu\nu}=\langle B_{\mu\nu}\rangle+\bar g_{\mu\nu}\langle B_{zz}\rangle/2$ and the completed radion $\phi_2=\sqrt{3}\langle B_{zz}\rangle/4$. The conserved linear Einstein source for $m$ is
$$
 \mathcal T_{\mu\nu}=-\langle Q_{\mu\nu}(H,H)\rangle
           +\tfrac12\bar g_{\mu\nu}\langle\operatorname{tr}_5 Q(H,H)\rangle.
$$
In ingoing areal EF gauge $v=t+x$, write $g_{E,vv}=-f+\epsilon^2(2\mu/r-2f\beta)$ and $g_{E,vr}=1+\epsilon^2\beta$. With zero mass perturbation and normalized clock, the solution is
$$
 \beta(v,r)=-\tfrac12\int_r^\infty R\mathcal T_{RR}(v,R)dR,\qquad
 \mu(v,r)=\tfrac12\int_r^\infty R^2\mathcal T^v{}_v(v,R)dR. \tag{3}
$$
The identity $\mu_v=r^2\mathcal T^r{}_v/2$ and the angular equation follow from conservation with vanishing flux at the fixed-$v$ end. These are the regular GOAL-002 equations (8)–(10) used by GOAL-005. They include the strong-field spherical backreaction. The metric is smooth at the future horizon and short range on bounded outgoing cuts since the GL mode is smooth in EF and decays as $e^{\Omega t-q_\infty r}$ up to powers. The prescribed Z radion solves its own sourced wave equation with zero initial completed pair. Its stress is quartic in $\epsilon$. In the mean noncompact blocks of (1) and in the exact dual curl $Q_z$, the powers of the mean radion cancel because $R=e^{-\phi/\sqrt3}r$, $b_2^{\mathrm{orbit}}=e^{-2\phi/\sqrt3}b_E$, and $g_{zz}=e^{4\phi/\sqrt3}$. The orbit volume density is $\sqrt{|b_E|}$. This cancellation holds for the mean radion contribution, with the quadratic nonzero-mode products still included in (2).

## Returning scalar including extraction products

Use $Q=R^4 *_b F$ with orientation $dt\wedge dr\wedge dz$. The exact dual equation is
$$
 D_b[R^{-2}(dQ)^{ba}]=\lambda R^{-4}Q^a,
 \qquad D_a(R^{-4}Q^a)=0. \tag{4}
$$
In the background $(t,r)$ chart define the density corrections
$$
 \mathcal C=\langle(P_1F_1+P_2F_0)^{tr}\rangle,\qquad
 \mathcal D^a=\lambda\langle M_1^{ab}A_{1b}+M_2^{ab}A_{0b}\rangle.
$$
Use the *exact* mean dual component coefficient $Q_{2z}=-r^4\langle F_{2,tr}\rangle+\mathcal C$, and $\Psi_2=2Q_{2z}/(\lambda r)$. The $t,r$ equations give
$$
 \lambda r^2 A_{2t}^{0}=f(\partial_r Q_{2z}+\mathcal D^t),\qquad
 \lambda r^2 f A_{2r}^{0}=\partial_t Q_{2z}-\mathcal D^r.
$$
Eliminating both using $F=dA$ yields
$$
 \left(-\partial_t^2+\partial_x^2-V_{\rm RW}\right)\Psi_2
 =\frac{2r}{\lambda}\left[-\partial_t(r^{-2}\mathcal D^r)
 -\partial_x(fr^{-2}\mathcal D^t)-\lambda f r^{-4}\mathcal C\right],
 \qquad V_{\rm RW}=f(6/r^2-3r_0/r^3). \tag{5}
$$
Using the background-only curl instead transfers $L_{\rm RW}[2\mathcal C/(\lambda r)]$ to the other side and changes the initial pair. The two sources agree as descriptions of the same physical trace after the matching data and extraction are included. The compact products in the Einstein-frame shear extraction vanish on the finite cuts used here by the weighted mixed estimate. A pure axial gauge has $F=0$. A spherical pure pullback of an unperturbed solution has zero physical change when the source, preparation and frame are pulled back together.
