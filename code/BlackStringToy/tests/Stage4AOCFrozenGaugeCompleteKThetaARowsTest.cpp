#include "Stage4AOFrozenGaugeOperator.hpp"
#include "Stage4AOFrozenGaugeZDerivativeAdapter.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
namespace Adapter =
    BlackStringToy::Stage4AOFrozenGaugeZDerivativeAdapter;
namespace Ricci = BlackStringToy::CartoonRicci;
namespace RicciAssembly =
    BlackStringToy::Stage4AOFrozenGaugeRicciAssembly;
namespace HiddenRww = BlackStringToy::Stage4AOFrozenGaugeHiddenRww;
namespace VisibleRxx = BlackStringToy::Stage4AOFrozenGaugeVisibleRxx;
namespace VisibleRxz = BlackStringToy::Stage4AOFrozenGaugeVisibleRxz;
namespace VisibleRzz = BlackStringToy::Stage4AOFrozenGaugeVisibleRzz;

using Variable = Operator::PerturbationVariable;
using Vector = Operator::FrozenGaugePerturbationVector;
using Real = long double;

constexpr int dimension = 4;
constexpr int hidden_multiplicity = 2;
constexpr double linear_tolerance = 3.0e-6;

enum Family : std::size_t
{
    advection,
    shift_stretching,
    geometric_ricci,
    encoded_z,
    algebraic,
    damping,
    family_count
};

constexpr std::array<const char *, family_count> family_names = {
    "common advection", "tensor shift stretching", "geometric Ricci",
    "encoded Z", "selected algebraic", "locked damping"};

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
    std::array<Jet, Operator::frozen_gauge_state_vector.size()> u = {};

    Jet &at(const Variable variable)
    {
        return u[Operator::variable_index(variable)];
    }
    const Jet &at(const Variable variable) const
    {
        return u[Operator::variable_index(variable)];
    }
};

using Tensor = std::array<std::array<Real, dimension>, dimension>;
using Vector4 = std::array<Real, dimension>;
using DerivativeTensor =
    std::array<std::array<std::array<Real, dimension>, dimension>, dimension>;
using SecondDerivativeTensor = std::array<DerivativeTensor, dimension>;
using Christoffel = std::array<Tensor, dimension>;

struct Rows
{
    std::array<double, 6> value = {};
};

struct NonlinearFamilies
{
    std::array<Rows, family_count> family = {};
    Rows total;
};

struct Context
{
    Real r0;
    Real x0;
    Real z0;
    Real epsilon;
    const Direction *direction;
    int hidden_copies = hidden_multiplicity;
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
                   const double expected, const double tolerance)
{
    if (std::abs(actual - expected) > tolerance)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " tolerance=" + std::to_string(tolerance));
    }
}

double max_error(const Rows &left, const Rows &right)
{
    double error = 0.0;
    for (std::size_t i = 0; i < left.value.size(); ++i)
    {
        error = std::max(error, std::abs(left.value[i] - right.value[i]));
    }
    return error;
}

Rows add(const Rows &left, const Rows &right)
{
    Rows out;
    for (std::size_t i = 0; i < out.value.size(); ++i)
    {
        out.value[i] = left.value[i] + right.value[i];
    }
    return out;
}

Rows subtract(const Rows &left, const Rows &right)
{
    Rows out;
    for (std::size_t i = 0; i < out.value.size(); ++i)
    {
        out.value[i] = left.value[i] - right.value[i];
    }
    return out;
}

Rows scale(const Rows &input, const double factor)
{
    Rows out;
    for (std::size_t i = 0; i < out.value.size(); ++i)
    {
        out.value[i] = factor * input.value[i];
    }
    return out;
}

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

AnalyticJet state_jet(const Context &context, const Variable variable)
{
    const auto &perturbation = context.direction->at(variable);
    AnalyticJet out{context.epsilon * perturbation.value,
                    context.epsilon * perturbation.dx,
                    context.epsilon * perturbation.dz,
                    context.epsilon * perturbation.dxx,
                    context.epsilon * perturbation.dxz,
                    context.epsilon * perturbation.dzz};
    const Real lambda =
        std::sqrt(context.r0 / (context.x0 * context.x0 * context.x0));
    const Real lambda_x = -1.5L * lambda / context.x0;
    const Real lambda_xx = 3.75L * lambda / (context.x0 * context.x0);
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

AnalyticJet reciprocal(const AnalyticJet &input)
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

AnalyticJet multiply(const AnalyticJet &left, const AnalyticJet &right)
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

AxisTensorJets lift_axis_tensor(const Real x, const AnalyticJet &xx,
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

AxisTensorJets conformal_metric_jets(const Context &context)
{
    return lift_axis_tensor(
        context.x0, state_jet(context, Variable::h_xx),
        state_jet(context, Variable::h_xz),
        state_jet(context, Variable::h_zz),
        state_jet(context, Variable::h_ww));
}

AxisTensorJets physical_metric_jets(const Context &context)
{
    const auto chi_inverse = reciprocal(state_jet(context, Variable::chi));
    return lift_axis_tensor(
        context.x0,
        multiply(state_jet(context, Variable::h_xx), chi_inverse),
        multiply(state_jet(context, Variable::h_xz), chi_inverse),
        multiply(state_jet(context, Variable::h_zz), chi_inverse),
        multiply(state_jet(context, Variable::h_ww), chi_inverse));
}

AxisTensorJets a_tensor_jets(const Context &context)
{
    return lift_axis_tensor(
        context.x0, state_jet(context, Variable::A_xx),
        state_jet(context, Variable::A_xz),
        state_jet(context, Variable::A_zz),
        state_jet(context, Variable::A_ww));
}

Tensor invert(const Tensor &input)
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
            fail("nonlinear oracle inverse", "singular metric");
        }
        std::swap(work[pivot], work[column]);
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

Geometry compute_geometry(const AxisTensorJets &jets)
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

Tensor encoded_z_tensor(const Context &context,
                        const Geometry &conformal_geometry,
                        const Geometry &physical_geometry)
{
    Vector4 contracted = {};
    std::array<Vector4, dimension> contracted_first = {};
    for (int upper = 0; upper < dimension; ++upper)
    {
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                contracted[upper] += conformal_geometry.inverse[i][j] *
                                     conformal_geometry.gamma[upper][i][j];
                for (int derivative = 0; derivative < dimension; ++derivative)
                {
                    contracted_first[derivative][upper] +=
                        conformal_geometry.inverse_first[derivative][i][j] *
                            conformal_geometry.gamma[upper][i][j] +
                        conformal_geometry.inverse[i][j] *
                            conformal_geometry.gamma_first[derivative][upper][i]
                                                            [j];
                }
            }
        }
    }

    const auto hx = state_jet(context, Variable::hat_Gamma_x);
    const auto hz = state_jet(context, Variable::hat_Gamma_z);
    Vector4 hatted = {hx.value, hz.value, 0.0L, 0.0L};
    std::array<Vector4, dimension> hatted_first = {};
    hatted_first[0] = {hx.dx, hz.dx, 0.0L, 0.0L};
    hatted_first[1] = {hx.dz, hz.dz, 0.0L, 0.0L};
    hatted_first[2][2] = hx.value / context.x0;
    hatted_first[3][3] = hx.value / context.x0;

    Vector4 z_over_chi = {};
    std::array<Vector4, dimension> z_over_chi_first = {};
    Vector4 z_lower = {};
    std::array<Vector4, dimension> z_lower_first = {};
    for (int i = 0; i < dimension; ++i)
    {
        z_over_chi[i] = 0.5L * (hatted[i] - contracted[i]);
        for (int derivative = 0; derivative < dimension; ++derivative)
        {
            z_over_chi_first[derivative][i] =
                0.5L * (hatted_first[derivative][i] -
                        contracted_first[derivative][i]);
        }
        for (int j = 0; j < dimension; ++j)
        {
            z_lower[i] += conformal_geometry.metric[i][j] * z_over_chi[j];
            for (int derivative = 0; derivative < dimension; ++derivative)
            {
                z_lower_first[derivative][i] +=
                    conformal_geometry.first[derivative][i][j] *
                        z_over_chi[j] +
                    conformal_geometry.metric[i][j] *
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
                q[i][j] -= 2.0L * physical_geometry.gamma[k][i][j] *
                           z_lower[k];
            }
        }
    }
    return q;
}

Real weighted_axis_trace(const Tensor &tensor, const Tensor &inverse,
                         const int hidden_copies)
{
    return inverse[0][0] * tensor[0][0] +
           2.0L * inverse[0][1] * tensor[0][1] +
           inverse[1][1] * tensor[1][1] +
           static_cast<Real>(hidden_copies) * inverse[2][2] * tensor[2][2];
}

Tensor weighted_axis_trace_free(const Tensor &tensor, const Tensor &metric,
                                const Tensor &inverse,
                                const int hidden_copies)
{
    Tensor out = tensor;
    const Real tensor_trace =
        weighted_axis_trace(tensor, inverse, hidden_copies);
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] -= metric[i][j] * tensor_trace / 4.0L;
        }
    }
    return out;
}

Vector4 shift_at_axis(const Context &context)
{
    return {std::sqrt(context.r0 / context.x0), 0.0L, 0.0L, 0.0L};
}

DerivativeTensor shift_derivatives_at_axis(const Context &context)
{
    DerivativeTensor derivative = {};
    const Real lambda =
        std::sqrt(context.r0 / (context.x0 * context.x0 * context.x0));
    derivative[0][0][0] = -0.5L * lambda;
    derivative[2][2][0] = lambda;
    derivative[3][3][0] = lambda;
    return derivative;
}

Real scalar_advection(const Context &context, const Variable variable)
{
    return shift_at_axis(context)[0] * state_jet(context, variable).dx;
}

Tensor tensor_advection(const Context &context, const AxisTensorJets &a)
{
    Tensor out = {};
    const Real beta = shift_at_axis(context)[0];
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = beta * a.first[0][i][j];
        }
    }
    return out;
}

Rows rows_from_tensor(const Real k, const Real theta, const Tensor &a)
{
    return {{static_cast<double>(k), static_cast<double>(theta),
             static_cast<double>(a[0][0]), static_cast<double>(a[0][1]),
             static_cast<double>(a[1][1]), static_cast<double>(a[2][2])}};
}

NonlinearFamilies evaluate_nonlinear_families(
    const double r0, const double x, const double z, const double epsilon,
    const Direction &direction,
    const int hidden_copies = hidden_multiplicity)
{
    const Context context{static_cast<Real>(r0), static_cast<Real>(x),
                          static_cast<Real>(z), static_cast<Real>(epsilon),
                          &direction, hidden_copies};
    const auto conformal_jets = conformal_metric_jets(context);
    const auto physical_jets = physical_metric_jets(context);
    const auto a_jets = a_tensor_jets(context);
    const auto conformal = compute_geometry(conformal_jets);
    const auto physical = compute_geometry(physical_jets);
    const auto q = encoded_z_tensor(context, conformal, physical);
    const auto &a = a_jets.value;
    const auto beta_derivative = shift_derivatives_at_axis(context);

    Real div_shift = beta_derivative[0][0][0] +
                     beta_derivative[1][1][0] +
                     static_cast<Real>(hidden_copies) *
                         beta_derivative[2][2][0];
    const Real chi = state_jet(context, Variable::chi).value;
    const Real k = state_jet(context, Variable::K).value;
    const Real theta = state_jet(context, Variable::Theta).value;

    NonlinearFamilies result;
    Tensor zero = {};
    const auto adv_a = tensor_advection(context, a_jets);
    result.family[advection] =
        rows_from_tensor(scalar_advection(context, Variable::K),
                         scalar_advection(context, Variable::Theta),
                         adv_a);

    Tensor shift_a = {};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            shift_a[i][j] = -0.5L * div_shift * a[i][j];
            for (int m = 0; m < dimension; ++m)
            {
                shift_a[i][j] +=
                    a[m][i] * beta_derivative[j][m][0] +
                    a[m][j] * beta_derivative[i][m][0];
            }
        }
    }
    result.family[shift_stretching] = rows_from_tensor(0.0L, 0.0L, shift_a);

    const Real geometric_scalar =
        chi * weighted_axis_trace(physical.ricci, conformal.inverse,
                                  hidden_copies);
    const auto geometric_tf = weighted_axis_trace_free(
        physical.ricci, conformal.metric, conformal.inverse, hidden_copies);
    Tensor geometric_a = geometric_tf;
    for (auto &row : geometric_a)
    {
        for (auto &value : row)
        {
            value *= chi;
        }
    }
    result.family[geometric_ricci] = rows_from_tensor(
        geometric_scalar, 0.5L * geometric_scalar, geometric_a);

    const Real q_scalar =
        chi * weighted_axis_trace(q, conformal.inverse, hidden_copies);
    const auto q_tf = weighted_axis_trace_free(
        q, conformal.metric, conformal.inverse, hidden_copies);
    Tensor q_a = q_tf;
    for (auto &row : q_a)
    {
        for (auto &value : row)
        {
            value *= chi;
        }
    }
    result.family[encoded_z] =
        rows_from_tensor(q_scalar, 0.5L * q_scalar, q_a);

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
    const Real tr_a2 =
        weighted_axis_trace(a, a_uu, hidden_copies);
    const Real k_algebraic = k * (k - 2.0L * theta);
    const Real theta_algebraic =
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
    result.family[algebraic] =
        rows_from_tensor(k_algebraic, theta_algebraic, algebraic_a);

    result.family[damping] = rows_from_tensor(
        -0.4L * theta, -0.25L * theta, zero);

    for (const auto &family : result.family)
    {
        result.total = add(result.total, family);
    }
    return result;
}

NonlinearFamilies central_difference(
    const double r0, const double x, const double z, const double epsilon,
    const Direction &direction,
    const int hidden_copies = hidden_multiplicity)
{
    const auto plus = evaluate_nonlinear_families(
        r0, x, z, epsilon, direction, hidden_copies);
    const auto minus = evaluate_nonlinear_families(
        r0, x, z, -epsilon, direction, hidden_copies);
    NonlinearFamilies result;
    for (std::size_t family = 0; family < family_count; ++family)
    {
        result.family[family] = scale(
            subtract(plus.family[family], minus.family[family]),
            1.0 / (2.0 * epsilon));
        result.total = add(result.total, result.family[family]);
    }
    return result;
}

BlackStringToy::ConformalCartoonAlgebra::ConformalMetric
metric_from_direction(const Direction &direction)
{
    return {direction.at(Variable::h_xx).value,
            direction.at(Variable::h_xz).value,
            direction.at(Variable::h_zz).value,
            direction.at(Variable::h_ww).value};
}

BlackStringToy::ConformalCartoonAlgebra::ConformalMetric metric_jet(
    const Direction &direction, const int derivative)
{
    auto value = [derivative](const Jet &jet) {
        switch (derivative)
        {
        case 0:
            return jet.dx;
        case 1:
            return jet.dz;
        case 2:
            return jet.dxx;
        case 3:
            return jet.dxz;
        default:
            return jet.dzz;
        }
    };
    return {value(direction.at(Variable::h_xx)),
            value(direction.at(Variable::h_xz)),
            value(direction.at(Variable::h_zz)),
            value(direction.at(Variable::h_ww))};
}

Ricci::MetricDerivatives metric_derivatives(const Direction &direction)
{
    return {metric_jet(direction, 0), metric_jet(direction, 1),
            metric_jet(direction, 2), metric_jet(direction, 3),
            metric_jet(direction, 4)};
}

Ricci::ScalarDerivatives chi_derivatives(const Direction &direction)
{
    const auto &chi = direction.at(Variable::chi);
    return {{chi.dx, chi.dz}, {chi.dxx, chi.dxz, chi.dzz}};
}

RicciAssembly::TraceFreeRicciAssembly
make_geometric_ricci(const double x, const Direction &direction)
{
    const auto h = metric_from_direction(direction);
    const auto derivatives = metric_derivatives(direction);
    const auto chi = direction.at(Variable::chi).value;
    const auto dchi = chi_derivatives(direction);
    const auto rxx = VisibleRxx::compute_visible_physical_delta_rxx(
        VisibleRxx::make_visible_rxx_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rxz = VisibleRxz::compute_visible_physical_delta_rxz(
        VisibleRxz::make_visible_rxz_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rzz = VisibleRzz::compute_visible_physical_delta_rzz(
        VisibleRzz::make_visible_rzz_perturbation(x, h, derivatives, chi,
                                                  dchi));
    const auto rww = HiddenRww::compute_hidden_physical_delta_rww(
        HiddenRww::make_hidden_rww_perturbation(x, h, derivatives, chi,
                                                dchi));
    return RicciAssembly::assemble_trace_free_ricci(
        RicciAssembly::make_raw_ricci_components(rxx, rxz, rzz, rww));
}

Adapter::DerivativeAdapterResult make_adapter(const double x,
                                              const Direction &direction)
{
    const auto &hx = direction.at(Variable::hat_Gamma_x);
    const auto &hz = direction.at(Variable::hat_Gamma_z);
    return Adapter::compute_derivative_adapter(
        Adapter::make_derivative_adapter_input(
            x, metric_from_direction(direction), metric_derivatives(direction),
            hx.value, hz.value, hx.dx, hx.dz, hz.dx, hz.dz));
}

Vector make_state(const Direction &direction)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] =
            direction.at(variable).value;
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Vector make_advection(const double r0, const double x,
                      const Direction &direction)
{
    std::array<double, Operator::frozen_gauge_state_vector.size()> values = {};
    const double beta = std::sqrt(r0 / x);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        values[Operator::variable_index(variable)] =
            beta * direction.at(variable).dx;
    }
    return Operator::make_frozen_gauge_perturbation_vector(values);
}

Rows rows_from_vector(const Vector &vector)
{
    return {{{vector.value(Variable::K), vector.value(Variable::Theta),
              vector.value(Variable::A_xx), vector.value(Variable::A_xz),
              vector.value(Variable::A_zz),
              vector.value(Variable::A_ww)}}};
}

std::array<Rows, family_count> production_families(
    const double r0, const double x, const Direction &direction)
{
    const auto state = make_state(direction);
    const auto advection_output = make_advection(r0, x, direction);
    const auto geometric = make_geometric_ricci(x, direction);
    const auto adapter = make_adapter(x, direction);
    const auto completion =
        Adapter::make_encoded_z_ricci_completion(x, adapter);

    std::array<Rows, family_count> result = {};
    result[advection] = rows_from_vector(advection_output);
    result[shift_stretching] = rows_from_vector(
        Operator::apply_tensor_shift_stretching_at_point(r0, x, state));

    const auto k_geometric =
        Operator::apply_k_equation_ricci_scalar_insertion_at_point(geometric);
    const auto theta_geometric =
        Operator::apply_theta_ricci_scalar_insertion_at_point(geometric);
    const auto a_geometric =
        Operator::apply_a_equation_ricci_curvature_insertion_at_point(
            geometric);
    result[geometric_ricci] =
        {{k_geometric.value(Variable::K),
          theta_geometric.value(Variable::Theta),
          a_geometric.value(Variable::A_xx),
          a_geometric.value(Variable::A_xz),
          a_geometric.value(Variable::A_zz),
          a_geometric.value(Variable::A_ww)}};

    const auto k_z =
        Operator::apply_k_equation_encoded_z_ricci_insertion_at_point(
            completion);
    const auto theta_z =
        Operator::apply_theta_equation_encoded_z_ricci_insertion_at_point(
            completion);
    const auto a_z =
        Operator::apply_a_equation_encoded_z_ricci_insertion_at_point(
            completion);
    result[encoded_z] =
        {{k_z.value(Variable::K), theta_z.value(Variable::Theta),
          a_z.value(Variable::A_xx), a_z.value(Variable::A_xz),
          a_z.value(Variable::A_zz), a_z.value(Variable::A_ww)}};

    const auto k_algebraic =
        Operator::apply_k_equation_ccz4_k_theta_at_point(r0, x, state);
    const auto theta_algebraic =
        Operator::apply_theta_equation_algebraic_non_ricci_at_point(r0, x,
                                                                    state);
    const auto theta_minus_k =
        Operator::apply_theta_equation_minus_k_delta_theta_at_point(r0, x,
                                                                    state);
    const auto a_algebraic =
        Operator::apply_a_equation_algebraic_non_curvature_at_point(r0, x,
                                                                    state);
    result[algebraic] =
        {{k_algebraic.value(Variable::K),
          theta_algebraic.value(Variable::Theta) +
              theta_minus_k.value(Variable::Theta),
          a_algebraic.value(Variable::A_xx),
          a_algebraic.value(Variable::A_xz),
          a_algebraic.value(Variable::A_zz),
          a_algebraic.value(Variable::A_ww)}};
    result[damping] = rows_from_vector(
        Operator::apply_ccz4_k_theta_damping_insertion_at_point(state));
    return result;
}

Rows assemble(const double r0, const double x, const Direction &direction,
              Vector *full_output = nullptr)
{
    const auto state = make_state(direction);
    const auto geometric = make_geometric_ricci(x, direction);
    const auto adapter = make_adapter(x, direction);
    const auto completion =
        Adapter::make_encoded_z_ricci_completion(x, adapter);
    const auto output = Operator::apply_complete_k_theta_a_rows_at_point(
        r0, x, state, make_advection(r0, x, direction), geometric,
        completion);
    if (full_output != nullptr)
    {
        *full_output = output;
    }
    return rows_from_vector(output);
}

Direction mixed_direction()
{
    Direction d;
    d.at(Variable::chi) = {0.13, 0.21, -0.16, -0.08, 0.12, 0.17};
    d.at(Variable::h_xx) = {0.31, -0.41, 0.23, 0.37, -0.19, 0.29};
    d.at(Variable::h_xz) = {-0.22, 0.19, -0.29, -0.33, 0.27, -0.24};
    d.at(Variable::h_zz) = {-0.17, 0.37, -0.11, 0.28, -0.31, 0.35};
    d.at(Variable::h_ww) = {0.28, -0.13, 0.07, -0.26, 0.18, -0.32};
    d.at(Variable::K) = {-0.27, 0.34, -0.29, 0.0, 0.0, 0.0};
    d.at(Variable::A_xx) = {0.11, -0.17, 0.13, 0.0, 0.0, 0.0};
    d.at(Variable::A_xz) = {-0.09, 0.14, -0.12, 0.0, 0.0, 0.0};
    d.at(Variable::A_zz) = {0.07, 0.16, -0.10, 0.0, 0.0, 0.0};
    d.at(Variable::A_ww) = {-0.05, -0.11, 0.08, 0.0, 0.0, 0.0};
    d.at(Variable::Theta) = {0.18, -0.26, 0.24, 0.0, 0.0, 0.0};
    d.at(Variable::hat_Gamma_x) = {0.23, 0.32, -0.21, -0.15, 0.24, 0.19};
    d.at(Variable::hat_Gamma_z) = {-0.19, -0.28, 0.26, 0.22, -0.17, -0.25};
    return d;
}

Direction second_mixed_direction()
{
    auto d = mixed_direction();
    for (std::size_t i = 0; i < d.u.size(); ++i)
    {
        const double sign = i % 2 == 0 ? -1.0 : 1.0;
        d.u[i].value *= sign * 0.83;
        d.u[i].dx *= -sign * 0.91;
        d.u[i].dz *= sign * 1.07;
        d.u[i].dxx *= -sign * 0.79;
        d.u[i].dxz *= sign * 0.88;
        d.u[i].dzz *= -sign * 0.94;
    }
    return d;
}

Direction reflected_direction(const Direction &positive)
{
    Direction negative;
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        const bool one_z = variable == Variable::h_xz ||
                           variable == Variable::A_xz ||
                           variable == Variable::hat_Gamma_z;
        const double parity = one_z ? -1.0 : 1.0;
        const auto &source = positive.at(variable);
        auto &target = negative.at(variable);
        target = {parity * source.value, parity * source.dx,
                  -parity * source.dz, parity * source.dxx,
                  -parity * source.dxz, parity * source.dzz};
    }
    return negative;
}

void check_background_and_zero()
{
    const Direction zero;
    const auto background =
        evaluate_nonlinear_families(1.0, 2.0, 0.3, 0.0, zero);
    std::cout << std::scientific << std::setprecision(12)
              << "INFO K/Theta/A GP residual K=" << background.total.value[0]
              << " Theta=" << background.total.value[1]
              << " Axx=" << background.total.value[2]
              << " Axz=" << background.total.value[3]
              << " Azz=" << background.total.value[4]
              << " Aww=" << background.total.value[5] << "\n";
    require_true("analytic-jet GP background residuals near roundoff",
                 max_error(background.total, Rows{}) < 2.0e-17);
    require_true("zero perturbation complete rows",
                 max_error(assemble(1.0, 2.0, zero), Rows{}) == 0.0);
}

void check_family_oracles_and_mutations()
{
    const double r0 = 1.0;
    const double x = 1.9;
    const double z = 0.37;
    const auto direction = mixed_direction();
    const auto oracle = central_difference(r0, x, z, 1.0e-5, direction);
    const auto production = production_families(r0, x, direction);
    const auto actual = assemble(r0, x, direction);

    for (std::size_t family = 0; family < family_count; ++family)
    {
        const double error = max_error(production[family],
                                       oracle.family[family]);
        std::cout << "INFO family=" << family_names[family]
                  << " max_error=" << error << "\n";
        require_true(std::string("independent family oracle ") +
                         family_names[family],
                     error < linear_tolerance);
        double amplitude = 0.0;
        for (const double value : oracle.family[family].value)
        {
            amplitude = std::max(amplitude, std::abs(value));
        }
        require_true(std::string("family activated ") + family_names[family],
                     amplitude > 1.0e-4);
        require_true(std::string("omission mutation ") + family_names[family],
                     max_error(actual,
                               subtract(actual, oracle.family[family])) >
                         1.0e-4);
        require_true(std::string("duplication mutation ") +
                         family_names[family],
                     max_error(actual, add(actual, oracle.family[family])) >
                         1.0e-4);
    }
    require_true("assembled rows equal independent total",
                 max_error(actual, oracle.total) < 3.0e-6);

    const auto hidden_one =
        central_difference(r0, x, z, 1.0e-5, direction, 1).total;
    require_true("wrong hidden multiplicity mutation",
                 max_error(actual, hidden_one) > 1.0e-4);
    Rows doubled_ww = actual;
    doubled_ww.value[5] += oracle.family[encoded_z].value[5];
    require_true("representative ww duplication mutation",
                 max_error(actual, doubled_ww) > 1.0e-4);
    require_true("encoded-Z omission mutation",
                 max_error(actual,
                           subtract(actual, oracle.family[encoded_z])) >
                     1.0e-4);
    require_true("encoded-Z double insertion mutation",
                 max_error(actual, add(actual, oracle.family[encoded_z])) >
                     1.0e-4);
}

void check_epsilon_tables()
{
    const double r0 = 0.9;
    const double x = 1.8;
    const double z = 0.41;
    const std::array<double, 6> epsilons = {
        1.0e-2, 1.0e-3, 1.0e-4, 1.0e-5, 1.0e-6, 1.0e-7};
    const std::array<const char *, 2> direction_names = {
        "mixed direction 1", "mixed direction 2"};
    const std::array<Direction, 2> directions = {
        mixed_direction(), second_mixed_direction()};
    for (std::size_t direction_index = 0;
         direction_index < directions.size(); ++direction_index)
    {
        const auto &direction = directions[direction_index];
        const auto actual = assemble(r0, x, direction);
        std::array<Rows, 6> estimates = {};
        std::array<double, 6> errors = {};
        std::cout << "INFO K/Theta/A epsilon table "
                  << direction_names[direction_index] << "\n";
        for (std::size_t i = 0; i < epsilons.size(); ++i)
        {
            estimates[i] =
                central_difference(r0, x, z, epsilons[i], direction).total;
            errors[i] = max_error(actual, estimates[i]);
            std::cout << std::scientific << std::setprecision(12)
                      << "INFO eps=" << epsilons[i]
                      << " err_max=" << errors[i]
                      << " err_K="
                      << std::abs(actual.value[0] - estimates[i].value[0])
                      << " err_Theta="
                      << std::abs(actual.value[1] - estimates[i].value[1])
                      << " err_Axx="
                      << std::abs(actual.value[2] - estimates[i].value[2])
                      << " err_Axz="
                      << std::abs(actual.value[3] - estimates[i].value[3])
                      << " err_Azz="
                      << std::abs(actual.value[4] - estimates[i].value[4])
                      << " err_Aww="
                      << std::abs(actual.value[5] - estimates[i].value[5])
                      << "\n";
        }
        const double ratio_21 = errors[0] / errors[1];
        const double ratio_32 = errors[1] / errors[2];
        std::cout << "INFO central-difference ratios "
                  << direction_names[direction_index]
                  << " e-2/e-3=" << ratio_21
                  << " e-3/e-4=" << ratio_32 << "\n";
        require_true(std::string("second-order epsilon convergence ") +
                         direction_names[direction_index],
                     ratio_21 > 80.0 && ratio_21 < 120.0 &&
                         ratio_32 > 80.0 && ratio_32 < 120.0);
        require_true(std::string("epsilon 1e-5 selected-branch oracle ") +
                         direction_names[direction_index],
                     errors[3] < 3.0e-6);
        require_true(std::string("epsilon 1e-6 selected-branch oracle ") +
                         direction_names[direction_index],
                     errors[4] < 3.0e-6);
        require_true(std::string("roundoff saturation after convergence ") +
                         direction_names[direction_index],
                     errors[5] > std::min(errors[3], errors[4]));
    }
}

void check_pure_and_mixed_directions()
{
    const double r0 = 1.0;
    const double x = 2.1;
    const double z = 0.29;
    std::array<Direction, 2> pure = {};
    pure[0].at(Variable::K) = {0.37, -0.24, 0.0, 0.0, 0.0, 0.0};
    pure[1].at(Variable::hat_Gamma_x) =
        {0.21, -0.18, 0.0, 0.0, 0.0, 0.0};
    pure[1].at(Variable::hat_Gamma_z) =
        {0.0, 0.0, -0.26, 0.0, 0.0, 0.0};
    const std::array<const char *, 2> labels = {
        "pure K", "pure hatted-Gamma Z"};
    for (std::size_t i = 0; i < pure.size(); ++i)
    {
        require_true(std::string(labels[i]) + " selected-branch oracle",
                     max_error(assemble(r0, x, pure[i]),
                               central_difference(r0, x, z, 1.0e-5,
                                                  pure[i])
                                   .total) <
                         3.0e-6);
    }
    require_true("mixed selected-branch oracle",
                 max_error(assemble(r0, x, mixed_direction()),
                           central_difference(r0, x, z, 1.0e-5,
                                              mixed_direction())
                               .total) <
                     3.0e-6);
}

void check_isolated_geometric_ricci_direction()
{
    const double r0 = 1.0;
    const double x = 2.1;
    const double z = 0.29;
    Direction direction;
    direction.at(Variable::h_xx).dxx = 0.43;
    direction.at(Variable::h_xx).dxz = -0.17;
    direction.at(Variable::h_xz).dxx = -0.31;
    direction.at(Variable::h_xz).dxz = 0.19;
    direction.at(Variable::h_xz).dzz = -0.23;
    direction.at(Variable::h_zz).dxz = 0.11;
    direction.at(Variable::h_zz).dzz = 0.27;
    direction.at(Variable::h_ww).dxx = -0.14;
    direction.at(Variable::h_ww).dxz = 0.09;
    direction.at(Variable::h_ww).dzz = -0.18;

    const auto contracted = make_adapter(x, direction);
    direction.at(Variable::hat_Gamma_x).value = contracted.g_x();
    direction.at(Variable::hat_Gamma_x).dx = contracted.dx_g_x();
    direction.at(Variable::hat_Gamma_x).dz = contracted.dz_g_x();
    direction.at(Variable::hat_Gamma_z).value = contracted.g_z();
    direction.at(Variable::hat_Gamma_z).dx = contracted.dx_g_z();
    direction.at(Variable::hat_Gamma_z).dz = contracted.dz_g_z();

    const auto adapter = make_adapter(x, direction);
    const auto completion =
        Adapter::make_encoded_z_ricci_completion(x, adapter);
    const std::array<double, 9> completion_values = {
        completion.q_xx(), completion.q_xz(), completion.q_zz(),
        completion.q_ww(), completion.scalar_trace(), completion.tf_xx(),
        completion.tf_xz(), completion.tf_zz(), completion.tf_ww()};
    for (const double value : completion_values)
    {
        require_close("isolated geometric Ricci encoded-Z component", value,
                      0.0, 2.0e-15);
    }

    const auto families = production_families(r0, x, direction);
    double geometric_amplitude = 0.0;
    for (const double value : families[geometric_ricci].value)
    {
        geometric_amplitude = std::max(geometric_amplitude, std::abs(value));
    }
    require_true("isolated geometric Ricci is nonzero",
                 geometric_amplitude > 1.0e-3);
    for (const auto family : {advection, shift_stretching, encoded_z,
                              algebraic, damping})
    {
        std::cout << "INFO isolated geometric family " << family_names[family]
                  << " max=" << max_error(families[family], Rows{}) << "\n";
        require_true(std::string("isolated geometric unrelated family zero ") +
                         family_names[family],
                     max_error(families[family], Rows{}) < 2.0e-14);
    }
    std::cout << "INFO isolated geometric family geometric Ricci max="
              << geometric_amplitude << "\n";
    require_true("isolated geometric selected-branch oracle",
                 max_error(assemble(r0, x, direction),
                           central_difference(r0, x, z, 1.0e-5, direction)
                               .total) <
                     3.0e-6);
}

void check_trace_free_parity_scope_and_flags()
{
    const double r0 = 1.0;
    const double x = 2.0;
    const auto direction = mixed_direction();
    const auto families = production_families(r0, x, direction);
    require_close("no direct A_xx damping", families[damping].value[2], 0.0,
                  0.0);
    require_close("no direct A_xz damping", families[damping].value[3], 0.0,
                  0.0);
    require_close("no direct A_zz damping", families[damping].value[4], 0.0,
                  0.0);
    require_close("no direct A_ww damping", families[damping].value[5], 0.0,
                  0.0);
    for (const auto family : {geometric_ricci, encoded_z})
    {
        require_close(std::string("weighted curvature trace-free ") +
                          family_names[family],
                      families[family].value[2] +
                          families[family].value[4] +
                          2.0 * families[family].value[5],
                      0.0, 2.0e-12);
    }

    Direction tangent;
    tangent.at(Variable::h_xx) = {0.12, 0.07, 0.0, 0.0, 0.0, 0.0};
    tangent.at(Variable::h_zz) = {-0.08, 0.04, 0.0, 0.0, 0.0, 0.0};
    tangent.at(Variable::h_ww) = {0.05, -0.06, 0.0, 0.0, 0.0, 0.0};
    tangent.at(Variable::A_xx) = {0.20, 0.30, -0.16, 0.0, 0.0, 0.0};
    tangent.at(Variable::A_zz) = {-0.10, -0.20, 0.08, 0.0, 0.0, 0.0};
    tangent.at(Variable::A_xz) = {0.07, -0.09, 0.11, 0.0, 0.0, 0.0};
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double lambda_x = -1.5 * lambda / x;
    const double metric_trace_input =
        7.0 * lambda * tangent.at(Variable::h_xx).value / 8.0 +
        3.0 * lambda * tangent.at(Variable::h_zz).value / 8.0 -
        5.0 * lambda * tangent.at(Variable::h_ww).value / 4.0;
    tangent.at(Variable::A_ww).value =
        -0.5 * (tangent.at(Variable::A_xx).value +
                tangent.at(Variable::A_zz).value + metric_trace_input);
    const double metric_trace_dx =
        7.0 / 8.0 *
            (lambda_x * tangent.at(Variable::h_xx).value +
             lambda * tangent.at(Variable::h_xx).dx) +
        3.0 / 8.0 *
            (lambda_x * tangent.at(Variable::h_zz).value +
             lambda * tangent.at(Variable::h_zz).dx) -
        5.0 / 4.0 *
            (lambda_x * tangent.at(Variable::h_ww).value +
             lambda * tangent.at(Variable::h_ww).dx);
    tangent.at(Variable::A_ww).dx =
        -0.5 * (tangent.at(Variable::A_xx).dx +
                tangent.at(Variable::A_zz).dx + metric_trace_dx);
    tangent.at(Variable::A_ww).dz = 0.04;

    const double tangent_input =
        tangent.at(Variable::A_xx).value +
        tangent.at(Variable::A_zz).value +
        2.0 * tangent.at(Variable::A_ww).value + metric_trace_input;
    const double tangent_dx =
        tangent.at(Variable::A_xx).dx + tangent.at(Variable::A_zz).dx +
        2.0 * tangent.at(Variable::A_ww).dx + metric_trace_dx;
    std::cout << "INFO metric-inclusive tangent inputs hxx="
              << tangent.at(Variable::h_xx).value
              << " hzz=" << tangent.at(Variable::h_zz).value
              << " hww=" << tangent.at(Variable::h_ww).value
              << " Axx=" << tangent.at(Variable::A_xx).value
              << " Azz=" << tangent.at(Variable::A_zz).value
              << " Aww=" << tangent.at(Variable::A_ww).value
              << " dx_hxx=" << tangent.at(Variable::h_xx).dx
              << " dx_hzz=" << tangent.at(Variable::h_zz).dx
              << " dx_hww=" << tangent.at(Variable::h_ww).dx
              << " dx_Axx=" << tangent.at(Variable::A_xx).dx
              << " dx_Azz=" << tangent.at(Variable::A_zz).dx
              << " dx_Aww=" << tangent.at(Variable::A_ww).dx << "\n";
    require_close("metric-inclusive tangent trace input", tangent_input, 0.0,
                  2.0e-15);
    require_close("metric-inclusive tangent trace x derivative", tangent_dx,
                  0.0, 2.0e-15);

    const auto tangent_rows = assemble(r0, x, tangent);
    const auto tangent_oracle =
        central_difference(r0, x, 0.23, 1.0e-5, tangent).total;
    const auto tangent_state = make_state(tangent);
    const auto metric_advection = make_advection(r0, x, tangent);
    const auto metric_stretching =
        Operator::apply_tensor_shift_stretching_at_point(r0, x,
                                                         tangent_state);
    const auto metric_algebraic =
        Operator::apply_algebraic_metric_chi_coupling_at_point(tangent_state);
    auto metric_rhs = [&](const Variable variable) {
        return metric_advection.value(variable) +
               metric_stretching.value(variable) +
               metric_algebraic.value(variable);
    };
    const double metric_rhs_contribution =
        7.0 * lambda * metric_rhs(Variable::h_xx) / 8.0 +
        3.0 * lambda * metric_rhs(Variable::h_zz) / 8.0 -
        5.0 * lambda * metric_rhs(Variable::h_ww) / 4.0;
    const double a_rhs_contribution =
        tangent_rows.value[2] + tangent_rows.value[4] +
        2.0 * tangent_rows.value[5];
    std::cout << "INFO metric-inclusive tangent outputs rhs_hxx="
              << metric_rhs(Variable::h_xx)
              << " rhs_hzz=" << metric_rhs(Variable::h_zz)
              << " rhs_hww=" << metric_rhs(Variable::h_ww)
              << " rhs_Axx=" << tangent_rows.value[2]
              << " rhs_Azz=" << tangent_rows.value[4]
              << " rhs_Aww=" << tangent_rows.value[5] << "\n";
    std::cout << "INFO metric-inclusive tangent trace input=" << tangent_input
              << " dx=" << tangent_dx
              << " metric_rhs=" << metric_rhs_contribution
              << " A_rhs=" << a_rhs_contribution
              << " total_rhs="
              << metric_rhs_contribution + a_rhs_contribution << "\n";
    require_true("metric tangent RHS contribution is nonzero",
                 std::abs(metric_rhs_contribution) > 1.0e-4);
    require_close("complete metric-inclusive tangent-trace RHS",
                  metric_rhs_contribution + a_rhs_contribution, 0.0,
                  3.0e-12);
    require_close("tangent nonlinear oracle agrees with assembled A rows",
                  max_error(tangent_rows, tangent_oracle), 0.0, 3.0e-6);

    const auto positive = assemble(r0, x, direction);
    const auto negative = assemble(r0, x, reflected_direction(direction));
    for (const std::size_t even_row : {std::size_t{0}, std::size_t{1},
                                      std::size_t{2}, std::size_t{4},
                                      std::size_t{5}})
    {
        require_close("scalar/even row parity", positive.value[even_row],
                      negative.value[even_row], 3.0e-12);
    }
    require_close("A_xz one-z parity", positive.value[3],
                  -negative.value[3], 3.0e-12);

    Vector full = make_state(Direction{});
    (void)assemble(r0, x, direction, &full);
    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        if (variable != Variable::K && variable != Variable::Theta &&
            !Operator::is_a_variable(variable))
        {
            require_close(std::string("illegal output write guard ") +
                              Operator::variable_name(variable),
                          full.value(variable), 0.0, 0.0);
        }
    }

    require_true("K family inventory closed",
                 Operator::k_theta_a_surviving_term_family_inventory_closed);
    require_true("K/Theta/A final-row assembly implemented",
                 Operator::k_theta_a_final_row_assembly_implemented);
    require_true("combined row validation implemented",
                 Operator::k_theta_a_assembled_row_validation_implemented);
    require_true("K row flag implemented",
                 Operator::k_equation_rhs_block_implemented);
    require_true("Theta row flag implemented",
                 Operator::theta_equation_rhs_block_implemented);
    require_true("A row-family flag implemented",
                 Operator::a_equation_rhs_block_implemented);
    for (const auto variable : {Variable::K, Variable::Theta, Variable::A_xx,
                                Variable::A_xz, Variable::A_zz,
                                Variable::A_ww})
    {
        require_true(std::string("row complete ") +
                         Operator::variable_name(variable),
                     Operator::variable_rhs_complete(variable));
    }
    for (const auto variable : {Variable::chi, Variable::h_xx,
                                Variable::h_xz, Variable::h_zz,
                                Variable::h_ww})
    {
        require_true(std::string("later chi/metric row is complete ") +
                         Operator::variable_name(variable),
                     Operator::variable_rhs_complete(variable));
    }
    const auto contract =
        Operator::make_default_frozen_gauge_operator_contract();
    require_true("boundary-bearing complete operator remains false",
                 !contract.complete_operator_implemented());
    require_true("full-operator eigensolver gate remains false",
                 !contract.eigensolver_allowed());
    require_true("boundaries remain incomplete",
                 !Operator::boundary_derivative_validation_implemented);
    require_true("4AO-D remains incomplete",
                 !Operator::live_gauge_stage_4ao_d_implemented);
}

} // namespace

int main()
{
    check_background_and_zero();
    check_family_oracles_and_mutations();
    check_pure_and_mixed_directions();
    check_isolated_geometric_ricci_direction();
    check_epsilon_tables();
    check_trace_free_parity_scope_and_flags();
    std::cout << "PASS Stage 4AO-C complete frozen-gauge K/Theta/A rows\n";
    return 0;
}
