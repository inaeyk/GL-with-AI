#include "CartoonRhsContract.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
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

void require_throw_bad_axis()
{
    const auto bridge_ricci = CartoonRicciBridge::to_rhs_ricci_components(
        CartoonRicci::RicciComponents(2.0, 3.0, 5.0, 7.0));

    const CartoonRhsContract::RhsLocalInputs inputs{
        {bridge_ricci, {11.0, 13.0, 17.0, 19.0}, 0.5}, 0.0};

    try
    {
        (void)CartoonRhsContract::make_inert_rhs_contract_outputs(inputs);
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS RHS contract away-axis guard rejects x = 0\n";
        return;
    }

    fail("RHS contract away-axis guard", "x = 0 was accepted");
}

void check_contract_trace()
{
    const CartoonRicci::RicciComponents raw_ricci(2.0, 3.0, 5.0, 7.0);

    // Raw cartoon Ricci is intentionally not accepted by the RHS contract:
    // CartoonRhsContract::RhsRicciInputs bad{raw_ricci, h_UU, chi};
    // would not compile. Future RHS-facing use must cross the Stage 4I bridge.
    const auto bridge_ricci =
        CartoonRicciBridge::to_rhs_ricci_components(raw_ricci);

    require_close("bridge-approved R_xx", bridge_ricci.xx, 2.0);
    require_close("bridge-approved R_xz", bridge_ricci.xz, 3.0);
    require_close("bridge-approved R_zz", bridge_ricci.zz, 5.0);
    require_close("bridge-approved R_ww", bridge_ricci.ww, 7.0);

    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{11.0, 13.0,
                                                               17.0, 19.0};
    const double chi = 0.5;
    const CartoonRhsContract::RhsLocalInputs inputs{
        {bridge_ricci, h_UU, chi}, 2.0};

    const auto outputs =
        CartoonRhsContract::make_inert_rhs_contract_outputs(inputs);

    // Hard-coded oracle:
    // h^{xx} R_xx + 2 h^{xz} R_xz + h^{zz} R_zz + 2 h^{ww} R_ww
    // = 11*2 + 2*13*3 + 17*5 + 2*19*7 = 451.
    require_close("RHS contract full 4D Ricci trace",
                  outputs.ricci_contractions.conformal_inverse_contract,
                  451.0);
    require_close("RHS contract physical Ricci scalar",
                  outputs.ricci_contractions.physical_scalar, 225.5);

    const double missing_rww =
        h_UU.xx * bridge_ricci.xx + 2.0 * h_UU.xz * bridge_ricci.xz +
        h_UU.zz * bridge_ricci.zz;
    if (std::abs(missing_rww - 451.0) <= tol)
    {
        fail("Rww omission sensitivity", "dropping Rww still matched oracle");
    }
    std::cout << "PASS dropping Rww would fail: " << missing_rww << "\n";

    const double missing_offdiag_factor =
        h_UU.xx * bridge_ricci.xx + h_UU.xz * bridge_ricci.xz +
        h_UU.zz * bridge_ricci.zz + 2.0 * h_UU.ww * bridge_ricci.ww;
    if (std::abs(missing_offdiag_factor - 451.0) <= tol)
    {
        fail("off-diagonal factor sensitivity",
             "missing off-diagonal factor still matched oracle");
    }
    std::cout << "PASS missing off-diagonal factor would fail: "
              << missing_offdiag_factor << "\n";
}

} // namespace

int main()
{
    std::cout << "Stage 4J BlackStringToy Ricci-to-RHS contract fixture\n";
    std::cout << "Local contract only; no RHS formulas, grid data, "
                 "evolution wiring, or small-axis regularization are used.\n";

    require_false("RHS terms are implemented",
                  CartoonRhsContract::rhs_terms_implemented);
    require_false("evolution wiring is implemented",
                  CartoonRhsContract::evolution_wiring_implemented);
    check_contract_trace();
    require_throw_bad_axis();

    std::cout << "All Stage 4J Ricci-to-RHS contract checks passed.\n";
    return 0;
}
