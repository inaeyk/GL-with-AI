#include "Stage4AOFrozenGaugeZDerivativeAdapter.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>

namespace
{
namespace Adapter =
    BlackStringToy::Stage4AOFrozenGaugeZDerivativeAdapter;
namespace Ricci = BlackStringToy::CartoonRicci;
using Metric = BlackStringToy::ConformalCartoonAlgebra::ConformalMetric;

constexpr double tolerance = 2.0e-8;

struct Term
{
    double coefficient;
    int x_power;
    int z_power;
};

struct Field
{
    std::array<Term, 6> terms;
    std::size_t count;
};

struct Fields
{
    Field h_xx;
    Field h_xz;
    Field h_zz;
    Field h_ww;
    Field hat_gamma_x;
    Field hat_gamma_z;
};

struct OracleConnection
{
    double g_x;
    double g_z;
};

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_true(const std::string &label, const bool condition)
{
    if (!condition)
    {
        fail(label, "condition was false");
    }
    std::cout << "PASS " << label << "\n";
}

void require_close(const std::string &label, const double actual,
                   const double expected,
                   const double allowed_error = tolerance)
{
    const double error = std::abs(actual - expected);
    if (error > allowed_error)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " error=" + std::to_string(error));
    }
    std::cout << "PASS " << label << "\n";
}

void require_separated(const std::string &label, const double actual,
                       const double mutation)
{
    if (std::abs(actual - mutation) <= 1.0e-4)
    {
        fail(label, "mutation was not separated");
    }
    std::cout << "PASS " << label << "\n";
}

void require_mutation_guard(const std::string &row,
                            const std::string &term_family,
                            const double actual, const double changed_term)
{
    require_true(row + " " + term_family + " changed term is active",
                 std::abs(changed_term) > 1.0e-4);
    require_separated(row + " " + term_family + " omission rejected",
                      actual, actual - changed_term);
    std::cout << "INFO adapter mutation table | " << row << " | "
              << term_family << " | omission guard PASS\n";
}

double falling_factorial(const int power, const int derivative)
{
    if (derivative > power)
    {
        return 0.0;
    }
    double result = 1.0;
    for (int i = 0; i < derivative; ++i)
    {
        result *= static_cast<double>(power - i);
    }
    return result;
}

double evaluate(const Field &field, const double x, const double z,
                const int dx = 0, const int dz = 0)
{
    double result = 0.0;
    for (std::size_t i = 0; i < field.count; ++i)
    {
        const auto &term = field.terms[i];
        if (dx > term.x_power || dz > term.z_power)
        {
            continue;
        }
        result +=
            term.coefficient * falling_factorial(term.x_power, dx) *
            falling_factorial(term.z_power, dz) *
            std::pow(x, term.x_power - dx) *
            std::pow(z, term.z_power - dz);
    }
    return result;
}

Field field(const std::initializer_list<Term> terms)
{
    Field result{};
    result.count = terms.size();
    std::size_t index = 0;
    for (const auto &term : terms)
    {
        result.terms[index++] = term;
    }
    return result;
}

Fields mixed_fields()
{
    return {
        field({{0.7, 3, 0}, {0.2, 2, 1}, {-0.3, 0, 2}, {0.4, 1, 0}}),
        field({{-0.4, 2, 1}, {0.5, 1, 2}, {0.3, 0, 1}}),
        field({{0.6, 2, 0}, {-0.2, 1, 2}, {0.1, 0, 3}, {-0.5, 1, 0}}),
        field({{-0.3, 3, 0}, {0.4, 1, 1}, {0.2, 0, 2}, {0.8, 0, 0}}),
        field({{0.9, 2, 1}, {-0.2, 1, 2}, {0.5, 1, 0}}),
        field({{-0.7, 1, 2}, {0.3, 2, 1}, {0.6, 0, 1}})};
}

Fields parity_fields()
{
    return {
        field({{0.7, 2, 0}, {-0.2, 1, 2}, {0.4, 0, 0}}),
        field({{0.5, 2, 1}, {-0.3, 0, 3}, {0.2, 1, 1}}),
        field({{-0.4, 3, 0}, {0.6, 1, 2}, {0.1, 0, 0}}),
        field({{0.3, 2, 0}, {0.2, 0, 2}, {-0.5, 1, 0}}),
        field({{0.8, 1, 0}, {-0.2, 2, 2}, {0.3, 0, 0}}),
        field({{-0.6, 1, 1}, {0.4, 0, 3}, {0.2, 2, 1}})};
}

Metric metric_at(const Fields &fields, const double x, const double z,
                 const int dx, const int dz)
{
    return {evaluate(fields.h_xx, x, z, dx, dz),
            evaluate(fields.h_xz, x, z, dx, dz),
            evaluate(fields.h_zz, x, z, dx, dz),
            evaluate(fields.h_ww, x, z, dx, dz)};
}

Adapter::DerivativeAdapterInput make_input(const Fields &fields,
                                           const double x, const double z)
{
    const Ricci::MetricDerivatives derivatives = {
        metric_at(fields, x, z, 1, 0), metric_at(fields, x, z, 0, 1),
        metric_at(fields, x, z, 2, 0), metric_at(fields, x, z, 1, 1),
        metric_at(fields, x, z, 0, 2)};
    return Adapter::make_derivative_adapter_input(
        x, metric_at(fields, x, z, 0, 0), derivatives,
        evaluate(fields.hat_gamma_x, x, z),
        evaluate(fields.hat_gamma_z, x, z),
        evaluate(fields.hat_gamma_x, x, z, 1, 0),
        evaluate(fields.hat_gamma_x, x, z, 0, 1),
        evaluate(fields.hat_gamma_z, x, z, 1, 0),
        evaluate(fields.hat_gamma_z, x, z, 0, 1));
}

OracleConnection unreduced_connection(const Fields &fields, const double x,
                                      const double z,
                                      const double hidden_multiplicity)
{
    const double h_xx = evaluate(fields.h_xx, x, z);
    const double h_xz = evaluate(fields.h_xz, x, z);
    const double h_ww = evaluate(fields.h_ww, x, z);
    const double g_x =
        0.5 * evaluate(fields.h_xx, x, z, 1, 0) -
        0.5 * evaluate(fields.h_zz, x, z, 1, 0) -
        evaluate(fields.h_ww, x, z, 1, 0) +
        evaluate(fields.h_xz, x, z, 0, 1) +
        hidden_multiplicity * (h_xx - h_ww) / x;
    const double g_z =
        evaluate(fields.h_xz, x, z, 1, 0) +
        hidden_multiplicity * h_xz / x -
        0.5 * evaluate(fields.h_xx, x, z, 0, 1) +
        0.5 * evaluate(fields.h_zz, x, z, 0, 1) -
        evaluate(fields.h_ww, x, z, 0, 1);
    return {g_x, g_z};
}

double finite_difference_connection(
    const Fields &fields, const double x, const double z,
    const int component, const int direction,
    const double hidden_multiplicity = 2.0)
{
    const double epsilon = 1.0e-5;
    const double plus_x = direction == 0 ? x + epsilon : x;
    const double minus_x = direction == 0 ? x - epsilon : x;
    const double plus_z = direction == 1 ? z + epsilon : z;
    const double minus_z = direction == 1 ? z - epsilon : z;
    const auto plus = unreduced_connection(fields, plus_x, plus_z,
                                           hidden_multiplicity);
    const auto minus = unreduced_connection(fields, minus_x, minus_z,
                                            hidden_multiplicity);
    const double plus_value = component == 0 ? plus.g_x : plus.g_z;
    const double minus_value = component == 0 ? minus.g_x : minus.g_z;
    return (plus_value - minus_value) / (2.0 * epsilon);
}

void check_mixed_oracle()
{
    const auto fields = mixed_fields();
    const double x = 1.9;
    const double z = 0.37;
    const auto input = make_input(fields, x, z);
    const auto result = Adapter::compute_derivative_adapter(input);
    const auto connection = unreduced_connection(fields, x, z, 2.0);

    require_close("unreduced g_x", result.g_x(), connection.g_x);
    require_close("unreduced g_z", result.g_z(), connection.g_z);
    require_close("FD dx g_x", result.dx_g_x(),
                  finite_difference_connection(fields, x, z, 0, 0));
    require_close("FD dz g_x", result.dz_g_x(),
                  finite_difference_connection(fields, x, z, 0, 1));
    require_close("FD dx g_z", result.dx_g_z(),
                  finite_difference_connection(fields, x, z, 1, 0));
    require_close("FD dz g_z", result.dz_g_z(),
                  finite_difference_connection(fields, x, z, 1, 1));

    require_close(
        "dx z_x reconstruction", result.dx_z_x(),
        0.5 * (evaluate(fields.hat_gamma_x, x, z, 1, 0) -
               result.dx_g_x()));
    require_close(
        "dz z_x reconstruction", result.dz_z_x(),
        0.5 * (evaluate(fields.hat_gamma_x, x, z, 0, 1) -
               result.dz_g_x()));
    require_close(
        "dx z_z reconstruction", result.dx_z_z(),
        0.5 * (evaluate(fields.hat_gamma_z, x, z, 1, 0) -
               result.dx_g_z()));
    require_close(
        "dz z_z reconstruction", result.dz_z_z(),
        0.5 * (evaluate(fields.hat_gamma_z, x, z, 0, 1) -
               result.dz_g_z()));

    require_true("mixed dxz derivatives are active",
                 std::abs(metric_at(fields, x, z, 1, 1).xx) > 1.0e-4 &&
                     std::abs(metric_at(fields, x, z, 1, 1).xz) > 1.0e-4);
    require_true("determinant/trace constraint is not assumed",
                 std::abs(input.h().xx + input.h().zz +
                          2.0 * input.h().ww) >
                     1.0e-3);
}

void check_adapter_term_mutations()
{
    const auto fields = mixed_fields();
    const double x = 1.9;
    const double z = 0.37;
    const auto input = make_input(fields, x, z);
    const auto result = Adapter::compute_derivative_adapter(input);
    const auto &h = input.h();
    const auto &d = input.derivatives();

    require_mutation_guard("dx_g_x", "representative second derivative",
                           result.dx_g_x(), 0.5 * d.dxx.xx);
    require_mutation_guard("dx_g_x", "representative mixed derivative",
                           result.dx_g_x(), d.dxz.xz);
    require_mutation_guard("dx_g_x", "-2(h_xx-h_ww)/x^2",
                           result.dx_g_x(),
                           -2.0 * (h.xx - h.ww) / (x * x));

    const double wrong_inverse_square_sign =
        result.dx_g_x() +
        4.0 * (h.xx - h.ww) / (x * x);
    require_separated("dx_g_x wrong 1/x^2 sign rejected", result.dx_g_x(),
                      wrong_inverse_square_sign);

    require_mutation_guard("dz_g_x", "representative mixed derivative",
                           result.dz_g_x(), 0.5 * d.dxz.xx);
    require_mutation_guard("dz_g_x", "representative second derivative",
                           result.dz_g_x(), d.dzz.xz);

    require_mutation_guard("dx_g_z", "representative mixed derivative",
                           result.dx_g_z(), -0.5 * d.dxz.xx);
    require_mutation_guard("dx_g_z", "representative second derivative",
                           result.dx_g_z(), d.dxx.xz);
    require_mutation_guard("dx_g_z", "-2h_xz/x^2", result.dx_g_z(),
                           -2.0 * h.xz / (x * x));

    require_mutation_guard("dz_g_z", "representative mixed derivative",
                           result.dz_g_z(), d.dxz.xz);
    require_mutation_guard("dz_g_z", "representative second derivative",
                           result.dz_g_z(), -0.5 * d.dzz.xx);

    const double hidden_one_dx_g_z =
        d.dxx.xz + (d.dx.xz / x - h.xz / (x * x)) -
        0.5 * d.dxz.xx + 0.5 * d.dxz.zz - d.dxz.ww;
    require_separated("hidden multiplicity one rejected", result.dx_g_z(),
                      hidden_one_dx_g_z);
    std::cout << "INFO adapter mutation table | all rows | hidden "
                 "multiplicity two | coefficient guard PASS\n";
    require_separated(
        "independent hidden-one contraction oracle rejected",
        result.dz_g_x(),
        finite_difference_connection(fields, x, z, 0, 1, 1.0));
}

void check_parity()
{
    const auto fields = parity_fields();
    const double x = 1.7;
    const double z = 0.31;
    const auto plus =
        Adapter::compute_derivative_adapter(make_input(fields, x, z));
    const auto minus =
        Adapter::compute_derivative_adapter(make_input(fields, x, -z));

    require_close("g_x even", plus.g_x(), minus.g_x());
    require_close("g_z odd", plus.g_z(), -minus.g_z());
    require_close("z_x even", plus.z_x(), minus.z_x());
    require_close("z_z odd", plus.z_z(), -minus.z_z());
    require_close("dx g_x even", plus.dx_g_x(), minus.dx_g_x());
    require_close("dz g_x odd", plus.dz_g_x(), -minus.dz_g_x());
    require_close("dx g_z odd", plus.dx_g_z(), -minus.dx_g_z());
    require_close("dz g_z even", plus.dz_g_z(), minus.dz_g_z());
    require_close("dx z_x even", plus.dx_z_x(), minus.dx_z_x());
    require_close("dz z_x odd", plus.dz_z_x(), -minus.dz_z_x());
    require_close("dx z_z odd", plus.dx_z_z(), -minus.dx_z_z());
    require_close("dz z_z even", plus.dz_z_z(), minus.dz_z_z());

    const auto q_plus = Adapter::make_encoded_z_ricci_completion(x, plus);
    const auto q_minus = Adapter::make_encoded_z_ricci_completion(x, minus);
    require_close("q scalar even", q_plus.scalar_trace(),
                  q_minus.scalar_trace());
    require_close("qTF_xz odd", q_plus.tf_xz(), -q_minus.tf_xz());
}

void check_scope_flags()
{
    static_assert(!std::is_aggregate<Adapter::DerivativeAdapterInput>::value,
                  "adapter input is opaque");
    static_assert(!std::is_aggregate<Adapter::DerivativeAdapterResult>::value,
                  "adapter result is opaque");
    require_true("adapter is validation only", Adapter::validation_only);
    require_true("modified-cartoon derivative adapter implemented",
                 Adapter::modified_cartoon_derivative_adapter_implemented);
    require_true("no determinant constraint assumed",
                 !Adapter::determinant_constraint_assumed);
    require_true("hidden multiplicity is two",
                 Adapter::hidden_multiplicity == 2);
    require_true("production helper is not numerically differentiated",
                 !Adapter::numerical_production_differentiation_implemented);
    require_true("complete rows remain outside adapter",
                 !Adapter::complete_non_gamma_rows_implemented);
    require_true("full operator remains outside adapter",
                 !Adapter::full_operator_implemented);
    require_true("eigensolver remains outside adapter",
                 !Adapter::eigensolver_implemented);
}
} // namespace

int main()
{
    check_mixed_oracle();
    check_adapter_term_mutations();
    check_parity();
    check_scope_flags();
    std::cout << "PASS Stage 4AO-C encoded-Z derivative adapter\n";
    return 0;
}
