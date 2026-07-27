#ifndef STOCK_DIM3_DEFINE_LAGRANGE_HPP_
#define STOCK_DIM3_DEFINE_LAGRANGE_HPP_

#include "DimensionDefinitions.hpp"

// GRAMR::define stores only an optional interpolator pointer. The focused stock
// boundary regression deliberately does not instantiate interpolation.
template <int Order, int N_DIMS = CH_SPACEDIM> class Lagrange;

#endif /* STOCK_DIM3_DEFINE_LAGRANGE_HPP_ */
