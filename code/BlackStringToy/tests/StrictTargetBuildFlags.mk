# Strict diagnostics for project-owned target fixtures.
#
# Chombo's Make.test adds its library headers with -I. Repeating those exact
# paths with -isystem makes GCC classify only the locked dependency headers as
# system headers; the fixture directory and BlackString sources remain ordinary
# -I paths and therefore remain fully subject to -Werror.

BLACKSTRING_STRICT_FLAGS := \
    -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror

BLACKSTRING_PROJECT_INCLUDES := \
    -I$(BLACKSTRING_SOURCE)

BLACKSTRING_EXTERNAL_INCLUDES := \
    -isystem$(CHOMBO_HOME)/src/BoxTools \
    -isystem$(CHOMBO_HOME)/src/BaseTools \
    -isystem$(GRCHOMBO_SOURCE)/utils \
    -isystem$(GRCHOMBO_SOURCE)/simd \
    -isystem$(GRCHOMBO_SOURCE)/BoxUtils \
    -isystem$(GRCHOMBO_SOURCE)/CCZ4

override XTRACPPFLAGS += \
    $(BLACKSTRING_PROJECT_INCLUDES) \
    $(BLACKSTRING_EXTERNAL_INCLUDES) \
    -DGR_SPACEDIM=4 -DDEFAULT_TENSOR_DIM=4
