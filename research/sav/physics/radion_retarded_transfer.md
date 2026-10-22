# Radion retarded transfer, flux and scope of the exponential law

Task `T-20261022-GLR-GOAL-001`. Use the field, amplitude and no-factorial
conventions of `quadratic_radion_source.md`. No numerical C0 is implied by
the formulas in this document. Computational results, if accepted, are
recorded separately with their residuals and limitations.

## 1. Laplace Green function and normalized coefficient

Let \(x=r_*=r+r_0\ln(r/r_0-1)\), \(s=2\Omega>0\), and
\(L_s=-\partial_x^2+V+s^2\), \(V=fr_0/r^3\ge0\).
For Fourier convention \(e^{-i\omega t}\), the growing source has
\(\omega=is\). The analytic continuation of a horizon-ingoing wave
is \(y_H\sim e^{sx}\) as \(x\to-\infty\); the infinity-outgoing wave
is \(y_\infty\sim e^{-sx}\) as \(x\to+\infty\), both with coefficient 1.
Define

\[
 W=y_H\partial_xy_\infty-(\partial_xy_H)y_\infty,\qquad
 G_s(x,x')=-\frac{y_H(x_<)y_\infty(x_>)}{W}.\tag{1}
\]

Then \(L_sG_s=\delta(x-x')\); the minus sign follows from the jump
\(G'_+-G'_-=-1\). Positivity of \(V+s^2\) implies positive bases and
\(W<0\). There is no upper-half-plane scalar pole: a square-integrable
homogeneous solution would give
\(\int (|y'|^2+(V+s^2)|y|^2)dx=0\). Thus the source-to-infinity response is

\[
 \boxed{C_0(k r_0,\Omega r_0)
 =-\frac{1}{r_0W}\int_{-\infty}^{\infty}
 y_H(x)\,\frac{\sqrt3 k^2 r f}{16}\mathcal H(r)\,dx}
 =-\frac{\sqrt3 k^2}{16r_0W}\int_{r_0}^{\infty}y_H(r)r\mathcal H(r)dr.
 \tag{2}
\]

The normalization \(H_{\theta\theta}(2r_0)/(2(2r_0)^2)=1\) is essential.
For a rescaling \(H\mapsto bH\), the associated \(A\mapsto A/b\)
and \(C_0\mapsto b^2C_0\). An additive change of x shifts the numerical
coefficient relative to a fixed \(A(u)\); the stated x convention fixes it.

Horizon smoothness in EF coordinates implies
\(\mathcal H=e^{2\Omega x}\times\) a smooth bounded function of r near
the future horizon. Although individual Schwarzschild component squares
diverge, their Lorentzian contraction does not. The integrand in x is
\(O(f e^{4\Omega x})\). At infinity, the GL radial exponential is
\(e^{-q r}\), \(q=\sqrt{k^2+\Omega^2}>\Omega\). Its square dominates the
\(e^{sx}\) growth of \(y_H\), so (2) converges at both ends. Numerical
work must preserve these cancellations before quadrature.

The Green kernel is positive, but \(\mathcal H\) is a Lorentzian tensor
contraction. Positivity of the kernel alone does **not** prove \(C_0\ne0\).
One must prove sign control for the particular on-shell mode, or calculate
the signed overlap with an error smaller than its distance from zero.

## 2. An exact noncancellation check at the static threshold

At a genuine static GL threshold \(\Omega=0\), time reversal allows
\(H_{tr}=0\). A nontrivial static TT perturbation then has
\(\mathcal H=T^2/f^2+f^2D^2+2K^2\ge0\), nonzero on an open radial set.
The regular scalar bases are explicitly

\[
 y_H=r/r_0,\quad y_\infty=-(r/r_0)\ln f,\quad W=-1/r_0.
\]

The limit of (2) is therefore

\[
 C_0(k_{\rm cr}r_0,0)
 =\frac{\sqrt3 k_{\rm cr}^2}{16r_0}
       \int_{r_0}^{\infty}r^2\mathcal H(r)dr>0.\tag{3}
\]

This is a static scalar charge response and has **zero news at threshold**.
It proves that the scalar-source/overlap construction does not cancel
identically on the threshold mode. If the normalized unstable GL branch
and its horizon-regular tail bounds vary continuously to that threshold,
dominated convergence implies C0 remains positive in some interval below
threshold. Equation (3) by itself gives neither the size of that interval
nor the sign for the four requested wavelengths. A claim about an unstable
mode must state the continuity assumption or establish it separately.

## 3. Causality, switching and an exponential response

For specified compatible Cauchy data on a regular horizon-penetrating slice,
the physical answer is
\[
 \psi(t,x)=\psi_{\rm hom}[\text{initial scalar data}]
 +\int_{\text{causal past}}G_{\rm ret}(t,x;t',x')
           S_0(t',x')\,dt' dx'.\tag{4}
\]
The scalar initial data are independent second-order physical data; the
spherical metric constraints must be solved consistently with the GL data.
"No incoming radiation" alone does not uniquely eliminate arbitrary
outgoing homogeneous scalar data on that slice.

An exponential source extended to the infinite past is a well-defined
Laplace forcing because it tends to zero there. The stable scalar resolvent
selects its unique response, and at infinity
\(\Phi(u)=r_0 C_0 A(u)^2\) exactly at quadratic perturbative order. Extending
that exponential to arbitrarily large future amplitude is not a physical
weakly nonlinear history. A finite-source-time regulator may define the
retarded scalar integral, but multiplying the linear metric by an arbitrary
switching function violates its first-order vacuum equations/constraints.
It must not be presented as a new vacuum GL history.

For admissible finite-time GL initial data, subtracting the particular
exponential response leaves a homogeneous scalar transient determined by
the initial scalar data (and the slice). Since the scalar operator is
nonnegative, it has no growing scalar eigenmode. This explains isolation
of the exponential coefficient after transients become small; it does
not provide an error bound at a finite u without estimating those data
and propagation tails. For a general actual first-order history, evaluate
the complete quadratic source and convolution (4); do not reuse a constant
C0 when the radial mode/time dependence changes.

## 4. Independent flux normalizations

For the convention
\(ds_5^2=e^{-2\phi/\sqrt3}ds_E^2+e^{4\phi/\sqrt3}(dz+2A)^2\),
the uncharged scalar action is
\(S_\phi=-(8\pi G_4)^{-1}\int\sqrt{-g_E}(\nabla\phi)^2\),
\(G_4=G_5/L\). Its stress tensor is
\(T_{\mu\nu}=(4\pi G_4)^{-1}[\partial_\mu\phi\partial_\nu\phi
-\tfrac12g_{\mu\nu}(\nabla\phi)^2]\).
For \(\phi=\Phi(u)/\rho+O(\rho^{-2})\), integration over the sphere gives

\[
 P_0=\frac{dE_0}{du}=\frac{N_\phi^2}{G_4},\quad
 N_\phi=\partial_u\Phi,\quad
 \frac{E_0}{M_0}=\frac{2}{r_0}\int N_\phi^2du,\quad
 M_0=\frac{r_0}{2G_4}.\tag{5}
\]

Independently, at each far-zone radial ray the 5D scalar polarization in
a transverse TT basis has
\(h_{11}=h_{22}=-2\phi/\sqrt3\), \(h_{zz}=4\phi/\sqrt3\), with zero
transverse trace. Thus \(\dot h_{IJ}\dot h^{IJ}=8\dot\phi^2\).
The Einstein-Hilbert quadratic outgoing energy current has coefficient
\(1/(32\pi G_5)\); multiplying by \(4\pi\rho^2L\) gives exactly (5).
The current follows equivalently from the EH TT quadratic kinetic term;
no temporal high-frequency average is needed for the leading scalar
symplectic flux (an averaging notation sometimes used for Isaacson flux
must not introduce a cosine factor here). Asymptotic curvature corrections
vanish at this order. See the foundation audit for the boundary convention.

For a settled exponential response, define \(\hat\Omega=r_0\Omega\).
Translation symmetry by half a compact period makes zero-mode observables
even in A for the restricted pure-mode family. Then

\[
 \frac{d(E_0/M_0)}{d(u/r_0)}
 =8\hat\Omega^2 C_0^2 A^4+O(A^6),\qquad
 \frac{E_0(u_1,u_2)}{M_0}
 =2\hat\Omega C_0^2(A_2^4-A_1^4)+O(A_2^6),\tag{6}
\]
provided an independent transient is negligible and higher-order response
norms are controlled. Reducing \(\epsilon\) by ten shifts a fixed weak
amplitude event by \(\ln10/\Omega\); it does not change (6) at fixed A.

## 5. Spectrum and amplitude control

For finite-energy news the positive-frequency convention is

\[
 \frac{dE_0}{d\omega}=\frac{|\widetilde N_\phi(\omega)|^2}{\pi G_4},
 \quad\widetilde N=\int e^{i\omega u}N(u)du,\quad\omega>0.\tag{7}
\]
For an integrable waveform with controlled boundary terms this becomes
\(\omega^2|\widetilde\Phi|^2/(\pi G_4)\). Scalar memory requires the
news form. A growing exponential over all u has no finite total energy
or Fourier spectrum. A **windowed news contribution** from an interval
where (6) holds can be written without inventing a rollover:

\[
 \widetilde N_{[u_1,u_2]}(\omega)
 =s r_0 C_0\frac{A_2^2e^{i\omega u_2}-A_1^2e^{i\omega u_1}}
                      {s+i\omega},\quad s=2\Omega.\tag{8}
\]

Its spectrum integrates to the interval energy in (6) by Parseval, but
sharp endpoints create artificial spectral tails. Equation (8) transforms
news times an observation window, **not** a discontinuously truncated
waveform; differentiating a truncated waveform would add delta functions.
It is not the full physical burst spectrum. Smooth tapering, initial
transients and eventual nonlinear rollover determine that spectrum.

A controlled quantitative A interval requires a fixed regular frame,
\(A\|H\|\ll1\), \(A^2\|q_2\|\ll1\), and a bound on the next even
scalar response relative to the quadratic one, plus the transient bound.
The invariant reference amplitude alone does not supply these bounds:
Schwarzschild component divergences near the horizon cannot set them.
If those response norms are not computed, (6) is an asymptotic conditional
law, not a validated finite-A emitted percentage. A~1 and the first-bulge
energy fraction need additional weakly nonlinear dynamics or physical
data. The FFG&A Table 2 windows in `timescales_energy.md` are geometric
events and do not supply that missing waveform or amplitude calibration.
