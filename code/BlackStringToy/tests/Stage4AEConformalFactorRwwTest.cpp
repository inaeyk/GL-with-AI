#include "CartoonConformalFactorRww.hpp"
#include "CartoonConformalRww.hpp"
#include "CartoonRicciBridge.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace
{
namespace CheckedChi = BlackStringToy::CartoonCheckedDxchiOverX;
namespace CF = BlackStringToy::CartoonConformalFactorRww;
namespace CR = BlackStringToy::CartoonConformalRww;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;

constexpr double tol = 1.0e-12;
constexpr double ricci_tol = 1.0e-10;

// Stage 4AE tests only the local away-axis conformal-factor correction
// R^chi_ww. It is not physical R_ww[gamma], not the Stage 4AF hard identity
// gate, not full Ricci, not CCZ4 RHS, and not evolution wiring. Checked local
// quotients do not prove global parity or finite-axis regularity.
struct Sample
{
    double x;
    double A;
    double B;
    double C;
    double W;
    double chi;
    double A_x;
    double A_z;
    double B_x;
    double B_z;
    double C_x;
    double C_z;
    double W_x;
    double W_z;
    double chi_x;
    double chi_z;
    double chi_xx;
    double chi_xz;
    double chi_zz;
};

static_assert(
    !std::is_aggregate<CheckedChi::CheckedDxchiOverXIngredient>::value,
    "Stage 4AE checked chi_x/x ingredient must not be an open aggregate");
static_assert(!std::is_aggregate<CF::ConformalFactorRwwInputs>::value,
              "Stage 4AE inputs must not be an open aggregate");
static_assert(!std::is_aggregate<CF::ConformalFactorRww>::value,
              "Stage 4AE result must not be an open aggregate");

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected, const double tolerance = tol)
{
    const double residual = std::abs(value - expected);
    if (residual > tolerance)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected) + ", residual " +
                        std::to_string(residual));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void require_throws(const std::string &label, const std::function<void()> &fn)
{
    try
    {
        fn();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    fail(label, "expected std::domain_error");
}

CF::ConformalFactorRwwInputs make_inputs(const Sample &sample)
{
    return CF::make_conformal_factor_rww_inputs(
        sample.x, sample.A, sample.B, sample.C, sample.W, sample.chi,
        sample.A_x, sample.A_z, sample.B_x, sample.B_z, sample.C_x,
        sample.C_z, sample.W_x, sample.W_z, sample.chi_x, sample.chi_z,
        sample.chi_xx, sample.chi_xz, sample.chi_zz);
}

CF::ConformalFactorRww compute(const Sample &sample)
{
    return CF::compute_conformal_factor_rww(make_inputs(sample));
}

Sample constant_chi_sample()
{
    return {2.0, 7.0, 2.0, 5.0, 3.0, 5.0, 3.0, 4.0, 5.0, 6.0,
            7.0, 8.0, 6.0, 4.0, 0.0, 0.0, 0.0, 0.0, 0.0};
}

Sample linear_x_chi_sample()
{
    return {2.0, 1.0, 0.0, 1.0, 1.0, 1.2, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0};
}

Sample linear_z_chi_sample()
{
    return {2.0, 1.0, 0.0, 1.0, 1.0, 1.6, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.2, 0.0, 0.0, 0.0};
}

Sample nonsymmetric_sample()
{
    return {2.0, 7.0, 2.0, 5.0, 3.0, 5.0, 3.0, 4.0, 5.0, 6.0,
            7.0, 8.0, 6.0, 4.0, 3.0, 2.0, 7.0, 11.0, 13.0};
}

Ricci::CartoonRicciInputs stage4g_inputs(const Sample &sample)
{
    Ricci::CartoonRicciInputs inputs{};
    inputs.x = sample.x;
    inputs.chi = sample.chi;
    inputs.h = {sample.A, sample.B, sample.C, sample.W};
    inputs.chi_derivatives.first = {sample.chi_x, sample.chi_z};
    inputs.chi_derivatives.second = {sample.chi_xx, sample.chi_xz,
                                    sample.chi_zz};
    inputs.h_derivatives.dx = {sample.A_x, sample.B_x, sample.C_x,
                               sample.W_x};
    inputs.h_derivatives.dz = {sample.A_z, sample.B_z, sample.C_z,
                               sample.W_z};
    inputs.h_derivatives.dxx = {0.0, 0.0, 0.0, 0.0};
    inputs.h_derivatives.dxz = {0.0, 0.0, 0.0, 0.0};
    inputs.h_derivatives.dzz = {0.0, 0.0, 0.0, 0.0};
    return inputs;
}

double stage4g_physical_rww(const Sample &sample)
{
    const auto ricci = RicciBridge::to_rhs_ricci_components(
        Ricci::compute_metric_derivative_ricci(stage4g_inputs(sample)));
    return ricci.ww;
}

double stage4ac_conformal_rww(const Sample &sample)
{
    const auto inputs = CR::make_conformal_rww_inputs(
        sample.x, sample.A, sample.B, sample.C, sample.W, sample.A_x,
        sample.A_z, sample.B_x, sample.B_z, sample.C_x, sample.C_z,
        sample.W_x, sample.W_z, 0.0, 0.0, 0.0);
    return CR::compute_conformal_rww(inputs).value();
}

void check_oracles()
{
    require_close("constant chi R^chi_ww", compute(constant_chi_sample()).value(),
                  0.0);
    require_close("linear x chi R^chi_ww", compute(linear_x_chi_sample()).value(),
                  11.0 / 144.0);
    require_close("linear z chi R^chi_ww", compute(linear_z_chi_sample()).value(),
                  -1.0 / 64.0);

    const auto nonsymmetric = compute(nonsymmetric_sample());
    require_close("nonsymmetric D_wD_w chi", nonsymmetric.hidden_hessian(),
                  131.0 / 62.0);
    require_close("nonsymmetric conformal Laplacian",
                  nonsymmetric.conformal_laplacian(), 10430.0 / 2883.0);
    require_close("nonsymmetric conformal gradient norm",
                  nonsymmetric.conformal_gradient_norm(), 49.0 / 31.0);
    require_close("nonsymmetric R^chi_ww", nonsymmetric.value(),
                  63341.0 / 48050.0);

    // This is one test-only pre-4AF cross-check, not the hard identity gate.
    // Stage 4AF still owns the general split-vs-direct physical Ricci gate.
    require_close("nonsymmetric Stage 4G difference comparison",
                  nonsymmetric.value(),
                  stage4g_physical_rww(nonsymmetric_sample()) -
                      stage4ac_conformal_rww(nonsymmetric_sample()),
                  ricci_tol);
}

void check_invalid_inputs_reject()
{
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    Sample sample = nonsymmetric_sample();

    sample.x = 0.0;
    require_throws("zero axis", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.x = -1.0;
    require_throws("negative axis", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.x = inf;
    require_throws("nonfinite axis", [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.W = 0.0;
    require_throws("zero h_ww", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.W = -1.0;
    require_throws("negative h_ww", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.W = nan;
    require_throws("nonfinite h_ww", [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.chi = 0.0;
    require_throws("zero chi", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.chi = -1.0;
    require_throws("negative chi", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.chi = inf;
    require_throws("nonfinite chi", [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.A = 1.0;
    sample.B = 1.0;
    sample.C = 1.0;
    require_throws("zero reduced determinant", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.A = 1.0;
    sample.B = 2.0;
    sample.C = 1.0;
    require_throws("negative reduced determinant",
                   [&]() { make_inputs(sample); });

    sample = nonsymmetric_sample();
    sample.A_x = nan;
    require_throws("nonfinite base derivative", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.W_x = inf;
    require_throws("nonfinite W derivative", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.chi_x = nan;
    require_throws("nonfinite chi first derivative",
                   [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.chi_xz = inf;
    require_throws("nonfinite chi second derivative",
                   [&]() { make_inputs(sample); });
}

} // namespace

int main()
{
    std::cout << "Stage 4AE BlackStringToy conformal-factor Rww fixture\n";
    std::cout << "R^chi_ww only; away-axis local values; no physical R_ww, "
                 "hard identity gate, RHS, grid reads, or evolution wiring.\n";

    check_oracles();
    check_invalid_inputs_reject();

    std::cout << "All Stage 4AE conformal-factor Rww checks passed.\n";
    return 0;
}
