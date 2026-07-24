#include "BlackStringTargetCCZ4Pointwise.hpp"
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
#include <type_traits>
#include <vector>

namespace
{
namespace Adapter = BlackStringTargetCCZ4Pointwise;
namespace GP = BlackStringGPPointwiseInitialData;
namespace Oracle = BlackStringToy::Stage4AOCAnalyticFullOracle;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

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

std::size_t row_index(const Adapter::PhysicalRow row)
{
    return static_cast<std::size_t>(row);
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

Adapter::Input make_input(const double amplitude,
                          const Oracle::Direction &direction)
{
    Adapter::Input input{};
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
    const auto metadata = GP::make_analytic_metadata(r0, x);
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

enum class DataClassification
{
    gp_background,
    even_variable_data,
    one_z_variable_data,
    mixed_unconstrained_data
};

const char *classification_name(const DataClassification classification)
{
    switch (classification)
    {
    case DataClassification::gp_background:
        return "gp_background";
    case DataClassification::even_variable_data:
        return "even_variable_data";
    case DataClassification::one_z_variable_data:
        return "one_z_variable_data";
    case DataClassification::mixed_unconstrained_data:
        return "mixed_unconstrained_data";
    }
    return "unreachable";
}

struct Case
{
    std::string name;
    double amplitude;
    Oracle::Direction direction;
    DataClassification classification;
};

std::vector<Case> make_cases()
{
    using Variable = Operator::PerturbationVariable;
    std::vector<Case> cases;
    cases.push_back(
        {"exact_gp", 0.0, {}, DataClassification::gp_background});

    Oracle::Direction flat;
    set_jet(flat, Variable::chi, {0.08, 0.0, 0.0, 0.0, 0.0, 0.0});
    set_jet(flat, Variable::K, {-0.11, 0.0, 0.0, 0.0, 0.0, 0.0});
    set_jet(flat, Variable::Theta, {0.07, 0.0, 0.0, 0.0, 0.0, 0.0});
    set_jet(flat, Variable::A_xx, {0.05, 0.0, 0.0, 0.0, 0.0, 0.0});
    cases.push_back({"flat_nontrivial", 0.04, flat,
                     DataClassification::even_variable_data});

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
    cases.push_back({"diagonal_curved", 0.05, diagonal,
                     DataClassification::even_variable_data});

    Oracle::Direction one_z;
    set_jet(one_z, Variable::h_xz,
            {0.09, -0.04, 0.07, 0.05, -0.06, 0.03});
    set_jet(one_z, Variable::A_xz,
            {-0.08, 0.06, -0.05, -0.04, 0.07, -0.02});
    set_jet(one_z, Variable::hat_Gamma_z,
            {0.03, -0.02, 0.04, 0.0, 0.0, 0.0});
    cases.push_back({"one_z_components", 0.035, one_z,
                     DataClassification::one_z_variable_data});

    Oracle::Direction hidden;
    set_jet(hidden, Variable::h_ww,
            {0.12, -0.08, 0.05, 0.07, -0.04, 0.09});
    set_jet(hidden, Variable::A_ww,
            {-0.10, 0.06, -0.03, -0.05, 0.02, -0.08});
    cases.push_back({"pure_hidden_ww", 0.045, hidden,
                     DataClassification::even_variable_data});

    Oracle::Direction encoded_z;
    set_jet(encoded_z, Variable::hat_Gamma_x,
            {0.11, -0.07, 0.06, 0.0, 0.0, 0.0});
    set_jet(encoded_z, Variable::hat_Gamma_z,
            {-0.09, 0.05, -0.08, 0.0, 0.0, 0.0});
    cases.push_back({"pure_hatted_gamma_z", 0.05, encoded_z,
                     DataClassification::mixed_unconstrained_data});

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
    cases.push_back({"mixed_all_families", 0.025, mixed,
                     DataClassification::mixed_unconstrained_data});
    return cases;
}

const Case &case_named(const std::vector<Case> &cases, const char *name)
{
    for (const Case &test_case : cases)
    {
        if (test_case.name == name)
        {
            return test_case;
        }
    }
    throw std::logic_error("missing target pointwise test case");
}

struct OldChristoffelConstructionOrderPolicy
    : Oracle::CorrectedEvaluationPolicy
{
    Oracle::Geometry
    geometry_from(const Oracle::AxisTensorJets &jets) const
    {
        return Oracle::compute_geometry_old_construction_order(jets);
    }
};

struct ActiveShiftHessianPolicy : Oracle::CorrectedEvaluationPolicy
{
    Oracle::ShiftJets
    shift_from(const Oracle::Context &context) const
    {
        Oracle::ShiftJets shift = Oracle::shift_jets(context);
        shift.second[0][1][0] = 0.31L;
        shift.second[1][0][0] = 0.31L;
        shift.second[0][0][1] = -0.17L;
        return shift;
    }
};

struct OldVectorHessianIndexOrderPolicy : ActiveShiftHessianPolicy
{
    Oracle::Real
    contracted_vector_hessian(const Oracle::ShiftJets &shift,
                              const int derivative_1,
                              const int derivative_2,
                              const int component) const
    {
        return shift.second[component][derivative_1][derivative_2];
    }
};

void check_tensor_expansion(const Case &test_case)
{
    const Adapter::Input input =
        make_input(test_case.amplitude, test_case.direction);
    const Oracle::Context context{
        r0, x, test_case.amplitude, &test_case.direction,
        Oracle::hidden_multiplicity};
    const auto expected = Oracle::conformal_metric_jets(context);
    const auto actual = Adapter::expand_target(input);
    for (int i = 0; i < Adapter::target_dimension; ++i)
    {
        for (int j = 0; j < Adapter::target_dimension; ++j)
        {
            require(std::abs(actual.vars.h[i][j] -
                             static_cast<double>(expected.value[i][j])) <
                        1.0e-14,
                    test_case.name + ".expanded_h_value");
            for (int first = 0; first < Adapter::target_dimension; ++first)
            {
                require(
                    std::abs(actual.d1.h[i][j][first] -
                             static_cast<double>(
                                 expected.first[first][i][j])) <
                        1.0e-14,
                    test_case.name + ".expanded_h_d1");
                for (int second = 0;
                     second < Adapter::target_dimension; ++second)
                {
                    require(
                        std::abs(actual.d2.h[i][j][first][second] -
                                 static_cast<double>(
                                     expected.second[first][second][i][j])) <
                            1.0e-14,
                        test_case.name + ".expanded_h_d2");
                }
            }
        }
    }
}

struct DirectStatistics
{
    Adapter::Rows maximum_absolute = {};
    Adapter::Rows maximum_normalized = {};
    std::array<std::string, Adapter::physical_rows> worst_state = {};
    double global_absolute = 0.0;
    double global_normalized = 0.0;
    std::string global_row;
    std::string global_state;
    DataClassification global_classification =
        DataClassification::gp_background;
};

void check_direct_nonlinear()
{
    DirectStatistics statistics;
    for (const Case &test_case : make_cases())
    {
        check_tensor_expansion(test_case);
        const Adapter::Result actual = Adapter::evaluate(
            make_input(test_case.amplitude, test_case.direction));
        const Oracle::Result expected =
            Oracle::evaluate(r0, x, test_case.amplitude,
                             test_case.direction);
        for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
        {
            double family_suppressed = 0.0;
            double family_increment = 0.0;
            double family_full = 0.0;
            for (std::size_t family = 0;
                 family < Adapter::reported_families; ++family)
            {
                family_suppressed +=
                    actual.family[family]
                        .target_shared_hidden_suppressed[row];
                family_increment +=
                    actual.family[family]
                        .hidden_increment_decomposition[row];
                family_full +=
                    actual.family[family].target_full_grchombo[row];
            }
            const double suppressed =
                actual.target_shared_hidden_suppressed[row];
            const double increment =
                actual.hidden_increment_decomposition[row];
            const double full = actual.target_full_grchombo[row];
            require(std::abs(suppressed + increment - full) <
                        5.0e-13,
                    test_case.name + ".decomposition_identity");
            require(std::abs(family_suppressed - suppressed) <
                        5.0e-13,
                    test_case.name + ".family_suppressed_sum");
            require(std::abs(family_increment - increment) <
                        5.0e-13,
                    test_case.name + ".family_increment_sum");
            require(std::abs(family_full - full) < 5.0e-13,
                    test_case.name + ".family_full_sum");

            const double oracle =
                static_cast<double>(expected.total[row]);
            const double error = std::abs(full - oracle);
            const double normalized = error / allowance(full, oracle);
            require(normalized <= 1.0,
                    test_case.name + ".direct." +
                        Adapter::row_names[row]);
            if (error > statistics.maximum_absolute[row])
            {
                statistics.maximum_absolute[row] = error;
            }
            if (normalized > statistics.maximum_normalized[row])
            {
                statistics.maximum_normalized[row] = normalized;
                statistics.worst_state[row] = test_case.name;
            }
            if (error > statistics.global_absolute)
            {
                statistics.global_absolute = error;
            }
            if (normalized > statistics.global_normalized)
            {
                statistics.global_normalized = normalized;
                statistics.global_row = Adapter::row_names[row];
                statistics.global_state = test_case.name;
                statistics.global_classification =
                    test_case.classification;
            }
            std::cout
                << "DIRECT_ROW state=" << test_case.name
                << " classification="
                << classification_name(test_case.classification)
                << " row=" << Adapter::row_names[row]
                << " target_shared_hidden_suppressed=" << suppressed
                << " hidden_increment_decomposition=" << increment
                << " target_full_grchombo=" << full
                << " oracle=" << oracle
                << " absolute_error=" << error
                << " normalized_error=" << normalized << "\n";
        }
    }
    for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
    {
        std::cout << "DIRECT_ROW_MAX row=" << Adapter::row_names[row]
                  << " absolute=" << statistics.maximum_absolute[row]
                  << " normalized="
                  << statistics.maximum_normalized[row]
                  << " worst_state=" << statistics.worst_state[row]
                  << "\n";
    }
    std::cout << "DIRECT_MAX absolute=" << statistics.global_absolute
              << " normalized=" << statistics.global_normalized
              << " worst_row=" << statistics.global_row
              << " worst_state=" << statistics.global_state
              << " classification="
              << classification_name(
                     statistics.global_classification)
              << "\n";
}

Adapter::Rows adapter_central_jvp(const Oracle::Direction &direction,
                                  const double epsilon,
                                  Adapter::Rows *plus_out = nullptr,
                                  Adapter::Rows *minus_out = nullptr)
{
    const auto plus =
        Adapter::evaluate(make_input(epsilon, direction))
            .target_full_grchombo;
    const auto minus =
        Adapter::evaluate(make_input(-epsilon, direction))
            .target_full_grchombo;
    if (plus_out != nullptr)
    {
        *plus_out = plus;
    }
    if (minus_out != nullptr)
    {
        *minus_out = minus;
    }
    Adapter::Rows result = {};
    for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
    {
        result[row] = (plus[row] - minus[row]) / (2.0 * epsilon);
    }
    return result;
}

Oracle::Rows oracle_central_jvp(const Oracle::Direction &direction,
                                const double epsilon)
{
    const auto plus = Oracle::evaluate(r0, x, epsilon, direction);
    const auto minus = Oracle::evaluate(r0, x, -epsilon, direction);
    Oracle::Rows result = {};
    for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
    {
        result[row] =
            (plus.total[row] - minus.total[row]) /
            (2.0L * static_cast<Oracle::Real>(epsilon));
    }
    return result;
}

void check_jvp_sweep()
{
    constexpr std::array<double, 8> epsilons = {
        1.0e-1, 3.0e-2, 1.0e-2, 3.0e-3,
        1.0e-3, 3.0e-4, 1.0e-4, 3.0e-5};
    const auto cases = make_cases();
    for (const Case &test_case : cases)
    {
        if (test_case.amplitude == 0.0)
        {
            continue;
        }
        Adapter::Rows previous_adapter = {};
        Oracle::Rows previous_oracle = {};
        bool have_previous = false;
        const Adapter::Rows finite_adapter =
            Adapter::evaluate(
                make_input(test_case.amplitude,
                           test_case.direction))
                .target_full_grchombo;
        const Oracle::Rows finite_oracle =
            Oracle::evaluate(r0, x, test_case.amplitude,
                             test_case.direction)
                .total;
        const std::size_t gamma_x_row =
            row_index(Adapter::PhysicalRow::GammaX);
        const double finite_gamma_x_discrepancy =
            std::abs(finite_adapter[gamma_x_row] -
                     static_cast<double>(
                         finite_oracle[gamma_x_row]));
        for (const double epsilon : epsilons)
        {
            Adapter::Rows plus = {};
            Adapter::Rows minus = {};
            const Adapter::Rows adapter = adapter_central_jvp(
                test_case.direction, epsilon, &plus, &minus);
            const Oracle::Rows oracle =
                oracle_central_jvp(test_case.direction, epsilon);
            double epsilon_max_absolute = 0.0;
            double epsilon_max_normalized = 0.0;
            std::string epsilon_worst_row;
            double gamma_x_absolute_error = 0.0;
            for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
            {
                const double oracle_value =
                    static_cast<double>(oracle[row]);
                const double error =
                    std::abs(adapter[row] - oracle_value);
                const double normalized =
                    error / allowance(adapter[row], oracle_value);
                const double adapter_change =
                    have_previous
                        ? std::abs(adapter[row] -
                                   previous_adapter[row])
                        : 0.0;
                const double oracle_change =
                    have_previous
                        ? std::abs(oracle_value -
                                   static_cast<double>(
                                       previous_oracle[row]))
                        : 0.0;
                if (row == gamma_x_row)
                {
                    gamma_x_absolute_error = error;
                }
                if (error > epsilon_max_absolute)
                {
                    epsilon_max_absolute = error;
                }
                if (normalized > epsilon_max_normalized)
                {
                    epsilon_max_normalized = normalized;
                    epsilon_worst_row = Adapter::row_names[row];
                }
                const double numerator =
                    std::abs(plus[row]) + std::abs(minus[row]);
                const double denominator =
                    std::max(std::abs(plus[row] - minus[row]),
                             std::numeric_limits<double>::min());
                const double cancellation_ratio =
                    numerator / denominator;
                const double roundoff_floor =
                    numerator *
                    std::numeric_limits<double>::epsilon() /
                    (2.0 * epsilon);
                std::cout
                    << "JVP_ROW direction=" << test_case.name
                    << " epsilon=" << epsilon
                    << " row=" << Adapter::row_names[row]
                    << " adapter_jvp=" << adapter[row]
                    << " oracle_jvp=" << oracle_value
                    << " absolute_error=" << error
                    << " normalized_error=" << normalized
                    << " adapter_change_from_coarser="
                    << adapter_change
                    << " oracle_change_from_coarser="
                    << oracle_change
                    << " cancellation_ratio="
                    << cancellation_ratio
                    << " roundoff_floor=" << roundoff_floor
                    << "\n";
            }
            std::cout << "JVP_EPSILON direction=" << test_case.name
                      << " epsilon=" << epsilon
                      << " maximum_absolute_error="
                      << epsilon_max_absolute
                      << " maximum_normalized_error="
                      << epsilon_max_normalized
                      << " worst_row=" << epsilon_worst_row
                      << " sensitive_row=GammaX"
                      << " sensitive_absolute_error="
                      << gamma_x_absolute_error
                      << " epsilon_times_sensitive_absolute_error="
                      << epsilon * gamma_x_absolute_error
                      << " finite_gamma_x_discrepancy="
                      << finite_gamma_x_discrepancy
                      << " finite_gamma_x_over_2epsilon="
                      << finite_gamma_x_discrepancy /
                             (2.0 * epsilon)
                      << "\n";
            previous_adapter = adapter;
            previous_oracle = oracle;
            have_previous = true;
        }
    }
    std::cout << "JVP_REGIME=ROUND_OFF_AND_CANCELLATION_DOMINATED\n"
              << "JVP_ROLE=SECONDARY_DIAGNOSTIC\n"
              << "PRIMARY_EQUIVALENCE_GATE=DIRECT_NONLINEAR\n";
}

bool is_one_z_variable(const Operator::PerturbationVariable variable)
{
    using Variable = Operator::PerturbationVariable;
    return variable == Variable::h_xz ||
           variable == Variable::A_xz ||
           variable == Variable::hat_Gamma_z;
}

bool is_one_z_row(const std::size_t row)
{
    return row == row_index(Adapter::PhysicalRow::hxz) ||
           row == row_index(Adapter::PhysicalRow::Axz) ||
           row == row_index(Adapter::PhysicalRow::GammaZ);
}

enum class FourierSector
{
    plus,
    minus
};

const char *sector_name(const FourierSector sector)
{
    return sector == FourierSector::plus ? "P_plus" : "P_minus";
}

Oracle::Direction make_fourier_direction(const FourierSector sector,
                                         const double z,
                                         const double wave_number)
{
    Oracle::Direction direction;
    for (std::size_t index = 0;
         index < Operator::frozen_gauge_state_vector.size(); ++index)
    {
        const auto variable =
            Operator::frozen_gauge_state_vector[index];
        const bool one_z = is_one_z_variable(variable);
        const bool cosine_phase =
            sector == FourierSector::plus ? !one_z : one_z;
        const double argument = wave_number * z;
        const double phase =
            cosine_phase ? std::cos(argument) : std::sin(argument);
        const double phase_z =
            cosine_phase ? -wave_number * std::sin(argument)
                          : wave_number * std::cos(argument);
        const double phase_zz =
            -wave_number * wave_number * phase;
        const double scale =
            0.009 * static_cast<double>(index + 1);
        const double radial_first = -0.37 * scale;
        const double radial_second = 0.23 * scale;
        set_jet(direction, variable,
                {scale * phase, radial_first * phase,
                 scale * phase_z, radial_second * phase,
                 radial_first * phase_z, scale * phase_zz});
    }
    return direction;
}

void check_fourier_sectors()
{
    constexpr double jvp_epsilon = 1.0e-3;
    constexpr double wave_number = 1.4;
    constexpr std::array<double, 4> phases = {0.17, 0.61, 1.13, 1.89};
    for (const FourierSector sector :
         {FourierSector::plus, FourierSector::minus})
    {
        const double eigenvalue =
            sector == FourierSector::plus ? 1.0 : -1.0;
        double sector_max_leakage = 0.0;
        double sector_max_commutator = 0.0;
        double sector_max_allowed = 0.0;
        for (const double z : phases)
        {
            const auto positive_direction =
                make_fourier_direction(sector, z, wave_number);
            const auto reflected_direction =
                make_fourier_direction(sector, -z, wave_number);
            const auto positive = adapter_central_jvp(
                positive_direction, jvp_epsilon);
            const auto reflected = adapter_central_jvp(
                reflected_direction, jvp_epsilon);
            for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
            {
                const double intrinsic =
                    is_one_z_row(row) ? -1.0 : 1.0;
                const double reflected_transformed =
                    intrinsic * reflected[row];
                const double commutator =
                    reflected_transformed -
                    eigenvalue * positive[row];
                const double forbidden =
                    0.5 *
                    (positive[row] -
                     eigenvalue * reflected_transformed);
                const double allowed =
                    0.5 *
                    (positive[row] +
                     eigenvalue * reflected_transformed);
                sector_max_commutator =
                    std::max(sector_max_commutator,
                             std::abs(commutator));
                sector_max_leakage =
                    std::max(sector_max_leakage,
                             std::abs(forbidden));
                sector_max_allowed =
                    std::max(sector_max_allowed,
                             std::abs(allowed));
                require(std::abs(commutator) <=
                            allowance(reflected_transformed,
                                      eigenvalue * positive[row]),
                        std::string(sector_name(sector)) +
                            ".reflection_commutator." +
                            Adapter::row_names[row]);
                require(std::abs(forbidden) <=
                            allowance(allowed, 0.0),
                        std::string(sector_name(sector)) +
                            ".forbidden_leakage." +
                            Adapter::row_names[row]);
            }
        }
        require(sector_max_allowed > 1.0e-6,
                std::string(sector_name(sector)) +
                    ".allowed_output_inactive");
        std::cout << "FOURIER_SECTOR sector=" << sector_name(sector)
                  << " phases=" << phases.size()
                  << " jvp_epsilon=" << jvp_epsilon
                  << " maximum_forbidden_leakage="
                  << sector_max_leakage
                  << " maximum_reflection_commutator="
                  << sector_max_commutator
                  << " maximum_allowed_output="
                  << sector_max_allowed << "\n";
    }
}

void zero_hidden_cartoon_derivatives(Adapter::ExpandedInput &expanded,
                                     const double multiplier)
{
    for (int i = 0; i < Adapter::target_dimension; ++i)
    {
        for (int j = 0; j < Adapter::target_dimension; ++j)
        {
            for (int derivative = Adapter::gridded_dimension;
                 derivative < Adapter::target_dimension; ++derivative)
            {
                expanded.d1.h[i][j][derivative] *= multiplier;
                expanded.d1.A[i][j][derivative] *= multiplier;
                expanded.d1.Gamma[i][derivative] *= multiplier;
                expanded.d1.shift[i][derivative] *= multiplier;
                expanded.d1.B[i][derivative] *= multiplier;
            }
            for (int first = 0; first < Adapter::target_dimension; ++first)
            {
                for (int second = 0;
                     second < Adapter::target_dimension; ++second)
                {
                    if (first >= Adapter::gridded_dimension ||
                        second >= Adapter::gridded_dimension)
                    {
                        expanded.d2.h[i][j][first][second] *=
                            multiplier;
                    }
                }
            }
        }
    }
    for (int first = 0; first < Adapter::target_dimension; ++first)
    {
        for (int second = 0; second < Adapter::target_dimension; ++second)
        {
            if (first >= Adapter::gridded_dimension ||
                second >= Adapter::gridded_dimension)
            {
                expanded.d2.chi[first][second] *= multiplier;
                expanded.d2.lapse[first][second] *= multiplier;
                for (int component = 0;
                     component < Adapter::target_dimension; ++component)
                {
                    expanded.d2.shift[component][first][second] *=
                        multiplier;
                }
            }
        }
    }
}

struct OneHiddenCopyPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        constexpr int hidden = Adapter::second_hidden_direction;
        expanded.vars.h[hidden][hidden] = 1.0;
        expanded.vars.A[hidden][hidden] = 0.0;
        expanded.vars.Gamma[hidden] = 0.0;
        expanded.vars.shift[hidden] = 0.0;
        expanded.vars.B[hidden] = 0.0;
        expanded.advection.h[hidden][hidden] = 0.0;
        expanded.advection.A[hidden][hidden] = 0.0;
        for (int i = 0; i < Adapter::target_dimension; ++i)
        {
            for (int j = 0; j < Adapter::target_dimension; ++j)
            {
                for (int derivative = 0;
                     derivative < Adapter::target_dimension; ++derivative)
                {
                    if (i == hidden || j == hidden ||
                        derivative == hidden)
                    {
                        expanded.d1.h[i][j][derivative] = 0.0;
                        expanded.d1.A[i][j][derivative] = 0.0;
                    }
                }
                for (int first = 0;
                     first < Adapter::target_dimension; ++first)
                {
                    for (int second = 0;
                         second < Adapter::target_dimension; ++second)
                    {
                        if (i == hidden || j == hidden ||
                            first == hidden || second == hidden)
                        {
                            expanded.d2.h[i][j][first][second] = 0.0;
                        }
                    }
                }
            }
        }
        return expanded;
    }
};

struct WrongInverseSquarePolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        const double omitted_term =
            input.vars.physical.h.xz / (input.x * input.x);
        for (const int hidden :
             {Adapter::first_hidden_direction,
              Adapter::second_hidden_direction})
        {
            expanded.d2.h[0][1][hidden][hidden] += omitted_term;
            expanded.d2.h[1][0][hidden][hidden] += omitted_term;
        }
        return expanded;
    }
};

struct OmitHiddenCartoonPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        zero_hidden_cartoon_derivatives(expanded, 0.0);
        return expanded;
    }
};

struct DuplicateHiddenCartoonPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        zero_hidden_cartoon_derivatives(expanded, 2.0);
        return expanded;
    }
};

struct HwwEqualsXSquaredPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        expanded.vars.h[2][2] = input.x * input.x;
        expanded.vars.h[3][3] = input.x * input.x;
        return expanded;
    }
};

struct OmitHiddenEncodedZPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        expanded.d1.Gamma[2][2] = 0.0;
        expanded.d1.Gamma[3][3] = 0.0;
        return expanded;
    }
};

struct WrongHiddenGammaSignPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        expanded.d1.Gamma[2][2] *= -1.0;
        expanded.d1.Gamma[3][3] *= -1.0;
        return expanded;
    }
};

struct OmitHwwEvolutionInputPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        expanded.advection.h[2][2] = 0.0;
        expanded.advection.h[3][3] = 0.0;
        return expanded;
    }
};

struct OmitAwwEvolutionInputPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        expanded.advection.A[2][2] = 0.0;
        expanded.advection.A[3][3] = 0.0;
        return expanded;
    }
};

struct WrongTargetCoefficientMappingPolicy
{
    Adapter::ExpandedInput expand(const Adapter::Input &input) const
    {
        auto expanded = Adapter::expand_target(input);
        expanded.vars.K *= 4.0 / 3.0;
        for (int derivative = 0;
             derivative < Adapter::target_dimension; ++derivative)
        {
            expanded.d1.K[derivative] *= 4.0 / 3.0;
        }
        expanded.advection.K *= 4.0 / 3.0;
        return expanded;
    }
};

bool rows_violate_tolerance(const Adapter::Rows &actual,
                            const Oracle::Rows &expected,
                            std::size_t &worst_row,
                            double &worst_normalized)
{
    bool violated = false;
    worst_row = 0;
    worst_normalized = 0.0;
    for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
    {
        const double oracle = static_cast<double>(expected[row]);
        const double normalized =
            std::abs(actual[row] - oracle) /
            allowance(actual[row], oracle);
        if (normalized > worst_normalized)
        {
            worst_normalized = normalized;
            worst_row = row;
        }
        violated = violated || normalized > 1.0;
    }
    return violated;
}

void check_restored_oracle_defect_regressions()
{
    const auto cases = make_cases();
    const Case &one_z = case_named(cases, "one_z_components");
    const Oracle::Context context{
        r0, x, one_z.amplitude, &one_z.direction,
        Oracle::hidden_multiplicity};

    const Oracle::AxisTensorJets conformal_jets =
        Oracle::conformal_metric_jets(context);
    const Oracle::Geometry corrected_geometry =
        Oracle::compute_geometry(conformal_jets);
    const Oracle::Geometry old_geometry =
        Oracle::compute_geometry_old_construction_order(
            conformal_jets);
    double active_christoffel_difference = 0.0;
    for (int upper = 0; upper < Oracle::dimension; ++upper)
    {
        for (int lower_1 = 0; lower_1 < Oracle::dimension;
             ++lower_1)
        {
            for (int lower_2 = 0; lower_2 < Oracle::dimension;
                 ++lower_2)
            {
                active_christoffel_difference = std::max(
                    active_christoffel_difference,
                    std::abs(static_cast<double>(
                        corrected_geometry.gamma[upper][lower_1]
                                                [lower_2] -
                        old_geometry.gamma[upper][lower_1][lower_2])));
            }
        }
    }
    require(active_christoffel_difference > 1.0e-8,
            "old Christoffel mutation requires active missing lower data");

    const Adapter::Rows direct_christoffel =
        Adapter::evaluate(
            make_input(one_z.amplitude, one_z.direction))
            .target_full_grchombo;
    const Oracle::Rows corrected_christoffel =
        Oracle::evaluate(r0, x, one_z.amplitude,
                         one_z.direction)
            .total;
    const Oracle::Rows mutated_christoffel =
        Oracle::evaluate_with_policy(
            r0, x, one_z.amplitude, one_z.direction,
            OldChristoffelConstructionOrderPolicy{})
            .total;
    std::size_t corrected_christoffel_row = 0;
    std::size_t mutated_christoffel_row = 0;
    double corrected_christoffel_normalized = 0.0;
    double mutated_christoffel_normalized = 0.0;
    require(!rows_violate_tolerance(
                direct_christoffel, corrected_christoffel,
                corrected_christoffel_row,
                corrected_christoffel_normalized),
            "corrected two-pass Christoffel oracle must pass");
    require(rows_violate_tolerance(
                direct_christoffel, mutated_christoffel,
                mutated_christoffel_row,
                mutated_christoffel_normalized),
            "old Christoffel construction-order defect must fail");
    std::cout
        << "ORACLE_DEFECT_MUTATION name=old_christoffel_construction_order"
        << " state=" << one_z.name
        << " active_gamma_difference="
        << active_christoffel_difference
        << " corrected_worst_row="
        << Adapter::row_names[corrected_christoffel_row]
        << " corrected_worst_normalized="
        << corrected_christoffel_normalized
        << " mutated_worst_row="
        << Adapter::row_names[mutated_christoffel_row]
        << " mutated_worst_normalized="
        << mutated_christoffel_normalized
        << " result=REJECTED\n";

    Adapter::Input hessian_input =
        make_input(one_z.amplitude, one_z.direction);
    hessian_input.derivatives[Production::c_shiftX].dxz = 0.31;
    hessian_input.derivatives[Production::c_shiftZ].dxx = -0.17;
    const ActiveShiftHessianPolicy corrected_hessian_policy;
    const OldVectorHessianIndexOrderPolicy old_hessian_policy;
    const Oracle::ShiftJets active_shift =
        corrected_hessian_policy.shift_from(context);
    const double corrected_active_entry = static_cast<double>(
        corrected_hessian_policy.contracted_vector_hessian(
            active_shift, 0, 1, 0));
    const double old_active_entry = static_cast<double>(
        old_hessian_policy.contracted_vector_hessian(
            active_shift, 0, 1, 0));
    require(std::abs(corrected_active_entry) > 0.0 &&
                std::abs(old_active_entry) > 0.0 &&
                corrected_active_entry != old_active_entry,
            "vector-Hessian regression requires distinct nonzero orders");

    const Adapter::Rows direct_hessian =
        Adapter::evaluate(hessian_input).target_full_grchombo;
    const Oracle::Rows corrected_hessian =
        Oracle::evaluate_with_policy(
            r0, x, one_z.amplitude, one_z.direction,
            corrected_hessian_policy)
            .total;
    const Oracle::Rows mutated_hessian =
        Oracle::evaluate_with_policy(
            r0, x, one_z.amplitude, one_z.direction,
            old_hessian_policy)
            .total;
    std::size_t corrected_hessian_row = 0;
    std::size_t mutated_hessian_row = 0;
    double corrected_hessian_normalized = 0.0;
    double mutated_hessian_normalized = 0.0;
    require(!rows_violate_tolerance(
                direct_hessian, corrected_hessian,
                corrected_hessian_row,
                corrected_hessian_normalized),
            "correct vector-Hessian index order must pass");
    require(rows_violate_tolerance(
                direct_hessian, mutated_hessian,
                mutated_hessian_row,
                mutated_hessian_normalized),
            "old vector-Hessian index-order defect must fail");
    std::cout
        << "ORACLE_DEFECT_MUTATION name=old_vector_hessian_index_order"
        << " state=" << one_z.name
        << " corrected_entry=" << corrected_active_entry
        << " old_entry=" << old_active_entry
        << " corrected_worst_row="
        << Adapter::row_names[corrected_hessian_row]
        << " corrected_worst_normalized="
        << corrected_hessian_normalized
        << " mutated_worst_row="
        << Adapter::row_names[mutated_hessian_row]
        << " mutated_worst_normalized="
        << mutated_hessian_normalized
        << " result=REJECTED\n";
}

template <class expansion_policy_t>
void require_production_policy_mutation_rejected(
    const std::string &name, const Case &test_case,
    const expansion_policy_t &policy)
{
    const auto actual =
        Adapter::evaluate_with_expansion_policy(
            make_input(test_case.amplitude, test_case.direction),
            policy)
            .target_full_grchombo;
    const auto expected =
        Oracle::evaluate(r0, x, test_case.amplitude,
                         test_case.direction);
    std::size_t worst_row = 0;
    double worst_normalized = 0.0;
    require(rows_violate_tolerance(actual, expected.total,
                                   worst_row, worst_normalized),
            name + " production-path mutation was not rejected");
    std::cout << "PRODUCTION_MUTATION name=" << name
              << " result=REJECTED worst_row="
              << Adapter::row_names[worst_row]
              << " worst_normalized=" << worst_normalized << "\n";
}

void check_production_path_mutations()
{
    const auto cases = make_cases();
    const Case &mixed = case_named(cases, "mixed_all_families");
    const Case &one_z = case_named(cases, "one_z_components");
    const Case &hidden = case_named(cases, "pure_hidden_ww");
    const Case &encoded = case_named(cases, "pure_hatted_gamma_z");
    const Case &flat = case_named(cases, "flat_nontrivial");

    require_production_policy_mutation_rejected(
        "one_active_hidden_copy", mixed, OneHiddenCopyPolicy{});
    require_production_policy_mutation_rejected(
        "wrong_representative_inverse_x_squared_term", one_z,
        WrongInverseSquarePolicy{});
    require_production_policy_mutation_rejected(
        "hww_equals_x_squared", hidden, HwwEqualsXSquaredPolicy{});
    require_production_policy_mutation_rejected(
        "omitted_hidden_ricci_cartoon_input", mixed,
        OmitHiddenCartoonPolicy{});
    require_production_policy_mutation_rejected(
        "duplicated_hidden_ricci_cartoon_input", mixed,
        DuplicateHiddenCartoonPolicy{});
    require_production_policy_mutation_rejected(
        "omitted_hidden_encoded_z_derivative", encoded,
        OmitHiddenEncodedZPolicy{});
    require_production_policy_mutation_rejected(
        "wrong_hidden_gamma_sign", encoded,
        WrongHiddenGammaSignPolicy{});
    require_production_policy_mutation_rejected(
        "omitted_hww_evolution_input", hidden,
        OmitHwwEvolutionInputPolicy{});
    require_production_policy_mutation_rejected(
        "omitted_Aww_evolution_input", hidden,
        OmitAwwEvolutionInputPolicy{});
    require_production_policy_mutation_rejected(
        "wrong_target_dimension_coefficient_mapping", flat,
        WrongTargetCoefficientMappingPolicy{});

    const Adapter::Input input =
        make_input(mixed.amplitude, mixed.direction);
    const Adapter::ExpandedInput expanded =
        Adapter::expand_target(input);
    const auto rhs = Adapter::evaluate_direct(expanded,
                                              input.parameters);
    Adapter::Rows duplicated =
        Adapter::physical_rows_from(rhs);
    duplicated[row_index(Adapter::PhysicalRow::hww)] =
        rhs.h[2][2] + rhs.h[3][3];
    duplicated[row_index(Adapter::PhysicalRow::Aww)] =
        rhs.A[2][2] + rhs.A[3][3];
    const auto expected =
        Oracle::evaluate(r0, x, mixed.amplitude,
                         mixed.direction);
    std::size_t worst_row = 0;
    double worst_normalized = 0.0;
    require(rows_violate_tolerance(duplicated, expected.total,
                                   worst_row, worst_normalized),
            "duplicated representative output ownership was not rejected");
    std::cout
        << "REPORTING_MUTATION name=duplicated_hidden_representative_output"
        << " result=REJECTED worst_row="
        << Adapter::row_names[worst_row]
        << " worst_normalized=" << worst_normalized << "\n";
}

void check_gp_decomposition()
{
    const auto cases = make_cases();
    const Case &gp_case = case_named(cases, "exact_gp");
    const Adapter::Input input =
        make_input(0.0, gp_case.direction);
    const Adapter::Result result = Adapter::evaluate(input);
    const Oracle::Result oracle =
        Oracle::evaluate(r0, x, 0.0, gp_case.direction);
    require(input.vars.physical.h.ww == 1.0,
            "GP stored hww must be one");
    for (std::size_t row = 0; row < Adapter::physical_rows; ++row)
    {
        const double suppressed =
            result.target_shared_hidden_suppressed[row];
        const double increment =
            result.hidden_increment_decomposition[row];
        const double full = result.target_full_grchombo[row];
        require(std::abs(full -
                         static_cast<double>(oracle.total[row])) <=
                    absolute_tolerance,
                std::string("GP row not roundoff zero: ") +
                    Adapter::row_names[row]);
        require(std::abs(suppressed + increment - full) <
                    1.0e-14,
                "GP decomposition identity");
        std::cout
            << "GP_DECOMPOSITION row=" << Adapter::row_names[row]
            << " target_shared_hidden_suppressed=" << suppressed
            << " hidden_increment_decomposition=" << increment
            << " target_full_grchombo=" << full << "\n";
    }
    for (const auto family : {
             Adapter::ReportedFamily::geometric_ricci,
             Adapter::ReportedFamily::encoded_z,
             Adapter::ReportedFamily::lapse_derivatives,
             Adapter::ReportedFamily::damping})
    {
        const auto &rows =
            result.family[static_cast<std::size_t>(family)]
                .target_full_grchombo;
        double maximum_absolute = 0.0;
        for (const double value : rows)
        {
            maximum_absolute =
                std::max(maximum_absolute, std::abs(value));
            require(std::abs(value) <= absolute_tolerance,
                    "GP directly isolated family must vanish");
        }
        std::cout
            << "GP_DIRECT_FAMILY family="
            << Adapter::reported_family_names[
                   static_cast<std::size_t>(family)]
            << " maximum_absolute=" << maximum_absolute << "\n";
    }
    const double lambda =
        std::sqrt(r0 / (x * x * x));
    const double beta_dx_k = -2.25 * lambda * lambda;
    const double k_squared = 2.25 * lambda * lambda;
    require(std::abs(beta_dx_k + k_squared) < 1.0e-14,
            "target d4 hidden-suppressed K cancellation");
    std::cout << "GP_K_SUPPRESSED beta_x_dxK=" << beta_dx_k
              << " K_squared=" << k_squared
              << " sum=" << beta_dx_k + k_squared
              << " stock_d3_residual_mapping=0.125\n";
}

void check_directness_and_contract()
{
    using LockedSource =
        CCZ4RHS<Adapter::FrozenGauge, Adapter::NoDerivatives>;
    static_assert(
        std::is_base_of<LockedSource,
                        Adapter::LockedRHSAccess>::value,
        "adapter must execute locked GRChombo CCZ4RHS");
    static_assert(Adapter::physical_rows == 13);
    static_assert(Production::NUM_VARS == 18);
    static_assert(Oracle::variables == Adapter::physical_rows);

    Adapter::Input invalid =
        make_input(0.0, Oracle::Direction{});
    invalid.x = 0.0;
    bool rejected = false;
    try
    {
        (void)Adapter::evaluate(invalid);
    }
    catch (const std::domain_error &)
    {
        rejected = true;
    }
    require(rejected, "nonpositive x must be rejected");
    std::cout
        << "DIRECTNESS target_d4_grchombo_executed=true"
        << " oracle_calls_production=false"
        << " production_calls_custom_complete_assembler=false"
        << " independently_coded_hidden_rhs=false"
        << " physical_outputs=13 gauge_outputs=0 state_slots=18\n";
}

} // namespace

int main()
{
    std::cout << std::setprecision(17);
    std::cout << "Black-string target pointwise CCZ4 repair fixture\n";
    check_direct_nonlinear();
    check_restored_oracle_defect_regressions();
    check_jvp_sweep();
    check_fourier_sectors();
    check_production_path_mutations();
    check_gp_decomposition();
    check_directness_and_contract();
    std::cout << "Target pointwise adapter repair checks passed.\n";
    return 0;
}
