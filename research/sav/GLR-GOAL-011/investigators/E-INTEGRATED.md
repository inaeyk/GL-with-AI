# Integrated adverse audit E

This second pass is non-blind. It reads the integrated GOAL-011
HIERARCHY, FINITE-DATA, WAVEFORM, GREEN-FUNCTION, barrier_delay script
and output, the separately derived C resonance note, and the prior
GOAL-007 external audit. The earlier independent reconstruction and
its exposure record remain in glr011-E.md. Here I distinguish algebra
and bounds checked on paper from execution of a shared implementation.

## Exact mean, slice, and null trace

The densities, alternating orientation, and signs in HIERARCHY (5)
agree with my independent mean elimination. In \((\tau,r)\), use
\(\sqrt{|b_0|}=1\), \(b_0^{\tau\tau}=-(1+s)\),
\(b_0^{\tau r}=s\), \(b_0^{rr}=f\). At \(\Sigma\) both the exact
mean order-two \(Q_z\) and the free \(A_{2r}\) vanish. The \(\tau\)
equation gives
\(A_{2\tau}=\mathcal D^\tau/[\lambda r^2(1+s)]\) there. The \(r\)
equation then gives
\(\partial_\tau Q_{2z}=\mathcal D^r+s\mathcal D^\tau/(1+s)\).
For the baseline the same \(r\) equation gives FINITE-DATA (1) and
\(\partial_v\Psi_0|_\Sigma=0\). This agrees with the dual rotation
\(Q_z=\pi^r\), \(Q_r=-\pi^z\),
\(\widetilde\pi^r=\lambda A_z\), \(\widetilde\pi^z=-\lambda A_r\).
The compact initial tangential curl and the dual Gauss numerator
vanish for this baseline pair, so the initial \(Q_n=0\) is
consistent. Changing to the background curl also changes the initial
position and velocity by the corresponding lower-order curl jet.

The exact source sign can be tested by setting \(\mathcal D=0\).
Then (5) is \(L_{\rm RW}\Psi_2=-2 f\mathcal C/r^3\).
The flat-curl equation has the extra
\(-L_{\rm RW}(2\mathcal C/(\lambda r))\); conflating the two fails
even with identical forcing metric. Both conventions yield the same
asymptotic trace when their data, gauge completion, and extraction
are matched.

For unit scalar harmonic norm, the angular norm is \(12\), the
CPM shear differs from the raw curl by \(-1/2\) at \(\ell=2\),
and the tensor flux normalization and interference cross term are
consistent. With the specified compact bump, the full support-length
integral of \(b'b'''\) is \(-\int(b'')^2\), and a small enough fixed
fractional subwindow gives positive interference. Those signs use
the ordered coefficient and sufficiently small width, without a
certified numerical threshold in \(\delta\) or nonlinear error.

## Tilted differentiated estimate

The graph \(v_\Sigma(u)\) has slope \(-f/(1+s)\) on each launch
neighborhood, so its inverse and finitely many derivatives are
uniformly bounded for small widths. Write
\(a(v)=\max(u_*,u_\Sigma(v))\) for the lower \(u\) endpoint at fixed
\(v\), with \(u_\Sigma(v)\) here the inverse graph. On the compact
triangular region,
\[
 w_{j,v}(u,v)=w_{j,v}|_\Sigma(a(v))+
 \int_{a(v)}^u \mathcal F_j(\bar u,v)\,d\bar u,\quad
 \mathcal F_j=A w_{j,u}+B w_{j,v}+D w_j+s_j.
\]
For \(v\ge v_\Sigma(u_*)\), replace the slice term by zero and
use the constant left endpoint. At the transition, compact bump
flatness removes all endpoint jumps. In the early triangle
differentiation gives
\[
\begin{split}
 w_{1,vv}(u,v)={}&a'(v)
 \big[(w_{1,v}|_\Sigma)'(a(v))-\mathcal F_1(a(v),v)\big]\\
 &+\int_{a(v)}^u[
 A_v w_{1,u}+A w_{1,uv}+B_v w_{1,v}+B w_{1,vv}
 +D_v w_1+D w_{1,v}+\partial_v s_1](\bar u,v)\,d\bar u .
\end{split}
\]
The moving endpoint term is essential. The one \(u\) derivative of
the initial \(w_{1,v}|_\Sigma=O(A_\delta/\delta)\) costs
\(O(A_\delta/\delta^2)\) by the algebraic electric and divergence
relations. At the baseline \(w_{0,v}|_\Sigma=0\);
integrating its scalar RW equation gives
\(w_{0,v}=O(A_\delta\delta)\),
\(w_{0,vv}=O(A_\delta)\),
\(w_{0,vvv}=O(A_\delta/\delta)\), including the two successive
moving-endpoint terms. In particular the third \(v\) derivative
that can occur in \(\partial_v s_1\) costs at most
\(O(A_\delta/\delta)\). Also
\(w_{0,uuv}=O(A_\delta/\delta)\) from its equation.
Thus \(\partial_v s_1\) and the other inhomogeneous terms in
the displayed mixed estimate are \(O(A_\delta/\delta^2)\)
in the mixed weighted norm. The term \(B I_u w_{1,vv}\) is
absorbed by the same small-width bound. This explicit recurrence
closes the \(w_{1,vv}\) term in the returning source.

For the estimates of \(w_u\), use the given Volterra equation
beyond the triangle and the bounded tilted data on it. For pure
\(u\) commutations after the triangle, differentiating the
integral either hits a smooth coefficient or removes the
\(u\)-integral and lowers the derivative count of the field.
On the triangle, differentiating the \(v\) integral creates
a slice endpoint term. One \(u\) derivative of a moving endpoint
costs at most one packet scale because the graph is smooth and
noncharacteristic. The required finite-order initial derivatives
follow from the same first-order slice system and the wave
equation. These checks give the baseline \(u,u u\) remainder
sizes \(O(A_\delta)\), \(O(A_\delta/\delta)\), and the mixed
\(u,u u\) remainder sizes \(O(A_\delta/\delta)\),
\(O(A_\delta/\delta^2)\). The leading transport cancellation
uses \(A e_z=0\) on the product even in the massive Fourier
sector. Compact derivatives can enter zero-order matrices there.
The first and returning principal transport coefficients are
\(-\chi\cos kz\) and \(\chi^2/4\) for ordinary Taylor coefficients.
The resulting integrated returning forcing is bounded at
\(O(A_\delta/\delta^2)\); one final \(u\) integration gives
the shear remainder \(O(A_\delta/\delta)\), while its differentiated
trace uses the forcing bound without that gain. These are
analytic scale estimates; no finite \(\delta\) was certified.

## Strong-field optical enclosure

In the given conventions \(y=(1+k^2r^3)U/r\),
\(p=U_x/U\). Substituting \(y_r/y=3k^2r^2/(1+k^2r^3)-1/r+p/f\)
and the trace relation \(fH_{rr}=H_{tt}/f-2H_{\theta\theta}/r^2\)
in the null contraction reproduces every constant and derivative
term in radial_ratio. The sign and the normalization \(U(2)<0\)
follow from \(H_{\theta\theta}(2)=8\). The integral has the
correct \(1/2\) from \(dv=2dx\) and the tilted launch factor
\(e^{\Omega(t_\Sigma-x)}\).

I inspected the imported interval Riccati code as well as its
call site. Its Hermite defect is enclosed cellwise, with positive
radial denominators and analytic endpoint brackets. The
forward comparison error is monotone in each cell. The backward
comparison error evolves monotonically toward the constant
cell equilibrium residual/rate. Hence the maximum of the
incoming and outgoing error bounds encloses the whole cell
in both directions. Reversing the validated cells preserves
each \(p\) enclosure and correctly reverses \(dx\). The update
\(U\mapsto U\exp(p_{\rm cell}\Delta x)\) encloses the full cell
integral of \(p\), while \(p_{\rm cell}[0,\Delta x]\)
encloses any partial step. The Riemann interval sum encloses
signed integrals across the change of sign. For \(r\ge60\),
\(p\in[-.65,-.60]\) and \(k^2=(\pi/5)^2\) bound the coefficient
radial_ratio even by \(2\) using triangle bounds. The code's \(5\)
is conservative. The tail then uses the proper decay exponent
\(0.60-\Omega\) in \(x\), and retains \(1/(2r)\le1/120\).

Execution used Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6,
SciPy 1.18.0:
- PYTHONDONTWRITEBYTECODE=1 python3 checks/barrier_delay.py piped
  through diff -u checks/barrier_delay.txt - gave no differences.
- A shared-code replay via runpy replaced main's global OMEGA in
  memory with the wider independently certified GOAL-005
  bracket [0.06328117,0.06328121]. The coefficient interval
  endpoints were
  0.2571914224604337237 and 0.3039356130825548194 at \(13/8\),
  and 0.0715887679240462861 and 0.0910886368061678044 at \(7/4\).
  The absolute weighted tail upper bound was \(7.187\,10^{-16}\)
  rounded upward. The displayed coarse bounds remain strict
  under the wider certified interval.

This was reproduction and a bounded parameter-box check of the
same Arb implementation. D's direct ODE quadrature checks central
values by a different implementation with ordinary numerical
accuracy. The Arb result certifies the optical coefficient of the
defined asymptotic packet expansion. It supplies no estimate of the
unknown asymptotic error constant for one particular width.

## Retarded pairing and preparation

GREEN-FUNCTION (1) and its directed graph signs pass the
constant-time free-wave test and direct Green flux calculation.
Equation (2) must refer to the transform of a whole outgoing
response for a specified source. The assertion that exchange
of spatial limit and transform “holds for the specified finite
causal windows” is too broad as written. A finite physical
trace window alone has no such consequence. C's proof for the
complete response to an auxiliary source tapered after the
constrained scalar projection supplies the needed distinct
domain and convergence conditions. Taking a transform of a
time-windowed waveform would be a different operation.

The two real upper-order graph data give a full-complex-rank
linear functional at a nonreal simple pole. If its range had
only one phase on all data supported in an open interval,
both \(w|_\Sigma\) and \(D_\Sigma w|_\Sigma\) would have that
phase, by varying position and derivative independently.
The tangent derivative would have the same phase, and
\(|T'|<1\) lets one solve for \(\partial_t w=-s_q w\),
which contradicts the nonreal frequency. Such data reconstruct
to constrained homogeneous axial data. Their support can lie
strictly inward of the cuts under comparison, with
\(u_\Sigma>U\); finite speed preserves the earlier signal.
They change the preparation, so this does not prove anything
about the numerical residue of FINITE-DATA (1) itself.

The late compact auxiliary-source counterexample is valid for
projected RW continuations. It does not imply tunability under
tapers of the fixed physical source; that requires a nonzero
physical sensitivity kernel in the chosen band. C's distinction
is necessary. A nonzero early waveform plus a simple continued
homogeneous RW pole still leaves the full source-data overlap,
coincidences with shifted resonances, and quantitative
late-time pole dominance open. No discrepancy was found in
the normalization or signs of the integrated short-packet
result. The transform-domain sentence above is the substantive
narrowing identified in the draft.
