# Adversarial audit of the full-packet sign

I audited [packet_signed.py](../observables/packet_signed.py) and its archived output after receiving the proposed interval. I reconstructed the signed correction equations, bound orientation and endpoint cancellations from the frozen mixed and returning equations. This check shares the GL and fixed-infinity mean geometry, the pinned axial source and the rotated coefficient assembly with the early certificate. It is a non-blind mathematical and implementation review. The coordinator owns the interval-heavy replay.

## Signed transport and graph coverage

Write \(W=V/4\) and \(J(u,v)=\int_{b(u)}^v W(u,s)\,ds\). The physical lower scalar satisfies \(w_{0,u}=B'-\int_b^v Ww_0\). Differentiation gives
\[
 w_{0,uu}-(B''-JB')=b'W|_\Sigma B-\int_b^v[W_u w_0+W(w_{0,u}-B')]\,ds .
\]
Thus if \(q\) is the rotated first source coefficient, its signed \(B'\) coefficient is \(S_1=S_u-qJ e_1\), where \(S_u\) denotes the source row on \(w_{0,u}\), not a derivative of the source. The code keeps the graph term in the six scalar error bounds. Its bound on \(w_{0,vv}\) includes the \(\alpha'(v)W|_\Sigma B\) term where a fixed-\(v\) ray starts on the graph. The orientation is \(\sup_v\|\cdot\|_{L^2_u}\) for volume and a uniform pointwise bound for the terminal ray.

Put \(A=\int_b^v q\,ds\). I checked the splitting
\[
 P=-A B''e_1/4+C B'+e_P,\qquad
 X=-A B'e_1/4+e_X,\qquad Q=-q B'e_1/4+e_Q.
\]
The signed coefficient \(C\) starts from the \(B'\) coefficient of the physical rotated graph \(P\). Its derivative is \(-M_+(-A e_1/4)-S_1/4\). Hence the remaining incoming \(e_P\) contains the \(B\) coefficient, and the error satisfies \((e_P)_v=-M_+e_X-S_E/4\). In the \(e_X\) equation the term proportional to \(B'\) is \(C+(A_u e_1-2A L e_1)/4\); in the \(e_Q\) equation it is \((q_u e_1-2q L e_1-S_1)/4-M_-(-A e_1/4)\). The code encloses \(M_-(-A e_1/4)\) separately and retains the full graph \(Q-Q_0\). Its optical \(X_0\) vanishes on the graph because \(A=0\) there. The identity \(A_u=\Omega A-q+(1-b')q|_\Sigma\) follows from \(q_u+q_v=\Omega q\) and the moving lower limit. The direct \(q_u\) coefficient uses \(r_u=-f/2\) at fixed \(v\).

The cell bands use \(s=r(d,v)\) and the exact upper bound \(x(r)\le x(s_1)+D/2\), \(D=3/25\). They start at \(s=1.596<b(d)\). Within every cell an increment of a signed primitive belongs to the interval hull of its whole-cell range times the cell length and zero. Completed cells after \(v>b(a)\) use the full increment. Earlier cells that may straddle the graph retain the optional increment. Both signed prefixes \(A,C\), as well as the final signed current primitive, use this logic. On the physical \(u\) segment, the skew evolution for \(e_X\) gives \(\|e_X\|\le\kappa(\|e_P\|+\|D_X B'\|)\), with \(\kappa=2D/\pi\) or \(D\) for the two norms. The recurrence for \(e_P\) then follows by Grönwall on a single \(v\) cell using the old accumulated bound and the current whole-cell forcing. This supplies both the volume bound and the terminal error even though \(B''\) changes sign. The early positive-jet ratio argument is unnecessary here.

## Signed observable and error budget

The physical signed \(F\) primitive on each ray has the exact split
\[
 \int_b^c F\,dv=\phi(u)B''(u)+C_F(u)B'(u)+e_F(u),\qquad
 \phi=A(u,c)^2/4 .
\]
The coefficient \(C_F\) integrates the signed \(F_P C\), \(F_Q(-q e_1/4)\), \(F_X(-A e_1/4)\) and baseline \(F_{B_u}\) rows. The reference \(F_P(-A e_1/4)B''=qA B''/2\) gives \(\phi\) after integration. Both reference terms vanish at the packet terminal cut. The scattered physical \(g(d)\) need not vanish, so the terminal pairing remains and is charged against the uniform \(e_F\) error. The inherited endpoint bracket implies \(|g(d)|<0.009755\).

In the bulk, \(g'=B''-J_\infty B'+\delta_g\). The \(\phi J_\infty B'B''\) term and the \(C_F B'B''\) term each allow centering of the coefficient interval, because \(\int_a^d B'B''=0\) exactly and \(\|B'\|_2=1\). The implementation uses the radius of the entire \(\phi J_\infty\) interval, not a product of separately centered quantities. It charges \(\delta_g\) and \(g'-B''\) separately in the two pairings. Thus the sign conclusion does not require \(C_F\), \(J_\infty\), or the phase to be constant.

For the dual error the needed \(M\) bounds \(\int\sup_u W\,dv\). On \(r\ge1.596\), both inequalities \(\pm\partial_u\log W\le1/4\) follow by expanding the rational numerators at \(r=1.596\). Their coefficients are positive. Since \(W=3(r-1)(2r-1)/(4r^4)\) decreases for \(r\ge2\), only the \(s\in[1.596,2]\) part of the supremum integral incurs the \(e^{D/4}\) multiplier. This proves the rectangular mass bound used by the code, including the exterior. With zero extension of \(W\) below the graph and backward dual integration, \(\sup_v\|Z\|_2\le e^{MD}\|g\|_2\), \(\sup_v\|Z_u-g'\|_2\le M\sup_v\|Z\|_2\), and \(\|Z_v(\cdot,v)\|_2\le\kappa\sup_u W\,\sup_v\|Z\|_2\) on the physical strip. The residual dual bound therefore has exactly the \(F,H,G\) weights in the implementation. The compact \(G\) cap contact cancels with the physical weak exterior using the same transported test, and the remaining physical exterior uncertainty is added once.

For the upper sign, rounding the archived constituents outward in the units of \(4\mathcal P\) gives leading magnitude above \(4393.82672\), with errors below \(44.02053,\ 2078.31966,\ 0.44415,\ 616.13375,\ 854.54175\), plus the separate exterior bound. Exact subtraction of these outward decimals gives \(\mathcal P<-200.09171\), consistent with the archived full-precision upper \(-200.091728878\ldots\). The reported outward interval \([-2758.258,-200.091]\) follows for the fixed ordered packet and pinned inputs. The prior absolute-envelope attempt spans zero. Keeping that result and charging the missing signed primitive terms explicitly distinguishes the successful estimate from the unsupported sign in the first attempt.

The early and packet projections have opposite strict signs. Continuity of the cumulative ordered overlap gives at least one zero between the two terminal cuts. This argument gives neither a location nor a uniqueness statement and has no finite-amplitude hypothesis beyond the ordered setup.
