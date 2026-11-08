"""Manufactured late scalar RW news with a compact v-slab source.

This finite-u floating-point exercise illustrates the exact construction
in REVIEW.md. It does not evaluate any physical GL current.
"""

from math import log
from platform import python_version

import numpy as np
import scipy
from scipy.integrate import cumulative_trapezoid, solve_ivp


FRONT = -13 / 8 - 2 * log(5 / 8)
PACKET = FRONT + 3 / 25
QNM_END = 8.407362
V0 = 14.0


def bump(u, a, b):
    y = (u - a) / (b - a)
    values = np.zeros_like(u)
    interior = (0 < y) & (y < 1)
    z = y[interior]
    values[interior] = np.exp(-1 / (z * (1 - z))) / (b - a)
    return values


def coefficient(u, y):
    """Return (V_RW/4) dv/dy at v=V0+2*y/(1-y)."""
    if y == 1:
        return np.full_like(u, 3.0)
    v = V0 + 2 * y / (1 - y)
    x = (v - u) / 2
    r = np.maximum(1.2, x - np.log(x))
    for _ in range(8):
        r -= (r + np.log(r - 1) - x) * (r - 1) / r
    f = 1 - 1 / r
    return 1.5 * f * (2 - 1 / r) / (r * (1 - y)) ** 2


def check(n):
    u = np.linspace(FRONT, 11, n + 1)
    q = bump(u, 0.5, 4.5) - bump(u, 9, 10)

    def rhs(y, p):
        shear = cumulative_trapezoid(p, u, initial=0.0)
        return -coefficient(u, y) * shear

    solve_options = dict(method="DOP853", rtol=3e-11, atol=3e-13)
    backward = solve_ivp(rhs, (1, 0), q, t_eval=[0.5, 0], **solve_options)
    assert backward.success
    p_slab_top = backward.y[:, 0]
    forward = solve_ivp(rhs, (0.5, 1), p_slab_top, **solve_options)
    assert forward.success
    q_returned = forward.y[:, -1]
    window = (0 <= u) & (u <= QNM_END)
    packet = u <= PACKET
    q_norm = np.sqrt(np.trapezoid(q[window] ** 2, u[window]))
    assert np.max(np.abs(q_returned[packet])) == 0
    assert np.max(np.abs(q_returned - q)) < 2e-11
    print(
        f"{n:4d}  {q_norm:.11g}  {np.trapezoid(q, u):+.3e}  "
        f"{np.max(np.abs(q_returned[packet])):.1e}  "
        f"{np.max(np.abs(q_returned - q)):.3e}  "
        f"{np.max(np.abs(p_slab_top - q)):.8g}"
    )


if __name__ == "__main__":
    print(f"Python {python_version()}, NumPy {np.__version__}, SciPy {scipy.__version__}")
    print(f"front={FRONT:.11f} packet end={PACKET:.11f}, QNM window=[0,{QNM_END}]")
    print("N_u   ||q||2 on QNM cut   integral q   packet sup   round-trip defect   cap-to-news difference")
    for n in (256, 512, 1024):
        check(n)
