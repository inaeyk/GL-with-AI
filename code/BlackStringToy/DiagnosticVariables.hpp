/* GRChombo
 * Copyright 2012 The GRChombo collaboration.
 * Please refer to LICENSE in GRChombo's root directory.
 */

#ifndef DIAGNOSTICVARIABLES_HPP
#define DIAGNOSTICVARIABLES_HPP

enum
{
    c_Ham,
    c_MomX,
    c_MomZ,
    NUM_DIAGNOSTIC_VARS
};

namespace DiagnosticVariables
{
static const std::array<std::string, NUM_DIAGNOSTIC_VARS> variable_names = {
    "Ham", "MomX", "MomZ"};
}

static_assert(NUM_DIAGNOSTIC_VARS == 3);

#endif /* DIAGNOSTICVARIABLES_HPP */
