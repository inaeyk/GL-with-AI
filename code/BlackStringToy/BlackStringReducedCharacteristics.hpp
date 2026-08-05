#ifndef BLACKSTRINGREDUCEDCHARACTERISTICS_HPP
#define BLACKSTRINGREDUCEDCHARACTERISTICS_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>

extern "C"
{
void dgesvd_(char *, char *, int *, int *, double *, int *, double *, double *,
             int *, double *, int *, double *, int *, int *);
}

namespace BlackStringReducedCharacteristics
{
inline constexpr int state_size = 23;
inline constexpr int scalar_size = 11;
inline constexpr int transverse_size = 5;
using State = std::array<double, state_size>;
using Matrix = std::array<std::array<double, state_size>, state_size>;

enum StateIndex : int
{
    qchi = 0,
    dxx,
    dxz,
    dzz,
    K,
    Axx,
    Axz,
    Azz,
    Theta,
    GammaX,
    GammaZ,
    qalpha,
    rx,
    rz,
    Bx,
    Bz,
    chi,
    hxx,
    hxz,
    hzz,
    lapse,
    shiftX,
    shiftZ
};

inline constexpr std::array<int, scalar_size> scalar_map = {
    qchi, dxx, dzz, K, Axx, Azz, Theta, GammaX, qalpha, rx, Bx};
inline constexpr std::array<int, transverse_size> transverse_map = {
    dxz, Axz, GammaZ, rz, Bz};
inline constexpr std::array<int, 7> primitive_map = {
    chi, hxx, hxz, hzz, lapse, shiftX, shiftZ};

using Scalar = std::array<double, scalar_size>;
using ScalarMatrix =
    std::array<std::array<double, scalar_size>, scalar_size>;
using Transverse = std::array<double, transverse_size>;
using TransverseMatrix =
    std::array<std::array<double, transverse_size>, transverse_size>;

inline double lapse_mu(const double b, const int sign)
{
    return 0.5 * (b + static_cast<double>(sign) * std::sqrt(b * b + 8.0));
}

inline double transverse_shift_mu(const double b, const int sign)
{
    return 0.5 * (b + static_cast<double>(sign) * std::sqrt(b * b + 3.0));
}

inline double longitudinal_shift_mu(const double b, const int sign)
{
    return 0.5 *
           (b + static_cast<double>(sign) * std::sqrt(b * b + 4.5));
}

inline void require_b(const double b)
{
    if (!std::isfinite(b) || !(b > 0.0) ||
        std::abs(8.0 * b - 1.0) <=
            64.0 * std::numeric_limits<double>::epsilon())
    {
        throw std::domain_error("M2-B CHARACTERISTIC_TRANSFORM_FAILURE");
    }
}

inline ScalarMatrix scalar_right(const double b)
{
    require_b(b);
    ScalarMatrix result{};
    const auto set_column = [&](const int column, const Scalar &values) {
        for (int row = 0; row < scalar_size; ++row)
        {
            result[static_cast<std::size_t>(row)]
                  [static_cast<std::size_t>(column)] =
                values[static_cast<std::size_t>(row)];
        }
    };
    int column = 0;
    for (const int sigma : {1, -1})
    {
        const double s = static_cast<double>(sigma);
        const double mu = b + s;
        set_column(column++, {0.0, -2.0 * s, 0.0, 0.0, 1.0, 0.0,
                              0.0, 0.0, 0.0, 0.0, 0.0});
        set_column(column++, {0.0, 0.0, -2.0 * s, 0.0, 0.0, 1.0,
                              0.0, 0.0, 0.0, 0.0, 0.0});
        set_column(column++, {-(4.0 * b + s) / 4.0,
                              9.0 * s / 8.0,
                              -3.0 * s / 8.0,
                              -s * (8.0 * b - s) / 4.0,
                              0.0,
                              0.0,
                              -s * (8.0 * b - s) / 8.0,
                              mu,
                              0.0,
                              0.75,
                              mu});
    }
    for (const int sign : {1, -1})
    {
        const double mu = lapse_mu(b, sign);
        set_column(column++, {-1.0 / 3.0, 1.0, -1.0 / 3.0,
                              -7.0 / (12.0 * mu), -7.0 / (16.0 * mu),
                              7.0 / (48.0 * mu), 0.0, 1.0,
                              7.0 / (6.0 * mu * mu), 3.0 / (4.0 * mu),
                              1.0});
    }
    for (const int sign : {1, -1})
    {
        const double mu = longitudinal_shift_mu(b, sign);
        set_column(column++, {-1.0 / 3.0, 1.0, -1.0 / 3.0, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0, 3.0 / (4.0 * mu), 1.0});
    }
    set_column(column, {0.5, -1.5, 0.5, 0.0, 0.0, 0.0, 0.0, -1.5,
                        0.0, b, 0.0});
    return result;
}

inline ScalarMatrix scalar_left(const double b)
{
    require_b(b);
    ScalarMatrix result{};
    const auto ell_x = [](const double s) {
        return Scalar{-1.5 * s, -0.5 * s, 0.0, -0.75, 1.0, 0.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
    };
    const auto ell_z = [](const double s) {
        return Scalar{0.5 * s, 0.0, -0.5 * s, 0.25, 0.0, 1.0,
                      0.0, 0.0, 0.0, 0.0, 0.0};
    };
    const auto ell_c = [](const double s) {
        return Scalar{3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2.0 * s, 1.0,
                      0.0, 0.0, 0.0};
    };
    const auto combine = [](const Scalar &first, const double a,
                            const Scalar &second, const double c) {
        Scalar row{};
        for (int index = 0; index < scalar_size; ++index)
        {
            row[static_cast<std::size_t>(index)] =
                a * first[static_cast<std::size_t>(index)] +
                c * second[static_cast<std::size_t>(index)];
        }
        return row;
    };
    int row = 0;
    for (const int sigma : {1, -1})
    {
        const double s = static_cast<double>(sigma);
        const Scalar x = ell_x(s);
        const Scalar z = ell_z(s);
        const Scalar c = ell_c(s);
        result[static_cast<std::size_t>(row++)] =
            combine(x, 0.5, c, 3.0 * s / 8.0);
        result[static_cast<std::size_t>(row++)] =
            combine(z, 0.5, c, -s / 8.0);
        Scalar normalized_c{};
        for (int index = 0; index < scalar_size; ++index)
        {
            normalized_c[static_cast<std::size_t>(index)] =
                -2.0 * c[static_cast<std::size_t>(index)] / (8.0 * b - s);
        }
        result[static_cast<std::size_t>(row++)] = normalized_c;
    }
    for (const int sign : {1, -1})
    {
        const double mu = lapse_mu(b, sign);
        const double normalization =
            7.0 * (mu * mu + 2.0) / (12.0 * mu * mu);
        result[static_cast<std::size_t>(row++)] =
            {0.0, 0.0, 0.0, -mu / normalization, 0.0, 0.0,
             2.0 * mu / normalization, 0.0, 1.0 / normalization,
             0.0, 0.0};
    }
    for (const int sign : {1, -1})
    {
        const double mu = longitudinal_shift_mu(b, sign);
        const double mu2 = mu * mu;
        const double mu3 = mu2 * mu;
        const double mu4 = mu2 * mu2;
        const double d = 64.0 * mu2 - 81.0;
        const double normalization =
            7.0 * d * (8.0 * mu2 + 9.0) / 3.0;
        result[static_cast<std::size_t>(row++)] =
            {1792.0 * mu4 / normalization,
             0.0,
             0.0,
             32.0 * mu3 * d / normalization,
             0.0,
             0.0,
             -128.0 * mu3 * (32.0 * mu2 - 51.0) / normalization,
             7.0 * (256.0 * mu4 - 408.0 * mu2 + 243.0) /
                 normalization,
             -32.0 * mu2 * d / normalization,
             28.0 * mu * d / normalization,
             21.0 * d / normalization};
    }
    result[static_cast<std::size_t>(row)] =
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -2.0 / 3.0,
         0.0, 0.0, 2.0 / 3.0};
    return result;
}

inline TransverseMatrix transverse_right(const double b)
{
    require_b(b);
    TransverseMatrix result{};
    const auto set_column = [&](const int column, const Transverse &values) {
        for (int row = 0; row < transverse_size; ++row)
        {
            result[static_cast<std::size_t>(row)]
                  [static_cast<std::size_t>(column)] =
                values[static_cast<std::size_t>(row)];
        }
    };
    set_column(0, {-2.0, 1.0, 0.0, 0.0, 0.0});
    set_column(1, {2.0, 1.0, 0.0, 0.0, 0.0});
    for (int column = 2; column <= 3; ++column)
    {
        const int sign = column == 2 ? 1 : -1;
        const double mu = transverse_shift_mu(b, sign);
        set_column(column, {1.0, 0.0, 1.0, 3.0 / (4.0 * mu), 1.0});
    }
    set_column(4, {-1.0, 0.0, -1.0, b, 0.0});
    return result;
}

inline TransverseMatrix transverse_left(const double b)
{
    require_b(b);
    TransverseMatrix result{};
    result[0] = {-0.25, 0.5, 0.25, 0.0, 0.0};
    result[1] = {0.25, 0.5, -0.25, 0.0, 0.0};
    for (int row = 2; row <= 3; ++row)
    {
        const int sign = row == 2 ? 1 : -1;
        const double mu = transverse_shift_mu(b, sign);
        result[static_cast<std::size_t>(row)] =
            {0.0, 0.0, b / (2.0 * mu - b),
             1.0 / (2.0 * mu - b),
             (mu - b) / (2.0 * mu - b)};
    }
    result[4] = {0.0, 0.0, -1.0, 0.0, 1.0};
    return result;
}

inline Matrix right_matrix(const double b)
{
    Matrix result{};
    const ScalarMatrix scalar = scalar_right(b);
    const TransverseMatrix transverse = transverse_right(b);
    for (int row = 0; row < scalar_size; ++row)
    {
        for (int column = 0; column < scalar_size; ++column)
        {
            result[static_cast<std::size_t>(scalar_map[row])]
                  [static_cast<std::size_t>(column)] =
                scalar[static_cast<std::size_t>(row)]
                      [static_cast<std::size_t>(column)];
        }
    }
    for (int row = 0; row < transverse_size; ++row)
    {
        for (int column = 0; column < transverse_size; ++column)
        {
            result[static_cast<std::size_t>(transverse_map[row])]
                  [static_cast<std::size_t>(scalar_size + column)] =
                transverse[static_cast<std::size_t>(row)]
                          [static_cast<std::size_t>(column)];
        }
    }
    for (int index = 0; index < 7; ++index)
    {
        result[static_cast<std::size_t>(primitive_map[index])]
              [static_cast<std::size_t>(scalar_size + transverse_size +
                                        index)] = 1.0;
    }
    return result;
}

inline Matrix left_matrix(const double b)
{
    Matrix result{};
    const ScalarMatrix scalar = scalar_left(b);
    const TransverseMatrix transverse = transverse_left(b);
    for (int row = 0; row < scalar_size; ++row)
    {
        for (int column = 0; column < scalar_size; ++column)
        {
            result[static_cast<std::size_t>(row)]
                  [static_cast<std::size_t>(scalar_map[column])] =
                scalar[static_cast<std::size_t>(row)]
                      [static_cast<std::size_t>(column)];
        }
    }
    for (int row = 0; row < transverse_size; ++row)
    {
        for (int column = 0; column < transverse_size; ++column)
        {
            result[static_cast<std::size_t>(scalar_size + row)]
                  [static_cast<std::size_t>(transverse_map[column])] =
                transverse[static_cast<std::size_t>(row)]
                          [static_cast<std::size_t>(column)];
        }
    }
    for (int index = 0; index < 7; ++index)
    {
        result[static_cast<std::size_t>(scalar_size + transverse_size +
                                       index)]
              [static_cast<std::size_t>(primitive_map[index])] = 1.0;
    }
    return result;
}

inline State multiply(const Matrix &matrix, const State &input)
{
    State output{};
    for (int row = 0; row < state_size; ++row)
    {
        for (int column = 0; column < state_size; ++column)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row)]
                      [static_cast<std::size_t>(column)] *
                input[static_cast<std::size_t>(column)];
        }
    }
    return output;
}

inline State to_characteristics(const double b, const State &normal_state)
{
    return multiply(left_matrix(b), normal_state);
}

inline State from_characteristics(const double b,
                                  const State &characteristics)
{
    return multiply(right_matrix(b), characteristics);
}

inline State normal_speeds(const double b, const int normal)
{
    require_b(b);
    if (normal != -1 && normal != 1)
    {
        throw std::domain_error("M2-B CHARACTERISTIC_NORMAL_FAILURE");
    }
    State result{};
    int index = 0;
    for (const int sigma : {1, -1})
    {
        for (int copy = 0; copy < 3; ++copy)
        {
            result[static_cast<std::size_t>(index++)] =
                -static_cast<double>(normal) *
                (b + static_cast<double>(sigma));
        }
    }
    for (const int sign : {1, -1})
    {
        result[static_cast<std::size_t>(index++)] =
            -static_cast<double>(normal) * lapse_mu(b, sign);
    }
    for (const int sign : {1, -1})
    {
        result[static_cast<std::size_t>(index++)] =
            -static_cast<double>(normal) *
            longitudinal_shift_mu(b, sign);
    }
    result[static_cast<std::size_t>(index++)] = 0.0;
    result[static_cast<std::size_t>(index++)] =
        -static_cast<double>(normal) * (b + 1.0);
    result[static_cast<std::size_t>(index++)] =
        -static_cast<double>(normal) * (b - 1.0);
    for (const int sign : {1, -1})
    {
        result[static_cast<std::size_t>(index++)] =
            -static_cast<double>(normal) *
            transverse_shift_mu(b, sign);
    }
    result[static_cast<std::size_t>(index++)] = 0.0;
    for (int copy = 0; copy < 4; ++copy)
    {
        result[static_cast<std::size_t>(index++)] =
            -static_cast<double>(normal) * b;
    }
    while (index < state_size)
    {
        result[static_cast<std::size_t>(index++)] = 0.0;
    }
    return result;
}

inline double roundtrip_residual(const double b)
{
    const Matrix left = left_matrix(b);
    const Matrix right = right_matrix(b);
    double residual = 0.0;
    for (int row = 0; row < state_size; ++row)
    {
        for (int column = 0; column < state_size; ++column)
        {
            double value = 0.0;
            for (int inner = 0; inner < state_size; ++inner)
            {
                value += left[static_cast<std::size_t>(row)]
                             [static_cast<std::size_t>(inner)] *
                         right[static_cast<std::size_t>(inner)]
                              [static_cast<std::size_t>(column)];
            }
            residual = std::max(
                residual,
                std::abs(value - (row == column ? 1.0 : 0.0)));
        }
    }
    return residual;
}

inline double normalized_frobenius_condition_bound(const double b,
                                                   const double r0)
{
    if (!std::isfinite(r0) || !(r0 > 0.0))
    {
        throw std::domain_error("M2-B CHARACTERISTIC_TRANSFORM_FAILURE");
    }
    Matrix right = right_matrix(b);
    const Matrix left = left_matrix(b);
    std::array<double, state_size> column_norm{};
    for (int column = 0; column < state_size; ++column)
    {
        double norm2 = 0.0;
        for (int row = 0; row < state_size; ++row)
        {
            const double scale = row < 16 ? r0 : 1.0;
            const double value = scale * right[static_cast<std::size_t>(row)]
                                              [static_cast<std::size_t>(column)];
            norm2 += value * value;
        }
        column_norm[static_cast<std::size_t>(column)] = std::sqrt(norm2);
    }
    double right_norm2 = 0.0;
    double inverse_norm2 = 0.0;
    for (int row = 0; row < state_size; ++row)
    {
        for (int column = 0; column < state_size; ++column)
        {
            const double state_scale = row < 16 ? r0 : 1.0;
            const double normalized_right =
                state_scale * right[static_cast<std::size_t>(row)]
                                   [static_cast<std::size_t>(column)] /
                column_norm[static_cast<std::size_t>(column)];
            right_norm2 += normalized_right * normalized_right;

            const double inverse =
                column_norm[static_cast<std::size_t>(row)] *
                left[static_cast<std::size_t>(row)]
                    [static_cast<std::size_t>(column)] /
                (column < 16 ? r0 : 1.0);
            inverse_norm2 += inverse * inverse;
        }
    }
    return std::sqrt(right_norm2 * inverse_norm2);
}

struct RankCondition
{
    int rank = 0;
    double condition = std::numeric_limits<double>::infinity();
};

inline RankCondition normalized_rank_condition(const double b,
                                               const double r0)
{
    if (!std::isfinite(r0) || !(r0 > 0.0))
    {
        throw std::domain_error("M2-B CHARACTERISTIC_TRANSFORM_FAILURE");
    }
    const Matrix right = right_matrix(b);
    std::array<double, state_size * state_size> matrix{};
    for (int column = 0; column < state_size; ++column)
    {
        double norm2 = 0.0;
        for (int row = 0; row < state_size; ++row)
        {
            const double value = (row < 16 ? r0 : 1.0) *
                                 right[static_cast<std::size_t>(row)]
                                      [static_cast<std::size_t>(column)];
            matrix[static_cast<std::size_t>(row + state_size * column)] =
                value;
            norm2 += value * value;
        }
        const double norm = std::sqrt(norm2);
        if (!std::isfinite(norm) || !(norm > 0.0))
        {
            throw std::domain_error("M2-B CHARACTERISTIC_TRANSFORM_FAILURE");
        }
        for (int row = 0; row < state_size; ++row)
        {
            matrix[static_cast<std::size_t>(row + state_size * column)] /=
                norm;
        }
    }

    int rows = state_size;
    int columns = state_size;
    int leading = state_size;
    std::array<double, state_size> singular{};
    double unused_u = 0.0;
    double unused_vt = 0.0;
    int unused_leading = 1;
    std::array<double, 8 * state_size> work{};
    int work_size = static_cast<int>(work.size());
    int info = 0;
    char no_vectors = 'N';
    dgesvd_(&no_vectors, &no_vectors, &rows, &columns, matrix.data(),
            &leading, singular.data(), &unused_u, &unused_leading,
            &unused_vt, &unused_leading, work.data(), &work_size, &info);
    if (info != 0 || !std::isfinite(singular[0]) ||
        !(singular[0] > 0.0))
    {
        throw std::domain_error("M2-B CHARACTERISTIC_TRANSFORM_FAILURE");
    }
    RankCondition result;
    for (const double value : singular)
    {
        if (std::isfinite(value) && value >= 1.0e-12 * singular[0])
        {
            ++result.rank;
        }
    }
    if (result.rank == state_size && singular.back() > 0.0)
    {
        result.condition = singular[0] / singular.back();
    }
    return result;
}

inline void validate_locked_transform_gate(const double r0)
{
    constexpr std::array<double, 7> x_over_r0 = {
        0.5, 0.5625, 1.0, 2.0, 4.4375, 4.5, 0.75};
    for (const double ratio : x_over_r0)
    {
        const double b = std::sqrt(1.0 / ratio);
        const RankCondition rank_condition =
            normalized_rank_condition(b, r0);
        if (roundtrip_residual(b) > 5.0e-12 ||
            rank_condition.rank != state_size ||
            rank_condition.condition > 1.0e3)
        {
            throw std::domain_error("M2-B CHARACTERISTIC_TRANSFORM_FAILURE");
        }
        for (const int normal : {-1, 1})
        {
            const State speeds = normal_speeds(b, normal);
            for (const double speed : speeds)
            {
                if (!std::isfinite(speed))
                {
                    throw std::domain_error(
                        "M2-B CHARACTERISTIC_CLASSIFICATION_FAILURE");
                }
            }
        }
    }
    const State horizon = normal_speeds(1.0, 1);
    const int zeros = static_cast<int>(std::count_if(
        horizon.begin(), horizon.end(),
        [](const double value) { return std::abs(value) <= 5.0e-13; }));
    if (zeros != 9 || roundtrip_residual(1.0) > 5.0e-12)
    {
        throw std::domain_error(
            "M2-B CHARACTERISTIC_HORIZON_DEGENERACY_FAILURE");
    }
}

} // namespace BlackStringReducedCharacteristics

#endif /* BLACKSTRINGREDUCEDCHARACTERISTICS_HPP */
