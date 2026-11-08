"""Enclose the complete translated physical residue in the fixed shear normalization."""

import argparse
import platform
import sys
from pathlib import Path
from types import SimpleNamespace

import flint
import numpy as np
import scipy
from flint import acb, arb, ctx
from scipy.integrate import solve_ivp

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-011/checks/physical_overlap"))

from certified_launch import gl_scale, spec
from denominator_refinement import outgoing_at_match, wronskian_derivative
from exterior_profiles import FAR, MATCH, Cell, CompactGL, ScalarProfile, gl_cells, horizon_rw_match, rw_profile, x
from exterior_source import Exterior, matrix, norm2
from horizon_series import LocalHorizon, box, matvec, upper
from ingoing_source import Ingoing, IngoingScalar, matching_enclosure
from mean_certificate import mean_tail
from outer_tail import OuterTail
from return_tail import mean_envelope, returning_tail
from tight_launch import baseline_horizon, normalized_bump
from signed_projection import (
    CAP, DEGREE, Leg, binary_point, dot, horizon_affine, horizon_mean,
    midpoint, solve, transpose, vector_multiply,
)


class UnitBaseline:
    """Residue profile with its overall packet coefficient divided out."""

    moments = {}

    def __init__(self, scalar):
        self.scalar = scalar

    def fields(self, index, t):
        psi, slope = self.scalar.evaluate(index, t)
        return psi, slope, acb(0)


def capture_left(frequency, massive):
    capture = []
    end, _ = spec.compact_bound(frequency, massive, False, spec.CELLS, False, capture=capture)
    return [Cell(*cell) for cell in capture if cell[0] >= CAP], end


class Left(Ingoing):
    def __init__(self, data, sigma):
        self.data = data
        self.s1 = data.s - data.omega
        self.cells, slopes = capture_left(self.s1, True)
        scalar, _ = capture_left(sigma, False)
        assert [(c.a, c.b) for c in scalar] == [(c.a, c.b) for c in self.cells]
        self.baseline = UnitBaseline(IngoingScalar(sigma, scalar))
        self.gl = CompactGL(data, grid=[c.a for c in self.cells] + [self.cells[-1].b])
        self.local = LocalHorizon(data.s, data.k, data.omega, data.gl_scale, acb(1), 48)
        self.start = self.cells[0].radius(acb(0))
        self.slopes = matrix(slopes)


class IngoingRay:
    def __init__(self, s, initial_slope, at_match, outgoing_cells):
        self.s = s
        self.at_match = at_match
        ray = complex(spec.W)
        center = complex(s)
        grid = [outgoing_cells[-1].b] + [c.a for c in reversed(outgoing_cells)]

        def rhs(rho, state):
            r = 2.5 + ray * rho
            f = 1 - 1 / r
            return ray * (f * (6 / r**2 - 3 / r**3) - 2 * center * state - state**2) / f

        approximate = complex(initial_slope - s)
        solution = solve_ivp(rhs, (0, float(FAR)), [approximate],
                             t_eval=grid, method="DOP853", rtol=4e-14, atol=1e-15)
        assert solution.success, solution.message
        samples = [(a, np.array([p]), np.array([rhs(a, p) / ray]))
                   for a, p in zip(grid, solution.y[0])]
        capture = []

        def coefficients(r, t):
            den, q, ell = spec.coefficients(r, t, s, False, False, acb(spec.K), s, acb(0))
            return den, q, ell, t * r**3

        initial_error = upper(initial_slope - s - acb(approximate))
        spec.directed_flow(samples, 1, True, s, acb(0), coefficients,
                           initial_error, arb(0), False, capture, initial_is_error=True)
        self.cells = [Cell(*c) for c in capture]
        self.prefixes = []
        prefix = acb(0)
        for cell in self.cells:
            primitive, error = cell.log_primitive()
            self.prefixes.append((prefix, primitive, error))
            prefix += primitive(1) - primitive(0) + box(error)

    def evaluate(self, index, t):
        cell = self.cells[index]
        # Leg's right path traverses the matching outgoing cells backwards.
        t = 1 - t
        prefix, primitive, error = self.prefixes[index]
        phase = self.s * (x(cell.radius(t)) - x(acb(MATCH)))
        amplitude = self.at_match * (phase + prefix + primitive(t) - primitive(0)
                                     + box(error * upper(t))).exp()
        return amplitude, cell.slopes(t, self.s, acb(0))[0]

    def far(self):
        return self.evaluate(len(self.cells) - 1, acb(0))


class Right(Exterior):
    def __init__(self, data, sigma, returning_left):
        self.data = data
        capture, exponent, power = gl_cells(data)
        self.gl = ScalarProfile(
            capture, exponent, power, data.gl_match[0] * (data.omega * x(acb(MATCH))).exp()
        )
        self.baseline = rw_profile(sigma, horizon_rw_match(sigma))
        capture = []
        s1 = data.s - data.omega
        spec.compact_bound(s1, True, True, spec.CELLS, False, capture=capture, knot=FAR)
        self.massive = [Cell(*cell) for cell in capture if cell[0] <= FAR]
        self.ell, self.power = spec.branch(s1, True, True, data.k)
        self.test = IngoingRay(data.s, returning_left, horizon_rw_match(data.s), self.massive)
        self.tail = OuterTail(k=data.k.real, omega=data.omega.real, s=data.s)
        geometry = [(c.a, c.b) for c in self.massive]
        for scalar in (self.gl, self.baseline):
            assert geometry == [(c.a, c.b) for c in reversed(scalar.cells)]
        assert geometry == [(c.b, c.a) for c in reversed(self.test.cells)]

    def caps(self, state):
        h_far, slope_far = self.test.far()
        tail = self.tail
        R = tail.rho0
        s = self.data.s
        h_log_error = max(arb(".005"), upper(slope_far - s))
        lower = ((tail.ray * s).real - upper(s) / (R - 1)).lower()
        upper_growth = ((tail.ray * s).real + upper(s) / (R - 1)
                        + tail.dx_cap * h_log_error).upper()
        assert 2 * lower * h_log_error > tail.dx_cap * (
            tail.returning.q0 + h_log_error**2
        )
        tail.returning = SimpleNamespace(
            decay=-upper_growth, p_cap=(upper(s) + h_log_error).upper()
        )
        assert tail.gl.decay + tail.mixed.decay > upper_growth
        assert tail.gl.decay * 2 + tail.rw.decay > upper_growth
        u_far = self.gl.far()[0]
        _, mean_decay = mean_envelope(tail, u_far)
        assert tail.rw.decay + mean_decay > upper_growth
        return returning_tail(u_far, self.baseline.far()[0], h_far, state, tail)


def loose_outgoing_at_match(frequency):
    slope, logarithm = spec.compact_bound(frequency, False, True, spec.CELLS, True)
    return slope[0], (logarithm - frequency * x(acb(MATCH))).exp()


def endpoint_box(value):
    return [(q.lower(), q.upper()) for q in (value.real, value.imag)]


def project(subdivisions, loose_normalization=False):
    root_box = spec.shift(spec.CENTER, arb("5e-8"))
    omega, k = acb(spec.OMEGA), acb(spec.K)
    scale, gl_match = gl_scale(k, omega)
    data = SimpleNamespace(s=root_box + 2 * omega, k=k, omega=omega, gl_scale=scale,
                           gl_match=gl_match, launch_left=acb(1))
    # The arithmetic hull in the explicitly shifted hierarchy may be wider.
    sigma = data.s - 2 * omega
    test_cells, test_match = capture_left(data.s, False)
    left = Left(data, sigma)
    right = Right(data, sigma, test_match[0])
    assert [(c.a, c.b) for c in test_cells] == [(c.a, c.b) for c in left.cells]
    pr = matrix(right.massive[-1].slopes(acb(1), right.ell, right.power))
    matching_left, matching_left_error, _ = left.affine(subdivisions)
    matching_right, matching_right_error, _, _ = right.affine(subdivisions)
    _, _, match_error, center_matrix = matching_enclosure(
        matching_left, matching_right, matching_left_error, matching_right_error,
        left.slopes, pr,
    )
    center = center_matrix.entries()
    print("fresh mixed matching center, error", center, match_error, flush=True)

    l = Leg("left", data, left, IngoingScalar(data.s, test_cells))
    r = Leg("right", data, right, right.test)
    cap = acb(left.cells[0].a)
    unit = LocalHorizon(data.s, k, omega, acb(1), acb(1), DEGREE)
    z_exact, constant, part, second = horizon_affine(unit, cap.real)
    z_exact = vector_multiply(scale, z_exact)
    z0 = [binary_point(v) for v in z_exact]
    part = vector_multiply(scale, part)
    constant, second = scale**2 * constant, scale**2 * second
    zl, _ = l.z_flow(z0, subdivisions)
    zr, _ = r.z_flow([acb(0), acb(0)], subdivisions)
    diff = [v - w for v, w in zip(zr, zl)]
    dt = transpose([[p - q for p, q in zip(v, w)] for v, w in zip(left.slopes, pr)])
    q = [binary_point(v) for v in solve([[midpoint(v) for v in row] for row in dt],
                                      [-midpoint(v) for v in diff])]
    mismatch = [v + w for v, w in zip(diff, matvec(dt, q))]
    defect = dot(mismatch, center) + box(norm2(mismatch) * match_error)
    wl, _ = l.w_flow(q)
    wr, _ = r.w_flow(q)
    p_cap = matrix(left.cells[0].slopes(acb(0), left.s1, acb(0)))
    affine0 = [part[i+2] + left.s1 * part[i] - dot(p_cap[i], part[:2]) for i in range(2)]
    far_affine = right.tail.affine_bound(right.gl.far()[0], right.baseline.far()[0])
    left_affine = l.affine_envelope(affine0)
    right_affine = r.affine_envelope([box(far_affine), box(far_affine)])
    left_state, collar_state = l.state_envelope(norm2(center) + match_error, left_affine)
    right_state, far_state = r.state_envelope(norm2(center) + match_error, right_affine)
    collar_mismatch = norm2([a - b for a, b in zip(z0, z_exact)]) * collar_state
    far_mean = mean_tail(right)
    r_result, match_mean, r_defect = r.project([box(far_mean), box(far_mean)], right_state, right_affine)
    l_result, collar_mean, l_defect = l.project([acb(v.real) for v in match_mean], left_state, left_affine)
    endpoints = constant + dot(wl, affine0) + defect + box(norm2(wr) * far_affine + collar_mismatch)
    collar = horizon_mean(unit, cap.real, scale, acb(1), [acb(v.real) for v in collar_mean])
    caps = right.caps(far_state)
    totals = [a + b + c + box(e) for a, b, c, e in zip(
        (endpoints, second, collar), l_result, r_result,
        (l_defect + r_defect + caps.hj, caps.quadratic, caps.mean),
    )]
    print("projected pieces for the unit horizon mode", totals, flush=True)
    print("absolute tail bounds", caps, flush=True)
    numerator = sum(totals)
    print("unit numerator", endpoint_box(numerator), flush=True)
    assert numerator.abs_lower() > 0, "enclosure cannot exclude cancellation"

    launch_moment, _ = baseline_horizon(root_box, normalized_bump(), moments_only=True)
    derivative, _, h_q, g_q, _ = wronskian_derivative()
    if loose_normalization:
        _, g_q = loose_outgoing_at_match(root_box)
    launch = (-launch_moment / derivative) * (g_q / h_q)
    print("baseline horizon-normalized residue", endpoint_box(launch), flush=True)
    if loose_normalization:
        right_slope, g = loose_outgoing_at_match(data.s)
    else:
        right_slopes, _ = spec.compact_bound(data.s, False, True, spec.CELLS, False)
        right_slope = right_slopes[0]
        g, _ = outgoing_at_match(data.s)
    returning_w = right.test.at_match * g * (right_slope - test_match[0])
    assert returning_w.abs_lower() > 0
    print("returning W", endpoint_box(returning_w), flush=True)
    amplitude = numerator * launch / returning_w
    print("shifted physical shear rectangle", endpoint_box(amplitude), flush=True)
    print("shifted physical shear modulus", amplitude.abs_lower(), amplitude.abs_upper(), flush=True)
    assert amplitude.abs_lower() > 0
    print("PASS physical translated residue excludes zero on the pinned boxes")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--subdivisions", type=int, default=2)
    parser.add_argument("--loose-normalization", action="store_true",
                        help="Reproduce the loose outgoing-amplitude failure")
    args = parser.parse_args()
    ctx.prec = 128
    ctx.cap = 128
    print("Python", platform.python_version(), "python-flint", flint.__version__,
          "NumPy", np.__version__, "SciPy", scipy.__version__, args, flush=True)
    project(args.subdivisions, args.loose_normalization)


if __name__ == "__main__":
    main()
