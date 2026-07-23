#include "BlackStringReducedVars.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

#ifdef USERVARIABLES_HPP
#error "reduced Vars test must not import the legacy 27-slot UserVariables.hpp"
#endif

namespace
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

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

using Storage = Reduced::Storage<double>;
using Variables = Reduced::Variables<double>;

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(Production::NUM_VARS == 18);
static_assert(!has_visible_y<Reduced::ReducedVisibleVector<double>>::value);
static_assert(!has_visible_yy<Reduced::ReducedSymmetricTensor<double>>::value);

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

Storage distinct_storage()
{
    Storage storage{};
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        storage[static_cast<std::size_t>(slot)] =
            1000.0 + 17.0 * static_cast<double>(slot);
    }
    return storage;
}

bool storage_equal(const Storage &left, const Storage &right)
{
    return std::equal(left.begin(), left.end(), right.begin());
}

bool mapping_policy_is_valid(const bool aliases_visible_y,
                             const int stored_hidden_representatives,
                             const int physical_hidden_copies)
{
    return !aliases_visible_y && stored_hidden_representatives == 1 &&
           physical_hidden_copies == 2;
}

void check_exact_load_store()
{
    const Storage input = distinct_storage();
    const Variables vars = Reduced::load(input);

    for (int index = 0; index < Production::NUM_VARS; ++index)
    {
        const auto slot = static_cast<Production::VariableSlot>(index);
        require(Reduced::component(vars, slot) ==
                    input[static_cast<std::size_t>(index)],
                "const component load mismatch at slot " +
                    std::to_string(index));
    }

    require(storage_equal(Reduced::store(vars), input),
            "exact 18-slot load/store round trip");

    for (int changed = 0; changed < Production::NUM_VARS; ++changed)
    {
        Variables mutable_vars = Reduced::load(input);
        const auto changed_slot =
            static_cast<Production::VariableSlot>(changed);
        const double replacement = -5000.0 - static_cast<double>(changed);
        Reduced::component(mutable_vars, changed_slot) = replacement;
        const Storage output = Reduced::store(mutable_vars);

        for (int observed = 0; observed < Production::NUM_VARS; ++observed)
        {
            const double expected =
                observed == changed
                    ? replacement
                    : input[static_cast<std::size_t>(observed)];
            require(output[static_cast<std::size_t>(observed)] == expected,
                    "mutable write leaked from slot " +
                        std::to_string(changed) + " to " +
                        std::to_string(observed));
        }
    }
}

void check_hidden_representative_storage()
{
    Variables vars = Reduced::load(distinct_storage());
    constexpr double hidden_marker = -9876.5;
    vars.physical.h.ww = hidden_marker;
    const Storage output = Reduced::store(vars);

    const int marker_count =
        static_cast<int>(std::count(output.begin(), output.end(),
                                    hidden_marker));
    require(marker_count == 1, "hww representative must occupy one slot");
    require(output[Production::c_hww] == hidden_marker,
            "hww representative must use c_hww");
    require(Production::metadata[Production::c_hww].storage_multiplicity == 1 &&
                Production::metadata[Production::c_hww].hidden_multiplicity ==
                    2,
            "hww storage/physical multiplicity metadata");
    require(Production::metadata[Production::c_Aww].storage_multiplicity == 1 &&
                Production::metadata[Production::c_Aww].hidden_multiplicity ==
                    2,
            "Aww storage/physical multiplicity metadata");

    const Reduced::ReducedSymmetricTensor<double> diagonal{1.5, 91.0, -0.5,
                                                           2.0};
    require(Reduced::physical_diagonal_trace(diagonal) == 5.0,
            "physical diagonal trace must count ww twice and ignore xz");
}

void check_grouping_and_parity()
{
    int physical_count = 0;
    int gauge_count = 0;
    int physical_one_z_count = 0;
    for (const auto &entry : Production::metadata)
    {
        require(!Production::uses_stock_visible_y_as_hidden,
                "visible-y storage must remain forbidden");
        if (entry.category == Production::Category::physical)
        {
            ++physical_count;
            if (entry.parity == Production::ParityClass::one_z_opposite)
            {
                require(entry.name == "hxz" || entry.name == "Axz" ||
                            entry.name == "GammaZ",
                        "unexpected physical one-z variable");
                ++physical_one_z_count;
            }
        }
        else
        {
            ++gauge_count;
        }
    }
    require(physical_count == 13 && gauge_count == 5,
            "Vars physical/gauge grouping must be 13+5");
    require(physical_one_z_count == 3,
            "Vars physical one-z grouping must be hxz/Axz/GammaZ");
}

void check_mutations()
{
    const Storage input = distinct_storage();
    const Variables vars = Reduced::load(input);

    Storage swapped = Reduced::store(vars);
    std::swap(swapped[Production::c_hxx], swapped[Production::c_hxz]);
    require(!storage_equal(swapped, input),
            "swapped load/store slots mutation must fail round trip");
    std::cout << "PASS mutation rejected: swapped hxx/hxz store slots\n";

    require(mapping_policy_is_valid(
                Production::uses_stock_visible_y_as_hidden,
                Production::stored_hidden_representatives,
                Production::physical_trace_hidden_copies),
            "production reduced mapping policy");
    require(!mapping_policy_is_valid(
                true, Production::stored_hidden_representatives,
                Production::physical_trace_hidden_copies),
            "visible-y aliasing mutation must be rejected");
    std::cout << "PASS mutation rejected: visible-y hidden alias\n";

    const Reduced::ReducedSymmetricTensor<double> trace_sample{
        -0.75, 0.0, -0.25, 0.5};
    const double correct_trace =
        Reduced::physical_diagonal_trace(trace_sample);
    const double multiplicity_one_trace =
        trace_sample.xx + trace_sample.zz + trace_sample.ww;
    require(correct_trace == 0.0 && multiplicity_one_trace != 0.0,
            "hidden multiplicity-one trace mutation must be detected");
    std::cout << "PASS mutation rejected: hidden trace multiplicity one\n";
}

} // namespace

int main()
{
    std::cout << "Black-string reduced Vars mapping fixture\n";
    check_exact_load_store();
    check_hidden_representative_storage();
    check_grouping_and_parity();
    check_mutations();
    std::cout << "All reduced Vars mapping checks passed.\n";
    return 0;
}
