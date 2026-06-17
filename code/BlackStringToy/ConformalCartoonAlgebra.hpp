#ifndef BLACKSTRINGTOY_CONFORMALCARTOONALGEBRA_HPP
#define BLACKSTRINGTOY_CONFORMALCARTOONALGEBRA_HPP

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace ConformalCartoonAlgebra
{
// Stage 4A targets the 5D black-string spatial bookkeeping:
// four physical spatial dimensions with two gridded directions.
static constexpr int physical_spatial_dim = 4;
static constexpr int gridded_spatial_dim = 2;
static constexpr int hidden_multiplicity =
    physical_spatial_dim - gridded_spatial_dim;
static constexpr double trace_denominator =
    static_cast<double>(physical_spatial_dim);
static constexpr double algebra_zero_tolerance = 1.0e-14;
static constexpr double inverse_metric_consistency_tolerance = 1.0e-12;

struct SymmetricTensor
{
    double xx;
    double xz;
    double zz;
    double ww;
};

using ConformalMetric = SymmetricTensor;
using ConformalExtrinsicCurvature = SymmetricTensor;

struct InverseConformalMetric
{
    double xx;
    double xz;
    double zz;
    double ww;
};

inline double reduced_determinant(const ConformalMetric &h)
{
    return h.xx * h.zz - h.xz * h.xz;
}

inline double determinant_4d(const ConformalMetric &h)
{
    return reduced_determinant(h) * h.ww * h.ww;
}

inline InverseConformalMetric inverse(const ConformalMetric &h)
{
    const double det = reduced_determinant(h);
    if (std::abs(det) <= algebra_zero_tolerance)
    {
        throw std::domain_error(
            "ConformalCartoonAlgebra inverse requires nonzero reduced "
            "determinant");
    }
    if (std::abs(h.ww) <= algebra_zero_tolerance)
    {
        throw std::domain_error(
            "ConformalCartoonAlgebra inverse requires nonzero hww");
    }

    return {h.zz / det, -h.xz / det, h.xx / det, 1.0 / h.ww};
}

inline void require_inverse_metric_consistency(
    const ConformalMetric &h_LL, const InverseConformalMetric &h_UU,
    const double tolerance = inverse_metric_consistency_tolerance)
{
    const double values[] = {h_LL.xx, h_LL.xz, h_LL.zz, h_LL.ww,
                             h_UU.xx, h_UU.xz, h_UU.zz, h_UU.ww};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "ConformalCartoonAlgebra metric/inverse consistency "
                "requires finite entries");
        }
    }

    const double reduced_xx = h_LL.xx * h_UU.xx + h_LL.xz * h_UU.xz;
    const double reduced_xz = h_LL.xx * h_UU.xz + h_LL.xz * h_UU.zz;
    const double reduced_zx = h_LL.xz * h_UU.xx + h_LL.zz * h_UU.xz;
    const double reduced_zz = h_LL.xz * h_UU.xz + h_LL.zz * h_UU.zz;
    const double hidden_ww = h_LL.ww * h_UU.ww;

    if (std::abs(reduced_xx - 1.0) > tolerance ||
        std::abs(reduced_xz) > tolerance ||
        std::abs(reduced_zx) > tolerance ||
        std::abs(reduced_zz - 1.0) > tolerance ||
        std::abs(hidden_ww - 1.0) > tolerance)
    {
        throw std::domain_error(
            "ConformalCartoonAlgebra requires h_LL and h_UU to be "
            "mutually consistent");
    }
}

inline double trace(const SymmetricTensor &tensor,
                    const InverseConformalMetric &h_UU)
{
    return h_UU.xx * tensor.xx + 2.0 * h_UU.xz * tensor.xz +
           h_UU.zz * tensor.zz +
           static_cast<double>(hidden_multiplicity) * h_UU.ww * tensor.ww;
}

inline double trace(const SymmetricTensor &tensor, const ConformalMetric &h)
{
    return trace(tensor, inverse(h));
}

inline SymmetricTensor trace_free_part(
    const SymmetricTensor &tensor, const ConformalMetric &h,
    const InverseConformalMetric &h_UU,
    const double denominator = trace_denominator)
{
    const double tr = trace(tensor, h_UU);
    return {tensor.xx - h.xx * tr / denominator,
            tensor.xz - h.xz * tr / denominator,
            tensor.zz - h.zz * tr / denominator,
            tensor.ww - h.ww * tr / denominator};
}

inline SymmetricTensor trace_free_part(
    const SymmetricTensor &tensor, const ConformalMetric &h,
    const double denominator = trace_denominator)
{
    return trace_free_part(tensor, h, inverse(h), denominator);
}

inline SymmetricTensor reconstruct_extrinsic_curvature(
    const ConformalExtrinsicCurvature &A, const ConformalMetric &h,
    const double chi, const double K,
    const double denominator = trace_denominator)
{
    if (std::abs(chi) <= algebra_zero_tolerance)
    {
        throw std::domain_error(
            "ConformalCartoonAlgebra K reconstruction requires nonzero chi");
    }

    const double inv_chi = 1.0 / chi;
    return {inv_chi * (A.xx + h.xx * K / denominator),
            inv_chi * (A.xz + h.xz * K / denominator),
            inv_chi * (A.zz + h.zz * K / denominator),
            inv_chi * (A.ww + h.ww * K / denominator)};
}

inline ConformalMetric conformal_metric_from_physical(
    const SymmetricTensor &gamma, const double chi)
{
    return {chi * gamma.xx, chi * gamma.xz, chi * gamma.zz,
            chi * gamma.ww};
}

inline double chi_from_physical_metric(const SymmetricTensor &gamma)
{
    const double det_gamma =
        (gamma.xx * gamma.zz - gamma.xz * gamma.xz) * gamma.ww * gamma.ww;
    if (det_gamma <= algebra_zero_tolerance)
    {
        throw std::domain_error(
            "ConformalCartoonAlgebra chi requires positive physical "
            "determinant");
    }
    const double chi_power =
        -1.0 / static_cast<double>(physical_spatial_dim);
    return std::pow(det_gamma, chi_power);
}

} // namespace ConformalCartoonAlgebra
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CONFORMALCARTOONALGEBRA_HPP */
