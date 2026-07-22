#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

constexpr std::size_t row_count = 5;
constexpr double tolerance = 3.0e-12;

struct Jet
{
    double value = 0.0;
    double dx = 0.0;
    double dz = 0.0;
    double dxx = 0.0;
    double dxz = 0.0;
    double dzz = 0.0;
};

struct Direction
{
    std::array<Jet, Operator::frozen_gauge_state_vector.size()> data = {};
    Jet &at(const Variable variable)
    {
        return data[Operator::variable_index(variable)];
    }
    const Jet &at(const Variable variable) const
    {
        return data[Operator::variable_index(variable)];
    }
};

struct Rows
{
    std::array<double, row_count> value = {};
};

enum Family : std::size_t
{
    advection,
    shift_stretching,
    algebraic,
    family_count
};

struct Families
{
    std::array<Rows, family_count> family = {};
    Rows total;
};

struct NonlinearState
{
    double chi = 0.0;
    double dx_chi = 0.0;
    double k = 0.0;
    double h[2][2] = {};
    double dx_h[2][2] = {};
    double h_ww = 0.0;
    double dx_h_ww = 0.0;
    double a[2][2] = {};
    double a_ww = 0.0;
};

[[noreturn]] void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_true(const std::string &label, const bool condition)
{
    if (!condition)
    {
        fail(label, "condition was false");
    }
    std::cout << "PASS " << label << "\n";
}

void require_close(const std::string &label, const double actual,
                   const double expected, const double allowed = tolerance)
{
    if (std::abs(actual - expected) > allowed)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected));
    }
}

double max_error(const Rows &left, const Rows &right)
{
    double out = 0.0;
    for (std::size_t i = 0; i < row_count; ++i)
    {
        out = std::max(out, std::abs(left.value[i] - right.value[i]));
    }
    return out;
}

Rows add(const Rows &left, const Rows &right)
{
    Rows out;
    for (std::size_t i = 0; i < row_count; ++i)
    {
        out.value[i] = left.value[i] + right.value[i];
    }
    return out;
}

Rows subtract(const Rows &left, const Rows &right)
{
    Rows out;
    for (std::size_t i = 0; i < row_count; ++i)
    {
        out.value[i] = left.value[i] - right.value[i];
    }
    return out;
}

Rows scale(const Rows &input, const double factor)
{
    Rows out;
    for (std::size_t i = 0; i < row_count; ++i)
    {
        out.value[i] = factor * input.value[i];
    }
    return out;
}

Rows rows_from_vector(const Vector &input)
{
    return {{{input.value(Variable::chi), input.value(Variable::h_xx),
              input.value(Variable::h_xz), input.value(Variable::h_zz),
              input.value(Variable::h_ww)}}};
}

NonlinearState make_nonlinear_state(const double r0, const double x,
                                    const double epsilon,
                                    const Direction &direction)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    NonlinearState state;
    state.chi = 1.0 + epsilon * direction.at(Variable::chi).value;
    state.dx_chi = epsilon * direction.at(Variable::chi).dx;
    state.k = 1.5 * lambda + epsilon * direction.at(Variable::K).value;

    double raw_h[2][2] = {};
    double raw_dx_h[2][2] = {};
    raw_h[0][0] = 1.0 + epsilon * direction.at(Variable::h_xx).value;
    raw_h[0][1] = epsilon * direction.at(Variable::h_xz).value;
    raw_h[1][0] = raw_h[0][1];
    raw_h[1][1] = 1.0 + epsilon * direction.at(Variable::h_zz).value;
    raw_dx_h[0][0] = epsilon * direction.at(Variable::h_xx).dx;
    raw_dx_h[0][1] = epsilon * direction.at(Variable::h_xz).dx;
    raw_dx_h[1][0] = raw_dx_h[0][1];
    raw_dx_h[1][1] = epsilon * direction.at(Variable::h_zz).dx;
    const double raw_h_ww =
        1.0 + epsilon * direction.at(Variable::h_ww).value;
    const double raw_dx_h_ww = epsilon * direction.at(Variable::h_ww).dx;
    const double reduced_det =
        raw_h[0][0] * raw_h[1][1] - raw_h[0][1] * raw_h[1][0];
    const double determinant = reduced_det * raw_h_ww * raw_h_ww;
    if (!(determinant > 0.0))
    {
        fail("analytic chi/metric oracle", "nonpositive conformal determinant");
    }
    const double dx_reduced_det =
        raw_dx_h[0][0] * raw_h[1][1] +
        raw_h[0][0] * raw_dx_h[1][1] -
        2.0 * raw_h[0][1] * raw_dx_h[0][1];
    const double dx_determinant =
        dx_reduced_det * raw_h_ww * raw_h_ww +
        2.0 * reduced_det * raw_h_ww * raw_dx_h_ww;
    const double normalization = std::pow(determinant, -0.25);
    const double dx_normalization =
        -0.25 * normalization * dx_determinant / determinant;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            state.h[i][j] = normalization * raw_h[i][j];
            state.dx_h[i][j] =
                dx_normalization * raw_h[i][j] +
                normalization * raw_dx_h[i][j];
        }
    }
    state.h_ww = normalization * raw_h_ww;
    state.dx_h_ww =
        dx_normalization * raw_h_ww + normalization * raw_dx_h_ww;

    const double reduced_h_det =
        state.h[0][0] * state.h[1][1] -
        state.h[0][1] * state.h[1][0];
    const double inverse[2][2] = {
        {state.h[1][1] / reduced_h_det,
         -state.h[0][1] / reduced_h_det},
        {-state.h[1][0] / reduced_h_det,
         state.h[0][0] / reduced_h_det}};
    double raw_a[2][2] = {};
    raw_a[0][0] = -0.875 * lambda +
                  epsilon * direction.at(Variable::A_xx).value;
    raw_a[0][1] = epsilon * direction.at(Variable::A_xz).value;
    raw_a[1][0] = raw_a[0][1];
    raw_a[1][1] = -0.375 * lambda +
                  epsilon * direction.at(Variable::A_zz).value;
    const double raw_a_ww =
        0.625 * lambda + epsilon * direction.at(Variable::A_ww).value;
    double trace = 2.0 * raw_a_ww / state.h_ww;
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            trace += inverse[i][j] * raw_a[i][j];
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            state.a[i][j] = raw_a[i][j] - 0.25 * state.h[i][j] * trace;
        }
    }
    state.a_ww = raw_a_ww - 0.25 * state.h_ww * trace;
    return state;
}

Families evaluate_nonlinear(const double r0, const double x,
                            const double epsilon,
                            const Direction &direction)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double beta = std::sqrt(r0 / x);
    const double dx_beta = -0.5 * lambda;
    const double beta_over_x = lambda;
    const double div_beta = dx_beta + 2.0 * beta_over_x;
    const auto state = make_nonlinear_state(r0, x, epsilon, direction);
    Families out;
    out.family[advection] =
        {{{beta * state.dx_chi, beta * state.dx_h[0][0],
           beta * state.dx_h[0][1], beta * state.dx_h[1][1],
           beta * state.dx_h_ww}}};
    out.family[shift_stretching] =
        {{{0.0,
           -0.5 * div_beta * state.h[0][0] +
               2.0 * dx_beta * state.h[0][0],
           -0.5 * div_beta * state.h[0][1] +
               dx_beta * state.h[0][1],
           -0.5 * div_beta * state.h[1][1],
           -0.5 * div_beta * state.h_ww +
               2.0 * beta_over_x * state.h_ww}}};
    out.family[algebraic] =
        {{{0.5 * state.chi * (state.k - div_beta),
           -2.0 * state.a[0][0], -2.0 * state.a[0][1],
           -2.0 * state.a[1][1], -2.0 * state.a_ww}}};
    for (const auto &family : out.family)
    {
        out.total = add(out.total, family);
    }
    return out;
}

Families central_difference(const double r0, const double x,
                            const double epsilon,
                            const Direction &direction)
{
    const auto plus = evaluate_nonlinear(r0, x, epsilon, direction);
    const auto minus = evaluate_nonlinear(r0, x, -epsilon, direction);
    Families out;
    for (std::size_t family = 0; family < family_count; ++family)
    {
        out.family[family] =
            scale(subtract(plus.family[family], minus.family[family]),
                  1.0 / (2.0 * epsilon));
        out.total = add(out.total, out.family[family]);
    }
    return out;
}

Vector make_state(const Direction &direction)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] = direction.at(variable).value;
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector make_advection(const double r0, const double x,
                      const Direction &direction)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    const double beta = std::sqrt(r0 / x);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] =
            beta * direction.at(variable).dx;
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Rows assemble(const double r0, const double x, const Direction &direction,
              const Vector *advection_dependency = nullptr,
              Vector *full_output = nullptr)
{
    const auto default_advection = make_advection(r0, x, direction);
    const auto &advection_output = advection_dependency == nullptr
                                       ? default_advection
                                       : *advection_dependency;
    const auto output = Operator::apply_complete_chi_metric_rows_at_point(
        r0, x, make_state(direction), advection_output);
    if (full_output != nullptr)
    {
        *full_output = output;
    }
    return rows_from_vector(output);
}

std::array<Rows, family_count> production_families(
    const double r0, const double x, const Direction &direction)
{
    const auto state = make_state(direction);
    const std::array<Vector, family_count> outputs = {
        make_advection(r0, x, direction),
        Operator::apply_tensor_shift_stretching_at_point(r0, x, state),
        Operator::apply_algebraic_metric_chi_coupling_at_point(state)};
    std::array<Rows, family_count> out = {};
    for (std::size_t family = 0; family < family_count; ++family)
    {
        out[family] = rows_from_vector(outputs[family]);
    }
    return out;
}

Direction mixed_direction(const double r0, const double x)
{
    Direction d;
    d.at(Variable::chi) = {0.13, 0.21, -0.16, -0.08, 0.12, 0.17};
    d.at(Variable::h_xx) = {0.31, -0.41, 0.23, 0.37, -0.19, 0.29};
    d.at(Variable::h_xz) = {-0.22, 0.19, -0.29, -0.33, 0.27, -0.24};
    d.at(Variable::h_zz) = {-0.17, 0.37, -0.11, 0.28, -0.31, 0.35};
    d.at(Variable::h_ww) = {-0.07, 0.02, -0.06, -0.08, 0.04, -0.32};
    d.at(Variable::K) = {-0.27, 0.34, -0.29, 0.0, 0.0, 0.0};
    d.at(Variable::A_xx) = {0.11, -0.17, 0.13, 0.0, 0.0, 0.0};
    d.at(Variable::A_xz) = {-0.09, 0.14, -0.12, 0.0, 0.0, 0.0};
    d.at(Variable::A_zz) = {0.07, 0.16, -0.10, 0.0, 0.0, 0.0};
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double lambda_x = -1.5 * lambda / x;
    const double metric_trace = 7.0 * lambda * d.at(Variable::h_xx).value / 8.0 +
                                3.0 * lambda * d.at(Variable::h_zz).value / 8.0 -
                                5.0 * lambda * d.at(Variable::h_ww).value / 4.0;
    d.at(Variable::A_ww).value =
        -0.5 * (d.at(Variable::A_xx).value +
                d.at(Variable::A_zz).value + metric_trace);
    const double metric_trace_dx =
        7.0 / 8.0 * (lambda_x * d.at(Variable::h_xx).value +
                     lambda * d.at(Variable::h_xx).dx) +
        3.0 / 8.0 * (lambda_x * d.at(Variable::h_zz).value +
                     lambda * d.at(Variable::h_zz).dx) -
        5.0 / 4.0 * (lambda_x * d.at(Variable::h_ww).value +
                     lambda * d.at(Variable::h_ww).dx);
    d.at(Variable::A_ww).dx =
        -0.5 * (d.at(Variable::A_xx).dx + d.at(Variable::A_zz).dx +
                metric_trace_dx);
    return d;
}

Direction second_mixed_direction(const double r0, const double x)
{
    auto d = mixed_direction(r0, x);
    for (std::size_t i = 0; i < d.data.size(); ++i)
    {
        const double sign = i % 2 == 0 ? -1.0 : 1.0;
        d.data[i].value *= 0.71 * sign;
        d.data[i].dx *= -0.83 * sign;
        d.data[i].dz *= 0.91 * sign;
    }
    d.at(Variable::h_ww).value =
        -0.5 * (d.at(Variable::h_xx).value +
                d.at(Variable::h_zz).value);
    d.at(Variable::h_ww).dx =
        -0.5 * (d.at(Variable::h_xx).dx + d.at(Variable::h_zz).dx);
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double lambda_x = -1.5 * lambda / x;
    const double metric_trace = 7.0 * lambda * d.at(Variable::h_xx).value / 8.0 +
                                3.0 * lambda * d.at(Variable::h_zz).value / 8.0 -
                                5.0 * lambda * d.at(Variable::h_ww).value / 4.0;
    d.at(Variable::A_ww).value =
        -0.5 * (d.at(Variable::A_xx).value +
                d.at(Variable::A_zz).value + metric_trace);
    const double metric_trace_dx =
        7.0 / 8.0 * (lambda_x * d.at(Variable::h_xx).value +
                     lambda * d.at(Variable::h_xx).dx) +
        3.0 / 8.0 * (lambda_x * d.at(Variable::h_zz).value +
                     lambda * d.at(Variable::h_zz).dx) -
        5.0 / 4.0 * (lambda_x * d.at(Variable::h_ww).value +
                     lambda * d.at(Variable::h_ww).dx);
    d.at(Variable::A_ww).dx =
        -0.5 * (d.at(Variable::A_xx).dx + d.at(Variable::A_zz).dx +
                metric_trace_dx);
    return d;
}

Direction reflected(const Direction &positive)
{
    Direction negative;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const bool one_z = variable == Variable::h_xz ||
                           variable == Variable::A_xz ||
                           variable == Variable::hat_Gamma_z;
        const double parity = one_z ? -1.0 : 1.0;
        const auto &source = positive.at(variable);
        negative.at(variable) =
            {parity * source.value, parity * source.dx,
             -parity * source.dz, parity * source.dxx,
             -parity * source.dxz, parity * source.dzz};
    }
    return negative;
}

void check_background_and_pure_cases()
{
    const Direction zero;
    const auto background = evaluate_nonlinear(1.0, 2.0, 0.0, zero);
    std::cout << std::scientific << std::setprecision(12)
              << "INFO chi/h GP residual chi=" << background.total.value[0]
              << " hxx=" << background.total.value[1]
              << " hxz=" << background.total.value[2]
              << " hzz=" << background.total.value[3]
              << " hww=" << background.total.value[4] << "\n";
    require_true("analytic chi/h GP residuals at roundoff",
                 max_error(background.total, Rows{}) < 2.0e-15);
    require_true("zero chi/h JVP", max_error(assemble(1.0, 2.0, zero), Rows{}) == 0.0);

    Direction pure_k;
    pure_k.at(Variable::K).value = 0.7;
    require_true("pure K activates only chi",
                 std::abs(assemble(1.0, 2.0, pure_k).value[0] - 0.35) <
                     tolerance);
    Direction pure_a;
    pure_a.at(Variable::A_xx).value = 0.4;
    pure_a.at(Variable::A_zz).value = -0.4;
    const auto a_rows = assemble(1.0, 2.0, pure_a);
    require_close("pure trace-free A hxx", a_rows.value[1], -0.8);
    require_close("pure trace-free A hzz", a_rows.value[3], 0.8);
}

void check_families_mutations_and_scope()
{
    const double r0 = 1.0;
    const double x = 1.9;
    const auto direction = mixed_direction(r0, x);
    const auto oracle = central_difference(r0, x, 1.0e-5, direction);
    const auto production = production_families(r0, x, direction);
    const auto actual = assemble(r0, x, direction);
    const std::array<const char *, family_count> names = {
        "common advection", "tensor shift stretching", "algebraic"};
    for (std::size_t family = 0; family < family_count; ++family)
    {
        require_true(std::string("independent chi/h family ") + names[family],
                     max_error(production[family], oracle.family[family]) <
                         2.0e-8);
        require_true(std::string("active chi/h family ") + names[family],
                     max_error(oracle.family[family], Rows{}) > 1.0e-4);
        require_true(std::string("omission mutation ") + names[family],
                     max_error(actual,
                               subtract(actual, oracle.family[family])) >
                         1.0e-4);
        require_true(std::string("duplication mutation ") + names[family],
                     max_error(actual, add(actual, oracle.family[family])) >
                         1.0e-4);
    }
    Rows doubled_ww = actual;
    doubled_ww.value[4] += actual.value[4];
    require_true("representative hww duplication rejected",
                 max_error(actual, doubled_ww) > 1.0e-4);

    Vector full = make_state(Direction{});
    (void)assemble(r0, x, direction, nullptr, &full);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::chi && !Operator::is_metric_variable(variable))
        {
            require_close(std::string("chi/h illegal write guard ") +
                              Operator::variable_name(variable),
                          full.value(variable), 0.0, 0.0);
        }
    }

    std::array<double, Operator::frozen_gauge_state_vector.size()> zeros = {};
    const auto zero_advection =
        Operator::make_frozen_gauge_perturbation_vector(zeros);
    const auto without_advection = assemble(r0, x, direction, &zero_advection);
    require_true("chi/h assembler does not reimplement advection",
                 max_error(actual, without_advection) > 1.0e-4);
}

void check_epsilon_convergence()
{
    const double r0 = 0.9;
    const double x = 1.8;
    const std::array<double, 6> epsilons = {
        1.0e-2, 1.0e-3, 1.0e-4, 1.0e-5, 1.0e-6, 1.0e-7};
    const std::array<Direction, 2> directions = {
        mixed_direction(r0, x), second_mixed_direction(r0, x)};
    for (std::size_t selected = 0; selected < directions.size(); ++selected)
    {
        const auto actual = assemble(r0, x, directions[selected]);
        std::array<double, 6> errors = {};
        std::cout << "INFO chi/h epsilon table direction=" << selected + 1
                  << "\n";
        for (std::size_t i = 0; i < epsilons.size(); ++i)
        {
            const auto estimate =
                central_difference(r0, x, epsilons[i], directions[selected])
                    .total;
            errors[i] = max_error(actual, estimate);
            std::cout << "INFO eps=" << epsilons[i]
                      << " max_error=" << errors[i];
            for (std::size_t row = 0; row < row_count; ++row)
            {
                std::cout << " row" << row << "="
                          << std::abs(actual.value[row] - estimate.value[row]);
            }
            std::cout << "\n";
        }
        std::cout << "INFO chi/h convergence ratios " << errors[0] / errors[1]
                  << " " << errors[1] / errors[2] << "\n";
        require_true("chi/h second-order convergence",
                     errors[0] / errors[1] > 70.0 &&
                         errors[1] / errors[2] > 70.0);
        require_true("chi/h roundoff saturation",
                     errors[5] > std::min(errors[3], errors[4]));
    }
}

void check_parity_tangent_and_flags()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const auto direction = mixed_direction(r0, x);
    const auto positive = assemble(r0, x, direction);
    const auto negative = assemble(r0, x, reflected(direction));
    for (const std::size_t row : {std::size_t{0}, std::size_t{1},
                                  std::size_t{3}, std::size_t{4}})
    {
        require_close("chi/h scalar-even parity", positive.value[row],
                      negative.value[row]);
    }
    require_close("h_xz one-z parity", positive.value[2],
                  -negative.value[2]);

    const auto state = make_state(direction);
    require_close("metric determinant tangent input",
                  direction.at(Variable::h_xx).value +
                      direction.at(Variable::h_zz).value +
                      2.0 * direction.at(Variable::h_ww).value,
                  0.0, 2.0e-14);
    require_close("metric determinant tangent x derivative",
                  direction.at(Variable::h_xx).dx +
                      direction.at(Variable::h_zz).dx +
                      2.0 * direction.at(Variable::h_ww).dx,
                  0.0, 2.0e-14);
    require_close("A tangent input",
                  Operator::delta_trace_a_at_point(r0, x, state), 0.0,
                  2.0e-14);
    require_close("complete metric rows preserve determinant tangent",
                  positive.value[1] + positive.value[3] +
                      2.0 * positive.value[4],
                  0.0, 3.0e-12);
    const auto nonlinear = make_nonlinear_state(r0, x, 1.0e-2, direction);
    require_close("nonlinear conformal determinant convention",
                  (nonlinear.h[0][0] * nonlinear.h[1][1] -
                   nonlinear.h[0][1] * nonlinear.h[1][0]) *
                      nonlinear.h_ww * nonlinear.h_ww,
                  1.0, 3.0e-12);

    require_true("chi/metric family inventory closed",
                 Operator::chi_metric_surviving_term_family_inventory_closed);
    require_true("chi/metric assembly implemented",
                 Operator::chi_metric_final_row_assembly_implemented);
    require_true("chi/metric validation implemented",
                 Operator::chi_metric_assembled_row_validation_implemented);
    for (const auto variable : {Variable::chi, Variable::h_xx,
                                Variable::h_xz, Variable::h_zz,
                                Variable::h_ww})
    {
        require_true(std::string("row complete ") +
                         Operator::variable_name(variable),
                     Operator::variable_rhs_complete(variable));
    }
}
} // namespace

int main()
{
    check_background_and_pure_cases();
    check_families_mutations_and_scope();
    check_epsilon_convergence();
    check_parity_tangent_and_flags();
    std::cout << "PASS Stage 4AO-C complete frozen-gauge chi/metric rows\n";
    return 0;
}
