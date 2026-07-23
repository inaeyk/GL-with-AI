#include "BlackStringProductionVariables.hpp"
#include "Tensor.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#ifdef USERVARIABLES_HPP
#error "the production contract fixture must not import legacy UserVariables.hpp"
#endif

namespace
{
namespace Production = BlackStringProductionVariables;

using Metadata = Production::VariableMetadata;
using Slot = Production::VariableSlot;

constexpr std::array<int, Production::NUM_VARS> expected_slots = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  10, 11, 12, 13, 14, 15, 16, 17};
constexpr std::array<std::string_view, Production::NUM_VARS> expected_names = {
    "chi",    "hxx",    "hxz",    "hzz",   "hww",    "K",
    "Axx",    "Axz",    "Azz",    "Aww",   "Theta",  "GammaX",
    "GammaZ", "lapse",  "shiftX", "shiftZ", "Bx",     "Bz"};
constexpr std::array<Production::Category, Production::NUM_VARS>
    expected_categories = {
        Production::Category::physical, Production::Category::physical,
        Production::Category::physical, Production::Category::physical,
        Production::Category::physical, Production::Category::physical,
        Production::Category::physical, Production::Category::physical,
        Production::Category::physical, Production::Category::physical,
        Production::Category::physical, Production::Category::physical,
        Production::Category::physical, Production::Category::gauge,
        Production::Category::gauge,    Production::Category::gauge,
        Production::Category::gauge,    Production::Category::gauge};
constexpr std::array<Production::ParityClass, Production::NUM_VARS>
    expected_parities = {
        Production::ParityClass::scalar_even,
        Production::ParityClass::scalar_even,
        Production::ParityClass::one_z_opposite,
        Production::ParityClass::scalar_even,
        Production::ParityClass::scalar_even,
        Production::ParityClass::scalar_even,
        Production::ParityClass::scalar_even,
        Production::ParityClass::one_z_opposite,
        Production::ParityClass::scalar_even,
        Production::ParityClass::scalar_even,
        Production::ParityClass::scalar_even,
        Production::ParityClass::scalar_even,
        Production::ParityClass::one_z_opposite,
        Production::ParityClass::gauge_even,
        Production::ParityClass::gauge_even,
        Production::ParityClass::one_z_opposite,
        Production::ParityClass::gauge_even,
        Production::ParityClass::one_z_opposite};
constexpr std::array<int, Production::NUM_VARS> expected_hidden_multiplicity = {
    1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1};
constexpr std::array<Production::StockOverlap, Production::NUM_VARS>
    expected_overlap = {
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::hidden_representative,
        Production::StockOverlap::dimension_adapted,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::hidden_representative,
        Production::StockOverlap::dimension_adapted,
        Production::StockOverlap::dimension_adapted,
        Production::StockOverlap::dimension_adapted,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible,
        Production::StockOverlap::direct_visible};
constexpr std::array<bool, Production::NUM_VARS> expected_cleanup_owner = {
    true, true, true, true, true,  false, true, true, true,
    true, false, false, false, true, false, false, false, false};

constexpr std::array<std::string_view, 9> forbidden_visible_y_names = {
    "hxy", "hyy", "hyz", "Axy", "Ayy", "Ayz", "GammaY", "shiftY", "By"};

static_assert(CH_SPACEDIM == 2);
static_assert(GR_SPACEDIM == 4);
static_assert(DEFAULT_TENSOR_DIM == 4);
static_assert(sizeof(Tensor<1, double>) == 4U * sizeof(double));
static_assert(Production::NUM_VARS == 18);
static_assert(Production::c_chi == 0);
static_assert(Production::c_Bz == 17);
static_assert(!Production::uses_stock_visible_y_as_hidden);

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

bool is_forbidden_visible_y_name(const std::string_view name)
{
    return std::find(forbidden_visible_y_names.begin(),
                     forbidden_visible_y_names.end(),
                     name) != forbidden_visible_y_names.end();
}

bool dimensions_are_valid(const int ch_spacedim, const int gr_spacedim,
                          const int default_tensor_dim,
                          const int hidden_directions,
                          const int stored_representatives)
{
    return ch_spacedim == 2 && gr_spacedim == 4 &&
           default_tensor_dim == 4 && hidden_directions == 2 &&
           stored_representatives == 1;
}

bool gp_storage_is_valid(const double x, const double chi,
                         const double stored_hww)
{
    if (!(x > 0.0) || !(chi > 0.0))
    {
        return false;
    }
    return std::abs(stored_hww -
                    Production::GPBackgroundConvention::stored_hww) <=
               1.0e-15 &&
           std::abs(Production::GPBackgroundConvention::gamma_theta_theta(x) -
                    x * x) <= 1.0e-15 &&
           std::abs(Production::GPBackgroundConvention::gamma_ww(stored_hww,
                                                                 chi) -
                    stored_hww / chi) <= 1.0e-15;
}

bool candidate_contract_is_valid(const std::vector<Metadata> &entries,
                                 const std::vector<std::string> &names)
{
    if (entries.size() != static_cast<std::size_t>(Production::NUM_VARS) ||
        names.size() != static_cast<std::size_t>(Production::NUM_VARS))
    {
        return false;
    }

    std::array<bool, Production::NUM_VARS> seen_slots{};
    std::array<bool, Production::NUM_VARS> seen_names{};
    int physical_count = 0;
    int gauge_count = 0;

    for (std::size_t index = 0; index < entries.size(); ++index)
    {
        const Metadata &entry = entries[index];
        const int slot = static_cast<int>(entry.slot);
        if (slot < 0 || slot >= Production::NUM_VARS ||
            seen_slots[static_cast<std::size_t>(slot)])
        {
            return false;
        }
        seen_slots[static_cast<std::size_t>(slot)] = true;

        if (slot != expected_slots[index] ||
            entry.name != expected_names[index] ||
            names[index] != expected_names[index] ||
            entry.category != expected_categories[index] ||
            entry.parity != expected_parities[index] ||
            entry.storage_multiplicity != 1 ||
            entry.hidden_multiplicity != expected_hidden_multiplicity[index] ||
            entry.stock_overlap != expected_overlap[index])
        {
            return false;
        }

        if (!Production::owns(entry, Production::FutureOwner::initial_data) ||
            !Production::owns(entry, Production::FutureOwner::rhs) ||
            !Production::owns(entry, Production::FutureOwner::diagnostics) ||
            Production::owns(entry, Production::FutureOwner::cleanup) !=
                expected_cleanup_owner[index])
        {
            return false;
        }

        if (is_forbidden_visible_y_name(entry.name) ||
            is_forbidden_visible_y_name(names[index]))
        {
            return false;
        }

        for (std::size_t other = 0; other < names.size(); ++other)
        {
            if (names[index] == expected_names[other])
            {
                if (seen_names[other])
                {
                    return false;
                }
                seen_names[other] = true;
                break;
            }
        }

        if (entry.category == Production::Category::physical)
        {
            ++physical_count;
        }
        else
        {
            ++gauge_count;
        }
    }

    return std::all_of(seen_slots.begin(), seen_slots.end(),
                       [](const bool seen) { return seen; }) &&
           std::all_of(seen_names.begin(), seen_names.end(),
                       [](const bool seen) { return seen; }) &&
           physical_count == 13 && gauge_count == 5;
}

std::vector<Metadata> production_metadata()
{
    return {Production::metadata.begin(), Production::metadata.end()};
}

std::vector<std::string> production_registration_names()
{
    return {Production::UserVariables::registration_names.begin(),
            Production::UserVariables::registration_names.end()};
}

void require_rejected(const std::string &label,
                      const std::vector<Metadata> &entries,
                      const std::vector<std::string> &names)
{
    require(!candidate_contract_is_valid(entries, names),
            label + " mutation was accepted");
    std::cout << "PASS mutation rejected: " << label << "\n";
}

void check_exact_contract()
{
    require(candidate_contract_is_valid(production_metadata(),
                                        production_registration_names()),
            "exact production contract");
    require(&Production::UserVariables::variable_names ==
                &Production::UserVariables::registration_names,
            "registration names must alias variable_names");
    require(&Production::UserVariables::variable_names ==
                &Production::UserVariables::checkpoint_names,
            "checkpoint names must preserve registration order");
    require(&Production::UserVariables::variable_names ==
                &Production::UserVariables::output_names,
            "output names must preserve registration order");

    int physical_one_z = 0;
    for (const Metadata &entry : Production::metadata)
    {
        if (entry.category == Production::Category::physical &&
            entry.parity == Production::ParityClass::one_z_opposite)
        {
            require(entry.name == "hxz" || entry.name == "Axz" ||
                        entry.name == "GammaZ",
                    "unexpected physical one-z field " +
                        std::string(entry.name));
            ++physical_one_z;
        }
        std::cout << "SLOT " << static_cast<int>(entry.slot) << " "
                  << entry.name << " category="
                  << (entry.category == Production::Category::physical
                          ? "physical"
                          : "gauge")
                  << " hidden_multiplicity=" << entry.hidden_multiplicity
                  << "\n";
    }
    require(physical_one_z == 3,
            "exactly hxz, Axz, and GammaZ must be physical one-z fields");

    require(dimensions_are_valid(
                CH_SPACEDIM, GR_SPACEDIM, DEFAULT_TENSOR_DIM,
                Production::hidden_equivalent_directions,
                Production::stored_hidden_representatives),
            "compiled dimension contract");
    require(Production::gridded_direction_names[0] == "x" &&
                Production::gridded_direction_names[1] == "z",
            "gridded directions must be x,z");
    require(!Production::stock_symmetric_enum_mapping_is_compatible &&
                Production::stock_visible_symmetric_slots == 3 &&
                Production::physical_symmetric_tensor_slots == 10,
            "stock symmetric enum mapping incompatibility must remain explicit");
    require(!Production::stock_vector_enum_mapping_is_compatible,
            "stock vector enum mapping incompatibility must remain explicit");
    require(gp_storage_is_valid(2.0, 1.0, 1.0),
            "GP hww representative convention");
    require(Production::hidden_trace_contribution(3.25) == 6.5,
            "hidden representative must be written once and counted twice");
}

void check_mutations()
{
    const auto base_entries = production_metadata();
    const auto base_names = production_registration_names();

    {
        auto entries = base_entries;
        std::swap(entries[Production::c_hxx].slot,
                  entries[Production::c_hxz].slot);
        require(static_cast<int>(entries[Production::c_hxx].slot) ==
                    Production::c_hxz,
                "swapped-slot mutation must change active data");
        require_rejected("swapped hxx/hxz slots", entries, base_names);
    }
    {
        auto entries = base_entries;
        entries[Production::c_hxz].slot = entries[Production::c_hxx].slot;
        require_rejected("duplicated hxx slot", entries, base_names);
    }
    {
        auto entries = base_entries;
        auto names = base_names;
        entries.erase(entries.begin() + Production::c_Azz);
        names.erase(names.begin() + Production::c_Azz);
        require_rejected("omitted Azz slot", entries, names);
    }
    {
        auto entries = base_entries;
        auto names = base_names;
        entries[Production::c_hww].name = "hyy";
        names[Production::c_hww] = "hyy";
        require(is_forbidden_visible_y_name(entries[Production::c_hww].name),
                "visible-y hidden-name mutation must be active");
        require_rejected("hww mapped to visible-y hyy", entries, names);
    }
    {
        require(!gp_storage_is_valid(2.0, 1.0, 4.0),
                "hww=x^2 storage mutation must be rejected");
        std::cout << "PASS mutation rejected: hww stores x^2\n";
    }
    {
        auto entries = base_entries;
        entries[Production::c_hww].hidden_multiplicity = 1;
        require_rejected("hww physical multiplicity one", entries, base_names);
    }
    {
        auto entries = base_entries;
        auto names = base_names;
        Metadata visible_y = entries.back();
        visible_y.slot = static_cast<Slot>(Production::NUM_VARS);
        visible_y.name = "hyy";
        visible_y.category = Production::Category::physical;
        entries.push_back(visible_y);
        names.emplace_back("hyy");
        require(is_forbidden_visible_y_name(entries.back().name),
                "added visible-y mutation must be active");
        require_rejected("added visible-y field", entries, names);
    }
    {
        auto names = base_names;
        std::swap(names[Production::c_GammaX], names[Production::c_GammaZ]);
        require_rejected("enum/registration order mismatch", base_entries,
                         names);
    }
    {
        require(!dimensions_are_valid(
                    3, GR_SPACEDIM, DEFAULT_TENSOR_DIM,
                    Production::hidden_equivalent_directions,
                    Production::stored_hidden_representatives),
                "wrong CH_SPACEDIM mutation must be rejected");
        std::cout << "PASS mutation rejected: wrong compile-time dimension\n";
    }
}

} // namespace

int main()
{
    std::cout << "Black-string production 18-slot contract fixture\n";
    check_exact_contract();
    check_mutations();
    std::cout << "All black-string production variable-contract checks passed.\n";
    return 0;
}
