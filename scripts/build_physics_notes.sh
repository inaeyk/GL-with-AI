#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"
note_dir="${repo_root}/docs/physics_notes"
build_dir="${note_dir}/.build"
tex_stage3a="stage3A_black_string_initial_data.tex"
pdf_stage3a="stage3A_black_string_initial_data.pdf"

usage() {
  echo "Usage: $0 [stage3A|all|clean]" >&2
}

build_stage3a() {
  mkdir -p "${build_dir}"
  cd "${note_dir}"

  if command -v latexmk >/dev/null 2>&1; then
    echo "Building ${pdf_stage3a} with latexmk"
    latexmk -pdf -interaction=nonstopmode -halt-on-error \
      -outdir="${build_dir}" "${tex_stage3a}"
  elif command -v pdflatex >/dev/null 2>&1; then
    echo "Building ${pdf_stage3a} with pdflatex"
    pdflatex -interaction=nonstopmode -halt-on-error \
      -output-directory="${build_dir}" "${tex_stage3a}"
    pdflatex -interaction=nonstopmode -halt-on-error \
      -output-directory="${build_dir}" "${tex_stage3a}"
  else
    echo "Error: no LaTeX compiler found. Install latexmk or pdflatex." >&2
    exit 1
  fi

  cp "${build_dir}/${pdf_stage3a}" "${note_dir}/${pdf_stage3a}"
}

target="${1:-all}"

case "${target}" in
  stage3A)
    build_stage3a
    ;;
  all)
    build_stage3a
    ;;
  clean)
    rm -rf "${build_dir}"
    ;;
  -h|--help|help)
    usage
    ;;
  *)
    usage
    exit 2
    ;;
esac
