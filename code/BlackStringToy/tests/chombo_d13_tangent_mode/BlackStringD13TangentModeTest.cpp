#include "BlackStringToyLevel.hpp"

#include "BlackStringGPInitialData.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"
#include "BoxLoops.hpp"
#include "DefaultLevelFactory.hpp"
#include "GRAMR.hpp"
#include "GRParmParse.hpp"
#include "SetupFunctions.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <sys/resource.h>
#include <utility>
#include <vector>

// Compile the unchanged production level into this strict fixture.
#include "../../BlackStringToyLevel.cpp"

namespace
{
namespace Production = BlackStringProductionVariables;

constexpr double pi = 3.141592653589793238462643383279502884;
constexpr int radial_cells = 32;
constexpr int compact_cells = 64;
constexpr int component_count = Production::NUM_VARS;
constexpr int constraint_count = 3;
constexpr int primary_iterations = 78;
constexpr int convergence_history = 5;
constexpr double overlap_threshold = 0.999;
constexpr double rate_variation_threshold = 0.02;
constexpr double epsilon_primary = 1.0e-8;
constexpr double epsilon_secondary = 5.0e-9;
constexpr double epsilon_agreement_tolerance = 5.0e-3;
constexpr double profile_center = 2.25;
constexpr double profile_half_width = 0.75;

static_assert(CH_SPACEDIM == 2);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(component_count == 18);

using StateRow = std::array<double, component_count>;
using RadialVector = std::array<StateRow, radial_cells>;
using ConstraintRow = std::array<double, constraint_count>;
using RadialConstraints = std::array<ConstraintRow, radial_cells>;

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_D13_FAIL " << message << '\n';
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

bool one_z_component(const int component)
{
    return component == c_hxz || component == c_Axz ||
           component == c_GammaZ || component == c_shiftZ ||
           component == c_Bz;
}

struct Profile
{
    double value = 0.0;
    double derivative = 0.0;
};

Profile compact_profile(const double x)
{
    const double s = (x - profile_center) / profile_half_width;
    if (std::abs(s) >= 1.0)
    {
        return {};
    }
    const double denominator = 1.0 - s * s;
    const double value = std::exp(1.0 - 1.0 / denominator);
    return {value,
            value * (-2.0 * s) /
                (profile_half_width * denominator * denominator)};
}

double inner_profile(const double x)
{
    constexpr double center = 0.90;
    constexpr double half_width = 0.34;
    const double s = (x - center) / half_width;
    if (std::abs(s) >= 1.0)
    {
        return 0.0;
    }
    return std::exp(1.0 - 1.0 / (1.0 - s * s));
}

double raw_dot(const RadialVector &left, const RadialVector &right,
               const double dx)
{
    double result = 0.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            result += left[static_cast<std::size_t>(radial)]
                           [static_cast<std::size_t>(component)] *
                      right[static_cast<std::size_t>(radial)]
                            [static_cast<std::size_t>(component)];
        }
    }
    return result * dx;
}

double raw_norm(const RadialVector &input, const double dx)
{
    return std::sqrt(std::max(0.0, raw_dot(input, input, dx)));
}

double scaled_norm(const RadialVector &input, const StateRow &scales,
                   const double dx)
{
    double result = 0.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            const double value =
                input[static_cast<std::size_t>(radial)]
                     [static_cast<std::size_t>(component)] /
                scales[static_cast<std::size_t>(component)];
            result += value * value;
        }
    }
    return std::sqrt(std::max(0.0, result * dx));
}

void scale(RadialVector &input, const double factor)
{
    for (auto &row : input)
    {
        for (double &value : row)
        {
            value *= factor;
        }
    }
}

RadialVector difference(RadialVector left, const RadialVector &right,
                        const double factor)
{
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            left[static_cast<std::size_t>(radial)]
                [static_cast<std::size_t>(component)] -=
                factor *
                right[static_cast<std::size_t>(radial)]
                     [static_cast<std::size_t>(component)];
        }
    }
    return left;
}

double normalized_overlap(const RadialVector &left,
                          const RadialVector &right, const double dx)
{
    const double denominator = raw_norm(left, dx) * raw_norm(right, dx);
    require(denominator > 0.0, "overlap requires nonzero vectors");
    return std::abs(raw_dot(left, right, dx)) / denominator;
}

double relative_projective_difference(const RadialVector &reference,
                                      RadialVector candidate,
                                      const double dx)
{
    const double denominator = raw_dot(reference, reference, dx);
    require(denominator > 0.0, "projective comparison has zero reference");
    const double coefficient = raw_dot(reference, candidate, dx) / denominator;
    candidate = difference(candidate, reference, coefficient);
    return raw_norm(candidate, dx) / raw_norm(reference, dx);
}

double vector_nyquist_fraction(const RadialVector &input, const double dx)
{
    double nyquist_energy = 0.0;
    double total_energy = 0.0;
    for (int component = 0; component < component_count; ++component)
    {
        double alternating = 0.0;
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            const double value =
                input[static_cast<std::size_t>(radial)]
                     [static_cast<std::size_t>(component)];
            alternating += (radial % 2 == 0 ? 1.0 : -1.0) * value;
            total_energy += value * value;
        }
        nyquist_energy += alternating * alternating /
                          static_cast<double>(radial_cells);
    }
    (void)dx;
    return total_energy > 0.0 ? nyquist_energy / total_energy : 0.0;
}

double boundary_fraction(const RadialVector &input)
{
    double boundary_energy = 0.0;
    double total_energy = 0.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            const double value =
                input[static_cast<std::size_t>(radial)]
                     [static_cast<std::size_t>(component)];
            const double energy = value * value;
            total_energy += energy;
            if (radial < 3 || radial >= radial_cells - 3)
            {
                boundary_energy += energy;
            }
        }
    }
    return total_energy > 0.0 ? boundary_energy / total_energy : 0.0;
}

int maximum_radial_index(const RadialVector &input)
{
    int maximum_index = 0;
    double maximum = -1.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        double energy = 0.0;
        for (double value : input[static_cast<std::size_t>(radial)])
        {
            energy += value * value;
        }
        if (energy > maximum)
        {
            maximum = energy;
            maximum_index = radial;
        }
    }
    return maximum_index;
}

double constraint_norm(const RadialConstraints &constraints,
                       const int component, const double dx)
{
    double result = 0.0;
    for (const auto &row : constraints)
    {
        const double value = row[static_cast<std::size_t>(component)];
        result += value * value;
    }
    return std::sqrt(result * dx);
}

struct FullSnapshot
{
    std::vector<double> state;
    std::vector<double> constraints;
};

struct ActionResult
{
    RadialVector response{};
    RadialConstraints constraints{};
    double parity_leakage = 0.0;
    double harmonic_leakage = 0.0;
};

class D13TangentLevel : public BlackStringToyLevel
{
    friend class DefaultLevelFactory<D13TangentLevel>;

  protected:
    using BlackStringToyLevel::BlackStringToyLevel;

  public:
    void prepare()
    {
        const Box domain = m_problem_domain.domainBox();
        require(domain.size(0) == radial_cells &&
                    domain.size(1) == compact_cells,
                "D13 requires the exact 32x64 domain");
        require(m_p.max_level == 0, "D13 must remain level zero");
        require(m_p.physical_radial_boundaries &&
                    !m_p.background_preserving_gp_radial_ghosts,
                "D13 requires the provisional physical radial boundary");
        require(m_p.ko_sigma == 0.3, "D13 requires ko_sigma=0.3");
        require(m_p.constraint_diagnostic_cadence == 0,
                "D13 production diagnostics must be disabled");
        m_dt = m_dx * m_p.dt_multiplier;
        m_time = 0.0;
        require(std::abs(m_dx - 0.125) < 1.0e-14,
                "D13 requires dx=dz=1/8");
        require(std::abs(m_dt - 0.00625) < 1.0e-14,
                "D13 requires CFL=0.05");
    }

    double spacing() const { return m_dx; }
    double timestep() const { return m_dt; }

    StateRow field_scales() const
    {
        StateRow scales{};
        const Box domain = m_problem_domain.domainBox();
        const auto offset = m_p.coordinate_offset();
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            IntVect point = domain.smallEnd();
            point[0] += radial;
            const double x =
                BlackStringGPInitialData::cell_centered_coordinates(
                    point, m_dx, offset)
                    .x;
            const auto background =
                BlackStringGPPointwiseInitialData::make_pointwise_state(
                    m_p.r0, x);
            for (int component = 0; component < component_count; ++component)
            {
                const double value =
                    background[static_cast<std::size_t>(component)];
                scales[static_cast<std::size_t>(component)] += value * value;
            }
        }
        for (double &value : scales)
        {
            value = std::max(0.1, std::sqrt(value / radial_cells));
        }
        return scales;
    }

    std::array<double, radial_cells> radial_coordinates() const
    {
        std::array<double, radial_cells> result{};
        const Box domain = m_problem_domain.domainBox();
        const auto offset = m_p.coordinate_offset();
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            IntVect point = domain.smallEnd();
            point[0] += radial;
            result[static_cast<std::size_t>(radial)] =
                BlackStringGPInitialData::cell_centered_coordinates(
                    point, m_dx, offset)
                    .x;
        }
        return result;
    }

    ActionResult apply_tangent(const RadialVector &input,
                               const int mode_number, const double epsilon)
    {
        require(mode_number == 1 || mode_number == 2,
                "D13 supports only k=pi/4 and pi/2");
        require(epsilon > 0.0 && std::isfinite(epsilon),
                "D13 epsilon must be finite and positive");
        const FullSnapshot positive =
            step_and_capture(input, mode_number, epsilon);
        const FullSnapshot negative =
            step_and_capture(input, mode_number, -epsilon);
        ++m_tangent_actions;
        return project_difference(positive, negative, mode_number, epsilon);
    }

    std::size_t tangent_actions() const { return m_tangent_actions; }
    std::size_t live_steps() const { return 2 * m_tangent_actions; }

  private:
    static std::size_t state_index(const int radial, const int compact,
                                   const int component)
    {
        return (static_cast<std::size_t>(radial) * compact_cells +
                static_cast<std::size_t>(compact)) *
                   component_count +
               static_cast<std::size_t>(component);
    }

    static std::size_t constraint_index(const int radial, const int compact,
                                        const int component)
    {
        return (static_cast<std::size_t>(radial) * compact_cells +
                static_cast<std::size_t>(compact)) *
                   constraint_count +
               static_cast<std::size_t>(component);
    }

    void reset_state(const RadialVector &input, const int mode_number,
                     const double signed_epsilon)
    {
        const Box domain = m_problem_domain.domainBox();
        const auto offset = m_p.coordinate_offset();
        const double k = 2.0 * pi * mode_number / 8.0;
        const DataIterator iterator = m_state_new.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            FArrayBox &state = m_state_new[data_index];
            const Box valid = m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const int radial = point[0] - domain.smallEnd(0);
                const auto coordinates =
                    BlackStringGPInitialData::cell_centered_coordinates(
                        point, m_dx, offset);
                const auto background =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        m_p.r0, coordinates.x);
                const double cosine = std::cos(k * coordinates.z);
                const double sine = std::sin(k * coordinates.z);
                for (int component = 0; component < component_count;
                     ++component)
                {
                    const double basis =
                        one_z_component(component) ? sine : cosine;
                    state(point, component) =
                        background[static_cast<std::size_t>(component)] +
                        signed_epsilon * basis *
                            input[static_cast<std::size_t>(radial)]
                                 [static_cast<std::size_t>(component)];
                }
            }
        }
        m_time = 0.0;
        fillAllGhosts();
    }

    FullSnapshot step_and_capture(const RadialVector &input,
                                  const int mode_number,
                                  const double signed_epsilon)
    {
        reset_state(input, mode_number, signed_epsilon);
        // GRAMRLevel narrows the access of AMRLevel::advance to private.
        // Calling through the public AMRLevel interface still dispatches to
        // the real GRAMRLevel four-stage implementation.
        (void)static_cast<AMRLevel *>(this)->advance();
        fillAllGhosts();
        BoxLoops::loop(
            BlackStringLive::ConstraintCompute(m_dx, m_p.coordinate_offset()),
            m_state_new, m_state_diagnostics, EXCLUDE_GHOST_CELLS,
            disable_simd());

        FullSnapshot snapshot;
        snapshot.state.resize(static_cast<std::size_t>(radial_cells) *
                              compact_cells * component_count);
        snapshot.constraints.resize(
            static_cast<std::size_t>(radial_cells) * compact_cells *
            constraint_count);
        const Box domain = m_problem_domain.domainBox();
        const DataIterator iterator = m_state_new.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &state = m_state_new[data_index];
            const FArrayBox &constraints = m_state_diagnostics[data_index];
            const Box valid = m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const int radial = point[0] - domain.smallEnd(0);
                const int compact = point[1] - domain.smallEnd(1);
                for (int component = 0; component < component_count;
                     ++component)
                {
                    snapshot.state[state_index(radial, compact, component)] =
                        state(point, component);
                }
                snapshot.constraints[constraint_index(radial, compact, 0)] =
                    constraints(point, c_Ham);
                snapshot.constraints[constraint_index(radial, compact, 1)] =
                    constraints(point, c_MomX);
                snapshot.constraints[constraint_index(radial, compact, 2)] =
                    constraints(point, c_MomZ);
            }
        }
        return snapshot;
    }

    ActionResult project_difference(const FullSnapshot &positive,
                                    const FullSnapshot &negative,
                                    const int mode_number,
                                    const double epsilon) const
    {
        ActionResult result;
        RadialVector forbidden{};
        RadialVector harmonic{};
        const Box domain = m_problem_domain.domainBox();
        const auto offset = m_p.coordinate_offset();
        const double k = 2.0 * pi * mode_number / 8.0;
        const int other_mode = mode_number == 1 ? 2 : 1;
        const double other_k = 2.0 * pi * other_mode / 8.0;
        const double factor = 2.0 / (compact_cells * 2.0 * epsilon);

        for (int compact = 0; compact < compact_cells; ++compact)
        {
            IntVect point = domain.smallEnd();
            point[1] += compact;
            const double z =
                BlackStringGPInitialData::cell_centered_coordinates(
                    point, m_dx, offset)
                    .z;
            const double cosine = std::cos(k * z);
            const double sine = std::sin(k * z);
            const double other_cosine = std::cos(other_k * z);
            const double other_sine = std::sin(other_k * z);
            for (int radial = 0; radial < radial_cells; ++radial)
            {
                for (int component = 0; component < component_count;
                     ++component)
                {
                    const double delta =
                        positive.state[state_index(radial, compact,
                                                   component)] -
                        negative.state[state_index(radial, compact,
                                                   component)];
                    const bool odd = one_z_component(component);
                    result.response[static_cast<std::size_t>(radial)]
                                   [static_cast<std::size_t>(component)] +=
                        factor * delta * (odd ? sine : cosine);
                    forbidden[static_cast<std::size_t>(radial)]
                             [static_cast<std::size_t>(component)] +=
                        factor * delta * (odd ? cosine : sine);
                    harmonic[static_cast<std::size_t>(radial)]
                            [static_cast<std::size_t>(component)] +=
                        factor * delta *
                        (odd ? other_sine : other_cosine);
                }
                for (int component = 0; component < constraint_count;
                     ++component)
                {
                    const double delta =
                        positive.constraints[constraint_index(
                            radial, compact, component)] -
                        negative.constraints[constraint_index(
                            radial, compact, component)];
                    const bool odd = component == 2;
                    result.constraints[static_cast<std::size_t>(radial)]
                                      [static_cast<std::size_t>(component)] +=
                        factor * delta * (odd ? sine : cosine);
                }
            }
        }
        const double response_norm = raw_norm(result.response, m_dx);
        require(std::isfinite(response_norm) && response_norm > 0.0,
                "D13 tangent response is zero or nonfinite");
        result.parity_leakage =
            raw_norm(forbidden, m_dx) / response_norm;
        result.harmonic_leakage =
            raw_norm(harmonic, m_dx) / response_norm;
        return result;
    }

    std::size_t m_tangent_actions = 0;
};

enum class SeedKind
{
    physical,
    smooth,
    inner
};

const char *seed_name(const SeedKind seed)
{
    switch (seed)
    {
    case SeedKind::physical:
        return "d12_physical";
    case SeedKind::smooth:
        return "smooth_bulk";
    case SeedKind::inner:
        return "inner_localized";
    }
    return "unknown";
}

RadialVector make_seed(const SeedKind seed, const int mode_number,
                       const std::array<double, radial_cells> &x)
{
    RadialVector result{};
    const double k = 2.0 * pi * mode_number / 8.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        const double coordinate = x[static_cast<std::size_t>(radial)];
        if (seed == SeedKind::physical)
        {
            const Profile profile = compact_profile(coordinate);
            auto &row = result[static_cast<std::size_t>(radial)];
            row[c_chi] = 0.25 * profile.value;
            row[c_hxx] = 0.50 * profile.value;
            row[c_hxz] = 0.10 * profile.value;
            row[c_hzz] = -0.25 * profile.value;
            row[c_hww] = -0.125 * profile.value;
            row[c_Axx] = -0.40 * profile.value;
            row[c_Axz] = 0.02 * profile.value;
            row[c_Azz] = 0.20 * profile.value;
            row[c_Aww] = 0.10 * profile.value;
            row[c_GammaX] =
                0.50 * profile.derivative +
                0.10 * k * profile.value +
                1.25 * profile.value / coordinate;
            row[c_GammaZ] =
                0.10 * profile.derivative +
                0.20 * profile.value / coordinate +
                0.25 * k * profile.value;
            continue;
        }

        const double radial_profile =
            seed == SeedKind::smooth ? compact_profile(coordinate).value
                                     : inner_profile(coordinate);
        for (int component = 0; component < component_count; ++component)
        {
            const double deterministic_weight =
                (component % 2 == 0 ? 1.0 : -1.0) *
                (0.35 + 0.03 * static_cast<double>(component));
            result[static_cast<std::size_t>(radial)]
                  [static_cast<std::size_t>(component)] =
                deterministic_weight * radial_profile *
                (1.0 + 0.05 * std::sin((component + 1) * coordinate));
        }
    }
    return result;
}

struct IterationRecord
{
    int iteration = 0;
    double lambda_raw = 0.0;
    double lambda_scaled = 0.0;
    double omega_raw = 0.0;
    double omega_scaled = 0.0;
    double signed_alignment = 0.0;
    double overlap = 0.0;
    double parity_leakage = 0.0;
    double harmonic_leakage = 0.0;
    double nyquist_fraction = 0.0;
    std::array<double, constraint_count> constraint_norms{};
};

struct SequenceResult
{
    int mode_number = 0;
    SeedKind seed = SeedKind::physical;
    bool converged = false;
    int iterations = 0;
    RadialVector mode{};
    ActionResult final_action;
    std::vector<IterationRecord> history;
    double epsilon_difference = std::numeric_limits<double>::infinity();
    double epsilon_lambda_relative_difference =
        std::numeric_limits<double>::infinity();
    double norm_omega_relative_difference =
        std::numeric_limits<double>::infinity();
};

double recent_rate_variation(const std::vector<IterationRecord> &history)
{
    if (history.size() < convergence_history)
    {
        return std::numeric_limits<double>::infinity();
    }
    const auto begin = history.end() - convergence_history;
    double minimum = begin->omega_raw;
    double maximum = begin->omega_raw;
    double scale_value = 0.0;
    for (auto it = begin; it != history.end(); ++it)
    {
        minimum = std::min(minimum, it->omega_raw);
        maximum = std::max(maximum, it->omega_raw);
        scale_value = std::max(scale_value, std::abs(it->omega_raw));
    }
    return scale_value > 0.0 ? (maximum - minimum) / scale_value
                             : std::numeric_limits<double>::infinity();
}

SequenceResult run_sequence(D13TangentLevel &level, const int mode_number,
                            const SeedKind seed, const StateRow &field_scales,
                            const std::array<double, radial_cells> &x)
{
    SequenceResult result;
    result.mode_number = mode_number;
    result.seed = seed;
    RadialVector iterate = make_seed(seed, mode_number, x);
    const double initial_norm = raw_norm(iterate, level.spacing());
    require(initial_norm > 0.0, "D13 seed has zero norm");
    scale(iterate, 1.0 / initial_norm);

    int consecutive = 0;
    for (int iteration = 1; iteration <= primary_iterations; ++iteration)
    {
        ActionResult action =
            level.apply_tangent(iterate, mode_number, epsilon_primary);
        const double input_raw = raw_norm(iterate, level.spacing());
        const double output_raw =
            raw_norm(action.response, level.spacing());
        const double input_scaled =
            scaled_norm(iterate, field_scales, level.spacing());
        const double output_scaled =
            scaled_norm(action.response, field_scales, level.spacing());
        const double signed_dot =
            raw_dot(iterate, action.response, level.spacing());
        const double signed_alignment =
            signed_dot >= 0.0 ? 1.0 : -1.0;
        const double lambda_raw = output_raw / input_raw;
        const double lambda_scaled = output_scaled / input_scaled;
        const double omega_raw =
            std::log(std::abs(lambda_raw)) / level.timestep();
        const double omega_scaled =
            std::log(std::abs(lambda_scaled)) / level.timestep();
        const double overlap =
            std::abs(signed_dot) / (input_raw * output_raw);

        IterationRecord record;
        record.iteration = iteration;
        record.lambda_raw = lambda_raw;
        record.lambda_scaled = lambda_scaled;
        record.omega_raw = omega_raw;
        record.omega_scaled = omega_scaled;
        record.signed_alignment = signed_alignment;
        record.overlap = overlap;
        record.parity_leakage = action.parity_leakage;
        record.harmonic_leakage = action.harmonic_leakage;
        record.nyquist_fraction =
            vector_nyquist_fraction(action.response, level.spacing());
        for (int constraint = 0; constraint < constraint_count; ++constraint)
        {
            record.constraint_norms[static_cast<std::size_t>(constraint)] =
                constraint_norm(action.constraints, constraint,
                                level.spacing()) /
                output_raw;
        }
        result.history.push_back(record);
        std::cout << std::scientific << std::setprecision(12)
                  << "D13_ITER mode_number=" << mode_number
                  << " k=" << 2.0 * pi * mode_number / 8.0
                  << " seed=" << seed_name(seed)
                  << " iteration=" << iteration
                  << " lambda_raw=" << lambda_raw
                  << " lambda_scaled=" << lambda_scaled
                  << " Omega_raw=" << omega_raw
                  << " Omega_scaled=" << omega_scaled
                  << " sign=" << signed_alignment
                  << " overlap=" << overlap
                  << " rate_variation=" << recent_rate_variation(result.history)
                  << " parity_leakage=" << action.parity_leakage
                  << " harmonic_leakage=" << action.harmonic_leakage
                  << " radial_nyquist=" << record.nyquist_fraction
                  << " H_over_state=" << record.constraint_norms[0]
                  << " Mx_over_state=" << record.constraint_norms[1]
                  << " Mz_over_state=" << record.constraint_norms[2] << '\n';

        iterate = action.response;
        scale(iterate, 1.0 / output_raw);
        if (overlap > overlap_threshold &&
            recent_rate_variation(result.history) <
                rate_variation_threshold)
        {
            ++consecutive;
        }
        else
        {
            consecutive = 0;
        }
        if (consecutive >= convergence_history)
        {
            result.converged = true;
            break;
        }
    }
    result.iterations = static_cast<int>(result.history.size());
    result.mode = iterate;

    const ActionResult primary_check =
        level.apply_tangent(iterate, mode_number, epsilon_primary);
    const ActionResult secondary_check =
        level.apply_tangent(iterate, mode_number, epsilon_secondary);
    result.final_action = primary_check;
    result.epsilon_difference = relative_projective_difference(
        primary_check.response, secondary_check.response, level.spacing());
    const double lambda_primary =
        raw_norm(primary_check.response, level.spacing()) /
        raw_norm(iterate, level.spacing());
    const double lambda_secondary =
        raw_norm(secondary_check.response, level.spacing()) /
        raw_norm(iterate, level.spacing());
    result.epsilon_lambda_relative_difference =
        std::abs(lambda_primary - lambda_secondary) /
        std::max(std::abs(lambda_primary), std::numeric_limits<double>::min());
    const double omega_raw =
        std::log(std::abs(lambda_primary)) / level.timestep();
    const double lambda_scaled =
        scaled_norm(primary_check.response, field_scales, level.spacing()) /
        scaled_norm(iterate, field_scales, level.spacing());
    const double omega_scaled =
        std::log(std::abs(lambda_scaled)) / level.timestep();
    result.norm_omega_relative_difference =
        std::abs(omega_raw - omega_scaled) /
        std::max({std::abs(omega_raw), std::abs(omega_scaled), 1.0e-14});
    require(result.epsilon_difference <= epsilon_agreement_tolerance,
            "D13 normalized tangent response failed epsilon agreement");
    require(result.epsilon_lambda_relative_difference <=
                epsilon_agreement_tolerance,
            "D13 amplification failed epsilon agreement");
    return result;
}

void print_sequence_summary(
    const SequenceResult &result, const StateRow &field_scales,
    const std::array<double, radial_cells> &x, const double dx,
    const double dt)
{
    const auto &final = result.final_action.response;
    const double final_norm = raw_norm(final, dx);
    const double input_norm = raw_norm(result.mode, dx);
    const double lambda = final_norm / input_norm;
    const double omega = std::log(std::abs(lambda)) / dt;
    const double signed_alignment =
        raw_dot(result.mode, final, dx) >= 0.0 ? 1.0 : -1.0;
    const int max_radial = maximum_radial_index(final);
    const double bdy_fraction = boundary_fraction(final);
    const double nyquist = vector_nyquist_fraction(final, dx);
    std::cout << std::scientific << std::setprecision(12)
              << "D13_SEQUENCE mode_number=" << result.mode_number
              << " k=" << 2.0 * pi * result.mode_number / 8.0
              << " seed=" << seed_name(result.seed)
              << " converged=" << (result.converged ? 1 : 0)
              << " iterations=" << result.iterations
              << " lambda=" << lambda << " Omega=" << omega
              << " sign=" << signed_alignment
              << " final_overlap=" << result.history.back().overlap
              << " rate_variation="
              << recent_rate_variation(result.history)
              << " epsilon_profile_difference="
              << result.epsilon_difference
              << " epsilon_lambda_relative_difference="
              << result.epsilon_lambda_relative_difference
              << " norm_Omega_relative_difference="
              << result.norm_omega_relative_difference
              << " parity_leakage="
              << result.final_action.parity_leakage
              << " harmonic_leakage="
              << result.final_action.harmonic_leakage
              << " radial_nyquist=" << nyquist
              << " boundary_six_cell_fraction=" << bdy_fraction
              << " maximum_radial_index=" << max_radial
              << " maximum_x=" << x[static_cast<std::size_t>(max_radial)]
              << " H_over_state="
              << constraint_norm(result.final_action.constraints, 0, dx) /
                     final_norm
              << " Mx_over_state="
              << constraint_norm(result.final_action.constraints, 1, dx) /
                     final_norm
              << " Mz_over_state="
              << constraint_norm(result.final_action.constraints, 2, dx) /
                     final_norm
              << '\n';

    double total_energy = 0.0;
    StateRow component_energy{};
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            const double value =
                final[static_cast<std::size_t>(radial)]
                     [static_cast<std::size_t>(component)];
            const double energy = value * value;
            component_energy[static_cast<std::size_t>(component)] += energy;
            total_energy += energy;
        }
    }
    for (int component = 0; component < component_count; ++component)
    {
        double input_component_norm = 0.0;
        double output_component_norm = 0.0;
        double component_dot = 0.0;
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            const double input =
                result.mode[static_cast<std::size_t>(radial)]
                           [static_cast<std::size_t>(component)];
            const double output =
                final[static_cast<std::size_t>(radial)]
                     [static_cast<std::size_t>(component)];
            input_component_norm += input * input;
            output_component_norm += output * output;
            component_dot += input * output;
        }
        const double component_lambda =
            input_component_norm > 0.0
                ? std::sqrt(output_component_norm / input_component_norm)
                : 0.0;
        std::cout << "D13_COMPONENT mode_number=" << result.mode_number
                  << " seed=" << seed_name(result.seed)
                  << " slot=" << component
                  << " name="
                  << UserVariables::variable_names
                         [static_cast<std::size_t>(component)]
                  << " raw_fraction="
                  << (total_energy > 0.0
                          ? component_energy[static_cast<std::size_t>(
                                component)] /
                                total_energy
                          : 0.0)
                  << " field_scale="
                  << field_scales[static_cast<std::size_t>(component)]
                  << " component_lambda=" << component_lambda
                  << " component_sign="
                  << (component_dot >= 0.0 ? 1.0 : -1.0) << '\n';
    }
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        double row_energy = 0.0;
        for (double value : final[static_cast<std::size_t>(radial)])
        {
            row_energy += value * value;
        }
        std::cout << "D13_PROFILE mode_number=" << result.mode_number
                  << " seed=" << seed_name(result.seed)
                  << " radial_index=" << radial
                  << " x=" << x[static_cast<std::size_t>(radial)]
                  << " row_norm=" << std::sqrt(row_energy)
                  << " normalized_row_norm="
                  << (final_norm > 0.0 ? std::sqrt(row_energy) / final_norm
                                       : 0.0);
        for (int component = 0; component < component_count; ++component)
        {
            std::cout << " v" << component << '='
                      << final[static_cast<std::size_t>(radial)]
                              [static_cast<std::size_t>(component)] /
                             final_norm;
        }
        std::cout << '\n';
    }
}

struct CrossSeedReport
{
    bool two_seed_convergence = false;
    double best_overlap = 0.0;
    double rate_relative_difference =
        std::numeric_limits<double>::infinity();
};

CrossSeedReport compare_seeds(const std::vector<SequenceResult> &results,
                              const int mode_number, const double dx,
                              const double dt)
{
    CrossSeedReport report;
    for (std::size_t left = 0; left < results.size(); ++left)
    {
        if (results[left].mode_number != mode_number ||
            !results[left].converged)
        {
            continue;
        }
        for (std::size_t right = left + 1; right < results.size(); ++right)
        {
            if (results[right].mode_number != mode_number ||
                !results[right].converged)
            {
                continue;
            }
            const double overlap =
                normalized_overlap(results[left].final_action.response,
                                   results[right].final_action.response, dx);
            const double left_lambda =
                raw_norm(results[left].final_action.response, dx) /
                raw_norm(results[left].mode, dx);
            const double right_lambda =
                raw_norm(results[right].final_action.response, dx) /
                raw_norm(results[right].mode, dx);
            const double left_omega = std::log(std::abs(left_lambda)) / dt;
            const double right_omega = std::log(std::abs(right_lambda)) / dt;
            const double rate_difference =
                std::abs(left_omega - right_omega) /
                std::max({std::abs(left_omega), std::abs(right_omega),
                          1.0e-14});
            report.best_overlap = std::max(report.best_overlap, overlap);
            report.rate_relative_difference =
                std::min(report.rate_relative_difference, rate_difference);
            if (overlap > overlap_threshold &&
                rate_difference < rate_variation_threshold)
            {
                report.two_seed_convergence = true;
            }
        }
    }
    std::cout << std::scientific << std::setprecision(12)
              << "D13_CROSS_SEED mode_number=" << mode_number
              << " k=" << 2.0 * pi * mode_number / 8.0
              << " two_seed_convergence="
              << (report.two_seed_convergence ? 1 : 0)
              << " best_overlap=" << report.best_overlap
              << " best_rate_relative_difference="
              << report.rate_relative_difference << '\n';
    return report;
}

long peak_rss_kib()
{
    rusage usage{};
    return getrusage(RUSAGE_SELF, &usage) == 0 ? usage.ru_maxrss : -1;
}

int run(SimulationParameters &parameters)
{
    require(parameters.max_level == 0, "D13 requires max_level=0");
    require(parameters.coarsest_dx == 0.125,
            "D13 requires the exact medium grid");
    require(parameters.dt_multiplier == 0.05,
            "D13 requires CFL=0.05");
    require(parameters.ko_sigma == 0.3, "D13 requires ko_sigma=0.3");
    require(parameters.constraint_diagnostic_cadence == 0,
            "D13 diagnostics must be disabled");
    std::cout << "D13_GRID_INPUT ivN=" << parameters.ivN
              << " block_factor=" << parameters.block_factor
              << " max_grid_size=" << parameters.max_grid_size
              << " periodic_x="
              << parameters.boundary_params.is_periodic[0]
              << " periodic_z="
              << parameters.boundary_params.is_periodic[1] << '\n';

    const auto start = std::chrono::steady_clock::now();
    GRAMR amr;
    DefaultLevelFactory<D13TangentLevel> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    require(levels.size() == 1, "D13 must create exactly one level");
    auto *level = dynamic_cast<D13TangentLevel *>(levels[0]);
    require(level != nullptr, "D13 factory returned the wrong level");
    level->prepare();

    const StateRow field_scales = level->field_scales();
    const auto x = level->radial_coordinates();
    std::cout << std::scientific << std::setprecision(12)
              << "D13_CONFIGURATION Lz=8.0 x_in=5.0e-1 x_out=4.5"
              << " Nx=" << radial_cells << " Nz=" << compact_cells
              << " dx=" << level->spacing() << " dz=" << level->spacing()
              << " dt=" << level->timestep() << " CFL=5.0e-2"
              << " ko_sigma=3.0e-1 epsilon_primary=" << epsilon_primary
              << " epsilon_secondary=" << epsilon_secondary
              << " primary_iteration_cap=" << primary_iterations
              << " tangent_action_cap_per_seed="
              << primary_iterations + 2
              << " production_diagnostics=disabled\n";
    std::cout << "D13_LIFECYCLE"
              << " step=GRAMRLevel_advance"
              << " periodic_exchange=framework"
              << " radial_ghosts=black_string_policy"
              << " rhs=direct_target_d4"
              << " gauge=live fixed_lapse_source=enabled"
              << " ko_sigma=0.3 outer_rhs_override=enabled"
              << " rk=locked_four_stage cleanup=each_update"
              << " signed_background_drift=canceled"
              << " fourier_projection=fixture_only\n";

    std::vector<SequenceResult> results;
    for (const int mode_number : {1, 2})
    {
        for (const SeedKind seed :
             {SeedKind::physical, SeedKind::smooth, SeedKind::inner})
        {
            results.push_back(run_sequence(*level, mode_number, seed,
                                           field_scales, x));
            print_sequence_summary(results.back(), field_scales, x,
                                   level->spacing(), level->timestep());
        }
    }

    const CrossSeedReport low =
        compare_seeds(results, 1, level->spacing(), level->timestep());
    const CrossSeedReport high =
        compare_seeds(results, 2, level->spacing(), level->timestep());
    bool credible = false;
    bool boundary_or_grid = false;
    bool constraint_or_gauge = false;
    for (const auto &result : results)
    {
        if (!result.converged)
        {
            continue;
        }
        const bool cross =
            result.mode_number == 1 ? low.two_seed_convergence
                                    : high.two_seed_convergence;
        const double bdy = boundary_fraction(result.final_action.response);
        const double nyquist =
            vector_nyquist_fraction(result.final_action.response,
                                    level->spacing());
        const double state =
            raw_norm(result.final_action.response, level->spacing());
        const double max_constraint =
            std::max({constraint_norm(result.final_action.constraints, 0,
                                      level->spacing()),
                      constraint_norm(result.final_action.constraints, 1,
                                      level->spacing()),
                      constraint_norm(result.final_action.constraints, 2,
                                      level->spacing())}) /
            state;
        double gauge_energy = 0.0;
        double total_energy = 0.0;
        for (const auto &row : result.final_action.response)
        {
            for (int component = 0; component < component_count; ++component)
            {
                const double energy =
                    row[static_cast<std::size_t>(component)] *
                    row[static_cast<std::size_t>(component)];
                total_energy += energy;
                if (component == c_lapse || component == c_shiftX ||
                    component == c_shiftZ || component == c_Bx ||
                    component == c_Bz)
                {
                    gauge_energy += energy;
                }
            }
        }
        const double gauge_fraction =
            total_energy > 0.0 ? gauge_energy / total_energy : 0.0;
        const bool physical_gate =
            cross && bdy < 0.35 && nyquist < 0.20 &&
            result.final_action.parity_leakage < 1.0e-8 &&
            result.final_action.harmonic_leakage < 1.0e-8 &&
            max_constraint < 1.0 &&
            result.epsilon_difference <= epsilon_agreement_tolerance &&
            result.norm_omega_relative_difference < 0.02;
        credible = credible || physical_gate;
        boundary_or_grid =
            boundary_or_grid || (cross && (bdy >= 0.35 || nyquist >= 0.20));
        constraint_or_gauge =
            constraint_or_gauge ||
            (cross && !boundary_or_grid &&
             (max_constraint >= 1.0 || gauge_fraction >= 0.50));
    }

    const char *classification = nullptr;
    if (credible)
    {
        classification =
            "CREDIBLE_KO_STABILIZED_MODE_CANDIDATE_IDENTIFIED";
    }
    else if (boundary_or_grid)
    {
        classification = "BOUNDARY_OR_GRID_MODE_IDENTIFIED";
    }
    else if (constraint_or_gauge)
    {
        classification = "DOMINANT_CONSTRAINT_OR_GAUGE_MODE_IDENTIFIED";
    }
    else
    {
        classification = "NONNORMAL_OR_NO_CONVERGED_MODE";
    }

    const auto stop = std::chrono::steady_clock::now();
    const double wall_seconds =
        std::chrono::duration<double>(stop - start).count();
    const std::size_t actions = level->tangent_actions();
    require(actions <= 6 * (primary_iterations + 2),
            "D13 exceeded its tangent-action budget");
    std::cout << std::scientific << std::setprecision(12)
              << "D13_EFFICIENCY sequences=6 tangent_actions=" << actions
              << " signed_live_steps=" << level->live_steps()
              << " rhs_stages=" << 4 * level->live_steps()
              << " wall_seconds=" << wall_seconds
              << " peak_rss_kib=" << peak_rss_kib()
              << " seconds_per_tangent_action="
              << wall_seconds / static_cast<double>(actions)
              << " valid_cell_rhs_evaluations="
              << static_cast<std::size_t>(radial_cells) * compact_cells * 4 *
                     level->live_steps()
              << '\n'
              << "D13_HOT_PATH direct_target_d4_evaluations_per_cell_stage=1"
              << " fused_ko_additions_per_cell_stage=1"
              << " second_rhs_evaluations=0"
              << " production_logging=0 production_allocations_added=0"
              << " production_counters_added=0 production_transforms_added=0"
              << '\n'
              << "D13_CLASSIFICATION " << classification << '\n'
              << "BLACKSTRING_D13_TANGENT_MODE_PASS\n";
    return 0;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fail("usage: BlackStringD13TangentModeTest <params>");
    }
    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);
    return run(parameters);
}
