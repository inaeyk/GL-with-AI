# GOAL-002: complete quadratic equations and partial regular reconstruction

2026-10-22, non-production perturbation theory. Input branch commit
`8d4163cd876b33752f7fb6938bb7861fe221b7c0`; historical scientific result
`7110bc2cb3c0496dd62dc97d37b34d11ec780a17`. Expansion has no factorial:
$g=\bar g+\epsilon h+\epsilon^2q+\cdots$. Only L/r0=10 is investigated.
This note derives every second-order equation, integrates the zero-mode
metric constraints in quadratures and specifies the forced second-harmonic
system. **It does not claim a fully evaluated second-harmonic Green
operator or a bound on the complete q. Gate B is not passed.**

## 1. Independent source route, including the other equations

For any perturbation on the Ricci-flat product background define

$$ C^A{}_{BC}=\tfrac12\bar g^{AD}
 (\bar\nabla_Bh_{DC}+\bar\nabla_Ch_{DB}-\bar\nabla_Dh_{BC}),
\qquad D^A{}_{BC}=-h^A{}_D C^D{}_{BC}.$$

The exact connection-difference identity, expanded without imposing a
scalar ansatz, gives

$$ Q_{AB}=R^{[2]}_{AB}[h,h]
=\bar\nabla_C D^C{}_{AB}-\bar\nabla_B D^C{}_{AC}
 +C^C{}_{CD}C^D{}_{AB}-C^C{}_{BD}C^D{}_{AC},\qquad
R'[q]_{AB}=-Q_{AB}. \tag{1}$$

With $h^U_{zA}=0$, the on-shell linear equations impose 4D TT. Putting
$h=A(t)H(r)\cos kz$ in (1) gives, in **real cosine amplitudes**,

$$ Q_{zz}=-\frac{k^2A^2}{8}\mathcal H(1+3\cos2kz),\quad
\mathcal H=H_{\mu\nu}H^{\mu\nu}
=T^2/f^2+f^2D_r^2+2K_s^2-2B^2. \tag{2}$$

Here $T=H_{tt}, B=H_{tr},D_r=H_{rr},K_s=H_{\theta\theta}/r^2$.
The complex +2 Fourier coefficient is half its real cosine coefficient.
The different coefficients 1 and 3 in (2) are essential. From
$R'_{zz}[q_0]=-\bar\Box\chi/2$, $\chi=q_{zz,0}$,

$$\bar\Box\chi=2Q_{zz,0},\qquad
\phi_2=\frac{\sqrt3}{4}\chi,\qquad
\boxed{\bar\Box\phi_2=-\frac{\sqrt3k^2}{16}A^2\mathcal H}. \tag{3}$$

This independently reobtains GOAL-001 by the full connection expansion.
The prior spacelike-ADM/action derivation is a separate route. There is
no unresolved source inconsistency. The Lorentz contraction is not a norm.
The scalar radial operator and its Green sign are unchanged:
$L_s=-\partial_x^2+V+s^2$, $s=2\Omega$, $V=fr_0/r^3$,
$G_s=-y_H(x_<)y_\infty(x_>)/W$, with the specified negative W.
An exact nontrivial $S=L_s b$ with smooth compactly supported b still
has zero outgoing overlap; source alone does not establish radiation.

The conserved tensor is the **Einstein** source
$\mathcal Q_{AB}=Q_{AB}-\bar g_{AB}Q^C{}_C/2$, not Ricci Q.
Expanded contracted Bianchi gives $\bar\nabla^A\mathcal Q_{AB}=0$
when every linear vacuum equation holds. Off-shell profiles or switched
GL metrics do not have this property.

## 2. Explicit SO(3) radial equation generator

These formulas specify all seven independent equations with no omitted
constraint or unexplained source placeholder. Write
$ds_5^2=g_{ab}dx^a dx^b+W d\Omega_2^2$, $a,b=v,r,z$, $W=R^2$.
The exact equations are particularly simple:

$$ E_{ab}=R^{(3)}_{ab}-W^{-1}\nabla_a\nabla_bW
 +\frac{W_{,a}W_{,b}}{2W^2}=0,\qquad
E_\Omega=1-\tfrac12\Box_3W=0. \tag{4}$$

Use $\bar g_{ab}dx^a dx^b=-f dv^2+2dvdr+dz^2$, $W_0=r^2$.
For each sector put $g_{ab}=\bar g_{ab}+\epsilon h_{ab}+\epsilon^2p_{ab}$,
$W=W_0+\epsilon w+\epsilon^2u$. C and D in the following formulas are
three-dimensional connection differences constructed from h. Set

$$ H^0_{ab}=\bar\nabla_a\bar\nabla_b W_0,\quad
H^1_{ab}=\bar\nabla_a\bar\nabla_b w-C^c{}_{ab}W_{0,c},\quad
H^2_{ab}=-D^c{}_{ab}W_{0,c}-C^c{}_{ab}w_{,c}.$$

The **evaluated algebraic differential expressions** for the sources are

$$\begin{aligned}
Q_{ab}={}&R^{(3)[2]}_{ab}-H^2_{ab}/W_0+wH^1_{ab}/W_0^2
 -w^2H^0_{ab}/W_0^3+\frac{w_{,a}w_{,b}}{2W_0^2}\\
&-\frac{w}{W_0^3}(W_{0,a}w_{,b}+w_{,a}W_{0,b})
 +\frac{3w^2}{2W_0^4}W_{0,a}W_{0,b},\\
Q_\Omega={}&-\tfrac12[\bar g^{ab}H^2_{ab}-h^{ab}H^1_{ab}
 +h^a{}_ch^{cb}H^0_{ab}]. \tag{5}
\end{aligned}$$

$R^{(3)[2]}$ is (1) restricted to three indices. The corresponding
linear left side for arbitrary p,u is

$$\begin{aligned}
\mathscr L_{ab}(p,u)={}&R^{(3)'}_{ab}[p]
 -[\bar\nabla_a\bar\nabla_bu-C[p]^c{}_{ab}W_{0,c}]/W_0
 +uH^0_{ab}/W_0^2\\
&+(W_{0,a}u_{,b}+u_{,a}W_{0,b})/(2W_0^2)
 -uW_{0,a}W_{0,b}/W_0^3,\\
\mathscr L_\Omega(p,u)={}&-\tfrac12\{
\bar g^{ab}[\bar\nabla_a\bar\nabla_bu-C[p]^c{}_{ab}W_{0,c}]
 -p^{ab}H^0_{ab}\}. \tag{6}
\end{aligned}$$

Thus $\mathscr L(p_n,u_n)=-\Pi_nQ$ is an explicit inhomogeneous radial
DAE after $\partial_v\mapsto2\Omega$, $\partial_z\mapsto ink$.
Use $h=e^{\Omega v}\widehat H(r)\cos kz$ with the regular tensor below.
Equations (5)-(6) retain the vv,vr,vz,rr,rz,zz and sphere equations.
Their normal projections are the Hamiltonian and two orbit momentum
constraints; their propagation follows from all three Bianchi identities.
Replacing constraints by a trace-free ansatz in the forced sector is wrong.

The new executable `goal002_sector_checks.py` substitutes the full master
reconstruction into (4)-(5). It checks all seven linear equations, both
zz source harmonics and **all three quadratic Bianchi identities** exactly
as rational functions of r for a local on-shell algebra specialization
$\kappa=ik=2/3,\Omega=1/7$. This is an analytic-continuation algebra
control, not a global physical eigenfunction or a proof by all-parameter
symbolic simplification. The general equations above follow by tensor
expansion; specialization tests their implementation. A doubled zz source
with unchanged other sources fails conservation. The independent geometry
audit uses a different direct-Christoffel flat on-shell TT control.

## 3. Complete zero-mode metric in quadratures

In the completed frame the zero-mode Einstein metric perturbation is
$p^E_{\mu\nu}=q^U_{\mu\nu,0}+\chi\bar g_{\mu\nu}/2$.
The 4D Ricci and Einstein equations reduce to

$$\delta R^{(4)}_{\mu\nu}[p^E]=-Q_{\mu\nu,0}
 -\tfrac12\bar g_{\mu\nu}Q_{zz,0},\qquad
\delta G^{(4)}_{\mu\nu}[p^E]=\mathcal T_{\mu\nu},$$
$$\mathcal T_{\mu\nu}=-Q_{\mu\nu,0}
 +\tfrac12\bar g_{\mu\nu}(Q^{\alpha}{}_{\alpha,0}+Q_{zz,0}),
\qquad\bar\nabla^\mu\mathcal T_{\mu\nu}=0. \tag{7}$$

The radion Hessian cancels in (7); its stress first enters at fourth
order, since background phi=0. Thus any homogeneous second-order scalar
can be added without changing pE at this order. Quadratic sources with
one z index have odd sine parity and average to zero. Reflection-symmetric
standing-wave data carry no forced zero-mode vector or KK momentum.
Fix independent KK momentum to zero as a data convention.

Use zero-mode Einstein-frame areal EF gauge

$$ds_E^2=-e^{2\epsilon^2\beta}(1-r_0/r-2\epsilon^2\mu/r)dv^2
 +2e^{\epsilon^2\beta}dvdr+r^2d\Omega^2.$$

All independent metric equations become

$$\mu_{,r}=-\tfrac12r^2\mathcal T^v{}_v,\quad
\mu_{,v}=\tfrac12r^2\mathcal T^r{}_v,\quad
\beta_{,r}=\tfrac12r\mathcal T_{rr}, \tag{8}$$
$$\delta G_{\theta\theta}
=r(r-r_0)\beta_{,rr}+r^2\beta_{,rv}
 +(r+r_0/2)\beta_{,r}-r\mu_{,rr}=\mathcal T_{\theta\theta}. \tag{9}$$

The remaining mixed equation follows from these. Equations (8)-(9) were
independently checked by direct 2D curvature plus sphere reduction. For
conserved sources, the angular and evolution equations follow with their
integration/boundary constraints; they cannot be ignored for trial sources.

With asymptotic clock $\beta(\infty,v)=0$ and fixed ADM mass shift
$\delta m=G_4\delta M$, the constraint-compatible quadrature is

$$\boxed{\beta(v,r)=-\tfrac12\int_r^\infty R\mathcal T_{rr}(v,R)dR,
\quad\mu(v,r)=\delta m+\tfrac12\int_r^\infty R^2\mathcal T^v{}_v(v,R)dR.} \tag{10}$$

Boundary flux at infinity vanishes at quadratic order for the spatially
localized first-order GL source. Radion Bondi mass loss starts at order
$\epsilon^4$, so $\delta m$ is constant. Fixed total ADM mass means
$\delta m=0$ at this order; the sourced horizon/interior mass need not
vanish. Convergence of (10) uses smooth EF source at the horizon and
exponential GL decay at infinity. Numerical suprema, derivative bounds
and complete source-tail enclosures in (10) have **not** been evaluated.

There is no independent horizon-regular static scalar charge at fixed L:
$\phi=c_\infty+c\log f$, fixed circle removes $c_\infty$, horizon
regularity removes c. At stationary KK infinity
$M=L(2c_t-c_z)/(4G_5)$, tension $=(c_t-2c_z)/(4G_5)$.
A regular homogeneous mass shift has $\delta c_z=0$, so its tension is
fixed by its mass; tension is not a second arbitrary regular charge.
Sourced scalar charge and finite-data waves are distinct. In the time
dependent exponential branch there is no stationary 1/r scalar charge at
spatial infinity; $\Phi(u)$ belongs to null asymptotics.

## 4. Forced n=2 sector and what remains unevaluated

At second order set $q_{zA,2}=0$ by the unique nonzero-K compensator,
$K=2k$. Use complex Fourier amplitudes consistently. Define
$\tau=\bar g^{\mu\nu}q_{\mu\nu,2}$ and
$j_\mu=\bar\nabla^\nu q_{\mu\nu,2}-\bar\nabla_\mu\tau$.
The z equations fix inhomogeneous trace and divergence:

$$\boxed{\tau=-2Q_{zz,2}/K^2,\qquad
j_\mu=-2Q_{\mu z,2}/(iK).} \tag{11}$$

For a real-cosine q the trace coefficient in this convention is
$\tau_{\cos2}=3A^2\mathcal H/16$. The remaining equations are

$$[\bar\Box_4+2\bar R_{\mu\alpha\nu\beta}(\cdot)^{\alpha\beta}-K^2]
q_{\mu\nu,2}=2Q_{\mu\nu,2}+\bar\nabla_\mu\bar\nabla_\nu\tau
 +\bar\nabla_\mu j_\nu+\bar\nabla_\nu j_\mu. \tag{12}$$

Together with (11), or equivalently the seven equations (6), this is the
complete forced system. Boundary conditions: future ingoing EF smoothness,
fixed asymptotic circle/clock and decaying outgoing analytic continuation
for the exponential particular response. At large r its homogeneous
radial exponential is $\exp[-\sqrt{(2k)^2+(2\Omega)^2}\,r]$ times powers.
Because this exponent equals twice the first-order exponent, quadratic
source tails can have polynomial/resonant factors; a naive homogeneous
outer Robin condition alone does not bound their omission.

A formal retarded tensor inverse of (12), plus homogeneous solutions
satisfying zero-source (11), specifies the initial-value response. It
is **not** a fully evaluated Green representation: the source projection,
independent horizon/infinity fundamental matrices, constraint matching,
Wronskian/nonresonance and endpoint error constants remain to be evaluated.
No numerical q2 norm is claimed from this formal inverse.

Two reconstruction routes were examined: (i) the regular reduced radial
DAE (6) with its inhomogeneous z constraints; (ii) the covariant massive
tensor equation (12) with prescribed trace/divergence. They establish a
consistent equation-level representation, but neither supplies the
missing constraint-compatible inverse/norm. This is the bounded stopping
point for this sector, not a successful Gate B solution. No extra scalar
coefficient solve can repair it. No new n=2 discretization was launched.

Homogeneous physical n=2 initial data remain independent. At L10,
$2kr_0=1.256637>k_{cr}r_0$ (historical threshold numerical evidence).
This is separate from massive-wave propagation: real frequencies need
$|\omega|>2k$, $q_r^2=\omega^2-(2k)^2$, group speed $q_r/|\omega|$.
The forced imaginary-frequency exponential is an evanescent response,
not an on-shell outgoing massive-energy flux. For L16 the independent
n=2 GL instability would need its own amplitude/data; no L16 solve is made.

## 5. Future-regular frame and amplitude convention

In EF coordinates $v=t+x$, the tensor is

$$\widehat H_{vv}=e^{-\Omega x}T,\quad
\widehat H_{vr}=e^{-\Omega x}(B-T/f),\quad
\widehat H_{rr}=e^{-\Omega x}(D_r-2B/f+T/f^2),\quad
\widehat H_{\theta\theta}=e^{-\Omega x}H_{\theta\theta}. \tag{13}$$

All components, rather than just their scalar contraction, are regular.
Audit 1 independently supplies the exact horizon constants using the
master Frobenius recurrence. A regular background tetrad is obtained from
$\ell=\partial_v+(f/2)\partial_r$, $n=-\partial_r$,
$e_0=(\ell+n)/\sqrt2$, $e_1=(\ell-n)/\sqrt2$ and unit sphere/z legs.
It has no inverse powers of f. The slices
$\tau=v-r=t+r_0\log(r/r_0-1)$ are spacelike, with radial metric $2-f$.
At r=2r0, tau=t, preserving the GOAL-001 reference clock.

On a tau slice the tensor profile is $e^{\Omega r}\widehat H$
or, equivalently, $\delta^{-\hat\Omega}H_{EF}^{raw}$, where raw denotes
the bracketed EF combinations in (13) before multiplication by
$e^{-\Omega x}$ (and with r0=1 in the latter expression). Thus it is
in r0=1 units, $\delta=r-1$. The saved-profile check finds Euclidean
tetrad Frobenius magnitude about **39.47** near the future horizon for
unit reference amplitude. This is a pointwise sampled diagnostic, not
a supremum/derivative enclosure; roundoff in saved delta near 1 is visible
at about 2e-4 in that norm. It already shows why A=0.1 cannot be assumed
small from its reference-radius definition. No A_* is inferred.

For a nonlinear perturbative coordinate convention, recursively set
nonzero-harmonic zA metric components to zero, use the zero-mode
Einstein-frame areal EF gauge above, fix its clock at infinity, KK phase,
and exclude residual large circle diffeomorphisms. Define in that fixed
gauge $A_{phys}(\tau)=2\langle(R(\tau,2r_0,z)/(2r_0)-1)\cos kz\rangle$.
It equals A at first order; zero and second harmonics cannot change its
coefficient at second order. This is an explicit **gauge-plus-clock**
prescription, not a claim that the unfixed coordinate r is a nonlinear
invariant. Its finite-A global existence and conversion to horizon bulge
geometry are not proved. Regular diffeomorphisms can instead be completed
back to this prescription before comparing observables.

The radion completion remains
$q^U=q+\mathcal L_\xi h+\tfrac12\mathcal L_\xi^2\bar g+\mathcal L_\eta\bar g$,
$\phi_2=\sqrt3\langle q^U_{zz}\rangle/4$; the eta zz average vanishes.
The independent periodic pure-gauge EF check rejects omission of the
half-Lie-squared term. At infinity first-order completion terms decay
exponentially and do not change the matched scalar news.
