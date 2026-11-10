# Directed prefix sign from a sharp late-tail comparison

Write \(\mathcal P(t)=\int_a^t N_{01}N_{21}\,du\) for the benchmark, \(d_1=a+0.12\), \(d_0=a+0.0955\), and keep the audited full-packet cap \(c=d_1+2x(400)\). For \(g=N_{01}\), use the test \(g\mathbf1_{[d_0,d_1]}\) in the full causal wedge. With \(\mathcal F(u)=\int_b^cF(u,v)\,dv\) the exact scalar pairing gives
\[
 4[\mathcal P(d_1)-\mathcal P(d_0)]
 =g(d_1)\mathcal F(d_1)-g(d_0)\mathcal F(d_0)-\int_{d_0}^{d_1} g'\mathcal F\,du+\text{dual}+\text{exterior}.
\]
Both sharp \(F\) contacts are required. The compact \(G\) cap term cancels against the physical exterior term using the infinite scalar test. The inherited dual operator has an upper bound \(854\|g\|_{L^2([d_0,d_1])}\) in four-times-projection units, using the full physical packet current norms. This follows by dividing the original \(854.54174865\) bound by the original test norm bound \(1.000930\). [GOAL-014 sharp test and cap identity](https://github.com/inaeyk/GL-with-AI/blob/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014/ADJOINT-AND-ENDPOINTS.md) · [GOAL-015 signed packet bound](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/observables/PACKET.md)

The pinned complete primitive is \(\mathcal F=\varphi B''+C_F B'+R_F\), where \(B\) is the normalized bump, \(\varphi=(\int_b^c q_0\,dv)^2/4\), \(C_F\in[-9.077,49.403]\), \(\|R_F\|_2<8.813\), \(\|R_F\|_\infty<45.531\). On the final suffix \(B'<0,B''>0\) in the interior. Let \(n_j=\|B^{(j)}\|_{L^2([d_0,d_1])}\), \(D=d_1-d_0\), \(M=1.60\). The scalar Volterra bound gives \(e_*=\|w_0-B\|_\infty<0.0171\), \(\|g\|_{2,[d_0,d_1]}\le n_1+M(n_0+e_*\sqrt D)\). Using \(|W_u|\le W/4\), \(W\le1/4\), \(|b'|\le5/21\), and \(W=V/4\), the once-differentiated ray gives \(\|g'-B''\|_2\le M n_1+3.020 n_0+2.960e_*\sqrt D\) on the suffix. At \(d_0\), \(g_0=g(d_0)\) lies in \(B'(d_0)-[1.562,1.60]B(d_0)+[-Me_*,Me_*]\). The right terminal contact uses \(|g(d_1)|<0.009755\) and flat bump jets.

Independently enclose the common phase for the suffix \([a+0.093,d_1]\). In the geometry quadrature on \(r\) use \(dv=2\,dr/f\), \(v=u+2x(r)\) and the exact graph \(b(u)=1+2W_0(e^{-(1+u)/2}/2)\) with \(W_0\) the principal Lambert function. A left optional radial panel bounds the variable launch and a right optional panel bounds the variable radius at the fixed cap. Directed whole-cell quadrature of the pinned radial coefficient yields \(\varphi\in[1.33075,1.53038]\). Direct differentiation of its outgoing primitive using graph and cap terms gives \(|\varphi'|<4\). Thus the principal adverse term in \(4[\mathcal P(d_0)-\mathcal P(d_1)]\) is bounded above by
\[
 \sup_{\varphi_0,g_0}\varphi_0(g_0 B''(d_0)+n_2^2)
 +4\int_{d_0}^{d_1}(u-d_0)B''(u)^2du .
\]
Here the same \(\varphi_0\) multiplies the signed endpoint and bulk terms. The integral is directed over whole cells. For the \(C_F\) bulk use \(C_F\ge-9.077\) and \(\int |B''B'|=B'(d_0)^2/2\). Retain the signed \(C_F(d_0)g_0 B'(d_0)\) endpoint. The \(R_F\) bulk and contact cost at most \(45.531(|B'(d_0)|+|g_0|)\), and the \(g'-B''\) bulk uses Cauchy–Schwarz with \(1.53038n_2+49.403n_1+8.813\). The adjoint and the two exterior sharp contacts use the inherited \(r_c=400\) weak-current tails.

The directed loss upper endpoint is less than \(195.66774\) in projection units. The benchmark full packet upper is \(-200.091\). The executable interval result gives \(\mathcal P(a+0.0955)<-4.42326\). The more aggressive \(a+0.095\) gives upper \(20.90142\) rounded outwards and leaves that cut undecided. This negative control retains the identical equations and phase enclosure. No numerical packet waveform enters either decision. [Directed run](checks/short_tail.txt) · [Exact command](REPRODUCTION.md)

The ball perturbation of each prefix is below \(0.041\) by restriction of the same two operator norms. Therefore every profile in the ball has \(\mathcal P(a+0.015)>3.31\), \(\mathcal P(a+0.0955)<-4.38\), and at least one zero strictly between. The certificate does not determine multiplicity or an instantaneous news sign.
