"""Recompute GOAL-016 certified decision margins from saved endpoint balls."""

import argparse
from decimal import Decimal, getcontext
import json
from pathlib import Path
import re

getcontext().prec = 80
NUMBER = r"[-+]?(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][-+]?[0-9]+)?"
BALL = re.compile(rf"\[?({NUMBER})(?:\s+\+/-\s+({NUMBER}))?\]?")


def ball(string):
    match = BALL.fullmatch(string)
    if match is None:
        raise ValueError(string)
    center = Decimal(match[1])
    radius = Decimal(match[2] or "0")
    return center - radius, center + radius


def box(data):
    return ball(data["lower"])[0], ball(data["upper"])[1]


def load_tube(path):
    result = json.loads(path.read_text())["radii"][0]
    return [box(row["schwarzschild_signed_flux"]) for row in result["values"]]


def gap(left, right):
    return left[0] - right[1]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source_checks", type=Path)
    args = parser.parse_args()
    d = args.source_checks
    rows = {R: load_tube(d / f"tube{R}.json") for R in (24, 48)}
    for R, tube in rows.items():
        assert all(lower > 0 for lower, _ in tube)
        g0, g1 = (gap(tube[i], tube[i+1]) for i in range(2))
        print(f"R={R}: adjacent gap lower bounds {g0:.12g} {g1:.12g}")
        assert min(g0, g1) > 0
    for i in (1, 2):
        g = gap(rows[24][i], rows[48][i])
        print(f"mass index {i}: R24-R48 lower bound {g:.12g}")
        assert g > 0
    for filename in ("tube48_coarse_failure.json", "tube24_first_pass.json"):
        tube = load_tube(d / filename)
        g0, g1 = (gap(tube[i], tube[i+1]) for i in range(2))
        print(f"{filename}: adjacent gap lower bounds {g0:.12g} {g1:.12g}")
        assert g0 < 0
    report = json.loads((d / "completed_band.json").read_text())
    band = report["outgoing_or_horizon_normalized_band_energy"]
    p_min = box(band["baseline_plus_pulse"])[0]
    b_band_max = box(band["baseline"])[1]
    b_all_max = ball(report["coarse_total_work_energy_upper_either_channel"]["baseline"])[1]
    print(f"pulsed minus baseline band margin > {p_min - b_band_max:.12g}")
    print(f"pulsed minus baseline all-band margin > {p_min - b_all_max:.12g}")
    assert p_min - b_band_max > Decimal("0.0335219")
    assert p_min - b_all_max > Decimal("0.0059839")


if __name__ == "__main__":
    main()
