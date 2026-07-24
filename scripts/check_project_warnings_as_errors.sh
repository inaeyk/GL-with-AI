#!/usr/bin/env bash

set -euo pipefail

warning_script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
warning_repo_root="$(cd "${warning_script_dir}/.." && pwd)"
warning_source="${warning_repo_root}/code/BlackStringToy/tests/project_warning_negative/ProjectOwnedWarning.cpp"
warning_log="$(mktemp /tmp/blackstring-project-warning.XXXXXX)"
trap 'rm -f "${warning_log}"' EXIT

warning_cxx="${CXX:-g++}"
warning_flags=(
    -std=c++17
    -O2
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    "-I${warning_repo_root}/code/BlackStringToy"
    -fsyntax-only
)

if "${warning_cxx}" "${warning_flags[@]}" "${warning_source}" \
    2>"${warning_log}"; then
    echo "PROJECT_WARNING_NEGATIVE_COMPILE=FAIL" >&2
    echo "Reason: project-owned unused parameter compiled under -Werror." >&2
    exit 1
fi

if ! awk '/unused parameter/ { found = 1 } END { exit(found ? 0 : 1) }' \
    "${warning_log}"; then
    echo "PROJECT_WARNING_NEGATIVE_COMPILE=FAIL" >&2
    echo "Reason: failure was not the expected project-owned warning." >&2
    exit 1
fi

echo "PROJECT_WARNING_NEGATIVE_COMPILE=PASS"
echo "Compiler: ${warning_cxx}"
echo "Flags: -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror"
echo "Diagnostic: project-owned unused parameter remains fatal"
