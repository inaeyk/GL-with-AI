#include "CartoonGuardedSourceConsumers.hpp"
#include "CartoonRhsSourceBlock.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

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

CartoonRhsSourceBlock::LocalGuardedGeometrySourceBlockInput matching_input()
{
    const double x = 0.5;
    const double h_ww = 1.0;
    const double h_xx = h_ww + 0.25 * x * x;
    const double d_x_hww = 1.0;
    return {{x, h_xx, h_ww, d_x_hww}};
}

void check_consumer_type_boundary()
{
    using Guarded =
        CartoonRegularityGuardedSources::RegularityGuardedGeometrySources;
    using Probe = CartoonGuardedSourceConsumers::GuardedGeometryConsumerProbe;
    using ConsumerFunction =
        decltype(&CartoonGuardedSourceConsumers::
                     compute_guarded_geometry_consumer_probe);

    static_assert(!std::is_aggregate<Guarded>::value,
                  "checked package must not be an open aggregate");
    static_assert(!std::is_aggregate<Probe>::value,
                  "consumer probe must not be an open aggregate");
    static_assert(std::is_invocable<ConsumerFunction, const Guarded &>::value,
                  "consumer must accept the checked package");
    static_assert(!std::is_invocable<ConsumerFunction, double, double,
                                     double>::value,
                  "consumer must not accept unchecked raw doubles");

    // Former bypasses, intentionally left as non-compiling examples:
    // CartoonGuardedSourceConsumers::compute_guarded_geometry_consumer_probe(
    //     2.0, 0.25, 0.1);
    // Guarded fake_guarded{2.0, 0.25, 0.1};
    std::cout << "PASS guarded consumer type boundary is closed\n";
}

void check_matching_case_from_stage4s()
{
    const auto source_output =
        CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            matching_input());
    const auto probe =
        CartoonGuardedSourceConsumers::compute_guarded_geometry_consumer_probe(
            source_output.regularity_guarded_geometry());

    require_close("consumer dx hww over x", probe.dx_hww_over_x(), 2.0);
    require_close("consumer hxx minus hww over x^2",
                  probe.hxx_minus_hww_over_x2(), 0.25);
    require_close("consumer matching residual",
                  probe.hxx_hww_matching_residual(), 0.25 / 1.0625);
    require_true("consumer residual is within Stage 4Q tolerance",
                 probe.matching_residual_within_stage4q_tolerance());
}

void check_matching_case_from_stage4r()
{
    const auto input = matching_input().guarded_geometry;
    const auto checked =
        CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(input);
    const auto probe =
        CartoonGuardedSourceConsumers::compute_guarded_geometry_consumer_probe(
            checked);

    require_close("direct guarded consumer dx hww over x",
                  probe.dx_hww_over_x(), 2.0);
    require_close("direct guarded consumer metric difference",
                  probe.hxx_minus_hww_over_x2(), 0.25);
}

void check_mismatch_fails_before_consumer()
{
    require_domain_error("mismatch fails before consumer", []() {
        const auto source_output =
            CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
                {{0.1, 1.2, 1.0, 1.0}});
        (void)CartoonGuardedSourceConsumers::
            compute_guarded_geometry_consumer_probe(
                source_output.regularity_guarded_geometry());
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4T BlackStringToy guarded source-consumer fixture\n";
    std::cout << "Diagnostic/probe consumer only; this is not full Ricci, not "
                 "full CCZ4 RHS, and not evolution wiring.\n";

    require_false("full Ricci or RHS formula implemented",
                  CartoonGuardedSourceConsumers::
                      full_ricci_or_rhs_formula_implemented);
    require_false("evolution wiring implemented",
                  CartoonGuardedSourceConsumers::evolution_wiring_implemented);
    require_true("consumer is diagnostic probe only",
                 CartoonGuardedSourceConsumers::diagnostic_probe_only);

    check_consumer_type_boundary();
    check_matching_case_from_stage4s();
    check_matching_case_from_stage4r();
    check_mismatch_fails_before_consumer();

    std::cout << "All Stage 4T guarded source-consumer checks passed.\n";
    return 0;
}
