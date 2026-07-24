#ifndef BLACKSTRINGTARGETCLEANUPCONSTRAINTSSOURCE_HPP
#define BLACKSTRINGTARGETCLEANUPCONSTRAINTSSOURCE_HPP

#include <algorithm>
#include <cmath>

#include "Tensor.hpp"
template <class data_t> struct emtensor_t;
#include "MovingPunctureGauge.hpp"

#include "BlackStringTargetCCZ4Pointwise.hpp"
#include "ConformalCartoonAlgebra.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace BlackStringTargetCleanupConstraintsSource
{
namespace Target = BlackStringTargetCCZ4Pointwise;
namespace Reduced = BlackStringReducedVars;
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace Production = BlackStringProductionVariables;

inline constexpr int target_dimension = Target::target_dimension;
inline constexpr int hidden_multiplicity = Target::hidden_multiplicity;
inline constexpr double target_trace_fraction = 0.25;
inline constexpr std::size_t constraint_outputs = 3;

static_assert(target_dimension == 4);
static_assert(hidden_multiplicity == 2);
static_assert(Production::NUM_VARS == 18);
static_assert(!Production::uses_stock_visible_y_as_hidden);

struct CleanupReport
{
    Reduced::Variables<double> cleaned{};
    double determinant_before = 0.0;
    double determinant_after = 0.0;
    double weighted_trace_before = 0.0;
    double weighted_trace_after = 0.0;
    double visible_stock_trace_before = 0.0;
    double hidden_trace_correction = 0.0;
    std::size_t hww_writes = 0;
    std::size_t Aww_writes = 0;
};

struct DefaultCleanupPolicy
{
    static constexpr int determinant_dimension = target_dimension;
    static constexpr int determinant_hidden_copies = hidden_multiplicity;
    static constexpr int trace_hidden_copies = hidden_multiplicity;
    static constexpr double trace_fraction = target_trace_fraction;
    static constexpr bool include_off_diagonal_trace = true;
    static constexpr bool apply_determinant_cleanup = true;
    static constexpr bool apply_trace_cleanup = true;
    static constexpr int hww_output_copies = 1;
    static constexpr int Aww_output_copies = 1;
};

inline void require_admissible_metric(
    const Reduced::ReducedSymmetricTensor<double> &h)
{
    const double values[] = {h.xx, h.xz, h.zz, h.ww};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "black-string cleanup requires finite metric components");
        }
    }
    const double reduced = h.xx * h.zz - h.xz * h.xz;
    const double determinant = reduced * h.ww * h.ww;
    if (!(h.xx > 0.0) || !(h.ww > 0.0) || !(reduced > 0.0) ||
        !std::isfinite(determinant) || !(determinant > 0.0))
    {
        throw std::domain_error(
            "black-string cleanup requires a finite positive-definite "
            "nonsingular reduced metric");
    }
}

inline double determinant(
    const Reduced::ReducedSymmetricTensor<double> &h)
{
    return (h.xx * h.zz - h.xz * h.xz) * h.ww * h.ww;
}

template <class policy_t>
inline double policy_determinant(
    const Reduced::ReducedSymmetricTensor<double> &h)
{
    const double visible = h.xx * h.zz - h.xz * h.xz;
    double result = visible;
    for (int copy = 0; copy < policy_t::determinant_hidden_copies; ++copy)
    {
        result *= h.ww;
    }
    return result;
}

template <class policy_t>
inline double policy_trace(
    const Reduced::ReducedSymmetricTensor<double> &A,
    const Reduced::ReducedSymmetricTensor<double> &h)
{
    const double visible_determinant = h.xx * h.zz - h.xz * h.xz;
    const double inv_xx = h.zz / visible_determinant;
    const double inv_xz = -h.xz / visible_determinant;
    const double inv_zz = h.xx / visible_determinant;
    const double off_diagonal =
        policy_t::include_off_diagonal_trace ? 2.0 * inv_xz * A.xz : 0.0;
    return inv_xx * A.xx + off_diagonal + inv_zz * A.zz +
           static_cast<double>(policy_t::trace_hidden_copies) * A.ww / h.ww;
}

template <class policy_t = DefaultCleanupPolicy>
inline CleanupReport cleanup_with_policy(
    const Reduced::Variables<double> &input, const policy_t & = policy_t{})
{
    require_admissible_metric(input.physical.h);
    CleanupReport report;
    report.cleaned = input;
    report.determinant_before = determinant(input.physical.h);

    const auto original_h = input.physical.h;
    const auto original_A = input.physical.A;
    const double visible_determinant =
        original_h.xx * original_h.zz - original_h.xz * original_h.xz;
    report.visible_stock_trace_before =
        original_h.zz * original_A.xx / visible_determinant -
        2.0 * original_h.xz * original_A.xz / visible_determinant +
        original_h.xx * original_A.zz / visible_determinant;
    report.hidden_trace_correction =
        2.0 * original_A.ww / original_h.ww;
    report.weighted_trace_before =
        policy_trace<DefaultCleanupPolicy>(original_A, original_h);

    if constexpr (policy_t::apply_determinant_cleanup)
    {
        const double policy_det = policy_determinant<policy_t>(original_h);
        if (!std::isfinite(policy_det) || !(policy_det > 0.0))
        {
            throw std::domain_error(
                "black-string cleanup policy produced invalid determinant");
        }
        const double factor =
            std::pow(policy_det, -1.0 /
                                     static_cast<double>(
                                         policy_t::determinant_dimension));
        report.cleaned.physical.h.xx = factor * original_h.xx;
        report.cleaned.physical.h.xz = factor * original_h.xz;
        report.cleaned.physical.h.zz = factor * original_h.zz;
        report.cleaned.physical.h.ww =
            static_cast<double>(policy_t::hww_output_copies) * factor *
            original_h.ww;
        report.hww_writes = 1;
    }

    if constexpr (policy_t::apply_trace_cleanup)
    {
        const auto &h = report.cleaned.physical.h;
        const double trace = policy_trace<policy_t>(original_A, h);
        report.cleaned.physical.A.xx =
            original_A.xx - policy_t::trace_fraction * h.xx * trace;
        report.cleaned.physical.A.xz =
            original_A.xz - policy_t::trace_fraction * h.xz * trace;
        report.cleaned.physical.A.zz =
            original_A.zz - policy_t::trace_fraction * h.zz * trace;
        report.cleaned.physical.A.ww =
            static_cast<double>(policy_t::Aww_output_copies) *
            (original_A.ww - policy_t::trace_fraction * h.ww * trace);
        report.Aww_writes = 1;
    }

    report.determinant_after = determinant(report.cleaned.physical.h);
    report.weighted_trace_after =
        policy_trace<DefaultCleanupPolicy>(report.cleaned.physical.A,
                                           report.cleaned.physical.h);
    return report;
}

inline CleanupReport cleanup(const Reduced::Variables<double> &input)
{
    return cleanup_with_policy(input, DefaultCleanupPolicy{});
}

struct ConstraintValues
{
    double hamiltonian = 0.0;
    std::array<double, 2> momentum = {};
};

struct ConstraintResult
{
    ConstraintValues target_hidden_suppressed{};
    ConstraintValues hidden_sensitive_increment{};
    ConstraintValues target_total{};
};

struct DefaultConstraintPolicy
{
    static constexpr double hamiltonian_k_squared_coefficient = 0.75;
    static constexpr double ricci_coefficient = 1.0;
    static constexpr int active_hidden_A_copies = hidden_multiplicity;
    static constexpr bool include_off_diagonal_A = true;
    static constexpr bool use_trace_contaminated_K_reconstruction = false;
    static constexpr double momentum_k_gradient_coefficient = 0.75;
    static constexpr bool include_hidden_momentum_derivatives = true;
    static constexpr bool include_off_diagonal_momentum_A = true;
    static constexpr double momentum_x_scale = 1.0;
    static constexpr double momentum_z_scale = 1.0;
    static constexpr bool leak_hidden_reduction_into_visible = false;

    void record_direct_grchombo_ricci(const double) const {}
};

inline ConstraintValues subtract(const ConstraintValues &left,
                                 const ConstraintValues &right)
{
    return {left.hamiltonian - right.hamiltonian,
            {left.momentum[0] - right.momentum[0],
             left.momentum[1] - right.momentum[1]}};
}

template <class policy_t>
inline Target::ExpandedInput
constraint_formula_input(Target::ExpandedInput input, const double x,
                         const bool complete_hidden_A_derivatives)
{
    if (complete_hidden_A_derivatives)
    {
        for (const int hidden :
             {Target::first_hidden_direction,
              Target::second_hidden_direction})
        {
            input.d1.A[hidden][0][hidden] =
                input.d1.A[0][hidden][hidden] =
                    (input.vars.A[0][0] -
                     input.vars.A[hidden][hidden]) /
                    x;
            input.d1.A[hidden][1][hidden] =
                input.d1.A[1][hidden][hidden] =
                    input.vars.A[0][1] / x;
        }
    }
    if constexpr (policy_t::active_hidden_A_copies == 1)
    {
        input.vars.A[Target::second_hidden_direction]
                    [Target::second_hidden_direction] = 0.0;
        for (int derivative = 0; derivative < target_dimension; ++derivative)
        {
            input.d1.A[Target::second_hidden_direction]
                      [Target::second_hidden_direction][derivative] = 0.0;
        }
    }
    if constexpr (!policy_t::include_off_diagonal_A)
    {
        input.vars.A[0][1] = input.vars.A[1][0] = 0.0;
        for (int derivative = 0; derivative < target_dimension; ++derivative)
        {
            input.d1.A[0][1][derivative] =
                input.d1.A[1][0][derivative] = 0.0;
        }
    }
    return input;
}

template <class policy_t>
inline double trace_contaminated_hamiltonian(
    const Target::ExpandedInput &input,
    const Tensor<2, double, target_dimension> &h_inverse,
    const double scalar_ricci)
{
    // Test-policy negative control only: this reproduces the rejected former
    // construction and is unreachable from DefaultConstraintPolicy.
    Tensor<2, double, target_dimension> physical_K = 0.0;
    const double chi = input.vars.chi;
    for (int i = 0; i < target_dimension; ++i)
    {
        for (int j = 0; j < target_dimension; ++j)
        {
            physical_K[i][j] =
                (input.vars.A[i][j] +
                 target_trace_fraction * input.vars.h[i][j] *
                     input.vars.K) /
                chi;
        }
    }
    double K_IJ_K_UU = 0.0;
    for (int i = 0; i < target_dimension; ++i)
    {
        for (int j = 0; j < target_dimension; ++j)
        {
            for (int m = 0; m < target_dimension; ++m)
            {
                for (int n = 0; n < target_dimension; ++n)
                {
                    K_IJ_K_UU +=
                        chi * h_inverse[i][m] * chi * h_inverse[j][n] *
                        physical_K[i][j] * physical_K[m][n];
                }
            }
        }
    }
    return scalar_ricci + input.vars.K * input.vars.K - K_IJ_K_UU;
}

template <class policy_t = DefaultConstraintPolicy>
inline ConstraintValues evaluate_expanded_constraints(
    const Target::ExpandedInput &raw_input,
    const double x, const bool complete_hidden_A_derivatives,
    const policy_t &policy = policy_t{})
{
    if (!std::isfinite(raw_input.vars.chi) ||
        !(raw_input.vars.chi > 0.0))
    {
        throw std::domain_error(
            "black-string constraints require finite chi>0");
    }
    const Target::ExpandedInput input =
        constraint_formula_input<policy_t>(
            raw_input, x, complete_hidden_A_derivatives);
    const auto h_inverse =
        TensorAlgebra::compute_inverse_sym(input.vars.h);
    const auto christoffel =
        TensorAlgebra::compute_christoffel(input.d1.h, h_inverse);

    // Constraints::constraint_equations is Cell/finite-difference owned and its
    // target mapping cannot be called safely with CH_SPACEDIM=2. The geometry
    // path remains direct compiled GRChombo; only the exact locked formula
    // below is transcribed as a target-(d=4) source/convention adapter.
    const auto ricci = CCZ4Geometry::compute_ricci(
        input.vars, input.d1, input.d2, h_inverse, christoffel);
    policy.record_direct_grchombo_ricci(ricci.scalar);

    const auto A_UU = TensorAlgebra::raise_all(input.vars.A, h_inverse);
    const double tr_A2 =
        TensorAlgebra::compute_trace(input.vars.A, A_UU);

    ConstraintValues result;
    if constexpr (policy_t::use_trace_contaminated_K_reconstruction)
    {
        result.hamiltonian = trace_contaminated_hamiltonian<policy_t>(
            input, h_inverse,
            policy_t::ricci_coefficient * ricci.scalar);
    }
    else
    {
        result.hamiltonian =
            policy_t::ricci_coefficient * ricci.scalar +
            policy_t::hamiltonian_k_squared_coefficient *
                input.vars.K * input.vars.K -
            tr_A2;
    }

    Target::ExpandedInput momentum_input = input;
    if constexpr (!policy_t::include_off_diagonal_momentum_A)
    {
        momentum_input.vars.A[0][1] =
            momentum_input.vars.A[1][0] = 0.0;
        for (int derivative = 0; derivative < target_dimension; ++derivative)
        {
            momentum_input.d1.A[0][1][derivative] =
                momentum_input.d1.A[1][0][derivative] = 0.0;
        }
    }
    Tensor<3, double, target_dimension> covd_A = 0.0;
    for (int derivative = 0; derivative < target_dimension; ++derivative)
    {
        for (int j = 0; j < target_dimension; ++j)
        {
            for (int k = 0; k < target_dimension; ++k)
            {
                covd_A[derivative][j][k] =
                    momentum_input.d1.A[j][k][derivative];
                for (int l = 0; l < target_dimension; ++l)
                {
                    covd_A[derivative][j][k] -=
                        christoffel.ULL[l][derivative][j] *
                            momentum_input.vars.A[l][k] +
                        christoffel.ULL[l][derivative][k] *
                            momentum_input.vars.A[l][j];
                }
            }
        }
    }

    const double chi_regularised = std::max(1.0e-6, input.vars.chi);
    std::array<double, Target::gridded_dimension> hidden_contribution = {};
    for (int i = 0; i < Target::gridded_dimension; ++i)
    {
        double momentum =
            -policy_t::momentum_k_gradient_coefficient * input.d1.K[i];
        for (int j = 0; j < target_dimension; ++j)
        {
            for (int k = 0; k < target_dimension; ++k)
            {
                const bool hidden_term =
                    j >= Target::gridded_dimension ||
                    k >= Target::gridded_dimension;
                const double term =
                    h_inverse[j][k] *
                    (covd_A[k][j][i] -
                     target_dimension * momentum_input.vars.A[i][j] *
                         input.d1.chi[k] /
                         (2.0 * chi_regularised));
                if (hidden_term)
                {
                    hidden_contribution[static_cast<std::size_t>(i)] += term;
                    if constexpr (!policy_t::
                                      include_hidden_momentum_derivatives)
                    {
                        continue;
                    }
                }
                momentum += term;
            }
        }
        result.momentum[static_cast<std::size_t>(i)] = momentum;
    }
    if constexpr (policy_t::leak_hidden_reduction_into_visible)
    {
        result.momentum[0] += hidden_contribution[1];
        result.momentum[1] += hidden_contribution[0];
    }
    result.momentum[0] *= policy_t::momentum_x_scale;
    result.momentum[1] *= policy_t::momentum_z_scale;
    return result;
}

template <class expansion_policy_t = Target::DefaultExpansionPolicy,
          class constraint_policy_t = DefaultConstraintPolicy>
inline ConstraintResult evaluate_constraints_with_policies(
    const Target::Input &input,
    const expansion_policy_t &expansion_policy = expansion_policy_t{},
    const constraint_policy_t &constraint_policy = constraint_policy_t{})
{
    Target::require_valid_input(input);
    const auto full = expansion_policy.expand(input);
    const auto suppressed = Target::suppress_hidden(full);
    ConstraintResult result;
    result.target_total =
        evaluate_expanded_constraints(full, input.x, true,
                                      constraint_policy);
    result.target_hidden_suppressed =
        evaluate_expanded_constraints(suppressed, input.x, false,
                                      constraint_policy);
    result.hidden_sensitive_increment =
        subtract(result.target_total, result.target_hidden_suppressed);
    return result;
}

inline ConstraintResult evaluate_constraints(const Target::Input &input)
{
    return evaluate_constraints_with_policies(input);
}

struct GaugeValues
{
    double lapse = 0.0;
    std::array<double, 2> shift = {};
    std::array<double, 2> B = {};
};

struct FixedGPLapseSourceResult
{
    GaugeValues moving_puncture_raw{};
    GaugeValues with_fixed_source{};
    double fixed_source = 0.0;
};

struct DefaultSourcePolicy
{
    static double value(const double r0, const double x,
                        const Reduced::Variables<double> &)
    {
        return 3.0 * std::sqrt(r0 / (x * x * x));
    }

    static void apply(Target::TargetVars<double> &rhs,
                      const double source)
    {
        rhs.lapse += source;
    }
};

inline GaugeValues gauge_values_from(
    const Target::TargetVars<double> &rhs)
{
    return {rhs.lapse,
            {rhs.shift[0], rhs.shift[1]},
            {rhs.B[0], rhs.B[1]}};
}

template <class source_policy_t = DefaultSourcePolicy>
inline FixedGPLapseSourceResult evaluate_fixed_gp_lapse_source_with_policy(
    const double locked_r0, const Target::Input &input,
    const source_policy_t & = source_policy_t{})
{
    if (!std::isfinite(locked_r0) || !(locked_r0 > 0.0))
    {
        throw std::domain_error(
            "fixed GP lapse source requires locked r0>0");
    }
    Target::require_valid_input(input);
    const auto expanded = Target::expand_target(input);
    auto rhs = Target::evaluate_direct(expanded, input.parameters);

    MovingPunctureGauge::params_t parameters;
    parameters.lapse_advec_coeff = 0.0;
    parameters.lapse_power = 1.0;
    parameters.lapse_coeff = 2.0;
    parameters.shift_Gamma_coeff = 0.75;
    parameters.shift_advec_coeff = 0.0;
    parameters.eta = 1.0;
    const MovingPunctureGauge gauge(parameters);
    gauge.rhs_gauge(rhs, expanded.vars, expanded.d1, expanded.d2,
                    expanded.advection);

    FixedGPLapseSourceResult result;
    result.moving_puncture_raw = gauge_values_from(rhs);
    result.fixed_source =
        source_policy_t::value(locked_r0, input.x, input.vars);
    source_policy_t::apply(rhs, result.fixed_source);
    result.with_fixed_source = gauge_values_from(rhs);
    return result;
}

inline FixedGPLapseSourceResult
evaluate_fixed_gp_lapse_source(const double locked_r0,
                               const Target::Input &input)
{
    return evaluate_fixed_gp_lapse_source_with_policy(locked_r0, input);
}

} // namespace BlackStringTargetCleanupConstraintsSource

#endif /* BLACKSTRINGTARGETCLEANUPCONSTRAINTSSOURCE_HPP */
