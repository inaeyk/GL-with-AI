# Independent adversarial audit I — action, normalization and gauge projection

Auditor: `audit_foundation`, independent scientific subagent, 2026-10-22.
Scope: GLR-GOAL-001 milestone E.2(i). This audit rederived the identities
below rather than treating historical notes or fixture spectra as proof.
No physical ODE was solved and no production setup was accessed or changed.

**Verdict:** the SO(3) bulk action/EOM, uncharged radion flux and proposed
unitary-gauge quadratic scalar source pass the checks described here.
There is a definite graviphoton-exponent error in the initial notation and
a misleading first-class-constraint count. These must be corrected.
Nonzero GL-to-news overlap, a normalized numerical coefficient and a
controlled weakly nonlinear interval are **not certified** by this audit.

## 1. Independent sphere reduction and the boundary cancellation

Use orbit indices \(a,b=0,1,2\), sphere indices \(i,j=1,2\), and
\(ds_5^2=g_{ab}dx^a dx^b+R^2\gamma_{ij}dy^i dy^j\).
The mixed Christoffels are

\[
\Gamma^a{}_{ij}=-R\nabla^aR\gamma_{ij},\qquad
\Gamma^i{}_{aj}=R^{-1}\partial_aR\delta^i_j.
\]

Direct contraction, with \({\rm Ric}[\gamma]_{ij}=\gamma_{ij}\), gives

\[
R^{(5)}_{ab}=R^{(3)}_{ab}-2R^{-1}\nabla_a\nabla_bR,
\quad R^{(5)}_{ij}=[1-(\nabla R)^2-R\Box R]\gamma_{ij},
\]

and therefore

\[
\mathcal R_5=\mathcal R_3+2R^{-2}-4R^{-1}\Box R
 -2R^{-2}(\nabla R)^2.
\]

The sphere integral is exactly \(4\pi\). The unintegrated bulk action is
\((4G_5)^{-1}\int\sqrt{-g}[R^2\mathcal R_3+2-4R\Box R-2(\nabla R)^2]\).
Integration by parts produces the bulk term \(+2(\nabla R)^2\) and boundary
term \(-G_5^{-1}\int_{\partial B}\epsilon\sqrt{|h|}R\,n\cdot\nabla R\).
For a smooth non-null product boundary \(\partial B\times S^2\),

\[
K_5=K_3+2n\cdot\nabla R/R,
\quad S_{\rm GHY,5}={1\over2G_5}\int_{\partial B}
 \epsilon\sqrt{|h|}R^2(K_3+2n\cdot\nabla R/R).
\]

The derivative-of-R terms cancel, leaving

\[
S_{\rm bulk}={1\over4G_5}\int_B\sqrt{-g}
 [R^2\mathcal R_3+2(\nabla R)^2+2],\qquad
S_{\rm bdy}={1\over2G_5}\int_{\partial B}
 \epsilon\sqrt{|h|}R^2K_3.
\]

This statement assumes the usual fixed induced metric and R Dirichlet
data. Corners require their corner terms. Null boundaries require the
appropriate null action or a controlled non-null limiting surface; this
audit does not authorize dropping a horizon boundary contribution in a
Green-function/action derivation merely by citing bulk integration by parts.

Varying \(R\) gives \(R\mathcal R_3-2\Box R=0\). Varying \(g^{ab}\)
gives exactly

\[
R^2G_{ab}+2R(g_{ab}\Box R-\nabla_a\nabla_bR)
 +g_{ab}[(\nabla R)^2-1]=0.
\]

The trace together with the scalar equation reproduces both direct Ricci
conditions above. Thus the potential sign \(+2\) is fixed by positive unit
sphere curvature, not a convention that can be adjusted to obtain growth.

## 2. Hamiltonian count: correction to the initial explanation

The spatial orbit metric \(q_{ij}\) has three components, and R supplies
one. The lapse and two shifts are multipliers, so the nontrivial canonical
phase space has dimension eight. The Hamiltonian constraint and two momentum
constraints are three first-class constraints: they remove six dimensions,
leaving two phase dimensions, or **one local configuration degree**.

One can make nondegeneracy explicit by the invertible transformation
\(g^{E}_{ab}=R^4g_{ab}\) in a region \(R>0\), with an arbitrary fixed
reference length understood to render this conformal factor dimensionless.
Up to constants/boundaries, the action becomes

\[
S\ \propto\ \int\sqrt{-g_E}
 [\mathcal R_E-6(\nabla\ln R)^2+2R^{-6}].
\]

Hence \(\chi=\sqrt{12}\ln R\) is an ordinary scalar with nonzero kinetic
term. For \(S=\alpha\int\sqrt{-g_E}[\mathcal R_E-
\tfrac12(\partial\chi)^2-U(\chi)]\), its ADM constraints are

\[
\mathcal H={\pi^{ij}\pi_{ij}-\pi^2+p_\chi^2/2\over\alpha\sqrt q}
 +\alpha\sqrt q[-\mathcal R[q]+\tfrac12(D\chi)^2+U],
\quad
\mathcal H_i=-2D_j\pi^j{}_i+p_\chi\partial_i\chi.
\]

If the lapse/shift canonical pairs are retained, the count instead starts
with 14 phase dimensions and **six** first-class constraints (three primary
momenta plus three secondary constraints), giving \((14-12)/2=1\).
The old phrase “three first-class constraints” while subtracting twelve
dimensions conflates these two descriptions; its final number was correct.

This local count says nothing about stationary global charges. At n=0,
\(\delta M\) is a regular Schwarzschild-family perturbation. A homogeneous
static radion solves \(\partial_r(r^2f\phi')=0\), hence
\(\phi=c_\infty+c\ln f\). At fixed asymptotic circle length
\(c_\infty=0\), and horizon regularity excludes the logarithmic mode.
Sourced stationary second-order scalar charge is a different question and
can affect mass/tension without news. No radiation conclusion follows from
stationary charge counting.

## 3. Definite KK convention error; scalar normalization survives

For the metric convention actually written in NOTATION.md,

\[
ds_5^2=e^{-2\phi/\sqrt3}ds_E^2
 +e^{4\phi/\sqrt3}(dz+2A)^2,
\]

the reduced action is

\[
S_4={1\over16\pi G_4}\int\sqrt{-g_E}
 [\mathcal R_E-2(\partial\phi)^2
  -e^{\mathbf{+}2\sqrt3\phi}F_{\mu\nu}F^{\mu\nu}],
\qquad G_4=G_5/L.
\]

The negative exponent printed in the initial notes corresponds to reversing
the sign of phi in the metric ansatz. An especially simple independent
check is constant-phi scaling. Write \(\alpha=-1/\sqrt3\),
\(\beta=2/\sqrt3\) so \(g_5=e^{2\alpha\phi}g_E+
e^{2\beta\phi}(dz+2A)^2\). The determinant contributes
\(e^{(4\alpha+\beta)\phi}\); the vector curvature contributes
\(e^{(2\beta-4\alpha)\phi}F^2\). Their product has exponent
\(3\beta=+2\sqrt3\). The factor two multiplying A gives exactly
\(-F^2\), starting from the connection-curvature factor \(-\tfrac14(d(2A))^2\).

The uncharged sector used for scalar radiation has A=0. Its kinetic
coefficient \(-2\) and \(\phi=\sqrt3\ln g_{zz}/4\) are consistent.

## 4. Independent five-dimensional scalar-news normalization

The four-dimensional scalar action is
\(-[8\pi G_4]^{-1}\int\sqrt{-g_E}(\partial\phi)^2\), so
\(T_{uu}=\dot\Phi^2/(4\pi G_4\rho^2)+O(\rho^{-3})\) for
\(\phi=\Phi(u)/\rho+O(\rho^{-2})\). Integrating a sphere yields

\[
{dE_0\over du}={\dot\Phi^2\over G_4}.
\]

An independent 5D route uses the physical TT polarization in a local
asymptotic plane transverse to an outgoing radial wave. Its three
transverse directions are two sphere tangent directions and z. The scalar
polarization is

\[
h_{IJ}^{\rm TT}={\Phi(u)\over\rho}
 \operatorname{diag}(-2/\sqrt3,-2/\sqrt3,4/\sqrt3),
\quad \sum_{IJ}(P_{IJ}^{\rm TT})^2=8,\quad\operatorname{tr}P=0.
\]

The radial/time metric components from the KK ansatz are removed by the
linear asymptotic wave gauge transformation; the displayed transverse
polarization is physical and traceless. The quadratic Einstein TT action
is \(-[64\pi G_5]^{-1}\int(\partial h_{IJ}^{\rm TT})^2\), and its
outgoing leading canonical energy current is
\([32\pi G_5]^{-1}(\partial_u h_{IJ}^{\rm TT})^2\). Therefore

\[
\int_0^L dz\int d\Omega_2\rho^2\mathcal F_5
 ={4\pi L\over32\pi G_5}\,8\dot\Phi^2
 ={\dot\Phi^2\over G_4}.
\]

This is the leading asymptotic quadratic/symplectic energy current; no
high-frequency or temporal-averaging assumption is needed for a linear
outgoing asymptotic wave. The same formula is often written with an
Isaacson average, but such averaging is not being used as justification
for its instantaneous Bondi coefficient here. Scalar and tensor
normalizations, sphere area and L have all been checked. For a scalar
generated at order A^2 this is order A^4 physical energy. It does not set
the value of the scalar coefficient.

With \(M_0=r_0/(2G_4)\), the conditional settled-exponential formulas
\(d(E_0/M_0)/d(u/r_0)=8(\Omega r_0)^2|C_0|^2 A^4\) and
\(\Delta E_0/M_0=2\Omega r_0|C_0|^2(A_2^4-A_1^4)\) follow.
The positive-frequency news spectrum is
\(|\widetilde N_\phi(\omega)|^2/(\pi G_4)\). This audit certifies these
normalizations only under their waveform/initial-data assumptions.

## 5. Unitary first-order gauge and second-order completion

Use \(g=\bar g+\epsilon h+\epsilon^2q+\cdots\), with no factorial in q,
and pullback generator \(\epsilon\xi+\epsilon^2\eta\). Then

\[
h'=h+\mathcal L_\xi\bar g,\qquad
q'=q+\mathcal L_\xi h+\tfrac12\mathcal L_\xi^2\bar g
 +\mathcal L_\eta\bar g.
\]

On the product background \(\bar g_{zz}=1,\bar g_{z\mu}=0\), the unique
nonzero Fourier compensator that sets \(h^U_{zA}=0\) is

\[
\xi_z=-{1\over2\partial_z}h_{zz},\qquad
\xi_\mu=-\partial_z^{-1}(h_{z\mu}+\partial_\mu\xi_z).
\]

Here \(\partial_z^{-1}\) means multiplication by \(1/(ik_n)\) on
\(n\ne0\), not an inverse on the zero harmonic. This is an admissible
periodic transformation for fixed nonzero k. It is local in the other
coordinates and regular if the starting tensor is smooth in a
horizon-penetrating chart. Coordinate component divergences in static
Schwarzschild coordinates must not be mistaken for failure of this test.

Define the quadratic zero-mode completion

\[
Q_{zz}^U=\Pi_0\left[q+\mathcal L_\xi h+
 \tfrac12\mathcal L_\xi^2\bar g\right]_{zz}.
\]

The term \(\Pi_0(\mathcal L_\eta\bar g)_{zz}
=2\Pi_0\partial_z\eta_z=0\) for periodic second-order transformations.
Uniqueness of first-order gauge fixing therefore makes this quantity
invariant under the allowed first-order nonzero-k diffeomorphisms, up to
the separately fixed background zero-mode coordinate freedoms. The
restricted family must have no independent physical first-order zero
mode. A constant z translation changes phase, not its zero-mode output.

At this order the physical perturbative radion is
\(\phi_0^{(2)}=\sqrt3 Q_{zz}^U/4\). In unitary gauge all \(g_{zA}\)
corrections begin at second order, and the nonlinear logarithm/subtractions
first affect this formula at higher order. For a genuine localized mode
with radial exponent \(q_r=\sqrt{k^2+\Omega^2}>\Omega\), the compensator
and its quadratic local terms decay at fixed u as
\(e^{-2(q_r-\Omega)r}\) times powers. Thus they do not alter the
\(1/\rho\) coefficient at null infinity. Fixing Bondi time and the
asymptotic circle identification is still required. Nonperiodic
transformations that rescale the circle are outside this gauge class.

A deliberate pure-gauge counterexample shows why raw averages are unsafe.
On \(\bar g_3=-fdt^2+dr^2/f+dz^2\), let
\(\chi^r=b(r)\cos kz,\chi^z=c(r)\sin kz\),
\(h=\mathcal L_\chi\bar g\), \(q=\tfrac12\mathcal L_\chi^2\bar g\).
The proper completion with \(\xi=-\chi\) is exactly zero. Omitting its
half-Lie-squared term instead leaves the spurious average
\(-k^2[c^2+b^2/f]/2\). A nonzero \(\langle g_{zz}\rangle\) source in
an unfixed chart therefore is not evidence of radiation.

## 6. Supplemental independent check of the proposed quadratic source

The main calculation supplied its source after the independent foundation
work above. In the first-order unitary gauge, put
\(\gamma_{\mu\nu}=\bar g_{\mu\nu}+A(t)H_{\mu\nu}(r)\cos kz\) and
\(ds_5^2=dz^2+\gamma_{\mu\nu}dx^\mu dx^\nu\) for evaluating the
quadratic first-order contribution. The z-extrinsic curvature is
\(K_{\mu\nu}=\gamma_{\mu\nu,z}/2\). The independent Gauss relation
\(R_{zz}=-\partial_zK-K_{\mu\nu}K^{\mu\nu}\) gives

\[
R_{zz}=-\tfrac12\operatorname{tr}(\gamma^{-1}\gamma_{,zz})
 +\tfrac14\operatorname{tr}(
 \gamma^{-1}\gamma_{,z}\gamma^{-1}\gamma_{,z}).
\]

For a traceless H, the two averaged quadratic terms are respectively
\(-k^2A^2H_{\mu\nu}H^{\mu\nu}/4\) and
\(+k^2A^2H_{\mu\nu}H^{\mu\nu}/8\). Their sum is

\[
\Pi_0R_{zz}^{[2]}=-{k^2A^2\over8}H_{\mu\nu}H^{\mu\nu}.
\]

For a general zero-mode second-order perturbation on the product
background, \(\delta R_{zz}=-\Box_4Q_{zz}^U/2\). There is no
\(q_{\mu\nu}\) or \(q_{z\mu}\) term, so other zero-mode constraints
do not subtract an additional scalar from this closed equation. Hence

\[
\Box_4\phi_0^{(2)}=-{\sqrt3 k^2\over16}A^2H_{\mu\nu}H^{\mu\nu},
\quad
(\partial_t^2-\partial_{r_*}^2+fr_0/r^3)(r\phi_0^{(2)})
 ={\sqrt3 k^2rf\over16}A^2H_{\mu\nu}H^{\mu\nu}.
\]

The source sign uses \(-rf\Box_4(\psi/r)\) equal to the displayed
master operator. The contraction is Lorentzian, not a positive norm.
Its sign and outgoing overlap cannot be inferred from the notation H^2.
Full sourced metric-constraint integrability still requires an actual
on-shell first-order mode and the associated Bianchi identity; this
supplement is not a certificate that an arbitrary trial H is physical.

## 7. Exact reproducibility and limitations

Command executed by this auditor:

```text
python3 research/sav/verification/audit_foundation_checks.py
```

Output, Python 3.14.8 / SymPy 1.14.0, exit 0:

```text
Python 3.14.8 SymPy 1.14.0
DIRECT_5D_WARP_CURVATURE: PASS (flat g3, arbitrary R(t,r,z))
MUTATIONS_WARP_GRADIENT_SIGN_AND_SPHERE_CURVATURE: REJECTED
RADIAL_SCALAR_OPERATOR_AND_SOURCE_SIGN: PASS
MUTATION_SCALAR_POTENTIAL_SIGN: REJECTED
5D_TT_RADION_NORM_8_AND_FLUX_1_OVER_G4: PASS
MUTATION_FLUX_FACTOR_2: REJECTED
KK_VECTOR_EXPONENT_PLUS_2_SQRT3: PASS; NEGATIVE_EXPONENT_REJECTED
SECOND_ORDER_UNITARY_COMPLETION_PURE_GAUGE: PASS
MUTATION_OMIT_HALF_LIE_SQUARED: REJECTED; spurious qzz0=-k^2(c^2+b^2/f)/2
SCOPE: exact identities only; not a GL eigenmode, C0, or nonzero-news proof
```

The direct-Christoffel executable check uses arbitrary R on a **flat**
three-dimensional base. General-base identities are analytically derived
above; the executable does not pretend to test arbitrary base curvature.
No ODE/growth benchmark, normalized physical H, full inhomogeneous metric
solution, overlap sign, numerical C0 or amplitude-error bound was tested.
No external paper was newly used as proof in this audit; its distinct
calculation pathways are explicit. This is an independent derivational
review within one AI system, not a human referee endorsement.
