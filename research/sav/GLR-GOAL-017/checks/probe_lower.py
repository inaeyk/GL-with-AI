"""Whole-cell mixed residual probes for numerical coefficient interpolants."""
from itertools import product
from math import comb

from flint import arb, ctx
import numpy as np


def coefficients(w, pu, pv, uv, hu, hv):
    out = []
    for i in range(4):
        row = []
        for j in range(4):
            side_u = (3-2*i) if i in (1,2) else 0
            side_v = (3-2*j) if j in (1,2) else 0
            x,y = i//2,j//2
            row.append(w[x][y]+side_u*hu*pu[x][y]/3+side_v*hv*pv[x][y]/3
                       +side_u*side_v*hu*hv*uv[x][y]/9)
        out.append(row)
    return out


def multiply(a,b):
    m,n,p,q = len(a)-1,len(a[0])-1,len(b)-1,len(b[0])-1
    out = [[arb(0) for _ in range(n+q+1)] for _ in range(m+p+1)]
    for i,j,k,l in product(range(m+1),range(n+1),range(p+1),range(q+1)):
        coef = (comb(m,i)*comb(n,j)*comb(p,k)*comb(q,l))
        out[i+k][j+l] += a[i][j]*b[k][l]*coef/(comb(m+p,i+k)*comb(n+q,j+l))
    return out


def elevate(a,m,n):
    return multiply(a,[[arb(1)]*(n+2-len(a[0])) for _ in range(m+2-len(a))])


def derivatives(w,hu,hv):
    du = [[3*(w[i+1][j]-w[i][j])/hu for j in range(4)] for i in range(3)]
    dv = [[3*(w[i][j+1]-w[i][j])/hv for j in range(3)] for i in range(4)]
    uv = [[3*(du[i][j+1]-du[i][j])/hv for j in range(3)] for i in range(3)]
    dtu, dtv = elevate(du,3,3), elevate(dv,3,3)
    dt = [[dtu[i][j]+dtv[i][j] for j in range(4)] for i in range(4)]
    return dt, uv


def inspect(s,W,P,Q,V,S,C):
    ctx.prec = 112
    probes = [(s.u[0]+.03,1.626),(s.u[0]+.06,1.67),(.2,1.61),(3,1.65),(7.9,1.65),
              (.2,4),(3,4),(7.9,4),(3,25),(7.9,80)]
    print("directed interior-cell mixed residual probe caps for numerical coefficients and source")
    for u,v in probes:
        i,j = np.searchsorted(s.u,u)-1,np.searchsorted(s.v,v)-1
        assert s.mask[i,j]
        hu = arb(float(s.u[i+1]))-arb(float(s.u[i]))
        hv = arb(float(s.v[j+1]))-arb(float(s.v[j]))
        # A binary-dyadic definition of each nodal interpolant.
        def block(values):
            return [[arb(float(t)) for t in row] for row in values[i:i+2,j:j+2]]

        w,pu,pv,pot,src = [[block(z[...,k]) for k in range(4)] for z in (W,P,Q,V,S)]
        c = block(C)
        yh,tyh,uyh = [],[],[]
        for k in range(4):
            other = k+1 if k%2==0 else k-1
            sign = -1 if k%2==0 else 1
            uv = [[(sign*c[x][y]*(pu[other][x][y]+pv[other][x][y])
                    -pot[k][x][y]*w[k][x][y]-src[k][x][y])/4
                   for y in range(2)] for x in range(2)]
            Y = coefficients(w[k],pu[k],pv[k],uv,hu,hv)
            dt,dudv = derivatives(Y,hu,hv)
            yh.append(Y)
            tyh.append(dt)
            uyh.append(elevate(dudv,4,4))
        caps = []
        for k in range(4):
            other = k+1 if k%2==0 else k-1
            sign = -1 if k%2==0 else 1
            vv = multiply(pot[k],yh[k])
            cc = multiply(c,tyh[other])
            ss = elevate(src[k],4,4)
            cap = max((-4*uyh[k][x][y]-vv[x][y]+sign*cc[x][y]-ss[x][y]).abs_upper()
                      for x,y in product(range(5),repeat=2))
            caps.append(cap.str(9))
        print(f"  u~{u:.7f}, v~{v:.7f}, {hu.str(5)} x {hv.str(5)}: "
              f"uncommuted {caps[:2]}; "
              f"shifted {caps[2:]}")
