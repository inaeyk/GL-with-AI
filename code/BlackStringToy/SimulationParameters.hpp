#ifndef SIMULATIONPARAMETERS_HPP_
#define SIMULATIONPARAMETERS_HPP_

#include "BlackStringAlgebraicReconstruction.hpp"
#include "BlackStringLive.hpp"
#include "BlackStringGammaZHelper.hpp"
#include "BlackStringPerturbativeRadialBoundary.hpp"
#include "ChomboParameters.hpp"
#include "GRParmParse.hpp"
#include "MayDay.H"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cmath>
#include <limits>

class SimulationParameters : public ChomboParameters
{
  public:
    explicit SimulationParameters(GRParmParse &pp) : ChomboParameters(pp)
    {
        pp.load("r_0", r0);
        pp.load("black_string_coordinate_minimum", coordinate_minimum);
        pp.load("fixed_lapse_source", fixed_lapse_source, true);
        pp.load("ko_sigma", ko_sigma);
        pp.load("constraint_diagnostic_cadence",
                constraint_diagnostic_cadence, 0);
        pp.load("background_preserving_gp_radial_ghosts",
                background_preserving_gp_radial_ghosts, false);
        pp.load("physical_radial_boundaries", physical_radial_boundaries,
                false);
        pp.load("constraint_corrected_fourier_seed",
                constraint_corrected_fourier_seed, false);
        pp.load("fourier_seed_mode_number", fourier_seed_mode_number, 1);
        pp.load("fourier_seed_amplitude", fourier_seed_amplitude, 0.0);
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
            run_m2b_prelaunch_gates();
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
    // Project-owned grid-direction KO coefficient. This is deliberately
    // separate from locked SimulationParametersBase::sigma because the live
    // reduced-state path cannot use its target-wide grid-stride wrapper.
    double ko_sigma = 0.0;
    // 0 disables the constraint loop; positive values run every N steps.
    int constraint_diagnostic_cadence = 0;
    // Diagnostic-only exact GP radial ghost data. This is not an accepted
    // physical boundary condition for perturbations.
    bool background_preserving_gp_radial_ghosts = false;
    // M2-B reduced-characteristic physical radial boundary.
    bool physical_radial_boundaries = false;
    bool constraint_corrected_fourier_seed = false;
    int fourier_seed_mode_number = 1;
    double fourier_seed_amplitude = 0.0;
    // Retained for input compatibility with M1 parameter files. The M2-B
    // characteristic boundary does not use a componentwise Sommerfeld speed.
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
        if (!std::isfinite(ko_sigma) || ko_sigma < 0.0)
        {
            MayDay::Error("ko_sigma must be finite and nonnegative");
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
        if (constraint_corrected_fourier_seed)
        {
            const int compact_cells =
                ivN[BlackStringLive::compact_direction] + 1;
            if (!physical_radial_boundaries ||
                !std::isfinite(fourier_seed_amplitude) ||
                fourier_seed_amplitude == 0.0 ||
                fourier_seed_mode_number <= 0 ||
                2 * fourier_seed_mode_number >= compact_cells)
            {
                MayDay::Error(
                    "M2-B corrected seed requires physical boundaries, a "
                    "nonzero finite amplitude, and a nonzero non-Nyquist mode");
            }
        }
        if (physical_radial_boundaries)
        {
            const double outer_face =
                coordinate_minimum[BlackStringLive::radial_direction] +
                static_cast<double>(ivN[BlackStringLive::radial_direction] +
                                    1) *
                    coarsest_dx;
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
            const auto close = [](const double left, const double right) {
                return std::abs(left - right) <=
                       64.0 * std::numeric_limits<double>::epsilon() *
                           std::max({1.0, std::abs(left), std::abs(right)});
            };
            if (!close(coordinate_minimum[0] / r0, 0.5) ||
                !close(outer_face / r0, 4.5))
            {
                MayDay::Error("M2-B characteristic boundary requires the "
                              "locked x/r_0 domain [0.5,4.5]");
            }
            if (!fixed_lapse_source || !close(gauge.lapse_advec_coeff, 0.0) ||
                !close(gauge.lapse_power, 1.0) ||
                !close(gauge.lapse_coeff, 2.0) ||
                !close(gauge.shift_Gamma_coeff, 0.75) ||
                !close(gauge.shift_advec_coeff, 0.0) ||
                !close(gauge.eta, 1.0))
            {
                MayDay::Error(
                    "M2-B characteristic boundary requires the locked gauge");
            }
        }
    }

    void run_m2b_prelaunch_gates() const
    {
        const double inner_face = coordinate_minimum[0];
        const double outer_face =
            inner_face + static_cast<double>(ivN[0] + 1) * coarsest_dx;
        BlackStringAlgebraicReconstruction::validate_locked_gate(r0);
        BlackStringGammaZHelper::validate_locked_gate(r0);
        BlackStringPerturbativeRadialBoundary::validate_locked_gate(
            r0, coarsest_dx, inner_face, outer_face);
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
        // EXTRAPOLATING grows the stock boundary layout. The project adapter
        // replaces its constant data with the reduced-characteristic fill.
        high[BlackStringLive::radial_direction] =
            BoundaryConditions::EXTRAPOLATING_BC;
        configured.set_hi_boundary(high);
        // The project-owned characteristic fill overwrites all three physical
        // radial ghost layers after exchange. Constant stock extrapolation is
        // only layout infrastructure and never owns physical data.
        configured.extrapolation_order = 0;
        configured.mixed_bc_vars_map.clear();
        boundary_params = configured;
    }
};

#endif /* SIMULATIONPARAMETERS_HPP_ */
