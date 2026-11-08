"""Non-production full-packet exploratory extension of the pinned solver.

The early-window and full-packet abscissae have different widths. Recompute
both canonical graph velocities at the enlarged cut. The remaining solver,
including the returning graph and potential transfer, is inherited.
"""

import argparse
from pathlib import Path
import platform
import sys

import numpy as np
from scipy import __version__ as scipy_version
from scipy.integrate import quad

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-014/checks"))
import c_compact as pilot


def full_packet(n, m, cap, iterations, tol, outer, output):
    base_strip = pilot.Strip
    base_velocity = pilot.point_velocities

    class PacketStrip(base_strip):
        def __init__(self, _early_width, cap, n, m, rule):
            super().__init__(pilot.DELTA, cap, n, m, rule)

    def graph_velocities(gl, n):
        # u = u_* + delta*y on the canonical v=r graph.
        s = PacketStrip(None, cap, n, 1, "simpson")
        u, b = s.u, s.b
        f, p = 1-1/b, -1/b
        normalizer = np.sqrt(quad(lambda y: pilot.packet(y)[1]**2, 0, 1,
                                  epsabs=1e-14, epsrel=1e-13)[0])
        a, ap, _ = [pilot.DELTA**(.5-j)*x/normalizer for j, x in
                    enumerate(pilot.packet((u-pilot.FRONT)/pilot.DELTA))]
        at, ax = (f*a/b-ap)/(2*b), ap/(2*b)
        vv, vr, rr = np.array([gl(r) for r in b]).real.T[:3]
        time = np.exp(pilot.OMEGA*b)
        ht, htx, hx = time*vv, time*(vv+f*vr), time*(vv+2*f*vr+f*f*rr)
        jt = -pilot.LAM*b*b*(ht*at-htx*ax)/(2*f)
        jx = -pilot.LAM*b*b*(-htx*at+hx*ax)/(2*f)
        return np.stack(((p*jt-jx)/(1-p*p),
                         pilot.K*(jt-p*jx)/(pilot.LAM*b*b*(1-p*p))), axis=-1)

    print(f"Python {platform.python_version()} NumPy {np.__version__} SciPy {scipy_version}")
    pilot.Strip, pilot.point_velocities = PacketStrip, graph_velocities
    try:
        pilot.calculate(n, m, cap, iterations, "simpson", tol, outer, output)
    finally:
        pilot.Strip, pilot.point_velocities = base_strip, base_velocity


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--n", type=int, default=192)
    parser.add_argument("--m", type=int, default=720)
    parser.add_argument("--cap", type=float, default=400)
    parser.add_argument("--iterations", type=int, default=24)
    parser.add_argument("--tol", type=float, default=8e-14)
    parser.add_argument("--outer", type=float, default=430)
    parser.add_argument("--dump")
    args = parser.parse_args()
    full_packet(args.n, args.m, args.cap, args.iterations, args.tol, args.outer, args.dump)
