# GOAL-016 specialist record · quadratic scalar, preparation and optical trace

I reconstructed the scalar source and graph current, the preparation lower and upper bounds, and the leading optical normal form from the displayed operators. I compared against the completed dossier and its published verdicts, so the review is non-blind. The normalized GL eigenpair, mode enclosure and the fixed-jet existence and mass-map theorem are inherited. I inspected the finite-slice waveform and constraint interfaces and the optical equations and normalization at the cited versions. The separate analytic work below concerns their use, the signs, and the regularity and causal domains. There were no physical evolutions in this track.

[GOAL-016 source at 6e7615f9d0a88a08813203ad5b7cadde990fa042](https://github.com/inaeyk/GL-with-AI/tree/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016) · [GOAL-005 waveform and constraint source](https://github.com/inaeyk/GL-with-AI/tree/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005) · [GOAL-009 optical equations](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/GLR-GOAL-009/checks/README.md) · [GOAL-009 audit](https://github.com/inaeyk/GL-with-AI/blob/844d1ccdd5e507392f4ace02debf08da34d543d0/research/sav/verification/GLR-GOAL-009/DERIVATIONS.md)

## Source and waveform extraction

Set \(r_0=1\), \(f=1-1/r\), \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\). Write the first jet as the real unhalved \(e^{\Omega t}H_{\mu\nu}\cos(kz)\), with \(k=\pi/5\), \(L=10\) and \(H_{\theta\theta}(2)/8=1\). In the compact-unitary first-order representative the compact lapse and shift have zero first jets. The compact extrinsic curvature starts with \(K_{\mu\nu}^{(1)}=-(k/2)e^{\Omega t}H_{\mu\nu}\sin(kz)\). The compact-normal trace is zero to first order by tracelessness. In the compact Ricci identity, the \(z\) derivative of the second-order trace averages to zero and the remaining quadratic forcing is \(-\langle K_1:K_1\rangle=-(k^2/8)e^{2\Omega t}H:H\). The colon in \(H:H\) denotes the Lorentzian contraction. Combining this with \(b=e^{2\phi/\sqrt3}\) gives \(\Box_4\phi_2=-\sqrt3\,k^2e^{2\Omega t}H:H/16\). For \(\psi=r\phi_2\), \(\Box_4(\psi/r)=-(4\psi_{uv}+V_0\psi)/(fr)\), \(V_0=f/r^3\). The sign and factors in the causal equation are therefore
\[
4\psi_{uv}+V_0\psi=F_2,\qquad
F_2=\frac{\sqrt3 k^2rf}{16}e^{2\Omega t}H:H .
\]
The contraction is indefinite before the profile is enclosed. For a far flat check, the two angular eigenvalues contribute \(2K^2\) with \(K=H_{\theta\theta}/r^2\), and the flat TT constraints give \(H_{tr}\sim-\Omega H_{tt}/q\), \(H_{rr}\sim\Omega^2H_{tt}/q^2\), \(2K\sim k^2H_{tt}/q^2\), \(q^2=k^2+\Omega^2\). Thus \(H_{tt}^2-2H_{tr}^2+H_{rr}^2\sim4K^2\). The total \(6K^2\) is positive at leading order in the far zone.

The Z graph is \(v=R(u)=g^{-1}(u)\) with \(g(r)=-r-2\log(r-1)\). The completed scalar value and normal velocity vanish there. Tangential and normal combinations are independent for \(r>1\), so both initial null derivatives vanish. Integrating at fixed \(u\) gives
\[
n_Z(u)=\frac14\int_{R(u)}^\infty (F_2-V_0\psi)(u,v)\,dv,\qquad
\frac14\int_{R(u)}^\infty V_0\,dv=\frac1{4R(u)^2}.
\]
The equality uses \(dv=2\,dr/f\). The same characteristic equation controls \(\psi_v\) from the past wedge and yields its vanishing on fixed early cuts, using the exponentially decaying forced tail and the integrable curvature coefficient. In particular the derivative of \(\psi\) along a finite constant-radius tube is \(\psi_t=\psi_u+\psi_v\), with the incoming derivative removed only in the null limit. A growing separated particular solution requires an added homogeneous field with the opposite graph jet. It changes the experiment to drop that transient. The numerical energy and pointwise news boxes are inherited from GOAL-005. This derivation verifies the measured field, source normalization, causal contacts and extraction formulas used in those boxes.

A pure gauge control on a product end sets \(z=z'+\epsilon\zeta(u,z')\), where \(\zeta\) is periodic and the map is orientation preserving. At fixed \(u\) the coordinate average of \(g_{z'z'}\) becomes \(1+\epsilon^2\langle(\partial_{z'}\zeta)^2\rangle\), with arbitrary time dependence. The physical circumference remains \(L\), since \(\int_0^L(1+\epsilon\zeta_{z'})\,dz'=L\). This control excludes raw mean \(g_{zz}\) as a gauge-independent radion diagnostic. The completed first-order representative and geometric length trace are material to extraction.

For the canonical scalar, spherical integration of the Einstein-frame stress gives \(dE/du_{\rm phys}=(\partial_{u_{\rm phys}}\Phi_{\rm phys})^2/G_4\) at the massless null end. Using \(M_0=r_0/(2G_4)\), \(\Phi_{\rm phys}=\epsilon^2 r_0\lim\psi\) and \(du_{\rm phys}=r_0du\) gives \(\mathcal K_Z=2\|n_Z\|_2^2\). There is no conversion of a massive timelike flux to this massless Bondi observable.

## Current, graph energy and cancellation cost

For \(h\) solving the homogeneous master equation use \(e=(h_t^2+h_x^2+V_0h^2)/2\), \(j=-h_t h_x\). The canonical physical scalar uses \(h_x-\beta h\), \(\beta=f/r\). Since \(V_0=\partial_x\beta+\beta^2\), direct expansion gives \(e_{\rm phys}-e=-\partial_x(\beta h^2)/2\) and \(j_{\rm phys}-j=+\partial_t(\beta h^2)/2\). The same differences hold for the added fixed-background mass potential because both densities include \(f\mu^2h^2/2\). Thus the physical one-form is the master one-form \(e\,dx-j\,dt\) minus \(d(\beta h^2)/2\). On the full graph this exact term vanishes between the compact outer and regular horizon endpoints. At a finite tube it contributes with the plus sign to outward flux,
\[
\mathcal K_{\mu,R}(I)=2\int_I-\psi_t\psi_x\,du+\frac fR [\psi^2]_{\partial I}.
\]
A flat incoming spherical pulse \(h=B(v)\) with \(B\) zero at both endpoints has signed coefficient \(-2\int_I (B')^2du\). Finite-tube Killing energy on a window can have either sign, and finite-slab comparisons additionally involve work and storage. This test uses an auxiliary free solution.

On \(\Sigma\), \(t=T(x)=-\log(r-1)\), \(p=T'=-1/r\). The graph density is \(e-pj=(1-p)h_u^2+(1+p)h_v^2+V_0h^2/2\). In null variables the closed form is \(-(h_u^2+V_0h^2/4)du+(h_v^2+V_0h^2/4)dv\). The upper \(v\) cap and right \(u\) ray have positive outgoing flux with their future orientations. Closing the wedge at a flat left front earlier than any compact data, and then taking caps to infinity with Fatou, gives \(\|N_h\|_{L^2(I)}^2\le\mathcal E_\Sigma\). This applies to incoming as well as outgoing initial jets. Exact cancellation requires \(N_h=-n_Z\), hence \(\mathcal E_\Sigma\ge b_Z^2\). The reverse and forward triangle inequalities give
\[
2(b_Z-\sqrt{\mathcal E_\Sigma})_+^2\le 2\|n_Z+N_h\|_2^2\le 2(b_Z+\sqrt{\mathcal E_\Sigma})^2 .
\]
If \(2\mathcal E_\Sigma\le (3.5316\times10^{-7})/4\), the lower bound is at least \((3.5316\times10^{-7})/4\).

For the constructive bound prescribe \(h=A(u)\), \(h_u=A'\), \(h_v=0\) on a padded graph interval \(J=[a,b]\), with \(A'\) flat before the left end. The actual free data are \(F=A/r\) and \(P=[(1-p)A'/r-p f A/r^2]/\sqrt{f(1-p^2)}\) because the future unit normal is \((\partial_t+p\partial_x)/\sqrt{f(1-p^2)}\). The causal wave integral bounds \(M(u)=\sup_v|h(u,v)|\) by \(J(|A'|+k_b M)\) for the left Volterra primitive and \(k_b=1/(4R(b)^2)\). Gronwall and the \(L^2\) norm \(2|J|/\pi\) of that primitive give \(\|N_h-A'\|_2\le\delta_J\|A'\|_2\), \(\delta_J=k_b e^{k_b|J|}2|J|/\pi\). Smoothness of the inverse follows by differentiating the ray integral and bootstrapping its integrable potential and graph contacts, as in the pinned preparation derivation. Choosing the target \(-\chi n_Z\) with a smooth flat left turn-on gives exact cancellation on \(I\). The graph derivative cost of the outgoing portion is exactly \(\|A'\|_2^2\) because \((1-p)dx=-du\). Its potential cost is bounded by \(|J|(R_b^{-2}-R_a^{-2})\|A'\|_2^2/4\).

There is a separate check of the static continuation. For fixed graph value \(a_\Sigma(x)=h(T(x),x)\), the graph energy density is
\[
\tfrac12\big((a_\Sigma')^2+(1-p^2)h_t^2+V_0a_\Sigma^2\big).
\]
On the inner interval, completing \((a_\Sigma'-\beta a_\Sigma)^2\) leaves the endpoint term \(f(R_b)A(b)^2/(2R_b)\). The regular continuation \(a_\Sigma=A(b)r/R_b,\ h_t=0\) attains that minimum for the fixed endpoint value. Equivalently the radion is constant there and its physical local density vanishes. The master and physical energies on this interval differ by the boundary term at the seam. Because the two prescribed values agree at the seam, smooth joining over width \(\eta\) after the causal interval incurs \(O(\eta)\) extra first-derivative energy for fixed smooth outgoing data. A smooth taper inside the horizon fits the original compact data prescription. The upper bound uses an infimum and does not require bounded higher derivatives of the shrinking seam.

Using slightly tighter rational endpoint tests gives \(\mathcal E_{\rm can}/b_Z^2<1.194\) with the same energy estimates. The following standalone Python 3.14.8 arithmetic check uses no evolution. Both root tests follow from monotonicity of \(g\). The numbers printed by this check are \(0.01229483781719602\) and \(1.1928918560952777\).
~~~python
from fractions import Fraction as Q
from math import factorial

def exp_lower(x, n=12):
    return sum((x**j / factorial(j) for j in range(n + 1)), Q(0))

def exp_upper(x, n=12):
    return exp_lower(x, n) + x**(n + 1) / factorial(n + 1) / (1 - x / (n + 2))

ra, rb = Q("6.57"), Q("5.15")
assert exp_upper((10 - ra) / 2) < ra - 1
assert exp_lower((8 - rb) / 2) > rb - 1
length = Q(2)
k = 1 / (4 * rb**2)
delta = k * exp_upper(k * length) * Q(32, 25)
ratio = (1 + length * (rb**-2 - ra**-2) / 4
         + length * (rb - 1) / (2 * rb**2)) / (1 - delta)**2
assert ratio < Q("1.194")
print(float(delta), float(ratio))
~~~

The inherited \(C^1\) ADM map with \(\partial_\lambda(M/M_0)=9/14\) supplies tuning for each fixed pair. On compact subdomains the pure \(\lambda\) scalar variation vanishes since the zero-seed spherical product has constant radion. Therefore the \(o(\epsilon)\) charge parameter does not alter the quadratic radion coefficient. This establishes fixed first jet, circumference and mass for the compared exact constrained initial families within their respective small-seed neighborhoods. The cost is the background Killing energy of the **added homogeneous degree of freedom**. It is neither a change in the total ADM mass nor a finite-amplitude cost bound for exact Bondi radiation. The flux comparison needs uniform trace and clock control separately.

## Optical failure and the leading repair

Linearize in \(w=R^{-s}g(z)\) around \(\bar q=\alpha/R\) at fixed \(u\), with decaying terminal operator data, keeping \(O(\alpha)\). Solving the radial constraints successively gives
\[
\frac{\beta_1}{g}=-\frac{3(s+1)\alpha R^{-s-2}}{2(s+2)},\quad
\frac{P_1}{g_z}=-\frac{6R^{-s-1}}{s-1}-\frac{3\alpha R^{-s-2}}{s+2},\quad
\frac{A_1}{g_z}=d_0R^{-s}+d_1\alpha R^{-s-1},
\]
where \(d_0=3(s+1)/[s(s-1)]\), \(d_1=[3(s+3)/(2(s+2))+3d_0]/(s+1)\). For example \(\partial_R\beta_1=3\alpha(s+1)R^{-s-3}g/2\), and inserting its integral in \((R^2P_1)_R\) yields \(+3\alpha sR^{-s-1}g_z/(s+2)\). The integrating factor \(e^{-3\alpha/R}\) in the \(A\) equation gives the \(+3d_0\) contribution to \(d_1\).

The angular potential term gives \((RV_1)_R=3(s+2)R^{1-s}g_{zz}/[s(s-1)]\) at \(\alpha=0\). Since \(\bar W_R=0\) in this leading model, it contributes through \(-\bar W(V_1)_R/R\). In units \(R^{-s}g_{zz}\) its coefficient is \(-(\alpha/R)3(s+2)/[s(s-2)]\). The remaining pieces of the compact coefficient are \(c_0=1+6/(s-1)-2d_0\) at order zero and \(3/(s+2)-2d_1+d_0-3c_0\) at order \(\alpha/R\). Consequently \(c_1=-3+K(s)\) with
\[
K(s)=-\frac{12(s^2+2)}{(s-2)(s-1)(s+1)(s+2)}
=6\big((s+2)^{-1}-(s+1)^{-1}+(s-1)^{-1}-(s-2)^{-1}\big).
\]
At \(s=4\) the angular contribution is \(-9/4\) and \(c_1=-21/5\), compared with \(-39/20\) if it is omitted. The terminal resolvent for \((s+c)^{-1}\) integrates along increasing \(\log R\) with kernel \(e^{-ct}\). Thus the combined kernel \(6(e^{-2t}-e^{-t}+e^t-e^{2t})\) is strictly negative for \(t>0\) and upper triangular in radius. Disjoint inner and outer positive bumps give nonzero pairing with \(K\) acting on the outer bump and zero in the opposite direction. For \(Z=Z_{\rm out}(R)\cos(k_m z)/k_m\) and \(Z_u=Z_{\rm in}(R)\cos(k_m z)\), ordinary first-derivative energy stays bounded while the skew pairing from \((\alpha/R)K(-D)\partial_z^2\) grows as \(|k_m|\). This holds for fixed normalized radial bumps and nonzero \(\alpha\), and persists after rescaling for a fixed number of compact commutations. It falsifies a same-order forcing estimate for the frozen transform on this operator domain.

Write \(D=R\partial_R\). The inherited \(F(D)=D(D-1)/[(D+2)(D+3)]\) removes \(c_0(-D)\) via \(F(D)c_0(-D)=F(D-1)\). The factor \(R^{-1}\) shifts the multiplier, so \(c_1(-D)\) remains after applying \(F(D)\) and rewriting in \(Z=F(D-1)w\). For \(b_*(s)=3/(s-1)-3/s+3/(s+1)-3/(s+2)\), put \(G=s b_*(s)\), \(H=s b_*(s-1)\). Expansion verifies \(G-H=K\) and \(sH(s+1)=(s+1)G(s)\), giving the stated first-order conjugation and preserving the mixed derivative at principal level. A time derivative of \(\alpha\) leaves a bounded radial operator because \(G=O(s^{-1})\). The weighted terminal resolvents are bounded on \(L^2(R^p\,dR)\) for \(p>5\). Under conjugation to \(L^2(d\log R)\), their kernel has integral \(1/((p+1)/2+c)\). This checks the operator and smallness domain of the repair. Subleading radial powers may still multiply two compact derivatives. An extra inverse-radius power does not bound high compact frequency at fixed radius. Arbitrary terminal integration functions and exact nonlinear coupling remain outside this leading calculation.

A separate check of the areal normalization starts with \(g_E\sim-du_B^2-2du_B\,dR\) and \(R=r_5+\alpha/2+o(1)\). In \(r_5\) the normalized time-time coefficient is \(-(1+\partial_{u_B}\alpha)\). Since \(\Phi=\sqrt3\,\alpha/2\), the needed limit is \(V_5/B_\infty=1+2N/\sqrt3\). Imposing unit limit in \(r_5\) would force \(N=0\) under these differentiated asymptotics. Another distinct control is outgoing Vaidya times a circle, with \(b=B=1,\ A=0,\ V=1-2m(u)/R\). It satisfies the radial subsystem and has \(\operatorname{Ric}_{uu}=-2m'(u)/R^2\). An \(O(R^{-2})\) supplementary residual alone cannot impose vacuum or set the physical mass aspect. Neither control models the actual prepared vacuum development.

With the potential on the left in \(4\psi_{uv}+V_\epsilon\psi=F_\epsilon\), subtraction gives the forcing error \(F_\epsilon-\epsilon^2 F_2+\epsilon^2(V_0-V_\epsilon)\psi_2\). To obtain a quartic news remainder one needs integrated absolute forcing control, homogeneous data errors, decay of the incoming derivative, and a common clock. For \(u_B=u+O_{C^1}(\epsilon^2)\) on padded cuts, \(d=u_B'\), the transformed derivative is \(\partial_u\Phi_\epsilon/d\). A quartic error in \(L^2(du)\) transforms with factor at most \((\inf d)^{-1/2}\), and smoothness of \(n_Z\) bounds the coefficient error \(\epsilon^2(n_Z(u)-d\,n_Z(u_B))\) by \(O(\epsilon^4)\) there. Half-translation alone allows the even correction \(|\epsilon|^3 g(u)\) and supplies no such bound. The independent checks therefore support the **quadratic** result and the **leading** operator repair. Exact optical principal energy, Cauchy/terminal matching, common clock and the finite-amplitude scalar trace remain unresolved. GOAL-015 is a separate ordered tensor calculation with an axial seed and cannot supply these scalar bounds.

For reproducibility, under Python 3.14.8 the pinned commands below matched their archived standard output byte for byte. The optical computation used SymPy 1.14.0.
~~~sh
set -o pipefail
g=GLR-GOAL-016
python3 -B "$g/checks/preparation_bounds.py" | diff -u "$g/checks/preparation_bounds.txt" -
python3 -B "$g/vacuum/check_optical_principal.py" | diff -u "$g/vacuum/check_optical_principal.txt" -
~~~
