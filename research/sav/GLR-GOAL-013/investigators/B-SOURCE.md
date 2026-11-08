# B. Causal source and a quantitative exterior-tail bound

Investigator B reconstructed the density equations, source and graph contacts in [SOURCE-LEDGER](../SOURCE-LEDGER.md) from the prescribed ordered action and canonical data. This is a separate time-domain calculation with exposure to the pinned equations and to the GOAL-012 causal construction. It inherits the GL spectral certificate. The fixed benchmark is the real \(R=13/8,\ \delta=3/25\) packet, the GOAL-005 mode normalized at \(r=2\), \(\Omega\in[0.06328118,0.06328120]\) and \(k=\pi/5\). All quantities here use \(r_0=1\). The argument covers \(u_*\le u\le U=8.407362\).

## Quantitative claim

For \(V_c=U+2x(r_c)\), \(r_c\ge100\), the complete returning source in the exact-curl convention obeys
\[
 B_c=\tfrac14\|S_2\|_{L^1_vL^2_u(v\ge V_c)}
 \le 10^{63}e^{-(2/5)(r_c-4)}+10^{55}(r_c+6)e^{-(4/5)(r_c-4)}. \tag{1}
\]
The first term bounds the part of the feedback linear in the mixed state. The second bounds local \(H^2\) terms including affine mixed reconstruction, and the fixed-mass mean. Directed evaluation gives \(B_c<1.7\times10^{-6}\) at \(r_c=400\) and \(B_c<7\times10^{-24}\) at \(r_c=500\). The bound is too large for useful error control at \(r_c=100\). The larger radius changes where the numerical truncation would be made. A homogeneous cap transfer there still requires a computed and certified candidate on that cap. Equation (1) does not estimate the forcing between the smaller and larger cap to the same tolerance.

## 1. Coefficients on real causal cuts

Inherit the absolute GL \(C^2\) bounds from [GOAL-005 proof §2](../../physics/goal005_enclosures.md#2-a-coarse-global-regular-c5-upper-envelope) and [directed output](../../verification/goal005_mode_bounds.json) at [87fd711bf2e8117467caad1ff52c1cd344966974](https://github.com/inaeyk/GL-with-AI/commit/87fd711bf2e8117467caad1ff52c1cd344966974). These include parameter and radial-domain remainders for raw covariant entries, including \(H_{\theta\theta}\). The maxima over the four components are below these rational upper endpoints.

| Region and chart | \(j=0\) | \(j=1\) | \(j=2\) |
|---|---:|---:|---:|
| \(1\le r\le4\), EF \(\tau=v-r\) profile | \(1.8\times10^7\) | \(2.6\times10^{11}\) | \(6\times10^{15}\) |
| \(4\le r\le40\), static profile times \(e^{(r-4)/2}\) | \(36\) | \(77\) | \(228\) |
| \(r\ge40\), same static weight | \(0.35\) | \(0.24\) | \(0.17\) |

Let \(h\) be the maximum absolute unhalved GL entries \(H_{tt},H_{tx},H_{xx},H_{\theta\theta}\) with time factor included and their \(t,x\) partial derivatives through total order two. Use \(D_u=U-u_*<10,\ v\ge1,\ f>1/200\). The last inequality follows from \(u_\Sigma(200/199)>U\) and the monotonicity of \(u_\Sigma\).

For \(r\le4\) the inherited profiles are entries in the \((v,r)\) tensor basis evaluated at \(\tau=0\), so transform the entries with \(dv=dt+dx,\ dr=f\,dx\). Multiply the profiles by \(e^{\Omega\tau}\) with \(\tau=u+2x-r<16\). The largest sum of absolute coefficients in any transformed entry is at most \(4\), from \(H_{xx}=H_{vv}+2fH_{vr}+f^2 H_{rr}\). The first and second \(x\)-derivative sums of these tensor coefficients are bounded by \(4\) and \(14\), using \(|f_x|\le1,\ |f_{xx}|\le3\). The scalar profile derivative operator is \(f\partial_r+\Omega(1-f)\) because \(d\tau=dt+(1-f)dx\). Its first and second applications cost at most \(1.064\) and \(2.2\) times the maximum relevant jet. Time derivatives multiply by \(\Omega\). Since \(3(4\cdot2.2+8\cdot1.064+14)(6\times10^{15})<10^{18}\), the inner bound follows. For \(r\ge4\), \(x\le3r/2,\ |f_x|\le1/16,\ |f_{xx}|\le3/64\). The static-to-tortoise transformation multiplies orbit entries by \(1,f,f^2\). Differentiating these multipliers and applying \(\partial_x=f\partial_r\) costs a factor less than \(4\) through order two. Since \(u<9,\ \Omega<0.064,\ \log(r-1)\le r/2\),
\[
 e^{\Omega(u+x)}e^{-(r-4)/2}\le3e^{-(2/5)(r-4)} .
\]
Hence \(h\le10^4 e^{-(2/5)(r-4)}\) at \(r\ge4\) and \(h\le10 e^{-(2/5)(r-4)}\) at \(r\ge40\). Globally use
\[
 h(u,v)\le10^{18}s(v),\qquad s(v)=\exp[-(v-20)_+/10],\qquad 1\le v<\infty . \tag{2}
\]
Indeed \(U+2x(4)<20\) and \(dr/dv=f/2\ge3/8\) when \(r\ge4\). Thus \(\|s\|_1=29,\ \|s\|_2=\sqrt{24}<5,\ \|s\|_\infty=1\).

For the fixed-mass mean at \(r\ge100\), convert the static profiles to \((v,r)\). The entries \(h_{vv}=h_{tt}\), \(h_{vr}=h_{tr}-h_{tt}/f\), \(h_{rr}=h_{rr}^{\rm stat}-2h_{tr}/f+h_{tt}/f^2\) and the raw sphere entry with their first two fixed-\(v\) radial derivatives are bounded by
\[
 m_H=20e^{\Omega(v-x(r))}e^{-(r-4)/2}.
\]
Here \(1/f\le100/99\), \(|(1/f)'|\le1/99^2\), \(|(1/f)''|\le2/99^3\) and the time factor has radial log derivative \(-\Omega/f\). In the actual quadratic EF Ricci expressions of [GOAL-011 mean source](../../GLR-GOAL-011/investigators/B-MEAN-QUADRATURE.md) and [coefficient implementation](../../GLR-GOAL-011/checks/physical_overlap/overlap_mean.py), use entry caps \(|b_0^{-1}|\le1,\ |h^\sharp|\le2m_H,\ |h^\uparrow|\le4m_H,\ |h^\sharp h^\uparrow|\le16m_H^2,\ |\operatorname{tr}_4h|\le6m_H\). The EF connection variations are bounded by \(6m_H,24m_H^2\), the three Hessian entries of \(R^2\) by \(2r,14rm_H,60rm_H^2\), and the first base scalar-curvature variation by \(13m_H\), using \(|f'|\le1,|f''|\le2,k,\Omega<1\). Substitution in the \(rr,\theta\theta,zz\) expressions gives \(|Q_{rr}|<50m_H^2,\ |Q_{\theta\theta}|/r^2<150m_H^2,\ |Q_{zz}|<3m_H^2\). In particular \(|\mathcal T^v{}_v|,|\mathcal T_{rr}|\le10^4m_H^2\).

Put \(A(r,v)=e^{2\Omega(v-x(r))}e^{-(r-4)}\) here only. At fixed \(v\), \(x(\rho)\ge x(r)\) for \(\rho\ge r\). The exact mean integrals therefore give
\[
 |\mu|\le2\times10^6 A(r,v)(r^2+2r+2),\qquad
 |\beta|\le2\times10^6 A(r,v)(r+1).
\]
Their fixed-\(v\) derivatives have caps \(2\times10^6 A r^2,\,2\times10^6 A r\) by the original integrands. Their \(v\) derivatives equal \(2\Omega\) times the fields. The \((t,x)\) mean entries are \(2\mu/r-2f\beta,\ 2\mu/r-f\beta,\ 2\mu/r\). Using \(\partial_x|_t=\partial_v+f\partial_r|_v\) and \(A\le7 e^{-(4/5)(r-4)}\), the maximum \(b_*\) of their absolute values and first \(t,x\) derivatives satisfies
\[
 b_*(u,v)\le10^9(r+1)e^{-(4/5)(r-4)}\qquad (r\ge100). \tag{3}
\]
This includes the full semi-infinite mean integrals and the fixed endpoint charges.

## 2. Operator coefficient bounds

For scalar or two-component \(w\) define
\[
 Z_0(w)=|w_t|+|w_x|+|w|/r,\quad Z_m(w)=|w_t|+|w_x|+|w|,\quad
 W_0=\sum_{j=0}^2 Z_0(\partial_t^j w_0),\quad
 W_m=\sum_{j=0}^1 Z_m(\partial_t^j(y,z_c)).
\]
Vector norms are Euclidean. The following deliberately rounded constants provide a coarse exterior estimate.

Put \(X_b=\bar b^{-1}H\) with the unhalved \(H\) and \(\bar b=f\,\operatorname{diag}(-1,1)\). Write \(Y_b=\bar b^{-1}\Delta b_E\), \(d=H_{\theta\theta}/(2r^2)\) and \(e=-d^2/2\) for the direct quadratic term. Ignoring Fourier factors of magnitude at most one for entry bounds, the first mass matrix \(M_1/r^2\) is \([(2d+\tfrac12\operatorname{tr}X_b)\mathrm{Id}-X_b]\operatorname{diag}(-1,1)\). The curl coefficient \(P^{tx}\) restricted to \(F_{tx}\), relative to its background factor \(-r^4/f\), is \(R^4/r^4\) divided by the ratio of noncompact volume densities. Its first coefficient is \(4d-\tfrac12\operatorname{tr}X_b\). The quadratic coefficients before Fourier projection are
\[
\begin{split}
 M_2/r^2:\quad&
 \left[X_b^2-Y_b-(2d+\tfrac12\operatorname{tr}X_b)X_b+
 (2e+d^2+d\operatorname{tr}X_b+\tfrac12\operatorname{tr}Y_b
 -\tfrac14\operatorname{tr}X_b^2+\tfrac18(\operatorname{tr}X_b)^2)\mathrm{Id}\right]
 \operatorname{diag}(-1,1),\\
 P_2/P_0:\quad&
 4e+6d^2-2d\operatorname{tr}X_b+\tfrac14\operatorname{tr}X_b^2
 +\tfrac18(\operatorname{tr}X_b)^2-\tfrac12\operatorname{tr}Y_b .
\end{split}
\]
Terms linear in \(Y_b\) enter unhalved. The direct quadratic terms carry the factor \(1/2\) from averaging the squared cosine. The mixed mean \(H A_1\) uses \((H A_{1,+}+H\overline{A_{1,+}})/2\), bounded by the unhalved \(H\) cap times the actual \(+k\) coefficient's cap.

Use \(1/f<200\), \(|\partial_x(1/f)|\le1/f,\ |\partial_x^2(1/f)|\le3/f\), \(|\partial_x^j(r^{-1})|\le3/r,\ |\partial_x^j(r^{-2})|\le8/r^2\) for \(0\le j\le2\), and \(|V_{\rm RW}|\le6/r^2,\ |\partial_x V_{\rm RW}|\le50/r^2\). The first mass matrix entries and their derivatives through order two are bounded by \(10^4h\), using \(6h/f\) for a differentiated \(X_b\) entry and \(13h\) for \(2d\). The two second-order coefficients and first derivatives have bound \(10^{10}(h^2+b_*)\) on the exterior. The same algebra holds down to \(r>200/199\) if \(b_*\) is supplied there.

| Quantity | Bound | Coefficient checks |
|---|---:|---|
| \(r|\partial^\alpha A_0|\), \(|\alpha|\le2\) | \(10^3 W_0\) | Substitute \(w_{0,xx}=w_{0,tt}+V_{\rm RW}w_0\), \(w_{0,xxx}=w_{0,ttx}+V_{\rm RW}w_{0,x}+V_{{\rm RW},x}w_0\), \(w_{0,txx}=w_{0,ttt}+V_{\rm RW}w_{0,t}\) |
| \(r|\partial^\alpha(J^a/r^2)|\), \(|\alpha|\le2\) | \(10^9h W_0\) | \(\lambda=4\), two entries and at most four Leibniz terms using the first mass matrix cap |
| \(|X|+|X_t|\) | \(10^4 r^2 h W_0\) | \(Q_{0z}=\lambda r w_0/2\). The first curl multiplier and its time derivative are each bounded by \(202h\) |
| \(|\partial_t^j S_1|\), \(j=0,1\) | \(10^{11}h W_0\) | \(\sqrt D/d_k=r/\sqrt{d_k}<2\), \(|d_{k,x}/d_k|\le2/r\), \(\sqrt D<3r^2\), \((L_c r^2)/\sqrt W=r^2/(2\sqrt{d_k})<r\), \(|\partial_x\log(L_c r^2)|\le4/r\) |

Here \(S_1\) is the Euclidean two-row source of the normalized mixed equations. The final row follows by inserting \(J^a/r^2\) and \(X\) in those two equations. For example the \(y\) row costs at most \(2\cdot(2+2/r)10^9h W_0/r+3\cdot10^4hW_0\), and the \(z_c\) row at most \((2+4/r)10^9 h W_0\), each also valid for its first time derivative.

On the exterior use \(K\le5\), \(|a_m|,|b_m|\le2/r\), \(|a_{m,x}|,|b_{m,x}|\le10/r^2\), \(|C|\le1\), so \(|V_y|,|V_z|\le10\) for \(r\ge100\). The spatial coefficient bounds follow from \(a_m=f(1/r+k^2r/d_k)\), \(b_m=f(2/r-k^2r/d_k)\). In \(A_1\) the derivative multipliers are \(1/\sqrt D\) and \(k/(2\sqrt{d_k})\) since \(L_c/\sqrt W=1/(2\sqrt{d_k})\). Their first derivatives have relative bound \(2/r\), and their products with \(r\) are bounded by \(2\). For the first derivatives of \(A_1\) substitute \((y,z_c)_{xx}=(y,z_c)_{tt}+\operatorname{diag}(V_y,V_z)(y,z_c)-C\sigma_1(y,z_c)_t+S_1\). The undifferentiated entries and first derivatives then satisfy
\[
 r(|A_1|_{\max}+|\partial A_1|_{\max})
 \le10^4 W_m+10^{14}hW_0 .
\]
The affine term \(J^a/D=(J^a/r^2)/d_k\) uses the current row and \(|d_{k,x}/d_k|\le2/r\). The derivative substitution adds another local \(S_1\) term, already included in the \(hW_0\) part.

The two differentiated mixed currents cost at most \(2\times10^9hW_m+2\times10^{19}h^2W_0\) in \(|S_2|\). Each derivative has two Leibniz terms, two potential entries, the factor \(\lambda\) in the current and \(2r/\lambda\) in \(S_2\). The mixed curl uses \(r^4F_{1,tx}/f=U_m+X\), \(\sqrt D\le3r^2\), the \(X\) bound and the factor \(2f/r^3\) in \(S_2\), costing at most \(10^5hW_m+10^9h^2W_0\). The second-order currents cost at most \(2\times10^{14}(h^2+b_*)W_0\) and the second-order curl at most \(10^{11}(h^2+b_*)W_0\). In particular
\[
 |S_2|\le10^{11}hW_m+10^{21}(h^2+b_*)W_0 \qquad (r\ge100). \tag{4}
\]

## 3. Absolute energies of the causal lower fields

For a scalar or mixed solution use the positive characteristic energy of [GOAL-012 B](../../GLR-GOAL-012/investigators/B-PHYSICAL-SOURCE.md#positive-characteristic-energy-and-residual-certification). The mixed spatial matrix \(\mathsf A=\operatorname{diag}(a_m,-b_m)\) is Hermitian. Its anti-Hermitian time coefficient has zero real work on \(w_t\). The graph energy is
\[
 E_\Sigma=\int [ (|w_t|^2+|D_xw|^2+K|w|^2)/2+p\Re(w_t^\dagger D_x w)]\,dx.
\]
The fluxes control \(\xi_u=w_u-\mathsf Aw/2,\ \xi_v=w_v+\mathsf Aw/2\) and \(\sqrt K w/2\) in their respective directional \(L^2\) norms. Here \(K\) is \(V_{\rm RW}\) for the scalar and \(f(k^2+\lambda/r^2)\) for the mixed system. The current divergence \(-\Re(w_t^\dagger S)\) and Cauchy–Schwarz on its two derivative terms give
\[
 M\le\sqrt{E_\Sigma}+\tfrac12(\|S\|_{L^1_vL^2_u}+\|S\|_{L^1_uL^2_v}),
\]
with \(M\) the largest flux square root on subdiamonds. The commuted fields vanish on the causal front by flat packet launch and finite propagation. Apply the estimate first on compact diamonds with zero incoming front data, and then uniformly as the cap increases.

Here \(\sqrt{V_{\rm RW}}>.1/r,\ \sqrt{K_{\rm mixed}}>.04,\ |\mathsf A|\le2/r\). The equalities \(w_t=\xi_u+\xi_v,\ w_x=\xi_v-\xi_u-\mathsf A w\) bound either \(Z\) by \(2(|\xi_u|+|\xi_v|)+150\sqrt K|w|/2\). For any nonnegative envelope \(c(v)\), extend \(c Z\) by zero outside the physical region. Then
\[
\begin{split}
 \|c Z\|_{L^1_vL^2_u}&\le M[152\|c\|_1+2\sqrt{D_u}\|c\|_2],\\
 \|c Z\|_{L^1_uL^2_v}&\le M[2\sqrt{D_u}\|c\|_2+152D_u\|c\|_\infty].
\end{split} \tag{5}
\]
The \(\|c\|_2\) factor in the first line comes from Cauchy–Schwarz in \(v\) followed by the fixed-\(u\) flux. In the second it comes from the fixed-\(v\) flux followed by Cauchy–Schwarz in \(u\). In particular this does not interchange the two directional derivative flux bounds.

For the graph bounds let \(\phi=w|_\Sigma,\ \psi=w_t|_\Sigma\). For \(-w_{tt}+w_{xx}-\mathcal Vw+\mathcal B w_t=S\), graph differentiation gives
\[
 (1-p^2)w_{tt}|_\Sigma=\phi''-2p\psi'-p'\psi-\mathcal V\phi+\mathcal B\psi-S|_\Sigma .
\]
Primes on graph traces are total \(x\) derivatives. For the unnormalized bump \(g(y)=e^{-t_b}\), \(t_b=1/[y(1-y)]\), Cauchy–Schwarz on \([0,1/2]\) gives \(\|g'\|_2^2\ge2e^{-8}\), so \(c_b\le e^4/\sqrt2<40\). The inequalities \(|t_b'|\le t_b^2,\ |t_b''|\le3t_b^3,\ |t_b'''|\le9t_b^4\) give \(|g^{(j)}|\le4t_b^{2j}e^{-t_b}\) for \(j\le3\). The maximum of \(t_b^n e^{-t_b}\) on \(t_b\ge4\) is at \(\max(4,n)\). Including \(\delta^{1/2-j}\) gives \(|a_\delta^{(j)}|<10^7=:A_*\) for \(j\le3\).

The scalar graph has \(w_{0,t}=a'\), \(w_{0,tt}=a''-a V_{\rm RW}/(1-p^2)\), \(|V_{\rm RW}/(1-p^2)|\le6\) and its \(x\) derivative bounded by \(50\) from \((6/r^2-3/r^3)/(1+1/r)\). Thus \(|(w_{0,tt}|_\Sigma)'|\le70A_*\) since \(|u_\Sigma'|\le2\). Applying the graph rule to \(w_{0,t}\) gives \(|w_{0,ttt}|\le200(5+140+7+6)A_*<4\times10^4A_*\), and \(\partial_x w_{0,tt}\) adds at most \(70 A_*\) using the trace derivative. Thus graph values and first \(t,x\) derivatives of \(\partial_t^j w_0\), \(j\le2\), are bounded by \(10^{12}\) each. They vanish outside the packet and the \(x\)-support length is at most \(\delta\). The graph energy square roots are below \(10^{13}\). The commuted baseline equations are homogeneous, so \(M_0^{(j)}\le10^{13}\) for \(j=0,1,2\).

The mixed graph jets are also supported on the same packet, where \(3/2<r<2\), \(1-p^2>1/3\). The bound \(W_0|_\Sigma\le9\times10^{12}\) and the current row of the coefficient table give \(|J^a|<2\times10^{40}\), \(|(J^a|_\Sigma)'|<10^{41}\). In the latter use \(\partial_x+p\partial_t\) on \(J^a/r^2\) and differentiate \(r^2\) separately. The reciprocal of \(1-p^2\) is below \(3\) and its graph derivative below \(18\). The forced velocities from the ledger, rescaled by \(1/\sqrt D,\sqrt W\), then have component bounds \(10^{42}\) and their graph derivatives \(10^{43}\). For example \(|(pJ^t-J^x)'|\le2.2\times10^{41}\) and the two contributions from the reciprocal product derivative are each \(<8\times10^{41}\) before rescaling. For the compact velocity the additional \(k/(\lambda r^2)\) reduces the cap, and \(\sqrt W<4\) with log derivative bounded by \(2/r\). The graph rule with zero mixed displacement, \(|C|\le1\), and \(|S_1|_\Sigma<10^{42}\) bounds the mixed \(w_{tt}\) components by \(10^{44}\). The \(x\) derivative of \(w_t\) follows by adding its tangential derivative. This gives graph energy square roots \(<10^{45}\) for the two commutations, using \(x\)-support length \(\le\delta\). Equations (2), (5) and the coefficient table bound the work term for \(j=0,1\) by
\[
 10^{11}\,10^{18}\,(3\times10^{13})\,10^4=3\times10^{46}.
\]
Here \(10^4\) dominates the average of the two norm constants in (5) for \(s\). Thus \(M_m^{(j)}\le10^{48}\) for \(j=0,1\). These are absolute energies of the physical lower hierarchy. An error estimate for a numerical candidate would instead use its graph error and its volume residual.

## 4. Tail integration

Let \(\ell=v-V_c\ge0\). Since \(dr/dx=f\le1,\ r(u,v)\le r_c+5+\ell/2\). Also \(r(u,v)\ge r_c+(49/100)\ell\) by \(f(r_c)\ge0.99\) and \(u\le U\). The coefficient bounds (2)–(3) give
\[
 h\le10e^{-(2/5)(r_c-4)}e^{-\ell/6},\qquad
 h^2+b_*\le[100+10^9(r_c+6)]e^{-(4/5)(r_c-4)}e^{-\ell/3}.
\]
For the latter, \(r+1\le(r_c+6)e^{\ell/200}\) since \(r_c\ge100\). The first line of (5) applied to \(c=e^{-\ell/6}\) gives \(152\cdot6+2\sqrt{D_u}\sqrt3<930\) and for \(c=e^{-\ell/3}\) gives \(152\cdot3+2\sqrt{D_u}\sqrt{3/2}<470\). Combining (4) with these constants, the two mixed and three scalar flux bounds and the factor \(1/4\) proves (1). The bound covers the exterior forcing on the complete fixed-cut domain even though no mixed numerical candidate is propagated there.

The exponential decay with polynomial factors also makes the compact extraction products and their outgoing derivatives vanish in the fixed-\(v\) null limit. For example the compact curl correction after dividing by \(r\) has the factor \(h/r\) times the caps on \(U_m\) and \(X\), where \(|U_m|=\sqrt D\,|y|\le3r^2|y|\). An outgoing derivative uses the fixed-\(v\) flux bound on \(\xi_u\) and the potential term, so the factors \(h(r+1)\) suffice.

## Evidence and limits

- **Proved given the pinned mode bounds and ordered equations.** Complete returning source ledger, induced upper graph jets, weak/strong contact and interface signs, EF collar identity, and the explicit finite-window source tail (1).
- **Directed arithmetic.** The companion [script](../checks/source_tail_bound.py) uses Python 3.14.8 and python-flint 0.9.0 at 128 bits to verify the causal geometry inequalities and evaluate (1). The archived [output](../checks/source_tail_bound.txt) records the overly broad result at \(r_c=100\). This is a bounded arithmetic check with no physical evolution.
- **Adverse cases.** Dropping a graph contact or a split-face term fails the distributional identity in the ledger. Treating a fixed-\(v\) flux as control of the opposite directional derivative would fail in (5).
- **Unresolved here.** Interior physical candidate and residual enclosure, the resulting Bondi waveform, QNM comparisons, and the two signed physical interference projections. The inherited complex pole overlap errors cannot supply those causal residuals.
