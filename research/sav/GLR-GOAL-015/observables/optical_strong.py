"""Directed compact strong-news bound using the shifted optical reference.

The rotated source/return coefficient enclosures are shared with the weak
route. The two-polarization commuted graph, scalar cap recovery, norms and
comparison with the digest-bound affine cap are separate. No physical
production evolution; the input pilot only supplies affine ordinates.
"""
from pathlib import Path
import argparse
import hashlib
import sys

from flint import arb, arb_mat, ctx
import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "analysis"))
from early_optical_certificate import (GeometryEnclosure, x, coeffs, col,
    diag, overlap, seed_bounds, upper, vector_norm)
from shifted_baseline import bump_jets, weights, derivative_norm

CAP_DIGEST="38ecd9ed4d942aeb261cc197bf5662e9d80a584fa6dae955a322a61817915c39"
D0,D1,D2=522,380,220


def graph(geo, r):
    """Upper mixed commuted graph norms in units of h', h=B'+2ΩB."""
    k, om=geo.k, geo.omega
    f=1-1/r
    p=-1/r
    px=f/r**2
    (h,hr,_),_,_=geo.evaluate(r)
    amp=(om*r).exp()
    a,b,c,_=[amp*t for t in h]
    ar,br,cr,_=[amp*t for t in hr]
    def orb(a,b,c):
        tx=a+f*b
        return arb_mat([[-a,-tx],[tx,a+2*f*b+f*f*c]])/f
    xb=orb(a,b,c)
    xb_x=(om-1/r**2)*xb+f*orb(ar,br,cr)+arb_mat(
        [[0,-b],[b,2*(b+f*c)]])/r**2
    eta=diag(-1,1)
    A=arb_mat([[-1,1,f/r],[1,1,0]])
    Ax=arb_mat([[0,0,f*(1/r**3-f/r**2)],[0,0,0]])
    J=-r*xb*eta*A
    Jx=(f/r)*J-r*(xb_x*eta*A+xb*eta*Ax)
    d=4+k*k*r*r
    alpha,beta=1/(r*d.sqrt()),k/(2*d.sqrt())
    am=f*(1/r+k*k*r/d)
    bm=f*k*k*r/d
    M=diag(alpha,beta)*arb_mat([[p,-1],[1,-p]])/(1-p*p)
    Mx=diag(-am,-bm)*M+diag(alpha,beta)*diag(px,-px)/(1-p*p)
    Mx+=(2*p*px/(1-p*p))*M
    g=M*J
    gx=Mx*J+M*(Jx+p*om*J)
    G=[vector_norm(col(g,j)) for j in (0,2)]
    Gx=[vector_norm(col(gx,j)) for j in (0,2)]
    # g = G_u B' + G_0 B; h' dominates B'' and 380 B'.
    gu=(G[0]+G[1]/D0)/D1
    gxu=(Gx[0]+Gx[1]/D0)/D1+upper(p-1)*(G[0]+G[1]/D1)
    _,S,F,A0,_,p0,q0,W=coeffs(geo,r,r)
    bp=-(1+p)/(1-p)
    # Original scalar's exact graph second jets:
    # B_uu=B''+b'WB, B_uv=-WB, B_vv=WB/b'.
    Sg=S[0]+(S[0]*upper(bp)*W+S[1]*W+S[2]*upper(1/bp)*W+S[5])/(D0*D1)+S[3]/D1
    spin=k*f/d
    # L_m=-d_t^2+d_x^2-W_m+C*d_t; C=4 L is skew.
    g1t=(upper(px)*gu+2*upper(p)*gxu+4*upper(spin)*gu+Sg)/(1-p*p)+om*gu
    g1p=upper(1+p)*upper(g1t)/2+gxu/2+upper(spin)*gu
    g1q=upper(1-p)*upper(g1t)/2+gxu/2+upper(spin)*gu
    pg=upper(p0[0]+p0[2]/D0)
    qg=upper(q0[0]+q0[2]/D0)
    fg=(F[0][0]*pg+F[0][1]*qg+A0[0][0]+A0[0][2]/D0)/D1
    return (upper(gu/D2),upper(g1p/D2),upper(g1q/D2),
            pg/D1,qg/D1,upper(fg))


def directed_cap_phase(geo, front, width, refinement):
    """Independent fixed-u radial integral; use only correlated v=u+2x(r)."""
    start=arb(13)/8
    cut=arb(81)/50
    rc=arb(400)
    segments=[(cut,start,32,True),(start,arb(5)/2,768,False),
              (arb(5)/2,arb(8),1024,False),(arb(8),arb(25),512,False),
              (arb(25),arb(64),256,False),(arb(64),rc,256,False),
              (rc,rc+width/2,1,True)]
    radial=arb(0)
    for lo,hi,n,optional in segments:
        n*=refinement
        dr=(hi-lo)/n
        for j in range(n):
            r=(lo+j*dr).union(lo+(j+1)*dr)
            f=1-1/r
            (h,_,_),_,_=geo.evaluate(r)
            vv,vr,_,sphere=h
            q=-2*vv/f-vr+sphere/r**2
            cell=2*(2*geo.omega*x(r)).exp()*q*dr/f
            radial+=cell.union(arb(0)) if optional else cell
    phase=(geo.omega*front.union(front+width)).exp()*radial
    return phase*phase/4


def enclosure(refinement):
    ctx.prec,ctx.cap=160,80
    geo=GeometryEnclosure(refinement=refinement)
    start=arb(13)/8
    front=start-2*x(start)
    width=arb(3)/200
    d=front+width
    cut=arb(81)/50
    assert cut-2*x(cut)>d
    xg,pgc,qgc,pg,qg,fg=graph(geo,cut.union(start))
    print("shifted mixed graph X,P,Q / h''",xg,pgc,qgc)
    print("physical uncommuted F graph/B''",fg)
    ej=[arb(z) for z in ("0.00720","0.00000301","0.000135","0.004565",
                         "0.000000054","0.00001202")]
    ej0=[arb("0.004147"),arb("0.000001124"),arb("0.000004740"),
         arb("1.003017")/D1,arb("0.0000008181")/D1,arb("1.003017")/(D0*D1)]
    configs=[(D1,ej0,arb(0),pg,qg),(D2,ej,xg,pgc,qgc)]
    eforce=[arb(0),arb(0)]
    eprop=[arb(0),arb(0)]
    FTerr=AF=AH=Gsup=arb(0)
    phase=arb(0)
    capF=capG=None
    panels=[(cut,arb(5)/2,384),(arb(5)/2,arb(8),768),
            (arb(8),arb(25),512),(arb(25),arb(64),256),
            (arb(64),arb(400),256)]
    for lo,hi,count in panels:
        count*=refinement
        for j in range(count):
            s0,s1=lo+(hi-lo)*j/count,lo+(hi-lo)*(j+1)/count
            v0,v1=d+2*x(s0),d+2*x(s1)
            dv=(v1-v0).upper()
            r,v=s0.union(s1+width/2),v0.union(v1)
            q,S,Y,B,M,_,_,_=coeffs(geo,r,v)
            step=q*(v1-v0)
            partial=step.union(arb(0))
            po=upper(phase+partial)/4
            wedge=v0<=start
            phase+=partial if wedge else step
            for i,(ratio,E,Xg,Pg,Qg) in enumerate(configs):
                x_in=Xg if wedge else arb(0)
                errS=overlap(S,E)
                eforce[i]=upper(eforce[i]+dv*errS/4)
                eprop[i]=upper(eprop[i]+dv*M*x_in)
                pc=upper((po+Pg+eforce[i]+eprop[i])*(M*dv/ratio).exp())
                eprop[i]=upper(eprop[i]+dv*M*pc/ratio)
                errp=Pg+eforce[i]+eprop[i]
                xc=x_in+pc/ratio
                qc=(Qg if wedge else 0)+(M*xc+(S[0]+errS)/4)/ratio
                if i==1:
                    FTerr+=dv*(Y[0][0]*errp+Y[0][1]*qc+Y[0][2]*xc+overlap(B[0],E[3:]))
                else:
                    vals=[Y[t][0]*pc+Y[t][1]*qc+Y[t][2]*xc+overlap(B[t],E[3:])
                          for t in range(3)]
                    AF+=dv*vals[0]
                    AH+=dv*vals[2]
                    Gsup=max(Gsup,upper(vals[1]))
                    capF,capG=vals[:2]
    chi2=(phase*phase/4).intersection(directed_cap_phase(geo,front,width,refinement))
    assert chi2>0
    print("chi_c^2, integral shifted F defect/h'', uncommuted F/H integrals and G sup/B''")
    print(chi2,FTerr,AF,AH,Gsup)
    return geo.omega,chi2,FTerr,AF,AH,Gsup,fg,capF,capG


def reference_mismatch(path, omega, chi2, pieces):
    with np.load(path) as dat:
        p=np.asarray(dat["p_cap"],dtype="<f8")
    digest=hashlib.sha256(p.tobytes()).hexdigest()
    assert digest==CAP_DIGEST and len(p)==193
    square=arb(0)
    for i,(l,r) in enumerate(zip(p,p[1:])):
        l,r=arb(float(l)),arb(float(r))
        for j in range(pieces):
            t=(arb(j)/pieces).union(arb(j+1)/pieces)
            _,_,B2,B3=bump_jets((i+t)/(8*(len(p)-1)),omega)
            ref=(B3+2*omega*B2)*chi2/4
            diff=ref-(l+(r-l)*t)
            square+=upper(diff)**2*(arb(3)/(200*(len(p)-1)*pieces))
    print("affine cap SHA256",digest)
    return square.sqrt()


def run(path,refinement,subcells):
    weights()
    om,chi2,et,af,ah,gsup,fg,fc,gc=enclosure(refinement)
    h2norm=derivative_norm().upper()
    B2=seed_bounds()[-1].sqrt().upper()
    width=arb(3)/200
    kap=2*width/arb.pi()
    M=arb("1.563294")
    beta=arb(26)/21
    normw=(kap*M).exp()*B2*(af+kap*(gsup+ah))/4
    cap=(fc+gc)*B2/4
    graph=beta*fg*B2/4
    source=et*h2norm/4
    H=ah*B2/4
    potential=M*normw
    ref=reference_mismatch(path,om,chi2,subcells)
    E=ref+cap+graph+source+H+potential
    for label,value in (("optical-to-affine E_read",ref),("cap boundary",cap),
                        ("graph boundary",graph),("one-current bulk time defect",source),
                        ("H term",H),("potential times shear",potential),
                        ("E_c bound",E)):
        print(label,upper(value).str(18))
    print("E_c<=400:",E<400)
    print("E_c<=570:",E<570)
    return E


if __name__=="__main__":
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate")
    parser.add_argument("--refinement",type=int,default=1)
    parser.add_argument("--subcells",type=int,default=32)
    opt=parser.parse_args()
    run(opt.candidate,opt.refinement,opt.subcells)
