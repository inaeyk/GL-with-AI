# Adverse check of the time-to-null Laplace exchange

The proposed estimate works for the fixed-order GOAL-011 hierarchy with smooth compact canonical axial data, the separated regular EF spherical representative, and the completed projected sources. A convenient safeguard is to use the normalized massive axial pair \(Y,Z\) in the homogeneous-plus-source form already derived. Its potential and first-time-derivative coefficient are bounded on the whole real \(x\) line. Factors such as \(f^{-1}\) may then occur in known forcing or reconstruction, where the causal support controls them. An energy estimate with an uncontrolled \(f^{-1}\) coefficient multiplying an unknown field would not justify the same conclusion.

## Global boundary bounds

The compact data determine finite lower bounds \(u_0,v_0\) on the two reference null coordinates of their causal future. Take these bounds strictly below the launch support. The axial support is inherited at each order because the spherical and GL coefficients only multiply lower axial fields, and the canonical Gauss completion is local. For a finite \(t\), this support has \(v_0-t\le x\le t-u_0\). Choose \(t_0\) strictly above the maximum launch value of \(T\) on the compact support. For \(t\ge t_0\) the zero-extended axial fields are smooth. Although the original graph can still meet such \(t\) slices near the horizon, it meets them outside the launch support, where the axial field vanishes in a neighborhood of the graph by finite propagation.

On that support \(f^{-N}\le C_N e^{Nt/r_0}\) and \(r\le C(1+t)\). The separated GL and mean Einstein coefficients are bounded by \(e^{m\Omega t}\) times powers of \(r,f^{-1}\) after any fixed number of reference derivatives, using regularity in EF coordinates and their far decay. The stationary normalized mixed coefficients, the scalar potential, and their reference derivatives are bounded. For a field \(w\) with such wave principal part, the energy including \(\|w\|_2^2\) obeys \(E'\le CE+C\|S\|_2^2\). Commute finitely many derivatives and use the triangular hierarchy in order \(P,J,K\). Each forcing uses finitely many lower-order derivatives, so start with enough lower-jet Sobolev regularity. Sobolev embedding gives bounds \(C e^{C't}\) for the pointwise derivatives needed at \(x=X_0\) and on the finite left interval. The finite pre-\(t_0\) part of the axial support is compact and needs no global energy estimate. These bounds allow different exponential rates at different orders.

## Uniform far estimates

Use \(u=t-x,\ v=t+x\) in the region \(u\ge u_0,\ v\ge u+2X_0\). For the mixed system write
\[
w_{uv}=A w_u+B w_v+D w+S,\qquad A,B=O(x^{-1}),\quad D=O(1).
\]
Choose \(0<\alpha<q_\infty-\Omega\) with some slack to absorb radial powers. In the supremum norm weighted by \(e^{-\gamma(u-u_0)-\alpha(x-X_0)}\), let \(U,V,W\) be the norms of \(w_u,w_v,w\), respectively, and \(a,b,d\) bound the coefficients. Let \(\mathfrak F=aU+bV+dW+\|S\|\). Integrate \(w_v\) in \(u\) from the zero line \(u=u_0\) at fixed \(v\), and \(w,w_u\) in \(v\) from the boundary \(v=u+2X_0\). Each path lies inside the far region. The weight gives
\[
U\le U_b+\mathcal L\mathfrak F,\qquad
V\le\mathcal K\mathfrak F,\qquad
W\le W_b+\mathcal L V,\qquad
\mathcal L=2/\alpha,\quad \mathcal K=1/(\gamma-\alpha/2).
\]
Here \(U_b,W_b\) are finite by the preceding boundary bound. Thus the coefficient of \(\mathfrak F\) on the right of its own bound is \(a\mathcal L+\mathcal K(b+d\mathcal L)\). Choose \(X_0\) large first and \(\gamma\) large second to make it less than one. Apply this inequality first in bounded characteristic regions so the supremum norms are finite, then take the limit. Commuting reference derivatives preserves the same leading equation. Derivatives of coefficients multiply lower jets, and derivatives of the timelike boundary data are provided by the pointwise time-energy estimate. The mixed estimate closes for any fixed finite number of derivatives with a common sufficiently large \(\gamma\).

For the baseline and return use the \(u\)-weight alone. Recover \(w\) from \(w_u\) by integrating in \(u\) from \(u_0\) at fixed \(v\), giving \(W\le U/\gamma\). The \(v\) integral for \(w_u\) then has potential contribution at most \(U(4\gamma)^{-1}\sup_u\int |V_{\rm RW}|\,dv\), which is absorbed for large \(\gamma\). The baseline is homogeneous. The first GL insertion applied to this baseline supplies the bounded mixed forcing in its weighted norm. On return, the \(HJ\) term and its needed derivatives have a bound \(C e^{\gamma'(u-u_0)-\beta(x-X_0)}\) for some \(\beta>0\) because \(\alpha<q_\infty-\Omega\). The \(H^2P\) and mean \(B_E P\) terms decay faster. This source is integrable in \(v\) in the unweighted-\(x\) estimate. Commuted scalar equations add integrable derivatives of \(V_{\rm RW}\) times lower jets. The \(v\) integral of \(w_{uv}=-(V_{\rm RW}w+S)/4\) gives convergence of \(w_u\) as \(v\to\infty\) on bounded \(u\) intervals. Integration in \(u\) from \(u_0\) gives the trace of \(w\). Both traces have a uniform bound \(C e^{\Gamma(u-u_0)}\) for some \(\Gamma\), with the same bound for \(w(u,v)\) and \(w_u(u,v)\) before the limit.

## Exchange and connection to continuation

For \(\Re s>\Gamma\), increasing \(\Gamma\) if needed, dominated convergence proves
\[
\int e^{-su}\Psi_{\mathscr I}(u)\,du
=\lim_{x\to\infty}e^{sx}\int e^{-st}\Theta\Psi(t,x)\,dt.
\]
For finite-radius time derivatives use \(\partial_t=\partial_u+\partial_v\). At fixed large \(v\), the integral of \(w_{uv}\) in \(u\) from the zero line gives a uniform exponential-in-\(u\) bound for \(w_v\) and \(w_v\to0\) on bounded cuts, since both \(Vw\) and \(S\) decay there. Thus the news exchange follows as well. The right transformed strong source is \(O(e^{-(\Re s+\beta)x})\). On the left the finite-time support gives \(t\ge v_0-x\), so the pointwise \(e^{C't}\) bound gives \(O(e^{(\Re s-C')x})\) for its transformed absolute magnitude when \(\Re s>C'\). This suffices against the decaying left Jost solution. Compact graph terms contribute finite entire transforms. In the same sufficiently right half-plane the mixed transformed unknown decays on the right at least as \(e^{-(\Re s_1-\alpha)x}\) and on the left by the time-support estimate. It therefore selects the physical retarded stationary resolvents before the earlier nested continuation. The strong source and the split canonical pairing are connected by their exact boundary identity. This closes the exchange for the formal ordered all-time trace and does not impose a finite-amplitude approximation at arbitrarily late time, or justify contour dominance on a finite observation window.
