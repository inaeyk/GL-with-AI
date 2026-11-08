"""Exploratory fixed-background early-strip solve, in units r0=1.

Float64 convergence study. This is neither an interval solver nor a
production Einstein evolution. Run with PYTHONDONTWRITEBYTECODE=1.
"""
import argparse
from pathlib import Path
import sys

import numpy as np
from scipy.integrate import cumulative_simpson, cumulative_trapezoid, quad, simpson, solve_ivp
from scipy.interpolate import CubicSpline
from scipy.special import wrightomega

sys.path.insert(0, str(Path(__file__).resolve().parents[2]
                       / "GLR-GOAL-011/checks/physical_overlap"))
from overlap_gl import GLProfile, K, OMEGA, tortoise
from overlap_mean import MeanGeometry

LAM, R, DELTA = 4, 13 / 8, 3 / 25
FRONT = -R - 2 * np.log(R - 1)


def radius(x):
    return 1 + wrightomega(x - 1)


def packet(y):
    """The normalizer makes the direct outgoing news have unit L2 norm."""
    with np.errstate(divide="ignore", invalid="ignore", under="ignore"):
        p = y * (1 - y)
        j = (1 - 2 * y) / p**2
        dj = -2 / p**2 - 2 * (1 - 2 * y)**2 / p**3
        b = np.exp(-1 / p)
        jets = [b, b * j, b * (j**2 + dj)]
    return [np.where((0 < y) & (y < 1), value, 0.0) for value in jets]


def ray_integral(z, x, axis, rule):
    if len(x) == 1:
        return np.zeros_like(z)
    if rule == "simpson" and len(x) > 2:
        return cumulative_simpson(z, x=x, axis=axis, initial=0.0)
    return cumulative_trapezoid(z, x=x, axis=axis, initial=0.0)


class Strip:
    def __init__(self, width, cap, n, m, rule):
        self.u = FRONT + np.linspace(0, width, n + 1)
        self.b = 1 + 2 * wrightomega(-(self.u + 1) / 2 - np.log(2))
        vmax = self.u[-1] + 2 * tortoise(cap)
        rmax = radius((vmax - FRONT) / 2)
        rout = R + np.expm1(np.linspace(0, np.log1p(rmax - R), m + 1))
        self.v = np.r_[self.b[:0:-1], FRONT + 2 * tortoise(rout)]
        self.n, self.rule = n, rule
        self.r = radius((self.v[None, :] - self.u[:, None]) / 2)
        self.f = 1 - 1 / self.r
        self.mask = np.arange(n + 1)[:, None] + np.arange(len(self.v)) >= n
        self.graph = np.arange(n + 1), np.arange(n, -1, -1)

    def vi(self, z):
        """Integral in v from the graph, along a fixed outgoing ray."""
        n = self.n
        out = np.zeros_like(z)
        for j in range(1, n + 1):
            out[j, n-j:n+1] = ray_integral(z[j, n-j:n+1], self.v[n-j:n+1],
                                           0, self.rule)
        out[:, n:] = (out[:, n:n+1] +
                      ray_integral(z[:, n:], self.v[n:], 1, self.rule))
        return out

    def ui(self, z):
        """Integral in u from the graph or front, along a fixed ingoing ray."""
        n = self.n
        out = np.zeros_like(z)
        for j in range(1, n):
            out[n-j:, j] = ray_integral(z[n-j:, j], self.u[n-j:], 0, self.rule)
        out[:, n:] = ray_integral(z[:, n:], self.u, 0, self.rule)
        return out

    def march(self, q, pu, pv, potential, source, iterations, skew=None):
        """Solve by strong characteristic ray integrals. Last axis is field."""
        q = q[:, None, :]
        pu = pu[:, None, :]
        pv = np.concatenate((pv[::-1], np.zeros((len(self.v)-self.n-1,
                                                 pv.shape[-1]))))[None, :, :]
        active = self.mask[:, :, None]
        p, h, w = pu * active, pv * active, q * active
        for _ in range(iterations):
            drive = -potential * w - source
            if skew is not None:
                p_t = p + h
                drive += np.stack((-skew * p_t[..., 1], skew * p_t[..., 0]), axis=2)
            drive = np.where(active, drive / 4, 0)
            p = (pu + self.vi(drive)) * active
            h = (pv + self.ui(drive)) * active
            w = (q + self.vi(h)) * active
        return w, p, h

    def weak_return(self, F, G, H, V, iterations):
        """Exact-form weak return discretization, with graph cancellation."""
        ray_F = self.vi(F)
        base = (ray_F + self.ui(G - self.vi(H))) / 4
        w = base.copy()
        for _ in range(iterations):
            w = base - self.ui(self.vi(V * w)) / 4
        return w, ray_F


def static_geometry(s, tol, outer):
    """Evaluate regular EF mode and fixed-mass integral on the actual strip."""
    gl = GLProfile(2.5, 1.0 + 0j, outer - 2.5, 0.5, tol)
    mean = MeanGeometry(gl, 2.5, 1.0, outer - 2.5, tol)
    r, f = s.r, s.f
    left = r <= gl.rmatch
    n = np.empty_like(r)
    nr = np.empty_like(r)
    n[left], nr[left] = gl.left.sol(r[left])
    p, log_u = gl.right.sol(r[~left] - gl.rmatch).real
    n[~left] = gl.n_match * np.exp(log_u - gl.log_match.real -
                                  gl.omega * (tortoise(r[~left]) - tortoise(gl.rmatch)))
    nr[~left] = (p - gl.omega) * n[~left] / f[~left]
    q, p, dq, dp = gl.ode(r, K**2, OMEGA)
    nrr = q * n + p * nr
    nrrr = dq * n + (q + dp) * nr + p * nrr
    jets = gl.scale * np.asarray(gl.ef(r, K**2, OMEGA, n, nr, nrr, nrrr))
    h, dh, _ = jets
    time = np.exp(OMEGA * s.v)[None, :]

    def block(ef):
        a, b, c = ef[:3]
        return np.stack((a, a + f*b, a + 2*f*b + f*f*c), axis=-1)

    ht = time[..., None] * block(h)
    correction = np.stack((np.zeros_like(r), h[1], 2 * (h[1] + f*h[2])), axis=-1)
    hx = OMEGA*ht + time[..., None] * f[..., None] * (
        block(dh) + correction / r[..., None]**2)
    d = time * h[3] / (2*r*r)
    efmean = np.empty((2,) + r.shape)
    efmean[:, left] = mean._real_solution.sol(r[left]).real
    efmean[:, ~left] = mean._ray_solution.sol(r[~left] - gl.rmatch).real
    mu, beta = efmean * time**2
    hmean = np.stack((2*mu/r-2*f*beta, 2*mu/r-f*beta, 2*mu/r), axis=-1)
    print(f"GL log-derivative mismatch={gl.match_error:.3e}; "
          f"mean EF finite-end proxy max={max(mean.tail_proxy_ef):.3e}", flush=True)
    return ht, hx, d, hmean, beta, gl


def mat(matrix, vector):
    return np.einsum("...ij,...j->...i", matrix, vector)


def orbit(h, f):
    """Endomorphism b0^-1 h with h=(h_tt,h_tx,h_xx)."""
    a, b, c = np.moveaxis(h, -1, 0)
    return (np.stack((-a, -b, b, c), axis=-1)
            .reshape(f.shape + (2, 2)) / f[..., None, None])


def geometry_mass(X):
    """Right multiply by eta=diag(-1,1)."""
    return X * np.array([-1, 1])


def point_velocities(gl, n):
    """Binary graph-velocity ordinates for the early-strip candidate."""
    from scipy.special import wrightomega

    u = FRONT + DELTA*np.linspace(0, 1, n+1)/8
    b = 1 + 2*wrightomega(-(1+u)/2 - np.log(2))
    norm = np.sqrt(quad(lambda y: packet(y)[1]**2, 0, 1,
                        epsabs=1e-14, epsrel=1e-13)[0])
    val, slope, _ = packet((u - FRONT)/DELTA)
    a, ap = val*np.sqrt(DELTA)/norm, slope/(np.sqrt(DELTA)*norm)
    f, p = 1 - 1/b, -1/b
    at, ax = (f*a/b-ap)/(2*b), ap/(2*b)
    hh = np.array([gl(r) for r in b]).real.T
    ht, htx, hx = np.exp(OMEGA*b) * np.array([hh[0], hh[0]+f*hh[1],
                                               hh[0]+2*f*hh[1]+f*f*hh[2]])
    jt = -LAM*b*b*(ht*at-htx*ax)/(2*f)
    jx = -LAM*b*b*(-htx*at+hx*ax)/(2*f)
    return np.array([(p*jt-jx)/(1-p*p), K*(jt-p*jx)/(LAM*b*b*(1-p*p))]).T


def calculate(n, m, cap, iterations, rule, tol, outer, dump):
    s = Strip(DELTA / 8, cap, n, m, rule)
    r, f, graph = s.r, s.f, s.graph
    p_graph = -1 / s.b
    y = (s.u - FRONT) / DELTA
    norm = np.sqrt(quad(lambda z: packet(z)[1]**2, 0, 1,
                        epsabs=1e-14, epsrel=1e-13)[0])
    a, ap, app = [DELTA**(0.5-j) * v / norm for j, v in enumerate(packet(y))]
    V = f * (6/r**2 - 3/r**3)
    zeros = np.zeros_like(a)
    data = np.stack((a, ap), axis=-1)
    wtt_g = app - V[graph]*a/(1-p_graph**2)
    wtx_g = -app + p_graph*V[graph]*a/(1-p_graph**2)
    pu = np.stack((ap, (wtt_g-wtx_g)/2), axis=-1)
    pv = np.stack((zeros, (wtt_g+wtx_g)/2), axis=-1)
    base, base_u, base_v = s.march(data, pu, pv, V[..., None], 0, iterations)
    w0, wt = np.moveaxis(base, -1, 0)
    wx = base_v[..., 0] - base_u[..., 0]
    wtt = base_u[..., 1] + base_v[..., 1]
    wtx = base_v[..., 1] - base_u[..., 1]
    wxx = wtt + V*w0
    A0 = np.stack((wx + f*w0/r, wt), axis=-1) / (2*r[..., None])
    A0t = np.stack((wtx + f*wt/r, wtt), axis=-1) / (2*r[..., None])
    A0x = (np.stack((wxx + f*wx/r + f*(1/r**3-f/r**2)*w0, wtx),
                    axis=-1) / (2*r[..., None]) - (f/r)[..., None]*A0)

    ht, hx, d, hm, beta, gl = static_geometry(s, tol, outer)
    Xb = orbit(ht, f)
    Xbx = orbit(hx, f) - Xb/r[..., None, None]**2
    M1 = -geometry_mass(Xb)
    M1x = -geometry_mass(Xbx)
    weight = LAM*r[..., None]**2 / 2
    J = weight * mat(M1, A0)
    Jt = OMEGA * J + weight * mat(M1, A0t)
    Jx = (2*f/r)[..., None]*J + weight*(mat(M1, A0x) + mat(M1x, A0))
    Q0 = LAM*r*w0/2
    X = 3*d*Q0
    dk = LAM + K*K*r*r
    rootD, rootW = r*np.sqrt(dk), np.sqrt(LAM)*r*r/np.sqrt(dk)
    Lc = r*r / dk
    a_m = f * (1/r + K*K*r/dk)
    b_m = f * (2/r - K*K*r/dk)
    common = f * (K*K + LAM/r**2)
    ax = a_m/r**2 + f*f*(-1/r**2 + K*K/dk - 2*K**4*r**2/dk**2)
    bx = b_m/r**2 + f*f*(-2/r**2 - K*K/dk + 2*K**4*r**2/dk**2)
    mixed_V = np.stack((common+a_m**2-ax, common+b_m**2+bx), axis=-1)
    mixed_S = np.stack(((Jt[..., 1]+Jx[..., 0]-2*a_m*J[..., 0])/rootD
                        + f*rootD*X/r**4,
                        -K*Lc*(Jt[..., 0]+Jx[..., 1]+(2*f/r-2*f*K*K*r/dk)*J[..., 1])/rootW),
                       axis=-1)
    raw_u_tg, raw_z_tg = point_velocities(gl, n).T
    mix_tg = np.stack((raw_u_tg/rootD[graph], raw_z_tg*rootW[graph]), axis=-1)
    mix, mix_u, mix_v = s.march(
        np.zeros_like(mix_tg), (1+p_graph[:, None])*mix_tg/2,
        (1-p_graph[:, None])*mix_tg/2, mixed_V, mixed_S, iterations,
        skew=2*K*np.sqrt(LAM)*f/dk)
    ym, Zm = np.moveaxis(mix, -1, 0)
    ymt, Zmt = np.moveaxis(mix_u+mix_v, -1, 0)
    ymx, Zmx = np.moveaxis(mix_v-mix_u, -1, 0)
    U, Ut, Ux = rootD*ym, rootD*ymt, rootD*(ymx+a_m*ym)
    Zt, Zx = Zmt/rootW, (Zmx-b_m*Zm)/rootW
    A1 = np.stack((J[..., 0]-Ux+K*r**4*Zt,
                   -J[..., 1]-Ut+K*r**4*Zx), axis=-1)/rootD[..., None]**2
    # Mean mixed term uses 2*Re of a +k coefficient paired with its conjugate.
    Xb2 = Xb @ Xb
    tr = np.trace(Xb2, axis1=-2, axis2=-1)
    M2_direct = geometry_mass((Xb2 - (2*d**2+tr/4)[..., None, None]*np.eye(2))/2)
    M2_mean = geometry_mass(beta[..., None, None]*np.eye(2) - orbit(hm, f))
    densities = [
        (mat(M1, A1), -6*d*(U + X)),
        (mat(M2_direct, A0), (7*d*d + tr/8)*Q0),
        (mat(M2_mean, A0), -beta*Q0),
    ]
    g, gprime = base_u[:, -1, 0], base_u[:, -1, 1]
    # g_u differs from (w_t)_u by -w_uv, which equals V*w/4.
    gprime = gprime + V[:, -1]*w0[:, -1]/4
    print(f"u=[{s.u[0]:.15g},{s.u[-1]:.15g}] cap={cap:g} rule={rule} "
          f"Nu={n} Nv_exterior={m} iterations={iterations}", flush=True)

    total = np.zeros((3,) + r.shape)
    for name, (density, curl) in zip(("mixed", "direct", "mean"), densities):
        Dt, Dx = LAM*np.moveaxis(density, -1, 0)
        currents = np.stack(((2*r/LAM)*(Dx-Dt), (2*r/LAM)*(Dx+Dt),
                             (2*f/LAM)*Dt - 2*f*curl/r**3))
        total += currents
        diagnose_return(s, V, currents, g, gprime, iterations, name)
    w, news = diagnose_return(s, V, total, g, gprime, iterations, "total")
    print(f"cap news norm={np.sqrt(simpson(news**2, x=s.u)):.12g}; "
          f"g norm={np.sqrt(simpson(g*g,x=s.u)):.12g}; "
          f"g(d)={g[-1]:.12g}; w(d,cap)={w[-1,-1]:.12g}; "
          f"peak mixed y={np.max(np.abs(ym)):.8g}; peak mixed Z={np.max(np.abs(Zm)):.8g}",
          flush=True)
    assert np.max(np.abs(A1[graph][..., 1] + p_graph*A1[graph][..., 0])) < 1e-12

    # A second discretization differentiates the currents before the scalar
    # march and includes the canonical graph velocity explicitly.
    F, G, H = total
    Fu, Gv = np.zeros_like(F), np.zeros_like(G)
    Fu[:, n:] = CubicSpline(s.u, F[:, n:], axis=0)(s.u, 1)
    for j in range(1, n):
        Fu[n-j:, j] = CubicSpline(s.u[n-j:], F[n-j:, j])(s.u[n-j:], 1)
    Fu[n, 0] = Fu[n, 1]  # Single graph corner has no fixed-v forward interval.
    for j in range(n + 1):
        Gv[j, n-j:] = CubicSpline(s.v[n-j:], G[j, n-j:])(s.v[n-j:], 1)
    bprime = -(1+p_graph)/(1-p_graph)
    graph_u = (G[graph] - bprime*F[graph])/4
    _, pu, _ = s.march(np.zeros((n+1, 1)), graph_u[:, None],
                      (-graph_u/bprime)[:, None], V[..., None],
                      (-Fu-Gv+H)[..., None], iterations)
    pstrong = pu[:, -1, 0]
    print(f"strong separate P={simpson(g*pstrong, x=s.u):.12g}; "
          f"strong-vs-shear L2={np.sqrt(simpson((pstrong-news)**2, x=s.u)):.9g}; "
          f"max cap G={np.max(np.abs(G[:, -1])):.9g}", flush=True)
    no_graph = np.zeros((n+1, 1))
    _, no_graph_p, _ = s.march(no_graph, no_graph, no_graph, V[..., None],
                               (-Fu-Gv+H)[..., None], iterations)
    print(f"wrong zero-upper-velocity control P={simpson(g*no_graph_p[:, -1, 0], x=s.u):.12g}; "
          f"max ray |F|={np.max(s.vi(np.abs(F))[:, -1]):.9g}; "
          f"max ray |H|={np.max(s.vi(np.abs(H))[:, -1]):.9g}; "
          f"max |G|={np.max(np.abs(G[s.mask])):.9g}; "
          f"terminal ray |F|={s.vi(np.abs(F))[-1, -1]:.9g}", flush=True)
    candidate_pair = continue_homogeneous(s, cap, np.stack((g, pstrong), axis=-1))
    gi, pi = candidate_pair.T
    print(f"zero-exterior-source continuation P={simpson(gi*pi, x=s.u):.12g}; "
          f"continued return norm={np.sqrt(simpson(pi*pi, x=s.u)):.12g}", flush=True)
    if dump:
        np.savez(dump, u=s.u, v=s.v, p_cap=pstrong, g_cap=g, g_infty=gi,
                 p_infty=pi, F=F, G=G, H=H, y=ym, Z=Zm, w=w)


def continue_homogeneous(s, cap, news_pair):
    """Empirical potential-only characteristic continuation to 1/r_d=0."""
    def rhs(rinv, field):
        field = field.reshape(news_pair.shape)
        if rinv == 0:
            coefficient = np.full_like(s.u, 3)
        else:
            rd = 1/rinv
            r = radius(tortoise(rd) + (s.u[-1]-s.u)/2)
            potential = (1-1/r)*(6/r**2-3/r**3)
            coefficient = potential/(2*(1-rinv)*rinv**2)
        primitive = ray_integral(field, s.u, 0, s.rule)
        return (coefficient[:, None]*primitive).ravel()

    result = solve_ivp(rhs, (1/cap, 0), news_pair.ravel(), rtol=3e-13,
                       atol=2e-14, method="DOP853")
    assert result.success
    return result.y[:, -1].reshape(news_pair.shape)


def transpose_pair(s, V, base, g, iterations):
    """Apply the Euclidean transpose of the discrete Volterra return."""
    n, nu, nv = s.n, len(s.u), len(s.v)

    def matrix(x):
        return ray_integral(np.eye(len(x)), x, 0, s.rule)

    vm = matrix(s.v[n:])
    vg = [matrix(s.v[n-j:n+1]) for j in range(n+1)]
    um = matrix(s.u)
    ug = [matrix(s.u[n-j:]) for j in range(n)]

    def vt(f):
        result = np.zeros_like(f)
        result[:, n:] = f[:, n:] @ vm
        tail_weight = f[:, n:].sum(axis=1)
        for j in range(1, nu):
            weights = np.r_[f[j, n-j:n], tail_weight[j]]
            result[j, n-j:n+1] += vg[j].T @ weights
        return result

    def ut(f):
        result = np.zeros_like(f)
        result[:, n:] = um.T @ f[:, n:]
        for j in range(1, n):
            result[n-j:, j] = ug[j].T @ f[n-j:, j]
        return result

    wu = simpson(np.eye(nu), x=s.u, axis=0)
    cap = np.zeros((nu, nv))
    cap[:, -1] = wu*g
    q = -V * vt(cap) / 4
    term = q.copy()
    accum = q.copy()
    for _ in range(iterations):
        term = -V * vt(ut(term)) / 4
        accum += term
    return np.sum(accum * base)


def diagnose_return(s, V, currents, g, gprime, iterations, name):
    F, G, H = currents
    w, rayF = s.weak_return(F, G, H, V, iterations)
    cap_HVW = s.vi(H + V*w)[:, -1]
    terminal = (g[-1]*rayF[-1, -1]-g[0]*rayF[0, -1])/4
    weak = terminal + simpson(-gprime*rayF[:, -1] + g*(G[:, -1]-cap_HVW), x=s.u)/4
    cap_news = CubicSpline(s.u, w[:, -1])(s.u, 1)
    direct = simpson(g*cap_news, x=s.u)
    print(f"{name:6s} P_weak={weak:.12g} P_shear={direct:.12g} "
          f"terminal={terminal:.12g} F_ray(d)={rayF[-1,-1]:.12g}", flush=True)
    if name == "total":
        base = (rayF+s.ui(G-s.vi(H)))/4
        dual = (terminal+simpson(-gprime*rayF[:, -1]+g*(G[:, -1]-s.vi(H)[:, -1]),
                                 x=s.u)/4+transpose_pair(s, V, base, g, iterations))
        assert abs(dual-weak) < 2e-12
        print(f"discrete transpose P={dual:.12g}; weak-vs-transpose={abs(dual-weak):.9g}",
              flush=True)
    return w, cap_news


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--n", type=int, default=64)
    parser.add_argument("--m", type=int, default=240)
    parser.add_argument("--cap", type=float, default=40)
    parser.add_argument("--iterations", type=int, default=12)
    parser.add_argument("--rule", choices=["trapezoid", "simpson"], default="simpson")
    parser.add_argument("--tol", type=float, default=2e-12)
    parser.add_argument("--outer", type=float, default=450)
    parser.add_argument("--dump", help="Optional .npz path for finite-cap candidate arrays")
    args = parser.parse_args()
    calculate(args.n, args.m, args.cap, args.iterations, args.rule, args.tol, args.outer, args.dump)
