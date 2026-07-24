#ifndef BLACKSTRINGCELLSTORAGE_HPP
#define BLACKSTRINGCELLSTORAGE_HPP

#include "BlackStringReducedVars.hpp"
#include "Cell.hpp"

#include <cstddef>
#include <stdexcept>

namespace BlackStringCellStorage
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

using Variables = Reduced::Variables<double>;
using Storage = Reduced::Storage<double>;

static_assert(Production::NUM_VARS == 18);
static_assert(Production::metadata[Production::c_hww].storage_multiplicity ==
              1);
static_assert(Production::metadata[Production::c_Aww].storage_multiplicity ==
              1);
static_assert(!Production::uses_stock_visible_y_as_hidden);

inline void require_input_component_count(const Cell<double> &cell)
{
    if (cell.get_num_in_vars() != Production::NUM_VARS)
    {
        throw std::invalid_argument(
            "black-string Cell input must contain exactly 18 components");
    }
}

inline void require_output_component_count(const Cell<double> &cell)
{
    if (cell.get_num_out_vars() != Production::NUM_VARS)
    {
        throw std::invalid_argument(
            "black-string Cell output must contain exactly 18 components");
    }
}

inline Variables load(const Cell<double> &cell)
{
    require_input_component_count(cell);

    Storage storage{};
    for (int index = 0; index < Production::NUM_VARS; ++index)
    {
        const auto slot = static_cast<Production::VariableSlot>(index);
        storage[static_cast<std::size_t>(slot)] =
            cell.load_vars(static_cast<int>(slot));
    }
    return Reduced::load(storage);
}

inline void store(const Cell<double> &cell, const Variables &vars)
{
    require_output_component_count(cell);

    for (int index = 0; index < Production::NUM_VARS; ++index)
    {
        const auto slot = static_cast<Production::VariableSlot>(index);
        cell.store_vars(Reduced::component(vars, slot),
                        static_cast<int>(slot));
    }
}

} // namespace BlackStringCellStorage

#endif /* BLACKSTRINGCELLSTORAGE_HPP */
