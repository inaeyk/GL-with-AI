# GLR-GOAL-011 B — near-barrier axial data and fixed-mass monopole

Work at first order in the real axial amplitude $\eta$, with no factorials
in the $\epsilon$ expansion. Pinned inputs inspected are GOAL-007
[0864d9d807de56cbdd6e64d215e6295508a3007f](https://github.com/inaeyk/GL-with-AI/commit/0864d9d807de56cbdd6e64d215e6295508a3007f),
INVESTIGATOR (1)–(4), AXIAL-TRACE, HIERARCHY and HANDOFF, and its GOAL-005
baseline [61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708),
CONSTRAINTS, DERIVATIONS and REPORT. The equations inherited by that pin
are physics/goal002_second_order.md (7)–(10) and
physics/goal003_data_nonlinear.md (1)–(5). Use $r_0=1$, $L=10$,
$k=2\pi/10$, $(-++++)$ and the unitary compact first jet normalized by
its unit fractional sphere-radius perturbation at $r=2$ and $t=0$.

## Pass 1. An explicit compact packet

Put $f=1-1/r$, $s=1/r$, $x=r+\log(r-1)$ outside the horizon, and
use ingoing EF $v=t+x$ and $\tau=v-r$. On the exact GOAL-005 reference
slice $\Sigma=\{\tau=0,r\ge3/4\}$ the background orbit variables are
$$
 B=-f\,d\tau^2+2s\,d\tau dr+(1+s)dr^2+dz^2,\qquad
 a_0=\sqrt{1+s},\quad\alpha_0=a_0^{-1},\quad
 \beta_0^r=\frac{s}{1+s},\quad \beta_0^z=0 .
$$
All are smooth across $r=1$. Radial null speeds
$(-s\pm1)/(1+s)$ both point inward at $r=3/4$, so the background
perturbative Cauchy problem needs no incoming data there.

Choose the real unit harmonic $Y_{20}=\sqrt{5/(16\pi)}(3\cos^2\theta-1)$,
$X_A=-\epsilon_A{}^B D_BY_{20}$, $X_{AB}=D_{(A}X_{B)}$ and
$\lambda_{\rm ax}=(\ell-1)(\ell+2)=4$. The completed axial variable is
$A_a=h_a/R^2-\partial_a(h_T/R^2)$ with $F=dA$. In the reference $n=0$
sector use GOAL-007's $\psi=r^3F_{tx}/f=r^3F_{\tau r}$,
$\mathcal O=r\psi$. Its CPM shear at null infinity is
$\Psi_{\rm CPM}=-2\psi/\lambda_{\rm ax}$.

The RW potential $V=f(6/r^2-3/r^3)$ has
$V'=-3(4r^2-9r+4)/r^5$. Its exterior peak is
$r_p=(9+\sqrt{17})/8\simeq1.640388203$, with $V(r_p)\simeq0.6051467983$.
Define
$$
 P(r)=
 \begin{cases}
 \exp(1-1/(1-y^2)),& |y|<1,\\
 0,&|y|\ge1,
 \end{cases}
 \quad y=(r-r_p)/0.30,\qquad
 W_{\rm out}=-\frac{r-1}{r+1}P',\quad W_{\rm in}=P'.
$$
Either $W$ specifies $\psi|_\Sigma=P$, $\partial_\tau\psi|_\Sigma=W$.
The support is $[r_p-0.30,r_p+0.30]\subset(1,2)$, and all derivatives
vanish at the endpoints. The two velocities are the initial free
characteristic directions because $u|_\Sigma=-r-2\log(r-1)$ and
$v|_\Sigma=r$. The RW potential scatters either direction.

The background Proca equations give
$A_t=-\mathcal O_x/(\lambda_{\rm ax}r^2)$ and
$A_x=-\mathcal O_t/(\lambda_{\rm ax}r^2)$.
The slice tangent gives $(rP)'=(\mathcal O_x-s\mathcal O_t)/f$ and
$A_r|_\Sigma=(A_x-sA_t)/f$. Hold the following coordinate pair fixed
at every small $\epsilon$ in the GOAL-005 completed slice chart:
$$
 \boxed{ A_r=-\frac{(1+s)W}{\lambda_{\rm ax}r}
       +\frac{(rP)'}{\lambda_{\rm ax}r^3},\qquad
 \pi^r=-rP,\qquad A_z=\pi^z=0. }\tag{B1}
$$
Indeed on the product background $\det B=-1$ and
$\pi^r=-r^4F_{\tau r}$. The data have nonzero invariant curvature,
compact smooth support and finite energy. There is no linear axial
contribution to ADM mass, momentum or spin since the field is pure
$\ell=2$ on each spherical background. Packet energy can enter at
order $\eta^2$. This finite slice defines the forward axial solution.

## Pass 2. Full 3D Gauss, velocities, and an independent dual check

Let $n^a$ be the future orbit slice normal, $\gamma_{ij}$ the induced
orbit metric with $i,j=r,z$, $\sigma=\sqrt{\det\gamma}$ and
$\alpha,\beta^i$ the lapse and shift. Define
$$
 \pi^i=\sigma R^4F^{ia}n_a,\qquad A_\perp=n^aA_a,\qquad
 E_i=F_{ia}n^a=\gamma_{ij}\pi^j/(\sigma R^4).
$$
The $a=\tau$ equation of
$\partial_b(\alpha\sigma R^4F^{ba})=\lambda_{\rm ax}\alpha\sigma R^2A^a$
is $-\partial_i\pi^i+\lambda_{\rm ax}\sigma R^2 A_\perp=0$ since
$n_a=-\alpha\,d\tau_a$. Hence (B1) gives
$$
\begin{aligned}
 A_\perp&=-\frac{(rP)'}{\lambda_{\rm ax}\sigma R^2},&
 A_\tau&=\alpha A_\perp+\beta^iA_i,\\
 F_{ij}&=\partial_i A_j-\partial_j A_i,&
 \partial_\tau A_i&=\partial_iA_\tau-\beta^j F_{ij}-\alpha E_i,\\
 \partial_\tau(\sigma R^2 A_\perp)
 &=\partial_i(\alpha\sigma R^2\gamma^{ij}A_j+
                  \sigma R^2\beta^iA_\perp).
\end{aligned}\tag{B2}
$$
The last line is the exact weighted divergence. Taking the spatial
divergence of
$\partial_\tau\pi^i=-\partial_j(\alpha\sigma R^4F^{ji})+
\lambda_{\rm ax}\alpha\sigma R^2 A^i$
and using (B2) gives Gauss propagation. These are the full constraints
of the $\ell\ge2$ axial reduction and hold before compact projection
or expansion in $\epsilon$. In particular $A_\perp$ generally has
mixed Fourier components even though $\pi^r$ is held fixed, and
$\partial_\tau A_z$ need not vanish. The exact assertion is linear in
$\eta$ on each constrained spherical background. An exact family at
finite $\eta$ would also have to solve the backreaction constraints.

For an explicit initial gauge extension take $\beta^z=0$,
$\alpha=\alpha_0\exp(\epsilon\alpha_1/\alpha_0)$,
$\beta^r=\beta_0^r+\epsilon\beta_1^r$ on the radial shift-zero slice,
where the regular completed first jet $h$ gives
$$
 2\alpha_0\alpha_1=-h_{\tau\tau}+2\beta_0^r h_{\tau r}
                   -(\beta_0^r)^2h_{rr},\qquad
 \beta_1^r=(h_{\tau r}-\beta_0^rh_{rr})/a_0^2 .
$$
Use a regular spacetime extension matching the specified first jet
and transform the data and physical slice under subsequent changes of
gauge. In the radial slice chart $F_{rz}=0$ and
$F_{\tau r}=-\alpha a\pi^r/(bR^4)$ on $\Sigma$. Thus setting higher
canonical coefficients to zero does not set all higher field
coefficients to zero. Reflection makes $A_r,\pi^r$ even and
$A_z,\pi^z$ odd in $z$. The half-period translation implements
$\epsilon\mapsto-\epsilon$ for the whole preparation.

For a dual check let $\mathcal Q=R^4*_B F$ with orientation
$d\tau\wedge dr\wedge dz$. The Proca equation gives
$A=(\lambda_{\rm ax}R^2)^{-1}*_B d\mathcal Q$. Define
$\widetilde\pi^i=\sigma R^{-2}(d\mathcal Q)^{ia}n_a$.
The slice Hodge identities are
$$
 \mathcal Q_z=\pi^r,\quad\mathcal Q_r=-\pi^z,\qquad
 A_z=\widetilde\pi^r/\lambda_{\rm ax},\quad
 A_r=-\widetilde\pi^z/\lambda_{\rm ax}.
$$
For example $(*F)_z=\alpha\sigma F^{\tau r}$ verifies the signs.
The matched dual data are $\mathcal Q_z=-rP$,
$\widetilde\pi^z=-\lambda_{\rm ax}A_r$ with other tangential
components zero. Their dual Gauss divergence vanishes, so
$\mathcal Q_\perp=0$. In this slice chart with $\beta^z=0$,
$\partial_\tau\mathcal Q_z=\lambda_{\rm ax}\alpha b R^2 A_r/a+
\beta^r\partial_r\mathcal Q_z$, which equals $-rW$ at $\epsilon=0$.
Direct and dual prescriptions with these matched weighted canonical
values agree exactly. Differences in tails can follow from holding
different data or weights fixed.

Compact higher-order canonical additions propagated by (B2) are
separate preparations. In particular an extra mean pair at $(2,1)$
adds a homogeneous tensor response while preserving lower tangents
and charges. The explicit choice (B1) sets this free pair to zero.

## Pass 3. Mean fixed-mass Z data and propagation quadratures

### Initial radial chart

Use GOAL-005's exact Z family $b=1$, $K^z{}_z=0$,
$R=re^{\epsilon G}$, $d=d_0+\epsilon D$ with $G=R_1/r$,
$D=\kappa_1+q_1/2$. Set $U=a^{-2}$, $Q=r^2q$, $J=r^2\eta_{\rm rad}$
(the GOAL-005 radial extrinsic-curvature component), $w=r(U-U_0)$.
A bracket is compact mean. Here $U_0=r/(r+1)$,
$U_1=-2U_0a_1/a_0$, $Q_1=r^2q_1$, and $J_1=r^2(\eta_{\rm rad})_1$.
The fixed physical GL first jet gives these functions including the
regular inner collar. GOAL-005 CONSTRAINTS (1) applies throughout
Z because $b=1$ and $K^z{}_z=0$ everywhere. Averaging its momentum
equation gives
$$
 \langle Q_2\rangle'=
 \langle (a_1)_zJ_1+2a_0G_zJ_1-2G_rQ_1+2r^2G_rD\rangle,\qquad
 \langle Q_2\rangle(r_i)=0,\qquad \langle J_2\rangle=0. \tag{B3}
$$
Use $\langle aJ_z+2a_zJ\rangle=\langle a_zJ\rangle$ and reflection.
The integrand decays exponentially.

Let $g=\epsilon G$, $H=2/r+2g_r$,
$B=2H_r+H^2+2(1/r+g_r)^2$ and
$\mathcal R_h=2e^{-2g}/r^2-4g_{zz}-6g_z^2$.
The exact Hamiltonian multiplied by $H$ is
$$
 H U_r=U^{-1}U_{zz}+2g_zU^{-1}U_z-\tfrac32U^{-2}U_z^2
        -BU+\mathcal R_h+Z,\quad
 Z=-Q^2/(2r^4)+2dQ/r^2-2J^2/r^4 .
$$
The coefficients of $B$ are $2/r^2$, $4G_{rr}+12G_r/r$ and $6G_r^2$.
Integrating $-U_1U_{1,zz}/U_0^2$ by parts, and writing
$C_Q=-Q_0/r^4+2d_0/r^2$, gives
$$
\begin{aligned}
 (r\langle U_2\rangle)'&=I_2(r),&
 I_2(r)&=\tfrac12r^2[C_Q\langle Q_2\rangle+\mathcal F_2],\\
 \mathcal F_2&=\big\langle
 -2G_r(U_1)_r-(4G_{rr}+12G_r/r)U_1-6G_r^2U_0
 +4G^2/r^2-6G_z^2\\
 &\qquad -Q_1^2/(2r^4)+2DQ_1/r^2-2J_1^2/r^4
 +2G_z(U_1)_z/U_0-(U_1)_z^2/(2U_0^2)\big\rangle .
\end{aligned}\tag{B4}
$$
The potentially nonexponential term $r^2C_Q\langle Q_2\rangle/2$
is $O(r^{-2})$. Fixed ADM mass gives
$$
 \boxed{r\langle U_2\rangle=-\int_r^\infty I_2(\rho)\,d\rho,\qquad
 \lambda_2=\big\langle(a_1/a_0)^2\big\rangle_{r_i}
 +\frac{1}{2r_iU_0(r_i)}\int_{r_i}^\infty I_2(\rho)\,d\rho .}\tag{B5}
$$
Here $\lambda$ is the inner mass knob. The mean spatial metric
coefficient in $rr$ is $-\langle U_2\rangle/U_0^2+
\langle U_1^2\rangle/U_0^3$, and in $AB$ is
$2r^2\langle G^2\rangle\gamma_{AB}$. For $K$ expand
$K_{rr}=a^2(-q/2+d)$, $K_{rz}=a\eta_{\rm rad}$,
$K_{AB}=R^2q\gamma_{AB}/2$ and $K_{zz}=0$. The mean Einstein-frame
areal radius on the Z slice is $r+\epsilon^2r\langle G^2\rangle$.

**Charge expansion on the exact family.** The pinned mass map is
$C^1$, with $\partial_\lambda(M/M_0)(0,0)=2r_iU_0(r_i)=9/14$.
To justify (B5) beyond a formal expansion, first fix $\lambda=0$.
The uniform energy (2) gives $w,Q-Q_0,J=O(|\epsilon|)$ in $H^m$;
(3) gives $w-\langle w\rangle=O(|\epsilon|r^{-3})$ and
$a_z=O(|\epsilon|r^{-4})$. The exact mean momentum forcing is
$\langle a_zJ+2ag_zJ-2g_r(Q-Q_0)+2r^2g_r(d-d_0)\rangle$,
bounded by $O(\epsilon^2[r^{-4}+e(r)])$ with a fixed exponential
envelope $e(r)$. Thus $\langle Q\rangle-Q_0=O(\epsilon^2)$.
In the mean $w$ equation the diffusion becomes
$-\langle(D_{\rm diff})_zw_z\rangle$, with
$D_{\rm diff}=(HU)^{-1}$ and
$(D_{\rm diff})_z=O(w_z)+O(|\epsilon|e(r))$. The resulting term, drift
and gradient square cost $O(\epsilon^2[r^{-6}+e(r)])$.
The term $(1/r-B/H)w$ is $O(\epsilon^2e(r))$. In the remaining
mean forcing the free linear terms have mean zero, and the momentum
terms are $O(\epsilon^2[r^{-2}+e(r)])$ using the estimate for
$\langle Q\rangle-Q_0$. This bounds $|\langle w\rangle'|/\epsilon^2$
by an integrable seed-independent function. Smooth dependence on
finite collars and dominated convergence give the actual coefficient
of $w_\infty$ at fixed $\lambda=0$, including the inner value.
The $C^1$ mass map and mean value theorem then imply
$\lambda(\epsilon)=\epsilon^2\lambda_2+o(\epsilon^2)$ for the exact
fixed-mass family. This establishes the second-order expansion at
the origin without assuming a $C^2$ mass map on a neighborhood.

### Evolved Einstein-frame monopole

In the first-order unitary compact chart set
$p^E_{\mu\nu}=B_{0,\mu\nu}+\bar g_{\mu\nu}B_{0,zz}/2$.
For the full Ricci quadratic coefficient $Q^{\rm Ric}$ of the exact
first GL jet, the conserved Einstein source is
$$
 \mathcal T_{\mu\nu}=-\langle Q^{\rm Ric}_{\mu\nu}\rangle+
 \tfrac12\bar g_{\mu\nu}\langle\operatorname{tr}_5Q^{\rm Ric}\rangle .
$$
In ingoing areal EF gauge $p^E_{vv}=2\mu/r-2f\beta$,
$p^E_{vr}=\beta$, $p^E_{rr}=p^E_{AB}=0$. For
$ds_E^2=b_{ij}dx^i dx^j+r^2d\Omega^2$,
$$
 G_{ij}=-2r^{-1}\nabla_i\nabla_jr+
       [2r^{-1}\Box r-(1-b^{rr})/r^2]b_{ij}.
$$
Substituting $b_{vv}=-e^{2\epsilon^2\beta}(f-2\epsilon^2\mu/r)$,
$b_{vr}=e^{\epsilon^2\beta}$ and $b_{rr}=0$ gives
$$
 \mu_{,r}=-\tfrac12r^2\mathcal T^v{}_v,\qquad
 \mu_{,v}=\tfrac12r^2\mathcal T^r{}_v,\qquad
 \beta_{,r}=\tfrac12r\mathcal T_{rr}. \tag{B6}
$$
A symbolic control with Python 3.14.8 and SymPy 1.14.0 substituted
$e^{\epsilon^2\beta}\to1+e\beta$, $\epsilon^2\mu\to e\mu$,
evaluated $\Box r=-b^{ij}\Gamma^r_{ij}$ and differentiated at $e=0$.
The printed coefficients for $G^v{}_v,G^r{}_v,G_{rr}$ were
$-2\mu_r/r^2$, $2\mu_v/r^2$ and $2\beta_r/r$ respectively.
A separate symbolic check of $(A_x-sA_t)/f$ reproduced (B1).

With mass shift zero and $\beta(v,\infty)=0$, the pinned quadratures
become
$$
 \mu(v,r)=\tfrac12\int_r^\infty \rho^2\mathcal T^v{}_v(v,\rho)\,d\rho,\qquad
 \beta(v,r)=-\tfrac12\int_r^\infty \rho\mathcal T_{rr}(v,\rho)\,d\rho .
 \tag{B7}
$$
The mixed components use the background inverse. Conservation for the
$v$ Killing current reads
$\partial_v(r^2\mathcal T^v{}_v)+\partial_r(r^2\mathcal T^r{}_v)=0$.
It gives the second equation of (B6) from (B7). Along the finite
tilted slice the mass can be found directly as
$$
 \mu(r,r)=\tfrac12\int_r^\infty
 \rho^2(\mathcal T^v{}_v-\mathcal T^r{}_v)(\rho,\rho)\,d\rho . \tag{B8}
$$
Fixed-$v$ integrals can sample the analytic prescribed first jet
$e^{\Omega v}\widehat H$ to the past of the initial slice. For
$\beta$ this fixes an advanced-time gauge, without additional axial
boundary data.

For an explicit mean chart conversion, parameterize the original
slice by its radius $r$ and write its image as
$(v,\rho)=(r+\epsilon^2V_2(r),r+\epsilon^2\rho_2(r))$ in EF gauge,
with $\rho_2=r\langle G^2\rangle$. Pull back the metric of (B6)
on this graph and equate the mean order-two $rr$ coefficient:
$$
 V_2'(r)=
 \frac{\langle\gamma_{rr,2}\rangle-2\mu/r-2s\beta-2\rho_2'
                  + f'\rho_2}{2s}.
 \tag{B9}
$$
Here $\mu,\beta$ are at $v=r$ and $\langle\gamma_{rr,2}\rangle$
is given after (B5). The additive constant specifies the clock
identification. The allowed $O(r^{-2})$ radial metric tail gives
only $V_2'=O(r^{-1})$, so a finite limiting slice displacement at
spatial infinity is not automatic. With $r$ retained as an intrinsic
slice label, keep (B1) on this graph. If it is relabelled by $\rho$,
the mean one-form data obey $A_\rho=(dr/d\rho)A_r$ and the radial
density component obeys $\pi^\rho=\pi^r$. Imposing the same numeric
pair instead on the undeformed graph $v=\rho$ would select other
higher-order data.

The full 5D mean metric additionally includes
$\phi_2=\sqrt3 B_{0,zz}/4$, from the GOAL-005 causal scalar equation
with the Z zero initial pair. The direct mean axial densities are
independent of $\phi_2$: $R=e^{-\phi/\sqrt3}r$,
$B^{ij}=e^{2\phi/\sqrt3}b_E^{ij}$ for the two noncompact indices,
and $B_{zz}=e^{4\phi/\sqrt3}$ leave the orbit volume
$\sqrt{|b_E|}$ and both Proca weights invariant. Reflection removes
the mean compact vector. The $n=\pm2$ spherical coefficient drops
out of the mean when it multiplies the baseline $n=0$ axial data.
All products of the first jet with the mixed axial field still enter.

The exact GL radial decay $e^{-q_\infty r}$ up to powers, with
$q_\infty=\sqrt{k^2+\Omega^2}>\Omega$, gives source decay up to powers
as $e^{2\Omega v-2(q_\infty+\Omega)r}$ in (B7). The integrals are
regular through the future horizon and converge at infinity. Their
decay persists along fixed outgoing cuts $v=u+2x$. The fixed mass
choice removes a homogeneous Schwarzschild mass term. A common
Bondi comparison must also fix the retarded clock and transform the
baseline waveform accordingly.

## Evidence and remaining gates

- **Proved at linear order in the axial amplitude.** Smooth regular
  near-barrier data (B1), the full Gauss and velocity map (B2), and
  equality of the direct and dual matched prescriptions.
- **Proved conditional on the pinned spherical existence and tail
  estimates.** The fixed-mass mean Z constraint quadratures
  (B3)–(B5) and the second-order charge expansion at the origin.
- **Proved identities.** (B6)–(B8) determine the mean Einstein-frame
  metric in the specified gauge at quadratic order. The radion
  cancellation is exact in the pure mean axial densities. No
  strong-field size or waveform is numerically evaluated here.
- **Open.** Mixed massive propagation with certified tails, the
  evaluated near-barrier mean waveform and pole residue, differentiated
  common-frame trace estimates and template tests, and the exact
  nonlinear two-parameter constraints starting at order $\eta^2$.
  The far-front interference sign does not determine the barrier
  signal's sign or magnitude.
