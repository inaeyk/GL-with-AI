"""Independent weak-certificate spot checks. No physical evolution."""
from fractions import Fraction as Q
from math import comb
from flint import arb, ctx


def shift(coeffs, origin):
    """Ascending coefficients after r = origin + s."""
    return [sum(Q(coef) * comb(i, j) * origin**(i-j)
                for i, coef in enumerate(coeffs) if i >= j)
            for j in range(len(coeffs))]


ctx.prec = 160
r_cut = Q('1.596')
for coeffs in ((-8, 18, -9, 2), (8, -18, 7, 2), (-3, 9, -6, 0, 1)):
    assert min(shift(coeffs, r_cut)) > 0
print('Packet |d_u log W| <= 1/4 and W <= 1/4: exact shifted polynomials positive')

# b = 1-u, g = u, c = 2, and w = u^2(v-b)^2/4, with W = G = H = 0.
ray = (0, 0, 1, 2, 1)  # Integral of F=4w_v along v, as polynomial in u.
terminal = sum(ray)
bulk = sum(Q(coef, i+1) for i, coef in enumerate(ray))
primal = sum(Q(i*coef, i+1) for i, coef in enumerate(ray))
assert primal == terminal-bulk == Q(89, 30)
assert -bulk == -Q(31, 30)
print(f'Tilted-graph endpoint fixture 4P={primal}, terminal={terminal}, without terminal={-bulk}')

early = (Q('70.17398932866') - sum(map(Q, ('51.33607717719',
         '6.86913231350', '4.25246355355', '0.88801490227'))))/4 - Q('6.948e-16')
packet = (-Q('4393.82672796645') + sum(map(Q, ('44.02052046911',
          '2078.31965086407', '0.44414526251', '616.13374721694',
          '854.54174864039'))))/4 + Q('1.63462e-14')
assert early > Q('1.70707534') and packet < Q('-200.09172887')
print(f'Outward rational replay early lower > {float(early):.11f}, packet upper < {float(packet):.11f} (display only)')


def x(r):
    return r+(r-1).log()


def mass(r):
    return 3/r-3/(4*r*r)


cut, R, D = arb('1.596'), arb(13)/8, arb(3)/25
front = R-2*x(R)
d = front+D
assert cut-2*x(cut)>d
assert mass(cut)+((D/4).exp()-1)*(mass(cut)-mass(arb(2)))<arb('1.60')
segments = [(cut, arb(5)/2, 512), (arb(5)/2, arb(8), 768),
            (arb(8), arb(25), 512), (arb(25), arb(64), 256),
            (arb(64), arb(400), 256)]
left_offsets = [d+2*x(lo+(hi-lo)*j/n)-R
                for lo, hi, n in segments for j in range(n)]
assert all(z < 0 or z > 0 for z in left_offsets)
minsep = min(z.abs_lower() for z in left_offsets)
assert minsep > arb('0.00466338')
print('Packet wedge cells', sum(z<0 for z in left_offsets),
      'with smallest directed |v0-R| lower bound', minsep.str(16))

mcap = mass(arb(400))
for label, width, gd, g1, gp1, tolerance in [
    ('early', '.015', '.412547300', '.000791631', '.413693', '6.948e-16'),
    ('packet', '.12', '.009754934', '.271373', '19.176120', '1.63462e-14'),
]:
    width, gd, g1, gp1 = map(arb, (width, gd, g1, gp1))
    A = (width*mcap).exp()
    ext = ((gd+gp1+(1+mcap)*A*g1)*arb('3.36e-15')
           +(A-1)*g1*arb('2.74e-15'))/4
    assert ext<arb(tolerance)
    print(label, 'weak exterior bound', ext.str(16))
