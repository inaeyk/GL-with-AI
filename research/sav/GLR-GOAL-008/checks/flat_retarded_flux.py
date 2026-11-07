#!/usr/bin/env python3
"""Manufactured half-line Green-function and energy-flux cross-check."""

import json
import platform

import numpy as np
import scipy
from scipy.integrate import cumulative_simpson, quad, simpson
from scipy.special import j0, j1, roots_legendre


S = 0.12656237795181546  # Pinned empirical L10 value, used only as a clock.
A = 2.0                # Manufactured source location on the half line.


def history(t):
    return np.exp(-np.logaddexp(S * t, -S * t))


def time_components(t, x, mu, n, cutoff):
    nodes, weights = roots_legendre(n)

    def one_front(distance):
        end = np.minimum(cutoff, t - distance)
        half = np.maximum(0, end + cutoff) / 2
        t_source = -cutoff + half[:, None] * (nodes + 1)
        elapsed = t[:, None] - t_source
        interval = np.sqrt(np.maximum(0, elapsed**2 - distance**2))
        h = history(t_source)
        j0_value = j0(mu * interval)
        j1_ratio = np.full_like(interval, mu / 2)
        np.divide(j1(mu * interval), interval, out=j1_ratio, where=interval > 0)
        return np.asarray((
            half * ((j0_value * h) @ weights) / 2,
            -S * half * ((j0_value * np.tanh(S * t_source) * h) @ weights) / 2,
            -history(t - distance) / 2 + mu * distance * half * ((j1_ratio * h) @ weights) / 2,
        ))

    return one_front(x - A) - one_front(x + A)


def spectral_energy(mu):
    # p is the propagating momentum: omega*p*domega = p^2*dp.
    def integrand(p):
        omega = np.hypot(mu, p)
        h_transform = np.pi / S * history(np.pi * omega / (2 * S**2))
        amplitude = h_transform * A * np.sinc(p * A / np.pi)
        return p**2 * amplitude**2 / np.pi

    return quad(integrand, 0, np.inf, epsabs=3e-13, epsrel=3e-12)[0]


def run():
    configurations = ((0.5, 144, 160), (0.25, 216, 200))
    masses = (0.0, 0.10, 0.30)
    radii = (12.0, 24.0, 48.0)
    windows = (0.0, 50.0, 150.0, 300.0, 600.0, 1200.0)
    spectra = {m: spectral_energy(m) for m in masses}
    results = []
    for step, order, cutoff in configurations:
        u = np.arange(-200, max(windows) + step / 2, step)
        for mu in masses:
            for x in radii:
                _, dt, dx = time_components(u + x, x, mu, order, cutoff)
                j = -dt * dx
                energy = [float(simpson(j[u <= w], x=u[u <= w])) for w in windows]
                cumulative = cumulative_simpson(j, x=u, initial=0)
                i_half = np.flatnonzero(cumulative >= 0.5 * spectra[mu])[0]
                crossing = slice(i_half - 1, i_half + 1)
                half_energy_cut = np.interp(0.5 * spectra[mu], cumulative[crossing], u[crossing])
                results.append(dict(mu=mu, x=x, step=step, gauss_order=order,
                                    source_time_cutoff=cutoff, energy_through_retarded_cut=energy,
                                    final_relative_error=energy[-1] / spectra[mu] - 1,
                                    half_energy_retarded_cut=float(half_energy_cut)))
    assert max(abs(result["final_relative_error"]) for result in results) < 3e-4
    assert max(abs(
        one["energy_through_retarded_cut"][-1] - two["energy_through_retarded_cut"][-1]
    ) for one, two in zip(results[:9], results[9:])) < 2e-10
    assert all(
        abs(result["final_relative_error"])
        < abs(result["energy_through_retarded_cut"][-2] / spectra[result["mu"]] - 1)
        for result in results if result["mu"] > 0
    )
    print(json.dumps(dict(
        environment=dict(python=platform.python_version(), numpy=np.__version__, scipy=scipy.__version__),
        s=S, source_position=A, retarded_cuts=windows, spectral_energies=spectra, runs=results,
    ), indent=2))


if __name__ == "__main__":
    run()
