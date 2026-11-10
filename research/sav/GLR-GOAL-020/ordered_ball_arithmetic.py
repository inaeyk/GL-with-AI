"""Exact arithmetic for GOAL-020. Analytic coefficient and energy caps are inputs."""
from decimal import Decimal, localcontext
from fractions import Fraction as Q
from math import comb, factorial
from platform import python_version


def pow10(n):
    return Q(10)**n


def show(x):
    """Decimal display only. All assertions compare exact fractions."""
    x = Q(x)
    with localcontext() as ctx:
        ctx.prec = 14
        return f"{Decimal(x.numerator) / Decimal(x.denominator):.12g}"


def exp_enclosure(x, terms=64):
    assert x >= 0 and x < terms+2
    term = total = Q(1)
    for j in range(1, terms+1):
        term *= x/j
        total += term
    tail = term*x/(terms+1)/(1-x/(terms+2))
    return total, total+tail


def log_enclosure(x, terms=24):
    assert x >= 1
    t = (x-1)/(x+1)
    lower = 2*sum(t**(2*j+1)/(2*j+1) for j in range(terms))
    tail = 2*t**(2*terms+1)/((2*terms+1)*(1-t*t))
    return lower, lower+tail


def bump_jet_factor(j):
    """Bell-polynomial bound in |b^(j)| <= C_j t^(2j) exp(-t), t>=4."""
    if j == 0:
        return Q(1)
    return sum(Q(factorial(j), factorial(k))*comb(j-1,k-1)*Q(4)**(k-j)
               for k in range(1,j+1))


def graph_and_volume():
    delta = Q(3,25)
    log85 = log_enclosure(Q(8,5))
    a = tuple(-Q(13,8)+2*z for z in log85)
    d = tuple(z+delta for z in a)
    log2 = log_enclosure(Q(2))
    assert -1 < a[0] < a[1] < d[0] < d[1] < 0
    assert d[1] < -Q(3,2)+2*log2[0]
    assert -2 < a[0]
    # x=(v-u)/2: at v=220 it exceeds 110, while x(100)<105.
    assert exp_enclosure(Q(5))[0] > 99
    assert (220-a[0])/2 < 120
    assert (1-Q(1,100))/2 > Q(49,100)
    assert 1-Q(2,3)**2 > Q(1,3)
    graph_baseline_energy = max(Q(1), Q(3)/Q(3,2)**2)
    assert graph_baseline_energy < 4
    e_short = exp_enclosure(Q(9,4)*delta)[1]
    assert e_short < Q(4,3) and delta < Q(7,20)**2
    assert Q(7,20)*(1+Q(9,4)*e_short) < 2
    print("graph a and d approx", show(a[0]), show(d[1]))
    print("baseline graph energy cap/U^2", graph_baseline_energy,
          "and sup news/U < 1.4")

    # Inherited raw mode envelope and the global mean-cap arithmetic.
    raw_gl = 3*(4*Q("2.2")+8*Q("1.064")+14)*6*pow10(15)
    assert raw_gl < pow10(18)
    assert exp_enclosure(Q("0.064")*220)[1] < 2*pow10(6)
    assert 100*2*pow10(6)*6*pow10(15) < pow10(25)
    assert 100*2*pow10(6)*228 < pow10(25)
    # Integral of (1+r^2) exp(-rate*(r-4)_+).
    rate = Q(4,5)
    inner = (4+Q(4)**3/3)-(Q(3,2)+Q(3,2)**3/3)
    integral = inner+17/rate+8/rate**2+2/rate**3
    assert integral < 1000
    assert Q(2,4) < rate and 16 < 100
    mean_integral_cap = Q(1,2)*pow10(54)*1000
    mean_radial_jet_cap = Q(1,2)*pow10(54)*100
    mean_v_jet_cap = 2*Q("0.064")*mean_integral_cap
    assert 100*max(mean_integral_cap, mean_radial_jet_cap, mean_v_jet_cap) < pow10(60)
    assert rate*Q("0.49")-Q("0.49")/101 > Q(1,10)
    assert pow10(9)*101 < pow10(60)
    assert Q(22,35)**2+4/Q(3,2)**2 < 5
    assert 5+14/Q(3,2)**2 < 20
    print("mean radial integral exactly", integral,
          "and transformed inner mean cap <=", show(100*mean_integral_cap))

    # B-SOURCE (5), using D_u<10 and sqrt(D_u)<4. These are upper caps
    # on the arithmetic mean of the two oriented mixed-norm coefficients.
    def work_average(c1, c2):
        return (152*c1+152*10)/2+2*4*c2

    raw_c1, raw_c2_sq = 20-1+10, 20-1+5
    c1, c2_sq = 220-1+10, 220-1+5
    assert raw_c1 == 29 and raw_c2_sq < 5**2
    assert c1 < 230 and c2_sq < 15**2
    assert work_average(Q(29),5) < pow10(4)
    assert work_average(Q(230),15) < pow10(5)

    baseline_commuted = pow10(13)
    mixed_graph = pow10(45)
    mixed_volume = pow10(11)*pow10(18)*3*baseline_commuted*pow10(4)
    assert mixed_graph+mixed_volume < pow10(48)
    h_gl, mean = pow10(18), pow10(60)
    w0_graph, wm_graph = pow10(13), pow10(45)
    a1_graph = pow10(5)*wm_graph+pow10(15)*h_gl*w0_graph
    a0_graph = pow10(3)*w0_graph
    return_graph_velocity = 24*(pow10(4)*h_gl*a1_graph
                                +pow10(10)*(h_gl**2+mean)*a0_graph)
    assert return_graph_velocity < pow10(90)
    assert delta*return_graph_velocity**2/2 < pow10(184)
    mixed_work = pow10(15)*h_gl*2*pow10(48)
    baseline_work = pow10(25)*(h_gl**2+mean)*3*baseline_commuted
    return_flux = pow10(92)+pow10(5)*(mixed_work+baseline_work)
    assert return_flux < pow10(105)
    assert mixed_graph+mixed_volume == Q("3.1e46")
    assert return_flux < Q("3.01e103") and return_graph_velocity < Q("2.41e87")
    print("mixed flux/U < 3.1e46; return flux/U < 3.01e103")
    print("return graph velocity/U < 2.41e87")
    return delta


def profile_and_ball(delta):
    cb = Q(40)
    assert exp_enclosure(Q(4))[1]**2 < 2*cb**2
    bell = [bump_jet_factor(j) for j in range(6)]
    assert max(bell[:5]) < 10 and bell[5] < 18
    maxima = [Q(max(4,n))**n/exp_enclosure(Q(max(4,n)))[0]
              for n in range(11)]
    assert max(maxima[:9]) < pow10(4) and maxima[10] < pow10(7)
    assert delta**(-7) < 2000**2
    jets = [pow10(5)]*5+[18*pow10(7)]
    base_sq = sum((cb*delta**(1-j)*jets[j])**2 for j in range(5))
    assert base_sq < (2*pow10(10))**2
    width_floor = delta/2
    shift_sq = sum((cb*width_floor**(-j)*jets[j+1])**2 for j in range(5))
    width_sq = sum((cb*width_floor**(-j)*(abs(Q(1,2)-j)*jets[j]+jets[j+1]))**2
                   for j in range(5))
    assert max(shift_sq,width_sq) < (3*pow10(15))**2
    print("bump Bell caps j=0..5:", ", ".join(str(x) for x in bell))
    print("canonical H4 norm < 2e10; both normalized profile parameter derivatives < 3e15")

    ball = pow10(-118)
    b = pow10(11)
    assert 2*pow10(10)+ball < b
    slab_scale = pow10(-136)
    left_shift = (slab_scale/2,3*slab_scale/2)
    inward_width = (2*slab_scale,3*slab_scale)
    assert 0 < left_shift[0] < left_shift[1] < inward_width[0] < inward_width[1] < delta/2
    param_error = pow10(17)*(left_shift[1]+inward_width[1])
    assert param_error < ball
    tight_param_error = 3*pow10(15)*(left_shift[1]+inward_width[1])
    assert tight_param_error == Q("1.35e-120")
    print("admissible two-parameter rectangle H4 displacement < 1.35e-120")

    early_input, packet_input = Q("3.3605"), Q("-200.091")
    error = 2*pow10(105)*(2*b+ball)*ball
    assert error < Q("0.041")
    early_margin = early_input-error
    packet_margin = -packet_input-error
    added_upper = packet_input-early_input+2*error
    assert early_margin > Q("3.31") and packet_margin > Q("200.05")
    assert added_upper < Q("-203.36")
    print("ball projection error <", show(Q("0.041")),
          "early lower >", show(Q("3.31")),
          "packet upper <", show(-Q("200.05")),
          "added upper <", show(Q("-203.36")))
    assert 2*pow10(105)*(2*b+pow10(-115))*pow10(-115) > early_input

    prefix = 2*pow10(105)*b*b
    assert prefix == 2*pow10(127)
    assert prefix*pow10(-130) == Q("0.002")
    assert Q("0.002") < min(early_margin,packet_margin)
    print("original symmetric crossing Δ=1e-260 costs 0.002 at each end")
    delta_e, delta_l = Q("2.7")*pow10(-254), pow10(-250)
    assert prefix**2*delta_e < Q("3.29")**2 < early_margin**2
    assert prefix*pow10(-125) == 200 < packet_margin
    assert delta_e+delta_l < delta-Q(3,200)
    print("asymmetric crossing offsets 2.7e-254 and 1e-250 cost <3.29 and exactly 200")
    print("negative control: radius 1e-115 exceeds this early error budget")


def main():
    print(f"Python {python_version()}, fractions.Fraction and integer decisions")
    print("GOAL-015 source 65a4af854a25ca02e40f31b00fe5ff789935bdc4")
    print("GOAL-015 audit 1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4")
    profile_and_ball(graph_and_volume())


if __name__ == "__main__":
    main()
