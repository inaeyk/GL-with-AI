#include "FArrayBox.H"
#include "CCZ4Geometry.hpp"
#include "Cell.hpp"
#include "DimensionDefinitions.hpp"
#include "Tensor.hpp"
#include "parstream.H"

#include "BlackStringReducedVars.hpp"

#include <type_traits>

namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(Production::NUM_VARS == 18);
static_assert(
    std::tuple_size<Reduced::Storage<double>>::value ==
    static_cast<std::size_t>(Production::NUM_VARS));

int main()
{
    FArrayBox *fab_type_probe = nullptr;
    Cell<double> *cell_type_probe = nullptr;
    Tensor<1, double> *tensor_type_probe = nullptr;
    Reduced::Storage<double> storage{};
    auto vars = Reduced::load(storage);
    vars.physical.h.ww = 1.0;
    const auto round_trip = Reduced::store(vars);

    return (fab_type_probe == nullptr && cell_type_probe == nullptr &&
            tensor_type_probe == nullptr &&
            round_trip[Production::c_hww] == 1.0)
               ? 0
               : 1;
}
