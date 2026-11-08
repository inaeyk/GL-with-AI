"""Directed real-axis GL jet and fixed-infinity mean on 1.59 <= r <= 401.

The normalized physical eigenmode and its growth bracket are inherited.
Float64 interpolants propose the Riccati cells. All error and quadrature
bounds are whole-cell Arb inclusions. Radii are measured in r_0 units.
"""
from bisect import bisect_left, bisect_right
from dataclasses import dataclass
from math import hypot, pi
from pathlib import Path
from types import SimpleNamespace
import sys

from flint import acb, arb, arb_poly, arb_series, ctx
import numpy as np
from scipy.integrate import solve_ivp

RESEARCH = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(RESEARCH / "GLR-GOAL-011/checks/physical_overlap"))
from certified_launch import gl_scale
from exterior_profiles import CompactGL, gl_coefficients
from mean_certificate import mean_rhs
from mean_flux import local_mass
from outer_gljet import njet

UNIT = arb("0.5", "0.5")


def symmetric(radius):
    return arb(0, radius.abs_upper())


def x(r):
    return r + (r - 1).log()


def potential(r, k):
    f = 1 - 1 / r
    denominator = 1 + k * k * r**3
    return f * (k * k + ((6*r - 11) + 18*(r - 1)/denominator**2
                          - 6*(4*r - 5)/denominator) / r**3)


def endpoint_radius(r, k, q, alpha):
    """Bound |p+q-alpha*f/r| on the real decaying branch at r."""
    attenuation = q - alpha / r
    assert attenuation > 0
    denominator = 1 + k*k*r**3
    force = (k*k + 6 + 11/r + alpha**2 + alpha*(1 + 2/r)
             + 18/denominator**2 + 6*(4 + 5/r)/denominator) / r**2
    kernel = 1 / (2*attenuation)
    radius = (2*kernel*force).upper()
    assert kernel*radius**2 < radius / 2
    assert 2*kernel*radius < 1
    return radius


@dataclass
class RightCell:
    """Descending radial candidate with exact binary abscissae and ordinates."""

    start: arb
    step: arb
    poly: arb_poly
    error: arb
    prefix: arb = None

    def __post_init__(self):
        # dx/ds = step (1 + 1/(start-1 + step*s)). Bound the geometric remainder.
        ratio = (self.step / (self.start - 1)).abs_upper()
        assert ratio < 1
        inverse = arb_poly([(-self.step / (self.start - 1))**j /
                            (self.start - 1) for j in range(9)])
        self.primitive = (self.step * (inverse + 1) * self.poly).integral()
        self.primitive_error = (
            self.step.abs_upper() * sum(c.abs_upper() for c in self.poly.coeffs()) *
            ratio**9 / ((self.start - 1)*(1-ratio))
            + self.error * (self.step * (1 + 1/(self.radius(UNIT)-1))).abs_upper()
        ).upper()

    def radius(self, s):
        return self.start + self.step*s

    def integral(self, s):
        # Primitive from the smaller r endpoint, corresponding to s=1.
        return (self.primitive(s) - self.primitive(1)
                + symmetric(self.primitive_error * (s-1).abs_upper()))


def right_profile(grid, k, omega):
    q = (k*k+omega*omega).sqrt()
    alpha = k*k/(2*q)
    k0, om0 = pi/5, float(omega.mid())
    q0 = hypot(k0, om0)
    alpha0 = k0*k0/(2*q0)

    def rhs(radius, value):
        den, force, ell, _ = gl_coefficients(
            radius, radius-1, k0, -q0, alpha0
        )
        p = value[0]
        return [radius/(radius-1) * (force[0]/den - 2*ell*p/den - p*p)]

    sample = solve_ivp(rhs, (grid[-1], grid[0]), [0.0], t_eval=grid[::-1],
                       method="DOP853", rtol=5e-14, atol=1e-16)
    assert sample.success
    ordinates = sample.y[0, :]
    slopes = [rhs(r, [p])[0] for r, p in zip(grid[::-1], ordinates)]
    endpoint = endpoint_radius(arb(float(grid[-1])), k, q, alpha)
    error = (endpoint + abs(float(ordinates[0]))).upper()
    cells = []
    for j, (left, right) in enumerate(zip(grid[:0:-1], grid[-2::-1])):
        start = arb(float(left))
        step = arb(float(right)) - start
        lo, hi, dl, dh = [arb(float(z)) for z in
                          (ordinates[j], ordinates[j+1], slopes[j], slopes[j+1])]
        jump = hi - lo
        poly = arb_poly([lo, step*dl, 3*jump-step*(2*dl+dh),
                         -2*jump+step*(dl+dh)])
        # Clear the rational denominator as a polynomial before range evaluation.
        radius = arb_poly([start, step])
        den, force, ell, fden = gl_coefficients(radius, radius-1, k, -q, alpha)
        residual = fden*poly.derivative()*(1/step) + 2*ell*poly - force[0] + den*poly*poly
        defect = (residual(UNIT)/den(UNIT)).abs_upper()
        dx_ds = step*radius(UNIT)/(radius(UNIT)-1)
        forcing = (dx_ds.abs_upper()*defect).upper()
        linear = (-2*dx_ds*(ell(UNIT)/den(UNIT)+poly(UNIT))).upper()
        budget = (2*(error+forcing)*max(arb(1), linear.exp().upper())).upper()
        rate = (linear + dx_ds.abs_upper()*budget).upper()
        weight = rate.expm1()/rate if rate != 0 else arb(1)
        error = (rate.exp()*error + weight*forcing).upper()
        assert error < budget, (j, start, error, budget)
        cells.append(RightCell(start, step, poly, budget))
    cells.reverse()
    prefix = arb(0)
    for cell in cells:
        cell.prefix = prefix
        prefix += cell.integral(arb(0))
    return cells, q, alpha, endpoint


def metric_jets(r, n, p, k, omega):
    """Exploit exact affine dependence of the normalized EF jets on p."""
    center = p.mid()
    slope = arb_series([center, 1], 2)
    normalized = njet(r, k, omega, arb(1), (slope-omega)/(1-1/r))
    offset = p-center
    return tuple(tuple(n*(c[0]+offset*c[1]) for c in row) for row in normalized)


def mean_value(r, n, p, k, omega):
    """Exploit exact quadratic dependence of the normalized mean RHS on p."""
    center = p.mid()
    slope = arb_series([center, 1], 3)
    normalized = njet(r, k, omega, arb(1), (slope-omega)/(1-1/r))
    q = mean_rhs(r, k, omega, normalized)
    offset = p-center
    return [n*n*(c[0]+offset*c[1]+offset*offset*c[2]) for c in q]


def centered_mass(r, n, p, k, omega):
    """Evaluate the local fixed-mass constraint as a centered quadratic."""
    center = p.mid()
    slope = arb_series([center, 1], 3)
    jets = njet(r, k, omega, arb(1), (slope-omega)/(1-1/r))
    q = local_mass(r, k, omega, jets)
    offset = p-center
    return n*n*(q[0]+offset*q[1]+offset*offset*q[2])


def mean_derivatives(r, n, p, k, omega, order):
    """Mean RHS derivative jet along the physical GL radial trajectory."""
    if order == 0:
        return mean_value(r, n, p, k, omega)
    radius = arb_series([r, 1], order+1)
    slope = arb_series([p], order+1)
    profile = arb_series([n], order+1)
    f = 1 - 1/radius
    for j in range(order):
        slope[j+1] = ((potential(radius, k)+omega**2-slope*slope)/f)[j]/(j+1)
        profile[j+1] = (profile*(slope-omega)/f)[j]/(j+1)
    derivative = profile*(slope-omega)/f
    return mean_rhs(radius, k, omega, njet(radius, k, omega, profile, derivative))


class GeometryEnclosure:
    """Arb boxes for factored GL jets and fixed-infinity mean.

    evaluate(r) accepts an Arb interval and returns (h_jets, mean, mean_r).
    h_jets[j] contains the j-th radial derivatives (j=0,1,2) in the EF
    basis (vv, vr, rr, theta theta). The caller multiplies by exp(omega*v).
    mean and mean_r each contain (mu, beta), factored by exp(2*omega*v).
    """

    def __init__(self, refinement=1):
        if refinement < 1 or refinement != int(refinement):
            raise ValueError("Require a positive integer refinement")
        if ctx.prec < 128 or ctx.cap < 64:
            raise ValueError("Require Flint ctx.prec >= 128 and ctx.cap >= 64")
        self.k = arb.pi()/5
        self.omega = arb("0.06328119", "0.00000001")
        self.r_outer = arb(512)
        self.r_match = arb("2.5")
        self.x_match = x(self.r_match)
        k, omega = acb(self.k), acb(self.omega)
        scale, match = gl_scale(k, omega)
        data = SimpleNamespace(k=k, omega=omega, gl_scale=scale, gl_match=match)
        # t=r-1 has binary knots. 1.5625 is below both packet domains.
        left_grid = np.r_[np.linspace(1/8, 9/16, 256*refinement, endpoint=False),
                          np.linspace(9/16, 1.5, 1024*refinement+1)]
        left = CompactGL(data, grid=left_grid)
        self.left_profile = left
        self.n_match, left_match_slope = (z.real for z in
                                         left.evaluate(len(left.cells)-1, acb(1)))
        right_grid = np.r_[np.linspace(2.5, 12, 1024*refinement, endpoint=False),
                           np.linspace(12, 64, 256*refinement, endpoint=False),
                           np.linspace(64, 512, 448*refinement+1)]
        self.right_cells, self.q, self.alpha, self.slope_tail = right_profile(
            right_grid, self.k, self.omega
        )
        end_match_slope = self._state(0, arb(1), right=True)[1]
        assert left_match_slope.overlaps(end_match_slope)
        # Knot values are Arb exact, even when 1 + binary delta rounds in float.
        self.edges = ([arb(1)+arb(float(q)) for q in left_grid]
                      + [arb(float(q)) for q in right_grid[1:]])
        self.float_edges = [float(q) for q in self.edges]
        self.left_count = len(left.cells)
        outer = self.r_outer
        # GOAL-013 B, section 1: fixed-v integrals on the physical GL profile.
        factor = 2*10**6*(-2*self.omega*x(outer)-(outer-4)).exp()
        self.mean_tail = [symmetric(factor*(outer**2+2*outer+2)),
                          symmetric(factor*(outer+1))]
        mean = self.mean_tail
        self.mean_outer = [None]*(len(self.edges)-1)
        self.mean_cell_derivative = [None]*(len(self.edges)-1)
        # Every retained cell lies above the outermost front/packet radius.
        self.first_mean_cell = 256*refinement
        for i in range(len(self.edges)-2, self.first_mean_cell-1, -1):
            a, b = self.edges[i:i+2]
            radius = a.union(b)
            center = (a+b)/2
            h = b-a
            n, p = self._cell_state(i, radius)
            nc, pc = self._cell_state(i, center)
            value = mean_derivatives(center, nc, pc, self.k, self.omega, 0)
            jet = mean_derivatives(radius, n, p, self.k, self.omega, 2)
            # Midpoint quadrature with a whole-cell second derivative bound.
            # The Taylor series coefficient is exactly half the second derivative.
            integral = [h*z+symmetric(h**3*f[2]/12) for z, f in zip(value, jet)]
            self.mean_outer[i] = mean
            self.mean_cell_derivative[i] = [
                f[0].intersection(z+symmetric(h*f[1]/2))
                for z, f in zip(value, jet)
            ]
            mean = [right-delta for right, delta in zip(mean, integral)]

    def _state(self, j, s, right):
        if not right:
            return tuple(z.real for z in self.left_profile.evaluate(j, acb(s)))
        cell = self.right_cells[j]
        r = cell.radius(s)
        phase = ((-self.q-self.omega)*(x(r)-self.x_match)
                 + self.alpha*(r/self.r_match).log())
        n = self.n_match*(phase+cell.prefix+cell.integral(s)).exp()
        p = (-self.q+self.alpha*(1-1/r)/r + cell.poly(s)
             + symmetric(cell.error))
        return n, p

    def _cell_state(self, i, r):
        right = i >= self.left_count
        j = i-self.left_count if right else i
        if right:
            cell = self.right_cells[j]
            return self._state(j, (r-cell.start)/cell.step, right=True)
        cell = self.left_profile.cells[j]
        return self._state(j, (r-1-cell.left.real)/cell.step.real, right=False)

    def evaluate(self, r, use_local_mass=True):
        if not self.edges[self.first_mean_cell] <= r or not r <= self.r_outer:
            raise ValueError("Radius interval lies outside the validated domain")
        # Binary searches propose a conservative index set. Arb comparisons,
        # followed by intersection with exact knots, decide all cell coverage.
        first = max(self.first_mean_cell, bisect_left(self.float_edges, float(r.lower()))-2)
        last = min(len(self.edges)-1, bisect_right(self.float_edges, float(r.upper()))+1)
        assert self.edges[first] <= r.lower() and r.upper() <= self.edges[last]
        merged = None
        for i in range(first, last):
            a, b = self.edges[i:i+2]
            if r.lower() > b or r.upper() < a:
                continue
            lower, upper = max(r.lower(), a), min(r.upper(), b)
            radius = lower.union(upper)
            n, p = self._cell_state(i, radius)
            radial_center = radius.mid()
            assert a <= radial_center <= b
            nc, pc = self._cell_state(i, radial_center)
            h0 = metric_jets(radial_center, nc, pc, self.k, self.omega)
            hc = metric_jets(radius, n, p, self.k, self.omega)
            offset = (radius-radial_center).abs_upper()
            h = tuple(tuple(q.intersection(m+symmetric(offset*d))
                            for q, m, d in zip(hc[j], h0[j], hc[j+1]))
                      if j < 2 else hc[j] for j in range(3))
            cell_rhs = self.mean_cell_derivative[i]
            mean_r = [a.intersection(b) for a, b in zip(
                cell_rhs, mean_value(radius, n, p, self.k, self.omega)
            )]
            mean = [right-(b-radius)*rhs for right, rhs in zip(self.mean_outer[i], cell_rhs)]
            if use_local_mass:
                local = centered_mass(radial_center, nc, pc, self.k, self.omega)
                mean[0] = mean[0].intersection(local+symmetric(offset*cell_rhs[0]))
            boxed = tuple(q for row in h for q in row) + tuple(mean) + tuple(mean_r)
            merged = boxed if merged is None else tuple(a.union(b) for a, b in zip(merged, boxed))
        assert merged is not None and all(q.is_finite() for q in merged)
        return (tuple(tuple(merged[4*j:4*j+4]) for j in range(3)),
                tuple(merged[12:14]), tuple(merged[14:16]))
