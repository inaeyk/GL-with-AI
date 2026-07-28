#include "BlackStringToyLevel.hpp"

#include "BlackStringGPInitialData.hpp"
#include "BlackStringLive.hpp"
#include "BoxLoops.hpp"
#include "BoxIterator.H"
#include "CH_Timer.H"
#include "Interval.H"
#include "UserVariables.hpp"
#include "parstream.H"

#include <algorithm>
#include <cmath>
#include <limits>

namespace
{
bool sample_component(const GRLevelData &state, const IntVect &point,
                      const int component, double &value)
{
    const DataIterator iterator = state.dataIterator();
    for (int ibox = 0; ibox < iterator.size(); ++ibox)
    {
        const DataIndex data_index = iterator[ibox];
        const FArrayBox &fab = state[data_index];
        if (fab.box().contains(point))
        {
            value = fab(point, component);
            return true;
        }
    }
    return false;
}
} // namespace

void BlackStringToyLevel::specificAdvance()
{
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_advance();
#endif
    // Cleanup is owned by specificUpdateODE, after every normal RK update.
}

void BlackStringToyLevel::initialData()
{
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_initial_data();
#endif
    CH_TIME("BlackStringToyLevel::initialData");
    if (m_verbosity)
    {
        pout() << "BlackStringToyLevel::initialData " << m_level << endl;
    }

    BoxLoops::loop(
        BlackStringGPInitialData::make_compute(
            m_p.r0, m_dx, m_p.coordinate_offset()),
        m_state_new, m_state_new, EXCLUDE_GHOST_CELLS, disable_simd());

    // The initializer owns valid cells only. Chombo owns intralevel exchange,
    // periodic z wrapping, coarse/fine fill, and configured radial boundaries.
    fillAllGhosts();
}

void BlackStringToyLevel::specificEvalRHS(GRLevelData &a_soln,
                                          GRLevelData &a_rhs,
                                          const double a_time)
{
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    // GRAMRLevel::evalRHS has just performed the one framework-owned
    // intralevel/periodic exchange, followed by the radial-only fill.
    m_instrumentation.record_rhs_after_framework_refresh();
#endif
    (void)a_time;
    BoxLoops::loop(
        BlackStringLive::RHSCompute(
            m_p.r0, m_dx, m_p.coordinate_offset(), m_p.gauge,
            m_p.fixed_lapse_source),
        a_soln, a_rhs, EXCLUDE_GHOST_CELLS, disable_simd());
}

void BlackStringToyLevel::specificUpdateODE(GRLevelData &a_soln,
                                            const GRLevelData &a_rhs,
                                            const Real a_dt)
{
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_update();
#endif
    (void)a_rhs;
    (void)a_dt;
    BoxLoops::loop(
        BlackStringLive::CleanupCompute(m_p.min_chi, m_p.min_lapse),
        a_soln, a_soln, EXCLUDE_GHOST_CELLS, disable_simd());
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_cleanup();
#endif
}

void BlackStringToyLevel::preTagCells()
{
    fillAllGhosts(VariableType::evolution, Interval(c_chi, c_chi));
}

void BlackStringToyLevel::computeTaggingCriterion(
    FArrayBox &tagging_criterion, const FArrayBox &current_state,
    const FArrayBox &current_state_diagnostics)
{
    (void)current_state;
    (void)current_state_diagnostics;
    tagging_criterion.setVal(0.0);
}

void BlackStringToyLevel::specificPostTimeStep()
{
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_post_step();
#endif
    const auto completed_steps =
        static_cast<std::size_t>(std::llround(m_time / m_dt));
    if (!m_p.constraint_diagnostic_due(completed_steps))
    {
        return;
    }
    fillAllGhosts();
    BoxLoops::loop(
        BlackStringLive::ConstraintCompute(m_dx, m_p.coordinate_offset()),
        m_state_new, m_state_diagnostics, EXCLUDE_GHOST_CELLS,
        disable_simd());
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_diagnostic_evaluation();
#endif
}

void BlackStringToyLevel::fill_background_radial_ghosts(
    GRLevelData &state,
    const std::array<double, CH_SPACEDIM> &coordinate_offset)
{
    BlackStringRadialGPGhost::fill(state, m_problem_domain, m_p.r0, m_dx,
                                   coordinate_offset, Side::Lo);
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_radial_fill(Side::Lo);
#endif
    BlackStringRadialGPGhost::fill(state, m_problem_domain, m_p.r0, m_dx,
                                   coordinate_offset, Side::Hi);
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_radial_fill(Side::Hi);
#endif

    // Chombo already owns the intralevel and pure-periodic-z exchange.
    // This policy owns only the physical radial strips, including their
    // radial/periodic corners. The exact GP background is z-independent.
}

void BlackStringToyLevel::fillBdyGhosts(GRLevelData &state,
                                        const Interval &components)
{
#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    m_instrumentation.record_ghost_fill();
#endif
    if (!m_p.background_preserving_gp_radial_ghosts)
    {
        GRAMRLevel::fillBdyGhosts(state, components);
        return;
    }
    fill_background_radial_ghosts(state, m_p.coordinate_offset());
}

void BlackStringToyLevel::copyBdyGhosts(const GRLevelData &source,
                                        GRLevelData &destination)
{
    if (!m_p.background_preserving_gp_radial_ghosts)
    {
        GRAMRLevel::copyBdyGhosts(source, destination);
        return;
    }
    (void)source;
    fillBdyGhosts(destination);
}

BlackStringToyLevel::EvolutionDiagnostic
BlackStringToyLevel::collect_evolution_diagnostic()
{
    EvolutionDiagnostic report;
    report.maximum_change.fill(0.0);
    report.constraint_maximum.fill(0.0);
    report.final_time = m_time;
    report.radial_cells =
        m_problem_domain.domainBox().size(BlackStringLive::radial_direction);
    report.compact_cells =
        m_problem_domain.domainBox().size(BlackStringLive::compact_direction);

    fillAllGhosts();
    const auto coordinate_offset = m_p.coordinate_offset();
    const Box domain = m_problem_domain.domainBox();
    const DataIterator iterator = m_state_new.dataIterator();
    for (int ibox = 0; ibox < iterator.size(); ++ibox)
    {
        const DataIndex data_index = iterator[ibox];
        const FArrayBox &state = m_state_new[data_index];
        const FArrayBox &constraints = m_state_diagnostics[data_index];
        const Box valid = m_grids[data_index] & domain;

        for (BoxIterator bit(state.box()); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            for (int component = 0; component < NUM_VARS; ++component)
            {
                report.finite_state =
                    report.finite_state &&
                    std::isfinite(state(point, component));
            }
        }

        for (BoxIterator bit(valid); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            const double x =
                BlackStringCoordinates::cell_centered<double>(
                    point[BlackStringLive::radial_direction], m_dx,
                    coordinate_offset[BlackStringLive::radial_direction]);
            const auto oracle =
                BlackStringGPPointwiseInitialData::make_pointwise_state(
                    m_p.r0, x);
            for (int component = 0; component < NUM_VARS; ++component)
            {
                const double difference =
                    std::abs(state(point, component) -
                             oracle[static_cast<std::size_t>(component)]);
                report.maximum_change[static_cast<std::size_t>(component)] =
                    std::max(
                        report.maximum_change[static_cast<std::size_t>(
                            component)],
                        difference);
                report.maximum_state_change =
                    std::max(report.maximum_state_change, difference);
            }
            report.maximum_lapse_drift =
                std::max(report.maximum_lapse_drift,
                         std::abs(state(point, c_lapse) - 1.0));
            for (int component = 0; component < NUM_DIAGNOSTIC_VARS;
                 ++component)
            {
                report.finite_constraints =
                    report.finite_constraints &&
                    std::isfinite(constraints(point, component));
                report.constraint_maximum[static_cast<std::size_t>(
                    component)] =
                    std::max(
                        report.constraint_maximum[static_cast<std::size_t>(
                            component)],
                        std::abs(constraints(point, component)));
            }
        }
    }

    const int radial_midpoint =
        domain.smallEnd(BlackStringLive::radial_direction) +
        domain.size(BlackStringLive::radial_direction) / 2;
    for (int ghost = 1; ghost <= m_num_ghosts; ++ghost)
    {
        IntVect low_ghost = domain.smallEnd();
        IntVect low_source = domain.smallEnd();
        low_ghost[BlackStringLive::radial_direction] = radial_midpoint;
        low_source[BlackStringLive::radial_direction] = radial_midpoint;
        low_ghost[BlackStringLive::compact_direction] -= ghost;
        low_source[BlackStringLive::compact_direction] =
            domain.bigEnd(BlackStringLive::compact_direction) - ghost + 1;

        IntVect high_ghost = domain.bigEnd();
        IntVect high_source = domain.bigEnd();
        high_ghost[BlackStringLive::radial_direction] = radial_midpoint;
        high_source[BlackStringLive::radial_direction] = radial_midpoint;
        high_ghost[BlackStringLive::compact_direction] += ghost;
        high_source[BlackStringLive::compact_direction] =
            domain.smallEnd(BlackStringLive::compact_direction) + ghost - 1;

        for (int component = 0; component < NUM_VARS; ++component)
        {
            double low_ghost_value = std::numeric_limits<double>::quiet_NaN();
            double low_source_value = std::numeric_limits<double>::quiet_NaN();
            double high_ghost_value = std::numeric_limits<double>::quiet_NaN();
            double high_source_value =
                std::numeric_limits<double>::quiet_NaN();
            const bool found =
                sample_component(m_state_new, low_ghost, component,
                                 low_ghost_value) &&
                sample_component(m_state_new, low_source, component,
                                 low_source_value) &&
                sample_component(m_state_new, high_ghost, component,
                                 high_ghost_value) &&
                sample_component(m_state_new, high_source, component,
                                 high_source_value);
            report.periodic_exchange_correct =
                report.periodic_exchange_correct && found;
            if (found)
            {
                report.maximum_periodic_ghost_error =
                    std::max(report.maximum_periodic_ghost_error,
                             std::abs(low_ghost_value - low_source_value));
                report.maximum_periodic_ghost_error =
                    std::max(report.maximum_periodic_ghost_error,
                             std::abs(high_ghost_value - high_source_value));
            }
        }
    }
    report.periodic_exchange_correct =
        report.periodic_exchange_correct &&
        report.maximum_periodic_ghost_error == 0.0;

    for (const Side::LoHiSide side : {Side::Lo, Side::Hi})
    {
        for (int ghost = 1; ghost <= m_num_ghosts; ++ghost)
        {
            IntVect point = domain.smallEnd();
            point[BlackStringLive::radial_direction] =
                side == Side::Lo
                    ? domain.smallEnd(BlackStringLive::radial_direction) -
                          ghost
                    : domain.bigEnd(BlackStringLive::radial_direction) +
                          ghost;
            point[BlackStringLive::compact_direction] =
                domain.smallEnd(BlackStringLive::compact_direction);
            const double x =
                BlackStringCoordinates::cell_centered<double>(
                    point[BlackStringLive::radial_direction], m_dx,
                    coordinate_offset[BlackStringLive::radial_direction]);
            const auto oracle =
                BlackStringGPPointwiseInitialData::make_pointwise_state(
                    m_p.r0, x);
            for (int component = 0; component < NUM_VARS; ++component)
            {
                double value = std::numeric_limits<double>::quiet_NaN();
                const bool found =
                    sample_component(m_state_new, point, component, value);
                report.radial_ghosts_correct =
                    report.radial_ghosts_correct && found;
                if (found)
                {
                    report.maximum_radial_ghost_error =
                        std::max(
                            report.maximum_radial_ghost_error,
                            std::abs(value -
                                     oracle[static_cast<std::size_t>(
                                         component)]));
                }
            }
        }
    }
    report.radial_ghosts_correct =
        report.radial_ghosts_correct &&
        report.maximum_radial_ghost_error == 0.0;
    return report;
}

#ifdef CH_USE_HDF5
void BlackStringToyLevel::prePlotLevel()
{
    specificPostTimeStep();
}
#endif
