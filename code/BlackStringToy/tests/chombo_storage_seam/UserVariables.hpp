#ifndef BLACKSTRING_CHOMBO_STORAGE_SEAM_USER_VARIABLES_HPP
#define BLACKSTRING_CHOMBO_STORAGE_SEAM_USER_VARIABLES_HPP

#include "BlackStringProductionVariables.hpp"

// Test-only application boundary required by GRChombo BoxPointers/Cell.
// The production enum remains the only slot authority.
inline constexpr int NUM_VARS =
    BlackStringProductionVariables::NUM_VARS;

#endif
