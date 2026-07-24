#include "FArrayBox.H"

#include "BlackStringCellStorage.hpp"
#include "BlackStringGPPointwiseInitialData.hpp"
#include "BoxPointers.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

#ifdef USERVARIABLES_HPP
#error "storage seam fixture must not import legacy 27-slot UserVariables.hpp"
#endif

namespace
{
namespace CellStorage = BlackStringCellStorage;
namespace GP = BlackStringGPPointwiseInitialData;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

using Storage = Reduced::Storage<double>;
using Variables = Reduced::Variables<double>;

constexpr double input_sentinel = -71001.0;
constexpr double output_sentinel = -72002.0;
constexpr double absolute_tolerance = 2.0e-14;
constexpr double relative_tolerance = 2.0e-13;

template <class type_t, class = void> struct has_visible_y : std::false_type
{
};

template <class type_t>
struct has_visible_y<type_t,
                     std::void_t<decltype(std::declval<type_t &>().y)>>
    : std::true_type
{
};

template <class type_t, class = void> struct has_visible_yy : std::false_type
{
};

template <class type_t>
struct has_visible_yy<type_t,
                      std::void_t<decltype(std::declval<type_t &>().yy)>>
    : std::true_type
{
};

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(Production::NUM_VARS == 18);
static_assert(Production::c_chi == 0);
static_assert(Production::c_hww == 4);
static_assert(Production::c_Aww == 9);
static_assert(Production::c_Theta == 10);
static_assert(Production::c_lapse == 13);
static_assert(Production::c_Bz == 17);
static_assert(!Production::uses_stock_visible_y_as_hidden);
static_assert(
    !has_visible_y<Reduced::ReducedVisibleVector<double>>::value);
static_assert(
    !has_visible_yy<Reduced::ReducedSymmetricTensor<double>>::value);

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "FAIL " << message << "\n";
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

void require_close(const double actual, const double expected,
                   const std::string &label)
{
    const double error = std::abs(actual - expected);
    const double allowance =
        absolute_tolerance +
        relative_tolerance * std::max(std::abs(actual), std::abs(expected));
    require(error <= allowance,
            label + " actual=" + std::to_string(actual) +
                " expected=" + std::to_string(expected));
}

template <class function_t>
void require_invalid_argument(function_t function, const std::string &label)
{
    bool rejected = false;
    try
    {
        function();
    }
    catch (const std::invalid_argument &)
    {
        rejected = true;
    }
    require(rejected, label + " must be rejected");
}

IntVect make_iv(const int x, const int z)
{
    return IntVect(D_DECL(x, z, 0));
}

Box make_test_box()
{
    return Box(make_iv(-2, 3), make_iv(1, 5));
}

Storage distinct_storage()
{
    Storage values{};
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        values[static_cast<std::size_t>(slot)] =
            1000.0 + 17.0 * static_cast<double>(slot);
    }
    return values;
}

void write_point(FArrayBox &fab, const IntVect &point,
                 const Storage &values)
{
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        fab(point, slot) = values[static_cast<std::size_t>(slot)];
    }
}

Storage read_point(const FArrayBox &fab, const IntVect &point)
{
    Storage values{};
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        values[static_cast<std::size_t>(slot)] = fab(point, slot);
    }
    return values;
}

bool storage_equal(const Storage &left, const Storage &right)
{
    return std::equal(left.begin(), left.end(), right.begin());
}

bool point_is_value(const FArrayBox &fab, const IntVect &point,
                    const double expected)
{
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        if (fab(point, slot) != expected)
        {
            return false;
        }
    }
    return true;
}

class RealFArrayBoxPoint
{
  public:
    RealFArrayBoxPoint(const FArrayBox &input, FArrayBox &output,
                       const IntVect &point)
        : m_box_pointers(input, output), m_cell(point, m_box_pointers)
    {
        require(input.box().contains(point),
                "input FArrayBox must contain selected IntVect");
        require(output.box().contains(point),
                "output FArrayBox must contain selected IntVect");
    }

    Variables load() const { return CellStorage::load(m_cell); }

    void store(const Variables &vars) const
    {
        CellStorage::store(m_cell, vars);
    }

    const Cell<double> &cell() const { return m_cell; }

  private:
    BoxPointers m_box_pointers;
    Cell<double> m_cell;
};

void check_exact_round_trip_and_ownership()
{
    const Box box = make_test_box();
    const IntVect target = make_iv(-1, 4);
    const IntVect neighbor = make_iv(-2, 4);
    const IntVect far_point = make_iv(1, 5);
    const Storage input_values = distinct_storage();

    FArrayBox input(box, Production::NUM_VARS);
    FArrayBox output(box, Production::NUM_VARS);
    input.setVal(input_sentinel);
    output.setVal(output_sentinel);
    write_point(input, target, input_values);

    const FArrayBox &const_input = input;
    RealFArrayBoxPoint point(const_input, output, target);
    require(input.nComp() == Production::NUM_VARS,
            "real input FArrayBox component count");
    require(output.nComp() == Production::NUM_VARS,
            "real output FArrayBox component count");
    require(point.cell().get_int_vect() == target,
            "Cell IntVect must equal selected nonzero point");
    require(point.cell().get_num_in_vars() == Production::NUM_VARS &&
                point.cell().get_num_out_vars() == Production::NUM_VARS,
            "Cell component counts must be exactly 18");

    const Variables vars = point.load();
    for (int index = 0; index < Production::NUM_VARS; ++index)
    {
        const auto slot = static_cast<Production::VariableSlot>(index);
        require(Reduced::component(vars, slot) ==
                    input_values[static_cast<std::size_t>(index)],
                "Cell load slot ordering at " + std::to_string(index));
    }

    point.store(vars);
    require(storage_equal(read_point(output, target), input_values),
            "FArrayBox to reduced Vars to FArrayBox round trip");
    require(point_is_value(output, neighbor, output_sentinel),
            "neighbor must remain unchanged");
    require(point_is_value(output, far_point, output_sentinel),
            "far point must remain unchanged");

    for (int changed = 0; changed < Production::NUM_VARS; ++changed)
    {
        output.setVal(output_sentinel);
        Variables mutated = point.load();
        const auto slot =
            static_cast<Production::VariableSlot>(changed);
        const double replacement = -9000.0 - static_cast<double>(changed);
        Reduced::component(mutated, slot) = replacement;
        point.store(mutated);

        for (int observed = 0; observed < Production::NUM_VARS; ++observed)
        {
            const double expected =
                observed == changed
                    ? replacement
                    : input_values[static_cast<std::size_t>(observed)];
            require(output(target, observed) == expected,
                    "reduced mutation leaked from slot " +
                        std::to_string(changed) + " to " +
                        std::to_string(observed));
        }
        require(point_is_value(output, neighbor, output_sentinel),
                "mutated store must not touch neighbor");
    }

    output.setVal(output_sentinel);
    Variables hidden = point.load();
    hidden.physical.h.ww = -8111.25;
    hidden.physical.A.ww = -8222.5;
    point.store(hidden);
    const Storage hidden_output = read_point(output, target);
    require(hidden_output[Production::c_hww] == -8111.25,
            "hww must use c_hww");
    require(hidden_output[Production::c_Aww] == -8222.5,
            "Aww must use c_Aww");
    require(std::count(hidden_output.begin(), hidden_output.end(), -8111.25) ==
                1,
            "hww must be written exactly once");
    require(std::count(hidden_output.begin(), hidden_output.end(), -8222.5) ==
                1,
            "Aww must be written exactly once");
    require(Production::metadata[Production::c_hww].storage_multiplicity ==
                    1 &&
                Production::metadata[Production::c_Aww]
                        .storage_multiplicity == 1,
            "hidden representatives have storage multiplicity one");

    std::cout << "PASS real 18-slot Cell/FArrayBox round trip and ownership\n";
}

using DestinationMap = std::array<int, Production::NUM_VARS>;

DestinationMap exact_destination_map()
{
    DestinationMap mapping{};
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        mapping[static_cast<std::size_t>(slot)] = slot;
    }
    return mapping;
}

void candidate_store(FArrayBox &output, const IntVect &point,
                     const Variables &vars, const DestinationMap &mapping)
{
    for (int source = 0; source < Production::NUM_VARS; ++source)
    {
        const int destination = mapping[static_cast<std::size_t>(source)];
        if (destination < 0)
        {
            continue;
        }
        require(destination < Production::NUM_VARS,
                "test mutation destination must remain in range");
        const auto slot =
            static_cast<Production::VariableSlot>(source);
        output(point, destination) = Reduced::component(vars, slot);
    }
}

void require_candidate_rejected(const Variables &vars,
                                const DestinationMap &mapping,
                                const std::string &label)
{
    const Box box = make_test_box();
    const IntVect target = make_iv(-1, 4);
    FArrayBox output(box, Production::NUM_VARS);
    output.setVal(output_sentinel);
    candidate_store(output, target, vars, mapping);
    require(!storage_equal(read_point(output, target), Reduced::store(vars)),
            label + " mutation must be rejected");
    std::cout << "PASS mutation rejected: " << label << "\n";
}

void check_mapping_mutations()
{
    const Variables vars = Reduced::load(distinct_storage());

    {
        DestinationMap mapping = exact_destination_map();
        std::swap(mapping[Production::c_hxx], mapping[Production::c_hxz]);
        require_candidate_rejected(vars, mapping, "swapped slots");
    }
    {
        DestinationMap mapping = exact_destination_map();
        mapping[Production::c_hxz] = Production::c_hxx;
        require_candidate_rejected(vars, mapping,
                                   "duplicate write and omitted destination");
    }
    {
        DestinationMap mapping = exact_destination_map();
        mapping[Production::c_Theta] = -1;
        require_candidate_rejected(vars, mapping, "omitted slot");
    }
    {
        DestinationMap mapping = exact_destination_map();
        for (int source = 0; source < Production::NUM_VARS; ++source)
        {
            mapping[static_cast<std::size_t>(source)] =
                (source + 1) % Production::NUM_VARS;
        }
        require_candidate_rejected(vars, mapping,
                                   "off-by-one component indexing");
    }
    {
        DestinationMap mapping = exact_destination_map();
        mapping[Production::c_hww] = Production::c_hzz;
        require_candidate_rejected(vars, mapping,
                                   "hww mapped to visible tensor component");
    }
    {
        const Box box = make_test_box();
        const IntVect target = make_iv(-1, 4);
        FArrayBox output(box, Production::NUM_VARS);
        output.setVal(output_sentinel);
        candidate_store(output, target, vars, exact_destination_map());
        output(target, Production::c_hzz) = vars.physical.h.ww;
        output(target, Production::c_Azz) = vars.physical.A.ww;
        require(!storage_equal(read_point(output, target),
                               Reduced::store(vars)),
                "hidden multiplicity-two storage mutation must be rejected");
        std::cout
            << "PASS mutation rejected: hidden representatives written twice\n";
    }
    {
        const Box box = make_test_box();
        const IntVect target = make_iv(-1, 4);
        const IntVect neighbor = make_iv(-2, 4);
        FArrayBox output(box, Production::NUM_VARS);
        output.setVal(output_sentinel);
        candidate_store(output, neighbor, vars, exact_destination_map());
        require(point_is_value(output, target, output_sentinel) &&
                    !point_is_value(output, neighbor, output_sentinel),
                "neighbor write mutation must be detected");
        std::cout << "PASS mutation rejected: neighboring-cell write\n";
    }

    {
        const Box box = make_test_box();
        const IntVect target = make_iv(-1, 4);
        FArrayBox old_input(box, 27);
        FArrayBox output(box, Production::NUM_VARS);
        BoxPointers pointers(old_input, output);
        Cell<double> cell(target, pointers);
        require_invalid_argument(
            [&cell] { (void)CellStorage::load(cell); },
            "legacy 27-slot input");
        std::cout << "PASS mutation rejected: legacy 27-slot enum count\n";
    }
    {
        const Box box = make_test_box();
        const IntVect target = make_iv(-1, 4);
        FArrayBox input(box, Production::NUM_VARS);
        FArrayBox off_by_one_output(box, Production::NUM_VARS + 1);
        BoxPointers pointers(input, off_by_one_output);
        Cell<double> cell(target, pointers);
        require_invalid_argument(
            [&cell, &vars] { CellStorage::store(cell, vars); },
            "off-by-one output component count");
        std::cout
            << "PASS mutation rejected: off-by-one FArrayBox component count\n";
    }
}

void check_gp_case(const double r0, const double x)
{
    const Box box = make_test_box();
    const IntVect target = make_iv(0, 4);
    const IntVect neighbor = make_iv(1, 4);
    FArrayBox input(box, Production::NUM_VARS);
    FArrayBox output(box, Production::NUM_VARS);
    FArrayBox scratch(box, Production::NUM_VARS);
    input.setVal(input_sentinel);
    output.setVal(output_sentinel);
    scratch.setVal(output_sentinel);

    const Variables oracle_vars = GP::make_pointwise_vars(r0, x);
    const Storage oracle_state = GP::make_pointwise_state(r0, x);
    RealFArrayBoxPoint write_access(input, output, target);
    write_access.store(oracle_vars);

    RealFArrayBoxPoint read_access(output, scratch, target);
    const Variables loaded = read_access.load();
    const Storage loaded_state = Reduced::store(loaded);
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        require_close(loaded_state[static_cast<std::size_t>(slot)],
                      oracle_state[static_cast<std::size_t>(slot)],
                      "GP stored slot " + std::to_string(slot));
    }

    require(loaded.physical.h.ww == 1.0, "GP stored hww=1");
    const double visible_determinant =
        loaded.physical.h.xx * loaded.physical.h.zz -
        loaded.physical.h.xz * loaded.physical.h.xz;
    const double conformal_determinant =
        visible_determinant * loaded.physical.h.ww *
        loaded.physical.h.ww;
    const double weighted_trace =
        Reduced::physical_diagonal_trace(loaded.physical.A);
    const double lambda = std::sqrt(r0 / (x * x * x));
    require_close(conformal_determinant, 1.0,
                  "GP stored conformal determinant");
    require_close(weighted_trace, 0.0, "GP stored weighted A trace");
    require_close(loaded.physical.A.xx + 0.25 * loaded.physical.K,
                  -0.5 * lambda, "GP stored Kxx");
    require_close(loaded.physical.A.zz + 0.25 * loaded.physical.K, 0.0,
                  "GP stored Kzz");
    require_close(loaded.physical.A.ww + 0.25 * loaded.physical.K,
                  lambda, "GP stored Kww");
    require_close(loaded.gauge.lapse, 1.0, "GP stored lapse");
    require_close(loaded.gauge.shift.x, std::sqrt(r0 / x),
                  "GP stored shiftX");
    require_close(loaded.gauge.shift.z, 0.0, "GP stored shiftZ");
    require_close(loaded.gauge.B.x, 0.0, "GP stored Bx");
    require_close(loaded.gauge.B.z, 0.0, "GP stored Bz");
    require(point_is_value(output, neighbor, output_sentinel),
            "GP store must not modify neighbor");

    std::cout << "PASS GP Cell/FArrayBox storage r0=" << r0
              << " x=" << x << "\n";
}

void check_gp_storage_round_trip()
{
    check_gp_case(1.0, 2.0);
    check_gp_case(2.5, 0.75);
    check_gp_case(0.4, 3.2);
}

} // namespace

int main()
{
    std::cout << "Black-string real Cell/FArrayBox storage seam fixture\n";
    check_exact_round_trip_and_ownership();
    check_mapping_mutations();
    check_gp_storage_round_trip();
    std::cout << "All Cell/FArrayBox storage seam checks passed.\n";
    return 0;
}
