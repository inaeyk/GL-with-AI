"""Non-production empirical shifted-baseline Laurent coefficient, r0=1."""

import argparse
import platform
import sys
from pathlib import Path

import numpy as np
import scipy

SOURCE = Path(__file__).resolve().parents[2] / "GLR-GOAL-011/checks/physical_overlap"
sys.path.insert(0, str(SOURCE))

from overlap_gl import GLProfile, integrate, tortoise
from overlap_mean import MeanGeometry
from overlap_mixed import MixedSolution
from overlap_projection import strong, strong_piece
from overlap_rw import Baseline, Jost, potential, pole
from returning_overlap import Experiment, LABELS


class IngoingTest(Jost):
    """Carry the unit ingoing solution across the match, including off pole."""

    def __init__(self, s, rmatch, ray, length, left_delta, tol):
        super().__init__(s, rmatch, ray, length, left_delta, tol)

        def equation(distance, state):
            r = rmatch + ray * distance
            return ray / (1 - 1 / r) * np.array(
                [state[1], potential(r) * state[0] - 2 * s * state[1]]
            )

        self.ingoing_ray = integrate(
            equation, (0, length), self.left_factored(rmatch), tol
        )

    def mode(self, r):
        if np.imag(r) == 0 and r.real <= self.rmatch:
            return self.left(r.real)
        distance = ((r - self.rmatch) / self.ray).real
        m, mx = self.ingoing_ray.sol(distance)
        return np.exp(self.s * tortoise(r)) * np.array([m, mx + self.s * m])


class BaselineResidue:
    """Normalized outgoing baseline residue with its regular ingoing collar."""

    def __init__(self, packet, derivative):
        self.jost = packet.jost
        self.inner, self.radius = packet.inner, packet.radius
        self.amplitude = -packet.full[0] / derivative

    def fields(self, r):
        qnm = self.jost
        f = 1 - 1 / r
        if np.imag(r) == 0 and r.real <= qnm.rmatch:
            m, mx = qnm.left_factored(r.real)
            factor = self.amplitude / qnm.ray_scale * np.exp(qnm.s * tortoise(r))
            shear = factor * m
            dx_shear = factor * (qnm.s * m + mx)
            ar = -shear / (2 * r**2) - factor * mx / (2 * r * f)
        else:
            shear, dx_shear = self.amplitude * qnm.right(r)
            ar = (qnm.s * shear / (2 * r) - (f * shear + r * dx_shear) / (2 * r**2)) / f
        av = (f * shear + r * dx_shear) / (2 * r**2)
        q = 2 * r * shear
        return q, av, ar, -q / r**4


class ShiftedTransfer(Experiment):
    """Reuse the pinned projected currents with the correct off-pole test."""

    def __init__(self, length, left_delta, tol, angle, offset=None):
        self.rmatch = 2.5
        self.ray = np.exp(1j * angle)
        self.length = length
        self.left = 1 + left_delta
        self.tol = tol
        args = (self.rmatch, self.ray, length, left_delta, tol)
        self.gl = GLProfile(*args)
        self.qnm = pole(*args)
        self.s = self.qnm + 2 * self.gl.omega
        if offset is not None:
            self.s += offset
        self.test = IngoingTest(self.s, *args)
        if offset is None:
            step = 1e-5
            derivative = (Jost(self.qnm + step, *args).w - Jost(self.qnm - step, *args).w) / (2 * step)
            packet = Baseline(Jost(self.qnm, *args, extend=True), 13 / 8, .12, tol)
            self.baseline = BaselineResidue(packet, derivative)
        else:
            self.baseline = Baseline(
                Jost(self.s - 2 * self.gl.omega, *args, extend=True), 13 / 8, .12, tol
            )
        self.mean = MeanGeometry(self.gl, self.rmatch, self.ray, length, tol)
        self.mixed = MixedSolution(self.mixed_source, self.s - self.gl.omega, self.gl.k, *args)



def strong_with_short_cells(experiment, cap):
    """Avoid evaluating a long-interval polynomial against a growing test."""
    collar, _ = experiment.collar(cap, 6)
    edges = [1 + cap, 1.045, 1.13, 1.3, 1.48, experiment.baseline.inner,
             experiment.baseline.radius, 1.85, 2.15, experiment.rmatch]
    compact = sum(strong_piece(experiment, a, b, lambda r: r,
                               experiment.regular_current, 36)
                  for a, b in zip(edges, edges[1:]))
    ray_edges = np.linspace(0, experiment.length, int(np.ceil(experiment.length / 2)) + 1)

    def radius(distance):
        return experiment.rmatch + experiment.ray * distance

    outer = sum(strong_piece(experiment, a, b, radius,
                              lambda q: experiment.regular_current(radius(q), q), 20)
                for a, b in zip(ray_edges, ray_edges[1:]))
    return collar + compact + outer


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--length", type=float, default=64)
    parser.add_argument("--left-delta", type=float, default=1e-6)
    parser.add_argument("--tol", type=float, default=2e-11)
    parser.add_argument("--angle", type=float, default=45, help="Ray angle in degrees")
    parser.add_argument("--cap", type=float, default=.008)
    parser.add_argument("--strong", action="store_true")
    parser.add_argument("--wide-strong-cells", action="store_true",
                        help="Reproduce the failed far-ray interpolation control")
    parser.add_argument("--offset", type=float, help="Check symmetric full-transform samples")
    args = parser.parse_args()
    setup = (args.length, args.left_delta, args.tol, np.deg2rad(args.angle))
    print(f"Python {platform.python_version()}, NumPy {np.__version__}, SciPy {scipy.__version__}")
    print(args, flush=True)
    experiment = ShiftedTransfer(*setup)
    numerator, _, ray = experiment.evaluate(args.cap, 6)
    print("s_q", experiment.qnm, "s_*", experiment.s, "Omega", experiment.gl.omega)
    print("baseline amplitude", experiment.baseline.amplitude, "returning W", experiment.test.w)
    print("massive match sigma_min", experiment.mixed.sigma_min)
    print("outer weak endpoint", np.linalg.norm(experiment.weak(
        experiment.rmatch + experiment.ray * experiment.length, experiment.length
    )))
    print("ray numerator", ray)
    for label, contribution in zip(LABELS, numerator / experiment.test.w):
        print(label, contribution)
    print("weak shifted amplitude", sum(numerator) / experiment.test.w, flush=True)
    if args.strong:
        cap = max(args.cap, .011)
        values = (strong(experiment, cap, 36) if args.wide_strong_cells
                  else strong_with_short_cells(experiment, cap))
        print("strong shifted amplitude", sum(values) / experiment.test.w, flush=True)
    if args.offset:
        values = []
        for offset in (args.offset, -args.offset, args.offset / 2, -args.offset / 2):
            sample = ShiftedTransfer(*setup, offset=offset)
            value = sum(sample.evaluate(args.cap, 6)[0]) / sample.test.w
            values.append(offset * value)
            print("offset", offset, "offset times full transform", values[-1], flush=True)
        print("symmetric sample pairs", (values[0] + values[1]) / 2, (values[2] + values[3]) / 2)


if __name__ == "__main__":
    main()
