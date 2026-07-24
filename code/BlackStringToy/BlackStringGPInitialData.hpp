#ifndef BLACKSTRINGGPINITIALDATA_HPP
#define BLACKSTRINGGPINITIALDATA_HPP

#include "BlackStringCellStorage.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"

#include <array>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace BlackStringGPInitialData
{
namespace GP = BlackStringGPPointwiseInitialData;
namespace Storage = BlackStringCellStorage;

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);

inline constexpr int radial_direction = 0;
inline constexpr int compact_direction = 1;

struct PhysicalCoordinates
{
    double x;
    double z;
};

// GRChombo's coordinate convention is
//     coordinate = (integer_index + 1/2) * dx - center_offset.
// Its stock Coordinates constructor has no CH_SPACEDIM=2,
// DEFAULT_TENSOR_DIM=4 branch, so this target-specific adapter owns the exact
// same cell-centering rule for the reduced (x,z) grid. The offset is supplied
// per AMR level through the same m_dx/m_p.center-style interface used by
// GRChombo compute classes; it is not a box-local x_min.
inline PhysicalCoordinates
cell_centered_coordinates(const IntVect &integer_coordinates, const double dx,
                          const std::array<double, CH_SPACEDIM> &origin)
{
    if (!std::isfinite(dx) || !(dx > 0.0))
    {
        throw std::domain_error(
            "black-string GP grid coordinates require finite dx>0");
    }
    for (const double value : origin)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "black-string GP coordinate origin must be finite");
        }
    }

    return {(integer_coordinates[radial_direction] + 0.5) * dx -
                origin[radial_direction],
            (integer_coordinates[compact_direction] + 0.5) * dx -
                origin[compact_direction]};
}

struct DefaultStoragePolicy
{
    void store(const Cell<double> current_cell,
               const Storage::Variables &vars) const
    {
        Storage::store(current_cell, vars);
    }
};

template <class storage_policy_t> class BasicCompute
{
  public:
    using storage_policy_type = storage_policy_t;

    BasicCompute(const double r0, const double dx,
                 const std::array<double, CH_SPACEDIM> &origin,
                 storage_policy_t storage_policy)
        : m_r0(r0), m_dx(dx), m_origin(origin),
          m_storage_policy(std::move(storage_policy))
    {
        if (!std::isfinite(m_r0) || !(m_r0 > 0.0))
        {
            throw std::domain_error(
                "black-string GP BoxLoop compute requires finite r0>0");
        }
        // Validate level-wide coordinate inputs before entering BoxLoops.
        (void)cell_centered_coordinates(IntVect::Zero, m_dx, m_origin);
    }

    void compute(const Cell<double> current_cell) const
    {
        const PhysicalCoordinates coordinates = cell_centered_coordinates(
            current_cell.get_int_vect(), m_dx, m_origin);
        if (!std::isfinite(coordinates.x) || !(coordinates.x > 0.0))
        {
            throw std::domain_error(
                "black-string GP BoxLoop cell requires physical x>0");
        }

        // The pointwise initializer remains the sole owner of GP physics.
        // The compact coordinate is intentionally absent from this call.
        const Storage::Variables vars =
            GP::make_pointwise_vars(m_r0, coordinates.x);
        m_storage_policy.store(current_cell, vars);
    }

  private:
    double m_r0;
    double m_dx;
    std::array<double, CH_SPACEDIM> m_origin;
    storage_policy_t m_storage_policy;
};

using Compute = BasicCompute<DefaultStoragePolicy>;

inline Compute
make_compute(const double r0, const double dx,
             const std::array<double, CH_SPACEDIM> &origin)
{
    return Compute(r0, dx, origin, DefaultStoragePolicy{});
}

template <class storage_policy_t>
inline BasicCompute<storage_policy_t>
make_compute_with_storage_policy(
    const double r0, const double dx,
    const std::array<double, CH_SPACEDIM> &origin,
    storage_policy_t storage_policy)
{
    return BasicCompute<storage_policy_t>(
        r0, dx, origin, std::move(storage_policy));
}

} // namespace BlackStringGPInitialData

#endif /* BLACKSTRINGGPINITIALDATA_HPP */
