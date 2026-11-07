import sympy as s
e,k,h,h1,h2,W,X,P,J,P1,J1,d1,beta=s.symbols("e k h h1 h2 W X P J P1 J1 d1 beta", real=True)
I=s.I
def cf(expr,i=2):
    return s.Poly(s.expand(expr), e).nth(i)
def tr(expr):
    return sum(cf(expr,i)*e**i for i in range(3))
def dz(expr):
    return I*k*e*s.diff(expr,e)
r=s.Rational(2)
U0=r/(r+1); U0r=1/(r+1)**2; a0=1/s.sqrt(U0)
d0=1/(2*r**s.Rational(3,2)*(r+1)**s.Rational(3,2)); P0=2*s.sqrt(U0)
H0=2/r; H1=2*h1; B0=2/r**2; B1=4*h2+12*h1/r; B2=6*h1**2
A=-a0**3*W/(2*r); A2=-a0**3*X/(2*r)+3*a0**5*W**2/(8*r**2)
d2=-beta*J/(2*I*k*a0*r**3)
a=a0+e*A+e**2*A2; ai=1/a0-e*A/a0**2+e**2*(A**2/a0**3-A2/a0**2)
Hinv=(1-e*H1/H0+e**2*H1**2/H0**2)/H0; U=U0+(e*W+e**2*X)/r
QQ=P0+e*P1+e**2*P; JJ=e*J1+e**2*J
g=e*h; gr=e*h1; d=d0+e*d1+e**2*d2; ell=1/r+gr
Z=tr(-QQ**2/(2*r**4)+2*d*QQ/r**2-2*JJ**2/r**4)
Rh=tr(2*(1-2*g+2*g**2)/r**2-4*dz(dz(g))-6*dz(g)**2)
B=B0+e*B1+e**2*B2
w2_direct=cf(X*e**2/r+r*(-2*ai*Hinv*(dz(dz(a))+2*dz(g)*dz(a))+
                       Hinv*(Rh+Z-U*B)-U0r))
P2_direct=cf(a*dz(JJ)+2*dz(a)*JJ+2*a*dz(g)*JJ-2*gr*QQ+2*r**2*ell*d)
J2_direct=cf(a*dz(QQ)/2-dz(a)*QQ/2+a*dz(g)*QQ-2*gr*JJ+r**2*dz(tr(a*d)))
D0=1/(H0*U0); D1=-D0*(H1/H0+W/(r*U0))
Z1=(2*d0/r**2-P0/r**4)*P1+2*d1*P0/r**2
Z2=(2*d0/r**2-P0/r**4)*P+2*d2*P0/r**2-P1**2/(2*r**4)+2*d1*P1/r**2-2*J1**2/r**4
Rh1=-4*h/r**2+4*k**2*h; Rh2=4*h**2/r**2+6*k**2*h**2
N1=Rh1+Z1-U0*B1-H1*U0r; N2=Rh2+Z2-U0*B2
w2=-4*k**2*D0*X-k**2*D1*W+2*D0*(I*k*h)*(I*k*W)-3*(I*k*W)**2/(2*r*H0*U0**2)+(-B1/H0+B0*H1/H0**2)*W+r*(N2-H1*N1/H0)/H0
P2=2*I*k*a0*J+2*r*d2+3*I*k*A*J1+2*I*k*a0*h*J1-2*h1*P1+2*r**2*h1*d1
J2=2*I*k*(a0*P/2+(r**2*d0-P0/2)*A2)+I*k*h*(a0*P1+A*P0)-2*h1*J1+2*I*k*r**2*A*d1-beta*J/r
deltas=[s.simplify(difference) for difference in (w2-w2_direct,P2-P2_direct,J2-J2_direct)]
assert deltas==[0,0,0]
negative=[s.simplify(w2-2*D0*(I*k*h)*(I*k*W)-w2_direct),
          s.simplify(P2-I*k*A*J1-P2_direct),
          s.simplify(J2+2*beta*J/r-J2_direct)]
assert all(negative)
print("direct geometric a expansion and w reconstruction", deltas)
print("negative controls (omit lapse drift, wrong product factor, reversed damping)", negative)
