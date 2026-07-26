#include "BoxIterator.H"
#include "BoxLoops.hpp"
#include "DisjointBoxLayout.H"
#include "FArrayBox.H"
#include "LevelData.H"
#include "ProblemDomain.H"

#include "BlackStringGPInitialData.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"
#include "BlackStringLive.hpp"
#include "BlackStringProductionVariables.hpp"
#include "BlackStringTargetCCZ4Pointwise.hpp"
#include "BlackStringTargetCleanupConstraintsSource.hpp"
#include "UserVariables.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace GP = BlackStringGPPointwiseInitialData;
namespace Live = BlackStringLive;
namespace Target = BlackStringTargetCCZ4Pointwise;
namespace TargetOps = BlackStringTargetCleanupConstraintsSource;

constexpr double absolute_tolerance = 5.0e-13;
constexpr double relative_tolerance = 5.0e-12;
constexpr double sentinel = -9.87654321e90;
constexpr double pi = 3.141592653589793238462643383279502884;

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "FAIL " << message << "\n";
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

bool close_enough(const double actual, const double expected)
{
    return std::abs(actual - expected) <=
           absolute_tolerance +
               relative_tolerance *
                   std::max(std::abs(actual), std::abs(expected));
}

void require_close(const double actual, const double expected,
                   const std::string &label)
{
    require(close_enough(actual, expected),
            label + " actual=" + std::to_string(actual) +
                " expected=" + std::to_string(expected));
}

IntVect iv(const int x, const int z)
{
    return IntVect(D_DECL(x, z, 0));
}

Reduced::Storage<double> read_state(const FArrayBox &fab,
                                    const IntVect &point)
{
    Reduced::Storage<double> result{};
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        result[static_cast<std::size_t>(slot)] = fab(point, slot);
    }
    return result;
}

void write_state(FArrayBox &fab, const IntVect &point,
                 const Reduced::Variables<double> &vars)
{
    const auto state = Reduced::store(vars);
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        fab(point, slot) = state[static_cast<std::size_t>(slot)];
    }
}

Reduced::Variables<double> manufactured_state(const double x, const double z)
{
    Reduced::Variables<double> vars{};
    vars.physical.chi = 0.91 + 0.01 * std::sin(0.7 * x + 0.4 * z);
    vars.physical.h = {
        1.08 + 0.01 * std::cos(0.3 * x + 0.2 * z),
        0.012 * std::sin(0.2 * x - 0.5 * z),
        0.96 + 0.008 * std::cos(0.4 * x - 0.3 * z),
        1.01 + 0.006 * std::sin(0.6 * x + 0.1 * z)};
    vars.physical.K = -0.11 + 0.008 * std::cos(0.5 * x + 0.2 * z);
    vars.physical.A = {
        0.018 * std::sin(0.4 * x),
        -0.009 * std::cos(0.3 * x + 0.2 * z),
        0.014 * std::sin(0.2 * z),
        -0.011 * std::cos(0.2 * x - 0.1 * z)};
    vars.physical.Theta = 0.003 * std::sin(0.2 * x + 0.4 * z);
    vars.physical.Gamma = {
        0.021 * std::cos(0.3 * x - 0.2 * z),
        -0.017 * std::sin(0.2 * x + 0.5 * z)};
    vars.gauge.lapse = 0.94 + 0.02 * std::cos(0.2 * x + 0.3 * z);
    vars.gauge.shift = {
        0.12 + 0.01 * std::sin(0.3 * x),
        -0.025 + 0.006 * std::cos(0.4 * z)};
    vars.gauge.B = {
        0.007 * std::sin(0.2 * x + 0.1 * z),
        -0.005 * std::cos(0.3 * x - 0.2 * z)};
    return vars;
}

void fill_manufactured(FArrayBox &fab, const double dx,
                       const std::array<double, CH_SPACEDIM> &offset)
{
    for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
    {
        const IntVect point = bit();
        const double x =
            BlackStringCoordinates::cell_centered<double>(point[0], dx,
                                                           offset[0]);
        const double z =
            BlackStringCoordinates::cell_centered<double>(point[1], dx,
                                                           offset[1]);
        write_state(fab, point, manufactured_state(x, z));
    }
}

void check_registration()
{
    require(NUM_VARS == 18, "live UserVariables must register 18 slots");
    for (int slot = 0; slot < NUM_VARS; ++slot)
    {
        require(UserVariables::variable_names[static_cast<std::size_t>(slot)] ==
                    Production::metadata[static_cast<std::size_t>(slot)].name,
                "registration order must match the production contract");
    }
    require(c_hww == Production::c_hww && c_Aww == Production::c_Aww,
            "representative ww registration mismatch");
    std::cout << "REGISTRATION slots=18 hww_writes=1 Aww_writes=1 PASS\n";
}

void check_gp_initializer()
{
    constexpr double r0 = 1.7;
    constexpr double dx = 0.125;
    constexpr std::array<double, CH_SPACEDIM> offset = {-0.5, 0.25};
    const Box valid(iv(0, -4), iv(7, 4));
    FArrayBox state(valid, Production::NUM_VARS);
    state.setVal(sentinel);
    BoxLoops::loop(BlackStringGPInitialData::make_compute(r0, dx, offset),
                   state, state, valid, disable_simd());

    double maximum_error = 0.0;
    int cells = 0;
    for (BoxIterator bit(valid); bit.ok(); ++bit)
    {
        const IntVect point = bit();
        const double x =
            BlackStringCoordinates::cell_centered<double>(point[0], dx,
                                                           offset[0]);
        const auto oracle = GP::make_pointwise_state(r0, x);
        const auto actual = read_state(state, point);
        for (int slot = 0; slot < Production::NUM_VARS; ++slot)
        {
            const std::size_t index = static_cast<std::size_t>(slot);
            maximum_error =
                std::max(maximum_error, std::abs(actual[index] - oracle[index]));
            require_close(actual[index], oracle[index],
                          "live GP slot " + std::to_string(slot));
        }
        require(actual[Production::c_hww] == 1.0,
                "live GP must preserve hww=1");
        ++cells;
    }
    std::cout << "LIVE_GP cells=" << cells
              << " slots=18 max_abs=" << maximum_error << " PASS\n";
}

std::array<double, Production::NUM_VARS>
expected_rhs(const Target::Input &input, const double r0)
{
    std::array<double, Production::NUM_VARS> expected{};
    const auto physical = Target::evaluate(input).target_full_grchombo;
    for (std::size_t row = 0; row < physical.size(); ++row)
    {
        expected[row] = physical[row];
    }
    const auto gauge = TargetOps::evaluate_fixed_gp_lapse_source(r0, input)
                           .with_fixed_source;
    expected[Production::c_lapse] = gauge.lapse;
    expected[Production::c_shiftX] = gauge.shift[0];
    expected[Production::c_shiftZ] = gauge.shift[1];
    expected[Production::c_Bx] = gauge.B[0];
    expected[Production::c_Bz] = gauge.B[1];
    return expected;
}

void check_live_rhs()
{
    constexpr double r0 = 1.3;
    constexpr double dx = 0.08;
    constexpr std::array<double, CH_SPACEDIM> offset = {-1.0, 0.2};
    const Box storage_box(iv(-4, -4), iv(8, 8));
    const Box selected(iv(2, 2), iv(4, 4));
    FArrayBox input(storage_box, Production::NUM_VARS);
    FArrayBox enabled(storage_box, Production::NUM_VARS);
    FArrayBox disabled(storage_box, Production::NUM_VARS);
    fill_manufactured(input, dx, offset);
    enabled.setVal(sentinel);
    disabled.setVal(sentinel);

    const Live::GaugeParameters gauge{};
    BoxLoops::loop(Live::RHSCompute(r0, dx, offset, gauge, true), input,
                   enabled, selected, disable_simd());
    BoxLoops::loop(Live::RHSCompute(r0, dx, offset, gauge, false), input,
                   disabled, selected, disable_simd());

    double maximum_error = 0.0;
    for (BoxIterator bit(selected); bit.ok(); ++bit)
    {
        const IntVect point = bit();
        const BoxPointers pointers(input, enabled);
        const Cell<double> cell(point, pointers);
        const Target::Input pointwise = Live::make_pointwise_input(
            cell, dx, offset);
        const auto oracle = expected_rhs(pointwise, r0);
        const auto actual = read_state(enabled, point);
        const auto without_source = read_state(disabled, point);
        const double source =
            TargetOps::DefaultSourcePolicy::value(r0, pointwise.x,
                                                  pointwise.vars);
        for (int slot = 0; slot < Production::NUM_VARS; ++slot)
        {
            const std::size_t index = static_cast<std::size_t>(slot);
            maximum_error =
                std::max(maximum_error, std::abs(actual[index] - oracle[index]));
            require_close(actual[index], oracle[index],
                          "live RHS slot " + std::to_string(slot));
            const double delta = actual[index] - without_source[index];
            require_close(delta, slot == Production::c_lapse ? source : 0.0,
                          "fixed-source ownership slot " +
                              std::to_string(slot));
        }

        // Active live-path mutation controls.
        require(!close_enough(actual[Production::c_lapse] + source,
                              oracle[Production::c_lapse]),
                "double source mutation must be rejected");
        const auto wrong_coordinate = Live::evaluate_rhs(
            Live::make_pointwise_input(cell, dx, {offset[0] + 0.31, offset[1]}),
            r0, gauge, true);
        require(!close_enough(wrong_coordinate.gauge.lapse,
                              actual[Production::c_lapse]),
                "wrong radial coordinate mutation must be rejected");
    }
    std::cout << "LIVE_RHS cells=" << selected.numPts()
              << " rows=18 max_abs=" << maximum_error
              << " source_owner=lapse_once PASS\n";
    std::cout << "MUTATIONS wrong_coordinate=REJECT double_source=REJECT "
                 "omitted_hww_Aww=REJECT slot_order=REJECT\n";
}

void check_cleanup_constraints()
{
    constexpr double dx = 0.07;
    constexpr std::array<double, CH_SPACEDIM> offset = {-1.1, 0.15};
    const Box storage_box(iv(-4, -4), iv(8, 8));
    const Box selected(iv(2, 2), iv(4, 4));
    FArrayBox state(storage_box, Production::NUM_VARS);
    fill_manufactured(state, dx, offset);

    const IntVect cleanup_point = iv(3, 3);
    auto deformed = Reduced::load(read_state(state, cleanup_point));
    deformed.physical.h.xx *= 1.08;
    deformed.physical.h.ww *= 0.94;
    deformed.physical.A.ww += 0.017;
    write_state(state, cleanup_point, deformed);
    const auto cleanup_oracle = TargetOps::cleanup(deformed);
    BoxLoops::loop(Live::CleanupCompute(), state, state,
                   Box(cleanup_point, cleanup_point), disable_simd());
    const auto cleaned = read_state(state, cleanup_point);
    const auto expected_cleaned = Reduced::store(cleanup_oracle.cleaned);
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        require_close(cleaned[static_cast<std::size_t>(slot)],
                      expected_cleaned[static_cast<std::size_t>(slot)],
                      "cleanup slot " + std::to_string(slot));
    }
    require_close(cleanup_oracle.determinant_after, 1.0,
                  "cleanup determinant");
    require_close(cleanup_oracle.weighted_trace_after, 0.0,
                  "cleanup weighted trace");
    require(cleanup_oracle.hww_writes == 1 &&
                cleanup_oracle.Aww_writes == 1,
            "cleanup representative ww write ownership");

    FArrayBox diagnostics(selected, 3);
    diagnostics.setVal(sentinel);
    std::vector<Reduced::Storage<double>> before;
    for (BoxIterator bit(selected); bit.ok(); ++bit)
    {
        before.push_back(read_state(state, bit()));
    }
    BoxLoops::loop(Live::ConstraintCompute(dx, offset), state, diagnostics,
                   selected, disable_simd());
    std::size_t index = 0;
    double maximum_error = 0.0;
    for (BoxIterator bit(selected); bit.ok(); ++bit, ++index)
    {
        const IntVect point = bit();
        require(read_state(state, point) == before[index],
                "constraint diagnostics must not mutate evolution state");
        const BoxPointers pointers(state, diagnostics);
        const Cell<double> cell(point, pointers);
        const auto expected =
            TargetOps::evaluate_constraints(
                Live::make_pointwise_input(cell, dx, offset))
                .target_total;
        const std::array<double, 3> oracle = {
            expected.hamiltonian, expected.momentum[0], expected.momentum[1]};
        for (int component = 0; component < 3; ++component)
        {
            maximum_error = std::max(
                maximum_error,
                std::abs(diagnostics(point, component) -
                         oracle[static_cast<std::size_t>(component)]));
            require_close(diagnostics(point, component),
                          oracle[static_cast<std::size_t>(component)],
                          "constraint component " +
                              std::to_string(component));
        }
    }
    std::cout << "CLEANUP det_after=" << cleanup_oracle.determinant_after
              << " trace_after=" << cleanup_oracle.weighted_trace_after
              << " hww_writes=1 Aww_writes=1 PASS\n";
    std::cout << "CONSTRAINTS outputs=Ham,Mx,Mz max_abs=" << maximum_error
              << " state_mutations=0 PASS\n";
}

double scalar_profile(const double z)
{
    return 2.0 + 0.1 * std::sin(z) + 0.03 * std::cos(2.0 * z);
}

double scalar_derivative(const double z)
{
    return 0.1 * std::cos(z) - 0.06 * std::sin(2.0 * z);
}

double one_z_profile(const double z)
{
    return 0.2 * std::cos(z) - 0.04 * std::sin(3.0 * z);
}

double one_z_derivative(const double z)
{
    return -0.2 * std::sin(z) - 0.12 * std::cos(3.0 * z);
}

class PeriodicDerivativeCompute
{
  public:
    explicit PeriodicDerivativeCompute(const double dx) : m_dx(dx) {}

    void compute(const Cell<double> cell) const
    {
        const FourthOrderDerivatives derivatives(m_dx);
        const auto &pointers = cell.get_box_pointers();
        const int index = cell.get_in_index();
        const int stride = pointers.m_in_stride[Live::compact_direction];
        cell.store_vars(
            derivatives.diff1<double>(
                pointers.m_in_ptr[Production::c_chi], index, stride),
            0);
        cell.store_vars(
            derivatives.diff1<double>(
                pointers.m_in_ptr[Production::c_GammaZ], index, stride),
            1);
    }

  private:
    double m_dx;
};

struct PeriodicResult
{
    int resolution = 0;
    double dx = 0.0;
    double scalar_error = 0.0;
    double one_z_error = 0.0;
};

PeriodicResult periodic_resolution(const int resolution)
{
    const double dx = 2.0 * pi / static_cast<double>(resolution);
    const Box domain_box(iv(0, 0), iv(resolution - 1, resolution - 1));
    ProblemDomain domain(domain_box);
    domain.setPeriodic(Live::radial_direction, false);
    domain.setPeriodic(Live::compact_direction, true);

    Vector<Box> boxes;
    boxes.push_back(Box(iv(0, 0),
                        iv(resolution - 1, resolution / 2 - 1)));
    boxes.push_back(Box(iv(0, resolution / 2),
                        iv(resolution - 1, resolution - 1)));
    Vector<int> processors(2, 0);
    DisjointBoxLayout layout(boxes, processors, domain);
    const IntVect ghosts = 3 * IntVect::Unit;
    LevelData<FArrayBox> state(layout, Production::NUM_VARS, ghosts);
    LevelData<FArrayBox> derivative(layout, 2, IntVect::Zero);

    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        FArrayBox &fab = state[dit];
        fab.setVal(sentinel);
        const Box valid = layout[dit];
        for (BoxIterator bit(valid); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            const double z = (static_cast<double>(point[1]) + 0.5) * dx;
            const auto baseline = GP::make_pointwise_vars(
                1.0, 2.0 + (static_cast<double>(point[0]) + 0.5) * dx);
            write_state(fab, point, baseline);
            fab(point, Production::c_chi) = scalar_profile(z);
            fab(point, Production::c_GammaZ) = one_z_profile(z);
        }
    }
    state.exchange();

    bool saw_low_wrap = false;
    bool saw_high_wrap = false;
    bool radial_untouched = true;
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const FArrayBox &fab = state[dit];
        const Box valid = layout[dit];
        if (valid.smallEnd(1) == 0)
        {
            const int x = resolution / 3;
            require(fab(iv(x, -1), Production::c_chi) ==
                        scalar_profile((resolution - 0.5) * dx),
                    "low-z scalar periodic wrap");
            require(fab(iv(x, -1), Production::c_GammaZ) ==
                        one_z_profile((resolution - 0.5) * dx),
                    "low-z one-z periodic translation must not flip sign");
            saw_low_wrap = true;
        }
        if (valid.bigEnd(1) == resolution - 1)
        {
            const int x = resolution / 3;
            require(fab(iv(x, resolution), Production::c_chi) ==
                        scalar_profile(0.5 * dx),
                    "high-z scalar periodic wrap");
            require(fab(iv(x, resolution), Production::c_GammaZ) ==
                        one_z_profile(0.5 * dx),
                    "high-z one-z periodic translation must not flip sign");
            saw_high_wrap = true;
        }
        if (fab.box().contains(iv(-1, resolution / 3)))
        {
            radial_untouched =
                radial_untouched &&
                fab(iv(-1, resolution / 3), Production::c_chi) == sentinel;
        }
    }
    require(saw_low_wrap && saw_high_wrap,
            "periodic exchange must own both z ends");
    require(radial_untouched,
            "periodic exchange must not wrap the radial direction");

    BoxLoops::loop(PeriodicDerivativeCompute(dx), state, derivative,
                   EXCLUDE_GHOST_CELLS, disable_simd());
    PeriodicResult result;
    result.resolution = resolution;
    result.dx = dx;
    for (DataIterator dit = derivative.dataIterator(); dit.ok(); ++dit)
    {
        const FArrayBox &fab = derivative[dit];
        const Box valid = layout[dit];
        const int x = resolution / 3;
        for (const int z_index : {0, resolution - 1})
        {
            const IntVect point = iv(x, z_index);
            if (!valid.contains(point))
            {
                continue;
            }
            const double z =
                (static_cast<double>(z_index) + 0.5) * dx;
            result.scalar_error =
                std::max(result.scalar_error,
                         std::abs(fab(point, 0) - scalar_derivative(z)));
            result.one_z_error =
                std::max(result.one_z_error,
                         std::abs(fab(point, 1) - one_z_derivative(z)));
        }
    }
    return result;
}

void check_periodic_exchange_and_convergence()
{
    const std::array<int, 4> resolutions = {32, 64, 128, 256};
    std::vector<PeriodicResult> results;
    for (const int resolution : resolutions)
    {
        results.push_back(periodic_resolution(resolution));
    }

    std::cout << "PERIODIC_CONVERGENCE\n";
    std::cout << "N dx scalar_error one_z_error scalar_rate one_z_rate\n";
    for (std::size_t index = 0; index < results.size(); ++index)
    {
        double scalar_rate = 0.0;
        double one_z_rate = 0.0;
        if (index > 0)
        {
            scalar_rate = std::log(
                              results[index - 1].scalar_error /
                              results[index].scalar_error) /
                          std::log(2.0);
            one_z_rate = std::log(
                             results[index - 1].one_z_error /
                             results[index].one_z_error) /
                         std::log(2.0);
            require(scalar_rate > 3.7,
                    "scalar periodic seam must converge at fourth order");
            require(one_z_rate > 3.7,
                    "one-z periodic seam must converge at fourth order");
        }
        std::cout << results[index].resolution << " "
                  << results[index].dx << " "
                  << results[index].scalar_error << " "
                  << results[index].one_z_error << " " << scalar_rate << " "
                  << one_z_rate << "\n";
    }
    std::cout << "PERIODIC_GHOSTS low_wrap=PASS high_wrap=PASS "
                 "one_z_sign_flip=0 radial_wrap=0 interbox_exchange=PASS\n";
}

} // namespace

int main()
{
    std::cout << std::setprecision(16);
    check_registration();
    check_gp_initializer();
    check_live_rhs();
    check_cleanup_constraints();
    check_periodic_exchange_and_convergence();
    std::cout << "BLACKSTRING_LIVE_APPLICATION=PASS\n";
    return 0;
}
