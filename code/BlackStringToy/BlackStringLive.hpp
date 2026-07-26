#ifndef BLACKSTRINGLIVE_HPP
#define BLACKSTRINGLIVE_HPP

// Load the real grid and derivative infrastructure before the pointwise
// adapter suppresses unrelated stock enum-mapping wrappers.
#include "FourthOrderDerivatives.hpp"

#include "BlackStringGPInitialData.hpp"
#include "BlackStringTargetCleanupConstraintsSource.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

namespace BlackStringLive
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace Storage = BlackStringCellStorage;
namespace Target = BlackStringTargetCCZ4Pointwise;
namespace TargetOps = BlackStringTargetCleanupConstraintsSource;

inline constexpr int radial_direction = 0;
inline constexpr int compact_direction = 1;
inline constexpr int required_ghost_width = 2;
inline constexpr double positivity_floor = 1.0e-4;

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(Production::NUM_VARS == 18);
static_assert(Production::c_hww == 4 && Production::c_Aww == 9);
static_assert(Production::metadata[Production::c_hww].storage_multiplicity ==
              1);
static_assert(Production::metadata[Production::c_Aww].storage_multiplicity ==
              1);

struct GaugeParameters
{
    double lapse_advec_coeff = 0.0;
    double lapse_power = 1.0;
    double lapse_coeff = 2.0;
    double shift_Gamma_coeff = 0.75;
    double shift_advec_coeff = 0.0;
    double eta = 1.0;
};

inline MovingPunctureGauge make_gauge(const GaugeParameters &parameters)
{
    MovingPunctureGauge::params_t grchombo{};
    grchombo.lapse_advec_coeff = parameters.lapse_advec_coeff;
    grchombo.lapse_power = parameters.lapse_power;
    grchombo.lapse_coeff = parameters.lapse_coeff;
    grchombo.shift_Gamma_coeff = parameters.shift_Gamma_coeff;
    grchombo.shift_advec_coeff = parameters.shift_advec_coeff;
    grchombo.eta = parameters.eta;
    return MovingPunctureGauge(grchombo);
}

inline std::array<double, CH_SPACEDIM>
coordinate_offset_from_minimum(
    const std::array<double, CH_SPACEDIM> &coordinate_minimum)
{
    return {-coordinate_minimum[radial_direction],
            -coordinate_minimum[compact_direction]};
}

inline Target::Input make_pointwise_input(const Cell<double> &cell,
                                          const double dx,
                                          const std::array<double,
                                                           CH_SPACEDIM>
                                              &coordinate_offset)
{
    Storage::require_input_component_count(cell);
    const auto coordinates = BlackStringGPInitialData::cell_centered_coordinates(
        cell.get_int_vect(), dx, coordinate_offset);
    Target::Input input;
    input.vars = Storage::load(cell);
    input.x = coordinates.x;

    const FourthOrderDerivatives derivatives(dx);
    const auto &pointers = cell.get_box_pointers();
    const int index = cell.get_in_index();
    const int x_stride = pointers.m_in_stride[radial_direction];
    const int z_stride = pointers.m_in_stride[compact_direction];
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        const double *values = pointers.m_in_ptr[slot];
        Target::Jet &jet = input.derivatives[static_cast<std::size_t>(slot)];
        jet.dx = derivatives.diff1<double>(values, index, x_stride);
        jet.dz = derivatives.diff1<double>(values, index, z_stride);
        jet.dxx = derivatives.diff2<double>(values, index, x_stride);
        jet.dxz = derivatives.mixed_diff2<double>(
            values, index, x_stride, z_stride);
        jet.dzz = derivatives.diff2<double>(values, index, z_stride);
    }
    return input;
}

inline Reduced::Variables<double>
evaluate_rhs(const Target::Input &input, const double r0,
             const GaugeParameters &gauge_parameters,
             const bool fixed_lapse_source_enabled)
{
    const Target::ExpandedInput expanded = Target::expand_target(input);
    Target::TargetVars<double> target_rhs =
        Target::evaluate_direct(expanded, input.parameters);
    make_gauge(gauge_parameters)
        .rhs_gauge(target_rhs, expanded.vars, expanded.d1, expanded.d2,
                   expanded.advection);
    if (fixed_lapse_source_enabled)
    {
        const double source =
            TargetOps::DefaultSourcePolicy::value(r0, input.x, input.vars);
        TargetOps::DefaultSourcePolicy::apply(target_rhs, source);
    }

    Reduced::Variables<double> rhs{};
    rhs.physical.chi = target_rhs.chi;
    rhs.physical.h = {target_rhs.h[0][0], target_rhs.h[0][1],
                      target_rhs.h[1][1], target_rhs.h[2][2]};
    rhs.physical.K = target_rhs.K;
    rhs.physical.A = {target_rhs.A[0][0], target_rhs.A[0][1],
                      target_rhs.A[1][1], target_rhs.A[2][2]};
    rhs.physical.Theta = target_rhs.Theta;
    rhs.physical.Gamma = {target_rhs.Gamma[0], target_rhs.Gamma[1]};
    rhs.gauge.lapse = target_rhs.lapse;
    rhs.gauge.shift = {target_rhs.shift[0], target_rhs.shift[1]};
    rhs.gauge.B = {target_rhs.B[0], target_rhs.B[1]};
    return rhs;
}

class RHSCompute
{
  public:
    RHSCompute(const double r0, const double dx,
               const std::array<double, CH_SPACEDIM> &coordinate_offset,
               const GaugeParameters &gauge_parameters,
               const bool fixed_lapse_source_enabled)
        : m_r0(r0), m_dx(dx), m_coordinate_offset(coordinate_offset),
          m_gauge_parameters(gauge_parameters),
          m_fixed_lapse_source_enabled(fixed_lapse_source_enabled)
    {
        if (!std::isfinite(m_r0) || !(m_r0 > 0.0))
        {
            throw std::domain_error("live black-string RHS requires r0>0");
        }
    }

    void compute(const Cell<double> cell) const
    {
        Storage::store(
            cell, evaluate_rhs(
                      make_pointwise_input(cell, m_dx, m_coordinate_offset),
                      m_r0, m_gauge_parameters,
                      m_fixed_lapse_source_enabled));
    }

  private:
    double m_r0;
    double m_dx;
    std::array<double, CH_SPACEDIM> m_coordinate_offset;
    GaugeParameters m_gauge_parameters;
    bool m_fixed_lapse_source_enabled;
};

class CleanupCompute
{
  public:
    CleanupCompute(const double min_chi = positivity_floor,
                   const double min_lapse = positivity_floor)
        : m_min_chi(min_chi), m_min_lapse(min_lapse)
    {
    }

    void compute(const Cell<double> cell) const
    {
        auto cleaned = TargetOps::cleanup(Storage::load(cell)).cleaned;
        cleaned.physical.chi =
            std::max(cleaned.physical.chi, m_min_chi);
        cleaned.gauge.lapse =
            std::max(cleaned.gauge.lapse, m_min_lapse);
        Storage::store(cell, cleaned);
    }

  private:
    double m_min_chi;
    double m_min_lapse;
};

class ConstraintCompute
{
  public:
    ConstraintCompute(
        const double dx,
        const std::array<double, CH_SPACEDIM> &coordinate_offset)
        : m_dx(dx), m_coordinate_offset(coordinate_offset)
    {
    }

    void compute(const Cell<double> cell) const
    {
        if (cell.get_num_out_vars() !=
            static_cast<int>(TargetOps::constraint_outputs))
        {
            throw std::invalid_argument(
                "black-string diagnostics require exactly three outputs");
        }
        const auto constraints = TargetOps::evaluate_constraints(
                                     make_pointwise_input(
                                         cell, m_dx, m_coordinate_offset))
                                     .target_total;
        cell.store_vars(constraints.hamiltonian, 0);
        cell.store_vars(constraints.momentum[0], 1);
        cell.store_vars(constraints.momentum[1], 2);
    }

  private:
    double m_dx;
    std::array<double, CH_SPACEDIM> m_coordinate_offset;
};

} // namespace BlackStringLive

#endif /* BLACKSTRINGLIVE_HPP */
