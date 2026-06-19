#ifndef BLACKSTRINGTOY_CARTOONHXZPARITYVALIDATION_HPP
#define BLACKSTRINGTOY_CARTOONHXZPARITYVALIDATION_HPP

#include <cmath>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <vector>

namespace BlackStringToy
{
namespace CartoonHxzParityValidation
{
// Stage 4AG validates only the two-sided odd parity of h_xz near the cartoon
// axis. It does not validate h_xx-h_ww=O(x^2), W_x=O(x), chi_x=O(x), full
// smoothness, finite-axis regularity, physical R_ww, CCZ4 RHS, or evolution.
// The tolerance below is a validation tolerance; it is never used to clamp a
// value or replace a division by an epsilon.
struct HxzParitySample
{
    double x;
    double h_xz;
};

class ValidatedHxzParityStencil
{
  public:
    std::size_t pair_count() const { return m_pair_count; }
    bool axis_value_checked() const { return m_axis_value_checked; }
    double max_odd_residual() const { return m_max_odd_residual; }
    double max_quotient_residual() const
    {
        return m_max_quotient_residual;
    }
    double tolerance() const { return m_tolerance; }

  private:
    ValidatedHxzParityStencil(const std::size_t pair_count,
                              const bool axis_value_checked,
                              const double max_odd_residual,
                              const double max_quotient_residual,
                              const double tolerance)
        : m_pair_count(pair_count), m_axis_value_checked(axis_value_checked),
          m_max_odd_residual(max_odd_residual),
          m_max_quotient_residual(max_quotient_residual),
          m_tolerance(tolerance)
    {
    }

    friend ValidatedHxzParityStencil validate_hxz_odd_parity(
        const std::vector<HxzParitySample> &positive_samples,
        const std::vector<HxzParitySample> &negative_samples,
        std::optional<double> axis_h_xz, double tolerance);

    std::size_t m_pair_count;
    bool m_axis_value_checked;
    double m_max_odd_residual;
    double m_max_quotient_residual;
    double m_tolerance;
};

inline void require_finite_sample(const HxzParitySample &sample)
{
    if (!std::isfinite(sample.x) || !std::isfinite(sample.h_xz))
    {
        throw std::domain_error(
            "CartoonHxzParityValidation requires finite sample data");
    }
}

inline ValidatedHxzParityStencil validate_hxz_odd_parity(
    const std::vector<HxzParitySample> &positive_samples,
    const std::vector<HxzParitySample> &negative_samples,
    const std::optional<double> axis_h_xz, const double tolerance)
{
    if (!std::isfinite(tolerance) || tolerance <= 0.0)
    {
        throw std::domain_error(
            "CartoonHxzParityValidation requires positive finite tolerance");
    }
    if (positive_samples.empty() ||
        positive_samples.size() != negative_samples.size())
    {
        throw std::domain_error(
            "CartoonHxzParityValidation requires paired nonempty samples");
    }

    for (const auto &sample : positive_samples)
    {
        require_finite_sample(sample);
        if (sample.x <= 0.0)
        {
            throw std::domain_error(
                "CartoonHxzParityValidation positive samples require x > 0");
        }
    }
    for (const auto &sample : negative_samples)
    {
        require_finite_sample(sample);
        if (sample.x >= 0.0)
        {
            throw std::domain_error(
                "CartoonHxzParityValidation negative samples require x < 0");
        }
    }

    bool axis_checked = false;
    if (axis_h_xz.has_value())
    {
        axis_checked = true;
        if (!std::isfinite(*axis_h_xz) || std::abs(*axis_h_xz) > tolerance)
        {
            throw std::domain_error(
                "CartoonHxzParityValidation requires h_xz(0,z)=0 within "
                "validation tolerance");
        }
    }

    std::vector<bool> negative_used(negative_samples.size(), false);
    double max_odd_residual = 0.0;
    double max_quotient_residual = 0.0;

    for (const auto &positive : positive_samples)
    {
        std::size_t match = negative_samples.size();
        for (std::size_t i = 0; i < negative_samples.size(); ++i)
        {
            if (!negative_used[i] &&
                std::abs(positive.x + negative_samples[i].x) <= tolerance)
            {
                match = i;
                break;
            }
        }
        if (match == negative_samples.size())
        {
            throw std::domain_error(
                "CartoonHxzParityValidation found no matching -x partner");
        }
        negative_used[match] = true;

        const auto &negative = negative_samples[match];
        const double odd_residual = std::abs(positive.h_xz + negative.h_xz);
        const double positive_quotient = positive.h_xz / positive.x;
        const double negative_quotient = negative.h_xz / negative.x;
        const double quotient_residual =
            std::abs(positive_quotient - negative_quotient);

        if (!std::isfinite(positive_quotient) ||
            !std::isfinite(negative_quotient) ||
            !std::isfinite(quotient_residual))
        {
            throw std::domain_error(
                "CartoonHxzParityValidation produced nonfinite quotient");
        }
        if (odd_residual > tolerance || quotient_residual > tolerance)
        {
            throw std::domain_error(
                "CartoonHxzParityValidation odd/even-quotient residual "
                "exceeds validation tolerance");
        }

        if (odd_residual > max_odd_residual)
        {
            max_odd_residual = odd_residual;
        }
        if (quotient_residual > max_quotient_residual)
        {
            max_quotient_residual = quotient_residual;
        }
    }

    return ValidatedHxzParityStencil(
        positive_samples.size(), axis_checked, max_odd_residual,
        max_quotient_residual, tolerance);
}

} // namespace CartoonHxzParityValidation
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_CARTOONHXZPARITYVALIDATION_HPP */
