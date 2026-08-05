#ifndef BLACKSTRINGPERTURBATIVERADIALBOUNDARY_HPP_
#define BLACKSTRINGPERTURBATIVERADIALBOUNDARY_HPP_

#include "BlackStringAlgebraicReconstruction.hpp"
#include "BlackStringCoordinates.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"
#include "BlackStringProductionVariables.hpp"
#include "BlackStringReducedCharacteristics.hpp"
#include "BlackStringReducedVars.hpp"
#include "BoxIterator.H"
#include "GRLevelData.hpp"
#include "Interval.H"
#include "MayDay.H"
#include "ProblemDomain.H"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

namespace BlackStringBoundaryImplementation
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace Characteristics = BlackStringReducedCharacteristics;

// Production M2-B reduced-characteristic boundary. The 23-state transform is
// applied only at the two global radial faces. It reconstructs 16 independent
// stored fields through three ghost layers and then reconstructs hww/Aww
// algebraically. Internal Chombo/MPI seams remain exchange-owned.
class BlackStringPerturbativeRadialBoundary
{
  public:
    static constexpr int radial_direction = 0;
    static constexpr int source_points = 5;
    static constexpr int supported_ghosts = 3;
    static constexpr int independent_slots = 16;
    using StoredState = Reduced::Storage<double>;
    using SourceStates = std::array<StoredState, source_points>;
    using GhostStates = std::array<StoredState, supported_ghosts>;
    using DeltaStates = std::array<StoredState, source_points>;

    struct FaceData
    {
        DeltaStates delta{};
        Characteristics::State characteristic{};
        Characteristics::State speeds{};
        Characteristics::State reconstructed{};
    };

    static FaceData evaluate_characteristic_face(
        const SourceStates &sources, const double face_x, const int normal,
        const double r0, const double dx)
    {
        if (!std::isfinite(face_x) || !(face_x > 0.0) ||
            !std::isfinite(r0) || !(r0 > 0.0) || !std::isfinite(dx) ||
            !(dx > 0.0) || (normal != -1 && normal != 1))
        {
            throw std::domain_error("M2-B CHARACTERISTIC_BOUNDARY_FAILURE");
        }

        FaceData result;
        for (int source = 0; source < source_points; ++source)
        {
            const double x =
                face_x - static_cast<double>(normal) *
                             (static_cast<double>(source) + 0.5) * dx;
            const StoredState gp =
                BlackStringGPPointwiseInitialData::make_pointwise_state(r0, x);
            for (int slot = 0; slot < Production::NUM_VARS; ++slot)
            {
                result.delta[static_cast<std::size_t>(source)]
                            [static_cast<std::size_t>(slot)] =
                    sources[static_cast<std::size_t>(source)]
                           [static_cast<std::size_t>(slot)] -
                    gp[static_cast<std::size_t>(slot)];
            }
        }

        const auto face_value = [&](const int slot) {
            return (315.0 * result.delta[0][static_cast<std::size_t>(slot)] -
                    420.0 * result.delta[1][static_cast<std::size_t>(slot)] +
                    378.0 * result.delta[2][static_cast<std::size_t>(slot)] -
                    180.0 * result.delta[3][static_cast<std::size_t>(slot)] +
                    35.0 * result.delta[4][static_cast<std::size_t>(slot)]) /
                   128.0;
        };
        const auto outward_derivative = [&](const int slot) {
            const double dy =
                -31.0 * result.delta[0][static_cast<std::size_t>(slot)] / 8.0 +
                229.0 * result.delta[1][static_cast<std::size_t>(slot)] / 24.0 -
                75.0 * result.delta[2][static_cast<std::size_t>(slot)] / 8.0 +
                37.0 * result.delta[3][static_cast<std::size_t>(slot)] / 8.0 -
                11.0 * result.delta[4][static_cast<std::size_t>(slot)] / 12.0;
            return -dy / dx;
        };

        Characteristics::State normal_state{};
        normal_state[Characteristics::qchi] =
            outward_derivative(Production::c_chi);
        normal_state[Characteristics::dxx] =
            outward_derivative(Production::c_hxx);
        normal_state[Characteristics::dxz] =
            outward_derivative(Production::c_hxz);
        normal_state[Characteristics::dzz] =
            outward_derivative(Production::c_hzz);
        normal_state[Characteristics::K] = face_value(Production::c_K);
        normal_state[Characteristics::Axx] = face_value(Production::c_Axx);
        normal_state[Characteristics::Axz] = face_value(Production::c_Axz);
        normal_state[Characteristics::Azz] = face_value(Production::c_Azz);
        normal_state[Characteristics::Theta] =
            face_value(Production::c_Theta);
        normal_state[Characteristics::GammaX] =
            face_value(Production::c_GammaX);
        normal_state[Characteristics::GammaZ] =
            face_value(Production::c_GammaZ);
        normal_state[Characteristics::qalpha] =
            outward_derivative(Production::c_lapse);
        normal_state[Characteristics::rx] =
            outward_derivative(Production::c_shiftX);
        normal_state[Characteristics::rz] =
            outward_derivative(Production::c_shiftZ);
        normal_state[Characteristics::Bx] = face_value(Production::c_Bx);
        normal_state[Characteristics::Bz] = face_value(Production::c_Bz);
        normal_state[Characteristics::chi] = face_value(Production::c_chi);
        normal_state[Characteristics::hxx] = face_value(Production::c_hxx);
        normal_state[Characteristics::hxz] = face_value(Production::c_hxz);
        normal_state[Characteristics::hzz] = face_value(Production::c_hzz);
        normal_state[Characteristics::lapse] =
            face_value(Production::c_lapse);
        normal_state[Characteristics::shiftX] =
            face_value(Production::c_shiftX);
        normal_state[Characteristics::shiftZ] =
            face_value(Production::c_shiftZ);

        const double b = std::sqrt(r0 / face_x);
        result.characteristic =
            Characteristics::to_characteristics(b, normal_state);
        result.speeds = Characteristics::normal_speeds(b, normal);
        for (int field = 0; field < Characteristics::state_size; ++field)
        {
            if (result.speeds[static_cast<std::size_t>(field)] < -5.0e-13)
            {
                result.characteristic[static_cast<std::size_t>(field)] = 0.0;
            }
        }
        result.reconstructed =
            Characteristics::from_characteristics(b, result.characteristic);
        return result;
    }

    static double incoming_characteristic_residual(
        const SourceStates &sources, const double face_x, const int normal,
        const double r0, const double dx)
    {
        const FaceData face =
            evaluate_characteristic_face(sources, face_x, normal, r0, dx);
        double maximum = 0.0;
        for (int field = 0; field < Characteristics::state_size; ++field)
        {
            if (face.speeds[static_cast<std::size_t>(field)] < -5.0e-13)
            {
                maximum =
                    std::max(maximum,
                             std::abs(face.characteristic[
                                 static_cast<std::size_t>(field)]));
            }
        }
        return maximum;
    }

    static GhostStates fill_characteristic_line(
        const SourceStates &sources, const double face_x, const int normal,
        const double r0, const double dx)
    {
        const FaceData face_data =
            evaluate_characteristic_face(sources, face_x, normal, r0, dx);
        const DeltaStates &delta = face_data.delta;
        const Characteristics::State &face = face_data.reconstructed;

        GhostStates ghosts{};
        constexpr std::array<int, 7> primitive_slots = {
            Production::c_chi,   Production::c_hxx,    Production::c_hxz,
            Production::c_hzz,  Production::c_lapse,  Production::c_shiftX,
            Production::c_shiftZ};
        constexpr std::array<int, 7> primitive_values = {
            Characteristics::chi,   Characteristics::hxx,
            Characteristics::hxz,   Characteristics::hzz,
            Characteristics::lapse, Characteristics::shiftX,
            Characteristics::shiftZ};
        constexpr std::array<int, 7> primitive_derivatives = {
            Characteristics::qchi, Characteristics::dxx,
            Characteristics::dxz,  Characteristics::dzz,
            Characteristics::qalpha, Characteristics::rx,
            Characteristics::rz};
        constexpr std::array<int, 9> dynamical_slots = {
            Production::c_K,      Production::c_Axx,
            Production::c_Axz,    Production::c_Azz,
            Production::c_Theta,  Production::c_GammaX,
            Production::c_GammaZ, Production::c_Bx,
            Production::c_Bz};
        constexpr std::array<int, 9> dynamical_values = {
            Characteristics::K,      Characteristics::Axx,
            Characteristics::Axz,    Characteristics::Azz,
            Characteristics::Theta,  Characteristics::GammaX,
            Characteristics::GammaZ, Characteristics::Bx,
            Characteristics::Bz};

        for (int layer = 0; layer < supported_ghosts; ++layer)
        {
            const double ghost_x =
                face_x + static_cast<double>(normal) *
                             (static_cast<double>(layer) + 0.5) * dx;
            ghosts[static_cast<std::size_t>(layer)] =
                BlackStringGPPointwiseInitialData::make_pointwise_state(
                    r0, ghost_x);
        }
        for (int component = 0; component < 7; ++component)
        {
            const int slot = primitive_slots[static_cast<std::size_t>(component)];
            const double u_face =
                face[static_cast<std::size_t>(
                    primitive_values[static_cast<std::size_t>(component)])];
            const double s_face =
                -dx * face[static_cast<std::size_t>(primitive_derivatives[
                          static_cast<std::size_t>(component)])];
            const std::array<double, 4> u = {
                delta[0][static_cast<std::size_t>(slot)],
                delta[1][static_cast<std::size_t>(slot)],
                delta[2][static_cast<std::size_t>(slot)],
                delta[3][static_cast<std::size_t>(slot)]};
            const std::array<double, 3> values = {
                -9088.0 * u_face / 3675.0 - 64.0 * s_face / 35.0 +
                    4.0 * u[0] - 2.0 * u[1] / 3.0 + 4.0 * u[2] / 25.0 -
                    u[3] / 49.0,
                -18048.0 * u_face / 245.0 - 192.0 * s_face / 7.0 +
                    90.0 * u[0] - 20.0 * u[1] + 27.0 * u[2] / 5.0 -
                    36.0 * u[3] / 49.0,
                -19840.0 * u_face / 49.0 - 960.0 * s_face / 7.0 +
                    500.0 * u[0] - 125.0 * u[1] + 36.0 * u[2] -
                    250.0 * u[3] / 49.0};
            for (int layer = 0; layer < supported_ghosts; ++layer)
            {
                ghosts[static_cast<std::size_t>(layer)]
                      [static_cast<std::size_t>(slot)] +=
                    values[static_cast<std::size_t>(layer)];
            }
        }
        for (int component = 0; component < 9; ++component)
        {
            const int slot =
                dynamical_slots[static_cast<std::size_t>(component)];
            const double w_face =
                face[static_cast<std::size_t>(
                    dynamical_values[static_cast<std::size_t>(component)])];
            const std::array<double, 4> w = {
                delta[0][static_cast<std::size_t>(slot)],
                delta[1][static_cast<std::size_t>(slot)],
                delta[2][static_cast<std::size_t>(slot)],
                delta[3][static_cast<std::size_t>(slot)]};
            const std::array<double, 3> values = {
                128.0 * w_face / 35.0 - 4.0 * w[0] + 2.0 * w[1] -
                    4.0 * w[2] / 5.0 + w[3] / 7.0,
                128.0 * w_face / 7.0 - 30.0 * w[0] + 20.0 * w[1] -
                    9.0 * w[2] + 12.0 * w[3] / 7.0,
                384.0 * w_face / 7.0 - 100.0 * w[0] + 75.0 * w[1] -
                    36.0 * w[2] + 50.0 * w[3] / 7.0};
            for (int layer = 0; layer < supported_ghosts; ++layer)
            {
                ghosts[static_cast<std::size_t>(layer)]
                      [static_cast<std::size_t>(slot)] +=
                    values[static_cast<std::size_t>(layer)];
            }
        }
        for (auto &ghost : ghosts)
        {
            auto vars = Reduced::load(ghost);
            BlackStringAlgebraicReconstruction::reconstruct(vars);
            ghost = Reduced::store(vars);
        }
        return ghosts;
    }

    static void validate_locked_gate(const double r0, const double dx,
                                     const double inner_face,
                                     const double outer_face)
    {
        Characteristics::validate_locked_transform_gate(r0);
        for (const auto face_and_normal :
             {std::array<double, 2>{inner_face, -1.0},
              std::array<double, 2>{outer_face, 1.0}})
        {
            const double face = face_and_normal[0];
            const int normal = static_cast<int>(face_and_normal[1]);
            SourceStates sources{};
            for (int source = 0; source < source_points; ++source)
            {
                const double x =
                    face - static_cast<double>(normal) *
                               (static_cast<double>(source) + 0.5) * dx;
                sources[static_cast<std::size_t>(source)] =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        r0, x);
            }
            const GhostStates ghosts =
                fill_characteristic_line(sources, face, normal, r0, dx);
            for (int layer = 0; layer < supported_ghosts; ++layer)
            {
                const double x =
                    face + static_cast<double>(normal) *
                               (static_cast<double>(layer) + 0.5) * dx;
                const StoredState expected =
                    BlackStringGPPointwiseInitialData::make_pointwise_state(
                        r0, x);
                for (int slot = 0; slot < Production::NUM_VARS; ++slot)
                {
                    const double scale =
                        1.0 + std::abs(expected[static_cast<std::size_t>(slot)]);
                    if (!std::isfinite(
                            ghosts[static_cast<std::size_t>(layer)]
                                  [static_cast<std::size_t>(slot)]) ||
                        std::abs(ghosts[static_cast<std::size_t>(layer)]
                                       [static_cast<std::size_t>(slot)] -
                                 expected[static_cast<std::size_t>(slot)]) /
                                scale >
                            2.0e-10)
                    {
                        throw std::domain_error(
                            "M2-B CHARACTERISTIC_GP_FIXED_POINT_FAILURE");
                    }
                }
                const auto vars = Reduced::load(
                    ghosts[static_cast<std::size_t>(layer)]);
                if (std::abs(BlackStringAlgebraicReconstruction::
                                 determinant_residual(vars)) >
                        5.0e-13 ||
                    std::abs(BlackStringAlgebraicReconstruction::
                                 weighted_trace_residual(vars)) >
                        5.0e-13 / r0)
                {
                    throw std::domain_error(
                        "M2-B CHARACTERISTIC_GHOST_RECONSTRUCTION_FAILURE");
                }
            }
        }
    }

    static void fill_solution_ghosts(
        GRLevelData &state, const ProblemDomain &problem_domain,
        const double r0, const double dx,
        const std::array<double, CH_SPACEDIM> &coordinate_offset,
        const Side::LoHiSide side, const Interval &components)
    {
        (void)components;
        const Box domain = problem_domain.domainBox();
        const int normal = side == Side::Lo ? -1 : 1;
        const int boundary_index =
            side == Side::Lo ? domain.smallEnd(radial_direction)
                             : domain.bigEnd(radial_direction);
        const double first_center =
            BlackStringCoordinates::cell_centered<double>(
                boundary_index, dx, coordinate_offset[radial_direction]);
        const double face_x =
            first_center + static_cast<double>(normal) * 0.5 * dx;
        const DataIterator iterator = state.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            FArrayBox &fab = state[data_index];
            Box ghosts = fab.box();
            if (side == Side::Lo)
            {
                if (ghosts.smallEnd(radial_direction) >= boundary_index)
                {
                    continue;
                }
                ghosts.setBig(radial_direction, boundary_index - 1);
            }
            else
            {
                if (ghosts.bigEnd(radial_direction) <= boundary_index)
                {
                    continue;
                }
                ghosts.setSmall(radial_direction, boundary_index + 1);
            }
            ghosts &= fab.box();
            if (ghosts.isEmpty())
            {
                continue;
            }
            Box line = ghosts;
            line.setSmall(radial_direction,
                          side == Side::Lo ? ghosts.bigEnd(radial_direction)
                                           : ghosts.smallEnd(radial_direction));
            line.setBig(radial_direction,
                        side == Side::Lo ? ghosts.bigEnd(radial_direction)
                                         : ghosts.smallEnd(radial_direction));
            for (BoxIterator bit(line); bit.ok(); ++bit)
            {
                const IntVect anchor = bit();
                SourceStates source_states{};
                for (int source = 0; source < source_points; ++source)
                {
                    IntVect point = anchor;
                    point[radial_direction] =
                        boundary_index - normal * source;
                    if (!fab.box().contains(point))
                    {
                        MayDay::Error(
                            "characteristic boundary needs five interior cells");
                    }
                    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
                    {
                        source_states[static_cast<std::size_t>(source)]
                                     [static_cast<std::size_t>(slot)] =
                            fab(point, slot);
                    }
                }
                const GhostStates values = fill_characteristic_line(
                    source_states, face_x, normal, r0, dx);
                for (int layer = 0; layer < supported_ghosts; ++layer)
                {
                    IntVect point = anchor;
                    point[radial_direction] =
                        boundary_index + normal * (layer + 1);
                    if (!fab.box().contains(point))
                    {
                        continue;
                    }
                    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
                    {
                        fab(point, slot) =
                            values[static_cast<std::size_t>(layer)]
                                  [static_cast<std::size_t>(slot)];
                    }
                }
            }
        }
    }

    static void apply_dependent_surface_rhs(
        const GRLevelData &state, GRLevelData &rhs,
        const ProblemDomain &problem_domain)
    {
        const Box domain = problem_domain.domainBox();
        const DataIterator iterator = rhs.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &state_fab = state[data_index];
            FArrayBox &rhs_fab = rhs[data_index];
            Box valid = rhs.disjointBoxLayout()[data_index] & domain;
            if (valid.isEmpty())
            {
                continue;
            }
            for (const int boundary : {domain.smallEnd(radial_direction),
                                       domain.bigEnd(radial_direction)})
            {
                if (valid.smallEnd(radial_direction) > boundary ||
                    valid.bigEnd(radial_direction) < boundary)
                {
                    continue;
                }
                Box surface = valid;
                surface.setSmall(radial_direction, boundary);
                surface.setBig(radial_direction, boundary);
                for (BoxIterator bit(surface); bit.ok(); ++bit)
                {
                    const IntVect point = bit();
                    StoredState state_storage{};
                    StoredState rhs_storage{};
                    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
                    {
                        state_storage[static_cast<std::size_t>(slot)] =
                            state_fab(point, slot);
                        rhs_storage[static_cast<std::size_t>(slot)] =
                            rhs_fab(point, slot);
                    }
                    const auto state_vars = Reduced::load(state_storage);
                    auto rhs_vars = Reduced::load(rhs_storage);
                    BlackStringAlgebraicReconstruction::
                        reconstruct_dependent_rhs(state_vars, rhs_vars);
                    rhs_fab(point, Production::c_hww) =
                        rhs_vars.physical.h.ww;
                    rhs_fab(point, Production::c_Aww) =
                        rhs_vars.physical.A.ww;
                }
            }
        }
    }
};

} // namespace BlackStringBoundaryImplementation

using BlackStringPerturbativeRadialBoundary =
    BlackStringBoundaryImplementation::BlackStringPerturbativeRadialBoundary;

#endif /* BLACKSTRINGPERTURBATIVERADIALBOUNDARY_HPP_ */
