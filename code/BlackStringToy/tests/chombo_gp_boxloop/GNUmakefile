# Real DIM2 Chombo/GRChombo GP BoxLoop fixture.

CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../..)

ebase := BlackStringGPBoxLoopInitialDataTest
LibNames := BoxTools

src_dirs :=

include ../StrictTargetBuildFlags.mk
include $(CHOMBO_HOME)/mk/Make.test

override CXXFLAGS := $(BLACKSTRING_STRICT_FLAGS)
