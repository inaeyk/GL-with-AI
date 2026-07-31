#include "BlackStringKODissipation.hpp"
#include "BlackStringLive.hpp"
#include "BlackStringPerturbativeRadialBoundary.hpp"
#include "BoxIterator.H"
#include "BoxPointers.hpp"
#include "DisjointBoxLayout.H"
#include "FourthOrderDerivatives.hpp"
#include "GRLevelData.hpp"
#include "Interval.H"
#include "ProblemDomain.H"
#include "SPMD.H"
#include "parstream.H"

#ifdef CH_MPI
#include "mpi.h"
#endif

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

// The fixture links only Chombo BoxTools. Supply the locked GRLevelData
// default constructor locally so the production radial policy receives its
// real state type without pulling the entire AMR driver into this short test.
GRLevelData::GRLevelData() : LevelData<FArrayBox>() {}

namespace
{
namespace KO = BlackStringKODissipation;
namespace Live = BlackStringLive;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace GP = BlackStringGPPointwiseInitialData;

constexpr int nx = 32;
constexpr int nz = 32;
constexpr int components = Production::NUM_VARS;
constexpr int ghost_depth = 3;
constexpr double dx = 0.125;
constexpr double sigma = 0.3;
constexpr double r0 = 1.0;
constexpr double pi = 3.141592653589793238462643383279502884;
constexpr double sentinel = -9.876543210123456e150;
constexpr double comparison_tolerance = 2.0e-10;
constexpr std::array<double, CH_SPACEDIM> coordinate_offset = {
    0.5 * dx - 0.5, 0.5 * dx};

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(components == 18);
static_assert(ghost_depth == KO::stencil_radius);
static_assert(KO::radial_direction == 0);
static_assert(KO::compact_direction == 1);

enum class LayoutKind
{
    compact,
    radial,
    both
};

enum class Operator
{
    dx1 = 0,
    dxx,
    dz1,
    dzz,
    dxz,
    ko_x,
    ko_z,
    ko_mixed,
    count
};

constexpr int operator_count = static_cast<int>(Operator::count);
constexpr std::array<const char *, operator_count> operator_names = {
    "dx", "dxx", "dz", "dzz", "dxz", "ko_x", "ko_z", "ko_x_plus_z"};

struct LayoutSpec
{
    LayoutKind kind;
    const char *name;
    Vector<Box> boxes;
    Vector<int> processors;
    ProblemDomain domain;
};

struct ErrorMetric
{
    double maximum = 0.0;
    double sum_squares = 0.0;
    long long count = 0;
    int worst_key = -1;
};

struct ReducedMetric
{
    double maximum = 0.0;
    double weighted_l2 = 0.0;
    long long count = 0;
    int worst_key = -1;
};

struct LayoutResult
{
    std::array<ReducedMetric, operator_count> global{};
    std::array<ReducedMetric, operator_count> seam{};
    ReducedMetric corner{};
    std::array<double, components> maximum_reference{};
    int hidden_direction_accesses = 0;
    int valid_overwrites = 0;
    int wrong_owner_ghosts = 0;
    int unfilled_ghosts = 0;
    int low_radial_ghost_boxes = 0;
    int high_radial_ghost_boxes = 0;
    int outer_rhs_boxes = 0;
    int framework_ghost_cells = 0;
    int radial_ghost_cells = 0;
    int radial_periodic_corner_cells = 0;
};

IntVect iv(const int x, const int z)
{
    return IntVect(D_DECL(x, z, 0));
}

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "M1C_FAIL rank=" << procID() << " " << message << '\n';
#ifdef CH_MPI
    MPI_Abort(MPI_COMM_WORLD, 1);
#endif
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

bool close_enough(const double actual, const double expected,
                  const double tolerance = comparison_tolerance)
{
    const double scale =
        std::max({1.0, std::abs(actual), std::abs(expected)});
    return std::isfinite(actual) && std::isfinite(expected) &&
           std::abs(actual - expected) <= tolerance * scale;
}

int wrap_z(int z)
{
    int wrapped = z % nz;
    if (wrapped < 0)
    {
        wrapped += nz;
    }
    return wrapped;
}

bool one_z_slot(const int component)
{
    return component == Production::c_hxz ||
           component == Production::c_Axz ||
           component == Production::c_GammaZ ||
           component == Production::c_shiftZ ||
           component == Production::c_Bz;
}

double manufactured_value(const IntVect &point, const int component)
{
    const int z_index = wrap_z(point[1]);
    const double radial =
        (static_cast<double>(point[0]) + 0.5) / static_cast<double>(nx);
    const double angle =
        2.0 * pi * (static_cast<double>(z_index) + 0.5) /
        static_cast<double>(nz);
    const double parity_profile =
        one_z_slot(component)
            ? std::sin(angle) + 0.27 * std::sin(2.0 * angle)
            : std::cos(angle) + 0.19 * std::cos(3.0 * angle);
    const double scale = 0.03 * static_cast<double>(component + 1);
    const double radial_profile =
        0.31 * radial + 0.17 * radial * radial +
        0.11 * std::pow(radial, 6);
    const double coupled_profile =
        (0.73 + 0.23 * radial + 0.09 * radial * radial) * parity_profile;
    const double base = one_z_slot(component)
                            ? 0.0
                            : 1.0 + 0.05 * static_cast<double>(component);
    return base + scale * (radial_profile + coupled_profile);
}

std::array<double, components> gp_state(const IntVect &point)
{
    const double x =
        (static_cast<double>(point[0]) + 0.5) * dx - coordinate_offset[0];
    return GP::make_pointwise_state(r0, x);
}

ProblemDomain global_domain()
{
    ProblemDomain domain(Box(iv(0, 0), iv(nx - 1, nz - 1)));
    domain.setPeriodic(0, false);
    domain.setPeriodic(1, true);
    return domain;
}

LayoutSpec make_layout(const LayoutKind kind, const bool distributed)
{
    LayoutSpec spec{kind, "", {}, {}, global_domain()};
    if (kind == LayoutKind::compact)
    {
        spec.name = "compact_z_seams";
        for (int block = 0; block < 4; ++block)
        {
            spec.boxes.push_back(
                Box(iv(0, 8 * block), iv(nx - 1, 8 * block + 7)));
        }
        spec.processors = Vector<int>(4, 0);
        if (distributed)
        {
            spec.processors[1] = 1;
            spec.processors[3] = 1;
        }
    }
    else if (kind == LayoutKind::radial)
    {
        spec.name = "radial_x_seams";
        for (int block = 0; block < 4; ++block)
        {
            spec.boxes.push_back(
                Box(iv(8 * block, 0), iv(8 * block + 7, nz - 1)));
        }
        spec.processors = Vector<int>(4, 0);
        if (distributed)
        {
            spec.processors[2] = 1;
            spec.processors[3] = 1;
        }
    }
    else
    {
        spec.name = "combined_xz_seams";
        spec.boxes.push_back(Box(iv(0, 0), iv(15, 15)));
        spec.boxes.push_back(Box(iv(0, 16), iv(15, 31)));
        spec.boxes.push_back(Box(iv(16, 0), iv(31, 15)));
        spec.boxes.push_back(Box(iv(16, 16), iv(31, 31)));
        spec.processors = Vector<int>(4, 0);
        if (distributed)
        {
            spec.processors[1] = 1;
            spec.processors[2] = 2;
            spec.processors[3] = 3;
        }
    }
    return spec;
}

int box_id(const LayoutSpec &spec, const Box &box)
{
    for (int index = 0; index < static_cast<int>(spec.boxes.size()); ++index)
    {
        if (spec.boxes[static_cast<std::size_t>(index)] == box)
        {
            return index;
        }
    }
    return -1;
}

void define_state(GRLevelData &state, const DisjointBoxLayout &layout)
{
    state.define(layout, components, ghost_depth * IntVect::Unit);
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        FArrayBox &fab = state[dit];
        fab.setVal(std::numeric_limits<double>::quiet_NaN());
        for (BoxIterator bit(layout[dit]); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            for (int component = 0; component < components; ++component)
            {
                fab(point, component) =
                    manufactured_value(point, component);
            }
        }
    }
}

void add_error(ErrorMetric &metric, const double actual,
               const double expected, const int component,
               const IntVect &point)
{
    const double error = std::abs(actual - expected);
    require(std::isfinite(error), "nonfinite manufactured comparison");
    metric.sum_squares += error * error;
    ++metric.count;
    const int key = component * nx * nz + point[0] * nz + wrap_z(point[1]);
    if (error > metric.maximum)
    {
        metric.maximum = error;
        metric.worst_key = key;
    }
}

ReducedMetric reduce_metric(const ErrorMetric &local)
{
    ReducedMetric reduced;
#ifdef CH_MPI
    MPI_Allreduce(&local.sum_squares, &reduced.weighted_l2, 1, MPI_DOUBLE,
                  MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&local.count, &reduced.count, 1, MPI_LONG_LONG, MPI_SUM,
                  MPI_COMM_WORLD);
    struct
    {
        double value;
        int key;
    } input{local.maximum, local.worst_key}, output{};
    MPI_Allreduce(&input, &output, 1, MPI_DOUBLE_INT, MPI_MAXLOC,
                  MPI_COMM_WORLD);
    reduced.maximum = output.value;
    reduced.worst_key = output.key;
#else
    reduced.weighted_l2 = local.sum_squares;
    reduced.count = local.count;
    reduced.maximum = local.maximum;
    reduced.worst_key = local.worst_key;
#endif
    reduced.weighted_l2 = dx * std::sqrt(reduced.weighted_l2);
    return reduced;
}

int reduce_sum(const int local)
{
#ifdef CH_MPI
    int global = 0;
    MPI_Allreduce(&local, &global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    return global;
#else
    return local;
#endif
}

double discrete_diff1(const IntVect &point, const int component,
                      const int direction)
{
    const std::array<int, 4> offsets = {-2, -1, 1, 2};
    const std::array<double, 4> weights = {
        1.0 / 12.0, -2.0 / 3.0, 2.0 / 3.0, -1.0 / 12.0};
    double result = 0.0;
    for (std::size_t index = 0; index < offsets.size(); ++index)
    {
        IntVect sample = point;
        sample[direction] += offsets[index];
        result += weights[index] * manufactured_value(sample, component);
    }
    return result / dx;
}

double discrete_diff2(const IntVect &point, const int component,
                      const int direction)
{
    const std::array<int, 5> offsets = {-2, -1, 0, 1, 2};
    const std::array<double, 5> weights = {
        -1.0 / 12.0, 4.0 / 3.0, -2.5, 4.0 / 3.0, -1.0 / 12.0};
    double result = 0.0;
    for (std::size_t index = 0; index < offsets.size(); ++index)
    {
        IntVect sample = point;
        sample[direction] += offsets[index];
        result += weights[index] * manufactured_value(sample, component);
    }
    return result / (dx * dx);
}

double discrete_mixed(const IntVect &point, const int component)
{
    const std::array<int, 4> offsets = {-2, -1, 1, 2};
    const std::array<double, 4> weights = {
        1.0 / 12.0, -2.0 / 3.0, 2.0 / 3.0, -1.0 / 12.0};
    double result = 0.0;
    for (std::size_t ix = 0; ix < offsets.size(); ++ix)
    {
        for (std::size_t iz = 0; iz < offsets.size(); ++iz)
        {
            IntVect sample = point;
            sample[0] += offsets[ix];
            sample[1] += offsets[iz];
            result += weights[ix] * weights[iz] *
                      manufactured_value(sample, component);
        }
    }
    return result / (dx * dx);
}

double discrete_ko(const IntVect &point, const int component,
                   const int direction)
{
    const std::array<int, 7> offsets = {-3, -2, -1, 0, 1, 2, 3};
    const std::array<double, 7> weights = {
        1.0, -6.0, 15.0, -20.0, 15.0, -6.0, 1.0};
    double result = 0.0;
    for (std::size_t index = 0; index < offsets.size(); ++index)
    {
        IntVect sample = point;
        sample[direction] += offsets[index];
        result += weights[index] * manufactured_value(sample, component);
    }
    return sigma * result / (64.0 * dx);
}

bool crosses_plane(const int coordinate, const int plane,
                   const int radius)
{
    return coordinate - radius < plane && coordinate + radius >= plane;
}

bool crosses_x_seam(const LayoutKind kind, const int x, const int radius)
{
    if (kind == LayoutKind::compact)
    {
        return false;
    }
    if (kind == LayoutKind::radial)
    {
        return crosses_plane(x, 8, radius) ||
               crosses_plane(x, 16, radius) ||
               crosses_plane(x, 24, radius);
    }
    return crosses_plane(x, 16, radius);
}

bool crosses_z_seam(const LayoutKind kind, const int z, const int radius)
{
    const bool periodic =
        z - radius < 0 || z + radius >= nz;
    if (kind == LayoutKind::compact)
    {
        return periodic || crosses_plane(z, 8, radius) ||
               crosses_plane(z, 16, radius) ||
               crosses_plane(z, 24, radius);
    }
    if (kind == LayoutKind::both)
    {
        return periodic || crosses_plane(z, 16, radius);
    }
    return periodic;
}

bool seam_point(const LayoutKind kind, const Operator op,
                const IntVect &point)
{
    switch (op)
    {
    case Operator::dx1:
    case Operator::dxx:
        return crosses_x_seam(kind, point[0], 2);
    case Operator::dz1:
    case Operator::dzz:
        return crosses_z_seam(kind, point[1], 2);
    case Operator::dxz:
        return crosses_x_seam(kind, point[0], 2) ||
               crosses_z_seam(kind, point[1], 2);
    case Operator::ko_x:
        return crosses_x_seam(kind, point[0], 3);
    case Operator::ko_z:
        return crosses_z_seam(kind, point[1], 3);
    case Operator::ko_mixed:
        return crosses_x_seam(kind, point[0], 3) ||
               crosses_z_seam(kind, point[1], 3);
    case Operator::count:
        break;
    }
    return false;
}

class ProjectedKOInput
{
  public:
    ProjectedKOInput(const Cell<double> &cell, const bool radial,
                     const bool compact)
        : m_cell(cell), m_index(cell.get_in_index()), m_radial(radial),
          m_compact(compact)
    {
    }

    int index() const { return m_index; }

    int stride(const int direction) const
    {
        require(direction >= 0 && direction < CH_SPACEDIM,
                "KO requested hidden grid direction");
        m_direction_mask |= 1 << direction;
        if ((direction == KO::radial_direction && !m_radial) ||
            (direction == KO::compact_direction && !m_compact))
        {
            return 0;
        }
        return m_cell.get_box_pointers().m_in_stride[direction];
    }

    const double *values(const int component) const
    {
        return m_cell.get_box_pointers().m_in_ptr[component];
    }

    int direction_mask() const { return m_direction_mask; }

  private:
    const Cell<double> &m_cell;
    int m_index;
    bool m_radial;
    bool m_compact;
    mutable int m_direction_mask = 0;
};

template <class policy_t = KO::DefaultPolicy>
Reduced::Storage<double> projected_ko(const Cell<double> &cell,
                                      const bool radial,
                                      const bool compact,
                                      int &direction_mask)
{
    ProjectedKOInput input(cell, radial, compact);
    Reduced::Variables<double> rhs{};
    KO::add_from_input<ProjectedKOInput, policy_t>(rhs, input, dx, sigma);
    direction_mask |= input.direction_mask();
    return Reduced::store(rhs);
}

double expected_radial_ghost(const IntVect &ghost, const int component,
                             const Side::LoHiSide side)
{
    const int distance =
        side == Side::Lo ? -ghost[0] : ghost[0] - (nx - 1);
    const std::array<std::array<double, 5>, 3> weights = {{
        {{5.0, -10.0, 10.0, -5.0, 1.0}},
        {{15.0, -40.0, 45.0, -24.0, 5.0}},
        {{35.0, -105.0, 126.0, -70.0, 15.0}},
    }};
    require(distance >= 1 && distance <= 3,
            "invalid radial ghost distance in fixture reference");
    const auto ghost_background = gp_state(ghost);
    double delta = 0.0;
    for (int source = 0; source < 5; ++source)
    {
        IntVect source_point = ghost;
        source_point[0] =
            side == Side::Lo ? source : nx - 1 - source;
        const auto source_background = gp_state(source_point);
        delta +=
            weights[static_cast<std::size_t>(distance - 1)]
                   [static_cast<std::size_t>(source)] *
            (manufactured_value(source_point, component) -
             source_background[static_cast<std::size_t>(component)]);
    }
    return ghost_background[static_cast<std::size_t>(component)] + delta;
}

void verify_exchange_and_radial_ownership(
    const LayoutSpec &spec, const DisjointBoxLayout &layout,
    GRLevelData &state, LayoutResult &result)
{
    state.exchange(Interval(0, components - 1));
    BlackStringPerturbativeRadialBoundary::fill_solution_ghosts(
        state, spec.domain, r0, dx, coordinate_offset, Side::Lo,
        Interval(0, components - 1));
    BlackStringPerturbativeRadialBoundary::fill_solution_ghosts(
        state, spec.domain, r0, dx, coordinate_offset, Side::Hi,
        Interval(0, components - 1));

    int local_low_boxes = 0;
    int local_high_boxes = 0;
    int local_framework_ghosts = 0;
    int local_radial_ghosts = 0;
    int local_corners = 0;
    int local_overwrites = 0;
    int local_wrong_owner = 0;
    int local_unfilled = 0;
    int low_mask = 0;
    int high_mask = 0;

    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = layout[dit];
        const int id = box_id(spec, valid);
        require(id >= 0, "local box missing from layout specification");
        if (valid.smallEnd(0) == 0)
        {
            ++local_low_boxes;
            low_mask |= 1 << id;
        }
        if (valid.bigEnd(0) == nx - 1)
        {
            ++local_high_boxes;
            high_mask |= 1 << id;
        }
        const FArrayBox &fab = state[dit];
        for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            if (valid.contains(point))
            {
                for (int component = 0; component < components; ++component)
                {
                    if (fab(point, component) !=
                        manufactured_value(point, component))
                    {
                        ++local_overwrites;
                    }
                }
                continue;
            }

            const bool radial =
                point[0] < 0 || point[0] >= nx;
            const bool periodic_corner =
                radial && (point[1] < 0 || point[1] >= nz);
            if (radial)
            {
                ++local_radial_ghosts;
                if (periodic_corner)
                {
                    ++local_corners;
                }
            }
            else
            {
                ++local_framework_ghosts;
            }

            for (int component = 0; component < components; ++component)
            {
                const double actual = fab(point, component);
                if (!std::isfinite(actual))
                {
                    ++local_unfilled;
                    continue;
                }
                const double expected =
                    radial
                        ? expected_radial_ghost(
                              point, component,
                              point[0] < 0 ? Side::Lo : Side::Hi)
                        : manufactured_value(point, component);
                if (!close_enough(actual, expected))
                {
                    ++local_wrong_owner;
                }
            }
        }
    }

    result.low_radial_ghost_boxes = reduce_sum(local_low_boxes);
    result.high_radial_ghost_boxes = reduce_sum(local_high_boxes);
    result.framework_ghost_cells = reduce_sum(local_framework_ghosts);
    result.radial_ghost_cells = reduce_sum(local_radial_ghosts);
    result.radial_periodic_corner_cells = reduce_sum(local_corners);
    result.valid_overwrites = reduce_sum(local_overwrites);
    result.wrong_owner_ghosts = reduce_sum(local_wrong_owner);
    result.unfilled_ghosts = reduce_sum(local_unfilled);

    std::array<int, 8> local_ownership = {
        static_cast<int>(state.dataIterator().size()),
        0,
        low_mask,
        high_mask,
        local_low_boxes,
        local_high_boxes,
        local_framework_ghosts,
        local_radial_ghosts};
    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        local_ownership[1] += layout[dit].numPts();
    }
    const int ranks = static_cast<int>(numProc());
    std::vector<int> gathered(
        static_cast<std::size_t>(8 * std::max(1, ranks)), 0);
#ifdef CH_MPI
    MPI_Gather(local_ownership.data(), 8, MPI_INT, gathered.data(), 8,
               MPI_INT, 0, MPI_COMM_WORLD);
#else
    std::copy(local_ownership.begin(), local_ownership.end(),
              gathered.begin());
#endif
    if (procID() == 0)
    {
        for (int rank = 0; rank < ranks; ++rank)
        {
            const int *entry =
                gathered.data() + static_cast<std::ptrdiff_t>(8 * rank);
            std::cout << "M1C_OWNERSHIP layout=" << spec.name
                      << " rank=" << rank
                      << " local_boxes=" << entry[0]
                      << " valid_cells=" << entry[1]
                      << " low_box_mask=0x" << std::hex << entry[2]
                      << " high_box_mask=0x" << entry[3] << std::dec
                      << " low_radial_box_fills=" << entry[4]
                      << " high_radial_box_fills=" << entry[5]
                      << " framework_ghost_cells=" << entry[6]
                      << " radial_ghost_cells=" << entry[7] << '\n';
        }
    }
}

void verify_outer_rhs_ownership(const LayoutSpec &spec,
                                const DisjointBoxLayout &layout,
                                const GRLevelData &state,
                                LayoutResult &result)
{
    GRLevelData rhs;
    rhs.define(layout, components, IntVect::Zero);
    for (DataIterator dit = rhs.dataIterator(); dit.ok(); ++dit)
    {
        rhs[dit].setVal(sentinel);
    }
    BlackStringPerturbativeRadialBoundary::apply_outer_rhs(
        state, rhs, spec.domain, r0, dx, coordinate_offset, 1.0,
        Interval(0, components - 1));

    int local_outer_boxes = 0;
    int local_wrong = 0;
    int outer_mask = 0;
    for (DataIterator dit = rhs.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = layout[dit];
        const int id = box_id(spec, valid);
        bool changed_box = false;
        for (BoxIterator bit(valid); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            for (int component = 0; component < components; ++component)
            {
                const double value = rhs[dit](point, component);
                const bool should_change = point[0] == nx - 1;
                if (should_change)
                {
                    if (!std::isfinite(value) || value == sentinel)
                    {
                        ++local_wrong;
                    }
                    changed_box = true;
                }
                else if (value != sentinel)
                {
                    ++local_wrong;
                }
            }
        }
        if (changed_box)
        {
            ++local_outer_boxes;
            outer_mask |= 1 << id;
        }
    }
    result.outer_rhs_boxes = reduce_sum(local_outer_boxes);
    require(reduce_sum(local_wrong) == 0,
            "outer RHS override escaped global outer surface");

    std::array<int, 2> local = {local_outer_boxes, outer_mask};
    const int ranks = static_cast<int>(numProc());
    std::vector<int> gathered(
        static_cast<std::size_t>(2 * std::max(1, ranks)), 0);
#ifdef CH_MPI
    MPI_Gather(local.data(), 2, MPI_INT, gathered.data(), 2, MPI_INT, 0,
               MPI_COMM_WORLD);
#else
    std::copy(local.begin(), local.end(), gathered.begin());
#endif
    if (procID() == 0)
    {
        for (int rank = 0; rank < ranks; ++rank)
        {
            std::cout << "M1C_OUTER_RHS layout=" << spec.name
                      << " rank=" << rank
                      << " surface_boxes=" << gathered[2 * rank]
                      << " box_mask=0x" << std::hex
                      << gathered[2 * rank + 1] << std::dec << '\n';
        }
    }
}

LayoutResult evaluate_layout(const LayoutSpec &spec)
{
    DisjointBoxLayout layout(spec.boxes, spec.processors, spec.domain);
    GRLevelData state;
    define_state(state, layout);
    LayoutResult result;
    verify_exchange_and_radial_ownership(spec, layout, state, result);
    verify_outer_rhs_ownership(spec, layout, state, result);

    std::array<ErrorMetric, operator_count> local_global{};
    std::array<ErrorMetric, operator_count> local_seam{};
    ErrorMetric local_corner;
    std::array<double, components> local_reference{};
    int local_direction_mask = 0;

    for (DataIterator dit = state.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = layout[dit];
        FArrayBox scratch(valid, components);
        scratch.setVal(0.0);
        BoxPointers pointers(state[dit], scratch);
        for (BoxIterator bit(valid); bit.ok(); ++bit)
        {
            const IntVect point = bit();
            if (point[0] < ghost_depth ||
                point[0] >= nx - ghost_depth)
            {
                continue;
            }
            Cell<double> cell(point, pointers);
            const auto live_input =
                Live::make_pointwise_input(cell, dx, coordinate_offset);
            int x_mask = 0;
            int z_mask = 0;
            int mixed_mask = 0;
            const auto ko_x =
                projected_ko(cell, true, false, x_mask);
            const auto ko_z =
                projected_ko(cell, false, true, z_mask);
            const auto ko_mixed =
                projected_ko(cell, true, true, mixed_mask);
            local_direction_mask |= x_mask | z_mask | mixed_mask;

            for (int component = 0; component < components; ++component)
            {
                const auto &jet =
                    live_input.derivatives[static_cast<std::size_t>(component)];
                const std::array<double, operator_count> actual = {
                    jet.dx,
                    jet.dxx,
                    jet.dz,
                    jet.dzz,
                    jet.dxz,
                    ko_x[static_cast<std::size_t>(component)],
                    ko_z[static_cast<std::size_t>(component)],
                    ko_mixed[static_cast<std::size_t>(component)]};
                const double ref_ko_x =
                    discrete_ko(point, component, 0);
                const double ref_ko_z =
                    discrete_ko(point, component, 1);
                const std::array<double, operator_count> expected = {
                    discrete_diff1(point, component, 0),
                    discrete_diff2(point, component, 0),
                    discrete_diff1(point, component, 1),
                    discrete_diff2(point, component, 1),
                    discrete_mixed(point, component),
                    ref_ko_x,
                    ref_ko_z,
                    ref_ko_x + ref_ko_z};
                require(close_enough(
                            actual[static_cast<int>(Operator::ko_mixed)],
                            actual[static_cast<int>(Operator::ko_x)] +
                                actual[static_cast<int>(Operator::ko_z)]),
                        "project KO mixed response is not additive");
                for (int op_index = 0; op_index < operator_count; ++op_index)
                {
                    const auto op = static_cast<Operator>(op_index);
                    add_error(local_global[static_cast<std::size_t>(op_index)],
                              actual[static_cast<std::size_t>(op_index)],
                              expected[static_cast<std::size_t>(op_index)],
                              component, point);
                    if (seam_point(spec.kind, op, point))
                    {
                        add_error(
                            local_seam[static_cast<std::size_t>(op_index)],
                            actual[static_cast<std::size_t>(op_index)],
                            expected[static_cast<std::size_t>(op_index)],
                            component, point);
                    }
                    local_reference[static_cast<std::size_t>(component)] =
                        std::max(
                            local_reference[static_cast<std::size_t>(component)],
                            std::abs(expected[
                                static_cast<std::size_t>(op_index)]));
                }
                if (spec.kind == LayoutKind::both &&
                    crosses_x_seam(spec.kind, point[0], 2) &&
                    crosses_z_seam(spec.kind, point[1], 2))
                {
                    add_error(local_corner, jet.dxz,
                              expected[static_cast<int>(Operator::dxz)],
                              component, point);
                }
            }
        }
    }

    require(local_direction_mask == 3,
            "normal KO path did not request exactly x and z strides");
    result.hidden_direction_accesses = 0;
    for (int op = 0; op < operator_count; ++op)
    {
        result.global[static_cast<std::size_t>(op)] =
            reduce_metric(local_global[static_cast<std::size_t>(op)]);
        result.seam[static_cast<std::size_t>(op)] =
            reduce_metric(local_seam[static_cast<std::size_t>(op)]);
    }
    result.corner = reduce_metric(local_corner);

    for (int component = 0; component < components; ++component)
    {
#ifdef CH_MPI
        MPI_Allreduce(
            &local_reference[static_cast<std::size_t>(component)],
            &result.maximum_reference[static_cast<std::size_t>(component)],
            1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
#else
        result.maximum_reference[static_cast<std::size_t>(component)] =
            local_reference[static_cast<std::size_t>(component)];
#endif
        require(result.maximum_reference[static_cast<std::size_t>(component)] >
                    1.0e-10,
                "manufactured field failed all-slot nonzero coverage");
    }

    require(result.valid_overwrites == 0,
            "ghost lifecycle overwrote valid cells");
    require(result.wrong_owner_ghosts == 0,
            "ghost value did not match its intended owner");
    require(result.unfilled_ghosts == 0,
            "required ghost cell remained unfilled");
    for (const auto &metric : result.global)
    {
        require(metric.maximum <= comparison_tolerance,
                "global manufactured operator mismatch");
    }
    for (const auto &metric : result.seam)
    {
        if (metric.count > 0)
        {
            require(metric.maximum <= comparison_tolerance,
                    "seam manufactured operator mismatch");
        }
    }
    if (spec.kind == LayoutKind::both)
    {
        require(result.corner.count > 0 &&
                    result.corner.maximum <= comparison_tolerance,
                "internal x-z corner mixed derivative mismatch");
    }
    return result;
}

void print_metric(const char *scope, const char *layout,
                  const char *name, const ReducedMetric &metric)
{
    if (procID() != 0)
    {
        return;
    }
    std::cout << "M1C_OPERATOR layout=" << layout
              << " scope=" << scope << " operator=" << name
              << " count=" << metric.count
              << " Linf=" << metric.maximum
              << " weighted_L2=" << metric.weighted_l2;
    if (metric.worst_key >= 0)
    {
        const int component = metric.worst_key / (nx * nz);
        const int cell = metric.worst_key % (nx * nz);
        std::cout << " worst_component="
                  << Production::UserVariables::variable_names[
                         static_cast<std::size_t>(component)]
                  << " worst_cell=(" << cell / nz << "," << cell % nz
                  << ")";
    }
    else
    {
        std::cout << " worst_component=NA worst_cell=NA";
    }
    std::cout << '\n';
}

void print_layout(const LayoutSpec &spec, const LayoutResult &result)
{
    if (procID() == 0)
    {
        std::cout << "M1C_LAYOUT name=" << spec.name
                  << " global_Nx=" << nx << " global_Nz=" << nz
                  << " dx=" << dx << " dz=" << dx
                  << " ghost_depth=" << ghost_depth
                  << " boxes=" << spec.boxes.size()
                  << " ranks=" << numProc()
                  << " low_radial_boxes=" << result.low_radial_ghost_boxes
                  << " high_radial_boxes=" << result.high_radial_ghost_boxes
                  << " outer_rhs_boxes=" << result.outer_rhs_boxes
                  << " framework_ghost_cells="
                  << result.framework_ghost_cells
                  << " radial_ghost_cells=" << result.radial_ghost_cells
                  << " radial_periodic_corners="
                  << result.radial_periodic_corner_cells
                  << " valid_overwrites=" << result.valid_overwrites
                  << " owner_mismatches=" << result.wrong_owner_ghosts
                  << " unfilled_ghosts=" << result.unfilled_ghosts
                  << " hidden_direction_accesses="
                  << result.hidden_direction_accesses << '\n';
        for (std::size_t index = 0; index < spec.boxes.size(); ++index)
        {
            const Box &box = spec.boxes[index];
            std::cout << "M1C_BOX layout=" << spec.name
                      << " id=" << index
                      << " owner=" << spec.processors[index]
                      << " lo=(" << box.smallEnd(0) << ","
                      << box.smallEnd(1) << ") hi=(" << box.bigEnd(0)
                      << "," << box.bigEnd(1) << ")"
                      << " radial_cells=" << box.size(0)
                      << " compact_cells=" << box.size(1)
                      << " inner_physical=" << (box.smallEnd(0) == 0)
                      << " outer_physical="
                      << (box.bigEnd(0) == nx - 1) << '\n';
        }
    }
    for (int op = 0; op < operator_count; ++op)
    {
        print_metric("global_stencil_safe", spec.name, operator_names[op],
                     result.global[static_cast<std::size_t>(op)]);
        print_metric("seam_only", spec.name, operator_names[op],
                     result.seam[static_cast<std::size_t>(op)]);
    }
    if (spec.kind == LayoutKind::both)
    {
        print_metric("internal_corner", spec.name, "dxz", result.corner);
    }
    if (procID() == 0)
    {
        std::cout << "M1C_COVERAGE layout=" << spec.name
                  << " components=18/18 scalar_even_slots=13"
                  << " one_z_slots=5 fourth_order_stencil=LOCKED"
                  << " measured_convergence=NOT_APPLICABLE_SINGLE_RESOLUTION"
                  << " expected_derivative_order=4"
                  << " ko_stencil_points=7\n";
    }
}

class AuditedDerivativePath
{
  public:
    explicit AuditedDerivativePath(const double spacing)
        : m_derivatives(spacing)
    {
    }

    double diff1(const Cell<double> &cell, const int component,
                 const int direction)
    {
        if (direction < 0 || direction >= CH_SPACEDIM)
        {
            throw std::domain_error(
                "fixture seam derivative rejected hidden grid direction");
        }
        m_mask |= 1 << direction;
        const auto &pointers = cell.get_box_pointers();
        return m_derivatives.diff1<double>(
            pointers.m_in_ptr[component], cell.get_in_index(),
            pointers.m_in_stride[direction]);
    }

    int mask() const { return m_mask; }

  private:
    FourthOrderDerivatives m_derivatives;
    int m_mask = 0;
};

struct OmitLastComponent
{
    static constexpr int component_count = components - 1;
    static constexpr double stencil_sign = 1.0;
};

void run_mutations()
{
    require(numProc() == 1 || numProc() == 2,
            "mutation runner requires one or two ranks");

    const LayoutSpec compact =
        make_layout(LayoutKind::compact, numProc() == 2);
    DisjointBoxLayout compact_layout(
        compact.boxes, compact.processors, compact.domain);
    GRLevelData unexchanged;
    define_state(unexchanged, compact_layout);
    int local_omitted_exchange = 0;
    for (DataIterator dit = unexchanged.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = compact_layout[dit];
        const int selected_z =
            valid.smallEnd(1) == 0 ? valid.bigEnd(1)
                                   : valid.smallEnd(1);
        const IntVect selected = iv(10, selected_z);
        FArrayBox scratch(valid, components);
        scratch.setVal(0.0);
        BoxPointers pointers(unexchanged[dit], scratch);
        Cell<double> cell(selected, pointers);
        const auto input =
            Live::make_pointwise_input(cell, dx, coordinate_offset);
        if (!std::isfinite(input.derivatives[0].dz))
        {
            local_omitted_exchange = 1;
        }
        break;
    }
    const bool exchange_mutation_detected =
        reduce_sum(local_omitted_exchange) > 0;
    require(exchange_mutation_detected,
            "omitted exchange mutation was not detected");

    GRLevelData exchanged;
    define_state(exchanged, compact_layout);
    exchanged.exchange(Interval(0, components - 1));
    int local_ko_omission = 0;
    int local_hidden_direction = 0;
    for (DataIterator dit = exchanged.dataIterator(); dit.ok(); ++dit)
    {
        const Box valid = compact_layout[dit];
        const IntVect selected =
            iv(10, valid.smallEnd(1) == 0 ? valid.bigEnd(1)
                                          : valid.smallEnd(1));
        FArrayBox scratch(valid, components);
        scratch.setVal(0.0);
        BoxPointers pointers(exchanged[dit], scratch);
        Cell<double> cell(selected, pointers);
        int production_mask = 0;
        int omitted_mask = 0;
        const auto production =
            projected_ko(cell, true, true, production_mask);
        const auto omitted =
            projected_ko<OmitLastComponent>(
                cell, true, true, omitted_mask);
        if (std::abs(production.back()) > 1.0e-12 &&
            omitted.back() != production.back())
        {
            local_ko_omission = 1;
        }

        AuditedDerivativePath audit(dx);
        static_cast<void>(audit.diff1(cell, 0, 0));
        static_cast<void>(audit.diff1(cell, 0, 1));
        require(audit.mask() == 3,
                "normal derivative audit missed a grid direction");
        try
        {
            static_cast<void>(audit.diff1(cell, 0, 2));
        }
        catch (const std::domain_error &)
        {
            local_hidden_direction = 1;
        }
        break;
    }
    require(reduce_sum(local_ko_omission) > 0,
            "omitted distributed KO component mutation was not detected");
    require(reduce_sum(local_hidden_direction) > 0,
            "hidden grid direction mutation was not detected");

    ProblemDomain fake_domain(Box(iv(8, 0), iv(31, 31)));
    fake_domain.setPeriodic(0, false);
    fake_domain.setPeriodic(1, true);
    Vector<Box> fake_boxes;
    fake_boxes.push_back(Box(iv(8, 0), iv(15, 31)));
    fake_boxes.push_back(Box(iv(24, 0), iv(31, 31)));
    Vector<int> fake_processors(2, 0);
    if (numProc() == 2)
    {
        fake_processors[1] = 1;
    }
    DisjointBoxLayout fake_layout(
        fake_boxes, fake_processors, fake_domain);
    GRLevelData false_boundary;
    define_state(false_boundary, fake_layout);
    false_boundary.exchange(Interval(0, components - 1));
    BlackStringPerturbativeRadialBoundary::fill_solution_ghosts(
        false_boundary, fake_domain, r0, dx, coordinate_offset, Side::Lo,
        Interval(0, components - 1));
    int local_false_boundary = 0;
    for (DataIterator dit = false_boundary.dataIterator(); dit.ok(); ++dit)
    {
        if (fake_layout[dit].smallEnd(0) != 8)
        {
            continue;
        }
        const IntVect ghost = iv(7, 7);
        const double actual =
            false_boundary[dit](ghost, Production::c_chi);
        const double correct_internal =
            manufactured_value(ghost, Production::c_chi);
        if (std::isfinite(actual) &&
            std::abs(actual - correct_internal) > 1.0e-10)
        {
            local_false_boundary = 1;
        }
    }
    require(reduce_sum(local_false_boundary) > 0,
            "internal radial seam physical-boundary mutation was not detected");

    if (procID() == 0)
    {
        std::cout
            << "M1C_MUTATIONS internal_radial_seam_as_physical=REJECTED"
            << " omit_exchange=REJECTED"
            << " omit_KO_component=REJECTED"
            << " hidden_grid_direction=REJECTED"
            << " mutation_path=REAL_EXCHANGE_DERIVATIVE_KO_RADIAL_FILL\n"
            << "M1C_MUTATION_RUNNER_PASS\n";
    }
}

LayoutKind parse_layout(const std::string &name)
{
    if (name == "z")
    {
        return LayoutKind::compact;
    }
    if (name == "x")
    {
        return LayoutKind::radial;
    }
    if (name == "both")
    {
        return LayoutKind::both;
    }
    fail("unknown layout argument " + name);
}

void run_one_layout(const LayoutKind kind, const bool distributed)
{
    const LayoutSpec spec = make_layout(kind, distributed);
    const LayoutResult result = evaluate_layout(spec);
    print_layout(spec, result);
    if (procID() == 0)
    {
        std::cout << "M1C_LAYOUT_PASS name=" << spec.name << '\n';
    }
}

} // namespace

int main(int argc, char *argv[])
{
#ifdef CH_MPI
    MPI_Init(&argc, &argv);
#endif
    if (argc != 2)
    {
        fail("usage: fixture serial|z|x|both|mutations");
    }
    std::cout << std::scientific << std::setprecision(12);
    const std::string mode = argv[1];
    if (mode == "serial")
    {
        require(numProc() == 1, "serial reference requires one process");
        run_one_layout(LayoutKind::compact, false);
        run_one_layout(LayoutKind::radial, false);
        run_one_layout(LayoutKind::both, false);
        run_mutations();
        if (procID() == 0)
        {
            std::cout << "M1C_SERIAL_REFERENCE_PASS layouts=3"
                      << " combined_mutation_runner=PASS\n";
        }
    }
    else if (mode == "mutations")
    {
        run_mutations();
    }
    else
    {
        const LayoutKind kind = parse_layout(mode);
        const int expected_ranks =
            kind == LayoutKind::both ? 4 : 2;
        require(static_cast<int>(numProc()) == expected_ranks,
                "distributed layout rank count mismatch");
        run_one_layout(kind, true);
    }
    if (procID() == 0)
    {
        std::cout << "M1C_FIXTURE_PASS mode=" << mode << '\n';
    }
#ifdef CH_MPI
    MPI_Finalize();
#endif
    return 0;
}
