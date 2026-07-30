CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../..)

ebase := BlackStringD16ConstraintSpectrumTest
LibNames := AMRTimeDependent AMRTools BoxTools
src_dirs := $(GRCHOMBO_SOURCE)/utils \
            $(GRCHOMBO_SOURCE)/simd \
            $(GRCHOMBO_SOURCE)/BoxUtils \
            $(GRCHOMBO_SOURCE)/GRChomboCore \
            $(GRCHOMBO_SOURCE)/AMRInterpolator

include ../StrictTargetBuildFlags.mk

override XTRACPPFLAGS += \
    -I../chombo_perturbative_radial_boundary \
    -include$(BLACKSTRING_SOURCE)/BlackStringChomboParameters.hpp \
    -DBLACKSTRING_BOUNDARY_DIMENSION_AUDIT

include $(CHOMBO_HOME)/mk/Make.test

override CXXFLAGS := $(BLACKSTRING_STRICT_FLAGS) -Wno-template-body -fopenmp

BLACKSTRING_LOCKED_OBJECTS := \
    $(addprefix $(_object_prefix),$(_srcs:.cpp=.o))
$(BLACKSTRING_LOCKED_OBJECTS): override CXXFLAGS := \
    -std=c++17 -O2 -Wno-template-body -fopenmp

BLACKSTRING_BOUNDARY_SCOPE := \
    $(BLACKSTRING_SOURCE)/BlackStringBoundaryDimensionScope.hpp

$(_object_prefix)BoundaryConditions.o \
$(_depends_prefix)BoundaryConditions.d: \
    XTRACPPFLAGS += -include$(BLACKSTRING_BOUNDARY_SCOPE)

$(_object_prefix)BoundaryConditions.o \
$(_depends_prefix)BoundaryConditions.d: $(BLACKSTRING_BOUNDARY_SCOPE)
