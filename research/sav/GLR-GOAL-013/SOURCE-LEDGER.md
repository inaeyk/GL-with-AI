# Axial source and contact ledger

The preparation and lower geometry are the pinned GOAL-011 construction at the GOAL-012 source commit [87fd711bf2e8117467caad1ff52c1cd344966974](https://github.com/inaeyk/GL-with-AI/commit/87fd711bf2e8117467caad1ff52c1cd344966974). Use \(r_0=1\), \(x=r+\log(r-1)\), \(u=t-x\), \(v=t+x\), \(f=1-1/r\), \(\lambda=4\) and \(p=T'=-1/r\). The real seed multiplies all axial fields. The growing GL mode is the unhalved \(e^{\Omega t}H(r)\cos(kz)\). We evolve the actual \(+k\) coefficient, so the first insertion contains \(1/2\). Real-field averaging pairs it with its conjugate.

## Geometric coefficients and independent extraction

For the axial angular coefficients \(h_a X_A\), \(2h_T X_{AB}\), set \(A_a=h_a/R^2-\partial_a(h_T/R^2)\) and \(F=dA\). An axial diffeomorphism adds \(d\Lambda\) to each of the two terms in \(A\). The exact orbit densities are \(P^{bacd}=\sqrt{|b|}R^4 b^{bc}b^{ad}\), \(M^{ab}=\sqrt{|b|}R^2b^{ab}\). Use the action \(-\sqrt{|b|}[R^4F^2/4+\lambda R^2 A^2/2]\) for the ordered axial equation. Varying \(A\) gives \(\partial_b(PF)^{ba}=\lambda M^{ab}A_b\), whose divergence gives the Gauss relation. The action reduction on the spherical vacuum solution and the exact inverse-metric and determinant coefficients are pinned in [GOAL-011 HIERARCHY, (1)–(4)](../GLR-GOAL-011/HIERARCHY.md). We independently expand its relevant two-dimensional blocks below. Write \(Q_z=(PF)^{tx}\), the noncompact density curl, in the product orientation. The baseline curl has \(Q_{0z}=-r^4F_{0,tx}/f\).

The shear normalization also follows directly in an asymptotically inertial Bondi gauge. Put \(C_{AB}=C X_{AB}\), with \(g_{AB}=r^2\gamma_{AB}+r C_{AB}+O(1)\). The Bondi radial constraint fixes the leading axial \(g_{uA}\) to \(\tfrac12 D^BC_{AB}=-\lambda C X_A/4\), and \(g_{rA}=0\). The gradient of \(h_T/R^2\) cancels from \(F\), so \(F_{ur}=-\lambda C/(2r^3)+O(r^{-4})\) and \(2Q_z/(\lambda r)\to C\). Here \(du\wedge dr=dt\wedge dr\), so the sign is fixed. At returning order, the compact products in the density and conformal conversion decay on fixed cuts by the GL-weighted mixed estimates in [B](investigators/B-SOURCE.md). The mean Einstein-frame radius and mass are fixed. Bondi news is the derivative of this shear in the physical Bondi clock.

## Term-by-term accounting

| Order | Contribution | Where it enters | Certification role |
|---|---|---|---|
| \(\eta\), zero mode | \(L_{\rm RW}w_0=0\), \(w_0|_\Sigma=a_\delta\), \((w_{0,t},w_{0,x})|_\Sigma=(a_\delta',-a_\delta')\) | \(A_{0t}=(w_{0,x}+fw_0/r)/(2r)\), \(A_{0x}=w_{0,t}/(2r)\), \(Q_{0z}=\lambda r w_0/2\) | Baseline graph, bulk and commuted trace errors propagate to both insertions |
| \(\epsilon\eta\), \(+k\) and \(-k\) | \(J^a=\lambda(M_1 A_0)^a_{+k}\), \(X=(P_1 F_0)^{tx}_{+k}\) | Both massive equations, \(J^z=0\) in the compact-unitary profile, compact unknown evolved | First-insertion coefficient, lower-field error and mixed graph and bulk errors |
| \(\epsilon\eta\), constraints | \(U_m|_\Sigma=A_z|_\Sigma=0\), forced velocities below | Normal and tangential equations, \(\pi^z=0\) fixes the compact velocity relative to \(A_t+pA_x\) | Include exact first and commuted graph jets, even though the displacement is zero |
| \(\epsilon^2\eta\), mean | \(\langle P_1F_1\rangle,\ \lambda\langle M_1 A_1\rangle\) | Exact curl and both strong current derivatives. Reconstruction contains \(A_z\) and \(U_m\), including the affine \(J,X\) products | Mixed feedback, mixed residual and source reconstruction |
| \(\epsilon^2\eta\), mean | \(\langle P_{2,\mathrm{quad}}F_0\rangle,\ \lambda\langle M_{2,\mathrm{quad}}A_0\rangle\) | Direct quadratic inverse, volume and sphere weights, with \(\langle\cos^2 kz\rangle=1/2\) | GL profile and source assembly |
| \(\epsilon^2\eta\), mean | \(\langle P_{2,\mathrm{mean}}F_0\rangle,\ \lambda\langle M_{2,\mathrm{mean}}A_0\rangle\) | Fixed-mass EF \(\mu,\beta\) and their derivatives | Real-axis mass/lapse quadrature and its outer tail |
| \(\epsilon^2\eta\), constraints | \(Q_{2z}|_\Sigma=0\), \((1-p^2)Q_{2z,t}|_\Sigma=\mathcal D^x-p\mathcal D^t\) | Complete induced mixed field on the graph and weak/strong contact cancellation | Returning graph and source residual |
| \(\epsilon^2\eta\), other sectors | The spherical \(n=\pm2\) field with the \(n=0\) baseline stays \(n=\pm2\). Pure mean radion factors cancel from noncompact \(P,M,Q_z\) | Reflection gives zero mean compact axial potential. The half-circle translation with \(\epsilon\mapsto-\epsilon\) removes odd GL orders of the returned mean | KK transport and radion flux are separate observables |

The mean radion cancellation can be checked directly. In the block-diagonal zero-mode substitution \(b_{\mu\nu}=e^{-2\phi/\sqrt3}b_{E,\mu\nu}\), \(b_{zz}=e^{4\phi/\sqrt3}\), \(R=e^{-\phi/\sqrt3}r\), the orbit volume factor is \(\sqrt{|b_E|}\). One and two noncompact inverse metrics then cancel the \(R^2\) and \(R^4\) weights respectively. The actual mean correction remaining in the noncompact density uses \(R=r\) and \(\Delta b_{E,vv}=2\mu/r-2f\beta\), \(\Delta b_{E,vr}=\beta\). The fixed-\(v\) radial integrals are \(\mu=\tfrac12\int_r^\infty \rho^2\mathcal T^v{}_v\,d\rho\) and \(\beta=-\tfrac12\int_r^\infty \rho\mathcal T_{\rho\rho}\,d\rho\). Their upper endpoint constants implement the fixed ADM mass and clock. The direct quadratic term still uses \(R^2=r^2+\epsilon H_{\theta\theta}\) through its first variation, so in the expansion of \(R/r\) its quadratic profile is \(e=-d^2/2\), \(d=H_{\theta\theta}/(2r^2)\). The separate mean Einstein-frame areal correction contributes no \(e\).

## Massive reconstruction and graph

Write \(d_k=\lambda+k^2r^2,\ D=r^2d_k,\ W=\lambda r^4/d_k,\ L_c=r^4/D\). The total mixed density \(-U_m=Q_{1z}\) includes \(X\). The \(t,x\) equations of the exact hierarchy give
\[
 D A_t=J^t-U_{m,x}-ik r^4 A_{z,t},\qquad
 D A_x=-J^x-U_{m,t}-ik r^4 A_{z,x},\qquad
 F_{1,tx}=f(U_m+X)/r^4.
\]
The term \(ik r^4 A_{z,t}\) comes from \(\partial_z(P_0F_1)^{zt}\), so it cannot be dropped on account of \(A_z|_\Sigma=0\). Substituting the reconstructions into \(F_{1,tx}=\partial_t A_x-\partial_xA_t\) cancels \(A_{z,tx}\) and leaves the coefficient \(ikL_{c,x}\) before rescaling. For the compact equation use
\[
 -\partial_t[r^4(A_{z,t}-ikA_t)]+\partial_x[r^4(A_{z,x}-ikA_x)]
 =\lambda f r^2 A_z+J^z .
\]
The second mixed derivatives of \(U_m\) cancel after substitution. With \(y=U_m/\sqrt D,\ z_c=\sqrt W A_z,\ a_m=(\log\sqrt D)_x,\ b_m=(\log\sqrt W)_x\) this yields
\[
\begin{split}
 (-\partial_t^2+\partial_x^2-K-a_m^2+a_{m,x})y+C z_{c,t}
 &=\sqrt D\{\partial_t(J^x/D)+\partial_x(J^t/D)\}+f\sqrt D X/r^4,\\
 (-\partial_t^2+\partial_x^2-K-b_m^2-b_{m,x})z_c+C y_t
 &=\{J^z-ik[\partial_t(L_cJ^t)+\partial_x(L_cJ^x)]\}/\sqrt W,\\
 K&=f(k^2+\lambda/r^2),\qquad C=2ik\sqrt{\lambda}f/d_k.
\end{split}
\]
The time-coupling matrix \(C\sigma_1\) is anti-Hermitian and the stationary spatial energy is the sum of the covariant derivative squares and \(K\). Recovering all density equations also recovers their Gauss relation. At \(k=0\), the \(y\) row has the Regge–Wheeler potential and no time coupling.

The graph-adapted electric density is \((PF)^{ti}-p(PF)^{xi}\). Its mixed entries are \(\pi^x=-U_m\) and
\(\pi^z=-r^4[(\partial_t+p\partial_x)A_z-ik(A_t+pA_x)]\). The latter has no \(P_1F_0\) correction because the unperturbed compact curl and \(H_{za}\) vanish. Enforcing the canonical zero upper graph displacement and tangential potential first gives \((\partial_x+p\partial_t)(U_m,A_z)=0\), \(A_x+pA_t=0\). Imposing \(\pi^z=0\) and the two reconstruction equations then gives
\[
 U_{m,t}|_\Sigma=(pJ^t-J^x)/(1-p^2),\qquad
 A_{z,t}|_\Sigma=ik(J^t-pJ^x)/[\lambda r^2(1-p^2)],\qquad
 (A_t+pA_x)|_\Sigma=(J^t-pJ^x)/(\lambda r^2).
\]
The cancellation \(D-k^2r^4=\lambda r^2\) matters in the compact velocity and normal constraint.

## Returning equation and weak representation

Set
\(\mathcal D^a=\lambda\langle M_1^{ab}A_{1b}+M_2^{ab}A_{0b}\rangle\) and
\(\mathcal C=\langle P_1F_1+P_2F_0\rangle^{tx}\).
Then \(Q_{2z}=-r^4\langle F_{2,tx}\rangle/f+\mathcal C\),
\(\lambda r^2 A_{2t}=Q_{2z,x}+\mathcal D^t\) and
\(\lambda r^2 A_{2x}=Q_{2z,t}-\mathcal D^x\).
For \(w_2=2Q_{2z}/(\lambda r)\), define \(P_e=r^{-2}\mathcal D^x\), \(Q_e=r^{-2}\mathcal D^t\) in \((t,x)\). Taking the curl gives
\[
 L_{\rm RW}w_2=S_2
 =\frac{2r}{\lambda}[-\partial_tP_e-\partial_xQ_e-\lambda f r^{-4}\mathcal C].
\]
The tangential potential constraint implies the velocity in the ledger. Using a background curl instead subtracts \(2\mathcal C/(\lambda r)\) from the master, its entire graph jet and its RW operator from the source.

For \(\Theta=\mathbf1_{t\ge T(x)}\), the graph contact of \(L_{\rm RW}(\Theta w_2)\) is \(-(1-p^2)w_{2,t}|_\Sigma\delta(t-T)\), using zero displacement and \(w_{2,x}=-p w_{2,t}\) there. On the other hand
\[
 -\Theta(\partial_tP_e+\partial_xQ_e)
 =-\partial_t(\Theta P_e)-\partial_x(\Theta Q_e)+(P_e-pQ_e)|_\Sigma\delta(t-T).
\]
The last term cancels the graph contact exactly after multiplying by \(2r/\lambda\). The baseline has nonzero displacement, so its separate step terms are required. In particular \(\Theta\partial_a Q_0=\partial_a(\Theta Q_0)-(\partial_a\Theta)Q_0\), with the opposite signs for \(a=t,x\) prescribed by the tilt.

To see the retarded source-plus-Cauchy formula, use the scalar Green kernel \(L_{\rm RW}G_R(z,z')=\delta(z-z')\) with forward support in \(z\) from \(z'\). The exact returning field is
\[
 w_2(z)=\int_{t'\ge T(x')}G_R(z,z')S_2(z')\,dt'\,dx'
 -\int G_R(z,(T(x'),x'))(1-p^2)w_{2,t}|_\Sigma(x')\,dx'.
\]
Indeed the extension vanishes to the past of the graph and uniqueness fixes the homogeneous freedom. For \(V=0\) the normalization is \(G_R=-\tfrac12\mathbf1_{t-t'\ge|x-x'|}\), which follows by applying \(-4\partial_u\partial_v\) and using \(dt\,dx=du\,dv/2\). A nonzero contact therefore contributes even if the strong bulk source vanishes. As a local free control, use a straight graph \(T=px\), constants \(P_e=1,Q_e=0\), and the scalar source with common prefactor set to one. The exact graph-zero solution in the future is \(w=(t-px)/(1-p^2)\). Its extension has \(-\delta(t-px)\) under the free operator and the weak form gives exactly \(-\partial_t\Theta=-\delta(t-px)\).

At either upper order \(j=1,2\), the weak vector equation can also be formed before scalar reduction. Let \(G_j=\sum_{m=1}^j P_mF_{j-m}\) and \(B_j^a=\sum_{m=1}^j M_m^{ab}A_{j-m,b}\), with the mean projection at \(j=2\). The higher tangential pullback vanishes, so \(d(\Theta A_j)=\Theta F_j\). The exact higher electric coefficient vanishes, so \((\partial_b\Theta)(P_0F_j+G_j)^{ba}=0\). Consequently
\[
 E_0[\Theta A_j]^a=-\partial_b(\Theta G_j^{ba})+\lambda\Theta B_j^a .
\]
This is the weak contact cancellation for the full mixed vector, including the compact equation, and for the mean vector. It does not impose zero upper normal potentials or zero induced velocities.

For the baseline scalar a general graph pair \(\phi=w|_\Sigma,\psi=w_t|_\Sigma\) gives
\[
 L_{\rm RW}(\Theta w)-\Theta L_{\rm RW}w
 =[-(1-p^2)\psi-2p\phi'-p'\phi]\delta(t-T)+(p^2-1)\phi\,\delta'(t-T)
\]
with graph-total \(\phi'\). Inserting \(\phi=a_\delta(T-x)\), \(\psi=a_\delta'(T-x)\), this is \([(1-p)\phi'-p'\phi]\delta-(1-p^2)\phi\,\delta'\). The transform against \(e^{-\sigma t}\) is thus \(e^{-\sigma T}[(1-p)\phi'-(p'+\sigma(1-p^2))\phi]\). This independently recovers the compact baseline launch source. Its nonzero contacts must be used even when the two upper vector equations use the contact-cancelled weak form.

With smooth bilinear test \(\varphi=r w^\dagger\), the weak bulk pairing integrates \((2/\lambda)(\varphi_tP_e+\varphi_xQ_e-\lambda f r^{-4}\varphi\mathcal C)\). On a right-hand subdomain \(x\ge x_a\) this replacement also contributes \(+(2/\lambda)\varphi Q_e\) at \(x_a\), and the opposite sign at a right endpoint. At other interfaces the same rule applies to the outgoing normal contraction of \((P_e,Q_e)\). Continuous physical values cancel on opposite faces, while a piecewise reconstruction must use its two traces or bound the jump. For example on \(1\le x\le2\), \(\varphi=1\), \(P_e=0,Q_e=x\), the strong derivative integrates to \(-1\). The weak bulk derivative vanishes, and the two boundary values give \(+1-2=-1\). Dropping the left face would give \(-2\).

In regular \((v,r)\) density variables let \(V_e=r^{-2}\mathcal D^v\). Then \(Q_e=fV_e-P_e\) and \(-\partial_tP_e-\partial_xQ_e=f[\partial_r(P_e-fV_e)-\partial_vV_e]\). This exact cancellation gives the regular strong collar evaluation. A collar kept strong and an exterior treated weakly still need the finite interface above. For finite cuts the characteristic domain ends at \(r_\Sigma(U)>1\), so all axial data further down the horizon collar are causally excluded. This exclusion applies to axial inputs. The nonlocal fixed-mass mean integrals are still part of the prescribed coefficients.

## Error propagation

For the strong calculation bound the graph jet and the smooth whole-cell residual of each stage. Feed the baseline errors through the two \(J,X\) maps and the direct and mean return. Feed the mixed error through both reconstructed polarizations. Bound coefficient errors on the candidate separately from the exact coefficients on field errors. A weak calculation must also account for interface traces in the same causal error norm. The explicit exterior bound in [B](investigators/B-SOURCE.md) controls the true \(S_2\) beyond a far cap, including the affine mixed terms. It uses flux energies of the physical solutions, so it is independent of the numerical residual inside the cap. The ledger gives no estimate of those interior residuals.

Inputs used for the hierarchy and graph were [HIERARCHY](../GLR-GOAL-011/HIERARCHY.md), [FINITE-DATA](../GLR-GOAL-011/FINITE-DATA.md), [PAIRING](../GLR-GOAL-011/PAIRING.md), [HORIZON-SOURCE](../GLR-GOAL-011/HORIZON-SOURCE.md), [SIGNED-RESIDUE](../GLR-GOAL-011/SIGNED-RESIDUE.md) and the pinned [GOAL-012 source investigation](../GLR-GOAL-012/investigators/B-PHYSICAL-SOURCE.md). The compact jet errors in that signed residue calculation pertain to its frequency-domain problem.
