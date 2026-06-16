/* GRChombo
 * Copyright 2012 The GRChombo collaboration.
 * Please refer to LICENSE in GRChombo's root directory.
 */

#include "BlackStringToyLevel.hpp"
#include "AMRReductions.hpp"
#include "BinaryBH.hpp"
#include "BoxLoops.hpp"
#include "CCZ4RHS.hpp"
#include "ChiExtractionTaggingCriterion.hpp"
#include "ChiPunctureExtractionTaggingCriterion.hpp"
#include "ComputePack.hpp"
#include "ConformalCartoonAlgebra.hpp"
#include "MayDay.H"
#include "NanCheck.hpp"
#include "NewConstraints.hpp"
#include "PositiveChiAndAlpha.hpp"
#include "PunctureTracker.hpp"
#include "SetValue.hpp"
#include "SixthOrderDerivatives.hpp"
#include "SmallDataIO.hpp"
#include "TraceARemoval.hpp"
#include "TwoPuncturesInitialData.hpp"
#include "UserVariables.hpp"
#include "Weyl4.hpp"
#include "WeylExtraction.hpp"

#include <cmath>
#include <exception>
#include <string>

// Stage 2A scaffold note: this class intentionally still uses inherited
// BinaryBH initial data and standard 3+1D CCZ4 behavior. It is not physical
// black-string evolution.

namespace
{
void check_finite_stage4e(const double value, const char *name)
{
    if (!std::isfinite(value))
    {
        const std::string message =
            std::string("Stage 4E grid-to-helper handoff produced nonfinite ") +
            name;
        MayDay::Error(message.c_str());
    }
}

class SetHiddenScaffoldVariables
{
    double m_hww;
    double m_Aww;

  public:
    SetHiddenScaffoldVariables(double a_hww, double a_Aww)
        : m_hww(a_hww), m_Aww(a_Aww)
    {
    }

    template <class data_t> void compute(Cell<data_t> current_cell) const
    {
        // Stage 4D smoke-only scaffold fill. These values keep the new hidden
        // variables finite in the inherited cheap smoke run; they are not
        // physical black-string initial data or cartoon evolution equations.
        // Future real hidden-sector RHS support must disable or replace this
        // freeze, with a loud guard against using both paths together.
        current_cell.store_vars(m_hww, c_hww);
        current_cell.store_vars(m_Aww, c_Aww);
    }
};

class CheckHiddenHandoffToAlgebra
{
  public:
    template <class data_t> void compute(Cell<data_t> current_cell) const
    {
        using namespace BlackStringToy::ConformalCartoonAlgebra;

        // Stage 4E diagnostic only: read real grid slots into the Stage 4A
        // local algebra helper API. The computed values are checked for
        // finiteness and are not written back or used for evolution.
        const double chi = current_cell.load_vars(c_chi);
        const ConformalMetric h{current_cell.load_vars(c_h11),
                                current_cell.load_vars(c_h12),
                                current_cell.load_vars(c_h22),
                                current_cell.load_vars(c_hww)};
        const ConformalExtrinsicCurvature A{current_cell.load_vars(c_A11),
                                            current_cell.load_vars(c_A12),
                                            current_cell.load_vars(c_A22),
                                            current_cell.load_vars(c_Aww)};
        const double K = current_cell.load_vars(c_K);

        check_finite_stage4e(chi, "chi input");
        check_finite_stage4e(h.xx, "hxx input");
        check_finite_stage4e(h.xz, "hxz input");
        check_finite_stage4e(h.zz, "hzz input");
        check_finite_stage4e(h.ww, "hww input");
        check_finite_stage4e(A.xx, "Axx input");
        check_finite_stage4e(A.xz, "Axz input");
        check_finite_stage4e(A.zz, "Azz input");
        check_finite_stage4e(A.ww, "Aww input");
        check_finite_stage4e(K, "K input");

        if (chi <= algebra_zero_tolerance)
        {
            MayDay::Error(
                "Stage 4E grid-to-helper handoff requires positive chi");
        }

        const double reduced_det = reduced_determinant(h);
        if (reduced_det <= algebra_zero_tolerance)
        {
            MayDay::Error("Stage 4E grid-to-helper handoff found invalid "
                          "reduced conformal determinant");
        }
        if (h.ww <= algebra_zero_tolerance)
        {
            MayDay::Error(
                "Stage 4E grid-to-helper handoff requires positive hww");
        }

        try
        {
            const double det_4d = determinant_4d(h);
            const auto h_UU = inverse(h);
            const double tr_A = trace(A, h_UU);
            const auto K_ij = reconstruct_extrinsic_curvature(A, h, chi, K);

            check_finite_stage4e(reduced_det, "reduced determinant output");
            check_finite_stage4e(det_4d, "4D determinant output");
            check_finite_stage4e(h_UU.xx, "inverse hxx output");
            check_finite_stage4e(h_UU.xz, "inverse hxz output");
            check_finite_stage4e(h_UU.zz, "inverse hzz output");
            check_finite_stage4e(h_UU.ww, "inverse hww output");
            check_finite_stage4e(tr_A, "full 4D trace output");
            check_finite_stage4e(K_ij.xx, "K_xx reconstruction output");
            check_finite_stage4e(K_ij.xz, "K_xz reconstruction output");
            check_finite_stage4e(K_ij.zz, "K_zz reconstruction output");
            check_finite_stage4e(K_ij.ww, "K_ww reconstruction output");
        }
        catch (const std::exception &error)
        {
            const std::string message =
                std::string("Stage 4E grid-to-helper handoff failed: ") +
                error.what();
            MayDay::Error(message.c_str());
        }
    }
};
} // namespace

// Things to do during the advance step after RK4 steps
void BlackStringToyLevel::specificAdvance()
{
    // Enforce the trace free A_ij condition and positive chi and alpha
    if (m_p.scaffold_freeze_hidden)
    {
        BoxLoops::loop(make_compute_pack(TraceARemoval(), PositiveChiAndAlpha(),
                                         SetHiddenScaffoldVariables(1.0, 0.0)),
                       m_state_new, m_state_new, INCLUDE_GHOST_CELLS);
    }
    else
    {
        BoxLoops::loop(make_compute_pack(TraceARemoval(), PositiveChiAndAlpha()),
                       m_state_new, m_state_new, INCLUDE_GHOST_CELLS);
    }

    if (m_p.scaffold_check_hidden_handoff)
    {
        BoxLoops::loop(CheckHiddenHandoffToAlgebra(), m_state_new,
                       m_state_new, EXCLUDE_GHOST_CELLS, disable_simd());
    }

    // Check for nan's
    if (m_p.nan_check)
        BoxLoops::loop(
            NanCheck(m_dx, m_p.center, "NaNCheck in specific Advance"),
            m_state_new, m_state_new, EXCLUDE_GHOST_CELLS, disable_simd());
}

// This initial data uses an approximation for the metric which
// is valid for small boosts
void BlackStringToyLevel::initialData()
{
    CH_TIME("BlackStringToyLevel::initialData");
    if (m_verbosity)
        pout() << "BlackStringToyLevel::initialData " << m_level << endl;
#ifdef USE_TWOPUNCTURES
    TwoPuncturesInitialData two_punctures_initial_data(
        m_dx, m_p.center, m_tp_amr.m_two_punctures);
    // Can't use simd with this initial data
    BoxLoops::loop(two_punctures_initial_data, m_state_new, m_state_new,
                   INCLUDE_GHOST_CELLS, disable_simd());
#else
    // Set up the inherited public BinaryBH initial data for scaffold testing.
    BinaryBH binary(m_p.bh1_params, m_p.bh2_params, m_dx);

    // First set everything to zero (to avoid undefinded values in constraints)
    // then calculate initial data
    BoxLoops::loop(make_compute_pack(SetValue(0.), binary), m_state_new,
                   m_state_new, INCLUDE_GHOST_CELLS);
    if (m_p.scaffold_freeze_hidden)
    {
        BoxLoops::loop(SetHiddenScaffoldVariables(1.0, 0.0), m_state_new,
                       m_state_new, INCLUDE_GHOST_CELLS);
    }
#endif
}

// Calculate RHS during RK4 substeps
void BlackStringToyLevel::specificEvalRHS(GRLevelData &a_soln, GRLevelData &a_rhs,
                                    const double a_time)
{
    // Enforce positive chi and alpha and trace free A
    BoxLoops::loop(make_compute_pack(TraceARemoval(), PositiveChiAndAlpha()),
                   a_soln, a_soln, INCLUDE_GHOST_CELLS);

    // Calculate CCZ4 right hand side
    if (m_p.max_spatial_derivative_order == 4)
    {
        BoxLoops::loop(CCZ4RHS<MovingPunctureGauge, FourthOrderDerivatives>(
                           m_p.ccz4_params, m_dx, m_p.sigma, m_p.formulation),
                       a_soln, a_rhs, EXCLUDE_GHOST_CELLS);
    }
    else if (m_p.max_spatial_derivative_order == 6)
    {
        BoxLoops::loop(CCZ4RHS<MovingPunctureGauge, SixthOrderDerivatives>(
                           m_p.ccz4_params, m_dx, m_p.sigma, m_p.formulation),
                       a_soln, a_rhs, EXCLUDE_GHOST_CELLS);
    }

    if (m_p.scaffold_freeze_hidden)
    {
        // Stage 4D smoke-only scaffold behavior: the inherited public CCZ4 RHS
        // does not evolve repo-owned hidden variables. Keep their RHS finite
        // and zero only for the cheap smoke run. This must not be used with a
        // future real hidden-sector RHS.
        BoxLoops::loop(SetHiddenScaffoldVariables(0.0, 0.0), a_rhs, a_rhs,
                       EXCLUDE_GHOST_CELLS);
    }
}

// enforce trace removal during RK4 substeps
void BlackStringToyLevel::specificUpdateODE(GRLevelData &a_soln,
                                      const GRLevelData &a_rhs, Real a_dt)
{
    // Enforce the trace free A_ij condition
    if (m_p.scaffold_freeze_hidden)
    {
        BoxLoops::loop(make_compute_pack(TraceARemoval(),
                                         SetHiddenScaffoldVariables(1.0, 0.0)),
                       a_soln, a_soln, INCLUDE_GHOST_CELLS);
    }
    else
    {
        BoxLoops::loop(TraceARemoval(), a_soln, a_soln, INCLUDE_GHOST_CELLS);
    }
}

void BlackStringToyLevel::preTagCells()
{
    // We only use chi in the tagging criterion so only fill the ghosts for chi
    fillAllGhosts(VariableType::evolution, Interval(c_chi, c_chi));
}

// specify the cells to tag
void BlackStringToyLevel::computeTaggingCriterion(
    FArrayBox &tagging_criterion, const FArrayBox &current_state,
    const FArrayBox &current_state_diagnostics)
{
    if (m_p.track_punctures)
    {
        std::vector<double> puncture_masses;
#ifdef USE_TWOPUNCTURES
        // use calculated bare masses from TwoPunctures
        puncture_masses = {m_tp_amr.m_two_punctures.mm,
                           m_tp_amr.m_two_punctures.mp};
#else
        puncture_masses = {m_p.bh1_params.mass, m_p.bh2_params.mass};
#endif /* USE_TWOPUNCTURES */
        auto puncture_coords =
            m_bh_amr.m_puncture_tracker.get_puncture_coords();
        BoxLoops::loop(ChiPunctureExtractionTaggingCriterion(
                           m_dx, m_level, m_p.max_level, m_p.extraction_params,
                           puncture_coords, m_p.activate_extraction,
                           m_p.track_punctures, puncture_masses),
                       current_state, tagging_criterion);
    }
    else
    {
        BoxLoops::loop(ChiExtractionTaggingCriterion(m_dx, m_level,
                                                     m_p.extraction_params,
                                                     m_p.activate_extraction),
                       current_state, tagging_criterion);
    }
}

void BlackStringToyLevel::specificPostTimeStep()
{
    CH_TIME("BlackStringToyLevel::specificPostTimeStep");

    bool first_step =
        (m_time == 0.); // this form is used when 'specificPostTimeStep' was
                        // called during setup at t=0 from Main
    // bool first_step = (m_time == m_dt); // if not called in Main

    if (m_p.activate_extraction == 1)
    {
        int min_level = m_p.extraction_params.min_extraction_level();
        bool calculate_weyl = at_level_timestep_multiple(min_level);
        if (calculate_weyl)
        {
            // Populate the Weyl Scalar values on the grid
            fillAllGhosts();
            BoxLoops::loop(
                Weyl4(m_p.extraction_params.center, m_dx, m_p.formulation),
                m_state_new, m_state_diagnostics, EXCLUDE_GHOST_CELLS);

            // Do the extraction on the min extraction level
            if (m_level == min_level)
            {
                CH_TIME("WeylExtraction");
                // Now refresh the interpolator and do the interpolation
                // fill ghosts manually to minimise communication
                bool fill_ghosts = false;
                m_gr_amr.m_interpolator->refresh(fill_ghosts);
                m_gr_amr.fill_multilevel_ghosts(
                    VariableType::diagnostic, Interval(c_Weyl4_Re, c_Weyl4_Im),
                    min_level);
                WeylExtraction my_extraction(m_p.extraction_params, m_dt,
                                             m_time, first_step,
                                             m_restart_time);
                my_extraction.execute_query(m_gr_amr.m_interpolator);
            }
        }
    }

    if (m_p.calculate_constraint_norms)
    {
        fillAllGhosts();
        BoxLoops::loop(Constraints(m_dx, c_Ham, Interval(c_Mom1, c_Mom3)),
                       m_state_new, m_state_diagnostics, EXCLUDE_GHOST_CELLS);
        if (m_level == 0)
        {
            AMRReductions<VariableType::diagnostic> amr_reductions(m_gr_amr);
            double L2_Ham = amr_reductions.norm(c_Ham);
            double L2_Mom = amr_reductions.norm(Interval(c_Mom1, c_Mom3));
            SmallDataIO constraints_file(m_p.data_path + "constraint_norms",
                                         m_dt, m_time, m_restart_time,
                                         SmallDataIO::APPEND, first_step);
            constraints_file.remove_duplicate_time_data();
            if (first_step)
            {
                constraints_file.write_header_line({"L^2_Ham", "L^2_Mom"});
            }
            constraints_file.write_time_data_line({L2_Ham, L2_Mom});
        }
    }

    // do puncture tracking on requested level
    if (m_p.track_punctures && m_level == m_p.puncture_tracking_level)
    {
        CH_TIME("PunctureTracking");
        // only do the write out for every coarsest level timestep
        int coarsest_level = 0;
        bool write_punctures = at_level_timestep_multiple(coarsest_level);
        m_bh_amr.m_puncture_tracker.execute_tracking(m_time, m_restart_time,
                                                     m_dt, write_punctures);
    }

#ifdef USE_AHFINDER
    if (m_p.AH_activate && m_level == m_p.AH_params.level_to_run)
    {
        if (m_p.AH_set_origins_to_punctures && m_p.track_punctures)
        {
            m_bh_amr.m_ah_finder.set_origins(
                m_bh_amr.m_puncture_tracker.get_puncture_coords());
        }
        m_bh_amr.m_ah_finder.solve(m_dt, m_time, m_restart_time);
    }
#endif
}

#ifdef CH_USE_HDF5
// Things to do before a plot level - need to calculate the Weyl scalars
void BlackStringToyLevel::prePlotLevel()
{
    fillAllGhosts();
    if (m_p.activate_extraction == 1)
    {
        BoxLoops::loop(
            make_compute_pack(
                Weyl4(m_p.extraction_params.center, m_dx, m_p.formulation),
                Constraints(m_dx, c_Ham, Interval(c_Mom1, c_Mom3))),
            m_state_new, m_state_diagnostics, EXCLUDE_GHOST_CELLS);
    }
}
#endif /* CH_USE_HDF5 */
