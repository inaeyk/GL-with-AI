# Investigator C — numerical validation

The calculation uses the pinned GOAL-013 axial equations, the
GOAL-011 real GL master and EF Ricci source, and the fixed benchmark.
The spectral box and whole-cell GL graph machinery are inherited.
The compact characteristic candidate, its return decompositions,
discrete transpose and candidate-dependent cap threshold are new.
The source assembly follows the hierarchy independently of the
old frequency-domain overlap integrator. Agent B supplied independent
tensor coefficient and graph-contact formulas before the first
complete diagnostic. This comparison was non-blind.

The early strip used two simultaneous mesh and cap refinements.
The cap differences are not independent spatial and temporal
discretization estimates.

| $N_u,N_v$ | Minimum cap radius | Weak cap projection | Strong cap projection | Source-free exterior candidate |
| --- | ---: | ---: | ---: | ---: |
| 48, 180 | 40 | 7.82064819095 | 7.82061284114 | 7.81778232565 |
| 96, 360 | 80 | 7.81923683071 | 7.81923466465 | 7.81781505279 |
| 192, 720 | 400 | 7.81810240688 | 7.81810227291 | 7.81781765437 |

The outward continuation to $1/r=0$ independently propagates the
potential-only cap candidate. The actual exterior sources are covered
by the audited absolute bound only at the large cap. The closer caps
are empirical sensitivity checks. The exact graph and mesh spacings
are not changed in any precision replays.

At the last cap the candidate has $mathcal F(d)=193.36257$ and
$g_c(d)=0.41255321$, giving a positive terminal-cut contribution
$19.94309$ larger than the net projection. The bulk contribution
therefore cancels about 61 percent of the terminal term. Removing the
terminal term numerically reverses the sign. The $F$ absolute ray
integral at the endpoint is about $293.516$, so cancellation also
occurs within that single ray. The mixed and mean return are present.
Both massive components are evolved with nonzero induced graph
velocities. A deliberate strong-return control using zero induced
**returning** velocities with the same bulk source gives
$7.81827638701$ instead of $7.81810227291$ on the fine cap.
That control changes only the return graph pair and is outside the
physical preparation. The numerically small projection difference
does not erase the exact contact mismatch.

Three evaluated numerical results have directed status conditional
on the inherited operators and input certificates. The uncommuted
one-mode mixed graph energy defect is below $4.10\times10^{-7}$.
The physical graph-exact baseline comparison gives both uncommuted
and once-commuted outgoing flux error bounds.
The affine $p_c$ reference projection and norm give a sufficient
candidate-specific condition $E_c\le570$ for the early sign. The
strong candidate uses binary ordinates, whose interpreted
interpolant is then exact. The residual of those ordinates against
the physical hierarchy has no directed enclosure. [Proof and gate](../STRONG-NEWS-PILOT.md)

The dominant missing evidence is a conforming compact mixed and
returning lift with directed coefficient and source errors,
whole-cell derivative residuals and interface and commuted graph
terms. For the primary weak route the terminal $F$ ray is the
first high-weight quantity. The full packet was held behind the
early error gate. No longer waveform or QNM calculation was used
to infer a physical sign.

## Mixed weak source identity for a future conforming lift

The calculation above uses the strong mixed equations. A separate exact
rewriting puts both rows in the return-style derivative-reduced form.
Set $j_a=J^a/\sqrt D$, $B_a=k L_c J^a/\sqrt W$, and take the
column vectors
$$
 P=(-j_x,B_t)^T,\quad Q=(-j_t,B_x)^T,\quad
 H=(-a_m j_t+f\sqrt D X/r^4,-b_mB_x)^T .
$$
Then for the real pair $\boldsymbol w=(y,Z)^T$, the right side of
$-4\boldsymbol w_{uv}-V_m\boldsymbol w$ is $-F_u-G_v+H$, with
$$
 F=P-Q+C_{\mathbb R}\boldsymbol w,\qquad
 G=P+Q+C_{\mathbb R}\boldsymbol w .
$$
The divergence of the two added terms is
$C_{\mathbb R}\boldsymbol w_t$ because $C_{\mathbb R}$ is
stationary. On the graph both mixed master displacements vanish,
and the two induced velocities give $4\boldsymbol w_u=G-b'F$.
In the compact row the equality uses
$\sqrt W/(\lambda r^2)=L_c/\sqrt W$.
This produces a continuous two-row Volterra equation for the shear
from undifferentiated baseline currents. The return currents also
involve the first mixed jets, so recovering the needed directional
derivative traces with quantitative error remains a distinct step.
