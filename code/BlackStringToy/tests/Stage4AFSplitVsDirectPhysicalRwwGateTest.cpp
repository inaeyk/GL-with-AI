#include "CartoonConformalFactorRww.hpp"
#include "CartoonConformalRww.hpp"
#include "CartoonRicciBridge.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <string>

namespace
{
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace CF = BlackStringToy::CartoonConformalFactorRww;
namespace CR = BlackStringToy::CartoonConformalRww;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;

constexpr double identity_tolerance = 1.0e-10;

// Stage 4AF is an internal split-vs-direct identity gate. The direct oracle is
// the project's Stage 4G metric-derivative engine, not external validation.
// Physical R_ww production assembly remains Stage 4AH; true parity/near-axis
// validation remains Stage 4AG; RHS and evolution remain unimplemented.
struct Sample
{
    double x;
    Algebra::ConformalMetric h;
    Ricci::MetricDerivatives h_derivatives;
    double chi;
    Ricci::ScalarDerivatives chi_derivatives;
};

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected,
                   const double tolerance = identity_tolerance)
{
    const double residual = std::abs(value - expected);
    if (residual > tolerance)
    {
        fail(label, "split " + std::to_string(value) + ", direct " +
                        std::to_string(expected) + ", residual " +
                        std::to_string(residual));
    }
    std::cout << "PASS " << label << " residual = " << residual << "\n";
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

Algebra::ConformalMetric scale(const Algebra::ConformalMetric &value,
                               const double factor)
{
    return {factor * value.xx, factor * value.xz, factor * value.zz,
            factor * value.ww};
}

Algebra::ConformalMetric add(const Algebra::ConformalMetric &left,
                             const Algebra::ConformalMetric &right)
{
    return {left.xx + right.xx, left.xz + right.xz, left.zz + right.zz,
            left.ww + right.ww};
}

Algebra::ConformalMetric physical_first_derivative(
    const Algebra::ConformalMetric &h, const Algebra::ConformalMetric &dh,
    const double chi, const double dchi)
{
    const double inv_chi = 1.0 / chi;
    return add(scale(dh, inv_chi),
               scale(h, -dchi * inv_chi * inv_chi));
}

Algebra::ConformalMetric physical_second_derivative(
    const Algebra::ConformalMetric &h, const Algebra::ConformalMetric &dh_a,
    const Algebra::ConformalMetric &dh_b,
    const Algebra::ConformalMetric &d2h_ab, const double chi,
    const double dchi_a, const double dchi_b, const double d2chi_ab)
{
    const double inv_chi = 1.0 / chi;
    const double inv_chi2 = inv_chi * inv_chi;
    const double inv_chi3 = inv_chi2 * inv_chi;

    // Product rule for gamma = chi^{-1} h, evaluated independently in this
    // gate before Stage 4G sees the already-physical metric jet.
    auto result = scale(d2h_ab, inv_chi);
    result = add(result, scale(dh_a, -dchi_b * inv_chi2));
    result = add(result, scale(dh_b, -dchi_a * inv_chi2));
    result = add(result, scale(h, -d2chi_ab * inv_chi2));
    return add(result, scale(h, 2.0 * dchi_a * dchi_b * inv_chi3));
}

Ricci::CartoonRicciInputs direct_physical_inputs(const Sample &sample)
{
    Ricci::CartoonRicciInputs inputs{};
    inputs.x = sample.x;
    inputs.chi = 1.0;
    inputs.h = scale(sample.h, 1.0 / sample.chi);
    inputs.h_derivatives.dx = physical_first_derivative(
        sample.h, sample.h_derivatives.dx, sample.chi,
        sample.chi_derivatives.first.dx);
    inputs.h_derivatives.dz = physical_first_derivative(
        sample.h, sample.h_derivatives.dz, sample.chi,
        sample.chi_derivatives.first.dz);
    inputs.h_derivatives.dxx = physical_second_derivative(
        sample.h, sample.h_derivatives.dx, sample.h_derivatives.dx,
        sample.h_derivatives.dxx, sample.chi,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dx,
        sample.chi_derivatives.second.dxx);
    inputs.h_derivatives.dxz = physical_second_derivative(
        sample.h, sample.h_derivatives.dx, sample.h_derivatives.dz,
        sample.h_derivatives.dxz, sample.chi,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dz,
        sample.chi_derivatives.second.dxz);
    inputs.h_derivatives.dzz = physical_second_derivative(
        sample.h, sample.h_derivatives.dz, sample.h_derivatives.dz,
        sample.h_derivatives.dzz, sample.chi,
        sample.chi_derivatives.first.dz, sample.chi_derivatives.first.dz,
        sample.chi_derivatives.second.dzz);
    return inputs;
}

double direct_physical_rww(const Sample &sample)
{
    const auto ricci = RicciBridge::to_rhs_ricci_components(
        Ricci::compute_metric_derivative_ricci(direct_physical_inputs(sample)));
    return ricci.ww;
}

CR::ConformalRwwInputs conformal_inputs(const Sample &sample)
{
    return CR::make_conformal_rww_inputs(
        sample.x, sample.h.xx, sample.h.xz, sample.h.zz, sample.h.ww,
        sample.h_derivatives.dx.xx, sample.h_derivatives.dz.xx,
        sample.h_derivatives.dx.xz, sample.h_derivatives.dz.xz,
        sample.h_derivatives.dx.zz, sample.h_derivatives.dz.zz,
        sample.h_derivatives.dx.ww, sample.h_derivatives.dz.ww,
        sample.h_derivatives.dxx.ww, sample.h_derivatives.dxz.ww,
        sample.h_derivatives.dzz.ww);
}

CF::ConformalFactorRwwInputs conformal_factor_inputs(const Sample &sample)
{
    return CF::make_conformal_factor_rww_inputs(
        sample.x, sample.h.xx, sample.h.xz, sample.h.zz, sample.h.ww,
        sample.chi, sample.h_derivatives.dx.xx,
        sample.h_derivatives.dz.xx, sample.h_derivatives.dx.xz,
        sample.h_derivatives.dz.xz, sample.h_derivatives.dx.zz,
        sample.h_derivatives.dz.zz, sample.h_derivatives.dx.ww,
        sample.h_derivatives.dz.ww, sample.chi_derivatives.first.dx,
        sample.chi_derivatives.first.dz, sample.chi_derivatives.second.dxx,
        sample.chi_derivatives.second.dxz,
        sample.chi_derivatives.second.dzz);
}

double split_rww(const Sample &sample)
{
    return CR::compute_conformal_rww(conformal_inputs(sample)).value() +
           CF::compute_conformal_factor_rww(
               conformal_factor_inputs(sample))
               .value();
}

Sample zero_derivative_sample(const double chi)
{
    Sample sample{};
    sample.x = 2.0;
    sample.h = {1.0, 0.0, 1.0, 1.0};
    sample.chi = chi;
    return sample;
}

Sample constant_chi_sample()
{
    Sample sample{};
    sample.x = 1.7;
    sample.h = {5.0, 1.0, 4.0, 2.0};
    sample.h_derivatives.dx = {0.4, -0.3, 0.2, 0.7};
    sample.h_derivatives.dz = {-0.2, 0.5, 0.6, -0.4};
    sample.h_derivatives.dxx = {0.8, -0.6, 0.9, 1.1};
    sample.h_derivatives.dxz = {-0.7, 0.3, -0.5, 0.4};
    sample.h_derivatives.dzz = {0.2, 0.9, -0.8, -0.6};
    sample.chi = 2.5;
    return sample;
}

Sample nonsymmetric_sample_one()
{
    Sample sample{};
    sample.x = 2.0;
    sample.h = {7.0, 2.0, 5.0, 3.0};
    sample.h_derivatives.dx = {3.0, 5.0, 7.0, 6.0};
    sample.h_derivatives.dz = {4.0, 6.0, 8.0, 4.0};
    sample.h_derivatives.dxx = {1.0, -4.0, 7.0, 31.0};
    sample.h_derivatives.dxz = {-2.0, 5.0, -8.0, 37.0};
    sample.h_derivatives.dzz = {3.0, -6.0, 9.0, 41.0};
    sample.chi = 5.0;
    sample.chi_derivatives.first = {3.0, 2.0};
    sample.chi_derivatives.second = {7.0, 11.0, 13.0};
    return sample;
}

Sample nonsymmetric_sample_two()
{
    Sample sample{};
    sample.x = 1.25;
    sample.h = {4.0, 0.75, 3.0, 2.5};
    sample.h_derivatives.dx = {0.6, -0.4, 0.8, 1.1};
    sample.h_derivatives.dz = {-0.5, 0.7, -0.9, 0.6};
    sample.h_derivatives.dxx = {0.3, -0.2, 0.4, 1.7};
    sample.h_derivatives.dxz = {-0.6, 0.5, -0.7, -1.3};
    sample.h_derivatives.dzz = {0.9, -0.8, 1.0, 2.1};
    sample.chi = 1.8;
    sample.chi_derivatives.first = {0.2, -0.3};
    sample.chi_derivatives.second = {0.15, -0.12, 0.18};
    return sample;
}

Sample nonsymmetric_sample_three()
{
    Sample sample{};
    sample.x = 3.0;
    sample.h = {6.0, -1.5, 4.0, 1.7};
    sample.h_derivatives.dx = {-0.7, 0.9, 0.5, -1.2};
    sample.h_derivatives.dz = {0.8, -0.6, 1.1, 0.75};
    sample.h_derivatives.dxx = {-0.4, 0.3, 0.6, 2.4};
    sample.h_derivatives.dxz = {0.5, -0.7, 0.2, -1.8};
    sample.h_derivatives.dzz = {0.9, 0.4, -0.5, 1.6};
    sample.chi = 2.2;
    sample.chi_derivatives.first = {-0.25, 0.35};
    sample.chi_derivatives.second = {0.11, -0.09, 0.14};
    return sample;
}

void check_identity(const std::string &label, const Sample &sample)
{
    require_close(label, split_rww(sample), direct_physical_rww(sample));
}

void check_required_identity_points()
{
    const auto constant_chi = constant_chi_sample();
    const double conformal =
        CR::compute_conformal_rww(conformal_inputs(constant_chi)).value();
    const double correction = CF::compute_conformal_factor_rww(
                                  conformal_factor_inputs(constant_chi))
                                  .value();
    require_close("constant chi correction", correction, 0.0);
    require_close("constant chi direct equals conformal", conformal,
                  direct_physical_rww(constant_chi));

    auto linear_x = zero_derivative_sample(1.2);
    linear_x.chi_derivatives.first.dx = 0.1;
    check_identity("flat conformal metric, linear x chi", linear_x);

    auto linear_z = zero_derivative_sample(1.6);
    linear_z.chi_derivatives.first.dz = 0.2;
    check_identity("flat conformal metric, linear z chi", linear_z);

    // Claude Audit C regression: the single Stage 4AE W''=0 point is not the
    // gate. These are three distinct nonsymmetric chi(x,z) jets, all with
    // nonzero W_x, W_z, W_xx, W_xz, and W_zz.
    const auto sample_one = nonsymmetric_sample_one();
    check_identity("nonsymmetric point 1", sample_one);
    check_identity("nonsymmetric point 2", nonsymmetric_sample_two());
    check_identity("nonsymmetric point 3", nonsymmetric_sample_three());

    // R_ww is independent of A/B/C second derivatives. Keep them nonzero on
    // the direct side above, then remove only those entries and verify the
    // Stage 4G result is unchanged.
    auto no_base_second = sample_one;
    no_base_second.h_derivatives.dxx.xx = 0.0;
    no_base_second.h_derivatives.dxx.xz = 0.0;
    no_base_second.h_derivatives.dxx.zz = 0.0;
    no_base_second.h_derivatives.dxz.xx = 0.0;
    no_base_second.h_derivatives.dxz.xz = 0.0;
    no_base_second.h_derivatives.dxz.zz = 0.0;
    no_base_second.h_derivatives.dzz.xx = 0.0;
    no_base_second.h_derivatives.dzz.xz = 0.0;
    no_base_second.h_derivatives.dzz.zz = 0.0;
    require_close("base second-derivative cancellation",
                  direct_physical_rww(sample_one),
                  direct_physical_rww(no_base_second));
}

void check_invalid_inputs_reject()
{
    auto sample = nonsymmetric_sample_one();
    sample.x = 0.0;
    require_throws("4AC invalid axis", [&]() { conformal_inputs(sample); });

    sample = nonsymmetric_sample_one();
    sample.chi = 0.0;
    require_throws("4AE invalid chi",
                   [&]() { conformal_factor_inputs(sample); });

    sample = nonsymmetric_sample_one();
    sample.h = {1.0, 2.0, 1.0, 3.0};
    require_throws("4AC negative determinant",
                   [&]() { conformal_inputs(sample); });

    sample = nonsymmetric_sample_one();
    sample.h.ww = 0.0;
    require_throws("4AE invalid h_ww",
                   [&]() { conformal_factor_inputs(sample); });

    sample = nonsymmetric_sample_one();
    sample.chi_derivatives.second.dxz =
        std::numeric_limits<double>::infinity();
    require_throws("4AE nonfinite derivative",
                   [&]() { conformal_factor_inputs(sample); });
}

} // namespace

int main()
{
    std::cout << "Stage 4AF split-vs-direct physical Rww identity gate\n";
    std::cout << "Internal away-axis identity gate only; no production physical "
                 "R_ww, parity validation, RHS, or evolution wiring.\n";

    check_required_identity_points();
    check_invalid_inputs_reject();

    std::cout << "All Stage 4AF split-vs-direct identity checks passed.\n";
    return 0;
}
