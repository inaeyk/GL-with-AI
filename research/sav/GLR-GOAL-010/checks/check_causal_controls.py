import sympy as z
t,s,w,F=z.symbols('t s w F', positive=True)
q=F*(z.exp(s*t)-z.cos(w*t)-s/w*z.sin(w*t))/(s*s+w*w)
qp=F*z.exp(s*t)/(s*s+w*w)
assert z.simplify(z.diff(q,t,2)+w*w*q-F*z.exp(s*t))==0
assert q.subs(t,0)==z.diff(q,t).subs(t,0)==0
assert qp.subs(t,0)!=0 and z.diff(qp,t).subs(t,0)!=0
print('oscillator: exact equation and both zero initial data; separated-only control rejected')
l,A,B,H=z.symbols('lambda A B H')
P=lambda v: H-v*B+v**2*A
assert z.cancel(P(s)/(l-s)+A*(l+s)-B-P(l)/(l-s))==0
assert z.cancel(P(s)/(l-s)+A*(l+s)-P(l)/(l-s))==B
print('tilted Laplace: compatible separated jet leaves only the source pole; omitted tilt term detected')

import sympy as z
r,a=z.symbols('r a',positive=True)
f=1-1/r
D=1+a*r**3
V=a*f+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D)
w0=z.exp(-r/2)*r/D
P0=(4*a*a-a)*r**5-7*a*r**4+10*a*r**3-(8*a+1)*r*r+5*r-2
def zero(e):
    return z.cancel(e)==0
d1=(V*w0-f*z.diff(f*z.diff(w0,r),r))/w0-f*P0/(4*r*r*D)
d2=z.limit(V/f,r,1)-(a-5+6/(a+1))
print('S-deformation residual:',z.cancel(d1))
print('potential horizon rational difference:',z.cancel(d2))
assert zero(d1)
assert zero(d2)
rho,b=z.symbols('rho b', nonnegative=True)
coef=z.Poly(P0.subs({r:1+rho,a:z.Rational(3,2)+b}).expand(),rho,b).coeffs()
assert all(c>0 for c in coef)
print('strict coefficient signs:',len(coef))
print('V/f(1) at K=2pi/5:', z.N((a-5+6/(a+1)).subs(a,4*z.pi**2/25),12))
print('SymPy',z.__version__)
