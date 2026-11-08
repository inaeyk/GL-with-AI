"""Certified absolute weak returning-numerator tail for rho >= 64.

Uses the pinned GOAL-011 outer endpoint bounds and regular EF mean
coefficients. No ODE solve or quadrature. Inputs are cutoff enclosures.
"""
from dataclasses import dataclass

from flint import acb, arb, ctx

from outer_tail import OuterTail


LAM = arb(4)


def cap(z):
    return abs(acb(z)).upper()


def l2(*entries):
    return sum(x*x for x in entries).sqrt().upper()


@dataclass(frozen=True)
class Bounds:
    hj: arb
    quadratic: arb
    mean: arb

    @property
    def total(self):
        return (self.hj+self.quadratic+self.mean).upper()


def gl_caps(tail):
    """Coefficients of |u|/|r| for (H_tt,H_tx,H_xx,K_sph)."""
    R, f, k2, om = tail.rho0, tail.f_cap, tail.k2, tail.omega
    d = k2-1/R**3
    j = tail.gl.p_cap/tail.f_floor+(3*k2/d+1)/R
    sph = (arb(1)/2+f*j/(k2*R)).upper()
    tt = f*(k2+1/(2*R**3))/d+om**2/k2
    tt += (f/(2*k2*R**2)+3*f*f/(d*R))*j
    tx = f*om/k2*(j+1/(2*R*(R-1)))
    xx = tt+2*f*sph
    return tt.upper(), tx.upper(), xx.upper(), sph


def mean_jet_check(tail):
    """Check uniform Cauchy caps for EF jets on every unit disk in the tail."""
    R, k2, om = tail.rho0, tail.k2, tail.omega
    r = R-1
    f = 1+1/r
    fmin = 1-1/r
    dmin = k2-1/r**3
    dmax = k2+1/r**3
    assert dmin > 0
    q = (k2+(6+11/r)/r**2+18*f/(dmin**2*r**8)+6*(4+5/r)*f/(dmin*r**4))/fmin
    b = (1/r**2+2*om)/fmin
    assert q+b < 1 and (tail.gl.p_cap+om)/tail.f_floor < 1
    growth = arb(1).exp()
    y = dmax*growth
    yr = (2*k2/r+1/r**4+dmax)*growth
    vr = (3*om/(dmin**2*r)+(k2+1/(2*r**3))/dmin**2)*y
    vr += (1/(2*k2*dmin*r**2)+3*f/(dmin**2*r)+om/(k2*dmin))*yr
    ftr = om*(f*yr+(om+1/(2*r**2))*y)/(k2*dmin)
    sph = y/(2*dmin)+(f*yr+om*y)/(k2*dmin*r)
    rr = 2*(vr+sph)/fmin
    disk = max(z.upper() for z in (ftr+f*vr, vr, rr, sph))
    jet_cap = (2*disk*(1+1/R)).upper()
    assert jet_cap < 64
    return jet_cap


def mean_ricci_check(tail):
    """Independent majorant of the exact mean Ricci expressions, divided by M^2."""
    g, f, om, k2 = 1/tail.rho0, tail.f_cap, tail.omega, tail.k2
    mixed_rv = 1+f
    upper_vr = 1+f
    upper_rr = mixed_rv+f*upper_vr
    upper2_vv = 1+upper_vr
    upper2_vr = upper_vr+upper_rr
    upper2_rr = upper_vr*(mixed_rv+upper_rr)
    trace4 = 1+upper_vr+2*g**2
    c_vr_v = (om+g**2)/2
    c_vv_r = (om*(1+2*f)+f+g**2*upper_rr)/2
    c_vr_r = (f*om+1+g**2*upper_vr)/2
    c_rr_r = 1+(f+om)/2
    # The next Hessian and weight bounds have |r| factored out.
    h1_vv = (om**2+g**2*(om+f)/2)*g+2*c_vv_r
    h1_vr = (om+g**2/2)*g+2*c_vr_r
    h1_rr = g+2*c_rr_r
    weight_v, weight_r = 2*mixed_rv+om*g, 2*upper_vr+g
    h2_vr = weight_v*c_vr_v+weight_r*c_vr_r
    h2_rr = weight_v/2+weight_r*c_rr_r
    curvature = (1+om**2+2*om+g**2*(om+2+f)/2
                 +2*g**3*(2+f)+g**4/2)
    rr4 = (curvature/2+h2_rr*g+(2*h1_rr*g**3+g**4)/2
           +4*(g+1)*g**5)
    sphere4_scaled = ((2*h2_vr+f*h2_rr+h1_vv+2*upper_vr*h1_vr+upper_rr*h1_rr)*g
                     +g**3*(2*upper2_vr+f*upper2_vv)+2*upper2_rr*g**2)/2
    qrr = rr4/2+k2*(upper2_vv/4+trace4/8)
    q_sphere_scaled = sphere4_scaled/2+k2*(g**4/4+trace4*g**2/8)
    qzz = k2*(1+upper_vr**2+2*mixed_rv+2*g**4)/8
    tvv = f*qrr/2+q_sphere_scaled+qzz/2
    source_cap = max(qrr.upper(), tvv.upper())
    assert source_cap < 128
    return source_cap


def mean_envelope(tail, u64):
    """Common static coefficient for |mu|,|beta| and its rho decay rate."""
    mean_jet_check(tail)
    mean_ricci_check(tail)
    R = tail.rho0
    radial_cap = R+arb("2.5")
    n_rate = tail.gl.decay+tail.omega*(tail.ray.real-1/(R-1))
    polynomial_loss = 4/radial_cap
    quadrature_rate = (2*n_rate-polynomial_loss).lower()
    static_rate = (2*tail.gl.decay-polynomial_loss).lower()
    assert quadrature_rate > 0 and static_rate > 0
    static_cap = (64**3*radial_cap**4*cap(u64)**2/quadrature_rate).upper()
    return static_cap, static_rate


def returning_tail(u64, psi64, h64, mixed_u64, tail=None):
    """Three absolute integral caps in dr, with no other overlap pieces."""
    if tail is None:
        tail = OuterTail()
    R, f, fmin = tail.rho0, tail.f_cap, tail.f_floor
    u0, p0, h0, m0 = [cap(z) for z in (u64, psi64, h64, mixed_u64)]
    up0 = u0*p0
    g, d2, dh, dm = tail.gl.decay, tail.rw.decay, tail.returning.decay, tail.mixed.decay
    tt, tx, xx, sph = gl_caps(tail)
    xmat = l2(tt, tx, tx, xx)/fmin
    adj = l2(tail.returning.p_cap+f/R, cap(tail.s))/LAM
    base = l2(tail.rw.p_cap+f/R, cap(tail.s2))/2

    # Fold both driven envelopes into the slower outgoing massive rate.
    gap = tail.source_decay-dm
    assert gap > 0
    umat = m0+tail.dx_cap*(tail.f0+tail.affine0)*up0/gap
    jmat = tail.mixed.p_cap*umat+tail.affine0*up0
    invd = 1/tail.dlow.sqrt()
    c0 = tail.k*invd/2
    ac = f*(2+tail.c_d)
    bc = f*(1+tail.c_d)
    # Sum of one-form component caps after extracting 1/|r|.
    at = invd*(jmat/R+(ac/R**2+tail.k*cap(tail.s1)/2)*umat)
    ax = invd*cap(tail.s1)*umat/R+c0*(jmat+bc*umat/R)+tail.y_x*up0/(LAM*R)
    hj_current = LAM*adj*xmat*(at+ax)/(fmin*R)
    hj_curl = 3*sph*(tail.dhigh.sqrt()*umat/R**2+tail.x_tx*up0/R**4)
    hj = (h0*u0*(hj_current+hj_curl)/(dh+g+dm)).upper()

    zq = (5*xmat**2/4+sph**2/2)/2
    wq = (xmat**2/4+7*sph**2/2)/2
    quadratic_factor = 2*LAM*adj*base*zq/(fmin*R**2)+4*wq/R**4
    quadratic = (h0*u0**2*p0*quadratic_factor/(dh+2*g+d2)).upper()

    bcap, bdecay = mean_envelope(tail, u0)
    zb = 1+(4/R+arb(6).sqrt()*f)/fmin
    mean_factor = 2*LAM*adj*base*zb/fmin+4/R**2
    mean = (h0*p0*bcap*mean_factor/(dh+d2+bdecay)).upper()
    return Bounds(hj, quadratic, mean)


def main():
    ctx.prec = 128
    tail = OuterTail()
    # Rounded-up cutoff caps in the physical exterior profile replay.
    u64, psi64, h64 = "3.346e-13", "6.881e-11", "1.262e-11"
    assert mean_jet_check(tail) < 64
    print("mean EF jet cap:", mean_jet_check(tail))
    print("mean Ricci cap / M^2:", mean_ricci_check(tail))
    print("mean static cutoff cap and rate:", mean_envelope(tail, u64))
    result = returning_tail(u64, psi64, h64, 1000, tail)
    print("HJ:", result.hj)
    print("H^2 P:", result.quadratic)
    print("B_E P:", result.mean)
    print("sum:", result.total)
    assert result.total < arb("1e-20")


if __name__ == "__main__":
    main()
