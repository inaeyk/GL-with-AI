# The scalar energy cost of changing the Z preparation

## Result and hypotheses

Use the dimensionless Schwarzschild coordinates of [GOAL-005](../GLR-GOAL-005/RADIATION.md), the completed Einstein-frame monopole \(\psi=r\phi_2\), and the graph \(\Sigma:v=R(u)\), where \(u=g(r)=-r-2\log(r-1)\). Keep the first GL jet, asymptotic circumference and ADM mass fixed. Add smooth compactly supported scalar Cauchy data \(F,P\) in the [exact original GOAL-005 constraint family](../GLR-GOAL-005/CONSTRAINTS.md). At quadratic order their difference from Z solves
\[
 h_{tt}-h_{xx}+V_0h=0,\qquad V_0=f/r^3,\qquad h=r\,\delta\phi_2 .
\]
The order-two \(zz\) source is unchanged because it is quadratic in the common first jet. In the adapted coordinate, \(F,P\) prescribe the completed scalar and its normal derivative. Their lift uses the adjustable inner leaf and the \(C^1\) ADM mass map, with \(\partial_\lambda(M/M_0)=9/14\). The inherited construction gives a tuned family near zero seed for each fixed smooth pair. The required size of that neighborhood can depend on the pair. A compact jet that reaches the horizon can be continued smoothly to the inner collar and tapered there.

The two-parameter local development on each fixed compact region is smooth before restricting to the tuned inner parameter \(\lambda(\epsilon)\). The pure \(\lambda\) family at \(\epsilon=0\) is a circle-independent vacuum product with constant radion. Thus its linear \(\lambda\) derivative in the scalar trace vanishes, and \(\lambda(\epsilon)=o(\epsilon)\) from the mass map suffices for the order-two scalar coefficient there. For the pinned Z pair GOAL-006 also derives \(\lambda=O(\epsilon^2)\) from the mean constraint equations. The mixed \(\epsilon\lambda\) and quadratic \(\lambda^2\) terms are \(o(\epsilon^2)\). This is an iterated perturbative and large-radius calculation. Uniform differentiability of the tuned development up to the actual null boundary is a separate premise. [GOAL-006 charge-rate estimate](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md#1-original-constraint-family-and-the-tuned-mass-rate)

Define the dimensionless homogeneous preparation energy \(\mathcal E_\Sigma\) by the scalar Killing energy on the exterior portion of \(\Sigma\) with normalization \(r_0/G_4\) removed. In physical units its leading fraction of \(M_0=r_0/(2G_4)\) is \(2\epsilon^4\mathcal E_\Sigma\). This is the energy of the added homogeneous degree of freedom on the background. The ADM mass of the full constrained data is held fixed by tuning.

Let \(I=[-10,-8]\), \(N_h=\lim_{v\to\infty}h_u\), \(b_Z=\|n_Z\|_{L^2(I)}\), and let \(\mathcal E_{\rm can}\) be the infimum of \(\mathcal E_\Sigma\) over these smooth preparations with \(n_Z+N_h=0\) on \(I\). Then
\[
 \boxed{b_Z^2\ \le\ \mathcal E_{\rm can}\ <\ 1.20\,b_Z^2.} \tag{1}
\]
More generally, the set of quadratic news realizable on \(I\) includes every smooth perturbation of \(n_Z\) that extends smoothly to a padded interval. Every preparation in the class obeys
\[
 2\big(b_Z-\sqrt{\mathcal E_\Sigma}\big)_+^2
 \ \le\ \mathcal K(F,P)\ \le\ 2\big(b_Z+\sqrt{\mathcal E_\Sigma}\big)^2 . \tag{2}
\]
For example, when \(2\mathcal E_\Sigma\le 3.5316\times10^{-7}/4\), the quadratic energy coefficient is at least \(3.5316\times10^{-7}/4\). The same energy norm bounds the \(L^2(I)\) change of waveform. The theorem permits arbitrary smooth incoming and outgoing jets for the lower bound. The near-minimum upper bound uses outgoing jets in the relevant early domain and an inward continuation of the value on the Cauchy graph.

## Lower bound from a positive characteristic balance

For \(e=(h_t^2+h_x^2+V_0h^2)/2\), \(j=-h_t h_x\), the homogeneous equation gives \(\partial_t e+\partial_xj=0\). Hence \(e\,dx-j\,dt\) is closed. The Einstein-frame physical density and current in the same units are obtained by replacing \(h_x\) in the kinetic terms with \(h_x-\beta_R h\) and dropping \(V_0h^2\), where \(\beta_R=f/r\). Since \(V_0=\partial_x\beta_R+\beta_R^2\), the difference of the physical and master one-forms is \(-d(\beta_R h^2)/2\). At the large-radius and horizon endpoints of \(\Sigma\) the boundary term vanishes by compact support and horizon regularity, respectively. Thus \(\mathcal E_\Sigma=\int_\Sigma(e\,dx-j\,dt)\) on the exterior, oriented by increasing \(x\).

Write \(t=T(x)=-\log(r-1)\) on \(\Sigma\). Its slope is \(p=T'=-1/r\). In the null coordinates \(u=t-x,\ v=t+x\) the closed one-form is
\[
 -(h_u^2+V_0h^2/4)\,du+(h_v^2+V_0h^2/4)\,dv .
\]
The corresponding integrands on the upper \(v\)-cap and right \(u\)-ray are nonnegative. Close a finite wedge by the initial graph, the cap, and the ray \(u=-8\), with a left outgoing ray earlier than the support of the data. Its field is zero by causality. The graph integrand in the \(x\) orientation is
\[
 (1-p)h_u^2+(1+p)h_v^2+V_0 h^2/2.
\]
The total on the graph dominates its portion in the wedge. The cap bound therefore gives \(\int_I h_u(u,v)^2du\le\mathcal E_\Sigma\) at sufficiently large cap radius. The GOAL-005 retarded integral with compact free data and the integrable curvature ray kernel gives the limit on \(I\). Fatou's lemma yields \(\|N_h\|_2^2\le\mathcal E_\Sigma\). Combining with \(N_h=-n_Z\) proves the lower bound in (1), and the two triangle inequalities prove (2).

## Near-minimum cancellation and arbitrary waveform control

On a padded interval \(J=[a,b]\), \(a<-10\), \(b\ge-8\), prescribe the initially outgoing scalar pair
\[
 h|_\Sigma=A(u),\qquad h_u|_\Sigma=A',\qquad h_v|_\Sigma=0,\qquad A=0\ \text{for }u\le a .
\]
On the exterior graph the future normal is \((\partial_t+p\partial_x)/\sqrt{f(1-p^2)}\), \(p=-1/r\). Thus the free pair in the constraint construction is
\[
 F=A/r,\qquad
 P=\frac{(1-p)A'/r-pfA/r^2}{\sqrt{f(1-p^2)}} .
\]
Let \(L_J=b-a\), \(k_b=1/(4R(b)^2)\), and \(M(u)=\sup_{v\ge R(u)}|h(u,v)|\). The free solution is \(A(u)\). The characteristic equation \(4h_{uv}=-V_0h\) and the exact ray integral \(\int_{R(u)}^\infty V_0\,dv/4=1/(4R(u)^2)\) give
\[
 M(u)\le\int_a^u |A'(s)|\,ds+k_b\int_a^u M(s)\,ds
 \le \int_a^u e^{k_b(u-s)}|A'(s)|\,ds.
\]
Consequently \(N_h=(1+\mathcal P)A'\) with
\[
 \|\mathcal P\|_{2\to2}\le
 \delta_J=k_b e^{k_b L_J}\frac{2 L_J}{\pi}. \tag{3}
\]
Here \(2L_J/\pi\) is the norm of integration from the left endpoint, by the first Dirichlet–Neumann eigenvalue of \(-\partial_u^2\) on \(J\). The graph and first characteristic contact follow from the homogeneous integral equation. Smooth outgoing data give a smooth trace. Conversely, the Volterra inverse on a smooth target flat near \(a\) gives a smooth \(A'\). One can see this by differentiating the ray integral for \(\mathcal P A'\) locally: the lower endpoint uses \(A\), and the differentiated potential and \(h_u\) are ray integrable. The retarded equation gives \(h_u=A'-\int_{R(u)}^v V_0 h/4\). Iterating this differentiation with the smooth graph and \(r^{-3}\) potential gives \(\mathcal P:C^j\to C^{j+1}\) locally on \(J\). The same bounds give a continuous first derivative of the integrated \(L^2\) solution, then bootstrap the inverse. Alternatively one can invert in \(C^0\) first using the corresponding bound \(k_b L_J e^{k_b L_J}<1\) and use \(L^2\) uniqueness.

To cancel, take the target \(-\chi n_Z\) with \(\chi=1\) on \(I\), \(0\le\chi\le1\), and \(\chi=0\) near \(a\). For an arbitrary smooth trace change replace \(-n_Z\) by its extension. In each case \(\|A'\|_{L^2(J)}\le (1-\delta_J)^{-1}\|\chi n_Z\|_2\) for the cancellation target. Causality permits a change of jets after the window with no effect on the cancellation.

For outgoing data on \(J\) the derivative part of the graph energy is exactly \(\int_J (A')^2du\), since \((1-p)dx=-du\). The potential part is at most
\[
 \frac{L_J}{4}\big(R(b)^{-2}-R(a)^{-2}\big)\|A'\|_{L^2(J)}^2,
\]
using \(\sup |A|^2\le L_J\|A'\|_2^2\) and \(V_0dx=dr/r^3\). After \(b\) choose the regular static continuation \(h=A(b)\,r/R(b)\) and \(h_t=0\) on the exterior graph. This solves the static master equation because \(\partial_x^2r=V_0r\), and sets \(\delta\phi_2=A(b)/R(b)\) on that portion. Its graph energy is
\[
 \frac12\int_{-\infty}^{x(R(b))}\big(h_x^2+V_0h^2\big)dx
 =\frac{f(R(b))}{2R(b)}\,A(b)^2
 \le \frac{L_J f(R(b))}{2R(b)}\|A'\|_2^2 . \tag{4}
\]
It is regular through the future horizon. The seam can be made smooth strictly after \(b\) with arbitrarily small excess energy. For fixed padded data extend \(A,A'\) smoothly a little beyond \(b\), blend the value with the static profile using a smooth cutoff over a length \(\eta\), and blend the independent \(h_t\) data. The two values agree at the seam, their difference is \(O(\eta)\) throughout the blend, and their first derivatives stay bounded as \(\eta\to0\). Thus the energy cost on the blend is \(O(\eta)\) for each fixed padded pair. This limit controls a first-derivative energy. It gives no common higher-regularity neighborhood across the sequence.

Let \(a\uparrow-10,\ b\downarrow-8\) and use \(\|\chi n_Z\|_2\to b_Z\). Equations (3)–(4) yield
\[
 \frac{\mathcal E_{\rm can}}{b_Z^2}
 \le
 \frac{1+\frac{L}{4}(R_b^{-2}-R_a^{-2})+\frac{L f(R_b)}{2R_b}}
 {(1-k_b e^{k_b L}2L/\pi)^2},\quad
 L=2,\quad R_a=R(-10),\quad R_b=R(-8).
\]
The rational check in [checks/preparation_bounds.py](checks/preparation_bounds.py) uses \(R_b>5.1,\ R_a<6.6\), \(2L/\pi<32/25\), and outward rational exponential bounds. It gives \(\delta_I<0.013\) and the displayed ratio \(<1.20\). The estimates concern the infimum. Every strict threshold larger than the upper estimate is attained or improved by a smooth compact preparation with exact cancellation.

## Relation to finite-amplitude radiation

Using the inherited directed \(\mathcal K_Z\) interval gives
\[
 \sqrt{3.5316\times10^{-7}/2}< b_Z<\sqrt{4.0171\times10^{-7}/2}.
\]
If a preparation has a common-clock finite-amplitude estimate
\(\|n_\epsilon-\epsilon^2(n_Z+N_h)\|_2\le C_I\epsilon^4\),
combine (2) with the exact scalar flux law to obtain
\[
 2|\epsilon|^4\big(b_Z-\sqrt{\mathcal E_\Sigma}-C_I\epsilon^2\big)_+^2
 \le E_I/M_0\le
 2|\epsilon|^4\big(b_Z+\sqrt{\mathcal E_\Sigma}+C_I\epsilon^2\big)^2 .
\]
For a particular preparation with news norm \(b_P\), the leading energy approximation has absolute error at most \(4b_P C_I|\epsilon|^6+2C_I^2|\epsilon|^8\). The homogeneous energy bound is unconditional for the order-two problem. The finite-amplitude comparison uses the stated uniform exterior estimate and geometric trace hypotheses. Damped Z and original Z share the same order-two Z trace. Their higher jets and end classes must be tracked separately in that estimate.
