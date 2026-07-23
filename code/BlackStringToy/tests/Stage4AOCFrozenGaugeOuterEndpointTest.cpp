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

std::size_t combined_rank(const Boundary::Matrix9x13 &state,
                          const Boundary::Matrix9x13 &radial)
{
    std::array<std::array<double, 26>, 9> work = {};
    for (std::size_t row = 0; row < 9; ++row)
    {
        for (std::size_t column = 0; column < 13; ++column)
        {
            work[row][column] = state[row][column];
            work[row][13 + column] = radial[row][column];
        }
    }
    std::size_t rank = 0;
    for (std::size_t column = 0; column < 26 && rank < 9; ++column)
    {
        std::size_t pivot = rank;
        for (std::size_t row = rank + 1; row < 9; ++row)
        {
            if (std::abs(work[row][column]) >
                std::abs(work[pivot][column]))
            {
                pivot = row;
            }
        }
        if (std::abs(work[pivot][column]) < 1.0e-11)
        {
            continue;
        }
        const auto swap = work[rank];
        work[rank] = work[pivot];
        work[pivot] = swap;
        const double divisor = work[rank][column];
        for (std::size_t entry = column; entry < 26; ++entry)
        {
            work[rank][entry] /= divisor;
        }
        for (std::size_t row = 0; row < 9; ++row)
        {
            if (row == rank)
            {
                continue;
            }
            const double factor = work[row][column];
            for (std::size_t entry = column; entry < 26; ++entry)
            {
                work[row][entry] -= factor * work[rank][entry];
            }
        }
        ++rank;
    }
    return rank;
}

void check_outer_stencils()
{
    constexpr double x = 2.3;
    constexpr double h = 0.07;
    for (int degree = 0; degree <= 2; ++degree)
    {
        std::vector<double> samples;
        for (int offset = 0; offset < 4; ++offset)
        {
            samples.push_back(std::pow(x - offset * h, degree));
        }
        const double expected =
            degree == 0 ? 0.0 : degree * std::pow(x, degree - 1);
        require_close("outer Dx polynomial exactness",
                      Boundary::outer_dx(samples, h), expected, 2.0e-12);
    }
    for (int degree = 0; degree <= 3; ++degree)
    {
        std::vector<double> samples;
        for (int offset = 0; offset < 4; ++offset)
        {
            samples.push_back(std::pow(x - offset * h, degree));
        }
        const double expected = degree < 2
                                    ? 0.0
                                    : degree * (degree - 1) *
                                          std::pow(x, degree - 2);
        require_close("outer Dxx polynomial exactness",
                      Boundary::outer_dxx(samples, h), expected, 3.0e-10);
    }
    double previous_dx = 0.0;
    double previous_dxx = 0.0;
    std::cout << "INFO outer stencil convergence\n";
    for (const double spacing : {0.16, 0.08, 0.04, 0.02, 0.01})
    {
        std::vector<double> samples;
        for (int offset = 0; offset < 4; ++offset)
        {
            samples.push_back(std::exp(0.37 * (x - offset * spacing)));
        }
        const double exact = std::exp(0.37 * x);
        const double dx_error =
            std::abs(Boundary::outer_dx(samples, spacing) - 0.37 * exact);
        const double dxx_error = std::abs(
            Boundary::outer_dxx(samples, spacing) - 0.37 * 0.37 * exact);
        std::cout << std::scientific << std::setprecision(12) << "  "
                  << spacing << ' ' << dx_error << ' ' << dxx_error << '\n';
        if (previous_dx > 0.0)
        {
            require_true("outer Dx order >=1.8",
                         previous_dx / dx_error >= std::pow(2.0, 1.8));
            require_true("outer Dxx order >=1.8",
                         previous_dxx / dxx_error >= std::pow(2.0, 1.8));
        }
        previous_dx = dx_error;
        previous_dxx = dxx_error;
    }
    require_domain_error("outer stencil rejects short reach", []() {
        (void)Boundary::outer_dxx({1.0, 2.0, 3.0}, 0.1);
    });
    require_domain_error("outer stencil rejects invalid spacing", []() {
        (void)Boundary::outer_dx({1.0, 2.0, 3.0}, 0.0);
    });
    require_domain_error("outer stencil rejects nonfinite data", []() {
        (void)Boundary::outer_dx(
            {1.0, std::numeric_limits<double>::infinity(), 3.0}, 0.1);
    });
    const std::vector<double> active = {1.7, -0.4, 0.9, -1.2};
    const double correct = Boundary::outer_dx(active, 0.2);
    const double wrong_sign = (-1.5 * active[0] + 2.0 * active[1] -
                               0.5 * active[2]) /
                              0.2;
    const double wrong_coefficient =
        (active[0] - 2.0 * active[1] + active[2]) / 0.2;
    require_true("outer stencil sign mutation rejected",
                 std::abs(correct - wrong_sign) > 1.0e-3);
    require_true("outer stencil coefficient mutation rejected",
                 std::abs(correct - wrong_coefficient) > 1.0e-3);
}

void check_diagnostic_selector_rank_and_rows()
{
    constexpr double r0 = 1.0;
    constexpr double x = 12.5;
    constexpr double k = 0.8;
    for (const auto sector : {Sector::P_plus, Sector::P_minus})
    {
        const auto b0 = Boundary::diagnostic_characteristic_boundary_matrix(
            r0, x, k, sector, false);
        const auto b1 = Boundary::diagnostic_characteristic_boundary_matrix(
            r0, x, k, sector, true);
        require_true("diagnostic characteristic B0/B1 rank is nine",
                     combined_rank(b0, b1) == 9);
        const auto basis =
            Outer::make_diagnostic_characteristic_basis(r0, x, k);
        const auto projector =
            Outer::make_diagnostic_characteristic_projector(basis);
        require_true("diagnostic retained rank four",
                     projector.diagnostic_rank() == 4);
        require_true("diagnostic excluded rank nine",
                     projector.complement_rank() == 9);
        Outer::AmplitudeVector mixed = {};
        for (std::size_t column = 0; column < basis.size(); ++column)
        {
            const auto residual =
                projector.projected_diagnostic_complement_residual(
                    basis[column]);
            require_close("diagnostic one-hot column annihilated",
                          Outer::norm(residual), 0.0, 3.0e-15);
            for (std::size_t row = 0; row < mixed.size(); ++row)
            {
                mixed[row] += (0.3 + 0.2 * column) * basis[column][row];
            }
        }
        require_close("mixed diagnostic columns annihilated",
                      Outer::norm(
                          projector.projected_diagnostic_complement_residual(
                              mixed)),
                      0.0, 4.0e-15);
        for (const auto amplitude : Outer::excluded_amplitudes)
        {
            Outer::AmplitudeVector excluded = {};
            excluded[Outer::amplitude_index(amplitude)] = 1.0;
            require_true("diagnostic excluded selector is nonzero",
                         Outer::norm(
                             projector
                                 .projected_diagnostic_complement_residual(
                                     excluded)) > 0.9);
        }
    }
    std::vector<Outer::Amplitude> locked(Outer::excluded_amplitudes.begin(),
                                         Outer::excluded_amplitudes.end());
    Outer::require_locked_outer_residual_layout(locked);
    auto omitted_f = locked;
    omitted_f[5] = Outer::Amplitude::W_T_out;
    require_domain_error("diagnostic layout rejects omitted F selector", [&]() {
        Outer::require_locked_outer_residual_layout(omitted_f);
    });
    auto omitted_g = locked;
    omitted_g[6] = Outer::Amplitude::W_TF_out;
    require_domain_error("diagnostic layout rejects omitted G selector", [&]() {
        Outer::require_locked_outer_residual_layout(omitted_g);
    });
    require_domain_error("diagnostic layout rejects a tenth selector", []() {
        Outer::require_locked_outer_residual_row_count(10);
    });
    require_domain_error("thirteen component rows rejected", []() {
        Outer::require_locked_outer_residual_row_count(13);
    });
}

void check_scalar_profile_and_stencil_refinement()
{
    constexpr double r0 = 1.0;
    constexpr double k = 0.8;
    std::cout << "INFO outer WKB location residuals\n";
    double previous_location = std::numeric_limits<double>::infinity();
    for (const double kx : {8.0, 10.0, 12.0})
    {
        const double x = kx / k;
        double residual = 0.0;
        for (const auto block : {Outer::LightBlock::transverse_trace,
                                 Outer::LightBlock::transverse_trace_free,
                                 Outer::LightBlock::vector_z4,
                                 Outer::LightBlock::scalar_z4})
        {
            residual = std::fmax(
                residual,
                std::abs(Outer::make_diagnostic_scalar_wkb_profile(
                             block, r0, x, k, true)
                             .continuum_residual_ratio()));
        }
        std::cout << "  " << kx << ' ' << std::scientific
                  << std::setprecision(12) << residual << '\n';
        require_true("outer residual decreases with x_out",
                     residual < previous_location);
        previous_location = residual;
    }

    constexpr double x = 12.5;
    const auto mode = Outer::make_diagnostic_scalar_wkb_profile(
        Outer::LightBlock::scalar_z4, r0, x, k, true);
    double previous = 0.0;
    double finest = 0.0;
    std::cout << "INFO outer endpoint WKB derivative convergence\n";
    for (const double h : {0.08, 0.04, 0.02, 0.01, 0.005})
    {
        std::vector<double> samples;
        for (int offset = 0; offset < 3; ++offset)
        {
            samples.push_back(Outer::make_diagnostic_scalar_wkb_profile(
                                  Outer::LightBlock::scalar_z4, r0,
                                  x - offset * h, k, true)
                                  .profile());
        }
        const double residual =
            std::abs(Boundary::outer_dx(samples, h) -
                     mode.radial_log_derivative() * mode.profile());
        std::cout << "  " << h << ' ' << residual << '\n';
        if (previous > 0.0)
        {
            require_true("outer WKB endpoint order >=1.8",
                         previous / residual >= std::pow(2.0, 1.8));
        }
        previous = residual;
        finest = residual;
    }
    require_true("finest normalized outer constraint residual below 1e-8",
                 finest < 1.0e-8);

}

void check_input_guards_and_flags()
{
    Boundary::Rows zero_rows = {};
    const auto zero =
        Operator::make_frozen_gauge_perturbation_vector(zero_rows);
    require_domain_error("outer residual rejects k=0", [&]() {
        (void)Boundary::diagnostic_characteristic_boundary_residual(
            1.0, 10.0, 0.0, Sector::P_plus, zero, zero);
    });
    require_domain_error("outer residual rejects invalid x_out", [&]() {
        (void)Boundary::diagnostic_characteristic_boundary_residual(
            1.0, 1.0, 0.8, Sector::P_plus, zero, zero);
    });
    require_true("mirrored outer derivative scaffolding flag true",
                 Boundary::mirrored_outer_derivative_scaffolding_implemented);
    require_true("diagnostic row-layout flag true",
                 Boundary::diagnostic_radial_row_layout_implemented);
    require_true("outer endpoint implementation flag false",
                 !Operator::outer_boundary_implementation_implemented);
    require_true("outer endpoint validation flag false",
                 !Operator::outer_boundary_validation_implemented);
    require_true("quadratic pencil representation flag false",
                 !Operator::
                     quadratic_pencil_coefficient_representation_implemented);
    require_true("eigensolver remains false", !Operator::eigensolver_implemented);
    require_true("shift-invert remains false",
                 !Operator::shift_invert_implemented);
    require_true("k=0 remains unsupported",
                 !Operator::k_zero_outer_boundary_implemented);
    require_true("nonzero-growth asymptotics remain unsupported",
                 !Operator::nonzero_growth_outer_asymptotics_implemented);
}
} // namespace

int main()
{
    try
    {
        check_outer_stencils();
        check_diagnostic_selector_rank_and_rows();
        check_scalar_profile_and_stencil_refinement();
        check_input_guards_and_flags();
    }
    catch (const std::exception &error)
    {
        std::cerr << "FAIL " << error.what() << '\n';
        return 1;
    }
    std::cout << "PASS Stage 4AO-C outer stencil and diagnostic row scaffolding\n";
    return 0;
}
