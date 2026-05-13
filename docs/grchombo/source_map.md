# GRChombo Source Map

Purpose: path-oriented map of the public GRChombo source inspected under `external/GRChombo`.

Status: first-pass inspection. Do not create `BlackStringToy` or edit GRChombo C++ until this map is reviewed and approved by the user.

## Top-Level Repo Structure

- `external/GRChombo/README.md`
  - Description: upstream project overview.
  - Why we care: confirms this is the public source being inspected.
  - Inspect next: build prerequisites and version/branch expectations.

- `external/GRChombo/GNUmakefile`
  - Description: top-level make targets for tests and examples.
  - Why we care: shows the upstream build walks `Tests/*/GNUmakefile` and `Examples/*/GNUmakefile`.
  - Inspect next: editable rebuild workflow outside Docker.

- `external/GRChombo/Source/`
  - Description: core reusable GRChombo infrastructure.
  - Why we care: source of CCZ4, AMR, AH finder, interpolation, initial data, and tagging classes.
  - Inspect next: include dependencies needed by a new example.

- `external/GRChombo/Examples/`
  - Description: runnable example applications: `BinaryBH`, `KerrBH`, scalar-field examples.
  - Why we care: best template for a new `BlackStringToy` example.
  - Inspect next: minimal file set needed for a new example directory.

- `external/GRChombo/Tests/`
  - Description: unit/integration tests for CCZ4, AH finder, interpolation, extraction, constraints, variables.
  - Why we care: tests reveal supported dimensions and expected APIs.
  - Inspect next: `ApparentHorizonFinderTest2D`, `CCZ4Test`, and extraction tests.

## Examples/BinaryBH Anatomy

- `external/GRChombo/Examples/BinaryBH/Main_BinaryBH.cpp`
  - Description: parses params, creates `BHAMR`/`TPAMR`, constructs `DefaultLevelFactory<BinaryBHLevel>`, sets `AMRInterpolator`, optionally wires AH finder, then runs AMR.
  - Why we care: main driver template for new examples.
  - Inspect next: how `BHAMR` differs from plain `GRAMR` and whether black-string work needs puncture tracking removed.

- `external/GRChombo/Examples/BinaryBH/BinaryBHLevel.hpp` and `.cpp`
  - Description: problem-specific AMR level hooks: initial data, RHS, tagging, post-timestep diagnostics, AH solve, constraints, Weyl extraction, pre-plot diagnostics.
  - Why we care: this is the clearest pattern for a black-string level class.
  - Inspect next: minimal `specificEvalRHS`, `initialData`, `computeTaggingCriterion`, and `specificPostTimeStep` needed for a toy run.

- `external/GRChombo/Examples/BinaryBH/SimulationParameters.hpp`
  - Description: extends `SimulationParametersBase` with BinaryBH-specific masses, centers, momenta, puncture tracking, and AH options.
  - Why we care: template for adding `r0`, `L`, perturbation amplitude/mode, compact direction, and diagnostic cadence.
  - Inspect next: parameter validation style and defaults.

- `external/GRChombo/Examples/BinaryBH/UserVariables.hpp`
  - Description: uses the standard CCZ4 variable set as evolution variables.
  - Why we care: black-string cartoon support may require extra variables such as `hww`/`Aww` if not already included by dimension settings.
  - Inspect next: interaction among `GR_SPACEDIM`, `CH_SPACEDIM`, and `CCZ4UserVariables.hpp`.

- `external/GRChombo/Examples/BinaryBH/DiagnosticVariables.hpp`
  - Description: declares Hamiltonian/momentum constraints and Weyl4 diagnostic variables.
  - Why we care: black-string diagnostics will need additional diagnostic variables and/or small-data outputs.
  - Inspect next: whether AH geometry output can supply enough horizon metric data without new grid diagnostics.

- `external/GRChombo/Examples/BinaryBH/GNUmakefile`
  - Description: example build file; includes source dirs for utils, simd, CCZ4, core, AMR interpolation, tagging, AH finder, initial conditions, and black holes.
  - Why we care: likely template for the first editable rebuild test.
  - Inspect next: compile flags for `USE_AHFINDER`, `CH_SPACEDIM=2`, and `GR_SPACEDIM=4`.

## Evolution Systems / CCZ4

- `external/GRChombo/Source/CCZ4/CCZ4RHS.hpp` and `.impl.hpp`
  - Description: templated CCZ4/BSSN RHS compute class.
  - Why we care: likely starting point for 4+1 vacuum evolution if the dimension macros and variables support it.
  - Inspect next: terms guarded by `GR_SPACEDIM != CH_SPACEDIM` and whether SO(3) source terms are complete.

- `external/GRChombo/Source/CCZ4/CCZ4Geometry.hpp`
  - Description: conformal geometry and Ricci-related helper functions.
  - Why we care: dimensional correctness is critical for 4+1 CCZ4.
  - Inspect next: every `GR_SPACEDIM` use under reduced dimensions.

- `external/GRChombo/Source/CCZ4/CCZ4Vars.hpp` and `CCZ4UserVariables.hpp`
  - Description: CCZ4 variable structs and grid variable enum/names.
  - Why we care: determines available evolution fields, including possible higher-dimensional components.
  - Inspect next: variable list when compiling with `GR_SPACEDIM != CH_SPACEDIM`.

- `external/GRChombo/Source/CCZ4/Constraints.hpp`, `NewConstraints.hpp`
  - Description: constraint diagnostics.
  - Why we care: reproduction plan requires constraint/gauge checks during GL growth.
  - Inspect next: whether momentum constraints include all needed directions for 2+1 cartoon reductions.

## Gauge

- `external/GRChombo/Source/CCZ4/MovingPunctureGauge.hpp`
  - Description: Bona-Masso/1+log-like lapse and Gamma-driver shift RHS.
  - Why we care: BinaryBH uses this gauge in `specificEvalRHS`.
  - Inspect next: parameter choices matching Pau/Figueras-Andrade-Franca-Gu.

- `external/GRChombo/Source/CCZ4/IntegratedMovingPunctureGauge.hpp`
  - Description: integrated Gamma-driver variant.
  - Why we care: Pau reproduction plan calls for integrated Gamma-driver shift.
  - Inspect next: how to initialize `B^i` and swap this gauge into the RHS.

- `external/GRChombo/Source/GRChomboCore/SimulationParametersBase.hpp`
  - Description: reads lapse, shift, damping, and formulation parameters.
  - Why we care: exposes `lapse_coeff`, `lapse_power`, `shift_Gamma_coeff`, `eta`, `kappa*`, `sigma`, and `formulation`.
  - Inspect next: names and ranges for desired reproduction params.

## Parameters

- `external/GRChombo/Source/GRChomboCore/ChomboParameters.hpp`
  - Description: reads grid size, boundaries, AMR levels, regrid thresholds, output intervals, checkpoint/plot prefixes, paths, and plot variables.
  - Why we care: compact `z`, rectangular grids, output cadence, and checkpoint layout will be configured here.
  - Inspect next: per-direction `N`, `N_full`, `L`, periodicity, and boundary parameter syntax in example params files.

- `external/GRChombo/Source/GRChomboCore/SimulationParametersBase.hpp`
  - Description: common physics/numerics parameters layered on top of `ChomboParameters`.
  - Why we care: provides shared CCZ4, gauge, extraction, and AH parameter loading.
  - Inspect next: AH parameter activation and extraction parameters.

- `external/GRChombo/Examples/BinaryBH/params_very_cheap.txt`
  - Description: known-good smoke-test parameter file for Docker run.
  - Why we care: baseline for script and output expectations.
  - Inspect next: minimal parameter set for an editable local rebuild test.

## Variables/Output

- `external/GRChombo/Source/GRChomboCore/UserVariables.inc.hpp`
  - Description: helper include for variable name loading and plot variable selection.
  - Why we care: plot/HDF5 output depends on variable enum/name consistency.
  - Inspect next: how to add black-string diagnostic variables without breaking checkpoint compatibility.

- `external/GRChombo/Source/GRChomboCore/GRAMRLevel.cpp`
  - Description: checkpoint and plot HDF5 read/write implementation.
  - Why we care: output and restart format for all runs.
  - Inspect next: variable-name metadata and restart mismatch handling.

- `external/GRChombo/Source/GRChomboCore/SetupFunctions.hpp`
  - Description: creates output directories, configures plot/checkpoint intervals, and sets AMR/regrid options.
  - Why we care: links params to actual `hdf5/` and data output.
  - Inspect next: path handling for project run directories.

- `external/GRChombo/Source/utils/SmallDataIO*`
  - Description: small text output helpers used by constraint norms and AH stats.
  - Why we care: good fit for time-series diagnostics such as area, minimum radius, and run metadata.
  - Inspect next: restart-safe append behavior.

## AMR And Tagging

- `external/GRChombo/Source/GRChomboCore/GRAMR.hpp` and `GRAMRLevel.hpp`
  - Description: AMR object and per-level base class with hooks for initial data, RHS, tagging, plot/checkpoint, and post-timestep work.
  - Why we care: all black-string evolution code will plug into these hooks.
  - Inspect next: lifecycle order around regrid, ghost filling, and post-timestep diagnostics.

- `external/GRChombo/Source/TaggingCriteria/`
  - Description: reusable tagging criteria including chi, extraction, fixed grids, and matter-related examples.
  - Why we care: black-string neck tracking likely needs custom tagging.
  - Inspect next: `FixedGridsTaggingCriterion.hpp` and chi-based examples.

- `external/GRChombo/Examples/BinaryBH/BinaryBHLevel.cpp`
  - Description: uses `ChiExtractionTaggingCriterion` and `ChiPunctureExtractionTaggingCriterion`.
  - Why we care: example of computing the tagging criterion from current state and diagnostics.
  - Inspect next: replace puncture-centered logic with string/neck-aware refinement.

## Initial Data

- `external/GRChombo/Source/InitialConditions/BlackHoles/BinaryBH.hpp`
  - Description: approximate binary black-hole initial data compute class.
  - Why we care: demonstrates field-writing compute class style.
  - Inspect next: variable storage order and initialization of lapse/shift.

- `external/GRChombo/Source/InitialConditions/BlackHoles/KerrBH.hpp`
  - Description: Kerr black-hole initial data.
  - Why we care: useful comparison for single-BH fields and gauge initialization.
  - Inspect next: whether any coordinate handling can be reused.

- `external/GRChombo/Source/InitialConditions/BlackHoles/TwoPuncturesInitialData.hpp`
  - Description: optional TwoPunctures-based initial data integration.
  - Why we care: not directly for black strings, but useful as an example of external initial-data solver wiring.
  - Inspect next: ignore unless editable build needs TwoPunctures disabled.

## Apparent Horizon Finder

- `external/GRChombo/Source/ApparentHorizonFinder/AHFinder.hpp` and `.impl.hpp`
  - Description: manager for one or more apparent horizons and mergers.
  - Why we care: likely interface for horizon solves in black-string diagnostics.
  - Inspect next: adding a string horizon rather than spherical BH horizons.

- `external/GRChombo/Source/ApparentHorizonFinder/ApparentHorizon.hpp` and `.impl.hpp`
  - Description: PETSc-backed AH solve, convergence tracking, stats output, coordinate output, and optional geometry data output.
  - Why we care: reproduction diagnostics need horizon coordinates and metric data.
  - Inspect next: `AH_print_geometry_data` output format and whether it gives `gamma_xx`, `gamma_xz`, `gamma_zz`, `gamma_ww`.

- `external/GRChombo/Source/ApparentHorizonFinder/AHStringGeometry.hpp`
  - Description: 2D periodic string geometry; string assumed along `x`, solve coordinate named `y`.
  - Why we care: closest public source match for string-like AH surfaces.
  - Inspect next: coordinate orientation versus desired `x = h(t,z)` convention and 4+1 cartoon compatibility, using `external/GRChombo/Tests/ApparentHorizonFinderTest2D/` as the first validation target.

- `external/GRChombo/Tests/ApparentHorizonFinderTest2D/`
  - Description: test wiring `AHStringGeometry` through `AHFinder`.
  - Why we care: minimal example of string geometry usage.
  - Inspect next: run and understand this test before adapting the pattern to physical CCZ4/cartoon evolution, and only after source-map review and approval by the user.

## Boundary Conditions

- `external/GRChombo/Source/GRChomboCore/BoundaryConditions.hpp` and `.cpp`
  - Description: periodic, Sommerfeld, reflective, extrapolating, and mixed boundary handling.
  - Why we care: black-string runs need compact periodic `z` and outer radial boundary handling.
  - Inspect next: exact params syntax and parity requirements for reflective/cartoon boundaries.

- `external/GRChombo/Source/GRChomboCore/ChomboParameters.hpp`
  - Description: reads boundary params and sets `ProblemDomain` periodicity through `SetupFunctions`.
  - Why we care: domain setup is param-driven.
  - Inspect next: per-direction boundary settings in examples.

## Build System

- `external/GRChombo/GNUmakefile`
  - Description: top-level recursive make.
  - Why we care: editable rebuild will likely compile a single example first, then broader tests if needed.
  - Inspect next: local `CHOMBO_HOME`, compiler, HDF5, PETSc/AH flags.

- `external/GRChombo/Examples/BinaryBH/GNUmakefile`
  - Description: example-specific source directory list and `Main_BinaryBH` executable base.
  - Why we care: direct template for `BlackStringToy/GNUmakefile`.
  - Inspect next: configuration variables used to include AH finder and dimension macros.

- `external/GRChombo/InstallNotes/`
  - Description: upstream install guidance.
  - Why we care: likely needed for editable WSL2 build.
  - Inspect next: WSL2-compatible Make.defs and dependency notes.

## Higher-Dimensional / Cartoon Findings

### GR_SPACEDIM search summary

- Total occurrences: 114 read-only matches in `external/GRChombo`.
- Key files/directories: `Source/utils/DimensionDefinitions.hpp`, `Source/CCZ4/CCZ4RHS.impl.hpp`, `Source/CCZ4/CCZ4Geometry.hpp`, `Source/CCZ4/Constraints.impl.hpp`, `Source/CCZ4/NewConstraints.impl.hpp`, `Source/ApparentHorizonFinder/AHFunctions.hpp`, `Source/ApparentHorizonFinder/ApparentHorizon.impl.hpp`, `Source/utils/TensorAlgebra.hpp`, and `Tests/`.
- Interpretation: CCZ4 formulas are dimension-parameterized in several places, including RHS, geometry, and constraints.
- Interpretation: the AH finder has explicit `GR_SPACEDIM != CH_SPACEDIM` support; `AHFunctions.hpp` includes `gww`/`Kww` handling, and `ApparentHorizon.impl.hpp` contains leftover-sphere terms.
- Interpretation: some AH geometry files are explicitly restricted to `GR_SPACEDIM == 3`, so cartoon/higher-dimensional support is not uniform across all AH geometry classes.
- Immediate validation target: `external/GRChombo/Tests/ApparentHorizonFinderTest2D/`, because it explicitly defines `GR_SPACEDIM 2` and exercises string-like AH behavior.
- Conclusion: encouraging infrastructure exists, but this does not prove that public GRChombo includes a complete Pau-style 5D black-string/SO(3) modified-cartoon evolution driver.

- `external/GRChombo/Source/utils/DimensionDefinitions.hpp`
  - Description: defines default `GR_SPACEDIM = 3` and `DEFAULT_TENSOR_DIM = CH_SPACEDIM` unless overridden.
  - Why we care: 4+1 work likely requires controlled `GR_SPACEDIM`/`CH_SPACEDIM` settings.
  - Inspect next: compile examples/tests with non-default dimensions before assuming support.

- `external/GRChombo/Source/ApparentHorizonFinder/ApparentHorizon.hpp`
  - Description: comments say AH support has been implemented for 3+1, 3+1 cartoon-reduced to 2+1, and 4+1 cartoon-reduced to 2+1.
  - Why we care: strong signal that horizon-side 4+1 cartoon support exists.
  - Inspect next: verify in a build/test and map required variables.

- `external/GRChombo/Source/ApparentHorizonFinder/AHFunctions.hpp`
  - Description: expansion function has extra-dimensional terms under `GR_SPACEDIM != CH_SPACEDIM`, including `gww` and `Kww` output headers.
  - Why we care: directly relevant to black-string horizon geometry diagnostics.
  - Inspect next: confirm variable names and indices for `hww`/`Aww` in the target compile mode, starting from `external/GRChombo/Tests/ApparentHorizonFinderTest2D/`.

- `external/GRChombo/Source/utils/Coordinates.hpp`
  - Description: comments mention 2D cartoon coordinate handling.
  - Why we care: possible coordinate support for reduced-dimensional grids.
  - Inspect next: whether it matches SO(3) modified cartoon for a 5D black string.

- First-pass negative search
  - Description: grep did not find ready-made `black string`, `Gregory`, `Laflamme`, or `modified cartoon` examples.
  - Why we care: project-specific physics pieces are likely new work.
  - Inspect next: deeper review of dimension-reduced CCZ4 terms before implementation.
