#ifndef BLACKSTRINGREDUCEDVARS_HPP
#define BLACKSTRINGREDUCEDVARS_HPP

#include "BlackStringProductionVariables.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace BlackStringReducedVars
{
namespace Production = BlackStringProductionVariables;

template <class data_t> struct ReducedSymmetricTensor
{
    data_t xx;
    data_t xz;
    data_t zz;
    data_t ww;
};

template <class data_t> struct ReducedVisibleVector
{
    data_t x;
    data_t z;
};

template <class data_t> struct PhysicalVariables
{
    data_t chi;
    ReducedSymmetricTensor<data_t> h;
    data_t K;
    ReducedSymmetricTensor<data_t> A;
    data_t Theta;
    ReducedVisibleVector<data_t> Gamma;
};

template <class data_t> struct GaugeVariables
{
    data_t lapse;
    ReducedVisibleVector<data_t> shift;
    ReducedVisibleVector<data_t> B;
};

template <class data_t> struct Variables
{
    PhysicalVariables<data_t> physical;
    GaugeVariables<data_t> gauge;
};

template <class data_t>
using Storage = std::array<data_t, Production::NUM_VARS>;

static_assert(Production::NUM_VARS == 18);
static_assert(Production::stored_hidden_representatives == 1);
static_assert(Production::physical_trace_hidden_copies == 2);
static_assert(!Production::uses_stock_visible_y_as_hidden);

template <class data_t>
data_t &component(Variables<data_t> &vars,
                  const Production::VariableSlot slot)
{
    switch (slot)
    {
    case Production::c_chi:
        return vars.physical.chi;
    case Production::c_hxx:
        return vars.physical.h.xx;
    case Production::c_hxz:
        return vars.physical.h.xz;
    case Production::c_hzz:
        return vars.physical.h.zz;
    case Production::c_hww:
        return vars.physical.h.ww;
    case Production::c_K:
        return vars.physical.K;
    case Production::c_Axx:
        return vars.physical.A.xx;
    case Production::c_Axz:
        return vars.physical.A.xz;
    case Production::c_Azz:
        return vars.physical.A.zz;
    case Production::c_Aww:
        return vars.physical.A.ww;
    case Production::c_Theta:
        return vars.physical.Theta;
    case Production::c_GammaX:
        return vars.physical.Gamma.x;
    case Production::c_GammaZ:
        return vars.physical.Gamma.z;
    case Production::c_lapse:
        return vars.gauge.lapse;
    case Production::c_shiftX:
        return vars.gauge.shift.x;
    case Production::c_shiftZ:
        return vars.gauge.shift.z;
    case Production::c_Bx:
        return vars.gauge.B.x;
    case Production::c_Bz:
        return vars.gauge.B.z;
    case Production::NUM_VARS:
        break;
    }
    throw std::out_of_range("invalid black-string production variable slot");
}

template <class data_t>
const data_t &component(const Variables<data_t> &vars,
                        const Production::VariableSlot slot)
{
    switch (slot)
    {
    case Production::c_chi:
        return vars.physical.chi;
    case Production::c_hxx:
        return vars.physical.h.xx;
    case Production::c_hxz:
        return vars.physical.h.xz;
    case Production::c_hzz:
        return vars.physical.h.zz;
    case Production::c_hww:
        return vars.physical.h.ww;
    case Production::c_K:
        return vars.physical.K;
    case Production::c_Axx:
        return vars.physical.A.xx;
    case Production::c_Axz:
        return vars.physical.A.xz;
    case Production::c_Azz:
        return vars.physical.A.zz;
    case Production::c_Aww:
        return vars.physical.A.ww;
    case Production::c_Theta:
        return vars.physical.Theta;
    case Production::c_GammaX:
        return vars.physical.Gamma.x;
    case Production::c_GammaZ:
        return vars.physical.Gamma.z;
    case Production::c_lapse:
        return vars.gauge.lapse;
    case Production::c_shiftX:
        return vars.gauge.shift.x;
    case Production::c_shiftZ:
        return vars.gauge.shift.z;
    case Production::c_Bx:
        return vars.gauge.B.x;
    case Production::c_Bz:
        return vars.gauge.B.z;
    case Production::NUM_VARS:
        break;
    }
    throw std::out_of_range("invalid black-string production variable slot");
}

template <class data_t>
Variables<data_t> load(const Storage<data_t> &storage)
{
    Variables<data_t> vars{};
    for (int index = 0; index < Production::NUM_VARS; ++index)
    {
        const auto slot =
            static_cast<Production::VariableSlot>(index);
        component(vars, slot) = storage[static_cast<std::size_t>(index)];
    }
    return vars;
}

template <class data_t>
void store(const Variables<data_t> &vars, Storage<data_t> &storage)
{
    for (int index = 0; index < Production::NUM_VARS; ++index)
    {
        const auto slot =
            static_cast<Production::VariableSlot>(index);
        storage[static_cast<std::size_t>(index)] = component(vars, slot);
    }
}

template <class data_t>
Storage<data_t> store(const Variables<data_t> &vars)
{
    Storage<data_t> storage{};
    store(vars, storage);
    return storage;
}

template <class data_t>
data_t physical_diagonal_trace(
    const ReducedSymmetricTensor<data_t> &tensor)
{
    return tensor.xx + tensor.zz +
           static_cast<data_t>(Production::physical_trace_hidden_copies) *
               tensor.ww;
}

// Future GRChombo wrappers will use load/store at the Cell/FArrayBox boundary
// and translate only the visible x,z families into stock CCZ4 calculations.
// This adapter replaces ADMConformalVars::enum_mapping,
// BSSNVars::enum_mapping, CCZ4Vars::enum_mapping, and the corresponding
// VarsTools::define_*_enum_mapping calls for the black-string state. It does
// not own Chombo storage, BoxLoops, derivatives, RHS formulas, or cleanup.

} // namespace BlackStringReducedVars

#endif /* BLACKSTRINGREDUCEDVARS_HPP */
