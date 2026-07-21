#ifndef STAGE4AO_FROZEN_GAUGE_Z_RICCI_COMPLETION_HPP_
#define STAGE4AO_FROZEN_GAUGE_Z_RICCI_COMPLETION_HPP_

#include <cmath>
#include <stdexcept>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeZRicciCompletion
{
constexpr int physical_spatial_dimension = 4;
constexpr int hidden_ww_multiplicity = 2;

constexpr bool validation_only = true;
constexpr bool linearized_encoded_z_ricci_completion_implemented = true;
constexpr bool geometric_ricci_implemented = false;
constexpr bool z_definition_derivative_expansion_implemented = false;
constexpr bool k_row_insertion_implemented = false;
constexpr bool theta_row_insertion_implemented = false;
constexpr bool a_row_insertion_implemented = false;
constexpr bool complete_non_gamma_rhs_implemented = false;
constexpr bool full_frozen_gauge_operator_implemented = false;
constexpr bool eigensolver_implemented = false;
constexpr bool nonlinear_selected_branch_oracle_implemented = false;

class EncodedZDerivativeJet
{
  public:
    double x() const { return m_x; }
    double z_x() const { return m_z_x; }
    double z_z() const { return m_z_z; }
    double dx_z_x() const { return m_dx_z_x; }
    double dz_z_x() const { return m_dz_z_x; }
    double dx_z_z() const { return m_dx_z_z; }
    double dz_z_z() const { return m_dz_z_z; }

  private:
    EncodedZDerivativeJet(const double a_x, const double a_z_x,
                          const double a_z_z, const double a_dx_z_x,
                          const double a_dz_z_x, const double a_dx_z_z,
                          const double a_dz_z_z)
        : m_x(a_x), m_z_x(a_z_x), m_z_z(a_z_z), m_dx_z_x(a_dx_z_x),
          m_dz_z_x(a_dz_z_x), m_dx_z_z(a_dx_z_z), m_dz_z_z(a_dz_z_z)
    {
    }

    double m_x;
    double m_z_x;
    double m_z_z;
    double m_dx_z_x;
    double m_dz_z_x;
    double m_dx_z_z;
    double m_dz_z_z;

    friend EncodedZDerivativeJet
    make_encoded_z_derivative_jet(double, double, double, double, double,
                                  double, double);
};

inline EncodedZDerivativeJet
make_encoded_z_derivative_jet(const double x, const double z_x,
                              const double z_z, const double dx_z_x,
                              const double dz_z_x, const double dx_z_z,
                              const double dz_z_z)
{
    const double values[] = {x,      z_x,    z_z,     dx_z_x,
                             dz_z_x, dx_z_z, dz_z_z};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::invalid_argument(
                "encoded-Z derivative jet entries must be finite");
        }
    }
    if (!(x > 0.0))
    {
        throw std::invalid_argument(
            "encoded-Z Ricci completion requires x > 0");
    }
    return EncodedZDerivativeJet(x, z_x, z_z, dx_z_x, dz_z_x, dx_z_z,
                                 dz_z_z);
}

class CompletionTensor
{
  public:
    double q_xx() const { return m_q_xx; }
    double q_xz() const { return m_q_xz; }
    double q_zz() const { return m_q_zz; }
    double q_ww() const { return m_q_ww; }
    double scalar_trace() const { return m_scalar_trace; }
    double tf_xx() const { return m_tf_xx; }
    double tf_xz() const { return m_tf_xz; }
    double tf_zz() const { return m_tf_zz; }
    double tf_ww() const { return m_tf_ww; }

  private:
    CompletionTensor(const double a_q_xx, const double a_q_xz,
                     const double a_q_zz, const double a_q_ww,
                     const double a_scalar_trace, const double a_tf_xx,
                     const double a_tf_xz, const double a_tf_zz,
                     const double a_tf_ww)
        : m_q_xx(a_q_xx), m_q_xz(a_q_xz), m_q_zz(a_q_zz), m_q_ww(a_q_ww),
          m_scalar_trace(a_scalar_trace), m_tf_xx(a_tf_xx),
          m_tf_xz(a_tf_xz), m_tf_zz(a_tf_zz), m_tf_ww(a_tf_ww)
    {
    }

    double m_q_xx;
    double m_q_xz;
    double m_q_zz;
    double m_q_ww;
    double m_scalar_trace;
    double m_tf_xx;
    double m_tf_xz;
    double m_tf_zz;
    double m_tf_ww;

    friend CompletionTensor compute(const EncodedZDerivativeJet &);
};

inline CompletionTensor compute(const EncodedZDerivativeJet &jet)
{
    const double q_xx = 2.0 * jet.dx_z_x();
    const double q_xz = jet.dx_z_z() + jet.dz_z_x();
    const double q_zz = 2.0 * jet.dz_z_z();
    const double q_ww = 2.0 * jet.z_x() / jet.x();
    const double scalar_trace =
        q_xx + q_zz + hidden_ww_multiplicity * q_ww;
    const double trace_share =
        scalar_trace / static_cast<double>(physical_spatial_dimension);

    const CompletionTensor result(
        q_xx, q_xz, q_zz, q_ww, scalar_trace, q_xx - trace_share, q_xz,
        q_zz - trace_share, q_ww - trace_share);

    const double values[] = {
        result.q_xx(), result.q_xz(), result.q_zz(), result.q_ww(),
        result.scalar_trace(), result.tf_xx(), result.tf_xz(),
        result.tf_zz(), result.tf_ww()};
    for (const double value : values)
    {
        if (!std::isfinite(value))
        {
            throw std::overflow_error(
                "encoded-Z Ricci completion produced a non-finite value");
        }
    }
    return result;
}

inline double trace_free_residual(const CompletionTensor &completion)
{
    return completion.tf_xx() + completion.tf_zz() +
           hidden_ww_multiplicity * completion.tf_ww();
}
} // namespace Stage4AOFrozenGaugeZRicciCompletion
} // namespace BlackStringToy

#endif
