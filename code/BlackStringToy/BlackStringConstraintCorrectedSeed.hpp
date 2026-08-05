#ifndef BLACKSTRINGCONSTRAINTCORRECTEDSEED_HPP
#define BLACKSTRINGCONSTRAINTCORRECTEDSEED_HPP

#include "BlackStringTargetCleanupConstraintsSource.hpp"
#include "BlackStringGammaZHelper.hpp"
#include "BlackStringPerturbativeRadialBoundary.hpp"
#include "BoxIterator.H"
#include "GRLevelData.hpp"
#include "ProblemDomain.H"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

extern "C"
{
void dgesvd_(char *, char *, int *, int *, double *, int *, double *, double *,
             int *, double *, int *, double *, int *, int *);
}

namespace BlackStringConstraintCorrectedSeed
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace Target = BlackStringTargetCCZ4Pointwise;
namespace TargetOps = BlackStringTargetCleanupConstraintsSource;

inline constexpr int correction_fields = 10;
inline constexpr int constraint_fields = 5;
inline constexpr int boundary_buffer = 4;
inline constexpr double pi = 3.141592653589793238462643383279502884;
inline constexpr std::array<int, correction_fields> correction_slots = {
    Production::c_chi,    Production::c_hxx,    Production::c_hxz,
    Production::c_hzz,    Production::c_K,      Production::c_Axx,
    Production::c_Axz,    Production::c_Azz,    Production::c_GammaX,
    Production::c_GammaZ};

struct Configuration
{
    int radial_cells = 0;
    int compact_cells = 0;
    int mode_number = 0;
    double r0 = 0.0;
    double dx = 0.0;
    double inner_face = 0.0;
    double compact_minimum = 0.0;
    double epsilon = 0.0;
};

struct Report
{
    int rank = 0;
    double retained_condition = 0.0;
    double scaled_rms = 0.0;
    double max_hm = 0.0;
    double max_gamma_z = 0.0;
    double determinant = 0.0;
    double weighted_trace = 0.0;
    double amplitude_error = 0.0;
    double retained_physical_fraction = 0.0;
    double linear_solve_residual = 0.0;
    double incoming_boundary = 0.0;
    double antisymmetry = 0.0;
};

struct Solution
{
    Configuration configuration{};
    std::vector<double> amplitudes;
    Report report{};

    double amplitude(const int radial, const int field) const
    {
        return amplitudes[static_cast<std::size_t>(radial *
                                                    correction_fields +
                                                    field)];
    }
};

inline bool odd_slot(const int slot)
{
    return Production::metadata[static_cast<std::size_t>(slot)].parity ==
           Production::ParityClass::one_z_opposite;
}

inline double field_scale(const int field, const double r0)
{
    return field <= 3 ? 1.0 : 1.0 / r0;
}

inline double bump(const int radial, const Configuration &configuration)
{
    if (radial < boundary_buffer ||
        radial >= configuration.radial_cells - boundary_buffer)
    {
        return 0.0;
    }
    const double first = static_cast<double>(boundary_buffer);
    const double last =
        static_cast<double>(configuration.radial_cells - boundary_buffer - 1);
    const double center = 0.5 * (first + last);
    const double half_width = 0.5 * (last - first) + 0.5;
    const double s = (static_cast<double>(radial) - center) / half_width;
    if (std::abs(s) >= 1.0)
    {
        return 0.0;
    }
    return std::exp(1.0 - 1.0 / (1.0 - s * s));
}

inline std::vector<double> raw_seed(const Configuration &configuration)
{
    std::vector<double> result(
        static_cast<std::size_t>(configuration.radial_cells) *
        correction_fields);
    constexpr std::array<double, correction_fields> coefficients = {
        0.0, 1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    for (int radial = 0; radial < configuration.radial_cells; ++radial)
    {
        const double profile = bump(radial, configuration);
        for (int field = 0; field < correction_fields; ++field)
        {
            result[static_cast<std::size_t>(radial * correction_fields +
                                            field)] =
                coefficients[static_cast<std::size_t>(field)] * profile *
                field_scale(field, configuration.r0);
        }
    }
    return result;
}

using ExtendedLine = std::vector<Reduced::Storage<double>>;

inline int extended_index(const int radial)
{
    return radial + BlackStringPerturbativeRadialBoundary::supported_ghosts;
}

inline std::array<ExtendedLine, 5> build_phase_lines(
    const Configuration &configuration, const std::vector<double> &amplitudes,
    const double center_phase)
{
    std::array<ExtendedLine, 5> lines;
    const double angle = 2.0 * pi *
                         static_cast<double>(configuration.mode_number) /
                         static_cast<double>(configuration.compact_cells);
    for (int phase_index = -2; phase_index <= 2; ++phase_index)
    {
        auto &line = lines[static_cast<std::size_t>(phase_index + 2)];
        line.resize(static_cast<std::size_t>(configuration.radial_cells + 6));
        const double phase = center_phase + angle * phase_index;
        for (int radial = 0; radial < configuration.radial_cells; ++radial)
        {
            const double x = configuration.inner_face +
                             (static_cast<double>(radial) + 0.5) *
                                 configuration.dx;
            auto vars =
                BlackStringGPPointwiseInitialData::make_pointwise_vars(
                    configuration.r0, x);
            for (int field = 0; field < correction_fields; ++field)
            {
                const int slot =
                    correction_slots[static_cast<std::size_t>(field)];
                const double harmonic = odd_slot(slot) ? std::sin(phase)
                                                       : std::cos(phase);
                Reduced::component(vars,
                                   static_cast<Production::VariableSlot>(slot)) +=
                    amplitudes[static_cast<std::size_t>(
                        radial * correction_fields + field)] *
                    harmonic;
            }
            BlackStringAlgebraicReconstruction::reconstruct(vars);
            line[static_cast<std::size_t>(extended_index(radial))] =
                Reduced::store(vars);
        }

        for (const int normal : {-1, 1})
        {
            BlackStringPerturbativeRadialBoundary::SourceStates sources{};
            const int boundary = normal < 0 ? 0
                                            : configuration.radial_cells - 1;
            for (int source = 0; source < 5; ++source)
            {
                const int radial = boundary - normal * source;
                sources[static_cast<std::size_t>(source)] =
                    line[static_cast<std::size_t>(extended_index(radial))];
            }
            const double face = normal < 0
                                    ? configuration.inner_face
                                    : configuration.inner_face +
                                          configuration.radial_cells *
                                              configuration.dx;
            const auto ghosts =
                BlackStringPerturbativeRadialBoundary::fill_characteristic_line(
                    sources, face, normal, configuration.r0,
                    configuration.dx);
            for (int layer = 0; layer < 3; ++layer)
            {
                const int radial = boundary + normal * (layer + 1);
                line[static_cast<std::size_t>(extended_index(radial))] =
                    ghosts[static_cast<std::size_t>(layer)];
            }
        }
    }
    return lines;
}

inline std::array<double, constraint_fields> constraint_at(
    const Configuration &configuration,
    const std::array<ExtendedLine, 5> &lines, const int radial)
{
    constexpr std::array<double, 5> d1 = {1.0, -8.0, 0.0, 8.0, -1.0};
    constexpr std::array<double, 5> d2 = {-1.0, 16.0, -30.0, 16.0, -1.0};
    Target::Input input;
    input.x = configuration.inner_face +
              (static_cast<double>(radial) + 0.5) * configuration.dx;
    input.vars = Reduced::load(
        lines[2][static_cast<std::size_t>(extended_index(radial))]);
    const double inverse_d1 = 1.0 / (12.0 * configuration.dx);
    const double inverse_d2 =
        1.0 / (12.0 * configuration.dx * configuration.dx);
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        Target::Jet &jet = input.derivatives[static_cast<std::size_t>(slot)];
        for (int offset = -2; offset <= 2; ++offset)
        {
            const double radial_value =
                lines[2][static_cast<std::size_t>(
                             extended_index(radial + offset))]
                        [static_cast<std::size_t>(slot)];
            const double z_value =
                lines[static_cast<std::size_t>(offset + 2)]
                     [static_cast<std::size_t>(extended_index(radial))]
                     [static_cast<std::size_t>(slot)];
            jet.dx += d1[static_cast<std::size_t>(offset + 2)] *
                      radial_value * inverse_d1;
            jet.dxx += d2[static_cast<std::size_t>(offset + 2)] *
                       radial_value * inverse_d2;
            jet.dz += d1[static_cast<std::size_t>(offset + 2)] * z_value *
                      inverse_d1;
            jet.dzz += d2[static_cast<std::size_t>(offset + 2)] * z_value *
                       inverse_d2;
            for (int z_offset = -2; z_offset <= 2; ++z_offset)
            {
                const double value =
                    lines[static_cast<std::size_t>(z_offset + 2)]
                         [static_cast<std::size_t>(
                             extended_index(radial + offset))]
                         [static_cast<std::size_t>(slot)];
                jet.dxz +=
                    d1[static_cast<std::size_t>(offset + 2)] *
                    d1[static_cast<std::size_t>(z_offset + 2)] * value *
                    inverse_d1 * inverse_d1;
            }
        }
    }
    const auto constraints =
        TargetOps::evaluate_constraints(input).target_total;
    const auto &hxx = input.derivatives[Production::c_hxx];
    const auto &hxz = input.derivatives[Production::c_hxz];
    const auto &hzz = input.derivatives[Production::c_hzz];
    const BlackStringGammaZHelper::IndependentMetricJet gamma_input{
        input.vars.physical.h.xx,
        input.vars.physical.h.xz,
        input.vars.physical.h.zz,
        hxx.dx,
        hxz.dx,
        hzz.dx,
        hxx.dz,
        hxz.dz,
        hzz.dz,
        input.vars.physical.Gamma.x,
        input.vars.physical.Gamma.z};
    const auto gamma =
        BlackStringGammaZHelper::evaluate(input.x, 1, gamma_input);
    return {configuration.r0 * configuration.r0 * constraints.hamiltonian,
            configuration.r0 * constraints.momentum[0],
            configuration.r0 * constraints.momentum[1],
            configuration.r0 * gamma.ZX,
            configuration.r0 * gamma.ZZ};
}

inline std::vector<double> constraint_vector(
    const Configuration &configuration,
    const std::vector<double> &amplitudes)
{
    const auto even_lines = build_phase_lines(configuration, amplitudes, 0.0);
    const auto odd_lines = build_phase_lines(configuration, amplitudes,
                                             0.5 * pi);
    std::vector<double> result(
        static_cast<std::size_t>(configuration.radial_cells) *
        constraint_fields);
    for (int radial = 0; radial < configuration.radial_cells; ++radial)
    {
        const auto even = constraint_at(configuration, even_lines, radial);
        const auto odd = constraint_at(configuration, odd_lines, radial);
        const std::array<double, constraint_fields> values = {
            even[0], even[1], odd[2], even[3], odd[4]};
        for (int constraint = 0; constraint < constraint_fields; ++constraint)
        {
            result[static_cast<std::size_t>(radial * constraint_fields +
                                            constraint)] =
                values[static_cast<std::size_t>(constraint)];
        }
    }
    return result;
}

inline std::vector<double> centered_constraint_direction(
    const Configuration &configuration, const std::vector<double> &direction,
    const double step)
{
    std::vector<double> plus(direction.size());
    std::vector<double> minus(direction.size());
    for (std::size_t index = 0; index < direction.size(); ++index)
    {
        plus[index] = step * direction[index];
        minus[index] = -step * direction[index];
    }
    const std::vector<double> plus_constraints =
        constraint_vector(configuration, plus);
    const std::vector<double> minus_constraints =
        constraint_vector(configuration, minus);
    std::vector<double> result(plus_constraints.size());
    for (std::size_t index = 0; index < result.size(); ++index)
    {
        result[index] =
            (plus_constraints[index] - minus_constraints[index]) /
            (2.0 * step);
    }
    return result;
}

inline double physical_norm(const std::vector<double> &values,
                            const Configuration &configuration)
{
    double sum = 0.0;
    for (int radial = 0; radial < configuration.radial_cells; ++radial)
    {
        for (int field = 0; field < 8; ++field)
        {
            const double scaled =
                values[static_cast<std::size_t>(radial * correction_fields +
                                                field)] /
                field_scale(field, configuration.r0);
            sum += scaled * scaled;
        }
    }
    return std::sqrt(sum /
                     static_cast<double>(configuration.radial_cells));
}

inline void validate_configuration(const Configuration &configuration)
{
    if (configuration.radial_cells <= 2 * boundary_buffer + 4 ||
        configuration.compact_cells < 8 || configuration.mode_number <= 0 ||
        2 * configuration.mode_number >= configuration.compact_cells ||
        !std::isfinite(configuration.r0) || !(configuration.r0 > 0.0) ||
        !std::isfinite(configuration.dx) || !(configuration.dx > 0.0) ||
        !std::isfinite(configuration.inner_face) ||
        !(configuration.inner_face > 0.0) ||
        !std::isfinite(configuration.epsilon) ||
        configuration.epsilon == 0.0)
    {
        throw std::domain_error(
            "M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE CONFIGURATION");
    }
}

inline Solution solve(const Configuration &configuration)
{
    validate_configuration(configuration);
    Solution solution;
    solution.configuration = configuration;
    const std::vector<double> raw = raw_seed(configuration);
    const double raw_norm = physical_norm(raw, configuration);
    const int interior_cells =
        configuration.radial_cells - 2 * boundary_buffer;
    const int columns = interior_cells * correction_fields;
    const int constraint_rows =
        configuration.radial_cells * constraint_fields;
    // The last SVD row is the locked scaled-physical-norm normalization row.
    // It fixes the first-order norm of raw+delta; the final homogeneous
    // rescaling then sets the requested signed epsilon exactly once.
    const int normalization_row = constraint_rows;
    const int rows = constraint_rows + 1;
    std::vector<double> matrix(static_cast<std::size_t>(rows) * columns);
    constexpr double finite_difference_step = 1.0 / 1048576.0;
    for (int column = 0; column < columns; ++column)
    {
        const int radial = boundary_buffer + column / correction_fields;
        const int field = column % correction_fields;
        std::vector<double> direction(raw.size());
        direction[static_cast<std::size_t>(radial * correction_fields +
                                           field)] =
            field_scale(field, configuration.r0);
        const std::vector<double> values = centered_constraint_direction(
            configuration, direction, finite_difference_step);
        for (int row = 0; row < constraint_rows; ++row)
        {
            matrix[static_cast<std::size_t>(row) +
                   static_cast<std::size_t>(rows) * column] =
                values[static_cast<std::size_t>(row)];
        }
        if (field < 8)
        {
            const double raw_scaled =
                raw[static_cast<std::size_t>(radial * correction_fields +
                                              field)] /
                field_scale(field, configuration.r0);
            matrix[static_cast<std::size_t>(normalization_row) +
                   static_cast<std::size_t>(rows) * column] =
                raw_scaled /
                (static_cast<double>(configuration.radial_cells) * raw_norm);
        }
    }
    std::vector<double> raw_constraints(
        static_cast<std::size_t>(constraint_rows));
    for (int column = 0; column < columns; ++column)
    {
        const int radial = boundary_buffer + column / correction_fields;
        const int field = column % correction_fields;
        const double raw_scaled =
            raw[static_cast<std::size_t>(radial * correction_fields + field)] /
            field_scale(field, configuration.r0);
        for (int row = 0; row < constraint_rows; ++row)
        {
            raw_constraints[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(rows) * column] *
                raw_scaled;
        }
    }
    std::vector<double> right_hand_side(static_cast<std::size_t>(rows));
    for (int row = 0; row < constraint_rows; ++row)
    {
        right_hand_side[static_cast<std::size_t>(row)] =
            -raw_constraints[static_cast<std::size_t>(row)];
    }
    right_hand_side[static_cast<std::size_t>(normalization_row)] = 0.0;

    const int minimum = std::min(rows, columns);
    std::vector<double> singular_values(static_cast<std::size_t>(minimum));
    std::vector<double> u(static_cast<std::size_t>(rows) * minimum);
    std::vector<double> vt(static_cast<std::size_t>(minimum) * columns);
    std::vector<double> copy = matrix;
    int m = rows;
    int n = columns;
    int lda = rows;
    int ldu = rows;
    int ldvt = minimum;
    int info = 0;
    int lwork = -1;
    double work_query = 0.0;
    char jobu = 'S';
    char jobvt = 'S';
    dgesvd_(&jobu, &jobvt, &m, &n, copy.data(), &lda,
            singular_values.data(), u.data(), &ldu, vt.data(), &ldvt,
            &work_query, &lwork, &info);
    if (info != 0)
    {
        throw std::runtime_error(
            "M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE SVD_QUERY");
    }
    lwork = std::max(5 * std::max(rows, columns),
                     static_cast<int>(std::ceil(work_query)));
    std::vector<double> work(static_cast<std::size_t>(lwork));
    copy = matrix;
    dgesvd_(&jobu, &jobvt, &m, &n, copy.data(), &lda,
            singular_values.data(), u.data(), &ldu, vt.data(), &ldvt,
            work.data(), &lwork, &info);
    if (info != 0 || singular_values.empty() ||
        !(singular_values.front() > 0.0))
    {
        throw std::runtime_error(
            "M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE SVD_SOLVE");
    }
    const double cutoff = 1.0e-12 * singular_values.front();
    solution.report.rank = static_cast<int>(std::count_if(
        singular_values.begin(), singular_values.end(),
        [&](const double value) { return value >= cutoff; }));
    if (solution.report.rank <= 0)
    {
        throw std::runtime_error(
            "M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE RANK");
    }
    solution.report.retained_condition =
        singular_values.front() /
        singular_values[static_cast<std::size_t>(solution.report.rank - 1)];
    if (!std::isfinite(solution.report.retained_condition) ||
        solution.report.retained_condition > 1.0e12)
    {
        throw std::runtime_error(
            "M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE CONDITION");
    }
    std::vector<double> coefficients(static_cast<std::size_t>(minimum));
    for (int singular = 0; singular < solution.report.rank; ++singular)
    {
        for (int row = 0; row < rows; ++row)
        {
            coefficients[static_cast<std::size_t>(singular)] +=
                u[static_cast<std::size_t>(row) +
                  static_cast<std::size_t>(rows) * singular] *
                right_hand_side[static_cast<std::size_t>(row)];
        }
        coefficients[static_cast<std::size_t>(singular)] /=
            singular_values[static_cast<std::size_t>(singular)];
    }
    solution.amplitudes = raw;
    std::vector<double> scaled_correction(static_cast<std::size_t>(columns));
    for (int column = 0; column < columns; ++column)
    {
        double correction = 0.0;
        for (int singular = 0; singular < solution.report.rank; ++singular)
        {
            correction +=
                vt[static_cast<std::size_t>(singular) +
                   static_cast<std::size_t>(minimum) * column] *
                coefficients[static_cast<std::size_t>(singular)];
        }
        const int radial = boundary_buffer + column / correction_fields;
        const int field = column % correction_fields;
        scaled_correction[static_cast<std::size_t>(column)] = correction;
        solution.amplitudes[static_cast<std::size_t>(
            radial * correction_fields + field)] +=
            field_scale(field, configuration.r0) * correction;
    }
    // Refine the retained-subspace solve without changing the cutoff or
    // introducing another factorization. This removes the roundoff plateau
    // from U*Sigma*V^T back-substitution at the locked Gamma/Z tolerance.
    for (int refinement = 0; refinement < 2; ++refinement)
    {
        std::vector<double> residual(static_cast<std::size_t>(rows));
        for (int row = 0; row < constraint_rows; ++row)
        {
            residual[static_cast<std::size_t>(row)] =
                raw_constraints[static_cast<std::size_t>(row)];
        }
        for (int column = 0; column < columns; ++column)
        {
            for (int row = 0; row < rows; ++row)
            {
                residual[static_cast<std::size_t>(row)] +=
                    matrix[static_cast<std::size_t>(row) +
                           static_cast<std::size_t>(rows) * column] *
                    scaled_correction[static_cast<std::size_t>(column)];
            }
        }
        std::vector<double> refinement_coefficients(
            static_cast<std::size_t>(solution.report.rank));
        for (int singular = 0; singular < solution.report.rank; ++singular)
        {
            for (int row = 0; row < rows; ++row)
            {
                refinement_coefficients[static_cast<std::size_t>(singular)] -=
                    u[static_cast<std::size_t>(row) +
                      static_cast<std::size_t>(rows) * singular] *
                    residual[static_cast<std::size_t>(row)];
            }
            refinement_coefficients[static_cast<std::size_t>(singular)] /=
                singular_values[static_cast<std::size_t>(singular)];
        }
        for (int column = 0; column < columns; ++column)
        {
            double improvement = 0.0;
            for (int singular = 0; singular < solution.report.rank; ++singular)
            {
                improvement +=
                    vt[static_cast<std::size_t>(singular) +
                       static_cast<std::size_t>(minimum) * column] *
                    refinement_coefficients[
                        static_cast<std::size_t>(singular)];
            }
            scaled_correction[static_cast<std::size_t>(column)] += improvement;
            const int radial = boundary_buffer + column / correction_fields;
            const int field = column % correction_fields;
            solution.amplitudes[static_cast<std::size_t>(
                radial * correction_fields + field)] +=
                field_scale(field, configuration.r0) * improvement;
        }
    }
    std::vector<double> linear_residual(
        static_cast<std::size_t>(constraint_rows));
    for (int row = 0; row < constraint_rows; ++row)
    {
        double value = raw_constraints[static_cast<std::size_t>(row)];
        for (int column = 0; column < columns; ++column)
        {
            value += matrix[static_cast<std::size_t>(row) +
                            static_cast<std::size_t>(rows) * column] *
                     scaled_correction[static_cast<std::size_t>(column)];
        }
        linear_residual[static_cast<std::size_t>(row)] = value;
        solution.report.linear_solve_residual =
            std::max(solution.report.linear_solve_residual, std::abs(value));
    }
    double normalization_linear_residual = 0.0;
    for (int column = 0; column < columns; ++column)
    {
        normalization_linear_residual +=
            matrix[static_cast<std::size_t>(normalization_row) +
                   static_cast<std::size_t>(rows) * column] *
            scaled_correction[static_cast<std::size_t>(column)];
    }
    solution.report.linear_solve_residual =
        std::max(solution.report.linear_solve_residual,
                 std::abs(normalization_linear_residual));
    const double corrected_norm = physical_norm(solution.amplitudes,
                                                configuration);
    solution.report.retained_physical_fraction = corrected_norm / raw_norm;
    if (!std::isfinite(corrected_norm) || !(corrected_norm > 0.0) ||
        solution.report.retained_physical_fraction < 0.25)
    {
        throw std::runtime_error(
            "M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE PHYSICAL_NORM");
    }
    const double normalization = configuration.epsilon / corrected_norm;
    for (double &value : solution.amplitudes)
    {
        value *= normalization;
    }
    solution.report.amplitude_error =
        std::abs(physical_norm(solution.amplitudes, configuration) -
                 std::abs(configuration.epsilon));

    double sum2 = 0.0;
    for (int radial = 0; radial < configuration.radial_cells; ++radial)
    {
        for (int constraint = 0; constraint < constraint_fields; ++constraint)
        {
            const double value =
                normalization * linear_residual[static_cast<std::size_t>(
                                    radial * constraint_fields + constraint)];
            sum2 += value * value;
            if (constraint < 3)
            {
                solution.report.max_hm =
                    std::max(solution.report.max_hm, std::abs(value));
            }
            else
            {
                solution.report.max_gamma_z =
                    std::max(solution.report.max_gamma_z, std::abs(value));
            }
        }
    }
    solution.report.scaled_rms =
        std::sqrt(sum2 / static_cast<double>(constraint_rows));
    const auto reconstructed =
        build_phase_lines(configuration, solution.amplitudes, 0.0);
    const auto reconstructed_odd =
        build_phase_lines(configuration, solution.amplitudes, 0.5 * pi);
    const auto record_incoming = [&](const std::array<ExtendedLine, 5> &lines) {
        for (int phase = 0; phase < 5; ++phase)
        {
            for (const int normal : {-1, 1})
            {
                BlackStringPerturbativeRadialBoundary::SourceStates sources{};
                const int boundary =
                    normal < 0 ? 0 : configuration.radial_cells - 1;
                for (int source = 0; source < 5; ++source)
                {
                    const int radial = boundary - normal * source;
                    sources[static_cast<std::size_t>(source)] =
                        lines[static_cast<std::size_t>(phase)]
                             [static_cast<std::size_t>(extended_index(radial))];
                }
                const double face =
                    normal < 0
                        ? configuration.inner_face
                        : configuration.inner_face +
                              configuration.radial_cells * configuration.dx;
                solution.report.incoming_boundary = std::max(
                    solution.report.incoming_boundary,
                    BlackStringPerturbativeRadialBoundary::
                        incoming_characteristic_residual(
                            sources, face, normal, configuration.r0,
                            configuration.dx));
            }
        }
    };
    record_incoming(reconstructed);
    record_incoming(reconstructed_odd);
    int nonzero_physical_fields = 0;
    for (int field = 0; field < 8; ++field)
    {
        double maximum = 0.0;
        for (int radial = 0; radial < configuration.radial_cells; ++radial)
        {
            maximum = std::max(
                maximum,
                std::abs(solution.amplitudes[static_cast<std::size_t>(
                    radial * correction_fields + field)]));
        }
        nonzero_physical_fields += maximum > 0.0 ? 1 : 0;
    }
    for (int radial = 0; radial < configuration.radial_cells; ++radial)
    {
        const auto vars = Reduced::load(
            reconstructed[2][static_cast<std::size_t>(
                extended_index(radial))]);
        solution.report.determinant =
            std::max(solution.report.determinant,
                     std::abs(BlackStringAlgebraicReconstruction::
                                  determinant_residual(vars)));
        solution.report.weighted_trace =
            std::max(solution.report.weighted_trace,
                     std::abs(BlackStringAlgebraicReconstruction::
                                  weighted_trace_residual(vars)));
    }
    if (solution.report.scaled_rms >
            1.0e-8 * std::abs(configuration.epsilon) ||
        solution.report.max_hm >
            5.0e-8 * std::abs(configuration.epsilon) ||
        solution.report.max_gamma_z >
            1.0e-10 * std::abs(configuration.epsilon) ||
        solution.report.incoming_boundary >
            1.0e-11 * std::abs(configuration.epsilon) ||
        solution.report.determinant > 5.0e-13 ||
        solution.report.weighted_trace > 5.0e-13 / configuration.r0 ||
        solution.report.amplitude_error > 5.0e-13 ||
        solution.report.antisymmetry > 5.0e-13 ||
        nonzero_physical_fields < 2)
    {
        std::ostringstream message;
        message.precision(17);
        message << "M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE "
                   "RESIDUAL rms="
                << solution.report.scaled_rms
                << " hm=" << solution.report.max_hm
                << " gamma_z=" << solution.report.max_gamma_z
                << " incoming=" << solution.report.incoming_boundary
                << " rank=" << solution.report.rank
                << " condition=" << solution.report.retained_condition
                << " retained="
                << solution.report.retained_physical_fraction
                << " linear=" << solution.report.linear_solve_residual;
        throw std::runtime_error(message.str());
    }
    return solution;
}

inline void apply(const Solution &solution, GRLevelData &state,
                  const ProblemDomain &problem_domain,
                  const std::array<double, CH_SPACEDIM> &coordinate_offset)
{
    const Box domain = problem_domain.domainBox();
    const double compact_length =
        solution.configuration.compact_cells * solution.configuration.dx;
    const double k = 2.0 * pi * solution.configuration.mode_number /
                     compact_length;
    const DataIterator iterator = state.dataIterator();
    for (int ibox = 0; ibox < iterator.size(); ++ibox)
    {
        const DataIndex data_index = iterator[ibox];
        FArrayBox &fab = state[data_index];
        const Box valid = state.disjointBoxLayout()[data_index] & domain;
        for (BoxIterator bit(valid); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            const int radial = point[0] - domain.smallEnd(0);
            const double x = BlackStringCoordinates::cell_centered<double>(
                point[0], solution.configuration.dx, coordinate_offset[0]);
            const double z = BlackStringCoordinates::cell_centered<double>(
                point[1], solution.configuration.dx, coordinate_offset[1]);
            auto vars =
                BlackStringGPPointwiseInitialData::make_pointwise_vars(
                    solution.configuration.r0, x);
            for (int field = 0; field < correction_fields; ++field)
            {
                const int slot =
                    correction_slots[static_cast<std::size_t>(field)];
                const double harmonic =
                    odd_slot(slot) ? std::sin(k * z) : std::cos(k * z);
                Reduced::component(vars,
                                   static_cast<Production::VariableSlot>(slot)) +=
                    solution.amplitude(radial, field) * harmonic;
            }
            BlackStringAlgebraicReconstruction::reconstruct(vars);
            const auto storage = Reduced::store(vars);
            for (int slot = 0; slot < Production::NUM_VARS; ++slot)
            {
                fab(point, slot) = storage[static_cast<std::size_t>(slot)];
            }
        }
    }
}

} // namespace BlackStringConstraintCorrectedSeed

#endif /* BLACKSTRINGCONSTRAINTCORRECTEDSEED_HPP */
