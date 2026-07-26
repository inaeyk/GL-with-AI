#include "BlackStringToyLevel.hpp"

#include "BlackStringGPInitialData.hpp"
#include "BlackStringLive.hpp"
#include "BoxLoops.hpp"
#include "CH_Timer.H"
#include "Interval.H"
#include "UserVariables.hpp"
#include "parstream.H"

void BlackStringToyLevel::specificAdvance()
{
    // Cleanup is owned by specificUpdateODE, after every normal RK update.
}

void BlackStringToyLevel::initialData()
{
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
    (void)a_rhs;
    (void)a_dt;
    BoxLoops::loop(
        BlackStringLive::CleanupCompute(m_p.min_chi, m_p.min_lapse),
        a_soln, a_soln, EXCLUDE_GHOST_CELLS, disable_simd());
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
    if (!m_p.calculate_constraints)
    {
        return;
    }
    fillAllGhosts();
    BoxLoops::loop(
        BlackStringLive::ConstraintCompute(m_dx, m_p.coordinate_offset()),
        m_state_new, m_state_diagnostics, EXCLUDE_GHOST_CELLS,
        disable_simd());
}

#ifdef CH_USE_HDF5
void BlackStringToyLevel::prePlotLevel()
{
    specificPostTimeStep();
}
#endif
