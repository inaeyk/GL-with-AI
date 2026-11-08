# Retained negative controls and numerical warnings

Both full-packet solver runs and the inherited fine early replay exited successfully after emitting the following SciPy warning, with its host path removed.

~~~text
scipy/integrate/_ivp/rk.py:691: RuntimeWarning: invalid value encountered in scalar divide
  return np.abs(h) * err5_norm_2 / np.sqrt(denom * len(scale))
~~~

The compact strong calculation that sets the returning graph velocity to zero deliberately violates the canonical upper graph condition. In the refined full packet it gives `P=-1454.62718179`, versus `-1455.61471298` with the induced return jet. The distinct early omitted-terminal control changes the reconstructed candidate interval to `[-12.128703,-12.118308]`. A wrong launch or omitted contact is not a numerical convergence test.

The raw interval-substitution lift probe loses cancellation within a cell. On the first rectangular panel of the terminal ray its `R_y` interval has magnitude up to 42.093. At the panel start point the bound is 3.228. Both are valid upper estimates, and their difference does not measure the physical residual itself. A simple sum of broad cell ranges has not been evaluated or claimed as a physical certificate.

The early relative lower-jet method does not apply across the entire packet. At its midpoint the physical `w_{0,u}` is nonzero on every outgoing ray segment above the graph, but the free seed derivative vanishes. See the exact midpoint control in [the compact record](COMPACT.md).
