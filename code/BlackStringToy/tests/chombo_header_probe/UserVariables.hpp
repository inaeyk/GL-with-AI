#ifndef BLACKSTRING_CHOMBO_HEADER_PROBE_USER_VARIABLES_HPP
#define BLACKSTRING_CHOMBO_HEADER_PROBE_USER_VARIABLES_HPP

#include "BlackStringProductionVariables.hpp"

// Probe-only application boundary required by GRChombo BoxPointers/Cell.
// The production enum remains namespaced and single-sourced in
// BlackStringProductionVariables.hpp.
inline constexpr int NUM_VARS =
    BlackStringProductionVariables::NUM_VARS;

#endif
