"""Direct five-dimensional Ricci coefficient check for smooth manufactured EF jets."""
import numpy as np
import sympy as sp
from overlap_mean import ricci_mean, source

v, r, z, theta, phi = sp.symbols("v r z theta phi", real=True)
coords = (v, r, z, theta, phi)
omega, k = sp.Rational(13, 137), sp.Rational(11, 19)
f = 1 - 1/r
radial_jet = sp.Matrix([r**2 - r/2, (r - 1)**2, r**3/11, 2*r - r**2/5])
amplitude = sp.exp(omega*v) * sp.cos(k*z)
p, q, a, w = amplitude * radial_jet
g0 = sp.Matrix([[ -f, 1, 0, 0, 0], [1, 0, 0, 0, 0],
                [0, 0, 1, 0, 0], [0, 0, 0, r*r, 0],
                [0, 0, 0, 0, r*r*sp.sin(theta)**2]])
g1 = sp.Matrix([[p, q, 0, 0, 0], [q, a, 0, 0, 0],
                [0, 0, 0, 0, 0], [0, 0, 0, w, 0],
                [0, 0, 0, 0, w*sp.sin(theta)**2]])
G = (g0, g1)
first = [[g.diff(x) for x in coords] for g in G]
second = [[[d.diff(x) for x in coords] for d in ds] for ds in first]
evaluate = lambda g, point: np.array(g.subs(point), dtype=float)

def curvature2(radius, phase):
    point = {v: 0, r: radius, z: phase/k, theta: sp.pi/3, phi: 0}
    a0, h = (evaluate(g, point) for g in G)
    inv0 = np.linalg.inv(a0)
    inv1 = -inv0 @ h @ inv0
    inv2 = -inv1 @ h @ inv0
    inv = np.array([inv0, inv1, inv2])
    dg = np.array([[evaluate(d, point) for d in ds] for ds in first])
    ddg = np.array([[[evaluate(d, point) for d in ds] for ds in rows] for rows in second])

    connection = np.zeros((3, 5, 5, 5))
    derivative = np.zeros((3, 5, 5, 5, 5))
    for n in range(3):
        for m in range(min(n, 1)+1):
            dg_m, ddg_m = dg[m], ddg[m].transpose(0, 2, 3, 1)
            product = dg_m + dg_m.transpose(2, 1, 0) - dg_m.transpose(1, 0, 2)
            dproduct = ddg_m + ddg_m.transpose(2, 1, 0, 3) - ddg_m.transpose(1, 0, 2, 3)
            connection[n] += np.einsum("il,jlk->ijk", inv[n-m], product)/2
            derivative[n] += np.einsum("il,jlka->ijka", inv[n-m], dproduct)/2
            for p in range(n-m+1):
                for q in range(min(n-m-p, 1)+1):
                    s = n-m-p-q
                    derivative[n] -= np.einsum(
                        "ib,abd,dl,jlk->ijka", inv[p], dg[q], inv[s], product)/2
    ricci = np.einsum("ijki->jk", derivative[2]) - np.einsum("ijik->jk", derivative[2])
    for m in range(3):
        ricci += np.einsum("iil,ljk->jk", connection[m], connection[2-m])
        ricci -= np.einsum("ikl,lji->jk", connection[m], connection[2-m])
    return inv0, ricci

class Manufactured:
    omega, k = float(omega), float(k)
    def jet(self, radius):
        return [tuple(float(c) for c in radial_jet.diff(r, j).subs(r, radius))
                for j in range(3)]

for radius in map(sp.Rational, ["1.02", "1.612", "2.50", "7.23"]):
    inv, qr = curvature2(radius, 0)
    _, qi = curvature2(radius, sp.pi/2)
    average = (qr + qi)/2
    expected = np.array([average[1, 1], average[3, 3], average[2, 2]])
    actual = ricci_mean(Manufactured(), float(radius))
    geometric_trace = np.einsum("ij,ji", inv, average)
    expected_t = np.array([-(inv @ average)[0, 0] + geometric_trace/2, -average[1, 1]])
    actual_t = source(Manufactured(), float(radius))
    scaled = lambda result, ref: np.max(np.abs(result-ref) / np.maximum(1, np.abs(ref)))
    print(f"r={float(radius):.3f}, Ricci scaled error {scaled(actual, expected):.3e}, "
          f"Einstein source scaled error {scaled(actual_t, expected_t):.3e}")
    assert scaled(actual, expected) < 1e-12
    assert scaled(actual_t, expected_t) < 1e-12
