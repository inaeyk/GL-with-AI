#ifndef BLACKSTRINGTOY_CARTOONRHSSOURCEBLOCK_HPP
#define BLACKSTRINGTOY_CARTOONRHSSOURCEBLOCK_HPP

#include "CartoonRhsContract.hpp"

namespace BlackStringToy
{
namespace CartoonRhsSourceBlock
{
// Stage 4K local RHS source-block skeleton only. This is the future container
// shape for repo-owned modified-cartoon RHS source terms. It is not called
// from BlackStringToyLevel, does not read grid data, and does not implement
// physical RHS formulas.
//
// The source block consumes the Stage 4J contract type. Raw
// CartoonRicci::RicciComponents is intentionally not accepted here.
static constexpr bool source_terms_implemented = false;
static constexpr bool evolution_wiring_implemented = false;

struct LocalSourceBlockInput
{
    CartoonRhsContract::RhsLocalInputs rhs_contract;
};

struct LocalSourceBlockOutput
{
    // Named future RHS pieces. Stage 4K's explicit inert test path sets these
    // to zero only to verify the container and contract boundary. These zeros
    // are not physical source terms.
    double rhs_chi;
    double rhs_hww;
    double rhs_Aww;
    double rhs_K;

    CartoonRhsContract::RhsRicciContractions ricci_contractions;
    bool inert_test_output;
};

inline LocalSourceBlockOutput make_empty_source_block_for_contract_test(
    const LocalSourceBlockInput &input)
{
    const auto contract_outputs =
        CartoonRhsContract::make_inert_rhs_contract_outputs(
            input.rhs_contract);

    return {0.0, 0.0, 0.0, 0.0,
            contract_outputs.ricci_contractions, true};
}

} // namespace CartoonRhsSourceBlock
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONRHSSOURCEBLOCK_HPP */
