#ifndef BLACKSTRINGRADIALGPGHOST_HPP_
#define BLACKSTRINGRADIALGPGHOST_HPP_

#include "BlackStringGPInitialData.hpp"
#include "BoxIterator.H"
#include "BoxLoops.hpp"
#include "GRLevelData.hpp"
#include "ProblemDomain.H"
#include "LoHiSide.H"

#include <array>

// This policy supplies background-preserving radial ghost data for the
// bounded, unperturbed GP evolution diagnostic only. It is not a physical
// radial boundary condition and is not suitable for perturbation evolution.
class BlackStringRadialGPGhost
{
  public:
    static void
    fill(GRLevelData &state, const ProblemDomain &problem_domain,
         const double r0, const double dx,
         const std::array<double, CH_SPACEDIM> &coordinate_offset,
         const Side::LoHiSide side)
    {
        const Box domain = problem_domain.domainBox();
        const DataIterator iterator = state.dataIterator();
        for (int ibox = 0; ibox < iterator.size(); ++ibox)
        {
            const DataIndex data_index = iterator[ibox];
            FArrayBox &fab = state[data_index];
            Box radial_ghosts = fab.box();

            if (side == Side::Lo)
            {
                radial_ghosts.setBig(
                    BlackStringGPInitialData::radial_direction,
                    domain.smallEnd(
                        BlackStringGPInitialData::radial_direction) -
                        1);
            }
            else
            {
                radial_ghosts.setSmall(
                    BlackStringGPInitialData::radial_direction,
                    domain.bigEnd(
                        BlackStringGPInitialData::radial_direction) +
                        1);
            }
            radial_ghosts &= fab.box();
            if (radial_ghosts.isEmpty())
            {
                continue;
            }

            BoxLoops::loop(
                BlackStringGPInitialData::make_compute(r0, dx,
                                                       coordinate_offset),
                fab, fab, radial_ghosts, disable_simd());
        }
    }
};

#endif /* BLACKSTRINGRADIALGPGHOST_HPP_ */
