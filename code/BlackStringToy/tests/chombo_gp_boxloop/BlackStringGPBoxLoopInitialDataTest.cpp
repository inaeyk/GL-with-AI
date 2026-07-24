#include "BoxIterator.H"
#include "BoxLoops.hpp"
#include "FArrayBox.H"

#include "BlackStringGPInitialData.hpp"
#include "BoxPointers.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef USERVARIABLES_HPP
#error "GP BoxLoop fixture must not import the legacy 27-slot enum"
#endif

namespace
{
namespace GridGP = BlackStringGPInitialData;
namespace GP = BlackStringGPPointwiseInitialData;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace Storage = BlackStringCellStorage;

using State = Reduced::Storage<double>;
using Variables = Reduced::Variables<double>;

constexpr double sentinel = -8.7654321e29;
constexpr double absolute_tolerance = 5.0e-13;
constexpr double relative_tolerance = 5.0e-12;
constexpr double r0 = 1.7;
constexpr double dx = 0.25;
constexpr std::array<double, CH_SPACEDIM> origin = {0.375, -1.25};

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(Production::NUM_VARS == 18);
static_assert(GridGP::radial_direction == 0);
static_assert(GridGP::compact_direction == 1);
static_assert(
    std::is_same<GridGP::Compute::storage_policy_type,
                 GridGP::DefaultStoragePolicy>::value);
static_assert(!Production::uses_stock_visible_y_as_hidden);

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

IntVect make_iv(const int radial, const int compact)
{
    return IntVect(D_DECL(radial, compact, 0));
}

Box outer_box()
{
    return Box(make_iv(1, -3), make_iv(6, 3));
}

Box requested_box()
{
    return Box(make_iv(2, -2), make_iv(5, 2));
}

struct TestCoordinates
{
    double x;
    double z;
};

TestCoordinates test_side_coordinates(const IntVect &iv)
{
    // Deliberately independent of the production coordinate adapter.
    return {(static_cast<double>(iv[0]) + 0.5) * dx - origin[0],
            (static_cast<double>(iv[1]) + 0.5) * dx - origin[1]};
}

State read_state(const FArrayBox &fab, const IntVect &iv)
{
    State state{};
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        state[static_cast<std::size_t>(slot)] = fab(iv, slot);
    }
    return state;
}

bool state_is_sentinel(const FArrayBox &fab, const IntVect &iv)
{
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        if (fab(iv, slot) != sentinel)
        {
            return false;
        }
    }
    return true;
}

class VisitRecorder
{
  public:
    explicit VisitRecorder(FArrayBox &visits) : m_visits(&visits) {}

    void compute(const Cell<double> current_cell) const
    {
        const IntVect iv = current_cell.get_int_vect();
        (*m_visits)(iv, 0) += 1.0;
    }

  private:
    FArrayBox *m_visits;
};

template <class compute_t>
void run_boxloop(const compute_t &compute, const FArrayBox &input,
                 FArrayBox &output, FArrayBox &visits, const Box &loop_box)
{
    BoxLoops::loop(make_compute_pack(compute, VisitRecorder(visits)), input,
                   output, loop_box, disable_simd());
}

struct AdapterInvocation
{
    IntVect target;
    std::array<int, Production::NUM_VARS> written_slots;
};

class AdapterInvocationState
{
  public:
    void record(AdapterInvocation invocation)
    {
        const std::lock_guard<std::mutex> lock(m_mutex);
        m_invocations.push_back(std::move(invocation));
    }

    std::vector<AdapterInvocation> snapshot() const
    {
        const std::lock_guard<std::mutex> lock(m_mutex);
        return m_invocations;
    }

  private:
    mutable std::mutex m_mutex;
    std::vector<AdapterInvocation> m_invocations;
};

class RecordingStoragePolicy
{
  public:
    explicit RecordingStoragePolicy(
        std::shared_ptr<AdapterInvocationState> state)
        : m_state(std::move(state))
    {
        if (!m_state)
        {
            throw std::invalid_argument(
                "recording storage policy requires shared state");
        }
    }

    void store(const Cell<double> current_cell,
               const Variables &vars) const
    {
        AdapterInvocation invocation{current_cell.get_int_vect(), {}};
        for (int slot = 0; slot < Production::NUM_VARS; ++slot)
        {
            invocation.written_slots[static_cast<std::size_t>(slot)] = slot;
        }

        // Instrument the policy boundary while retaining the real production
        // adapter as the sole implementation of Cell component writes.
        Storage::store(current_cell, vars);
        m_state->record(std::move(invocation));
    }

  private:
    std::shared_ptr<AdapterInvocationState> m_state;
};

struct AdapterReport
{
    bool valid = true;
    int expected_invocations = 0;
    int observed_invocations = 0;
    int outside_invocations = 0;
};

AdapterReport
validate_adapter_invocations(const AdapterInvocationState &state)
{
    AdapterReport report;
    const Box outer = outer_box();
    const Box requested = requested_box();
    FArrayBox invocation_counts(outer, 1);
    invocation_counts.setVal(0.0);

    const std::vector<AdapterInvocation> invocations = state.snapshot();
    report.expected_invocations = requested.numPts();
    report.observed_invocations = static_cast<int>(invocations.size());
    report.valid =
        report.observed_invocations == report.expected_invocations;

    for (const AdapterInvocation &invocation : invocations)
    {
        if (!outer.contains(invocation.target) ||
            !requested.contains(invocation.target))
        {
            ++report.outside_invocations;
            report.valid = false;
            continue;
        }
        invocation_counts(invocation.target, 0) += 1.0;

        std::array<int, Production::NUM_VARS> slot_counts{};
        for (const int slot : invocation.written_slots)
        {
            if (slot < 0 || slot >= Production::NUM_VARS)
            {
                report.valid = false;
                continue;
            }
            ++slot_counts[static_cast<std::size_t>(slot)];
        }
        for (const int count : slot_counts)
        {
            report.valid = report.valid && count == 1;
        }
        report.valid =
            report.valid &&
            slot_counts[static_cast<std::size_t>(Production::c_hww)] == 1 &&
            slot_counts[static_cast<std::size_t>(Production::c_Aww)] == 1;
    }

    for (BoxIterator bit(outer); bit.ok(); ++bit)
    {
        const IntVect iv = bit();
        const int count =
            static_cast<int>(std::llround(invocation_counts(iv, 0)));
        report.valid =
            report.valid && count == (requested.contains(iv) ? 1 : 0);
    }
    return report;
}

struct ComparisonReport
{
    bool valid = true;
    double maximum_absolute_error = 0.0;
    double maximum_normalized_error = 0.0;
    int worst_slot = Production::c_chi;
    IntVect worst_iv = make_iv(2, -2);
    TestCoordinates worst_coordinates = test_side_coordinates(worst_iv);
    int expected_visits = 0;
    int observed_visits = 0;
    int outside_points = 0;
};

ComparisonReport validate_full_box(const FArrayBox &output,
                                   const FArrayBox &visits,
                                   const bool check_algebra)
{
    ComparisonReport report;
    bool valid = true;
    const Box outer = outer_box();
    const Box requested = requested_box();

    for (BoxIterator bit(outer); bit.ok(); ++bit)
    {
        const IntVect iv = bit();
        const bool requested_point = requested.contains(iv);
        const int visit_count =
            static_cast<int>(std::llround(visits(iv, 0)));
        if (!requested_point)
        {
            ++report.outside_points;
            valid = valid && visit_count == 0 && state_is_sentinel(output, iv);
            continue;
        }

        ++report.expected_visits;
        report.observed_visits += visit_count;
        valid = valid && visit_count == 1;

        const TestCoordinates coordinates = test_side_coordinates(iv);
        const State oracle = GP::make_pointwise_state(r0, coordinates.x);
        const State actual = read_state(output, iv);
        for (int slot = 0; slot < Production::NUM_VARS; ++slot)
        {
            const std::size_t index = static_cast<std::size_t>(slot);
            valid = valid && actual[index] != sentinel;
            const double error = std::abs(actual[index] - oracle[index]);
            const double allowance =
                absolute_tolerance +
                relative_tolerance *
                    std::max(std::abs(actual[index]), std::abs(oracle[index]));
            const double normalized = error / allowance;
            if (error > report.maximum_absolute_error)
            {
                report.maximum_absolute_error = error;
            }
            if (normalized > report.maximum_normalized_error)
            {
                report.maximum_normalized_error = normalized;
                report.worst_slot = slot;
                report.worst_iv = iv;
                report.worst_coordinates = coordinates;
            }
            valid = valid && normalized <= 1.0;
        }

        if (check_algebra)
        {
            const Variables vars = Reduced::load(actual);
            const double lambda =
                std::sqrt(r0 / (coordinates.x * coordinates.x *
                                coordinates.x));
            require_close(vars.physical.h.xx * vars.physical.h.zz *
                              vars.physical.h.ww * vars.physical.h.ww,
                          1.0, "full-box conformal determinant");
            require_close(vars.physical.A.xx + vars.physical.A.zz +
                              2.0 * vars.physical.A.ww,
                          0.0, "full-box weighted A trace");
            require_close(vars.physical.A.xx + 0.25 * vars.physical.K,
                          -0.5 * lambda, "full-box Kxx");
            require_close(vars.physical.A.zz + 0.25 * vars.physical.K, 0.0,
                          "full-box Kzz");
            require_close(vars.physical.A.ww + 0.25 * vars.physical.K,
                          lambda, "full-box Kww");
            require_close(vars.physical.Theta, 0.0, "full-box Theta");
            require_close(vars.physical.Gamma.x, 0.0,
                          "full-box GammaX");
            require_close(vars.physical.Gamma.z, 0.0,
                          "full-box GammaZ");
            require_close(vars.gauge.lapse, 1.0, "full-box lapse");
            require_close(vars.gauge.shift.x,
                          std::sqrt(r0 / coordinates.x),
                          "full-box betaX");
            require_close(vars.gauge.shift.z, 0.0, "full-box betaZ");
            require_close(vars.gauge.B.x, 0.0, "full-box Bx");
            require_close(vars.gauge.B.z, 0.0, "full-box Bz");
            require(vars.physical.h.ww == 1.0,
                    "full-box hww must be one representative, not x^2");
        }
    }

    report.valid = valid;
    return report;
}

void check_radial_and_compact_ownership(const FArrayBox &output)
{
    const Box requested = requested_box();
    for (int radial = requested.smallEnd(0);
         radial <= requested.bigEnd(0); ++radial)
    {
        const State reference =
            read_state(output, make_iv(radial, requested.smallEnd(1)));
        for (int compact = requested.smallEnd(1) + 1;
             compact <= requested.bigEnd(1); ++compact)
        {
            require(read_state(output, make_iv(radial, compact)) == reference,
                    "same radial index must be exactly z-independent");
        }
    }

    const double inner_beta =
        output(make_iv(requested.smallEnd(0), 0), Production::c_shiftX);
    const double outer_beta =
        output(make_iv(requested.bigEnd(0), 0), Production::c_shiftX);
    require(inner_beta != outer_beta,
            "different radial positions must receive distinct GP data");
}

void check_production_boxloop()
{
    const Box outer = outer_box();
    FArrayBox input(outer, Production::NUM_VARS);
    FArrayBox output(outer, Production::NUM_VARS);
    FArrayBox visits(outer, 1);
    input.setVal(sentinel);
    output.setVal(sentinel);
    visits.setVal(0.0);

    const auto adapter_state =
        std::make_shared<AdapterInvocationState>();
    const auto compute = GridGP::make_compute_with_storage_policy(
        r0, dx, origin, RecordingStoragePolicy(adapter_state));
    run_boxloop(compute, input, output, visits, requested_box());
    const ComparisonReport report =
        validate_full_box(output, visits, true);
    const AdapterReport adapter_report =
        validate_adapter_invocations(*adapter_state);
    require(report.valid, "production full-box contract");
    require(adapter_report.valid,
            "one real storage-adapter invocation must own all 18 slots per "
            "requested cell");
    check_radial_and_compact_ownership(output);

    std::cout << "COVERAGE expected_visits=" << report.expected_visits
              << " observed_visits=" << report.observed_visits
              << " outside_points_unchanged=" << report.outside_points
              << "\n";
    std::cout << "ADAPTER expected_invocations="
              << adapter_report.expected_invocations
              << " observed_invocations="
              << adapter_report.observed_invocations
              << " outside_invocations="
              << adapter_report.outside_invocations
              << " slots_per_invocation=" << Production::NUM_VARS
              << " hww_writes=1 Aww_writes=1\n";
    std::cout << "ORACLE max_abs_error=" << report.maximum_absolute_error
              << " max_normalized_error="
              << report.maximum_normalized_error
              << " worst_slot=" << report.worst_slot << " worst_iv=("
              << report.worst_iv[0] << "," << report.worst_iv[1]
              << ") worst_x=" << report.worst_coordinates.x
              << " worst_z=" << report.worst_coordinates.z << "\n";
    std::cout << "PASS full-box GP BoxLoop oracle and algebra\n";
}

enum class CoordinateMutation
{
    wrong_radial_direction,
    node_centered,
    ignored_origin,
    compact_dependence
};

class CoordinateMutationCompute
{
  public:
    explicit CoordinateMutationCompute(const CoordinateMutation mutation)
        : m_mutation(mutation)
    {
    }

    void compute(const Cell<double> current_cell) const
    {
        const IntVect iv = current_cell.get_int_vect();
        const TestCoordinates coordinates = test_side_coordinates(iv);
        double x = coordinates.x;
        if (m_mutation == CoordinateMutation::wrong_radial_direction)
        {
            x = coordinates.z;
        }
        else if (m_mutation == CoordinateMutation::node_centered)
        {
            x = static_cast<double>(iv[0]) * dx - origin[0];
        }
        else if (m_mutation == CoordinateMutation::ignored_origin)
        {
            x = (static_cast<double>(iv[0]) + 0.5) * dx;
        }
        else
        {
            x += 0.125 * coordinates.z;
        }
        Storage::store(current_cell, GP::make_pointwise_vars(r0, x));
    }

  private:
    CoordinateMutation m_mutation;
};

class HwwMutationCompute
{
  public:
    void compute(const Cell<double> current_cell) const
    {
        const double x =
            test_side_coordinates(current_cell.get_int_vect()).x;
        Variables vars = GP::make_pointwise_vars(r0, x);
        vars.physical.h.ww = x * x;
        Storage::store(current_cell, vars);
    }
};

enum class WriteMutation
{
    omitted_variable,
    duplicate_component,
    swapped_slots
};

class DirectWriteBypassCompute
{
  public:
    explicit DirectWriteBypassCompute(RecordingStoragePolicy storage_policy)
        : m_storage_policy(std::move(storage_policy))
    {
    }

    void compute(const Cell<double> current_cell) const
    {
        // This mutation is handed the observable policy but deliberately
        // bypasses it while producing the otherwise correct state.
        (void)m_storage_policy;
        const double x =
            test_side_coordinates(current_cell.get_int_vect()).x;
        const Variables vars = GP::make_pointwise_vars(r0, x);
        for (int slot = 0; slot < Production::NUM_VARS; ++slot)
        {
            const auto production_slot =
                static_cast<Production::VariableSlot>(slot);
            current_cell.store_vars(
                Reduced::component(vars, production_slot), slot);
        }
    }

  private:
    RecordingStoragePolicy m_storage_policy;
};

class DirectWriteMutationCompute
{
  public:
    explicit DirectWriteMutationCompute(const WriteMutation mutation)
        : m_mutation(mutation)
    {
    }

    void compute(const Cell<double> current_cell) const
    {
        const double x =
            test_side_coordinates(current_cell.get_int_vect()).x;
        const Variables vars = GP::make_pointwise_vars(r0, x);
        for (int source = 0; source < Production::NUM_VARS; ++source)
        {
            if (m_mutation == WriteMutation::omitted_variable &&
                source == Production::c_shiftX)
            {
                continue;
            }
            int destination = source;
            if (m_mutation == WriteMutation::swapped_slots)
            {
                if (source == Production::c_K)
                {
                    destination = Production::c_Axx;
                }
                else if (source == Production::c_Axx)
                {
                    destination = Production::c_K;
                }
            }
            const auto slot =
                static_cast<Production::VariableSlot>(source);
            current_cell.store_vars(Reduced::component(vars, slot),
                                    destination);
        }
        if (m_mutation == WriteMutation::duplicate_component)
        {
            current_cell.store_vars(vars.gauge.shift.x + 0.25,
                                    Production::c_shiftX);
        }
    }

  private:
    WriteMutation m_mutation;
};

template <class compute_t>
void require_compute_mutation_rejected(const std::string &label,
                                       const compute_t &compute)
{
    const Box outer = outer_box();
    FArrayBox input(outer, Production::NUM_VARS);
    FArrayBox output(outer, Production::NUM_VARS);
    FArrayBox visits(outer, 1);
    input.setVal(sentinel);
    output.setVal(sentinel);
    visits.setVal(0.0);
    run_boxloop(compute, input, output, visits, requested_box());

    const bool rejected = !validate_full_box(output, visits, false).valid;
    require(rejected, label + " mutation must fail independently");
    std::cout << "PASS mutation rejected: " << label << "\n";
}

void require_direct_bypass_rejected()
{
    const Box outer = outer_box();
    FArrayBox input(outer, Production::NUM_VARS);
    FArrayBox output(outer, Production::NUM_VARS);
    FArrayBox visits(outer, 1);
    input.setVal(sentinel);
    output.setVal(sentinel);
    visits.setVal(0.0);
    const auto adapter_state =
        std::make_shared<AdapterInvocationState>();

    run_boxloop(
        DirectWriteBypassCompute(RecordingStoragePolicy(adapter_state)),
        input, output, visits, requested_box());

    const ComparisonReport numerical =
        validate_full_box(output, visits, false);
    const AdapterReport adapter =
        validate_adapter_invocations(*adapter_state);
    require(numerical.valid,
            "direct-write bypass must otherwise match the complete GP oracle");
    require(!adapter.valid && adapter.observed_invocations == 0,
            "direct-write bypass must fail only because no adapter "
            "invocation was recorded");
    std::cout
        << "PASS mutation rejected: direct writes bypass storage adapter "
           "(numerics valid, adapter invocations=0)\n";
}

void require_traversal_mutation_rejected(const std::string &label,
                                         const Box &first_loop,
                                         const Box *second_loop)
{
    const Box outer = outer_box();
    FArrayBox input(outer, Production::NUM_VARS);
    FArrayBox output(outer, Production::NUM_VARS);
    FArrayBox visits(outer, 1);
    input.setVal(sentinel);
    output.setVal(sentinel);
    visits.setVal(0.0);
    const GridGP::Compute compute = GridGP::make_compute(r0, dx, origin);
    run_boxloop(compute, input, output, visits, first_loop);
    if (second_loop != nullptr)
    {
        run_boxloop(compute, input, output, visits, *second_loop);
    }

    const bool rejected = !validate_full_box(output, visits, false).valid;
    require(rejected, label + " mutation must fail independently");
    std::cout << "PASS mutation rejected: " << label << "\n";
}

void check_nonpositive_radial_rejection()
{
    const Box point_box(make_iv(0, 0), make_iv(0, 0));
    FArrayBox input(point_box, Production::NUM_VARS);
    FArrayBox output(point_box, Production::NUM_VARS);
    BoxPointers pointers(input, output);
    Cell<double> cell(make_iv(0, 0), pointers);
    const std::array<double, CH_SPACEDIM> bad_origin = {0.25, 0.0};
    const GridGP::Compute compute =
        GridGP::make_compute(r0, dx, bad_origin);
    bool rejected = false;
    try
    {
        compute.compute(cell);
    }
    catch (const std::domain_error &)
    {
        rejected = true;
    }
    require(rejected, "nonpositive physical x must be rejected");
    std::cout << "PASS mutation rejected: nonpositive physical x\n";
}

void check_legacy_shape_rejection()
{
    const Box point_box(make_iv(2, 0), make_iv(2, 0));
    FArrayBox input(point_box, Production::NUM_VARS);
    FArrayBox legacy_output(point_box, 27);
    BoxPointers pointers(input, legacy_output);
    Cell<double> cell(make_iv(2, 0), pointers);
    bool rejected = false;
    try
    {
        GridGP::make_compute(r0, dx, origin).compute(cell);
    }
    catch (const std::invalid_argument &)
    {
        rejected = true;
    }
    require(rejected, "legacy 27-slot output must be rejected");
    std::cout << "PASS mutation rejected: legacy 27-slot enum\n";
}

void check_mutations()
{
    require_compute_mutation_rejected(
        "wrong radial direction",
        CoordinateMutationCompute(CoordinateMutation::wrong_radial_direction));
    require_compute_mutation_rejected(
        "node-centered radial coordinate",
        CoordinateMutationCompute(CoordinateMutation::node_centered));
    require_compute_mutation_rejected(
        "ignored coordinate origin",
        CoordinateMutationCompute(CoordinateMutation::ignored_origin));
    require_compute_mutation_rejected(
        "compact-coordinate dependence",
        CoordinateMutationCompute(CoordinateMutation::compact_dependence));
    require_compute_mutation_rejected("hww=x^2", HwwMutationCompute());
    require_direct_bypass_rejected();
    require_compute_mutation_rejected(
        "omitted variable",
        DirectWriteMutationCompute(WriteMutation::omitted_variable));
    require_compute_mutation_rejected(
        "duplicate component write",
        DirectWriteMutationCompute(WriteMutation::duplicate_component));
    require_compute_mutation_rejected(
        "swapped slots",
        DirectWriteMutationCompute(WriteMutation::swapped_slots));

    const Box incomplete(make_iv(2, -2), make_iv(4, 2));
    require_traversal_mutation_rejected("incomplete box traversal",
                                        incomplete, nullptr);
    const Box one_cell(make_iv(3, 0), make_iv(3, 0));
    require_traversal_mutation_rejected("double visit of a cell",
                                        requested_box(), &one_cell);
    const Box outside_cell(make_iv(6, 3), make_iv(6, 3));
    require_traversal_mutation_rejected("write outside requested box",
                                        requested_box(), &outside_cell);
    check_nonpositive_radial_rejection();
    check_legacy_shape_rejection();
}

} // namespace

int main()
{
    std::cout << "Black-string real GP BoxLoop initializer fixture\n";
    check_production_boxloop();
    check_mutations();
    std::cout << "All GP BoxLoop initializer checks passed.\n";
    return 0;
}
