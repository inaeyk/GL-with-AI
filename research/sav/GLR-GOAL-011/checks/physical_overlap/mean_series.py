"""Scalar-generic GOAL-011 mean Ricci source, in regular ingoing EF gauge.

The input h=(hvv,hvr,hrr,h_theta_theta) is the unhalved real-cosine
amplitude multiplying exp(omega*v), with analytic radial jets dh,d2h.
Both returned sources factor out exp(2*omega*v). All constants in the
calculation are integers to allow native ball and Taylor arithmetic.
"""


def ricci_from_jet(r, omega, k, h, dh, d2h):
    """Return the mean coefficients (Q_rr, Q_theta_theta, Q_zz)."""
    p, q, a, w = h
    dp, dq, da, dw = dh
    ddp, _, _, ddw = d2h
    inv_r = 1 / r
    inv_r2 = inv_r**2
    f, df, ddf = 1 - inv_r, inv_r2, -2 * inv_r**3

    # b^{-1} h has rows (q,a), (mixed_rv,upper_vr).
    # The first inverse variation is minus the symmetric upper tensor.
    mixed_rv = p + f * q
    upper_vr = q + f * a
    upper_rr = mixed_rv + f * upper_vr
    upper2_vv = a * (q + upper_vr)
    upper2_vr = q * upper_vr + a * upper_rr
    upper2_rr = upper_vr * (mixed_rv + upper_rr)
    angular = w * inv_r2
    trace4 = q + upper_vr + 2 * angular

    # Noncompact connection variations C^a_bc needed for W=R^2.
    c_vr_v = (omega + df) * a / 2
    c_vv_r = (omega * (p + 2 * f * q) - f * dp - df * upper_rr) / 2
    c_vr_r = (f * omega * a + dp + df * upper_vr) / 2
    c_rr_r = dq + (f * da - omega * a) / 2
    grad_v = omega * w
    hess1_vv = omega * grad_v - df * (grad_v + f * dw) / 2 - 2 * r * c_vv_r
    hess1_vr = (omega + df / 2) * dw - 2 * r * c_vr_r
    hess1_rr = ddw - 2 * r * c_rr_r

    # The second connection is -b^{-1}h C. Thus Hess(W)_2 uses
    # the contracted first connection with the following two weights.
    weight_v = 2 * r * mixed_rv - grad_v
    weight_r = 2 * r * upper_vr - dw
    hess2_vr = weight_v * c_vr_v + weight_r * c_vr_r
    hess2_rr = weight_v * da / 2 + weight_r * c_rr_r

    # In 2D, Ric[b]_rr = b_rr Scal[b]/2. The background b_rr is zero.
    curvature1 = (
        ddp + omega**2 * a - 2 * omega * dq
        + df * (omega * a + 2 * dq + f * da) / 2
        + ddf * (2 * q + f * a) + df**2 * a / 2
    )
    rr4 = (
        a * curvature1 / 2 - hess2_rr * inv_r2
        + (2 * w * hess1_rr + dw**2) * inv_r2**2 / 2
        + 4 * w * (w * inv_r - dw) * inv_r**5
    )
    sphere4 = -(
        2 * hess2_vr + f * hess2_rr
        - a * hess1_vv - 2 * upper_vr * hess1_vr - upper_rr * hess1_rr
        + r * df * (2 * upper2_vr - f * upper2_vv) + 2 * upper2_rr
    ) / 2

    # Noncompact terms get the cos^2 mean. Compact terms already
    # contain the sin^2 mean from the spacelike z Gauss terms.
    k2 = k**2
    compact_rr = k2 * (upper2_vv / 4 - trace4 * a / 8)
    compact_sphere = k2 * (w * angular / 4 - trace4 * w / 8)
    trace_square = q**2 + upper_vr**2 + 2 * a * mixed_rv + 2 * angular**2
    return rr4 / 2 + compact_rr, sphere4 / 2 + compact_sphere, -k2 * trace_square / 8


def ricci_mean(gl, r):
    """Adapt gl.omega, gl.k, and gl.jet(r) to ricci_from_jet."""
    return ricci_from_jet(r, gl.omega, gl.k, *gl.jet(r))


def source(gl, r):
    """Return the mean Einstein-source pair (T^v_v, T_rr)."""
    q_rr, q_sphere, q_zz = ricci_mean(gl, r)
    inv_r = 1 / r
    return (1 - inv_r) * q_rr / 2 + q_sphere * inv_r**2 + q_zz / 2, -q_rr
