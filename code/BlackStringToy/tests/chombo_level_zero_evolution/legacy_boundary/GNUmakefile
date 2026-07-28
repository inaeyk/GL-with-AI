CHOMBO_HOME ?= $(abspath ../../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../../..)

ebase := BlackStringLevelZeroEvolutionLegacyBoundaryTest
LibNames := AMRTimeDependent AMRTools BoxTools
src_dirs := $(GRCHOMBO_SOURCE)/utils \
            $(GRCHOMBO_SOURCE)/simd \
            $(GRCHOMBO_SOURCE)/BoxUtils \
            $(GRCHOMBO_SOURCE)/GRChomboCore \
            $(GRCHOMBO_SOURCE)/AMRInterpolator

include ../../StrictTargetBuildFlags.mk

override XTRACPPFLAGS += \
    -include$(BLACKSTRING_SOURCE)/BlackStringChomboParameters.hpp \
    -DBLACKSTRING_E2_LEVEL_DIAGNOSTICS \
    -DBLACKSTRING_BOUNDARY_DIMENSION_AUDIT \
    -DBLACKSTRING_BOUNDARY_LEGACY_TENSOR_LOOP \
    -DBLACKSTRING_BOUNDARY_REJECT_NON_GRID_DIRECTION

include $(CHOMBO_HOME)/mk/Make.test

override CXXFLAGS := $(BLACKSTRING_STRICT_FLAGS) -fopenmp

BLACKSTRING_LOCKED_OBJECTS := \
    $(addprefix $(_object_prefix),$(_srcs:.cpp=.o))
$(BLACKSTRING_LOCKED_OBJECTS): override CXXFLAGS := \
    -std=c++17 -O2 -fopenmp

BLACKSTRING_BOUNDARY_SCOPE := \
    $(BLACKSTRING_SOURCE)/BlackStringBoundaryDimensionScope.hpp

$(_object_prefix)BoundaryConditions.o \
$(_depends_prefix)BoundaryConditions.d: \
    XTRACPPFLAGS += -include$(BLACKSTRING_BOUNDARY_SCOPE)

$(_object_prefix)BoundaryConditions.o \
$(_depends_prefix)BoundaryConditions.d: $(BLACKSTRING_BOUNDARY_SCOPE)

.PHONY: legacy-boundary-negative
legacy-boundary-negative: all
	@log=$$(mktemp /tmp/blackstring-e2-legacy.XXXXXX); \
	trap 'rm -f "$$log"' EXIT; \
	exe="$(firstword $(_app_configs))"; \
	test -n "$$exe"; \
	if OMP_NUM_THREADS=1 "$$exe" ../params_n32.txt exact \
	    >"$$log" 2>&1; then \
	    echo "E2 legacy boundary mutation unexpectedly passed" >&2; \
	    exit 1; \
	fi; \
	grep -q 'legacy boundary tensor loop attempted a non-grid direction' \
	    "$$log"; \
	echo "E2_LEGACY_BOUNDARY_MUTATION=REJECT"; \
	echo "E2_LEGACY_FAILURE_PATH=level-zero-setup-before-evolution"; \
	echo "BLACKSTRING_LEVEL_ZERO_LEGACY_BOUNDARY_NEGATIVE=PASS"
