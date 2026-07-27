#include "AMRLevel.H"
#include "BoundaryConditions.hpp"
#include "DefaultLevelFactory.hpp"
#include "GRAMR.hpp"
#include "GRAMRLevel.hpp"
#include "GRParmParse.hpp"
#include "ProblemDomain.H"
#include "SimulationParameters.hpp"

#include <iostream>
#include <string>

namespace
{
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

  private:
    void initialData() override { ++initial_data_calls; }

    void specificEvalRHS(GRLevelData &, GRLevelData &, double) override
    {
        ++rhs_calls;
    }
};

int DefineOnlyLevel::initial_data_calls = 0;
int DefineOnlyLevel::rhs_calls = 0;

int fail(const std::string &message)
{
    std::cerr << "STOCK_DIM3_DEFINE_FAIL " << message << '\n';
    return 1;
}
} // namespace

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return fail("usage: StockDim3BoundaryDefineTest <parameter-file>");
    }

    static_assert(CH_SPACEDIM == 3);
    static_assert(GR_SPACEDIM == 3);
    static_assert(DEFAULT_TENSOR_DIM == 3);

    GRParmParse pp(0, nullptr, nullptr, argv[1]);
    SimulationParameters parameters(pp);

    Box domain_box(IntVect::Zero, parameters.ivN);
    ProblemDomain physical_domain(domain_box);
    for (int direction = 0; direction < CH_SPACEDIM; ++direction)
    {
        physical_domain.setPeriodic(
            direction, parameters.boundary_params.is_periodic[direction]);
    }

    GRAMR amr;
    DefaultLevelFactory<DefineOnlyLevel> factory(amr, parameters);
    amr.define(parameters.max_level, parameters.ref_ratios, physical_domain,
               &factory);

    if (DefineOnlyLevel::initial_data_calls != 0 ||
        DefineOnlyLevel::rhs_calls != 0)
    {
        return fail("define-only path entered initialization or RHS");
    }
    if (amr.getAMRLevels().size() != 1)
    {
        return fail("AMR::define did not create exactly one stock level");
    }

    InspectableBoundaryConditions boundaries;
    boundaries.define(parameters.coarsest_dx, parameters.center,
                      parameters.boundary_params, physical_domain,
                      parameters.num_ghosts);

    for (int direction = 0; direction < CH_SPACEDIM; ++direction)
    {
        if (boundaries.center()[direction] != parameters.center[direction] ||
            boundaries.domain().isPeriodic(direction) !=
                physical_domain.isPeriodic(direction) ||
            boundaries.parameters().is_periodic[direction] !=
                parameters.boundary_params.is_periodic[direction])
        {
            return fail("ordinary DIM3 boundary parameter transfer changed");
        }
    }

    std::cout
        << "STOCK_DEFINE_CALL_PATH=AMR::define->GRAMRLevel::define"
           "->BoundaryConditions::define\n"
        << "STOCK_DIMENSIONS_ACCESSED=0,1,2\n"
        << "STOCK_CENTER=" << boundaries.center()[0] << ','
        << boundaries.center()[1] << ',' << boundaries.center()[2] << '\n'
        << "STOCK_PERIODIC=0,1,0\n"
        << "STOCK_DIM3_BOUNDARY_DEFINE_PASS\n";
    return 0;
}
