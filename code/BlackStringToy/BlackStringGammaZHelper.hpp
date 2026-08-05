#ifndef BLACKSTRINGGAMMAZHELPER_HPP
#define BLACKSTRINGGAMMAZHELPER_HPP

#include "BlackStringAlgebraicReconstruction.hpp"
#include "BlackStringTargetCCZ4Pointwise.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <stdexcept>

namespace BlackStringGammaZHelper
{
namespace Algebraic = BlackStringAlgebraicReconstruction;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;
namespace Target = BlackStringTargetCCZ4Pointwise;

struct IndependentMetricJet
{
    double hxx = 1.0;
    double hxz = 0.0;
    double hzz = 1.0;
    double qn_hxx = 0.0;
    double qn_hxz = 0.0;
    double qn_hzz = 0.0;
    double dz_hxx = 0.0;
    double dz_hxz = 0.0;
    double dz_hzz = 0.0;
    double GammaX = 0.0;
    double GammaZ = 0.0;
};

struct Result
{
    double gX = 0.0;
    double gZ = 0.0;
    double ZX = 0.0;
    double ZZ = 0.0;
    double hww = 1.0;
    double qn_hww = 0.0;
    double dz_hww = 0.0;
};

// The locked helper is the signed tangent of the exact algebraic manifold at
// the flat conformal GP metric. Inputs carry the full visible metric so that
// admissibility is checked before its perturbation direction is extracted.
inline Result evaluate(const double x, const int normal,
                       const IndependentMetricJet &input)
{
    if (!std::isfinite(x) || !(x > 0.0) ||
        (normal != -1 && normal != 1))
    {
        throw std::domain_error("M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
    }
    (void)Algebraic::evaluate(input.hxx, input.hxz, input.hzz, 0.0, 0.0,
                              0.0);

    const double delta_hxx = input.hxx - 1.0;
    const double delta_hzz = input.hzz - 1.0;
    const double delta_hww = -0.5 * (delta_hxx + delta_hzz);
    const double qn_hww = -0.5 * (input.qn_hxx + input.qn_hzz);
    const double dz_hww = -0.5 * (input.dz_hxx + input.dz_hzz);
    const double nx = static_cast<double>(normal);
    const double gX =
        0.5 * nx * input.qn_hxx - 0.5 * nx * input.qn_hzz -
        nx * qn_hww + input.dz_hxz +
        2.0 * (delta_hxx - delta_hww) / x;
    const double gZ = nx * input.qn_hxz + 2.0 * input.hxz / x -
                      0.5 * input.dz_hxx + 0.5 * input.dz_hzz - dz_hww;
    const Result result{gX,
                        gZ,
                        0.5 * (input.GammaX - gX),
                        0.5 * (input.GammaZ - gZ),
                        1.0 + delta_hww,
                        qn_hww,
                        dz_hww};
    const double values[] = {result.gX, result.gZ, result.ZX, result.ZZ,
                             result.hww, result.qn_hww, result.dz_hww};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
        }
    }
    return result;
}

inline std::array<double, 4>
live_mapped_signed_linearization(const double x, const int normal,
                                 const IndependentMetricJet &direction,
                                 const double step)
{
    if (!(step > 0.0) || !std::isfinite(step))
    {
        throw std::domain_error("M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
    }
    std::array<std::array<double, 4>, 2> signed_values{};
    for (int side = 0; side < 2; ++side)
    {
        const double sign = side == 0 ? -1.0 : 1.0;
        const double scale = sign * step;
        Target::Input mapped{};
        mapped.x = x;
        mapped.vars.physical.chi = 1.0;
        mapped.vars.gauge.lapse = 1.0;
        mapped.vars.physical.h.xx =
            1.0 + scale * (direction.hxx - 1.0);
        mapped.vars.physical.h.xz = scale * direction.hxz;
        mapped.vars.physical.h.zz =
            1.0 + scale * (direction.hzz - 1.0);
        mapped.vars.physical.Gamma.x = scale * direction.GammaX;
        mapped.vars.physical.Gamma.z = scale * direction.GammaZ;
        Algebraic::reconstruct(mapped.vars);

        const double qn_hxx = scale * direction.qn_hxx;
        const double qn_hxz = scale * direction.qn_hxz;
        const double qn_hzz = scale * direction.qn_hzz;
        const double dz_hxx = scale * direction.dz_hxx;
        const double dz_hxz = scale * direction.dz_hxz;
        const double dz_hzz = scale * direction.dz_hzz;
        const double delta =
            mapped.vars.physical.h.xx * mapped.vars.physical.h.zz -
            mapped.vars.physical.h.xz * mapped.vars.physical.h.xz;
        const double qn_delta =
            mapped.vars.physical.h.zz * qn_hxx +
            mapped.vars.physical.h.xx * qn_hzz -
            2.0 * mapped.vars.physical.h.xz * qn_hxz;
        const double dz_delta =
            mapped.vars.physical.h.zz * dz_hxx +
            mapped.vars.physical.h.xx * dz_hzz -
            2.0 * mapped.vars.physical.h.xz * dz_hxz;
        const double qn_hww =
            -mapped.vars.physical.h.ww * qn_delta / (2.0 * delta);
        const double dz_hww =
            -mapped.vars.physical.h.ww * dz_delta / (2.0 * delta);
        const double nx = static_cast<double>(normal);
        mapped.derivatives[Production::c_hxx].dx = nx * qn_hxx;
        mapped.derivatives[Production::c_hxz].dx = nx * qn_hxz;
        mapped.derivatives[Production::c_hzz].dx = nx * qn_hzz;
        mapped.derivatives[Production::c_hww].dx = nx * qn_hww;
        mapped.derivatives[Production::c_hxx].dz = dz_hxx;
        mapped.derivatives[Production::c_hxz].dz = dz_hxz;
        mapped.derivatives[Production::c_hzz].dz = dz_hzz;
        mapped.derivatives[Production::c_hww].dz = dz_hww;

        const Target::ExpandedInput expanded = Target::expand_target(mapped);
        const auto inverse =
            TensorAlgebra::compute_inverse_sym(expanded.vars.h);
        const auto christoffel =
            TensorAlgebra::compute_christoffel(expanded.d1.h, inverse);
        signed_values[static_cast<std::size_t>(side)] = {
            christoffel.contracted[0],
            christoffel.contracted[1],
            0.5 * (expanded.vars.Gamma[0] - christoffel.contracted[0]),
            0.5 * (expanded.vars.Gamma[1] - christoffel.contracted[1])};
    }
    std::array<double, 4> result{};
    for (int component = 0; component < 4; ++component)
    {
        result[static_cast<std::size_t>(component)] =
            (signed_values[1][static_cast<std::size_t>(component)] -
             signed_values[0][static_cast<std::size_t>(component)]) /
            (2.0 * step);
    }
    return result;
}

// Evaluate the same expanded target metric and contracted-Christoffel object
// in extended precision. The production mapper remains the independently
// exercised double path above; extended precision is used only by this
// pre-launch validation gate so that the locked derivative-level absolute
// tolerance is not weakened by subtracting two O(1) double values.
inline std::array<double, 4>
live_mapped_extended_linearization(const double x, const int normal,
                                   const IndependentMetricJet &direction,
                                   const double step)
{
    if (!(step > 0.0) || !std::isfinite(step) ||
        (normal != -1 && normal != 1))
    {
        throw std::domain_error("M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
    }
    using extended_t = long double;
    using Metric = Tensor<2, extended_t, 4>;
    using FirstMetric = Tensor<2, Tensor<1, extended_t, 4>, 4>;
    std::array<std::array<extended_t, 4>, 2> signed_values{};
    for (int side = 0; side < 2; ++side)
    {
        const extended_t sign = side == 0 ? -1.0L : 1.0L;
        const extended_t scale = sign * static_cast<extended_t>(step);
        const extended_t radial = static_cast<extended_t>(x);
        const extended_t nx = static_cast<extended_t>(normal);
        const extended_t hxx =
            1.0L + scale * static_cast<extended_t>(direction.hxx - 1.0);
        const extended_t hxz =
            scale * static_cast<extended_t>(direction.hxz);
        const extended_t hzz =
            1.0L + scale * static_cast<extended_t>(direction.hzz - 1.0);
        const extended_t delta = hxx * hzz - hxz * hxz;
        const extended_t hww = 1.0L / std::sqrt(delta);

        const extended_t qn_hxx =
            scale * static_cast<extended_t>(direction.qn_hxx);
        const extended_t qn_hxz =
            scale * static_cast<extended_t>(direction.qn_hxz);
        const extended_t qn_hzz =
            scale * static_cast<extended_t>(direction.qn_hzz);
        const extended_t dz_hxx =
            scale * static_cast<extended_t>(direction.dz_hxx);
        const extended_t dz_hxz =
            scale * static_cast<extended_t>(direction.dz_hxz);
        const extended_t dz_hzz =
            scale * static_cast<extended_t>(direction.dz_hzz);
        const extended_t qn_delta =
            hzz * qn_hxx + hxx * qn_hzz - 2.0L * hxz * qn_hxz;
        const extended_t dz_delta =
            hzz * dz_hxx + hxx * dz_hzz - 2.0L * hxz * dz_hxz;
        const extended_t qn_hww = -hww * qn_delta / (2.0L * delta);
        const extended_t dz_hww = -hww * dz_delta / (2.0L * delta);

        Metric metric = 0.0L;
        metric[0][0] = hxx;
        metric[0][1] = metric[1][0] = hxz;
        metric[1][1] = hzz;
        metric[2][2] = metric[3][3] = hww;
        FirstMetric first = 0.0L;
        first[0][0][0] = nx * qn_hxx;
        first[0][1][0] = first[1][0][0] = nx * qn_hxz;
        first[1][1][0] = nx * qn_hzz;
        first[2][2][0] = first[3][3][0] = nx * qn_hww;
        first[0][0][1] = dz_hxx;
        first[0][1][1] = first[1][0][1] = dz_hxz;
        first[1][1][1] = dz_hzz;
        first[2][2][1] = first[3][3][1] = dz_hww;
        for (const int hidden : {2, 3})
        {
            first[hidden][0][hidden] = first[0][hidden][hidden] =
                (hxx - hww) / radial;
            first[hidden][1][hidden] = first[1][hidden][hidden] =
                hxz / radial;
        }

        const auto inverse = TensorAlgebra::compute_inverse_sym(metric);
        const auto christoffel =
            TensorAlgebra::compute_christoffel(first, inverse);
        const extended_t gamma_x =
            scale * static_cast<extended_t>(direction.GammaX);
        const extended_t gamma_z =
            scale * static_cast<extended_t>(direction.GammaZ);
        signed_values[static_cast<std::size_t>(side)] = {
            christoffel.contracted[0], christoffel.contracted[1],
            0.5L * (gamma_x - christoffel.contracted[0]),
            0.5L * (gamma_z - christoffel.contracted[1])};
    }
    std::array<double, 4> result{};
    for (int component = 0; component < 4; ++component)
    {
        result[static_cast<std::size_t>(component)] = static_cast<double>(
            (signed_values[1][static_cast<std::size_t>(component)] -
             signed_values[0][static_cast<std::size_t>(component)]) /
            (2.0L * static_cast<extended_t>(step)));
    }
    return result;
}

inline std::uint64_t next_random(std::uint64_t &state)
{
    state ^= state << 13U;
    state ^= state >> 7U;
    state ^= state << 17U;
    return state;
}

inline double signed_quarter(std::uint64_t &state)
{
    const double unit = static_cast<double>(next_random(state) >> 11U) /
                        static_cast<double>(std::uint64_t{1} << 53U);
    return 0.5 * (unit - 0.5);
}

inline void validate_case(const double x, const int normal,
                          const IndependentMetricJet &jet, const double r0)
{
    constexpr double h_fd = 1.0 / 1048576.0;
    constexpr double absolute_tolerance_factor = 5.0e-12;
    constexpr double normalized_tolerance = 2.0e-11;
    constexpr double refinement_tolerance = 5.0e-9;
    const Result helper = evaluate(x, normal, jet);
    const std::array<double, 4> expected = {helper.gX, helper.gZ, helper.ZX,
                                            helper.ZZ};
    const auto primary =
        live_mapped_extended_linearization(x, normal, jet, h_fd);
    const auto refined =
        live_mapped_extended_linearization(x, normal, jet, 0.5 * h_fd);
    const auto live_double =
        live_mapped_signed_linearization(x, normal, jet, h_fd);
    for (int component = 0; component < 4; ++component)
    {
        const double reference = expected[static_cast<std::size_t>(component)];
        const double absolute_error =
            std::abs(primary[static_cast<std::size_t>(component)] -
                     reference);
        if (absolute_error > absolute_tolerance_factor / r0 ||
            absolute_error / (1.0 / r0 + std::abs(reference)) >
                normalized_tolerance ||
            std::abs(primary[static_cast<std::size_t>(component)] -
                     refined[static_cast<std::size_t>(component)]) /
                    (1.0 / r0 +
                     std::abs(refined[static_cast<std::size_t>(component)])) >
                refinement_tolerance ||
            std::abs(live_double[static_cast<std::size_t>(component)] -
                     primary[static_cast<std::size_t>(component)]) /
                    (1.0 / r0 + std::abs(reference)) >
                refinement_tolerance)
        {
            throw std::domain_error(
                "M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
        }
    }
}

inline void validate_locked_gate(const double r0)
{
    if (!std::isfinite(r0) || !(r0 > 0.0))
    {
        throw std::domain_error("M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
    }
    constexpr std::array<double, 6> ratios = {
        0.5, 0.5625, 1.0, 2.0, 4.4375, 4.5};

    const std::array<IndependentMetricJet, 7> analytic = {
        IndependentMetricJet{},
        IndependentMetricJet{1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.21 / r0, 0.0},
        IndependentMetricJet{1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                             0.0, 0.0, -0.17 / r0},
        IndependentMetricJet{1.17, 0.0, 0.91, 0.23 / r0, 0.0,
                             -0.11 / r0, 0.0, 0.0, 0.0, 0.0, 0.0},
        IndependentMetricJet{1.0, 0.14, 1.0, 0.0, -0.19 / r0, 0.0, 0.0,
                             0.0, 0.0, 0.0, 0.0},
        IndependentMetricJet{1.0, -0.12, 1.0, 0.0, 0.0, 0.0, 0.0,
                             0.18 / r0, 0.0, 0.0, 0.0},
        IndependentMetricJet{1.13, 0.09, 0.86, 0.17 / r0, -0.15 / r0,
                             0.08 / r0, -0.12 / r0, 0.19 / r0,
                             0.11 / r0, 0.07 / r0, -0.05 / r0}};
    for (const double ratio : ratios)
    {
        for (const auto &jet : analytic)
        {
            for (const int normal : {-1, 1})
            {
                validate_case(ratio * r0, normal, jet, r0);
            }
        }
    }

    std::uint64_t random_state = 0x4d32415aULL;
    for (int sample = 0; sample < 256; ++sample)
    {
        IndependentMetricJet jet;
        jet.hxx += signed_quarter(random_state);
        jet.hxz = signed_quarter(random_state);
        jet.hzz += signed_quarter(random_state);
        jet.qn_hxx = signed_quarter(random_state) / r0;
        jet.qn_hxz = signed_quarter(random_state) / r0;
        jet.qn_hzz = signed_quarter(random_state) / r0;
        jet.dz_hxx = signed_quarter(random_state) / r0;
        jet.dz_hxz = signed_quarter(random_state) / r0;
        jet.dz_hzz = signed_quarter(random_state) / r0;
        jet.GammaX = signed_quarter(random_state) / r0;
        jet.GammaZ = signed_quarter(random_state) / r0;
        const double x = ratios[static_cast<std::size_t>(sample % 6)] * r0;
        for (const int normal : {-1, 1})
        {
            validate_case(x, normal, jet, r0);
        }

        IndependentMetricJet parity = jet;
        parity.hxz = -parity.hxz;
        parity.qn_hxz = -parity.qn_hxz;
        parity.dz_hxx = -parity.dz_hxx;
        parity.dz_hzz = -parity.dz_hzz;
        parity.GammaZ = -parity.GammaZ;
        const Result plus = evaluate(x, 1, jet);
        const Result reflected = evaluate(x, 1, parity);
        if (std::abs(plus.gX - reflected.gX) > 5.0e-12 / r0 ||
            std::abs(plus.gZ + reflected.gZ) > 5.0e-12 / r0 ||
            std::abs(plus.ZX - reflected.ZX) > 5.0e-12 / r0 ||
            std::abs(plus.ZZ + reflected.ZZ) > 5.0e-12 / r0)
        {
            throw std::domain_error(
                "M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
        }
    }

    IndependentMetricJet mutation{1.2, 0.17, 0.9, 0.21 / r0,
                                  -0.18 / r0, -0.13 / r0, -0.16 / r0,
                                  0.19 / r0, 0.14 / r0, 0.11 / r0,
                                  -0.12 / r0};
    const double x = 2.0 * r0;
    const Result actual = evaluate(x, 1, mutation);
    const double delta_hww = actual.hww - 1.0;
    const double one_hidden_gx =
        actual.gX - ((mutation.hxx - 1.0) - delta_hww) / x;
    const double one_hidden_gz = actual.gZ - mutation.hxz / x;
    const double deleted_x_coupling =
        actual.gX - 2.0 * ((mutation.hxx - 1.0) - delta_hww) / x;
    const double deleted_z_coupling = actual.gZ - 2.0 * mutation.hxz / x;
    const double wrong_dz_x = actual.gX - 2.0 * mutation.dz_hxz;
    const double wrong_dz_z =
        actual.gZ + mutation.dz_hxx - mutation.dz_hzz +
        2.0 * actual.dz_hww;
    const double wrong_normal_x = evaluate(x, -1, mutation).gX;
    const double wrong_normal_z = evaluate(x, -1, mutation).gZ;
    const double independent_hww_x =
        actual.gX + 2.0 * (delta_hww - 0.23) / x;
    const double removed_half_x = mutation.GammaX - actual.gX;
    const double removed_half_z = mutation.GammaZ - actual.gZ;
    const double minimum = 1.0e-6 / r0;
    if (std::abs(actual.gX - one_hidden_gx) <= minimum ||
        std::abs(actual.gZ - one_hidden_gz) <= minimum ||
        std::abs(actual.gX - deleted_x_coupling) <= minimum ||
        std::abs(actual.gZ - deleted_z_coupling) <= minimum ||
        std::abs(actual.gX - wrong_dz_x) <= minimum ||
        std::abs(actual.gZ - wrong_dz_z) <= minimum ||
        std::abs(actual.gX - wrong_normal_x) <= minimum ||
        std::abs(actual.gZ - wrong_normal_z) <= minimum ||
        std::abs(actual.gX - independent_hww_x) <= minimum ||
        std::abs(actual.ZX - removed_half_x) <= minimum ||
        std::abs(actual.ZZ - removed_half_z) <= minimum)
    {
        throw std::domain_error("M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE");
    }
}

} // namespace BlackStringGammaZHelper

#endif /* BLACKSTRINGGAMMAZHELPER_HPP */
