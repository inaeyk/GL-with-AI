"""Float64 diagnostic of terminal-ray adjoint coefficient sizes.

The static geometry uses C's empirical central-parameter candidate. This
script does not enclose the GL coefficient, the lower residual or the sign.
"""
import argparse
from types import SimpleNamespace

import numpy as np
from scipy.integrate import cumulative_simpson, simpson
from scipy.special import wrightomega

from c_compact import DELTA, FRONT, K, LAM, geometry_mass, orbit, static_geometry, tortoise


def integral_to_cap(z, v):
    return cumulative_simpson(z[::-1], x=-v[::-1], axis=0, initial=0)[::-1]


def l2(z, v):
    return np.sqrt(simpson(np.sum(z*z, axis=-1), x=v))


def evaluate(cap, panels):
    d = FRONT + DELTA/8
    b = 1+2*wrightomega(-(1+d)/2-np.log(2))
    r = b+np.expm1(np.linspace(0, np.log1p(cap-b), panels+1))
    v = d+2*tortoise(r)
    f = 1-1/r
    ht, _, diam, hm, beta, _ = static_geometry(
        SimpleNamespace(r=r[None, :], f=f[None, :], v=v), 8e-14, 430
    )
    ht, diam, hm, beta = ht[0], diam[0], hm[0], beta[0]
    xb = orbit(ht, f)
    xb2 = xb @ xb
    trace = np.trace(xb2, axis1=-2, axis2=-1)
    first = -geometry_mass(xb)
    direct = geometry_mass((xb2-(2*diam**2+trace/4)[:, None, None]*np.eye(2))/2)
    mean = geometry_mass(beta[:, None, None]*np.eye(2)-orbit(hm, f))
    row = 2*r[:, None]*(first[:, 1, :]-first[:, 0, :])
    lt, lx = row.T

    dk = LAM+K*K*r*r
    invrootD = 1/(r*np.sqrt(dk))
    a_mix = f*(1/r+K*K*r/dk)
    b_mix = f*(2/r-K*K*r/dk)
    a_x = a_mix/r**2+f*f*(-1/r**2+K*K/dk-2*K**4*r**2/dk**2)
    b_x = b_mix/r**2+f*f*(-2/r**2-K*K/dk+2*K**4*r**2/dk**2)
    kc = f*(K*K+LAM/r**2)
    w_eff = np.stack((kc+a_mix**2-a_x, kc+b_mix**2+b_x), axis=-1)
    compact = K/(np.sqrt(LAM)*np.sqrt(dk))
    bu = (lt-lx)[:, None]*np.stack((invrootD, compact), axis=-1)
    bv = (lt+lx)[:, None]*np.stack((-invrootD, compact), axis=-1)
    b0 = np.stack((-lt*a_mix*invrootD, -lx*b_mix*compact), axis=-1)

    sigma = 2*K*np.sqrt(LAM)*f/dk
    theta = cumulative_simpson(sigma/4, x=v, initial=0)
    cos, sin = np.cos(theta), np.sin(theta)
    bu_rotated = np.stack((bu[:, 0]*cos+bu[:, 1]*sin,
                           -bu[:, 0]*sin+bu[:, 1]*cos), axis=-1)
    anti = integral_to_cap(bu_rotated, v)
    dual = np.stack((anti[:, 0]*cos-anti[:, 1]*sin,
                     anti[:, 0]*sin+anti[:, 1]*cos), axis=-1)
    dual_times_c = sigma[:, None]*np.stack((dual[:, 1], -dual[:, 0]), axis=-1)
    j = bv+dual_times_c/4
    k = b0-dual*w_eff/4-j*np.stack((a_mix, -b_mix), axis=-1)/2
    coef = np.hypot(l2(j, v), 2*l2(k/np.sqrt(kc[:, None]), v))

    combined = direct+mean
    base = 2*r[:, None]*(combined[:, 1, :]-combined[:, 0, :])
    base += LAM/(2*dk[:, None])*np.einsum("ij,ijk->ik", row*np.array([1, -1]), first)
    base_u = (-base[:, 0]+base[:, 1])/(2*r)
    base_v = (base[:, 0]+base[:, 1])/(2*r)
    base_0 = f*base[:, 0]/(2*r*r)
    base_dual = integral_to_cap(base_u, v)
    V = f*(6/r**2-3/r**3)
    base_coef = np.hypot(l2(base_v[:, None], v),
                         2*l2(((base_0-base_dual*V/4)/np.sqrt(V))[:, None], v))

    print(f"central candidate, terminal ray r={b:.12g} to {cap:g}, {panels} panels")
    print(f"mixed graph row: {dual[0]} (norm={np.linalg.norm(dual[0]):.12g})")
    print(f"mixed weighted flux multiplier: {coef:.12g}")
    print(f"mixed weighted L1 residual: max |lambda|/4="
          f"{np.max(np.linalg.norm(dual, axis=-1))/4:.12g}")
    print(f"baseline graph multiplier: {base_dual[0]:.12g}")
    print(f"baseline weighted flux multiplier: {base_coef:.12g}")
    print(f"baseline weighted L1 residual: max |lambda|/4={np.max(np.abs(base_dual))/4:.12g}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cap", type=float, default=400)
    parser.add_argument("--panels", type=int, default=1440)
    args = parser.parse_args()
    evaluate(args.cap, args.panels)
