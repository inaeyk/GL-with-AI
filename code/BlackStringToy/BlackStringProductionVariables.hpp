#ifndef BLACKSTRINGPRODUCTIONVARIABLES_HPP
#define BLACKSTRINGPRODUCTIONVARIABLES_HPP

#include "DimensionDefinitions.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace BlackStringProductionVariables
{
static_assert(CH_SPACEDIM == 2,
              "black-string production requires CH_SPACEDIM=2");
static_assert(GR_SPACEDIM == 4,
              "black-string production requires GR_SPACEDIM=4");
static_assert(DEFAULT_TENSOR_DIM == 4,
              "black-string production requires DEFAULT_TENSOR_DIM=4");

inline constexpr int physical_spatial_dimension = 4;
inline constexpr int gridded_spatial_dimension = 2;
inline constexpr int x_direction = 0;
inline constexpr int z_direction = 1;
inline constexpr int hidden_equivalent_directions = 2;
inline constexpr int stored_hidden_representatives = 1;
inline constexpr int physical_trace_hidden_copies = 2;
inline constexpr bool uses_stock_visible_y_as_hidden = false;
inline constexpr std::array<std::string_view, gridded_spatial_dimension>
    gridded_direction_names = {"x", "z"};
inline constexpr int stock_visible_symmetric_slots =
    gridded_spatial_dimension * (gridded_spatial_dimension + 1) / 2;
inline constexpr int physical_symmetric_tensor_slots =
    DEFAULT_TENSOR_DIM * (DEFAULT_TENSOR_DIM + 1) / 2;
inline constexpr bool stock_symmetric_enum_mapping_is_compatible =
    stock_visible_symmetric_slots == physical_symmetric_tensor_slots;
inline constexpr bool stock_vector_enum_mapping_is_compatible =
    gridded_spatial_dimension == DEFAULT_TENSOR_DIM;

static_assert(CH_SPACEDIM == gridded_spatial_dimension);
static_assert(GR_SPACEDIM == physical_spatial_dimension);
static_assert(DEFAULT_TENSOR_DIM == physical_spatial_dimension);
static_assert(x_direction == 0 && z_direction == 1);
static_assert(hidden_equivalent_directions == 2);
static_assert(stored_hidden_representatives == 1);
static_assert(physical_trace_hidden_copies == hidden_equivalent_directions);
static_assert(!uses_stock_visible_y_as_hidden);
static_assert(stock_visible_symmetric_slots == 3);
static_assert(physical_symmetric_tensor_slots == 10);
static_assert(!stock_symmetric_enum_mapping_is_compatible);
static_assert(!stock_vector_enum_mapping_is_compatible);

enum VariableSlot : int
{
    c_chi = 0,
    c_hxx,
    c_hxz,
    c_hzz,
    c_hww,
    c_K,
    c_Axx,
    c_Axz,
    c_Azz,
    c_Aww,
    c_Theta,
    c_GammaX,
    c_GammaZ,
    c_lapse,
    c_shiftX,
    c_shiftZ,
    c_Bx,
    c_Bz,
    NUM_VARS
};

static_assert(c_chi == 0);
static_assert(c_hxx == 1);
static_assert(c_hxz == 2);
static_assert(c_hzz == 3);
static_assert(c_hww == 4);
static_assert(c_K == 5);
static_assert(c_Axx == 6);
static_assert(c_Axz == 7);
static_assert(c_Azz == 8);
static_assert(c_Aww == 9);
static_assert(c_Theta == 10);
static_assert(c_GammaX == 11);
static_assert(c_GammaZ == 12);
static_assert(c_lapse == 13);
static_assert(c_shiftX == 14);
static_assert(c_shiftZ == 15);
static_assert(c_Bx == 16);
static_assert(c_Bz == 17);
static_assert(NUM_VARS == 18);

namespace UserVariables
{
inline const std::array<std::string, NUM_VARS> variable_names = {
    "chi",    "hxx",    "hxz",    "hzz",   "hww",    "K",
    "Axx",    "Axz",    "Azz",    "Aww",   "Theta",  "GammaX",
    "GammaZ", "lapse",  "shiftX", "shiftZ", "Bx",     "Bz"};

// GRChombo checkpoint and plot/output registration use the evolution-variable
// ordering. These aliases make that single ordering the source of truth.
inline const auto &registration_names = variable_names;
inline const auto &checkpoint_names = variable_names;
inline const auto &output_names = variable_names;
} // namespace UserVariables

enum class Category : std::uint8_t
{
    physical,
    gauge
};

enum class ParityClass : std::uint8_t
{
    scalar_even,
    one_z_opposite,
    gauge_even
};

enum class StockOverlap : std::uint8_t
{
    direct_visible,
    dimension_adapted,
    hidden_representative
};

enum class FutureOwner : std::uint8_t
{
    initial_data = 1U << 0U,
    rhs = 1U << 1U,
    cleanup = 1U << 2U,
    diagnostics = 1U << 3U
};

constexpr std::uint8_t owner_bit(const FutureOwner owner)
{
    return static_cast<std::uint8_t>(owner);
}

constexpr std::uint8_t owners(const FutureOwner first,
                              const FutureOwner second,
                              const FutureOwner third)
{
    return owner_bit(first) | owner_bit(second) | owner_bit(third);
}

constexpr std::uint8_t owners(const FutureOwner first,
                              const FutureOwner second,
                              const FutureOwner third,
                              const FutureOwner fourth)
{
    return owners(first, second, third) | owner_bit(fourth);
}

inline constexpr std::uint8_t standard_owners =
    owners(FutureOwner::initial_data, FutureOwner::rhs,
           FutureOwner::diagnostics);
inline constexpr std::uint8_t cleanup_owners =
    owners(FutureOwner::initial_data, FutureOwner::rhs, FutureOwner::cleanup,
           FutureOwner::diagnostics);

struct VariableMetadata
{
    VariableSlot slot;
    std::string_view name;
    Category category;
    ParityClass parity;
    int storage_multiplicity;
    int hidden_multiplicity;
    StockOverlap stock_overlap;
    std::uint8_t future_owners;
};

inline constexpr std::array<VariableMetadata, NUM_VARS> metadata = {{
    {c_chi, "chi", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_hxx, "hxx", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_hxz, "hxz", Category::physical, ParityClass::one_z_opposite, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_hzz, "hzz", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_hww, "hww", Category::physical, ParityClass::scalar_even, 1, 2,
     StockOverlap::hidden_representative, cleanup_owners},
    {c_K, "K", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::dimension_adapted, standard_owners},
    {c_Axx, "Axx", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_Axz, "Axz", Category::physical, ParityClass::one_z_opposite, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_Azz, "Azz", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_Aww, "Aww", Category::physical, ParityClass::scalar_even, 1, 2,
     StockOverlap::hidden_representative, cleanup_owners},
    {c_Theta, "Theta", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::dimension_adapted, standard_owners},
    {c_GammaX, "GammaX", Category::physical, ParityClass::scalar_even, 1, 1,
     StockOverlap::dimension_adapted, standard_owners},
    {c_GammaZ, "GammaZ", Category::physical, ParityClass::one_z_opposite, 1,
     1, StockOverlap::dimension_adapted, standard_owners},
    {c_lapse, "lapse", Category::gauge, ParityClass::gauge_even, 1, 1,
     StockOverlap::direct_visible, cleanup_owners},
    {c_shiftX, "shiftX", Category::gauge, ParityClass::gauge_even, 1, 1,
     StockOverlap::direct_visible, standard_owners},
    {c_shiftZ, "shiftZ", Category::gauge, ParityClass::one_z_opposite, 1, 1,
     StockOverlap::direct_visible, standard_owners},
    {c_Bx, "Bx", Category::gauge, ParityClass::gauge_even, 1, 1,
     StockOverlap::direct_visible, standard_owners},
    {c_Bz, "Bz", Category::gauge, ParityClass::one_z_opposite, 1, 1,
     StockOverlap::direct_visible, standard_owners},
}};

constexpr bool owns(const VariableMetadata &entry, const FutureOwner owner)
{
    return (entry.future_owners & owner_bit(owner)) != 0U;
}

struct GPBackgroundConvention
{
    static constexpr double stored_hww = 1.0;

    static constexpr double gamma_theta_theta(const double x)
    {
        return x * x;
    }

    static constexpr double gamma_ww(const double hww, const double chi)
    {
        return hww / chi;
    }
};

constexpr double hidden_trace_contribution(const double representative)
{
    return static_cast<double>(physical_trace_hidden_copies) * representative;
}

static_assert(GPBackgroundConvention::stored_hww == 1.0);
static_assert(GPBackgroundConvention::gamma_theta_theta(2.0) == 4.0);
static_assert(GPBackgroundConvention::gamma_ww(1.0, 1.0) == 1.0);
static_assert(hidden_trace_contribution(3.0) == 6.0);

} // namespace BlackStringProductionVariables

#endif /* BLACKSTRINGPRODUCTIONVARIABLES_HPP */
