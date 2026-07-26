#ifndef BLACKSTRINGTOYLEVEL_HPP_
#define BLACKSTRINGTOYLEVEL_HPP_

#include "DefaultLevelFactory.hpp"
#include "GRAMRLevel.hpp"

class BlackStringToyLevel : public GRAMRLevel
{
    friend class DefaultLevelFactory<BlackStringToyLevel>;
    using GRAMRLevel::GRAMRLevel;

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
};

#endif /* BLACKSTRINGTOYLEVEL_HPP_ */
