#define BLACKSTRINGTOY_LAYOUT_TEST_ONLY
#include "ConformalCartoonAlgebra.hpp"
#include "UserVariables.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
using namespace BlackStringToy::ConformalCartoonAlgebra;

constexpr double tolerance = 1.0e-12;

struct HelperInputs
{
    double chi;
    ConformalMetric h;
    ConformalExtrinsicCurvature A;
    double K;
};

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected)
{
    if (std::abs(value - expected) > tolerance)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

HelperInputs build_helper_inputs_from_components(
    const std::array<double, NUM_VARS> &components)
{
    return {components[c_chi],
            {components[c_h11], components[c_h12], components[c_h22],
             components[c_hww]},
            {components[c_A11], components[c_A12], components[c_A22],
             components[c_Aww]},
            components[c_K]};
}

void check_distinct_value_mapping()
{
    std::array<double, NUM_VARS> components = {};

    // Deliberately distinct values make swapped component slots fail loudly.
    components[c_chi] = 2.0;
    components[c_h11] = 3.0;
    components[c_h12] = 5.0;
    components[c_h22] = 37.0;
    components[c_hww] = 11.0;
    components[c_A11] = 13.0;
    components[c_A12] = 17.0;
    components[c_A22] = 19.0;
    components[c_Aww] = 23.0;
    components[c_K] = 29.0;

    const HelperInputs inputs = build_helper_inputs_from_components(components);

    require_close("helper.chi receives c_chi", inputs.chi, 2.0);
    require_close("helper.h_xx receives c_h11", inputs.h.xx, 3.0);
    require_close("helper.h_xz receives c_h12", inputs.h.xz, 5.0);
    require_close("helper.h_zz receives c_h22", inputs.h.zz, 37.0);
    require_close("helper.hww receives c_hww", inputs.h.ww, 11.0);
    require_close("helper.A_xx receives c_A11", inputs.A.xx, 13.0);
    require_close("helper.A_xz receives c_A12", inputs.A.xz, 17.0);
    require_close("helper.A_zz receives c_A22", inputs.A.zz, 19.0);
    require_close("helper.Aww receives c_Aww", inputs.A.ww, 23.0);
    require_close("helper.K receives c_K", inputs.K, 29.0);

    const SymmetricTensor K_ij =
        reconstruct_extrinsic_curvature(inputs.A, inputs.h, inputs.chi,
                                         inputs.K);

    // Independent arithmetic oracle:
    // chi=2, hww=11, Aww=23, K=29 gives
    // K_ww = (1/2) * (23 + 11*29/4) = 51.375.
    require_close("independent mapped K_ww oracle", K_ij.ww, 51.375);

    const double wrong_hww_from_hzz =
        (inputs.A.ww + inputs.h.zz * inputs.K / trace_denominator) /
        inputs.chi;
    if (std::abs(wrong_hww_from_hzz - K_ij.ww) <= tolerance)
    {
        fail("hww swap guard",
             "K_ww oracle did not distinguish c_hww from c_h22");
    }
    std::cout << "PASS hww swap guard distinguishes c_hww from c_h22\n";
}

} // namespace

int main()
{
    std::cout << "Stage 4E BlackStringToy grid-to-helper mapping fixture\n";
    std::cout << "Distinct local component values only; no Cell, Vars, "
                 "FArrayBox, AMR data, or evolution data are read.\n";

    check_distinct_value_mapping();

    std::cout << "All Stage 4E grid-to-helper mapping checks passed.\n";
    return 0;
}
