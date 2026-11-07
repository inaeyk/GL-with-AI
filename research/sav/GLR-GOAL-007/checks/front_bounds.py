"""Exact arithmetic for the comparison constants in WITNESS-BOUNDS.md."""

from fractions import Fraction as Q

min_ratio = Q("0.55") ** 2 / Q("0.395") - Q("0.28")
max_ratio = Q("0.70") ** 2 / Q("0.394") + Q("0.28")
assert min_ratio > Q("0.48")
assert max_ratio < Q("1.53")

# exp(-x) >= 1 - x + x^2/2 - x^3/6 for x >= 0.
a = Q("0.91")
assert Q("0.9") / Q("0.99") < a
integral_lower = 1 - a / 2 + a**2 / 6 - a**3 / 24
relative_lower = Q("0.48") / 2 * Q(100, 101) * integral_lower
relative_upper = Q("1.53") / 2 / Q("0.99") / Q("0.52")
assert relative_lower > Q("0.14")
assert relative_upper < Q("1.6")

# The pulse is launched at t=0. These inherit the GOAL-005 master bounds.
assert Q("0.14") * Q("1e-40") / 101 > Q("9e-44")
assert Q("1.6") * Q("3e-21") / 100 == Q("4.8e-23")
assert Q("9e-44") ** 2 / 4 > Q("2e-87")
assert Q("4.8e-23") ** 2 / 4 < Q("6e-46")

print("Exact comparison arithmetic passed.")
