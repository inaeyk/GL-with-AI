"""Empirical coefficient sizes for the conditional rotated weak estimator.

Uses GOAL-014 central GL and fixed-mass mean and samples the causal strip.
The printed quantities are diagnostics, since sampled maxima and ordinary
quadrature are used in place of directed coefficient-cell enclosures.
"""
from pathlib import Path
from types import SimpleNamespace
import sys

import numpy as np
from scipy.integrate import cumulative_trapezoid, quad, trapezoid
from scipy.special import wrightomega

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-014/checks"))
from c_compact import (DELTA, FRONT, K, LAM, OMEGA, R, geometry_mass,
                       mat, orbit, packet, static_geometry, tortoise)

KAPPA, KAPPA2 = 522, 380


def norm(z):
    return np.linalg.norm(z, axis=-1)


def coefficients(r, f, v):
    ht, hx, diam, hm, mean_beta, _ = static_geometry(
        SimpleNamespace(r=r, f=f, v=v), 8e-14, 430)
    xb = orbit(ht, f)
    xb_x = orbit(hx, f) - xb / r[..., None, None]**2
    first = -geometry_mass(xb)
    first_x = -geometry_mass(xb_x)
    xb2 = xb @ xb
    trace = np.trace(xb2, axis1=-2, axis2=-1)
    direct = geometry_mass((xb2 - (2*diam**2 + trace/4)[..., None, None]*np.eye(2))/2)
    mean = geometry_mass(mean_beta[..., None, None]*np.eye(2) - orbit(hm, f))
    d = LAM + K*K*r*r
    rootd = np.sqrt(d)
    alpha, beta = 1/(r*rootd), K/(np.sqrt(LAM)*rootd)
    a_m = f*(1/r + K*K*r/d)
    b_m = f*(2/r - K*K*r/d)
    ax = a_m/r**2 + f*f*(-1/r**2 + K*K/d - 2*K**4*r**2/d**2)
    bx = b_m/r**2 + f*f*(-2/r**2 - K*K/d + 2*K**4*r**2/d**2)
    k_eff = f*(K*K + LAM/r**2)
    w_eff = np.stack((k_eff+a_m*a_m-ax, k_eff+b_m*b_m+bx), axis=-1)
    omega = K*np.sqrt(LAM)*f/(2*d)
    omegav = omega*(1/(2*r*r)-f*K*K*r/d)
    rotation = np.stack((np.sqrt(LAM)/rootd, -K*r/rootd,
                         K*r/rootd, np.sqrt(LAM)/rootd), axis=-1).reshape(r.shape+(2, 2))
    jq = np.array([[0., -1.], [1., 0.]])
    skew = omega[..., None, None]*jq
    wrot = np.swapaxes(rotation, -2, -1) @ (w_eff[..., :, None]*rotation)
    pm = wrot/4 + omega[..., None, None]**2*np.eye(2)
    mplus = pm + omegav[..., None, None]*jq
    mminus = pm - omegav[..., None, None]*jq

    a0 = np.broadcast_to(np.array([[-1, 1, 0], [1, 1, 0]]), r.shape+(2, 3)).copy().astype(float)
    a0[..., 0, 2] = f/r
    a0x = np.zeros_like(a0)
    a0x[..., 0, 2] = f*(1/r**3-f/r**2)
    j = (LAM*r/4)[..., None, None]*(first @ a0)
    jx = (f/r)[..., None, None]*j + (LAM*r/4)[..., None, None]*(first_x @ a0 + first @ a0x)
    ju, jv = (OMEGA*j-jx)/2, (OMEGA*j+jx)/2
    jd, je = j[..., 1, :]-j[..., 0, :], j[..., 1, :]+j[..., 0, :]
    jdu, jev = ju[..., 1, :]-ju[..., 0, :], jv[..., 1, :]+jv[..., 0, :]
    n = np.stack((alpha, beta), axis=-1)
    m = np.stack((alpha, -beta), axis=-1)
    lower = np.stack((-2*a_m[..., None]*alpha[..., None]*j[..., 0, :],
                      -beta[..., None]*(2*f/r-2*f*K*K*r/d)[..., None]*j[..., 1, :]), axis=-2)
    lower[..., 0, 2] += f*(r*rootd)*(3*diam)*(LAM*r/2)/r**4
    source = np.stack((jd[..., 0, None]*n,
                       jd[..., 1, None]*n + je[..., 0, None]*m,
                       je[..., 1, None]*m,
                       (jdu[..., 0, None]+jd[..., 2, None])*n+jev[..., 0, None]*m+lower[..., :, 0],
                       jdu[..., 1, None]*n+(jev[..., 1, None]+je[..., 2, None])*m+lower[..., :, 1],
                       jdu[..., 2, None]*n+jev[..., 2, None]*m+lower[..., :, 2]), axis=-1)

    du = np.stack((alpha, beta, -alpha, -beta), axis=-1).reshape(r.shape+(2, 2))
    dv = np.stack((-alpha, beta, -alpha, beta), axis=-1).reshape(r.shape+(2, 2))
    dx = np.stack((-alpha*a_m, np.zeros_like(r), np.zeros_like(r), -beta*b_m),
                  axis=-1).reshape(r.shape+(2, 2))
    ap, aq, ax = du@rotation, dv@rotation, (du+dv)@rotation@skew+dx@rotation
    ab = j*np.array([[1.], [-1.]])/(r*r*d)[..., None, None]
    current_y = first @ np.concatenate((ap, aq, ax), axis=-1)
    current_b = first @ ab + (direct+mean) @ (a0/(2*r)[..., None, None])
    curl_y = np.zeros(r.shape+(6,))
    curl_y[..., 4:6] = (-6*diam*r*rootd)[..., None]*rotation[..., 0, :]
    curl_b = np.zeros(r.shape+(3,))
    curl_b[..., 2] = (-18*diam**2 + 7*diam**2 + trace/8 - mean_beta)*LAM*r/2
    fg = np.array([[-1., 1.], [1., 1.]])

    def currents(rows, curl):
        return np.concatenate(((2*r)[..., None, None]*(fg @ rows),
                               (2*f)[..., None, None]*rows[..., 0:1, :]
                               - (2*f/r**3)[..., None, None]*curl[..., None, :]), axis=-2)

    y = currents(current_y, curl_y)
    base = currents(current_b, curl_b)
    optical = mat(np.swapaxes(rotation, -1, -2), source[..., :, 0])[..., 0]
    assert np.max(np.abs(y[..., 0, 1])) < 1e-11
    assert np.max(np.abs(y[..., 0, 0] + 8*optical/LAM)) < 1e-11

    p = -1/r
    ytime = np.stack(((p[..., None]*j[..., 0, :]-j[..., 1, :])*alpha[..., None],
                      (j[..., 0, :]-p[..., None]*j[..., 1, :])*beta[..., None]), axis=-2)
    # beta = k sqrt(W)/(lambda r^2)
    ytime /= (1-p*p)[..., None, None]
    return (optical, source, y, base, mplus, mminus,
            (1+p)[..., None, None]*ytime/2, (1-p)[..., None, None]*ytime/2)


def run(panels, cuts):
    width = DELTA/8
    u = np.linspace(FRONT, FRONT+width, cuts+1)
    b = 1+2*wrightomega(-(u+1)/2-np.log(2))
    rc = b[-1]+np.expm1(np.linspace(0, np.log1p(400-b[-1]), panels+1))
    v = np.union1d(b, u[-1]+2*tortoise(rc))
    r = 1+wrightomega((v[None, :]-u[:, None])/2-1)
    f = 1-1/r
    graph = np.arange(cuts+1), np.searchsorted(v, b)
    q0, source, mix, base, mp, mm, pg, qg = coefficients(r, f, v)
    active = v[None, :] >= b[:, None]
    q0 = np.where(active, q0, 0)
    primitive = cumulative_trapezoid(q0, v, axis=-1, initial=0)
    popt = np.max(np.abs(primitive), axis=0)/4
    u00 = np.array([.004147, 1.124e-6, 4.740e-6, 1.003017/KAPPA2,
                    8.181e-7/KAPPA2, 1.003017/(KAPPA*KAPPA2)])
    error_s = np.max(norm(source.swapaxes(-1, -2)) @ u00, axis=0)
    max_s = np.max(norm(source.swapaxes(-1, -2)) @ (u00+np.eye(6)[0]), axis=0)
    graph_w = np.array([1., 0., 1/KAPPA])
    pgmax = np.max(norm(pg[graph].swapaxes(-1, -2)) @ graph_w)
    qgmax = np.max(norm(qg[graph].swapaxes(-1, -2)) @ graph_w)
    mpmax = np.max(np.linalg.norm(mp, ord=2, axis=(-2, -1)), axis=0)
    mmmax = np.max(np.linalg.norm(mm, ord=2, axis=(-2, -1)), axis=0)
    accum = pgmax/KAPPA2 + cumulative_trapezoid(error_s, v, initial=0)/4
    pmax = popt+accum
    for _ in range(16):
        pmax = popt+accum+cumulative_trapezoid(mpmax*pmax/KAPPA2, v, initial=0)
    qmax = (qgmax*(v<=b[0])+max_s/4+mmmax*pmax/KAPPA2)/KAPPA2
    p_err = pmax-popt
    mixmax = np.max(np.stack((norm(mix[..., :2]), norm(mix[..., 2:4]),
                             norm(mix[..., 4:6])), axis=-1), axis=0)
    basemax = np.max(np.abs(base), axis=0)
    fc = (mixmax[..., 0]*np.stack((p_err, pmax, pmax), axis=-1) +
          mixmax[..., 1]*qmax[:, None] + mixmax[..., 2]*(pmax/KAPPA2)[:, None] +
          basemax @ u00[3:])
    ri = trapezoid(fc, v, axis=0)
    active_q = np.abs(primitive[:, -1])/np.sqrt(LAM)
    scalar_V = f*(6/r**2-3/r**3)
    vcap_g = trapezoid(np.max(scalar_V, axis=0)*fc[..., 1]/4, v)
    nrm = np.sqrt(quad(lambda y: packet(y)[1]**2, 0, 1)[0])
    b1d = packet(.125)[1]/(np.sqrt(DELTA)*nrm)
    b2d = packet(.125)[2]/(DELTA**1.5*nrm)
    b2sq = quad(lambda y: packet(y)[2]**2, 0, .125)[0]/(DELTA**2*nrm*nrm)
    print(f"empirical phase abs range: {active_q.min():.12g}, {active_q.max():.12g}")
    print(f"pg/Bprime≤{pgmax:.12g}, qg/Bprime≤{qgmax:.12g}, int S-error/B2 {4*(accum[-1]-pgmax/KAPPA2):.12g}")
    print(f"Bprime(d)={b1d:.12g}, B2(d)={b2d:.12g}, int B2²={b2sq:.12g}")
    print("empirical normalized integrated errors [F-Fopt,G,H]:", ri)
    print(f"weighted G bound int sup(V/4)|G|/B2: {vcap_g:.12g}")
    print(f"diagnostic early terminal F-opt error bound: {ri[0]*b2d:.12g}")
    print(f"diagnostic early F-only projection error bound: {(b1d*b2d+b2sq)*ri[0]/4:.12g}")
    print(f"diagnostic opt projected lower with separate phase extrema: "
          f"{(b1d*b2d*active_q.min()**2-b2sq*active_q.max()**2)/4:.12g}")


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--panels", type=int, default=1200)
    parser.add_argument("--cuts", type=int, default=24)
    options = parser.parse_args()
    run(options.panels, options.cuts)
