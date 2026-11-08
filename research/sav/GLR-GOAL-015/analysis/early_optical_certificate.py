"""Directed early weak sign from a rotated two-polarization optical proxy."""
from pathlib import Path
from platform import python_version
import argparse
import sys

import flint
from flint import arb, arb_mat, ctx

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "source"))
from source_geometry import GeometryEnclosure, x

LAMBDA = 4
KAPPA, KAPPA2 = 522, 380


def upper(value):
    return value.abs_upper()


def vector_norm(row):
    return upper(sum(upper(z)**2 for z in row).sqrt())


def col(matrix, j):
    return [matrix[i, j] for i in range(matrix.nrows())]


def row(matrix, i):
    return [matrix[i, j] for j in range(matrix.ncols())]


def diag(a, b):
    return arb_mat([[a, 0], [0, b]])


def overlap(a, b):
    return sum(ai*bi for ai, bi in zip(a, b))


def coeffs(geo, r, v, signed=False):
    """Coefficient boxes for arbitrary r,v cells on the strip."""
    f = 1-1/r
    k, omega = geo.k, geo.omega
    (h, hr, _), mean, _ = geo.evaluate(r)
    ht = (omega*v).exp()
    h, hr = tuple(ht*z for z in h), tuple(ht*z for z in hr)
    vv, vr, rr, sph = h
    vvr, vrr, rrr, _ = hr
    diam = sph/(2*r*r)

    def orbit(vv, vr, rr):
        vt, xx = vv+f*vr, vv+2*f*vr+f*f*rr
        return arb_mat([[-vv, -vt], [vt, xx]])/f

    xb = orbit(vv, vr, rr)
    hb_radial = orbit(vvr, vrr, rrr)
    change = arb_mat([[0, -vr], [vr, 2*(vr+f*rr)]])/(r*r)
    xb_x = (omega-1/(r*r))*xb + f*hb_radial + change
    eta = diag(-1, 1)
    unit = diag(1, 1)
    first, first_x = -xb*eta, -xb_x*eta
    xb2 = xb*xb
    tr = xb2[0, 0]+xb2[1, 1]
    direct = (xb2-(2*diam*diam+tr/4)*unit)*eta/2
    mu, beta_mean = [z*ht**2 for z in mean]
    rank = beta_mean-2*mu/(r*f)
    mean_mass = rank*arb_mat([[1, -1], [-1, 1]])

    d = LAMBDA+k*k*r*r
    rootd = d.sqrt()
    alpha, beta = 1/(r*rootd), k/(2*rootd)
    a_m = f*(1/r+k*k*r/d)
    b_m = f*(2/r-k*k*r/d)
    a_x = a_m/r**2 + f*f*(-1/r**2+k*k/d-2*k**4*r**2/d**2)
    b_x = b_m/r**2 + f*f*(-2/r**2-k*k/d+2*k**4*r**2/d**2)
    common = f*(k*k+LAMBDA/r**2)
    weff = max(upper(common+a_m*a_m-a_x), upper(common+b_m*b_m+b_x))
    spin = k*f/d
    spin_v = spin*(1/(2*r*r)-f*k*k*r/d)
    mp = upper(weff/4+spin*spin+upper(spin_v))
    rot = arb_mat([[2, -k*r], [k*r, 2]])/rootd
    skew = spin*arb_mat([[0, -1], [1, 0]])

    a0 = arb_mat([[-1, 1, f/r], [1, 1, 0]])
    a0_x = arb_mat([[0, 0, f*(1/r**3-f/r**2)], [0, 0, 0]])
    jj = (LAMBDA*r/4)*first*a0
    jx = (f/r)*jj + (LAMBDA*r/4)*(first_x*a0+first*a0_x)
    ju, jv = (omega*jj-jx)/2, (omega*jj+jx)/2
    jd = row(arb_mat([[-1, 1]])*jj, 0)
    je = row(arb_mat([[1, 1]])*jj, 0)
    jdu = row(arb_mat([[-1, 1]])*ju, 0)
    jev = row(arb_mat([[1, 1]])*jv, 0)
    n = arb_mat([[alpha], [beta]])
    m = arb_mat([[alpha], [-beta]])
    lower = diag(-2*a_m*alpha, -beta*(2*f/r-2*f*k*k*r/d))*jj
    curl_low = f*(r*rootd)*3*diam*(LAMBDA*r/2)/r**4
    # Compact-unitary trace simplifies H_LL = 4 H_vv + 2 f H_vr - 4 f d_b.
    q0 = -2*vv/f-vr+2*diam
    q_check = jd[0]/(2*r)
    q0 = q0.intersection(q_check)
    src = [upper(q0), vector_norm(col(jd[1]*n+je[0]*m, 0)),
           vector_norm(col(je[1]*m, 0)),
           vector_norm([z+lower[i, 0] for i, z in
                        enumerate(col((jdu[0]+jd[2])*n+jev[0]*m, 0))]),
           vector_norm([z+lower[i, 1] for i, z in
                        enumerate(col(jdu[1]*n+(jev[1]+je[2])*m, 0))]),
           vector_norm([z+lower[i, 2]+(curl_low if i == 0 else 0) for i, z in
                        enumerate(col(jdu[2]*n+jev[2]*m, 0))])]

    du = arb_mat([[alpha, beta], [-alpha, -beta]])
    dv = arb_mat([[-alpha, beta], [-alpha, beta]])
    dx = diag(-alpha*a_m, -beta*b_m)
    ap, aq, ax = du*rot, dv*rot, (du+dv)*rot*skew+dx*rot
    ab = diag(1, -1)*jj/(r*r*d)
    dy = [first*m for m in (ap, aq, ax)]
    dbase = first*ab+(direct+mean_mass)*a0/(2*r)
    c_y = [-6*diam*r*rootd*rot[0, j] for j in range(2)]
    c_b = (-11*diam*diam+tr/8-beta_mean)*(LAMBDA*r/2)
    fg = 2*r*arb_mat([[-1, 1], [1, 1]])
    mixed = [[vector_norm(row(fg*m, i)) for m in dy] for i in range(2)]
    mixed.append([vector_norm([2*f*m[0, j]-(2*f/r**3)*z
                              for j, z in enumerate(c_y if i == 2 else [0, 0])])
                  for i, m in enumerate(dy)])
    mixed[0][0] = upper(8*q0/LAMBDA)
    base = [[upper(z) for z in row(fg*dbase, i)] for i in range(2)]
    base.append([upper(2*f*dbase[0, j]-(2*f/r**3)*(c_b if j == 2 else 0))
                 for j in range(3)])

    p = -1/r
    y_t = diag(alpha, beta)*arb_mat([[p, -1], [1, -p]])*jj/(1-p*p)
    pg = [vector_norm(col((1+p)*y_t/2, i)) for i in range(3)]
    qg = [vector_norm(col((1-p)*y_t/2, i)) for i in range(3)]
    vpot = upper(f*(6/r**2-3/r**3)/4)
    if signed:
        # The optional signed rows support the separate full-packet refinement.
        S = (jd[0]*n,
             jd[1]*n+je[0]*m, je[1]*m,
             (jdu[0]+jd[2])*n+jev[0]*m+arb_mat([[lower[0,0]],[lower[1,0]]]),
             jdu[1]*n+(jev[1]+je[2])*m+arb_mat([[lower[0,1]],[lower[1,1]]]),
             jdu[2]*n+jev[2]*m+arb_mat([[lower[0,2]],[lower[1,2]]])+arb_mat([[curl_low],[0]]))
        Gp = rot.transpose()*(1+p)*y_t/2
        Mplus = (rot.transpose()*diag(common+a_m*a_m-a_x,common+b_m*b_m+b_x)*rot)/4
        Mplus += spin*spin*unit+spin_v*arb_mat([[0,-1],[1,0]])
        # F(P) is the exact principal row. Keeping this identity avoids
        # interval expansion of a zero second component.
        return (rot.transpose()*S[3], Gp, Mplus, skew,
                arb_mat([[-8*q0/LAMBDA,0]]), arb_mat([row(fg*dy[1],0)]),
                arb_mat([row(fg*dy[2],0)]), (fg*dbase)[0,0])
    return q0, src, mixed, base, mp, pg, qg, vpot


def seed_bounds(cells=4096, include_lower=False):
    normalization = arb("0.04679151545").union(arb("0.04679151582"))
    delta = arb(3)/25

    def derivatives(y):
        p = y*(1-y)
        j = (1-2*y)/p**2
        jprime = -2/p**2-2*(1-2*y)**2/p**3
        bump = (-1/p).exp()
        return bump*j, bump*(j*j+jprime)

    y = arb(1)/8
    step = y/cells
    b1, b2 = derivatives(y)
    cap = 32*step
    t = 1/cap
    square = cap*(512*t**8*(-t).exp())**2
    lower_square = arb(0)
    for i in range(32, cells):
        _, second = derivatives((i*step).union((i+1)*step))
        square += step*upper(second)**2
        lower_square += step*second.abs_lower()**2
    scale = delta*delta*normalization**2
    output = (b1/(delta.sqrt()*normalization),
              b2/(delta*delta.sqrt()*normalization), upper(square/scale))
    return (*output, (lower_square/scale).lower()) if include_lower else output


def enclose(refinement=1):
    ctx.prec, ctx.cap = 160, 80
    geo = GeometryEnclosure(refinement=refinement)
    width = arb(3)/200
    start = arb(13)/8
    front = start-2*x(start)
    end = front+width
    cut = arb(81)/50
    assert cut-2*x(cut) > end
    graph_r = cut.union(start)
    q_graph, _, _, _, _, pg, qg, _ = coeffs(geo, graph_r, graph_r)
    pg = upper(pg[0]+pg[2]/KAPPA)
    qg = upper(qg[0]+qg[2]/KAPPA)
    ej = [arb("0.004147"), arb("0.000001124"), arb("0.000004740"),
          arb("1.003017")/KAPPA2, arb("0.0000008181")/KAPPA2,
          arb("1.003017")/(KAPPA*KAPPA2)]
    # s is r(d,v). Since r_u=-f/2, s<=r(u,v)<=s+width/2.
    panels = [(cut, arb(5)/2, 384), (arb(5)/2, arb(8), 768),
              (arb(8), arb(25), 512), (arb(25), arb(64), 256),
              (arb(64), arb(400), 256)]
    opt_signed = arb(0)
    err_force, err_prop = arb(0), arb(0)
    error = [arb(0) for _ in range(3)]
    abs_f, weighted_g = arb(0), arb(0)
    for lo, hi, count in panels:
        count *= refinement
        for j in range(count):
            s0 = lo+(hi-lo)*j/count
            s1 = lo+(hi-lo)*(j+1)/count
            v0, v1 = end+2*x(s0), end+2*x(s1)
            dv = (v1-v0).upper()
            r = s0.union(s1+width/2)
            v = v0.union(v1)
            q0, src, mix, base, mcap, _, _, vpot = coeffs(geo, r, v)
            phase_step = q0*(v1-v0)
            partial_phase = phase_step.union(arb(0))
            phase = opt_signed+partial_phase
            if not v0 > start:
                opt_signed += partial_phase
            else:
                opt_signed += phase_step
            p0 = upper(phase)/4
            err_force = upper(err_force+dv*overlap(src, ej)/4)
            inp = p0+pg/KAPPA2+err_force+err_prop
            pcap = upper(inp*(mcap*dv/KAPPA2).exp())
            err_prop = upper(err_prop+dv*mcap*pcap/KAPPA2)
            err_p = upper(pg/KAPPA2+err_force+err_prop)
            xcap = pcap/KAPPA2
            qcap = ((qg if not v0 > start else 0)
                    +(overlap(src, ej)+src[0])/4+mcap*xcap)/KAPPA2
            for i in range(3):
                e = mix[i][0]*(err_p if i == 0 else pcap)+mix[i][1]*qcap
                e += mix[i][2]*xcap+overlap(base[i], ej[3:])
                error[i] += dv*e
                if i == 0:
                    abs_f += dv*(e+mix[i][0]*p0)
                if i == 1:
                    weighted_g += dv*vpot*e
    # GOAL-011 directed chi at a, widened to allow its entire r>=60 tail.
    chi0 = arb("-1.102605931").union(arb("-1.01612393709"))
    chi0 += arb(0, arb("1e-14"))
    rcap = arb(400).union(arb(400)+width/2)
    vcap = end+2*x(arb(400))
    qcap, *_ = coeffs(geo, rcap, vcap)
    drive = (qcap-(2*graph_r/(graph_r+1))*q_graph)/2
    assert drive < 0
    amp = (geo.omega*width).exp()
    chi = arb(1).union(amp)*chi0 + arb(0).union(width*amp)*drive
    chi_d = amp*chi0 + width.union(width*amp)*drive
    chi2 = chi*chi
    chi2_d = chi_d*chi_d
    assert chi2 > 0 and chi2_d > 0
    b1, b2, b2sq, b2sq_lower = seed_bounds(include_lower=True)
    gd = arb("0.4125472765").union(arb("0.4125472991"))
    max_z = arb("1.003017")*b1.upper()*(arb("1.57")*width).exp()
    lead_terminal = (gd*b2*chi2_d).lower()
    lead_bulk = (arb("1.004147")*b2sq*chi2).upper()
    lead_terminal_upper = (gd*b2*chi2_d).upper()
    lead_bulk_lower = (arb("0.995853")*b2sq_lower*chi2).lower()
    err_terminal = (gd.upper()*b2.upper()*error[0]).upper()
    err_bulk = (arb("1.004147")*b2sq*error[0]).upper()
    err_dual = (b1.upper()*max_z*(arb("1.57")*abs_f+width*weighted_g+error[2])).upper()
    err_tail = arb("6.948e-16")
    lower = ((lead_terminal-lead_bulk-err_terminal-err_bulk-err_dual)/4-err_tail).lower()
    upper_p = ((lead_terminal_upper-lead_bulk_lower+err_terminal+err_bulk+err_dual)/4+err_tail).upper()
    print(f"Python {python_version()} python-flint {flint.__version__}, "
          f"{ctx.prec} bits, radial refinement {refinement}")
    print("physical graph P/Bprime,Q/Bprime", pg, qg)
    print("chi_c^2 uniform, terminal", chi2, chi2_d)
    print("chi_c(u) drive enclosure", drive)
    print("Bprime(d), Bsecond(d), integral Bsecond^2 upper", b1, b2, b2sq)
    print("uniform integrated F defect, G, H / Bsecond upper", *map(upper, error))
    print("uniform integrated |F|, V|G|/4 / Bsecond upper", upper(abs_f), upper(weighted_g))
    print("4 P optical terminal lower, bulk upper", lead_terminal, lead_bulk)
    print("4 P defects terminal upper, bulk upper, remaining dual upper", err_terminal, err_bulk, err_dual)
    print("P lower and upper", lower, upper_p)
    assert lower > 0
    return lower


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--refinement", type=int, default=1)
    opts = parser.parse_args()
    enclose(opts.refinement)
