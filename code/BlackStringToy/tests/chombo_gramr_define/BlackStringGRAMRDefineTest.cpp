#include "AMRLevel.H"
#include "BlackStringBoundaryDimensionAudit.hpp"
#include "BoundaryConditions.hpp"
#include "DefaultLevelFactory.hpp"
#include "GRAMR.hpp"
#include "GRAMRLevel.hpp"
#include "GRParmParse.hpp"
#include "ProblemDomain.H"
#include "SimulationParameters.hpp"
#include "UserVariables.hpp"

#include <array>
#include <cstdint>
#include <iostream>
#include <string>

namespace
{
constexpr std::uint64_t canary_before = 0x8f3a71c5d9426be1ULL;
constexpr std::uint64_t canary_after = 0x32d84eb70a6fc915ULL;

template <class value_t> struct GuardedGridArray
{
    std::uint64_t before = canary_before;
    std::array<value_t, CH_SPACEDIM> values{};
    std::uint64_t after = canary_after;

    bool intact() const
    {
        return before == canary_before && after == canary_after;
    }
};

class InspectableBoundaryConditions : public BoundaryConditions
{
  public:
    const RealVect &center() const { return m_center; }
    const ProblemDomain &domain() const { return m_domain; }
    const params_t &parameters() const { return m_params; }
};

class DefineOnlyLevel : public GRAMRLevel
{
    friend class DefaultLevelFactory<DefineOnlyLevel>;
    using GRAMRLevel::GRAMRLevel;

  public:
    static int initial_data_calls;
    static int rhs_calls;

    const SimulationParameters &parameters() const { return m_p; }

  private:
    void initialData() override { ++initial_data_calls; }

    void specificEvalRHS(GRLevelData &, GRLevelData &, double) override
    {
        ++rhs_calls;
    }
};

int DefineOnlyLevel::initial_data_calls = 0;
int DefineOnlyLevel::rhs_calls = 0;

bool same_domain(const ProblemDomain &lhs, const ProblemDomain &rhs)
{
    if (lhs.domainBox() != rhs.domainBox())
    {
        return false;
    }
    for (int direction = 0; direction < CH_SPACEDIM; ++direction)
    {
        if (lhs.isPeriodic(direction) != rhs.isPeriodic(direction))
        {
            return false;
        }
    }
    return true;
}

int fail(const std::string &message)
{
    std::cerr << "BLACKSTRING_GRAMR_DEFINE_FAIL " << message << '\n';
    return 1;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return fail("usage: BlackStringGRAMRDefineTest <parameter-file>");
    }

    static_assert(CH_SPACEDIM == 2);
    static_assert(GR_SPACEDIM == 4);
    static_assert(DEFAULT_TENSOR_DIM == 4);
    static_assert(NUM_VARS == 18);
    static_assert(c_hww == 4 && c_Aww == 9);

    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);

    GuardedGridArray<double> center_guard;
    center_guard.values = parameters.center;
    GuardedGridArray<int> extent_guard;
    extent_guard.values = {parameters.ivN[0], parameters.ivN[1]};
    GuardedGridArray<bool> periodic_guard;
    periodic_guard.values = {false, true};
    GuardedGridArray<int> low_guard;
    low_guard.values = {BoundaryConditions::EXTRAPOLATING_BC,
                        BoundaryConditions::STATIC_BC};
    GuardedGridArray<int> high_guard;
    high_guard.values = {BoundaryConditions::EXTRAPOLATING_BC,
                         BoundaryConditions::STATIC_BC};

    BlackStringBoundaryDimensionAudit::reset();
    BoundaryConditions::params_t guarded_boundary_params =
        parameters.boundary_params;
    guarded_boundary_params.set_is_periodic(periodic_guard.values);
    guarded_boundary_params.set_lo_boundary(low_guard.values);
    guarded_boundary_params.set_hi_boundary(high_guard.values);

    if (!periodic_guard.intact() || !low_guard.intact() ||
        !high_guard.intact())
    {
        return fail("two-component boundary input canary changed");
    }
    if (BlackStringBoundaryDimensionAudit::out_of_range_visits.load() != 0 ||
        BlackStringBoundaryDimensionAudit::maximum_direction.load() != 1)
    {
        return fail("boundary setter used a non-grid direction");
    }

    SimulationParameters guarded_parameters = parameters;
    guarded_parameters.center = center_guard.values;
    guarded_parameters.boundary_params = guarded_boundary_params;
    for (int direction = 0; direction < CH_SPACEDIM; ++direction)
    {
        guarded_parameters.ivN[direction] = extent_guard.values[direction];
    }

    IntVect guarded_domain_high = IntVect::Zero;
    for (int direction = 0; direction < CH_SPACEDIM; ++direction)
    {
        guarded_domain_high[direction] = extent_guard.values[direction];
    }
    Box domain_box(IntVect::Zero, guarded_domain_high);
    ProblemDomain physical_domain(domain_box);
    for (int direction = 0; direction < CH_SPACEDIM; ++direction)
    {
        physical_domain.setPeriodic(
            direction,
            guarded_parameters.boundary_params.is_periodic[direction]);
    }

    BlackStringBoundaryDimensionAudit::reset();
    GRAMR amr;
    DefaultLevelFactory<DefineOnlyLevel> factory(amr, guarded_parameters);
    amr.define(guarded_parameters.max_level, guarded_parameters.ref_ratios,
               physical_domain, &factory);

    if (DefineOnlyLevel::initial_data_calls != 0 ||
        DefineOnlyLevel::rhs_calls != 0)
    {
        return fail("define-only path entered initialization or RHS");
    }
    if (BlackStringBoundaryDimensionAudit::loop_visits.load() != CH_SPACEDIM ||
        BlackStringBoundaryDimensionAudit::maximum_direction.load() != 1 ||
        BlackStringBoundaryDimensionAudit::out_of_range_visits.load() != 0)
    {
        return fail("AMR define did not use the DIM2 boundary copy loop");
    }
    if (!periodic_guard.intact() || !low_guard.intact() ||
        !high_guard.intact() || !center_guard.intact() ||
        !extent_guard.intact())
    {
        return fail("real AMR define changed a guarded two-component input");
    }

    const Vector<AMRLevel *> levels = amr.getAMRLevels();
    if (levels.size() != 1)
    {
        return fail("AMR::define did not create exactly one level");
    }
    const auto *level = dynamic_cast<const DefineOnlyLevel *>(levels[0]);
    if (level == nullptr)
    {
        return fail("level factory returned the wrong concrete level");
    }
    if (guarded_parameters.center != center_guard.values ||
        guarded_parameters.ivN != guarded_domain_high ||
        level->parameters().center != center_guard.values ||
        level->parameters().ivN[0] != extent_guard.values[0] ||
        level->parameters().ivN[1] != extent_guard.values[1])
    {
        return fail("level parameter transfer changed center or extents");
    }

    BlackStringBoundaryDimensionAudit::reset();
    InspectableBoundaryConditions inspected_boundaries;
    inspected_boundaries.define(
        guarded_parameters.coarsest_dx, center_guard.values,
        guarded_parameters.boundary_params, physical_domain,
        guarded_parameters.num_ghosts);

    if (!center_guard.intact() || !extent_guard.intact())
    {
        return fail("boundary define changed a guarded two-component input");
    }
    if (BlackStringBoundaryDimensionAudit::loop_visits.load() != CH_SPACEDIM ||
        BlackStringBoundaryDimensionAudit::maximum_direction.load() != 1 ||
        BlackStringBoundaryDimensionAudit::out_of_range_visits.load() != 0)
    {
        return fail("direct boundary define used a non-grid direction");
    }
    for (int direction = 0; direction < CH_SPACEDIM; ++direction)
    {
        if (inspected_boundaries.center()[direction] !=
            center_guard.values[direction])
        {
            return fail("boundary center transfer changed a component");
        }
    }
    if (!same_domain(inspected_boundaries.domain(), physical_domain))
    {
        return fail("boundary ProblemDomain transfer changed");
    }
    if (inspected_boundaries.parameters().is_periodic[0] ||
        !inspected_boundaries.parameters().is_periodic[1] ||
        physical_domain.isPeriodic(0) || !physical_domain.isPeriodic(1))
    {
        return fail("radial/compact periodic directions changed");
    }

    std::cout
        << "DEFINE_CALL_PATH=AMR::define->GRAMRLevel::define"
           "->BoundaryConditions::define\n"
        << "GRID_DIMENSIONS_ACCESSED=0,1\n"
        << "GRID_DIRECTION_2_OR_3_ACCESSED=0\n"
        << "BOUNDARY_CENTER=" << inspected_boundaries.center()[0] << ','
        << inspected_boundaries.center()[1] << '\n'
        << "DOMAIN_HIGH=" << inspected_boundaries.domain().domainBox().bigEnd(0)
        << ',' << inspected_boundaries.domain().domainBox().bigEnd(1) << '\n'
        << "PERIODIC_RADIAL=0 PERIODIC_COMPACT_Z=1\n"
        << "CANARIES=PASS REGISTRATION_SLOTS=" << NUM_VARS << '\n'
        << "BLACKSTRING_GRAMR_DEFINE_PASS\n";
    return 0;
}
