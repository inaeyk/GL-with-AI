#!/usr/bin/env bash
set -euo pipefail

fixture_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
cd "$fixture_dir"

tool_dir=
if ! command -v gfortran >/dev/null 2>&1; then
    versioned_fortran=$(command -v gfortran-15)
    tool_dir=$(mktemp -d /tmp/blackstring-d13-tools.XXXXXX)
    trap 'rm -rf "$tool_dir"' EXIT
    ln -s "$versioned_fortran" "$tool_dir/gfortran"
    PATH="$tool_dir:$PATH"
    export PATH
fi

make -j2 BlackStringD13TangentModeTest \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=TRUE
executable=$(find . -maxdepth 1 -type f \
    -name 'BlackStringD13TangentModeTest*.ex' -print -quit)
if [[ -z "$executable" ]]; then
    echo "D13 executable was not built" >&2
    exit 1
fi

OMP_NUM_THREADS=1 "$executable" params_d13.txt
