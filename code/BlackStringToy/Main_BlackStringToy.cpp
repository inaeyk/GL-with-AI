#include "CH_Timer.H"
#include "DefaultLevelFactory.hpp"
#include "GRAMR.hpp"
#include "GRParmParse.hpp"
#include "SetupFunctions.hpp"
#include "SimulationParameters.hpp"
#include "parstream.H"

#include "BlackStringToyLevel.hpp"

#include <iostream>

int runGRChombo(int argc, char *argv[])
{
    GRParmParse pp(argc - 2, argv + 2, nullptr, argv[1]);
    SimulationParameters sim_params(pp);
    if (sim_params.just_check_params)
    {
        return 0;
    }

    GRAMR black_string_amr;
    DefaultLevelFactory<BlackStringToyLevel> level_factory(
        black_string_amr, sim_params);
    setupAMRObject(black_string_amr, level_factory);

    // Stage 4AO-D-E1 wires the real application hooks but does not qualify a
    // sustained run. The first unperturbed run is the separate 4AO-D-F gate.
    black_string_amr.run(sim_params.stop_time, sim_params.max_steps);
    black_string_amr.conclude();
    CH_TIMER_REPORT();
    return 0;
}

int main(int argc, char *argv[])
{
    mainSetup(argc, argv);
    const int status = runGRChombo(argc, argv);
    mainFinalize();
    if (status == 0)
    {
        pout() << "GRChombo finished." << std::endl;
    }
    return status;
}
