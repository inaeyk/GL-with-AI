#include "Stage4AOCAnalyticFullOracle.hpp"
#include "Stage4AOFrozenGaugeZDerivativeAdapter.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace
{
namespace Oracle = BlackStringToy::Stage4AOCAnalyticFullOracle;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Adapter = BlackStringToy::Stage4AOFrozenGaugeZDerivativeAdapter;
namespace Connection =
    BlackStringToy::Stage4AOFrozenGaugeContractedConnection;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciAssembly =
    BlackStringToy::Stage4AOFrozenGaugeRicciAssembly;
namespace HiddenRww = BlackStringToy::Stage4AOFrozenGaugeHiddenRww;
namespace VisibleRxx = BlackStringToy::Stage4AOFrozenGaugeVisibleRxx;
namespace VisibleRxz = BlackStringToy::Stage4AOFrozenGaugeVisibleRxz;
namespace VisibleRzz = BlackStringToy::Stage4AOFrozenGaugeVisibleRzz;

using Direction = Oracle::Direction;
using Jet = Oracle::Jet;
using Rows = std::array<double, Oracle::variables>;
using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

constexpr double oracle_tolerance = 4.0e-7;
constexpr double roundoff_tolerance = 4.0e-11;

void fail(const std::string &label, const std::string &details)
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
                   const double expected,
                   const double tolerance = roundoff_tolerance)
{
    if (std::abs(actual - expected) > tolerance)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected));
    }
}

Rows rows_from_vector(const Vector &input)
{
    Rows out = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        out[Operator::variable_index(variable)] = input.value(variable);
    }
    return out;
}

Rows rows_from_oracle(const Oracle::Rows &input)
{
    Rows out = {};
    for (std::size_t i = 0; i < out.size(); ++i)
    {
        out[i] = static_cast<double>(input[i]);
    }
    return out;
}

double max_norm(const Rows &input)
{
    double out = 0.0;
    for (const auto value : input)
    {
        out = std::max(out, std::abs(value));
    }
    return out;
}

double max_error(const Rows &left, const Rows &right)
{
    double out = 0.0;
    for (std::size_t i = 0; i < left.size(); ++i)
    {
        out = std::max(out, std::abs(left[i] - right[i]));
    }
    return out;
}

BlackStringToy::ConformalCartoonAlgebra::ConformalMetric
metric_from_direction(const Direction &direction)
{
    return {direction.at(Variable::h_xx).value,
            direction.at(Variable::h_xz).value,
            direction.at(Variable::h_zz).value,
            direction.at(Variable::h_ww).value};
}

BlackStringToy::ConformalCartoonAlgebra::ConformalMetric
metric_jet(const Direction &direction, const int derivative)
{
    const auto component = [derivative](const Jet &jet) {
        switch (derivative)
        {
        case 0:
            return jet.dx;
        case 1:
            return jet.dz;
        case 2:
            return jet.dxx;
        case 3:
            return jet.dxz;
        default:
            return jet.dzz;
        }
    };
    return {component(direction.at(Variable::h_xx)),
            component(direction.at(Variable::h_xz)),
            component(direction.at(Variable::h_zz)),
            component(direction.at(Variable::h_ww))};
}

Ricci::MetricDerivatives metric_derivatives(const Direction &direction)
{
    return {metric_jet(direction, 0), metric_jet(direction, 1),
            metric_jet(direction, 2), metric_jet(direction, 3),
            metric_jet(direction, 4)};
}

Ricci::ScalarDerivatives chi_derivatives(const Direction &direction)
{
    const auto &chi = direction.at(Variable::chi);
    return {{chi.dx, chi.dz}, {chi.dxx, chi.dxz, chi.dzz}};
}

RicciAssembly::TraceFreeRicciAssembly
make_geometric_ricci(const double x, const Direction &direction)
{
    const auto h = metric_from_direction(direction);
    const auto derivatives = metric_derivatives(direction);
    const auto chi = direction.at(Variable::chi).value;
    const auto dchi = chi_derivatives(direction);
    const auto rxx = VisibleRxx::compute_visible_physical_delta_rxx(
        VisibleRxx::make_visible_rxx_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rxz = VisibleRxz::compute_visible_physical_delta_rxz(
        VisibleRxz::make_visible_rxz_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rzz = VisibleRzz::compute_visible_physical_delta_rzz(
        VisibleRzz::make_visible_rzz_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rww = HiddenRww::compute_hidden_physical_delta_rww(
        HiddenRww::make_hidden_rww_perturbation(x, h, derivatives, chi,
                                                dchi));
    return RicciAssembly::assemble_trace_free_ricci(
        RicciAssembly::make_raw_ricci_components(rxx, rxz, rzz, rww));
}

Adapter::DerivativeAdapterResult make_adapter(const double x,
                                              const Direction &direction)
{
    const auto &hx = direction.at(Variable::hat_Gamma_x);
    const auto &hz = direction.at(Variable::hat_Gamma_z);
    return Adapter::compute_derivative_adapter(
        Adapter::make_derivative_adapter_input(
            x, metric_from_direction(direction), metric_derivatives(direction),
            hx.value, hz.value, hx.dx, hx.dz, hz.dx, hz.dz));
}

Connection::PerturbationJet make_connection_input(
    const double x, const Direction &direction)
{
    const auto &hxx = direction.at(Variable::h_xx);
    const auto &hxz = direction.at(Variable::h_xz);
    const auto &hzz = direction.at(Variable::h_zz);
    const auto &hww = direction.at(Variable::h_ww);
    return Connection::make_perturbation_jet(
        x, hxx.value, hxz.value, hzz.value, hww.value, hxx.dx, hxz.dx,
        hzz.dx, hww.dx, hxx.dz, hxz.dz, hzz.dz, hww.dz,
        direction.at(Variable::hat_Gamma_x).value,
        direction.at(Variable::hat_Gamma_z).value);
}

Vector make_state(const Direction &direction)
{
    Rows values = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] =
            direction.at(variable).value;
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector make_advection(const double r0, const double x,
                      const Direction &direction, const double factor = 1.0)
{
    Rows values = {};
    const double beta = std::sqrt(r0 / x);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] =
            factor * beta * direction.at(variable).dx;
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector assemble_vector(const double r0, const double x,
                       const Direction &direction,
                       const double advection_factor = 1.0)
{
    const auto adapter = make_adapter(x, direction);
    const auto encoded = Adapter::make_encoded_z_ricci_completion(x, adapter);
    return Operator::apply_complete_frozen_gauge_interior_operator_at_point(
        r0, make_state(direction),
        make_advection(r0, x, direction, advection_factor),
        make_geometric_ricci(x, direction), encoded,
        make_connection_input(x, direction), direction.at(Variable::K).dx,
        direction.at(Variable::K).dz,
        direction.at(Variable::Theta).dx,
        direction.at(Variable::Theta).dz,
        direction.at(Variable::chi).dx,
        direction.at(Variable::chi).dz);
}

Rows assemble(const double r0, const double x, const Direction &direction,
              const double advection_factor = 1.0)
{
    return rows_from_vector(
        assemble_vector(r0, x, direction, advection_factor));
}

Direction mixed_direction()
{
    Direction d;
    d.at(Variable::chi) = {0.13, 0.21, -0.16, -0.08, 0.12, 0.17};
    d.at(Variable::h_xx) = {0.31, -0.41, 0.23, 0.37, -0.19, 0.29};
    d.at(Variable::h_xz) = {-0.22, 0.19, -0.29, -0.33, 0.27, -0.24};
    d.at(Variable::h_zz) = {-0.17, 0.37, -0.11, 0.28, -0.31, 0.35};
    d.at(Variable::h_ww) = {0.28, -0.13, 0.07, -0.26, 0.18, -0.32};
    d.at(Variable::K) = {-0.27, 0.34, -0.29, 0.11, -0.07, 0.16};
    d.at(Variable::A_xx) = {0.11, -0.17, 0.13, -0.08, 0.06, 0.14};
    d.at(Variable::A_xz) = {-0.09, 0.14, -0.12, 0.09, -0.05, -0.11};
    d.at(Variable::A_zz) = {0.07, 0.16, -0.10, -0.12, 0.04, 0.08};
    d.at(Variable::A_ww) = {-0.05, -0.11, 0.08, 0.07, -0.03, -0.09};
    d.at(Variable::Theta) = {0.18, -0.26, 0.24, -0.06, 0.05, 0.12};
    d.at(Variable::hat_Gamma_x) = {0.23, 0.32, -0.21, -0.15, 0.24, 0.19};
    d.at(Variable::hat_Gamma_z) = {-0.19, -0.28, 0.26, 0.22, -0.17, -0.25};
    return d;
}

Direction second_mixed_direction()
{
    auto d = mixed_direction();
    for (std::size_t i = 0; i < d.data.size(); ++i)
    {
        const double sign = i % 2 == 0 ? -1.0 : 1.0;
        auto &jet = d.data[i];
        jet.value *= sign * 0.83;
        jet.dx *= -sign * 0.91;
        jet.dz *= sign * 1.07;
        jet.dxx *= -sign * 0.79;
        jet.dxz *= sign * 0.88;
        jet.dzz *= -sign * 0.94;
    }
    return d;
}

double intrinsic_parity(const Variable variable)
{
    return variable == Variable::h_xz || variable == Variable::A_xz ||
                   variable == Variable::hat_Gamma_z
               ? -1.0
               : 1.0;
}

Direction reflect(const Direction &input)
{
    Direction out;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const double p = intrinsic_parity(variable);
        const auto &source = input.at(variable);
        out.at(variable) = {p * source.value, p * source.dx,
                            -p * source.dz, p * source.dxx,
                            -p * source.dxz, p * source.dzz};
    }
    return out;
}

Direction sector_projection(const Direction &input, const double eigenvalue)
{
    const auto reflected = reflect(input);
    Direction out;
    for (std::size_t i = 0; i < out.data.size(); ++i)
    {
        const auto &a = input.data[i];
        const auto &b = reflected.data[i];
        out.data[i] = {0.5 * (a.value + eigenvalue * b.value),
                       0.5 * (a.dx + eigenvalue * b.dx),
                       0.5 * (a.dz + eigenvalue * b.dz),
                       0.5 * (a.dxx + eigenvalue * b.dxx),
                       0.5 * (a.dxz + eigenvalue * b.dxz),
                       0.5 * (a.dzz + eigenvalue * b.dzz)};
    }
    return out;
}

Rows reflect_rows(const Rows &input)
{
    Rows out = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto i = Operator::variable_index(variable);
        out[i] = intrinsic_parity(variable) * input[i];
    }
    return out;
}

void check_background_and_zero(const double r0, const double x)
{
    const Direction zero;
    const auto background = Oracle::evaluate(r0, x, 0.0, zero);
    std::cout << std::scientific << std::setprecision(12)
              << "INFO full analytic GP background residuals\n";
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto i = Operator::variable_index(variable);
        std::cout << "  " << Operator::variable_name(variable) << " "
                  << static_cast<double>(background.total[i]) << "\n";
    }
    require_true("all 13 analytic GP residuals at roundoff",
                 max_norm(rows_from_oracle(background.total)) < 5.0e-16);
    require_true("zero perturbation gives zero full JVP",
                 max_norm(assemble(r0, x, zero)) == 0.0);
}

void check_epsilon_convergence(const double r0, const double x,
                               const Direction &direction,
                               const std::string &name)
{
    constexpr std::array<double, 6> epsilons = {
        1.0e-2, 1.0e-3, 1.0e-4, 1.0e-5, 1.0e-6, 1.0e-7};
    const auto production = assemble(r0, x, direction);
    std::array<Rows, epsilons.size()> errors = {};
    std::cout << "INFO full epsilon table " << name << "\n";
    std::cout << "  epsilon";
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        std::cout << " " << Operator::variable_name(variable);
    }
    std::cout << " max\n";
    for (std::size_t e = 0; e < epsilons.size(); ++e)
    {
        const auto oracle = rows_from_oracle(
            Oracle::central_difference(r0, x, epsilons[e], direction).total);
        for (std::size_t i = 0; i < production.size(); ++i)
        {
            errors[e][i] = std::abs(production[i] - oracle[i]);
        }
        std::cout << "  " << epsilons[e];
        for (const auto error : errors[e])
        {
            std::cout << " " << error;
        }
        std::cout << " " << max_norm(errors[e]) << "\n";
    }
    for (std::size_t i = 0; i < production.size(); ++i)
    {
        if (errors[0][i] < 1.0e-12 && errors[1][i] < 1.0e-12 &&
            errors[2][i] < 1.0e-12)
        {
            require_true(name + " row " + std::to_string(i) +
                             " exact-linear central difference at roundoff",
                         errors[3][i] < 2.0e-11 &&
                             errors[4][i] < 2.0e-11 &&
                             errors[5][i] < 2.0e-11);
            continue;
        }
        const double ratio_12 = errors[0][i] / errors[1][i];
        const double ratio_23 = errors[1][i] / errors[2][i];
        require_true(name + " row " + std::to_string(i) +
                         " second-order central difference",
                     ratio_12 > 45.0 && ratio_12 < 160.0 &&
                         ratio_23 > 35.0 && ratio_23 < 220.0);
    }
    require_true(name + " 1e-5 nonlinear JVP agrees with production",
                 max_norm(errors[3]) < oracle_tolerance);
    require_true(name + " roundoff saturation follows convergence",
                 max_norm(errors[5]) > 0.1 * max_norm(errors[4]));
}

void check_family_activation_and_ownership(const double r0, const double x,
                                           const Direction &direction)
{
    const auto nonlinear = Oracle::central_difference(r0, x, 1.0e-5,
                                                       direction);
    for (std::size_t family = 0; family < Oracle::family_count; ++family)
    {
        require_true(std::string("mixed direction activates ") +
                         Oracle::family_names[family],
                     max_norm(rows_from_oracle(nonlinear.family[family])) >
                         1.0e-8);
    }

    const auto state = make_state(direction);
    const auto advection = make_advection(r0, x, direction);
    const auto geometric = make_geometric_ricci(x, direction);
    const auto adapter = make_adapter(x, direction);
    const auto encoded = Adapter::make_encoded_z_ricci_completion(x, adapter);
    const auto connection = make_connection_input(x, direction);
    const auto chi_metric = Operator::apply_complete_chi_metric_rows_at_point(
        r0, x, state, advection);
    const auto k_theta_a = Operator::apply_complete_k_theta_a_rows_at_point(
        r0, x, state, advection, geometric, encoded);
    const auto gamma = Operator::apply_complete_hat_gamma_rows_at_point(
        r0, state, advection, connection, direction.at(Variable::K).dx,
        direction.at(Variable::K).dz,
        direction.at(Variable::Theta).dx,
        direction.at(Variable::Theta).dz, direction.at(Variable::chi).dx,
        direction.at(Variable::chi).dz);
    const auto full = assemble_vector(r0, x, direction);
    const auto normal = rows_from_vector(full);
    double owner_amplitudes[3] = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const bool chi_owner =
            Operator::receives_chi_metric_complete_row_assembly(variable);
        const bool kta_owner =
            Operator::receives_k_theta_a_complete_row_assembly(variable);
        const auto expected = chi_owner ? chi_metric.value(variable)
                              : kta_owner ? k_theta_a.value(variable)
                                          : gamma.value(variable);
        require_close(std::string("full slot owner ") +
                          Operator::variable_name(variable),
                      full.value(variable), expected, 0.0);
        owner_amplitudes[chi_owner ? 0 : kta_owner ? 1 : 2] =
            std::max(owner_amplitudes[chi_owner ? 0 : kta_owner ? 1 : 2],
                     std::abs(expected));
        for (int owner = 0; owner < 3; ++owner)
        {
            const bool owns = owner == 0 ? chi_owner
                              : owner == 1 ? kta_owner
                                           : !chi_owner && !kta_owner;
            const double candidate = owner == 0 ? chi_metric.value(variable)
                                     : owner == 1 ? k_theta_a.value(variable)
                                                  : gamma.value(variable);
            if (!owns)
            {
                require_close("complete-row owner has no illegal output write",
                              candidate, 0.0, 0.0);
            }
        }
    }
    for (int owner = 0; owner < 3; ++owner)
    {
        auto omitted = normal;
        auto duplicated = normal;
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            const bool chi_owner =
                Operator::receives_chi_metric_complete_row_assembly(variable);
            const bool kta_owner =
                Operator::receives_k_theta_a_complete_row_assembly(variable);
            const int variable_owner = chi_owner ? 0 : kta_owner ? 1 : 2;
            if (variable_owner != owner)
            {
                continue;
            }
            const auto i = Operator::variable_index(variable);
            omitted[i] -= normal[i];
            duplicated[i] += normal[i];
        }
        require_true("complete-row owner omission mutation rejected",
                     owner_amplitudes[owner] > 1.0e-8 &&
                         max_error(normal, omitted) > 1.0e-8);
        require_true("complete-row owner duplication mutation rejected",
                     owner_amplitudes[owner] > 1.0e-8 &&
                         max_error(normal, duplicated) > 1.0e-8);
    }

    const auto missing_advection = assemble(r0, x, direction, 0.0);
    const auto doubled_advection = assemble(r0, x, direction, 2.0);
    require_true("absent common advection rejected",
                 max_error(normal, missing_advection) > 1.0e-8);
    require_true("duplicate common advection rejected",
                 max_error(normal, doubled_advection) > 1.0e-8);

    const auto wrong_hidden = rows_from_oracle(
        Oracle::central_difference(r0, x, 1.0e-5, direction, 1).total);
    require_true("wrong hidden multiplicity rejected",
                 max_error(normal, wrong_hidden) > 1.0e-5);
    auto doubled_hww = normal;
    doubled_hww[Operator::variable_index(Variable::h_ww)] *= 2.0;
    auto doubled_aww = normal;
    doubled_aww[Operator::variable_index(Variable::A_ww)] *= 2.0;
    require_true("representative hww duplication rejected",
                 max_error(normal, doubled_hww) > 1.0e-8);
    require_true("representative Aww duplication rejected",
                 max_error(normal, doubled_aww) > 1.0e-8);
}

Direction tangent_direction(const double r0, const double x)
{
    Direction d;
    d.at(Variable::h_xx) = {0.20, -0.14, 0.0, 0.08, 0.0, -0.03};
    d.at(Variable::h_zz) = {-0.10, 0.06, 0.0, -0.04, 0.0, 0.02};
    d.at(Variable::h_ww) = {-0.05, 0.04, 0.0, -0.02, 0.0, 0.005};
    d.at(Variable::A_xx).value = 0.07;
    d.at(Variable::A_zz).value = -0.03;
    d.at(Variable::A_xx).dx = -0.09;
    d.at(Variable::A_zz).dx = 0.04;
    const double lambda = Operator::lambda_gp(r0, x);
    const double lambda_x = -1.5 * lambda / x;
    const auto metric_term = [&d, lambda]() {
        return 0.875 * lambda * d.at(Variable::h_xx).value +
               0.375 * lambda * d.at(Variable::h_zz).value -
               1.25 * lambda * d.at(Variable::h_ww).value;
    }();
    d.at(Variable::A_ww).value =
        -0.5 * (d.at(Variable::A_xx).value +
                d.at(Variable::A_zz).value + metric_term);
    const double metric_term_x =
        0.875 * (lambda_x * d.at(Variable::h_xx).value +
                 lambda * d.at(Variable::h_xx).dx) +
        0.375 * (lambda_x * d.at(Variable::h_zz).value +
                 lambda * d.at(Variable::h_zz).dx) -
        1.25 * (lambda_x * d.at(Variable::h_ww).value +
                lambda * d.at(Variable::h_ww).dx);
    d.at(Variable::A_ww).dx =
        -0.5 * (d.at(Variable::A_xx).dx + d.at(Variable::A_zz).dx +
                metric_term_x);
    return d;
}

void check_tangent_identities(const double r0, const double x)
{
    const auto direction = tangent_direction(r0, x);
    const auto input = make_state(direction);
    require_close("metric tangent input",
                  direction.at(Variable::h_xx).value +
                      direction.at(Variable::h_zz).value +
                      2.0 * direction.at(Variable::h_ww).value,
                  0.0, 2.0e-17);
    require_close("metric tangent x derivative",
                  direction.at(Variable::h_xx).dx +
                      direction.at(Variable::h_zz).dx +
                      2.0 * direction.at(Variable::h_ww).dx,
                  0.0, 2.0e-17);
    require_true("metric contribution to A tangent is nonzero",
                 std::abs(Operator::delta_trace_a_at_point(r0, x, input) -
                          (input.value(Variable::A_xx) +
                           input.value(Variable::A_zz) +
                           2.0 * input.value(Variable::A_ww))) >
                     1.0e-4);
    require_close("full metric-inclusive A tangent input",
                  Operator::delta_trace_a_at_point(r0, x, input), 0.0,
                  2.0e-16);
    const auto output = assemble_vector(r0, x, direction);
    const double metric_rhs_trace = output.value(Variable::h_xx) +
                                    output.value(Variable::h_zz) +
                                    2.0 * output.value(Variable::h_ww);
    const double a_tangent_rhs =
        Operator::delta_trace_a_at_point(r0, x, output);
    std::cout << "INFO full tangent input="
              << Operator::delta_trace_a_at_point(r0, x, input)
              << " metric_rhs_trace=" << metric_rhs_trace
              << " A_tangent_rhs=" << a_tangent_rhs << "\n";
    require_close("complete metric determinant tangent output",
                  metric_rhs_trace, 0.0, 3.0e-15);
    require_close("complete metric-inclusive A tangent output",
                  a_tangent_rhs, 0.0, 4.0e-15);
}

void check_parity_sectors(const double r0, const double x)
{
    const auto direction = mixed_direction();
    const auto reflected = reflect(direction);
    const auto output = assemble(r0, x, direction);
    const auto reflected_output = assemble(r0, x, reflected);
    const double commutator =
        max_error(reflected_output, reflect_rows(output));
    require_true("full operator commutes with z reflection",
                 commutator < roundoff_tolerance);

    const auto even_sector = assemble(r0, x, sector_projection(direction, 1.0));
    const auto odd_sector = assemble(r0, x, sector_projection(direction, -1.0));
    double even_to_odd_leakage = 0.0;
    double odd_to_even_leakage = 0.0;
    double even_amplitude = 0.0;
    double odd_amplitude = 0.0;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto i = Operator::variable_index(variable);
        if (intrinsic_parity(variable) > 0.0)
        {
            even_amplitude = std::max(even_amplitude, std::abs(even_sector[i]));
            odd_to_even_leakage =
                std::max(odd_to_even_leakage, std::abs(odd_sector[i]));
        }
        else
        {
            odd_amplitude = std::max(odd_amplitude, std::abs(odd_sector[i]));
            even_to_odd_leakage =
                std::max(even_to_odd_leakage, std::abs(even_sector[i]));
        }
    }
    std::cout << "INFO parity sectors P+={chi,hxx,hzz,hww,K,Axx,Azz,Aww,"
                 "Theta,Gamma_x}; P-={hxz,Axz,Gamma_z}\n"
              << "INFO parity leakage P+->P-=" << even_to_odd_leakage
              << " P-->P+=" << odd_to_even_leakage
              << " commutator=" << commutator << "\n";
    require_true("even parity sector activated", even_amplitude > 1.0e-6);
    require_true("one-z parity sector activated", odd_amplitude > 1.0e-6);
    require_true("forbidden even-to-one-z leakage at roundoff",
                 even_to_odd_leakage < roundoff_tolerance);
    require_true("forbidden one-z-to-even leakage at roundoff",
                 odd_to_even_leakage < roundoff_tolerance);
}

void check_gates()
{
    const auto contract =
        Operator::make_default_frozen_gauge_operator_contract();
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        require_true(std::string("variable RHS complete: ") +
                         Operator::variable_name(variable),
                     contract.rhs_complete(variable));
    }
    require_true("full frozen-gauge interior assembly gate open",
                 contract.complete_interior_operator_implemented());
    require_true("full interior JVP gate open",
                 contract.full_interior_jvp_validated());
    require_true("full interior parity gate open",
                 contract.full_interior_parity_validated());
    require_true("boundary-bearing complete operator remains closed",
                 !contract.complete_operator_implemented());
    require_true("radial boundary gate remains closed",
                 !Operator::boundary_derivative_validation_implemented);
    require_true("eigensolver gate remains closed",
                 !contract.eigensolver_allowed());
    require_true("Stage 4AO-D remains closed",
                 !Operator::live_gauge_stage_4ao_d_implemented);
}
} // namespace

int main()
{
    constexpr double r0 = 1.0;
    constexpr double x = 2.0;
    check_background_and_zero(r0, x);
    check_epsilon_convergence(r0, x, mixed_direction(), "mixed direction 1");
    check_epsilon_convergence(r0, x, second_mixed_direction(),
                              "mixed direction 2");
    check_family_activation_and_ownership(r0, x, mixed_direction());
    check_family_activation_and_ownership(r0, x, second_mixed_direction());
    check_tangent_identities(r0, x);
    check_parity_sectors(r0, x);
    check_gates();
    std::cout << "PASS Stage 4AO-C full 13-variable interior operator\n";
    return 0;
}
