#ifndef BLACKSTRINGCOORDINATES_HPP
#define BLACKSTRINGCOORDINATES_HPP

#include <cmath>
#include <stdexcept>

namespace BlackStringCoordinates
{
template <class data_t>
inline data_t cell_centered(const int integer_position, const double dx,
                            const double center_offset)
{
    if (!std::isfinite(dx) || !(dx > 0.0) ||
        !std::isfinite(center_offset))
    {
        throw std::domain_error(
            "black-string coordinates require finite dx>0 and offset");
    }
    return (static_cast<double>(integer_position) + 0.5) * dx -
           center_offset;
}
} // namespace BlackStringCoordinates

#endif /* BLACKSTRINGCOORDINATES_HPP */
