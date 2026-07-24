#ifndef BLACKSTRING_CHOMBO_GP_BOXLOOP_USER_VARIABLES_HPP
#define BLACKSTRING_CHOMBO_GP_BOXLOOP_USER_VARIABLES_HPP

#include "BlackStringProductionVariables.hpp"

// Test-only GRChombo application boundary. The production 18-slot enum is the
// sole variable authority.
inline constexpr int NUM_VARS =
    BlackStringProductionVariables::NUM_VARS;

#endif
