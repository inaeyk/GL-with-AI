#include "CartoonPhysicalRwwRhsContract.hpp"

#include "CartoonRicciInterface.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>

namespace
{
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace Physical = BlackStringToy::CartoonAwayAxisPhysicalRww;
namespace Contract = BlackStringToy::CartoonPhysicalRwwRhsContract;
namespace Ricci = BlackStringToy::CartoonRicci;

constexpr double tolerance = 1.0e-12;

// Stage 4AI contracts only the hidden contribution from the reviewed Stage
// 4AH physical lower/lower R_ww[gamma]. It is not a full Ricci scalar, full
// CCZ4 RHS, grid read, finite-axis implementation, or evolution path.
struct Sample
{
    double x;
    Algebra::ConformalMetric h;
    Ricci::MetricDerivatives h_derivatives;
    double chi;
    Ricci::ScalarDerivatives chi_derivatives;
};

using ContractFunction = Contract::PhysicalRwwRhsContract (*)(
    const Physical::AwayAxisPhysicalRww &);

static_assert(!std::is_aggregate<Physical::AwayAxisPhysicalRww>::value,
              "Stage 4AH result must remain non-aggregate");
static_assert(!std::is_aggregate<Contract::PhysicalRwwRhsContract>::value,
              "Stage 4AI contract result must not be an open aggregate");
static_assert(
    !std::is_invocable<ContractFunction, double>::value,
    "Stage 4AI must not accept a loose raw physical R_ww double");
static_assert(Contract::away_axis_only,
              "Stage 4AI must remain away-axis only");
static_assert(!Contract::finite_axis_implemented,
              "Stage 4AI must not claim finite-axis support");
static_assert(!Contract::grid_hxz_parity_validated,
              "Stage 4AI must not claim actual grid h_xz parity validation");
static_assert(!Contract::grid_hxx_hww_matching_validated,
              "Stage 4AI must not claim grid diagonal matching");
static_assert(!Contract::grid_Wx_parity_validated,
              "Stage 4AI must not claim grid W_x parity");
static_assert(!Contract::grid_chix_parity_validated,
              "Stage 4AI must not claim grid chi_x parity");
static_assert(!Contract::ccz4_rhs_implemented,
              "Stage 4AI must not claim CCZ4 RHS implementation");
static_assert(!Contract::evolution_wiring_implemented,
              "Stage 4AI must not claim evolution wiring");

void fail(const std::string &label, const double value,
          const double expected)
{
    std::cerr << "FAIL " << label << ": got " << value << ", expected "
              << expected << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected)
{
    const double residual = std::abs(value - expected);
    if (residual > tolerance)
    {
        fail(label, value, expected);
    }
    std::cout << "PASS " << label << " = " << value
              << ", residual = " << residual << "\n";
}

Physical::AwayAxisPhysicalRww compute_physical(const Sample &sample)
{
    const auto inputs = Physical::make_away_axis_physical_rww_inputs(
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
    return Physical::compute_away_axis_physical_rww(inputs);
}

Contract::PhysicalRwwRhsContract compute_contract(const Sample &sample)
{
    return Contract::compute_physical_rww_rhs_contract(
        compute_physical(sample));
}

Sample flat_sample(const double chi = 1.0)
{
    Sample sample{};
    sample.x = 2.0;
    sample.h = {1.0, 0.0, 1.0, 1.0};
    sample.chi = chi;
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
    return sample;
}

void check_required_oracles()
{
    const auto flat = compute_contract(flat_sample());
    require_close("flat hidden conformal trace",
                  flat.hidden_conformal_trace(), 0.0);
    require_close("flat hidden physical scalar contribution",
                  flat.hidden_physical_scalar_contribution(), 0.0);

    auto cone_sample = flat_sample();
    cone_sample.h.ww = 4.0;
    const auto cone = compute_contract(cone_sample);
    require_close("constant cone hidden conformal trace",
                  cone.hidden_conformal_trace(), -3.0 / 8.0);
    require_close("constant cone hidden physical scalar contribution",
                  cone.hidden_physical_scalar_contribution(), -3.0 / 8.0);

    auto linear_x_sample = flat_sample(6.0 / 5.0);
    linear_x_sample.chi_derivatives.first.dx = 0.1;
    const auto linear_x = compute_contract(linear_x_sample);
    require_close("linear-x hidden conformal trace",
                  linear_x.hidden_conformal_trace(), 11.0 / 72.0);
    require_close("linear-x hidden physical scalar contribution",
                  linear_x.hidden_physical_scalar_contribution(),
                  11.0 / 60.0);
}

void check_nonsymmetric_contract()
{
    const auto sample = nonsymmetric_sample();
    const auto physical = compute_physical(sample);
    const auto contract =
        Contract::compute_physical_rww_rhs_contract(physical);

    require_close("nonsymmetric hidden conformal trace",
                  contract.hidden_conformal_trace(),
                  (2.0 / sample.h.ww) * physical.physical_rww());
    require_close("nonsymmetric hidden physical scalar contribution",
                  contract.hidden_physical_scalar_contribution(),
                  sample.chi * (2.0 / sample.h.ww) *
                      physical.physical_rww());
}

} // namespace

int main()
{
    std::cout << "Stage 4AI local physical Rww Ricci/RHS contract fixture\n";
    std::cout << "Hidden contribution contract only; no full Ricci scalar, "
                 "RHS, grid reads, finite-axis support, or evolution.\n";

    check_required_oracles();
    check_nonsymmetric_contract();

    std::cout << "All Stage 4AI physical Rww contract checks passed.\n";
    return 0;
}
