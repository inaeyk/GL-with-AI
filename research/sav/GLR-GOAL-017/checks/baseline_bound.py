"""Directed continuum residual for a separate compact baseline solve.

The Hermite lift is defined by exact binary64 nodal data on a rectangular
grid. Arb bounds the residual of that lift in the exact curved problem.
"""
import argparse
from itertools import product
from platform import python_version

import flint
from flint import arb, ctx
import numpy as np

from late_march import END, LateStrip, baseline_graph, bump_jets, march, weights


def phi_jet_range(y, cells):
    """Bound raw fourth derivative on a packet subinterval."""
    # Use an ordinary interval away from the endpoints, the global analytic
    # log-derivative polynomial times the flat exponential at the ends.
    if y.lower() <= 0 or y.upper() >= 1:
        # For y in [0,1/n], 1/y <= s <= (1/y)/(1-1/n).
        # |B''''| <= (s^8+12s^7+36s^6+48s^5+36s^4)e^-s
        # is relaxed to 512 s^8 e^-s, decreasing for s>=n>=16.
        n = arb(cells-1)
        return (512*n**8*(-n).exp()).upper()
    z = 1/(y*(1-y))
    q = 1-2*y
    j1 = q*z**2
    j2 = -2*z**2-2*q*q*z**3
    j3 = 12*q*z**3+6*q**3*z**4
    j4 = -24*z**3-72*q*q*z**4-24*q**4*z**5
    return ((-z).exp()*(j1**4+6*j1*j1*j2+3*j2*j2+4*j1*j3+j4)).abs_upper()


def raw_phi_jet(y):
    if not (0 < y and y < 1):
        return arb(0), arb(0)
    z = 1/(y*(1-y))
    phi = (-z).exp()
    return phi, phi*(1-2*y)*z*z


def inverse_radius(x):
    return 1+(x-1).exp().lambertw()


def potential(r):
    return (1-1/r)*(6/r**2-3/r**3)


def graph(u):
    return 1+2*((-(1+u)/2).exp()/2).lambertw()


def symmetric_upper(values):
    return max(x.abs_upper() for x in values)


def squared_norm_bernstein(values, width):
    # Exact integral of a quadratic in the Bernstein basis squared.
    gram = ((6,3,1),(3,4,3),(1,3,6))
    return width*sum(values[i]*values[j]*gram[i][j]
                     for i,j in product(range(3),repeat=2))/30


def bernstein_block(ends, us, vs, uvs, hu, hv):
    """Tensor bicubic Hermite-to-Bernstein transformation."""
    return [
        [ends[i//2][j//2] + (i%3!=0)*(3-2*i)*us[i//2][j//2]*hu/3
         + (j%3!=0)*(3-2*j)*vs[i//2][j//2]*hv/3
         + (i%3!=0)*(j%3!=0)*(3-2*i)*(3-2*j)*uvs[i//2][j//2]*hu*hv/9
         for j in range(4)]
        for i in range(4)
    ]


# Degree elevation from 2 to 3, weighted by 3.
elev = (((0,3),),((0,1),(1,2)),((1,2),(2,1)),((2,3),))


def residual_cell(w, phi, phi_error, hu, hv, r_lo, r_hi, region):
    z = 1/r_hi.union(r_lo)
    slope = (z**3*(1-z)*(12-27*z+12*z*z)).abs_upper()
    v_mid = (potential(r_lo)+potential(r_hi))/2
    v_radius = slope*(hu+hv)/4
    d = [[(w[i+1][j+1]-w[i][j+1]-w[i+1][j]+w[i][j])*36/(hu*hv)
           for j in range(3)] for i in range(3)]
    active = region != 0
    inactive = region != 1
    value_bound = arb(0)
    volume_bound = arb(0)
    for i,j in product(range(4),repeat=2):
        elevated = sum(a*b*d[m][n] for m,a in elev[i] for n,b in elev[j])/9
        if inactive:
            value_bound = max(value_bound, w[i][j].abs_upper())
            volume_bound = max(volume_bound,(elevated+v_mid*w[i][j]).abs_upper())
        if active:
            value_bound = max(value_bound, (w[i][j]+phi[i]).abs_upper())
            volume_bound = max(volume_bound,(elevated+v_mid*(w[i][j]+phi[i])).abs_upper())
    return (volume_bound+v_radius*value_bound
            +(abs(v_mid)+v_radius)*phi_error*active).upper()


def run(n_packet, n_late, m, cap, direct):
    s = LateStrip(n_packet,n_late,m,cap)
    r, f = s.r, s.f
    vpot = f*(6/r**2-3/r**3)
    p = -1/s.b
    phi, phi_u = bump_jets(s.u)[:2]
    shape = r.shape+(1,)
    if direct:
        q, gu, gv = baseline_graph(s,vpot,p,f[s.graph]/s.b**2)
        W,P,Q = march(q[:,:1],gu[:,:1],gv[:,:1],vpot[...,None],np.zeros(shape),
                      np.zeros_like(r),weights(s.u,4),weights(s.v,4),s.n,4,False)
        U = np.where(s.mask,W[...,0]-phi[:,None],0)
        U_u = np.where(s.mask,P[...,0]-phi_u[:,None],0)
        U_v = Q[...,0]
        U_uv = -vpot*W[...,0]/4
    else:
        # Numerically solve for the deviation from the exact free packet.
        # Its graph displacement and both null graph jets vanish.
        zero = np.zeros((len(s.u),1))
        source = vpot*phi[:,None]
        W,P,Q = march(zero,zero,zero,vpot[...,None],source[...,None],
                      np.zeros_like(r),weights(s.u,4),weights(s.v,4),s.n,4,False)
        U,U_u,U_v = W[...,0],P[...,0],Q[...,0]
        U_uv = -np.where(s.mask,vpot*U+source,0)/4
    for a in (U,U_u,U_v,U_uv):
        a[s.graph] = 0
        a[0,:] = 0
        a[:,0] = 0
    front = -arb(13)/8-2*(arb(5)/8).log()
    end = arb("8.407362")
    delta = arb(3)/25
    u = [front]+[arb(float(t)) for t in s.u[1:-1]]+[end]
    v = [arb(float(t)) for t in s.v]
    b = [graph(x) for x in u]
    assert v[0] <= b[-1]
    du = [x1-x0 for x0,x1 in zip(u[:-1],u[1:])]
    dv = [x1-x0 for x0,x1 in zip(v[:-1],v[1:])]
    # Outward relaxation of GOAL-013/checks/d_leading_news.txt (dyadic mesh 2^15).
    normalizer = arb("21.37139570607348").union(arb("21.37139586406795"))
    sqrt_delta = delta.sqrt()
    phi_model = []
    for i in range(len(du)):
        if i >= n_packet:
            # The dyadic node at the packet interface can differ from
            # the exact interface; bound a tiny possible support sliver.
            if i == n_packet:
                assert abs(u[i]-front-delta) < arb("1e-15")
            phi_model.append(([arb(0)]*4, arb("1e-100") if i == n_packet else arb(0)))
            continue
        y0,y1=(u[i]-front)/delta,(u[i+1]-front)/delta
        b0,p0=raw_phi_jet(y0)
        b1,p1=raw_phi_jet(y1)
        y = y0.union(y1)
        span = phi_jet_range(y,n_packet)
        h = du[i]
        phi_model.append((
            [normalizer*sqrt_delta*t for t in (b0,b0+h*p0/(3*delta),
                                               b1-h*p1/(3*delta),b1)],
            (normalizer/sqrt_delta**7*span*h**4/384).upper()))
    width = len(v)-1
    v_sq = [arb(0)]*width
    u_sum = arb(0)
    max_residual = arb(0)
    max_crossing = arb(0)
    max_cell = None
    u0_index = int(np.searchsorted(s.u,0.))
    cap_norm_sq = arb(0)
    full_cap_norm_sq = arb(0)
    initial_shear = arb(float(U[u0_index,-1])).abs_upper()
    r_row = [inverse_radius((x-u[0])/2) for x in v]
    raw = (U,U_u,U_v,U_uv)
    print(f"baseline shape={U.shape}, cap={cap}, packet={n_packet}, late={n_late}, radial={m}",flush=True)
    for i,h in enumerate(du):
        r_next = [inverse_radius((x-u[i+1])/2) for x in v]
        phi_h, phi_error = phi_model[i]
        row_sq = arb(0)
        first = max(0,s.n-i-2)
        if first:
            assert v[first] < b[i+1]
        for j in range(first,width):
            region = 1 if v[j] >= b[i] else 0 if v[j+1] <= b[i+1] else 2
            c = [np.array(a[i:i+2,j:j+2]) for a in raw]
            if region == 0 and not any(np.any(a) for a in c):
                continue
            vals = [[[arb(float(t)) for t in row] for row in a] for a in c]
            w = bernstein_block(*vals,h,dv[j])
            bound = residual_cell(w,phi_h,phi_error,h,dv[j],r_next[j],r_row[j+1],region)
            sq = bound*bound
            v_sq[j] = (v_sq[j]+h*sq).upper()
            row_sq = (row_sq+dv[j]*sq).upper()
            if bound > max_residual:
                max_residual = bound
                max_cell = i,j
            if region == 2:
                max_crossing = max(max_crossing,bound)
        u_sum = (u_sum+h*row_sq.sqrt()).upper()
        q0,q1 = arb(float(U[i,-1])),arb(float(U[i+1,-1]))
        d0,d1 = arb(float(U_u[i,-1])),arb(float(U_u[i+1,-1]))
        poly = [d0,3*(q1-q0)/h-d0-d1,d1]
        if i >= u0_index:
            cap_norm_sq += squared_norm_bernstein(poly,h)
        full_cap_norm_sq += squared_norm_bernstein(poly,h)
        r_row = r_next
    v_sum = sum(step*sq.sqrt() for step,sq in zip(dv,v_sq)).upper()
    compact_error = ((u_sum+v_sum)/2).upper()
    cap_norm = cap_norm_sq.sqrt()
    # Local exterior estimate. For u in [0,d], split I P=I_late P+
    # w(0,v). The signed potential energy controls the late homogeneous
    # gain. For the early trace use unit free-bump norm and the same
    # signed bound on [a,0], then integrate the early primitive once.
    cap_min = arb(str(cap))-arb("1e-8")
    assert inverse_radius((v[-1]-end)/2) > cap_min
    assert cap_min >= 100
    jcap = 3/cap_min-3/(4*cap_min**2)
    pi = arb.pi()
    le = -front
    late_gain = (3*end**2/(pi**2*cap_min**2)).exp()
    early_gain = (3*le**2/(pi**2*cap_min**2)).exp()
    early_p = (1+full_cap_norm_sq.sqrt()+compact_error)*early_gain
    shear_cap = initial_shear+le.sqrt()*compact_error
    max_shear = shear_cap+le.sqrt()*(2*le/pi)*jcap*early_p
    # Compare the cap and infinity without a numerical exterior step.
    # Norm bound on the physical P along the late exterior gives the
    # first term. The fixed early primitive gives the second term.
    max_late_p = late_gain*(cap_norm+compact_error+jcap*end.sqrt()*max_shear)
    exterior_error = (jcap*((2*end/pi)*max_late_p+end.sqrt()*max_shear)).upper()
    total = (compact_error+exterior_error).upper()
    print(f"max residual {max_residual}, at cell {max_cell}, max graph-crossing residual {max_crossing}")
    print(f"mixed residual norms U={u_sum}, V={v_sum}")
    print(f"directed baseline compact cap error <= {compact_error}")
    print(f"directed baseline exact lift late cap norm {cap_norm}")
    print(f"directed baseline exterior no-transfer error <= {exterior_error}")
    print(f"directed baseline physical late norm >= {(cap_norm-total).lower()}")
    print(f"directed baseline physical late norm <= {(cap_norm+total).upper()}")


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument("--packet",type=int,default=64)
    p.add_argument("--late",type=int,default=100)
    p.add_argument("--radial",type=int,default=120)
    p.add_argument("--cap",type=float,default=400)
    p.add_argument("--direct",action="store_true",help="Reproduce the failed unsubtracted lift.")
    p.add_argument("--bits",type=int,default=112)
    args=p.parse_args()
    ctx.prec=args.bits
    print(f"Python {python_version()}, python-flint {flint.__version__}, Arb {ctx.prec} bits")
    run(args.packet,args.late,args.radial,args.cap,args.direct)


if __name__=="__main__":
    main()
