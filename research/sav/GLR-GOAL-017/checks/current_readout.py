"""Directed readout and directional scale for a bilinear late current lift.

This consumes sampled candidates. Its finite-element evaluations are exact
apart from bounded Arb roundoff, with no bound on their physical residual.
"""
import argparse
from platform import python_version

import flint
from flint import arb, ctx
import numpy as np


def compute(data, volume):
    v = [arb(float(x)) for x in volume["v"]]
    widths = [(x1-x0)/2 for x0,x1 in zip(v[:-1],v[1:])]
    u_binary = volume["u"]
    i0 = int(np.searchsorted(u_binary,0.))
    assert u_binary[i0] == 0 and u_binary[-1] == float("8.407362")
    assert np.array_equal(u_binary,data["u_full"])
    u = [arb(float(x)) for x in u_binary[i0:-1]]+[arb("8.407362")]
    F,G,H,V,w,Ft = [volume[name][i0:] for name in ("F","G","H","V","w","Ft")]
    p_shear = [arb(float(x)) for x in data["p_shear_cap"][i0:]]
    p_infty = [arb(float(x)) for x in data["p_shear_infty"][i0:]]

    # Every late F current row is zero below this causal collar.
    # The exact graph lies below 1.5 on the late window.
    j0 = int(np.searchsorted(volume["v"],1.5))
    assert j0 > 0 and not np.any(F[:,:j0+1]) and not np.any(Ft[:,:j0+1])
    assert v[j0] >= arb("1.5") and 1+2*(arb("-0.5").exp()/2).lambertw() < arb("1.5")
    assert not np.any(G[:,:j0+1]) and not np.any(H[:,:j0+1])
    # Truncate the binary weak-Volterra pre-echo at a causally zero collar.
    # Convex interpolation and the length of the collar bound the edit.
    leakage = arb(float(np.max(abs(w[:,:j0+1]))))
    assert leakage < arb("1e-150")
    w = w.copy()
    w[:,:j0+1] = 0
    widths = widths[j0:]
    fprev = None
    f_integral_prev = None
    radial_prev = None
    ft_abs_prev = None
    ft_signed_prev = None
    fu_sq, ft_abs_sq, ft_signed_sq, cap_err_sq, inf_err_sq = [arb(0)]*5

    def dyadic(row):
        return [arb(float(y)) for y in row[j0:]]

    def trapezoid(a):
        return sum(dv*(a0+a1) for a0,a1,dv in zip(a[:-1],a[1:],widths))

    def lin_sq(a,b,h):
        return h*(a*a+a*b+b*b)/3

    for i in range(len(u)):
        f = dyadic(F[i])
        ft = dyadic(Ft[i])
        f_integral = trapezoid(f)
        ft_abs = trapezoid([abs(x) for x in ft])
        ft_signed = trapezoid(ft)
        volume_terms = [hh+vv*ww for hh,vv,ww in zip(dyadic(H[i]),dyadic(V[i]),dyadic(w[i]))]
        radial = arb(float(G[i,-1]))-trapezoid(volume_terms)
        if i:
            h = u[i]-u[i-1]
            derivative = (f_integral-f_integral_prev)/h
            pu = (radial_prev+derivative)/4
            pv = (radial+derivative)/4
            cap_err_sq += lin_sq(pu-p_shear[i-1],pv-p_shear[i],h)
            inf_err_sq += lin_sq(pu-p_infty[i-1],pv-p_infty[i],h)
            # For the bilinear interpolant, d_u F is exactly affine in v.
            # Convexity bounds the absolute value integral by trapezoids.
            fu = trapezoid([abs(a-b)/h for a,b in zip(f,fprev)])
            fu_sq += h*fu*fu
            ft_abs_sq += lin_sq(ft_abs_prev,ft_abs,h)
            ft_signed_sq += lin_sq(ft_signed_prev,ft_signed,h)
        fprev, f_integral_prev, radial_prev = f, f_integral, radial
        ft_abs_prev,ft_signed_prev = ft_abs,ft_signed

    print(f"late collar first retained v={v[j0]}, max zeroed pre-echo {leakage}")
    print(f"||int |d_u F_h| dv||_2 <= {fu_sq.sqrt().upper()}")
    print(f"||int |(F_t)_h| dv||_2 <= {ft_abs_sq.sqrt().upper()}")
    print(f"||int (F_t)_h dv||_2 = {ft_signed_sq.sqrt()}")
    print(f"||p_bilinear - p_shear_cap||_2 = {cap_err_sq.sqrt()}")
    print(f"||p_bilinear - n_shear_infty||_2 = {inf_err_sq.sqrt()}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("data")
    parser.add_argument("volume")
    parser.add_argument("--bits",type=int,default=112)
    args = parser.parse_args()
    ctx.prec=args.bits
    print(f"Python {python_version()}, python-flint {flint.__version__}, Arb {ctx.prec} bits")
    compute(np.load(args.data),np.load(args.volume))


if __name__ == "__main__":
    main()
