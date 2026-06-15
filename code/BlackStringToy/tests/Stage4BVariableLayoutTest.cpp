#include "CCZ4UserVariables.hpp"

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

constexpr int reduced_x_direction = 1;
constexpr int reduced_z_direction = 2;
constexpr int slot_before_K = c_K - 1;
constexpr int slot_before_Theta = c_Theta - 1;

static_assert(c_chi == 0, "Expected chi to remain the first CCZ4 variable");
static_assert(c_h11 == c_chi + 1, "Expected h11 immediately after chi");
static_assert(c_h12 == c_h11 + 1, "Expected contiguous visible metric block");
static_assert(c_h13 == c_h12 + 1, "Expected contiguous visible metric block");
static_assert(c_h22 == c_h13 + 1, "Expected contiguous visible metric block");
static_assert(c_h23 == c_h22 + 1, "Expected contiguous visible metric block");
static_assert(c_h33 == c_h23 + 1, "Expected contiguous visible metric block");
static_assert(c_K == c_h33 + 1, "Expected K immediately after metric block");
static_assert(c_A11 == c_K + 1, "Expected A block immediately after K");
static_assert(c_A12 == c_A11 + 1, "Expected contiguous visible A block");
static_assert(c_A13 == c_A12 + 1, "Expected contiguous visible A block");
static_assert(c_A22 == c_A13 + 1, "Expected contiguous visible A block");
static_assert(c_A23 == c_A22 + 1, "Expected contiguous visible A block");
static_assert(c_A33 == c_A23 + 1, "Expected contiguous visible A block");
static_assert(c_Theta == c_A33 + 1, "Expected Theta immediately after A block");
static_assert(c_Gamma1 == c_Theta + 1,
              "Expected Gamma block immediately after Theta");
static_assert(c_Gamma2 == c_Gamma1 + 1, "Expected contiguous Gamma block");
static_assert(c_Gamma3 == c_Gamma2 + 1, "Expected contiguous Gamma block");
static_assert(NUM_CCZ4_VARS == c_B3 + 1,
              "Expected NUM_CCZ4_VARS to terminate the CCZ4 enum");

void fail(const std::string &label, const std::string &details)
{
    std::cerr << "FAIL " << label << ": " << details << "\n";
    std::exit(1);
}

void note(const std::string &label)
{
    std::cout << "NOTE " << label << "\n";
}

int lookup_name(const std::string &name)
{
    for (int i = 0; i < NUM_CCZ4_VARS; ++i)
    {
        if (UserVariables::ccz4_variable_names[i] == name)
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
    if (component < 0 || component >= NUM_CCZ4_VARS)
    {
        fail(label, "component out of range: " + std::to_string(component));
    }

    const std::string actual = UserVariables::ccz4_variable_names[component];
    if (actual != expected_name)
    {
        fail(label, "component " + std::to_string(component) + " is '" +
                        actual + "', expected '" + expected_name + "'");
    }
    std::cout << "PASS " << label << " -> " << actual << " at " << component
              << "\n";
}

void require_absent(const std::string &name)
{
    const int component = lookup_name(name);
    if (component >= 0)
    {
        fail("current public CCZ4 variable absence for " + name,
             "found at component " + std::to_string(component));
    }
    std::cout << "PASS current public CCZ4 has no named " << name
              << " component yet\n";
}

void check_binding(const Binding &binding)
{
    require_name(std::string(binding.helper_input) + " gets " +
                     binding.component_name,
                 binding.component, binding.variable_name);
}

void check_current_public_layout()
{
    require_component("reduced x direction label", reduced_x_direction, 1);
    require_component("reduced z direction label", reduced_z_direction, 2);
    require_component("NUM_CCZ4_VARS", NUM_CCZ4_VARS, 25);

    const Binding current_bindings[] = {
        {"helper.chi", c_chi, "c_chi", "chi"},
        {"helper.h_xx", c_h11, "c_h11", "h11"},
        {"helper.h_xz", c_h12, "c_h12", "h12"},
        {"helper.h_zz", c_h22, "c_h22", "h22"},
        {"helper.A_xx", c_A11, "c_A11", "A11"},
        {"helper.A_xz", c_A12, "c_A12", "A12"},
        {"helper.A_zz", c_A22, "c_A22", "A22"},
        {"helper.K", c_K, "c_K", "K"},
        {"helper.Theta", c_Theta, "c_Theta", "Theta"},
        {"helper.hat_Gamma^x", c_Gamma1, "c_Gamma1", "Gamma1"},
        {"helper.hat_Gamma^z", c_Gamma2, "c_Gamma2", "Gamma2"},
    };

    for (const Binding &binding : current_bindings)
    {
        check_binding(binding);
    }

    require_absent("hww");
    require_absent("Aww");
    require_name("current public slot c_K - 1 is h33, not hww",
                 slot_before_K, "h33");
    require_name("current public slot c_Theta - 1 is A33, not Aww",
                 slot_before_Theta, "A33");
}

void document_future_stage4c_requirements()
{
    note("Stage 4B proves only the public CCZ4 baseline layout.");
    note("Stage 4B does not prove hww/Aww placement because c_hww and c_Aww do not exist yet.");
    note("Stage 4C must add real repo-owned hidden enum symbols before grid wiring.");
    note("After c_hww exists, add static_assert(c_hww == c_K - 1, ...).");
    note("After c_Aww exists, add static_assert(c_Aww == c_Theta - 1, ...).");
}

} // namespace

int main()
{
    std::cout << "Stage 4B BlackStringToy variable-layout fixture\n";
    std::cout << "Public GRChombo CCZ4 baseline-layout check only; no grid "
                 "variables, Cell, Vars, FArrayBox, or evolution data are "
                 "read.\n";
    std::cout << "Current BlackStringToy UserVariables.hpp aliases public "
                 "CCZ4 variables; hidden hww/Aww enum edits are deferred.\n";

    check_current_public_layout();
    document_future_stage4c_requirements();

    std::cout << "All Stage 4B public CCZ4 baseline-layout checks passed.\n";
    return 0;
}
