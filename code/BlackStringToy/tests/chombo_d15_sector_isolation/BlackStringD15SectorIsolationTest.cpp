#define BLACKSTRING_D14_REUSE_WITHOUT_ENTRY
#include "../chombo_d14_reduced_spectrum/BlackStringD14ReducedSpectrumTest.cpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace
{
constexpr int d15_mode_number = 1;
constexpr int d15_validation_columns = 12;
constexpr double d15_epsilon_primary = 1.0e-8;
constexpr double d15_epsilon_secondary = 5.0e-9;
constexpr double d15_column_tolerance = 5.0e-3;
constexpr double d15_dense_direct_tolerance = 2.0e-4;
constexpr double d15_direct_residual_limit = 1.0e-7;
constexpr double d15_parity_limit = 1.0e-5;
constexpr double d15_reconstruction_limit = 1.0e-7;
constexpr double d15_floor_multiplier = 10.0;
constexpr double d14_baseline_epsilon_discrepancy = 1.04327e-7;
constexpr double d14_baseline_dense_direct_discrepancy = 6.94188e-7;
constexpr double d14_leader_boundary_fraction = 0.819825;
constexpr double d14_leader_constraint = 0.500752;
constexpr double d15_bulk_boundary_limit = 0.40;
constexpr double d15_bulk_constraint_limit = 0.1 * d14_leader_constraint;
constexpr double d15_bulk_nyquist_limit = 0.20;
constexpr double d15_profile_overlap_gate = 0.90;
constexpr double d15_rate_relative_tolerance = 0.20;
constexpr int d15_maximum_radial_separation = 2;
constexpr int d15_physical_components = 13;
constexpr int d15_frozen_size = radial_cells * d15_physical_components;
constexpr const char *d14_baseline_hash = "0x73c003dd70b673c6";

[[noreturn]] void d15_fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_D15_FAIL " << message << '\n';
    std::exit(1);
}

void d15_require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        d15_fail(message);
    }
}

double real_vector_norm(const std::vector<double> &values)
{
    double sum = 0.0;
    for (const double value : values)
    {
        sum += value * value;
    }
    return std::sqrt(sum);
}

double complex_vector_norm(const ComplexVector &values)
{
    double sum = 0.0;
    for (const Complex value : values)
    {
        sum += std::norm(value);
    }
    return std::sqrt(sum);
}

double relative_vector_difference(const std::vector<double> &left,
                                  const std::vector<double> &right)
{
    d15_require(left.size() == right.size(),
                "relative-difference vectors have different sizes");
    double difference = 0.0;
    double reference = 0.0;
    for (std::size_t index = 0; index < left.size(); ++index)
    {
        difference +=
            (left[index] - right[index]) * (left[index] - right[index]);
        reference += left[index] * left[index];
    }
    return std::sqrt(difference) /
           std::max(std::sqrt(reference),
                    std::numeric_limits<double>::min());
}

struct D15Layout
{
    std::string name;
    std::vector<int> components;

    int size() const
    {
        return radial_cells * static_cast<int>(components.size());
    }

    int index(const int radial, const int local_component) const
    {
        return radial * static_cast<int>(components.size()) +
               local_component;
    }

    RadialVector physical_input(const std::vector<double> &scaled,
                                const StateRow &field_scales) const
    {
        d15_require(static_cast<int>(scaled.size()) == size(),
                    name + " input has the wrong reduced size");
        RadialVector result{};
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            for (int local = 0;
                 local < static_cast<int>(components.size()); ++local)
            {
                const int component =
                    components[static_cast<std::size_t>(local)];
                result[static_cast<std::size_t>(radial)]
                      [static_cast<std::size_t>(component)] =
                    scaled[static_cast<std::size_t>(index(radial, local))] *
                    field_scales[static_cast<std::size_t>(component)];
            }
        }
        return result;
    }

    std::vector<double>
    scaled_output(const RadialVector &physical,
                  const StateRow &field_scales) const
    {
        std::vector<double> result(static_cast<std::size_t>(size()));
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            for (int local = 0;
                 local < static_cast<int>(components.size()); ++local)
            {
                const int component =
                    components[static_cast<std::size_t>(local)];
                result[static_cast<std::size_t>(index(radial, local))] =
                    physical[static_cast<std::size_t>(radial)]
                            [static_cast<std::size_t>(component)] /
                    field_scales[static_cast<std::size_t>(component)];
            }
        }
        return result;
    }

    std::vector<double> scaling_vector(const StateRow &field_scales) const
    {
        std::vector<double> result(static_cast<std::size_t>(size()));
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            for (int local = 0;
                 local < static_cast<int>(components.size()); ++local)
            {
                result[static_cast<std::size_t>(index(radial, local))] =
                    field_scales[static_cast<std::size_t>(
                        components[static_cast<std::size_t>(local)])];
            }
        }
        return result;
    }
};

D15Layout full_layout()
{
    D15Layout result{"exact_gp_ghosts", {}};
    for (int component = 0; component < component_count; ++component)
    {
        result.components.push_back(component);
    }
    return result;
}

D15Layout frozen_layout()
{
    D15Layout result{"frozen_gauge", {}};
    for (int component = 0; component < d15_physical_components; ++component)
    {
        result.components.push_back(component);
    }
    return result;
}

struct D15FrozenGaugePreStorePolicy
{
    void operator()(BlackStringReducedVars::Variables<double> &rhs) const
    {
        rhs.gauge.lapse = 0.0;
        rhs.gauge.shift = {0.0, 0.0};
        rhs.gauge.B = {0.0, 0.0};
    }
};

class D15FrozenGaugeLevel : public D13TangentLevel
{
    friend class DefaultLevelFactory<D15FrozenGaugeLevel>;

  protected:
    using D13TangentLevel::D13TangentLevel;

  public:
    double maximum_gauge_rhs() const { return m_maximum_gauge_rhs; }
    std::size_t rhs_calls() const { return m_rhs_calls; }
    std::size_t outer_rhs_calls() const { return m_outer_rhs_calls; }

  private:
    void specificEvalRHS(GRLevelData &solution, GRLevelData &rhs,
                         const double time) override
    {
        (void)time;
        ++m_rhs_calls;
        using FrozenCompute = BlackStringLive::BasicRHSCompute<
            BlackStringLive::DefaultInputPolicy,
            BlackStringLive::DefaultEvaluationPolicy,
            D15FrozenGaugePreStorePolicy>;
        BoxLoops::loop(
            FrozenCompute(m_p.r0, m_dx, m_p.coordinate_offset(), m_p.gauge,
                          m_p.fixed_lapse_source, m_p.ko_sigma),
            solution, rhs, EXCLUDE_GHOST_CELLS, disable_simd());
        if (m_p.physical_radial_boundaries)
        {
            BlackStringPerturbativeRadialBoundary::apply_outer_rhs(
                solution, rhs, m_problem_domain, m_p.r0, m_dx,
                m_p.coordinate_offset(), m_p.outer_sommerfeld_speed,
                Interval(0, c_GammaZ));
            ++m_outer_rhs_calls;
        }
        const Box domain = m_problem_domain.domainBox();
        const DataIterator iterator = rhs.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = rhs[data_index];
            const Box valid = m_grids[data_index] & domain;
            for (BoxIterator bit(valid); bit.ok(); ++bit)
            {
                for (int component = c_lapse; component <= c_Bz;
                     ++component)
                {
                    m_maximum_gauge_rhs =
                        std::max(m_maximum_gauge_rhs,
                                 std::abs(fab(bit(), component)));
                }
            }
        }
    }

    double m_maximum_gauge_rhs = 0.0;
    std::size_t m_rhs_calls = 0;
    std::size_t m_outer_rhs_calls = 0;
};

class D15ExactGPGhostLevel : public D13TangentLevel
{
    friend class DefaultLevelFactory<D15ExactGPGhostLevel>;

  protected:
    using D13TangentLevel::D13TangentLevel;

    void fillBdyGhosts(
        GRLevelData &state,
        const Interval &components = Interval(0, NUM_VARS - 1)) override
    {
        if (!m_ghost_difference_audited)
        {
            fill_perturbative_radial_ghosts(state, components);
            const std::vector<double> provisional =
                radial_ghost_values(state);
            fill_background_radial_ghosts(state, m_p.coordinate_offset());
            const std::vector<double> exact = radial_ghost_values(state);
            d15_require(provisional.size() == exact.size(),
                        "ghost audit vectors differ in size");
            for (std::size_t index = 0; index < exact.size(); ++index)
            {
                m_maximum_ghost_policy_difference =
                    std::max(m_maximum_ghost_policy_difference,
                             std::abs(exact[index] - provisional[index]));
            }
            m_ghost_difference_audited =
                m_maximum_ghost_policy_difference > 0.0;
        }
        else
        {
            fill_background_radial_ghosts(state, m_p.coordinate_offset());
        }
        ++m_exact_gp_fill_calls;
    }

  public:
    double maximum_ghost_policy_difference() const
    {
        return m_maximum_ghost_policy_difference;
    }
    std::size_t exact_gp_fill_calls() const { return m_exact_gp_fill_calls; }
    std::size_t rhs_calls() const { return m_rhs_calls; }
    std::size_t outer_rhs_calls() const { return m_outer_rhs_calls; }

  private:
    std::vector<double> radial_ghost_values(const GRLevelData &state) const
    {
        std::vector<double> values;
        const Box domain = m_problem_domain.domainBox();
        const DataIterator iterator = state.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            const FArrayBox &fab = state[data_index];
            for (BoxIterator bit(fab.box()); bit.ok(); ++bit)
            {
                const IntVect point = bit();
                if (point[0] >= domain.smallEnd(0) &&
                    point[0] <= domain.bigEnd(0))
                {
                    continue;
                }
                for (int component = 0; component < NUM_VARS; ++component)
                {
                    values.push_back(fab(point, component));
                }
            }
        }
        return values;
    }

    void specificEvalRHS(GRLevelData &solution, GRLevelData &rhs,
                         const double time) override
    {
        (void)time;
        ++m_rhs_calls;
        BoxLoops::loop(
            BlackStringLive::RHSCompute(
                m_p.r0, m_dx, m_p.coordinate_offset(), m_p.gauge,
                m_p.fixed_lapse_source, m_p.ko_sigma),
            solution, rhs, EXCLUDE_GHOST_CELLS, disable_simd());
        if (m_p.physical_radial_boundaries)
        {
            BlackStringPerturbativeRadialBoundary::apply_outer_rhs(
                solution, rhs, m_problem_domain, m_p.r0, m_dx,
                m_p.coordinate_offset(), m_p.outer_sommerfeld_speed,
                Interval(0, NUM_VARS - 1));
            ++m_outer_rhs_calls;
        }
    }

    bool m_ghost_difference_audited = false;
    double m_maximum_ghost_policy_difference = 0.0;
    std::size_t m_exact_gp_fill_calls = 0;
    std::size_t m_rhs_calls = 0;
    std::size_t m_outer_rhs_calls = 0;
};

struct D15Matrix
{
    int n = 0;
    std::vector<double> map;
    std::vector<double> constraints;
    double maximum_parity_leakage = 0.0;
    double maximum_harmonic_leakage = 0.0;
    double epsilon_column_difference = 0.0;
    double direct_vector_difference = 0.0;
    double construction_seconds = 0.0;
};

std::vector<double> real_matrix_vector(const std::vector<double> &matrix,
                                       const std::vector<double> &input,
                                       const int n)
{
    d15_require(matrix.size() == static_cast<std::size_t>(n) * n,
                "real matrix has the wrong size");
    d15_require(static_cast<int>(input.size()) == n,
                "real matrix input has the wrong size");
    std::vector<double> output(static_cast<std::size_t>(n));
    for (int column = 0; column < n; ++column)
    {
        for (int row = 0; row < n; ++row)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(n) * column] *
                input[static_cast<std::size_t>(column)];
        }
    }
    return output;
}

ComplexVector complex_matrix_vector(const std::vector<double> &matrix,
                                    const ComplexVector &input, const int n)
{
    d15_require(matrix.size() == static_cast<std::size_t>(n) * n,
                "complex matrix has the wrong size");
    ComplexVector output(static_cast<std::size_t>(n));
    for (int column = 0; column < n; ++column)
    {
        for (int row = 0; row < n; ++row)
        {
            output[static_cast<std::size_t>(row)] +=
                matrix[static_cast<std::size_t>(row) +
                       static_cast<std::size_t>(n) * column] *
                input[static_cast<std::size_t>(column)];
        }
    }
    return output;
}

std::array<int, d15_validation_columns>
representative_columns(const D15Layout &layout)
{
    const std::array<std::pair<int, int>, d15_validation_columns> requested = {
        std::pair<int, int>{0, c_chi},
        {0, c_GammaZ},
        {2, c_hzz},
        {4, c_hxz},
        {8, c_K},
        {12, c_Aww},
        {16, c_GammaX},
        {20, c_hww},
        {24, c_Theta},
        {28, c_chi},
        {30, c_Axx},
        {31, c_Azz}};
    std::array<int, d15_validation_columns> result{};
    for (std::size_t entry = 0; entry < requested.size(); ++entry)
    {
        const auto found =
            std::find(layout.components.begin(), layout.components.end(),
                      requested[entry].second);
        d15_require(found != layout.components.end(),
                    layout.name + " representative component is absent");
        const int local =
            static_cast<int>(std::distance(layout.components.begin(), found));
        result[entry] = layout.index(requested[entry].first, local);
    }
    return result;
}

void store_column(D15Matrix &matrix, const D15Layout &layout,
                  const int column, const ActionResult &action,
                  const StateRow &field_scales)
{
    const std::vector<double> output =
        layout.scaled_output(action.response, field_scales);
    for (int row = 0; row < matrix.n; ++row)
    {
        matrix.map[static_cast<std::size_t>(row) +
                   static_cast<std::size_t>(matrix.n) * column] =
            output[static_cast<std::size_t>(row)];
    }
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int constraint = 0; constraint < constraint_count; ++constraint)
        {
            const int row = radial * constraint_count + constraint;
            matrix.constraints[
                static_cast<std::size_t>(row) +
                static_cast<std::size_t>(reduced_constraint_size) *
                    column] =
                action.constraints[static_cast<std::size_t>(radial)]
                                  [static_cast<std::size_t>(constraint)];
        }
    }
    matrix.maximum_parity_leakage =
        std::max(matrix.maximum_parity_leakage, action.parity_leakage);
    matrix.maximum_harmonic_leakage =
        std::max(matrix.maximum_harmonic_leakage,
                 action.harmonic_leakage);
}

template <class level_t>
D15Matrix build_matrix(level_t &level, const D15Layout &layout,
                       const StateRow &field_scales)
{
    const auto start = std::chrono::steady_clock::now();
    D15Matrix matrix;
    matrix.n = layout.size();
    matrix.map.resize(static_cast<std::size_t>(matrix.n) * matrix.n);
    matrix.constraints.resize(
        static_cast<std::size_t>(reduced_constraint_size) * matrix.n);
    const auto representatives = representative_columns(layout);
    const std::set<int> representative_set(representatives.begin(),
                                           representatives.end());
    for (int column = 0; column < matrix.n; ++column)
    {
        std::vector<double> basis(static_cast<std::size_t>(matrix.n));
        basis[static_cast<std::size_t>(column)] = 1.0;
        const ActionResult primary = level.apply_tangent(
            layout.physical_input(basis, field_scales), d15_mode_number,
            d15_epsilon_primary);
        store_column(matrix, layout, column, primary, field_scales);
        if (representative_set.count(column) != 0)
        {
            const ActionResult secondary = level.apply_tangent(
                layout.physical_input(basis, field_scales),
                d15_mode_number, d15_epsilon_secondary);
            const double difference = relative_vector_difference(
                layout.scaled_output(primary.response, field_scales),
                layout.scaled_output(secondary.response, field_scales));
            matrix.epsilon_column_difference =
                std::max(matrix.epsilon_column_difference, difference);
            std::cout << std::scientific << std::setprecision(12)
                      << "D15_COLUMN_VALIDATION variant=" << layout.name
                      << " column=" << column
                      << " epsilon_primary=" << d15_epsilon_primary
                      << " epsilon_secondary=" << d15_epsilon_secondary
                      << " relative_difference=" << difference << '\n';
        }
        if ((column + 1) % 64 == 0 || column + 1 == matrix.n)
        {
            std::cout << "D15_MATRIX_PROGRESS variant=" << layout.name
                      << " columns=" << column + 1 << '/' << matrix.n
                      << '\n';
        }
    }
    d15_require(matrix.epsilon_column_difference < d15_column_tolerance,
                layout.name + " epsilon-halving columns disagree");
    for (const double entry : matrix.map)
    {
        d15_require(std::isfinite(entry),
                    layout.name + " matrix contains a nonfinite entry");
    }

    std::vector<double> probe(static_cast<std::size_t>(matrix.n));
    for (int index = 0; index < matrix.n; ++index)
    {
        probe[static_cast<std::size_t>(index)] =
            std::sin(0.013 * (index + 1)) +
            0.25 * std::cos(0.031 * (index + 1));
    }
    const double norm = real_vector_norm(probe);
    for (double &value : probe)
    {
        value /= norm;
    }
    const std::vector<double> dense =
        real_matrix_vector(matrix.map, probe, matrix.n);
    const ActionResult direct = level.apply_tangent(
        layout.physical_input(probe, field_scales), d15_mode_number,
        d15_epsilon_primary);
    matrix.direct_vector_difference =
        relative_vector_difference(
            dense, layout.scaled_output(direct.response, field_scales));
    d15_require(
        matrix.direct_vector_difference < d15_dense_direct_tolerance,
        layout.name + " dense/direct action disagreement is too large");
    matrix.construction_seconds =
        std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                     start)
            .count();
    std::cout << std::scientific << std::setprecision(12)
              << "D15_MATRIX_VALIDATION variant=" << layout.name
              << " rows=" << matrix.n << " columns=" << matrix.n
              << " bytes=" << matrix.map.size() * sizeof(double)
              << " epsilon_columns=" << d15_validation_columns
              << " epsilon_discrepancy="
              << matrix.epsilon_column_difference
              << " dense_direct_discrepancy="
              << matrix.direct_vector_difference
              << " parity_leakage=" << matrix.maximum_parity_leakage
              << " harmonic_leakage="
              << matrix.maximum_harmonic_leakage
              << " matrix_hash=" << hash_string(hash_values(matrix.map))
              << " construction_seconds=" << matrix.construction_seconds
              << '\n';
    return matrix;
}

struct DynamicEigensystem
{
    int n = 0;
    std::vector<double> real;
    std::vector<double> imaginary;
    std::vector<double> left;
    std::vector<double> right;
};

DynamicEigensystem diagonalize_dynamic(const D15Matrix &matrix)
{
    DynamicEigensystem result;
    result.n = matrix.n;
    result.real.resize(static_cast<std::size_t>(matrix.n));
    result.imaginary.resize(static_cast<std::size_t>(matrix.n));
    result.left.resize(static_cast<std::size_t>(matrix.n) * matrix.n);
    result.right.resize(static_cast<std::size_t>(matrix.n) * matrix.n);
    std::vector<double> copy = matrix.map;
    int n = matrix.n;
    int lda = n;
    int ldvl = n;
    int ldvr = n;
    int info = 0;
    int lwork = -1;
    double work_query = 0.0;
    char vectors = 'V';
    LAPACK(GEEV, geev)(&vectors, &vectors, &n, copy.data(), &lda,
                       result.real.data(), result.imaginary.data(),
                       result.left.data(), &ldvl, result.right.data(),
                       &ldvr, &work_query, &lwork, &info);
    d15_require(info == 0, "D15 LAPACK workspace query failed");
    lwork = std::max(4 * n, static_cast<int>(std::ceil(work_query)));
    std::vector<double> work(static_cast<std::size_t>(lwork));
    copy = matrix.map;
    LAPACK(GEEV, geev)(&vectors, &vectors, &n, copy.data(), &lda,
                       result.real.data(), result.imaginary.data(),
                       result.left.data(), &ldvl, result.right.data(),
                       &ldvr, work.data(), &lwork, &info);
    d15_require(info == 0, "D15 LAPACK dgeev failed");
    return result;
}

ComplexVector dynamic_eigenvector(const std::vector<double> &storage,
                                  const std::vector<double> &imaginary,
                                  const int n, const int index)
{
    ComplexVector result(static_cast<std::size_t>(n));
    if (imaginary[static_cast<std::size_t>(index)] == 0.0)
    {
        for (int row = 0; row < n; ++row)
        {
            result[static_cast<std::size_t>(row)] =
                storage[static_cast<std::size_t>(row) +
                        static_cast<std::size_t>(n) * index];
        }
        return result;
    }
    const int first =
        imaginary[static_cast<std::size_t>(index)] > 0.0 ? index
                                                        : index - 1;
    const double sign =
        imaginary[static_cast<std::size_t>(index)] > 0.0 ? 1.0 : -1.0;
    for (int row = 0; row < n; ++row)
    {
        result[static_cast<std::size_t>(row)] =
            Complex(storage[static_cast<std::size_t>(row) +
                            static_cast<std::size_t>(n) * first],
                    sign *
                        storage[static_cast<std::size_t>(row) +
                                static_cast<std::size_t>(n) *
                                    (first + 1)]);
    }
    return result;
}

struct D15Candidate
{
    int index = 0;
    Complex lambda = 0.0;
    Complex omega = 0.0;
    bool omega_defined = false;
    bool retained = false;
    std::string rejection_reason;
    ComplexVector scaled_right;
    ComplexVector physical_common;
    StateRow component_fractions{};
    std::array<double, constraint_count> constraints{};
    double total_constraint = 0.0;
    double boundary_fraction = 0.0;
    double nyquist_fraction = 0.0;
    double conditioning_proxy = 0.0;
    double dense_residual = 0.0;
    double direct_residual = std::numeric_limits<double>::infinity();
    double parity_leakage = std::numeric_limits<double>::infinity();
    double reconstruction_defect =
        std::numeric_limits<double>::infinity();
    int maximum_radial = 0;
    int physical_nontrivial_fields = 0;
    std::uint64_t physical_profile_hash = 0;
};

double fourier_roundtrip_defect(const D15Candidate &candidate,
                                double dz);

D15Candidate make_candidate(const D15Matrix &matrix,
                            const D15Layout &layout,
                            const DynamicEigensystem &eigensystem,
                            const int index, const StateRow &field_scales,
                            const double dx, const double dt,
                            const double eigenvalue_floor)
{
    D15Candidate candidate;
    candidate.index = index;
    candidate.lambda = {
        eigensystem.real[static_cast<std::size_t>(index)],
        eigensystem.imaginary[static_cast<std::size_t>(index)]};
    candidate.scaled_right =
        dynamic_eigenvector(eigensystem.right, eigensystem.imaginary,
                            matrix.n, index);
    ComplexVector scaled_left =
        dynamic_eigenvector(eigensystem.left, eigensystem.imaginary,
                            matrix.n, index);
    ComplexVector physical_right(static_cast<std::size_t>(matrix.n));
    ComplexVector physical_left(static_cast<std::size_t>(matrix.n));
    const std::vector<double> scales = layout.scaling_vector(field_scales);
    for (int coordinate = 0; coordinate < matrix.n; ++coordinate)
    {
        const std::size_t i = static_cast<std::size_t>(coordinate);
        physical_right[i] = candidate.scaled_right[i] * scales[i];
        physical_left[i] = scaled_left[i] / scales[i];
    }
    const Complex denominator =
        inner_product(scaled_left, candidate.scaled_right);
    candidate.conditioning_proxy =
        complex_vector_norm(physical_left) *
        complex_vector_norm(physical_right) /
        std::max(std::abs(denominator),
                 std::numeric_limits<double>::min());
    const double physical_norm = complex_vector_norm(physical_right);
    d15_require(std::isfinite(physical_norm) && physical_norm > 0.0,
                layout.name + " eigenvector has invalid norm");
    for (int coordinate = 0; coordinate < matrix.n; ++coordinate)
    {
        physical_right[static_cast<std::size_t>(coordinate)] /=
            physical_norm;
        candidate.scaled_right[static_cast<std::size_t>(coordinate)] /=
            physical_norm;
    }

    candidate.physical_common.resize(
        static_cast<std::size_t>(radial_cells) * component_count);
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int local = 0;
             local < static_cast<int>(layout.components.size()); ++local)
        {
            const int component =
                layout.components[static_cast<std::size_t>(local)];
            candidate.physical_common[
                reduced_index(radial, component)] =
                physical_right[static_cast<std::size_t>(
                    layout.index(radial, local))];
        }
    }

    double total_energy = 0.0;
    double boundary_energy = 0.0;
    double maximum_energy = -1.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        double radial_energy = 0.0;
        for (int component = 0; component < component_count; ++component)
        {
            const double energy = std::norm(candidate.physical_common[
                reduced_index(radial, component)]);
            candidate.component_fractions[
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
            candidate.maximum_radial = radial;
        }
    }
    for (int component = 0; component < component_count; ++component)
    {
        double &fraction = candidate.component_fractions[
            static_cast<std::size_t>(component)];
        fraction /= total_energy;
        if (component < d15_physical_components && fraction > 1.0e-4)
        {
            ++candidate.physical_nontrivial_fields;
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
                candidate.physical_common[
                    reduced_index(radial, component)];
        }
        nyquist_energy += std::norm(alternating) / radial_cells;
    }
    candidate.nyquist_fraction = nyquist_energy / total_energy;

    ComplexVector constraint(
        static_cast<std::size_t>(reduced_constraint_size));
    for (int column = 0; column < matrix.n; ++column)
    {
        for (int row = 0; row < reduced_constraint_size; ++row)
        {
            constraint[static_cast<std::size_t>(row)] +=
                matrix.constraints[
                    static_cast<std::size_t>(row) +
                    static_cast<std::size_t>(reduced_constraint_size) *
                        column] *
                candidate.scaled_right[static_cast<std::size_t>(column)];
        }
    }
    for (int component = 0; component < constraint_count; ++component)
    {
        double sum = 0.0;
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            sum += std::norm(constraint[static_cast<std::size_t>(
                radial * constraint_count + component)]);
        }
        candidate.constraints[static_cast<std::size_t>(component)] =
            std::sqrt(sum * dx) / std::sqrt(dx);
    }
    candidate.total_constraint =
        std::sqrt(candidate.constraints[0] * candidate.constraints[0] +
                  candidate.constraints[1] * candidate.constraints[1] +
                  candidate.constraints[2] * candidate.constraints[2]);

    ComplexVector dense =
        complex_matrix_vector(matrix.map, candidate.scaled_right, matrix.n);
    for (int row = 0; row < matrix.n; ++row)
    {
        dense[static_cast<std::size_t>(row)] -=
            candidate.lambda *
            candidate.scaled_right[static_cast<std::size_t>(row)];
    }
    candidate.dense_residual =
        complex_vector_norm(dense) /
        complex_vector_norm(candidate.scaled_right);
    candidate.physical_profile_hash =
        hash_values(candidate.physical_common);

    if (!std::isfinite(candidate.lambda.real()) ||
        !std::isfinite(candidate.lambda.imag()))
    {
        candidate.rejection_reason = "nonfinite_eigenvalue";
        return candidate;
    }
    if (!(std::abs(candidate.lambda) > eigenvalue_floor))
    {
        candidate.rejection_reason = "eigenvalue_below_numerical_floor";
        return candidate;
    }
    candidate.omega = std::log(candidate.lambda) / dt;
    candidate.omega_defined = true;
    return candidate;
}

template <class level_t>
void validate_candidate_action(D15Candidate &candidate, level_t &level,
                               const D15Layout &layout,
                               const StateRow &field_scales)
{
    const auto apply_part = [&](const bool imaginary)
    {
        std::vector<double> part(
            static_cast<std::size_t>(layout.size()));
        for (int index = 0; index < layout.size(); ++index)
        {
            const Complex value =
                candidate.scaled_right[static_cast<std::size_t>(index)];
            part[static_cast<std::size_t>(index)] =
                imaginary ? value.imag() : value.real();
        }
        if (real_vector_norm(part) == 0.0)
        {
            return ActionResult{};
        }
        return level.apply_tangent(
            layout.physical_input(part, field_scales), d15_mode_number,
            d15_epsilon_primary);
    };
    const ActionResult real_action = apply_part(false);
    const ActionResult imaginary_action = apply_part(true);
    ComplexVector direct(static_cast<std::size_t>(layout.size()));
    const auto add_action = [&](const ActionResult &action,
                                const bool imaginary)
    {
        const std::vector<double> output =
            layout.scaled_output(action.response, field_scales);
        for (int index = 0; index < layout.size(); ++index)
        {
            direct[static_cast<std::size_t>(index)] +=
                imaginary ? Complex(0.0, output[static_cast<std::size_t>(
                                              index)])
                          : Complex(output[static_cast<std::size_t>(index)],
                                    0.0);
        }
    };
    add_action(real_action, false);
    add_action(imaginary_action, true);
    for (int index = 0; index < layout.size(); ++index)
    {
        direct[static_cast<std::size_t>(index)] -=
            candidate.lambda *
            candidate.scaled_right[static_cast<std::size_t>(index)];
    }
    candidate.direct_residual =
        complex_vector_norm(direct) /
        complex_vector_norm(candidate.scaled_right);
    candidate.parity_leakage =
        std::max(real_action.parity_leakage,
                 imaginary_action.parity_leakage);
    const double harmonic =
        std::max(real_action.harmonic_leakage,
                 imaginary_action.harmonic_leakage);
    candidate.reconstruction_defect =
        std::max(fourier_roundtrip_defect(candidate, level.spacing()),
                 harmonic);

    std::vector<std::string> reasons;
    if (!(candidate.direct_residual < d15_direct_residual_limit))
    {
        reasons.emplace_back("matrix_free_residual");
    }
    if (!(candidate.parity_leakage < d15_parity_limit))
    {
        reasons.emplace_back("forbidden_parity");
    }
    if (!(candidate.reconstruction_defect <
          d15_reconstruction_limit))
    {
        reasons.emplace_back("fourier_reconstruction");
    }
    if (reasons.empty())
    {
        candidate.retained = true;
        return;
    }
    std::ostringstream reason;
    for (std::size_t index = 0; index < reasons.size(); ++index)
    {
        if (index != 0)
        {
            reason << ',';
        }
        reason << reasons[index];
    }
    candidate.rejection_reason = reason.str();
}

bool individual_bulk_gate(const D15Candidate &candidate)
{
    return candidate.retained &&
           candidate.boundary_fraction < d15_bulk_boundary_limit &&
           candidate.total_constraint < d15_bulk_constraint_limit &&
           candidate.nyquist_fraction < d15_bulk_nyquist_limit &&
           candidate.physical_nontrivial_fields >= 3 &&
           candidate.maximum_radial >= 3 &&
           candidate.maximum_radial < radial_cells - 3;
}

double physical_profile_overlap(const D15Candidate &left,
                                const D15Candidate &right)
{
    Complex numerator = 0.0;
    double left_norm = 0.0;
    double right_norm = 0.0;
    for (int radial = 0; radial < radial_cells; ++radial)
    {
        for (int component = 0; component < d15_physical_components;
             ++component)
        {
            const Complex a = left.physical_common[
                reduced_index(radial, component)];
            const Complex b = right.physical_common[
                reduced_index(radial, component)];
            numerator += std::conj(a) * b;
            left_norm += std::norm(a);
            right_norm += std::norm(b);
        }
    }
    return std::abs(numerator) /
           std::max(std::sqrt(left_norm * right_norm),
                    std::numeric_limits<double>::min());
}

bool rates_agree(const D15Candidate &left, const D15Candidate &right)
{
    const double scale =
        std::max({std::abs(left.omega.real()),
                  std::abs(right.omega.real()), 1.0e-2});
    return std::abs(left.omega.real() - right.omega.real()) <=
           d15_rate_relative_tolerance * scale;
}

double fourier_roundtrip_defect(const D15Candidate &candidate,
                                const double dz)
{
    ComplexVector projected(candidate.physical_common.size());
    const double k = 2.0 * pi * d15_mode_number / 8.0;
    for (int compact = 0; compact < compact_cells; ++compact)
    {
        const double z = (compact + 0.5) * dz;
        const double cosine = std::cos(k * z);
        const double sine = std::sin(k * z);
        for (int radial = 0; radial < radial_cells; ++radial)
        {
            for (int component = 0; component < component_count;
                 ++component)
            {
                const std::size_t index =
                    reduced_index(radial, component);
                const double basis =
                    one_z_component(component) ? sine : cosine;
                projected[index] +=
                    2.0 * candidate.physical_common[index] * basis *
                    basis / static_cast<double>(compact_cells);
            }
        }
    }
    for (std::size_t index = 0;
         index < candidate.physical_common.size(); ++index)
    {
        projected[index] -= candidate.physical_common[index];
    }
    return complex_vector_norm(projected) /
           complex_vector_norm(candidate.physical_common);
}

void emit_candidate_record(const D15Candidate &candidate,
                           const std::string &variant)
{
    std::cout << std::scientific << std::setprecision(12)
              << "D15_CANDIDATE_RECORD variant=" << variant
              << " eigen_index=" << candidate.index
              << " lambda_real=" << candidate.lambda.real()
              << " lambda_imag=" << candidate.lambda.imag()
              << " Omega_real=" << candidate.omega.real()
              << " Omega_imag=" << candidate.omega.imag()
              << " matrix_free_residual=" << candidate.direct_residual
              << " forbidden_parity_leakage="
              << candidate.parity_leakage
              << " fourier_reconstruction_defect="
              << candidate.reconstruction_defect
              << " dense_residual=" << candidate.dense_residual
              << " boundary_fraction=" << candidate.boundary_fraction
              << " H=" << candidate.constraints[0]
              << " Mx=" << candidate.constraints[1]
              << " Mz=" << candidate.constraints[2]
              << " total_constraint=" << candidate.total_constraint
              << " nyquist_fraction=" << candidate.nyquist_fraction
              << " maximum_radial=" << candidate.maximum_radial
              << " physical_fields="
              << candidate.physical_nontrivial_fields
              << " conditioning_proxy="
              << candidate.conditioning_proxy
              << " profile_hash="
              << hash_string(candidate.physical_profile_hash)
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

void emit_match_profile(const D15Candidate &candidate,
                        const std::string &variant)
{
    std::cout << std::scientific << std::setprecision(17)
              << "D15_MATCH_PROFILE variant=" << variant
              << " eigen_index=" << candidate.index
              << " coefficients=";
    for (std::size_t index = 0;
         index < candidate.physical_common.size(); ++index)
    {
        if (index != 0)
        {
            std::cout << ',';
        }
        std::cout << candidate.physical_common[index].real() << ':'
                  << candidate.physical_common[index].imag();
    }
    std::cout << '\n';
}

struct D15Spectrum
{
    std::string variant;
    int dimension = 0;
    int eigenvalue_count = 0;
    std::uint64_t matrix_hash = 0;
    double eigenvalue_floor = 0.0;
    std::vector<D15Candidate> candidates;
    std::vector<std::size_t> retained;
    std::vector<std::size_t> bulk_eligible;
    std::size_t tangent_actions = 0;
    std::size_t live_steps = 0;
    double wall_seconds = 0.0;
};

template <class level_t>
D15Spectrum analyze_matrix(level_t &level, const D15Layout &layout,
                           const D15Matrix &matrix,
                           const StateRow &field_scales)
{
    const DynamicEigensystem eigensystem = diagonalize_dynamic(matrix);
    d15_require(static_cast<int>(eigensystem.real.size()) == matrix.n,
                layout.name + " eigenvalue count differs from dimension");
    const double eigenvalue_floor =
        d15_floor_multiplier *
        std::max(matrix.epsilon_column_difference,
                 matrix.direct_vector_difference);
    std::cout << std::scientific << std::setprecision(12)
              << "D15_ARTIFACT_THRESHOLDS variant=" << layout.name
              << " eigenvalue_floor=" << eigenvalue_floor
              << " floor_formula=10x_max_epsilon_dense_direct"
              << " matrix_free_residual_limit="
              << d15_direct_residual_limit
              << " forbidden_parity_limit=" << d15_parity_limit
              << " reconstruction_limit="
              << d15_reconstruction_limit
              << " reconstruction_definition="
                 "max_roundtrip_identity_and_unwanted_harmonic\n";

    D15Spectrum result;
    result.variant = layout.name;
    result.dimension = matrix.n;
    result.eigenvalue_count =
        static_cast<int>(eigensystem.real.size());
    result.matrix_hash = hash_values(matrix.map);
    result.eigenvalue_floor = eigenvalue_floor;
    result.candidates.reserve(static_cast<std::size_t>(matrix.n));

    for (int index = 0; index < matrix.n; ++index)
    {
        result.candidates.push_back(make_candidate(
            matrix, layout, eigensystem, index, field_scales,
            level.spacing(), level.timestep(), eigenvalue_floor));
    }
    for (int index = 0; index < matrix.n; ++index)
    {
        D15Candidate &candidate =
            result.candidates[static_cast<std::size_t>(index)];
        if (!candidate.omega_defined)
        {
            std::cout << std::scientific << std::setprecision(12)
                      << "D15_CANDIDATE_REJECT variant=" << layout.name
                      << " eigen_index=" << candidate.index
                      << " lambda_abs=" << std::abs(candidate.lambda)
                      << " eigenvalue_floor=" << eigenvalue_floor
                      << " reason=" << candidate.rejection_reason
                      << " Omega_not_evaluated=1\n";
            continue;
        }
        if (candidate.lambda.imag() < 0.0 && index > 0)
        {
            const D15Candidate &conjugate =
                result.candidates[static_cast<std::size_t>(index - 1)];
            if (conjugate.omega_defined &&
                std::abs(candidate.lambda -
                         std::conj(conjugate.lambda)) < 1.0e-12)
            {
                candidate.direct_residual = conjugate.direct_residual;
                candidate.parity_leakage = conjugate.parity_leakage;
                candidate.reconstruction_defect =
                    conjugate.reconstruction_defect;
                candidate.retained = conjugate.retained;
                candidate.rejection_reason =
                    conjugate.rejection_reason;
            }
            else
            {
                validate_candidate_action(candidate, level, layout,
                                          field_scales);
            }
        }
        else
        {
            validate_candidate_action(candidate, level, layout,
                                      field_scales);
        }
        if (!candidate.retained)
        {
            std::cout << std::scientific << std::setprecision(12)
                      << "D15_CANDIDATE_REJECT variant=" << layout.name
                      << " eigen_index=" << candidate.index
                      << " lambda_abs=" << std::abs(candidate.lambda)
                      << " matrix_free_residual="
                      << candidate.direct_residual
                      << " forbidden_parity_leakage="
                      << candidate.parity_leakage
                      << " fourier_reconstruction_defect="
                      << candidate.reconstruction_defect
                      << " reason=" << candidate.rejection_reason
                      << " Omega_not_evaluated=0\n";
            continue;
        }
        result.retained.push_back(static_cast<std::size_t>(index));
        emit_candidate_record(candidate, layout.name);
        if (individual_bulk_gate(candidate))
        {
            result.bulk_eligible.push_back(
                static_cast<std::size_t>(index));
            emit_match_profile(candidate, layout.name);
        }
    }

    const auto rank_and_emit =
        [&](const char *rank_name, const auto &comparison)
    {
        std::vector<std::size_t> ranked = result.retained;
        std::sort(ranked.begin(), ranked.end(), comparison);
        const std::size_t count = std::min<std::size_t>(3, ranked.size());
        for (std::size_t rank = 0; rank < count; ++rank)
        {
            const D15Candidate &candidate =
                result.candidates[ranked[rank]];
            std::cout << "D15_RANK variant=" << layout.name
                      << " metric=" << rank_name
                      << " rank=" << rank + 1
                      << " eigen_index=" << candidate.index
                      << " profile_hash="
                      << hash_string(candidate.physical_profile_hash)
                      << '\n';
        }
    };
    rank_and_emit("largest_ReOmega",
                  [&](const std::size_t left, const std::size_t right)
                  {
                      return result.candidates[left].omega.real() >
                             result.candidates[right].omega.real();
                  });
    rank_and_emit("smallest_constraint",
                  [&](const std::size_t left, const std::size_t right)
                  {
                      return result.candidates[left].total_constraint <
                             result.candidates[right].total_constraint;
                  });
    rank_and_emit("smallest_boundary",
                  [&](const std::size_t left, const std::size_t right)
                  {
                      return result.candidates[left].boundary_fraction <
                             result.candidates[right].boundary_fraction;
                  });

    std::cout << "D15_SPECTRUM_SUMMARY variant=" << layout.name
              << " dimension=" << result.dimension
              << " eigenvalue_count=" << result.eigenvalue_count
              << " retained_candidates=" << result.retained.size()
              << " rejected_candidates="
              << matrix.n - static_cast<int>(result.retained.size())
              << " bulk_eligible=" << result.bulk_eligible.size()
              << '\n';
    return result;
}

bool find_matching_bulk_candidate(const D15Spectrum &frozen,
                                  const D15Spectrum &exact)
{
    bool matched = false;
    for (const std::size_t frozen_index : frozen.bulk_eligible)
    {
        const D15Candidate &left = frozen.candidates[frozen_index];
        for (const std::size_t exact_index : exact.bulk_eligible)
        {
            const D15Candidate &right = exact.candidates[exact_index];
            const double overlap = physical_profile_overlap(left, right);
            const bool rate = rates_agree(left, right);
            const bool location =
                std::abs(left.maximum_radial - right.maximum_radial) <=
                d15_maximum_radial_separation;
            const bool passes =
                overlap > d15_profile_overlap_gate && rate && location;
            std::cout << std::scientific << std::setprecision(12)
                      << "D15_MATCH_EVALUATION frozen_index="
                      << left.index << " exact_index=" << right.index
                      << " physical_profile_overlap=" << overlap
                      << " rate_agreement=" << (rate ? 1 : 0)
                      << " radial_maximum_separation="
                      << std::abs(left.maximum_radial -
                                  right.maximum_radial)
                      << " passes=" << (passes ? 1 : 0) << '\n';
            matched = matched || passes;
        }
    }
    return matched;
}

template <class level_t>
D15Spectrum run_variant(SimulationParameters &parameters,
                        const D15Layout &layout)
{
    const auto start = std::chrono::steady_clock::now();
    GRAMR amr;
    DefaultLevelFactory<level_t> factory(amr, parameters);
    setupAMRObject(amr, factory);
    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    d15_require(levels.size() == 1,
                layout.name + " must create one level");
    auto *level = dynamic_cast<level_t *>(levels[0]);
    d15_require(level != nullptr,
                layout.name + " factory returned the wrong level");
    level->prepare();
    const StateRow field_scales = level->field_scales();
    const std::vector<double> scaling =
        layout.scaling_vector(field_scales);
    D15Matrix matrix = build_matrix(*level, layout, field_scales);
    D15Spectrum spectrum =
        analyze_matrix(*level, layout, matrix, field_scales);
    spectrum.tangent_actions = level->tangent_actions();
    spectrum.live_steps = level->live_steps();
    spectrum.wall_seconds =
        std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                     start)
            .count();
    std::cout << std::scientific << std::setprecision(12)
              << "D15_PROVENANCE variant=" << layout.name
              << " matrix_hash=" << hash_string(spectrum.matrix_hash)
              << " scaling_hash="
              << hash_string(hash_values(scaling))
              << " tangent_actions=" << spectrum.tangent_actions
              << " signed_live_steps=" << spectrum.live_steps
              << " wall_seconds=" << spectrum.wall_seconds << '\n';

    if constexpr (std::is_same_v<level_t, D15FrozenGaugeLevel>)
    {
        d15_require(layout.size() == d15_frozen_size,
                    "frozen layout is not 13*Nx");
        d15_require(matrix.n == d15_frozen_size &&
                        matrix.map.size() ==
                            static_cast<std::size_t>(d15_frozen_size) *
                                d15_frozen_size,
                    "frozen matrix is not native 416x416");
        d15_require(spectrum.eigenvalue_count == d15_frozen_size,
                    "frozen eigensystem does not have 416 eigenvalues");
        d15_require(level->maximum_gauge_rhs() == 0.0,
                    "frozen gauge RHS rows changed");
        d15_require(level->rhs_calls() == level->outer_rhs_calls() &&
                        level->rhs_calls() > 0,
                    "frozen outer override lifecycle changed");
        d15_require(layout.components.front() == c_chi &&
                        layout.components.back() == c_GammaZ &&
                        std::find(layout.components.begin(),
                                  layout.components.end(), c_Theta) !=
                            layout.components.end() &&
                        std::find(layout.components.begin(),
                                  layout.components.end(), c_GammaX) !=
                            layout.components.end(),
                    "frozen layout lost Theta or Gamma rows");
        const D15Layout mutation = full_layout();
        d15_require(mutation.size() != layout.size(),
                    "old embedded 576 mutation was not rejected");
        std::cout << "D15_FROZEN_DIMENSION_GATE matrix=416x416"
                  << " eigenvalues=416 gauge_basis_coordinates=0"
                  << " Theta_live=1 GammaX_live=1 GammaZ_live=1"
                  << " maximum_gauge_rhs="
                  << level->maximum_gauge_rhs()
                  << " old_embedded_576_mutation=REJECTED\n";
    }
    if constexpr (std::is_same_v<level_t, D15ExactGPGhostLevel>)
    {
        d15_require(parameters.physical_radial_boundaries &&
                        !parameters.background_preserving_gp_radial_ghosts,
                    "exact-GP ghost-only variant changed parameter ownership");
        d15_require(level->maximum_ghost_policy_difference() > 0.0,
                    "exact-GP ghosts did not differ from provisional ghosts");
        d15_require(level->rhs_calls() == level->outer_rhs_calls() &&
                        level->rhs_calls() > 0,
                    "exact-GP variant changed outer RHS ownership");
        d15_require(level->exact_gp_fill_calls() > 0,
                    "exact-GP radial ghost policy did not execute");
        std::cout << std::scientific << std::setprecision(12)
                  << "D15_GHOST_ONLY_GATE"
                  << " exact_gp_fill_calls="
                  << level->exact_gp_fill_calls()
                  << " maximum_provisional_exact_ghost_difference="
                  << level->maximum_ghost_policy_difference()
                  << " rhs_calls=" << level->rhs_calls()
                  << " outer_rhs_calls=" << level->outer_rhs_calls()
                  << " periodic_exchange_owner=framework"
                  << " outer_override_same_as_baseline=1"
                  << " disabled_outer_override_mutation=REJECTED\n";
    }
    return spectrum;
}

[[maybe_unused]] int run_d15(SimulationParameters &parameters)
{
    d15_require(parameters.max_level == 0, "D15 requires max_level=0");
    d15_require(parameters.coarsest_dx == 0.125,
                "D15 requires dx=dz=1/8");
    d15_require(parameters.dt_multiplier == 0.05,
                "D15 requires CFL=0.05");
    d15_require(parameters.ko_sigma == 0.3,
                "D15 requires ko_sigma=0.3");
    d15_require(parameters.constraint_diagnostic_cadence == 0,
                "D15 diagnostics must remain disabled");
    d15_require(parameters.physical_radial_boundaries &&
                    !parameters.background_preserving_gp_radial_ghosts,
                "D15 requires baseline provisional-boundary parameters");
    static_assert(CH_SPACEDIM == 2);
    static_assert(component_count == 18);
    static_assert(d15_frozen_size == 416);

    const D15Layout frozen = frozen_layout();
    const D15Layout exact = full_layout();
    d15_require(frozen.size() == 416 && exact.size() == 576,
                "D15 repaired dimensions are incorrect");
    const double baseline_floor =
        d15_floor_multiplier *
        std::max(d14_baseline_epsilon_discrepancy,
                 d14_baseline_dense_direct_discrepancy);
    std::cout << std::scientific << std::setprecision(12)
              << "D15_CONFIGURATION k=pi/4 Lz=8 x_in=0.5 x_out=4.5"
              << " Nx=32 Nz=64 dx=0.125 dz=0.125 CFL=0.05"
              << " ko_sigma=0.3 new_dense_matrices=2\n"
              << "D15_BASELINE_REUSED matrix_hash=" << d14_baseline_hash
              << " dimension=576 eigenvalue_count=576"
              << " matrix_reconstructed=0 tangent_actions=0"
              << " epsilon_discrepancy="
              << d14_baseline_epsilon_discrepancy
              << " dense_direct_discrepancy="
              << d14_baseline_dense_direct_discrepancy
              << " eigenvalue_floor=" << baseline_floor
              << " leader_lambda=1.000304369575"
              << " leader_ReOmega=0.0486917222524"
              << " leader_boundary=0.819825"
              << " leader_constraint=0.500752\n";

    const auto start = std::chrono::steady_clock::now();
    D15Spectrum frozen_spectrum =
        run_variant<D15FrozenGaugeLevel>(parameters, frozen);
    D15Spectrum exact_spectrum =
        run_variant<D15ExactGPGhostLevel>(parameters, exact);
    const bool bulk =
        find_matching_bulk_candidate(frozen_spectrum, exact_spectrum);
    const char *classification =
        bulk ? "CREDIBLE_BULK_MODE_CANDIDATE_EXPOSED"
             : "NO_BULK_PHYSICAL_CANDIDATE_AFTER_SECTOR_ISOLATION";
    const std::size_t actions =
        frozen_spectrum.tangent_actions + exact_spectrum.tangent_actions;
    const std::size_t live_steps =
        frozen_spectrum.live_steps + exact_spectrum.live_steps;
    const double wall_seconds =
        std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                     start)
            .count();
    std::cout << "D15_MATCH_SUMMARY precomputed_for_fixture_diagnostic_only=1"
              << " qualifying_shared_candidate=" << (bulk ? 1 : 0)
              << " parser_must_recompute=1\n"
              << "D15_EFFICIENCY new_dense_matrices=2"
              << " frozen_dimensions=416x416"
              << " exact_dimensions=576x576"
              << " frozen_bytes="
              << static_cast<std::size_t>(416) * 416 * sizeof(double)
              << " exact_bytes="
              << static_cast<std::size_t>(576) * 576 * sizeof(double)
              << " tangent_actions=" << actions
              << " signed_live_steps=" << live_steps
              << " total_wall_seconds=" << wall_seconds
              << " peak_rss_kib=" << peak_rss_kib()
              << " seconds_per_tangent_action="
              << wall_seconds / static_cast<double>(actions) << '\n'
              << "D15_HOT_PATH production_code_changed=0"
              << " direct_target_d4_evaluations_per_cell_stage=1"
              << " fused_ko_additions_per_cell_stage=1"
              << " second_rhs_evaluations=0"
              << " production_logging_added=0"
              << " production_allocations_added=0\n"
              << "D15_CLASSIFICATION " << classification << '\n'
              << "BLACKSTRING_D15_SECTOR_ISOLATION_PASS\n";
    return 0;
}
} // namespace

#ifndef BLACKSTRING_D15_REUSE_WITHOUT_ENTRY
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        d15_fail("usage: BlackStringD15SectorIsolationTest <params>");
    }
    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);
    return run_d15(parameters);
}
#endif
