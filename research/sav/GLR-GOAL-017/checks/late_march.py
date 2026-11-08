"""Non-production late characteristic pilot in the pinned ordered hierarchy.

The node calculation is binary64. Its refinements and alternate news readout
measure numerical sensitivity, and supply no physical continuum certificate.
"""
import argparse
from pathlib import Path
import sys

from numba import njit
import numpy as np
from scipy.integrate import quad, simpson
from scipy.interpolate import CubicSpline
from scipy.special import wrightomega

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-014/checks"))
from c_compact import (DELTA, FRONT, K, LAM, OMEGA, R, Strip, continue_homogeneous,
                       geometry_mass, mat, orbit, packet, radius, static_geometry,
                       tortoise)

END = 8.407362
Q = complex(-0.177924631, -0.747343369)
STAR = Q + 2 * OMEGA
A_Q = complex((.08223 + .18771) / 2, (-.42418 - .31707) / 2)
A_STAR = complex((-.21850 - .16857) / 2, (-.44959 - .39966) / 2)


class LateStrip(Strip):
    def __init__(self, n_packet, n_late, m, cap, cluster=0):
        # The two interfaces (packet end and u=0) are exact mesh nodes.
        end_packet = FRONT + DELTA
        n_before_zero = max(16, round(n_late * (-end_packet) / (END-end_packet)))
        self.u = np.concatenate((np.linspace(FRONT, end_packet, n_packet+1),
                                 np.linspace(end_packet, 0, n_before_zero+1)[1:],
                                 np.linspace(0, END, n_late+1)[1:]))
        self.n = len(self.u) - 1
        self.rule = "simpson"
        self.b = 1 + 2 * wrightomega(-(self.u + 1) / 2 - np.log(2))
        c = END + 2 * tortoise(cap)
        r_max = radius((c - FRONT) / 2)
        radii = R + np.expm1(np.linspace(0, np.log1p(r_max-R), m+1))
        radial_v = FRONT + 2*tortoise(radii)
        if cluster:
            split = int(np.searchsorted(radial_v, 12.))
            origin, end = radial_v[0], radial_v[split]
            dense = origin + .5*np.expm1(np.linspace(0,np.log1p(2*(end-origin)),cluster+1))
            radial_v = np.r_[dense[:-1],radial_v[split:]]
        self.v = np.r_[self.b[:0:-1], radial_v]
        self.r = radius((self.v[None, :] - self.u[:, None]) / 2)
        self.f = 1 - 1/self.r
        self.mask = np.arange(self.n+1)[:, None] + np.arange(len(self.v)) >= self.n
        self.graph = np.arange(self.n+1), np.arange(self.n, -1, -1)


def weights(coordinates, order):
    """Right-end stencil weights on the final interval (up to 4 points)."""
    out = np.zeros((len(coordinates), 5, 4))
    for i in range(1, len(coordinates)):
        step = coordinates[i] - coordinates[i-1]
        for n in range(2, min(i+1, order)+1):
            nodes = (coordinates[i-n+1:i+1][::-1] - coordinates[i])/step
            # Drop long extrapolatory stencils at adjacent spacing transitions.
            if n > 2 and (nodes[-1] < -8 or np.min(np.abs(np.diff(nodes))) < .12):
                out[i, n] = out[i, n-1]
            else:
                vandermonde = nodes[None, :]**np.arange(n)[:, None]
                moment = (-1.)**np.arange(n)/(np.arange(n)+1)
                out[i, n, :n] = step*np.linalg.solve(vandermonde, moment)
        if order < 4:
            out[i, order+1:] = out[i, order]
    return out


@njit
def march(q, pu, pv, potential, source, skew, wu, wv, n_graph, order, pairs):
    """Implicit local nodal march, retaining P=w_u and H=w_v.

    B_u and B_v are the endpoint quadrature weights. With D=w_uv at
    the new point, P=P_b+B_v D, H=H_b+B_u D and
    w=w_b+B_u B_v D. This reduces the local solve to scalar or 2x2
    systems. The two integrals of w are averaged at each point.
    """
    shape = potential.shape
    W, P, H, D = (np.zeros(shape), np.zeros(shape), np.zeros(shape), np.zeros(shape))
    pbase, hbase, wbase = (np.zeros(shape[-1]), np.zeros(shape[-1]), np.zeros(shape[-1]))
    a, rhs = np.zeros(shape[-1]), np.zeros(shape[-1])
    for i in range(shape[0]):
        jg = n_graph-i
        for j in range(jg, shape[1]):
            c = skew[i, j]
            if j == jg:
                W[i, j], P[i, j], H[i, j] = q[i], pu[i], pv[i]
            elif i > 0:
                nv = min(order, j-jg+1)
                nu = min(order, i-max(0, n_graph-j)+1)
                b_u, b_v = wu[i, nu, 0], wv[j, nv, 0]
                for k in range(shape[-1]):
                    pb, hb = P[i, j-1, k], H[i-1, j, k]
                    w_from_v, w_from_u = W[i, j-1, k], W[i-1, j, k]
                    for z in range(1, nv):
                        pb += wv[j, nv, z]*D[i, j-z, k]
                        w_from_v += wv[j, nv, z]*H[i, j-z, k]
                    for z in range(1, nu):
                        hb += wu[i, nu, z]*D[i-z, j, k]
                        w_from_u += wu[i, nu, z]*P[i-z, j, k]
                    pbase[k], hbase[k] = pb, hb
                    wbase[k] = (w_from_v + b_v*hb + w_from_u + b_u*pb)/2
                    a[k] = 4 + potential[i, j, k]*b_u*b_v
                    rhs[k] = -potential[i, j, k]*wbase[k]-source[i, j, k]
                if pairs:
                    b = c*(b_u+b_v)
                    for k in range(0, shape[-1], 2):
                        r0 = rhs[k]-c*(pbase[k+1]+hbase[k+1])
                        r1 = rhs[k+1]+c*(pbase[k]+hbase[k])
                        det = a[k]*a[k+1]+b*b
                        D[i,j,k] = (a[k+1]*r0-b*r1)/det
                        D[i,j,k+1] = (a[k]*r1+b*r0)/det
                else:
                    D[i,j] = rhs/a
                for k in range(shape[-1]):
                    P[i,j,k] = pbase[k]+b_v*D[i,j,k]
                    H[i,j,k] = hbase[k]+b_u*D[i,j,k]
                    W[i,j,k] = wbase[k]+b_u*b_v*D[i,j,k]
                continue
            # The exact graph and the flat front provide input to interior cells.
            for k in range(shape[-1]):
                dr = -potential[i,j,k]*W[i,j,k]-source[i,j,k]
                if pairs:
                    other = k+1 if k % 2 == 0 else k-1
                    dr += (c if k % 2 else -c)*(P[i,j,other]+H[i,j,other])
                D[i,j,k] = dr/4
    return W, P, H


def bump_jets(u):
    """Exact analytic bump derivatives evaluated in binary64."""
    y = (u-FRONT)/DELTA
    norm = np.sqrt(quad(lambda z: packet(z)[1]**2, 0, 1,
                        epsabs=1e-14, epsrel=1e-13)[0])
    jets = packet(y)
    with np.errstate(divide="ignore", invalid="ignore", under="ignore"):
        p = y*(1-y)
        px = 1-2*y
        j = px/p**2
        jx = -2/p**2-2*px**2/p**3
        jxx = 12*px/p**3+6*px**3/p**4
        third = jets[0]*(j**3+3*j*jx+jxx)
    jets.append(np.where((0 < y) & (y < 1), third, 0.))
    return [DELTA**(.5-i)*jet/norm for i, jet in enumerate(jets)]


def baseline_graph(s, V, p, p_x):
    phi, phiu, phiuu, phiuuu = bump_jets(s.u)
    f, r = s.f[s.graph], s.b
    q = V[s.graph]*phi/(1-p*p)
    Vx = f*((6/r**2-3/r**3)/r**2+f*(-12/r**3+9/r**4))
    qx = (Vx*phi+V[s.graph]*(p-1)*phiu+2*p*p_x*q)/(1-p*p)
    g, h = phiuu-q, -phiuu+p*q
    gx = (p-1)*phiuuu-qx
    hx = -(p-1)*phiuuu+p_x*q+p*qx
    g2 = (hx-p*gx-V[s.graph]*phiu)/(1-p*p)
    h2 = gx-p*g2
    displacement = np.stack((phi, phiu, g), axis=-1)
    wt = np.stack((phiu, g, g2), axis=-1)
    wx = np.stack((-phiu, h, h2), axis=-1)
    return displacement, (wt-wx)/2, (wt+wx)/2


def insertion(s, base, lower_u, lower_v, Xb, Xbx, d, V, root_d, root_w, a_m):
    r, f = s.r, s.f
    w, wt = np.moveaxis(base, -1, 0)
    wx = lower_v[...,0]-lower_u[...,0]
    wtt, wtx = lower_u[...,1]+lower_v[...,1], lower_v[...,1]-lower_u[...,1]
    wxx = wtt+V*w
    A = np.stack((wx+f*w/r, wt), axis=-1)/(2*r[...,None])
    At = np.stack((wtx+f*wt/r, wtt), axis=-1)/(2*r[...,None])
    Ax = (np.stack((wxx+f*wx/r+f*(1/r**3-f/r**2)*w, wtx),
                   axis=-1)/(2*r[...,None])-(f/r)[...,None]*A)
    M1, M1x = -geometry_mass(Xb), -geometry_mass(Xbx)
    weight = LAM*r[...,None]**2/2
    J = weight*mat(M1,A)
    Jt = OMEGA*J+weight*mat(M1,At)
    Jx = (2*f/r)[...,None]*J+weight*(mat(M1,Ax)+mat(M1x,A))
    Q0 = LAM*r*w/2
    X = 3*d*Q0
    d_k = LAM+K*K*r*r
    Lc = r*r/d_k
    mixed_source = np.stack(((Jt[...,1]+Jx[...,0]-2*a_m*J[...,0])/root_d
                             + f*root_d*X/r**4,
                             -K*Lc*(Jt[...,0]+Jx[...,1]
                                    +(2*f/r-2*f*K*K*r/d_k)*J[...,1])/root_w), axis=-1)
    return A, J, Q0, X, mixed_source, Jt, Jx


def mixed_graph(s, p, px, J, Jt, Jx, source, root_d, root_w, a_m, b_m, skew):
    r = s.b
    jt, jx = np.moveaxis(J[s.graph], -1, 0)
    # Derivative along the tilted graph in x, with fixed first-order geometry.
    jtx, jxx = np.moveaxis(Jx[s.graph]+p[:,None]*Jt[s.graph], -1, 0)
    f0, f1 = p*jt-jx, jt-p*jx
    f0x, f1x = px*jt+p*jtx-jxx, jtx-px*jx-p*jxx
    den = 1-p*p
    u_raw, z_raw = f0/den, K*f1/(LAM*r*r*den)
    u_rawx = (f0x+2*p*px*u_raw)/den
    z_rawx = K*(f1x+2*p*px*f1/den-2*s.f[s.graph]*f1/r)/(LAM*r*r*den)
    g = np.stack((u_raw/root_d[s.graph], z_raw*root_w[s.graph]), axis=-1)
    gx = np.stack(((u_rawx-a_m[s.graph]*u_raw)/root_d[s.graph],
                   (z_rawx+b_m[s.graph]*z_raw)*root_w[s.graph]), axis=-1)
    cg = skew[s.graph][:,None]*np.stack((-g[:,1], g[:,0]), axis=-1)
    g_t = (-px[:,None]*g-2*p[:,None]*gx+cg-source[s.graph])/den[:,None]
    g_x = gx-p[:,None]*g_t
    q = np.concatenate((np.zeros_like(g), g), axis=-1)
    gt = np.concatenate((g, g_t+OMEGA*g), axis=-1)
    hx = np.concatenate((-p[:,None]*g, g_x-OMEGA*p[:,None]*g), axis=-1)
    return q, (gt-hx)/2, (gt+hx)/2


def currents(s, insertion_data, geometry, mixed, pu, pv, coeffs, shift):
    A, J, Q0, X = insertion_data[:4]
    Xb, M2, d, beta, tr = geometry
    root_d, root_w, a_m, b_m = coeffs
    r, f = s.r, s.f
    ym, Zm = np.moveaxis(mixed[...,shift:shift+2], -1, 0)
    ymt, Zmt = np.moveaxis(pu[...,shift:shift+2]+pv[...,shift:shift+2], -1, 0)
    ymx, Zmx = np.moveaxis(pv[...,shift:shift+2]-pu[...,shift:shift+2], -1, 0)
    U, Ut, Ux = root_d*ym, root_d*ymt, root_d*(ymx+a_m*ym)
    Zt, Zx = Zmt/root_w, (Zmx-b_m*Zm)/root_w
    A1 = np.stack((J[...,0]-Ux+K*r**4*Zt,
                   -J[...,1]-Ut+K*r**4*Zx), axis=-1)/root_d[...,None]**2
    if shift == 0:
        graph_tan = A1[s.graph][...,1]-A1[s.graph][...,0]/s.b
        print(f"max canonical mixed tangential graph defect={np.max(np.abs(graph_tan)):.3e}")
    density = -mat(geometry_mass(Xb), A1)+mat(M2,A)
    curl = -6*d*(U+X)+(7*d*d+tr/8-beta)*Q0
    Dt, Dx = LAM*np.moveaxis(density,-1,0)
    return (2*r/LAM)*(Dx-Dt), (2*r/LAM)*(Dx+Dt), (2*f/LAM)*Dt-2*f*curl/r**3


def diagnostics(s, g, p, g_cap, p_cap, p_derivative):
    i0 = np.searchsorted(s.u, 0.)
    u, base, news = s.u[i0:], g[i0:], p[i0:]
    q = 2*np.real(Q*A_Q*np.exp(Q*u))
    star = 2*np.real(STAR*A_STAR*np.exp(STAR*u))
    def norm(x):
        return np.sqrt(simpson(x*x,x=u))
    print(f"late ||g_infty||2={norm(base):.12g}; ||p_infty||2={norm(news):.12g}; "
          f"||p_cap||2={norm(p_cap[i0:]):.12g}; peak |p_infty|={np.max(abs(news)):.12g}")
    print(f"late cap continuation L2 difference: baseline={norm(base-g_cap[i0:]):.12g}; "
          f"return={norm(news-p_cap[i0:]):.12g}")
    print(f"late independent readout discrepancy (shared fields and shear) "
          f"||p_cap - derivative||2={norm(p_cap[i0:]-p_derivative[i0:]):.12g}")
    for label, template in (("Q",q),("Q+S*",q+star)):
        print(f"{label:4s}: midpoint norm={norm(template):.12g} "
              f"candidate mismatch={norm(news-template):.12g} "
              f"ratio={norm(news-template)/norm(template):.12g}")
    print(f"candidate coefficient ratio ||N21||2/||N01||2={norm(news)/norm(base):.12g}")
    return u, base, news, q, star


def run(args):
    s = LateStrip(args.packet, args.late, args.radial, args.cap, args.cluster)
    print(f"u=[{s.u[0]:.15g},{s.u[-1]:.15g}], cap={args.cap:g}, "
          f"b(d)={s.b[-1]:.15g}, actual cap v={s.v[-1]:.15g}, shape={s.r.shape}, "
          f"quadrature order={args.order}", flush=True)
    r, f = s.r, s.f
    V = f*(6/r**2-3/r**3)
    p, px = -1/s.b, f[s.graph]/s.b**2
    wu, wv = weights(s.u,args.order), weights(s.v,args.order)
    q, gu, gv = baseline_graph(s,V,p,px)
    b,b_u,b_v = march(q,gu,gv,np.repeat(V[...,None],3,axis=-1),
                      np.zeros(r.shape+(3,)),np.zeros_like(r),wu,wv,s.n,args.order,False)
    ht,hx,d,hm,beta,_ = static_geometry(s,args.tol,args.outer)
    Xb = orbit(ht,f)
    Xbx = orbit(hx,f)-Xb/r[...,None,None]**2
    del ht,hx
    d_k = LAM+K*K*r*r
    root_d, root_w = r*np.sqrt(d_k), np.sqrt(LAM)*r*r/np.sqrt(d_k)
    a_m = f*(1/r+K*K*r/d_k)
    b_m = f*(2/r-K*K*r/d_k)
    common = f*(K*K+LAM/r**2)
    ax = a_m/r**2+f*f*(-1/r**2+K*K/d_k-2*K**4*r**2/d_k**2)
    bx = b_m/r**2+f*f*(-2/r**2-K*K/d_k+2*K**4*r**2/d_k**2)
    pot = np.stack((common+a_m*a_m-ax,common+b_m*b_m+bx),axis=-1)
    skew = 2*K*np.sqrt(LAM)*f/d_k
    data = insertion(s,b[...,:2],b_u[...,:2],b_v[...,:2],Xb,Xbx,d,V,root_d,root_w,a_m)
    graph = mixed_graph(s,p,px,data[1],data[5],data[6],data[4],root_d,root_w,a_m,b_m,skew)
    data = data[:5]
    shifted = insertion(s,b[...,1:]+2*OMEGA*b[...,:2],
                        b_u[...,1:]+2*OMEGA*b_u[...,:2],
                        b_v[...,1:]+2*OMEGA*b_v[...,:2],Xb,Xbx,d,V,root_d,root_w,a_m)[:5]
    del Xbx,b,b_v
    potential = np.concatenate((pot,pot),axis=-1)
    source = np.concatenate((data[4],shifted[4]),axis=-1)
    mixed,mu,mv = march(*graph,potential,source,skew,wu,wv,s.n,args.order,True)
    if args.probes:
        from probe_lower import inspect
        inspect(s,mixed,mu,mv,potential,source,skew)
    del graph,pot,potential,source,skew
    Xb2 = Xb@Xb
    tr = np.trace(Xb2,axis1=-2,axis2=-1)
    M2 = geometry_mass((Xb2-(2*d*d+tr/4)[...,None,None]*np.eye(2))/2
                       +beta[...,None,None]*np.eye(2)-orbit(hm,f))
    del Xb2,hm
    coeffs = root_d,root_w,a_m,b_m
    geom = Xb,M2,d,beta,tr
    F,G,H = currents(s,data,geom,mixed,mu,mv,coeffs,0)
    Ft = currents(s,shifted,geom,mixed,mu,mv,coeffs,2)[0]
    del mixed,mu,mv,data,shifted,geom,coeffs,Xb,M2
    w, rayF = s.weak_return(F,G,H,V,args.iterations)
    bprime = -(1+p)/(1-p)
    radial_Ft = s.vi(Ft)[:,-1]
    rest = G[:,-1]-s.vi(H+V*w)[:,-1]
    news = (rest-F[:,-1]+(1-bprime)*F[s.graph]+radial_Ft)/4
    derivative = CubicSpline(s.u,w[:,-1])(s.u,1)
    ray_derivative = CubicSpline(s.u,rayF[:,-1])(s.u,1)
    weak_readout = (rest+ray_derivative)/4
    late = s.u >= 0
    vnorm = np.sqrt(simpson(s.vi(abs(Ft))[late,-1]**2,x=s.u[late]))
    disagreement = np.sqrt(simpson((weak_readout[late]-derivative[late])**2,x=s.u[late]))
    ray_disagreement = np.sqrt(simpson((radial_Ft[late]-ray_derivative[late])**2,x=s.u[late]))
    print(f"late F_t cancellation: ||int_v |F_t| ||2={vnorm:.12g}; "
          f"||int_v F_t - d_u int_v F||2={ray_disagreement:.12g}; "
          f"||weak primitive readout - shear slope||2={disagreement:.12g}")
    g = b_u[:,-1,0]
    null = continue_homogeneous(s,args.cap,np.stack((g,news,derivative,weak_readout),axis=-1))
    u, g_inf, p_inf, q_mid, star_mid = diagnostics(s,null[:,0],null[:,1],g,news,derivative)
    if args.output:
        np.savez(args.output, u=u, baseline=g_inf, news=p_inf, q=q_mid, star=star_mid,
                 u_full=s.u, g_cap=g, p_cap=news, g_infty=null[:,0], p_infty=null[:,1],
                 p_shear_cap=derivative, p_shear_infty=null[:,2], p_weak_infty=null[:,3])
    if args.volume:
        np.savez(args.volume,u=s.u,v=s.v,F=F,G=G,H=H,Ft=Ft,V=V,w=w)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--packet",type=int,default=160)
    ap.add_argument("--late",type=int,default=240)
    ap.add_argument("--radial",type=int,default=450)
    ap.add_argument("--cluster",type=int,default=0,help="Replace the initial radial mesh through v≈12.")
    ap.add_argument("--cap",type=float,default=400)
    ap.add_argument("--outer",type=float,default=430)
    ap.add_argument("--order",type=int,choices=(2,3,4),default=4)
    ap.add_argument("--iterations",type=int,default=32)
    ap.add_argument("--tol",type=float,default=8e-14)
    ap.add_argument("--output")
    ap.add_argument("--volume")
    ap.add_argument("--probes",action="store_true")
    run(ap.parse_args())


if __name__ == "__main__":
    main()
