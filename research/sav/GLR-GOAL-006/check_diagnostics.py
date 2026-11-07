"""Non-production checks of the GOAL-006 ideal constraint end and ray estimates.

Run from research/sav with python3 -B GLR-GOAL-006/check_diagnostics.py.
The ODE starts with manufactured momentum at an outer leaf. The null model
uses forced flat radial Klein–Gordon, with no black-string evolution.
"""

import math
import platform

import numpy as np
import scipy
import sympy as sp
from scipy.integrate import quad, solve_ivp


def check_constraint_coefficients():
    """Recover the w,Q,J linear coefficients from the exact ideal-end equations."""
    r = sp.symbols("r", positive=True)
    w, W, J = sp.symbols("w W J", real=True)
    U0 = r / (r + 1)
    Q0 = 2 * sp.sqrt(U0)
    d0 = 1 / (2 * (r * (r + 1)) ** sp.Rational(3, 2))
    U = U0 + w / r
    Q = Q0 + W
    Z = -Q**2 / (2 * r**4) + 2 * d0 * Q / r**2 - 2 * J**2 / r**4
    assert sp.simplify(Z.subs({W: 0, J: 0}) + 2 / (r**2 * (r + 1) ** 2)) == 0
    C = sp.diff(r**2 * Z / 2, W).subs(W, 0)
    B = sp.diff(U ** (-sp.Rational(1, 2)), w).subs(w, 0) * (r**2 * d0 - Q0 / 2)
    assert sp.simplify(C + (2 * r + 1) / (2 * (r * (r + 1)) ** sp.Rational(3, 2))) == 0
    assert sp.simplify(B - (2 * r + 1) / (4 * r**2)) == 0
    assert sp.simplify(r / (2 * U0) - (r + 1) / 2) == 0
    assert sp.simplify(B + (2 * r + 1) / (4 * r**2)) != 0  # wrong sign
    primitive = sp.sqrt(r * (r + 1)) + sp.asinh(sp.sqrt(r))
    assert sp.simplify(sp.diff(primitive, r) - sp.sqrt(1 / U0)) == 0
    # The tuned mass derivative is for a variation of the inner lapse a, not U.
    lam = sp.symbols("lambda", real=True)
    ri = sp.Rational(3, 4)
    M_over_M0 = 1 - ri**2 / (ri + 1) * (sp.exp(-2 * lam) - 1)
    assert sp.diff(M_over_M0, lam).subs(lam, 0) == sp.Rational(9, 14)
    print("Exact ideal-end coefficients, Coulomb phase and 9/14 mass slope verified.")


def check_constraint_transfer():
    """Track a reflection-compatible second harmonic from R=8 to r=256."""
    k = 2 * math.pi / 5
    R = 8.0

    def phase_primitive(r):
        return np.sqrt(r * (r + 1)) + np.arcsinh(np.sqrt(r))

    def rhs(r, state):
        w, q_scaled, j = state
        a0 = math.sqrt(1 + 1 / r)
        C = -(2 * r + 1) / (2 * (r * (r + 1)) ** 1.5)
        B = (2 * r + 1) / (4 * r**2)
        freq = k * a0 / math.sqrt(2)
        return [-k**2 * (r + 1) * w / 2 + C * math.sqrt(2) * q_scaled,
                freq * j, -freq * q_scaled - k * B * w]

    radii = np.array([32., 64., 128., 256.])
    solutions = []
    for tolerance in (1e-8, 1e-11):
        result = solve_ivp(rhs, (R, radii[-1]), [0., 1., 0.], t_eval=radii,
                           method="Radau", rtol=tolerance, atol=tolerance / 20)
        assert result.success, result.message
        solutions.append(result.y)
    np.testing.assert_allclose(*solutions, rtol=2e-7, atol=2e-8)
    w, q_scaled, j = solutions[-1]
    phase = k * (phase_primitive(radii) - phase_primitive(R)) / math.sqrt(2)
    c = np.exp(1j * phase) * (q_scaled + 1j * j)
    sigma = 3 * math.sqrt(2) / (4 * k * R**3)
    assert np.max(np.abs(c - 1)) < sigma / (1 - sigma)
    drift = abs(c[-1] - c[-2])
    assert drift < 5e-7
    no_log = np.exp(1j * k * (radii - R) / math.sqrt(2)) * (q_scaled + 1j * j)
    wrong_drift = abs(no_log[-1] - no_log[-2])
    assert wrong_drift > 0.1  # deleting the long-range phase fails
    print(f"Manufactured n=2 transfer at r=256: {c[-1]:.9g}, σ={sigma:.6g}.")
    print(f"Demodulated change [128,256]: {drift:.3g}; no-log mutation: {wrong_drift:.3g}.")
    print(f"Projected lapse diagnostic: max r³|w|={max(radii**3 * abs(w)):.6g}.")


def check_flat_flux():
    """Integrate a manufactured identity over the wedge from t=0 to u=−4, v=12."""
    # ψ = r^-1 cos(p r - ω t) cos(k z), with ψ=r h and <cos²(kz)>=1/2.
    # F=ψ_tt-ψ_rr-ψ_zz, so dispersion cancels the undifferentiated plane wave.
    p, k = 0.7, 2 * math.pi / 5
    omega = math.hypot(p, k)
    u_cut, v_cut = -4., 12.

    def terms(u, v):
        r, t = (v - u) / 2, (v + u) / 2
        cos_phase, sin_phase = math.cos(p * r - omega * t), math.sin(p * r - omega * t)
        value = cos_phase / r
        dt = omega * sin_phase / r
        dr = -cos_phase / r**2 - p * sin_phase / r
        du, dv = (dt - dr) / 2, (dt + dr) / 2
        F = -2 * cos_phase / r**3 - 2 * p * sin_phase / r**2
        mass = k**2 * value**2 / 8
        return dv**2 / 2, du**2 / 2, mass, F * dt / 4

    def integrate(func, lower, upper):
        return quad(func, lower, upper, epsabs=1e-11, epsrel=1e-11)[0]

    work = integrate(lambda v: integrate(lambda u: terms(u, v)[3], -v, u_cut),
                     -u_cut, v_cut)

    def boundary_balance(mass_factor):
        def outgoing(u, v):
            dv_flux, _, mass, _ = terms(u, v)
            return dv_flux + mass_factor * mass

        def incoming(u, v):
            _, du_flux, mass, _ = terms(u, v)
            return du_flux + mass_factor * mass

        return (integrate(lambda v: outgoing(u_cut, v), -u_cut, v_cut)
                + integrate(lambda u: incoming(u, v_cut), -v_cut, u_cut)
                - integrate(lambda v: outgoing(-v, v) + incoming(-v, v), -u_cut, v_cut))

    error = abs(boundary_balance(1) - work)
    bad_error = abs(boundary_balance(0) - work)
    assert error < 1e-10
    assert bad_error > 1e-3  # wrong estimate drops the mass term
    print(f"Manufactured null energy balance error: {error:.3g}; no-mass mutation: {bad_error:.3g}.")

    # r <h_z²> = <ψ_z²>/r is a model quadratic massless source.
    # Its tail is bounded by R^-1 times the unweighted outgoing mass flux.
    R, R_end = 16., 100.
    v_start, v_end = u_cut + 2 * R, u_cut + 2 * R_end
    flux = integrate(lambda v: 4 * terms(u_cut, v)[2], v_start, v_end)
    source_tail = integrate(lambda v: 8 * terms(u_cut, v)[2] / (v - u_cut), v_start, v_end)
    assert 0 < source_tail <= flux / R
    print(f"Model ray source on [16,100]: {source_tail:.6g}; flux/R: {flux/R:.6g}.")


def check_log_clock():
    """Show finite worldtube Taylor control can coexist with zero radius limit."""
    def profile(u):
        return math.exp(-1 / (1 - u**2)) if abs(u) < 1 else 0.

    profile_norm = math.sqrt(quad(lambda u: profile(u)**2, -1., 1., epsabs=1e-12)[0])
    results = []
    for eps in (0.2, 0.1, 0.05):
        fixed_radius_shift = eps**2 * 8
        l2 = math.sqrt(quad(lambda u: (profile(u + fixed_radius_shift) - profile(u))**2,
                            -1., 1., epsabs=1e-12)[0])
        # Use log(R) directly to represent the exponentially distant radius.
        large_radius_shift = eps**2 * (3 / eps**2)
        assert profile(-1 + large_radius_shift) == profile(1 + large_radius_shift) == 0
        results.append((l2 / eps**2, profile_norm / eps**2))
    assert all(fixed < 8 for fixed, _ in results)
    assert results[-1][1] > 15 * results[0][1]
    print("Clock errors divided by ε⁴, at log R=8 and log R=3/ε²:",
          [tuple(round(x, 5) for x in row) for row in results])


if __name__ == "__main__":
    print(f"Python {platform.python_version()}, NumPy {np.__version__}, "
          f"SciPy {scipy.__version__}, SymPy {sp.__version__}.")
    check_constraint_coefficients()
    check_constraint_transfer()
    check_flat_flux()
    check_log_clock()
