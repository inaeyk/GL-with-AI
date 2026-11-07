# Dual check of the axial trace

For the orbit metric $B$ and axial invariant $A$ use
$Q=R^4 *_B dA$ with orientation $dt\wedge dx\wedge dz$.
The Proca equation implies
$$
A=(\lambda R^2)^{-1}*_B dQ,\qquad
D_b(R^{-2}(dQ)^{ba})=\lambda R^{-4}Q^a .
$$
This uses $*_B^2=-1$ on orbit one- and two-forms and
$*_B d(R^{-2} *_B dQ)=\lambda R^{-4}Q$.
The dual divergence gives $D_aQ^a=4Q^a\partial_a\log R$.
Substitution gives a normally hyperbolic equation with principal
symbol $B^{ab}\xi_a\xi_b$ times the identity. In the background
rescaled components $Q/r$, the first-derivative coefficients are
$O(r^{-1})$ at large radius. Reflection leaves only $Q_z$ in the
mean. Its background scalar equation is
$$
-\partial_t(r^{-2}\partial_t Q_z)+\partial_x(r^{-2}\partial_x Q_z)
-\lambda f r^{-4} Q_z=0 .
$$
Thus $\Psi=2Q_z/(\lambda r)=-2\psi/\lambda$ in the investigator's
notation. It satisfies the RW equation with
$V=f[\ell(\ell+1)/r^2-6M/r^3]$. In the pure mean geometry
$Q_z=R^4(*_B dA)_z$ is independent of the radion: $R^4$
scales by $e^{-4\varphi/\sqrt3}$, the two inverse noncompact
metrics by $e^{4\varphi/\sqrt3}$ and the orbit volume density
equals $\sqrt{|b_E|}$. Nonzero-mode extraction products decay
on the wedge by the mixed estimate below.

An alternative finite-data prescription uses spatial $Q_i$ and its
densitized normal electric field. Prescribe only their $z$
components, independent of $z$ and fixed in the completed chart.
The electric density has zero divergence and the algebraic dual
Gauss constraint gives $Q_n=0$. The electric definition and
weighted divergence determine all initial velocities. This gives
the same highest front orders as [the primary construction](INVESTIGATOR.md).
The detailed smooth tails can differ because the prescriptions fix
different canonical variables on the deformed orbit.

For the mixed equation write $W=Q/r$ and
$W_{uv}=A_1W_u+A_2W_v+CW+S$, with $A_i=O(r^{-1})$, $C$ bounded.
For $v$ beyond the compact initial part of the strip let $I_u$
integrate from the front and $J=W_u$. The identity
$$
(1-A_2 I_u)J_v=(A_1+C I_u)J+S
$$
has a bounded Volterra inverse when $d\sup|A_2|<1$ for strip
width $d$. The growth is polynomial times $\exp(C_1d v)$.
Choose $d$ so the latter exponent is below the decay exponent of
the GL jet. Any fixed launch radius in the comparison region is
allowed. Local hyperbolic evolution covers the finite initial portion
before the weighted estimate begins. Commuting a fixed number of derivatives gives the same
bound with additional polynomial factors. In the mean equation,
the direct spherical Einstein-frame perturbation is exponential
after mass matching, and the radion cancels. The mixed field and
its derivatives are always multiplied by the decaying GL first
jet. Hence the mean forcing and needed derivatives are integrable
in $v$. The scalar RW potential is also integrable. The same
identity for the mean has an integrable growth coefficient and
gives a bounded differentiated trace. This estimate only uses
the fixed-order equations on the far strip.

The normalized principal perturbation is
$-h_{\mathsf L\mathsf L}\partial_u^2/f$. On the leading outgoing
$Q_z/r$ polarization it gives the transport
$1,-\chi\cos kz,\chi^2\cos^2(kz)/2$ at successive orders.
Other components have zero baseline leading polarization.
Terms with fewer front derivatives, including a direct order-two
metric or clock perturbation, occur one power later at the
front. The mean transport coefficient agrees with
[the front bound](WITNESS-BOUNDS.md).
