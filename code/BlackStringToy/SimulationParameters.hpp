#ifndef SIMULATIONPARAMETERS_HPP_
#define SIMULATIONPARAMETERS_HPP_

#include "BlackStringLive.hpp"
#include "ChomboParameters.hpp"
#include "GRParmParse.hpp"
#include "MayDay.H"

#include <array>
#include <cmath>

class SimulationParameters : public ChomboParameters
{
  public:
    explicit SimulationParameters(GRParmParse &pp) : ChomboParameters(pp)
    {
        pp.load("r_0", r0);
        pp.load("black_string_coordinate_minimum", coordinate_minimum);
        pp.load("fixed_lapse_source", fixed_lapse_source, true);
        pp.load("calculate_constraints", calculate_constraints, true);
        pp.load("min_chi", min_chi, BlackStringLive::positivity_floor);
        pp.load("min_lapse", min_lapse, BlackStringLive::positivity_floor);

        pp.load("lapse_advec_coeff", gauge.lapse_advec_coeff, 0.0);
        pp.load("lapse_power", gauge.lapse_power, 1.0);
        pp.load("lapse_coeff", gauge.lapse_coeff, 2.0);
        pp.load("shift_Gamma_coeff", gauge.shift_Gamma_coeff, 0.75);
        pp.load("shift_advec_coeff", gauge.shift_advec_coeff, 0.0);
        pp.load("eta", gauge.eta, 1.0);
        check_black_string_params();
    }

    std::array<double, CH_SPACEDIM> coordinate_offset() const
    {
        return BlackStringLive::coordinate_offset_from_minimum(
            coordinate_minimum);
    }

    double r0 = 0.0;
    std::array<double, CH_SPACEDIM> coordinate_minimum{};
    bool fixed_lapse_source = true;
    bool calculate_constraints = true;
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
    }
};

#endif /* SIMULATIONPARAMETERS_HPP_ */
