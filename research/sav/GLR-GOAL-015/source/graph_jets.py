"""Constraint-compatible graph traces for the two physical mixed fields."""
from dataclasses import dataclass

from flint import arb, arb_series
from source_geometry import symmetric
from certified_launch import normalized_bump


def bump_jet(y):
    """Unnormalized flat bump exp(4-1/(y(1-y))) and two derivatives."""
    a, b = y.lower(), y.upper()
    if b <= 0 or a >= 1:
        return (arb(0),)*3
    if 0 < a and b < 1:
        q = 1/(y*(1-y))
        tilt = 1-2*y
        shape = (4-q).exp()
        return (shape, tilt*q**2*shape,
                (tilt*tilt*(q**4-2*q**3)-2*q**2)*shape)
    if a <= 0 and b < arb("0.5"):
        edge = b
    elif b >= 1 and a > arb("0.5"):
        edge = 1-a
    else:
        raise ValueError("Subdivide a wide bump cell crossing an endpoint")
    q = 1/(edge*(1-edge))
    assert q > 4
    z = (4-q).exp()
    return (arb(0).union(z.upper()), symmetric(q**2*z),
            symmetric((q**4+2*q**3+2*q**2)*z))


def matrix_product(matrix, vector):
    return tuple(a*vector[0]+b*vector[1] for a, b in matrix)


def ef_transform(r, h):
    f = 1-1/r
    vv, vr, rr, angular = h
    return (vv, vv+f*vr, vv+2*f*vr+f*f*rr, angular)


def first_density(r, omega, h):
    """Return M_1/r² in (t,x) on v=r, with no Fourier half factor."""
    f = 1-1/r
    a, b, c, _ = ef_transform(r, h)
    coeff = -(omega*r).exp()/f
    return ((coeff*a, -coeff*b), (-coeff*b, coeff*c))


@dataclass
class GraphData:
    raw: tuple          # U_t, Im A_z,t
    g: tuple            # normalized mixed Y_t
    g_tan: tuple        # derivative of Y_t along graph with respect to x
    g_t: tuple          # mixed Y_tt at the graph
    g_x: tuple          # mixed Y_tx at the graph
    source: tuple       # S_y, Im S_z at the graph


class GraphEnclosure:
    def __init__(self, geometry):
        self.geometry = geometry
        self.r0 = arb(13)/8
        self.onset = -self.r0-2*(self.r0-1).log()
        self.delta = arb(3)/25
        self.norm = normalized_bump()

    def radius(self, u):
        return 1+2*((-(1+u)/2).exp()/2).lambertw()

    def evaluate(self, u):
        return self.evaluate_radius(self.radius(u))

    def evaluate_radius(self, r):
        k, omega = self.geometry.k, self.geometry.omega
        h, _, _ = self.geometry.evaluate(r)
        f, p = 1-1/r, -1/r
        p_tan = f/r**2
        u = -r-2*(r-1).log()
        phi, phiu, phiuu = tuple(
            self.norm*b*self.delta.sqrt()/self.delta**j
            for j, b in enumerate(bump_jet((u-self.onset)/self.delta))
        )
        # Carry one total radial derivative along v=r for the induced jets.
        radius = arb_series([r, 1], 2)
        f_s, p_s = 1-1/radius, -1/radius
        h_s = tuple(arb_series([val, deriv], 2) for val, deriv in zip(h[0], h[1]))
        ur = (p-1)/f
        phi_s, phiu_s = (arb_series([val, ur*deriv], 2)
                         for val, deriv in [(phi, phiu), (phiu, phiuu)])
        m_s = first_density(radius, omega, h_s)
        av = (f_s*phi_s/radius-phiu_s)/(2*radius), phiu_s/(2*radius)
        jt, jx = (2*radius*radius*v for v in matrix_product(m_s, av))
        dk_s = 4+(k*radius)**2
        d_s, w_s = radius*radius*dk_s, 4*radius**4/dk_s
        graph_den = 1-p_s*p_s
        ut = (p_s*jt-jx)/graph_den
        zt = k*(jt-p_s*jx)/(4*radius*radius*graph_den)
        y = ut/d_s.sqrt(), zt*w_s.sqrt()
        g = tuple(q[0] for q in y)
        g_tan = tuple(f*q[1] for q in y)

        # The graph tangent M_{1,x}+p M_{1,t} is f*d_r along v=r.
        # The stationary-background time partial is omega M_1.
        m = tuple(tuple(q[0] for q in row) for row in m_s)
        mx = tuple(tuple(f*q[1]-p*omega*q[0] for q in row) for row in m_s)
        dk = dk_s[0]
        rootd, rootw = d_s[0].sqrt(), w_s[0].sqrt()
        v_rw = f*(6/r**2-3/r**3)
        wtt = phiuu-v_rw*phi/(1-p*p)
        wtx = -phiuu+p*v_rw*phi/(1-p*p)
        wxx = wtt+v_rw*phi
        a = tuple(q[0] for q in av)
        at = (wtx+f*phiu/r)/(2*r), wtt/(2*r)
        ax = ((wxx-f*phiu/r+f*(1/r**3-f/r**2)*phi)/(2*r)-f*a[0]/r,
              wtx/(2*r)-f*a[1]/r)
        j = jt[0], jx[0]
        jt_part = tuple(omega*x+2*r*r*t for x, t in zip(j, matrix_product(m, at)))
        jx_part = tuple(2*f*x/r+2*r*r*(t+s) for x, t, s in
                        zip(j, matrix_product(mx, a), matrix_product(m, ax)))
        dm = f*(1/r+k*k*r/dk)
        dlc = 2*f/r-2*f*k*k*r/dk
        xc = 3*(omega*r).exp()*h[0][3]*phi/r  # 3 d_b Q_0, lambda=4.
        src = ((jt_part[1]+jx_part[0]-2*dm*j[0])/rootd+f*rootd*xc/r**4,
               -k*r*r/(dk*rootw)*(jt_part[0]+jx_part[1]+dlc*j[1]))
        skew = 4*k*f/dk
        bg = -skew*g[1], skew*g[0]
        g_t = tuple((b-s-p_tan*a-2*p*ap)/(1-p*p)
                    for b, s, a, ap in zip(bg, src, g, g_tan))
        g_x = tuple(ap-p*a for ap, a in zip(g_tan, g_t))
        return GraphData((ut[0], zt[0]), g, g_tan, g_t, g_x, src)
