# Nonmean compact principal symbol and paired correction

Linearize the terminal optical subsystem in a test \(w=R^{-s}g(z)\) of \(W=Rq\) about \(\bar q=\lambda R^{-n}a(z)\), integer \(n\ge5\), with zero terminal homogeneous data and \(\Re s>3\). Retain first variation in \(\lambda\) and the coefficient of \(g_{zz}\). A compact derivative hitting \(a\) reduces the compact order in the test. Some such terms still contain radial derivatives. The manufactured background need not be an evolved vacuum solution.

Set \(c_0=1-6/[s(s-1)]\), \(d_0=3(s+1)/[s(s-1)]\), \(v_0=-3(s+2)/[s(s-1)(s-2)]\). At zero background the shift response is \(d_0R^{-s}g_z\) and the compact potential response is \(v_0R^{1-s}g_{zz}\). The lapse, torsion and shift equations yield at mixed order
\[
 b_n=-\frac{3n(s+1)}{2(s+n+1)},\quad
 p_n=-\frac{3n(s-n+1)}{(s+n+1)(s+n-1)},\quad
 d_n=\frac{3n d_0-p_n-b_n}{s+n}.
\]
Their powers are \(R^{-s-n-1},R^{-s-n-1},R^{-s-n}\), multiplying \(\lambda ag\) or \(\lambda ag_z\). The scalar coefficient of \(R^{-s-n}ag_{zz}\) is
\[
 c_n=-p_n-2d_n+n d_0-3c_0+v_0n(s+n-2).
\]
The final term follows from \((V_1\bar W_R)_R-(V_1)_R\bar W/R\) with \(\bar W=\lambda R^{1-n}a\). It survives even for the constant-\(a\), \(n=1\) limit where the first summand vanishes.

Let \(F(D)=D(D-1)/[(D+2)(D+3)]\), \(D=R\partial_R\). Apply the inherited left multiplier \(F(D)\) and field \(Z=F(D-1)w\). The new coefficient is \(C_n=F(-s-n)c_n/F(-s-1)=-3+K_n\), reducing to the source's leading mean coefficient at \(n=1\). At \(n=5\) the rational nonlocal remainder and its terminal log-radius kernel are
\[
 K_5=-\frac{180(s^2+4s+20)}{s(s+1)(s+3)(s+4)},\quad
 k_5(t)=300e^{-4t}-510e^{-3t}+510e^{-t}-300=-180t+O(t^2).
\]
Take adjacent disjoint fixed radial bumps, outer for \(Z=\mathrm{bump}\cdot\cos(mkz)/(mk)\), inner for \(Z_u=\mathrm{bump}\cdot\cos((m+1)kz)\). For \(a=\cos(kz)\), \(m\ge2\), the overlap is \(L/4\). The same-order first-jet energy stays bounded, while the first variation of the forcing pairing grows as \(|\lambda|mk\) times a nonzero fixed radial coefficient. The local term vanishes on disjoint supports and lower compact terms remain bounded on fixed radial profiles. Extra radial decay does not repair the estimate.

A paired repair is
\[
 G_n(s)=\frac{3sn(2s^2+2ns+n^2+3n-2)}
 {(s+n-1)(s+1)(s+2)(s+n-2)},\quad
 H_n(s)=\frac{s+n-1}{s-1}G_n(s-1).
\]
Substitution gives \(G_n-H_n=K_n\) and \(sH_n(s+1)=(s+n)G_n(s)\). Use left multiplier \(1+\lambda aR^{-n}H_n(-D)\) and field \(Z_*=[1+\lambda aR^{-n}G_n(-D)]Z\). The second identity preserves the mixed \(uR\) term at first order for fixed \(a\), and the added \(a_u\) term has radial order zero because \((s+n)G_n(s)\) is bounded. The first identity changes the top compact coefficient to \(1-3\lambda aR^{-n}\). Compact commutators from differentiating \(a\) have at most one compact test derivative. The flat radial second derivative after conjugation adds a term with multiplier proportional to \(s(s+n+1)K_n(s+1)\), of radial order zero. The exact nonmean equations still have genuine mixed and radial principal terms. Finite sums of these powers follow by first-variation superposition.

For \(L^2(R^{2\sigma-1}dR)\), \(R\ge R_*\), \(\sigma>3\), each \((s+c)^{-1}\) is a terminal convolution of norm at most \(1/(\sigma+c)\). The proper rational \(G_n,H_n\) have poles left of the line. At \(\sigma=4\) their norm caps for \(n=5\) are \(825/32,645/16\) and for \(n=6\) are \(297/8,4293/64\). Invert each transform by Neumann when \(|\lambda|\|a\|_\infty R_*^{-n}\max(\|G_n\|,\|H_n\|)<1\). Finite terminal problems need compatible end data or explicit boundary terms. The \(O(\lambda^2)\) top terms, general radial profiles, and an exact coupled energy estimate remain open. [Pinned leading calculation and optical hierarchy, Eqs. (1)–(6)](https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/vacuum/TRACE.md)
