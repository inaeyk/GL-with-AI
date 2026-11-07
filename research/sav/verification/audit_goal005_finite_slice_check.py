"""Exterior Z-waveform diagnostic using the audited master and tensor ratios.

Integrate the right branch and evaluate free news plus one exact-kernel
potential insertion by direct Gauss-Legendre quadrature. Floating inputs,
finite cutoff, and omitted insertions limit this to a diagnostic.
"""
import numpy as np
from numpy.polynomial.legendre import leggauss
from scipy.integrate import solve_ivp
from scipy.optimize import brentq
from scipy.special import wrightomega

a = (np.pi / 5)**2
omega = 0.06328119
s = 2 * omega
q = np.sqrt(a + omega**2)
beta = (a + 2 * omega**2) / (2 * q)


def x(r):
    return r + np.log(r - 1)


def potential(r):
    d = 1 + a * r**3
    return (1 - 1/r) * (a + 6/r**2 - 11/r**3
                        + 18*(r-1)/(r**3*d**2) - 6*(4*r-5)/(r**3*d))


def calculate(rmax, n):
    def rhs(r, state):
        p, _ = state
        f = 1 - 1/r
        return [(potential(r) + omega**2 - p*p)/f, p/f]

    sol = solve_ivp(rhs, (rmax, 2), (-q+(q-beta)/rmax, 0.),
                    method="DOP853", atol=2e-12, rtol=2e-12, dense_output=True)
    assert sol.success

    def tensor_ratios(r, p):
        f = 1 - 1/r
        d = 1 + a*r**3
        ell = 3*a*r**2/d - 1/r + p/f
        tt = ((0.5-a*r**3)/d - omega**2/(a*f)
              + (1/(2*a*r**2)-3*(r-1)*r/d)*ell)
        tr = -omega/a * (ell - 1/(2*r*(r-1)))
        sphere = -0.5 - f*ell/(a*r)
        return tt, tr, sphere

    p2, log2 = sol.sol(2)
    _, _, sphere_at_two = tensor_ratios(2, p2)
    master_at_two = 4 / sphere_at_two
    r_node, r_w = leggauss(n)
    t_node, t_w = leggauss(n//2)

    def news(u):
        b = brentq(lambda r: -r-2*np.log(r-1)-u, 4, 8)
        r = b + (rmax-b)*(r_node+1)/2
        p, logm = sol.sol(r)
        tt, tr, sphere = tensor_ratios(r, p)
        rr = tt-2*sphere
        contraction = (tt**2+rr**2+2*sphere**2-2*tr**2)
        source = np.sqrt(3)*a/(16*r) * contraction * (master_at_two * np.exp(logm - log2))**2
        high, low = u+x(r), -np.log(r-1)
        t = low[:, None] + (high-low)[:, None]*(t_node+1)/2
        scatter_r = 1+wrightomega((t+x(r)[:, None]-u)/2-1)
        ti = (high-low)/2 * ((np.exp(s*t)/scatter_r**2) @ t_w)
        rw = (rmax-b)/2 * r_w
        n0 = np.dot(rw, np.exp(s*high)*source)/2
        n1 = n0-np.dot(rw, source*ti)/8
        return n0, n1

    u_node, u_w = leggauss(32)
    nn = np.array([news(float(u)) for u in -9+u_node])
    free_energy, first_energy = 2 * (u_w @ (nn * nn))
    print(f"rmax={rmax}, radial nodes={n}, |U(2)|={abs(master_at_two):.11f}")
    print(f"K_free={free_energy:.8e}, K_first={first_energy:.8e}")
    for cut in (-10, -9, -8):
        free, first = news(cut)
        print(f"u={cut}, n_free={free:.8e}, n_first={first:.8e}")


if __name__ == "__main__":
    calculate(60, 64)
    calculate(80, 128)
