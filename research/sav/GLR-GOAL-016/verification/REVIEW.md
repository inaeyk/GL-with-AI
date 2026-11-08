# Independent reconstruction and failure controls

This track received the problem and frozen source assumptions from the
coordinator. The inherited conversation and cited records exposed the
earlier Z certificate, qualitative order-of-limits arguments, and the
scope of the outstanding vacuum estimate. No new GOAL-016 derivation or
implementation was provided before the calculations in §§1–5. In particular,
the weighted null evolution below is a new reconstruction for the actual Z
source, as distinct from checking an earlier numerical transport result.
The audit of the coordinator's preparation theorem, when added, is a
separate exposed review. These calculations use no numerical evolution.
The inputs are the [GOAL-005 Z problem at
\(\texttt{61c7b5c2}\)](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/RADIATION.md),
[GOAL-008 transport at
\(\texttt{4aac5ad0}\)](https://github.com/inaeyk/GL-with-AI/blob/4aac5ad001a9d6c63efa86640ce05837282fb73f/research/sav/GLR-GOAL-008/TRANSPORT.md),
and the [GOAL-009 audit at
\(\texttt{844d1ccd}\)](https://github.com/inaeyk/GL-with-AI/blob/844d1ccdd5e507392f4ace02debf08da34d543d0/research/sav/verification/GLR-GOAL-009/REPORT.md).
The GOAL-005 mode tail, source, and directed news bounds are shared inputs.

## 1. Current, normalization and characteristic energy

The dimensionless quantities below use \(r_0=1\), \(x=r+\log(r-1)\),
\(f=1-1/r\), \(u=t-x\), \(v=t+x\), and the zero-Z graph
\(v=R(u)\) with \(g(R)=R-2x(R)\) and \(R=g^{-1}\).
The frozen EFT experiment evolves
\[
 P_\mu\psi=F,\quad P_\mu=\partial_t^2-\partial_x^2+V_\mu,\quad
 V_\mu=V_0+\mu^2 f,\quad V_0=f/r^3,\quad
 F=e^{s(u+v)/2}S(r),\qquad s=2\Omega r_0.
\]
The field \(\psi\) and its initial first derivatives are zero on the
graph. In physical units \(\phi=\epsilon^2 r_0\psi/r_{\rm phys}\)
at this order. From the action with canonical field
\(\phi/\sqrt{4\pi G_4}\), \(4\pi r_{\rm phys}^2(-T^r{}_t)\) gives the
outward power. The lapse factor in this coordinate-time Killing current is \(f\).
The experiment integrates in \(t\), the stationary Killing clock at infinity.
With \(b=f/r\), the dimensionless flux coefficient relative to
\(\epsilon^4 M_0\), \(M_0=r_0/(2G_4)\), is
\[
 \mathcal K_\mu(R_{\rm obs},I)
 =2\int_I \left[p^2-q^2+b\,\psi(p+q)\right]_{v=u+2x(R_{\rm obs})}du,
 \qquad p=\psi_u,\quad q=\psi_v. \tag{1}
\]
Indeed the power is \(-\epsilon^4\psi_t(\psi_x-b\psi)/G_4\) with
dimensionless derivatives, and \(dt_{\rm phys}=r_0\,du\) on the tube.
The term with \(b\) contributes the endpoint \(b[\psi^2]_{\partial I}\)
to (1). In particular, the same signed finite-cut experiment with
\(-\psi_t\psi_x\) instead would differ by this endpoint.

For the master density \(e=(\psi_t^2+\psi_x^2+V_\mu\psi^2)/2\)
and current \(j=-\psi_t\psi_x\), \(\partial_t e+\partial_x j=F\psi_t\).
The physical density with the same overall normalization is
\((\psi_t^2+(\psi_x-b\psi)^2+\mu^2 f\psi^2)/2\).
Since \(b^2+b_x=f/r^3\), the physical-minus-master density
and current are \(-\partial_x(b\psi^2)/2\) and
\(\partial_t(b\psi^2)/2\). Thus the two current one-forms
\(e\,dx-j\,dt\) differ by an exact boundary differential. The
incoming cap \(v=\mathrm{const}\) has outgoing master energy
density \(p^2+V_\mu\psi^2/4\) per \(du\), and the outgoing boundary
\(u=\mathrm{const}\) has \(q^2+V_\mu\psi^2/4\) per \(dv\).
The Jacobian in the work term \(dt\,dx\) is \(du\,dv/2\).
The graph contribution vanishes for Z. A homogeneous nonzero
preparation contributes \(e-jT'\) per \(dx\) on \(t=T(x)\).
The early wedge lies wholly outside the horizon. In a domain
including the future horizon a regular ingoing field has
nonnegative inward power \(\psi_v^2\), since \(V_\mu,b\to0\).
These identities allow negative **signed** flux on part of a tube
and include storage when the interval ends before all signal
has crossed.

## 2. Critical small-mass scaling for the actual Z source

**Theorem in the frozen scalar experiment.** Let \(A=R(u_1)\) with
\(u_1=-8\), and let \(I\subset(-\infty,u_1]\) be compact. For \(R_{\rm obs}
\to\infty\), \(\mu\to0\) and \(\mu^2 x(R_{\rm obs})\to\Lambda<\infty\), put
\[
 (J h)(u)=\int_{-\infty}^{u}h(w)\,dw,\qquad
 n_\Lambda=e^{-(\Lambda/2)J}n_Z,\qquad
 \Phi_\Lambda=J n_\Lambda .
 \tag{2}
\]
Then the tube traces of \(p,\psi,q\) converge uniformly on \(I\) to
\(n_\Lambda,\Phi_\Lambda,0\), respectively, and
\[
 \lim\mathcal K_\mu(R_{\rm obs},I)=2\int_I n_\Lambda^2\,du. \tag{3}
\]
The transform in (2) uses the actual earlier trace on the whole
early half-line. No future source completion is needed. In particular,
\(\Lambda=0\) recovers the GOAL-005 coefficient. Since \(x/r\to1\),
\(\mu^2 R_{\rm obs}\) gives the same scaling parameter in the
dimensionless radius convention.

**Proof and explicit budget.** Extend the solution by zero below the
graph, on \(u\le u_1,\ v\ge A\). The two zero initial jets remove
distributional contacts in \(4\psi_{uv}\). Replace \(F\) by
\(F_v(u)=\mathbf 1_{v\ge R(u)}F(u,v)\) in this rectangle. Write
\(X_\alpha\) for the weighted sup norm
\(\|h\|_\alpha=\sup_{u\le u_1}e^{-\alpha u}|h(u)|\).
For any \(s<\beta<2\sqrt{k^2+\Omega^2}\), the pinned mode tail gives
\(|S|\le C_\beta e^{-\beta x}\) on \(r\ge A\). Factors polynomial in \(r\)
can be absorbed by using strict \(\beta<2\sqrt{k^2+\Omega^2}\).
Take \(0<\alpha<(s+\beta)/2\). Then
\[
 \|F_v\|_\alpha\le C_\beta e^{[(s+\beta)/2-\alpha]u_1}
 e^{-(\beta-s)v/2},\qquad
 \|J\|_\alpha\le 1/\alpha .
\]
Here \(x\le 2r\) and \(v-u_1>0\) imply
\(V_0\le64/(v-u_1)^3\) and \(1-f\le4/(v-u_1)\), uniformly in \(u\).
Define the finite numbers
\[
 M_j=\int_A^\infty(v-A)^j\|F_v\|_\alpha\,dv,\qquad
 B_j=\int_A^\infty(v-A)^j\|V_0(\cdot,v)\|_\infty\,dv,\quad j=0,1,
\]
\[
 T(v)=\int_v^\infty(\|F_z\|_\alpha+K\|V_0(\cdot,z)\|_\infty)\,dz,\qquad
 L_f(v)=4\log\!\frac{v-u_1}{A-u_1}.
\]
Here \(K\) is defined below. The source moments may be bounded by
the preceding exponential, and the potential moments obey
\(B_0\le32/(A-u_1)^2,\ B_1\le32/(A-u_1)\).

Integration from \(u=-\infty\) gives
\(\psi_v=J(F_v-V_\mu\psi)/4\) and \(\psi=Jp\).
For \(\mu^2(v-A)\le L\), Gronwall in \(v\) gives the bounds
\[
 \|\psi_\mu(\cdot,z)\|_\alpha,\ \|\psi_0(\cdot,z)\|_\alpha\le
 K:=\frac{M_0}{4\alpha}\exp[(B_0+L)/(4\alpha)],
 \quad
 \|(\psi_\mu-\psi_0)(\cdot,z)\|_\alpha
 \le \frac{\mu^2(z-A)K}{4\alpha}e^{B_0/(4\alpha)}
 \tag{4}
\]
for \(A\le z\le v\). These follow by writing the difference equation
with \(V_0(\psi_\mu-\psi_0)\) and source \(-\mu^2 f\psi_\mu\).
Put \(d=\mu^2(v-A)/4\) and \(D_\mu(z)=F_z-V_0\psi_\mu+\mu^2(1-f)\psi_\mu\)
at \(z\). Then \(p_v=-(\mu^2/4)Jp+D_\mu(v)/4\).
Thus \(p_\mu(v)=\tfrac14\int_A^v e^{-\mu^2(v-z)J/4}D_\mu(z)\,dz\).
These integral formulas can be used pointwise in \(u\) and bounded in
\(X_\alpha\) afterward. This avoids requiring sup-norm continuity of
the zero-extended source at its moving graph.

For the massless limit \(n_Z=\tfrac14\int_A^\infty D_0(z)\,dz\).
Subtract \(e^{-dJ}n_Z\). Replacing the semigroup in the massless
source by \(e^{-dJ}\) costs at most
\(\mu^2(z-A)e^{L/(4\alpha)}/(4\alpha)\) times its norm.
Use (4) for the potential on the difference, \(L_f\) for the
Coulomb redshift error, and \(T(v)\) for the massless tail. The result is
\[
 \left\|p_\mu(v)-e^{-dJ}n_Z\right\|_\alpha
 \le \frac{e^{L/(4\alpha)}}4\left[
 \frac{\mu^2}{4\alpha}(M_1+K B_1)
 +\frac{\mu^2 K e^{B_0/(4\alpha)}}{4\alpha}B_1
 +\mu^2 K L_f(v)+T(v)\right]. \tag{5}
\]
The corresponding \(\psi\) error is at most (5) divided by \(\alpha\).
The separate bound on \(q=\psi_v\) is
\((\|F_v\|_\alpha+K[\|V_0(\cdot,v)\|_\infty+\mu^2])/(4\alpha)\).
Replacing \(d\) by \(\Lambda/2\) costs at most
\(e^{\max(d,\Lambda/2)/\alpha}|d-\Lambda/2|\,\|n_Z\|_\alpha/\alpha\)
in the \(p\) estimate. At the tube \(v=u+2x(R_{\rm obs})\), all these
bounds are uniform on \(I\) and vanish, proving (2) and (3).

For small \(\Lambda\), \(n_\Lambda=n_Z-(\Lambda/2)\Phi_Z+O(\Lambda^2)\)
in \(X_\alpha\), with norm of the error bounded by
\((\Lambda/2\alpha)^2e^{\Lambda/(2\alpha)}\|n_Z\|_\alpha/2\).
The pinned early tail has
\(n_Z(u)\sim C e^{2q_*u}|u|^\gamma\) as \(u\to-\infty\), where
\(q_*=\sqrt{k^2+\Omega^2}\), \(\gamma=4q_*-1-2\beta_{\rm GL}-s\),
and \(\beta_{\rm GL}\) is the pinned Coulomb power of the GL master.
Thus \(J^j n_Z(u)/n_Z(u)\to(2q_*)^{-j}\) for \(j\ge1\).
Use the integral kernel \((u-w)^{j-1}/(j-1)!\) and the domination
\(|n_Z(u-h)/n_Z(u)|\le C_\eta e^{-\eta h}\) for \(h\ge0\) and any
fixed \(0<\eta<2q_*\) at sufficiently early \(u\). This also
dominates the semigroup series. Consequently
\[
 \lim_{u\to-\infty}\frac{n_\Lambda(u)}{n_Z(u)}
   = e^{-\Lambda/(4q_*)}
 \tag{6}
\]
for each finite \(\Lambda\). Formula (6) gives a strictly positive
early-tail limit with suppression in amplitude. It does not
supply a signed difference at the specific benchmark cut
for a finite \(\Lambda\) without evaluating (2).

## 3. Fixed positive mass and the two limits for the same source

The null trace vanishes on the same early wedge for every fixed
\(\mu>0\). The following argument controls the full finite-slice
response with the exponentially decaying spatial source in §2. It does
not require a frequency cutoff, and its constants need not be uniform
as \(\mu\downarrow0\).

Choose \(0<\alpha_1<\alpha_2<\alpha_3<(s+\beta)/2\).
Let \(H_i=L^2((-\infty,u_1],e^{-2\alpha_i u}du)\).
The \(\alpha_3\) weighted sup estimate for the source implies that
\(\|F_v\|_{H_2}\) is integrable in \(v\).
The same finite-\(v\) estimates in §2 give \(\psi,p\in H_2\)
on every bounded truncation. In \(H_i\), \(J\) is bounded by
\(1/\alpha_i\) and accretive. For \(Y=Jh\), integration by parts gives
\[
 \operatorname{Re}\langle h,Jh\rangle_{H_i}
 =\frac12 e^{-2\alpha_i u_1}|Y(u_1)|^2+\alpha_i\|Y\|_{H_i}^2\ge0 .
 \tag{7}
\]
Use compact support or the weighted Sobolev trace to justify the
vanishing term at \(-\infty\).

For a field \(z=\psi\) or \(p\), write \(z_v=-J(V_\mu z)/4+G\).
Here \(G=JF_v/4\) for \(\psi\) and \(G=(F_v-J(V_{\mu,u}\psi))/4\)
for \(p\). The latter identity follows from
\(V_\mu\psi=J(V_\mu p+V_{\mu,u}\psi)\) and does not differentiate
the step at the initial graph. Consider
\(\mathcal E(z)=\langle z,V_\mu z\rangle_{H_2}\).
Since \(V_\mu\ge\mu^2 f(A)>0\), the quadratic norm is equivalent to
\(H_2\), with constants depending on the fixed mass.
The term with \(-J(V_\mu z)\) is nonpositive by (7). The derivative
of the weight costs \(\|V_{\mu,v}/V_\mu\|_\infty\mathcal E\).
The coefficient is integrable in \(v\), since \(r_v=f/2\) and
\(V_{\mu,v}=O_\mu((v-u_1)^{-2})\). Dividing the inequality by
\(\sqrt{\mathcal E}\), with a regularization at zero, and applying
Gronwall proves a uniform \(H_2\) bound for \(\psi\).
The term \(V_{\mu,u}\psi\) is also integrable in \(v\) because
\(r_u=-f/2\), yielding a uniform bound for \(p\) in turn.
These energy identities hold with the graph extension, by absolute
continuity in weighted \(L^2\) because the graph value and first jets match.

Now use the comparison coefficient \(c(v)=\mu^2(1-2/v)/4>0\).
The difference \(V_\mu-4c(v)\) is integrable as an operator
\(H_2\to H_1\). In fact
\[
 \left|\frac1r-\frac2v\right|
  =\frac{|u+2\log(r-1)|}{vr}
  \le C\frac{|u|+\log v}{v^2},
\]
where the last bound follows from \(r\ge(v-u)/4\) and \(u\le u_1<0\).
The ratio of weights \(e^{(\alpha_2-\alpha_1)u}\) absorbs \(|u|\),
and \(V_0=O(v^{-3})\) uniformly. Therefore
\(z_v=-c(v)Jz+\widetilde G\) with
\(\widetilde G\in L^1_v H_1\) for both \(\psi\) and \(p\).

The semigroup \(e^{-aJ}\), \(a\ge0\), is a contraction on \(H_1\)
and converges strongly to zero as \(a\to\infty\). To see this, extend
data by zero to \(u>u_1\), multiply by \(e^{-\alpha_1 u}\), and use the
full-line Fourier multiplier of \(J\), \(1/(\alpha_1+i\xi)\) in the
negative Fourier sign convention. Its real part is strictly positive
at every finite frequency. Dominated convergence in Fourier
\(L^2\) proves strong decay, and restricting back to \(u\le u_1\)
commutes with the semigroup by causality. Since
\(\int_z^v c(w)dw\to\infty\) for each fixed \(z\), variation of
constants and dominated convergence in the \(L^1_vH_1\) forcing
prove \(\psi(v),p(v)\to0\) in \(H_1\).
The flux requires a tilted tube. First,
\(\|q(\cdot,v)\|_{L^\infty(I)}\to0\) by
\(q=J(F_v-V_\mu\psi)/4\) and the \(H_1\) Cauchy–Schwarz bound
\(|Jh(u)|\le e^{\alpha_1u}\|h\|_{H_1}/\sqrt{2\alpha_1}\).
Also \(p_v=(F_v-V_\mu\psi)/4\to0\) in \(H_1\).
Across the finite range \(v=2x(R_{\rm obs})+I\), integrate \(p_v\)
from the first \(v\) and use Cauchy–Schwarz in \(v\), then
integrate in \(u\). The \(L^2(I)\) variation of \(p\) is at most
\(|I|\sup_v\|p_v\|_{L^2(I)}\to0\). The same argument using \(q\)
handles \(\psi\). Thus all three terms in (1) vanish in the integrated
limit.

In particular, for \(I=[-10,-8]\),
\[
 \lim_{\mu\downarrow0}\lim_{R_{\rm obs}\to\infty}\mathcal K_\mu=0,\qquad
 \lim_{R_{\rm obs}\to\infty}\lim_{\mu\downarrow0}\mathcal K_\mu
 =\mathcal K_Z\in(3.5316,4.0171)\times10^{-7}.
\]
Fixed-\(R\) mass continuity follows directly from (4) and the
characteristic derivative equations. These distinct limits concern
the fixed-retarded-cut signed flux in the propagator-only EFT.
They leave the total timelike escape and horizon/stored energy for a
completed history as separate observables.

## 4. Above-threshold escape and completion control

For smooth compact spatial forcing with an integrable time history,
the radial outgoing solution at \(\omega>\mu>0\) has unit
asymptotic phase \(p_\omega x+\mu^2\log r/(2p_\omega)\) with
\(p_\omega=\sqrt{\omega^2-\mu^2}\).
The unit Wronskian current is \(p_\omega\). For a source-free exterior,
the time-integrated current on that band is
\(\int_B\omega p_\omega |a(\omega)|^2\,d\omega/(\pi G_4)\)
when restoring physical \(\omega\), \(p_\omega\) and the physical
field amplitude. The sign follows directly from
\(\psi_t=-i\omega\psi,\ \psi_x\simeq i p_\omega\psi\) for the
positive-frequency amplitude and Parseval, with both conjugate
frequencies counted. For the massive horizon, the ingoing wavenumber
is \(\omega\), so the ingoing channel has weight \(\omega^2\) in the
same normalization. Subthreshold decaying radial solutions have
zero Wronskian flux. The threshold itself has Coulomb-type
local wavenumber \(\sim\mu/\sqrt r\), so the ordinary unit-\(p_\omega\)
plane-wave asymptotic is singular as a frequency-uniform statement.
Claims about integrated escape outside a resolved band require
threshold and high-frequency bounds, and an outer-source error for
noncompact spatial support.

Here is an exact test of why an early window cannot specify that
energy. Take any stipulated completion with an outgoing amplitude
on a compact band strictly above threshold, and add
\(\lambda b(t-T)c(x)\) to its scalar source. Choose both bumps
smooth and compact, and \(T\) large enough that the support has
\(u>u_1\). Finite propagation gives exact equality of the two fields
throughout the entire early wedge. At a chosen \(\omega_*>\mu\),
the real-frequency ingoing solution \(h_L\) has nonzero current
and is nonzero at any finite \(x\). Thus a real bump \(c\) supported
near any one \(x_*\) can have
\(\int h_L(\omega_*,x)c(x)\,dx\ne0\).
Choose a real short bump \(b\) with \(\widehat b(\omega_*)\ne0\).
The outgoing amplitude increment per unit \(\lambda\) is
\(-W^{-1}e^{i\omega T}\widehat b(\omega)\int h_Lc\,dx\).
The denominator is nonzero since a simultaneous ingoing-horizon
and outgoing-right homogeneous solution would require opposite
signs for the same conserved current. Hence the increment has
strictly positive weighted energy on a small band about
\(\omega_*\). The completed band energy is a quadratic polynomial in
\(\lambda\) with a strictly positive quadratic coefficient.
Even the exact early history \(e^{st}\) and all early trace data
therefore leave this energy underdetermined. This control concerns
stipulated scalar EFT sources. Changing the vacuum GL metric in
this way would change its field equations.

## 5. Vacuum extraction failure control and remainder budget

Half-period symmetry makes the averaged trace even in the seed
if the clock and extraction surfaces are fixed equivariantly.
Evenness and smooth fixed-radius Taylor expansions still allow a
failure at infinity. To exhibit it within an exact wave problem,
take the right-moving massless band
\(n(u)=\operatorname{Re}\int_a^b C(\omega)e^{-i\omega u}\,d\omega\)
for smooth \(C\) supported in \(0<a<b\) and \(n\ne0\).
The flat Klein–Gordon derivative field
\[
 n_\epsilon(u,x)=\epsilon^2\operatorname{Re}\int_a^b C(\omega)
 e^{-i\omega u+i[\sqrt{\omega^2-\epsilon^2}-\omega]x}\,d\omega
\]
is even and analytic for each fixed \(x\) near \(\epsilon=0\).
Its quadratic coefficient is exactly \(n\), and its quartic
coefficient is
\(x\operatorname{Re}\int_a^b[-iC(\omega)/(2\omega)]e^{-i\omega u}\,d\omega\).
For each fixed nonzero \(\epsilon\) with \(|\epsilon|<a\), the derivative
of the spatial phase in \(\omega\) is
\(x(\omega/\sqrt{\omega^2-\epsilon^2}-1)\), bounded away from zero in
units of \(x\). Repeated integration by parts gives zero null trace.
For an interval where \(n\) is nonzero this cannot have a uniform
quartic expansion of its null trace. It is a manufactured
mass-variation control rather than an exact vacuum counterexample.

A separate clock-only control holds even for a pure outgoing massless
field. If a reference extraction uses \(u\) while the actual outgoing
label is \(u-\epsilon^2\log r\), then
\(\epsilon^2 n(u-\epsilon^2\log r)\) has a fixed-radius
quartic coefficient \(-(\log r)n'(u)\). For \(n\) of compact
support the trace in the displaced reference label vanishes for
fixed nonzero seed and \(r\to\infty\), while it is
\(\epsilon^2 n\) in the outgoing label. This distinguishes a missing
clock comparison from loss of physical radiation. The construction
does not assert that the exact vacuum optical system realizes this
shift. It tests the required **radius-uniform** comparison.

In a common normalized clock, if the required vacuum estimate
\(\|N_\epsilon-\epsilon^2 n_Z\|_{L^2(I)}\le C_I\epsilon^4\)
is proved, the order-four energy is bounded by
\[
 2\epsilon^4\max(0,\sqrt{\mathcal K_-/2}-C_I\epsilon^2)^2
 \ \le E_I/M_0 \le\
 2\epsilon^4(\sqrt{\mathcal K_+/2}+C_I\epsilon^2)^2 .
\]
Here \(\mathcal K_-=3.5316\times10^{-7}\) and
\(\mathcal K_+=4.0171\times10^{-7}\). Strict positivity follows
for \(C_I\epsilon^2<4.20214\times10^{-4}\), a rounded-down bound
on \(\sqrt{\mathcal K_-/2}\). These inequalities include no bound
on the unknown \(C_I\). The reference-wedge linear and quadratic
estimates in the pinned audit and the fixed scalar proofs above
do not control the quasilinear vacuum optical problem.

## 6. Strict early attenuation on a nonzero critical-scale interval

The actual Z benchmark admits an analytic **signed** decision in the
joint limit, in addition to the positive asymptotic-tail amplitude
in (6). Write \(n_0\) for the free-source massless news in GOAL-005,
distinct from the full \(n_Z\). For \(0<\Lambda\le s\), with \(s=2\Omega r_0\),
\[
 0< \left(1-\frac{21\Lambda}{25s}\right)n_Z(u)
 \le n_\Lambda(u)
 \le n_Z(u)-\frac{3\Lambda}{10}\Phi_Z(u)
 <n_Z(u),\qquad u\le-8. \tag{8}
\]
On \(I=[-10,-8]\) this gives
\[
 \mathcal K_Z-\lim_{\mu^2x\to\Lambda}\mathcal K_\mu
 >5.17\times10^{-9}\,\Lambda,\qquad
 \lim_{\mu^2x\to\Lambda}\mathcal K_\mu
 \ge (1-21\Lambda/(25s))^2\mathcal K_Z. \tag{9}
\]
All fluxes in (9) are ordered scalar coefficients in the declared
fixed-background model. This comparison is for the joint limit
\(\mu\to0,\ R_{\rm obs}\to\infty\) at finite \(\Lambda\).
A finite tube has the additional errors in (5) and the
separate \(q,b\psi\) terms in (1).

For the source sign, set \(a=k^2\), \(y=(1+ar^3)U_{\rm GL}/r\),
\(d_y=y_r/y\), and \(p_*=U_{{\rm GL},x}/U_{\rm GL}\).
The pinned GOAL-005 Riccati barrier gives
\(-[0.9-0.4/r]<p_*<0\) for \(r\ge2\). For \(r\ge5\),
\[
 d_y=\frac{2ar^3-1}{r(1+ar^3)}+\frac{p_*}{f}
 \in(-0.9,0.4).
\]
Indeed \(ar^3>2\) gives a first term greater than \(1/r\), and \(p_*/f>-0.9-0.5/(r-1)\). The first term is below \(2/r\).
Let \(K=H_{\theta\theta}/r^2\). In units of \(U_{\rm GL}/r\),
the pinned angular component is \(-1/2-fd_y/(ar)\) and the mixed
component is \(-(\Omega/a)[d_y-1/(2r(r-1))]\). Use
\(a>0.39,\ \Omega<0.064,\ f/r\le4/25\) on \(r\ge5\).
The angular ratio is less than \(-0.13\) and the absolute mixed
ratio is below \(0.152\). The trace relation
\(fH_{rr}=H_{tt}/f-2K\) gives
\[
 H:H=2(H_{tt}/f-K)^2+4K^2-2H_{tr}^2>0
\]
since \(4(0.13)^2-2(0.152)^2=0.021392>0\).
The sign of the source is thus positive throughout the early
domain, because \(R(-8)>5\) follows from \(e^{3/2}>4\) and the
strict decrease of \(g\).

Positivity and monotonicity of the lower endpoint in the free ray
integral imply \(0<n_0(u-h)\le e^{-s h}n_0(u)\) for \(h\ge0\).
The sup norm of the free field through cut \(u\) is at most
\(\int_{-\infty}^u n_0(w)dw\le n_0(u)/s\). The exact curvature
operator bound \(\kappa_{R(u)}\) of GOAL-005 therefore gives
\[
 |n_Z(u)-n_0(u)|
 \le \frac{n_0(u)}{4s R(u)^2(1-\kappa_{R(u)})}<0.09\,n_0(u).
\]
Here \(s>0.126,\ R(u)>5,\ \kappa_{R(u)}<0.075\). It follows that
\(n_Z>0\) on the whole half-line and
\(n_Z(u-h)\le(6/5)e^{-s h}n_Z(u)\).
Consequently \(\Phi_Z=Jn_Z>0,\ \Phi_Z\le6n_Z/(5s)\),
and integrating the same translated inequality yields
\(\Phi_Z(u-h)\le(6/5)e^{-s h}\Phi_Z(u)\).
For \(d=\Lambda/2\in(0,s/2]\), the absolute remainder
in \(n_\Lambda=n_Z-d\Phi_Z+\mathcal R\) is bounded by
\[
 |\mathcal R|
 \le \frac65\,s(e^{d/s}-1-d/s)\Phi_Z
 <\frac25\,d\Phi_Z.
\]
This follows from \(J^j n_Z\le(6/5)s^{1-j}\Phi_Z\) for \(j\ge2\)
and the increasing function \((e^y-1-y)/y\) on \(0<y\le1/2\),
whose maximum is \(2e^{1/2}-3<1/3\). This proves (8). To obtain
the strict lower difference in (9), put
\(\Delta\Phi=\Phi_Z(-8)-\Phi_Z(-10)>2(6.5671\times10^{-5})\) by
the pinned pointwise interval. Then
\[
 2\int_I(n_Z-n_\Lambda)(n_Z+n_\Lambda)\,du
 \ge \frac65d\int_I n_Z\Phi_Z\,du
 \ge\frac35d(\Delta\Phi)^2.
\]
The remaining arithmetic is exact rational. Reproduce from
the scoped directory using Python 3.14.8
with `python3 -B GLR-GOAL-016/verification/checks/scalar_sign_gates.py`.
The output is archived in [scalar_sign_gates.txt](checks/scalar_sign_gates.txt).
This check validates constants after the analytic reconstruction.
It does not independently reproduce the input GL spectral bracket
or the GOAL-005 outward-rounded waveform.

## 7. Exposed audit of the preparation-cost theorem

The coordinator provided [PREPARATION.md](../PREPARATION.md) after
the independent current and limiting arguments above. This section
checks the cost result with advance knowledge of that derivation.
The frozen GOAL-005 exact constraint-lift and mass map are used as
proved inputs, with no new constraint computation here.

- **Sign and norm.** The one-form in §1 gives
  \(e-jT'=(1-T')h_u^2+(1+T')h_v^2+V_0h^2/2\) on the
  spacelike graph. Its opposite orientations on the two future
  characteristic boundaries make both future energy losses
  nonnegative. The earlier outgoing boundary can be chosen before
  the support of a compact homogeneous preparation and has zero
  flux by causality. Thus a cap controls the squared news norm
  on \(I\) with no factor of two. The factor of two enters the
  physical fraction relative to \(M_0\) as in (1). The one-form
  endpoint correction vanishes at both ends of the entire exterior
  graph because \(f/r\to0\) at the horizon and the data vanish at
  the spatial end.
- **Smooth right inverse.** With the outgoing graph jet
  \((h,h_u,h_v)=(A,A',0)\), its tangential derivative is indeed
  \(A'\). The perturbation of the identity on \(A'\) is the ray
  integral of \(V_0h/4\) with a minus sign. Bounding the retarded
  solution pointwise in \(u\) by the integrated \(|A'|\) and
  the positive exponential kernel gives the stated
  \(2|J|/\pi\) norm, where \(J\) denotes the padded interval
  in the preparation derivation. A useful regularity route is
  inversion on \(C^0\) first, because
  \(k_b |J|e^{k_b|J|}<1\). The transport equation shows
  \(h_u=A'-\int_{R(u)}^v V_0h/4\) and \(h_v\) is the corresponding
  integral from \(g(v)\). Differentiating the ray map adds one
  \(u\)-derivative of \(h\), which these identities give with
  bounded tails. Moving lower endpoints are smooth data.
  Iteration therefore increases differentiability of the ray-map
  term. The smooth target yields a smooth inverse.
  Causal uniqueness on any prefix where the target vanishes gives
  flatness near the left cutoff. An inward change beyond the right
  cutoff cannot affect the news on \(I\).
- **Seam and exact lift.** Write \(p_\Sigma=T'=-1/r\). The future
  unit normal is
  \((\partial_t+p_\Sigma\partial_x)/\sqrt{f(1-p_\Sigma^2)}\).
  For outgoing \(h=A\), \(h_t=A'\), \(h_x=-A'\), the exact-lift
  inputs at the coefficient order are
  \(F=A/r\) and
  \(P=[(1-p_\Sigma)A'/r-p_\Sigma fA/r^2]/\sqrt{f(1-p_\Sigma^2)}\).
  The sign of the second term comes from differentiating \(1/r\).
  In the static continuation \(F=A(b)/R(b)\) is constant and
  \(P=0\), so this pair extends regularly across the horizon and
  can be tapered inside it. A blend of values with the same seam
  value and independently blended \(h_t\) keeps
  \(h_x=d_x(h|_\Sigma)-T'h_t\) bounded. The excess seam energy is
  \(O(\eta)\) for each fixed padded pair. The static energy
  follows independently from
  \((h h_x)_x=h_x^2+V_0h^2\), so its upper endpoint contributes
  \(f(R(b))A(b)^2/(2R(b))\), with zero lower endpoint.
  The original GOAL-005 constraint theorem applies to every fixed
  smooth lifted pair and tunes its ADM mass. No common lower
  bound on the available seed range is implied as the seam
  and padding shrink.
- **Adversarial budget.** Dropping the static term would remove a
  genuine positive contribution for \(A(b)\ne0\).
  Allowing a nonzero clock displacement would also change the
  target on a fixed cut. The lower cost bound is unaffected by
  arbitrary compact incoming data. With the published padded
  inverse and seam construction, the independent checks found
  no defect in the reported interval
  \(b_Z^2\le\mathcal E_{\rm can}<1.20b_Z^2\).

## 8. Exposed audit of the directed tube certificate

This audit used the transport specialist's
[tube code](../transport/checks/tube_intervals.py)
and [archived \(R=48\) output](../transport/checks/tube48.json).
It is an independent reconstruction of the certificate's inequalities
and a code inspection, with the numerical conclusion visible. It does
not replay the interval-heavy implementation or independently generate
the shared GL Riccati boxes. The review checked the following points.

1. **Zero-data Riemann representation and graph intersection.**
   With \(F_\nu(t)={}_0F_1(\nu+1;-t)\), the exact constant-mass
   kernel for \(4\partial_{uv}+\mu^2\) is
   \(F_0(\mu^2(u-U)(v-V)/4)\). This follows by substituting its
   power series and using its unit value on each characteristic.
   The residual potential in the true equation is
   \(\Delta=V_0-\mu^2/r\), with source \(F-\Delta\psi\).
   Zero extension below \(v=R(u)\) produces no graph distribution.
   On the active graph domain \(U\ge g(V)\), so the source radius
   satisfies \(r'\le V\le v_{\max}\). On the source part restricted
   to \(r'<R_{\rm obs}\), change variables at fixed \(U\) by
   \(dV=2\,dr'/f(r')\). Put \(a=u-U\), \(c=2(x(R_{\rm obs})-x(r'))>0\)
   and \(d=(u-g(r'))_+\). The range is exactly \(a\in[0,d]\),
   the argument is \((\mu^2/4)a(c+a)\), and the radial source
   measure after the overall factor \(1/4\) is
   \(M(r')dr'=S(r')e^{sx(r')}dr'/(2f(r'))\).
   The differentiated kernel contributes
   \(-(\mu^2/4)(c+a) F_1\) for \(p\) and
   \(-(\mu^2/4)aF_1\) for \(q\).
   The \(p\) source endpoint takes all fully active radial cells,
   a nonnegative interval enclosing each partial cell, and zero
   for each wholly inactive cell. The \(q\) endpoint at \(V=v\)
   has \(r'\ge R_{\rm obs}\), so it is entirely included in
   the omitted-source bound.
2. **Signed kernel and complete quadrature boxes.**
   The exact \(F_0(4)<0\) and \(F_1(4)<0\) controls in
   [scalar_sign_gates.txt](checks/scalar_sign_gates.txt)
   reject a positivity shortcut for either kernel. The code
   uses instead \(|F_\nu(t)|\le1\) and
   \(|F'_\nu(t)|\le1/(\nu+1)\) for \(t\ge0\), from the
   normalized Bessel integral and derivative identity. The
   Lipschitz interval about a nonnegative center therefore covers
   either sign for each quadrature node. The chain-rule third and
   fourth derivative bounds use the exact quadratic argument,
   for which higher derivatives of the argument vanish. The
   \(aF_1\) Simpson error has the additional fourth derivative
   term \(4(F_1\exp(-sa))^{(3)}\). The \(u\)-cell and grouped radial
   boxes propagate through the entire node expressions, including
   the moving endpoint \(d\). The exact integral for each \(u,r'\)
   is bounded by the interval Simpson error before the radial
   grouping. Since \(M\) is positive on the grouped source cells,
   multiplication of that signed kernel enclosure by the positive
   measure of the group is valid for either sign of the kernel.
   Float conversions choose only a positive even node count
   or an integer cap which is then checked to lie beyond \(v_{\max}\).
3. **Tail and continuum resolvent budget.**
   The remaining source cells bound the integrals of
   \(M d^j e^{D d}\) using the largest \(d\) in the cell.
   The analytic \(r\ge60\) bound uses the inherited master
   decay, \(d\le3r\), and \(r/60\le\exp[(x-x(60))/60]\).
   The analytic tail remains exponentially integrable for
   the retained moments. Setting \(J_0(u)\) to the positive
   Gronwall upper field and \(J_1,J_2\) to its successive
   causal primitives, the \(C^0\) correction is at most
   \(D J_1\), with \(D=\sup\tfrac14\int|\Delta|\,dv\)
   on the physical wedge. For \(p\), the direct endpoint
   costs \(D_{\rm wall}J_0\) and the interior derivative at most
   \((\mu^2/4)(v_{\max}-5)D J_1\), since each active graph
   ordinate is \(V\ge r_i^{\rm wedge}>5\). For \(q\), the
   endpoint costs \((\mu^2/R_{\rm obs}+1/R_{\rm obs}^3)J_1/4\)
   and the interior term \((\mu^2/4)D J_2\). These are
   continuum error inequalities, including the graph and the
   outer-source residual, so there is no artificial interface
   between PDE solution cells. The sharper moment option for
   \(J_j\) follows by commuting its \(j\) causal primitives
   with the exponential Gronwall kernel, giving integrals
   of \(a^j e^{-(s-D)a}/j!\) on \([0,d]\).
4. **Current and output interpretation.**
   The code's \(2(p+q)(p-q+(f/R_{\rm obs})\psi)\) matches
   (1), including the physical endpoint term and sign.
   Integration by interval range on each full \(u\)-cell
   is an enclosure rather than sample convergence.
   The archived intervals separate all three masses at \(R=48\).
   The result measures signed power integrated through a finite
   tube on the chosen early cuts. A horizon flux does not enter
   this domain. The corresponding null-cap storage/work identity
   was derived in §1. The reported tube integral alone does
   not imply an asymptotic escape or absorption budget.

No defect in the primary enclosure was identified by these checks.
A shared premise remains the pinned GL root enclosure and radial
mode reconstruction. The exact sign-changing control tests one
possible shortcut and does not certify any numerical flux by itself.
