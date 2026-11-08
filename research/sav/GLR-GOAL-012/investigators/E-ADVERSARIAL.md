# E. Independent reconstruction and adversarial review

## Input exposure and status

Initial reconstruction used the fixed GOAL-012 contract, scoped agent instructions, the personal multi-agent guidance and its pinned policy, and the following GOAL-011 files read with `git show eb439228601a649c658ce719b503ff0bdc37a83f:research/sav/GLR-GOAL-011/<file>`. The foundational inputs were [FINITE-DATA.md](https://github.com/inaeyk/GL-with-AI/blob/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011/FINITE-DATA.md), [HIERARCHY.md](https://github.com/inaeyk/GL-with-AI/blob/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011/HIERARCHY.md), and [GREEN-FUNCTION.md](https://github.com/inaeyk/GL-with-AI/blob/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011/GREEN-FUNCTION.md). The exact curl operator and Cauchy pair, GOAL-011 retarded signs, and pole and residue given in the new contract were shared inputs. In particular GREEN-FUNCTION already displays the classical source-plus-Cauchy numerator, so agreement with that expression is a non-blind sign and normalization check. The distributional contact cancellation, endpoint tests and counterexamples below were reconstructed before exposure to any GOAL-012 A–D or coordinator derivation, code or intermediate result. No inherited pole or residue enclosure was independently reproduced here. No manuscript or production calculation was accessed.

## Returning slice contact

Write \(x=r_*\), \(T(x)=r(x)-x\), \(\tau=t-T(x)\), \(p(x)=\Psi_2|_\Sigma\), \(q(x)=\partial_\tau\Psi_2|_\Sigma\), \(a=r_0/r\), \(f=1-a\), and \(H=\mathbf 1_{\tau\ge0}\). A prime on \(T,p\) denotes a graph derivative in \(x\). The operator in \((\tau,x)\) is
\[
 L=\partial_x^2-2T'\partial_x\partial_\tau-T''\partial_\tau
   -(1-T'^2)\partial_\tau^2-V .
\]
Applying it to the zero extension gives the distribution identity
\[
 L(H\Psi_2)=HS-\big[(1-T'^2)q+2T'p'+T''p\big]\delta(\tau)
   -(1-T'^2)p\,\delta'(\tau).
\]
In particular at \(T'=0\) the stationary radial right side contains \(-q-sp\) against \(e^{-st}\). At general \(T'\), testing against \(w=e^{-st}h(s,x)\) and integrating by parts in \(x\) gives \(-\int [wD_\Sigma\Psi_2-\Psi_2D_\Sigma w]\,dx\). Here \(D_\Sigma=\partial_t+T'\partial_x|_t\). Compact support or vanishing endpoint terms suffice for the integration by parts. This reproduces the inherited sign and graph Jacobian.

An exact cancellation is available in the pinned preparation. Use
\[
 P=r^{-2}\mathcal D^r,\qquad Q=fr^{-2}\mathcal D^t,\qquad
 K=\lambda f r^{-4}\mathcal C,\qquad m=2r/\lambda ,
 \qquad S=-m(\partial_tP+\partial_x Q+K).
\]
In these definitions \(\partial_x\) in \(S\) is taken at fixed \(t\). The source's classical zero extension is
\[
 HS=-m\big[\partial_t(HP)+\partial_x(HQ)+HK\big]
      +m(P_\Sigma-T'Q_\Sigma)\delta(\tau).
\]
For the exact returning curl \(p=0\). The two coordinate systems \((t,r)\), \((\tau,r)\) have Jacobian determinant one. Therefore the contravariant density obeys \(\mathcal D^t=\mathcal D^\tau+(T'/f)\mathcal D^r\). Using \(T'=-a\) and the pinned Cauchy pair yields
\[
 m(P_\Sigma-T'Q_\Sigma)
 =\frac{2}{\lambda r}\big[(1-a^2)\mathcal D^r+af\mathcal D^\tau\big]_\Sigma
 =(1-T'^2)q .
\]
The full returning distributional source is consequently
\[
 L(H\Psi_2)=-m\big[\partial_t(HP)+\partial_x(HQ)+HK\big].
\]
Equivalently, where the boundary terms vanish, the exact full numerator is
\[
 I(s)=\iint_{t\ge T(x)}
 \big[-s m w P+\partial_x(mw)Q-mwK\big]\,dt\,dx.
\]
A computation using classical derivatives of the source must retain the velocity Cauchy term. A computation using distributional derivatives of zero extensions already includes that term. These identities assume the fixed canonical pair and the exact returning curl. Switching curl variable, slice, or higher-order canonical data requires rederiving the cancellation. Distributional integration by parts first uses compact tests. Testing with the noncompact resolvent weight also requires endpoint and time estimates.

## Continuation failure tests

The following tests distinguish local endpoint estimates from analytic continuation of the physical transform.

- At the horizon, set \(v=t+x\). Then \(L=f(2\partial_v\partial_r+f\partial_r^2+f'\partial_r-V/f)\). Thus a regular exact scalar gives \(S=f\,\widetilde S(v,r)\) at bounded \(v\). If a continued left Jost branch is \(h=e^{sx}\widehat h\) with bounded regular \(\widehat h\), its pairing becomes \(e^{-sv}e^{2sx}\widehat h\,\widetilde S\,dv\,dr\). Since \(x=r_0\log(r-r_0)+O(1)\), a bounded nonvanishing endpoint integrand is absolutely integrable radially for \(\Re(sr_0)>-1/2\). This is a local test for the raw pairing and presupposes the \(v\) estimate or its correctly continued substitute. In particular, the Frobenius denominators of \(h/e^{sx}\) contain \(n(n+2sr_0)\), and shifted operators need their own tests. A zero or pole due to common rescaling of a Jost solution cannot by itself produce a physical singularity in \(I/W\).
- At the outer end a generic continued left solution contains a component with growth \(e^{-sx}\) for \(\Re s<0\). Taking only the outgoing \(e^{sx}\) component off the pole can remove a factor \(e^{-2sx}\) from an integrand written at fixed \(u\). A conditional test uses an outgoing unshifted massless radial tail \(e^{-(s-2\Omega)x}\) and a GL coefficient \(e^{-q_\infty r}\), both up to powers, where \(q_\infty=\sqrt{k^2+\Omega^2}\). A direct two-insertion outer pairing is then bounded by an exponential of rate \(-2q_\infty-2\Re(s-\Omega)\) in \(r\). A homogeneous outgoing massive term with \(\kappa=\sqrt{(s-\Omega)^2+k^2}\) gives the rate \(-q_\infty-\Re(s+\kappa)\) after one more GL insertion. Strict negativity suffices for these respective tails once the stated asymptotics and the continued branch are established. This test does not control the forced massive tail or near-threshold logarithmic and power factors.
- At fixed \(x\), multiplication by \(e^{j\Omega t}\) in the original common convergence half-plane shifts the transformed argument to \(s-j\Omega\). The candidate spectral features of the returning transform accordingly come from its own massless denominator at \(s\), from the mixed propagator at \(s-\Omega\), and from the input massless propagator at \(s-2\Omega\). For example the massive threshold square root has candidate points \(\Omega\pm ik\). A cut or pole of any factor requires a nonzero physical discontinuity or residue after applying sources, contractions, contacts and extraction. This shift identity alone establishes neither a physical abscissa nor a continuation domain.
- A source cutoff after \(u_{\rm c}\) in its own outgoing time leaves the returning waveform for \(u<u_{\rm c}\) unchanged by finite propagation. In the causal past of an observer at \((t,x)\), \(t'-y\le t-x\) follows from \(t-t'\ge |x-y|\). The cutoff can change the transform numerator and pole residue. Applying a cutoff for finite-time accuracy therefore does not authorize transferring the auxiliary residue to the physical waveform.

## Finite-time falsification tests and a benchmark

The first test uses a scalar causal waveform. For any \(D\) beyond the launch cut, take
\[
 C_D(u)=2\Re[A e^{s_qu}]\mathbf 1_{u\ge D},\qquad
 \widehat C_D(s)=
 \frac{A e^{-(s-s_q)D}}{s-s_q}
 +\frac{\overline A e^{-(s-\overline{s_q})D}}{s-\overline{s_q}} .
\]
The residue at \(s_q\) is \(A\) for every \(D\). On \(u<D\), the integral of the resolvent pair cannot be replaced by the residues, since the waveform vanishes identically there. Moving the inversion left incurs an exponential type that depends on \(D\). Smooth causal switches equal to one after a later finite time yield the same residues and failure. Their transform differs by an entire term. An arbitrary common compact smooth prompt can also be added without changing any residue, so prompt arrival alone does not imply the vanishing of the displaced contour. This is an explicit counterexample to the inference from meromorphy and residue location to finite-time contour completeness. It does not assert that the physical Schwarzschild trace has this particular exponential type.

For the second test choose a smooth causal waveform equal to \(2\Re[Ae^{s_q u}]\) after a transition, and let \(I\) be a compact interval in that region. Add \(-2\Re[Ae^{s_q u}]\) multiplied by a smooth compact window equal to one on a neighborhood of \(I\). The transformed addition is entire and leaves the pole residue unchanged. On \(I\) the resulting news is zero, hence the norm of the remainder after subtracting the formal pole news equals the norm of the pole news. Scaling the addition makes that ratio arbitrarily large on \(I\). This is a smooth counterexample to residue-only dominance, with the same reality partner and no extra singularities. The model changes the waveform and is a test of logical sufficiency, not a counterexample using the pinned physical source.

For the one-period test in the contract, \(B=s_q A_{21,q}\), \(s_q=\sigma+i\omega\), \(P_q=2\pi/|\omega|\), \(J=[U,U+P_q]\). Directly expanding the square of \(2\Re(Be^{s_q u})\) gives
\[
 \|N_q\|_{L^2(J)}^2
 \ge 2|B|^2e^{2\sigma U}
 \frac{e^{2\sigma P_q}-1}{2\sigma}
 \left(1-\frac{|\sigma|}{|s_q|}\right).
\]
The ratio of the absolute oscillatory integral to the nonoscillatory integral is \(|\sigma|/|s_q|\). At the printed approximate pole and the nearest point in the residue rectangle this gives \(P_q\simeq8.40736\,r_0\) and a lower-bound expression of approximately \(0.50965\,e^{-0.177924631 U/r_0}\) in units of \(\epsilon^2\eta\sqrt{r_0}\) when the residue is expressed in physical units and \(u\) has length units. The numbers are arithmetic checks from Python's `math` functions and the printed pole, without an interval for \(s_q\). The inequality is exact for the stated inputs and only applies to the pole waveform on a window where that pole term is justified.

One can control spectral truncation on the physical inversion line without a contour move. If \(e^{-\gamma u}N\in L^2(du)\), use the bilateral Laplace transform with a causal zero extension in \(u\) and \(\widehat N=s\widehat C\) in the distributional sense. Restricting the inverse to \(|\Im s|\le \Lambda\) incurs on \(J=[U,U+P_q]\) the bound
\[
 \|N-N_\Lambda\|_{L^2(J)}
 \le \frac{e^{\gamma(U+P_q)}}{\sqrt{2\pi}}
 \|\widehat N(\gamma+i\cdot)\|_{L^2(|\omega|>\Lambda)}
 \qquad(\gamma\ge0).
\]
A quadrature and source error can be bounded in the same weighted norm. This is a conditional Plancherel inequality, with no computed tail enclosure here. Using it for a comparison with a stipulated exponential on \(J\) still requires separately justifying the pole decomposition or calling that comparison algebraic.

## Initial disposition

The returning distributional contact cancellation and the two falsification tests are exact under the stated hypotheses. The continuation inequalities are conditional tests for candidate tails, and the pole norm number is an arithmetic benchmark. The physical analytic sheet, complete contour contribution, shifted features' excitation, nonzero finite-time interval of dominance and finite-width interference sign are open in this initial reconstruction.

## Non-blind review of the integrated routes

The coordinator requested this phase after the initial reconstruction. I inspected the A–D reports in this directory, the GOAL-012 scripts for shifted transfer, the two downstream denominators, the baseline launch kernel, the interval projection attempt, the news Gram norm and the finite-width reference flux. I also inspected their recorded outputs available at review, including the failed long-cell strong quadrature, plus the inherited Jost, mixed solution, projection, spectral validator, directed Riccati cell, and returning tail routines imported by those scripts. Those reports disclosed the candidate translated amplitude, the noncoincidence and packet bounds, the one-sided threshold theorem and the proposed residual estimates before this audit. This phase is a code and analytic review with disclosed conclusions, following the separate initial reconstruction above. I traced the displayed arithmetic and formulas without independently replaying the lengthy interval checks or solving the physical waveform.

### Translated overlap and continuation

For \(s_*=s_q+2\Omega\), the compact baseline numerator is \(-J_h\), with the graph reduction
\[
 J_h=\int_\Sigma e^{-s_qT}(1-T')a_\delta(h_x+s_q h)\,dx .
\]
The sign follows from integrating \(e^{-s_qT}(1-T')a_\delta' h\) by parts in \(x\), using \(du_\Sigma/dx=T'-1\). With the baseline Laurent part, the left profile is its horizon-normalized ingoing mode and the right profile is the outgoing mode scaled to match it. In the empirical implementation the division by \(\texttt{ray_scale}=h/g_+\) on the left does this. The interval kernel check evaluates \(e^{-s_qT}(h_x+s_q h)\) in real radial cells covering the packet. Its positive real component, multiplied by the nonnegative nonzero bump and \(1-T'>0\), makes \(J_h\ne0\). The two noncoincidences use \(s_*\) and \(s_q+\Omega\), not the inherited massive frequency \(s_q-\Omega\). The recorded interval mismatch and matching determinant exclude zero. This is an audit of the use of the pinned spectral validator and its saved bounds.

The shifted test solves \(m_{xx}+2s_*m_x-Vm=0\) for \(h=e^{s_*x}m\) forward from the ingoing match onto the ray. It therefore retains both far components of the genuine ingoing solution. In the interval attempt, the continued slope \(h_x/h-s_*\) solves the corresponding stable forward Riccati equation on that ray. The use of the same right ray geometry as the outgoing solve does not replace the ingoing data. The baseline right profile in the unit-transfer attempt is proportional to the left profile at the actual simple pole. That identity is required only at the true pole inside the spectral box. The contact coefficients evaluated entirely on the finite graph are holomorphic here through the two regular downstream inverses. They contribute to the full trace and do not add a Laurent numerator at \(s_*\).

All three projected couplings occur in the empirical and interval transfer formulations. The mixed return uses the massive solution with both polarizations, the direct term uses the quadratic GL operator, and the separate mean term uses the mass-matched Einstein geometry. To check the horizon sign independently, put \(P_e=r^{-2}\mathcal D^r_{\rm EF}\) and \(V_e=r^{-2}\mathcal D^v_{\rm EF}\). The two derivative terms of the strong source are \(mf[\partial_r(P_e-fV_e)-\partial_v V_e]\). Its projected radial integrand thus contains \(m h e^{s_*x}\partial_r(P_e-fV_e)_{\rm reg}\). Splitting the strong collar from the weak outer integral produces the lower primitive
\[
 -m h e^{s_*x}(P_e-fV_e)_{\rm reg}\big|_{\rm split}.
\]
This agrees with the subtraction in the empirical overlap. The far endpoint must also vanish or be bounded in a finite-radius comparison. The failed long-cell strong computation against a growing test is a useful numerical conditioning check. The short-cell result and symmetric offset samples agree with the weak computation at the reported resolution. They share the field solver and do not provide independent interval errors.

The interval attempt invokes fresh shifted profile transports, matching and absolute tails. I did not find a use of the old signed amplitude enclosure as a bound on the translated overlap. Its growing-test tail needs \(\texttt{rw.decay}+\texttt{bdecay}>\texttt{upper_growth}\) for the mean, with \(\texttt{bdecay}=2\texttt{gl.decay}-4/(\rho_0+2.5)\), in addition to the analogous checks for the mixed and direct terms. A run or explicit check should establish this positivity before claiming an absolute mean-tail cap. The attempt had no completed projection output at the time of this review. The observed translated value near \(-0.19366-0.42461i\) therefore remains empirical in this audit.

A's attractive Coulomb endpoint calculation checks on the specified propagating-side sector. Differentiating its Volterra kernel gives a factor bounded by \(|p(x)|/|p(y)|\), and the common \(O(r^{-2})\) remainder yields the integrable \(O(r^{-3/2})\) contraction weight. The ingoing horizon current is negative and the normalized threshold outgoing current is positive. Matching at the endpoint is consequently nonsingular in that sector. This does not control the evanescent lip where the radial square root can turn, a projected discontinuity, threshold accumulation, or a large-frequency contour estimate. The local corridor to the translated pole must be kept separate from any all-frequency contour deformation.

### Causal and flux estimates

The mixed equations in B admit the same distributional cancellation as the returning equation. In the first source row, the classical divergence minus its zero-extended distributional version contributes
\((pJ^t-J^x)\delta/\sqrt D=(1-p^2)y_t\delta\).
In the compact row it contributes \(ik L_c(J^t-pJ^x)\delta/\sqrt{W_c}=(1-p^2)z_{c,t}\delta\), using \(W_c=\lambda r^2 L_c\). The first-order coupling creates no additional contact since the positions vanish. These cancel the contacts in the left-hand side with the fixed exact-curl canonical data. The classical source formulation still requires both velocities.

The stationary factorization \(D_x^\dagger D_x=-\partial_x^2+\mathsf A^2-\mathsf A_x\) and anti-Hermitian time coupling give the stated energy identity. On \(v=\mathrm{const}\), \((\mathcal E+\mathcal J)/2=|\xi_u|^2+K|w|^2/4\), and the corresponding other null flux uses \(\xi_v\). The graph current is \(\mathcal E-T'\mathcal J\). The volume work factor \(1/2\) follows from \(dt\,dx=du\,dv/2\), and \(w_t=\xi_u+\xi_v\) gives both mixed residual norms in (B3). Integrating the cap equation gives the \(1/4\) factor in (B4) and the amplifier \(e^{K_c}\). These are conditional bounds pending smooth physical candidates and evaluated residual, graph and source-tail envelopes.

C's positive-line Wronskian estimate follows from the nonnegative derivative of \(|a|^2-|b|^2\) and the endpoint values \(1\) and \(|W/(2s)|^2\). Its finite-band square error uses the full positive-line transfer and the two-sided Fourier tail \(2K^2/(3B^3)\) before division by \(2\pi\). The cutoff is justified on the observed cuts by the causal ordering of outgoing time. Its auxiliary residue generally differs from the physical residue. These estimates do not enclose the physical low-frequency integrand or its weighted source norms by themselves.

D's geometric ray mass \(3/r-3/(4r^2)\), sign of \(J'(u)\), and short-strip Volterra bound follow from the baseline characteristic equation with the tilted launch. The saved table and algebra refer to the frozen optical reference against the exact scattered baseline. Its finite-width sign margins do not control the projection of the full returning residual. The \(\delta^{-2}\) factor follows by multiplying the scales of the two news coefficients and \(du=\delta\,dy\). The \(12/(16\pi G_4)\) cross factor follows from the real unit-normalized tensor harmonic and the factor of two in the square of the tensor news. The table gives no estimate of total tensor power, radion flux, or massive KK transport.

### Consequence of a verified slower pole

Let \(\sigma=\Re s_q<0\), \(P=2\pi/|\Im s_q|\), and subtract the news pole with any fixed causal activation before the test intervals. Put \(J_j=[U+jP,U+(j+1)P]\). If \(\|N_{\rm rest}\|_{2,J_j}\le\rho\|N_q\|_{2,J_j}\) for all sufficiently large \(j\) and any fixed finite \(\rho\), then \(\|N_q\|_{2,J_j}\le 2|s_q A_{21,q}|\sqrt P\,e^{\sigma(U+jP)}\). Cauchy–Schwarz yields, for \(a=\Re s>\sigma\),
\[
 \int_{J_j}e^{-a u}|N_{\rm rest}(u)|\,du
 \le 2\rho |s_q A_{21,q}|P\max(1,e^{-aP})e^{-(a-\sigma)(U+jP)} .
\]
The sum converges locally uniformly in \(a>\sigma\). The earlier compact time integral is entire for locally integrable causal news. Consequently the remainder transform is holomorphic for \(\Re s>\sigma\). If an actual translated pole with \(\Re s_*>\sigma\) can be reached by analytic continuation along a path lying in that same half-plane from the physical right domain, uniqueness contradicts the holomorphy. A merely formal singularity or a pole on a sheet accessible only by leaving that half-plane is insufficient. The horizontal corridor near the inherited fundamental frequency is the relevant sheet test. When all hypotheses hold, each fixed finite \(\rho\) is exceeded in arbitrarily late period bins. No identified finite period follows from the theorem.

The Gram norm of the fundamental uses the full fixed-clock residue rectangle. A translated-only ratio computed with the empirical trial amplitude is conditional on that amplitude. Even a certified translated coefficient would supply only the norm of that isolated trial contribution, since the other contributions to the same remainder can cancel it on a given interval. None of the finite-window routes reviewed here supplied an evaluated full physical news remainder or a finite-width physical returning flux residual. The reviewed evidence therefore does not decide any designated one-period dominance test or either physical interference sign at \(\delta=0.12\).

### Completed translated-pole certificate addendum

This final review was non-blind. I saw the coordinator's reported unit box, the completed unit and physical quotient logs, the earlier wide Cartesian quotient, and the completed `shifted_enclosure.py`. I traced its calls into the pinned signed projection, directed-vector and Riccati propagators, local horizon and far-tail estimates, refined outgoing normalization, and finite-packet moment routines. The following is an implementation and normalization audit with the same certified inputs, plus a separate rational modulus check. I did not rerun the full directed integration.

The outgoing mixed cells run from \(\rho=64\) to \(0\), while the new ingoing test runs from \(0\) to \(64\). Reversing the cell list and evaluating the test at \(1-t\) identifies the same radial point used by the signed right-leg quadrature. The test's logarithmic primitive accumulates in its outward direction. The ingoing factor \(e^{s_*x}\) yields the \(-2s_*\) slope term. The Riccati residual uses the actual directed complex step, including in the numerical-abscissa bound. The mixed affine envelope advances with \(-P\), and the state envelope advances in the reverse direction with \(P\). In the signed identity the match term has \(z_R-z_L+(P_L-P_R)^{\mathsf T}q\), since \(L_L-L_R=-(P_L-P_R)U\). The two physical endpoints are accounted for by the strong collar and affine far error. The completed code independently checks all three outer decay inequalities with the growing ingoing test, including the polynomial loss in the mean tail. The directed run gives absolute outer caps approximately \(0.01826\), \(1.22\times10^{-25}\), and \(2.85\times10^{-9}\) for the mixed, quadratic and mean terms respectively.

At the actual input pole, the unit horizon profile is proportional to \(g_q\), with \(h_q\) and \(g_q\) now denoting their match amplitudes. The compact input gives the normalized outgoing coefficient \(-J_h/W'(s_q)\), so the horizon multiplier is \((-J_h/W')(g_q/h_q)\). With \(D=p_L-p_R\), this also equals \(J_h/(h_q^2D')\). The receiving Wronskian is \(h_*g_*(p_R-p_L)\) at \(s_*\). These signs and scale factors yield \(A_*=I_{\rm unit}(-J_h/W')(g_q/h_q)/W(s_*)\) in the fixed-clock shear normalization. Its news residue is \(s_*A_*\). The finite-packet moment uses \(a_\delta=\sqrt{\delta}\,b\), the \(dx/dr\) and \(1-T'\) graph factors, and the unit kinetic bump normalization. The \(W'\) formula is evaluated for the actual certified root, so the term proportional to \(D(s_q)\) vanishes there.

The completed saved output gives \(I_{\rm unit}\in[-226.235,-197.215]+i[-776.201,-747.183]\) and \(A_*\in[-0.21850,-0.16857]+i[-0.44959,-0.39966]\) after outward rounding. As a separate check, taking rational square bounds on the three factor boxes in the loose-normalization run gives
\[
 772<|I_{\rm unit}|<810,\qquad
 0.000438<|(-J_h/W')(g_q/h_q)|<0.001214,\qquad
 0.996<|W(s_*)|<1.792.
\]
Exact Python `Fraction` arithmetic gives quotient bounds \(42267/224000\) and \(16389/16600\), hence \(0.188<|A_*|<0.988\) even from the loose factors. The earlier Cartesian enclosure contains zero because of interval overestimation in the complex arithmetic. This wider independent quotient check reuses the directed factor enclosures and does not independently verify their production. I found no blocking issue in the audited cell orientation, endpoint factors, or residue normalization. The completed nonzero result supplies the local pole hypothesis for the late-period theorem on the corridor described above. It supplies no finite-window enclosure of the full news remainder. [Directed code](../checks/shifted_enclosure.py) · [Completed output](../checks/shifted_enclosure.txt) · [Loose quotient](../checks/shifted_enclosure_failures.txt)
