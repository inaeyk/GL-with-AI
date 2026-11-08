"""Empirical late weak moments. No physical residual certificate is implied."""
import argparse

import numpy as np
from numba import njit
from scipy.integrate import cumulative_trapezoid, simpson
from scipy.interpolate import CubicSpline


Q_POLE = complex(-0.177924631, -0.747343369)
S_POLE = Q_POLE + 2 * 0.06328119
Q_RESIDUE = complex(0.13497, -0.370625)
S_RESIDUE = complex(-0.193535, -0.424625)
# Rounded-down directed values from witness_boxes.py.
REJECTION_LOWER = (0.1702965, 0.5102615)


def templates(u):
    q = 2 * Q_POLE * Q_RESIDUE * np.exp(Q_POLE * u)
    star = 2 * S_POLE * S_RESIDUE * np.exp(S_POLE * u)
    return np.stack(((q).real, (q + star).real)), np.stack(
        ((Q_POLE * q).real, (Q_POLE * q + S_POLE * star).real)
    )


def test(u, which, norm):
    """Sine-tapered midpoint template and its interior u derivative."""
    length = u[-1]
    u_pos = np.maximum(0, u)
    modes, derivatives = templates(u_pos)
    taper = np.sin(np.pi * u_pos / length)
    taper_prime = np.pi * np.cos(np.pi * u_pos / length) / length
    h = taper * modes[which] / norm
    hp = (taper_prime * modes[which] + taper * derivatives[which]) / norm
    h[(u <= 0) | (u == length)] = 0.0
    hp[u < 0] = 0.0
    return h, hp


@njit
def adjoint(u, v, W, g):
    """Backward diamond discretization of Z_uv = -W Z.

    Each evaluated cell lies on the causal side of the graph. The graph
    is at the anti-diagonal of the supplied mesh. No interpolation across it
    is used by this backward solve.
    """
    n, m = W.shape
    z = np.zeros((n, m))
    z[:, -1] = g
    for i in range(n - 2, -1, -1):
        for j in range(m - 2, n - 2 - i, -1):
            t = (u[i + 1] - u[i]) * (v[j + 1] - v[j]) / 4
            rhs = z[i, j + 1] + z[i + 1, j] - z[i + 1, j + 1]
            rhs -= t * (
                W[i, j + 1] * z[i, j + 1]
                + W[i + 1, j] * z[i + 1, j]
                + W[i + 1, j + 1] * z[i + 1, j + 1]
            )
            z[i, j] = rhs / (1 + t * W[i, j])
    return z


def integrate_rows(field, u, v):
    rows = np.empty(len(u))
    for i in range(len(u)):
        j = len(u) - 1 - i
        rows[i] = simpson(field[i, j:], x=v[j:])
    return rows


def calculate(cap_path, volume_path):
    with np.load(cap_path) as raw_cap, np.load(volume_path) as vol:
        u, v, V, F, G, H, w = (vol[k] for k in ("u", "v", "V", "F", "G", "H", "w"))
        p = raw_cap["p_shear_infty"]
        assert np.array_equal(raw_cap["u_full"], u)
    late = u >= 0
    s = np.linspace(0, u[-1], 16385)
    candidates = CubicSpline(u[late], p[late])(s)
    for k, name in enumerate(("Q", "Q+S*")):
        g0, _ = test(s, k, 1.0)
        norm = np.sqrt(simpson(g0**2, x=s))
        g, gp = test(u, k, norm)
        g_dense, gp_dense = test(s, k, norm)
        g_early = g[: np.flatnonzero(late)[0] + 1]
        assert np.all(g_early == 0.0)
        gm = simpson(g_dense * templates(s)[0][k], x=s)
        p_fit = simpson(g_dense * candidates, x=s)
        z = adjoint(u, v, V / 4, g)
        wz = V * z / 4
        zu = gp[:, None] - cumulative_trapezoid(
            wz[:, ::-1], x=v[::-1], axis=1, initial=0
        )[:, ::-1]
        zv = -cumulative_trapezoid(wz[::-1], x=u[::-1], axis=0, initial=0)[::-1]
        fg = integrate_rows(zu * F + zv * G + z * H, u, v)
        dual = simpson(g * G[:, -1] - fg, x=u) / 4
        rf = integrate_rows(F, u, v)
        rh = integrate_rows(H + V * w, u, v)
        primal = simpson((-gp * rf + g * (G[:, -1] - rh))[late], x=u[late]) / 4
        reserve = REJECTION_LOWER[k] - p_fit
        print(f"{name}: ||g'||2={np.sqrt(simpson(gp_dense**2, x=s)):.10f}, "
              f"<g,Pmid>={gm:+.10f}")
        print(f"  empirical moments: shear candidate={p_fit:+.10f}, "
              f"primal cap={primal:+.10f}, dual cap={dual:+.10f}")
        print(f"  provisional absolute physical moment-error allowance: {reserve:+.10f}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("cap")
    parser.add_argument("volume")
    args = parser.parse_args()
    calculate(args.cap, args.volume)
