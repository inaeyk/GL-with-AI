# CCZ4 Cartoon Variables

## Purpose

Concise naming reference for the future 5D Gregory-Laflamme black-string implementation with SO(3) symmetry. The intended target is `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, so the code evolves an effective 2+1D system while reconstructing the suppressed S^2 by symmetry.

## Standard CCZ4 Variables

- `chi`: conformal factor.
- `h_ij`: conformal metric components.
- `A_ij`: conformal traceless extrinsic-curvature components.
- `K`: trace of the physical extrinsic curvature.
- `Gamma^i`: conformal connection functions.
- `Theta`: CCZ4 constraint-damping variable.
- `lapse`, `shift^i`, `B^i`: gauge variables.

## GRChombo Grid-Variable Naming

GRChombo stores variables through `c_` enum slots and string names in `UserVariables.hpp` / `CCZ4UserVariables.hpp`. Standard public examples map CCZ4 variables into names such as `chi`, `h11`, `A11`, `K`, `Theta`, `Gamma1`, `lapse`, `shift1`, and `B1`.

Exact enum order for any new `BlackStringToy/UserVariables.hpp` must be verified against public GRChombo source before enabling AH output.

## `c_` Enum-Index Convention

`c_foo` names are integer component indices into the flattened GRChombo grid-variable arrays. They are not symbolic tensors; ordering matters whenever code accesses variables by enum arithmetic or contiguous intervals.

## Cartoon-Method Extension Variables

- `hww`: conformal metric component in the absorbed cartoon direction.
- `Aww`: conformal traceless extrinsic-curvature component in the absorbed cartoon direction.
- `c_hww`: user-supplied enum slot for `hww`.
- `c_Aww`: user-supplied enum slot for `Aww`.

These are not the physical components `g_ww` and `K_ww`; they are conformal variables that must be reconstructed.

## Physical Reconstruction Formulas

```text
g_ww = chi^{-1} hww
```

```text
K_ww = chi^{-1} (Aww + hww K / GR_SPACEDIM)
```

Here GR_SPACEDIM is the full physical spatial dimension, not the spacetime dimension. For the 5D black-string project, `CH_SPACEDIM=2` and `GR_SPACEDIM=4`.

## Dimensional Macros

- `CH_SPACEDIM`: computational Chombo grid dimension.
- `GR_SPACEDIM`: physical spatial dimension used in GR formulas.
- Target convention: `CH_SPACEDIM=2`, `GR_SPACEDIM=4`.

## Fragility / Implementation Hazards

- Verified from public source: under `GR_SPACEDIM != CH_SPACEDIM`, `AHFunctions.hpp` sets `int comp_hww = c_K - 1;` and reads `hww` / `dhww` from that positional slot.
- Verified from public source: the same block reads `Aww` by name via `int comp_Aww = c_Aww;`, so the public convention is asymmetric: positional for `hww`, named for `Aww`.
- Future `UserVariables.hpp` enum ordering must preserve `c_hww == c_K - 1` before using the AH finder.
- A wrong enum order may compile but produce incorrect horizon data.
- `hww/Aww` are conformal variables; confusing them with `g_ww/K_ww` will corrupt horizon quantities such as `gamma_ww`.
- The `hww/Aww` enum-order issue is tracked as a gate in `research_plan/stage_checklists.md`.

## Implications For BlackStringToy

- Add `hww` and `Aww` deliberately, with explicit comments documenting their enum positions.
- Preserve the verified public AH convention `c_hww == c_K - 1`, and verify `c_Aww` still matches the expected named slot before enabling horizon geometry output.
- Keep reconstruction formulas close to horizon diagnostics and tests.
- Treat the source-level convention as verified, but still treat target-configuration compile/runtime behavior as needing validation.
