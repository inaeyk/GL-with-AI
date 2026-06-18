#include "CartoonSourceFormulaAuthoringGate.hpp"

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

double dummy_authoring_gate_consumer(
    const CartoonSourceFormulaAuthoringGate::
        RegularitySensitiveSourceInputs &inputs)
{
    // Harmless diagnostic scalar only. This is not a Ricci or RHS formula; it
    // exists to demonstrate the future source-formula signature shape.
    const auto &geometry = inputs.checked_geometry();
    return geometry.dx_hww_over_x() + geometry.hxx_minus_hww_over_x2();
}

void check_type_boundary()
{
    using Guarded =
        CartoonRegularityGuardedSources::RegularityGuardedGeometrySources;
    using Inputs =
        CartoonSourceFormulaAuthoringGate::RegularitySensitiveSourceInputs;
    using DummyFunction = decltype(&dummy_authoring_gate_consumer);

    static_assert(!std::is_aggregate<Guarded>::value,
                  "checked geometry package must not be an open aggregate");
    static_assert(!std::is_aggregate<Inputs>::value,
                  "authoring-gate input must not be an open aggregate");
    static_assert(!std::is_constructible<Inputs, double, double, double>::value,
                  "authoring-gate input must not be constructible from raw "
                  "geometry doubles");
    static_assert(std::is_invocable<DummyFunction, const Inputs &>::value,
                  "dummy consumer must accept the authoring-gate input");
    static_assert(!std::is_invocable<DummyFunction, double, double,
                                     double>::value,
                  "dummy consumer must not accept raw h_xx, h_ww, x");

    // Former bypass shape, intentionally left as a non-compiling example:
    // dummy_authoring_gate_consumer(h_xx, h_ww, x);
    std::cout << "PASS source-formula authoring type boundary is closed\n";
}

void check_sanctioned_stage4s_path()
{
    const auto source_output =
        CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
            matching_input());
    const auto formula_inputs =
        CartoonSourceFormulaAuthoringGate::
            make_regularity_sensitive_source_inputs(source_output);

    const auto &geometry = formula_inputs.checked_geometry();
    require_close("gate carries dx hww over x", geometry.dx_hww_over_x(),
                  2.0);
    require_close("gate carries hxx minus hww over x^2",
                  geometry.hxx_minus_hww_over_x2(), 0.25);
    require_close("dummy consumer diagnostic",
                  dummy_authoring_gate_consumer(formula_inputs), 2.25);
}

void check_sanctioned_stage4r_path()
{
    const auto guarded =
        CartoonRegularityGuardedSources::
            compute_away_axis_regularly_matched_geometry_sources(
                matching_input().guarded_geometry);
    const auto formula_inputs =
        CartoonSourceFormulaAuthoringGate::
            make_regularity_sensitive_source_inputs(guarded);

    require_close("direct guarded gate diagnostic",
                  dummy_authoring_gate_consumer(formula_inputs), 2.25);
}

void check_mismatch_fails_before_gate()
{
    require_domain_error("mismatch fails before authoring gate", []() {
        const auto source_output =
            CartoonRhsSourceBlock::compute_local_guarded_geometry_source_block(
                {{0.1, 1.2, 1.0, 1.0}});
        (void)CartoonSourceFormulaAuthoringGate::
            make_regularity_sensitive_source_inputs(source_output);
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4U BlackStringToy source-formula authoring-gate "
                 "fixture\n";
    std::cout << "Authoring gate only; this is not a Ricci/RHS formula and "
                 "not evolution wiring.\n";

    require_false("full Ricci or RHS formula implemented",
                  CartoonSourceFormulaAuthoringGate::
                      full_ricci_or_rhs_formula_implemented);
    require_false("evolution wiring implemented",
                  CartoonSourceFormulaAuthoringGate::
                      evolution_wiring_implemented);
    require_false("grid reads implemented",
                  CartoonSourceFormulaAuthoringGate::grid_reads_implemented);

    check_type_boundary();
    check_sanctioned_stage4s_path();
    check_sanctioned_stage4r_path();
    check_mismatch_fails_before_gate();

    std::cout << "All Stage 4U source-formula authoring-gate checks passed.\n";
    return 0;
}
