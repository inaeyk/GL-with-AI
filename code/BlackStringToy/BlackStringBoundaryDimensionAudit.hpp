#ifndef BLACKSTRINGBOUNDARYDIMENSIONAUDIT_HPP_
#define BLACKSTRINGBOUNDARYDIMENSIONAUDIT_HPP_

#include "DimensionDefinitions.hpp"

#include <atomic>
#include <stdexcept>

// Test-only counters for proving that the black-string BoundaryConditions
// object was built through the grid-dimensional loop policy. Production builds
// do not define BLACKSTRING_BOUNDARY_DIMENSION_AUDIT and incur no counters.
namespace BlackStringBoundaryDimensionAudit
{
#ifdef BLACKSTRING_BOUNDARY_DIMENSION_AUDIT
inline std::atomic<int> loop_visits{0};
inline std::atomic<int> maximum_direction{-1};
inline std::atomic<int> out_of_range_visits{0};
#ifdef BLACKSTRING_BOUNDARY_REJECT_NON_GRID_DIRECTION
inline std::atomic<bool> reject_non_grid_direction{false};
#endif

inline void reset()
{
    loop_visits.store(0);
    maximum_direction.store(-1);
    out_of_range_visits.store(0);
#ifdef BLACKSTRING_BOUNDARY_REJECT_NON_GRID_DIRECTION
    reject_non_grid_direction.store(false);
#endif
}

inline void arm_non_grid_rejection()
{
#ifdef BLACKSTRING_BOUNDARY_REJECT_NON_GRID_DIRECTION
    reject_non_grid_direction.store(true);
#endif
}

inline bool observe(const int direction)
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
#ifdef BLACKSTRING_BOUNDARY_REJECT_NON_GRID_DIRECTION
        if (reject_non_grid_direction.load())
        {
            throw std::out_of_range(
                "legacy boundary tensor loop attempted a non-grid direction");
        }
#endif
        // Observe the legacy mutation without executing its out-of-range body
        // before the real level-zero setup path arms hard rejection.
        return false;
    }
    return true;
}
#endif
} // namespace BlackStringBoundaryDimensionAudit

#endif /* BLACKSTRINGBOUNDARYDIMENSIONAUDIT_HPP_ */
