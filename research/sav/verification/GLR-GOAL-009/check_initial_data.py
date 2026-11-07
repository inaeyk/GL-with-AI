"""Independent warped-curvature and momentum checks for the damped Z ansatz."""
import sympy as sp

r, z, theta, phi = sp.symbols("r z theta phi", positive=True)
coords = (r, z, theta, phi)
a, radius, q, eta, d = (
    sp.Function(name)(r, z) for name in ("a", "R", "q", "eta", "d")
)
kappa = d - q / 2
gamma = sp.diag(a**2, 1, radius**2, radius**2 * sp.sin(theta)**2)
inverse = gamma.inv()
christoffel = [
    [
        [
            sum(
                inverse[k, ell]
                * (
                    sp.diff(gamma[j, ell], coords[i])
                    + sp.diff(gamma[i, ell], coords[j])
                    - sp.diff(gamma[i, j], coords[ell])
                )
                / 2
                for ell in range(4)
            )
            for j in range(4)
        ]
        for i in range(4)
    ]
    for k in range(4)
]
curvature = sum(
    inverse[i, i]
    * sum(
        sp.diff(christoffel[k][i][i], coords[k])
        - sp.diff(christoffel[k][k][i], coords[i])
        + sum(
            christoffel[k][k][ell] * christoffel[ell][i][i]
            - christoffel[k][i][ell] * christoffel[ell][k][i]
            for ell in range(4)
        )
        for k in range(4)
    )
    for i in range(4)
)
extrinsic = sp.Matrix(
    [[kappa, eta / a, 0, 0], [a * eta, 0, 0, 0],
     [0, 0, q / 2, 0], [0, 0, 0, q / 2]]
)
trace = sp.trace(extrinsic)
Z = trace**2 - sp.trace(extrinsic**2)
assert sp.simplify(Z - (-q**2 / 2 + 2 * d * q - 2 * eta**2)) == 0

U = a**-2
H = sp.diff(radius, r) / radius
gz = sp.diff(radius, z) / radius
leaf_curvature = 2 / radius**2 - 4 * sp.diff(gz, z) - 6 * gz**2
curvature_expected = (
    leaf_curvature - (4 * sp.diff(H, r) + 6 * H**2) * U
    - 2 * H * sp.diff(U, r) + sp.diff(U, z, 2) / U
    + 2 * gz * sp.diff(U, z) / U - 3 * sp.diff(U, z)**2 / (2 * U**2)
)
assert sp.simplify(curvature - curvature_expected) == 0
print("PASS direct spatial Christoffels: Hamiltonian curvature and K contraction")

momentum = []
for j in range(2):
    momentum.append(
        sum(
            sp.diff(extrinsic[i, j], coords[i])
            + sum(
                christoffel[i][i][ell] * extrinsic[ell, j]
                - christoffel[ell][i][j] * extrinsic[i, ell]
                for ell in range(4)
            )
            for i in range(4)
        )
        - sp.diff(trace, coords[j])
    )
q_rhs = a * sp.diff(eta, z) + 2 * sp.diff(a, z) * eta + 2 * a * gz * eta
q_rhs += 2 * H * (d - q)
eta_rhs = a * (sp.diff(d, z) + sp.diff(q, z) / 2 + gz * q)
eta_rhs += (d - q / 2) * sp.diff(a, z) - 2 * H * eta
assert sp.simplify(momentum[0] - (q_rhs - sp.diff(q, r))) == 0
assert sp.simplify(a * momentum[1] - (sp.diff(eta, r) - eta_rhs)) == 0
assert sp.simplify(momentum[0] - sp.diff(a, z) * eta - (q_rhs - sp.diff(q, r))) != 0
print("PASS both momentum components and wrong radial acceleration control")

p, w0, X, Y, IY, k, D0, A0, B0, C0 = sp.symbols(
    "p w0 X Y IY k D0 A0 B0 C0", real=True
)
# For a real Fourier pair X cos(kz), Y sin(kz), integrate over one
# period with normalized average.  IY is then -Y cos(kz)/k.
osc_X = A0 * k * Y
osc_Y = -A0 * k * X / 2 - p * Y / r
P = X**2 / 4 + Y**2 / 2
S = -X * Y / (2 * k)
def osc_derivative(expression):
    return sp.diff(expression, X) * osc_X + sp.diff(expression, Y) * osc_Y

assert sp.simplify(osc_derivative(S) - (A0 * (X**2 / 4 - Y**2 / 2) - p * S / r)) == 0
assert sp.simplify(osc_derivative(P - p * S / (r * A0)) + p * P / r - p**2 * S / (r**2 * A0)) == 0
assert sp.simplify(osc_derivative(P + p * S / (r * A0)) + p * P / r) != 0
print("PASS oscillator sign and wrong correction sign control")

lam = sp.symbols("lambda", real=True)
ri = sp.Rational(3, 4)
mass_ratio = 1 - ri**2 * (sp.exp(-2 * lam) - 1) / (ri + 1)
assert sp.diff(mass_ratio, lam).subs(lam, 0) == sp.Rational(9, 14)
print("PASS spherical ADM transversality 9/14")

# Mean-free far block: ideal g=0, chi=1, linearization about Schwarzschild.
s = sp.symbols("s", real=True)
Xz, Yz, Wz, Wzz = sp.symbols("X_z Y_z W_z W_zz", real=True)
Q0 = 2 * sp.sqrt(r / (r + 1))
d0 = 1 / (2 * r**sp.Rational(3, 2) * (r + 1)**sp.Rational(3, 2))
U0 = r / (r + 1)
a0 = U0**-sp.Rational(1, 2)
U_s = U0 + s * w0 / r
a_s = U_s**-sp.Rational(1, 2)
a_z_s = -s * Wz / (2 * r * U_s**sp.Rational(3, 2))
Q_s = Q0 + s * X
J_s = s * Y
d_control_s = -s * p / (a_s * r**3) * IY
hamiltonian_rhs = (
    r * s * Wzz / (2 * U_s) - 3 * (s * Wz)**2 / (4 * U_s**2)
    - (Q_s**2 - Q0**2) / (4 * r**2)
    + d0 * (Q_s - Q0) + d_control_s * Q_s - J_s**2 / r**2
)
Q_rhs = a_s * s * Yz + 2 * a_z_s * J_s + 2 * r * d_control_s
J_rhs = a_s * s * Xz / 2 + (r**2 * d0 - Q_s / 2) * a_z_s - p * J_s / r
expected = (
    r * Wzz / (2 * U0) + (d0 - Q0 / (2 * r**2)) * X - p * Q0 * IY / (a0 * r**3),
    a0 * Yz - 2 * p * IY / (a0 * r**2),
    a0 * Xz / 2 - p * Y / r - a0**3 * (r**2 * d0 - Q0 / 2) * Wz / (2 * r),
)
for rhs, linear_term in zip((hamiltonian_rhs, Q_rhs, J_rhs), expected):
    assert sp.simplify(sp.diff(rhs, s).subs(s, 0) - linear_term) == 0
print("PASS mean-free far block linearization")
