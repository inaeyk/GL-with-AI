"""Independently reduce archived directed numbers to exact rational margins."""
from fractions import Fraction
from pathlib import Path
import re

SOURCE = Path(__file__).resolve().parents[2] / "GLR-GOAL-017"


def named_bounds(text, label):
    line = next(line for line in text.splitlines() if line.startswith(label + ":"))
    endpoints = re.findall(r"\[([^\[\]]+?) \+/- ([^\[\]]+?)\]", line)
    assert endpoints
    left, right = tuple(map(Fraction, endpoints[0])), tuple(map(Fraction, endpoints[-1]))
    return left[0] - left[1], right[0] + right[1]


baseline = (SOURCE / "spectral/baseline.txt").read_text()
gates = (SOURCE / "checks/RESULTS.txt").read_text()
base_lower, base_upper = named_bounds(baseline, "exact norm")
_, energy_error = named_bounds(baseline, "energy error upper")
_, pole_upper = named_bounds(baseline, "baseline ordinary pole pair norm")
mismatch_low, mismatch_hi = named_bounds(baseline, "exact baseline ordinary pole relative mismatch")
assert base_lower > Fraction(".00501844") and base_upper < Fraction(".00509641")
assert energy_error < Fraction("3.898e-5")
assert mismatch_low > Fraction("2.02612") and mismatch_hi < Fraction("2.16772")
# This independent rejection dispenses with the cross integral and its phase.
assert base_lower / pole_upper - 1 > Fraction("1.9297")
print("Baseline rejects its pole even from the reverse triangle bound > 1.9297")

for template, floor in (("Q", ".31731"), ("Q+S*", ".80484")):
    lower, _ = named_bounds(gates, template + " subbox direct rejection threshold")
    assert lower > Fraction(floor)
    print(f"{template}: strict rejection if physical E21 <= {floor}")

candidate_lower, candidate_upper = named_bounds(gates, "candidate returning norm")
assert candidate_lower > Fraction(".08741378900667")
assert candidate_upper < Fraction(".08741378900668")
assert candidate_lower / Fraction(".00509641") > Fraction("17.1520")
assert candidate_upper / Fraction(".00501844") < Fraction("17.4186")
print("Candidate-only relative ordered change is within [17.1520, 17.4186] epsilon^2")
print("Physical N21 nonzero from this candidate if E21 < .08741378900667")
