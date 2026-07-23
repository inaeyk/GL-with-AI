#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_INNER_BOUNDARY_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_INNER_BOUNDARY_HPP

#include "Stage4AOFrozenGaugeOperator.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeInnerBoundary
{
namespace Operator = Stage4AOFrozenGaugeOperator;
namespace Adapter = Stage4AOFrozenGaugeZDerivativeAdapter;
namespace Connection = Stage4AOFrozenGaugeContractedConnection;
namespace Ricci = CartoonRicci;
namespace RicciAssembly = Stage4AOFrozenGaugeRicciAssembly;
namespace HiddenRww = Stage4AOFrozenGaugeHiddenRww;
namespace VisibleRxx = Stage4AOFrozenGaugeVisibleRxx;
namespace VisibleRxz = Stage4AOFrozenGaugeVisibleRxz;
namespace VisibleRzz = Stage4AOFrozenGaugeVisibleRzz;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

static constexpr bool validation_only = true;
static constexpr bool inner_endpoint_derivative_helper_implemented = true;
static constexpr bool inner_full_row_application_implemented = true;
static constexpr bool inner_pure_outflow_contract_implemented = true;
static constexpr bool outer_boundary_implemented = false;
static constexpr bool boundary_system_complete = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_wiring_implemented = false;

inline constexpr std::array<double, 3> inner_dx_coefficients = {
    -1.5, 2.0, -0.5};
inline constexpr std::array<double, 4> inner_dxx_coefficients = {
    2.0, -5.0, 4.0, -1.0};
static constexpr std::size_t inner_dx_required_points = 3;
static constexpr std::size_t inner_dxx_required_points = 4;
static constexpr std::size_t inner_full_row_required_points = 4;
static constexpr std::size_t inner_dx_maximum_offset = 2;
static constexpr std::size_t inner_dxx_maximum_offset = 3;

enum class FourierParitySector
{
    P_plus,
    P_minus
};

enum class InnerCausalClassification
{
    pure_outflow,
    glancing,
    invalid_for_no_data_contract
};

class InnerOutflowReport
{
  public:
    double r0() const { return m_r0; }
    double x_in() const { return m_x_in; }
    double x_in_over_r0() const { return m_x_in / m_r0; }
    double v_in() const { return m_v_in; }
    double c_plus() const { return 1.0 - m_v_in; }
    double c_minus() const { return -1.0 - m_v_in; }
    double c_zero() const { return -m_v_in; }
    InnerCausalClassification classification() const
    {
        return m_classification;
    }
    bool pure_outflow() const
    {
        return m_classification == InnerCausalClassification::pure_outflow;
    }
    bool glancing() const
    {
        return m_classification == InnerCausalClassification::glancing;
    }
    bool valid_for_no_data_contract() const { return pure_outflow(); }
    bool complete_characteristic_diagonalization_claimed() const
    {
        return false;
    }

  private:
    InnerOutflowReport(const double r0, const double x_in, const double v_in,
                       const InnerCausalClassification classification)
        : m_r0(r0), m_x_in(x_in), m_v_in(v_in),
          m_classification(classification)
    {
    }

    friend InnerOutflowReport make_inner_outflow_report(double, double);

    double m_r0;
    double m_x_in;
    double m_v_in;
    InnerCausalClassification m_classification;
};

inline InnerOutflowReport make_inner_outflow_report(const double r0,
                                                    const double x_in)
{
    if (!std::isfinite(r0) || !std::isfinite(x_in) || !(r0 > 0.0) ||
        !(x_in > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C inner outflow contract requires finite r0>0 and x_in>0");
    }
    const double v_in = std::sqrt(r0 / x_in);
    const auto classification =
        x_in < r0
            ? InnerCausalClassification::pure_outflow
            : x_in == r0
                  ? InnerCausalClassification::glancing
                  : InnerCausalClassification::invalid_for_no_data_contract;
    return InnerOutflowReport(r0, x_in, v_in, classification);
}

inline void require_positive_spacing(const double dx)
{
    if (!std::isfinite(dx) || !(dx > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C inner endpoint closure requires finite dx>0");
    }
}

inline void require_finite_samples(const std::vector<double> &values)
{
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C inner endpoint closure requires finite samples");
        }
    }
}

inline double inner_dx(const std::vector<double> &values, const double dx)
{
    require_positive_spacing(dx);
    if (values.size() < inner_dx_required_points)
    {
        throw std::domain_error(
            "Stage 4AO-C inner D_x closure requires at least three radial points");
    }
    require_finite_samples(values);
    double numerator = 0.0;
    for (std::size_t i = 0; i < inner_dx_coefficients.size(); ++i)
    {
        numerator += inner_dx_coefficients[i] * values[i];
    }
    return numerator / dx;
}

inline double inner_dxx(const std::vector<double> &values, const double dx)
{
    require_positive_spacing(dx);
    if (values.size() < inner_dxx_required_points)
    {
        throw std::domain_error(
            "Stage 4AO-C inner D_xx closure requires at least four radial points");
    }
    require_finite_samples(values);
    double numerator = 0.0;
    for (std::size_t i = 0; i < inner_dxx_coefficients.size(); ++i)
    {
        numerator += inner_dxx_coefficients[i] * values[i];
    }
    return numerator / (dx * dx);
}

inline double intrinsic_z_parity(const Variable variable)
{
    return variable == Variable::h_xz || variable == Variable::A_xz ||
                   variable == Variable::hat_Gamma_z
               ? -1.0
               : 1.0;
}

inline double fourier_dz_multiplier(const Variable variable,
                                    const double wavenumber,
                                    const FourierParitySector sector)
{
    if (!std::isfinite(wavenumber) || wavenumber < 0.0)
    {
        throw std::domain_error(
            "Stage 4AO-C Fourier endpoint closure requires finite k>=0");
    }
    const double sector_sign =
        sector == FourierParitySector::P_plus ? 1.0 : -1.0;
    return -sector_sign * intrinsic_z_parity(variable) * wavenumber;
}

class InnerEndpointDerivativeJet
{
  public:
    const Vector &value() const { return m_value; }
    const Vector &dx() const { return m_dx; }
    const Vector &dz() const { return m_dz; }
    const Vector &dxx() const { return m_dxx; }
    const Vector &dxz() const { return m_dxz; }
    const Vector &dzz() const { return m_dzz; }
    FourierParitySector sector() const { return m_sector; }
    double wavenumber() const { return m_wavenumber; }
    std::size_t radial_points_consumed() const
    {
        return inner_full_row_required_points;
    }
    bool stores_radial_ghost_unknowns() const { return false; }

  private:
    InnerEndpointDerivativeJet(const Vector &value, const Vector &dx,
                               const Vector &dz, const Vector &dxx,
                               const Vector &dxz, const Vector &dzz,
                               const FourierParitySector sector,
                               const double wavenumber)
        : m_value(value), m_dx(dx), m_dz(dz), m_dxx(dxx), m_dxz(dxz),
          m_dzz(dzz), m_sector(sector), m_wavenumber(wavenumber)
    {
    }

    friend InnerEndpointDerivativeJet make_inner_endpoint_derivative_jet(
        const std::vector<Vector> &, double, double, FourierParitySector);
    friend InnerEndpointDerivativeJet make_boundary_derivative_jet(
        const Vector &, const Vector &, const Vector &, double,
        FourierParitySector);

    Vector m_value;
    Vector m_dx;
    Vector m_dz;
    Vector m_dxx;
    Vector m_dxz;
    Vector m_dzz;
    FourierParitySector m_sector;
    double m_wavenumber;
};

inline InnerEndpointDerivativeJet make_boundary_derivative_jet(
    const Vector &value, const Vector &dx, const Vector &dxx,
    const double wavenumber, const FourierParitySector sector)
{
    if (!std::isfinite(wavenumber) || wavenumber < 0.0)
    {
        throw std::domain_error(
            "Stage 4AO-C boundary derivative jet requires finite k>=0");
    }
    std::array<double, Operator::frozen_gauge_state_vector.size()> dz = {};
    std::array<double, Operator::frozen_gauge_state_vector.size()> dxz = {};
    std::array<double, Operator::frozen_gauge_state_vector.size()> dzz = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto slot = Operator::variable_index(variable);
        const double factor =
            fourier_dz_multiplier(variable, wavenumber, sector);
        dz[slot] = factor * value.value(variable);
        dxz[slot] = factor * dx.value(variable);
        dzz[slot] = -wavenumber * wavenumber * value.value(variable);
    }
    return InnerEndpointDerivativeJet(
        value, dx,
        Operator::make_frozen_gauge_perturbation_vector(dz), dxx,
        Operator::make_frozen_gauge_perturbation_vector(dxz),
        Operator::make_frozen_gauge_perturbation_vector(dzz), sector,
        wavenumber);
}

inline InnerEndpointDerivativeJet make_inner_endpoint_derivative_jet(
    const std::vector<Vector> &radial_amplitudes, const double dx,
    const double wavenumber, const FourierParitySector sector)
{
    require_positive_spacing(dx);
    if (!std::isfinite(wavenumber) || wavenumber < 0.0)
    {
        throw std::domain_error(
            "Stage 4AO-C Fourier endpoint closure requires finite k>=0");
    }
    if (radial_amplitudes.size() < inner_full_row_required_points)
    {
        throw std::domain_error(
            "Stage 4AO-C inner full-row closure requires at least four radial points");
    }

    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    std::array<double, Operator::frozen_gauge_state_vector.size()> dx_values = {};
    std::array<double, Operator::frozen_gauge_state_vector.size()> dz_values = {};
    std::array<double, Operator::frozen_gauge_state_vector.size()> dxx_values = {};
    std::array<double, Operator::frozen_gauge_state_vector.size()> dxz_values = {};
    std::array<double, Operator::frozen_gauge_state_vector.size()> dzz_values = {};

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto slot = Operator::variable_index(variable);
        std::vector<double> samples;
        samples.reserve(inner_full_row_required_points);
        for (std::size_t i = 0; i < inner_full_row_required_points; ++i)
        {
            samples.push_back(radial_amplitudes[i].value(variable));
        }
        const double radial_first = inner_dx(samples, dx);
        const double radial_second = inner_dxx(samples, dx);
        const double dz_factor =
            fourier_dz_multiplier(variable, wavenumber, sector);
        const double value = samples[0];
        values[slot] = value;
        dx_values[slot] = radial_first;
        dz_values[slot] = dz_factor * value;
        dxx_values[slot] = radial_second;
        dxz_values[slot] = dz_factor * radial_first;
        dzz_values[slot] = -wavenumber * wavenumber * value;
    }

    return InnerEndpointDerivativeJet(
        Operator::make_frozen_gauge_perturbation_vector(values),
        Operator::make_frozen_gauge_perturbation_vector(dx_values),
        Operator::make_frozen_gauge_perturbation_vector(dz_values),
        Operator::make_frozen_gauge_perturbation_vector(dxx_values),
        Operator::make_frozen_gauge_perturbation_vector(dxz_values),
        Operator::make_frozen_gauge_perturbation_vector(dzz_values), sector,
        wavenumber);
}

inline ConformalCartoonAlgebra::ConformalMetric
metric_from_vector(const Vector &input)
{
    return {input.value(Variable::h_xx), input.value(Variable::h_xz),
            input.value(Variable::h_zz), input.value(Variable::h_ww)};
}

inline Ricci::MetricDerivatives
metric_derivatives_from_jet(const InnerEndpointDerivativeJet &jet)
{
    return {metric_from_vector(jet.dx()), metric_from_vector(jet.dz()),
            metric_from_vector(jet.dxx()), metric_from_vector(jet.dxz()),
            metric_from_vector(jet.dzz())};
}

inline Ricci::ScalarDerivatives
chi_derivatives_from_jet(const InnerEndpointDerivativeJet &jet)
{
    return {{jet.dx().value(Variable::chi), jet.dz().value(Variable::chi)},
            {jet.dxx().value(Variable::chi),
             jet.dxz().value(Variable::chi),
             jet.dzz().value(Variable::chi)}};
}

inline RicciAssembly::TraceFreeRicciAssembly
geometric_ricci_from_jet(const double x,
                         const InnerEndpointDerivativeJet &jet)
{
    const auto h = metric_from_vector(jet.value());
    const auto derivatives = metric_derivatives_from_jet(jet);
    const double chi = jet.value().value(Variable::chi);
    const auto dchi = chi_derivatives_from_jet(jet);
    const auto rxx = VisibleRxx::compute_visible_physical_delta_rxx(
        VisibleRxx::make_visible_rxx_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rxz = VisibleRxz::compute_visible_physical_delta_rxz(
        VisibleRxz::make_visible_rxz_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rzz = VisibleRzz::compute_visible_physical_delta_rzz(
        VisibleRzz::make_visible_rzz_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rww = HiddenRww::compute_hidden_physical_delta_rww(
        HiddenRww::make_hidden_rww_perturbation(x, h, derivatives, chi,
                                                dchi));
    return RicciAssembly::assemble_trace_free_ricci(
        RicciAssembly::make_raw_ricci_components(rxx, rxz, rzz, rww));
}

inline Vector common_advection_from_jet(const double r0, const double x,
                                        const InnerEndpointDerivativeJet &jet)
{
    return Operator::apply_gp_shift_advection_at_point(r0, x, jet.dx());
}

inline Connection::PerturbationJet
connection_from_jet(const double x, const InnerEndpointDerivativeJet &jet)
{
    return Connection::make_perturbation_jet(
        x, jet.value().value(Variable::h_xx),
        jet.value().value(Variable::h_xz),
        jet.value().value(Variable::h_zz),
        jet.value().value(Variable::h_ww),
        jet.dx().value(Variable::h_xx), jet.dx().value(Variable::h_xz),
        jet.dx().value(Variable::h_zz), jet.dx().value(Variable::h_ww),
        jet.dz().value(Variable::h_xx), jet.dz().value(Variable::h_xz),
        jet.dz().value(Variable::h_zz), jet.dz().value(Variable::h_ww),
        jet.value().value(Variable::hat_Gamma_x),
        jet.value().value(Variable::hat_Gamma_z));
}

inline Adapter::DerivativeAdapterResult
encoded_z_adapter_from_jet(const double x,
                           const InnerEndpointDerivativeJet &jet)
{
    return Adapter::compute_derivative_adapter(
        Adapter::make_derivative_adapter_input(
            x, metric_from_vector(jet.value()),
            metric_derivatives_from_jet(jet),
            jet.value().value(Variable::hat_Gamma_x),
            jet.value().value(Variable::hat_Gamma_z),
            jet.dx().value(Variable::hat_Gamma_x),
            jet.dz().value(Variable::hat_Gamma_x),
            jet.dx().value(Variable::hat_Gamma_z),
            jet.dz().value(Variable::hat_Gamma_z)));
}

inline Vector apply_complete_interior_from_jet(
    const double r0, const double x, const InnerEndpointDerivativeJet &jet)
{
    const auto adapter = encoded_z_adapter_from_jet(x, jet);
    const auto encoded = Adapter::make_encoded_z_ricci_completion(x, adapter);
    return Operator::apply_complete_frozen_gauge_interior_operator_at_point(
        r0, jet.value(), common_advection_from_jet(r0, x, jet),
        geometric_ricci_from_jet(x, jet), encoded, connection_from_jet(x, jet),
        jet.dx().value(Variable::K), jet.dz().value(Variable::K),
        jet.dx().value(Variable::Theta), jet.dz().value(Variable::Theta),
        jet.dx().value(Variable::chi), jet.dz().value(Variable::chi));
}

class InnerEndpointApplyResult
{
  public:
    const Vector &rhs() const { return m_rhs; }
    const InnerOutflowReport &outflow() const { return m_outflow; }
    FourierParitySector sector() const { return m_sector; }
    std::size_t pde_rows_retained() const
    {
        return Operator::frozen_gauge_state_vector.size();
    }
    std::size_t continuum_boundary_equations() const { return 0; }
    std::size_t complete_interior_owner_applications() const { return 1; }
    bool physical_values_reset() const { return false; }
    bool extrapolated_physical_data() const { return false; }
    bool radial_ghost_unknowns_stored() const { return false; }
    bool algebraic_cleanup_applied() const { return false; }

  private:
    InnerEndpointApplyResult(const Vector &rhs,
                             const InnerOutflowReport &outflow,
                             const FourierParitySector sector)
        : m_rhs(rhs), m_outflow(outflow), m_sector(sector)
    {
    }

    friend InnerEndpointApplyResult apply_inner_endpoint_operator(
        const Operator::RadialGrid &, const std::vector<Vector> &, double,
        FourierParitySector);

    Vector m_rhs;
    InnerOutflowReport m_outflow;
    FourierParitySector m_sector;
};

inline InnerEndpointApplyResult apply_inner_endpoint_operator(
    const Operator::RadialGrid &grid,
    const std::vector<Vector> &radial_amplitudes, const double wavenumber,
    const FourierParitySector sector)
{
    if (radial_amplitudes.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C inner endpoint input must match the radial grid");
    }
    const auto outflow = make_inner_outflow_report(
        grid.domain().r0(), grid.domain().x_in());
    if (!outflow.valid_for_no_data_contract())
    {
        throw std::domain_error(
            "Stage 4AO-C no-data inner endpoint requires x_in<r0");
    }
    const auto jet = make_inner_endpoint_derivative_jet(
        radial_amplitudes, grid.dx(), wavenumber, sector);
    return InnerEndpointApplyResult(
        apply_complete_interior_from_jet(grid.domain().r0(),
                                         grid.domain().x_in(), jet),
        outflow, sector);
}

inline double metric_determinant_tangent_residual(const Vector &input)
{
    return input.value(Variable::h_xx) + input.value(Variable::h_zz) +
           2.0 * input.value(Variable::h_ww);
}

inline Vector project_inner_endpoint_algebraic_constraints(
    const double r0, const double x, const Vector &input)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] = input.value(variable);
    }
    const double determinant_error = metric_determinant_tangent_residual(input);
    constexpr double one_over_d = 0.25;
    values[Operator::variable_index(Variable::h_xx)] -=
        one_over_d * determinant_error;
    values[Operator::variable_index(Variable::h_zz)] -=
        one_over_d * determinant_error;
    values[Operator::variable_index(Variable::h_ww)] -=
        one_over_d * determinant_error;
    const auto metric_projected =
        Operator::make_frozen_gauge_perturbation_vector(values);
    return Operator::project_delta_a_trace_free_at_point(r0, x,
                                                         metric_projected);
}

} // namespace Stage4AOFrozenGaugeInnerBoundary
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_INNER_BOUNDARY_HPP */
