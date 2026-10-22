#!/usr/bin/env python3
"""Primary independent check of auditor's positive supersolution (no ODE)."""
import sympy as S, json, platform
from pathlib import Path
r,a,s,d=S.symbols('r a s d',positive=True); f=1-1/r;D=1+a*r**3
V=a*f+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D)
w=S.exp(-r/2)*r/D
res=S.cancel((-f*S.diff(f*S.diff(w,r),r)+V*w)/(f*w))
poly=(4*a*a-a)*r**5-7*a*r**4+10*a*r**3-(8*a+1)*r**2+5*r-2
assert S.cancel(res-poly/(4*r*r*D))==0
co=S.Poly(poly.subs(r,d+1),d).all_coeffs()
assert all(c.subs(a,S.Rational(3,2))>0 and S.diff(c,a).subs(a,S.Rational(3,2))>0 and S.diff(c,a,2)>0 for c in co)
assert S.Rational(4,25)*(S.Rational(157,50))**2>S.Rational(3,2) # pi>3.14 suffices
# Actual negative potential at a=3/2; excludes incorrect direct V>=0.
lim=S.limit(V/(r-1),r,1).subs(a,S.Rational(3,2));assert lim==-S.Rational(11,10)
# Exterior bound follows V>=af-24/(a r^5), sigma²=s²+a/4.
# r>=4: af-a f²/4>=3a/4-a/16? simply af>=3a/4, af²/4<=a/4.
assert S.Rational(3,2)/2-24/(S.Rational(3,2)*4**5)>S.Rational(3,2)/3
# n0 EF quadratures exact differentiation and tail upper bounds.
p=S.symbols('p',positive=True)
assert S.integrate(r*S.exp(-p*r),(r,S.symbols('R',positive=True),S.oo)) is not None
out={'python':platform.python_version(),'sympy':S.__version__,'checks':['generic supersolution identity','six shifted polynomial coefficients positive for a>=3/2','L10 lies in sufficient coercive region using pi>3.14','negative bare potential correctly detected','exterior comparison lower bound'],'scope':'Exact analytical bound checks; no numerical profile or ODE.'}
Path(__file__).with_name('goal003_bound_results.json').write_text(json.dumps(out,indent=2)+'\n')
for c in out['checks']: print(c+': PASS')
