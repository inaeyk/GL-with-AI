"""Validate the RW threshold noncoincidence and translated baseline moment."""
from pathlib import Path
import platform
import sys

import flint
import numpy
import scipy
from flint import acb, arb, ctx

source_checks = Path(__file__).resolve().parents[2] / "GLR-GOAL-011" / "checks"
sys.path.insert(0, str(source_checks))
sys.path.insert(0, str(source_checks / "physical_overlap"))
import certified_launch as launch
import spectral_certificate as spec


def main():
    print("Python", platform.python_version(), "NumPy", numpy.__version__,
          "SciPy", scipy.__version__, "python-flint", flint.__version__)
    ctx.prec = 128
    ctx.cap = 80
    omega = acb(spec.OMEGA)
    threshold_d = spec.rw_determinant(-omega - spec.I * spec.K)
    assert threshold_d.real > arb(".4783")
    print("threshold baseline D", threshold_d)

    s = spec.shift(spec.CENTER, arb("5e-8"))
    left, radius, lower = arb("1.125"), arb(13) / 8, arb("1.595")
    n, nr = [series.value(acb(left - 1)) for series in launch.rw_horizon(s, 40)]
    exp_sx = (s * (left + (left - 1).log())).exp()
    operator = launch.massless(s)
    initial = [exp_sx * n, exp_sx * (s * n + (1 - 1 / left) * nr)]
    state = launch.transport(operator, left, radius, initial, 40)
    step = (lower - radius) / 4
    for j in range(4):
        start = radius + j * step
        local = launch.advance(operator, start, step, state)
        t = acb(step * arb(".5", ".5"))
        r = acb(start) + t
        h, hx = [series.value(t) for series in local]
        kernel = (s * (r - 1).log()).exp() * (hx + s * h)
        assert kernel.real > arb(".44")
        print("packet cell", j, "moment kernel", kernel)
        state = [series.value(acb(step)) for series in local]

    u_width = (-lower - 2 * (lower - 1).log()) - (-radius - 2 * (radius - 1).log())
    assert u_width > arb(".12")
    print("covered retarded packet width", u_width)


if __name__ == "__main__":
    main()
