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
namespace CR = BlackStringToy::CartoonConformalRww;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;

constexpr double tol = 1.0e-12;
constexpr double ricci_tol = 1.0e-10;

struct ConformalRwwSample
{
    double x;
    double A;
    double B;
    double C;
    double W;
    double A_x;
    double A_z;
    double B_x;
    double B_z;
    double C_x;
    double C_z;
    double W_x;
    double W_z;
    double W_xx;
    double W_xz;
    double W_zz;
};

static_assert(!std::is_aggregate<CR::ConformalRwwInputs>::value,
              "Stage 4AC conformal Rww inputs must not be an open aggregate");
static_assert(!std::is_aggregate<CR::ConformalRww>::value,
              "Stage 4AC conformal Rww result must not be an open aggregate");

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

CR::ConformalRwwInputs make_inputs(const ConformalRwwSample &sample)
{
    return CR::make_conformal_rww_inputs(
        sample.x, sample.A, sample.B, sample.C, sample.W, sample.A_x,
        sample.A_z, sample.B_x, sample.B_z, sample.C_x, sample.C_z,
        sample.W_x, sample.W_z, sample.W_xx, sample.W_xz, sample.W_zz);
}

CR::ConformalRww compute(const ConformalRwwSample &sample)
{
    return CR::compute_conformal_rww(make_inputs(sample));
}

Ricci::CartoonRicciInputs stage4g_inputs(const ConformalRwwSample &sample)
{
    Ricci::CartoonRicciInputs inputs{};
    inputs.x = sample.x;
    inputs.chi = 1.0;
    inputs.h = {sample.A, sample.B, sample.C, sample.W};
    inputs.h_derivatives.dx = {sample.A_x, sample.B_x, sample.C_x,
                               sample.W_x};
    inputs.h_derivatives.dz = {sample.A_z, sample.B_z, sample.C_z,
                               sample.W_z};
    inputs.h_derivatives.dxx = {0.0, 0.0, 0.0, sample.W_xx};
    inputs.h_derivatives.dxz = {0.0, 0.0, 0.0, sample.W_xz};
    inputs.h_derivatives.dzz = {0.0, 0.0, 0.0, sample.W_zz};
    return inputs;
}

double stage4g_rww(const ConformalRwwSample &sample)
{
    const auto ricci = RicciBridge::to_rhs_ricci_components(
        Ricci::compute_metric_derivative_ricci(stage4g_inputs(sample)));
    return ricci.ww;
}

ConformalRwwSample flat_sample()
{
    return {2.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
}

ConformalRwwSample constant_cone_sample()
{
    return {2.0, 1.0, 0.0, 1.0, 4.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
}

ConformalRwwSample nonconstant_w_sample()
{
    return {1.0, 1.0, 0.0, 1.0, 4.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 4.0, 0.0, 2.0, 0.0, 0.0};
}

ConformalRwwSample nonsymmetric_sample()
{
    return {2.0, 7.0, 2.0, 5.0, 3.0, 11.0, 13.0, 17.0,
            19.0, 23.0, 29.0, 6.0, 4.0, 31.0, 37.0, 41.0};
}

void check_flat_oracle()
{
    const auto result = compute(flat_sample());
    require_close("flat G_sing", result.singular_block(), 0.0);
    require_close("flat G_grad", result.gradient_block(), 0.0);
    require_close("flat G_Hess", result.hessian_block(), 0.0);
    require_close("flat tilde R_ww", result.value(), 0.0);
}

void check_constant_cone_oracle()
{
    const auto result = compute(constant_cone_sample());
    require_close("constant cone G_sing", result.singular_block(), -0.75);
    require_close("constant cone G_grad", result.gradient_block(), 0.0);
    require_close("constant cone G_Hess", result.hessian_block(), 0.0);
    require_close("constant cone tilde R_ww", result.value(), -0.75);
}

void check_nonconstant_w_oracle()
{
    const auto result = compute(nonconstant_w_sample());
    require_close("nonconstant W G_sing", result.singular_block(), -3.0);
    require_close("nonconstant W G_grad", result.gradient_block(), -5.0);
    require_close("nonconstant W G_Hess", result.hessian_block(), -4.0);
    require_close("nonconstant W tilde R_ww", result.value(), -12.0);
    require_close("nonconstant W direct Stage 4G comparison", result.value(),
                  stage4g_rww(nonconstant_w_sample()), ricci_tol);
}

void check_nonsymmetric_oracle()
{
    const auto result = compute(nonsymmetric_sample());
    require_close("nonsymmetric G_sing", result.singular_block(), 4.0 / 31.0);
    require_close("nonsymmetric G_grad", result.gradient_block(), -82.0 / 93.0);
    require_close("nonsymmetric G_Hess", result.hessian_block(),
                  -8558.0 / 2883.0);
    require_close("nonsymmetric tilde R_ww", result.value(), -3576.0 / 961.0);
    require_close("nonsymmetric direct Stage 4G comparison", result.value(),
                  stage4g_rww(nonsymmetric_sample()), ricci_tol);
}

void check_invalid_inputs_reject()
{
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    ConformalRwwSample sample = nonsymmetric_sample();

    sample.x = 0.0;
    require_throws("zero axis", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.x = -1.0;
    require_throws("negative axis", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.x = inf;
    require_throws("infinite axis", [&]() { make_inputs(sample); });

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
    sample.W_x = nan;
    require_throws("nonfinite W_x", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.C_z = inf;
    require_throws("nonfinite base derivative",
                   [&]() { make_inputs(sample); });
}

} // namespace

int main()
{
    std::cout << "Stage 4AC BlackStringToy conformal Rww assembly fixture\n";
    std::cout << "Conformal tilde R_ww only; no R^chi_ww, physical Ricci, "
                 "RHS, grid reads, or evolution wiring.\n";

    check_flat_oracle();
    check_constant_cone_oracle();
    check_nonconstant_w_oracle();
    check_nonsymmetric_oracle();
    check_invalid_inputs_reject();

    std::cout << "All Stage 4AC conformal Rww assembly checks passed.\n";
    return 0;
}
