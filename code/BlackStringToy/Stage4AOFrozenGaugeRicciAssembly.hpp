#ifndef BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_RICCI_ASSEMBLY_HPP
#define BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_RICCI_ASSEMBLY_HPP

#include "Stage4AOFrozenGaugeHiddenRww.hpp"
#include "Stage4AOFrozenGaugeVisibleRxx.hpp"
#include "Stage4AOFrozenGaugeVisibleRxz.hpp"
#include "Stage4AOFrozenGaugeVisibleRzz.hpp"

#include <cmath>
#include <stdexcept>
#include <string>

namespace BlackStringToy
{
namespace Stage4AOFrozenGaugeRicciAssembly
{
// Stage 4AO-C validation-only Ricci assembly from already validated raw
// lower/lower physical component helpers. This assembles the scalar trace and
// trace-free Ricci tensor at the locked flat frozen-GP background. This helper
// does not itself perform the separately implemented A, Theta, and K Ricci
// insertions. It is not a complete frozen-gauge operator or production wiring.
static constexpr bool validation_only = true;
static constexpr bool raw_ricci_trace_implemented = true;
static constexpr bool trace_free_ricci_assembly_implemented = true;
static constexpr bool trace_free_a_curvature_source_implemented = false;
static constexpr bool theta_ricci_scalar_rhs_implemented = false;
static constexpr bool full_frozen_gauge_operator_implemented = false;
static constexpr bool eigensolver_implemented = false;
static constexpr bool production_rhs_wiring_implemented = false;
static constexpr int physical_spatial_dimension = 4;
static constexpr int hidden_multiplicity = 2;

namespace HiddenRww = Stage4AOFrozenGaugeHiddenRww;
namespace VisibleRxx = Stage4AOFrozenGaugeVisibleRxx;
namespace VisibleRxz = Stage4AOFrozenGaugeVisibleRxz;
namespace VisibleRzz = Stage4AOFrozenGaugeVisibleRzz;

class TraceFreeRicciAssembly;

class RawRicciComponents
{
  public:
    double rxx() const { return m_rxx; }
    double rxz() const { return m_rxz; }
    double rzz() const { return m_rzz; }
    double rww() const { return m_rww; }

  private:
    RawRicciComponents(const double rxx, const double rxz, const double rzz,
                       const double rww)
        : m_rxx(rxx), m_rxz(rxz), m_rzz(rzz), m_rww(rww)
    {
    }

    friend RawRicciComponents make_raw_ricci_components(
        const VisibleRxx::VisiblePhysicalDeltaRxx &rxx,
        const VisibleRxz::VisiblePhysicalDeltaRxz &rxz,
        const VisibleRzz::VisiblePhysicalDeltaRzz &rzz,
        const HiddenRww::HiddenPhysicalDeltaRww &rww);
    friend class TraceFreeRicciAssembly;
    friend TraceFreeRicciAssembly
    assemble_trace_free_ricci(const RawRicciComponents &components);

    double m_rxx;
    double m_rxz;
    double m_rzz;
    double m_rww;
};

class TraceFreeRicciAssembly
{
  public:
    double raw_rxx() const { return m_components.rxx(); }
    double raw_rxz() const { return m_components.rxz(); }
    double raw_rzz() const { return m_components.rzz(); }
    double raw_rww() const { return m_components.rww(); }
    double scalar_trace() const { return m_scalar_trace; }
    double tf_xx() const { return m_tf_xx; }
    double tf_xz() const { return m_tf_xz; }
    double tf_zz() const { return m_tf_zz; }
    double tf_ww() const { return m_tf_ww; }

  private:
    TraceFreeRicciAssembly(const RawRicciComponents &components,
                           const double scalar_trace, const double tf_xx,
                           const double tf_xz, const double tf_zz,
                           const double tf_ww)
        : m_components(components), m_scalar_trace(scalar_trace),
          m_tf_xx(tf_xx), m_tf_xz(tf_xz), m_tf_zz(tf_zz), m_tf_ww(tf_ww)
    {
    }

    friend TraceFreeRicciAssembly
    assemble_trace_free_ricci(const RawRicciComponents &components);

    RawRicciComponents m_components;
    double m_scalar_trace;
    double m_tf_xx;
    double m_tf_xz;
    double m_tf_zz;
    double m_tf_ww;
};

inline void require_finite_component(const double value, const char *name)
{
    if (!std::isfinite(value))
    {
        throw std::domain_error(std::string("Stage 4AO-C Ricci assembly requires finite ") +
                                name);
    }
}

inline RawRicciComponents make_raw_ricci_components(
    const VisibleRxx::VisiblePhysicalDeltaRxx &rxx,
    const VisibleRxz::VisiblePhysicalDeltaRxz &rxz,
    const VisibleRzz::VisiblePhysicalDeltaRzz &rzz,
    const HiddenRww::HiddenPhysicalDeltaRww &rww)
{
    const double raw_rxx = rxx.delta_rxx();
    const double raw_rxz = rxz.delta_rxz();
    const double raw_rzz = rzz.delta_rzz();
    const double raw_rww = rww.delta_rww();
    require_finite_component(raw_rxx, "delta R_xx");
    require_finite_component(raw_rxz, "delta R_xz");
    require_finite_component(raw_rzz, "delta R_zz");
    require_finite_component(raw_rww, "delta R_ww");
    return RawRicciComponents(raw_rxx, raw_rxz, raw_rzz, raw_rww);
}

inline TraceFreeRicciAssembly
assemble_trace_free_ricci(const RawRicciComponents &components)
{
    // At the locked frozen-GP background h^xz=0 and R_IJ^GP=0, so inverse
    // metric variations do not contribute to the linear scalar trace. Hidden
    // multiplicity two enters only through the representative ww component.
    const double trace =
        components.rxx() + components.rzz() +
        static_cast<double>(hidden_multiplicity) * components.rww();
    const double one_over_d =
        1.0 / static_cast<double>(physical_spatial_dimension);

    const double tf_xx = components.rxx() - one_over_d * trace;
    const double tf_xz = components.rxz();
    const double tf_zz = components.rzz() - one_over_d * trace;
    const double tf_ww = components.rww() - one_over_d * trace;

    require_finite_component(trace, "delta R scalar trace");
    require_finite_component(tf_xx, "trace-free delta R_xx");
    require_finite_component(tf_xz, "trace-free delta R_xz");
    require_finite_component(tf_zz, "trace-free delta R_zz");
    require_finite_component(tf_ww, "trace-free delta R_ww");

    return TraceFreeRicciAssembly(components, trace, tf_xx, tf_xz, tf_zz,
                                  tf_ww);
}

inline double trace_free_residual(const TraceFreeRicciAssembly &assembly)
{
    return assembly.tf_xx() + assembly.tf_zz() +
           static_cast<double>(hidden_multiplicity) * assembly.tf_ww();
}

} // namespace Stage4AOFrozenGaugeRicciAssembly
} // namespace BlackStringToy

#endif /* BLACKSTRINGTOY_STAGE4AO_FROZEN_GAUGE_RICCI_ASSEMBLY_HPP */
