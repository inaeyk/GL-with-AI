"""Exact constant checks for the early-source sign and critical-scale bounds."""
from fractions import Fraction as Q
from math import factorial

mode_k_squared_lower = Q("0.39")
omega_upper = Q("0.064")
s_lower = Q("0.126")
a_floor = Q(5)
assert mode_k_squared_lower * a_floor**3 > 2
assert 1 + Q(3, 2) + Q(3, 2)**2/2 + Q(3, 2)**3/6 > 4  # exp(3/2)>4

# d=y_r/y lies in (-.9,.4). The angular and mixed ratios are
# normalized by U_GL/r. f/r decreases for r>=5.
angular_magnitude_lower = Q(1, 2) - Q("0.9") * Q(4, 25) / mode_k_squared_lower
mixed_magnitude_upper = omega_upper * (Q("0.9") + Q(1, 40)) / mode_k_squared_lower
assert angular_magnitude_lower > Q("0.13")
assert mixed_magnitude_upper < Q("0.152")
assert 4 * Q("0.13")**2 - 2 * Q("0.152")**2 == Q("0.021392")

# log(5/4)<1/4. The curvature error is gamma*n_0.
kappa_upper = (2 * Q(1, 4) - 1/a_floor) / 4
gamma_upper = 1 / (4 * s_lower * a_floor**2 * (1-kappa_upper))
assert kappa_upper == Q("0.075")
assert gamma_upper < Q("0.09")
assert (1+Q("0.09"))/(1-Q("0.09")) < Q(6, 5)

# e^(1/2) uses its first three terms and a geometric bound on the rest.
exp_half_upper = 1 + Q(1, 2) + Q(1, 8) + Q(1, 48)/(1-Q(1, 8))
assert exp_half_upper < Q(5, 3)
assert Q(6, 5) * (2*Q(5, 3)-3) == Q(2, 5)
assert Q(7, 5) * Q(6, 5) / 2 == Q(21, 25)
assert Q(3, 10) * (2*Q("6.5671e-5"))**2 > Q("5.17e-9")

# Sharp positive remainder threshold from the pinned Z energy lower endpoint.
assert Q("4.20214e-4")**2 < Q("3.5316e-7")/2
print("Exact gates: source sign, curvature error < 0.09, and early joint-scale suppression for 0<Lambda<=s")
print("Exact gates: benchmark energy reduction > 5.17e-9 Lambda and seed-positivity norm threshold")

# Signed kernel mutation control: use exact alternating terms at t=4.

for nu in (0, 1):
    # F_nu(t) = 0F1(nu+1; -t). From j>=2 the magnitudes decrease.
    def term(j):
        return Q((-4)**j * factorial(nu), factorial(j) * factorial(j+nu))
    truncated = sum((term(j) for j in range(11)), Q(0))
    omitted = term(11)
    assert truncated + abs(omitted) < 0
    print(f"Exact kernel control: F_{nu}(4) < 0, upper bound (display) {float(truncated+abs(omitted)):.12f}")
