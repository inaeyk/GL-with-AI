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
#include <type_traits>
#include <utility>
#include <vector>

namespace
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace GP = BlackStringGPPointwiseInitialData;
namespace Live = BlackStringLive;
namespace Target = BlackStringTargetCCZ4Pointwise;
namespace TargetOps = BlackStringTargetCleanupConstraintsSource;
namespace CellStorage = BlackStringCellStorage;

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

const char *parity_name(const Production::ParityClass parity)
{
    switch (parity)
    {
    case Production::ParityClass::scalar_even:
        return "scalar_even";
    case Production::ParityClass::one_z_opposite:
        return "one_z";
    case Production::ParityClass::gauge_even:
        return "gauge_even";
    }
    return "unknown";
}

struct WorstError
{
    double error = 0.0;
    int row = -1;
    IntVect point = IntVect::Zero;
    double x = 0.0;
    double z = 0.0;
    const char *parity = "none";
};

void retain_worst(WorstError &worst, const double error, const int row,
                  const IntVect &point, const double x, const double z,
                  const char *parity)
{
    if (error > worst.error)
    {
        worst = {error, row, point, x, z, parity};
    }
}

void print_worst(const char *family, const int resolution,
                 const WorstError &worst)
{
    std::cout << "MANUFACTURED_WORST family=" << family
              << " N=" << resolution << " error=" << worst.error
              << " row=" << worst.row << " iv=(" << worst.point[0] << ","
              << worst.point[1] << ") x=" << worst.x << " z=" << worst.z
              << " parity=" << worst.parity << "\n";
}

struct AnalyticJet
{
    double value = 0.0;
    double dx = 0.0;
    double dz = 0.0;
    double dxx = 0.0;
    double dxz = 0.0;
    double dzz = 0.0;
};

AnalyticJet mode(const double offset, const double amplitude,
                 const double kx, const double kz, const double x,
                 const double z, const bool sine)
{
    const double phase = kx * x + kz * z;
    const double primary = sine ? std::sin(phase) : std::cos(phase);
    const double first = sine ? std::cos(phase) : -std::sin(phase);
    return {offset + amplitude * primary,
            amplitude * kx * first,
            amplitude * kz * first,
            -amplitude * kx * kx * primary,
            -amplitude * kx * kz * primary,
            -amplitude * kz * kz * primary};
}

void set_slot(Target::Input &input, const Production::VariableSlot slot,
              const AnalyticJet &analytic)
{
    Reduced::component(input.vars, slot) = analytic.value;
    input.derivatives[static_cast<std::size_t>(slot)] = {
        analytic.dx, analytic.dz, analytic.dxx, analytic.dxz, analytic.dzz};
}

Target::Input manufactured_input(const double x, const double z)
{
    Target::Input input{};
    input.x = x;
    set_slot(input, Production::c_chi,
             mode(0.91, 0.01, 1.7, 1.4, x, z, true));
    set_slot(input, Production::c_hxx,
             mode(1.08, 0.01, 1.3, 1.2, x, z, false));
    set_slot(input, Production::c_hxz,
             mode(0.0, 0.012, 1.2, -1.5, x, z, true));
    set_slot(input, Production::c_hzz,
             mode(0.96, 0.008, 1.4, -1.3, x, z, false));
    set_slot(input, Production::c_hww,
             mode(1.01, 0.006, 1.6, 1.1, x, z, true));
    set_slot(input, Production::c_K,
             mode(-0.11, 0.008, 1.5, 1.2, x, z, false));
    set_slot(input, Production::c_Axx,
             mode(0.0, 0.018, 1.4, 0.0, x, z, true));
    set_slot(input, Production::c_Axz,
             mode(0.0, -0.009, 1.3, 1.2, x, z, false));
    set_slot(input, Production::c_Azz,
             mode(0.0, 0.014, 0.0, 1.2, x, z, true));
    set_slot(input, Production::c_Aww,
             mode(0.0, -0.011, 1.2, -1.1, x, z, false));
    set_slot(input, Production::c_Theta,
             mode(0.0, 0.003, 1.2, 1.4, x, z, true));
    set_slot(input, Production::c_GammaX,
             mode(0.0, 0.021, 1.3, -1.2, x, z, false));
    set_slot(input, Production::c_GammaZ,
             mode(0.0, -0.017, 1.2, 1.5, x, z, true));
    set_slot(input, Production::c_lapse,
             mode(0.94, 0.02, 1.1, 0.9, x, z, false));
    set_slot(input, Production::c_shiftX,
             mode(0.12, 0.01, 1.3, 0.0, x, z, true));
    set_slot(input, Production::c_shiftZ,
             mode(-0.025, 0.006, 0.0, 1.4, x, z, false));
    set_slot(input, Production::c_Bx,
             mode(0.0, 0.007, 1.2, 1.1, x, z, true));
    set_slot(input, Production::c_Bz,
             mode(0.0, -0.005, 1.3, -1.2, x, z, false));
    return input;
}

Reduced::Variables<double> manufactured_state(const double x, const double z)
{
    return manufactured_input(x, z).vars;
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

struct CountingStoragePolicy
{
    Box valid;
    std::vector<int> *counts;

    void store(const Cell<double> cell,
               const CellStorage::Variables &vars) const
    {
        CellStorage::store(cell, vars);
        const IntVect point = cell.get_int_vect();
        const int width = valid.size(0);
        const int index = point[0] - valid.smallEnd(0) +
                          width * (point[1] - valid.smallEnd(1));
        ++counts->at(static_cast<std::size_t>(index));
    }
};

struct BypassingHwwStoragePolicy
{
    double dx;
    std::array<double, CH_SPACEDIM> offset;

    void store(const Cell<double> cell,
               const CellStorage::Variables &vars) const
    {
        CellStorage::store(cell, vars);
        const double x = BlackStringCoordinates::cell_centered<double>(
            cell.get_int_vect()[Live::radial_direction], dx,
            offset[Live::radial_direction]);
        cell.store_vars(x * x, Production::c_hww);
    }
};

void check_gp_initializer()
{
    constexpr double r0 = 1.7;
    constexpr double dx = 0.125;
    constexpr std::array<double, CH_SPACEDIM> offset = {-0.5, 0.25};
    const Box valid(iv(0, -4), iv(7, 4));
    FArrayBox state(valid, Production::NUM_VARS);
    state.setVal(sentinel);
    std::vector<int> writes(static_cast<std::size_t>(valid.numPts()), 0);
    BoxLoops::loop(
        BlackStringGPInitialData::make_compute_with_storage_policy(
            r0, dx, offset, CountingStoragePolicy{valid, &writes}),
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
    require(std::all_of(writes.begin(), writes.end(),
                        [](const int count) { return count == 1; }),
            "live GP initializer must store every valid cell exactly once");
    FArrayBox bypassed(valid, Production::NUM_VARS);
    bypassed.setVal(sentinel);
    BoxLoops::loop(
        BlackStringGPInitialData::make_compute_with_storage_policy(
            r0, dx, offset, BypassingHwwStoragePolicy{dx, offset}),
        bypassed, bypassed, valid, disable_simd());
    const IntVect mutation_point = iv(3, 0);
    require(!close_enough(bypassed(mutation_point, Production::c_hww),
                          state(mutation_point, Production::c_hww)),
            "initializer storage bypass with hww=x^2 must be rejected");
    std::cout << "LIVE_GP cells=" << cells
              << " slots=18 writes_per_cell=1 max_abs=" << maximum_error
              << " PASS\n";
    std::cout << "MUTATIONS initializer_storage_bypass=REJECT "
                 "hww_x_squared=REJECT\n";
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

struct OneHiddenCopyExpansion
{
    Target::ExpandedInput expand(const Target::Input &input) const
    {
        auto expanded = Target::expand_target(input);
        constexpr int removed = Target::second_hidden_direction;
        expanded.vars.h[removed][removed] = 1.0;
        expanded.vars.A[removed][removed] = 0.0;
        expanded.vars.Gamma[removed] = 0.0;
        expanded.vars.shift[removed] = 0.0;
        expanded.vars.B[removed] = 0.0;
        for (int i = 0; i < Target::target_dimension; ++i)
        {
            expanded.d1.chi[removed] = 0.0;
            expanded.d1.lapse[removed] = 0.0;
            expanded.d1.K[removed] = 0.0;
            expanded.d1.Theta[removed] = 0.0;
            expanded.d1.Gamma[i][removed] =
                expanded.d1.Gamma[removed][i] = 0.0;
            expanded.d1.shift[i][removed] =
                expanded.d1.shift[removed][i] = 0.0;
            expanded.d1.B[i][removed] =
                expanded.d1.B[removed][i] = 0.0;
            for (int j = 0; j < Target::target_dimension; ++j)
            {
                expanded.d2.chi[i][removed] =
                    expanded.d2.chi[removed][i] = 0.0;
                expanded.d2.lapse[i][removed] =
                    expanded.d2.lapse[removed][i] = 0.0;
                for (int k = 0; k < Target::target_dimension; ++k)
                {
                    if (i == removed || j == removed || k == removed)
                    {
                        expanded.d1.h[i][j][k] = 0.0;
                        expanded.d1.A[i][j][k] = 0.0;
                    }
                    expanded.d2.shift[i][j][removed] =
                        expanded.d2.shift[i][removed][j] = 0.0;
                    for (int l = 0; l < Target::target_dimension; ++l)
                    {
                        if (i == removed || j == removed ||
                            k == removed || l == removed)
                        {
                            expanded.d2.h[i][j][k][l] = 0.0;
                        }
                    }
                }
            }
        }
        expanded.advection.h[removed][removed] = 0.0;
        expanded.advection.A[removed][removed] = 0.0;
        return expanded;
    }
};

struct SwappedRegistrationInputPolicy
{
    int *calls;

    Target::Input
    operator()(const Cell<double> &cell, const double dx,
               const std::array<double, CH_SPACEDIM> &offset) const
    {
        ++(*calls);
        CellStorage::require_input_component_count(cell);
        const auto coordinates =
            BlackStringGPInitialData::cell_centered_coordinates(
                cell.get_int_vect(), dx, offset);
        Target::Input input{};
        input.x = coordinates.x;
        const FourthOrderDerivatives derivatives(dx);
        const auto &pointers = cell.get_box_pointers();
        const int index = cell.get_in_index();
        const int x_stride = pointers.m_in_stride[Live::radial_direction];
        const int z_stride = pointers.m_in_stride[Live::compact_direction];
        for (int logical = 0; logical < Production::NUM_VARS; ++logical)
        {
            int registered = logical;
            if (logical == Production::c_chi)
            {
                registered = Production::c_hxx;
            }
            else if (logical == Production::c_hxx)
            {
                registered = Production::c_chi;
            }
            const auto logical_slot =
                static_cast<Production::VariableSlot>(logical);
            Reduced::component(input.vars, logical_slot) =
                cell.load_vars(registered);
            const double *values = pointers.m_in_ptr[registered];
            Target::Jet &jet =
                input.derivatives[static_cast<std::size_t>(logical)];
            jet.dx = derivatives.diff1<double>(values, index, x_stride);
            jet.dz = derivatives.diff1<double>(values, index, z_stride);
            jet.dxx = derivatives.diff2<double>(values, index, x_stride);
            jet.dxz = derivatives.mixed_diff2<double>(
                values, index, x_stride, z_stride);
            jet.dzz = derivatives.diff2<double>(values, index, z_stride);
        }
        return input;
    }
};

struct OmitSourceEvaluationPolicy
{
    int *calls;

    Reduced::Variables<double>
    operator()(const Target::Input &input, const double r0,
               const Live::GaugeParameters &gauge,
               const bool source_enabled) const
    {
        ++(*calls);
        require(source_enabled, "omitted-source policy must replace enabled path");
        return Live::evaluate_rhs(input, r0, gauge, false);
    }
};

struct DoubleSourceEvaluationPolicy
{
    int *calls;

    Reduced::Variables<double>
    operator()(const Target::Input &input, const double r0,
               const Live::GaugeParameters &gauge,
               const bool source_enabled) const
    {
        ++(*calls);
        require(source_enabled, "double-source policy must replace enabled path");
        auto rhs = Live::evaluate_rhs(input, r0, gauge, true);
        rhs.gauge.lapse +=
            TargetOps::DefaultSourcePolicy::value(r0, input.x, input.vars);
        return rhs;
    }
};

struct ShiftSourceEvaluationPolicy
{
    int *calls;

    Reduced::Variables<double>
    operator()(const Target::Input &input, const double r0,
               const Live::GaugeParameters &gauge,
               const bool source_enabled) const
    {
        ++(*calls);
        require(source_enabled, "shift-source policy must replace enabled path");
        auto rhs = Live::evaluate_rhs(input, r0, gauge, false);
        rhs.gauge.shift.x +=
            TargetOps::DefaultSourcePolicy::value(r0, input.x, input.vars);
        return rhs;
    }
};

struct BSourceEvaluationPolicy
{
    int *calls;

    Reduced::Variables<double>
    operator()(const Target::Input &input, const double r0,
               const Live::GaugeParameters &gauge,
               const bool source_enabled) const
    {
        ++(*calls);
        require(source_enabled, "B-source policy must replace enabled path");
        auto rhs = Live::evaluate_rhs(input, r0, gauge, false);
        rhs.gauge.B.z +=
            TargetOps::DefaultSourcePolicy::value(r0, input.x, input.vars);
        return rhs;
    }
};

enum class WriteMutation
{
    omit_hww,
    omit_Aww,
    duplicate_hww,
    duplicate_K
};

struct CountingMutatedRHSStoragePolicy
{
    std::array<int, Production::NUM_VARS> *counts;
    int *calls;
    WriteMutation mutation;

    void store(const Cell<double> &cell,
               const Reduced::Variables<double> &rhs) const
    {
        ++(*calls);
        CellStorage::require_output_component_count(cell);
        const auto write = [&](const int row) {
            const auto slot = static_cast<Production::VariableSlot>(row);
            cell.store_vars(Reduced::component(rhs, slot), row);
            ++counts->at(static_cast<std::size_t>(row));
        };
        for (int row = 0; row < Production::NUM_VARS; ++row)
        {
            if ((mutation == WriteMutation::omit_hww &&
                 row == Production::c_hww) ||
                (mutation == WriteMutation::omit_Aww &&
                 row == Production::c_Aww))
            {
                continue;
            }
            write(row);
        }
        if (mutation == WriteMutation::duplicate_hww)
        {
            write(Production::c_hww);
        }
        if (mutation == WriteMutation::duplicate_K)
        {
            write(Production::c_K);
        }
    }
};

enum class HookEvent
{
    rhs,
    update,
    cleanup_inside_rhs,
    cleanup_after_update
};

struct TracedEvaluationPolicy
{
    std::vector<HookEvent> *events;

    Reduced::Variables<double>
    operator()(const Target::Input &input, const double r0,
               const Live::GaugeParameters &gauge,
               const bool source_enabled) const
    {
        events->push_back(HookEvent::rhs);
        return Live::evaluate_rhs(input, r0, gauge, source_enabled);
    }
};

struct CleanupInsideRHSPreStorePolicy
{
    std::vector<HookEvent> *events;
    int *rejections;

    void operator()(Reduced::Variables<double> &rhs) const
    {
        events->push_back(HookEvent::cleanup_inside_rhs);
        try
        {
            rhs = TargetOps::cleanup(rhs).cleaned;
        }
        catch (const std::domain_error &)
        {
            ++(*rejections);
            rhs.physical.chi = sentinel;
        }
    }
};

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

        const auto wrong_coordinate = Live::evaluate_rhs(
            Live::make_pointwise_input(cell, dx, {offset[0] + 0.31, offset[1]}),
            r0, gauge, true);
        require(!close_enough(wrong_coordinate.gauge.lapse,
                              actual[Production::c_lapse]),
                "wrong radial coordinate mutation must be rejected");
        const auto one_hidden = Target::evaluate_with_expansion_policy(
            pointwise, OneHiddenCopyExpansion{});
        bool hidden_copy_differs = false;
        for (std::size_t row = 0; row < Target::physical_rows; ++row)
        {
            hidden_copy_differs =
                hidden_copy_differs ||
                !close_enough(one_hidden.target_full_grchombo[row],
                              oracle[row]);
        }
        require(hidden_copy_differs,
                "one-hidden-copy live target expansion must be rejected");
    }

    const IntVect mutation_point = iv(3, 3);
    const auto baseline = read_state(enabled, mutation_point);
    int registration_calls = 0;
    FArrayBox reordered_rhs(storage_box, Production::NUM_VARS);
    reordered_rhs.setVal(sentinel);
    using RegistrationMutationCompute =
        Live::BasicRHSCompute<SwappedRegistrationInputPolicy>;
    BoxLoops::loop(
        RegistrationMutationCompute(
            r0, dx, offset, gauge, true,
            SwappedRegistrationInputPolicy{&registration_calls}),
        input, reordered_rhs,
                   Box(mutation_point, mutation_point), disable_simd());
    bool reordered_differs = false;
    const auto reordered_value = read_state(reordered_rhs, mutation_point);
    for (int row = 0; row < Production::NUM_VARS; ++row)
    {
        reordered_differs =
            reordered_differs ||
            !close_enough(reordered_value[static_cast<std::size_t>(row)],
                          baseline[static_cast<std::size_t>(row)]);
    }
    require(registration_calls == 1 && reordered_differs,
            "wrong live variable registration order mutation must be rejected");

    const auto run_source_mutation =
        [&](const auto &evaluation_policy, const char *label,
            const std::array<int, 2> expected_wrong_rows) {
            using EvaluationPolicy =
                std::decay_t<decltype(evaluation_policy)>;
            using Compute =
                Live::BasicRHSCompute<Live::DefaultInputPolicy,
                                      EvaluationPolicy>;
            FArrayBox mutated(storage_box, Production::NUM_VARS);
            mutated.setVal(sentinel);
            BoxLoops::loop(
                Compute(r0, dx, offset, gauge, true,
                        Live::DefaultInputPolicy{}, evaluation_policy),
                input, mutated, Box(mutation_point, mutation_point),
                disable_simd());
            const auto value = read_state(mutated, mutation_point);
            bool failed = false;
            for (const int row : expected_wrong_rows)
            {
                if (row >= 0)
                {
                    failed =
                        failed ||
                        !close_enough(
                            value[static_cast<std::size_t>(row)],
                            baseline[static_cast<std::size_t>(row)]);
                }
            }
            require(failed, label);
        };
    int omitted_source_calls = 0;
    run_source_mutation(
        OmitSourceEvaluationPolicy{&omitted_source_calls},
        "live omitted-source policy must fail the application oracle",
        {Production::c_lapse, -1});
    require(omitted_source_calls == 1,
            "omitted-source live path must execute once");
    int double_source_calls = 0;
    run_source_mutation(
        DoubleSourceEvaluationPolicy{&double_source_calls},
        "live double-source policy must fail the application oracle",
        {Production::c_lapse, -1});
    require(double_source_calls == 1,
            "double-source live path must execute once");
    int shift_source_calls = 0;
    run_source_mutation(
        ShiftSourceEvaluationPolicy{&shift_source_calls},
        "live shift-source policy must fail the application oracle",
        {Production::c_lapse, Production::c_shiftX});
    require(shift_source_calls == 1,
            "shift-source live path must execute once");
    int B_source_calls = 0;
    run_source_mutation(
        BSourceEvaluationPolicy{&B_source_calls},
        "live B-source policy must fail the application oracle",
        {Production::c_lapse, Production::c_Bz});
    require(B_source_calls == 1,
            "B-source live path must execute once");

    const auto run_write_mutation =
        [&](const WriteMutation mutation, const int defective_row,
            const int expected_count, const char *label) {
            std::array<int, Production::NUM_VARS> counts{};
            int storage_calls = 0;
            FArrayBox mutated(storage_box, Production::NUM_VARS);
            mutated.setVal(sentinel);
            const CountingMutatedRHSStoragePolicy policy{
                &counts, &storage_calls, mutation};
            using Compute = Live::BasicRHSCompute<
                Live::DefaultInputPolicy, Live::DefaultEvaluationPolicy,
                Live::DefaultPreStorePolicy,
                CountingMutatedRHSStoragePolicy>;
            BoxLoops::loop(
                Compute(r0, dx, offset, gauge, true,
                        Live::DefaultInputPolicy{},
                        Live::DefaultEvaluationPolicy{},
                        Live::DefaultPreStorePolicy{}, policy),
                input, mutated, Box(mutation_point, mutation_point),
                disable_simd());
            require(storage_calls == 1 &&
                        counts[static_cast<std::size_t>(defective_row)] ==
                            expected_count,
                    label);
            for (int row = 0; row < Production::NUM_VARS; ++row)
            {
                if (row != defective_row)
                {
                    require(counts[static_cast<std::size_t>(row)] == 1,
                            "RHS write mutation must affect one owner only");
                }
            }
            require(std::abs(
                        baseline[static_cast<std::size_t>(defective_row)]) >
                        1.0e-12,
                    "RHS ownership mutation requires active nonzero data");
        };
    run_write_mutation(WriteMutation::omit_hww, Production::c_hww, 0,
                       "live omitted hww write must be detected");
    run_write_mutation(WriteMutation::omit_Aww, Production::c_Aww, 0,
                       "live omitted Aww write must be detected");
    run_write_mutation(WriteMutation::duplicate_hww, Production::c_hww, 2,
                       "live duplicate representative write must be detected");
    run_write_mutation(WriteMutation::duplicate_K, Production::c_K, 2,
                       "live duplicate nonrepresentative write must be detected");

    FArrayBox legacy_input(storage_box, 27);
    FArrayBox legacy_output(storage_box, Production::NUM_VARS);
    legacy_input.setVal(1.0);
    bool rejected_legacy = false;
    try
    {
        const BoxPointers pointers(legacy_input, legacy_output);
        const Cell<double> cell(mutation_point, pointers);
        Live::RHSCompute(r0, dx, offset, gauge, true).compute(cell);
    }
    catch (const std::invalid_argument &)
    {
        rejected_legacy = true;
    }
    require(rejected_legacy,
            "legacy 27-slot live input mutation must be rejected");

    std::cout << "LIVE_RHS cells=" << selected.numPts()
              << " rows=18 max_abs=" << maximum_error
              << " source_owner=lapse_once PASS\n";
    std::cout << "LIVE_PATH_MUTATIONS registration_swap=REJECT "
                 "source_omitted=REJECT source_twice=REJECT "
                 "source_shift=REJECT source_B=REJECT "
                 "omit_hww=REJECT omit_Aww=REJECT duplicate_hww=REJECT "
                 "duplicate_K=REJECT "
                 "wrong_coordinate=REJECT "
                 "one_hidden_copy=REJECT slot_order=REJECT legacy_27=REJECT "
                 "path_execution=PASS\n";
}

Target::Rows shift_derivative_rows(const Target::Input &input)
{
    const auto expanded = Target::expand_target(input);
    auto without_shift_derivatives = expanded;
    for (int component = 0; component < Target::target_dimension; ++component)
    {
        for (int first = 0; first < Target::target_dimension; ++first)
        {
            without_shift_derivatives.d1.shift[component][first] = 0.0;
            for (int second = 0; second < Target::target_dimension; ++second)
            {
                without_shift_derivatives
                    .d2.shift[component][first][second] = 0.0;
            }
        }
    }
    return Target::subtract_rows(
        Target::physical_rows_from(
            Target::evaluate_direct(expanded, input.parameters)),
        Target::physical_rows_from(Target::evaluate_direct(
            without_shift_derivatives, input.parameters)));
}

struct ManufacturedResult
{
    int resolution = 0;
    double dx = 0.0;
    WorstError ricci;
    WorstError encoded_z;
    WorstError advection;
    WorstError shift_terms;
    WorstError lapse_derivatives;
    WorstError combined_rows;
    WorstError constraints;
};

ManufacturedResult manufactured_resolution(const int resolution)
{
    constexpr double r0 = 1.3;
    const double dx = 1.0 / static_cast<double>(resolution);
    constexpr std::array<double, CH_SPACEDIM> offset = {-1.5, -0.2};
    const Box storage_box(iv(-3, -3),
                          iv(resolution + 2, resolution + 2));
    const Box selected(iv(3, 3),
                       iv(resolution - 4, resolution - 4));
    FArrayBox input(storage_box, Production::NUM_VARS);
    FArrayBox output(storage_box, Production::NUM_VARS);
    fill_manufactured(input, dx, offset);
    output.setVal(sentinel);
    BoxLoops::loop(Live::RHSCompute(r0, dx, offset, Live::GaugeParameters{},
                                    true),
                   input, output, selected, disable_simd());

    ManufacturedResult result;
    result.resolution = resolution;
    result.dx = dx;
    for (BoxIterator bit(selected); bit.ok(); ++bit)
    {
        const IntVect point = bit();
        const double x =
            BlackStringCoordinates::cell_centered<double>(point[0], dx,
                                                           offset[0]);
        const double z =
            BlackStringCoordinates::cell_centered<double>(point[1], dx,
                                                           offset[1]);
        const Target::Input analytic = manufactured_input(x, z);
        const BoxPointers pointers(input, output);
        const Cell<double> cell(point, pointers);
        const Target::Input discrete =
            Live::make_pointwise_input(cell, dx, offset);
        const auto actual = read_state(output, point);
        const auto expected = expected_rhs(analytic, r0);
        for (int row = 0; row < Production::NUM_VARS; ++row)
        {
            retain_worst(
                result.combined_rows,
                std::abs(actual[static_cast<std::size_t>(row)] -
                         expected[static_cast<std::size_t>(row)]),
                row, point, x, z,
                parity_name(
                    Production::metadata[static_cast<std::size_t>(row)]
                        .parity));
        }

        const auto discrete_report = Target::evaluate(discrete);
        const auto analytic_report = Target::evaluate(analytic);
        const auto compare_family =
            [&](const Target::ReportedFamily family, WorstError &worst) {
                const std::size_t family_index =
                    static_cast<std::size_t>(family);
                for (std::size_t row = 0; row < Target::physical_rows; ++row)
                {
                    retain_worst(
                        worst,
                        std::abs(discrete_report.family[family_index]
                                     .target_full_grchombo[row] -
                                 analytic_report.family[family_index]
                                     .target_full_grchombo[row]),
                        static_cast<int>(row), point, x, z,
                        parity_name(Production::metadata[row].parity));
                }
            };
        compare_family(Target::ReportedFamily::geometric_ricci,
                       result.ricci);
        compare_family(Target::ReportedFamily::encoded_z,
                       result.encoded_z);
        compare_family(Target::ReportedFamily::lapse_derivatives,
                       result.lapse_derivatives);

        const auto discrete_advection = Target::physical_rows_from(
            Target::expand_target(discrete).advection);
        const auto analytic_advection = Target::physical_rows_from(
            Target::expand_target(analytic).advection);
        const auto discrete_shift = shift_derivative_rows(discrete);
        const auto analytic_shift = shift_derivative_rows(analytic);
        for (std::size_t row = 0; row < Target::physical_rows; ++row)
        {
            retain_worst(result.advection,
                         std::abs(discrete_advection[row] -
                                  analytic_advection[row]),
                         static_cast<int>(row), point, x, z,
                         parity_name(Production::metadata[row].parity));
            retain_worst(result.shift_terms,
                         std::abs(discrete_shift[row] -
                                  analytic_shift[row]),
                         static_cast<int>(row), point, x, z,
                         parity_name(Production::metadata[row].parity));
        }

        const auto discrete_constraints =
            TargetOps::evaluate_constraints(discrete).target_total;
        const auto analytic_constraints =
            TargetOps::evaluate_constraints(analytic).target_total;
        const std::array<double, 3> constraint_errors = {
            std::abs(discrete_constraints.hamiltonian -
                     analytic_constraints.hamiltonian),
            std::abs(discrete_constraints.momentum[0] -
                     analytic_constraints.momentum[0]),
            std::abs(discrete_constraints.momentum[1] -
                     analytic_constraints.momentum[1])};
        for (int component = 0; component < 3; ++component)
        {
            retain_worst(
                result.constraints,
                constraint_errors[static_cast<std::size_t>(component)],
                component, point, x, z,
                component == 2 ? "one_z" : "scalar_even");
        }
    }
    return result;
}

double convergence_rate(const double coarse, const double fine)
{
    return std::log(coarse / fine) / std::log(2.0);
}

void check_manufactured_convergence()
{
    const std::array<int, 4> resolutions = {8, 16, 32, 64};
    std::vector<ManufacturedResult> results;
    for (const int resolution : resolutions)
    {
        results.push_back(manufactured_resolution(resolution));
    }

    std::cout << "MANUFACTURED_CONVERGENCE\n";
    std::cout << "N dx ricci encoded_Z advection shift_terms "
                 "lapse_derivatives combined_rows constraints\n";
    for (std::size_t index = 0; index < results.size(); ++index)
    {
        const auto &current = results[index];
        std::cout << current.resolution << " " << current.dx << " "
                  << current.ricci.error << " "
                  << current.encoded_z.error << " "
                  << current.advection.error << " "
                  << current.shift_terms.error << " "
                  << current.lapse_derivatives.error << " "
                  << current.combined_rows.error << " "
                  << current.constraints.error << "\n";
        print_worst("ricci", current.resolution, current.ricci);
        print_worst("encoded_Z", current.resolution, current.encoded_z);
        print_worst("advection", current.resolution, current.advection);
        print_worst("shift_terms", current.resolution,
                    current.shift_terms);
        print_worst("lapse_derivatives", current.resolution,
                    current.lapse_derivatives);
        print_worst("combined_rows", current.resolution,
                    current.combined_rows);
        print_worst("constraints", current.resolution,
                    current.constraints);
        if (index == 0)
        {
            continue;
        }
        const auto &coarse = results[index - 1];
        const std::array<double, 7> rates = {
            convergence_rate(coarse.ricci.error, current.ricci.error),
            convergence_rate(coarse.encoded_z.error,
                             current.encoded_z.error),
            convergence_rate(coarse.advection.error,
                             current.advection.error),
            convergence_rate(coarse.shift_terms.error,
                             current.shift_terms.error),
            convergence_rate(coarse.lapse_derivatives.error,
                             current.lapse_derivatives.error),
            convergence_rate(coarse.combined_rows.error,
                             current.combined_rows.error),
            convergence_rate(coarse.constraints.error,
                             current.constraints.error)};
        for (const double rate : rates)
        {
            require(rate > 3.5,
                    "manufactured live derivative path must converge at "
                    "fourth order");
        }
    }
    std::cout << "MANUFACTURED_RATES expected_order=4 all_families=PASS\n";
}

struct OneDeterminantCopyPolicy
    : public TargetOps::DefaultCleanupPolicy
{
    static constexpr int determinant_hidden_copies = 1;
};

struct OneTraceCopyPolicy : public TargetOps::DefaultCleanupPolicy
{
    static constexpr int trace_hidden_copies = 1;
};

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

    const auto run_update_hook =
        [&](const bool apply_cleanup_after_update) {
            FArrayBox hook_state(storage_box, Production::NUM_VARS);
            hook_state.copy(state);
            FArrayBox hook_rhs(storage_box, Production::NUM_VARS);
            hook_rhs.setVal(sentinel);
            std::vector<HookEvent> events;
            using TracedCompute =
                Live::BasicRHSCompute<Live::DefaultInputPolicy,
                                      TracedEvaluationPolicy>;
            BoxLoops::loop(
                TracedCompute(
                    1.3, dx, offset, Live::GaugeParameters{}, true,
                    Live::DefaultInputPolicy{},
                    TracedEvaluationPolicy{&events}),
                hook_state, hook_rhs, Box(cleanup_point, cleanup_point),
                disable_simd());
            const auto rhs_values = read_state(hook_rhs, cleanup_point);
            require(std::any_of(
                        rhs_values.begin(), rhs_values.end(),
                        [](const double value) {
                            return std::abs(value) > 1.0e-12;
                        }),
                    "cleanup hook fixture requires active nonzero RHS data");
            auto updated = read_state(hook_state, cleanup_point);
            constexpr double dt = 1.0e-12;
            for (int row = 0; row < Production::NUM_VARS; ++row)
            {
                updated[static_cast<std::size_t>(row)] +=
                    dt * rhs_values[static_cast<std::size_t>(row)];
            }
            events.push_back(HookEvent::update);
            write_state(hook_state, cleanup_point, Reduced::load(updated));
            if (apply_cleanup_after_update)
            {
                events.push_back(HookEvent::cleanup_after_update);
                BoxLoops::loop(Live::CleanupCompute(), hook_state, hook_state,
                               Box(cleanup_point, cleanup_point),
                               disable_simd());
            }
            return std::make_pair(read_state(hook_state, cleanup_point),
                                  events);
        };

    const auto correct_hook = run_update_hook(true);
    const auto omitted_hook = run_update_hook(false);
    require(correct_hook.second ==
                std::vector<HookEvent>{
                    HookEvent::rhs, HookEvent::update,
                    HookEvent::cleanup_after_update},
            "correct live update hook order must be RHS, update, cleanup");
    require(omitted_hook.second ==
                std::vector<HookEvent>{HookEvent::rhs, HookEvent::update},
            "omitted live update cleanup path must execute RHS and update");
    const auto correct_hook_state = Reduced::load(correct_hook.first);
    const auto omitted_hook_state = Reduced::load(omitted_hook.first);
    require_close(TargetOps::determinant(correct_hook_state.physical.h), 1.0,
                  "correct update-hook determinant cleanup");
    require_close(
        TargetOps::policy_trace<TargetOps::DefaultCleanupPolicy>(
            correct_hook_state.physical.A, correct_hook_state.physical.h),
        0.0, "correct update-hook trace cleanup");
    require(!close_enough(
                TargetOps::determinant(omitted_hook_state.physical.h), 1.0) ||
                !close_enough(
                    TargetOps::policy_trace<
                        TargetOps::DefaultCleanupPolicy>(
                        omitted_hook_state.physical.A,
                        omitted_hook_state.physical.h),
                    0.0),
            "omitted live update-hook cleanup must leave a dirty state");

    std::vector<HookEvent> inside_events;
    int inside_cleanup_rejections = 0;
    FArrayBox inside_rhs(storage_box, Production::NUM_VARS);
    inside_rhs.setVal(sentinel);
    bool inside_path_rejected = false;
    try
    {
        using InsideCompute = Live::BasicRHSCompute<
            Live::DefaultInputPolicy, TracedEvaluationPolicy,
            CleanupInsideRHSPreStorePolicy>;
        BoxLoops::loop(
            InsideCompute(
                1.3, dx, offset, Live::GaugeParameters{}, true,
                Live::DefaultInputPolicy{},
                TracedEvaluationPolicy{&inside_events},
                CleanupInsideRHSPreStorePolicy{
                    &inside_events, &inside_cleanup_rejections}),
            state, inside_rhs, Box(cleanup_point, cleanup_point),
            disable_simd());
        FArrayBox ordinary_rhs(storage_box, Production::NUM_VARS);
        ordinary_rhs.setVal(sentinel);
        BoxLoops::loop(
            Live::RHSCompute(
                1.3, dx, offset, Live::GaugeParameters{}, true),
            state, ordinary_rhs, Box(cleanup_point, cleanup_point),
            disable_simd());
        inside_path_rejected =
            read_state(inside_rhs, cleanup_point) !=
            read_state(ordinary_rhs, cleanup_point);
    }
    catch (const std::domain_error &)
    {
        inside_path_rejected = true;
    }
    require(inside_events ==
                std::vector<HookEvent>{
                    HookEvent::rhs, HookEvent::cleanup_inside_rhs} &&
                inside_cleanup_rejections == 1 && inside_path_rejected,
            "cleanup-inside-RHS live policy must execute and be rejected");

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
    const auto one_determinant_copy =
        TargetOps::cleanup_with_policy(deformed,
                                       OneDeterminantCopyPolicy{});
    const auto one_trace_copy =
        TargetOps::cleanup_with_policy(deformed, OneTraceCopyPolicy{});
    require(!close_enough(
                TargetOps::determinant(one_determinant_copy.cleaned.physical.h),
                1.0),
            "determinant multiplicity-one mutation must be rejected");
    require(!close_enough(
                TargetOps::policy_trace<TargetOps::DefaultCleanupPolicy>(
                    one_trace_copy.cleaned.physical.A,
                    one_trace_copy.cleaned.physical.h),
                0.0),
            "trace multiplicity-one mutation must be rejected");

    FArrayBox diagnostics(selected, 3);
    diagnostics.setVal(sentinel);
    std::vector<Reduced::Storage<double>> before;
    for (BoxIterator bit(selected); bit.ok(); ++bit)
    {
        before.push_back(read_state(state, bit()));
    }
    BoxLoops::loop(Live::ConstraintCompute(dx, offset), state, diagnostics,
                   selected, disable_simd());
    bool rejected_mutating_constraints = false;
    try
    {
        const BoxPointers pointers(state, state);
        const Cell<double> cell(cleanup_point, pointers);
        Live::ConstraintCompute(dx, offset).compute(cell);
    }
    catch (const std::invalid_argument &)
    {
        rejected_mutating_constraints = true;
    }
    require(rejected_mutating_constraints,
            "constraint output alias that could mutate state must be rejected");
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
              << " hww_writes=1 Aww_writes=1 "
                 "hook_order=RHS_UPDATE_CLEANUP "
                 "inside_rhs=REJECT omitted_after_update=REJECT "
                 "multiplicity_one=REJECT PASS\n";
    std::cout << "CONSTRAINTS outputs=Ham,Mx,Mz max_abs=" << maximum_error
              << " state_mutations=0 mutating_alias=REJECT PASS\n";
}

double scalar_profile(const double z)
{
    return 2.0 + 0.1 * std::sin(z) + 0.03 * std::cos(2.0 * z);
}

double scalar_derivative(const double z)
{
    return 0.1 * std::cos(z) - 0.06 * std::sin(2.0 * z);
}

double scalar_second_derivative(const double z)
{
    return -0.1 * std::sin(z) - 0.12 * std::cos(2.0 * z);
}

double one_z_profile(const double z)
{
    return 0.2 * std::cos(z) - 0.04 * std::sin(3.0 * z);
}

double one_z_derivative(const double z)
{
    return -0.2 * std::sin(z) - 0.12 * std::cos(3.0 * z);
}

double one_z_second_derivative(const double z)
{
    return -0.2 * std::cos(z) + 0.36 * std::sin(3.0 * z);
}

double mixed_profile(const double x, const double z)
{
    return 1.0 + 0.02 * std::sin(0.8 * x) * std::cos(2.0 * z) +
           0.01 * std::cos(1.1 * x - 3.0 * z);
}

double mixed_derivative(const double x, const double z)
{
    return -0.032 * std::cos(0.8 * x) * std::sin(2.0 * z) +
           0.033 * std::cos(1.1 * x - 3.0 * z);
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
        const int x_stride =
            pointers.m_in_stride[Live::radial_direction];
        const int z_stride =
            pointers.m_in_stride[Live::compact_direction];
        cell.store_vars(
            derivatives.diff1<double>(
                pointers.m_in_ptr[Production::c_chi], index, z_stride),
            0);
        cell.store_vars(
            derivatives.diff1<double>(
                pointers.m_in_ptr[Production::c_GammaZ], index, z_stride),
            1);
        cell.store_vars(
            derivatives.diff2<double>(
                pointers.m_in_ptr[Production::c_chi], index, z_stride),
            2);
        cell.store_vars(
            derivatives.diff2<double>(
                pointers.m_in_ptr[Production::c_GammaZ], index, z_stride),
            3);
        cell.store_vars(
            derivatives.mixed_diff2<double>(
                pointers.m_in_ptr[Production::c_hxx], index, x_stride,
                z_stride),
            4);
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
    std::array<double, 5> interbox_errors{};
    WorstError interbox_worst;
    std::array<int, 2> interbox_side_evaluations{};
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
    LevelData<FArrayBox> derivative(layout, 5, IntVect::Zero);

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
            const double x =
                (static_cast<double>(point[0]) + 0.5) * dx;
            fab(point, Production::c_hxx) = mixed_profile(x, z);
        }
    }

    bool no_exchange_rejected = false;
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = layout[dit];
        if (valid.smallEnd(1) != 0)
        {
            continue;
        }
        const IntVect point = iv(resolution / 3, 0);
        FArrayBox derivative_without_exchange(Box(point, point), 5);
        BoxLoops::loop(PeriodicDerivativeCompute(dx), state[dit],
                       derivative_without_exchange, Box(point, point),
                       disable_simd());
        const double z = 0.5 * dx;
        no_exchange_rejected =
            !close_enough(derivative_without_exchange(point, 0),
                          scalar_derivative(z)) &&
            !close_enough(derivative_without_exchange(point, 1),
                          one_z_derivative(z));
    }
    require(no_exchange_rejected,
            "omitted periodic ghost exchange mutation must be rejected");
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
        for (const int z_index :
             {resolution / 2 - 1, resolution / 2})
        {
            const IntVect point = iv(x, z_index);
            if (!valid.contains(point))
            {
                continue;
            }
            const double physical_x =
                (static_cast<double>(x) + 0.5) * dx;
            const double physical_z =
                (static_cast<double>(z_index) + 0.5) * dx;
            const std::size_t side_index =
                z_index == resolution / 2 - 1 ? 0U : 1U;
            ++result.interbox_side_evaluations[side_index];
            const std::array<double, 5> analytic = {
                scalar_derivative(physical_z),
                one_z_derivative(physical_z),
                scalar_second_derivative(physical_z),
                one_z_second_derivative(physical_z),
                mixed_derivative(physical_x, physical_z)};
            for (int component = 0; component < 5; ++component)
            {
                const double error =
                    std::abs(fab(point, component) -
                             analytic[static_cast<std::size_t>(component)]);
                result.interbox_errors[static_cast<std::size_t>(component)] =
                    std::max(
                        result.interbox_errors[
                            static_cast<std::size_t>(component)],
                        error);
                const char *parity =
                    component == 1 || component == 3
                        ? "one_z"
                        : "scalar_even";
                retain_worst(result.interbox_worst, error, component, point,
                             physical_x, physical_z, parity);
            }
        }
    }
    require(result.interbox_side_evaluations[0] == 1 &&
                result.interbox_side_evaluations[1] == 1,
            "internal stencil outputs must be evaluated on both seam sides");
    return result;
}

void check_wrong_periodic_direction_mutation()
{
    constexpr int resolution = 16;
    const double dx = 2.0 * pi / static_cast<double>(resolution);
    const Box domain_box(iv(0, 0), iv(resolution - 1, resolution - 1));
    ProblemDomain wrong_domain(domain_box);
    wrong_domain.setPeriodic(Live::radial_direction, true);
    wrong_domain.setPeriodic(Live::compact_direction, false);
    Vector<Box> boxes(1, domain_box);
    Vector<int> processors(1, 0);
    DisjointBoxLayout layout(boxes, processors, wrong_domain);
    LevelData<FArrayBox> state(layout, Production::NUM_VARS,
                               3 * IntVect::Unit);
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        FArrayBox &fab = state[dit];
        fab.setVal(sentinel);
        for (BoxIterator bit(layout[dit]); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            fab(point, Production::c_chi) =
                scalar_profile((static_cast<double>(point[1]) + 0.5) * dx);
        }
    }
    state.exchange();
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const FArrayBox &fab = state[dit];
        require(fab(iv(-1, resolution / 3), Production::c_chi) != sentinel,
                "wrong periodic direction must incorrectly wrap radial ghost");
        require(fab(iv(resolution / 3, -1), Production::c_chi) == sentinel,
                "wrong periodic direction must leave compact ghost unfilled");
    }
    std::cout << "MUTATIONS wrong_periodic_direction=REJECT "
                 "no_periodic_exchange=REJECT\n";
}

void check_periodic_exchange_and_convergence()
{
    check_wrong_periodic_direction_mutation();
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

    constexpr std::array<const char *, 5> seam_fields = {
        "chi_dz", "GammaZ_dz", "chi_dzz", "GammaZ_dzz", "hxx_dxz"};
    std::cout << "INTERBOX_SEAM_CONVERGENCE\n";
    std::cout << "N dx scalar_dz scalar_dzz one_z_dz one_z_dzz mixed_dxz "
                 "rate_scalar_dz rate_scalar_dzz rate_one_z_dz "
                 "rate_one_z_dzz rate_mixed_dxz worst_error worst_rate "
                 "worst_field side iv x z\n";
    for (std::size_t index = 0; index < results.size(); ++index)
    {
        const auto &current = results[index];
        std::array<double, 5> rates{};
        double worst_rate = 0.0;
        if (index > 0)
        {
            const auto &coarse = results[index - 1];
            for (std::size_t component = 0;
                 component < current.interbox_errors.size(); ++component)
            {
                rates[component] = convergence_rate(
                    coarse.interbox_errors[component],
                    current.interbox_errors[component]);
                require(rates[component] > 3.7,
                        "internal inter-box stencil must converge at fourth "
                        "order");
            }
            worst_rate = convergence_rate(
                coarse.interbox_worst.error,
                current.interbox_worst.error);
        }
        const int worst_component = current.interbox_worst.row;
        require(worst_component >= 0 && worst_component < 5,
                "internal seam provenance requires a worst field");
        const char *side =
            current.interbox_worst.point[1] ==
                    current.resolution / 2 - 1
                ? "lower_box"
                : "upper_box";
        std::cout
            << current.resolution << " " << current.dx << " "
            << current.interbox_errors[0] << " "
            << current.interbox_errors[2] << " "
            << current.interbox_errors[1] << " "
            << current.interbox_errors[3] << " "
            << current.interbox_errors[4] << " " << rates[0] << " "
            << rates[2] << " " << rates[1] << " " << rates[3] << " "
            << rates[4] << " " << current.interbox_worst.error << " "
            << worst_rate << " "
            << seam_fields[static_cast<std::size_t>(worst_component)] << " "
            << side << " (" << current.interbox_worst.point[0] << ","
            << current.interbox_worst.point[1] << ") "
            << current.interbox_worst.x << " "
            << current.interbox_worst.z << "\n";
    }
    std::cout << "PERIODIC_OWNERSHIP low_end_wrap=PASS high_end_wrap=PASS "
                 "internal_lower_stencil=PASS internal_upper_stencil=PASS "
                 "internal_interbox_stencils=PASS one_z_sign_flip=0 "
                 "radial_wrap=0\n";
}

Target::Input analytic_gp_input(const double r0, const double x)
{
    Target::Input input{};
    input.x = x;
    input.vars = GP::make_pointwise_vars(r0, x);
    const auto analytic = GP::make_analytic_metadata(r0, x);
    const auto set_radial_jet =
        [&](const Production::VariableSlot slot,
            const GP::RadialJet &jet) {
            input.derivatives[static_cast<std::size_t>(slot)] =
                {jet.dx, 0.0, jet.dxx, 0.0, 0.0};
        };
    set_radial_jet(Production::c_K, analytic.K);
    set_radial_jet(Production::c_Axx, analytic.A.xx);
    set_radial_jet(Production::c_Axz, analytic.A.xz);
    set_radial_jet(Production::c_Azz, analytic.A.zz);
    set_radial_jet(Production::c_Aww, analytic.A.ww);
    set_radial_jet(Production::c_shiftX, analytic.beta_x);
    return input;
}

struct GPResidualResult
{
    int resolution = 0;
    double dx = 0.0;
    std::array<double, Production::NUM_VARS> rows = {};
    std::array<double, 3> constraints = {};
    double lapse_before_source = 0.0;
    double lapse_after_source = 0.0;
};

GPResidualResult gp_residual_resolution(const int resolution)
{
    constexpr double r0 = 1.0;
    const double dx = 2.0 / static_cast<double>(resolution);
    const std::array<double, CH_SPACEDIM> offset = {-1.0, 0.0};
    const Box domain_box(iv(0, 0), iv(resolution - 1, resolution - 1));
    ProblemDomain domain(domain_box);
    domain.setPeriodic(Live::radial_direction, false);
    domain.setPeriodic(Live::compact_direction, true);

    Vector<Box> boxes;
    boxes.push_back(
        Box(iv(0, 0), iv(resolution - 1, resolution / 2 - 1)));
    boxes.push_back(Box(iv(0, resolution / 2),
                        iv(resolution - 1, resolution - 1)));
    Vector<int> processors(2, 0);
    DisjointBoxLayout layout(boxes, processors, domain);
    const IntVect ghosts = 3 * IntVect::Unit;
    LevelData<FArrayBox> state(layout, Production::NUM_VARS, ghosts);
    LevelData<FArrayBox> enabled(layout, Production::NUM_VARS,
                                 IntVect::Zero);
    LevelData<FArrayBox> disabled(layout, Production::NUM_VARS,
                                  IntVect::Zero);
    LevelData<FArrayBox> diagnostics(layout, 3, IntVect::Zero);

    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        FArrayBox &fab = state[dit];
        fab.setVal(sentinel);
        const Box valid = layout[dit];
        BoxLoops::loop(
            BlackStringGPInitialData::make_compute(r0, dx, offset),
            fab, fab, valid, disable_simd());
    }
    state.exchange();

    GPResidualResult result;
    result.resolution = resolution;
    result.dx = dx;
    bool saw_periodic_gp_ghost = false;
    bool radial_ghost_untouched = true;
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = layout[dit];
        const Box selected(iv(3, valid.smallEnd(1)),
                           iv(resolution - 4, valid.bigEnd(1)));
        enabled[dit].setVal(sentinel);
        disabled[dit].setVal(sentinel);
        diagnostics[dit].setVal(sentinel);
        BoxLoops::loop(
            Live::RHSCompute(r0, dx, offset, Live::GaugeParameters{}, true),
            state[dit], enabled[dit], selected, disable_simd());
        BoxLoops::loop(
            Live::RHSCompute(r0, dx, offset, Live::GaugeParameters{}, false),
            state[dit], disabled[dit], selected, disable_simd());
        BoxLoops::loop(Live::ConstraintCompute(dx, offset), state[dit],
                       diagnostics[dit], selected, disable_simd());

        const FArrayBox &state_fab = state[dit];
        if (valid.smallEnd(1) == 0)
        {
            const int x_index = resolution / 2;
            saw_periodic_gp_ghost =
                state_fab(iv(x_index, -1), Production::c_hww) == 1.0;
        }
        if (state_fab.box().contains(iv(-1, resolution / 3)))
        {
            radial_ghost_untouched =
                radial_ghost_untouched &&
                state_fab(iv(-1, resolution / 3), Production::c_chi) ==
                    sentinel;
        }

        for (BoxIterator bit(selected); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            const double x =
                BlackStringCoordinates::cell_centered<double>(
                    point[0], dx, offset[0]);
            const auto analytic = analytic_gp_input(r0, x);
            const auto analytic_rhs = expected_rhs(analytic, r0);
            const auto analytic_constraints =
                TargetOps::evaluate_constraints(analytic).target_total;
            for (int row = 0; row < Production::NUM_VARS; ++row)
            {
                require(std::abs(analytic_rhs[static_cast<std::size_t>(row)]) <
                            2.0e-12,
                        "analytic GP target RHS must be stationary");
                result.rows[static_cast<std::size_t>(row)] =
                    std::max(result.rows[static_cast<std::size_t>(row)],
                             std::abs(enabled[dit](point, row)));
            }
            const std::array<double, 3> analytic_constraint_values = {
                analytic_constraints.hamiltonian,
                analytic_constraints.momentum[0],
                analytic_constraints.momentum[1]};
            for (int component = 0; component < 3; ++component)
            {
                require(std::abs(
                            analytic_constraint_values[
                                static_cast<std::size_t>(component)]) <
                            2.0e-12,
                        "analytic GP target constraints must vanish");
                result.constraints[static_cast<std::size_t>(component)] =
                    std::max(
                        result.constraints[static_cast<std::size_t>(component)],
                        std::abs(diagnostics[dit](point, component)));
            }
            result.lapse_before_source =
                std::max(result.lapse_before_source,
                         std::abs(disabled[dit](point,
                                               Production::c_lapse)));
            result.lapse_after_source =
                std::max(result.lapse_after_source,
                         std::abs(enabled[dit](point,
                                              Production::c_lapse)));
        }
    }
    require(saw_periodic_gp_ghost,
            "GP residual fixture requires real periodic z ghost exchange");
    require(radial_ghost_untouched,
            "GP periodic exchange must leave radial boundary ghosts owned "
            "independently");
    return result;
}

void check_gp_residual_convergence()
{
    const std::array<int, 4> resolutions = {32, 64, 128, 256};
    std::vector<GPResidualResult> results;
    for (const int resolution : resolutions)
    {
        results.push_back(gp_residual_resolution(resolution));
    }

    std::cout << "GP_RESIDUALS\n";
    std::cout << "N dx";
    for (int row = 0; row < Production::NUM_VARS; ++row)
    {
        std::cout << " rhs_" << row;
    }
    std::cout << " Ham Mx Mz lapse_before lapse_after\n";
    for (const auto &result : results)
    {
        std::cout << result.resolution << " " << result.dx;
        for (const double row : result.rows)
        {
            std::cout << " " << row;
        }
        std::cout << " " << result.constraints[0] << " "
                  << result.constraints[1] << " " << result.constraints[2]
                  << " " << result.lapse_before_source << " "
                  << result.lapse_after_source << "\n";
    }

    const auto maximum_row = [](const GPResidualResult &result) {
        return *std::max_element(result.rows.begin(), result.rows.end());
    };
    const auto maximum_constraint = [](const GPResidualResult &result) {
        return *std::max_element(result.constraints.begin(),
                                 result.constraints.end());
    };
    for (std::size_t index = 1; index < results.size(); ++index)
    {
        require(maximum_row(results[index]) <
                    maximum_row(results[index - 1]),
                "GP live RHS residual must converge toward stationarity");
        require(maximum_constraint(results[index]) <
                    maximum_constraint(results[index - 1]),
                "GP live constraints must converge toward zero");
        require(results[index].lapse_after_source <=
                    results[index - 1].lapse_after_source + 1.0e-14,
                "fixed-source GP lapse drift must converge toward zero");
    }
    require(results.back().lapse_after_source <
                results.back().lapse_before_source,
            "fixed source must reduce GP lapse drift");
    std::cout << "GP_RESIDUAL_CONVERGENCE rhs=PASS constraints=PASS "
                 "fixed_source=PASS radial_boundary=EXCLUDED\n";
}

} // namespace

int main()
{
    std::cout << std::setprecision(16);
    check_registration();
    check_gp_initializer();
    check_live_rhs();
    check_manufactured_convergence();
    check_cleanup_constraints();
    check_periodic_exchange_and_convergence();
    check_gp_residual_convergence();
    std::cout << "BLACKSTRING_LIVE_APPLICATION=PASS\n";
    return 0;
}
