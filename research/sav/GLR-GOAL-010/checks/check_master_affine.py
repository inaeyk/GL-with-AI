import sympy as m
r,K,s=m.symbols('r K s',positive=True); I=m.I; a=K**2; D=1+a*r**3; f=1-1/r
T,B,C,S=[m.Function(k)(r) for k in ('qvv','qvr','qrr','S')]
g=m.Matrix([[-f,1,0],[1,0,0],[0,0,1]]); G=g.inv(); h=m.Matrix([[T,B,0],[B,C,0],[0,0,0]]); dG=-G*h*G
def d0(u,i): return m.diff(u,r) if i==1 else m.S.Zero
def d(u,i): return (s*u,m.diff(u,r),I*K*u)[i]
A=range(3)
def cs(h,d,i,j,k): return d(h[k,i],j)+d(h[k,j],i)-d(h[i,j],k)
Gamma={(k,i,j):sum(G[k,l]*cs(g,d0,i,j,l)/2 for l in A) for k in A for i in A for j in A}
delta={(k,i,j):sum((G[k,l]*cs(h,d,i,j,l)+dG[k,l]*cs(g,d0,i,j,l))/2 for l in A) for k in A for i in A for j in A}
w0=r**2; Dw0=[0,2*r,0]
hess0={(i,j):d0(Dw0[i],j)-2*r*Gamma[1,i,j] for i in A for j in A}
hess={(i,j):d(d(S,i),j)-sum(Gamma[k,i,j]*d(S,k) for k in A)-2*r*delta[1,i,j] for i in A for j in A}
def src(i,j):
  ric0=sum(d(delta[k,i,j],k)-d(delta[k,i,k],j) for k in A)
  ric1=sum(Gamma[k,k,l]*delta[l,i,j]+delta[k,k,l]*Gamma[l,i,j]-Gamma[k,j,l]*delta[l,i,k]-delta[k,j,l]*Gamma[l,i,k] for k in A for l in A)
  return m.factor(-(ric0+ric1+S*hess0[i,j]/w0**2-hess[i,j]/w0-S*Dw0[i]*Dw0[j]/w0**3+(Dw0[i]*d(S,j)+Dw0[j]*d(S,i))/(2*w0**2)))
Qv,Qb,Qc,Qzz,Qvz,Qrz=[src(i,j) for i,j in [(0,0),(0,1),(1,1),(2,2),(0,2),(1,2)]]
Qt=m.factor(sum(dG[i,j]*hess0[i,j]+G[i,j]*hess[i,j] for i in A for j in A)/2)
Z=2*Qb+f*Qc+2*Qt/r**2+Qzz; tau=-2*Qzz/a; jv=2*I*Qvz/K; jr=2*I*Qrz/K
P=1/(r-1)-5/r+6/(r*D); Qh=3/(r*r*(r-1))-a*r/(r-1)-3/(r*r*(r-1)*D)
Y=-4*r*T-2*(a*r**3-2*r+3)/r**2*S+4*(f*d(S,1)+d(S,0))-4*r*f*tau
M=-2*a*r*r*(f*jr+jv)-4*a*r*Qt-a*tau+2*r*r*(f*d(Z,1)+d(Z,0))+4*r*r*(d(Qv,1)-d(Qb,0))-2*(a*r**3*(2*r-3)-4*r+3)*Z/D-8*r*(a*r**3-2)*Qv/D
L=f*d(d(Y,1),1)+2*d(d(Y,1),0)+f*P*d(Y,1)+(P-d0(f,1)/f)*d(Y,0)+f*Qh*Y
N=f*Z+2*Qv; Cs=2*r**3*N/(a*D); A0=(a*r**3-2*r+3)/(2*r**3); G0=f*(m.Rational(1,2)-a*r**3)/D**2; E=(r-1)/(2*a*r**3*D)-3*(r-1)**2/D**2
Rs=-r**2*Y/(2*D)-(r-1)/(a*D)*d(Y,1)-r/(a*D)*d(Y,0)
Rv=G0*Y+E*(d(Y,1)+d(Y,0)/f)-d(d(Y,0),0)/(a*D)
Rb=-d(d(Y,0),1)/(a*D)+3*r*r/D**2*d(Y,0)-G0/f*Y-E/f*d(Y,1)
Rc=-d(d(Y,1),1)/(a*D)
recipes=[('master',L-M),
 ('S',Rs+Cs-S),
 ('vv',Rv-f*M/(a*D)-A0*Cs+(f*d(Cs,1)+d(Cs,0))/r-f*tau-T),
 ('vr',Rb-2*Qb/a+Z/a-2*r*N/(a*D)+M/(a*D)-d(Cs,1)/r+tau-B),
 ('rr',Rc+(2*(2*r-1-a*r**3)*Qc+2*r*(r-1)*d(Qc,1)+4*d(Qt,1)+4*r*Qzz+2*r*r*d(Qzz,1))/(a*D)+2*r*r*jr/D-C)]
for name,diff in recipes:
    # Fast simplification on rational coefficient of each arbitrary function jet
    x=m.expand(diff); atoms=x.atoms(m.Derivative); atoms.update((T,B,C,S))
    bad=[(p,m.factor(x.coeff(p))) for p in atoms if m.factor(x.coeff(p))!=0]
    assert not bad, (name, bad)
    print(name, 'PASS', flush=True)
print('SymPy',m.__version__)
