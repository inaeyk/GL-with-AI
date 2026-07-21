#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_CONTRACTED_CONNECTION_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_CONTRACTED_CONNECTION_HPP

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeContractedConnection
{
// Stage 4AO-C validation-only linearized contracted-connection and encoded-Z
// reconstruction helper. This is not a Gamma RHS block and writes no evolved
// output. In particular it contains no damping, connection-A, shift-Hessian,
// K/Theta/chi-gradient, advection, or complete Gamma-evolution term.
static constexpr bool validation_only = true;
static constexpr int hidden_multiplicity = 2;
static constexpr bool contracted_connection_and_z_reconstruction_implemented =
    true;
static constexpr bool gamma_rhs_output_implemented = false;
static constexpr bool gamma_damping_implemented = false;
static constexpr bool connection_a_implemented = false;
static constexpr bool vector_hessian_implemented = false;
static constexpr bool k_theta_chi_gradients_implemented = false;
static constexpr bool complete_gamma_evolution_implemented = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_wiring_implemented = false;

class PerturbationJet
{
  public:
    double x() const { return m_x; }
    double h_xx() const { return m_h_xx; }
    double h_xz() const { return m_h_xz; }
    double h_zz() const { return m_h_zz; }
    double h_ww() const { return m_h_ww; }
    double dx_h_xx() const { return m_dx_h_xx; }
    double dx_h_xz() const { return m_dx_h_xz; }
    double dx_h_zz() const { return m_dx_h_zz; }
    double dx_h_ww() const { return m_dx_h_ww; }
    double dz_h_xx() const { return m_dz_h_xx; }
    double dz_h_xz() const { return m_dz_h_xz; }
    double dz_h_zz() const { return m_dz_h_zz; }
    double dz_h_ww() const { return m_dz_h_ww; }
    double hat_gamma_x() const { return m_hat_gamma_x; }
    double hat_gamma_z() const { return m_hat_gamma_z; }

  private:
    PerturbationJet(const double x, const double h_xx, const double h_xz,
                    const double h_zz, const double h_ww,
                    const double dx_h_xx, const double dx_h_xz,
                    const double dx_h_zz, const double dx_h_ww,
                    const double dz_h_xx, const double dz_h_xz,
                    const double dz_h_zz, const double dz_h_ww,
                    const double hat_gamma_x, const double hat_gamma_z)
        : m_x(x), m_h_xx(h_xx), m_h_xz(h_xz), m_h_zz(h_zz), m_h_ww(h_ww),
          m_dx_h_xx(dx_h_xx), m_dx_h_xz(dx_h_xz), m_dx_h_zz(dx_h_zz),
          m_dx_h_ww(dx_h_ww), m_dz_h_xx(dz_h_xx), m_dz_h_xz(dz_h_xz),
          m_dz_h_zz(dz_h_zz), m_dz_h_ww(dz_h_ww),
          m_hat_gamma_x(hat_gamma_x), m_hat_gamma_z(hat_gamma_z)
    {
    }

    friend PerturbationJet make_perturbation_jet(
        double x, double h_xx, double h_xz, double h_zz, double h_ww,
        double dx_h_xx, double dx_h_xz, double dx_h_zz, double dx_h_ww,
        double dz_h_xx, double dz_h_xz, double dz_h_zz, double dz_h_ww,
        double hat_gamma_x, double hat_gamma_z);

    double m_x;
    double m_h_xx;
    double m_h_xz;
    double m_h_zz;
    double m_h_ww;
    double m_dx_h_xx;
    double m_dx_h_xz;
    double m_dx_h_zz;
    double m_dx_h_ww;
    double m_dz_h_xx;
    double m_dz_h_xz;
    double m_dz_h_zz;
    double m_dz_h_ww;
    double m_hat_gamma_x;
    double m_hat_gamma_z;
};

inline PerturbationJet make_perturbation_jet(
    const double x, const double h_xx, const double h_xz, const double h_zz,
    const double h_ww, const double dx_h_xx, const double dx_h_xz,
    const double dx_h_zz, const double dx_h_ww, const double dz_h_xx,
    const double dz_h_xz, const double dz_h_zz, const double dz_h_ww,
    const double hat_gamma_x, const double hat_gamma_z)
{
    const double values[] = {x,          h_xx,        h_xz,        h_zz,
                             h_ww,       dx_h_xx,     dx_h_xz,     dx_h_zz,
                             dx_h_ww,    dz_h_xx,     dz_h_xz,     dz_h_zz,
                             dz_h_ww,    hat_gamma_x, hat_gamma_z};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "Stage 4AO-C contracted connection requires finite inputs");
        }
    }
    if (!(x > 0.0))
    {
        throw std::domain_error(
            "Stage 4AO-C contracted connection requires x>0");
    }
    return PerturbationJet(x, h_xx, h_xz, h_zz, h_ww, dx_h_xx, dx_h_xz,
                           dx_h_zz, dx_h_ww, dz_h_xx, dz_h_xz, dz_h_zz,
                           dz_h_ww, hat_gamma_x, hat_gamma_z);
}

class ContractedConnectionAndZ
{
  public:
    double g_x() const { return m_g_x; }
    double g_z() const { return m_g_z; }
    double z_x() const { return m_z_x; }
    double z_z() const { return m_z_z; }

  private:
    ContractedConnectionAndZ(const double g_x, const double g_z,
                             const double z_x, const double z_z)
        : m_g_x(g_x), m_g_z(g_z), m_z_x(z_x), m_z_z(z_z)
    {
    }

    friend ContractedConnectionAndZ
    compute_contracted_connection_and_z(const PerturbationJet &input);

    double m_g_x;
    double m_g_z;
    double m_z_x;
    double m_z_z;
};

inline ContractedConnectionAndZ
compute_contracted_connection_and_z(const PerturbationJet &input)
{
    const double hidden_x =
        static_cast<double>(hidden_multiplicity) *
        (input.h_xx() - input.h_ww()) / input.x();
    const double hidden_z =
        static_cast<double>(hidden_multiplicity) * input.h_xz() / input.x();

    // Linearization of the general Stage 4AN full x contraction about the
    // flat conformal GP metric. No determinant constraint is used.
    const double g_x = 0.5 * input.dx_h_xx() - 0.5 * input.dx_h_zz() -
                       input.dx_h_ww() + input.dz_h_xz() + hidden_x;

    // Full hidden-aware z companion. The hidden contribution comes from two
    // copies of delta Gamma^z_ww = delta h_xz/x - 0.5 dz(delta h_ww).
    const double g_z = input.dx_h_xz() + hidden_z -
                       0.5 * input.dz_h_xx() +
                       0.5 * input.dz_h_zz() - input.dz_h_ww();

    const double z_x = 0.5 * (input.hat_gamma_x() - g_x);
    const double z_z = 0.5 * (input.hat_gamma_z() - g_z);
    return ContractedConnectionAndZ(g_x, g_z, z_x, z_z);
}

} // namespace Stage4AOFrozenGaugeContractedConnection
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_CONTRACTED_CONNECTION_HPP */
