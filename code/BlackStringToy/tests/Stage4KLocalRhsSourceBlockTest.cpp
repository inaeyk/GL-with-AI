#include "CartoonRhsSourceBlock.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
using namespace BlackStringToy;

constexpr double tol = 1.0e-12;

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected)
{
    const double residual = std::abs(value - expected);
    if (residual > tol)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected) + ", residual " +
                        std::to_string(residual));
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

void require_true(const std::string &label, const bool value)
{
    if (!value)
    {
        fail(label, "expected true");
    }
    std::cout << "PASS " << label << " = true\n";
}

void require_finite(const std::string &label, const double value)
{
    if (!std::isfinite(value))
    {
        fail(label, "value is not finite");
    }
    std::cout << "PASS " << label << " is finite\n";
}

void check_empty_source_block_contract()
{
    const CartoonRicci::RicciComponents raw_ricci(2.0, 3.0, 5.0, 7.0);

    // Raw cartoon Ricci is intentionally not accepted by the source block:
    // CartoonRhsSourceBlock::LocalSourceBlockInput bad{raw_ricci};
    // would not compile. Cross through the Stage 4I bridge and Stage 4J
    // contract first.
    const auto bridge_ricci =
        CartoonRicciBridge::to_rhs_ricci_components(raw_ricci);

    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{11.0, 13.0,
                                                               17.0, 19.0};
    const CartoonRhsContract::RhsLocalInputs contract_input{
        {bridge_ricci, h_UU, 0.5}, 2.0};
    const CartoonRhsSourceBlock::LocalSourceBlockInput source_input{
        contract_input, {1.0, 0.0, 1.0, 1.0}};

    const auto output =
        CartoonRhsSourceBlock::make_empty_source_block_for_contract_test(
            source_input);

    require_true("explicit inert source-block test output",
                 output.inert_test_output);
    require_close("inert rhs_chi", output.rhs_chi, 0.0);
    require_close("inert rhs_hww", output.rhs_hww, 0.0);
    require_close("inert rhs_Aww", output.rhs_Aww, 0.0);
    require_close("inert rhs_K", output.rhs_K, 0.0);
    require_finite("rhs_chi", output.rhs_chi);
    require_finite("rhs_hww", output.rhs_hww);
    require_finite("rhs_Aww", output.rhs_Aww);
    require_finite("rhs_K", output.rhs_K);

    require_close("source block carries full 4D Ricci trace",
                  output.ricci_contractions.conformal_inverse_contract,
                  451.0);
    require_close("source block carries physical Ricci scalar",
                  output.ricci_contractions.physical_scalar, 225.5);
}

} // namespace

int main()
{
    std::cout << "Stage 4K BlackStringToy local RHS source-block skeleton "
                 "fixture\n";
    std::cout << "Skeleton/container only; no real RHS formulas, grid data, "
                 "or evolution wiring are used.\n";

    require_false("source terms are implemented",
                  CartoonRhsSourceBlock::source_terms_implemented);
    require_false("evolution wiring is implemented",
                  CartoonRhsSourceBlock::evolution_wiring_implemented);
    check_empty_source_block_contract();

    std::cout << "All Stage 4K local RHS source-block skeleton checks "
                 "passed.\n";
    return 0;
}
