# Obstructions and open estimates

## Precise limits of the new results

| Proposed shortcut | Test and consequence |
| --- | --- |
| Set the physical length to its Z initial value at a fixed optical/areal tube | On the pinned growing mode the explicit quadratic length correction is strictly negative at sufficiently early reference intersections. The original exact Z scalar jet still vanishes on the initial hypersurface. Transform the slice and the cut tangent separately. [Calculation](DERIVATIONS.md#4-quadratic-matching-and-the-nonzero-finite-anchor-correction) |
| Treat weighted background flux as the same estimate for the exact massive sectors | The compact projection does not commute with the dynamical principal part. Terms such as \(P_o((g^{ab}-\bar g^{ab})\bar\nabla_a\bar\nabla_b h)\) include high incoming derivatives and mean–massive couplings. The mass controls the time-commuted field in the fixed operator, while the nonlinear time commutator differentiates the coefficients. [Relative gauge terms](EXTERIOR.md#3-relative-wave-gauge-and-its-nonlinear-cost) |
| Bound all harmonic-gauge time deformations by \(C/r\) using only uniform small news | A massless outgoing scalar in the weak-field harmonic model forces a \(u\)-dependent \((\log r)/r\) term. The perturbative calculation challenges the all-component coefficient ansatz. Null-component estimates or a geometric gauge can treat it differently. [Harmonic model](EXTERIOR.md#3-relative-wave-gauge-and-its-nonlinear-cost) |
| Use massless \(r^p\) estimates without accounting for the compact mass | Multiplying \(4q_{uv}+Vq=0\) by \(r^p q_v\) puts \(\partial_v(r^p V)q^2/2\) on the forcing side. It has the adverse sign when \(p>0\) and \(V\to K^2>0\). The time-commuted weighted current in [DERIVATIONS §1](DERIVATIONS.md#1-weighted-causal-massive-jets-on-the-reference-exterior) uses a lower radial weight to bound the unsigned term. |
| Insert a finite-radius compact tilt and extend outward | In flat product spacetime a stationary outgoing eikonal with periodic initial slope \(p=a'(z)\) transports \(z=z_0+(r-A)p/\sqrt{1-p^2}\). Its Jacobian contains \((r-A)a''/(1-p^2)^{3/2}\), giving a caustic for every nonconstant smooth periodic \(a\) with small slope. [GOAL-006 §4](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md#4-optical-clock-and-geometric-news) |
| Infer terminal compact injectivity from absolute dyadic flux for general metrics | A manufactured \(e r^{-\nu}\cos(kz)dr^2\) tail with \(3/2<\nu<2\) satisfies the strengthened dyadic size test and develops a backwards conjugate point as the terminal radius recedes. This metric violates vacuum. The vacuum \(rr\) equation supplies a signed radial derivative that eliminates the problematic moment under the stated areal-chart hypotheses. [Construction](OPTICAL.md#6-negative-control-and-dependency) |
| Solve radial optical constraints and choose terminal constants independently of the Cauchy solution | The torsion constant \(C=\lim R^2P\) must vanish at a normalized reflection-symmetric product end under the stated decay assumptions. The supplementary equations fix additional data and the \(1/R\) mass aspect remains an integration function. Even setting the torsion constant to zero, the manufactured radial subsystem with \(\log R/R\) in \(V\) fails the scalar evolution. [Characteristic tests](checks/README.md#necessary-compatibility-at-a-product-end) |
| Let the instantaneous mass specify the Schwarzschild logarithm | A \(u\)-dependent mismatch in a clock shift can grow like \(\epsilon^2\log r\). For \(n\) smooth and compactly supported, \(\epsilon^2 n(u+\epsilon^2\log r)\) has the right leading Taylor coefficient on every finite tube and vanishes in the fixed-seed limit \(r\to\infty\). The positive-mass background subtraction uses fixed ADM mass. A dynamical normalized eikonal still needs an actual comparison estimate. |
| Deduce tube derivative convergence from a field or fixed-\(v\) derivative limit | For the free incoming tail \(H=\epsilon^4\sin(v^2)/v\), both field trace and fixed-\(v\) \(u\)-derivative vanish. On \(v=u+2R\), its \(u\)-derivative contains \(2\epsilon^4\cos(v^2)\), whose squared integral tends to \(2\epsilon^8|I|\). This inherited control remains applicable. [Work audit §4](https://github.com/inaeyk/GL-with-AI/blob/c4cac8f2e2dd12cfbdf50651863d2188a75fc87c/research/sav/verification/AUDIT_GOAL006_80f030df.md#4-physical-cuts-news-and-energy) |
| Use the mean \(g_{zz}\) as the length or the mean null surface as a physical characteristic | A periodic reparameterization changes the mean of \(g_{zz}\) at second order with circumference unchanged. A compact shift \(X=\alpha\partial_u\) gives \(g^{uu}=\alpha^2\) on a mean-null slice in the manufactured product example. [Exact controls](checks/README.md#negative-controls-and-reproducibility) |
| Assert pointwise massive master positivity everywhere outside the horizon | The homogeneous frozen potential is negative near the horizon for both relevant harmonics. Far-exterior tensor estimates use the explicit positive compact term and bound curvature separately. [Potential calculation](checks/README.md#flux-and-background-cone-checks) |
| Infer a quartic remainder from evenness | \(f_\epsilon=\epsilon^2 f_2+|\epsilon|^3g\) is even and has the required quadratic coefficient. Uniform estimates in a trace norm or an equivalent differentiated parameter bound are needed. |

The Ricci sign, inverse compact lapse, compact-shift term and loss of the
quadratic zero-mode source are rejected by the symbolic mutations in
[the saved check](checks/check_optical_identities.txt).

## Next exact estimate

The primary geometric route uses the full optical local-area equations
to avoid a pure incoming derivative square in the deformation. It
requires a positive commuted energy for the coupled system, including
the compact derivatives in the radial integrations for \(P,A,V\).
Even the linear terminal elimination exhibits the issue explicitly.
In the local area chart of [the independent calculation](checks/README.md),
linearize at \(b=B=1\) and set the reflected torsion charge to zero.
For \(W=R\log b\) the constraints give
\[
 P=-\frac{6}{R^2}\int_R^\infty W_z(s)\,ds,\qquad
 A=\frac{6}{R}\int_R^\infty W_z(s)\,ds
      -3\int_R^\infty W_z(s)\frac{ds}{s}.
\]
The compact principal term in the scalar equation then contains
\[
 (I-6\mathcal H)W_{zz},\qquad
 (\mathcal H f)(R)=\int_R^\infty(1/R-1/s)f(s)\,ds .
\]
The sign displayed is for this term on the same side as
\(-2W_{uR}+(fW_R)_R-(f_R/R)W\).
The integral carries two compact derivatives and cannot be
classified as a bounded zero-order source in an unweighted
commuted energy. For example, for a radial power \(R^{-a}\),
\(a>1\), the compact coefficient has eigenvalue
\(1-6/[a(a-1)]\). It vanishes at \(a=3\). This illustrates why
the log example in the radial subsystem requires the coupled
scalar equation and why coercivity on a chosen radial domain
must be checked. The fixed-background unitary tensor estimate avoids this integral.
[The linear optical normal form](DERIVATIONS.md#6-a-radial-normal-form-for-the-linear-optical-principal-part)
also removes its compact derivative loss at the operator level.
Its terminal radial inverse changes the Cauchy problem, and the
dynamical coefficients and integration constants still enter a
nonlinear estimate.

The concrete next target is a uniform Schwarzschild-relative
exterior estimate for the constrained scalar/shift system on
finite truncated wedges with constants independent of the outer
boundary. Establish the compact-principal estimate without losing
a compact derivative, propagate the two supplementary constraints
from compatible data, and verify a commuted forcing bound with
\(\delta<1\). The optical normal form then removes one separate
first-moment condition. A comparison of the mean lapse and clock,
a quartic mean forcing estimate and the asymptotic flux regularity
are still required for physical news.
