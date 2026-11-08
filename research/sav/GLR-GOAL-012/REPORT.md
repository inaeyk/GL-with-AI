# GLR-GOAL-012 — Finite-time control and the translated fundamental

**Outcome B.** For the fixed-canonical \(R/r_0=13/8\), \(\delta=0.12\) axial quadrupole in five-dimensional vacuum at \(L/r_0=10\), the physical ordered \(\epsilon^2\eta\) trace has a certified translated pole in addition to the GOAL-011 returning fundamental. The two pole contributions can be compared quantitatively on finite windows. A bound on the complete finite-time remainder and the physical interference signs at this width remain open.

## A second certified pole

Use \(u=u_{\rm phys}/r_0\), the fixed ADM mass, circumference, Bondi frame and \(u_0=0\), with \(s\) in units of \(r_0^{-1}\). The returning pole is \(s_q\simeq-0.177924631-0.747343369i\). The inherited exact graph and the inserted growing GL mode give the arguments \(\sigma=s-2\Omega,\ s_1=s-\Omega,\ s\). At \(s_*=s_q+2\Omega\simeq-0.051362253-0.747343369i\), the baseline has its ordinary fundamental. The pinned bump excites that mode, the massive and returning inverses are regular, and the complete translated returning overlap survives. Its shear residue is
\[
 A_*\in[-0.21850,-0.16857]+i[-0.44959,-0.39966]
 \qquad\text{in units of }\epsilon^2\eta r_0 .
\]
The enclosure includes the mixed insertion, direct quadratic term, fixed-mass mean, strong horizon collar, and the rotated outer tail with the off-pole ingoing test. It uses fresh matching bounds for this packet and shifted frequency. Weak and strong quadratures and symmetric off-pole samples provide empirical checks, with the converged value near \(-0.19365677-0.42461147i\). A wide-cell strong quadrature and a loose interval normalization failed and are preserved. [Spectral atlas and error caps](SPECTRAL-ATLAS.md) · [Directed certificate](checks/shifted_enclosure.txt) · [Review](INDEPENDENT-REVIEW.md).

The shifted massive Coulomb thresholds admit a finite propagating-side limit for the full continued trace in specified sectors. At the threshold the two radial phases behave as \(r^{1/4}e^{\pm2ik\sqrt r}\) up to asymptotic factors. The flux sign gives complementary outgoing and ingoing endpoint subspaces. This controls that side of the threshold and leaves the opposite approach, projected cut jumps and possible massive resonances open. The two massless branch endpoints, their strengths and a globally closed frequency contour also remain open. [Spectral atlas](SPECTRAL-ATLAS.md) · [Contour and inversion](CONTOUR-AND-INVERSION.md).

## Finite windows and the ordered late-window obstruction

Define the fundamental and translated news pairs as \(Q=2\Re(s_q A_{21,q}e^{s_q u})\) and \(S_*=2\Re(s_* A_*e^{s_*u})\), with a subtraction onset before the window. The GOAL-011 residue is \(A_{21,q}\in[0.08223,0.18771]+i[-0.42418,-0.31707]\). The shifted pair has more than \(1.04495\) times the fundamental pair's \(L^2\) norm on every window of at least one exact period beginning at or after the packet's support cut \(u_*+0.12\). On \([0,8.407362]\) the ratio is in \([1.2109,2.0388]\). For the full remainder to be less than half the fundamental there, the news after subtracting both pairs must have norm above \(0.6228\) and cancel part of the shifted pair. It has not been bounded. [Norms and dominance](QNM-DOMINANCE.md) · [Directed norm outputs](checks/qnm_norm.txt).

For the formal ordered coefficient, a uniform bound \(\|N_{21}-Q\|_2\le\rho\|Q\|_2\) on all sufficiently late consecutive periods would make the remainder transform holomorphic for \(\Re s>\Re s_q\). The physical sheet contains the verified \(s_*\) pole there, contradicting this bound. Every finite \(\rho\) must be exceeded on arbitrarily late period bins. This gives no first violating interval and leaves finite-window cancellations and finite-seed validity unresolved. [Proof](QNM-DOMINANCE.md#a-late-window-obstruction-for-the-formal-ordered-response).

Two independent error routes are available. The causal energy method controls the finite-diamond \(L^2\) news residual and a computable correction to null infinity. The tapered positive-line inverse uses \(|W(s)|\ge2|s|\), weighted source derivatives and a finite-band enclosure. Neither was supplied with the complete evaluated physical source and residuals in this Goal. A fit or an isolated Laurent coefficient does not supply those inputs. [Causal estimator](FINITE-TIME-WAVEFORM.md) · [Inversion estimator](CONTOUR-AND-INVERSION.md).

## Finite-width tensor interference

The coefficient of the tensor energy at order \(\epsilon^2\eta^2\) is \(12r_0/(16\pi G_4)\) times \(\int N_{01}N_{21}\,du\). Against the exact causal baseline, the optical reference for this last integral is in \([-1392,-1149]\) on the full packet cut and \([6.52,7.84]\) on \([u_*,u_*+0.015]\). The full returning-news residual can change either sign. Sufficient strict \(L^2(dy)\) bounds on the scaled residual are \(16.45\) and \(6.88\), respectively. These are explicit tests on the physical residual and no such residual enclosure was obtained. [Flux and error budget](FLUX-AND-ERRORS.md).

The next calculation is a directed causal evolution of the pinned lower hierarchy and return through \(u=8.407362\), with graph and whole-cell residuals and a Bondi tail enclosure. It would permit a physical dominance test on \([0,8.407362]\) and signed flux on the initial two cuts without closing a global frequency contour. [Handoff](HANDOFF.md).

[Goal](GOAL.md) · [Investigator exposure and verification](INDEPENDENT-REVIEW.md) · [Reproduction](checks/README.md) · [Ledger](LEDGER.md) · [GOAL-011 source PR #13](https://github.com/inaeyk/GL-with-AI/pull/13) · [Work audit PR #14](https://github.com/inaeyk/GL-with-AI/pull/14).
