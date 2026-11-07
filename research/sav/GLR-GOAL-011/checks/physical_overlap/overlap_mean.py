"""GOAL-011 fixed-mass Einstein-frame GL monopole, r0=1.

The GL jet is the real cosine amplitude in regular ingoing EF coordinates:
gl.jet(r) -> (h, h_r, h_rr), each ordered (hvv, hvr, hrr, h_theta_theta).
The factored time dependence of that jet is exp(gl.omega*v).
"""
import numpy as np
from scipy.integrate import solve_ivp


def ricci_mean(gl, r):
    """Return (Q_rr,Q_theta_theta,Q_zz) factoring out exp(2*omega*v)."""
    (p, q, a, w), (dp, dq, da, dw), (ddp, _, _, ddw) = gl.jet(r)
    omega, k = gl.omega, gl.k
    f, df, ddf = 1 - 1 / r, 1 / r**2, -2 / r**3
    inverse = np.array([[0, 1], [1, f]])
    h = np.array([[p, q], [q, a]])
    mixed = inverse @ h
    upper = mixed @ inverse
    trace4 = np.trace(mixed) + 2 * w / r**2

    # Two-dimensional connection variation C^i_jk, in (v,r) order.
    c_vv_v = omega * q - dp / 2 - df * (q + f * a) / 2
    c_vr_v = (omega + df) * a / 2
    c_rr_v = da / 2
    c_vv_r = (
        omega * (p / 2 + f * q) - f * dp / 2
        - df * (p + 2 * f * q + f**2 * a) / 2
    )
    c_vr_r = (f * omega * a + dp + df * (q + f * a)) / 2
    c_rr_r = dq + (f * da - omega * a) / 2
    connection = np.array([
        [[c_vv_v, c_vr_v], [c_vr_v, c_rr_v]],
        [[c_vv_r, c_vr_r], [c_vr_r, c_rr_r]],
    ])
    second_connection = -(mixed @ connection.reshape(2, 4)).reshape(2, 2, 2)

    # Expansion of the Hessian of W=R^2 with W_0=r^2 and W_1=w.
    hess0 = np.array([[-r * f * df, r * df], [r * df, 2]])
    grad1 = np.array([omega * w, dw])
    hess1 = np.array([
        [omega**2 * w - df * (omega * w + f * dw) / 2,
         (omega + df / 2) * dw],
        [(omega + df / 2) * dw, ddw],
    ]) - 2 * r * connection[1]
    hess2 = -2 * r * second_connection[1] - np.einsum(
        "ijk,i->jk", connection, grad1
    )

    # Since b_rr,0=0 and Ric[b]_ij=b_ij Scal[b]/2, only Scal[b]_1 enters.
    curvature1 = (
        ddp + omega**2 * a - 2 * omega * dq
        + df * (omega * a / 2 + dq + f * da / 2)
        + ddf * (2 * q + f * a) + df**2 * a / 2
    )
    rr4 = (
        a * curvature1 / 2 - hess2[1, 1] / r**2
        + (w * hess1[1, 1] + dw**2 / 2) / r**4
        - (w**2 * hess0[1, 1] + 4 * r * w * dw) / r**6
        + 6 * w**2 / r**6
    )
    sphere4 = -np.sum(inverse * hess2 - upper * hess1
                     + (mixed @ upper) * hess0) / 2

    # Four-dimensional terms are cos^2 averaged. Compact Gauss terms
    # below are already sin^2 averaged, with the trace kept for checks.
    compact_rr = k**2 * ((h @ mixed)[1, 1] / 4 - trace4 * a / 8)
    compact_sphere = k**2 * (w**2 / (4 * r**2) - trace4 * w / 8)
    zz = -k**2 / 8 * (np.trace(mixed @ mixed) + 2 * (w / r**2)**2)
    return rr4 / 2 + compact_rr, sphere4 / 2 + compact_sphere, zz


def source(gl, r):
    """Return (T^v_v,T_rr), the mean Einstein source factoring exp(2*omega*v)."""
    q_rr, q_sphere, q_zz = ricci_mean(gl, r)
    return (1 - 1 / r) * q_rr / 2 + q_sphere / r**2 + q_zz / 2, -q_rr


class MeanGeometry:
    """Backward contour quadratures with a zero finite outer endpoint.

    mean(r) is (mu,beta) multiplying exp(2*omega*t), with tensor
    coefficients p_vv=2*mu/r-2*f*beta, p_vr=beta in ingoing EF gauge.
    jet(r) also returns derivatives of the two static-time amplitudes.
    gl.rmin is the smallest real radius supported by the GL jet.
    """

    def __init__(self, gl, rmatch, ray, length, tol):
        self.gl = gl
        self.rmatch = float(rmatch)
        self.ray = complex(ray)
        self.length = float(length)
        self.tol = float(tol)
        self.rmin = float(gl.rmin)
        if not (1 < self.rmin <= self.rmatch and self.ray.real > 0
                and self.length > 0 and 0 < self.tol < 1):
            raise ValueError("Require 1 < rmin <= rmatch, Re(ray)>0, length>0 and 0<tol<1")
        self.outer = self.rmatch + self.ray * self.length
        zero = np.zeros(2, dtype=complex)
        self._ray_solution = self._solve(
            lambda u, _: self.ray * self._rhs(self.rmatch + self.ray * u),
            (self.length, 0), zero, max_step=0.75 / abs(self.ray)
        )
        self._match = self._ray_solution.sol(0)
        self._real_solution = None
        if self.rmin < self.rmatch:
            self._real_solution = self._solve(
                lambda r, _: self._rhs(r), (self.rmatch, self.rmin), self._match
            )

        decay = 2 * (np.sqrt(gl.k**2 + gl.omega**2) + gl.omega)
        # Leading-exponential diagnostic, without a polynomial or interval bound.
        self.tail_proxy_ef = np.abs(self._rhs(self.outer)) / decay
        self.match_ef = self._match.copy()
        self.nfev = self._ray_solution.nfev + (
            self._real_solution.nfev if self._real_solution is not None else 0
        )

    def _solve(self, rhs, interval, initial, max_step=0.75):
        result = solve_ivp(rhs, interval, initial, method="DOP853",
                           rtol=self.tol, atol=0.01 * self.tol,
                           max_step=max_step, dense_output=True)
        if not result.success:
            raise RuntimeError("Mean geometry quadrature failed: " + result.message)
        return result

    def _rhs(self, r):
        r = complex(r)
        if r.imag == 0:
            r = r.real
        tvv, trr = source(self.gl, r)
        return np.array([-r**2 * tvv / 2, r * trr / 2], dtype=complex)

    def _ef(self, r):
        r = complex(r)
        u = (r - self.rmatch) / self.ray
        slack = 1e-11 * max(1, abs(u), self.length)
        if abs(u.imag) <= slack and -slack <= u.real <= self.length + slack:
            return self._ray_solution.sol(np.clip(u.real, 0, self.length))
        if r.imag == 0 and self.rmin <= r.real <= self.rmatch:
            return self._real_solution.sol(r.real)
        if r.imag == 0 and r.real > self.rmatch:
            # Available when gl supplies a real profile beyond rmatch.
            # At large radii this continuation can subtract similar numbers.
            return self._solve(lambda x, _: self._rhs(x), (self.rmatch, r.real),
                               self._match).sol(r.real)
        raise ValueError("Radius outside the real interval and supplied contour")

    def _time_factor(self, r):
        return np.exp(2 * self.gl.omega * (r + np.log(r - 1)))

    def __call__(self, r):
        return tuple(self._time_factor(r) * self._ef(r))

    def jet(self, r):
        ef = self._ef(r)
        factor = self._time_factor(r)
        rate = 2 * self.gl.omega / (1 - 1 / r)
        return tuple(factor * ef), tuple(factor * (self._rhs(r) + rate * ef))


def _check_manufactured():
    # Static W=r^2+epsilon*cos(k*z) and unperturbed (v,r,z) metric.
    class SphereOffset:
        omega, k, rmin = 0.0, 2 * np.pi / 10, 1.000001

        def jet(self, r):
            return (0, 0, 0, 1), (0, 0, 0, 0), (0, 0, 0, 0)

    gl = SphereOffset()
    worst = 0.0
    for ray in (1.0, np.exp(0.35j)):
        mean = MeanGeometry(gl, 3.0, ray, 24.0, 1e-11)
        end = mean.outer
        for r in (gl.rmin, 2.0, 3.0, 3.0 + 2.37 * ray):
            q_expected = (2 / r**6, 0, -gl.k**2 / (4 * r**4))
            np.testing.assert_allclose(ricci_mean(gl, r), q_expected,
                                       atol=1e-15, rtol=1e-13)
            delta3, delta4 = r**-3 - end**-3, r**-4 - end**-4
            expected = (delta3 / 6 - delta4 / 8
                        - gl.k**2 * (1 / r - 1 / end) / 16, delta4 / 4)
            error = np.max(np.abs(np.array(mean(r)) - expected))
            worst = max(worst, error)
    assert worst < 1e-11, worst
    print(f"Manufactured finite-end mean quadrature max error: {worst:.3g}")


if __name__ == "__main__":
    _check_manufactured()
