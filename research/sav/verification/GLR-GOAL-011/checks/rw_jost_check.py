"""Independent numerical RW normalization and graph-moment check, r0=1.

Solves the second-order amplitude equations directly, with separate endpoint
series and no imports from the audited implementation. Empirical arithmetic.
"""
import cmath

import numpy as np
from scipy.integrate import quad, solve_ivp
from scipy.optimize import brentq, root

S_CENTER = -0.177924631 - 0.747343369j
MATCH = 2.5


def x(r):
    return r + cmath.log(r - 1)


def horizon_coefficients(s, order=16):
    a = [1.0 + 0j]
    for n in range(1, order + 1):
        prev = (2 * (n - 1) * (n - 2) + (1 + 6*s) * (n - 1) - 3) * a[n-1]
        if n >= 2:
            prev += ((n - 2) * (n - 3) + 6*s * (n - 2) - 6) * a[n-2]
        if n >= 3:
            prev += 2*s * (n - 3) * a[n-3]
        a.append(-prev / (n * (n + 2*s)))
    return np.asarray(a)


def outer_coefficients(s, order):
    a = [1.0 + 0j]
    for n in range(1, order + 1):
        prev = (6 - (n - 1) * n) * a[n-1]
        if n >= 2:
            prev += ((n - 2) * n - 3) * a[n-2]
        a.append(prev / (2*s*n))
    return np.asarray(a)


def integrate(s, origin, direction, bounds, data, sign, rtol):
    def ode(q, values):
        r = origin + direction * q
        f = 1 - 1/r
        v, vr = values
        vrr = ((6/r**2 - 3/r**3)*v - (1/r**2 + sign*2*s)*vr) / f
        return direction * np.asarray([vr, vrr])

    sol = solve_ivp(ode, bounds, data, method="DOP853", rtol=rtol,
                    atol=rtol/10, dense_output=True)
    if not sol.success:
        raise RuntimeError(sol.message)
    return sol


def jost(s, far, order, angle, match=MATCH, rtol=1e-12):
    poly = np.polynomial.Polynomial(horizon_coefficients(s))
    start = 0.01
    h_sol = integrate(s, 1, 1, (start, match-1),
                      [poly(start), poly.deriv()(start)], 1, rtol)
    ray = cmath.exp(1j * angle)
    radius = match + ray*far
    poly = np.polynomial.Polynomial(outer_coefficients(s, order))
    g_sol = integrate(s, match, ray, (far, 0),
                      [poly(1/radius), -poly.deriv()(1/radius)/radius**2],
                      -1, rtol)
    v, vr = h_sol.y[:, -1]
    w, wr = g_sol.y[:, -1]
    f = 1 - 1/match
    wronskian = f*(v*wr - vr*w) - 2*s*v*w
    return wronskian, h_sol


def bump(y):
    p = y*(1-y)
    return np.exp(-1/p)


def bump_prime(y):
    p = y*(1-y)
    return bump(y)*(1-2*y)/p**2


def moment(s, h_sol):
    width = 0.12
    R = 13/8
    u_star = R - 2*x(R).real
    norm = quad(lambda y: bump_prime(y)**2, 0, 1, epsabs=1e-14)[0]**-0.5

    def integrand(y):
        u = u_star + width*y
        r = brentq(lambda r: r - 2*x(r).real - u, 1.5, 1.7, xtol=5e-15)
        v, vr = h_sol.sol(r-1)
        f = 1 - 1/r
        return width**1.5 * norm * bump(y) * cmath.exp(-s*u) * (f*vr + 2*s*v)

    return quad(integrand, 0, 1, complex_func=True, epsabs=1e-13)[0]


def main():
    reference = 7.83649093 - 1.70981020j
    candidate = 0.13523378825882212 - 0.3706200776960893j
    for far, order, angle, match in [(48, 8, 0.59, 2.2),
                                      (80, 11, 0.78, 2.5),
                                      (144, 14, 0.92, 3.1)]:
        def evaluate(s):
            return jost(s, far, order, angle, match)

        def real_w(pair):
            w, _ = evaluate(complex(*pair))
            return [w.real, w.imag]

        found = root(real_w, [S_CENTER.real, S_CENTER.imag], tol=1e-10)
        s = complex(*found.x)
        w, h_sol = evaluate(s)
        assert abs(w) < 2e-12 and abs(s-S_CENTER) < 5e-8
        slopes = []
        for step in (1e-4, 3e-5):
            p = evaluate(s+step)[0] - evaluate(s-step)[0]
            q = evaluate(s+2*step)[0] - evaluate(s-2*step)[0]
            slopes.append((8*p-q)/(12*step))
        Wp = slopes[-1]
        assert abs(Wp-reference) < 2e-5 and abs(Wp-slopes[0]) < 2e-6
        J = moment(s, h_sol)
        A0 = -J / Wp
        direction = s*A0
        perpendicular = (candidate*direction.conjugate()).imag / abs(direction)
        # One real clock displacement cancels only the parallel component.
        translated = candidate + 230*direction
        shifted = (translated*direction.conjugate()).imag / abs(direction)
        assert abs(shifted - perpendicular) < 2e-15
        assert -0.44590 < perpendicular < -0.33745
        print(f"far={far} order={order} angle={angle} match={match} "
              f"s={s:.13g} |W(s)|={abs(w):.2g}")
        print(f"  W'={Wp:.11g} J_h={J:.11g} A01={A0:.11g} "
              f"perpendicular={perpendicular:.10g}")


if __name__ == "__main__":
    main()
