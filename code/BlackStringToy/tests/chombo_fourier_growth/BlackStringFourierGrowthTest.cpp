#include "BlackStringToyLevel.hpp"

#include "BlackStringCellStorage.hpp"
#include "BlackStringGPInitialData.hpp"
#include "BoxLoops.hpp"
#include "DefaultLevelFactory.hpp"
#include "GRAMR.hpp"
#include "GRParmParse.hpp"
#include "SetupFunctions.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// Compile the real project level implementation into this strict fixture.
#include "../../BlackStringToyLevel.cpp"

namespace
{
constexpr double pi = 3.141592653589793238462643383279502884;
constexpr double profile_center = 2.25;
constexpr double profile_half_width = 0.75;
constexpr double fit_end = 0.40;
constexpr double analysis_x_min = 1.25;
constexpr double analysis_x_max = 3.50;
constexpr double corrected_gamma_z_derivative_coefficient = 0.10;
constexpr double legacy_gamma_z_derivative_coefficient = 1.00;

enum class RunKind
{
    control,
    seeded,
    legacy_gamma_z
};

struct RunConfiguration
{
    RunKind kind = RunKind::control;
    double epsilon = 0.0;

    bool seeded() const { return kind != RunKind::control; }

    double gamma_z_derivative_coefficient() const
    {
        return kind == RunKind::legacy_gamma_z
                   ? legacy_gamma_z_derivative_coefficient
                   : corrected_gamma_z_derivative_coefficient;
    }
};

RunConfiguration run_configuration;
const char *d9_live_variant = nullptr;

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_FOURIER_GROWTH_FAIL " << message << '\n';
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
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
    const double derivative =
        value * (-2.0 * s) /
        (profile_half_width * denominator * denominator);
    return {value, derivative};
}

template <int mode_number> class FourierInitialData
{
  public:
    FourierInitialData(
        const double r0, const double dx,
        const std::array<double, CH_SPACEDIM> &coordinate_offset,
        const double compact_length)
        : m_r0(r0), m_dx(dx), m_coordinate_offset(coordinate_offset),
          m_compact_length(compact_length)
    {
    }

    void compute(const Cell<double> cell) const
    {
        const auto coordinates =
            BlackStringGPInitialData::cell_centered_coordinates(
                cell.get_int_vect(), m_dx, m_coordinate_offset);
        auto vars = BlackStringGPPointwiseInitialData::make_pointwise_vars(
            m_r0, coordinates.x);
        const Profile profile = compact_profile(coordinates.x);
        const double k = 2.0 * pi * mode_number / m_compact_length;
        const double cosine = std::cos(k * coordinates.z);
        const double sine = std::sin(k * coordinates.z);
        const double epsilon =
            run_configuration.seeded() ? run_configuration.epsilon : 0.0;
        const double even = epsilon * profile.value * cosine;
        const double odd = epsilon * profile.value * sine;

        namespace Production = BlackStringProductionVariables;
        namespace Reduced = BlackStringReducedVars;

        // A small determinant-free-at-linear-order physical seed. Scalar and
        // zero-z components use cos(kz); the one-z metric, curvature, and
        // contracted-connection components use sin(kz).
        Reduced::component(vars, Production::c_chi) += 0.25 * even;
        Reduced::component(vars, Production::c_hxx) += 0.50 * even;
        Reduced::component(vars, Production::c_hxz) += 0.10 * odd;
        Reduced::component(vars, Production::c_hzz) -= 0.25 * even;
        Reduced::component(vars, Production::c_hww) -= 0.125 * even;

        // The diagonal A seed has zero four-dimensional trace at linear order.
        Reduced::component(vars, Production::c_Axx) -= 0.40 * even;
        Reduced::component(vars, Production::c_Axz) += 0.02 * odd;
        Reduced::component(vars, Production::c_Azz) += 0.20 * even;
        Reduced::component(vars, Production::c_Aww) += 0.10 * even;

        // Prescribed connection seed. The Gamma-z derivative coefficient is
        // audited independently below because the previous fixture used 1.00.
        const double gamma_x =
            epsilon *
            (0.50 * profile.derivative + 0.10 * k * profile.value +
             1.25 * profile.value / coordinates.x) *
            cosine;
        const double gamma_z =
            epsilon *
            (run_configuration.gamma_z_derivative_coefficient() *
                 profile.derivative +
             0.20 * profile.value / coordinates.x +
             0.25 * k * profile.value) *
            sine;
        Reduced::component(vars, Production::c_GammaX) += gamma_x;
        Reduced::component(vars, Production::c_GammaZ) += gamma_z;

        BlackStringCellStorage::store(cell, vars);
    }

  private:
    double m_r0;
    double m_dx;
    std::array<double, CH_SPACEDIM> m_coordinate_offset;
    double m_compact_length;
};

struct Sample
{
    struct RadialFourier
    {
        double x = 0.0;
        double q_cosine = 0.0;
        double q_sine = 0.0;
        std::array<double, 3> constraint_cosine{};
        std::array<double, 3> constraint_sine{};
    };

    double time = 0.0;
    double cosine = 0.0;
    double sine = 0.0;
    double amplitude = 0.0;
    double phase = 0.0;
    std::array<double, NUM_DIAGNOSTIC_VARS> constraints{};
    std::array<double, NUM_DIAGNOSTIC_VARS> constraint_l2{};
    std::array<int, NUM_DIAGNOSTIC_VARS> constraint_maximum_radial{};
    double state_drift = 0.0;
    double state_l2 = 0.0;
    std::array<double, NUM_VARS> variable_maximum{};
    std::array<double, NUM_VARS> variable_l2{};
    std::array<int, NUM_VARS> variable_maximum_radial{};
    std::array<double, NUM_VARS> variable_z_span{};
    std::array<double, NUM_VARS> variable_fourier_amplitude{};
    std::array<double, NUM_VARS> variable_radial_nyquist{};
    double determinant_error = 0.0;
    double determinant_l2 = 0.0;
    int determinant_maximum_radial = 0;
    double weighted_trace_error = 0.0;
    double weighted_trace_l2 = 0.0;
    int weighted_trace_maximum_radial = 0;
    std::vector<RadialFourier> radial_fourier;
    std::vector<RadialFourier> secondary_radial_fourier;
};

const char *maximum_region(const int radial_index, const int radial_cells)
{
    constexpr int boundary_band_cells = 5;
    if (radial_index < boundary_band_cells)
    {
        return "inner_boundary";
    }
    if (radial_index >= radial_cells - boundary_band_cells)
    {
        return "outer_boundary";
    }
    return "bulk";
}

void print_d9_sample(const char *variant, const Sample &sample,
                     const int radial_cells)
{
    const auto maximum_variable = static_cast<int>(std::distance(
        sample.variable_maximum.begin(),
        std::max_element(sample.variable_maximum.begin(),
                         sample.variable_maximum.end())));
    const auto maximum_z_variable = static_cast<int>(std::distance(
        sample.variable_z_span.begin(),
        std::max_element(sample.variable_z_span.begin(),
                         sample.variable_z_span.end())));
    const auto maximum_fourier_variable = static_cast<int>(std::distance(
        sample.variable_fourier_amplitude.begin(),
        std::max_element(sample.variable_fourier_amplitude.begin(),
                         sample.variable_fourier_amplitude.end())));
    const auto maximum_nyquist_variable = static_cast<int>(std::distance(
        sample.variable_radial_nyquist.begin(),
        std::max_element(sample.variable_radial_nyquist.begin(),
                         sample.variable_radial_nyquist.end())));
    std::cout
        << "D9_SAMPLE variant=" << variant << " t=" << sample.time
        << " state_max=" << sample.state_drift
        << " state_l2=" << sample.state_l2
        << " state_max_variable="
        << UserVariables::variable_names[static_cast<std::size_t>(
               maximum_variable)]
        << " state_max_location="
        << maximum_region(
               sample.variable_maximum_radial
                   [static_cast<std::size_t>(maximum_variable)],
               radial_cells)
        << " H_max=" << sample.constraints[c_Ham]
        << " H_l2=" << sample.constraint_l2[c_Ham]
        << " H_location="
        << maximum_region(sample.constraint_maximum_radial[c_Ham],
                          radial_cells)
        << " Mx_max=" << sample.constraints[c_MomX]
        << " Mx_l2=" << sample.constraint_l2[c_MomX]
        << " Mx_location="
        << maximum_region(sample.constraint_maximum_radial[c_MomX],
                          radial_cells)
        << " Mz_max=" << sample.constraints[c_MomZ]
        << " Mz_l2=" << sample.constraint_l2[c_MomZ]
        << " Mz_location="
        << maximum_region(sample.constraint_maximum_radial[c_MomZ],
                          radial_cells)
        << " determinant_max=" << sample.determinant_error
        << " determinant_l2=" << sample.determinant_l2
        << " determinant_location="
        << maximum_region(sample.determinant_maximum_radial, radial_cells)
        << " weighted_trace_max=" << sample.weighted_trace_error
        << " weighted_trace_l2=" << sample.weighted_trace_l2
        << " weighted_trace_location="
        << maximum_region(sample.weighted_trace_maximum_radial, radial_cells)
        << " maximum_z_span="
        << sample.variable_z_span[static_cast<std::size_t>(
               maximum_z_variable)]
        << " maximum_z_span_variable="
        << UserVariables::variable_names[static_cast<std::size_t>(
               maximum_z_variable)]
        << " maximum_fourier="
        << sample.variable_fourier_amplitude[static_cast<std::size_t>(
               maximum_fourier_variable)]
        << " maximum_fourier_variable="
        << UserVariables::variable_names[static_cast<std::size_t>(
               maximum_fourier_variable)]
        << " maximum_radial_nyquist="
        << sample.variable_radial_nyquist[static_cast<std::size_t>(
               maximum_nyquist_variable)]
        << " maximum_radial_nyquist_variable="
        << UserVariables::variable_names[static_cast<std::size_t>(
               maximum_nyquist_variable)]
        << '\n';
    for (int component = 0; component < NUM_VARS; ++component)
    {
        const auto slot = static_cast<std::size_t>(component);
        std::cout << "D9_VARIABLE variant=" << variant
                  << " t=" << sample.time << " slot=" << component
                  << " name=" << UserVariables::variable_names[slot]
                  << " maximum=" << sample.variable_maximum[slot]
                  << " l2=" << sample.variable_l2[slot]
                  << " location="
                  << maximum_region(sample.variable_maximum_radial[slot],
                                    radial_cells)
                  << " z_span=" << sample.variable_z_span[slot]
                  << " fourier=" << sample.variable_fourier_amplitude[slot]
                  << " radial_nyquist="
                  << sample.variable_radial_nyquist[slot] << '\n';
    }
}

struct Fit
{
    double omega = 0.0;
    double intercept = 0.0;
    double standard_error = 0.0;
    double r_squared = 0.0;
    std::size_t points = 0;
};

struct FitWindow
{
    double start = 0.0;
    double end = 0.0;
};

constexpr std::array<FitWindow, 3> fit_windows = {
    FitWindow{0.10, fit_end}, FitWindow{0.15, fit_end},
    FitWindow{0.20, fit_end}};
constexpr std::array<FitWindow, 3> adaptive_scan_fit_windows = {
    FitWindow{0.20, 0.80}, FitWindow{0.30, 0.80},
    FitWindow{0.40, 0.80}};
constexpr std::array<FitWindow, 3> extended_scan_fit_windows = {
    FitWindow{0.40, 1.60}, FitWindow{0.60, 1.60},
    FitWindow{0.80, 1.60}};
// These fixed windows preserve the earlier bounded checkpoint output. They
// describe short-time transients and are not physical stable/unstable labels.

struct ParityReport
{
    double even_cosine = 0.0;
    double even_sine_leakage = 0.0;
    double one_z_sine = 0.0;
    double one_z_cosine_leakage = 0.0;
};

struct SeedReport
{
    double corrected_gamma_z_maximum_error = 0.0;
    double legacy_gamma_z_maximum_difference = 0.0;
};

Fit fit_growth(const std::vector<Sample> &samples, const FitWindow window)
{
    double sum_t = 0.0;
    double sum_y = 0.0;
    double sum_tt = 0.0;
    double sum_ty = 0.0;
    std::size_t count = 0;
    for (const auto &sample : samples)
    {
        if (sample.time + 1.0e-12 < window.start ||
            sample.time - 1.0e-12 > window.end ||
            !(sample.amplitude > 0.0))
        {
            continue;
        }
        const double y = std::log(sample.amplitude);
        sum_t += sample.time;
        sum_y += y;
        sum_tt += sample.time * sample.time;
        sum_ty += sample.time * y;
        ++count;
    }
    require(count >= 4, "growth fit has fewer than four samples");
    const double denominator =
        static_cast<double>(count) * sum_tt - sum_t * sum_t;
    require(denominator > 0.0, "growth fit time window is singular");
    Fit fit;
    fit.points = count;
    fit.omega =
        (static_cast<double>(count) * sum_ty - sum_t * sum_y) / denominator;
    fit.intercept =
        (sum_y - fit.omega * sum_t) / static_cast<double>(count);

    const double mean = sum_y / static_cast<double>(count);
    double residual = 0.0;
    double total = 0.0;
    double centered_time_square_sum = 0.0;
    const double mean_time = sum_t / static_cast<double>(count);
    for (const auto &sample : samples)
    {
        if (sample.time + 1.0e-12 < window.start ||
            sample.time - 1.0e-12 > window.end ||
            !(sample.amplitude > 0.0))
        {
            continue;
        }
        const double y = std::log(sample.amplitude);
        const double predicted = fit.intercept + fit.omega * sample.time;
        residual += (y - predicted) * (y - predicted);
        total += (y - mean) * (y - mean);
        centered_time_square_sum +=
            (sample.time - mean_time) * (sample.time - mean_time);
    }
    fit.r_squared = total > 0.0 ? 1.0 - residual / total : 1.0;
    require(count > 2 && centered_time_square_sum > 0.0,
            "growth fit cannot estimate slope uncertainty");
    fit.standard_error = std::sqrt(
        residual /
        (static_cast<double>(count - 2) * centered_time_square_sum));
    return fit;
}

template <int mode_number>
class FourierGrowthLevel : public BlackStringToyLevel
{
    friend class DefaultLevelFactory<FourierGrowthLevel<mode_number>>;

  protected:
    using BlackStringToyLevel::BlackStringToyLevel;

  public:
    void record_initial_sample() { record_sample(); }

    SeedReport inspect_gamma_z_seed() const
    {
        const Box domain = m_problem_domain.domainBox();
        const int compact_cells = domain.size(1);
        const double compact_length = m_dx * compact_cells;
        const double k = 2.0 * pi * mode_number / compact_length;
        const auto offset = m_p.coordinate_offset();
        SeedReport report;
        const DataIterator iterator = m_state_new.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &state = m_state_new[data_index];
            const Box valid = m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const double x =
                    BlackStringCoordinates::cell_centered<double>(
                        point[0], m_dx, offset[0]);
                const double z =
                    BlackStringCoordinates::cell_centered<double>(
                        point[1], m_dx, offset[1]);
                const Profile profile = compact_profile(x);
                const double sine = std::sin(k * z);
                const auto background =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        m_p.r0, x);
                const double corrected =
                    background[static_cast<std::size_t>(c_GammaZ)] +
                    run_configuration.epsilon *
                        (corrected_gamma_z_derivative_coefficient *
                             profile.derivative +
                         0.20 * profile.value / x +
                         0.25 * k * profile.value) *
                        sine;
                const double legacy =
                    background[static_cast<std::size_t>(c_GammaZ)] +
                    run_configuration.epsilon *
                        (legacy_gamma_z_derivative_coefficient *
                             profile.derivative +
                         0.20 * profile.value / x +
                         0.25 * k * profile.value) *
                        sine;
                report.corrected_gamma_z_maximum_error =
                    std::max(report.corrected_gamma_z_maximum_error,
                             std::abs(state(point, c_GammaZ) - corrected));
                report.legacy_gamma_z_maximum_difference =
                    std::max(report.legacy_gamma_z_maximum_difference,
                             std::abs(corrected - legacy));
            }
        }
        return report;
    }

    ParityReport inspect_initial_parity() const
    {
        const Box domain = m_problem_domain.domainBox();
        const int compact_cells = domain.size(1);
        const double compact_length = m_dx * compact_cells;
        const double k = 2.0 * pi * mode_number / compact_length;
        const auto offset = m_p.coordinate_offset();
        double even_cosine = 0.0;
        double even_sine = 0.0;
        double one_z_sine = 0.0;
        double one_z_cosine = 0.0;
        std::size_t points = 0;
        const DataIterator iterator = m_state_new.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &state = m_state_new[data_index];
            const Box valid = m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const double x =
                    BlackStringCoordinates::cell_centered<double>(
                        point[0], m_dx, offset[0]);
                const double z =
                    BlackStringCoordinates::cell_centered<double>(
                        point[1], m_dx, offset[1]);
                const double cosine = std::cos(k * z);
                const double sine = std::sin(k * z);
                const auto background =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        m_p.r0, x);
                const double even =
                    state(point, c_hww) -
                    background[static_cast<std::size_t>(c_hww)];
                even_cosine += even * cosine;
                even_sine += even * sine;
                for (const int component : {c_hxz, c_Axz, c_GammaZ})
                {
                    const double one_z =
                        state(point, component) -
                        background[static_cast<std::size_t>(component)];
                    one_z_sine += one_z * sine;
                    one_z_cosine += one_z * cosine;
                }
                ++points;
            }
        }
        require(points > 0, "initial parity inspection visited no cells");
        require(run_configuration.epsilon != 0.0,
                "parity inspection requires a seeded run");
        const double normalization =
            2.0 /
            (std::abs(run_configuration.epsilon) *
             static_cast<double>(points));
        return {std::abs(even_cosine) * normalization,
                std::abs(even_sine) * normalization,
                std::abs(one_z_sine) * normalization,
                std::abs(one_z_cosine) * normalization};
    }

    const std::vector<Sample> &samples() const { return m_samples; }

    double maximum_state_drift() const { return m_maximum_state_drift; }

    double maximum_phase_rotation_error() const
    {
        return m_maximum_phase_rotation_error;
    }

    bool finite() const { return m_finite; }

    int radial_cells() const { return m_problem_domain.domainBox().size(0); }

    int compact_cells() const { return m_problem_domain.domainBox().size(1); }

    double spacing() const { return m_dx; }

    double maximum_component_drift(const int component) const
    {
        double result = 0.0;
        for (const auto &sample : m_samples)
        {
            result = std::max(
                result,
                sample.variable_maximum[static_cast<std::size_t>(component)]);
        }
        return result;
    }

    bool d9_ghost_audit_complete() const { return false; }
    bool d9_valid_cells_unchanged() const { return false; }
    double d9_minimum_inner_ghost_x() const
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    std::array<double, 3> d9_inner_ghost_coordinates() const { return {}; }

  protected:
    void fillIntralevelGhosts(const Interval &components) override
    {
        instrumentation().begin_fillall_periodic_exchange();
        m_state_new.exchange(components, m_exchange_copier);
        fillBdyGhosts(m_state_new, components);
    }

  private:
    void initialData() override
    {
        instrumentation().record_initial_data();
        const int compact_cells = m_problem_domain.domainBox().size(1);
        const double compact_length = m_dx * compact_cells;
        BoxLoops::loop(
            FourierInitialData<mode_number>(
                m_p.r0, m_dx, m_p.coordinate_offset(), compact_length),
            m_state_new, m_state_new, EXCLUDE_GHOST_CELLS, disable_simd());
        fillAllGhosts();
    }

    void specificPostTimeStep() override
    {
        instrumentation().record_post_step();
        const auto completed_steps =
            static_cast<std::size_t>(std::llround(m_time / m_dt));
        if (m_p.constraint_diagnostic_due(completed_steps))
        {
            record_sample();
        }
    }

    void record_sample()
    {
        fillAllGhosts();
        BoxLoops::loop(
            BlackStringLive::ConstraintCompute(m_dx, m_p.coordinate_offset()),
            m_state_new, m_state_diagnostics, EXCLUDE_GHOST_CELLS,
            disable_simd());
        instrumentation().record_diagnostic_evaluation();

        const Box domain = m_problem_domain.domainBox();
        const int compact_cells = domain.size(1);
        const int radial_cells = domain.size(0);
        const double compact_length = m_dx * compact_cells;
        const double k = 2.0 * pi * mode_number / compact_length;
        constexpr int secondary_mode_number = mode_number == 1 ? 2 : 1;
        const double secondary_k =
            2.0 * pi * secondary_mode_number / compact_length;
        const auto offset = m_p.coordinate_offset();
        std::vector<double> cosine_sum(
            static_cast<std::size_t>(radial_cells), 0.0);
        std::vector<double> sine_sum(
            static_cast<std::size_t>(radial_cells), 0.0);
        std::vector<double> secondary_cosine_sum(
            static_cast<std::size_t>(radial_cells), 0.0);
        std::vector<double> secondary_sine_sum(
            static_cast<std::size_t>(radial_cells), 0.0);
        std::vector<int> compact_count(
            static_cast<std::size_t>(radial_cells), 0);
        std::array<std::vector<double>, NUM_VARS> variable_cosine_sum;
        std::array<std::vector<double>, NUM_VARS> variable_sine_sum;
        std::array<std::vector<double>, NUM_VARS>
            variable_radial_difference_sum;
        std::array<std::vector<double>, NUM_VARS> variable_minimum;
        std::array<std::vector<double>, NUM_VARS> variable_maximum;
        for (int component = 0; component < NUM_VARS; ++component)
        {
            const auto slot = static_cast<std::size_t>(component);
            variable_cosine_sum[slot].assign(
                static_cast<std::size_t>(radial_cells), 0.0);
            variable_sine_sum[slot].assign(
                static_cast<std::size_t>(radial_cells), 0.0);
            variable_radial_difference_sum[slot].assign(
                static_cast<std::size_t>(radial_cells), 0.0);
            variable_minimum[slot].assign(
                static_cast<std::size_t>(radial_cells),
                std::numeric_limits<double>::infinity());
            variable_maximum[slot].assign(
                static_cast<std::size_t>(radial_cells),
                -std::numeric_limits<double>::infinity());
        }
        constexpr std::array<int, 3> constraint_components = {
            c_Ham, c_MomX, c_MomZ};
        std::array<std::vector<double>, 3> constraint_cosine_sum;
        std::array<std::vector<double>, 3> constraint_sine_sum;
        std::array<std::vector<double>, 3>
            secondary_constraint_cosine_sum;
        std::array<std::vector<double>, 3>
            secondary_constraint_sine_sum;
        for (std::size_t diagnostic = 0;
             diagnostic < constraint_components.size(); ++diagnostic)
        {
            constraint_cosine_sum[diagnostic].assign(
                static_cast<std::size_t>(radial_cells), 0.0);
            constraint_sine_sum[diagnostic].assign(
                static_cast<std::size_t>(radial_cells), 0.0);
            secondary_constraint_cosine_sum[diagnostic].assign(
                static_cast<std::size_t>(radial_cells), 0.0);
            secondary_constraint_sine_sum[diagnostic].assign(
                static_cast<std::size_t>(radial_cells), 0.0);
        }

        Sample sample;
        sample.time = m_time;
        sample.constraints.fill(0.0);
        sample.constraint_l2.fill(0.0);
        sample.constraint_maximum_radial.fill(0);
        sample.variable_maximum.fill(0.0);
        sample.variable_l2.fill(0.0);
        sample.variable_maximum_radial.fill(0);
        sample.variable_z_span.fill(0.0);
        sample.variable_fourier_amplitude.fill(0.0);
        sample.variable_radial_nyquist.fill(0.0);
        double state_square_sum = 0.0;
        double determinant_square_sum = 0.0;
        double weighted_trace_square_sum = 0.0;
        std::array<double, NUM_DIAGNOSTIC_VARS> constraint_square_sum{};
        std::size_t valid_points = 0;
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
                const double x =
                    BlackStringCoordinates::cell_centered<double>(
                        point[0], m_dx, offset[0]);
                const double z =
                    BlackStringCoordinates::cell_centered<double>(
                        point[1], m_dx, offset[1]);
                const int radial_index = point[0] - domain.smallEnd(0);
                const double cosine = std::cos(k * z);
                const double sine = std::sin(k * z);
                const double secondary_cosine = std::cos(secondary_k * z);
                const double secondary_sine = std::sin(secondary_k * z);
                const double chi = state(point, c_chi);
                const double hww = state(point, c_hww);
                m_finite = m_finite && std::isfinite(chi) &&
                           std::isfinite(hww) && chi > 0.0 && hww > 0.0;
                if (chi > 0.0 && hww > 0.0)
                {
                    const double areal_log = 0.5 * std::log(hww / chi);
                    cosine_sum[static_cast<std::size_t>(radial_index)] +=
                        areal_log * cosine;
                    sine_sum[static_cast<std::size_t>(radial_index)] +=
                        areal_log * sine;
                    secondary_cosine_sum
                        [static_cast<std::size_t>(radial_index)] +=
                        areal_log * secondary_cosine;
                    secondary_sine_sum
                        [static_cast<std::size_t>(radial_index)] +=
                        areal_log * secondary_sine;
                    ++compact_count[static_cast<std::size_t>(radial_index)];
                }

                const auto background =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        m_p.r0, x);
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    const auto slot = static_cast<std::size_t>(component);
                    const double value = state(point, component);
                    m_finite = m_finite && std::isfinite(value);
                    const double difference =
                        value - background[static_cast<std::size_t>(component)];
                    const double absolute_difference = std::abs(difference);
                    if (absolute_difference > sample.variable_maximum[slot])
                    {
                        sample.variable_maximum[slot] = absolute_difference;
                        sample.variable_maximum_radial[slot] = radial_index;
                    }
                    sample.variable_l2[slot] += difference * difference;
                    state_square_sum += difference * difference;
                    sample.state_drift =
                        std::max(sample.state_drift, absolute_difference);
                    variable_cosine_sum[slot]
                                       [static_cast<std::size_t>(
                                           radial_index)] += value * cosine;
                    variable_sine_sum[slot]
                                     [static_cast<std::size_t>(
                                         radial_index)] += value * sine;
                    variable_radial_difference_sum[slot]
                                                  [static_cast<std::size_t>(
                                                      radial_index)] +=
                        difference;
                    variable_minimum[slot]
                                    [static_cast<std::size_t>(radial_index)] =
                        std::min(variable_minimum[slot]
                                                [static_cast<std::size_t>(
                                                    radial_index)],
                                 value);
                    variable_maximum[slot]
                                    [static_cast<std::size_t>(radial_index)] =
                        std::max(variable_maximum[slot]
                                                [static_cast<std::size_t>(
                                                    radial_index)],
                                 value);
                }
                for (int component = 0; component < NUM_DIAGNOSTIC_VARS;
                     ++component)
                {
                    const auto slot = static_cast<std::size_t>(component);
                    const double value = constraints(point, component);
                    m_finite = m_finite && std::isfinite(value);
                    const double absolute_value = std::abs(value);
                    if (absolute_value > sample.constraints[slot])
                    {
                        sample.constraints[slot] = absolute_value;
                        sample.constraint_maximum_radial[slot] = radial_index;
                    }
                    constraint_square_sum[slot] += value * value;
                }
                for (std::size_t diagnostic = 0;
                     diagnostic < constraint_components.size(); ++diagnostic)
                {
                    const double value =
                        constraints(point, constraint_components[diagnostic]);
                    constraint_cosine_sum[diagnostic]
                                         [static_cast<std::size_t>(
                                             radial_index)] += value * cosine;
                    constraint_sine_sum[diagnostic]
                                       [static_cast<std::size_t>(
                                           radial_index)] += value * sine;
                    secondary_constraint_cosine_sum[diagnostic]
                                                       [static_cast<std::size_t>(
                                                           radial_index)] +=
                        value * secondary_cosine;
                    secondary_constraint_sine_sum[diagnostic]
                                                     [static_cast<std::size_t>(
                                                         radial_index)] +=
                        value * secondary_sine;
                }

                const double hxx = state(point, c_hxx);
                const double hxz = state(point, c_hxz);
                const double hzz = state(point, c_hzz);
                const double block_determinant = hxx * hzz - hxz * hxz;
                const double determinant =
                    block_determinant * hww * hww;
                const double determinant_error = std::abs(determinant - 1.0);
                if (determinant_error > sample.determinant_error)
                {
                    sample.determinant_error = determinant_error;
                    sample.determinant_maximum_radial = radial_index;
                }
                determinant_square_sum +=
                    determinant_error * determinant_error;

                double weighted_trace =
                    std::numeric_limits<double>::infinity();
                if (block_determinant != 0.0 && hww != 0.0)
                {
                    weighted_trace =
                        (hzz * state(point, c_Axx) -
                         2.0 * hxz * state(point, c_Axz) +
                         hxx * state(point, c_Azz)) /
                            block_determinant +
                        2.0 * state(point, c_Aww) / hww;
                }
                const double weighted_trace_error =
                    std::abs(weighted_trace);
                m_finite =
                    m_finite && std::isfinite(determinant_error) &&
                    std::isfinite(weighted_trace_error);
                if (weighted_trace_error > sample.weighted_trace_error)
                {
                    sample.weighted_trace_error = weighted_trace_error;
                    sample.weighted_trace_maximum_radial = radial_index;
                }
                weighted_trace_square_sum +=
                    weighted_trace_error * weighted_trace_error;
                ++valid_points;
            }
        }
        require(valid_points ==
                    static_cast<std::size_t>(radial_cells * compact_cells),
                "control diagnostic did not visit every valid cell");
        for (int component = 0; component < NUM_VARS; ++component)
        {
            const auto slot = static_cast<std::size_t>(component);
            sample.variable_l2[slot] =
                std::sqrt(sample.variable_l2[slot] /
                          static_cast<double>(valid_points));
        }
        for (int component = 0; component < NUM_DIAGNOSTIC_VARS; ++component)
        {
            const auto slot = static_cast<std::size_t>(component);
            sample.constraint_l2[slot] =
                std::sqrt(constraint_square_sum[slot] /
                          static_cast<double>(valid_points));
        }
        sample.state_l2 =
            std::sqrt(state_square_sum /
                      static_cast<double>(valid_points * NUM_VARS));
        sample.determinant_l2 =
            std::sqrt(determinant_square_sum /
                      static_cast<double>(valid_points));
        sample.weighted_trace_l2 =
            std::sqrt(weighted_trace_square_sum /
                      static_cast<double>(valid_points));
        m_maximum_state_drift =
            std::max(m_maximum_state_drift, sample.state_drift);

        double cosine_square_sum = 0.0;
        double sine_square_sum = 0.0;
        double cosine_sign_sum = 0.0;
        double sine_sign_sum = 0.0;
        int analysis_count = 0;
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            const double x =
                BlackStringCoordinates::cell_centered<double>(
                    domain.smallEnd(0) + radial, m_dx, offset[0]);
            if (x < analysis_x_min || x > analysis_x_max)
            {
                continue;
            }
            const int count =
                compact_count[static_cast<std::size_t>(radial)];
            require(count == compact_cells,
                    "Fourier sampler did not visit every compact cell");
            const double cosine_coefficient =
                2.0 * cosine_sum[static_cast<std::size_t>(radial)] /
                static_cast<double>(count);
            const double sine_coefficient =
                2.0 * sine_sum[static_cast<std::size_t>(radial)] /
                static_cast<double>(count);
            const double secondary_cosine_coefficient =
                2.0 *
                secondary_cosine_sum[static_cast<std::size_t>(radial)] /
                static_cast<double>(count);
            const double secondary_sine_coefficient =
                2.0 * secondary_sine_sum[static_cast<std::size_t>(radial)] /
                static_cast<double>(count);
            Sample::RadialFourier radial_fourier;
            radial_fourier.x = x;
            radial_fourier.q_cosine = cosine_coefficient;
            radial_fourier.q_sine = sine_coefficient;
            for (std::size_t diagnostic = 0;
                 diagnostic < constraint_components.size(); ++diagnostic)
            {
                radial_fourier.constraint_cosine[diagnostic] =
                    2.0 * constraint_cosine_sum[diagnostic]
                                                [static_cast<std::size_t>(
                                                    radial)] /
                    static_cast<double>(count);
                radial_fourier.constraint_sine[diagnostic] =
                    2.0 * constraint_sine_sum[diagnostic]
                                              [static_cast<std::size_t>(
                                                  radial)] /
                    static_cast<double>(count);
            }
            sample.radial_fourier.push_back(radial_fourier);
            Sample::RadialFourier secondary_radial_fourier;
            secondary_radial_fourier.x = x;
            secondary_radial_fourier.q_cosine =
                secondary_cosine_coefficient;
            secondary_radial_fourier.q_sine = secondary_sine_coefficient;
            for (std::size_t diagnostic = 0;
                 diagnostic < constraint_components.size(); ++diagnostic)
            {
                secondary_radial_fourier.constraint_cosine[diagnostic] =
                    2.0 *
                    secondary_constraint_cosine_sum[diagnostic]
                                                       [static_cast<std::size_t>(
                                                           radial)] /
                    static_cast<double>(count);
                secondary_radial_fourier.constraint_sine[diagnostic] =
                    2.0 *
                    secondary_constraint_sine_sum[diagnostic]
                                                     [static_cast<std::size_t>(
                                                         radial)] /
                    static_cast<double>(count);
            }
            sample.secondary_radial_fourier.push_back(
                secondary_radial_fourier);
            cosine_square_sum += cosine_coefficient * cosine_coefficient;
            sine_square_sum += sine_coefficient * sine_coefficient;
            cosine_sign_sum += cosine_coefficient;
            sine_sign_sum += sine_coefficient;
            ++analysis_count;
        }
        for (int component = 0; component < NUM_VARS; ++component)
        {
            const auto slot = static_cast<std::size_t>(component);
            double fourier_square_sum = 0.0;
            double radial_nyquist_sum = 0.0;
            for (int radial = 0; radial < radial_cells; ++radial)
            {
                const auto radial_slot = static_cast<std::size_t>(radial);
                const int count = compact_count[radial_slot];
                require(count == compact_cells,
                        "z-independence sampler missed compact cells");
                const double cosine_coefficient =
                    2.0 * variable_cosine_sum[slot][radial_slot] /
                    static_cast<double>(count);
                const double sine_coefficient =
                    2.0 * variable_sine_sum[slot][radial_slot] /
                    static_cast<double>(count);
                fourier_square_sum +=
                    cosine_coefficient * cosine_coefficient +
                    sine_coefficient * sine_coefficient;
                const double radial_mean_difference =
                    variable_radial_difference_sum[slot][radial_slot] /
                    static_cast<double>(count);
                radial_nyquist_sum +=
                    (radial % 2 == 0 ? 1.0 : -1.0) *
                    radial_mean_difference;
                sample.variable_z_span[slot] =
                    std::max(sample.variable_z_span[slot],
                             variable_maximum[slot][radial_slot] -
                                 variable_minimum[slot][radial_slot]);
            }
            sample.variable_fourier_amplitude[slot] =
                std::sqrt(fourier_square_sum /
                          static_cast<double>(radial_cells));
            sample.variable_radial_nyquist[slot] =
                std::abs(radial_nyquist_sum) /
                static_cast<double>(radial_cells);
        }
        require(analysis_count > 0,
                "Fourier analysis window contains no cells");
        sample.cosine = std::copysign(
            std::sqrt(cosine_square_sum /
                      static_cast<double>(analysis_count)),
            cosine_sign_sum);
        sample.sine = std::copysign(
            std::sqrt(sine_square_sum / static_cast<double>(analysis_count)),
            sine_sign_sum);
        sample.amplitude = std::hypot(sample.cosine, sample.sine);
        sample.phase = std::atan2(sample.sine, sample.cosine);
        for (const double angle : {0.37, 1.10})
        {
            const double rotated_cosine =
                sample.cosine * std::cos(angle) -
                sample.sine * std::sin(angle);
            const double rotated_sine =
                sample.cosine * std::sin(angle) +
                sample.sine * std::cos(angle);
            const double rotated_amplitude =
                std::hypot(rotated_cosine, rotated_sine);
            const double scale =
                std::max(sample.amplitude,
                         std::numeric_limits<double>::min());
            m_maximum_phase_rotation_error =
                std::max(m_maximum_phase_rotation_error,
                         std::abs(rotated_amplitude - sample.amplitude) /
                             scale);
        }
        if (d9_live_variant != nullptr)
        {
            print_d9_sample(d9_live_variant, sample, radial_cells);
            std::cout.flush();
        }
        m_samples.push_back(sample);
    }

    std::vector<Sample> m_samples;
    double m_maximum_state_drift = 0.0;
    double m_maximum_phase_rotation_error = 0.0;
    bool m_finite = true;
};

struct D8MetricSnapshot
{
    double hxx = 0.0;
    double hxz = 0.0;
    double hzz = 0.0;
    double hww = 0.0;
    double principal_1 = 0.0;
    double principal_2 = 0.0;
    double principal_3 = 0.0;
    double determinant = 0.0;
};

D8MetricSnapshot d8_metric_snapshot(const FArrayBox &state,
                                    const IntVect &point)
{
    D8MetricSnapshot result;
    result.hxx = state(point, c_hxx);
    result.hxz = state(point, c_hxz);
    result.hzz = state(point, c_hzz);
    result.hww = state(point, c_hww);
    result.principal_1 = result.hxx;
    result.principal_2 =
        result.hxx * result.hzz - result.hxz * result.hxz;
    result.principal_3 = result.principal_2 * result.hww;
    result.determinant = result.principal_3 * result.hww;
    return result;
}

bool d8_admissible_metric(const D8MetricSnapshot &metric)
{
    return std::isfinite(metric.hxx) && std::isfinite(metric.hxz) &&
           std::isfinite(metric.hzz) && std::isfinite(metric.hww) &&
           metric.principal_1 > 0.0 && metric.principal_2 > 0.0 &&
           metric.principal_3 > 0.0 && metric.determinant > 0.0 &&
           std::isfinite(metric.determinant);
}

const char *d8_offending_metric_field(const D8MetricSnapshot &metric)
{
    if (!std::isfinite(metric.hxx))
        return "hxx_nonfinite";
    if (!std::isfinite(metric.hxz))
        return "hxz_nonfinite";
    if (!std::isfinite(metric.hzz))
        return "hzz_nonfinite";
    if (!std::isfinite(metric.hww))
        return "hww_nonfinite";
    if (!(metric.principal_1 > 0.0))
        return "hxx_principal_minor";
    if (!(metric.principal_2 > 0.0))
        return "visible_2x2_principal_minor";
    if (!(metric.principal_3 > 0.0))
        return "first_hidden_principal_minor";
    return "determinant";
}

void d8_print_metric(const char *label, const D8MetricSnapshot &metric)
{
    std::cout << std::scientific << std::setprecision(12)
              << "D8_METRIC label=" << label << " hxx=" << metric.hxx
              << " hxz=" << metric.hxz << " hzz=" << metric.hzz
              << " hww=" << metric.hww
              << " principal_1=" << metric.principal_1
              << " principal_2=" << metric.principal_2
              << " principal_3=" << metric.principal_3
              << " determinant=" << metric.determinant << '\n';
}

const char *d8_point_region(const IntVect &point, const Box &domain)
{
    const bool radial_low = point[0] < domain.smallEnd(0);
    const bool radial_high = point[0] > domain.bigEnd(0);
    const bool compact_ghost =
        point[1] < domain.smallEnd(1) || point[1] > domain.bigEnd(1);
    if ((radial_low || radial_high) && compact_ghost)
        return "radial_periodic_corner";
    if (radial_low)
        return "inner_radial_ghost";
    if (radial_high)
        return "outer_radial_ghost";
    if (compact_ghost)
        return "pure_z_ghost";
    const int radial = point[0] - domain.smallEnd(0);
    return maximum_region(radial, domain.size(0));
}

template <int mode_number>
class D8ExactGPAbortLevel : public FourierGrowthLevel<mode_number>
{
    friend class DefaultLevelFactory<D8ExactGPAbortLevel<mode_number>>;
    using Base = FourierGrowthLevel<mode_number>;

  protected:
    using Base::Base;

    void fillBdyGhosts(
        GRLevelData &state,
        const Interval &components = Interval(0, NUM_VARS - 1)) override
    {
        const Box domain = this->m_problem_domain.domainBox();
        ObjectAudit &object = audit_for(state, domain);
        std::vector<ValidState> valid_before;
        std::size_t invalid_before = 0;
        std::size_t invalid_after = 0;
        std::size_t radial_ghost_cells = 0;
        std::size_t radial_corner_cells = 0;
        std::size_t pure_z_ghost_cells = 0;
        double maximum_radial_gp_error = 0.0;

        const DataIterator iterator = state.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = state[data_index];
            const Box valid = this->m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                ValidState saved;
                saved.point = bit();
                for (int component = 0; component < NUM_VARS; ++component)
                    saved.values[static_cast<std::size_t>(component)] =
                        fab(saved.point, component);
                valid_before.push_back(saved);
                object.before[valid_index(saved.point, domain)] =
                    d8_metric_snapshot(fab, saved.point);
            }
            for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
            {
                if (!d8_admissible_metric(d8_metric_snapshot(fab, bit())))
                    ++invalid_before;
            }
        }

        BlackStringToyLevel::fillBdyGhosts(state, components);

        std::size_t valid_cursor = 0;
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = state[data_index];
            const Box valid = this->m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const ValidState &saved = valid_before[valid_cursor++];
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    if (fab(saved.point, component) !=
                        saved.values[static_cast<std::size_t>(component)])
                        object.valid_overwritten = true;
                }
                object.after[valid_index(saved.point, domain)] =
                    d8_metric_snapshot(fab, saved.point);
            }
            for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const auto metric = d8_metric_snapshot(fab, point);
                if (!d8_admissible_metric(metric))
                    ++invalid_after;
                if (domain.contains(point))
                    continue;
                const bool radial_ghost =
                    point[0] < domain.smallEnd(0) ||
                    point[0] > domain.bigEnd(0);
                const bool compact_ghost =
                    point[1] < domain.smallEnd(1) ||
                    point[1] > domain.bigEnd(1);
                if (!radial_ghost)
                {
                    ++pure_z_ghost_cells;
                    continue;
                }
                if (compact_ghost)
                    ++radial_corner_cells;
                else
                    ++radial_ghost_cells;
                const double x =
                    BlackStringCoordinates::cell_centered<double>(
                        point[0], this->m_dx,
                        this->m_p.coordinate_offset()[0]);
                const auto background =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        this->m_p.r0, x);
                for (int component = 0; component < NUM_VARS; ++component)
                    maximum_radial_gp_error =
                        std::max(maximum_radial_gp_error,
                                 std::abs(fab(point, component) -
                                          background[static_cast<std::size_t>(
                                              component)]));
            }
        }
        ++m_ghost_fill_calls;
        object.has_fill = true;
        object.invalid_before = invalid_before;
        object.invalid_after = invalid_after;
        object.maximum_radial_gp_error = maximum_radial_gp_error;
        if (m_ghost_fill_calls == 1)
        {
            std::cout << "D8_GHOST_AUDIT fill=1"
                      << " components_begin=" << components.begin()
                      << " components_end=" << components.end()
                      << " invalid_metric_cells_before=" << invalid_before
                      << " invalid_metric_cells_after=" << invalid_after
                      << " radial_ghost_cells=" << radial_ghost_cells
                      << " radial_corner_cells=" << radial_corner_cells
                      << " pure_z_ghost_cells=" << pure_z_ghost_cells
                      << " valid_overwritten="
                      << (object.valid_overwritten ? 1 : 0)
                      << " maximum_radial_gp_error="
                      << maximum_radial_gp_error
                      << " ordering=chombo_exchange_then_radial_fill\n";
        }
    }

  private:
    struct ValidState
    {
        IntVect point;
        std::array<double, NUM_VARS> values{};
    };

    struct ObjectAudit
    {
        const GRLevelData *identity = nullptr;
        std::vector<D8MetricSnapshot> before;
        std::vector<D8MetricSnapshot> after;
        bool has_fill = false;
        bool valid_overwritten = false;
        std::size_t invalid_before = 0;
        std::size_t invalid_after = 0;
        double maximum_radial_gp_error = 0.0;
    };

    static std::size_t valid_index(const IntVect &point, const Box &domain)
    {
        const int compact = point[1] - domain.smallEnd(1);
        const int radial = point[0] - domain.smallEnd(0);
        return static_cast<std::size_t>(radial * domain.size(1) + compact);
    }

    ObjectAudit &audit_for(GRLevelData &state, const Box &domain)
    {
        for (auto &audit : m_object_audits)
            if (audit.identity == &state)
                return audit;
        ObjectAudit audit;
        audit.identity = &state;
        const auto cells =
            static_cast<std::size_t>(domain.size(0) * domain.size(1));
        audit.before.resize(cells);
        audit.after.resize(cells);
        m_object_audits.push_back(audit);
        return m_object_audits.back();
    }

    const ObjectAudit *find_audit(const GRLevelData &state) const
    {
        for (const auto &audit : m_object_audits)
            if (audit.identity == &state)
                return &audit;
        return nullptr;
    }

    void specificUpdateODE(GRLevelData &solution, const GRLevelData &rhs,
                           const Real dt) override
    {
        this->instrumentation().record_update();
        (void)rhs;
        ++m_update_calls;
        const Box domain = this->m_problem_domain.domainBox();
        const DataIterator iterator = solution.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = solution[data_index];
            const Box valid = this->m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const auto failed = d8_metric_snapshot(fab, point);
                if (d8_admissible_metric(failed))
                    continue;

                const std::size_t update_in_step =
                    (m_update_calls - 1) % updates_per_step + 1;
                const std::size_t timestep =
                    (m_update_calls - 1) / updates_per_step + 1;
                const int rk_stage =
                    update_in_step <= 2
                        ? 1
                        : (update_in_step <= 4
                               ? 2
                               : (update_in_step <= 6 ? 3 : 4));
                const char *target =
                    update_in_step == 2 || update_in_step == 4 ||
                            update_in_step == 6
                        ? "stage_predictor"
                        : "new_solution_accumulator";
                std::cout << "D8_EXACT_GP_ABORT"
                          << " first_failing_timestep=" << timestep
                          << " rk_stage=" << rk_stage
                          << " update_in_step=" << update_in_step
                          << " update_target=" << target
                          << " base_time=" << this->m_time
                          << " full_dt=" << this->m_dt
                          << " update_dt=" << dt << '\n'
                          << "D8_INVALID_CELL index_x=" << point[0]
                          << " index_z=" << point[1]
                          << " valid_cell=1 ghost_cell=0 region="
                          << d8_point_region(point, domain)
                          << " offending_field="
                          << d8_offending_metric_field(failed) << '\n';
                const ObjectAudit *audit = find_audit(solution);
                if (audit != nullptr && audit->has_fill)
                {
                    const auto index = valid_index(point, domain);
                    d8_print_metric("immediately_before_last_ghost_fill",
                                    audit->before[index]);
                    d8_print_metric("immediately_after_last_ghost_fill",
                                    audit->after[index]);
                    std::cout
                        << "D8_GHOST_STATE_AT_ABORT"
                        << " invalid_before=" << audit->invalid_before
                        << " invalid_after=" << audit->invalid_after
                        << " valid_overwritten="
                        << (audit->valid_overwritten ? 1 : 0)
                        << " maximum_radial_gp_error="
                        << audit->maximum_radial_gp_error << '\n';
                }
                d8_print_metric("after_rk_update_before_cleanup", failed);
                std::cout.flush();
                std::exit(86);
            }
        }
        BoxLoops::loop(
            BlackStringLive::CleanupCompute(this->m_p.min_chi,
                                            this->m_p.min_lapse),
            solution, solution, EXCLUDE_GHOST_CELLS, disable_simd());
        this->instrumentation().record_cleanup();
    }

    static constexpr std::size_t updates_per_step = 7;
    std::vector<ObjectAudit> m_object_audits;
    std::size_t m_ghost_fill_calls = 0;
    std::size_t m_update_calls = 0;
};

struct D8FrozenGaugePreStorePolicy
{
    void operator()(BlackStringReducedVars::Variables<double> &rhs) const
    {
        rhs.gauge.lapse = 0.0;
        rhs.gauge.shift = {0.0, 0.0};
        rhs.gauge.B = {0.0, 0.0};
    }
};

template <int mode_number>
class D8FrozenGaugeLevel : public FourierGrowthLevel<mode_number>
{
    friend class DefaultLevelFactory<D8FrozenGaugeLevel<mode_number>>;
    using Base = FourierGrowthLevel<mode_number>;

  protected:
    using Base::Base;

  private:
    void specificEvalRHS(GRLevelData &solution, GRLevelData &rhs,
                         const double time) override
    {
        this->instrumentation().record_rhs_after_framework_refresh();
        (void)time;
        using FrozenCompute = BlackStringLive::BasicRHSCompute<
            BlackStringLive::DefaultInputPolicy,
            BlackStringLive::DefaultEvaluationPolicy,
            D8FrozenGaugePreStorePolicy>;
        BoxLoops::loop(
            FrozenCompute(this->m_p.r0, this->m_dx,
                          this->m_p.coordinate_offset(), this->m_p.gauge,
                          this->m_p.fixed_lapse_source, this->m_p.ko_sigma),
            solution, rhs, EXCLUDE_GHOST_CELLS, disable_simd());
        if (this->m_p.physical_radial_boundaries)
        {
            BlackStringPerturbativeRadialBoundary::apply_outer_rhs(
                solution, rhs, this->m_problem_domain, this->m_p.r0,
                this->m_dx, this->m_p.coordinate_offset(),
                this->m_p.outer_sommerfeld_speed,
                Interval(0, c_GammaZ));
            this->instrumentation().record_outer_radiative_rhs();
        }
    }
};

template <int mode_number>
class D9ControlLevel : public FourierGrowthLevel<mode_number>
{
    friend class DefaultLevelFactory<D9ControlLevel<mode_number>>;
    using Base = FourierGrowthLevel<mode_number>;

  protected:
    using Base::Base;

    void fillBdyGhosts(
        GRLevelData &state,
        const Interval &components = Interval(0, NUM_VARS - 1)) override
    {
        if (m_ghost_audit_complete)
        {
            BlackStringToyLevel::fillBdyGhosts(state, components);
            return;
        }

        require(components.begin() == 0 && components.end() == NUM_VARS - 1,
                "D9 first ghost audit must preserve all 18 stored slots");
        const Box domain = this->m_problem_domain.domainBox();
        std::vector<ValidState> valid_before;
        const DataIterator iterator = state.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = state[data_index];
            const Box valid = this->m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                ValidState saved;
                saved.point = bit();
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    saved.values[static_cast<std::size_t>(component)] =
                        fab(saved.point, component);
                }
                valid_before.push_back(saved);
            }
        }

        BlackStringToyLevel::fillBdyGhosts(state, components);

        std::size_t valid_cursor = 0;
        std::size_t radial_ghost_cells = 0;
        std::size_t radial_corner_cells = 0;
        std::size_t pure_z_ghost_cells = 0;
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = state[data_index];
            const Box valid = this->m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const ValidState &saved = valid_before[valid_cursor++];
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    if (fab(saved.point, component) !=
                        saved.values[static_cast<std::size_t>(component)])
                    {
                        m_valid_cells_unchanged = false;
                    }
                }
            }
            for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                if (domain.contains(point))
                {
                    continue;
                }
                const bool radial_ghost =
                    point[0] < domain.smallEnd(0) ||
                    point[0] > domain.bigEnd(0);
                const bool compact_ghost =
                    point[1] < domain.smallEnd(1) ||
                    point[1] > domain.bigEnd(1);
                if (!radial_ghost)
                {
                    ++pure_z_ghost_cells;
                }
                else if (compact_ghost)
                {
                    ++radial_corner_cells;
                }
                else
                {
                    ++radial_ghost_cells;
                }
            }
        }
        require(valid_cursor == valid_before.size(),
                "D9 ghost audit did not revisit every valid cell");

        const auto offset = this->m_p.coordinate_offset();
        for (int layer = 1; layer <= 3; ++layer)
        {
            m_inner_ghost_coordinates[static_cast<std::size_t>(layer - 1)] =
                BlackStringCoordinates::cell_centered<double>(
                    domain.smallEnd(0) - layer, this->m_dx, offset[0]);
        }
        m_minimum_inner_ghost_x =
            *std::min_element(m_inner_ghost_coordinates.begin(),
                              m_inner_ghost_coordinates.end());
        m_ghost_audit_complete = true;
        std::cout << std::scientific << std::setprecision(12)
                  << "D9_GHOST_AUDIT variant=" << d9_live_variant
                  << " components_begin=" << components.begin()
                  << " components_end=" << components.end()
                  << " radial_ghost_cells=" << radial_ghost_cells
                  << " radial_corner_cells=" << radial_corner_cells
                  << " pure_z_ghost_cells=" << pure_z_ghost_cells
                  << " valid_cells_overwritten="
                  << (m_valid_cells_unchanged ? 0 : 1)
                  << " inner_ghost_x_near=" << m_inner_ghost_coordinates[0]
                  << " inner_ghost_x_middle="
                  << m_inner_ghost_coordinates[1]
                  << " inner_ghost_x_far=" << m_inner_ghost_coordinates[2]
                  << " minimum_inner_ghost_x=" << m_minimum_inner_ghost_x
                  << " ordering=chombo_exchange_then_radial_fill\n";
    }

    void specificUpdateODE(GRLevelData &solution, const GRLevelData &rhs,
                           const Real dt) override
    {
        this->instrumentation().record_update();
        (void)rhs;
        ++m_update_calls;
        const Box domain = this->m_problem_domain.domainBox();
        const DataIterator iterator = solution.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = solution[data_index];
            const Box valid = this->m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const auto metric = d8_metric_snapshot(fab, point);
                if (d8_admissible_metric(metric))
                {
                    continue;
                }
                const std::size_t update_in_step =
                    (m_update_calls - 1) % updates_per_step + 1;
                const std::size_t timestep =
                    (m_update_calls - 1) / updates_per_step + 1;
                const int rk_stage =
                    update_in_step <= 2
                        ? 1
                        : (update_in_step <= 4
                               ? 2
                               : (update_in_step <= 6 ? 3 : 4));
                std::cout << "D9_INVALID_METRIC variant=" << d9_live_variant
                          << " first_failing_timestep=" << timestep
                          << " rk_stage=" << rk_stage
                          << " update_in_step=" << update_in_step
                          << " base_time=" << this->m_time
                          << " full_dt=" << this->m_dt
                          << " update_dt=" << dt
                          << " index_x=" << point[0]
                          << " index_z=" << point[1]
                          << " valid_cell=1 ghost_cell=0 region="
                          << d8_point_region(point, domain)
                          << " offending_field="
                          << d8_offending_metric_field(metric) << '\n';
                d8_print_metric("d9_after_rk_update_before_cleanup", metric);
                std::cout.flush();
                std::exit(87);
            }
        }
        BoxLoops::loop(
            BlackStringLive::CleanupCompute(this->m_p.min_chi,
                                            this->m_p.min_lapse),
            solution, solution, EXCLUDE_GHOST_CELLS, disable_simd());
        this->instrumentation().record_cleanup();
    }

  public:
    bool d9_ghost_audit_complete() const { return m_ghost_audit_complete; }
    bool d9_valid_cells_unchanged() const
    {
        return m_valid_cells_unchanged;
    }
    double d9_minimum_inner_ghost_x() const
    {
        return m_minimum_inner_ghost_x;
    }
    std::array<double, 3> d9_inner_ghost_coordinates() const
    {
        return m_inner_ghost_coordinates;
    }

  private:
    struct ValidState
    {
        IntVect point;
        std::array<double, NUM_VARS> values{};
    };

    static constexpr std::size_t updates_per_step = 7;
    bool m_ghost_audit_complete = false;
    bool m_valid_cells_unchanged = true;
    double m_minimum_inner_ghost_x =
        std::numeric_limits<double>::infinity();
    std::array<double, 3> m_inner_ghost_coordinates{};
    std::size_t m_update_calls = 0;
};

template <int mode_number,
          template <int> class level_template = FourierGrowthLevel>
int run_case(SimulationParameters &parameters, const char *mode_name)
{
    constexpr int secondary_mode_number = mode_number == 1 ? 2 : 1;
    const std::string mode(mode_name);
    d9_live_variant =
        mode.rfind("d9_", 0) == 0 || mode.rfind("d11_", 0) == 0
            ? mode_name
            : nullptr;
    const bool exact_gp_control =
        mode == "d7_exact_gp" || mode == "d8_exact_gp" ||
        mode == "d8_exact_abort" || mode == "d8_combined" ||
        mode == "d9_exact_gp";
    if (exact_gp_control)
    {
        require(parameters.background_preserving_gp_radial_ghosts &&
                    !parameters.physical_radial_boundaries,
                "D7 exact-GP control requires only exact-GP radial ghosts");
    }
    else
    {
        require(parameters.physical_radial_boundaries,
                "physical radial boundary policy must be enabled");
        require(!parameters.background_preserving_gp_radial_ghosts,
                "exact-GP diagnostic radial policy must be disabled");
    }
    require(parameters.constraint_diagnostic_cadence > 0,
            "growth fixture requires an explicit positive diagnostic cadence");
    require(parameters.max_level == 0,
            "growth fixture must remain serial level zero");

    GRAMR amr;
    using Level = level_template<mode_number>;
    DefaultLevelFactory<Level> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    require(levels.size() == 1, "growth fixture must create one level");
    auto *level = dynamic_cast<Level *>(levels[0]);
    require(level != nullptr, "growth fixture factory returned wrong level");
    const SeedReport seed = level->inspect_gamma_z_seed();
    const double epsilon_magnitude = std::abs(run_configuration.epsilon);
    const double seed_tolerance =
        std::max(1.0e-24, epsilon_magnitude * 1.0e-10);
    require(seed.legacy_gamma_z_maximum_difference >
                epsilon_magnitude * 1.0e-3 ||
                !run_configuration.seeded(),
            "Gamma-z seed audit does not distinguish the legacy mutation");
    require(seed.corrected_gamma_z_maximum_error <= seed_tolerance,
            "corrected Gamma-z seed check failed");

    ParityReport parity;
    if (run_configuration.seeded())
    {
        parity = level->inspect_initial_parity();
        require(parity.even_cosine > 1.0e-4,
                "even Fourier seed has no cosine-sector support");
        require(parity.one_z_sine > 1.0e-4,
                "one-z Fourier seed has no sine-sector support");
        require(parity.even_sine_leakage < 1.0e-5,
                "even Fourier seed leaked into the sine sector");
        require(parity.one_z_cosine_leakage < 1.0e-5,
                "one-z Fourier seed leaked into the cosine sector");
    }
    level->record_initial_sample();

    amr.run(parameters.stop_time, parameters.max_steps);
    if (level->samples().empty() ||
        std::abs(level->samples().back().time - parameters.stop_time) >
            0.5 * parameters.coarsest_dx * parameters.dt_multiplier)
    {
        level->record_initial_sample();
    }

    const auto &counts = level->instrumentation_report();
    require(level->finite(), "growth evolution or diagnostics became nonfinite");
    require(level->maximum_phase_rotation_error() < 1.0e-14,
            "phase-neutral amplitude changed under quadrature rotation");
    require(counts.low_radial_fills == counts.ghost_fill_calls &&
                counts.high_radial_fills == counts.ghost_fill_calls,
            "radial ghosts were not filled exactly once per refresh");
    require(parameters.physical_radial_boundaries
                ? counts.outer_radiative_rhs_calls == counts.rhs_calls
                : counts.outer_radiative_rhs_calls == 0,
            "outer radial RHS count disagrees with boundary policy");
    require(counts.rhs_periodic_exchanges == counts.rhs_calls,
            "framework periodic exchange count differs from live RHS count");
    require(counts.periodic_exchanges ==
                counts.fillall_periodic_exchanges +
                    counts.rhs_periodic_exchanges,
            "radial policy performed a duplicate periodic exchange");
    require(counts.diagnostic_evaluations == level->samples().size(),
            "diagnostic cadence count differs from collected samples");
    if (mode.rfind("d9_", 0) == 0 || mode.rfind("d11_", 0) == 0)
    {
        require(level->d9_ghost_audit_complete(),
                "D9 did not audit its first radial ghost fill");
        require(level->d9_valid_cells_unchanged(),
                "D9 radial ghost fill overwrote a valid cell");
        require(level->d9_minimum_inner_ghost_x() > 0.0,
                "D9 radial ghost coordinate is not strictly positive");
        const auto ghost_x = level->d9_inner_ghost_coordinates();
        std::cout << "D9_GHOST_CHECK variant=" << mode_name
                  << " valid_cells_overwritten=0"
                  << " inner_ghost_x_near=" << ghost_x[0]
                  << " inner_ghost_x_middle=" << ghost_x[1]
                  << " inner_ghost_x_far=" << ghost_x[2]
                  << " minimum_inner_ghost_x="
                  << level->d9_minimum_inner_ghost_x()
                  << " all_positive=1 all_18_slots_preserved=1\n";
    }
    if (mode == "d8_frozen_gauge" || mode == "d8_combined")
    {
        constexpr double frozen_tolerance = 1.0e-14;
        for (const int component :
             {c_lapse, c_shiftX, c_shiftZ, c_Bx, c_Bz})
        {
            require(level->maximum_component_drift(component) <=
                        frozen_tolerance,
                    "D8 frozen-gauge slot changed during evolution");
        }
        std::cout << "D8_FROZEN_GAUGE_CHECK"
                  << " lapse_max="
                  << level->maximum_component_drift(c_lapse)
                  << " shift_x_max="
                  << level->maximum_component_drift(c_shiftX)
                  << " shift_z_max="
                  << level->maximum_component_drift(c_shiftZ)
                  << " B_x_max=" << level->maximum_component_drift(c_Bx)
                  << " B_z_max=" << level->maximum_component_drift(c_Bz)
                  << " tolerance=" << frozen_tolerance
                  << " theta_frozen=0 gamma_frozen=0\n";
    }

    const int radial_cells = level->radial_cells();
    const int compact_cells = level->compact_cells();
    const double compact_length = level->spacing() * compact_cells;
    const double x_out =
        parameters.coordinate_minimum[0] +
        level->spacing() * radial_cells;
    const double k = 2.0 * pi * mode_number / compact_length;
    const char *kind_name =
        run_configuration.kind == RunKind::control ? "control" : "seeded";
    std::cout << std::scientific << std::setprecision(12)
              << "FOURIER_CASE mode=" << mode_name
              << " kind=" << kind_name
              << " mode_number=" << mode_number << " k=" << k
              << " x_in=" << parameters.coordinate_minimum[0]
              << " x_out=" << x_out << " Lz=" << compact_length
              << " Nx=" << radial_cells << " Nz=" << compact_cells
              << " dx=" << level->spacing()
              << " dz=" << level->spacing()
              << " dt="
              << level->spacing() * parameters.dt_multiplier
              << " CFL=" << parameters.dt_multiplier
              << " final_time=" << level->samples().back().time
              << " epsilon=" << run_configuration.epsilon
              << " ko_sigma=" << parameters.ko_sigma
              << " profile_center=" << profile_center
              << " profile_half_width=" << profile_half_width << '\n';
    std::cout << "FOURIER_SEED_CHECK mode=" << mode_name
              << " kind=" << kind_name << " x_out=" << x_out
              << " Nx=" << radial_cells
              << " corrected_gamma_z_maximum_error="
              << seed.corrected_gamma_z_maximum_error
              << " legacy_gamma_z_maximum_difference="
              << seed.legacy_gamma_z_maximum_difference
              << " derivative_coefficient="
              << corrected_gamma_z_derivative_coefficient << '\n';
    if (run_configuration.seeded())
    {
        std::cout << "FOURIER_PARITY mode=" << mode_name
                  << " kind=" << kind_name << " x_out=" << x_out
                  << " Nx=" << radial_cells
                  << " even_cosine=" << parity.even_cosine
                  << " even_sine_leakage=" << parity.even_sine_leakage
                  << " one_z_sine=" << parity.one_z_sine
                  << " one_z_cosine_leakage="
                  << parity.one_z_cosine_leakage << '\n';
    }
    for (const auto &sample : level->samples())
    {
        const double normalized_amplitude =
            run_configuration.seeded()
                ? sample.amplitude / epsilon_magnitude
                : sample.amplitude;
        std::cout << "FOURIER_SAMPLE mode=" << mode_name
                  << " kind=" << kind_name
                  << " x_out=" << x_out << " Nx=" << radial_cells
                  << " epsilon=" << run_configuration.epsilon
                  << " t=" << sample.time << " Ck=" << sample.cosine
                  << " Sk=" << sample.sine
                  << " amplitude=" << sample.amplitude
                  << " normalized_amplitude=" << normalized_amplitude
                  << " phase=" << sample.phase
                  << " H=" << sample.constraints[c_Ham]
                  << " Mx=" << sample.constraints[c_MomX]
                  << " Mz=" << sample.constraints[c_MomZ]
                  << " state_drift=" << sample.state_drift << '\n';
        const std::string diagnostic_prefix =
            std::string(mode_name).rfind("d8_", 0) == 0 ? "D8" : "D7";
        if (diagnostic_prefix == "D8" ||
            std::string(mode_name).rfind("d7_", 0) == 0)
        {
            const auto maximum_variable =
                static_cast<int>(std::distance(
                    sample.variable_maximum.begin(),
                    std::max_element(sample.variable_maximum.begin(),
                                     sample.variable_maximum.end())));
            const auto maximum_z_variable =
                static_cast<int>(std::distance(
                    sample.variable_z_span.begin(),
                    std::max_element(sample.variable_z_span.begin(),
                                     sample.variable_z_span.end())));
            const auto maximum_fourier_variable =
                static_cast<int>(std::distance(
                    sample.variable_fourier_amplitude.begin(),
                    std::max_element(
                        sample.variable_fourier_amplitude.begin(),
                        sample.variable_fourier_amplitude.end())));
            std::cout
                << diagnostic_prefix << "_SAMPLE variant=" << mode_name
                << " t=" << sample.time
                << " state_max=" << sample.state_drift
                << " state_l2=" << sample.state_l2
                << " state_max_variable="
                << UserVariables::variable_names[static_cast<std::size_t>(
                       maximum_variable)]
                << " state_max_location="
                << maximum_region(
                       sample.variable_maximum_radial
                           [static_cast<std::size_t>(maximum_variable)],
                       radial_cells)
                << " H_max=" << sample.constraints[c_Ham]
                << " H_l2=" << sample.constraint_l2[c_Ham]
                << " H_location="
                << maximum_region(
                       sample.constraint_maximum_radial[c_Ham], radial_cells)
                << " Mx_max=" << sample.constraints[c_MomX]
                << " Mx_l2=" << sample.constraint_l2[c_MomX]
                << " Mx_location="
                << maximum_region(
                       sample.constraint_maximum_radial[c_MomX], radial_cells)
                << " Mz_max=" << sample.constraints[c_MomZ]
                << " Mz_l2=" << sample.constraint_l2[c_MomZ]
                << " Mz_location="
                << maximum_region(
                       sample.constraint_maximum_radial[c_MomZ], radial_cells)
                << " determinant_max=" << sample.determinant_error
                << " determinant_l2=" << sample.determinant_l2
                << " determinant_location="
                << maximum_region(sample.determinant_maximum_radial,
                                  radial_cells)
                << " weighted_trace_max=" << sample.weighted_trace_error
                << " weighted_trace_l2=" << sample.weighted_trace_l2
                << " weighted_trace_location="
                << maximum_region(sample.weighted_trace_maximum_radial,
                                  radial_cells)
                << " maximum_z_span="
                << sample.variable_z_span[static_cast<std::size_t>(
                       maximum_z_variable)]
                << " maximum_z_span_variable="
                << UserVariables::variable_names[static_cast<std::size_t>(
                       maximum_z_variable)]
                << " maximum_fourier="
                << sample.variable_fourier_amplitude
                       [static_cast<std::size_t>(maximum_fourier_variable)]
                << " maximum_fourier_variable="
                << UserVariables::variable_names[static_cast<std::size_t>(
                       maximum_fourier_variable)]
                << '\n';
            for (int component = 0; component < NUM_VARS; ++component)
            {
                const auto slot = static_cast<std::size_t>(component);
                std::cout
                    << diagnostic_prefix << "_VARIABLE variant=" << mode_name
                    << " t=" << sample.time << " slot=" << component
                    << " name=" << UserVariables::variable_names[slot]
                    << " maximum=" << sample.variable_maximum[slot]
                    << " l2=" << sample.variable_l2[slot]
                    << " location="
                    << maximum_region(sample.variable_maximum_radial[slot],
                                      radial_cells)
                    << " z_span=" << sample.variable_z_span[slot]
                    << " fourier=" << sample.variable_fourier_amplitude[slot]
                    << '\n';
            }
        }
        if (std::string(mode_name) == "transient")
        {
            const auto print_radial =
                [&](const std::vector<Sample::RadialFourier> &values,
                    const int output_mode_number)
            {
                for (std::size_t radial = 0; radial < values.size(); ++radial)
                {
                    const auto &value = values[radial];
                    std::cout
                        << "FOURIER_RADIAL_SAMPLE mode=" << mode_name
                        << " kind=" << kind_name
                        << " mode_number=" << output_mode_number
                        << " epsilon=" << run_configuration.epsilon
                        << " t=" << sample.time << " radial_index=" << radial
                        << " x=" << value.x << " qC=" << value.q_cosine
                        << " qS=" << value.q_sine
                        << " HC=" << value.constraint_cosine[0]
                        << " HS=" << value.constraint_sine[0]
                        << " MxC=" << value.constraint_cosine[1]
                        << " MxS=" << value.constraint_sine[1]
                        << " MzC=" << value.constraint_cosine[2]
                        << " MzS=" << value.constraint_sine[2] << '\n';
                }
            };
            print_radial(sample.radial_fourier, mode_number);
            print_radial(sample.secondary_radial_fourier,
                         secondary_mode_number);
        }
    }
    if (run_configuration.seeded() &&
        std::string(mode_name) != "transient")
    {
        const bool scan_mode = std::string(mode_name) == "scan";
        const auto &selected_fit_windows =
            scan_mode && parameters.stop_time > 1.0
                ? extended_scan_fit_windows
                : (scan_mode ? adaptive_scan_fit_windows : fit_windows);
        for (const FitWindow window : selected_fit_windows)
        {
            const Fit fit = fit_growth(level->samples(), window);
            std::cout << "FOURIER_FIT mode=" << mode_name
                      << " kind=" << kind_name << " x_out=" << x_out
                      << " Nx=" << radial_cells
                      << " epsilon=" << run_configuration.epsilon
                      << " window_start=" << window.start
                      << " window_end=" << window.end
                      << " points=" << fit.points
                      << " Omega=" << fit.omega
                      << " standard_error=" << fit.standard_error
                      << " R2=" << fit.r_squared << '\n';
        }
    }
    std::cout << "FOURIER_PHASE_INVARIANCE mode=" << mode_name
              << " kind=" << kind_name << " x_out=" << x_out
              << " Nx=" << radial_cells
              << " maximum_relative_rotation_error="
              << level->maximum_phase_rotation_error() << '\n'
              << "FOURIER_DRIFT mode=" << mode_name
              << " kind=" << kind_name << " x_out=" << x_out
              << " Nx=" << radial_cells
              << " maximum_state_drift=" << level->maximum_state_drift()
              << '\n'
              << "FOURIER_COUNTS mode=" << mode_name
              << " kind=" << kind_name << " x_out=" << x_out
              << " Nx=" << radial_cells
              << " valid_cells=" << radial_cells * compact_cells
              << " rhs_evaluations=" << counts.rhs_calls
              << " periodic_exchanges=" << counts.periodic_exchanges
              << " inner_calls=" << counts.low_radial_fills
              << " outer_calls=" << counts.high_radial_fills
              << " outer_rhs_calls=" << counts.outer_radiative_rhs_calls
              << " diagnostic_evaluations="
              << counts.diagnostic_evaluations << '\n'
              << "BLACKSTRING_FOURIER_GROWTH_CASE_PASS\n";
    amr.conclude();
    return 0;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fail("usage: BlackStringFourierGrowthTest <params> "
             "<unstable|stable|scan|transient-low|transient-high|"
             "d7-exact-gp|d7-frozen-gauge|d7-half-cfl|d7-fine|"
             "d8-exact-abort|d8-exact-gp|d8-frozen-gauge|d8-combined|"
             "d9-medium|d9-fine|d9-exact-gp|"
             "d11-ref|d11-double|d11-fine> "
             "<control|seeded|legacy-gammaz> <epsilon>");
    }
    const std::string kind(argv[3]);
    char *epsilon_end = nullptr;
    const double epsilon = std::strtod(argv[4], &epsilon_end);
    require(epsilon_end != argv[4] && *epsilon_end == '\0' &&
                std::isfinite(epsilon),
            "epsilon must be a finite number");
    if (kind == "control")
    {
        require(epsilon == 0.0, "control run requires epsilon=0");
        run_configuration = {RunKind::control, 0.0};
    }
    else if (kind == "seeded")
    {
        require(epsilon != 0.0, "seeded run requires nonzero epsilon");
        run_configuration = {RunKind::seeded, epsilon};
    }
    else if (kind == "legacy-gammaz")
    {
        require(epsilon > 0.0, "legacy mutation requires positive epsilon");
        run_configuration = {RunKind::legacy_gamma_z, epsilon};
    }
    else
    {
        fail("run kind must be control, seeded, or legacy-gammaz");
    }
    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);
    const std::string mode(argv[2]);
    if (mode == "unstable")
    {
        return run_case<1>(parameters, "unstable");
    }
    if (mode == "stable")
    {
        return run_case<2>(parameters, "stable");
    }
    if (mode == "scan")
    {
        return run_case<1>(parameters, "scan");
    }
    if (mode == "transient-low")
    {
        return run_case<1>(parameters, "transient");
    }
    if (mode == "transient-high")
    {
        return run_case<2>(parameters, "transient");
    }
    if (mode == "d7-exact-gp")
    {
        require(run_configuration.kind == RunKind::control,
                "D7 controls must remain unperturbed");
        return run_case<1>(parameters, "d7_exact_gp");
    }
    if (mode == "d7-frozen-gauge")
    {
        require(run_configuration.kind == RunKind::control,
                "D7 controls must remain unperturbed");
        require(!parameters.fixed_lapse_source &&
                    parameters.gauge.lapse_advec_coeff == 0.0 &&
                    parameters.gauge.lapse_coeff == 0.0 &&
                    parameters.gauge.shift_Gamma_coeff == 0.0 &&
                    parameters.gauge.shift_advec_coeff == 0.0 &&
                    parameters.gauge.eta == 0.0,
                "D7 frozen gauge requires the existing zero-coefficient "
                "gauge-parameter seam");
        return run_case<1>(parameters, "d7_frozen_gauge");
    }
    if (mode == "d7-half-cfl")
    {
        require(run_configuration.kind == RunKind::control,
                "D7 controls must remain unperturbed");
        return run_case<1>(parameters, "d7_half_cfl");
    }
    if (mode == "d7-fine")
    {
        require(run_configuration.kind == RunKind::control,
                "D7 controls must remain unperturbed");
        return run_case<1>(parameters, "d7_fine");
    }
    if (mode == "d8-exact-abort")
    {
        require(run_configuration.kind == RunKind::control,
                "D8 controls must remain unperturbed");
        return run_case<1, D8ExactGPAbortLevel>(parameters,
                                                "d8_exact_abort");
    }
    if (mode == "d8-exact-gp")
    {
        require(run_configuration.kind == RunKind::control,
                "D8 controls must remain unperturbed");
        return run_case<1>(parameters, "d8_exact_gp");
    }
    if (mode == "d8-frozen-gauge")
    {
        require(run_configuration.kind == RunKind::control,
                "D8 controls must remain unperturbed");
        return run_case<1, D8FrozenGaugeLevel>(parameters,
                                               "d8_frozen_gauge");
    }
    if (mode == "d8-combined")
    {
        require(run_configuration.kind == RunKind::control,
                "D8 controls must remain unperturbed");
        return run_case<1, D8FrozenGaugeLevel>(parameters, "d8_combined");
    }
    if (mode == "d9-medium")
    {
        require(run_configuration.kind == RunKind::control,
                "D9 controls must remain unperturbed");
        return run_case<1, D9ControlLevel>(parameters, "d9_medium");
    }
    if (mode == "d9-fine")
    {
        require(run_configuration.kind == RunKind::control,
                "D9 controls must remain unperturbed");
        return run_case<1, D9ControlLevel>(parameters, "d9_fine");
    }
    if (mode == "d9-exact-gp")
    {
        require(run_configuration.kind == RunKind::control,
                "D9 controls must remain unperturbed");
        return run_case<1, D9ControlLevel>(parameters, "d9_exact_gp");
    }
    if (mode == "d11-ref")
    {
        require(run_configuration.kind == RunKind::control,
                "D11 controls must remain unperturbed");
        require(parameters.ko_sigma > 0.0,
                "D11 reference control requires positive KO");
        return run_case<1, D9ControlLevel>(parameters, "d11_ref");
    }
    if (mode == "d11-double")
    {
        require(run_configuration.kind == RunKind::control,
                "D11 controls must remain unperturbed");
        require(parameters.ko_sigma > 0.0,
                "D11 doubled control requires positive KO");
        return run_case<1, D9ControlLevel>(parameters, "d11_double");
    }
    if (mode == "d11-fine")
    {
        require(run_configuration.kind == RunKind::control,
                "D11 controls must remain unperturbed");
        require(parameters.ko_sigma > 0.0,
                "D11 fine control requires positive KO");
        return run_case<1, D9ControlLevel>(parameters, "d11_fine");
    }
    fail("mode must be unstable, stable, scan, transient-low, or "
         "transient-high, d7-exact-gp, d7-frozen-gauge, d7-half-cfl, or "
         "d7-fine, d8-exact-abort, d8-exact-gp, d8-frozen-gauge, or "
         "d8-combined, d9-medium, d9-fine, d9-exact-gp, d11-ref, "
         "d11-double, or d11-fine");
}
