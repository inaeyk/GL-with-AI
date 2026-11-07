"""Exact independent normalization and characteristic negative controls."""

import platform

import sympy as sp


def angular_vacuum_identity():
    u, r, z = sp.symbols("u r z", real=True)
    B, b, V, shift = [
        sp.Function(name)(u, r, z) for name in ("B", "b", "V", "shift")
    ]
    metric = sp.Matrix([
        [-B * V + b**2 * shift**2, -B, b**2 * shift],
        [-B, 0, 0],
        [b**2 * shift, 0, b**2],
    ])
    inverse = metric.inv()
    assert metric.det() == -(B * b)**2
    density = B * b
    box_r = sum(
        sp.diff(density * inverse[i, 1], coordinate)
        for i, coordinate in enumerate((u, r, z))
    ) / density
    sphere_ricci = 1 - inverse[1, 1] - r * box_r
    expected = density + r * b.diff(u) - (r * b * V).diff(r)
    expected -= r * (b * shift).diff(z)
    assert sp.simplify(density * sphere_ricci - expected) == 0
    assert sp.simplify(density * sphere_ricci - expected + r * b.diff(u)) != 0
    print("PASS angular vacuum identity; dropping the radion time derivative fails")

    phi = sp.Function("Phi")(u)
    trace = 2 * phi / sp.sqrt(3)
    # Substitute the leading averaged profiles into the exact angular identity.
    beta, velocity = sp.symbols("B_inf V_inf")
    b_end = 1 + trace / r
    residual = (
        (r * b_end * velocity).diff(r) - beta * b_end - r * b_end.diff(u)
    )
    leading = sp.limit(residual, r, sp.oo)
    assert sp.solve(leading, velocity) == [beta + 2 * phi.diff(u) / sp.sqrt(3)]
    assert leading.subs(velocity, beta) == -2 * phi.diff(u) / sp.sqrt(3)
    print("PASS areal end requires V_inf = B_inf + 2 Phi_lambda/sqrt(3)")


def einstein_radius_pullback():
    u = sp.symbols("u", real=True)
    phi = sp.Function("Phi")(u)
    # rho = r_5 + Phi/sqrt(3) at leading order in a normalized Bondi clock.
    jacobian = sp.Matrix([[1, 0], [phi.diff(u) / sp.sqrt(3), 1]])
    bondi_metric = sp.Matrix([[-1, -1], [-1, 0]])
    sphere_metric = jacobian.T * bondi_metric * jacobian
    assert sphere_metric[0, 1] == -1
    assert sp.simplify(sphere_metric[0, 0] + 1 + 2 * phi.diff(u) / sp.sqrt(3)) == 0
    assert sphere_metric[0, 0] != -1
    print("PASS Einstein-radius pullback agrees; unit areal time-time coefficient fails")


def characteristic_controls():
    t, x = sp.symbols("t x", real=True)
    radius = sp.Function("r")(x)
    weight = sp.Function("W")
    p, f = sp.symbols("p f", real=True)
    w = radius**p * weight(t - x)
    assert w.diff(t) == radius**p * weight(t - x).diff(t)
    expected_x = p * radius**(p - 1) * f * weight(t - x) - w.diff(t)
    assert sp.simplify(w.diff(x).subs(radius.diff(x), f) - expected_x) == 0

    s = sp.symbols("s", positive=True)
    omega = (s + 1/s) / 2
    momentum = (s - 1/s) / 2
    assert sp.simplify(omega**2 - momentum**2) == 1
    assert sp.cancel(omega**2 / ((omega - momentum)**2 + 1)) == (s**2 + 1) / 4
    print("PASS weighted current signs and unbounded transverse-to-null density ratio")

    a, omega, momentum = sp.Rational(99, 100), sp.Rational(41, 9), sp.Rational(40, 9)
    assert omega**2 - momentum**2 == 1
    background_flux = (omega**2 + momentum**2) / 2 - omega * momentum / a
    physical_flux = (omega**2 + momentum**2) / 2 - omega * momentum
    assert background_flux == -sp.Rational(3181, 16038)
    assert background_flux < 0 < physical_flux
    print(f"PASS widened cone rejects background flux positivity: {background_flux}")


def circumference_control():
    phase = sp.symbols("theta", real=True)
    scale = sp.symbols("scale", real=True)
    stretch = 1 + scale * sp.cos(phase)
    # For |scale|<1 the metric length density is stretch, not its square.
    average = lambda field: sp.integrate(field, (phase, 0, 2 * sp.pi)) / (2 * sp.pi)
    assert average(stretch) == 1
    assert sp.simplify(average(stretch**2) - 1 - scale**2 / 2) == 0
    print("PASS exact circumference is invariant while the metric mean changes")


if __name__ == "__main__":
    print(f"Python {platform.python_version()}, SymPy {sp.__version__}")
    angular_vacuum_identity()
    einstein_radius_pullback()
    characteristic_controls()
    circumference_control()
