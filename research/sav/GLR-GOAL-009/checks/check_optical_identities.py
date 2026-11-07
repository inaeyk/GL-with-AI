"""Exact symbolic and manufactured checks for the GOAL-009 optical subsystem.

No spacetime evolution or sampled approximation to a vacuum solution.
"""

import sympy as s


def connection(metric, coords):
    inverse = metric.inv()
    indices = range(len(coords))
    return [
        [
            [
                s.cancel(sum(
                    inverse[i, k] * (
                        metric[k, j].diff(coords[l])
                        + metric[k, l].diff(coords[j])
                        - metric[j, l].diff(coords[k])
                    )
                    for k in indices
                ) / 2)
                for l in indices
            ]
            for j in indices
        ]
        for i in indices
    ]


def ricci_component(christoffel, coords, i, j):
    indices = range(len(coords))
    c = christoffel
    return s.expand(
        sum(c[k][i][j].diff(coords[k]) - c[k][i][k].diff(coords[j])
            for k in indices)
        + sum(c[k][k][l] * c[l][i][j] - c[k][j][l] * c[l][i][k]
              for k in indices for l in indices)
    )


def report_equal(name, lhs, rhs):
    assert s.simplify(lhs - rhs) == 0, name
    print(name + ": exact")


def reject(name, lhs, rhs):
    assert s.simplify(lhs - rhs) != 0, name
    print(name + ": rejected")


def optical_checks():
    u, R, z = s.symbols("u R z", real=True)
    coords = (u, R, z)
    B, V, A, b = [s.Function(label)(*coords) for label in ("B", "V", "A", "b")]
    metric = s.Matrix([[-B*V, -B, -B*A], [-B, 0, 0], [-B*A, 0, b**2]])
    inverse = metric.inv()
    c = connection(metric, coords)
    radius = R/s.sqrt(b)

    def warp_ric(i, j):
        hess = radius.diff(coords[i], coords[j]) - sum(
            c[k][i][j]*radius.diff(coords[k]) for k in range(3)
        )
        return ricci_component(c, coords, i, j) - 2*hess/radius

    ric_RR = warp_ric(1, 1)
    ric_Rz = warp_ric(1, 2)
    ric_zz = warp_ric(2, 2)
    q = s.log(b)
    beta = s.log(B*b)
    Q = q.diff(z) - A*q.diff(R)
    P = A.diff(R) + A*(beta.diff(R)-3*q.diff(R)) - beta.diff(z) + 3*q.diff(z)
    U = (-P + 2*Q)/(2*b)
    w = (-A/R - Q/2)/b
    T = 2*U + 2*w
    K_squared = 2*(U**2 + w**2)

    def normal(field):
        return (field.diff(z) - A*field.diff(R))/b

    h_coords = (u, R)
    h_inverse = s.Matrix([[0, -1/B], [-1/B, V/B]])
    h_density = B*radius**2
    box_h_b = sum(
        (h_density*h_inverse[i, j]*b.diff(h_coords[j])).diff(h_coords[i])
        for i in range(2) for j in range(2)
    )/h_density
    ric_nn = (ric_zz - 2*A*ric_Rz + A**2*ric_RR)/b**2

    report_equal("normal-normal Ricci", ric_nn, -normal(T)-K_squared-box_h_b/b)
    reject("reversed compact curvature sign", ric_nn, -normal(T)+K_squared-box_h_b/b)
    reject("omitted compact shift", ric_nn, -T.diff(z)/b-K_squared-box_h_b/b)
    reject("omitted lapse factor", ric_nn, -normal(T)-K_squared-box_h_b)

    def divergence(fields, variables):
        return sum(field.diff(variable) for field, variable in zip(fields, variables))

    currents = [
        sum(h_density*h_inverse[i, j]*b.diff(h_coords[j]) for j in range(2))
        for i in range(2)
    ]
    currents[1] -= h_density*A*T
    report_equal(
        "local compact divergence with density and Ricci defect",
        divergence(currents, h_coords)+(h_density*T).diff(z),
        b*h_density*(T**2-K_squared-ric_nn),
    )
    report_equal("optical radial lapse", ric_RR, 2*beta.diff(R)/R-3*q.diff(R)**2/2)
    report_equal(
        "optical radial shift",
        2*ric_Rz, P.diff(R)+2*P/R+(4*beta.diff(z)-6*q.diff(z))/R-3*q.diff(R)*q.diff(z),
    )

    density = B*b
    radius_grad = s.Matrix([radius.diff(coord) for coord in coords])
    box_radius = divergence(density*inverse*radius_grad, coords)/density
    ric_sphere = 1-(radius_grad.T*inverse*radius_grad)[0]-radius*box_radius
    report_equal(
        "optical radial potential",
        ric_sphere + R**2*ric_nn/(2*b),
        1-(R*V).diff(R)/(B*b)-R**2/b*(normal(T/2+w)+T**2/4+2*w**2),
    )
    report_equal("potential compact cancellation", T/2+w, (-P/2-2*A/R)/b)

    W = R*q
    report_equal(
        "optical evolution scalar operator",
        R*B*box_h_b/b,
        -2*W.diff(u, R)+(V*W.diff(R)).diff(R)-V.diff(R)*W/R,
    )


def manufactured_checks():
    r, eps, z = s.symbols("r eps z", real=True)
    k, f, H_squared = s.symbols("k f Hsq", nonzero=True)
    # The circle average uses phase z in [0, 2π]. The physical derivative
    # contributes the explicit k outside the circle average.
    avg_sin_sq = s.integrate(s.sin(z)**2, (z, 0, 2*s.pi))/(2*s.pi)
    D0 = f*r**2/2
    source_mean = -D0*k**2*H_squared*avg_sin_sq/4
    scalar_source = -s.sqrt(3)*source_mean/r
    report_equal("quadratic mean GL source", scalar_source, s.sqrt(3)*k**2*r*f*H_squared/16)
    reject("discarded quadratic zero mode", scalar_source, 0)

    # The flat pullback z_old = z + eps*sin(z) has the same circumference
    # for |eps| < 1. Its raw metric average is not the length-squared ratio.
    stretch = 1+eps*s.cos(z)
    report_equal("flat pullback length ratio", s.integrate(stretch, (z, 0, 2*s.pi))/(2*s.pi), 1)
    report_equal(
        "flat pullback coordinate metric mean",
        s.integrate(stretch**2, (z, 0, 2*s.pi))/(2*s.pi), 1+eps**2/2,
    )

    # The leaf densitized inverse is z-independent and its u normal is null.
    # The full inverse has a nonzero compact-shift correction.
    a = s.symbols("a", real=True)
    h = s.Matrix([[0, -1], [-1, 0]])
    X = s.Matrix([a, 0])
    full = h.row_join(h*X).col_join((X.T*h).row_join(s.Matrix([[1+(X.T*h*X)[0]]])))
    report_equal("mean-leaf null normal has positive full norm", full.inv()[0, 0], a**2)

    # Exact static monopole potential in units r0=1, from GOAL-003.
    a_mass = s.symbols("a_mass", positive=True)
    D = 1+a_mass*r**3
    f_bg = 1-1/r
    potential = (a_mass*f_bg+(6*r-11)*(r-1)/r**4
                 +18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D))
    # Brito–Cardoso–Pani v3, potential immediately following (30).
    bcp = f_bg*(1/r**3+a_mass+(12*(s.Rational(1, 2)-r)*a_mass
                                +6*r**3*(r-2)*a_mass**2)/D**2)
    report_equal("massive potential BCP convention", potential, bcp)
    horizon_bracket = s.limit(potential/f_bg, r, 1)
    report_equal("massive horizon bracket", horizon_bracket, (a_mass**2-4*a_mass+1)/(1+a_mass))
    for mode in (1, 2):
        value = horizon_bracket.subs(a_mass, (mode*s.pi/5)**2)
        # Use 3<π<22/7 so a∈[amin,2] and a²−4a+1 is decreasing.
        amin = (s.Rational(3*mode, 5))**2
        assert (s.Rational(22*mode, 35))**2 < 2
        assert amin**2-4*amin+1 < 0
        print(f"n={mode} horizon bracket: {float(value):.9f}")

    # Normalize the whole far region rather than extrapolate horizon sign.
    # Since r>=2, 4r-5>0. D>=a_mass*r**3 gives the lower bound below.
    # The first positive curvature term is nonnegative for r>=2.
    # V/f >= a_mass - 24/(a_mass*r**5).
    for mode in (1, 2):
        square = (mode*s.pi/5)**2
        lower = square - 24/(square*5**5)
        amin = (s.Rational(3*mode, 5))**2
        assert amin-24/(amin*5**5) > 0
        print(f"n={mode} far bracket lower bound for r>=5: {float(lower):.9f}")

    strain = s.Matrix([-2, -2, 4])/s.sqrt(3)
    report_equal("radion transverse strain trace", sum(strain), 0)
    report_equal("radion transverse strain norm", strain.dot(strain), 8)


if __name__ == "__main__":
    optical_checks()
    manufactured_checks()
