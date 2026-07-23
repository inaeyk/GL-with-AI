#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_OUTER_PROJECTOR_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_OUTER_PROJECTOR_HPP

#include "Stage4AOFrozenGaugeInnerBoundary.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeOuterProjector
{
namespace Operator = Stage4AOFrozenGaugeOperator;
namespace Connection = Stage4AOFrozenGaugeContractedConnection;
namespace InnerBoundary = Stage4AOFrozenGaugeInnerBoundary;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;
using FourierParitySector = InnerBoundary::FourierParitySector;

static constexpr bool validation_only = true;
static constexpr bool diagnostic_characteristic_transform_implemented = true;
static constexpr bool diagnostic_characteristic_projector_implemented = true;
static constexpr bool full_wkb_boundary_jets_implemented = false;
static constexpr bool wkb_left_nullspace_implemented = false;
static constexpr bool retained_pde_dual_implemented = false;
static constexpr bool endpoint_pde_rows_replaced = false;
static constexpr bool boundary_bearing_operator_implemented = false;
static constexpr bool generalized_or_polynomial_pencil_rows_created = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_wiring_implemented = false;
static constexpr std::size_t amplitude_count = 13;
static constexpr std::size_t decaying_dimension = 4;
static constexpr std::size_t excluded_dimension = 9;
static constexpr int hidden_multiplicity = 2;

enum class LightBlock
{
    transverse_trace,
    transverse_trace_free,
    vector_z4,
    scalar_z4
};

enum class Amplitude
{
    W_T_out,
    W_T_in,
    W_TF_out,
    W_TF_in,
    W_V_out,
    W_V_in,
    W_S_out,
    W_S_in,
    J,
    F,
    G,
    C_h,
    C_A
};

inline constexpr std::array<Amplitude, amplitude_count> amplitude_order = {
    Amplitude::W_T_out,  Amplitude::W_T_in,  Amplitude::W_TF_out,
    Amplitude::W_TF_in, Amplitude::W_V_out,  Amplitude::W_V_in,
    Amplitude::W_S_out, Amplitude::W_S_in,   Amplitude::J,
    Amplitude::F,       Amplitude::G,        Amplitude::C_h,
    Amplitude::C_A};

inline constexpr std::array<Amplitude, decaying_dimension>
    decaying_amplitudes = {Amplitude::W_T_out, Amplitude::W_TF_out,
                           Amplitude::W_V_out, Amplitude::W_S_out};

inline constexpr std::array<Amplitude, excluded_dimension>
    excluded_amplitudes = {
        Amplitude::W_T_in, Amplitude::W_TF_in, Amplitude::W_V_in,
        Amplitude::W_S_in, Amplitude::J,       Amplitude::F,
        Amplitude::G,      Amplitude::C_h,     Amplitude::C_A};

inline constexpr std::size_t amplitude_index(const Amplitude amplitude)
{
    return static_cast<std::size_t>(amplitude);
}

inline constexpr double damping_transport_rate(const LightBlock block)
{
    switch (block)
    {
    case LightBlock::transverse_trace:
    case LightBlock::transverse_trace_free:
        return 0.0;
    case LightBlock::vector_z4:
        return 0.1;
    case LightBlock::scalar_z4:
        return 0.5;
    }
    return 0.0;
}

inline constexpr Amplitude outgoing_amplitude(const LightBlock block)
{
    switch (block)
    {
    case LightBlock::transverse_trace:
        return Amplitude::W_T_out;
    case LightBlock::transverse_trace_free:
        return Amplitude::W_TF_out;
    case LightBlock::vector_z4:
        return Amplitude::W_V_out;
    case LightBlock::scalar_z4:
        return Amplitude::W_S_out;
    }
    return Amplitude::W_T_out;
}

inline constexpr Amplitude incoming_amplitude(const LightBlock block)
{
    switch (block)
    {
    case LightBlock::transverse_trace:
        return Amplitude::W_T_in;
    case LightBlock::transverse_trace_free:
        return Amplitude::W_TF_in;
    case LightBlock::vector_z4:
        return Amplitude::W_V_in;
    case LightBlock::scalar_z4:
        return Amplitude::W_S_in;
    }
    return Amplitude::W_T_in;
}

class TransformedAmplitudes
{
  public:
    double value(const Amplitude amplitude) const
    {
        return m_values[amplitude_index(amplitude)];
    }
    const std::array<double, amplitude_count> &values() const
    {
        return m_values;
    }
    FourierParitySector sector() const { return m_sector; }
    double wavenumber() const { return m_wavenumber; }

  private:
    TransformedAmplitudes(const std::array<double, amplitude_count> &values,
                          const FourierParitySector sector,
                          const double wavenumber)
        : m_values(values), m_sector(sector), m_wavenumber(wavenumber)
    {
    }

    friend TransformedAmplitudes transform_boundary_state(
        double, double, double, FourierParitySector, const Vector &,
        const Vector &);

    std::array<double, amplitude_count> m_values;
    FourierParitySector m_sector;
    double m_wavenumber;
};

inline void require_outer_inputs(const double r0, const double x_out,
                                 const double wavenumber)
{
    if (!std::isfinite(r0) || !std::isfinite(x_out) ||
        !std::isfinite(wavenumber) || !(r0 > 0.0) || !(x_out > r0) ||
        !(wavenumber > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C outer projector requires finite r0>0, x_out>r0, and k>0");
    }
}

inline TransformedAmplitudes transform_boundary_state(
    const double r0, const double x_out, const double wavenumber,
    const FourierParitySector sector, const Vector &state,
    const Vector &radial_derivative)
{
    require_outer_inputs(r0, x_out, wavenumber);

    const double chi = state.value(Variable::chi);
    const double dx_chi = radial_derivative.value(Variable::chi);
    const double h_xx = state.value(Variable::h_xx);
    const double h_xz = state.value(Variable::h_xz);
    const double h_zz = state.value(Variable::h_zz);
    const double h_ww = state.value(Variable::h_ww);
    const double dx_h_xx = radial_derivative.value(Variable::h_xx);
    const double dx_h_xz = radial_derivative.value(Variable::h_xz);
    const double dx_h_zz = radial_derivative.value(Variable::h_zz);
    const double dx_h_ww = radial_derivative.value(Variable::h_ww);

    const auto dz = [&](const Variable variable) {
        return InnerBoundary::fourier_dz_multiplier(variable, wavenumber,
                                                     sector) *
               state.value(variable);
    };
    const auto connection = Connection::compute_contracted_connection_and_z(
        Connection::make_perturbation_jet(
            x_out, h_xx, h_xz, h_zz, h_ww, dx_h_xx, dx_h_xz, dx_h_zz,
            dx_h_ww, dz(Variable::h_xx), dz(Variable::h_xz),
            dz(Variable::h_zz), dz(Variable::h_ww),
            state.value(Variable::hat_Gamma_x),
            state.value(Variable::hat_Gamma_z)));

    const double d_xx = dx_h_xx - dx_chi;
    const double d_xz = dx_h_xz;
    const double d_zz = dx_h_zz - dx_chi;
    const double d_ww = dx_h_ww - dx_chi;
    const double d_t = d_zz + static_cast<double>(hidden_multiplicity) * d_ww;

    const double k_gp = Operator::div_beta_gp(r0, x_out);
    constexpr double one_over_d = 0.25;
    const double p_xx =
        state.value(Variable::A_xx) + one_over_d * state.value(Variable::K) +
        one_over_d * k_gp * h_xx -
        chi * (Operator::gp_background_a_xx(r0, x_out) +
               one_over_d * k_gp);
    const double p_xz = state.value(Variable::A_xz) +
                        one_over_d * k_gp * h_xz;
    const double p_zz =
        state.value(Variable::A_zz) + one_over_d * state.value(Variable::K) +
        one_over_d * k_gp * h_zz -
        chi * (Operator::gp_background_a_zz(r0, x_out) +
               one_over_d * k_gp);
    const double p_ww =
        state.value(Variable::A_ww) + one_over_d * state.value(Variable::K) +
        one_over_d * k_gp * h_ww -
        chi * (Operator::gp_background_a_ww(r0, x_out) +
               one_over_d * k_gp);
    const double p_t = p_zz + static_cast<double>(hidden_multiplicity) * p_ww;
    const double theta_minus_p_t = state.value(Variable::Theta) - p_t;

    std::array<double, amplitude_count> values = {};
    values[amplitude_index(Amplitude::W_T_out)] = p_t + 0.5 * d_t;
    values[amplitude_index(Amplitude::W_T_in)] = p_t - 0.5 * d_t;
    values[amplitude_index(Amplitude::W_TF_out)] =
        (p_zz - p_ww) + 0.5 * (d_zz - d_ww);
    values[amplitude_index(Amplitude::W_TF_in)] =
        (p_zz - p_ww) - 0.5 * (d_zz - d_ww);
    values[amplitude_index(Amplitude::W_V_out)] = p_xz - connection.z_z();
    values[amplitude_index(Amplitude::W_V_in)] = p_xz + connection.z_z();
    values[amplitude_index(Amplitude::W_S_out)] =
        theta_minus_p_t - connection.z_x();
    values[amplitude_index(Amplitude::W_S_in)] =
        theta_minus_p_t + connection.z_x();
    values[amplitude_index(Amplitude::J)] =
        d_xz + static_cast<double>(hidden_multiplicity) * connection.z_z();
    values[amplitude_index(Amplitude::F)] =
        p_xx + p_t - 2.0 * state.value(Variable::Theta);
    values[amplitude_index(Amplitude::G)] =
        d_xx - d_t + 4.0 * connection.z_x();
    values[amplitude_index(Amplitude::C_h)] =
        h_xx + h_zz + static_cast<double>(hidden_multiplicity) * h_ww;
    values[amplitude_index(Amplitude::C_A)] =
        Operator::delta_trace_a_at_point(r0, x_out, state);

    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C outer amplitude transform produced nonfinite data");
        }
    }
    return TransformedAmplitudes(values, sector, wavenumber);
}

// Scalar master-profile diagnostic only. This is not a 13-component WKB
// boundary jet and must not be used to open an outer-boundary gate.
class DiagnosticScalarWkbProfile
{
  public:
    LightBlock block() const { return m_block; }
    double gamma() const { return m_gamma; }
    double leading_radial_exponent() const { return m_leading_exponent; }
    double power() const { return m_power; }
    double u0() const { return 1.0; }
    double u1() const { return m_u1; }
    double u2() const { return m_u2; }
    double u3() const { return m_u3; }
    double profile() const { return m_profile; }
    double radial_log_derivative() const { return m_log_derivative; }
    double continuum_residual_ratio() const { return m_residual_ratio; }
    bool decaying() const { return m_decaying; }
    int retained_half_orders() const { return 3; }

  private:
    DiagnosticScalarWkbProfile(const LightBlock block, const double gamma,
                               const double leading_exponent,
                               const double power, const double u1,
                               const double u2, const double u3,
                               const double profile,
                               const double log_derivative,
                               const double residual_ratio,
                               const bool decaying)
        : m_block(block), m_gamma(gamma),
          m_leading_exponent(leading_exponent), m_power(power), m_u1(u1),
          m_u2(u2), m_u3(u3), m_profile(profile),
          m_log_derivative(log_derivative), m_residual_ratio(residual_ratio),
          m_decaying(decaying)
    {
    }

    friend DiagnosticScalarWkbProfile make_diagnostic_scalar_wkb_profile(
        LightBlock, double, double, double, bool);

    LightBlock m_block;
    double m_gamma;
    double m_leading_exponent;
    double m_power;
    double m_u1;
    double m_u2;
    double m_u3;
    double m_profile;
    double m_log_derivative;
    double m_residual_ratio;
    bool m_decaying;
};

inline DiagnosticScalarWkbProfile make_diagnostic_scalar_wkb_profile(
    const LightBlock block, const double r0, const double x_out,
    const double wavenumber, const bool decaying = true)
{
    require_outer_inputs(r0, x_out, wavenumber);
    const double gamma = damping_transport_rate(block);
    const double leading = decaying ? -wavenumber : wavenumber;
    const double sqrt_r0 = std::sqrt(r0);
    const double a_log = -0.5 * gamma * sqrt_r0;
    const double power =
        decaying
            ? 1.0 + 0.5 * wavenumber * r0 +
                  gamma * gamma * r0 / (8.0 * wavenumber)
            : 1.0 - 0.5 * wavenumber * r0 -
                  gamma * gamma * r0 / (8.0 * wavenumber);
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

    const double t = 1.0 / std::sqrt(x_out);
    const double t2 = t * t;
    const double t3 = t2 * t;
    const double t4 = t2 * t2;
    const double t5 = t4 * t;
    const double t6 = t3 * t3;
    const double t7 = t6 * t;
    const double series = 1.0 + u1 * t + u2 * t2 + u3 * t3;
    if (!(series > 0.0) || !std::isfinite(series))
    {
        throw std::domain_error(
            "Stage 4AO-C outer WKB series normalization is invalid");
    }
    const double series_x =
        -0.5 * u1 * t3 - u2 * t4 - 1.5 * u3 * t5;
    const double series_xx =
        0.75 * u1 * t5 + 2.0 * u2 * t6 + 3.75 * u3 * t7;
    const double base_log_derivative = leading + a_log * t + b_log * t2;
    const double base_log_derivative_x =
        -0.5 * a_log * t3 - b_log * t4;
    const double series_log_derivative = series_x / series;
    const double log_derivative =
        base_log_derivative + series_log_derivative;
    const double log_derivative_x =
        base_log_derivative_x + series_xx / series -
        series_log_derivative * series_log_derivative;
    const double beta_squared = r0 / x_out;
    const double radial_coefficient =
        2.0 / x_out - r0 / (x_out * x_out) +
        gamma * std::sqrt(r0 / x_out);
    const double residual_ratio =
        (1.0 - beta_squared) *
            (log_derivative_x + log_derivative * log_derivative) +
        radial_coefficient * log_derivative - wavenumber * wavenumber;
    const double exponent = leading * x_out - gamma * std::sqrt(r0 * x_out);
    const double profile =
        std::exp(exponent) * std::pow(x_out, -power) * series;
    if (!std::isfinite(profile) || !std::isfinite(log_derivative) ||
        !std::isfinite(residual_ratio))
    {
        throw std::domain_error(
            "Stage 4AO-C outer WKB construction produced nonfinite data");
    }
    return DiagnosticScalarWkbProfile(block, gamma, leading, power, u1, u2,
                                      u3, profile, log_derivative,
                                      residual_ratio, decaying);
}

using AmplitudeVector = std::array<double, amplitude_count>;
using DiagnosticCharacteristicBasis =
    std::array<AmplitudeVector, decaying_dimension>;
using SquareMatrix =
    std::array<std::array<double, amplitude_count>, amplitude_count>;

inline DiagnosticCharacteristicBasis make_diagnostic_characteristic_basis(
    const double r0, const double x_out, const double wavenumber)
{
    require_outer_inputs(r0, x_out, wavenumber);
    DiagnosticCharacteristicBasis basis = {};
    const std::array<LightBlock, decaying_dimension> blocks = {
        LightBlock::transverse_trace, LightBlock::transverse_trace_free,
        LightBlock::vector_z4, LightBlock::scalar_z4};
    for (std::size_t column = 0; column < blocks.size(); ++column)
    {
        const auto mode = make_diagnostic_scalar_wkb_profile(
            blocks[column], r0, x_out, wavenumber, true);
        basis[column][amplitude_index(outgoing_amplitude(blocks[column]))] =
            mode.profile();
    }
    return basis;
}

inline double dot(const AmplitudeVector &left, const AmplitudeVector &right)
{
    double result = 0.0;
    for (std::size_t i = 0; i < amplitude_count; ++i)
    {
        result += left[i] * right[i];
    }
    return result;
}

inline double norm(const AmplitudeVector &vector)
{
    return std::sqrt(dot(vector, vector));
}

class DiagnosticCharacteristicProjector
{
  public:
    const DiagnosticCharacteristicBasis &basis_columns() const
    {
        return m_basis;
    }
    const SquareMatrix &diagnostic_projector() const { return m_decaying; }
    const SquareMatrix &complement_projector() const { return m_excluded; }
    std::size_t diagnostic_rank() const { return decaying_dimension; }
    std::size_t complement_rank() const { return excluded_dimension; }
    std::size_t nullity() const { return decaying_dimension; }
    double basis_condition_estimate() const { return m_condition; }
    bool replaces_endpoint_rows() const { return false; }
    std::size_t boundary_equation_count() const { return 0; }

    std::array<double, excluded_dimension>
    selector_residuals(const TransformedAmplitudes &amplitudes) const
    {
        std::array<double, excluded_dimension> residuals = {};
        for (std::size_t i = 0; i < excluded_amplitudes.size(); ++i)
        {
            const auto amplitude = excluded_amplitudes[i];
            const bool divide_by_k =
                amplitude == Amplitude::J || amplitude == Amplitude::F ||
                amplitude == Amplitude::G || amplitude == Amplitude::C_A;
            residuals[i] =
                amplitudes.value(amplitude) /
                (divide_by_k ? amplitudes.wavenumber() : 1.0);
        }
        return residuals;
    }

    AmplitudeVector projected_diagnostic_complement_residual(
        const AmplitudeVector &amplitudes) const
    {
        AmplitudeVector result = {};
        for (std::size_t row = 0; row < amplitude_count; ++row)
        {
            for (std::size_t column = 0; column < amplitude_count; ++column)
            {
                result[row] += m_excluded[row][column] * amplitudes[column];
            }
        }
        return result;
    }

  private:
    DiagnosticCharacteristicProjector(
        const DiagnosticCharacteristicBasis &basis,
        const SquareMatrix &decaying, const SquareMatrix &excluded,
        const double condition)
        : m_basis(basis), m_decaying(decaying), m_excluded(excluded),
          m_condition(condition)
    {
    }

    friend DiagnosticCharacteristicProjector
    make_diagnostic_characteristic_projector(
        const DiagnosticCharacteristicBasis &);

    DiagnosticCharacteristicBasis m_basis;
    SquareMatrix m_decaying;
    SquareMatrix m_excluded;
    double m_condition;
};

inline DiagnosticCharacteristicProjector
make_diagnostic_characteristic_projector(
    const DiagnosticCharacteristicBasis &basis)
{
    DiagnosticCharacteristicBasis orthonormal = {};
    double largest_norm = 0.0;
    double smallest_residual = std::numeric_limits<double>::infinity();
    for (std::size_t column = 0; column < decaying_dimension; ++column)
    {
        AmplitudeVector candidate = basis[column];
        const double original_norm = norm(candidate);
        if (!std::isfinite(original_norm) || !(original_norm > 0.0))
        {
            throw std::domain_error(
                "Stage 4AO-C diagnostic characteristic basis contains a zero or nonfinite column");
        }
        largest_norm = std::fmax(largest_norm, original_norm);
        for (std::size_t previous = 0; previous < column; ++previous)
        {
            const double projection = dot(candidate, orthonormal[previous]);
            for (std::size_t row = 0; row < amplitude_count; ++row)
            {
                candidate[row] -= projection * orthonormal[previous][row];
            }
        }
        const double residual_norm = norm(candidate);
        if (!std::isfinite(residual_norm) ||
            !(residual_norm > 256.0 * std::numeric_limits<double>::epsilon() *
                                   original_norm))
        {
            throw std::domain_error(
                "Stage 4AO-C diagnostic characteristic basis lost rank before four columns");
        }
        smallest_residual = std::fmin(smallest_residual, residual_norm);
        for (std::size_t row = 0; row < amplitude_count; ++row)
        {
            orthonormal[column][row] = candidate[row] / residual_norm;
        }
    }

    SquareMatrix decaying = {};
    SquareMatrix excluded = {};
    for (std::size_t row = 0; row < amplitude_count; ++row)
    {
        for (std::size_t column = 0; column < amplitude_count; ++column)
        {
            for (std::size_t basis_column = 0;
                 basis_column < decaying_dimension; ++basis_column)
            {
                decaying[row][column] += orthonormal[basis_column][row] *
                                         orthonormal[basis_column][column];
            }
            excluded[row][column] =
                (row == column ? 1.0 : 0.0) - decaying[row][column];
        }
    }
    return DiagnosticCharacteristicProjector(
        basis, decaying, excluded, largest_norm / smallest_residual);
}

inline DiagnosticCharacteristicProjector
make_locked_diagnostic_characteristic_projector(
    const double r0, const double x_out, const double wavenumber)
{
    return make_diagnostic_characteristic_projector(
        make_diagnostic_characteristic_basis(r0, x_out, wavenumber));
}

inline void require_locked_outer_residual_row_count(const std::size_t rows)
{
    if (rows != excluded_dimension)
    {
        throw std::domain_error(
            "Stage 4AO-C outer transformed residual requires exactly nine rows");
    }
}

inline void require_locked_outer_residual_layout(
    const std::vector<Amplitude> &layout)
{
    require_locked_outer_residual_row_count(layout.size());
    std::array<bool, amplitude_count> seen = {};
    for (const auto amplitude : layout)
    {
        const auto index = amplitude_index(amplitude);
        if (seen[index])
        {
            throw std::domain_error(
                "Stage 4AO-C outer transformed residual duplicates an amplitude row");
        }
        seen[index] = true;
    }
    for (const auto amplitude : excluded_amplitudes)
    {
        if (!seen[amplitude_index(amplitude)])
        {
            throw std::domain_error(
                "Stage 4AO-C outer transformed residual omits a locked excluded row");
        }
    }
}

} // namespace Stage4AOFrozenGaugeOuterProjector
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_OUTER_PROJECTOR_HPP */
