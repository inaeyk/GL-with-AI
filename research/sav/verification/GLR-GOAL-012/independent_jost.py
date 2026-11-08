"""Independent floating-point Jost matching and graph moment, r0=1.

Uses full factored columns, without importing any source modules or Riccati
solver. The massive outgoing finite-cut seed is only used for the subspace
logarithmic derivative. Values are convergence evidence, not enclosures.
"""
import cmath
from math import exp, log, pi, sqrt

import numpy as np
from scipy.integrate import quad, solve_ivp
from scipy.optimize import root
from scipy.special import wrightomega

K = pi / 5
OMEGA = 0.06328119
R_MATCH = 2.5
S_CENTER = -0.177924631 - 0.747343369j


def tortoise(r):
    return r + cmath.log(r - 1)


def potential(r, frequency, massive, horizon_offset=None):
    dr = r - 1 if horizon_offset is None else horizon_offset
    f = dr / r
    if not massive:
        return np.array([[f * (6 / r**2 - 3 / r**3)]], complex)
    d = 4 + K**2 * r**2
    c = K**2 * r**2 / d
    c_r = 8 * K**2 * r / d**2
    j = f / r
    j_r = (2 - r) / r**3
    a = j * (1 + c)
    a_x = f * (j_r * (1 + c) + j * c_r)
    b = j * (2 - c)
    b_x = f * (j_r * (2 - c) - j * c_r)
    common = f * (K**2 + 4 / r**2)
    off = -4j * frequency * K * f / d
    return np.array([[common + a*a - a_x, off],
                     [off, common + b*b + b_x]], complex)


def integrate(rhs, lower, upper, m, p, tol):
    n = m.shape[0]
    sol = solve_ivp(rhs, (lower, upper), np.r_[m.ravel(), p.ravel()],
                    method="DOP853", atol=tol * .02, rtol=tol,
                    dense_output=True)
    assert sol.success, sol.message

    def result(t):
        return sol.sol(t).reshape(2, n, n)

    return result


def horizon(frequency, massive, cutoff, tol):
    n = 2 if massive else 1
    eye = np.eye(n, dtype=complex)
    d = 4 + K**2
    c = K**2 / d
    if massive:
        off = -4j * frequency * K / d
        leading = np.array([[K**2 + 3 - c, off],
                            [off, K**2 + 6 - c]], complex)
    else:
        leading = np.array([[3]], complex)
    a = leading / (1 + 2 * frequency)

    def rhs(y, state):
        delta = exp(y)
        r = 1 + delta
        m, p = state.reshape(2, n, n)
        q = potential(r, frequency, massive, delta)
        return (r * np.array([p, q @ m - 2 * frequency * p])).ravel()

    return integrate(rhs, log(cutoff), log(R_MATCH-1), eye + a*cutoff,
                     a*cutoff/(1+cutoff), tol)


def outgoing(frequency, massive, length, theta, tol):
    ray = cmath.exp(1j * theta)
    n = 2 if massive else 1
    if massive:
        kappa = -cmath.sqrt(frequency**2 + K**2)
        alpha = K**2 / (2*kappa)
        m = np.eye(n, dtype=complex)
        p = np.zeros_like(m)
    else:
        kappa, alpha = frequency, 0
        endpoint = R_MATCH + length * ray
        # Laurent polynomial for f m_rr + (f_r-2s)m_r -(6/r^2-3/r^3)m.
        coeff = [1+0j]
        for index in range(1, 13):
            prior = coeff[index - 1] * ((index-1)*index - 6)
            if index >= 2:
                prior += coeff[index-2] * (3 - (index-2)*index)
            coeff.append(-prior/(2*frequency*index))
        m = np.array([[sum(c/endpoint**i for i,c in enumerate(coeff))]])
        p = np.array([[-(1-1/endpoint)*sum(i*c/endpoint**(i+1)
                                         for i,c in enumerate(coeff))]])

    def rhs(rho, state):
        r = R_MATCH + ray*rho
        f = 1 - 1/r
        ell = -kappa + alpha*f/r
        ell_x = alpha*f*(2-r)/r**3
        m, p = state.reshape(2, n, n)
        q = potential(r, frequency, massive)
        q += (frequency**2 - ell**2 - ell_x) * np.eye(n)
        return (ray/f * np.array([p, q @ m - 2*ell*p])).ravel()

    evaluate = integrate(rhs, length, 0, m, p, tol)
    m, p = evaluate(0)
    factor = cmath.exp(-kappa*tortoise(R_MATCH))*R_MATCH**alpha
    slope = -kappa + alpha*(1-1/R_MATCH)/R_MATCH
    return factor*m, slope*np.eye(n) + p @ np.linalg.inv(m)


def match(frequency, massive, length, theta, cutoff, tol):
    h = horizon(frequency, massive, cutoff, tol)
    m, p = h(log(R_MATCH-1))
    left = frequency*np.eye(m.shape[0]) + p @ np.linalg.inv(m)
    hl = cmath.exp(frequency*tortoise(R_MATCH))*m
    gr, right = outgoing(frequency, massive, length, theta, tol)
    return left-right, hl, gr, h


def graph_moment(frequency, h):
    radius, width = 13/8, 3/25
    earliest = (radius - 2*tortoise(radius)).real

    def bump(y):
        return exp(4 - 1/(y*(1-y)))

    def derivative_sq(y):
        return (bump(y)*(1-2*y)/(y*(1-y))**2)**2

    norm = quad(derivative_sq, 0, 1, epsabs=2e-13)[0]**(-.5)

    def integrand(y):
        u = earliest + width*y
        radius = 1 + 2*wrightomega(-log(2) - (1+u)/2)
        m, p = h(log(radius-1))
        return (cmath.exp(-frequency*u) * width**1.5 * norm * bump(y)
                * (2*frequency*m[0, 0] + p[0, 0]))

    return quad(integrand, 0, 1, complex_func=True, epsabs=2e-13)[0]


def run(length, theta, cutoff, tol):
    def scalar(s):
        d, h, g, solver = match(s, False, length, theta, cutoff, tol)
        return d[0, 0], h[0, 0], g[0, 0], solver

    def objective(x):
        d = scalar(complex(*x))[0]
        return [d.real, d.imag]

    solution = root(objective, [S_CENTER.real, S_CENTER.imag], tol=1e-10)
    s = complex(*solution.x)
    d, h, g, solver = scalar(s)
    assert abs(d) < 3e-10
    step = 4e-5
    d_prime = (-scalar(s+2*step)[0] + 8*scalar(s+step)[0]
               - 8*scalar(s-step)[0] + scalar(s-2*step)[0])/(12*step)
    w_prime = -h*g*d_prime
    moment = graph_moment(s, solver)
    launch = -moment*g/(w_prime*h)
    shifted_d, shifted_h, shifted_g, _ = scalar(s+2*OMEGA)
    shifted_w = -shifted_d*shifted_h*shifted_g
    massive_d, *_ = match(s+OMEGA, True, length, theta, cutoff, tol)
    threshold_d, *_ = scalar(-OMEGA-1j*K)
    print(f"length={length} angle={theta*180/pi:.0f} cutoff={cutoff} tolerance={tol}")
    for name, value in (("s_q", s), ("s_star", s+2*OMEGA),
                        ("W_prime", w_prime), ("J_h", moment),
                        ("unit-horizon launch", launch),
                        ("shifted returning D", shifted_d),
                        ("shifted returning W", shifted_w),
                        ("shifted massive det", np.linalg.det(massive_d)),
                        ("baseline D at lower threshold", threshold_d)):
        print(f"{name}: {value.real:+.12f}{value.imag:+.12f}j")


if __name__ == "__main__":
    run(60, pi/4, 1e-7, 2e-11)
    run(110, 40*pi/180, 1e-8, 4e-13)
