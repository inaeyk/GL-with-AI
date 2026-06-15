/* GRChombo
 * Copyright 2012 The GRChombo collaboration.
 * Please refer to LICENSE in GRChombo's root directory.
 */

#ifndef USERVARIABLES_HPP
#define USERVARIABLES_HPP

#include <array>
#include <string>

#include "ArrayTools.hpp"
#include "DiagnosticVariables.hpp"

/// This enum gives the index of every variable stored in the grid
enum
{
    c_chi,

    c_h11,
    c_h12,
    c_h13,
    c_h22,
    c_h23,
    c_h33,
    c_hww,

    c_K,

    c_A11,
    c_A12,
    c_A13,
    c_A22,
    c_A23,
    c_A33,
    c_Aww,

    c_Theta,

    c_Gamma1,
    c_Gamma2,
    c_Gamma3,

    c_lapse,

    c_shift1,
    c_shift2,
    c_shift3,

    c_B1,
    c_B2,
    c_B3,

    NUM_VARS
};

static_assert(c_hww == c_K - 1, "hww must be immediately before K");
static_assert(c_Aww == c_Theta - 1,
              "Aww must be immediately before Theta");
static_assert(NUM_VARS == c_B3 + 1,
              "NUM_VARS must terminate the BlackStringToy enum");

namespace UserVariables
{
static const std::array<std::string, NUM_VARS> variable_names = {
    "chi",

    "h11",    "h12",    "h13",    "h22", "h23", "h33", "hww",

    "K",

    "A11",    "A12",    "A13",    "A22", "A23", "A33", "Aww",

    "Theta",

    "Gamma1", "Gamma2", "Gamma3",

    "lapse",

    "shift1", "shift2", "shift3",

    "B1",     "B2",     "B3"};
} // namespace UserVariables

#ifndef BLACKSTRINGTOY_LAYOUT_TEST_ONLY
#include "UserVariables.inc.hpp"
#endif

#endif /* USERVARIABLES_HPP */
