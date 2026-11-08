# Auditable sign and strong-news budgets

All bounds use dimensionless $u,v,r$ with the fixed physical cuts.
A directed compact value $[p_-,p_+]$ and upper error $E$ give a strict
positive result if $p_->E$ and a strict negative result if $p_+<-E$.

## Weak-current and adjoint route

At the cap $v=c=d+2x(400)$ use the transported exact test
$h=U(\infty,c)^*N_{01}$. Solve the scalar backward characteristic
adjoint on the graph domain. The compact formula and the separately
bounded exterior weak pairing cancel the artificial $G$ cap contact.
For $A_F(u)=\int_{b(u)}^c |F-F_h|dv$,
$A_H(u)=\int_{b(u)}^c |H-H_h|dv$ and
$A_G(u)=\sup_{[b(u),c]}|G-G_h|$, write $f_*,h_*,g_*$ for their
uniform upper bounds. Include the terminal $F$ ray in $f_*$.
The error on the early window obeys the conditional inequality
$$
 E_{\rm weak}\le
 0.206883890 f_*+0.000202708 h_*+0.000004778 g_*
 +E_{\rm test}+E_{\rm dual}+E_{\rm quad}+6.948\times10^{-16}.
$$
On the packet window the corresponding weights are
$4.927654673,\ 0.082180215,\ 0.014275917$ and the weak exterior
contribution is below $1.63462\times10^{-14}$. If a separate strong
exterior calculation is used, the cap $G$ contact remains in the
compact pairing. Then the $G$ weight equals the $H$ weight and the
exterior error is below $2.32221\times10^{-8}$ early and
$1.7017001\times10^{-6}$ on the packet. These estimates use different
exterior norms. [Adjoint derivation and constants](ADJOINT-AND-ENDPOINTS.md).

On the early window the sharp endpoint coefficient is
$g(d)/4\in[0.1031368191,0.1031368248]$ and the front term is zero.
The remaining $F$ weight may be applied to the bulk, separately from
$A_F(d)$. At the empirical candidate $p\simeq7.8181$, a zero-budget
estimate for all other errors would allow a terminal ray error of
roughly $75.8$, or roughly $37.8$ for the uniform $f_*$ alone.
Reserving half the empirical margin for the other errors would require
about $37.9$ for the terminal ray alone. These are target tolerances.
The [signed terminal adjoint](ADJOINT-AND-ENDPOINTS.md) also converts
the signed-ray error into lower-field flux and weighted residuals.
The sensitivity multipliers evaluated at central geometry are empirical.

| Budget input | Status |
|---|---|
| GL spectral input | Inherited directed normalized mode, growth bracket and global jet envelopes. Early graph retransport is directed. Coefficient differences to the chosen physical bulk candidate remain unbounded. |
| Baseline and test | Whole-window $L^2$, derivative and endpoint errors are evaluated. Uniform early physical compact jet errors through second order are available for the free outgoing candidate. |
| Both mixed polarizations | Induced graph jet and uncommuted incoming graph error are directed. The physical mixed bulk error and the commuted reconstruction required by the strong route remain open. |
| Fixed-mass mean | Directed complete constraint data on the early graph and directed exterior bounds. A separate whole-cell mean error through the compact bulk remains open. |
| Compact weak currents | Empirical on the early strip. No outward $f_*,h_*,g_*$ covering the complete physical current reconstruction. The sharp terminal ray is a separate requirement when using bulk norms. |
| Dual transfer | Exact formula with evaluated operator weights. Approximate test, dual defect and arithmetic are separate. The discrete transpose check supplies an algebraic comparison of the candidate and does not bound its continuum residual. |
| Candidate quadrature and interfaces | A current pairing needs directed quadrature of the compact reconstruction and test. All strong seams require matching derivatives or included distributional jumps. No complete physical weak quadrature budget was evaluated. |
| Exterior forcing and weak tails | Separate, directed conditional estimates. At $400$, the $F,H$ ray integrals are below $3.354\times10^{-15}$, and the $F,G$ suprema below $2.739\times10^{-15}$. These apply on the terminal ray as well. |
| Strong news transfer | Scalar potential, exterior forcing and cap continuation are controlled. The full compact cap error $E_c$ is unevaluated. |

The explicit $F$ weight is the largest in the simple uniform-ray budget
and the terminal and interior $F$ pieces dominate the empirical
cancellation. The numerical discrepancy between cap radii and meshes
does not rank the unknown true bulk errors. The mixed block supplies
the largest empirical projection. Its volume residual remains the
principal unevaluated stage. [Current data](CURRENTS-AND-DATA.md) ·
[Baseline bounds](PHYSICAL-PROJECTION.md).

## Independent early strong-news gate

The affine $p_c$ through the stored binary pilot ordinates at the exact
192-panel early cut coordinates is a deterministic candidate. Directed
evaluation of its pairing with the free launch gives
$\langle B',p_c\rangle\in[7.80653,7.86816]$ and
$\|p_c\|_2\simeq635.495$. At this stage the interval encloses a
candidate calculation. A physical proof still needs
$E_c\ge\|(w_2)_u(\cdot,c)-p_c\|_2$. Using the full scalar potential
transfer, the physical baseline error and inherited source tail gives
$$
 |\mathcal P-\langle B',p_c\rangle|
 <0.024231+0.013648127 E_c .
$$
Thus $E_c\le570$ would be sufficient for positivity on the early cut.
In particular the directed candidate arithmetic does not close the
inequality. The cap norm, baseline error and this early sufficient
bound are separate from the longer-cut GOAL-013 waveform formula.
[Candidate and residuals](STRONG-NEWS-PILOT.md).

## Open estimates in causal order

First certify a conforming mixed solution on the early graph strip
against the complete lower source. Include the geometry and mean
interpolation intervals, graph errors, all cell residuals and seam
terms. Transfer the errors to the signed terminal $F$ ray and the
weighted bulk pairing. A bound of the above size for the strong cap
error would independently close the same physical sign. Neither
inequality has been evaluated for the complete physical candidate.
Extending either solve to the full packet before this gate would
increase its domain and require a separate error budget.
