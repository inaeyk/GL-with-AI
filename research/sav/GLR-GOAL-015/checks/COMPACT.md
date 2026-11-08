# Compact reconstruction, directed candidate projections and full-packet checks

These fixed-background checks use the ordered benchmark at [GOAL-014 source 0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/tree/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014), including the two normalized real mixed fields \((y,Z)=(U/\sqrt D,\Im A_{1z}\sqrt W)\). The [independent GOAL-014 audit 84c9824f826e3517c8a1ed74d96bea061ab88584](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/C-COMPACT.md) was visible before these checks. The new quadrature uses the continuous physical dual identity, rather than the source's discrete transpose or differentiated numerical shear. Its current ordinates are shared with the inherited time march. The enlarged solves reuse that march and coefficient evaluation. Agreement between the strong, weak and continued candidate calculations is an implementation check with shared lower fields.

Use dimensionless coordinates, \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\). Put \(R=13/8\), \(a=R-2x(R)\), \(\delta=3/25\), \(b(u)=r_\Sigma(u)\) for the canonical graph \(v=r\), and \(c=d+2x(400)\) on each respective cut. The early and packet widths are \(d-a=3/200\) and \(d-a=\delta\). The cap values differ between the two cuts. The physical benchmark keeps both mixed polarizations, fixed ADM mass and circumference, fixed inertial clock and Einstein-frame cuts, and the entire fixed-infinity mean.

## Exact current reconstruction and directed weak projection

Treat the binary \(F,G,H\) ordinates as exact real numbers. Their abscissae in \(u\) are \(a+j(d-a)/n\). Above \(v=R\) use bilinear interpolation with \(v\)-abscissae from the stored binary interior knots and exact endpoints \(R,c\). On the narrow graph wedge \(b(u)\le v\le R\), interpolate in \(u\) both the stored graph values and the stored \(R\)-row values. Then interpolate linearly in \(v\) between the two traces. Thus the reconstructed \(F_h\) ray integral \(\mathcal F_h(u)\) is the sum of a piecewise affine function of \(u\) from the rectangle and
[
 \mathcal F_{h,w}(u)=\tfrac12(R-b(u))(F_{h,\Sigma}(u)+F_{h,R}(u))
]
from the wedge. The exact front currents are zero. These current candidates are defined directly. They need not agree with the application of the physical current map to the conforming field lift described below.

For the physical test \(g=N_{01}\) the sharp weak formula in [GOAL-014, Eqs. (A5)–(A7)](../../GLR-GOAL-014/ADJOINT-AND-ENDPOINTS.md) uses the physical dual \(Z\) transported from infinity. Its exact terminal trace is \(Z(d,v)=g(d)\). After cap cancellation, use the candidate functional
[
 4P_h=g(d)\mathcal F_h(d)-\int_{D_c}(Z_u F_h+Z_vG_h+ZH_h),du,dv.
]
With \(J(r)=3/r-3/(4r^2)\), write \(Z_u=g'+J(r)g+\mathcal R_u\). An integration by parts in \(u\) combines the terminal and the \(g'\) term exactly. Thus
[
 4P_h=\int_a^d g(u)\mathcal F_h'(u),du
       -\int_{D_c}g(u)(JF_h+H_h),du,dv+\mathcal R .
]
Let \(A_F,A_H\) bound the uniform absolute ray integrals of the candidate and \(A_G\) its maximum absolute value. The same Volterra majorant as in (A10) gives
[
 |\mathcal R|\le (e^{M(d-a)}-1)\|g\|_1(M A_F+A_H+A_G).
]
In particular, \(\int_{b(u)}^c |Z_v|dv\le M\int_u^d S_g(z),dz=S_g(u)-|g(u)|\), where \(S_g(u)=|g(u)|+M\int_u^d e^{M(z-u)}|g(z)|dz\). The same integrated bound controls \(\sup_v|Z-g|\). This explains the remainder of the \(JF_h\) approximation and the retained bound on the full \(G_h\) term. It keeps the sharp upper cut.

Use \(g=B'-J(b)B+\Delta_g\), where \(B=\sqrt\delta,b_0/\|b_0'\|_2\) and \(b_0(y)=e^{-1/[y(1-y)]}\) with flat extension. On the early cut \(|\Delta_g|\le M(C-1)B\), \(C=(1-M/522)^{-1}\) by the [directed baseline bounds](../../GLR-GOAL-014/PHYSICAL-PROJECTION.md). On the full packet use \(|\Delta_g|\le M^2e^{M\delta}\int_a^u B\) from the first-scatters Volterra remainder. The rectangle part of the \(B'\) integration is exact in each affine panel using differences of \(B\) at the ends. The wedge derivative and remaining terms are integrated using Arb boxes over whole subcells. The \(J(r)\) weight decreases radially. Its enclosure on each \(v\)-panel uses the two extremal radii over the entire \(u\) window. Center its interval and bound the signed moment by the midpoint moment plus the width times the absolute candidate ray integral. This preserves the signs even where \(F_h\) crosses zero.

| Candidate functional | Early, 160 bits | Full packet, 192 bits |
| --- | ---: | ---: |
| Directed complete \(P_h\), rounded out | \([7.81166934,7.82206151]\) | \([-1493.883639,-1421.054163]\) |
| Signed terminal \(\mathcal F_h(d)\) | \(193.33900842644\) | \(0.098375208081832\) |
| Terminal projection \(g(d)\mathcal F_h(d)/4\) | \([19.94037034,19.94037144]\) | \([0.00014030,0.00023992]\) |
| Dual remainder \(|\mathcal R|/4\) | \(<0.004413818\) | \(<29.248299\) |

The early candidate lower endpoint exceeds the charter's 7.80 target. The 192-bit variant with twice as many \(u\)-subcells gives \([7.81171988,7.82201101]\). Dropping the early terminal changes the candidate to \([-12.128703,-12.118308]\). The terminal values differ slightly from the empirical Simpson calculations because the exact candidate uses different interpolation and integration.

For physical weak errors \(f_*,h_*,g_*\) defined respectively as uniform absolute ray integral errors for \(F,H\) and a uniform value error for \(G\), the full-packet sign follows from the independently useful condition
[
 4.927654673 f_* + 0.082180215 h_* +0.014275917 g_* +1.63462\times10^{-14}<1421.054163.
]
The weights are the pinned [GOAL-014 packet budget](../../GLR-GOAL-014/ERROR-BUDGET.md), which already includes the terminal in the \(F\) weight. For example, the input caps \((280,300,700)\) leave over 6.66 negative margin. These caps are sufficient tolerances. The calculation above supplies no bound on the physical mismatch itself.

## Early conforming two-field lift

For each polarization prescribe a continuous \(D_h=(Y_h)_{uv}\) using the same rectangle and wedge interpolation as above, but with exact binary drive ordinates captured from the mixed step. On the graph use the archived exact raw \((U_t,\Im A_{1z,t})\) ordinates, affine interpolation in \(u\), and exact stationary rescaling to \(Y_t\). Let \(p=-1/b(u)\), \(P_g=(1+p)Y_t/2\), \(Q_g=(1-p)Y_t/2\), so \(P_g=-b' Q_g\). Extend the incoming \(Q_g\) on a fixed \(v\) characteristic by its physical graph value at \(\alpha(v)=u_\Sigma(v)\) for \(v<R\), and use zero at \(\alpha(v)=a\) for \(v\ge R\). The lift is
[
 \begin{aligned}
 Q_h(u,v)&=Q_g(\alpha(v))+\int_{\alpha(v)}^u D_h(s,v),ds,\
 Y_h(u,v)&=\int_{b(u)}^v Q_h(u,z),dz,\
 P_h^{\rm field}(u,v)&=P_g(u)+\int_{b(u)}^v D_h(u,z),dz .
 \end{aligned}
]
For \(v\ge R\) the first term in \(Q_h\) is zero. The identities \((Y_h)_v=Q_h\) and \((Y_h)_u=P_h^{\rm field}\) follow from the fundamental theorem and the moving lower endpoint. On the front all prescribed data vanish. At \(v=R\), Fubini gives the exact wedge primitive
[
 Y_h(u,R)=\int_a^u \Big[-b'(s)Q_g(s)+\tfrac12(R-b(s))(D_\Sigma(s)+D_R(s))\Big]ds .
]
The rectangle antiderivatives of the bilinear drive are polynomial on each cell. The wedge integral in the implemented enclosure uses whole graph cells. The limiting wedge vertex at \(u=a,v=R\) is regular. The difference \(D_R-D_\Sigma\) is affine and vanishes at \(a\) in the first panel, and \((R-b(u))/(u-a)\) is nonzero and smooth there. Therefore the interpolated drive is locally Lipschitz on the strip. Across interfaces both \(Y_h\) and its gradient are continuous. Also \((\partial_t Y_h)_{uv}=\partial_t D_h\) distributionally with \(\partial_t=\partial_u+\partial_v\), so differentiating this drive introduces no seam measures. A bound for its time-commuted volume error remains separate.

This lift has the same exact raw affine graph jets and stationary rescaling as the [GOAL-014 directed graph calculation](../../GLR-GOAL-014/STRONG-NEWS-PILOT.md). Thus its single-\(+k\) incoming graph energy error is bounded by \(4.10\times10^{-7}\) under the pinned physical GL mode hypotheses. The [lift output](mixed_lift.txt) binds the drive, raw graph and interior radial knots by SHA-256. The overlap test of numerical seam enclosures is a check of the analytic equality. The candidate data remain reproducible from text sources.

The [directed probe](lift_probe.txt) uses the source track's real-axis GL and fixed-infinity mean boxes and the audited leading jet bounds. It enforces the physical matrix potential, skew term and both source rows. It finds the following bounds on the cell intervals for the two mixed residual components, rounded out.

| Region | \(\sup|R_y|\) | \(\sup|R_Z|\) |
| --- | ---: | ---: |
| Terminal ray at \(v=R\) | 3.228 | 1.646 |
| Entire first rectangular \(v\) panel on that ray | 42.093 | 22.994 |
| Entire last \(u\) panel × first rectangular \(v\) panel | 88.698 | 47.039 |
| Endpoint-side eighth by eighth subcell | 13.166 | 6.879 |
| Entire rectangular \(v\) panel 30 on the terminal ray | 31.131 | 19.610 |

For the last \(u\) panel and first \(v\) panel the corresponding bound on the magnitude of the difference between the map-on-lift current and the independent \(F_h\) is 28.998, reduced to 3.410 on the endpoint-side subcell. The larger intervals include the common variation of the fields, source and interpolants within the cell. They are upper bounds from interval substitution, not measured nonzero lower bounds for physical residuals. The probe therefore isolates the need for a centered whole-strip residual or a sharper observable-specific error estimate before using this lift to certify the current candidate. It does not supply a full directional norm, complete signed terminal ray mismatch, or physical compact cap defect.

## Enlarged full-packet solve and strong threshold

The first packet run used 128 retarded panels, 480 exterior advanced panels and 24 fixed-point iterations. A single refinement used 256, 960 and 28. Both recompute the canonical graph launches on the actual wider graph interval at \(r_c=400\), keeping the GL and mean outer integration endpoint at 430. The baseline, mixed and return equations and quadrature outside the changed width are the inherited pilot. The smaller and refined weak candidate results are \(-1455.61446963\) and \(-1455.61480450\). The refined mixed, direct and mean contributions are respectively \(-1459.02670976,+2.72300453,+0.68890073\). The distinct derivative treatment yields \(-1455.61471298\), and its potential-only continuation yields \(-1455.61471683\). The output warning recurred at the tighter tolerance, with successful solver exit. No production evolution was run. These comparisons share the same numerical GL mode splice, mean, candidate lower fields, and sampled currents within each grid.

For a separate strong gate, the exact piecewise affine \(p_c\) uses the refined binary ordinates and exact \(u\)-nodes. In the projection against the **true** baseline apply the diagonal second-scatter approximation
[
 g=B'-J(b)B+\tfrac12\int_a^u J(b(s))^2 B(s),ds+\Delta_2,qquad
 |\Delta_2|\le M^2(e^{\delta/4}/8+Me^{M\delta})\int_a^u(u-s)B(s),ds ,
]
which is [GOAL-013, Eq. (1)](../../GLR-GOAL-013/FLUX.md) in physical strip variables. The integral against \(B'\) is integrated by parts, since the actual bump vanishes at both cuts. The upper test value \(g(d)\) need not vanish. Use exact panel integrals for the squared affine norm and whole-cell signed integration for the two scatter terms. The refined evaluation gives
[
 \|p_c\|_2\in[2104.73069710,2104.73069711],qquad
 \int g p_c,du\in[-1456.072442,-1454.600893].
]
The little-endian binary64 ordinate SHA-256 is `f8039040c6b21af5780878e24f21584d13aa8285a58b4f36af54731162272b6b`.

Let \(E_c=\|w_{2,u}(\cdot,c)-p_c\|_{L^2(du)}\) on the full packet. The homogeneous exterior potential integral is \(J(400)\), and the zero-front primitive has operator norm \(2\delta/\pi\) on \(L^2\). Thus the exterior transfer has norm at most \(e^\alpha\) and distance to the identity at most \(e^\alpha-1\), for \(\alpha=2\delta J(400)/\pi\). The pinned physical exterior forcing norm before this transfer is \(<1.7\times10^{-6}\) by [GOAL-013, Eq. (1)](../../GLR-GOAL-013/investigators/B-SOURCE.md). Multiplying the resulting norm error by the audited \(\|g\|_2\le1.00093\) gives
[
 \left|\mathcal P_{\rm packet}-\int g p_c,du\right|
 \le 1.206637+1.001504 E_c .
]
Consequently \(E_c\le1451\) would certify strict negativity. The stronger \(E_c\le1400\) would leave over 51 of negative margin. The strong gate uses the complete physical cap error, not the measured difference between the two derivative treatments. The full-packet physical sign from this route remains conditional. A verified sign would give the matched-cut coefficient \(3r_0\mathcal P_{\rm packet}/(4\pi G_4)\) at order \(\epsilon^2\eta^2\).

The early relative-jet proof cannot be extended verbatim to the packet. At \(u=a+\delta/2\), \(B'=0\). By monotonicity up to this point, the Volterra bound gives \(|w_0-B|\le M(\delta/2)e^{M\delta/2}B<0.107 B\). Hence \(w_0>0\) on the midpoint ray and \(w_{0,u}=-\int_{b(u)}^v(V/4)w_0\ne0\) for \(v>b(u)\). Any uniform bound on \(|w_{0,u}-B'|/|B'|\) therefore fails there. At the terminal cut the pinned directed test satisfies \(g(d)\in[0.0057047207,0.0097549333]\) despite the flat bump. An absolute or weighted error estimate on the full packet is required.

## Reproduce

Run from `research/sav` with Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0, and python-flint 0.9.0. The first packet run and refinement took about 6.4 and 14.3 wall seconds with one OpenBLAS thread. Capturing the early mixed drive took about 7.2 seconds. The directed candidate gates each took less than one second. Retain only the text outputs and digests. The binary arrays can be regenerated in scratch.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
g=GLR-GOAL-015/checks
scratch=$(mktemp -d)
python3 GLR-GOAL-014/checks/c_compact.py --n 192 --m 720 --cap 400 --outer 430 --tol 8e-14 --dump "$scratch/early.npz"
python3 "$g/weak_candidate.py" "$scratch/early.npz" --subcells 32
python3 "$g/weak_candidate.py" "$scratch/early.npz" --subcells 64 --bits 192
python3 "$g/capture_drive.py" "$scratch/mixed.npz"
python3 "$g/mixed_lift.py" "$scratch/mixed.npz"
python3 "$g/lift_probe.py" "$scratch/mixed.npz" "$scratch/early.npz"
python3 "$g/packet_pilot.py" --n 128 --m 480 --iterations 24 --dump "$scratch/packet.npz"
python3 "$g/packet_pilot.py" --n 256 --m 960 --iterations 28 --dump "$scratch/packet-refine.npz"
python3 "$g/packet_gate.py" "$scratch/packet.npz" --subcells 64
python3 "$g/packet_gate.py" "$scratch/packet-refine.npz" --subcells 256 --bits 192
python3 "$g/weak_candidate.py" "$scratch/packet-refine.npz" --packet --subcells 64 --bits 192
~~~

Outputs: [early weak](weak_candidate.txt), [early refinement](weak_candidate_variant.txt), [lift](mixed_lift.txt), [cell probes](lift_probe.txt), [packet pilot](packet_pilot.txt), [packet refinement](packet_pilot_refine.txt), [coarse strong gate](packet_gate_coarse.txt), [refined strong gate](packet_gate_refine.txt), [packet weak gate](packet_weak_gate.txt), [negative controls and warnings](FAILURES.md).
