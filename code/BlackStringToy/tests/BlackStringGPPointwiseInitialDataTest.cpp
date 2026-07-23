#include "BlackStringGPPointwiseInitialData.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#ifdef USERVARIABLES_HPP
#error "GP pointwise test must not import the legacy 27-slot UserVariables.hpp"
#endif

namespace
{
namespace GP = BlackStringGPPointwiseInitialData;
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

using Storage = Reduced::Storage<double>;

constexpr double absolute_tolerance = 2.0e-14;
constexpr double relative_tolerance = 2.0e-13;

[[noreturn]] void fail(const std::string &message)
{
    std::cerr << "FAIL " << message << "\n";
    std::exit(1);
}

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        fail(message);
    }
}

void require_close(const double actual, const double expected,
                   const std::string &label,
                   const double absolute = absolute_tolerance,
                   const double relative = relative_tolerance)
{
    const double error = std::abs(actual - expected);
    const double allowance =
        absolute + relative * std::max(std::abs(actual), std::abs(expected));
    if (!(error <= allowance))
    {
        fail(label + " actual=" + std::to_string(actual) +
             " expected=" + std::to_string(expected) +
             " error=" + std::to_string(error));
    }
}

Storage independent_expected_state(const double r0, const double x)
{
    const double beta_x = std::sqrt(r0) * std::pow(x, -0.5);
    const double lambda = std::sqrt(r0) * std::pow(x, -1.5);
    Storage expected{};
    expected[Production::c_chi] = 1.0;
    expected[Production::c_hxx] = 1.0;
    expected[Production::c_hxz] = 0.0;
    expected[Production::c_hzz] = 1.0;
    expected[Production::c_hww] = 1.0;
    expected[Production::c_K] = 3.0 * lambda / 2.0;
    expected[Production::c_Axx] = -7.0 * lambda / 8.0;
    expected[Production::c_Axz] = 0.0;
    expected[Production::c_Azz] = -3.0 * lambda / 8.0;
    expected[Production::c_Aww] = 5.0 * lambda / 8.0;
    expected[Production::c_Theta] = 0.0;
    expected[Production::c_GammaX] = 0.0;
    expected[Production::c_GammaZ] = 0.0;
    expected[Production::c_lapse] = 1.0;
    expected[Production::c_shiftX] = beta_x;
    expected[Production::c_shiftZ] = 0.0;
    expected[Production::c_Bx] = 0.0;
    expected[Production::c_Bz] = 0.0;
    return expected;
}

bool state_matches(const Storage &actual, const Storage &expected)
{
    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        const std::size_t index = static_cast<std::size_t>(slot);
        const double error = std::abs(actual[index] - expected[index]);
        const double allowance =
            absolute_tolerance +
            relative_tolerance *
                std::max(std::abs(actual[index]), std::abs(expected[index]));
        if (!(error <= allowance))
        {
            return false;
        }
    }
    return true;
}

void check_case(const double r0, const double x)
{
    const Storage state = GP::make_pointwise_state(r0, x);
    const Storage expected = independent_expected_state(r0, x);
    require(state_matches(state, expected),
            "all 18 GP fields at r0=" + std::to_string(r0) +
                " x=" + std::to_string(x));

    const auto vars = Reduced::load(state);
    const auto analytic = GP::make_analytic_metadata(r0, x);
    const double lambda = analytic.lambda.value;

    const double conformal_determinant =
        vars.physical.h.xx * vars.physical.h.zz *
        vars.physical.h.ww * vars.physical.h.ww;
    const double weighted_trace =
        Reduced::physical_diagonal_trace(vars.physical.A);
    require_close(conformal_determinant, 1.0,
                  "GP conformal determinant");
    require_close(weighted_trace, 0.0, "GP weighted A trace");

    require_close(vars.physical.A.xx + 0.25 * vars.physical.K,
                  -0.5 * lambda, "GP K_xx reconstruction");
    require_close(vars.physical.A.zz + 0.25 * vars.physical.K, 0.0,
                  "GP K_zz reconstruction");
    require_close(vars.physical.A.ww + 0.25 * vars.physical.K, lambda,
                  "GP K_ww reconstruction");

    require_close(vars.physical.Gamma.x, 0.0, "GP GammaX");
    require_close(vars.physical.Gamma.z, 0.0, "GP GammaZ");
    require_close(vars.gauge.lapse, 1.0, "GP lapse");
    require_close(vars.gauge.shift.x, std::sqrt(r0 / x), "GP betaX");
    require_close(vars.gauge.shift.z, 0.0, "GP betaZ");
    require_close(vars.gauge.B.x, 0.0, "GP Bx");
    require_close(vars.gauge.B.z, 0.0, "GP Bz");

    require_close(GP::gamma_theta_theta(x), x * x,
                  "GP gamma_theta_theta");
    require_close(GP::gamma_ww(vars), 1.0, "GP gamma_ww");

    std::cout << "CASE r0=" << r0 << " x=" << x
              << " lambda=" << lambda << " betaX=" << vars.gauge.shift.x
              << " det_h=" << conformal_determinant
              << " tr_A=" << weighted_trace << "\n";
}

double independent_beta(const double r0, const double x)
{
    return std::sqrt(r0) * std::pow(x, -0.5);
}

double independent_lambda(const double r0, const double x)
{
    return std::sqrt(r0) * std::pow(x, -1.5);
}

template <class function_t>
double central_first(function_t function, const double x, const double h)
{
    return (function(x + h) - function(x - h)) / (2.0 * h);
}

template <class function_t>
double central_second(function_t function, const double x, const double h)
{
    return (function(x + h) - 2.0 * function(x) + function(x - h)) /
           (h * h);
}

struct DifferenceErrors
{
    double first;
    double second;
};

template <class function_t>
DifferenceErrors derivative_errors(function_t function, const double x,
                                   const double h, const GP::RadialJet &jet)
{
    return {std::abs(central_first(function, x, h) - jet.dx),
            std::abs(central_second(function, x, h) - jet.dxx)};
}

void check_second_order_window(
    const std::array<DifferenceErrors, 8> &errors, const std::string &label)
{
    for (std::size_t level = 1; level < 4; ++level)
    {
        const double first_order =
            std::log(errors[level - 1].first / errors[level].first) /
            std::log(2.0);
        const double second_order =
            std::log(errors[level - 1].second / errors[level].second) /
            std::log(2.0);
        require(first_order > 1.8 && second_order > 1.8,
                label + " central-difference convergence order");
    }

    double minimum_second = errors[0].second;
    for (const DifferenceErrors &entry : errors)
    {
        minimum_second = std::min(minimum_second, entry.second);
    }
    require(errors.back().second > minimum_second,
            label + " fine-epsilon second derivative must show roundoff");
}

template <class function_t>
void check_jet_against_differences(const std::string &label,
                                   function_t function, const double x,
                                   const GP::RadialJet &jet)
{
    const std::array<double, 8> relative_steps = {
        8.0e-2, 4.0e-2, 2.0e-2, 1.0e-2,
        1.0e-3, 1.0e-4, 1.0e-5, 1.0e-7};
    std::array<DifferenceErrors, relative_steps.size()> errors{};
    for (std::size_t level = 0; level < relative_steps.size(); ++level)
    {
        const double h = relative_steps[level] * x;
        errors[level] = derivative_errors(function, x, h, jet);
        std::cout << "FD " << label << " h=" << h
                  << " err_dx=" << errors[level].first
                  << " err_dxx=" << errors[level].second << "\n";
    }
    check_second_order_window(errors, label);

    const double comparison_h = 1.0e-4 * x;
    require_close(central_first(function, x, comparison_h), jet.dx,
                  label + " finite-difference dx", 2.0e-8, 2.0e-8);
    require_close(central_second(function, x, comparison_h), jet.dxx,
                  label + " finite-difference dxx", 2.0e-7, 2.0e-7);
}

void check_analytic_derivatives()
{
    constexpr double r0 = 1.3;
    constexpr double x = 2.1;
    const auto metadata = GP::make_analytic_metadata(r0, x);

    const auto beta_function =
        [r0](const double radius) { return independent_beta(r0, radius); };
    const auto lambda_function =
        [r0](const double radius) { return independent_lambda(r0, radius); };
    check_jet_against_differences("betaX", beta_function, x, metadata.beta_x);
    check_jet_against_differences("lambda", lambda_function, x,
                                  metadata.lambda);

    const auto K_function = [lambda_function](const double radius) {
        return 1.5 * lambda_function(radius);
    };
    const auto Axx_function = [lambda_function](const double radius) {
        return -0.875 * lambda_function(radius);
    };
    const auto Azz_function = [lambda_function](const double radius) {
        return -0.375 * lambda_function(radius);
    };
    const auto Aww_function = [lambda_function](const double radius) {
        return 0.625 * lambda_function(radius);
    };
    check_jet_against_differences("K", K_function, x, metadata.K);
    check_jet_against_differences("Axx", Axx_function, x, metadata.A.xx);
    check_jet_against_differences("Azz", Azz_function, x, metadata.A.zz);
    check_jet_against_differences("Aww", Aww_function, x, metadata.A.ww);
    require(metadata.A.xz.value == 0.0 && metadata.A.xz.dx == 0.0 &&
                metadata.A.xz.dxx == 0.0,
            "Axz analytic jet must vanish");
}

template <class function_t>
void require_domain_error(function_t function, const std::string &label)
{
    bool rejected = false;
    try
    {
        function();
    }
    catch (const std::domain_error &)
    {
        rejected = true;
    }
    require(rejected, label + " must be rejected");
}

void check_input_rejections()
{
    require_domain_error([] { GP::make_pointwise_state(0.0, 2.0); },
                         "r0=0");
    require_domain_error([] { GP::make_pointwise_state(-1.0, 2.0); },
                         "r0<0");
    require_domain_error([] { GP::make_pointwise_state(1.0, 0.0); }, "x=0");
    require_domain_error([] { GP::make_pointwise_state(1.0, -2.0); },
                         "x<0");
    require_domain_error(
        [] {
            GP::make_pointwise_state(
                std::numeric_limits<double>::infinity(), 2.0);
        },
        "nonfinite r0");
    require_domain_error(
        [] {
            GP::make_pointwise_state(
                1.0, std::numeric_limits<double>::quiet_NaN());
        },
        "nonfinite x");
}

void require_mutated_state_rejected(const Storage &mutated, const double r0,
                                    const double x, const std::string &label)
{
    require(!state_matches(mutated, independent_expected_state(r0, x)),
            label + " mutation was accepted");
    std::cout << "PASS mutation rejected: " << label << "\n";
}

void check_mutations()
{
    constexpr double r0 = 1.0;
    constexpr double x = 2.0;
    const Storage correct = GP::make_pointwise_state(r0, x);

    {
        Storage mutation = correct;
        mutation[Production::c_hww] = x * x;
        require_mutated_state_rejected(mutation, r0, x, "hww=x^2");
    }
    {
        Storage mutation = correct;
        mutation[Production::c_K] = -mutation[Production::c_K];
        require_mutated_state_rejected(mutation, r0, x, "wrong K sign");
    }
    {
        Storage mutation = correct;
        mutation[Production::c_Aww] *= -1.0;
        require_mutated_state_rejected(mutation, r0, x, "wrong Aww");
    }
    {
        const auto vars = Reduced::load(correct);
        const double wrong_trace = vars.physical.A.xx + vars.physical.A.zz +
                                   vars.physical.A.ww;
        require(std::abs(wrong_trace) > 1.0e-6,
                "hidden multiplicity-one trace must fail");
        std::cout
            << "PASS mutation rejected: hidden multiplicity one in trace\n";
    }
    {
        Storage mutation = correct;
        mutation[Production::c_lapse] =
            std::numeric_limits<double>::quiet_NaN();
        require_mutated_state_rejected(mutation, r0, x,
                                       "missing gauge initialization");
    }
    {
        const double mock_evolved_K = 0.37;
        Storage mutation = correct;
        mutation[Production::c_K] += mock_evolved_K;
        require_mutated_state_rejected(mutation, r0, x,
                                       "field-dependent initializer");
        require(state_matches(GP::make_pointwise_state(r0, x), correct),
                "correct initializer must ignore external evolved fields");
    }
    {
        constexpr double mock_horizon_radius = 1.4;
        const Storage mutation =
            GP::make_pointwise_state(mock_horizon_radius, x);
        require_mutated_state_rejected(mutation, r0, x,
                                       "horizon-dependent initializer");
        require(state_matches(GP::make_pointwise_state(r0, x), correct),
                "correct initializer must use locked r0 only");
    }
}

} // namespace

int main()
{
    std::cout << "Black-string exact GP pointwise initializer fixture\n";
    check_case(1.0, 2.0);
    check_case(2.5, 1.25);
    check_case(0.7, 3.4);
    check_analytic_derivatives();
    check_input_rejections();
    check_mutations();
    std::cout << "All GP pointwise initialization checks passed.\n";
    return 0;
}
