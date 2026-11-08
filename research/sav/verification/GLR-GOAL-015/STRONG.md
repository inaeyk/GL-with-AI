# Early strong cap audit

The exact strong argument reconstructs a physical norm comparison on the early strip with the source benchmark and inherited GL/exterior inputs. It uses neither the weak projection result nor numerical derivatives of the affine candidate. The audit compared the pinned source conclusions non-blind. The stationary commutation, graph identities, cap formula and scalar bounds below are independently reconstructed.

## Recover the cap from one time-differentiated current

With $D_c=\{a\le u\le d,\ b(u)\le v\le c\}$, the inherited equation is $4w_{uv}=F_u+G_v-H-Vw$. The canonical row gives $4w_u|_\Sigma=G-b'F$. Subtracting $G-F$ from $4w_u$ cancels $G_v$ and leaves $F_u+F_v-H-Vw$ in its $v$ derivative. Evaluating the difference first on $\Sigma$ and then at the cap gives
$$
4w_u(u,c)=(G-F)(u,c)+(1-b'(u))F(u,b(u))+
 \int_{b(u)}^c (F_t-H-Vw)\,dv .
$$
Here $\partial_t=\partial_u+\partial_v$ is the stationary derivative, computed in the interior. Differentiating a zero extension of $F$ would add $(1-b')F|_\Sigma\delta(v-b)$ to the interior derivative, so including that derivative and the explicit graph term would double count. The exact polynomial control in [check_strong_geometry.py](check_strong_geometry.py) has nonzero graph and cap terms and independently reproduces the formula. The scalar equation permits an oscillatory $n^{-1/2}\chi(u)\sin(nu)\rho(v)$ error whose undifferentiated currents and cap-current errors tend to zero while its cap news grows as $n^{1/2}$. [Source cap identity and counterexample][strong].

The zero-extended shear obeys the causal integral equation $4w=\mathcal J F+\mathcal I(G-\mathcal J H)-\mathcal I\mathcal J(Vw)$, where $\mathcal I$ integrates in $u$ from $a$ and $\mathcal J$ in $v$ from $b(d)$. The graph equation cancels its distributional contact. The exact $L^2_u$ norm of $\mathcal I$ is $2(d-a)/\pi$, by the lowest mixed Dirichlet/Neumann eigenvalue for the primitive. In $L^\infty_vL^2_u$, the $j$-fold product of the potential operator is bounded by $(\|\mathcal I\| M)^j/j!$ where $M=\frac14\int\sup_u V\,dv$. This yields the exponential and explains why the $G$ bound uses fixed-$v$ slices. The direction of the integration matters for the causal operator.

For $c=d+2x(400)$, $r\ge b(d)$, $|\partial_u\log V|\le1/4$, and $dv=2\,dr/f$ on the last ray, so $M\le e^{(d-a)/4}\big[J(b(d))-J(400)\big]$ with $J(r)=3/r-3/(4r^2)$. The independent Arb and polynomial check brackets $b(d)$ between $1.62143634172470$ and $1.62143634172471$, encloses the upper-bound expression near $1.5632939412368$, and confirms the executable's $M=1.563294$ majorant. It also certifies $V<1$, $|(\log V)_x|\le1/2$, the scalar graph coefficient bounds and the graph speed bounds on the stated domain.

## Commute the true source and transform the true data

The mixed operator is stationary and its source operator has coefficients of the form $e^{\Omega t}$ times functions of $x$. Thus if $Y_1=(\partial_t+\Omega)Y$, its source is the original mixed source applied to $\Psi=(\partial_t+2\Omega)w_0$. The $F$ reconstruction splits into a homogeneous mixed map with weight $e^{\Omega t}$ and a direct, affine and fixed-mass map linear in $w_0$ with weight $e^{2\Omega t}$. The stationary derivative is precisely the sum of these maps evaluated on $Y_1,\Psi$. In particular an affine first-source term inside the mixed reconstruction carries the second weight after multiplication by the returning GL insertion. Differentiating the optical primitive instead would require a derivative of its launch-dependent phase and would give a different quantity. [Source reconstruction][strong].

For $p=T_x=-1/r$, $w_0=B$, $w_{0,u}=B'$, $w_{0,v}=0$ on $\Sigma$, and $W_0=V/4$, tangential differentiation gives $w_{0,uu}=B''+b'W_0B$, $w_{0,uv}=-W_0B$, and $w_{0,vv}=W_0B/b'$. With $h=B'+2\Omega B$, these imply $\Psi|_\Sigma=h$, $\Psi_u|_\Sigma=h'-VB/[2(1-p)]$ and $\Psi_v|_\Sigma=-VB/[2(1+p)]$. Differentiating $Y_x=-pY_t$ and $Y_t=g_m$ along the spacelike graph gives $Y_{xx}=-p_xg_m-2p\,d g_m/dx+p^2Y_{tt}$. Inserting $-Y_{tt}+Y_{xx}+\mathsf C Y_t=S_m$ on $Y|_\Sigma=0$ then gives
$$
(1-p^2)Y_{tt}|_\Sigma=-p_xg_m-2p\,d g_m/dx+\mathsf Cg_m-S_m|_\Sigma.
$$
The shifted data therefore have displacement $g_m$ and independent induced derivative traces. The graph derivative of the time-dependent coefficient includes $\Omega p$ as well as its fixed-$t$ derivative. The graph calculation and script retain both. [Source early certificate][early] · [Graph reconstruction](GEOMETRY.md).

## Close the optical and affine budgets

On this cut the scalar weights imply $h''\ge220 h'\ge0$. Rotating $Y_1$ as in the weak proof gives $X_u=2LX+P$, $P_v=-M_+X-\widetilde S/4$, with $L$ skew. The fixed-$v$ integral of $h''$ up to $u$ is at most $h'(u)$, so the shift yields $|X|/h''\le X_{\Sigma,*}+P_*/220$. The incoming graph $Q$ is handled in units of $h'$ evaluated at the earlier graph intersection and then compared to $h''(u)/220$. This argument requires monotonicity of $h'$ and never uses monotonicity of $h''$. On a radial slab, bound the partial optical primitive and source by their slab suprema. The uniform $P$ bound on the slab is the optical and incoming majorant plus accrued and slab source defects, multiplied by $e^{M_{\rm slab}\Delta v/220}$. Updating the next defect with $M_{\rm slab}\Delta v\,P_*/220$ bounds the accumulated error. The same Euclidean estimate encloses the transverse polarization.

The principal shifted readout has $F_t^{\rm ref}=2q_0 A h''/\lambda$ with $A=\int_b^v q_0\,ds$. Integrating it and dividing by four gives $h'' A(u,c)^2/(4\lambda)$. The independently correlated phase integral follows by $v=u+2x(r)$ with $dv=2\,dr/f$, and the optional cells cover the moving endpoints. At a fixed cap $x(r(u,c))-x(400)\le(d-a)/2$, so the upper endpoint lies below $400+(d-a)/2$. Intersecting this enclosure with the slab phase is valid. The affine comparison is integrated over whole subcells at exact abscissae $a+j(d-a)/192$, with the binary64 values interpreted as exact ordinates. [Executable][code].

| Contribution to $E_c=\lVert w_u(\cdot,c)-p_c\rVert_2$ | Upward bound |
| --- | ---: |
| Optical reference versus affine trace | 100.181752 |
| True cap trace | $3.59\times10^{-100}$ |
| True graph trace | 0.270272 |
| Commuted $F$ ray defect | 215.363846 |
| $H$ ray | 1.779083 |
| Causal shear potential | 6.388838 |
| Total | $<323.984$ |

The replay gives an upper bound below $323.983789883114$. The inherited homogeneous $L^2$ exterior transfer and the strong forced exterior norm give $\big|\mathcal P-\langle B',p_c\rangle\big|<0.024231+0.013648127E_c$. With the source affine enclosure this gives $[3.360524222032,12.314165777968]$ using the displayed bound and conservative coefficients, hence the outward published interval. The independent integration-by-parts check tightens the candidate pairing and supports a sharper cross-check while sharing the true-error and exterior bounds. This cap estimate is on the first eighth of the launch width. Its positive derivative ratios cease to hold on the full packet. [GOAL-014 strong transfer][transfer] · [Independent replay](REPRODUCTION.md).

[strong]: https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/observables/STRONG-CAP.md
[early]: https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/observables/EARLY-CERTIFICATE.md
[code]: https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/observables/optical_strong.py
[transfer]: https://github.com/inaeyk/GL-with-AI/blob/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014/STRONG-NEWS-PILOT.md
