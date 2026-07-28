#ifndef BLACKSTRINGBOUNDARYDIMENSIONSCOPE_HPP_
#define BLACKSTRINGBOUNDARYDIMENSIONSCOPE_HPP_

#include "DimensionDefinitions.hpp"

// This header is force-included only while compiling the black-string-owned
// BoundaryConditions.cpp object. BoundaryConditions operates on Chombo grid
// coordinates, so each of its FOR loops is bounded by CH_SPACEDIM. The target
// tensor dimension remains four in every physics translation unit.
static_assert(CH_SPACEDIM == 2,
              "the black-string boundary adapter is DIM2-only");
static_assert(DEFAULT_TENSOR_DIM == 4,
              "the black-string boundary adapter is target-d=4-only");

#ifdef BLACKSTRING_BOUNDARY_DIMENSION_AUDIT
#include "BlackStringBoundaryDimensionAudit.hpp"
#define BLACKSTRING_BOUNDARY_OBSERVE(IDX)                                   \
    (::BlackStringBoundaryDimensionAudit::observe(IDX))
#else
#define BLACKSTRING_BOUNDARY_OBSERVE(IDX) true
#endif

#undef FOR1
#undef FOR2
#undef FOR3
#undef FOR4
#undef FOR5

#ifdef BLACKSTRING_BOUNDARY_LEGACY_TENSOR_LOOP
#define BLACKSTRING_BOUNDARY_LOOP_DIM DEFAULT_TENSOR_DIM
#else
#define BLACKSTRING_BOUNDARY_LOOP_DIM CH_SPACEDIM
#endif

#define FOR1(IDX)                                                            \
    for (int IDX = 0;                                                        \
         IDX < BLACKSTRING_BOUNDARY_LOOP_DIM &&                              \
         BLACKSTRING_BOUNDARY_OBSERVE(IDX);                                  \
         ++IDX)
#define FOR2(IDX1, IDX2) FOR1(IDX1) FOR1(IDX2)
#define FOR3(IDX1, IDX2, IDX3) FOR2(IDX1, IDX2) FOR1(IDX3)
#define FOR4(IDX1, IDX2, IDX3, IDX4) FOR2(IDX1, IDX2) FOR2(IDX3, IDX4)
#define FOR5(IDX1, IDX2, IDX3, IDX4, IDX5)                                  \
    FOR4(IDX1, IDX2, IDX3, IDX4) FOR1(IDX5)

#endif /* BLACKSTRINGBOUNDARYDIMENSIONSCOPE_HPP_ */
