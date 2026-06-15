#include "ConformalCartoonAlgebra.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
using namespace BlackStringToy::ConformalCartoonAlgebra;

constexpr double tolerance = 1.0e-12;

void require_close(const std::string &label, const double value,
                   const double expected, const double tol = tolerance)
{
    if (std::abs(value - expected) > tol)
    {
        std::cerr << "FAIL " << label << ": got " << value << ", expected "
                  << expected << "\n";
        std::exit(1);
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void require_nonzero(const std::string &label, const double value,
                     const double tol = tolerance)
{
    if (std::abs(value) <= tol)
    {
        std::cerr << "FAIL " << label << ": expected nonzero, got " << value
                  << "\n";
        std::exit(1);
    }
    std::cout << "PASS " << label << " fails as expected = " << value
              << "\n";
}

void test_offdiagonal_determinant_and_inverse()
{
    const ConformalMetric h{3.0, 0.4, 2.0, 1.5};
    const double det_reduced_expected = 3.0 * 2.0 - 0.4 * 0.4;
    const double det_4d_expected = det_reduced_expected * 1.5 * 1.5;

    require_close("reduced determinant with h_xz", reduced_determinant(h),
                  det_reduced_expected);
    require_close("4D conformal determinant with hww", determinant_4d(h),
                  det_4d_expected);

    const InverseConformalMetric h_UU = inverse(h);
    require_close("inverse h^xx", h_UU.xx, h.zz / det_reduced_expected);
    require_close("inverse h^xz", h_UU.xz, -h.xz / det_reduced_expected);
    require_close("inverse h^zz", h_UU.zz, h.xx / det_reduced_expected);
    require_close("inverse h^ww", h_UU.ww, 1.0 / h.ww);

    require_close("block inverse product xx",
                  h.xx * h_UU.xx + h.xz * h_UU.xz, 1.0);
    require_close("block inverse product xz",
                  h.xx * h_UU.xz + h.xz * h_UU.zz, 0.0);
    require_close("block inverse product zx",
                  h.xz * h_UU.xx + h.zz * h_UU.xz, 0.0);
    require_close("block inverse product zz",
                  h.xz * h_UU.xz + h.zz * h_UU.zz, 1.0);
}

void test_trace_free_projection_and_denominator_guard()
{
    const ConformalMetric h{3.0, 0.4, 2.0, 1.5};
    const SymmetricTensor raw{1.1, -0.6, 0.9, 0.2};

    const SymmetricTensor A = trace_free_part(raw, h);
    require_close("full 4D trace-free projection", trace(A, h), 0.0);

    const SymmetricTensor A_p2 = trace_free_part(raw, h, 2.0);
    const SymmetricTensor A_p3 = trace_free_part(raw, h, 3.0);
    require_nonzero("p=2 trace-free denominator guard", trace(A_p2, h));
    require_nonzero("p=3 trace-free denominator guard", trace(A_p3, h));
}

void test_extrinsic_curvature_reconstruction()
{
    const ConformalMetric h{3.0, 0.4, 2.0, 1.5};
    const SymmetricTensor raw_A{1.1, -0.6, 0.9, 0.2};
    const ConformalExtrinsicCurvature A = trace_free_part(raw_A, h);
    const double chi = 0.8;
    const double K = 1.7;

    const SymmetricTensor Kij = reconstruct_extrinsic_curvature(A, h, chi, K);
    require_close("K_xx reconstruction",
                  Kij.xx - (A.xx + h.xx * K / 4.0) / chi, 0.0);
    require_close("K_xz reconstruction",
                  Kij.xz - (A.xz + h.xz * K / 4.0) / chi, 0.0);
    require_close("K_zz reconstruction",
                  Kij.zz - (A.zz + h.zz * K / 4.0) / chi, 0.0);
    require_close("K_ww reconstruction",
                  Kij.ww - (A.ww + h.ww * K / 4.0) / chi, 0.0);

    const SymmetricTensor Kij_p2 =
        reconstruct_extrinsic_curvature(A, h, chi, K, 2.0);
    const SymmetricTensor Kij_p3 =
        reconstruct_extrinsic_curvature(A, h, chi, K, 3.0);
    require_nonzero("p=2 K_xz reconstruction denominator guard",
                    Kij_p2.xz - Kij.xz);
    require_nonzero("p=3 K_ww reconstruction denominator guard",
                    Kij_p3.ww - Kij.ww);
}

void test_independent_k_reconstruction_oracles()
{
    // Independent arithmetic oracle:
    // chi=2, h_xz=3, A_xz=5, K=7 gives
    // K_xz = (1/2) * (5 + 3*7/4) = 41/8 = 5.125.
    // With hww=6 and Aww=11,
    // K_ww = (1/2) * (11 + 6*7/4) = 43/4 = 10.75.
    const ConformalMetric h{4.0, 3.0, 5.0, 6.0};
    const ConformalExtrinsicCurvature A{13.0, 5.0, -2.0, 11.0};
    const double chi = 2.0;
    const double K = 7.0;

    const SymmetricTensor Kij = reconstruct_extrinsic_curvature(A, h, chi, K);
    require_close("independent oracle K_xz = 41/8", Kij.xz, 41.0 / 8.0);
    require_close("independent oracle K_ww = 43/4", Kij.ww, 43.0 / 4.0);
}

void test_diagonal_limit()
{
    const ConformalMetric h{3.0, 0.0, 2.0, 1.5};
    const SymmetricTensor raw{1.1, 0.0, 0.9, 0.2};
    const InverseConformalMetric h_UU = inverse(h);

    require_close("diagonal limit determinant", determinant_4d(h),
                  h.xx * h.zz * h.ww * h.ww);
    require_close("diagonal limit h^xx", h_UU.xx, 1.0 / h.xx);
    require_close("diagonal limit h^xz", h_UU.xz, 0.0);
    require_close("diagonal limit h^zz", h_UU.zz, 1.0 / h.zz);
    require_close("diagonal limit h^ww", h_UU.ww, 1.0 / h.ww);

    const SymmetricTensor A = trace_free_part(raw, h);
    require_close("diagonal limit A_xz", A.xz, 0.0);
    require_close("diagonal limit trace-free projection", trace(A, h), 0.0);

    const SymmetricTensor Kij =
        reconstruct_extrinsic_curvature(A, h, 0.8, 1.7);
    require_close("diagonal limit K_xz", Kij.xz, 0.0);
}

void test_normalized_determinant_fixture()
{
    const SymmetricTensor gamma{2.0, 0.25, 1.4, 1.1};
    const double det_gamma =
        (gamma.xx * gamma.zz - gamma.xz * gamma.xz) * gamma.ww * gamma.ww;
    const double chi = chi_from_physical_metric(gamma);
    const ConformalMetric h = conformal_metric_from_physical(gamma, chi);

    require_close("chi^4 det gamma_4D normalized",
                  std::pow(chi, 4) * det_gamma, 1.0);
    require_close("normalized det h_4D", determinant_4d(h), 1.0);
}

} // namespace

int main()
{
    std::cout << "Stage 4A BlackStringToy conformal-cartoon algebra fixture\n";
    std::cout << "Local values only; no grid variables or enum slots are read.\n";

    test_offdiagonal_determinant_and_inverse();
    test_trace_free_projection_and_denominator_guard();
    test_extrinsic_curvature_reconstruction();
    test_independent_k_reconstruction_oracles();
    test_diagonal_limit();
    test_normalized_determinant_fixture();

    std::cout << "All Stage 4A conformal-cartoon algebra fixture checks passed.\n";
    return 0;
}
