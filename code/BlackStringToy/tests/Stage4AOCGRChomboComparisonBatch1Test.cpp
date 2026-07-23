#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "Stage4AOFrozenGaugeContractedConnection.hpp"
#include "Stage4AOFrozenGaugeOperator.hpp"

#define BLACKSTRINGTOY_LAYOUT_TEST_ONLY
#include "UserVariables.hpp"
#undef BLACKSTRINGTOY_LAYOUT_TEST_ONLY

namespace StockGRChomboLayout
{
#include "CCZ4UserVariables.hpp"
}

#include "CCZ4Geometry.hpp"
#include "TensorAlgebra.hpp"

namespace
{
constexpr int dimension = 3;
constexpr int x_dir = 0;
constexpr int y_dir = 1;
constexpr int z_dir = 2;
constexpr double absolute_tolerance = 5.0e-13;
constexpr double relative_tolerance = 5.0e-12;

static_assert(CH_SPACEDIM == dimension,
              "batch-1 bridge must compile the inspected GRChombo path in 3D");
static_assert(GR_SPACEDIM == dimension,
              "batch-1 comparison must use the GRChombo physical dimension");
static_assert(DEFAULT_TENSOR_DIM == dimension,
              "batch-1 tensor loops must cover the compiled GRChombo state");
static_assert(
    BlackStringToy::ConformalCartoonAlgebra::physical_spatial_dim == 4,
    "the production custom oracle must remain d=4");
static_assert(
    BlackStringToy::ConformalCartoonAlgebra::gridded_spatial_dim == 2,
    "the production custom oracle must remain a two-coordinate reduction");
static_assert(
    BlackStringToy::ConformalCartoonAlgebra::hidden_multiplicity == 2,
    "the production custom oracle must retain two hidden directions");

using Matrix = std::array<std::array<double, dimension>, dimension>;
using Derivative1 = std::array<Matrix, dimension>;
using Derivative2 = std::array<std::array<Matrix, dimension>, dimension>;
using Vector = std::array<double, dimension>;

struct Statistics
{
    double max_absolute = 0.0;
    double max_normalized = 0.0;
    std::string worst_absolute;
    std::string worst_normalized;
    bool passed = true;

    void observe(const double a, const double b, const std::string &label)
    {
        const double absolute = std::abs(a - b);
        const double allowance =
            absolute_tolerance +
            relative_tolerance * std::max(std::abs(a), std::abs(b));
        const double normalized = absolute / allowance;
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

Matrix identity()
{
    Matrix out{};
    for (int i = 0; i < dimension; ++i)
    {
        out[i][i] = 1.0;
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
        const double scale = augmented[column][column];
        for (double &entry : augmented[column])
        {
            entry /= scale;
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
                augmented[row][j] -= multiplier * augmented[column][j];
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

double determinant(Matrix input)
{
    double result = 1.0;
    int parity = 1;
    for (int column = 0; column < dimension; ++column)
    {
        int pivot = column;
        for (int row = column + 1; row < dimension; ++row)
        {
            if (std::abs(input[row][column]) >
                std::abs(input[pivot][column]))
            {
                pivot = row;
            }
        }
        if (pivot != column)
        {
            std::swap(input[pivot], input[column]);
            parity = -parity;
        }
        const double diagonal = input[column][column];
        result *= diagonal;
        for (int row = column + 1; row < dimension; ++row)
        {
            const double multiplier = input[row][column] / diagonal;
            for (int j = column + 1; j < dimension; ++j)
            {
                input[row][j] -= multiplier * input[column][j];
            }
        }
    }
    return static_cast<double>(parity) * result;
}

double trace(const Matrix &tensor, const Matrix &inverse_metric)
{
    double out = 0.0;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out += inverse_metric[i][j] * tensor[i][j];
        }
    }
    return out;
}

Tensor<2, double> to_gr_tensor(const Matrix &input)
{
    Tensor<2, double> out;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            out[i][j] = input[i][j];
        }
    }
    return out;
}

Matrix from_gr_tensor(const Tensor<2, double> &input)
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

void compare_matrices(Statistics &stats, const Matrix &a, const Matrix &b,
                      const std::string &prefix)
{
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            stats.observe(a[i][j], b[i][j],
                          prefix + "[" + std::to_string(i) + "," +
                              std::to_string(j) + "]");
        }
    }
}

Statistics run_conformal_algebra_comparison()
{
    const std::vector<Matrix> metrics = {
        {{{1.18, 0.0, 0.16}, {0.0, 0.91, 0.0}, {0.16, 0.0, 1.07}}},
        {{{1.31, -0.12, 0.09}, {-0.12, 1.04, 0.07}, {0.09, 0.07, 0.88}}},
        {{{0.82, 0.11, -0.06}, {0.11, 1.27, 0.13}, {-0.06, 0.13, 1.19}}}};
    const std::vector<Matrix> tensors = {
        {{{0.37, -0.14, 0.08}, {-0.14, -0.22, 0.19}, {0.08, 0.19, 0.41}}},
        {{{-0.28, 0.17, -0.09}, {0.17, 0.33, 0.06}, {-0.09, 0.06, 0.24}}},
        {{{0.46, 0.05, 0.12}, {0.05, -0.31, -0.07}, {0.12, -0.07, 0.29}}}};

    Statistics stats;
    for (std::size_t case_index = 0; case_index < metrics.size(); ++case_index)
    {
        const Matrix &h = metrics[case_index];
        const Matrix &a = tensors[case_index];
        const Matrix h_inverse = inverse(h);
        const auto gr_h = to_gr_tensor(h);
        const auto gr_a = to_gr_tensor(a);
        const auto gr_inverse = TensorAlgebra::compute_inverse_sym(gr_h);
        const std::string label = "metric_case_" + std::to_string(case_index);

        stats.observe(TensorAlgebra::compute_determinant_sym(gr_h),
                      determinant(h), label + ".determinant");
        compare_matrices(stats, from_gr_tensor(gr_inverse), h_inverse,
                         label + ".inverse");
        stats.observe(TensorAlgebra::compute_trace(gr_a, gr_inverse),
                      trace(a, h_inverse), label + ".trace");

        auto gr_tf = gr_a;
        TensorAlgebra::make_trace_free(gr_tf, gr_h, gr_inverse);
        Matrix expected_tf = a;
        const double tr = trace(a, h_inverse);
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                expected_tf[i][j] -= h[i][j] * tr / dimension;
            }
        }
        compare_matrices(stats, from_gr_tensor(gr_tf), expected_tf,
                         label + ".trace_free");

        const Vector covector = {0.21 + 0.03 * case_index,
                                 -0.17 + 0.02 * case_index,
                                 0.32 - 0.01 * case_index};
        Tensor<1, double> gr_covector;
        for (int i = 0; i < dimension; ++i)
        {
            gr_covector[i] = covector[i];
        }
        const auto raised = TensorAlgebra::raise_all(gr_covector, gr_inverse);
        const auto lowered = TensorAlgebra::lower_all(raised, gr_h);
        for (int i = 0; i < dimension; ++i)
        {
            double expected_raised = 0.0;
            for (int j = 0; j < dimension; ++j)
            {
                expected_raised += h_inverse[i][j] * covector[j];
            }
            stats.observe(raised[i], expected_raised,
                          label + ".raise[" + std::to_string(i) + "]");
            stats.observe(lowered[i], covector[i],
                          label + ".lower[" + std::to_string(i) + "]");
        }
    }
    return stats;
}

bool conformal_dimension_mutation_detected()
{
    const Matrix h =
        {{{1.18, 0.0, 0.16}, {0.0, 0.91, 0.0}, {0.16, 0.0, 1.07}}};
    const Matrix a =
        {{{0.37, -0.14, 0.08}, {-0.14, -0.22, 0.19}, {0.08, 0.19, 0.41}}};
    const auto gr_h = to_gr_tensor(h);
    const auto gr_inverse = TensorAlgebra::compute_inverse_sym(gr_h);
    auto gr_tf = to_gr_tensor(a);
    TensorAlgebra::make_trace_free(gr_tf, gr_h, gr_inverse);
    const double tr = trace(a, inverse(h));
    Statistics mutation;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            const double wrong_d4 = a[i][j] - h[i][j] * tr / 4.0;
            mutation.observe(gr_tf[i][j], wrong_d4, "wrong_d4_trace");
        }
    }
    return !mutation.passed && mutation.max_normalized > 1000.0;
}

struct ConnectionCase
{
    std::string name;
    double x;
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
    double hat_gamma_x;
    double hat_gamma_z;
};

Vector grchombo_linearized_contracted_connection(const ConnectionCase &input)
{
    constexpr double epsilon = 1.0e-6;
    Vector result{};
    std::array<Vector, 2> evaluations{};
    for (int side = 0; side < 2; ++side)
    {
        const double sign = side == 0 ? -1.0 : 1.0;
        Tensor<2, double> h;
        Tensor<2, Tensor<1, double>> d1_h;
        for (int i = 0; i < dimension; ++i)
        {
            for (int j = 0; j < dimension; ++j)
            {
                h[i][j] = i == j ? 1.0 : 0.0;
                for (int derivative = 0; derivative < dimension; ++derivative)
                {
                    d1_h[i][j][derivative] = 0.0;
                }
            }
        }
        h[x_dir][x_dir] += sign * epsilon * input.h_xx;
        h[x_dir][z_dir] += sign * epsilon * input.h_xz;
        h[z_dir][x_dir] = h[x_dir][z_dir];
        h[z_dir][z_dir] += sign * epsilon * input.h_zz;
        d1_h[x_dir][x_dir][x_dir] =
            sign * epsilon * input.dx_h_xx;
        d1_h[x_dir][z_dir][x_dir] =
            sign * epsilon * input.dx_h_xz;
        d1_h[z_dir][x_dir][x_dir] =
            sign * epsilon * input.dx_h_xz;
        d1_h[z_dir][z_dir][x_dir] =
            sign * epsilon * input.dx_h_zz;
        d1_h[x_dir][x_dir][z_dir] =
            sign * epsilon * input.dz_h_xx;
        d1_h[x_dir][z_dir][z_dir] =
            sign * epsilon * input.dz_h_xz;
        d1_h[z_dir][x_dir][z_dir] =
            sign * epsilon * input.dz_h_xz;
        d1_h[z_dir][z_dir][z_dir] =
            sign * epsilon * input.dz_h_zz;

        const auto h_inverse = TensorAlgebra::compute_inverse_sym(h);
        const auto chris =
            TensorAlgebra::compute_christoffel(d1_h, h_inverse);
        for (int i = 0; i < dimension; ++i)
        {
            evaluations[side][i] = chris.contracted[i];
        }
    }
    for (int i = 0; i < dimension; ++i)
    {
        result[i] =
            (evaluations[1][i] - evaluations[0][i]) / (2.0 * epsilon);
    }
    return result;
}

Statistics run_connection_comparison()
{
    const std::vector<ConnectionCase> cases = {
        {"diagonal", 5.0, 0.17, 0.0, -0.09, 0.04, 0.23, 0.0, -0.31,
         0.07, -0.11, 0.0, 0.19, -0.05, 0.0, 0.0},
        {"off_diagonal", 7.0, -0.08, 0.14, 0.12, -0.03, 0.0, -0.21, 0.0,
         0.04, 0.0, 0.27, 0.0, -0.06, 0.0, 0.0},
        {"pure_hat_gamma", 11.0, 0.0, 0.0, 0.0, 0.09, 0.0, 0.0, 0.0,
         -0.02, 0.0, 0.0, 0.0, 0.03, 0.41, -0.28},
        {"mixed", 13.0, 0.13, -0.07, -0.16, 0.11, -0.18, 0.22, 0.29,
         -0.12, 0.31, -0.26, 0.15, 0.08, -0.34, 0.27}};

    namespace Custom =
        BlackStringToy::Stage4AOFrozenGaugeContractedConnection;
    Statistics stats;
    for (const auto &input : cases)
    {
        const auto custom_jet = Custom::make_perturbation_jet(
            input.x, input.h_xx, input.h_xz, input.h_zz, input.h_ww,
            input.dx_h_xx, input.dx_h_xz, input.dx_h_zz, input.dx_h_ww,
            input.dz_h_xx, input.dz_h_xz, input.dz_h_zz, input.dz_h_ww,
            input.hat_gamma_x, input.hat_gamma_z);
        const auto custom =
            Custom::compute_contracted_connection_and_z(custom_jet);
        const double hidden_x =
            -input.dx_h_ww +
            2.0 * (input.h_xx - input.h_ww) / input.x;
        const double hidden_z =
            2.0 * input.h_xz / input.x - input.dz_h_ww;
        const double visible_x = custom.g_x() - hidden_x;
        const double visible_z = custom.g_z() - hidden_z;
        const Vector gr = grchombo_linearized_contracted_connection(input);
        const std::string prefix = input.name + ".";
        stats.observe(gr[x_dir], visible_x, prefix + "Gamma_x");
        stats.observe(gr[z_dir], visible_z, prefix + "Gamma_z");
        stats.observe(0.5 * (input.hat_gamma_x - gr[x_dir]),
                      0.5 * (input.hat_gamma_x - visible_x),
                      prefix + "Z_over_chi_x");
        stats.observe(0.5 * (input.hat_gamma_z - gr[z_dir]),
                      0.5 * (input.hat_gamma_z - visible_z),
                      prefix + "Z_over_chi_z");
    }
    return stats;
}

bool connection_sign_mutation_detected()
{
    const ConnectionCase input = {"mutation", 9.0, 0.11, -0.08, -0.07,
                                  0.04,       0.21, -0.19, 0.13,  0.02,
                                  -0.17,      0.23, 0.09, -0.03, 0.0,
                                  0.0};
    const Vector gr = grchombo_linearized_contracted_connection(input);
    const double expected_x =
        0.5 * input.dx_h_xx - 0.5 * input.dx_h_zz + input.dz_h_xz;
    Statistics mutation;
    mutation.observe(-gr[x_dir], expected_x, "contracted_connection_sign");
    return !mutation.passed && mutation.max_normalized > 1000.0;
}

template <typename data_t> struct DirectVars
{
    data_t chi;
    Tensor<2, data_t> h;
    Tensor<1, data_t> Gamma;
};

template <typename data_t> struct DirectDiff2Vars
{
    data_t chi;
    Tensor<2, data_t> h;
};

struct ConformalJet
{
    std::string name;
    double chi = 1.0;
    Vector d1_chi{};
    Matrix d2_chi{};
    Matrix h = identity();
    Derivative1 d1_h{};
    Derivative2 d2_h{};
};

void complete_determinant_one_hidden_component(ConformalJet &jet)
{
    const double a = jet.h[x_dir][x_dir];
    const double b = jet.h[x_dir][z_dir];
    const double c = jet.h[z_dir][z_dir];
    const double determinant_xz = a * c - b * b;
    jet.h[y_dir][y_dir] = 1.0 / determinant_xz;
    for (int derivative = 0; derivative < dimension; ++derivative)
    {
        const double da = jet.d1_h[derivative][x_dir][x_dir];
        const double db = jet.d1_h[derivative][x_dir][z_dir];
        const double dc = jet.d1_h[derivative][z_dir][z_dir];
        const double ddet = da * c + a * dc - 2.0 * b * db;
        jet.d1_h[derivative][y_dir][y_dir] =
            -ddet / (determinant_xz * determinant_xz);
    }
    for (int first = 0; first < dimension; ++first)
    {
        for (int second = 0; second < dimension; ++second)
        {
            const double da_first =
                jet.d1_h[first][x_dir][x_dir];
            const double db_first =
                jet.d1_h[first][x_dir][z_dir];
            const double dc_first =
                jet.d1_h[first][z_dir][z_dir];
            const double da_second =
                jet.d1_h[second][x_dir][x_dir];
            const double db_second =
                jet.d1_h[second][x_dir][z_dir];
            const double dc_second =
                jet.d1_h[second][z_dir][z_dir];
            const double ddet_first =
                da_first * c + a * dc_first - 2.0 * b * db_first;
            const double ddet_second =
                da_second * c + a * dc_second - 2.0 * b * db_second;
            const double dda =
                jet.d2_h[first][second][x_dir][x_dir];
            const double ddb =
                jet.d2_h[first][second][x_dir][z_dir];
            const double ddc =
                jet.d2_h[first][second][z_dir][z_dir];
            const double d2det =
                dda * c + da_first * dc_second +
                da_second * dc_first + a * ddc -
                2.0 * (db_first * db_second + b * ddb);
            jet.d2_h[first][second][y_dir][y_dir] =
                2.0 * ddet_first * ddet_second /
                    (determinant_xz * determinant_xz * determinant_xz) -
                d2det / (determinant_xz * determinant_xz);
        }
    }
}

void set_symmetric_metric_entry(Matrix &matrix, const int i, const int j,
                                const double value)
{
    matrix[i][j] = value;
    matrix[j][i] = value;
}

void set_symmetric_first_entry(Derivative1 &derivative, const int direction,
                               const int i, const int j, const double value)
{
    derivative[direction][i][j] = value;
    derivative[direction][j][i] = value;
}

void set_symmetric_second_entry(Derivative2 &derivative,
                                const int first_direction,
                                const int second_direction, const int i,
                                const int j, const double value)
{
    derivative[first_direction][second_direction][i][j] = value;
    derivative[first_direction][second_direction][j][i] = value;
    derivative[second_direction][first_direction][i][j] = value;
    derivative[second_direction][first_direction][j][i] = value;
}

std::vector<ConformalJet> make_ricci_cases()
{
    ConformalJet flat;
    flat.name = "flat";

    ConformalJet diagonal;
    diagonal.name = "diagonal_curved";
    diagonal.h = {};
    diagonal.h[x_dir][x_dir] = 1.14;
    diagonal.h[z_dir][z_dir] = 0.93;
    set_symmetric_first_entry(diagonal.d1_h, x_dir, x_dir, x_dir, 0.17);
    set_symmetric_first_entry(diagonal.d1_h, x_dir, z_dir, z_dir, -0.11);
    set_symmetric_first_entry(diagonal.d1_h, z_dir, x_dir, x_dir, 0.08);
    set_symmetric_first_entry(diagonal.d1_h, z_dir, z_dir, z_dir, 0.14);
    set_symmetric_second_entry(diagonal.d2_h, x_dir, x_dir, x_dir, x_dir,
                               -0.09);
    set_symmetric_second_entry(diagonal.d2_h, x_dir, x_dir, z_dir, z_dir,
                               0.07);
    set_symmetric_second_entry(diagonal.d2_h, x_dir, z_dir, x_dir, x_dir,
                               0.05);
    set_symmetric_second_entry(diagonal.d2_h, x_dir, z_dir, z_dir, z_dir,
                               -0.04);
    set_symmetric_second_entry(diagonal.d2_h, z_dir, z_dir, x_dir, x_dir,
                               0.06);
    set_symmetric_second_entry(diagonal.d2_h, z_dir, z_dir, z_dir, z_dir,
                               -0.08);
    complete_determinant_one_hidden_component(diagonal);

    ConformalJet off_diagonal;
    off_diagonal.name = "off_diagonal";
    off_diagonal.h = {};
    off_diagonal.h[x_dir][x_dir] = 1.09;
    off_diagonal.h[z_dir][z_dir] = 1.03;
    set_symmetric_metric_entry(off_diagonal.h, x_dir, z_dir, 0.16);
    set_symmetric_first_entry(off_diagonal.d1_h, x_dir, x_dir, z_dir, -0.21);
    set_symmetric_first_entry(off_diagonal.d1_h, z_dir, x_dir, z_dir, 0.18);
    set_symmetric_second_entry(off_diagonal.d2_h, x_dir, x_dir, x_dir, z_dir,
                               0.13);
    set_symmetric_second_entry(off_diagonal.d2_h, x_dir, z_dir, x_dir, z_dir,
                               -0.12);
    set_symmetric_second_entry(off_diagonal.d2_h, z_dir, z_dir, x_dir, z_dir,
                               0.09);
    complete_determinant_one_hidden_component(off_diagonal);

    ConformalJet conformal;
    conformal.name = "conformal_factor";
    conformal.chi = 0.87;
    conformal.d1_chi = {0.12, 0.0, -0.09};
    conformal.d2_chi[x_dir][x_dir] = -0.07;
    conformal.d2_chi[x_dir][z_dir] = 0.04;
    conformal.d2_chi[z_dir][x_dir] = 0.04;
    conformal.d2_chi[z_dir][z_dir] = 0.11;

    ConformalJet mixed;
    mixed.name = "mixed";
    mixed.chi = 1.08;
    mixed.d1_chi = {-0.08, 0.0, 0.13};
    mixed.d2_chi[x_dir][x_dir] = 0.06;
    mixed.d2_chi[x_dir][z_dir] = -0.05;
    mixed.d2_chi[z_dir][x_dir] = -0.05;
    mixed.d2_chi[z_dir][z_dir] = -0.09;
    mixed.h = {};
    mixed.h[x_dir][x_dir] = 0.96;
    mixed.h[z_dir][z_dir] = 1.21;
    set_symmetric_metric_entry(mixed.h, x_dir, z_dir, -0.12);
    set_symmetric_first_entry(mixed.d1_h, x_dir, x_dir, x_dir, 0.14);
    set_symmetric_first_entry(mixed.d1_h, x_dir, x_dir, z_dir, -0.17);
    set_symmetric_first_entry(mixed.d1_h, x_dir, z_dir, z_dir, 0.09);
    set_symmetric_first_entry(mixed.d1_h, z_dir, x_dir, x_dir, -0.06);
    set_symmetric_first_entry(mixed.d1_h, z_dir, x_dir, z_dir, 0.15);
    set_symmetric_first_entry(mixed.d1_h, z_dir, z_dir, z_dir, -0.11);
    set_symmetric_second_entry(mixed.d2_h, x_dir, x_dir, x_dir, x_dir, 0.08);
    set_symmetric_second_entry(mixed.d2_h, x_dir, x_dir, x_dir, z_dir, -0.07);
    set_symmetric_second_entry(mixed.d2_h, x_dir, x_dir, z_dir, z_dir, 0.05);
    set_symmetric_second_entry(mixed.d2_h, x_dir, z_dir, x_dir, x_dir, -0.04);
    set_symmetric_second_entry(mixed.d2_h, x_dir, z_dir, x_dir, z_dir, 0.12);
    set_symmetric_second_entry(mixed.d2_h, x_dir, z_dir, z_dir, z_dir, -0.03);
    set_symmetric_second_entry(mixed.d2_h, z_dir, z_dir, x_dir, x_dir, 0.07);
    set_symmetric_second_entry(mixed.d2_h, z_dir, z_dir, x_dir, z_dir, 0.02);
    set_symmetric_second_entry(mixed.d2_h, z_dir, z_dir, z_dir, z_dir, -0.10);
    complete_determinant_one_hidden_component(mixed);

    return {flat, diagonal, off_diagonal, conformal, mixed};
}

struct RicciResult
{
    Matrix components{};
    double scalar = 0.0;
};

RicciResult independent_physical_ricci(const ConformalJet &jet)
{
    Matrix gamma{};
    Derivative1 d1_gamma{};
    Derivative2 d2_gamma{};
    const double inv_chi = 1.0 / jet.chi;
    const double inv_chi2 = inv_chi * inv_chi;
    const double inv_chi3 = inv_chi2 * inv_chi;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            gamma[i][j] = jet.h[i][j] * inv_chi;
            for (int first = 0; first < dimension; ++first)
            {
                d1_gamma[first][i][j] =
                    jet.d1_h[first][i][j] * inv_chi -
                    jet.h[i][j] * jet.d1_chi[first] * inv_chi2;
                for (int second = 0; second < dimension; ++second)
                {
                    d2_gamma[first][second][i][j] =
                        jet.d2_h[first][second][i][j] * inv_chi -
                        (jet.d1_h[first][i][j] * jet.d1_chi[second] +
                         jet.d1_h[second][i][j] * jet.d1_chi[first] +
                         jet.h[i][j] * jet.d2_chi[first][second]) *
                            inv_chi2 +
                        2.0 * jet.h[i][j] * jet.d1_chi[first] *
                            jet.d1_chi[second] * inv_chi3;
                }
            }
        }
    }

    const Matrix gamma_inverse = inverse(gamma);
    std::array<Matrix, dimension> d1_gamma_inverse{};
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
                        d1_gamma_inverse[derivative][i][j] -=
                            gamma_inverse[i][p] * d1_gamma[derivative][p][q] *
                            gamma_inverse[q][j];
                    }
                }
            }
        }
    }

    std::array<std::array<std::array<double, dimension>, dimension>,
               dimension>
        christoffel{};
    std::array<decltype(christoffel), dimension> d1_christoffel{};
    for (int upper = 0; upper < dimension; ++upper)
    {
        for (int lower_1 = 0; lower_1 < dimension; ++lower_1)
        {
            for (int lower_2 = 0; lower_2 < dimension; ++lower_2)
            {
                for (int contracted = 0; contracted < dimension; ++contracted)
                {
                    const double bracket =
                        d1_gamma[lower_1][contracted][lower_2] +
                        d1_gamma[lower_2][contracted][lower_1] -
                        d1_gamma[contracted][lower_1][lower_2];
                    christoffel[upper][lower_1][lower_2] +=
                        0.5 * gamma_inverse[upper][contracted] * bracket;
                    for (int derivative = 0; derivative < dimension;
                         ++derivative)
                    {
                        const double derivative_bracket =
                            d2_gamma[derivative][lower_1][contracted][lower_2] +
                            d2_gamma[derivative][lower_2][contracted][lower_1] -
                            d2_gamma[derivative][contracted][lower_1][lower_2];
                        d1_christoffel[derivative][upper][lower_1][lower_2] +=
                            0.5 *
                            (d1_gamma_inverse[derivative][upper][contracted] *
                                 bracket +
                             gamma_inverse[upper][contracted] *
                                 derivative_bracket);
                    }
                }
            }
        }
    }

    RicciResult out;
    for (int i = 0; i < dimension; ++i)
    {
        for (int j = 0; j < dimension; ++j)
        {
            for (int k = 0; k < dimension; ++k)
            {
                out.components[i][j] +=
                    d1_christoffel[k][k][i][j] -
                    d1_christoffel[j][k][i][k];
                for (int l = 0; l < dimension; ++l)
                {
                    out.components[i][j] +=
                        christoffel[k][i][j] * christoffel[l][k][l] -
                        christoffel[l][i][k] * christoffel[k][j][l];
                }
            }
        }
    }
    out.scalar = trace(out.components, gamma_inverse);
    return out;
}

RicciResult direct_grchombo_ricci(const ConformalJet &jet)
{
    DirectVars<double> vars;
    DirectVars<Tensor<1, double>> d1;
    DirectDiff2Vars<Tensor<2, double>> d2;
    vars.chi = jet.chi;
    for (int i = 0; i < dimension; ++i)
    {
        vars.Gamma[i] = 0.0;
        d1.chi[i] = jet.d1_chi[i];
        for (int j = 0; j < dimension; ++j)
        {
            vars.h[i][j] = jet.h[i][j];
            d2.chi[i][j] = jet.d2_chi[i][j];
            for (int derivative = 0; derivative < dimension; ++derivative)
            {
                d1.h[i][j][derivative] =
                    jet.d1_h[derivative][i][j];
                d1.Gamma[i][derivative] = 0.0;
                for (int second = 0; second < dimension; ++second)
                {
                    d2.h[i][j][derivative][second] =
                        jet.d2_h[derivative][second][i][j];
                }
            }
        }
    }

    const auto h_inverse = TensorAlgebra::compute_inverse_sym(vars.h);
    const auto chris = TensorAlgebra::compute_christoffel(d1.h, h_inverse);
    const auto d1_chris = CCZ4Geometry::compute_d1_chris_contracted(
        h_inverse, d1.h, d2.h);
    for (int i = 0; i < dimension; ++i)
    {
        vars.Gamma[i] = chris.contracted[i];
        for (int derivative = 0; derivative < dimension; ++derivative)
        {
            d1.Gamma[i][derivative] = d1_chris[i][derivative];
        }
    }
    const auto gr =
        CCZ4Geometry::compute_ricci(vars, d1, d2, h_inverse, chris);
    return {from_gr_tensor(gr.LL), gr.scalar};
}

Statistics run_ricci_comparison()
{
    Statistics stats;
    for (const auto &jet : make_ricci_cases())
    {
        const RicciResult independent = independent_physical_ricci(jet);
        const RicciResult direct = direct_grchombo_ricci(jet);
        compare_matrices(stats, direct.components, independent.components,
                         jet.name + ".R");
        stats.observe(direct.scalar, independent.scalar,
                      jet.name + ".scalar");
    }
    return stats;
}

bool ricci_sign_mutation_detected()
{
    const ConformalJet mixed = make_ricci_cases().back();
    const RicciResult independent = independent_physical_ricci(mixed);
    const RicciResult direct = direct_grchombo_ricci(mixed);
    Statistics mutation;
    mutation.observe(-direct.components[x_dir][z_dir],
                     independent.components[x_dir][z_dir],
                     "Ricci_xz_sign");
    return !mutation.passed && mutation.max_normalized > 1000.0;
}

enum class SlotOwnership
{
    directly_shared,
    custom_hidden_representative,
    stock_visible_y_component,
    gauge_variable,
    otherwise_non_overlapping
};

struct SlotExpectation
{
    std::string_view custom_name;
    int expected_custom_slot;
    std::string_view stock_component;
    int expected_stock_slot;
    SlotOwnership ownership;
    int frozen_state_slot;
};

constexpr int no_slot = -1;

constexpr std::array<int, 27> actual_custom_slots = {
    c_chi,    c_h11,    c_h12,    c_h13,    c_h22,    c_h23,    c_h33,
    c_hww,    c_K,      c_A11,    c_A12,    c_A13,    c_A22,    c_A23,
    c_A33,    c_Aww,    c_Theta,  c_Gamma1, c_Gamma2, c_Gamma3, c_lapse,
    c_shift1, c_shift2, c_shift3, c_B1,     c_B2,     c_B3};

constexpr std::array<SlotExpectation, 27> expected_custom_layout = {{
    {"chi", 0, "chi", StockGRChomboLayout::c_chi,
     SlotOwnership::directly_shared, 0},
    {"h11", 1, "h11", StockGRChomboLayout::c_h11,
     SlotOwnership::directly_shared, 1},
    {"h12", 2, "h12", StockGRChomboLayout::c_h12,
     SlotOwnership::stock_visible_y_component, no_slot},
    {"h13", 3, "h13", StockGRChomboLayout::c_h13,
     SlotOwnership::directly_shared, 2},
    {"h22", 4, "h22", StockGRChomboLayout::c_h22,
     SlotOwnership::stock_visible_y_component, no_slot},
    {"h23", 5, "h23", StockGRChomboLayout::c_h23,
     SlotOwnership::stock_visible_y_component, no_slot},
    {"h33", 6, "h33", StockGRChomboLayout::c_h33,
     SlotOwnership::directly_shared, 3},
    {"hww", 7, "none", no_slot,
     SlotOwnership::custom_hidden_representative, 4},
    {"K", 8, "K", StockGRChomboLayout::c_K,
     SlotOwnership::directly_shared, 5},
    {"A11", 9, "A11", StockGRChomboLayout::c_A11,
     SlotOwnership::directly_shared, 6},
    {"A12", 10, "A12", StockGRChomboLayout::c_A12,
     SlotOwnership::stock_visible_y_component, no_slot},
    {"A13", 11, "A13", StockGRChomboLayout::c_A13,
     SlotOwnership::directly_shared, 7},
    {"A22", 12, "A22", StockGRChomboLayout::c_A22,
     SlotOwnership::stock_visible_y_component, no_slot},
    {"A23", 13, "A23", StockGRChomboLayout::c_A23,
     SlotOwnership::stock_visible_y_component, no_slot},
    {"A33", 14, "A33", StockGRChomboLayout::c_A33,
     SlotOwnership::directly_shared, 8},
    {"Aww", 15, "none", no_slot,
     SlotOwnership::custom_hidden_representative, 9},
    {"Theta", 16, "Theta", StockGRChomboLayout::c_Theta,
     SlotOwnership::directly_shared, 10},
    {"Gamma1", 17, "Gamma1", StockGRChomboLayout::c_Gamma1,
     SlotOwnership::directly_shared, 11},
    {"Gamma2", 18, "Gamma2", StockGRChomboLayout::c_Gamma2,
     SlotOwnership::stock_visible_y_component, no_slot},
    {"Gamma3", 19, "Gamma3", StockGRChomboLayout::c_Gamma3,
     SlotOwnership::directly_shared, 12},
    {"lapse", 20, "lapse", StockGRChomboLayout::c_lapse,
     SlotOwnership::gauge_variable, no_slot},
    {"shift1", 21, "shift1", StockGRChomboLayout::c_shift1,
     SlotOwnership::gauge_variable, no_slot},
    {"shift2", 22, "shift2", StockGRChomboLayout::c_shift2,
     SlotOwnership::gauge_variable, no_slot},
    {"shift3", 23, "shift3", StockGRChomboLayout::c_shift3,
     SlotOwnership::gauge_variable, no_slot},
    {"B1", 24, "B1", StockGRChomboLayout::c_B1,
     SlotOwnership::gauge_variable, no_slot},
    {"B2", 25, "B2", StockGRChomboLayout::c_B2,
     SlotOwnership::gauge_variable, no_slot},
    {"B3", 26, "B3", StockGRChomboLayout::c_B3,
     SlotOwnership::gauge_variable, no_slot},
}};

constexpr bool custom_slots_form_complete_permutation()
{
    std::array<bool, 27> seen{};
    for (const int slot : actual_custom_slots)
    {
        if (slot < 0 || slot >= NUM_VARS ||
            seen[static_cast<std::size_t>(slot)])
        {
            return false;
        }
        seen[static_cast<std::size_t>(slot)] = true;
    }
    for (const bool present : seen)
    {
        if (!present)
        {
            return false;
        }
    }
    return true;
}

constexpr bool frozen_slots_exclude_visible_y_and_gauge()
{
    for (const auto &entry : expected_custom_layout)
    {
        const bool frozen = entry.frozen_state_slot != no_slot;
        if ((entry.ownership == SlotOwnership::stock_visible_y_component ||
             entry.ownership == SlotOwnership::gauge_variable) &&
            frozen)
        {
            return false;
        }
    }
    return true;
}

static_assert(NUM_VARS == 27, "custom layout must contain exactly 27 slots");
static_assert(StockGRChomboLayout::NUM_CCZ4_VARS == 25,
              "inspected stock GRChombo layout must contain 25 slots");
static_assert(c_chi == 0 && c_h11 == 1 && c_h12 == 2 && c_h13 == 3 &&
                  c_h22 == 4 && c_h23 == 5 && c_h33 == 6 && c_hww == 7 &&
                  c_K == 8,
              "custom metric/K slots changed");
static_assert(c_A11 == 9 && c_A12 == 10 && c_A13 == 11 && c_A22 == 12 &&
                  c_A23 == 13 && c_A33 == 14 && c_Aww == 15 &&
                  c_Theta == 16,
              "custom A/Theta slots changed");
static_assert(c_Gamma1 == 17 && c_Gamma2 == 18 && c_Gamma3 == 19 &&
                  c_lapse == 20 && c_shift1 == 21 && c_shift2 == 22 &&
                  c_shift3 == 23 && c_B1 == 24 && c_B2 == 25 && c_B3 == 26,
              "custom Gamma/gauge slots changed");
static_assert(c_hww != c_h12 && c_hww != c_h22 && c_hww != c_h23,
              "hidden hww must not alias a visible-y metric component");
static_assert(c_Aww != c_A12 && c_Aww != c_A22 && c_Aww != c_A23,
              "hidden Aww must not alias a visible-y A component");
static_assert(custom_slots_form_complete_permutation(),
              "custom slots must be unique and cover [0,27)");
static_assert(frozen_slots_exclude_visible_y_and_gauge(),
              "visible-y and gauge slots must not enter the frozen state map");

const char *ownership_name(const SlotOwnership ownership)
{
    switch (ownership)
    {
    case SlotOwnership::directly_shared:
        return "directly_shared";
    case SlotOwnership::custom_hidden_representative:
        return "custom_hidden_representative";
    case SlotOwnership::stock_visible_y_component:
        return "stock_visible_y_component";
    case SlotOwnership::gauge_variable:
        return "gauge_variable";
    case SlotOwnership::otherwise_non_overlapping:
        return "otherwise_non_overlapping";
    }
    return "unknown";
}

bool mapping_table_is_exact(
    const std::array<SlotExpectation, 27> &candidate)
{
    namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;
    using Variable = Operator::PerturbationVariable;
    constexpr std::array<Variable, 13> expected_frozen_order = {
        Variable::chi,         Variable::h_xx,       Variable::h_xz,
        Variable::h_zz,        Variable::h_ww,       Variable::K,
        Variable::A_xx,        Variable::A_xz,       Variable::A_zz,
        Variable::A_ww,        Variable::Theta,      Variable::hat_Gamma_x,
        Variable::hat_Gamma_z};

    if (Operator::frozen_gauge_state_vector != expected_frozen_order)
    {
        return false;
    }

    std::array<bool, 27> seen_custom_slots{};
    std::array<bool, 13> seen_frozen_slots{};
    std::array<int, 5> ownership_counts{};
    for (std::size_t index = 0; index < candidate.size(); ++index)
    {
        const auto &entry = candidate[index];
        const auto &expected = expected_custom_layout[index];
        if (entry.custom_name != expected.custom_name ||
            entry.expected_custom_slot != actual_custom_slots[index] ||
            entry.expected_custom_slot != expected.expected_custom_slot ||
            entry.stock_component != expected.stock_component ||
            entry.expected_stock_slot != expected.expected_stock_slot ||
            entry.ownership != expected.ownership ||
            entry.frozen_state_slot != expected.frozen_state_slot ||
            entry.expected_custom_slot < 0 ||
            entry.expected_custom_slot >= NUM_VARS ||
            UserVariables::variable_names[static_cast<std::size_t>(
                entry.expected_custom_slot)] != entry.custom_name)
        {
            return false;
        }

        const std::size_t custom_slot =
            static_cast<std::size_t>(entry.expected_custom_slot);
        if (seen_custom_slots[custom_slot])
        {
            return false;
        }
        seen_custom_slots[custom_slot] = true;
        ++ownership_counts[static_cast<std::size_t>(entry.ownership)];

        if (entry.frozen_state_slot != no_slot)
        {
            if (entry.frozen_state_slot < 0 ||
                entry.frozen_state_slot >=
                    static_cast<int>(seen_frozen_slots.size()) ||
                seen_frozen_slots[static_cast<std::size_t>(
                    entry.frozen_state_slot)])
            {
                return false;
            }
            seen_frozen_slots[static_cast<std::size_t>(
                entry.frozen_state_slot)] = true;
        }
    }

    return std::all_of(seen_custom_slots.begin(), seen_custom_slots.end(),
                       [](const bool present) { return present; }) &&
           std::all_of(seen_frozen_slots.begin(), seen_frozen_slots.end(),
                       [](const bool present) { return present; }) &&
           ownership_counts[static_cast<std::size_t>(
               SlotOwnership::directly_shared)] == 11 &&
           ownership_counts[static_cast<std::size_t>(
               SlotOwnership::custom_hidden_representative)] == 2 &&
           ownership_counts[static_cast<std::size_t>(
               SlotOwnership::stock_visible_y_component)] == 7 &&
           ownership_counts[static_cast<std::size_t>(
               SlotOwnership::gauge_variable)] == 7 &&
           ownership_counts[static_cast<std::size_t>(
               SlotOwnership::otherwise_non_overlapping)] == 0;
}

bool mapping_data_changed(
    const std::array<SlotExpectation, 27> &candidate)
{
    for (std::size_t index = 0; index < candidate.size(); ++index)
    {
        const auto &entry = candidate[index];
        const auto &expected = expected_custom_layout[index];
        if (entry.custom_name != expected.custom_name ||
            entry.expected_custom_slot != expected.expected_custom_slot ||
            entry.stock_component != expected.stock_component ||
            entry.expected_stock_slot != expected.expected_stock_slot ||
            entry.ownership != expected.ownership ||
            entry.frozen_state_slot != expected.frozen_state_slot)
        {
            return true;
        }
    }
    return false;
}

struct MappingMutationResults
{
    bool swap_h11_h13;
    bool hww_to_visible_y;
    bool Aww_to_visible_y;
    bool duplicate_shared_slot;
    bool omit_shared_slot;

    bool all_rejected() const
    {
        return swap_h11_h13 && hww_to_visible_y && Aww_to_visible_y &&
               duplicate_shared_slot && omit_shared_slot;
    }
};

MappingMutationResults run_mapping_mutations()
{
    auto rejected = [](const std::array<SlotExpectation, 27> &mutation)
    {
        return mapping_data_changed(mutation) &&
               !mapping_table_is_exact(mutation);
    };

    auto swap_h11_h13 = expected_custom_layout;
    std::swap(swap_h11_h13[1].expected_custom_slot,
              swap_h11_h13[3].expected_custom_slot);

    auto hww_to_visible_y = expected_custom_layout;
    hww_to_visible_y[7].expected_custom_slot = c_h12;
    hww_to_visible_y[7].stock_component = "h12";
    hww_to_visible_y[7].expected_stock_slot =
        StockGRChomboLayout::c_h12;
    hww_to_visible_y[7].ownership =
        SlotOwnership::stock_visible_y_component;
    hww_to_visible_y[7].frozen_state_slot = no_slot;

    auto Aww_to_visible_y = expected_custom_layout;
    Aww_to_visible_y[15].expected_custom_slot = c_A12;
    Aww_to_visible_y[15].stock_component = "A12";
    Aww_to_visible_y[15].expected_stock_slot =
        StockGRChomboLayout::c_A12;
    Aww_to_visible_y[15].ownership =
        SlotOwnership::stock_visible_y_component;
    Aww_to_visible_y[15].frozen_state_slot = no_slot;

    auto duplicate_shared_slot = expected_custom_layout;
    duplicate_shared_slot[11].expected_custom_slot = c_A11;

    auto omit_shared_slot = expected_custom_layout;
    omit_shared_slot[19].stock_component = "none";
    omit_shared_slot[19].expected_stock_slot = no_slot;
    omit_shared_slot[19].ownership =
        SlotOwnership::otherwise_non_overlapping;
    omit_shared_slot[19].frozen_state_slot = no_slot;

    return {rejected(swap_h11_h13), rejected(hww_to_visible_y),
            rejected(Aww_to_visible_y), rejected(duplicate_shared_slot),
            rejected(omit_shared_slot)};
}

void print_mapping_summary()
{
    for (const auto &entry : expected_custom_layout)
    {
        std::cout << "LAYOUT custom=" << entry.custom_name
                  << " custom_slot=" << entry.expected_custom_slot
                  << " stock=" << entry.stock_component
                  << " stock_slot=" << entry.expected_stock_slot
                  << " ownership=" << ownership_name(entry.ownership)
                  << " frozen_slot=" << entry.frozen_state_slot << '\n';
    }
}

void print_result(const std::string &name, const Statistics &stats)
{
    std::cout << "RESULT " << name << " max_abs=" << stats.max_absolute
              << " worst_abs=" << stats.worst_absolute
              << " max_normalized=" << stats.max_normalized
              << " worst_normalized=" << stats.worst_normalized
              << " status=" << (stats.passed ? "PASS" : "MISMATCH") << '\n';
}
} // namespace

int main()
{
    std::cout << std::scientific << std::setprecision(17);
    std::cout << "MANIFEST CH_SPACEDIM=" << CH_SPACEDIM
              << " GR_SPACEDIM=" << GR_SPACEDIM
              << " DEFAULT_TENSOR_DIM=" << DEFAULT_TENSOR_DIM
              << " custom_physical_dim="
              << BlackStringToy::ConformalCartoonAlgebra::physical_spatial_dim
              << " custom_gridded_dim="
              << BlackStringToy::ConformalCartoonAlgebra::gridded_spatial_dim
              << " custom_hidden_multiplicity="
              << BlackStringToy::ConformalCartoonAlgebra::hidden_multiplicity
              << " atol=" << absolute_tolerance
              << " rtol=" << relative_tolerance << '\n';

    const Statistics conformal = run_conformal_algebra_comparison();
    const Statistics connection = run_connection_comparison();
    const Statistics ricci = run_ricci_comparison();
    print_result("conformal_algebra", conformal);
    print_result("contracted_connection_and_Z", connection);
    print_result("visible_physical_ricci", ricci);

    print_mapping_summary();
    const bool state_map = mapping_table_is_exact(expected_custom_layout);
    const MappingMutationResults mapping_mutations = run_mapping_mutations();
    const bool conformal_mutation = conformal_dimension_mutation_detected();
    const bool connection_mutation = connection_sign_mutation_detected();
    const bool ricci_mutation = ricci_sign_mutation_detected();
    std::cout << "EXACT custom_27_slot_layout="
              << (state_map ? "PASS" : "FAIL")
              << " shared_11_mapping=" << (state_map ? "PASS" : "FAIL")
              << " ownership_separation=" << (state_map ? "PASS" : "FAIL")
              << '\n';
    std::cout << "MAPPING_MUTATION swap_h11_h13="
              << (mapping_mutations.swap_h11_h13 ? "REJECTED" : "MISSED")
              << " hww_to_visible_y="
              << (mapping_mutations.hww_to_visible_y ? "REJECTED" : "MISSED")
              << " Aww_to_visible_y="
              << (mapping_mutations.Aww_to_visible_y ? "REJECTED" : "MISSED")
              << " duplicate_shared_slot="
              << (mapping_mutations.duplicate_shared_slot ? "REJECTED"
                                                         : "MISSED")
              << " omit_shared_slot="
              << (mapping_mutations.omit_shared_slot ? "REJECTED" : "MISSED")
              << '\n';
    std::cout << "MUTATION wrong_trace_dimension="
              << (conformal_mutation ? "DETECTED" : "MISSED")
              << " connection_sign="
              << (connection_mutation ? "DETECTED" : "MISSED")
              << " ricci_xz_sign="
              << (ricci_mutation ? "DETECTED" : "MISSED") << '\n';
    std::cout << "CLASSIFICATION gamma_equals_h_over_chi=SOURCE_ONLY"
              << " Kij_reconstruction=SOURCE_ONLY"
              << " hidden_ww_and_multiplicity=CUSTOM_ONLY"
              << " tensor_algebra=DIRECT_COMPILED_GRCHOMBO"
              << " contracted_connection=DIRECT_COMPILED_GRCHOMBO"
              << " physical_ricci=DIRECT_COMPILED_GRCHOMBO\n";

    const bool passed =
        conformal.passed && connection.passed && ricci.passed && state_map &&
        mapping_mutations.all_rejected() &&
        conformal_mutation && connection_mutation && ricci_mutation;
    std::cout << "BATCH1 " << (passed ? "PASS" : "FAIL") << '\n';
    return passed ? 0 : 1;
}
