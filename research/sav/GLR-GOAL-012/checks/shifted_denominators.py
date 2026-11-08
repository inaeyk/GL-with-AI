"""Outward-rounded local noncoincidence checks at the shifted RW fundamental."""

import platform
import sys
from pathlib import Path

import flint
import numpy as np
import scipy
from flint import arb

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-011/checks"))

import spectral_certificate as pinned


def main():
    print("Python", platform.python_version(), "python-flint", flint.__version__,
          "NumPy", np.__version__, "SciPy", scipy.__version__, flush=True)
    pole_box = pinned.shift(pinned.CENTER, arb("5e-8"))
    shift = pole_box + 2 * pinned.OMEGA
    print("GOAL-011 pole disk enclosing box", pole_box, "shifted box", shift, flush=True)
    print("Cells per side", pinned.CELLS, "inner offset", pinned.INNER,
          "ray length", pinned.OUTER, "ray angle pi/4", flush=True)
    returning = pinned.rw_determinant(shift)
    print("returning logarithmic derivative mismatch", returning, flush=True)
    assert returning.imag < arb("-0.39"), returning
    matrix, _ = pinned.matching(pole_box + pinned.OMEGA, massive=True)
    determinant = matrix[0] * matrix[3] - matrix[1] * matrix[2]
    print("massive determinant", determinant, flush=True)
    assert determinant.abs_lower() > 0, determinant
    print("PASS noncoincidence for the full pinned pole and growth boxes")


if __name__ == "__main__":
    main()
