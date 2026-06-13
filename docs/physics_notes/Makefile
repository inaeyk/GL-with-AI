TEX_STAGE3A := stage3A_black_string_initial_data.tex
PDF_STAGE3A := stage3A_black_string_initial_data.pdf
BUILDDIR := .build

.PHONY: all stage3A clean

all: stage3A

stage3A: $(PDF_STAGE3A)

$(PDF_STAGE3A): $(TEX_STAGE3A)
	@mkdir -p $(BUILDDIR)
	@set -e; \
	if command -v latexmk >/dev/null 2>&1; then \
	  echo "Building $@ with latexmk"; \
	  latexmk -pdf -interaction=nonstopmode -halt-on-error -outdir=$(BUILDDIR) $(TEX_STAGE3A); \
	elif command -v pdflatex >/dev/null 2>&1; then \
	  echo "Building $@ with pdflatex"; \
	  pdflatex -interaction=nonstopmode -halt-on-error -output-directory=$(BUILDDIR) $(TEX_STAGE3A); \
	  pdflatex -interaction=nonstopmode -halt-on-error -output-directory=$(BUILDDIR) $(TEX_STAGE3A); \
	else \
	  echo "Error: no LaTeX compiler found. Install latexmk or pdflatex." >&2; \
	  exit 1; \
	fi
	@cp $(BUILDDIR)/$(PDF_STAGE3A) $(PDF_STAGE3A)

clean:
	rm -rf $(BUILDDIR)
