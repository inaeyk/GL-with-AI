#include "CartoonTraceFreeCurvatureLapseBlock.hpp"

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
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace Block = BlackStringToy::CartoonTraceFreeCurvatureLapseBlock;
namespace PhysicalRww = BlackStringToy::CartoonAwayAxisPhysicalRww;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciBridge = BlackStringToy::CartoonRicciBridge;

constexpr double tolerance = 1.0e-10;

// Stage 4AL is the local away-axis trace-free curvature/lapse geometry block:
// source_ij = chi [C_ij - (1/4) h_ij h^kl C_kl].
// It excludes Z4 terms, nonlinear A/K terms, full RHS, grid reads, and
// evolution wiring.
struct Sample
{
    double x = 2.0;
    Algebra::ConformalMetric h{1.0, 0.0, 1.0, 1.0};
    Ricci::MetricDerivatives h_derivatives{};
    double chi = 1.0;
    Ricci::ScalarDerivatives chi_derivatives{};
    double alpha = 1.0;
    Block::ScalarJet alpha_jet{1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
};

using BlockFunction = Block::TraceFreeCurvatureLapseBlock (*)(
    const Block::TraceFreeCurvatureLapseBlockInputs &);

static_assert(!std::is_aggregate<
                  Block::TraceFreeCurvatureLapseBlockInputs>::value,
              "Stage 4AL inputs must not be an open aggregate");
static_assert(!std::is_aggregate<Block::TraceFreeCurvatureLapseBlock>::value,
              "Stage 4AL result must not be an open aggregate");
static_assert(!std::is_invocable<BlockFunction, double, double>::value,
              "Stage 4AL must not accept loose Ricci/Hessian component values");
static_assert(Block::away_axis_only, "Stage 4AL must remain away-axis only");
static_assert(!Block::z4_terms_implemented,
              "Stage 4AL must not claim Z4 terms");
static_assert(!Block::nonlinear_terms_implemented,
              "Stage 4AL must not claim nonlinear A/K terms");
static_assert(!Block::full_ccz4_rhs_implemented,
              "Stage 4AL must not claim the full CCZ4 RHS");
static_assert(!Block::grid_wiring_implemented,
              "Stage 4AL must not claim grid wiring");
static_assert(!Block::evolution_wiring_implemented,
              "Stage 4AL must not claim evolution wiring");

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

Block::TraceFreeCurvatureLapseBlockInputs make_inputs(const Sample &sample)
{
    return Block::make_trace_free_curvature_lapse_block_inputs(
        sample.x, sample.h.xx, sample.h.xz, sample.h.zz, sample.h.ww,
        sample.chi, sample.alpha, sample.h_derivatives.dx.xx,
        sample.h_derivatives.dz.xx, sample.h_derivatives.dx.xz,
        sample.h_derivatives.dz.xz, sample.h_derivatives.dx.zz,
        sample.h_derivatives.dz.zz, sample.h_derivatives.dx.ww,
        sample.h_derivatives.dz.ww, sample.h_derivatives.dxx.xx,
        sample.h_derivatives.dxx.xz, sample.h_derivatives.dxx.zz,
        sample.h_derivatives.dxx.ww, sample.h_derivatives.dxz.xx,
        sample.h_derivatives.dxz.xz, sample.h_derivatives.dxz.zz,
        sample.h_derivatives.dxz.ww, sample.h_derivatives.dzz.xx,
        sample.h_derivatives.dzz.xz, sample.h_derivatives.dzz.zz,
        sample.h_derivatives.dzz.ww, sample.chi_derivatives.first.dx,
        sample.chi_derivatives.first.dz, sample.chi_derivatives.second.dxx,
        sample.chi_derivatives.second.dxz,
        sample.chi_derivatives.second.dzz, sample.alpha_jet.dx,
        sample.alpha_jet.dz, sample.alpha_jet.dxx,
        sample.alpha_jet.dxz, sample.alpha_jet.dzz);
}

Block::TraceFreeCurvatureLapseBlock compute_block(const Sample &sample)
{
    return Block::compute_trace_free_curvature_lapse_block(make_inputs(sample));
}

double trace_free_trace(const Sample &sample,
                        const Block::TraceFreeCurvatureLapseBlock &block)
{
    const auto h_UU = Algebra::inverse(sample.h);
    const Algebra::SymmetricTensor trace_free{block.tf_xx(), block.tf_xz(),
                                              block.tf_zz(), block.tf_ww()};
    return Algebra::trace(trace_free, h_UU);
}

double stage4g_rww(const Sample &sample)
{
    Ricci::CartoonRicciInputs inputs{};
    inputs.x = sample.x;
    inputs.chi = sample.chi;
    inputs.h = sample.h;
    inputs.chi_derivatives = sample.chi_derivatives;
    inputs.h_derivatives = sample.h_derivatives;
    return RicciBridge::to_rhs_ricci_components(
               Ricci::compute_metric_derivative_ricci(inputs))
        .ww;
}

double stage4ah_rww(const Sample &sample)
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

void require_all_zero(const Block::TraceFreeCurvatureLapseBlock &block)
{
    require_close("flat core_xx", block.core_xx(), 0.0);
    require_close("flat core_xz", block.core_xz(), 0.0);
    require_close("flat core_zz", block.core_zz(), 0.0);
    require_close("flat core_ww", block.core_ww(), 0.0);
    require_close("flat full trace", block.full_trace(), 0.0);
    require_close("flat tf_xx", block.tf_xx(), 0.0);
    require_close("flat tf_xz", block.tf_xz(), 0.0);
    require_close("flat tf_zz", block.tf_zz(), 0.0);
    require_close("flat tf_ww", block.tf_ww(), 0.0);
    require_close("flat chi_tf_xx", block.chi_tf_xx(), 0.0);
    require_close("flat chi_tf_xz", block.chi_tf_xz(), 0.0);
    require_close("flat chi_tf_zz", block.chi_tf_zz(), 0.0);
    require_close("flat chi_tf_ww", block.chi_tf_ww(), 0.0);
}

Sample constant_cone_sample()
{
    Sample sample{};
    sample.h.ww = 4.0;
    sample.alpha = 2.0;
    sample.alpha_jet.value = 2.0;
    return sample;
}

Sample nonsymmetric_sample()
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
    sample.alpha = 2.0;
    sample.alpha_jet = {2.0, 8.0, 7.0, 11.0, 13.0, 17.0};
    return sample;
}

void check_simple_oracles()
{
    require_all_zero(compute_block(Sample{}));

    Sample linear_x{};
    linear_x.alpha = 8.0 / 5.0;
    linear_x.alpha_jet.value = 8.0 / 5.0;
    linear_x.alpha_jet.dx = 3.0 / 10.0;
    const auto linear_x_block = compute_block(linear_x);
    require_close("linear-x tf_xx", linear_x_block.tf_xx(), 3.0 / 40.0);
    require_close("linear-x tf_xz", linear_x_block.tf_xz(), 0.0);
    require_close("linear-x tf_zz", linear_x_block.tf_zz(), 3.0 / 40.0);
    require_close("linear-x tf_ww", linear_x_block.tf_ww(), -3.0 / 40.0);
    require_close("linear-x chi_tf_xx", linear_x_block.chi_tf_xx(),
                  3.0 / 40.0);
    require_close("linear-x chi_tf_ww", linear_x_block.chi_tf_ww(),
                  -3.0 / 40.0);

    const auto cone = compute_block(constant_cone_sample());
    require_close("constant cone core_ww", cone.core_ww(), -1.5);
    require_close("constant cone full trace", cone.full_trace(), -0.75);
    require_close("constant cone tf_xx", cone.tf_xx(), 3.0 / 16.0);
    require_close("constant cone tf_xz", cone.tf_xz(), 0.0);
    require_close("constant cone tf_zz", cone.tf_zz(), 3.0 / 16.0);
    require_close("constant cone tf_ww", cone.tf_ww(), -0.75);
}

void check_nonsymmetric_oracle()
{
    const auto sample = nonsymmetric_sample();
    const auto block = compute_block(sample);

    require_close("nonsymmetric Stage 4G/4AH Rww agreement",
                  stage4g_rww(sample), stage4ah_rww(sample));
    require_close("nonsymmetric TF trace", trace_free_trace(sample, block),
                  0.0);

    require_close("nonsymmetric core_xx", block.core_xx(),
                  -20.79847381200139);
    require_close("nonsymmetric core_xz", block.core_xz(),
                  -26.639625390218519);
    require_close("nonsymmetric core_zz", block.core_zz(),
                  -26.550102902069604);
    require_close("nonsymmetric core_ww", block.core_ww(),
                  -11.562081165452653);
    require_close("nonsymmetric full trace", block.full_trace(),
                  -13.620460168805643);
    require_close("nonsymmetric tf_xx", block.tf_xx(),
                  3.0373314834084866);
    require_close("nonsymmetric tf_xz", block.tf_xz(),
                  -19.829395305815698);
    require_close("nonsymmetric tf_zz", block.tf_zz(),
                  -9.524527691062552);
    require_close("nonsymmetric tf_ww", block.tf_ww(),
                  -1.3467360388484213);
    require_close("nonsymmetric chi_tf_xx", block.chi_tf_xx(),
                  15.186657417042433);
    require_close("nonsymmetric chi_tf_xz", block.chi_tf_xz(),
                  -99.146976529078486);
    require_close("nonsymmetric chi_tf_zz", block.chi_tf_zz(),
                  -47.62263845531276);
    require_close("nonsymmetric chi_tf_ww", block.chi_tf_ww(),
                  -6.7336801942421065);
}

void check_invalid_inputs()
{
    Sample sample{};
    sample.x = 0.0;
    require_throws("invalid axis", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.chi = 0.0;
    require_throws("invalid chi", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.h.ww = 0.0;
    require_throws("invalid W", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.h = {1.0, 2.0, 1.0, 1.0};
    require_throws("bad determinant", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.h_derivatives.dxx.xz = std::numeric_limits<double>::infinity();
    require_throws("nonfinite metric derivative",
                   [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.alpha_jet.dxz = std::numeric_limits<double>::quiet_NaN();
    require_throws("nonfinite alpha_xz", [&]() { make_inputs(sample); });

    require_throws("inconsistent inverse metric helper", []() {
        Algebra::require_inverse_metric_consistency(
            {1.0, 0.0, 1.0, 1.0}, {2.0, 0.0, 1.0, 1.0});
    });
}

} // namespace

int main()
{
    std::cout << "Stage 4AL trace-free curvature/lapse block fixture\n";
    std::cout << "Local geometry block only; no Z4 terms, nonlinear A/K terms, "
                 "full RHS, grid reads, or evolution.\n";

    check_simple_oracles();
    check_nonsymmetric_oracle();
    check_invalid_inputs();

    std::cout << "All Stage 4AL trace-free curvature/lapse checks passed.\n";
    return 0;
}
