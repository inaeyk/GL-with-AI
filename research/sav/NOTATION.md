# Conventions and notation (initial lock; independently check in GOAL-001)

- Signature `(-,+,+,+,+)`; vacuum action
  `S_5=(16 pi G_5)^{-1} int d^5x sqrt(-g_5) R_5`, with standard GHY
  terms as required. Natural units c=1.
- Coordinates `X^A=(t,r,theta,varphi,z)`; `z ~ z+L`.
  The transverse 2-sphere is round under exact SO(3).
- Uniform Schwarzschild string:
  `ds^2=-f dt^2+dr^2/f+r^2 dOmega_2^2+dz^2`,
  `f=1-r_0/r`; horizon at `r_0`. Gullstrand–Painleve time differs from
  static Schwarzschild time. The numerical cartoon x and metric factors must
  never be identified naively with asymptotic areal r.
- Initial ADM mass `M_0=L r_0/(2 G_5)=r_0/(2 G_4)` and
  `G_4=G_5/L` for a fixed asymptotic circle length.
- Compact Fourier convention:
  `F_n=(1/L) int_0^L dz F(z) exp(-i k_n z)`,
  `F(z)=sum_n F_n exp(i k_n z)`, `k_n=2 pi n/L`,
  `F_{-n}=F_n^*` for real F. For `A cos(k_1 z)`, each
  Fourier coefficient is A/2; no hidden factor of two.
- A pure GL eigenmode is `a(t) h_1(r) cos(k_1 z)`,
  `a(t)=epsilon p_1 exp(Omega_GL (t-t_0))`. Choice of a
  *geometric* normalization and overlap p_1 must be stated; numeric chi
  amplitude and gauge-invariant GL amplitude differ.
- `u=t-r_*` only at leading asymptotic order; exact `u` must be defined
  via 4D Einstein-frame Bondi coordinates. `rho` is the 4D Einstein-frame
  areal radius on spheres of constant u. A finite-radius `x` is not `rho`.
- KK reduction for *z-independent zero modes*:
  `ds_5^2=e^{-2 phi/sqrt(3)} ds_{4,E}^2
    +e^{4 phi/sqrt(3)}(dz+2 A_mu dx^mu)^2`.
  `S_4=(16 pi G_4)^{-1} int sqrt(-g_E)
   [R_E-2 (nabla phi)^2-e^{-2 sqrt(3) phi} F^2]`.
  At finite radius in a z-dependent geometry, extracting nonlinear phi
  requires an asymptotic gauge/KK projection, not raw log average g_zz.
- Asymptotic zero-mode radion:
  `phi_0(u,rho)=Phi(u)/rho+O(rho^{-2})`.
  `N_phi=partial_u Phi`.
  With this normalization, `dE_0/du=(partial_u Phi)^2/G_4`,
  `E_0/M_0=(2/r_0) int du (partial_u Phi)^2`.
  Fourier transform `Phi_tilde(omega)=int du exp(i omega u) Phi(u)`.
  Positive-frequency spectrum:
  `dE_0/domega=omega^2 |Phi_tilde|^2/(pi G_4)` for omega>0,
  **provided** boundary terms are treated correctly (for a memory step
  use Fourier transform of news rather than an ordinary integrable Phi).
- Massive n!=0 threshold in weak asymptotic field:
  `q_n^2=omega^2-k_n^2`. Propagation if `|omega|>|k_n|`;
  no massive energy flux through generic 4D future null infinity.
- Define `X= t/r_0`, `lambda_L=L/r_0`,
  `hat Omega=r_0 Omega_GL`; set `r_0=1` only in stated
  numerical conventions.

Status: definitions and elementary algebra checked by inspection;
derivation of nonlinear/asymptotic gauge invariance and canonical flux
normalization from the full reduced action remain GOAL-001 proof obligations.
