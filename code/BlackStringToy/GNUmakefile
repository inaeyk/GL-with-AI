# -*- Mode: Makefile -*-

### This makefile produces an executable for each name in the `ebase'
###  variable using the libraries named in the `LibNames' variable.

# Included makefiles need an absolute path to the Chombo installation
# CHOMBO_HOME := Please set the CHOMBO_HOME locally (e.g. export CHOMBO_HOME=... in bash)
#
CHOMBO_HOME ?= $(abspath ../../external/Chombo/lib)
GRCHOMBO_SOURCE = $(abspath ../../external/GRChombo/Source)

ebase := Main_BlackStringToy

LibNames := AMRTimeDependent AMRTools BoxTools

src_dirs := $(GRCHOMBO_SOURCE)/utils \
            $(GRCHOMBO_SOURCE)/simd  \
            $(GRCHOMBO_SOURCE)/CCZ4  \
            $(GRCHOMBO_SOURCE)/BoxUtils  \
            $(GRCHOMBO_SOURCE)/GRChomboCore  \
            $(GRCHOMBO_SOURCE)/AMRInterpolator  \
            $(GRCHOMBO_SOURCE)/TaggingCriteria

override CXXFLAGS := -std=c++17 -O2 -fopenmp
override XTRACPPFLAGS += \
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
    -DGR_SPACEDIM=4 -DDEFAULT_TENSOR_DIM=4

include $(CHOMBO_HOME)/mk/Make.test
