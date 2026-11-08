"""Independent manufactured exact polynomial check of the local march."""
import numpy as np

from late_march import LateStrip, march, weights


def main():
    s = LateStrip(16,16,16,3)
    u = ((s.u-s.u[0])/10)[:,None]
    v = (s.v/10)[None,:]
    W = np.stack((u*u*v*v,u*u*v),axis=-1)
    P = np.stack((u*v*v/5,u*v/5),axis=-1)
    Q = np.stack((u*u*v/5,np.broadcast_to(u*u/10,s.r.shape)),axis=-1)
    D = np.stack((u*v/25,np.broadcast_to(u/50,s.r.shape)),axis=-1)
    potential = np.stack((.7+0*s.r,.9+0*s.r),axis=-1)
    skew = .21+0*s.r
    rhs = -4*D-potential*W+skew[...,None]*np.stack((-(P+Q)[...,1],(P+Q)[...,0]),axis=-1)
    a,b,c = march(W[s.graph],P[s.graph],Q[s.graph],potential,rhs,skew,
                  weights(s.u,4),weights(s.v,4),s.n,4,True)
    errors = tuple(np.max(abs(X[s.mask]-Y[s.mask])) for X,Y in ((a,W),(b,P),(c,Q)))
    assert max(errors) < 3e-13
    print("manufactured coupled polynomial graph/front max nodal errors W,P,Q:",
          ", ".join(f"{x:.6e}" for x in errors))


if __name__=="__main__":
    main()
