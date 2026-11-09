"""Recompute separation margins from archived directed text using rational arithmetic."""
from fractions import Fraction
import json
from pathlib import Path
import re

CHECKS = Path(__file__).resolve().parents[2] / "GLR-GOAL-016/transport/checks"


def parse_ball(ball):
    match = re.fullmatch(r"\[([^ ]+) \+/- ([^ ]+)\]", ball)
    if match:
        midpoint, radius = map(Fraction, match.groups())
        return midpoint - radius, midpoint + radius
    return Fraction(ball), Fraction(ball)


def read_json(filename):
    return json.loads((CHECKS / filename).read_text())


def flux_intervals(filename):
    records = read_json(filename)["radii"][0]["values"]
    assert [entry["r"] for entry in records] == [records[0]["r"]] * 3
    result = []
    for entry in records:
        bounds = entry["schwarzschild_signed_flux"]
        result.append((parse_ball(bounds["lower"])[0], parse_ball(bounds["upper"])[1]))
    return result


def gap(left, right):
    return left[0] - right[1]


def check_lower(label, value, floor):
    assert value > Fraction(floor), (label, value, floor)
    print(f"{label}: > {floor}")


r24 = flux_intervals("tube24.json")
r48 = flux_intervals("tube48.json")
for radius, rows, floors in ((24, r24, ("4.89321e-9", "6.08054e-8")),
                             (48, r48, ("5.84475e-8", "8.35627e-8"))):
    for i, floor in enumerate(floors):
        check_lower(f"R={radius}, adjacent mass gap {i+1}", gap(rows[i], rows[i+1]), floor)
    assert all(lo > 0 for lo, _ in rows)

check_lower("mu=.10, R24 minus R48", gap(r24[1], r48[1]), "8.59866e-9")
check_lower("mu=.20, R24 minus R48", gap(r24[2], r48[2]), "1.07902e-8")
assert gap(flux_intervals("tube24_first_pass.json")[0],
           flux_intervals("tube24_first_pass.json")[1]) < 0
coarse48 = flux_intervals("tube48_coarse_failure.json")
assert gap(coarse48[0], coarse48[1]) < 0 and gap(coarse48[1], coarse48[2]) < 0
print("Archived first-pass adjacent comparisons retain overlap")

band = read_json("completed_band.json")
channels = band["outgoing_or_horizon_normalized_band_energy"]
pulsed_low = parse_ball(channels["baseline_plus_pulse"]["lower"])[0]
baseline_band_hi = parse_ball(channels["baseline"]["upper"])[1]
baseline_total_hi = parse_ball(
    band["coarse_total_work_energy_upper_either_channel"]["baseline"]
)[1]
check_lower("Pulsed band minus baseline band, each channel",
            pulsed_low - baseline_band_hi, "0.0335219")
check_lower("Pulsed band minus baseline all-band work ceiling, each channel",
            pulsed_low - baseline_total_hi, "0.0059839")

# The displayed outward lower for n_Z determines a positive finite-seed threshold
# only once the nonlinear remainder constant and common physical window exist.
assert Fraction("0.000420214")**2 < Fraction("3.5316e-7") / 2
assert 2 * Fraction("7.692e-6")**2 < Fraction("1.088e-5")**2
print("Conditional vacuum seed threshold and free-source L2 budget checked")
