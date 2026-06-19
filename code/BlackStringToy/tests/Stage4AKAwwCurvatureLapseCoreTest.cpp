#include "CartoonAwwCurvatureLapseCore.hpp"

#include "CartoonRicciInterface.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace
{
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace Core = BlackStringToy::CartoonAwwCurvatureLapseCore;
namespace Lapse = BlackStringToy::CartoonPhysicalHiddenLapseHessian;
namespace PhysicalRww = BlackStringToy::CartoonAwayAxisPhysicalRww;
namespace Ricci = BlackStringToy::CartoonRicci;

constexpr double tolerance = 1.0e-12;

// Stage 4AK is only the local away-axis geometric core
// C_ww = -D_wD_w alpha + alpha R_ww[gamma]. It is not the Z4 term, trace-free
// projection, outer chi prefactor, nonlinear A/K terms, RHS, grid read,
// finite-axis support, or evolution wiring.
struct Sample
{
    double x = 2.0;
    Algebra::ConformalMetric h{1.0, 0.0, 1.0, 1.0};
    Ricci::MetricDerivatives h_derivatives{};
    double chi = 1.0;
    Ricci::ScalarDerivatives chi_derivatives{};
    double alpha = 1.0;
    double alpha_x = 0.0;
    double alpha_z = 0.0;
};

using CoreFunction = Core::AwwCurvatureLapseCore (*)(
    const Core::AwwCurvatureLapseCoreInputs &);

static_assert(!std::is_aggregate<Core::AwwCurvatureLapseCoreInputs>::value,
              "Stage 4AK inputs must not be an open aggregate");
static_assert(!std::is_aggregate<Core::AwwCurvatureLapseCore>::value,
              "Stage 4AK result must not be an open aggregate");
static_assert(!std::is_invocable<CoreFunction, double, double>::value,
              "Stage 4AK must not accept loose R_ww or D_wD_w alpha values");
static_assert(Core::away_axis_only, "Stage 4AK must remain away-axis only");
static_assert(!Core::z4_vector_term_implemented,
              "Stage 4AK must not claim the Z4 vector term");
static_assert(!Core::trace_free_projection_implemented,
              "Stage 4AK must not claim trace-free projection");
static_assert(!Core::chi_prefactor_implemented,
              "Stage 4AK must not claim the outer chi prefactor");
static_assert(!Core::ccz4_rhs_implemented,
              "Stage 4AK must not claim CCZ4 RHS implementation");
static_assert(!Core::evolution_wiring_implemented,
              "Stage 4AK must not claim evolution wiring");

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected, const double tol = tolerance)
{
    const double residual = std::abs(value - expected);
    if (residual > tol)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected) + ", residual " +
                        std::to_string(residual));
    }
    std::cout << "PASS " << label << " = " << value
              << ", residual = " << residual << "\n";
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

Core::AwwCurvatureLapseCoreInputs make_inputs(const Sample &sample)
{
    return Core::make_aww_curvature_lapse_core_inputs(
        sample.x, sample.h.xx, sample.h.xz, sample.h.zz, sample.h.ww,
        sample.chi, sample.alpha, sample.h_derivatives.dx.xx,
        sample.h_derivatives.dz.xx, sample.h_derivatives.dx.xz,
        sample.h_derivatives.dz.xz, sample.h_derivatives.dx.zz,
        sample.h_derivatives.dz.zz, sample.h_derivatives.dx.ww,
        sample.h_derivatives.dz.ww, sample.h_derivatives.dxx.ww,
        sample.h_derivatives.dxz.ww, sample.h_derivatives.dzz.ww,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dz,
        sample.chi_derivatives.second.dxx,
        sample.chi_derivatives.second.dxz,
        sample.chi_derivatives.second.dzz, sample.alpha_x,
        sample.alpha_z);
}

Core::AwwCurvatureLapseCore compute_core(const Sample &sample)
{
    return Core::compute_aww_curvature_lapse_core(make_inputs(sample));
}

double reviewed_physical_rww(const Sample &sample)
{
    const auto inputs = PhysicalRww::make_away_axis_physical_rww_inputs(
        sample.x, sample.h.xx, sample.h.xz, sample.h.zz, sample.h.ww,
        sample.chi, sample.h_derivatives.dx.xx,
        sample.h_derivatives.dz.xx, sample.h_derivatives.dx.xz,
        sample.h_derivatives.dz.xz, sample.h_derivatives.dx.zz,
        sample.h_derivatives.dz.zz, sample.h_derivatives.dx.ww,
        sample.h_derivatives.dz.ww, sample.h_derivatives.dxx.ww,
        sample.h_derivatives.dxz.ww, sample.h_derivatives.dzz.ww,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dz,
        sample.chi_derivatives.second.dxx,
        sample.chi_derivatives.second.dxz,
        sample.chi_derivatives.second.dzz);
    return PhysicalRww::compute_away_axis_physical_rww(inputs).physical_rww();
}

double reviewed_dww_alpha(const Sample &sample)
{
    const auto inputs = Lapse::make_physical_hidden_lapse_hessian_inputs(
        sample.x, sample.h.xx, sample.h.xz, sample.h.zz, sample.h.ww,
        sample.chi, sample.h_derivatives.dx.ww, sample.h_derivatives.dz.ww,
        sample.chi_derivatives.first.dx, sample.chi_derivatives.first.dz,
        sample.alpha_x, sample.alpha_z);
    return Lapse::compute_physical_hidden_lapse_hessian(inputs).dww_alpha();
}

Sample constant_cone_sample()
{
    Sample sample{};
    sample.h.ww = 4.0;
    return sample;
}

Sample nonsymmetric_sample()
{
    Sample sample{};
    sample.x = 2.0;
    sample.h = {7.0, 2.0, 5.0, 3.0};
    sample.h_derivatives.dx = {3.0, 5.0, 7.0, 6.0};
    sample.h_derivatives.dz = {4.0, 6.0, 8.0, 4.0};
    sample.h_derivatives.dxx = {0.0, 0.0, 0.0, 31.0};
    sample.h_derivatives.dxz = {0.0, 0.0, 0.0, 37.0};
    sample.h_derivatives.dzz = {0.0, 0.0, 0.0, 41.0};
    sample.chi = 5.0;
    sample.chi_derivatives.first = {3.0, 2.0};
    sample.chi_derivatives.second = {7.0, 11.0, 13.0};
    sample.alpha = 2.0;
    sample.alpha_x = 8.0;
    sample.alpha_z = 7.0;
    return sample;
}

void check_required_oracles()
{
    const auto flat = compute_core(Sample{});
    require_close("flat constant-alpha minus D_wD_w alpha",
                  flat.minus_dww_alpha(), 0.0);
    require_close("flat constant-alpha alpha R_ww", flat.alpha_rww(), 0.0);
    require_close("flat constant-alpha core", flat.curvature_lapse_core(), 0.0);

    Sample linear_alpha{};
    linear_alpha.alpha = 8.0 / 5.0;
    linear_alpha.alpha_x = 3.0 / 10.0;
    const auto linear_alpha_core = compute_core(linear_alpha);
    require_close("flat linear-alpha minus D_wD_w alpha",
                  linear_alpha_core.minus_dww_alpha(), -3.0 / 20.0);
    require_close("flat linear-alpha alpha R_ww",
                  linear_alpha_core.alpha_rww(), 0.0);
    require_close("flat linear-alpha core",
                  linear_alpha_core.curvature_lapse_core(), -3.0 / 20.0);

    auto cone = constant_cone_sample();
    cone.alpha = 2.0;
    const auto cone_core = compute_core(cone);
    require_close("constant cone minus D_wD_w alpha",
                  cone_core.minus_dww_alpha(), 0.0);
    require_close("constant cone alpha R_ww", cone_core.alpha_rww(), -1.5);
    require_close("constant cone core", cone_core.curvature_lapse_core(),
                  -1.5);

    Sample varying_chi{};
    varying_chi.chi = 6.0 / 5.0;
    varying_chi.chi_derivatives.first.dx = 1.0 / 10.0;
    varying_chi.alpha = 8.0 / 5.0;
    varying_chi.alpha_x = 3.0 / 10.0;
    const auto varying_chi_core = compute_core(varying_chi);
    require_close("varying-chi minus D_wD_w alpha",
                  varying_chi_core.minus_dww_alpha(), -11.0 / 80.0);
    require_close("varying-chi alpha R_ww", varying_chi_core.alpha_rww(),
                  11.0 / 90.0);
    require_close("varying-chi core",
                  varying_chi_core.curvature_lapse_core(), -11.0 / 720.0);
}

void check_nonsymmetric_same_point_case()
{
    const auto sample = nonsymmetric_sample();
    const auto core = compute_core(sample);
    const double expected_dww_alpha = reviewed_dww_alpha(sample);
    const double expected_physical_rww = reviewed_physical_rww(sample);

    require_close("nonsymmetric minus D_wD_w alpha",
                  core.minus_dww_alpha(), -expected_dww_alpha);
    require_close("nonsymmetric reviewed D_wD_w alpha", expected_dww_alpha,
                  699.0 / 155.0);
    require_close("nonsymmetric alpha R_ww", core.alpha_rww(),
                  sample.alpha * expected_physical_rww);
    require_close("nonsymmetric signed core", core.curvature_lapse_core(),
                  -expected_dww_alpha + sample.alpha * expected_physical_rww);
    require_close("nonsymmetric exposed-piece sum",
                  core.curvature_lapse_core(),
                  core.minus_dww_alpha() + core.alpha_rww());
}

void check_invalid_inputs()
{
    Sample sample{};
    sample.x = 0.0;
    require_throws("invalid axis", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.h.ww = 0.0;
    require_throws("invalid W", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.chi = 0.0;
    require_throws("invalid chi", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.alpha = std::numeric_limits<double>::quiet_NaN();
    require_throws("nonfinite alpha", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.h.xx = 1.0;
    sample.h.xz = 2.0;
    sample.h.zz = 1.0;
    require_throws("bad determinant", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.h_derivatives.dz.ww = std::numeric_limits<double>::infinity();
    require_throws("nonfinite W_z", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.alpha_x = std::numeric_limits<double>::quiet_NaN();
    require_throws("nonfinite alpha_x", [&]() { make_inputs(sample); });
}

} // namespace

int main()
{
    std::cout << "Stage 4AK local A_ww curvature/lapse core fixture\n";
    std::cout << "Geometric core only; no Z4 term, trace-free projection, "
                 "chi prefactor, nonlinear A/K terms, RHS, grid reads, "
                 "finite-axis support, or evolution.\n";

    check_required_oracles();
    check_nonsymmetric_same_point_case();
    check_invalid_inputs();

    std::cout << "All Stage 4AK A_ww curvature/lapse core checks passed.\n";
    return 0;
}
