#include "CartoonRicciBridge.hpp"

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

void require_throw_bad_chi()
{
    CartoonRicciBridge::RhsRicciBridgeInput input{
        CartoonRicci::RicciComponents(2.0, 3.0, 5.0, 7.0),
        {11.0, 13.0, 17.0, 19.0},
        0.0};

    try
    {
        (void)CartoonRicciBridge::make_rhs_ricci_bridge_contract(input);
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS bridge rejects nonpositive chi\n";
        return;
    }

    fail("bridge chi guard", "nonpositive chi was accepted");
}

void check_trace_contract()
{
    const CartoonRicci::RicciComponents ricci_LL(2.0, 3.0, 5.0, 7.0);
    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{11.0, 13.0,
                                                               17.0, 19.0};
    const double chi = 0.5;

    const CartoonRicciBridge::RhsRicciBridgeInput input{ricci_LL, h_UU, chi};
    const auto contract =
        CartoonRicciBridge::make_rhs_ricci_bridge_contract(input);

    // Hard-coded oracle:
    // h^{xx} R_xx + 2 h^{xz} R_xz + h^{zz} R_zz + 2 h^{ww} R_ww
    // = 11*2 + 2*13*3 + 17*5 + 2*19*7 = 451.
    require_close("full 4D conformal-inverse Ricci contraction",
                  contract.conformal_inverse_contract, 451.0);
    require_close("physical Ricci scalar from chi h^ij R_ij",
                  contract.physical_scalar, 225.5);

    // Direct ricci_LL.xx-style raw-field access intentionally no longer
    // compiles. The bridge exposes a distinct RHS-facing component type so
    // future code has one reviewed convention crossing point.
    const auto rhs_ricci =
        CartoonRicciBridge::to_rhs_ricci_components(ricci_LL);
    require_close("bridge exposes R_xx", rhs_ricci.xx, 2.0);
    require_close("bridge exposes R_xz", rhs_ricci.xz, 3.0);
    require_close("bridge exposes R_zz", rhs_ricci.zz, 5.0);
    require_close("bridge exposes R_ww", rhs_ricci.ww, 7.0);

    const double missing_offdiag_factor =
        h_UU.xx * rhs_ricci.xx + h_UU.xz * rhs_ricci.xz +
        h_UU.zz * rhs_ricci.zz + 2.0 * h_UU.ww * rhs_ricci.ww;
    const double missing_hidden_factor =
        h_UU.xx * rhs_ricci.xx + 2.0 * h_UU.xz * rhs_ricci.xz +
        h_UU.zz * rhs_ricci.zz + h_UU.ww * rhs_ricci.ww;

    if (std::abs(missing_offdiag_factor - 451.0) <= tol)
    {
        fail("off-diagonal factor sensitivity",
             "missing off-diagonal factor still matched oracle");
    }
    std::cout << "PASS missing off-diagonal factor would fail: "
              << missing_offdiag_factor << "\n";

    if (std::abs(missing_hidden_factor - 451.0) <= tol)
    {
        fail("hidden multiplicity sensitivity",
             "missing hidden factor still matched oracle");
    }
    std::cout << "PASS missing hidden factor would fail: "
              << missing_hidden_factor << "\n";
}

} // namespace

int main()
{
    std::cout << "Stage 4I BlackStringToy Ricci bridge contract fixture\n";
    std::cout << "Local contract only; no RHS wiring, grid data, Cell, Vars, "
                 "FArrayBox, or evolution data are used.\n";

    require_false("RHS wiring is implemented",
                  CartoonRicciBridge::rhs_wiring_implemented);
    check_trace_contract();
    require_throw_bad_chi();

    std::cout << "All Stage 4I Ricci bridge contract checks passed.\n";
    return 0;
}
