# Stabilization, constraints, and conversion

The conventions and vacuum perturbation geometry are those of [GOAL-005 at `61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708`](https://github.com/inaeyk/GL-with-AI/tree/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav). The results below concern the specified extra matter theories and their reduced limits. The Casimir construction gives a perturbatively stabilized Minkowski circle. Its curved-space GL background and response remain to be computed.

## 1. Classical consistency tests

Let the circle coordinate have period \(L\), use signature \((-++++)\), and normalize \(M_5^3=(8\pi G_5)^{-1}\), \(M_4^2=L M_5^3=(8\pi G_4)^{-1}\). For a smooth four-dimensionally Poincaré-invariant warped circle \(ds^2=e^{2A(y)}\eta_{\mu\nu}dx^\mu dx^\nu+dy^2\), choose \(k=e^{-A}\partial_t+\partial_y\). The warped-product Ricci formulas give

\[
R_{\mu\nu}=-(A''+4A'^2)g_{\mu\nu},\qquad R_{yy}=-4(A''+A'^2),\qquad R_{AB}k^Ak^B=-3A''.
\]

Einstein's equation and the five-dimensional null energy condition give \(A''\le0\). Smooth periodicity makes \(A\) constant. The flat product then requires the **total** stress to vanish. A canonical scalar winding has \(T_{AB}k^Ak^B=(k\cdot d\chi)^2>0\). A cosmological term drops out of this contraction and cannot rescue the winding, even after adjusting its sign and magnitude. This argument assumes smoothness and the NEC, and proves an obstruction for canonical winding under those assumptions. An arbitrary healthy stabilization mechanism can evade the hypotheses. Pure vacuum Einstein has every circumference as a flat-product solution, with an exactly flat radion potential. Local curvature terms evaluated on the flat products are insensitive to the circumference apart from the volume multiplying a cosmological term.

An exact but degenerate classical example illustrates why solving the background equations is only one test. Add \(S_\chi=\int\sqrt{-g_5}P(X)\), \(X=g^{AB}\partial_A\chi\partial_B\chi\), with compact scalar winding \(\chi=qz\), \(qL=2\pi N f_\chi\). Its equations are

\[
T_{AB}=P g_{AB}-2P_X\partial_A\chi\partial_B\chi,\qquad \nabla_A(P_X\nabla^A\chi)=0.
\]

At \(X_0=q^2\), choose \(P=P_X=0\) and \(P_{XX}<0\). Every Ricci-flat four-metric times the equilibrium circle then solves the exact field equations, including Schwarzschild. For \(g_{zz}=e^{4\phi/\sqrt3}\) the reduced potential is \(V=-L e^{-2\phi/\sqrt3}P(X_0e^{-4\phi/\sqrt3})\), with \(m_\phi^2=-64\pi G_5 X_0^2P_{XX}/3>0\). The quadratic matter action is proportional to

\[
\tfrac12 P_{XX}(2q\partial_z\pi-q^2 h_{zz})^2,\qquad \pi=\delta\chi.
\]

There is no quadratic time kinetic term for the winding fluctuation. In the zero-mode sector the vector and \(\pi\) enter through the diffeomorphism-invariant combination \(\partial_\mu\pi-2qA_\mu\), whose quadratic kinetic or Stückelberg mass term vanishes at \(P_X=0\). The dependence returns at higher order. For example on the flat fixed metric and a \(z\)-independent \(\pi\), \(X-X_0=(\partial_\mu\pi)^2\) and the leading self-interaction is quartic. Thus the naive two-derivative theory supplies no regular weakly coupled fluctuation expansion for this sector. For nonzero \(n\) the linear \(\chi\) equation imposes \(\partial_z\delta X=0\), so \(\delta X_n=0\) and \(\delta T_{AB,n}=0\). The resulting formal zero GL eigenvalue shift is a property of this degenerate model. Restoring a controlled matter kinetic sector would require specifying additional operators and redoing the constraint and stability analysis.

## 2. A smooth semiclassical Minkowski circle

Consider 5D Einstein gravity with renormalized bulk vacuum energy density \(\rho_5\) and two free massive Dirac fermions of mass magnitude \(M\), periodic around the spatial circle. Choose opposite mass signs and exchange the fields under reflection to permit a reflection-symmetric regulator and state. In Lorentzian signature the local action is

\[
S_5=\int d^5x\sqrt{-g_5}\left[\tfrac12 M_5^3 R_5-\rho_5+\sum_{s=\pm}\bar\Psi_s(i\slashed\nabla-sM)\Psi_s\right]+S_{\rm ct}.
\]

Here \(S_{\rm ct}\) includes local covariant gravitational counterterms, with the flat-space vacuum energy already counted in \(\rho_5\). The construction is a gravitational EFT using the Poincaré-invariant circle vacuum, retaining the finite winding part of the one-loop determinant. The flat graviton and ghost determinant gives the count of five physical bosonic polarizations. Each 5D Dirac determinant gives the opposite sign and four times the real-scalar contribution, from squaring the flat Dirac operator inside the trace logarithm. The opposite masses give the same parity-even contribution. The smooth-circle use of competing massless and massive Casimir energies with a bulk cosmological counterterm is discussed by [Pontón and Poppitz, hep-ph/0105021v3, §2.2 (smooth-circle discussion) and §2.4 (massive determinant)](https://arxiv.org/abs/hep-ph/0105021v3). The winding derivation here specifies a particular matter count and tuning.

For a single real periodic scalar on a flat product of proper circumference \(\ell\), use the Schwinger logarithm in \(\tfrac12\operatorname{Tr}\log(-\partial^2+M^2)\). Integration over the four Euclidean momenta and Poisson resummation give the Jordan-frame energy per unit four-volume

\[
W_b(\ell,M)=-\frac{\ell}{2(4\pi)^{5/2}}
 \sum_{w\ne0}\int_0^\infty \frac{ds}{s^{7/2}}
 e^{-sM^2-w^2\ell^2/(4s)}
=-\frac{3}{4\pi^2\ell^4}\sum_{w=1}^\infty\frac{h(wM\ell)}{w^5},
\qquad h(y)=e^{-y}(1+y+y^2/3).
\]

The \(w=0\) term is local and included in the bulk renormalization. The second equality uses the elementary \(K_{5/2}\) integral, and gives \(-3\zeta(5)/(4\pi^2\ell^4)\) at \(M=0\). With \(A=5\), \(B=8\), \(C=3\zeta(5)/(4\pi^2)\) and \(F(x)=\zeta(5)^{-1}\sum_{w\ge1}w^{-5}h(wx)\), the complete one-loop static energy and its Einstein-frame version are

\[
W(\ell)=\rho_5\ell+C\ell^{-4}[-A+B F(M\ell)],\qquad
V(\phi)= (L/\ell)^2 W(\ell),\qquad \ell=L e^{2\phi/\sqrt3}.
\]

In the orthonormal product frame the one-loop total stress including \(\rho_5\) has components \(T^\mu{}_{\nu}=-(W/\ell)\delta^\mu_\nu\) and \(T^z{}_z=-W'\), from varying the external volume and the proper length independently at fixed winding boundary conditions. There is no mixed stress in the symmetric vacuum. Hence the full flat-product Einstein equation is \(W=W'=0\), equivalent to \(V=V_\phi=0\) at \(\phi=0\). These conditions require

\[
B[5F(x)-xF'(x)]=5A,\qquad
\rho_5=C L^{-5}[A-BF(x)]=-\frac{CB}{5L^5}xF'(x),\qquad x=ML.
\]

They tune the positive renormalized bulk energy. To establish a stable solution, \(h'=-e^{-y}(y+y^2)/3\), \(h''=e^{-y}(y^2-y-1)/3\), and

\[
y^2h''-4y h'=\tfrac13 e^{-y} y^2(y^2+3y+3)>0\quad (y>0).
\]

It follows that \(5F-xF'\) is strictly decreasing from 5 to 0. Since \(B>A>0\), there is exactly one positive stationary zero with the indicated tuning, and

\[
W''(L)=CB L^{-6}[x^2F''-4xF']>0,\qquad
m_\phi^2=4\pi G_4 V_{\phi\phi}(0)=\frac{16\pi G_4}{3}L^2W''(L)>0.
\]

The mass formula uses the leading Einstein-frame kinetic coefficient. Corrections to it belong to the loop expansion. For \(G_5/L^3\ll1\) at fixed species count, higher loops are perturbations at the compactification scale. The Jacobian of \((W,W')\) with respect to \((\rho_5,\ell)\) has determinant \(LW''\) at the tuned point. This permits retuning the local vacuum order by order, assuming finite perturbative renormalized couplings. Positivity here concerns the small radion fluctuation about this Minkowski vacuum, without a claim of nonperturbative absolute stability or UV completeness.

A bounded standard-library check (Python 3.14.8) evaluates the dimensionless parameters. With truncations 32, 64, and 128 the root is respectively 2.2294019862, 2.2294014468, and 2.2294014120. The final truncation gives \(\rho_5 L^5\simeq0.07165806\), \(L^6W''\simeq1.64470787\), and \(m_\phi^2 L^2\simeq27.557345\,G_5/L^3\). These are truncated-sum diagnostics for the flat vacuum, with the sign and uniqueness established analytically above.

```python
import math

for nmax in (32, 64, 128):
    zeta = sum(n**-5 for n in range(1, nmax + 1))
    def sums(x):
        f = fp = fpp = 0.0
        for n in range(1, nmax + 1):
            y = n * x
            e = math.exp(-y) / n**5
            f += e * (1 + y + y*y/3)
            fp -= e * n * (y + y*y)/3
            fpp += e * n*n * (y*y - y - 1)/3
        return f/zeta, fp/zeta, fpp/zeta
    lo, hi = 0.0, 20.0
    for _ in range(70):
        x = (lo + hi) / 2
        f, fp, fpp = sums(x)
        if 8 * (5*f - x*fp) > 25:
            lo = x
        else:
            hi = x
    c = 3 * zeta / (4 * math.pi**2)
    w2 = 8 * c * (x*x*fpp - 4*x*fp)
    print(nmax, x, c*(5-8*f), w2, 16*math.pi*w2/3)
```

## 3. Reduced equations and constraints

For the zero-mode metric use

\[
ds_5^2=e^{-2\phi/\sqrt3}ds_{4,E}^2+e^{4\phi/\sqrt3}(dz+2A)^2.
\]

At leading two-derivative order the reduced model has

\[
S_4=\int \sqrt{-g}\left[\tfrac12 M_4^2 R-M_4^2 (\nabla\phi)^2
 -\tfrac12 M_4^2 Z(\phi)F^2-V(\phi)\right]+S_{\rm ret},\qquad Z=e^{2\sqrt3\phi}.
\]

Here \(S_{\rm ret}\) denotes the additional fields or matched operators retained in a proposed application. The displayed kinetic terms are the tree-level circle reduction. Renormalized two- and higher-derivative operators also follow from a specified microscopic model. With \(J_{\rm ret}=-\delta S_{\rm ret}/(2M_4^2\sqrt{-g}\,\delta\phi)\), the scalar equation reads

\[
\Box\phi=\frac{\sqrt3}{2}ZF^2+\frac{V_\phi}{2M_4^2}+J_{\rm ret}.
\]

The Einstein stress is \(T^{\phi}_{\mu\nu}=2M_4^2[\nabla_\mu\phi\nabla_\nu\phi-\tfrac12 g_{\mu\nu}(\nabla\phi)^2]-g_{\mu\nu}V\), \(T^A_{\mu\nu}=2M_4^2 Z(F_{\mu\alpha}F_\nu{}^\alpha-\tfrac14g_{\mu\nu}F^2)\), plus \(T^{\rm ret}\). For \(F=0\), \(\nabla^\mu T^\phi_{\mu\nu}=2M_4^2 J_{\rm ret}\partial_\nu\phi\). Diffeomorphism invariance gives the opposite exchange from the retained on-shell sector. The isolated zero-mode action has the exact solution \(\phi=F=0\) and any Ricci-flat four-metric at the tuned minimum. The Hamiltonian constraint for induced three-metric \(\gamma\) and \(K_{ij}=-\tfrac12(\mathcal L_n\gamma)_{ij}\) has \({}^{(3)}R+K^2-K_{ij}K^{ij}=16\pi G_4\,T_{nn}\), with

\[
T_{nn}=M_4^2\big[(n\cdot\partial\phi)^2+(D\phi)^2+Z(E^2+B^2)\big]+V+T^{\rm ret}_{nn}.
\]

The momentum constraint is \(D_j(K^{ij}-\gamma^{ij}K)=8\pi G_4 j^i\) with \(j_i=-n^\mu T_{\mu i}\) in slice components. The scalar contributes \(-2M_4^2(n\cdot\partial\phi)D_i\phi\). Neutral Maxwell Gauss is \(D_i(ZE^i)=0\), where \(E_i=F_{i\mu}n^\mu\); retained charged fields contribute their Gauss source. A prescribed radion source alone represents external work on the scalar, with no corresponding matched Einstein stress.

In the semiclassical five-dimensional theory the background equation is \(G^{(5)}_{AB}=8\pi G_5 T^{\rm eff}_{AB}\), with \(T^{\rm eff}_{AB}=-\rho_5 g_{AB}+\langle T_{AB}\rangle_{\rm ren}\) including all loop and local counterterm variations placed on the right. Its normal projections have the same ADM form with four-dimensional induced spatial metric, \(G_5\), and \(T^{\rm eff}_{NN},-N^A T^{\rm eff}_{AI}\) as constraint sources. This projection is an identity for the effective field equations and does not replace computing the stress. The flat-space determinant supplies the tuned flat projections only. A causal perturbed problem needs the state-dependent retarded stress response and its diffeomorphism identity, including the state perturbation if one is present.

## 4. Leading interactions and symmetry

Put \(\varphi=\sqrt2 M_4\phi\), \(\mathcal A=\sqrt2 M_4 A\), and \(g=\bar g+2h/M_4\) for the canonically normalized radion, photon and graviton at the minimum. Expanding the displayed action gives

\[
\mathcal L_{\varphi \mathcal A\mathcal A}=-\frac{\sqrt6}{4M_4}\varphi\,\mathcal F_{\mu\nu}\mathcal F^{\mu\nu},\qquad
\mathcal L_{h\varphi\varphi}=\frac{h_{\mu\nu}}{M_4}T^{\mu\nu}_{\rm free\ \varphi}.
\]

A single radion has no \(\varphi hh\) vertex in this two-derivative Einstein-frame action at a stationary Minkowski minimum. Higher-curvature operators can change that vertex count. The neutral Maxwell equation is homogeneous in \(F\), so a nonzero classical radion field can modulate or amplify a seeded Maxwell field, and \(F=0\) remains an exact neutral solution. The cubic vertex also permits a quantum decay to two photons in flat space if a massive radion exists. Such a particle decay is a different calculation from deterministic classical GL radiation.

At tree level the circle-gradient term gives a KK spin-2 squared mass \(m_n^2(\phi)=k_n^2 e^{-2\sqrt3\phi}\), \(k_n=2\pi n/L\), by taking the ratio of the inverse metric in the compact and external directions. For a real canonically normalized component \(H\), the Fierz–Pauli term \(-\tfrac12m_n^2(\varphi)(H_{\mu\nu}H^{\mu\nu}-H^2)\) contains a radion cubic with coefficient \(+\sqrt6 k_n^2/(2M_4)\). On the circle the invariant product pairs Fourier modes \(n,-n\), with the conventional reality and normalization factors. This mass derivative is a universal tree-level coupling at constant radion. Derivative, nonlinear gauge completion, and stabilizer terms must also be included for a general on-shell amplitude or an inhomogeneous GL source. The photon gauges compact translations and couples to the charged modes. In the convention \(z'=z+\xi(x)\), \(A'=A-\tfrac12d\xi\), \(H_n'=e^{-ik_n\xi}H_n\), the covariant derivative has the term \(-2ik_n A H_n\).

If the action and the background preserve circle translations, the harmonic products \((n,-n)\to 0\) and \((n,n)\to 2n\) are allowed. A zero-mode radion alone cannot source a single nonzero harmonic from exactly zero KK data. It can scatter an existing harmonic without changing its compact momentum. At early GL order, the seeded GL mode is first order, the zero-mode radion second order, and their product feeds back into the first harmonic at third order. A quantum radion decay into a KK pair additionally needs \(m_\phi\ge 2 m_n\) in flat space (strictly greater for nonzero phase space), which fails for the light radion in the perturbative Casimir example. Classical transients use their actual frequency spectrum and can probe other interactions in the tower.

For exact transverse \(SO(3)\), an invariant symmetric tensor tangent to a round cut is proportional to its sphere metric. The traceless Bondi shear and news are therefore identically zero, even with spherical scalar stress. Equivalently there are no spin-weight two invariant harmonics. The only invariant Maxwell fields have radial electric or magnetic components, with no invariant tangent one-form and no radiative flux. If compact reflection also holds, \(A\) is odd and the entire zero-mode vector vanishes for a reflection-symmetric standing GL mode and data. The monopole scalar is allowed. A massive KK spin-2 field can have a spherically symmetric propagating sector because its longitudinal polarization and radial components are physical. Compact momentum and angular symmetry allow that channel, while an outgoing excitation still requires real frequency above its asymptotic mass and a nonzero source/data projection.

## 5. What remains before a stabilized GL prediction

The flat compactification determines an infrared radion mass and a two-derivative outer EFT. In the GL regime \(r_0/L\lesssim 1/(2\pi/0.876)\), and the unstable Fourier mode carries circle momentum \(2\pi/L\). A zero-mode derivative expansion alone cannot compute the near-horizon or nonzero-harmonic determinant response. In particular, substituting \(V\) in the reduced equations and observing their Schwarzschild solution supplies no proof that the full semiclassical stress vanishes on Schwarzschild times a circle. It would require a compatible quantum state, renormalized curved-space stress and background correction. A thermal equilibrium state at nonzero temperature has an asymptotic bath, and a collapse-like state with Hawking flux would require a slowly evolving backreacted solution. The flat circle vacuum calculation gives neither state on the horizon. A retarded response kernel at nonzero harmonic and the perturbed normal projections would then enter the GL eigenproblem. Since the effective action contains curvature operators beyond the flat potential, \(m_\phi^2\) alone fixes neither this response nor its sign. The Casimir estimate gives \(\mu^2\simeq27.56(G_5/r_0^3)(r_0/L)^5\). A loop-scale horizon correction need not carry the extra \((r_0/L)^5\) factor. A choice of local EFT couplings and a perturbatively controlled curvature regime are separate inputs.

A conditional shift formula makes the missing ingredients precise. Suppose a family of backgrounds and states is given, with a stationary description valid on the GL time interval, and a retarded, constraint-reduced linear operator has a differentiable Evans determinant \(D(s,k;\eta)\), where \(\eta\) measures the stabilizer deformation. Fix the horizon and asymptotic charge conventions and include ingoing regularity and decaying infinity data in \(D\). At a simple isolated unstable root \(\Omega_0\), \(\partial_\eta\Omega=-D_\eta/D_s\). For a simple static threshold root viewed as a root in \(k\), \(\partial_\eta k_c=-D_\eta(0,k_c;0)/D_k(0,k_c;0)\). Both formulas are implicit differentiation. Evaluating \(D_\eta\) requires the corrected background, constraints, linear matter response and boundary conditions. If mixing with additional modes or branch points invalidates simplicity, these formulas require replacement. A time comparison needs \(|\delta\Omega|T\ll1\) or a resummed and controlled rate. The second-order source also changes through the GL eigenfunction and stabilizer variations, and a first-bulge comparison needs a nonlinear remainder and constrained initial data. None of those follow from the flat minimum or the formal degenerate winding ansatz.
