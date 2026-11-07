# GLR-GOAL-007 report

**Outcome A for an early finite-data axial response. Outcome B for
ringdown and the general polar response.** Pure five-dimensional
vacuum gravity permits a growing spherical GL mode to modify
massless quadrupolar tensor news. We find a nonzero
$\epsilon^2\eta$ coefficient for specified smooth, finite-energy
axial data at fixed perturbative ADM mass and fixed Bondi frame.
The result is an ordered perturbative radiation trace on early
far-exterior cuts. A strong-field ringdown coefficient and a
uniform nonlinear Bondi expansion remain open.

## Physical result

The mixed order $\epsilon\eta$ response is $\ell=2$ with $n=\pm1$.
The second interaction with the growing GL mode returns part of
this response to $n=0$. The $\epsilon^2$ monopole geometry and
the direct cubic Einstein terms also enter at $\epsilon^2\eta$.
All three contributions, the constraints and the gauge completion
are included in the [hierarchy](HIERARCHY.md). Exact $SO(3)$ alone
would remove all tensor radiation, and the compact translation
symmetry removes odd powers of $\epsilon$ from this zero mode.

The [axial calculation](INVESTIGATOR.md) treats the nonspherical
amplitude linearly on the spherical GL family and uses the exact
three-dimensional axial Proca equations on the $(t,r,z)$ orbit.
Fix the canonical axial pair on a finite slice in the unitary
compact gauge. A right-going profile with leading shear
$aF_j(u-u_*)$, $F_j(s)=s_+^j/j!$, supplies an identifiable front.
The order-one outgoing optical label changes by
$-\chi_\infty\cos kz$ after the GL interaction. The exact
front transport gives the coefficient
$$
C_{21,AB}=a\,\frac{\chi_\infty^2}{4}\,F_{j-2}(u-u_*)X_{AB}
   +O((u-u_*)_+^{j-1}),\qquad N_{21,AB}=\partial_u C_{21,AB}.
$$
The factor $1/4$ is the second-order transport factor $1/2$
times the compact mean of $\cos^2 kz$. The direct second-order
orbit deformation, lower-derivative interactions, a constant
time shift and a mass-family derivative contribute at most one
front derivative in the shear. The common-frame retarded trace
preserves the two-derivative term. The proof uses short-strip
bounds for the massive intermediate and a scalar mean RW
equation with integrable forcing. The mean radion cancels from
the axial gravitational equation at this order.
[Trace and constraint proof](INVESTIGATOR.md) · [Dual check](AXIAL-TRACE.md)

For $100\le R/r_0\le101$ the analytic L10 far-tail envelope gives
$2\times10^{-87}<(\chi_\infty/r_0)^2/4<6\times10^{-46}$.
This bounds the leading front coefficient relative to the ordinary
shear, with derivatives taken in $u/r_0$. The bounds are loose and
the far-exterior signal is small. For one real unit-normalized
$\ell=2$ harmonic the tensor energy flux is
$12|\partial_u\Psi_{\mathscr I}|^2/(32\pi G_4)$.
The cross term $2\,\partial_u\Psi_{01}\partial_u\Psi_{21}$
is strictly positive on a sufficiently short interval to the
right of the rising front after integration. Continuity of the
trace gives smooth compact preparations with the same strict
inequality and preserves separation from mass/time templates.
The sign integrated over a whole waveform depends on the data.
[Amplitude bounds and normalized energy](WITNESS-BOUNDS.md)

## Ringdown interpretation

The response depends on the constraint-compatible higher-order
radiation data. Adding a compact homogeneous zero-mode RWZ wave
at $(2,1)$ changes the coefficient while preserving both first
jets and the charges. The finite-slice data used in the axial
witness fix this freedom. The witness probes direct propagation,
before signal from the strong-field region can reach those cuts.

For ringdown, prescribe a finite wave packet and its matched
higher-order data, then propagate the $(1,1)$ constrained
massive response and project the full $(2,1)$ source through
the massless retarded inverse. A QNM residue diagnoses a pole
of the continued Green function after applying that finite-data
source. In a separated source term $e^{\Omega t}$ shifts the
Laplace argument by $-\Omega$ per growing GL factor. The
spherical second-order response and the initial slice also
supply transient terms. These ingredients permit time-dependent
changes of the ringdown waveform. They do not determine a
universal shift of a stationary QNM frequency. A separate
calculation near the potential peak is needed to evaluate a
specific strong-field residue. [Retarded formulation](HIERARCHY.md)

## Rotating strings and mergers

The rotating literature distinguishes the linear massive TT
spectrum of five-dimensional Kerr strings from the nonlinear
four-dimensional Einstein–Weyl backreaction evolved by
East–Siemonsen and the six-dimensional vacuum black resonators
and helical strings of Dias–Ishii–Murata–Santos–Way. The recent
separated massive-spin-two Kerr calculation independently maps
the competition among the GL branch and three sampled
superradiant branches. The compactification discretizes that
map through $\mu=|n|2\pi/L$. Its branch dominance and binding
results are linear. [Versioned rotating map and audit](LITERATURE.md)

The exact uplift of a four-dimensional vacuum merger preserves
the ordinary tensor waveform and has an invariant zero-KK
subspace. A pre-existing compact seed propagates by a massive
TT equation on the time-dependent merger geometry. Its quadratic
mean source can modify tensor radiation. A local neck estimate
$\int \Omega_{\rm loc}\,d\tau$ needs a justified approximate
string patch, mode capture and separation of scales. The
available noncompact merger analogues establish no such
criterion for this uplift. [Merger formulation](MERGER.md)

The [independent review](INDEPENDENT-REVIEW.md) includes a blind
hierarchy reconstruction and a separate disclosed audit of the
front argument. The [reproducible checks](CHECKS.md) cover local
Einstein coefficients, pure gauge, constraints, compact
projections, flux and a manufactured retarded response. Their
scope is separate from the physical tail and trace argument.
