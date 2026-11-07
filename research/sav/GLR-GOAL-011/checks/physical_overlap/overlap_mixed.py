"""Non-production forced massive axial radial solve, r0=1 and ell=2.

MixedSolution(source, s1, k, rmatch, ray, length, left_delta, tol).
source(r) returns (Xtx, Yt, Yx) in (t,x,z) vector-density conventions,
including its radial exponential. It must accept the complex outgoing ray.
MixedSolution(r) returns (At, Ax, Ftx, Az) for real r>=1+left_delta.
Input is the full cosine coefficient, represented in the +k equations.
Call prepare_right(rmax) before a batch of real outer radial evaluations.
ray_fields(distance) returns the same tuple on the solved complex ray.
"""

import bisect
import cmath
import math

import numpy as np
from scipy.integrate import solve_ivp


LAMBDA = 4.0
SQRT_LAMBDA = 2.0
EXCHANGE = np.array([[0, 1], [1, 0]], dtype=complex)
IDENTITY = np.eye(2, dtype=complex)


def tortoise(r):
    """Analytic branch in the right radial sector."""
    return r + cmath.log(r - 1)


class MixedSolution:
    """Inhomogeneous stationary Green solve by two-ended matching."""

    def __init__(self, source, s1, k, rmatch=2.5,
                 ray=cmath.exp(1j * math.pi / 4), length=72,
                 left_delta=1e-5, tol=2e-10):
        if not 0 < left_delta < rmatch - 1 or not length > 0 or not tol > 0:
            raise ValueError("Require 0 < left_delta < rmatch-1, length>0, tol>0")
        self.source = source
        self.s = complex(s1)
        self.k = float(k)
        self.rmatch = float(rmatch)
        # A real ray argument is an angle in radians; a complex ray is its direction.
        self.ray = cmath.exp(1j * ray) if isinstance(ray, (float, int)) else ray
        if abs(abs(self.ray) - 1) > 1e-12 or self.ray.real <= 0:
            raise ValueError("ray must specify a unit direction in the right half-plane")
        self.length = float(length)
        self.left_delta = float(left_delta)
        self.left_r = 1 + self.left_delta
        self.tol = float(tol)
        self.kappa = cmath.sqrt(self.s**2 + self.k**2)
        if self.s.imag * self.kappa.imag < 0:
            self.kappa = -self.kappa
        if (self.kappa * self.ray).real <= 0:
            raise ValueError("outgoing exponent does not decay on this ray")
        if 1 + 2 * self.s.real <= 0:
            raise ValueError("left endpoint lies outside the regular convergence strip")
        self.power = self.k**2 / (2 * self.kappa)
        self._real_solutions = []
        self._real_bounds = []
        self._match()

    def _background(self, r):
        f = 1 - 1 / r
        f_r = 1 / r**2
        d = LAMBDA + self.k**2 * r**2
        root_d = cmath.sqrt(d)
        root_D = r * root_d
        root_W = SQRT_LAMBDA * r**2 / root_d
        radial = self.k**2 * r / d
        radial_r = self.k**2 / d - 2 * self.k**4 * r**2 / d**2
        a_rlog = 1 / r + radial
        b_rlog = 2 / r - radial
        a = f * a_rlog
        b = f * b_rlog
        a_x = f * (f_r * a_rlog + f * (-1 / r**2 + radial_r))
        b_x = f * (f_r * b_rlog + f * (-2 / r**2 - radial_r))
        base = f * (self.k**2 + LAMBDA / r**2)
        c = 1j * self.k / (SQRT_LAMBDA * root_d)
        mix = 2j * self.k * SQRT_LAMBDA * f / d
        effective = np.diag([base + a*a - a_x, base + b*b + b_x])
        effective = effective - self.s * mix * EXCHANGE
        return f, f_r, root_D, root_W, a, b, c, effective

    def _forcing(self, r, bg):
        f, _, root_D, _, a, b, c, _ = bg
        x_tx, y_t, y_x = self.source(r)
        force_position = np.array([y_t / root_D, -c * y_x], dtype=complex)
        # Form G+sF directly to retain the exact regular-horizon cancellation.
        force_regular = np.array([
            (self.s * (y_x + y_t) - a * y_t) / root_D
            + f * root_D * x_tx / r**4,
            -c * (self.s * (y_t + y_x) + b * y_x),
        ], dtype=complex)
        return force_position, force_regular

    def _factor(self, r, bg, left):
        if left:
            return cmath.exp(self.s * tortoise(r)), self.s, 0
        f, f_r = bg[:2]
        factor = cmath.exp(-self.kappa * tortoise(r) + self.power * cmath.log(r))
        log_slope = -self.kappa + self.power * f / r
        log_slope_x = self.power * f * (f_r / r - f / r**2)
        return factor, log_slope, log_slope_x

    def _rhs(self, r, dx_dparameter, vector, left):
        bg = self._background(r)
        factor, log_slope, log_slope_x = self._factor(r, bg, left)
        position, slope = vector.reshape(2, 2, -1)
        remainder = bg[-1] + (self.s**2 - log_slope**2 - log_slope_x) * IDENTITY
        d_position = slope.copy()
        d_slope = remainder @ position - 2 * log_slope * slope
        force_position, force_regular = self._forcing(r, bg)
        d_position[:, -1] += force_position / factor
        d_slope[:, -1] += (force_regular - (self.s + log_slope) * force_position) / factor
        return dx_dparameter * np.stack((d_position, d_slope)).reshape(-1)

    def _integrate(self, rhs, span, initial, dense):
        sol = solve_ivp(
            rhs, span, initial.reshape(-1), method="DOP853",
            rtol=self.tol, atol=self.tol * 0.01, dense_output=dense,
        )
        if not sol.success:
            raise RuntimeError(sol.message)
        return sol

    def _left_initial(self):
        d_h = LAMBDA + self.k**2
        a_h = 1 + self.k**2 / d_h
        b_h = 2 - self.k**2 / d_h
        vh = np.diag([d_h - a_h, d_h + b_h]).astype(complex)
        vh -= self.s * 2j * self.k * SQRT_LAMBDA / d_h * EXCHANGE
        homogeneous_first = vh / (1 + 2 * self.s)
        r = self.left_r
        rp = 1 + 2 * self.left_delta
        bg = self._background(r)
        f = bg[0]
        fpos, freg = self._forcing(r, bg)
        fpos2, _ = self._forcing(rp, self._background(rp))
        fpos /= self._factor(r, bg, True)[0]
        fpos2 /= cmath.exp(self.s * tortoise(rp))
        freg /= self._factor(r, bg, True)[0]
        # Set the particular's regular horizon value to zero. Its first
        # derivative follows from m''+2s*m'-V*m=F_e'+(G+sF)_e.
        particular_first = ((fpos2 - fpos) / (rp - r) + freg / f) / (1 + 2 * self.s)
        position = np.column_stack((
            IDENTITY + self.left_delta * homogeneous_first,
            self.left_delta * particular_first,
        ))
        slope = np.column_stack((f * homogeneous_first, f * particular_first - fpos))
        return np.stack((position, slope))

    def _unfactor(self, r, vector, left):
        position, slope = vector.reshape(2, 2, -1)
        factor, log_slope, _ = self._factor(r, self._background(r), left)
        return factor * np.concatenate((position, log_slope * position + slope), axis=0)

    def _match(self):
        def left_rhs(y, vector):
            r = 1 + math.exp(y)
            return self._rhs(r, r, vector, left=True)

        def right_rhs(a, vector):
            r = self.rmatch + self.ray * a
            return self._rhs(r, self.ray / (1 - 1 / r), vector, left=False)

        self._left = self._integrate(
            left_rhs, (math.log(self.left_delta), math.log(self.rmatch - 1)),
            self._left_initial(), dense=True,
        )
        right_initial = np.zeros((2, 2, 3), dtype=complex)
        right_initial[0, :, :2] = IDENTITY
        right = self._integrate(right_rhs, (self.length, 0), right_initial, dense=True)
        self._right_ray = right
        left_end = self._unfactor(self.rmatch, self._left.y[:, -1], left=True)
        right_end = self._unfactor(self.rmatch, right.y[:, -1], left=False)
        q_left, r_left = np.linalg.qr(left_end[:, :2])
        q_right, r_right = np.linalg.qr(right_end[:, :2])
        mat = np.column_stack((q_left, -q_right))
        self.sigma_min = float(np.linalg.svd(mat, compute_uv=False)[-1])
        self.nfev = (self._left.nfev, right.nfev)
        amplitudes = np.linalg.solve(mat, right_end[:, -1] - left_end[:, -1])
        self._left_amplitudes = np.append(np.linalg.solve(r_left, amplitudes[:2]), 1)
        right_amplitudes = np.append(np.linalg.solve(r_right, amplitudes[2:]), 1)
        self._right_amplitudes = right_amplitudes
        self._right_initial = right.y[:, -1].reshape(2, 2, 3) @ right_amplitudes
        left_match = left_end @ self._left_amplitudes
        right_match = right_end @ right_amplitudes
        self.match_error = float(np.linalg.norm(left_match - right_match))

    def prepare_right(self, rmax):
        """Pre-integrate a dense real outgoing leg, without extending the ray."""
        rmax = float(rmax)
        start = self._real_bounds[-1] if self._real_bounds else self.rmatch
        if rmax <= start:
            return
        initial = self._real_solutions[-1].y[:, -1] if self._real_solutions else self._right_initial

        def rhs(r, vector):
            return self._rhs(r, 1 / (1 - 1 / r), vector, left=False)

        sol = self._integrate(rhs, (start, rmax), initial, dense=True)
        self._real_solutions.append(sol)
        self._real_bounds.append(rmax)

    def __call__(self, r):
        r = float(r)
        if r < self.left_r - 4 * np.finfo(float).eps:
            raise ValueError("requested radius is below the truncated horizon endpoint")
        if r <= self.rmatch:
            y = math.log(max(r - 1, self.left_delta))
            physical = self._unfactor(r, self._left.sol(y), left=True) @ self._left_amplitudes
        else:
            self.prepare_right(r)
            sol = self._real_solutions[bisect.bisect_left(self._real_bounds, r)]
            physical = self._unfactor(r, sol.sol(r), left=False)[:, 0]
        return self._fields(r, physical)

    def ray_fields(self, distance):
        """Return (At, Ax, Ftx, Az) at r=rmatch+ray*distance."""
        distance = float(distance)
        if not 0 <= distance <= self.length:
            raise ValueError("distance outside the solved outgoing ray")
        r = self.rmatch + self.ray * distance
        physical = self._unfactor(r, self._right_ray.sol(distance), left=False)
        return self._fields(r, physical @ self._right_amplitudes)

    def _fields(self, r, physical):
        y, z, j_y, j_z = physical
        f, _, root_D, root_W, a, b, _, _ = self._background(r)
        gamma = 1j * self.k * r / SQRT_LAMBDA
        u_flux = root_D * y
        a_t = -(j_y + a * y + gamma * self.s * z) / root_D
        b_flux = root_W * (j_z - b * z + gamma * self.s * y)
        x_tx, _, y_x = self.source(r)
        a_x = (-self.s * u_flux - 1j * self.k * b_flux - y_x) / (LAMBDA * r**2)
        f_tx = f * (u_flux + x_tx) / r**4
        a_z = z / root_W
        return a_t, a_x, f_tx, a_z


def manufactured_check():
    """Compact exact field on the inner real leg, zero outside its support."""
    from numpy.polynomial import Polynomial

    s = -0.24120619 - 0.747343j
    k = 2 * math.pi / 10
    center = 1.8
    half_width = 0.35
    profile = Polynomial([1, 0, -1])**6
    derivs = [profile.deriv(n) for n in range(4)]

    def data(r):
        if abs(complex(r).imag) > 0 or abs(r.real - center) >= half_width:
            return (0j, 0j, 0j), (0j, 0j, 0j, 0j)
        r = r.real
        y = (r - center) / half_width
        p, p_r, p_rr, p_rrr = [deriv(y) / half_width**n for n, deriv in enumerate(derivs)]
        f = 1 - 1 / r
        f_r = 1 / r**2
        g = (r**-4 - r**-5) / s
        g_r = (-4 * r**-5 + 5 * r**-6) / s
        g_rr = (20 * r**-6 - 30 * r**-7) / s
        a_t = g * p_r
        a_x = p / r**4
        f_tx = s * a_x - f * (g_r * p_r + g * p_rr)
        u = r**4 * f_tx / f
        u_x = f * (
            s * p_r / f - s * p * f_r / f**2
            - 4 * r**3 * (g_r * p_r + g * p_rr)
            - r**4 * (g_rr * p_r + 2 * g_r * p_rr + g * p_rrr)
        )
        y_t = u_x + r**2 * (LAMBDA + k**2 * r**2) * a_t
        y_x = -s * u - k**2 * p - LAMBDA * r**2 * a_x
        return (0j, y_t, y_x), (a_t, a_x, f_tx, 0j)

    for left_delta, length, tol in ((1e-4, 48, 2e-10), (1e-6, 96, 2e-12)):
        sol = MixedSolution(lambda r: data(r)[0], s, k, 2.5, math.pi/4, length, left_delta, tol)
        sol.prepare_right(12)
        points = np.linspace(1.15, 12, 401)
        err = max(np.linalg.norm(np.asarray(sol(r)) - data(r)[1]) for r in points)
        print(f"delta={left_delta:g} length={length:g} tol={tol:g} "
              f"max_error={err:.6g} sigma_min={sol.sigma_min:.10f} "
              f"match_error={sol.match_error:.3g} nfev={sol.nfev}")
        if err > 1000 * tol:
            raise AssertionError("compact manufactured field failed")


def endpoint_check():
    """Analytic forced example with nonzero horizon data and ray tail."""
    s = -0.24120619 - 0.747343j
    k = 2 * math.pi / 10
    beta = 2.0

    def data(r):
        f = 1 - 1 / r
        f_r = 1 / r**2
        f_x = f * f_r
        f_r_x = -2 * f / r**3
        p = cmath.exp(s * tortoise(r) - beta * (r - 1))
        ell = s - beta * f
        ell_x = -beta * f_x
        x_tx = 0.3 * p
        o_coeff = 2 * beta - beta**2 * f / s + beta * f_r / s + 4 * ell / (s * r)
        o_coeff_x = (
            -beta**2 * f_x / s + beta * f_r_x / s
            + 4 * (ell_x / r - ell * f / r**2) / s
        )
        u = p * (o_coeff - 0.3)
        u_x = ell * u + p * o_coeff_x
        a_t = ell * p / (s * r**4)
        a_x = p / r**4
        f_tx = f * p * o_coeff / r**4
        y_t = u_x + r**2 * (LAMBDA + k**2 * r**2) * a_t
        y_x = -s * u - k**2 * p - LAMBDA * r**2 * a_x
        return (x_tx, y_t, y_x), (a_t, a_x, f_tx, 0j)

    ray = cmath.exp(1j * math.pi / 4)
    for left_delta, length, tol in ((1e-4, 48, 2e-10), (1e-6, 96, 2e-12)):
        sol = MixedSolution(lambda r: data(r)[0], s, k, 2.5, ray, length, left_delta, tol)
        sol.prepare_right(12)
        real_points = np.geomspace(0.001, 11.0, 401) + 1
        ray_points = np.linspace(0, 20, 81)
        real_err = max(np.linalg.norm(np.asarray(sol(r)) - data(r)[1]) for r in real_points)
        ray_err = max(np.linalg.norm(
            np.asarray(sol.ray_fields(distance)) - data(2.5 + ray * distance)[1]
        ) for distance in ray_points)
        print(f"analytic endpoints: delta={left_delta:g} length={length:g} tol={tol:g} "
              f"real_max_error={real_err:.6g} ray_max_error={ray_err:.6g}")
        if max(real_err, ray_err) > 10_000 * left_delta**1.45 + 1000 * tol:
            raise AssertionError("analytic endpoint test failed")


if __name__ == "__main__":
    manufactured_check()
    endpoint_check()
