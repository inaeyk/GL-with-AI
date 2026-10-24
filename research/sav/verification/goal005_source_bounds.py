"""Explicit conservative component-count bound, not a source interpolant.
All arithmetic/proof decisions exact rational or outward Arb. See physics note.
"""
from flint import arb,ctx
from pathlib import Path
import json,hashlib
ctx.prec=160
HERE=Path(__file__).resolve().parent
B=arb('2e31');K=arb(1000)
checks=[]
def test(name,left,right):
    assert left<right,name
    checks.append(dict(name=name,left=str(left),strict_upper=str(right)))
test('EF coefficient conversion',32*arb('4e29'),B)
test('relative sphere C5',32*720,arb('1e5'))
I1=arb('1e9');I2=arb('1e14');G0=arb('1e8');G1=arb('1e14');G2=arb('1e19')
test('inverse1 C4 per B',9*3**4*K*K,I1)
test('inverse2 C4 per B2',81*5**4*K**3,I2)
test('connection0 C4',arb('9/2')*16*K*K,G0)
test('connection1 C4 per B',72*(K+I1*K),G1)
test('connection2 C4 per B2',72*(I1+I2*K),G2)
test('orbit Ricci quadratic C3 per B2',6*G2+36*8*G0*G2+18*8*G1*G1,arb('1e32'))
W=arb('1e5');L=K
warped=16*W*W+24*G2*L+24*G1*W+81*G0*W*W+27*L*W*W+4*W*W
test('warped orbit quadratic C3 per B2',warped,arb('1e25'))
# Reduced sphere Hessian factors X_i after division by background r².
W1=arb('1e10');X0=arb('1e13');X1=arb('1e21');X2=arb('1e26')
test('W1 first/second derivatives divided r2 C3 per B',W+3*16*L*W,W1)
test('sphere Hessian0 C3',L+24*G0*L,X0)
test('sphere Hessian1 C3 per B',W1+24*G0*W1+24*G1*L,X1)
test('sphere Hessian2 C3 per B2',24*(G1*W1+G2*L),X2)
test('sphere quadratic divided r2 C3 per B2',36*(K*X2+I1*X1+I2*X0),arb('1e33'))
test('raw sphere product per r2 B2',16*arb('1e33'),arb('2e34'))
# Trace adjustment: at most nine orbit products and two equal sphere terms.
test('Einstein trace-adjusted coefficient per r2 B2',arb('2e34')*(1+arb('1/2')*27*K*(9*K+48)),arb('1e44'))
om=arb('0.06328119','0.00000001');a=(2*arb.pi()/5)**2
sigma=(4*om*om+a/4).sqrt()
test('old inverse exterior weight sigma',sigma,arb('0.65'))
test('r2 exterior exponential multiplier',(4+2/arb('0.35'))**2,arb(100))
test('all seven Q and conserved Einstein source C3',arb('1e46')*B*B,arb('1e110'))
out=dict(scope='Coarse physical C3 source upper envelope only; full historical Y0 including third master-force derivative NOT certified',
    common_C3_bound='1e110',interior='regular EF coefficients 1<=r<=4',
    exterior='exp(sigma*(r-4)) static coefficients r>=4',sigma=str(sigma),
    assumptions='Certified mixed metric C5 bound4e29; exact on-shell GL reconstruction; explicit quadratic connection and reduced sphere identities',
    checks=checks,source_sha256={f:hashlib.sha256((HERE/f).read_bytes()).hexdigest() for f in ['goal005_source_bounds.py','goal005_mode_bounds.py','goal005_mode_bounds.json']})
(HERE/'goal005_source_bounds.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
