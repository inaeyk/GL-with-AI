#include "Stage4AOFrozenGaugeOperator.hpp"
#include "Stage4AOFrozenGaugeZRicciCompletion.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace
{
namespace Completion =
    BlackStringToy::Stage4AOFrozenGaugeZRicciCompletion;
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Input = Completion::EncodedZDerivativeJet;
using Result = Completion::CompletionTensor;

constexpr double tolerance = 1.0e-12;

struct AnalyticCartoonOracle
{
    double q_xx;
    double q_xz;
    double q_zz;
    double q_ww;
    double trace;
    double tf_xx;
    double tf_xz;
    double tf_zz;
    double tf_ww;
};

void fail(const std::string &label, const std::string &details)
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
                   const double expected,
                   const double allowed_error = tolerance)
{
    const double error = std::abs(actual - expected);
    if (error > allowed_error)
    {
        fail(label, "actual=" + std::to_string(actual) +
                        " expected=" + std::to_string(expected) +
                        " error=" + std::to_string(error));
    }
    std::cout << "PASS " << label << "\n";
}

void require_separated(const std::string &label, const double actual,
                       const double mutation)
{
    if (std::abs(actual - mutation) <= 1.0e-3)
    {
        fail(label, "mutation was not separated from the actual result");
    }
    std::cout << "PASS " << label << "\n";
}

template <class Function>
void require_invalid_argument(const std::string &label, Function &&function)
{
    try
    {
        function();
    }
    catch (const std::invalid_argument &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    catch (const std::exception &exception)
    {
        fail(label, std::string("wrong exception: ") + exception.what());
    }
    fail(label, "expected std::invalid_argument");
}

AnalyticCartoonOracle analytic_modified_cartoon_oracle(
    const double x, const double z_x, const double dx_z_x,
    const double dz_z_x, const double dx_z_z, const double dz_z_z)
{
    // At theta=pi/2 in two hidden spherical directions,
    // gamma_x(theta theta)=gamma_x(phi phi)=-x.  Hence
    // 2 D_theta Z_theta=2 x Z_x and the representative orthonormal hidden
    // component is (2 x Z_x)/x^2.  The two angular directions enter only
    // when the scalar trace is formed.
    const double q_xx = 2.0 * dx_z_x;
    const double q_xz = dx_z_z + dz_z_x;
    const double q_zz = 2.0 * dz_z_z;
    const double q_theta_theta = 2.0 * x * z_x;
    const double q_phi_phi = 2.0 * x * z_x;
    const double q_ww = q_theta_theta / (x * x);
    const double trace = q_xx + q_zz + q_theta_theta / (x * x) +
                         q_phi_phi / (x * x);
    const double trace_share = trace / 4.0;
    return {q_xx, q_xz, q_zz, q_ww, trace, q_xx - trace_share,
            q_xz, q_zz - trace_share, q_ww - trace_share};
}

Result compute(const double x, const double z_x, const double z_z,
               const double dx_z_x, const double dz_z_x,
               const double dx_z_z, const double dz_z_z)
{
    return Completion::compute(Completion::make_encoded_z_derivative_jet(
        x, z_x, z_z, dx_z_x, dz_z_x, dx_z_z, dz_z_z));
}

void check_scope_contract()
{
    using Function = decltype(&Completion::compute);
    static_assert(!std::is_aggregate<Input>::value,
                  "encoded-Z input must be checked and opaque");
    static_assert(!std::is_aggregate<Result>::value,
                  "encoded-Z result must be read-only and opaque");
    static_assert(std::is_invocable<Function, const Input &>::value,
                  "helper accepts only a checked Z derivative jet");
    static_assert(Completion::physical_spatial_dimension == 4,
                  "the trace-free projection is four-dimensional");
    static_assert(Completion::hidden_ww_multiplicity == 2,
                  "the scalar trace has two hidden directions");

    require_true("helper is validation only", Completion::validation_only);
    require_true("linearized encoded-Z completion is implemented",
                 Completion::
                     linearized_encoded_z_ricci_completion_implemented);
    require_true("geometric Ricci is not part of this helper",
                 !Completion::geometric_ricci_implemented);
    require_true("Z definition derivatives are not expanded",
                 !Completion::z_definition_derivative_expansion_implemented);
    require_true("helper itself performs no K insertion",
                 !Completion::k_row_insertion_implemented);
    require_true("helper itself performs no Theta insertion",
                 !Completion::theta_row_insertion_implemented);
    require_true("helper itself performs no A insertion",
                 !Completion::a_row_insertion_implemented);
    require_true("separate operator insertion block is implemented",
                 Operator::
                     encoded_z_ricci_completion_insertion_block_implemented);
    require_true("non-Gamma completion remains false",
                 !Completion::complete_non_gamma_rhs_implemented);
    require_true("full operator gate remains false",
                 !Completion::full_frozen_gauge_operator_implemented &&
                     !Operator::complete_frozen_gauge_operator_implemented);
    require_true("eigensolver gate remains false",
                 !Completion::eigensolver_implemented &&
                     !Operator::eigensolver_implemented);
    require_true("nonlinear selected-branch oracle is deferred",
                 !Completion::
                     nonlinear_selected_branch_oracle_implemented);
}

void check_components_trace_and_projection()
{
    const double x = 2.0;
    const double z_x = 3.0;
    const double z_z = 5.0;
    const double dx_z_x = 7.0;
    const double dz_z_x = 11.0;
    const double dx_z_z = 13.0;
    const double dz_z_z = 17.0;
    const auto result =
        compute(x, z_x, z_z, dx_z_x, dz_z_x, dx_z_z, dz_z_z);
    const auto oracle = analytic_modified_cartoon_oracle(
        x, z_x, dx_z_x, dz_z_x, dx_z_z, dz_z_z);

    require_close("q_xx component", result.q_xx(), oracle.q_xx);
    require_close("q_xz component", result.q_xz(), oracle.q_xz);
    require_close("q_zz component", result.q_zz(), oracle.q_zz);
    require_close("representative q_ww component", result.q_ww(),
                  oracle.q_ww);
    require_close("two-copy scalar trace", result.scalar_trace(),
                  oracle.trace);
    require_close("four-dimensional TF xx", result.tf_xx(), oracle.tf_xx);
    require_close("four-dimensional TF xz", result.tf_xz(), oracle.tf_xz);
    require_close("four-dimensional TF zz", result.tf_zz(), oracle.tf_zz);
    require_close("four-dimensional TF ww", result.tf_ww(), oracle.tf_ww);
    require_close("TF weighted trace vanishes",
                  Completion::trace_free_residual(result), 0.0);

    const double one_hidden_copy =
        result.q_xx() + result.q_zz() + result.q_ww();
    const double doubled_representative_output = 2.0 * result.q_ww();
    const double wrong_hidden_sign = -2.0 * z_x / x;
    const double wrong_hidden_coefficient = z_x / x;
    require_separated("wrong hidden trace multiplicity rejected",
                      result.scalar_trace(), one_hidden_copy);
    require_separated("representative ww output is not doubled",
                      result.q_ww(), doubled_representative_output);
    require_separated("wrong 1/x sign rejected", result.q_ww(),
                      wrong_hidden_sign);
    require_separated("wrong 1/x coefficient rejected", result.q_ww(),
                      wrong_hidden_coefficient);

    const double omitted_projection = result.q_xx();
    const double doubled_projection =
        result.q_xx() - 2.0 * result.scalar_trace() / 4.0;
    require_separated("omitted TF projection rejected", result.tf_xx(),
                      omitted_projection);
    require_separated("double TF projection rejected", result.tf_xx(),
                      doubled_projection);
}

void check_zero_jet()
{
    const auto zero = compute(2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    require_close("zero q_xx", zero.q_xx(), 0.0);
    require_close("zero q_xz", zero.q_xz(), 0.0);
    require_close("zero q_zz", zero.q_zz(), 0.0);
    require_close("zero q_ww", zero.q_ww(), 0.0);
    require_close("zero scalar trace", zero.scalar_trace(), 0.0);
    require_close("zero TF xx", zero.tf_xx(), 0.0);
    require_close("zero TF xz", zero.tf_xz(), 0.0);
    require_close("zero TF zz", zero.tf_zz(), 0.0);
    require_close("zero TF ww", zero.tf_ww(), 0.0);
}

void check_parity()
{
    const double x = 1.7;
    const double z = 0.31;
    const double wave_number = 3.0;
    const auto mode = [x, wave_number](const double coordinate) {
        const double cosine = std::cos(wave_number * coordinate);
        const double sine = std::sin(wave_number * coordinate);
        const double z_x = x * x * cosine;
        const double z_z = x * sine;
        const double dx_z_x = 2.0 * x * cosine;
        const double dz_z_x = -wave_number * x * x * sine;
        const double dx_z_z = sine;
        const double dz_z_z = wave_number * x * cosine;
        return compute(x, z_x, z_z, dx_z_x, dz_z_x, dx_z_z, dz_z_z);
    };

    const auto plus = mode(z);
    const auto minus = mode(-z);
    require_close("xx/even parity", plus.q_xx(), minus.q_xx());
    require_close("zz/even parity", plus.q_zz(), minus.q_zz());
    require_close("ww/even parity", plus.q_ww(), minus.q_ww());
    require_close("trace/even parity", plus.scalar_trace(),
                  minus.scalar_trace());
    require_close("xz/one-z-index odd parity", plus.q_xz(),
                  -minus.q_xz());
    require_close("TF xx/even parity", plus.tf_xx(), minus.tf_xx());
    require_close("TF xz/one-z-index odd parity", plus.tf_xz(),
                  -minus.tf_xz());
}

void check_input_guards()
{
    require_invalid_argument("x=0", []() {
        (void)Completion::make_encoded_z_derivative_jet(
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    });
    require_invalid_argument("non-finite derivative", []() {
        (void)Completion::make_encoded_z_derivative_jet(
            1.0, 0.0, 0.0, std::numeric_limits<double>::quiet_NaN(), 0.0,
            0.0, 0.0);
    });
}
} // namespace

int main()
{
    check_scope_contract();
    check_components_trace_and_projection();
    check_zero_jet();
    check_parity();
    check_input_guards();
    std::cout
        << "PASS Stage 4AO-C encoded-Z Ricci completion tensor helper\n";
    return 0;
}
