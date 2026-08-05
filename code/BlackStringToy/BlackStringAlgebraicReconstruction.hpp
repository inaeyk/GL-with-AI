#ifndef BLACKSTRINGALGEBRAICRECONSTRUCTION_HPP
#define BLACKSTRINGALGEBRAICRECONSTRUCTION_HPP

#include "BlackStringReducedVars.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace BlackStringAlgebraicReconstruction
{
namespace Reduced = BlackStringReducedVars;

inline constexpr double relative_determinant_floor = 1.0e-14;
inline constexpr double metric_condition_limit = 1.0e8;

struct Result
{
    double delta = 0.0;
    double numerator = 0.0;
    double hww = 0.0;
    double Aww = 0.0;
    double metric_condition = 0.0;
};

inline Result evaluate(const double hxx, const double hxz, const double hzz,
                       const double Axx, const double Axz, const double Azz)
{
    const double values[] = {hxx, hxz, hzz, Axx, Axz, Azz};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE");
        }
    }

    const double delta = hxx * hzz - hxz * hxz;
    const double metric_scale = std::max(hxx, hzz);
    if (!(hxx > 0.0) || !(hzz > 0.0) || !(delta > 0.0) ||
        !(delta >= relative_determinant_floor * metric_scale * metric_scale))
    {
        throw std::domain_error(
            "M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE");
    }

    const double trace = hxx + hzz;
    const double discriminant =
        std::hypot(hxx - hzz, 2.0 * hxz);
    const double lambda_max = 0.5 * (trace + discriminant);
    const double lambda_min = delta / lambda_max;
    const double condition = lambda_max / lambda_min;
    if (!std::isfinite(condition) || condition > metric_condition_limit)
    {
        throw std::domain_error(
            "M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE");
    }

    const double sqrt_delta = std::sqrt(delta);
    const double delta_three_halves = delta * sqrt_delta;
    const double numerator =
        hzz * Axx - 2.0 * hxz * Axz + hxx * Azz;
    const double hww = 1.0 / sqrt_delta;
    const double Aww = -numerator / (2.0 * delta_three_halves);
    if (!std::isfinite(hww) || !std::isfinite(Aww))
    {
        throw std::domain_error(
            "M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE");
    }
    return {delta, numerator, hww, Aww, condition};
}

inline Result reconstruct(Reduced::Variables<double> &vars)
{
    const Result result =
        evaluate(vars.physical.h.xx, vars.physical.h.xz,
                 vars.physical.h.zz, vars.physical.A.xx,
                 vars.physical.A.xz, vars.physical.A.zz);
    vars.physical.h.ww = result.hww;
    vars.physical.A.ww = result.Aww;
    return result;
}

inline void reconstruct_dependent_rhs(
    const Reduced::Variables<double> &state,
    Reduced::Variables<double> &rhs)
{
    const Result algebraic =
        evaluate(state.physical.h.xx, state.physical.h.xz,
                 state.physical.h.zz, state.physical.A.xx,
                 state.physical.A.xz, state.physical.A.zz);
    const double dot_delta =
        state.physical.h.zz * rhs.physical.h.xx +
        state.physical.h.xx * rhs.physical.h.zz -
        2.0 * state.physical.h.xz * rhs.physical.h.xz;
    const double dot_numerator =
        rhs.physical.h.zz * state.physical.A.xx +
        state.physical.h.zz * rhs.physical.A.xx -
        2.0 * (rhs.physical.h.xz * state.physical.A.xz +
               state.physical.h.xz * rhs.physical.A.xz) +
        rhs.physical.h.xx * state.physical.A.zz +
        state.physical.h.xx * rhs.physical.A.zz;

    const double sqrt_delta = std::sqrt(algebraic.delta);
    const double delta_three_halves = algebraic.delta * sqrt_delta;
    const double delta_five_halves =
        algebraic.delta * delta_three_halves;
    rhs.physical.h.ww =
        -(algebraic.hww / (2.0 * algebraic.delta)) * dot_delta;
    rhs.physical.A.ww =
        -dot_numerator / (2.0 * delta_three_halves) +
        3.0 * algebraic.numerator * dot_delta /
            (4.0 * delta_five_halves);
    if (!std::isfinite(rhs.physical.h.ww) ||
        !std::isfinite(rhs.physical.A.ww))
    {
        throw std::domain_error(
            "M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE");
    }
}

inline double determinant_residual(const Reduced::Variables<double> &vars)
{
    const double delta = vars.physical.h.xx * vars.physical.h.zz -
                         vars.physical.h.xz * vars.physical.h.xz;
    return delta * vars.physical.h.ww * vars.physical.h.ww - 1.0;
}

inline double weighted_trace_residual(
    const Reduced::Variables<double> &vars)
{
    const double delta = vars.physical.h.xx * vars.physical.h.zz -
                         vars.physical.h.xz * vars.physical.h.xz;
    return vars.physical.h.zz * vars.physical.A.xx / delta -
           2.0 * vars.physical.h.xz * vars.physical.A.xz / delta +
           vars.physical.h.xx * vars.physical.A.zz / delta +
           2.0 * vars.physical.A.ww / vars.physical.h.ww;
}

inline void validate_locked_gate(const double r0)
{
    if (!std::isfinite(r0) || !(r0 > 0.0))
    {
        throw std::domain_error(
            "M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE");
    }
    Reduced::Variables<double> state{};
    state.physical.h = {1.17, 0.08, 0.91, 0.0};
    state.physical.A = {-0.31 / r0, 0.07 / r0, 0.22 / r0, 0.0};
    reconstruct(state);
    if (std::abs(determinant_residual(state)) > 5.0e-13 ||
        std::abs(weighted_trace_residual(state)) > 5.0e-13 / r0)
    {
        throw std::domain_error(
            "M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE");
    }

    Reduced::Variables<double> rhs{};
    rhs.physical.h = {0.13 / r0, -0.04 / r0, 0.09 / r0, 0.0};
    rhs.physical.A = {-0.12 / (r0 * r0), 0.03 / (r0 * r0),
                      0.08 / (r0 * r0), 0.0};
    reconstruct_dependent_rhs(state, rhs);
    const auto centered = [&](const double step) {
        Reduced::Variables<double> plus = state;
        Reduced::Variables<double> minus = state;
        plus.physical.h.xx += step * rhs.physical.h.xx;
        plus.physical.h.xz += step * rhs.physical.h.xz;
        plus.physical.h.zz += step * rhs.physical.h.zz;
        plus.physical.A.xx += step * rhs.physical.A.xx;
        plus.physical.A.xz += step * rhs.physical.A.xz;
        plus.physical.A.zz += step * rhs.physical.A.zz;
        minus.physical.h.xx -= step * rhs.physical.h.xx;
        minus.physical.h.xz -= step * rhs.physical.h.xz;
        minus.physical.h.zz -= step * rhs.physical.h.zz;
        minus.physical.A.xx -= step * rhs.physical.A.xx;
        minus.physical.A.xz -= step * rhs.physical.A.xz;
        minus.physical.A.zz -= step * rhs.physical.A.zz;
        reconstruct(plus);
        reconstruct(minus);
        return std::array<double, 2>{
            (plus.physical.h.ww - minus.physical.h.ww) / (2.0 * step),
            (plus.physical.A.ww - minus.physical.A.ww) / (2.0 * step)};
    };
    const auto primary = centered(std::ldexp(r0, -18));
    const auto refined = centered(std::ldexp(r0, -19));
    const double scale_h = 1.0 / r0 + std::abs(rhs.physical.h.ww);
    const double scale_A =
        1.0 / (r0 * r0) + std::abs(rhs.physical.A.ww);
    if (std::abs(primary[0] - rhs.physical.h.ww) / scale_h > 5.0e-9 ||
        std::abs(primary[1] - rhs.physical.A.ww) / scale_A > 5.0e-9 ||
        std::abs(primary[0] - refined[0]) / scale_h > 5.0e-9 ||
        std::abs(primary[1] - refined[1]) / scale_A > 5.0e-9)
    {
        throw std::domain_error(
            "M2-B ALGEBRAIC_RECONSTRUCTION_CHAIN_RULE_FAILURE");
    }
}

} // namespace BlackStringAlgebraicReconstruction

#endif /* BLACKSTRINGALGEBRAICRECONSTRUCTION_HPP */
