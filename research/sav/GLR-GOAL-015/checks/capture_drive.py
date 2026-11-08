"""Capture nodal inputs for a conforming mixed-field lift from GOAL-014.

This wraps the pinned source at runtime and keeps its implementation intact.
The output is temporary binary data whose entries define exact ordinates.
"""

import argparse
from pathlib import Path
import sys

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-014/checks"))
import c_compact as pilot


def capture(n, m, cap, tol, outer, filename):
    original_march = pilot.Strip.march
    original_graph = pilot.point_velocities
    fields = {}

    def march(strip, q, pu, pv, potential, source, iterations, skew=None):
        output = original_march(strip, q, pu, pv, potential, source, iterations, skew)
        if skew is not None:
            field, p, qv = output
            skew_term = np.stack((-skew*(p+qv)[..., 1], skew*(p+qv)[..., 0]), axis=2)
            fields["D"] = np.where(strip.mask[..., None],
                                   (-potential*field - source + skew_term)/4, 0)
            fields.update(u=strip.u, v=strip.v, Y=field, P=p, Q=qv)
        return output

    def graph_velocities(gl, n):
        velocities = original_graph(gl, n)
        fields["raw_graph"] = velocities
        return velocities

    try:
        pilot.Strip.march = march
        pilot.point_velocities = graph_velocities
        pilot.calculate(n, m, cap, 12, "simpson", tol, outer, None)
    finally:
        pilot.Strip.march = original_march
        pilot.point_velocities = original_graph
    assert np.all(fields["D"][0] == 0)
    assert np.all(fields["raw_graph"][0] == 0)
    assert np.isfinite(fields["D"]).all()
    np.savez(filename, **fields)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output")
    parser.add_argument("--n", type=int, default=192)
    parser.add_argument("--m", type=int, default=720)
    parser.add_argument("--cap", type=float, default=400)
    parser.add_argument("--tol", type=float, default=8e-14)
    parser.add_argument("--outer", type=float, default=430)
    args = parser.parse_args()
    capture(args.n, args.m, args.cap, args.tol, args.outer, args.output)
