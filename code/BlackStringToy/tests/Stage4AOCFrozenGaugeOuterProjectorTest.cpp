#include "Stage4AOFrozenGaugeOuterProjector.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace Outer = BlackStringToy::Stage4AOFrozenGaugeOuterProjector;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Amplitude = Outer::Amplitude;
using Block = Outer::LightBlock;
using Sector = Outer::FourierParitySector;
using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

namespace
{
void fail(const std::string &label, const std::string &details)
{
    throw std::runtime_error(label + ": " + details);
}

void require_true(const std::string &label, const bool condition)
{
    if (!condition)
    {
        fail(label, "condition is false");
    }
    std::cout << "PASS " << label << '\n';
}

void require_close(const std::string &label, const double actual,
                   const double expected, const double tolerance)
{
    if (!std::isfinite(actual) || !std::isfinite(expected) ||
        std::abs(actual - expected) > tolerance)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected));
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

Vector make_vector(const std::array<double, 13> &values)
{
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

double sector_dz_factor(const Variable variable, const double k,
                        const Sector sector)
{
    const bool one_z = variable == Variable::h_xz ||
                       variable == Variable::A_xz ||
                       variable == Variable::hat_Gamma_z;
    const double intrinsic = one_z ? -1.0 : 1.0;
    const double sector_sign = sector == Sector::P_plus ? 1.0 : -1.0;
    return -sector_sign * intrinsic * k;
}

std::array<double, 13> independent_amplitude_oracle(
    const double r0, const double x, const double k, const Sector sector,
    const Vector &state, const Vector &dx)
{
    const auto value = [&](const Variable variable) {
        return state.value(variable);
    };
    const auto radial = [&](const Variable variable) {
        return dx.value(variable);
    };
    const auto dz = [&](const Variable variable) {
        return sector_dz_factor(variable, k, sector) * value(variable);
    };
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double k_gp = 1.5 * lambda;
    const double a_gp_xx = -7.0 * lambda / 8.0;
    const double a_gp_zz = -3.0 * lambda / 8.0;
    const double a_gp_ww = 5.0 * lambda / 8.0;
    const double g_x =
        0.5 * radial(Variable::h_xx) -
        0.5 * radial(Variable::h_zz) - radial(Variable::h_ww) +
        dz(Variable::h_xz) +
        2.0 * (value(Variable::h_xx) - value(Variable::h_ww)) / x;
    const double g_z =
        radial(Variable::h_xz) + 2.0 * value(Variable::h_xz) / x -
        0.5 * dz(Variable::h_xx) + 0.5 * dz(Variable::h_zz) -
        dz(Variable::h_ww);
    const double z_x = 0.5 * (value(Variable::hat_Gamma_x) - g_x);
    const double z_z = 0.5 * (value(Variable::hat_Gamma_z) - g_z);
    const double d_xx = radial(Variable::h_xx) - radial(Variable::chi);
    const double d_xz = radial(Variable::h_xz);
    const double d_zz = radial(Variable::h_zz) - radial(Variable::chi);
    const double d_ww = radial(Variable::h_ww) - radial(Variable::chi);
    const double d_t = d_zz + 2.0 * d_ww;
    const double p_xx = value(Variable::A_xx) + 0.25 * value(Variable::K) +
                        0.25 * k_gp * value(Variable::h_xx) -
                        value(Variable::chi) * (a_gp_xx + 0.25 * k_gp);
    const double p_xz = value(Variable::A_xz) +
                        0.25 * k_gp * value(Variable::h_xz);
    const double p_zz = value(Variable::A_zz) + 0.25 * value(Variable::K) +
                        0.25 * k_gp * value(Variable::h_zz) -
                        value(Variable::chi) * (a_gp_zz + 0.25 * k_gp);
    const double p_ww = value(Variable::A_ww) + 0.25 * value(Variable::K) +
                        0.25 * k_gp * value(Variable::h_ww) -
                        value(Variable::chi) * (a_gp_ww + 0.25 * k_gp);
    const double p_t = p_zz + 2.0 * p_ww;
    const double theta_minus_p_t = value(Variable::Theta) - p_t;
    const double c_a = value(Variable::A_xx) + value(Variable::A_zz) +
                       2.0 * value(Variable::A_ww) -
                       a_gp_xx * value(Variable::h_xx) -
                       a_gp_zz * value(Variable::h_zz) -
                       2.0 * a_gp_ww * value(Variable::h_ww);
    return {p_t + 0.5 * d_t,
            p_t - 0.5 * d_t,
            (p_zz - p_ww) + 0.5 * (d_zz - d_ww),
            (p_zz - p_ww) - 0.5 * (d_zz - d_ww),
            p_xz - z_z,
            p_xz + z_z,
            theta_minus_p_t - z_x,
            theta_minus_p_t + z_x,
            d_xz + 2.0 * z_z,
            p_xx + p_t - 2.0 * value(Variable::Theta),
            d_xx - d_t + 4.0 * z_x,
            value(Variable::h_xx) + value(Variable::h_zz) +
                2.0 * value(Variable::h_ww),
            c_a};
}

void check_boundary_state_transformation()
{
    constexpr double r0 = 1.3;
    constexpr double x = 11.0;
    constexpr double k = 0.8;
    const Vector state = make_vector(
        {0.07, -0.11, 0.13, 0.17, -0.19, 0.23, -0.29,
         0.31, 0.37, -0.41, 0.43, -0.47, 0.53});
    const Vector dx = make_vector(
        {-0.59, 0.61, -0.67, 0.71, -0.73, 0.79, -0.83,
         0.89, -0.97, 1.01, -1.03, 1.07, -1.09});

    for (const auto sector : {Sector::P_plus, Sector::P_minus})
    {
        const auto actual =
            Outer::transform_boundary_state(r0, x, k, sector, state, dx);
        const auto expected =
            independent_amplitude_oracle(r0, x, k, sector, state, dx);
        for (const auto amplitude : Outer::amplitude_order)
        {
            const auto index = Outer::amplitude_index(amplitude);
            require_close("transformed amplitude independent oracle",
                          actual.value(amplitude), expected[index], 3.0e-14);
        }
        require_true("transformed amplitude sector retained",
                     actual.sector() == sector);
        require_close("transformed amplitude k retained", actual.wavenumber(),
                      k, 0.0);
    }

    const auto amplitudes = Outer::transform_boundary_state(
        r0, x, k, Sector::P_plus, state, dx);
    const auto projector =
        Outer::make_locked_diagnostic_characteristic_projector(r0, x, k);
    const auto normalized_residuals =
        projector.selector_residuals(amplitudes);
    for (std::size_t i = 0; i < Outer::excluded_amplitudes.size(); ++i)
    {
        const auto amplitude = Outer::excluded_amplitudes[i];
        const bool divided_by_k =
            amplitude == Amplitude::J || amplitude == Amplitude::F ||
            amplitude == Amplitude::G || amplitude == Amplitude::C_A;
        require_close("dimensionful excluded-amplitude normalization",
                      normalized_residuals[i],
                      amplitudes.value(amplitude) /
                          (divided_by_k ? k : 1.0),
                      3.0e-14);
    }
    const double wrong_hidden_c_h = state.value(Variable::h_xx) +
                                    state.value(Variable::h_zz) +
                                    state.value(Variable::h_ww);
    const double doubled_representative_c_h =
        state.value(Variable::h_xx) + state.value(Variable::h_zz) +
        4.0 * state.value(Variable::h_ww);
    require_true("wrong hidden multiplicity mutation rejected",
                 std::abs(amplitudes.value(Amplitude::C_h) -
                          wrong_hidden_c_h) > 1.0e-3);
    require_true("doubled representative ww mutation rejected",
                 std::abs(amplitudes.value(Amplitude::C_h) -
                          doubled_representative_c_h) > 1.0e-3);

    require_domain_error("k=0 rejected", [&]() {
        Outer::transform_boundary_state(r0, x, 0.0, Sector::P_plus, state, dx);
    });
    require_domain_error("nonfinite k rejected", [&]() {
        Outer::transform_boundary_state(
            r0, x, std::numeric_limits<double>::infinity(), Sector::P_plus,
            state, dx);
    });
    require_domain_error("x_out<=r0 rejected", [&]() {
        Outer::transform_boundary_state(r0, r0, k, Sector::P_plus, state, dx);
    });
    require_domain_error("invalid r0 rejected", [&]() {
        Outer::transform_boundary_state(-r0, x, k, Sector::P_plus, state, dx);
    });
    std::array<double, 13> nonfinite_values = {};
    nonfinite_values[Operator::variable_index(Variable::chi)] =
        std::numeric_limits<double>::quiet_NaN();
    require_domain_error("nonfinite boundary state rejected", [&]() {
        (void)make_vector(nonfinite_values);
    });
}

struct OracleMode
{
    double power;
    double u1;
    double u2;
    double u3;
};

OracleMode independent_mode_coefficients(const double gamma, const double r0,
                                          const double k,
                                          const bool decaying)
{
    const double leading = decaying ? -k : k;
    const double a_log = -0.5 * gamma * std::sqrt(r0);
    const double power = decaying
                             ? 1.0 + 0.5 * k * r0 +
                                   gamma * gamma * r0 / (8.0 * k)
                             : 1.0 - 0.5 * k * r0 -
                                   gamma * gamma * r0 / (8.0 * k);
    const double b_log = -power;
    const double c_log = a_log * (r0 - 3.0 / (4.0 * leading));
    const double u1 = -2.0 * c_log;
    const double q = b_log * b_log + b_log - r0 * a_log * a_log -
                     2.0 * r0 * leading * b_log - r0 * leading;
    const double d_log = -q / (2.0 * leading);
    const double u2 = 0.5 * u1 * u1 - d_log;
    const double e_log =
        -(2.0 * b_log * c_log + 0.5 * c_log -
          2.0 * r0 * leading * c_log - 2.0 * r0 * a_log * b_log -
          0.5 * r0 * a_log) /
        (2.0 * leading);
    const double u3 = u1 * u2 - u1 * u1 * u1 / 3.0 - 2.0 * e_log / 3.0;
    return {power, u1, u2, u3};
}

double mutated_profile(const double gamma, const double power,
                       const double u1, const double u2, const double u3,
                       const double r0, const double x, const double k)
{
    return std::exp(-k * x - gamma * std::sqrt(r0 * x)) *
           std::pow(x, -power) *
           (1.0 + u1 / std::sqrt(x) + u2 / x +
            u3 / (x * std::sqrt(x)));
}

void check_scalar_profile_and_diagnostic_columns()
{
    constexpr double r0 = 1.2;
    constexpr double x = 12.0;
    constexpr double k = 0.75;
    const std::array<Block, 4> blocks = {
        Block::transverse_trace, Block::transverse_trace_free,
        Block::vector_z4, Block::scalar_z4};
    for (const auto block : blocks)
    {
        const auto mode =
            Outer::make_diagnostic_scalar_wkb_profile(block, r0, x, k, true);
        const double gamma = Outer::damping_transport_rate(block);
        const auto expected =
            independent_mode_coefficients(gamma, r0, k, true);
        require_close("scalar diagnostic gamma", mode.gamma(), gamma, 0.0);
        require_close("scalar diagnostic corrected power", mode.power(),
                      expected.power, 2.0e-15);
        require_close("scalar diagnostic x^-1/2 coefficient", mode.u1(),
                      expected.u1, 3.0e-15);
        require_close("scalar diagnostic x^-1 coefficient", mode.u2(),
                      expected.u2, 5.0e-15);
        require_close("scalar diagnostic x^-3/2 coefficient", mode.u3(),
                      expected.u3, 8.0e-15);
        require_true("scalar diagnostic retains through x^-3/2",
                     mode.retained_half_orders() == 3);
        require_true("decaying scalar diagnostic profile is positive",
                     mode.profile() > 0.0);

        const double p_one = mutated_profile(
            gamma, 1.0, mode.u1(), mode.u2(), mode.u3(), r0, x, k);
        const double no_kr0 = mutated_profile(
            gamma, 1.0 + gamma * gamma * r0 / (8.0 * k), mode.u1(),
            mode.u2(), mode.u3(), r0, x, k);
        require_true("p=1 mutation rejected",
                     std::abs(p_one - mode.profile()) >
                         1.0e-4 * std::abs(mode.profile()));
        require_true("omitted k r0/2 mutation rejected",
                     std::abs(no_kr0 - mode.profile()) >
                         1.0e-4 * std::abs(mode.profile()));
        if (gamma > 0.0)
        {
            const double no_sqrt = mutated_profile(
                0.0, mode.power(), mode.u1(), mode.u2(), mode.u3(), r0, x,
                k);
            require_true("omitted Z4 sqrt(x) exponent rejected",
                         std::abs(no_sqrt - mode.profile()) >
                             1.0e-4 * std::abs(mode.profile()));
        }
        else
        {
            require_close("physical mode has no sqrt(x) exponent",
                          mode.gamma(), 0.0, 0.0);
        }
        const double no_x_three_halves = mutated_profile(
            gamma, mode.power(), mode.u1(), mode.u2(), 0.0, r0, x, k);
        if (std::abs(mode.u3()) > 1.0e-14)
        {
            require_true("omitted x^-3/2 recursion mutation rejected",
                         std::abs(no_x_three_halves - mode.profile()) >
                             1.0e-8 * std::abs(mode.profile()));
        }
        else
        {
            require_close("vanishing physical x^-3/2 coefficient retained",
                          mode.u3(), 0.0, 1.0e-14);
        }
    }
    const auto vector = Outer::make_diagnostic_scalar_wkb_profile(
        Block::vector_z4, r0, x, k, true);
    const auto scalar = Outer::make_diagnostic_scalar_wkb_profile(
        Block::scalar_z4, r0, x, k, true);
    require_true("0.1/0.5 damping swap mutation rejected",
                 std::abs(vector.profile() - scalar.profile()) >
                     1.0e-3 * std::abs(vector.profile()));

    const auto basis =
        Outer::make_diagnostic_characteristic_basis(r0, x, k);
    for (std::size_t column = 0; column < basis.size(); ++column)
    {
        for (std::size_t row = 0; row < Outer::amplitude_count; ++row)
        {
            const bool expected_nonzero =
                row == Outer::amplitude_index(
                           Outer::outgoing_amplitude(blocks[column]));
            require_true("one-hot diagnostic column ownership",
                         expected_nonzero ? basis[column][row] != 0.0
                                          : basis[column][row] == 0.0);
        }
    }
}

void check_residual_convergence()
{
    constexpr double r0 = 1.0;
    constexpr double k = 0.8;
    const std::array<double, 5> x_values = {10.0, 20.0, 40.0, 80.0, 160.0};
    std::cout << "INFO scalar profile residual convergence\n";
    std::cout << "  x max_residual scaled_x2\n";
    double previous = 0.0;
    for (std::size_t i = 0; i < x_values.size(); ++i)
    {
        double maximum = 0.0;
        for (const auto block : {Block::transverse_trace,
                                 Block::transverse_trace_free,
                                 Block::vector_z4, Block::scalar_z4})
        {
            maximum = std::fmax(
                maximum,
                std::abs(Outer::make_diagnostic_scalar_wkb_profile(
                             block, r0, x_values[i], k, true)
                             .continuum_residual_ratio()));
        }
        std::cout << "  " << std::scientific << std::setprecision(12)
                  << x_values[i] << ' ' << maximum << ' '
                  << maximum * x_values[i] * x_values[i] << '\n';
        if (i > 0)
        {
            require_true("scalar residual converges at least O(x^-2)",
                         maximum <= previous / 3.8);
        }
        previous = maximum;
    }
}

double matrix_max_difference(const Outer::SquareMatrix &left,
                             const Outer::SquareMatrix &right)
{
    double maximum = 0.0;
    for (std::size_t row = 0; row < Outer::amplitude_count; ++row)
    {
        for (std::size_t column = 0; column < Outer::amplitude_count; ++column)
        {
            maximum = std::fmax(maximum,
                                std::abs(left[row][column] -
                                         right[row][column]));
        }
    }
    return maximum;
}

void check_rank_residuals_and_invariance()
{
    constexpr double r0 = 1.0;
    constexpr double x = 14.0;
    constexpr double k = 0.7;
    const auto basis =
        Outer::make_diagnostic_characteristic_basis(r0, x, k);
    const auto projector =
        Outer::make_diagnostic_characteristic_projector(basis);
    require_true("diagnostic basis rank is four",
                 projector.diagnostic_rank() == 4);
    require_true("diagnostic complement rank is nine",
                 projector.complement_rank() == 9);
    require_true("diagnostic basis nullity is four", projector.nullity() == 4);
    require_true("locked basis condition diagnostic finite",
                 std::isfinite(projector.basis_condition_estimate()));
    require_true("locked basis condition diagnostic below 1e6",
                 projector.basis_condition_estimate() < 1.0e6);
    std::cout << "INFO diagnostic projector complement rank="
              << projector.complement_rank()
              << " nullity=" << projector.nullity()
              << " basis_condition=" << std::scientific
              << projector.basis_condition_estimate() << '\n';
    require_true("helper replaces no endpoint PDE rows",
                 !projector.replaces_endpoint_rows());
    require_true("helper creates no boundary equations",
                 projector.boundary_equation_count() == 0);

    Outer::AmplitudeVector mixed_decaying = {};
    for (std::size_t column = 0; column < basis.size(); ++column)
    {
        const double coefficient =
            std::array<double, 4>{0.7, -1.1, 0.4, 1.3}[column];
        const auto projected =
            projector.projected_diagnostic_complement_residual(
            basis[column]);
        require_close("individual diagnostic column annihilated",
                      Outer::norm(projected), 0.0, 2.0e-15);
        for (std::size_t row = 0; row < Outer::amplitude_count; ++row)
        {
            mixed_decaying[row] += coefficient * basis[column][row];
        }
    }
    require_close("mixed diagnostic combination annihilated",
                  Outer::norm(
                      projector.projected_diagnostic_complement_residual(
                          mixed_decaying)),
                  0.0, 3.0e-15);

    for (const auto amplitude : Outer::excluded_amplitudes)
    {
        Outer::AmplitudeVector excluded = {};
        excluded[Outer::amplitude_index(amplitude)] = 1.0;
        require_close("each diagnostic complement selector retained",
                      Outer::norm(
                          projector.projected_diagnostic_complement_residual(
                              excluded)),
                      1.0, 2.0e-15);
    }
    for (const auto block : {Block::transverse_trace,
                             Block::transverse_trace_free,
                             Block::vector_z4, Block::scalar_z4})
    {
        const auto growing = Outer::make_diagnostic_scalar_wkb_profile(
            block, r0, x, k, false);
        Outer::AmplitudeVector profile = {};
        profile[Outer::amplitude_index(Outer::incoming_amplitude(block))] =
            growing.profile();
        require_close("diagnostic incoming scalar profile selected",
                      Outer::norm(
                          projector.projected_diagnostic_complement_residual(
                              profile)),
                      std::abs(growing.profile()),
                      3.0e-15 * std::abs(growing.profile()));
    }

    Outer::DiagnosticCharacteristicBasis transformed = {};
    const std::array<std::array<double, 4>, 4> mixing = {{
        {{2.0, 0.3, -0.2, 0.1}},
        {{-0.4, -1.7, 0.5, 0.2}},
        {{0.6, 0.1, 1.3, -0.3}},
        {{0.2, -0.5, 0.4, 0.9}},
    }};
    for (std::size_t output = 0; output < 4; ++output)
    {
        for (std::size_t input = 0; input < 4; ++input)
        {
            for (std::size_t row = 0; row < Outer::amplitude_count; ++row)
            {
                transformed[output][row] += mixing[output][input] *
                                            basis[input][row];
            }
        }
    }
    const auto transformed_projector =
        Outer::make_diagnostic_characteristic_projector(transformed);
    const double invariance = matrix_max_difference(
        projector.complement_projector(),
        transformed_projector.complement_projector());
    require_true("mixing preserves diagnostic projector",
                 invariance <= 100.0 * std::numeric_limits<double>::epsilon());
    std::cout << "INFO diagnostic projector basis-invariance difference="
              << std::scientific << invariance << '\n';

    Outer::SquareMatrix naive = {};
    for (const auto &column : transformed)
    {
        for (std::size_t row = 0; row < Outer::amplitude_count; ++row)
        {
            for (std::size_t col = 0; col < Outer::amplitude_count; ++col)
            {
                naive[row][col] += column[row] * column[col];
            }
        }
    }
    require_true("basis-dependent unnormalized mutation rejected",
                 matrix_max_difference(naive,
                                       projector.diagnostic_projector()) >
                     1.0e-4);

    Outer::DiagnosticCharacteristicBasis rank_lost = basis;
    rank_lost[3] = rank_lost[2];
    require_domain_error("rank-loss mutation rejected", [&]() {
        Outer::make_diagnostic_characteristic_projector(rank_lost);
    });

    std::vector<Amplitude> locked(Outer::excluded_amplitudes.begin(),
                                  Outer::excluded_amplitudes.end());
    Outer::require_locked_outer_residual_layout(locked);
    std::vector<Amplitude> missing_row = locked;
    missing_row.pop_back();
    require_domain_error("missing residual row count rejected", [&]() {
        Outer::require_locked_outer_residual_layout(missing_row);
    });
    std::vector<Amplitude> omitted_f = locked;
    omitted_f[5] = Amplitude::W_T_out;
    require_domain_error("omitted F Jordan row rejected", [&]() {
        Outer::require_locked_outer_residual_layout(omitted_f);
    });
    std::vector<Amplitude> omitted_g = locked;
    omitted_g[6] = Amplitude::W_TF_out;
    require_domain_error("omitted G Jordan row rejected", [&]() {
        Outer::require_locked_outer_residual_layout(omitted_g);
    });
    std::vector<Amplitude> duplicated_f = locked;
    duplicated_f[Outer::excluded_dimension - 1] = Amplitude::F;
    require_domain_error("duplicated F row rejected", [&]() {
        Outer::require_locked_outer_residual_layout(duplicated_f);
    });
    std::vector<Amplitude> duplicated_g = locked;
    duplicated_g[Outer::excluded_dimension - 1] = Amplitude::G;
    require_domain_error("duplicated G row rejected", [&]() {
        Outer::require_locked_outer_residual_layout(duplicated_g);
    });
    require_domain_error("accidental tenth condition rejected", [&]() {
        Outer::require_locked_outer_residual_row_count(10);
    });
    require_domain_error("thirteen componentwise rows rejected", [&]() {
        Outer::require_locked_outer_residual_row_count(13);
    });

    for (const double kx : {8.0, 10.0, 12.0})
    {
        const auto swept =
            Outer::make_locked_diagnostic_characteristic_projector(
                r0, kx / k, k);
        std::cout << "INFO diagnostic projector kx=" << kx
                  << " basis_condition=" << std::scientific
                  << swept.basis_condition_estimate() << '\n';
        require_true("basis conditioning below 1e6 over kx sweep",
                     swept.basis_condition_estimate() < 1.0e6);
    }
}

void check_parity_and_gates()
{
    constexpr double r0 = 1.0;
    constexpr double x = 13.0;
    constexpr double k = 0.9;
    const auto projector =
        Outer::make_locked_diagnostic_characteristic_projector(r0, x, k);
    const Vector state = make_vector(
        {0.21, -0.17, 0.13, 0.29, -0.31, 0.37, -0.41,
         0.43, 0.47, -0.53, 0.59, -0.61, 0.67});
    const Vector dx = make_vector(
        {-0.19, 0.23, -0.27, 0.33, -0.39, 0.45, -0.49,
         0.55, -0.57, 0.63, -0.69, 0.71, -0.77});
    double allowed_norm = 0.0;
    for (const auto sector : {Sector::P_plus, Sector::P_minus})
    {
        const auto amplitudes =
            Outer::transform_boundary_state(r0, x, k, sector, state, dx);
        const auto residuals =
            projector.selector_residuals(amplitudes);
        double sector_norm = 0.0;
        for (const double residual : residuals)
        {
            sector_norm += residual * residual;
        }
        allowed_norm += sector_norm;
        require_true("diagnostic sector complement rank remains nine",
                     projector.complement_rank() == 9);
        require_true("parity-sector nullity remains four",
                     projector.nullity() == 4);
        require_true("allowed-sector data are nonzero", sector_norm > 1.0e-6);
    }
    require_true("diagnostic sectors carry nonzero transformed data",
                 allowed_norm > 1.0e-6);
    require_true("diagnostic characteristic transform flag true",
                 Operator::
                     outer_diagnostic_characteristic_transform_implemented);
    require_true("diagnostic characteristic projector flag true",
                 Operator::
                     outer_diagnostic_characteristic_projector_implemented);
    require_true("full WKB boundary jets remain false",
                 !Operator::outer_full_wkb_boundary_jets_implemented);
    require_true("outer endpoint implementation remains false",
                 !Operator::outer_boundary_implementation_implemented);
    require_true("outer endpoint validation remains false",
                 !Operator::outer_boundary_validation_implemented);
    require_true("aggregate radial boundary remains incomplete",
                 !Operator::radial_boundary_system_complete);
    require_true("boundary-bearing operator remains false",
                 !Operator::complete_frozen_gauge_operator_implemented);
    require_true("exact quadratic boundary pencil remains false",
                 !Operator::
                     quadratic_pencil_coefficient_representation_implemented);
    require_true("eigensolver remains false",
                 !Operator::eigensolver_implemented);
    require_true("shift-invert remains false",
                 !Operator::shift_invert_implemented);
    require_true("threshold remains false",
                 !Operator::threshold_search_implemented);
    require_true("production wiring remains false",
                 !Operator::production_rhs_wiring_implemented);
    require_true("Stage 4AO-D remains false",
                 !Operator::live_gauge_stage_4ao_d_implemented);
}
} // namespace

int main()
{
    try
    {
        check_boundary_state_transformation();
        check_scalar_profile_and_diagnostic_columns();
        check_residual_convergence();
        check_rank_residuals_and_invariance();
        check_parity_and_gates();
    }
    catch (const std::exception &error)
    {
        std::cerr << "FAIL " << error.what() << '\n';
        return 1;
    }
    std::cout << "PASS Stage 4AO-C diagnostic outer characteristic scaffolding\n";
    return 0;
}
