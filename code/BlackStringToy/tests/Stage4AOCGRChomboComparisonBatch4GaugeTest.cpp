#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#define CH_SPACEDIM 3
#include "Tensor.hpp"
template <class data_t> struct emtensor_t
{
    Tensor<1, data_t> Si;
};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "MovingPunctureGauge.hpp"

namespace
{
constexpr int dimension = 3;
constexpr double r0 = 1.0;
constexpr double x = 2.0;

template <class data_t> struct GaugeVars
{
    data_t lapse{};
    data_t K{};
    data_t Theta{};
    Tensor<1, data_t> shift{};
    Tensor<1, data_t> B{};
    Tensor<1, data_t> Gamma{};
};

template <class data_t> struct GaugeDiff2
{
    data_t lapse{};
};

struct SourceFields
{
    double chi = 1.0;
    double hxx = 1.0;
    double hxz = 0.0;
    double hzz = 1.0;
    double hww = 1.0;
    double K = 0.0;
    double Axx = 0.0;
    double Axz = 0.0;
    double Azz = 0.0;
    double Aww = 0.0;
    double Theta = 0.0;
    double GammaX = 0.0;
    double GammaZ = 0.0;
    double lapse = 1.0;
    std::array<double, dimension> shift = {};
    std::array<double, dimension> B = {};
};

constexpr std::size_t source_field_count = 20;
constexpr std::array<const char *, source_field_count> source_field_names = {
    "chi",    "hxx",    "hxz",    "hzz",    "hww",
    "K",      "Axx",    "Axz",    "Azz",    "Aww",
    "Theta",  "GammaX", "GammaZ", "lapse",  "shiftX",
    "shiftY", "shiftZ", "Bx",     "By",     "Bz"};

double &source_field(SourceFields &fields, const std::size_t index)
{
    switch (index)
    {
    case 0:
        return fields.chi;
    case 1:
        return fields.hxx;
    case 2:
        return fields.hxz;
    case 3:
        return fields.hzz;
    case 4:
        return fields.hww;
    case 5:
        return fields.K;
    case 6:
        return fields.Axx;
    case 7:
        return fields.Axz;
    case 8:
        return fields.Azz;
    case 9:
        return fields.Aww;
    case 10:
        return fields.Theta;
    case 11:
        return fields.GammaX;
    case 12:
        return fields.GammaZ;
    case 13:
        return fields.lapse;
    case 14:
    case 15:
    case 16:
        return fields.shift[index - 14];
    case 17:
    case 18:
    case 19:
        return fields.B[index - 17];
    default:
        std::abort();
    }
}

struct SourceInputs
{
    double locked_r0;
    double coordinate_x;
    const SourceFields &fields;
    double mock_evolving_horizon_radius;
};

class FixedGPLapseSourceAdapter
{
  public:
    double value(const SourceInputs &inputs) const
    {
        (void)inputs.fields;
        (void)inputs.mock_evolving_horizon_radius;
        return 3.0 *
               std::sqrt(inputs.locked_r0 /
                         (inputs.coordinate_x * inputs.coordinate_x *
                          inputs.coordinate_x));
    }

    void apply(GaugeVars<double> &rhs, const SourceInputs &inputs) const
    {
        rhs.lapse += value(inputs);
    }
};

class FieldDependentSourceMutation
{
  public:
    double value(const SourceInputs &inputs) const
    {
        return 2.0 * inputs.fields.K;
    }
};

class HorizonDependentSourceMutation
{
  public:
    double value(const SourceInputs &inputs) const
    {
        return 3.0 *
               std::sqrt(inputs.mock_evolving_horizon_radius /
                         (inputs.coordinate_x * inputs.coordinate_x *
                          inputs.coordinate_x));
    }
};

class ShiftOwnershipSourceMutation
{
  public:
    void apply(GaugeVars<double> &rhs, const SourceInputs &inputs) const
    {
        const FixedGPLapseSourceAdapter correct;
        correct.apply(rhs, inputs);
        rhs.shift[0] += correct.value(inputs);
    }
};

class BOwnershipSourceMutation
{
  public:
    void apply(GaugeVars<double> &rhs, const SourceInputs &inputs) const
    {
        const FixedGPLapseSourceAdapter correct;
        correct.apply(rhs, inputs);
        rhs.B[0] += correct.value(inputs);
    }
};

class WrongSignSourceMutation
{
  public:
    void apply(GaugeVars<double> &rhs, const SourceInputs &inputs) const
    {
        const FixedGPLapseSourceAdapter correct;
        rhs.lapse -= correct.value(inputs);
    }
};

class MissingFactorThreeSourceMutation
{
  public:
    void apply(GaugeVars<double> &rhs, const SourceInputs &inputs) const
    {
        rhs.lapse +=
            std::sqrt(inputs.locked_r0 /
                      (inputs.coordinate_x * inputs.coordinate_x *
                       inputs.coordinate_x));
    }
};

void require(const bool condition, const std::string &message)
{
    if (!condition)
    {
        std::cerr << "FAIL " << message << "\n";
        std::exit(1);
    }
}

void require_close(const double actual, const double expected,
                   const std::string &message, const double tolerance = 1.0e-13)
{
    require(std::abs(actual - expected) <= tolerance, message);
}

GaugeVars<double> direct_gauge_rhs(const double K, const double Theta,
                                   const double lapse,
                                   const std::array<double, dimension> &B)
{
    MovingPunctureGauge::params_t parameters;
    parameters.lapse_advec_coeff = 0.0;
    parameters.lapse_power = 1.0;
    parameters.lapse_coeff = 2.0;
    parameters.shift_Gamma_coeff = 0.75;
    parameters.shift_advec_coeff = 0.0;
    parameters.eta = 1.0;
    const MovingPunctureGauge gauge(parameters);

    GaugeVars<double> rhs{};
    GaugeVars<double> vars{};
    GaugeVars<Tensor<1, double>> d1{};
    GaugeDiff2<Tensor<2, double>> d2{};
    GaugeVars<double> advec{};
    vars.K = K;
    vars.Theta = Theta;
    vars.lapse = lapse;
    rhs.lapse = rhs.K = rhs.Theta = 0.0;
    advec.lapse = advec.K = advec.Theta = 0.0;
    for (int i = 0; i < dimension; ++i)
    {
        vars.B[i] = B[static_cast<std::size_t>(i)];
        rhs.shift[i] = rhs.B[i] = rhs.Gamma[i] = 0.0;
        advec.shift[i] = advec.B[i] = advec.Gamma[i] = 0.0;
        for (int j = 0; j < dimension; ++j)
        {
            d1.shift[i][j] = 0.0;
            d1.B[i][j] = 0.0;
            d1.Gamma[i][j] = 0.0;
        }
    }
    gauge.rhs_gauge(rhs, vars, d1, d2, advec);
    return rhs;
}

void test_direct_gauge_and_fixed_source()
{
    const double lambda = std::sqrt(r0 / (x * x * x));
    const double K = 1.5 * lambda;
    const std::array<double, dimension> zero_B = {0.0, 0.0, 0.0};
    const auto direct = direct_gauge_rhs(K, 0.0, 1.0, zero_B);
    require_close(direct.lapse, -3.0 * lambda,
                  "direct GRChombo lapse gives -3 lambda");
    for (int i = 0; i < dimension; ++i)
    {
        require_close(direct.shift[i], 0.0,
                      "direct GP shift gauge residual is zero");
        require_close(direct.B[i], 0.0,
                      "direct GP B gauge residual is zero");
    }

    SourceFields fields;
    fields.K = K;
    fields.Axx = -7.0 * lambda / 8.0;
    fields.Azz = -3.0 * lambda / 8.0;
    fields.Aww = 5.0 * lambda / 8.0;
    const SourceInputs inputs{r0, x, fields, 1.37};
    const FixedGPLapseSourceAdapter adapter;
    const double source = adapter.value(inputs);

    auto held = direct;
    for (int i = 0; i < dimension; ++i)
    {
        held.shift[i] = 0.25 + 0.1 * static_cast<double>(i);
        held.B[i] = -0.4 - 0.2 * static_cast<double>(i);
    }
    const auto before_source = held;
    adapter.apply(held, inputs);
    require_close(held.lapse, 0.0, "fixed source cancels GP lapse drift");
    for (int i = 0; i < dimension; ++i)
    {
        require_close(held.shift[i], before_source.shift[i],
                      "fixed source does not enter shift");
        require_close(held.B[i], before_source.B[i],
                      "fixed source does not enter B");
    }

    // Numerical Frechet derivatives with respect to every evolved field.
    const double epsilon = 1.0e-6;
    double max_field_jacobian = 0.0;
    for (std::size_t field = 0; field < source_field_count; ++field)
    {
        auto plus_fields = fields;
        auto minus_fields = fields;
        source_field(plus_fields, field) += epsilon;
        source_field(minus_fields, field) -= epsilon;
        const SourceInputs plus_inputs{r0, x, plus_fields, 1.37};
        const SourceInputs minus_inputs{r0, x, minus_fields, 1.37};
        const double plus_source = adapter.value(plus_inputs);
        const double minus_source = adapter.value(minus_inputs);
        require_close(plus_source, source,
                      std::string("field-independent source plus ") +
                          source_field_names[field],
                      0.0);
        require_close(minus_source, source,
                      std::string("field-independent source minus ") +
                          source_field_names[field],
                      0.0);
        const double derivative =
            (plus_source - minus_source) / (2.0 * epsilon);
        max_field_jacobian =
            std::max(max_field_jacobian, std::abs(derivative));
        require_close(derivative, 0.0,
                      std::string("zero source Frechet derivative ") +
                          source_field_names[field],
                      0.0);
    }

    const FieldDependentSourceMutation field_mutation;
    auto plus_k = fields;
    auto minus_k = fields;
    plus_k.K += epsilon;
    minus_k.K -= epsilon;
    const double wrong_field_jacobian =
        (field_mutation.value({r0, x, plus_k, 1.37}) -
         field_mutation.value({r0, x, minus_k, 1.37})) /
        (2.0 * epsilon);
    require(std::abs(wrong_field_jacobian) > 1.0,
            "2K evolving-field source mutation is detected");

    const SourceInputs horizon_a{r0, x, fields, 0.91};
    const SourceInputs horizon_b{r0, x, fields, 1.44};
    require_close(adapter.value(horizon_a), adapter.value(horizon_b),
                  "correct source ignores evolving horizon", 0.0);
    const HorizonDependentSourceMutation horizon_mutation;
    const double wrong_horizon_a = horizon_mutation.value(horizon_a);
    const double wrong_horizon_b = horizon_mutation.value(horizon_b);
    require(std::abs(wrong_horizon_a - wrong_horizon_b) > 1.0e-3,
            "instantaneous-horizon source mutation is detected");

    auto wrong_shift_output = before_source;
    const ShiftOwnershipSourceMutation shift_owner_mutation;
    shift_owner_mutation.apply(wrong_shift_output, inputs);
    require(std::abs(wrong_shift_output.shift[0] -
                     before_source.shift[0]) > 1.0e-3,
            "shift-output ownership mutation is detected");
    auto wrong_b_output = before_source;
    const BOwnershipSourceMutation b_owner_mutation;
    b_owner_mutation.apply(wrong_b_output, inputs);
    require(std::abs(wrong_b_output.B[0] - before_source.B[0]) > 1.0e-3,
            "B-output ownership mutation is detected");

    auto wrong_sign_output = direct;
    const WrongSignSourceMutation wrong_sign_mutation;
    wrong_sign_mutation.apply(wrong_sign_output, inputs);
    require(std::abs(wrong_sign_output.lapse) > 1.0e-3,
            "wrong source sign is detected through adapter output");
    auto missing_factor_output = direct;
    const MissingFactorThreeSourceMutation missing_factor_mutation;
    missing_factor_mutation.apply(missing_factor_output, inputs);
    require(std::abs(missing_factor_output.lapse) > 1.0e-3,
            "missing factor three is detected through adapter output");

    std::cout << "RESULT gauge direct_lapse=" << direct.lapse
              << " fixed_source=" << source
              << " held_lapse=" << held.lapse
              << " evolved_fields_tested=" << source_field_count
              << " max_source_jacobian=" << max_field_jacobian
              << " wrong_2K_jacobian=" << wrong_field_jacobian
              << " shift_source=0 B_source=0\n";
    std::cout << "MUTATION wrong_sign=DETECTED missing_factor_three=DETECTED "
                 "evolving_2K_source=DETECTED "
                 "instantaneous_horizon_source=DETECTED "
                 "wrong_shift_owner=DETECTED wrong_B_owner=DETECTED\n";
    std::cout
        << "CLASSIFICATION direct_compiled=MovingPunctureGauge_raw_rhs "
           "source_convention=fixed_background_lapse_source "
           "blocked=production_source_adapter\n";
}

} // namespace

int main()
{
    std::cout << std::scientific << std::setprecision(8);
    test_direct_gauge_and_fixed_source();
    std::cout << "PASS comparison batch 4 direct gauge/source convention\n";
    return 0;
}
#pragma GCC diagnostic pop
