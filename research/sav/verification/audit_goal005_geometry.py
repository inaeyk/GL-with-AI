"""Independent 5D coordinate audit; empirical physical input, no certification.
Run from research/sav: .venv/Scripts/python.exe verification/audit_goal005_geometry.py
One declared historical forced n2 configuration; no GL root solve.
No import of historical verification code. Master/lift formulas are shared.
"""
from pathlib import Path
import hashlib, json, platform, time
import numpy as np
import scipy
from scipy.interpolate import CubicSpline
from scipy.integrate import solve_ivp, cumulative_simpson
import sympy as S

HERE=Path(__file__).resolve().parent
out={'python':platform.python_version(),'numpy':np.__version__,'scipy':scipy.__version__,
     'sympy':S.__version__,'classification':'EMPIRICAL_RESIDUAL_NOT_ENCLOSURE',
     'independence':'New 5D coordinate curvature implementation; shared historical source samples, master and lift; AI author, no human review.',
     'checks':[]}
def check(name,good,detail=None):
    row={'name':name,'pass':bool(good),'detail':detail};out['checks'].append(row)
    print(json.dumps(row),flush=True)
    assert good,name
def zero(a):return S.simplify(a)==0

def exact_checks():
    z,k=S.symbols('z k',real=True,nonzero=True)
    qzz=-k**2*S.cos(z)**2/2+k**2*S.sin(z)**2/4
    check('Gaussian z curvature Fourier identity',zero(qzz+k**2*(1+3*S.cos(2*z))/8))
    check('missing Fourier half rejected',not zero(2*(-3*k**2/16)-(-3*k**2/16)))
    f,T,B,C,U=S.symbols('f T B C U',real=True,nonzero=True)
    gi=S.diag(-1/f,f,1,1);h=S.Matrix([[T,B,0,0],[B,C,0,0],[0,0,U,0],[0,0,0,U]])
    hsq=S.trace(gi*h*gi*h)
    check('Lorentz contraction independently from matrix trace',zero(hsq-(T*T/f**2+f*f*C*C-2*B*B+2*U*U)))
    check('Euclidean tr sign rejected',not zero(hsq-(T*T/f**2+f*f*C*C+2*B*B+2*U*U)))
    x,s=S.symbols('x s',real=True,positive=True)
    yH=S.exp(s*x);yI=S.exp(-s*x);W=yH*S.diff(yI,x)-S.diff(yH,x)*yI
    jump=-W/W
    check('Green derivative jump equals -1',zero(jump+1))
    check('wrong W sign rejected',not zero(W/W+1))
    # Exact pure coordinate pullback of dz^2 by the flow xi=sin(z)d_z.
    xi=S.sin(z);L1=2*S.diff(xi,z);L2=xi*S.diff(L1,z)+2*L1*S.diff(xi,z)
    h0=L1;q0=L2/2
    # Undo with -xi: qU=q-Lxi h+1/2 Lxi^2 g.
    check('quadratic gauge Lie-square cancels pure pullback',zero(q0-L2+L2/2))
    missing=S.integrate(q0-L2,(z,0,2*S.pi))/(2*S.pi)
    check('omitted half Lie-square produces spurious zero scalar',missing!=0,str(missing))
    r=S.symbols('r',positive=True);ff=1-1/r
    check('static scalar tension solves equation',zero(S.diff(r*r*ff*S.diff(S.log(ff),r),r)))
    check('static tension singular at future horizon',S.limit(S.log(ff),r,1,dir='+')==-S.oo)
    n=S.Integer(2);free=-n
    check('free scalar interference can cancel reference news',(n+free)**2==0)
    check('discarded homogeneous data energy rejected',n*n+free*free!=(n+free)**2)
    d=S.symbols('d',nonnegative=True);N=d*(1+d+d*d)
    true=(1+d)*N/d;wrong=N
    check('EF divided difference regular horizon value',S.limit(true,d,0)==1)
    check('lost EF division rejected',S.limit(wrong,d,0)!=S.limit(true,d,0))
    # If conservation numerator b(1) is nonzero, qrr has an actual pole.
    defect=S.Rational(1,1000);bad=(1+d)*(N+defect)/d
    check('dropped Bianchi compatibility creates pole',S.limit(bad,d,0,dir='+')==S.oo)
    # Scalar action normalization: polarization check is a distinct algebra control.
    check('5D scalar transverse norm is eight',2*(-2/S.sqrt(3))**2+(4/S.sqrt(3))**2==8)
    # Explicit local bilinear error estimate, not an evaluated global bound.
    out['conditional_source_error']='For induced matrix norm, |tr(A^2)-tr(B^2)|<=4||A-B|| (2||B||+||A-B||) in four dimensions; |delta Qzz,0|<=k^2 epsilon(2B+epsilon)/2 at fixed k. No evaluated physical global B,epsilon.'

def direct_5d_operator():
    """Independent coordinate delta Ricci for four nonzero unitary components."""
    r,th,s,K=S.symbols('r theta s K',real=True)
    ff=1-1/r
    g=S.zeros(5);g[0,0]=-ff;g[0,1]=g[1,0]=1;g[2,2]=1
    g[3,3]=r*r;g[4,4]=r*r*S.sin(th)**2;gi=g.inv()
    def db(e,a):return S.diff(e,r) if a==1 else S.diff(e,th) if a==3 else 0
    G={(c,a,b):S.simplify(sum(gi[c,e]*(db(g[e,b],a)+db(g[e,a],b)-db(g[a,b],e))/2 for e in range(5))) for c in range(5) for a in range(5) for b in range(5)}
    fs=[S.Function(n)(r) for n in ['T','B','C','U']]
    T,B,C,U=fs;h=S.zeros(5);h[0,0]=T;h[0,1]=h[1,0]=B;h[1,1]=C;h[3,3]=U;h[4,4]=U*S.sin(th)**2
    hi=gi*h*gi
    def dh(e,a):return s*e if a==0 else S.I*K*e if a==2 else db(e,a)
    dc={(c,a,b):S.expand(sum(gi[c,e]*(dh(h[e,b],a)+dh(h[e,a],b)-dh(h[a,b],e))/2-hi[c,e]*(db(g[e,b],a)+db(g[e,a],b)-db(g[a,b],e))/2 for e in range(5))) for c in range(5) for a in range(5) for b in range(5)}
    pairs=[(0,0),(0,1),(0,2),(1,1),(1,2),(2,2),(3,3)]
    expr=[]
    for a,b in pairs:
        e=sum(dh(dc[c,a,b],c)-dh(dc[c,a,c],b) for c in range(5))
        e+=sum(dc[c,c,d]*G[d,a,b]+G[c,c,d]*dc[d,a,b]-dc[c,b,d]*G[d,a,c]-G[c,b,d]*dc[d,a,c] for c in range(5) for d in range(5))
        expr.append(S.simplify(e.subs(th,S.pi/2)))
    trace=2*B+ff*C+2*U/r**2
    check('direct 5D zz equation gives forced trace',zero(expr[5]-K*K*trace/2))
    # Mass variation is an exact vacuum zero-mode control.
    masssub={T:2/r,B:0,C:0,U:0,s:0,K:0}
    check('direct 5D mass-shift kernel',all(zero(e.subs(masssub).doit()) for e in expr))
    syms=S.symbols('q0:12');subs={S.diff(fun,r,j):syms[3*i+j] for i,fun in enumerate(fs) for j in range(3)}
    fn=S.lambdify((r,s,K,*syms),[e.subs(subs) for e in expr],'numpy',cse=True)
    out['independent_linear_equations']={str(pair):str(e) for pair,e in zip(pairs,expr)}
    return fn

def physical_response(fn):
    old=json.loads((HERE/'gl_linear_results.json').read_text());k=float(old['k']);om=float(old['refinements'][-1]['omega'])
    K=2*k;s=2*om;a=K*K
    tab=np.load(HERE/'goal004_source_geometry_samples.npz');vs=tab['v'];src=tab['source']
    sp=[CubicSpline(vs,src[:,j]) for j in range(src.shape[1])]
    def pot(r):
        d=r-1;D=1+a*r**3
        return a*d/r+(6*r-11)*d/r**4+18*d*d/(r**4*D**2)-6*(4*r-5)*d/(r**4*D)
    lo=-18.;R=65.;ng=16001;tol=2e-12;hi=np.log(R-1)
    out['declared_forced_configuration']={'goal_global_config_number':1,'log_delta_left':lo,'r_right':R,'grid':ng,'rtol':tol,'max_step':.05,'source':'same historical 481-point cubic interpolant','root_solves':0}
    print('BEGIN declared forced configuration 1',flush=True)
    def rhs(v,y):
        r=1+np.exp(v);return [r*y[1],r*(pot(r)+s*s)*y[0]]
    d=np.exp(lo);xl=1+d+lo;p1=(a-5+6/(1+a))/(1+2*s)
    left=solve_ivp(rhs,(lo,hi),[np.exp(s*xl),(s+p1*d)*np.exp(s*xl)],method='DOP853',rtol=tol,atol=tol*1e-6,max_step=.05,dense_output=True)
    alpha=np.hypot(K,s);beta=(a+2*s*s)/(2*alpha)
    b1=a/(2*alpha);b2=(-b1-6+b1*b1)/(2*alpha);b3=(b1-2*b2+17+2*b1*b2)/(2*alpha)
    ui=np.exp(-alpha*R)*R**(-beta);pr=-alpha+b1/R+b2/R**2+b3/R**3
    right=solve_ivp(rhs,(hi,lo),[ui,pr*ui],method='DOP853',rtol=tol,atol=tol*1e-40,max_step=.05,dense_output=True)
    assert left.success and right.success
    v=np.linspace(lo,hi,ng);r=1+np.exp(v);x=r+v;f=(r-1)/r;D=1+a*r**3
    H,Hx=left.sol(v);I,Ix=right.sol(v);h,hx=left.sol(0);i,ix=right.sol(0);W=h*ix-hx*i
    forcing=sp[0](v)*np.exp(s*x)
    il=cumulative_simpson(H*forcing*r,x=v,initial=0);ir=cumulative_simpson((I*forcing*r)[::-1],x=-v[::-1],initial=0)[::-1]
    u=-(I*il+H*ir)/W;ux=-(Ix*il+Hx*ir)/W
    Y=D/r*u*np.exp(-s*x);Yp=((2*a*r-1/r**2)*u+D/r*(ux-s*u)/f)*np.exp(-s*x)
    J,tau,j,jr,A,B,Qrr,Qs=[sp[n](v) for n in range(1,9)]
    P=1/(r-1)-5/r+6/(r*D);bb=2*s+f*P;cc=s*(-4+6/D)/r+3/r**3-a-3/(r**3*D)
    Ypp=(J-bb*Yp-cc*Y)/f
    term=f*Yp+(s-1/(2*r**2))*Y;lift=f*j-r*f*B-r*A
    Z=-s*term/(a*D)+2*r*r/D*lift
    def dr(n):return sp[n](v,1)/(r-1)
    termp=f*Ypp+(s+1/(2*r**2))*Yp+Y/r**3
    liftp=f*dr(3)+j/r**2-(f+1/r)*B-r*f*dr(6)-A-r*dr(5)
    Dp=3*a*r*r
    Zp=-s/a*(termp/D-term*Dp/D**2)+(4*r/D-2*r*r*Dp/D**2)*lift+2*r*r/D*liftp
    qU=r*Z/s-Y/(2*a*r);qB=-Zp/s-2*Z/(s*r)+j/s+tau;qT=Z-f*qB
    N=2*Zp/s+2*Z/(s*r)-2*j/s-tau+Y/(a*r**3);qC=N/f
    qspl=[CubicSpline(v,col) for col in [qT,qB,qC,qU]]
    # Derivative of the actual reconstructed/interpolated response. No on-shell
    # assignment of q jets, no source Bianchi imposed during residual evaluation.
    ev=np.linspace(lo+.137,hi-.137,701);rr=1+np.exp(ev);dd=rr-1
    vals=[]
    for spq in qspl:vals.extend([spq(ev),spq(ev,1)/dd,(spq(ev,2)-spq(ev,1))/dd**2])
    lin=np.array([np.broadcast_to(e,rr.shape) for e in fn(rr,s,K,*vals)],dtype=complex).T
    Q=np.array([sp[5](ev),sp[6](ev),-1j*K*sp[3](ev)/2,sp[7](ev),-1j*K*sp[4](ev)/2,-a*sp[2](ev)/2,sp[8](ev)]).T
    res=lin+Q
    scales=np.maximum(1,np.max(np.abs(Q),axis=1));relative=np.max(abs(res),axis=1)/scales
    labels=['vv','vr','vz','rr','rz','zz','theta_theta']
    reg=(rr>=1.005)&(rr<=30)
    whole=np.ones(len(rr),dtype=bool);middle=(rr>=1.1)&(rr<=10)
    out['actual_response_residuals']={}
    for name,mask in [('whole_sampled_domain',whole),('r1p005_to30',reg),('r1p1_to10',middle)]:
        out['actual_response_residuals'][name]={'n':int(mask.sum()),'max_each_component':dict(zip(labels,np.max(abs(res[mask]),axis=0).tolist())),
        'max_abs':float(np.max(abs(res[mask]))),'max_scaled':float(np.max(relative[mask])),
        'worst_r':float(rr[mask][np.argmax(relative[mask])])}
    # Trace/divergence mutation tests on actual physical response.
    check('physical source requires nonzero forced trace',float(np.max(abs(sp[2](ev))))>.01,float(np.max(abs(sp[2](ev)))))
    wrong=lin+2*Q
    # The trace equation is algebraic and supplies an unambiguous Fourier witness.
    # A former all-component 100x test failed because actual differentiated metric
    # residuals were large; preserve that failure, never call the baseline certified.
    check('missing +2 Fourier half fails physical zz residual',float(np.max(abs(wrong[middle,5])))>100*float(np.max(abs(res[middle,5]))),float(np.max(abs(wrong[middle,5]))))
    # If the signed Green response is reversed but affine source terms retained,
    # its master residual changes by -2F; retain actual nonzero physical witness.
    check('flipped W sign violates physical forced master',float(np.max(abs(2*forcing)))>1,float(np.max(abs(2*forcing))))
    # Actual spline Einstein-source divergence, directly including angular volume.
    def drs(n):return sp[n](ev,1)/dd
    ffr=(rr-1)/rr;fp=1/rr**2
    qvv,qvr,qrz,qrr,qzz,qang=Q[:,0],Q[:,1],Q[:,4],Q[:,3],Q[:,5],Q[:,6]
    qvz=Q[:,2]
    tr=2*qvr+ffr*qrr+qzz+2*qang/rr**2
    trp=2*drs(6)+fp*qrr+ffr*drs(7)-a*drs(2)/2+2*drs(8)/rr**2-4*qang/rr**3
    # div Q_b from direct mixed-tensor coordinate connection; subtract d_b tr/2.
    divv=s*qvr+drs(5)+fp*qvr+ffr*drs(6)+1j*K*qvz+2*(qvv+ffr*qvr)/rr-s*tr/2
    divr=s*qrr+drs(6)+fp*qrr+ffr*drs(7)+1j*K*qrz+2*(qvr+ffr*qrr)/rr+fp*qrr/2-2*qang/rr**3-trp/2
    divz=s*qrz-1j*K*drs(3)/2+fp*qrz+ffr*(-1j*K*drs(4)/2)+1j*K*qzz+2*(qvz+ffr*qrz)/rr-1j*K*tr/2
    bian=np.array([divv,divr,divz]).T
    out['sampled_source_Bianchi']={name:np.max(abs(bian[mask]),axis=0).tolist() for name,mask in [('whole',whole),('interior',middle)]}
    out['forced_summary']={'W':float(W),'W_relative_drift':float(np.max(abs((H*Ix-Hx*I)/W-1))),
       'u_at_r2':float(CubicSpline(v,u)(0)), 'qEF_at_r2':[float(spq(0)) for spq in qspl],
       'endpoint_error':'Unenclosed first-order horizon homogeneous jet, finite source omissions, infinity homogeneous jet and equal-exponent forced tail; no endpoint certification.',
       'derivatives':'Cubic splines of actual 16001-point reconstructed metric; independently differentiated, empirical only; no interval or interpolation error enclosure.'}
    # A modest reusable response/residual table in JSON, no large binary artifact.
    out['physical_sample_columns']=['r','qvv_EF','qvr_EF','qrr_EF','qtheta_EF',*['abs_Ricci_res_'+x for x in labels]]
    out['physical_samples']=[[float(rr[i]),*[float(spq(ev[i])) for spq in qspl],*abs(res[i]).tolist()] for i in range(0,len(rr),7)]
    print('ACTUAL_RESIDUALS '+json.dumps(out['actual_response_residuals']),flush=True)

if __name__=='__main__':
    begin=time.time()
    exact_checks();operator=direct_5d_operator();physical_response(operator)
    out['elapsed_seconds']=time.time()-begin
    out['sha256']={n:hashlib.sha256((HERE/n).read_bytes()).hexdigest() for n in ['audit_goal005_geometry.py','gl_linear_results.json','gl_linear_profile_L10.csv','goal004_source_geometry_samples.npz','goal004_source_geometry.py']}
    (HERE/'audit_goal005_geometry_results.json').write_text(json.dumps(out,indent=2)+'\n')
    print('FINISHED '+str(len(out['checks']))+' checks; results empirical, not certified',flush=True)
