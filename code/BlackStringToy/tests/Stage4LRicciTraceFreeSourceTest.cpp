#include "CartoonRhsSourceBlock.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
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
    const ConformalCartoonAlgebra::ConformalMetric h_LL{
        17.0 / 18.0, -13.0 / 18.0, 11.0 / 18.0, 1.0 / 19.0};
    const auto source_input = make_source_input(ricci, h_UU, h_LL);

    const auto tf =
        CartoonRhsSourceBlock::compute_ricci_trace_free_source_block(
            source_input);

    // Hard-coded trace oracle:
    // 11*2 + 2*13*3 + 17*5 + 2*19*7 = 451.
    // h_LL is the exact inverse of the reduced h_UU block above, with
    // h_ww = 1/19, so this oracle also guards metric/inverse consistency.
    const double source_trace =
        CartoonRhsContract::compute_ricci_contractions(
            source_input.rhs_contract.ricci)
            .conformal_inverse_contract;
    require_close("hard-coded Ricci trace", source_trace, 451.0);
    require_close("R_xx trace-free oracle", tf.xx,
                  2.0 - h_LL.xx * 451.0 / 4.0);
    require_close("R_xz trace-free oracle", tf.xz,
                  3.0 - h_LL.xz * 451.0 / 4.0);
    require_close("R_zz trace-free oracle", tf.zz,
                  5.0 - h_LL.zz * 451.0 / 4.0);
    require_close("R_ww trace-free oracle", tf.ww,
                  7.0 - h_LL.ww * 451.0 / 4.0);

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

void check_metric_inverse_consistency_guard()
{
    const auto ricci = CartoonRicciBridge::to_rhs_ricci_components(
        CartoonRicci::RicciComponents(2.0, 3.0, 5.0, 7.0));
    const ConformalCartoonAlgebra::InverseConformalMetric h_UU{11.0, 13.0,
                                                               17.0, 19.0};
    const ConformalCartoonAlgebra::ConformalMetric h_LL{
        17.0 / 18.0, -13.0 / 18.0, 11.0 / 18.0, 1.0 / 19.0};

    ConformalCartoonAlgebra::require_inverse_metric_consistency(h_LL, h_UU);
    const auto consistent_input = make_source_input(ricci, h_UU, h_LL);
    (void)CartoonRhsSourceBlock::compute_ricci_trace_free_source_block(
        consistent_input);
    std::cout << "PASS consistent metric/inverse pair accepted\n";

    auto bad_reduced_inverse = h_UU;
    bad_reduced_inverse.xx += 0.125;
    auto bad_reduced_input =
        make_source_input(ricci, bad_reduced_inverse, h_LL);
    require_domain_error("reduced metric/inverse mismatch",
                         [&bad_reduced_input]() {
                             (void)CartoonRhsSourceBlock::
                                 compute_ricci_trace_free_source_block(
                                     bad_reduced_input);
                         });

    auto bad_hidden_inverse = h_UU;
    bad_hidden_inverse.ww = 18.0;
    auto bad_hidden_input = make_source_input(ricci, bad_hidden_inverse, h_LL);
    require_domain_error("hidden metric/inverse mismatch",
                         [&bad_hidden_input]() {
                             (void)CartoonRhsSourceBlock::
                                 compute_ricci_trace_free_source_block(
                                     bad_hidden_input);
                         });

    auto nonfinite_metric = h_LL;
    nonfinite_metric.xx = std::numeric_limits<double>::infinity();
    auto nonfinite_input = make_source_input(ricci, h_UU, nonfinite_metric);
    require_domain_error("nonfinite metric/inverse consistency input",
                         [&nonfinite_input]() {
                             (void)CartoonRhsSourceBlock::
                                 compute_ricci_trace_free_source_block(
                                     nonfinite_input);
                         });
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
    check_metric_inverse_consistency_guard();
    check_consistent_trace_free_zero();
    check_away_axis_guard();

    std::cout << "All Stage 4L trace-free Ricci source checks passed.\n";
    return 0;
}
