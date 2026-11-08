"""Directed GOAL-015 weak pairings for piecewise current candidates.

Use the pinned early GOAL-014 dump or the refined GOAL-015 packet dump at
r_c=400 (see dossier).
This encloses an exact reconstructed candidate and its test/dual uncertainty.
Physical current mismatch is a separate input to the result.
"""

import argparse
import hashlib
import platform

import flint
from flint import arb, ctx
import numpy as np


EARLY_DIGESTS = {
    "F": "89c6b9697a1dd98938fe36224ce57a2e6a4a338791e4cfaff62bbe0c9f4540eb",
    "G": "9bb64729787b7cd3e29ea9e7df138f2e0fe3258811fb67924be0aa56504fa6a5",
    "H": "3cab7b6ccfc6baf26d626c567db6268ab567eba73a74671a2cf7cd7d36617ea0",
    "u": "890ac51aa2d7485b108b053eaea043ce3e880ac1b96620cdf80ab6734ced01c7",
    "v": "4eda4ff3a7714ee87d4b2bd6ad92901d9a8c6ca47467bf9aca78cb6de31d78c3",
}


PACKET_DIGESTS = {
    "F": "270742549955721276872574e53b082911c2e031425e2f930c78e49e022bd56b",
    "G": "a4cb59e32469e8793c7a1e71a7c8bb67774a0d0df5015a001e1e13e7d428f6ce",
    "H": "125fb151a7d31256768370e6c2f8f9aa41e106924bcdd1aeeab9eb7c3c8705b2",
    "u": "4456677228f79d04cb2ab8f81c455ce2a890e634a18e99d9d1ea16e72d1d0765",
    "v": "3b7717fd03a578c7865f0f8c5c6845027deee13fd18533a8760406991b56863a",
}


def graph(u):
    return 1 + 2 * (((-1 - u) / 2).exp() / 2).lambertw()


def J(radius):
    return 3 / radius - 3 / (4 * radius**2)


def radial(u, v):
    return 1 + (((v - u) / 2 - 1).exp()).lambertw()


def bump(y, delta, norm):
    """Physical free shear and news, including flat endpoint enclosures."""
    if y.upper() <= 0 or y.lower() >= 1:
        return arb(0), arb(0)
    scale = delta.sqrt() / norm
    if y.lower() <= 0 or y.upper() >= 1:
        t = y.upper() if y.lower() <= 0 else 1-y.lower()
        s_min = 1 / (t * (1 - t))
        assert s_min > 2
        a_upper = (-s_min).exp() * scale
        ap_upper = s_min**2 * a_upper / delta
        return arb(0).union(a_upper), (arb(0).union(ap_upper) if y.lower() <= 0
                                     else arb(0).union(-ap_upper))
    s = 1 / (y * (1 - y))
    a = (-s).exp() * scale
    return a, (1 - 2 * y) * s**2 * a / delta


def symmetric(radius):
    return arb(0, radius.abs_upper())


def array(filename, packet=False):
    expected = PACKET_DIGESTS if packet else EARLY_DIGESTS
    with np.load(filename) as data:
        result = {key: np.asarray(data[key], dtype="<f8") for key in expected}
    for name, values in result.items():
        digest = hashlib.sha256(values.tobytes()).hexdigest()
        assert digest == expected[name], f"{name} digest {digest}"
        if packet:
            print(f"{name} binary64-LE SHA-256 {digest}")
        assert np.isfinite(values).all()
    return result


def row_ranges(data, v, W):
    """Certified ray moments and absolute norms at the exact u nodes.

    On each v-panel use the exact affine integral. W_j encloses J(r(u,v))
    across all u and the entire panel. Split its midpoint and half-width
    so sign changes of the affine current are handled correctly.
    """
    n = data["F"].shape[0] - 1
    rows = []
    for i in range(n + 1):
        values = [[arb(float(x)) for x in data[k][i, n:]] for k in "FGH"]
        graph_values = [arb(float(data[k][i, n-i])) for k in "FGH"]
        norm_F, norm_H, moment_F, moment_H, moment_JF = [arb(0) for _ in range(5)]
        for j, (left, right) in enumerate(zip(v, v[1:])):
            step = right - left
            F0, F1 = values[0][j:j+2]
            H0, H1 = values[2][j:j+2]
            f_signed = step * (F0 + F1) / 2
            f_abs = step * (abs(F0) + abs(F1)) / 2
            moment_F += f_signed
            moment_H += step * (H0 + H1) / 2
            norm_F += f_abs
            norm_H += step * (abs(H0) + abs(H1)) / 2
            mid = W[j].mid()
            moment_JF += mid * f_signed + symmetric((W[j] - mid) * f_abs)
        sup_G = max(abs(x).upper() for x in [graph_values[1], *values[1]])
        rows.append((moment_F, moment_H, moment_JF, norm_F, norm_H, sup_G,
                     graph_values[0], values[0][0], graph_values[2], values[2][0]))
    return rows


def lerp(lo, hi, t):
    return lo + (hi - lo) * t


def calculate(filename, subcells, bits, packet=False):
    ctx.prec = bits
    data = array(filename, packet)
    delta = arb(3) / 25
    D = delta if packet else delta / 8
    R = arb(13) / 8
    rc = arb(400)
    a = -R - 2 * (R - 1).log()
    d = a + D
    c = d + 2 * (rc + (rc - 1).log())
    n = data["F"].shape[0] - 1
    if not packet:
        assert n == 192 and data["F"].shape[1] == n + 721
    assert abs(float(data["u"][-1] - data["u"][0])-float(D)) < 2e-14
    assert abs(float(data["v"][-1])-float(c)) < 2e-12
    v = [R, *(arb(float(x)) for x in data["v"][n+1:-1]), c]
    assert all(r > l for l, r in zip(v, v[1:]))
    radial_moments = [J(radial(a, right)).union(J(radial(d, left)))
                      for left, right in zip(v, v[1:])]
    rows = row_ranges(data, v, radial_moments)
    h = D / n
    du = h / subcells
    norm = arb("0.04679151546115").union(arb("0.04679151580708"))
    M = arb("1.61" if packet else "1.57")
    assert J(graph(d)) < M
    # |g - (B' - J(b)B)| <= M*(C-1)*B for the physical early baseline.
    C = 1 / (1 - M / 522)
    F_prim, F_test, F_wedge, JF, H = [arb(0) for _ in range(5)]
    all_F, all_H = [arb(0) for _ in range(2)]
    all_G = max(row[5] for row in rows)
    wedge_max = R - graph(d)
    area = arb(0)
    scale = delta/D
    for i, (lo, hi) in enumerate(zip(rows, rows[1:])):
        # Rectangle ray derivative is constant; integrate B' exactly here.
        slope = (hi[0] - lo[0]) / h
        B_l, _ = bump(arb(i)/(scale*n), delta, norm)
        B_r, _ = bump(arb(i+1)/(scale*n), delta, norm)
        F_prim += slope * (B_r - B_l)
        for j in range(subcells):
            t0, t1 = arb(j)/subcells, arb(j+1)/subcells
            t = t0.union(t1)
            y = (i + t)/(scale*n)
            b = graph(a + delta * y)
            bprime = -(b-1)/(b+1)
            B, Bp = bump(y, delta, norm)
            if packet:
                # Entire first scatter remainder in physical u:
                # |g-B′+JB| <= M^2 exp(MD) integral_a^u B.
                area += du*B.abs_upper()
                test_error = M*M*(M*D).exp()*area
            else:
                test_error = M*(C-1)*B
            g_minus_Bp = -J(b)*B + symmetric(test_error)
            g = Bp + g_minus_Bp
            F_test += du * slope * g_minus_Bp
            fg, fR = [lerp(lo[k], hi[k], t) for k in (6, 7)]
            hg, hR = [lerp(lo[k], hi[k], t) for k in (8, 9)]
            wedge = R - b
            Fw_slope = (-bprime*(fg+fR) + wedge*(hi[6]+hi[7]-lo[6]-lo[7])/h)/2
            F_wedge += du * g * Fw_slope
            Fw = wedge*(fg+fR)/2
            Fw_abs = (wedge*(abs(fg)+abs(fR))/2).abs_upper()
            # J varies only over the small radial wedge. Use a centered J.
            wedge_J = J(radial(a + delta*y.lower(), R)).union(J(b))
            Jmid = wedge_J.mid()
            JF += du * g * (
                lo[2].union(hi[2]) + Jmid * Fw + symmetric((wedge_J-Jmid)*Fw_abs)
            )
            H += du * g * (lerp(lo[1], hi[1], t) + wedge*(hg+hR)/2)
        fw_abs = wedge_max*max(abs(x).upper() for x in [*lo[6:8], *hi[6:8]])
        hw_abs = wedge_max*max(abs(x).upper() for x in [*lo[8:10], *hi[8:10]])
        all_F = max(all_F, (max(lo[3].upper(), hi[3].upper())+fw_abs).upper())
        all_H = max(all_H, (max(lo[4].upper(), hi[4].upper())+hw_abs).upper())
    # Z-g solves the residual Volterra equation. In L1(du) of sup_v,
    # ||Z-g|| <= (exp(MD)-1)||g||_1. Also ||Z_v||_{L1(D_c)}
    # <= (exp(MD)-1)||g||_1 with the lower bound on each ray kept.
    gL1 = arb("0.271373" if packet else "0.000791631")  # directed GOAL-014 baseline
    dual_error = ((M*D).exp()-1) * gL1 * (M*all_F+all_H+all_G)/4
    candidate = (F_prim+F_test+F_wedge-JF-H)/4 + symmetric(dual_error)
    b_d = graph(d)
    end_F = rows[-1][0] + (R-b_d)*(rows[-1][6]+rows[-1][7])/2
    g_end = (arb("0.0057047207").union(arb("0.0097549333")) if packet
             else arb("0.4125472765").union(arb("0.4125472991")))
    terminal = g_end * end_F / 4
    print(f"Python {platform.python_version()}, NumPy {np.__version__}, "
          f"python-flint {flint.__version__}, {bits} bits, {subcells} u subcells")
    for name, value in (
        ("terminal F ray", end_F), ("terminal projection", terminal),
        ("F integration by parts including terminal", (F_prim+F_test+F_wedge)/4),
        ("first dual F correction", -JF/4), ("leading H correction", -H/4),
        ("dual remainder radius", dual_error.upper()),
        ("uniform ray |F|", all_F), ("uniform ray |H|", all_H), ("uniform |G|", all_G),
        ("complete weak candidate interval", candidate),
        ("omitting terminal control", candidate-terminal),
    ):
        print(f"{name}: {value.lower().str(14)} {value.upper().str(14)}")
    print("Physical current mismatch remains outside this candidate enclosure.")
    if packet:
        assert candidate < 0
    else:
        assert candidate > arb("7.80")
        assert candidate-terminal < 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate")
    parser.add_argument("--subcells", type=int, default=32)
    parser.add_argument("--bits", type=int, default=160)
    parser.add_argument("--packet", action="store_true")
    args = parser.parse_args()
    calculate(args.candidate, args.subcells, args.bits, args.packet)
