#ifndef BLACKSTRINGGPPOINTWISEINITIALDATA_HPP
#define BLACKSTRINGGPPOINTWISEINITIALDATA_HPP

#include "BlackStringReducedVars.hpp"

#include <cmath>
#include <stdexcept>

namespace BlackStringGPPointwiseInitialData
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

struct RadialJet
{
    double value;
    double dx;
    double dxx;
};

struct AnalyticMetadata
{
    double r0;
    double x;
    RadialJet beta_x;
    RadialJet lambda;
    RadialJet K;
    Reduced::ReducedSymmetricTensor<RadialJet> A;
};

inline void require_valid_inputs(const double r0, const double x)
{
    if (!std::isfinite(r0) || !std::isfinite(x) || !(r0 > 0.0) ||
        !(x > 0.0))
    {
        throw std::domain_error(
            "black-string GP point data require finite r0>0 and x>0");
    }
}

inline RadialJet scaled_jet(const double coefficient, const RadialJet &jet)
{
    return {coefficient * jet.value, coefficient * jet.dx,
            coefficient * jet.dxx};
}

inline AnalyticMetadata make_analytic_metadata(const double r0,
                                               const double x)
{
    require_valid_inputs(r0, x);

    const double beta_x = std::sqrt(r0 / x);
    const double lambda = std::sqrt(r0 / (x * x * x));
    if (!std::isfinite(beta_x) || !std::isfinite(lambda))
    {
        throw std::domain_error(
            "black-string GP point data produced nonfinite beta or lambda");
    }

    const RadialJet beta_jet{beta_x, -beta_x / (2.0 * x),
                             3.0 * beta_x / (4.0 * x * x)};
    const RadialJet lambda_jet{lambda, -3.0 * lambda / (2.0 * x),
                               15.0 * lambda / (4.0 * x * x)};
    const RadialJet zero_jet{0.0, 0.0, 0.0};

    return {r0,
            x,
            beta_jet,
            lambda_jet,
            scaled_jet(3.0 / 2.0, lambda_jet),
            {scaled_jet(-7.0 / 8.0, lambda_jet), zero_jet,
             scaled_jet(-3.0 / 8.0, lambda_jet),
             scaled_jet(5.0 / 8.0, lambda_jet)}};
}

inline Reduced::Variables<double> make_pointwise_vars(const double r0,
                                                      const double x)
{
    const AnalyticMetadata analytic = make_analytic_metadata(r0, x);

    Reduced::Variables<double> vars{};
    vars.physical.chi = 1.0;
    vars.physical.h = {1.0, 0.0, 1.0,
                       Production::GPBackgroundConvention::stored_hww};
    vars.physical.K = analytic.K.value;
    vars.physical.A = {analytic.A.xx.value, analytic.A.xz.value,
                       analytic.A.zz.value, analytic.A.ww.value};
    vars.physical.Theta = 0.0;
    vars.physical.Gamma = {0.0, 0.0};

    vars.gauge.lapse = 1.0;
    vars.gauge.shift = {analytic.beta_x.value, 0.0};
    vars.gauge.B = {0.0, 0.0};
    return vars;
}

inline Reduced::Storage<double> make_pointwise_state(const double r0,
                                                     const double x)
{
    return Reduced::store(make_pointwise_vars(r0, x));
}

inline double gamma_theta_theta(const double x)
{
    if (!std::isfinite(x) || !(x > 0.0))
    {
        throw std::domain_error(
            "black-string GP gamma_theta_theta requires finite x>0");
    }
    return Production::GPBackgroundConvention::gamma_theta_theta(x);
}

inline double gamma_ww(const Reduced::Variables<double> &vars)
{
    return Production::GPBackgroundConvention::gamma_ww(
        vars.physical.h.ww, vars.physical.chi);
}

} // namespace BlackStringGPPointwiseInitialData

#endif /* BLACKSTRINGGPPOINTWISEINITIALDATA_HPP */
