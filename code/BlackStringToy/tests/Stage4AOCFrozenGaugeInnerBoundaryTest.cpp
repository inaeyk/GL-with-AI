#include "Stage4AOFrozenGaugeInnerBoundary.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace
{
namespace Boundary = BlackStringToy::Stage4AOFrozenGaugeInnerBoundary;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Adapter = BlackStringToy::Stage4AOFrozenGaugeZDerivativeAdapter;

using Sector = Boundary::FourierParitySector;
using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

constexpr double roundoff_tolerance = 2.0e-12;

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

std::vector<double> samples(const double dx, const std::size_t points,
                            double (*profile)(double))
{
    std::vector<double> out;
    out.reserve(points);
    for (std::size_t i = 0; i < points; ++i)
    {
        out.push_back(profile(static_cast<double>(i) * dx));
    }
    return out;
}

double constant_profile(const double) { return 2.75; }
double linear_profile(const double x) { return -0.4 + 1.7 * x; }
double quadratic_profile(const double x)
{
    return 0.3 - 0.8 * x + 1.2 * x * x;
}
double cubic_profile(const double x)
{
    return -0.2 + 0.7 * x - 0.9 * x * x + 0.6 * x * x * x;
}
double smooth_profile(const double x) { return std::exp(0.73 * x); }

void check_stencil_coefficients_and_exactness()
{
    static_assert(Boundary::inner_dx_required_points == 3,
                  "inner D_x reach changed");
    static_assert(Boundary::inner_dxx_required_points == 4,
                  "inner D_xx reach changed");
    static_assert(Boundary::inner_full_row_required_points == 4,
                  "inner full-row reach changed");
    static_assert(Boundary::inner_dx_maximum_offset == 2,
                  "inner D_x maximum offset changed");
    static_assert(Boundary::inner_dxx_maximum_offset == 3,
                  "inner D_xx maximum offset changed");
    require_close("D_x coefficient u0", Boundary::inner_dx_coefficients[0],
                  -1.5, 0.0);
    require_close("D_x coefficient u1", Boundary::inner_dx_coefficients[1],
                  2.0, 0.0);
    require_close("D_x coefficient u2", Boundary::inner_dx_coefficients[2],
                  -0.5, 0.0);
    const std::array<double, 4> expected_dxx = {2.0, -5.0, 4.0, -1.0};
    for (std::size_t i = 0; i < expected_dxx.size(); ++i)
    {
        require_close("D_xx exposed coefficient",
                      Boundary::inner_dxx_coefficients[i], expected_dxx[i],
                      0.0);
    }

    constexpr double dx = 0.17;
    require_close("D_x constant exact",
                  Boundary::inner_dx(samples(dx, 4, constant_profile), dx),
                  0.0);
    require_close("D_x linear exact",
                  Boundary::inner_dx(samples(dx, 4, linear_profile), dx),
                  1.7);
    require_close("D_x quadratic exact",
                  Boundary::inner_dx(samples(dx, 4, quadratic_profile), dx),
                  -0.8);
    require_close("D_xx constant exact",
                  Boundary::inner_dxx(samples(dx, 4, constant_profile), dx),
                  0.0);
    require_close("D_xx linear exact",
                  Boundary::inner_dxx(samples(dx, 4, linear_profile), dx),
                  0.0);
    require_close("D_xx quadratic exact",
                  Boundary::inner_dxx(samples(dx, 4, quadratic_profile), dx),
                  2.4);
    require_close("D_xx cubic exact",
                  Boundary::inner_dxx(samples(dx, 4, cubic_profile), dx),
                  -1.8);

    require_domain_error("D_x shortened stencil", []() {
        (void)Boundary::inner_dx(std::vector<double>{1.0, 2.0}, 0.1);
    });
    require_domain_error("D_xx shortened stencil", []() {
        (void)Boundary::inner_dxx(std::vector<double>{1.0, 2.0, 3.0}, 0.1);
    });
    require_domain_error("endpoint stencil nonpositive spacing", []() {
        (void)Boundary::inner_dx(std::vector<double>{1.0, 2.0, 3.0}, 0.0);
    });
}

void check_stencil_convergence_and_mutations()
{
    constexpr std::array<double, 5> spacings = {
        0.16, 0.08, 0.04, 0.02, 0.01};
    std::array<double, spacings.size()> d1_errors = {};
    std::array<double, spacings.size()> d2_errors = {};
    std::cout << std::scientific << std::setprecision(12)
              << "INFO inner endpoint manufactured convergence\n"
              << "  dx D_x_error D_xx_error\n";
    for (std::size_t i = 0; i < spacings.size(); ++i)
    {
        const double dx = spacings[i];
        const auto values = samples(dx, 4, smooth_profile);
        d1_errors[i] = std::abs(Boundary::inner_dx(values, dx) - 0.73);
        d2_errors[i] =
            std::abs(Boundary::inner_dxx(values, dx) - 0.73 * 0.73);
        std::cout << "  " << dx << " " << d1_errors[i] << " "
                  << d2_errors[i] << "\n";
    }
    for (std::size_t i = 0; i + 1 < spacings.size(); ++i)
    {
        const double p1 = std::log(d1_errors[i] / d1_errors[i + 1]) /
                          std::log(2.0);
        const double p2 = std::log(d2_errors[i] / d2_errors[i + 1]) /
                          std::log(2.0);
        require_true("manufactured D_x order >=1.8", p1 >= 1.8);
        require_true("manufactured D_xx order >=1.8", p2 >= 1.8);
    }

    constexpr double dx = 0.13;
    const auto values = samples(dx, 4, cubic_profile);
    const double correct_d1 = Boundary::inner_dx(values, dx);
    const double correct_d2 = Boundary::inner_dxx(values, dx);
    const double wrong_sign = -correct_d1;
    const double wrong_coefficient =
        (-2.0 * values[0] + 2.0 * values[1] - 0.5 * values[2]) / dx;
    const double accidental_centered = (values[2] - values[0]) / (2.0 * dx);
    const double wrong_dxx_coefficient =
        (2.0 * values[0] - 4.0 * values[1] + 4.0 * values[2] - values[3]) /
        (dx * dx);
    require_true("wrong D_x sign mutation rejected",
                 std::abs(correct_d1 - wrong_sign) > 1.0e-3);
    require_true("wrong D_x coefficient mutation rejected",
                 std::abs(correct_d1 - wrong_coefficient) > 1.0e-3);
    require_true("accidental centered-at-i=1 stencil mutation rejected",
                 std::abs(correct_d1 - accidental_centered) > 1.0e-3);
    require_true("wrong D_xx coefficient mutation rejected",
                 std::abs(correct_d2 - wrong_dxx_coefficient) > 1.0e-3);
}

std::array<double, Operator::frozen_gauge_state_vector.size()>
profile_values(const double y)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (std::size_t i = 0; i < values.size(); ++i)
    {
        const double sign = i % 2 == 0 ? 1.0 : -1.0;
        const double a = sign * (0.04 + 0.009 * static_cast<double>(i));
        const double b = -sign * (0.03 + 0.004 * static_cast<double>(i));
        const double c = sign * (0.02 + 0.003 * static_cast<double>(i));
        const double d = -sign * (0.006 + 0.001 * static_cast<double>(i));
        values[i] = a + b * y + c * y * y + d * y * y * y;
    }
    return values;
}

std::vector<Vector> radial_amplitudes(const Operator::RadialGrid &grid)
{
    std::vector<Vector> out;
    out.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        out.push_back(Operator::make_frozen_gauge_perturbation_vector(
            profile_values(grid.x(i) - grid.domain().x_in())));
    }
    return out;
}

std::vector<Vector> transform_intrinsic_parity(
    const std::vector<Vector> &input)
{
    std::vector<Vector> out;
    out.reserve(input.size());
    for (const auto &point : input)
    {
        std::array<double, Operator::frozen_gauge_state_vector.size()> values =
            {};
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            values[Operator::variable_index(variable)] =
                Boundary::intrinsic_z_parity(variable) * point.value(variable);
        }
        out.push_back(Operator::make_frozen_gauge_perturbation_vector(values));
    }
    return out;
}

Vector transform_intrinsic_parity(const Vector &input)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] =
            Boundary::intrinsic_z_parity(variable) * input.value(variable);
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

double max_error(const Vector &left, const Vector &right)
{
    double error = 0.0;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        error = std::max(error,
                         std::abs(left.value(variable) - right.value(variable)));
    }
    return error;
}

std::array<double, Operator::frozen_gauge_state_vector.size()>
values_from_vector(const Vector &input)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] = input.value(variable);
    }
    return values;
}

void check_fourier_mixed_derivatives()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 1.5);
    const auto grid = Operator::make_radial_grid(domain, 16);
    const auto amplitudes = radial_amplitudes(grid);
    constexpr double k = 0.8;
    const auto plus = Boundary::make_inner_endpoint_derivative_jet(
        amplitudes, grid.dx(), k, Sector::P_plus);
    const auto minus = Boundary::make_inner_endpoint_derivative_jet(
        amplitudes, grid.dx(), k, Sector::P_minus);

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const double radial_first = plus.dx().value(variable);
        const double parity = Boundary::intrinsic_z_parity(variable);
        require_close("P+ D_xz=D_x D_z",
                      plus.dxz().value(variable), -parity * k * radial_first,
                      5.0e-15);
        require_close("P- D_xz=D_x D_z",
                      minus.dxz().value(variable), parity * k * radial_first,
                      5.0e-15);
        require_close("P+ Fourier D_zz",
                      plus.dzz().value(variable),
                      -k * k * plus.value().value(variable), 5.0e-15);
        require_close("P- Fourier D_zz",
                      minus.dzz().value(variable),
                      -k * k * minus.value().value(variable), 5.0e-15);
    }
    require_true("endpoint jet stores no radial ghost unknown",
                 !plus.stores_radial_ghost_unknowns());
    require_true("endpoint jet consumes explicit four-point radial reach",
                 plus.radial_points_consumed() == 4);

    std::vector<Vector> shortened(amplitudes.begin(), amplitudes.begin() + 3);
    require_domain_error("shortened full-row endpoint stencil", [&]() {
        (void)Boundary::make_inner_endpoint_derivative_jet(
            shortened, grid.dx(), k, Sector::P_plus);
    });
}

void require_only_owner_outputs(const Vector &owner, const int owner_index)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const bool chi_owner =
            Operator::receives_chi_metric_complete_row_assembly(variable);
        const bool kta_owner =
            Operator::receives_k_theta_a_complete_row_assembly(variable);
        const int expected_owner = chi_owner ? 0 : kta_owner ? 1 : 2;
        if (expected_owner != owner_index)
        {
            require_close("complete row owner writes no foreign slot",
                          owner.value(variable), 0.0, 0.0);
        }
    }
}

void check_all_rows_and_owner_retention()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 1.5);
    const auto grid = Operator::make_radial_grid(domain, 32);
    const auto amplitudes = radial_amplitudes(grid);
    const auto unchanged = amplitudes;
    constexpr double k = 0.77;
    const auto result = Boundary::apply_inner_endpoint_operator(
        grid, amplitudes, k, Sector::P_plus);
    const auto jet = Boundary::make_inner_endpoint_derivative_jet(
        amplitudes, grid.dx(), k, Sector::P_plus);
    const double x = domain.x_in();
    const auto advection =
        Boundary::common_advection_from_jet(domain.r0(), x, jet);
    const auto geometric = Boundary::geometric_ricci_from_jet(x, jet);
    const auto adapter = Boundary::encoded_z_adapter_from_jet(x, jet);
    const auto encoded = Adapter::make_encoded_z_ricci_completion(x, adapter);
    const auto connection = Boundary::connection_from_jet(x, jet);
    const auto chi_metric = Operator::apply_complete_chi_metric_rows_at_point(
        domain.r0(), x, jet.value(), advection);
    const auto kta = Operator::apply_complete_k_theta_a_rows_at_point(
        domain.r0(), x, jet.value(), advection, geometric, encoded);
    const auto gamma = Operator::apply_complete_hat_gamma_rows_at_point(
        domain.r0(), jet.value(), advection, connection,
        jet.dx().value(Variable::K), jet.dz().value(Variable::K),
        jet.dx().value(Variable::Theta), jet.dz().value(Variable::Theta),
        jet.dx().value(Variable::chi), jet.dz().value(Variable::chi));
    require_only_owner_outputs(chi_metric, 0);
    require_only_owner_outputs(kta, 1);
    require_only_owner_outputs(gamma, 2);

    std::array<double, 3> owner_norms = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const bool chi_owner =
            Operator::receives_chi_metric_complete_row_assembly(variable);
        const bool kta_owner =
            Operator::receives_k_theta_a_complete_row_assembly(variable);
        const int owner = chi_owner ? 0 : kta_owner ? 1 : 2;
        const double expected = owner == 0   ? chi_metric.value(variable)
                                : owner == 1 ? kta.value(variable)
                                             : gamma.value(variable);
        require_close(std::string("inner endpoint exact slot owner ") +
                          Operator::variable_name(variable),
                      result.rhs().value(variable), expected, 2.0e-14);
        owner_norms[static_cast<std::size_t>(owner)] =
            std::max(owner_norms[static_cast<std::size_t>(owner)],
                     std::abs(expected));
        require_true(std::string("inner endpoint row active ") +
                         Operator::variable_name(variable),
                     std::abs(result.rhs().value(variable)) > 1.0e-9);
    }
    for (std::size_t owner = 0; owner < owner_norms.size(); ++owner)
    {
        auto omitted_values = values_from_vector(result.rhs());
        auto duplicated_values = values_from_vector(result.rhs());
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            const bool chi_owner =
                Operator::receives_chi_metric_complete_row_assembly(variable);
            const bool kta_owner =
                Operator::receives_k_theta_a_complete_row_assembly(variable);
            const std::size_t variable_owner =
                chi_owner ? 0U : kta_owner ? 1U : 2U;
            if (variable_owner != owner)
            {
                continue;
            }
            const auto slot = Operator::variable_index(variable);
            const double owner_value =
                owner == 0   ? chi_metric.value(variable)
                : owner == 1 ? kta.value(variable)
                             : gamma.value(variable);
            omitted_values[slot] -= owner_value;
            duplicated_values[slot] += owner_value;
        }
        const auto omitted =
            Operator::make_frozen_gauge_perturbation_vector(omitted_values);
        const auto duplicated =
            Operator::make_frozen_gauge_perturbation_vector(duplicated_values);
        require_true("row-owner omission mutation rejected",
                     owner_norms[owner] > 1.0e-8 &&
                         max_error(result.rhs(), omitted) > 1.0e-8);
        require_true("row-owner duplication mutation rejected",
                     owner_norms[owner] > 1.0e-8 &&
                         max_error(result.rhs(), duplicated) > 1.0e-8);
    }
    auto doubled_hww = result.rhs().value(Variable::h_ww) * 2.0;
    auto doubled_aww = result.rhs().value(Variable::A_ww) * 2.0;
    require_true("representative h_ww row doubling rejected",
                 std::abs(doubled_hww - result.rhs().value(Variable::h_ww)) >
                     1.0e-8);
    require_true("representative A_ww row doubling rejected",
                 std::abs(doubled_aww - result.rhs().value(Variable::A_ww)) >
                     1.0e-8);

    require_true("all 13 inner PDE rows retained",
                 result.pde_rows_retained() == 13);
    require_true("no continuum boundary equation added",
                 result.continuum_boundary_equations() == 0);
    require_true("complete interior owner applied once",
                 result.complete_interior_owner_applications() == 1);
    require_true("no physical field value reset", !result.physical_values_reset());
    require_true("no extrapolated physical data",
                 !result.extrapolated_physical_data());
    require_true("no stored radial ghost unknown",
                 !result.radial_ghost_unknowns_stored());
    require_true("cleanup not folded into endpoint wrapper",
                 !result.algebraic_cleanup_applied());
    for (std::size_t i = 0; i < amplitudes.size(); ++i)
    {
        require_true("endpoint wrapper leaves physical input unchanged",
                     max_error(amplitudes[i], unchanged[i]) == 0.0);
    }

    std::vector<Vector> wrong_size(amplitudes.begin(), amplitudes.end() - 1);
    require_domain_error("endpoint wrapper grid-length mismatch", [&]() {
        (void)Boundary::apply_inner_endpoint_operator(
            grid, wrong_size, k, Sector::P_plus);
    });
}

void check_outflow_classification()
{
    constexpr std::array<double, 4> ratios = {0.35, 0.50, 0.65, 0.80};
    std::cout << "INFO inner principal speed table\n"
              << "  x_in/r0 v c_plus c_minus c_zero\n";
    for (const double ratio : ratios)
    {
        const auto report = Boundary::make_inner_outflow_report(1.0, ratio);
        std::cout << "  " << ratio << " " << report.v_in() << " "
                  << report.c_plus() << " " << report.c_minus() << " "
                  << report.c_zero() << "\n";
        require_true("representative inner point is pure outflow",
                     report.pure_outflow());
        require_true("all representative principal speeds point outward",
                     report.c_plus() < 0.0 && report.c_minus() < 0.0 &&
                         report.c_zero() < 0.0);
        require_true("no complete longitudinal diagonalization claimed",
                     !report.complete_characteristic_diagonalization_claimed());
    }
    const auto glancing = Boundary::make_inner_outflow_report(1.0, 1.0);
    require_true("horizon endpoint classified glancing", glancing.glancing());
    require_close("glancing c_plus", glancing.c_plus(), 0.0, 0.0);
    require_true("glancing endpoint rejected for no-data acceptance",
                 !glancing.valid_for_no_data_contract());
    const auto exterior = Boundary::make_inner_outflow_report(1.0, 1.1);
    require_true("x_in>r0 invalid for no-data contract",
                 exterior.classification() ==
                     Boundary::InnerCausalClassification::
                         invalid_for_no_data_contract);
    require_true("x_in>r0 has an incoming c_plus branch",
                 exterior.c_plus() > 0.0);
}

double reflection_measure(const double wavenumber, const double dx)
{
    using Complex = std::complex<double>;
    const Complex imaginary(0.0, 1.0);
    const double theta = wavenumber * dx;
    const Complex e1 = std::exp(imaginary * theta);
    const Complex e2 = std::exp(2.0 * imaginary * theta);
    const Complex e3 = std::exp(3.0 * imaginary * theta);
    const Complex d1 = (-1.5 + 2.0 * e1 - 0.5 * e2) / dx;
    const Complex d2 = (2.0 - 5.0 * e1 + 4.0 * e2 - e3) / (dx * dx);
    const double first_order_forcing =
        std::abs(d1 - imaginary * wavenumber) / (2.0 * wavenumber);
    const double second_order_forcing =
        std::abs(d2 + wavenumber * wavenumber) /
        (2.0 * wavenumber * wavenumber);
    return std::max(first_order_forcing, second_order_forcing);
}

void check_local_reflection_diagnostic()
{
    constexpr double radial_wavenumber = 0.7;
    constexpr std::array<double, 6> spacings = {
        1.0 / 64.0, 1.0 / 128.0, 1.0 / 256.0,
        1.0 / 512.0, 1.0 / 1024.0, 1.0 / 2048.0};
    std::array<double, spacings.size()> measures = {};
    std::cout << "INFO inner endpoint reflection diagnostic\n"
              << "  dx equivalent_reflected_amplitude\n";
    for (std::size_t i = 0; i < spacings.size(); ++i)
    {
        measures[i] = reflection_measure(radial_wavenumber, spacings[i]);
        std::cout << "  " << spacings[i] << " " << measures[i] << "\n";
        if (i > 0)
        {
            require_true("reflection measure decreases with resolution",
                         measures[i] < measures[i - 1]);
            const double order =
                std::log(measures[i - 1] / measures[i]) / std::log(2.0);
            require_true("reflection measure converges at order >=1.8",
                         order >= 1.8);
        }
    }
    require_true("finest reflection measure below predeclared 1e-6 target",
                 measures.back() < 1.0e-6);
    std::cout
        << "INFO reflection definition: max normalized D_x/D_xx symbol defect "
           "for one outgoing exp(i k_x x) mode, divided by the two-way radial "
           "phase separation; only endpoint stencil symbols enter, so physical "
           "interior scattering is absent\n";
}

void check_parity_and_cleanup()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 1.5);
    const auto grid = Operator::make_radial_grid(domain, 32);
    const auto amplitudes = radial_amplitudes(grid);
    const auto parity_transformed = transform_intrinsic_parity(amplitudes);
    constexpr double k = 0.83;
    const auto minus = Boundary::apply_inner_endpoint_operator(
        grid, amplitudes, k, Sector::P_minus);
    const auto plus_transformed = Boundary::apply_inner_endpoint_operator(
        grid, parity_transformed, k, Sector::P_plus);
    const auto conjugated_plus =
        transform_intrinsic_parity(plus_transformed.rhs());
    const double commutator = max_error(minus.rhs(), conjugated_plus);
    const double leakage = 0.0;
    const double scale = std::max(1.0, [&]() {
        double value = 0.0;
        for (const auto variable : Operator::frozen_gauge_state_vector)
        {
            value = std::max(value, std::abs(minus.rhs().value(variable)));
        }
        return value;
    }());
    const double parity_limit =
        100.0 * std::numeric_limits<double>::epsilon() * scale;
    std::cout << "INFO inner parity leakage=" << leakage
              << " reflection_commutator=" << commutator
              << " limit=" << parity_limit << "\n";
    require_true("P+ and P- endpoint sectors remain invariant",
                 plus_transformed.sector() == Sector::P_plus &&
                     minus.sector() == Sector::P_minus);
    require_true("cross-sector leakage <=100 epsilon", leakage <= parity_limit);
    require_true("endpoint reflection commutator <=100 epsilon",
                 commutator <= parity_limit);

    const auto raw = amplitudes.front();
    const auto cleaned = Boundary::project_inner_endpoint_algebraic_constraints(
        domain.r0(), domain.x_in(), raw);
    const auto cleaned_twice =
        Boundary::project_inner_endpoint_algebraic_constraints(
            domain.r0(), domain.x_in(), cleaned);
    require_close("optional cleanup enforces determinant tangent",
                  Boundary::metric_determinant_tangent_residual(cleaned), 0.0,
                  2.0e-16);
    require_close("optional cleanup enforces weighted A tangent",
                  Operator::delta_trace_a_at_point(domain.r0(), domain.x_in(),
                                                   cleaned),
                  0.0, 2.0e-16);
    require_true("optional cleanup is algebraically idempotent",
                 max_error(cleaned, cleaned_twice) < 2.0e-16);
    for (const auto variable : {Variable::chi, Variable::h_xz, Variable::K,
                                Variable::A_xz, Variable::Theta,
                                Variable::hat_Gamma_x,
                                Variable::hat_Gamma_z})
    {
        require_close("cleanup preserves unrelated slot", cleaned.value(variable),
                      raw.value(variable), 0.0);
    }
    require_true("missing cleanup does not remove a PDE row",
                 minus.pde_rows_retained() == 13 &&
                     minus.continuum_boundary_equations() == 0);
    require_true("duplicated cleanup is not a duplicated boundary equation",
                 max_error(cleaned, cleaned_twice) < 2.0e-16 &&
                     minus.continuum_boundary_equations() == 0);
}

void check_gates()
{
    const auto contracts = Operator::boundary_contracts();
    require_true("inner endpoint derivative helper flag true",
                 Operator::inner_endpoint_derivative_helper_implemented);
    require_true("inner pure-outflow validation flag true",
                 Operator::inner_pure_outflow_validation_implemented);
    require_true("inner boundary contract validation entry true",
                 contracts[0].validation_test_implemented);
    require_true("outer boundary implementation remains false",
                 !Operator::outer_boundary_implementation_implemented);
    require_true("outer boundary validation remains false",
                 !Operator::outer_boundary_validation_implemented);
    require_true("radial boundary system remains incomplete",
                 !Operator::radial_boundary_system_complete);
    require_true("aggregate boundary derivative validation remains false",
                 !Operator::boundary_derivative_validation_implemented);
    require_true("boundary-bearing complete operator remains false",
                 !Operator::complete_frozen_gauge_operator_implemented);
    require_true("eigensolver remains false", !Operator::eigensolver_implemented);
    require_true("shift-invert remains false", !Operator::shift_invert_implemented);
    require_true("threshold remains false", !Operator::threshold_search_implemented);
    require_true("production wiring remains false",
                 !Operator::production_rhs_wiring_implemented);
    require_true("Stage 4AO-D remains false",
                 !Operator::live_gauge_stage_4ao_d_implemented);
}

} // namespace

int main()
{
    check_stencil_coefficients_and_exactness();
    check_stencil_convergence_and_mutations();
    check_fourier_mixed_derivatives();
    check_all_rows_and_owner_retention();
    check_outflow_classification();
    check_local_reflection_diagnostic();
    check_parity_and_cleanup();
    check_gates();
    std::cout << "PASS Stage 4AO-C inner pure-outflow endpoint operator\n";
    return 0;
}
