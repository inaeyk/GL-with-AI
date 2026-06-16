#include "CartoonRicciBridge.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
using namespace BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;

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

void require_throw_away_axis()
{
    CartoonRicciInputs inputs{};
    inputs.x = 0.0;
    inputs.chi = 1.0;
    inputs.h = {1.0, 0.0, 1.0, 1.0};

    try
    {
        (void)compute_metric_derivative_ricci(inputs);
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS away-axis guard rejects x = 0\n";
        return;
    }

    fail("away-axis guard", "x = 0 was accepted");
}

CartoonRicciInputs flat_inputs()
{
    CartoonRicciInputs inputs{};
    inputs.x = 2.0;
    inputs.chi = 1.0;
    inputs.h = {1.0, 0.0, 1.0, 1.0};
    return inputs;
}

CartoonRicciInputs constant_q_cone_inputs()
{
    CartoonRicciInputs inputs = flat_inputs();
    inputs.h.ww = 4.0;
    return inputs;
}

CartoonRicciInputs nonconstant_q_inputs()
{
    // Stage 3E warped-product profile with lambda = 1, mu = 0 at x = 1:
    // f = x (1 + x), q = gamma_ww = (1 + x)^2.
    // Therefore q = 4, q_x = 4, q_xx = 2 at x = 1.
    CartoonRicciInputs inputs{};
    inputs.x = 1.0;
    inputs.chi = 1.0;
    inputs.h = {1.0, 0.0, 1.0, 4.0};
    inputs.h_derivatives.dx = {0.0, 0.0, 0.0, 4.0};
    inputs.h_derivatives.dxx = {0.0, 0.0, 0.0, 2.0};
    return inputs;
}

CartoonRicciInputs x_shear_flat_inputs()
{
    // Stage 3G flat pullback with dZ = dz + x dx at x = 2:
    // gamma_xx = 1 + x^2, gamma_xz = x, gamma_zz = 1, gamma_ww = 1.
    CartoonRicciInputs inputs{};
    inputs.x = 2.0;
    inputs.chi = 1.0;
    inputs.h = {5.0, 2.0, 1.0, 1.0};
    inputs.h_derivatives.dx = {4.0, 1.0, 0.0, 0.0};
    inputs.h_derivatives.dxx = {2.0, 0.0, 0.0, 0.0};
    return inputs;
}

void check_flat_zero()
{
    const auto ricci =
        RicciBridge::to_rhs_ricci_components(compute_metric_derivative_ricci(
            flat_inputs()));
    require_close("flat R_xx", ricci.xx, 0.0);
    require_close("flat R_xz", ricci.xz, 0.0);
    require_close("flat R_zz", ricci.zz, 0.0);
    require_close("flat R_ww", ricci.ww, 0.0);
}

void check_constant_q_cone()
{
    // Stage 3D constant-q cone with q0 = 4 at x = 2:
    // R_theta theta = 1 - q0 = -3, so the Cartesian-like hidden component is
    // R_ww = R_theta theta / x^2 = -3/4.
    const auto ricci =
        RicciBridge::to_rhs_ricci_components(compute_metric_derivative_ricci(
            constant_q_cone_inputs()));
    require_close("constant-q cone R_xx", ricci.xx, 0.0);
    require_close("constant-q cone R_xz", ricci.xz, 0.0);
    require_close("constant-q cone R_zz", ricci.zz, 0.0);
    require_close("constant-q cone R_ww", ricci.ww, -0.75);
}

void check_nonconstant_q_oracle()
{
    // Stage 3E warped-product formulas for f = x(1+x) at x = 1:
    // f = 2, f_x = 3, f_xx = 2, Delta f = 2.
    // R_xx = -2 f_xx / f = -2.
    // R_theta theta = 1 - |grad f|^2 - f Delta f = 1 - 9 - 4 = -12,
    // so R_ww = -12 at x = 1.
    const auto ricci =
        RicciBridge::to_rhs_ricci_components(compute_metric_derivative_ricci(
            nonconstant_q_inputs()));
    require_close("nonconstant-q R_xx", ricci.xx, -2.0);
    require_close("nonconstant-q R_xz", ricci.xz, 0.0);
    require_close("nonconstant-q R_zz", ricci.zz, 0.0);
    require_close("nonconstant-q R_ww", ricci.ww, -12.0);
}

void check_x_shear_flat_gate()
{
    const auto ricci =
        RicciBridge::to_rhs_ricci_components(compute_metric_derivative_ricci(
            x_shear_flat_inputs()));
    require_close("x-shear flat R_xx", ricci.xx, 0.0);
    require_close("x-shear flat R_xz", ricci.xz, 0.0);
    require_close("x-shear flat R_zz", ricci.zz, 0.0);
    require_close("x-shear flat R_ww", ricci.ww, 0.0);
}

} // namespace

int main()
{
    std::cout << "Stage 4G BlackStringToy metric-derivative cartoon Ricci "
                 "fixture\n";
    std::cout << "Local-value only; no grid data, RHS wiring, evolution, or "
                 "small-axis regularization is used.\n";

    check_flat_zero();
    check_constant_q_cone();
    check_nonconstant_q_oracle();
    check_x_shear_flat_gate();
    require_throw_away_axis();

    std::cout << "All Stage 4G metric-derivative cartoon Ricci checks passed.\n";
    return 0;
}
