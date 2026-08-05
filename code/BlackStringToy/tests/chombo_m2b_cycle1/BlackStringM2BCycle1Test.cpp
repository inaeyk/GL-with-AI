#include "BlackStringAlgebraicReconstruction.hpp"
#include "BlackStringConstraintCorrectedSeed.hpp"
#include "BlackStringGammaZHelper.hpp"
#include "BlackStringPerturbativeRadialBoundary.hpp"
#include "BlackStringReducedCharacteristics.hpp"
#include "DisjointBoxLayout.H"
#include "SPMD.H"

#ifdef CH_MPI
#include <mpi.h>
#endif

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

// This focused fixture links BoxTools only; production links the GRChombo
// implementation of this constructor.
GRLevelData::GRLevelData() : LevelData<FArrayBox>() {}

namespace
{
namespace Algebraic = BlackStringAlgebraicReconstruction;
namespace Characteristics = BlackStringReducedCharacteristics;
namespace Seed = BlackStringConstraintCorrectedSeed;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

IntVect iv(const int x, const int z)
{
    IntVect result = IntVect::Zero;
    result[0] = x;
    result[1] = z;
    return result;
}

Reduced::Storage<double> manufactured_state(const int radial, const int compact)
{
    constexpr int nz = 16;
    constexpr double dx = 0.125;
    const int wrapped = ((compact % nz) + nz) % nz;
    const double x = 0.5 + (static_cast<double>(radial) + 0.5) * dx;
    const double phase = 2.0 * 3.14159265358979323846 *
                         (static_cast<double>(wrapped) + 0.5) /
                         static_cast<double>(nz);
    auto vars = BlackStringGPPointwiseInitialData::make_pointwise_vars(1.0, x);
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        if (slot == Production::c_hww || slot == Production::c_Aww)
        {
            continue;
        }
        const double harmonic =
            Production::metadata[static_cast<std::size_t>(slot)].parity ==
                    Production::ParityClass::one_z_opposite
                ? std::sin(phase)
                : std::cos(phase);
        Reduced::component(vars,
                           static_cast<Production::VariableSlot>(slot)) +=
            1.0e-10 * static_cast<double>(slot + 1) * (1.0 + 0.1 * x) *
            harmonic;
    }
    Algebraic::reconstruct(vars);
    return Reduced::store(vars);
}

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_M2B_CYCLE1_FAIL " << message << '\n';
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

void check_transform_and_classification()
{
    for (const double ratio : {0.5, 0.5625, 1.0, 2.0, 4.4375, 4.5})
    {
        const double b = std::sqrt(1.0 / ratio);
        const auto rank_condition =
            Characteristics::normalized_rank_condition(b, 1.0);
        std::cout << "M2B_TRANSFORM ratio=" << ratio
                  << " residual=" << Characteristics::roundtrip_residual(b)
                  << " rank=" << rank_condition.rank
                  << " condition=" << rank_condition.condition
                  << '\n';
        require(Characteristics::roundtrip_residual(b) <= 5.0e-12,
                "transform roundtrip exceeded tolerance");
        require(rank_condition.rank == 23,
                "transform numerical rank is below 23");
        require(rank_condition.condition <= 1.0e3,
                "transform condition bound exceeded 1e3");
        Characteristics::State state{};
        for (int index = 0; index < Characteristics::state_size; ++index)
        {
            state[static_cast<std::size_t>(index)] =
                std::sin(0.37 * static_cast<double>(index + 1));
        }
        const auto recovered = Characteristics::from_characteristics(
            b, Characteristics::to_characteristics(b, state));
        for (int index = 0; index < Characteristics::state_size; ++index)
        {
            require(std::abs(recovered[static_cast<std::size_t>(index)] -
                             state[static_cast<std::size_t>(index)]) <=
                        5.0e-12,
                    "forward/inverse family roundtrip failed");
        }
    }
    Characteristics::validate_locked_transform_gate(1.0);
    const auto horizon = Characteristics::normal_speeds(1.0, 1);
    require(std::count_if(horizon.begin(), horizon.end(), [](double value) {
                return std::abs(value) <= 5.0e-13;
            }) == 9,
            "horizon does not have nine glancing fields");
    const auto outer = Characteristics::normal_speeds(std::sqrt(2.0 / 9.0), 1);
    const auto inner = Characteristics::normal_speeds(std::sqrt(2.0), -1);
    require(std::count_if(outer.begin(), outer.end(), [](double value) {
                return value < -5.0e-13;
            }) == 11,
            "outer incoming classification mismatch");
    require(std::count_if(inner.begin(), inner.end(), [](double value) {
                return value < -5.0e-13;
            }) == 3,
            "inner incoming classification mismatch");
    require(std::abs(inner[7] + 0.874032048898) < 1.0e-10,
            "inner lapse orientation mutation was not rejected");
    require(std::abs(Characteristics::normalized_rank_condition(
                         std::sqrt(2.0), 1.0)
                         .condition -
                     36.701445735) < 1.0e-8,
            "locked full-domain condition maximum changed");
    require(std::abs(Characteristics::normalized_rank_condition(1.0, 1.0)
                         .condition -
                     31.112685441) < 1.0e-8,
            "locked horizon condition changed");
}

void check_algebraic_and_gamma()
{
    Algebraic::validate_locked_gate(1.0);
    BlackStringGammaZHelper::validate_locked_gate(1.0);
    Reduced::Variables<double> vars{};
    vars.physical.h = {1.2, 0.1, 0.9, -77.0};
    vars.physical.A = {-0.3, 0.04, 0.2, 88.0};
    Algebraic::reconstruct(vars);
    require(std::abs(Algebraic::determinant_residual(vars)) <= 5.0e-13,
            "nonlinear determinant reconstruction failed");
    require(std::abs(Algebraic::weighted_trace_residual(vars)) <= 5.0e-13,
            "nonlinear weighted trace reconstruction failed");
    bool rejected = false;
    try
    {
        (void)Algebraic::evaluate(1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
    }
    catch (const std::domain_error &)
    {
        rejected = true;
    }
    require(rejected, "singular metric mutation was accepted");
}

void check_boundary_fields()
{
    BlackStringPerturbativeRadialBoundary::validate_locked_gate(
        1.0, 0.125, 0.5, 4.5);
    for (const auto face_normal :
         {std::array<double, 2>{0.5, -1.0},
          std::array<double, 2>{4.5, 1.0}})
    {
        const double face = face_normal[0];
        const int normal = static_cast<int>(face_normal[1]);
        BlackStringPerturbativeRadialBoundary::SourceStates sources{};
        for (int source = 0; source < 5; ++source)
        {
            const double x = face - normal * (source + 0.5) * 0.125;
            sources[static_cast<std::size_t>(source)] =
                BlackStringGPPointwiseInitialData::make_pointwise_state(1.0,
                                                                         x);
            const double profile = 1.0e-9 * std::pow(source + 0.5, 4);
            sources[static_cast<std::size_t>(source)][Production::c_chi] +=
                profile;
            sources[static_cast<std::size_t>(source)][Production::c_hxz] +=
                0.2 * profile;
            auto vars = Reduced::load(sources[static_cast<std::size_t>(source)]);
            Algebraic::reconstruct(vars);
            sources[static_cast<std::size_t>(source)] = Reduced::store(vars);
        }
        const auto ghosts =
            BlackStringPerturbativeRadialBoundary::fill_characteristic_line(
                sources, face, normal, 1.0, 0.125);
        for (const auto &ghost : ghosts)
        {
            for (const double value : ghost)
            {
                require(std::isfinite(value),
                        "three-layer boundary left a nonfinite slot");
            }
            const auto vars = Reduced::load(ghost);
            require(std::abs(Algebraic::determinant_residual(vars)) <= 5.0e-13,
                    "ghost determinant residual failed");
            require(std::abs(Algebraic::weighted_trace_residual(vars)) <=
                        5.0e-13,
                    "ghost trace residual failed");
        }
    }
}

double outgoing_manufactured_error(const double dx,
                                   double &incoming_contamination)
{
    constexpr double face = 4.5;
    constexpr int normal = 1;
    constexpr int outgoing_physical_field = 3;
    constexpr double amplitude = 0.1;
    constexpr double sixth_order_coefficient = 1.0e-5;
    constexpr std::array<int, 7> primitive_slots = {
        Production::c_chi,   Production::c_hxx,    Production::c_hxz,
        Production::c_hzz,  Production::c_lapse,  Production::c_shiftX,
        Production::c_shiftZ};
    constexpr std::array<int, 7> primitive_values = {
        Characteristics::chi,   Characteristics::hxx,
        Characteristics::hxz,   Characteristics::hzz,
        Characteristics::lapse, Characteristics::shiftX,
        Characteristics::shiftZ};
    constexpr std::array<int, 7> primitive_derivatives = {
        Characteristics::qchi,   Characteristics::dxx,
        Characteristics::dxz,    Characteristics::dzz,
        Characteristics::qalpha, Characteristics::rx,
        Characteristics::rz};
    constexpr std::array<int, 9> dynamical_slots = {
        Production::c_K,      Production::c_Axx,
        Production::c_Axz,    Production::c_Azz,
        Production::c_Theta,  Production::c_GammaX,
        Production::c_GammaZ, Production::c_Bx,
        Production::c_Bz};
    constexpr std::array<int, 9> dynamical_values = {
        Characteristics::K,      Characteristics::Axx,
        Characteristics::Axz,    Characteristics::Azz,
        Characteristics::Theta,  Characteristics::GammaX,
        Characteristics::GammaZ, Characteristics::Bx,
        Characteristics::Bz};

    const double b = std::sqrt(1.0 / face);
    Characteristics::State characteristic{};
    characteristic[outgoing_physical_field] = amplitude;
    const auto face_state =
        Characteristics::from_characteristics(b, characteristic);
    const auto correction = [](const int field, const double s) {
        return sixth_order_coefficient * static_cast<double>(field + 1) *
               std::pow(s, 6);
    };
    const auto make_state = [&](const double s) {
        auto vars = BlackStringGPPointwiseInitialData::make_pointwise_vars(
            1.0, face + s);
        for (int field = 0; field < 7; ++field)
        {
            Reduced::component(
                vars, static_cast<Production::VariableSlot>(
                          primitive_slots[static_cast<std::size_t>(field)])) +=
                face_state[static_cast<std::size_t>(
                    primitive_values[static_cast<std::size_t>(field)])] +
                s * face_state[static_cast<std::size_t>(
                        primitive_derivatives[static_cast<std::size_t>(field)])] +
                correction(field, s);
        }
        for (int field = 0; field < 9; ++field)
        {
            Reduced::component(
                vars, static_cast<Production::VariableSlot>(
                          dynamical_slots[static_cast<std::size_t>(field)])) +=
                face_state[static_cast<std::size_t>(
                    dynamical_values[static_cast<std::size_t>(field)])] +
                correction(field + 7, s);
        }
        Algebraic::reconstruct(vars);
        return Reduced::store(vars);
    };

    BlackStringPerturbativeRadialBoundary::SourceStates sources{};
    for (int source = 0; source < 5; ++source)
    {
        sources[static_cast<std::size_t>(source)] =
            make_state(-(static_cast<double>(source) + 0.5) * dx);
    }
    const auto face_data =
        BlackStringPerturbativeRadialBoundary::evaluate_characteristic_face(
            sources, face, normal, 1.0, dx);
    const auto speeds = Characteristics::normal_speeds(b, normal);
    incoming_contamination = 0.0;
    for (int field = 0; field < Characteristics::state_size; ++field)
    {
        if (speeds[static_cast<std::size_t>(field)] < -5.0e-13)
        {
            incoming_contamination =
                std::max(incoming_contamination,
                         std::abs(face_data.characteristic[
                             static_cast<std::size_t>(field)]));
        }
    }
    incoming_contamination /= amplitude;

    const auto ghosts =
        BlackStringPerturbativeRadialBoundary::fill_characteristic_line(
            sources, face, normal, 1.0, dx);
    double maximum = 0.0;
    for (int layer = 0; layer < 3; ++layer)
    {
        const auto expected =
            make_state((static_cast<double>(layer) + 0.5) * dx);
        for (int slot = 0; slot < Production::NUM_VARS; ++slot)
        {
            maximum = std::max(
                maximum,
                std::abs(ghosts[static_cast<std::size_t>(layer)]
                               [static_cast<std::size_t>(slot)] -
                         expected[static_cast<std::size_t>(slot)]));
        }
    }
    return maximum;
}

void check_outgoing_manufactured_refinement()
{
    double coarse_contamination = 0.0;
    double fine_contamination = 0.0;
    const double coarse_error =
        outgoing_manufactured_error(0.125, coarse_contamination);
    const double fine_error =
        outgoing_manufactured_error(0.0625, fine_contamination);
    require(coarse_contamination <= 1.0e-8 &&
                fine_contamination <= 1.0e-8,
            "manufactured outgoing pulse contaminated incoming fields");
    require(fine_error > 0.0 && coarse_error / fine_error >= 12.0,
            "manufactured outgoing boundary refinement order failed");
}

void check_characteristic_controls()
{
    constexpr double amplitude = 1.0e-7;
    constexpr double dx = 0.125;
    constexpr std::array<int, 7> primitive_slots = {
        Production::c_chi,   Production::c_hxx,    Production::c_hxz,
        Production::c_hzz,  Production::c_lapse,  Production::c_shiftX,
        Production::c_shiftZ};
    constexpr std::array<int, 7> primitive_values = {
        Characteristics::chi,   Characteristics::hxx,
        Characteristics::hxz,   Characteristics::hzz,
        Characteristics::lapse, Characteristics::shiftX,
        Characteristics::shiftZ};
    constexpr std::array<int, 7> primitive_derivatives = {
        Characteristics::qchi,   Characteristics::dxx,
        Characteristics::dxz,    Characteristics::dzz,
        Characteristics::qalpha, Characteristics::rx,
        Characteristics::rz};
    constexpr std::array<int, 9> dynamical_slots = {
        Production::c_K,      Production::c_Axx,
        Production::c_Axz,    Production::c_Azz,
        Production::c_Theta,  Production::c_GammaX,
        Production::c_GammaZ, Production::c_Bx,
        Production::c_Bz};
    constexpr std::array<int, 9> dynamical_values = {
        Characteristics::K,      Characteristics::Axx,
        Characteristics::Axz,    Characteristics::Azz,
        Characteristics::Theta,  Characteristics::GammaX,
        Characteristics::GammaZ, Characteristics::Bx,
        Characteristics::Bz};

    for (const auto face_normal :
         {std::array<double, 2>{0.5, -1.0},
          std::array<double, 2>{4.5, 1.0}})
    {
        const double face = face_normal[0];
        const int normal = static_cast<int>(face_normal[1]);
        const double b = std::sqrt(1.0 / face);
        const auto speeds = Characteristics::normal_speeds(b, normal);
        for (int controlled = 0; controlled < Characteristics::state_size;
             ++controlled)
        {
            Characteristics::State characteristic{};
            characteristic[static_cast<std::size_t>(controlled)] = amplitude;
            const auto normal_face =
                Characteristics::from_characteristics(b, characteristic);
            BlackStringPerturbativeRadialBoundary::SourceStates sources{};
            for (int source = 0; source < 5; ++source)
            {
                const double y = static_cast<double>(source) + 0.5;
                const double x = face - normal * y * dx;
                auto vars =
                    BlackStringGPPointwiseInitialData::make_pointwise_vars(
                        1.0, x);
                for (int field = 0; field < 7; ++field)
                {
                    const double value =
                        normal_face[static_cast<std::size_t>(
                            primitive_values[static_cast<std::size_t>(field)])];
                    const double slope =
                        -dx * normal_face[static_cast<std::size_t>(
                                  primitive_derivatives[
                                      static_cast<std::size_t>(field)])];
                    Reduced::component(
                        vars, static_cast<Production::VariableSlot>(
                                  primitive_slots[
                                      static_cast<std::size_t>(field)])) +=
                        value + slope * y;
                }
                for (int field = 0; field < 9; ++field)
                {
                    Reduced::component(
                        vars, static_cast<Production::VariableSlot>(
                                  dynamical_slots[
                                      static_cast<std::size_t>(field)])) +=
                        normal_face[static_cast<std::size_t>(
                            dynamical_values[static_cast<std::size_t>(field)])];
                }
                Algebraic::reconstruct(vars);
                sources[static_cast<std::size_t>(source)] =
                    Reduced::store(vars);
            }
            const auto evaluated =
                BlackStringPerturbativeRadialBoundary::
                    evaluate_characteristic_face(sources, face, normal, 1.0,
                                                 dx);
            for (int field = 0; field < Characteristics::state_size; ++field)
            {
                const double expected =
                    field == controlled &&
                            speeds[static_cast<std::size_t>(field)] >=
                                -5.0e-13
                        ? amplitude
                        : 0.0;
                require(std::abs(evaluated.characteristic[
                                     static_cast<std::size_t>(field)] -
                                 expected) <= 2.0e-13,
                        "physical/constraint/gauge/glancing control failed");
            }
        }
    }
}

void check_leveldata_boundary_ownership()
{
    constexpr int nx = 32;
    constexpr int nz = 16;
    ProblemDomain domain(Box(iv(0, 0), iv(nx - 1, nz - 1)));
    domain.setPeriodic(0, false);
    domain.setPeriodic(1, true);
    Vector<Box> boxes;
    boxes.push_back(Box(iv(0, 0), iv(15, 7)));
    boxes.push_back(Box(iv(0, 8), iv(15, 15)));
    boxes.push_back(Box(iv(16, 0), iv(31, 7)));
    boxes.push_back(Box(iv(16, 8), iv(31, 15)));
    Vector<int> processors(4, 0);
    for (int index = 0; index < 4; ++index)
    {
        processors[index] = index % numProc();
    }
    DisjointBoxLayout layout(boxes, processors, domain);
    GRLevelData state;
    state.define(layout, Production::NUM_VARS, 3 * IntVect::Unit);
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        FArrayBox &fab = state[dit];
        fab.setVal(std::numeric_limits<double>::quiet_NaN());
        for (BoxIterator bit(layout[dit]); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            const auto expected = manufactured_state(point[0], point[1]);
            for (int slot = 0; slot < Production::NUM_VARS; ++slot)
            {
                fab(point, slot) = expected[static_cast<std::size_t>(slot)];
            }
        }
    }
    state.exchange(Interval(0, Production::NUM_VARS - 1));
    const std::array<double, CH_SPACEDIM> offset = {-0.5, 0.0};
    BlackStringPerturbativeRadialBoundary::fill_solution_ghosts(
        state, domain, 1.0, 0.125, offset, Side::Lo,
        Interval(0, Production::NUM_VARS - 1));
    BlackStringPerturbativeRadialBoundary::fill_solution_ghosts(
        state, domain, 1.0, 0.125, offset, Side::Hi,
        Interval(0, Production::NUM_VARS - 1));

    std::vector<double> physical_ghost_snapshot;
    int local_physical_ghost_points = 0;
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = layout[dit];
        const FArrayBox &fab = state[dit];
        for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            const bool radial_ghost = point[0] < 0 || point[0] >= nx;
            if (radial_ghost)
            {
                ++local_physical_ghost_points;
                Reduced::Storage<double> storage{};
                for (int slot = 0; slot < Production::NUM_VARS; ++slot)
                {
                    const double value = fab(point, slot);
                    require(std::isfinite(value),
                            "one of 18 physical ghost slots was unfilled");
                    storage[static_cast<std::size_t>(slot)] = value;
                    physical_ghost_snapshot.push_back(value);
                }
                const auto vars = Reduced::load(storage);
                require(std::abs(Algebraic::determinant_residual(vars)) <=
                            5.0e-13 &&
                            std::abs(Algebraic::weighted_trace_residual(vars)) <=
                                5.0e-13,
                        "dependent physical ghost reconstruction failed");
                continue;
            }
            if (valid.contains(point) || domain.domainBox().contains(point))
            {
                const auto expected = manufactured_state(point[0], point[1]);
                for (int slot = 0; slot < Production::NUM_VARS; ++slot)
                {
                    const double scale =
                        1.0 + std::abs(expected[static_cast<std::size_t>(slot)]);
                    require(std::abs(fab(point, slot) -
                                     expected[static_cast<std::size_t>(slot)]) <=
                                5.0e-14 * scale,
                            "valid cell or internal MPI seam was overwritten");
                }
            }
        }
    }
    require(local_physical_ghost_points > 0 || numProc() > 1,
            "medium layout exposed no local physical ghost points");

    for (int radial : {0, nx - 1})
    {
        for (int compact = 0; compact < nz; ++compact)
        {
            const auto positive = manufactured_state(radial, compact);
            const auto reflected =
                manufactured_state(radial, nz - 1 - compact);
            const auto gp =
                BlackStringGPPointwiseInitialData::make_pointwise_state(
                    1.0, 0.5 + (static_cast<double>(radial) + 0.5) * 0.125);
            for (int slot = 0; slot < Production::NUM_VARS; ++slot)
            {
                const double sign =
                    Production::metadata[static_cast<std::size_t>(slot)]
                                .parity ==
                            Production::ParityClass::one_z_opposite
                        ? -1.0
                        : 1.0;
                require(std::abs(
                            (reflected[static_cast<std::size_t>(slot)] -
                             gp[static_cast<std::size_t>(slot)]) -
                            sign *
                                (positive[static_cast<std::size_t>(slot)] -
                                 gp[static_cast<std::size_t>(slot)])) <=
                            5.0e-13,
                        "all-slot parity classification failed");
            }
        }
    }

    BlackStringPerturbativeRadialBoundary::fill_solution_ghosts(
        state, domain, 1.0, 0.125, offset, Side::Lo,
        Interval(0, Production::NUM_VARS - 1));
    BlackStringPerturbativeRadialBoundary::fill_solution_ghosts(
        state, domain, 1.0, 0.125, offset, Side::Hi,
        Interval(0, Production::NUM_VARS - 1));
    std::size_t snapshot_index = 0;
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const FArrayBox &fab = state[dit];
        for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            if (point[0] < 0 || point[0] >= nx)
            {
                for (int slot = 0; slot < Production::NUM_VARS; ++slot)
                {
                    require(fab(point, slot) ==
                                physical_ghost_snapshot[snapshot_index++],
                            "physical boundary was double-applied");
                }
            }
        }
    }
    require(snapshot_index == physical_ghost_snapshot.size(),
            "physical ghost lifecycle snapshot mismatch");
}

void check_seed()
{
    const Seed::Configuration configuration{
        32, 16, 1, 1.0, 0.125, 0.5, 0.0, 1.0e-9};
    const auto raw = Seed::raw_seed(configuration);
    const auto raw_residual = Seed::centered_constraint_direction(
        configuration, raw, 1.0 / 1048576.0);
    double raw_maximum = 0.0;
    for (const double value : raw_residual)
    {
        raw_maximum = std::max(raw_maximum, std::abs(value));
    }
    std::cout << "M2B_SEED_RAW_MAXIMUM=" << raw_maximum << '\n';
    const Seed::Solution positive = Seed::solve(configuration);
    require(positive.report.rank > 0,
            "seed correction retained no singular values");
    require(positive.report.retained_condition <= 1.0e12,
            "seed correction condition limit failed");
    require(positive.report.scaled_rms <= 1.0e-17,
            "seed differential RMS tolerance failed");
    require(positive.report.max_hm <= 5.0e-17,
            "seed H/M maximum tolerance failed");
    require(positive.report.max_gamma_z <= 1.0e-19,
            "seed Gamma/Z maximum tolerance failed");
    require(positive.report.incoming_boundary <= 1.0e-20,
            "seed incoming characteristic tolerance failed");
    require(positive.report.retained_physical_fraction >= 0.25,
            "seed physical deformation collapsed");
    Seed::Configuration negative_configuration = configuration;
    negative_configuration.epsilon = -configuration.epsilon;
    const Seed::Solution negative = Seed::solve(negative_configuration);
    for (std::size_t index = 0; index < positive.amplitudes.size(); ++index)
    {
        require(std::abs(positive.amplitudes[index] +
                         negative.amplitudes[index]) <= 5.0e-22,
                "signed seed antisymmetry failed");
    }
    for (int radial = 0; radial < Seed::boundary_buffer; ++radial)
    {
        for (int field = 0; field < Seed::correction_fields; ++field)
        {
            require(positive.amplitude(radial, field) == 0.0 &&
                        positive.amplitude(configuration.radial_cells - 1 -
                                               radial,
                                           field) == 0.0,
                    "seed boundary buffer was modified");
        }
    }
}
} // namespace

int main(int argc, char **argv)
{
#ifdef CH_MPI
    MPI_Init(&argc, &argv);
#else
    (void)argc;
    (void)argv;
#endif
    check_transform_and_classification();
    check_algebraic_and_gamma();
    check_boundary_fields();
    check_characteristic_controls();
    check_outgoing_manufactured_refinement();
    check_leveldata_boundary_ownership();
    check_seed();
    if (procID() == 0)
    {
        std::cout << "M2B_TRANSFORM_GATE=PASS\n"
                  << "M2B_GAMMA_Z_GATE=PASS\n"
                  << "M2B_RECONSTRUCTION_GATE=PASS\n"
                  << "M2B_CHARACTERISTIC_BOUNDARY_GATE=PASS\n"
                  << "M2B_CORRECTED_SEED_GATE=PASS\n"
                  << "BLACKSTRING_M2B_CYCLE1_FOCUSED_PASS\n";
    }
#ifdef CH_MPI
    MPI_Finalize();
#endif
    return 0;
}
