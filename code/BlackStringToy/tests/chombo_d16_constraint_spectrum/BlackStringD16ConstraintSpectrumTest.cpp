#include "../../ConformalCartoonAlgebra.hpp"

#define BLACKSTRING_D16_CONSTRAINT_PROBE
#define BLACKSTRING_D15_REUSE_WITHOUT_ENTRY
#include "../chombo_d15_sector_isolation/BlackStringD15SectorIsolationTest.cpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <limits>
#include <set>
#include <string>
#include <tuple>
#include <vector>

extern "C"
{
    void dgesvd_(char *, char *, int *, int *, double *, int *, double *,
                 double *, int *, double *, int *, double *, int *, int *);
}

namespace
{
constexpr int d16_state_size = radial_cells * component_count;
constexpr int d16_all_rows = radial_cells * d16_constraint_count;
constexpr int d16_interior_radial_cells = radial_cells - 6;
constexpr int d16_interior_rows =
    d16_interior_radial_cells * d16_constraint_count;
constexpr double d16_epsilon_primary = 1.0e-8;
constexpr double d16_epsilon_secondary = 5.0e-9;
constexpr double d16_svd_noise_multiplier = 10.0;
constexpr double d16_invariance_limit = 1.0e-3;
constexpr double d16_full_residual_limit = 1.0e-6;
constexpr double d16_constraint_limit = 0.0500752;
constexpr double d16_boundary_limit = 0.30;
constexpr double d16_nyquist_limit = 0.20;
constexpr double d16_overlap_limit = 0.90;
constexpr double d16_rate_tolerance = 0.20;
constexpr int d16_maximum_separation = 2;
constexpr const char *d16_baseline_hash = "0x73c003dd70b673c6";

[[noreturn]] void d16_fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_D16_FAIL " << message << '\n';
    std::exit(1);
}

void d16_require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        d16_fail(message);
    }
}

Complex inner_product_local(const ComplexVector &left,
                            const ComplexVector &right)
{
    d16_require(left.size() == right.size(),
                "inner-product vectors have different sizes");
    Complex result = 0.0;
    for (std::size_t index = 0; index < left.size(); ++index)
    {
        result += std::conj(left[index]) * right[index];
    }
    return result;
}

std::vector<double> real_matrix_vector_local(
    const std::vector<double> &matrix, const int rows, const int columns,
    const std::vector<double> &input)
{
    d16_require(matrix.size() ==
                    static_cast<std::size_t>(rows) * columns &&
                    static_cast<int>(input.size()) == columns,
                "real matrix-vector dimensions disagree");
    std::vector<double> output(static_cast<std::size_t>(rows));
    for (int column = 0; column < columns; ++column)
    {
        for (int row = 0; row < rows; ++row)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(rows) * column] *
                input[static_cast<std::size_t>(column)];
        }
    }
    return output;
}

ComplexVector complex_matrix_vector_local(
    const std::vector<double> &matrix, const int rows, const int columns,
    const ComplexVector &input)
{
    d16_require(matrix.size() ==
                    static_cast<std::size_t>(rows) * columns &&
                    static_cast<int>(input.size()) == columns,
                "complex matrix-vector dimensions disagree");
    ComplexVector output(static_cast<std::size_t>(rows));
    for (int column = 0; column < columns; ++column)
    {
        for (int row = 0; row < rows; ++row)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(rows) * column] *
                input[static_cast<std::size_t>(column)];
        }
    }
    return output;
}

struct ConstraintMatrix
{
    int rows = 0;
    int columns = d16_state_size;
    std::vector<double> values;
    double epsilon_discrepancy = 0.0;
    std::uint64_t hash = 0;
};

std::vector<double>
flatten_constraints(const D16RadialConstraints &constraints,
                    const int first_radial, const int last_radial)
{
    std::vector<double> result(
        static_cast<std::size_t>(last_radial - first_radial) *
        d16_constraint_count);
    for (int radial = first_radial; radial < last_radial; ++radial)
    {
        for (int component = 0; component < d16_constraint_count;
             ++component)
        {
            result[static_cast<std::size_t>(
                (radial - first_radial) * d16_constraint_count +
                component)] =
                constraints[static_cast<std::size_t>(radial)]
                           [static_cast<std::size_t>(component)];
        }
    }
    return result;
}

void store_constraint_column(ConstraintMatrix &matrix, const int column,
                             const std::vector<double> &values)
{
    d16_require(static_cast<int>(values.size()) == matrix.rows,
                "constraint column has the wrong row count");
    for (int row = 0; row < matrix.rows; ++row)
    {
        matrix.values[static_cast<std::size_t>(row) +
                      static_cast<std::size_t>(matrix.rows) * column] =
            values[static_cast<std::size_t>(row)];
    }
}

struct ConstraintPair
{
    ConstraintMatrix all;
    ConstraintMatrix interior;
};

ConstraintPair build_constraint_matrices(D13TangentLevel &level,
                                         const D15Layout &layout,
                                         const StateRow &field_scales)
{
    ConstraintPair result;
    result.all.rows = d16_all_rows;
    result.interior.rows = d16_interior_rows;
    result.all.values.resize(
        static_cast<std::size_t>(result.all.rows) * d16_state_size);
    result.interior.values.resize(
        static_cast<std::size_t>(result.interior.rows) * d16_state_size);
    const auto representatives = representative_columns(layout);
    const std::set<int> representative_set(representatives.begin(),
                                           representatives.end());

    for (int column = 0; column < d16_state_size; ++column)
    {
        std::vector<double> basis(static_cast<std::size_t>(d16_state_size));
        basis[static_cast<std::size_t>(column)] = 1.0;
        const RadialVector physical =
            layout.physical_input(basis, field_scales);
        const D16RadialConstraints primary =
            level.evaluate_constraint_tangent(
                physical, d15_mode_number, d16_epsilon_primary);
        const std::vector<double> primary_all =
            flatten_constraints(primary, 0, radial_cells);
        const std::vector<double> primary_interior =
            flatten_constraints(primary, 3, radial_cells - 3);
        store_constraint_column(result.all, column, primary_all);
        store_constraint_column(result.interior, column, primary_interior);

        if (representative_set.count(column) != 0)
        {
            const D16RadialConstraints secondary =
                level.evaluate_constraint_tangent(
                    physical, d15_mode_number, d16_epsilon_secondary);
            const std::vector<double> secondary_all =
                flatten_constraints(secondary, 0, radial_cells);
            const std::vector<double> secondary_interior =
                flatten_constraints(secondary, 3, radial_cells - 3);
            result.all.epsilon_discrepancy =
                std::max(result.all.epsilon_discrepancy,
                         relative_vector_difference(primary_all,
                                                    secondary_all));
            result.interior.epsilon_discrepancy =
                std::max(result.interior.epsilon_discrepancy,
                         relative_vector_difference(primary_interior,
                                                    secondary_interior));
            std::cout << std::scientific << std::setprecision(12)
                      << "D16_CONSTRAINT_EPSILON column=" << column
                      << " all_relative="
                      << relative_vector_difference(primary_all,
                                                    secondary_all)
                      << " interior_relative="
                      << relative_vector_difference(primary_interior,
                                                    secondary_interior)
                      << '\n';
        }
        if ((column + 1) % 64 == 0 || column + 1 == d16_state_size)
        {
            std::cout << "D16_CONSTRAINT_PROGRESS columns=" << column + 1
                      << '/' << d16_state_size << '\n';
        }
    }
    result.all.hash = hash_values(result.all.values);
    result.interior.hash = hash_values(result.interior.values);
    return result;
}

struct Nullspace
{
    int rows = 0;
    int state_size = d16_state_size;
    int rank = 0;
    int dimension = 0;
    double rank_tolerance = 0.0;
    double cn_residual = 0.0;
    std::vector<double> singular_values;
    std::vector<double> basis;
    std::uint64_t hash = 0;
};

Nullspace constraint_nullspace(const ConstraintMatrix &matrix)
{
    Nullspace result;
    result.rows = matrix.rows;
    const int minimum = std::min(matrix.rows, matrix.columns);
    result.singular_values.resize(static_cast<std::size_t>(minimum));
    std::vector<double> copy = matrix.values;
    std::vector<double> vt(static_cast<std::size_t>(matrix.columns) *
                           matrix.columns);
    double dummy_u = 0.0;
    int m = matrix.rows;
    int n = matrix.columns;
    int lda = m;
    int ldu = 1;
    int ldvt = n;
    int info = 0;
    int lwork = -1;
    double work_query = 0.0;
    char jobu = 'N';
    char jobvt = 'A';
    dgesvd_(&jobu, &jobvt, &m, &n, copy.data(), &lda,
            result.singular_values.data(), &dummy_u, &ldu, vt.data(),
            &ldvt, &work_query, &lwork, &info);
    d16_require(info == 0, "D16 LAPACK dgesvd workspace query failed");
    lwork = std::max(5 * std::max(m, n),
                     static_cast<int>(std::ceil(work_query)));
    std::vector<double> work(static_cast<std::size_t>(lwork));
    copy = matrix.values;
    dgesvd_(&jobu, &jobvt, &m, &n, copy.data(), &lda,
            result.singular_values.data(), &dummy_u, &ldu, vt.data(),
            &ldvt, work.data(), &lwork, &info);
    d16_require(info == 0, "D16 LAPACK dgesvd failed");

    const double largest = result.singular_values.empty()
                               ? 0.0
                               : result.singular_values.front();
    const double roundoff =
        100.0 * std::max(m, n) * std::numeric_limits<double>::epsilon() *
        largest;
    const double finite_difference =
        d16_svd_noise_multiplier * matrix.epsilon_discrepancy * largest;
    result.rank_tolerance = std::max(roundoff, finite_difference);
    result.rank = static_cast<int>(std::count_if(
        result.singular_values.begin(), result.singular_values.end(),
        [&](const double value) { return value > result.rank_tolerance; }));
    result.dimension = n - result.rank;
    d16_require(result.dimension > 0,
                "D16 constraint matrix has no numerical nullspace");
    result.basis.resize(static_cast<std::size_t>(n) * result.dimension);
    for (int column = 0; column < result.dimension; ++column)
    {
        const int singular_column = result.rank + column;
        for (int row = 0; row < n; ++row)
        {
            result.basis[static_cast<std::size_t>(row) +
                         static_cast<std::size_t>(n) * column] =
                vt[static_cast<std::size_t>(singular_column) +
                   static_cast<std::size_t>(n) * row];
        }
    }
    double residual_sum = 0.0;
    for (int column = 0; column < result.dimension; ++column)
    {
        std::vector<double> vector(static_cast<std::size_t>(n));
        for (int row = 0; row < n; ++row)
        {
            vector[static_cast<std::size_t>(row)] =
                result.basis[static_cast<std::size_t>(row) +
                             static_cast<std::size_t>(n) * column];
        }
        const std::vector<double> residual = real_matrix_vector_local(
            matrix.values, matrix.rows, n, vector);
        for (const double value : residual)
        {
            residual_sum += value * value;
        }
    }
    result.cn_residual =
        std::sqrt(residual_sum) /
        std::sqrt(static_cast<double>(result.dimension));
    result.hash = hash_values(result.basis);
    return result;
}

void emit_svd(const char *name, const ConstraintMatrix &matrix,
              const Nullspace &nullspace)
{
    std::cout << std::scientific << std::setprecision(12)
              << "D16_CONSTRAINT_SVD projection=" << name
              << " rows=" << matrix.rows
              << " columns=" << matrix.columns
              << " matrix_hash=" << hash_string(matrix.hash)
              << " epsilon_discrepancy=" << matrix.epsilon_discrepancy
              << " rank_tolerance=" << nullspace.rank_tolerance
              << " rank=" << nullspace.rank
              << " nullspace_dimension=" << nullspace.dimension
              << " nullspace_hash=" << hash_string(nullspace.hash)
              << " CN_residual=" << nullspace.cn_residual
              << " singular_values=";
    for (std::size_t index = 0;
         index < nullspace.singular_values.size(); ++index)
    {
        if (index != 0)
        {
            std::cout << ',';
        }
        std::cout << nullspace.singular_values[index];
    }
    std::cout << '\n';
}

struct ProjectedMap
{
    int dimension = 0;
    std::vector<double> values;
    double invariance_leakage = 0.0;
    std::uint64_t hash = 0;
};

ProjectedMap project_map(const D15Matrix &matrix,
                         const Nullspace &nullspace)
{
    ProjectedMap result;
    result.dimension = nullspace.dimension;
    const int n = matrix.n;
    const int q = nullspace.dimension;
    std::vector<double> mn(static_cast<std::size_t>(n) * q);
    for (int column = 0; column < q; ++column)
    {
        for (int source = 0; source < n; ++source)
        {
            const double coefficient =
                nullspace.basis[static_cast<std::size_t>(source) +
                                static_cast<std::size_t>(n) * column];
            for (int row = 0; row < n; ++row)
            {
                mn[static_cast<std::size_t>(row) +
                   static_cast<std::size_t>(n) * column] +=
                    matrix.map[static_cast<std::size_t>(row) +
                               static_cast<std::size_t>(n) * source] *
                    coefficient;
            }
        }
    }
    result.values.resize(static_cast<std::size_t>(q) * q);
    for (int column = 0; column < q; ++column)
    {
        for (int reduced_row = 0; reduced_row < q; ++reduced_row)
        {
            for (int row = 0; row < n; ++row)
            {
                result.values[static_cast<std::size_t>(reduced_row) +
                              static_cast<std::size_t>(q) * column] +=
                    nullspace.basis[static_cast<std::size_t>(row) +
                                    static_cast<std::size_t>(n) *
                                        reduced_row] *
                    mn[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(n) * column];
            }
        }
    }
    double total = 0.0;
    double leakage = 0.0;
    for (int column = 0; column < q; ++column)
    {
        for (int row = 0; row < n; ++row)
        {
            double reconstructed = 0.0;
            for (int reduced_row = 0; reduced_row < q; ++reduced_row)
            {
                reconstructed +=
                    nullspace.basis[static_cast<std::size_t>(row) +
                                    static_cast<std::size_t>(n) *
                                        reduced_row] *
                    result.values[static_cast<std::size_t>(reduced_row) +
                                  static_cast<std::size_t>(q) * column];
            }
            const double value =
                mn[static_cast<std::size_t>(row) +
                   static_cast<std::size_t>(n) * column];
            total += value * value;
            leakage +=
                (value - reconstructed) * (value - reconstructed);
        }
    }
    result.invariance_leakage =
        std::sqrt(leakage) /
        std::max(std::sqrt(total), std::numeric_limits<double>::min());
    result.hash = hash_values(result.values);
    return result;
}

ComplexVector lift_vector(const Nullspace &nullspace,
                          const ComplexVector &reduced)
{
    ComplexVector result(static_cast<std::size_t>(nullspace.state_size));
    for (int column = 0; column < nullspace.dimension; ++column)
    {
        for (int row = 0; row < nullspace.state_size; ++row)
        {
            result[static_cast<std::size_t>(row)] +=
                nullspace.basis[static_cast<std::size_t>(row) +
                                static_cast<std::size_t>(
                                    nullspace.state_size) *
                                    column] *
                reduced[static_cast<std::size_t>(column)];
        }
    }
    return result;
}

struct ProjectedCandidate
{
    std::string projection;
    int index = 0;
    Complex lambda = 0.0;
    Complex omega = 0.0;
    bool omega_defined = false;
    ComplexVector scaled;
    ComplexVector physical;
    StateRow component_fractions{};
    double projected_residual = 0.0;
    double full_dense_residual = 0.0;
    double full_matrix_free_residual =
        std::numeric_limits<double>::infinity();
    double constraint_residual = 0.0;
    double hm_constraint = 0.0;
    double determinant_constraint = 0.0;
    double trace_constraint = 0.0;
    double boundary_fraction = 0.0;
    double nyquist_fraction = 0.0;
    double conditioning_proxy = 0.0;
    double parity_leakage = std::numeric_limits<double>::infinity();
    int maximum_radial = 0;
    int physical_fields = 0;
    std::uint64_t profile_hash = 0;
};

ProjectedCandidate make_projected_candidate(
    const char *projection, const D15Matrix &full_matrix,
    const ConstraintMatrix &constraints, const Nullspace &nullspace,
    const ProjectedMap &projected, const DynamicEigensystem &eigensystem,
    const int index, const StateRow &field_scales, const double dt,
    const double eigenvalue_floor)
{
    ProjectedCandidate result;
    result.projection = projection;
    result.index = index;
    result.lambda = {
        eigensystem.real[static_cast<std::size_t>(index)],
        eigensystem.imaginary[static_cast<std::size_t>(index)]};
    const ComplexVector right_reduced =
        dynamic_eigenvector(eigensystem.right, eigensystem.imaginary,
                            projected.dimension, index);
    const ComplexVector left_reduced =
        dynamic_eigenvector(eigensystem.left, eigensystem.imaginary,
                            projected.dimension, index);
    result.scaled = lift_vector(nullspace, right_reduced);
    const double scaled_norm = complex_norm(result.scaled);
    for (Complex &value : result.scaled)
    {
        value /= scaled_norm;
    }
    result.physical.resize(static_cast<std::size_t>(d16_state_size));
    ComplexVector physical_left(static_cast<std::size_t>(d16_state_size));
    const ComplexVector lifted_left =
        lift_vector(nullspace, left_reduced);
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < component_count; ++component)
        {
            const std::size_t coordinate = reduced_index(radial, component);
            const double scale =
                field_scales[static_cast<std::size_t>(component)];
            result.physical[coordinate] = result.scaled[coordinate] * scale;
            physical_left[coordinate] = lifted_left[coordinate] / scale;
        }
    }
    const Complex denominator =
        inner_product_local(left_reduced, right_reduced);
    result.conditioning_proxy =
        complex_norm(physical_left) * complex_norm(result.physical) /
        std::max(std::abs(denominator),
                 std::numeric_limits<double>::min());

    if (std::abs(result.lambda) > eigenvalue_floor)
    {
        result.omega = std::log(result.lambda) / dt;
        result.omega_defined = true;
    }
    const ComplexVector reduced_action = complex_matrix_vector_local(
        projected.values, projected.dimension, projected.dimension,
        right_reduced);
    ComplexVector projected_error = reduced_action;
    for (int row = 0; row < projected.dimension; ++row)
    {
        projected_error[static_cast<std::size_t>(row)] -=
            result.lambda * right_reduced[static_cast<std::size_t>(row)];
    }
    result.projected_residual =
        complex_norm(projected_error) / complex_norm(right_reduced);

    ComplexVector full_error = complex_matrix_vector_local(
        full_matrix.map, full_matrix.n, full_matrix.n, result.scaled);
    for (int row = 0; row < full_matrix.n; ++row)
    {
        full_error[static_cast<std::size_t>(row)] -=
            result.lambda * result.scaled[static_cast<std::size_t>(row)];
    }
    result.full_dense_residual = complex_norm(full_error);

    const ComplexVector constraint = complex_matrix_vector_local(
        constraints.values, constraints.rows, constraints.columns,
        result.scaled);
    result.constraint_residual = complex_norm(constraint);
    double h = 0.0;
    double mx = 0.0;
    double mz = 0.0;
    double determinant = 0.0;
    double trace = 0.0;
    const int first_radial =
        constraints.rows == d16_all_rows ? 0 : 3;
    const int radial_count =
        constraints.rows / d16_constraint_count;
    for (int local = 0; local < radial_count; ++local)
    {
        const int base = local * d16_constraint_count;
        h += std::norm(constraint[static_cast<std::size_t>(base)]);
        mx += std::norm(constraint[static_cast<std::size_t>(base + 1)]);
        mz += std::norm(constraint[static_cast<std::size_t>(base + 2)]);
        determinant +=
            std::norm(constraint[static_cast<std::size_t>(base + 3)]);
        trace +=
            std::norm(constraint[static_cast<std::size_t>(base + 4)]);
        (void)first_radial;
    }
    result.hm_constraint = std::sqrt(h + mx + mz);
    result.determinant_constraint = std::sqrt(determinant);
    result.trace_constraint = std::sqrt(trace);

    double total_energy = 0.0;
    double boundary_energy = 0.0;
    double maximum_energy = -1.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        double radial_energy = 0.0;
        for (int component = 0; component < component_count; ++component)
        {
            const std::size_t coordinate = reduced_index(radial, component);
            const double energy = std::norm(result.physical[coordinate]);
            result.component_fractions[
                static_cast<std::size_t>(component)] += energy;
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
            result.maximum_radial = radial;
        }
    }
    for (int component = 0; component < component_count; ++component)
    {
        double &fraction = result.component_fractions[
            static_cast<std::size_t>(component)];
        fraction /= total_energy;
        if (component < d15_physical_components && fraction > 1.0e-4)
        {
            ++result.physical_fields;
        }
    }
    result.boundary_fraction = boundary_energy / total_energy;
    double nyquist = 0.0;
    for (int component = 0; component < component_count; ++component)
    {
        Complex alternating = 0.0;
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            alternating +=
                (radial % 2 == 0 ? 1.0 : -1.0) *
                result.physical[reduced_index(radial, component)];
        }
        nyquist += std::norm(alternating) / radial_cells;
    }
    result.nyquist_fraction = nyquist / total_energy;
    result.profile_hash = hash_values(result.physical);
    return result;
}

void validate_matrix_free(ProjectedCandidate &candidate,
                          D13TangentLevel &level,
                          const D15Layout &layout,
                          const StateRow &field_scales)
{
    ComplexVector direct(static_cast<std::size_t>(d16_state_size));
    double parity = 0.0;
    for (int part = 0; part < 2; ++part)
    {
        std::vector<double> scaled(static_cast<std::size_t>(d16_state_size));
        for (int index = 0; index < d16_state_size; ++index)
        {
            scaled[static_cast<std::size_t>(index)] =
                part == 0
                    ? candidate.scaled[static_cast<std::size_t>(index)].real()
                    : candidate.scaled[static_cast<std::size_t>(index)].imag();
        }
        if (vector_norm(scaled) == 0.0)
        {
            continue;
        }
        const ActionResult action = level.apply_tangent(
            layout.physical_input(scaled, field_scales), d15_mode_number,
            d16_epsilon_primary);
        const std::vector<double> output =
            layout.scaled_output(action.response, field_scales);
        for (int index = 0; index < d16_state_size; ++index)
        {
            direct[static_cast<std::size_t>(index)] +=
                part == 0
                    ? Complex(output[static_cast<std::size_t>(index)], 0.0)
                    : Complex(0.0, output[static_cast<std::size_t>(index)]);
        }
        parity = std::max(parity, action.parity_leakage);
    }
    for (int index = 0; index < d16_state_size; ++index)
    {
        direct[static_cast<std::size_t>(index)] -=
            candidate.lambda *
            candidate.scaled[static_cast<std::size_t>(index)];
    }
    candidate.full_matrix_free_residual = complex_norm(direct);
    candidate.parity_leakage = parity;
}

void emit_candidate(const char *role,
                    const ProjectedCandidate &candidate)
{
    std::cout << std::scientific << std::setprecision(12)
              << "D16_CANDIDATE projection=" << candidate.projection
              << " role=" << role
              << " eigen_index=" << candidate.index
              << " lambda_real=" << candidate.lambda.real()
              << " lambda_imag=" << candidate.lambda.imag()
              << " Omega_real=" << candidate.omega.real()
              << " Omega_imag=" << candidate.omega.imag()
              << " projected_residual=" << candidate.projected_residual
              << " full_dense_residual=" << candidate.full_dense_residual
              << " full_matrix_free_residual="
              << candidate.full_matrix_free_residual
              << " constraint_residual="
              << candidate.constraint_residual
              << " HM_constraint=" << candidate.hm_constraint
              << " determinant_constraint="
              << candidate.determinant_constraint
              << " trace_constraint=" << candidate.trace_constraint
              << " boundary_fraction=" << candidate.boundary_fraction
              << " maximum_radial=" << candidate.maximum_radial
              << " nyquist_fraction=" << candidate.nyquist_fraction
              << " physical_fields=" << candidate.physical_fields
              << " conditioning_proxy="
              << candidate.conditioning_proxy
              << " parity_leakage=" << candidate.parity_leakage
              << " profile_hash=" << hash_string(candidate.profile_hash)
              << " component_fractions=";
    for (int component = 0; component < component_count; ++component)
    {
        if (component != 0)
        {
            std::cout << ',';
        }
        std::cout << UserVariables::variable_names[
                         static_cast<std::size_t>(component)]
                  << ':'
                  << candidate.component_fractions[
                         static_cast<std::size_t>(component)];
    }
    std::cout << '\n';
}

double profile_overlap(const ProjectedCandidate &left,
                       const ProjectedCandidate &right)
{
    return std::abs(inner_product_local(left.physical, right.physical)) /
           std::max(complex_norm(left.physical) *
                        complex_norm(right.physical),
                    std::numeric_limits<double>::min());
}

bool rate_agreement(const ProjectedCandidate &left,
                    const ProjectedCandidate &right)
{
    const double scale =
        std::max({std::abs(left.omega.real()),
                  std::abs(right.omega.real()), 1.0e-2});
    return std::abs(left.omega.real() - right.omega.real()) <=
           d16_rate_tolerance * scale;
}

bool individual_bulk_gate(const ProjectedCandidate &candidate)
{
    return candidate.omega_defined &&
           candidate.full_dense_residual < d16_full_residual_limit &&
           candidate.hm_constraint < d16_constraint_limit &&
           candidate.boundary_fraction < d16_boundary_limit &&
           candidate.maximum_radial >= 3 &&
           candidate.maximum_radial < radial_cells - 3 &&
           candidate.nyquist_fraction < d16_nyquist_limit &&
           candidate.physical_fields >= 3;
}

struct SpectrumResult
{
    std::string name;
    double invariance = 0.0;
    std::vector<ProjectedCandidate> candidates;
    int leader = -1;
    int lowest_constraint = -1;
};

SpectrumResult analyze_projection(
    const char *name, D13TangentLevel &level, const D15Layout &layout,
    const D15Matrix &full_matrix, const ConstraintMatrix &constraints,
    const Nullspace &nullspace, const ProjectedMap &projected,
    const StateRow &field_scales, const double eigenvalue_floor)
{
    D15Matrix reduced_matrix;
    reduced_matrix.n = projected.dimension;
    reduced_matrix.map = projected.values;
    const DynamicEigensystem eigensystem =
        diagonalize_dynamic(reduced_matrix);
    SpectrumResult result;
    result.name = name;
    result.invariance = projected.invariance_leakage;
    result.candidates.reserve(
        static_cast<std::size_t>(projected.dimension));
    for (int index = 0; index < projected.dimension; ++index)
    {
        result.candidates.push_back(make_projected_candidate(
            name, full_matrix, constraints, nullspace, projected,
            eigensystem, index, field_scales, level.timestep(),
            eigenvalue_floor));
    }
    for (int index = 0; index < projected.dimension; ++index)
    {
        const ProjectedCandidate &candidate =
            result.candidates[static_cast<std::size_t>(index)];
        if (!candidate.omega_defined)
        {
            continue;
        }
        if (result.leader < 0 ||
            candidate.omega.real() >
                result.candidates[static_cast<std::size_t>(
                    result.leader)]
                    .omega.real())
        {
            result.leader = index;
        }
        if (result.lowest_constraint < 0 ||
            candidate.hm_constraint <
                result.candidates[static_cast<std::size_t>(
                    result.lowest_constraint)]
                    .hm_constraint)
        {
            result.lowest_constraint = index;
        }
    }
    d16_require(result.leader >= 0 && result.lowest_constraint >= 0,
                result.name + " has no resolved projected candidates");
    std::set<int> direct_indices = {result.leader,
                                    result.lowest_constraint};
    for (const int index : direct_indices)
    {
        validate_matrix_free(
            result.candidates[static_cast<std::size_t>(index)], level,
            layout, field_scales);
    }
    emit_candidate("largest_ReOmega",
                   result.candidates[static_cast<std::size_t>(
                       result.leader)]);
    emit_candidate("smallest_constraint",
                   result.candidates[static_cast<std::size_t>(
                       result.lowest_constraint)]);
    std::cout << std::scientific << std::setprecision(12)
              << "D16_PROJECTED_MAP projection=" << name
              << " dimension=" << projected.dimension
              << " map_hash=" << hash_string(projected.hash)
              << " invariance_eta=" << projected.invariance_leakage
              << " invariance_limit=" << d16_invariance_limit << '\n';
    return result;
}

bool find_bulk_match(SpectrumResult &all, SpectrumResult &interior,
                     D13TangentLevel &level, const D15Layout &layout,
                     const StateRow &field_scales)
{
    bool matched = false;
    for (ProjectedCandidate &left : all.candidates)
    {
        if (!individual_bulk_gate(left))
        {
            continue;
        }
        for (ProjectedCandidate &right : interior.candidates)
        {
            if (!individual_bulk_gate(right))
            {
                continue;
            }
            const double overlap = profile_overlap(left, right);
            const bool rate = rate_agreement(left, right);
            const int separation =
                std::abs(left.maximum_radial - right.maximum_radial);
            if (overlap <= d16_overlap_limit || !rate ||
                separation > d16_maximum_separation)
            {
                continue;
            }
            if (!std::isfinite(left.full_matrix_free_residual))
            {
                validate_matrix_free(left, level, layout, field_scales);
            }
            if (!std::isfinite(right.full_matrix_free_residual))
            {
                validate_matrix_free(right, level, layout, field_scales);
            }
            const bool passes =
                left.full_matrix_free_residual <
                    d16_full_residual_limit &&
                right.full_matrix_free_residual <
                    d16_full_residual_limit;
            std::cout << std::scientific << std::setprecision(12)
                      << "D16_MATCH all_index=" << left.index
                      << " interior_index=" << right.index
                      << " profile_overlap=" << overlap
                      << " rate_agreement=" << (rate ? 1 : 0)
                      << " radial_separation=" << separation
                      << " all_matrix_free_residual="
                      << left.full_matrix_free_residual
                      << " interior_matrix_free_residual="
                      << right.full_matrix_free_residual
                      << " passes=" << (passes ? 1 : 0) << '\n';
            matched = matched || passes;
        }
    }
    return matched;
}

int run_d16(SimulationParameters &parameters)
{
    d16_require(parameters.max_level == 0, "D16 requires max_level=0");
    d16_require(parameters.coarsest_dx == 0.125,
                "D16 requires dx=dz=1/8");
    d16_require(parameters.dt_multiplier == 0.05,
                "D16 requires CFL=0.05");
    d16_require(parameters.ko_sigma == 0.3,
                "D16 requires ko_sigma=0.3");
    d16_require(parameters.constraint_diagnostic_cadence == 0,
                "D16 production diagnostics must remain disabled");
    static_assert(d16_state_size == 576);
    static_assert(d16_all_rows == 160);
    static_assert(d16_interior_rows == 130);

    const auto start = std::chrono::steady_clock::now();
    GRAMR amr;
    DefaultLevelFactory<D13TangentLevel> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    d16_require(levels.size() == 1, "D16 must create one level");
    auto *level = dynamic_cast<D13TangentLevel *>(levels[0]);
    d16_require(level != nullptr, "D16 level factory type mismatch");
    level->prepare();
    const StateRow field_scales = level->field_scales();
    const D15Layout layout = full_layout();

    std::cout << "D16_CONFIGURATION k=pi/4 Lz=8 x_in=0.5 x_out=4.5"
              << " Nx=32 Nz=64 dx=0.125 dz=0.125 CFL=0.05"
              << " ko_sigma=0.3 tangent_matrices=1"
              << " constraint_components=H,Mx,Mz,determinant,weighted_trace"
              << " encoded_Z_Gamma_consistency=OMITTED"
              << " omission_reason=no_validated_live_helper\n";

    D15Matrix matrix = build_matrix(*level, layout, field_scales);
    const std::string matrix_hash = hash_string(hash_values(matrix.map));
    d16_require(matrix.n == d16_state_size,
                "D16 tangent matrix is not 576x576");
    d16_require(matrix_hash == d16_baseline_hash,
                "D16 tangent matrix does not reproduce D14/D15 baseline");
    const ConstraintPair constraints =
        build_constraint_matrices(*level, layout, field_scales);
    const Nullspace all_null =
        constraint_nullspace(constraints.all);
    const Nullspace interior_null =
        constraint_nullspace(constraints.interior);
    emit_svd("all_cells", constraints.all, all_null);
    emit_svd("interior_only", constraints.interior, interior_null);

    const ProjectedMap all_projected =
        project_map(matrix, all_null);
    const ProjectedMap interior_projected =
        project_map(matrix, interior_null);
    const double eigenvalue_floor =
        d15_floor_multiplier *
        std::max(matrix.epsilon_column_difference,
                 matrix.direct_vector_difference);
    SpectrumResult all_spectrum = analyze_projection(
        "all_cells", *level, layout, matrix, constraints.all, all_null,
        all_projected, field_scales, eigenvalue_floor);
    SpectrumResult interior_spectrum = analyze_projection(
        "interior_only", *level, layout, matrix, constraints.interior,
        interior_null, interior_projected, field_scales,
        eigenvalue_floor);
    const bool bulk = find_bulk_match(
        all_spectrum, interior_spectrum, *level, layout, field_scales);

    const bool invariant =
        all_projected.invariance_leakage < d16_invariance_limit &&
        interior_projected.invariance_leakage < d16_invariance_limit;
    const auto &all_leader =
        all_spectrum.candidates[static_cast<std::size_t>(
            all_spectrum.leader)];
    const auto &interior_leader =
        interior_spectrum.candidates[static_cast<std::size_t>(
            interior_spectrum.leader)];
    const bool leader_residuals =
        all_leader.full_matrix_free_residual <
            d16_full_residual_limit &&
        interior_leader.full_matrix_free_residual <
            d16_full_residual_limit;
    const char *classification = nullptr;
    if (!invariant || !leader_residuals)
    {
        classification = "CONSTRAINT_NULLSPACE_NOT_INVARIANT";
    }
    else if (bulk)
    {
        classification =
            "CREDIBLE_CONSTRAINT_COMPATIBLE_BULK_CANDIDATE_EXPOSED";
    }
    else
    {
        classification = "PROJECTED_SPECTRUM_REMAINS_BOUNDARY_DOMINATED";
    }

    const double overlap =
        profile_overlap(all_leader, interior_leader);
    const double wall_seconds =
        std::chrono::duration<double>(
            std::chrono::steady_clock::now() - start)
            .count();
    std::cout << std::scientific << std::setprecision(12)
              << "D16_CROSS_PROJECTION leader_profile_overlap="
              << overlap
              << " leader_rate_agreement="
              << (rate_agreement(all_leader, interior_leader) ? 1 : 0)
              << " all_bulk_candidates="
              << std::count_if(all_spectrum.candidates.begin(),
                               all_spectrum.candidates.end(),
                               [](const ProjectedCandidate &candidate)
                               {
                                   return individual_bulk_gate(candidate);
                               })
              << " interior_bulk_candidates="
              << std::count_if(interior_spectrum.candidates.begin(),
                               interior_spectrum.candidates.end(),
                               [](const ProjectedCandidate &candidate)
                               {
                                   return individual_bulk_gate(candidate);
                               })
              << " qualifying_matches=" << (bulk ? 1 : 0) << '\n'
              << "D16_EFFICIENCY tangent_matrix_hash=" << matrix_hash
              << " tangent_matrix_basis_actions=576"
              << " tangent_epsilon_validation_actions=12"
              << " tangent_random_dense_direct_probe_actions=1"
              << " tangent_candidate_full_map_residual_actions=5"
              << " candidate_all_cell_real_leader_actions=1"
              << " candidate_all_cell_real_lowest_constraint_actions=1"
              << " candidate_interior_complex_leader_signed_component_actions=2"
              << " candidate_interior_real_lowest_constraint_actions=1"
              << " tangent_actions=" << level->tangent_actions()
              << " signed_live_steps=" << level->live_steps()
              << " constraint_basis_actions=576"
              << " constraint_epsilon_validation_actions=12"
              << " constraint_actions=" << level->constraint_actions()
              << " constraint_signed_evaluations="
              << 2 * level->constraint_actions()
              << " wall_seconds=" << wall_seconds
              << " peak_rss_kib=" << peak_rss_kib() << '\n'
              << "D16_HOT_PATH production_code_changed=0"
              << " direct_target_d4_evaluations_per_cell_stage=1"
              << " fused_ko_additions_per_cell_stage=1"
              << " second_rhs_evaluations=0"
              << " production_diagnostics_added=0\n"
              << "D16_CLASSIFICATION " << classification << '\n'
              << "BLACKSTRING_D16_CONSTRAINT_SPECTRUM_PASS\n";
    return 0;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        d16_fail("usage: BlackStringD16ConstraintSpectrumTest <params>");
    }
    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);
    return run_d16(parameters);
}
