#!/usr/bin/env bash

set -euo pipefail

probe_script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
probe_repo_root="$(cd "${probe_script_dir}/.." && pwd)"
probe_chombo_root="${probe_repo_root}/external/Chombo"
probe_candidate_revision=""
probe_make_overrides=()

if [[ -n "${CHOMBO_CSHELL:-}" ]]; then
    [[ -x "${CHOMBO_CSHELL}" ]] || {
        echo "CHOMBO_CSHELL is not executable: ${CHOMBO_CSHELL}" >&2
        exit 64
    }
    probe_make_overrides+=(
        "CSHELLCMD=${CHOMBO_CSHELL} -f -c"
    )
fi
if [[ -n "${CHOMBO_FC:-}" ]]; then
    [[ -x "${CHOMBO_FC}" ]] || {
        echo "CHOMBO_FC is not executable: ${CHOMBO_FC}" >&2
        exit 64
    }
    probe_make_overrides+=(
        "FC=${CHOMBO_FC}"
    )
fi

while (($# > 0)); do
    case "$1" in
    --chombo-root)
        (($# >= 2)) || {
            echo "--chombo-root requires a path" >&2
            exit 64
        }
        probe_chombo_root="$2"
        shift 2
        ;;
    --candidate-revision)
        (($# >= 2)) || {
            echo "--candidate-revision requires a commit" >&2
            exit 64
        }
        probe_candidate_revision="$2"
        shift 2
        ;;
    *)
        echo "Usage: scripts/probe_grchombo_target_headers.sh [--chombo-root PATH] [--candidate-revision COMMIT]" >&2
        exit 64
        ;;
    esac
done

if [[ ! -e "${probe_chombo_root}" ]]; then
    "${probe_script_dir}/verify_grchombo_dependency.sh" --metadata-only \
        --chombo-root "${probe_chombo_root}"
    echo "TARGET_HEADER_PROBE=BLOCKED"
    echo "Reason: no Chombo checkout is available at ${probe_chombo_root}."
    exit 2
fi

probe_verifier_arguments=(
    --require-probe
    --chombo-root "${probe_chombo_root}"
)
if [[ -n "${probe_candidate_revision}" ]]; then
    probe_verifier_arguments+=(
        --candidate-chombo-revision "${probe_candidate_revision}"
    )
fi
"${probe_script_dir}/verify_grchombo_dependency.sh" \
    "${probe_verifier_arguments[@]}"

probe_directory="${probe_repo_root}/code/BlackStringToy/tests/chombo_header_probe"
probe_chombo_home="${probe_chombo_root}/lib"
probe_warning_flags="-Wall -Wextra -Wpedantic -Werror -Wno-error=unused-parameter -Wno-error=deprecated-copy"
probe_include_flags="-I${probe_repo_root}/code/BlackStringToy -I${probe_repo_root}/external/GRChombo/Source/utils -I${probe_repo_root}/external/GRChombo/Source/simd -I${probe_repo_root}/external/GRChombo/Source/BoxUtils -I${probe_repo_root}/external/GRChombo/Source/CCZ4"

make -C "${probe_directory}" clean \
    CHOMBO_HOME="${probe_chombo_home}" DIM=2 USE_HDF=FALSE \
    "${probe_make_overrides[@]}" >/dev/null
make -C "${probe_directory}" -j1 all \
    CHOMBO_HOME="${probe_chombo_home}" \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=FALSE \
    "${probe_make_overrides[@]}" \
    XTRACPPFLAGS="-std=c++17 -O2 ${probe_warning_flags} ${probe_include_flags} -DGR_SPACEDIM=4 -DDEFAULT_TENSOR_DIM=4"
make -C "${probe_directory}" -j1 run-only \
    CHOMBO_HOME="${probe_chombo_home}" \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=FALSE \
    "${probe_make_overrides[@]}" \
    XTRACPPFLAGS="-std=c++17 -O2 ${probe_warning_flags} ${probe_include_flags} -DGR_SPACEDIM=4 -DDEFAULT_TENSOR_DIM=4"

echo "TARGET_HEADER_PROBE=PASS"
echo "Macros: CH_SPACEDIM=2 GR_SPACEDIM=4 DEFAULT_TENSOR_DIM=4"
