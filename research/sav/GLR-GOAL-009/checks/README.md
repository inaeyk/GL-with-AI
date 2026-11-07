# Independent optical subsystem and normalization checks

This work reconstructs the compact-normal identity and flux before comparing the detailed source formulas. The local-area optical hierarchy is a new derivation. A direct Christoffel calculation provides a separate algebraic implementation check. The subsystem assumes an existing smooth optical chart and a monotone local area variable. Its use for an infinite wedge still requires analytic estimates and compatibility with the prepared Cauchy data. The checks contain no evolution of GL data.

## Compact-normal identity

Use signature \((-++++)\) and the Ricci sign for which a unit round sphere has positive Ricci. Write
\[
g_5=b^2dz^2+h_{ab}(dx^a+X^a dz)(dx^b+X^b dz),\quad
n=b^{-1}(\partial_z-X),\quad K=(\partial_z h-\mathcal L_X h)/(2b),\quad T=\operatorname{tr}_h K.
\]
The unit spacelike normal acceleration is \(a=-\nabla_h\log b\). In the full divergence its normal part is \(g(n,\nabla_n a)=-a:a\). Thus
\(\operatorname{div}_5 a=\operatorname{div}_h a-a:a=-\Box_h b/b\).
The contracted Ricci identity applied to \(n\) gives
\[
\operatorname{Ric}_{nn}=-n(T)-K:K-\Box_h b/b.
\]
The density \(D=\sqrt{|\det h|}/\sin\theta\) satisfies \(bDT=\partial_zD-\partial_a(DX^a)\). Multiplying by \(bD\) and expanding the product rules gives the local off-shell equation
\[
\partial_a(Dh^{ab}\partial_b b-DX^a T)+\partial_z(DT)
=bD(T^2-K:K-\operatorname{Ric}_{nn}).
\]
Its periodic average yields the vacuum mean identity. In the completed GL gauge \(b_1=X_1=T_1=0\), and \(K_1=-(k/2)e^{\Omega t}H\sin(kz)\). The compact average of the square contributes \(1/2\). With \(D_0=fr^2/2\) in the reference double-null chart the mean source is \(-fr^2 k^2 e^{2\Omega t}H:H/16\). Rescaling by \(r\sqrt3/2\) and using the background null density yields the positive coefficient \(+\sqrt3 k^2 rf e^{2\Omega t}H:H/16\) in \(4\psi_{uv}\), where \(H:H\) is the Lorentzian contraction. This matches the audited Z coefficient. [GOAL-006 §3](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/GLR-GOAL-006/DERIVATIONS.md#3-route-b-exact-scalar-equation-and-a-trace-estimate) · [GOAL-005 Z waveform](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/RADIATION.md#causal-z-preparation)

## Full optical local-area subsystem

Let \(u\) be an actual five-dimensional optical function, \(z\) periodic and transported along its null generators, and \(r=r_5\) the radius of the round orbits. Set
\[
R=r\sqrt b,\quad b=\sqrt{g_{zz}},\quad \partial_rR>0
\]
in the \(r\) optical chart. On changing to \(R\) the conditions \(g_{RR}=g_{Rz}=0\) persist and \(g_{zz}=b^2\) is preserved, because \(du=0\) in this metric restriction. Use \(B>0,V,A\) in the new chart:
\[
g_5=-BV\,du^2-2B\,du\,dR-2BA\,du\,dz+b^2dz^2+r^2d\omega^2 .
\]
The compact leaf shift is \(A\partial_R\), the compact lapse is \(b\), and \(D=Br^2\). Write
\[
q=\log b,\quad \beta=\log(Bb),\quad Q=q_z-Aq_R,\quad
P=A_R+A(\beta_R-3q_R)-\beta_z+3q_z .
\]
The local coordinate \(R\) fixes the transverse area density \(r^2b=R^2\) on each generator. A surface of constant \(R\) can intersect the generators at different sphere radii. Identification with the physical cut observable requires a comparison.

For the two angular directions let \(w=K^\theta{}_\theta=(-A/R-Q/2)/b\). The base \((u,R)\) block satisfies \(K_{RR}=0\) and \(U=K_{uR}/h_{uR}=(-P+2Q)/(2b)\). Consequently
\[
T=2(U+w),\qquad K:K=2(U^2+w^2),\qquad T/2+w=(-P/2-2A/R)/b .
\]
In particular \(K_{uu}\) drops out of both contractions. This follows from \(h^{uu}=0\) and \(K_{RR}=0\).

**Lapse.** The vector \(\partial_R\) has nonaffinity \(B_R/B\), expansion \(2/R\), and transverse principal expansion rates \(r_R/r,r_R/r,b_R/b\). The null Raychaudhuri identity and \(2r_R/r+b_R/b=2/R\) give
\[
\operatorname{Ric}_{RR}=2\beta_R/R-3q_R^2/2,\qquad
\beta_R=3Rq_R^2/4 \quad\text{in vacuum}.
\]
This contains no second radial derivative. For example, if \(\beta\to0\) and \(\int_R^\infty s q_s^2\,ds\) is finite, then \(\beta(R)=-(3/4)\int_R^\infty s q_s^2\,ds\).

**Shift.** Applying the three-dimensional connection formula to the \((u,R,z)\) metric and adding the spherical warp term \(-2r^{-1}\nabla_R\nabla_z r\) gives the off-shell identity
\[
2\operatorname{Ric}_{Rz}=\partial_R P+2P/R+(4\beta_z-6q_z)/R-3q_Rq_z .
\]
For example the base connection has \(\Gamma^u_{Rz}=0\), \(\Gamma^z_{Rz}=q_R\) and
\(2\Gamma^R_{Rz}=A(\partial_R\log B-2q_R)+A_R+\partial_z\log B\).
Vacuum gives two radial equations
\[
(R^2P)_R=6Rq_z-4R\beta_z+3R^2q_Rq_z,\qquad
(e^{\beta-3q}A)_R=e^{\beta-3q}(P+\beta_z-3q_z).
\]
These use one compact derivative of the scalar and no \(u\) derivatives.

**Potential.** The conformally rescaled leaf metric \(h_E=b h\) has exact areal form
\[
h_E=e^\beta(-Vdu^2-2du\,dR)+R^2d\omega^2 .
\]
This conformal identity is pointwise in \(z\), independent of a Killing assumption. The tangential Ricci split uses
\[
\operatorname{Ric}^{(5)}_{ab}=\operatorname{Ric}^h_{ab}
-b^{-1}(\partial_z-\mathcal L_X)K_{ab}-T K_{ab}+2K_{ac}K^c{}_b-b^{-1}\operatorname{Hess}^h_{ab}b .
\]
The four-dimensional conformal formulas give
\(\operatorname{Ric}^h-b^{-1}\operatorname{Hess}^h b
=\operatorname{Ric}^{h_E}+(h_E/2)\Box_{h_E}q-(3/2)dq\otimes dq\)
and \(\Box_h b/b=b\Box_{h_E}q\). The angular projection of \(b^{-1}(\partial_z-\mathcal L_X)K\) is \((nw+2w^2)h_{\theta\theta}\), which cancels the \(2K_{ac}K^c{}_b\) contribution there. In the angular Ricci identity use the compact-normal Ricci equation to remove \(\Box_{h_E}q\). With \(\operatorname{Ric}^{h_E}_{\theta\theta}=1-e^{-\beta}(RV)_R\) this yields the off-shell combination
\[
\operatorname{Ric}^{(5)}_{\theta\theta}+\frac{R^2}{2b}\operatorname{Ric}_{nn}
 =1-e^{-\beta}(RV)_R-\frac{R^2}{b}\left[n(T/2+w)+T^2/4+2w^2\right].
\]
The vacuum equation for \(V\) is therefore radial, with no \(u\) derivatives. The cancellation \(T/2+w=(-P/2-2A/R)/b\) removes \(Q\) from the differentiated compact expression.

**Scalar.** With \(W=Rq\) the remaining principal equation is
\[
-2W_{uR}+(V W_R)_R-(V_R/R)W
 =-R e^\beta b^{-1}\left[n(T)+2(U^2+w^2)\right].
\]
It is \(\operatorname{Ric}_{nn}=0\). At first order about the Schwarzschild string with terminal \(\beta=0\), \(P=A_R+3q_z\), \((R^2P)_R=6W_z\), and moving \(-W_{zz}\) to the left gives the wave principal part with the compact Laplacian. The nonlocal linear source \(RP_z+2A_z\) remains. A high-order PDE argument must estimate these radial integral terms together with the scalar equation. The constraints alone do not provide a differentiated massive energy.

**Supplementary equations.** If the four equations above hold, \(\operatorname{Ric}_{RR}=\operatorname{Ric}_{Rz}=\operatorname{Ric}_{zz}=\operatorname{Ric}_{\theta\theta}=0\). The curvature trace is \(-2\operatorname{Ric}_{uR}/B\), so the Einstein tensor \(G_{aR}\) vanishes. In density form
\(\nabla_a G^a{}_R=(\sqrt{|g_5|})^{-1}\partial_a(\sqrt{|g_5|}G^a{}_R)-\tfrac12\partial_R g_{ab}G^{ab}\).
The derivative contraction reduces to the \(z\) and sphere diagonal terms, yielding
\(-(\operatorname{Ric}_{uR}/B)(b_R/b+2r_R/r)=-2\operatorname{Ric}_{uR}/(BR)\).
Hence \(\operatorname{Ric}_{uR}=0\). The remaining trace is zero. The two Bianchi components then give
\[
\partial_R(R^2\operatorname{Ric}_{uz})=0,\qquad
\partial_R\!\left[R^2\operatorname{Ric}_{uu}
       +(B/b^2)R^2 A\operatorname{Ric}_{uz}\right]
   =\partial_z[(B/b^2)R^2\operatorname{Ric}_{uz}].
\]
Here \(\sqrt{|g_5|}/\sin\theta=B R^2\), \(g^{uR}=-1/B\), \(g^{Rz}=-A/b^2\), and contractions with \(\partial_u g\) and \(\partial_z g\) vanish because \(g_{RR}=g_{Rz}=0\). Enforcing both supplementary equations at one cross-section per generator, or enforcing their \(o(R^{-2})\) decay at infinity, gives the complete vacuum equations. Establishing this from compatible characteristic or Cauchy data is separate from the algebra.

## Necessary compatibility at a product end

Suppose an optical end as above is smooth and, for some \(\eta>0\), has
\[
q=O(R^{-1}),\quad q_R=O(R^{-2}),\quad
\partial_z^j q=O(R^{-2-\eta}),\quad
\partial_z^j q_R=O(R^{-3-\eta})\quad(j=1,2)
\]
at fixed \(u\), uniformly in \(z\). If \(\beta,A\to0\), the lapse integral implies \(\beta=O(R^{-2})\) and its first two compact derivatives are \(O(R^{-3-\eta})\). The shift source for \((R^2P)_R\) and its first compact derivative are \(O(R^{-1-\eta})\), so \(R^2P\to C(u,z)\) in \(C^1_z\) with \(O(R^{-\eta})\) error. The remaining shift equation gives \(A=-C/R+O(R^{-1-\eta'})\) in \(C^1_z\) where \(\eta'=\min(\eta,1)\). Its radial derivative and that of \(P\) follow from the same equations. Inserting the asymptotics into the potential equation gives
\[
(RV)_R=1-\tfrac32\partial_z C+O(R^{-\eta'}),\qquad
\lim_{R\to\infty}V=1-\tfrac32\partial_z C .
\]
Indeed \(T/2+w=3C/(2R^2)+O(R^{-2-\eta'})\), the \(A\partial_R\) term in its normal derivative is \(O(R^{-4})\), and the quadratic contractions contribute \(O(R^{-2})\) to \((RV)_R\). The last limit follows by radial integration. Thus \(V\to1\) forces \(C\) to be independent of \(z\). Compact reflection in an equivariant chart makes \(P\) odd, so then \(C=0\). A normalized potential cannot be inferred from the lapse and shift boundary conditions without the corresponding torsion compatibility. In particular, with \(S_P=6Rq_z-4R\beta_z+3R^2q_Rq_z\), prescribed finite-radius data at \(R=R_*\) give
\[
C=R_*^2P(R_*)+\int_{R_*}^\infty S_P(s)\,ds .
\]
Setting \(C=0\) terminally instead selects \(R_*^2P(R_*)=-\int_{R_*}^\infty S_P(s)\,ds\). The two prescriptions agree only when the compatibility holds. In an integrable product end the radial equation for \(RV\) also leaves an additive integration constant in \(RV\). The condition \(V\to1\) does not fix this \(1/R\) coefficient of \(V\). Its boundary data and supplementary evolution must be matched to the physical mass and initial preparation.

If \(C=0\), \(\eta>1\) is a sufficient decay condition here to make \((RV)_R-1\) absolutely integrable. To see why weaker scalar decay does not suffice for the **radial equations alone**, prescribe \(q=a(z)/R^3\) for smooth nonconstant periodic \(a\), with \(\beta(\infty)=A(\infty)=C=0\). Direct integration gives
\[
\beta=O(R^{-6}),\qquad P=-6a_z/R^3+O(R^{-6}),\qquad A=(9/2)a_z/R^2+O(R^{-5}).
\]
Therefore \((RV)_R=1+6a_{zz}/R+O(R^{-4})\) and \(V=1+6a_{zz}\log R/R+O(R^{-1})\). This constructed hierarchy solves the three radial equations. The scalar equation at leading order gives \(0=2a_{zz}/R^2+o(R^{-2})\), so the nonconstant example fails the scalar equation. It exposes the need for genuine dynamical decay or cancellation, with no obstruction asserted for the physical GL development.

## Flux and background cone checks

For the circle-independent leading geometry, \(q=2\phi/\sqrt3\) and \(g_5=e^{-q}(g_E+e^{3q}dz^2)\). The one-dimensional warped product \(\widetilde g=g_E+e^{3q}dz^2\) has \(\widetilde R=R_E-3\Box_Eq-(9/2)|dq|^2_E\). The five-dimensional conformal change multiplies the volume by \(e^{-5q/2}\) and yields
\(R(g_5)=e^q[\widetilde R+4\widetilde\Box q-3|dq|^2_E]\), where \(\widetilde\Box q=\Box_E q+(3/2)|dq|^2_E\).
The resulting density is \(\sqrt{|g_E|}[R_E+\Box_E q-(3/2)|dq|^2_E]\) per coordinate compact length. After the circle integral and boundary divergence the scalar action is \(-2(\nabla\phi)^2/(16\pi G_4)\). On a normalized product end with the requisite flux regularity this gives \(dE/du_B=N^2/G_4\).

Independently, the two transverse sphere strain entries in an orthonormal asymptotic frame are \(-2\phi/\sqrt3\) each, and the compact entry is \(4\phi/\sqrt3\). The trace is zero and the squared coefficient is \(8\). Under the same asymptotic regularity the five-dimensional transverse quadratic gravitational current per unit area is \(8N^2/(32\pi G_5 r_E^2)\). Integration against \(4\pi L r_E^2\) again gives \(N^2/G_4\). These coefficients presume decay of compact-dependent contributions sufficient for the massless flux law. Exact \(\mathrm{SO}(3)\) excludes a sphere tangent trace-free tensor. The compact polarization still permits a scalar. With dimensionless cuts the energy coefficient is \(2\|n_Z\|_2^2\). Given the target news error on \(I\), the triangle inequality gives positivity for \(C_I\epsilon^2<\|n_Z\|_2\) in the proven exterior smallness range.

For the homogeneous monopole massive potential, set \(r_0=1,\ a=(n\pi/5)^2,\ f=1-1/r,\ D_m=1+ar^3\). The pinned form of the potential divided by \(f\) is
\[
a+\frac{6r-11}{r^3}+\frac{18(r-1)}{r^3D_m^2}
       -\frac{6(4r-5)}{r^3 D_m}.
\]
Its horizon limit is \((a^2-4a+1)/(1+a)<0\) for \(n=1,2\). For \(r\ge5\) the first two corrections are nonnegative and the last is bounded below by \(-24/(ar^5)\). Thus \(V_n/f\ge a-24/(ar^5)>0\) for all nonzero harmonics there. The positivity statement is for the homogeneous scalar master on the frozen Schwarzschild string. It does not bound the true commuted tensor principal part or provide physical-cone comparison. [Pinned GOAL-003 potential](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/physics/goal003_forced_master.md) · [Brito, Cardoso and Pani, arXiv v3, §IV after (30)](https://arxiv.org/html/1304.6725v3)

## Negative controls and reproducibility

From this directory run:

    python3 --version
    python3 -B -c 'import sympy; print("SymPy", sympy.__version__)'
    python3 -B check_optical_identities.py

[Saved output](check_optical_identities.txt) uses Python 3.14.8 and SymPy 1.14.0. The checks use exact functions and derivatives, not a numerical solution or a finite series truncation. The script constructs the 3D connection, adds the sphere warp term, and checks the Ricci components against the proposed identities. No error tolerance is used. Potential displays are rounded approximations to exact expressions. Their signs use the exact rational comparison \(3<\pi<22/7\).

- The Ricci check rejects reversing the compact curvature sign, dropping \(X(T)\), and dropping the inverse lapse in the wave term. The quadratic source check rejects setting the zero harmonic to zero before taking the square.
- The product metric pullback \(z_{\rm old}=z+\epsilon\sin z\) preserves length at \(|\epsilon|<1\). It changes the raw mean of \(g_{zz}\) by \(\epsilon^2/2\).
- The compact-shift example has \(h=-2du\,dv\), \(b=1\), \(X=\alpha\partial_u\). The common mean density is \(u\)-null, but \(g_5^{uu}=\alpha^2>0\) for \(\alpha\ne0\). Causal cone energies on those surfaces cannot be identified merely by averaging.
- The frozen massive potential is negative just outside the horizon for \(n=1,2\), while positive on the specified far exterior.
- At fixed Schwarzschild time, \(r_*(m)=r+2G_4m\log(r/(2G_4m)-1)\), so \(u_{M+\Delta M}-u_M=-2G_4\Delta M\log r+O(1)\). The manufactured clock shift \(\epsilon^2 n(u+\epsilon^2\log r)\) for compactly supported \(n\) has a finite-radius quadratic coefficient \(n(u)\), but vanishes at \(r=\infty\) for fixed nonzero \(\epsilon\). The two limits fail to commute even though the family is even.
- On flat \(t,r,z\) with \(u=t-S(r,z)\), the eikonal requires \(S_r=\sqrt{1-S_z^2}\) on the outgoing branch. For smooth nonconstant periodic \(S(R_*,z)=R_*+\alpha(z)\), let \(p_0=\alpha'(z_0)\) with \(|p_0|<1\). Rays have \(z=z_0+(r-R_*)p_0/\sqrt{1-p_0^2}\). The Jacobian \(1+(r-R_*)\alpha''(z_0)/(1-p_0^2)^{3/2}\) vanishes somewhere at finite radius because \(\alpha''\) is negative somewhere. This remains possible for arbitrarily small nonzero tilt. A normalized terminal construction or equivalent control is needed.
- Even the free equation \(H_{uv}=0\) with \(H=\epsilon^4\sin(v^2)/v\) on \(v\ge v_*>0\) has a vanishing field trace and fixed-\(v\) derivative. At flat tube \(v=u+2R\), its total \(u\) derivative is \(\epsilon^4[2\cos(v^2)-\sin(v^2)/v^2]\), whose \(L^2(I)\) norm squared tends to \(2\epsilon^8|I|\). The oscillatory part has zero integrated limit by one integration by parts in the nonstationary phase \(2v^2\). The chart trace theorem cannot supply the extra tube derivative. [Pinned independent Work audit, §4](https://github.com/inaeyk/GL-with-AI/blob/c4cac8f2e2dd12cfbdf50651863d2188a75fc87c/research/sav/verification/AUDIT_GOAL006_80f030df.md#4-optical-identification-and-physical-flux)
