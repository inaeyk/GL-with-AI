#define BLACKSTRINGTOY_LAYOUT_TEST_ONLY
#include "UserVariables.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace
{
struct Binding
{
    const char *helper_input;
    int component;
    const char *component_name;
    const char *variable_name;
};

static_assert(c_hww == c_K - 1, "hww must be immediately before K");
static_assert(c_Aww == c_Theta - 1,
              "Aww must be immediately before Theta");
static_assert(NUM_VARS == 27,
              "Stage 4C BlackStringToy layout should contain 27 variables");

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

int lookup_name(const std::string &name)
{
    for (int i = 0; i < NUM_VARS; ++i)
    {
        if (UserVariables::variable_names[i] == name)
        {
            return i;
        }
    }
    return -1;
}

void require_component(const std::string &label, const int value,
                       const int expected)
{
    if (value != expected)
    {
        fail(label, "got component " + std::to_string(value) +
                        ", expected " + std::to_string(expected));
    }
    std::cout << "PASS " << label << " = " << value << "\n";
}

void require_name(const std::string &label, const int component,
                  const std::string &expected_name)
{
    if (component < 0 || component >= NUM_VARS)
    {
        fail(label, "component out of range: " + std::to_string(component));
    }

    const std::string actual = UserVariables::variable_names[component];
    if (actual != expected_name)
    {
        fail(label, "component " + std::to_string(component) + " is '" +
                        actual + "', expected '" + expected_name + "'");
    }
    std::cout << "PASS " << label << " -> " << actual << " at " << component
              << "\n";
}

void require_lookup(const std::string &name, const int expected_component)
{
    require_component("lookup " + name, lookup_name(name), expected_component);
}

void check_binding(const Binding &binding)
{
    require_name(std::string(binding.helper_input) + " gets " +
                     binding.component_name,
                 binding.component, binding.variable_name);
}

void check_stage4c_layout()
{
    require_component("NUM_VARS", NUM_VARS, 27);

    require_component("c_hww exists", c_hww, 7);
    require_component("c_K follows c_hww", c_K, 8);
    require_component("c_Aww exists", c_Aww, 15);
    require_component("c_Theta follows c_Aww", c_Theta, 16);
    require_component("c_Gamma1 follows c_Theta", c_Gamma1, 17);

    require_component("c_hww == c_K - 1", c_hww, c_K - 1);
    require_component("c_Aww == c_Theta - 1", c_Aww, c_Theta - 1);

    require_lookup("hww", c_hww);
    require_lookup("Aww", c_Aww);

    const Binding helper_bindings[] = {
        {"helper.chi", c_chi, "c_chi", "chi"},
        {"helper.h_xx", c_h11, "c_h11", "h11"},
        {"helper.h_xz", c_h12, "c_h12", "h12"},
        {"helper.h_zz", c_h22, "c_h22", "h22"},
        {"helper.hww", c_hww, "c_hww", "hww"},
        {"helper.A_xx", c_A11, "c_A11", "A11"},
        {"helper.A_xz", c_A12, "c_A12", "A12"},
        {"helper.A_zz", c_A22, "c_A22", "A22"},
        {"helper.Aww", c_Aww, "c_Aww", "Aww"},
        {"helper.K", c_K, "c_K", "K"},
        {"helper.Theta", c_Theta, "c_Theta", "Theta"},
        {"helper.hat_Gamma^x", c_Gamma1, "c_Gamma1", "Gamma1"},
        {"helper.hat_Gamma^z", c_Gamma2, "c_Gamma2", "Gamma2"},
    };

    for (const Binding &binding : helper_bindings)
    {
        check_binding(binding);
    }
}

} // namespace

int main()
{
    std::cout << "Stage 4C BlackStringToy hidden-variable placement fixture\n";
    std::cout << "Header-level enum placement check only; no grid variables, "
                 "Cell, Vars, FArrayBox, or evolution data are read.\n";

    check_stage4c_layout();

    std::cout << "All Stage 4C hidden-variable placement checks passed.\n";
    return 0;
}
