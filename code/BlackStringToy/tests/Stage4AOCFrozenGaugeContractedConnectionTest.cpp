#include "CartoonHatGammaX.hpp"
#include "Stage4AOFrozenGaugeContractedConnection.hpp"
#include "Stage4AOFrozenGaugeOperator.hpp"

#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace
{
namespace Connection =
    BlackStringToy::Stage4AOFrozenGaugeContractedConnection;
namespace HatX = BlackStringToy::CartoonHatGammaX;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Input = Connection::PerturbationJet;
using Result = Connection::ContractedConnectionAndZ;

constexpr double tolerance = 1.0e-12;
constexpr double plateau_tolerance = 2.0e-7;
constexpr double pi = 3.141592653589793238462643383279502884;

struct Direction
{
    double h_xx;
    double h_xz;
    double h_zz;
    double h_ww;
    double dx_h_xx;
    double dx_h_xz;
    double dx_h_zz;
    double dx_h_ww;
    double dz_h_xx;
    double dz_h_xz;
    double dz_h_zz;
    double dz_h_ww;
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
                       const double wrong)
{
    if (std::abs(actual - wrong) <= 1.0e-3)
    {
        fail(label, "mutation was not separated from the actual result");
    }
    std::cout << "PASS " << label << "\n";
}

template <class Function> void require_domain_error(const std::string &label,
                                                    Function &&function)
{
    try
    {
        function();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    catch (const std::exception &exception)
    {
        fail(label, std::string("wrong exception: ") + exception.what());
    }
    fail(label, "expected std::domain_error");
}

Input make_input(const double x, const Direction &direction,
                 const double hat_gamma_x = 0.0,
                 const double hat_gamma_z = 0.0)
{
    return Connection::make_perturbation_jet(
        x, direction.h_xx, direction.h_xz, direction.h_zz, direction.h_ww,
        direction.dx_h_xx, direction.dx_h_xz, direction.dx_h_zz,
        direction.dx_h_ww, direction.dz_h_xx, direction.dz_h_xz,
        direction.dz_h_zz, direction.dz_h_ww, hat_gamma_x, hat_gamma_z);
}

Result compute(const double x, const Direction &direction,
               const double hat_gamma_x = 0.0,
               const double hat_gamma_z = 0.0)
{
    return Connection::compute_contracted_connection_and_z(
        make_input(x, direction, hat_gamma_x, hat_gamma_z));
}

double nonlinear_tilde_gamma_x(const double x, const Direction &direction,
                               const double epsilon)
{
    const auto inputs = HatX::make_hat_gamma_x_inputs(
        x, 1.0 + epsilon * direction.h_xx,
        epsilon * direction.h_xz, 1.0 + epsilon * direction.h_zz,
        1.0 + epsilon * direction.h_ww,
        epsilon * direction.dx_h_xx, epsilon * direction.dz_h_xx,
        epsilon * direction.dx_h_xz, epsilon * direction.dz_h_xz,
        epsilon * direction.dx_h_zz, epsilon * direction.dz_h_zz,
        epsilon * direction.dx_h_ww, epsilon * direction.dz_h_ww, 0.0);
    return HatX::compute_hat_gamma_x(inputs).tilde_gamma_x();
}

double nonlinear_tilde_gamma_z(const double x, const Direction &direction,
                               const double epsilon)
{
    // Independent test-only full contraction. It evaluates the visible 2x2
    // Christoffels generically, then adds two hidden Gamma^z_ww copies.
    const double h[2][2] = {
        {1.0 + epsilon * direction.h_xx,
         epsilon * direction.h_xz},
        {epsilon * direction.h_xz,
         1.0 + epsilon * direction.h_zz}};
    const double determinant = h[0][0] * h[1][1] - h[0][1] * h[1][0];
    const double inverse[2][2] = {{h[1][1] / determinant,
                                   -h[0][1] / determinant},
                                  {-h[1][0] / determinant,
                                   h[0][0] / determinant}};
    const double derivatives[2][2][2] = {
        {{epsilon * direction.dx_h_xx, epsilon * direction.dx_h_xz},
         {epsilon * direction.dx_h_xz, epsilon * direction.dx_h_zz}},
        {{epsilon * direction.dz_h_xx, epsilon * direction.dz_h_xz},
         {epsilon * direction.dz_h_xz, epsilon * direction.dz_h_zz}}};

    double base_contraction = 0.0;
    for (int j = 0; j < 2; ++j)
    {
        for (int k = 0; k < 2; ++k)
        {
            double gamma_z_jk = 0.0;
            for (int ell = 0; ell < 2; ++ell)
            {
                gamma_z_jk +=
                    0.5 * inverse[1][ell] *
                    (derivatives[j][ell][k] + derivatives[k][ell][j] -
                     derivatives[ell][j][k]);
            }
            base_contraction += inverse[j][k] * gamma_z_jk;
        }
    }

    const double h_ww = 1.0 + epsilon * direction.h_ww;
    const double radial_hidden =
        ((h[0][0] - h_ww) / x -
         0.5 * epsilon * direction.dx_h_ww);
    const double z_hidden =
        h[0][1] / x - 0.5 * epsilon * direction.dz_h_ww;
    const double gamma_z_ww = inverse[1][0] * radial_hidden +
                              inverse[1][1] * z_hidden;
    return base_contraction + 2.0 * gamma_z_ww / h_ww;
}

void check_type_and_scope_contract()
{
    using Function = decltype(
        &Connection::compute_contracted_connection_and_z);
    static_assert(!std::is_aggregate<Input>::value,
                  "contracted-connection input must not be open");
    static_assert(!std::is_aggregate<Result>::value,
                  "contracted-connection result must not be open");
    static_assert(std::is_invocable<Function, const Input &>::value,
                  "helper accepts only its checked perturbation jet");
    static_assert(Connection::hidden_multiplicity == 2,
                  "Stage 4AO-C needs two hidden directions");

    require_true("helper is validation only", Connection::validation_only);
    require_true("contracted connection and Z reconstruction implemented",
                 Connection::
                     contracted_connection_and_z_reconstruction_implemented);
    require_true("helper writes no Gamma RHS output",
                 !Connection::gamma_rhs_output_implemented);
    require_true("Gamma damping remains absent",
                 !Connection::gamma_damping_implemented);
    require_true("connection-A remains absent",
                 !Connection::connection_a_implemented);
    require_true("vector Hessian remains absent",
                 !Connection::vector_hessian_implemented);
    require_true("K/Theta/chi gradients remain absent",
                 !Connection::k_theta_chi_gradients_implemented);
    require_true("complete Gamma evolution remains absent",
                 !Connection::complete_gamma_evolution_implemented);
    require_true("helper has no eigensolver",
                 !Connection::eigensolver_implemented);
    require_true("helper has no production wiring",
                 !Connection::production_wiring_implemented);
}

void check_zero_and_z_reconstruction()
{
    const Direction zero = {};
    const auto zero_result = compute(2.0, zero);
    require_close("zero perturbation g_x", zero_result.g_x(), 0.0);
    require_close("zero perturbation g_z", zero_result.g_z(), 0.0);
    require_close("zero perturbation Z_x with H_x=0", zero_result.z_x(), 0.0);
    require_close("zero perturbation Z_z with H_z=0", zero_result.z_z(), 0.0);

    const Direction metric{0.4, -0.3, 0.2, -0.1, 0.7, -0.5, -0.2, 0.6,
                           -0.4, 0.8, 0.3, -0.9};
    const auto result = compute(2.5, metric, 1.7, -0.6);
    require_close("nonzero H_x reconstructs Z_x", result.z_x(),
                  0.5 * (1.7 - result.g_x()));
    require_close("nonzero H_z reconstructs Z_z", result.z_z(),
                  0.5 * (-0.6 - result.g_z()));
}

void check_pure_component_oracles()
{
    const Direction pure_hww{0.0, 0.0, 0.0, 1.5, 0.0, 0.0, 0.0, 0.7,
                              0.0, 0.0, 0.0, -0.4};
    const auto hww = compute(2.0, pure_hww);
    require_close("pure h_ww g_x", hww.g_x(), -2.2);
    require_close("pure h_ww g_z", hww.g_z(), 0.4);

    const Direction pure_hxz{0.0, 0.8, 0.0, 0.0, 0.0, -0.3, 0.0, 0.0,
                              0.0, 1.1, 0.0, 0.0};
    const auto hxz = compute(2.0, pure_hxz);
    require_close("pure h_xz g_x", hxz.g_x(), 1.1);
    require_close("pure h_xz g_z", hxz.g_z(), 0.5);

    const Direction diagonal{0.6, 0.0, -0.4, 0.2, 0.9, 0.0, -0.5, 0.3,
                              -0.7, 0.0, 0.8, -0.2};
    const auto diagonal_result = compute(2.0, diagonal);
    const double expected_x =
        0.5 * 0.9 - 0.5 * -0.5 - 0.3 + 2.0 * (0.6 - 0.2) / 2.0;
    const double expected_z = -0.5 * -0.7 + 0.5 * 0.8 - -0.2;
    require_close("diagonal metric g_x", diagonal_result.g_x(), expected_x);
    require_close("diagonal metric g_z", diagonal_result.g_z(), expected_z);
}

void check_nonlinear_finite_difference_oracles()
{
    const double x = 2.3;
    const Direction direction{0.35, -0.27, -0.18, 0.11, 0.62, -0.44,
                              -0.31, 0.29, -0.26, 0.53, 0.47, -0.38};
    const auto expected = compute(x, direction);
    constexpr std::array<double, 5> epsilons = {
        1.0e-2, 1.0e-4, 1.0e-5, 1.0e-6, 1.0e-7};

    double middle_x = 0.0;
    double middle_z = 0.0;
    double next_x = 0.0;
    double next_z = 0.0;
    for (const double epsilon : epsilons)
    {
        const double finite_difference_x =
            (nonlinear_tilde_gamma_x(x, direction, epsilon) -
             nonlinear_tilde_gamma_x(x, direction, -epsilon)) /
            (2.0 * epsilon);
        const double finite_difference_z =
            (nonlinear_tilde_gamma_z(x, direction, epsilon) -
             nonlinear_tilde_gamma_z(x, direction, -epsilon)) /
            (2.0 * epsilon);
        const double error_x = std::abs(finite_difference_x - expected.g_x());
        const double error_z = std::abs(finite_difference_z - expected.g_z());
        std::cout << "INFO contracted connection eps=" << epsilon
                  << " x_error=" << error_x << " z_error=" << error_z
                  << "\n";
        if (epsilon == 1.0e-5)
        {
            middle_x = finite_difference_x;
            middle_z = finite_difference_z;
        }
        if (epsilon == 1.0e-6)
        {
            next_x = finite_difference_x;
            next_z = finite_difference_z;
        }
    }

    require_close("Stage 4AN nonlinear x finite difference", middle_x,
                  expected.g_x(), plateau_tolerance);
    require_close("independent nonlinear z finite difference", middle_z,
                  expected.g_z(), plateau_tolerance);
    require_close("x epsilon middle plateau", next_x, middle_x,
                  plateau_tolerance);
    require_close("z epsilon middle plateau", next_z, middle_z,
                  plateau_tolerance);
}

void check_negative_guards()
{
    const double x = 2.0;
    const Direction direction{0.6, 0.8, -0.4, 0.2, 0.9, -0.3, -0.5, 0.7,
                              -0.7, 1.1, 0.8, -0.4};
    const auto actual = compute(x, direction);

    const double one_hidden_x =
        0.5 * direction.dx_h_xx - 0.5 * direction.dx_h_zz -
        direction.dx_h_ww + direction.dz_h_xz +
        (direction.h_xx - direction.h_ww) / x;
    const double one_hidden_z =
        direction.dx_h_xz + direction.h_xz / x -
        0.5 * direction.dz_h_xx + 0.5 * direction.dz_h_zz -
        direction.dz_h_ww;
    require_separated("wrong hidden multiplicity in g_x would fail",
                      actual.g_x(), one_hidden_x);
    require_separated("wrong hidden multiplicity in g_z would fail",
                      actual.g_z(), one_hidden_z);

    const double wrong_g_x_derivative_sign =
        0.5 * direction.dx_h_xx - 0.5 * direction.dx_h_zz -
        direction.dx_h_ww - direction.dz_h_xz +
        2.0 * (direction.h_xx - direction.h_ww) / x;
    const double wrong_g_z_derivative_sign =
        -direction.dx_h_xz + 2.0 * direction.h_xz / x +
        0.5 * direction.dz_h_xx + 0.5 * direction.dz_h_zz -
        direction.dz_h_ww;
    require_separated("wrong dz(h_xz) sign in g_x would fail", actual.g_x(),
                      wrong_g_x_derivative_sign);
    require_separated("wrong dx(h_xz)/dz(h_xx) signs in g_z would fail",
                      actual.g_z(), wrong_g_z_derivative_sign);
}

void check_determinant_reduced_optional_cross_check()
{
    const double x = 2.0;
    const Direction trace_free{0.4, 0.5, -0.2, -0.1, 0.6, 0.7, -0.2, -0.2,
                               -0.3, -0.4, 0.1, 0.1};
    require_close("trace-free value condition",
                  trace_free.h_xx + trace_free.h_zz +
                      2.0 * trace_free.h_ww,
                  0.0);
    require_close("trace-free dx condition",
                  trace_free.dx_h_xx + trace_free.dx_h_zz +
                      2.0 * trace_free.dx_h_ww,
                  0.0);
    require_close("trace-free dz condition",
                  trace_free.dz_h_xx + trace_free.dz_h_zz +
                      2.0 * trace_free.dz_h_ww,
                  0.0);
    const auto result = compute(x, trace_free);
    const double reduced_x =
        trace_free.dx_h_xx + trace_free.dz_h_xz +
        2.0 * (trace_free.h_xx - trace_free.h_ww) / x;
    const double reduced_z =
        trace_free.dx_h_xz + trace_free.dz_h_zz +
        2.0 * trace_free.h_xz / x;
    require_close("determinant-reduced x identity optional cross-check",
                  result.g_x(), reduced_x);
    require_close("determinant-reduced z identity optional cross-check",
                  result.g_z(), reduced_z);

    const Direction unconstrained{0.4, 0.5, 0.3, -0.1, 0.6, 0.7, 0.2, -0.2,
                                  -0.3, -0.4, -0.2, 0.1};
    const auto unconstrained_result = compute(x, unconstrained);
    const double invalid_reduced_x =
        unconstrained.dx_h_xx + unconstrained.dz_h_xz +
        2.0 * (unconstrained.h_xx - unconstrained.h_ww) / x;
    require_separated("determinant-reduced identity is not assumed",
                      unconstrained_result.g_x(), invalid_reduced_x);
}

void check_parity()
{
    constexpr int points = 32;
    constexpr int mode = 3;
    double gx_cos = 0.0;
    double gx_sin = 0.0;
    double gz_cos = 0.0;
    double gz_sin = 0.0;
    for (int n = 0; n < points; ++n)
    {
        const double z = 2.0 * pi * static_cast<double>(n) /
                         static_cast<double>(points);
        const double cosine = std::cos(static_cast<double>(mode) * z);
        const double sine = std::sin(static_cast<double>(mode) * z);
        const Direction direction{
            0.4 * cosine,
            0.3 * sine,
            -0.2 * cosine,
            0.1 * cosine,
            0.7 * cosine,
            -0.4 * sine,
            -0.5 * cosine,
            0.2 * cosine,
            -static_cast<double>(mode) * 0.4 * sine,
            static_cast<double>(mode) * 0.3 * cosine,
            static_cast<double>(mode) * 0.2 * sine,
            -static_cast<double>(mode) * 0.1 * sine};
        const auto result = compute(2.0, direction);
        gx_cos += result.g_x() * cosine;
        gx_sin += result.g_x() * sine;
        gz_cos += result.g_z() * cosine;
        gz_sin += result.g_z() * sine;
    }
    const double normalization = 2.0 / static_cast<double>(points);
    gx_cos *= normalization;
    gx_sin *= normalization;
    gz_cos *= normalization;
    gz_sin *= normalization;

    require_true("g_x scalar/even allowed amplitude is nonzero",
                 std::abs(gx_cos) > 1.0e-3);
    require_close("g_x forbidden one-z leakage", gx_sin, 0.0, 1.0e-12);
    require_true("g_z one-z allowed amplitude is nonzero",
                 std::abs(gz_sin) > 1.0e-3);
    require_close("g_z forbidden scalar/even leakage", gz_cos, 0.0,
                  1.0e-12);
}

void check_invalid_inputs_and_completion_gates()
{
    const Direction zero = {};
    require_domain_error("x=0", [&]() { (void)compute(0.0, zero); });
    require_domain_error("nonfinite metric perturbation", [&]() {
        Direction invalid = zero;
        invalid.h_xx = std::numeric_limits<double>::quiet_NaN();
        (void)compute(2.0, invalid);
    });

    require_true("operator records contracted-connection helper",
                 Operator::
                     contracted_connection_and_z_reconstruction_helper_implemented);
    require_true("complete operator Gamma RHS is now implemented",
                 Operator::hat_gamma_rhs_block_implemented);
    require_true("first Gamma Z/kappa/shift-gradient block is implemented",
                 Operator::
                     hat_gamma_z4_kappa_shift_gradient_block_implemented);
    require_true("hat_Gamma^x RHS is now complete",
                 Operator::variable_rhs_complete(
                     Operator::PerturbationVariable::hat_Gamma_x));
    require_true("hat_Gamma^z RHS is now complete",
                 Operator::variable_rhs_complete(
                     Operator::PerturbationVariable::hat_Gamma_z));
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("full operator remains incomplete",
                 !contract.complete_operator_implemented());
    require_true("eigensolver remains disallowed",
                 !contract.eigensolver_allowed());
}

} // namespace

int main()
{
    check_type_and_scope_contract();
    check_zero_and_z_reconstruction();
    check_pure_component_oracles();
    check_nonlinear_finite_difference_oracles();
    check_negative_guards();
    check_determinant_reduced_optional_cross_check();
    check_parity();
    check_invalid_inputs_and_completion_gates();
    std::cout << "PASS Stage 4AO-C contracted connection and Z reconstruction\n";
    return 0;
}
