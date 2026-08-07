#include "BlackStringToyLevel.hpp"

#include "BlackStringAlgebraicReconstruction.hpp"
#include "BlackStringConstraintCorrectedSeed.hpp"
#include "BlackStringGammaZHelper.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"
#include "BlackStringLive.hpp"
#include "DefaultLevelFactory.hpp"
#include "GRAMR.hpp"
#include "GRParmParse.hpp"
#include "SetupFunctions.hpp"
#include "SimulationParameters.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/resource.h>
#include <vector>

// Compile the accepted production level into this focused evidence runner.
// No measurement or fitting code is linked into the production application.
#include "../../BlackStringToyLevel.cpp"

namespace Production = BlackStringProductionVariables;
namespace Seed = BlackStringConstraintCorrectedSeed;
namespace TargetOps = BlackStringTargetCleanupConstraintsSource;

namespace
{
constexpr double pi = 3.141592653589793238462643383279502884;
constexpr int metric_outputs = 9;
constexpr int physical_fields = 5;
constexpr int constraint_fields = 5;
constexpr int retained_harmonics = 4;

const std::array<const char *, NUM_VARS> field_names = {
    "chi",    "hxx",    "hxz",    "hzz",    "hww", "K",
    "Axx",    "Axz",    "Azz",    "Aww",    "Theta",
    "GammaX", "GammaZ", "lapse",  "shiftX", "shiftZ",
    "Bx",     "Bz"};
const std::array<const char *, physical_fields> physical_names = {
    "q", "hww_over_chi", "K", "Aww", "GammaX"};
const std::array<const char *, constraint_fields> constraint_names = {
    "H", "Mx", "Mz", "Zx", "Zz"};

std::string output_directory;
std::string launch_identifier;
int requested_mode = 1;

struct ExpectedCase
{
    int nx;
    int nz;
    int mode;
    int cadence;
    int steps;
    double outer_face;
    double epsilon;
    bool seeded;
};

ExpectedCase expected_case(const std::string &identifier)
{
    if (identifier == "medium-near-gp")
        return {24, 64, 1, 4, 1280, 3.5, 0.0, false};
    if (identifier == "medium-near-low-plus")
        return {24, 64, 1, 4, 1280, 3.5, 1.0e-8, true};
    if (identifier == "medium-near-low-minus")
        return {24, 64, 1, 4, 1280, 3.5, -1.0e-8, true};
    if (identifier == "medium-near-high-plus")
        return {24, 64, 2, 4, 1280, 3.5, 1.0e-8, true};
    if (identifier == "medium-near-high-minus")
        return {24, 64, 2, 4, 1280, 3.5, -1.0e-8, true};
    if (identifier == "fine-near-gp")
        return {36, 96, 1, 6, 1920, 3.5, 0.0, false};
    if (identifier == "fine-near-low-plus")
        return {36, 96, 1, 6, 1920, 3.5, 1.0e-8, true};
    if (identifier == "fine-near-high-plus")
        return {36, 96, 2, 6, 1920, 3.5, 1.0e-8, true};
    if (identifier == "medium-far-gp")
        return {32, 64, 1, 4, 1280, 4.5, 0.0, false};
    if (identifier == "medium-far-low-plus")
        return {32, 64, 1, 4, 1280, 4.5, 1.0e-8, true};
    if (identifier == "medium-far-high-plus")
        return {32, 64, 2, 4, 1280, 4.5, 1.0e-8, true};
    if (identifier == "medium-near-low-half")
        return {24, 64, 1, 4, 1280, 3.5, 5.0e-9, true};
    throw std::runtime_error("unknown frozen launch identifier: " + identifier);
}

bool close(const double left, const double right)
{
    return std::abs(left - right) <=
           128.0 * std::numeric_limits<double>::epsilon() *
               std::max({1.0, std::abs(left), std::abs(right)});
}

void validate_parameters(const std::string &identifier,
                         const SimulationParameters &parameters)
{
    const ExpectedCase expected = expected_case(identifier);
    const int nx = parameters.ivN[0] + 1;
    const int nz = parameters.ivN[1] + 1;
    const double outer = parameters.coordinate_minimum[0] +
                         static_cast<double>(nx) * parameters.coarsest_dx;
    const double compact_length =
        static_cast<double>(nz) * parameters.coarsest_dx;
    if (nx != expected.nx || nz != expected.nz ||
        parameters.fourier_seed_mode_number != expected.mode ||
        parameters.constraint_diagnostic_cadence != expected.cadence ||
        parameters.max_steps != expected.steps ||
        parameters.constraint_corrected_fourier_seed != expected.seeded ||
        !close(parameters.fourier_seed_amplitude, expected.epsilon) ||
        !close(parameters.r0, 1.0) ||
        !close(parameters.coordinate_minimum[0], 0.5) ||
        !close(parameters.coordinate_minimum[1], 0.0) ||
        !close(outer, expected.outer_face) || !close(compact_length, 8.0) ||
        !close(parameters.dt_multiplier, 0.05) ||
        !close(parameters.stop_time, 8.0) || !close(parameters.ko_sigma, 0.3) ||
        !parameters.physical_radial_boundaries ||
        parameters.background_preserving_gp_radial_ghosts ||
        !parameters.fixed_lapse_source || parameters.max_level != 0 ||
        parameters.num_ghosts != 3)
    {
        throw std::runtime_error("frozen M2-C parameter mismatch for " +
                                 identifier);
    }
    const auto far_condition =
        BlackStringReducedCharacteristics::normalized_rank_condition(
            std::sqrt(parameters.r0 / outer), parameters.r0);
    if (far_condition.rank != 23 || far_condition.condition > 1.0e3)
    {
        throw std::runtime_error("frozen outer-face transform gate failed");
    }
    requested_mode = expected.mode;
}

struct MetricCompute
{
    double dx;
    std::array<double, CH_SPACEDIM> coordinate_offset;

    void compute(const Cell<double> cell) const
    {
        const auto input =
            BlackStringLive::make_pointwise_input(cell, dx, coordinate_offset);
        const auto constraints =
            TargetOps::evaluate_constraints(input).target_total;
        const auto &hxx = input.derivatives[Production::c_hxx];
        const auto &hxz = input.derivatives[Production::c_hxz];
        const auto &hzz = input.derivatives[Production::c_hzz];
        const BlackStringGammaZHelper::IndependentMetricJet gamma_input{
            input.vars.physical.h.xx, input.vars.physical.h.xz,
            input.vars.physical.h.zz, hxx.dx, hxz.dx, hzz.dx, hxx.dz,
            hxz.dz, hzz.dz, input.vars.physical.Gamma.x,
            input.vars.physical.Gamma.z};
        const auto gamma =
            BlackStringGammaZHelper::evaluate(input.x, 1, gamma_input);
        const double trace = input.vars.physical.h.xx +
                             input.vars.physical.h.zz;
        const double discriminant =
            std::hypot(input.vars.physical.h.xx - input.vars.physical.h.zz,
                       2.0 * input.vars.physical.h.xz);
        const double eig_lo = 0.5 * (trace - discriminant);
        const double eig_hi = 0.5 * (trace + discriminant);
        const double metric_min =
            std::min(eig_lo, input.vars.physical.h.ww);
        const double metric_max =
            std::max(eig_hi, input.vars.physical.h.ww);
        const std::array<double, metric_outputs> values = {
            constraints.hamiltonian,
            constraints.momentum[0],
            constraints.momentum[1],
            gamma.ZX,
            gamma.ZZ,
            BlackStringAlgebraicReconstruction::determinant_residual(
                input.vars),
            BlackStringAlgebraicReconstruction::weighted_trace_residual(
                input.vars),
            metric_min,
            metric_max / metric_min};
        cell.store_vars<metric_outputs>(values);
    }
};

double state_scale(const int component, const double r0)
{
    switch (component)
    {
    case Production::c_K:
    case Production::c_Axx:
    case Production::c_Axz:
    case Production::c_Azz:
    case Production::c_Aww:
    case Production::c_Theta:
    case Production::c_GammaX:
    case Production::c_GammaZ:
    case Production::c_Bx:
    case Production::c_Bz:
        return r0;
    default:
        return 1.0;
    }
}

void reduce_sum(std::vector<double> &values)
{
#ifdef CH_MPI
    std::vector<double> global(values.size());
    MPI_Allreduce(values.data(), global.data(), static_cast<int>(values.size()),
                  MPI_DOUBLE, MPI_SUM, Chombo_MPI::comm);
    values.swap(global);
#endif
}

void reduce_max(std::vector<double> &values)
{
#ifdef CH_MPI
    std::vector<double> global(values.size());
    MPI_Allreduce(values.data(), global.data(), static_cast<int>(values.size()),
                  MPI_DOUBLE, MPI_MAX, Chombo_MPI::comm);
    values.swap(global);
#endif
}

void reduce_min(std::vector<double> &values)
{
#ifdef CH_MPI
    std::vector<double> global(values.size());
    MPI_Allreduce(values.data(), global.data(), static_cast<int>(values.size()),
                  MPI_DOUBLE, MPI_MIN, Chombo_MPI::comm);
    values.swap(global);
#endif
}

class AcceptanceLevel : public BlackStringToyLevel
{
    friend class DefaultLevelFactory<AcceptanceLevel>;
    using BlackStringToyLevel::BlackStringToyLevel;

  public:
    void record_initial_sample() { record_sample(); }

    void record_final_sample()
    {
        if (last_sample_time < m_time - 1.0e-12)
        {
            record_sample();
        }
    }

  private:
    void specificPostTimeStep() override
    {
        const std::size_t step = static_cast<std::size_t>(
            std::llround(m_time / (m_dx * m_p.dt_multiplier)));
        if (m_p.constraint_diagnostic_due(step))
        {
            record_sample();
        }
    }

    void open_outputs()
    {
        if (procID() != 0)
        {
            return;
        }
        if (measurements.is_open())
        {
            return;
        }
        measurements.open(output_directory + "/measurements.tsv");
        profiles.open(output_directory + "/profiles.tsv");
        status.open(output_directory + "/status.tsv");
        boxes.open(output_directory + "/boxes.tsv");
        if (!measurements || !profiles || !status || !boxes)
        {
            throw std::runtime_error("cannot open retained M2-C evidence");
        }
        measurements
            << "time\tstep\tkind\tname\tminimum\tmaximum\tlinf\tweighted_l2"
               "\tcosine\tsine\tamplitude\tnormalized_ratio"
               "\tboundary_fraction\tnyquist_fraction\tnyquist_amplitude"
               "\tparity_leakage\tharmonic_leakage\n";
        profiles << "time\tstep\tradial_index\tx\tkind\tname\tcosine\tsine"
                    "\tamplitude\n";
        status << "time\tstep\tfinite\tmetric_admissible\tminimum_metric_eigenvalue"
                  "\tmaximum_metric_condition\tdeterminant_max\tweighted_trace_max"
                  "\tvalid_cells\townership_pass\tphysical_norm\tmaximum_state_drift"
                  "\tmaximum_boundary_fraction\tmaximum_nyquist_fraction"
                  "\tmaximum_parity_leakage\tmaximum_harmonic_leakage"
                  "\tlow_radial_fills\thigh_radial_fills"
                  "\touter_rhs_calls\trhs_calls\n";
        boxes << "box\towner\tlo_x\thi_x\tlo_z\thi_z\tvalid_cells"
                 "\ttouches_low_face\ttouches_high_face\n";
        emit_ownership();
    }

    void emit_ownership()
    {
        const Box domain = m_problem_domain.domainBox();
        std::uint64_t total = 0;
        bool pass = true;
        std::vector<Box> all_boxes;
        std::vector<int> all_owners;
        std::vector<int> boxes_per_rank(static_cast<std::size_t>(numProc()));
        int low_face_boxes = 0;
        int high_face_boxes = 0;
        for (LayoutIterator lit = m_grids.layoutIterator(); lit.ok(); ++lit)
        {
            const Box valid = m_grids[lit()] & domain;
            const int owner = m_grids.procID(lit());
            all_boxes.push_back(valid);
            all_owners.push_back(owner);
            total += static_cast<std::uint64_t>(valid.numPts());
            pass = pass && owner >= 0 && owner < numProc();
            if (owner >= 0 && owner < numProc())
                ++boxes_per_rank[static_cast<std::size_t>(owner)];
            const bool touches_low =
                valid.smallEnd(0) == domain.smallEnd(0);
            const bool touches_high =
                valid.bigEnd(0) == domain.bigEnd(0);
            low_face_boxes += static_cast<int>(touches_low);
            high_face_boxes += static_cast<int>(touches_high);
            boxes << all_boxes.size() - 1 << '\t' << owner
                  << '\t' << valid.smallEnd(0) << '\t' << valid.bigEnd(0)
                  << '\t' << valid.smallEnd(1) << '\t' << valid.bigEnd(1)
                  << '\t' << valid.numPts() << '\t'
                  << static_cast<int>(touches_low) << '\t'
                  << static_cast<int>(touches_high) << '\n';
        }
        int internal_mpi_seams = 0;
        for (std::size_t left = 0; left < all_boxes.size(); ++left)
        {
            for (std::size_t right = left + 1; right < all_boxes.size();
                 ++right)
            {
                Box overlap = all_boxes[left] & all_boxes[right];
                pass = pass && overlap.isEmpty();
                const bool compact_neighbors =
                    all_boxes[left].bigEnd(1) + 1 ==
                        all_boxes[right].smallEnd(1) ||
                    all_boxes[right].bigEnd(1) + 1 ==
                        all_boxes[left].smallEnd(1);
                const bool radial_overlap =
                    std::max(all_boxes[left].smallEnd(0),
                             all_boxes[right].smallEnd(0)) <=
                    std::min(all_boxes[left].bigEnd(0),
                             all_boxes[right].bigEnd(0));
                if (compact_neighbors && radial_overlap &&
                    all_owners[left] != all_owners[right])
                {
                    ++internal_mpi_seams;
                }
            }
        }
        pass = pass && numProc() == 2 &&
               std::all_of(boxes_per_rank.begin(), boxes_per_rank.end(),
                           [](const int count) { return count > 0; }) &&
               low_face_boxes > 0 && high_face_boxes > 0 &&
               internal_mpi_seams > 0 &&
               total == static_cast<std::uint64_t>(domain.numPts());
        ownership_pass = pass;
        std::cout << "M2C_OWNERSHIP launch=" << launch_identifier
                  << " pass=" << static_cast<int>(pass)
                  << " valid_cells=" << total << " ranks=" << numProc()
                  << " boxes=" << all_boxes.size()
                  << " rank0_boxes=" << boxes_per_rank[0]
                  << " rank1_boxes=" << boxes_per_rank[1]
                  << " low_face_boxes=" << low_face_boxes
                  << " high_face_boxes=" << high_face_boxes
                  << " internal_mpi_seams=" << internal_mpi_seams
                  << " seam_policy=exchange_only\n";
    }

    void record_sample()
    {
        fillAllGhosts();
        GRLevelData metrics;
        metrics.define(m_grids, metric_outputs, IntVect::Zero);
        BoxLoops::loop(MetricCompute{m_dx, m_p.coordinate_offset()},
                       m_state_new, metrics, EXCLUDE_GHOST_CELLS,
                       disable_simd());
        open_outputs();

        const Box domain = m_problem_domain.domainBox();
        const int nx = domain.size(0);
        const int nz = domain.size(1);
        const std::size_t step = static_cast<std::size_t>(
            std::llround(m_time / (m_dx * m_p.dt_multiplier)));
        const auto offset = m_p.coordinate_offset();
        std::vector<double> field_min(NUM_VARS,
                                      std::numeric_limits<double>::infinity());
        std::vector<double> field_max(NUM_VARS,
                                      -std::numeric_limits<double>::infinity());
        std::vector<double> drift_max(NUM_VARS);
        std::vector<double> drift_sum(NUM_VARS);
        std::vector<double> field_cos(static_cast<std::size_t>(nx) * NUM_VARS);
        std::vector<double> field_sin(static_cast<std::size_t>(nx) * NUM_VARS);
        std::vector<double> physical_cos(static_cast<std::size_t>(nx) *
                                         physical_fields);
        std::vector<double> physical_sin(static_cast<std::size_t>(nx) *
                                         physical_fields);
        std::vector<double> constraint_cos(static_cast<std::size_t>(nx) *
                                           constraint_fields);
        std::vector<double> constraint_sin(static_cast<std::size_t>(nx) *
                                           constraint_fields);
        std::vector<double> harmonic_cos(
            static_cast<std::size_t>(retained_harmonics) * nx *
            physical_fields);
        std::vector<double> harmonic_sin(harmonic_cos.size());
        std::vector<double> constraint_max(constraint_fields + 2);
        std::vector<double> constraint_sum(constraint_fields + 2);
        std::vector<double> radial_counts(nx);
        double local_weight = 0.0;
        double local_min_metric = std::numeric_limits<double>::infinity();
        double local_max_condition = 0.0;
        int local_finite = 1;
        std::uint64_t local_cells = 0;

        for (DataIterator dit = m_state_new.dataIterator(); dit.ok(); ++dit)
        {
            const DataIndex data_index = dit();
            const FArrayBox &state_fab = m_state_new[data_index];
            const FArrayBox &metric_fab = metrics[data_index];
            const Box valid = m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const int radial = point[0] - domain.smallEnd(0);
                const int compact = point[1] - domain.smallEnd(1);
                const double x = BlackStringCoordinates::cell_centered<double>(
                    point[0], m_dx, offset[0]);
                const double weight = x * x;
                const double angle = 2.0 * pi * requested_mode *
                                     (static_cast<double>(compact) + 0.5) /
                                     static_cast<double>(nz);
                const double cosine = std::cos(angle);
                const double sine = std::sin(angle);
                const auto gp =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        m_p.r0, x);
                std::array<double, NUM_VARS> deltas{};
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    const double value = state_fab(point, component);
                    const double delta = value - gp[static_cast<std::size_t>(component)];
                    deltas[static_cast<std::size_t>(component)] = delta;
                    local_finite = local_finite && std::isfinite(value);
                    field_min[static_cast<std::size_t>(component)] =
                        std::min(field_min[static_cast<std::size_t>(component)],
                                 value);
                    field_max[static_cast<std::size_t>(component)] =
                        std::max(field_max[static_cast<std::size_t>(component)],
                                 value);
                    drift_max[static_cast<std::size_t>(component)] =
                        std::max(drift_max[static_cast<std::size_t>(component)],
                                 std::abs(delta));
                    drift_sum[static_cast<std::size_t>(component)] +=
                        weight * delta * delta;
                    const std::size_t index =
                        static_cast<std::size_t>(radial) * NUM_VARS + component;
                    field_cos[index] += delta * cosine;
                    field_sin[index] += delta * sine;
                }
                const std::array<double, physical_fields> physical = {
                    0.5 * std::log(state_fab(point, Production::c_hww) /
                                   state_fab(point, Production::c_chi)),
                    state_fab(point, Production::c_hww) /
                            state_fab(point, Production::c_chi) -
                        1.0,
                    deltas[Production::c_K] * m_p.r0,
                    deltas[Production::c_Aww] * m_p.r0,
                    deltas[Production::c_GammaX] * m_p.r0};
                for (int field = 0; field < physical_fields; ++field)
                {
                    const std::size_t index =
                        static_cast<std::size_t>(radial) * physical_fields +
                        field;
                    physical_cos[index] += physical[static_cast<std::size_t>(field)] * cosine;
                    physical_sin[index] += physical[static_cast<std::size_t>(field)] * sine;
                    for (int harmonic = 1; harmonic <= retained_harmonics;
                         ++harmonic)
                    {
                        const double harmonic_angle =
                            2.0 * pi * harmonic *
                            (static_cast<double>(compact) + 0.5) /
                            static_cast<double>(nz);
                        const std::size_t harmonic_index =
                            (static_cast<std::size_t>(harmonic - 1) * nx +
                             radial) *
                                physical_fields +
                            field;
                        harmonic_cos[harmonic_index] +=
                            physical[static_cast<std::size_t>(field)] *
                            std::cos(harmonic_angle);
                        harmonic_sin[harmonic_index] +=
                            physical[static_cast<std::size_t>(field)] *
                            std::sin(harmonic_angle);
                    }
                }
                const std::array<double, constraint_fields + 2> constraints = {
                    metric_fab(point, 0), metric_fab(point, 1),
                    metric_fab(point, 2), metric_fab(point, 3),
                    metric_fab(point, 4), metric_fab(point, 5),
                    metric_fab(point, 6)};
                for (int field = 0; field < constraint_fields + 2; ++field)
                {
                    const double value = constraints[static_cast<std::size_t>(field)];
                    local_finite = local_finite && std::isfinite(value);
                    constraint_max[static_cast<std::size_t>(field)] =
                        std::max(constraint_max[static_cast<std::size_t>(field)],
                                 std::abs(value));
                    constraint_sum[static_cast<std::size_t>(field)] +=
                        weight * value * value;
                    if (field < constraint_fields)
                    {
                        const std::size_t index =
                            static_cast<std::size_t>(radial) * constraint_fields + field;
                        constraint_cos[index] += value * cosine;
                        constraint_sin[index] += value * sine;
                    }
                }
                local_min_metric = std::min(local_min_metric, metric_fab(point, 7));
                local_max_condition = std::max(local_max_condition, metric_fab(point, 8));
                local_finite = local_finite && std::isfinite(metric_fab(point, 7)) &&
                               std::isfinite(metric_fab(point, 8)) &&
                               state_fab(point, Production::c_chi) > 0.0 &&
                               state_fab(point, Production::c_lapse) > 0.0;
                radial_counts[static_cast<std::size_t>(radial)] += 1.0;
                local_weight += weight;
                ++local_cells;
            }
        }

        reduce_min(field_min);
        reduce_max(field_max);
        reduce_max(drift_max);
        reduce_sum(drift_sum);
        reduce_sum(field_cos);
        reduce_sum(field_sin);
        reduce_sum(physical_cos);
        reduce_sum(physical_sin);
        reduce_sum(constraint_cos);
        reduce_sum(constraint_sin);
        reduce_sum(harmonic_cos);
        reduce_sum(harmonic_sin);
        reduce_max(constraint_max);
        reduce_sum(constraint_sum);
        reduce_sum(radial_counts);
        double total_weight = local_weight;
        double minimum_metric = local_min_metric;
        double maximum_condition = local_max_condition;
        int finite = local_finite;
        std::uint64_t valid_cells = local_cells;
#ifdef CH_MPI
        MPI_Allreduce(&local_weight, &total_weight, 1, MPI_DOUBLE, MPI_SUM,
                      Chombo_MPI::comm);
        MPI_Allreduce(&local_min_metric, &minimum_metric, 1, MPI_DOUBLE,
                      MPI_MIN, Chombo_MPI::comm);
        MPI_Allreduce(&local_max_condition, &maximum_condition, 1, MPI_DOUBLE,
                      MPI_MAX, Chombo_MPI::comm);
        MPI_Allreduce(&local_finite, &finite, 1, MPI_INT, MPI_MIN,
                      Chombo_MPI::comm);
        MPI_Allreduce(&local_cells, &valid_cells, 1, MPI_UINT64_T, MPI_SUM,
                      Chombo_MPI::comm);
#endif
        if (procID() != 0)
        {
            last_sample_time = m_time;
            return;
        }

        for (int radial = 0; radial < nx; ++radial)
        {
            if (radial_counts[static_cast<std::size_t>(radial)] != nz)
            {
                throw std::runtime_error("Fourier sampler ownership mismatch");
            }
        }
        const auto profile_metrics = [&](const std::vector<double> &cosines,
                                         const std::vector<double> &sines,
                                         const int stride, const int field,
                                         const bool odd) {
            std::array<double, 8> result{};
            double total = 0.0;
            double boundary = 0.0;
            double unwanted = 0.0;
            double signed_cosine = 0.0;
            double signed_sine = 0.0;
            double nyquist_cosine = 0.0;
            double nyquist_sine = 0.0;
            double weight_sum = 0.0;
            for (int radial = 0; radial < nx; ++radial)
            {
                const double x = m_p.coordinate_minimum[0] +
                                 (static_cast<double>(radial) + 0.5) * m_dx;
                const double weight = x * x;
                const std::size_t index =
                    static_cast<std::size_t>(radial) * stride + field;
                const double normalization =
                    2.0 / radial_counts[static_cast<std::size_t>(radial)];
                const double c = normalization * cosines[index];
                const double s = normalization * sines[index];
                const double energy = c * c + s * s;
                total += weight * energy;
                if (radial < 5 || radial >= nx - 5)
                    boundary += weight * energy;
                unwanted += weight * (odd ? c * c : s * s);
                signed_cosine += weight * c;
                signed_sine += weight * s;
                nyquist_cosine += (radial % 2 == 0 ? 1.0 : -1.0) * c;
                nyquist_sine += (radial % 2 == 0 ? 1.0 : -1.0) * s;
                weight_sum += weight;
            }
            result[0] = signed_cosine / weight_sum;
            result[1] = signed_sine / weight_sum;
            result[2] = std::sqrt(total / weight_sum);
            result[3] = total > 0.0 ? boundary / total : 0.0;
            result[4] = total > 0.0
                            ? std::hypot(nyquist_cosine, nyquist_sine) /
                                  std::sqrt(static_cast<double>(nx) * total /
                                            (weight_sum /
                                             static_cast<double>(nx)))
                            : 0.0;
            result[5] = std::hypot(nyquist_cosine, nyquist_sine) /
                        static_cast<double>(nx);
            result[6] = total > 0.0 ? std::sqrt(unwanted / total) : 0.0;
            result[7] = total;
            return result;
        };

        std::array<std::array<double, 8>, physical_fields> physical_metric{};
        double physical_energy = 0.0;
        double maximum_boundary = 0.0;
        double maximum_nyquist = 0.0;
        double maximum_parity = 0.0;
        double maximum_harmonic = 0.0;
        for (int field = 0; field < physical_fields; ++field)
        {
            physical_metric[static_cast<std::size_t>(field)] =
                profile_metrics(physical_cos, physical_sin, physical_fields,
                                field, false);
            physical_energy += physical_metric[static_cast<std::size_t>(field)][7];
        }
        const double physical_norm =
            std::sqrt(physical_energy /
                      (static_cast<double>(physical_fields) *
                       total_weight / static_cast<double>(nz)));

        measurements << std::scientific << std::setprecision(17);
        profiles << std::scientific << std::setprecision(17);
        status << std::scientific << std::setprecision(17);
        for (int component = 0; component < NUM_VARS; ++component)
        {
            const auto metric = profile_metrics(
                field_cos, field_sin, NUM_VARS, component,
                Production::metadata[static_cast<std::size_t>(component)]
                        .parity == Production::ParityClass::one_z_opposite);
            const double scale = state_scale(component, m_p.r0);
            measurements << m_time << '\t' << step << "\tfield\t"
                         << field_names[static_cast<std::size_t>(component)]
                         << '\t' << field_min[static_cast<std::size_t>(component)]
                         << '\t' << field_max[static_cast<std::size_t>(component)]
                         << '\t' << drift_max[static_cast<std::size_t>(component)]
                         << '\t'
                         << std::sqrt(drift_sum[static_cast<std::size_t>(component)] /
                                      total_weight)
                         << '\t' << scale * metric[0] << '\t' << scale * metric[1]
                         << '\t' << scale * metric[2] << "\t0\t" << metric[3]
                         << '\t' << metric[4] << '\t' << scale * metric[5]
                         << '\t' << metric[6] << "\t0\n";
        }

        for (int field = 0; field < physical_fields; ++field)
        {
            auto &metric = physical_metric[static_cast<std::size_t>(field)];
            double harmonic_leakage = 0.0;
            for (int harmonic = 1; harmonic <= retained_harmonics; ++harmonic)
            {
                if (harmonic == requested_mode)
                    continue;
                std::vector<double> one_cos(static_cast<std::size_t>(nx) *
                                            physical_fields);
                std::vector<double> one_sin(one_cos.size());
                for (int radial = 0; radial < nx; ++radial)
                {
                    const std::size_t source =
                        (static_cast<std::size_t>(harmonic - 1) * nx + radial) *
                            physical_fields +
                        field;
                    const std::size_t destination =
                        static_cast<std::size_t>(radial) * physical_fields +
                        field;
                    one_cos[destination] = harmonic_cos[source];
                    one_sin[destination] = harmonic_sin[source];
                }
                const auto unwanted = profile_metrics(
                    one_cos, one_sin, physical_fields, field, false);
                if (metric[2] > 0.0)
                    harmonic_leakage =
                        std::max(harmonic_leakage, unwanted[2] / metric[2]);
            }
            maximum_boundary = std::max(maximum_boundary, metric[3]);
            maximum_nyquist = std::max(maximum_nyquist, metric[4]);
            maximum_parity = std::max(maximum_parity, metric[6]);
            maximum_harmonic =
                std::max(maximum_harmonic, harmonic_leakage);
            measurements << m_time << '\t' << step << "\tphysical\t"
                         << physical_names[static_cast<std::size_t>(field)]
                         << "\t0\t0\t0\t0\t" << metric[0] << '\t' << metric[1]
                         << '\t' << metric[2] << "\t0\t" << metric[3] << '\t'
                         << metric[4] << '\t' << metric[5] << '\t' << metric[6]
                         << '\t' << harmonic_leakage << '\n';
            for (int radial = 0; radial < nx; ++radial)
            {
                const double normalization =
                    2.0 / radial_counts[static_cast<std::size_t>(radial)];
                const std::size_t index =
                    static_cast<std::size_t>(radial) * physical_fields + field;
                const double c = normalization * physical_cos[index];
                const double s = normalization * physical_sin[index];
                const double x = m_p.coordinate_minimum[0] +
                                 (static_cast<double>(radial) + 0.5) * m_dx;
                profiles << m_time << '\t' << step << '\t' << radial << '\t'
                         << x << "\tphysical\t"
                         << physical_names[static_cast<std::size_t>(field)]
                         << '\t' << c << '\t' << s << '\t' << std::hypot(c, s)
                         << '\n';
            }
        }

        for (int field = 0; field < constraint_fields; ++field)
        {
            const bool odd = field == 2 || field == 4;
            const auto metric = profile_metrics(
                constraint_cos, constraint_sin, constraint_fields, field, odd);
            const double constraint_scale = field == 0 ? m_p.r0 * m_p.r0
                                                       : m_p.r0;
            const double ratio = physical_norm > 0.0
                                     ? constraint_scale * metric[2] /
                                           physical_norm
                                     : 0.0;
            measurements << m_time << '\t' << step << "\tconstraint\t"
                         << constraint_names[static_cast<std::size_t>(field)]
                         << "\t0\t0\t"
                         << constraint_max[static_cast<std::size_t>(field)]
                         << '\t'
                         << std::sqrt(constraint_sum[static_cast<std::size_t>(field)] /
                                      total_weight)
                         << '\t' << constraint_scale * metric[0] << '\t'
                         << constraint_scale * metric[1] << '\t'
                         << constraint_scale * metric[2] << '\t' << ratio
                         << '\t' << metric[3] << '\t' << metric[4] << '\t'
                         << constraint_scale * metric[5] << '\t' << metric[6]
                         << "\t0\n";
            for (int radial = 0; radial < nx; ++radial)
            {
                const double normalization =
                    2.0 / radial_counts[static_cast<std::size_t>(radial)];
                const std::size_t index =
                    static_cast<std::size_t>(radial) * constraint_fields + field;
                const double c = constraint_scale * normalization * constraint_cos[index];
                const double s = constraint_scale * normalization * constraint_sin[index];
                const double x = m_p.coordinate_minimum[0] +
                                 (static_cast<double>(radial) + 0.5) * m_dx;
                profiles << m_time << '\t' << step << '\t' << radial << '\t'
                         << x << "\tconstraint\t"
                         << constraint_names[static_cast<std::size_t>(field)]
                         << '\t' << c << '\t' << s << '\t' << std::hypot(c, s)
                         << '\n';
            }
        }

        for (int field = 0; field < 2; ++field)
        {
            const int source = constraint_fields + field;
            measurements << m_time << '\t' << step << "\talgebraic\t"
                         << (field == 0 ? "determinant" : "weighted_trace")
                         << "\t0\t0\t"
                         << constraint_max[static_cast<std::size_t>(source)]
                         << '\t'
                         << std::sqrt(constraint_sum[static_cast<std::size_t>(source)] /
                                      total_weight)
                         << "\t0\t0\t0\t0\t0\t0\t0\t0\t0\n";
        }
        const double maximum_drift =
            *std::max_element(drift_max.begin(), drift_max.end());
        const auto &counts = instrumentation_report();
        const bool lifecycle_pass =
            counts.low_radial_fills == counts.ghost_fill_calls &&
            counts.high_radial_fills == counts.ghost_fill_calls &&
            counts.outer_radiative_rhs_calls == counts.rhs_calls &&
            counts.rhs_periodic_exchanges == counts.rhs_calls &&
            counts.periodic_exchanges ==
                counts.fillall_periodic_exchanges +
                    counts.rhs_periodic_exchanges;
        status << m_time << '\t' << step << '\t' << finite << '\t'
               << static_cast<int>(finite && minimum_metric > 0.0 &&
                                   std::isfinite(maximum_condition))
               << '\t' << minimum_metric << '\t' << maximum_condition << '\t'
               << constraint_max[5] << '\t' << constraint_max[6] << '\t'
               << valid_cells << '\t'
               << static_cast<int>(ownership_pass && lifecycle_pass)
               << '\t' << physical_norm << '\t' << maximum_drift << '\t'
               << maximum_boundary << '\t' << maximum_nyquist << '\t'
               << maximum_parity << '\t' << maximum_harmonic << '\t'
               << counts.low_radial_fills << '\t'
               << counts.high_radial_fills << '\t'
               << counts.outer_radiative_rhs_calls << '\t'
               << counts.rhs_calls << '\n';
        measurements.flush();
        profiles.flush();
        status.flush();
        std::cout << std::scientific << std::setprecision(17)
                  << "M2C_SAMPLE launch=" << launch_identifier
                  << " time=" << m_time << " step=" << step
                  << " finite=" << finite
                  << " metric_admissible=" << static_cast<int>(minimum_metric > 0.0)
                  << " physical_norm=" << physical_norm
                  << " max_drift=" << maximum_drift
                  << " determinant_max=" << constraint_max[5]
                  << " weighted_trace_max=" << constraint_max[6] << '\n';
        last_sample_time = m_time;
    }

    bool ownership_pass = false;
    double last_sample_time = -1.0;
    std::ofstream measurements;
    std::ofstream profiles;
    std::ofstream status;
    std::ofstream boxes;
};

void emit_seed_report(const SimulationParameters &parameters)
{
    if (procID() != 0 || !parameters.constraint_corrected_fourier_seed)
        return;
    const Seed::Configuration configuration{
        parameters.ivN[0] + 1,
        parameters.ivN[1] + 1,
        parameters.fourier_seed_mode_number,
        parameters.r0,
        parameters.coarsest_dx,
        parameters.coordinate_minimum[0],
        parameters.coordinate_minimum[1],
        parameters.fourier_seed_amplitude};
    const auto solution = Seed::solve(configuration);
    std::ofstream output(output_directory + "/seed_report.tsv");
    if (!output)
        throw std::runtime_error("cannot open seed report");
    output << "rank\tretained_condition\tscaled_rms\tmax_hm\tmax_gamma_z"
              "\tdeterminant\tweighted_trace\tamplitude_error"
              "\tretained_physical_fraction\tlinear_solve_residual"
              "\tincoming_boundary\tantisymmetry\n"
           << std::scientific << std::setprecision(17) << solution.report.rank
           << '\t' << solution.report.retained_condition << '\t'
           << solution.report.scaled_rms << '\t' << solution.report.max_hm
           << '\t' << solution.report.max_gamma_z << '\t'
           << solution.report.determinant << '\t'
           << solution.report.weighted_trace << '\t'
           << solution.report.amplitude_error << '\t'
           << solution.report.retained_physical_fraction << '\t'
           << solution.report.linear_solve_residual << '\t'
           << solution.report.incoming_boundary << '\t'
           << solution.report.antisymmetry << '\n';
    std::cout << std::scientific << std::setprecision(17)
              << "M2C_SEED_REPORT launch=" << launch_identifier
              << " rank=" << solution.report.rank
              << " condition=" << solution.report.retained_condition
              << " scaled_rms=" << solution.report.scaled_rms
              << " max_hm=" << solution.report.max_hm
              << " max_gamma_z=" << solution.report.max_gamma_z
              << " incoming=" << solution.report.incoming_boundary << '\n';
}

int run_case(const std::string &identifier, const std::string &parameter_file,
             const std::string &evidence_directory)
{
    launch_identifier = identifier;
    output_directory = evidence_directory;
    GRParmParse pp(0, nullptr, nullptr, parameter_file.c_str());
    SimulationParameters parameters(pp);
    validate_parameters(identifier, parameters);
    emit_seed_report(parameters);
#ifdef CH_MPI
    MPI_Barrier(Chombo_MPI::comm);
    const double start = MPI_Wtime();
#else
    const double start = 0.0;
#endif
    GRAMR amr;
    DefaultLevelFactory<AcceptanceLevel> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    if (levels.size() != 1)
        throw std::runtime_error("M2-C requires one AMR level");
    auto *level = dynamic_cast<AcceptanceLevel *>(levels[0]);
    if (level == nullptr)
        throw std::runtime_error("M2-C factory returned wrong level type");
    level->record_initial_sample();
    amr.run(parameters.stop_time, parameters.max_steps);
    level->time(amr.getCurrentTime());
    level->record_final_sample();
    amr.conclude();
#ifdef CH_MPI
    MPI_Barrier(Chombo_MPI::comm);
    const double elapsed = MPI_Wtime() - start;
#else
    const double elapsed = 0.0;
#endif
    struct rusage usage
    {
    };
    getrusage(RUSAGE_SELF, &usage);
    long local_rss = usage.ru_maxrss;
    long maximum_rss = local_rss;
    long total_rss = local_rss;
#ifdef CH_MPI
    MPI_Allreduce(&local_rss, &maximum_rss, 1, MPI_LONG, MPI_MAX,
                  Chombo_MPI::comm);
    MPI_Allreduce(&local_rss, &total_rss, 1, MPI_LONG, MPI_SUM,
                  Chombo_MPI::comm);
#endif
    if (procID() == 0)
    {
        std::cout << std::scientific << std::setprecision(17)
                  << "M2C_RUN_END launch=" << identifier << " exit_status=0"
                  << " final_time=" << amr.getCurrentTime()
                  << " final_step=" << parameters.max_steps
                  << " wall_seconds=" << elapsed
                  << " peak_rss_kib_max=" << maximum_rss
                  << " peak_rss_kib_sum=" << total_rss << '\n'
                  << "M2C_HOT_PATH target_d4_rhs_per_cell_stage=1"
                  << " fused_ko_additions=1 second_full_volume_rhs_pass=0"
                  << " boundary_work=surface_only runtime_eigensystem=0"
                  << " production_spectral_fitting=0"
                  << " per_cell_allocation_logging_counters_mutexes_virtual=0\n";
    }
    return 0;
}
} // namespace

int main(int argc, char **argv)
{
    mainSetup(argc, argv);
    int status_code = 0;
    try
    {
        if (argc == 4 && std::string(argv[1]) == "preflight")
        {
            launch_identifier = argv[2];
            GRParmParse pp(0, nullptr, nullptr, argv[3]);
            SimulationParameters parameters(pp);
            validate_parameters(launch_identifier, parameters);
            if (procID() == 0)
            {
                const ExpectedCase expected = expected_case(launch_identifier);
                const auto condition =
                    BlackStringReducedCharacteristics::normalized_rank_condition(
                        std::sqrt(1.0 / expected.outer_face), 1.0);
                std::cout << "M2C_PARAMETER_PREFLIGHT_PASS launch="
                          << launch_identifier << " transform_rank="
                          << condition.rank << " transform_condition="
                          << std::setprecision(17) << condition.condition
                          << " numerical_launch=0\n";
            }
        }
        else if (argc == 5 && std::string(argv[1]) == "run")
        {
            status_code = run_case(argv[2], argv[3], argv[4]);
        }
        else
        {
            throw std::runtime_error(
                "usage: M2CAcceptanceRunner preflight ID PARAM | "
                "M2CAcceptanceRunner run ID PARAM EVIDENCE_DIR");
        }
    }
    catch (const std::exception &error)
    {
        std::cerr << "M2C_RUNNER_FAILURE " << error.what() << '\n';
        status_code = 1;
    }
    mainFinalize();
    return status_code;
}
