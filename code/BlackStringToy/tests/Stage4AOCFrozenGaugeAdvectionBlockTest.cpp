#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;
using ApplyResult = Operator::FrozenGaugeApplyResult;

constexpr double pi = 3.141592653589793238462643383279502884;

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
                   const double expected, const double tolerance)
{
    const double error = std::abs(actual - expected);
    if (error > tolerance)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " error=" + std::to_string(error) +
                        " tolerance=" + std::to_string(tolerance));
    }
}

template <class Function> void require_domain_error(const std::string &label,
                                                    Function &&function)
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

constexpr std::array<Variable, 13> expected_state_order = {
    Variable::chi,         Variable::h_xx,        Variable::h_xz,
    Variable::h_zz,        Variable::h_ww,        Variable::K,
    Variable::A_xx,        Variable::A_xz,        Variable::A_zz,
    Variable::A_ww,        Variable::Theta,       Variable::hat_Gamma_x,
    Variable::hat_Gamma_z};

double slot_scale(const std::size_t slot) { return 1.0 + 0.25 * slot; }

double radial_profile(const double x, const std::size_t slot)
{
    return slot_scale(slot) * (x * x + 0.5 * x + 1.0);
}

double radial_profile_dx(const double x, const std::size_t slot)
{
    return slot_scale(slot) * (2.0 * x + 0.5);
}

Vector make_radial_vector_at(const double x)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (std::size_t slot = 0; slot < values.size(); ++slot)
    {
        values[slot] = radial_profile(x, slot);
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

std::vector<Vector> make_radial_field(const Operator::RadialGrid &grid)
{
    std::vector<Vector> field;
    field.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        field.push_back(make_radial_vector_at(grid.x(i)));
    }
    return field;
}

void check_state_order_and_gauge_exclusion()
{
    static_assert(Operator::frozen_gauge_state_vector.size() == 13,
                  "Stage 4AO-C frozen vector has exactly 13 variables");
    static_assert(Vector::size() == 13,
                  "Stage 4AO-C vector package has exactly 13 slots");
    static_assert(!std::is_aggregate<Vector>::value,
                  "Stage 4AO-C frozen vector must not be an open aggregate");
    static_assert(!std::is_aggregate<ApplyResult>::value,
                  "Stage 4AO-C apply result must not be an open aggregate");
    static_assert(!std::is_constructible<
                      Vector, std::array<double, 18>>::value,
                  "Gauge variables must not fit into the 13-slot frozen vector");

    for (std::size_t i = 0; i < expected_state_order.size(); ++i)
    {
        if (Operator::frozen_gauge_state_vector[i] != expected_state_order[i] ||
            Operator::variable_index(expected_state_order[i]) != i)
        {
            fail("state-vector positional order",
                 "slot " + std::to_string(i) + " is not the locked order");
        }
    }
    require_true("state-vector order is exactly locked", true);
    require_true("gauge perturbations remain excluded",
                 Operator::excluded_gauge_perturbations.size() == 5);
}

void check_radial_advection_apply()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 64);
    const auto input = make_radial_field(grid);
    const auto result = Operator::apply_gp_shift_advection_block(grid, input);

    require_true("apply result is partial only", result.partial_operator_only());
    require_true("boundary values are placeholders",
                 result.boundary_values_are_placeholders());
    require_true("first valid point is the first interior point",
                 result.first_valid_index() == 1);
    require_true("last valid point is the last interior point",
                 result.last_valid_index() == grid.points() - 2);

    double max_error = 0.0;
    double wrong_beta_residual = 0.0;
    double wrong_sign_residual = 0.0;
    double wrong_order_residual = 0.0;

    for (std::size_t i = result.first_valid_index();
         i <= result.last_valid_index(); ++i)
    {
        const double x = grid.x(i);
        const double beta = Operator::beta_gp_x(domain.r0(), x);
        for (std::size_t slot = 0; slot < Operator::frozen_gauge_state_vector.size();
             ++slot)
        {
            const double expected = beta * radial_profile_dx(x, slot);
            const double actual = result.at(i).value_at_index(slot);
            max_error = std::max(max_error, std::abs(actual - expected));

            const double wrong_beta_expected = x * radial_profile_dx(x, slot);
            wrong_beta_residual =
                std::max(wrong_beta_residual,
                         std::abs(actual - wrong_beta_expected));

            wrong_sign_residual =
                std::max(wrong_sign_residual, std::abs(actual + expected));
        }

        const double h_xx_output = result.at(i).value(Variable::h_xx);
        const double h_xz_expected_if_order_swapped =
            beta * radial_profile_dx(x, Operator::variable_index(Variable::h_xz));
        wrong_order_residual =
            std::max(wrong_order_residual,
                     std::abs(h_xx_output - h_xz_expected_if_order_swapped));
    }

    require_close("GP-shift advection matches beta_GP^x d_x(delta u)",
                  max_error, 0.0, 2.0e-13);
    require_true("wrong beta profile negative guard would fail",
                 wrong_beta_residual > 1.0e-1);
    require_true("wrong derivative sign negative guard would fail",
                 wrong_sign_residual > 1.0);
    require_true("wrong variable ordering negative guard would fail",
                 wrong_order_residual > 1.0e-1);

    for (const auto boundary_index : {std::size_t{0}, grid.points() - 1})
    {
        for (std::size_t slot = 0; slot < Operator::frozen_gauge_state_vector.size();
             ++slot)
        {
            require_close("boundary placeholder is zero",
                          result.at(boundary_index).value_at_index(slot), 0.0,
                          0.0);
        }
    }
}

void check_zero_input()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 16);
    const std::array<double, Operator::frozen_gauge_state_vector.size()> zero = {};
    std::vector<Vector> input;
    input.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        input.push_back(Operator::make_frozen_gauge_perturbation_vector(zero));
    }

    const auto result = Operator::apply_gp_shift_advection_block(grid, input);
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        for (std::size_t slot = 0; slot < Operator::frozen_gauge_state_vector.size();
             ++slot)
        {
            require_close("zero input gives zero output",
                          result.at(i).value_at_index(slot), 0.0, 0.0);
        }
    }
    std::cout << "PASS zero input gives zero output\n";
}

void check_derivative_infrastructure()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    const auto grid = Operator::make_radial_grid(domain, 32);
    std::vector<double> radial_values;
    radial_values.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        const double x = grid.x(i);
        radial_values.push_back(x * x + 3.0 * x + 2.0);
    }

    for (std::size_t i = grid.first_interior_index();
         i <= grid.last_interior_index(); ++i)
    {
        const double expected = 2.0 * grid.x(i) + 3.0;
        require_close("radial central derivative is exact on quadratics",
                      Operator::radial_dx_interior(grid, radial_values, i),
                      expected, 1.0e-13);
    }

    require_domain_error("radial derivative rejects boundary", [&]() {
        (void)Operator::radial_dx_interior(grid, radial_values, 0);
    });

    const double period = 2.0 * pi;
    constexpr std::size_t z_points = 512;
    std::vector<double> z_values(z_points);
    for (std::size_t j = 0; j < z_points; ++j)
    {
        const double z = period * static_cast<double>(j) /
                         static_cast<double>(z_points);
        z_values[j] = std::sin(z);
    }

    double max_dz_error = 0.0;
    double max_dzz_error = 0.0;
    for (std::size_t j = 0; j < z_points; ++j)
    {
        const double z = period * static_cast<double>(j) /
                         static_cast<double>(z_points);
        max_dz_error =
            std::max(max_dz_error,
                     std::abs(Operator::periodic_dz(z_values, period, j) -
                              std::cos(z)));
        max_dzz_error =
            std::max(max_dzz_error,
                     std::abs(Operator::periodic_dzz(z_values, period, j) +
                              std::sin(z)));
    }
    require_true("periodic z first derivative is second-order accurate",
                 max_dz_error < 3.0e-5);
    require_true("periodic z second derivative is second-order accurate",
                 max_dzz_error < 3.0e-5);
}

void check_completion_guards()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("validation-only wrapper", Operator::validation_only);
    require_true("advection block implemented",
                 Operator::gp_shift_advection_block_implemented);
    require_true("complete frozen-gauge operator remains false",
                 !contract.complete_operator_implemented());
    require_true("eigensolver is still disallowed", !contract.eigensolver_allowed());
    require_true("shift-invert is still absent", !Operator::shift_invert_implemented);
    require_true("threshold search is still absent",
                 !Operator::threshold_search_implemented);
}

} // namespace

int main()
{
    check_state_order_and_gauge_exclusion();
    check_radial_advection_apply();
    check_zero_input();
    check_derivative_infrastructure();
    check_completion_guards();
    std::cout << "PASS Stage 4AO-C GP-shift advection block\n";
    return 0;
}
