"""Weak returning projection coefficients in the normalized massive variables."""
from flint import acb

from horizon_series import endomorphism,inverse,matmul,matvec
from outer_gljet import njet

I=acb(0,1)
LAM=4


def dot(a,b):
    return sum(x*y for x,y in zip(a,b))


def transpose(a):
    return list(map(list,zip(*a)))


def test_covector(r,s,p):
    """Unit adjoint RW profile, in the covector of the weak contraction."""
    f=(r-1)/r
    return [(f+r*p)/(LAM*r*r),-1/(LAM*r*r)-(s+p)/(LAM*r*f)]


def test_geometry(r,k,omega,s,p_gl,p_h):
    f=(r-1)/r
    metric=njet(r,k,omega,1,(p_gl-omega)/f)[0]
    X,S=endomorphism(r,metric)
    av=test_covector(r,s,p_h)
    return X,S,av


def mixed_coefficients(r,k,omega,s,p_gl,p_h):
    """x-density coefficients for U and J-s1 U, and unit insertion terms."""
    f=(r-1)/r
    s1=s-omega
    X,S,av=test_geometry(r,k,omega,s,p_gl,p_h)
    w=matvec(transpose(matmul(X,inverse(r))),[-LAM*r*r*p for p in av])
    d=LAM+k*k*r*r
    droot=d.sqrt()
    dr=r*droot
    dw=2*r*r/droot
    a=f*(1/r+k*k*r/d)
    b=f*(2/r-k*k*r/d)
    gamma=I*k*r/2
    wc=3*f*S/r**3
    alpha=[-f*w[0]*(s1+a)/dr+w[1]*a/dr+wc*dr,
           -f*w[0]*gamma*s1/dr+w[1]*I*k*dw*b/(LAM*r*r)]
    beta=[(w[1]-f*w[0])/dr,-w[1]*I*k*dw/(LAM*r*r)]
    return alpha+beta+[-w[1]/(LAM*r*r),6*r*S*wc]


def canonical_baseline(r,sigma,psi,psix,launch):
    f=(r-1)/r
    av=(f*psi+r*psix)/(2*r*r)
    ar=(-psi/r**2+(sigma*psi-psix-launch)/(f*r))/2
    return [av,ar]


def algebraic_coefficients(r,k,omega,s,p_gl,p_h,psi,psix,launch):
    """x-density H²P with u²h removed, plus mean coefficients with h removed."""
    X,S,av=test_geometry(r,k,omega,s,p_gl,p_h)
    f=(r-1)/r
    sigma=s-2*omega
    base=matvec(inverse(r),canonical_baseline(r,sigma,psi,psix,launch))
    X2=matmul(X,X)
    trace=X2[0][0]+X2[1][1]
    direct=[[(X2[i][j]-(trace/4+S*S/2)*(i==j))/2 for j in range(2)] for i in range(2)]
    curl=(trace/4+7*S*S/2)*r*psi
    second=2*f*(LAM*r*r*dot(av,matvec(direct,base))-curl/r**3)
    # beta*I - inverse(EF background) times the EF monopole tensor.
    mu=2*f*LAM*r*r*av[1]*(-2*base[0]/r)
    beta=2*f*(LAM*r*r*av[1]*(f*base[0])+2*psi/r**2)
    return [second,mu,beta]


def homogeneous_coefficients(r,k,omega,s,p_gl,p_h,p_psi):
    return algebraic_coefficients(r,k,omega,s,p_gl,p_h,1,p_psi,0)
