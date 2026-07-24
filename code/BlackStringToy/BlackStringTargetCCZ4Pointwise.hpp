#ifndef BLACKSTRINGTARGETCCZ4POINTWISE_HPP
#define BLACKSTRINGTARGETCCZ4POINTWISE_HPP

#include "BlackStringReducedVars.hpp"
#include "TensorAlgebra.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

// Locked GRChombo has direct 2x2 and 3x3 symmetric inverse overloads, but no
// 4x4 overload. The black-string target tensor is block diagonal outside its
// gridded (x,z) block, so this narrow overload supplies exactly that inverse
// without changing dependency source.
namespace TensorAlgebra
{
template <class data_t>
Tensor<2, data_t, 4>
compute_inverse_sym(const Tensor<2, data_t, 4> &matrix)
{
    const data_t reduced_determinant =
        matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[0][1];
    const data_t inverse_reduced_determinant = 1.0 / reduced_determinant;
    Tensor<2, data_t, 4> inverse = 0.0;
    inverse[0][0] = matrix[1][1] * inverse_reduced_determinant;
    inverse[0][1] = -matrix[0][1] * inverse_reduced_determinant;
    inverse[1][0] = inverse[0][1];
    inverse[1][1] = matrix[0][0] * inverse_reduced_determinant;
    inverse[2][2] = 1.0 / matrix[2][2];
    inverse[3][3] = 1.0 / matrix[3][3];
    return inverse;
}
} // namespace TensorAlgebra

// CCZ4RHS::rhs_equation is header implemented, but its public header also
// imports grid storage, derivative, gauge, and enum-mapping wrappers. This
// pointwise seam suppresses only those unrelated wrappers and supplies the
// minimum types required to instantiate the locked source routine.
#define CELL_HPP_
template <class data_t> class Cell;
#define FOURTHORDERDERIVATIVES_HPP_
class FourthOrderDerivatives;
#define MOVINGPUNCTUREGAUGE_HPP_
class MovingPunctureGauge
{
  public:
    struct params_t
    {
    };
};
#define CCZ4VARS_HPP_
namespace CCZ4Vars
{
template <class data_t> struct VarsWithGauge;
template <class data_t> struct Diff2VarsWithGauge;
} // namespace CCZ4Vars
#define USERVARIABLES_HPP
#define VARSTOOLS_HPP_
#define SIMD_HPP_

namespace MayDay
{
inline void Error(const char *message) { throw std::runtime_error(message); }
} // namespace MayDay

#include "CCZ4RHS.hpp"

namespace BlackStringTargetCCZ4Pointwise
{
namespace Production = BlackStringProductionVariables;
namespace Reduced = BlackStringReducedVars;

inline constexpr int target_dimension = 4;
inline constexpr int gridded_dimension = 2;
inline constexpr int radial_direction = 0;
inline constexpr int compact_direction = 1;
inline constexpr int first_hidden_direction = 2;
inline constexpr int second_hidden_direction = 3;
inline constexpr int hidden_multiplicity = 2;
inline constexpr std::size_t physical_rows = 13;
inline constexpr std::size_t reported_families = 5;

static_assert(CH_SPACEDIM == gridded_dimension);
static_assert(GR_SPACEDIM == target_dimension);
static_assert(DEFAULT_TENSOR_DIM == target_dimension);
static_assert(Production::NUM_VARS == 18);
static_assert(Production::physical_trace_hidden_copies ==
              hidden_multiplicity);
static_assert(!Production::uses_stock_visible_y_as_hidden);

enum class PhysicalRow : std::size_t
{
    chi = 0,
    hxx,
    hxz,
    hzz,
    hww,
    K,
    Axx,
    Axz,
    Azz,
    Aww,
    Theta,
    GammaX,
    GammaZ,
    count
};

inline constexpr std::array<const char *, physical_rows> row_names = {
    "chi",   "hxx",   "hxz",   "hzz",   "hww",   "K",      "Axx",
    "Axz",   "Azz",   "Aww",   "Theta", "GammaX", "GammaZ"};

using Rows = std::array<double, physical_rows>;

enum class ReportedFamily : std::size_t
{
    geometric_ricci = 0,
    encoded_z,
    lapse_derivatives,
    damping,
    remaining_shared_algebra,
    count
};

inline constexpr std::array<const char *, reported_families>
    reported_family_names = {"geometric_ricci", "encoded_z",
                             "lapse_derivatives", "damping",
                             "remaining_shared_algebra"};

struct OwnershipSplit
{
    Rows target_shared_hidden_suppressed = {};
    Rows hidden_increment_decomposition = {};
    Rows target_full_grchombo = {};
};

struct Jet
{
    double dx = 0.0;
    double dz = 0.0;
    double dxx = 0.0;
    double dxz = 0.0;
    double dzz = 0.0;
};

using DerivativeJets = std::array<Jet, Production::NUM_VARS>;

struct LockedCCZ4Parameters
{
    double kappa1 = 0.1;
    double kappa2 = 0.0;
    double kappa3 = 1.0;
    bool covariant_z4 = true;
    double cosmological_constant = 0.0;
};

struct Input
{
    Reduced::Variables<double> vars;
    DerivativeJets derivatives;
    double x = 0.0;
    LockedCCZ4Parameters parameters;
};

struct Result
{
    // Ownership/decomposition evidence only:
    // hidden_increment_decomposition =
    //     target_full_grchombo - target_shared_hidden_suppressed.
    // The increment is not an independently implemented hidden CCZ4 RHS.
    Rows target_shared_hidden_suppressed = {};
    Rows hidden_increment_decomposition = {};
    Rows target_full_grchombo = {};
    std::array<OwnershipSplit, reported_families> family = {};
};

template <class data_t> struct TargetVars
{
    data_t chi;
    Tensor<2, data_t> h;
    data_t K;
    Tensor<2, data_t> A;
    data_t Theta;
    Tensor<1, data_t> Gamma;
    data_t lapse;
    Tensor<1, data_t> shift;
    Tensor<1, data_t> B;
};

template <class data_t> struct TargetDiff2Vars
{
    data_t chi;
    Tensor<2, data_t> h;
    data_t lapse;
    Tensor<1, data_t> shift;
};

struct FrozenGauge
{
    struct params_t
    {
    };

    explicit FrozenGauge(const params_t &) {}

    template <class data_t, template <typename> class vars_t,
              template <typename> class diff2_vars_t>
    void rhs_gauge(vars_t<data_t> &, const vars_t<data_t> &,
                   const vars_t<Tensor<1, data_t>> &,
                   const diff2_vars_t<Tensor<2, data_t>> &,
                   const vars_t<data_t> &) const
    {
    }
};

struct NoDerivatives
{
    explicit NoDerivatives(const double) {}
};

class LockedRHSAccess : public CCZ4RHS<FrozenGauge, NoDerivatives>
{
    using Base = CCZ4RHS<FrozenGauge, NoDerivatives>;

  public:
    using Base::USE_CCZ4;
    using Base::rhs_equation;

    explicit LockedRHSAccess(const LockedCCZ4Parameters &parameters)
        : Base(make_parameters(parameters), 1.0, 0.0, Base::USE_CCZ4,
               parameters.cosmological_constant)
    {
    }

  private:
    static typename Base::params_t
    make_parameters(const LockedCCZ4Parameters &parameters)
    {
        typename Base::params_t result{};
        result.kappa1 = parameters.kappa1;
        result.kappa2 = parameters.kappa2;
        result.kappa3 = parameters.kappa3;
        result.covariantZ4 = parameters.covariant_z4;
        return result;
    }
};

struct ExpandedInput
{
    TargetVars<double> vars;
    TargetVars<Tensor<1, double>> d1;
    TargetDiff2Vars<Tensor<2, double>> d2;
    TargetVars<double> advection;
};

inline std::size_t slot_index(const Production::VariableSlot slot)
{
    return static_cast<std::size_t>(slot);
}

inline const Jet &jet(const Input &input,
                      const Production::VariableSlot slot)
{
    return input.derivatives[slot_index(slot)];
}

inline void require_valid_input(const Input &input)
{
    if (!std::isfinite(input.x) || !(input.x > 0.0))
    {
        throw std::domain_error(
            "black-string target CCZ4 pointwise input requires x>0");
    }
    const auto storage = Reduced::store(input.vars);
    for (const double value : storage)
    {
        if (!std::isfinite(value))
        {
            throw std::domain_error(
                "black-string target CCZ4 state must be finite");
        }
    }
    for (const Jet &derivative : input.derivatives)
    {
        const double values[] = {derivative.dx, derivative.dz,
                                 derivative.dxx, derivative.dxz,
                                 derivative.dzz};
        for (const double value : values)
        {
            if (!std::isfinite(value))
            {
                throw std::domain_error(
                    "black-string target CCZ4 derivative jets must be finite");
            }
        }
    }
    const auto &parameters = input.parameters;
    if (parameters.kappa1 != 0.1 || parameters.kappa2 != 0.0 ||
        parameters.kappa3 != 1.0 || !parameters.covariant_z4 ||
        parameters.cosmological_constant != 0.0)
    {
        throw std::domain_error(
            "black-string target CCZ4 parameters must match the locked tuple");
    }
}

inline void zero_target_vars(TargetVars<double> &vars)
{
    vars.chi = 0.0;
    vars.K = 0.0;
    vars.Theta = 0.0;
    vars.lapse = 0.0;
    for (int i = 0; i < target_dimension; ++i)
    {
        vars.Gamma[i] = 0.0;
        vars.shift[i] = 0.0;
        vars.B[i] = 0.0;
        for (int j = 0; j < target_dimension; ++j)
        {
            vars.h[i][j] = 0.0;
            vars.A[i][j] = 0.0;
        }
    }
}

inline void zero_target_d1(TargetVars<Tensor<1, double>> &vars)
{
    for (int derivative = 0; derivative < target_dimension; ++derivative)
    {
        vars.chi[derivative] = 0.0;
        vars.K[derivative] = 0.0;
        vars.Theta[derivative] = 0.0;
        vars.lapse[derivative] = 0.0;
        for (int i = 0; i < target_dimension; ++i)
        {
            vars.Gamma[i][derivative] = 0.0;
            vars.shift[i][derivative] = 0.0;
            vars.B[i][derivative] = 0.0;
            for (int j = 0; j < target_dimension; ++j)
            {
                vars.h[i][j][derivative] = 0.0;
                vars.A[i][j][derivative] = 0.0;
            }
        }
    }
}

inline void zero_target_d2(TargetDiff2Vars<Tensor<2, double>> &vars)
{
    for (int first = 0; first < target_dimension; ++first)
    {
        for (int second = 0; second < target_dimension; ++second)
        {
            vars.chi[first][second] = 0.0;
            vars.lapse[first][second] = 0.0;
            for (int i = 0; i < target_dimension; ++i)
            {
                vars.shift[i][first][second] = 0.0;
                for (int j = 0; j < target_dimension; ++j)
                {
                    vars.h[i][j][first][second] = 0.0;
                }
            }
        }
    }
}

inline void set_reduced_tensor(Tensor<2, double> &target,
                               const Reduced::ReducedSymmetricTensor<double>
                                   &source)
{
    target[0][0] = source.xx;
    target[0][1] = target[1][0] = source.xz;
    target[1][1] = source.zz;
    target[2][2] = target[3][3] = source.ww;
}

inline void set_visible_tensor_derivatives(
    Tensor<2, Tensor<1, double>> &first,
    Tensor<2, Tensor<2, double>> &second, const Input &input,
    const Production::VariableSlot xx_slot,
    const Production::VariableSlot xz_slot,
    const Production::VariableSlot zz_slot,
    const Production::VariableSlot ww_slot)
{
    const std::array<Production::VariableSlot, 4> slots = {
        xx_slot, xz_slot, zz_slot, ww_slot};
    const std::array<std::array<int, 2>, 4> indices = {
        std::array<int, 2>{0, 0}, {0, 1}, {1, 1}, {2, 2}};
    for (std::size_t component = 0; component < slots.size(); ++component)
    {
        const int i = indices[component][0];
        const int j = indices[component][1];
        const Jet &component_jet = jet(input, slots[component]);
        first[i][j][0] = first[j][i][0] = component_jet.dx;
        first[i][j][1] = first[j][i][1] = component_jet.dz;
        second[i][j][0][0] = second[j][i][0][0] = component_jet.dxx;
        second[i][j][0][1] = second[j][i][0][1] = component_jet.dxz;
        second[i][j][1][0] = second[j][i][1][0] = component_jet.dxz;
        second[i][j][1][1] = second[j][i][1][1] = component_jet.dzz;
    }
    const Jet &ww_jet = jet(input, ww_slot);
    first[3][3][0] = ww_jet.dx;
    first[3][3][1] = ww_jet.dz;
    second[3][3][0][0] = ww_jet.dxx;
    second[3][3][0][1] = second[3][3][1][0] = ww_jet.dxz;
    second[3][3][1][1] = ww_jet.dzz;
}

inline void add_hidden_tensor_derivatives(
    Tensor<2, Tensor<1, double>> &first,
    Tensor<2, Tensor<2, double>> &second,
    const Reduced::ReducedSymmetricTensor<double> &value,
    const Input &input, const Production::VariableSlot xx_slot,
    const Production::VariableSlot xz_slot,
    const Production::VariableSlot zz_slot,
    const Production::VariableSlot ww_slot)
{
    const double x = input.x;
    const Jet &xx = jet(input, xx_slot);
    const Jet &xz = jet(input, xz_slot);
    const Jet &zz = jet(input, zz_slot);
    const Jet &ww = jet(input, ww_slot);
    for (const int hidden :
         {first_hidden_direction, second_hidden_direction})
    {
        const int other = hidden == first_hidden_direction
                              ? second_hidden_direction
                              : first_hidden_direction;
        first[hidden][0][hidden] = first[0][hidden][hidden] =
            (value.xx - value.ww) / x;
        first[hidden][1][hidden] = first[1][hidden][hidden] =
            value.xz / x;
        second[0][0][hidden][hidden] =
            xx.dx / x - 2.0 * (value.xx - value.ww) / (x * x);
        second[0][1][hidden][hidden] =
            second[1][0][hidden][hidden] =
                xz.dx / x - value.xz / (x * x);
        second[1][1][hidden][hidden] = zz.dx / x;
        second[hidden][hidden][hidden][hidden] =
            ww.dx / x + 2.0 * (value.xx - value.ww) / (x * x);
        second[other][other][hidden][hidden] = ww.dx / x;

        const double radial_x =
            (xx.dx - ww.dx) / x -
            (value.xx - value.ww) / (x * x);
        const double radial_z = xz.dx / x - value.xz / (x * x);
        second[hidden][0][hidden][0] =
            second[hidden][0][0][hidden] = radial_x;
        second[0][hidden][hidden][0] =
            second[0][hidden][0][hidden] = radial_x;
        second[hidden][1][hidden][0] =
            second[hidden][1][0][hidden] = radial_z;
        second[1][hidden][hidden][0] =
            second[1][hidden][0][hidden] = radial_z;

        const double axial_x = (xx.dz - ww.dz) / x;
        const double axial_z = xz.dz / x;
        second[hidden][0][hidden][1] =
            second[hidden][0][1][hidden] = axial_x;
        second[0][hidden][hidden][1] =
            second[0][hidden][1][hidden] = axial_x;
        second[hidden][1][1][hidden] =
            second[hidden][1][hidden][1] = axial_z;
        second[1][hidden][1][hidden] =
            second[1][hidden][hidden][1] = axial_z;
    }
    const double mixed_hidden = (value.xx - value.ww) / (x * x);
    second[2][3][2][3] = second[2][3][3][2] = mixed_hidden;
    second[3][2][2][3] = second[3][2][3][2] = mixed_hidden;
}

inline void set_scalar_jet(Tensor<1, double> &first,
                           Tensor<2, double> &second, const Jet &source,
                           const double x)
{
    first[0] = source.dx;
    first[1] = source.dz;
    second[0][0] = source.dxx;
    second[0][1] = second[1][0] = source.dxz;
    second[1][1] = source.dzz;
    second[2][2] = second[3][3] = source.dx / x;
}

inline void set_vector_jet(Tensor<1, Tensor<1, double>> &first,
                           Tensor<1, Tensor<2, double>> *second,
                           const Reduced::ReducedVisibleVector<double> &value,
                           const Jet &x_jet, const Jet &z_jet,
                           const double x)
{
    first[0][0] = x_jet.dx;
    first[0][1] = x_jet.dz;
    first[1][0] = z_jet.dx;
    first[1][1] = z_jet.dz;
    first[2][2] = first[3][3] = value.x / x;
    if (second == nullptr)
    {
        return;
    }
    auto &d2 = *second;
    d2[0][0][0] = x_jet.dxx;
    d2[0][0][1] = d2[0][1][0] = x_jet.dxz;
    d2[0][1][1] = x_jet.dzz;
    d2[1][0][0] = z_jet.dxx;
    d2[1][0][1] = d2[1][1][0] = z_jet.dxz;
    d2[1][1][1] = z_jet.dzz;
    for (const int hidden :
         {first_hidden_direction, second_hidden_direction})
    {
        d2[0][hidden][hidden] =
            x_jet.dx / x - value.x / (x * x);
        d2[1][hidden][hidden] = z_jet.dx / x;
        d2[hidden][0][hidden] = d2[hidden][hidden][0] =
            x_jet.dx / x - value.x / (x * x);
        d2[hidden][1][hidden] = d2[hidden][hidden][1] =
            x_jet.dz / x;
    }
}

inline ExpandedInput expand_target(const Input &input)
{
    require_valid_input(input);
    ExpandedInput result;
    zero_target_vars(result.vars);
    zero_target_vars(result.advection);
    zero_target_d1(result.d1);
    zero_target_d2(result.d2);

    const auto &source = input.vars;
    result.vars.chi = source.physical.chi;
    result.vars.K = source.physical.K;
    result.vars.Theta = source.physical.Theta;
    result.vars.lapse = source.gauge.lapse;
    set_reduced_tensor(result.vars.h, source.physical.h);
    set_reduced_tensor(result.vars.A, source.physical.A);
    result.vars.Gamma[0] = source.physical.Gamma.x;
    result.vars.Gamma[1] = source.physical.Gamma.z;
    result.vars.shift[0] = source.gauge.shift.x;
    result.vars.shift[1] = source.gauge.shift.z;
    result.vars.B[0] = source.gauge.B.x;
    result.vars.B[1] = source.gauge.B.z;

    set_scalar_jet(result.d1.chi, result.d2.chi,
                   jet(input, Production::c_chi), input.x);
    set_scalar_jet(result.d1.lapse, result.d2.lapse,
                   jet(input, Production::c_lapse), input.x);
    result.d1.K[0] = jet(input, Production::c_K).dx;
    result.d1.K[1] = jet(input, Production::c_K).dz;
    result.d1.Theta[0] = jet(input, Production::c_Theta).dx;
    result.d1.Theta[1] = jet(input, Production::c_Theta).dz;

    set_visible_tensor_derivatives(
        result.d1.h, result.d2.h, input, Production::c_hxx,
        Production::c_hxz, Production::c_hzz, Production::c_hww);
    add_hidden_tensor_derivatives(
        result.d1.h, result.d2.h, source.physical.h, input,
        Production::c_hxx, Production::c_hxz, Production::c_hzz,
        Production::c_hww);

    Tensor<2, Tensor<2, double>> unused_a_second = 0.0;
    set_visible_tensor_derivatives(
        result.d1.A, unused_a_second, input, Production::c_Axx,
        Production::c_Axz, Production::c_Azz, Production::c_Aww);

    set_vector_jet(result.d1.Gamma, nullptr, source.physical.Gamma,
                   jet(input, Production::c_GammaX),
                   jet(input, Production::c_GammaZ), input.x);
    set_vector_jet(result.d1.shift, &result.d2.shift, source.gauge.shift,
                   jet(input, Production::c_shiftX),
                   jet(input, Production::c_shiftZ), input.x);
    set_vector_jet(result.d1.B, nullptr, source.gauge.B,
                   jet(input, Production::c_Bx),
                   jet(input, Production::c_Bz), input.x);

    for (int slot = 0; slot < Production::NUM_VARS; ++slot)
    {
        const auto production_slot =
            static_cast<Production::VariableSlot>(slot);
        const Jet &derivative = jet(input, production_slot);
        const double advection =
            source.gauge.shift.x * derivative.dx +
            source.gauge.shift.z * derivative.dz;
        switch (production_slot)
        {
        case Production::c_chi:
            result.advection.chi = advection;
            break;
        case Production::c_hxx:
            result.advection.h[0][0] = advection;
            break;
        case Production::c_hxz:
            result.advection.h[0][1] =
                result.advection.h[1][0] = advection;
            break;
        case Production::c_hzz:
            result.advection.h[1][1] = advection;
            break;
        case Production::c_hww:
            result.advection.h[2][2] =
                result.advection.h[3][3] = advection;
            break;
        case Production::c_K:
            result.advection.K = advection;
            break;
        case Production::c_Axx:
            result.advection.A[0][0] = advection;
            break;
        case Production::c_Axz:
            result.advection.A[0][1] =
                result.advection.A[1][0] = advection;
            break;
        case Production::c_Azz:
            result.advection.A[1][1] = advection;
            break;
        case Production::c_Aww:
            result.advection.A[2][2] =
                result.advection.A[3][3] = advection;
            break;
        case Production::c_Theta:
            result.advection.Theta = advection;
            break;
        case Production::c_GammaX:
            result.advection.Gamma[0] = advection;
            break;
        case Production::c_GammaZ:
            result.advection.Gamma[1] = advection;
            break;
        case Production::c_lapse:
            result.advection.lapse = advection;
            break;
        case Production::c_shiftX:
            result.advection.shift[0] = advection;
            break;
        case Production::c_shiftZ:
            result.advection.shift[1] = advection;
            break;
        case Production::c_Bx:
            result.advection.B[0] = advection;
            break;
        case Production::c_Bz:
            result.advection.B[1] = advection;
            break;
        case Production::NUM_VARS:
            break;
        }
    }
    return result;
}

struct DefaultExpansionPolicy
{
    ExpandedInput expand(const Input &input) const
    {
        return expand_target(input);
    }
};

inline ExpandedInput suppress_hidden(ExpandedInput input)
{
    input.vars.h[2][2] = 1.0;
    input.vars.h[3][3] = 1.0;
    input.vars.A[2][2] = 0.0;
    input.vars.A[3][3] = 0.0;
    for (int first = 0; first < target_dimension; ++first)
    {
        input.d1.Gamma[2][first] = 0.0;
        input.d1.Gamma[3][first] = 0.0;
        input.d1.shift[2][first] = 0.0;
        input.d1.shift[3][first] = 0.0;
        input.d1.B[2][first] = 0.0;
        input.d1.B[3][first] = 0.0;
        input.d1.h[2][2][first] = 0.0;
        input.d1.h[3][3][first] = 0.0;
        input.d1.A[2][2][first] = 0.0;
        input.d1.A[3][3][first] = 0.0;
        for (int second = 0; second < target_dimension; ++second)
        {
            if (first >= gridded_dimension ||
                second >= gridded_dimension)
            {
                input.d2.chi[first][second] = 0.0;
                input.d2.lapse[first][second] = 0.0;
                for (int component = 0; component < target_dimension;
                     ++component)
                {
                    input.d2.shift[component][first][second] = 0.0;
                    for (int tensor = 0; tensor < target_dimension; ++tensor)
                    {
                        input.d2.h[component][tensor][first][second] = 0.0;
                    }
                }
            }
        }
    }
    for (int derivative = 0; derivative < target_dimension; ++derivative)
    {
        for (int i = 0; i < target_dimension; ++i)
        {
            if (derivative >= gridded_dimension ||
                i >= gridded_dimension)
            {
                input.d1.Gamma[i][derivative] = 0.0;
                input.d1.shift[i][derivative] = 0.0;
                input.d1.B[i][derivative] = 0.0;
            }
            for (int j = 0; j < target_dimension; ++j)
            {
                if (derivative >= gridded_dimension ||
                    i >= gridded_dimension || j >= gridded_dimension)
                {
                    input.d1.h[i][j][derivative] = 0.0;
                    input.d1.A[i][j][derivative] = 0.0;
                }
            }
        }
    }
    input.advection.h[2][2] = input.advection.h[3][3] = 0.0;
    input.advection.A[2][2] = input.advection.A[3][3] = 0.0;
    return input;
}

inline TargetVars<double> evaluate_direct(const ExpandedInput &input,
                                          const LockedCCZ4Parameters &params)
{
    TargetVars<double> rhs;
    zero_target_vars(rhs);
    const LockedRHSAccess access(params);
    access.rhs_equation(rhs, input.vars, input.d1, input.d2,
                        input.advection);
    return rhs;
}

inline Rows physical_rows_from(const TargetVars<double> &rhs)
{
    return {rhs.chi,       rhs.h[0][0], rhs.h[0][1], rhs.h[1][1],
            rhs.h[2][2],  rhs.K,       rhs.A[0][0], rhs.A[0][1],
            rhs.A[1][1],  rhs.A[2][2], rhs.Theta,   rhs.Gamma[0],
            rhs.Gamma[1]};
}

inline Rows subtract_rows(const Rows &left, const Rows &right)
{
    Rows result = {};
    for (std::size_t row = 0; row < physical_rows; ++row)
    {
        result[row] = left[row] - right[row];
    }
    return result;
}

inline Rows curvature_rows(const ExpandedInput &input,
                           const bool include_encoded_z)
{
    const auto inverse =
        TensorAlgebra::compute_inverse_sym(input.vars.h);
    const auto christoffel =
        TensorAlgebra::compute_christoffel(input.d1.h, inverse);
    Tensor<1, double> z_over_chi = 0.0;
    for (int i = 0; i < target_dimension; ++i)
    {
        z_over_chi[i] =
            0.5 * (input.vars.Gamma[i] - christoffel.contracted[i]);
    }
    const auto ricci =
        include_encoded_z
            ? CCZ4Geometry::compute_ricci_Z(
                  input.vars, input.d1, input.d2, inverse, christoffel,
                  z_over_chi)
            : CCZ4Geometry::compute_ricci(
                  input.vars, input.d1, input.d2, inverse, christoffel);

    TargetVars<double> contribution;
    zero_target_vars(contribution);
    contribution.K = input.vars.lapse * ricci.scalar;
    contribution.Theta = 0.5 * input.vars.lapse * ricci.scalar;
    for (int i = 0; i < target_dimension; ++i)
    {
        for (int j = 0; j < target_dimension; ++j)
        {
            contribution.A[i][j] =
                input.vars.chi * input.vars.lapse * ricci.LL[i][j];
        }
    }
    TensorAlgebra::make_trace_free(contribution.A, input.vars.h,
                                   inverse);
    return physical_rows_from(contribution);
}

inline Rows evaluate_without_lapse_derivatives(ExpandedInput input,
                                               const LockedCCZ4Parameters
                                                   &parameters)
{
    for (int i = 0; i < target_dimension; ++i)
    {
        input.d1.lapse[i] = 0.0;
        for (int j = 0; j < target_dimension; ++j)
        {
            input.d2.lapse[i][j] = 0.0;
        }
    }
    return physical_rows_from(evaluate_direct(input, parameters));
}

inline Rows evaluate_without_damping(
    const ExpandedInput &input, LockedCCZ4Parameters parameters)
{
    parameters.kappa1 = 0.0;
    parameters.kappa2 = 0.0;
    return physical_rows_from(evaluate_direct(input, parameters));
}

inline OwnershipSplit
make_ownership_split(const Rows &full, Rows shared)
{
    shared[static_cast<std::size_t>(PhysicalRow::hww)] = 0.0;
    shared[static_cast<std::size_t>(PhysicalRow::Aww)] = 0.0;
    OwnershipSplit result;
    result.target_full_grchombo = full;
    result.target_shared_hidden_suppressed = shared;
    result.hidden_increment_decomposition =
        subtract_rows(full, shared);
    return result;
}

template <class expansion_policy_t>
inline Result evaluate_with_expansion_policy(
    const Input &input, const expansion_policy_t &expansion_policy)
{
    require_valid_input(input);
    const ExpandedInput target = expansion_policy.expand(input);
    const ExpandedInput shared = suppress_hidden(target);
    Result result;
    result.target_full_grchombo =
        physical_rows_from(evaluate_direct(target, input.parameters));
    result.target_shared_hidden_suppressed =
        physical_rows_from(evaluate_direct(shared, input.parameters));
    result.target_shared_hidden_suppressed[
        static_cast<std::size_t>(PhysicalRow::hww)] = 0.0;
    result.target_shared_hidden_suppressed[
        static_cast<std::size_t>(PhysicalRow::Aww)] = 0.0;
    for (std::size_t row = 0; row < physical_rows; ++row)
    {
        result.hidden_increment_decomposition[row] =
            result.target_full_grchombo[row] -
            result.target_shared_hidden_suppressed[row];
    }

    const Rows target_geometric = curvature_rows(target, false);
    const Rows shared_geometric = curvature_rows(shared, false);
    result.family[static_cast<std::size_t>(
        ReportedFamily::geometric_ricci)] =
        make_ownership_split(target_geometric, shared_geometric);

    const Rows target_combined = curvature_rows(target, true);
    const Rows shared_combined = curvature_rows(shared, true);
    result.family[static_cast<std::size_t>(ReportedFamily::encoded_z)] =
        make_ownership_split(
            subtract_rows(target_combined, target_geometric),
            subtract_rows(shared_combined, shared_geometric));

    result.family[static_cast<std::size_t>(
        ReportedFamily::lapse_derivatives)] =
        make_ownership_split(
            subtract_rows(
                result.target_full_grchombo,
                evaluate_without_lapse_derivatives(target,
                                                   input.parameters)),
            subtract_rows(
                result.target_shared_hidden_suppressed,
                evaluate_without_lapse_derivatives(shared,
                                                   input.parameters)));

    result.family[static_cast<std::size_t>(ReportedFamily::damping)] =
        make_ownership_split(
            subtract_rows(result.target_full_grchombo,
                          evaluate_without_damping(target,
                                                   input.parameters)),
            subtract_rows(result.target_shared_hidden_suppressed,
                          evaluate_without_damping(shared,
                                                   input.parameters)));

    Rows accounted_target = {};
    Rows accounted_shared = {};
    for (std::size_t family = 0; family + 1 < reported_families; ++family)
    {
        for (std::size_t row = 0; row < physical_rows; ++row)
        {
            accounted_target[row] +=
                result.family[family].target_full_grchombo[row];
            accounted_shared[row] +=
                result.family[family]
                    .target_shared_hidden_suppressed[row];
        }
    }
    result.family[static_cast<std::size_t>(
        ReportedFamily::remaining_shared_algebra)] =
        make_ownership_split(
            subtract_rows(result.target_full_grchombo,
                          accounted_target),
            subtract_rows(result.target_shared_hidden_suppressed,
                          accounted_shared));
    return result;
}

inline Result evaluate(const Input &input)
{
    return evaluate_with_expansion_policy(input,
                                          DefaultExpansionPolicy{});
}

} // namespace BlackStringTargetCCZ4Pointwise

#endif /* BLACKSTRINGTARGETCCZ4POINTWISE_HPP */
