"""Independent source, graph and weak-tail checks for GLR-GOAL-014."""
from platform import python_version

from flint import arb, ctx
import flint
import sympy as sp


def require_equal(lhs, rhs):
    delta = lhs - rhs
    if isinstance(delta, sp.MatrixBase):
        assert all(sp.simplify(entry) == 0 for entry in delta), delta
    else:
        assert sp.simplify(delta) == 0, delta


def densities():
    # Every eta-selfadjoint mixed tensor with the GL trace relation.
    e, h, j, d, mu, beta, scale = sp.symbols("e h j d mu beta f", real=True)
    eta = sp.diag(-1, 1)
    identity = sp.eye(2)
    x = sp.Matrix([[h, j], [-j, -4*d-h]])
    jacobian = identity + e*x
    sphere = 1 + 2*e*d
    volume = sp.sqrt(jacobian.det())
    m = volume*sphere*jacobian.inv()*eta
    p = sphere**2/volume
    taylor = lambda expression, order: sp.diff(expression, e, order).subs(e, 0)/sp.factorial(order)
    require_equal(m.applyfunc(lambda term: taylor(term, 1)), -x*eta)
    require_equal(taylor(p, 1), 6*d)
    require_equal(m.applyfunc(lambda term: taylor(term, 2))/2,
                  (x*x - (2*d*d + sp.trace(x*x)/4)*identity)*eta/2)
    require_equal(taylor(p, 2)/2, (14*d*d + sp.trace(x*x)/4)/2)

    # EF mean as a tensor transformed by dv=dt+dx, dr=f dx.
    r = sp.symbols("r", positive=True)
    ingoing = sp.Matrix([[2*mu/r-2*scale*beta, beta], [beta, 0]])
    ef_from_tortoise = sp.Matrix([[1, 1], [0, scale]])
    perturbation = ef_from_tortoise.T*ingoing*ef_from_tortoise
    y = eta*perturbation/scale
    rank_one = (beta-2*mu/(r*scale))*sp.Matrix([[1, -1], [-1, 1]])
    require_equal(sp.trace(y)/2, beta)
    require_equal((sp.trace(y)*identity/2-y)*eta, rank_one)
    wt, wx, w, lam = sp.symbols("w_t w_x w lam", real=True)
    a = sp.Matrix([wx+scale*w/r, wt])/(2*r)
    dt, dx = lam*r*r*rank_one*a
    fcurrent = 2*(dx-dt)/(lam*r)
    gcurrent = 2*(dx+dt)/(lam*r)
    require_equal(fcurrent, 2*(beta-2*mu/(r*scale))*(wt-wx-scale*w/r))
    require_equal(gcurrent, 0)
    qcurl = -beta*lam*r*w/2
    hcurrent = 2*scale*dt/(lam*r*r)-2*scale*qcurl/r**3
    require_equal(hcurrent, -scale*fcurrent/(2*r) + lam*scale*beta*w/r**2)
    print("exact density expansion, Fourier factor 1/2, rank-one mean and F,G,H: PASS")


def graph_and_operator():
    r, lam, k = sp.symbols("r lam k", positive=True)
    p, jt, jx, dt, dx = sp.symbols("p Jt Jx Dt Dx", real=True)
    f = 1-1/r
    at, ax, ut, zt = sp.symbols("At Ax Ut Zt")
    dk = lam+k*k*r*r
    big_d = r*r*dk
    cap_w = lam*r**4/dk
    lc = r**4/big_d
    graph = sp.solve([
        big_d*at - jt - p*ut + sp.I*k*r**4*zt,
        big_d*ax + jx + ut - p*sp.I*k*r**4*zt,
        ax+p*at,
        (1-p*p)*zt-sp.I*k*(at+p*ax),
    ], [at, ax, ut, zt])
    require_equal(graph[ut], (p*jt-jx)/(1-p*p))
    require_equal(graph[zt], sp.I*k*(jt-p*jx)/(lam*r*r*(1-p*p)))
    require_equal((at+p*ax).subs(graph), (jt-p*jx)/(lam*r*r))

    def radial(expression):
        return f*sp.diff(expression, r)

    coupling = sp.I*k*sp.sqrt(big_d)*radial(lc)/sp.sqrt(cap_w)
    require_equal(coupling, 2*sp.I*k*sp.sqrt(lam)*f/dk)
    require_equal(coupling+sp.conjugate(coupling), 0)
    a = radial(sp.sqrt(big_d))/sp.sqrt(big_d)
    require_equal((f*(k*k+lam/r**2) + a*a-radial(a)).subs({k: 0, lam: 4}),
                  f*(6/r**2-3/r**3))
    p_graph = -1/r
    t_density = (dt-p_graph*dx)/f
    require_equal(dx+(1/r)/(1+1/r)*t_density, (dx-p_graph*dt)/(1-p_graph**2))
    graph_velocity = 2*(dx-p*dt)/(lam*r*(1-p*p))
    bprime = -(1+p)/(1-p)
    fcurrent, gcurrent = 2*(dx-dt)/(lam*r), 2*(dx+dt)/(lam*r)
    require_equal(4*(1+p)*graph_velocity/2, gcurrent-bprime*fcurrent)
    print("canonical mixed and mean graph jets, EF density Jacobian, coupling and RW limit: PASS")


def directed_tails():
    ctx.prec = 192
    assert (1-arb(1)/100)*(arb.pi()/5)**2 > arb(1)/4
    # Flux bound of each component pair, using dr/dv=f/2 on every ray.
    assert 3*arb(2).sqrt()+2 < 8
    assert 2*(4+1+arb(4)/100) < 12
    # Strict slack after absorbing the exterior radial factor.
    assert arb(2)/5*arb("0.49") > arb(1)/6
    assert arb(4)/5*arb("0.49")-arb(1)/212 > arb(1)/3
    assert arb(1)/210 < arb(1)/12
    for r in (100, 350, 400):
        mixed = 12*10**53*(-arb(2)*(r-4)/5).exp()
        scalar = 8*10**21*(100+10**9*(r+6))*(r+5)*(-arb(4)*(r-4)/5).exp()
        ray = arb(3).sqrt()*mixed+arb(2).sqrt()*scalar
        trace = arb(2).sqrt()*(mixed+scalar)
        print(f"r={r} ray {ray.str(14)}, trace {trace.str(14)}")
        if r == 400:
            assert ray < arb("3.354e-15") and trace < arb("2.739e-15")


def divergence_free_control():
    u, v, amplitude = sp.symbols("u v B", real=True)
    stream = amplitude*u*sp.exp(1-v)
    fcurrent = sp.diff(stream, v)
    gcurrent = -sp.diff(stream, u)
    require_equal(sp.diff(fcurrent, u)+sp.diff(gcurrent, v), 0)
    # On v>=1, with sharp cut u in [0,1] and g(u)=u, W=0.
    # Localized smooth continuation below c=1 can vanish near any graph.
    terminal = sp.integrate(fcurrent.subs(u, 1), (v, 1, sp.oo))
    bulk = -sp.integrate(sp.integrate(fcurrent, (v, 1, sp.oo)), (u, 0, 1))
    cap = -sp.integrate(u*gcurrent.subs(v, 1), (u, 0, 1))
    require_equal(terminal, -amplitude)
    require_equal(bulk, amplitude/2)
    require_equal(cap, amplitude/2)
    require_equal(terminal+bulk+cap, 0)
    print("divergence-free exterior control: terminal=-B, bulk=B/2, cap=B/2, source=0")


if __name__ == "__main__":
    print(f"Python {python_version()}, SymPy {sp.__version__}, python-flint {flint.__version__}")
    densities()
    graph_and_operator()
    directed_tails()
    divergence_free_control()
