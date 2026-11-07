#!/usr/bin/env python3
"""Bounded independent checks of GLR-GOAL-008. No source solver imports."""

import json
import platform

import mpmath as mp
import numpy as np
import scipy
from scipy.integrate import simpson, solve_ivp
from scipy.interpolate import CubicSpline
from scipy.special import roots_legendre, wrightomega

OMEGA = 0.06328118897590773
S = 2 * OMEGA
K2 = (np.pi / 5) ** 2
MASSES = np.array([0.0, 0.02, 0.05, 0.10, 0.20, 0.30])
RADII = (12.0, 24.0, 48.0)
CUTS = np.array([-10.0, -9.0, -8.0])


def riccati_source(omega, end=80):
    """Reconstruct the decaying exterior from the vacuum scalar master."""
    wave_number = np.sqrt(K2 + omega**2)
    beta = (K2 + 2 * omega**2) / (2 * wave_number)

    def ode(r, state):
        p, _ = state
        f = 1 - 1 / r
        d = 1 + K2 * r**3
        potential = f / r**3 * (
            K2 * r**3 + 6 * r - 11 + 18 * (r - 1) / d**2 - 6 * (4 * r - 5) / d
        )
        return ((potential + omega**2 - p**2) / f, p / f)

    solution = solve_ivp(
        ode, (end, 2), [(1 - 1 / end) * (-wave_number - beta / end), 0],
        method="DOP853", dense_output=True, rtol=2e-12, atol=2e-13,
    )
    assert solution.success

    def metric(r):
        p, log_master = solution.sol(r)
        f = 1 - 1 / r
        d = 1 + K2 * r**3
        y = d / r * np.exp(log_master - solution.y[1, -1])
        derivative = p / f + 3 * K2 * r**2 / d - 1 / r
        ang = y * (-1 / (2 * d) - f * derivative / (K2 * d * r))
        tt = y * (
            f * (0.5 - K2 * r**3) / d**2 - omega**2 / (K2 * d)
            + (f / (2 * K2 * r**2 * d) - 3 * (r - 1)**2 / d**2) * derivative
        )
        tr = -omega * y / (K2 * d) * (derivative - 1 / (2 * r * (r - 1)))
        return tt / f, tr, ang

    scale = 2 / metric(2)[2]

    def radial(r):
        tt, tr, ang = metric(np.clip(r, 2, end))
        contraction = scale**2 * (tt**2 - 2 * tr**2 + (tt - 2 * ang)**2 + 2 * ang**2)
        # Smooth buffers outside the causal extraction domain and in the exponentially small tail.
        lo = np.clip((r - 3.2) / 1.3, 0, 1)
        hi = np.clip((end - r) / 5, 0, 1)
        return np.sqrt(3) * K2 * (r - 1) * contraction / 16 * smooth(lo) * smooth(hi)

    return radial


def smooth(weight):
    return weight**3 * (10 - 15 * weight + 6 * weight**2)


def upwind_derivative(values, dx, *, reverse=False):
    if reverse:
        values = values[:, ::-1]
    padded = np.pad(values, ((0, 0), (3, 2)))
    derivative = (-2 * padded[:, :-5] + 15 * padded[:, 1:-4] - 60 * padded[:, 2:-3]
                  + 20 * padded[:, 3:-2] + 30 * padded[:, 4:-1] - 3 * padded[:, 5:]) / (60 * dx)
    return -derivative[:, ::-1] if reverse else derivative


def spacelike_evolution(dx, source, omega=OMEGA, upper=113.0):
    # Zero data on tau=t+x-r=0. The extraction windows end at tau~47.7.
    # Remove forcing strictly beyond the future characteristic of the measured cuts.
    # This avoids numerical leakage from exponentially growing, causally irrelevant data.
    x = np.arange(3.5, upper + dx / 2, dx)
    r = 1 + wrightomega(x - 1)
    inv_r = 1 / r
    f = 1 - inv_r
    potential = f * (inv_r**3 + MASSES[:, None]**2)
    source_on_slice = source(r) * np.exp(2 * omega * (r - x))
    initial_u = r - 2 * x
    windows = np.array([CUTS + rr + 2 * np.log(rr - 1) for rr in RADII])
    extraction = []
    for rr in RADII:
        xx = rr + np.log(rr - 1)
        j = np.searchsorted(x, xx)
        stencil = np.arange(j - 2, j + 2)
        weight = CubicSpline(x[stencil], np.eye(4), axis=0)(xx)
        extraction.append((stencil, weight))

    def rhs(tau, state):
        psi, p, q = state
        cut_weight = smooth(np.clip((-6.0 - tau - initial_u) / 1.5, 0, 1))
        force = (np.exp(2 * omega * tau) * source_on_slice * cut_weight - potential * psi) / 2
        return np.array([
            p + q, (force - upwind_derivative(p, dx)) / (1 + inv_r),
            (force + upwind_derivative(q, dx, reverse=True)) / f,
        ])

    dt = 0.45 * dx
    time = np.arange(0, windows.max() + 2 * dt, dt)
    history = np.empty((len(time), 3, len(MASSES), len(RADII)))
    state = np.zeros((3, len(MASSES), len(x)))
    for i, tau in enumerate(time):
        for j, (stencil, weight) in enumerate(extraction):
            history[i, :, :, j] = state[:, :, stencil] @ weight
        if i + 1 == len(time):
            break
        k1 = rhs(tau, state)
        k2 = rhs(tau + dt / 2, state + dt / 2 * k1)
        k3 = rhs(tau + dt / 2, state + dt / 2 * k2)
        k4 = rhs(tau + dt, state + dt * k3)
        state += dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4)

    energy, derivative, endpoint = [], [], []
    for j, rr in enumerate(RADII):
        curve = CubicSpline(time, history[:, :, :, j])
        tau = np.linspace(windows[j, 0], windows[j, -1], 1001)
        psi, p, q = curve(tau).transpose(1, 0, 2)
        energy.append(2 * simpson((p + q) * (p - q + (1 - 1 / rr) / rr * psi), x=tau, axis=0))
        at_cuts = curve(windows[j])
        derivative.append((at_cuts[:, 1] + at_cuts[:, 2]).tolist())
        endpoint.append(((1 - 1 / rr) / rr * (psi[-1]**2 - psi[0]**2)).tolist())
    return dict(dx=dx, energies=np.asarray(energy).T.tolist(),
                time_derivative_by_radius=derivative, physical_minus_master_by_radius=endpoint)


def flat_fourier(mu, x, order):
    # Reconstruct the retarded outgoing solution in real frequency, including the evanescent part.
    # The source is 1/(2 cosh(S t)) at A=2 with an odd image at -A.
    z, w = roots_legendre(order)
    p = 1.5 * (1 + z)
    freq = np.hypot(mu, p)
    a = np.sin(2 * p) / p * np.pi / (2 * S * np.cosh(np.pi * freq / (2 * S)))
    propagating = 1.5 * w * a * (p / freq) / np.pi * np.exp(1j * p * x)
    theta = np.pi / 4 * (z + 1)
    kappa = mu * np.cos(theta)
    sub_freq = mu * np.sin(theta)
    if mu:
        below = np.sinh(2 * kappa) / kappa * np.pi / (2 * S * np.cosh(np.pi * sub_freq / (2 * S)))
        evanescent = np.pi / 4 * w * below * kappa * np.exp(-kappa * x) / np.pi
    else:
        evanescent = np.zeros(order)
    u = np.arange(-200.0, 1200.125, 0.25)
    j = np.empty_like(u)
    for start in range(0, len(u), 128):
        ts = slice(start, start + 128)
        t = u[ts, None] + x
        phases = np.exp(-1j * t * freq)
        below_phase = np.exp(-1j * t * sub_freq)
        dt = np.real(phases @ (-1j * freq * propagating) + below_phase @ (-1j * sub_freq * evanescent))
        dx = np.real(phases @ (1j * p * propagating) + below_phase @ (-kappa * evanescent))
        j[ts] = -dt * dx
    all_time = 1.5 / np.pi * np.dot(w, p**2 * a**2)
    return dict(mu=mu, x=x, quadrature_order=order, all_time=float(all_time),
                cuts=[float(simpson(j[u <= end], x=u[u <= end])) for end in (0, 50, 150, 300, 600, 1200)])


def casimir():
    # Independent polylog summation and differentiation. Solve stress cancellation directly.
    mp.mp.dps = 48

    def w_loop(length, mass):
        x = mass * length
        z = mp.exp(-x)
        fermion = mp.polylog(5, z) + x * mp.polylog(4, z) + x**2 / 3 * mp.polylog(3, z)
        return 3 / (4 * mp.pi**2 * length**4) * (-5 * mp.zeta(5) + 8 * fermion)

    def pressure_condition(mass):
        return mp.diff(lambda length: w_loop(length, mass), 1) - w_loop(1, mass)

    root = mp.findroot(pressure_condition, (2, 2.5))
    rho = -w_loop(1, root)
    w2 = mp.diff(lambda length: w_loop(length, root), 1, 2)
    coefficient = 16 * mp.pi / 3 * w2
    return dict(ML=float(root), rho5_L5=float(rho), Wpp_L6=float(w2),
                mass_squared_L2_per_G5_over_L3=float(coefficient),
                G5_over_r0_cubed_needed_at_L10=[float(m**2 * 10**5 / coefficient) for m in MASSES[1:]])


def main():
    radial = riccati_source(OMEGA)
    curves = [spacelike_evolution(dx, radial) for dx in (0.1, 0.05, 0.025)]
    base = np.array(curves[-1]["energies"])
    ext = spacelike_evolution(0.05, radial, upper=125.0)
    assert np.max(np.abs(np.asarray(ext["energies"]) - np.asarray(curves[1]["energies"]))) < 2e-15
    assert np.max(np.abs(base - np.asarray(curves[1]["energies"]))) < 4e-10
    rcheck = np.linspace(5.15, 55, 201)
    right_ratio = riccati_source(OMEGA, 95)(rcheck) / radial(rcheck)
    mode_endpoints = [
        spacelike_evolution(0.05, riccati_source(omega), omega)
        for omega in (0.06328117, 0.06328121)
    ]
    source_spectrum = {}
    for ratio in (0.5, 1.0, 2.0, 4.0):
        angle = mp.pi / (1 + ratio)
        scale = mp.pi / (S * (1 + ratio))
        if ratio == 1.0:
            integral = lambda z: z / (1 + z)
        else:
            cosine, sine = mp.cos(2 * angle), abs(mp.sin(2 * angle))
            integral = lambda z: (mp.atan((z - cosine) / sine) + mp.atan(cosine / sine)) / sine
        source_spectrum[str(ratio)] = [float(integral(mp.exp(-2 * scale * m)) / integral(1)) for m in MASSES]
    flat = [flat_fourier(mu, 48, n) for mu in (0, .1, .3) for n in (1152, 1536)]
    assert max(abs(np.asarray(a["cuts"]) - np.asarray(b["cuts"])).max()
               for a, b in zip(flat[::2], flat[1::2])) < 3e-10
    print(json.dumps(dict(versions=dict(python=platform.python_version(),
                                        numpy=np.__version__, scipy=scipy.__version__, mpmath=mp.__version__),
                          masses=MASSES.tolist(), radii=RADII, curves=curves,
                          far_boundary_repeat=ext["energies"],
                          right_profile_relative_change=float(np.max(np.abs(right_ratio - 1))),
                          mode_endpoints=[row["energies"] for row in mode_endpoints],
                          source_spectrum=source_spectrum, flat=flat, casimir=casimir()), indent=2))


if __name__ == "__main__":
    main()
