# GRChombo Capability Map

Purpose: map what public GRChombo appears to provide for the 5D black-string project, based on a shallow local inspection clone at `external/GRChombo`.

Status: first-pass source inspection. These maps must be reviewed and approved by the user before `BlackStringToy` or C++ edits.

Framing: public GRChombo main/default examples are primarily 3+1D. The target 5D black-string problem keeps SO(3) symmetry and should be treated as an effective 2+1D modified-cartoon problem, with the S^2 reconstructed by symmetry, not as a full 4-spatial-dimensional grid evolution. Read the table with that distinction in mind. We have access to Pau's code through collaboration, but are deliberately withholding it at this stage to benchmark AI-agent development against public GRChombo.

| Capability | Relevant files/directories | How it helps this project | Status |
|---|---|---|---|
| Example structure | `external/GRChombo/Examples/BinaryBH/`, `external/GRChombo/Examples/KerrBH/`, `external/GRChombo/Examples/ScalarFieldBH/` | Provides the expected pattern for `Main_*`, `*Level`, `SimulationParameters`, `UserVariables`, `DiagnosticVariables`, params files, and `GNUmakefile`. | usable now |
| Docker image / example-run workflow | `scripts/run_binarybh_smoke.sh`, `external/GRChombo/Examples/BinaryBH/params_very_cheap.txt` | The public Docker image successfully runs the BinaryBH very-cheap test locally under WSL2. Useful as a smoke-test baseline. | usable now |
| CCZ4 infrastructure | `external/GRChombo/Source/CCZ4/CCZ4RHS.hpp`, `CCZ4RHS.impl.hpp`, `CCZ4Geometry.hpp`, `CCZ4Vars.hpp`, `CCZ4UserVariables.hpp`, `Constraints.hpp` | Provides the core vacuum CCZ4/BSSN RHS, variables, geometry, and constraints used by examples. | promising but needs inspection |
| Moving-puncture / integrated gauge infrastructure | `external/GRChombo/Source/CCZ4/MovingPunctureGauge.hpp`, `IntegratedMovingPunctureGauge.hpp`, `SimulationParametersBase.hpp` | Provides 1+log-like lapse and Gamma-driver shift parameters; integrated gauge class exists but BinaryBH currently instantiates `MovingPunctureGauge`. | promising but needs inspection |
| Parameter parsing | `external/GRChombo/Source/GRChomboCore/ChomboParameters.hpp`, `SimulationParametersBase.hpp`, `Examples/BinaryBH/SimulationParameters.hpp` | Defines grid, boundary, AMR, output, gauge, CCZ4, extraction, AH, and problem-specific parameters. | usable now |
| Output/HDF5 infrastructure | `external/GRChombo/Source/GRChomboCore/GRAMRLevel.cpp`, `SetupFunctions.hpp`, `ChomboParameters.hpp`, `Examples/BinaryBH/params*.txt` | Handles checkpoint/plot intervals, prefixes, HDF5 paths, restart files, and selected plot variables. | usable now |
| AMR infrastructure | `external/GRChombo/Source/GRChomboCore/GRAMR.hpp`, `GRAMRLevel.hpp`, `DefaultLevelFactory.hpp`, `SetupFunctions.hpp` | Provides Chombo AMR setup, RK4 level evolution hooks, regridding, ghost filling, and AMR level specialization points. | usable now |
| Tagging/regridding criteria | `external/GRChombo/Source/TaggingCriteria/`, `Examples/BinaryBH/BinaryBHLevel.cpp` | Provides examples of chi/extraction/puncture tagging and the hook for custom black-string tagging. | promising but needs inspection |
| Apparent-horizon finder | `external/GRChombo/Source/ApparentHorizonFinder/`, `Examples/BinaryBH/Main_BinaryBH.cpp`, `BinaryBHLevel.cpp` | Provides PETSc-based AH solve, stats/coordinate output, geometry data output, and example wiring under `USE_AHFINDER`. | promising but needs inspection |
| String-like apparent-horizon geometry | `external/GRChombo/Source/ApparentHorizonFinder/AHStringGeometry.hpp`, `Tests/ApparentHorizonFinderTest2D/` | A periodic string geometry exists for 2D AH tests; this may be directly relevant to horizon curves along a string. | promising but needs inspection |
| Initial-condition infrastructure | `external/GRChombo/Source/InitialConditions/BlackHoles/`, `Examples/BinaryBH/BinaryBHLevel.cpp`, `Examples/KerrBH/` | Shows compute-class pattern for writing fields into `m_state_new`; black-hole examples can guide a black-string initial-data class. | usable now |
| Boundary conditions | `external/GRChombo/Source/GRChomboCore/BoundaryConditions.hpp`, `ChomboParameters.hpp`, `SetupFunctions.hpp` | Supports periodic, Sommerfeld, reflective, extrapolating, and mixed boundary handling through params. Compact `z` periodicity should reuse this pattern. | promising but needs inspection |
| Radiation / Weyl extraction | `external/GRChombo/Source/CCZ4/Weyl4.hpp`, `Examples/BinaryBH/BinaryBHLevel.cpp`, `Source/AMRInterpolator/SphericalExtraction.hpp` | Existing 3+1 Weyl4 extraction is useful as a later design reference, but the 5D radiation target is not solved by this directly. | uncertain |
| Higher-dimensional / cartoon hooks | `external/GRChombo/Source/utils/DimensionDefinitions.hpp`, `Source/ApparentHorizonFinder/AHFunctions.hpp`, `ApparentHorizon.hpp`, `Source/utils/Coordinates.hpp` | Code uses `GR_SPACEDIM`, `CH_SPACEDIM`, and extra metric components in AH functions; comments mention 4+1 cartoon-reduced to 2+1 AH support. | promising but needs inspection |
| `GR_SPACEDIM` / `CH_SPACEDIM` separation | `external/GRChombo/Source/utils/DimensionDefinitions.hpp`, `Source/CCZ4/`, `Source/ApparentHorizonFinder/`, `Source/utils/TensorAlgebra.hpp`, `Tests/` | Read-only search found 114 `GR_SPACEDIM` occurrences across CCZ4, constraints, AH finder, tensor algebra, and tests. Default `GR_SPACEDIM` is 3. This is a positive sign for dimension-aware infrastructure, but it still needs verification whether public GRChombo has a complete 4+1/SO(3) black-string driver. | promising but needs inspection |
| `gww` and `Kww` cartoon data | `external/GRChombo/Source/ApparentHorizonFinder/AHFunctions.hpp`, `Source/ApparentHorizonFinder/AHFunctionDefault.hpp` | `gww` and `Kww` are relevant to the cartoon `w`-direction metric/extrinsic-curvature data and directly relate to the reproduction plan's horizon quantity `gamma_ww`. | promising but needs inspection |
| `Tests/ApparentHorizonFinderTest2D` validation target | `external/GRChombo/Tests/ApparentHorizonFinderTest2D/` | High-value Stage 1 validation target before `BlackStringToy`; may be the closest public validation harness for string-like AH geometry. | promising but needs inspection |
| 4+1 CCZ4 / SO(3) modified cartoon evolution | `Source/CCZ4/` uses `GR_SPACEDIM`, but no dedicated SO(3) black-string example was found in first-pass grep. | May provide parts of dimensional support, but no complete Pau-style evolution driver was identified. | uncertain |

## Likely Missing For Pau Reproduction

- 5D black-string initial data.
- Compact `z` setup specific to the black-string domain.
- Gregory-Laflamme perturbation controls.
- Complete 4+1 CCZ4 / SO(3) modified cartoon implementation, unless later inspection confirms it is already present.
- Black-string-specific AMR strategy, including neck tracking and rectangular refinement boxes.
- Horizon extraction pipeline for `R(t,z)`, `A/A0`, `R_min`, Fourier modes, and generation detection.
- Radiation extraction for the 5D black-string transition.

## Inspection Notes

- Searches for `black string`, `Gregory`, `Laflamme`, and `modified cartoon` did not find a ready-made black-string example.
- `AHStringGeometry.hpp` is real and tested in `Tests/ApparentHorizonFinderTest2D`, but its suitability for 4+1 cartoon black-string diagnostics needs review.
- Hazard: keep `hww/Aww` distinct from reconstructed `g_ww/K_ww`; verified from public source, `AHFunctions.hpp` reads `hww` positionally via `c_K - 1`, so future `UserVariables.hpp` ordering needs verification.
- Public GRChombo appears to provide strong reusable infrastructure, but the project-specific physics and diagnostics should be treated as new development until verified otherwise.

## Hazards and Silent-Failure Modes

- Modified-cartoon CCZ4 hazard:
  The target configuration is `CH_SPACEDIM=2`, `GR_SPACEDIM=4`. Public CCZ4 appears to use `GR_SPACEDIM` in algebraic dimension-dependent coefficients, but the public source map does not yet show a complete modified-cartoon source-term implementation for `GR_SPACEDIM != CH_SPACEDIM`.
- Modified-cartoon CCZ4 hazard:
  A target-configuration build or run could therefore compile, step forward, and write HDF5 while still omitting source terms that represent the absorbed `S^2` geometry. That would be physically wrong even if constraints, residuals, or plots look numerically healthy.
- Modified-cartoon CCZ4 hazard:
  Do not treat any `GR_SPACEDIM=4` / `CH_SPACEDIM=2` output as physical until the modified-cartoon CCZ4 source terms are implemented, inspected, and tested on controlled cases.
- `hww/Aww` enum-order hazard:
  The AH finder expects cartoon-direction data in specific variable slots. Verified from public source, `AHFunctions.hpp` reconstructs the extra metric component with positional access `comp_hww = c_K - 1`, then reads `hww` from `a_data.vars.at(comp_hww)`, while `Aww` is read separately through `c_Aww`.
- `hww/Aww` enum-order hazard:
  Incorrect `UserVariables.hpp` ordering could therefore compile and still produce wrong horizon quantities such as reconstructed `g_ww`, `K_ww`, or downstream horizon diagnostics.
- `hww/Aww` enum-order hazard:
  The exact convention is now verified from public source for `hww`, but any future `BlackStringToy` variable list must preserve that layout before AH output is enabled.
