# C. Retarded axial null trace and pole functional

Independent construction from GOAL-007 commit 0864d9d807de56cbdd6e64d215e6295508a3007f, especially HIERARCHY, INVESTIGATOR, AXIAL-TRACE, and INDEPENDENT-REVIEW, with the pinned GOAL-005 GL profile and normalization. Scope is the fixed-order, reflected, mass-matched axial \(\ell=2\) vacuum string, not an exact nonlinear radiation theorem. I use \(r_0=2M\) with geometric \(M\), \(f=1-r_0/r\), \(x=r+r_0\log(r/r_0-1)\), \(u=t-x\), \(v=t+x\), \(\lambda=(\ell-1)(\ell+2)\), \(k=2\pi/L\), \(q=\sqrt{k^2+\Omega^2}\). The real GL cosine has coefficients \(1/2\) at each of \(\pm k\). Jet coefficients contain no factorials. Take compact smooth axial canonical data on a spacelike graph \(t=\tau(x)\) agreeing with \(t=0\) at the far end, with \(|\tau'|<1\) and \(u_\Sigma=\tau-x\to+\infty\) at the left end. A regular future-horizon slice has \(\tau+x\to v_H\).

## 1. Completed master and finite-slice response

In the pinned axial Proca reduction let \(F=dA\) and write the mean source density \(I^a=-\langle E_1 A_1+E_2 A_0\rangle^a\), where \(A_1\) is the constrained massive retarded field. The second term includes the spherical \(B_0\) deformation and the explicit \(H^2\) terms. Reflection kills \(I^z\) at \(n=0\). Put \(\mathcal O=r^4 F_{tx}/f=r\psi\). The two orbit equations are
\[
\partial_x\mathcal O+\lambda r^2 A_t=I^t,\qquad
-\partial_t\mathcal O-\lambda r^2 A_x=I^x.
\]
Substituting into \(F_{tx}=\partial_t A_x-\partial_x A_t\) gives
\[
\mathcal L\psi=S_\psi=r[\partial_t(I^x/r^2)+\partial_x(I^t/r^2)],\qquad
\mathcal L=-\partial_t^2+\partial_x^2-V,\qquad
V=f[\ell(\ell+1)/r^2-3r_0/r^3].
\]
I use the Cunningham–Price–Moncrief normalization \(\Psi=-2\psi/\lambda\) below, so its source is \(S=-2S_\psi/\lambda\). With the fixed asymptotic Bondi frame and unit-norm real spherical harmonic, \(C_{AB}=\Psi_{\mathscr I} X_{AB}\) and \(N_{AB}=\partial_u\Psi_{\mathscr I}X_{AB}\). The pure mean radion cancels from the axial Proca densities on the two-dimensional noncompact block. Fixed order-two ADM mass and a normalized asymptotic clock remove the \(O(\log r)\) cut mismatch from a constant mass shift. All other lower-order metric/gauge terms must be included in the completed source and initial data. The result uses the CPM shear variable, with a derivative required to obtain the news. [Martel–Poisson v1, Eqs. (5.13)–(5.20), (6.12)–(6.16)](https://arxiv.org/pdf/gr-qc/0502028v1).

On \(t=0\), the transform is \(\widehat X(s)=\int_0^\infty e^{-st}X(t)\,dt\) and \(s=-i\omega\) in the \(e^{-i\omega t}\) convention. The pencil and inhomogeneity are
\[
P_s=\partial_x^2-V-s^2,\qquad P_s\widehat\Psi=\widehat S-s\Psi_0-\Psi_1.
\]
The signs follow from zero extension: \(\mathcal L(\mathbf1_{t>0}\Psi)=\mathbf1_{t>0}S-\Psi_0\delta'(t)-\Psi_1\delta(t)\). On the graph, use the same stationary transform of the zero-extended solution. If \(w=e^{-st}h(s,x)\) is any separated homogeneous test solution, the slice contribution to the radial pairing is
\[
D_\Sigma(s)=-\int [w\mathcal N\Psi-\Psi\mathcal N w]_{t=\tau(x)}\,dx,
\qquad \mathcal N=\partial_t+\tau'\partial_x .
\]
This follows by integrating the bilinear Green current \((-w\Psi_t+\Psi w_t,\ w\Psi_x-\Psi w_x)\) across the graph. Its flux gives the distribution commutator with the step, and at \(\tau=0\) gives \(-\int h(s,x)(s\Psi_0+\Psi_1)\,dx\). Data here mean the induced order-\((2,1)\) radiative pair of the preparation. A fixed canonical pair in a deformed orbit need not mean zero data in a reference master.

Let \(h\sim e^{sx}\) at the future horizon and \(g\sim e^{-sx}\) at infinity for \(\Re s>0\), and let \(W=h g'-h'g\). The Green kernel of \(P_s\) is \(h(x_<)g(x_>)/W\) because its derivative jump is \(+1\). For \(\ell\ge2\), \(V\ge0\), so an \(L^2\) solution with these two decays would require \(\langle \Psi,(-\partial_x^2+V)\Psi\rangle=-s^2\|\Psi\|^2\), impossible for \(\Re s>0\). The static \(s=0\) endpoint is outside this argument. For a compact graph datum and an admissible source \(S_\chi\) as below, the retarded null transform is
\[
\widehat\Psi_{\mathscr I}(s)=
\frac{D_\Sigma(s)+\iint_{t>\tau(x)} e^{-st}h(s,x)S_\chi(t,x)\,dt\,dx}{W(s)} . \tag{C1}
\]
For the free \(V=0\) control, \(W=-2s\) and a velocity delta at \(x=0,t=0\) gives \(\widehat\Psi_{\mathscr I}=1/(2s)\), consistent with a half-step. In deriving (C1), the trace transform is bilateral in \(u\) with a finite earliest cut. The radial factor is \(e^{sx}\widehat\Psi(s,x)\), as follows from \(u=t-x\). The Green sign of Casals–Ottewill is opposite with their Wronskian ordering and Green source convention. [Casals–Ottewill, arXiv:1210.0519, §II Eqs. (2.1)–(2.8), PRD 87 064010](https://doi.org/10.1103/PhysRevD.87.064010).

## 2. Causal window with quantitative continuation domain

Choose a real smooth \(\chi(u)\) equal to one up through a specified \(U\) and zero for \(u\ge U+\Delta\). Set \(S_\chi=\chi(t-x)S\) **after** the constrained projection, and leave the compact slice data fixed. At finite \(u\le U\) the physical and windowed solutions coincide by finite propagation, since \(u'\le u\) in the past of the observer. This is an auxiliary projected-master continuation. Applying the cutoff directly to the five-dimensional sources would in general spoil their Bianchi constraints. There is a finite lower \(x\) bound for the windowed volume source since \(\tau(x)-x\to+\infty\) as \(x\to-\infty\). A lower cut \(u_0\) also bounds the support because the axial slice data and all their induced local jets are compact, and the spherical fields enter only multiplied by an axial field.

For each bounded causal strip and finite derivative order needed for the source, one has a sufficient estimate of the form
\[
|S(t,x)|\le C_\delta e^{-(\beta-\delta)x},\qquad
\beta=q-\Omega,\quad x\gg1,\quad u\in[u_0,U+\Delta], \tag{C2}
\]
for any \(\delta>0\), under the pinned fixed-order smooth preparation. Polynomial factors in \(x\) are absorbed into arbitrarily small losses \(\delta\). To see the massive step, use a normalized first-order axial curl/dual field in a fixed Fourier mode. Its equation on the far end has the structure \(W_{uv}=A_1 W_u+A_2 W_v+CW+F\), with \(A_i=O(x^{-1})\), bounded \(C\), and corresponding derivative bounds. Baseline mean axial data have a bounded differentiated trace by their short-range scalar RW equation. Hence \(F\) for the massive field is exponentially decaying in \(x\) on the finite strip. Subdivide the \(u\) interval into finitely many slabs of width \(d\). If \(J=W_u\), \(I_u J=\int_{u_L}^u J\), the equation on a slab is
\[
(1-A_2 I_u)J_v=(A_1+CI_u)J+A_2 W_L'+CW_L+F .
\]
At large \(v\) the inverse has norm at most 2. Choose \(d\) small and the starting \(v\) large so the homogeneous evolution of \(J\) in \(v\) grows at a rate below a given positive target. The first slab has zero left data before \(u_0\). Each next slab has left data at the target rate from the previous slab, and variation of constants gives the same target rate since it is strictly above the homogeneous rate. Recover \(W\) and \(W_v\) from the integral and equation. Commuted equations have the same leading operator with lower differentiated fields in the forcing, so the argument holds for any fixed finite number of derivatives. Since \(v=2x+u\), the massive field can be bounded by \(C_\delta e^{\delta x}\) on the given finite interval. The returning \(H J\) product gives (C2), while \(H^2P\) and the fixed-mass mean Einstein-frame \(B_0P\) have the faster \(2\beta\) rate. This argument extends the pinned short-strip estimate by a finite subdivision and gives no uniform control as \(U\to+\infty\).

For the scalar solution on every fixed compact \(u\) interval, \(\mathcal L=-4\partial_u\partial_v-V\) and \(V=O(x^{-2})\) imply convergence of \(\partial_u\Psi\) at \(v=\infty\) by integrating \(-\tfrac14(V\Psi+S_\chi)\) in \(v\). Boundedness of \(\Psi\) follows by the characteristic double integral and Gronwall in \(u\), since \(\int |V|dv\le 2\int |V|dx\) on each cut. The bounded smooth graph data contribute the flat homogeneous Cauchy solution to this formula. For the windowed source the spacetime integral of \(|S_\chi|\) is finite, giving a uniform exponential-in-\(u\) bound for \(\Psi\) independently of \(v\). This bound justifies commuting the null limit with the transform on a sufficiently rightward contour. For \(\Re s>0\) the second piece of the radial Green integral (source \(y>x\)) vanishes in the null limit. Indeed \(|h(s,x)|\lesssim e^{\Re s x}\), \(|g(s,y)|\lesssim e^{-\Re s y}\), and \(|\widehat S_\chi(s,y)|\lesssim e^{-(\beta-\delta+\Re s)y}\). The first piece tends to the numerator in (C1).

A concrete holomorphic continuation of the horizon solution follows by writing \(h=e^{sx}m\) and using the Volterra kernel \((1-e^{-2s(x-y)})/(2s)\) for \(y<x\). The horizon falloff \(V=O(e^{x/r_0})\) gives a locally holomorphic solution for \(\Re s>-1/(2r_0)\). The continuation of the infinity solution is fixed by rotating the far radial ray. The inverse relation \(r=z-r_0\log(r/r_0-1)\) has an analytic branch for sufficiently large \(z\) in a closed right sector by contraction in a disk of radius \(O(\log|z|)\) about \(z\). Thus \(V(z)=O(z^{-2})\) there. For \(g=e^{-sz}n\) use the same kernel in \(w-z\), integrating \(w=z+e^{i\theta}a\), \(a\ge0\), with \(\Re(s e^{i\theta})>0\). The Neumann–Volterra iteration converges and gives \(n=1+O(1/z)\). In the overlap \(\Re s>0\), the solution is the physical outgoing Jost function by uniqueness of the decaying asymptotic branch. Transport the Jost data to the finite real interval. At \(\Im s<0\), \(\theta=\pi/4\) suffices near the fundamental. This supplies a local continuation around the pole on the side obtained from the physical half-plane. The negative real \(s\) axis is the usual cut. Alternatively use the Jost continuation of the inspected Leaver and Casals–Ottewill papers. [Leaver, PRD 34 384, §§II–III and (89)–(93)](https://doi.org/10.1103/PhysRevD.34.384).

Uniformly on compact spectral subsets away from \(s=0\), the finite-interval Cauchy data for \(h\) and variation of constants on the real far end give \(|h(s,x)|\le C_K e^{|\Re s|x}\). This follows by factoring that exponential out of the free \(\cosh(sx),\sinh(sx)/s\) kernels and applying \(\int|V|dx<\infty\). Consequently the numerator of (C1) is holomorphic in the overlap region through
\[
\Re s>-\tfrac12(\beta-\delta),\qquad \Re s>-1/(2r_0),
\]
on the declared continuation. This conclusion concerns a source window compact in \(u\) that extends all the way to spatial infinity. No rotation or analyticity of that source in \(x\) is required. For L10, \(\beta r_0\in[0.568215978,0.568215997]\) using the pinned \(\Omega r_0\in[0.06328118,0.06328120]\), whereas \(2|\Re s_q|r_0\simeq0.355850\) for the axial fundamental. Thus the sufficient convergence bound has a positive margin for that pole. The published pole value is numerical and no zero or simplicity enclosure was run. [Leaver, Proc. R. Soc. A 402 285, Table 1](https://the-center-of-gravity.com/documents/40/Leaver_An-Analytic-representation-for-the-quasi-normal-modes-of-Kerr-black-holes.pdf).

## 3. Pole coefficient, adjoint, and nonuniqueness

At a simple isolated zero \(s_q\) of \(W\) the CPM shear residue is
\[
C_q^\chi=\frac{D_\Sigma(s_q)+\iint e^{-s_qt}h(s_q,x)S_\chi(t,x)\,dt\,dx}{W'(s_q)} .
\]
In an inverse-Laplace pole term the coefficient is \(+C_q^\chi e^{s_q u}\), and the news residue is \(s_q C_q^\chi X_{AB}\). With an \(e^{-i\omega u}\) inversion the corresponding multiplier of the \(\omega\)-plane residue is \(-i\). Reality supplies the conjugate pole. There is no conjugation in the bilinear source projection, which comes from the transpose of the continued pencil. A real-axis \(L^2\) norm is divergent. On a finite interval \([a,b]\) enclosing a compact radial source choose \(a,b\) away from endpoint zeros, set \(\alpha=h'(a)/h(a)\), \(\zeta=g'(b)/g(b)\), and write \(R=g(s_q,\cdot)\), \(h(s_q,\cdot)=c_qR\). Multiplying the order-zero Laurent equation by \(R\), integrating by parts, and differentiating the transparent Robin conditions gives
\[
C_q=-\frac{\int_a^b R F_q\,dx}
{2s_q\int_a^bR^2dx-\zeta'(s_q)R(b)^2+\alpha'(s_q)R(a)^2}.
\]
Here \(F_q\) includes the slice source distribution. In detail the constant Laurent term satisfies \(P_{s_q}\Psi_{\rm reg}-2s_q C_q R=F_q\), with \((\partial_x-\alpha)\Psi_{\rm reg}|_a=C_q\alpha'R(a)\), and the corresponding formula with \(\zeta\) at \(b\). Its Green boundary term proves the signs. The denominator is \(-W'(s_q)/c_q\), including the endpoint derivatives. This finite-radius adjoint check applies to compact radial approximants of the window and tends to (C1) by absolute numerator convergence. For the extended source at finite radius, one must keep its exterior forcing rather than simply apply homogeneous Robin conditions there.

Compact source or data support alone gives no dominance of the pole on an interval. In the presence of the Schwarzschild cut and prompt contribution an inverse-contour estimate is still required for any modal dominance or fitting claim. A nonzero finite-cut trace, including the pinned far-front result, gives no nonzero value for this separate linear numerator. For example a compact spacetime change \(\Delta S=\mathcal L Z\), \(Z\) smooth and supported strictly above the slice, has zero pole and zero null trace, by integration against the homogeneous \(w\).

Two independent nonuniqueness statements hold. With unspecified upper radiative data, add a compact homogeneous CPM Cauchy datum supported in an open slice interval with \(u_\Sigma>U\). It affects neither the lower jets nor the homogeneous upper constraints and vanishes on \(u\le U\). Its pairing has full complex rank over real Cauchy data at a nonreal simple pole. If it had only one common phase for all such data, both \(w|_\Sigma\) and \(\mathcal N w|_\Sigma\) would have that phase throughout the open interval. The tangent derivative would too. As \(|\tau'|<1\), this would force \(\partial_t w=-s_qw\) to have the same phase, contradicting \(\Im s_q\ne0\) for nonzero \(w\). For a flat slice one real bump used once as displacement and once as velocity immediately gives the ratio \(s_q\). With fixed data but an unspecified auxiliary causal continuation, arbitrary late real compact projected sources can tune the pole while preserving the physical trace through \(U\). Use a bump with nonzero pairing and its later time translate, with delay such that \(e^{-s_q\Delta t}\) has a distinct phase. If continuations are restricted to tapers of the *same* physical \(S\), the exact dependence is
\[
\Delta C_q=\frac1{W'(s_q)}\int\Delta\chi(u)e^{-s_qu}\!
 \int_{\tau(x)<u+x} e^{-s_qx}h(s_q,x)S(u+x,x)\,dx\,du .
\]
Whether this particular physical sensitivity kernel vanishes in an adjustable taper band remains to be checked. Neither nonuniqueness statement modifies the residue of a fully specified source and preparation.

## 4. All-time nested transforms and the massive outgoing endpoint

The exact shift bookkeeping on a tilted slice uses \(\widetilde X=\mathbf1_{t>\tau(x)}X\) and a two-sided stationary transform of the extension. For a stationary finite-order differential operator \(D\),
\[
\mathbf1_{t>\tau} e^{m\Omega t}D X
=e^{m\Omega t}\{D\widetilde X-[D,\mathbf1_{t>\tau}]X\}.
\]
Time differentiation of \(\widetilde X\) is multiplication by its transform argument, and multiplication by \(e^{m\Omega t}\) shifts that argument by \(-m\Omega\). The commutator is a finite sum of graph distributions with compact radial support for local finite jets of the compact axial data. On a flat slice it reproduces \(\widehat{\partial_t^j X}(s-m\Omega)=(s-m\Omega)^j\widehat X-\sum_{a=0}^{j-1}(s-m\Omega)^{j-1-a}\partial_t^a X(0)\). No transformed pure QNM supplies these Cauchy terms.

At fixed ADM mass the linearized Einstein-frame monopole equations for \(B_0\) have source quadratic in \(H\) and independent of the order-two radion. The mass and lapse radial constraints in spherical Bondi coordinates have stationary radial operators and right sides \(e^{2\Omega u}\) times radial functions. The mass integration constant is zero and the lapse is fixed at infinity, so this representative is proportional to \(e^{2\Omega u}\), or to \(e^{2\Omega t}\) times a different radial profile. For a horizon argument use a regular ingoing EF representative there. The mass aspect is invariant under the order-two orbit change since its background value is constant. Its time flux equation gives \(m_{2,v}=e^{2\Omega v}F_m(r)\) with regular \(F_m\), so \(m_2=e^{2\Omega v}F_m/(2\Omega)\) after the radial constraint and mass matching. The regular EF lapse radial equation also admits a separated representative. Two such separated gauges at fixed areal radius and mass differ by a temporal gauge generator with the same separated time dependence, up to a static gauge choice. This can be seen from the \(tt\) component \((\mathcal L_\xi\bar g)_{tt}=-2f\partial_t\xi^t\) on any finite radial overlap, with the \(tr\) equation fixing a static radial gauge ambiguity. A radial partition on a finite overlap keeps the separation while allowing horizon-regular and Bondi gauges at their respective ends. Lower-order pullbacks of the axial fields, their compact slice terms, and projected sources must be carried with this choice. Raw outgoing Bondi coordinates alone do not control the future horizon.

For an independent sign and analyticity check on the massive intermediate, keep the \(n=+1\) Fourier component and write its current density as \(I^a\) on the product background. Define
\[
d=\lambda+k^2r^2,\quad D=r^2d,\quad L_c=r^4/D,\quad W_c=\lambda r^4/d,\quad
\mathcal O=r^4F_{tx}/f,\quad Y=\mathcal O/\sqrt D,\quad Z=\sqrt{W_c}A_z .
\]
The \(t,x\) density equations give
\[
A_t=(I^t-\mathcal O_x-ik r^4A_{z,t})/D,\qquad
A_x=(-I^x-\mathcal O_t-ik r^4A_{z,x})/D.
\]
Substitute into the curl definition and the \(z\) density equation. With primes \(d/dx\), \(a=(\log\sqrt D)'\), \(b=(\log\sqrt{W_c})'\), the result is the two-by-two system
\[
\begin{aligned}
(-\partial_t^2+\partial_x^2-V_Y)Y+C\partial_tZ
 &=\sqrt D\,[\partial_t(I^x/D)+\partial_x(I^t/D)],\\
(-\partial_t^2+\partial_x^2-V_Z)Z+C\partial_tY
 &=\frac{I^z-ik[\partial_t(L_c I^t)+\partial_x(L_c I^x)]}{\sqrt{W_c}},\\
V_Y&=f(k^2+\lambda/r^2)+a^2-a',\qquad
V_Z=f(k^2+\lambda/r^2)+b^2+b',\qquad
C=2ik\sqrt\lambda f/d .
\end{aligned}
\]
In particular the cross coefficient before normalization is \(ikL_c'\partial_t\) in the \(z\) equation and \(ikD L_c'\partial_t\) in the curl equation. The equations retain \(I^z\) and reconstruct the orbit field without dropping its Gauss equation. The divergence follows from the three sourced density equations. At \(k=0\), \(Y=\psi/\sqrt\lambda\), \(C=0\), and \(V_Y=V_{\rm RW}\), reproducing the mean result. The opposite Fourier mode is its conjugate. In ingoing EF coordinates \((v,r)\), \(\mathcal O=r^4F_{vr}\) and \(A_z\) are regular. The density components satisfy \(I^t=fI^v_{\rm EF}-I^r_{\rm EF}\), \(I^x=I^r_{\rm EF}\), \(I^z=fI^z_{\rm EF}\). Since \(\partial_t=\partial_v,\ \partial_x=\partial_v+f\partial_r\), both projected sources have a factor \(f\) for a regular EF current; this follows by cancellation of the two terms with \(\partial_v I^r_{\rm EF}\). All potential and mixing terms are \(O(f)\) at the horizon.

Let \(s_1=s-\Omega,\ s_2=s-2\Omega\). The formal all-time expression consists of a massless compact-data inverse at \(s_2\), a massive inverse at \(s_1\) applied to the first GL insertion and compact data, then the second GL insertion and the direct \(H^2/B_E\) source in the massless \(s\) pencil. On the outgoing sheet put \(\kappa(s_1)=\sqrt{s_1^2+k^2}\) continued from \(\Re s_1>0,\Re\kappa>0\). At \(s_q r_0\simeq-0.177925-0.747343i\), with \(r_0=1\) for the following numbers,
\[
\begin{aligned}
\kappa&\simeq-0.367495-0.490520i,\\
q+s_2+\kappa&\simeq-0.040486-1.237863i,\qquad
q+s+\kappa\simeq+0.086077-1.237863i .
\end{aligned}
\]
The outgoing factor times the inner source has the candidate leading exponential \(e^{-(q+s_2+\kappa)x}\). The negative real part invalidates the naive unweighted real-axis right-end integral unless the complete physical forcing cancels that exponential. It gives no nonexistence result for the meromorphic continuation. The \(\pi/4\) rotation gives positive real parts \(0.086992\) for \(\kappa e^{i\pi/4}\), \(0.313146\) for \(s_2e^{i\pi/4}\), \(0.846674\) for \((q+s_2+\kappa)e^{i\pi/4}\), and \(q/\sqrt2\) for the GL factor. These arithmetic checks use the literature pole location and the midpoint of the pinned GL enclosure.

The massive diagonal potential has a common Coulomb tail \(k^2-r_0k^2/z+O(\log z/z^2)\), and \(C=O(z^{-2})\). After continuation into the far complex \(z=x\) sector, the outgoing massive matrix Jost solution can be constructed with
\[
E(z)=e^{-\kappa z}z^{\,r_0 k^2/(2\kappa)},\qquad
Y_{\rm out}(z)=E(z)N(z),\qquad
(E^2N')'=E^2R(z)N,\quad R(z)=O(\log z/z^2).
\]
The last equation is a matrix equation with scalar integrating factor. Along the rotated ray \(N=I+\int_z^\infty E(v)^{-2}\int_v^\infty E(w)^2 R(w)N(w)\,dw\,dv\). The \(w-v\) exponential ratio is integrable with any polynomial factor on compact spectral subsets with \(\Re(\kappa e^{i\theta})>0\), and the outer integral has norm \(O(\log |z|/|z|)\). This yields the analytic two-dimensional outgoing subspace near the target. The same Coulomb construction applies to the pinned GL master using its rational analytic potential and the positive real \(q\), with rational reconstruction on the sufficiently far sector. The far radial constraints for \(B_E\) integrate decaying separated \(H^2\) coefficients and admit the same far-sector continuation. The compact slice transform can be smooth and nonanalytic in \(x\); deform only beyond its support where the baseline massless response is the outgoing homogeneous solution. The inner rotated source integral then converges on the same local sector and equals the physical right-half-plane prescription in an overlap, by analytic contour deformation and decay. Transport its value back from the finite matching point. For the outer real-axis integral use the real-axis forward ODE from that matching point. The continued massive field is bounded by a polynomial times \(e^{|\Re\kappa|x}\) because the unmultiplied inner source there already decays as \(e^{-\Re(q+s_2)x}\) times a polynomial. The positive \(q+\Re(s+\kappa)\) margin suffices for the outer \(HJ\) projection at the target. The \(H^2/B_E\) term has the easier \(2(q-\Omega+\Re s)>0\) margin. At the horizon the compact baseline data give a regular ingoing branch \(e^{s_2 x}\), the GL factor adds \(e^{\Omega x}\), and the regular projected current adds \(f\). The corresponding sufficient inner convergence bound is \(1/r_0+2\Re s_1>0\), and outer \(1/r_0+2\Re s>0\). These estimates are local near the fundamental and presume the horizon-regular representative.

This is an endpoint continuation recipe conditional on a meromorphic path from a common physical half-plane, regular constrained completion, and the shifted homogeneous spectral determinants. Along a horizontal path at the target imaginary part the shifted baseline pole occurs already at \(s=s_q+2\Omega\), requiring a meromorphic detour or treatment. A massive resonance at \(s_1\) or another baseline pole at \(s_2\) can change the Laurent order and must be checked. The Coulomb and endpoint bounds establish no quantitative determinant bound, absence of such coincidences, simple-pole certification, or nonzero full source-plus-data overlap. They also do not establish dominance or a frequency shift. A different constant comparison mass would add a stationary family derivative and its possible double-pole term, which is excluded by the fixed-mass convention in this derivation.

## Evidence and stopping point

- **Derived for fixed-order jets**: completed mean projection, graph Cauchy signs, finite-cut causality, arbitrary finite-strip source falloff, windowed null trace, and the bilinear local residue formula at a simple isolated zero. Full complex-rank variation from unconstrained *free radiative* upper data refers to data reconstructed to satisfy the homogeneous upper constraints. Arbitrary late auxiliary scalar source changes refer to the diagnostic equation.
- **Derived endpoint criterion, conditional spectral application**: exact massive axial two-by-two reduction, Coulomb far contour construction, real-axis growth and convergence margins for the proposed all-time nested transform. Numerical inputs for the endpoint margins are the published frequency and pinned GL enclosure. The supplemental two-point massive determinant diagnostic is separately documented below.
- **Unresolved**: verified spectral continuation path and noncoincidence for the exact shifted resolvents, evaluation and error enclosure of the complete physical \(HJ+H^2P+B_EP\) and Cauchy pairings for a declared compact strong-field pulse, and a finite-cut signal decomposition controlling prompt, cut and pole contributions. Neither the finite-window continuation nor the all-time formal jet supplies a nonlinear finite-amplitude Bondi or ringdown theorem.

### Path and stability checks

The endpoint conditions above hold along a narrow horizontal spectral strip from a common physical right half-plane to a neighborhood of the reported pole, with the discrete determinant zeros treated meromorphically. For fixed \(s=a-i b_\omega\), \(b_\omega\simeq0.747343/r_0>k\), write \(\kappa=\alpha-i\nu\) with \(\nu>0\) on the outgoing continuation. Then \({\rm sign}\,\alpha={\rm sign}(a-\Omega)\), \(d\alpha/da=\Re[(s-\Omega)/\kappa]>0\), and \(\nu\ge\sqrt{b_\omega^2-k^2}\). The latter follows by squaring the explicit discriminant \((A^2-b_\omega^2+k^2)^2+4A^2 b_\omega^2\), \(A=a-\Omega\). It implies \(\alpha+\nu>0\) throughout the strip at and to the right of the target, because \(-0.367495+\sqrt{0.747343^2-(2\pi/10)^2}>0\). Therefore the \(\pi/4\) endpoint rotation works across the massive continuous-spectrum line and the massless shifted imaginary axis. For \(a\le\Omega\), the outer real-axis test \(q+\alpha-|a|>0\) follows from monotonicity and the target margin on \(a\le0\), and from \(q+\alpha(a_{\rm target})-\Omega>0\) on \(0\le a\le\Omega\). For \(a>\Omega\) the real-axis massive outgoing Green split bounds the intermediate by a decay rate \(\min(\alpha,q+a-2\Omega)-\delta\) for arbitrary small positive \(\delta\). The same discriminant gives \(\alpha\ge a-\Omega\) there, so the outer margin \(q+\min(\alpha,q+a-2\Omega)-a\) is at least \(\min(q-\Omega,2q-2\Omega)>0\). The horizon tests strengthen toward the right. These endpoint conditions concern meromorphic continuation with the exact shifted poles included. Actual evaluation at the target still requires a determinant and Laurent-order check.

The massive diagonal spatial operator \(\mathcal M=-\partial_x^2+\operatorname{diag}(V_Y,V_Z)\) is nonnegative. Its two derivative terms factor as \((-\partial_x+a)(\partial_x+a)\) and \((\partial_x+b)(-\partial_x+b)\), in addition to \(f(k^2+\lambda/r^2)\) in both rows. The mixing \(\mathcal C=C\sigma_1\) is anti-Hermitian. For an outgoing/ingoing decaying mode in \(\Re s_1>0\), divide \((s_1^2+\mathcal M-s_1\mathcal C)U=0\) by \(s_1\) and take the real quadratic form to rule it out. On \(s_1=-i\omega\) with \(\omega>k\) the stationary matrix is real symmetric. The constant current \(\operatorname{Im}(U^\dagger U')\) is \(-\omega\|A_{\rm hor}\|^2\) at the left end and \(+\sqrt{\omega^2-k^2}\|A_{\rm out}\|^2\) at the right end, so there is no pole on that crossing either. These tests leave the shifted damped target open.

Manufactured independent algebra control for the massive equations used Python 3/SymPy with \(r_0=1,k=3/5,\lambda=4,\partial_t=7/10\) acting on the manufactured common time factor, \(A_t=1+r^2,A_x=r^3,A_z=2-r^2\) and \(\partial_x=f\,d/dr\). Form \(\mathcal O\) and the three \(I^a=E_0^a[A]\) directly from the pinned Proca densities, substitute into the two displayed normalized equations, and simplify both left minus right residuals. Both outputs were exact zero. This tests signs and coefficients of the forced reduction on manufactured fields, separately from the analytic derivation. The manufactured algebra check involved no numerical ODE integration.

### Supplemental two-point massive determinant diagnostic

Run `python3 GLR-GOAL-011/checks/massive_match.py` with Python 3.14.8, NumPy 2.4.6 and SciPy 1.18.0. This integrates just the homogeneous massive 2-by-2 ODE. In units \(r_0=1\), the match is at \(r=2.5\). The left basis uses \(e^{s_1x}\) times a first-order Frobenius expansion starting at \(r=1+\delta\), integrated in \(\log(r-1)\). The right basis factors \(e^{-\kappa x(r)}r^{k^2/(2\kappa)}\) and integrates inward along \(r=2.5+e^{i\pi/4}a\). The endpoints \((\delta,a)\) run over \((10^{-3},24),(10^{-4},48),(10^{-5},96),(10^{-6},144)\). Orthonormalize each two-dimensional subspace of matching values and \(x\)-slopes, and form the combined four-by-four column matrix. At \(s_1=-0.24120619-0.747343i\) the finest endpoint run gives determinant modulus \(0.1164150552\) and smallest singular value \(0.0881836155\). The only variation, \(s_1+0.003\), gives \(0.1166090480\) and \(0.0878475128\). Tightening DOP853 relative tolerance from \(2\times10^{-10}\) to \(2\times10^{-12}\) leaves the displayed final digits at fixed finest endpoints. The complete endpoint series is in [the saved output](../checks/massive_match.txt). This is numerical evidence against a massive coincidence at the sampled arguments. The precise continued subspaces, exact target pole and an enclosure in frequency remain unverified. No source overlap was computed.
