# Semiclassical stabilization and the GL matching gate

The GOAL-008 Casimir candidate fixes a healthy leading flat-circle radion. For a specified Unruh prescription, outgoing Dirac power obstructs a stationary, reflection-symmetric spherical string at first backreaction order. Even in a stationary matching, the flat mass leaves a curvature coupling unmeasured. The fixed-ADM static threshold has a strictly positive directional derivative in the local Gauss–Bonnet direction.

## Inputs and conventions

- GOAL-008 source [4aac5ad001a9d6c63efa86640ce05837282fb73f](https://github.com/inaeyk/GL-with-AI/blob/4aac5ad001a9d6c63efa86640ce05837282fb73f/research/sav/GLR-GOAL-008/STABILIZATION.md), §§1–3 and 5.
- Read-only GOAL-008 Work audit [a54aacd959711945edb12668e1d26edb1b9bde33](https://github.com/inaeyk/GL-with-AI/blob/a54aacd959711945edb12668e1d26edb1b9bde33/research/sav/verification/AUDIT_GOAL008_MASSIVE_RADION_4aac5ad0.md), §6. It already identified the Gauss–Bonnet background ambiguity. The nonzero-harmonic insertion and signed fixed-ADM threshold derivative below extend it.
- GOAL-005 [61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/tree/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005) is inherited at the GOAL-014 source pin [0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195). The static critical mode below and the growing benchmark at $k_1r_0=\pi/5$ are distinct spectral problems.

Use $c=\hbar=1$, signature $(-++++)$, $G_4=G_5/L$, $M_4^2=(8\pi G_4)^{-1}$, $M_{\rm ADM}^{(0)}=Lr_0/(2G_5)$, $f=1-r_0/r$, and the asymptotically unit Schwarzschild Killing clock. The coordinate $z$ has period $L$. In the circle-invariant sector with vanishing vector, the radion convention is $g_{zz}=e^{4\phi/\sqrt3}$ and $g^{(5)}_{\mu\nu}=e^{-2\phi/\sqrt3}g^E_{\mu\nu}$.

## Flat-circle kinetic and stress test

The candidate consists of Einstein gravity, renormalized bulk vacuum density $\rho_5$, and two ordinary 5D Dirac fields of masses $+M,-M$ with periodic spatial spin structure. Compact reflection exchanges the fields. The EFT also has local covariant counterterms. Use the reflection-invariant Poincaré vacuum on the flat product. The nonzero windings of the flat proper-time determinant of a real periodic scalar give $-3\sum_{w\ge1}w^{-5}h(wM\ell)/(4\pi^2\ell^4)$ per unit external Jordan volume, where $h(y)=e^{-y}(1+y+y^2/3)$ and $\ell$ is the proper circumference. The graviton and ghost determinant gives five real massless scalar weights. Each free Dirac gives four opposite scalar weights from the spin trace and Grassmann determinant sign. Thus
\[
 W(\ell)=\rho_5\ell+C\ell^{-4}[-5+8F(M\ell)],\qquad
 C=\frac{3\zeta(5)}{4\pi^2},\qquad
 F(x)=\frac{1}{\zeta(5)}\sum_{w\ge1}\frac{h(wx)}{w^5}.
\]
The zero winding is local and belongs to bulk renormalization. The homogeneous total mixed stress has external entries $-W/\ell$ and compact entry $-W'$. Both vanish at a Minkowski solution. Eliminating $\rho_5$ gives $8(5F-xF')=25$, $x=ML$. The identity $y^2h''-4yh'=e^{-y}y^2(y^2+3y+3)/3>0$ makes $5F-xF'$ decrease strictly from $5$ to $0$, so there is a unique positive $x$. At the root
\[
 W''(L)=8CL^{-6}(x^2F''-4xF')>0,\qquad
 V(\phi)=(L/\ell)^2 W(\ell),\qquad \ell=Le^{2\phi/\sqrt3}.
\]
Reduction of the Einstein term gives $-M_4^2(\nabla\phi)^2$, or the canonical $\varphi=\sqrt2M_4\phi$, and hence $m_\phi^2=V_{\phi\phi}(0)/(2M_4^2)=16\pi G_4L^2W''(L)/3$. The local kinetic sign is positive at tree level and persists for renormalized kinetic corrections small compared with $M_4^2$. The full curved constrained kinetic operator and retarded response remain open. This flat reconstruction had prior exposure to the pinned source and audit. [Pontón and Poppitz, JHEP 06 (2001) 019, hep-ph/0105021v3, §§2.2, 2.4 and Eqs. (18)–(20)](https://arxiv.org/pdf/hep-ph/0105021v3)

The Work audit gives $ML\simeq2.22940$ and $\mu^2=(m_\phi r_0)^2\simeq27.55734(G_5/r_0^3)(r_0/L)^5$. At $L/r_0=10$, the near-string loop condition $G_5/r_0^3\ll1$ implies $\mu\ll0.01660$. The test-field sweep values $\mu=0.10,0.20$ would require $G_5/r_0^3$ near $36.3,145$, respectively, in this candidate. Also $Mr_0\simeq0.223$, $k_1/M\simeq2.82$, and $\mathcal K_4^{1/4}/M\simeq8.35$ at the horizon, where $\mathcal K_4=12r_0^2/r^6$. Weak gravity loops and a light infrared radion can coexist with the failure of a heavy-Dirac derivative expansion on the GL scale.

## Stationary obstruction for the specified collapse-state prescription

Suppose the exterior deformation preserves sphere rotations, circle translations and circle reflection. A stationary metric with these symmetries has only a possible $dt\,dr$ cross term. Where the stationary Killing field is timelike, a shift of $t$ by a function of $r$ removes it. Time reflection is then a metric isometry. Every metric-only covariant time-even tensor in the field equation has zero mixed radial Killing-energy current, including $G^r{}_t$ and the Gauss–Bonnet $H^r{}_t$.

Choose the ordinary Unruh prescription for the free Dirac sector on the unperturbed Schwarzschild string, with no incoming quanta from infinity and the horizon channel populated with the Fermi factor at $T_H=(4\pi r_0)^{-1}$. The $n=0$ sector already contains a massive four-dimensional Dirac field. For an angular channel $\kappa_j=\pm(j+1/2)$ its outward power contains
\[
 P_{\kappa_j,0}=\frac{d_{\kappa_j}}{2\pi}
    \int_M^\infty\frac{\omega\,\Gamma_{\kappa_j,0}(\omega)}
                        {e^{4\pi r_0\omega}+1}\,d\omega>0,\qquad
 d_{\kappa_j}>0 .
\]
This is large timelike tube power with the propagation threshold $\omega>M$. The mode count uses $v_g\,dp=d\omega$. [Page, Phys. Rev. D 13 (1976) 198, §II, Eqs. (4), (6), (11)–(12)](https://doi.org/10.1103/PhysRevD.13.198)

For the strict sign, a choice of radial spinor basis gives the Dirac Hamiltonian $-i\sigma_2\partial_x+\sqrt f(M\sigma_3+\kappa_j\sigma_1/r)$ with $x=r+r_0\log(r/r_0-1)$ and conserved current $u^\dagger\sigma_2u$. At the horizon the nonconstant coefficients decay exponentially in $x$. Normalize the regular ingoing solution to current $-1$. At large $r$ the first-order system has separated eigenvalues $\pm i q(r)$, $q^2=\omega^2-f(M^2+\kappa_j^2/r^2)$. Diagonalization gives an integrable $O(r^{-2})$ change-of-basis derivative, and removing the phases $\pm\int q\,dx$ preserves that bound. Thus the amplitudes of the ingoing horizon solution at infinity are finite and satisfy $|a|^2-|b|^2=1$ in unit-current conventions. Its absorption probability $|a|^{-2}$ is positive. Unitarity of the two-channel flux-normalized scattering matrix makes the reciprocal transmission probability equal. These arguments are uniform on compact frequency bands above threshold. In Painlevé–Gullstrand coordinates the conserved Dirac Wronskian gives the same sign, with a strictly negative horizon current on the regular branch and the difference of squared infinity amplitudes. [Doran, Lasenby, Dolan and Hinder, Phys. Rev. D 71 (2005) 124020, inspected gr-qc/0503019v1 §§2–4, Eqs. (16), (18)–(25)](https://arxiv.org/pdf/gr-qc/0503019)

With no compensating inward power from other sectors, $-\int_{S^2\times S^1}r^2T^r{}_t\,d\omega_2\,dz>0$. At first backreaction order this stress is evaluated on the vacuum geometry. A first-order metric deformation cannot cancel it by changing the Dirac propagator at the same order, and local time-even counterterms have zero flux. Hence the stationary semiclassical equation is inconsistent for this state prescription and symmetry class. A positive-temperature equilibrium prescription introduces an asymptotic bath. The flat graviton determinant leaves the curved prescription for the unstable harmonic open. A quasi-static isolated comparison would need time-dependent backreaction and a bound on its effect at the order of the proposed GL shift. [Candelas, Phys. Rev. D 21 (1980) 2185, inspected publisher abstract on the three states for massless fields](https://doi.org/10.1103/PhysRevD.21.2185)

## A curvature coupling invisible to the flat tuning

Consider the finite local action variation
\[
 \delta S_5=\frac{\alpha}{16\pi G_5}\int \sqrt{-g_5}\,\mathcal G_5,\qquad
 \mathcal G_5=R_{ABCD}R^{ABCD}-4R_{AB}R^{AB}+R^2,\qquad |\alpha|/r_0^2\ll1 .
\]
The equation is $G_{AB}+\alpha H_{AB}=8\pi G_5T_{AB}$. The Lovelock tensor $H$ is quadratic in curvature, so it and its first variation vanish about every flat circle. This leaves the entire retained flat one-loop potential unchanged since the flat quadratic fluctuation determinant is unchanged. This is a comparison of finite renormalized couplings within the same EFT field content, on the branch continuous with Einstein gravity.

On Schwarzschild times the circle the only curvature is four-dimensional. The four-dimensional Lovelock identity gives $H_{\mu\nu}=0$, and $H_{zz}=-\mathcal K_4/2=-6r_0^2/r^6$. The first-order Einstein-frame radion $\phi=\alpha p$ with horizon regularity and fixed circumference obeys
\[
 (r^2fp')'=-\frac{2\sqrt3 r_0^2}{r^4},\qquad
 p(r)=\frac{2}{\sqrt3 r_0}\left(\frac1r+\frac{r_0}{2r^2}+\frac{r_0^2}{3r^3}\right).
 \tag{1}
\]
The first variation in $\phi$ of the reduced Gauss–Bonnet action at $\phi=0$ is that of $\alpha e^{2\phi/\sqrt3}\mathcal G_4$, up to divergences. This also follows directly from $H_{zz}$, $H_{\mu\nu}=0$ and the metric Weyl factors. Varying the tree scalar action gives $\Box\phi=-\alpha\mathcal G_4/(2\sqrt3)$. Horizon regularity sets $r^2fp'=0$ at $r=r_0$, and fixed circumference sets $p(\infty)=0$. The first-order Einstein-frame metric obeys the linear spherical vacuum equation. The radion kinetic stress is quadratic in $\phi$, and the coefficient of the four-dimensional Euler density in the Einstein-metric variation is constant at order $\alpha$. Fixed ADM mass and asymptotic clock then fix the Einstein metric to Schwarzschild up to diffeomorphism. For $\phi=a_\phi/r+o(r^{-1})$, the five-dimensional ADM mass combination is $2c_t-c_z=2(r_0+2a_\phi/\sqrt3)-4a_\phi/\sqrt3=2r_0$. The Einstein-frame radius and clock keep the horizon and Euclidean period fixed to first order. Independently, the published pure Gauss–Bonnet $g_{zz}$ in Eq. (3.25) agrees with $1+4\alpha p/\sqrt3$ after setting $\alpha=\alpha_{\rm BDR}/4$. An $O(\alpha)$ change of radial coordinate or horizon parameter does not alter this comparison because the background $g_{zz}$ is constant. [Brihaye, Delsate and Radu, JHEP 07 (2010) 022, 1004.2164v2, Eqs. (2.1)–(2.4), (2.7), (3.25)](https://arxiv.org/pdf/1004.2164v2)

For $h_{\mu\nu}e^{ikz}$ with $h_{zA}=0$ on the vacuum product, the entire $D H_{\mu\nu}$ from variations of external curvatures at fixed $z$ vanishes by the four-dimensional Lovelock identity. Contractions linear in a curvature component with one compact index vanish against the background. The term $-4R_{\mu\rho\nu\sigma}\delta R^{\rho\sigma}$ sees the extra $-\partial_z^2h^{\rho\sigma}/2$ in the five-dimensional Ricci variation. Hence
\[
 (D H_{\mu\nu})_{\bar g}[h]=2R_{\mu\rho\nu\sigma}\partial_z^2h^{\rho\sigma}
                          =-2k^2 R_{\mu\rho\nu\sigma}h^{\rho\sigma}.
 \tag{2}
\]
In a Lorentzian orthonormal frame put $c=r_0/(2r^3)$ and order the spherical components $(h_{tt},h_{rr},h_{\theta\theta}=h_{\varphi\varphi},h_{tr})$. With $R_{0i0j}=c\,\mathrm{diag}(-2,1,1)$ the curvature contraction in (2), divided by $c$, is
\[
 \begin{pmatrix}
 0&-2&2&0\\-2&0&-2&0\\1&-1&2&0\\0&0&0&-2
 \end{pmatrix}.
\]
Its kernel is the pure metric trace. The tracefree vacuum GL mode has a nonzero insertion wherever it is nonzero. The projected growth-rate derivative at positive growth still requires the rest of the perturbed operator and its left null functional.

### Signed static threshold derivative

Use Euclidean Schwarzschild, smooth at the bolt with period $4\pi r_0$, and its positive $L^2$ tensor pairing $\langle\, ,\,\rangle$. Let $h$ be a nonzero regular, decaying TT Schwarzschild negative mode with
\[
 \Delta_L h=-k_c^2 h,\qquad \Delta_L=-\nabla_E^2-2\mathcal R,\qquad
 (\mathcal R h)_{\mu\nu}=R_{\mu\rho\nu\sigma}h^{\rho\sigma}.
\]
This is the static vacuum GL mode after Wick rotation and stripping the $e^{ik_c z}$ factor. This calculation treats the continuous critical wavenumber of the uniform covering string at fixed mass and normalized compact coordinate. Compactification samples the dispersion at discrete $k_n$. The mode is smooth at the bolt and exponentially decays. Integration by parts gives
\[
 \langle h,\mathcal Rh\rangle=\tfrac12(\|\nabla_Eh\|_2^2+k_c^2\|h\|_2^2)>0 .
 \tag{3}
\]
[Reall, Phys. Rev. D 64 (2001) 044005, hep-th/0104071v2, Eqs. (1.4)–(1.5) and the vacuum threshold matching before §1.4](https://arxiv.org/pdf/hep-th/0104071v2)

The vacuum Einstein block on this mode is $(\Delta_L+k^2)/2$. To calculate its background variation, use the spacelike $z$ split of the Einstein action for a $z$-independent $\phi(r)$, lapse $e^{2\phi/\sqrt3}$, zero compact shift, and external metric $e^{-2\phi/\sqrt3}g_{\mu\nu}(x,z)$. The external Ricci term reduces to the Einstein-frame action for $g(x,z)$ with the scalar kinetic term, up to an external total divergence at each $z$. The compact term in the Lorentzian integrand is
\[
 \frac{\sqrt{-g}}4 e^{-2\sqrt3\phi}
  \big[(\operatorname{tr}g^{-1}g_z)^2-\operatorname{tr}((g^{-1}g_z)^2)\big].
\]
The mixed extrinsic curvature is $(g^{-1}g_z)/(2e^{2\phi/\sqrt3})$, and lapse times leaf volume is $e^{-2\phi/\sqrt3}\sqrt{-g}$. Hence the background varies the projected Einstein mass term as $k^2e^{-2\sqrt3\alpha p}/2$. The scalar kinetic term contributes at order $\alpha^2$. This quadratic-form calculation allows the corrected perturbation to have induced compact components.

Suppose the stationary local Gauss–Bonnet branch and its static threshold admit differentiable smooth and decaying variations on a common domain, with the simple vacuum threshold as base. The vacuum linearization is the Einstein Hessian on this regular domain. Pairing the perturbed static equation with the vacuum kernel removes the derivative of the eigenfunction, including any induced compact components. From (1)–(3) and $\partial_{k^2}D G[h]=h/2$ in the vacuum block, the directional derivative is
\[
 \left.\frac{d k_c^2}{d\alpha}\right|_{\mathrm{GB,\ fixed\ ADM}}
 =2\sqrt3\,k_c^2\,\frac{\int p\,|h|^2\,d{\rm vol}_E}{\|h\|_2^2}
  +4k_c^2\,\frac{\langle h,\mathcal Rh\rangle}{\|h\|_2^2}
 >2k_c^4.
 \tag{4}
\]
Both contributions have the same sign. An analytic upper bound follows from $p(r)\le p(r_0)=11/(3\sqrt3\,r_0^2)$ and $\mathcal R\le 4c\,I\le 2I/r_0^2$ in the Euclidean tensor pairing. For the latter, the Euclidean sectional curvatures in the orthonormal frame are $R_{0101}=R_{2323}=2c$ and $R_{0202}=R_{0303}=R_{1212}=R_{1313}=-c$. On the diagonal tensor block the curvature contraction has eigenvalues $0,-2c,-2c,4c$, and on each off-diagonal component it acts by $-R_{ijij}$. Hence
\[
 2k_c^4 < \left.\frac{d k_c^2}{d\alpha}\right|_{\mathrm{GB,\ fixed\ ADM}}
 \le \frac{46}{3}\frac{k_c^2}{r_0^2}.
\]
The bounds use no numerical negative-mode profile and have units of inverse length to the fourth power. The direct insertion vanishes in the flat and $k=0$ controls. Holding the original five-dimensional horizon area coordinate fixed would impose a different background variation.

Equation (4) isolates the difference of first-order stationary threshold insertions between perturbative EFT matchings with the same leading curved state stress and response and different $\alpha$ at the perturbative order of interest. This extension assumes differentiability of the matched stationary backgrounds with the same vanishing circumference correction at infinity for their derivatives. A fixed positive radion mass can change the far-zone static tail. The massless profile (1) is the derivative at the vacuum base. The total semiclassical shift and the growing L10 rate shift remain open. The Unruh prescription above fails the stationary premise. Published pure Gauss–Bonnet thresholds at fixed five-dimensional horizon parameter use a different matching and do not independently verify (4). [Brihaye, Delsate and Radu, §4.2 and Fig. 7](https://arxiv.org/pdf/1004.2164v2)

## Conditional retarded spectral interface

Let $\lambda$ count the small deformation from the vacuum string, with its curvature coefficients, state and renormalization prescription specified. In a stationary formulation write $g_\lambda=\bar g+\lambda b+\cdots$. The equation for $b$ requires the complete renormalized curved stress. With additional metric terms transferred to an effective stress, its constraint projections are
\[
 R(\gamma)+K^2-K_{IJ}K^{IJ}=16\pi G_5 T^{\rm eff}_{NN},\qquad
 D_J(K^{IJ}-\gamma^{IJ}K)=8\pi G_5 j^I,\qquad j_I=-T^{\rm eff}_{NI},
\]
where $K_{IJ}=-\tfrac12(\mathcal L_N\gamma)_{IJ}$. Fix ADM mass, circumference and asymptotic lapse, and impose future horizon regularity. Off-diagonal energy flux or an asymptotic bath must be addressed before constructing a stationary branch.

For a specified state prescription define $\mathcal P^{\rm ret}$ as the metric functional derivative of the renormalized stress, including the local contact term. Its nonlocal part contains the causal stress commutator in the curved state. A change of independent state data contributes $\delta_\sigma T$ separately. The mixed-index linear Ward identity for the combined variation is
\[
 \bar\nabla_A\delta T^A{}_B+
 (\delta\Gamma^A{}_{AC})\bar T^C{}_B-
 (\delta\Gamma^C{}_{AB})\bar T^A{}_C=0.
\]
It uses the same prescription as the background and is needed for constraint propagation. Compact translation and stationarity make the kernel diagonal in signed Fourier number and Laplace parameter. The $n=1$ kernel remains a radial operator with two radial arguments. The flat homogeneous static Hessian constrains a different projection. The quadratic mean source additionally needs the causal second variation or equivalent retained field equations, including the induced state data. [Anderson, Molina-París and Mottola, Phys. Rev. D 67 (2003) 024026, gr-qc/0209075v1 §III, Eqs. (3.6)–(3.9) and the following contact-term discussion](https://arxiv.org/pdf/gr-qc/0209075v1)

Let $\mathscr L(s,k,\lambda)$ be the homogeneous retarded pencil after pulling both boundary conditions to a common regular constrained domain. At a simple isolated root with right null vector $h$ and left functional $\ell$ annihilating the vacuum range, solvability gives
\[
 \Omega'=-\frac{\ell(\mathscr L_\lambda h)}{\ell(\mathscr L_s h)},\qquad
 k_c'=-\frac{\ell(\mathscr L_\lambda h)}{\ell(\mathscr L_k h)}
 \quad\text{for the static threshold in the second formula.}
\]
These agree with the derivatives $-D_\lambda/D_s$ and $-D_\lambda/D_k$ for a differentiable local Evans function with the indicated simple zero. The numerator includes $D^2G[b,h]$, local curvature variations, the projected causal stress $-8\pi G_5\mathcal P^{\rm ret}_{1}(s)h$, and derivatives of the constraints and domain. Changes in horizon regularity or decay enter through the pullback or explicit boundary pairings. A nonlocal pencil needs a differentiability and Fredholm check and a denominator bounded away from zero. A nonstationary state requires a time-domain comparison. Equation (4) proves that one undetermined local term has a strictly signed net static overlap after including its geometric variation.

## Classical winding and algebra controls

For a smooth Poincaré-invariant warped circle $ds^2=e^{2A(y)}\eta_{\mu\nu}dx^\mu dx^\nu+dy^2$ with $y$ periodic, contract the Ricci tensor with $k=e^{-A}\partial_t+\partial_y$. The result is $R_{kk}=-3A''$. Einstein gravity and the NEC give $A''\le0$, hence constant $A$ by periodicity. The flat product requires zero total stress. A canonical scalar with nonzero winding has $T_{kk}=(k\cdot d\chi)^2>0$ wherever its winding derivative is nonzero. A cosmological or scalar-potential term has $T_{kk}=0$. The winding plus potential is therefore obstructed in this class, including smooth warping. The unwarped Schwarzschild product has the same pointwise obstruction by Ricci flatness. The argument assumes a smooth circle, the Einstein equation, and no NEC-violating stress canceling the winding.

For a shift-symmetric $P(X)$ winding with $X=g^{AB}\partial_A\chi\partial_B\chi$, the flat tuning requires $P=P_X=0$ at the spacelike winding $X_0=q^2$, after absorbing bulk vacuum energy into $P$. $P_{XX}<0$ gives positive flat radion curvature. The matter quadratic term is proportional to $P_{XX}(2q\partial_z\delta\chi-q^2 h_{zz})^2$ and the principal symbol $P_Xg^{AB}+2P_{XX}\partial^A\chi\partial^B\chi$ lacks a time coefficient at the tuned point. The formal zero stress response in a nonzero harmonic follows from $\partial_z\delta X=0$ in this degenerate system. It therefore supplies no healthy winding kinetic example. [Pinned GOAL-008 winding analysis](https://github.com/inaeyk/GL-with-AI/blob/4aac5ad001a9d6c63efa86640ce05837282fb73f/research/sav/GLR-GOAL-008/STABILIZATION.md#1-classical-consistency-tests)

Exact algebra control with Python 3.14.8 and SymPy 1.14.0.
```sh
python3 - <<'PY'
import sympy as s
r, r0 = s.symbols('r r0', positive=True)
f = 1-r0/r
p = 2/(s.sqrt(3)*r0)*(1/r+r0/(2*r**2)+r0**2/(3*r**3))
assert s.simplify(s.diff(r*r*f*s.diff(p,r),r)/r**2
                  + 2*s.sqrt(3)*r0**2/r**6) == 0
assert s.simplify(p/s.sqrt(3) - (2/(3*r0*r)+1/(3*r*r)+2*r0/(9*r**3))) == 0
curvature = s.Matrix([[0,-2,2,0],[-2,0,-2,0],[1,-1,2,0],[0,0,0,-2]])
assert curvature.nullspace() == [s.Matrix([-1,1,1,0])]
assert curvature.col_join(s.Matrix([[-1,1,2,0]])).rank() == 4
euclidean_diagonal = s.Matrix([[0,2,-1,-1],[2,0,-1,-1],[-1,-1,0,2],[-1,-1,2,0]])
assert euclidean_diagonal.eigenvals() == {-2: 2, 0: 1, 4: 1}
print('GB scalar residual and published factor: zero')
print('spherical curvature contraction kernel: [-1, 1, 1, 0], TT-augmented rank: 4')
print('Euclidean diagonal curvature eigenvalues in units of c: -2, -2, 0, 4')
PY
```

The command exited successfully and printed the three confirmations. These checks reuse the derived matrices and scalar equation. The comparison with the independently constructed pure Gauss–Bonnet metric checks the scalar background and normalization. The Euclidean variational argument checks the sign of the direct term by a route separate from the pointwise contraction. Equation (4) has no independent numerical threshold replay. A targeted primary-literature search found a flat determinant, a causal stress response formalism, the vacuum static threshold map, a massive Dirac scattering equation and a pure Gauss–Bonnet comparison. These inspected sources leave the full curved Dirac and gravitational stress and the constraint-projected $n=1$ retarded polarization open for the tuned candidate.

The action-level matching stops at the specified stationary-state obstruction and the unmeasured curvature direction. The first input for a quantitative stabilized GL waveform is a choice of renormalized state and a horizon-compatible backreacted geometry. This needs either a stationary boundary problem justified by its stress or a controlled time-dependent prescription. The subsequent inputs are the retarded nonzero-harmonic and quadratic mean responses and the matched initial constraints. The fixed-source scalar transport benchmark uses a separate action and prescribes the vacuum source as external work.
