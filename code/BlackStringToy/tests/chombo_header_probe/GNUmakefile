# Minimal dependency/header probe. This is not a production application.

CHOMBO_HOME ?= $(abspath ../../../../external/Chombo/lib)
GRCHOMBO_SOURCE ?= $(abspath ../../../../external/GRChombo/Source)
BLACKSTRING_SOURCE ?= $(abspath ../..)

ebase := BlackStringChomboHeaderProbe
LibNames := BaseTools BoxTools

src_dirs := . \
            $(BLACKSTRING_SOURCE) \
            $(GRCHOMBO_SOURCE)/utils \
            $(GRCHOMBO_SOURCE)/simd \
            $(GRCHOMBO_SOURCE)/BoxUtils

include $(CHOMBO_HOME)/mk/Make.test
