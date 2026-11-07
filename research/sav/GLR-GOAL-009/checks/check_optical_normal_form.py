"""Exact coefficient checks for the terminal radial optical normal form."""
import sympy as sp


def main():
    d, exponent, mass, growth = sp.symbols("d a K s", nonzero=True)

    def transform(value):
        return value * (value - 1) / ((value + 2) * (value + 3))

    compact_factor = 1 - 6 / (d * (d + 1))
    assert sp.cancel(transform(d) * compact_factor - transform(d - 1)) == 0

    flat_factor = sp.cancel(transform(d - 2) / transform(d - 1))
    assert sp.cancel(flat_factor - 1 + 6 / (d * (d - 1))) == 0

    curvature_factor = sp.cancel(
        transform(d - 3) * (d - 1) ** 2 / transform(d - 1)
    )
    curvature_target = d**2 - 2 * d - 11 + 24 / (d * (d - 2))
    assert sp.cancel(curvature_factor - curvature_target) == 0

    # Substitute R**(-a) exp(s*u + i*K*z) on both sides.
    # The keys give the extra radial powers over R**(-a).
    test_power = -exponent
    before = {
        0: -mass**2 * compact_factor.subs(d, test_power),
        1: -2 * growth * test_power,
        2: test_power * (test_power - 1),
        3: -(test_power - 1) ** 2,
    }
    after = {
        0: -mass**2,
        1: -2 * growth * test_power,
        2: test_power * (test_power - 1) - 6,
        3: -curvature_target.subs(d, test_power),
    }
    for radial_shift, coefficient in before.items():
        discrepancy = (
            transform(test_power - radial_shift) * coefficient
            - transform(test_power - 1) * after[radial_shift]
        )
        assert sp.cancel(discrepancy) == 0

    wrong_curvature = curvature_target - 24 / (d * (d - 2))
    assert sp.cancel(curvature_factor - wrong_curvature) != 0
    print(f"SymPy {sp.__version__}")
    print("PASS terminal compact, mixed, flat radial and Schwarzschild factors")
    print("PASS independent radial-power coefficient substitution")
    print("REJECT dropping the radial order-zero Schwarzschild integral")


if __name__ == "__main__":
    main()
