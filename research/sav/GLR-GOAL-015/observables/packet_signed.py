"""One full-packet refinement: keep the subprincipal F primitive signed."""
from pathlib import Path
import sys

from flint import arb, arb_mat, ctx

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/"analysis"))
from early_optical_certificate import GeometryEnclosure, coeffs, overlap, x, upper, vector_norm
from packet_weak import exact_norms, raw_q, cap_phase


def hull0(value):
    return value.union(arb(0))


def column(a,b):
    return arb_mat([[a],[b]])


def nvec(v):
    return vector_norm([v[0,0],v[1,0]])


def product_step(v,dv,wedge):
    return column(*[hull0(v[i,0]*dv) if wedge else v[i,0]*dv for i in range(2)])


def Wmass(r):
    return 3/r-3/(4*r*r)


def bound():
    ctx.prec,ctx.cap=160,80
    geo=GeometryEnclosure(refinement=1)
    D=arb(3)/25
    R=arb(13)/8
    a=R-2*x(R)
    d=a+D
    cut=arb("1.596")
    assert cut-2*x(cut)>d
    M=arb("1.60")
    W=arb(1)/4
    assert Wmass(cut)+((D/4).exp()-1)*(Wmass(cut)-Wmass(arb(2)))<M
    beta=arb(26)/21
    bp=arb(5)/21
    B,BSup,B2sqlo=exact_norms(geo.omega)
    gP0=arb(0)
    GQ=[arb(0)]*3
    g0=g1=None
    for j in range(64):
        h=(R-cut)/64
        rg=(cut+j*h).union(cut+(j+1)*h)
        q,_,_,_,_,gp,gq,_=coeffs(geo,rg,rg)
        _,G,*_=coeffs(geo,rg,rg,signed=True)
        gg=(2*rg/(rg+1))*q
        g0=gg if g0 is None else g0.union(gg)
        g1=[G[i,0] if g1 is None else g1[i].union(G[i,0]) for i in range(2)]
        gP0=max(gP0,gp[2])
        GQ=[max(l,h) for l,h in zip(GQ,gq)]
    kap=[2*D/arb.pi(),D]
    conf=[]
    for norm,k,leng in zip((B,BSup),kap,(D.sqrt(),arb(1))):
        b0,b1,b2=norm
        wy=(k*M).exp()*b0
        pe=M*wy
        p=b1+pe
        q=k*W*wy
        base=[bp*W*b0+M*(wy/4+pe),W*wy,
              5*W*BSup[0]*leng+k*(W*wy/4+W*q),pe,q,wy]
        Qg=(GQ[0]*BSup[1]+GQ[2]*BSup[0])*leng
        conf.append((norm,k,leng,base,Qg))
    ep=[gP0*B[0],gP0*BSup[0]]
    rerrF=[arb(0),arb(0)]
    absF=absH=Gwork=Cint=arb(0)
    Aint=arb(0)
    Cprior=column(*g1)
    e1=column(1,0)
    Jb=Wmass(cut.union(R))
    segments=[(cut,arb(5)/2,512),(arb(5)/2,arb(8),768),
              (arb(8),arb(25),512),(arb(25),arb(64),256),(arb(64),arb(400),256)]
    for lo,hi,n in segments:
        for j in range(n):
            s0,s1=lo+(hi-lo)*j/n,lo+(hi-lo)*(j+1)/n
            v0,v1=d+2*x(s0),d+2*x(s1)
            dh=v1-v0
            dv=dh.upper()
            rhi=1+(x(s1)+D/2-1).exp().lambertw()
            r,v=s0.union(rhi),v0.union(v1)
            q,S,Y,Bm,Mm,_,_,w=coeffs(geo,r,v)
            Su,_,Mp,L,FP,FQ,FX,FB=coeffs(geo,r,v,signed=True)
            _,qr=raw_q(geo,r)
            qu=-(1-1/r)/2*(geo.omega*v).exp()*qr
            qmax=upper(q)
            wedge=v0<=R
            astep=q*dh
            A=Aint+hull0(astep)
            Aint+=hull0(astep) if wedge else astep
            # Scalar J=∫W on active graph rays. Intersection with its
            # positive absolute bound preserves its sign on wedge cells.
            J=(Jb-Wmass(r)).intersection(arb(0).union(M))
            S1=Su-q*J*e1
            X0=-A*e1/4
            Q0=-q*e1/4
            CP_rhs=-Mp*X0-S1/4
            C=Cprior+product_step(CP_rhs,dh,True)
            Cprior+=product_step(CP_rhs,dh,wedge)
            Au=geo.omega*A-q+g0
            DX=C+(Au*e1-2*A*(L*e1))/4
            # Q uses M_- rather than M_+. Use triangle on its unshared
            # potential action. Signed derivative-minus-source retained.
            DQnorm=nvec((qu*e1-2*q*(L*e1)-S1)/4)+Mm*nvec(X0)
            CF=(FP*C+FQ*Q0+FX*X0)[0,0]+FB
            Cint+=hull0(CF*dh) if wedge else CF*dh
            for i,(norm,k,leng,base,Qg) in enumerate(conf):
                b0,b1,b2=norm
                ES=overlap(S,base)
                DXwork=nvec(DX)*b1
                pe=(ep[i]+dv*(Mm*k*DXwork+ES/4))*(Mm*k*dv).exp()
                xe=k*(pe+DXwork)
                qe=(Qg+qmax*BSup[1]*leng/4 if wedge else 0)+k*(Mm*xe+DQnorm*b1+ES/4)
                ep[i]+=dv*(Mm*xe+ES/4)
                de=Y[0][0]*pe+Y[0][1]*qe+Y[0][2]*xe+overlap(Bm[0],base[3:])
                rerrF[i]+=dv*de
                if i==0:
                    pa=upper(A)*b2/4+nvec(C)*b1+pe
                    qa=nvec(Q0)*b1+qe
                    xa=nvec(X0)*b1+xe
                    bjets=[b1+base[3],base[4],base[5]]
                    Fa=upper(q*A/2)*b2+upper(CF)*b1+de
                    Ga=Y[1][0]*pa+Y[1][1]*qa+Y[1][2]*xa+overlap(Bm[1],bjets)
                    Ha=Y[2][0]*pa+Y[2][1]*qa+Y[2][2]*xa+overlap(Bm[2],bjets)
                    absF+=dv*Fa
                    absH+=dv*Ha
                    Gwork+=dv*w*Ga
    phase=cap_phase(geo,a,D,1)
    assert phase>0
    print("signed C_F primitive", Cint.lower().str(18), Cint.upper().str(18))
    print("residual F norms", rerrF[0].str(18), rerrF[1].str(18))
    # g' comparison includes the J∞(u)B' scalar term. Its product against
    # the leading optical B'' has exact constant part cancellation too.
    def radius(box):
        return upper(box-box.mid())
    # Baseline E_uu here is for g'−(B''−J∞B'). The norm of g'−B''
    # additionally contains ||J∞B'||.
    testerr=conf[0][3][0]
    dg=testerr+M
    leading=phase.lower()*B2sqlo
    test=(phase.upper()*testerr+
          radius(phase*Jb))*B[2]
    cancel=radius(Cint)*B[2]+upper(Cint)*dg
    term=arb("0.009755")*rerrF[1]
    other=(B[2]+dg)*rerrF[0]
    Z=(M*D).exp()*arb("1.000930")
    dual=Z*(M*absF+absH+kap[0]*Gwork)
    lo=(-phase.upper()*B[2]**2-test-cancel-term-other-dual)/4-arb("1.63462e-14")
    hi=(-leading+test+cancel+term+other+dual)/4+arb("1.63462e-14")
    for name,value in (("4*negative leading lower",leading),("4*baseline test error",test),
                       ("4*signed primitive cancellation error",cancel),("4*terminal bound",term),
                       ("4*other F error",other),("4*potential/G/H",dual),
                       ("P lower",lo.lower()),("P upper",hi.upper())):
        print(name,value.str(18))
    print("P_PACKET_STRICTLY_NEGATIVE",hi<0)
    return lo,hi


if __name__=="__main__":
    bound()
