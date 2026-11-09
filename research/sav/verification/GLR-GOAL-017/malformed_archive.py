"""Reproduce two fail-open controls for the pinned GOAL-017 candidate gate."""

import argparse
from pathlib import Path
from subprocess import run
import sys
from tempfile import TemporaryDirectory

import numpy as np


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source_gate", type=Path, help="pinned checks/candidate_gates.py")
    parser.add_argument("candidate", type=Path, help="original clustered candidate NPZ")
    args = parser.parse_args()
    with np.load(args.candidate, allow_pickle=False) as original:
        archive = {key: original[key] for key in original.files}

    with TemporaryDirectory(prefix="glr017-malformed-") as directory:
        folded = Path(directory) / "folded-u.npz"
        nonfinite = Path(directory) / "nonfinite-candidate.npz"
        archive["u"][1] *= -1
        np.savez(folded, **archive)
        archive["u"][1] *= -1
        late = np.searchsorted(archive["u_full"], 0.0)
        archive["p_shear_infty"][late+1] = np.nan
        np.savez(nonfinite, **archive)

        for path in (folded, nonfinite):
            result = run(
                [sys.executable, "-B", str(args.source_gate), str(path), "--sharpen"],
                capture_output=True, text=True, timeout=30, check=False,
            )
            print(f"{path.stem} source gate exit={result.returncode}")
            lines = result.stdout.splitlines()
            for line in lines:
                if "candidate returning norm" in line or "subbox direct rejection" in line:
                    print(line)
            if result.returncode != 0:
                continue
            if path == nonfinite:
                assert sum("100.0000000000000, 100.0000000000000" in line for line in lines) == 2
            else:
                assert sum("subbox direct rejection threshold" in line for line in lines) == 2


if __name__ == "__main__":
    main()
