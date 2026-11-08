# A. Spectral continuation of the ordered axial waveform

Inputs are GOAL-011 at `eb439228601a649c658ce719b503ff0bdc37a83f` and the Work audit continuation record (requested review pin `75cb3ff3feef98164a65cf14105be6df77ea809c`, local inspected file blob `3cf7392b1bb6d414519002efe9008ccc0b6f16bc`). This was non-blind. I read the source hierarchy, finite data, canonical pairing, null-transform, QNM, horizon and exterior source notes, the local spectral validator, and the Work continuation argument. I take the constrained reduction, GL eigenmode and its bracket, and fundamental pole disk from those inputs. The threshold argument and compact-moment sign test below were developed for GOAL-012. The interval checks reuse the pinned validators. [GOAL-011 source](https://github.com/inaeyk/GL-with-AI/pull/13) · [Work audit](https://github.com/inaeyk/GL-with-AI/pull/14).

Set $r_0=1$ (frequencies below are in $r_0^{-1}$), $f=1-1/r$, $x=r+\log(r-1)$, $t=T(x)=r-x$ on the launch, $u=t-x$, $k=2\pi/10$, $q=\sqrt{k^2+\Omega^2}$, and $\Omega\in[0.06328118,0.06328120]$. All statements are for the fixed canonical ordered axial $\ell=2$ coefficients. The GL cosine has Fourier coefficients $1/2$. Keep the ADM mass, circumference, and $u_0=0$ fixed. The Laplace sign is $e^{-st}$ or $e^{-su}$, and $W(s)=h g_+'-h'g_+$, with unit $e^{sx}$ and $e^{-sx}$ endpoint normalizations respectively.

## Physical transform and a pole-free right domain

Let $\Theta=\mathbf1_{t>T(x)}$. Testing the extension of the scalar master with $e^{-st}y(x)$ gives the graph term
$$
 -\int e^{-sT}\big[y(w_t+T'w_x)+w(sy-T'y')\big]_\Sigma\,dx .
$$
The first term has the negative normal flux sign. The baseline graph data $(w,w_t,w_x)=(a,a'_{\delta},-a'_{\delta})$ reduce it to $-\int e^{-sT}(1-T')a(y'+sy)\,dx$. The derivative contacts in the reconstructed baseline are $+e^{-\sigma T}a$ for the radial derivative written in split Green form, and $-e^{-\sigma T}a$ for the time derivative. Here $\sigma=s-2\Omega$. The graph commutator $[D,\Theta]w$ for each insertion is compact radially, with an entire transformed graph coefficient. Exact upper electric and tangential data yield the strong source plus graph pairing, or equivalently the weak canonical pairing with a **positive** $2\phi q_c/\lambda$ term at a finite left cutoff (notation of GOAL-011 PAIRING). The weak bulk alone has a potential $e^{2sx}$ divergence. In the model $\widehat q_c=-\widehat p=-e^{sx}p_H$ the leading strong source vanishes and the finite left term cancels the weak lower primitive. The contact terms are included even though no independent upper axial canonical data are prescribed.

Causal support is in $u\ge u_-$ and $v=t+x\ge v_-$ below the minima on the compact support. On a constant $t$ after the launch, use bounded-coefficient wave energy estimates with enough commutations for the finite derivative losses in the triangular source. On its support, $f^{-m}=O(e^{mt})$ and $r=O(1+t)$, so each source and required Sobolev energy is bounded by an exponential in $t$. For the right end one needs a bound uniform along a fixed $u$. Fix $0<\alpha<q-\Omega$ with slack and use the mixed norm weighted by $e^{-\gamma(u-u_-)-\alpha(x-X)}$. If its characteristic system is $w_{uv}=A w_u+B w_v+D w+S$, the characteristic integration constants are $L=2/\alpha$ along fixed $u$ and $K=(\gamma-\alpha/2)^{-1}$ along fixed $v$. If $G$ bounds the weighted right side, then
$$
 \|w_u\|\le U_b+LG,\quad \|w_v\|\le KG,\quad
 \|w\|\le W_b+LKG.
$$
Choose $X$ large enough for $L\|A\|<1/2$ and then $\gamma$ above the boundary growth rate and large enough for $K(\|B\|+L\|D\|)<1/2$. The inequalities apply on bounded characteristic strips and are uniform in the upper endpoint of $v$. For the massless stages use $\alpha=0$, $\|w\|\le\|w_u\|/\gamma$, and the integrable potential in the $v$-integration. The right GL jet has rate $e^{\Omega u-(q-\Omega)x}$ up to powers. The mixed estimate followed by the second GL insertion makes the strong returning source $O(e^{\gamma(u-u_-)-b x})$ for some $b>0$. The direct quadratic and EF mean terms decay faster. Integrating the massless characteristic equation gives the outgoing limit and news, each bounded by $C e^{\gamma(u-u_-)}$. At the left a time-domain source bound with growth constant $c$ and $t\ge v_--x$ gives $|\widehat S|\le C_s e^{(\Re s-c)x}$. Taking $\Re s>\max(c,\gamma,2\Omega)$, with constants enlarged across the required finite jets, suffices for absolute convergence and dominated passage to the null transform. This is a sufficient physical half-plane. The exact abscissa was not determined.

In this half-plane the derivative jump of $h(x_<)g_+(x_>)/W(s)$ is $+1$, and the transformed news is $s\mathcal T(s)$. There is no past endpoint since the trace vanishes on early cuts. The nested representation is exact there, with baseline inverse $R_0(\sigma)$, mixed inverse $R_k(s_1)$ at $s_1=s-\Omega$, and returning trace inverse $R_0^{\mathscr I}(s)$. A separated $m$-fold GL insertion shifts by $-m\Omega$ since $e^{-st}e^{m\Omega t}=e^{-(s-m\Omega)t}$. The separated mass-matched EF mean shares the double shift. The mean radion cancels in the prescribed axial projection. The denominators before possible cancellations are $W(\sigma)$, the two-polarization matching determinant at $s_1$, and $W(s)$.

There are no homogeneous poles on $\Re s>2\Omega$ on the physical branches. In particular the massive homogeneous pencil is $s_1^2+\mathcal M-s_1\mathcal C$, with $\mathcal C^*=-\mathcal C$ and nonnegative $\mathcal M$ on $H^2(\mathbb R,\mathbb C^2)$. Its derivative pieces in the quadratic form are $\| (\partial_x+a)Y\|^2+\|(-\partial_x+b)Z\|^2$ plus the nonnegative $f(k^2+\lambda/r^2)$ term. For $\Re s_1>0$ both outgoing/ingoing ends decay. Dividing the pencil equation by $s_1$ and taking the real part against the field gives
$$
 0=\Re s_1\left(\|U\|^2+\langle U,\mathcal M U\rangle/|s_1|^2\right).
$$
The massless counterpart follows with $\mathcal C=0$ and $V_{\rm RW}\ge0$. Also $\Re\kappa\ge\Re s_1$ for the physical square root $\kappa=\sqrt{s_1^2+k^2}$. For $a=\Re s_1>0$ and $b=-\Im s_1$, the discriminant $(a^2-b^2+k^2)^2+4a^2 b^2$ exceeds $(a^2+b^2-k^2)^2$ by $4a^2 k^2$. This proves the inequality by the usual formula for the real part of the square root. All right real-axis endpoint margins then hold for $\Re s>2\Omega$, including $q+\Re\kappa-\Re s\ge q-\Omega>0$. This proves holomorphy of the radial continuation there. Holomorphy alone does not establish absolute convergence of the causal transform throughout this larger domain.

## Endpoint conditions and candidate singularities

The left horizon Jost conditions and both strong sources use the regular EF frame. For example, $I^x=I^r_{\rm EF}$, $I^t=f I^v_{\rm EF}-I^r_{\rm EF}$, $I^z=f I^z_{\rm EF}$, and $\partial_x=\partial_v+f\partial_r$. The normalized mixed source terms with $\partial_v I^r_{\rm EF}$ cancel, leaving $O(f e^{s_1x})$. The returning strong source is $O(f e^{sx})$ by the same EF mechanism. Sufficient conditions are $1+2\Re \sigma>0$, $1+2\Re s_1>0$, and $1+2\Re s>0$. The first concerns the baseline ingoing Jost construction. The other two follow by pairing the strong $f e^{zx}$ source with the $e^{zx}$ ingoing basis. A failed sufficient condition can require a different normalization or further continuation, and does not imply a physical singularity.

On the far ray $r=2.5+e^{i\theta}\rho$, let $m_\theta(z)=\Re(e^{i\theta}z)$. Sufficient nonthreshold conditions include positive $m_\theta(\sigma),m_\theta(s),m_\theta(\kappa),m_\theta(q)$ for the Jost branches, $m_\theta(q+\sigma+\kappa)>0$ for the inner affine tail, and $m_\theta(q+\kappa-s)>0$ for the outer test against the possibly growing $e^{+sx}$ part of $h$. The forced massive particular and direct mean and quadratic terms additionally use $m_\theta(2q+\sigma-s)=2(q-\Omega)\cos\theta>0$. Only deform beyond the compact packet. At $s=s_q$, the physical outgoing continuation for $s_1$ crosses the lower propagating line with $-\Im s_1>k$. Then $\kappa\simeq-0.367495-0.490520i$ rather than the principal square root there. The inner real-axis rate is about $-0.040486$. On $\theta=\pi/4$ its rate is $0.846674$, and $m_\theta(\kappa)\simeq0.086992$. Off the pole the outer check cannot use a purely outgoing $h$. In the inherited horizontal corridor the bound
$$
 |\kappa-(s-\Omega)|\le \frac{k^2}{b+\sqrt{b^2-k^2}},\qquad
 m_\theta(q+\kappa-s)\ge\frac{q-\Omega}{\sqrt2}
                         -\frac{k^2}{b+\sqrt{b^2-k^2}}>0.058
$$
holds for $b=-\Im s\simeq0.747343$ and $0\le\theta\le\pi/4$. It follows from $(\kappa-(s-\Omega))(\kappa+(s-\Omega))=k^2$ and $-\Im\kappa\ge\sqrt{b^2-k^2}$. This also controls $s_q+2\Omega$ on the same corridor. In the right overlap both contour integrals can be deformed with vanishing arcs. The continued matches away from threshold invert finite holomorphic matrices there.

| Location | Spectral origin and status |
| --- | --- |
| $s=s_j$ and $s=0$ | Returning RW outgoing zeros and Schwarzschild branch endpoint. The certified physical residue at $s_q$ survives. The inherited work counts a single zero locally at $s_q$, and does not count all $s_j$ globally. The full waveform cut jump at $0$ has not been evaluated here. |
| $s=s_j+2\Omega$ and $s=2\Omega$ | Shifted baseline RW zeros and translated cut endpoint. For the fundamental $s_q+2\Omega\simeq-0.051362251-0.747343369i$, the actual baseline moment survives and the other two pencils are invertible, as shown below. The full returning residue still requires its translated overlap. Its decay exponent if excited is smaller than $|\Re s_q|$. |
| $s=\Omega\pm ik$ | Shifted massive Coulomb thresholds. The phase-renormalized massive outgoing inverse is finite at the propagating-side limit, proved below. Branch behavior, the other lip, and the physical projected jump are open. |
| $s=\Omega+s_{1,j}$ | Candidate mixed QNM or quasibound poles on their corresponding outgoing or evanescent continued sheets, requiring genuine zeros and returning overlap. There are no growing massive homogeneous modes. This analysis does not bound damped locations or rule out subthreshold accumulation. |
| Other possible endpoint obstructions | The left $1+2\Re z$ lines and far rate inequalities are convergence boundaries of the constructions above. Their failure may require new contours or radial subtractions. No global singularity count or infinity estimate follows from these conditions. |

The negative real axis is a convenient RW cut choice, translated by $2\Omega$ for the baseline. For the massive problem one can cut the square root between $\pm ik$ in the $s_1$ plane and continue through the propagating lines while keeping the branch fixed by $\Re\kappa>0$ on the right. Coulomb powers and the origin of the continuation must also be tracked. These cuts are prescriptions on sheets, and their geometric overlap is not a residue calculation. See Leaver for the single RW cut, QNM, and high-frequency remnant, and Koyama and Tomimatsu for a scalar Coulomb-threshold example. Neither gives the projected jump of the present two-polarization source. [Leaver (1986), §§II–III](https://doi.org/10.1103/PhysRevD.34.384) · [Koyama and Tomimatsu (2001)](https://arxiv.org/abs/gr-qc/0103086).

## Propagating-side threshold theorem

Consider $s_1$ near $-ik$ on the physical side. Put $z=-(s_1^2+k^2)$, $A=k^2$, and fix a sector $0\le\arg z\le\pi-\varepsilon$ with $0<\varepsilon<\pi$ (the propagating boundary has $z\ge0$). For $z$ small and real $r$ large the homogeneous normalized two-polarization equation from GOAL-011 is
$$
 U_{xx}+(z I+A I/r+M(s_1,r))U=0,qquad M=O(r^{-2})
$$
uniformly in $z$, since $V_Y,V_Z=k^2-k^2/r+O(r^{-2})$ and $C=O(r^{-2})$. For $p=(z+A/r)^{1/2}$ take $\Re p>0$, $\Im p\ge0$. Then $|p|\ge c_\varepsilon (|z|+A/r)^{1/2}$. The identities $p_x=(A/r)_x/(2p)$ and its derivative give $p_x/p=O(r^{-1})$, $p_{xx}/p=O(r^{-2})$ uniformly in the sector. With a fixed far reference point,
$$
 v_+(x)=p(x)^{-1/2}e^{i\Phi(x)},\qquad
 \Phi(x)=\int^x p\,dx,\qquad
 v_+''+p^2 v_+=Jv_+,\qquad
 J=\tfrac34(p_x/p)^2-\tfrac12 p_{xx}/p .
$$
Writing $U_+=v_+N$ gives $(v_+^2 N')'=-v_+^2(M+JI)N$. Integrating twice from infinity, the kernel multiplying $-(M+JI)N$ is
$$
 K(x,y)=v_+(y)^2\int_x^y v_+(u)^{-2}du
       =\frac{e^{2i(\Phi(y)-\Phi(x))}-1}{2i p(y)} .
$$
Its norm is at most $1/|p(y)|$. The integral norm on a far half-line is bounded by $C\int_X^\infty r^{-3/2} dx=O(X^{-1/2})$, uniformly in the sector. The contraction fixes $N=I+O(r^{-1/2})$ and $N_x=O(|p|r^{-1/2})$. Dominated convergence of the Volterra iterates and their differentiated equation gives continuous matching data at any fixed finite radius as $z\to0$ there. For $z\ne0$ in this sector the asymptotic phase of $v_+$ is $\sqrt z\,x+[A/(2\sqrt z)]\log r+O(1)$, equivalent up to a scalar to the retarded Coulomb normalization with $\kappa=-i\sqrt z$. For $z=0$, $\Phi=2k\sqrt r+O(1)$, so the right outgoing phase is $e^{+2ik\sqrt r}$ with amplitude proportional to $r^{1/4}$. Keeping the outgoing subspace and choosing a finite phase reference removes a singular scalar normalization. A fixed exponential Jost estimate based on $\Re(\kappa e^{i\theta})>0$ degenerates at this threshold.

At $s_1=-ik$ the stationary matrix is Hermitian, because $s_1C$ is real and the off-diagonal entries equal. Thus $j=\operatorname{Im}(U^\dagger U_x)$ is constant. An ingoing horizon solution $U\sim e^{-ikx}a_H$ has $j=-k\|a_H\|^2$. An outgoing threshold solution $U=U_+a_+$ has $j=+\|a_+\|^2$, since $|v_+|^2p=1$ at $z=0$ and the $N_x$ correction vanishes in the flux. A simultaneous homogeneous solution must vanish. The two endpoint subspaces are complementary and their normalized matching inverse is continuous and bounded in a sufficiently small neighborhood in the specified sector. For $s_1=+ik$ use conjugation of the real physical modes. This is the attractive Coulomb endpoint at $k>0$. Sending $k\to0$ removes the positive $A/r$ and is a different threshold problem. The sector excludes the approach $z<0$ on which $z+A/r$ can turn at a large real radius.

There is also a direct endpoint check at $z=0$. With $\xi=\Phi(x)$ and $U=p^{-1/2}H(\xi)$, the equation is $H_{\xi\xi}+H=-p^{-2}(M+JI)H$. The perturbation is $O(r^{-1})=O(\xi^{-2})$. Its plane-wave Volterra equation recovers the two phases $e^{\pm i\xi}$ and their opposite normalized currents. This checks the attractive sign of the $A/r$ term and the $r^{1/4}$ amplitude independently of taking a spectral limit.

The threshold limit extends to the forced nested transform from this sector. The baseline Wronskian at $\sigma=-\Omega-ik$ is nonzero by the interval check below, and the returning Wronskian at $s=\Omega-ik$ is nonzero by positivity. The continued baseline field on the real far axis has at most $e^{|\Re\sigma|x}$ growth, by the RW free-kernel variation of constants with integrable potential. Its first GL insertion thus decays with some uniform exponent below $q-\Omega>0$. To check the massive forced endpoint, set $P=U_+'U_+^{-1}$ for large real $r$ and $L=U_x-PU$. The complex stationary matrix is transpose symmetric. The transpose Wronskian of $U_+$ with itself vanishes at infinity by the endpoint estimates, hence $P=P^T$. The backward propagator of $L_x=-PL+S_1$ from $y$ to $x\le y$ is $U_+(x)^{-T}U_+(y)^T$. Its norm is bounded by $C(r(y)/r(x))^{1/4}$ from the scalar WKB factor and uniformly invertible $N$. The same bound controls the forward homogeneous ratio $U_+(y)U_+(x)^{-1}$. These bounds and exponential $S_1$ give a continuous affine tail and $U,U_x$ with at most polynomial growth uniformly at the limit. The initial matching includes the compact contacts and the regular ingoing left source. A second GL factor makes the outer projection against $h_s$ integrable since $q-\Re s>0$ nearby. This real-axis massive solution is connected to the physical right half-plane with $\Re s_1>0$ at fixed $-\Im s$ near $k$. The shifted baseline Jost function is continued on its far ray and brought back to real $r$. Along this path $q+\Re\sigma>0$ and $q+\Re\kappa-\Re s\ge q-\Omega$ suffice for the tails. Isolated shifted massless zeros, if any along the path, admit meromorphic detours. Dominated convergence proves a finite lateral limit of the physical returning transform at this threshold. This argument gives no estimate on a jump, tail exponent, or accumulation approaching the evanescent side.

## Isolated translated fundamental and interval controls

Put $s_*=s_q+2\Omega$ and denote the baseline meromorphic solution by $w_0(\sigma)$. From the compact graph formula its residue is
$$
 \operatorname{Res}_{\sigma=s_q} w_0=-J_h(s_q) g_+(s_q,\cdot)/W'(s_q),
 \quad
 J_h(s_q)=\int e^{-s_qT}(1-T')a(h_x+s_q h)\,dx.
$$
This holds on the entire real radial line and its continued far end by proportionality of $h$ and $g_+$ at the pole. The positivity of the real part of the moment kernel on the whole packet, checked below, proves $J_h(s_q)\ne0$ for the actual bump. Both $W(s_*)$ and the massive determinant at $s_1=s_q+\Omega$ are nonzero on the same sheet. Thus $\mathcal T$ has at most a simple pole there. To compute its residue, apply the returning trace Green operator to the complete direct $H^2/B_E$ insertion of the baseline residue and the second GL insertion of its massive retarded response at $s_q+\Omega$. Pure graph contacts are holomorphic at the translated pole since they use finite local Cauchy jets and the two downstream inverses there are holomorphic. This isolates a single candidate residue that still needs the full projection, including radial cancellations. Using the GOAL-011 value at the unshifted pole, or a denominator alone, cannot certify that projection.

The input $s$ box encloses the certified disk and the checks retain the GL rate uncertainty. Run from the scoped research directory with Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0, and python-flint 0.9.0.

```sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-012/checks/a_spectral.py
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 python3 GLR-GOAL-012/checks/shifted_denominators.py
```

[Threshold and packet check](../checks/a_spectral.py) · [Recorded threshold and packet bounds](../checks/a_spectral.txt) · [Shifted denominators](../checks/shifted_denominators.txt)

The threshold baseline $D$ enclosed $0.4783256-0.1515783i$ with component uncertainties $6.97\times10^{-8}$ and $4.09\times10^{-8}$. At the translated fundamental, the returning $D$ enclosed $0.204908-0.392937i$ with uncertainties $8.45\times10^{-7}$ and $5.21\times10^{-7}$, and the massive determinant enclosed $0.1988-0.0103i$ with uncertainties $1.57\times10^{-5}$ and $4.57\times10^{-5}$. The compact-moment kernel real component had cell outputs $[0.5\pm0.0109]$, $[0.5\pm0.0239]$, $[0.53\pm0.00879]$, $[0.5\pm0.0510]$. This uses $T=-\log(r-1)$, $1-T'>0$ and the strictly positive bump in its support interior. The initial inline horizon-series call at the default Flint series cap failed its precision assertion. Setting `ctx.cap=80` as in the pinned validator resolved that setup error. The reproduction commands rerun the bounded checks.

## Evidence and remaining questions

- **Proved under pinned inputs**. A sufficient physical right half-plane and exact nested causal pairing with contacts, a pole-free radial continuation on $\Re s>2\Omega$, endpoint continuation and singularity criteria local to the inherited pole corridor, and the one-sided Coulomb-threshold matching result. The exact physical convergence abscissa remains open.
- **Interval checks using inherited validators**. Noncoincidences at the translated fundamental and baseline noncoincidence at the lower massive threshold. The translated fundamental's baseline excitation is strictly nonzero for the actual finite packet.
- **Unresolved**. Enclosure of the complete translated returning residue, the shifted cut strengths on the continued physical projection, possible massive subthreshold modes and both threshold lips, and estimates uniform in large imaginary frequency needed for contour closure and the prompt term. The pole-pair subtraction only removes local Laurent parts. No finite-time dominance or finite-seed waveform conclusion follows here.
