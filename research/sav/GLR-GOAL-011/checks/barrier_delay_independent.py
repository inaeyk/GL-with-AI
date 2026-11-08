"""Bounded independent L10 optical and smooth-packet controls.

Run: python3 GLR-GOAL-011/checks/barrier_delay_independent.py
The direct y-ODE implementation uses no project Python modules.
"""
from pathlib import Path
import sys

import numpy as np
import scipy
from scipy.integrate import quad, solve_ivp
from scipy.interpolate import CubicSpline

PROFILE = Path(__file__).resolve().parents[2] / 'verification/gl_linear_profile_L10.csv'
K2 = (np.pi / 5)**2
OMEGA = 0.06328118897590773
RADII = (13/8, 7/4)


def x(r):
    return r + np.log(r - 1)


def outgoing_clock(r):
    return r - 2*x(r)


def solve_radial(end, tol, omega):
    q = np.hypot(np.sqrt(K2), omega)
    beta = (K2 + 2*omega**2) / (2*q)
    # y ~ (1 + k²r³)/r * exp(-qr) r^-beta.
    slope = -q - (beta + 1)/end + 3*K2*end**2/(1 + K2*end**3)

    def ode(r, state):
        d, ell = state
        f = 1 - 1/r
        denominator = 1 + K2*r**3
        p_y = 1/(r-1) - 5/r + 6/(r*denominator)
        q_y = 3/(r*r*(r-1)) - K2/f - 3/(r*r*(r-1)*denominator)
        return (omega**2/f**2 - q_y - p_y*d - d*d, d)

    result = solve_ivp(ode, (end, min(RADII)), (slope, 0.0), method='DOP853',
                       dense_output=True, rtol=tol, atol=tol/20, max_step=0.4)
    assert result.success
    d2, ell2 = result.sol(2.0)
    y2 = 8 / (-2/(1+8*K2) - d2/(K2*(1+8*K2)))

    def metric(r):
        d, ell = result.sol(r)
        y = y2*np.exp(ell-ell2)
        yp = d*y
        f = 1 - 1/r
        denominator = 1 + K2*r**3
        h_tt = ((f*(0.5-K2*r**3)/denominator**2 - omega**2/(K2*denominator))*y
                + ((r-1)/(2*K2*r**3*denominator) - 3*(r-1)**2/denominator**2)*yp)
        h_tr = -omega/(K2*denominator)*(yp-y/(2*r*(r-1)))
        h_ang = -r*r*y/(2*denominator) - (r-1)*yp/(K2*denominator)
        h_rr = h_tt/f**2 - 2*h_ang/(r*(r-1))
        return np.array([h_tt, h_tr, h_rr, h_ang])

    return metric


def make_csv_interpolator():
    data = np.genfromtxt(PROFILE, delimiter=',', names=True)
    sampled = data[data['r'] >= 1.35]
    metric = CubicSpline(sampled['r'], np.column_stack([
        sampled['Htt'], sampled['Htr'], sampled['Hrr'], sampled['Htheta']]))
    return metric, float(sampled['r'][-1])


def ray_integrand(r, radius, omega, metric):
    f = 1 - 1/r
    htt, htr, hrr, _ = metric(r)
    # eikonal coefficient: e^(Omega t) H(L,L)/(2 f²) along the ray.
    return np.exp(omega*(outgoing_clock(radius)+x(r))) * (htt+2*f*htr+f*f*hrr)/(2*f*f)


def integrate_ray(radius, omega, metric, end, knots=None):
    return quad(ray_integrand, radius, end, args=(radius, omega, metric),
                epsabs=2e-11, epsrel=2e-11, points=knots, limit=1000)[0]


def bump_jet(s):
    """b=exp(4-1/[s(1-s)]) inside (0,1), flat outside."""
    if not 0 < s < 1:
        return np.zeros(4)
    f = s*(1-s)
    first = 1-2*s
    ell1 = first/f**2
    ell2 = -2/f**2 - 2*first**2/f**3
    ell3 = 12*first/f**3 + 6*first**3/f**4
    b = np.exp(4-1/f)
    return b*np.array([1, ell1, ell2+ell1**2, ell3+3*ell1*ell2+ell1**3])


def control():
    def integrate(function, a=0.0, b=1.0):
        return quad(function, a, b, epsabs=5e-10, epsrel=2e-12)[0]

    return (
        integrate(lambda s: bump_jet(s)[2]**2),
        integrate(lambda s: bump_jet(s)[1]*bump_jet(s)[3]),
        integrate(lambda s: bump_jet(s)[1]*bump_jet(s)[2]),
        integrate(lambda s: bump_jet(s)[1]*bump_jet(s)[3], b=.16),
    )


def main():
    print(f'Python {sys.version.split()[0]}, SciPy {scipy.__version__}, NumPy {np.__version__}')
    print('r0=1, L=10, Htheta(2)=8, omega=', OMEGA)
    print('R, u*, t_R:', [(r, outgoing_clock(r), -np.log(r-1)) for r in RADII])
    for end, tol in [(30., 2e-8), (45., 2e-10), (65., 2e-12)]:
        metric = solve_radial(end, tol, OMEGA)
        print('ODE', end, tol, 'Htheta(2):', metric(2.0)[3])
        for radius in RADII:
            delay = integrate_ray(radius, OMEGA, metric, end)
            print(f' R={radius:g}: chi={delay:.13g}, chi²/4={delay**2/4:.13g}')
        if end == 65:
            # Inherited exact p bounds: -.9<p<0 for r>=2 and p<-.59
            # for r>=10. For r>=65 the metric component formulas give
            # |Htt/f+2Htr+f Hrr| <= 4*|U|/r.
            for radius in RADII:
                delay = integrate_ray(radius, OMEGA, metric, end)
                lower_limit_slope = 1/(1-2/(1-1/radius))
                derivative = (OMEGA*delay
                              - ray_integrand(radius, radius, OMEGA, metric)*lower_limit_slope)
                time_zero_delay = delay*np.exp(OMEGA*np.log(radius-1))
                tail_bound = (2*250*np.exp(-.59*(x(end)-x(10))
                                           + .07*(outgoing_clock(radius)+x(end)))
                              / (end*(1-1/end)*(.59-.07)))
                print(f' R={radius:g}: dchi/du={derivative:.12g}, '
                      f't=0 slice chi={time_zero_delay:.12g}, |tail beyond 65|<{tail_bound:.3g}')
            for radius in RADII:
                edges = [radius, 2., 3., 5., 10., 20., 40., end]
                pieces = [quad(ray_integrand, a, b, args=(radius, OMEGA, metric),
                               epsabs=1e-24, epsrel=3e-11)[0]
                          for a,b in zip(edges[:-1], edges[1:])]
                print(f' R={radius:g}: radial bin edges={edges}, signed pieces={pieces}')
    for omega in (0.06328117, 0.06328121):
        metric = solve_radial(45., 2e-11, omega)
        print('omega endpoint', omega, [integrate_ray(r, omega, metric, 45.) for r in RADII])
    metric, end = make_csv_interpolator()
    knots = metric.x
    print('separate pinned-component spline', [integrate_ray(
        r, OMEGA, metric, end, knots[(knots > r) & (knots < end)]) for r in RADII])
    print('smooth bump: int bpp^2, int bp*bppp, int bp*bpp, int_0^.16 bp*bppp:', control())


if __name__ == '__main__':
    main()
