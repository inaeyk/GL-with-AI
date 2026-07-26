#ifndef USERVARIABLES_HPP
#define USERVARIABLES_HPP

#include "BlackStringProductionVariables.hpp"
#include "DiagnosticVariables.hpp"

// GRChombo requires these names in the global namespace. Their values and
// registration strings are aliases of the single production contract; this is
// not a second enum and cannot drift back to the legacy 27-slot layout.
inline constexpr int c_chi = BlackStringProductionVariables::c_chi;
inline constexpr int c_hxx = BlackStringProductionVariables::c_hxx;
inline constexpr int c_hxz = BlackStringProductionVariables::c_hxz;
inline constexpr int c_hzz = BlackStringProductionVariables::c_hzz;
inline constexpr int c_hww = BlackStringProductionVariables::c_hww;
inline constexpr int c_K = BlackStringProductionVariables::c_K;
inline constexpr int c_Axx = BlackStringProductionVariables::c_Axx;
inline constexpr int c_Axz = BlackStringProductionVariables::c_Axz;
inline constexpr int c_Azz = BlackStringProductionVariables::c_Azz;
inline constexpr int c_Aww = BlackStringProductionVariables::c_Aww;
inline constexpr int c_Theta = BlackStringProductionVariables::c_Theta;
inline constexpr int c_GammaX = BlackStringProductionVariables::c_GammaX;
inline constexpr int c_GammaZ = BlackStringProductionVariables::c_GammaZ;
inline constexpr int c_lapse = BlackStringProductionVariables::c_lapse;
inline constexpr int c_shiftX = BlackStringProductionVariables::c_shiftX;
inline constexpr int c_shiftZ = BlackStringProductionVariables::c_shiftZ;
inline constexpr int c_Bx = BlackStringProductionVariables::c_Bx;
inline constexpr int c_Bz = BlackStringProductionVariables::c_Bz;
inline constexpr int NUM_VARS = BlackStringProductionVariables::NUM_VARS;

namespace UserVariables
{
inline const auto &variable_names =
    BlackStringProductionVariables::UserVariables::registration_names;
}

static_assert(NUM_VARS == 18);
static_assert(c_hww == 4 && c_Aww == 9);

#ifndef BLACKSTRINGTOY_LAYOUT_TEST_ONLY
#include "UserVariables.inc.hpp"
#endif

#endif /* USERVARIABLES_HPP */
