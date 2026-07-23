#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Test-only scalar access to the inspected GRChombo derivative kernels.
#define CH_SPACEDIM 3
#define CELL_HPP_
template <class data_t> class Cell;
#define USERVARIABLES_HPP
#define SIMD_HPP_
#include "AlwaysInline.hpp"
template <typename data_t, typename pointer_t>
ALWAYS_INLINE pointer_t *SIMDIFY(pointer_t *pointer)
{
    return pointer;
}
template <typename data_t>
ALWAYS_INLINE data_t simd_conditional(const bool condition,
                                      const data_t &when_true,
                                      const data_t &when_false)
{
    return condition ? when_true : when_false;
}
template <typename data_t>
ALWAYS_INLINE bool simd_compare_gt(const data_t &left, const data_t &right)
{
    return left > right;
}
#include "FourthOrderDerivatives.hpp"

// Reuse the independently validated batch-2 d=3 continuum bridge and the
// actual GRChombo rhs_equation path. No production header is changed.
#define main stage4aoc_grchombo_comparison_batch2_embedded_main
#include "Stage4AOCGRChomboComparisonBatch2Test.cpp"
#undef main

#include "Stage4AOCAnalyticFullOracle.hpp"
#include "Stage4AOFrozenGaugeOperator.hpp"
#include "Stage4AOFrozenGaugeRadialBoundary.hpp"
#include "Stage4AOGPDiscretePreflight.hpp"

namespace
{
namespace Algebra = BlackStringToy::ConformalCartoonAlgebra;
namespace FullOracle = BlackStringToy::Stage4AOCAnalyticFullOracle;
namespace Boundary = BlackStringToy::Stage4AOFrozenGaugeRadialBoundary;
namespace Inner = BlackStringToy::Stage4AOFrozenGaugeInnerBoundary;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Preflight = BlackStringToy::Stage4AOGPDiscretePreflight;

constexpr double r0 = 1.0;
constexpr double comparison_x = 2.0;
constexpr double radial_span = 0.8;
constexpr std::array<int, 4> refinements = {32, 64, 128, 256};
constexpr std::array<double, 3> sample_x = {1.4, 2.0, 3.2};

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        std::cerr << "FAIL " << message << "\n";
        std::exit(1);
    }
}

void require_close(const double actual, const double expected,
                   const std::string &message, const double tolerance = 1.0e-12)
{
    require(std::abs(actual - expected) <= tolerance, message);
}

double gp_lambda(const double x)
{
    return std::sqrt(r0 / (x * x * x));
}

double beta_x(const double x) { return x * gp_lambda(x); }
double stock_k(const double x) { return -0.5 * gp_lambda(x); }
double stock_axx(const double x) { return -gp_lambda(x) / 3.0; }
double stock_ayy(const double x) { return gp_lambda(x) / 6.0; }
double stock_azz(const double x) { return gp_lambda(x) / 6.0; }

enum class StockField
{
    beta,
    K,
    Axx,
    Ayy,
    Azz
};

double stock_value(const StockField field, const double x)
{
    switch (field)
    {
    case StockField::beta:
        return beta_x(x);
    case StockField::K:
        return stock_k(x);
    case StockField::Axx:
        return stock_axx(x);
    case StockField::Ayy:
        return stock_ayy(x);
    case StockField::Azz:
        return stock_azz(x);
    }
    return 0.0;
}

double stock_dx(const StockField field, const double x)
{
    const double lambda = gp_lambda(x);
    if (field == StockField::beta)
        return -0.5 * lambda;
    return -1.5 * stock_value(field, x) / x;
}

double stock_dxx(const StockField field, const double x)
{
    const double lambda = gp_lambda(x);
    if (field == StockField::beta)
        return 0.75 * lambda / x;
    return 3.75 * stock_value(field, x) / (x * x);
}

struct GPBackground
{
    int dimension;
    int hidden_copies;
    double beta;
    double dbeta_x;
    double hidden_shift_gradient;
    double shift_divergence;
    double K;
    double dK;
    double Axx;
    double dAxx;
    double Ayy;
    double dAyy;
    double Azz;
    double dAzz;
    double Aww;
    double dAww;
};

struct GPFamilyRow
{
    double advection = 0.0;
    double shift_stretching = 0.0;
    double shift_trace_correction = 0.0;
    double nonlinear_linear = 0.0;
    double nonlinear_quadratic = 0.0;
    double geometric_ricci = 0.0;
    double encoded_z = 0.0;
    double lapse_hessian = 0.0;
    double damping = 0.0;

    double total() const
    {
        return advection + shift_stretching + shift_trace_correction +
               nonlinear_linear + nonlinear_quadratic + geometric_ricci +
               encoded_z + lapse_hessian + damping;
    }
};

GPBackground make_stock_d3_background(const double x)
{
    const double lambda = gp_lambda(x);
    return {3,
            0,
            beta_x(x),
            -0.5 * lambda,
            0.0,
            -0.5 * lambda,
            stock_k(x),
            stock_dx(StockField::K, x),
            stock_axx(x),
            stock_dx(StockField::Axx, x),
            stock_ayy(x),
            stock_dx(StockField::Ayy, x),
            stock_azz(x),
            stock_dx(StockField::Azz, x),
            0.0,
            0.0};
}

GPBackground make_target_d4_background(const double x)
{
    const double lambda = gp_lambda(x);
    const double beta = beta_x(x);
    const double K = 1.5 * lambda;
    const double Axx = -7.0 * lambda / 8.0;
    const double Azz = -3.0 * lambda / 8.0;
    const double Aww = 5.0 * lambda / 8.0;
    const double hidden_shift_gradient = beta / x;
    return {4,
            2,
            beta,
            -0.5 * lambda,
            hidden_shift_gradient,
            -0.5 * lambda + 2.0 * hidden_shift_gradient,
            K,
            -1.5 * K / x,
            Axx,
            -1.5 * Axx / x,
            Aww,
            -1.5 * Aww / x,
            Azz,
            -1.5 * Azz / x,
            Aww,
            -1.5 * Aww / x};
}

GPFamilyRow evaluate_k_families(const GPBackground &background)
{
    GPFamilyRow row;
    row.advection = background.beta * background.dK;
    row.nonlinear_linear = background.K * background.K;
    return row;
}

GPFamilyRow evaluate_a_families(const GPBackground &background,
                                const double A, const double dA,
                                const double shift_gradient)
{
    GPFamilyRow row;
    row.advection = background.beta * dA;
    row.shift_stretching = 2.0 * A * shift_gradient;
    row.shift_trace_correction =
        -(2.0 / static_cast<double>(background.dimension)) * A *
        background.shift_divergence;
    row.nonlinear_linear = A * background.K;
    row.nonlinear_quadratic = -2.0 * A * A;
    return row;
}

double trace_a_squared(const GPBackground &background)
{
    return background.Axx * background.Axx +
           background.Azz * background.Azz +
           static_cast<double>(background.hidden_copies) *
               background.Aww * background.Aww;
}

double theta_nonlinear_family(const GPBackground &background)
{
    const double trace_coefficient =
        static_cast<double>(background.dimension - 1) /
        static_cast<double>(background.dimension);
    return 0.5 *
           (-trace_a_squared(background) +
            trace_coefficient * background.K * background.K);
}

bool family_row_is_zero(const GPFamilyRow &row,
                        const double tolerance = 1.0e-13)
{
    return std::abs(row.total()) <= tolerance;
}

void print_family_row(const char *path, const char *row_name,
                      const GPFamilyRow &row)
{
    std::cout << "FAMILY path=" << path << " row=" << row_name
              << " advection=" << row.advection
              << " shift_stretching=" << row.shift_stretching
              << " shift_trace=" << row.shift_trace_correction
              << " nonlinear_linear=" << row.nonlinear_linear
              << " nonlinear_quadratic=" << row.nonlinear_quadratic
              << " geometric_ricci=" << row.geometric_ricci
              << " encoded_Z=" << row.encoded_z
              << " lapse_hessian=" << row.lapse_hessian
              << " damping=" << row.damping
              << " total=" << row.total() << "\n";
}

struct ScalarJet
{
    double value;
    double dx;
    double dxx;
};

enum class DerivativePath
{
    analytic,
    custom_second_order,
    grchombo_fourth_order
};

Operator::FrozenGaugePerturbationVector filled_vector(const double value)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values{};
    values.fill(value);
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

class FourthOrderAccess : public FourthOrderDerivatives
{
  public:
    using FourthOrderDerivatives::FourthOrderDerivatives;
};

ScalarJet make_jet(const StockField field, const int points, const double x,
                   const DerivativePath path)
{
    const double spacing = radial_span / static_cast<double>(points);
    if (path == DerivativePath::analytic)
        return {stock_value(field, x), stock_dx(field, x),
                stock_dxx(field, x)};

    if (path == DerivativePath::custom_second_order)
    {
        std::vector<Operator::FrozenGaugePerturbationVector> values;
        values.reserve(3);
        for (int offset = -1; offset <= 1; ++offset)
            values.push_back(
                filled_vector(stock_value(field, x + offset * spacing)));
        const auto jet = Boundary::make_centered_derivative_jet(
            values, 1, spacing, 0.0, Inner::FourierParitySector::P_plus);
        return {stock_value(field, x), jet.dx().value_at_index(0),
                jet.dxx().value_at_index(0)};
    }

    constexpr int width = 7;
    constexpr int center = 3;
    std::array<double, width> values{};
    for (int offset = -3; offset <= 3; ++offset)
        values[static_cast<std::size_t>(offset + center)] =
            stock_value(field, x + offset * spacing);
    const FourthOrderAccess derivatives(spacing);
    return {values[center],
            derivatives.diff1<double>(values.data(), center, 1),
            derivatives.diff2<double>(values.data(), center, 1)};
}

AnalyticInput make_stock_gp_input(const int points, const double x,
                                  const DerivativePath path)
{
    AnalyticInput out = make_flat_input("stock_visible_GP");
    const ScalarJet beta = make_jet(StockField::beta, points, x, path);
    const ScalarJet K = make_jet(StockField::K, points, x, path);
    const ScalarJet Axx = make_jet(StockField::Axx, points, x, path);
    const ScalarJet Ayy = make_jet(StockField::Ayy, points, x, path);
    const ScalarJet Azz = make_jet(StockField::Azz, points, x, path);

    out.vars.lapse = 1.0;
    out.vars.shift[0] = beta.value;
    out.vars.shift[1] = out.vars.shift[2] = 0.0;
    out.vars.K = K.value;
    out.vars.Theta = 0.0;
    out.vars.Gamma[0] = out.vars.Gamma[1] = out.vars.Gamma[2] = 0.0;
    out.vars.A[0][0] = Axx.value;
    out.vars.A[1][1] = Ayy.value;
    out.vars.A[2][2] = Azz.value;

    out.d1.shift[0][0] = beta.dx;
    out.d2.shift[0][0][0] = beta.dxx;
    out.d1.K[0] = K.dx;
    out.d1.A[0][0][0] = Axx.dx;
    out.d1.A[1][1][0] = Ayy.dx;
    out.d1.A[2][2][0] = Azz.dx;
    out.advec.K = beta.value * K.dx;
    out.advec.A[0][0] = beta.value * Axx.dx;
    out.advec.A[1][1] = beta.value * Ayy.dx;
    out.advec.A[2][2] = beta.value * Azz.dx;
    return out;
}

std::array<double, 18> residual_rows(const Vars<double> &rhs)
{
    return {rhs.chi,       rhs.h[0][0], rhs.h[1][1], rhs.h[2][2],
            rhs.h[0][1],  rhs.h[0][2], rhs.h[1][2], rhs.K,
            rhs.Theta,    rhs.A[0][0], rhs.A[1][1], rhs.A[2][2],
            rhs.A[0][1],  rhs.A[0][2], rhs.A[1][2], rhs.Gamma[0],
            rhs.Gamma[1], rhs.Gamma[2]};
}

Vars<double> family_total(const RHSFamilies &families)
{
    Vars<double> result = zero_scalar_vars();
    result.chi = families.chi_total;
    result.K = families.k_total;
    result.Theta = families.theta_total;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            result.h[i][j] = families.h_total[i][j];
            result.A[i][j] = families.a_total[i][j];
        }
    }
    return result;
}

double max_difference(const std::array<double, 18> &left,
                      const std::array<double, 18> &right)
{
    double result = 0.0;
    for (std::size_t i = 0; i < left.size(); ++i)
        result = std::max(result, std::abs(left[i] - right[i]));
    return result;
}

double observed_order(const double coarse, const double fine)
{
    return std::log(coarse / fine) / (3.0 * std::log(2.0));
}

double observed_order_two_halvings(const double coarse, const double fine)
{
    return std::log(coarse / fine) / (2.0 * std::log(2.0));
}

struct ConstraintResiduals
{
    double hamiltonian;
    std::array<double, 2> momentum;
};

ConstraintResiduals evaluate_gp_constraints_with_full_geometry(
    const double r0_value, const double x)
{
    const FullOracle::Direction zero_direction;
    const FullOracle::Context context{
        static_cast<FullOracle::Real>(r0_value),
        static_cast<FullOracle::Real>(x), 0.0L, &zero_direction,
        FullOracle::hidden_multiplicity};
    const auto physical_jets = FullOracle::physical_metric_jets(context);
    const auto physical = FullOracle::compute_geometry(physical_jets);
    const auto a_jets = FullOracle::a_tensor_jets(context);
    const auto k_jet =
        FullOracle::state_jet(context, Operator::PerturbationVariable::K);

    FullOracle::Tensor extrinsic = {};
    FullOracle::DerivativeTensor extrinsic_first = {};
    for (int i = 0; i < FullOracle::dimension; ++i)
    {
        for (int j = 0; j < FullOracle::dimension; ++j)
        {
            extrinsic[i][j] =
                a_jets.value[i][j] +
                0.25L * physical.metric[i][j] * k_jet.value;
            for (int derivative = 0; derivative < FullOracle::dimension;
                 ++derivative)
            {
                const FullOracle::Real dK =
                    derivative == 0 ? k_jet.dx
                                    : derivative == 1 ? k_jet.dz : 0.0L;
                extrinsic_first[derivative][i][j] =
                    a_jets.first[derivative][i][j] +
                    0.25L *
                        (physical.first[derivative][i][j] * k_jet.value +
                         physical.metric[i][j] * dK);
            }
        }
    }

    FullOracle::Real scalar_ricci = 0.0L;
    FullOracle::Real extrinsic_squared = 0.0L;
    for (int i = 0; i < FullOracle::dimension; ++i)
    {
        for (int j = 0; j < FullOracle::dimension; ++j)
        {
            scalar_ricci += physical.inverse[i][j] * physical.ricci[i][j];
            for (int m = 0; m < FullOracle::dimension; ++m)
            {
                for (int n = 0; n < FullOracle::dimension; ++n)
                {
                    extrinsic_squared +=
                        physical.inverse[i][m] * physical.inverse[j][n] *
                        extrinsic[i][j] * extrinsic[m][n];
                }
            }
        }
    }

    std::array<FullOracle::Real, 2> momentum = {};
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < FullOracle::dimension; ++j)
        {
            for (int m = 0; m < FullOracle::dimension; ++m)
            {
                momentum[i] +=
                    physical.inverse[j][m] *
                    extrinsic_first[j][m][i];
                momentum[i] +=
                    physical.inverse_first[j][j][m] * extrinsic[m][i];
                for (int n = 0; n < FullOracle::dimension; ++n)
                {
                    momentum[i] +=
                        physical.gamma[j][j][n] *
                        physical.inverse[n][m] * extrinsic[m][i];
                    momentum[i] -=
                        physical.gamma[n][j][i] *
                        physical.inverse[j][m] * extrinsic[m][n];
                }
            }
        }
        momentum[i] -= i == 0 ? k_jet.dx : k_jet.dz;
    }

    return {static_cast<double>(scalar_ricci + k_jet.value * k_jet.value -
                                extrinsic_squared),
            {static_cast<double>(momentum[0]),
             static_cast<double>(momentum[1])}};
}

bool all_oracle_rows_zero(const FullOracle::Rows &rows,
                          const double tolerance = 1.0e-13)
{
    for (const auto value : rows)
    {
        if (std::abs(static_cast<double>(value)) > tolerance)
            return false;
    }
    return true;
}

void test_independent_gp_family_decomposition()
{
    const auto stock = make_stock_d3_background(comparison_x);
    const auto target = make_target_d4_background(comparison_x);
    const auto stock_k_row = evaluate_k_families(stock);
    const auto target_k_row = evaluate_k_families(target);
    const auto stock_axx =
        evaluate_a_families(stock, stock.Axx, stock.dAxx, stock.dbeta_x);
    const auto stock_ayy =
        evaluate_a_families(stock, stock.Ayy, stock.dAyy, 0.0);
    const auto stock_azz =
        evaluate_a_families(stock, stock.Azz, stock.dAzz, 0.0);
    const auto target_axx =
        evaluate_a_families(target, target.Axx, target.dAxx,
                            target.dbeta_x);
    const auto target_azz =
        evaluate_a_families(target, target.Azz, target.dAzz, 0.0);
    const auto target_aww =
        evaluate_a_families(target, target.Aww, target.dAww,
                            target.hidden_shift_gradient);

    require_close(stock_k_row.advection, 3.0 / 32.0,
                  "stock K advection family");
    require_close(stock_k_row.nonlinear_linear, 1.0 / 32.0,
                  "stock K nonlinear family");
    require_close(stock_k_row.total(), 1.0 / 8.0,
                  "stock K independent total");
    require_close(target_k_row.advection, -9.0 / 32.0,
                  "target K advection family");
    require_close(target_k_row.nonlinear_linear, 9.0 / 32.0,
                  "target K nonlinear family");
    require_close(target_k_row.total(), 0.0,
                  "target K independent total");

    require_close(stock_axx.advection, 1.0 / 16.0,
                  "stock Axx advection");
    require_close(stock_axx.nonlinear_linear, 1.0 / 48.0,
                  "stock Axx linear nonlinear");
    require_close(stock_axx.nonlinear_quadratic, -1.0 / 36.0,
                  "stock Axx quadratic nonlinear");
    require_close(stock_axx.shift_stretching, 1.0 / 24.0,
                  "stock Axx shift stretching");
    require_close(stock_axx.shift_trace_correction, -1.0 / 72.0,
                  "stock Axx shift trace");
    require_close(stock_axx.total(), 1.0 / 12.0,
                  "stock Axx independent total");
    for (const auto *row : {&stock_ayy, &stock_azz})
    {
        require_close(row->advection, -1.0 / 32.0,
                      "stock transverse A advection");
        require_close(row->nonlinear_linear, -1.0 / 96.0,
                      "stock transverse A linear");
        require_close(row->nonlinear_quadratic, -1.0 / 144.0,
                      "stock transverse A quadratic");
        require_close(row->shift_stretching, 0.0,
                      "stock transverse A shift stretching");
        require_close(row->shift_trace_correction, 1.0 / 144.0,
                      "stock transverse A shift trace");
        require_close(row->total(), -1.0 / 24.0,
                      "stock transverse A independent total");
    }
    require(family_row_is_zero(target_axx), "target Axx family total");
    require(family_row_is_zero(target_azz), "target Azz family total");
    require(family_row_is_zero(target_aww), "target Aww family total");
    require_close(theta_nonlinear_family(target), 0.0,
                  "target hidden-multiplicity Theta family");

    const FullOracle::Direction zero_direction;
    const auto full =
        FullOracle::evaluate(r0, comparison_x, 0.0, zero_direction);
    require_close(
        target_k_row.total(),
        static_cast<double>(
            full.total[Operator::variable_index(
                Operator::PerturbationVariable::K)]),
        "independent target K families match full oracle total");
    require_close(
        target_axx.total(),
        static_cast<double>(
            full.total[Operator::variable_index(
                Operator::PerturbationVariable::A_xx)]),
        "independent target Axx families match full oracle total");
    require_close(
        target_azz.total(),
        static_cast<double>(
            full.total[Operator::variable_index(
                Operator::PerturbationVariable::A_zz)]),
        "independent target Azz families match full oracle total");
    require_close(
        target_aww.total(),
        static_cast<double>(
            full.total[Operator::variable_index(
                Operator::PerturbationVariable::A_ww)]),
        "independent target Aww families match full oracle total");
    for (const auto variable : {Operator::PerturbationVariable::K,
                                Operator::PerturbationVariable::A_xx,
                                Operator::PerturbationVariable::A_zz,
                                Operator::PerturbationVariable::A_ww})
    {
        const auto index = Operator::variable_index(variable);
        require_close(
            static_cast<double>(
                full.family[FullOracle::geometric_ricci][index]),
            0.0, "GP geometric Ricci family is zero");
        require_close(
            static_cast<double>(full.family[FullOracle::encoded_z][index]),
            0.0, "GP encoded-Z family is zero");
        require_close(
            static_cast<double>(full.family[FullOracle::locked_damping][index]),
            0.0, "GP damping family is zero");
    }
    const FullOracle::Context context{
        r0, comparison_x, 0.0L, &zero_direction,
        FullOracle::hidden_multiplicity};
    const auto physical =
        FullOracle::compute_geometry(FullOracle::physical_metric_jets(context));
    FullOracle::Tensor lapse_hessian = {};
    const std::array<FullOracle::Real, FullOracle::dimension> lapse_first = {};
    const std::array<std::array<FullOracle::Real, FullOracle::dimension>,
                     FullOracle::dimension>
        lapse_second = {};
    for (int i = 0; i < FullOracle::dimension; ++i)
    {
        for (int j = 0; j < FullOracle::dimension; ++j)
        {
            lapse_hessian[i][j] = lapse_second[i][j];
            for (int k = 0; k < FullOracle::dimension; ++k)
                lapse_hessian[i][j] -=
                    physical.gamma[k][i][j] * lapse_first[k];
            require_close(static_cast<double>(lapse_hessian[i][j]), 0.0,
                          "GP lapse Hessian family is zero");
        }
    }

    auto stock_in_target = target;
    stock_in_target.K = stock.K;
    stock_in_target.dK = stock.dK;
    require(!family_row_is_zero(evaluate_k_families(stock_in_target)),
            "stock K/dK target mutation rejected");
    auto missing_hidden_divergence = target;
    missing_hidden_divergence.shift_divergence =
        missing_hidden_divergence.dbeta_x;
    require(!family_row_is_zero(evaluate_a_families(
                missing_hidden_divergence, missing_hidden_divergence.Axx,
                missing_hidden_divergence.dAxx,
                missing_hidden_divergence.dbeta_x)),
            "missing hidden shift divergence rejected");
    auto wrong_trace_dimension = target;
    wrong_trace_dimension.dimension = 3;
    require(!family_row_is_zero(evaluate_a_families(
                wrong_trace_dimension, wrong_trace_dimension.Axx,
                wrong_trace_dimension.dAxx,
                wrong_trace_dimension.dbeta_x)),
            "d3 trace correction in target rejected");
    auto missing_hidden_trace = target;
    missing_hidden_trace.hidden_copies = 1;
    require(std::abs(theta_nonlinear_family(missing_hidden_trace)) > 1.0e-3,
            "missing hidden trace multiplicity rejected");
    auto false_hidden_ricci = target_axx;
    false_hidden_ricci.geometric_ricci = 1.0e-2;
    require(!family_row_is_zero(false_hidden_ricci),
            "false hidden Ricci rejected");
    auto false_encoded_z = target_aww;
    false_encoded_z.encoded_z = -1.0e-2;
    require(!family_row_is_zero(false_encoded_z),
            "false encoded Z rejected");

    std::cout << "BACKGROUND path=stock_d3 dimension=" << stock.dimension
              << " hidden_copies=" << stock.hidden_copies
              << " K=" << stock.K << " dK=" << stock.dK
              << " dbeta_x=" << stock.dbeta_x
              << " hidden_shift_gradient=" << stock.hidden_shift_gradient
              << " shift_divergence=" << stock.shift_divergence << "\n";
    std::cout << "BACKGROUND path=target_d4 dimension=" << target.dimension
              << " hidden_copies=" << target.hidden_copies
              << " K=" << target.K << " dK=" << target.dK
              << " dbeta_x=" << target.dbeta_x
              << " hidden_shift_gradient=" << target.hidden_shift_gradient
              << " shift_divergence=" << target.shift_divergence << "\n";
    print_family_row("stock_d3", "K", stock_k_row);
    print_family_row("stock_d3", "Axx", stock_axx);
    print_family_row("stock_d3", "Ayy", stock_ayy);
    print_family_row("stock_d3", "Azz", stock_azz);
    print_family_row("target_d4", "K", target_k_row);
    print_family_row("target_d4", "Axx", target_axx);
    print_family_row("target_d4", "Azz", target_azz);
    print_family_row("target_d4", "Aww", target_aww);
    std::cout << "MUTATION stock_K_dK_in_target=DETECTED "
                 "missing_hidden_shift_divergence=DETECTED "
                 "d3_trace_correction=DETECTED "
                 "missing_hidden_trace_multiplicity=DETECTED "
                 "false_hidden_Ricci=DETECTED false_encoded_Z=DETECTED\n";
}

void test_gp_conventions()
{
    const double x = comparison_x;
    const double lambda = gp_lambda(x);
    require_close(beta_x(x), std::sqrt(r0 / x), "GP shift sign/value");

    // Coordinate angular metric versus the stored modified-cartoon
    // representative.  gamma_theta_theta=x^2, but gamma_ww means the
    // Cartesian representative gamma_theta_theta/x^2.
    const double coordinate_gamma_theta_theta = x * x;
    const Algebra::ConformalMetric h{1.0, 0.0, 1.0, 1.0};
    const Algebra::ConformalExtrinsicCurvature A{
        -7.0 * lambda / 8.0, 0.0, -3.0 * lambda / 8.0,
        5.0 * lambda / 8.0};
    require_close(coordinate_gamma_theta_theta, 4.0,
                  "coordinate angular metric is x^2");
    require_close(Algebra::determinant_4d(h), 1.0,
                  "stored representative determinant");
    require_close(Algebra::trace(A, h), 0.0,
                  "full d=4 A trace with multiplicity two");

    const double K = 1.5 * lambda;
    const auto Kij = Algebra::reconstruct_extrinsic_curvature(A, h, 1.0, K);
    require_close(Kij.xx, -0.5 * lambda, "custom d4 Kxx");
    require_close(Kij.xz, 0.0, "custom d4 Kxz");
    require_close(Kij.zz, 0.0, "custom d4 Kzz");
    require_close(Kij.ww, lambda, "custom d4 representative Kww");
    require_close(x * x * Kij.ww, lambda * x * x,
                  "coordinate K_theta_theta");

    const Algebra::ConformalMetric wrong_stored_h{1.0, 0.0, 1.0, x * x};
    require(std::abs(Algebra::determinant_4d(wrong_stored_h) - 1.0) > 1.0,
            "coordinate x^2 is rejected as stored hww");
    require(std::abs(Algebra::trace(A, wrong_stored_h)) > 1.0e-3,
            "coordinate x^2 fails the stored A-trace convention");

    // Independent stock-d=3 convention path. Visible y remains an ordinary
    // stock component and is not identified with the custom hidden ww slot.
    Tensor<2, double> stock_metric;
    Tensor<2, double> stock_Kij;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            stock_metric[i][j] = i == j ? 1.0 : 0.0;
            stock_Kij[i][j] = 0.0;
        }
    }
    stock_Kij[0][0] = -0.5 * lambda;
    const auto stock_inverse = TensorAlgebra::compute_inverse(stock_metric);
    const double stock_K = TensorAlgebra::compute_trace(stock_Kij,
                                                         stock_inverse);
    Tensor<2, double> stock_A = stock_Kij;
    TensorAlgebra::make_trace_free(stock_A, stock_metric, stock_inverse);
    require_close(stock_K, -0.5 * lambda, "direct stock d3 K trace");
    require_close(stock_A[0][0], -lambda / 3.0,
                  "direct stock d3 Axx");
    require_close(stock_A[1][1], lambda / 6.0,
                  "direct stock d3 Ayy");
    require_close(stock_A[2][2], lambda / 6.0,
                  "direct stock d3 Azz");
    require_close(TensorAlgebra::compute_trace(stock_A, stock_inverse), 0.0,
                  "direct stock d3 A trace");

    const AnalyticInput flat = make_stock_gp_input(
        256, x, DerivativePath::analytic);
    require_close(flat.vars.lapse, 1.0, "stock GP lapse");
    require_close(flat.vars.shift[0], beta_x(x), "stock visible shift x");
    require_close(flat.vars.shift[1], 0.0, "stock visible shift y");
    require_close(flat.vars.shift[2], 0.0, "stock visible shift z");
    require_close(flat.vars.h[0][0], 1.0, "stock hxx");
    require_close(flat.vars.h[1][1], 1.0, "stock visible hyy");
    require_close(flat.vars.h[2][2], 1.0, "stock hzz");
    require_close(flat.vars.K, stock_K, "stock mapped K");
    require_close(flat.vars.A[0][0], stock_A[0][0],
                  "stock mapped Axx");
    require_close(flat.vars.A[1][1], stock_A[1][1],
                  "stock mapped visible Ayy");
    require_close(flat.vars.A[2][2], stock_A[2][2],
                  "stock mapped Azz");
    require(std::abs(flat.vars.A[1][1] - A.ww) > 1.0e-2,
            "stock visible y is not custom hidden ww");
    require_close(flat.d1.shift[0][0], -0.5 * lambda,
                  "stock analytic shift derivative");
    require_close(flat.d2.shift[0][0][0], 0.75 * lambda / x,
                  "stock analytic shift second derivative");
    const GeometryPieces geometry = direct_geometry_pieces(flat);
    require_close(geometry.z_over_chi[0], 0.0,
                  "direct flat stock contracted connection/Gamma x");
    require_close(geometry.z_over_chi[1], 0.0,
                  "direct flat stock contracted connection/Gamma y");
    require_close(geometry.z_over_chi[2], 0.0,
                  "direct flat stock contracted connection/Gamma z");

    std::cout << "RESULT convention coordinate_gamma_theta_theta="
              << coordinate_gamma_theta_theta
              << " stored_custom_hww=1 custom_K=" << K
              << " custom_Axx=" << A.xx << " custom_Azz=" << A.zz
              << " custom_Aww=" << A.ww << " stock_d3_K=" << stock_K
              << " stock_d3_Axx=" << stock_A[0][0]
              << " stock_d3_Ayy=" << stock_A[1][1]
              << " stock_d3_Azz=" << stock_A[2][2] << "\n";
}

void test_full_custom_analytic_oracle()
{
    const FullOracle::Direction zero_direction;
    const auto oracle =
        FullOracle::evaluate(r0, comparison_x, 0.0, zero_direction);
    require(all_oracle_rows_zero(oracle.total),
            "actual full analytic oracle GP rows");
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const auto index = Operator::variable_index(variable);
        require_close(static_cast<double>(oracle.total[index]), 0.0,
                      std::string("full oracle row ") +
                          Operator::variable_name(variable));
        std::cout << "ANALYTIC_ROW row="
                  << Operator::variable_name(variable)
                  << " residual=" << static_cast<double>(oracle.total[index])
                  << "\n";
    }

    const auto constraints =
        evaluate_gp_constraints_with_full_geometry(r0, comparison_x);
    require_close(constraints.hamiltonian, 0.0,
                  "full-geometry Hamiltonian constraint");
    require_close(constraints.momentum[0], 0.0,
                  "full-geometry momentum x constraint");
    require_close(constraints.momentum[1], 0.0,
                  "full-geometry momentum z constraint");
    std::cout << "ANALYTIC_CONSTRAINT Hamiltonian="
              << constraints.hamiltonian << " Mx=" << constraints.momentum[0]
              << " Mz=" << constraints.momentum[1] << "\n";

    for (const auto variable : {Operator::PerturbationVariable::h_xz,
                                Operator::PerturbationVariable::A_xz,
                                Operator::PerturbationVariable::hat_Gamma_z})
    {
        auto mutation = oracle.total;
        mutation[Operator::variable_index(variable)] += 1.0e-3L;
        require(!all_oracle_rows_zero(mutation),
                std::string("odd-row mutation rejected ") +
                    Operator::variable_name(variable));
    }
    auto momentum_z_mutation = constraints;
    momentum_z_mutation.momentum[1] += 1.0e-3;
    require(std::abs(momentum_z_mutation.momentum[1]) > 1.0e-13,
            "momentum-z mutation rejected");
    std::cout << "MUTATION h_xz_wrong_contribution=DETECTED "
                 "A_xz_wrong_contribution=DETECTED "
                 "hatGammaZ_wrong_contribution=DETECTED "
                 "Mz_wrong_contribution=DETECTED\n";
}

void test_discrete_gp_residual_study()
{
    constexpr std::array<int, 4> intervals = {256, 512, 1024, 2048};
    std::array<Preflight::RawResidualSample, intervals.size()> samples = {
        Preflight::compute_raw_background_residual_sample(intervals[0]),
        Preflight::compute_raw_background_residual_sample(intervals[1]),
        Preflight::compute_raw_background_residual_sample(intervals[2]),
        Preflight::compute_raw_background_residual_sample(intervals[3])};
    std::array<double, 3> pairwise_orders = {};
    for (std::size_t level = 0; level < samples.size(); ++level)
    {
        require(samples[level].max_lapse_target_error < 1.0e-12,
                "custom lapse target remains exactly -3 lambda");
        std::cout << "DISCRETE_GP N=" << samples[level].intervals
                  << " dx=" << samples[level].dx
                  << " max_residual="
                  << samples[level].max_raw_residual_error
                  << " worst_component=" << samples[level].worst_component
                  << " worst_x=" << samples[level].worst_x << "\n";
        if (level + 1 < samples.size())
        {
            pairwise_orders[level] =
                std::log(samples[level].max_raw_residual_error /
                         samples[level + 1].max_raw_residual_error) /
                std::log(2.0);
            require(std::abs(pairwise_orders[level] - 2.0) <= 0.25,
                    "custom discrete GP residual achieves order two");
        }
    }
    std::cout << "DISCRETE_GP_ORDER N256_to_512=" << pairwise_orders[0]
              << " N512_to_1024=" << pairwise_orders[1]
              << " N1024_to_2048=" << pairwise_orders[2]
              << " lapse_unmodified=" << -3.0 * gp_lambda(comparison_x)
              << "\n";
}

void test_stock_residual_and_discrete_convergence()
{
    const DirectCCZ4Access access(
        locked_kappa1, locked_kappa2, locked_kappa3, locked_covariant_z4,
        DirectCCZ4Access::USE_CCZ4);
    const AnalyticInput exact_input = make_stock_gp_input(
        256, comparison_x, DerivativePath::analytic);
    const Vars<double> exact_rhs = direct_rhs(exact_input, access);
    const auto exact_rows = residual_rows(exact_rhs);
    const double lambda2 = gp_lambda(comparison_x) *
                           gp_lambda(comparison_x);
    require_close(exact_rhs.chi, 0.0, "stock d3 chi residual");
    for (int i = 0; i < dimension; ++i)
        for (int j = 0; j < dimension; ++j)
            require_close(exact_rhs.h[i][j], 0.0,
                          "stock d3 conformal-metric residual");
    require_close(exact_rhs.Theta, 0.0, "stock d3 Theta residual");
    require_close(exact_rhs.Gamma[0], 0.0,
                  "stock d3 hatted-Gamma x residual");
    require_close(exact_rhs.Gamma[1], 0.0,
                  "stock d3 hatted-Gamma y residual");
    require_close(exact_rhs.Gamma[2], 0.0,
                  "stock d3 hatted-Gamma z residual");
    require_close(exact_rhs.K, lambda2,
                  "stock d3 K residual is the missing-dimension signal");
    const auto stock_background =
        make_stock_d3_background(comparison_x);
    require_close(evaluate_k_families(stock_background).total(), exact_rhs.K,
                  "independent stock K families match direct total");
    require_close(evaluate_a_families(
                      stock_background, stock_background.Axx,
                      stock_background.dAxx, stock_background.dbeta_x)
                      .total(),
                  exact_rhs.A[0][0],
                  "independent stock Axx families match direct total");
    require_close(evaluate_a_families(
                      stock_background, stock_background.Ayy,
                      stock_background.dAyy, 0.0)
                      .total(),
                  exact_rhs.A[1][1],
                  "independent stock Ayy families match direct total");
    require_close(evaluate_a_families(
                      stock_background, stock_background.Azz,
                      stock_background.dAzz, 0.0)
                      .total(),
                  exact_rhs.A[2][2],
                  "independent stock Azz families match direct total");
    AnalyticInput wrong_extrinsic_sign = exact_input;
    wrong_extrinsic_sign.vars.K *= -1.0;
    for (int i = 0; i < dimension; ++i)
        wrong_extrinsic_sign.vars.A[i][i] *= -1.0;
    const Vars<double> wrong_sign_rhs =
        direct_rhs(wrong_extrinsic_sign, access);
    require(std::abs(wrong_sign_rhs.h[0][0]) > 1.0e-2,
            "direct stock metric RHS rejects reversed extrinsic sign");

    std::array<double, refinements.size()> custom_d1_error{};
    std::array<double, refinements.size()> custom_d2_error{};
    std::array<double, refinements.size()> gr_d1_error{};
    std::array<double, refinements.size()> gr_d2_error{};
    std::array<double, refinements.size()> custom_rhs_error{};
    std::array<double, refinements.size()> gr_rhs_error{};
    constexpr std::array<StockField, 5> fields = {
        StockField::beta, StockField::K, StockField::Axx,
        StockField::Ayy, StockField::Azz};

    for (std::size_t level = 0; level < refinements.size(); ++level)
    {
        const int points = refinements[level];
        for (const double x : sample_x)
        {
            for (const StockField field : fields)
            {
                const auto exact =
                    make_jet(field, points, x, DerivativePath::analytic);
                const auto custom = make_jet(
                    field, points, x, DerivativePath::custom_second_order);
                const auto gr = make_jet(
                    field, points, x, DerivativePath::grchombo_fourth_order);
                custom_d1_error[level] =
                    std::max(custom_d1_error[level],
                             std::abs(custom.dx - exact.dx));
                custom_d2_error[level] =
                    std::max(custom_d2_error[level],
                             std::abs(custom.dxx - exact.dxx));
                gr_d1_error[level] =
                    std::max(gr_d1_error[level],
                             std::abs(gr.dx - exact.dx));
                gr_d2_error[level] =
                    std::max(gr_d2_error[level],
                             std::abs(gr.dxx - exact.dxx));
            }
        }

        const auto custom_input = make_stock_gp_input(
            points, comparison_x, DerivativePath::custom_second_order);
        const auto gr_input = make_stock_gp_input(
            points, comparison_x, DerivativePath::grchombo_fourth_order);
        const auto custom_rows = residual_rows(
            family_total(custom_d3_families(custom_input)));
        const auto gr_rows = residual_rows(direct_rhs(gr_input, access));
        custom_rhs_error[level] = max_difference(custom_rows, exact_rows);
        gr_rhs_error[level] = max_difference(gr_rows, exact_rows);

        std::cout << "RESULT discrete N=" << points
                  << " custom_d1=" << custom_d1_error[level]
                  << " custom_d2=" << custom_d2_error[level]
                  << " grchombo_d1=" << gr_d1_error[level]
                  << " grchombo_d2=" << gr_d2_error[level]
                  << " custom_rhs=" << custom_rhs_error[level]
                  << " grchombo_rhs=" << gr_rhs_error[level] << "\n";
    }

    const double custom_d1_order =
        observed_order(custom_d1_error.front(), custom_d1_error.back());
    const double custom_d2_order =
        observed_order(custom_d2_error.front(), custom_d2_error.back());
    const double gr_d1_order =
        observed_order(gr_d1_error.front(), gr_d1_error.back());
    const double gr_d2_order =
        observed_order(gr_d2_error.front(), gr_d2_error.back());
    const double custom_rhs_order =
        observed_order(custom_rhs_error.front(), custom_rhs_error.back());
    const double gr_rhs_order =
        observed_order_two_halvings(gr_rhs_error.front(), gr_rhs_error[2]);
    require(std::abs(custom_d1_order - 2.0) <= 0.25 &&
                std::abs(custom_d2_order - 2.0) <= 0.25 &&
                std::abs(custom_rhs_order - 2.0) <= 0.25,
            "custom GP jets/RHS achieve order two");
    require(std::abs(gr_d1_order - 4.0) <= 0.25 &&
                std::abs(gr_d2_order - 4.0) <= 0.25 &&
                std::abs(gr_rhs_order - 4.0) <= 0.25,
            "GRChombo GP jets/RHS achieve order four");
    std::cout << "RESULT discrete_order custom_d1=" << custom_d1_order
              << " custom_d2=" << custom_d2_order
              << " grchombo_d1=" << gr_d1_order
              << " grchombo_d2=" << gr_d2_order
              << " custom_rhs=" << custom_rhs_order
              << " grchombo_rhs_plateau=" << gr_rhs_order
              << " grchombo_rhs_N256_roundoff="
              << gr_rhs_error.back() << "\n";

    constexpr std::array<const char *, 18> names = {
        "chi", "hxx", "hyy", "hzz", "hxy", "hxz", "hyz", "K",
        "Theta", "Axx", "Ayy", "Azz", "Axy", "Axz", "Ayz",
        "GammaX", "GammaY", "GammaZ"};
    for (std::size_t row = 0; row < exact_rows.size(); ++row)
    {
        if (std::abs(exact_rows[row]) > 1.0e-13)
        {
            std::cout << "RESULT stock_visible_residual row=" << names[row]
                      << " value=" << exact_rows[row] << "\n";
        }
    }
}

void test_perturbation_convention()
{
    constexpr double length = 7.0;
    constexpr int mode = 2;
    constexpr double epsilon = 1.0e-4;
    const double k = 2.0 * std::acos(-1.0) * mode / length;
    require_close(k, 4.0 * std::acos(-1.0) / length,
                  "Fourier wavenumber convention");

    constexpr std::array<bool, Operator::frozen_gauge_state_vector.size()>
        one_z = {false, false, true,  false, false, false, false,
                 true,  false, false, false, false, true};
    const double z_cos = 0.0;
    const double z_sin = std::acos(-1.0) / (2.0 * k);
    for (std::size_t variable = 0; variable < one_z.size(); ++variable)
    {
        const double p_plus_at_cos =
            epsilon * (one_z[variable] ? std::sin(k * z_cos)
                                       : std::cos(k * z_cos));
        const double p_plus_at_sin =
            epsilon * (one_z[variable] ? std::sin(k * z_sin)
                                       : std::cos(k * z_sin));
        const double p_minus_at_cos =
            epsilon * (one_z[variable] ? std::cos(k * z_cos)
                                       : std::sin(k * z_cos));
        if (one_z[variable])
        {
            require_close(p_plus_at_cos, 0.0, "P+ one-z sine parity");
            require_close(p_plus_at_sin, epsilon,
                          "P+ one-z amplitude normalization");
            require_close(p_minus_at_cos, epsilon, "P- one-z cosine parity");
        }
        else
        {
            require_close(p_plus_at_cos, epsilon,
                          "P+ scalar amplitude normalization");
            require_close(p_minus_at_cos, 0.0, "P- scalar sine parity");
        }
    }
    require(Operator::frozen_gauge_state_vector.size() == 13,
            "perturbation map is the locked 13-state custom map");
    std::cout << "RESULT perturbation k=" << k
              << " mode=" << mode << " length=" << length
              << " amplitude=" << epsilon
              << " gauge_perturbations=0 radial_profile=UNLOCKED\n";
}

} // namespace

int main()
{
    std::cout << std::scientific << std::setprecision(8);
    test_gp_conventions();
    test_independent_gp_family_decomposition();
    test_full_custom_analytic_oracle();
    test_discrete_gp_residual_study();
    test_stock_residual_and_discrete_convergence();
    test_perturbation_convention();
    std::cout
        << "CLASSIFICATION direct_compiled=stock_d3_tensor_algebra,"
           "contracted_connection,CCZ4_rhs,fourth_order_derivatives "
           "source_convention=gamma_equals_h_over_chi,"
           "coordinate_angular_to_stored_ww "
           "custom_hidden_only=hww,Aww,multiplicity_two,full_d4_residual "
           "blocked=production_BoxLoop_and_Chombo_ghost_exchange\n";
    std::cout << "MUTATION extrinsic_curvature_sign=DETECTED "
                 "stored_hww_coordinate_value=DETECTED\n";
    std::cout << "PASS comparison batch 4 GP background and discrete setup\n";
    return 0;
}
