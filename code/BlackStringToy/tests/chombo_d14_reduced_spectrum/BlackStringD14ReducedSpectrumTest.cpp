#define main blackstring_d13_fixture_entry_not_run
#include "../chombo_d13_tangent_mode/BlackStringD13TangentModeTest.cpp"
#undef main

#include "FortranNameMacro.H"

#include <complex>
#include <cstdint>
#include <cstring>
#include <set>
#include <sstream>

extern "C"
{
void LAPACK(GEEV, geev)(char *jobvl, char *jobvr, int *n, double *a,
                        int *lda, double *wr, double *wi, double *vl,
                        int *ldvl, double *vr, int *ldvr, double *work,
                        int *lwork, int *info);
}

namespace
{
constexpr int reduced_size = radial_cells * component_count;
constexpr int reduced_constraint_size = radial_cells * constraint_count;
constexpr int representative_columns_per_mode = 6;
constexpr double d14_epsilon_primary = 1.0e-8;
constexpr double d14_epsilon_secondary = 5.0e-9;
constexpr double column_agreement_tolerance = 5.0e-3;
constexpr double direct_agreement_tolerance = 2.0e-4;
constexpr double minimum_resolved_rate = 2.0e-4;

using Complex = std::complex<double>;
using ComplexVector = std::vector<Complex>;

[[noreturn]] void d14_fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_D14_FAIL " << message << '\n';
    std::exit(1);
}

void d14_require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        d14_fail(message);
    }
}

std::size_t reduced_index(const int radial, const int component)
{
    return static_cast<std::size_t>(radial) * component_count + component;
}

std::size_t constraint_reduced_index(const int radial, const int component)
{
    return static_cast<std::size_t>(radial) * constraint_count + component;
}

double vector_norm(const std::vector<double> &values)
{
    double sum = 0.0;
    for (const double value : values)
    {
        sum += value * value;
    }
    return std::sqrt(sum);
}

double complex_norm(const ComplexVector &values)
{
    double sum = 0.0;
    for (const Complex value : values)
    {
        sum += std::norm(value);
    }
    return std::sqrt(sum);
}

std::uint64_t hash_bytes(const void *data, const std::size_t size,
                         std::uint64_t hash = 1469598103934665603ULL)
{
    const auto *bytes = static_cast<const unsigned char *>(data);
    for (std::size_t i = 0; i < size; ++i)
    {
        hash ^= bytes[i];
        hash *= 1099511628211ULL;
    }
    return hash;
}

std::uint64_t hash_values(const std::vector<double> &values)
{
    std::uint64_t hash = 1469598103934665603ULL;
    for (const double value : values)
    {
        std::uint64_t bits = 0;
        static_assert(sizeof(bits) == sizeof(value));
        std::memcpy(&bits, &value, sizeof(bits));
        hash = hash_bytes(&bits, sizeof(bits), hash);
    }
    return hash;
}

std::uint64_t hash_values(const ComplexVector &values)
{
    std::uint64_t hash = 1469598103934665603ULL;
    for (const Complex value : values)
    {
        const std::array<double, 2> parts = {value.real(), value.imag()};
        for (const double part : parts)
        {
            std::uint64_t bits = 0;
            std::memcpy(&bits, &part, sizeof(bits));
            hash = hash_bytes(&bits, sizeof(bits), hash);
        }
    }
    return hash;
}

std::string hash_string(const std::uint64_t hash)
{
    std::ostringstream output;
    output << "0x" << std::hex << std::setfill('0') << std::setw(16)
           << hash;
    return output.str();
}

ComplexVector matrix_vector(const std::vector<double> &matrix,
                            const ComplexVector &input)
{
    d14_require(matrix.size() ==
                    static_cast<std::size_t>(reduced_size) * reduced_size,
                "dense matrix has wrong size");
    d14_require(input.size() == static_cast<std::size_t>(reduced_size),
                "dense input has wrong size");
    ComplexVector output(static_cast<std::size_t>(reduced_size));
    for (int column = 0; column < reduced_size; ++column)
    {
        const Complex coefficient = input[static_cast<std::size_t>(column)];
        for (int row = 0; row < reduced_size; ++row)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(reduced_size) * column] *
                coefficient;
        }
    }
    return output;
}

ComplexVector transpose_matrix_vector(const std::vector<double> &matrix,
                                      const ComplexVector &input)
{
    d14_require(matrix.size() ==
                    static_cast<std::size_t>(reduced_size) * reduced_size,
                "transpose matrix has wrong size");
    d14_require(input.size() == static_cast<std::size_t>(reduced_size),
                "transpose input has wrong size");
    ComplexVector output(static_cast<std::size_t>(reduced_size));
    for (int column = 0; column < reduced_size; ++column)
    {
        for (int row = 0; row < reduced_size; ++row)
        {
            output[static_cast<std::size_t>(column)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(reduced_size) * column] *
                input[static_cast<std::size_t>(row)];
        }
    }
    return output;
}

double right_eigen_residual(const std::vector<double> &matrix,
                            const ComplexVector &right,
                            const Complex lambda)
{
    ComplexVector residual = matrix_vector(matrix, right);
    for (int row = 0; row < reduced_size; ++row)
    {
        residual[static_cast<std::size_t>(row)] -=
            lambda * right[static_cast<std::size_t>(row)];
    }
    return complex_norm(residual) / complex_norm(right);
}

double left_eigen_residual(const std::vector<double> &matrix,
                           const ComplexVector &left,
                           const Complex lambda)
{
    ComplexVector residual = transpose_matrix_vector(matrix, left);
    for (int row = 0; row < reduced_size; ++row)
    {
        residual[static_cast<std::size_t>(row)] -=
            std::conj(lambda) * left[static_cast<std::size_t>(row)];
    }
    return complex_norm(residual) / complex_norm(left);
}

std::vector<double> matrix_vector(const std::vector<double> &matrix,
                                  const std::vector<double> &input)
{
    std::vector<double> output(static_cast<std::size_t>(reduced_size));
    for (int column = 0; column < reduced_size; ++column)
    {
        for (int row = 0; row < reduced_size; ++row)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(reduced_size) * column] *
                input[static_cast<std::size_t>(column)];
        }
    }
    return output;
}

RadialVector physical_input(const std::vector<double> &scaled,
                            const StateRow &field_scales)
{
    d14_require(scaled.size() == static_cast<std::size_t>(reduced_size),
                "scaled vector has wrong size");
    RadialVector result{};
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            result[static_cast<std::size_t>(radial)]
                  [static_cast<std::size_t>(component)] =
                scaled[reduced_index(radial, component)] *
                field_scales[static_cast<std::size_t>(component)];
        }
    }
    return result;
}

std::vector<double> scaled_output(const RadialVector &physical,
                                  const StateRow &field_scales)
{
    std::vector<double> result(static_cast<std::size_t>(reduced_size));
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            result[reduced_index(radial, component)] =
                physical[static_cast<std::size_t>(radial)]
                        [static_cast<std::size_t>(component)] /
                field_scales[static_cast<std::size_t>(component)];
        }
    }
    return result;
}

ComplexVector physical_vector(const ComplexVector &scaled,
                              const StateRow &field_scales)
{
    ComplexVector result(scaled.size());
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            const std::size_t index = reduced_index(radial, component);
            result[index] =
                scaled[index] *
                field_scales[static_cast<std::size_t>(component)];
        }
    }
    return result;
}

ComplexVector physical_left_vector(const ComplexVector &scaled,
                                   const StateRow &field_scales)
{
    ComplexVector result(scaled.size());
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            const std::size_t index = reduced_index(radial, component);
            result[index] =
                scaled[index] /
                field_scales[static_cast<std::size_t>(component)];
        }
    }
    return result;
}

struct ReducedMatrix
{
    std::vector<double> map;
    std::vector<double> constraints;
    double maximum_parity_leakage = 0.0;
    double maximum_harmonic_leakage = 0.0;
    double epsilon_column_difference = 0.0;
    double direct_vector_difference = 0.0;
    double construction_seconds = 0.0;
};

std::array<int, representative_columns_per_mode> representative_columns()
{
    return {0,
            c_GammaZ,
            reduced_size / 4 + c_hxz,
            reduced_size / 2 + c_K,
            3 * reduced_size / 4 + c_Aww,
            reduced_size - 1};
}

void store_column(ReducedMatrix &matrix, const int column,
                  const ActionResult &action, const StateRow &field_scales)
{
    const std::vector<double> output =
        scaled_output(action.response, field_scales);
    for (int row = 0; row < reduced_size; ++row)
    {
        matrix.map[static_cast<std::size_t>(row) +
                   static_cast<std::size_t>(reduced_size) * column] =
            output[static_cast<std::size_t>(row)];
    }
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int constraint = 0; constraint < constraint_count; ++constraint)
        {
            const std::size_t row =
                constraint_reduced_index(radial, constraint);
            matrix.constraints[row +
                               static_cast<std::size_t>(
                                   reduced_constraint_size) *
                                   column] =
                action.constraints[static_cast<std::size_t>(radial)]
                                  [static_cast<std::size_t>(constraint)];
        }
    }
    matrix.maximum_parity_leakage =
        std::max(matrix.maximum_parity_leakage, action.parity_leakage);
    matrix.maximum_harmonic_leakage =
        std::max(matrix.maximum_harmonic_leakage, action.harmonic_leakage);
}

double relative_difference(const std::vector<double> &left,
                           const std::vector<double> &right)
{
    d14_require(left.size() == right.size(),
                "relative-difference vectors differ in size");
    double difference_sum = 0.0;
    double reference_sum = 0.0;
    for (std::size_t i = 0; i < left.size(); ++i)
    {
        const double difference = left[i] - right[i];
        difference_sum += difference * difference;
        reference_sum += left[i] * left[i];
    }
    return std::sqrt(difference_sum) /
           std::max(std::sqrt(reference_sum),
                    std::numeric_limits<double>::min());
}

void validate_fourier_roundtrip(const D13TangentLevel &level,
                                const int mode_number)
{
    std::vector<double> coefficients(static_cast<std::size_t>(reduced_size));
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            coefficients[reduced_index(radial, component)] =
                std::sin(0.17 * (1 + radial) * (1 + component));
        }
    }
    std::vector<double> projected(static_cast<std::size_t>(reduced_size));
    std::vector<double> forbidden(static_cast<std::size_t>(reduced_size));
    std::vector<double> harmonic(static_cast<std::size_t>(reduced_size));
    const double k = 2.0 * pi * mode_number / 8.0;
    const double other_k =
        2.0 * pi * (mode_number == 1 ? 2 : 1) / 8.0;
    const double dz = level.spacing();
    for (int compact = 0; compact < compact_cells; ++compact)
    {
        const double z = (compact + 0.5) * dz;
        const double cosine = std::cos(k * z);
        const double sine = std::sin(k * z);
        const double other_cosine = std::cos(other_k * z);
        const double other_sine = std::sin(other_k * z);
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            for (int component = 0; component < component_count; ++component)
            {
                const std::size_t index = reduced_index(radial, component);
                const bool odd = one_z_component(component);
                const double value =
                    coefficients[index] * (odd ? sine : cosine);
                projected[index] +=
                    2.0 * value * (odd ? sine : cosine) / compact_cells;
                forbidden[index] +=
                    2.0 * value * (odd ? cosine : sine) / compact_cells;
                harmonic[index] +=
                    2.0 * value *
                    (odd ? other_sine : other_cosine) / compact_cells;
            }
        }
    }
    const double identity_error =
        relative_difference(coefficients, projected);
    const double forbidden_fraction =
        vector_norm(forbidden) / vector_norm(coefficients);
    const double harmonic_fraction =
        vector_norm(harmonic) / vector_norm(coefficients);
    d14_require(identity_error < 2.0e-14,
                "Fourier reconstruction/projection is not an identity");
    d14_require(forbidden_fraction < 2.0e-14,
                "Fourier roundtrip retained forbidden parity");
    d14_require(harmonic_fraction < 2.0e-14,
                "Fourier roundtrip retained another harmonic");
    std::cout << std::scientific << std::setprecision(12)
              << "D14_FOURIER_ROUNDTRIP mode_number=" << mode_number
              << " identity_relative_error=" << identity_error
              << " parity_leakage=" << forbidden_fraction
              << " harmonic_leakage=" << harmonic_fraction << '\n';
}

ReducedMatrix build_reduced_matrix(D13TangentLevel &level,
                                   const int mode_number,
                                   const StateRow &field_scales)
{
    const auto start = std::chrono::steady_clock::now();
    ReducedMatrix matrix;
    matrix.map.resize(static_cast<std::size_t>(reduced_size) * reduced_size);
    matrix.constraints.resize(
        static_cast<std::size_t>(reduced_constraint_size) * reduced_size);
    const auto representatives = representative_columns();
    std::set<int> representative_set(representatives.begin(),
                                     representatives.end());
    for (int column = 0; column < reduced_size; ++column)
    {
        std::vector<double> basis(static_cast<std::size_t>(reduced_size));
        basis[static_cast<std::size_t>(column)] = 1.0;
        const ActionResult primary = level.apply_tangent(
            physical_input(basis, field_scales), mode_number,
            d14_epsilon_primary);
        store_column(matrix, column, primary, field_scales);
        if (representative_set.count(column) != 0)
        {
            const ActionResult secondary = level.apply_tangent(
                physical_input(basis, field_scales), mode_number,
                d14_epsilon_secondary);
            const double difference =
                relative_difference(scaled_output(primary.response,
                                                  field_scales),
                                    scaled_output(secondary.response,
                                                  field_scales));
            matrix.epsilon_column_difference =
                std::max(matrix.epsilon_column_difference, difference);
            std::cout << std::scientific << std::setprecision(12)
                      << "D14_COLUMN_VALIDATION mode_number=" << mode_number
                      << " column=" << column
                      << " epsilon_primary=" << d14_epsilon_primary
                      << " epsilon_secondary=" << d14_epsilon_secondary
                      << " relative_difference=" << difference << '\n';
        }
        if ((column + 1) % 64 == 0 || column + 1 == reduced_size)
        {
            std::cout << "D14_MATRIX_PROGRESS mode_number=" << mode_number
                      << " columns=" << column + 1 << '/' << reduced_size
                      << '\n';
        }
    }
    d14_require(matrix.epsilon_column_difference <
                    column_agreement_tolerance,
                "representative columns fail epsilon-halving agreement");
    for (const double entry : matrix.map)
    {
        d14_require(std::isfinite(entry),
                    "reduced tangent matrix contains a nonfinite entry");
    }

    std::vector<double> probe(static_cast<std::size_t>(reduced_size));
    for (int index = 0; index < reduced_size; ++index)
    {
        probe[static_cast<std::size_t>(index)] =
            std::sin(0.013 * (index + 1)) +
            0.25 * std::cos(0.031 * (index + 1));
    }
    const double norm = vector_norm(probe);
    for (double &value : probe)
    {
        value /= norm;
    }
    const std::vector<double> dense = matrix_vector(matrix.map, probe);
    const ActionResult direct =
        level.apply_tangent(physical_input(probe, field_scales), mode_number,
                            d14_epsilon_primary);
    matrix.direct_vector_difference =
        relative_difference(dense,
                            scaled_output(direct.response, field_scales));
    d14_require(matrix.direct_vector_difference < direct_agreement_tolerance,
                "dense matrix does not reproduce direct D13 tangent action");
    const auto stop = std::chrono::steady_clock::now();
    matrix.construction_seconds =
        std::chrono::duration<double>(stop - start).count();
    std::cout << std::scientific << std::setprecision(12)
              << "D14_MATRIX_VALIDATION mode_number=" << mode_number
              << " n=" << reduced_size << " columns=" << reduced_size
              << " bytes=" << matrix.map.size() * sizeof(double)
              << " direct_vector_relative_difference="
              << matrix.direct_vector_difference
              << " maximum_epsilon_column_difference="
              << matrix.epsilon_column_difference
              << " maximum_parity_leakage="
              << matrix.maximum_parity_leakage
              << " maximum_harmonic_leakage="
              << matrix.maximum_harmonic_leakage
              << " matrix_hash=" << hash_string(hash_values(matrix.map))
              << " construction_seconds=" << matrix.construction_seconds
              << '\n';
    return matrix;
}

struct Eigensystem
{
    std::vector<double> real;
    std::vector<double> imaginary;
    std::vector<double> left;
    std::vector<double> right;
};

Eigensystem diagonalize(const std::vector<double> &matrix)
{
    d14_require(matrix.size() ==
                    static_cast<std::size_t>(reduced_size) * reduced_size,
                "eigensolver matrix has wrong size");
    std::vector<double> copy = matrix;
    Eigensystem result;
    result.real.resize(reduced_size);
    result.imaginary.resize(reduced_size);
    result.left.resize(static_cast<std::size_t>(reduced_size) * reduced_size);
    result.right.resize(static_cast<std::size_t>(reduced_size) * reduced_size);
    int n = reduced_size;
    int lda = n;
    int ldvl = n;
    int ldvr = n;
    int info = 0;
    int lwork = -1;
    double work_query = 0.0;
    char vectors = 'V';
    LAPACK(GEEV, geev)(&vectors, &vectors, &n, copy.data(), &lda,
                       result.real.data(), result.imaginary.data(),
                       result.left.data(), &ldvl, result.right.data(), &ldvr,
                       &work_query, &lwork, &info);
    d14_require(info == 0, "LAPACK dgeev workspace query failed");
    lwork = std::max(4 * n, static_cast<int>(std::ceil(work_query)));
    std::vector<double> work(static_cast<std::size_t>(lwork));
    copy = matrix;
    LAPACK(GEEV, geev)(&vectors, &vectors, &n, copy.data(), &lda,
                       result.real.data(), result.imaginary.data(),
                       result.left.data(), &ldvl, result.right.data(), &ldvr,
                       work.data(), &lwork, &info);
    d14_require(info == 0, "LAPACK dgeev failed to converge");
    return result;
}

ComplexVector eigenvector(const std::vector<double> &storage,
                          const std::vector<double> &imaginary,
                          const int index)
{
    ComplexVector result(static_cast<std::size_t>(reduced_size));
    if (imaginary[static_cast<std::size_t>(index)] == 0.0)
    {
        for (int row = 0; row < reduced_size; ++row)
        {
            result[static_cast<std::size_t>(row)] =
                storage[static_cast<std::size_t>(row) +
                        static_cast<std::size_t>(reduced_size) * index];
        }
        return result;
    }
    const int first =
        imaginary[static_cast<std::size_t>(index)] > 0.0 ? index : index - 1;
    const double sign =
        imaginary[static_cast<std::size_t>(index)] > 0.0 ? 1.0 : -1.0;
    for (int row = 0; row < reduced_size; ++row)
    {
        result[static_cast<std::size_t>(row)] =
            Complex(storage[static_cast<std::size_t>(row) +
                            static_cast<std::size_t>(reduced_size) * first],
                    sign *
                        storage[static_cast<std::size_t>(row) +
                                static_cast<std::size_t>(reduced_size) *
                                    (first + 1)]);
    }
    return result;
}

Complex inner_product(const ComplexVector &left,
                      const ComplexVector &right)
{
    d14_require(left.size() == right.size(),
                "complex vectors differ in size");
    Complex result = 0.0;
    for (std::size_t i = 0; i < left.size(); ++i)
    {
        result += std::conj(left[i]) * right[i];
    }
    return result;
}

struct Candidate
{
    int index = 0;
    Complex lambda = 0.0;
    Complex omega = 0.0;
    ComplexVector scaled_right;
    ComplexVector scaled_left;
    ComplexVector physical_right;
    ComplexVector physical_left;
    double left_right_overlap = 0.0;
    double physical_conditioning_proxy = 0.0;
    double legacy_scaled_coordinate_proxy = 0.0;
    double denominator_relative_error = 0.0;
    double secondary_scaling_proxy_relative_error = 0.0;
    double physical_right_residual = 0.0;
    double physical_left_residual = 0.0;
    StateRow component_fractions{};
    std::array<double, constraint_count> constraints{};
    double total_constraint = 0.0;
    double boundary_fraction = 0.0;
    double nyquist_fraction = 0.0;
    int maximum_radial = 0;
    int nontrivial_fields = 0;
    double dense_residual = 0.0;
    double direct_residual = std::numeric_limits<double>::infinity();
    double parity_leakage = std::numeric_limits<double>::infinity();
    double harmonic_leakage = std::numeric_limits<double>::infinity();
    double scaling_eigenvalue_difference =
        std::numeric_limits<double>::infinity();
    double scaling_profile_overlap = 0.0;
    double scaling_left_profile_overlap = 0.0;
    double unscaled_right_residual = 0.0;
    double unscaled_left_residual = 0.0;
    bool preliminary_physical = false;
};

ComplexVector apply_constraint_matrix(const ReducedMatrix &matrix,
                                      const ComplexVector &scaled)
{
    ComplexVector output(static_cast<std::size_t>(reduced_constraint_size));
    for (int column = 0; column < reduced_size; ++column)
    {
        for (int row = 0; row < reduced_constraint_size; ++row)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix.constraints[static_cast<std::size_t>(row) +
                                   static_cast<std::size_t>(
                                       reduced_constraint_size) *
                                       column] *
                scaled[static_cast<std::size_t>(column)];
        }
    }
    return output;
}

double deterministic_secondary_scale(const int index)
{
    return std::exp(0.2 * std::sin(0.017 * (index + 1)) +
                    0.1 * std::cos(0.031 * (index + 1)));
}

double conditioning_proxy(const ComplexVector &left,
                          const ComplexVector &right,
                          const Complex denominator)
{
    return complex_norm(left) * complex_norm(right) /
           std::max(std::abs(denominator),
                    std::numeric_limits<double>::min());
}

std::vector<double> expanded_field_scaling(const StateRow &field_scales)
{
    std::vector<double> result(static_cast<std::size_t>(reduced_size));
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            result[reduced_index(radial, component)] =
                field_scales[static_cast<std::size_t>(component)];
        }
    }
    return result;
}

std::vector<double> secondary_scaling_vector()
{
    std::vector<double> result(static_cast<std::size_t>(reduced_size));
    for (int coordinate = 0; coordinate < reduced_size; ++coordinate)
    {
        result[static_cast<std::size_t>(coordinate)] =
            deterministic_secondary_scale(coordinate);
    }
    return result;
}

void validate_conditioning_formula_regression()
{
    const ComplexVector scaled_right = {
        Complex(1.0, 0.25), Complex(-0.3, 0.8),
        Complex(0.6, -0.2), Complex(-0.4, -0.5)};
    const ComplexVector scaled_left = {
        Complex(0.4, -0.1), Complex(1.2, 0.3),
        Complex(-0.5, 0.7), Complex(0.2, -0.9)};
    const std::array<double, 4> diagonal = {0.25, 0.8, 2.5, 6.0};
    const std::array<double, 4> secondary = {1.4, 0.6, 2.2, 0.9};
    ComplexVector physical_right(4);
    ComplexVector physical_left(4);
    ComplexVector twice_scaled_right(4);
    ComplexVector twice_scaled_left(4);
    ComplexVector recovered_right(4);
    ComplexVector recovered_left(4);
    for (std::size_t i = 0; i < diagonal.size(); ++i)
    {
        physical_right[i] = diagonal[i] * scaled_right[i];
        physical_left[i] = scaled_left[i] / diagonal[i];
        twice_scaled_right[i] = physical_right[i] / secondary[i];
        twice_scaled_left[i] = physical_left[i] * secondary[i];
        recovered_right[i] = secondary[i] * twice_scaled_right[i];
        recovered_left[i] = twice_scaled_left[i] / secondary[i];
    }
    const Complex scaled_denominator =
        inner_product(scaled_left, scaled_right);
    const Complex physical_denominator =
        inner_product(physical_left, physical_right);
    const Complex twice_scaled_denominator =
        inner_product(twice_scaled_left, twice_scaled_right);
    const double physical_proxy =
        conditioning_proxy(physical_left, physical_right,
                           scaled_denominator);
    const double recovered_proxy =
        conditioning_proxy(recovered_left, recovered_right,
                           twice_scaled_denominator);
    const double legacy_proxy =
        conditioning_proxy(scaled_left, scaled_right,
                           scaled_denominator);
    const double denominator_error =
        std::abs(physical_denominator - scaled_denominator) /
        std::abs(scaled_denominator);
    const double secondary_error =
        std::abs(recovered_proxy - physical_proxy) / physical_proxy;
    const double legacy_difference =
        std::abs(legacy_proxy - physical_proxy) / physical_proxy;
    d14_require(denominator_error < 1.0e-14,
                "synthetic denominator identity failed");
    d14_require(secondary_error < 1.0e-14,
                "synthetic secondary-scaling invariance failed");
    d14_require(legacy_difference > 0.1,
                "synthetic legacy scaled-left mutation survived");
    std::cout << std::scientific << std::setprecision(12)
              << "D14_CONDITIONING_FORMULA_REGRESSION"
              << " physical_proxy=" << physical_proxy
              << " legacy_scaled_coordinate_proxy=" << legacy_proxy
              << " denominator_relative_error=" << denominator_error
              << " secondary_scaling_relative_error=" << secondary_error
              << " legacy_relative_difference=" << legacy_difference
              << " legacy_scaled_left_mutation_rejected=1\n";
}

Candidate make_candidate(const ReducedMatrix &matrix,
                         const Eigensystem &eigensystem, const int index,
                         const StateRow &field_scales, const double dx,
                         const double dt,
                         const std::vector<double> &physical_map)
{
    Candidate candidate;
    candidate.index = index;
    candidate.lambda =
        Complex(eigensystem.real[static_cast<std::size_t>(index)],
                eigensystem.imaginary[static_cast<std::size_t>(index)]);
    candidate.omega = std::log(candidate.lambda) / dt;
    candidate.scaled_right =
        eigenvector(eigensystem.right, eigensystem.imaginary, index);
    candidate.scaled_left =
        eigenvector(eigensystem.left, eigensystem.imaginary, index);
    const Complex scaled_denominator =
        inner_product(candidate.scaled_left, candidate.scaled_right);
    candidate.physical_right =
        physical_vector(candidate.scaled_right, field_scales);
    candidate.physical_left =
        physical_left_vector(candidate.scaled_left, field_scales);
    const Complex physical_denominator =
        inner_product(candidate.physical_left, candidate.physical_right);
    candidate.denominator_relative_error =
        std::abs(physical_denominator - scaled_denominator) /
        std::max(std::abs(scaled_denominator),
                 std::numeric_limits<double>::min());
    candidate.left_right_overlap =
        std::abs(physical_denominator) /
        (complex_norm(candidate.physical_left) *
         complex_norm(candidate.physical_right));
    candidate.physical_conditioning_proxy =
        conditioning_proxy(candidate.physical_left,
                           candidate.physical_right,
                           scaled_denominator);
    candidate.legacy_scaled_coordinate_proxy =
        conditioning_proxy(candidate.scaled_left,
                           candidate.scaled_right,
                           scaled_denominator);
    candidate.physical_right_residual =
        right_eigen_residual(physical_map, candidate.physical_right,
                             candidate.lambda);
    candidate.physical_left_residual =
        left_eigen_residual(physical_map, candidate.physical_left,
                            candidate.lambda);

    ComplexVector secondary_scaled_right(candidate.physical_right.size());
    ComplexVector secondary_scaled_left(candidate.physical_left.size());
    ComplexVector secondary_physical_right(candidate.physical_right.size());
    ComplexVector secondary_physical_left(candidate.physical_left.size());
    for (int coordinate = 0; coordinate < reduced_size; ++coordinate)
    {
        const std::size_t i = static_cast<std::size_t>(coordinate);
        const double scale_value =
            deterministic_secondary_scale(coordinate);
        secondary_scaled_right[i] =
            candidate.physical_right[i] / scale_value;
        secondary_scaled_left[i] =
            candidate.physical_left[i] * scale_value;
        secondary_physical_right[i] =
            secondary_scaled_right[i] * scale_value;
        secondary_physical_left[i] =
            secondary_scaled_left[i] / scale_value;
    }
    const double secondary_proxy =
        conditioning_proxy(
            secondary_physical_left, secondary_physical_right,
            inner_product(secondary_scaled_left,
                          secondary_scaled_right));
    candidate.secondary_scaling_proxy_relative_error =
        std::abs(secondary_proxy -
                 candidate.physical_conditioning_proxy) /
        std::max(candidate.physical_conditioning_proxy,
                 std::numeric_limits<double>::min());

    const double physical_right_norm =
        complex_norm(candidate.physical_right);
    const double physical_left_norm =
        complex_norm(candidate.physical_left);
    for (Complex &value : candidate.physical_right)
    {
        value /= physical_right_norm;
    }
    for (Complex &value : candidate.scaled_right)
    {
        value /= physical_right_norm;
    }
    for (Complex &value : candidate.physical_left)
    {
        value /= physical_left_norm;
    }
    for (Complex &value : candidate.scaled_left)
    {
        value /= physical_left_norm;
    }

    double total_energy = 0.0;
    double boundary_energy = 0.0;
    double maximum_energy = -1.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        double radial_energy = 0.0;
        for (int component = 0; component < component_count; ++component)
        {
            const double energy =
                std::norm(candidate.physical_right[
                    reduced_index(radial, component)]);
            candidate.component_fractions[static_cast<std::size_t>(
                component)] += energy;
            total_energy += energy;
            radial_energy += energy;
            if (radial < 3 || radial >= radial_cells - 3)
            {
                boundary_energy += energy;
            }
        }
        if (radial_energy > maximum_energy)
        {
            maximum_energy = radial_energy;
            candidate.maximum_radial = radial;
        }
    }
    for (double &fraction : candidate.component_fractions)
    {
        fraction /= total_energy;
        if (fraction > 1.0e-4)
        {
            ++candidate.nontrivial_fields;
        }
    }
    candidate.boundary_fraction = boundary_energy / total_energy;

    double nyquist_energy = 0.0;
    for (int component = 0; component < component_count; ++component)
    {
        Complex alternating = 0.0;
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            alternating +=
                (radial % 2 == 0 ? 1.0 : -1.0) *
                candidate.physical_right[
                    reduced_index(radial, component)];
        }
        nyquist_energy += std::norm(alternating) / radial_cells;
    }
    candidate.nyquist_fraction = nyquist_energy / total_energy;

    const ComplexVector constraint =
        apply_constraint_matrix(matrix, candidate.scaled_right);
    for (int component = 0; component < constraint_count; ++component)
    {
        double sum = 0.0;
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            sum += std::norm(constraint[constraint_reduced_index(
                radial, component)]);
        }
        candidate.constraints[static_cast<std::size_t>(component)] =
            std::sqrt(sum * dx) / std::sqrt(dx);
    }
    candidate.total_constraint =
        std::sqrt(candidate.constraints[0] * candidate.constraints[0] +
                  candidate.constraints[1] * candidate.constraints[1] +
                  candidate.constraints[2] * candidate.constraints[2]);

    const ComplexVector dense =
        matrix_vector(matrix.map, candidate.scaled_right);
    ComplexVector residual = dense;
    for (int row = 0; row < reduced_size; ++row)
    {
        residual[static_cast<std::size_t>(row)] -=
            candidate.lambda *
            candidate.scaled_right[static_cast<std::size_t>(row)];
    }
    candidate.dense_residual =
        complex_norm(residual) / complex_norm(candidate.scaled_right);
    return candidate;
}

ActionResult direct_complex_part(D13TangentLevel &level,
                                 const ComplexVector &scaled,
                                 const StateRow &field_scales,
                                 const int mode_number,
                                 const bool imaginary_part)
{
    std::vector<double> part(static_cast<std::size_t>(reduced_size));
    for (int index = 0; index < reduced_size; ++index)
    {
        const Complex value = scaled[static_cast<std::size_t>(index)];
        part[static_cast<std::size_t>(index)] =
            imaginary_part ? value.imag() : value.real();
    }
    if (vector_norm(part) == 0.0)
    {
        return {};
    }
    return level.apply_tangent(physical_input(part, field_scales),
                               mode_number, d14_epsilon_primary);
}

void add_scaled_output(ComplexVector &output, const ActionResult &action,
                       const StateRow &field_scales, const bool imaginary)
{
    const std::vector<double> part =
        scaled_output(action.response, field_scales);
    for (int index = 0; index < reduced_size; ++index)
    {
        if (imaginary)
        {
            output[static_cast<std::size_t>(index)] +=
                Complex(0.0, part[static_cast<std::size_t>(index)]);
        }
        else
        {
            output[static_cast<std::size_t>(index)] +=
                part[static_cast<std::size_t>(index)];
        }
    }
}

void validate_candidate_direct(Candidate &candidate, D13TangentLevel &level,
                               const StateRow &field_scales,
                               const int mode_number)
{
    const ActionResult real_action = direct_complex_part(
        level, candidate.scaled_right, field_scales, mode_number, false);
    const ActionResult imaginary_action = direct_complex_part(
        level, candidate.scaled_right, field_scales, mode_number, true);
    ComplexVector direct(static_cast<std::size_t>(reduced_size));
    add_scaled_output(direct, real_action, field_scales, false);
    add_scaled_output(direct, imaginary_action, field_scales, true);
    for (int index = 0; index < reduced_size; ++index)
    {
        direct[static_cast<std::size_t>(index)] -=
            candidate.lambda *
            candidate.scaled_right[static_cast<std::size_t>(index)];
    }
    candidate.direct_residual =
        complex_norm(direct) / complex_norm(candidate.scaled_right);
    candidate.parity_leakage =
        std::max(real_action.parity_leakage,
                 imaginary_action.parity_leakage);
    candidate.harmonic_leakage =
        std::max(real_action.harmonic_leakage,
                 imaginary_action.harmonic_leakage);
}

std::vector<double> unscaled_matrix(const std::vector<double> &scaled_matrix,
                                    const StateRow &field_scales)
{
    std::vector<double> result = scaled_matrix;
    for (int column = 0; column < reduced_size; ++column)
    {
        const int column_component = column % component_count;
        for (int row = 0; row < reduced_size; ++row)
        {
            const int row_component = row % component_count;
            result[static_cast<std::size_t>(row) +
                   static_cast<std::size_t>(reduced_size) * column] *=
                field_scales[static_cast<std::size_t>(row_component)] /
                field_scales[static_cast<std::size_t>(column_component)];
        }
    }
    return result;
}

int matching_eigenvalue(const Eigensystem &eigensystem,
                        const Complex target)
{
    int best = 0;
    double best_difference = std::numeric_limits<double>::infinity();
    for (int index = 0; index < reduced_size; ++index)
    {
        const Complex value(
            eigensystem.real[static_cast<std::size_t>(index)],
            eigensystem.imaginary[static_cast<std::size_t>(index)]);
        const double difference = std::abs(value - target);
        if (difference < best_difference)
        {
            best = index;
            best_difference = difference;
        }
    }
    return best;
}

void validate_scaling_identity(Candidate &candidate,
                               const Eigensystem &unscaled_eigensystem,
                               const std::vector<double> &physical_map)
{
    const int match =
        matching_eigenvalue(unscaled_eigensystem, candidate.lambda);
    const Complex unscaled_lambda(
        unscaled_eigensystem.real[static_cast<std::size_t>(match)],
        unscaled_eigensystem.imaginary[static_cast<std::size_t>(match)]);
    candidate.scaling_eigenvalue_difference =
        std::abs(unscaled_lambda - candidate.lambda);
    ComplexVector unscaled_right = eigenvector(
        unscaled_eigensystem.right, unscaled_eigensystem.imaginary, match);
    const double norm = complex_norm(unscaled_right);
    for (Complex &value : unscaled_right)
    {
        value /= norm;
    }
    ComplexVector physical = candidate.physical_right;
    const double physical_norm = complex_norm(physical);
    for (Complex &value : physical)
    {
        value /= physical_norm;
    }
    candidate.scaling_profile_overlap =
        std::abs(inner_product(unscaled_right, physical));
    ComplexVector unscaled_left = eigenvector(
        unscaled_eigensystem.left, unscaled_eigensystem.imaginary, match);
    const double left_norm = complex_norm(unscaled_left);
    for (Complex &value : unscaled_left)
    {
        value /= left_norm;
    }
    ComplexVector physical_left = candidate.physical_left;
    const double physical_left_norm = complex_norm(physical_left);
    for (Complex &value : physical_left)
    {
        value /= physical_left_norm;
    }
    candidate.scaling_left_profile_overlap =
        std::abs(inner_product(unscaled_left, physical_left));
    candidate.unscaled_right_residual =
        right_eigen_residual(physical_map, unscaled_right,
                             unscaled_lambda);
    candidate.unscaled_left_residual =
        left_eigen_residual(physical_map, unscaled_left,
                            unscaled_lambda);
}

void print_candidate(const Candidate &candidate, const int mode_number,
                     const std::string &rank, const int rank_index,
                     const std::array<double, radial_cells> &x)
{
    std::cout << std::scientific << std::setprecision(12)
              << "D14_CANDIDATE mode_number=" << mode_number
              << " rank=" << rank << " rank_index=" << rank_index
              << " eigen_index=" << candidate.index
              << " lambda_real=" << candidate.lambda.real()
              << " lambda_imag=" << candidate.lambda.imag()
              << " Omega_real=" << candidate.omega.real()
              << " Omega_imag=" << candidate.omega.imag()
              << " conjugate_pair="
              << (candidate.lambda.imag() != 0.0 ? 1 : 0)
              << " dense_residual=" << candidate.dense_residual
              << " direct_matrix_free_residual="
              << candidate.direct_residual
              << " physical_left_right_overlap="
              << candidate.left_right_overlap
              << " physical_coordinate_eigenvalue_conditioning_proxy="
              << candidate.physical_conditioning_proxy
              << " legacy_scaled_coordinate_proxy="
              << candidate.legacy_scaled_coordinate_proxy
              << " physical_right_residual="
              << candidate.physical_right_residual
              << " physical_left_residual="
              << candidate.physical_left_residual
              << " denominator_relative_error="
              << candidate.denominator_relative_error
              << " secondary_scaling_proxy_relative_error="
              << candidate.secondary_scaling_proxy_relative_error
              << " boundary_six_cell_fraction="
              << candidate.boundary_fraction
              << " radial_nyquist_fraction="
              << candidate.nyquist_fraction
              << " parity_leakage=" << candidate.parity_leakage
              << " harmonic_leakage=" << candidate.harmonic_leakage
              << " H_over_state=" << candidate.constraints[0]
              << " Mx_over_state=" << candidate.constraints[1]
              << " Mz_over_state=" << candidate.constraints[2]
              << " total_constraint_over_state="
              << candidate.total_constraint
              << " nontrivial_fields=" << candidate.nontrivial_fields
              << " maximum_radial_index=" << candidate.maximum_radial
              << " maximum_x="
              << x[static_cast<std::size_t>(candidate.maximum_radial)]
              << " scaling_eigenvalue_difference="
              << candidate.scaling_eigenvalue_difference
              << " scaling_profile_overlap="
              << candidate.scaling_profile_overlap
              << " scaling_left_profile_overlap="
              << candidate.scaling_left_profile_overlap
              << " unscaled_right_residual="
              << candidate.unscaled_right_residual
              << " unscaled_left_residual="
              << candidate.unscaled_left_residual
              << " physical_right_hash="
              << hash_string(hash_values(candidate.physical_right))
              << " physical_left_hash="
              << hash_string(hash_values(candidate.physical_left))
              << " preliminary_physical_filter="
              << (candidate.preliminary_physical ? 1 : 0) << '\n';
    for (int component = 0; component < component_count; ++component)
    {
        std::cout << "D14_COMPONENT mode_number=" << mode_number
                  << " eigen_index=" << candidate.index
                  << " slot=" << component
                  << " name="
                  << UserVariables::variable_names[
                         static_cast<std::size_t>(component)]
                  << " fraction="
                  << candidate.component_fractions[
                         static_cast<std::size_t>(component)]
                  << '\n';
    }
    const double physical_norm =
        complex_norm(candidate.physical_right);
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        double row_energy = 0.0;
        for (int component = 0; component < component_count; ++component)
        {
            row_energy += std::norm(candidate.physical_right[
                reduced_index(radial, component)]);
        }
        std::cout << "D14_PROFILE mode_number=" << mode_number
                  << " eigen_index=" << candidate.index
                  << " radial_index=" << radial
                  << " x=" << x[static_cast<std::size_t>(radial)]
                  << " normalized_row_norm="
                  << std::sqrt(row_energy) / physical_norm;
        for (int component = 0; component < component_count; ++component)
        {
            const Complex value = candidate.physical_right[
                reduced_index(radial, component)] /
                                  physical_norm;
            std::cout << " v" << component << "_real=" << value.real()
                      << " v" << component << "_imag=" << value.imag();
        }
        std::cout << '\n';
    }
}

struct ModeSummary
{
    bool preliminary_physical = false;
    bool spectral_radius_is_boundary_constraint_or_gauge = false;
    bool severe_nonnormality = false;
    bool clustered_leading_spectrum = false;
    bool complex_leading = false;
    double leading_gap = 0.0;
    double maximum_conditioning_proxy = 0.0;
    double leading_conditioning_proxy = 0.0;
    double leading_legacy_scaled_proxy = 0.0;
    int direct_candidate_count = 0;
};

ModeSummary analyze_spectrum(D13TangentLevel &level,
                             const ReducedMatrix &matrix,
                             const int mode_number,
                             const StateRow &field_scales,
                             const std::array<double, radial_cells> &x)
{
    const auto start = std::chrono::steady_clock::now();
    const Eigensystem eigensystem = diagonalize(matrix.map);
    const std::vector<double> physical_map =
        unscaled_matrix(matrix.map, field_scales);
    const Eigensystem unscaled_eigensystem = diagonalize(physical_map);
    std::vector<Candidate> candidates;
    candidates.reserve(reduced_size);
    for (int index = 0; index < reduced_size; ++index)
    {
        Candidate candidate =
            make_candidate(matrix, eigensystem, index, field_scales,
                           level.spacing(), level.timestep(), physical_map);
        validate_scaling_identity(candidate, unscaled_eigensystem,
                                  physical_map);
        candidates.push_back(std::move(candidate));
        std::cout << std::scientific << std::setprecision(12)
                  << "D14_EIGENVALUE mode_number=" << mode_number
                  << " index=" << index
                  << " lambda_real="
                  << eigensystem.real[static_cast<std::size_t>(index)]
                  << " lambda_imag="
                  << eigensystem.imaginary[static_cast<std::size_t>(index)]
                  << " Omega_real=" << candidates.back().omega.real()
                  << " Omega_imag=" << candidates.back().omega.imag()
                  << " boundary_fraction="
                  << candidates.back().boundary_fraction
                  << " nyquist_fraction="
                  << candidates.back().nyquist_fraction
                  << " constraint_norm="
                  << candidates.back().total_constraint
                  << " physical_conditioning_proxy="
                  << candidates.back().physical_conditioning_proxy
                  << " legacy_scaled_coordinate_proxy="
                  << candidates.back().legacy_scaled_coordinate_proxy
                  << " physical_right_residual="
                  << candidates.back().physical_right_residual
                  << " physical_left_residual="
                  << candidates.back().physical_left_residual
                  << " denominator_relative_error="
                  << candidates.back().denominator_relative_error
                  << " secondary_scaling_proxy_relative_error="
                  << candidates.back()
                         .secondary_scaling_proxy_relative_error
                  << " nontrivial_fields="
                  << candidates.back().nontrivial_fields
                  << " scaling_eigenvalue_difference="
                  << candidates.back().scaling_eigenvalue_difference
                  << " scaling_profile_overlap="
                  << candidates.back().scaling_profile_overlap
                  << " scaling_left_profile_overlap="
                  << candidates.back().scaling_left_profile_overlap << '\n';
    }

    std::vector<int> growth(reduced_size);
    std::iota(growth.begin(), growth.end(), 0);
    std::sort(growth.begin(), growth.end(), [&](const int left,
                                                const int right) {
        return candidates[static_cast<std::size_t>(left)].omega.real() >
               candidates[static_cast<std::size_t>(right)].omega.real();
    });
    std::vector<int> constraints = growth;
    std::sort(constraints.begin(), constraints.end(),
              [&](const int left, const int right) {
                  return candidates[static_cast<std::size_t>(left)]
                             .total_constraint <
                         candidates[static_cast<std::size_t>(right)]
                             .total_constraint;
              });
    std::vector<int> boundary = growth;
    std::sort(boundary.begin(), boundary.end(),
              [&](const int left, const int right) {
                  return candidates[static_cast<std::size_t>(left)]
                             .boundary_fraction <
                         candidates[static_cast<std::size_t>(right)]
                             .boundary_fraction;
              });

    std::set<int> leading;
    for (int rank = 0; rank < 3; ++rank)
    {
        leading.insert(growth[static_cast<std::size_t>(rank)]);
        leading.insert(constraints[static_cast<std::size_t>(rank)]);
        leading.insert(boundary[static_cast<std::size_t>(rank)]);
    }
    std::vector<int> physical_prefilter;
    for (int index = 0; index < reduced_size; ++index)
    {
        const Candidate &candidate =
            candidates[static_cast<std::size_t>(index)];
        const bool passes =
            candidate.dense_residual < 1.0e-8 &&
            std::abs(candidate.lambda) > 1.0e-5 &&
            std::abs(candidate.omega.real()) > minimum_resolved_rate &&
            candidate.boundary_fraction < 0.35 &&
            candidate.nyquist_fraction < 0.20 &&
            candidate.nontrivial_fields >= 3 &&
            candidate.total_constraint < 0.25 &&
            candidate.scaling_eigenvalue_difference < 1.0e-8 &&
            candidate.scaling_profile_overlap > 0.99;
        if (passes)
        {
            physical_prefilter.push_back(index);
        }
    }
    std::sort(physical_prefilter.begin(), physical_prefilter.end(),
              [&](const int left, const int right) {
                  return candidates[static_cast<std::size_t>(left)]
                             .omega.real() >
                         candidates[static_cast<std::size_t>(right)]
                             .omega.real();
              });
    constexpr std::size_t direct_prefilter_cap = 12;
    for (std::size_t i = 0;
         i < std::min(direct_prefilter_cap, physical_prefilter.size()); ++i)
    {
        leading.insert(physical_prefilter[i]);
    }
    std::cout << "D14_PHYSICAL_PREFILTER mode_number=" << mode_number
              << " passing=" << physical_prefilter.size()
              << " direct_checked="
              << std::min(direct_prefilter_cap, physical_prefilter.size())
              << '\n';
    for (const int index : leading)
    {
        Candidate &candidate = candidates[static_cast<std::size_t>(index)];
        validate_candidate_direct(candidate, level, field_scales,
                                  mode_number);
        candidate.preliminary_physical =
            candidate.direct_residual < 5.0e-6 &&
            std::abs(candidate.lambda) >
                10.0 * candidate.direct_residual &&
            std::abs(candidate.omega.real()) > minimum_resolved_rate &&
            candidate.boundary_fraction < 0.35 &&
            candidate.nyquist_fraction < 0.20 &&
            candidate.parity_leakage < 1.0e-7 &&
            candidate.harmonic_leakage < 1.0e-10 &&
            candidate.nontrivial_fields >= 3 &&
            candidate.total_constraint < 0.25 &&
            candidate.scaling_eigenvalue_difference < 1.0e-8 &&
            candidate.scaling_profile_overlap > 0.99;
    }

    for (int rank = 0; rank < 3; ++rank)
    {
        const std::array<std::pair<const char *, int>, 3> ranked = {
            std::pair<const char *, int>{
                "largest_ReOmega",
                growth[static_cast<std::size_t>(rank)]},
            std::pair<const char *, int>{
                "smallest_constraint",
                constraints[static_cast<std::size_t>(rank)]},
            std::pair<const char *, int>{
                "weakest_boundary",
                boundary[static_cast<std::size_t>(rank)]}};
        for (const auto &entry : ranked)
        {
            print_candidate(
                candidates[static_cast<std::size_t>(entry.second)],
                mode_number, entry.first, rank + 1, x);
        }
    }

    ModeSummary summary;
    summary.direct_candidate_count = static_cast<int>(leading.size());
    double maximum_physical_right_residual = 0.0;
    double maximum_physical_left_residual = 0.0;
    double maximum_denominator_error = 0.0;
    double maximum_secondary_scaling_error = 0.0;
    double maximum_legacy_relative_difference = 0.0;
    int packed_complex_eigenvectors = 0;
    for (const Candidate &candidate : candidates)
    {
        summary.preliminary_physical =
            summary.preliminary_physical ||
            candidate.preliminary_physical;
        summary.maximum_conditioning_proxy =
            std::max(summary.maximum_conditioning_proxy,
                     candidate.physical_conditioning_proxy);
        maximum_physical_right_residual =
            std::max(maximum_physical_right_residual,
                     candidate.physical_right_residual);
        maximum_physical_left_residual =
            std::max(maximum_physical_left_residual,
                     candidate.physical_left_residual);
        maximum_denominator_error =
            std::max(maximum_denominator_error,
                     candidate.denominator_relative_error);
        maximum_secondary_scaling_error =
            std::max(maximum_secondary_scaling_error,
                     candidate.secondary_scaling_proxy_relative_error);
        maximum_legacy_relative_difference =
            std::max(
                maximum_legacy_relative_difference,
                std::abs(candidate.legacy_scaled_coordinate_proxy -
                         candidate.physical_conditioning_proxy) /
                    std::max(candidate.physical_conditioning_proxy,
                             std::numeric_limits<double>::min()));
        if (candidate.lambda.imag() != 0.0)
        {
            ++packed_complex_eigenvectors;
        }
    }
    const Candidate &spectral_leader =
        candidates[static_cast<std::size_t>(growth.front())];
    summary.leading_conditioning_proxy =
        spectral_leader.physical_conditioning_proxy;
    summary.leading_legacy_scaled_proxy =
        spectral_leader.legacy_scaled_coordinate_proxy;
    double gauge_fraction = 0.0;
    for (const int component :
         {c_lapse, c_shiftX, c_shiftZ, c_Bx, c_Bz})
    {
        gauge_fraction += spectral_leader.component_fractions[
            static_cast<std::size_t>(component)];
    }
    summary.spectral_radius_is_boundary_constraint_or_gauge =
        spectral_leader.boundary_fraction >= 0.35 ||
        spectral_leader.total_constraint >= 0.25 ||
        gauge_fraction >= 0.50;
    summary.severe_nonnormality =
        spectral_leader.physical_conditioning_proxy >= 1.0e3;
    summary.complex_leading =
        std::abs(spectral_leader.lambda.imag()) > 1.0e-12;
    summary.leading_gap =
        candidates[static_cast<std::size_t>(growth[0])].omega.real() -
        candidates[static_cast<std::size_t>(growth[1])].omega.real();
    summary.clustered_leading_spectrum =
        std::abs(summary.leading_gap) < 0.05;
    d14_require(maximum_physical_right_residual < 1.0e-9,
                "physical right eigenvector residual gate failed");
    d14_require(maximum_physical_left_residual < 1.0e-9,
                "physical left eigenvector residual gate failed");
    d14_require(maximum_denominator_error < 1.0e-7,
                "left/right denominator invariance gate failed");
    d14_require(maximum_secondary_scaling_error < 1.0e-7,
                "secondary diagonal scaling invariance gate failed");
    d14_require(maximum_legacy_relative_difference > 1.0e-2,
                "legacy scaled-left mutation was not detected");
    d14_require(spectral_leader.scaling_profile_overlap > 0.99,
                "unscaled comparison rejected transformed leading right");
    d14_require(spectral_leader.scaling_left_profile_overlap > 0.99,
                "unscaled comparison rejected transformed leading left");
    d14_require(spectral_leader.unscaled_right_residual < 1.0e-9 &&
                    spectral_leader.unscaled_left_residual < 1.0e-9,
                "unscaled left/right residual gate failed");
    const auto stop = std::chrono::steady_clock::now();
    std::cout << std::scientific << std::setprecision(12)
              << "D14_CONDITIONING_REGRESSION mode_number="
              << mode_number
              << " maximum_physical_right_residual="
              << maximum_physical_right_residual
              << " maximum_physical_left_residual="
              << maximum_physical_left_residual
              << " maximum_denominator_relative_error="
              << maximum_denominator_error
              << " maximum_secondary_scaling_proxy_relative_error="
              << maximum_secondary_scaling_error
              << " maximum_legacy_proxy_relative_difference="
              << maximum_legacy_relative_difference
              << " legacy_scaled_left_mutation_rejected=1"
              << " packed_complex_eigenvectors="
              << packed_complex_eigenvectors
              << " leading_unscaled_right_overlap="
              << spectral_leader.scaling_profile_overlap
              << " leading_unscaled_left_overlap="
              << spectral_leader.scaling_left_profile_overlap
              << " leading_unscaled_right_residual="
              << spectral_leader.unscaled_right_residual
              << " leading_unscaled_left_residual="
              << spectral_leader.unscaled_left_residual << '\n';
    std::cout << std::scientific << std::setprecision(12)
              << "D14_MODE_SUMMARY mode_number=" << mode_number
              << " preliminary_physical="
              << (summary.preliminary_physical ? 1 : 0)
              << " leading_boundary_constraint_or_gauge="
              << (summary.spectral_radius_is_boundary_constraint_or_gauge
                      ? 1
                      : 0)
              << " severe_nonnormality="
              << (summary.severe_nonnormality ? 1 : 0)
              << " complex_leading="
              << (summary.complex_leading ? 1 : 0)
              << " clustered_leading="
              << (summary.clustered_leading_spectrum ? 1 : 0)
              << " leading_ReOmega_gap=" << summary.leading_gap
              << " leading_physical_conditioning_proxy="
              << summary.leading_conditioning_proxy
              << " leading_legacy_scaled_coordinate_proxy="
              << summary.leading_legacy_scaled_proxy
              << " maximum_physical_conditioning_proxy="
              << summary.maximum_conditioning_proxy
              << " direct_candidates=" << summary.direct_candidate_count
              << " diagonalization_and_analysis_seconds="
              << std::chrono::duration<double>(stop - start).count()
              << '\n';
    return summary;
}

int run_d14(SimulationParameters &parameters)
{
    d14_require(parameters.max_level == 0, "D14 requires max_level=0");
    d14_require(parameters.coarsest_dx == 0.125,
                "D14 requires dx=dz=1/8");
    d14_require(parameters.dt_multiplier == 0.05,
                "D14 requires CFL=0.05");
    d14_require(parameters.ko_sigma == 0.3,
                "D14 requires ko_sigma=0.3");
    d14_require(parameters.constraint_diagnostic_cadence == 0,
                "D14 production diagnostics must be disabled");
    const auto start = std::chrono::steady_clock::now();

    GRAMR amr;
    DefaultLevelFactory<D13TangentLevel> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    d14_require(levels.size() == 1,
                "D14 must create exactly one level");
    auto *level = dynamic_cast<D13TangentLevel *>(levels[0]);
    d14_require(level != nullptr, "D14 factory returned wrong level");
    level->prepare();
    const StateRow field_scales = level->field_scales();
    const auto x = level->radial_coordinates();
    const std::vector<double> expanded_scales =
        expanded_field_scaling(field_scales);
    const std::vector<double> secondary_scales =
        secondary_scaling_vector();
    validate_conditioning_formula_regression();

    std::cout << std::scientific << std::setprecision(12)
              << "D14_LAPACK source=locked_Chombo_USE_LAPACK"
              << " routine=dgeev linked_by=Chombo_syslibflags"
              << " new_dependency=0\n"
              << "D14_CONFIGURATION Lz=8.0 x_in=5.0e-1 x_out=4.5"
              << " Nx=" << radial_cells << " Nz=" << compact_cells
              << " dx=" << level->spacing() << " dz=" << level->spacing()
              << " dt=" << level->timestep() << " CFL=5.0e-2"
              << " ko_sigma=3.0e-1 reduced_slots="
              << component_count << " reduced_size=" << reduced_size
              << " dense_matrices=2 optional_fine_matrix=0\n"
              << "D14_SIMILARITY coordinates=field_scaled"
              << " definition=A_scaled=D_inverse_A_physical_D"
              << " right_transform=v=D_v_scaled"
              << " left_transform=w=D_inverse_w_scaled"
              << " eigenvalues_unchanged=1\n"
              << "D14_SCALING_HASH field_scaling_fnv1a64="
              << hash_string(hash_values(expanded_scales))
              << " secondary_scaling_fnv1a64="
              << hash_string(hash_values(secondary_scales)) << '\n'
              << "D14_RATE_RESOLUTION minimum_abs_ReOmega="
              << minimum_resolved_rate
              << " basis=conservative_dense_vs_direct_action_error_over_dt"
              << " near_neutral_signs=unresolved\n"
              << "D14_LIFECYCLE action=D13_validated_signed_one_step"
              << " periodic_exchange=framework"
              << " radial_ghosts=black_string_policy"
              << " rhs=direct_target_d4 gauge=live"
              << " fixed_lapse_source=enabled ko_sigma=0.3"
              << " outer_rhs_override=enabled rk=locked_four_stage"
              << " cleanup=each_update projection=fixture_only\n";

    std::vector<ModeSummary> summaries;
    for (const int mode_number : {1, 2})
    {
        validate_fourier_roundtrip(*level, mode_number);
        ReducedMatrix matrix =
            build_reduced_matrix(*level, mode_number, field_scales);
        summaries.push_back(analyze_spectrum(*level, matrix, mode_number,
                                             field_scales, x));
    }

    bool physical = false;
    bool dominated = false;
    bool nonnormal = false;
    for (const ModeSummary &summary : summaries)
    {
        physical = physical || summary.preliminary_physical;
        dominated =
            dominated ||
            summary.spectral_radius_is_boundary_constraint_or_gauge;
        nonnormal = nonnormal || summary.severe_nonnormality ||
                    summary.clustered_leading_spectrum ||
                    summary.complex_leading;
    }
    std::cout << "D14_D13_EXPLANATION"
              << " nearby_eigenvalues="
              << ((summaries[0].clustered_leading_spectrum ||
                   summaries[1].clustered_leading_spectrum)
                      ? 1
                      : 0)
              << " severe_eigenvector_nonorthogonality="
              << ((summaries[0].severe_nonnormality ||
                   summaries[1].severe_nonnormality)
                      ? 1
                      : 0)
              << " spectrum_wide_large_conditioning_proxy="
              << ((summaries[0].maximum_conditioning_proxy >= 1.0e3 ||
                   summaries[1].maximum_conditioning_proxy >= 1.0e3)
                      ? 1
                      : 0)
              << " complex_or_oscillatory_leaders="
              << ((summaries[0].complex_leading ||
                   summaries[1].complex_leading)
                      ? 1
                      : 0)
              << " boundary_constraint_spectral_radius="
              << (dominated ? 1 : 0)
              << " D13_local_alignment="
              << "not_explained_by_conditioning_proxy_alone\n";
    const char *classification = nullptr;
    if (physical)
    {
        classification =
            "CREDIBLE_REDUCED_GL_MODE_CANDIDATE_IDENTIFIED";
    }
    else if (dominated)
    {
        classification =
            "CONSTRAINT_GAUGE_OR_BOUNDARY_SPECTRUM_DOMINATES";
    }
    else if (nonnormal)
    {
        classification =
            "NONNORMAL_SPECTRUM_EXPLAINS_D13_WITHOUT_PHYSICAL_CANDIDATE";
    }
    else
    {
        classification = "REDUCED_SPECTRAL_EXTRACTION_NOT_READY";
    }

    const auto stop = std::chrono::steady_clock::now();
    const double wall_seconds =
        std::chrono::duration<double>(stop - start).count();
    const std::size_t actions = level->tangent_actions();
    std::cout << std::scientific << std::setprecision(12)
              << "D14_EFFICIENCY dense_matrices=2"
              << " matrix_columns=" << 2 * reduced_size
              << " tangent_actions=" << actions
              << " signed_live_steps=" << level->live_steps()
              << " wall_seconds=" << wall_seconds
              << " peak_rss_kib=" << peak_rss_kib()
              << " seconds_per_tangent_action="
              << wall_seconds / static_cast<double>(actions)
              << " matrix_memory_each_bytes="
              << static_cast<std::size_t>(reduced_size) * reduced_size *
                     sizeof(double)
              << '\n'
              << "D14_HOT_PATH direct_target_d4_evaluations_per_cell_stage=1"
              << " fused_ko_additions_per_cell_stage=1"
              << " second_rhs_evaluations=0"
              << " production_logging_added=0"
              << " production_allocations_added=0"
              << " production_counters_added=0"
              << " production_transforms_added=0\n"
              << "D14_FINE_VALIDATION performed=0"
              << " reason=no_medium_candidate_has_yet_passed_filter\n"
              << "D14_CLASSIFICATION " << classification << '\n'
              << "D14_AUDIT_REQUEST"
              << " focus=reduced_matrix,spectrum,physical_filter\n"
              << "BLACKSTRING_D14_REDUCED_SPECTRUM_PASS\n";
    return 0;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        d14_fail("usage: BlackStringD14ReducedSpectrumTest <params>");
    }
    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);
    return run_d14(parameters);
}
