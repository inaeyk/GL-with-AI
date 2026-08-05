CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../..)

ebase := BlackStringM2BCycle1Test
LibNames := BoxTools
src_dirs :=

include ../StrictTargetBuildFlags.mk

override XTRACPPFLAGS += \
    -include$(BLACKSTRING_SOURCE)/BlackStringChomboParameters.hpp

include $(CHOMBO_HOME)/mk/Make.test

override CXXFLAGS := $(BLACKSTRING_STRICT_FLAGS) -fopenmp
