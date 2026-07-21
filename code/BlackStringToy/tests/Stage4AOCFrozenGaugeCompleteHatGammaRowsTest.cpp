#include "Stage4AOFrozenGaugeOperator.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Connection =
    BlackStringToy::Stage4AOFrozenGaugeContractedConnection;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;

constexpr int dimension = 4;
constexpr int hidden_multiplicity = 2;
constexpr double kappa1 = 0.1;
constexpr double kappa3 = 1.0;
constexpr double tolerance = 3.0e-12;

enum Family : std::size_t
{
    advection,
    z4_kappa_shift_gradient,
    k_theta_chi_gradient,
    connection_a,
    vector_hessian,
    grad_div,
    family_count
};

constexpr std::array<const char *, family_count> family_names = {
    "common advection",       "Z/kappa/shift gradient",
    "K/Theta/chi gradients",  "connection-A",
    "vector Hessian",         "grad-div"};

struct Pair
{
    double x;
    double z;
};

struct Direction
{
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
    double chi = 0.0;
    double dx_chi = 0.0;
    double dz_chi = 0.0;
    double k = 0.0;
    double dx_k = 0.0;
    double dz_k = 0.0;
    double theta = 0.0;
    double dx_theta = 0.0;
    double dz_theta = 0.0;
    double a_xx = 0.0;
    double a_xz = 0.0;
    double a_zz = 0.0;
    double a_ww = 0.0;
    double h_x = 0.0;
    double h_z = 0.0;
    double dx_h_x = 0.0;
    double dx_h_z = 0.0;
};

struct NonlinearPoint
{
    double h[2][2];
    double dh[2][2][2];
    double h_ww;
    double dx_h_ww;
    double dz_h_ww;
    double chi;
    double d_chi[2];
    double k;
    double d_k[2];
    double theta;
    double d_theta[2];
    double a[2][2];
    double a_ww;
    double hatted_gamma[2];
    double dx_hatted_gamma[2];
};

struct Geometry
{
    double inverse[2][2];
    double inverse_ww;
    double gamma[2][2][2];
    double gamma_ww[2];
    double contracted[2];
    double a_uu[2][2];
    double a_ww_uu;
};

struct NonlinearFamilies
{
    std::array<Pair, family_count> family;
    Pair total;
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
                   const double expected, const double allowed = tolerance)
{
    if (std::abs(actual - expected) > allowed)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " tolerance=" + std::to_string(allowed));
    }
    std::cout << "PASS " << label << "\n";
}

void require_pair_close(const std::string &label, const Pair actual,
                        const Pair expected, const double allowed)
{
    require_close(label + " x", actual.x, expected.x, allowed);
    require_close(label + " z", actual.z, expected.z, allowed);
}

void require_pair_separated(const std::string &label, const Pair actual,
                            const Pair mutation)
{
    const double separation =
        std::max(std::abs(actual.x - mutation.x),
                 std::abs(actual.z - mutation.z));
    if (separation < 1.0e-5)
    {
        fail(label, "mutation separation was too small");
    }
    std::cout << "PASS " << label << "\n";
}

Pair add(const Pair left, const Pair right)
{
    return {left.x + right.x, left.z + right.z};
}

Pair subtract(const Pair left, const Pair right)
{
    return {left.x - right.x, left.z - right.z};
}

Pair scale(const Pair value, const double factor)
{
    return {factor * value.x, factor * value.z};
}

NonlinearPoint make_nonlinear_point(const double r0, const double x,
                                    const double epsilon,
                                    const Direction &direction)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    NonlinearPoint point = {};
    point.h[0][0] = 1.0 + epsilon * direction.h_xx;
    point.h[0][1] = epsilon * direction.h_xz;
    point.h[1][0] = point.h[0][1];
    point.h[1][1] = 1.0 + epsilon * direction.h_zz;
    point.h_ww = 1.0 + epsilon * direction.h_ww;

    point.dh[0][0][0] = epsilon * direction.dx_h_xx;
    point.dh[0][0][1] = epsilon * direction.dx_h_xz;
    point.dh[0][1][0] = point.dh[0][0][1];
    point.dh[0][1][1] = epsilon * direction.dx_h_zz;
    point.dh[1][0][0] = epsilon * direction.dz_h_xx;
    point.dh[1][0][1] = epsilon * direction.dz_h_xz;
    point.dh[1][1][0] = point.dh[1][0][1];
    point.dh[1][1][1] = epsilon * direction.dz_h_zz;
    point.dx_h_ww = epsilon * direction.dx_h_ww;
    point.dz_h_ww = epsilon * direction.dz_h_ww;

    point.chi = 1.0 + epsilon * direction.chi;
    point.d_chi[0] = epsilon * direction.dx_chi;
    point.d_chi[1] = epsilon * direction.dz_chi;
    point.k = 3.0 * lambda / 2.0 + epsilon * direction.k;
    point.d_k[0] =
        -9.0 * lambda / (4.0 * x) + epsilon * direction.dx_k;
    point.d_k[1] = epsilon * direction.dz_k;
    point.theta = epsilon * direction.theta;
    point.d_theta[0] = epsilon * direction.dx_theta;
    point.d_theta[1] = epsilon * direction.dz_theta;

    point.a[0][0] = -7.0 * lambda / 8.0 + epsilon * direction.a_xx;
    point.a[0][1] = epsilon * direction.a_xz;
    point.a[1][0] = point.a[0][1];
    point.a[1][1] = -3.0 * lambda / 8.0 + epsilon * direction.a_zz;
    point.a_ww = 5.0 * lambda / 8.0 + epsilon * direction.a_ww;
    point.hatted_gamma[0] = epsilon * direction.h_x;
    point.hatted_gamma[1] = epsilon * direction.h_z;
    point.dx_hatted_gamma[0] = epsilon * direction.dx_h_x;
    point.dx_hatted_gamma[1] = epsilon * direction.dx_h_z;
    return point;
}

Geometry compute_geometry(const NonlinearPoint &point, const double x,
                          const int hidden_copies)
{
    Geometry geometry = {};
    const double determinant =
        point.h[0][0] * point.h[1][1] - point.h[0][1] * point.h[0][1];
    if (!(determinant > 0.0) || !(point.h_ww > 0.0))
    {
        fail("nonlinear oracle metric", "metric lost positive determinant");
    }
    geometry.inverse[0][0] = point.h[1][1] / determinant;
    geometry.inverse[0][1] = -point.h[0][1] / determinant;
    geometry.inverse[1][0] = geometry.inverse[0][1];
    geometry.inverse[1][1] = point.h[0][0] / determinant;
    geometry.inverse_ww = 1.0 / point.h_ww;

    for (int upper = 0; upper < 2; ++upper)
    {
        for (int lower_one = 0; lower_one < 2; ++lower_one)
        {
            for (int lower_two = 0; lower_two < 2; ++lower_two)
            {
                double value = 0.0;
                for (int contracted = 0; contracted < 2; ++contracted)
                {
                    value += 0.5 * geometry.inverse[upper][contracted] *
                             (point.dh[lower_one][contracted][lower_two] +
                              point.dh[lower_two][contracted][lower_one] -
                              point.dh[contracted][lower_one][lower_two]);
                }
                geometry.gamma[upper][lower_one][lower_two] = value;
            }
        }
    }

    const double first_kind_hidden_x =
        (point.h[0][0] - point.h_ww) / x - 0.5 * point.dx_h_ww;
    const double first_kind_hidden_z =
        point.h[0][1] / x - 0.5 * point.dz_h_ww;
    for (int upper = 0; upper < 2; ++upper)
    {
        geometry.gamma_ww[upper] =
            geometry.inverse[upper][0] * first_kind_hidden_x +
            geometry.inverse[upper][1] * first_kind_hidden_z;
        double contracted = 0.0;
        for (int lower_one = 0; lower_one < 2; ++lower_one)
        {
            for (int lower_two = 0; lower_two < 2; ++lower_two)
            {
                contracted += geometry.inverse[lower_one][lower_two] *
                              geometry.gamma[upper][lower_one][lower_two];
            }
        }
        geometry.contracted[upper] =
            contracted + static_cast<double>(hidden_copies) *
                             geometry.inverse_ww * geometry.gamma_ww[upper];
    }

    for (int upper_one = 0; upper_one < 2; ++upper_one)
    {
        for (int upper_two = 0; upper_two < 2; ++upper_two)
        {
            double value = 0.0;
            for (int lower_one = 0; lower_one < 2; ++lower_one)
            {
                for (int lower_two = 0; lower_two < 2; ++lower_two)
                {
                    value += geometry.inverse[upper_one][lower_one] *
                             geometry.inverse[upper_two][lower_two] *
                             point.a[lower_one][lower_two];
                }
            }
            geometry.a_uu[upper_one][upper_two] = value;
        }
    }
    geometry.a_ww_uu =
        point.a_ww * geometry.inverse_ww * geometry.inverse_ww;
    return geometry;
}

NonlinearFamilies evaluate_nonlinear_families(
    const double r0, const double x, const double epsilon,
    const Direction &direction,
    const int hidden_copies = hidden_multiplicity)
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double beta_x = std::sqrt(r0 / x);
    const double dx_beta_x = -lambda / 2.0;
    const double div_beta = 3.0 * lambda / 2.0;
    const double dxx_beta_x = 3.0 * lambda / (4.0 * x);
    const double dww_beta_x = -3.0 * lambda / (2.0 * x);
    const double dx_div_beta = -9.0 * lambda / (4.0 * x);
    const auto point = make_nonlinear_point(r0, x, epsilon, direction);
    const auto geometry = compute_geometry(point, x, hidden_copies);

    NonlinearFamilies result = {};
    result.family[advection] =
        {beta_x * point.dx_hatted_gamma[0],
         beta_x * point.dx_hatted_gamma[1]};

    double z_over_chi[2] = {};
    for (int i = 0; i < 2; ++i)
    {
        z_over_chi[i] =
            0.5 * (point.hatted_gamma[i] - geometry.contracted[i]);
        const double encoded_connection =
            geometry.contracted[i] + 2.0 * kappa3 * z_over_chi[i];
        const double algebraic =
            (2.0 / static_cast<double>(dimension)) *
                (div_beta * encoded_connection -
                 2.0 * point.k * z_over_chi[i]) -
            2.0 * kappa1 * z_over_chi[i];
        if (i == 0)
        {
            result.family[z4_kappa_shift_gradient].x =
                algebraic -
                (geometry.contracted[0] +
                 2.0 * kappa3 * z_over_chi[0]) *
                    dx_beta_x;
        }
        else
        {
            result.family[z4_kappa_shift_gradient].z = algebraic;
        }
    }

    for (int i = 0; i < 2; ++i)
    {
        double gradient = 0.0;
        for (int j = 0; j < 2; ++j)
        {
            gradient += 2.0 * geometry.inverse[i][j] * point.d_theta[j];
            gradient -=
                2.0 * static_cast<double>(dimension - 1) /
                static_cast<double>(dimension) * geometry.inverse[i][j] *
                point.d_k[j];
            gradient -= static_cast<double>(dimension) *
                        geometry.a_uu[i][j] * point.d_chi[j] / point.chi;
        }
        if (i == 0)
        {
            result.family[k_theta_chi_gradient].x = gradient;
        }
        else
        {
            result.family[k_theta_chi_gradient].z = gradient;
        }

        double connection = 0.0;
        for (int lower_one = 0; lower_one < 2; ++lower_one)
        {
            for (int lower_two = 0; lower_two < 2; ++lower_two)
            {
                connection += geometry.gamma[i][lower_one][lower_two] *
                              geometry.a_uu[lower_one][lower_two];
            }
        }
        connection += static_cast<double>(hidden_copies) *
                      geometry.gamma_ww[i] * geometry.a_ww_uu;
        if (i == 0)
        {
            result.family[connection_a].x = 2.0 * connection;
        }
        else
        {
            result.family[connection_a].z = 2.0 * connection;
        }
    }

    result.family[vector_hessian] =
        {geometry.inverse[0][0] * dxx_beta_x +
             static_cast<double>(hidden_copies) * geometry.inverse_ww *
                 dww_beta_x,
         0.0};
    const double grad_div_prefactor =
        static_cast<double>(dimension - 2) / static_cast<double>(dimension);
    result.family[grad_div] =
        {grad_div_prefactor * geometry.inverse[0][0] * dx_div_beta,
         grad_div_prefactor * geometry.inverse[1][0] * dx_div_beta};

    result.total = {0.0, 0.0};
    for (const auto family : result.family)
    {
        result.total = add(result.total, family);
    }
    return result;
}

NonlinearFamilies central_difference(
    const double r0, const double x, const double epsilon,
    const Direction &direction,
    const int hidden_copies = hidden_multiplicity)
{
    const auto plus =
        evaluate_nonlinear_families(r0, x, epsilon, direction, hidden_copies);
    const auto minus =
        evaluate_nonlinear_families(r0, x, -epsilon, direction, hidden_copies);
    NonlinearFamilies result = {};
    for (std::size_t family = 0; family < family_count; ++family)
    {
        result.family[family] =
            scale(subtract(plus.family[family], minus.family[family]),
                  1.0 / (2.0 * epsilon));
        result.total = add(result.total, result.family[family]);
    }
    return result;
}

Vector make_linear_state(const Direction &direction)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    values[Operator::variable_index(Variable::chi)] = direction.chi;
    values[Operator::variable_index(Variable::h_xx)] = direction.h_xx;
    values[Operator::variable_index(Variable::h_xz)] = direction.h_xz;
    values[Operator::variable_index(Variable::h_zz)] = direction.h_zz;
    values[Operator::variable_index(Variable::h_ww)] = direction.h_ww;
    values[Operator::variable_index(Variable::K)] = direction.k;
    values[Operator::variable_index(Variable::A_xx)] = direction.a_xx;
    values[Operator::variable_index(Variable::A_xz)] = direction.a_xz;
    values[Operator::variable_index(Variable::A_zz)] = direction.a_zz;
    values[Operator::variable_index(Variable::A_ww)] = direction.a_ww;
    values[Operator::variable_index(Variable::Theta)] = direction.theta;
    values[Operator::variable_index(Variable::hat_Gamma_x)] = direction.h_x;
    values[Operator::variable_index(Variable::hat_Gamma_z)] = direction.h_z;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector make_common_advection_dependency(const double r0, const double x,
                                        const Direction &direction)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    const double beta = std::sqrt(r0 / x);
    values[Operator::variable_index(Variable::hat_Gamma_x)] =
        beta * direction.dx_h_x;
    values[Operator::variable_index(Variable::hat_Gamma_z)] =
        beta * direction.dx_h_z;
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Connection::PerturbationJet make_connection_jet(const double x,
                                                 const Direction &direction)
{
    return Connection::make_perturbation_jet(
        x, direction.h_xx, direction.h_xz, direction.h_zz, direction.h_ww,
        direction.dx_h_xx, direction.dx_h_xz, direction.dx_h_zz,
        direction.dx_h_ww, direction.dz_h_xx, direction.dz_h_xz,
        direction.dz_h_zz, direction.dz_h_ww, direction.h_x, direction.h_z);
}

Pair assemble(const double r0, const double x, const Direction &direction,
              const Vector *advection_dependency = nullptr)
{
    const auto state = make_linear_state(direction);
    const auto default_advection =
        make_common_advection_dependency(r0, x, direction);
    const auto &advection_output =
        advection_dependency == nullptr ? default_advection
                                        : *advection_dependency;
    const auto output = Operator::apply_complete_hat_gamma_rows_at_point(
        r0, state, advection_output, make_connection_jet(x, direction),
        direction.dx_k, direction.dz_k, direction.dx_theta,
        direction.dz_theta, direction.dx_chi, direction.dz_chi);
    return {output.value(Variable::hat_Gamma_x),
            output.value(Variable::hat_Gamma_z)};
}

std::array<Pair, family_count> production_families(
    const double r0, const double x, const Direction &direction)
{
    const auto state = make_linear_state(direction);
    const auto jet = make_connection_jet(x, direction);
    const auto advection_output =
        make_common_advection_dependency(r0, x, direction);
    const auto z4 =
        Operator::apply_hat_gamma_z4_kappa_shift_gradient_insertion_at_point(
            r0, jet);
    const auto gradients =
        Operator::apply_hat_gamma_k_theta_chi_gradient_insertion_at_point(
            r0, x, state, direction.dx_k, direction.dz_k,
            direction.dx_theta, direction.dz_theta, direction.dx_chi,
            direction.dz_chi);
    const auto connection =
        Operator::apply_hat_gamma_connection_a_insertion_at_point(
            r0, x, state, direction.dx_h_xx, direction.dx_h_xz,
            direction.dx_h_zz, direction.dx_h_ww, direction.dz_h_xx,
            direction.dz_h_xz, direction.dz_h_zz, direction.dz_h_ww);
    const auto vector =
        Operator::apply_hat_gamma_vector_hessian_insertion_at_point(r0, x,
                                                                    state);
    const auto grad =
        Operator::apply_hat_gamma_grad_div_insertion_at_point(r0, x, state);
    const std::array<Vector, family_count> outputs = {
        advection_output, z4, gradients, connection, vector, grad};
    std::array<Pair, family_count> result = {};
    for (std::size_t family = 0; family < family_count; ++family)
    {
        result[family] =
            {outputs[family].value(Variable::hat_Gamma_x),
             outputs[family].value(Variable::hat_Gamma_z)};
    }
    return result;
}

Direction mixed_one()
{
    Direction d;
    d.h_xx = 0.31;
    d.h_xz = -0.22;
    d.h_zz = -0.17;
    d.h_ww = 0.28;
    d.dx_h_xx = -0.41;
    d.dx_h_xz = 0.19;
    d.dx_h_zz = 0.37;
    d.dx_h_ww = -0.13;
    d.dz_h_xx = 0.23;
    d.dz_h_xz = -0.29;
    d.dz_h_zz = -0.11;
    d.dz_h_ww = 0.07;
    d.chi = 0.13;
    d.dx_chi = 0.21;
    d.dz_chi = -0.16;
    d.k = -0.27;
    d.dx_k = 0.34;
    d.dz_k = -0.29;
    d.theta = 0.18;
    d.dx_theta = -0.26;
    d.dz_theta = 0.24;
    d.a_xx = 0.11;
    d.a_xz = -0.09;
    d.a_zz = 0.07;
    d.a_ww = -0.05;
    d.h_x = 0.23;
    d.h_z = -0.19;
    d.dx_h_x = 0.32;
    d.dx_h_z = -0.28;
    return d;
}

Direction mixed_two()
{
    Direction d = mixed_one();
    d.h_xx = -0.24;
    d.h_xz = 0.27;
    d.h_zz = 0.16;
    d.h_ww = -0.21;
    d.dx_h_xx = 0.33;
    d.dx_h_xz = -0.28;
    d.dx_h_zz = -0.35;
    d.dx_h_ww = 0.17;
    d.dz_h_xx = -0.26;
    d.dz_h_xz = 0.31;
    d.dz_h_zz = 0.14;
    d.dz_h_ww = -0.09;
    d.dx_k = -0.38;
    d.dz_k = 0.25;
    d.dx_theta = 0.29;
    d.dz_theta = -0.22;
    d.dx_chi = -0.18;
    d.dz_chi = 0.20;
    d.h_x = -0.17;
    d.h_z = 0.21;
    d.dx_h_x = -0.30;
    d.dx_h_z = 0.26;
    return d;
}

void check_background_and_zero()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const Direction zero;
    const auto background = evaluate_nonlinear_families(r0, x, 0.0, zero);
    std::cout << std::scientific << std::setprecision(12)
              << "INFO Gamma GP background gradient_x="
              << background.family[k_theta_chi_gradient].x
              << " vector_x=" << background.family[vector_hessian].x
              << " grad_div_x=" << background.family[grad_div].x
              << " total_x=" << background.total.x
              << " total_z=" << background.total.z << "\n";
    require_close("exact GP nonlinear Gamma x residual", background.total.x,
                  0.0, 2.0e-14);
    require_close("exact GP nonlinear Gamma z residual", background.total.z,
                  0.0, 2.0e-14);
    require_pair_close("zero perturbation linearized rows",
                       assemble(r0, x, zero), {0.0, 0.0}, 0.0);
}

void check_directed_family_oracles()
{
    const double r0 = 1.1;
    const double x = 2.2;
    std::array<Direction, family_count> directions = {};
    directions[advection].dx_h_x = 0.7;
    directions[advection].dx_h_z = -0.6;
    directions[z4_kappa_shift_gradient].h_x = 0.8;
    directions[z4_kappa_shift_gradient].h_z = -0.5;
    directions[k_theta_chi_gradient].dx_k = 0.4;
    directions[k_theta_chi_gradient].dz_k = -0.3;
    directions[k_theta_chi_gradient].dx_theta = -0.6;
    directions[k_theta_chi_gradient].dz_theta = 0.7;
    directions[k_theta_chi_gradient].dx_chi = 0.5;
    directions[k_theta_chi_gradient].dz_chi = -0.4;
    directions[connection_a].dx_h_xx = 0.9;
    directions[connection_a].dx_h_zz = 0.9;
    directions[vector_hessian].h_xx = 0.65;
    directions[vector_hessian].h_ww = 0.65;
    directions[grad_div].h_xz = -0.75;

    for (std::size_t selected = 0; selected < family_count; ++selected)
    {
        const auto oracle =
            central_difference(r0, x, 1.0e-5, directions[selected]);
        const auto production =
            production_families(r0, x, directions[selected]);
        require_pair_close(std::string("independent directed-family oracle ") +
                               family_names[selected],
                           production[selected], oracle.family[selected],
                           3.0e-8);
        require_pair_close(std::string("directed-family assembled total ") +
                               family_names[selected],
                           assemble(r0, x, directions[selected]), oracle.total,
                           4.0e-8);
    }
}

void check_isolated_vector_hessian_and_grad_div()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double a = 0.37;

    Direction isolated_vector;
    isolated_vector.h_ww = a;
    isolated_vector.dx_h_ww = -2.0 * a / x;
    const auto vector_jet = make_connection_jet(x, isolated_vector);
    const auto vector_connection =
        Connection::compute_contracted_connection_and_z(vector_jet);
    const auto vector_families =
        production_families(r0, x, isolated_vector);
    const Pair vector_target = {3.0 * lambda * a / x, 0.0};

    require_close("isolated vector-Hessian g_x", vector_connection.g_x(),
                  0.0, 2.0e-14);
    require_close("isolated vector-Hessian Z_x", vector_connection.z_x(),
                  0.0, 2.0e-14);
    require_pair_close("isolated vector-Hessian advection family",
                       vector_families[advection], {0.0, 0.0}, 0.0);
    require_pair_close("isolated vector-Hessian Z/kappa family",
                       vector_families[z4_kappa_shift_gradient], {0.0, 0.0},
                       2.0e-14);
    require_pair_close("isolated vector-Hessian gradient family",
                       vector_families[k_theta_chi_gradient], {0.0, 0.0},
                       0.0);
    require_pair_close("isolated vector-Hessian connection-A family",
                       vector_families[connection_a], {0.0, 0.0}, 2.0e-14);
    require_pair_close("isolated vector-Hessian target family",
                       vector_families[vector_hessian], vector_target,
                       2.0e-14);
    require_true("isolated vector-Hessian target is nonzero",
                 std::abs(vector_families[vector_hessian].x) > 1.0e-4);
    require_pair_close("isolated vector-Hessian grad-div family",
                       vector_families[grad_div], {0.0, 0.0}, 0.0);
    require_pair_close("isolated vector-Hessian assembled total",
                       assemble(r0, x, isolated_vector), vector_target,
                       3.0e-14);
    require_pair_close(
        "isolated vector-Hessian nonlinear finite-difference total",
        central_difference(r0, x, 1.0e-5, isolated_vector).total,
        vector_target, 4.0e-8);

    Direction isolated_grad_div;
    isolated_grad_div.h_xz = a;
    isolated_grad_div.dx_h_xz = 0.0;
    isolated_grad_div.dz_h_ww = 2.0 * a / x;
    isolated_grad_div.dz_theta = 27.0 * lambda * a / (16.0 * x);
    const auto grad_div_jet = make_connection_jet(x, isolated_grad_div);
    const auto grad_div_connection =
        Connection::compute_contracted_connection_and_z(grad_div_jet);
    const auto grad_div_families =
        production_families(r0, x, isolated_grad_div);
    const Pair grad_div_target = {0.0, 9.0 * lambda * a / (8.0 * x)};

    require_close("isolated grad-div g_z", grad_div_connection.g_z(), 0.0,
                  2.0e-14);
    require_close("isolated grad-div Z_z", grad_div_connection.z_z(), 0.0,
                  2.0e-14);
    require_pair_close("isolated grad-div advection family",
                       grad_div_families[advection], {0.0, 0.0}, 0.0);
    require_pair_close("isolated grad-div Z/kappa family",
                       grad_div_families[z4_kappa_shift_gradient], {0.0, 0.0},
                       2.0e-14);
    require_pair_close("isolated grad-div gradient family",
                       grad_div_families[k_theta_chi_gradient], {0.0, 0.0},
                       2.0e-14);
    require_pair_close("isolated grad-div connection-A family",
                       grad_div_families[connection_a], {0.0, 0.0}, 2.0e-14);
    require_pair_close("isolated grad-div vector-Hessian family",
                       grad_div_families[vector_hessian], {0.0, 0.0}, 0.0);
    require_pair_close("isolated grad-div target family",
                       grad_div_families[grad_div], grad_div_target,
                       2.0e-14);
    require_true("isolated grad-div target is nonzero",
                 std::abs(grad_div_families[grad_div].z) > 1.0e-4);
    require_pair_close("isolated grad-div assembled total",
                       assemble(r0, x, isolated_grad_div), grad_div_target,
                       3.0e-14);
    require_pair_close("isolated grad-div nonlinear finite-difference total",
                       central_difference(r0, x, 1.0e-5, isolated_grad_div)
                           .total,
                       grad_div_target, 4.0e-8);

    std::cout << std::scientific << std::setprecision(12)
              << "INFO isolated vector-Hessian x=" << vector_target.x
              << " isolated grad-div z=" << grad_div_target.z << "\n";
}

void check_epsilon_sweeps_and_mixed_cases()
{
    const double r0 = 0.9;
    const double x = 1.8;
    const std::array<double, 5> epsilons = {1.0e-2, 1.0e-4, 1.0e-5,
                                            1.0e-6, 1.0e-7};
    const auto direction = mixed_one();
    const auto assembled = assemble(r0, x, direction);
    std::array<Pair, 5> estimates = {};
    std::cout << "INFO Gamma epsilon table\n";
    for (std::size_t i = 0; i < epsilons.size(); ++i)
    {
        estimates[i] = central_difference(r0, x, epsilons[i], direction).total;
        std::cout << std::scientific << std::setprecision(12)
                  << "INFO eps=" << epsilons[i]
                  << " fd_x=" << estimates[i].x
                  << " err_x=" << std::abs(estimates[i].x - assembled.x)
                  << " fd_z=" << estimates[i].z
                  << " err_z=" << std::abs(estimates[i].z - assembled.z)
                  << "\n";
    }
    require_pair_close("mixed-one epsilon 1e-5 oracle", assembled,
                       estimates[2], 5.0e-8);
    require_pair_close("mixed-one epsilon 1e-6 oracle", assembled,
                       estimates[3], 8.0e-8);
    require_pair_close("mixed-one stable 1e-5/1e-6 plateau", estimates[2],
                       estimates[3], 8.0e-8);

    const auto second = mixed_two();
    const auto second_assembled = assemble(r0, x, second);
    const auto second_mid = central_difference(r0, x, 1.0e-5, second).total;
    const auto second_low = central_difference(r0, x, 1.0e-6, second).total;
    require_pair_close("mixed-two epsilon 1e-5 oracle", second_assembled,
                       second_mid, 5.0e-8);
    require_pair_close("mixed-two stable middle plateau", second_mid,
                       second_low, 8.0e-8);
}

void check_ownership_and_mutations()
{
    const double r0 = 1.0;
    const double x = 1.9;
    const auto direction = mixed_one();
    const auto actual = assemble(r0, x, direction);
    const auto oracle = central_difference(r0, x, 1.0e-5, direction);
    const auto production = production_families(r0, x, direction);

    for (std::size_t family = 0; family < family_count; ++family)
    {
        require_pair_close(std::string("mixed independent family ") +
                               family_names[family],
                           production[family], oracle.family[family],
                           5.0e-8);
        require_true(std::string("mixed family is activated ") +
                         family_names[family],
                     std::max(std::abs(oracle.family[family].x),
                              std::abs(oracle.family[family].z)) > 1.0e-4);
        require_pair_separated(std::string("omission mutation ") +
                                   family_names[family],
                               actual,
                               subtract(actual, oracle.family[family]));
        require_pair_separated(std::string("duplicate-family mutation ") +
                                   family_names[family],
                               actual, add(actual, oracle.family[family]));
    }

    const double lambda = std::sqrt(r0 / (x * x * x));
    require_pair_separated("spurious +lambda H_z/2 mutation", actual,
                           {actual.x,
                            actual.z + lambda * direction.h_z / 2.0});
    const auto hidden_one =
        central_difference(r0, x, 1.0e-5, direction, 1).total;
    require_pair_separated("hidden multiplicity-one mutation", actual,
                           hidden_one);
}

void check_actual_common_advection_dependency()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 3.5);
    const auto grid = Operator::make_radial_grid(domain, 6);
    constexpr std::size_t center = 3;
    const double center_x = grid.x(center);
    const double slope_x = 0.7;
    const double slope_z = -0.55;
    std::vector<Vector> field;
    field.reserve(grid.points());
    for (std::size_t i = 0; i < grid.points(); ++i)
    {
        std::array<double, Operator::frozen_gauge_state_vector.size()> values =
            {};
        values[Operator::variable_index(Variable::hat_Gamma_x)] =
            slope_x * (grid.x(i) - center_x);
        values[Operator::variable_index(Variable::hat_Gamma_z)] =
            slope_z * (grid.x(i) - center_x);
        field.push_back(Operator::make_frozen_gauge_perturbation_vector(values));
    }
    const auto advection = Operator::apply_gp_shift_advection_block(grid, field);
    Direction direction;
    direction.dx_h_x = slope_x;
    direction.dx_h_z = slope_z;
    const auto actual =
        assemble(domain.r0(), center_x, direction, &advection.at(center));
    const double beta = std::sqrt(domain.r0() / center_x);
    require_pair_close("assembler consumes actual common-advection output",
                       actual, {beta * slope_x, beta * slope_z}, 2.0e-13);

    std::array<double, Operator::frozen_gauge_state_vector.size()> zeros = {};
    const auto zero_dependency =
        Operator::make_frozen_gauge_perturbation_vector(zeros);
    require_pair_close("assembler does not reimplement missing advection",
                       assemble(domain.r0(), center_x, direction,
                                &zero_dependency),
                       {0.0, 0.0}, 0.0);
}

Direction parity_direction(const double z)
{
    const double wave_number = 1.4;
    const double cosine = std::cos(wave_number * z);
    const double sine = std::sin(wave_number * z);
    Direction d = mixed_one();
    d.h_xx = 0.31 * cosine;
    d.h_xz = -0.22 * sine;
    d.h_zz = -0.17 * cosine;
    d.h_ww = 0.28 * cosine;
    d.dx_h_xx = -0.41 * cosine;
    d.dx_h_xz = 0.19 * sine;
    d.dx_h_zz = 0.37 * cosine;
    d.dx_h_ww = -0.13 * cosine;
    d.dz_h_xx = -0.31 * wave_number * sine;
    d.dz_h_xz = -0.22 * wave_number * cosine;
    d.dz_h_zz = 0.17 * wave_number * sine;
    d.dz_h_ww = -0.28 * wave_number * sine;
    d.chi = 0.13 * cosine;
    d.dx_chi = 0.21 * cosine;
    d.dz_chi = -0.13 * wave_number * sine;
    d.k = -0.27 * cosine;
    d.dx_k = 0.34 * cosine;
    d.dz_k = 0.27 * wave_number * sine;
    d.theta = 0.18 * cosine;
    d.dx_theta = -0.26 * cosine;
    d.dz_theta = -0.18 * wave_number * sine;
    d.a_xx = 0.11 * cosine;
    d.a_xz = -0.09 * sine;
    d.a_zz = 0.07 * cosine;
    d.a_ww = -0.05 * cosine;
    d.h_x = 0.23 * cosine;
    d.h_z = -0.19 * sine;
    d.dx_h_x = 0.32 * cosine;
    d.dx_h_z = -0.28 * sine;
    return d;
}

void check_parity_scope_and_flags()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const auto positive = assemble(r0, x, parity_direction(0.37));
    const auto negative = assemble(r0, x, parity_direction(-0.37));
    const double forbidden_x_odd = 0.5 * (positive.x - negative.x);
    const double forbidden_z_even = 0.5 * (positive.z + negative.z);
    require_close("assembled raw x forbidden odd leakage", forbidden_x_odd,
                  0.0, 3.0e-13);
    require_close("assembled raw z forbidden even leakage", forbidden_z_even,
                  0.0, 3.0e-13);
    require_true("assembled x even amplitude is nonzero",
                 std::abs(0.5 * (positive.x + negative.x)) > 1.0e-4);
    require_true("assembled z one-z amplitude is nonzero",
                 std::abs(0.5 * (positive.z - negative.z)) > 1.0e-4);

    const auto state = make_linear_state(mixed_one());
    const auto output = Operator::apply_complete_hat_gamma_rows_at_point(
        r0, state, make_common_advection_dependency(r0, x, mixed_one()),
        make_connection_jet(x, mixed_one()), mixed_one().dx_k,
        mixed_one().dz_k, mixed_one().dx_theta, mixed_one().dz_theta,
        mixed_one().dx_chi, mixed_one().dz_chi);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::hat_Gamma_x &&
            variable != Variable::hat_Gamma_z)
        {
            require_close(std::string("complete Gamma rows leave ") +
                              Operator::variable_name(variable) +
                              " untouched",
                          output.value(variable), 0.0, 0.0);
        }
    }

    const auto contract =
        Operator::make_default_frozen_gauge_operator_contract();
    require_true("complete Gamma family inventory remains closed",
                 Operator::hat_gamma_surviving_term_family_inventory_closed);
    require_true("complete Gamma row assembly flag is true",
                 Operator::hat_gamma_final_row_assembly_implemented);
    require_true("assembled Gamma validation flag is true",
                 Operator::hat_gamma_assembled_row_validation_implemented);
    require_true("complete Gamma RHS flag is true",
                 Operator::hat_gamma_rhs_block_implemented);
    require_true("hat_Gamma^x variable RHS is complete",
                 Operator::variable_rhs_complete(Variable::hat_Gamma_x));
    require_true("hat_Gamma^z variable RHS is complete",
                 Operator::variable_rhs_complete(Variable::hat_Gamma_z));
    require_true("K variable RHS remains incomplete",
                 !Operator::variable_rhs_complete(Variable::K));
    require_true("complete 13-variable operator remains false",
                 !contract.complete_operator_implemented());
    require_true("eigensolver remains disallowed",
                 !contract.eigensolver_allowed());
}

} // namespace

int main()
{
    check_background_and_zero();
    check_directed_family_oracles();
    check_isolated_vector_hessian_and_grad_div();
    check_epsilon_sweeps_and_mixed_cases();
    check_ownership_and_mutations();
    check_actual_common_advection_dependency();
    check_parity_scope_and_flags();
    std::cout << "PASS Stage 4AO-C complete frozen-gauge hatted-Gamma rows\n";
    return 0;
}
