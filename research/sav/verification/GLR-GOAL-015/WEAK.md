# Independent audit of the two weak signs

This review reconstructs the characteristic and dual identities from the pinned ordered wave equations. It uses the pinned GR/KK source reduction, spectral and mean coefficients, baseline norm and endpoint bounds, and exterior current bounds as shared inputs. The source report and its sign claims were visible before this review because the completed dossier was the audit target. The derivations below were checked algebraically and the displayed margins were independently recomputed from outward-rounded constituents. The interval-heavy physical coefficient integrations were outside this track's checks.

## Sharp-window identity

Use $W=V/4$, the physical domain $D_c=\{a\le u\le d,\ b(u)\le v\le c\}$ and the cut test $g=N_{01}$. The exact current equation is $4w_{uv}+4Ww=F_u+G_v-H$ and the upper graph conditions are $w|_\Sigma=0$ and $4w_u|_\Sigma=G-b'F$. The backward dual satisfies $Z_{uv}+WZ=0$, $Z(u,\infty)=g(u)$, and $Z(d,v)=g(d)$. Since $b'\!<0$, the future rectangle from every point in the strip stays above the graph. Integrating

$$
4\partial_v(Zw_u)-4\partial_u(wZ_v)=Z(F_u+G_v-H)
$$

leaves the lower-face terms $-4Zw_u$ and $Z(b'F-G)$, which agree. The front vanishes by the flat launch. At $u=d$, the $wZ_v$ term vanishes and the integration by parts of $F_u$ leaves $g(d)\int_{b(d)}^c F(d,v)\,dv$. The finite-cap $G$ contact $+\int Z(u,c)G(u,c)\,du$ cancels the negative contact from the weak exterior integration using the same $Z$. The $G$ trace at infinity is zero under the inherited exterior current bound. Hence

$$
4\mathcal P=g(d)\int_{b(d)}^c F(d,v)\,dv
-\int_{D_c}(Z_uF+Z_vG+ZH)\,du\,dv+4\mathcal P_{\rm ext}.
$$

An exact negative control uses $a=0,d=1,c=2,b=1-u$, $g(u)=u$, $W=G=H=0$, $w=u^2(v-b)^2/4$, and $F=4w_v$. The graph displacement and normal derivative vanish, the front gives zero contact, and $\int_b^c F\,dv=u^2(1+u)^2$. The direct and adjoint values agree at $4\mathcal P=89/30$. Omitting the terminal value $+4$ changes the result to $-31/30$. This algebraic fixture is independent of the physical GL coefficients. [Pinned adjoint](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/analysis/ADJOINT.md#L9-L27)

## Mixed rotation and early route

Write $\bar V=O^{\mathsf T}\mathsf VO$, $Y=OX$, $O_v=OL$, $O_u=-OL$, with $L$ skew. The original mixed equation implies

$$
X_{uv}=2LX_v-(\bar V/4-L_v-L^2)X-O^{\mathsf T}S/4 .
$$

Thus for $P=X_u-2LX$, $Q=X_v$, one gets $P_v=-(\bar V/4+L_v-L^2)X-O^{\mathsf T}S/4$ and $Q_u=2LQ-(\bar V/4-L_v-L^2)X-O^{\mathsf T}S/4$. The opposite $L_v$ signs are essential. The characteristic integrations in $u$ have unitary propagators. The mixed source row on $B''$ and current row on $P$ rotate to $q e_1$ and $-8q e_1^{\mathsf T}/\lambda$. In particular the latter sign follows from $A_{1t}$ and $A_{1x}$ having opposite rows on $Y_u$, so the contraction with the first mass row difference is proportional to $\ell_t-\ell_x$. Here the actual $+k$ insertion fixes the factor in $q$. If $A=\int_b^v q\,ds$, the leading $P$ is $-Ae_1B''/4$ and its $F$ primitive is exactly $A^2 B''/\lambda$. This square does not assume a sign of $q$ or $A$.

In the early strip, $B''\ge380 B'\ge0$. A slice bound $\Pi(v)$ for $|P|/B''$ controls $|X|$ by $\Pi(v)B'$, because $X$ vanishes on the incoming graph or front and the integral of $B''$ is at most $B'(u)$. The induced graph value of $P$ remains in its outgoing ray integral. Consequently the compact error in $P$ is bounded by the graph error plus $\int(s_E/4+m\Pi/380)\,dv$, with $s_E$ the full source defect weight and $m$ a common matrix bound. A slab supremum follows by one-dimensional Grönwall and applies equally at $u=d$. A bound only on interior residual samples would not give that terminal control. The terminal phase transport follows from moving the lower endpoint of $A$ and $q_u+q_v=\Omega q$, which gives $A_u=\Omega A-q+(1-b')q|_\Sigma$. The narrowed phase bracket uses the independently pinned GOAL-011 launch value with a widened asymptotic tail and retains the graph drive.

The decisive terms, in units of $4\mathcal P$, are a terminal value above $70.17398932866$, a bulk subtraction below $51.33607717719$, and adverse terminal, bulk and remaining dual errors below $6.86913231350$, $4.25246355355$ and $0.88801490227$. Independent rational subtraction with the physical exterior term gives $\mathcal P_{\rm early}>1.70707534$. The final weak archive reports the outward interval $[1.70707,13.47110]$, and the source also has a separate tighter strong enclosure. The proof uses neither the approximate $+7.8181$ pilot center nor differentiation of sampled mixed fields. [Pinned early proof](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/analysis/ADJOINT.md#L29-L128) · [Final early arithmetic](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/analysis/early_optical_certificate.txt)

## Signed full-packet route

The ratios of early bump derivatives fail on the full packet. The primitive on each fixed-$v$ interval with zero extension below the graph has operator norm at most $2D/\pi$ in $L^2_u$ and $D$ in $L^\infty_u$, with $D=d-a$. The first bound follows from the mixed Dirichlet/Neumann Poincaré inequality applied to the primitive's norm and the second by integration. The skew integrating factor preserves the vector norms pointwise. Extending the scalar integrals by zero below the graph and applying Minkowski before Grönwall gives $\sup_v\|w_0(\cdot,v)\|_p\le e^{k_pM}\|B\|_p$, for $M\ge\int \sup_u W\,dv$. For the adjoint a backward pointwise Volterra majorant with fixed-ray mass $M$ has $L^2_u$ operator norm at most $e^{MD}$. Then $\|Z_u-g'\|_2\le M e^{MD}\|g\|_2$, and $\|Z_v(\cdot,v)\|_2\le(2D/\pi)e^{MD}\|g\|_2\sup_u W(u,v)$. The resulting $F,G,H$ error bounds use the actual $L^1_vL^2_u$ current envelopes. The terminal term still requires its own supremum envelope.

For the packet, $W=3(r-1)(2r-1)/(4r^4)$. I independently verified $W\le1/4$ and $|\partial_{u,v}\log W|\le1/4$ by positive coefficients after translation of the relevant rational numerators to $r=1.596+s$. Here $\partial_v\log W=(-4r^2+9r-4)/(2r^2(2r-1))$ and $\partial_u\log W=-\partial_v\log W$. The last-ray radius $s=r(d,v)$ is a lower bound for all radii at that $v$. For $s\ge2$ the supremum is the last-ray value by monotonicity, and for $1.596\le s\le2$ it is bounded by $e^{D/4}$ times that value. Directed arithmetic gives a rectangular mass below $1.593566926<1.60$, including the exterior.

For $J=\int_b^v W\,ds$, differentiate $w_{0,u}=B'-\int_b^v Ww_0\,ds$ to get

$$
w_{0,uu}-(B''-JB')=b'W|_\Sigma B-\int_b^v\{W_u w_0+W(w_{0,u}-B')\}\,ds .
$$

This supplies the signed mixed source coefficient $S_1=O^{\mathsf T}S^{\rm coeff}_u-qJe_1$ on $B'$. The lower $v$-derivative graph term likewise enters the bound on $w_{0,vv}$ because the fixed-$v$ primitive's lower endpoint is $\alpha(v)=b^{-1}(v)$ in the wedge. Use the references $X_0=-Ae_1B'/4$, $Q_0=-qe_1B'/4$, $P_0=-Ae_1B''/4+CB'$, with $C_v=-M_+(-Ae_1/4)-S_1/4$. Starting $C$ from the physical graph's coefficient of $B'$ leaves the graph's $B$ contribution to the $P$ error. The residual in $X$ has forcing $P-P_0+[C+(A_ue_1-2ALe_1)/4]B'$, and the residual in $Q$ keeps the full graph mismatch on the wedge. Grönwall on a slab bounds the whole slice errors, without differentiation of a sampled candidate. Optional signed increments of $A,C,C_F$ cover cells that straddle the graph.

The exact split $\int_b^c F\,dv=\phi B''+C_F B'+e_F$ has $\phi=A(u,c)^2/\lambda$. Since $B'(a)=B'(d)=0$, the integral $\int B'B''=0$ cancels the midpoint of the entire $C_F$ interval in its $B''$ pairing, and likewise the midpoint of the entire $\phi J_\infty$ interval in the leading test correction. The scattered $g(d)$ remains positive even though the two reference terms vanish at $d$. The dedicated term $|g(d)|\sup|e_F|$ is required. The inherited $|g(d)|<0.009755$ and $\|g\|_2<1.000930$ are conservative outward roundings of the GOAL-014 and GOAL-013 archives, respectively.

In units of $4\mathcal P$, the leading negative magnitude exceeds $4393.82672796645$ and the five adverse terms are below $44.02052046911$, $2078.31965086407$, $0.44414526251$, $616.13374721694$, and $854.54174864039$. The resulting rational upper bound, including the separate exterior bound, is $\mathcal P_{\rm packet}<-200.09172887$. This confirms the strict upper margin using archived constituents and independently checked budget signs. The full archived interval rounds outward to $[-2758.258,-200.091]$. The first absolute-envelope attempt spans zero, so its negative center is insufficient. The common exterior estimate independently reproduces bounds below $6.948\times10^{-16}$ and $1.63462\times10^{-14}$ from the inherited tail norms. [Pinned packet proof](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/observables/PACKET.md) · [Signed packet arithmetic](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/observables/packet_signed.txt) · [Exterior inputs](https://github.com/inaeyk/GL-with-AI/blob/0b4bfcbb978b52626fb0816874df06055703c195/research/sav/GLR-GOAL-014/ADJOINT-AND-ENDPOINTS.md#L61-L80)

## Qualification and disposition

The signed packet implementation uses `wedge = v0 <= R`. An overlapping Arb comparison can return false when the left endpoint enclosure straddles $R=b(a)$. Every pinned cell was independently checked at 160 bits. Exactly four cells have $v_0<R$, none are ambiguous and $\min|v_0-R|>0.00466338$. This is safe at the audited mesh. Reusing the method on another mesh requires a disjointness assertion or the conservative `not v0 > R` test used by the early implementation. [Pinned wedge line](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/observables/packet_signed.py#L94) · [Early comparison](https://github.com/inaeyk/GL-with-AI/blob/65a4af854a25ca02e40f31b00fe5ff789935bdc4/research/sav/GLR-GOAL-015/analysis/early_optical_certificate.py#L218)

I found no sign-invalidating error in the weak characteristic or dual arguments under the pinned physical inputs. The two strict endpoint signs force a cumulative zero by continuity on the intervening retarded-time interval. This reasoning leaves its location and multiplicity undetermined. The weak overlap is a single linear test of the returning news and leaves the strong packet norm open. A smooth packet-local perturbation of the returning news orthogonal to $g$ can have arbitrarily large norm at fixed overlap, so the weak result alone gives no bound on strong waveform error. No independent five-dimensional evolution, strong packet replay or finite-amplitude remainder enters this audit.
