"""Independent strong-source quadrature on the same computed physical fields."""
import numpy as np
from numpy.polynomial import Chebyshev
from numpy.polynomial.legendre import leggauss

from overlap_gl import tortoise
from returning_overlap import Experiment,LAMBDA


def strong_piece(experiment,a,b,r_of,current,degree):
    nodes=np.cos(np.pi*(np.arange(degree+1)+.5)/(degree+1))
    midpoint=(a+b)/2
    scale=(b-a)/2
    param=midpoint+scale*nodes
    sampled=np.asarray([current(q) for q in param])
    fit=[[Chebyshev.fit(nodes,sampled[:,i,j],degree,domain=[-1,1])
          for j in range(3)] for i in range(3)]
    z,w=leggauss(degree+8)
    radii=np.array([r_of(midpoint+scale*p) for p in z])
    ds=(r_of(b)-r_of(a))/2
    derivatives=np.stack([q.deriv()(z)/ds for q in fit[0]],axis=1)
    vv=np.stack([q(z) for q in fit[1]],axis=1)
    cc=np.stack([q(z) for q in fit[2]],axis=1)
    h=np.array([experiment.test.mode(r)[0] for r in radii])
    factor=2/LAMBDA*w*radii*h*np.exp(experiment.s*tortoise(radii))*ds
    return factor @ (derivatives-experiment.s*vv-LAMBDA*cc/radii[:,None]**4)


def strong(experiment,cap,degree):
    collar,_=experiment.collar(cap,6)
    real_edges=[1+cap,1.045,1.13,1.3,1.48,experiment.baseline.inner,
                experiment.baseline.radius,1.85,2.15,experiment.rmatch]
    ray_edges=[0,1,3,6,12,22,36,experiment.length]

    def ray_r(u):
        return experiment.rmatch+experiment.ray*u

    real=sum(strong_piece(experiment,a,b,lambda r:r,experiment.regular_current,degree)
             for a,b in zip(real_edges,real_edges[1:]))
    ray=sum(strong_piece(experiment,a,b,ray_r,
                         lambda q:experiment.regular_current(ray_r(q),q),degree)
            for a,b in zip(ray_edges,ray_edges[1:]))
    return collar+real+ray


if __name__=="__main__":
    e=Experiment(13/8,.12,64,1e-6,2e-11,np.pi/4)
    weak,_,_=e.evaluate(.008,6)
    print("weak numerator parts",weak)
    for n in (20,30,42):
        value=strong(e,.011,n)
        print("strong degree",n,"numerator parts",value)
        print("strong minus weak",value-weak,"residue",sum(value)/e.wprime)
