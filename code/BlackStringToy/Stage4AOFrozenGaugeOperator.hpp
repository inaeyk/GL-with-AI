#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_OPERATOR_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_OPERATOR_HPP

#include "Stage4AOFrozenGaugeRicciAssembly.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeOperator
{
// Stage 4AO-C validation-only contract for the future frozen-gauge GL
// operator. This file defines the state vector, gauge exclusions, RHS block
// inventory, and radial boundary contract. It deliberately does not implement
// the coupled operator, an eigensolver, shift-invert, threshold search,
// production RHS wiring, or live evolution.
static constexpr bool validation_only = true;
static constexpr bool gp_shift_advection_block_implemented = true;
static constexpr bool tensor_shift_stretching_block_implemented = true;
static constexpr bool algebraic_metric_chi_coupling_block_implemented = true;
static constexpr bool k_equation_ccz4_k_theta_block_implemented = true;
static constexpr bool k_equation_ricci_scalar_insertion_block_implemented =
    true;
static constexpr bool k_equation_z_ricci_contributions_implemented = false;
static constexpr bool k_equation_kappa_damping_block_implemented = false;
static constexpr bool k_equation_lapse_hessian_vanishes_in_frozen_gauge = true;
static constexpr bool cosmological_constant_locked_to_zero = true;
static constexpr bool k_equation_cosmological_terms_implemented = false;
static constexpr bool a_equation_algebraic_non_curvature_block_implemented =
    true;
static constexpr bool theta_equation_algebraic_non_ricci_block_implemented =
    true;
static constexpr bool theta_equation_minus_k_delta_theta_block_implemented =
    true;
static constexpr bool theta_ricci_scalar_insertion_block_implemented = true;
static constexpr bool a_equation_ricci_curvature_insertion_block_implemented =
    true;
static constexpr bool trace_free_delta_a_projector_contract_implemented = true;
static constexpr bool boundary_derivative_validation_implemented = false;
static constexpr bool complete_frozen_gauge_operator_implemented = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool shift_invert_implemented = false;
static constexpr bool threshold_search_implemented = false;
static constexpr bool live_gauge_stage_4ao_d_implemented = false;
static constexpr bool production_rhs_wiring_implemented = false;

enum class PerturbationVariable
{
    chi,
    h_xx,
    h_xz,
    h_zz,
    h_ww,
    K,
    A_xx,
    A_xz,
    A_zz,
    A_ww,
    Theta,
    hat_Gamma_x,
    hat_Gamma_z
};

enum class FrozenGaugeVariable
{
    alpha,
    beta_x,
    beta_z,
    B_x,
    B_z
};

enum class RhsPiece
{
    gp_shift_advection,
    tensor_shift_stretching,
    algebraic_metric_chi_coupling,
    k_equation_ccz4_k_theta,
    k_equation_ricci_scalar_insertion,
    a_equation_algebraic_non_curvature,
    theta_equation_algebraic_non_ricci,
    theta_equation_minus_k_delta_theta,
    theta_ricci_scalar_insertion,
    a_equation_ricci_curvature_insertion,
    radial_derivatives,
    z_derivatives,
    hidden_sphere_terms,
    ricci_curvature_terms,
    k_a_trace_trace_free_terms,
    theta_constraint_terms,
    hat_gamma_hidden_evolution_terms
};

enum class PieceStatus
{
    implemented_now,
    reusable_helper,
    requires_modified_cartoon_rhs,
    missing_placeholder,
    not_applicable
};

enum class BoundaryRegion
{
    inner_boundary,
    outer_boundary,
    physical_gl_sector,
    constraint_related_variables,
    hatted_gamma_variables
};

struct RhsPieceStatus
{
    RhsPiece piece;
    PieceStatus status;
};

struct BoundaryConditionContract
{
    BoundaryRegion region;
    const char *contract;
    bool validation_test_implemented;
};

inline constexpr std::array<PerturbationVariable, 13>
    frozen_gauge_state_vector = {
        PerturbationVariable::chi,
        PerturbationVariable::h_xx,
        PerturbationVariable::h_xz,
        PerturbationVariable::h_zz,
        PerturbationVariable::h_ww,
        PerturbationVariable::K,
        PerturbationVariable::A_xx,
        PerturbationVariable::A_xz,
        PerturbationVariable::A_zz,
        PerturbationVariable::A_ww,
        PerturbationVariable::Theta,
        PerturbationVariable::hat_Gamma_x,
        PerturbationVariable::hat_Gamma_z};

inline constexpr std::array<FrozenGaugeVariable, 5>
    excluded_gauge_perturbations = {
        FrozenGaugeVariable::alpha,
        FrozenGaugeVariable::beta_x,
        FrozenGaugeVariable::beta_z,
        FrozenGaugeVariable::B_x,
        FrozenGaugeVariable::B_z};

inline constexpr std::array<const char *, 4> implemented_wrapper_pieces = {
    "frozen-gauge perturbation state-vector layout",
    "delta alpha, delta beta^i, and delta B^i exclusion contract",
    "RHS block inventory with implemented/reusable/missing labels",
    "radial-domain and boundary-condition contract"};

inline constexpr std::array<const char *, 12> implemented_operator_pieces = {
    "matrix-free GP-shift advection block beta_GP^x d_x(delta u)",
    "GP-shift tensor stretching for h_IJ and A_IJ",
    "algebraic h_IJ <- -2 A_IJ and chi <- +K/2 couplings",
    "K-output CCZ4 K(K-2Theta) linearization",
    "K-output physical Ricci scalar insertion +delta R",
    "A_IJ-output algebraic non-curvature linearization",
    "Theta-output algebraic non-Ricci linearization",
    "Theta-output -K_GP deltaTheta linearization",
    "Theta-output Ricci scalar insertion +0.5 delta R",
    "A_IJ-output Ricci curvature insertion [delta R_IJ]^TF",
    "trace-free delta A subspace and projector contract",
    "validation-only radial and periodic-z derivative scaffolding"};

inline constexpr std::array<const char *, 7> next_validation_hooks = {
    "finite-difference JVP of the actual frozen-gauge operator",
    "independent block-derived JVP for the same operator",
    "epsilon plateau for the actual operator or matrix assembly",
    "actual-operator parity leakage test",
    "radial-resolution convergence",
    "boundary-location convergence",
    "linearized MOTS map from delta U to delta R_H"};

inline const char *variable_name(const PerturbationVariable variable)
{
    switch (variable)
    {
    case PerturbationVariable::chi:
        return "chi";
    case PerturbationVariable::h_xx:
        return "h_xx";
    case PerturbationVariable::h_xz:
        return "h_xz";
    case PerturbationVariable::h_zz:
        return "h_zz";
    case PerturbationVariable::h_ww:
        return "h_ww";
    case PerturbationVariable::K:
        return "K";
    case PerturbationVariable::A_xx:
        return "A_xx";
    case PerturbationVariable::A_xz:
        return "A_xz";
    case PerturbationVariable::A_zz:
        return "A_zz";
    case PerturbationVariable::A_ww:
        return "A_ww";
    case PerturbationVariable::Theta:
        return "Theta";
    case PerturbationVariable::hat_Gamma_x:
        return "hat_Gamma^x";
    case PerturbationVariable::hat_Gamma_z:
        return "hat_Gamma^z";
    }
    return "unknown";
}

inline constexpr std::size_t variable_index(const PerturbationVariable variable)
{
    return static_cast<std::size_t>(variable);
}

inline const char *piece_status_name(const PieceStatus status)
{
    switch (status)
    {
    case PieceStatus::implemented_now:
        return "implemented now";
    case PieceStatus::reusable_helper:
        return "reusable helper only";
    case PieceStatus::requires_modified_cartoon_rhs:
        return "requires modified-cartoon RHS";
    case PieceStatus::missing_placeholder:
        return "missing placeholder";
    case PieceStatus::not_applicable:
        return "not applicable";
    }
    return "unknown";
}

inline bool is_metric_variable(const PerturbationVariable variable)
{
    return variable == PerturbationVariable::h_xx ||
           variable == PerturbationVariable::h_xz ||
           variable == PerturbationVariable::h_zz ||
           variable == PerturbationVariable::h_ww;
}

inline bool is_a_variable(const PerturbationVariable variable)
{
    return variable == PerturbationVariable::A_xx ||
           variable == PerturbationVariable::A_xz ||
           variable == PerturbationVariable::A_zz ||
           variable == PerturbationVariable::A_ww;
}

inline bool receives_tensor_shift_stretching(
    const PerturbationVariable variable)
{
    return is_metric_variable(variable) || is_a_variable(variable);
}

inline bool receives_algebraic_metric_chi_coupling(
    const PerturbationVariable variable)
{
    return variable == PerturbationVariable::chi || is_metric_variable(variable);
}

inline bool receives_k_equation_ccz4_k_theta(
    const PerturbationVariable variable)
{
    return variable == PerturbationVariable::K;
}

inline bool receives_k_equation_ricci_scalar_insertion(
    const PerturbationVariable variable)
{
    return variable == PerturbationVariable::K;
}

inline bool receives_a_equation_algebraic_non_curvature(
    const PerturbationVariable variable)
{
    return is_a_variable(variable);
}

inline bool receives_theta_equation_algebraic_non_ricci(
    const PerturbationVariable variable)
{
    return variable == PerturbationVariable::Theta;
}

inline bool receives_theta_equation_minus_k_delta_theta(
    const PerturbationVariable variable)
{
    return variable == PerturbationVariable::Theta;
}

inline bool receives_theta_ricci_scalar_insertion(
    const PerturbationVariable variable)
{
    return variable == PerturbationVariable::Theta;
}

inline bool receives_a_equation_ricci_curvature_insertion(
    const PerturbationVariable variable)
{
    return is_a_variable(variable);
}

inline bool uses_ricci_or_curvature(const PerturbationVariable variable)
{
    return variable == PerturbationVariable::K ||
           variable == PerturbationVariable::Theta ||
           is_a_variable(variable);
}

inline PieceStatus rhs_piece_status(const PerturbationVariable variable,
                                    const RhsPiece piece)
{
    switch (piece)
    {
    case RhsPiece::gp_shift_advection:
        return PieceStatus::implemented_now;

    case RhsPiece::tensor_shift_stretching:
        return receives_tensor_shift_stretching(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::algebraic_metric_chi_coupling:
        return receives_algebraic_metric_chi_coupling(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::k_equation_ccz4_k_theta:
        return receives_k_equation_ccz4_k_theta(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::k_equation_ricci_scalar_insertion:
        return receives_k_equation_ricci_scalar_insertion(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::a_equation_algebraic_non_curvature:
        return receives_a_equation_algebraic_non_curvature(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::theta_equation_algebraic_non_ricci:
        return receives_theta_equation_algebraic_non_ricci(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::theta_equation_minus_k_delta_theta:
        return receives_theta_equation_minus_k_delta_theta(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::theta_ricci_scalar_insertion:
        return receives_theta_ricci_scalar_insertion(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::a_equation_ricci_curvature_insertion:
        return receives_a_equation_ricci_curvature_insertion(variable)
                   ? PieceStatus::implemented_now
                   : PieceStatus::not_applicable;

    case RhsPiece::radial_derivatives:
    case RhsPiece::z_derivatives:
        return PieceStatus::reusable_helper;

    case RhsPiece::hidden_sphere_terms:
        if (variable == PerturbationVariable::h_ww ||
            variable == PerturbationVariable::A_ww ||
            variable == PerturbationVariable::hat_Gamma_x)
        {
            return PieceStatus::reusable_helper;
        }
        if (variable == PerturbationVariable::hat_Gamma_z)
        {
            return PieceStatus::missing_placeholder;
        }
        return PieceStatus::requires_modified_cartoon_rhs;

    case RhsPiece::ricci_curvature_terms:
        return uses_ricci_or_curvature(variable)
                   ? PieceStatus::reusable_helper
                   : PieceStatus::not_applicable;

    case RhsPiece::k_a_trace_trace_free_terms:
        return is_metric_variable(variable) || variable == PerturbationVariable::K ||
                       is_a_variable(variable)
                   ? PieceStatus::requires_modified_cartoon_rhs
                   : PieceStatus::not_applicable;

    case RhsPiece::theta_constraint_terms:
        return variable == PerturbationVariable::Theta ||
                       variable == PerturbationVariable::hat_Gamma_x ||
                       variable == PerturbationVariable::hat_Gamma_z
                   ? PieceStatus::requires_modified_cartoon_rhs
                   : PieceStatus::not_applicable;

    case RhsPiece::hat_gamma_hidden_evolution_terms:
        if (variable == PerturbationVariable::hat_Gamma_x)
        {
            return PieceStatus::reusable_helper;
        }
        if (variable == PerturbationVariable::hat_Gamma_z)
        {
            return PieceStatus::missing_placeholder;
        }
        return PieceStatus::not_applicable;
    }
    return PieceStatus::missing_placeholder;
}

inline bool variable_rhs_complete(const PerturbationVariable)
{
    return false;
}

inline bool any_variable_rhs_complete()
{
    for (const auto variable : frozen_gauge_state_vector)
    {
        if (variable_rhs_complete(variable))
        {
            return true;
        }
    }
    return false;
}

class FrozenGaugePerturbationVector
{
  public:
    static constexpr std::size_t size() { return frozen_gauge_state_vector.size(); }

    double value(const PerturbationVariable variable) const
    {
        return m_values[variable_index(variable)];
    }

    double value_at_index(const std::size_t index) const
    {
        if (index >= m_values.size())
        {
            throw std::out_of_range("Stage 4AO-C perturbation slot is invalid");
        }
        return m_values[index];
    }

  private:
    explicit FrozenGaugePerturbationVector(
        const std::array<double, frozen_gauge_state_vector.size()> &values)
        : m_values(values)
    {
    }

    friend FrozenGaugePerturbationVector make_frozen_gauge_perturbation_vector(
        const std::array<double, frozen_gauge_state_vector.size()> &values);

    std::array<double, frozen_gauge_state_vector.size()> m_values;
};

inline FrozenGaugePerturbationVector make_frozen_gauge_perturbation_vector(
    const std::array<double, frozen_gauge_state_vector.size()> &values)
{
    for (const auto value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C perturbation values must be finite");
        }
    }
    return FrozenGaugePerturbationVector(values);
}

class RadialDomainContract
{
  public:
    double r0() const { return m_r0; }
    double x_in() const { return m_x_in; }
    double x_out() const { return m_x_out; }
    bool z_periodic() const { return true; }
    bool excludes_axis() const { return true; }
    bool brackets_horizon() const
    {
        return 0.0 < m_x_in && m_x_in < m_r0 && m_r0 < m_x_out;
    }

  private:
    RadialDomainContract(const double r0, const double x_in,
                         const double x_out)
        : m_r0(r0), m_x_in(x_in), m_x_out(x_out)
    {
    }

    friend RadialDomainContract make_radial_domain_contract(
        double r0, double x_in, double x_out);

    double m_r0;
    double m_x_in;
    double m_x_out;
};

inline RadialDomainContract make_radial_domain_contract(const double r0,
                                                        const double x_in,
                                                        const double x_out)
{
    if (!std::isfinite(r0) || !std::isfinite(x_in) ||
        !std::isfinite(x_out) || !(0.0 < x_in && x_in < r0 && r0 < x_out))
    {
        throw std::domain_error(
            "Stage 4AO-C radial domain requires 0 < x_in < r0 < x_out");
    }
    return RadialDomainContract(r0, x_in, x_out);
}

class RadialGrid
{
  public:
    const RadialDomainContract &domain() const { return m_domain; }
    std::size_t points() const { return m_x.size(); }
    double dx() const { return m_dx; }
    double x(const std::size_t index) const
    {
        if (index >= m_x.size())
        {
            throw std::out_of_range("Stage 4AO-C radial index is invalid");
        }
        return m_x[index];
    }
    std::size_t first_interior_index() const { return 1; }
    std::size_t last_interior_index() const { return m_x.size() - 2; }

  private:
    RadialGrid(const RadialDomainContract &domain, const std::size_t intervals)
        : m_domain(domain),
          m_x(intervals + 1),
          m_dx((domain.x_out() - domain.x_in()) / intervals)
    {
        for (std::size_t i = 0; i < m_x.size(); ++i)
        {
            m_x[i] = domain.x_in() + static_cast<double>(i) * m_dx;
        }
    }

    friend RadialGrid make_radial_grid(const RadialDomainContract &domain,
                                       std::size_t intervals);

    RadialDomainContract m_domain;
    std::vector<double> m_x;
    double m_dx;
};

inline RadialGrid make_radial_grid(const RadialDomainContract &domain,
                                   const std::size_t intervals)
{
    if (intervals < 4)
    {
        throw std::domain_error(
            "Stage 4AO-C radial grid needs at least four intervals");
    }
    return RadialGrid(domain, intervals);
}

inline double beta_gp_x(const double r0, const double x)
{
    if (!std::isfinite(r0) || !std::isfinite(x) || !(r0 > 0.0) ||
        !(x > 0.0))
    {
        throw std::domain_error("Stage 4AO-C GP shift needs r0>0 and x>0");
    }
    return std::sqrt(r0 / x);
}

inline double lambda_gp(const double r0, const double x)
{
    if (!std::isfinite(r0) || !std::isfinite(x) || !(r0 > 0.0) ||
        !(x > 0.0))
    {
        throw std::domain_error("Stage 4AO-C GP lambda needs r0>0 and x>0");
    }
    return std::sqrt(r0 / (x * x * x));
}

inline double dx_beta_gp_x(const double r0, const double x)
{
    return -0.5 * lambda_gp(r0, x);
}

inline double beta_gp_x_over_x(const double r0, const double x)
{
    return lambda_gp(r0, x);
}

inline double div_beta_gp(const double r0, const double x)
{
    // Stage 4AO-A GP background in d=4 with two hidden directions:
    // partial_x beta^x + 2 beta^x/x = -lambda/2 + 2 lambda.
    return dx_beta_gp_x(r0, x) + 2.0 * beta_gp_x_over_x(r0, x);
}

inline double tensor_shift_stretching_coefficient(
    const PerturbationVariable variable, const double r0, const double x)
{
    const double lambda = lambda_gp(r0, x);
    switch (variable)
    {
    case PerturbationVariable::h_xx:
    case PerturbationVariable::A_xx:
        return -7.0 * lambda / 4.0;

    case PerturbationVariable::h_xz:
    case PerturbationVariable::A_xz:
        return -5.0 * lambda / 4.0;

    case PerturbationVariable::h_zz:
    case PerturbationVariable::A_zz:
        return -3.0 * lambda / 4.0;

    case PerturbationVariable::h_ww:
    case PerturbationVariable::A_ww:
        return 5.0 * lambda / 4.0;

    case PerturbationVariable::chi:
    case PerturbationVariable::K:
    case PerturbationVariable::Theta:
    case PerturbationVariable::hat_Gamma_x:
    case PerturbationVariable::hat_Gamma_z:
        return 0.0;
    }
    return 0.0;
}

inline double gp_background_a_xx(const double r0, const double x)
{
    return -7.0 * lambda_gp(r0, x) / 8.0;
}

inline double gp_background_a_zz(const double r0, const double x)
{
    return -3.0 * lambda_gp(r0, x) / 8.0;
}

inline double gp_background_a_ww(const double r0, const double x)
{
    return 5.0 * lambda_gp(r0, x) / 8.0;
}

inline double delta_trace_a_at_point(
    const double r0, const double x,
    const FrozenGaugePerturbationVector &input)
{
    // Linearized from tr A = h^xx A_xx + h^zz A_zz + 2 h^ww A_ww on the
    // locked diagonal GP background. The hidden multiplicity two is part of
    // the trace, and h_xz/A_xz do not enter because the background is
    // diagonal with A_xz_GP=0.
    return input.value(PerturbationVariable::A_xx) +
           input.value(PerturbationVariable::A_zz) +
           2.0 * input.value(PerturbationVariable::A_ww) -
           gp_background_a_xx(r0, x) *
               input.value(PerturbationVariable::h_xx) -
           gp_background_a_zz(r0, x) *
               input.value(PerturbationVariable::h_zz) -
           2.0 * gp_background_a_ww(r0, x) *
               input.value(PerturbationVariable::h_ww);
}

inline FrozenGaugePerturbationVector project_delta_a_trace_free_at_point(
    const double r0, const double x,
    const FrozenGaugePerturbationVector &input)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : frozen_gauge_state_vector)
    {
        values[variable_index(variable)] = input.value(variable);
    }

    // Validation-only Stage 4AO-C projector. It mirrors GRChombo's
    // make_trace_free convention at the locked flat conformal GP background:
    // subtract (1/d) h_IJ delta_tr_A with d=4 from each diagonal lower-index
    // A component. Hidden multiplicity two is already in delta_tr_A, so the
    // representative A_ww component receives the same -trace/4 subtraction as
    // A_xx and A_zz. A_xz and non-A variables are preserved.
    const double trace_error = delta_trace_a_at_point(r0, x, input);
    constexpr double one_over_d = 0.25;
    values[variable_index(PerturbationVariable::A_xx)] -=
        one_over_d * trace_error;
    values[variable_index(PerturbationVariable::A_zz)] -=
        one_over_d * trace_error;
    values[variable_index(PerturbationVariable::A_ww)] -=
        one_over_d * trace_error;

    return make_frozen_gauge_perturbation_vector(values);
}

inline double radial_dx_interior(const RadialGrid &grid,
                                 const std::vector<double> &values,
                                 const std::size_t index)
{
    if (values.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C radial derivative values do not match the grid");
    }
    if (index == 0 || index + 1 >= grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C radial derivative is validated only on interiors");
    }
    return (values[index + 1] - values[index - 1]) / (2.0 * grid.dx());
}

inline double periodic_z_spacing(const std::size_t points,
                                 const double period)
{
    if (points < 4 || !std::isfinite(period) || !(period > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C periodic z grid needs at least four points and L>0");
    }
    return period / static_cast<double>(points);
}

inline double periodic_dz(const std::vector<double> &values,
                          const double period, const std::size_t index)
{
    const auto points = values.size();
    const double dz = periodic_z_spacing(points, period);
    if (index >= points)
    {
        throw std::out_of_range("Stage 4AO-C z index is invalid");
    }
    const auto left = (index + points - 1) % points;
    const auto right = (index + 1) % points;
    return (values[right] - values[left]) / (2.0 * dz);
}

inline double periodic_dzz(const std::vector<double> &values,
                           const double period, const std::size_t index)
{
    const auto points = values.size();
    const double dz = periodic_z_spacing(points, period);
    if (index >= points)
    {
        throw std::out_of_range("Stage 4AO-C z index is invalid");
    }
    const auto left = (index + points - 1) % points;
    const auto right = (index + 1) % points;
    return (values[right] - 2.0 * values[index] + values[left]) / (dz * dz);
}

class FrozenGaugeApplyResult
{
  public:
    const std::vector<FrozenGaugePerturbationVector> &values() const
    {
        return m_values;
    }

    const FrozenGaugePerturbationVector &at(const std::size_t index) const
    {
        if (index >= m_values.size())
        {
            throw std::out_of_range("Stage 4AO-C apply-result index invalid");
        }
        return m_values[index];
    }

    bool partial_operator_only() const { return true; }
    bool boundary_values_are_placeholders() const
    {
        return m_boundary_values_are_placeholders;
    }
    std::size_t first_valid_index() const { return m_first_valid_index; }
    std::size_t last_valid_index() const { return m_last_valid_index; }

  private:
    FrozenGaugeApplyResult(
        const std::vector<FrozenGaugePerturbationVector> &values,
        const std::size_t first_valid_index,
        const std::size_t last_valid_index,
        const bool boundary_values_are_placeholders)
        : m_values(values),
          m_first_valid_index(first_valid_index),
          m_last_valid_index(last_valid_index),
          m_boundary_values_are_placeholders(boundary_values_are_placeholders)
    {
    }

    friend FrozenGaugeApplyResult apply_gp_shift_advection_block(
        const RadialGrid &grid,
        const std::vector<FrozenGaugePerturbationVector> &input);

    friend FrozenGaugeApplyResult apply_tensor_shift_stretching_block(
        const RadialGrid &grid,
        const std::vector<FrozenGaugePerturbationVector> &input);

    friend FrozenGaugeApplyResult apply_algebraic_metric_chi_coupling_block(
        const RadialGrid &grid,
        const std::vector<FrozenGaugePerturbationVector> &input);

    friend FrozenGaugeApplyResult apply_k_equation_ccz4_k_theta_block(
        const RadialGrid &grid,
        const std::vector<FrozenGaugePerturbationVector> &input);

    friend FrozenGaugeApplyResult apply_k_equation_ricci_scalar_insertion_block(
        const RadialGrid &grid,
        const std::vector<
            Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly>
            &ricci_assemblies);

    friend FrozenGaugeApplyResult
    apply_a_equation_algebraic_non_curvature_block(
        const RadialGrid &grid,
        const std::vector<FrozenGaugePerturbationVector> &input);

    friend FrozenGaugeApplyResult apply_theta_equation_algebraic_non_ricci_block(
        const RadialGrid &grid,
        const std::vector<FrozenGaugePerturbationVector> &input);

    friend FrozenGaugeApplyResult
    apply_theta_equation_minus_k_delta_theta_block(
        const RadialGrid &grid,
        const std::vector<FrozenGaugePerturbationVector> &input);

    friend FrozenGaugeApplyResult apply_theta_ricci_scalar_insertion_block(
        const RadialGrid &grid,
        const std::vector<
            Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly>
            &ricci_assemblies);

    friend FrozenGaugeApplyResult
    apply_a_equation_ricci_curvature_insertion_block(
        const RadialGrid &grid,
        const std::vector<
            Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly>
            &ricci_assemblies);

    std::vector<FrozenGaugePerturbationVector> m_values;
    std::size_t m_first_valid_index;
    std::size_t m_last_valid_index;
    bool m_boundary_values_are_placeholders;
};

inline FrozenGaugePerturbationVector apply_tensor_shift_stretching_at_point(
    const double r0, const double x,
    const FrozenGaugePerturbationVector &input)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};
    for (const auto variable : frozen_gauge_state_vector)
    {
        const auto index = variable_index(variable);
        values[index] =
            tensor_shift_stretching_coefficient(variable, r0, x) *
            input.value(variable);
    }
    return make_frozen_gauge_perturbation_vector(values);
}

inline FrozenGaugePerturbationVector
apply_algebraic_metric_chi_coupling_at_point(
    const FrozenGaugePerturbationVector &input)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // Linearized from GRChombo's d=4 CCZ4 convention on the locked GP
    // background: alpha_GP=1, chi_GP=1, and K_GP=div beta_GP.
    values[variable_index(PerturbationVariable::chi)] =
        0.5 * input.value(PerturbationVariable::K);

    values[variable_index(PerturbationVariable::h_xx)] =
        -2.0 * input.value(PerturbationVariable::A_xx);
    values[variable_index(PerturbationVariable::h_xz)] =
        -2.0 * input.value(PerturbationVariable::A_xz);
    values[variable_index(PerturbationVariable::h_zz)] =
        -2.0 * input.value(PerturbationVariable::A_zz);
    values[variable_index(PerturbationVariable::h_ww)] =
        -2.0 * input.value(PerturbationVariable::A_ww);

    return make_frozen_gauge_perturbation_vector(values);
}

inline double k_equation_ccz4_k_theta_coefficient(
    const PerturbationVariable variable, const double r0, const double x)
{
    const double lambda = lambda_gp(r0, x);

    switch (variable)
    {
    case PerturbationVariable::K:
        return 3.0 * lambda;
    case PerturbationVariable::Theta:
        return -3.0 * lambda;
    case PerturbationVariable::chi:
    case PerturbationVariable::h_xx:
    case PerturbationVariable::h_xz:
    case PerturbationVariable::h_zz:
    case PerturbationVariable::h_ww:
    case PerturbationVariable::A_xx:
    case PerturbationVariable::A_xz:
    case PerturbationVariable::A_zz:
    case PerturbationVariable::A_ww:
    case PerturbationVariable::hat_Gamma_x:
    case PerturbationVariable::hat_Gamma_z:
        return 0.0;
    }
    return 0.0;
}

inline FrozenGaugePerturbationVector apply_k_equation_ccz4_k_theta_at_point(
    const double r0, const double x,
    const FrozenGaugePerturbationVector &input)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // Selected USE_CCZ4 branch: alpha * K * (K - 2 Theta). With
    // K_GP=3 lambda/2, Theta_GP=0, alpha_GP=1, and delta alpha=0, the
    // linearization is +3 lambda delta K - 3 lambda delta Theta. The former
    // A_IJ A^IJ + K^2/d block belongs to the rejected USE_BSSN branch and is
    // intentionally absent from this row.
    const double k_output =
        k_equation_ccz4_k_theta_coefficient(PerturbationVariable::K, r0, x) *
            input.value(PerturbationVariable::K) +
        k_equation_ccz4_k_theta_coefficient(PerturbationVariable::Theta, r0,
                                             x) *
            input.value(PerturbationVariable::Theta);

    values[variable_index(PerturbationVariable::K)] = k_output;
    return make_frozen_gauge_perturbation_vector(values);
}

inline double a_equation_algebraic_non_curvature_coefficient(
    const PerturbationVariable output_variable,
    const PerturbationVariable input_variable, const double r0, const double x)
{
    const double lambda = lambda_gp(r0, x);
    const double lambda_squared = lambda * lambda;

    switch (output_variable)
    {
    case PerturbationVariable::A_xx:
        switch (input_variable)
        {
        case PerturbationVariable::h_xx:
            return 49.0 * lambda_squared / 32.0;
        case PerturbationVariable::A_xx:
            return 5.0 * lambda;
        case PerturbationVariable::K:
            return -7.0 * lambda / 8.0;
        case PerturbationVariable::Theta:
            return 7.0 * lambda / 4.0;
        default:
            return 0.0;
        }

    case PerturbationVariable::A_xz:
        switch (input_variable)
        {
        case PerturbationVariable::h_xz:
            return 21.0 * lambda_squared / 32.0;
        case PerturbationVariable::A_xz:
            return 4.0 * lambda;
        default:
            return 0.0;
        }

    case PerturbationVariable::A_zz:
        switch (input_variable)
        {
        case PerturbationVariable::h_zz:
            return 9.0 * lambda_squared / 32.0;
        case PerturbationVariable::A_zz:
            return 3.0 * lambda;
        case PerturbationVariable::K:
            return -3.0 * lambda / 8.0;
        case PerturbationVariable::Theta:
            return 3.0 * lambda / 4.0;
        default:
            return 0.0;
        }

    case PerturbationVariable::A_ww:
        switch (input_variable)
        {
        case PerturbationVariable::h_ww:
            return 25.0 * lambda_squared / 32.0;
        case PerturbationVariable::A_ww:
            return -lambda;
        case PerturbationVariable::K:
            return 5.0 * lambda / 8.0;
        case PerturbationVariable::Theta:
            return -5.0 * lambda / 4.0;
        default:
            return 0.0;
        }

    case PerturbationVariable::chi:
    case PerturbationVariable::h_xx:
    case PerturbationVariable::h_xz:
    case PerturbationVariable::h_zz:
    case PerturbationVariable::h_ww:
    case PerturbationVariable::K:
    case PerturbationVariable::Theta:
    case PerturbationVariable::hat_Gamma_x:
    case PerturbationVariable::hat_Gamma_z:
        return 0.0;
    }
    return 0.0;
}

inline FrozenGaugePerturbationVector
apply_a_equation_algebraic_non_curvature_at_point(
    const double r0, const double x,
    const FrozenGaugePerturbationVector &input)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // Linearized from GRChombo's A_ij algebraic non-curvature term
    // (K - 2 Theta) A_ij - 2 h^kl A_ik A_lj on the locked GP background.
    // The h^kl variation is included. A_ww is one component equation, so no
    // hidden multiplicity factor is applied to this componentwise block.
    for (const auto output_variable : frozen_gauge_state_vector)
    {
        if (!is_a_variable(output_variable))
        {
            continue;
        }
        double output = 0.0;
        for (const auto input_variable : frozen_gauge_state_vector)
        {
            output += a_equation_algebraic_non_curvature_coefficient(
                          output_variable, input_variable, r0, x) *
                      input.value(input_variable);
        }
        values[variable_index(output_variable)] = output;
    }

    return make_frozen_gauge_perturbation_vector(values);
}

inline double theta_equation_algebraic_non_ricci_coefficient(
    const PerturbationVariable variable, const double r0, const double x)
{
    const double lambda = lambda_gp(r0, x);
    const double lambda_squared = lambda * lambda;

    switch (variable)
    {
    case PerturbationVariable::h_xx:
        return 49.0 * lambda_squared / 64.0;
    case PerturbationVariable::h_zz:
        return 9.0 * lambda_squared / 64.0;
    case PerturbationVariable::h_ww:
        return 25.0 * lambda_squared / 32.0;
    case PerturbationVariable::A_xx:
        return 7.0 * lambda / 8.0;
    case PerturbationVariable::A_zz:
        return 3.0 * lambda / 8.0;
    case PerturbationVariable::A_ww:
        return -5.0 * lambda / 4.0;
    case PerturbationVariable::K:
        return 9.0 * lambda / 8.0;
    case PerturbationVariable::chi:
    case PerturbationVariable::h_xz:
    case PerturbationVariable::A_xz:
    case PerturbationVariable::Theta:
    case PerturbationVariable::hat_Gamma_x:
    case PerturbationVariable::hat_Gamma_z:
        return 0.0;
    }
    return 0.0;
}

inline FrozenGaugePerturbationVector
apply_theta_equation_algebraic_non_ricci_at_point(
    const double r0, const double x,
    const FrozenGaugePerturbationVector &input)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // Linearized from the non-Ricci algebraic part of GRChombo's CCZ4 Theta
    // RHS: 0.5 * (((d - 1) / d) K^2 - A_IJ A^IJ). Ricci scalar,
    // -Theta*K, damping, Z-dot-grad-lapse, and cosmological terms are not
    // part of this validation-only substep.
    double theta_output = 0.0;
    for (const auto variable : frozen_gauge_state_vector)
    {
        theta_output +=
            theta_equation_algebraic_non_ricci_coefficient(variable, r0, x) *
            input.value(variable);
    }

    values[variable_index(PerturbationVariable::Theta)] = theta_output;
    return make_frozen_gauge_perturbation_vector(values);
}

inline double theta_equation_minus_k_delta_theta_coefficient(
    const PerturbationVariable variable, const double r0, const double x)
{
    if (variable == PerturbationVariable::Theta)
    {
        return -3.0 * lambda_gp(r0, x) / 2.0;
    }
    return 0.0;
}

inline FrozenGaugePerturbationVector
apply_theta_equation_minus_k_delta_theta_at_point(
    const double r0, const double x,
    const FrozenGaugePerturbationVector &input)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // Linearized from GRChombo's 0.5 * (-2 Theta K) = -Theta K on the locked
    // GP background. Since Theta_GP=0, only -K_GP deltaTheta contributes, with
    // K_GP = 3 lambda / 2. Z4 damping remains a separate, unimplemented block.
    values[variable_index(PerturbationVariable::Theta)] =
        theta_equation_minus_k_delta_theta_coefficient(
            PerturbationVariable::Theta, r0, x) *
        input.value(PerturbationVariable::Theta);

    return make_frozen_gauge_perturbation_vector(values);
}

inline FrozenGaugePerturbationVector
apply_k_equation_ricci_scalar_insertion_at_point(
    const Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly
        &ricci_assembly)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // Selected USE_CCZ4 K RHS contains +alpha R. Frozen gauge has
    // alpha_GP=1 and delta alpha=0, while R_GP=0, so the physical-Ricci
    // contribution is exactly +delta R. The reviewed assembly supplies
    // delta R_xx + delta R_zz + 2 delta R_ww. Z/hat_Gamma-dependent Ricci
    // contributions remain separate and unimplemented.
    values[variable_index(PerturbationVariable::K)] =
        ricci_assembly.scalar_trace();

    return make_frozen_gauge_perturbation_vector(values);
}

inline FrozenGaugePerturbationVector
apply_theta_ricci_scalar_insertion_at_point(
    const Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly
        &ricci_assembly)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // Frozen gauge has alpha_GP=1 and delta alpha=0, so the GRChombo Theta RHS
    // term 0.5 * alpha * R contributes only +0.5 * delta R. The scalar trace
    // is supplied by the reviewed Stage 4AO-C Ricci assembly helper, which
    // already owns the hidden ww multiplicity and the absence of R_xz from the
    // background scalar trace. This is not the full Theta RHS.
    values[variable_index(PerturbationVariable::Theta)] =
        0.5 * ricci_assembly.scalar_trace();

    return make_frozen_gauge_perturbation_vector(values);
}

inline FrozenGaugePerturbationVector
apply_a_equation_ricci_curvature_insertion_at_point(
    const Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly
        &ricci_assembly)
{
    std::array<double, frozen_gauge_state_vector.size()> values = {};

    // GRChombo forms the A_IJ curvature/lapse source as the trace-free part of
    // -D_I D_J alpha + chi * alpha * R_IJ. In Stage 4AO-C frozen gauge,
    // delta alpha=0, alpha_GP=1, chi_GP=1, R_IJ^GP=0, and
    // partial_I alpha_GP=0. Therefore delta(D_I D_J alpha)=0 and
    // delta(chi * alpha * R_IJ)=delta R_IJ, so this insertion is exactly the
    // assembled [delta R_IJ]^TF with prefactor one. The representative A_ww
    // output receives tf_ww once; the hidden multiplicity already entered the
    // Ricci trace assembly.
    values[variable_index(PerturbationVariable::A_xx)] =
        ricci_assembly.tf_xx();
    values[variable_index(PerturbationVariable::A_xz)] =
        ricci_assembly.tf_xz();
    values[variable_index(PerturbationVariable::A_zz)] =
        ricci_assembly.tf_zz();
    values[variable_index(PerturbationVariable::A_ww)] =
        ricci_assembly.tf_ww();

    return make_frozen_gauge_perturbation_vector(values);
}

inline FrozenGaugeApplyResult apply_gp_shift_advection_block(
    const RadialGrid &grid,
    const std::vector<FrozenGaugePerturbationVector> &input)
{
    if (input.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C advection input must have one vector per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(input.size());
    const std::array<double, frozen_gauge_state_vector.size()> zero_values = {};
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        output.push_back(make_frozen_gauge_perturbation_vector(zero_values));
    }

    for (std::size_t i = grid.first_interior_index();
         i <= grid.last_interior_index(); ++i)
    {
        std::array<double, frozen_gauge_state_vector.size()> values = {};
        const double beta = beta_gp_x(grid.domain().r0(), grid.x(i));
        for (std::size_t slot = 0; slot < frozen_gauge_state_vector.size();
             ++slot)
        {
            const double derivative =
                (input[i + 1].value_at_index(slot) -
                 input[i - 1].value_at_index(slot)) /
                (2.0 * grid.dx());
            values[slot] = beta * derivative;
        }
        output[i] = make_frozen_gauge_perturbation_vector(values);
    }

    return FrozenGaugeApplyResult(output, grid.first_interior_index(),
                                  grid.last_interior_index(), true);
}

inline FrozenGaugeApplyResult apply_tensor_shift_stretching_block(
    const RadialGrid &grid,
    const std::vector<FrozenGaugePerturbationVector> &input)
{
    if (input.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C tensor-stretching input must have one vector per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        output.push_back(apply_tensor_shift_stretching_at_point(
            grid.domain().r0(), grid.x(i), input[i]));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult apply_algebraic_metric_chi_coupling_block(
    const RadialGrid &grid,
    const std::vector<FrozenGaugePerturbationVector> &input)
{
    if (input.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C algebraic-coupling input must have one vector per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        output.push_back(apply_algebraic_metric_chi_coupling_at_point(input[i]));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult apply_k_equation_ccz4_k_theta_block(
    const RadialGrid &grid,
    const std::vector<FrozenGaugePerturbationVector> &input)
{
    if (input.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C K CCZ4 K/Theta input must have one vector per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        output.push_back(apply_k_equation_ccz4_k_theta_at_point(
            grid.domain().r0(), grid.x(i), input[i]));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult apply_k_equation_ricci_scalar_insertion_block(
    const RadialGrid &grid,
    const std::vector<Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly>
        &ricci_assemblies)
{
    if (ricci_assemblies.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C K Ricci-scalar input must have one assembly per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(ricci_assemblies.size());
    for (const auto &assembly : ricci_assemblies)
    {
        output.push_back(
            apply_k_equation_ricci_scalar_insertion_at_point(assembly));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult
apply_a_equation_algebraic_non_curvature_block(
    const RadialGrid &grid,
    const std::vector<FrozenGaugePerturbationVector> &input)
{
    if (input.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C A algebraic input must have one vector per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        output.push_back(apply_a_equation_algebraic_non_curvature_at_point(
            grid.domain().r0(), grid.x(i), input[i]));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult apply_theta_equation_algebraic_non_ricci_block(
    const RadialGrid &grid,
    const std::vector<FrozenGaugePerturbationVector> &input)
{
    if (input.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C Theta algebraic input must have one vector per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        output.push_back(apply_theta_equation_algebraic_non_ricci_at_point(
            grid.domain().r0(), grid.x(i), input[i]));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult apply_theta_equation_minus_k_delta_theta_block(
    const RadialGrid &grid,
    const std::vector<FrozenGaugePerturbationVector> &input)
{
    if (input.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C Theta minus-K input must have one vector per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i)
    {
        output.push_back(apply_theta_equation_minus_k_delta_theta_at_point(
            grid.domain().r0(), grid.x(i), input[i]));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult apply_theta_ricci_scalar_insertion_block(
    const RadialGrid &grid,
    const std::vector<Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly>
        &ricci_assemblies)
{
    if (ricci_assemblies.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C Theta Ricci-scalar input must have one assembly per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(ricci_assemblies.size());
    for (const auto &assembly : ricci_assemblies)
    {
        output.push_back(apply_theta_ricci_scalar_insertion_at_point(assembly));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline FrozenGaugeApplyResult
apply_a_equation_ricci_curvature_insertion_block(
    const RadialGrid &grid,
    const std::vector<Stage4AOFrozenGaugeRicciAssembly::TraceFreeRicciAssembly>
        &ricci_assemblies)
{
    if (ricci_assemblies.size() != grid.points())
    {
        throw std::domain_error(
            "Stage 4AO-C A Ricci-curvature input must have one assembly per radial grid point");
    }

    std::vector<FrozenGaugePerturbationVector> output;
    output.reserve(ricci_assemblies.size());
    for (const auto &assembly : ricci_assemblies)
    {
        output.push_back(
            apply_a_equation_ricci_curvature_insertion_at_point(assembly));
    }

    return FrozenGaugeApplyResult(output, 0, grid.points() - 1, false);
}

inline std::array<BoundaryConditionContract, 5> boundary_contracts()
{
    return {{
        {BoundaryRegion::inner_boundary,
         "inner boundary is inside the GP horizon; the future operator must "
         "use a characteristic/outflow-compatible or otherwise justified "
         "radial condition and show x_in convergence",
         false},
        {BoundaryRegion::outer_boundary,
         "outer boundary must use a documented asymptotic/radiative or "
         "frozen-background condition and show x_out convergence",
         false},
        {BoundaryRegion::physical_gl_sector,
         "physical GL sector uses compact-z periodicity and the locked "
         "horizon-radius observable; radial conditions are not validated yet",
         false},
        {BoundaryRegion::constraint_related_variables,
         "constraint variables must use a constraint-compatible condition and "
         "be checked in the actual-operator parity/JVP tests",
         false},
        {BoundaryRegion::hatted_gamma_variables,
         "hat_Gamma variables must preserve the GRChombo hatted convention, "
         "hidden multiplicity, and parity sector before spectral use",
         false}}};
}

class FrozenGaugeOperatorContract
{
  public:
    const RadialDomainContract &domain() const { return m_domain; }

    bool complete_operator_implemented() const
    {
        return complete_frozen_gauge_operator_implemented;
    }

    bool eigensolver_allowed() const
    {
        return complete_operator_implemented() && false;
    }

    PieceStatus status(const PerturbationVariable variable,
                       const RhsPiece piece) const
    {
        return rhs_piece_status(variable, piece);
    }

    bool rhs_complete(const PerturbationVariable variable) const
    {
        return variable_rhs_complete(variable);
    }

  private:
    explicit FrozenGaugeOperatorContract(const RadialDomainContract &domain)
        : m_domain(domain)
    {
    }

    friend FrozenGaugeOperatorContract make_frozen_gauge_operator_contract(
        const RadialDomainContract &domain);

    RadialDomainContract m_domain;
};

inline FrozenGaugeOperatorContract make_frozen_gauge_operator_contract(
    const RadialDomainContract &domain)
{
    return FrozenGaugeOperatorContract(domain);
}

inline FrozenGaugeOperatorContract make_default_frozen_gauge_operator_contract()
{
    // Matches the Stage 4AO-B provisional local validation domain. 4AO-C must
    // still test boundary-location convergence and must not treat these values
    // as a final spectral-domain choice.
    return make_frozen_gauge_operator_contract(
        make_radial_domain_contract(1.0, 0.5, 4.0));
}

} // namespace Stage4AOFrozenGaugeOperator
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_OPERATOR_HPP */
