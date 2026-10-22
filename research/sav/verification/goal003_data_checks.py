"""GOAL-003 exact non-production checks: constraints, hierarchy and AH bridge.

No physical ODE or spacetime evolution. This is a primary derivation check,
not either independent Goal audit. Run with Python 3 + SymPy.
"""
import json
import platform
from pathlib import Path
import sympy as s

out = {"python": platform.python_version(), "sympy": s.__version__, "checks": []}

def check(name, expression, zero=True):
    value = s.simplify(s.expand(expression))
    ok = (value == 0) if zero else (value != 0)
    assert ok, (name, value)
    out["checks"].append({"name": name, "status": "PASS" if zero else "REJECTED_MUTATION"})
    print(name + ": " + out["checks"][-1]["status"], flush=True)

r, z, th, ph = s.symbols("r z theta phi", positive=True)
x = [r, z, th, ph]
a, b, R, kap, q, v, eta = [s.Function(name)(r,z) for name in ["a","b","R","kap","q","v","eta"]]
w=(q-v)/2
g=s.diag(a*a,b*b,R*R,R*R*s.sin(th)**2)
gi=s.diag(a**-2,b**-2,R**-2,1/(R*R*s.sin(th)**2))
Gamma=[[[s.simplify(sum(gi[i,l]*(s.diff(g[l,k],x[j])+s.diff(g[l,j],x[k])-s.diff(g[j,k],x[l])) for l in range(4))/2) for k in range(4)] for j in range(4)] for i in range(4)]
Ric=s.zeros(4)
for i in range(4):
    for j in range(i,4):
        Ric[i,j]=s.simplify(sum(s.diff(Gamma[k][i][j],x[k])-s.diff(Gamma[k][i][k],x[j])+sum(Gamma[k][k][l]*Gamma[l][i][j]-Gamma[k][j][l]*Gamma[l][i][k] for l in range(4)) for k in range(4)))
        Ric[j,i]=Ric[i,j]
Scal=s.simplify(sum(gi[i,j]*Ric[i,j] for i in range(4) for j in range(4)))
Hs=s.diff(b,r)/b+2*s.diff(R,r)/R
A2=(s.diff(b,r)/b)**2+2*(s.diff(R,r)/R)**2
Rh=2/R**2-4*(s.diff(R,z,2)-s.diff(b,z)*s.diff(R,z)/b)/(b*b*R)-2*s.diff(R,z)**2/(b*b*R*R)
Lap=s.diff(R*R*s.diff(a,z)/b,z)/(b*R*R)
Scal_rad=Rh+2*Hs*s.diff(a,r)/a**3-(2*s.diff(Hs,r)+Hs**2+A2)/a**2-2*Lap/a
check("DIRECT_4D_SCALAR_CURVATURE_RADIAL_IDENTITY",Scal-Scal_rad)
K=s.diag(a*a*kap,b*b*v,R*R*w,R*R*s.sin(th)**2*w)
K[0,1]=K[1,0]=a*eta
Km=gi*K
trace=kap+q
mom=[]
for j in [0,1]:
    mom.append(s.simplify(sum(s.diff(Km[i,j],x[i])+sum(Gamma[i][i][l]*Km[l,j]-Gamma[l][i][j]*Km[i,l] for l in range(4)) for i in range(4))-s.diff(trace,x[j])))
diveta=s.diff(R*R*eta/b,z)/(b*R*R)
sigmaA=(v-q/3)*s.diff(b,r)/b+2*(w-q/3)*s.diff(R,r)/R
rhsq=a*diveta+2*eta*s.diff(a,z)/b**2+Hs*(kap-q/3)-sigmaA
rhseta=a*(s.diff(kap,z)+s.diff(q,z)-s.diff(v,z)-2*s.diff(R,z)*(v-w)/R)-Hs*eta+(kap-v)*s.diff(a,z)
check("DIRECT_RADIAL_MOMENTUM",mom[0]+s.diff(q,r)-rhsq)
check("DIRECT_TANGENTIAL_MOMENTUM",mom[1]-(s.diff(eta,r)-rhseta)/a)
sigma2=v*v+2*w*w-q*q/3
Z=2*kap*q+2*q*q/3-2*eta*eta/b**2-sigma2
Ham=s.simplify(Scal+trace*trace-s.trace(Km*Km))
rhsa=(2*a*a*Lap-a**3*(Rh+Z)+a*(2*s.diff(Hs,r)+Hs*Hs+A2))/(2*Hs)
check("DIRECT_HAMILTONIAN_PARABOLIC_IDENTITY",Ham-2*Hs*(s.diff(a,r)-rhsa)/a**3)
check("OMITTED_RADIAL_LAPSE_GRADIENT_MUTATION",mom[0]+s.diff(q,r)-(rhsq-2*eta*s.diff(a,z)/b**2),False)
check("WRONG_SPATIAL_DIMENSION_Q2_FACTOR_MUTATION",Ham-(Scal+2*kap*q+q*q/2-2*eta*eta/b**2-sigma2),False)

# Exact tau=v-r Schwarzschild-string slice, including its nonzero K.
r0=s.symbols("r0",positive=True)
a0=s.sqrt(1+r0/r)
w0=r0/(r*s.sqrt(r*(r+r0)))
kap0=w0+r*s.diff(w0,r)
R0scalar=2*(1-a0**-2)/r**2+4*s.diff(a0,r)/(a0**3*r)
check("BACKGROUND_HORIZON_CROSSING_HAMILTONIAN",R0scalar+4*kap0*w0+2*w0*w0)
check("BACKGROUND_HORIZON_CROSSING_MOMENTUM",s.diff(2*w0,r)-2*(kap0-w0)/r)

# Exact obstruction of the first all-orders prescription outside the source.
C=s.symbols("C",nonzero=True)
bad=C*r**s.Rational(-2,3)
check("NAIVE_FIXED_KAPPA_SIGMA_BAD_TAIL",s.diff(bad,r)+2*bad/(3*r))
check("NAIVE_BAD_TAIL_IS_NOT_R_MINUS_2",s.diff(r*r*bad,r),False)

# Modified free rule, on b=1,R=r exterior: q and eta weighted by r^2.
Q,P,A,Az,Qz,Pz=s.symbols("Q P A Az Qz Pz",real=True)
Qr=A*Pz+2*P*Az
Pr=A*Qz/2-Q*Az/2
# Integration by parts of A(Q P_z+P Q_z) contributes -A_z QP.
energy_local=s.expand(Q*Qr+2*P*Pr)
check("MODIFIED_RULE_EXACT_WEIGHTED_ENERGY_CANCELLATION",energy_local-A*(Q*Pz+P*Qz)-Az*Q*P)
mat=s.Matrix([[0,A],[A/2,0]])
sym=s.diag(s.Rational(1,2),1)
check("MODIFIED_RULE_SYMMETRIC_PRINCIPAL_SYMBOL",(sym*mat-(sym*mat).T)[0,1])

# Ordered inverse-matrix coefficients, noncommuting small exact matrices.
e=s.symbols("epsilon")
U1=s.Matrix([[1,2],[0,-1]])
U2=s.Matrix([[0,1],[3,2]])
U3=s.Matrix([[2,-1],[1,0]])
inv=[s.eye(2),-U1,U1*U1-U2,-U3+U1*U2+U2*U1-U1**3]
prod=(s.eye(2)+e*U1+e**2*U2+e**3*U3)*sum((e**j*inv[j] for j in range(4)),s.zeros(2))
for j in range(1,4):
    check("ORDERED_INVERSE_RECURSION_ORDER_"+str(j),sum(s.expand(prod[k,l]).coeff(e,j)**2 for k in range(2) for l in range(2)))
wrong=-U3+2*U1*U2-U1**3
check("COMMUTED_CUBIC_INVERSE_MUTATION",s.trace((wrong-inv[3]).T*(wrong-inv[3])),False)

# Fourier convolution and parity, including counterexample to a harmonic cutoff.
base={-1,1}
def addset(A,B): return {a+b for a in A for b in B}
two=addset(base,base); three=addset(base,two); four=addset(two,two)|addset(base,three)
assert two=={-2,0,2} and three=={-3,-1,1,3} and four=={-4,-2,0,2,4}
out["checks"].append({"name":"HARMONIC_CONVOLUTION_2_3_4", "status":"PASS"})
zz=s.symbols("zz",real=True)
check("HALF_PERIOD_ALLOWS_FREE_ORDER_TWO_N6",(-e)**2*s.cos(6*zz)-e**2*s.cos(6*(zz+s.pi)))

# All-component horizon MOTS map from freely varied ADM metric data.
Ricnn=2*s.diff(a0,r)/(a0**3*r)
Aleaf2=2/(a0*a0*r*r)
graph_potential=s.simplify(-a0*(Ricnn+Aleaf2)-s.diff(2*w0,r))
check("AH_GRAPH_STABILITY_POTENTIAL",graph_potential.subs(r,r0)-s.sqrt(2)/r0**2)
dtt,dtr,drr,ds,dsr,Om=s.symbols("dtt dtr drr ds dsr Omega",real=True)
N0=1/s.sqrt(2); beta0=s.Rational(1,2); q0=s.sqrt(2) # r0=1
dbeta=(dtr-beta0*drr)/2
dNrel=-dtt+dtr-drr/4
dq=2*dbeta/N0+beta0*dsr/N0-Om*ds/N0-q0*dNrel
dH=dsr/s.sqrt(2)-drr/(2*s.sqrt(2))
check("AH_DATA_EXPANSION_CANCELLATIONS",dH-dq-s.sqrt(2)*(Om*ds-dtt))
k2,ah=s.symbols("k2 a_h",positive=True)
T=ah*Om*(1-2*Om)/(2*k2*(k2+1))
S=-ah*(k2+2*Om)/(2*k2*(k2+1))
rho=(T-Om*S)/(k2+1)
check("AH_GL_AREAL_RADIUS_BRIDGE",S/2+rho+ah/(4*(k2+1)))
check("AH_COORDINATE_GRAPH_EQUALS_AREAL_RADIUS_MUTATION",rho+ah/(4*(k2+1)),False)

# Secular growth integral and quartic scalar/log coefficient.
t,Tm,Omega,sigma=s.symbols("t T Omega sigma",positive=True)
p=s.symbols("p",positive=True)
I=(s.exp(p*Omega*Tm)-s.exp(sigma*Tm))/(p*Omega-sigma)
check("UNSTABLE_TIME_DUHAMEL_GROWTH",s.diff(I,Tm)-sigma*I-s.exp(p*Omega*Tm))
chi2,chi4=s.symbols("chi2 chi4")
log4=s.series(s.log(1+e**2*chi2+e**4*chi4),e,0,5).removeO().coeff(e,4)
check("QUARTIC_RADION_LOG_COMPLETION",log4-chi4+chi2**2/2)
check("OMITTED_QUARTIC_LOG_MUTATION",log4-chi4,False)

out["scope"]="Exact/manufactured identities only; no global constraint family, finite amplitude theorem or physical evolution certified."
out["count"]=len(out["checks"])
path=Path(__file__).with_name("goal003_data_checks_results.json")
path.write_text(json.dumps(out,indent=2)+"\n")
print(json.dumps({"checks":out["count"],"python":out["python"],"sympy":out["sympy"]}))
