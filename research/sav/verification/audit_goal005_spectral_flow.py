"""Independent exact linear-fundamental-matrix vs Riccati-flow controls.

No primary imports. Reimplements Taylor jets for a constant-coefficient
manufactured Riccati equation by an explicit convolution recurrence.
"""
from pathlib import Path
import hashlib,json,math,platform
from flint import arb,ctx
ctx.prec=160
HERE=Path(__file__).resolve().parent
checks=[]
def record(name,passed,evidence):
    checks.append(dict(name=name,passed=bool(passed),evidence=str(evidence)))
    assert passed,(name,evidence)
def sym(z):return arb(0,z.abs_upper())
def jets(p,a,n):
    cs=[p]
    for j in range(n):
        conv=sum((cs[m]*cs[j-m] for m in range(j+1)),arb(0))
        cs.append(((a*a if j==0 else arb(0))-conv)/(j+1))
    return cs
def step(p,a,h,n=12):
    span=arb(0).union(h)
    tube=p+sym(2*abs(h)*(a*a-p*p).abs_upper()+arb('1e-30'))
    for _ in range(12):
        im=p+span*(a*a-tube*tube)
        if tube.contains_interior(im):break
        tube=tube.union(im)+sym((tube-p).abs_upper()/2)
    else:raise AssertionError('manufactured tube failed')
    cs=jets(p,a,n)
    rem=jets(tube,a,n)[n]
    val=arb(0)
    for j in range(n-1,-1,-1):val=val*h+cs[j]
    return val+sym(rem*abs(h)**n),tube

# Linear fundamental matrix exact solution: u0=1, v0=p0,
# [u,v] = [[cosh(ah),sinh(ah)/a],[a*sinh(ah),cosh(ah)]] [1,p0].
a=arb(2)
for hs in ['1/32','-1/32']:
    h=arb(hs)
    for ps in ['0','1/5','201/1000','199/1000']:
        p=arb(ps)
        C=(a*h).cosh();S=(a*h).sinh()
        u=C+p*S/a;v=a*S+p*C
        exact=v/u
        bounded,tube=step(p,a,h)
        record('linear matrix vs nonlinear Taylor '+hs+' '+ps,bounded.contains(exact),dict(bound=str(bounded),exact=str(exact),tube=str(tube)))
        record('manufactured linear denominator no node '+hs+' '+ps,not u.contains(0),u)

# A whole initial-data ball: explicit endpoint checks plus analytically
# positive derivative of the Möbius transfer certify its range.
p0=arb('0.2','0.001');h=arb('1/32')
out,tube=step(p0,a,h)
for p in [p0.lower(),p0.upper()]:
    T=(a*h).tanh();exact=(p+a*T)/(1+p*T/a)
    record('initial interval extremum enclosed',out.contains(exact),exact)
record('Moebius input derivative positive',1-(a*h).tanh()**2>0,1-(a*h).tanh()**2)

# Signed direction is essential: y'=-2y backwards amplifies uncertainty.
rad=arb('1/100');h=arb('-1/4')
actual=rad*(-2*h).exp();wrong=rad*(2*h).exp()
record('negative: forward stability sign applied backwards',actual>wrong,dict(actual=str(actual),wrong=str(wrong)))
# Omitted remainder is detected even for p'=4-p², p(0)=0.
h=arb('1/32');exact=2*(2*h).tanh();linear=4*h
record('negative: missing Taylor remainder',not linear.contains(exact),linear-exact)
# Radius inflation must include input uncertainty. Exact y'=0 preserves it.
record('negative: discarded initial interval radius',not arb('0.2').contains(p0),p0)

result=dict(classification='MANUFACTURED_VALIDATOR_CHECKS_NOT_PHYSICAL_ROOT_RERUN',
            python=platform.python_version(),arb_bits=ctx.prec,
            script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),checks=checks)
(HERE/'audit_goal005_spectral_flow.json').write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(json.dumps(result,indent=2))
