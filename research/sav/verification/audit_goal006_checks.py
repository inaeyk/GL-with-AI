"""Independent lightweight checks for the pinned GOAL-006 audit."""

import math
import platform

import scipy
import sympy as sp
from scipy.integrate import quad


def check_mass_and_damping():
    """Check the radial ADM coefficient and oscillatory cross-energy signs."""
    radius = sp.Rational(3, 4)
    inner_lapse = sp.symbols("lambda", real=True)
    mass_ratio = 1 - radius**2 / (radius + 1) * (sp.exp(-2 * inner_lapse) - 1)
    assert sp.diff(mass_ratio, inner_lapse).subs(inner_lapse, 0) == sp.Rational(9, 14)

    r, a, wave_number, damping = sp.symbols("r a k p", positive=True)
    x, y = sp.symbols("x y", real=True)
    # X=x cos(kz), Y=y sin(kz); a is frozen in z.
    x_prime = a * wave_number * y
    y_prime = -a * wave_number * x / 2 - damping * y / r
    energy = x**2 / 4 + y**2 / 2
    cross = -x * y / (2 * wave_number)
    assert sp.simplify(sp.diff(energy, x) * x_prime + sp.diff(energy, y) * y_prime
                       + damping * y**2 / r) == 0
    modified = energy - damping * cross / (r * a)
    modified_rate = (sp.diff(modified, x) * x_prime + sp.diff(modified, y) * y_prime
                     + sp.diff(modified, r))
    assert sp.simplify(modified_rate + damping * energy / r
                       - damping * (damping + 1) * cross / (a * r**2)) == 0
    reversed_rate = modified_rate + sp.diff(modified, y) * 2 * damping * y / r
    assert sp.simplify(sp.expand(reversed_rate - modified_rate).coeff(y, 2)) == 2 * damping / r
    print("ADM slope 9/14 and weighted cross energy verified; reversed damping rejected.")


def check_compact_optical_operator():
    """Compute the reduced lapse wave operator from the inverse leaf metric."""
    u, r = sp.symbols("u r", real=True, positive=True)
    lapse = sp.Function("b")(u, r)
    shift = sp.Function("A")(u, r)
    coeff = sp.Function("B")(u, r)
    potential = sp.Function("V")(u, r)
    leaf_metric = sp.Matrix([[-coeff * potential, -coeff], [-coeff, 0]])
    density = coeff * r**2
    gradient = sp.Matrix([sp.diff(lapse, u), sp.diff(lapse, r)])
    current = density * leaf_metric.inv() * gradient
    wave = sum(sp.diff(component, coord) for component, coord in zip(current, (u, r))) / density
    q = r * (lapse - 1)
    target = (-2 * sp.diff(q, u, r) + sp.diff(potential * sp.diff(q, r), r)
              - sp.diff(potential, r) * q / r)
    assert sp.simplify(r * coeff * wave - target) == 0
    assert sp.simplify(r * coeff * wave - target - 2 * sp.diff(q, u, r)) != 0

    # With g_rr=g_rz=0 the radial compact shift makes K_rr=0.
    z = sp.symbols("z")
    coeff = sp.Function("B")(u, r, z)
    potential = sp.Function("V")(u, r, z)
    leaf_metric = sp.Matrix([[-coeff * potential, -coeff], [-coeff, 0]])
    jacobian = sp.Matrix([[0, 0], [sp.diff(shift, u), sp.diff(shift, r)]])
    twice_b_k = (leaf_metric.diff(z) - shift * leaf_metric.diff(r)
                 - jacobian.T * leaf_metric - leaf_metric * jacobian)
    mixed = leaf_metric.inv() * twice_b_k / 2
    h = (sp.diff(coeff, z) - shift * sp.diff(coeff, r)) / coeff - sp.diff(shift, r)
    assert sp.simplify(sp.trace(mixed) - h) == 0
    assert sp.simplify(sp.trace(mixed * mixed) - h**2 / 2) == 0
    print("Full-optical lapse operator and compact extrinsic-curvature traces verified.")


def check_forced_ricci():
    """Construct the flat five-dimensional Ricci symbol from the connection."""
    metric = sp.diag(-1, 1, 1, 1, 1)
    omega, wave_number = sp.symbols("omega K", real=True, nonzero=True)
    derivative = sp.I * sp.Matrix([-omega, 2, 0, 0, wave_number])
    raised = metric * derivative
    tensor = sp.zeros(5)
    for row in range(4):
        for col in range(row, 4):
            tensor[row, col] = tensor[col, row] = sp.Symbol(f"h{row}{col}", real=True)

    def christoffel(upper, first, second):
        return metric[upper, upper] * (derivative[first] * tensor[second, upper]
                                       + derivative[second] * tensor[first, upper]
                                       - derivative[upper] * tensor[first, second]) / 2

    ricci = sp.Matrix(5, 5, lambda a, b: sum(
        derivative[c] * christoffel(c, a, b) - derivative[b] * christoffel(c, a, c)
        for c in range(5)))
    source = -ricci
    einstein_source = source - metric * sp.trace(metric * source) / 2
    assert all(sp.expand(entry) == 0 for entry in einstein_source * raised)

    expected_trace = sp.trace(metric * tensor)
    expected_div = (tensor * raised)[:4, :]
    trace = -2 * source[4, 4] / wave_number**2
    div = derivative[:4, :] * trace + 2 * sp.I * source[:4, 4] / wave_number
    assert sp.expand(trace - expected_trace) == 0
    assert all(sp.expand(entry) == 0 for entry in div - expected_div)
    assert sp.expand(trace) != 0
    assert any(sp.expand(entry) != 0 for entry in div)
    bad_trace = -trace
    assert sp.expand(bad_trace - expected_trace) != 0

    deriv4 = derivative[:4, :]
    raised4 = raised[:4, :]
    wave_symbol = (deriv4.dot(raised4) - wave_number**2)
    forcing = (2 * source[:4, :4] + deriv4 * div.T + div * deriv4.T
               - trace * deriv4 * deriv4.T)
    assert all(sp.expand(entry) == 0 for entry in forcing - wave_symbol * tensor[:4, :4])
    assert sp.expand(sp.trace(metric[:4, :4] * forcing) - wave_symbol * trace) == 0
    assert all(sp.expand(entry) == 0 for entry in forcing * raised4 - wave_symbol * div)
    # The static time-frequency specialization also obeys the forced system.
    assert all(sp.expand(entry.subs(omega, 0)) == 0
               for entry in forcing - wave_symbol * tensor[:4, :4])
    print("Five-dimensional Ricci symbol, source Bianchi, forced defects and static limit verified.")


def check_massive_threshold_and_tube_trace():
    """Check the published monopole potential and a free-wave limit counterexample."""
    mass_squared = (2 * math.pi / 5)**2
    bracket_at_horizon = (mass_squared**2 - 4 * mass_squared + 1) / (1 + mass_squared)
    assert bracket_at_horizon < 0
    print(f"Monopole potential V/f at the horizon: {bracket_at_horizon:.9f}.")

    # H=sin(v^2)/v is a free profile. H_u at fixed v is zero.
    # Along a tube of fixed r on flat space, v=u+2r.
    print("Fixed-r squared derivative norms for the vanishing free field:")
    for radius in (8, 32, 128):
        def derivative_square(u):
            v = u + 2 * radius
            return (2 * math.cos(v**2) - math.sin(v**2) / v**2)**2

        norm_squared = quad(derivative_square, -1, 1, epsabs=1e-9, limit=1000)[0]
        assert abs(norm_squared - 4) < 0.1
        print(f"  r={radius}: {norm_squared:.9f}; limiting value 4")


if __name__ == "__main__":
    print(f"Python {platform.python_version()}, SciPy {scipy.__version__}, SymPy {sp.__version__}.")
    check_mass_and_damping()
    check_compact_optical_operator()
    check_forced_ricci()
    check_massive_threshold_and_tube_trace()
