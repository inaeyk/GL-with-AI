#ifndef SIMULATIONPARAMETERS_HPP_
#define SIMULATIONPARAMETERS_HPP_

#include "ChomboParameters.hpp"
#include "GRParmParse.hpp"

class SimulationParameters : public ChomboParameters
{
  public:
    explicit SimulationParameters(GRParmParse &pp) : ChomboParameters(pp) {}
};

#endif /* SIMULATIONPARAMETERS_HPP_ */
