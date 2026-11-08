"""Directed fixed-background baseline on a compact, graph-conforming strip."""
import argparse
import csv
from platform import python_version

import flint
from flint import acb, arb, ctx

ctx.prec = 192
Q = arb
zero, one = Q(0), Q(1)
delta = Q(3)/25
radius0 = Q(13)/8
end = Q('8.407362')


def cap(x):
    return x.abs_upper()


def hull(*x):
    result = x[0]
    for a in x[1:]:
        result = result.union(a)
    return result


def g(z):
    return z*z*(1-z)


def gp(z):
    return z*(2-3*z)


def gpp(z):
    return 2-6*z


def graph(u):
    return 1 + 2*(((-(1+u)/2).exp())/2).lambertw()


def zflow(p, zr):
    """Inverse radius after flowing by p at fixed v."""
    if zr.is_zero() or p.is_zero():
        return zr
    r = 1/zr
    x = r+(r-1).log()-p/2
    return 1/(1+(x-1).exp().lambertw())


def graph_z(p, z, front):
    v = graph(front + delta - z)
    return 1/(1+((v-front-p)/2-1).exp().lambertw())


def bjet(y):
    t = 1/(y*(1-y))
    log1 = (1-2*y)*t*t
    log2 = -2*t*t-2*(1-2*y)*(1-2*y)*t**3
    log3 = 12*(1-2*y)*t**3+6*(1-2*y)*(1-2*y)*(1-2*y)*t**4
    b = (-t).exp()
    return b, b*log1, b*(log1*log1+log2), b*(log3+3*log1*log2+log1*log1*log1)


def bump_normalization(cells):
    area = Q(0)
    h = 1/Q(cells)
    flat = 512*Q(cells)**8*(-Q(cells)).exp()
    for i in range(cells):
        lo, hi = i*h, (i+1)*h
        _, m1, _, _ = bjet((lo+hi)/2)
        if i in (0, cells-1):
            b1 = b2 = b3 = hull(-flat, flat)
        else:
            _, b1, b2, b3 = bjet(hull(lo, hi))
        error = h**3/12*cap(b2*b2+b1*b3)
        area += h*m1*m1+hull(-error, error)
    assert area > 0, area
    return area.sqrt()


def coef_node(p, z, inner, front, at_infinity):
    if inner:
        v = graph(front + delta - z)
        rz = graph_z(p, z, front)
        return g(rz)*(3-Q('1.5')*rz)/2 * (v-1)/(v+1)
    za = delta + 1/radius0-z
    if at_infinity:
        assert za.contains(0)
        return Q(3)
    assert za > 0
    rz = zflow(p, za)
    return (3-Q('1.5')*rz)*g(rz)/g(za)


def inner_coef_cell(p0, p1, z0, z1, front):
    # Zp=g(Z)/2 and Zz=−q g(Z)/2, q=dv/dz.
    Z = hull(graph_z(p0,z1,front),graph_z(p1,z0,front))
    v = hull(graph(front + delta-z0), graph(front + delta-z1))
    q = (v-1)/(v+1)
    qv = 2/(v+1)**2
    qz = qv*q
    qzz = -4*q*q/(v+1)**3+qv*qz
    B = 3-Q('1.5')*Z
    l = B*g(Z)/2
    l1 = (B*gp(Z)-Q('1.5')*g(Z))/2
    l2 = (B*gpp(Z)-3*gp(Z))/2
    a = g(Z)/2
    a1 = gp(Z)/2
    return (q*l, q*l1*a, qz*l-q*q*l1*a,
            q*(l2*a*a+l1*a1*a),
            qzz*l-3*q*qz*l1*a+q**3*(l2*a*a+l1*a1*a),
            qz*l1*a-q*q*(l2*a*a+l1*a1*a),
            (1/q).upper())


def outer_coef_cell(p0, p1, z0, z1, touches_infinity):
    za0 = delta+1/radius0-z1
    za1 = delta+1/radius0-z0
    if touches_infinity:
        assert za0.contains(0)
        za0 = zero
    else:
        assert za0 > 0
    Z0 = zflow(p0,za0)
    Z1 = zflow(p1,za1)
    Z = hull(Z0,Z1)
    travel = hull(za0,Z1)
    B = 3-Q('1.5')*Z
    f, f1, f2 = g(Z)/2, gp(Z)/2, gpp(Z)/2
    if za0 > 0:
        za = hull(za0,za1)
        J = g(Z)/g(za)
        D = (gp(Z)-gp(za))/g(za)
        H = J*D
        I = J*(D*D+(gpp(Z)*J-gpp(za)-D*gp(za))/g(za))
        # Cross-check the divided-difference estimate below. Intersection
        # is unnecessary for correctness, but avoids loose interval quotients.
    else:
        J = H = I = None
    # Variational equations give J=e^{int f'}, H=J int f''J,
    # I=J int (f''' J²+3 f'' H); bound at all p up to p1.
    f1_upper = min((gp(travel)/2).upper(), (Q(1)/6).upper())
    Jmax = (p1*max(zero, f1_upper)).exp().upper()
    Hmax = (Jmax**2*p1*cap(gpp(travel)/2)).upper()
    Imax = (Jmax*p1*(3*Jmax**2+3*cap(gpp(travel)/2)*Hmax)).upper()
    j_var, h_var, i_var = hull(0*Jmax,Jmax), hull(-Hmax,Hmax), hull(-Imax,Imax)
    if J is not None:
        J, H, I = J.intersection(j_var), H.intersection(h_var), I.intersection(i_var)
    else:
        J,H,I = j_var,h_var,i_var
    B1 = -Q('1.5')
    d = B1*f+B*f1
    d1 = 2*B1*f1+B*f2
    az = (g(hull(za0,za1))/2).upper()
    return (B*J, d*J, -(B1*J*J+B*H), (d1*f+d*f1)*J,
            3*B1*J*H+B*I, -(d1*J*J+d*H), az)


def grid(n, late, radial, front):
    p = [delta*i/n for i in range(n+1)]
    p += [delta+(-front-delta)*i/(late//4) for i in range(1,late//4+1)]
    middle = len(p)-1
    p += [-front+end*i/late for i in range(1,late+1)]
    z = [delta*j/n for j in range(n+1)]
    z += [delta+j/(radius0*radial) for j in range(1,radial+1)]
    return p,z,middle


def integrate(n, late, radial, norm_cells):
    front = -radius0-2*(radius0-1).log()
    p,z,middle = grid(n,late,radial,front)
    P,Z = len(p),len(z)
    H = [p[i+1]-p[i] for i in range(P-1)]
    K = [z[j+1]-z[j] for j in range(Z-1)]
    norm = bump_normalization(norm_cells)
    normalizer = delta.sqrt()/norm
    a = [normalizer*bjet(Q(i)/n)[0] if 0 < i < n else zero for i in range(P)]
    a1, a2, av = [], [], []
    t_edge = Q(n)*n/(n-1)
    edge0 = (-t_edge).exp()
    edge1 = t_edge**2*edge0
    edge2 = (t_edge**4+4*t_edge**3)*edge0
    for i in range(P-1):
        if i < n:
            if i in (0,n-1):
                b,b1,b2 = hull(zero,edge0),hull(-edge1,edge1),hull(-edge2,edge2)
            else:
                b,b1,b2,_ = bjet(hull(Q(i)/n,Q(i+1)/n))
            av.append(cap(normalizer*b))
            a1.append(cap(normalizer*b1/delta))
            a2.append(cap(normalizer*b2/delta**2))
        else:
            av.append(zero)
            a1.append(zero)
            a2.append(zero)
    val = [[zero for _ in z] for _ in p]
    cum = [[zero for _ in z] for _ in p]
    err = [[zero for _ in z] for _ in p]
    # An interior seam at z=delta has unequal dv/dz on its two sides.
    # Preserve the same w and transform the current density by the ratio.
    seam = (radius0-1)/(radius0+1)*g(1/radius0)/2
    for i in range(P):
        row_cum = zero
        for j in range(Z):
            if i+j < n:
                continue
            k = coef_node(p[i],z[j],j<n,front,j==Z-1)
            if i==0 or j==0:
                f = (-k*a[i]).mid()
                val[i][j] = f
                err[i][j] = cap(f+k*a[i])
                continue
            left_density = seam if j==n else one
            m = H[i-1]*K[j-1]
            if i+j == n:
                m = zero
            elif i+j == n+1:
                m /= 6
                row_cum += m*(left_density*val[i-1][j]+val[i][j-1])
            else:
                m /= 4
                row_cum += m*(val[i-1][j-1]+left_density*val[i-1][j]+val[i][j-1])
            m *= left_density
            base = cum[i-1][j]+row_cum
            f = (-k*(a[i]+base)/(1+m*k)).mid()
            val[i][j] = f
            row_cum += m*f
            cum[i][j] = cum[i-1][j]+row_cum
            err[i][j] = cap(f+k*(a[i]+cum[i][j]))
    # Weighted directional L2 errors in each strip. All operations directed.
    col = [zero]*(Z-1)
    row = [zero]*(P-1)
    qz_prefix = [zero]*(Z-1)
    qzz_prefix = [zero]*(Z-1)
    maxrho, residual_cell = zero, None
    graph_f = [max(cap(val[i][n-i]*(seam if i==0 else one)),
                   cap(val[i+1][n-i-1])) for i in range(n)]
    # One-sided derivatives on the compactification interface z=delta.
    # Use side-specific endpoint ordinates in that case, since F=K*w is a density.
    for i in range(P-1):
        q_u, q_uu = zero, zero
        for j in range(Z-1):
            if i+j+2 <= n:
                continue
            hi,kj = H[i],K[j]
            triangle = i+j+1 == n
            left_density = seam if j==n-1 else one
            lower_left,lower_right = val[i][j],left_density*val[i][j+1]
            upper_left,upper_right = val[i+1][j],left_density*val[i+1][j+1]
            corners = (lower_right,upper_left,upper_right) if triangle else (
                lower_left,lower_right,upper_left,upper_right)
            Fs = max(cap(x) for x in corners)
            dp = cap((upper_right-lower_right)/hi)
            dz = cap((upper_right-upper_left)/kj)
            if not triangle:
                dp = max(dp,cap((upper_left-lower_left)/hi))
                dz = max(dz,cap((lower_right-lower_left)/kj))
            q_u = (q_u+kj*Fs).upper()
            q_uu = (q_uu+kj*dp).upper()
            # Column prefix arrays collect w_z and the fixed-z integral of F_z.
            qz_prefix[j] = (qz_prefix[j]+hi*Fs).upper()
            qzz_prefix[j] = (qzz_prefix[j]+hi*dz).upper()
            Wu = (a1[i]+q_u).upper()
            Wz = qz_prefix[j]
            Wuu = (a2[i]+q_uu+(graph_f[i] if i<n else zero)).upper()
            Wzz = (qzz_prefix[j]+(graph_f[n-j-1] if j<n else zero)).upper()
            W = (max(cap(a[ii]+cum[ii][jj]) for ii,jj in (
                (i,j+1),(i+1,j),(i+1,j+1))) + hi*Wu+kj*Wz).upper()
            kc,kp,kz,kpp,kzz,kpz,alpha = (inner_coef_cell(
                p[i],p[i+1],z[j],z[j+1],front) if j<n else outer_coef_cell(
                    p[i],p[i+1],z[j],z[j+1],j==Z-2))
            ipp = cap(kpp)*W+2*cap(kp)*Wu+cap(kc)*Wuu
            izz = cap(kzz)*W+2*cap(kz)*Wz+cap(kc)*Wzz
            if triangle:
                ipz = cap(kpz)*W+cap(kp)*Wz+cap(kz)*Wu+cap(kc)*Fs
                rem = (hi**2*ipp+2*hi*kj*ipz+kj**2*izz)/2
            else:
                rem = (hi**2*ipp+kj**2*izz)/8
            defect = max((left_density*err[i][j+1]).upper(),err[i+1][j],(left_density*err[i+1][j+1]).upper())
            if not triangle:
                defect = max(defect,err[i][j])
            rho = (defect+rem).upper()
            if rho > maxrho:
                maxrho = rho
                residual_cell = i,j
            col[j] += hi*rho**2
            row[i] += kj*alpha*rho**2
    norm_vu = sum(k*c.sqrt() for k,c in zip(K,col))
    norm_uv = sum(h*r.sqrt() for h,r in zip(H,row))
    E = 2*(norm_vu+norm_uv)
    trace = [sum(k*(v[j]+v[j+1]*(seam if j==n-1 else one))/2
                 for j,k in enumerate(K)) for v in val[middle:]]
    norm_h = sum(h*(x*x+x*y+y*y)/3 for h,x,y in zip(
        H[middle:],trace,trace[1:])).sqrt()
    print('max residual cell', residual_cell)
    report('max residual bound', maxrho)
    report('L1_v L2_u residual upper', norm_vu.upper())
    report('L1_u L2_v residual upper', norm_uv.upper())
    report('candidate norm', norm_h)
    report('energy error upper', E.upper())
    report('exact norm', norm_h+hull(-E.upper(),E.upper()))
    compare_baseline_pole(trace, norm_h, E.upper())
    return p[middle:],trace,E.upper()


def exp_integral(s, length):
    return (s*length).expm1()/s


def compare_baseline_pole(trace, norm_h, error):
    qbox = arb('5e-8')
    s = acb(arb('-.177924631')+hull(-qbox,qbox),
            arb('-.747343369')+hull(-qbox,qbox))
    # Wider than the displayed inherited signed_projection.txt Arb box.
    A = acb(hull(arb('-.000565'),arb('-.000555')),
            hull(arb('.000653'),arb('.000667')))
    z = s*A
    norm_sq = 2*(abs(z)**2*exp_integral(2*s.real,end)
                 +(z*z*exp_integral(2*s,end)).real)
    norm = norm_sq.sqrt()
    width = end/(len(trace)-1)
    area = exp_integral(s,width)
    moment = ((s*width).exp()*(s*width-1)+1)/(s*s)
    cross = arb(0)
    for i,(left,right) in enumerate(zip(trace,trace[1:])):
        cross += 2*(z*(s*width*i).exp()*
                    (left*area+(right-left)*moment/width)).real
    mismatch_h = (norm_h*norm_h+norm_sq-2*cross).sqrt()
    mismatch = mismatch_h+hull(-error,error)
    report('baseline ordinary pole pair norm', norm)
    report('exact baseline minus ordinary pole norm', mismatch)
    report('exact baseline ordinary pole relative mismatch', mismatch/norm)


def report(name, x):
    print(f'{name}: [{x.lower().str(16)}, {x.upper().str(16)}]', flush=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--early',type=int,default=64)
    parser.add_argument('--late',type=int,default=128)
    parser.add_argument('--radial',type=int,default=128)
    parser.add_argument('--norm',type=int,default=8192)
    parser.add_argument('--dump', help='optional decimal candidate ordinates and interval radii')
    args = parser.parse_args()
    print(f'Python {python_version()}, python-flint {flint.__version__}, Arb precision {ctx.prec}',
          flush=True)
    print(f'graph {args.early}, late {args.late}, outer {args.radial}, normalization {args.norm}',
          flush=True)
    if args.early < 8 or args.late < 4 or args.late % 4 or args.radial < 2 or args.norm < 8:
        parser.error('require early>=8, late>=4 divisible by 4, radial>=2 and norm>=8')
    _, trace, _ = integrate(args.early,args.late,args.radial,args.norm)
    if args.dump:
        with open(args.dump, 'w', newline='') as handle:
            writer = csv.writer(handle)
            writer.writerow(('u','candidate_midpoint','arb_radius'))
            for i,value in enumerate(trace):
                writer.writerow(((end*i/args.late).mid().str(24, radius=False),
                                 value.mid().str(24, radius=False),
                                 value.rad().str(24, radius=False)))
