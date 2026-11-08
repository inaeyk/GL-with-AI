# Later baseline, matched signals and an alternate inverse

Use \(r_0=1\) in the orbit equations, \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\), \(f=1-1/r\), and \(I=[0,8.407362]\). The graph is \(v=r\). The real unit-normalized axial \(\ell=2\) seed is the canonical packet at \(R=13/8\) of width \(\delta=3/25\). The growing real \(L/r_0=10\) GL tangent has \(\Omega r_0\in[0.06328118,0.06328120]\). The comparison holds the canonical seed, ADM mass, circle circumference and inertial Einstein-frame cuts fixed. The source stack is [GOAL-011 at eb439228](https://github.com/inaeyk/GL-with-AI/tree/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011), [GOAL-012 at 87fd711b](https://github.com/inaeyk/GL-with-AI/tree/87fd711bf2e8117467caad1ff52c1cd344966974/research/sav/GLR-GOAL-012), [GOAL-013 at 1e8e628f](https://github.com/inaeyk/GL-with-AI/tree/1e8e628f68af4cab174fcc44b81af4748a22115c/research/sav/GLR-GOAL-013) and [GOAL-014 at 0b4bfcbb](https://github.com/inaeyk/GL-with-AI/tree/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014). The spectral boxes and physical qualifications also use the read-only [GOAL-011 audit at 75cb3ff3](https://github.com/inaeyk/GL-with-AI/blob/75cb3ff3feef98164a65cf14105be6df77ea809c/research/sav/verification/GLR-GOAL-011/AUDIT.md) and [GOAL-014 audit at 84c9824f](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/AUDIT.md).

## Directed baseline on the full later window

Let \(a=u_*=-R-2\log(R-1)\), \(p=u-a\), \(D=8.407362-a\) and \(\nu(u)=r_\Sigma(u)\). The graph radius is \(\nu(u)=1+2W_0(e^{-(1+u)/2}/2)\) with \(W_0\) the real principal Lambert function. Put \(A(p)=\sqrt{\delta}\,b(p/\delta)\) with the exact normalized bump of the preparation. The dimensionless shear master \(w\) has
\[
 w_{uv}+\frac V4 w=0,\qquad V=f(6/r^2-3/r^3),\qquad
 (w,w_v)|_\Sigma=(A(p),0).
\]
The wave vanishes below \(v=\nu(a+\delta)\) by domain of dependence. Past graph points of any point below this ingoing ray all lie strictly after the packet. The front \(p=0\) has zero data as well. The observable is \(N_{01}=w_u|_{\mathscr I^+}\). [Canonical pair, Eq. (1)](../../GLR-GOAL-011/FINITE-DATA.md) · [Earlier packet certificate](../../GLR-GOAL-013/FLUX.md).

Use a monotone radial coordinate \(z\) on the remaining strip:
\[
 \begin{cases}
 v=\nu(a+\delta-z), & 0\le z\le\delta,\\
 z=\delta+1/R-1/r((v-a)/2), & \delta\le z\le Z_{\max}=\delta+1/R .
 \end{cases}
\]
In the first formula, \(r_\Sigma(u)=\nu(u)\) lies on \(v=r\). In the second, \(r(x)\) is the inverse tortoise. The two formulas meet at \(v=R\). The physical domain is \(z\ge z_0(p)=\max(0,\delta-p)\). Set \(\alpha=dz/dv\), \(K=V/(4\alpha)\), and let \(F\) be the exact-dyadic numerical density reconstructed below. Define the candidate on the physical strip by
\[
 w_h(p,z)=A(p)+\int_0^p \int_0^z F(s,t)\mathbf 1_{s+t\ge\delta}\,dt\,ds.
 \tag{1}
\]
The integral and both of its first derivatives vanish on the graph. At the ingoing edge with \(p\ge\delta\), the candidate vanishes. The bulk defect is \(w_{h,uv}+Vw_h/4=\alpha(F+Kw_h)\). This lift keeps the graph data exact, including when the normalization of \(A\) is itself enclosed as an interval in the calculation.

The graph part is divided into equal intervals in \(p,z\) over \([0,\delta]\), producing a triangular cell whenever the graph crosses a square. Use an affine \(F\) on that triangle and a bilinear \(F\) on each full rectangle. The interface \(z=\delta\) lies on a grid line. Write \(g(\zeta)=\zeta^2(1-\zeta)\). The inner value of \(F\) at the interface equals the outer value times \((R-1)g(1/R)/(2(R+1))\), the ratio of the two \(dv/dz\) factors. Thus \(F/\left(dv/dz\right)\) and both first derivatives of \(w_h\) match there. The diagonal graph trace is bounded by its vertex values. Exact cell masses in (1) are the triangle area times the three-vertex mean or the rectangle area times the four-vertex mean. Sum row and column prefixes to evaluate the double primitive at nodes. At each active node only the local mass contains its own new value \(F_{ij}\). Solve the affine collocation equation \(F_{ij}=-K_{ij}(A_i+C_{ij})\) by dividing by \(1+K_{ij} m_{ij}\), where \(m_{ij}\) is its nonnegative mass weight, and use the Arb midpoint as the exact stored ordinate. Recompute the directed nodal defects with that stored ordinate. Each step uses interval values for the normalization and geometry. Summing local row increments before advancing the column avoids dependency inflation from repeated subtraction of the same interval prefix.

For each physical cell, bound \(\rho=|F+Kw_h|\) by the maximum nodal defect plus the interpolation error of \(Kw_h\). On a rectangle of side lengths \(h,k\) the latter is at most \((h^2 M_{pp}+k^2 M_{zz})/8\), with \(M\) the appropriate whole-cell Hessian component bounds. On the triangle use \((h^2 M_{pp}+2hk M_{pz}+k^2 M_{zz})/2\) from Taylor's formula with barycentric weights. Derivatives of \(w_h\) follow by differentiating (1). For example,
\[
 \partial_p^2(w_h-A)
 =\int_{z_0(p)}^z F_p(p,t)\,dt+\mathbf 1_{p<\delta} F(p,\delta-p)
\]
within an active cell. The \(zz\) formula likewise includes \(\mathbf 1_{z<\delta}F(\delta-z,z)\). The mixed derivative is \(F\). Prefix bounds on the value and slope of the affine or bilinear density enclose all these derivatives. One-sided Hessians suffice on the fitted mesh because the interpolation and residual estimates use the interior of each cell.

For the outer coefficient, let \(\zeta_a=\delta+1/R-z\) and \(\zeta=1/r((v-u)/2)\). Then \(\partial_p\zeta=g(\zeta)/2\), \(K=(3-3\zeta/2)\partial_{\zeta_a}\zeta\), and \(\alpha=g(\zeta_a)/2\). Away from \(\zeta_a=0\), obtain whole-cell bounds by differentiating \(\partial_{\zeta_a}\zeta=g(\zeta)/g(\zeta_a)\) and using monotone endpoint values of the flow for \(\zeta\). Use the variational equations up to third order in \(\zeta_a\) for the endpoint cell and intersect both bounds elsewhere. Explicitly, with \(b=g/2\), \(J=\partial_{\zeta_a}\zeta\), \(H=\partial_{\zeta_a}^2\zeta\), \(L=\partial_{\zeta_a}^3\zeta\),
\[
 J=e^{\int_0^p b'(\zeta)\,ds},\quad
 H=J\int_0^p b''(\zeta)J\,ds,\quad
 L=J\int_0^p\big[b'''(\zeta)J^2+3b''(\zeta)H\big]ds.
\]
The first inequality \(b'\le1/6\) and whole-cell ranges of \(\zeta\) along the flow give the bounds used in the script. At \(\zeta_a=0\), \(\zeta=0\) and \(J=1\), so \(K=3\). On the graph patch, \(dv/dz=q=(v-1)/(v+1)\), \(\zeta_z=-qg(\zeta)/2\), and \(K=qg(\zeta)(3-3\zeta/2)/2\). Differentiate these identities for the inner cells. The normalization of \(A\) uses directed midpoint quadrature of \((b_0')^2\) with second derivative \(2[(b_0'')^2+b_0'b_0''']\). For either end cell, \(\theta=[y(1-y)]^{-1}\) and \(\theta^j e^{-\theta}\) decrease for \(\theta>j\), giving bounds for the flat jets up to the orders used.

The nonnegative Killing energy for the error \(e=w-w_h\) has flux densities \(e_u^2+Ve^2/4\) on \(v=\mathrm{const}\) and \(e_v^2+Ve^2/4\) on \(u=\mathrm{const}\). The initial graph and front errors vanish. If \(R=w_{h,uv}+Vw_h/4\), the work integral has absolute bound \(2\int |R|(|e_u|+|e_v|)\,du\,dv\) since \(dt\,dx=du\,dv/2\). Applying Cauchy–Schwarz in each flux direction and maximizing over subdiamonds gives
\[
 \|N_{01}-n_{01}\|_{L^2(I)}
 \le 2\left(\|R\|_{L^1_vL^2_u}+\|R\|_{L^1_uL^2_v}\right).
 \tag{2}
\]
The two source norms are bounded by integrating the whole-cell upper bounds for \(\rho\) in \(L^1_zL^2_p\) and \(\sqrt{\alpha}\rho\) in \(L^1_pL^2_z\), respectively. Finite cap fluxes give (2) at null infinity by Fatou. The bounded compact \(K\) also gives the trace by direct Volterra integration, so no finite-radius transfer uncertainty is needed here. The candidate is \(n_{01}=A'(p)+\int F(p,z)\mathbf 1_{p+z\ge\delta}dz\) at \(Z_{\max}\). On the later window \(A'=0\) and the entire radial row is active. The candidate is continuous piecewise affine and its norm uses exact affine-cell integration.

The primary 192-bit calculation gives
\[
 \|n_{01}\|_2\simeq0.005057426316671,\quad E_0<3.897887\times10^{-5},
 \qquad \boxed{\,0.00501844<\|N_{01}\|_2<0.00509641\,}.
 \tag{3}
\]
The separate directed bounds on the two terms of (2) before the factor of two are \(1.693983\times10^{-5}\) and \(2.549611\times10^{-6}\). The coarser mesh independently encloses its own trace with \(E_0<2.17878\times10^{-4}\) and agrees within both errors. It uses the same algorithm and physical inputs. A separate characteristic march on the clustered finite-cap grid with exterior continuation gives an empirical \(L^2\) difference of \(1.6651\times10^{-7}\) from the primary compactified candidate, integrating their affine difference on the union mesh. The two marches share the preparation and potential but use different radial domains and discretizations. These computations solve only a linear Schwarzschild fixed-background baseline. [Primary output](baseline.txt) · [Coarser output](baseline-coarse.txt) · [Source](baseline.py).

## Ordinary baseline ringing and the matched ordered comparison

Let \(P_{01}=2\Re(s_q A_{01,q}e^{s_qu})\) be the actual baseline pole pair in news units. Enclose \(A_{01,q}\) by \([-0.000565,-0.000555]+i[0.000653,0.000667]\), rounded outward from the common-clock block of the pinned signed projection. The pole disk has radius \(5\times10^{-8}\) around \(-0.177924631-0.747343369i\). The baseline code propagates its Cartesian hull and the residue box jointly. For \(s=s_q\), \(Z=s A_{01,q}\), \(T=|I|\), and \(J_s(h)=(e^{sh}-1)/s\), use
\[
 \|P_{01}\|_2^2=2\left[|Z|^2J_{2\Re s}(T)+\Re(Z^2J_{2s}(T))\right].
\]
The cross term against each affine \(n_{01}\) cell \([u_j,u_j+h]\) is \(2\Re[Z e^{s u_j}(n_j J_s(h)+(n_{j+1}-n_j) \int_0^h y e^{sy}dy/h)]\), with \(\int_0^h y e^{sy}dy=[e^{sh}(sh-1)+1]/s^2\). Directed summation and (2) give
\[
 \begin{aligned}
 0.00167156&<\|P_{01}\|_2<0.00171293,\\
 0.00347060&<\|N_{01}-P_{01}\|_2<0.00362348,\\
 2.02612&<\|N_{01}-P_{01}\|_2/\|P_{01}\|_2<2.16772 .
 \end{aligned}
\]
Thus the ordinary fundamental alone fails the \(1/2\) test for the *leading* physical waveform on this window. This bound controls the combined baseline remainder including prompt and scattered terms. It gives no separate modal or branch weights. [Baseline residue and signed source output](../../GLR-GOAL-011/checks/signed_projection.txt) · [Audit of baseline normalization and clock direction](https://github.com/inaeyk/GL-with-AI/blob/75cb3ff3feef98164a65cf14105be6df77ea809c/research/sav/verification/GLR-GOAL-011/AUDIT.md#quotient-and-real-clock-direction).

For the returning trace use \(Q=2\Re(s_q A_{21,q}e^{s_qu})\) and \(S_*=2\Re(s_* A_*e^{s_*u})\), with \(s_*=s_q+2\Omega r_0\) in the dimensionless clock, and inherited physical boxes
\[
 \begin{aligned}
 A_{21,q}&\in[0.08223,0.18771]+i[-0.42418,-0.31707],\\
 A_*&\in[-0.21850,-0.16857]+i[-0.44959,-0.39966].
 \end{aligned}
\]
The switched *news* terms have a common onset at or before \(u=0\). In particular, the norms of \(Q\), \(S_*\) and \(Q+S_*\) involve the real profiles with their complex relative phase. The shift adds \(2\Omega r_0\) to the real part and leaves the carrier frequency unchanged. It comes from multiplying by the growing GL factors in the ordered forcing. The stationary Schwarzschild operator retains its original spectrum. [GOAL-012 signed projection and pole identity](../../GLR-GOAL-012/SPECTRAL-ATLAS.md) · [GOAL-013 thresholds](../../GLR-GOAL-013/WAVEFORM.md) · [Independent template geometry](../verification/REVIEW.md).

For the physical comparison, fix the *entire* leading waveform and define \(B=\eta N_{01}\), \(F=B+\epsilon^2\eta N_{21}\). The Einstein-frame monopole used in the ordered equation has the homogeneous mass and lapse constants fixed at infinity, with \(G_4=G_5/L\), \(2G_4M_{\mathrm{ADM}}=r_0\) through quadratic GL order and fixed \(L\). The sourced mass and lapse profiles still enter the wave operator. The completed canonical pair is held fixed. Spherical symmetry removes the tensor harmonic at \(\eta=0\), and half-period covariance makes the averaged trace even in \(\epsilon\). The outgoing shear normalization is \(C_{AB,\mathrm{phys}}=r_0(\eta C_{01}+\epsilon^2\eta C_{21})X_{AB}\), \(\partial_{u_{\mathrm{phys}}}C_{AB,\mathrm{phys}}=(\eta N_{01}+\epsilon^2\eta N_{21})X_{AB}\). The harmonic has \(\int X_{AB}X^{AB}d\omega=12\). Physical news norms with \(du_{\mathrm{phys}}\) acquire a factor \(\sqrt{r_0}\), and sphere-integrated norms an additional \(\sqrt{12}\). Both factors cancel in ratios. A common pullback moves the cuts and both waveforms together. A relative retiming at fixed cut labels changes the experiment by a term proportional to \(N_{01}'\) at this order. [Hierarchy, Eqs. (2)–(5)](../../GLR-GOAL-011/HIERARCHY.md) · [Matched preparation](../../GLR-GOAL-011/FINITE-DATA.md) · [Frame and flux](../../GLR-GOAL-014/FLUX-AND-FRAME.md).

Write \(L_0,U_0\) for (3), \(L_2,U_2\) for any directed enclosure of \(\|N_{21}\|_2\), and take \(\eta\ne0\). The fixed ordered comparison is exactly
\[
 \|F-B\|_2\in|\eta|\epsilon^2[L_2,U_2],\qquad
 \frac{\|F-B\|_2}{\|B\|_2}
 \in\epsilon^2[L_2/U_0,U_2/L_0] .
 \tag{4}
\]
For the selected clustered \(p_{\rm shear,\infty}\) candidate, directed arithmetic on the fixed affine ordinates gives \(c_2=\|n_{21}\|_2\in[0.08741378900667,0.08741378900668]\). Its physical error remains open. With any valid \(E_{21}\), (4) becomes
\[
 \frac{\|F-B\|_2}{\|B\|_2}
 \in\epsilon^2
 \left[\frac{(0.08741378900667-E_{21})_+}{0.00509641},
       \frac{0.08741378900668+E_{21}}{0.00501844}\right].
 \tag{4a}
\]
The absolute difference is \(|\eta|\epsilon^2\) times the corresponding norm interval before dividing by the baseline bounds. Using the candidate in place of the physical return gives a relative change between \(17.1520\,\epsilon^2\) and \(17.4186\,\epsilon^2\), where the denominator is physical and directed. A strong error strictly below \(0.08741378900667\) would guarantee a nonzero matched ordered difference from this norm test. A coarser strong error can still suffice for the two returning pole-template rejections. The physical comparison and the candidate comparison therefore have distinct error gates. [Candidate and directed ordinate arithmetic](../checks/candidate_gates.py).

A candidate \(\eta n_{01}+\epsilon^2\eta n_{21}\) for \(F\) has error at most \(|\eta|(E_0+\epsilon^2 E_{21})\), and its difference from \(\eta n_{01}\) has error \(|\eta|\epsilon^2 E_{21}\). In particular, an evaluated directed lower projection \(|\langle h,N_{21}\rangle|>c\) against \(h\ne0\) suffices for \(L_2=c/\|h\|_2\) even without a strong return upper bound. In the chosen family there are no free nuisance parameters. The ordinary pure-pole span test in the independent record refers to the isolated translated pair. Fitting amplitude or time against the *physical* baseline would require the full candidate and a common finite-translation remainder for \(N_{01}\) on enlarged cuts. [Independent review](../verification/REVIEW.md).

Conditional finite-seed interpretation follows from the uniform derivative hypotheses in the GOAL-014 frame result. Denote the exact difference from the matched unperturbed solution by \(\Delta\mathcal N\), and suppose that in the same \(L^2\) trace \(\|\Delta\mathcal N/(\epsilon^2\eta)-N_{21}\|\le e_{21}=M_{41}\epsilon^2/24+M_{22}|\eta|/4\) and \(\|\mathcal N(0,\eta)/\eta-N_{01}\|\le e_{01}=M_{02}|\eta|/2\). Then for \(e_{01}<L_0\) the relative exact norm is enclosed by
\[
 \epsilon^2\left[
 \frac{(L_2-e_{21})_+}{U_0+e_{01}},
 \frac{U_2+e_{21}}{L_0-e_{01}}
 \right].
\]
A target lower ratio \(\rho>0\) needs \(e_{21}<L_2-\rho(U_0+e_{01})/\epsilon^2\) with positive right side. The derivative constants for the full constrained family are open. A finite-\(\epsilon\) application also needs small \(|\epsilon|e^{\Omega t}\) throughout the significant fixed-radius source region and the decaying spatial envelope to control the far wedge. The ratio of isolated pole terms, including any putative late-bin growth relative to an ordinary pole, does not meet these finite-seed conditions. [Uniform trace hypotheses](../../GLR-GOAL-014/FLUX-AND-FRAME.md#conditional-nonlinear-bridge).

## Positive-line inversion with direct and first scattering terms retained

Use the exact-curl returning scalar equation \(w_{tt}-w_{xx}+Vw=-S\), where \(S\) includes the two-polarization mixed insertion, direct GL-squared term and fixed-mass mean from the hierarchy. The displacement of \(w\) on the graph is zero and its forced time velocity contributes \(g_\Sigma(u)=f(X(u))w_t|_\Sigma\), where \(X(u)\) is the graph tortoise radius. The compact lower launch terms are kept in the computation of \(S\) and in \(g_\Sigma\). Extend \(S\) by zero below the physical graph and multiply the resulting ordinary strong source by \(\mathbf 1_{u\le d}\), \(d=8.407362\). The auxiliary wave agrees with the physical wave through \(d\) by causality. This sharp cutoff needs only the \(L^1,L^2\) source hypotheses stated below. Its new residues are immaterial to the positive-line inversion. The actual untapered boxes above define the test templates. [Pinned strong pairing and graph sign](../../GLR-GOAL-012/investigators/C-INVERSION.md#returning-transfer-and-the-graph-contact).

On \(\Re s=\gamma>0\), let \(h=e^{sx}m\) be unit ingoing and \(W=-2s m_\infty\) in the outgoing Jost convention. Put \(y=u-a\) and \(A(s,x)=s\,m(s,x)/W=-m/(2m_\infty)\). The auxiliary news transform is
\[
 H(s)=\int e^{-sy}\left[
 \int A(s,x)S_\chi(u+x,x)\,dx
 -A(s,X(u))g_\Sigma(u)
 \right]du . \tag{5}
\]
The minus sign in front of the graph term is fixed by the free solution \(N_{\mathrm{free}}(u)=-\frac12\int S_\chi(u+x,x)dx+\frac12 g_\Sigma(u)\). The nonnegative-potential flux bound gives \(|m_\infty|\ge1\) and \(|A(s,x)|\le e^{M/(8\gamma)}\) for \(M=\int V\,dx=9/2\). For a fixed graph and source errors \(\Delta S\) extended in the same way, apply Plancherel for each \(x\) and Minkowski over \(x\):
\[
 \|\Delta N_{\rm vol}\|_{L^2(I)}
 \le e^{\gamma(d-a)+M/(8\gamma)}
       \int\|e^{-\gamma(u-a)}\Delta S(u+x,x)\|_{L^2_u}\,dx .
 \tag{6}
\]
The bound costs no time derivative. It is for volume errors. The graph error belongs explicitly in (5), since its kernel varies with emission position \(X(u)\).

The ingoing Volterra equation is
\[
 m(s,x)=1+\frac1{2s}\int_{-\infty}^x
       \left(1-e^{-2s(x-z)}\right)V(z)m(s,z)\,dz .
\]
It gives \(\|m\|_\infty\le e^{M/|s|}\) and \(\|m-1\|_\infty\le e^{M/|s|}-1\). Define
\[
 R_s[b](x)=\frac{1}{2s}\left[
 \int_x^\infty V(z)b(z)\,dz+
 \int_{-\infty}^x e^{-2s(x-z)}V(z)b(z)\,dz\right].
\]
Then \(m_\infty-m=R_s[m]\). The leading scattering correction to \(A\) is \(B_1=R_s[1]/2\), after the free term \(-1/2\). With \(\|R_s\|\le M/(2|s|)\), \(|m_\infty-1|\le M e^{M/|s|}/(2|s|)\), and \(|m_\infty|\ge1\),
\[
 \big|A+1/2-B_1\big|
 =\frac12\left|\frac{R_s[m-1]-(m_\infty-1)R_s[1]}{m_\infty}\right|
 \le \frac{3 M^2 e^{M/|s|}}{8|s|^2}. \tag{7}
\]
The inverse Laplace transform of \(B_1\) is the causal kernel
\[
 b_1(\tau,x)=\frac{\mathbf 1_{\tau\ge0}}4\int_{x-\tau/2}^\infty V(z)\,dz .
\]
Indeed \(e^{-2s(x-z)}/s\) is the transform of a step delayed by \(2(x-z)\) for \(z\le x\). Thus the first scattering part can be integrated in the time domain with the same source and signed graph emission, including the latter's varying \(X(u)\). The radial integral equals \(6/r-3/(2r^2)\) evaluated at \(r(x-\tau/2)\). Both the instantaneous free term and this scattering term are retained over all frequencies.

Let \(A_0=\int e^{-\gamma y}(\int |S_\chi(u+x,x)|dx+|g_\Sigma(u)|)du\). Suppose it is finite and the free volume integral and graph contact have finite weighted \(L^2\) norms. Approximate the remaining transform on \([-B,B]\) by complex constants with directed full-cell errors \(e_j\) and widths \(h_j\). The high-frequency bound (7), disjoint Fourier supports and \(L^2\) Plancherel give the absolute later-window error
\[
 e^{\gamma(d-a)}
 \left[
   \frac{\sum_j h_j e_j^2}{2\pi}
   + \frac{1}{3\pi B^3}
       \left(\frac{3M^2 e^{M/\sqrt{\gamma^2+B^2}}}{8} A_0\right)^2
 \right]^{1/2}. \tag{8}
\]
Add directed time-domain error for the free and first scattering terms. No source derivative enters (8). At \(\gamma=1/4\), the volume gain in (6) is below \(92.12\), and at \(B=128\) the second term contributes at most \(0.017178 A_0\) in \(L^2(I)\). A tail allocation of \(0.05\) would follow, for example, from \(A_0<2.9107\) with this band. At \(B=256\) the corresponding sufficient \(A_0\) upper bound is \(8.3788\). Physical \(A_0\), low-band cells and time-domain errors still require the complete source and graph. The [square-barrier controls](inverse_check.txt) independently transform the time-domain ramp of \(b_1\), compare it to the radial kernel, and test (7) against the exact barrier transfer for 168 frequency, radius and height choices. The bound also reduces to zero with \(M=0\). [Replay](inverse_check.py).

A contour shift of the untapered transform poses a distinct task. For a finite rectangle enclosing specified poles on a specified sheet, the integral on the original right segment equals the left upward segment plus residues minus both oriented horizontal wings and the clockwise boundary integrals around slits, with all nonresidue terms scaled by \(1/(2\pi i)\). The original positive-line frequencies outside the rectangle remain in the inverse. The untapered return and shifted baseline have branch endpoints at \(0\) and \(2\Omega r_0\), and the massive intermediate endpoints at \(\Omega r_0\pm ikr_0\). The inherited one-sided propagating massive threshold control gives no enclosure on the other lip or the evanescent sector. The local two-pole corridor gives neither the global count nor the needed uniform wing estimates. Equation (8) avoids deforming the line and bounds the combined finite-time response when its physical integrals are available. It does not identify individual branch or overtone amplitudes. [Pinned contour conditions](../../GLR-GOAL-012/CONTOUR-AND-INVERSION.md) · [Threshold atlas](../../GLR-GOAL-012/SPECTRAL-ATLAS.md) · [Single Schwarzschild inverse decomposition, Leaver, §§ II–III](https://doi.org/10.1103/PhysRevD.34.384).

## Same-residue finite-window freedom in the returning scalar class

A delayed source can change the window mismatch while retaining both residues, the original graph data and early waveform, even if its radial support is compact. This statement is about the returning fixed-background scalar equation with freely changed sources.

Take a real \(G\in C_c^\infty((c,\infty))\) with \(c=0\), and choose \(X_c>X(c)\). In \(x\ge X_c\) define
\[
 w(u,v)=G(u)+\int_c^u ds\int_v^\infty
           \frac{V((v'-s)/2)}4\,w(s,v')\,dv'
\]
and set \(w=0\) for \(u\le c\). The region is preserved by the integral because \(v'\ge v\), \(s\le u\) imply \((v'-s)/2\ge(v-u)/2\). The ray mass is bounded uniformly by \(\frac12\int_{X_c}^\infty V dx\). Volterra powers in \(u\) give factorially convergent iteration on every finite window. They also give an exponential upper bound on growth with an exponent made arbitrarily small by increasing \(X_c\). Differentiating proves \(w_{uv}=-Vw/4\), and taking \(v\to\infty\) gives \(w=G\), \(N=G'\) at null infinity. Multiply this exterior solution by a smooth cutoff in \(x\) that is zero for \(x\le X_c\) and one for \(x\ge X_c+1\). The manufactured forcing is supported in the transition annulus and \(u>c\); it can have late-time support there. Its graph pair is zero because the graph in that annulus lies at \(u<c\). The retarded trace has compact support in \(u\), so its Laplace transform is entire. Addition to a physical waveform leaves all continued pole residues unchanged wherever that waveform continues. To approximate any \(L^2(I)\) change, approximate it by a smooth function compactly supported in the interior of \(I\) and compensate its integral by a delayed bump after \(d\), giving a compact primitive \(G\). Consequently no positive lower bound for either physical template mismatch follows from the two residues, early traces and radial source decay alone in this class. The manufactured forcing varies the fixed Einstein source, so the construction does not decide either comparison for the pinned preparation. The verifier's distinct forward construction uses a compact spacetime forcing. [Independent review](../verification/REVIEW.md).

## Reproduction and status

From the pinned source plus this goal, under `research/sav`:
```sh
export PYTHONDONTWRITEBYTECODE=1
python3 GLR-GOAL-017/spectral/baseline.py --early 64 --late 128 --radial 128 --norm 4096
python3 GLR-GOAL-017/spectral/baseline.py --early 128 --late 256 --radial 256 --norm 8192
python3 GLR-GOAL-017/spectral/inverse_check.py
```
Python 3.14.8 and python-flint 0.9.0 produced the archived outputs. Final baseline CPU times were about 1.5 s and 6.0 s, respectively. These are bounded perturbation checks. An initial run used generic Arb exponentiation to square a sign-straddling interval and produced a NaN, and a naive interval prefix subtraction inflated the accumulation uncertainty. The final code uses interval multiplication for such squares and causal row-prefix sums. The coarse flat-jet bound at \(n=16\) also gave a vacuous energy error and was replaced by a derivative-specific endpoint estimate. The compact baseline proof uses the inherited scalar equation and preparation but newly constructs the compactification, cell reconstruction and residual certificate. The barrier checks use no GL input. The new estimates were derived with exposure to GOAL-011–014 and their review summaries. Neither the baseline enclosure nor the inverse bound by itself supplies a strong return or a finite-amplitude waveform.
