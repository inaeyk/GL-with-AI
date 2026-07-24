#!/usr/bin/env bash

set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$(mktemp -d /tmp/blackstring_target_cleanup_constraints_source.XXXXXX)"
trap 'rm -rf "$build_dir"' EXIT

production_header="$repo_root/code/BlackStringToy/BlackStringTargetCleanupConstraintsSource.hpp"
oracle_header="$repo_root/code/BlackStringToy/tests/Stage4AOCAnalyticFullOracle.hpp"

if awk '/Stage4AOCAnalyticFullOracle/ { found = 1 }
        END { exit(found ? 0 : 1) }' "$production_header"
then
    echo "TARGET_CLEANUP_CONSTRAINT_SOURCE_ARCHITECTURE=FAIL production-calls-oracle"
    exit 1
fi

if awk '/BlackStringTargetCleanupConstraintsSource/ { found = 1 }
        END { exit(found ? 0 : 1) }' "$oracle_header"
then
    echo "TARGET_CLEANUP_CONSTRAINT_SOURCE_ARCHITECTURE=FAIL oracle-calls-production"
    exit 1
fi

"${CXX:-g++}" \
    -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror \
    -DCH_SPACEDIM=2 -DGR_SPACEDIM=4 -DDEFAULT_TENSOR_DIM=4 \
    -I"$repo_root/code/BlackStringToy" \
    -I"$repo_root/code/BlackStringToy/tests" \
    -isystem "$repo_root/external/GRChombo/Source/CCZ4" \
    -isystem "$repo_root/external/GRChombo/Source/utils" \
    -isystem "$repo_root/external/GRChombo/Source/simd" \
    -isystem "$repo_root/external/GRChombo/Source/BoxUtils" \
    "$repo_root/code/BlackStringToy/tests/BlackStringTargetCleanupConstraintsSourceTest.cpp" \
    -o "$build_dir/BlackStringTargetCleanupConstraintsSourceTest"

"$build_dir/BlackStringTargetCleanupConstraintsSourceTest"
echo "TARGET_CLEANUP_CONSTRAINT_SOURCE_ARCHITECTURE=PASS"
