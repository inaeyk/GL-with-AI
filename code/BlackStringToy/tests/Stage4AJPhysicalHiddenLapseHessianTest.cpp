#include "CartoonPhysicalHiddenLapseHessian.hpp"

#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include <type_traits>

namespace
{
namespace Lapse = BlackStringToy::CartoonPhysicalHiddenLapseHessian;

constexpr double tolerance = 1.0e-12;

// Stage 4AJ is the physical hidden lapse Hessian only. It uses gamma=h/chi,
// is local and away-axis only, does not apply the later source minus sign, and
// is not the full A_ww source, CCZ4 RHS, grid read, or evolution wiring.
struct Sample
{
    double x = 2.0;
    double A = 1.0;
    double B = 0.0;
    double C = 1.0;
    double W = 1.0;
    double chi = 1.0;
    double W_x = 0.0;
    double W_z = 0.0;
    double chi_x = 0.0;
    double chi_z = 0.0;
    double alpha_x = 0.0;
    double alpha_z = 0.0;
};

static_assert(!std::is_aggregate<
                  Lapse::PhysicalHiddenLapseHessianInputs>::value,
              "Stage 4AJ inputs must not be an open aggregate");
static_assert(!std::is_aggregate<Lapse::PhysicalHiddenLapseHessian>::value,
              "Stage 4AJ result must not be an open aggregate");
static_assert(!std::is_aggregate<
                  BlackStringToy::CartoonCheckedDxalphaOverX::
                      CheckedDxalphaOverXIngredient>::value,
              "checked alpha_x/x package must not be an open aggregate");
static_assert(Lapse::away_axis_only,
              "Stage 4AJ must remain away-axis only");
static_assert(Lapse::uses_physical_metric,
              "Stage 4AJ must use the physical metric");
static_assert(!Lapse::conformal_hessian_reused,
              "Stage 4AJ must not reuse the conformal hidden Hessian");
static_assert(!Lapse::source_minus_sign_applied,
              "Stage 4AK owns the -D_wD_w alpha source sign");
static_assert(!Lapse::full_Aww_source_implemented,
              "Stage 4AJ must not claim the full A_ww source");
static_assert(!Lapse::ccz4_rhs_implemented,
              "Stage 4AJ must not claim CCZ4 RHS implementation");
static_assert(!Lapse::evolution_wiring_implemented,
              "Stage 4AJ must not claim evolution wiring");

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void require_close(const std::string &label, const double value,
                   const double expected, const double tol = tolerance)
{
    const double residual = std::abs(value - expected);
    if (residual > tol)
    {
        fail(label, "got " + std::to_string(value) + ", expected " +
                        std::to_string(expected) + ", residual " +
                        std::to_string(residual));
    }
    std::cout << "PASS " << label << " = " << value
              << ", residual = " << residual << "\n";
}

void require_throws(const std::string &label, const std::function<void()> &fn)
{
    try
    {
        fn();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    fail(label, "expected std::domain_error");
}

Lapse::PhysicalHiddenLapseHessianInputs make_inputs(const Sample &sample)
{
    return Lapse::make_physical_hidden_lapse_hessian_inputs(
        sample.x, sample.A, sample.B, sample.C, sample.W, sample.chi,
        sample.W_x, sample.W_z, sample.chi_x, sample.chi_z,
        sample.alpha_x, sample.alpha_z);
}

double compute_dww_alpha(const Sample &sample)
{
    return Lapse::compute_physical_hidden_lapse_hessian(make_inputs(sample))
        .dww_alpha();
}

double direct_physical_formula(const Sample &sample)
{
    const double x2 = sample.x * sample.x;
    const double D = sample.A * sample.C - sample.B * sample.B;
    const double gamma_xx = sample.chi * sample.C / D;
    const double gamma_xz = -sample.chi * sample.B / D;
    const double gamma_zz = sample.chi * sample.A / D;

    const double d_x_x2_gamma_ww =
        2.0 * sample.x * sample.W / sample.chi +
        x2 * (sample.W_x / sample.chi -
              sample.W * sample.chi_x / (sample.chi * sample.chi));
    const double d_z_x2_gamma_ww =
        x2 * (sample.W_z / sample.chi -
              sample.W * sample.chi_z / (sample.chi * sample.chi));

    return (gamma_xx * d_x_x2_gamma_ww * sample.alpha_x +
            gamma_xz *
                (d_x_x2_gamma_ww * sample.alpha_z +
                 d_z_x2_gamma_ww * sample.alpha_x) +
            gamma_zz * d_z_x2_gamma_ww * sample.alpha_z) /
           (2.0 * x2);
}

void check_required_oracles()
{
    require_close("constant alpha physical D_wD_w alpha",
                  compute_dww_alpha(Sample{}), 0.0);

    Sample linear_x{};
    linear_x.alpha_x = 3.0 / 10.0;
    require_close("flat linear-x alpha physical D_wD_w alpha",
                  compute_dww_alpha(linear_x), 3.0 / 20.0);

    Sample linear_z{};
    linear_z.alpha_z = 0.7;
    require_close("flat linear-z alpha physical D_wD_w alpha",
                  compute_dww_alpha(linear_z), 0.0);

    Sample varying_chi{};
    varying_chi.chi = 6.0 / 5.0;
    varying_chi.chi_x = 1.0 / 10.0;
    varying_chi.alpha_x = 3.0 / 10.0;
    const double varying_chi_value = compute_dww_alpha(varying_chi);
    require_close("varying-chi physical D_wD_w alpha", varying_chi_value,
                  11.0 / 80.0);
    require_close("varying-chi direct physical formula", varying_chi_value,
                  direct_physical_formula(varying_chi));
    if (std::abs(varying_chi_value - 3.0 / 20.0) <= tolerance)
    {
        fail("varying-chi conformal reuse guard",
             "physical value matched the conformal-metric value");
    }
    std::cout << "PASS varying-chi differs from conformal value 0.15\n";

    Sample nonsymmetric{};
    nonsymmetric.A = 7.0;
    nonsymmetric.B = 2.0;
    nonsymmetric.C = 5.0;
    nonsymmetric.W = 3.0;
    nonsymmetric.chi = 5.0;
    nonsymmetric.W_x = 6.0;
    nonsymmetric.W_z = 4.0;
    nonsymmetric.chi_x = 3.0;
    nonsymmetric.chi_z = 2.0;
    nonsymmetric.alpha_x = 8.0;
    nonsymmetric.alpha_z = 7.0;
    const double nonsymmetric_value = compute_dww_alpha(nonsymmetric);
    require_close("nonsymmetric physical D_wD_w alpha", nonsymmetric_value,
                  699.0 / 155.0);
    require_close("nonsymmetric direct physical formula", nonsymmetric_value,
                  direct_physical_formula(nonsymmetric));
}

void check_invalid_inputs()
{
    Sample sample{};
    sample.x = 0.0;
    require_throws("invalid axis", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.x = -1.0;
    require_throws("negative axis", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.W = 0.0;
    require_throws("invalid W", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.chi = 0.0;
    require_throws("invalid chi", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.A = 1.0;
    sample.B = 2.0;
    sample.C = 1.0;
    require_throws("bad determinant", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.W_z = std::numeric_limits<double>::infinity();
    require_throws("nonfinite W_z", [&]() { make_inputs(sample); });

    sample = Sample{};
    sample.alpha_x = std::numeric_limits<double>::quiet_NaN();
    require_throws("nonfinite alpha_x", [&]() { make_inputs(sample); });
}

} // namespace

int main()
{
    std::cout << "Stage 4AJ physical hidden lapse Hessian fixture\n";
    std::cout << "Physical D_wD_w alpha only; no source minus sign, full "
                 "A_ww source, RHS, grid reads, finite-axis support, or "
                 "evolution.\n";

    check_required_oracles();
    check_invalid_inputs();

    std::cout << "All Stage 4AJ physical hidden lapse Hessian checks passed.\n";
    return 0;
}
