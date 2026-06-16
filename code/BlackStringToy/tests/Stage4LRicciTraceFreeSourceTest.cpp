#include "CartoonRhsSourceBlock.hpp"

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

void require_true(const std::string &label, const bool value)
{
    if (!value)
    {
        fail(label, "expected true");
    }
    std::cout << "PASS " << label << " = true\n";
}

template <class Function> void require_domain_error(const std::string &label,
                                                    Function &&function)
{
    try
    {
        function();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    catch (const std::exception &exception)
    {
        fail(label, std::string("wrong exception: ") + exception.what());
    }
    fail(label, "expected std::domain_error");
}

CartoonRhsSourceBlock::LocalSourceBlockInput make_source_input(
    const CartoonRicciBridge::RhsRicciComponents &ricci,
    const ConformalCartoonAlgebra::InverseConformalMetric &h_UU,
    const ConformalCartoonAlgebra::ConformalMetric &h_LL)
{
    return {{{ricci, h_UU, 0.5}, 2.0}, h_LL};
}

void check_hard_coded_projection_oracle()
{
    const auto ricci = CartoonRicciBridge::to_rhs_ricci_components(
        CartoonRicci::RicciComponents(2.0, 3.0, 5.0, 7.0));
    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{11.0, 13.0,
                                                               17.0, 19.0};
    const ConformalCartoonAlgebra::ConformalMetric h_LL{1.0, 0.0, 1.0, 1.0};
    const auto source_input = make_source_input(ricci, h_UU, h_LL);

    const auto tf =
        CartoonRhsSourceBlock::compute_ricci_trace_free_source_block(
            source_input);

    // Hard-coded trace oracle:
    // 11*2 + 2*13*3 + 17*5 + 2*19*7 = 451.
    // With h_xx=h_zz=h_ww=1 and h_xz=0:
    // R_xx^TF = 2 - 451/4 = -110.75
    // R_xz^TF = 3
    // R_zz^TF = 5 - 451/4 = -107.75
    // R_ww^TF = 7 - 451/4 = -105.75
    const double source_trace =
        CartoonRhsContract::compute_ricci_contractions(
            source_input.rhs_contract.ricci)
            .conformal_inverse_contract;
    require_close("hard-coded Ricci trace", source_trace, 451.0);
    require_close("R_xx trace-free oracle", tf.xx, -110.75);
    require_close("R_xz trace-free oracle", tf.xz, 3.0);
    require_close("R_zz trace-free oracle", tf.zz, -107.75);
    require_close("R_ww trace-free oracle", tf.ww, -105.75);

    const double trace_without_hidden_factor =
        h_UU.xx * ricci.xx + 2.0 * h_UU.xz * ricci.xz +
        h_UU.zz * ricci.zz + h_UU.ww * ricci.ww;
    const double xx_without_hidden_factor =
        ricci.xx - h_LL.xx * trace_without_hidden_factor / 4.0;
    if (std::abs(xx_without_hidden_factor - tf.xx) <= tol)
    {
        fail("hidden multiplicity sensitivity",
             "missing hidden factor matched R_xx^TF oracle");
    }
    std::cout << "PASS missing hidden factor would fail: "
              << xx_without_hidden_factor << "\n";

    const double trace_without_rww =
        h_UU.xx * ricci.xx + 2.0 * h_UU.xz * ricci.xz +
        h_UU.zz * ricci.zz;
    const double xx_without_rww = ricci.xx - h_LL.xx * trace_without_rww / 4.0;
    if (std::abs(xx_without_rww - tf.xx) <= tol)
    {
        fail("Rww omission sensitivity",
             "dropping Rww matched R_xx^TF oracle");
    }
    std::cout << "PASS dropping Rww would fail: " << xx_without_rww << "\n";

    const double xx_with_denominator_3 = ricci.xx - h_LL.xx * 451.0 / 3.0;
    if (std::abs(xx_with_denominator_3 - tf.xx) <= tol)
    {
        fail("denominator sensitivity", "/3 matched R_xx^TF oracle");
    }
    std::cout << "PASS using /3 would fail: " << xx_with_denominator_3
              << "\n";
}

void check_consistent_trace_free_zero()
{
    const auto ricci = CartoonRicciBridge::to_rhs_ricci_components(
        CartoonRicci::RicciComponents(2.0, 3.0, 5.0, 7.0));
    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{1.0, 0.0, 1.0,
                                                               1.0};
    const ConformalCartoonAlgebra::ConformalMetric h_LL{1.0, 0.0, 1.0, 1.0};
    const auto source_input = make_source_input(ricci, h_UU, h_LL);

    const auto tf =
        CartoonRhsSourceBlock::compute_ricci_trace_free_source_block(
            source_input);

    // Full 4D trace is 2 + 5 + 2*7 = 21 for this consistent diagonal metric.
    require_close("consistent R_xx trace-free", tf.xx, -3.25);
    require_close("consistent R_xz trace-free", tf.xz, 3.0);
    require_close("consistent R_zz trace-free", tf.zz, -0.25);
    require_close("consistent R_ww trace-free", tf.ww, 1.75);
    require_close("full 4D trace of trace-free projection", tf.trace, 0.0);
}

void check_away_axis_guard()
{
    const auto ricci = CartoonRicciBridge::to_rhs_ricci_components(
        CartoonRicci::RicciComponents(2.0, 3.0, 5.0, 7.0));
    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{1.0, 0.0, 1.0,
                                                               1.0};
    const ConformalCartoonAlgebra::ConformalMetric h_LL{1.0, 0.0, 1.0, 1.0};
    CartoonRhsSourceBlock::LocalSourceBlockInput source_input =
        make_source_input(ricci, h_UU, h_LL);
    source_input.rhs_contract.x = 0.0;

    require_domain_error("Stage 4L away-axis guard",
                         [&source_input]() {
                             (void)CartoonRhsSourceBlock::
                                 compute_ricci_trace_free_source_block(
                                     source_input);
                         });
}

} // namespace

int main()
{
    std::cout << "Stage 4L BlackStringToy local trace-free Ricci source "
                 "fixture\n";
    std::cout << "Local formula block only; no live RHS wiring, grid data, "
                 "or evolution path is used.\n";

    require_true("trace-free Ricci projection is implemented",
                 CartoonRhsSourceBlock::trace_free_ricci_projection_implemented);
    check_hard_coded_projection_oracle();
    check_consistent_trace_free_zero();
    check_away_axis_guard();

    std::cout << "All Stage 4L trace-free Ricci source checks passed.\n";
    return 0;
}
