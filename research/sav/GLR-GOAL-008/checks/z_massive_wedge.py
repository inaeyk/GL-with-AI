#!/usr/bin/env python3
"""Non-production, finite causal wedge for the pinned L10 Z scalar source."""

import hashlib
import json
from pathlib import Path
import platform
import sys

import numpy as np
import scipy
from scipy.integrate import cumulative_trapezoid, quad, simpson, trapezoid
from scipy.special import wrightomega

HERE = Path(__file__).resolve().parent
PINNED = HERE.parents[1] / "GLR-GOAL-005" / "compute"
sys.path.insert(0, str(PINNED))
from early_null import B_MIN, B_START, SavedSource, slice_u, tortoise  # noqa: E402

RADII = (12.0, 24.0, 48.0)
MASSES = (0.0, 0.02, 0.05, 0.10, 0.20, 0.30)
CUTS = (-10.0, -9.0, -8.0)
V_END = CUTS[-1] + 2 * tortoise(max(RADII))


def manufactured_row(b, v, g, potential, mass_sign):
    """Smooth exact Cauchy jet, independent of the saved GL interpolant."""
    h = np.exp(-(b - B_MIN))
    hp = -h
    d = 1 - b / v
    exact = h * d**2
    exact_u = -(hp * d**2 - 2 * h * d / v) / g
    exact_v = 2 * h * d * b / v**2
    mixed_four = -8 * ((b * hp + h) * d - h * b / v) / (g * v**2)
    return mixed_four + mass_sign * potential * exact, (exact, exact_u, exact_v)


def characteristic(source, mu, resolution, *, manufactured=False, wrong=False):
    # b labels the outgoing rays by their intersection v=b with the Z slice.
    spans = ((B_MIN, B_START, 1), (B_START, 18, 3), (18, 40, 3), (40, V_END, 3))
    b = np.concatenate(
        [np.linspace(a, z, count * resolution, endpoint=False) for a, z, count in spans]
        + [np.asarray([V_END])]
    )
    u = slice_u(b)
    g = (b + 1) / (b - 1)
    work = np.zeros(len(b))
    v_surface = np.zeros(len(b))
    u_surface = 0.0
    wall = np.empty((resolution + 1, len(RADII), 4))
    max_manufactured_error = np.zeros(3)
    last_psi_v = np.zeros(1)
    last_weighted_effective = np.zeros(1)
    # In increasing u, the past row is i+1; j=i is on the initial slice.
    for i in range(len(b) - 2, -1, -1):
        v = b[i:]
        x = (v - u[i]) / 2
        radius = 1 + wrightomega(x - 1)
        f = 1 - 1 / radius
        a = f / radius
        v0 = f / radius**3
        massive = f * mu**2
        potential = v0 + massive
        if manufactured:
            force, exact = manufactured_row(b[i], v, g[i], potential, 1)
        else:
            force = np.exp(source.s * (v + u[i]) / 2) * source(radius)

        # Integrate 4 psi_uv=Q twice by aligned trapezoids.
        # psi_v = C - D psi uses the previous ray's psi_v and Q.
        # At v=b_i the value and both derivatives vanish.
        eff_potential = v0 - massive if wrong else potential
        a_b = (b[i + 1] - b[i]) / 8
        c = np.r_[0, last_psi_v + a_b * (g[i] * force[1:] + last_weighted_effective)]
        d = a_b * g[i] * eff_potential
        d[0] = 0
        a_v = np.diff(v) / 2
        denominator = 1 + a_v * d[1:]
        recurrence = (1 - a_v * d[:-1]) / denominator
        product = np.cumprod(recurrence)
        assert product[-1] > 1e-6
        inhomogeneous = a_v * (c[1:] + c[:-1]) / denominator
        psi = np.r_[0, product * np.cumsum(inhomogeneous / product)]
        psi_v = c - d * psi
        effective = force - eff_potential * psi
        psi_u = cumulative_trapezoid(effective / 4, v, initial=0)
        last_psi_v = psi_v
        last_weighted_effective = g[i] * effective

        if manufactured:
            error = np.max(np.abs(np.asarray((psi, psi_u, psi_v)) - exact), axis=1)
            max_manufactured_error = np.maximum(max_manufactured_error, error)

        # Spherical Klein-Gordon Killing energy, stripped of 1/G4.
        # j = -psi_t(psi_x-a psi), e = (psi_t^2+(psi_x-a psi)^2+f mu^2 psi^2)/2.
        psi_t = psi_u + psi_v
        radial_derivative = psi_v - psi_u - a * psi
        j = -psi_t * radial_derivative
        work[i] = trapezoid(force * psi_t, v) / 2
        v_surface[i] = ((psi_t[-1] - radial_derivative[-1]) ** 2 + massive[-1] * psi[-1] ** 2) / 4
        if i == 0:
            u_surface = trapezoid(((psi_t + radial_derivative) ** 2 + massive * psi**2) / 4, v)
        if i <= resolution:
            for ir, r_obs in enumerate(RADII):
                v_obs = u[i] + 2 * tortoise(r_obs)
                assert v[0] <= v_obs <= v[-1]
                wall[i, ir] = [np.interp(v_obs, v, item) for item in (psi, psi_u, psi_v, j)]

    work_total = trapezoid(g * work, b)
    v_surface_total = trapezoid(g * v_surface, b)
    res = (u_surface + v_surface_total - work_total) / work_total
    early_u = u[: resolution + 1][::-1]
    wall = wall[::-1]
    record = {}
    for ir, r_obs in enumerate(RADII):
        samples = [np.interp(CUTS, early_u, wall[:, ir, col]).tolist() for col in range(4)]
        record[str(r_obs)] = {
            "psi": samples[0],
            "psi_u_at_fixed_v": samples[1],
            "psi_v_at_fixed_u": samples[2],
            "outward_power_over_M0_eps4": (2 * np.asarray(samples[3])).tolist(),
            "signed_wall_energy_over_M0_eps4": float(2 * simpson(wall[:, ir, 3], x=early_u)),
        }
    result = {
        "resolution": resolution, "aligned_rows": len(b), "mu": mu, "walls": record,
        "normalized_domain_balance": {
            "work": float(work_total), "future_u_surface": float(u_surface),
            "future_v_surface": float(v_surface_total), "signed_relative_error": float(res),
        },
    }
    if manufactured:
        result["manufactured_max_psi_psi_u_psi_v_error"] = max_manufactured_error.tolist()
    return result


def spectrum(s):
    # h=e^(s t)/(1+e^((s+beta)t)): source-only smooth history, all t.
    # Early e^(s t) is recovered asymptotically, with no putative vacuum continuation.
    out = {}
    for beta_over_s in (0.5, 1, 2, 4):
        beta = beta_over_s * s
        angle = np.pi * s / (s + beta)
        c = np.pi / (s + beta)
        # Stable transform square for all positive omega.
        def abs_h_sq(omega):
            z = np.exp(-2 * c * omega)
            return 4 * c**2 * z / ((1 - z) ** 2 + 4 * z * np.sin(angle) ** 2)

        positive = quad(abs_h_sq, 0, np.inf, epsabs=1e-10)[0]
        time_l2 = quad(
            lambda t: np.exp(2 * s * t - 2 * np.logaddexp(0, (s + beta) * t)),
            -np.inf, np.inf, epsabs=1e-10,
        )[0]
        assert abs(positive / np.pi / time_l2 - 1) < 1e-11
        out[str(beta_over_s)] = {
            "source_L2_parseval_relative_error": abs(positive / np.pi / time_l2 - 1),
            "above_mu_source_power_fraction": [
                quad(abs_h_sq, mu, np.inf, epsabs=1e-10)[0] / positive for mu in MASSES
            ],
        }
    return {
        "smooth_beta_over_s": out,
        "sharp_early_halfline_above_mu_source_power_fraction": (
            1 - 2 / np.pi * np.arctan(np.asarray(MASSES) / s)
        ).tolist(),
    }


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    source = SavedSource()
    resolutions = (48, 96, 192)
    runs = [characteristic(source, m, n) for n in resolutions for m in MASSES]
    refined = runs[-len(MASSES):]
    previous = runs[-2 * len(MASSES):-len(MASSES)]
    for new, old in zip(refined, previous):
        assert abs(new["normalized_domain_balance"]["signed_relative_error"]) < 3e-4
        for r in map(str, RADII):
            n = new["walls"][r]["signed_wall_energy_over_M0_eps4"]
            o = old["walls"][r]["signed_wall_energy_over_M0_eps4"]
            assert abs(n - o) < 2e-10
    tests = [characteristic(source, .3, n, manufactured=True) for n in resolutions[:2]]
    wrong = characteristic(source, .3, resolutions[1], manufactured=True, wrong=True)
    f_err = np.asarray(tests[-1]["manufactured_max_psi_psi_u_psi_v_error"])
    c_err = np.asarray(tests[0]["manufactured_max_psi_psi_u_psi_v_error"])
    w_err = np.asarray(wrong["manufactured_max_psi_psi_u_psi_v_error"])
    assert np.all(f_err < c_err / 3)
    assert np.all(f_err < w_err / 100)

    baseline_path = PINNED / "early_null_waveform.csv"
    baseline = np.genfromtxt(baseline_path, delimiter=",", names=True)
    null_news = np.interp(CUTS, baseline["u_over_r0"], baseline["N2"])
    finite_u_derivative = sum(
        np.asarray(refined[0]["walls"][str(RADII[-1])][key])
        for key in ("psi_u_at_fixed_v", "psi_v_at_fixed_u")
    )
    assert np.max(np.abs(finite_u_derivative / null_news - 1)) < 1e-4
    out = {
        "environment": dict(python=platform.python_version(), numpy=np.__version__, scipy=scipy.__version__),
        "inputs_sha256": {
            str(path.relative_to(HERE.parents[1])): sha(path) for path in (
                PINNED / "early_null.py", baseline_path,
                HERE.parents[1] / "verification" / "gl_linear_profile_L10.csv",
                HERE.parents[1] / "verification" / "gl_linear_results.json",
            )
        },
        "scales": dict(mu=MASSES, r=RADII, u=CUTS, r0=1, omega=source.omega, s=source.s, v_end=V_END),
        "massless_benchmark": {
            "pinned_null_news": null_news.tolist(),
            "largest_radius_finite_time_derivative": finite_u_derivative.tolist(),
            "relative_difference_vs_pinned": (finite_u_derivative / null_news - 1).tolist(),
            "pinned_empirical_null_energy": float(2 * simpson(baseline["N2"] ** 2, x=baseline["u_over_r0"])),
        },
        "runs": runs,
        "manufactured_tests": tests,
        "wrong_mass_sign_control": wrong,
        "temporal_spectrum": spectrum(source.s),
    }
    print(json.dumps(out, indent=2))


if __name__ == "__main__":
    run()
