#ifndef DIAGNOSTICVARIABLES_HPP
#define DIAGNOSTICVARIABLES_HPP

#include <array>
#include <string>

enum
{
    c_stock_diagnostic,
    NUM_DIAGNOSTIC_VARS
};

namespace DiagnosticVariables
{
inline const std::array<std::string, NUM_DIAGNOSTIC_VARS> variable_names = {
    "stock_diagnostic"};
}

#endif /* DIAGNOSTICVARIABLES_HPP */
