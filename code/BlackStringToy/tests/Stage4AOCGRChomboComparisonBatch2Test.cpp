#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ConformalCartoonAlgebra.hpp"

// Test-only infrastructure boundary.  The production CCZ4 RHS is header
// implemented, but its public header also includes Chombo grid wrappers that
// are irrelevant to rhs_equation.  Suppress only those wrappers and provide
// the minimum declarations required to instantiate the real rhs_equation.
#define CELL_HPP_
template <class data_t> class Cell;
#define FOURTHORDERDERIVATIVES_HPP_
class FourthOrderDerivatives;
#define MOVINGPUNCTUREGAUGE_HPP_
class MovingPunctureGauge
{
  public:
    struct params_t
    {
    };
};
#define CCZ4VARS_HPP_
namespace CCZ4Vars
{
template <class data_t> struct VarsWithGauge;
template <class data_t> struct Diff2VarsWithGauge;
} // namespace CCZ4Vars
#define USERVARIABLES_HPP
#define VARSTOOLS_HPP_
#define SIMD_HPP_

namespace MayDay
{
inline void Error(const char *message) { throw std::runtime_error(message); }
} // namespace MayDay

#include "CCZ4RHS.hpp"

namespace
{
constexpr int dimension = 3;
constexpr double absolute_tolerance = 5.0e-13;
constexpr double relative_tolerance = 5.0e-12;
constexpr double locked_kappa1 = 0.1;
constexpr double locked_kappa2 = 0.0;
constexpr double locked_kappa3 = 1.0;
constexpr double locked_cosmological_constant = 0.0;
constexpr bool locked_covariant_z4 = true;

static_assert(CH_SPACEDIM == dimension,
              "batch 2 direct bridge must compile in stock d=3");
static_assert(GR_SPACEDIM == dimension,
              "batch 2 direct bridge must use physical d=3");
static_assert(DEFAULT_TENSOR_DIM == dimension,
              "batch 2 tensor loops must cover d=3");
static_assert(
    BlackStringToy::ConformalCartoonAlgebra::physical_spatial_dim == 4,
    "the custom production oracle must remain physical d=4");
static_assert(
    BlackStringToy::ConformalCartoonAlgebra::gridded_spatial_dim == 2,
    "the custom production oracle must remain gridded d=2");
static_assert(
    BlackStringToy::ConformalCartoonAlgebra::hidden_multiplicity == 2,
    "the custom production oracle must retain two hidden directions");

using Matrix = std::array<std::array<double, dimension>, dimension>;
using Vector = std::array<double, dimension>;
using Derivative1 = std::array<Matrix, dimension>;
using Derivative2 = std::array<std::array<Matrix, dimension>, dimension>;

struct Statistics
{
    double max_absolute = 0.0;
    double max_normalized = 0.0;
    std::string worst_absolute;
    std::string worst_normalized;
    bool passed = true;
    std::size_t observations = 0;

    void observe(const double direct, const double custom,
                 const std::string &label)
    {
        const double absolute = std::abs(direct - custom);
        const double allowance =
            absolute_tolerance +
            relative_tolerance * std::max(std::abs(direct), std::abs(custom));
        const double normalized = absolute / allowance;
        ++observations;
        if (absolute > max_absolute)
        {
            max_absolute = absolute;
            worst_absolute = label;
        }
        if (normalized > max_normalized)
        {
            max_normalized = normalized;
            worst_normalized = label;
        }
        passed = passed && normalized <= 1.0;
    }
};

void compare_matrix(Statistics &statistics, const Matrix &direct,
                    const Matrix &custom, const std::string &label)
{
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            statistics.observe(
                direct[i][j], custom[i][j],
                label + "[" + std::to_string(i) + "," +
                    std::to_string(j) + "]");
        }
    }
}

Matrix add(const Matrix &left, const Matrix &right)
{
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = left[i][j] + right[i][j];
        }
    }
    return out;
}

Matrix subtract(const Matrix &left, const Matrix &right)
{
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = left[i][j] - right[i][j];
        }
    }
    return out;
}

Matrix scale(const Matrix &input, const double factor)
{
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = factor * input[i][j];
        }
    }
    return out;
}

Matrix inverse(const Matrix &input)
{
    std::array<std::array<double, 2 * dimension>, dimension> augmented{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            augmented[i][j] = input[i][j];
            augmented[i][j + dimension] = i == j ? 1.0 : 0.0;
        }
    }
    for (int column = 0; column < dimension; ++column)
    {
        int pivot = column;
        for (int row = column + 1; row < dimension; ++row)
        {
            if (std::abs(augmented[row][column]) >
                std::abs(augmented[pivot][column]))
            {
                pivot = row;
            }
        }
        if (std::abs(augmented[pivot][column]) <
            100.0 * std::numeric_limits<double>::epsilon())
        {
            throw std::runtime_error("independent inverse received singular data");
        }
        std::swap(augmented[pivot], augmented[column]);
        const double diagonal = augmented[column][column];
        for (double &entry : augmented[column])
        {
            entry /= diagonal;
        }
        for (int row = 0; row < dimension; ++row)
        {
            if (row == column)
            {
                continue;
            }
            const double multiplier = augmented[row][column];
            for (int j = 0; j < 2 * dimension; ++j)
            {
                augmented[row][j] -=
                    multiplier * augmented[column][j];
            }
        }
    }
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = augmented[i][j + dimension];
        }
    }
    return out;
}

double contract(const Matrix &lower, const Matrix &inverse_metric)
{
    double out = 0.0;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out += inverse_metric[i][j] * lower[i][j];
        }
    }
    return out;
}

Matrix trace_free(const Matrix &input, const Matrix &metric,
                  const Matrix &inverse_metric)
{
    Matrix out = input;
    const double input_trace = contract(input, inverse_metric);
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] -= metric[i][j] * input_trace / dimension;
        }
    }
    return out;
}

Matrix from_gr(const Tensor<2, double> &input)
{
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = input[i][j];
        }
    }
    return out;
}

template <class data_t> struct Vars
{
    data_t chi;
    Tensor<2, data_t> h;
    data_t K;
    Tensor<2, data_t> A;
    data_t Theta;
    Tensor<1, data_t> Gamma;
    data_t lapse;
    Tensor<1, data_t> shift;
    Tensor<1, data_t> B;
};

template <class data_t> struct Diff2Vars
{
    data_t chi;
    Tensor<2, data_t> h;
    data_t lapse;
    Tensor<1, data_t> shift;
};

struct FrozenGauge
{
    struct params_t
    {
    };

    explicit FrozenGauge(const params_t &) {}

    template <class data_t, template <typename> class vars_t,
              template <typename> class diff2_vars_t>
    void rhs_gauge(vars_t<data_t> &, const vars_t<data_t> &,
                   const vars_t<Tensor<1, data_t>> &,
                   const diff2_vars_t<Tensor<2, data_t>> &,
                   const vars_t<data_t> &) const
    {
    }
};

struct NoDerivatives
{
    explicit NoDerivatives(const double) {}
};

class DirectCCZ4Access : public CCZ4RHS<FrozenGauge, NoDerivatives>
{
    using Base = CCZ4RHS<FrozenGauge, NoDerivatives>;

  public:
    using Base::USE_BSSN;
    using Base::USE_CCZ4;
    using Base::rhs_equation;

    DirectCCZ4Access(const double kappa1, const double kappa2,
                     const double kappa3, const bool covariant_z4,
                     const int formulation)
        : Base(make_params(kappa1, kappa2, kappa3, covariant_z4), 1.0, 0.0,
               formulation, locked_cosmological_constant)
    {
    }

  private:
    static typename Base::params_t make_params(const double kappa1,
                                               const double kappa2,
                                               const double kappa3,
                                               const bool covariant_z4)
    {
        typename Base::params_t out{};
        out.kappa1 = kappa1;
        out.kappa2 = kappa2;
        out.kappa3 = kappa3;
        out.covariantZ4 = covariant_z4;
        return out;
    }
};

struct AnalyticInput
{
    std::string name;
    Vars<double> vars;
    Vars<Tensor<1, double>> d1;
    Diff2Vars<Tensor<2, double>> d2;
    Vars<double> advec;
};

Vars<double> zero_scalar_vars()
{
    Vars<double> out;
    out.chi = 0.0;
    out.K = 0.0;
    out.Theta = 0.0;
    out.lapse = 0.0;
    for (int i = 0; i < dimension; ++i)
    {
        out.Gamma[i] = 0.0;
        out.shift[i] = 0.0;
        out.B[i] = 0.0;
        for (int j = 0; j < dimension; ++j)
        {
            out.h[i][j] = 0.0;
            out.A[i][j] = 0.0;
        }
    }
    return out;
}

AnalyticInput make_flat_input(const std::string &name)
{
    AnalyticInput out;
    out.name = name;
    out.vars = zero_scalar_vars();
    out.advec = zero_scalar_vars();
    out.vars.chi = 1.0;
    out.vars.lapse = 1.0;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out.vars.h[i][j] = i == j ? 1.0 : 0.0;
            for (int derivative = 0; derivative < dimension; ++derivative)
            {
                out.d1.h[i][j][derivative] = 0.0;
                out.d1.A[i][j][derivative] = 0.0;
                out.d1.Gamma[i][derivative] = 0.0;
                out.d1.shift[i][derivative] = 0.0;
                out.d1.B[i][derivative] = 0.0;
                for (int second = 0; second < dimension; ++second)
                {
                    out.d2.h[i][j][derivative][second] = 0.0;
                    out.d2.shift[i][derivative][second] = 0.0;
                }
            }
        }
        out.d1.chi[i] = 0.0;
        out.d1.K[i] = 0.0;
        out.d1.Theta[i] = 0.0;
        out.d1.lapse[i] = 0.0;
        for (int j = 0; j < dimension; ++j)
        {
            out.d2.chi[i][j] = 0.0;
            out.d2.lapse[i][j] = 0.0;
        }
    }
    return out;
}

Vars<double> direct_rhs(const AnalyticInput &input,
                        const DirectCCZ4Access &access)
{
    Vars<double> out = zero_scalar_vars();
    access.rhs_equation(out, input.vars, input.d1, input.d2, input.advec);
    return out;
}

using Connection =
    std::array<std::array<std::array<double, dimension>, dimension>, dimension>;
using ConnectionDerivative = std::array<Connection, dimension>;

struct RicciResult
{
    Matrix tensor{};
    double scalar = 0.0;
};

struct GeometryPieces
{
    RicciResult raw;
    RicciResult encoded;
    RicciResult combined;
    Matrix encoded_trace_free{};
    Vector z_over_chi{};
    Vector z_upper{};
    Vector z_lower{};
};

struct MetricGeometry
{
    Matrix inverse_metric{};
    Connection christoffel{};
    ConnectionDerivative d1_christoffel{};
};

MetricGeometry metric_geometry(const Matrix &metric,
                               const Derivative1 &d1_metric,
                               const Derivative2 &d2_metric)
{
    MetricGeometry out;
    out.inverse_metric = inverse(metric);
    std::array<Matrix, dimension> d1_inverse{};
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                for (int p = 0; p < dimension; ++p)
                {
                    for (int q = 0; q < dimension; ++q)
                    {
                        d1_inverse[derivative][i][j] -=
                            out.inverse_metric[i][p] *
                            d1_metric[derivative][p][q] *
                            out.inverse_metric[q][j];
                    }
                }
            }
        }
    }

    for (int upper = 0; upper < dimension; ++upper)
    {
        for (int lower_1 = 0; lower_1 < dimension; ++lower_1)
        {
            for (int lower_2 = 0; lower_2 < dimension; ++lower_2)
            {
                for (int contracted = 0; contracted < dimension; ++contracted)
                {
                    const double bracket =
                        d1_metric[lower_1][contracted][lower_2] +
                        d1_metric[lower_2][contracted][lower_1] -
                        d1_metric[contracted][lower_1][lower_2];
                    out.christoffel[upper][lower_1][lower_2] +=
                        0.5 * out.inverse_metric[upper][contracted] * bracket;
                    for (int derivative = 0; derivative < dimension;
                         ++derivative)
                    {
                        const double derivative_bracket =
                            d2_metric[derivative][lower_1][contracted]
                                     [lower_2] +
                            d2_metric[derivative][lower_2][contracted]
                                     [lower_1] -
                            d2_metric[derivative][contracted][lower_1]
                                     [lower_2];
                        out.d1_christoffel[derivative][upper][lower_1]
                                            [lower_2] +=
                            0.5 *
                            (d1_inverse[derivative][upper][contracted] *
                                 bracket +
                             out.inverse_metric[upper][contracted] *
                                 derivative_bracket);
                    }
                }
            }
        }
    }
    return out;
}

void extract_conformal_metric_jet(const AnalyticInput &input, Matrix &h,
                                  Derivative1 &d1_h, Derivative2 &d2_h)
{
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            h[i][j] = input.vars.h[i][j];
            for (int first = 0; first < dimension; ++first)
            {
                d1_h[first][i][j] = input.d1.h[i][j][first];
                for (int second = 0; second < dimension; ++second)
                {
                    d2_h[first][second][i][j] =
                        input.d2.h[i][j][first][second];
                }
            }
        }
    }
}

void physical_metric_jet(const AnalyticInput &input, Matrix &gamma,
                         Derivative1 &d1_gamma, Derivative2 &d2_gamma)
{
    const double chi = input.vars.chi;
    const double inv_chi = 1.0 / chi;
    const double inv_chi2 = inv_chi * inv_chi;
    const double inv_chi3 = inv_chi2 * inv_chi;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            const double h = input.vars.h[i][j];
            gamma[i][j] = h * inv_chi;
            for (int first = 0; first < dimension; ++first)
            {
                const double d1_h = input.d1.h[i][j][first];
                const double d1_chi = input.d1.chi[first];
                d1_gamma[first][i][j] =
                    d1_h * inv_chi - h * d1_chi * inv_chi2;
                for (int second = 0; second < dimension; ++second)
                {
                    d2_gamma[first][second][i][j] =
                        input.d2.h[i][j][first][second] * inv_chi -
                        (d1_h * input.d1.chi[second] +
                         input.d1.h[i][j][second] * d1_chi +
                         h * input.d2.chi[first][second]) *
                            inv_chi2 +
                        2.0 * h * d1_chi * input.d1.chi[second] * inv_chi3;
                }
            }
        }
    }
}

RicciResult independent_raw_ricci(const AnalyticInput &input)
{
    Matrix gamma{};
    Derivative1 d1_gamma{};
    Derivative2 d2_gamma{};
    physical_metric_jet(input, gamma, d1_gamma, d2_gamma);
    const MetricGeometry geometry =
        metric_geometry(gamma, d1_gamma, d2_gamma);

    RicciResult out;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                out.tensor[i][j] +=
                    geometry.d1_christoffel[k][k][i][j] -
                    geometry.d1_christoffel[j][k][i][k];
                for (int l = 0; l < dimension; ++l)
                {
                    out.tensor[i][j] +=
                        geometry.christoffel[k][i][j] *
                            geometry.christoffel[l][k][l] -
                        geometry.christoffel[l][i][k] *
                            geometry.christoffel[k][j][l];
                }
            }
        }
    }
    out.scalar = contract(out.tensor, geometry.inverse_metric);
    return out;
}

GeometryPieces independent_geometry_pieces(const AnalyticInput &input)
{
    GeometryPieces out;
    out.raw = independent_raw_ricci(input);

    Matrix h{};
    Derivative1 d1_h{};
    Derivative2 d2_h{};
    extract_conformal_metric_jet(input, h, d1_h, d2_h);
    const MetricGeometry conformal_geometry =
        metric_geometry(h, d1_h, d2_h);

    Vector contracted{};
    std::array<Vector, dimension> d1_contracted{};
    for (int upper = 0; upper < dimension; ++upper)
    {
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                contracted[upper] +=
                    conformal_geometry.inverse_metric[i][j] *
                    conformal_geometry.christoffel[upper][i][j];
            }
        }
    }

    std::array<Matrix, dimension> d1_h_inverse{};
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                for (int p = 0; p < dimension; ++p)
                {
                    for (int q = 0; q < dimension; ++q)
                    {
                        d1_h_inverse[derivative][i][j] -=
                            conformal_geometry.inverse_metric[i][p] *
                            d1_h[derivative][p][q] *
                            conformal_geometry.inverse_metric[q][j];
                    }
                }
            }
        }
    }
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        for (int upper = 0; upper < dimension; ++upper)
        {
            for (int i = 0; i < dimension; ++i)
            {
                for (int j = 0; j < dimension; ++j)
                {
                    d1_contracted[derivative][upper] +=
                        d1_h_inverse[derivative][i][j] *
                            conformal_geometry.christoffel[upper][i][j] +
                        conformal_geometry.inverse_metric[i][j] *
                            conformal_geometry
                                .d1_christoffel[derivative][upper][i][j];
                }
            }
        }
    }

    std::array<Vector, dimension> d1_z_over_chi{};
    for (int i = 0; i < dimension; ++i)
    {
        out.z_over_chi[i] = 0.5 * (input.vars.Gamma[i] - contracted[i]);
        out.z_upper[i] = input.vars.chi * out.z_over_chi[i];
        for (int derivative = 0; derivative < dimension; ++derivative)
        {
            d1_z_over_chi[derivative][i] =
                0.5 * (input.d1.Gamma[i][derivative] -
                       d1_contracted[derivative][i]);
        }
    }
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out.z_lower[i] += h[i][j] * out.z_over_chi[j];
        }
    }

    Matrix gamma{};
    Derivative1 d1_gamma{};
    Derivative2 d2_gamma{};
    physical_metric_jet(input, gamma, d1_gamma, d2_gamma);
    const MetricGeometry physical_geometry =
        metric_geometry(gamma, d1_gamma, d2_gamma);
    Matrix covariant_z_derivative{};
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        for (int lower = 0; lower < dimension; ++lower)
        {
            for (int j = 0; j < dimension; ++j)
            {
                covariant_z_derivative[derivative][lower] +=
                    d1_h[derivative][lower][j] * out.z_over_chi[j] +
                    h[lower][j] * d1_z_over_chi[derivative][j] -
                    physical_geometry.christoffel[j][derivative][lower] *
                        out.z_lower[j];
            }
        }
    }

    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out.encoded.tensor[i][j] =
                covariant_z_derivative[i][j] +
                covariant_z_derivative[j][i];
            out.combined.tensor[i][j] =
                out.raw.tensor[i][j] + out.encoded.tensor[i][j];
        }
    }
    out.encoded.scalar =
        contract(out.encoded.tensor, physical_geometry.inverse_metric);
    out.combined.scalar = out.raw.scalar + out.encoded.scalar;
    out.encoded_trace_free =
        trace_free(out.encoded.tensor, h, conformal_geometry.inverse_metric);
    return out;
}

GeometryPieces direct_geometry_pieces(const AnalyticInput &input)
{
    const Tensor<2, double> h_inverse =
        TensorAlgebra::compute_inverse_sym(input.vars.h);
    const auto christoffel =
        TensorAlgebra::compute_christoffel(input.d1.h, h_inverse);
    Tensor<1, double> z_over_chi;
    for (int i = 0; i < dimension; ++i)
    {
        z_over_chi[i] =
            0.5 * (input.vars.Gamma[i] - christoffel.contracted[i]);
    }
    const auto raw = CCZ4Geometry::compute_ricci(
        input.vars, input.d1, input.d2, h_inverse, christoffel);
    const auto combined = CCZ4Geometry::compute_ricci_Z(
        input.vars, input.d1, input.d2, h_inverse, christoffel, z_over_chi);

    GeometryPieces out;
    out.raw = {from_gr(raw.LL), raw.scalar};
    out.combined = {from_gr(combined.LL), combined.scalar};
    out.encoded.tensor = subtract(out.combined.tensor, out.raw.tensor);
    out.encoded.scalar = out.combined.scalar - out.raw.scalar;
    const Matrix h = from_gr(input.vars.h);
    const Matrix h_inv = from_gr(h_inverse);
    out.encoded_trace_free =
        trace_free(out.encoded.tensor, h, h_inv);
    // The contracted connection above comes from GRChombo's directly called
    // geometry path.  These standalone Z values are local reconstructions of
    // the inspected CCZ4RHS.impl.hpp convention; rhs_equation constructs
    // Z_over_chi and Z internally but exposes no callable standalone output.
    for (int i = 0; i < dimension; ++i)
    {
        out.z_over_chi[i] = z_over_chi[i];
        out.z_upper[i] = input.vars.chi * z_over_chi[i];
        for (int j = 0; j < dimension; ++j)
        {
            out.z_lower[i] += input.vars.h[i][j] * z_over_chi[j];
        }
    }
    return out;
}

void set_symmetric(Tensor<2, double> &tensor, const int i, const int j,
                   const double value)
{
    tensor[i][j] = value;
    tensor[j][i] = value;
}

void set_d1_symmetric(AnalyticInput &input, const int derivative, const int i,
                      const int j, const double value)
{
    input.d1.h[i][j][derivative] = value;
    input.d1.h[j][i][derivative] = value;
}

void set_d2_symmetric(AnalyticInput &input, const int first, const int second,
                      const int i, const int j, const double value)
{
    input.d2.h[i][j][first][second] = value;
    input.d2.h[j][i][first][second] = value;
    input.d2.h[i][j][second][first] = value;
    input.d2.h[j][i][second][first] = value;
}

void complete_determinant_one_component(AnalyticInput &input)
{
    constexpr int x = 0;
    constexpr int y = 1;
    constexpr int z = 2;
    const double a = input.vars.h[x][x];
    const double b = input.vars.h[x][z];
    const double c = input.vars.h[z][z];
    const double determinant_xz = a * c - b * b;
    input.vars.h[y][y] = 1.0 / determinant_xz;
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        const double da = input.d1.h[x][x][derivative];
        const double db = input.d1.h[x][z][derivative];
        const double dc = input.d1.h[z][z][derivative];
        const double ddet = da * c + a * dc - 2.0 * b * db;
        input.d1.h[y][y][derivative] =
            -ddet / (determinant_xz * determinant_xz);
    }
    for (int first = 0; first < dimension; ++first)
    {
        for (int second = 0; second < dimension; ++second)
        {
            const double da_first = input.d1.h[x][x][first];
            const double db_first = input.d1.h[x][z][first];
            const double dc_first = input.d1.h[z][z][first];
            const double da_second = input.d1.h[x][x][second];
            const double db_second = input.d1.h[x][z][second];
            const double dc_second = input.d1.h[z][z][second];
            const double ddet_first =
                da_first * c + a * dc_first - 2.0 * b * db_first;
            const double ddet_second =
                da_second * c + a * dc_second - 2.0 * b * db_second;
            const double dda = input.d2.h[x][x][first][second];
            const double ddb = input.d2.h[x][z][first][second];
            const double ddc = input.d2.h[z][z][first][second];
            const double d2det =
                dda * c + da_first * dc_second + da_second * dc_first +
                a * ddc - 2.0 * (db_first * db_second + b * ddb);
            input.d2.h[y][y][first][second] =
                2.0 * ddet_first * ddet_second /
                    (determinant_xz * determinant_xz * determinant_xz) -
                d2det / (determinant_xz * determinant_xz);
        }
    }
}

AnalyticInput make_geometry_case(const std::string &name, const double sign)
{
    AnalyticInput out = make_flat_input(name);
    out.vars.chi = 0.91 + 0.04 * sign;
    out.d1.chi[0] = 0.08 * sign;
    out.d1.chi[2] = -0.11;
    out.d2.chi[0][0] = -0.06;
    out.d2.chi[0][2] = 0.035 * sign;
    out.d2.chi[2][0] = out.d2.chi[0][2];
    out.d2.chi[2][2] = 0.09;

    out.vars.h[0][0] = 1.08 + 0.03 * sign;
    out.vars.h[2][2] = 1.04 - 0.02 * sign;
    set_symmetric(out.vars.h, 0, 2, 0.13 * sign);
    set_d1_symmetric(out, 0, 0, 0, 0.14);
    set_d1_symmetric(out, 0, 0, 2, -0.17 * sign);
    set_d1_symmetric(out, 0, 2, 2, 0.09);
    set_d1_symmetric(out, 2, 0, 0, -0.07 * sign);
    set_d1_symmetric(out, 2, 0, 2, 0.12);
    set_d1_symmetric(out, 2, 2, 2, -0.10 * sign);
    set_d2_symmetric(out, 0, 0, 0, 0, 0.08);
    set_d2_symmetric(out, 0, 0, 0, 2, -0.06 * sign);
    set_d2_symmetric(out, 0, 0, 2, 2, 0.05);
    set_d2_symmetric(out, 0, 2, 0, 0, -0.04);
    set_d2_symmetric(out, 0, 2, 0, 2, 0.11 * sign);
    set_d2_symmetric(out, 0, 2, 2, 2, -0.03);
    set_d2_symmetric(out, 2, 2, 0, 0, 0.07);
    set_d2_symmetric(out, 2, 2, 0, 2, 0.025 * sign);
    set_d2_symmetric(out, 2, 2, 2, 2, -0.09);
    complete_determinant_one_component(out);

    out.vars.Gamma[0] = 0.31 * sign;
    out.vars.Gamma[1] = -0.08;
    out.vars.Gamma[2] = -0.27;
    for (int i = 0; i < dimension; ++i)
    {
        for (int derivative = 0; derivative < dimension; ++derivative)
        {
            out.d1.Gamma[i][derivative] =
                0.025 * (i + 1) * (derivative + 1) *
                (derivative == 2 ? sign : 1.0);
        }
    }
    return out;
}

struct GeometryStatistics
{
    Statistics raw_tensor;
    Statistics raw_scalar;
    Statistics encoded_tensor;
    Statistics encoded_scalar;
    Statistics encoded_trace_free;
    Statistics combined_tensor;
    Statistics combined_scalar;
    Statistics z_mapping;

    bool passed() const
    {
        return raw_tensor.passed && raw_scalar.passed &&
               encoded_tensor.passed && encoded_scalar.passed &&
               encoded_trace_free.passed && combined_tensor.passed &&
               combined_scalar.passed && z_mapping.passed;
    }
};

GeometryStatistics run_geometry_comparisons(
    const std::vector<AnalyticInput> &cases)
{
    GeometryStatistics stats;
    for (const auto &input : cases)
    {
        const GeometryPieces direct = direct_geometry_pieces(input);
        const GeometryPieces custom = independent_geometry_pieces(input);
        compare_matrix(stats.raw_tensor, direct.raw.tensor, custom.raw.tensor,
                       input.name + ".raw_R");
        stats.raw_scalar.observe(direct.raw.scalar, custom.raw.scalar,
                                 input.name + ".raw_scalar");
        compare_matrix(stats.encoded_tensor, direct.encoded.tensor,
                       custom.encoded.tensor, input.name + ".encoded_Z");
        stats.encoded_scalar.observe(
            direct.encoded.scalar, custom.encoded.scalar,
            input.name + ".encoded_Z_scalar");
        compare_matrix(stats.encoded_trace_free, direct.encoded_trace_free,
                       custom.encoded_trace_free,
                       input.name + ".encoded_Z_TF");
        compare_matrix(stats.combined_tensor, direct.combined.tensor,
                       custom.combined.tensor, input.name + ".combined");
        stats.combined_scalar.observe(
            direct.combined.scalar, custom.combined.scalar,
            input.name + ".combined_scalar");
        for (int i = 0; i < dimension; ++i)
        {
            stats.z_mapping.observe(
                direct.z_over_chi[i], custom.z_over_chi[i],
                input.name + ".Z_over_chi[" + std::to_string(i) + "]");
            stats.z_mapping.observe(
                direct.z_upper[i], custom.z_upper[i],
                input.name + ".Z_upper[" + std::to_string(i) + "]");
            stats.z_mapping.observe(
                direct.z_lower[i], custom.z_lower[i],
                input.name + ".Z_lower[" + std::to_string(i) + "]");
        }
    }
    return stats;
}

struct RHSFamilies
{
    double chi_lapse_k = 0.0;
    double chi_shift_divergence = 0.0;
    double chi_advection = 0.0;
    double chi_total = 0.0;

    Matrix h_algebraic{};
    Matrix h_advection{};
    Matrix h_shift_stretching{};
    Matrix h_trace_correction{};
    Matrix h_total{};

    double k_ricci_z = 0.0;
    double k_nonlinear = 0.0;
    double k_damping = 0.0;
    double k_lapse_hessian = 0.0;
    double k_advection = 0.0;
    double k_matter_lambda = 0.0;
    double k_total = 0.0;

    double theta_ricci_z = 0.0;
    double theta_k2_a2 = 0.0;
    double theta_minus_k_theta = 0.0;
    double theta_damping = 0.0;
    double theta_lapse_gradient_z = 0.0;
    double theta_advection = 0.0;
    double theta_matter_lambda = 0.0;
    double theta_total = 0.0;

    Matrix a_ricci_z{};
    Matrix a_lapse_hessian{};
    Matrix a_linear{};
    Matrix a_quadratic{};
    Matrix a_advection{};
    Matrix a_shift_stretching{};
    Matrix a_trace_correction{};
    Matrix a_damping{};
    Matrix a_total{};
};

Matrix input_matrix(const Tensor<2, double> &input) { return from_gr(input); }

Matrix physical_lapse_hessian(const AnalyticInput &input)
{
    Matrix gamma{};
    Derivative1 d1_gamma{};
    Derivative2 d2_gamma{};
    physical_metric_jet(input, gamma, d1_gamma, d2_gamma);
    const MetricGeometry geometry =
        metric_geometry(gamma, d1_gamma, d2_gamma);
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = input.d2.lapse[i][j];
            for (int k = 0; k < dimension; ++k)
            {
                out[i][j] -= geometry.christoffel[k][i][j] *
                             input.d1.lapse[k];
            }
        }
    }
    return out;
}

RHSFamilies custom_d3_families(const AnalyticInput &input,
                               const double kappa1 = locked_kappa1,
                               const double kappa2 = locked_kappa2)
{
    RHSFamilies out;
    const Matrix h = input_matrix(input.vars.h);
    const Matrix h_inverse = inverse(h);
    const Matrix a = input_matrix(input.vars.A);
    const GeometryPieces geometry = independent_geometry_pieces(input);
    const Matrix lapse_hessian = physical_lapse_hessian(input);

    double shift_divergence = 0.0;
    for (int i = 0; i < dimension; ++i)
    {
        shift_divergence += input.d1.shift[i][i];
    }
    const double effective_kappa1_lapse =
        locked_covariant_z4 ? kappa1 : kappa1 * input.vars.lapse;

    out.chi_lapse_k =
        (2.0 / dimension) * input.vars.chi * input.vars.lapse * input.vars.K;
    out.chi_shift_divergence =
        -(2.0 / dimension) * input.vars.chi * shift_divergence;
    out.chi_advection = input.advec.chi;
    out.chi_total =
        out.chi_lapse_k + out.chi_shift_divergence + out.chi_advection;

    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out.h_algebraic[i][j] =
                -2.0 * input.vars.lapse * input.vars.A[i][j];
            out.h_advection[i][j] = input.advec.h[i][j];
            out.h_trace_correction[i][j] =
                -(2.0 / dimension) * input.vars.h[i][j] *
                shift_divergence;
            for (int k = 0; k < dimension; ++k)
            {
                out.h_shift_stretching[i][j] +=
                    input.vars.h[k][i] * input.d1.shift[k][j] +
                    input.vars.h[k][j] * input.d1.shift[k][i];
            }
            out.h_total[i][j] =
                out.h_algebraic[i][j] + out.h_advection[i][j] +
                out.h_shift_stretching[i][j] +
                out.h_trace_correction[i][j];
        }
    }

    Matrix a_upper{};
    Matrix a_mixed{};
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                a_mixed[i][j] += a[i][k] * h_inverse[k][j];
                for (int l = 0; l < dimension; ++l)
                {
                    a_upper[i][j] += h_inverse[i][k] * h_inverse[j][l] *
                                     a[k][l];
                }
            }
        }
    }
    const double trace_a2 = contract(a, a_upper);
    const double laplacian_lapse =
        input.vars.chi * contract(lapse_hessian, h_inverse);

    out.k_ricci_z = input.vars.lapse * geometry.combined.scalar;
    out.k_nonlinear =
        input.vars.lapse * input.vars.K *
        (input.vars.K - 2.0 * input.vars.Theta);
    out.k_damping = -effective_kappa1_lapse * dimension *
                    (1.0 + kappa2) * input.vars.Theta;
    out.k_lapse_hessian = -laplacian_lapse;
    out.k_advection = input.advec.K;
    out.k_matter_lambda = 0.0;
    out.k_total = out.k_ricci_z + out.k_nonlinear + out.k_damping +
                  out.k_lapse_hessian + out.k_advection +
                  out.k_matter_lambda;

    out.theta_ricci_z =
        0.5 * input.vars.lapse * geometry.combined.scalar;
    out.theta_k2_a2 =
        0.5 * input.vars.lapse *
        (-trace_a2 + ((dimension - 1.0) / dimension) *
                         input.vars.K * input.vars.K);
    out.theta_minus_k_theta =
        -input.vars.lapse * input.vars.Theta * input.vars.K;
    out.theta_damping =
        -0.5 * input.vars.Theta * effective_kappa1_lapse *
        ((dimension + 1.0) + kappa2 * (dimension - 1.0));
    for (int i = 0; i < dimension; ++i)
    {
        out.theta_lapse_gradient_z -=
            geometry.z_upper[i] * input.d1.lapse[i];
    }
    out.theta_advection = input.advec.Theta;
    out.theta_matter_lambda = 0.0;
    out.theta_total =
        out.theta_ricci_z + out.theta_k2_a2 +
        out.theta_minus_k_theta + out.theta_damping +
        out.theta_lapse_gradient_z + out.theta_advection +
        out.theta_matter_lambda;

    out.a_ricci_z =
        trace_free(scale(geometry.combined.tensor,
                         input.vars.chi * input.vars.lapse),
                   h, h_inverse);
    out.a_lapse_hessian =
        trace_free(scale(lapse_hessian, -input.vars.chi), h, h_inverse);
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out.a_linear[i][j] =
                input.vars.lapse *
                (input.vars.K - 2.0 * input.vars.Theta) * a[i][j];
            out.a_advection[i][j] = input.advec.A[i][j];
            out.a_trace_correction[i][j] =
                -(2.0 / dimension) * shift_divergence * a[i][j];
            for (int k = 0; k < dimension; ++k)
            {
                out.a_shift_stretching[i][j] +=
                    a[k][i] * input.d1.shift[k][j] +
                    a[k][j] * input.d1.shift[k][i];
                out.a_quadratic[i][j] -=
                    2.0 * input.vars.lapse * a_mixed[i][k] * a[k][j];
            }
            out.a_total[i][j] =
                out.a_ricci_z[i][j] + out.a_lapse_hessian[i][j] +
                out.a_linear[i][j] + out.a_quadratic[i][j] +
                out.a_advection[i][j] +
                out.a_shift_stretching[i][j] +
                out.a_trace_correction[i][j] + out.a_damping[i][j];
        }
    }
    return out;
}

void make_trace_free_a(AnalyticInput &input)
{
    const Matrix h = input_matrix(input.vars.h);
    const Matrix h_inverse = inverse(h);
    Matrix a = input_matrix(input.vars.A);
    a = trace_free(a, h, h_inverse);
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            input.vars.A[i][j] = a[i][j];
        }
    }
}

AnalyticInput make_mixed_rhs_case(const std::string &name, const double sign)
{
    AnalyticInput out = make_geometry_case(name, sign);
    out.vars.K = 0.37 * sign;
    out.vars.Theta = -0.12;
    out.vars.lapse = 1.16 + 0.05 * sign;
    out.d1.lapse[0] = 0.09;
    out.d1.lapse[1] = -0.035 * sign;
    out.d1.lapse[2] = -0.07;
    out.d2.lapse[0][0] = -0.08;
    out.d2.lapse[0][2] = 0.045 * sign;
    out.d2.lapse[2][0] = out.d2.lapse[0][2];
    out.d2.lapse[1][1] = 0.025;
    out.d2.lapse[2][2] = 0.11;

    set_symmetric(out.vars.A, 0, 0, 0.18);
    set_symmetric(out.vars.A, 0, 2, -0.09 * sign);
    set_symmetric(out.vars.A, 1, 1, -0.07);
    set_symmetric(out.vars.A, 2, 2, 0.12);
    make_trace_free_a(out);

    out.d1.shift[0][0] = 0.08;
    out.d1.shift[0][2] = -0.04 * sign;
    out.d1.shift[1][1] = -0.03;
    out.d1.shift[2][0] = 0.055;
    out.d1.shift[2][2] = 0.06;

    out.advec.chi = 0.021 * sign;
    out.advec.K = -0.032;
    out.advec.Theta = 0.018 * sign;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out.advec.h[i][j] =
                0.006 * (i + 1) * (j + 1) * (i == j ? sign : 1.0);
            out.advec.A[i][j] =
                -0.004 * (i + 1) * (j + 1) *
                (i == j ? 1.0 : sign);
        }
    }
    return out;
}

struct CombinedRHSStatistics
{
    Statistics chi;
    Statistics h;
    Statistics k;
    Statistics theta;
    Statistics a;

    bool passed() const
    {
        return chi.passed && h.passed && k.passed && theta.passed &&
               a.passed;
    }
};

CombinedRHSStatistics run_combined_rhs_comparison(
    const std::vector<AnalyticInput> &cases,
    const DirectCCZ4Access &access)
{
    CombinedRHSStatistics stats;
    for (const auto &input : cases)
    {
        const Vars<double> direct = direct_rhs(input, access);
        const RHSFamilies custom = custom_d3_families(input);
        stats.chi.observe(direct.chi, custom.chi_total,
                          input.name + ".chi");
        stats.k.observe(direct.K, custom.k_total, input.name + ".K");
        stats.theta.observe(direct.Theta, custom.theta_total,
                            input.name + ".Theta");
        compare_matrix(stats.h, from_gr(direct.h), custom.h_total,
                       input.name + ".h");
        compare_matrix(stats.a, from_gr(direct.A), custom.a_total,
                       input.name + ".A");
    }
    return stats;
}

struct FamilyStatistics
{
    Statistics chi_lapse_k;
    Statistics chi_shift_divergence;
    Statistics chi_advection;
    Statistics h_algebraic;
    Statistics h_advection;
    Statistics h_shift_combined;
    Statistics k_ricci_z;
    Statistics k_nonlinear;
    Statistics k_damping;
    Statistics k_lapse_hessian;
    Statistics k_advection;
    Statistics theta_ricci_z;
    Statistics theta_k2_a2;
    Statistics theta_minus_k_theta;
    Statistics theta_damping;
    Statistics theta_lapse_gradient_z;
    Statistics theta_advection;
    Statistics a_ricci_z;
    Statistics a_lapse_hessian;
    Statistics a_linear;
    Statistics a_quadratic;
    Statistics a_advection;
    Statistics a_shift_combined;
    Statistics a_no_damping;

    bool passed() const
    {
        return chi_lapse_k.passed && chi_shift_divergence.passed &&
               chi_advection.passed && h_algebraic.passed &&
               h_advection.passed && h_shift_combined.passed &&
               k_ricci_z.passed && k_nonlinear.passed &&
               k_damping.passed && k_lapse_hessian.passed &&
               k_advection.passed && theta_ricci_z.passed &&
               theta_k2_a2.passed && theta_minus_k_theta.passed &&
               theta_damping.passed && theta_lapse_gradient_z.passed &&
               theta_advection.passed && a_ricci_z.passed &&
               a_lapse_hessian.passed && a_linear.passed &&
               a_quadratic.passed && a_advection.passed &&
               a_shift_combined.passed && a_no_damping.passed;
    }
};

void set_test_a(AnalyticInput &input, const double sign = 1.0)
{
    set_symmetric(input.vars.A, 0, 0, 0.16 * sign);
    set_symmetric(input.vars.A, 0, 2, -0.08 * sign);
    set_symmetric(input.vars.A, 1, 1, -0.06 * sign);
    set_symmetric(input.vars.A, 2, 2, 0.11 * sign);
    make_trace_free_a(input);
}

FamilyStatistics run_family_comparisons(
    const DirectCCZ4Access &locked_access,
    const DirectCCZ4Access &zero_damping_access)
{
    FamilyStatistics stats;

    AnalyticInput chi_k = make_flat_input("chi_lapse_K");
    chi_k.vars.chi = 0.87;
    chi_k.vars.lapse = 1.23;
    chi_k.vars.K = 0.42;
    auto direct = direct_rhs(chi_k, locked_access);
    auto custom = custom_d3_families(chi_k);
    stats.chi_lapse_k.observe(direct.chi, custom.chi_lapse_k,
                              chi_k.name);

    AnalyticInput chi_div = make_flat_input("chi_shift_divergence");
    chi_div.vars.chi = 0.94;
    chi_div.d1.shift[0][0] = 0.08;
    chi_div.d1.shift[1][1] = -0.03;
    chi_div.d1.shift[2][2] = 0.06;
    direct = direct_rhs(chi_div, locked_access);
    custom = custom_d3_families(chi_div);
    stats.chi_shift_divergence.observe(
        direct.chi, custom.chi_shift_divergence, chi_div.name);

    AnalyticInput chi_adv = make_flat_input("chi_advection");
    chi_adv.advec.chi = -0.073;
    direct = direct_rhs(chi_adv, locked_access);
    custom = custom_d3_families(chi_adv);
    stats.chi_advection.observe(direct.chi, custom.chi_advection,
                                chi_adv.name);

    AnalyticInput h_algebraic = make_flat_input("h_algebraic");
    h_algebraic.vars.lapse = 1.17;
    set_test_a(h_algebraic);
    direct = direct_rhs(h_algebraic, zero_damping_access);
    custom = custom_d3_families(h_algebraic, 0.0, 0.0);
    compare_matrix(stats.h_algebraic, from_gr(direct.h),
                   custom.h_algebraic, h_algebraic.name);

    AnalyticInput h_advection = make_flat_input("h_advection");
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            h_advection.advec.h[i][j] =
                0.017 * (i + 1) * (j + 1);
        }
    }
    direct = direct_rhs(h_advection, zero_damping_access);
    custom = custom_d3_families(h_advection, 0.0, 0.0);
    compare_matrix(stats.h_advection, from_gr(direct.h),
                   custom.h_advection, h_advection.name);

    AnalyticInput h_shift = make_flat_input("h_shift_total");
    h_shift.vars.h[0][0] = 1.12;
    h_shift.vars.h[1][1] = 0.97;
    h_shift.vars.h[2][2] = 0.93;
    set_symmetric(h_shift.vars.h, 0, 2, 0.11);
    h_shift.d1.shift[0][0] = 0.07;
    h_shift.d1.shift[0][2] = -0.05;
    h_shift.d1.shift[2][0] = 0.04;
    h_shift.d1.shift[2][2] = 0.09;
    direct = direct_rhs(h_shift, zero_damping_access);
    custom = custom_d3_families(h_shift, 0.0, 0.0);
    compare_matrix(stats.h_shift_combined, from_gr(direct.h),
                   add(custom.h_shift_stretching,
                       custom.h_trace_correction),
                   h_shift.name);

    AnalyticInput geometry = make_geometry_case("ricci_Z_family", 1.0);
    geometry.vars.lapse = 1.19;
    direct = direct_rhs(geometry, zero_damping_access);
    custom = custom_d3_families(geometry, 0.0, 0.0);
    stats.k_ricci_z.observe(direct.K, custom.k_ricci_z, geometry.name);
    stats.theta_ricci_z.observe(direct.Theta, custom.theta_ricci_z,
                                geometry.name);
    compare_matrix(stats.a_ricci_z, from_gr(direct.A),
                   custom.a_ricci_z, geometry.name);

    AnalyticInput k_nonlinear = make_flat_input("K_nonlinear");
    k_nonlinear.vars.lapse = 1.14;
    k_nonlinear.vars.K = 0.39;
    k_nonlinear.vars.Theta = -0.13;
    direct = direct_rhs(k_nonlinear, zero_damping_access);
    custom = custom_d3_families(k_nonlinear, 0.0, 0.0);
    stats.k_nonlinear.observe(direct.K, custom.k_nonlinear,
                              k_nonlinear.name);

    AnalyticInput damping = make_flat_input("damping");
    damping.vars.Theta = -0.16;
    const auto direct_damped = direct_rhs(damping, locked_access);
    const auto direct_undamped = direct_rhs(damping, zero_damping_access);
    custom = custom_d3_families(damping);
    stats.k_damping.observe(direct_damped.K - direct_undamped.K,
                            custom.k_damping, "K_damping");
    stats.theta_damping.observe(
        direct_damped.Theta - direct_undamped.Theta,
        custom.theta_damping, "Theta_damping");

    AnalyticInput hessian = make_flat_input("lapse_hessian");
    hessian.vars.chi = 0.88;
    hessian.d1.chi[0] = 0.07;
    hessian.d1.chi[2] = -0.04;
    hessian.vars.lapse = 1.21;
    hessian.d1.lapse[0] = 0.06;
    hessian.d1.lapse[2] = -0.09;
    hessian.d2.lapse[0][0] = -0.08;
    hessian.d2.lapse[0][2] = 0.035;
    hessian.d2.lapse[2][0] = 0.035;
    hessian.d2.lapse[1][1] = 0.02;
    hessian.d2.lapse[2][2] = 0.11;
    AnalyticInput hessian_base = hessian;
    for (int i = 0; i < dimension; ++i)
    {
        hessian_base.d1.lapse[i] = 0.0;
        for (int j = 0; j < dimension; ++j)
        {
            hessian_base.d2.lapse[i][j] = 0.0;
        }
    }
    direct = direct_rhs(hessian, zero_damping_access);
    const auto direct_hessian_base =
        direct_rhs(hessian_base, zero_damping_access);
    custom = custom_d3_families(hessian, 0.0, 0.0);
    stats.k_lapse_hessian.observe(
        direct.K - direct_hessian_base.K, custom.k_lapse_hessian,
        hessian.name);
    compare_matrix(stats.a_lapse_hessian,
                   subtract(from_gr(direct.A),
                            from_gr(direct_hessian_base.A)),
                   custom.a_lapse_hessian, hessian.name);

    AnalyticInput k_advection = make_flat_input("K_advection");
    k_advection.advec.K = -0.083;
    direct = direct_rhs(k_advection, zero_damping_access);
    custom = custom_d3_families(k_advection, 0.0, 0.0);
    stats.k_advection.observe(direct.K, custom.k_advection,
                              k_advection.name);

    AnalyticInput theta_k2_a2 = make_flat_input("Theta_K2_A2");
    theta_k2_a2.vars.lapse = 1.12;
    theta_k2_a2.vars.K = 0.36;
    set_test_a(theta_k2_a2);
    direct = direct_rhs(theta_k2_a2, zero_damping_access);
    custom = custom_d3_families(theta_k2_a2, 0.0, 0.0);
    stats.theta_k2_a2.observe(direct.Theta, custom.theta_k2_a2,
                              theta_k2_a2.name);

    AnalyticInput theta_cross = make_flat_input("Theta_minus_KTheta");
    theta_cross.vars.lapse = 1.09;
    theta_cross.vars.K = 0.41;
    theta_cross.vars.Theta = -0.14;
    AnalyticInput theta_cross_base = theta_cross;
    theta_cross_base.vars.Theta = 0.0;
    const auto direct_theta_cross =
        direct_rhs(theta_cross, zero_damping_access);
    const auto direct_theta_base =
        direct_rhs(theta_cross_base, zero_damping_access);
    custom = custom_d3_families(theta_cross, 0.0, 0.0);
    stats.theta_minus_k_theta.observe(
        direct_theta_cross.Theta - direct_theta_base.Theta,
        custom.theta_minus_k_theta, theta_cross.name);

    AnalyticInput lapse_z = make_geometry_case("Theta_lapse_gradient_Z", -1.0);
    lapse_z.vars.lapse = 1.18;
    lapse_z.d1.lapse[0] = 0.08;
    lapse_z.d1.lapse[1] = -0.03;
    lapse_z.d1.lapse[2] = -0.06;
    AnalyticInput lapse_z_base = lapse_z;
    for (int i = 0; i < dimension; ++i)
    {
        lapse_z_base.d1.lapse[i] = 0.0;
    }
    const auto direct_lapse_z =
        direct_rhs(lapse_z, zero_damping_access);
    const auto direct_lapse_z_base =
        direct_rhs(lapse_z_base, zero_damping_access);
    custom = custom_d3_families(lapse_z, 0.0, 0.0);
    stats.theta_lapse_gradient_z.observe(
        direct_lapse_z.Theta - direct_lapse_z_base.Theta,
        custom.theta_lapse_gradient_z, lapse_z.name);

    AnalyticInput theta_advection = make_flat_input("Theta_advection");
    theta_advection.advec.Theta = 0.067;
    direct = direct_rhs(theta_advection, zero_damping_access);
    custom = custom_d3_families(theta_advection, 0.0, 0.0);
    stats.theta_advection.observe(
        direct.Theta, custom.theta_advection, theta_advection.name);

    AnalyticInput a_linear = make_flat_input("A_linear");
    a_linear.vars.lapse = 1.15;
    a_linear.vars.K = 0.34;
    a_linear.vars.Theta = -0.11;
    set_test_a(a_linear, 1.0);
    AnalyticInput a_linear_negative = a_linear;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            a_linear_negative.vars.A[i][j] *= -1.0;
        }
    }
    const auto direct_a_positive =
        direct_rhs(a_linear, zero_damping_access);
    const auto direct_a_negative =
        direct_rhs(a_linear_negative, zero_damping_access);
    custom = custom_d3_families(a_linear, 0.0, 0.0);
    compare_matrix(
        stats.a_linear,
        scale(subtract(from_gr(direct_a_positive.A),
                       from_gr(direct_a_negative.A)),
              0.5),
        custom.a_linear, a_linear.name);

    AnalyticInput a_quadratic = make_flat_input("A_quadratic");
    a_quadratic.vars.lapse = 1.13;
    set_test_a(a_quadratic);
    direct = direct_rhs(a_quadratic, zero_damping_access);
    custom = custom_d3_families(a_quadratic, 0.0, 0.0);
    compare_matrix(stats.a_quadratic, from_gr(direct.A),
                   custom.a_quadratic, a_quadratic.name);

    AnalyticInput a_advection = make_flat_input("A_advection");
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            a_advection.advec.A[i][j] =
                -0.009 * (i + 1) * (j + 1);
        }
    }
    direct = direct_rhs(a_advection, zero_damping_access);
    custom = custom_d3_families(a_advection, 0.0, 0.0);
    compare_matrix(stats.a_advection, from_gr(direct.A),
                   custom.a_advection, a_advection.name);

    AnalyticInput a_shift = make_flat_input("A_shift_total");
    a_shift.vars.lapse = 1.07;
    set_test_a(a_shift);
    a_shift.d1.shift[0][0] = 0.07;
    a_shift.d1.shift[0][2] = -0.045;
    a_shift.d1.shift[1][1] = -0.025;
    a_shift.d1.shift[2][0] = 0.035;
    a_shift.d1.shift[2][2] = 0.08;
    AnalyticInput a_shift_base = a_shift;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            a_shift_base.d1.shift[i][j] = 0.0;
        }
    }
    const auto direct_a_shift =
        direct_rhs(a_shift, zero_damping_access);
    const auto direct_a_shift_base =
        direct_rhs(a_shift_base, zero_damping_access);
    custom = custom_d3_families(a_shift, 0.0, 0.0);
    compare_matrix(
        stats.a_shift_combined,
        subtract(from_gr(direct_a_shift.A),
                 from_gr(direct_a_shift_base.A)),
        add(custom.a_shift_stretching, custom.a_trace_correction),
        a_shift.name);

    const auto direct_a_damped = direct_rhs(a_shift, locked_access);
    const auto direct_a_undamped =
        direct_rhs(a_shift, zero_damping_access);
    compare_matrix(stats.a_no_damping,
                   subtract(from_gr(direct_a_damped.A),
                            from_gr(direct_a_undamped.A)),
                   custom.a_damping, "A_no_direct_damping");

    return stats;
}

bool mismatch_detected(const Statistics &statistics)
{
    return !statistics.passed && statistics.max_normalized > 1.0;
}

struct MutationResults
{
    bool omit_encoded_z = false;
    bool double_encoded_z = false;
    bool wrong_z_lowering = false;
    bool wrong_z_conformal_factor = false;
    bool wrong_trace_free_dimension = false;
    bool rhs_family_omission = false;
    bool rhs_family_duplication = false;
    bool rhs_sign = false;
    bool rhs_dimension = false;
    bool bssn_branch = false;

    bool passed() const
    {
        return omit_encoded_z && double_encoded_z && wrong_z_lowering &&
               wrong_z_conformal_factor && wrong_trace_free_dimension &&
               rhs_family_omission && rhs_family_duplication && rhs_sign &&
               rhs_dimension && bssn_branch;
    }
};

MutationResults run_mutations(const AnalyticInput &geometry_input,
                              const AnalyticInput &mixed_input,
                              const DirectCCZ4Access &locked_access,
                              const DirectCCZ4Access &zero_damping_access,
                              const DirectCCZ4Access &bssn_access)
{
    MutationResults result;
    const GeometryPieces direct_geometry =
        direct_geometry_pieces(geometry_input);
    const GeometryPieces custom_geometry =
        independent_geometry_pieces(geometry_input);

    Statistics omit;
    compare_matrix(omit, direct_geometry.combined.tensor,
                   custom_geometry.raw.tensor, "omit_encoded_Z");
    result.omit_encoded_z = mismatch_detected(omit);

    Statistics duplicate;
    compare_matrix(
        duplicate, direct_geometry.combined.tensor,
        add(custom_geometry.raw.tensor,
            scale(custom_geometry.encoded.tensor, 2.0)),
        "double_encoded_Z");
    result.double_encoded_z = mismatch_detected(duplicate);

    // These two controls discriminate the local standalone reconstruction
    // from the inspected source convention.  They are not mutations of a
    // public GRChombo Z-vector output.
    Statistics lowering;
    for (int i = 0; i < dimension; ++i)
    {
        double wrong_lower = 0.0;
        for (int j = 0; j < dimension; ++j)
        {
            wrong_lower += geometry_input.vars.h[i][j] *
                           custom_geometry.z_upper[j];
        }
        lowering.observe(direct_geometry.z_lower[i], wrong_lower,
                         "wrong_Z_lowering[" + std::to_string(i) + "]");
    }
    result.wrong_z_lowering = mismatch_detected(lowering);

    Statistics conformal_factor;
    for (int i = 0; i < dimension; ++i)
    {
        conformal_factor.observe(
            direct_geometry.z_upper[i], custom_geometry.z_over_chi[i],
            "missing_chi_in_Z_upper[" + std::to_string(i) + "]");
    }
    result.wrong_z_conformal_factor = mismatch_detected(conformal_factor);

    const Matrix h = input_matrix(geometry_input.vars.h);
    const Matrix h_inverse = inverse(h);
    Matrix wrong_tf = custom_geometry.encoded.tensor;
    const double encoded_trace =
        contract(custom_geometry.encoded.tensor, h_inverse);
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            wrong_tf[i][j] -= h[i][j] * encoded_trace / 4.0;
        }
    }
    Statistics dimension_tf;
    compare_matrix(dimension_tf, direct_geometry.encoded_trace_free,
                   wrong_tf, "wrong_d4_TF");
    result.wrong_trace_free_dimension = mismatch_detected(dimension_tf);

    const Vars<double> direct_mixed = direct_rhs(mixed_input, locked_access);
    const RHSFamilies custom = custom_d3_families(mixed_input);
    Statistics omission;
    omission.observe(direct_mixed.K,
                     custom.k_total - custom.k_damping,
                     "omit_K_damping");
    result.rhs_family_omission = mismatch_detected(omission);

    Statistics rhs_duplicate;
    rhs_duplicate.observe(direct_mixed.Theta,
                          custom.theta_total + custom.theta_ricci_z,
                          "duplicate_Theta_Ricci");
    result.rhs_family_duplication = mismatch_detected(rhs_duplicate);

    Statistics sign;
    sign.observe(direct_mixed.K,
                 custom.k_total - 2.0 * custom.k_lapse_hessian,
                 "flip_K_lapse_hessian");
    result.rhs_sign = mismatch_detected(sign);

    Statistics rhs_dimension;
    const double wrong_d4_chi =
        mixed_input.advec.chi +
        0.5 * mixed_input.vars.chi *
            (mixed_input.vars.lapse * mixed_input.vars.K -
             (mixed_input.d1.shift[0][0] +
              mixed_input.d1.shift[1][1] +
              mixed_input.d1.shift[2][2]));
    rhs_dimension.observe(direct_mixed.chi, wrong_d4_chi,
                          "wrong_d4_chi_coefficient");
    result.rhs_dimension = mismatch_detected(rhs_dimension);

    const Vars<double> direct_ccz4_zero_damping =
        direct_rhs(mixed_input, zero_damping_access);
    const Vars<double> direct_bssn = direct_rhs(mixed_input, bssn_access);
    Statistics branch;
    branch.observe(direct_ccz4_zero_damping.K, direct_bssn.K,
                   "CCZ4_vs_BSSN_K");
    branch.observe(direct_ccz4_zero_damping.Theta, direct_bssn.Theta,
                   "CCZ4_vs_BSSN_Theta");
    result.bssn_branch = mismatch_detected(branch);
    return result;
}

void print_statistics(const std::string &name,
                      const Statistics &statistics,
                      const std::string &classification =
                          "DIRECT_COMPILED_GRCHOMBO")
{
    std::cout << "RESULT " << name
              << " classification=" << classification
              << " max_abs=" << statistics.max_absolute
              << " max_normalized=" << statistics.max_normalized
              << " worst_abs=" << statistics.worst_absolute
              << " worst_normalized=" << statistics.worst_normalized
              << " observations=" << statistics.observations
              << " status=" << (statistics.passed ? "PASS" : "MISMATCH")
              << '\n';
}

void print_geometry_statistics(const GeometryStatistics &stats)
{
    print_statistics("raw_Ricci_tensor", stats.raw_tensor);
    print_statistics("raw_Ricci_scalar", stats.raw_scalar);
    print_statistics("encoded_Z_tensor", stats.encoded_tensor);
    print_statistics("encoded_Z_scalar", stats.encoded_scalar);
    print_statistics("encoded_Z_trace_free", stats.encoded_trace_free);
    print_statistics("combined_Ricci_Z_tensor", stats.combined_tensor);
    print_statistics("combined_Ricci_Z_scalar", stats.combined_scalar);
    print_statistics("Z_index_conversions", stats.z_mapping,
                     "SOURCE_CONVENTION_LOCAL_RECONSTRUCTION");
}

void print_family_statistics(const FamilyStatistics &stats)
{
    print_statistics("chi.lapse_K", stats.chi_lapse_k);
    print_statistics("chi.shift_divergence", stats.chi_shift_divergence);
    print_statistics("chi.advection", stats.chi_advection);
    print_statistics("h.algebraic", stats.h_algebraic);
    print_statistics("h.advection", stats.h_advection);
    print_statistics("h.shift_stretching_plus_trace_correction",
                     stats.h_shift_combined);
    std::cout << "RESULT h.shift_stretching classification=SOURCE_ONLY\n";
    std::cout << "RESULT h.trace_divergence_correction "
                 "classification=SOURCE_ONLY\n";
    print_statistics("K.Ricci_Z", stats.k_ricci_z);
    print_statistics("K.K_K_minus_2Theta", stats.k_nonlinear);
    print_statistics("K.damping", stats.k_damping);
    print_statistics("K.lapse_Hessian", stats.k_lapse_hessian);
    print_statistics("K.advection", stats.k_advection);
    std::cout << "RESULT K.matter_Lambda_zero "
                 "classification=SOURCE_ONLY\n";
    print_statistics("Theta.half_Ricci_Z", stats.theta_ricci_z);
    print_statistics("Theta.K2_A2", stats.theta_k2_a2);
    print_statistics("Theta.minus_K_Theta", stats.theta_minus_k_theta);
    print_statistics("Theta.damping", stats.theta_damping);
    print_statistics("Theta.lapse_gradient_Z",
                     stats.theta_lapse_gradient_z);
    print_statistics("Theta.advection", stats.theta_advection);
    std::cout << "RESULT Theta.matter_Lambda_zero "
                 "classification=SOURCE_ONLY\n";
    print_statistics("A.trace_free_Ricci_Z", stats.a_ricci_z);
    print_statistics("A.lapse_Hessian", stats.a_lapse_hessian);
    print_statistics("A.K_minus_2Theta_linear", stats.a_linear);
    print_statistics("A.quadratic", stats.a_quadratic);
    print_statistics("A.advection", stats.a_advection);
    print_statistics("A.shift_stretching_plus_trace_correction",
                     stats.a_shift_combined);
    std::cout << "RESULT A.shift_stretching classification=SOURCE_ONLY\n";
    std::cout << "RESULT A.trace_divergence_correction "
                 "classification=SOURCE_ONLY\n";
    print_statistics("A.direct_damping_absent", stats.a_no_damping);
}

void print_combined_statistics(const CombinedRHSStatistics &stats)
{
    print_statistics("combined.chi", stats.chi);
    print_statistics("combined.h", stats.h);
    print_statistics("combined.K", stats.k);
    print_statistics("combined.Theta", stats.theta);
    print_statistics("combined.A", stats.a);
}
} // namespace

int main()
{
    std::cout << std::scientific << std::setprecision(17);
    std::cout << "FORMULATION implementation=GRChombo_CCZ4RHS_rhs_equation"
              << " formulation=USE_CCZ4"
              << " GR_SPACEDIM=" << GR_SPACEDIM
              << " CH_SPACEDIM=" << CH_SPACEDIM
              << " Lambda=" << locked_cosmological_constant
              << " kappa1=" << locked_kappa1
              << " kappa2=" << locked_kappa2
              << " kappa3=" << locked_kappa3
              << " covariantZ4=" << (locked_covariant_z4 ? "true" : "false")
              << " matter=vacuum"
              << " gauge=frozen_test_adapter"
              << " lapse_shift=supplied_analytic_inputs"
              << " gauge_outputs=not_compared"
              << " custom_production_d4=unchanged\n";

    const DirectCCZ4Access locked_access(
        locked_kappa1, locked_kappa2, locked_kappa3,
        locked_covariant_z4, DirectCCZ4Access::USE_CCZ4);
    const DirectCCZ4Access zero_damping_access(
        0.0, 0.0, locked_kappa3, locked_covariant_z4,
        DirectCCZ4Access::USE_CCZ4);
    const DirectCCZ4Access bssn_access(
        0.0, 0.0, 0.0, locked_covariant_z4,
        DirectCCZ4Access::USE_BSSN);

    const std::vector<AnalyticInput> geometry_cases = {
        make_geometry_case("geometry_positive", 1.0),
        make_geometry_case("geometry_negative", -1.0)};
    const std::vector<AnalyticInput> mixed_cases = {
        make_mixed_rhs_case("mixed_positive", 1.0),
        make_mixed_rhs_case("mixed_negative", -1.0)};

    const GeometryStatistics geometry =
        run_geometry_comparisons(geometry_cases);
    const FamilyStatistics families =
        run_family_comparisons(locked_access, zero_damping_access);
    const CombinedRHSStatistics combined =
        run_combined_rhs_comparison(mixed_cases, locked_access);
    const MutationResults mutations =
        run_mutations(geometry_cases.front(), mixed_cases.front(),
                      locked_access, zero_damping_access, bssn_access);

    print_geometry_statistics(geometry);
    print_family_statistics(families);
    print_combined_statistics(combined);
    std::cout << "MUTATION omit_encoded_Z="
              << (mutations.omit_encoded_z ? "DETECTED" : "MISSED")
              << " double_encoded_Z="
              << (mutations.double_encoded_z ? "DETECTED" : "MISSED")
              << " source_convention_wrong_Z_lowering="
              << (mutations.wrong_z_lowering ? "DETECTED" : "MISSED")
              << " source_convention_missing_chi_in_Z_upper="
              << (mutations.wrong_z_conformal_factor ? "DETECTED" : "MISSED")
              << " wrong_TF_dimension="
              << (mutations.wrong_trace_free_dimension ? "DETECTED"
                                                       : "MISSED")
              << " family_omission="
              << (mutations.rhs_family_omission ? "DETECTED" : "MISSED")
              << " family_duplication="
              << (mutations.rhs_family_duplication ? "DETECTED" : "MISSED")
              << " family_sign="
              << (mutations.rhs_sign ? "DETECTED" : "MISSED")
              << " RHS_dimension="
              << (mutations.rhs_dimension ? "DETECTED" : "MISSED")
              << " BSSN_branch="
              << (mutations.bssn_branch ? "DETECTED" : "MISSED") << '\n';
    std::cout << "CLASSIFICATION"
              << " direct=compute_ricci_compute_ricci_Z_rhs_equation"
              << " source_only=Z_index_conversions_from_direct_contracted_connection"
              << ",separate_shift_stretching_trace_correction"
              << " custom_hidden_only=hww_Aww_and_hidden_cartoon_terms"
              << " blocked=Chombo_grid_execution_not_required\n";

    const bool passed = geometry.passed() && families.passed() &&
                        combined.passed() && mutations.passed();
    std::cout << "BATCH2 " << (passed ? "PASS" : "FAIL") << '\n';
    return passed ? 0 : 1;
}
