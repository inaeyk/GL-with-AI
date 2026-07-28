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
    double time = 0.0;
    double cosine = 0.0;
    double sine = 0.0;
    double amplitude = 0.0;
    double phase = 0.0;
    std::array<double, NUM_DIAGNOSTIC_VARS> constraints{};
    double state_drift = 0.0;
};

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
        require(run_configuration.epsilon > 0.0,
                "parity inspection requires a seeded run");
        const double normalization =
            2.0 /
            (run_configuration.epsilon * static_cast<double>(points));
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
        const auto offset = m_p.coordinate_offset();
        std::vector<double> cosine_sum(
            static_cast<std::size_t>(radial_cells), 0.0);
        std::vector<double> sine_sum(
            static_cast<std::size_t>(radial_cells), 0.0);
        std::vector<int> compact_count(
            static_cast<std::size_t>(radial_cells), 0);

        Sample sample;
        sample.time = m_time;
        sample.constraints.fill(0.0);
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
                const double chi = state(point, c_chi);
                const double hww = state(point, c_hww);
                m_finite = m_finite && std::isfinite(chi) &&
                           std::isfinite(hww) && chi > 0.0 && hww > 0.0;
                if (chi > 0.0 && hww > 0.0)
                {
                    const double areal_log = 0.5 * std::log(hww / chi);
                    const int radial_index =
                        point[0] - domain.smallEnd(0);
                    cosine_sum[static_cast<std::size_t>(radial_index)] +=
                        areal_log * std::cos(k * z);
                    sine_sum[static_cast<std::size_t>(radial_index)] +=
                        areal_log * std::sin(k * z);
                    ++compact_count[static_cast<std::size_t>(radial_index)];
                }

                const auto background =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        m_p.r0, x);
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    const double value = state(point, component);
                    m_finite = m_finite && std::isfinite(value);
                    sample.state_drift =
                        std::max(sample.state_drift,
                                 std::abs(value -
                                          background[static_cast<std::size_t>(
                                              component)]));
                }
                for (int component = 0; component < NUM_DIAGNOSTIC_VARS;
                     ++component)
                {
                    const double value = constraints(point, component);
                    m_finite = m_finite && std::isfinite(value);
                    sample.constraints[static_cast<std::size_t>(component)] =
                        std::max(
                            sample.constraints[static_cast<std::size_t>(
                                component)],
                            std::abs(value));
                }
            }
        }
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
            cosine_square_sum += cosine_coefficient * cosine_coefficient;
            sine_square_sum += sine_coefficient * sine_coefficient;
            cosine_sign_sum += cosine_coefficient;
            sine_sign_sum += sine_coefficient;
            ++analysis_count;
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
        m_samples.push_back(sample);
    }

    std::vector<Sample> m_samples;
    double m_maximum_state_drift = 0.0;
    double m_maximum_phase_rotation_error = 0.0;
    bool m_finite = true;
};

template <int mode_number>
int run_case(SimulationParameters &parameters, const char *mode_name)
{
    require(parameters.physical_radial_boundaries,
            "physical radial boundary policy must be enabled");
    require(!parameters.background_preserving_gp_radial_ghosts,
            "exact-GP diagnostic radial policy must be disabled");
    require(parameters.constraint_diagnostic_cadence > 0,
            "growth fixture requires an explicit positive diagnostic cadence");
    require(parameters.max_level == 0,
            "growth fixture must remain serial level zero");

    GRAMR amr;
    using Level = FourierGrowthLevel<mode_number>;
    DefaultLevelFactory<Level> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    require(levels.size() == 1, "growth fixture must create one level");
    auto *level = dynamic_cast<Level *>(levels[0]);
    require(level != nullptr, "growth fixture factory returned wrong level");
    const SeedReport seed = level->inspect_gamma_z_seed();
    const double seed_tolerance =
        std::max(1.0e-24, run_configuration.epsilon * 1.0e-10);
    require(seed.legacy_gamma_z_maximum_difference >
                run_configuration.epsilon * 1.0e-3 ||
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
    require(counts.outer_radiative_rhs_calls == counts.rhs_calls,
            "outer radial RHS was not applied once per live RHS");
    require(counts.rhs_periodic_exchanges == counts.rhs_calls,
            "framework periodic exchange count differs from live RHS count");
    require(counts.periodic_exchanges ==
                counts.fillall_periodic_exchanges +
                    counts.rhs_periodic_exchanges,
            "radial policy performed a duplicate periodic exchange");
    require(counts.diagnostic_evaluations == level->samples().size(),
            "diagnostic cadence count differs from collected samples");

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
                ? sample.amplitude / run_configuration.epsilon
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
    }
    if (run_configuration.seeded())
    {
        for (const FitWindow window : fit_windows)
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
             "<unstable|stable> <control|seeded|legacy-gammaz> <epsilon>");
    }
    const std::string kind(argv[3]);
    char *epsilon_end = nullptr;
    const double epsilon = std::strtod(argv[4], &epsilon_end);
    require(epsilon_end != argv[4] && *epsilon_end == '\0' &&
                std::isfinite(epsilon) && epsilon >= 0.0,
            "epsilon must be a finite nonnegative number");
    if (kind == "control")
    {
        require(epsilon == 0.0, "control run requires epsilon=0");
        run_configuration = {RunKind::control, 0.0};
    }
    else if (kind == "seeded")
    {
        require(epsilon > 0.0, "seeded run requires positive epsilon");
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
    fail("mode must be unstable or stable");
}
