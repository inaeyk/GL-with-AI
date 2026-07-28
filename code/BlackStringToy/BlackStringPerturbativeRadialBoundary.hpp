#ifndef BLACKSTRINGPERTURBATIVERADIALBOUNDARY_HPP_
#define BLACKSTRINGPERTURBATIVERADIALBOUNDARY_HPP_

#include "BlackStringCoordinates.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"
#include "BoxIterator.H"
#include "GRLevelData.hpp"
#include "Interval.H"
#include "MayDay.H"
#include "ProblemDomain.H"

#include <array>
#include <cmath>

// A deliberately inexpensive finite-domain boundary for early perturbative
// runs. It is not the frozen-operator WKB boundary:
//
//   inner: delta U ghosts are fourth-order polynomial extrapolations,
//   outer: dt(delta U) + c [dx(delta U) + delta U / x] = 0,
//
// where delta U = U - U_GP and U_GP is evaluated analytically. The adapter is
// componentwise, preserves the reduced 18-slot storage exactly, and touches
// only a fixed number of radial surface layers.
class BlackStringPerturbativeRadialBoundary
{
  public:
    static constexpr int radial_direction = 0;
    static constexpr int source_points = 5;
    static constexpr int supported_ghosts = 3;

    static void fill_solution_ghosts(
        GRLevelData &state, const ProblemDomain &problem_domain,
        const double r0, const double dx,
        const std::array<double, CH_SPACEDIM> &coordinate_offset,
        const Side::LoHiSide side, const Interval &components)
    {
        const Box domain = problem_domain.domainBox();
        const DataIterator iterator = state.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            FArrayBox &fab = state[data_index];
            Box ghosts = fab.box();
            if (side == Side::Lo)
            {
                ghosts.setBig(radial_direction,
                              domain.smallEnd(radial_direction) - 1);
            }
            else
            {
                ghosts.setSmall(radial_direction,
                                domain.bigEnd(radial_direction) + 1);
            }
            ghosts &= fab.box();
            if (ghosts.isEmpty())
            {
                continue;
            }

            for (BoxIterator bit(ghosts); bit.ok(); ++bit)
            {
                const IntVect ghost = bit();
                const int distance =
                    side == Side::Lo
                        ? domain.smallEnd(radial_direction) -
                              ghost[radial_direction]
                        : ghost[radial_direction] -
                              domain.bigEnd(radial_direction);
                if (distance < 1 || distance > supported_ghosts)
                {
                    MayDay::Error("black-string perturbative boundary supports "
                                  "exactly three radial ghost layers");
                }

                const auto ghost_background =
                    background(ghost, r0, dx, coordinate_offset);
                std::array<std::array<double, NUM_VARS>, source_points>
                    source_background{};
                std::array<IntVect, source_points> sources{};
                for (int source = 0; source < source_points; ++source)
                {
                    sources[static_cast<std::size_t>(source)] = ghost;
                    sources[static_cast<std::size_t>(source)][radial_direction] =
                        side == Side::Lo
                            ? domain.smallEnd(radial_direction) + source
                            : domain.bigEnd(radial_direction) - source;
                    if (!fab.box().contains(
                            sources[static_cast<std::size_t>(source)]))
                    {
                        MayDay::Error(
                            "radial boundary box needs five interior cells");
                    }
                    source_background[static_cast<std::size_t>(source)] =
                        background(sources[static_cast<std::size_t>(source)],
                                   r0, dx, coordinate_offset);
                }

                const auto &weights =
                    extrapolation_weights[static_cast<std::size_t>(distance -
                                                                   1)];
                for (int component = components.begin();
                     component <= components.end(); ++component)
                {
                    double delta = 0.0;
                    for (int source = 0; source < source_points; ++source)
                    {
                        const auto source_index =
                            static_cast<std::size_t>(source);
                        delta +=
                            weights[source_index] *
                            (fab(sources[source_index], component) -
                             source_background[source_index]
                                              [static_cast<std::size_t>(
                                                  component)]);
                    }
                    fab(ghost, component) =
                        ghost_background[static_cast<std::size_t>(component)] +
                        delta;
                }
            }
        }
    }

    static void apply_outer_rhs(
        const GRLevelData &state, GRLevelData &rhs,
        const ProblemDomain &problem_domain, const double r0, const double dx,
        const std::array<double, CH_SPACEDIM> &coordinate_offset,
        const double outgoing_speed, const Interval &components)
    {
        const Box domain = problem_domain.domainBox();
        const int outer_index = domain.bigEnd(radial_direction);
        const DataIterator iterator = rhs.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &state_fab = state[data_index];
            FArrayBox &rhs_fab = rhs[data_index];
            Box surface = rhs_fab.box() & domain;
            if (surface.isEmpty() ||
                surface.bigEnd(radial_direction) != outer_index)
            {
                continue;
            }
            surface.setSmall(radial_direction, outer_index);

            for (BoxIterator bit(surface); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                std::array<IntVect, source_points> sources{};
                std::array<std::array<double, NUM_VARS>, source_points>
                    source_background{};
                for (int source = 0; source < source_points; ++source)
                {
                    sources[static_cast<std::size_t>(source)] = point;
                    sources[static_cast<std::size_t>(source)]
                           [radial_direction] -= source;
                    if (!state_fab.box().contains(
                            sources[static_cast<std::size_t>(source)]))
                    {
                        MayDay::Error(
                            "outer Sommerfeld closure needs five interior cells");
                    }
                    source_background[static_cast<std::size_t>(source)] =
                        background(sources[static_cast<std::size_t>(source)],
                                   r0, dx, coordinate_offset);
                }

                const double x = BlackStringCoordinates::cell_centered<double>(
                    point[radial_direction], dx,
                    coordinate_offset[radial_direction]);
                for (int component = components.begin();
                     component <= components.end(); ++component)
                {
                    std::array<double, source_points> delta{};
                    for (int source = 0; source < source_points; ++source)
                    {
                        const auto source_index =
                            static_cast<std::size_t>(source);
                        delta[source_index] =
                            state_fab(sources[source_index], component) -
                            source_background[source_index]
                                             [static_cast<std::size_t>(
                                                 component)];
                    }
                    rhs_fab(point, component) =
                        outgoing_rhs(delta, x, dx, outgoing_speed);
                }
            }
        }
    }

    static double
    outgoing_rhs(const std::array<double, source_points> &outer_to_inner_delta,
                 const double x, const double dx,
                 const double outgoing_speed)
    {
        return -outgoing_speed *
               (backward_derivative(outer_to_inner_delta, dx) +
                outer_to_inner_delta[0] / x);
    }

    static double backward_derivative(
        const std::array<double, source_points> &outer_to_inner_values,
        const double dx)
    {
        return (25.0 * outer_to_inner_values[0] -
                48.0 * outer_to_inner_values[1] +
                36.0 * outer_to_inner_values[2] -
                16.0 * outer_to_inner_values[3] +
                3.0 * outer_to_inner_values[4]) /
               (12.0 * dx);
    }

  private:
    using State = std::array<double, NUM_VARS>;

    static State
    background(const IntVect &point, const double r0, const double dx,
               const std::array<double, CH_SPACEDIM> &coordinate_offset)
    {
        const double x = BlackStringCoordinates::cell_centered<double>(
            point[radial_direction], dx,
            coordinate_offset[radial_direction]);
        return BlackStringGPPointwiseInitialData::make_pointwise_state(r0, x);
    }

    inline static constexpr
        std::array<std::array<double, source_points>, supported_ghosts>
            extrapolation_weights{{
                {{5.0, -10.0, 10.0, -5.0, 1.0}},
                {{15.0, -40.0, 45.0, -24.0, 5.0}},
                {{35.0, -105.0, 126.0, -70.0, 15.0}},
            }};
};

#endif /* BLACKSTRINGPERTURBATIVERADIALBOUNDARY_HPP_ */
