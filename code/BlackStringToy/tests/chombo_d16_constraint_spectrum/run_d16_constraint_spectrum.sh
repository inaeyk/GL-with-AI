#!/usr/bin/env bash
set -euo pipefail

fixture_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
cd "$fixture_dir"

tool_dir=
if ! command -v gfortran >/dev/null 2>&1; then
    versioned_fortran=$(command -v gfortran-15)
    tool_dir=$(mktemp -d /tmp/blackstring-d16-tools.XXXXXX)
    trap 'rm -rf "$tool_dir"' EXIT
    ln -s "$versioned_fortran" "$tool_dir/gfortran"
    PATH="$tool_dir:$PATH"
    export PATH
fi

make -j2 BlackStringD16ConstraintSpectrumTest \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=TRUE USE_LAPACK=TRUE
executable=$(find . -maxdepth 1 -type f \
    -name 'BlackStringD16ConstraintSpectrumTest*.ex' -print -quit)
if [[ -z "$executable" ]]; then
    echo "D16 executable was not built" >&2
    exit 1
fi

OMP_NUM_THREADS=1 "$executable" params_d16.txt
