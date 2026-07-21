#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Connection =
    BlackStringToy::Stage4AOFrozenGaugeContractedConnection;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

constexpr double tolerance = 2.0e-13;
constexpr int spatial_dimension = 4;
constexpr double kappa1 = 0.1;
constexpr double kappa3 = 1.0;

struct JetData
{
    double x = 2.0;
    double h_xx = 0.0;
    double h_xz = 0.0;
    double h_zz = 0.0;
    double h_ww = 0.0;
    double dx_h_xx = 0.0;
    double dx_h_xz = 0.0;
    double dx_h_zz = 0.0;
    double dx_h_ww = 0.0;
    double dz_h_xx = 0.0;
    double dz_h_xz = 0.0;
    double dz_h_zz = 0.0;
    double dz_h_ww = 0.0;
    double h_x = 0.0;
    double h_z = 0.0;
};

struct OracleConnection
{
    double g_x;
    double g_z;
    double z_x;
    double z_z;
};

struct OracleCoefficients
{
    double lambda;
    double k0;
    double g;
    double z;
    double shift_x;
};

[[noreturn]] void fail(const std::string &label, const std::string &details)
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
                   const double expected, const double scale = 1.0)
{
    const double allowed = tolerance * std::max(1.0, std::abs(scale));
    if (std::abs(actual - expected) > allowed)
    {
        fail(label, "got " + std::to_string(actual) + ", expected " +
                        std::to_string(expected));
    }
    std::cout << "PASS " << label << "\n";
}

void require_separated(const std::string &label, const double actual,
                       const double mutation)
{
    if (std::abs(actual - mutation) < 1.0e-5)
    {
        fail(label, "mutation was not separated from the implementation");
    }
    std::cout << "PASS " << label << "\n";
}

Connection::PerturbationJet make_jet(const JetData &data)
{
    return Connection::make_perturbation_jet(
        data.x, data.h_xx, data.h_xz, data.h_zz, data.h_ww, data.dx_h_xx,
        data.dx_h_xz, data.dx_h_zz, data.dx_h_ww, data.dz_h_xx,
        data.dz_h_xz, data.dz_h_zz, data.dz_h_ww, data.h_x, data.h_z);
}

OracleConnection independent_connection(const JetData &data,
                                        const double hidden_multiplicity = 2.0)
{
    const double g_x =
        0.5 * data.dx_h_xx - 0.5 * data.dx_h_zz - data.dx_h_ww +
        data.dz_h_xz +
        hidden_multiplicity * (data.h_xx - data.h_ww) / data.x;
    const double g_z =
        data.dx_h_xz + hidden_multiplicity * data.h_xz / data.x -
        0.5 * data.dz_h_xx + 0.5 * data.dz_h_zz - data.dz_h_ww;
    return {g_x, g_z, 0.5 * (data.h_x - g_x),
            0.5 * (data.h_z - g_z)};
}

OracleCoefficients independent_coefficients(const double r0, const double x)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double k0 = 3.0 * lambda / 2.0;

    // Independently linearized selected USE_CCZ4 coefficients. Before the
    // kappa3=1 simplification, the algebraic Z coefficient is
    // (4 K0/d)(kappa3-1)-2 kappa1. The x shift-gradient reconstructs H_x
    // because g_x+2 kappa3 Z_x=H_x at kappa3=1.
    const double g_coefficient =
        2.0 * k0 / static_cast<double>(spatial_dimension);
    const double z_coefficient =
        4.0 * k0 / static_cast<double>(spatial_dimension) * (kappa3 - 1.0) -
        2.0 * kappa1;
    const double shift_x_coefficient = lambda / 2.0;
    return {lambda, k0, g_coefficient, z_coefficient, shift_x_coefficient};
}

Vector apply(const double r0, const JetData &data)
{
    return Operator::
        apply_hat_gamma_z4_kappa_shift_gradient_insertion_at_point(
            r0, make_jet(data));
}

void require_only_gamma_outputs(const std::string &label,
                                const Vector &output)
{
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable == Variable::hat_Gamma_x ||
            variable == Variable::hat_Gamma_z)
        {
            continue;
        }
        require_close(label + " leaves " + Operator::variable_name(variable) +
                          " untouched",
                      output.value(variable), 0.0, 0.0);
    }
}

void check_independent_coefficients_and_pure_h()
{
    const double r0 = 1.3;
    JetData data;
    data.x = 2.4;
    data.h_x = 1.1;
    data.h_z = -0.7;
    const auto coefficients = independent_coefficients(r0, data.x);
    const auto connection = independent_connection(data);
    const auto output = apply(r0, data);

    require_close("K0 is independently 3 lambda/2", coefficients.k0,
                  1.5 * coefficients.lambda, coefficients.lambda);
    require_close("g coefficient derives as 2 K0/d",
                  coefficients.g, 0.75 * coefficients.lambda,
                  coefficients.lambda);
    require_close("locked kappa3 cancellation leaves -2 kappa1 Z",
                  coefficients.z, -0.2);
    require_close("x shift-gradient coefficient is lambda/2",
                  coefficients.shift_x, 0.5 * coefficients.lambda,
                  coefficients.lambda);
    require_close("pure H_x reconstructs Z_x", connection.z_x,
                  0.5 * data.h_x);
    require_close("pure H_z reconstructs Z_z", connection.z_z,
                  0.5 * data.h_z);

    const double expected_x = coefficients.g * connection.g_x +
                              coefficients.z * connection.z_x +
                              coefficients.shift_x * data.h_x;
    const double expected_z = coefficients.g * connection.g_z +
                              coefficients.z * connection.z_z;
    require_close("pure H_x Gamma output", output.value(Variable::hat_Gamma_x),
                  expected_x, expected_x);
    require_close("pure H_z Gamma output", output.value(Variable::hat_Gamma_z),
                  expected_z, expected_z);
    require_separated("no extra +lambda H_z/2",
                      output.value(Variable::hat_Gamma_z),
                      expected_z + coefficients.shift_x * data.h_z);
    require_only_gamma_outputs("pure H block", output);
}

void check_metric_path_z_reconstruction_and_mutations()
{
    const double r0 = 0.9;
    JetData data;
    data.x = 1.8;
    data.h_xx = 0.31;
    data.h_xz = -0.22;
    data.h_zz = -0.17;
    data.h_ww = 0.28;
    data.dx_h_xx = -0.41;
    data.dx_h_xz = 0.19;
    data.dx_h_zz = 0.37;
    data.dx_h_ww = -0.13;
    data.dz_h_xx = 0.23;
    data.dz_h_xz = -0.29;
    data.dz_h_zz = -0.11;
    data.dz_h_ww = 0.07;

    const auto expected_connection = independent_connection(data);
    const auto helper_connection =
        Connection::compute_contracted_connection_and_z(make_jet(data));
    const auto coefficients = independent_coefficients(r0, data.x);
    const auto output = apply(r0, data);

    require_close("metric oracle g_x", helper_connection.g_x(),
                  expected_connection.g_x, expected_connection.g_x);
    require_close("metric oracle g_z", helper_connection.g_z(),
                  expected_connection.g_z, expected_connection.g_z);
    require_close("H=0 reconstructs Z_x=-g_x/2", helper_connection.z_x(),
                  -0.5 * expected_connection.g_x,
                  expected_connection.g_x);
    require_close("H=0 reconstructs Z_z=-g_z/2", helper_connection.z_z(),
                  -0.5 * expected_connection.g_z,
                  expected_connection.g_z);

    const double expected_x = coefficients.g * expected_connection.g_x +
                              coefficients.z * expected_connection.z_x;
    const double expected_z = coefficients.g * expected_connection.g_z +
                              coefficients.z * expected_connection.z_z;
    require_close("pure metric feeds Gamma x only through g_x/Z_x",
                  output.value(Variable::hat_Gamma_x), expected_x, expected_x);
    require_close("pure metric feeds Gamma z only through g_z/Z_z",
                  output.value(Variable::hat_Gamma_z), expected_z, expected_z);

    const double wrong_sign_x = coefficients.g * expected_connection.g_x -
                                coefficients.z * expected_connection.z_x;
    const double wrong_sign_z = coefficients.g * expected_connection.g_z -
                                coefficients.z * expected_connection.z_z;
    require_separated("wrong positive Z damping sign fails for x",
                      output.value(Variable::hat_Gamma_x), wrong_sign_x);
    require_separated("wrong positive Z damping sign fails for z",
                      output.value(Variable::hat_Gamma_z), wrong_sign_z);
}

void check_hidden_multiplicity_and_no_advection()
{
    const double r0 = 1.1;
    JetData data;
    data.x = 2.2;
    data.h_ww = 0.8;
    data.h_xz = -0.6;
    data.h_x = 0.35;
    data.h_z = -0.25;
    const auto coefficients = independent_coefficients(r0, data.x);
    const auto correct = independent_connection(data, 2.0);
    const auto wrong_hidden = independent_connection(data, 1.0);
    const auto output = apply(r0, data);

    const double expected_x = coefficients.g * correct.g_x +
                              coefficients.z * correct.z_x +
                              coefficients.shift_x * data.h_x;
    const double expected_z =
        coefficients.g * correct.g_z + coefficients.z * correct.z_z;
    const double wrong_hidden_x = coefficients.g * wrong_hidden.g_x +
                                  coefficients.z * wrong_hidden.z_x +
                                  coefficients.shift_x * data.h_x;
    const double wrong_hidden_z = coefficients.g * wrong_hidden.g_z +
                                  coefficients.z * wrong_hidden.z_z;
    require_close("hidden multiplicity two x result",
                  output.value(Variable::hat_Gamma_x), expected_x, expected_x);
    require_close("hidden multiplicity two z result",
                  output.value(Variable::hat_Gamma_z), expected_z, expected_z);
    require_separated("wrong hidden multiplicity fails for x",
                      output.value(Variable::hat_Gamma_x), wrong_hidden_x);
    require_separated("wrong hidden multiplicity fails for z",
                      output.value(Variable::hat_Gamma_z), wrong_hidden_z);

    const double beta_gp = std::sqrt(r0 / data.x);
    const double hypothetical_dx_h_x = 0.81;
    const double hypothetical_dx_h_z = -0.63;
    require_separated("common advection is not duplicated in Gamma x block",
                      output.value(Variable::hat_Gamma_x),
                      expected_x + beta_gp * hypothetical_dx_h_x);
    require_separated("common advection is not duplicated in Gamma z block",
                      output.value(Variable::hat_Gamma_z),
                      expected_z + beta_gp * hypothetical_dx_h_z);
}

JetData parity_jet(const double z)
{
    const double k = 1.7;
    const double cosine = std::cos(k * z);
    const double sine = std::sin(k * z);
    JetData data;
    data.x = 2.1;
    data.h_xx = 0.27 * cosine;
    data.h_xz = -0.19 * sine;
    data.h_zz = -0.16 * cosine;
    data.h_ww = 0.11 * cosine;
    data.dx_h_xx = -0.07 * cosine;
    data.dx_h_xz = 0.13 * sine;
    data.dx_h_zz = 0.09 * cosine;
    data.dx_h_ww = -0.05 * cosine;
    data.dz_h_xx = -0.27 * k * sine;
    data.dz_h_xz = -0.19 * k * cosine;
    data.dz_h_zz = 0.16 * k * sine;
    data.dz_h_ww = -0.11 * k * sine;
    data.h_x = 0.23 * cosine;
    data.h_z = -0.21 * sine;
    return data;
}

void check_parity_and_completion_gates()
{
    const double r0 = 1.0;
    const auto positive = apply(r0, parity_jet(0.43));
    const auto negative = apply(r0, parity_jet(-0.43));
    require_close("hat_Gamma^x output stays scalar/even",
                  positive.value(Variable::hat_Gamma_x),
                  negative.value(Variable::hat_Gamma_x),
                  positive.value(Variable::hat_Gamma_x));
    require_close("hat_Gamma^z output stays one-z/odd",
                  positive.value(Variable::hat_Gamma_z),
                  -negative.value(Variable::hat_Gamma_z),
                  positive.value(Variable::hat_Gamma_z));

    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("first hatted-Gamma partial block is implemented",
                 Operator::
                     hat_gamma_z4_kappa_shift_gradient_block_implemented);
    require_true("complete hatted-Gamma RHS remains false",
                 !Operator::hat_gamma_rhs_block_implemented);
    require_true("hat_Gamma^x RHS remains incomplete",
                 !Operator::variable_rhs_complete(Variable::hat_Gamma_x));
    require_true("hat_Gamma^z RHS remains incomplete",
                 !Operator::variable_rhs_complete(Variable::hat_Gamma_z));
    require_true("complete frozen-gauge operator remains false",
                 !contract.complete_operator_implemented());
    require_true("eigensolver remains disallowed",
                 !contract.eigensolver_allowed());
}

} // namespace

int main()
{
    check_independent_coefficients_and_pure_h();
    check_metric_path_z_reconstruction_and_mutations();
    check_hidden_multiplicity_and_no_advection();
    check_parity_and_completion_gates();
    std::cout << "PASS Stage 4AO-C hatted-Gamma Z/kappa/shift-gradient block\n";
    return 0;
}
