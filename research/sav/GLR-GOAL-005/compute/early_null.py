#!/usr/bin/env python3
"""Non-production scalar quadrature for finite-slice L10 Z data.

The physical source input is an empirical saved GL profile. The exact
Volterra iteration bounds do not certify the profile or mesh quadrature.
"""

import json
from pathlib import Path
import platform

import numpy as np
import scipy
from scipy.integrate import cumulative_trapezoid, quad, simpson, trapezoid
from scipy.interpolate import CubicSpline
from scipy.optimize import brentq
from scipy.special import wrightomega


HERE = Path(__file__).resolve().parent
INPUTS = HERE.parents[1] / "verification"
U_START, U_END = -10.0, -8.0
R_TAPER, R_ZERO = 60.0, 65.0


def slice_u(b):
    return -b - 2 * np.log(b - 1)


def slice_radius(u):
    return brentq(lambda b: slice_u(b) - u, 2, 15)


def tortoise(r):
    return r + np.log(r - 1)


B_MIN, B_START = slice_radius(U_END), slice_radius(U_START)


class SavedSource:
    """Direct reconstruction, plus an independent CSV-source interpolant."""

    def __init__(self):
        metadata = json.loads((INPUTS / "gl_linear_results.json").read_text())
        profile = np.genfromtxt(
            INPUTS / "gl_linear_profile_L10.csv", delimiter=",", names=True
        )
        self.k2 = metadata["k"] ** 2
        self.omega = metadata["refinements"][-1]["omega"]
        self.s = 2 * self.omega
        self.scale = metadata["profile"]["scale"]
        exterior = profile[profile["r"] >= 4]
        log_radius = np.log(exterior["r"] - 1)
        self.log_master = CubicSpline(log_radius, exterior["log_u_relative_match"])
        self.riccati = CubicSpline(log_radius, exterior["p"])
        self.log_raw_source = CubicSpline(log_radius, np.log(exterior["S_raw"]))

    def master(self, r):
        return self.scale * np.exp(self.log_master(np.log(r - 1)))

    def __call__(self, radius, raw=False):
        r = np.minimum(radius, R_ZERO)
        delta = r - 1
        f = delta / r
        weight = np.clip((R_ZERO - radius) / (R_ZERO - R_TAPER), 0, 1)
        taper = weight**3 * (10 - 15 * weight + 6 * weight**2)
        if raw:
            return taper * np.exp(self.log_raw_source(np.log(delta)))
        k2 = self.k2
        d = 1 + k2 * r**3
        m = self.master(r)
        y = d * m / r
        yp = (2 * k2 * r - r**-2) * m + y * self.riccati(np.log(delta)) / f
        h_tt = (delta * (0.5 - k2 * r**3) / (r * d**2) - self.omega**2 / (k2 * d)) * y
        h_tt += (delta / (2 * k2 * r**3 * d) - 3 * delta**2 / d**2) * yp
        h_tr = -self.omega * (yp - y / (2 * r * delta)) / (k2 * d)
        h_ang = -y / (2 * d) - delta * yp / (k2 * d * r**2)
        h_rr = h_tt / f**2 - 2 * h_ang / f
        contraction = (h_tt / f) ** 2 + (f * h_rr) ** 2 - 2 * h_tr**2 + 2 * h_ang**2
        return taper * np.sqrt(3) * k2 * delta * contraction / 16


class NullQuadrature:
    """Aligned trapezoids in b and v, with p=-1/v after the last b row."""

    def __init__(self, resolution):
        segments = (
            np.linspace(B_MIN, B_START, resolution + 1),
            np.linspace(B_START, 18, 3 * resolution + 1)[1:],
            np.linspace(18, R_ZERO, 3 * resolution + 1)[1:],
        )
        self.b = np.concatenate(segments)
        self.u = slice_u(self.b)
        self.early = slice(0, resolution + 1)
        self.last = len(self.b)
        self.p = np.linspace(-1 / R_ZERO, 0, 2 * resolution + 1)
        self.v = np.concatenate((self.b, -1 / self.p[1:-1]))
        self.g = (self.b + 1) / (self.b - 1)
        self.mask = self.v[None, :] >= self.b[:, None]
        x = (self.v[None, :] - self.u[:, None]) / 2
        self.r = 1 + wrightomega(x - 1)
        self.time = (self.v[None, :] + self.u[:, None]) / 2
        self.potential = np.where(self.mask, (self.r - 1) / self.r**4, 0)

    def _join_compactified_tail(self, integrand, endpoint):
        near = integrand[:, : self.last]
        tail = integrand[:, self.last - 1 :] * self.v[None, self.last - 1 :] ** 2
        return near, np.column_stack((tail, np.broadcast_to(endpoint, (self.last,))))

    def _inner(self, weighted, include):
        increments = np.diff(self.b)
        if weighted.ndim > 1:
            increments = increments[:, None]
        cells = (weighted[1:] + weighted[:-1]) * (increments / 2) * include
        result = np.zeros_like(weighted)
        result[:-1] = np.cumsum(cells[::-1], axis=0)[::-1]
        return result

    def potential_integral(self, integrand, endpoint):
        """The double integral, evaluated at each finite (b,v) node."""
        inner = self._inner(self.g[:, None] * integrand, self.mask[1:]) / 4
        inner_endpoint = self._inner(self.g * np.broadcast_to(endpoint, self.g.shape), 1) / 4
        near, tail = self._join_compactified_tail(inner, inner_endpoint)
        left = cumulative_trapezoid(near, self.b, axis=1, initial=0)
        right = left[:, -1:] + cumulative_trapezoid(tail, self.p, axis=1, initial=0)
        joined = np.column_stack((left, right[:, 1:]))
        return joined[:, :-1], joined[:, -1]

    def news(self, integrand, endpoint):
        near, tail = self._join_compactified_tail(integrand, endpoint)
        cells = (near[:, :-1] + near[:, 1:]) * (np.diff(self.b)[None, :] / 2)
        cells *= self.mask[:, : self.last - 1]
        return (np.sum(cells, axis=1) + trapezoid(tail, x=self.p, axis=1)) / 4

    def solve(self, source, source_endpoint=0, iterations=12, potential_sign=1):
        free, infinity_free = self.potential_integral(source, source_endpoint)
        iterand = free
        n0 = self.news(source, source_endpoint)
        n1 = self.news(source - potential_sign * self.potential * free, source_endpoint)
        for _ in range(iterations):
            correction, infinity_correction = self.potential_integral(
                -potential_sign * self.potential * iterand, 0
            )
            previous = iterand
            iterand = free + correction
        return dict(
            free_news=n0,
            first_corrected_news=n1,
            news=self.news(source - potential_sign * self.potential * iterand, source_endpoint),
            waveform=infinity_free + infinity_correction,
            last_field_difference=float(np.max(np.abs(iterand - previous))),
        )


def free_radial(source, b, *, raw=False, absolute=False):
    """Independent single radial quadratures for free news and waveform."""
    u = slice_u(b)
    s = source.s

    def amplitude(r):
        value = source(r, raw=raw) / (1 - 1 / r)
        return abs(value) if absolute else value

    def integrand(r):
        return np.exp(s * (u + tortoise(r))) * amplitude(r) / 2

    def integrated(r):
        return (r - 1) ** -s * np.expm1(s * (u - slice_u(r))) * amplitude(r) / (2 * s)

    options = dict(epsabs=2e-15, epsrel=2e-11, limit=160, points=[R_TAPER])
    return quad(integrand, b, R_ZERO, **options)[0], quad(integrated, b, R_ZERO, **options)[0]


def manufactured(grid):
    # Exact test ψ=h(b)(1-b/v)^2 with both slice traces zero.
    cutoff = (R_ZERO - grid.b) / (R_ZERO - B_MIN)
    exponential = np.exp(-(grid.b - B_MIN))
    h = cutoff**4 * exponential
    hp = -(h + 4 * cutoff**3 * exponential / (R_ZERO - B_MIN))
    b = grid.b[:, None]
    v = grid.v[None, :]
    radial = 1 - b / v
    exact = h[:, None] * radial**2
    mixed = -8 * ((b * hp[:, None] + h[:, None]) * radial - b * h[:, None] / v)
    source = np.where(grid.mask, mixed / (grid.g[:, None] * v**2) + grid.potential * exact, 0)
    source_endpoint = -8 * (grid.b * hp + h) / grid.g
    target = -hp / grid.g
    correct = grid.solve(source, source_endpoint)
    wrong = grid.solve(source, source_endpoint, potential_sign=-1)
    news_error = np.max(np.abs(correct["news"] - target))
    wrong_sign_error = np.max(np.abs(wrong["news"] - target))
    assert news_error < wrong_sign_error / 5
    return dict(max_news_error=float(news_error), wrong_potential_sign_max_news_error=float(wrong_sign_error),
                max_waveform_error=float(np.max(np.abs(correct["waveform"] - h))))


def run():
    source = SavedSource()
    sample_b = [B_MIN, slice_radius(-9), B_START]
    free_refs = np.asarray([free_radial(source, b) for b in sample_b])
    raw_refs = np.asarray([free_radial(source, b, raw=True) for b in sample_b])
    _, absolute_free = free_radial(source, B_MIN, absolute=True)
    kappa = (2 * np.log(B_MIN / (B_MIN - 1)) - 1 / B_MIN) / 4
    news_norm = 1 / (4 * B_MIN**2)
    configurations = []
    for resolution in (32, 64, 128):
        grid = NullQuadrature(resolution)
        r = grid.r
        force = np.where(grid.mask & (r < R_ZERO),
                         np.exp(source.s * np.minimum(grid.time, R_ZERO + 10)) * source(r), 0)
        solved = grid.solve(force)
        indices = grid.early
        u = grid.u[indices][::-1]

        def energy(news):
            return float(2 * simpson(news[indices][::-1] ** 2, x=u))

        def at_reference(field):
            middle = CubicSpline(u, field[indices][::-1])(-9)
            return np.array([field[0], middle, field[resolution]])

        record = dict(
            resolution=resolution, b_nodes=grid.last, finite_v_nodes=len(grid.v),
            news_u_minus_8_9_10=at_reference(solved["news"]).tolist(),
            waveform_u_minus_8_9_10=at_reference(solved["waveform"]).tolist(),
            free_news_u_minus_8_9_10=at_reference(solved["free_news"]).tolist(),
            first_corrected_news_u_minus_8_9_10=at_reference(solved["first_corrected_news"]).tolist(),
            energy=energy(solved["news"]), energy_free=energy(solved["free_news"]),
            energy_first_corrected=energy(solved["first_corrected_news"]),
            max_relative_free_news_error_vs_radial_quadrature=float(np.max(
                np.abs(at_reference(solved["free_news"]) / free_refs[:, 0] - 1))),
            max_relative_phi_prime_error=float(np.max(np.abs(
                CubicSpline(u, solved["waveform"][indices][::-1])(u, 1)
                / solved["news"][indices][::-1] - 1))),
            last_field_difference=solved["last_field_difference"],
        )
        if resolution == 32:
            record["manufactured"] = manufactured(grid)
        if resolution == 128:
            np.savetxt(HERE / "early_null_waveform.csv",
                       np.column_stack((u, solved["waveform"][indices][::-1],
                                        solved["news"][indices][::-1])), delimiter=",",
                       header="u_over_r0,Phi2_over_r0,N2", comments="", fmt="%.16e")
        configurations.append(record)
        print(json.dumps(record), flush=True)

    s = source.s
    m60 = float(source.master(R_TAPER))
    # The bound is multiplied by |m(60)|² of the true normalized solution.
    # Reporting it with the sampled value does not certify the continuum tail.
    tail_multiplier = (50 * R_TAPER**5 * np.exp(s * (U_END + tortoise(R_TAPER)))
                       / (1.2 - s - 5 / R_TAPER))
    output = dict(
        python=platform.python_version(), numpy=np.__version__, scipy=scipy.__version__,
        benchmark=dict(r0=1, L=10, omega=source.omega, k=float(np.sqrt(source.k2)),
                       u_window=[U_START, U_END], b_min=B_MIN, b_start=B_START),
        kernel=dict(kappa=float(kappa), news_operator_norm=float(news_norm),
                    absolute_free_waveform_quadrature_estimate=absolute_free,
                    news_error_first_corrected_bound_using_sampled_absolute_free=float(
                        news_norm * absolute_free * kappa / (1 - kappa)),
                    news_error_12_updates_bound_using_sampled_absolute_free=float(
                        news_norm * absolute_free * kappa**13 / (1 - kappa))),
        independent_free_radial=dict(u=[-8, -9, -10], columns=["news", "waveform"],
                                     reconstruct=free_refs.tolist(), raw=raw_refs.tolist()),
        source_tail=dict(taper_starts=R_TAPER, taper_ends=R_ZERO,
                         direct_news_bound_per_true_m60_squared=float(tail_multiplier),
                         sampled_m60=m60,
                         direct_news_bound_if_m60_exact=float(tail_multiplier * m60**2)),
        configurations=configurations,
        scope="Saved-profile linear finite-time response with floating mesh and quadrature evidence."
    )
    (HERE / "early_null_results.json").write_text(json.dumps(output, indent=2) + "\n")


if __name__ == "__main__":
    run()
