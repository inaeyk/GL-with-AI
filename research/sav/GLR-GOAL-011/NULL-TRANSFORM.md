# Time-to-null Laplace exchange for the ordered solution

This argument uses the prescribed finite launch and the continued radial hierarchy of [PAIRING](PAIRING.md). It applies to the ordered coefficients. An exponential bound in retarded time supplies a physical right half-plane for the outgoing transform.

## Global time bounds on the inner portion

The launch and its induced upper-order axial data have compact spatial support on the graph. All axial fields and their sources vanish outside the causal future of that support. Choose constants $u_0,v_0$ strictly below its earliest $u,v$ and a constant $t_0$ strictly above its latest launch $t$. At $t=t_0$ the zero-extended fields are smooth and supported in a compact $x$ interval. Where that slice crosses the initial graph outside the launch support, the axial jets vanish on an open graph segment, and finite propagation supplies a zero neighborhood. The same reasoning makes the continued zero extension smooth for $t\ge t_0$.

Use the scalar RW system at orders zero and two and the normalized massive two-field system in [C's derivation](investigators/C-RESONANCE.md) at the intermediate order. Their background coefficients and derivatives are bounded on the entire tortoise line. On the causal support $v_0-t\le x\le t-u_0$, $f^{-m}\lesssim e^{mt/r_0}$ for fixed $m$ and the radius is at most linear in $t$. The explicit GL coefficients, their quadratic EF backreaction, and the finite number of reconstruction factors and derivatives in each triangular source have at most exponential growth on this support. Energy estimates for the bounded-coefficient systems, first for the baseline and then with their known lower-order sources, bound every required finite-order Sobolev norm by $C_m e^{c_m t}$. In the inhomogeneous steps this follows by Gronwall from $E^{1/2}$ and the source norm. Reserve enough baseline derivatives to cover the loss from the source formulas. Sobolev embedding bounds the pointwise fields and the boundary jets at any fixed outer line $x=X_0$ by exponentials in $u=t-X_0$. The constants for this inner estimate need no uniformity in packet width.

## Right-end weights

Increase $X_0$ so $T(x)-x<u_0$ throughout the far region. In $v\ge u+2X_0$, take the mixed equations in the form
$$
 w_{uv}=A w_u+B w_v+D w+S,
$$
where $A,B=O(x^{-1})$, $D$ is bounded, and the same estimates hold for the required coefficient derivatives. For the normalized massive variables the time-derivative coupling has faster $O(x^{-2})$ decay. Use the weighted supremum norm with multiplier $e^{-\gamma(u-u_0)-\alpha(x-X_0)}$, $\alpha>0$. Integrating at fixed $v$ from $u_0$ has weighted norm at most $K=(\gamma-\alpha/2)^{-1}$, and integrating at fixed $u$ from $v=u+2X_0$ has norm at most $L=2/\alpha$. At fixed $v$ the first integration stays in the exterior, and the value of $w_v$ at $u_0$ is zero. With $U,V,W$ the weighted norms of $w_u,w_v,w$, and $G$ the weighted norm of the right-hand side, the characteristic identities give
$$
 U\le U_b+L G,\qquad V\le K G,\qquad W\le W_b+L V.
$$
If $a,b,d$ are bounds on the three coefficient matrices then $G\le aU+bV+dW+\|S\|$. Choose $X_0$ so $aL<1/2$, and then $\gamma$ so $K(b+dL)<1/2-aL$. The estimates absorb $G$ on finite rectangles uniformly in their terminal $v$. This proves the mixed exponential bound for any chosen $\alpha>0$ for which the forcing is in the weighted norm. A fixed finite number of commutations uses the same estimate, treating differentiated coefficients multiplying lower jets as known forcing. Boundary jets come from the inner energy estimate.

For a massless stage use the weight $e^{-\gamma(u-u_0)}$ alone. Integrating $w$ in $u$ from zero gives $W\le U/\gamma$. In the $v$ integral for $U$, the RW potential has a uniform integrable envelope at fixed $u$. With any source of the form $|S|\le C e^{\gamma(u-u_0)-b x}$, $b>0$, its $v$ integral is also bounded. Taking $\gamma$ large absorbs the potential. This bounds the scalar and its differentiated trace. Apply this first with zero forcing to the baseline. Along the right end the GL jet and its derivatives are bounded by $e^{\Omega u-(q_\infty-\Omega-o(1))x}$ up to polynomial factors. The mixed forcing is therefore in the right-end weight just used. Take $0<\alpha<q_\infty-\Omega$ with strict margin. The returning mixed product restores a positive radial decay rate. The direct quadratic and mass-matched EF terms decay faster. Hence the exact returning source and its requisite derivatives have the stated massless bound with some $b>0$ and finite $\gamma$. The compact-mode extraction corrections also decay on every fixed cut.

## Outgoing transform and pole meaning

The returning equation now gives $\Psi_2(u+x,x)\to\mathcal W_{\rm GL}(u)$ and $|\Psi_2(u+x,x)|\le C e^{\gamma(u-u_0)}$ for $x\ge X_0$. The differentiated trace follows by integrating the integrable characteristic equation for $w_u$ and then integrating in $u$ from $u_0$. The returning $w_v$ also tends to zero on each cut, since its integral in $u$ uses the decaying potential and source. Thus the finite-radius time derivative $w_t=w_u+w_v$ has the same limiting news and an exponential bound in $u$. Dominated convergence proves, for sufficiently large $\operatorname{Re}s$,
$$
 \int e^{-su}\mathcal W_{\rm GL}(u)\,du
     =\lim_{x\to\infty} e^{sx}\widehat{\Theta\Psi_2}(s,x).
$$
The right source transform has the bound $|\widehat{\Theta S}(s,x)|\le C_s e^{-(\operatorname{Re}s+b)x}$. At the left, the time bound and $t\ge v_0-x$ yield a bound of the form $C_s e^{(\operatorname{Re}s-c)x}$ for a fixed $c$, which is sufficient after multiplication by $h\sim e^{sx}$ for large $\operatorname{Re}s$. The graph terms are compact. These bounds justify the outgoing resolvent pairing in [GREEN-FUNCTION](GREEN-FUNCTION.md) without an auxiliary cut in $u$. The continuation of this right-half-plane identity gives the simple-pole coefficient on the specified outgoing sheet using the local spectral certificate and the continued physical source. The proof does not isolate that pole from prompt and branch-cut contributions on a time window. It does not bound a finite-amplitude waveform after prolonged GL growth.


### Pole term and remainder on the continued sheet

Write the outgoing transform as $\mathcal T(s)$ and choose a reference
cut $u_b$. For the fundamental and its reality partner, subtract
$A_q e^{-(s-s_q)u_b}/(s-s_q)$ and its conjugate-parameter counterpart.
Their inverse Laplace transforms are the corresponding exponentials
supported on $u\ge u_b$. The remaining transform is analytic at
these two poles and represents the exact remainder for that choice
of cut. Its inverse includes the prompt waveform, other singularities
and the non-pole part. The [signed certificate](SIGNED-RESIDUE.md)
evaluates $A_q$ for the ordered physical packet. The subtraction
does not give a time interval on which the remainder is small.
