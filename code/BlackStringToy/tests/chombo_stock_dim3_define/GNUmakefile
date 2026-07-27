CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)

ebase := StockDim3BoundaryDefineTest
LibNames := AMRTimeDependent AMRTools BoxTools
src_dirs := $(GRCHOMBO_SOURCE)/utils \
            $(GRCHOMBO_SOURCE)/simd \
            $(GRCHOMBO_SOURCE)/BoxUtils \
            $(GRCHOMBO_SOURCE)/GRChomboCore \
            $(GRCHOMBO_SOURCE)/AMRInterpolator

override XTRACPPFLAGS += \
    -isystem$(CHOMBO_HOME)/src/AMRTimeDependent \
    -isystem$(CHOMBO_HOME)/src/AMRTools \
    -isystem$(CHOMBO_HOME)/src/BoxTools \
    -isystem$(CHOMBO_HOME)/src/BaseTools \
    -isystem$(GRCHOMBO_SOURCE)/utils \
    -isystem$(GRCHOMBO_SOURCE)/simd \
    -isystem$(GRCHOMBO_SOURCE)/BoxUtils \
    -isystem$(GRCHOMBO_SOURCE)/GRChomboCore \
    -isystem$(GRCHOMBO_SOURCE)/AMRInterpolator \
    -DGR_SPACEDIM=3 -DDEFAULT_TENSOR_DIM=3

include $(CHOMBO_HOME)/mk/Make.test

override CXXFLAGS := \
    -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -fopenmp

# Keep the fixture strict while compiling the locked stock implementation with
# its ordinary upstream warning policy.
STOCK_LOCKED_OBJECTS := $(addprefix $(_object_prefix),$(_srcs:.cpp=.o))
$(STOCK_LOCKED_OBJECTS): override CXXFLAGS := \
    -std=c++17 -O2 -fopenmp
