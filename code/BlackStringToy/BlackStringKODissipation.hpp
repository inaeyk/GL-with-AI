#ifndef BLACKSTRINGKODISSIPATION_HPP
#define BLACKSTRINGKODISSIPATION_HPP

#include "BlackStringReducedVars.hpp"
#include "Cell.hpp"

#include <cstddef>

namespace BlackStringKODissipation
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

inline constexpr int radial_direction = 0;
inline constexpr int compact_direction = 1;
inline constexpr int stencil_radius = 3;

static_assert(CH_SPACEDIM == 2);
static_assert(radial_direction < CH_SPACEDIM);
static_assert(compact_direction < CH_SPACEDIM);
static_assert(Production::NUM_VARS == 18);

struct DefaultPolicy
{
    static constexpr int component_count = Production::NUM_VARS;
    static constexpr double stencil_sign = 1.0;
};

static_assert(DefaultPolicy::component_count == 18);
static_assert(DefaultPolicy::stencil_sign == 1.0);

class CellInputView
{
  public:
    explicit CellInputView(const Cell<double> &cell)
        : m_cell(cell), m_index(cell.get_in_index())
    {
    }

    int index() const { return m_index; }

    int stride(const int direction) const
    {
        return m_cell.get_box_pointers().m_in_stride[direction];
    }

    const double *values(const int component) const
    {
        return m_cell.get_box_pointers().m_in_ptr[component];
    }

  private:
    const Cell<double> &m_cell;
    int m_index;
};

template <class input_t, class policy_t = DefaultPolicy>
inline void add_from_input(Reduced::Variables<double> &rhs,
                           const input_t &input, const double dx,
                           const double sigma)
{
    // Besides preserving the exact pre-D11 RHS when sigma is zero, this
    // prevents any stencil or stride access in check-only zero-KO fixtures.
    if (sigma == 0.0)
    {
        return;
    }

    const int index = input.index();
    const int x_stride = input.stride(radial_direction);
    const int z_stride = input.stride(compact_direction);
    const double factor = policy_t::stencil_sign * sigma / (64.0 * dx);

    for (int component = 0; component < policy_t::component_count;
         ++component)
    {
        const double *values = input.values(component);
        const auto stencil = [values, index](const int stride)
        {
            return values[index - 3 * stride] -
                   6.0 * values[index - 2 * stride] +
                   15.0 * values[index - stride] -
                   20.0 * values[index] +
                   15.0 * values[index + stride] -
                   6.0 * values[index + 2 * stride] +
                   values[index + 3 * stride];
        };
        const auto slot =
            static_cast<Production::VariableSlot>(component);
        Reduced::component(rhs, slot) +=
            factor * (stencil(x_stride) + stencil(z_stride));
    }
}

template <class policy_t = DefaultPolicy>
inline void add(Reduced::Variables<double> &rhs, const Cell<double> &cell,
                const double dx, const double sigma)
{
    const CellInputView input(cell);
    add_from_input<CellInputView, policy_t>(rhs, input, dx, sigma);
}

} // namespace BlackStringKODissipation

#endif /* BLACKSTRINGKODISSIPATION_HPP */
