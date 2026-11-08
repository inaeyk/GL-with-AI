# GLR-GOAL-015 — the ordered tensor interference changes sign

**Full outcome for the fixed ordered benchmark.** The same real
canonical axial perturbation, evolved with the physical two-polarization
GL source at $L/r_0=10$, has a strictly positive early projection and a
strictly negative full-packet projection. These are directed enclosures
of the complete physical *ordered* calculation at null infinity.

| Matched cut | Directed physical $\mathcal P=\int N_{01}N_{21}\,du$ | Ordered tensor energy coefficient in $r_0/G_4$ |
| --- | ---: | ---: |
| $[a,a+0.015]$ | $[3.3605,12.3142]$ | $[0.80226,2.93980]$ |
| $[a,a+0.12]$ | $[-2758.258,-200.091]$ | $[-658.486,-47.768]$ |

Here $a=-13/8-2\log(5/8)$ and $G_4=G_5/L$. The coefficient is
$3r_0\mathcal P/(4\pi G_4)$ at order $\epsilon^2\eta^2$, with the
same mass, circumference, inertial clock and physical cuts.
The accumulated ordered correction must cross zero between the two
terminal cuts, and the contribution from the rest of the packet is
strictly negative. The calculation does not locate that crossing.
The benchmark and normalization are pinned in [INPUTS](INPUTS.md);
[energy and frame](observables/ENERGY.md) explains the extraction,
sign and conditional finite-amplitude interpretation.

The early weak proof uses a real rotation of the mixed polarizations.
The outgoing phase and the transverse mixed $F$ row align in one
component, so the principal signed terminal primitive is an exact
phase square times $B''$. A coupled characteristic enclosure controls
the other polarization and all finite-width terms. The independently
checked early weak interval was $[1.70019,13.47799]$; the frozen-source
replay tightened it to $[1.70707,13.47110]$. The distinct strong proof
commutes the lower hierarchy once in stationary time, includes the
nonzero shifted graph data and recovers cap news using only $F_t$,
the uncommuted traces and a Volterra shear. With a separately directed
radial phase it gives $E_c<323.984$ for the pinned affine candidate,
below both the design target 400 and the inherited sufficient 570.
It yields the tighter interval in the table. These proofs share the
physical source and coefficient enclosures, with separate strong and
weak error estimates. [Weak derivation](analysis/ADJOINT.md) ·
[Strong derivation and budget](observables/EARLY-CERTIFICATE.md).

For the full packet, the early derivative ratios fail at the zeros of
the bump derivatives. The first absolute error estimate spans zero.
The successful refinement keeps the signed subprincipal $B'$ term of
the $F$ primitive and subtracts its constant part from the bulk test
using $\int B'B''=0$. The proof carries the true scattered endpoint
test, the remaining terminal $F$ error, both mixed polarizations,
and the independent exterior cancellation. The packet certificate
uses no packet pilot values. The refined empirical candidate is near
$-1455.615$ and helps interpret the scale, but the directed interval
is wider. The separate strong packet gate $E_c\le1451$ remains
unevaluated. [Packet proof](observables/PACKET.md) ·
[Adversarial packet audit](verification/PACKET.md) ·
[Compact pilot and lift](checks/COMPACT.md).

The source track also derived a local fixed-mass formula from
background conservation and checked it against radial quadrature.
It enclosed the early mean, direct and affine return pieces; their
combined local contribution lies in $[0.21084844,0.34618244]$ in the
specified exact-curl split. The leading direct and affine outgoing
coefficients cancel in a calculable fraction. These splits are
useful diagnostics, not additional invariant observables.
[Source enclosures and negative controls](source/ENCLOSURES.md).

The result gives a two-cut benchmark for the perturbative tensor
response to GL growth. A full finite-amplitude comparison requires
uniform mixed derivatives of the nonlinear solution-to-news map on
matched physical cuts. A normalized energy remainder below $3.3605$
would suffice to preserve both signs, with the hypotheses in
[ENERGY](observables/ENERGY.md); no such remainder or seed radius is
certified here. The early strong norm also gives no late ringdown
error estimate. [Handoff](HANDOFF.md) · [Acceptance matrix](ACCEPTANCE.md)
· [Reproduction and review scope](REPRODUCTION.md).
