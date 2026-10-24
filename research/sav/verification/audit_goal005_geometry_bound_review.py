"""Bounded independent coefficient-count review; no ODEs or interval reruns.
Actual H5 enclosure is an explicitly shared/audited prerequisite.
"""
from fractions import Fraction as F
from pathlib import Path
import json,hashlib
import math
import sympy as s
HERE=Path(__file__).resolve().parent
e,w,wa,wb,wab,wd,La,Lb,Lab,Ld,G0,G1,G2=s.symbols('e w wa wb wab wd La Lb Lab Ld G0 G1 G2')
la=La+e*wa-e**2*w*wa;lb=Lb+e*wb-e**2*w*wb
lab=Lab+e*wab-e**2*(wa*wb+w*wab);ld=Ld+e*wd-e**2*w*wd
full=-lab+(G0+e*G1+e**2*G2)*ld-la*lb/2
expected=w*wab+wa*wb/2-G0*w*wd+G1*wd+G2*Ld+w*(La*wb+Lb*wa)/2
assert s.expand(full).coeff(e,2)==s.expand(expected)
K=1000;B=2*10**31;W=10**5
II1=10**9;II2=10**14;GG0=10**8;GG1=10**14;GG2=10**19
rows=[]
def record(name,value,cap):
    assert value<cap,name
    rows.append({'name':name,'derived_bound':str(value),'strict_cap':str(cap)})
record('inverse1 three factors, two contractions',9*3**4*K**2,II1)
record('inverse2 five factors, four contractions',81*5**4*K**3,II2)
record('Gamma0 two factors, nine half terms',F(9,2)*2**4*K**2,GG0)
record('Gamma1 two coefficient products',F(9,2)*2**4*(K+II1*K),GG1)
record('Gamma2 two coefficient products',F(9,2)*2**4*(II1+II2*K),GG2)
record('Ric3 quadratic coefficient derivative/products',6*GG2+36*2**3*GG0*GG2+18*2**3*GG1**2,10**32)
# Exact log-W form has w*w_ab plus half w_a*w_b (12 W²), two
# connection source terms, and three-factor connection/gradient terms.
record('warped term from independent log-W expansion',12*W**2+24*GG2*K+24*GG1*W+81*GG0*W**2+27*K*W**2,10**25)
X0=10**13;X1=10**21;X2=10**26;W1=10**10
record('normalized sphere second derivative',W+3*16*K*W,W1)
record('sphere background Hessian',K+24*GG0*K,X0)
record('sphere first Hessian',W1+24*GG0*W1+24*GG1*K,X1)
record('sphere second Hessian',24*(GG1*W1+GG2*K),X2)
record('sphere inverse-Hessian contraction',36*(K*X2+II1*X1+II2*X0),10**33)
record('raw sphere r² product',16*10**33,2*10**34)
# More conservative than the primary's original +2: r^-2 derivatives through
# order three are at most24. Alternatively use the normalized sphere bound.
record('trace adjustment with explicit inverse-sphere derivatives',2*10**34*(1+F(27,2)*K*(9*K+48)),10**44)
record('exterior polynomial at decay margin .35',(4+F(2,F(35,100)))**2,100)
record('final physical C3 component bound',10**46*B**2,10**110)
# Independent coarse rational version of the primary source-to-scalar estimate.
# 0.12<s<0.14 follows from the certified root bracket. Hence exp(8s)<exp(2)<9,
# 3^(2s+2)<27, and 2s+2>2. No floating exponential is needed below.
Q=10**110;Cj=7*10**10*Q;CF=10**12*Q
moment=sum(F(math.comb(16,j)*4**(16-j)*math.factorial(j),2**(j+1)) for j in range(17))
leftsq=64*9*Cj**2*F(27,2)
tailsq=F(4,3)*CF**2*moment
source_squared=leftsq+tailsq
smin=F(1265,10000)
record('independent global source L2 squared',source_squared,10**256)
record('resolvent L2 squared using s>0.1265',source_squared/smin**4,(5*10**129)**2)
record('Sobolev sup squared using s>0.1265',2/smin**2*(1/smin+F(5,2)/smin**2)*source_squared,(2*10**130)**2)
out={'classification':'CONDITIONAL_EXACT_COEFFICIENT_COUNT_REVIEW','logW_identity':True,'checks':rows,
 'prerequisite':'Primary physical mixed H C5 bound4e29 and resulting EF coefficient bound2e31, checked by spectral review; this audit does not rerun its validated endpoint propagation.',
 'coverage':'Every quadratic orbit/sphere Ricci and trace-adjusted Einstein component through three radial derivatives, with declared EF/interior and weighted static/exterior norms.',
 'master_review':'Primary rational-coefficient source formulas and exact rational subdomain bounding inspected. Independent exact moment plus loose rational rate factors support ||F||L2<1e128, ||u||L2<5e129, ||u||infinity<2e130. Exact source-selected complex+2 scalar master only.',
 'not_covered':'Not by itself a full Y0 norm or physical metric M2: mixed Qmu_z fourth derivatives / master force third derivative and full regular lift/zero-sector constants remain separate.',
 'hashes':{n:hashlib.sha256((HERE/n).read_bytes()).hexdigest() for n in ['audit_goal005_geometry_bound_review.py','goal005_source_bounds.py','goal005_source_bounds.json','goal005_mode_bounds.py','goal005_mode_bounds.json','goal005_master_bounds.py','goal005_master_bounds.json']},
 'physics_note_sha256':hashlib.sha256((HERE.parent/'physics'/'goal005_enclosures.md').read_bytes()).hexdigest()}
(HERE/'audit_goal005_geometry_bound_review.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
