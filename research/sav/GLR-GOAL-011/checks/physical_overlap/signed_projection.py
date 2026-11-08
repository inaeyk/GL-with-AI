"""Finite-contour adjoint enclosure of the physical signed returning numerator."""
import argparse
from dataclasses import dataclass

from flint import acb,acb_mat,acb_series,arb,ctx

from certified_launch import spec
from tight_launch import inputs
from directed_vector import advance,directed_vector,indices,local_t
from denominator_refinement import wronskian_derivative
from enclosure_math import centered
from exterior_profiles import Cell,x
from exterior_source import Exterior,matrix,norm2,numerical_abscissa,forcing_coefficients
from horizon_series import AnalyticSeries,LocalHorizon,box,matmul,matvec,upper
from ingoing_source import Ingoing,IngoingScalar
from mean_certificate import factored_mean,mean_rhs,mean_tail
from outer_gljet import njet
from return_tail import returning_tail
from return_coefficients import (algebraic_coefficients,dot,homogeneous_coefficients,
                                 mixed_coefficients,transpose)


I=acb(0,1)
CAP=13/512
DEGREE=80


def midpoint(z):
    return acb(z.real.mid(),z.imag.mid())


def binary_point(z):
    return acb(complex(float(z.real.mid()),float(z.imag.mid())))


def vector_multiply(r,a):
    return [r*v for v in a]


def split(values,*points):
    return tuple(values[a:b] for a,b in zip((0,*points),(*points,len(values))))


def solve(a,b):
    return acb_mat(a).solve(acb_mat([[v] for v in b])).entries()


@dataclass
class WeakCell:
    P: list
    F: list
    G: list
    a: list
    b: list
    c: acb
    second: acb
    mean_weight: list
    mean_derivative: list


class Leg:
    def __init__(self,side,data,solver,test):
        self.side,self.data,self.solver,self.test=side,data,solver,test
        self.cells=solver.cells if side=="left" else solver.massive
        self.cache={}
        self.z=None
        self.w=None

    def evaluate(self,index,t,slot):
        key=(index,slot,None if slot is not None else int(t.real==1))
        if key not in self.cache:
            self.cache[key]=self.fields(index,t)
        return self.cache[key]

    def profiles(self,index,t):
        d=self.data
        s,omega,k=d.s,d.omega,d.k
        s1=s-omega
        r=self.cells[index].radius(t)
        if self.side=="left":
            n,p=self.solver.gl.evaluate(index,t)
            u=n*(omega*x(r)).exp()
            psi,p_psi,launch=self.solver.baseline.fields(index,t)
            h,p_h=self.test.evaluate(index,t)
            packet=index in self.solver.baseline.moments
        else:
            j=len(self.cells)-1-index
            u,p=self.solver.gl.evaluate(j,t)
            n=u*(-omega*x(r)).exp()
            psi,p_psi=self.solver.baseline.evaluate(j,t)
            h,p_h=self.test.evaluate(j,t)
            packet=False
            launch=acb(0)
        if self.side=="left":
            P=matrix(self.cells[index].slopes(t,s1,acb(0)))
        else:
            P=matrix(self.cells[index].slopes(t,self.solver.ell,self.solver.power))
        return r,n,u,p,psi,p_psi,launch,h,p_h,P,packet

    def fields(self,index,t):
        d=self.data
        s,omega,k=d.s,d.omega,d.k
        s1=s-omega
        r,n,u,p,psi,p_psi,launch,h,p_h,P,packet=self.profiles(index,t)
        F,G,_=self.solver.stationary_data(index,t)
        args=r,k,omega,s,p,p_h
        coeff=centered(mixed_coefficients,args)
        alpha,beta=coeff[:2],coeff[2:4]
        R=[[P[i][j]-s1*(i==j) for j in range(2)] for i in range(2)]
        a=[u*h*(alpha[i]+dot(beta,transpose(R)[i])) for i in range(2)]
        b=vector_multiply(u*h,beta)
        y_x=F[1]*2*(4+k*k*r*r).sqrt()/(-I*k)
        c=u*h*(coeff[4]*y_x+coeff[5]*u*psi)
        if packet:
            second,mu,beta=centered(algebraic_coefficients,(*args,psi,p_psi,launch))
        else:
            second,mu,beta=vector_multiply(psi,centered(homogeneous_coefficients,(*args,p_psi)))
        mean_weight=vector_multiply(h*(2*omega*x(r)).exp(),[mu,beta])
        return WeakCell(P,F,G,a,b,c,u*u*h*second,mean_weight,
                        factored_mean(r,k,omega,n,p))

    def z_coeff(self,index,t,slot):
        field=self.evaluate(index,t,slot)
        return [vector_multiply(-1,row) for row in transpose(field.P)],vector_multiply(-1,field.a)

    def w_coeff(self,index,t,slot):
        field=self.evaluate(index,t,slot)
        if slot is None:
            j=0 if t.real==0 else self.subdivisions-1
        else:
            j=slot
        z=self.z[index].value(t,j)
        return transpose(field.P),[-v-w for v,w in zip(z,field.b)]

    def z_flow(self,initial,subdivisions):
        self.subdivisions=subdivisions
        result,error,self.z=directed_vector(self.cells,self.z_coeff,initial,subdivisions,candidate_only=True)
        return result,error

    def w_flow(self,initial):
        result,error,self.w=directed_vector(self.cells,self.w_coeff,initial,self.subdivisions,True,True)
        return result,error

    def affine_envelope(self,initial):
        """Piecewise absolute L envelope using the validated affine candidate."""
        def coefficients(index,t,slot):
            d=self.evaluate(index,t,slot)
            return [vector_multiply(-1,row) for row in d.P], [g-p for g,p in zip(d.G,matvec(d.P,d.F))]
        _,_,pieces=directed_vector(self.cells,coefficients,initial,self.subdivisions)
        return {(i,j): norm2(piece.value(local_t(j,self.subdivisions),j))
                for i,piece in pieces.items() for j in range(self.subdivisions)}

    def state_envelope(self,match_norm,affine):
        """Bound U from the match to the other endpoint in either direction."""
        m=self.subdivisions
        U=match_norm
        bounds={}
        for i in reversed(range(len(self.cells))):
            cell=self.cells[i]
            for j in reversed(range(m)):
                t=local_t(j,m)
                d=self.evaluate(i,t,j)
                dx=cell.dx(t)/m
                next_U=advance(U,numerical_abscissa(d.P,dx),upper(dx)*(affine[i,j]+norm2(d.F)))
                bounds[i,j]=max(U,next_U)
                U=next_U
        return bounds,U

    def series_fields(self,i,t,degree):
        """Jets with rigorous values and derivatives outside the launch support."""
        r,n,u,p,psi,p_psi,launch,h,p_h,P,packet=self.profiles(i,t)
        assert not packet
        d=self.data
        cell=self.cells[i]
        z=acb_series([r,cell.step],prec=degree)
        gamma=cell.step*z/(z-1)
        state=[[v] for v in [u,p,psi,p_psi,h,p_h]+sum(P,[])]
        for j in range(1,degree):
            U,p,S,q,H,v,*rest=[acb_series(c,prec=degree) for c in state]
            A=matrix(rest)
            f=(z-1)/z
            dd=1+d.k*d.k*z**3
            q_gl=d.omega*d.omega+f*(d.k*d.k+(6*z-11)/z**3+18*(z-1)/(z**3*dd**2)-6*(4*z-5)/(z**3*dd))
            v_rw=f*(6/z**2-3/z**3)
            s1=d.s-d.omega
            den,Q,_=spec.coefficients(z,z-1,s1,True,False,d.k,s1,acb(0))
            square=sum(matmul(A,A),[])
            rhs=[p*U,q_gl-p*p,q*S,(d.s-2*d.omega)**2+v_rw-q*q,
                 v*H,d.s*d.s+v_rw-v*v]
            rhs+=[V/den+s1*s1*(k in (0,3))-W for k,(V,W) in enumerate(zip(Q,square))]
            state=[c+[(gamma*b)[j-1]/j] for c,b in zip(state,rhs)]
        U,p,S,q,H,v,*rest=[acb_series(c,prec=degree) for c in state]
        A=matrix(rest)
        n=U*(-d.omega*x(z)).exp()
        f=(z-1)/z
        F,G=split(vector_multiply(U*S,forcing_coefficients(z,d.k,d.omega,d.s,p,q)),2)
        alpha,beta,coeff=split(mixed_coefficients(z,d.k,d.omega,d.s,p,v),2,4)
        R=[[A[i][j]-(d.s-d.omega)*(i==j) for j in range(2)] for i in range(2)]
        a=[U*H*(u+dot(beta,w)) for u,w in zip(alpha,transpose(R))]
        b=vector_multiply(U*H,beta)
        y_x=F[1]*2*(4+d.k*d.k*z*z).sqrt()/(-I*d.k)
        c=U*H*(coeff[0]*y_x+coeff[1]*U*S)
        second,mu,beta=vector_multiply(S,homogeneous_coefficients(z,d.k,d.omega,d.s,p,v,q))
        mean_weight=vector_multiply(H*(2*d.omega*x(z)).exp(),[mu,beta])
        derivative=mean_rhs(z,d.k,d.omega,njet(z,d.k,d.omega,n,(p-d.omega)*n/f))
        return WeakCell(A,F,G,a,b,c,U*U*H*second,mean_weight,derivative)

    def midpoint_data(self,i,j,mean,direction):
        m=self.subdivisions
        center=acb(arb(2*j+1)/(2*m))
        t=local_t(j,m)
        step=self.cells[i].step
        field=self.evaluate(i,t,j)
        mid=self.fields(i,center)
        # Midpoint mean needs only a first-derivative enclosure.
        mid_mean=[q+direction*step*v/(2*m) for q,v in zip(mean,field.mean_derivative)]
        mean_interval=[q+direction*step*arb(".5",".5")*v/m for q,v in zip(mean,field.mean_derivative)]
        # Obtain exact derivative bounds by Taylor differentiation of the ODE,
        # with constant coefficients ranging over the whole subcell.
        jets=self.series_fields(i,t,3)
        mean_jets=[acb_series([v,step*d[0],step*d[1]/2],prec=3)
                   for v,d in zip(mean_interval,jets.mean_derivative)]
        return center,mid,mid_mean,jets,mean_jets

    def residuals(self,i,t,field,z,w):
        cell=self.cells[i]
        dz=self.z[i].jet(t,2)
        dw=self.w[i].jet(t,2)
        dx=cell.dx(t)
        zdot=[a[1] for a in dz]
        wdot=[a[1] for a in dw]
        P=transpose(field.P)
        return ([a+dx*(b+c) for a,b,c in zip(zdot,matvec(P,z),field.a)],
                [a+dx*(-b+c+d) for a,b,c,d in zip(wdot,matvec(P,w),z,field.b)])

    def residual_derivative(self,i,t,jets):
        r=self.cells[i].radius(t)
        dr=self.cells[i].step
        z=self.z[i].jet(t,3)
        w=self.w[i].jet(t,3)
        dx=dr*acb_series([r,dr],prec=3)/(acb_series([r,dr],prec=3)-1)
        P=transpose(jets.P)
        return ([2*a[2]+(dx*(b+c))[1] for a,b,c in zip(z,matvec(P,z),jets.a)],
                [2*a[2]+(dx*(-b+c+d))[1] for a,b,c,d in zip(w,matvec(P,w),z,jets.b)])

    def integrands(self,i,t,d,mean):
        z=self.z[i].jet(t,3)
        w=self.w[i].jet(t,3)
        H=[g-p for g,p in zip(d.G,matvec(d.P,d.F))]
        return [dot(z,d.F)+dot(w,H)+d.c,d.second,dot(d.mean_weight,mean)]

    def project(self,mean0,U,L):
        """Pair point adjoint candidates and certify their residual terms."""
        mean=list(mean0)
        total=[acb(0),acb(0),acb(0)]
        defect=arb(0)
        reverse=self.side=="left"
        direction=-1 if reverse else 1
        sign=1 if reverse else -1
        m=self.subdivisions
        for i in indices(len(self.cells),reverse):
            cell=self.cells[i]
            for j in indices(m,reverse):
                t=local_t(j,m)
                field=self.evaluate(i,t,j)
                step=cell.step
                if self.side=="left" and i in self.solver.baseline.moments:
                    z=self.z[i].value(t,j)
                    w=self.w[i].value(t,j)
                    dz,dw=self.residuals(i,t,field,z,w)
                    derivative=field.mean_derivative
                    inside=[q+direction*step*arb(".5",".5")*v/m for q,v in zip(mean,derivative)]
                    H=[g-p for g,p in zip(field.G,matvec(field.P,field.F))]
                    integral=vector_multiply(sign*cell.dx(t)/m,
                              [dot(z,field.F)+dot(w,H)+field.c,field.second,
                               dot(field.mean_weight,inside)])
                    delta=vector_multiply(direction*step/m,derivative)
                else:
                    center,mid,mid_mean,jets,mean_jets=self.midpoint_data(i,j,mean,direction)
                    z=self.z[i].value(center,j)
                    w=self.w[i].value(center,j)
                    dz,dw=self.residuals(i,center,mid,z,w)
                    ddz,ddw=self.residual_derivative(i,t,jets)
                    dz=[q+arb(0,arb(1)/(2*m))*v for q,v in zip(dz,ddz)]
                    dw=[q+arb(0,arb(1)/(2*m))*v for q,v in zip(dw,ddw)]
                    r=acb_series([cell.radius(t),step],prec=3)
                    gamma=step*r/(r-1)
                    series=vector_multiply(gamma,self.integrands(i,t,jets,mean_jets))
                    H=[g-p for g,p in zip(mid.G,matvec(mid.P,mid.F))]
                    mid_values=[dot(z,mid.F)+dot(w,H)+mid.c,mid.second,
                                dot(mid.mean_weight,mid_mean)]
                    integral=[sign*cell.dx(center)*q/m+box(upper(v[2])/(12*m**3))
                              for q,v in zip(mid_values,series)]
                    delta=[direction*step*q/m+box(upper(step*v[2])/(12*m**3))
                           for q,v in zip(mid.mean_derivative,jets.mean_derivative)]
                defect+=(norm2(dz)*U[i,j]+norm2(dw)*L[i,j])/m
                total=[v+q for v,q in zip(total,integral)]
                mean=[q+v for q,v in zip(mean,delta)]
        return total,mean,defect.upper()


def horizon_affine(model,cap):
    """Convert the strong collar HJ linear functional to an endpoint covector."""
    zero_mean=model.mean(cap,[acb(0),acb(0)])
    cols=model.initial(1+cap)
    # U and J-s1 U for each regular solution at the cut.
    scale=(model.s1*x(1+cap)).exp()
    cols=[vector_multiply(scale,c) for c in cols]
    rows=[]
    for j in range(2):
        result,_=model.contribution(cap,model.columns[j],zero_mean,driven=False)
        rows.append(result[0])
    z0=solve([c[:2] for c in cols[:2]],[-v for v in rows])
    result,_=model.contribution(cap,model.columns[2],zero_mean)
    constant=result[0]+dot(z0,cols[2][:2])
    return z0,constant,cols[2],result[1]


def horizon_mean(unit,cap,g,base,mean):
    """Factor physical lower amplitudes out of the mean collar."""
    zero=[AnalyticSeries(unit.t*0,arb(".04"),arb(0)) for _ in range(4)]
    values=[]
    for mu,beta in ((0,0),(1,0),(0,1)):
        profile=unit.mean(cap,[acb(mu),acb(beta)])
        result,_=unit.contribution(cap,zero,profile,driven=False)
        values.append(result[2])
    return base*(g*g*values[0]+mean[0]*(values[1]-values[0])+mean[1]*(values[2]-values[0]))


def run(subdivisions,input_builder=inputs):
    data=input_builder()
    outer=Exterior(data)
    left=Ingoing(data,outer.baseline.evaluate(0,acb(1))[1],start=CAP)
    captured=[]
    spec.compact_bound(data.s,False,False,4200,False,capture=captured)
    cells=[Cell(*v) for v in captured if v[0]>=CAP]
    assert [(c.a,c.b) for c in cells]==[(c.a,c.b) for c in left.cells]
    l=Leg("left",data,left,IngoingScalar(data.s,cells))
    r=Leg("right",data,outer,outer.test)
    cap=acb(left.cells[0].a)
    unit=LocalHorizon(data.s,data.k,data.omega,acb(1),acb(1),DEGREE)
    z_exact,C,part,second=horizon_affine(unit,cap.real)
    g,b=data.gl_scale,data.launch_left
    z_exact=vector_multiply(g,z_exact)
    z0=[binary_point(v) for v in z_exact]
    part=vector_multiply(g*b,part)
    C,second=g*g*b*C,g*g*b*second
    print("collar start",cap,"z_L(c)",z_exact,"HJ affine constant",C,"second",second,flush=True)
    zl,_=l.z_flow(z0,subdivisions)
    zr,_=r.z_flow([acb(0),acb(0)],subdivisions)
    diff=[v-w for v,w in zip(zr,zl)]
    pr=matrix(outer.massive[-1].slopes(acb(1),outer.ell,outer.power))
    D=[[p-q for p,q in zip(v,w)] for v,w in zip(left.slopes,pr)]
    dt=transpose(D)
    q=[binary_point(v) for v in solve([[midpoint(v) for v in row] for row in dt],
                                 [-midpoint(v) for v in diff])]
    mismatch=[v+w for v,w in zip(diff,matvec(dt,q))]
    # Refined eight-subdivision ball from checks/exterior_source.txt, same inputs.
    # The 8e-6 upward slack also covers the displayed center rounding.
    center=[acb(-.18746108862784384,-.41681210448350057),
            acb(-.0588905954473658,-.49465461340480154)]
    match_error=arb(".33257")
    match_norm=norm2(center)+match_error
    defect=dot(mismatch,center)+box(norm2(mismatch)*match_error)
    print("common w match center",q,"match correction",defect,flush=True)
    wl,_=l.w_flow(q)
    wr,_=r.w_flow(q)
    P_cap=matrix(left.cells[0].slopes(acb(0),left.s1,acb(0)))
    L0=[part[i+2]+left.s1*part[i]-dot(P_cap[i],part[:2]) for i in range(2)]
    far_affine=outer.tail.affine_bound(outer.gl.far()[0],outer.baseline.far()[0])
    L_left=l.affine_envelope(L0)
    L_right=r.affine_envelope([box(far_affine),box(far_affine)])
    U_left,cap_state=l.state_envelope(match_norm,L_left)
    U_right,far_state=r.state_envelope(match_norm,L_right)
    print("state bounds Ucap,Ufar,max Lleft,max Lright",cap_state,far_state,
          max(L_left.values()),max(L_right.values()),flush=True)
    collar_mismatch=norm2([a-b for a,b in zip(z0,z_exact)])*cap_state
    far_mean=mean_tail(outer)
    right_result,mean_match,defect_right=r.project([box(far_mean),box(far_mean)],U_right,L_right)
    left_result,mean_cap,defect_left=l.project([acb(v.real) for v in mean_match],U_left,L_left)
    # w_L(c)L_L(c) and -w_R(64)L_R(64) are the endpoints after integrating L.
    endpoint=C+dot(wl,L0)+defect+box(norm2(wr)*far_affine+collar_mismatch)
    collar_mean=horizon_mean(unit,cap.real,data.gl_scale,data.launch_left,
                            [acb(v.real) for v in mean_cap])
    local=[endpoint,second,collar_mean]
    caps=returning_tail(outer.gl.far()[0],outer.baseline.far()[0],outer.test.far()[0],
                        far_state,outer.tail)
    totals=[a+b+c+box(e) for a,b,c,e in zip(local,left_result,right_result,
                        (defect_left+defect_right+caps.hj,caps.quadratic,caps.mean))]
    print("mean cap",mean_cap,"complete returning tail upper",caps.total,flush=True)
    print("mixed endpoint plus collar",local,flush=True)
    print("left projected",left_result,flush=True)
    print("right projected",right_result,flush=True)
    print("mixed defect caps left/right",defect_left,defect_right,flush=True)
    numerator=sum(totals)
    print("full projected pieces",totals,"sum",numerator,flush=True)
    print("numerator rectangle",[(v.lower(),v.upper()) for v in (numerator.real,numerator.imag)],flush=True)
    assert numerator.abs_lower()>0
    W,*_=wronskian_derivative()
    value=numerator/W
    print("denominator W'",W,flush=True)
    print("physical residue rectangle",[(v.lower(),v.upper()) for v in (value.real,value.imag)],flush=True)
    print("residue modulus endpoints",value.abs_lower(),value.abs_upper(),flush=True)
    return totals


if __name__=="__main__":
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument("--subdivisions",type=int,default=2)
    args=p.parse_args()
    assert args.subdivisions>0
    ctx.prec=128
    ctx.cap=128
    run(args.subdivisions)
