#include "BlackStringTargetCleanupConstraintsSource.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"
#include "Stage4AOCAnalyticFullOracle.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
namespace Seam = BlackStringTargetCleanupConstraintsSource;
namespace Target = BlackStringTargetCCZ4Pointwise;
namespace Reduced = BlackStringReducedVars;
namespace Production = BlackStringProductionVariables;
namespace Oracle = BlackStringToy::Stage4AOCAnalyticFullOracle;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

constexpr double r0 = 1.7;
constexpr double x = 1.35;
constexpr double absolute_tolerance = 5.0e-13;
constexpr double relative_tolerance = 5.0e-12;

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "FAIL " << message << "\n";
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

double allowance(const double left, const double right)
{
    return absolute_tolerance +
           relative_tolerance *
               std::max(std::abs(left), std::abs(right));
}

void require_close(const double actual, const double expected,
                   const std::string &message)
{
    require(std::abs(actual - expected) <= allowance(actual, expected),
            message);
}

Production::VariableSlot
production_slot(const Operator::PerturbationVariable variable)
{
    using Variable = Operator::PerturbationVariable;
    switch (variable)
    {
    case Variable::chi:
        return Production::c_chi;
    case Variable::h_xx:
        return Production::c_hxx;
    case Variable::h_xz:
        return Production::c_hxz;
    case Variable::h_zz:
        return Production::c_hzz;
    case Variable::h_ww:
        return Production::c_hww;
    case Variable::K:
        return Production::c_K;
    case Variable::A_xx:
        return Production::c_Axx;
    case Variable::A_xz:
        return Production::c_Axz;
    case Variable::A_zz:
        return Production::c_Azz;
    case Variable::A_ww:
        return Production::c_Aww;
    case Variable::Theta:
        return Production::c_Theta;
    case Variable::hat_Gamma_x:
        return Production::c_GammaX;
    case Variable::hat_Gamma_z:
        return Production::c_GammaZ;
    }
    throw std::logic_error("unreachable physical variable");
}

Target::Input make_input(const double amplitude,
                         const Oracle::Direction &direction)
{
    Target::Input input{};
    const Oracle::Context context{r0, x, amplitude, &direction,
                                  Oracle::hidden_multiplicity};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto analytic = Oracle::state_jet(context, variable);
        const auto slot = production_slot(variable);
        Reduced::component(input.vars, slot) =
            static_cast<double>(analytic.value);
        input.derivatives[static_cast<std::size_t>(slot)] = {
            static_cast<double>(analytic.dx),
            static_cast<double>(analytic.dz),
            static_cast<double>(analytic.dxx),
            static_cast<double>(analytic.dxz),
            static_cast<double>(analytic.dzz)};
    }
    const auto metadata =
        BlackStringGPPointwiseInitialData::make_analytic_metadata(r0, x);
    input.vars.gauge.lapse = 1.0;
    input.vars.gauge.shift = {metadata.beta_x.value, 0.0};
    input.vars.gauge.B = {0.0, 0.0};
    input.derivatives[Production::c_shiftX] = {
        metadata.beta_x.dx, 0.0, metadata.beta_x.dxx, 0.0, 0.0};
    input.x = x;
    return input;
}

void set_jet(Oracle::Direction &direction,
             const Operator::PerturbationVariable variable,
             const Oracle::Jet &jet)
{
    direction.at(variable) = jet;
}

struct ConstraintCase
{
    std::string name;
    double amplitude;
    Oracle::Direction direction;
};

Oracle::Direction make_fourier_direction(const bool p_plus,
                                         const double phase)
{
    Oracle::Direction direction;
    constexpr double k = 0.83;
    using Variable = Operator::PerturbationVariable;
    for (std::size_t index = 0;
         index < Operator::frozen_gauge_state_vector.size(); ++index)
    {
        const auto variable = Operator::frozen_gauge_state_vector[index];
        const bool odd =
            variable == Variable::h_xz || variable == Variable::A_xz ||
            variable == Variable::hat_Gamma_z;
        const double amplitude = 0.04 + 0.006 * static_cast<double>(index);
        const double radial_first = -0.7 * amplitude;
        const double radial_second = 0.45 * amplitude;
        const bool cosine = p_plus ? !odd : odd;
        const double trig = cosine ? std::cos(phase) : std::sin(phase);
        const double dz_trig =
            cosine ? -k * std::sin(phase) : k * std::cos(phase);
        set_jet(direction, variable,
                {amplitude * trig, radial_first * trig,
                 amplitude * dz_trig, radial_second * trig,
                 radial_first * dz_trig,
                 -k * k * amplitude * trig});
    }
    return direction;
}

std::vector<ConstraintCase> make_constraint_cases()
{
    using Variable = Operator::PerturbationVariable;
    std::vector<ConstraintCase> cases;
    cases.push_back({"exact_gp", 0.0, {}});

    Oracle::Direction flat;
    set_jet(flat, Variable::chi, {0.08, 0.0, 0.0, 0.0, 0.0, 0.0});
    set_jet(flat, Variable::K, {-0.11, 0.0, 0.0, 0.0, 0.0, 0.0});
    set_jet(flat, Variable::A_xx, {0.05, 0.0, 0.0, 0.0, 0.0, 0.0});
    cases.push_back({"flat_nontrivial", 0.04, flat});

    Oracle::Direction diagonal;
    set_jet(diagonal, Variable::chi,
            {0.04, -0.07, 0.03, 0.09, -0.02, 0.05});
    set_jet(diagonal, Variable::h_xx,
            {0.06, 0.08, -0.04, -0.03, 0.025, 0.07});
    set_jet(diagonal, Variable::h_zz,
            {-0.03, 0.05, 0.02, 0.06, -0.015, -0.04});
    set_jet(diagonal, Variable::K,
            {0.02, -0.05, 0.04, 0.01, 0.03, -0.02});
    set_jet(diagonal, Variable::A_zz,
            {0.07, 0.03, -0.02, -0.05, 0.04, 0.06});
    cases.push_back({"diagonal_curved", 0.05, diagonal});

    Oracle::Direction off_diagonal;
    set_jet(off_diagonal, Variable::h_xz,
            {0.09, -0.04, 0.07, 0.05, -0.06, 0.03});
    set_jet(off_diagonal, Variable::A_xz,
            {-0.08, 0.06, -0.05, -0.04, 0.07, -0.02});
    cases.push_back({"off_diagonal", 0.035, off_diagonal});

    Oracle::Direction hidden;
    set_jet(hidden, Variable::h_ww,
            {0.12, -0.08, 0.05, 0.07, -0.04, 0.09});
    set_jet(hidden, Variable::A_ww,
            {-0.10, 0.06, -0.03, -0.05, 0.02, -0.08});
    cases.push_back({"pure_hidden", 0.045, hidden});

    Oracle::Direction mixed;
    for (std::size_t index = 0;
         index < Operator::frozen_gauge_state_vector.size(); ++index)
    {
        const auto variable = Operator::frozen_gauge_state_vector[index];
        const double scale = 0.01 * static_cast<double>(index + 1);
        set_jet(mixed, variable,
                {scale, -0.7 * scale, 0.5 * scale, 0.9 * scale,
                 -0.4 * scale, 0.6 * scale});
    }
    cases.push_back({"mixed_all_family", 0.025, mixed});
    for (const bool p_plus : {true, false})
    {
        for (const double phase : {0.29, 0.91, 1.73})
        {
            cases.push_back(
                {std::string(p_plus ? "P_plus_" : "P_minus_") +
                     std::to_string(phase),
                 0.018, make_fourier_direction(p_plus, phase)});
        }
    }
    return cases;
}

Target::Input make_non_trace_free_flat_input()
{
    Target::Input input{};
    input.x = x;
    input.vars.physical.chi = 1.0;
    input.vars.physical.h = {1.0, 0.0, 1.0, 1.0};
    input.vars.physical.K = 0.63;
    input.vars.physical.A = {0.31, 0.08, -0.11, 0.19};
    input.vars.physical.Theta = 0.0;
    input.vars.physical.Gamma = {0.0, 0.0};
    input.vars.gauge.lapse = 1.0;
    input.vars.gauge.shift = {0.0, 0.0};
    input.vars.gauge.B = {0.0, 0.0};
    input.derivatives[Production::c_K] =
        {0.0, 0.0, 0.0, 0.0, 0.0};
    input.derivatives[Production::c_Axx] =
        {0.04, -0.03, 0.0, 0.0, 0.0};
    input.derivatives[Production::c_Axz] =
        {-0.02, 0.05, 0.0, 0.0, 0.0};
    input.derivatives[Production::c_Azz] =
        {0.03, -0.01, 0.0, 0.0, 0.0};
    input.derivatives[Production::c_Aww] =
        {-0.06, 0.04, 0.0, 0.0, 0.0};
    return input;
}

Oracle::AnalyticJet oracle_jet(const Target::Input &input,
                              const Production::VariableSlot slot)
{
    const auto &derivative =
        input.derivatives[static_cast<std::size_t>(slot)];
    return {static_cast<Oracle::Real>(
                Reduced::component(input.vars, slot)),
            static_cast<Oracle::Real>(derivative.dx),
            static_cast<Oracle::Real>(derivative.dz),
            static_cast<Oracle::Real>(derivative.dxx),
            static_cast<Oracle::Real>(derivative.dxz),
            static_cast<Oracle::Real>(derivative.dzz)};
}

Oracle::Tensor
oracle_d1_chris_contracted(const Oracle::Geometry &conformal)
{
    Oracle::Tensor result = {};
    for (int component = 0; component < Oracle::dimension; ++component)
    {
        for (int derivative = 0; derivative < Oracle::dimension;
             ++derivative)
        {
            for (int m = 0; m < Oracle::dimension; ++m)
            {
                for (int n = 0; n < Oracle::dimension; ++n)
                {
                    for (int p = 0; p < Oracle::dimension; ++p)
                    {
                        Oracle::Real first_derivative_terms = 0.0L;
                        for (int q = 0; q < Oracle::dimension; ++q)
                        {
                            for (int r = 0; r < Oracle::dimension; ++r)
                            {
                                first_derivative_terms -=
                                    conformal.inverse[q][r] *
                                    (conformal.first[derivative][n][q] *
                                         conformal.first[r][m][p] +
                                     conformal.first[derivative][m][n] *
                                         conformal.first[r][p][q]);
                            }
                        }
                        result[component][derivative] +=
                            conformal.inverse[component][m] *
                            conformal.inverse[n][p] *
                            (conformal.second[derivative][p][m][n] +
                             first_derivative_terms);
                    }
                }
            }
        }
    }
    return result;
}

Oracle::Real oracle_grchombo_ricci_scalar(
    const Oracle::Geometry &conformal,
    const Oracle::AnalyticJet &chi, const Oracle::Real radial_x,
    const Target::Input &input)
{
    const auto contracted = Oracle::contracted_connection(conformal);
    const auto d1_contracted = oracle_d1_chris_contracted(conformal);
    const auto gamma_x = oracle_jet(input, Production::c_GammaX);
    const auto gamma_z = oracle_jet(input, Production::c_GammaZ);
    const Oracle::Vector4 hatted = {
        gamma_x.value, gamma_z.value, 0.0L, 0.0L};
    std::array<Oracle::Vector4, Oracle::dimension> hatted_first = {};
    hatted_first[0] = {gamma_x.dx, gamma_z.dx, 0.0L, 0.0L};
    hatted_first[1] = {gamma_x.dz, gamma_z.dz, 0.0L, 0.0L};
    hatted_first[2][2] = gamma_x.value / radial_x;
    hatted_first[3][3] = gamma_x.value / radial_x;

    Oracle::Christoffel lower = {};
    Oracle::Christoffel lower_lower_upper = {};
    for (int i = 0; i < Oracle::dimension; ++i)
    {
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            for (int k = 0; k < Oracle::dimension; ++k)
            {
                lower[i][j][k] =
                    0.5L * (conformal.first[j][i][k] +
                            conformal.first[k][i][j] -
                            conformal.first[i][j][k]);
            }
        }
    }
    for (int i = 0; i < Oracle::dimension; ++i)
    {
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            for (int k = 0; k < Oracle::dimension; ++k)
            {
                for (int l = 0; l < Oracle::dimension; ++l)
                {
                    lower_lower_upper[i][j][k] +=
                        conformal.inverse[k][l] * lower[i][j][l];
                }
            }
        }
    }

    const Oracle::Vector4 chi_first = {chi.dx, chi.dz, 0.0L, 0.0L};
    Oracle::Tensor chi_second = {};
    chi_second[0][0] = chi.dxx;
    chi_second[0][1] = chi_second[1][0] = chi.dxz;
    chi_second[1][1] = chi.dzz;
    chi_second[2][2] = chi_second[3][3] = chi.dx / radial_x;
    Oracle::Tensor covariant_chi_second = {};
    Oracle::Real box_chi = 0.0L;
    Oracle::Real chi_gradient_squared = 0.0L;
    for (int i = 0; i < Oracle::dimension; ++i)
    {
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            covariant_chi_second[i][j] = chi_second[i][j];
            for (int k = 0; k < Oracle::dimension; ++k)
            {
                covariant_chi_second[i][j] -=
                    conformal.gamma[k][i][j] * chi_first[k];
            }
            box_chi += conformal.inverse[i][j] *
                       covariant_chi_second[i][j];
            chi_gradient_squared += conformal.inverse[i][j] *
                                    chi_first[i] * chi_first[j];
        }
    }

    Oracle::Tensor ricci = {};
    for (int i = 0; i < Oracle::dimension; ++i)
    {
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            Oracle::Real ricci_hat = 0.0L;
            for (int k = 0; k < Oracle::dimension; ++k)
            {
                ricci_hat +=
                    0.5L *
                    (conformal.metric[k][i] * hatted_first[j][k] +
                     conformal.metric[k][j] * hatted_first[i][k]);
                ricci_hat +=
                    0.5L * hatted[k] * conformal.first[k][i][j];
                for (int l = 0; l < Oracle::dimension; ++l)
                {
                    ricci_hat +=
                        -0.5L * conformal.inverse[k][l] *
                            conformal.second[k][l][i][j] +
                        conformal.gamma[k][l][i] *
                            lower_lower_upper[j][k][l] +
                        conformal.gamma[k][l][j] *
                            lower_lower_upper[i][k][l] +
                        conformal.gamma[k][i][l] *
                            lower_lower_upper[k][j][l];
                }
            }
            const Oracle::Real ricci_chi =
                0.5L *
                (2.0L * covariant_chi_second[i][j] +
                 conformal.metric[i][j] * box_chi -
                 (2.0L * chi_first[i] * chi_first[j] +
                  4.0L * conformal.metric[i][j] *
                      chi_gradient_squared) /
                     (2.0L * chi.value));
            ricci[i][j] =
                (ricci_chi + chi.value * ricci_hat) / chi.value;
            for (int m = 0; m < Oracle::dimension; ++m)
            {
                ricci[i][j] +=
                    0.5L *
                    (conformal.metric[m][i] *
                         (d1_contracted[m][j] -
                          hatted_first[j][m]) +
                     conformal.metric[m][j] *
                         (d1_contracted[m][i] -
                          hatted_first[i][m]) +
                     (contracted[m] - hatted[m]) *
                         conformal.first[m][i][j]);
            }
        }
    }
    Oracle::Real scalar = 0.0L;
    for (int i = 0; i < Oracle::dimension; ++i)
    {
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            scalar += chi.value * conformal.inverse[i][j] * ricci[i][j];
        }
    }
    return scalar;
}

Oracle::Real oracle_A_squared(const Oracle::Geometry &conformal,
                              const Oracle::AxisTensorJets &a_jets)
{
    Oracle::Real result = 0.0L;
    for (int i = 0; i < Oracle::dimension; ++i)
    {
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            for (int m = 0; m < Oracle::dimension; ++m)
            {
                for (int n = 0; n < Oracle::dimension; ++n)
                {
                    result += conformal.inverse[i][m] *
                              conformal.inverse[j][n] *
                              a_jets.value[i][j] *
                              a_jets.value[m][n];
                }
            }
        }
    }
    return result;
}

Seam::ConstraintValues oracle_constraints(const Target::Input &input)
{
    const auto chi = oracle_jet(input, Production::c_chi);
    const auto conformal_jets = Oracle::lift_axis_tensor(
        static_cast<Oracle::Real>(input.x),
        oracle_jet(input, Production::c_hxx),
        oracle_jet(input, Production::c_hxz),
        oracle_jet(input, Production::c_hzz),
        oracle_jet(input, Production::c_hww));
    const auto conformal = Oracle::compute_geometry(conformal_jets);
    const auto a_jets = Oracle::lift_axis_tensor(
        static_cast<Oracle::Real>(input.x),
        oracle_jet(input, Production::c_Axx),
        oracle_jet(input, Production::c_Axz),
        oracle_jet(input, Production::c_Azz),
        oracle_jet(input, Production::c_Aww));
    const auto k_jet = oracle_jet(input, Production::c_K);

    Oracle::Real scalar_ricci = 0.0L;
    const Oracle::Real A_squared = oracle_A_squared(conformal, a_jets);
    scalar_ricci = oracle_grchombo_ricci_scalar(
        conformal, chi, static_cast<Oracle::Real>(input.x), input);

    Oracle::Christoffel covd_A = {};
    for (int derivative = 0; derivative < Oracle::dimension; ++derivative)
    {
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            for (int k = 0; k < Oracle::dimension; ++k)
            {
                covd_A[derivative][j][k] =
                    a_jets.first[derivative][j][k];
                for (int l = 0; l < Oracle::dimension; ++l)
                {
                    covd_A[derivative][j][k] -=
                        conformal.gamma[l][derivative][j] *
                            a_jets.value[l][k] +
                        conformal.gamma[l][derivative][k] *
                            a_jets.value[l][j];
                }
            }
        }
    }
    std::array<Oracle::Real, 2> momentum = {};
    for (int i = 0; i < 2; ++i)
    {
        momentum[i] =
            -0.75L * (i == 0 ? k_jet.dx : k_jet.dz);
        for (int j = 0; j < Oracle::dimension; ++j)
        {
            for (int k = 0; k < Oracle::dimension; ++k)
            {
                momentum[i] +=
                    conformal.inverse[j][k] *
                    (covd_A[k][j][i] -
                     static_cast<Oracle::Real>(Oracle::dimension) *
                         a_jets.value[i][j] *
                         (k == 0 ? chi.dx
                                 : k == 1 ? chi.dz : 0.0L) /
                         (2.0L * std::max<Oracle::Real>(1.0e-6L,
                                                       chi.value)));
            }
        }
    }
    return {static_cast<double>(scalar_ricci +
                                0.75L * k_jet.value * k_jet.value -
                                A_squared),
            {static_cast<double>(momentum[0]),
             static_cast<double>(momentum[1])}};
}

struct Statistics
{
    double max_absolute = 0.0;
    double max_normalized = 0.0;
    std::string worst_case;
    std::string worst_output;

    void observe(const double actual, const double expected,
                 const std::string &case_name,
                 const std::string &output_name)
    {
        const double absolute = std::abs(actual - expected);
        const double normalized = absolute / allowance(actual, expected);
        if (absolute > max_absolute)
        {
            max_absolute = absolute;
        }
        if (normalized > max_normalized)
        {
            max_normalized = normalized;
            worst_case = case_name;
            worst_output = output_name;
        }
        require(normalized <= 1.0,
                case_name + "." + output_name +
                    " exceeds locked tolerance actual=" +
                    std::to_string(actual) +
                    " expected=" + std::to_string(expected) +
                    " normalized=" + std::to_string(normalized));
    }
};

Statistics test_constraints()
{
    Statistics stats;
    for (const auto &test_case : make_constraint_cases())
    {
        const auto input =
            make_input(test_case.amplitude, test_case.direction);
        const auto actual = Seam::evaluate_constraints(input);
        const auto expected = oracle_constraints(input);
        stats.observe(actual.target_total.hamiltonian,
                      expected.hamiltonian, test_case.name, "Hamiltonian");
        stats.observe(actual.target_total.momentum[0],
                      expected.momentum[0], test_case.name, "Mx");
        stats.observe(actual.target_total.momentum[1],
                      expected.momentum[1], test_case.name, "Mz");
        require_close(
            actual.hidden_sensitive_increment.hamiltonian,
            actual.target_total.hamiltonian -
                actual.target_hidden_suppressed.hamiltonian,
            test_case.name + ".Hamiltonian decomposition");
        for (std::size_t direction = 0; direction < 2; ++direction)
        {
            require_close(
                actual.hidden_sensitive_increment.momentum[direction],
                actual.target_total.momentum[direction] -
                    actual.target_hidden_suppressed
                        .momentum[direction],
                test_case.name + ".momentum decomposition");
        }
        std::cout << "CONSTRAINT case=" << test_case.name
                  << " shared_H="
                  << actual.target_hidden_suppressed.hamiltonian
                  << " hidden_H="
                  << actual.hidden_sensitive_increment.hamiltonian
                  << " total_H="
                  << actual.target_total.hamiltonian
                  << " oracle_H=" << expected.hamiltonian
                  << " total_Mx="
                  << actual.target_total.momentum[0]
                  << " oracle_Mx=" << expected.momentum[0]
                  << " total_Mz="
                  << actual.target_total.momentum[1]
                  << " oracle_Mz=" << expected.momentum[1] << "\n";
    }
    const auto non_trace_free = make_non_trace_free_flat_input();
    const auto non_trace_actual =
        Seam::evaluate_constraints(non_trace_free);
    const auto non_trace_expected = oracle_constraints(non_trace_free);
    stats.observe(non_trace_actual.target_total.hamiltonian,
                  non_trace_expected.hamiltonian,
                  "non_trace_free_flat", "Hamiltonian");
    stats.observe(non_trace_actual.target_total.momentum[0],
                  non_trace_expected.momentum[0],
                  "non_trace_free_flat", "Mx");
    stats.observe(non_trace_actual.target_total.momentum[1],
                  non_trace_expected.momentum[1],
                  "non_trace_free_flat", "Mz");
    const double non_trace = Seam::policy_trace<Seam::DefaultCleanupPolicy>(
        non_trace_free.vars.physical.A,
        non_trace_free.vars.physical.h);
    require(std::abs(non_trace) > 0.1,
            "non-trace-free Hamiltonian case is active");
    std::cout << "CONSTRAINT case=non_trace_free_flat"
              << " trace_A=" << non_trace
              << " total_H="
              << non_trace_actual.target_total.hamiltonian
              << " oracle_H=" << non_trace_expected.hamiltonian
              << " total_Mx="
              << non_trace_actual.target_total.momentum[0]
              << " oracle_Mx=" << non_trace_expected.momentum[0]
              << " total_Mz="
              << non_trace_actual.target_total.momentum[1]
              << " oracle_Mz=" << non_trace_expected.momentum[1] << "\n";
    return stats;
}

Seam::ConstraintValues central_constraint_response(
    const double epsilon, const Oracle::Direction &direction)
{
    const auto positive =
        Seam::evaluate_constraints(make_input(epsilon, direction))
            .target_total;
    const auto negative =
        Seam::evaluate_constraints(make_input(-epsilon, direction))
            .target_total;
    return {0.5 * (positive.hamiltonian - negative.hamiltonian),
            {0.5 * (positive.momentum[0] - negative.momentum[0]),
             0.5 * (positive.momentum[1] - negative.momentum[1])}};
}

void test_constraint_fourier_sectors()
{
    constexpr double sector_epsilon = 1.0e-6;
    for (const bool p_plus : {true, false})
    {
        double maximum_leakage = 0.0;
        double maximum_commutator = 0.0;
        double maximum_allowed = 0.0;
        for (const double phase : {0.29, 0.91, 1.73})
        {
            const auto positive = central_constraint_response(
                sector_epsilon,
                make_fourier_direction(p_plus, phase));
            const auto reflected = central_constraint_response(
                sector_epsilon,
                make_fourier_direction(p_plus, -phase));
            const std::array<double, 3> values = {
                positive.hamiltonian, positive.momentum[0],
                positive.momentum[1]};
            const std::array<double, 3> reflected_values = {
                reflected.hamiltonian, reflected.momentum[0],
                reflected.momentum[1]};
            const std::array<double, 3> reflection_sign = {
                p_plus ? 1.0 : -1.0,
                p_plus ? 1.0 : -1.0,
                p_plus ? -1.0 : 1.0};
            for (std::size_t output = 0; output < values.size(); ++output)
            {
                const double allowed =
                    0.5 * (values[output] +
                           reflection_sign[output] *
                               reflected_values[output]);
                const double forbidden =
                    0.5 * (values[output] -
                           reflection_sign[output] *
                               reflected_values[output]);
                const double commutator =
                    reflected_values[output] -
                    reflection_sign[output] * values[output];
                maximum_allowed =
                    std::max(maximum_allowed, std::abs(allowed));
                maximum_leakage =
                    std::max(maximum_leakage, std::abs(forbidden));
                maximum_commutator =
                    std::max(maximum_commutator,
                             std::abs(commutator));
            }
        }
        require(maximum_allowed > 1.0e-10,
                "Fourier constraint sector has nonzero allowed output");
        require(maximum_leakage <= absolute_tolerance,
                "Fourier constraint forbidden-sector leakage");
        require(maximum_commutator <= absolute_tolerance,
                "Fourier constraint reflection commutator");
        std::cout << "CONSTRAINT_PARITY sector="
                  << (p_plus ? "P_PLUS" : "P_MINUS")
                  << " phases=3"
                  << " max_allowed=" << maximum_allowed
                  << " max_forbidden_leakage=" << maximum_leakage
                  << " max_reflection_commutator="
                  << maximum_commutator << "\n";
    }
}

Reduced::Variables<double> cleanup_input()
{
    Reduced::Variables<double> vars{};
    vars.physical.chi = 0.83;
    vars.physical.h = {1.31, 0.17, 0.92, 1.18};
    vars.physical.K = -0.29;
    vars.physical.A = {0.26, -0.08, -0.03, 0.19};
    vars.physical.Theta = 0.07;
    vars.physical.Gamma = {0.12, -0.14};
    vars.gauge.lapse = 0.91;
    vars.gauge.shift = {0.21, -0.16};
    vars.gauge.B = {-0.05, 0.08};
    return vars;
}

struct HiddenMultiplicityOneTraceCleanup : Seam::DefaultCleanupPolicy
{
    static constexpr int trace_hidden_copies = 1;
};

struct HiddenMultiplicityOneDeterminant : Seam::DefaultCleanupPolicy
{
    static constexpr int determinant_hidden_copies = 1;
};

struct WrongDeterminantExponent : Seam::DefaultCleanupPolicy
{
    static constexpr int determinant_dimension = 3;
};

struct OmitHiddenDeterminant : Seam::DefaultCleanupPolicy
{
    static constexpr int determinant_hidden_copies = 0;
};

struct DoubleHwwWrite : Seam::DefaultCleanupPolicy
{
    static constexpr int hww_output_copies = 2;
};

struct DoubleAwwWrite : Seam::DefaultCleanupPolicy
{
    static constexpr int Aww_output_copies = 2;
};

struct WrongTraceFraction : Seam::DefaultCleanupPolicy
{
    static constexpr double trace_fraction = 1.0 / 3.0;
};

struct OmitOffDiagonalTrace : Seam::DefaultCleanupPolicy
{
    static constexpr bool include_off_diagonal_trace = false;
};

struct OmitDeterminantCleanup : Seam::DefaultCleanupPolicy
{
    static constexpr bool apply_determinant_cleanup = false;
};

struct OmitTraceCleanup : Seam::DefaultCleanupPolicy
{
    static constexpr bool apply_trace_cleanup = false;
};

double state_max_difference(const Reduced::Variables<double> &left,
                            const Reduced::Variables<double> &right)
{
    const auto a = Reduced::store(left);
    const auto b = Reduced::store(right);
    double result = 0.0;
    for (std::size_t slot = 0; slot < a.size(); ++slot)
    {
        result = std::max(result, std::abs(a[slot] - b[slot]));
    }
    return result;
}

void test_cleanup()
{
    const auto input = cleanup_input();
    const auto cleaned = Seam::cleanup(input);
    require_close(cleaned.determinant_after, 1.0,
                  "hidden-aware determinant cleanup");
    require_close(cleaned.weighted_trace_after, 0.0,
                  "hidden-aware A trace cleanup");
    require(cleaned.hww_writes == 1 && cleaned.Aww_writes == 1,
            "representative hww/Aww each written once");
    require_close(cleaned.weighted_trace_before,
                  cleaned.visible_stock_trace_before +
                      cleaned.hidden_trace_correction,
                  "visible stock trace plus hidden correction");

    const auto twice = Seam::cleanup(cleaned.cleaned);
    const double idempotence =
        state_max_difference(cleaned.cleaned, twice.cleaned);
    require_close(idempotence, 0.0, "cleanup idempotence");
    for (const auto slot :
         {Production::c_chi, Production::c_K, Production::c_Theta,
          Production::c_GammaX, Production::c_GammaZ,
          Production::c_lapse, Production::c_shiftX,
          Production::c_shiftZ, Production::c_Bx, Production::c_Bz})
    {
        require_close(Reduced::component(cleaned.cleaned, slot),
                      Reduced::component(input, slot),
                      "cleanup nonowned slot unchanged");
    }

    // The two exact algebraic maps commute under uniform metric scaling.
    auto reverse_metric = input;
    const double trace_before =
        Seam::policy_trace<Seam::DefaultCleanupPolicy>(
            input.physical.A, input.physical.h);
    reverse_metric.physical.A.xx -=
        0.25 * input.physical.h.xx * trace_before;
    reverse_metric.physical.A.xz -=
        0.25 * input.physical.h.xz * trace_before;
    reverse_metric.physical.A.zz -=
        0.25 * input.physical.h.zz * trace_before;
    reverse_metric.physical.A.ww -=
        0.25 * input.physical.h.ww * trace_before;
    const auto reversed = Seam::cleanup_with_policy(reverse_metric);
    require_close(state_max_difference(reversed.cleaned, cleaned.cleaned),
                  0.0, "exact cleanup maps commute");

    require(std::abs(Seam::cleanup_with_policy(
                         input, HiddenMultiplicityOneTraceCleanup{})
                         .weighted_trace_after) >
                1.0e-3,
            "hidden trace multiplicity one mutation detected");
    require(std::abs(Seam::cleanup_with_policy(
                         input, HiddenMultiplicityOneDeterminant{})
                         .determinant_after -
                     1.0) >
                1.0e-3,
            "determinant hidden multiplicity one mutation detected");
    require(std::abs(Seam::cleanup_with_policy(
                         input, WrongDeterminantExponent{})
                         .determinant_after -
                     1.0) >
                1.0e-3,
            "wrong determinant exponent mutation detected");
    require(std::abs(Seam::cleanup_with_policy(
                         input, OmitHiddenDeterminant{})
                         .determinant_after -
                     1.0) >
                1.0e-3,
            "omitted hidden determinant mutation detected");
    require(state_max_difference(
                Seam::cleanup_with_policy(input, DoubleHwwWrite{}).cleaned,
                cleaned.cleaned) >
                0.1,
            "doubled hww write mutation detected");
    require(state_max_difference(
                Seam::cleanup_with_policy(input, DoubleAwwWrite{}).cleaned,
                cleaned.cleaned) >
                1.0e-3,
            "doubled Aww write mutation detected");
    require(std::abs(Seam::cleanup_with_policy(input,
                                               WrongTraceFraction{})
                         .weighted_trace_after) >
                1.0e-3,
            "wrong trace projection mutation detected");
    require(std::abs(Seam::cleanup_with_policy(
                         input, OmitOffDiagonalTrace{})
                         .weighted_trace_after) >
                1.0e-3,
            "omitted off-diagonal inverse term mutation detected");
    require(state_max_difference(
                Seam::cleanup_with_policy(input, OmitDeterminantCleanup{})
                    .cleaned,
                cleaned.cleaned) >
                0.1,
            "determinant cleanup omission mutation detected");
    require(state_max_difference(
                Seam::cleanup_with_policy(input, OmitTraceCleanup{}).cleaned,
                cleaned.cleaned) >
                0.1,
            "trace cleanup omission mutation detected");

    for (auto invalid : {
             Reduced::ReducedSymmetricTensor<double>{1.0, 0.0, 0.0, 1.0},
             Reduced::ReducedSymmetricTensor<double>{1.0, 2.0, 1.0, 1.0},
             Reduced::ReducedSymmetricTensor<double>{
                 1.0, 0.0, 1.0,
                 std::numeric_limits<double>::infinity()}})
    {
        auto state = input;
        state.physical.h = invalid;
        bool rejected = false;
        try
        {
            (void)Seam::cleanup(state);
        }
        catch (const std::domain_error &)
        {
            rejected = true;
        }
        require(rejected, "inadmissible cleanup input rejected");
    }
    std::cout << "CLEANUP determinant_before="
              << cleaned.determinant_before
              << " determinant_after=" << cleaned.determinant_after
              << " trace_before=" << cleaned.weighted_trace_before
              << " trace_after=" << cleaned.weighted_trace_after
              << " idempotence_max=" << idempotence
              << " order=DETERMINANT_THEN_TRACE"
              << " reverse_relation=ALGEBRAICALLY_COMMUTES\n"
              << "MUTATION cleanup_det_hidden_one=DETECTED"
              << " cleanup_trace_hidden_one=DETECTED"
              << " doubled_hww=DETECTED doubled_Aww=DETECTED"
              << " omitted_determinant=DETECTED"
              << " omitted_trace=DETECTED"
              << " omitted_offdiag_inverse=DETECTED\n";
}

struct TraceContaminatedHamiltonian : Seam::DefaultConstraintPolicy
{
    static constexpr bool use_trace_contaminated_K_reconstruction = true;
};

struct WrongHamiltonianKCoefficient : Seam::DefaultConstraintPolicy
{
    static constexpr double hamiltonian_k_squared_coefficient = 2.0 / 3.0;
};

struct OneHiddenAContraction : Seam::DefaultConstraintPolicy
{
    static constexpr int active_hidden_A_copies = 1;
};

struct OmitOffDiagonalHamiltonianA : Seam::DefaultConstraintPolicy
{
    static constexpr bool include_off_diagonal_A = false;
};

struct OmitRicciInput : Seam::DefaultConstraintPolicy
{
    static constexpr double ricci_coefficient = 0.0;
};

struct IncorrectMx : Seam::DefaultConstraintPolicy
{
    static constexpr double momentum_x_scale = 0.0;
};

struct IncorrectMz : Seam::DefaultConstraintPolicy
{
    static constexpr double momentum_z_scale = 0.0;
};

struct OmitHiddenMomentumDerivative : Seam::DefaultConstraintPolicy
{
    static constexpr bool include_hidden_momentum_derivatives = false;
};

struct WrongMomentumKGradient : Seam::DefaultConstraintPolicy
{
    static constexpr double momentum_k_gradient_coefficient = 2.0 / 3.0;
};

struct OmitOffDiagonalMomentumA : Seam::DefaultConstraintPolicy
{
    static constexpr bool include_off_diagonal_momentum_A = false;
};

struct HiddenMomentumReductionLeakage : Seam::DefaultConstraintPolicy
{
    static constexpr bool leak_hidden_reduction_into_visible = true;
};

struct DirectRicciCounter : Seam::DefaultConstraintPolicy
{
    std::size_t *calls;
    explicit DirectRicciCounter(std::size_t &counter) : calls(&counter) {}
    void record_direct_grchombo_ricci(const double) const { ++*calls; }
};

template <class policy_t>
Seam::ConstraintResult mutate_constraints(const Target::Input &input,
                                          const policy_t &policy)
{
    return Seam::evaluate_constraints_with_policies(
        input, Target::DefaultExpansionPolicy{}, policy);
}

void require_mutated(const double mutated, const double correct,
                     const std::string &name)
{
    require(std::abs(mutated - correct) >
                10.0 * allowance(mutated, correct),
            name + " mutation did not change active output");
}

void test_constraint_mutations()
{
    const auto cases = make_constraint_cases();
    const auto non_trace_free = make_non_trace_free_flat_input();
    const auto non_trace_correct =
        Seam::evaluate_constraints(non_trace_free);
    const auto old_hamiltonian =
        mutate_constraints(non_trace_free,
                           TraceContaminatedHamiltonian{});
    require_mutated(old_hamiltonian.target_total.hamiltonian,
                    non_trace_correct.target_total.hamiltonian,
                    "trace-contaminated reconstructed-K Hamiltonian");
    const auto wrong_k2 =
        mutate_constraints(non_trace_free,
                           WrongHamiltonianKCoefficient{});
    require_mutated(wrong_k2.target_total.hamiltonian,
                    non_trace_correct.target_total.hamiltonian,
                    "wrong 3/4 Hamiltonian coefficient");

    const auto &hidden = cases[4];
    const auto hidden_input =
        make_input(hidden.amplitude, hidden.direction);
    const auto hidden_correct = Seam::evaluate_constraints(hidden_input);
    const auto one_hidden_A =
        mutate_constraints(hidden_input, OneHiddenAContraction{});
    require_mutated(one_hidden_A.target_total.hamiltonian,
                    hidden_correct.target_total.hamiltonian,
                    "one-hidden-copy A squared contraction");
    const auto omitted_hidden_momentum =
        mutate_constraints(hidden_input,
                           OmitHiddenMomentumDerivative{});
    require_mutated(omitted_hidden_momentum.target_total.momentum[0],
                    hidden_correct.target_total.momentum[0],
                    "omitted hidden derivative contribution to Mx");

    const auto &off_diagonal = cases[3];
    const auto off_input =
        make_input(off_diagonal.amplitude, off_diagonal.direction);
    const auto off_correct = Seam::evaluate_constraints(off_input);
    const auto omitted_offdiag_H =
        mutate_constraints(off_input,
                           OmitOffDiagonalHamiltonianA{});
    require_mutated(omitted_offdiag_H.target_total.hamiltonian,
                    off_correct.target_total.hamiltonian,
                    "omitted off-diagonal A contraction");
    const auto omitted_offdiag_momentum =
        mutate_constraints(off_input,
                           OmitOffDiagonalMomentumA{});
    require_mutated(omitted_offdiag_momentum.target_total.momentum[1],
                    off_correct.target_total.momentum[1],
                    "omitted off-diagonal momentum contribution");

    const auto &diagonal = cases[2];
    const auto diagonal_input =
        make_input(diagonal.amplitude, diagonal.direction);
    const auto diagonal_correct =
        Seam::evaluate_constraints(diagonal_input);
    const auto omitted_ricci =
        mutate_constraints(diagonal_input, OmitRicciInput{});
    require_mutated(omitted_ricci.target_total.hamiltonian,
                    diagonal_correct.target_total.hamiltonian,
                    "omitted direct Ricci input");
    const auto wrong_k_gradient =
        mutate_constraints(diagonal_input, WrongMomentumKGradient{});
    require_mutated(wrong_k_gradient.target_total.momentum[0],
                    diagonal_correct.target_total.momentum[0],
                    "wrong d=4 K-gradient coefficient");

    const auto &mixed = cases[5];
    const auto mixed_input =
        make_input(mixed.amplitude, mixed.direction);
    const auto mixed_correct = Seam::evaluate_constraints(mixed_input);
    const auto incorrect_mx =
        mutate_constraints(mixed_input, IncorrectMx{});
    const auto incorrect_mz =
        mutate_constraints(mixed_input, IncorrectMz{});
    require_mutated(incorrect_mx.target_total.momentum[0],
                    mixed_correct.target_total.momentum[0],
                    "independently incorrect Mx");
    require_mutated(incorrect_mz.target_total.momentum[1],
                    mixed_correct.target_total.momentum[1],
                    "independently incorrect Mz");
    const auto leaked =
        mutate_constraints(mixed_input,
                           HiddenMomentumReductionLeakage{});
    require_mutated(leaked.target_total.momentum[0],
                    mixed_correct.target_total.momentum[0],
                    "hidden reduction leakage into Mx");
    require_mutated(leaked.target_total.momentum[1],
                    mixed_correct.target_total.momentum[1],
                    "hidden reduction leakage into Mz");

    std::size_t direct_ricci_calls = 0;
    (void)mutate_constraints(mixed_input,
                             DirectRicciCounter(direct_ricci_calls));
    require(direct_ricci_calls == 2,
            "direct GRChombo Ricci called for full and suppressed paths");
    static_assert(
        std::tuple_size<decltype(Seam::ConstraintValues{}.momentum)>::value ==
        2);
    static_assert(Seam::constraint_outputs == 3);

    std::cout << "NON_TRACE_FREE corrected_H="
              << non_trace_correct.target_total.hamiltonian
              << " rejected_reconstructed_K_H="
              << old_hamiltonian.target_total.hamiltonian
              << " difference="
              << old_hamiltonian.target_total.hamiltonian -
                     non_trace_correct.target_total.hamiltonian
              << "\n"
              << "DIRECT_GRCHOMBO_RICCI_CALLS="
              << direct_ricci_calls << "\n"
              << "MUTATION Hamiltonian_old_Kij=DETECTED"
              << " wrong_three_quarters=DETECTED"
              << " hidden_A2_multiplicity_one=DETECTED"
              << " omitted_offdiag_A2=DETECTED"
              << " omitted_ricci=DETECTED\n"
              << "MUTATION Mx=DETECTED Mz=DETECTED"
              << " omitted_hidden_derivative=DETECTED"
              << " wrong_K_gradient=DETECTED"
              << " omitted_offdiag_momentum=DETECTED"
              << " hidden_reduction_leakage=DETECTED"
              << " outputs=H,Mx,Mz\n";
}

struct WrongSourceSign : Seam::DefaultSourcePolicy
{
    static void apply(Target::TargetVars<double> &rhs,
                      const double source)
    {
        rhs.lapse -= source;
    }
};

struct MissingFactorThree : Seam::DefaultSourcePolicy
{
    static double value(const double r0_value, const double x_value,
                        const Reduced::Variables<double> &)
    {
        return std::sqrt(r0_value /
                         (x_value * x_value * x_value));
    }
};

struct FieldDependentSource : Seam::DefaultSourcePolicy
{
    static double value(const double, const double,
                        const Reduced::Variables<double> &vars)
    {
        return 2.0 * vars.physical.K;
    }
};

struct HorizonDependentSource : Seam::DefaultSourcePolicy
{
    static double mock_horizon;
    static double value(const double, const double x_value,
                        const Reduced::Variables<double> &)
    {
        return 3.0 * std::sqrt(mock_horizon /
                              (x_value * x_value * x_value));
    }
};
double HorizonDependentSource::mock_horizon = 0.91;

struct ShiftOwnerSource : Seam::DefaultSourcePolicy
{
    static void apply(Target::TargetVars<double> &rhs,
                      const double source)
    {
        rhs.lapse += source;
        rhs.shift[0] += source;
    }
};

struct BOwnerSource : Seam::DefaultSourcePolicy
{
    static void apply(Target::TargetVars<double> &rhs,
                      const double source)
    {
        rhs.lapse += source;
        rhs.B[0] += source;
    }
};

struct DoubleSource : Seam::DefaultSourcePolicy
{
    static void apply(Target::TargetVars<double> &rhs,
                      const double source)
    {
        rhs.lapse += 2.0 * source;
    }
};

void test_fixed_source()
{
    const Oracle::Direction zero;
    const auto gp = make_input(0.0, zero);
    const auto result = Seam::evaluate_fixed_gp_lapse_source(r0, gp);
    const double lambda = std::sqrt(r0 / (x * x * x));
    require_close(result.moving_puncture_raw.lapse, -3.0 * lambda,
                  "direct MovingPunctureGauge GP lapse drift");
    require_close(result.fixed_source, 3.0 * lambda,
                  "fixed GP source");
    require_close(result.with_fixed_source.lapse, 0.0,
                  "fixed GP source cancels lapse drift");
    for (std::size_t direction = 0; direction < 2; ++direction)
    {
        require_close(result.with_fixed_source.shift[direction],
                      result.moving_puncture_raw.shift[direction],
                      "source leaves shift unchanged");
        require_close(result.with_fixed_source.B[direction],
                      result.moving_puncture_raw.B[direction],
                      "source leaves B unchanged");
    }

    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        auto mutated = gp;
        Reduced::component(
            mutated.vars,
            static_cast<Production::VariableSlot>(slot)) += 0.013;
        const auto varied =
            Seam::evaluate_fixed_gp_lapse_source(r0, mutated);
        require_close(varied.fixed_source, result.fixed_source,
                      "source field derivative is zero");
    }

    const auto wrong_sign =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, gp, WrongSourceSign{});
    const auto missing_three =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, gp, MissingFactorThree{});
    auto field_mutated_input = gp;
    field_mutated_input.vars.physical.K += 0.23;
    const auto field_source =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, field_mutated_input, FieldDependentSource{});
    require(std::abs(wrong_sign.with_fixed_source.lapse) > 0.1,
            "wrong source sign mutation detected");
    require(std::abs(missing_three.with_fixed_source.lapse) > 0.1,
            "missing factor three mutation detected");
    require(std::abs(field_source.fixed_source - result.fixed_source) >
                0.1,
            "evolving K source mutation detected");

    const auto horizon_a =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, gp, HorizonDependentSource{});
    HorizonDependentSource::mock_horizon = 1.44;
    const auto horizon_b =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, gp, HorizonDependentSource{});
    require(std::abs(horizon_a.fixed_source - horizon_b.fixed_source) >
                0.1,
            "instantaneous horizon source mutation detected");
    const auto shift_owner =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, gp, ShiftOwnerSource{});
    const auto b_owner =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, gp, BOwnerSource{});
    const auto doubled =
        Seam::evaluate_fixed_gp_lapse_source_with_policy(
            r0, gp, DoubleSource{});
    require(std::abs(shift_owner.with_fixed_source.shift[0] -
                     result.with_fixed_source.shift[0]) >
                0.1,
            "shift source ownership mutation detected");
    require(std::abs(b_owner.with_fixed_source.B[0] -
                     result.with_fixed_source.B[0]) >
                0.1,
            "B source ownership mutation detected");
    require(std::abs(doubled.with_fixed_source.lapse) > 0.1,
            "double source application mutation detected");

    std::cout << "SOURCE moving_puncture_lapse="
              << result.moving_puncture_raw.lapse
              << " fixed_source=" << result.fixed_source
              << " final_lapse=" << result.with_fixed_source.lapse
              << " shift_owners=0 B_owners=0"
              << " evolved_field_jacobian=0\n"
              << "MUTATION source_wrong_sign=DETECTED"
              << " missing_factor_three=DETECTED"
              << " evolving_K=DETECTED horizon_radius=DETECTED"
              << " shift_owner=DETECTED B_owner=DETECTED"
              << " applied_twice=DETECTED\n";
}

} // namespace

int main()
{
    std::cout << std::scientific << std::setprecision(12);
    test_cleanup();
    const auto constraints = test_constraints();
    test_constraint_fourier_sectors();
    test_constraint_mutations();
    test_fixed_source();
    std::cout << "CONSTRAINT_MAX absolute=" << constraints.max_absolute
              << " normalized=" << constraints.max_normalized
              << " worst_case=" << constraints.worst_case
              << " worst_output=" << constraints.worst_output << "\n"
              << "PASS hidden-aware cleanup constraints and fixed GP "
                 "lapse source\n";
    return 0;
}
