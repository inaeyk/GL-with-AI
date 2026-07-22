#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace
{
namespace Completion =
    BlackStringToy::Stage4AOFrozenGaugeZRicciCompletion;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace RicciAssembly = BlackStringToy::Stage4AOFrozenGaugeRicciAssembly;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;
using Tensor = Completion::CompletionTensor;

constexpr double tolerance = 1.0e-12;

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
                   const double allowed_error = tolerance)
{
    const double error = std::abs(actual - expected);
    if (error > allowed_error)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " error=" + std::to_string(error));
    }
    std::cout << "PASS " << label << "\n";
}

void require_separated(const std::string &label, const double actual,
                       const double mutation)
{
    if (std::abs(actual - mutation) <= 1.0e-3)
    {
        fail(label, "mutation was not separated from the actual result");
    }
    std::cout << "PASS " << label << "\n";
}

template <class Function>
void require_domain_error(const std::string &label, Function &&function)
{
    try
    {
        function();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    catch (const std::exception &exception)
    {
        fail(label, std::string("wrong exception: ") + exception.what());
    }
    fail(label, "expected std::domain_error");
}

Tensor make_fixed_completion()
{
    return Completion::compute(Completion::make_encoded_z_derivative_jet(
        2.0, 3.0, 5.0, 7.0, 11.0, 13.0, 17.0));
}

Tensor make_parity_completion(const double x, const double z)
{
    const double wave_number = 3.0;
    const double cosine = std::cos(wave_number * z);
    const double sine = std::sin(wave_number * z);
    const double z_x = x * x * cosine;
    const double z_z = x * sine;
    const double dx_z_x = 2.0 * x * cosine;
    const double dz_z_x = -wave_number * x * x * sine;
    const double dx_z_z = sine;
    const double dz_z_z = wave_number * x * cosine;
    return Completion::compute(Completion::make_encoded_z_derivative_jet(
        x, z_x, z_z, dx_z_x, dz_z_x, dx_z_z, dz_z_z));
}

void require_only(const std::string &label, const Vector &output,
                  const std::array<Variable, 4> &allowed,
                  const std::size_t allowed_count)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        bool is_allowed = false;
        for (std::size_t i = 0; i < allowed_count; ++i)
        {
            is_allowed = is_allowed || variable == allowed[i];
        }
        if (!is_allowed)
        {
            require_close(label + " leaves " +
                              std::string(Operator::variable_name(variable)) +
                              " untouched",
                          output.value(variable), 0.0, 0.0);
        }
    }
}

void check_typed_ownership_contract()
{
    using KFunction = Vector (*)(const Tensor &);
    using ThetaFunction = Vector (*)(const Tensor &);
    using AFunction = Vector (*)(const Tensor &);
    using GeometricRicci =
        RicciAssembly::TraceFreeRicciAssembly;

    static_assert(std::is_invocable<KFunction, const Tensor &>::value,
                  "K insertion consumes the encoded-Z completion");
    static_assert(std::is_invocable<ThetaFunction, const Tensor &>::value,
                  "Theta insertion consumes the encoded-Z completion");
    static_assert(std::is_invocable<AFunction, const Tensor &>::value,
                  "A insertion consumes the encoded-Z completion");
    static_assert(!std::is_invocable<KFunction,
                                     const GeometricRicci &>::value,
                  "K encoded-Z insertion must not accept geometric Ricci");
    static_assert(!std::is_invocable<ThetaFunction,
                                     const GeometricRicci &>::value,
                  "Theta encoded-Z insertion must not accept geometric Ricci");
    static_assert(!std::is_invocable<AFunction,
                                     const GeometricRicci &>::value,
                  "A encoded-Z insertion must not accept geometric Ricci");

    static_assert(
        std::is_same<
            decltype(
                &Operator::apply_k_equation_encoded_z_ricci_insertion_at_point),
            KFunction>::value,
        "K insertion has the encoded-Z-only signature");
    static_assert(
        std::is_same<
            decltype(&Operator::
                         apply_theta_equation_encoded_z_ricci_insertion_at_point),
            ThetaFunction>::value,
        "Theta insertion has the encoded-Z-only signature");
    static_assert(
        std::is_same<
            decltype(
                &Operator::apply_a_equation_encoded_z_ricci_insertion_at_point),
            AFunction>::value,
        "A insertion has the encoded-Z-only signature");
    require_true("typed encoded-Z-only insertion functions are available",
                 true);
}

void check_isolated_point_insertions()
{
    const auto completion = make_fixed_completion();
    const auto k_output =
        Operator::apply_k_equation_encoded_z_ricci_insertion_at_point(
            completion);
    const auto theta_output =
        Operator::apply_theta_equation_encoded_z_ricci_insertion_at_point(
            completion);
    const auto a_output =
        Operator::apply_a_equation_encoded_z_ricci_insertion_at_point(
            completion);

    require_close("K receives isolated +q", k_output.value(Variable::K),
                  completion.scalar_trace());
    require_only("K insertion", k_output,
                 {Variable::K, Variable::K, Variable::K, Variable::K}, 1);

    require_close("Theta receives isolated +q/2",
                  theta_output.value(Variable::Theta),
                  0.5 * completion.scalar_trace());
    require_only(
        "Theta insertion", theta_output,
        {Variable::Theta, Variable::Theta, Variable::Theta, Variable::Theta},
        1);

    require_close("A_xx receives qTF_xx", a_output.value(Variable::A_xx),
                  completion.tf_xx());
    require_close("A_xz receives qTF_xz", a_output.value(Variable::A_xz),
                  completion.tf_xz());
    require_close("A_zz receives qTF_zz", a_output.value(Variable::A_zz),
                  completion.tf_zz());
    require_close("representative A_ww receives qTF_ww once",
                  a_output.value(Variable::A_ww), completion.tf_ww());
    require_only("A insertion", a_output,
                 {Variable::A_xx, Variable::A_xz, Variable::A_zz,
                  Variable::A_ww},
                 4);

    require_close("inserted A completion is weighted trace-free",
                  a_output.value(Variable::A_xx) +
                      a_output.value(Variable::A_zz) +
                      2.0 * a_output.value(Variable::A_ww),
                  0.0);
}

void check_mutations()
{
    const auto completion = make_fixed_completion();
    const auto k_output =
        Operator::apply_k_equation_encoded_z_ricci_insertion_at_point(
            completion);
    const auto theta_output =
        Operator::apply_theta_equation_encoded_z_ricci_insertion_at_point(
            completion);
    const auto a_output =
        Operator::apply_a_equation_encoded_z_ricci_insertion_at_point(
            completion);

    require_separated("wrong K coefficient q/2 rejected",
                      k_output.value(Variable::K),
                      0.5 * completion.scalar_trace());
    require_separated("wrong Theta coefficient q rejected",
                      theta_output.value(Variable::Theta),
                      completion.scalar_trace());
    require_separated("raw q_xx instead of qTF_xx rejected",
                      a_output.value(Variable::A_xx), completion.q_xx());
    require_separated("raw q_zz instead of qTF_zz rejected",
                      a_output.value(Variable::A_zz), completion.q_zz());
    require_separated("raw q_ww instead of qTF_ww rejected",
                      a_output.value(Variable::A_ww), completion.q_ww());
    require_separated(
        "double TF projection rejected for A_xx",
        a_output.value(Variable::A_xx),
        completion.tf_xx() - completion.scalar_trace() / 4.0);
    require_separated(
        "double TF projection rejected for A_ww",
        a_output.value(Variable::A_ww),
        completion.tf_ww() - completion.scalar_trace() / 4.0);
    require_separated("representative A_ww is not doubled",
                      a_output.value(Variable::A_ww),
                      2.0 * completion.tf_ww());

    const double synthetic_geometric_trace = 19.0;
    require_separated("duplicated geometric Ricci rejected in K",
                      k_output.value(Variable::K),
                      completion.scalar_trace() + synthetic_geometric_trace);
    require_separated(
        "duplicated geometric Ricci rejected in Theta",
        theta_output.value(Variable::Theta),
        0.5 * (completion.scalar_trace() + synthetic_geometric_trace));
    require_separated("duplicated geometric Ricci rejected in A_xx",
                      a_output.value(Variable::A_xx),
                      completion.tf_xx() + 3.0);
}

void check_grid_blocks()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 12);
    std::vector<Tensor> completions;
    completions.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        completions.push_back(make_parity_completion(grid.x(i), 0.37));
    }

    const auto k_result =
        Operator::apply_k_equation_encoded_z_ricci_insertion_block(
            grid, completions);
    const auto theta_result =
        Operator::apply_theta_equation_encoded_z_ricci_insertion_block(
            grid, completions);
    const auto a_result =
        Operator::apply_a_equation_encoded_z_ricci_insertion_block(
            grid, completions);

    require_true("K grid insertion remains partial",
                 k_result.partial_operator_only());
    require_true("Theta grid insertion remains partial",
                 theta_result.partial_operator_only());
    require_true("A grid insertion remains partial",
                 a_result.partial_operator_only());
    require_true("grid insertions have no boundary placeholders",
                 !k_result.boundary_values_are_placeholders() &&
                     !theta_result.boundary_values_are_placeholders() &&
                     !a_result.boundary_values_are_placeholders());

    double max_error = 0.0;
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        max_error = std::max(
            max_error,
            std::abs(k_result.at(i).value(Variable::K) -
                     completions[i].scalar_trace()));
        max_error = std::max(
            max_error,
            std::abs(theta_result.at(i).value(Variable::Theta) -
                     0.5 * completions[i].scalar_trace()));
        max_error = std::max(
            max_error,
            std::abs(a_result.at(i).value(Variable::A_xx) -
                     completions[i].tf_xx()));
        max_error = std::max(
            max_error,
            std::abs(a_result.at(i).value(Variable::A_xz) -
                     completions[i].tf_xz()));
        max_error = std::max(
            max_error,
            std::abs(a_result.at(i).value(Variable::A_zz) -
                     completions[i].tf_zz()));
        max_error = std::max(
            max_error,
            std::abs(a_result.at(i).value(Variable::A_ww) -
                     completions[i].tf_ww()));
    }
    require_close("all grid insertion values", max_error, 0.0);

    completions.pop_back();
    require_domain_error("K grid size mismatch", [&]() {
        (void)Operator::apply_k_equation_encoded_z_ricci_insertion_block(
            grid, completions);
    });
    require_domain_error("Theta grid size mismatch", [&]() {
        (void)Operator::apply_theta_equation_encoded_z_ricci_insertion_block(
            grid, completions);
    });
    require_domain_error("A grid size mismatch", [&]() {
        (void)Operator::apply_a_equation_encoded_z_ricci_insertion_block(
            grid, completions);
    });
}

void check_parity()
{
    const double x = 1.7;
    const double z = 0.31;
    const auto plus = make_parity_completion(x, z);
    const auto minus = make_parity_completion(x, -z);
    const auto k_plus =
        Operator::apply_k_equation_encoded_z_ricci_insertion_at_point(plus);
    const auto k_minus =
        Operator::apply_k_equation_encoded_z_ricci_insertion_at_point(minus);
    const auto theta_plus =
        Operator::apply_theta_equation_encoded_z_ricci_insertion_at_point(
            plus);
    const auto theta_minus =
        Operator::apply_theta_equation_encoded_z_ricci_insertion_at_point(
            minus);
    const auto a_plus =
        Operator::apply_a_equation_encoded_z_ricci_insertion_at_point(plus);
    const auto a_minus =
        Operator::apply_a_equation_encoded_z_ricci_insertion_at_point(minus);

    require_close("K scalar/even parity", k_plus.value(Variable::K),
                  k_minus.value(Variable::K));
    require_close("Theta scalar/even parity",
                  theta_plus.value(Variable::Theta),
                  theta_minus.value(Variable::Theta));
    require_close("A_xx even parity", a_plus.value(Variable::A_xx),
                  a_minus.value(Variable::A_xx));
    require_close("A_zz even parity", a_plus.value(Variable::A_zz),
                  a_minus.value(Variable::A_zz));
    require_close("A_ww even parity", a_plus.value(Variable::A_ww),
                  a_minus.value(Variable::A_ww));
    require_close("A_xz one-z-index odd parity",
                  a_plus.value(Variable::A_xz),
                  -a_minus.value(Variable::A_xz));
}

void check_completion_guards()
{
    require_true("encoded-Z insertion block is implemented",
                 Operator::
                     encoded_z_ricci_completion_insertion_block_implemented);
    require_true("K encoded-Z insertion is implemented",
                 Operator::k_equation_z_ricci_contributions_implemented);
    require_true("Theta encoded-Z insertion is implemented",
                 Operator::
                     theta_equation_z_ricci_contributions_implemented);
    require_true("A encoded-Z insertion is implemented",
                 Operator::a_equation_z_ricci_contributions_implemented);

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::hat_Gamma_x &&
            variable != Variable::hat_Gamma_z)
        {
            require_true(
                std::string(Operator::variable_name(variable)) +
                    " RHS remains incomplete",
                !Operator::variable_rhs_complete(variable));
        }
    }
    require_true("complete operator remains false",
                 !Operator::complete_frozen_gauge_operator_implemented);
    require_true("eigensolver remains false",
                 !Operator::eigensolver_implemented);
}
} // namespace

int main()
{
    check_typed_ownership_contract();
    check_isolated_point_insertions();
    check_mutations();
    check_grid_blocks();
    check_parity();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C encoded-Z Ricci row insertions\n";
    return 0;
}
