import sympy as s
from itertools import product
v,r,k=s.symbols('v r k', real=True, positive=True)
I=s.I; a=k*k; D=1+a*r**3; f=(r-1)/r
A,B,C,S=(s.Function(n)(v,r) for n in ('A','B','C','S'))
g=s.Matrix([[-f,1,0],[1,0,0],[0,0,1]]); gi=g.inv()
h=s.Matrix([[A,B,0],[B,C,0],[0,0,0]])
inds=range(3)
def d(x,i,mode=True):
    return s.diff(x,(v,r)[i]) if i<2 else I*k*x if mode else 0
def sr(x): return s.factor(x)
G={(i,j,m):sr(sum(gi[i,n]*(d(g[n,j],m,False)+d(g[n,m],j,False)-d(g[j,m],n,False))/2 for n in inds)) for i,j,m in product(inds,repeat=3)}
dh={(i,j,m):d(h[j,m],i)-sum(G[n,i,j]*h[n,m]+G[n,i,m]*h[j,n] for n in inds) for i,j,m in product(inds,repeat=3)}
DG={(i,j,m):sr(sum(gi[i,n]*(dh[j,m,n]+dh[m,j,n]-dh[n,j,m])/2 for n in inds)) for i,j,m in product(inds,repeat=3)}
def dc(i,j,m,w):
    return d(DG[i,j,m],w)+sum(G[i,w,n]*DG[n,j,m]-G[n,w,j]*DG[i,n,m]-G[n,w,m]*DG[i,j,n] for n in inds)
dR=S/(2*r)
Ric={(j,m):sr(sum(dc(i,j,m,i)-dc(i,j,i,m) for i in inds)-2*(d(d(dR,j),m)-sum(G[n,j,m]*d(dR,n) for n in inds)-DG[1,j,m])/r-2*G[1,j,m]*dR/r**2) for j,m in product(inds,repeat=2)}
dgi=-gi*h*gi
lin_box_S=sum(gi[i,j]*(d(d(S,i),j)-sum(G[n,i,j]*d(S,n) for n in inds)) for i,j in product(inds,repeat=2))
lin_box_W=sum(dgi[i,j]*(2*(i==j==1)-2*r*G[1,i,j])-2*r*gi[i,j]*DG[1,i,j] for i,j in product(inds,repeat=2))
P,Bq,Rrr,Rang= -Ric[0,0],-Ric[0,1],-Ric[1,1],sr((lin_box_S+lin_box_W)/2)
j=-2*I*Ric[2,0]/k; l=-2*I*Ric[2,1]/k; tr=2*Ric[2,2]/a
U=2*a*r*((r-1)*B+r*A-s.diff(S,v))
J=2*a*r**2*f*(s.diff(j,r)-s.diff(l,v))-4*a*r*s.diff(Rang,v)-a*s.diff(tr,v)+(4*a*r**2*(a*r**3-2)/D)*(f*Bq+P)-2*a*(a*r**3*(2*r-3)-4*r+3)/D*j
Ph=1/(r-1)-5/r+6/(r*D)
master=f*s.diff(U,r,2)+2*s.diff(U,v,r)+f*Ph*s.diff(U,r)+(-4+6/D)/r*s.diff(U,v)+(3/r**3-a-3/(r**3*D))*U-J
b0=2*r**2*(j-r*Bq)/D; bm=-2*r**3*P/D
M=-(f*s.diff(U,r)+s.diff(U,v))/(a*D)+U/(2*a*D*r**2)+f*b0+bm
L=s.diff(M,r)+2*M/r-j-s.diff(tr,v)
RR=-s.diff(U,r,2)/(a*D)+(2*r**2*(s.diff(j,r)+s.diff(l,v))+4*r*j-2*r**3*s.diff(Rrr,v))/D
checks=[master,M-(f*B+A),r*M-U/(2*a*r)-s.diff(S,v), f*L+s.diff(M,v)-s.diff(A,v), -L-s.diff(B,v), RR-s.diff(C,v)]
for name,expr in zip(('master','M','qtheta','qvv','qvr','qrr'),checks): print(name,s.factor(expr))
assert all(s.factor(expr)==0 for expr in checks)
# Deliberate failures: drop the EF 2*d_v*d_r term or reverse rr source sign.
assert s.factor(master-2*s.diff(U,v,r))!=0
assert s.factor(checks[-1]+4*r**3*s.diff(Rrr,v)/D)!=0
print('negative controls rejected: missing EF mixed term, reversed RR Ricci term')
