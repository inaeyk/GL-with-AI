"""Directed square-barrier tests and derivative-free inverse-band budgets."""
from platform import python_version

import flint
from flint import acb, arb, ctx


def barrier(s, x, height):
    q = (s*s+height).sqrt()
    def interior(y):
        return ((q*y).cosh()+s/q*(q*y).sinh(),
                q*(q*y).sinh()+s*(q*y).cosh())
    h, hx = interior(arb(1))
    m_plus = (-s).exp()*(h+hx/s)/2
    if x < 0:
        m = acb(1)
    elif x <= 1:
        m = (-s*x).exp()*interior(x)[0]
    else:
        m = m_plus+(-s*(2*x-1)).exp()*(h-hx/s)/2
    return -m/(2*m_plus)


def transform_born(s, x, height):
    # Laplace transform of the time-domain ramp and plateau.
    delay_left = 2*max(x-1, arb(0))
    delay_right = 2*max(x, arb(0))
    mass = 1-min(max(x, arb(0)), arb(1))
    return height/4*(mass/s+((-s*delay_left).exp()-(-s*delay_right).exp())/(2*s*s))


def spatial_born(s, x, height):
    # Radial expression from the resolvent, evaluated independently.
    if x < 0:
        return height/(4*s)
    stop = min(x, arb(1))
    undelayed = (1-stop)*height
    delayed = ((-2*s*(x-stop)).exp()-(-2*s*x).exp())*height/(2*s)
    return (undelayed+delayed)/(4*s)


def residual_majorant(s, mass):
    modulus = abs(s)
    return 3*mass**2*(mass/modulus).exp()/(8*modulus**2)


def main():
    ctx.prec = 128
    print(f'Python {python_version()}, python-flint {flint.__version__}, Arb precision {ctx.prec}')
    tested = 0
    for sigma in ('.25', '1', '5'):
        for frequency in ('0', '.75', '8', '32'):
            s = acb(sigma, frequency)
            for x in map(arb, ('-.5', '0', '.125', '.5', '1', '1.25', '4')):
                for height in map(arb, ('.125', '1')):
                    exact = barrier(s,x,height)
                    born = transform_born(s,x,height)
                    assert born.overlaps(spatial_born(s,x,height))
                    assert abs(exact+arb('.5')-born) < residual_majorant(s,height)
                    tested += 1
    print(f'{tested} square-barrier transfer and time-kernel tests passed')
    duration = arb('8.407362')+arb(13)/8+2*(arb(5)/8).log()
    gamma, mass = arb('.25'), arb('4.5')
    transfer = (gamma*duration + mass/(8*gamma)).exp()
    print(f'volume L1_x L2_u error gain < {transfer.upper().str(15)}')
    print('band  Born-tail gain/A0  sufficient A0 cap for tail allocation 0.05')
    for B in map(arb,(16,32,64,128,256,512)):
        gain = (gamma*duration+mass/(gamma**2+B**2).sqrt()).exp()
        gain *= 3*mass**2/(8*(3*arb.pi()).sqrt()*B**arb('1.5'))
        print(f'{B}  {gain.upper().str(12)}  {(arb(".05")/gain).lower().str(12)}')


if __name__=='__main__':
    main()
