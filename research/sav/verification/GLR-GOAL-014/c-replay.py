"""Replay the pinned GOAL-014 C checks from research/sav (fixed background)."""

from fractions import Fraction
from hashlib import sha256
from os import environ
from pathlib import Path
import subprocess
import sys
from tempfile import TemporaryDirectory

import flint
import numpy as np
import scipy
import sympy


def main():
    checks = Path("GLR-GOAL-014/checks")
    recorded = (checks / "c_results.txt").read_text().splitlines()[2:]
    recorded = [line for line in recorded
                if "RuntimeWarning" not in line and "return np.abs(h)" not in line]
    env = dict(environ, PYTHONDONTWRITEBYTECODE="1", OPENBLAS_NUM_THREADS="1")
    print("Python, python-flint, NumPy, SciPy, SymPy:",
          sys.version.split()[0], flint.__version__, np.__version__,
          scipy.__version__, sympy.__version__)

    all_lines = []
    warnings = []
    with TemporaryDirectory(prefix="glr014-c-replay-") as scratch:
        cap = str(Path(scratch) / "cap.npz")
        runs = (
            ("coarse", "c_compact.py", "--n", "48", "--m", "180", "--cap", "40", "--outer", "180"),
            ("medium", "c_compact.py", "--n", "96", "--m", "360", "--cap", "80", "--outer", "200"),
            ("fine", "c_compact.py", "--n", "192", "--m", "720", "--cap", "400", "--outer", "430",
             "--tol", "8e-14", "--dump", cap),
            ("cap160", "c_cap_gate.py", cap),
            ("cap192", "c_cap_gate.py", cap, "--bits", "192"),
            ("graph160", "c_graph_residual.py"),
            ("graph192", "c_graph_residual.py", "--bits", "192"),
        )
        for name, script, *options in runs:
            output = subprocess.run(
                [sys.executable, str(checks / script), *options],
                env=env, capture_output=True, text=True, check=True,
            )
            lines = output.stdout.splitlines()
            all_lines.extend(lines)
            for warning in output.stderr.splitlines():
                if "RuntimeWarning" in warning:
                    warnings.append((name, warning.split("site-packages/")[-1]))
            print(f"{name}: exit 0, {len(lines)} stdout lines")
        with np.load(cap, allow_pickle=False) as data:
            p_cap = np.asarray(data["p_cap"], dtype="<f8")
            print("SHA-256 of little-endian float64 p_cap ordinates:",
                  sha256(p_cap.tobytes()).hexdigest())

    for name, warning in warnings:
        print(f"{name}: {warning}")
    print("Archived stdout lines reproduced:", len(all_lines))
    assert recorded == all_lines
    print("Archived stdout SHA-256:", sha256(("\n".join(all_lines) + "\n").encode()).hexdigest())

    # Rational upper bound, independently avoiding Arb exponentiation.
    q = Fraction
    width, radius = q(3, 200), q(400)
    alpha_upper = 2 * width * (3 / radius - 3 / (4 * radius**2)) / q("3.14159265358979")
    amplification_upper = 1 / (1 - alpha_upper)  # exp(x) <= 1/(1-x), 0 <= x < 1
    cap_norm, baseline_error = q("635.49461516"), q("0.0000371512")
    baseline_norm, tail = q("0.01364715"), q("0.000001701")
    fixed = baseline_error * cap_norm + baseline_norm * (
        (amplification_upper - 1) * cap_norm + tail
    )
    coefficient = baseline_norm * amplification_upper
    assert fixed < q("0.024231")
    assert coefficient < q("0.013648127")
    margin = q("7.80653") - q("0.024231") - q("0.013648127") * 570
    assert margin == q("0.00286661")
    print("Rounded budget at E_c=570 gives strictly positive margin 0.00286661.")


if __name__ == "__main__":
    main()
