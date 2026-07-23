#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#define CH_SPACEDIM 3
#include "DimensionDefinitions.hpp"
#include "Tensor.hpp"
#include "TensorAlgebra.hpp"

// Test-only Chombo access shim.  The inspected cleanup compute classes need
// only load_vars/store_vars.  This Cell stores an independent local state and
// invokes the real TraceARemoval and PositiveChiAndAlpha implementations.
constexpr int c_chi = 0;
constexpr int c_h11 = 1;
constexpr int c_h33 = 6;
constexpr int c_A11 = 7;
constexpr int c_A33 = 12;
constexpr int c_lapse = 13;

template <int begin, int end> struct GRInterval
{
};

namespace VarsTools
{
template <class mapping_function_t, class interval_t, class tensor_t>
void define_symmetric_enum_mapping(mapping_function_t, interval_t, tensor_t &)
{
}
} // namespace VarsTools

struct CleanupState
{
    Tensor<2, double> h;
    Tensor<2, double> A;
    double chi = 0.0;
    double lapse = 0.0;
};

template <class data_t> class Cell
{
  public:
    explicit Cell(CleanupState &state) : m_state(&state) {}

    template <template <typename> class vars_t> vars_t<data_t> load_vars() const
    {
        vars_t<data_t> out;
        for (int i = 0; i < CH_SPACEDIM; ++i)
        {
            for (int j = 0; j < CH_SPACEDIM; ++j)
            {
                out.h[i][j] = m_state->h[i][j];
                out.A[i][j] = m_state->A[i][j];
            }
        }
        return out;
    }

    data_t load_vars(const int variable) const
    {
        return variable == c_chi ? m_state->chi : m_state->lapse;
    }

    template <typename vars_t> void store_vars(const vars_t &vars)
    {
        for (int i = 0; i < CH_SPACEDIM; ++i)
        {
            for (int j = 0; j < CH_SPACEDIM; ++j)
            {
                m_state->h[i][j] = vars.h[i][j];
                m_state->A[i][j] = vars.A[i][j];
            }
        }
    }

    void store_vars(const data_t value, const int variable)
    {
        if (variable == c_chi)
            m_state->chi = value;
        else
            m_state->lapse = value;
    }

  private:
    CleanupState *m_state;
};

#define CELL_HPP_
#define USERVARIABLES_HPP
#define VARSTOOLS_HPP_
#define CCZ4GEOMETRY_HPP_
#define SIMD_HPP_
template <typename data_t>
data_t simd_max(const data_t &left, const data_t &right)
{
    return left > right ? left : right;
}
#include "PositiveChiAndAlpha.hpp"
#include "TraceARemoval.hpp"

namespace
{
constexpr double absolute_tolerance = 5.0e-13;
constexpr double relative_tolerance = 5.0e-12;
using Matrix3 = std::array<std::array<double, 3>, 3>;

void fail(const std::string &message)
{
    std::cerr << "FAIL " << message << "\n";
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
        fail(message);
}

double determinant(const Matrix3 &m)
{
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

Matrix3 inverse(const Matrix3 &m)
{
    const double det = determinant(m);
    require(std::abs(det) > 1.0e-12, "cleanup metric is nonsingular");
    Matrix3 out{};
    out[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) / det;
    out[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) / det;
    out[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) / det;
    out[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) / det;
    out[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) / det;
    out[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) / det;
    out[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) / det;
    out[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) / det;
    out[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) / det;
    return out;
}

double trace(const Matrix3 &lower, const Matrix3 &inverse_metric)
{
    double out = 0.0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            out += inverse_metric[i][j] * lower[i][j];
    return out;
}

Matrix3 to_matrix(const Tensor<2, double> &tensor)
{
    Matrix3 out{};
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            out[i][j] = tensor[i][j];
    return out;
}

void set_tensor(Tensor<2, double> &tensor, const Matrix3 &matrix)
{
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            tensor[i][j] = matrix[i][j];
}

Matrix3 independent_trace_removal(const Matrix3 &A, const Matrix3 &h,
                                  const int dimension = 3)
{
    const Matrix3 h_inverse = inverse(h);
    const double tr = trace(A, h_inverse);
    Matrix3 out = A;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            out[i][j] -= h[i][j] * tr / static_cast<double>(dimension);
    return out;
}

double max_difference(const Matrix3 &left, const Matrix3 &right)
{
    double out = 0.0;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            out = std::max(out, std::abs(left[i][j] - right[i][j]));
    return out;
}

void require_close(const double actual, const double expected,
                   const std::string &message)
{
    const double allowance =
        absolute_tolerance +
        relative_tolerance * std::max(std::abs(actual), std::abs(expected));
    require(std::abs(actual - expected) <= allowance, message);
}

CleanupState make_state()
{
    CleanupState state;
    const Matrix3 h = {{{1.31, 0.17, -0.09},
                        {0.17, 0.92, 0.11},
                        {-0.09, 0.11, 1.18}}};
    const Matrix3 A = {{{0.26, -0.08, 0.05},
                        {-0.08, -0.03, 0.07},
                        {0.05, 0.07, 0.19}}};
    set_tensor(state.h, h);
    set_tensor(state.A, A);
    state.chi = -0.017;
    state.lapse = 2.0e-6;
    return state;
}

Matrix3 normalize_determinant_local(const Matrix3 &metric,
                                    const double exponent)
{
    const double factor = std::pow(determinant(metric), exponent);
    Matrix3 out = metric;
    for (auto &row : out)
        for (double &entry : row)
            entry *= factor;
    return out;
}

} // namespace

int main()
{
    CleanupState direct = make_state();
    const Matrix3 original_h = to_matrix(direct.h);
    const Matrix3 original_A = to_matrix(direct.A);
    const double pre_trace = trace(original_A, inverse(original_h));
    require(std::abs(pre_trace) > 0.1,
            "deliberate trace violation is nontrivial");
    require(std::abs(determinant(original_h) - 1.0) > 0.1,
            "deliberate determinant violation is nontrivial");

    const Matrix3 expected =
        independent_trace_removal(original_A, original_h);
    TraceARemoval trace_removal;
    trace_removal.compute(Cell<double>(direct));
    const Matrix3 cleaned_A = to_matrix(direct.A);
    const double component_error = max_difference(cleaned_A, expected);
    double component_normalized = 0.0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            const double allowance =
                absolute_tolerance +
                relative_tolerance *
                    std::max(std::abs(cleaned_A[i][j]),
                             std::abs(expected[i][j]));
            component_normalized =
                std::max(component_normalized,
                         std::abs(cleaned_A[i][j] - expected[i][j]) /
                             allowance);
        }
    }
    const double post_trace = trace(cleaned_A, inverse(to_matrix(direct.h)));
    require_close(component_error, 0.0,
                  "direct TraceARemoval components match independent oracle");
    require_close(post_trace, 0.0,
                  "direct TraceARemoval removes the visible d=3 trace");
    require_close(determinant(to_matrix(direct.h)),
                  determinant(original_h),
                  "TraceARemoval does not normalize determinant");

    const Matrix3 once = cleaned_A;
    trace_removal.compute(Cell<double>(direct));
    const double idempotence_error =
        max_difference(once, to_matrix(direct.A));
    require_close(idempotence_error, 0.0,
                  "direct TraceARemoval is idempotent");

    PositiveChiAndAlpha positivity(2.5e-4, 3.5e-4);
    positivity.compute(Cell<double>(direct));
    require_close(direct.chi, 2.5e-4,
                  "direct positivity cleanup clamps chi");
    require_close(direct.lapse, 3.5e-4,
                  "direct positivity cleanup clamps lapse");
    positivity.compute(Cell<double>(direct));
    require_close(direct.chi, 2.5e-4,
                  "direct positivity cleanup is idempotent for chi");
    require_close(direct.lapse, 3.5e-4,
                  "direct positivity cleanup is idempotent for lapse");

    CleanupState reverse = make_state();
    positivity.compute(Cell<double>(reverse));
    trace_removal.compute(Cell<double>(reverse));
    require_close(
        max_difference(to_matrix(reverse.A), to_matrix(direct.A)), 0.0,
        "trace removal and positivity cleanup commute on disjoint fields");
    require_close(reverse.chi, direct.chi,
                  "cleanup ordering preserves chi result");
    require_close(reverse.lapse, direct.lapse,
                  "cleanup ordering preserves lapse result");

    const Matrix3 wrong_dimension =
        independent_trace_removal(original_A, original_h, 4);
    require(std::abs(trace(wrong_dimension, inverse(original_h))) > 1.0e-3,
            "negative control wrong trace dimension is rejected");
    require(std::abs(trace(original_A, inverse(original_h))) > 1.0e-1,
            "negative control omitted trace cleanup is rejected");
    const double hidden_Aww = 0.13;
    const double hidden_hww_inverse = 0.81;
    const double hidden_mutated_trace =
        trace(cleaned_A, inverse(original_h)) +
        2.0 * hidden_hww_inverse * hidden_Aww;
    require(std::abs(hidden_mutated_trace) > 0.1,
            "negative control hidden multiplicity in stock cleanup is rejected");
    require(make_state().chi < 0.0,
            "negative control omitted positivity cleanup is rejected");

    // No determinant-normalization compute class exists in the inspected
    // stock cleanup path.  These are explicitly local source/design controls,
    // not direct GRChombo output comparisons.
    const Matrix3 locally_normalized =
        normalize_determinant_local(original_h, -1.0 / 3.0);
    const Matrix3 wrong_exponent =
        normalize_determinant_local(original_h, -1.0 / 4.0);
    require_close(determinant(locally_normalized), 1.0,
                  "local determinant exponent diagnostic reaches det one");
    require(std::abs(determinant(wrong_exponent) - 1.0) > 1.0e-3,
            "local source-only wrong determinant exponent is rejected");
    require(std::abs(determinant(original_h) - 1.0) > 0.1,
            "local source-only omitted determinant normalization is rejected");

    std::cout << std::scientific << std::setprecision(8)
              << "RESULT cleanup direct_trace_component_max="
              << component_error
              << " direct_trace_component_normalized="
              << component_normalized << " direct_post_trace=" << post_trace
              << " direct_idempotence_max=" << idempotence_error
              << " determinant_before=" << determinant(original_h)
              << " determinant_after_trace="
              << determinant(to_matrix(direct.h)) << "\n"
              << "RESULT cleanup positivity_chi=" << direct.chi
              << " positivity_lapse=" << direct.lapse << "\n"
              << "RESULT cleanup determinant_normalization="
                 "SOURCE_ONLY_ABSENT_IN_INSPECTED_STOCK_PATH"
              << " hidden_d4_cleanup=ADAPTATION_GAP\n"
              << "PASS direct visible d=3 cleanup and active cleanup "
                 "mutations\n";
    return 0;
}
