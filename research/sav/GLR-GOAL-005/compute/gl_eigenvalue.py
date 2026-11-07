#!/usr/bin/env python3
"""L10 non-production spectral check and interval Riccati bracket.

The finite-difference check is empirical. The endpoint signs in the
separate bracket use Arb ball arithmetic and analytic endpoint bounds.
"""

import json
from pathlib import Path
import platform

import flint
from flint import arb, arb_poly
import numpy as np
import scipy
from scipy.integrate import solve_ivp
from scipy.linalg import eigh_tridiagonal
from scipy.special import wrightomega


HERE = Path(__file__).resolve().parent
LEFT = 1 + 2**-34
RIGHT = 60
OMEGA_LOW, OMEGA_HIGH = "0.06328117", "0.06328121"
K2 = (np.pi / 5) ** 2
flint.ctx.prec = 112


def potential(r):
    d = 1 + K2 * r**3
    return ((r - 1) / r**4
            * (K2 * r**3 + 6 * r - 11 + 18 * (r - 1) / d**2 - 6 * (4 * r - 5) / d))


def spectrum():
    """Centered differences and Dirichlet truncations, with no matching."""
    results = []
    for left, right, h in [(-90, 30, .05), (-120, 45, .025), (-150, 60, .0125)]:
        x = np.linspace(left, right, round((right - left) / h) + 1)[1:-1]
        r = 1 + wrightomega(x - 1)
        diagonal = 2 / h**2 + potential(r)
        off_diagonal = np.full(len(x) - 1, -1 / h**2)
        values, modes = eigh_tridiagonal(diagonal, off_diagonal, select="i", select_range=(0, 0))
        vector = modes[:, 0]
        product = diagonal * vector
        product[1:] += off_diagonal * vector[:-1]
        product[:-1] += off_diagonal * vector[1:]
        results.append(dict(x_left=left, x_right=right, h=h, points=len(x),
                            omega=float(np.sqrt(-values[0])),
                            matrix_residual_l2=float(np.linalg.norm(product - values[0] * vector)),
                            minimum_positive_vector_sample=float(np.min(np.abs(vector))),
                            sign_changes=int(np.count_nonzero(vector[1:] * vector[:-1] < 0))))
    return results


def candidate(omega, left_side):
    """Construct candidate p(r) only. None of its solver tolerances are trusted."""
    if left_side:
        start, stop = LEFT, 2
        first = (K2 - 5 + 6 / (1 + K2)) / (1 + 2 * omega)
        initial = omega + first * (LEFT - 1)
        points = 12289
    else:
        start, stop = RIGHT, 2
        q = np.hypot(np.sqrt(K2), omega)
        initial = -q + K2 / (2 * q * RIGHT)
        points = 8193

    def rhs(s, p):
        r = 1 + np.exp(s)
        return r * (potential(r) + omega**2 - p**2)

    ss = np.linspace(np.log(start - 1), np.log(stop - 1), points)
    solution = solve_ivp(rhs, (ss[0], ss[-1]), [initial], t_eval=ss, method="DOP853",
                         rtol=3e-14, atol=1e-15)
    assert solution.success
    rr = 1 + np.exp(ss)
    rr[0], rr[-1] = start, stop
    pp = solution.y[0]
    dd = rr / (rr - 1) * (potential(rr) + omega**2 - pp**2)
    return rr, pp, dd


def cell_bounds(r0, r1, p0, p1, d0, d1, omega2, k2):
    """Return a p range and bound on |d_x p - A + p²| on one exact Hermite cell."""
    step = r1 - r0
    delta_p = p1 - p0
    # Coefficients are in local t=(r-r0)/step. The polynomial is continuous
    # at mesh nodes because the same binary point data define both neighbors.
    p = arb_poly([p0, step * d0, 3 * delta_p - step * (2 * d0 + d1),
                  -2 * delta_p + step * (d0 + d1)])
    dp_dr = p.derivative() * (1 / step)
    r = arb_poly([r0, step])
    r3 = r**3
    r4 = r3 * r
    d = r3 * k2 + 1
    d2 = d**2
    q = (r3 * k2 + r * 6 - 11) * d2 + (r - 1) * 18 - (r * 4 - 5) * d * 6
    residual_numerator = ((r - 1) * (r3 * d2 * dp_dr - q)
                          + r4 * d2 * (p**2 - omega2))
    t_range = arb(.5, .5)
    r_min = r0.min(r1)
    denominator_floor = r_min**4 * (1 + k2 * r_min**3)**2
    residual = (residual_numerator(t_range).abs_upper() / denominator_floor).upper()
    length = step + ((r1 - 1) / (r0 - 1)).log()
    return p(t_range), residual, length


def validate_branch(omega, left_side, capture_cells=False):
    """Propagate rigorous Riccati errors under stated analytic endpoint bounds."""
    k2 = (arb.pi() / 5)**2
    rr, pp, dd = candidate(float(omega.mid()), left_side)
    rr = [arb(float(r)) for r in rr]
    pp = [arb(float(p)) for p in pp]
    dd = [arb(float(d)) for d in dd]
    if left_side:
        error = (12 * (rr[0] - 1) + (pp[0] - omega).abs_upper()).upper()
        bootstrap = arb("0.001")
    else:
        error = (pp[0] - arb("-.65").union(arb("-.60"))).abs_upper()
        bootstrap = arb("0.05")
    assert error < bootstrap
    initial_error = error
    worst_residual = arb(0)
    cells = []
    for i in range(len(rr) - 1):
        p_box, residual, step = cell_bounds(rr[i], rr[i+1], pp[i], pp[i+1], dd[i], dd[i+1],
                                             omega**2, k2)
        worst_residual = worst_residual.max(residual).upper()
        previous_error = error
        if left_side:
            rate = (-2 * p_box.lower()).max(0) + bootstrap
            factor = (rate * step).exp()
            error = (factor * error + (factor - 1) * residual / rate).upper()
        else:
            # In backwards x, d|error|/d(-x) <= (2p+0.05)|error|+|residual|.
            # p<-0.44 on each cell implies uniform damping by at least 0.83.
            assert p_box < arb("-0.44")
            rate = arb("0.83")
            factor = (rate * step).exp()  # step<0
            assert residual / rate < bootstrap
            error = (factor * error + (1 - factor) * residual / rate).upper()
        assert error < bootstrap
        if capture_cells:
            cells.append((rr[i], rr[i+1], p_box + arb(0, error.max(previous_error).upper()), step))
    enclosure = pp[-1] + arb(0, error)
    stats = dict(initial_error=str(initial_error), endpoint_error=str(error),
                 max_cell_residual=str(worst_residual), endpoint=str(enclosure), cells=len(rr)-1)
    if capture_cells:
        return enclosure, stats, cells
    return enclosure, stats


def horizon_connection(left_branches):
    """Enclose the horizon amplitude by reusing the two validated left branches."""
    (p_low, cells_low), (p_high, cells_high) = left_branches
    omega = arb(OMEGA_LOW).union(arb(OMEGA_HIGH))
    k2 = (arb.pi() / 5)**2
    d_at_two = 1 + 8 * k2
    p_at_two = p_low.union(p_high)
    y_r_over_m = 4 * k2 - arb(".25") + d_at_two * p_at_two
    m_at_two = 2 / (-arb(".25") - y_r_over_m / (4 * k2 * d_at_two))
    delta_left = arb(LEFT) - 1
    horizon_tail = 12 * (delta_left + delta_left**2 / 2)
    p_minus_omega_integral = arb(0, horizon_tail.upper())
    for low, high in zip(cells_low, cells_high, strict=True):
        _, _, p1, length = low
        _, _, p2, _ = high
        p_minus_omega_integral += (p1.union(p2) - omega) * length
    # y=(D/r)m, and x(2)=2. The limiting regular factor is
    # m(r)/(r-1)^Omega = m(2)*exp(-Omega-int_{-infty}^{2}(p-Omega) dx).
    a_h = (1 + k2) * m_at_two * (-omega - p_minus_omega_integral).exp()
    beta = abs(a_h) / (4 * (1 + k2))
    return dict(a_h_lower=str(a_h.lower()), a_h_upper=str(a_h.upper()),
                apparent_horizon_contrast_slope_lower=str(beta.lower()),
                apparent_horizon_contrast_slope_upper=str(beta.upper()))


def run():
    results = spectrum()
    print(json.dumps(dict(finite_difference=results)), flush=True)
    matched = []
    left_branches = []
    for omega_label in (OMEGA_LOW, OMEGA_HIGH):
        omega = arb(omega_label)
        left, left_stats, left_cells = validate_branch(omega, True, capture_cells=True)
        left_branches.append((left, left_cells))
        right, right_stats = validate_branch(omega, False)
        mismatch = left - right
        result = dict(omega=omega_label, left=left_stats, right=right_stats,
                      mismatch=str(mismatch), mismatch_lower=str(mismatch.lower()),
                      mismatch_upper=str(mismatch.upper()))
        matched.append(result)
        print(json.dumps(result), flush=True)
        if omega_label == OMEGA_LOW:
            assert mismatch < 0
        else:
            assert mismatch > 0
    report = dict(python=platform.python_version(), scipy=scipy.__version__, numpy=np.__version__,
                  flint=flint.__version__, arb_precision_bits=flint.ctx.prec, L=10,
                  finite_difference=results, matched=matched,
                  certified_root_open_interval=[OMEGA_LOW, OMEGA_HIGH],
                  horizon_connection=horizon_connection(left_branches))
    print(json.dumps(report["horizon_connection"]), flush=True)
    (HERE / "gl_eigenvalue_results.json").write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    run()
