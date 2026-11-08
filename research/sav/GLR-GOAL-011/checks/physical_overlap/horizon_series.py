"""Acb local EF power series for the ordered GLR-011 overlap.

The enclosure is conditional on its supplied matching amplitudes.
It bounds local coefficient/tail arithmetic, not the global ODE errors.
"""
from dataclasses import dataclass
from math import factorial

from flint import acb, acb_mat, acb_poly, acb_series, arb, ctx

from mean_series import ricci_from_jet

I = acb(0, 1)
LAMBDA = 4


def upper(value):
    return abs(value).upper()


def box(radius, center=0):
    return acb(arb(center, radius), arb(0, radius))


def coefficients(value, count):
    if isinstance(value, acb_series):
        assert value.prec >= count
        return [value[i] for i in range(count)]
    return [acb(value)] + [acb(0)]*(count-1)


def poly_at(series, t):
    return acb_poly(series.coeffs())(t)


def tail_bound(bound, radius, inside, order):
    ratio = inside/radius
    assert 0 <= ratio < 1
    return (bound*ratio**order/(1-ratio)).upper()


@dataclass
class AnalyticSeries:
    series: acb_series
    radius: arb
    norm: arb
    remainder: arb | None = None

    def tail(self,radius):
        if self.remainder is not None:
            assert 0 <= radius < self.radius
            return (self.remainder*(radius/self.radius)**self.series.prec).upper()
        return tail_bound(self.norm,self.radius,radius,self.series.prec)

    def sup(self, radius, derivative=0):
        series = self.series
        for _ in range(derivative):
            series = series.derivative()
        low = sum(upper(c)*radius**n for n,c in enumerate(series.coeffs()))
        outer = (self.radius+radius)/2 if derivative else radius
        tail = self.tail(outer)
        if derivative:
            tail *= factorial(derivative)/(outer-radius)**derivative
        return (low+tail).upper()

    def value(self, t):
        error = self.tail(upper(t))
        return poly_at(self.series,t)+box(error)


def norm_matrix(a):
    return max(sum(upper(a[i,j]) for j in range(a.ncols())).upper()
               for i in range(a.nrows()))


def norm_vector(a):
    return max(upper(a[i,0]) for i in range(a.nrows()))


def fuchsian(system, initial, nu, degree, rho, outer):
    """Enclose the regular solution of t y'=A(t)y+b(t), A0²=-nu A0."""
    size=len(initial)
    variable=acb_series([0,1],prec=degree+1)
    mat,force=system(variable)
    a_coeff=[[coefficients(mat[i][j],degree+1) for j in range(size)]
             for i in range(size)]
    b_coeff=[coefficients(force[i],degree+1) for i in range(size)]
    matrices=[acb_mat([[a_coeff[i][j][m] for j in range(size)]
                      for i in range(size)]) for m in range(degree+1)]
    a0=matrices[0]
    y=[acb_mat([[x] for x in initial])]
    leading=a0*y[0]+acb_mat([[row[0]] for row in b_coeff])
    identity=a0*a0+nu*a0
    assert all(leading[i,0].contains(0) for i in range(size))
    assert all(identity[i,j].contains(0) for i in range(size) for j in range(size))
    for n in range(1,degree+1):
        rhs=acb_mat([[row[n]] for row in b_coeff])
        for m in range(1,n+1):
            rhs+=matrices[m]*y[n-m]
        y.append((rhs+(a0*rhs)/(n+nu))/n)

    disk_a,disk_b=system(box(outer))
    m_a=norm_matrix(acb_mat(disk_a))
    m_b=norm_vector(acb_mat([[item] for item in disk_b]))
    a_tail=tail_bound(m_a,outer,rho,degree+1)
    b_tail=tail_bound(m_b,outer,rho,degree+1)
    rho_a=[norm_matrix(a)*rho**n for n,a in enumerate(matrices)]
    rho_y=[norm_vector(v)*rho**n for n,v in enumerate(y)]
    high_residual=b_tail+sum(
        yj*(sum(rho_a[degree+1-j:])+a_tail) for j,yj in enumerate(rho_y)
    )
    inverse=(1+rho_a[0]/(degree+1+nu.real.lower()))/(degree+1)
    contraction=(inverse*(sum(rho_a[1:])+a_tail)).upper()
    assert contraction < 1
    error=(inverse*high_residual/(1-contraction)).upper()
    solution=[]
    for j in range(size):
        norm=(sum(upper(v[j,0])*rho**n for n,v in enumerate(y))+error).upper()
        solution.append(AnalyticSeries(acb_series([v[j,0] for v in y],prec=degree+1),
                                       rho,norm,error))
    return solution,(contraction,error)


def rw_horizon(s,degree):
    def system(t):
        r=1+t
        return [[0,t],[(6*r-3)/r**2,-1/r-2*s*r]], [0,0]
    nu=1+2*s
    return fuchsian(system,[1,3/nu],nu,degree,arb(".14"),arb(".24"))[0]


def gl_regular(k,omega,scale,degree):
    def system(t):
        r=1+t
        d=1+k*k*r**3
        c=k*k*r**3+6*r-11+18*t/d**2-6*(4*r-5)/d
        return [[0,t],[c/r**2,-1/r-2*omega*r]], [0,0]
    nu=1+2*omega
    return fuchsian(system,[scale,scale*(k*k-5+6/(1+k*k))/nu],
                   nu,degree,arb(".14"),arb(".22"))[0]


def gl_horizon(k,omega,scale,degree):
    n,nr=gl_regular(k,omega,scale,degree)

    def numerators(t,nv,drv):
        r=1+t
        f=t/r
        a=k*k
        d=1+a*r**3
        y=d*nv/r
        yp=(2*a*r-1/r**2)*nv+d*drv/r
        b=t/(2*a*r**3*d)-3*t**2/d**2
        bf=1/(2*a*r**2*d)-3*r*t/d**2
        vv=(f*(1-2*a*r**3)/(2*d**2)-omega**2/(a*d))*y+b*yp+omega*bf*y
        ftr=-omega/(a*d)*(f*yp+(omega-1/(2*r*r))*y)
        angular=-r*r*y/(2*d)-t*yp/(a*d)-omega*r*y/(a*d)
        return [vv,r*(ftr-vv),2*r*r*(vv-ftr-f*angular/r**2),angular]

    t=acb_series([0,1],prec=degree+1)
    series=numerators(t,n.series,nr.series)
    radius=n.radius
    magnitude=numerators(box(radius),box(n.norm),box(nr.norm))
    result=[]
    for value, bound, drop in zip(series,magnitude,(0,1,2,0)):
        assert all(value[j].contains(0) for j in range(drop))
        shifted=acb_series([value[j] for j in range(drop,value.prec)],prec=value.prec-drop)
        result.append(AnalyticSeries(shifted,radius,(upper(bound)/radius**drop).upper()))
    return result


def matvec(a,x):
    return [sum(ai*xj for ai,xj in zip(row,x)) for row in a]


def matmul(a,b):
    return [matvec(list(zip(*b)),row) for row in a]


def scaled(scale,x):
    return [scale*v for v in x]


def inverse(r):
    return [[0,1],[1,1-1/r]]


def endomorphism(r,h):
    p,q,a,w=h
    return matmul(inverse(r),[[p,q],[q,a]]),w/r**2


class LocalHorizon:
    """Regular EF fields and their conditional local error enclosures."""

    def __init__(self,s,k,omega,gl_amplitude,base_amplitude,degree=34):
        self.s,self.k,self.omega=s,k,omega
        self.s1=s-omega
        self.sigma=s-2*omega
        self.base_amplitude=base_amplitude
        self.gl_amplitude=gl_amplitude
        self.degree=degree
        self.t=acb_series([0,1],prec=degree+1)
        self.gl=gl_horizon(k,omega,gl_amplitude,degree+6)
        self.base=rw_horizon(self.sigma,degree+6)
        self.test=rw_horizon(s,degree+6)[0]
        self.columns=[]
        self.diagnostics=[]
        for mode in ("first","second","particular"):
            state,stats=self.solve_column(mode)
            self.columns.append(state)
            self.diagnostics.append(stats)

    def lower(self,t,radius=None):
        r=1+t
        f=t/r
        if radius is None:
            h=[g.series for g in self.gl]
            n,dn=[q.series for q in self.base]
        else:
            h=[box(g.sup(radius)) for g in self.gl]
            n,dn=[box(q.sup(radius)) for q in self.base]
        psi=self.base_amplitude*n
        psi_r=self.base_amplitude*dn
        a0=[(f*psi+r*(self.sigma*psi+f*psi_r))/(2*r**2),
            -psi/(2*r**2)-psi_r/(2*r)]
        x,sphere=endomorphism(r,h)
        return x,sphere,a0,2*r*psi

    def mixed_source(self,t,radius=None):
        r=1+t
        x,sphere,a0,q0=self.lower(t,radius)
        y=scaled(-LAMBDA*r*r,matvec(x,matvec(inverse(r),a0)))
        return 3*sphere*q0,*y

    def solve_column(self,mode):
        s,k=self.s1,self.k
        outer=arb(".075")
        forced=mode=="particular"

        def system(t):
            r=1+t
            ik=I*k
            middle=-ik*s*r**5
            b=-ik*s/(LAMBDA*r)
            diag=-s*r
            a=[[diag,middle,-r**3*(LAMBDA+k*k*r*r),0],
               [b,diag,0,1/r**3+k*k/(LAMBDA*r)],
               [-s*s/(LAMBDA*r)-t/r**4,0,diag,b],
               [0,s*s*r**5+LAMBDA*t*r*r,middle,diag]]
            if not forced:
                return a,[0,0,0,0]
            if isinstance(t,acb_series):
                xx,yv,yr=self.mixed_source(t)
            else:
                xx,yv,yr=self.mixed_source(t,outer)
            return a,[t*yv-r*yr,-ik*yr/(LAMBDA*r),
                      -s*yr/(LAMBDA*r)-t*xx/r**4,0]

        d=LAMBDA+k*k
        u0=d.sqrt() if mode=="first" else acb(0)
        z0=d.sqrt()/2 if mode=="second" else acb(0)
        yr0=self.mixed_source(self.t)[2][0] if forced else acb(0)
        a0=(-s*u0-I*k*s*z0-yr0)/d
        j0=s*z0-I*k*a0
        return fuchsian(system,[u0,z0,a0,j0],2*s,self.degree,arb(".043"),outer)

    def initial(self,radius):
        """Two regular homogeneous columns and the driven column in solver variables."""
        t=acb(radius)-1
        r=1+t
        f=t/r
        d=LAMBDA+self.k**2*r*r
        root_d=r*d.sqrt()
        root_w=2*r*r/d.sqrt()
        a=f*(1/r+self.k**2*r/d)
        b=f*(2/r-self.k**2*r/d)
        gamma=I*self.k*r/2
        columns=[]
        for column in self.columns:
            u,z,at,bflux=[q.value(t) for q in column]
            y=u/root_d
            zz=root_w*z
            jy=-root_d*at-a*y-gamma*self.s1*zz
            jz=bflux/root_w+b*zz-gamma*self.s1*y
            columns.append([y,zz,jy-self.s1*y,jz-self.s1*zz])
        return columns

    def matched(self,amplitudes):
        output=[]
        for j in range(4):
            values=[col[j] for col in self.columns]
            series=sum(a*q.series for a,q in zip(amplitudes,values))
            norm=sum(upper(a)*q.norm for a,q in zip(amplitudes,values)).upper()
            output.append(AnalyticSeries(series,values[0].radius,norm))
        return output

    def mixed_radial(self,t,state,radius=None,driven=True):
        r=1+t
        if radius is None:
            u,z,a,j=[v.series for v in state]
            xx,_,yr=self.mixed_source(t) if driven else (0,0,0)
        else:
            u,z,a,j=[box(v.sup(radius)) for v in state]
            xx,_,yr=self.mixed_source(t,radius) if driven else (0,0,0)
        numerator=r*(-self.s1*u-I*self.k*j-yr-LAMBDA*r*r*a)/(LAMBDA*r*r)
        if radius is None:
            assert numerator[0].contains(0)
            radial=acb_series([numerator[i] for i in range(1,numerator.prec)],
                              prec=numerator.prec-1)
        else:
            radial=box(upper(numerator)/radius)
        return [a,radial],(u+xx)/r**4

    def mean_rhs(self,t,radius=None):
        r=1+t
        if radius is None:
            h=[v.series for v in self.gl]
            dh=[v.derivative() for v in h]
            d2h=[v.derivative() for v in dh]
        else:
            h,dh,d2h=([box(q.sup(radius,j)) for q in self.gl] for j in range(3))
        rr,angular,zz=ricci_from_jet(r,self.omega,self.k,h,dh,d2h)
        return [-r*r*((1-1/r)*rr/2+angular/r**2+zz/2)/2,-r*rr/2]

    def mean(self,t_match,values):
        radius=arb(".04")
        coeff=self.mean_rhs(self.t)
        bounds=self.mean_rhs(box(radius),radius)
        results=[]
        for derivative,sup,value in zip(coeff,bounds,values):
            primitive=AnalyticSeries(derivative.integral(),radius,(radius*upper(sup)).upper())
            horizon=value-primitive.value(t_match)
            results.append(AnalyticSeries(horizon+primitive.series,radius,
                                         (upper(horizon)+primitive.norm).upper()))
        return results

    def returning_currents(self,t,state,mean,radius=None,driven=True):
        r=1+t
        f=t/r
        x,sphere,a0,q0=self.lower(t,radius)
        a1,f1=self.mixed_radial(t,state,radius,driven)
        if radius is None:
            mu,beta=[v.series for v in mean]
        else:
            mu,beta=[box(v.sup(radius)) for v in mean]
        metric=matmul(inverse(r),[[2*mu/r-2*f*beta,beta],[beta,0]])
        x2=matmul(x,x)
        trace=x2[0][0]+x2[1][1]
        zq=[[(x2[i][j]-(trace/4+sphere*sphere/2)*(i==j))/2
             for j in range(2)] for i in range(2)]
        zb=[[beta*(i==j)-metric[i][j] for j in range(2)] for i in range(2)]
        base=matvec(inverse(r),a0)
        first=matvec(inverse(r),a1)
        d=[scaled(-LAMBDA/2,matvec(x,first)),scaled(LAMBDA,matvec(zq,base)),
           scaled(LAMBDA,matvec(zb,base))]
        curls=[-3*sphere*r**4*f1/2,(trace/4+7*sphere*sphere/2)*q0/2,-beta*q0]
        # d already stores D divided by r^2.
        return [[rr-f*vv,vv,cc] for (vv,rr),cc in zip(d,curls)]

    def contribution(self,cap,state,mean,driven=True):
        """Exact strong collar integral plus its weak-form split boundary."""
        t=self.t
        r=1+t
        s=self.s
        raw=self.returning_currents(t,state,mean,driven=driven)
        r_k,r_g=(arb(".03"),arb(".018")) if cap<arb(".018") else (arb(".038"),arb(".034"))
        assert cap<r_g
        bound_k=self.returning_currents(box(r_k),state,mean,r_k,driven)
        bound_g=self.returning_currents(box(r_g),state,mean,r_g,driven)
        weight=r*self.test.series*(2*s*r).exp()*2/LAMBDA
        norm_weight=2/LAMBDA*(1+r_g)*self.test.sup(r_g)*(
            2*s.real+2*upper(s)*r_g).exp()
        cap_complex=cap**(1+2*s)
        values=[]
        diagnostics=[]
        for row, k_bound, g_bound in zip(raw,bound_k,bound_g):
            k,v,c=row
            g=weight*(k.derivative()-s*v-LAMBDA*c/r**4)
            integral=sum(q*cap_complex*cap**n/(n+1+2*s) for n,q in enumerate(g.coeffs()))
            norm_g=norm_weight*(upper(k_bound[0])/(r_k-r_g)+upper(s)*upper(g_bound[1])
                                +LAMBDA*upper(g_bound[2])/(1-r_g)**4)
            error=tail_bound(norm_g,r_g,cap,g.prec)*upper(cap_complex)/(
                g.prec+1+2*s.real.lower())
            series_boundary=weight*k
            boundary=poly_at(series_boundary,cap)*cap**(2*s)
            norm_boundary=norm_weight*upper(g_bound[0])
            boundary_error=tail_bound(norm_boundary,r_g,cap,series_boundary.prec)*upper(cap**(2*s))
            values.append(integral-boundary+box((error+boundary_error).upper()))
            diagnostics.append((error+boundary_error).upper())
        return values,diagnostics


    def factorized_contribution(self,cap,g,base,mixed,mean_at_cap):
        """Evaluate the exact amplitude polynomial using unit source inputs.

        Invoke on a LocalHorizon built with both lower amplitudes equal to one.
        Mixed homogeneous amplitudes use the same normalized horizon basis.
        """
        assert self.base_amplitude == self.gl_amplitude == 1
        def constants(*values):
            return [AnalyticSeries(acb_series([v],prec=self.degree+1),
                                   arb(".04"),upper(acb(v))) for v in values]

        zero_state=constants(0,0,0,0)
        zero_mean=constants(0,0)
        driven_mean=self.mean(cap,[acb(0),acb(0)])
        driven,_=self.contribution(cap,self.columns[2],driven_mean)
        h1=self.contribution(cap,self.columns[0],zero_mean,driven=False)[0][0]
        h2=self.contribution(cap,self.columns[1],zero_mean,driven=False)[0][0]
        mu=self.contribution(cap,zero_state,constants(1,0),driven=False)[0][2]
        beta=self.contribution(cap,zero_state,constants(0,1),driven=False)[0][2]
        common=g*g*base
        return [g*(mixed[0]*h1+mixed[1]*h2)+common*driven[0],
                common*driven[1],common*driven[2]+base*(mean_at_cap[0]*mu+mean_at_cap[1]*beta)]
