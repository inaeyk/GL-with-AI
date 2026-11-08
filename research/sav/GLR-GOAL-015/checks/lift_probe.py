"""Directed sample of whole-cell defects of the exact mixed lift.

Each reported region covers its complete cell or terminal subray. This
probe does not sum the full physical mixed error on the early strip.
"""

import argparse
from pathlib import Path
import platform
import sys

import flint
from flint import arb, ctx

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "source"))
from source_geometry import GeometryEnclosure
from mixed_lift import MixedLift, affine_range
from ordered_maps import physical_source, residual, return_current
from weak_candidate import array, radial


def display(values):
    return [(q.lower().str(9), q.upper().str(9)) for q in values]


def run(mixed_file, current_file, bits):
    ctx.prec, ctx.cap = bits, 80
    geo = GeometryEnclosure(refinement=2)
    lift = MixedLift(mixed_file, max_v=arb(10))
    data = array(current_file)
    delta = arb(3)/25
    i = lift.n-1
    probes = [
        ("terminal start point", 0, arb(1), arb(0)),
        ("terminal first full v-panel", 0, arb(1), arb(0).union(arb(1))),
        ("last full u-panel, first v-panel", 0, arb(0).union(arb(1)), arb(0).union(arb(1))),
        ("last u-panel, first v-panel, endpoint-side 1/8 × 1/8 subcell",
         0, (arb(7)/8).union(arb(1)), arb(0).union(arb(1)/8)),
        ("terminal v-panel 30", 30, arb(1), arb(0).union(arb(1))),
        ("last u-panel, v-panel 30", 30, arb(0).union(arb(1)), arb(0).union(arb(1))),
    ]
    print(f"Python {platform.python_version()} python-flint {flint.__version__}; {bits} bits")
    for name, j, t, s in probes:
        u = lift.a+lift.h*(i+t)
        v = lift.v[j]+lift.dv[j]*s
        r = radial(u, v)
        source, back, potential = physical_source((u-lift.a)/delta, v, r, geo)
        Y = lift.rectangle(i, j, t, s)
        cur = return_current(r, geo.k, Y, back, potential)
        target = []
        col = lift.n+j
        for k in "FGH":
            to_arb = lambda row, c: arb(float(data[k][row, c]))
            left = affine_range(to_arb(i, col), to_arb(i+1, col), t)
            right = affine_range(to_arb(i, col+1), to_arb(i+1, col+1), t)
            target.append(affine_range(left, right, s))
        print(name)
        print("  r", display((r,)), "R_y,R_Z", display(residual(Y, source, potential)))
        print("  map-on-lift minus independent current candidate", display(
            tuple(a-b for a, b in zip(cur, target))))
    print("Whole-strip residual norms and propagated physical mismatch remain open.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("mixed_lift")
    parser.add_argument("current_candidate")
    parser.add_argument("--bits", type=int, default=160)
    args = parser.parse_args()
    run(args.mixed_lift, args.current_candidate, args.bits)
