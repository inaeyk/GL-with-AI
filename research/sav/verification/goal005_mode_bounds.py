"""Coarse outward GL metric derivative bounds, using certified GL1 flow.
Not an error estimate for the historical interpolated profile. No ODE tolerance.
"""
from flint import arb,arb_series,ctx
from pathlib import Path
import math,json,hashlib,platform
import sympy as sp
import goal005_validate_gl as G
ctx.prec=160; ctx.cap=12
HERE=Path(__file__).resolve().parent
O=arb('0.06328119','0.00000001'); A=G.K2
N=8

def box(x): return arb(0,x.abs_upper())
def maximum(a,b): return a.union(b).abs_upper()
def series_bounds(vals,n=N):
    return arb_series([box(vals[j])/math.factorial(j) for j in range(len(vals))],n+1)
def coeff(r):
    f=(r-1)/r;D=1+A*r**3
    b=2*O+1/r-5*f/r+6*f/(r*D)
    c=O*(-4+6/D)/r+3/r**3-A-3/(r**3*D)
    return f,b,c
def hom_ef(r,Y):
    f=(r-1)/r;D=1+A*r**3
    Z=-O/(A*D)*(f*Y.derivative()+(O-1/(2*r*r))*Y)
    B=-Z.derivative()/O-2*Z/(O*r)
    T=Z-f*B;S=r*Z/O-Y/(2*A*r)
    NN=2*Z.derivative()/O+2*Z/(O*r)+Y/(A*r**3)
    return T,B,NN,S
def static_metric(r,u,p):
    f=(r-1)/r;D=1+A*r**3
    y=D/r*u;yp=(2*A*r-1/r**2+D/r*p/f)*u
    T=((r-1)*(arb('1/2')-A*r**3)/(r*D**2)-O**2/(A*D))*y+((r-1)/(2*A*r**3*D)-3*(r-1)**2/D**2)*yp
    B=-O/(A*D)*(yp-y/(2*r*(r-1)))
    S=-r*r*y/(2*D)-(r-1)*yp/(A*D)
    C=T/f**2-2*S/(r*(r-1))
    return T,B,C,S
def linear_jet(r0,u0,p0):
    r=arb_series([r0,1],N+1);f=(r-1)/r
    p=arb_series([p0],N+1);u=arb_series([u0],N+1)
    v=G.potential(r)
    for j in range(N): p[j+1]=((v+O**2-p*p)/f)[j]/(j+1)
    for j in range(N): u[j+1]=(p/f*u)[j]/(j+1)
    return r,u,p
def cell_integrals(cells):
    vals=[];total=arb(0)
    for t,h,p in cells:
        ti=t.union(t+h);g=(1+ti.exp())*p
        vals.append((t,h,p,g,total));total+=h*g
    return vals,total
def serialize(z): return dict(ball=str(z),lower=str(z.lower()),upper=str(z.upper()))

def main():
    ph,pi,ep=G.endpoints(O);left=[];right=[]
    pm,_=G.propagate(arb(-28),ph,O,'horizon',left)
    G.propagate(arb(39).log(),pi,O,'infinity',right)
    ll,IL=cell_integrals(left);rr,IR=cell_integrals(right)
    r2,u2,p2=linear_jet(arb(2),arb(1),pm)
    norm=static_metric(r2,u2,p2)[3][0]/8
    assert not norm.contains(0)
    scale=1/norm
    dh=arb(-28).exp();xh=1+dh-28
    etaH=(A+47)*(dh+dh*dh/2)/(2*O)
    # vH in [(1-2eta)/(1-eta),1/(1-eta)], normalized to1 at horizon.
    vh=arb(1)+box(etaH/(1-etaH))
    C=scale*(-IL-O*xh).exp()/vh
    dc=arb(1)/4096
    eta=(A+47)*(dc+dc*dc/2)/(2*O)
    assert eta<arb('1/2')
    rc=arb(1).union(1+dc);rs=arb_series([rc,1],N+1)
    f,b,c=coeff(rs)
    assert b[0]>1 and f.derivative()[0]>0
    B=[(C*(1+A*rc**3)/rc/(1-eta)).abs_upper()]
    for n in range(N):
        z=arb(0)
        for j in range(2,n+1): z+=math.comb(n,j)*(f[j]*math.factorial(j)).abs_upper()*B[n+2-j]
        for j in range(1,n+1): z+=math.comb(n,j)*(b[j]*math.factorial(j)).abs_upper()*B[n+1-j]
        for j in range(n+1): z+=math.comb(n,j)*(c[j]*math.factorial(j)).abs_upper()*B[n-j]
        B.append((z/b[0].lower()).abs_upper())
    Y=series_bounds(B)
    T,V,NN,S=hom_ef(rs,Y)
    # Exact N(1)=0, so derivatives of N/d are bounded by N^(j+1)/(j+1).
    dd=arb_series([box(NN[j+1]) for j in range(7)],8)
    Crr=rs*dd
    tau=(O*rs).exp()
    collar=[[((z*tau)[j]*math.factorial(j)).abs_upper() for j in range(6)] for z in [T,V,Crr,S]]
    middle=[[arb(0) for j in range(6)] for _ in range(4)]
    outside=[[arb(0) for j in range(6)] for _ in range(4)]
    # Whole-cell bounds: no sampled supremum is used.
    for records,total in [(ll,IL),(rr,IR)]:
        for t,h,p,g,prefix in records:
            ti=t.union(t+h);r0=1+ti.exp()
            if r0.upper()<1+dc: continue
            logu=prefix-total+arb(0).union(h)*g
            r,u,ps=linear_jet(r0,scale*logu.exp(),p)
            st=static_metric(r,u,ps)
            # tau slice coefficients at tau=0: static tensor*(r-1)^(-Omega).
            if r0.lower()<4:
                fac=(-O*(r-1).log()).exp()
                ff=(r-1)/r
                ef=[st[0],st[1]-st[0]/ff,st[2]-2*st[1]/ff+st[0]/ff**2,st[3]]
                for i,z in enumerate(ef):
                    for j in range(6):middle[i][j]=maximum(middle[i][j],(z*fac)[j]*math.factorial(j))
            if r0.upper()>4:
                weight=((r0-4)/2).exp()
                for i,z in enumerate(st):
                    for j in range(6):outside[i][j]=maximum(outside[i][j],z[j]*math.factorial(j)*weight)
    # Exterior r>=40: compactify coefficients and factor r*u from every H.
    z,aa,oo,pp=sp.symbols('z aa oo pp')
    r=1/z;f=(r-1)/r;D=1+aa*r**3
    y=D/r;yp=2*aa*r-1/r**2+D/r*pp/f
    TT=((r-1)*(sp.Rational(1,2)-aa*r**3)/(r*D**2)-oo**2/(aa*D))*y+((r-1)/(2*aa*r**3*D)-3*(r-1)**2/D**2)*yp
    BB=-oo/(aa*D)*(yp-y/(2*r*(r-1)))
    SS=-r*r*y/(2*D)-(r-1)*yp/(aa*D)
    CC=TT/f**2-2*SS/(r*(r-1))
    expr=[sp.cancel(v*z) for v in [TT,BB,CC,SS]]
    def ev(e,env):
        if e in env:return env[e]
        if e.is_Integer:return int(e)
        if e.is_Rational:return arb(int(e.p))/int(e.q)
        if e.is_Add:return sum(ev(x,env) for x in e.args)
        if e.is_Mul:
            v=1
            for x in e.args:v*=ev(x,env)
            return v
        if e.is_Pow and e.exp.is_Integer:return ev(e.base,env)**int(e.exp)
        raise TypeError(e)
    zz=arb(0).union(arb(1)/40);ts=arb_series([zz],N+1)/(1+arb_series([0,zz],N+1))
    qq=(A+O*O).sqrt();beta=qq-A/(2*qq)
    cr=abs(6-qq*qq+4*qq*beta-beta*beta-beta+qq)+abs(-17-2*qq*beta+2*beta*beta+3*beta-qq)/40+abs(11-beta*beta-2*beta)/1600+18/(A*40**3)**2+6*(4+arb(5)/40)/(A*40**3)
    etaI=cr/(2*qq*(arb(39)/40)**2*40)
    error=cr/(2*qq*(arb(39)/40)*1600*(1-2*etaI))
    p0=-(1-zz)*(qq+beta*zz)+box(error)
    p=arb_series([p0],N+1);f=1-ts;d=ts**3+A
    vv=A*f+(6-11*ts)*ts**2*f+18*ts**8*f**2/d**2-6*(4-5*ts)*ts**5*f/d
    us=arb_series([1],N+1)
    for j in range(N):p[j+1]=((vv+O*O-p*p)/f)[j]/(j+1)
    for j in range(N):us[j+1]=(p/f*us)[j]/(j+1)
    env={z:ts,aa:A,oo:O,pp:p}
    U40=(scale*(-IR).exp()).abs_upper()
    radial=(U40/(1-2*etaI)*(arb(40)+1/(qq-arb('1/2')))*arb(18).exp()).abs_upper()
    tail=[]
    for e in expr:
        v=ev(e,env)*(1+arb_series([0,zz],N+1))*us
        tail.append([(v[j]*math.factorial(j)).abs_upper()*radial for j in range(6)])
    final=[[maximum(maximum(collar[i][j],middle[i][j]),maximum(outside[i][j],tail[i][j])) for j in range(6)] for i in range(4)]
    assert all(x<arb('4e29') for row in final for x in row)
    report=dict(python=platform.python_version(),arb_bits=ctx.prec,omega=str(O),
        definition='max component/radial derivative j<=5; EF tau=v-r slice r<=4; exp((r-4)/2) static profiles r>=4; raw sphere coefficient Htheta included',
        normalized_horizon_u_coefficient=serialize(C),horizon_master_derivative_bounds=[str(x) for x in B],
        log_integrals=[serialize(IL),serialize(IR)],normalization=serialize(norm),
        collar=[[str(x) for x in row] for row in collar],middle=[[str(x) for x in row] for row in middle],
        exterior_finite=[[str(x) for x in row] for row in outside],exterior_infinite=[[str(x) for x in row] for row in tail],
        global_bounds=[[str(x) for x in row] for row in final],
        maximum_bound='400000000000000000000000000000',
        source_sha256={f:hashlib.sha256((HERE/f).read_bytes()).hexdigest() for f in ['goal005_mode_bounds.py','goal005_validate_gl.py']})
    (HERE/'goal005_mode_bounds.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))

if __name__=='__main__':
    try:main()
    except Exception as exc:
        print(type(exc).__name__+': '+str(exc),flush=True)
        raise SystemExit(1)
