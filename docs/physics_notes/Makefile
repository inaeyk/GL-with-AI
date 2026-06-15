TEX_FILES := $(wildcard *.tex)
PDF_FILES := $(TEX_FILES:.tex=.pdf)
BUILDDIR := .build

.PHONY: all stage3A clean

all: $(PDF_FILES)

stage3A: stage3A_black_string_initial_data.pdf

%.pdf: %.tex
	@mkdir -p $(BUILDDIR)
	@set -e; \
	if command -v latexmk >/dev/null 2>&1; then \
	  echo "Building $@ with latexmk"; \
	  latexmk -pdf -interaction=nonstopmode -halt-on-error -outdir=$(BUILDDIR) $<; \
	elif command -v pdflatex >/dev/null 2>&1; then \
	  echo "Building $@ with pdflatex"; \
	  pdflatex -interaction=nonstopmode -halt-on-error -output-directory=$(BUILDDIR) $<; \
	  pdflatex -interaction=nonstopmode -halt-on-error -output-directory=$(BUILDDIR) $<; \
	else \
	  echo "Error: no LaTeX compiler found. Install latexmk or pdflatex." >&2; \
	  exit 1; \
	fi
	@cp $(BUILDDIR)/$@ $@

clean:
	rm -rf $(BUILDDIR)
