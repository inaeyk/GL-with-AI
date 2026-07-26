#ifndef COORDINATES_HPP_
#define COORDINATES_HPP_

#include "BlackStringCoordinates.hpp"
#include "DimensionDefinitions.hpp"
#include "IntVect.H"
#include "simd.hpp"

#include <array>
#include <cmath>

#include "UsingNamespace.H"

// Target-specific replacement for the stock coordinate wrapper, whose
// preprocessor branches do not include CH_SPACEDIM=2/DEFAULT_TENSOR_DIM=4.
// Direction 0 is x, direction 1 is z, and both hidden directions are suppressed.
template <class data_t> class Coordinates
{
  public:
    data_t x;
    double y = 0.0;
    double z = 0.0;
    std::array<double, CH_SPACEDIM> m_center;

    Coordinates(const IntVect integer_coords, const double dx,
                const std::array<double, CH_SPACEDIM> center = {0.0, 0.0})
        : m_center(center)
    {
        compute_coord(x, integer_coords[0], dx, center[0]);
        compute_coord(z, integer_coords[1], dx, center[1]);
    }

    static void compute_coord(double &out, const int position, const double dx,
                              const double center_distance = 0.0)
    {
        out = BlackStringCoordinates::cell_centered<double>(
            position, dx, center_distance);
    }

    static void compute_coord(simd<double> &out, const int position,
                              const double dx,
                              const double center_distance = 0.0)
    {
        double values[simd_traits<double>::simd_len];
        for (int lane = 0; lane < simd_traits<double>::simd_len; ++lane)
        {
            values[lane] = BlackStringCoordinates::cell_centered<double>(
                position + lane, dx, center_distance);
        }
        out = simd<double>::load(values);
    }

    data_t get_radius() const
    {
        return simd_max(sqrt(x * x + z * z), 1.0e-6);
    }

    static data_t
    get_radius(const IntVect integer_coords, const double dx,
               const std::array<double, CH_SPACEDIM> center = {0.0, 0.0})
    {
        data_t x_value;
        double z_value;
        compute_coord(x_value, integer_coords[0], dx, center[0]);
        compute_coord(z_value, integer_coords[1], dx, center[1]);
        return simd_max(sqrt(x_value * x_value + z_value * z_value),
                        1.0e-6);
    }
};

#endif /* COORDINATES_HPP_ */
