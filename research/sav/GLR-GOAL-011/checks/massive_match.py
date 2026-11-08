"""Empirical, non-production massive axial matching at two spectral points.

Uses r0=1, L/r0=10 and l=2. Run as python3 GLR-GOAL-011/checks/massive_match.py.
No full source, exact spectral location, or determinant enclosure is tested.
"""
import math
import sys

import numpy as np
import scipy
from scipy.integrate import solve_ivp


K = 2 * math.pi / 10
LAMBDA = 4.0
OMEGA = 0.06328119
S_Q_APPROX = -0.177925 - 0.747343j
R_MATCH = 2.5
ANGLE = math.pi / 4
IDENTITY = np.eye(2, dtype=complex)
EXCHANGE = np.array([[0, 1], [1, 0]], dtype=complex)


def background(r, s):
    f = 1 - 1 / r
    f_r = 1 / r**2
    d = LAMBDA + K**2 * r**2
    rational = K**2 * r / d
    rational_r = K**2 / d - 2 * K**4 * r**2 / d**2
    a_rlog = 1 / r + rational
    b_rlog = 2 / r - rational
    a = f * a_rlog
    b = f * b_rlog
    a_x = f * (f_r * a_rlog + f * (-1 / r**2 + rational_r))
    b_x = f * (f_r * b_rlog + f * (-2 / r**2 - rational_r))
    base = f * (K**2 + LAMBDA / r**2)
    mixing = 2j * K * math.sqrt(LAMBDA) * f / d
    potential = np.diag([base + a**2 - a_x, base + b**2 + b_x])
    return potential - s * mixing * EXCHANGE, f, f_r


def pack(value, slope):
    return np.stack([value, slope]).reshape(-1)


def unpack(vector):
    return vector.reshape(2, 2, 2)


def integrate(rhs, interval, value, slope, rtol):
    sol = solve_ivp(
        rhs, interval, pack(value, slope), method="DOP853",
        rtol=rtol, atol=rtol * 0.01,
    )
    if not sol.success:
        raise RuntimeError(sol.message)
    return unpack(sol.y[:, -1]), sol.nfev


def left_basis(s, delta, rtol):
    d_h = LAMBDA + K**2
    a_h = 1 + K**2 / d_h
    b_h = 2 - K**2 / d_h
    horizon_potential = np.diag([d_h - a_h, d_h + b_h])
    horizon_potential = horizon_potential - (
        s * 2j * K * math.sqrt(LAMBDA) / d_h * EXCHANGE
    )
    first = horizon_potential / (1 + 2 * s)

    # y=log(r-1), dx/dy=r, and the slope is d/dx.
    def rhs(y, vector):
        r = 1 + math.exp(y)
        value, slope = unpack(vector)
        potential, _, _ = background(r, s)
        return r * pack(slope, potential @ value - 2 * s * slope)

    (value, slope), nfev = integrate(
        rhs, (math.log(delta), math.log(R_MATCH - 1)),
        IDENTITY + delta * first, delta / (1 + delta) * first, rtol,
    )
    return np.vstack([value, s * value + slope]), nfev


def right_basis(s, length, rtol):
    # Principal sqrt flips at the continued target Im s < -k, Re s < 0.
    kappa = np.sqrt(s**2 + K**2)
    if kappa.imag > 0:
        kappa = -kappa
    ray = np.exp(1j * ANGLE)
    if np.real(kappa * ray) <= 0:
        raise ValueError("The declared ray does not decay at this spectral point")
    exponent = K**2 / (2 * kappa)

    def rhs(distance, vector):
        r = R_MATCH + ray * distance
        value, slope = unpack(vector)
        potential, f, f_r = background(r, s)
        logarithmic_slope = -kappa + exponent * f / r
        logarithmic_slope_x = exponent * f * (f_r / r - f / r**2)
        remainder = potential + (
            s**2 - logarithmic_slope**2 - logarithmic_slope_x
        ) * IDENTITY
        return ray / f * pack(
            slope, remainder @ value - 2 * logarithmic_slope * slope,
        )

    (value, slope), nfev = integrate(
        rhs, (length, 0.0), IDENTITY, np.zeros_like(IDENTITY), rtol,
    )
    f_match = 1 - 1 / R_MATCH
    logarithmic_slope = -kappa + exponent * f_match / R_MATCH
    return np.vstack([value, logarithmic_slope * value + slope]), nfev


def diagnostics(s, delta, length, rtol):
    left, n_left = left_basis(s, delta, rtol)
    right, n_right = right_basis(s, length, rtol)
    q_left, _ = np.linalg.qr(left)
    q_right, _ = np.linalg.qr(right)
    matched = np.hstack([q_left, q_right])
    det = abs(np.linalg.det(matched))
    sigma_min = np.linalg.svd(matched, compute_uv=False)[-1]
    print(
        f"  delta={delta:.0e} length={length:6.1f} rtol={rtol:.0e} "
        f"|det_Q|={det:.10f} sigma_min={sigma_min:.10f} "
        f"nfev=({n_left},{n_right})"
    )


def main():
    print(f"Python {sys.version.split()[0]}, NumPy {np.__version__}, SciPy {scipy.__version__}")
    print(f"r0=1 k={K:.16g} lambda={LAMBDA:g} r_match={R_MATCH:g} ray angle=pi/4")
    for variation in (0.0, 0.003):
        s = S_Q_APPROX - OMEGA + variation
        print(f"s1={s.real:+.8f}{s.imag:+.8f}i")
        for delta, length in ((1e-3, 24), (1e-4, 48), (1e-5, 96), (1e-6, 144)):
            diagnostics(s, delta, length, 2e-10)
        diagnostics(s, 1e-6, 144, 2e-12)


if __name__ == "__main__":
    main()
