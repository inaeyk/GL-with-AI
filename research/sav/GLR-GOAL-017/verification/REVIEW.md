# Adversarial verification of the later tensor comparison

The target is the real axial zero-mode news coefficient \(N_{21}\) on the physical dimensionless interval \(I=[0,d]\), \(d=8.407362\). Use \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\), and the Regge–Wheeler operator \(L=-4\partial_u\partial_v-V\), \(V=(1-1/r)(6/r^2-3/r^3)\). The exact-curl outgoing shear \(C_{21}\) appears as \(r_0\epsilon^2\eta C_{21}X_{AB}\) in the physical shear. Its derivative with respect to \(r_0u\) gives \(\epsilon^2\eta\,\partial_u C_{21}X_{AB}\). The five-dimensional reduction, Einstein-frame extraction, matched charges and clock, canonical graph, and local spectral inputs are inherited. [GOAL-014 source 0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195) · [GOAL-014 audit 84c9824f826e3517c8a1ed74d96bea061ab88584](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/AUDIT.md)

The launch exposed the prior thresholds and test question. The verifier read the previous template formula and output, graph/source derivations, and the GOAL-014 Work late-source example. The real-basis calculation below is a fresh implementation using the original spectral boxes and exact box optimization. It shares the inherited pole and residue enclosures. The compact-force construction uses time-Laplace annihilation and a forward causal estimate. The earlier Work construction prescribed null-infinity news and integrated backward to make an advanced-slab source. This review did not rerun the spectral pole or residue proofs. The controls and Gram bounds preceded access to any new GOAL-017 waveform candidate or strong error estimate. The weak-witness audit below was a separate non-blind review of the coordinator's derivation. The runtime role is independent verification. The backend model and effort were not separately observed.

## Directed pole templates

Let \(s_q^0=-0.177924631-0.747343369i\), \(s_*^0=s_q^0+2(0.06328119)\), with real midpoint \(-0.051362251\) for the latter. The certified disk radii can be taken as \(5\cdot10^{-8}\) and \(7\cdot10^{-8}\), respectively, using the GOAL-011 Euclidean disk and the full growth bracket. The inherited shear residue boxes are
\[
 A_q\in[0.08223,0.18771]+i[-0.42418,-0.31707],\qquad
 A_*\in[-0.21850,-0.16857]+i[-0.44959,-0.39966].
\]
The residue in the news is \(s_j A_j\). The derivative of a causal waveform multiplies its transform by \(s\) up to an entire endpoint term. Reality supplies the conjugate, so \(Q=2\operatorname{Re}(s_q A_q e^{s_q u})\), \(S_*=2\operatorname{Re}(s_* A_*e^{s_*u})\). Use a common news onset at or before zero. Switching a shear before differentiating it would also create an onset contact. The fixed cut exceeds the largest allowed fundamental period by \(6.8768\times10^{-7}\). [GOAL-011 disk](../../GLR-GOAL-011/SPECTRAL-CERTIFICATE.md) · [GOAL-011 signed enclosure](../../GLR-GOAL-011/SIGNED-RESIDUE.md) · [GOAL-012 spectral atlas](../../GLR-GOAL-012/SPECTRAL-ATLAS.md) · [GOAL-013 waveform conventions](../../GLR-GOAL-013/WAVEFORM.md)

For fixed \(s=\alpha+i\beta\), the real and imaginary parts of \(A\) multiply the columns
\[
 2e^{\alpha u}(\alpha\cos\beta u-\beta\sin\beta u),\qquad
 -2e^{\alpha u}(\beta\cos\beta u+\alpha\sin\beta u).
\]
All Gram entries use elementary real integrals of \(e^{(\alpha+\gamma)u}\) against \(1,\cos 2\beta u,\sin 2\beta u\). The Gram matrix is positive semidefinite. The largest norm over a box is attained at a corner by convexity, including for the radius about its midpoint. For the minimum, a directed check of the box KKT signs suffices because
\[
 x^\mathsf T Gx-x_*^\mathsf T Gx_*=(x-x_*)^\mathsf T G(x-x_*)+
 2(x-x_*)^\mathsf T Gx_* .
\]
The first term is nonnegative and the second has the correct sign componentwise at a certified face. All three minima here are corners. For a pole displacement \(\delta s\) of modulus at most \(r_s\), integrate \((1+su)e^{su}\) along the segment in the left half-plane. The induced news error is at most \(2r_s |A|\sqrt d[1+(|s^0|+r_s)d]\). Use the box maximum of \(|A|\) and add the two bounds for the combined template. The resulting enclosures keep the exact amplitude optimization at the center frequencies and cover the shared uncertain physical growth bracket.

| Template | Directed norm range | Upper midpoint radius | Sufficient pass if \(d_h+E_{21}\) is below | Sufficient rejection if \(d_l-E_{21}\) is at least |
| --- | --- | --- | --- | --- |
| \(Q\) | \([0.59933322,0.87547766]\) | \(0.14763029\) | \(0.15203632\) | \(0.58536912\) |
| \(Q+S_*\) | \([1.58394176,1.92785119]\) | \(0.23674392\) | \(0.55522696\) | \(1.20066951\) |
| \(S_*\) | \([1.06148476,1.22043506]\) | \(0.09173771\) | — | — |

Here \([d_l,d_h]\) encloses the candidate mismatch from the exact center template, and \(E_{21}\) is the absolute physical candidate error. The forward and reverse triangle inequalities give decisive margins \(L_P/2-\Delta_P-d_h\) for passage and \(d_l-U_P/2-\Delta_P\) for rejection. The pass threshold uses the strictly positive first margin. Rejection permits equality with the threshold. [Directed calculation](template_boxes.py) · [Archived output](template_boxes.txt)

A pass for \(Q\) requires \(\|N_{21}-Q-S_*\|_2>0.62374593\) by \(\|S_*\|_2-\|Q\|_2/2\). Also \(\|S_*\|_2-\|Q+S_*\|_2/2>0.09755917\). Thus \(N=Q\) passes \(Q\) and rejects \(Q+S_*\), while \(N=Q+S_*\) reverses the outcomes. The **midpoint** pass balls overlap. Their summed radii minus the distance between their centers exceeds \(0.10479383\). For example, \(N=Q_{\rm mid}+0.27(S_*)_{\rm mid}\) has mismatch norms \(0.27\|(S_*)_{\rm mid}\|\) and \(0.73\|(S_*)_{\rm mid}\|\), each below its half norm. The two pass tests can therefore agree without identifying a unique decomposition.

## Compact retarded same-residue control

Consider real smooth compact-spacetime sources added to \(Lw=S_{21}\), with zero independent added graph data and no incoming signal. Set
\[
 p_*(z)=(z-s_q)(z-\bar s_q)(z-s_*)(z-\bar s_*).
\]
This polynomial has real coefficients. Integration by parts in time gives \(\widehat{p_*(\partial_t)R}(s,x)=p_*(s)\widehat R(s,x)\) for compact \(R\). The stationary retarded scalar transfer then has the same multiplier in the radiation transform. Its continued source projection is locally analytic at all four points, since the radial integration for compact \(R\) uses a compact interval with locally analytic Jost factors. The scalar Green function has at most simple poles at \(s_q,\bar s_q\), and its Wronskian is nonzero at \(s_*,\bar s_*\). Thus \(p_*(\partial_t)R\) contributes zero residue at all four points on the inherited local sheet. [Regge–Wheeler transfer](../../GLR-GOAL-011/GREEN-FUNCTION.md) · [Downstream noncoincidences](../../GLR-GOAL-012/SPECTRAL-ATLAS.md)

The attainable later news perturbations are dense in \(L^2(I)\). To see this, start with a real \(\varphi\in C_c^\infty((0,d))\). Solve \(p_*(\partial_u)h=\varphi\) with \(h\) zero before the support, then cut \(h\) off smoothly after \(d\). This gives a smooth compact \(h\) with the prescribed equation on \(I\). Take real \(\psi\in C_c^\infty((0,1))\) of integral one and
\[
 R(u,v)=-4h(u)\psi(v-v_0),\qquad R_p=p_*(\partial_t)R .
\]
For sufficiently large \(v_0\) the support is entirely in the future of the graph and at \(u>0\). Causality keeps all early and packet traces unchanged. Both forces are compact in \((t,x)\). In the free equation the added news is \(-\frac14\int R_p(u,v)\,dv=\varphi(u)\) on \(I\), since \(\partial_t=\partial_u+\partial_v\) and the integrated \(\partial_v\) terms vanish.

On \(I\times[v_0,\infty)\) the true cap news \(P=\partial_u w\) starts at zero, and \(w(0,v)=0\). Its evolution is
\[
 P_v=A(v)P-R_p/4,\qquad A(v)P=-\tfrac14V(\cdot,v)\int_0^{(\cdot)}P(u')\,du'.
\]
At \(r\ge400\) the potential decreases, so its maximum along each \(u\)-section is at \(d\). The elementary Young bound for the primitive gives
\[
 K=\int_{v_0}^\infty\|A(v)\|_{2\to2}\,dv
 \le d \left(\frac{3}{r_m}-\frac{3}{4r_m^2}\right),\qquad
 r_m=r((v_0-d)/2).
\]
Writing \(B=\|R_p\|_{L^1_vL^2_u}/4\), Grönwall yields \(\|P(v)\|_2\le B\exp(\int_{v_0}^v\|A\|)\). Integrating \(A(v)P\) proves convergence and \(\|\delta N-\varphi\|_2\le(e^K-1)B\). The source norm \(B\) is independent of \(v_0\) by translation. Since \(K\to0\) and compact smooth targets are dense, the claimed density follows.

Given a reference waveform with the pinned residues and early traces, there are retarded modified waveforms arbitrarily close to \(Q\) or \(Q+S_*\) on \(I\), each with the same four residues and all early traces. The strict margins above give opposite pass/reject outcomes for these two modifications. Waveforms arbitrarily close to zero give two rejections. The theorem tests implications from residues and early traces alone. The canonical GL hierarchy fixes all sources and contains no free \(R_p\).

## Nuisance-degenerate controls and isolated separation

For a pure ordinary baseline pair \(n_q=2\operatorname{Re}(z_0e^{s_qu})\), \(z_0\ne0\), its amplitude and common time-origin tangents \(n_q,n_q'\) span the full two-dimensional ordinary pair space. The coefficient of any ordinary correction is \(z=z_0(a+b s_q)\) for real \(a,b\), with \(b=\operatorname{Im}(z/z_0)/\operatorname{Im}s_q\) and \(a=\operatorname{Re}(z/z_0)-b\operatorname{Re}s_q\). Locally a finite complex amplitude change is also equivalent to a common time shift and positive real scaling for a pure pair.

For the isolated translated pair there is a directed separation. Let \(B_q,B_*\) be the two real-column template maps at the center exponents and \(G_q=B_q^\dagger B_q\), \(G_*=B_*^\dagger B_*\), \(C=B_q^\dagger B_*\). The projected squared norm is the quadratic form of \(G_*-C^\mathsf T G_q^{-1}C\) on the translated residue vector. Use the same directed KKT and corner checks. The perturbed ordinary column map differs from \(B_q\) in operator norm by at most the unit-residue frequency bound \(\delta_q\). Both best-fit coefficients are bounded by the maximum target norm divided by \(\sigma_{\min}(B_q)-\delta_q>0\), since the projected target has no larger norm than the target. This controls the change of each direction of the distance comparison by the translated frequency error plus the coefficient bound times \(\delta_q\). The resulting enclosure is
\[
 0.30064053\ \le\ \operatorname{dist}_{L^2(I)}
 (S_*,\,\operatorname{span}_{\mathbb R}\{\Re e^{s_qu},\Im e^{s_qu}\})\
 \le\ 0.34380362 .
\]
This is the isolated translated pair's distance from the whole ordinary amplitude and phase family. The full return can contain additional terms in that transverse direction. [Directed calculation](template_boxes.py)

The compact-source theorem can make the ordered correction arbitrarily close to \(cN_{01}\) for any fixed allowed nuisance tangent \(c\), preserving the four residues and early traces. Then the matched waveform can be arbitrarily close to the nuisance waveform \((1+\epsilon^2c)\eta N_{01}\) on \(I\). There is also exact degeneracy in a larger scalar forced class with integrable outer tails. Assuming smooth traces near \(I\), choose \(g\in C_c^\infty\) zero on the early window with \(g'=cN_{01}-N_{21}\) on \(I\), by integrating a smooth extension and cutting the primitive off outside the window. Take a smooth \(\chi(v)\) zero near the graph on the support of \(g\) and equal to one at large \(v\). Then \(\delta w=g(u)\chi(v)\) has source \(-4g'\chi'-Vg\chi\), zero graph data and news \(g'\). The source has an \(O(r^{-2})\) tail at fixed \(u\). The added news transform is entire because \(g'\) is compact. The control changes the prescribed scalar source and is independent of any permissible change to the physical preparation.

## Graph, cap and regularity checks

1. \(T(x)=r-x\) has \(p=T'=-1/r\) and \(b'=(1+p)/(p-1)\). Zero upper displacement gives \(w_x=-pw_t\), so \(w_u=(1+p)w_t/2\) and \(w_v=(1-p)w_t/2\). The exact-curl constraint \((1-p^2)w_t=m(P_e-pQ_e)\) gives \(4w_u=2m(P_e-pQ_e)/(1-p)=G-b'F\). This independently checks the sign and the single factor \(1-p\) in the characteristic velocity. The two massive graph velocities arise from the noncompact and compact electric constraints. [Pinned current algebra](../../GLR-GOAL-014/CURRENTS-AND-DATA.md)
2. Evolve the entire causal strip \(u_*\le u\le d\), \(v\ge b(u)\), before restricting the trace to \(I\). The graph intersection \(b(d)\) is strictly exterior. Measure the minimum cap radius on the final \(u=d\) ray. The fixed-mass mean constraint integrals retain their outer endpoints. The early and packet strips do not cover the late domain.
3. Across \(u=u_s\), the continuous field \(w=(u-u_s)_+\psi(v)\) has common tangential \(w_v\). Its uncommuted \(\partial_u\partial_v w=\mathbf1_{u>u_s}\psi'(v)\) has no delta, whereas \(\partial_t Lw\) contains \(-4\delta(u-u_s)\psi'(v)\). Thus even aligned null seams of a continuous lift can require explicit commuted seam terms. The time-commuted graph jet similarly uses \((1-p^2)e_{tt}=h'-p g'-We+\mathsf Cg-R|_\Sigma\) for \(g=e_t|_\Sigma\) and \(h=e_x|_\Sigma\), with primes denoting graph derivatives in \(x\). This follows by eliminating \(e_{tx}\) and \(e_{xx}\) from the tangential derivatives and the PDE. Near the final ray the graph denominator is small. [Finite-cut energy ledger](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md)
4. To use strong cap transfer, bind a specific candidate \(p_c\) on the full causal window and evaluate its difference from the cap trace of the residual-tested volume lift. The physical strong exterior force, potential-series truncation and quadrature each contribute separately. A weak exterior cancellation controls a different quantity. [GOAL-013 cap bound](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md) · [GOAL-014 cap pilot](../../GLR-GOAL-014/STRONG-NEWS-PILOT.md)

## Non-blind weak-witness audit

The signs and orientations of the finite-cap witness identity in [WEAK-WITNESSES](../analysis/WEAK-WITNESSES.md) follow from \(ZL^+w-wL^+Z=4\partial_v(Zw_u)-4\partial_u(Z_vw)\), \(L^+=4\partial_u\partial_v+V\). The graph term \(-4Z_b w_u\) cancels \(b'Z_bF_b-Z_bG_b\) by the canonical graph relation. If \(G\) is extended by zero across \(v=c\), its divergence contributes \(-G(c)\delta(v-c)\). The finite-cap physical formula retains \(+\langle g,G(c)\rangle\). The infinite-cap pairing with the extended currents incorporates the negative contact through transposition. Mixing those conventions would omit or double count the cap term.

The backward energy identity follows by differentiating the two squared null derivatives and using \(V_u+V_v=0\). Every backward integration rectangle from a point in the wedge to the top and right sides stays in the wedge since \(b\) decreases. The right flux is zero because \(Z(d,v)=Z_v(d,v)=0\). The two lower positive fluxes sum to at most the top cap energy. The mixed-norm estimate then follows by directional Cauchy–Schwarz. Opposite directional products obey weighted Cauchy–Schwarz over the wedge and the row weight enters once in each square integral, giving the geometric mean of the two section-supremum integrals. For \(H\) one must include the \(2/\sqrt V\) row factor. If a witness is exactly orthogonal to the midpoint pair, the prefactor error \((s-s^0)Ae^{s^0u}\) pairs to zero. The remaining \(sA(e^{su}-e^{s^0u})\) gives the stated \(u\)-weighted shift bound. Approximate orthogonality requires a separate pairing error.

These checks validate the estimator identities. A rejection via the weak route still needs an evaluated physical moment error, including lower-field graph and bulk estimates, whole-cell coefficient rows and the directed witness quadrature. A strong waveform conclusion requires the full-strip physical graph/front, coupled residual, seam and cap trace estimates or another valid strong bound. Uniform finite-amplitude news control in the matched frame is a separate gate.

## Non-blind strong estimator and pilot audit

The free subtraction in [STRONG-NEWS, equations (2) and (8)](../analysis/STRONG-NEWS.md) checks directly. The full \(Lw\) equals the free derivative source, while \(LJ_s\) adds \(-VJ_s\), so \(L(w-J_s)=VJ_s\). The corrected graph displacement and both first jets are zero. Differentiating the free cap primitive retains \(-b'\delta F|_\Sigma\) and \(+\delta G|_c\). An oblique seam \(v=\gamma(u)\) adds \(-\gamma'[\delta F]\) in that differentiated ray. A scalar double primitive of a continuous drive has a continuous gradient including at the graph/front junction. Its continuity does not control the commuted residual of an independently interpolated lower-field lift.

For the exterior, an initialized news candidate gives the shear by taking its primitive from the true front. Any discrepancy with a separate cap shear candidate enters both the earlier \(L^2\) shear and the endpoint at zero. The late signed gain follows from \(\partial_v\|p_e\|_2\le\frac18\sup V_u\,\|\mathcal I_0\|^2\|p_e\|_2\) for the homogeneous evolution, where one can use the positive part of \(\sup V_u\). Integrating the coefficient gives \(3d^2/(\pi^2r_c^2)\). This check verifies the strong bound with its stated graph, cap and regularity assumptions. It supplies no measured coupled residual.

The pilot's time-differentiated \(F\) readout uses \((\partial_t+\Omega)Y\) in the mixed return and \((\partial_t+2\Omega)w_0\) in the lower source. These agree with the time factors from one GL profile in the mixed contribution and two GL factors in the direct and fixed-mass mean contributions. For the fine pilot's affine shear-derived continuation, 12-point Gauss–Legendre quadrature per affine segment gives
\[
 \|n\|_2=0.08742919688403797,\quad
 \|n-Q_{\rm mid}\|_2=0.7490076831884529,\quad
 \|n-(Q+S_*)_{\rm mid}\|_2=1.789254738034351 .
\]
This independent quadrature uses shared candidate ordinates. It confirms the candidate norms and mismatches at the displayed precision. The directed norm gate is separate and needs the physical waveform error. The two pilot readouts shared the lower fields and return shear, so their difference measures sensitivity of the readout and supplies neither a consistency bound for the shared hierarchy nor a strong physical error. [Candidate quadrature](candidate_replay.py) · [Archived output](candidate_replay.txt)

## Non-blind near-horizon coefficient audit

The regular EF extension in [geometry_extension.py](../analysis/geometry_extension.py) uses the inherited regular GL branch and matched scale. At \(r=9/8\), the analytic profile and the inherited outward compact construction are enclosures of the same solution because the compact construction starts from the regular profile with the same normalization. Their overlap is a consistency check. At \(r=25/16\), the inherited mean box is at the right edge of the first retained outer mean cell. Subtracting the cell length times its whole-cell derivative enclosure yields a valid left endpoint box for backward continuation.

The Taylor coefficient \(q[2]\) is half the mean-integrand second derivative, so midpoint remainder \(h^3\sup|f''|/24\) is \(h^3\sup|q[2]|/12\). The radial mean step subtracts this enclosed integral from the right mean value. The box intersection with the local mass constraint depends on the inherited fixed-infinity local mass identity. The regular-series derivative tail \(j!\,{\rm tail}(\rho)/(\rho-\rho_{\rm in})^j\) follows from Cauchy on disks of radius \(\rho-\rho_{\rm in}\) centered at points of modulus at most \(\rho_{\rm in}\). It applies to the analytic error after subtracting the retained polynomial.

A symbolic check verifies the removable EF powers. The constant coefficients in both the \(vr\) and \(rr\) numerators vanish identically. The \(rr\) linear coefficient does **not** vanish for an arbitrary horizon slope. It vanishes after imposing the exact regularity relation \(n'(0)=n(0)(k^2-5+6/(1+k^2))/(1+2\Omega)\). A check that an interval coefficient merely contains zero would be insufficient to justify removal of this power. [SymPy calculation](horizon_contacts.py) · [Archived output](horizon_contacts.txt)

The helper checks the digests of the two GOAL-015 files it imports directly. The replay must also pin the checkout tree because those files import inherited GOAL-011 modules. This audit checked the formulas and contacts without running the full interval geometry. A geometry enclosure alone does not certify any sampled candidate current or strong residual.

## Reproduction

From the pinned checkout at `research/sav` run

```sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-017/verification/template_boxes.py
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-017/verification/horizon_contacts.py
```

The archived run used Python 3.14.8, python-flint 0.9.0 and 192-bit Arb intervals. The residue and exponent decimals are read directly into Arb. The analytic controls require no numerical evolution.

To replay the non-directed pilot quadrature, generate the fine NPZ as specified in the computational track and pass its path to

```sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-017/verification/candidate_replay.py "$candidate_npz"
```

This run used Python 3.14.8, NumPy 2.4.6 and SciPy 1.18.0.
