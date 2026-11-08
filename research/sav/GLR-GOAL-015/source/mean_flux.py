"""Local fixed-mass constraint from the quadratic Einstein flux."""


def quadratic_ricci_flux(r, k, omega, h, dh, d2h):
    """Return Q_vv + f Q_vr with the e^(2 omega v) factor removed."""
    a, b, c, w = h
    ar, br, cr, wr = dh
    arr = d2h[0]
    f, fr, frr = 1-1/r, 1/r**2, -2/r**3
    l = a+f*b
    m = b+f*c

    # Sum the vv and f vr slots before quadratic expansion. The
    # background Ricci scalar and Hessian contributions to this row cancel.
    curvature1 = (arr+omega**2*c-2*omega*br
                  + fr*(omega*c+2*br+f*cr)/2
                  + frr*(2*b+f*c)+fr*fr*c/2)
    cv = omega*b-ar/2-fr*m/2+f*(omega+fr)*c/2
    ct = (omega*(a+2*f*b)-f*ar-fr*(l+f*m))/2
    ct += f*(f*omega*c+ar+fr*m)/2
    hessian1 = (omega-fr/2)*omega*w+f*omega*wr-2*r*ct
    hessian2 = (2*r*l-omega*w)*cv+(2*r*m-wr)*ct
    gradient2 = omega*w*(omega*w+f*wr)/(2*r**4)-2*f*omega*w*w/r**5
    four = l*curvature1/2-hessian2/r**2+w*hessian1/r**4+gradient2
    trace4 = b+m+2*w/r**2
    compact = k*k*l*((b+m)/4-trace4/8)
    return four/2+compact


def local_mass(r, k, omega, jets):
    """Physical fixed-mass EF amplitude at v=0 for a decaying eigenmode."""
    return -r*r*quadratic_ricci_flux(r, k, omega, *jets)/(4*omega)
