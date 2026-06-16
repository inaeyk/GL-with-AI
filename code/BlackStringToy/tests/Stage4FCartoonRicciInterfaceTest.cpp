#include "CartoonRicciInterface.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
using namespace BlackStringToy::CartoonRicci;

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_equal(const std::string &label, const int value,
                   const int expected)
{
    if (value != expected)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void require_false(const std::string &label, const bool value)
{
    if (value)
    {
        fail(label, "expected false");
    }
    std::cout << "PASS " << label << " = false\n";
}

void check_interface_types()
{
    require_equal("hidden multiplicity", hidden_multiplicity, 2);
    require_false("Ricci formulas are not implemented",
                  ricci_formulas_implemented);

    CartoonRicciInputs inputs{};
    inputs.x = 1.25;
    inputs.chi = 0.8;
    inputs.h = {2.0, 0.1, 3.0, 1.4};
    inputs.chi_derivatives = {{0.01, 0.02}, {0.03, 0.04, 0.05}};
    inputs.h_derivatives.dx = {0.11, 0.12, 0.13, 0.14};
    inputs.h_derivatives.dz = {0.21, 0.22, 0.23, 0.24};
    inputs.h_derivatives.dxx = {0.31, 0.32, 0.33, 0.34};
    inputs.h_derivatives.dxz = {0.41, 0.42, 0.43, 0.44};
    inputs.h_derivatives.dzz = {0.51, 0.52, 0.53, 0.54};

    RicciComponents output_shape{0.0, 0.0, 0.0, 0.0};

    (void)inputs;
    (void)output_shape;

    std::cout << "PASS local-value input and output structures instantiate\n";
}

} // namespace

int main()
{
    std::cout << "Stage 4F BlackStringToy cartoon Ricci interface fixture\n";
    std::cout << "Interface/type check only; no Ricci formulas, grid data, "
                 "Cell, Vars, FArrayBox, or evolution data are used.\n";

    check_interface_types();

    std::cout << "All Stage 4F cartoon Ricci interface checks passed.\n";
    return 0;
}
