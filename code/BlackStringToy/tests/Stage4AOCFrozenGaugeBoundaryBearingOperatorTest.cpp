#include "Stage4AOFrozenGaugeRadialBoundary.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace Boundary = BlackStringToy::Stage4AOFrozenGaugeRadialBoundary;
namespace Inner = BlackStringToy::Stage4AOFrozenGaugeInnerBoundary;
namespace Outer = BlackStringToy::Stage4AOFrozenGaugeOuterProjector;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
using Sector = Boundary::Sector;
using Vector = Boundary::Vector;

namespace
{
void fail(const std::string &label)
{
    throw std::runtime_error(label);
}

void require_true(const std::string &label, const bool condition)
{
    if (!condition)
    {
        fail(label);
    }
    std::cout << "PASS " << label << '\n';
}

void require_close(const std::string &label, const double actual,
                   const double expected, const double tolerance)
{
    if (!std::isfinite(actual) || std::abs(actual - expected) > tolerance)
    {
        fail(label);
    }
    std::cout << "PASS " << label << '\n';
}

template <class Function>
void require_domain_error(const std::string &label, Function &&function)
{
    bool rejected = false;
    try
    {
        function();
    }
    catch (const std::domain_error &)
    {
        rejected = true;
    }
    require_true(label, rejected);
}

std::vector<Vector> analytic_radial_data(const Operator::RadialGrid &grid)
{
    std::vector<Vector> data;
    data.reserve(grid.points());
    for (std::size_t index = 0; index < grid.points(); ++index)
    {
        const double x = grid.x(index);
        Boundary::Rows values = {};
        for (std::size_t slot = 0; slot < values.size(); ++slot)
        {
            const double coefficient =
                (slot % 2 == 0 ? 1.0 : -1.0) *
                (0.03 + 0.007 * static_cast<double>(slot));
            values[slot] = coefficient *
                           std::exp(-(0.12 + 0.01 * slot) * x) *
                           (1.0 + 0.02 * x * x);
        }
        data.push_back(Operator::make_frozen_gauge_perturbation_vector(values));
    }
    return data;
}

double row_norm(const std::array<double, 4> &rows)
{
    double result = 0.0;
    for (const double value : rows)
    {
        result += value * value;
    }
    return std::sqrt(result);
}

void check_diagnostic_row_layout_and_basis_invariance()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 12.5);
    const auto grid = Operator::make_radial_grid(domain, 48);
    const auto data = analytic_radial_data(grid);
    constexpr double k = 0.8;
    for (const auto sector : {Sector::P_plus, Sector::P_minus})
    {
        const auto result = Boundary::apply_diagnostic_radial_row_layout(
            grid, data, k, sector);
        require_true("radial point count retained",
                     result.radial_points() == grid.points());
        require_true("thirteen equations per radial point",
                     result.equations_per_point() == 13);
        require_true("total radial equation count exact",
                     result.total_equations() == 13 * grid.points());
        require_true("no radial ghost unknowns",
                     !result.radial_ghost_unknowns_stored());
        require_true("sector identity retained", result.sector() == sector);
        for (std::size_t index = 0; index < result.points().size(); ++index)
        {
            const auto &point = result.points()[index];
            if (index == 0)
            {
                require_true("inner owns thirteen PDE rows",
                             point.owner == Boundary::RadialRowOwner::inner_pde &&
                                 point.pde_rows == 13 &&
                                 point.boundary_rows == 0);
            }
            else if (index + 1 == result.points().size())
            {
                require_true("diagnostic outer layout records four plus nine rows",
                             point.owner ==
                                     Boundary::RadialRowOwner::
                                         diagnostic_outer_projected_pde_and_characteristic_rows &&
                                 point.pde_rows == 4 &&
                                 point.boundary_rows == 9);
            }
            else
            {
                require_true("interior owns thirteen centered PDE rows",
                             point.owner ==
                                     Boundary::RadialRowOwner::
                                         centered_interior_pde &&
                                 point.pde_rows == 13 &&
                                 point.boundary_rows == 0);
            }
            for (const double equation : point.equations)
            {
                require_true("assembled radial equation finite",
                             std::isfinite(equation));
            }
        }

        const auto base_basis =
            Outer::make_diagnostic_characteristic_basis(1.0, 12.5, k);
        Outer::DiagnosticCharacteristicBasis mixed = {};
        const std::array<std::array<double, 4>, 4> transform = {{
            {{2.0, 0.2, -0.1, 0.3}},
            {{-0.4, 1.7, 0.3, -0.2}},
            {{0.1, -0.3, 1.4, 0.2}},
            {{0.2, 0.1, -0.4, 0.9}},
        }};
        for (std::size_t output = 0; output < 4; ++output)
        {
            for (std::size_t input = 0; input < 4; ++input)
            {
                for (std::size_t row = 0; row < 13; ++row)
                {
                    mixed[output][row] +=
                        transform[output][input] * base_basis[input][row];
                }
            }
        }
        const auto jet = Boundary::make_outer_derivative_jet(
            data, grid.dx(), k, sector);
        const auto pde = Inner::apply_complete_interior_from_jet(
            1.0, 12.5, jet);
        const auto base_rows = Boundary::diagnostic_retained_pde_residuals(
            1.0, 12.5, k, sector, pde, base_basis);
        const auto mixed_rows = Boundary::diagnostic_retained_pde_residuals(
            1.0, 12.5, k, sector, pde, mixed);
        require_close("retained PDE equation space basis invariant",
                      row_norm(base_rows), row_norm(mixed_rows), 2.0e-12);
        const auto base_outer = Boundary::apply_diagnostic_outer_row_layout(
            grid, data, k, sector, base_basis);
        const auto mixed_outer = Boundary::apply_diagnostic_outer_row_layout(
            grid, data, k, sector, mixed);
        for (std::size_t row = 0; row < 9; ++row)
        {
            require_close("boundary residual subspace basis invariant",
                          base_outer.boundary_residuals()[row],
                          mixed_outer.boundary_residuals()[row], 2.0e-13);
        }
        require_true("outer resets no field", !base_outer.physical_values_reset());
        require_true("outer stores no ghost unknown",
                     !base_outer.radial_ghost_unknowns_stored());
    }
}

void check_inner_location_and_reflection()
{
    std::cout << "INFO inner location speeds\n";
    for (const double ratio : {0.35, 0.50, 0.65, 0.80})
    {
        const auto report = Inner::make_inner_outflow_report(1.0, ratio);
        std::cout << "  " << ratio << ' ' << report.c_plus() << ' '
                  << report.c_minus() << ' ' << report.c_zero() << '\n';
        require_true("inner location pure outflow",
                     report.pure_outflow() && report.c_plus() < 0.0 &&
                         report.c_minus() < 0.0 && report.c_zero() < 0.0);
    }
    require_true("glancing placement rejected for acceptance",
                 Inner::make_inner_outflow_report(1.0, 1.0).glancing());
    require_true("non-outflow placement rejected",
                 !Inner::make_inner_outflow_report(1.0, 1.1)
                      .valid_for_no_data_contract());

    constexpr double kx = 0.7;
    double finest = 0.0;
    std::cout << "INFO inner endpoint reflection diagnostic\n";
    for (const int denominator : {64, 128, 256, 512, 1024, 2048})
    {
        const double dx = 1.0 / denominator;
        const double theta = kx * dx;
        const double real_d1 =
            (-3.0 + 4.0 * std::cos(theta) - std::cos(2.0 * theta)) /
            (2.0 * dx);
        const double imag_d1 =
            (4.0 * std::sin(theta) - std::sin(2.0 * theta)) /
            (2.0 * dx);
        const double d1_defect =
            std::hypot(real_d1, imag_d1 - kx) / kx;
        const double reflection = d1_defect * dx;
        std::cout << "  " << dx << ' ' << std::scientific
                  << std::setprecision(12) << reflection << '\n';
        finest = reflection;
    }
    require_true("inner finest reflection below 1e-6", finest < 1.0e-6);
}

void check_constraint_cleanup_at_both_endpoints()
{
    Boundary::Rows values =
        {0.17, -0.21, 0.13, 0.29, -0.31, 0.37, -0.41,
         0.43, 0.47, -0.53, 0.59, -0.61, 0.67};
    const auto input = Operator::make_frozen_gauge_perturbation_vector(values);
    for (const double x : {0.5, 12.5})
    {
        const auto cleaned =
            Inner::project_inner_endpoint_algebraic_constraints(1.0, x, input);
        require_close("determinant tangent cleanup at endpoint",
                      Inner::metric_determinant_tangent_residual(cleaned), 0.0,
                      2.0e-15);
        require_close("metric-inclusive weighted A trace at endpoint",
                      Operator::delta_trace_a_at_point(1.0, x, cleaned), 0.0,
                      3.0e-15);
    }
}

void check_guards_and_gates()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 12.5);
    const auto grid = Operator::make_radial_grid(domain, 8);
    const auto data = analytic_radial_data(grid);
    require_domain_error("diagnostic radial layout rejects k=0", [&]() {
        (void)Boundary::apply_diagnostic_radial_row_layout(
            grid, data, 0.0, Sector::P_plus);
    });
    auto short_data = data;
    short_data.pop_back();
    require_domain_error("diagnostic radial layout rejects grid mismatch", [&]() {
        (void)Boundary::apply_diagnostic_radial_row_layout(
            grid, short_data, 0.8, Sector::P_plus);
    });
    require_true("diagnostic radial row-layout flag true",
                 Boundary::diagnostic_radial_row_layout_implemented);
    require_true("aggregate radial boundary flag false",
                 !Operator::radial_boundary_system_complete);
    require_true("boundary derivative validation flag false",
                 !Operator::boundary_derivative_validation_implemented);
    require_true("boundary-bearing complete operator flag false",
                 !Operator::complete_frozen_gauge_operator_implemented);
    require_true("exact quadratic boundary pencil flag false",
                 !Operator::
                     quadratic_pencil_coefficient_representation_implemented);
    require_true("eigensolver still false", !Operator::eigensolver_implemented);
    require_true("threshold still false",
                 !Operator::threshold_search_implemented);
    require_true("production still false",
                 !Operator::production_rhs_wiring_implemented);
    require_true("Stage 4AO-D still false",
                 !Operator::live_gauge_stage_4ao_d_implemented);
}
} // namespace

int main()
{
    try
    {
        check_diagnostic_row_layout_and_basis_invariance();
        check_inner_location_and_reflection();
        check_constraint_cleanup_at_both_endpoints();
        check_guards_and_gates();
    }
    catch (const std::exception &error)
    {
        std::cerr << "FAIL " << error.what() << '\n';
        return 1;
    }
    std::cout << "PASS Stage 4AO-C diagnostic radial row-layout scaffolding\n";
    return 0;
}
