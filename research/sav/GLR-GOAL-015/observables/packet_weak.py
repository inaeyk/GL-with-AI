"""Separate directed full-packet optical weak bound (no pilot ordinates)."""
from pathlib import Path
import argparse
import sys

from flint import arb, ctx

sys.path.insert(0,str(Path(__file__).resolve().parents[1]/"analysis"))
from early_optical_certificate import GeometryEnclosure, coeffs, overlap, x, upper
from shifted_baseline import bump_jets


def exact_norms(om,n=2048):
    # Reflection supplies the second half; all normed jets have fixed parity.
    sq=[arb(0),arb(0),arb(0)]
    sup=[arb(0),arb(0),arb(0)]
    h=arb(1)/(2*n)
    for i in range(n):
        cell=(i*h).union((i+1)*h)
        vals=bump_jets(cell,om)[:3]
        for j,v in enumerate(vals):
            sq[j]+=2*h*(arb(3)/25)*v**2
            sup[j]=max(sup[j],upper(v))
    return [sq[0].sqrt().upper(),arb(1),sq[2].sqrt().upper()],sup,sq[2].lower()


def raw_q(geo,r):
    (h,hr,_),_,_=geo.evaluate(r)
    f=1-1/r
    a,b,_,c=h
    ar,br,_,cr=hr
    q=-2*a/f-b+c/r**2
    qr=-2*ar/f+2*a/(f*f*r*r)-br+cr/r**2-2*c/r**3
    return q,qr


def cap_phase(geo,front,width,ref):
    cut=arb("1.596")
    start=arb(13)/8
    rc=arb(400)
    segments=[(cut,start,80,True),(start,arb(5)/2,768,False),
              (arb(5)/2,arb(8),1024,False),(arb(8),arb(25),512,False),
              (arb(25),arb(64),256,False),(arb(64),rc,256,False),
              (rc,rc+width/2,1,True)]
    integral=arb(0)
    for a,b,n,optional in segments:
        n*=ref
        dr=(b-a)/n
        for i in range(n):
            r=(a+i*dr).union(a+(i+1)*dr)
            q,_=raw_q(geo,r)
            val=dr*2*(2*geo.omega*x(r)).exp()*q/(1-1/r)
            integral+=val.union(arb(0)) if optional else val
    A=(geo.omega*front.union(front+width)).exp()*integral
    assert A>0 or A<0
    return A*A/4


def bound(refinement=1):
    ctx.prec,ctx.cap=160,80
    geo=GeometryEnclosure(refinement=refinement)
    D=arb(3)/25
    R=arb(13)/8
    a=R-2*x(R)
    d=a+D
    cut=arb("1.596")
    assert cut-2*x(cut)>d
    M=arb("1.60")
    W=arb(1)/4
    Jcut=3/cut-3/(4*cut**2)
    J2=3/arb(2)-3/(4*arb(2)**2)
    assert Jcut+((D/4).exp()-1)*(Jcut-J2)<M
    bprime=arb(5)/21
    B,BSup,B2sqlo=exact_norms(geo.omega)
    print("packet B,B',B'' L2 upper",*B)
    print("packet B,B',B'' sup upper",*BSup)
    print("packet ||B''||_2^2 lower",B2sqlo)
    gp,gq,gg=[arb(0)]*3
    # Cover the actual wedge graph by whole cells for maximum jet coefficients.
    for j in range(32*refinement):
        dr=(R-cut)/(32*refinement)
        r=(cut+j*dr).union(cut+(j+1)*dr)
        q,_,_,_,_,p0,q0,_=coeffs(geo,r,r)
        gg=max(gg,upper(q))
        if j==0:
            gp=p0
            gq=q0
        else:
            gp=[max(l,h) for l,h in zip(gp,p0)]
            gq=[max(l,h) for l,h in zip(gq,q0)]
    abs_beta=arb(26)/21
    Ew=[]
    kappa=[2*D/arb.pi(),D]
    for norm,k,leng in zip((B,BSup),kappa,(D.sqrt(),arb(1))):
        b0,b1,b2=norm
        wy=(k*M).exp()*b0
        p=b1+M*wy
        q=k*W*wy
        pu=bprime*W*b0+M*(wy/4+p)
        pv=W*wy
        qv=5*W*BSup[0]*leng+k*(W*wy/4+W*q)
        Pgraph=gp[0]*b1+gp[2]*b0
        Qgraph=(gq[0]*BSup[1]+gq[2]*BSup[0])*leng
        Ew.append(([pu,pv,qv,p,q,wy],Pgraph,Qgraph))
    # P-Popt = Pgraph - integrals of M+ X and the nonprincipal source.
    # Xopt and Qopt use u-integration by parts of A B'' and q B''.
    Aint=arb(0)
    ep=[t[1] for t in Ew]
    errF=[arb(0),arb(0)]
    absF=absH=Gwork=arb(0)
    panels=[(cut,arb(5)/2,384),(arb(5)/2,arb(8),768),(arb(8),arb(25),512),
            (arb(25),arb(64),256),(arb(64),arb(400),256)]
    for lo,hi,n in panels:
        n*=refinement
        for j in range(n):
            s0,s1=lo+(hi-lo)*j/n,lo+(hi-lo)*(j+1)/n
            v0,v1=d+2*x(s0),d+2*x(s1)
            dv=(v1-v0).upper()
            rhi=1+(x(s1)+D/2-1).exp().lambertw()
            r,v=s0.union(rhi),v0.union(v1)
            q,S,Y,Bm,Wm,_,_,w=coeffs(geo,r,v)
            _,qr=raw_q(geo,r)
            f=1-1/r
            L=upper(geo.k*f/(4+geo.k**2*r*r))
            qu=upper(f/2*(geo.omega*v).exp()*qr)
            qmax=upper(q)
            step=q*(v1-v0)
            inc=step.union(arb(0))
            Amax=upper(Aint+inc)
            wedge=v0<=R
            Aint+=inc if wedge else step
            for i,(k,leng,norm,weights) in enumerate(zip(kappa,(D.sqrt(),arb(1)),
                                                         (B,BSup),Ew)):
                jets,_,Qgraph=weights
                Serr=overlap(S,jets)
                Xopt=(Amax+k*((geo.omega+2*L)*Amax+qmax+abs_beta*gg))*norm[1]/4
                Qopt=(qmax*norm[1]+k*(qu+2*L*qmax)*norm[1]
                      +(qmax*BSup[1]*leng if wedge else 0))/4
                emax=(ep[i]+dv*(Wm*Xopt+Serr/4))*(Wm*k*dv).exp()
                ep[i]+=dv*(Wm*(Xopt+k*emax)+Serr/4)
                Xall=Xopt+k*emax
                Qall=(Qgraph if wedge else 0)+Qopt+k*(Wm*Xall+Serr/4)
                Popt=Amax*norm[2]/4
                Ferror=Y[0][0]*emax+Y[0][1]*Qall+Y[0][2]*Xall+overlap(Bm[0],jets[3:])
                errF[i]+=dv*Ferror
                if i==0:
                    Fabs=Ferror+Y[0][0]*Popt
                    Gabs=Y[1][0]*(emax+Popt)+Y[1][1]*Qall+Y[1][2]*Xall+overlap(Bm[1],jets[3:])
                    Habs=Y[2][0]*(emax+Popt)+Y[2][1]*Qall+Y[2][2]*Xall+overlap(Bm[2],jets[3:])
                    absF+=dv*Fabs
                    absH+=dv*Habs
                    Gwork+=dv*w*Gabs
    phase=cap_phase(geo,a,D,refinement)
    assert phase>0
    gn=arb("1.000930")
    dg=Ew[0][0][0]
    # At d the optical terminal primitive vanishes because B''(d)=0.
    term=arb("0.009755")*errF[1]
    lead_neg=phase.lower()*B2sqlo
    test=phase.upper()*dg*B[2]
    Fwork=(B[2]+dg)*errF[0]
    Z=(D*M).exp()*gn
    dual=Z*(M*absF+absH+kappa[0]*Gwork)
    lo=(-phase.upper()*B[2]**2-test-term-Fwork-dual)/4-arb("1.63462e-14")
    hi=(-lead_neg+test+term+Fwork+dual)/4+arb("1.63462e-14")
    for name,value in (("phase square lower",phase.lower()),("phase square upper",phase.upper()),
                       ("F mismatch L1_vL2_u",errF[0]),("F terminal mismatch upper",errF[1]),
                       ("g' minus B'' L2",dg),("four times optical negative size",lead_neg),
                       ("four times terminal",term),("four times leading-test error",test),
                       ("four times F mismatch work",Fwork),("four times potential/G/H work",dual),
                       ("P lower",lo.lower()),("P upper",hi.upper())):
        print(name,value.str(18))
    print("P_PACKET_STRICTLY_NEGATIVE",hi<0)
    return lo,hi


if __name__=="__main__":
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument("--refinement",type=int,default=1)
    args=p.parse_args()
    bound(args.refinement)
