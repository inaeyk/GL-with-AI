"""Validated compact real affine datum for the physical shifted massive field."""
import argparse

from flint import acb,acb_mat,arb,ctx

from certified_launch import advance,bump,inputs,massless,spec
from enclosure_math import centered
from exterior_profiles import Cell,CompactGL,x
from exterior_source import Exterior,forcing,from_fields,matrix,norm2,validate_affine
from horizon_series import LocalHorizon,box,rw_horizon,upper
from outer_gljet import njet


PACKET_LOW=arb("1.595")
PACKET_HIGH=arb(13)/8


class IngoingScalar:
    def __init__(self,s,cells):
        start=cells[0].radius(acb(0))
        n=rw_horizon(s,40)[0].value(start-1)
        self.initial=n*(s*x(start)).exp()
        self.start=x(start)
        self.s=s
        self.cells=cells
        self.primitives=[]
        prefix=acb(0)
        for cell in cells:
            primitive,error=cell.log_primitive()
            self.primitives.append((prefix,primitive,error))
            prefix+=primitive(1)-primitive(0)+box(error)

    def evaluate(self,index,t):
        cell=self.cells[index]
        prefix,primitive,error=self.primitives[index]
        phase=self.s*(x(cell.radius(t))-self.start)
        logarithm=prefix+primitive(t)-primitive(0)+box(error*upper(t))
        return self.initial*(phase+logarithm).exp(),cell.slopes(t,self.s,acb(0))[0]


class CompactBaseline:
    def __init__(self,data,cells,ingoing,right_slope):
        self.data=data
        self.cells=cells
        self.ingoing=ingoing
        self.sigma=data.s-2*data.omega
        self.width=arb(3)/25
        self.u0=self.clock(acb(PACKET_HIGH)).real
        assert self.clock(acb(PACKET_LOW)).real-self.u0>self.width
        self.g_fields={}
        state=[acb(1),right_slope]
        for index in range(len(cells)-1,-1,-1):
            cell=cells[index]
            if cell.radius(acb(1)).real<PACKET_LOW:
                break
            local=advance(massless(self.sigma),cell.radius(acb(1)),-cell.step,state)
            self.g_fields[index]=local
            state=[q.value(-cell.step) for q in local]
        assert max(self.g_fields)==len(cells)-1
        self.moments={}
        prefix=acb(0)
        for index in sorted(self.g_fields):
            if cells[index].radius(acb(0)).real>=PACKET_HIGH:
                break
            weights=[self.integrand(index,acb((j+arb(".5",".5"))/16)) for j in range(16)]
            self.moments[index]=[prefix,acb(0),weights]
            prefix+=sum(v[0] for v in weights)/16
        suffix=acb(0)
        for index in reversed(self.moments):
            self.moments[index][1]=suffix
            suffix+=sum(v[1] for v in self.moments[index][2])/16
        assert (-prefix/data.baseline_w).overlaps(data.launch_right)
        assert (-suffix/data.baseline_w).overlaps(data.launch_left)

    @staticmethod
    def clock(r):
        return -r-2*(r-1).log()

    def launch(self,r):
        time=-(r-1).log()
        y=(self.clock(r).real-self.u0)/self.width
        return (-self.sigma*time).exp()*self.width.sqrt()*self.data.bump_normalization*bump(y)

    def right(self,index,t):
        return [q.value((t-1)*self.cells[index].step) for q in self.g_fields[index]]

    def integrand(self,index,t):
        cell=self.cells[index]
        r=cell.radius(t)
        h,h_slope=self.ingoing.evaluate(index,t)
        g,gx=self.right(index,t)
        weight=self.launch(r)*(1+1/r)*cell.dx(t)
        return weight*(h_slope+self.sigma)*h,weight*(gx+self.sigma*g)

    def fields(self,index,t):
        cell=self.cells[index]
        r=cell.radius(t)
        if index not in self.g_fields:
            h,p=self.ingoing.evaluate(index,t)
            return self.data.launch_left*h,p,0
        g,gx=self.right(index,t)
        if index not in self.moments:
            return self.data.launch_right*g,gx/g,0
        prefix,suffix,weights=self.moments[index]
        for j,(a,b) in enumerate(weights):
            pa=(16*t.real-j).min(arb(1)).max(arb(0))
            pb=1-pa
            prefix+=a*pa/16
            suffix+=b*pb/16
        h,p=self.ingoing.evaluate(index,t)
        launch=self.launch(r)
        psi=-(g*prefix+h*suffix)/self.data.baseline_w
        psix=-(gx*prefix+p*h*suffix)/self.data.baseline_w+launch
        return psi,psix,launch


def packet_coefficients(r,k,omega,s,p,psi,psix,launch):
    f=(r-1)/r
    h,_,_=njet(r,k,omega,1,(p-omega)/f)
    av=(f*psi+r*psix)/(2*r*r)
    ar=((s-2*omega)*psi/(2*r)-launch/(2*r)-av)/f
    return from_fields(r,k,s-omega,h,psi,av,ar)


class Ingoing:
    def __init__(self,data,right_slope,cells=4200):
        self.data=data
        self.s1=data.s-data.omega
        massive=[]
        scalar=[]
        spec.compact_bound(self.s1,True,False,cells,False,capture=massive)
        spec.compact_bound(data.s-2*data.omega,False,False,cells,False,capture=scalar)
        self.cells=[Cell(*v) for v in massive if v[0]>=1/32]
        scalar=[Cell(*v) for v in scalar if v[0]>=1/32]
        assert [(c.a,c.b) for c in scalar]==[(c.a,c.b) for c in self.cells]
        self.baseline=CompactBaseline(data,self.cells,IngoingScalar(data.s-2*data.omega,scalar),
                                      right_slope)
        self.gl=CompactGL(data,grid=[c.a for c in self.cells]+[self.cells[-1].b])
        self.local=LocalHorizon(data.s,data.k,data.omega,data.gl_scale,data.launch_left,48)
        self.start=self.cells[0].radius(acb(0))
        assert self.start.real<arb("1.04")
        self.slopes=matrix(self.cells[-1].slopes(acb(1),self.s1,acb(0)))

    def stationary_data(self,index,t):
        cell=self.cells[index]
        r=cell.radius(t)
        N,p=self.gl.evaluate(index,t)
        u=N*(self.data.omega*x(r)).exp()
        psi,psi_x,launch=self.baseline.fields(index,t)
        args=r,self.data.k,self.data.omega,self.data.s
        if index in self.baseline.moments:
            result=[u*v for v in centered(packet_coefficients,(*args,p,psi,psi_x,launch))]
            F,G=result[:2],result[2:]
        else:
            F,G=forcing(*args,u,p,psi,psi_x)
        P=matrix(cell.slopes(t,self.s1,acb(0)))
        return F,G,P

    def affine(self,subdivisions=2):
        part=self.local.initial(self.start)[2]
        P=matrix(self.cells[0].slopes(acb(0),self.s1,acb(0)))
        scale=(self.s1*x(self.start)).exp()
        initial=[scale*(part[i+2]+self.s1*part[i]-sum(P[i][j]*part[j] for j in range(2)))
                 for i in range(2)]
        return validate_affine(self.cells,self.stationary_data,initial,subdivisions)


def matching_enclosure(left,right,left_error,right_error,P_left,P_right):
    """Use the two Euclidean affine error bounds in the matched inverse."""
    D=acb_mat([[a-b for a,b in zip(l,r)] for l,r in zip(P_left,P_right)])
    determinant=D.det()
    assert determinant.abs_lower()>0

    def mid(z):
        return acb(z.real.mid(),z.imag.mid())

    identity=acb_mat([[1,0],[0,1]])
    inverse=acb_mat([[mid(D[i,j]) for j in range(2)] for i in range(2)]).solve(identity)
    inverse=acb_mat([[mid(inverse[i,j]) for j in range(2)] for i in range(2)])
    offset=acb_mat([[mid(r)-mid(l)] for l,r in zip(left,right)])
    center=inverse*offset
    center=acb_mat([[mid(center[i,0])] for i in range(2)])
    contraction=norm2((identity-inverse*D).entries())
    assert contraction<1
    residual=norm2((inverse*(offset-D*center)).entries())
    error=((residual+norm2(inverse.entries())*(left_error+right_error))/(1-contraction)).upper()
    return determinant,[center[i,0]+box(error) for i in range(2)],error,center


if __name__=="__main__":
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--subdivisions",type=int,default=2)
    args=parser.parse_args()
    assert args.subdivisions>0
    print("affine subdivisions per cell",args.subdivisions,flush=True)
    ctx.prec=128
    ctx.cap=80
    data=inputs()
    right=Exterior(data)
    left=Ingoing(data,right.baseline.evaluate(0,acb(1))[1])
    value_left,error,peak=left.affine(args.subdivisions)
    print("ingoing affine start r",left.start,"cells",len(left.cells),flush=True)
    print("ingoing affine match error norm upper",error,"peak",peak,flush=True)
    print("ingoing affine match enclosure",value_left,flush=True)
    value_right,right_error,tail,peak=right.affine(args.subdivisions)
    print("outgoing affine match error norm upper",right_error,"tail",tail,flush=True)
    print("outgoing affine match enclosure",value_right,flush=True)
    P_right=matrix(right.massive[-1].slopes(acb(1),right.ell,right.power))
    determinant,state,error,center=matching_enclosure(
        value_left,value_right,error,right_error,left.slopes,P_right)
    print("physical mixed determinant",determinant,flush=True)
    print("physical mixed state match center",center,flush=True)
    print("physical mixed state match Euclidean error upper",error,flush=True)
    print("physical mixed state at match",state,flush=True)
