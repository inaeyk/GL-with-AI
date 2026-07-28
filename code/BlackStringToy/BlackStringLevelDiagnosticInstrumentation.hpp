#ifndef BLACKSTRINGLEVELDIAGNOSTICINSTRUMENTATION_HPP_
#define BLACKSTRINGLEVELDIAGNOSTICINSTRUMENTATION_HPP_

#include "LoHiSide.H"

#include <cstddef>

// Test-only E2 lifecycle/ghost instrumentation. BlackStringToyLevel includes
// this type and stores it only when BLACKSTRING_E2_LEVEL_DIAGNOSTICS is
// defined; ordinary production builds contain neither these counters nor
// their update sites.
class BlackStringLevelDiagnosticInstrumentation
{
  public:
    struct Report
    {
        std::size_t initial_data_calls = 0;
        std::size_t rhs_calls = 0;
        std::size_t update_calls = 0;
        std::size_t cleanup_calls = 0;
        std::size_t advance_calls = 0;
        std::size_t post_step_calls = 0;
        std::size_t ghost_fill_calls = 0;
        std::size_t diagnostic_evaluations = 0;
        std::size_t low_radial_fills = 0;
        std::size_t high_radial_fills = 0;
        std::size_t radial_refreshes = 0;
        std::size_t periodic_exchanges = 0;
        std::size_t fillall_periodic_exchanges = 0;
        std::size_t rhs_periodic_exchanges = 0;
        std::size_t legacy_policy_duplicate_exchanges = 0;
        bool radial_low_then_high = true;
        bool exchange_before_radial = true;
        bool radial_before_rhs = true;
    };

    void record_initial_data() { ++m_report.initial_data_calls; }
    void record_update() { ++m_report.update_calls; }
    void record_cleanup() { ++m_report.cleanup_calls; }
    void record_advance() { ++m_report.advance_calls; }
    void record_post_step() { ++m_report.post_step_calls; }
    void record_ghost_fill() { ++m_report.ghost_fill_calls; }
    void record_diagnostic_evaluation()
    {
        ++m_report.diagnostic_evaluations;
    }

    void begin_fillall_periodic_exchange()
    {
        ++m_report.periodic_exchanges;
        ++m_report.fillall_periodic_exchanges;
        m_fillall_exchange_open = true;
    }

    void record_radial_fill(const Side::LoHiSide side)
    {
        if (side == Side::Lo)
        {
            ++m_report.low_radial_fills;
            m_report.radial_low_then_high =
                m_report.radial_low_then_high && !m_low_radial_open;
            m_low_radial_open = true;
            return;
        }

        ++m_report.high_radial_fills;
        m_report.radial_low_then_high =
            m_report.radial_low_then_high && m_low_radial_open;
        m_low_radial_open = false;
        ++m_report.radial_refreshes;
        ++m_radial_generation;
        if (m_fillall_exchange_open)
        {
            m_report.exchange_before_radial =
                m_report.exchange_before_radial &&
                m_fillall_exchange_open;
            m_fillall_exchange_open = false;
        }
    }

    void record_rhs_after_framework_refresh()
    {
        ++m_report.rhs_calls;
        ++m_report.periodic_exchanges;
        ++m_report.rhs_periodic_exchanges;
        m_report.radial_before_rhs =
            m_report.radial_before_rhs && !m_low_radial_open &&
            m_radial_generation > m_last_rhs_radial_generation;
        m_last_rhs_radial_generation = m_radial_generation;
    }

    void record_legacy_policy_duplicate_exchange()
    {
        ++m_report.periodic_exchanges;
        ++m_report.legacy_policy_duplicate_exchanges;
    }

    const Report &report() const { return m_report; }

  private:
    Report m_report;
    bool m_fillall_exchange_open = false;
    bool m_low_radial_open = false;
    std::size_t m_radial_generation = 0;
    std::size_t m_last_rhs_radial_generation = 0;
};

#endif /* BLACKSTRINGLEVELDIAGNOSTICINSTRUMENTATION_HPP_ */
