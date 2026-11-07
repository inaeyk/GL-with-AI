# Complete quadratic source

Set \(r_0=1\), \(k=\pi/5\), \(K=2k\), and \(\Omega\in[0.06328118,0.06328120]\). Take \(\bar g=-f\,dv^2+2\,dv\,dr+r^2d\omega^2+dz^2\), \(f=1-1/r\), and work with \(\mathrm{Ric}(\bar g)=0\). Expand \(g=\bar g+\epsilon h+\epsilon^2q\) with no factorial. Complete the first-order mode to unitary gauge \(h_{zA}=0\). Its \(+1\) coefficient is \(e^{\Omega v}\) times one half of the regular EF radial profile of the cosine mode. The \(-1\) coefficient is its conjugate. Only \(+1\) times \(+1\) contributes to \(+2\). The real \(n=\pm2\) field also includes the conjugate coefficient. The use of a completed first-order gauge is essential because a change of first-order gauge induces \(\mathcal L_{\xi_1}h+\tfrac12\mathcal L_{\xi_1}^2\bar g\) in the quadratic field.

Let \(C^A{}_{BC}=\tfrac12\bar g^{AD}(\bar\nabla_Bh_{DC}+\bar\nabla_Ch_{DB}-\bar\nabla_Dh_{BC})\) and \(D^A{}_{BC}=-h^A{}_E C^E{}_{BC}\). Then
\[
 Q_{AB}=\Pi_{+2}
 [\bar\nabla_C D^C{}_{AB}-\bar\nabla_B D^C{}_{AC}
 + C^C{}_{CD}C^D{}_{AB}-C^C{}_{BD}C^D{}_{AC}],\qquad
 Q_E=Q-\tfrac12\bar g\,\operatorname{tr}_5 Q .
\]
The equation is \(R^{(1)}[q]=-Q\). On the completed first jet \(\bar\nabla^A(Q_E)_{AB}=0\) exactly by the order-two contracted Bianchi identity. The linear Ricci and trace-reversed Bianchi point checks include zero, while the deliberate omission of \(Q_{zz}\) excludes zero in the Bianchi residual. The compact-normal Ricci identity gives the independent absolute-scale check \(Q_{zz}=-3k^2\operatorname{tr}_4(H^2)/16\), with the common time factor removed and \(H\) the unhalved profile in the same coordinate convention. It detects the factor of four from dropping both cosine halves.

## Point intervals

[Full numerical record](checks/source_intervals.jsonl) gives both real and imaginary bounds for all seven independent \(Q_{AB}\) and \(Q_E\) components at each of \(r=1,2,4,8,16\), including the second implementation's Ricci intervals, normalized linear mode values, scale controls and Bianchi diagnostics. The angular \(\varphi\varphi\) component equals \(\sin^2\theta\) times the recorded \(\theta\theta\) component. Other components follow by symmetry or vanish. Bounds are for the complex \(+2\) coefficient at \(v=0\), retaining the ingoing EF component basis. To obtain values on \(\tau=0\) in that same basis, multiply by the recorded \(e^{2\Omega r}\) interval. A tensor basis change is separate.

For example, \(Q_{zz}\) lies in \([-82.99491,-82.74331]\) at the horizon and in \([-0.882202,-0.882155]\) at \(r=2\). The horizon Frobenius recurrence evaluates the regular jet directly. Exterior values use the pinned validated backward Riccati flow and outward Taylor integration of the normalized log-amplitude. The two curvature implementations share the same spectral inputs and jet reconstruction. One expands the full inverse, Christoffels and Ricci tensor through order two in five coordinates. The other uses a separate \(2\times2\) orbit inverse and the exact warped-sphere identities with squared sphere radius \(W\),
\[
 R^{(5)}_{ab}=R^{(3)}_{ab}-W^{-1}\nabla_a\nabla_b W
             +\frac{\partial_aW\,\partial_bW}{2W^2},\qquad
 R^{(5)}_{\theta\theta}=1-\tfrac12\Box_3 W.
\]
The checks also compare two off-axis angular evaluations and trace-reversed source divergence. See [reproduction](REPRODUCIBILITY.md) for exact inputs, commands and limits. The enclosures are pointwise. Continuous error bounds for the physical retarded projection and gravitational flux remain open.

[GOAL-005 enclosure and source regularity](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/physics/goal005_enclosures.md) · [GOAL-003 source functional and separated tail](https://github.com/inaeyk/GL-with-AI/blob/80f030df0bec99bebe76fd9ab473c419ad1af5a5/research/sav/physics/goal003_forced_master.md)
