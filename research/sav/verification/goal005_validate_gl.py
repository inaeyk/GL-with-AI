"""GOAL005 non-production L10 validated scalar Riccati propagation.

All proof decisions use Arb balls, never float tolerances. See companion
physics note for endpoint and signed-flow/Taylor remainder proof.
No prior scientific outputs are overwritten.
"""
from flint import arb, arb_series, ctx
from pathlib import Path
import json, platform, hashlib, time

ctx.prec = 160
ctx.cap = 14
ORDER = 12
HERE = Path(__file__).resolve().parent
K2 = (arb.pi()/5)**2

def potential(r):
    d = 1+K2*r**3
    return K2*(r-1)/r+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*d**2)-6*(4*r-5)*(r-1)/(r**4*d)

def rhs(t,p,om):
    r=1+t.exp()
    return r*(potential(r)+om**2-p**2)

def sym(x):
    return arb(0,x.abs_upper())

def hull(a,b):
    return a.union(b)

def coefficients(t,p,om,n=ORDER):
    ts=arb_series([t,1],n+1)
    r=1+ts.exp()
    a=r*(potential(r)+om**2)
    ps=arb_series([p],n+1)
    for j in range(n):
        ps[j+1]=(a-r*ps*ps)[j]/(j+1)
    return [ps[j] for j in range(n+1)]

def step(t, p, h, om):
    # Tube encloses the interval-data solution and the center-data solution.
    ti=hull(t,t+h)
    span=hull(arb(0),h)
    tube=p+sym(2*abs(h)*rhs(ti,p,om).abs_upper()+arb('1e-30'))
    for _ in range(12):
        image=p+span*rhs(ti,tube,om)
        if tube.contains_interior(image):
            break
        tube=hull(tube,image)+sym((tube-p).abs_upper()/2+arb('1e-30'))
    else:
        raise ArithmeticError('Picard tube failed')
    center=p.mid()
    cs=coefficients(t,center,om)
    # Coefficient ORDER over the whole tube encloses y^(ORDER)/ORDER!.
    rem=coefficients(ti,tube,om)[ORDER]
    val=arb(0)
    for c in reversed(cs[:ORDER]):
        val=val*h+c
    val += sym(rem*abs(h)**ORDER)
    # Mean-value variational flow for fixed same omega/k. h may be negative.
    exponent=(-2*(1+ti.exp())*tube*h).upper()
    propagated=p.rad()*exponent.exp()
    out=val+sym(propagated)
    return out,tube,rem*abs(h)**ORDER

def endpoints(om):
    dh=arb(-28).exp(); cv=K2+47
    eta=cv*(dh+dh**2/2)/(2*om)
    assert eta < arb('1/2')
    # Volterra |v_x| <= cv*dh*(1+dh)/(1+2om)/(1-eta), v>= (1-2eta)/(1-eta).
    eh=cv*dh*(1+dh)/((1+2*om)*(1-2*eta))
    ph=om+sym(eh)
    R=arb(40); q=(K2+om**2).sqrt(); beta=q-K2/(2*q)
    c0=6-q*q+4*q*beta-beta*beta-beta+q
    c1=-17-2*q*beta+2*beta*beta+3*beta-q
    c2=11-beta*beta-2*beta
    cr=abs(c0)+abs(c1)/R+abs(c2)/R**2+18/(K2*R**3)**2+6*(4+5/R)/(K2*R**3)
    f=1-1/R
    ei=cr/(2*q*f*f*R)
    assert ei < arb('1/2')
    ep=cr/(2*q*f*R**2*(1-2*ei))
    pi=-f*(q+beta/R)+sym(ep)
    return ph,pi,dict(eta_h=str(eta),p_h=str(ph),eta_inf=str(ei),p_inf=str(pi),horizon_p_error=str(eh),infinity_p_error=str(ep))

def propagate(t0,p,om,label,cells=None):
    # Equal-sized exact rational fraction of enclosed t0: interval t endpoints
    # cover the exact log(39), and h=-t0/N covers that same exact value.
    n=int(abs(float(t0))*32)+1
    h=-t0/n
    maxrad=arb(0); maxrem=arb(0); tubes=[]
    subdivisions=0
    def advance(t,p,h,depth=0):
        nonlocal subdivisions
        try:
            result=step(t,p,h,om)
            if cells is not None:
                cells.append((t,h,result[1]))
            return result
        except ArithmeticError:
            if depth>=10: raise
            subdivisions+=1
            pm,ta,ra=advance(t,p,h/2,depth+1)
            po,tb,rb=advance(t+h/2,pm,h/2,depth+1)
            return po,ta.union(tb),ra.union(rb)
    for i in range(n):
        t=t0*(n-i)/n
        p,tube,rem=advance(t,p,h)
        if not p.is_finite(): raise ArithmeticError('nonfinite enclosure')
        maxrad=hull(maxrad,p.rad()).abs_upper()
        maxrem=hull(maxrem,rem.abs_upper()).abs_upper()
        if i%64==0 or i==n-1:
            tubes.append(dict(t=str(t+h),p=str(p),tube=str(tube)))
    return p,dict(label=label,steps=n,subdivisions=subdivisions,max_radius=str(maxrad),max_local_remainder=str(maxrem),samples=tubes)

def run(omstr):
    om=arb(omstr)
    ph,pi,ep=endpoints(om)
    left,lh=propagate(arb(-28),ph,om,'horizon')
    right,li=propagate(arb(39).log(),pi,om,'infinity')
    F=left-right
    # Unit-u(2) sphere-radius functional, exact reconstruction.
    r=arb(2); d=1+K2*r**3; f=1-1/r
    y=d/r; yp=2*K2*r-1/r**2+y*left/f
    norm=(-r*r*y/(2*d)-(r-1)*yp/(K2*d))/(2*r*r)
    return dict(omega=omstr,F=str(F),F_sign=-1 if F<0 else 1 if F>0 else 0,
                F_lower=str(F.lower()),F_upper=str(F.upper()),F_width=str(2*F.rad()),
                pH=str(left),pI=str(right),normalization_u2_one=str(norm),
                normalization_lower=str(norm.lower()),normalization_upper=str(norm.upper()),
                normalization_nonzero=not norm.contains(0),endpoints=ep,left=lh,right=li)

def main():
    started=time.time()
    report=dict(scope='validated Riccati endpoint and finite IVP; physical metric derivatives not yet certified',
                python=platform.python_version(),precision_bits=ctx.prec,order=ORDER,
                config=dict(tH=-28,R=40,match_r=2,max_step='1/32'),
                script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),results=[])
    for om in ['0.06327','0.06330','0.063285 +/- 0.000015',
               '0.06328118','0.06328120','0.06328119 +/- 0.00000001']:
        result=run(om); report['results'].append(result)
        print(json.dumps({k:result[k] for k in ['omega','F','F_sign','pH','pI','normalization_u2_one']}),flush=True)
    report['elapsed_seconds']=time.time()-started
    report['bracket_signs']=report['results'][0]['F_sign']==-1 and report['results'][1]['F_sign']==1
    report['tight_bracket_signs']=report['results'][3]['F_sign']==-1 and report['results'][4]['F_sign']==1
    (HERE/'goal005_gl_results.json').write_text(json.dumps(report,indent=2)+'\n')
    assert report['bracket_signs'], 'No certified sign bracket'

if __name__=='__main__':
    try: main()
    except Exception as exc:
        print(type(exc).__name__+': '+str(exc),flush=True)
        raise SystemExit(1)
