#include "Stage4AOGPDiscretePreflight.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
namespace Preflight = BlackStringToy::Stage4AOGPDiscretePreflight;

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

void require_less(const std::string &label, const double value,
                  const double bound)
{
    if (!(value < bound))
    {
        fail(label, "got " + std::to_string(value) + ", bound " +
                        std::to_string(bound));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void require_greater(const std::string &label, const double value,
                     const double bound)
{
    if (!(value > bound))
    {
        fail(label, "got " + std::to_string(value) + ", bound " +
                        std::to_string(bound));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void check_raw_background_residual_convergence()
{
    // Stage 4AO-B raw residual preflight uses the unmodified discrete RHS.
    // It does not apply the Stage 4AO-D fixed GP-holding source S_alpha.
    const auto coarse = Preflight::compute_raw_background_residual_sample(256);
    const auto medium = Preflight::compute_raw_background_residual_sample(512);
    const auto fine = Preflight::compute_raw_background_residual_sample(1024);

    require_true("provisional domain has x_in > 0", Preflight::x_in > 0.0);
    require_true("provisional domain brackets r0",
                 Preflight::x_in < Preflight::r0 &&
                     Preflight::r0 < Preflight::x_out);
    require_less("raw lapse target residual coarse",
                 coarse.max_lapse_target_error, 1.0e-12);
    require_less("raw lapse target residual medium",
                 medium.max_lapse_target_error, 1.0e-12);
    require_less("raw lapse target residual fine",
                 fine.max_lapse_target_error, 1.0e-12);

    const double ratio_cm =
        coarse.max_raw_residual_error / medium.max_raw_residual_error;
    const double ratio_mf =
        medium.max_raw_residual_error / fine.max_raw_residual_error;
    std::cout << "INFO raw residual errors: " << coarse.max_raw_residual_error
              << ", " << medium.max_raw_residual_error << ", "
              << fine.max_raw_residual_error << "\n";
    require_greater("raw residual coarse/medium convergence", ratio_cm, 3.4);
    require_greater("raw residual medium/fine convergence", ratio_mf, 3.4);
    require_less("fine raw hat_Gamma^x residual",
                 fine.max_hat_gamma_x_error, 1.0e-3);
}

void check_delta_hww_hidden_contraction()
{
    // This isolates only the hidden-contraction response to delta h_ww. The
    // expected value includes the two hidden directions and the analytic
    // away-axis 1/x cancellation pattern from Stage 4AM/4AO-A.
    const double error =
        Preflight::max_delta_hww_hidden_contraction_error(128, 1.0e-6);
    require_less("delta hww hidden-contraction oracle", error, 1.0e-14);

    const double x = 2.0;
    const double amplitude = 1.0e-6;
    const double W = 1.0 + amplitude * x * x;
    const double W_x = 2.0 * amplitude * x;
    const double multiplicity_two =
        Preflight::hidden_contraction_from_hww(x, W, W_x);
    const double multiplicity_one = 0.5 * multiplicity_two;
    const double expected =
        -4.0 * amplitude * x / (1.0 + amplitude * x * x);
    require_less("hidden multiplicity-two residual",
                 std::abs(multiplicity_two - expected), 1.0e-15);
    require_greater("hidden multiplicity-one negative check",
                    std::abs(multiplicity_one - expected), 1.0e-6);
}

void check_jacobian_plateau()
{
    // The finite-difference Jacobian-vector product uses the unmodified local
    // RHS harness. The large epsilon shows nonlinear contamination, the middle
    // epsilons form the useful plateau, and the tiny epsilon shows roundoff.
    const auto plateau = Preflight::compute_jacobian_plateau(128);
    std::cout << "INFO JVP errors large/midA/midB/small: "
              << plateau.error_large_epsilon << ", "
              << plateau.error_middle_epsilon_a << ", "
              << plateau.error_middle_epsilon_b << ", "
              << plateau.error_small_epsilon << "\n";
    require_less("JVP middle epsilon A", plateau.error_middle_epsilon_a,
                 2.0e-5);
    require_less("JVP middle epsilon B", plateau.error_middle_epsilon_b,
                 2.0e-5);
    require_greater("JVP large epsilon nonlinear signal",
                    plateau.error_large_epsilon,
                    10.0 * plateau.error_middle_epsilon_a);
    require_greater("JVP small epsilon roundoff signal",
                    plateau.error_small_epsilon,
                    plateau.error_middle_epsilon_b);
}

void check_parity_block_diagonalization()
{
    // Stage 4AO-B explicitly checks the scalar GL parity sectors under
    // z -> -z. In the even sector, scalar/zero-z/two-z variables
    // (chi, h_xx, h_zz, h_ww, K, A_xx, A_zz, A_ww, Theta, alpha, beta^x,
    // B^x, hat_Gamma^x) use cos(kz), while one-z variables
    // (h_xz, A_xz, beta^z, B^z, hat_Gamma^z) use sin(kz). The odd sector
    // swaps those assignments.
    //
    // The representative sub-operator uses periodic finite-difference D_z and
    // D_zz stencils. It includes h_xz-like output receiving D_z beta^x,
    // hat_Gamma^z/momentum-z-like output receiving D_z K, and scalar output
    // receiving D_z beta^z. This is not the Stage 4AO-C spectral gate.
    const auto leakage = Preflight::compute_z_coupled_parity_leakage(64);
    std::cout << "INFO parity allowed/leakage even: "
              << leakage.even_allowed_norm << ", "
              << leakage.even_forbidden_to_allowed << "\n";
    std::cout << "INFO parity allowed/leakage odd: "
              << leakage.odd_allowed_norm << ", "
              << leakage.odd_forbidden_to_allowed << "\n";
    std::cout << "INFO parity negative guard leakage: "
              << leakage.negative_guard_forbidden_to_allowed << "\n";

    require_greater("even-sector allowed parity output",
                    leakage.even_allowed_norm, 1.0e-1);
    require_less("even-sector forbidden parity leakage",
                 leakage.even_forbidden_to_allowed, 1.0e-13);
    require_greater("odd-sector allowed parity output",
                    leakage.odd_allowed_norm, 1.0e-1);
    require_less("odd-sector forbidden parity leakage",
                 leakage.odd_forbidden_to_allowed, 1.0e-13);
    require_greater("parity negative guard detects flipped beta^z",
                    leakage.negative_guard_forbidden_to_allowed, 1.0e-1);
}

} // namespace

int main()
{
    check_raw_background_residual_convergence();
    check_delta_hww_hidden_contraction();
    check_jacobian_plateau();
    check_parity_block_diagonalization();
    std::cout << "PASS Stage 4AO-B discrete operator preflight\n";
    return 0;
}
