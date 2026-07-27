#ifndef BLACKSTRINGBOUNDARYDIMENSIONAUDIT_HPP_
#define BLACKSTRINGBOUNDARYDIMENSIONAUDIT_HPP_

#include "DimensionDefinitions.hpp"

#include <atomic>

// Test-only counters for proving that the black-string BoundaryConditions
// object was built through the grid-dimensional loop policy. Production builds
// do not define BLACKSTRING_BOUNDARY_DIMENSION_AUDIT and incur no counters.
namespace BlackStringBoundaryDimensionAudit
{
#ifdef BLACKSTRING_BOUNDARY_DIMENSION_AUDIT
inline std::atomic<int> loop_visits{0};
inline std::atomic<int> maximum_direction{-1};
inline std::atomic<int> out_of_range_visits{0};

inline void reset()
{
    loop_visits.store(0);
    maximum_direction.store(-1);
    out_of_range_visits.store(0);
}

inline void observe(const int direction)
{
    loop_visits.fetch_add(1);
    int maximum = maximum_direction.load();
    while (maximum < direction &&
           !maximum_direction.compare_exchange_weak(maximum, direction))
    {
    }
    if (direction < 0 || direction >= CH_SPACEDIM)
    {
        out_of_range_visits.fetch_add(1);
    }
}
#endif
} // namespace BlackStringBoundaryDimensionAudit

#endif /* BLACKSTRINGBOUNDARYDIMENSIONAUDIT_HPP_ */
