"""Exact manufactured controls and rational cap constants (non-production)."""

import platform
from fractions import Fraction

import sympy as sp


def graph_contact():
    # Flat wave, affine graph t=p*x, zero displacement and w_t=1.
    # The past cone crosses the graph between these two roots.
    t, x, p = sp.symbols("t x p", real=True)
    left = (x - t) / (1 - p)
    right = (x + t) / (1 + p)
    retarded = (1 - p**2) * (right - left) / 2
    assert sp.simplify(retarded - (t - p * x)) == 0
    assert sp.simplify((right - left) / 2 - (t - p * x)) != 0
    print("Graph-only retarded solution: t-p*x; omitted/wrong contact rejected.")


def coupled_energy():
    t, x = sp.symbols("t x", real=True)
    rotation = sp.Matrix([[0, -1], [1, 0]])
    connection = sp.diag(1 + x, -2 + x)
    skew = (2 - x) * rotation
    positive = sp.diag(2 + x**2, 3)
    field = sp.Matrix([t**2 + t*x + 2*x**2, 5*t**2 + 7*t*x + 11*x**2])

    def dx(vector):
        return vector.diff(x) + connection * vector

    time = field.diff(t)
    space = dx(field)
    source = time.diff(t) - space.diff(x) + connection * space + positive * field + skew * time
    assert all(entry != 0 for entry in skew * time)
    energy = (time.dot(time) + space.dot(space) + field.dot(positive * field)) / 2
    current = -time.dot(space)
    work_defect = energy.diff(t) + current.diff(x) - time.dot(source)
    assert sp.expand(work_defect) == 0
    wrong_coupling = (2 - x) * sp.Matrix([[0, 1], [1, 0]])
    wrong_source = source - skew * time + wrong_coupling * time
    assert sp.expand(energy.diff(t) + current.diff(x) - time.dot(wrong_source)) != 0
    print("Genuine two-field skew/connection energy verified; symmetric mutation rejected.")


def omitted_interface():
    t, x, u, v, end = sp.symbols("t x u v end", real=True, positive=True)
    left = (t + x)**2
    right = (t - x)**2
    assert left.diff(t, 2) == left.diff(x, 2)
    assert right.diff(t, 2) == right.diff(x, 2)
    assert (right.diff(x) - left.diff(x)).subs(x, 0) == -4*t
    null_right = right.subs({t: (u + v)/2, x: (v - u)/2})
    norm_sq = sp.integrate(null_right.diff(u)**2, (u, 0, end))
    assert norm_sq == 4*end**3/3
    print("Bulk residual zero but interface residual 4*t*delta(x); cap norm^2=4*U^3/3.")


def cap_constants():
    # The finite positive atanh series gives a lower bound for -log(5/8).
    a = Fraction(3, 13)
    log_lower = 2 * sum(a**(2*j + 1) / (2*j + 1) for j in range(6))
    d_upper = Fraction("8.407362") + Fraction(13, 8) - 2 * log_lower
    assert d_upper < Fraction("9.092355")
    # Machin's identity and alternating Taylor bounds for both arctangents.
    atan5_lower = sum(Fraction((-1)**j, 5**(2*j + 1) * (2*j + 1)) for j in range(4))
    pi_lower = 16 * atan5_lower - Fraction(4, 239)
    assert pi_lower > Fraction("3.14159")
    k_upper = d_upper * (Fraction(6, 100) - Fraction(3, 2 * 100**2)) / pi_lower
    assert k_upper < Fraction("0.173218")
    lambda_upper = 3 * d_upper**2 / (pi_lower**2 * 100**2)
    exp_upper = 1 / (1 - lambda_upper)
    degree_three_factor = exp_upper * k_upper**4 / 24
    assert lambda_upper < Fraction("0.002514")
    assert exp_upper < Fraction("1.00252")
    assert degree_three_factor < Fraction("0.00003761")
    print("r_c=100: Lambda<0.002514, exp(Lambda)<1.00252, degree-three error factor<3.761e-5.")


def leading_weights():
    delta = Fraction(3, 25)
    mass = Fraction("1.61")
    pi_lower = Fraction("3.14159")
    argument = delta * mass
    exp_upper = 1 + argument + argument**2 / (2 * (1 - argument / 3))
    shear_upper = exp_upper / pi_lower
    derivative_error = delta * (
        delta * mass * shear_upper / 4
        + delta * Fraction(5, 21) * Fraction(2, 9) / pi_lower
        + mass * (1 + argument * shear_upper)
    )
    endpoint = (delta * mass)**2 * shear_upper
    assert derivative_error < Fraction("0.211")
    assert endpoint < Fraction("0.0145")
    print("Exact leading test weights: ||g'-b''||_2<0.211 and |g(1)|<0.0145.")


if __name__ == "__main__":
    print(f"Python {platform.python_version()}, SymPy {sp.__version__}")
    graph_contact()
    coupled_energy()
    omitted_interface()
    cap_constants()
    leading_weights()
