# Real Chombo/GRChombo one-point storage fixture. This is not a BoxLoop.

CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../..)

ebase := BlackStringCellFArrayBoxStorageTest
LibNames := BoxTools

src_dirs :=

include ../StrictTargetBuildFlags.mk
include $(CHOMBO_HOME)/mk/Make.test

override CXXFLAGS := $(BLACKSTRING_STRICT_FLAGS)
