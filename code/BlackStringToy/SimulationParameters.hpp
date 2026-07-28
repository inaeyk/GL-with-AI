#ifndef SIMULATIONPARAMETERS_HPP_
#define SIMULATIONPARAMETERS_HPP_

#include "BlackStringLive.hpp"
#include "ChomboParameters.hpp"
#include "GRParmParse.hpp"
#include "MayDay.H"

#include <array>
#include <cstddef>
#include <cmath>

class SimulationParameters : public ChomboParameters
{
  public:
    explicit SimulationParameters(GRParmParse &pp) : ChomboParameters(pp)
    {
        pp.load("r_0", r0);
        pp.load("black_string_coordinate_minimum", coordinate_minimum);
        pp.load("fixed_lapse_source", fixed_lapse_source, true);
        pp.load("constraint_diagnostic_cadence",
                constraint_diagnostic_cadence, 0);
        pp.load("background_preserving_gp_radial_ghosts",
                background_preserving_gp_radial_ghosts, false);
        pp.load("physical_radial_boundaries", physical_radial_boundaries,
                false);
        pp.load("outer_sommerfeld_speed", outer_sommerfeld_speed, 1.0);
        pp.load("min_chi", min_chi, BlackStringLive::positivity_floor);
        pp.load("min_lapse", min_lapse, BlackStringLive::positivity_floor);

        pp.load("lapse_advec_coeff", gauge.lapse_advec_coeff, 0.0);
        pp.load("lapse_power", gauge.lapse_power, 1.0);
        pp.load("lapse_coeff", gauge.lapse_coeff, 2.0);
        pp.load("shift_Gamma_coeff", gauge.shift_Gamma_coeff, 0.75);
        pp.load("shift_advec_coeff", gauge.shift_advec_coeff, 0.0);
        pp.load("eta", gauge.eta, 1.0);
        check_black_string_params();
        if (physical_radial_boundaries)
        {
            configure_physical_radial_boundary_infrastructure();
        }
    }

    std::array<double, CH_SPACEDIM> coordinate_offset() const
    {
        return BlackStringLive::coordinate_offset_from_minimum(
            coordinate_minimum);
    }

    bool constraint_diagnostic_due(const std::size_t completed_steps) const
    {
        return constraint_diagnostic_cadence > 0 && completed_steps > 0 &&
               completed_steps %
                       static_cast<std::size_t>(
                           constraint_diagnostic_cadence) ==
                   0;
    }

    double r0 = 0.0;
    std::array<double, CH_SPACEDIM> coordinate_minimum{};
    bool fixed_lapse_source = true;
    // 0 disables the constraint loop; positive values run every N steps.
    int constraint_diagnostic_cadence = 0;
    // Diagnostic-only exact GP radial ghost data. This is not an accepted
    // physical boundary condition for perturbations.
    bool background_preserving_gp_radial_ghosts = false;
    // Provisional excision/outflow plus GP-subtracted outer Sommerfeld policy.
    bool physical_radial_boundaries = false;
    double outer_sommerfeld_speed = 1.0;
    double min_chi = BlackStringLive::positivity_floor;
    double min_lapse = BlackStringLive::positivity_floor;
    BlackStringLive::GaugeParameters gauge{};

  private:
    void check_black_string_params() const
    {
        if (!std::isfinite(r0) || !(r0 > 0.0))
        {
            MayDay::Error("BlackStringToy requires finite r_0 > 0");
        }
        if (!std::isfinite(coordinate_minimum[0]) ||
            !std::isfinite(coordinate_minimum[1]) ||
            !(coordinate_minimum[0] > 0.0))
        {
            MayDay::Error("BlackStringToy requires finite "
                          "black_string_coordinate_minimum with x_min > 0");
        }
        if (max_spatial_derivative_order != 4)
        {
            MayDay::Error(
                "Stage 4AO-D-E1 supports only fourth-order derivatives");
        }
        if (max_level != 0)
        {
            MayDay::Error("Stage 4AO-D-E1 is not AMR-qualified; max_level "
                          "must remain zero");
        }
        if (boundary_params.is_periodic[BlackStringLive::radial_direction] ||
            !boundary_params.is_periodic[BlackStringLive::compact_direction])
        {
            MayDay::Error("BlackStringToy requires isPeriodic = 0 1 "
                          "(radial x nonperiodic, compact z periodic)");
        }
        if (min_chi < 0.0 || min_lapse < 0.0)
        {
            MayDay::Error("BlackStringToy positivity floors must be nonnegative");
        }
        if (constraint_diagnostic_cadence < 0)
        {
            MayDay::Error(
                "constraint_diagnostic_cadence must be zero or positive");
        }
        if (background_preserving_gp_radial_ghosts &&
            physical_radial_boundaries)
        {
            MayDay::Error("exact-GP diagnostic and physical radial boundary "
                          "policies are mutually exclusive");
        }
        if (physical_radial_boundaries)
        {
            if (!(coordinate_minimum[BlackStringLive::radial_direction] < r0))
            {
                MayDay::Error(
                    "physical inner radial boundary requires x_in < r_0");
            }
            const double innermost_ghost_x =
                coordinate_minimum[BlackStringLive::radial_direction] -
                (static_cast<double>(num_ghosts) - 0.5) * coarsest_dx;
            if (!(innermost_ghost_x > 0.0))
            {
                MayDay::Error("physical radial boundary requires every inner "
                              "ghost to remain at x > 0");
            }
            if (num_ghosts != 3)
            {
                MayDay::Error("physical radial boundary requires exactly "
                              "three ghost cells");
            }
            if (!std::isfinite(outer_sommerfeld_speed) ||
                !(outer_sommerfeld_speed > 0.0))
            {
                MayDay::Error(
                    "outer_sommerfeld_speed must be finite and positive");
            }
        }
    }

    void configure_physical_radial_boundary_infrastructure()
    {
        const auto periodic = boundary_params.is_periodic;
        BoundaryConditions::params_t configured;
        configured.set_is_periodic(periodic);

        auto low = configured.lo_boundary;
        low[BlackStringLive::radial_direction] =
            BoundaryConditions::EXTRAPOLATING_BC;
        configured.set_lo_boundary(low);

        auto high = configured.hi_boundary;
        // MIXED grows the stock boundary layout. All reduced components are
        // assigned to its extrapolating branch because the black-string
        // adapter supplies the physical GP-subtracted Sommerfeld surface RHS.
        high[BlackStringLive::radial_direction] =
            BoundaryConditions::MIXED_BC;
        configured.set_hi_boundary(high);
        // These RHS ghosts are overwritten by the project-owned solution
        // closure after each RK update; constant extension is sufficient and
        // avoids using stock Euclidean-radius slope fitting.
        configured.extrapolation_order = 0;
        configured.mixed_bc_vars_map.clear();
        for (int component = 0; component < NUM_VARS; ++component)
        {
            configured.mixed_bc_vars_map.emplace(
                component, BoundaryConditions::EXTRAPOLATING_BC);
        }
        boundary_params = configured;
    }
};

#endif /* SIMULATIONPARAMETERS_HPP_ */
