#ifndef BLACKSTRINGCHOMBOPARAMETERS_HPP
#define BLACKSTRINGCHOMBOPARAMETERS_HPP

// ChomboParameters owns grid/domain arrays sized with CH_SPACEDIM. Locked
// GRChombo uses the tensor-wide FOR macro in that infrastructure header,
// which overruns those arrays for the target 2/4/4 build. Compile that one
// header with grid-dimensional loops, then immediately restore target-d=4
// tensor loops for the black-string physics path.
#include "DimensionDefinitions.hpp"

#undef FOR1
#undef FOR2
#undef FOR3
#undef FOR4
#undef FOR5
#define FOR1(IDX) for (int IDX = 0; IDX < CH_SPACEDIM; ++IDX)
#define FOR2(IDX1, IDX2) FOR1(IDX1) FOR1(IDX2)
#define FOR3(IDX1, IDX2, IDX3) FOR2(IDX1, IDX2) FOR1(IDX3)
#define FOR4(IDX1, IDX2, IDX3, IDX4) FOR2(IDX1, IDX2) FOR2(IDX3, IDX4)
#define FOR5(IDX1, IDX2, IDX3, IDX4, IDX5)                                  \
    FOR4(IDX1, IDX2, IDX3, IDX4) FOR1(IDX5)

// Use the locked dependency path explicitly. This header is repository-owned
// and fixed at this location; avoiding the non-standard include_next extension
// keeps the project-owned adapter clean under -Wpedantic -Werror.
#include "../../external/GRChombo/Source/GRChomboCore/ChomboParameters.hpp"

#undef FOR1
#undef FOR2
#undef FOR3
#undef FOR4
#undef FOR5
#define FOR1(IDX) for (int IDX = 0; IDX < DEFAULT_TENSOR_DIM; ++IDX)
#define FOR2(IDX1, IDX2) FOR1(IDX1) FOR1(IDX2)
#define FOR3(IDX1, IDX2, IDX3) FOR2(IDX1, IDX2) FOR1(IDX3)
#define FOR4(IDX1, IDX2, IDX3, IDX4) FOR2(IDX1, IDX2) FOR2(IDX3, IDX4)
#define FOR5(IDX1, IDX2, IDX3, IDX4, IDX5)                                  \
    FOR4(IDX1, IDX2, IDX3, IDX4) FOR1(IDX5)

#endif /* BLACKSTRINGCHOMBOPARAMETERS_HPP */
