#ifndef BLACKSTRINGTOYLEVEL_HPP_
#define BLACKSTRINGTOYLEVEL_HPP_

#include "DefaultLevelFactory.hpp"
#include "GRAMRLevel.hpp"
#include "BlackStringRadialGPGhost.hpp"
#include "BlackStringPerturbativeRadialBoundary.hpp"

#include <array>

#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
#include "BlackStringLevelDiagnosticInstrumentation.hpp"
#endif

class BlackStringToyLevel : public GRAMRLevel
{
    friend class DefaultLevelFactory<BlackStringToyLevel>;

  public:
    struct EvolutionDiagnostic
    {
        std::array<double, NUM_VARS> maximum_change{};
        std::array<double, NUM_DIAGNOSTIC_VARS> constraint_maximum{};
        double maximum_state_change = 0.0;
        double maximum_lapse_drift = 0.0;
        double maximum_radial_ghost_error = 0.0;
        double maximum_periodic_ghost_error = 0.0;
        double final_time = 0.0;
        int radial_cells = 0;
        int compact_cells = 0;
        bool finite_state = true;
        bool finite_constraints = true;
        bool periodic_exchange_correct = true;
        bool radial_ghosts_correct = true;
    };

    EvolutionDiagnostic collect_evolution_diagnostic();

#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    const BlackStringLevelDiagnosticInstrumentation::Report &
    instrumentation_report() const
    {
        return m_instrumentation.report();
    }
#endif

  protected:
    using GRAMRLevel::GRAMRLevel;

    void fillBdyGhosts(
        GRLevelData &state,
        const Interval &components = Interval(0, NUM_VARS - 1)) override;
    void copyBdyGhosts(const GRLevelData &source,
                       GRLevelData &destination) override;
    void fill_background_radial_ghosts(
        GRLevelData &state,
        const std::array<double, CH_SPACEDIM> &coordinate_offset);
    void fill_perturbative_radial_ghosts(GRLevelData &state,
                                         const Interval &components);

#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    BlackStringLevelDiagnosticInstrumentation &instrumentation()
    {
        return m_instrumentation;
    }
#endif

  private:
    void specificAdvance() override;
    void initialData() override;
    void specificEvalRHS(GRLevelData &a_soln, GRLevelData &a_rhs,
                         double a_time) override;
    void specificUpdateODE(GRLevelData &a_soln, const GRLevelData &a_rhs,
                           Real a_dt) override;
    void preTagCells() override;
    void computeTaggingCriterion(
        FArrayBox &tagging_criterion, const FArrayBox &current_state,
        const FArrayBox &current_state_diagnostics) override;
    void specificPostTimeStep() override;

#ifdef CH_USE_HDF5
    void prePlotLevel() override;
#endif

#ifdef BLACKSTRING_E2_LEVEL_DIAGNOSTICS
    BlackStringLevelDiagnosticInstrumentation m_instrumentation;
#endif
};

#endif /* BLACKSTRINGTOYLEVEL_HPP_ */
