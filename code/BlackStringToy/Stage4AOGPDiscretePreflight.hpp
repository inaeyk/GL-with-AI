#ifndef BLACKSTRINGTOY_STAGE4AO_GP_DISCRETE_PREFLIGHT_HPP
#define BLACKSTRINGTOY_STAGE4AO_GP_DISCRETE_PREFLIGHT_HPP

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace BlackStringToy
{
namespace Stage4AOGPDiscretePreflight
{
// Stage 4AO-B local validation harness for the locked Stage 4AO-A uniform
// ingoing-GP background. This is not production RHS wiring, not an eigensolver,
// and not a Stage 4AO-C threshold/growth-rate search.
static constexpr double r0 = 1.0;
static constexpr double x_in = 0.5;
static constexpr double x_out = 4.0;

struct RadialGrid
{
    int intervals;
    double dx;
    std::vector<double> x;
};

struct RawResidualSample
{
    int intervals;
    double dx;
    double max_raw_residual_error;
    double max_lapse_target_error;
    double max_hat_gamma_x_error;
};

struct JacobianPlateau
{
    double error_large_epsilon;
    double error_middle_epsilon_a;
    double error_middle_epsilon_b;
    double error_small_epsilon;
};

struct ParityLeakage
{
    double even_allowed_norm;
    double even_forbidden_norm;
    double even_forbidden_to_allowed;
    double odd_allowed_norm;
    double odd_forbidden_norm;
    double odd_forbidden_to_allowed;
    double negative_guard_forbidden_to_allowed;
};

inline double lambda(const double x)
{
    return std::sqrt(r0 / (x * x * x));
}

inline double beta_x(const double x) { return x * lambda(x); }
inline double K_trace(const double x) { return 1.5 * lambda(x); }
inline double A_xx(const double x) { return -0.875 * lambda(x); }
inline double A_zz(const double x) { return -0.375 * lambda(x); }
inline double A_ww(const double x) { return 0.625 * lambda(x); }

inline RadialGrid make_radial_grid(const int intervals)
{
    if (intervals < 16)
    {
        throw std::domain_error("Stage 4AO-B grid requires at least 16 intervals");
    }
    RadialGrid grid;
    grid.intervals = intervals;
    grid.dx = (x_out - x_in) / static_cast<double>(intervals);
    grid.x.reserve(static_cast<std::size_t>(intervals + 1));
    for (int i = 0; i <= intervals; ++i)
    {
        grid.x.push_back(x_in + grid.dx * static_cast<double>(i));
    }
    return grid;
}

inline std::vector<double> values_on_grid(const RadialGrid &grid,
                                          double (*function)(double))
{
    std::vector<double> values;
    values.reserve(grid.x.size());
    for (const double x : grid.x)
    {
        values.push_back(function(x));
    }
    return values;
}

inline double d1(const std::vector<double> &values, const RadialGrid &grid,
                 const int i)
{
    return (values[static_cast<std::size_t>(i + 1)] -
            values[static_cast<std::size_t>(i - 1)]) /
           (2.0 * grid.dx);
}

inline double d2(const std::vector<double> &values, const RadialGrid &grid,
                 const int i)
{
    return (values[static_cast<std::size_t>(i + 1)] -
            2.0 * values[static_cast<std::size_t>(i)] +
            values[static_cast<std::size_t>(i - 1)]) /
           (grid.dx * grid.dx);
}

inline double abs_max(const double current, const double candidate)
{
    const double value = std::abs(candidate);
    return value > current ? value : current;
}

inline double divergence_beta_at(const std::vector<double> &beta,
                                 const RadialGrid &grid, const int i)
{
    const double x = grid.x[static_cast<std::size_t>(i)];
    return d1(beta, grid, i) + 2.0 * beta[static_cast<std::size_t>(i)] / x;
}

inline RawResidualSample compute_raw_background_residual_sample(
    const int intervals)
{
    const RadialGrid grid = make_radial_grid(intervals);
    const auto beta = values_on_grid(grid, beta_x);
    const auto K = values_on_grid(grid, K_trace);
    const auto Axx = values_on_grid(grid, A_xx);
    const auto Azz = values_on_grid(grid, A_zz);
    const auto Aww = values_on_grid(grid, A_ww);

    std::vector<double> div(beta.size(), 0.0);
    for (int i = 1; i < intervals; ++i)
    {
        div[static_cast<std::size_t>(i)] = divergence_beta_at(beta, grid, i);
    }

    double max_error = 0.0;
    double max_lapse_error = 0.0;
    double max_gamma_error = 0.0;
    for (int i = 2; i <= intervals - 2; ++i)
    {
        const std::size_t idx = static_cast<std::size_t>(i);
        const double x = grid.x[idx];
        const double lam = lambda(x);
        const double beta_i = beta[idx];
        const double d_beta = d1(beta, grid, i);
        const double d2_beta = d2(beta, grid, i);
        const double d_K = d1(K, grid, i);
        const double d_Axx = d1(Axx, grid, i);
        const double d_Azz = d1(Azz, grid, i);
        const double d_Aww = d1(Aww, grid, i);
        const double d_div = d1(div, grid, i);

        const double rhs_chi = 0.5 * (K[idx] - div[idx]);
        const double rhs_hxx = -2.0 * Axx[idx] - 0.5 * div[idx] + 2.0 * d_beta;
        const double rhs_hzz = -2.0 * Azz[idx] - 0.5 * div[idx];
        const double rhs_hww =
            -2.0 * Aww[idx] - 0.5 * div[idx] + 2.0 * beta_i / x;
        const double rhs_hxz = 0.0;

        const double rhs_K = beta_i * d_K + K[idx] * K[idx];
        const double tr_A2 = Axx[idx] * Axx[idx] + Azz[idx] * Azz[idx] +
                             2.0 * Aww[idx] * Aww[idx];
        const double rhs_Theta =
            0.5 * (-tr_A2 + 0.75 * K[idx] * K[idx]);

        const double trace_factor = K[idx] - 0.5 * div[idx];
        const double rhs_Axx =
            beta_i * d_Axx + Axx[idx] * trace_factor +
            2.0 * Axx[idx] * d_beta - 2.0 * Axx[idx] * Axx[idx];
        const double rhs_Azz =
            beta_i * d_Azz + Azz[idx] * trace_factor -
            2.0 * Azz[idx] * Azz[idx];
        const double rhs_Aww =
            beta_i * d_Aww + Aww[idx] * trace_factor +
            2.0 * Aww[idx] * beta_i / x - 2.0 * Aww[idx] * Aww[idx];
        const double rhs_Axz = 0.0;

        const double hamiltonian =
            -tr_A2 + 0.75 * K[idx] * K[idx];
        const double momentum_x =
            d_Axx + 2.0 * (Axx[idx] - Aww[idx]) / x - 0.75 * d_K;
        const double momentum_z = 0.0;
        const double det_h_minus_one = 0.0;
        const double tr_A = Axx[idx] + Azz[idx] + 2.0 * Aww[idx];

        const double hidden_second_beta = d_beta / x - beta_i / (x * x);
        const double rhs_hat_gamma_x =
            -1.5 * d_K + d2_beta + 2.0 * hidden_second_beta + 0.5 * d_div;
        const double rhs_hat_gamma_z = 0.0;

        const double rhs_lapse = -2.0 * K[idx];
        const double lapse_target = -3.0 * lam;
        const double rhs_shift_x = 0.0;
        const double rhs_shift_z = 0.0;
        const double rhs_Bx = rhs_hat_gamma_x;
        const double rhs_Bz = rhs_hat_gamma_z;

        const double components[] = {
            rhs_chi,     rhs_hxx,       rhs_hzz,    rhs_hww,
            rhs_hxz,     rhs_K,         rhs_Theta,  rhs_Axx,
            rhs_Azz,     rhs_Aww,       rhs_Axz,    hamiltonian,
            momentum_x,  momentum_z,    det_h_minus_one,
            tr_A,        rhs_hat_gamma_x, rhs_hat_gamma_z,
            rhs_shift_x, rhs_shift_z,   rhs_Bx,     rhs_Bz};
        for (const double component : components)
        {
            max_error = abs_max(max_error, component);
        }
        max_lapse_error = abs_max(max_lapse_error, rhs_lapse - lapse_target);
        max_gamma_error = abs_max(max_gamma_error, rhs_hat_gamma_x);
    }

    return {intervals, grid.dx, max_error, max_lapse_error, max_gamma_error};
}

inline double hidden_contraction_from_hww(const double x, const double W,
                                          const double W_x)
{
    const double gamma_x_ww = (1.0 - W) / x - 0.5 * W_x;
    return 2.0 * gamma_x_ww / W;
}

inline double max_delta_hww_hidden_contraction_error(const int intervals,
                                                     const double amplitude)
{
    const RadialGrid grid = make_radial_grid(intervals);
    std::vector<double> W;
    W.reserve(grid.x.size());
    for (const double x : grid.x)
    {
        W.push_back(1.0 + amplitude * x * x);
    }

    double max_error = 0.0;
    for (int i = 1; i < intervals; ++i)
    {
        const double x = grid.x[static_cast<std::size_t>(i)];
        const double W_x = d1(W, grid, i);
        const double discrete = hidden_contraction_from_hww(
            x, W[static_cast<std::size_t>(i)], W_x);
        const double expected =
            -4.0 * amplitude * x / (1.0 + amplitude * x * x);
        max_error = abs_max(max_error, discrete - expected);
    }
    return max_error;
}

inline std::vector<double> gamma_x_rhs_for_hww(
    const RadialGrid &grid, const std::vector<double> &W)
{
    const auto beta = values_on_grid(grid, beta_x);
    const auto K = values_on_grid(grid, K_trace);
    const auto Aww = values_on_grid(grid, A_ww);

    std::vector<double> div(beta.size(), 0.0);
    for (int i = 1; i < grid.intervals; ++i)
    {
        div[static_cast<std::size_t>(i)] = divergence_beta_at(beta, grid, i);
    }

    std::vector<double> rhs(beta.size(), 0.0);
    for (int i = 2; i <= grid.intervals - 2; ++i)
    {
        const std::size_t idx = static_cast<std::size_t>(i);
        const double x = grid.x[idx];
        const double d_beta = d1(beta, grid, i);
        const double d2_beta = d2(beta, grid, i);
        const double d_K = d1(K, grid, i);
        const double d_div = d1(div, grid, i);
        const double W_x = d1(W, grid, i);
        const double gamma_x_ww = (1.0 - W[idx]) / x - 0.5 * W_x;
        const double hidden_contracted = 2.0 * gamma_x_ww / W[idx];
        const double hidden_second_beta = d_beta / x - beta[idx] / (x * x);

        rhs[idx] =
            -1.5 * d_K + d2_beta + (2.0 / W[idx]) * hidden_second_beta +
            0.5 * d_div + 0.5 * div[idx] * hidden_contracted +
            4.0 * gamma_x_ww * Aww[idx] / (W[idx] * W[idx]);
    }
    return rhs;
}

inline double analytic_hww_jvp_gamma_x_rhs(const double x, const double delta_W,
                                           const double delta_W_x)
{
    const double lam = lambda(x);
    return -lam * delta_W / x - 2.0 * lam * delta_W_x;
}

inline double discrete_hww_jvp_gamma_x_rhs(
    const double x, const double hidden_second_beta, const double div_beta,
    const double Aww, const double delta_W, const double delta_W_x)
{
    const double delta_gamma_x_ww = -delta_W / x - 0.5 * delta_W_x;
    return -2.0 * hidden_second_beta * delta_W +
           div_beta * delta_gamma_x_ww + 4.0 * Aww * delta_gamma_x_ww;
}

inline double max_jvp_error_for_epsilon(const int intervals, const double eps)
{
    const RadialGrid grid = make_radial_grid(intervals);
    const auto beta = values_on_grid(grid, beta_x);
    const auto Aww = values_on_grid(grid, A_ww);
    std::vector<double> W_plus;
    std::vector<double> W_minus;
    std::vector<double> delta_W;
    W_plus.reserve(grid.x.size());
    W_minus.reserve(grid.x.size());
    delta_W.reserve(grid.x.size());
    for (const double x : grid.x)
    {
        const double perturbation = x * x;
        delta_W.push_back(perturbation);
        W_plus.push_back(1.0 + eps * perturbation);
        W_minus.push_back(1.0 - eps * perturbation);
    }

    std::vector<double> div(beta.size(), 0.0);
    for (int i = 1; i < intervals; ++i)
    {
        div[static_cast<std::size_t>(i)] = divergence_beta_at(beta, grid, i);
    }

    const auto rhs_plus = gamma_x_rhs_for_hww(grid, W_plus);
    const auto rhs_minus = gamma_x_rhs_for_hww(grid, W_minus);
    double max_error = 0.0;
    for (int i = 2; i <= intervals - 2; ++i)
    {
        const std::size_t idx = static_cast<std::size_t>(i);
        const double x = grid.x[idx];
        const double finite_difference_jvp =
            (rhs_plus[idx] - rhs_minus[idx]) / (2.0 * eps);
        const double hidden_second_beta =
            d1(beta, grid, i) / x - beta[idx] / (x * x);
        const double expected = discrete_hww_jvp_gamma_x_rhs(
            x, hidden_second_beta, div[idx], Aww[idx], delta_W[idx],
            d1(delta_W, grid, i));
        max_error = abs_max(max_error, finite_difference_jvp - expected);
    }
    return max_error;
}

inline JacobianPlateau compute_jacobian_plateau(const int intervals)
{
    return {max_jvp_error_for_epsilon(intervals, 1.0e-3),
            max_jvp_error_for_epsilon(intervals, 1.0e-5),
            max_jvp_error_for_epsilon(intervals, 1.0e-6),
            max_jvp_error_for_epsilon(intervals, 1.0e-11)};
}

struct ZParityState
{
    std::vector<double> scalar_beta_x;
    std::vector<double> scalar_K;
    std::vector<double> one_z_beta_z;
    std::vector<double> one_z_hxz;
};

struct ZParityOutput
{
    std::vector<double> scalar_rhs;
    std::vector<double> one_z_hxz_rhs;
    std::vector<double> one_z_hat_gamma_z_rhs;
};

struct ZParityProjection
{
    double allowed_norm;
    double forbidden_norm;
    double forbidden_to_allowed;
};

inline double z_spacing(const int z_points)
{
    const double two_pi = 6.283185307179586476925286766559;
    return two_pi / static_cast<double>(z_points);
}

inline double periodic_dz(const std::vector<double> &values, const int j)
{
    const int n = static_cast<int>(values.size());
    const int jp = (j + 1) % n;
    const int jm = (j + n - 1) % n;
    return (values[static_cast<std::size_t>(jp)] -
            values[static_cast<std::size_t>(jm)]) /
           (2.0 * z_spacing(n));
}

inline double periodic_dzz(const std::vector<double> &values, const int j)
{
    const int n = static_cast<int>(values.size());
    const int jp = (j + 1) % n;
    const int jm = (j + n - 1) % n;
    return (values[static_cast<std::size_t>(jp)] -
            2.0 * values[static_cast<std::size_t>(j)] +
            values[static_cast<std::size_t>(jm)]) /
           (z_spacing(n) * z_spacing(n));
}

inline ZParityState make_z_parity_state(const int z_points,
                                        const bool even_sector,
                                        const bool flip_beta_z_parity)
{
    if (z_points < 16)
    {
        throw std::domain_error("Stage 4AO-B parity check needs >=16 z points");
    }
    const double two_pi = 6.283185307179586476925286766559;
    const int mode = 3;
    ZParityState state;
    state.scalar_beta_x.reserve(static_cast<std::size_t>(z_points));
    state.scalar_K.reserve(static_cast<std::size_t>(z_points));
    state.one_z_beta_z.reserve(static_cast<std::size_t>(z_points));
    state.one_z_hxz.reserve(static_cast<std::size_t>(z_points));
    for (int j = 0; j < z_points; ++j)
    {
        const double z = two_pi * static_cast<double>(j) /
                         static_cast<double>(z_points);
        const double cosine = std::cos(static_cast<double>(mode) * z);
        const double sine = std::sin(static_cast<double>(mode) * z);

        // For the even scalar GL sector, scalar/zero-z/two-z variables use
        // cos(kz), while one-z variables use sin(kz). The odd sector swaps
        // these assignments.
        const double scalar_shape = even_sector ? cosine : sine;
        const double one_z_shape = even_sector ? sine : cosine;
        const double wrong_one_z_shape = even_sector ? cosine : sine;

        state.scalar_beta_x.push_back(1.1 * scalar_shape);
        state.scalar_K.push_back(-0.7 * scalar_shape);
        state.one_z_beta_z.push_back(
            1.3 * (flip_beta_z_parity ? wrong_one_z_shape : one_z_shape));
        state.one_z_hxz.push_back(0.4 * one_z_shape);
    }
    return state;
}

inline ZParityOutput apply_z_coupled_parity_operator(
    const ZParityState &state)
{
    const int z_points = static_cast<int>(state.scalar_beta_x.size());
    ZParityOutput output;
    output.scalar_rhs.resize(static_cast<std::size_t>(z_points), 0.0);
    output.one_z_hxz_rhs.resize(static_cast<std::size_t>(z_points), 0.0);
    output.one_z_hat_gamma_z_rhs.resize(static_cast<std::size_t>(z_points),
                                        0.0);

    for (int j = 0; j < z_points; ++j)
    {
        const std::size_t idx = static_cast<std::size_t>(j);

        // Representative 4AO-B z-coupled sub-operator:
        // - scalar/zero-z output receiving D_z of one-z beta^z, plus D_zz K;
        // - h_xz-like one-z output receiving D_z beta^x, plus D_zz h_xz;
        // - hat_Gamma^z/momentum-z-like one-z output receiving D_z K, plus
        //   D_zz beta^z.
        output.scalar_rhs[idx] =
            periodic_dz(state.one_z_beta_z, j) +
            0.25 * periodic_dzz(state.scalar_K, j);
        output.one_z_hxz_rhs[idx] =
            periodic_dz(state.scalar_beta_x, j) +
            0.5 * periodic_dzz(state.one_z_hxz, j);
        output.one_z_hat_gamma_z_rhs[idx] =
            periodic_dz(state.scalar_K, j) +
            0.2 * periodic_dzz(state.one_z_beta_z, j);
    }
    return output;
}

inline double projection_amplitude(const std::vector<double> &values,
                                   const int mode, const bool cosine_basis)
{
    const double two_pi = 6.283185307179586476925286766559;
    const int z_points = static_cast<int>(values.size());
    double dot = 0.0;
    double norm = 0.0;
    for (int j = 0; j < z_points; ++j)
    {
        const double z = two_pi * static_cast<double>(j) /
                         static_cast<double>(z_points);
        const double phase = static_cast<double>(mode) * z;
        const double basis = cosine_basis ? std::cos(phase) : std::sin(phase);
        dot += values[static_cast<std::size_t>(j)] * basis;
        norm += basis * basis;
    }
    return dot / norm;
}

inline ZParityProjection project_z_parity_output(const ZParityOutput &output,
                                                 const bool even_sector)
{
    const int mode = 3;
    const bool scalar_allowed_cosine = even_sector;
    const bool one_z_allowed_cosine = !even_sector;

    const double scalar_allowed = projection_amplitude(
        output.scalar_rhs, mode, scalar_allowed_cosine);
    const double scalar_forbidden = projection_amplitude(
        output.scalar_rhs, mode, !scalar_allowed_cosine);
    const double hxz_allowed = projection_amplitude(
        output.one_z_hxz_rhs, mode, one_z_allowed_cosine);
    const double hxz_forbidden = projection_amplitude(
        output.one_z_hxz_rhs, mode, !one_z_allowed_cosine);
    const double gamma_z_allowed = projection_amplitude(
        output.one_z_hat_gamma_z_rhs, mode, one_z_allowed_cosine);
    const double gamma_z_forbidden = projection_amplitude(
        output.one_z_hat_gamma_z_rhs, mode, !one_z_allowed_cosine);

    const double allowed_norm =
        std::sqrt(scalar_allowed * scalar_allowed +
                  hxz_allowed * hxz_allowed +
                  gamma_z_allowed * gamma_z_allowed);
    const double forbidden_norm =
        std::sqrt(scalar_forbidden * scalar_forbidden +
                  hxz_forbidden * hxz_forbidden +
                  gamma_z_forbidden * gamma_z_forbidden);
    return {allowed_norm, forbidden_norm, forbidden_norm / allowed_norm};
}

inline ParityLeakage compute_z_coupled_parity_leakage(const int z_points)
{
    const auto even_projection = project_z_parity_output(
        apply_z_coupled_parity_operator(
            make_z_parity_state(z_points, true, false)),
        true);
    const auto odd_projection = project_z_parity_output(
        apply_z_coupled_parity_operator(
            make_z_parity_state(z_points, false, false)),
        false);
    const auto negative_projection = project_z_parity_output(
        apply_z_coupled_parity_operator(
            make_z_parity_state(z_points, true, true)),
        true);

    return {even_projection.allowed_norm,
            even_projection.forbidden_norm,
            even_projection.forbidden_to_allowed,
            odd_projection.allowed_norm,
            odd_projection.forbidden_norm,
            odd_projection.forbidden_to_allowed,
            negative_projection.forbidden_to_allowed};
}

} // namespace Stage4AOGPDiscretePreflight
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_GP_DISCRETE_PREFLIGHT_HPP */
