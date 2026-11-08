"""Interval expressions for the pinned real +k mixed and zero-mode currents."""

from flint import arb

from weak_candidate import bump, symmetric


def add(*vectors):
    return tuple(sum(column) for column in zip(*vectors))


def scale(value, vector):
    return tuple(value*x for x in vector)


def mat(matrix, vector):
    return tuple(sum(x*y for x, y in zip(row, vector)) for row in matrix)


def mixed_jets(radius, k):
    f = 1-1/radius
    d = 4+k*k*radius**2
    rootD = radius*d.sqrt()
    rootW = 2*radius**2/d.sqrt()
    a = f*(1/radius+k*k*radius/d)
    b = f*(2/radius-k*k*radius/d)
    ax = a/radius**2+f*f*(-1/radius**2+k*k/d-2*k**4*radius**2/d**2)
    bx = b/radius**2+f*f*(-2/radius**2-k*k/d+2*k**4*radius**2/d**2)
    K = f*(k*k+4/radius**2)
    skew = 4*k*f/d
    return rootD, rootW, a, b, K, (K+a*a-ax, K+b*b+bx), skew


def baseline(y, radius):
    """Whole-cell enclosure of the physical leading jet through order two."""
    delta = arb(3)/25
    norm = arb("0.04679151546115").union(arb("0.04679151580708"))
    B, Bp = bump(y, delta, norm)
    if y.upper() <= 0:
        Bpp = arb(0)
    elif y.lower() <= 0:
        s = 1/(y.upper()*(1-y.upper()))
        assert s > 4
        Bpp = arb(0).union(s**4*(-s).exp()/((delta*delta.sqrt())*norm))
    else:
        s = 1/(y*(1-y))
        q = 1-2*y
        Bpp = B*(q*q*s**4-2*s*s-2*q*q*s**3)/delta**2
    f = 1-1/radius
    V = f*(6/radius**2-3/radius**3)
    field = B+symmetric(arb(".003017")*B)
    du = Bp+symmetric(arb(".003017")*Bp)
    dv = symmetric(arb("8.181e-7")*Bp)
    duu = Bpp+symmetric(arb(".004147")*Bpp)
    duv = -V*field/4
    dvv = symmetric(arb("4.740e-6")*Bpp)
    return (field, du, dv, duu, duv, dvv), V


def orbit(h, r):
    """Mass density -b_0^{-1}H eta, with H in regular EF coordinates."""
    f = 1-1/r
    vv, vr, rr = h[:3]
    return ((-vv/f, vv/f+vr),
            (vv/f+vr, -vv/f-2*vr-f*rr))


def physical_source(y, v, r, geometry, geometric_boxes=None):
    """Physical source and convenient intermediate quantities on a whole cell."""
    k, omega = geometry.k, geometry.omega
    if geometric_boxes is None:
        geometric_boxes = geometry.evaluate(r)
    h, mean, _ = geometric_boxes
    texp = (omega*v).exp()
    M = tuple(scale(texp, row) for row in orbit(h[0], r))
    # Differentiate M at fixed v, then use d_x = f d_r + d_v.
    f = 1-1/r
    vv, vr, rr = h[0][:3]
    vvr, vrr, rrr = h[1][:3]
    vterm = vvr/f-vv/(r*r*f*f)
    Mr = ((-vterm, vterm+vrr),
          (vterm+vrr, -vterm-2*vrr-rr/(r*r)-f*rrr))
    Mx = tuple(add(scale(omega, row), scale(f*texp, dr))
               for row, dr in zip(M, Mr))
    db = texp*h[0][3]/(2*r*r)
    mu, beta = scale(texp**2, mean)
    (w, wu, wv, wuu, wuv, wvv), V = baseline(y, r)
    wt, wx = wu+wv, wv-wu
    wtt, wtx, wxx = wuu+2*wuv+wvv, wvv-wuu, wuu-2*wuv+wvv
    A0 = ((wx+f*w/r)/(2*r), wt/(2*r))
    A0t = ((wtx+f*wt/r)/(2*r), wtt/(2*r))
    A0x = add(scale(1/(2*r), (wxx+f*wx/r+f*(1/r**3-f/r**2)*w, wtx)),
              scale(-f/r, A0))
    weight = 2*r*r
    J = scale(weight, mat(M, A0))
    Jt = add(scale(omega, J), scale(weight, mat(M, A0t)))
    Jx = add(scale(2*f/r, J), scale(weight, mat(M, A0x)), scale(weight, mat(Mx, A0)))
    Q0 = 2*r*w
    X = 3*db*Q0
    rootD, rootW, am, bm, K, W, c = mixed_jets(r, k)
    d = 4+k*k*r*r
    L = r*r/d
    source = ((Jt[1]+Jx[0]-2*am*J[0])/rootD + f*rootD*X/r**4,
              -k*L*(Jt[0]+Jx[1]+(2*f/r-2*f*k*k*r/d)*J[1])/rootW)
    return source, (M, db, mu, beta, A0, J, Q0, X), (rootD, rootW, am, bm, K, W, c)


def return_current(r, k, mixed, background, potential):
    M, db, mu, beta, A0, J, Q0, X = background
    rootD, rootW, am, bm, _, _, _ = potential
    f = 1-1/r
    Y, P, Q = mixed[:3]
    U = rootD*Y[0]
    Ut = rootD*(P[0]+Q[0])
    Ux = rootD*(Q[0]-P[0]+am*Y[0])
    Zt = (P[1]+Q[1])/rootW
    Zx = (Q[1]-P[1]-bm*Y[1])/rootW
    A1 = ((J[0]-Ux+k*r**4*Zt)/rootD**2, (-J[1]-Ut+k*r**4*Zx)/rootD**2)
    # X = -M eta; the letter X in background denotes the separate curl insertion.
    X0 = ((M[0][0], -M[0][1]), (M[1][0], -M[1][1]))
    Xsq = tuple(tuple(sum(X0[i][q]*X0[q][j] for q in (0,1))
                      for j in (0,1)) for i in (0,1))
    trace = Xsq[0][0]+Xsq[1][1]
    D0 = 2*db*db+trace/4
    Mdir = ((-(Xsq[0][0]-D0)/2, Xsq[0][1]/2),
            (-Xsq[1][0]/2, (Xsq[1][1]-D0)/2))
    beta_s = beta-2*mu/(r*f)
    m0, m1 = add(mat(M, A1), mat(Mdir, A0),
                 (beta_s*(A0[0]-A0[1]), -beta_s*(A0[0]-A0[1])))
    curl = -6*db*(U+X)+(7*db*db+trace/8-beta)*Q0
    return (2*r*(m1-m0), 2*r*(m1+m0), 2*f*m0-2*f*curl/r**3)


def residual(mixed, source, potential):
    Y, P, Q, drive = mixed
    _, _, _, _, _, W, c = potential
    return (-4*drive[0]-W[0]*Y[0]-c*(P[1]+Q[1])-source[0],
            -4*drive[1]-W[1]*Y[1]+c*(P[0]+Q[0])-source[1])
