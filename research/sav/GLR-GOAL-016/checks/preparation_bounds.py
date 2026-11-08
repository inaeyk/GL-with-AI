"""Exact rational bounds for the cancellation-cost estimate."""
from fractions import Fraction as Q
from math import factorial, isqrt


def exp_partial(x, n):
    return sum((x**j / factorial(j) for j in range(n + 1)), Q(0))


def exp_upper(x, n):
    first_omitted = x ** (n + 1) / factorial(n + 1)
    return exp_partial(x, n) + first_omitted / (1 - x / (n + 2))


# Signs of g(5.1)+8 and g(6.6)+10, using monotonicity of g.
assert exp_partial(Q(29, 20), 8) > Q(41, 10)
assert exp_upper(Q(17, 10), 12) < Q(28, 5)
left_cap, right_floor = Q(33, 5), Q(51, 10)
length = Q(2)
ray_max = 1 / (4 * right_floor**2)
# Use pi > 25/8 and monotonicity of (r-1)/r^2 for r>2.
response = ray_max * exp_upper(ray_max * length, 8) * Q(32, 25)
in_cut_potential = length * (right_floor**-2 - left_cap**-2) / 4
static_tail = length * (right_floor - 1) / (2 * right_floor**2)
ratio = (1 + in_cut_potential + static_tail) / (1 - response)**2
assert response < Q(13, 1000)
assert ratio < Q(6, 5)
print("proved: R(-8) > 5.1 and R(-10) < 6.6")
print(f"response upper bound (display) = {float(response):.9f}")
print(f"cost ratio upper bound (display) = {float(ratio):.9f}")
print("strict rational gates: response < 0.013, cost ratio < 1.20")

# A directed display of the inherited Z norm interval using integer square roots.
scale = 10**11
lo, hi = Q("3.5316e-7") / 2, Q("4.0171e-7") / 2
scaled_lo, scaled_hi = lo * scale**2, hi * scale**2
lo_int = isqrt(scaled_lo.numerator // scaled_lo.denominator)
hi_int = 1 + isqrt(scaled_hi.numerator // scaled_hi.denominator)
assert Q(lo_int, scale)**2 < lo < hi < Q(hi_int, scale)**2
print(f"Z norm directed bounds: {lo_int}/{scale} < ||n_Z||_2 < {hi_int}/{scale}")
