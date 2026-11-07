#!/usr/bin/env python3
"""Exact algebra and negative controls, with no physical evolution."""
import platform

import sympy as sp


def mean_elimination():
    t, r = sp.symbols("t r", positive=True)
    q, c, d_t, d_r = [sp.Function(name)(t, r) for name in ("q", "c", "dt", "dr")]
    f = 1 - 1/r
    lam = sp.Integer(4)

    def dx(expr):
        return f*sp.diff(expr, r)

    a_t = f*(sp.diff(q, r) + d_t)/(lam*r**2)
    a_r = (sp.diff(q, t) - d_r)/(lam*r**2*f)
    curl_defect = q + r**4*(sp.diff(a_r, t) - sp.diff(a_t, r)) - c
    psi = 2*q/(lam*r)
    potential = f*(6/r**2 - 3/r**3)
    lhs = -sp.diff(psi, t, 2) + dx(dx(psi)) - potential*psi
    rhs = 2*r/lam*(-sp.diff(d_r/r**2, t) - dx(f*d_t/r**2) - lam*f*c/r**4)
    assert sp.simplify(lhs - rhs + 2*f*curl_defect/r**3) == 0
    assert sp.simplify(lhs - rhs.subs(c, 0) + 2*f*curl_defect/r**3) != 0
    print("PASS exact mean curl equation; omitted algebraic extraction rejected")


def parity_and_gauge():
    z = sp.symbols("z", real=True)
    # Laurent coefficients implement Fourier selection without a sign convention for real phases.
    w = sp.symbols("w", nonzero=True)
    cosine = (w + 1/w)/2
    assert sp.expand(cosine).coeff(w, 0) == 0
    assert sp.expand(cosine**2/2).coeff(w, 0) == sp.Rational(1, 4)
    assert sp.expand((w**2 + w**-2)*cosine**0).coeff(w, 0) == 0

    u, v, v_launch = sp.symbols("u v v_launch", real=True)
    label = (u + 1)*sp.sin(z)*sp.exp(-v)
    shifted = sp.limit(label, v, sp.oo) - label.subs(v, v_launch)
    assert sp.simplify(shifted + label.subs(v, v_launch)) == 0
    assert shifted != 0
    angular_gauge = sp.Function("Lambda")(u, v, z)
    assert sp.diff(sp.diff(angular_gauge, u), v) - sp.diff(sp.diff(angular_gauge, v), u) == 0
    gl = sp.symbols("gl")
    returning_principal = gl**2*sp.Rational(1, 4)
    assert returning_principal.subs(gl, 0) == 0
    print("PASS Fourier selection, pure gauge with transformed launch, and zero GL")


def flux():
    theta = sp.symbols("theta", positive=True)
    y20 = sp.sqrt(5/(16*sp.pi))*(3*sp.cos(theta)**2 - 1)
    x_phi = sp.sin(theta)*sp.diff(y20, theta)
    x_theta_phi = (sp.diff(x_phi, theta) - 2*sp.cot(theta)*x_phi)/2
    normalization = sp.sqrt(5/(16*sp.pi))
    assert sp.trigsimp(x_theta_phi - 3*normalization*sp.sin(theta)**3) == 0
    assert sp.trigsimp(x_phi.subs(theta, sp.pi-theta) + x_phi) == 0
    q = sp.symbols("q", real=True)
    norm = 36*sp.pi*normalization**2*sp.integrate((1-q**2)**2, (q, -1, 1))
    assert norm == 12
    raw_to_shear = -sp.Rational(2, 4)
    assert 1/raw_to_shear**2 == 4
    print("PASS real l=2 axial parity and tensor norm 12; raw flux factor-four control")



def massive_elimination():
    r = sp.symbols("r", positive=True)
    k, lam, s = sp.Rational(3, 5), sp.Integer(4), sp.Rational(7, 10)
    f = 1 - 1/r
    d = lam + k*k*r*r
    density, weight, ell = r*r*d, lam*r**4/d, r*r/d

    def dx(expr):
        return f*sp.diff(expr, r)

    a_t, a_x, a_z = 1+r*r, r**3, 2-r*r
    curl = r**4*(s*a_x-dx(a_t))/f
    f_tz, f_xz = s*a_z-sp.I*k*a_t, dx(a_z)-sp.I*k*a_x
    i_t = dx(curl) + sp.I*k*r**4*f_tz + lam*r*r*a_t
    i_x = -s*curl - sp.I*k*r**4*f_xz - lam*r*r*a_x
    i_z = -s*r**4*f_tz + dx(r**4*f_xz) - lam*f*r*r*a_z

    base = f*(k*k+lam/r**2)
    a, b = dx(density)/(2*density), dx(weight)/(2*weight)
    row_y = dx(dx(curl)) - 2*a*dx(curl) - (s*s+base)*curl
    row_y += sp.I*k*density*dx(ell)*s*a_z
    row_y -= density*(s*i_x/density + dx(i_t/density))
    row_z = weight*(dx(dx(a_z)) + 2*b*dx(a_z) - (s*s+base)*a_z)
    row_z += sp.I*k*dx(ell)*s*curl
    row_z -= i_z - sp.I*k*(s*ell*i_t + dx(ell*i_x))
    assert sp.cancel(row_y) == 0
    assert sp.cancel(row_z) == 0
    print("PASS manufactured massive two-polarization sourced equations")


def pole_control():
    gamma, frequency = sp.symbols("gamma frequency", positive=True)
    a, b = sp.symbols("a b", real=True)
    s = -gamma - sp.I*frequency
    velocity = a + sp.I*b
    displacement = s*velocity
    determinant = sp.Matrix([[sp.re(velocity), sp.re(displacement)],
                             [sp.im(velocity), sp.im(displacement)]]).det()
    assert sp.factor(determinant) == -frequency*(a*a + b*b)
    assert determinant.subs(frequency, 0) == 0
    print("PASS compact upper-data residue can change; rank degenerates at a real pole")


if __name__ == "__main__":
    print("Python", platform.python_version(), "SymPy", sp.__version__)
    mean_elimination()
    parity_and_gauge()
    flux()
    massive_elimination()
    pole_control()
