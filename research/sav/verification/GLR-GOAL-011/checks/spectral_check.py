"""Non-production RW check using a continued fraction and direct Jost ODEs.

Run with Python 3, mpmath, NumPy, and SciPy. The finite continued fraction,
asymptotic endpoint truncations and DOP853 integrations are empirical.
No original GLR-GOAL-011 routines or outputs are read by the calculation.
"""
import cmath
import math

import mpmath as mp
import numpy as np
from scipy.integrate import solve_ivp


def continued_fraction(s, nmax, angular=6):
    tail = mp.mpc(0)
    for n in range(nmax, -1, -1):
        beta = -2 * n**2 - (8 * s + 2) * n - angular - 8 * s**2 - 4 * s + 3
        denominator = beta - tail
        if n == 0:
            return denominator
        tail = (n * (n + 2 * s)) * ((n + 2 * s)**2 - 4) / denominator


def jost_data(s, right, cap, terms, tolerance):
    angular = 6
    match = 2.5
    if right:
        direction = cmath.exp(1j * math.pi / 4)
        radius = match + direction * cap
        c_previous, c = 0j, 1 + 0j
        d_previous, d = 0j, 0j
        value, slope, value_s, slope_s = c, 0j, d, 0j
        for n in range(terms):
            a = angular - n * (n + 1)
            b = n**2 - 4
            c_next = (a * c + b * c_previous) / (2 * s * (n + 1))
            d_next = (a * d + b * d_previous) / (2 * s * (n + 1)) - c_next / s
            c_previous, c = c, c_next
            d_previous, d = d, d_next
            power = radius**(-(n + 1))
            dx_power = -(n + 1) * (1 - 1 / radius) * power / radius
            value += c * power
            slope += c * dx_power
            value_s += d * power
            slope_s += d * dx_power

        def chart(parameter):
            r = match + direction * parameter
            f = 1 - 1 / r
            return r, f, direction / f

        sign = -1
        stop = 0
    else:
        value, slope, value_s, slope_s = 1 + 0j, 0j, 0j, 0j

        def chart(parameter):
            t = math.exp(parameter)
            r = 1 + t
            return r, t / r, r

        sign = 1
        stop = math.log(match - 1)

    def rhs(parameter, state):
        r, f, dx = chart(parameter)
        pot = f * (angular / r**2 - 3 / r**3)
        v, vx, vs, vxs = state
        return dx * np.array([
            vx,
            pot * v - 2 * sign * s * vx,
            vxs,
            pot * vs - 2 * sign * (s * vxs + vx),
        ])

    sol = solve_ivp(
        rhs, (cap, stop), (value, slope, value_s, slope_s), method="DOP853",
        rtol=tolerance, atol=tolerance * 0.02,
    )
    if not sol.success:
        raise RuntimeError(sol.message)
    return sol.y[:, -1]


def main():
    mp.mp.dps = 40
    seed = mp.mpc("-0.177924631", "-0.747343369")
    for depth in (200, 600, 1600):
        s = mp.findroot(
            lambda z: continued_fraction(z, depth),
            (seed, seed + mp.mpf("0.0001")),
        )
        sigma = s - 2 * mp.mpf("0.06328119")
        print(f"depth {depth:4d}: s = {mp.nstr(s, 28)}")
        print(f"            |s - center| = {mp.nstr(abs(s-seed), 8)}")
        print(f"            shifted continued fraction = {mp.nstr(continued_fraction(sigma, depth), 16)}")
    z = complex(s)
    for left_cap, right_cap, terms, tol in (
        (-30, 64, 8, 2e-11),
        (-40, 96, 12, 2e-12),
        (-50, 144, 16, 2e-13),
    ):
        h, hx, hs, hxs = jost_data(z, False, left_cap, terms, tol)
        g, gx, gs, gxs = jost_data(z, True, right_cap, terms, tol)
        # h and g here are the factored solutions. Their exponential
        # factors cancel from W(s), including after differentiation.
        w = h * gx - hx * g - 2 * z * h * g
        dw = hs * gx + h * gxs - hxs * g - hx * gs
        dw -= 2 * h * g + 2 * z * (hs * g + h * gs)
        print(f"caps {left_cap:3d}, {right_cap:3d}  |W|={abs(w):.3e}  W'={dw:.12g}")


if __name__ == "__main__":
    main()
