import json
import math
from itertools import product
from subprocess import check_output
from flint import arb, acb, arb_series, ctx
ctx.prec=144
ctx.cap=10
PIN='80f030df0bec99bebe76fd9ab473c419ad1af5a5'
omega=arb('0.06328119','0.00000001')
k=arb.pi()/5
a=k*k
I=acb(0,1)
def pin(path):
    return check_output(['git','show',f'{PIN}:research/sav/{path}'],text=True)

def mode(r0, y0, yp0, omega):
    """Return unhalved EF profiles in exp(omega*v) convention, radial jets."""
    r=arb_series([r0,1])
    f=(r-1)/r
    D=1+a*r**3
    b=2*omega+1/r-5*f/r+6*f/(r*D)
    c=omega*(-4+6/D)/r+3/r**3-a-3/(r**3*D)
    y=arb_series([y0])
    if r0==1:
        for n in range(7):
            y[n+1]=-(f*y.derivative().derivative()+b*y.derivative()+c*y)[n]/((n+1)*(n+1+2*omega))
    else:
        y[1]=yp0
        for n in range(6):
            y[n+2]=-(f*y.derivative().derivative()+b*y.derivative()+c*y)[n]/(f[0]*(n+1)*(n+2))
    Z=-omega/(a*D)*(f*y.derivative()+(omega-1/(2*r*r))*y)
    B=-Z.derivative()/omega-2*Z/(omega*r)
    T=Z-f*B
    S=r*Z/omega-y/(2*a*r)
    N=2*Z.derivative()/omega+2*Z/(omega*r)+y/(a*r**3)
    if r0==1:
        assert N[0].contains(0)
        # N(1)=0 identically for the exact Frobenius solution.
        C=r*arb_series([N[n+1] for n in range(4)])
    else:
        C=N/f
    return T,B,C,S

class Jet:
    """Finite multivariate Taylor ring, coefficients in complex Arb balls.

    Keys (harmonic,radial degree,polar angle degree). Retain harmonics 0..2;
    for the positive branch its mode number also equals perturbation order.
    Radial and angle jets carry coefficients, not raw derivatives.
    """
    def __init__(self, coeff=None):
        self.coeff = {} if coeff is None else {key:val for key,val in coeff.items() if not acb(val).is_zero()}
    def __add__(self, rhs):
        if not isinstance(rhs,Jet):
            rhs=Jet({(0,0,0):rhs})
        out=self.coeff.copy()
        for key,val in rhs.coeff.items():
            out[key]=out.get(key,0)+val
        return Jet(out)
    __radd__=__add__
    def __neg__(self):
        return Jet({key:-val for key,val in self.coeff.items()})
    def __sub__(self,rhs):
        return self + (-rhs)
    def __mul__(self,rhs):
        if not isinstance(rhs,Jet):
            return Jet({key:val*rhs for key,val in self.coeff.items()})
        out={}
        for (n,j,l),x in self.coeff.items():
            for (m,i,h),y in rhs.coeff.items():
                if n+m<=2 and j+i+l+h<=3:
                    key=(n+m,j+i,l+h)
                    out[key]=out.get(key,0)+x*y
        return Jet(out)
    __rmul__=__mul__
    def d(self,coord,omega):
        if coord==0 or coord==2:
            factor=omega if coord==0 else I*k
            return Jet({key:val*(factor*key[0]) for key,val in self.coeff.items() if key[0]})
        if coord==1:
            return Jet({(n,j-1,l):j*val for (n,j,l),val in self.coeff.items() if j})
        if coord==3:
            return Jet({(n,j,l-1):l*val for (n,j,l),val in self.coeff.items() if l})
        return Jet()
    def select(self,n):
        return Jet({key:val for key,val in self.coeff.items() if key[0]==n})
    def val(self,n):
        return acb(self.coeff.get((n,0,0),0))

def jr(series,n=0):
    return Jet({(n,j,0):acb(series[j]) for j in range(4)})
def jd(arr):
    return Jet({(0,0,l):acb(val) for l,val in enumerate(arr)})

def calc(r0, profiles, omega, angle=False, half=arb('0.5'), bz_mutation=False):
    """Coordinate Taylor expansion of all five-dimensional Ricci blocks."""
    r=arb_series([r0,1])
    f=jr((r-1)/r)
    r2=jr(r*r)
    rinv2=jr(1/(r*r))
    one=jd([1])
    # angle=False is pi/2, True is pi/3.  Include polar-angle derivatives.
    sn2=([arb(3)/4,arb(3).sqrt()/2,-arb(1)/2,-arb(3).sqrt()/3]
         if angle else [1,0,-1,0])
    sq=jd(sn2)
    sn=arb_series(sn2)
    invsq=jd([(1/sn)[j] for j in range(4)])
    T,B,C,S=(jr(x*half,1) for x in profiles)
    indices=range(5)
    pairs=list(product(indices, repeat=2))
    triples=list(product(indices, repeat=3))
    g0={(i,j):Jet() for i,j in pairs}
    g0.update({(0,0):-f,(0,1):one,(1,0):one,(2,2):one,(3,3):r2,(4,4):r2*sq})
    gi={(i,j):Jet() for i,j in pairs}
    gi.update({(0,1):one,(1,0):one,(1,1):f,(2,2):one,(3,3):rinv2,(4,4):rinv2*invsq})
    h={(i,j):Jet() for i,j in pairs}
    h.update({(0,0):T,(0,1):B,(1,0):B,(1,1):C,(3,3):S,(4,4):S*sq})
    g={pair:g0[pair]+h[pair] for pair in pairs}
    def mm(A,B):
        return {(i,j):sum(A[i,c]*B[c,j] for c in indices) for i,j in pairs}
    gh=mm(gi,h)
    inv1=mm(gh,gi)
    inv2=mm(gh,inv1)
    inv={pair:gi[pair]-inv1[pair]+inv2[pair] for pair in pairs}
    dg={(a,b,c):g[b,c].d(a,omega) for a,b,c in triples}
    G={(c,a,b):sum(inv[c,d]*(dg[a,d,b]+dg[b,d,a]-dg[d,a,b]) for d in indices)*arb('0.5')
       for c,a,b in triples}
    R={}
    for i,j in pairs:
        R[i,j]=sum(G[c,i,j].d(c,omega)-G[c,i,c].d(j,omega) for c in indices)
        R[i,j]+=sum(G[c,c,d]*G[d,i,j]-G[c,j,d]*G[d,i,c] for c,d in pairs)
    Q={pair:R[pair].select(2) for pair in pairs}
    if bz_mutation:
        Q[2,2]=Jet()
    tr=sum(gi[i,j]*Q[i,j] for i,j in pairs)
    E={pair:Q[pair]-g0[pair]*tr*arb('0.5') for pair in pairs}
    Em=mm(gi,E)
    G0={key:G[key].select(0) for key in G}
    div=[sum(Em[i,j].d(i,omega) for i in indices)
         +sum(G0[i,i,c]*Em[c,j]-G0[c,i,j]*Em[i,c] for i,c in pairs) for j in indices]
    hsq=sum(gh[i,j]*gh[j,i] for i,j in pairs)
    qzz_expected=hsq*(-3*a/4) # hsq already contains the two Fourier halves
    return Q,E,div,R,qzz_expected

def warped(r0, profiles, omega):
    """Independent warped-product route with a 2x2 orbit inverse."""
    r=arb_series([r0,1])
    f=jr((r-1)/r)
    r2=jr(r*r)
    rinv2=jr(1/(r*r))
    one=jd([1])
    T,B,C,S=(jr(x*arb('0.5'),1) for x in profiles)
    orbit=range(3)
    pairs=list(product(orbit, repeat=2))
    g={(i,j):Jet() for i,j in pairs}
    g.update({(0,0):T-f,(0,1):one+B,(1,0):one+B,(1,1):C,(2,2):one})
    ddet=(T-f)*C-(one+B)*(one+B)+one  # det(vr block) = -1 + ddet
    idet=-one-ddet-ddet*ddet
    inv={(i,j):Jet() for i,j in pairs}
    inv.update({(0,0):C*idet,(0,1):-(one+B)*idet,(1,0):-(one+B)*idet,
                (1,1):(T-f)*idet,(2,2):one})
    W=r2+S
    rel=rinv2*S
    iW=rinv2*(one-rel+rel*rel)
    dg={(i,j,k):g[j,k].d(i,omega) for i,j,k in product(orbit, repeat=3)}
    G={(c,i,j):sum(inv[c,k]*(dg[i,k,j]+dg[j,k,i]-dg[k,i,j])
                    for k in orbit)*arb('0.5') for c,i,j in dg}
    dW=[W.d(c,omega) for c in orbit]
    Hess={(i,j):dW[i].d(j,omega)-sum(G[c,i,j]*dW[c] for c in orbit) for i,j in pairs}
    R={}
    for i,j in pairs:
        ric=sum(G[c,i,j].d(c,omega)-G[c,i,c].d(j,omega) for c in orbit)
        ric+=sum(G[c,c,k]*G[k,i,j]-G[c,j,k]*G[k,i,c] for c,k in pairs)
        R[i,j]=ric-iW*Hess[i,j]+iW*iW*dW[i]*dW[j]*arb('0.5')
    R[3,3]=one-sum(inv[i,j]*Hess[i,j] for i,j in pairs)*arb('0.5')
    return R

def inputs():
    spectral=json.loads(pin('verification/goal005_gl_results.json'))
    center=spectral['results'][-1]
    p2=arb(center['pH'])
    # Re-evaluate the normalization using the *same* p2 ball in the linear map.
    D=1+8*a
    yoveru=D/2
    ystprimeoveru=4*a-arb('0.25')+D*p2
    norm=(-2*yoveru/D-ystprimeoveru/(a*D))/8
    amp=1/norm
    y02=yoveru*amp*(-2*omega).exp()
    hor=json.loads(pin('GLR-GOAL-005/compute/gl_eigenvalue_results.json'))['horizon_connection']
    y01=arb(hor['a_h_lower']).union(arb(hor['a_h_upper']))*(-omega).exp()
    return [(arb(1),y01,None),(arb(2),y02, (4*a-arb('0.25'))/yoveru+(p2-omega)*2)]

def exterior_inputs():
    """Backward Riccati shooting and validated integration of d log|u|/dt=r*p."""
    from types import ModuleType
    flow = ModuleType("pinned_goal005_validate_gl")
    flow.__file__ = "verification/goal005_validate_gl.py"
    exec(compile(pin(flow.__file__),flow.__file__,'exec'),flow.__dict__)
    _,p,_=flow.endpoints(omega)
    t=arb(39).log()
    integrals={}
    p_values={}
    for target in (16,8,4,2):
        integral=arb(0)
        stop=arb(target-1).log()
        steps=math.ceil(float(t-stop)*64)
        h=(stop-t)/steps
        for i in range(steps):
            left=t+i*h
            next_p,tube,_=flow.step(left,p,h,omega)
            # Taylor integration of r*p with the exact-input p ball.
            n=5
            def integrand(x, px):
                p_ser=arb_series(flow.coefficients(x,px,omega,n),n+1)
                return (1+arb_series([x,1],n+1).exp())*p_ser
            center=integrand(left,p)
            rest=integrand(left.union(left+h),tube)[n]
            for j in range(n):
                integral += center[j]*h**(j+1)/(j+1)
            integral += flow.sym(rest*abs(h)**(n+1)/(n+1))
            p=next_p
        integrals[target]=integral
        p_values[target]=p
        t=stop
    r2,y2,_=inputs()[1]
    u2=y2*(2*omega).exp()*r2/(1+a*r2**3)
    for r0 in (4,8,16):
        r=arb(r0)
        f=1-1/r
        D=1+a*r**3
        h=D/r
        lyp=(2*a*r-1/r**2)/h+(p_values[r0]-omega)/f
        u=u2*(-sum(integrals[rs] for rs in (2,4,8,16) if rs<r0)).exp()
        y=h*u*(-omega*(r+(r-1).log())).exp()
        yield r,y,lyp

def parts(z):
    def bounds(x):
        return ([0,0] if x.is_zero() else
                [math.nextafter(float(x.lower()), -math.inf), math.nextafter(float(x.upper()), math.inf)])
    return [bounds(z.real),bounds(z.imag)]
def row(r0,y0,log_y_prime,omega):
    h=mode(r0,arb(1),log_y_prime,omega)
    scale=y0*y0
    Q,E,div,R,qzz=calc(r0,h,omega)
    W=warped(r0,h,omega)
    Qa,Ea,diva,Ra,_=calc(r0,h,omega,angle=True)
    Qwrong,Ewrong,Dwrong,_,_=calc(r0,h,omega,half=arb(1))
    Qomit,Eomit,Domit,_,_=calc(r0,h,omega,bz_mutation=True)
    labels={'vv':(0,0),'vr':(0,1),'vz':(0,2),'rr':(1,1),'rz':(1,2),'zz':(2,2),'thth':(3,3)}
    assert all(R[a,b].val(n).contains(0) for a,b in R for n in (0,1))
    assert all(d.val(2).contains(0) for d in div),[str(d.val(2)) for d in div]
    assert all((Q[pair].val(2)-Qa[pair].val(2)).contains(0) for pair in labels.values())
    assert all((Q[pair].val(2)-W[pair].val(2)).contains(0) for pair in labels.values())
    assert (Q[4,4].val(2)-Q[3,3].val(2)).contains(0)
    assert (Qa[4,4].val(2)-Qa[3,3].val(2)*(arb(3)/4)).contains(0)
    assert (Q[2,2].val(2)-qzz.val(2)).contains(0)
    assert not (Qwrong[2,2].val(2)-qzz.val(2)).contains(0)
    assert any(not z.val(2).contains(0) for z in Domit)
    return dict(r=str(r0),y0=str(y0),log_y_prime=str(log_y_prime),
        mode=[parts(acb(y0*hh[0])) for hh in h],Q={name:parts(Q[pair].val(2)*scale) for name,pair in labels.items()},
        Einstein={name:parts(E[pair].val(2)*scale) for name,pair in labels.items()},
        warped_source={name:parts(W[pair].val(2)*scale) for name,pair in labels.items()},
        Bianchi=[parts(z.val(2)*scale) for z in div],Bianchi_omit_Qzz=[parts(z.val(2)*scale) for z in Domit],
        analytic_Qzz=parts(qzz.val(2)*scale),Qzz_missing_cosine_half=parts(Qwrong[2,2].val(2)*scale),
        Q_tau0_factor=str((2*omega*r0).exp()))

if __name__=='__main__':
    for r0,y0,yp0 in [*inputs(),*exterior_inputs()]:
        print(json.dumps(row(r0,y0,yp0,omega)))
