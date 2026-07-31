CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../..)

ebase := BlackStringM1CDistributedSeamTest
LibNames := BoxTools
src_dirs :=

include ../StrictTargetBuildFlags.mk
include $(CHOMBO_HOME)/mk/Make.test

override CXXFLAGS := $(BLACKSTRING_STRICT_FLAGS) -fopenmp
