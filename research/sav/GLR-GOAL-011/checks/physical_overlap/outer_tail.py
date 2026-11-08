"""Acb exterior endpoint bounds for the GOAL-011 axial overlap.

No ODE integration. Fixed r0=1, ell=2, ray r=2.5+exp(i*pi/4)*rho.
Use Arb/Acb inputs (or decimal strings) for rigorous endpoint amplitudes.
"""
from dataclasses import dataclass

from flint import acb, arb, ctx


ONE = arb(1)
ZERO = arb(0)
I = acb(0, 1)


def cap(value):
    return abs(value).upper()


def bound2(a, b):
    return (a*a+b*b).sqrt().upper()


@dataclass(frozen=True)
class RiccatiTail:
    rho0: arb
    exponent: acb
    power: acb
    q0: arb
    radius: arb
    decay: arb
    p_cap: arb

    def riccati_radius(self, rho):
        rho = arb(rho)
        if rho < self.rho0 or not rho >= self.rho0:
            raise ValueError("rho must be an exact point or a ball above rho0")
        return (self.radius * (self.rho0/rho)**2).upper()

    def derivative(self, rho):
        """Enclose the scalar log derivative, or one massive diagonal entry."""
        rho = arb(rho)
        radius = self.riccati_radius(rho)
        r = acb("2.5") + (1+I)*rho/arb(2).sqrt()
        f = 1-1/r
        return -self.exponent + self.power*f/r + acb(arb(0, radius), arb(0, radius))

    def amplitude(self, rho, initial):
        self.riccati_radius(rho)
        return (cap(acb(initial)) * (-self.decay*(arb(rho)-self.rho0)).exp()).upper()


class OuterTail:
    """Endpoint, source and affine-envelope constants over the physical box."""

    def __init__(self, k=None, omega=None, s=None, rho0=64):
        self.rho0 = arb(rho0)
        if self.rho0 < 64:
            raise ValueError("the stated exterior estimates require rho0 >=64")
        self.k = arb.pi()/5 if k is None else arb(k)
        self.omega = arb("0.06328119", "0.00000001") if omega is None else arb(omega)
        d = arb("0.00000005")
        self.s = (acb(arb("-0.177924631", d), arb("-0.747343369", d))
                  if s is None else acb(s))
        self.s1 = self.s-self.omega
        self.s2 = self.s-2*self.omega
        self.ray = (1+I)/arb(2).sqrt()
        self.k2 = self.k**2
        self.f_cap = 1+1/self.rho0
        self.f_floor = 1-1/self.rho0
        self.dx_cap = (1+1/(self.rho0-1)).upper()
        self.dlow = self.k2-4/self.rho0**2
        self.dhigh = self.k2+4/self.rho0**2
        self.c_d = (4/(self.dlow*self.rho0**2)).upper()
        assert self.dlow > 0

        qgl = (self.k2+self.omega**2).sqrt()
        ac_gl = self.k2/(2*qgl)
        self.gl = self._tail(qgl, ac_gl, self._gl_q(ac_gl))
        qrw = (self.f_cap*(6/self.rho0**2+3/self.rho0**3)).upper()
        self.rw = self._tail(self.s2, ZERO, qrw)
        self.returning = self._tail(self.s, ZERO, qrw)

        radicand = self.s1*self.s1+self.k2
        assert radicand.imag > 0 and self.s1.imag < -self.k
        kappa = -radicand.sqrt()
        ac_m = self.k2/(2*kappa)
        self.mixed = self._tail(kappa, ac_m, self._massive_q(ac_m))
        self.source_decay = (self.gl.decay+self.rw.decay).lower()
        self._source_caps()

    def _tail(self, exponent, power, q0):
        exponent, power = acb(exponent), acb(power)
        attenuation = ((exponent*self.ray).real-cap(exponent)/(self.rho0-1)
                       - cap(power)/self.rho0).lower()
        assert attenuation > 0
        kernel = (self.dx_cap/(2*attenuation)).upper()
        discriminant = (1-4*kernel**2*q0).lower()
        assert discriminant > 0, (q0, attenuation, discriminant)
        # A small inflation of the smaller invariant-ball root leaves a strict check.
        radius = (arb("1.02") * 2*kernel*q0/(1+discriminant.sqrt())).upper()
        assert kernel*(q0+radius**2) < radius and 2*kernel*radius < 1
        decay = (attenuation-self.dx_cap*radius).lower()
        assert decay > 0
        p_cap = (cap(exponent)+cap(power)*self.f_cap/self.rho0+radius).upper()
        return RiccatiTail(self.rho0, exponent, power, q0, radius, decay, p_cap)

    def _gl_q(self, power):
        r = self.rho0
        f = self.f_cap
        d = self.k2-1/r**3
        p = cap(power)
        assert d > 0
        leading = self.k2+(6+11/r)*(1+1/r)
        tail = 18*(1+1/r)**2/(d*d*r**6)+6*(4+5/r)*(1+1/r)/(d*r**3)
        coulomb = p*p*f*f + p*f*(1/r+f)
        return ((leading+tail+coulomb)/r**2).upper()

    def _massive_q(self, power):
        r = self.rho0
        f = self.f_cap
        c = self.c_d
        p = cap(power)
        jx = f*(1+2/r)        # |r^2 (f/r)_x|
        rcx = 8*self.k2*f/(self.dlow**2*r**2)  # |r (4/d)_x|
        common = cap(4-self.k2-power**2+power)
        common += (4+p*p*(2+1/r)+p+2*p*f)/r
        # Compare Y's derivative contribution to 6/r^2. The Z counterpart
        # uses (f/r)^2+(f/r)_x=f/r^3 exactly.
        y_remainder = (4*(2+1/r)+2+4*f)/r
        y_remainder += f*f*(4*c+c*c)+jx*c+f*rcx
        z_cap = f/r+f*f*(2*c+c*c)+jx*c+f*rcx
        ydiag = common+6+y_remainder
        zdiag = common+z_cap
        off = 4*cap(self.s1)*self.k*f/self.dlow
        return ((ydiag**2+zdiag**2+2*off**2).sqrt()/r**2).upper()

    def _source_caps(self):
        r = self.rho0
        f = self.f_cap
        d_gl = self.k2-1/r**3
        j = self.gl.p_cap/self.f_floor+(3*self.k2/d_gl+1)/r
        # Each metric coefficient below multiplies |u|/|r|.
        sphere = (ONE/2 + f*j/(self.k2*r)).upper()
        h_tt = f*(self.k2+ONE/(2*r**3))/d_gl + self.omega**2/self.k2
        h_tt += (f/(2*self.k2*r**2)+3*f*f/(d_gl*r))*j
        h_tx = f*self.omega/self.k2*(j+ONE/(2*r*(r-1)))
        h_xx = h_tt+2*f*sphere
        # Each baseline one-form coefficient multiplies |psi|/|r|.
        a_t = (self.rw.p_cap+f/r)/2
        a_x = cap(self.s2)/2
        self.x_tx = (6*sphere).upper()
        self.y_t = (4*(h_tt*a_t+h_tx*a_x)/self.f_floor).upper()
        self.y_x = (4*(h_tx*a_t+h_xx*a_x)/self.f_floor).upper()
        inv_droot = 1/self.dlow.sqrt()
        c_const = self.k*inv_droot/2
        a_const = f*(2+self.c_d)
        b_const = f*(1+self.c_d)
        # F/G position 0 coefficients carry rho^-2; position 1 carry rho^-1.
        self.f_y = (self.y_t*inv_droot).upper()
        self.f_z = (c_const*self.y_x).upper()
        self.g_y = ((cap(self.s1)*self.y_x+a_const*self.y_t/r)*inv_droot
                    + f*self.dhigh.sqrt()*self.x_tx).upper()
        self.g_z = (c_const*(cap(self.s1)*self.y_t+b_const*self.y_x/r)).upper()
        self.f0 = bound2(self.f_y/r**2, self.f_z/r)
        self.g0 = bound2(self.g_y/r**2, self.g_z/r)
        self.affine_force0 = (self.g0+self.mixed.p_cap*self.f0).upper()
        self.affine0 = (self.dx_cap*self.affine_force0
                        /(self.source_decay+self.mixed.decay)).upper()

    def fields(self, rho, u64, psi64):
        """Return outward caps for |u|, |u_x|, |psi|, |psi_x|."""
        u = self.gl.amplitude(rho, u64)
        psi = self.rw.amplitude(rho, psi64)
        return u, (self.gl.p_cap*u).upper(), psi, (self.rw.p_cap*psi).upper()

    def forcing(self, rho, u64, psi64):
        """Pointwise magnitude caps for [F_y,F_z], [G_y,G_z]."""
        u, _, psi, _ = self.fields(rho, u64, psi64)
        r = arb(rho)
        u_psi = u*psi
        return ((self.f_y*u_psi/r**2).upper(), (self.f_z*u_psi/r).upper()), (
            (self.g_y*u_psi/r**2).upper(), (self.g_z*u_psi/r).upper()
        )

    def affine_bound(self, u64, psi64, rho=None):
        """Euclidean |J-P_R U| cap for outgoing forced mixed solution."""
        r = self.rho0 if rho is None else arb(rho)
        self.mixed.riccati_radius(r)
        source = cap(acb(u64))*cap(acb(psi64))
        return (source*self.affine0*(self.rho0/r)
                *(-self.source_decay*(r-self.rho0)).exp()).upper()


    def mixed_state(self, rho, u64, psi64, mixed_u64):
        """Outgoing forced caps for the Euclidean norms of U and J."""
        r = arb(rho)
        self.mixed.riccati_radius(r)
        t = r-self.rho0
        gap = self.source_decay-self.mixed.decay
        assert gap > 0
        source = cap(acb(u64))*cap(acb(psi64))
        coefficient = self.dx_cap*(self.f0+self.affine0)*source/gap
        u = ((cap(acb(mixed_u64))+coefficient)*(-self.mixed.decay*t).exp()).upper()
        j = (self.mixed.p_cap*u+self.affine_bound(u64, psi64, r)).upper()
        return u, j


def main():
    ctx.prec = 128
    tail = OuterTail()
    for name in ("gl", "rw", "returning", "mixed"):
        spec = getattr(tail, name)
        print(f"{name}: Q_64={float(spec.q0):.9g} radius={float(spec.radius):.9g}"
              f" decay={float(spec.decay):.9g} p_cap={float(spec.p_cap):.9g}")
    for name in ("x_tx", "y_t", "y_x", "f_y", "f_z", "g_y", "g_z",
                 "source_decay", "affine_force0", "affine0"):
        print(f"{name}={float(getattr(tail, name)):.9g}")
    print("Unit endpoint amplitude affine bound", tail.affine_bound("1", "1"))
    assert tail.affine0 < arb("0.1")


if __name__ == "__main__":
    main()
