#include "CartoonConformalRwwGradientBlock.hpp"
#include "CartoonConformalRwwHessianBlock.hpp"
#include "CartoonConformalRwwSingularBlock.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace
{
namespace HB = BlackStringToy::CartoonConformalRwwHessianBlock;
namespace GB = BlackStringToy::CartoonConformalRwwGradientBlock;
namespace SB = BlackStringToy::CartoonConformalRwwSingularBlock;

constexpr double tol = 1.0e-12;

struct HessianSample
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

static_assert(!std::is_aggregate<HB::ConformalRwwHessianBlockInputs>::value,
              "Stage 4AB Hessian inputs must not be an open aggregate");
static_assert(!std::is_aggregate<HB::ConformalRwwHessianBlock>::value,
              "Stage 4AB Hessian result must not be an open aggregate");

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

HB::ConformalRwwHessianBlockInputs make_inputs(const HessianSample &sample)
{
    return HB::make_conformal_rww_hessian_block_inputs(
        sample.x, sample.A, sample.B, sample.C, sample.W, sample.A_x,
        sample.A_z, sample.B_x, sample.B_z, sample.C_x, sample.C_z,
        sample.W_x, sample.W_z, sample.W_xx, sample.W_xz, sample.W_zz);
}

double hessian_block(const HessianSample &sample)
{
    const auto inputs = make_inputs(sample);
    return HB::compute_conformal_rww_hessian_block(inputs).hessian_block();
}

void check_flat_oracle()
{
    const HessianSample flat{2.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    require_close("flat Hessian block", hessian_block(flat), 0.0);
}

void check_constant_cone_oracle()
{
    const HessianSample cone{2.0, 1.0, 0.0, 1.0, 4.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    require_close("constant cone Hessian block", hessian_block(cone), 0.0);
}

void check_nonconstant_hidden_metric_oracle()
{
    // Flat base, W=(1+x)^2 at x=1 gives W=4, W_x=4, W_xx=2 and
    // G_Hess = -4. This is still only the Hessian sub-block.
    const HessianSample nonconstant{1.0, 1.0, 0.0, 1.0, 4.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0, 4.0,
                                    0.0, 2.0, 0.0, 0.0};

    require_close("nonconstant W Hessian block", hessian_block(nonconstant),
                  -4.0);
}

HessianSample nonsymmetric_sample()
{
    return {2.0, 7.0, 2.0, 5.0, 3.0, 11.0, 13.0, 17.0,
            19.0, 23.0, 29.0, 6.0, 4.0, 31.0, 37.0, 41.0};
}

void check_claude_verified_nonsymmetric_oracle()
{
    const HessianSample sample = nonsymmetric_sample();

    // Claude Audit A verified this sample against the independent Stage 4G
    // conformal Ricci engine. It exercises off-diagonal Christoffels, rho_xz,
    // W_z-dependent terms, and the (-2B/D) contraction; the simpler flat,
    // cone, and W=(1+x)^2 tests are not sufficient by themselves.
    require_close("verified nonsymmetric Hessian block", hessian_block(sample),
                  -8558.0 / 2883.0);
}

void check_test_only_conformal_sum_crosscheck()
{
    const HessianSample sample = nonsymmetric_sample();

    // Test-only cross-check of reviewed sub-blocks. This test does not add a
    // production full-tilde-R_ww assembly path.
    const auto singular_inputs = SB::make_conformal_rww_singular_block_inputs(
        sample.x, sample.A, sample.B, sample.C, sample.W);
    const auto gradient_inputs = GB::make_conformal_rww_gradient_block_inputs(
        sample.x, sample.A, sample.B, sample.C, sample.W, sample.W_x,
        sample.W_z);

    const double singular =
        SB::compute_conformal_rww_singular_gradient_block(singular_inputs)
            .singular_gradient_block();
    const double gradient =
        GB::compute_conformal_rww_gradient_block(gradient_inputs)
            .gradient_block();
    const double hessian = hessian_block(sample);

    require_close("nonsymmetric singular sub-block", singular, 4.0 / 31.0);
    require_close("nonsymmetric gradient sub-block", gradient, -82.0 / 93.0);
    require_close("test-only conformal sub-block sum",
                  singular + gradient + hessian, -3576.0 / 961.0);
}

void check_invalid_inputs_reject()
{
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    HessianSample sample = nonsymmetric_sample();

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
    sample.W_z = inf;
    require_throws("nonfinite W_z", [&]() { make_inputs(sample); });
    sample = nonsymmetric_sample();
    sample.A_x = inf;
    require_throws("nonfinite base derivative",
                   [&]() { make_inputs(sample); });
}

} // namespace

int main()
{
    std::cout << "Stage 4AB BlackStringToy conformal Rww Hessian block "
                 "fixture\n";
    std::cout << "Hessian sub-block only; no full tilde R_ww, R^chi_ww, "
                 "physical Ricci, RHS, grid reads, or evolution wiring.\n";

    check_flat_oracle();
    check_constant_cone_oracle();
    check_nonconstant_hidden_metric_oracle();
    check_claude_verified_nonsymmetric_oracle();
    check_test_only_conformal_sum_crosscheck();
    check_invalid_inputs_reject();

    std::cout << "All Stage 4AB conformal Rww Hessian block checks passed.\n";
    return 0;
}
