#include "BlackStringToyLevel.hpp"

#include "BlackStringBoundaryDimensionAudit.hpp"
#include "DefaultLevelFactory.hpp"
#include "GRAMR.hpp"
#include "GRParmParse.hpp"
#include "SetupFunctions.hpp"
#include "SimulationParameters.hpp"
#include "UserVariables.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

// Compile the real project level implementation into this strict fixture.
#include "../../BlackStringToyLevel.cpp"

namespace
{
enum class Mutation
{
    exact,
    missing,
    wrong_coordinate,
    cadence_disabled,
    duplicate_exchange_baseline
};

class DiagnosticAccessLevel : public BlackStringToyLevel
{
  protected:
    using BlackStringToyLevel::BlackStringToyLevel;

  public:
    struct RHSProbe
    {
        std::array<double, NUM_VARS> maximum{};
        bool finite = true;
        IntVect worst_point = IntVect::Zero;
        int worst_component = 0;
    };

    RHSProbe probe_rhs()
    {
        GRLevelData rhs;
        defineRHSData(rhs, m_state_new);
        LevelFluxRegister fine_register;
        LevelFluxRegister coarse_register;
        const GRLevelData undefined_coarse;
        evalRHS(rhs, m_state_new, fine_register, coarse_register,
                undefined_coarse, 0.0, undefined_coarse, 0.0, m_time, 0.0);

        RHSProbe probe;
        probe.maximum.fill(0.0);
        const DataIterator iterator = rhs.dataIterator();
        double worst = 0.0;
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = rhs[data_index];
            const Box valid = m_grids[data_index] & m_problem_domain.domainBox();
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    const double value = fab(point, component);
                    probe.finite = probe.finite && std::isfinite(value);
                    const double magnitude = std::abs(value);
                    probe.maximum[static_cast<std::size_t>(component)] =
                        std::max(
                            probe.maximum[static_cast<std::size_t>(component)],
                            magnitude);
                    if (!std::isfinite(value) || magnitude > worst)
                    {
                        worst = magnitude;
                        probe.worst_point = point;
                        probe.worst_component = component;
                    }
                }
            }
        }
        return probe;
    }

  protected:
    void fillIntralevelGhosts(const Interval &components) override
    {
        // Mirror the locked GRAMR implementation while recording the actual
        // framework exchange. fillBdyGhosts remains radial-only.
        instrumentation().begin_fillall_periodic_exchange();
        m_state_new.exchange(components, m_exchange_copier);
        fillBdyGhosts(m_state_new, components);
    }
};

class ExactDiagnosticLevel : public DiagnosticAccessLevel
{
    friend class DefaultLevelFactory<ExactDiagnosticLevel>;
    using DiagnosticAccessLevel::DiagnosticAccessLevel;
};

class MissingRadialGhostLevel : public DiagnosticAccessLevel
{
    friend class DefaultLevelFactory<MissingRadialGhostLevel>;
    using DiagnosticAccessLevel::DiagnosticAccessLevel;

  protected:
    void fillBdyGhosts(GRLevelData &state,
                       const Interval &components) override
    {
        // Live-path negative control: the framework has already exchanged
        // periodic ghosts; omit only the physical radial GP fills.
        (void)state;
        (void)components;
    }
};

class WrongCoordinateRadialGhostLevel : public DiagnosticAccessLevel
{
    friend class DefaultLevelFactory<WrongCoordinateRadialGhostLevel>;
    using DiagnosticAccessLevel::DiagnosticAccessLevel;

  protected:
    void fillBdyGhosts(GRLevelData &state,
                       const Interval &components) override
    {
        auto wrong_offset = m_p.coordinate_offset();
        wrong_offset[BlackStringLive::radial_direction] += 0.5 * m_dx;
        (void)components;
        fill_background_radial_ghosts(state, wrong_offset);
    }
};

class DuplicateExchangeBaselineLevel : public DiagnosticAccessLevel
{
    friend class DefaultLevelFactory<DuplicateExchangeBaselineLevel>;
    using DiagnosticAccessLevel::DiagnosticAccessLevel;

  protected:
    void fillBdyGhosts(GRLevelData &state,
                       const Interval &components) override
    {
        BlackStringToyLevel::fillBdyGhosts(state, components);
        state.exchange(components, m_exchange_copier);
        instrumentation().record_legacy_policy_duplicate_exchange();
    }
};

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_LEVEL_ZERO_EVOLUTION_FAIL " << message << '\n';
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

Mutation parse_mutation(const std::string &name)
{
    if (name == "exact")
    {
        return Mutation::exact;
    }
    if (name == "missing")
    {
        return Mutation::missing;
    }
    if (name == "wrong-coordinate")
    {
        return Mutation::wrong_coordinate;
    }
    if (name == "cadence-disabled")
    {
        return Mutation::cadence_disabled;
    }
    if (name == "duplicate-exchange-baseline")
    {
        return Mutation::duplicate_exchange_baseline;
    }
    fail("mutation must be exact, missing, wrong-coordinate, or "
         "cadence-disabled, or duplicate-exchange-baseline");
}

template <class level_t>
int run_diagnostic(SimulationParameters &parameters, const Mutation mutation)
{
    require(parameters.max_level == 0, "fixture must remain level zero");
    require(parameters.background_preserving_gp_radial_ghosts,
            "diagnostic analytic radial ghosts must be enabled");
    require(parameters.fixed_lapse_source,
            "fixed lapse source must remain enabled");
    require(parameters.constraint_diagnostic_cadence == 1,
            "E2 must collect target constraints every step");
    require(!parameters.constraint_diagnostic_due(0) &&
                parameters.constraint_diagnostic_due(1),
            "constraint cadence scheduling contract changed");

    GRAMR amr;
    DefaultLevelFactory<level_t> factory(amr, parameters);
    setupAMRObject(amr, factory);

    Vector<AMRLevel *> levels = amr.getAMRLevels();
    require(levels.size() == 1,
            "AMR::define/setup must create exactly one level");
    auto *level = dynamic_cast<level_t *>(levels[0]);
    require(level != nullptr, "factory returned the wrong level type");

    const auto initial_diagnostic = level->collect_evolution_diagnostic();
    require(initial_diagnostic.finite_state,
            "initialized state or ghosts contain a nonfinite value");
    if (mutation == Mutation::exact)
    {
        require(initial_diagnostic.maximum_state_change == 0.0,
                "live initializer differs from stationary GP data");
        require(initial_diagnostic.radial_ghosts_correct,
                "radial GP ghosts are wrong before the first RHS");
        require(initial_diagnostic.periodic_exchange_correct,
                "periodic z ghosts are wrong before the first RHS");
    }

    const auto rhs_probe = level->probe_rhs();
    std::cout << std::scientific << std::setprecision(17)
              << "E2_INITIAL_RHS finite=" << rhs_probe.finite
              << " worst_component=" << rhs_probe.worst_component
              << " worst_point=" << rhs_probe.worst_point
              << " worst_magnitude="
              << rhs_probe.maximum[static_cast<std::size_t>(
                     rhs_probe.worst_component)]
              << '\n';
    if (mutation == Mutation::exact)
    {
        require(rhs_probe.finite, "initial live RHS contains a nonfinite value");
    }

    if (mutation != Mutation::exact)
    {
        const bool oracle_rejected =
            !initial_diagnostic.radial_ghosts_correct ||
            initial_diagnostic.maximum_radial_ghost_error > 1.0e-13 ||
            !rhs_probe.finite ||
            rhs_probe.maximum[static_cast<std::size_t>(
                rhs_probe.worst_component)] >
                1.0e-2;
        require(oracle_rejected,
                "live radial ghost mutation escaped the pre-RK oracle");
        std::cout << "E2_MUTATION "
                  << (mutation == Mutation::missing ? "missing"
                                                    : "wrong_coordinate")
                  << "=REJECT\n";
        amr.conclude();
        return 0;
    }

    amr.run(parameters.stop_time, parameters.max_steps);

    const auto diagnostic = level->collect_evolution_diagnostic();
    const auto &instrumentation = level->instrumentation_report();
    const std::size_t valid_cells =
        static_cast<std::size_t>(diagnostic.radial_cells) *
        static_cast<std::size_t>(diagnostic.compact_cells);
    const double dx = parameters.coarsest_dx;
    const double dz = dx;
    const double dt =
        diagnostic.final_time /
        static_cast<double>(instrumentation.advance_calls);
    const double cfl = dt / dx;
    const std::size_t old_policy_duplicate_exchanges =
        instrumentation.ghost_fill_calls;
    const std::size_t old_total_periodic_exchanges =
        instrumentation.periodic_exchanges +
        old_policy_duplicate_exchanges;

    std::cout << std::scientific << std::setprecision(17)
              << "E2_SUMMARY radial_cells=" << diagnostic.radial_cells
              << " compact_cells=" << diagnostic.compact_cells
              << " steps=" << instrumentation.advance_calls
              << " final_time=" << diagnostic.final_time
              << " state_max=" << diagnostic.maximum_state_change
              << " lapse_drift=" << diagnostic.maximum_lapse_drift << '\n';
    std::cout << "E2_DOMAIN Lx="
              << dx * static_cast<double>(diagnostic.radial_cells)
              << " Lz="
              << dz * static_cast<double>(diagnostic.compact_cells)
              << " dx=" << dx << " dz=" << dz << " dt=" << dt
              << " CFL=" << cfl
              << " steps=" << instrumentation.advance_calls
              << " final_time=" << diagnostic.final_time << '\n';
    for (int component = 0; component < NUM_VARS; ++component)
    {
        std::cout << "E2_VARIABLE slot=" << component
                  << " name=" << UserVariables::variable_names[component]
                  << " max_change="
                  << diagnostic.maximum_change[static_cast<std::size_t>(
                         component)]
                  << '\n';
    }
    std::cout << "E2_CONSTRAINT H=" << diagnostic.constraint_maximum[0]
              << " Mx=" << diagnostic.constraint_maximum[1]
              << " Mz=" << diagnostic.constraint_maximum[2] << '\n'
              << "E2_GHOST low_invocations="
              << instrumentation.low_radial_fills
              << " high_invocations="
              << instrumentation.high_radial_fills
              << " radial_refreshes="
              << instrumentation.radial_refreshes
              << " old_policy_duplicate_exchanges="
              << old_policy_duplicate_exchanges
              << " old_total_periodic_exchanges="
              << old_total_periodic_exchanges
              << " periodic_exchanges="
              << instrumentation.periodic_exchanges
              << " fillall_exchanges="
              << instrumentation.fillall_periodic_exchanges
              << " rhs_exchanges="
              << instrumentation.rhs_periodic_exchanges
              << " radial_error="
              << diagnostic.maximum_radial_ghost_error
              << " periodic_error="
              << diagnostic.maximum_periodic_ghost_error << '\n'
              << "E2_LIFECYCLE define=1 initialization="
              << instrumentation.initial_data_calls << " rhs="
              << instrumentation.rhs_calls << " update="
              << instrumentation.update_calls << " cleanup="
              << instrumentation.cleanup_calls << " ghost_fill="
              << instrumentation.ghost_fill_calls << " advance="
              << instrumentation.advance_calls << " post_step="
              << instrumentation.post_step_calls << " diagnostics="
              << instrumentation.diagnostic_evaluations << '\n'
              << "E2_COUNTS valid_cells=" << valid_cells
              << " rhs_evaluations=" << instrumentation.rhs_calls
              << " periodic_exchanges="
              << instrumentation.periodic_exchanges
              << " radial_ghost_fills="
              << instrumentation.radial_refreshes
              << " diagnostic_evaluations="
              << instrumentation.diagnostic_evaluations << '\n'
              << "E2_GHOST_ORDER exchange_before_radial="
              << instrumentation.exchange_before_radial
              << " radial_low_then_high="
              << instrumentation.radial_low_then_high
              << " radial_before_rhs="
              << instrumentation.radial_before_rhs << '\n';

    require(diagnostic.finite_state, "evolved state contains a nonfinite value");
    require(diagnostic.finite_constraints,
            "constraint state contains a nonfinite value");
    require(diagnostic.radial_ghosts_correct,
            "exact radial GP ghost values changed");
    require(diagnostic.periodic_exchange_correct,
            "periodic z ghost exchange changed");
    require(instrumentation.low_radial_fills > 0 &&
                instrumentation.high_radial_fills > 0,
            "both radial sides must be invoked");
    require(instrumentation.periodic_exchanges ==
                instrumentation.fillall_periodic_exchanges +
                    instrumentation.rhs_periodic_exchanges,
            "each logical periodic refresh must have exactly one owner");
    require(instrumentation.rhs_periodic_exchanges ==
                instrumentation.rhs_calls,
            "evalRHS must own exactly one periodic exchange");
    require(instrumentation.periodic_exchanges <
                old_total_periodic_exchanges,
            "duplicate radial-policy periodic exchanges were not removed");
    require(instrumentation.exchange_before_radial &&
                instrumentation.radial_low_then_high &&
                instrumentation.radial_before_rhs,
            "framework exchange/radial fill/RHS call order changed");
    require(instrumentation.initial_data_calls == 1,
            "initialization must execute exactly once");
    require(instrumentation.rhs_calls > 0 &&
                instrumentation.update_calls > 0,
            "real RK RHS and update paths must execute");
    require(instrumentation.cleanup_calls ==
                instrumentation.update_calls,
            "cleanup must execute once per RK update");
    require(instrumentation.advance_calls ==
                static_cast<std::size_t>(parameters.max_steps),
            "time advancement count differs from max_steps");
    require(instrumentation.post_step_calls ==
                instrumentation.advance_calls,
            "post-step diagnostics must execute after every step");
    require(instrumentation.diagnostic_evaluations ==
                instrumentation.advance_calls,
            "cadence one must evaluate constraints every step");
    require(diagnostic.final_time > 0.0,
            "real time advancement did not occur");
    require(diagnostic.maximum_state_change < 1.0e-2,
            "unperturbed GP state left the bounded diagnostic tolerance");
    require(diagnostic.maximum_lapse_drift < 1.0e-3,
            "sourced lapse drift left the bounded diagnostic tolerance");
    require(*std::max_element(diagnostic.constraint_maximum.begin(),
                              diagnostic.constraint_maximum.end()) <
                1.0e3,
            "constraints are not bounded");

    std::cout << "BLACKSTRING_LEVEL_ZERO_EVOLUTION_PASS\n";
    amr.conclude();
    return 0;
}

template <class level_t>
int run_disabled_cadence(SimulationParameters &parameters)
{
    parameters.constraint_diagnostic_cadence = 0;
    parameters.max_steps = 1;
    parameters.stop_time = parameters.coarsest_dx * parameters.dt_multiplier;
    require(!parameters.constraint_diagnostic_due(1),
            "zero cadence must disable constraint diagnostics");

    GRAMR amr;
    DefaultLevelFactory<level_t> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    require(levels.size() == 1,
            "disabled-cadence setup must create one level");
    auto *level = dynamic_cast<level_t *>(levels[0]);
    require(level != nullptr,
            "disabled-cadence factory returned the wrong level type");
    amr.run(parameters.stop_time, parameters.max_steps);
    const auto &instrumentation = level->instrumentation_report();
    require(instrumentation.advance_calls == 1,
            "disabled-cadence probe must advance once");
    require(instrumentation.post_step_calls == 1,
            "disabled-cadence post-step hook must execute once");
    require(instrumentation.diagnostic_evaluations == 0,
            "zero cadence executed the constraint loop");
    std::cout << "E2_DIAGNOSTIC_CADENCE cadence=0 steps=1 evaluations=0\n"
              << "BLACKSTRING_LEVEL_ZERO_CADENCE_DISABLED_PASS\n";
    amr.conclude();
    return 0;
}

template <class level_t>
int run_duplicate_exchange_baseline(SimulationParameters &parameters)
{
    GRAMR amr;
    DefaultLevelFactory<level_t> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    require(levels.size() == 1,
            "duplicate-exchange baseline must create one level");
    auto *level = dynamic_cast<level_t *>(levels[0]);
    require(level != nullptr,
            "duplicate-exchange baseline factory returned the wrong type");

    const auto initial = level->collect_evolution_diagnostic();
    require(initial.radial_ghosts_correct &&
                initial.periodic_exchange_correct,
            "duplicate-exchange baseline changed initial ghost values");
    const auto rhs_probe = level->probe_rhs();
    require(rhs_probe.finite,
            "duplicate-exchange baseline initial RHS is nonfinite");
    amr.run(parameters.stop_time, parameters.max_steps);
    const auto final = level->collect_evolution_diagnostic();
    const auto &instrumentation = level->instrumentation_report();
    require(final.radial_ghosts_correct &&
                final.periodic_exchange_correct,
            "duplicate-exchange baseline changed final ghost values");
    require(instrumentation.legacy_policy_duplicate_exchanges ==
                instrumentation.ghost_fill_calls,
            "legacy policy baseline must exchange after every radial fill");

    const std::size_t valid_cells =
        static_cast<std::size_t>(final.radial_cells) *
        static_cast<std::size_t>(final.compact_cells);
    std::cout << "E2_OLD_BASELINE valid_cells=" << valid_cells
              << " rhs_evaluations=" << instrumentation.rhs_calls
              << " periodic_exchanges="
              << instrumentation.periodic_exchanges
              << " policy_duplicate_exchanges="
              << instrumentation.legacy_policy_duplicate_exchanges
              << " radial_ghost_fills="
              << instrumentation.radial_refreshes
              << " diagnostic_evaluations="
              << instrumentation.diagnostic_evaluations << '\n'
              << "BLACKSTRING_LEVEL_ZERO_DUPLICATE_EXCHANGE_BASELINE_PASS\n";
    amr.conclude();
    return 0;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fail("usage: BlackStringLevelZeroEvolutionTest <params> "
             "<exact|missing|wrong-coordinate|cadence-disabled|"
             "duplicate-exchange-baseline>");
    }

    const Mutation mutation = parse_mutation(argv[2]);
    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);

#ifdef BLACKSTRING_BOUNDARY_LEGACY_TENSOR_LOOP
    // Parameter parsing safely observes the legacy direction. Arm the hard
    // rejection only for the real level-zero setup call path.
    BlackStringBoundaryDimensionAudit::arm_non_grid_rejection();
#endif

    if (mutation == Mutation::cadence_disabled)
    {
        return run_disabled_cadence<ExactDiagnosticLevel>(parameters);
    }
    if (mutation == Mutation::duplicate_exchange_baseline)
    {
        return run_duplicate_exchange_baseline<
            DuplicateExchangeBaselineLevel>(parameters);
    }
    if (mutation == Mutation::exact)
    {
        return run_diagnostic<ExactDiagnosticLevel>(parameters, mutation);
    }
    if (mutation == Mutation::missing)
    {
        return run_diagnostic<MissingRadialGhostLevel>(parameters, mutation);
    }
    return run_diagnostic<WrongCoordinateRadialGhostLevel>(parameters,
                                                            mutation);
}
