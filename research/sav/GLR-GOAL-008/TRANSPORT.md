# Fixed-source massive transport

## Source and radial evolution

Use \(g_5=e^{-2\phi/\sqrt3}g_E+e^{4\phi/\sqrt3}(dz+2A)^2\), \(G_4=G_5/L\) and \(\varphi=\phi/\sqrt{4\pi G_4}\). The canonical terms \(-m^2\varphi^2/2-J\phi/(4\pi G_4)\) yield \((\Box_E-m^2)\phi=J\), where \(m=m_\phi\), \(\mu=mr_0\). On Schwarzschild, \(f=1-r_0/r\), \(x=r+r_0\log(r/r_0-1)\). The coefficient \(\phi_2\) multiplies \(\epsilon^2\) in the physical field.

Use the audited \(L/r_0=10\) GL jet \(h=e^{\Omega t}H(r)\cos(kz)\), \(k=2\pi/L\), \(s=2\Omega\), with \(H_{\theta\theta}/(2r^2)=1\) at \(2r_0\). In its first-order unitary representative \(h_{zA}=0\), \(R_{zz}=-\partial_z\operatorname{tr}B-\operatorname{tr}B^2\) with \(B=g_4^{-1}\partial_zg_4/2\). Periodic averaging removes the derivative. The quadratic mean is \(-k^2e^{st}H_{\alpha\beta}H^{\alpha\beta}/8\), with the Lorentzian \(-2H_{tr}^2\) term. The linear term is \(-\Box\langle q_{zz}^{(2),U}\rangle/2\), giving \(J_{\rm GL}=-(\sqrt3 k^2/16)e^{st}H_{\alpha\beta}H^{\alpha\beta}\) for \(\phi_2=\sqrt3\langle q_{zz}^{(2),U}\rangle/4\). The gauge completion and vacuum constraint source are inherited. [Pinned source](../GLR-GOAL-005/DERIVATIONS.md)

For \(\psi=r\phi_2\),
\[
 (\partial_t^2+\mathcal H_m)\psi=F,\qquad
 \mathcal H_m=-\partial_x^2+V_m,\qquad
 V_m=f(m^2+r_0/r^3),\qquad F=-rfJ_{\rm GL}=e^{st}S(r).
 \tag{1}
\]
For a scalar multipole \(\ell\), add \(f\ell(\ell+1)/r^2\). The nonnegative self-adjoint operator on \(H^2(\mathbb R_x)\) gives the causal kernel \(\Theta(t)\sin(t\sqrt{\mathcal H_m})/\sqrt{\mathcal H_m}\). Hyperbolic finite propagation preserves the light cone for all masses.

For \(\widetilde F=\int e^{i\omega t}F\,dt\), take the resolvent of \(\mathcal H_m-\omega^2\) from \(\operatorname{Im}\omega>0\). The ingoing horizon branch is \(h_L\sim e^{-i\omega x}\). At positive real \(\omega>m\), \(p=\sqrt{\omega^2-m^2}\) and the unit outgoing branch is \(h_R\sim\exp i[p x+(m^2r_0/(2p))\log(r/r_0)]\), as \(\omega^2-V_m=p^2+m^2r_0/r+O(r^{-3})\). Continue \(p\sim\omega\), \(\operatorname{Im}p>0\) in the upper half-plane. Negative frequencies follow by reality. For \(0<\omega<m\) the right branch is \(e^{-\kappa x}(r/r_0)^{m^2r_0/(2\kappa)}\), \(p=i\kappa\). With \(W=h_Lh_R'-h_L'h_R\),
\[
 G_\omega(x,y)=-h_L(x_<)h_R(x_>)/W.
 \tag{2}
\]
The jump of its first derivative is \(-1\). Constant \(V=m^2\) on the full line gives \(i e^{ip|x-y|}/(2p)\) and time kernel \(\Theta(t-|x-y|)J_0(m\sqrt{t^2-(x-y)^2})/2\). The Bessel equation and the front jump give an independent flat-space check. For compact radial forcing the outgoing amplitude is \(a_m=-W^{-1}\int h_L\widetilde F\,dx\), with a separate contribution from free data. Noncompact forcing needs convergence of the overlap and control of the outer-source term.

## Finite Z data and massless recovery

In this subsection use \(r_0=1\). Set \(u=t-x\), \(v=t+x\), \(g(r)=-r-2\log(r-1)\), \(R(u)=g^{-1}(u)\). On \(v=r\), both null derivatives vanish because the scalar value and normal derivative are zero. Thus
\[
 \psi_m=\mathcal T[F]-\mathcal T[V_m\psi_m],\qquad
 \mathcal T b(u,v)=\tfrac14\int_{g(v)}^u d\hat u
                     \int_{R(\hat u)}^v d\hat v\, b(\hat u,\hat v).
 \tag{3}
\]
One may extend by zero to the past of the slice and restrict the scalar forcing to its future. Zero Cauchy jets remove delta terms at the slice. This operation leaves the vacuum first jet unchanged.

At \(m=0\) the curvature kernel has norm at most \(\kappa_A=[2\log(A/(A-1))-1/A]/4\) for \(u\le u_1=g(A)\), from the ray integral \(1/(4R(u)^2)\). Equation (3) recovers the pinned \(n_Z=\lim_{v\to\infty}\partial_u\psi_0\), including curvature and transient. The independently audited coefficient is \(3.5316\times10^{-7}<2\int_{-10}^{-8}n_Z^2du<4.0171\times10^{-7}\). [Pinned Z waveform](../GLR-GOAL-005/RADIATION.md)

On \(v\le v_*\), the unweighted norm of \(\mathcal T[f\,\cdot]\) is at most \(C=(u_1-g(v_*))(v_*-A)/4\). For \(\kappa_A+\mu^2 C<1\), the resolvent identity gives
\[
 \|\psi_m-\psi_0\|_\infty
 \le \frac{\mu^2 C\|\psi_0\|_\infty}{1-\kappa_A-\mu^2 C}.
\]
One ray integration in each coordinate gives \(O(\mu^2)\) convergence of both null derivatives at fixed \(v_*\), hence of the finite-tube current. The weighted proof reaches the joint regime \(\mu^2 R\to0\) on this early wedge. At fixed \(m>0\) the complete-ray mass integral diverges. [Independent weighted estimate](INDEPENDENT_REVIEW.md#3-finite-slice-massless-recovery-and-a-joint-early-cut-estimate)

## Killing energy and timelike escape

Restore dimensions. The free stress is
\[
 T_{ab}=\frac{\partial_a\phi_2\partial_b\phi_2
 -g_{ab}[(\nabla\phi_2)^2+m^2\phi_2^2]/2}{4\pi G_4}.
\]
For \(\xi=\partial_t\), \(\nabla_a(-T^a{}_b\xi^b)=-J\partial_t\phi_2/(4\pi G_4)\). The physical outward power at \(R\) per \(dt\) is
\[
 \mathcal F_R=-\psi_t(\psi_x-b\psi)/G_4,\qquad b=f/r .
 \tag{4}
\]
The physical energy per \(dx\) is \([\psi_t^2+(\psi_x-b\psi)^2+fm^2\psi^2]/(2G_4)\). The master density \(e=(\psi_t^2+\psi_x^2+V_m\psi^2)/(2G_4)\) and flux \(j=-\psi_t\psi_x/G_4\) obey \(\partial_t e+\partial_x j=F\psi_t/G_4\). Since \(V_m=fm^2+b'+b^2\), physical and master densities differ by \(-\partial_x(b\psi^2)/(2G_4)\), and their fluxes by \(+\partial_t(b\psi^2)/(2G_4)\). Keep the endpoint term on finite cuts. The horizon absorbs nonnegative power \(\psi_v^2/G_4\) for regular ingoing fields. Source work balances change in stored energy plus fluxes at both boundaries. The physical order-four flux carries an overall \(\epsilon^4\). A prescribed \(F\) yields an open scalar energy balance.

Use \(E_R[t_1,t_2]=\int_{t_1}^{t_2}\mathcal F_Rdt\). Signed tube power can include inward return and energy stored nearby. For a completed pulse with a source-free exterior, integrate the net flux over the full transit, then take \(R\to\infty\), or project onto the outgoing channel. With vanishing endpoint term and sufficient spectral integrability, Parseval gives
\[
 \frac{dE_\infty}{d\omega}
 =\frac{\omega\sqrt{\omega^2-m^2}}{\pi G_4}|a_m(\omega)|^2,\qquad \omega>m .
 \tag{5}
\]
The Wronskian current of \(h_R\) equals \(p\). Below threshold the decaying branch is real up to a constant phase and carries zero net spectral flux. Exactly at \(\omega=m>0\), the attractive \(m^2r_0/r\) tail permits \(r^{1/4}\exp(\pm2im\sqrt{r_0r})\) WKB waves. Limiting speed vanishes, and the unit-amplitude above-threshold expansion is singular there. This endpoint supplies no atom for the regular pulse class. At \(m=0\), the integrand is \(\omega^2|a_0|^2/(\pi G_4)\). Use the news transform for memory. The weight \(p/\omega\) compares transport at fixed amplitude. The massive amplitude itself depends on changed overlap and curvature transfer.

For a smooth outgoing compact band above threshold, the large-\(R\) stationary-phase arrival time is
\[
 t=x(R)\frac{\omega}{p}-\frac{m^2r_0\omega}{2p^3}\log(R/r_0)
     +\partial_\omega\arg a_m+o(1).
\]
Away from threshold the relative delay is \(x(R)(\omega/p-1)\simeq x(R)m^2/(2\omega^2)\). At fixed \(m>0\) and bounded \(u=t-x(R)\), the phase derivative stays bounded away from zero in units of \(R\). Integration by parts makes the band field and its first derivatives vanish on these cuts. At \(m=0\) the same band generally has a nonzero null trace. This proves noncommutation in the stated class. Broad signals need control of frequencies outside the band. The unended growing GL forcing is outside the finite-energy scattering problem. Along \(t/x(R)\to1/v>1\), the stationary phase selects \(v=p/\omega\). Equation (5) can be pushed forward to speed or arrival-time ratio. Full-time escape may be continuous at zero mass under dominated convergence of the energy integrand, independently of the fixed-cut discontinuity.

## Storage, quasi-bound response and late tails

For \(y=r/r_0\), potential extrema satisfy \(\mu^2y^3-3y+4=0\). A barrier and outer well exist for \(0<\mu<1/2\), with extrema near \(4/3\) and \(\sqrt3/\mu\) at small mass. They coalesce at \(y=2,\mu=1/2\); at larger mass there is no well. In the Coulomb outer approximation the binding scale is \(\kappa\simeq m^2r_0/(2N)\) and \(\operatorname{Re}\omega_N/m\simeq1-\mu^2/(8N^2)\) at fixed principal \(N\ge1\). Horizon matching determines resonances and residues. Wronskian conservation precludes a nonzero real mode simultaneously ingoing and decaying. Positivity rules out an \(L^2\) growing mode and, by integration by parts, a horizon-regular static decaying mode. Damped quasi-bound excitations can store energy and feed the horizon.

At fixed positive mass the compact-data fixed-radius theorem gives an oscillatory \(t^{-5/6}\) leading term for nonzero data functional and an \(O(t^{-1+\delta})\) remainder in its stated domain. The weak-curvature intermediate monopole \(t^{-3/2}\) regime and massless Price decay have different assumptions. No late-time onset or resonant residue is evaluated for the Z GL signal. [Primary sources and domains](LITERATURE.md)
