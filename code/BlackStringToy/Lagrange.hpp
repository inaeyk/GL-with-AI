#ifndef LAGRANGE_HPP_
#define LAGRANGE_HPP_

#include "DimensionDefinitions.hpp"

// Stage 4AO-D-E1 deliberately does not instantiate AMR interpolation,
// extraction, horizon finding, or other interpolation clients. GRAMR stores
// only an optional pointer to this type, so a forward declaration keeps that
// deferred surface out of the isolated DIM2/target-d=4 core build.
template <int Order, int N_DIMS = CH_SPACEDIM> class Lagrange;

#endif /* LAGRANGE_HPP_ */
