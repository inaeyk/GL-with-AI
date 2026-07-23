#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_RADIAL_BOUNDARY_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_RADIAL_BOUNDARY_HPP

#include "Stage4AOFrozenGaugeOuterProjector.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeRadialBoundary
{
namespace Operator = Stage4AOFrozenGaugeOperator;
namespace Inner = Stage4AOFrozenGaugeInnerBoundary;
namespace Outer = Stage4AOFrozenGaugeOuterProjector;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;
using Sector = Inner::FourierParitySector;
using Rows = std::array<double, 13>;
using BoundaryRows = std::array<double, 9>;
using Matrix9x13 = std::array<Rows, 9>;
using Matrix4x13 = std::array<Rows, 4>;

static constexpr bool validation_only = true;
static constexpr bool outer_endpoint_implemented = false;
static constexpr bool outer_endpoint_validated = false;
static constexpr bool radial_boundary_system_validated = false;
static constexpr bool boundary_bearing_operator_implemented = false;
static constexpr bool quadratic_pencil_representation_exact = false;
static constexpr bool mirrored_outer_derivative_scaffolding_implemented = true;
static constexpr bool diagnostic_radial_row_layout_implemented = true;
static constexpr bool k_zero_supported = false;
static constexpr bool nonzero_growth_asymptotics_supported = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_wiring_implemented = false;
static constexpr std::size_t outer_dx_required_points = 3;
static constexpr std::size_t outer_dxx_required_points = 4;
static constexpr std::size_t outer_full_row_required_points = 4;
inline constexpr std::array<double, 3> outer_dx_coefficients =
    {1.5, -2.0, 0.5};
inline constexpr std::array<double, 4> outer_dxx_coefficients =
    {2.0, -5.0, 4.0, -1.0};

inline Rows rows_from_vector(const Vector &input)
{
    Rows rows = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        rows[Operator::variable_index(variable)] = input.value(variable);
    }
    return rows;
}

inline Vector vector_from_rows(const Rows &rows)
{
    return Operator::make_frozen_gauge_perturbation_vector(rows);
}

inline void require_outer_samples(const std::vector<double> &values,
                                  const std::size_t required)
{
    if (values.size() < required)
    {
        throw std::domain_error(
            "Stage 4AO-C outer endpoint stencil has insufficient radial reach");
    }
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C outer endpoint stencil requires finite data");
        }
    }
}

inline double outer_dx(const std::vector<double> &last_first,
                       const double dx)
{
    Inner::require_positive_spacing(dx);
    require_outer_samples(last_first, outer_dx_required_points);
    double result = 0.0;
    for (std::size_t i = 0; i < outer_dx_coefficients.size(); ++i)
    {
        result += outer_dx_coefficients[i] * last_first[i];
    }
    return result / dx;
}

inline double outer_dxx(const std::vector<double> &last_first,
                        const double dx)
{
    Inner::require_positive_spacing(dx);
    require_outer_samples(last_first, outer_dxx_required_points);
    double result = 0.0;
    for (std::size_t i = 0; i < outer_dxx_coefficients.size(); ++i)
    {
        result += outer_dxx_coefficients[i] * last_first[i];
    }
    return result / (dx * dx);
}

inline Inner::InnerEndpointDerivativeJet make_outer_derivative_jet(
    const std::vector<Vector> &radial_values, const double dx,
    const double wavenumber, const Sector sector)
{
    Inner::require_positive_spacing(dx);
    if (radial_values.size() < outer_full_row_required_points)
    {
        throw std::domain_error(
            "Stage 4AO-C outer endpoint requires at least four radial points");
    }
    Rows value = {};
    Rows radial_first = {};
    Rows radial_second = {};
    const std::size_t last = radial_values.size() - 1;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto slot = Operator::variable_index(variable);
        std::vector<double> samples;
        samples.reserve(outer_full_row_required_points);
        for (std::size_t offset = 0; offset < outer_full_row_required_points;
             ++offset)
        {
            samples.push_back(radial_values[last - offset].value(variable));
        }
        value[slot] = samples[0];
        radial_first[slot] = outer_dx(samples, dx);
        radial_second[slot] = outer_dxx(samples, dx);
    }
    return Inner::make_boundary_derivative_jet(
        vector_from_rows(value), vector_from_rows(radial_first),
        vector_from_rows(radial_second), wavenumber, sector);
}

inline Inner::InnerEndpointDerivativeJet make_centered_derivative_jet(
    const std::vector<Vector> &radial_values, const std::size_t index,
    const double dx, const double wavenumber, const Sector sector)
{
    Inner::require_positive_spacing(dx);
    if (index == 0 || index + 1 >= radial_values.size())
    {
        throw std::domain_error(
            "Stage 4AO-C centered radial derivative requires an interior point");
    }
    Rows value = {};
    Rows radial_first = {};
    Rows radial_second = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto slot = Operator::variable_index(variable);
        const double left = radial_values[index - 1].value(variable);
        const double middle = radial_values[index].value(variable);
        const double right = radial_values[index + 1].value(variable);
        value[slot] = middle;
        radial_first[slot] = (right - left) / (2.0 * dx);
        radial_second[slot] =
            (right - 2.0 * middle + left) / (dx * dx);
    }
    return Inner::make_boundary_derivative_jet(
        vector_from_rows(value), vector_from_rows(radial_first),
        vector_from_rows(radial_second), wavenumber, sector);
}

// Diagnostic characteristic rows only. These selectors are not the primary
// Stage 4AO-C WKB boundary residual because no full 13-component WKB jet or
// left nullspace is available yet.
inline BoundaryRows diagnostic_characteristic_boundary_residual(
    const double r0, const double x_out, const double wavenumber,
    const Sector sector, const Vector &state, const Vector &radial_derivative,
    const Outer::DiagnosticCharacteristicBasis &basis)
{
    const auto amplitudes = Outer::transform_boundary_state(
        r0, x_out, wavenumber, sector, state, radial_derivative);
    return Outer::make_diagnostic_characteristic_projector(basis)
        .selector_residuals(amplitudes);
}

inline BoundaryRows diagnostic_characteristic_boundary_residual(
    const double r0, const double x_out, const double wavenumber,
    const Sector sector, const Vector &state, const Vector &radial_derivative)
{
    return diagnostic_characteristic_boundary_residual(
        r0, x_out, wavenumber, sector, state, radial_derivative,
        Outer::make_diagnostic_characteristic_basis(r0, x_out, wavenumber));
}

inline BoundaryRows diagnostic_transformed_dirichlet_residual(
    const double r0, const double x_out, const double wavenumber,
    const Sector sector, const Vector &state, const Vector &radial_derivative)
{
    const auto amplitudes = Outer::transform_boundary_state(
        r0, x_out, wavenumber, sector, state, radial_derivative);
    BoundaryRows result = {};
    result[0] = amplitudes.value(Outer::Amplitude::W_T_out) +
                amplitudes.value(Outer::Amplitude::W_T_in);
    result[1] = amplitudes.value(Outer::Amplitude::W_TF_out) +
                amplitudes.value(Outer::Amplitude::W_TF_in);
    result[2] = amplitudes.value(Outer::Amplitude::W_V_out) +
                amplitudes.value(Outer::Amplitude::W_V_in);
    result[3] = amplitudes.value(Outer::Amplitude::W_S_out) +
                amplitudes.value(Outer::Amplitude::W_S_in);
    const auto diagnostic = diagnostic_characteristic_boundary_residual(
        r0, x_out, wavenumber, sector, state, radial_derivative);
    for (std::size_t i = 4; i < result.size(); ++i)
    {
        result[i] = diagnostic[i];
    }
    return result;
}

inline Matrix9x13 diagnostic_characteristic_boundary_matrix(
    const double r0, const double x_out, const double wavenumber,
    const Sector sector, const bool radial_part)
{
    Rows zero_rows = {};
    const auto zero = vector_from_rows(zero_rows);
    Matrix9x13 matrix = {};
    for (std::size_t column = 0; column < 13; ++column)
    {
        Rows unit_rows = {};
        unit_rows[column] = 1.0;
        const auto unit = vector_from_rows(unit_rows);
        const auto residual = diagnostic_characteristic_boundary_residual(
            r0, x_out, wavenumber, sector,
            radial_part ? zero : unit, radial_part ? unit : zero);
        for (std::size_t row = 0; row < residual.size(); ++row)
        {
            matrix[row][column] = residual[row];
        }
    }
    return matrix;
}

inline std::size_t numerical_row_rank(const Matrix9x13 &matrix,
                                      const double tolerance = 1.0e-11)
{
    Matrix9x13 work = matrix;
    std::size_t rank = 0;
    for (std::size_t column = 0; column < 13 && rank < 9; ++column)
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
        if (std::abs(work[pivot][column]) <= tolerance)
        {
            continue;
        }
        const auto temporary = work[rank];
        work[rank] = work[pivot];
        work[pivot] = temporary;
        const double divisor = work[rank][column];
        for (std::size_t entry = column; entry < 13; ++entry)
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
            for (std::size_t entry = column; entry < 13; ++entry)
            {
                work[row][entry] -= factor * work[rank][entry];
            }
        }
        ++rank;
    }
    return rank;
}

inline Outer::DiagnosticCharacteristicBasis
orthonormal_diagnostic_characteristic_basis(
    const Outer::DiagnosticCharacteristicBasis &basis)
{
    Outer::DiagnosticCharacteristicBasis result = {};
    for (std::size_t column = 0; column < basis.size(); ++column)
    {
        auto candidate = basis[column];
        for (std::size_t previous = 0; previous < column; ++previous)
        {
            const double projection = Outer::dot(candidate, result[previous]);
            for (std::size_t row = 0; row < candidate.size(); ++row)
            {
                candidate[row] -= projection * result[previous][row];
            }
        }
        const double length = Outer::norm(candidate);
        if (!std::isfinite(length) ||
            length <= 256.0 * std::numeric_limits<double>::epsilon())
        {
            throw std::domain_error(
                "Stage 4AO-C retained outer PDE basis lost rank");
        }
        for (std::size_t row = 0; row < candidate.size(); ++row)
        {
            result[column][row] = candidate[row] / length;
        }
    }
    return result;
}

// Layout experiment only: the diagnostic amplitude coordinates do not supply
// the stationary-symbol left dual required for retained production PDE rows.
inline std::array<double, 4> diagnostic_retained_pde_residuals(
    const double r0, const double x_out, const double wavenumber,
    const Sector sector, const Vector &pde_residual,
    const Outer::DiagnosticCharacteristicBasis &basis)
{
    Rows zero_rows = {};
    const auto transformed = Outer::transform_boundary_state(
        r0, x_out, wavenumber, sector, pde_residual,
        vector_from_rows(zero_rows));
    const auto orthonormal =
        orthonormal_diagnostic_characteristic_basis(basis);
    std::array<double, 4> result = {};
    for (std::size_t row = 0; row < result.size(); ++row)
    {
        result[row] = Outer::dot(orthonormal[row], transformed.values());
    }
    return result;
}

class DiagnosticOuterRowLayout
{
  public:
    const Rows &equations() const { return m_equations; }
    const BoundaryRows &boundary_residuals() const { return m_boundary; }
    std::size_t retained_pde_rows() const { return 4; }
    std::size_t boundary_rows() const { return 9; }
    std::size_t total_rows() const { return 13; }
    bool physical_values_reset() const { return false; }
    bool radial_ghost_unknowns_stored() const { return false; }

  private:
    DiagnosticOuterRowLayout(const Rows &equations,
                             const BoundaryRows &boundary)
        : m_equations(equations), m_boundary(boundary)
    {
    }
    friend DiagnosticOuterRowLayout apply_diagnostic_outer_row_layout(
        const Operator::RadialGrid &, const std::vector<Vector> &, double,
        Sector, const Outer::DiagnosticCharacteristicBasis &);
    Rows m_equations;
    BoundaryRows m_boundary;
};

inline DiagnosticOuterRowLayout apply_diagnostic_outer_row_layout(
    const Operator::RadialGrid &grid, const std::vector<Vector> &radial_values,
    const double wavenumber, const Sector sector,
    const Outer::DiagnosticCharacteristicBasis &basis)
{
    if (radial_values.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C diagnostic outer row layout must match the radial grid");
    }
    Outer::require_outer_inputs(grid.domain().r0(), grid.domain().x_out(),
                                wavenumber);
    const auto jet =
        make_outer_derivative_jet(radial_values, grid.dx(), wavenumber, sector);
    const auto pde = Inner::apply_complete_interior_from_jet(
        grid.domain().r0(), grid.domain().x_out(), jet);
    const auto retained = diagnostic_retained_pde_residuals(
        grid.domain().r0(), grid.domain().x_out(), wavenumber, sector, pde,
        basis);
    const auto boundary = diagnostic_characteristic_boundary_residual(
        grid.domain().r0(), grid.domain().x_out(), wavenumber, sector,
        jet.value(), jet.dx(), basis);
    Rows equations = {};
    for (std::size_t i = 0; i < retained.size(); ++i)
    {
        equations[i] = retained[i];
    }
    for (std::size_t i = 0; i < boundary.size(); ++i)
    {
        equations[4 + i] = boundary[i];
    }
    return DiagnosticOuterRowLayout(equations, boundary);
}

inline DiagnosticOuterRowLayout apply_diagnostic_outer_row_layout(
    const Operator::RadialGrid &grid, const std::vector<Vector> &radial_values,
    const double wavenumber, const Sector sector)
{
    return apply_diagnostic_outer_row_layout(
        grid, radial_values, wavenumber, sector,
        Outer::make_diagnostic_characteristic_basis(
            grid.domain().r0(), grid.domain().x_out(), wavenumber));
}

enum class RadialRowOwner
{
    inner_pde,
    centered_interior_pde,
    diagnostic_outer_projected_pde_and_characteristic_rows
};

struct RadialPointEquations
{
    Rows equations;
    RadialRowOwner owner;
    std::size_t pde_rows;
    std::size_t boundary_rows;
};

class DiagnosticRadialRowLayoutResult
{
  public:
    const std::vector<RadialPointEquations> &points() const { return m_points; }
    std::size_t radial_points() const { return m_points.size(); }
    std::size_t equations_per_point() const { return 13; }
    std::size_t total_equations() const { return 13 * m_points.size(); }
    bool radial_ghost_unknowns_stored() const { return false; }
    Sector sector() const { return m_sector; }

  private:
    DiagnosticRadialRowLayoutResult(
        const std::vector<RadialPointEquations> &points, const Sector sector)
        : m_points(points), m_sector(sector)
    {
    }
    friend DiagnosticRadialRowLayoutResult apply_diagnostic_radial_row_layout(
        const Operator::RadialGrid &, const std::vector<Vector> &, double,
        Sector);
    std::vector<RadialPointEquations> m_points;
    Sector m_sector;
};

inline DiagnosticRadialRowLayoutResult apply_diagnostic_radial_row_layout(
    const Operator::RadialGrid &grid, const std::vector<Vector> &radial_values,
    const double wavenumber, const Sector sector)
{
    if (radial_values.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C diagnostic radial row layout must match the grid");
    }
    Outer::require_outer_inputs(grid.domain().r0(), grid.domain().x_out(),
                                wavenumber);
    std::vector<RadialPointEquations> result;
    result.reserve(grid.points());
    const auto inner = Inner::apply_inner_endpoint_operator(
        grid, radial_values, wavenumber, sector);
    result.push_back({rows_from_vector(inner.rhs()),
                      RadialRowOwner::inner_pde, 13, 0});
    for (std::size_t index = 1; index + 1 < grid.points(); ++index)
    {
        const auto jet = make_centered_derivative_jet(
            radial_values, index, grid.dx(), wavenumber, sector);
        result.push_back(
            {rows_from_vector(Inner::apply_complete_interior_from_jet(
                 grid.domain().r0(), grid.x(index), jet)),
             RadialRowOwner::centered_interior_pde, 13, 0});
    }
    const auto outer = apply_diagnostic_outer_row_layout(
        grid, radial_values, wavenumber, sector);
    result.push_back({outer.equations(),
                      RadialRowOwner::
                          diagnostic_outer_projected_pde_and_characteristic_rows,
                      4, 9});
    return DiagnosticRadialRowLayoutResult(result, sector);
}

} // namespace Stage4AOFrozenGaugeRadialBoundary
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_RADIAL_BOUNDARY_HPP */
