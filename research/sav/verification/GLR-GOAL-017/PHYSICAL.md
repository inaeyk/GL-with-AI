# GOAL-017 specialist record · physical extraction and causal error tests

The audited ordered return is at 9e2fe32c35151a8a4108864622cf2aef88bb0839. I independently reconstructed the axial Bondi factor, clock and graph terms, the free-current and weak identities, and controls for what the early estimates can imply. The completed conclusions and formulas were visible, so the comparison is non-blind. The full five-dimensional reduction, physical mode/mean cells, Jost and residue boxes, and outer source certificates enter as pinned inputs. Re-derivation of their full numerical enclosures is separate from this track. The manufactured arguments below use analytic fixed-background models only.

[GOAL-017 source](https://github.com/inaeyk/GL-with-AI/tree/9e2fe32c35151a8a4108864622cf2aef88bb0839/research/sav/GLR-GOAL-017) · [GOAL-013 axial source and contact ledger](https://github.com/inaeyk/GL-with-AI/blob/1e8e628f68af4cab174fcc44b81af4748a22115c/research/sav/GLR-GOAL-013/SOURCE-LEDGER.md) · [GOAL-014 frame and flux](https://github.com/inaeyk/GL-with-AI/blob/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014/FLUX-AND-FRAME.md) · [GOAL-015 independent audit](https://github.com/inaeyk/GL-with-AI/blob/1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4/research/sav/verification/GLR-GOAL-015/AUDIT.md)

## Ordered shear and inertial news

Use \(r_0=1,\ k=\pi/5,\ L=10\), the real unhalved GL cosine, and the normalized real \(\ell=2\) axial seed with canonical displacement on \(v=r\), start radius \(13/8\) and width \(3/25\). \(u=t-x\), \(v=t+x\), \(x=r+\log(r-1)\), \(p=T'(x)=-1/r\). In the compact product densities a pure radion \(s=\phi/\sqrt3\) multiplies the noncompact inverse orbit metric by \(e^{2s}\) and the sphere radius square by \(e^{-2s}\). The \(t,x,z\) orbit determinant is unchanged because the metric factors contribute \(e^{-4s}e^{4s}=1\). The noncompact axial density with one inverse orbit factor and one sphere radius square, and the curl density with two of each, are therefore radion-independent. Reflection removes the zero-mode compact vector, and the massive compact products vanish in the asymptotic extraction under the pinned weighted estimates. This leaves the interior fixed-ADM Einstein-frame mean term in the return. For the Fourier mean, \(\langle H\cos(kz)(a e^{ikz}+\bar a e^{-ikz})\rangle=H\Re a\). Here \(a\) denotes the actual \(+k\) coefficient, which already contains the half-amplitude from the unhalved GL mode at first insertion.

Let \(X_A=-\varepsilon_A{}^B D_B Y\) for \(\int Y^2d\omega=1\), and \(X_{AB}=D_{(A}X_{B)}\). The sphere curvature gives
\[
D^BX_{AB}=-\tfrac12(\ell(\ell+1)-2)X_A,\qquad
\int X_{AB}X^{AB}\,d\omega=\tfrac12\ell(\ell+1)(\ell(\ell+1)-2).
\]
The second identity follows by integrating the first against \(-X^A\) and using \(\int X_A X^A=\ell(\ell+1)\). At \(\ell=2\) the norm is \(12\) and \(\lambda=\ell(\ell+1)-2=4\). With shear coefficient \(C\) in \(g_{AB}=r^2\gamma_{AB}+r C X_{AB}+O(1)\), the Bondi radial condition gives \(g_{uA}=\tfrac12 D^B(CX_{AB})+O(r^{-1})\) and \(g_{rA}=0\). The gauge-completed curl has
\[
F_{ur}=-\partial_r(h_u/r^2)=-\frac{\lambda C}{2r^3}+O(r^{-4}).
\]
The curl of the axial potential's gradient term vanishes, and \(du\wedge dr=dt\wedge dr\). With the pinned orientation \(Q_z\sim-r^4F_{tr}\), the dimensionless shear is \(w=\lim 2Q_z/(\lambda r)\). The physical shear is \(r_0w\), so differentiating with \(u_{\rm phys}=r_0 u\) gives \(N_T=\eta N_{01}+\epsilon^2\eta N_{21}\) with no additional \(r_0\) factor. Physical \(L^2\) norms introduce \(\sqrt{r_0}\) and angular integration introduces \(\sqrt{12}\). Both cancel in relative waveform comparisons.

The integrated tensor power is \(\int N_{AB}N^{AB}\,d\omega/(32\pi G_4)\). Its \(\epsilon^2\eta^2\) coefficient integrated on a matched window is \(3r_0\mathcal P/(4\pi G_4)\), \(\mathcal P=\int N_{01}N_{21}\,du\). An ordered interference coefficient can be negative while the total exact energy is positive. For \(u_{\rm old}=u+\epsilon^2 c(u)\), inertial news composed with the common pullback gains \(cN'_{01}\) at returning order. The flux Jacobian adds \(c'N_{01}^2/2\) to the integrand of \(\mathcal P\). Together they integrate to \([cN_{01}^2]/2\), canceled by the pulled-back endpoints. A relative clock shift or fixed new coordinate cuts change the projection. A coordinate derivative of the shear must first be converted back to inertial news to apply this flux formula.

## Graph, free readout and two transpositions

The physical return equation is \(4w_{uv}+Vw=F_u+G_v-H\), \(V=f(6/r^2-3/r^3)>0\), with \(w|_\Sigma=0\), \(4w_u|_\Sigma=G-b'F\) on \(v=b(u)\). Here \(b'=-(1+p)/(1-p)<0\). The tangential condition is \(w_u+b'w_v=0\). Integrating the equation from the graph on each outgoing ray, with the derivative of the moving lower endpoint retained, gives
\[
4w_u(u,v)
=G(u,v)+\partial_u\int_{b(u)}^v F(u,s)\,ds
-\int_{b(u)}^v(H+Vw)(u,s)\,ds.
\]
This is the cap extraction that can be compared with a numerical shear derivative. Even for \(V=0\) and constant \(F,G\), there can be nonzero induced velocity though the differentiated bulk currents vanish. On the straight decreasing graph \(b(u)=b_0-\gamma u\), \(\gamma>0\), the exact free check is \(4w=(v-b(u))F+(u-b^{-1}(v))G\) on a local patch. Its derivative is \(4w_u=\gamma F+G\). Zero upper canonical pairs do not mean zero graph velocity.

Let \(J\) be the graph-compatible free lift, obtained by using the same readout with \(V=0\) and integrating in \(u\) from the graph or flat front. Then \(4(J)_u=\partial_u\int_b^v F+G-\int_b^v H\). In \(L_{\rm RW}=-4\partial_u\partial_v-V\), the remainder \(w-J\) has source \(VJ\) and zero graph jets. This separates the strong effect of a single differentiated \(F\)-ray from all other current errors, which enter the scattered estimate through undifferentiated primitives. For a conforming approximate scattered field \(z_h\), its error equation is \(L_{\rm RW}((w-J)-z_h)=V(J-J_h)-[L_{\rm RW}z_h-VJ_h]\). The positive stationary energy gives the two mixed-norm source gains by applying Cauchy–Schwarz to the \(\partial_u,\partial_v\) fluxes on their corresponding sections. It also requires the actual graph and front flux defects and absence or enclosure of seam distributions. Piecewise differentiation under a moving ray integral would include a term \(-\gamma'[\delta F]\) on a discontinuity \(v=\gamma(u)\).

The failure of strong control by undifferentiated currents can be made exact even at nonzero \(V\). Choose a smooth nonzero \(\chi\) supported strictly inside \(I=[0,8.407362]\), set \(a_n=n^{-1/2}\chi(u)\sin(nu)\), and choose a smooth switch \(\xi(v)\) vanishing for \(v\le R=13/8\) and equal to one at and above a fixed cap. Then
\[
w_n=a_n(u)\xi(v)/4,\qquad F_n=a_n(u)\xi'(v),\qquad G_n=0,\qquad H_n=-Vw_n
\]
solve the prescribed scalar equation exactly with the graph and front flat. The undifferentiated \(F_n\) primitive, \(\int |H_n|\,dv\), and the potential-weighted weak norms on the strip are \(O(n^{-1/2})\). The late news derivative is \(a_n'/4\), with norm asymptotic to \(n^{1/2}\|\chi\|_2/(4\sqrt2)\). This control works for the allowed scalar current class. The physical \(F,G,H\) are determined by the GL hierarchy, so the control diagnoses the estimator rather than providing a free physical correction.

For a weak witness, take \(g\in H^1_0(I)\), extend by zero to the front \(a=-13/8-2\log(5/8)\), and solve \(4Z_{uv}+VZ=0\) backward with \(Z(\cdot,c)=g,\ Z(d,\cdot)=0\). The Green identity follows from
\[
4(Zw_{uv}-wZ_{uv})=2\partial_v(Zw_u-wZ_u)+2\partial_u(Zw_v-wZ_v).
\]
On the cap, \(g\) vanishes at its endpoints and integration by parts gives \(4\langle g,w_u(\cdot,c)\rangle\). On the graph, the wave boundary term is \(-2Zw_u+2b'Zw_v=-4Zw_u=Z(b'F-G)\), which matches the graph boundary term from integrating \(F_u+G_v\) by parts. Thus
\[
4\langle g,w_u(\cdot,c)\rangle
=\langle g,G(\cdot,c)\rangle-\iint(Z_u F+Z_vG+ZH)\,du\,dv .
\]
The flat front and vanishing right trace eliminate their terms. For real fields, stationarity \(V_u+V_v=0\) gives
\[
\partial_v(Z_u^2+VZ^2/4)+\partial_u(Z_v^2+VZ^2/4)=0 .
\]
Integrate it on the future rectangle \([u,d]\times[v,c]\), which is contained in the graph wedge because \(b'<0\). The flux on \(u=d\) is zero and the flux on \(v=c\) is at most \(\|g'\|_2^2+\int Vg^2/4\). Nonnegative flux bounds both other sections, giving the two weak mixed norms for \(F,G\) and the \(H/\sqrt V\) bound with a factor of two. As the cap tends to infinity the physical \(G\) trace and cap potential term vanish under the inherited exterior bounds. This yields a rigorous conditional signed rejection test but no evaluated compact physical moment error.

The second transposition is consistent with the mixed graph forcing. For \(Y|_\Sigma=0\), \(Y_x=-pY_t\) there. Applying the wave principal part of \(L_m\) to \(\Theta Y\), \(\Theta=\mathbf1_{t\ge T(x)}\), gives the graph term \(-(1-p^2)Y_t\delta(t-T)\). The physical current launch supplies the same coefficient \(B^t-pB^x\) in \(\partial_t(\Theta B^t)+\partial_x(\Theta B^x)\). No new contact appears from a first-order term because the field vanishes on the graph. The stationary skew time matrix gives \((\mathsf C\partial_t)^*=\mathsf C\partial_t\) in the real spacetime pairing. A smooth tapered mixed adjoint can therefore move the physical source to an uncommuted baseline first-jet pairing. Its own forcing contains derivatives of the scalar test, so a scalar first-derivative energy alone does not certify the mixed solve. At the terminal \(u=d\), \(Z=Z_v=0\) and \(\partial_v Z_u=-VZ/4=0\); hence \(Z_u=0\) there only if \(g'(d)=0\). The printed sine witnesses generally fail this derivative condition. A stronger taper or the terminal boundary term is needed if using them with the mixed transposition. Their direct scalar weak identity remains valid.

## GOAL-015 and what the later comparison can decide

The earlier verified \([3.3605,12.3142]\) and \([-2758.258,-200.091]\) are signs of one weak overlap on \([a,a+0.015]\) and \([a,a+0.12]\). Both intervals end before \(I\). The cumulative ordered coefficient is continuous because its integrand is \(L^1\), so the two cut signs force a zero between the ends. Neither sign identifies the sign of the exact tensor energy or the sign of the news. The strong cap bound \(E_c<323.984\) applies to the exact affine early benchmark on its short interval. Even an exact early waveform and cap readout leave later source or lower-field errors uncontrolled. The new causal graph descends to \(b(d)\approx1.0090210771\). The public GOAL-015 coefficient interface starting at \(25/16\) and its lower regular-branch cells starting at \(9/8\) both require extension. GOAL-017 supplies a directed coefficient extension to \(1+1/112<b(d)\), with wide second-jet cells and a separate unclosed map to physical candidate errors.

For a direct control of inference from early signs, graph and residues, choose \(h'=g\) on \(I\) for any \(g\in C_c^\infty(I)\), with \(h=0\) before zero and \(h\) smoothly tapered to zero after \(d\). Let \(w=h(u)\chi(x)\), where the radial switch is zero inside a large radius and one beyond a fixed larger radius. Define the auxiliary source \(S=(4\partial_u\partial_v+V)w\). It is smooth, starts after the early cuts and is zero on the whole initial graph. The graph assertion uses the negative launch \(u\) at all radii where \(\chi\) can be nonzero. The source has an integrable \(Vh\) outer ray tail, so this example uses an allowed decaying source instead of claiming compact spacetime support. The exact retarded solution is \(w\); the source leaves the full early trace and cap unchanged, while the late news is exactly \(g\). The Laplace transform of the added news is entire because \(h'\) has compact retarded support. It leaves both specified news-pole residues unchanged. This elementary control recovers the dense late freedom in the enlarged scalar problem and is separate from the source's compact-forcing proof. It is inapplicable as a variation of the pinned Einstein source.

The baseline ordinary-pair failure follows already from the norm ranges without using correlation or candidate mismatch quadrature:
\[
\frac{\|N_{01}-P_{01}\|_2}{\|P_{01}\|_2}
\ge \frac{0.00501844}{0.00171293}-1>1.929 .
\]
The source's sharper directed result \([2.02612,2.16772]\) uses the affine correlation and the physical baseline error. No return result follows from it. An independent, weaker sufficient rejection test for \(P=Q,Q+S_*\) uses only the directed candidate norm and the template lower norm. If \(E_{21}+0.08741378900668\le L_P/2\), then the reverse triangle inequality gives \(\|N_{21}-P\|_2\ge\|P\|_2/2\). The archived template lower endpoints give sufficient \(E_{21}\le 0.21225\) and \(E_{21}\le 0.70455\), respectively. These are deliberately weaker than the candidate-mismatch thresholds. All these gates still require a physical error estimate. Template rejection alone can occur even at zero physical return, so it does not establish a positive matched-signal difference. A norm bound for that difference from the stated candidate would instead require \(E_{21}<0.08741378900667\), or a separate directed nonzero witness. The reported matched ratio with candidate numerator is a numerical estimate with a directed baseline denominator.

The exact ordered result relies on the physical early history, common cuts and fixed source. Applying it at finite seed needs a uniform two-parameter Einstein solution-to-news expansion on matched cuts, alongside perturbative smallness in the significant source region. The two physical return tests, separate overtone and cut budgets, positive matched separation, and finite-seed identifiability remain open. The calculation supports a directed later leading waveform and leading ordinary-pair mismatch, exact conditional estimators, directed arithmetic on a numerical returning candidate, and obstructions in enlarged source classes.
