#include "BlackStringToyLevel.hpp"

#include "BlackStringCellStorage.hpp"
#include "BlackStringGPInitialData.hpp"
#include "BlackStringPerturbativeRadialBoundary.hpp"
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

// Compile the real project level implementation into this strict fixture.
#include "../../BlackStringToyLevel.cpp"

namespace
{
namespace Reduced = BlackStringReducedVars;

enum class PulseSector
{
    gp,
    scalar,
    one_z
};

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_PERTURBATIVE_BOUNDARY_FAIL " << message << '\n';
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

PulseSector parse_sector(const std::string &name)
{
    if (name == "gp")
    {
        return PulseSector::gp;
    }
    if (name == "scalar")
    {
        return PulseSector::scalar;
    }
    if (name == "one-z")
    {
        return PulseSector::one_z;
    }
    fail("mode must be gp, scalar, one-z, or manufactured");
}

template <PulseSector sector> class PerturbedInitialData
{
  public:
    PerturbedInitialData(
        const double r0, const double dx,
        const std::array<double, CH_SPACEDIM> &coordinate_offset,
        const double pulse_center, const double compact_length)
        : m_r0(r0), m_dx(dx), m_coordinate_offset(coordinate_offset),
          m_pulse_center(pulse_center), m_compact_length(compact_length)
    {
    }

    void compute(const Cell<double> cell) const
    {
        const auto coordinates =
            BlackStringGPInitialData::cell_centered_coordinates(
                cell.get_int_vect(), m_dx, m_coordinate_offset);
        auto vars = BlackStringGPPointwiseInitialData::make_pointwise_vars(
            m_r0, coordinates.x);
        constexpr double amplitude = 1.0e-8;
        constexpr double width = 0.30;
        const double radial =
            std::exp(-std::pow((coordinates.x - m_pulse_center) / width, 2));
        const double phase =
            2.0 * 3.14159265358979323846 * coordinates.z / m_compact_length;
        if constexpr (sector == PulseSector::scalar)
        {
            BlackStringReducedVars::component(
                vars, BlackStringProductionVariables::c_chi) +=
                amplitude * radial * std::cos(phase);
        }
        else if constexpr (sector == PulseSector::one_z)
        {
            BlackStringReducedVars::component(
                vars, BlackStringProductionVariables::c_hxz) +=
                amplitude * radial * std::sin(phase);
        }
        BlackStringCellStorage::store(cell, vars);
    }

  private:
    double m_r0;
    double m_dx;
    std::array<double, CH_SPACEDIM> m_coordinate_offset;
    double m_pulse_center;
    double m_compact_length;
};

struct StateReport
{
    std::array<double, NUM_VARS> maximum_delta{};
    double maximum_radial_ghost_delta = 0.0;
    double radial_wrap_gap = 0.0;
    bool finite = true;
};

template <PulseSector sector>
class PerturbativeBoundaryLevel : public BlackStringToyLevel
{
    friend class DefaultLevelFactory<PerturbativeBoundaryLevel<sector>>;
    using BlackStringToyLevel::BlackStringToyLevel;

  public:
    double probe_outer_rhs_maximum()
    {
        GRLevelData rhs;
        defineRHSData(rhs, m_state_new);
        LevelFluxRegister fine_register;
        LevelFluxRegister coarse_register;
        const GRLevelData undefined_coarse;
        evalRHS(rhs, m_state_new, fine_register, coarse_register,
                undefined_coarse, 0.0, undefined_coarse, 0.0, m_time, 0.0);

        double maximum = 0.0;
        const int outer = m_problem_domain.domainBox().bigEnd(0);
        const DataIterator iterator = rhs.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const FArrayBox &fab = rhs[iterator[ibox]];
            Box surface = fab.box() & m_problem_domain.domainBox();
            if (surface.isEmpty() || surface.bigEnd(0) != outer)
            {
                continue;
            }
            surface.setSmall(0, outer);
            for (BoxIterator bit(surface); bit.ok(); ++bit)
            {
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    maximum =
                        std::max(maximum, std::abs(fab(bit(), component)));
                }
            }
        }
        return maximum;
    }

    StateReport inspect_state()
    {
        fillAllGhosts();
        StateReport report;
        report.maximum_delta.fill(0.0);
        const Box domain = m_problem_domain.domainBox();
        const auto offset = m_p.coordinate_offset();
        const DataIterator iterator = m_state_new.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = m_state_new[data_index];
            for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    report.finite =
                        report.finite &&
                        std::isfinite(fab(point, component));
                }
            }

            const Box valid = m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                const auto background = gp_state(point, offset);
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    report.maximum_delta[static_cast<std::size_t>(component)] =
                        std::max(
                            report.maximum_delta[static_cast<std::size_t>(
                                component)],
                            std::abs(fab(point, component) -
                                     background[static_cast<std::size_t>(
                                         component)]));
                }
            }

            for (const Side::LoHiSide side : {Side::Lo, Side::Hi})
            {
                Box ghosts = fab.box();
                if (side == Side::Lo)
                {
                    ghosts.setBig(0, domain.smallEnd(0) - 1);
                }
                else
                {
                    ghosts.setSmall(0, domain.bigEnd(0) + 1);
                }
                ghosts &= fab.box();
                for (BoxIterator bit(ghosts); bit.ok(); ++bit)
                {
                    const IntVect point = bit();
                    const auto background = gp_state(point, offset);
                    for (int component = 0; component < NUM_VARS; ++component)
                    {
                        report.maximum_radial_ghost_delta =
                            std::max(report.maximum_radial_ghost_delta,
                                     std::abs(
                                         fab(point, component) -
                                         background[static_cast<std::size_t>(
                                             component)]));
                    }
                }
            }
        }

        if constexpr (sector != PulseSector::gp)
        {
            IntVect low_ghost = domain.smallEnd();
            IntVect high_valid = domain.bigEnd();
            low_ghost[0] -= 1;
            low_ghost[1] = domain.smallEnd(1) + domain.size(1) / 4;
            high_valid[1] = low_ghost[1];
            const int component =
                sector == PulseSector::scalar ? c_chi : c_hxz;
            double low = 0.0;
            double high = 0.0;
            const bool found =
                sample(low_ghost, component, low) &&
                sample(high_valid, component, high);
            require(found, "radial wrap probe points were not found");
            const auto low_background = gp_state(low_ghost, offset);
            const auto high_background = gp_state(high_valid, offset);
            report.radial_wrap_gap =
                std::abs((low - low_background[component]) -
                         (high - high_background[component]));
        }
        return report;
    }

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
        const int radial_cells = m_problem_domain.domainBox().size(0);
        const int compact_cells = m_problem_domain.domainBox().size(1);
        const double x_out =
            m_p.coordinate_minimum[0] + m_dx * radial_cells;
        const double compact_length = m_dx * compact_cells;
        BoxLoops::loop(
            PerturbedInitialData<sector>(
                m_p.r0, m_dx, m_p.coordinate_offset(), x_out - 0.25,
                compact_length),
            m_state_new, m_state_new, EXCLUDE_GHOST_CELLS, disable_simd());
        fillAllGhosts();
    }

    std::array<double, NUM_VARS>
    gp_state(const IntVect &point,
             const std::array<double, CH_SPACEDIM> &offset) const
    {
        const double x = BlackStringCoordinates::cell_centered<double>(
            point[0], m_dx, offset[0]);
        return BlackStringGPPointwiseInitialData::make_pointwise_state(m_p.r0,
                                                                       x);
    }

    bool sample(const IntVect &point, const int component, double &value) const
    {
        const DataIterator iterator = m_state_new.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const FArrayBox &fab = m_state_new[iterator[ibox]];
            if (fab.box().contains(point))
            {
                value = fab(point, component);
                return true;
            }
        }
        return false;
    }
};

void run_manufactured_outgoing_test()
{
    constexpr double face = 4.5;
    constexpr double dx = 0.125;
    BlackStringPerturbativeRadialBoundary::SourceStates sources{};
    for (int source = 0;
         source < BlackStringPerturbativeRadialBoundary::source_points;
         ++source)
    {
        const double x = face - (static_cast<double>(source) + 0.5) * dx;
        sources[static_cast<std::size_t>(source)] =
            BlackStringGPPointwiseInitialData::make_pointwise_state(1.0, x);
        const double profile =
            1.0e-8 * std::exp(-std::pow((x - 3.5) / 0.4, 2));
        sources[static_cast<std::size_t>(source)][c_hxx] += profile;
        sources[static_cast<std::size_t>(source)][c_Axx] += 0.5 * profile;
        auto vars = Reduced::load(sources[static_cast<std::size_t>(source)]);
        BlackStringAlgebraicReconstruction::reconstruct(vars);
        sources[static_cast<std::size_t>(source)] = Reduced::store(vars);
    }
    const auto ghosts =
        BlackStringPerturbativeRadialBoundary::fill_characteristic_line(
            sources, face, 1, 1.0, dx);
    double maximum = 0.0;
    for (const auto &ghost : ghosts)
    {
        for (const double value : ghost)
        {
            require(std::isfinite(value),
                    "manufactured outgoing ghost is nonfinite");
            maximum = std::max(maximum, std::abs(value));
        }
        const auto vars = Reduced::load(ghost);
        require(std::abs(BlackStringAlgebraicReconstruction::
                             determinant_residual(vars)) <= 5.0e-13 &&
                    std::abs(BlackStringAlgebraicReconstruction::
                                 weighted_trace_residual(vars)) <= 5.0e-13,
                "manufactured outgoing ghost left algebraic manifold");
    }
    std::cout << std::scientific << std::setprecision(12)
              << "BOUNDARY_OUTGOING_PULSE ghost_maximum=" << maximum << '\n'
              << "BLACKSTRING_MANUFACTURED_OUTGOING_BOUNDARY_PASS\n";
}

template <PulseSector sector>
int run_live_case(SimulationParameters &parameters)
{
    require(parameters.physical_radial_boundaries,
            "physical radial boundary policy must be enabled");
    require(!parameters.background_preserving_gp_radial_ghosts,
            "exact-GP diagnostic policy must be disabled");
    require(parameters.constraint_diagnostic_cadence == 0,
            "boundary smoke diagnostics must remain disabled");
    require(parameters.coordinate_minimum[0] < parameters.r0,
            "inner boundary must lie inside r0");

    GRAMR amr;
    using Level = PerturbativeBoundaryLevel<sector>;
    DefaultLevelFactory<Level> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    require(levels.size() == 1, "boundary smoke must create one level");
    auto *level = dynamic_cast<Level *>(levels[0]);
    require(level != nullptr, "boundary factory returned the wrong level");

    const StateReport initial = level->inspect_state();
    require(initial.finite, "initial state or radial ghosts are nonfinite");
    const double initial_outer_rhs = level->probe_outer_rhs_maximum();
    if constexpr (sector == PulseSector::gp)
    {
        require(*std::max_element(initial.maximum_delta.begin(),
                                  initial.maximum_delta.end()) <
                    1.0e-13,
                "physical boundary changed exact GP initial data");
        require(initial.maximum_radial_ghost_delta < 1.0e-12,
                "physical radial closure does not exactly preserve GP");
        require(std::isfinite(initial_outer_rhs) && initial_outer_rhs < 1.0,
                "GP outer surface RHS is nonfinite or unbounded");
    }
    else if constexpr (sector == PulseSector::scalar)
    {
        require(initial.maximum_delta[c_chi] > 1.0e-10,
                "scalar pulse was not installed");
        for (const int component :
             {c_hxz, c_Axz, c_GammaZ, c_shiftZ, c_Bz})
        {
            require(initial.maximum_delta[component] < 1.0e-13,
                    "scalar pulse leaked into the one-z sector");
        }
        require(initial.radial_wrap_gap > 1.0e-10,
                "scalar radial boundary behaved like a periodic wrap");
    }
    else
    {
        require(initial.maximum_delta[c_hxz] > 1.0e-10,
                "one-z pulse was not installed");
        require(initial.maximum_delta[c_chi] < 1.0e-13,
                "one-z pulse leaked into the scalar sector");
        require(initial.radial_wrap_gap > 1.0e-10,
                "one-z radial boundary behaved like a periodic wrap");
    }

    amr.run(parameters.stop_time, parameters.max_steps);
    const StateReport final = level->inspect_state();
    const auto &counts = level->instrumentation_report();
    require(final.finite, "perturbative boundary smoke became nonfinite");
    if constexpr (sector == PulseSector::gp)
    {
        require(*std::max_element(final.maximum_delta.begin(),
                                  final.maximum_delta.end()) <
                    1.0e-3,
                "GP truncation drift exceeded the short boundary smoke");
        require(final.maximum_radial_ghost_delta < 2.0e-2,
                "GP boundary drift exceeded the short boundary smoke");
    }
    require(counts.low_radial_fills == counts.ghost_fill_calls &&
                counts.high_radial_fills == counts.ghost_fill_calls,
            "inner or outer radial ghosts were not filled exactly once");
    require(counts.outer_radiative_rhs_calls == counts.rhs_calls,
            "outer radiative RHS was not applied once per RHS");
    require(counts.rhs_periodic_exchanges == counts.rhs_calls,
            "RHS periodic exchange count changed");
    require(counts.periodic_exchanges ==
                counts.fillall_periodic_exchanges +
                    counts.rhs_periodic_exchanges,
            "radial policy performed a duplicate exchange");
    require(counts.diagnostic_evaluations == 0,
            "disabled diagnostics executed a constraint loop");

    const char *name = sector == PulseSector::gp
                           ? "gp"
                           : (sector == PulseSector::scalar ? "scalar"
                                                            : "one-z");
    std::cout << std::scientific << std::setprecision(12)
              << "BOUNDARY_LIVE mode=" << name
              << " state_delta="
              << *std::max_element(final.maximum_delta.begin(),
                                   final.maximum_delta.end())
              << " radial_ghost_delta=" << final.maximum_radial_ghost_delta
              << " initial_outer_rhs=" << initial_outer_rhs
              << " inner_calls=" << counts.low_radial_fills
              << " outer_calls=" << counts.high_radial_fills
              << " outer_rhs_calls=" << counts.outer_radiative_rhs_calls
              << " rhs_calls=" << counts.rhs_calls
              << " periodic_exchanges=" << counts.periodic_exchanges
              << " diagnostics=" << counts.diagnostic_evaluations << '\n'
              << "BLACKSTRING_PERTURBATIVE_BOUNDARY_LIVE_PASS\n";
    amr.conclude();
    return 0;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fail("usage: BlackStringPerturbativeRadialBoundaryTest <params> "
             "<gp|scalar|one-z|manufactured>");
    }
    if (std::string(argv[2]) == "manufactured")
    {
        run_manufactured_outgoing_test();
        return 0;
    }

    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);
    switch (parse_sector(argv[2]))
    {
    case PulseSector::gp:
        return run_live_case<PulseSector::gp>(parameters);
    case PulseSector::scalar:
        return run_live_case<PulseSector::scalar>(parameters);
    case PulseSector::one_z:
        return run_live_case<PulseSector::one_z>(parameters);
    }
    fail("unreachable pulse sector");
}
