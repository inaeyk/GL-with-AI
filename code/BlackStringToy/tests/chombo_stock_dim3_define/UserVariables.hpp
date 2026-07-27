#ifndef USERVARIABLES_HPP
#define USERVARIABLES_HPP

#include "DiagnosticVariables.hpp"

#include <array>
#include <string>

enum
{
    c_stock_a,
    c_stock_b,
    NUM_VARS
};

namespace UserVariables
{
inline const std::array<std::string, NUM_VARS> variable_names = {"stock_a",
                                                                 "stock_b"};
}

#include "UserVariables.inc.hpp"

#endif /* USERVARIABLES_HPP */
