#include "Stage4AOFrozenGaugeOperator.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace
{
namespace Operator = BlackStringToy::Stage4AOFrozenGaugeOperator;

using Variable = Operator::PerturbationVariable;
using Gauge = Operator::FrozenGaugeVariable;
using Piece = Operator::RhsPiece;
using Status = Operator::PieceStatus;
using Domain = Operator::RadialDomainContract;
using Contract = Operator::FrozenGaugeOperatorContract;

constexpr std::array<Variable, 13> expected_state_order = {
    Variable::chi,         Variable::h_xx,        Variable::h_xz,
    Variable::h_zz,        Variable::h_ww,        Variable::K,
    Variable::A_xx,        Variable::A_xz,        Variable::A_zz,
    Variable::A_ww,        Variable::Theta,       Variable::hat_Gamma_x,
    Variable::hat_Gamma_z};

constexpr std::array<Piece, 23> all_rhs_pieces = {
    Piece::gp_shift_advection,
    Piece::tensor_shift_stretching,
    Piece::algebraic_metric_chi_coupling,
    Piece::k_equation_ccz4_k_theta,
    Piece::k_equation_ricci_scalar_insertion,
    Piece::ccz4_k_theta_damping_insertion,
    Piece::hat_gamma_z4_kappa_shift_gradient_insertion,
    Piece::hat_gamma_k_theta_chi_gradient_insertion,
    Piece::hat_gamma_connection_a_insertion,
    Piece::hat_gamma_vector_hessian_insertion,
    Piece::hat_gamma_grad_div_insertion,
    Piece::a_equation_algebraic_non_curvature,
    Piece::theta_equation_algebraic_non_ricci,
    Piece::theta_equation_minus_k_delta_theta,
    Piece::theta_ricci_scalar_insertion,
    Piece::a_equation_ricci_curvature_insertion,
    Piece::radial_derivatives,
    Piece::z_derivatives,
    Piece::hidden_sphere_terms,
    Piece::ricci_curvature_terms,
    Piece::k_a_trace_trace_free_terms,
    Piece::theta_constraint_terms,
    Piece::hat_gamma_hidden_evolution_terms};

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

template <class Function> void require_domain_error(const std::string &label,
                                                    Function &&function)
{
    try
    {
        function();
    }
    catch (const std::domain_error &)
    {
        std::cout << "PASS " << label << " rejected\n";
        return;
    }
    catch (const std::exception &exception)
    {
        fail(label, std::string("wrong exception: ") + exception.what());
    }
    fail(label, "expected std::domain_error");
}

template <class Value, std::size_t Size>
bool contains(const std::array<Value, Size> &values, const Value expected)
{
    for (const auto value : values)
    {
        if (value == expected)
        {
            return true;
        }
    }
    return false;
}

void require_status(const std::string &label, const Variable variable,
                    const Piece piece, const Status expected)
{
    const auto status = Operator::rhs_piece_status(variable, piece);
    if (status != expected)
    {
        fail(label, std::string("got ") +
                        Operator::piece_status_name(status) + ", expected " +
                        Operator::piece_status_name(expected));
    }
    std::cout << "PASS " << label << " = "
              << Operator::piece_status_name(status) << "\n";
}

void check_state_vector_and_gauge_freeze()
{
    static_assert(Operator::frozen_gauge_state_vector.size() == 13,
                  "Stage 4AO-C frozen state must have 13 variables");
    static_assert(expected_state_order.size() == 13,
                  "Stage 4AO-C expected state order must have 13 variables");
    static_assert(Operator::excluded_gauge_perturbations.size() == 5,
                  "Stage 4AO-C freezes alpha, beta^i, and B^i");
    static_assert(!std::is_aggregate<Domain>::value,
                  "Stage 4AO-C domain contract must not be an open aggregate");
    static_assert(!std::is_aggregate<Contract>::value,
                  "Stage 4AO-C operator contract must not be an open "
                  "aggregate");

    for (std::size_t i = 0; i < expected_state_order.size(); ++i)
    {
        if (Operator::frozen_gauge_state_vector[i] != expected_state_order[i])
        {
            fail("exact state-vector order",
                 "slot " + std::to_string(i) + " got " +
                     Operator::variable_name(
                         Operator::frozen_gauge_state_vector[i]) +
                     ", expected " +
                     Operator::variable_name(expected_state_order[i]));
        }
    }
    std::cout << "PASS exact state-vector order: 0 chi, 1 h_xx, 2 h_xz, "
                 "3 h_zz, 4 h_ww, 5 K, 6 A_xx, 7 A_xz, 8 A_zz, "
                 "9 A_ww, 10 Theta, 11 hat_Gamma_x, 12 hat_Gamma_z\n";

    require_true("gauge freezes alpha",
                 contains(Operator::excluded_gauge_perturbations,
                          Gauge::alpha));
    require_true("gauge freezes beta^x",
                 contains(Operator::excluded_gauge_perturbations,
                          Gauge::beta_x));
    require_true("gauge freezes beta^z",
                 contains(Operator::excluded_gauge_perturbations,
                          Gauge::beta_z));
    require_true("gauge freezes B^x",
                 contains(Operator::excluded_gauge_perturbations, Gauge::B_x));
    require_true("gauge freezes B^z",
                 contains(Operator::excluded_gauge_perturbations, Gauge::B_z));
}

void check_domain_and_boundary_contract()
{
    const auto domain = Operator::make_radial_domain_contract(1.0, 0.5, 4.0);
    require_true("domain excludes x=0", domain.excludes_axis());
    require_true("domain brackets r0", domain.brackets_horizon());
    require_true("z direction is periodic", domain.z_periodic());

    require_domain_error("domain rejects x_in=0", []() {
        (void)Operator::make_radial_domain_contract(1.0, 0.0, 4.0);
    });
    require_domain_error("domain rejects horizon outside interval", []() {
        (void)Operator::make_radial_domain_contract(1.0, 1.5, 4.0);
    });

    const auto boundary_contracts = Operator::boundary_contracts();
    require_true("boundary contract has inner/outer/sector entries",
                 boundary_contracts.size() == 5);
    for (const auto &boundary : boundary_contracts)
    {
        require_true("boundary tests are not yet implemented",
                     !boundary.validation_test_implemented);
    }
}

void check_rhs_inventory()
{
    // The GP-shift advection piece is now a real partial operator block.
    // Tensor shift-stretching is implemented for h_IJ and A_IJ slots, and the
    // local algebraic h<-A / chi<-K block is implemented for h_IJ and chi
    // outputs. The K-equation CCZ4 K/Theta and physical-Ricci scalar pieces
    // are implemented only as K-output blocks. The simple locked-convention
    // kappa damping insertion is implemented only for K and Theta outputs;
    // the first non-advection hatted-Gamma Z/kappa plus kappa3
    // shift-gradient insertion is implemented only for hatted-Gamma outputs.
    // The separate K/Theta/chi gradient, connection-A, vector-Hessian, and
    // grad-div metric insertions are likewise implemented only for
    // hatted-Gamma outputs.
    // The rejected BSSN
    // A^2+K^2/d row is absent. The A-equation non-curvature algebraic block is
    // implemented only for A_IJ outputs. The Theta-equation non-Ricci
    // algebraic block and the -K_GP deltaTheta block are implemented only for
    // the Theta output. The Theta Ricci scalar insertion is also implemented
    // only for the Theta output, through the separate Ricci assembly helper.
    // The A-equation Ricci curvature insertion is implemented only for A_IJ
    // outputs through the same reviewed Ricci trace-free assembly.
    // Everything else remains inventory or helper coverage, not a complete
    // coupled modified-cartoon RHS.
    require_status("chi advection block is implemented", Variable::chi,
                   Piece::gp_shift_advection, Status::implemented_now);
    require_status("h_xx tensor stretching block is implemented",
                   Variable::h_xx, Piece::tensor_shift_stretching,
                   Status::implemented_now);
    require_status("A_ww tensor stretching block is implemented",
                   Variable::A_ww, Piece::tensor_shift_stretching,
                   Status::implemented_now);
    require_status("chi has no tensor stretching", Variable::chi,
                   Piece::tensor_shift_stretching, Status::not_applicable);
    require_status("hat_Gamma^x tensor stretching is not this block",
                   Variable::hat_Gamma_x, Piece::tensor_shift_stretching,
                   Status::not_applicable);
    require_status("chi algebraic coupling is implemented", Variable::chi,
                   Piece::algebraic_metric_chi_coupling,
                   Status::implemented_now);
    require_status("h_ww algebraic coupling is implemented", Variable::h_ww,
                   Piece::algebraic_metric_chi_coupling,
                   Status::implemented_now);
    require_status("A_ww has no reciprocal algebraic coupling",
                   Variable::A_ww, Piece::algebraic_metric_chi_coupling,
                   Status::not_applicable);
    require_status("K output has no algebraic coupling from this block",
                   Variable::K, Piece::algebraic_metric_chi_coupling,
                   Status::not_applicable);
    require_status("hat_Gamma^z algebraic coupling is not this block",
                   Variable::hat_Gamma_z,
                   Piece::algebraic_metric_chi_coupling,
                   Status::not_applicable);
    require_status("K CCZ4 K/Theta block is implemented", Variable::K,
                   Piece::k_equation_ccz4_k_theta,
                   Status::implemented_now);
    require_status("chi has no K CCZ4 K/Theta output", Variable::chi,
                   Piece::k_equation_ccz4_k_theta,
                   Status::not_applicable);
    require_status("A_ww has no K CCZ4 K/Theta output", Variable::A_ww,
                   Piece::k_equation_ccz4_k_theta,
                   Status::not_applicable);
    require_status("K Ricci scalar insertion is implemented", Variable::K,
                   Piece::k_equation_ricci_scalar_insertion,
                   Status::implemented_now);
    require_status("Theta has no K Ricci scalar output", Variable::Theta,
                   Piece::k_equation_ricci_scalar_insertion,
                   Status::not_applicable);
    require_status("K CCZ4 damping insertion is implemented", Variable::K,
                   Piece::ccz4_k_theta_damping_insertion,
                   Status::implemented_now);
    require_status("Theta CCZ4 damping insertion is implemented",
                   Variable::Theta,
                   Piece::ccz4_k_theta_damping_insertion,
                   Status::implemented_now);
    require_status("A_ww has no K/Theta damping output", Variable::A_ww,
                   Piece::ccz4_k_theta_damping_insertion,
                   Status::not_applicable);
    require_status("hat_Gamma^x Z/kappa/shift-gradient insertion implemented",
                   Variable::hat_Gamma_x,
                   Piece::hat_gamma_z4_kappa_shift_gradient_insertion,
                   Status::implemented_now);
    require_status("hat_Gamma^z Z/kappa insertion implemented",
                   Variable::hat_Gamma_z,
                   Piece::hat_gamma_z4_kappa_shift_gradient_insertion,
                   Status::implemented_now);
    require_status("K has no hatted-Gamma partial-block output", Variable::K,
                   Piece::hat_gamma_z4_kappa_shift_gradient_insertion,
                   Status::not_applicable);
    require_status("hat_Gamma^x K/Theta/chi gradients implemented",
                   Variable::hat_Gamma_x,
                   Piece::hat_gamma_k_theta_chi_gradient_insertion,
                   Status::implemented_now);
    require_status("hat_Gamma^z K/Theta/chi gradients implemented",
                   Variable::hat_Gamma_z,
                   Piece::hat_gamma_k_theta_chi_gradient_insertion,
                   Status::implemented_now);
    require_status("Theta has no hatted-Gamma gradient-block output",
                   Variable::Theta,
                   Piece::hat_gamma_k_theta_chi_gradient_insertion,
                   Status::not_applicable);
    require_status("hat_Gamma^x connection-A block implemented",
                   Variable::hat_Gamma_x,
                   Piece::hat_gamma_connection_a_insertion,
                   Status::implemented_now);
    require_status("hat_Gamma^z connection-A block implemented",
                   Variable::hat_Gamma_z,
                   Piece::hat_gamma_connection_a_insertion,
                   Status::implemented_now);
    require_status("A_xx has no connection-A-block output", Variable::A_xx,
                   Piece::hat_gamma_connection_a_insertion,
                   Status::not_applicable);
    require_status("hat_Gamma^x vector Hessian implemented",
                   Variable::hat_Gamma_x,
                   Piece::hat_gamma_vector_hessian_insertion,
                   Status::implemented_now);
    require_status("hat_Gamma^z vector Hessian zero row is implemented",
                   Variable::hat_Gamma_z,
                   Piece::hat_gamma_vector_hessian_insertion,
                   Status::implemented_now);
    require_status("K has no vector-Hessian-block output", Variable::K,
                   Piece::hat_gamma_vector_hessian_insertion,
                   Status::not_applicable);
    require_status("hat_Gamma^x grad-div block implemented",
                   Variable::hat_Gamma_x,
                   Piece::hat_gamma_grad_div_insertion,
                   Status::implemented_now);
    require_status("hat_Gamma^z grad-div block implemented",
                   Variable::hat_Gamma_z,
                   Piece::hat_gamma_grad_div_insertion,
                   Status::implemented_now);
    require_status("h_xz has no grad-div-block output", Variable::h_xz,
                   Piece::hat_gamma_grad_div_insertion,
                   Status::not_applicable);
    require_status("A_xx algebraic non-curvature block is implemented",
                   Variable::A_xx,
                   Piece::a_equation_algebraic_non_curvature,
                   Status::implemented_now);
    require_status("A_ww algebraic non-curvature block is implemented",
                   Variable::A_ww,
                   Piece::a_equation_algebraic_non_curvature,
                   Status::implemented_now);
    require_status("K has no A algebraic non-curvature output", Variable::K,
                   Piece::a_equation_algebraic_non_curvature,
                   Status::not_applicable);
    require_status("h_xx has no A algebraic non-curvature output",
                   Variable::h_xx,
                   Piece::a_equation_algebraic_non_curvature,
                   Status::not_applicable);
    require_status("Theta non-Ricci algebraic block is implemented",
                   Variable::Theta,
                   Piece::theta_equation_algebraic_non_ricci,
                   Status::implemented_now);
    require_status("K has no Theta non-Ricci algebraic output", Variable::K,
                   Piece::theta_equation_algebraic_non_ricci,
                   Status::not_applicable);
    require_status("A_ww has no Theta non-Ricci algebraic output",
                   Variable::A_ww,
                   Piece::theta_equation_algebraic_non_ricci,
                   Status::not_applicable);
    require_status("Theta minus-K block is implemented",
                   Variable::Theta,
                   Piece::theta_equation_minus_k_delta_theta,
                   Status::implemented_now);
    require_status("K has no Theta minus-K output", Variable::K,
                   Piece::theta_equation_minus_k_delta_theta,
                   Status::not_applicable);
    require_status("h_xx has no Theta minus-K output", Variable::h_xx,
                   Piece::theta_equation_minus_k_delta_theta,
                   Status::not_applicable);
    require_status("Theta Ricci scalar insertion is implemented",
                   Variable::Theta, Piece::theta_ricci_scalar_insertion,
                   Status::implemented_now);
    require_status("A_xx has no Theta Ricci scalar output", Variable::A_xx,
                   Piece::theta_ricci_scalar_insertion,
                   Status::not_applicable);
    require_status("K has no Theta Ricci scalar output", Variable::K,
                   Piece::theta_ricci_scalar_insertion,
                   Status::not_applicable);
    require_status("A_xx Ricci curvature insertion is implemented",
                   Variable::A_xx,
                   Piece::a_equation_ricci_curvature_insertion,
                   Status::implemented_now);
    require_status("A_xz Ricci curvature insertion is implemented",
                   Variable::A_xz,
                   Piece::a_equation_ricci_curvature_insertion,
                   Status::implemented_now);
    require_status("A_ww Ricci curvature insertion is implemented",
                   Variable::A_ww,
                   Piece::a_equation_ricci_curvature_insertion,
                   Status::implemented_now);
    require_status("Theta has no A Ricci curvature output", Variable::Theta,
                   Piece::a_equation_ricci_curvature_insertion,
                   Status::not_applicable);
    require_status("K has no A Ricci curvature output", Variable::K,
                   Piece::a_equation_ricci_curvature_insertion,
                   Status::not_applicable);
    require_status("h_xx has no A Ricci curvature output", Variable::h_xx,
                   Piece::a_equation_ricci_curvature_insertion,
                   Status::not_applicable);
    require_status("chi radial derivatives are helper-only scaffolding",
                   Variable::chi, Piece::radial_derivatives,
                   Status::reusable_helper);
    require_status("chi z derivatives are helper-only scaffolding",
                   Variable::chi, Piece::z_derivatives,
                   Status::reusable_helper);
    require_status("h_ww hidden terms have helper coverage only",
                   Variable::h_ww, Piece::hidden_sphere_terms,
                   Status::reusable_helper);
    require_status("A_ww curvature terms have helper coverage only",
                   Variable::A_ww, Piece::ricci_curvature_terms,
                   Status::reusable_helper);
    require_status("remaining K Ricci/Z path has helper coverage only",
                   Variable::K, Piece::ricci_curvature_terms,
                   Status::reusable_helper);
    require_status("hat_Gamma^x hidden evolution has helper coverage only",
                   Variable::hat_Gamma_x,
                   Piece::hat_gamma_hidden_evolution_terms,
                   Status::reusable_helper);
    require_status("hat_Gamma^z hidden terms have helper coverage only",
                   Variable::hat_Gamma_z, Piece::hidden_sphere_terms,
                   Status::reusable_helper);
    require_status("hat_Gamma^z contracted-connection helper is reusable",
                   Variable::hat_Gamma_z,
                   Piece::hat_gamma_hidden_evolution_terms,
                   Status::reusable_helper);
    require_status("Theta constraint terms need actual RHS", Variable::Theta,
                   Piece::theta_constraint_terms,
                   Status::requires_modified_cartoon_rhs);

    require_true("no frozen-gauge RHS variable is complete",
                 !Operator::any_variable_rhs_complete());

    for (const auto variable : Operator::frozen_gauge_state_vector)
    {
        for (const auto piece : all_rhs_pieces)
        {
            const auto status = Operator::rhs_piece_status(variable, piece);
            if (piece == Piece::gp_shift_advection)
            {
                if (status != Status::implemented_now)
                {
                    fail("GP-shift advection implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark the advection block implemented");
                }
            }
            else if (piece == Piece::tensor_shift_stretching)
            {
                const bool should_be_implemented =
                    Operator::receives_tensor_shift_stretching(variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("tensor stretching implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark tensor stretching implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("tensor stretching scalar/vector guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks tensor stretching "
                             "implemented");
                }
            }
            else if (piece == Piece::algebraic_metric_chi_coupling)
            {
                const bool should_be_implemented =
                    Operator::receives_algebraic_metric_chi_coupling(variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("algebraic coupling implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark algebraic coupling implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("algebraic coupling scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks algebraic coupling "
                             "implemented");
                }
            }
            else if (piece == Piece::k_equation_ccz4_k_theta)
            {
                const bool should_be_implemented =
                    Operator::receives_k_equation_ccz4_k_theta(variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("K CCZ4 K/Theta implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark K/Theta coupling "
                             "implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("K CCZ4 K/Theta scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks K/Theta coupling "
                             "implemented");
                }
            }
            else if (piece == Piece::k_equation_ricci_scalar_insertion)
            {
                const bool should_be_implemented =
                    Operator::receives_k_equation_ricci_scalar_insertion(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("K Ricci insertion implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark K Ricci insertion implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("K Ricci insertion scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks K Ricci insertion "
                             "implemented");
                }
            }
            else if (piece == Piece::ccz4_k_theta_damping_insertion)
            {
                const bool should_be_implemented =
                    Operator::receives_ccz4_k_theta_damping_insertion(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("K/Theta damping implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark K/Theta damping implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("K/Theta damping scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks K/Theta damping implemented");
                }
            }
            else if (
                piece ==
                Piece::hat_gamma_z4_kappa_shift_gradient_insertion)
            {
                const bool should_be_implemented =
                    Operator::
                        receives_hat_gamma_z4_kappa_shift_gradient_insertion(
                            variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("hat-Gamma partial block implemented guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark the Z/kappa/shift-gradient "
                             "insertion implemented");
                }
                if (!should_be_implemented &&
                    status != Status::not_applicable)
                {
                    fail("hat-Gamma partial block scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly receives hatted-Gamma output");
                }
            }
            else if (
                piece ==
                Piece::hat_gamma_k_theta_chi_gradient_insertion)
            {
                const bool should_be_implemented =
                    Operator::
                        receives_hat_gamma_k_theta_chi_gradient_insertion(
                            variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("hat-Gamma gradient block implemented guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark K/Theta/chi gradients "
                             "implemented");
                }
                if (!should_be_implemented &&
                    status != Status::not_applicable)
                {
                    fail("hat-Gamma gradient block scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly receives hatted-Gamma gradients");
                }
            }
            else if (piece == Piece::hat_gamma_connection_a_insertion)
            {
                const bool should_be_implemented =
                    Operator::receives_hat_gamma_connection_a_insertion(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("hat-Gamma connection-A block implemented guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark the connection-A metric "
                             "insertion implemented");
                }
                if (!should_be_implemented &&
                    status != Status::not_applicable)
                {
                    fail("hat-Gamma connection-A block scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly receives connection-A output");
                }
            }
            else if (piece == Piece::hat_gamma_vector_hessian_insertion)
            {
                const bool should_be_implemented =
                    Operator::receives_hat_gamma_vector_hessian_insertion(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("hat-Gamma vector-Hessian implemented guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark vector Hessian implemented");
                }
                if (!should_be_implemented &&
                    status != Status::not_applicable)
                {
                    fail("hat-Gamma vector-Hessian scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly receives vector Hessian output");
                }
            }
            else if (piece == Piece::hat_gamma_grad_div_insertion)
            {
                const bool should_be_implemented =
                    Operator::receives_hat_gamma_grad_div_insertion(variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("hat-Gamma grad-div implemented guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark grad-div implemented");
                }
                if (!should_be_implemented &&
                    status != Status::not_applicable)
                {
                    fail("hat-Gamma grad-div scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly receives grad-div output");
                }
            }
            else if (piece == Piece::a_equation_algebraic_non_curvature)
            {
                const bool should_be_implemented =
                    Operator::receives_a_equation_algebraic_non_curvature(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("A algebraic implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark A algebraic coupling "
                             "implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("A algebraic scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks A algebraic coupling "
                             "implemented");
                }
            }
            else if (piece == Piece::theta_equation_algebraic_non_ricci)
            {
                const bool should_be_implemented =
                    Operator::receives_theta_equation_algebraic_non_ricci(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("Theta algebraic implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark Theta algebraic coupling "
                             "implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("Theta algebraic scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks Theta algebraic coupling "
                             "implemented");
                }
            }
            else if (piece == Piece::theta_equation_minus_k_delta_theta)
            {
                const bool should_be_implemented =
                    Operator::receives_theta_equation_minus_k_delta_theta(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("Theta minus-K implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark Theta minus-K coupling "
                             "implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("Theta minus-K scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks Theta minus-K coupling "
                             "implemented");
                }
            }
            else if (piece == Piece::theta_ricci_scalar_insertion)
            {
                const bool should_be_implemented =
                    Operator::receives_theta_ricci_scalar_insertion(variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("Theta Ricci insertion implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark Theta Ricci insertion "
                             "implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("Theta Ricci insertion scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks Theta Ricci insertion "
                             "implemented");
                }
            }
            else if (piece == Piece::a_equation_ricci_curvature_insertion)
            {
                const bool should_be_implemented =
                    Operator::receives_a_equation_ricci_curvature_insertion(
                        variable);
                if (should_be_implemented &&
                    status != Status::implemented_now)
                {
                    fail("A Ricci insertion implemented_now guard",
                         std::string(Operator::variable_name(variable)) +
                             " does not mark A Ricci insertion implemented");
                }
                if (!should_be_implemented &&
                    status == Status::implemented_now)
                {
                    fail("A Ricci insertion scope guard",
                         std::string(Operator::variable_name(variable)) +
                             " unexpectedly marks A Ricci insertion "
                             "implemented");
                }
            }
            else if (status == Status::implemented_now)
            {
                fail("full RHS inventory implemented_now guard",
                     std::string(Operator::variable_name(variable)) +
                         " unexpectedly marked a non-advection RHS piece "
                         "implemented");
            }
        }
    }
    std::cout << "PASS only GP-shift advection, tensor stretching, "
                 "algebraic metric/chi, K CCZ4 K/Theta, K Ricci scalar, "
                 "K/Theta damping, hat-Gamma Z/kappa/shift-gradient, "
                 "hat-Gamma K/Theta/chi gradients, "
                 "hat-Gamma connection-A metric insertion, "
                 "hat-Gamma vector-Hessian and grad-div metric insertions, "
                 "A algebraic "
                 "non-curvature, Theta algebraic non-Ricci, Theta minus-K, "
                 "Theta Ricci scalar insertion, and A Ricci curvature "
                 "insertion blocks are "
                 "implemented_now\n";
}

void check_operator_completion_guard()
{
    const auto contract = Operator::make_default_frozen_gauge_operator_contract();
    require_true("contract uses provisional Stage 4AO-B domain",
                 contract.domain().r0() == 1.0 &&
                     contract.domain().x_in() == 0.5 &&
                     contract.domain().x_out() == 4.0);
    require_true("wrapper is validation only", Operator::validation_only);
    require_true("GP-shift advection block is implemented",
                 Operator::gp_shift_advection_block_implemented);
    require_true("tensor shift-stretching block is implemented",
                 Operator::tensor_shift_stretching_block_implemented);
    require_true("algebraic metric/chi coupling block is implemented",
                 Operator::algebraic_metric_chi_coupling_block_implemented);
    require_true("K CCZ4 K/Theta block is implemented",
                 Operator::k_equation_ccz4_k_theta_block_implemented);
    require_true("K Ricci scalar insertion block is implemented",
                 Operator::k_equation_ricci_scalar_insertion_block_implemented);
    require_true("K Z/hat-Gamma Ricci contributions remain missing",
                 !Operator::k_equation_z_ricci_contributions_implemented);
    require_true("main CCZ4 damping convention is locked",
                 Operator::main_ccz4_damping_convention_locked);
    require_true("simple K/Theta damping insertion is implemented",
                 Operator::ccz4_k_theta_damping_insertion_block_implemented);
    require_true(
        "contracted-connection and Z reconstruction helper is implemented",
        Operator::
            contracted_connection_and_z_reconstruction_helper_implemented);
    require_true("complete hatted-Gamma RHS remains missing",
                 !Operator::hat_gamma_rhs_block_implemented);
    require_true("first hat-Gamma Z/kappa/shift-gradient block implemented",
                 Operator::
                     hat_gamma_z4_kappa_shift_gradient_block_implemented);
    require_true("hat-Gamma K/Theta/chi gradient block implemented",
                 Operator::
                     hat_gamma_k_theta_chi_gradient_block_implemented);
    require_true("hat-Gamma connection-A block implemented",
                 Operator::hat_gamma_connection_a_block_implemented);
    require_true("hat-Gamma vector-Hessian block implemented",
                 Operator::hat_gamma_vector_hessian_block_implemented);
    require_true("hat-Gamma grad-div block implemented",
                 Operator::hat_gamma_grad_div_block_implemented);
    require_true("Gamma mathematical term-family inventory is closed",
                 Operator::hat_gamma_surviving_term_family_inventory_closed);
    require_true("Gamma final row assembly remains missing",
                 !Operator::hat_gamma_final_row_assembly_implemented);
    require_true("assembled Gamma row validation remains missing",
                 !Operator::hat_gamma_assembled_row_validation_implemented);
    require_true("K lapse Hessian vanishes in frozen gauge",
                 Operator::k_equation_lapse_hessian_vanishes_in_frozen_gauge);
    require_true("cosmological constant remains locked to zero",
                 Operator::cosmological_constant_locked_to_zero);
    require_true("no cosmological K block is implemented",
                 !Operator::k_equation_cosmological_terms_implemented);
    require_true("A algebraic non-curvature block is implemented",
                 Operator::a_equation_algebraic_non_curvature_block_implemented);
    require_true(
        "Theta algebraic non-Ricci block is implemented",
        Operator::theta_equation_algebraic_non_ricci_block_implemented);
    require_true(
        "Theta minus-K block is implemented",
        Operator::theta_equation_minus_k_delta_theta_block_implemented);
    require_true(
        "Theta Ricci scalar insertion block is implemented",
        Operator::theta_ricci_scalar_insertion_block_implemented);
    require_true(
        "A Ricci curvature insertion block is implemented",
        Operator::a_equation_ricci_curvature_insertion_block_implemented);
    require_true("trace-free delta A projector contract is implemented",
                 Operator::trace_free_delta_a_projector_contract_implemented);
    require_true("boundary derivative validation remains missing",
                 !Operator::boundary_derivative_validation_implemented);
    require_true("complete frozen-gauge operator is not implemented",
                 !contract.complete_operator_implemented());
    require_true("eigensolver is not implemented",
                 !Operator::eigensolver_implemented);
    require_true("shift-invert is not implemented",
                 !Operator::shift_invert_implemented);
    require_true("threshold search is not implemented",
                 !Operator::threshold_search_implemented);
    require_true("production RHS wiring is not implemented",
                 !Operator::production_rhs_wiring_implemented);
    require_true("eigensolver is not allowed by this contract",
                 !contract.eigensolver_allowed());
    require_true("next validation hooks are explicit",
                 Operator::next_validation_hooks.size() == 7);
}

} // namespace

int main()
{
    check_state_vector_and_gauge_freeze();
    check_domain_and_boundary_contract();
    check_rhs_inventory();
    check_operator_completion_guard();
    std::cout << "PASS Stage 4AO-C frozen-gauge operator contract\n";
    return 0;
}
