CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../..)

ebase := M2CAcceptanceRunner
LibNames := AMRTimeDependent AMRTools BoxTools
src_dirs := $(GRCHOMBO_SOURCE)/utils \
            $(GRCHOMBO_SOURCE)/simd \
            $(GRCHOMBO_SOURCE)/CCZ4 \
            $(GRCHOMBO_SOURCE)/BoxUtils \
            $(GRCHOMBO_SOURCE)/GRChomboCore \
            $(GRCHOMBO_SOURCE)/AMRInterpolator \
            $(GRCHOMBO_SOURCE)/TaggingCriteria

override CXXFLAGS := -std=c++17 -O2 -fopenmp
override XTRACPPFLAGS += \
    -include$(BLACKSTRING_SOURCE)/BlackStringChomboParameters.hpp \
    -I$(BLACKSTRING_SOURCE) \
    -isystem$(CHOMBO_HOME)/src/AMRTimeDependent \
    -isystem$(CHOMBO_HOME)/src/AMRTools \
    -isystem$(CHOMBO_HOME)/src/BoxTools \
    -isystem$(CHOMBO_HOME)/src/BaseTools \
    -isystem$(GRCHOMBO_SOURCE)/utils \
    -isystem$(GRCHOMBO_SOURCE)/simd \
    -isystem$(GRCHOMBO_SOURCE)/CCZ4 \
    -isystem$(GRCHOMBO_SOURCE)/BoxUtils \
    -isystem$(GRCHOMBO_SOURCE)/GRChomboCore \
    -isystem$(GRCHOMBO_SOURCE)/AMRInterpolator \
    -isystem$(GRCHOMBO_SOURCE)/TaggingCriteria \
    -DGR_SPACEDIM=4 -DDEFAULT_TENSOR_DIM=4 \
    -DBLACKSTRING_E2_LEVEL_DIAGNOSTICS

include $(CHOMBO_HOME)/mk/Make.test

BLACKSTRING_BOUNDARY_SCOPE := \
    $(BLACKSTRING_SOURCE)/BlackStringBoundaryDimensionScope.hpp

$(_object_prefix)BoundaryConditions.o \
$(_depends_prefix)BoundaryConditions.d: \
    XTRACPPFLAGS += -include$(BLACKSTRING_BOUNDARY_SCOPE)

$(_object_prefix)BoundaryConditions.o \
$(_depends_prefix)BoundaryConditions.d: $(BLACKSTRING_BOUNDARY_SCOPE)
