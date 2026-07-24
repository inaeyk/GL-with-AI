#ifndef BLACKSTRINGTOY_TESTS_STAGE4AOC_ANALYTIC_FULL_ORACLE_HPP
#define BLACKSTRINGTOY_TESTS_STAGE4AOC_ANALYTIC_FULL_ORACLE_HPP

#include "Stage4AOFrozenGaugeOperator.hpp"

#include <array>
#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace Stage4AOCAnalyticFullOracle
{
namespace Operator = Stage4AOFrozenGaugeOperator;
using Variable = Operator::PerturbationVariable;
using Real = long double;

constexpr int dimension = 4;
constexpr int hidden_multiplicity = 2;
constexpr std::size_t variables = Operator::frozen_gauge_state_vector.size();

struct Jet
{
    double value = 0.0;
    double dx = 0.0;
    double dz = 0.0;
    double dxx = 0.0;
    double dxz = 0.0;
    double dzz = 0.0;
};

struct Direction
{
    std::array<Jet, variables> data = {};
    Jet &at(const Variable variable)
    {
        return data[Operator::variable_index(variable)];
    }
    const Jet &at(const Variable variable) const
    {
        return data[Operator::variable_index(variable)];
    }
};

using Tensor = std::array<std::array<Real, dimension>, dimension>;
using Vector4 = std::array<Real, dimension>;
using DerivativeTensor =
    std::array<std::array<std::array<Real, dimension>, dimension>, dimension>;
using SecondDerivativeTensor = std::array<DerivativeTensor, dimension>;
using Christoffel = std::array<Tensor, dimension>;
using Rows = std::array<Real, variables>;

enum Family : std::size_t
{
    common_advection,
    chi_metric_algebraic,
    metric_shift_stretching,
    geometric_ricci,
    encoded_z,
    k_theta_a_algebraic,
    locked_damping,
    a_shift_stretching,
    gamma_non_advection,
    family_count
};

inline constexpr std::array<const char *, family_count> family_names = {
    "common advection",       "chi/metric algebraic",
    "metric shift stretching", "geometric Ricci",
    "encoded Z",             "K/Theta/A algebraic",
    "locked damping",        "A shift stretching",
    "Gamma non-advection"};

struct Result
{
    std::array<Rows, family_count> family = {};
    Rows total = {};
};

struct Context
{
    Real r0;
    Real x;
    Real epsilon;
    const Direction *direction;
    int hidden_copies;
};

struct AnalyticJet
{
    Real value = 0.0L;
    Real dx = 0.0L;
    Real dz = 0.0L;
    Real dxx = 0.0L;
    Real dxz = 0.0L;
    Real dzz = 0.0L;
};

struct AxisTensorJets
{
    Tensor value = {};
    DerivativeTensor first = {};
    SecondDerivativeTensor second = {};
};

struct Geometry
{
    Tensor metric = {};
    Tensor inverse = {};
    DerivativeTensor first = {};
    DerivativeTensor inverse_first = {};
    SecondDerivativeTensor second = {};
    Christoffel gamma = {};
    std::array<Christoffel, dimension> gamma_first = {};
    Tensor ricci = {};
};

inline std::size_t slot(const Variable variable)
{
    return Operator::variable_index(variable);
}

inline AnalyticJet state_jet(const Context &context, const Variable variable)
{
    const auto &perturbation = context.direction->at(variable);
    AnalyticJet out{context.epsilon * perturbation.value,
                    context.epsilon * perturbation.dx,
                    context.epsilon * perturbation.dz,
                    context.epsilon * perturbation.dxx,
                    context.epsilon * perturbation.dxz,
                    context.epsilon * perturbation.dzz};
    const Real lambda =
        std::sqrt(context.r0 / (context.x * context.x * context.x));
    const Real lambda_x = -1.5L * lambda / context.x;
    const Real lambda_xx = 3.75L * lambda / (context.x * context.x);
    Real constant = 0.0L;
    Real lambda_coefficient = 0.0L;
    switch (variable)
    {
    case Variable::chi:
    case Variable::h_xx:
    case Variable::h_zz:
    case Variable::h_ww:
        constant = 1.0L;
        break;
    case Variable::K:
        lambda_coefficient = 1.5L;
        break;
    case Variable::A_xx:
        lambda_coefficient = -0.875L;
        break;
    case Variable::A_zz:
        lambda_coefficient = -0.375L;
        break;
    case Variable::A_ww:
        lambda_coefficient = 0.625L;
        break;
    default:
        break;
    }
    out.value += constant + lambda_coefficient * lambda;
    out.dx += lambda_coefficient * lambda_x;
    out.dxx += lambda_coefficient * lambda_xx;
    return out;
}

inline AnalyticJet reciprocal(const AnalyticJet &input)
{
    const Real inverse = 1.0L / input.value;
    const Real inverse2 = inverse * inverse;
    const Real inverse3 = inverse2 * inverse;
    return {inverse,
            -input.dx * inverse2,
            -input.dz * inverse2,
            2.0L * input.dx * input.dx * inverse3 - input.dxx * inverse2,
            2.0L * input.dx * input.dz * inverse3 - input.dxz * inverse2,
            2.0L * input.dz * input.dz * inverse3 - input.dzz * inverse2};
}

inline AnalyticJet multiply(const AnalyticJet &left,
                            const AnalyticJet &right)
{
    return {left.value * right.value,
            left.dx * right.value + left.value * right.dx,
            left.dz * right.value + left.value * right.dz,
            left.dxx * right.value + 2.0L * left.dx * right.dx +
                left.value * right.dxx,
            left.dxz * right.value + left.dx * right.dz +
                left.dz * right.dx + left.value * right.dxz,
            left.dzz * right.value + 2.0L * left.dz * right.dz +
                left.value * right.dzz};
}

inline AxisTensorJets lift_axis_tensor(const Real x, const AnalyticJet &xx,
                                       const AnalyticJet &xz,
                                       const AnalyticJet &zz,
                                       const AnalyticJet &ww)
{
    AxisTensorJets out;
    out.value[0][0] = xx.value;
    out.value[0][1] = out.value[1][0] = xz.value;
    out.value[1][1] = zz.value;
    out.value[2][2] = out.value[3][3] = ww.value;
    const std::array<const AnalyticJet *, 4> components = {&xx, &xz, &zz,
                                                           &ww};
    const std::array<std::array<int, 2>, 4> indices = {
        std::array<int, 2>{0, 0}, {0, 1}, {1, 1}, {2, 2}};
    for (std::size_t component = 0; component < components.size(); ++component)
    {
        const int i = indices[component][0];
        const int j = indices[component][1];
        const auto &jet = *components[component];
        out.first[0][i][j] = out.first[0][j][i] = jet.dx;
        out.first[1][i][j] = out.first[1][j][i] = jet.dz;
        out.second[0][0][i][j] = out.second[0][0][j][i] = jet.dxx;
        out.second[0][1][i][j] = out.second[1][0][i][j] = jet.dxz;
        out.second[0][1][j][i] = out.second[1][0][j][i] = jet.dxz;
        out.second[1][1][i][j] = out.second[1][1][j][i] = jet.dzz;
    }
    out.first[0][3][3] = ww.dx;
    out.first[1][3][3] = ww.dz;
    out.second[0][0][3][3] = ww.dxx;
    out.second[0][1][3][3] = out.second[1][0][3][3] = ww.dxz;
    out.second[1][1][3][3] = ww.dzz;

    for (const int hidden : {2, 3})
    {
        const int other = hidden == 2 ? 3 : 2;
        out.first[hidden][hidden][0] = out.first[hidden][0][hidden] =
            (xx.value - ww.value) / x;
        out.first[hidden][hidden][1] = out.first[hidden][1][hidden] =
            xz.value / x;
        out.second[hidden][hidden][0][0] =
            xx.dx / x - 2.0L * (xx.value - ww.value) / (x * x);
        out.second[hidden][hidden][0][1] =
            out.second[hidden][hidden][1][0] = xz.dx / x - xz.value / (x * x);
        out.second[hidden][hidden][1][1] = zz.dx / x;
        out.second[hidden][hidden][hidden][hidden] =
            ww.dx / x + 2.0L * (xx.value - ww.value) / (x * x);
        out.second[hidden][hidden][other][other] = ww.dx / x;

        const Real radial_x =
            (xx.dx - ww.dx) / x - (xx.value - ww.value) / (x * x);
        const Real radial_z = xz.dx / x - xz.value / (x * x);
        out.second[0][hidden][hidden][0] =
            out.second[hidden][0][hidden][0] = radial_x;
        out.second[0][hidden][0][hidden] =
            out.second[hidden][0][0][hidden] = radial_x;
        out.second[0][hidden][hidden][1] =
            out.second[hidden][0][hidden][1] = radial_z;
        out.second[0][hidden][1][hidden] =
            out.second[hidden][0][1][hidden] = radial_z;

        const Real axial_x = (xx.dz - ww.dz) / x;
        const Real axial_z = xz.dz / x;
        out.second[1][hidden][hidden][0] =
            out.second[hidden][1][hidden][0] = axial_x;
        out.second[1][hidden][0][hidden] =
            out.second[hidden][1][0][hidden] = axial_x;
        out.second[1][hidden][hidden][1] =
            out.second[hidden][1][hidden][1] = axial_z;
        out.second[1][hidden][1][hidden] =
            out.second[hidden][1][1][hidden] = axial_z;
    }
    const Real mixed_hidden = (xx.value - ww.value) / (x * x);
    out.second[2][3][2][3] = out.second[2][3][3][2] = mixed_hidden;
    out.second[3][2][2][3] = out.second[3][2][3][2] = mixed_hidden;
    return out;
}

inline AxisTensorJets conformal_metric_jets(const Context &context)
{
    return lift_axis_tensor(
        context.x, state_jet(context, Variable::h_xx),
        state_jet(context, Variable::h_xz),
        state_jet(context, Variable::h_zz),
        state_jet(context, Variable::h_ww));
}

inline AxisTensorJets physical_metric_jets(const Context &context)
{
    const auto chi_inverse = reciprocal(state_jet(context, Variable::chi));
    return lift_axis_tensor(
        context.x,
        multiply(state_jet(context, Variable::h_xx), chi_inverse),
        multiply(state_jet(context, Variable::h_xz), chi_inverse),
        multiply(state_jet(context, Variable::h_zz), chi_inverse),
        multiply(state_jet(context, Variable::h_ww), chi_inverse));
}

inline AxisTensorJets a_tensor_jets(const Context &context)
{
    return lift_axis_tensor(
        context.x, state_jet(context, Variable::A_xx),
        state_jet(context, Variable::A_xz),
        state_jet(context, Variable::A_zz),
        state_jet(context, Variable::A_ww));
}

inline Tensor invert(const Tensor &input)
{
    std::array<std::array<Real, 2 * dimension>, dimension> work = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            work[i][j] = input[i][j];
            work[i][j + dimension] = i == j ? 1.0L : 0.0L;
        }
    }
    for (int column = 0; column < dimension; ++column)
    {
        int pivot = column;
        for (int row = column + 1; row < dimension; ++row)
        {
            if (std::abs(work[row][column]) >
                std::abs(work[pivot][column]))
            {
                pivot = row;
            }
        }
        if (std::abs(work[pivot][column]) < 1.0e-14L)
        {
            throw std::runtime_error("analytic full oracle singular metric");
        }
        const auto temporary = work[pivot];
        work[pivot] = work[column];
        work[column] = temporary;
        const Real divisor = work[column][column];
        for (auto &value : work[column])
        {
            value /= divisor;
        }
        for (int row = 0; row < dimension; ++row)
        {
            if (row == column)
            {
                continue;
            }
            const Real factor = work[row][column];
            for (int entry = 0; entry < 2 * dimension; ++entry)
            {
                work[row][entry] -= factor * work[column][entry];
            }
        }
    }
    Tensor inverse = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            inverse[i][j] = work[i][j + dimension];
        }
    }
    return inverse;
}

inline Geometry compute_geometry(const AxisTensorJets &jets)
{
    Geometry geometry;
    geometry.metric = jets.value;
    geometry.first = jets.first;
    geometry.second = jets.second;
    geometry.inverse = invert(geometry.metric);
    Christoffel lower = {};
    for (int a = 0; a < dimension; ++a)
    {
        for (int b = 0; b < dimension; ++b)
        {
            for (int c = 0; c < dimension; ++c)
            {
                lower[a][b][c] =
                    0.5L * (geometry.first[b][a][c] +
                            geometry.first[c][a][b] -
                            geometry.first[a][b][c]);
            }
        }
    }
    for (int a = 0; a < dimension; ++a)
    {
        for (int b = 0; b < dimension; ++b)
        {
            for (int c = 0; c < dimension; ++c)
            {
                for (int d = 0; d < dimension; ++d)
                {
                    geometry.gamma[a][b][c] +=
                        geometry.inverse[a][d] * lower[d][b][c];
                }
            }
        }
    }
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        for (int a = 0; a < dimension; ++a)
        {
            for (int d = 0; d < dimension; ++d)
            {
                for (int e = 0; e < dimension; ++e)
                {
                    for (int f = 0; f < dimension; ++f)
                    {
                        geometry.inverse_first[derivative][a][d] -=
                            geometry.inverse[a][e] *
                            geometry.first[derivative][e][f] *
                            geometry.inverse[f][d];
                    }
                }
            }
        }
        for (int a = 0; a < dimension; ++a)
        {
            for (int b = 0; b < dimension; ++b)
            {
                for (int c = 0; c < dimension; ++c)
                {
                    for (int d = 0; d < dimension; ++d)
                    {
                        const Real derivative_lower =
                            0.5L *
                            (geometry.second[derivative][b][d][c] +
                             geometry.second[derivative][c][d][b] -
                             geometry.second[derivative][d][b][c]);
                        geometry.gamma_first[derivative][a][b][c] +=
                            geometry.inverse_first[derivative][a][d] *
                                lower[d][b][c] +
                            geometry.inverse[a][d] * derivative_lower;
                    }
                }
            }
        }
    }
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int a = 0; a < dimension; ++a)
            {
                geometry.ricci[i][j] +=
                    geometry.gamma_first[a][a][i][j] -
                    geometry.gamma_first[j][a][i][a];
                for (int b = 0; b < dimension; ++b)
                {
                    geometry.ricci[i][j] +=
                        geometry.gamma[a][i][j] *
                            geometry.gamma[b][a][b] -
                        geometry.gamma[a][j][b] *
                            geometry.gamma[b][i][a];
                }
            }
        }
    }
    return geometry;
}

// Test-only regression path reproducing the former one-pass construction
// defect. The corrected oracle above first fills every lower-index
// Christoffel and only then raises the first index. Here each raised entry is
// formed while later lower-index entries are still zero.
inline Geometry
compute_geometry_old_construction_order(const AxisTensorJets &jets)
{
    Geometry geometry = compute_geometry(jets);
    geometry.gamma = {};
    geometry.ricci = {};
    Christoffel lower = {};
    for (int a = 0; a < dimension; ++a)
    {
        for (int b = 0; b < dimension; ++b)
        {
            for (int c = 0; c < dimension; ++c)
            {
                lower[a][b][c] =
                    0.5L * (geometry.first[b][a][c] +
                            geometry.first[c][a][b] -
                            geometry.first[a][b][c]);
                for (int d = 0; d < dimension; ++d)
                {
                    geometry.gamma[a][b][c] +=
                        geometry.inverse[a][d] * lower[d][b][c];
                }
            }
        }
    }
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int a = 0; a < dimension; ++a)
            {
                geometry.ricci[i][j] +=
                    geometry.gamma_first[a][a][i][j] -
                    geometry.gamma_first[j][a][i][a];
                for (int b = 0; b < dimension; ++b)
                {
                    geometry.ricci[i][j] +=
                        geometry.gamma[a][i][j] *
                            geometry.gamma[b][a][b] -
                        geometry.gamma[a][j][b] *
                            geometry.gamma[b][i][a];
                }
            }
        }
    }
    return geometry;
}

inline Vector4 contracted_connection(const Geometry &geometry)
{
    Vector4 out = {};
    for (int upper = 0; upper < dimension; ++upper)
    {
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                out[upper] += geometry.inverse[i][j] *
                              geometry.gamma[upper][i][j];
            }
        }
    }
    return out;
}

inline Tensor encoded_z_tensor(const Context &context,
                               const Geometry &conformal,
                               const Geometry &physical,
                               Vector4 &z_over_chi_out)
{
    const auto contracted = contracted_connection(conformal);
    std::array<Vector4, dimension> contracted_first = {};
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        for (int upper = 0; upper < dimension; ++upper)
        {
            for (int i = 0; i < dimension; ++i)
            {
                for (int j = 0; j < dimension; ++j)
                {
                    contracted_first[derivative][upper] +=
                        conformal.inverse_first[derivative][i][j] *
                            conformal.gamma[upper][i][j] +
                        conformal.inverse[i][j] *
                            conformal.gamma_first[derivative][upper][i][j];
                }
            }
        }
    }
    const auto hx = state_jet(context, Variable::hat_Gamma_x);
    const auto hz = state_jet(context, Variable::hat_Gamma_z);
    const Vector4 hatted = {hx.value, hz.value, 0.0L, 0.0L};
    std::array<Vector4, dimension> hatted_first = {};
    hatted_first[0] = {hx.dx, hz.dx, 0.0L, 0.0L};
    hatted_first[1] = {hx.dz, hz.dz, 0.0L, 0.0L};
    hatted_first[2][2] = hx.value / context.x;
    hatted_first[3][3] = hx.value / context.x;
    std::array<Vector4, dimension> z_over_chi_first = {};
    Vector4 z_lower = {};
    std::array<Vector4, dimension> z_lower_first = {};
    for (int i = 0; i < dimension; ++i)
    {
        z_over_chi_out[i] = 0.5L * (hatted[i] - contracted[i]);
        for (int derivative = 0; derivative < dimension; ++derivative)
        {
            z_over_chi_first[derivative][i] =
                0.5L * (hatted_first[derivative][i] -
                        contracted_first[derivative][i]);
        }
        for (int j = 0; j < dimension; ++j)
        {
            z_lower[i] += conformal.metric[i][j] * z_over_chi_out[j];
            for (int derivative = 0; derivative < dimension; ++derivative)
            {
                z_lower_first[derivative][i] +=
                    conformal.first[derivative][i][j] * z_over_chi_out[j] +
                    conformal.metric[i][j] *
                        z_over_chi_first[derivative][j];
            }
        }
    }
    Tensor q = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            q[i][j] = z_lower_first[i][j] + z_lower_first[j][i];
            for (int k = 0; k < dimension; ++k)
            {
                q[i][j] -= 2.0L * physical.gamma[k][i][j] * z_lower[k];
            }
        }
    }
    return q;
}

// Independent analytic transcription of the locked Gamma-based CCZ4
// Ricci-Z formula. This consumes only the oracle's analytic target tensors
// and jets; it does not call GRChombo or the production pointwise adapter.
// The older covariant encoded_z_tensor remains useful as a geometric
// cross-check, but finite nonlinear comparison must use the same Gamma-based
// convention as the production source.
inline Tensor analytic_ccz4_ricci_z(const Context &context,
                                    const Geometry &conformal,
                                    Vector4 &z_over_chi_out)
{
    const auto chi = state_jet(context, Variable::chi);
    const auto gamma_x =
        state_jet(context, Variable::hat_Gamma_x);
    const auto gamma_z =
        state_jet(context, Variable::hat_Gamma_z);
    const Vector4 hatted = {gamma_x.value, gamma_z.value, 0.0L, 0.0L};
    std::array<Vector4, dimension> hatted_first = {};
    hatted_first[0] = {gamma_x.dx, gamma_z.dx, 0.0L, 0.0L};
    hatted_first[1] = {gamma_x.dz, gamma_z.dz, 0.0L, 0.0L};
    hatted_first[2][2] = gamma_x.value / context.x;
    hatted_first[3][3] = gamma_x.value / context.x;

    const Vector4 chi_first = {chi.dx, chi.dz, 0.0L, 0.0L};
    Tensor chi_second = {};
    chi_second[0][0] = chi.dxx;
    chi_second[0][1] = chi_second[1][0] = chi.dxz;
    chi_second[1][1] = chi.dzz;
    chi_second[2][2] = chi_second[3][3] = chi.dx / context.x;

    const auto contracted = contracted_connection(conformal);
    for (int i = 0; i < dimension; ++i)
    {
        z_over_chi_out[i] = 0.5L * (hatted[i] - contracted[i]);
    }

    Christoffel lower = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                lower[i][j][k] =
                    0.5L * (conformal.first[j][i][k] +
                            conformal.first[k][i][j] -
                            conformal.first[i][j][k]);
            }
        }
    }

    Christoffel lower_lower_upper = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                for (int l = 0; l < dimension; ++l)
                {
                    lower_lower_upper[i][j][k] +=
                        conformal.inverse[k][l] * lower[i][j][l];
                }
            }
        }
    }

    Tensor covariant_tilde_chi_second = {};
    Real box_tilde_chi = 0.0L;
    Real chi_gradient_squared = 0.0L;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            covariant_tilde_chi_second[i][j] = chi_second[i][j];
            for (int k = 0; k < dimension; ++k)
            {
                covariant_tilde_chi_second[i][j] -=
                    conformal.gamma[k][i][j] * chi_first[k];
            }
            box_tilde_chi +=
                conformal.inverse[i][j] *
                covariant_tilde_chi_second[i][j];
            chi_gradient_squared +=
                conformal.inverse[i][j] * chi_first[i] * chi_first[j];
        }
    }

    Tensor result = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            Real ricci_hat = 0.0L;
            for (int k = 0; k < dimension; ++k)
            {
                ricci_hat +=
                    0.5L *
                    (conformal.metric[k][i] * hatted_first[j][k] +
                     conformal.metric[k][j] * hatted_first[i][k]);
                ricci_hat +=
                    0.5L * hatted[k] * conformal.first[k][i][j];
                for (int l = 0; l < dimension; ++l)
                {
                    ricci_hat +=
                        -0.5L * conformal.inverse[k][l] *
                            conformal.second[k][l][i][j] +
                        conformal.gamma[k][l][i] *
                            lower_lower_upper[j][k][l] +
                        conformal.gamma[k][l][j] *
                            lower_lower_upper[i][k][l] +
                        conformal.gamma[k][i][l] *
                            lower_lower_upper[k][j][l];
                }
            }

            const Real ricci_chi =
                0.5L *
                (2.0L * covariant_tilde_chi_second[i][j] +
                 conformal.metric[i][j] * box_tilde_chi -
                 (2.0L * chi_first[i] * chi_first[j] +
                  4.0L * conformal.metric[i][j] *
                      chi_gradient_squared) /
                     (2.0L * chi.value));
            Real z_terms = 0.0L;
            for (int k = 0; k < dimension; ++k)
            {
                z_terms += z_over_chi_out[k] *
                           (conformal.metric[i][k] * chi_first[j] +
                            conformal.metric[j][k] * chi_first[i] -
                            conformal.metric[i][j] * chi_first[k]);
            }
            result[i][j] =
                (ricci_chi + chi.value * ricci_hat + z_terms) /
                chi.value;
        }
    }
    return result;
}

inline Real trace(const Tensor &tensor, const Tensor &inverse,
                  const int hidden_copies)
{
    return inverse[0][0] * tensor[0][0] +
           2.0L * inverse[0][1] * tensor[0][1] +
           inverse[1][1] * tensor[1][1] +
           static_cast<Real>(hidden_copies) * inverse[2][2] * tensor[2][2];
}

inline Tensor trace_free(const Tensor &tensor, const Tensor &metric,
                         const Tensor &inverse, const int hidden_copies)
{
    Tensor out = tensor;
    const Real tensor_trace = trace(tensor, inverse, hidden_copies);
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] -= 0.25L * metric[i][j] * tensor_trace;
        }
    }
    return out;
}

struct ShiftJets
{
    Vector4 value = {};
    std::array<Vector4, dimension> first = {};
    std::array<std::array<Vector4, dimension>, dimension> second = {};
};

inline ShiftJets shift_jets(const Context &context)
{
    ShiftJets out;
    const Real lambda =
        std::sqrt(context.r0 / (context.x * context.x * context.x));
    out.value[0] = lambda * context.x;
    out.first[0][0] = -0.5L * lambda;
    out.first[2][2] = lambda;
    out.first[3][3] = lambda;
    out.second[0][0][0] = 0.75L * lambda / context.x;
    for (const int hidden : {2, 3})
    {
        out.second[hidden][hidden][0] = -1.5L * lambda / context.x;
        out.second[0][hidden][hidden] = -1.5L * lambda / context.x;
        out.second[hidden][0][hidden] = -1.5L * lambda / context.x;
    }
    return out;
}

struct CorrectedEvaluationPolicy
{
    Geometry geometry_from(const AxisTensorJets &jets) const
    {
        return compute_geometry(jets);
    }

    ShiftJets shift_from(const Context &context) const
    {
        return shift_jets(context);
    }

    Real contracted_vector_hessian(const ShiftJets &shift,
                                   const int derivative_1,
                                   const int derivative_2,
                                   const int component) const
    {
        return shift.second[derivative_1][derivative_2][component];
    }
};

inline void add_representative_tensor(Rows &rows, const Tensor &tensor,
                                      const Variable xx,
                                      const Variable xz,
                                      const Variable zz,
                                      const Variable ww,
                                      const Real factor = 1.0L)
{
    rows[slot(xx)] += factor * tensor[0][0];
    rows[slot(xz)] += factor * tensor[0][1];
    rows[slot(zz)] += factor * tensor[1][1];
    rows[slot(ww)] += factor * tensor[2][2];
}

template <class evaluation_policy_t>
inline Result evaluate_with_policy(
    const double r0, const double x, const double epsilon,
    const Direction &direction, const evaluation_policy_t &evaluation_policy,
    const int hidden_copies = hidden_multiplicity)
{
    const Context context{static_cast<Real>(r0), static_cast<Real>(x),
                          static_cast<Real>(epsilon), &direction,
                          hidden_copies};
    const auto conformal_jets = conformal_metric_jets(context);
    const auto physical_jets = physical_metric_jets(context);
    const auto a_jets = a_tensor_jets(context);
    const auto conformal =
        evaluation_policy.geometry_from(conformal_jets);
    const auto physical =
        evaluation_policy.geometry_from(physical_jets);
    const auto shift = evaluation_policy.shift_from(context);
    Vector4 z_over_chi = {};
    const auto combined_ricci =
        analytic_ccz4_ricci_z(context, conformal, z_over_chi);
    Tensor q = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            q[i][j] = combined_ricci[i][j] - physical.ricci[i][j];
        }
    }
    const auto &a = a_jets.value;
    const Real chi = state_jet(context, Variable::chi).value;
    const Real k = state_jet(context, Variable::K).value;
    const Real theta = state_jet(context, Variable::Theta).value;
    Real div_shift = shift.first[0][0] + shift.first[1][1] +
                     static_cast<Real>(hidden_copies) * shift.first[2][2];
    Result result;

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        result.family[common_advection][slot(variable)] =
            shift.value[0] * state_jet(context, variable).dx;
    }

    result.family[chi_metric_algebraic][slot(Variable::chi)] =
        0.5L * chi * (k - div_shift);
    Tensor metric_algebraic = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            metric_algebraic[i][j] = -2.0L * a[i][j];
        }
    }
    add_representative_tensor(result.family[chi_metric_algebraic],
                              metric_algebraic, Variable::h_xx,
                              Variable::h_xz, Variable::h_zz, Variable::h_ww);

    Tensor metric_shift = {};
    Tensor a_shift = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            metric_shift[i][j] = -0.5L * div_shift * conformal.metric[i][j];
            a_shift[i][j] = -0.5L * div_shift * a[i][j];
            for (int m = 0; m < dimension; ++m)
            {
                metric_shift[i][j] +=
                    conformal.metric[m][i] * shift.first[m][j] +
                    conformal.metric[m][j] * shift.first[m][i];
                a_shift[i][j] += a[m][i] * shift.first[m][j] +
                                 a[m][j] * shift.first[m][i];
            }
        }
    }
    add_representative_tensor(result.family[metric_shift_stretching],
                              metric_shift, Variable::h_xx, Variable::h_xz,
                              Variable::h_zz, Variable::h_ww);
    add_representative_tensor(result.family[a_shift_stretching], a_shift,
                              Variable::A_xx, Variable::A_xz, Variable::A_zz,
                              Variable::A_ww);

    const Real geometric_scalar =
        chi * trace(physical.ricci, conformal.inverse, hidden_copies);
    auto geometric_tf = trace_free(physical.ricci, conformal.metric,
                                   conformal.inverse, hidden_copies);
    for (auto &row : geometric_tf)
    {
        for (auto &value : row)
        {
            value *= chi;
        }
    }
    result.family[geometric_ricci][slot(Variable::K)] = geometric_scalar;
    result.family[geometric_ricci][slot(Variable::Theta)] =
        0.5L * geometric_scalar;
    add_representative_tensor(result.family[geometric_ricci], geometric_tf,
                              Variable::A_xx, Variable::A_xz, Variable::A_zz,
                              Variable::A_ww);

    const Real q_scalar = chi * trace(q, conformal.inverse, hidden_copies);
    auto q_tf = trace_free(q, conformal.metric, conformal.inverse,
                           hidden_copies);
    for (auto &row : q_tf)
    {
        for (auto &value : row)
        {
            value *= chi;
        }
    }
    result.family[encoded_z][slot(Variable::K)] = q_scalar;
    result.family[encoded_z][slot(Variable::Theta)] = 0.5L * q_scalar;
    add_representative_tensor(result.family[encoded_z], q_tf,
                              Variable::A_xx, Variable::A_xz, Variable::A_zz,
                              Variable::A_ww);

    Tensor a_uu = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int m = 0; m < dimension; ++m)
            {
                for (int n = 0; n < dimension; ++n)
                {
                    a_uu[i][j] += conformal.inverse[i][m] *
                                  conformal.inverse[j][n] * a[m][n];
                }
            }
        }
    }
    const Real tr_a2 = trace(a, a_uu, hidden_copies);
    result.family[k_theta_a_algebraic][slot(Variable::K)] =
        k * (k - 2.0L * theta);
    result.family[k_theta_a_algebraic][slot(Variable::Theta)] =
        0.5L * (-tr_a2 + 0.75L * k * k - 2.0L * theta * k);
    Tensor algebraic_a = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            algebraic_a[i][j] = a[i][j] * (k - 2.0L * theta);
            for (int m = 0; m < dimension; ++m)
            {
                for (int n = 0; n < dimension; ++n)
                {
                    algebraic_a[i][j] -=
                        2.0L * conformal.inverse[m][n] * a[i][m] * a[n][j];
                }
            }
        }
    }
    add_representative_tensor(result.family[k_theta_a_algebraic], algebraic_a,
                              Variable::A_xx, Variable::A_xz, Variable::A_zz,
                              Variable::A_ww);
    result.family[locked_damping][slot(Variable::K)] = -0.4L * theta;
    result.family[locked_damping][slot(Variable::Theta)] = -0.25L * theta;

    Rows &gamma_rows = result.family[gamma_non_advection];
    for (int i = 0; i < 2; ++i)
    {
        Real gamma_rhs =
            0.5L * (div_shift * (contracted_connection(conformal)[i] +
                                 2.0L * z_over_chi[i]) -
                    2.0L * k * z_over_chi[i]) -
            0.2L * z_over_chi[i];
        for (int j = 0; j < dimension; ++j)
        {
            const Real d_theta = j == 0 ? state_jet(context, Variable::Theta).dx
                                        : j == 1 ? state_jet(context, Variable::Theta).dz
                                                 : 0.0L;
            const Real d_k = j == 0 ? state_jet(context, Variable::K).dx
                                    : j == 1 ? state_jet(context, Variable::K).dz
                                             : 0.0L;
            const Real d_chi = j == 0 ? state_jet(context, Variable::chi).dx
                                      : j == 1 ? state_jet(context, Variable::chi).dz
                                               : 0.0L;
            gamma_rhs += 2.0L * conformal.inverse[i][j] * d_theta -
                         1.5L * conformal.inverse[i][j] * d_k -
                         4.0L * a_uu[i][j] * d_chi / chi;
            gamma_rhs -=
                (contracted_connection(conformal)[j] +
                 2.0L * z_over_chi[j]) * shift.first[i][j];
            for (int m = 0; m < dimension; ++m)
            {
                gamma_rhs +=
                    2.0L * conformal.gamma[i][j][m] * a_uu[j][m];
                gamma_rhs +=
                    conformal.inverse[j][m] *
                    evaluation_policy.contracted_vector_hessian(
                        shift, j, m, i);
                gamma_rhs += 0.5L * conformal.inverse[i][j] *
                             shift.second[m][j][m];
            }
        }
        gamma_rows[slot(i == 0 ? Variable::hat_Gamma_x
                               : Variable::hat_Gamma_z)] = gamma_rhs;
    }

    for (const auto &family : result.family)
    {
        for (std::size_t i = 0; i < variables; ++i)
        {
            result.total[i] += family[i];
        }
    }
    return result;
}

inline Result evaluate(const double r0, const double x, const double epsilon,
                       const Direction &direction,
                       const int hidden_copies = hidden_multiplicity)
{
    return evaluate_with_policy(r0, x, epsilon, direction,
                                CorrectedEvaluationPolicy{},
                                hidden_copies);
}

inline Result central_difference(const double r0, const double x,
                                 const double epsilon,
                                 const Direction &direction,
                                 const int hidden_copies = hidden_multiplicity)
{
    const auto plus = evaluate(r0, x, epsilon, direction, hidden_copies);
    const auto minus = evaluate(r0, x, -epsilon, direction, hidden_copies);
    Result out;
    for (std::size_t family = 0; family < family_count; ++family)
    {
        for (std::size_t i = 0; i < variables; ++i)
        {
            out.family[family][i] =
                (plus.family[family][i] - minus.family[family][i]) /
                (2.0L * static_cast<Real>(epsilon));
            out.total[i] += out.family[family][i];
        }
    }
    return out;
}
} // namespace Stage4AOCAnalyticFullOracle
} // namespace BlackStringToy

#endif
