# Independent review of the compact interference certificate

## Inputs and independence

I reconstructed the cutoff, graph and flux identities from the ordered density and wave equations, and constructed the terminal-layer obstruction before receiving any new investigator's derivation. The charter, GOAL-014 results, and the substance of the earlier independent review were already visible. This was therefore an independent derivation with predecessor exposure, not a blind repeat of the earlier reviews. The algebra uses the inherited exact-curl master, Einstein frame, axial canonical data, GL coefficient conventions and physical cuts. It does not independently validate the GL eigenfunction or derive the full 5D source hierarchy from the field equations.

| Read-only input | Exact provenance |
| --- | --- |
| Ordered source hierarchy and graph | [GOAL-013 SOURCE-LEDGER](https://github.com/inaeyk/GL-with-AI/blob/1e8e628f68af4cab174fcc44b81af4748a22115c/research/sav/GLR-GOAL-013/SOURCE-LEDGER.md), [FINITE-CUT-BOUNDS](https://github.com/inaeyk/GL-with-AI/blob/1e8e628f68af4cab174fcc44b81af4748a22115c/research/sav/GLR-GOAL-013/FINITE-CUT-BOUNDS.md) |
| Physical density and parameter definitions | [GOAL-014 CURRENTS-AND-DATA](https://github.com/inaeyk/GL-with-AI/blob/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014/CURRENTS-AND-DATA.md), (1), (4)–(6) |
| Prior numeric gates and adverse controls | [GOAL-014 Work audit](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/AUDIT.md), [compact audit](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/C-COMPACT.md) |

## Sharp physical cut and cap contact

Use $u=t-x$, $v=t+x$, $r_0=1$ in the strip and $\Sigma=\{v=b(u)\}$, where $p=T'(x)=-1/r$ and $b'=-(1+p)/(1-p)$. Let the return $w=w_2$ obey
\[
 -4w_{uv}-Vw=-F_u-G_v+H,\qquad
 F=m(P_e-Q_e),\quad G=m(P_e+Q_e),\quad m=2r/\lambda.
\]
The canonical graph gives $w=0$, $w_x=-p w_t$ and $(1-p^2)w_t=m(P_e-pQ_e)$. Since $w_u=(1+p)w_t/2$, one obtains $4w_u=G-b'F$ on $\Sigma$. The factor $(1+p)/(1-p)$ follows from the differential relation $(du,dv)=((p-1),(p+1))\,dx$. Both facts matter even when independent upper graph data vanish.

Let $g=N_{01}$ and $Z$ satisfy $4Z_{uv}=-VZ$, $Z(d,v)=g(d)$, $Z(\cdot,c)=h$. In the compact strip $D_c=\{a\le u\le d,\ b(u)\le v\le c\}$ use the divergence identity
\[
 \partial_v(4Zw_u)-\partial_u(4Z_vw)=Z(F_u+G_v-H).
\]
The outgoing $u=d$ boundary on the left vanishes by $Z_v(d,v)=0$. The lower moving endpoint of $\int ZF_u\,dv$ contributes $+b'ZF|_\Sigma$, and $\int ZG_v\,dv$ contributes $-ZG|_\Sigma$. Both are canceled by the canonical graph term $-4Zw_u|_\Sigma$ on the left. The front $u=a$ is flat. The finite-cap identity is
\[
 4\!\int_a^d h(u)w_u(u,c)\,du
 =g(d)\!\int_{b(d)}^c F(d,v)\,dv +\int_a^d h G(u,c)\,du
 -\!\iint_{D_c}(Z_uF+Z_vG+ZH)\,du\,dv.
\]
To compare with infinity, the unforced exterior equation for $P=w_u$ has generator $P_v=-V\mathcal I P/4$, $\mathcal I P(u)=\int_a^uP(s)\,ds$. The backwards adjoint gives $Z_v=\mathcal I^*(VZ)/4$, hence constant $Z(d,v)$ and the same adjoint PDE. Its cap trace is $h=\mathcal U(\infty,c)^*g$. Applying the divergence identity on the exterior, with its own convergence and trace hypotheses, gives $-\int hG(\cdot,c)$ at the shared cap. The resulting compact term is the terminal $F$ term minus the displayed bulk pairing. This cancellation uses the same test $h$ and the physical $G$ at the two traces. Keeping one cap contact after joining the weak exterior changes the observable. Using a strong exterior formula keeps the compact $+\int hG$ and bounds the strong forced response separately.

The first manufactured calculation in [check_contacts.py](check_contacts.py) solves the free equation directly by integrating the flux row $4w_u=G+\partial_u\int_{b(u)}^v F(u,s)\,ds$. Its exterior-inclusive $4\mathcal P$ is $3/4$, while deleting the terminal ray gives $-1/4$ and deleting only the exterior cap contact gives $23/20$. A second free control with $p=-1/3$ tests the nonzero graph trace. It gives $4\mathcal P=3/2$ with the correct row, $11/8$ after setting the graph velocity to zero, and $5/4$ with the reversed tilt term. These controls are exact and do not use the physical GL mode.

## Flux, interfaces and the strong error estimate

For $-e_{tt}+e_{xx}-(K+\mathsf A^2-\mathsf A_x)e+\mathsf C e_t=R$, let $D_xe=e_x+\mathsf A e$, with $\mathsf A$ Hermitian, $K$ positive semidefinite and $\mathsf C$ skew-Hermitian, all stationary. Multiplication by $e_t^\dagger$, taking the real part, gives
\[
 E_t+J_x=-\Re(e_t^\dagger R),\quad
 E=\tfrac12(|e_t|^2+|D_xe|^2+e^\dagger Ke),\quad
 J=-\Re(e_t^\dagger D_xe).
\]
Pullback of $E\,dx-J\,dt$ to the graph gives $E-pJ$. When $e|_\Sigma=0$, its density is $(1-p^2)|e_t|^2/2$ per $dx$ and $(1+p)|e_t|^2/2$ per $|du|$. With nonzero graph displacement, use the full quadratic flux. On $v=\mathrm{const}$ the flux per $du$ is $(E+J)/2=|e_u-\mathsf A e/2|^2+e^\dagger Ke/4$. On $u=\mathrm{const}$ the flux per $dv$ is $(E-J)/2=|e_v+\mathsf A e/2|^2+e^\dagger Ke/4$. Each derivative is controlled in the direction tangent to that null side. Their sum is $e_t$, and $dt\,dx=du\,dv/2$. Testing the energy on all truncated diamonds, if $M$ bounds each null flux square root,
\[
 M^2\le E_{\rm in}+\tfrac12
       (\|R\|_{L^1_vL^2_u}+\|R\|_{L^1_uL^2_v})M .
\]
Here $E_{\rm in}$ includes graph and front fluxes. For the scalar, this controls $\|e_u(\cdot,c)\|_2$. To use the GOAL-014 cap arithmetic the strong error must also include the difference between the lift's cap derivative and the specific affine binary-ordinate candidate used for that arithmetic.

At a null seam $u=u_0$, $e=(u-u_0)_+\rho(v)$ gives common values of $e,e_v$, and the uncommuted operator has no delta. For stationary $V$, commuting with $\partial_t=\partial_u+\partial_v$ gives the extra $-4\rho'(v)\delta(u-u_0)$. Choosing $\rho=0$ near the graph localizes the failure away from initial data. Continuity of the uncommuted field and its tangential derivative is therefore insufficient for a commuted error. An uncommuted numerical candidate used solely as a weak current in the integrated pairing can be piecewise if the current integrals and designated traces are enclosed. A candidate used to build strong PDE residuals must include any distributional face contributions, or enforce the corresponding conormal compatibility. The whole-cell requirement also survives at finite resolution, since a nodally invisible interior perturbation can change both the continuum residual and cap news. The [earlier compact audit](https://github.com/inaeyk/GL-with-AI/blob/84c9824f826e3517c8a1ed74d96bea061ab88584/research/sav/verification/GLR-GOAL-014/C-COMPACT.md) supplies such a nodal counterexample with the true scalar potential.

## Terminal-ray obstruction for the uncommuted work estimate

Concentration in a thin layer next to the terminal cut gives a second failure, consistent with the scalar terminal-concentration example in GOAL-014 ADJOINT-AND-ENDPOINTS. The mixed error-field construction below tests the two work norms and null fluxes used in this dossier. Fix a smooth $\chi(v)$ compactly supported strictly between $b(d)$ and $c$, and let $\rho(s)$ be smooth on $s\ge0$, vanish for $s\ge1$, and have $\rho(0)=1$. Set $\Delta F_\delta=\rho((d-u)/\delta)\chi(v)$, $\Delta G=\Delta H=0$. For small $\delta$ the perturbation vanishes near the graph, front and cap. All finite-$p$ volume $L^p$ norms tend to zero, while the exact $F$ terminal integral is $\int\chi\,dv$. For the true smooth bounded RW adjoint on this compact support, its bulk change is $O(\delta)$ and the projection tends to $g(d)\int\chi\,dv/4$. Thus no bound that tends to zero with only these volume norms controls the sharp observable whenever $g(d)\ne0$.

There is an error-field version for the actual mixed operator with its bounded coefficients on the compact strip. Set
\[
 e_\delta=\delta\psi((d-u)/\delta)\chi(v)\,e_j,\qquad
 \psi(0)=0,\quad \psi'(0)=1,\quad \operatorname{supp}\psi\subset[0,1],
\]
where $e_j$ is a constant unit polarization. The incoming graph/front vanish. One has $e_\delta,e_{\delta,v}=O(\delta)$ and $e_{\delta,u}=O(1)$ in a strip of width $\delta$. The principal mixed derivative and all lower-order terms of its residual are bounded uniformly there. In particular, the two work norms of $R$ are $O(\sqrt\delta)$ and $O(\delta)$, and the uncommuted null flux square-root supremum tends to zero. However $e_{\delta,u}(d,v)=-\chi(v)e_j$. The tangential derivative and field on $u=d$ vanish. This rules out inferring a bound for the transverse trace from the uncommuted flux theorem alone. The script reproduces an exact polynomial scaling on a rectangular strip, $\|(e_\delta)_u(d,\cdot)\|_2=1$ and $\|(e_\delta)_u\|_2^2=3\delta/35$.

For the mixed feedback in the physical $F$, (1) and (4) of CURRENTS-AND-DATA give the coefficient of $\Re(U_u-ik r^4A_{1z,u})$ as
\[
 \frac{m\lambda(H_{tt}+2H_{tx}+H_{xx})}{fD}.
\]
This follows by taking row $x$ minus row $t$ of $\widetilde M_1$ and inserting the opposite $u$-derivative signs in $A_{1t},A_{1x}$. Wherever this coefficient is nonzero the error construction can be supported there, with a choice of $\chi$ yielding a nonzero signed terminal integral. The example concerns residuals of approximate solutions, without varying the exact benchmark. An estimator using a directed bound on the terminal residual, or a commuted estimate with its trace and seam terms, falls outside this obstruction. In particular $P=e_u$ obeys along $u=d$
\[
 P_v=\mathsf C P/4+(\mathsf C e_v-(K+\mathsf A^2-\mathsf A_x)e-R)/4.
\]
The skew propagator is unitary. A signed weighted terminal residual and the graph trace of $P$ can therefore repair the missing estimate without using the two bulk work norms to bound $R(d,\cdot)$.

## Targeted check of the exact polarization rotation

I received the new rotation, first-order identities and optical square from the causal investigator after completing the previous derivations. The checks in this section are post-exposure reconstructions and share the pinned mixed equations. Set $Y=(y,\Im z_c)$ for the real parity sector, $J_0=\bigl(\begin{smallmatrix}0&-1\\1&0\end{smallmatrix}\bigr)$, $\theta=\arctan(kr/\sqrt\lambda)$, $R_\theta=\exp(\theta J_0)$, $X=R_\theta^\mathsf T Y$ and $L=\theta_vJ_0$. Then $C\sigma_1$ in the complex variables becomes $4L$ in the real variables, and $\theta_u=-\theta_v$, since $r=r((v-u)/2)$. Directly differentiating $Y=R_\theta X$ gives
\[
 R_\theta^\mathsf T Y_{uv}=X_{uv}+LX_u-LX_v-(L_v+L^2)X.
\]
Write $\widetilde W=R_\theta^\mathsf T(K+\mathsf A^2-\mathsf A_x)R_\theta$, $\widetilde S=R_\theta^\mathsf T S_{\rm real}$. With $P=X_u-2LX$ and $Q=X_v$ the checked identities are
\[
 P_v=-(\widetilde W/4+L_v-L^2)X-\widetilde S/4,\qquad
 Q_u-2LQ=-(\widetilde W/4-L_v-L^2)X-\widetilde S/4.
\]
The opposite $L_v$ signs follow by differentiating $2LX$ when forming $P_v$.

For the free leading field $B(u)$, let $\ell=\widetilde M_1[x,:]-\widetilde M_1[t,:]$ and $q_0=\sqrt\lambda(\ell_x-\ell_t)/4$. The $B''$ coefficient before rotation comes from $J^x-J^t$ and is $\lambda(\ell_x-\ell_t)(1,kr/\sqrt\lambda)/(4\sqrt{d_k})$. Its rotated row is $(q_0,0)$. In the physical feedback $F$, the coefficient of $P$ is $m\lambda(\ell_t-\ell_x)[\sqrt D,kr^4/\sqrt W]R_\theta/D=(-8q_0/\lambda,0)$. The source $+k$ uses the cosine half and the returning mean uses the unhalved GL coefficient times the real $+k$ field. Under the optical truncation with zero graph $P$, $P_v=-q_0B''(1,0)^\mathsf T/4$ and the signed ray primitive is
\[
 \int_{b(u)}^c F_{\rm opt}\,dv
   =\frac{B''}{\lambda}\left(\int_{b(u)}^c q_0\,dv\right)^2 .
\]
This principal term alone omits the induced graph data, remaining mixed source, direct term, fixed-mass mean, and propagation effects. The exact row and the optical coefficient pass the algebra check. The physical overlap needs a directed bound on the omitted contributions.


## Audit of the directed weak candidate

I received the implementation and provisional interval after the earlier reconstruction. This is a non-blind audit of [weak_candidate.py](../checks/weak_candidate.py), including independent reconstruction of its range and error logic and a replay of the same implementation. The reconstructed current is exact for the bound digest of the inherited sampled candidate. In the rectangle the ray primitive is affine between the exact $u$ nodes, because the candidate uses affine $v$ panels and linear interpolation of their endpoint rows. The wedge primitive is
\[
 A_w(u)=\tfrac12(R-b(u))(F_\Sigma(u)+F_R(u)).
\]
The code's derivative has both terms, including $-b'(u)(F_\Sigma+F_R)/2$. The primitive is continuous at $u$ nodes and the front primitive vanishes. Integrating its derivative against $g$ is exactly the sum of the terminal pairing and the bulk term with $-g'$. It does not drop the terminal pairing or depend on a sampled derivative of the physical test.

For the global transported test $Z$, the exact identity is $Z_u=g'+\int_v^\infty(V/4)Z$. The first correction upon replacing $Z$ by $g$ is $J(r(u,v))g(u)$, with $J(r)=3/r-3/(4r^2)$ from integrating the full ray from $v$ to infinity. Substituting a finite-cap primitive here would change the result. Since $J$ is decreasing, the two rectangle corners used by the implementation enclose the whole-cell range, and the wedge range runs from its upper-radius corner to its graph. The splitting into a midpoint signed moment and radius times an absolute moment handles sign changes of $F$ without choosing a sign within a cell. The baseline test enclosure uses $g=B'-J(b)B+\Delta$, $|\Delta|\le M(C-1)B$, directly from the early scalar bound $|w_0-B|\le(C-1)B$ in GOAL-014 PHYSICAL-PROJECTION. The constants $M=1.57$, $C=(1-M/522)^{-1}$, the exact bump normalizer box, and the directed $L^1$ and endpoint data agree with the pinned evidence.

For the dual remainder define $q(u)=\sup_{v\ge b(u)}|Z(u,v)|$ and $(\mathcal I^*q)(u)=\int_u^d q(s)\,ds$. The inequality $q\le |g|+M\mathcal I^*q$ holds because $b$ decreases. Its ordered majorant gives
\[
 \left\|M\mathcal I^*q\right\|_1\le (e^{M(d-a)}-1)\|g\|_1.
\]
This bounds $\|\sup_v|Z-g|\|_1$. The same bound applies to $\|Z_v\|_{L^1(D_c)}$ after interchanging the $v,s$ integrations in $Z_v=\int_u^d(V(s,v)/4)Z(s,v)\,ds$ and using $b(s)\le b(u)$. The residual in $Z_u-g'-Jg$ is bounded in the raywise supremum by $M\sup_v|Z-g|$. Hence the code's remainder radius follows after multiplying the separate absolute $F,H,G$ candidate norms. No zero trace at the cap is assumed for the adjoint or the candidate $G$. The physical shared-cap contact has already been canceled at the level of the full weak identity.

The bounded replay used Python 3.14.8, NumPy 2.4.6, python-flint 0.9.0, 160 bits and 32 $u$ subcells. Run the following from the research directory with the pinned 192 by 720 pilot dump.

~~~sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 \
  GLR-GOAL-015/checks/weak_candidate.py "$candidate" --subcells 32 --bits 160
~~~

The output rounded outward encloses the candidate by $[7.81166934,7.82206151]$ and its terminal contribution by $[19.94037034,19.94037144]$. Subtracting that contribution gives a strictly negative candidate interval $[-12.12870210,-12.11830883]$. The implementation checked all five digests. The replay used approximately 0.38 user CPU seconds and shares the candidate and arithmetic with the original run. The mathematical audit supports the interval for this candidate evaluated with the true baseline test and transported dual. It leaves a physical compact current mismatch and the already separately controlled physical weak exterior to add. In particular a graph sample in the numerical candidate supplies neither a validated physical graph jet nor a uniform physical lower-field error.

## Audit of the one-commuted-current cap estimate

I reviewed [STRONG-CAP](../observables/STRONG-CAP.md) after its derivation was available. Independently adding and subtracting $F_v$ in the ray identity yields
\[
 4w_u(u,c)=(G-F)(u,c)+(1-b'(u))F(u,b(u))
     +\int_{b(u)}^c(F_t-H-Vw)(u,v)\,dv .
\]
In particular the factor $1-b'$ and the cap $G$ trace both survive. The stationary $F_t$ here is the interior derivative. Differentiating a zero extension or a discontinuous current lift generates contact measures that the pointwise interior formula does not include.

The compact Volterra integral acts on the zero extensions, since for each fixed $v$ the physical $u$ segment starts on the graph or the flat front. Using the exact $L^2$ primitive norm $\kappa=2(d-a)/\pi$, successive $v$ integrations are ordered. Their kernel bound is $(\kappa M)^j/j!$. At a fixed $v$, the term $\mathcal J\Delta F$ is bounded by $E_F$ and the $u$ primitives of $\Delta G$ and $\mathcal J\Delta H$ by $\kappa(E_G+E_H)$. This verifies the orientation $L^\infty_vL^2_u$ in Eq. (3). In Eq. (4), the scalar potential contribution is $\|\int V\Delta w\,dv\|_2/4\le M\|\Delta w\|_{L^\infty_vL^2_u}$. The graph factor satisfies $\sup(1-b')=26/21$, attained at $b(a)=13/8$. The stated inequality follows with the necessary candidate-to-reference readback error. It does not presume that an uncommuted numerical residual controls $F_t$.

The oscillatory counterexample in STRONG-CAP solves the actual scalar current equation by direct differentiation of its defined $F_n$. The incoming support is zero, $F_n$ and its cap/terminal traces are uniformly $O(n^{-1/2})$ on the compact set, and the cap derivative has $L^2$ norm $\sqrt n(\|\chi\|_2/\sqrt2+o(1))$. This proves the stated failure in the undifferentiated current class even with a small signed terminal weak trace. It does not impose a physical constraint on the fixed GL-source solution. A directed bound for the actual mixed $F_t$ and remaining graph, volume, cap and reference errors is required to use this strong estimate.

## Physical normalization and decision rules

For a unit real spherical harmonic and $X_{AB}=D_{(A}X_{B)}$, $\int X_AX^A=\ell(\ell+1)$. The divergence-free sphere vector identity is $D^AX_{AB}=-(\ell(\ell+1)-2)X_B/2$. Integration by parts gives $\int X_{AB}X^{AB}=12$ at $\ell=2$. With the inherited four-dimensional Einstein-frame Bondi flux convention $1/(32\pi G_4)$, the cross term in the two ordered news coefficients gives $3r_0\mathcal P/(4\pi G_4)$ at matched physical cuts. This normalization converts an ordered sign if certified. Finite-amplitude control remains a separate theorem.

| Check | Acceptance requirement |
| --- | --- |
| Weak early sign | Directed compact candidate with outward quadrature. Add baseline/test and adjoint errors, coefficient and lower-field errors, and the weak exterior. Enclose the separately weighted signed terminal $F$ error, with $g(d)$ or the correct transported test at $d$. |
| Strong early sign | Verify the cap ordinate and abscissa identity, both polarizations, all residuals against the true coupled source, incoming graph/front and seam defects, and the trace discrepancy of the conforming lift. Close the cap-to-infinity transfer with its separate strong exterior bound. |
| Full packet sign | Independently enclose the larger domain and its leading test, endpoint, return, and exterior. The earlier window has no implication for the larger sign without more bounds. |
| Physical scope | Keep the fixed-infinity mean (including mass and lapse constants), direct and mixed channels, conjugacy factors, canonical graph and matched physical cuts. Nodal agreement, empirical convergence, and manufactured residual controls check separate assertions. |

For the specified early target allocation, exact arithmetic gives a conditional margin $7.80-3.8-37(0.1031368248)=0.1839374824$. The inherited strong directed cap arithmetic would leave margins $2.3230482$ at $E_c=400$ and $0.00286661$ at $E_c=570$. These margins require the stated directed physical errors and the corresponding candidate identities.

From `research/sav`, run `PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-015/verification/check_contacts.py`. [Archived output](check_contacts.txt) uses Python 3.14.8 and SymPy 1.14.0. The controls use exact rational or symbolic arithmetic and fixed manufactured data.
