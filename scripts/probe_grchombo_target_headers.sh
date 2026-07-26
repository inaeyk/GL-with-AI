#!/usr/bin/env bash

set -euo pipefail

probe_script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
probe_repo_root="$(cd "${probe_script_dir}/.." && pwd)"
probe_chombo_root="${probe_repo_root}/external/Chombo"
probe_chombo_root_from_argument="false"
probe_candidate_revision=""
probe_make_overrides=()
probe_resolution_errors=()

probe_reject_temporary_path()
{
    local label="$1"
    local resolved_path="$2"
    case "${resolved_path}" in
    /tmp/*|/var/tmp/*)
        probe_resolution_errors+=(
            "${label} resolves through a temporary path: ${resolved_path}"
        )
        ;;
    esac
}

probe_resolve_executable()
{
    local label="$1"
    local requested_command="$2"
    local command_variable="$3"
    local real_variable="$4"
    local command_path=""
    local real_path=""

    if [[ "${requested_command}" == */* ]]; then
        command_path="${requested_command}"
    else
        command_path="$(command -v "${requested_command}" 2>/dev/null || true)"
    fi

    if [[ -z "${command_path}" || ! -x "${command_path}" ]]; then
        echo "  ${label}: unresolved (${requested_command})"
        probe_resolution_errors+=(
            "${label} is not an executable command: ${requested_command}"
        )
        return
    fi

    real_path="$(readlink -f "${command_path}")"
    if [[ -z "${real_path}" || ! -x "${real_path}" ]]; then
        echo "  ${label}: unresolved real path (${command_path})"
        probe_resolution_errors+=(
            "${label} has no executable real path: ${command_path}"
        )
        return
    fi

    printf -v "${command_variable}" '%s' "${command_path}"
    printf -v "${real_variable}" '%s' "${real_path}"
    echo "  ${label}: command=${command_path} real=${real_path}"
    probe_reject_temporary_path "${label}" "${real_path}"
}

while (($# > 0)); do
    case "$1" in
    --chombo-root)
        (($# >= 2)) || {
            echo "--chombo-root requires a path" >&2
            exit 64
        }
        probe_chombo_root="$2"
        probe_chombo_root_from_argument="true"
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

if [[ -n "${CHOMBO_HOME:-}" ]]; then
    [[ -d "${CHOMBO_HOME}" ]] || {
        echo "CHOMBO_HOME is not a directory: ${CHOMBO_HOME}" >&2
        exit 64
    }
    probe_chombo_home="$(readlink -f "${CHOMBO_HOME}")"
    if [[ "${probe_chombo_root_from_argument}" == "true" ]]; then
        probe_expected_chombo_home="$(
            readlink -f "${probe_chombo_root}/lib"
        )"
        [[ "${probe_chombo_home}" == "${probe_expected_chombo_home}" ]] || {
            echo "CHOMBO_HOME disagrees with --chombo-root: ${probe_chombo_home} != ${probe_expected_chombo_home}" >&2
            exit 64
        }
    else
        probe_chombo_root="$(dirname "${probe_chombo_home}")"
    fi
else
    probe_chombo_home="$(readlink -f "${probe_chombo_root}/lib")"
fi

if [[ ! -e "${probe_chombo_root}" ]]; then
    "${probe_script_dir}/verify_grchombo_dependency.sh" --metadata-only \
        --chombo-root "${probe_chombo_root}"
    echo "TARGET_HEADER_PROBE=BLOCKED"
    echo "Reason: no Chombo checkout is available at ${probe_chombo_root}."
    exit 2
fi

probe_chombo_root="$(readlink -f "${probe_chombo_root}")"
probe_reject_temporary_path "Chombo checkout" "${probe_chombo_root}"
probe_reject_temporary_path "CHOMBO_HOME" "${probe_chombo_home}"

echo "Target-header probe environment"
echo "  Chombo checkout: ${probe_chombo_root}"
echo "  CHOMBO_HOME: ${probe_chombo_home}"

probe_cxx_command=""
probe_cxx_real=""
probe_fc_command=""
probe_fc_real=""
probe_cshell_command=""
probe_cshell_real=""
probe_resolve_executable \
    "C++ compiler" "${CXX:-g++}" probe_cxx_command probe_cxx_real
probe_resolve_executable \
    "Fortran compiler" "${CHOMBO_FC:-gfortran}" \
    probe_fc_command probe_fc_real
probe_resolve_executable \
    "C shell" "${CHOMBO_CSHELL:-/bin/csh}" \
    probe_cshell_command probe_cshell_real

probe_reverse="${probe_chombo_home}/mk/reverse"
if [[ ! -x "${probe_reverse}" ]]; then
    echo "  Chombo reverse helper: unresolved (${probe_reverse})"
    probe_resolution_errors+=(
        "Chombo reverse helper is missing or not executable: ${probe_reverse}"
    )
else
    probe_reverse="$(readlink -f "${probe_reverse}")"
    echo "  Chombo reverse helper: ${probe_reverse}"
    probe_reject_temporary_path "Chombo reverse helper" "${probe_reverse}"
    IFS= read -r probe_reverse_shebang < "${probe_reverse}"
    if [[ "${probe_reverse_shebang}" == "#!/bin/csh -f" ]] &&
        [[ ! -x /bin/csh ]]; then
        probe_resolution_errors+=(
            "Chombo reverse helper requires missing interpreter /bin/csh"
        )
    fi
fi

probe_required_headers=(
    "${probe_chombo_home}/src/BaseTools/parstream.H"
    "${probe_chombo_home}/src/BoxTools/FArrayBox.H"
    "${probe_repo_root}/external/GRChombo/Source/BoxUtils/Cell.hpp"
    "${probe_repo_root}/code/BlackStringToy/BlackStringReducedVars.hpp"
)
for probe_required_header in "${probe_required_headers[@]}"; do
    if [[ ! -f "${probe_required_header}" ]]; then
        probe_resolution_errors+=(
            "required target header is missing: ${probe_required_header}"
        )
    else
        echo "  Target header: $(readlink -f "${probe_required_header}")"
    fi
done

if [[ -n "${probe_cxx_command}" && -n "${probe_fc_command}" ]]; then
    probe_config="2d_ch.Linux.64.$(basename "${probe_cxx_command}").$(basename "${probe_fc_real}").OPT.OPENMPCC"
    probe_required_libraries=(
        "libbasetools${probe_config}.a"
        "libboxtools${probe_config}.a"
        "libamrtools${probe_config}.a"
        "libamrtimedependent${probe_config}.a"
    )
    for probe_required_library in "${probe_required_libraries[@]}"; do
        probe_required_library_path="${probe_chombo_home}/${probe_required_library}"
        if [[ ! -f "${probe_required_library_path}" ]]; then
            probe_resolution_errors+=(
                "required qualified DIM=2 library is missing: ${probe_required_library_path}"
            )
        else
            echo "  DIM2 library: $(readlink -f "${probe_required_library_path}")"
        fi
    done
fi

if ((${#probe_resolution_errors[@]} > 0)); then
    for probe_resolution_error in "${probe_resolution_errors[@]}"; do
        echo "Target-header probe environment error: ${probe_resolution_error}" >&2
    done
    exit 64
fi

probe_make_overrides+=(
    "CXX=${probe_cxx_command}"
    "FC=${probe_fc_real}"
    "CSHELLCMD=${probe_cshell_command} -f -c"
)

probe_verifier_arguments=(
    --require-build
    --chombo-root "${probe_chombo_root}"
)
if [[ -n "${probe_candidate_revision}" ]]; then
    probe_verifier_arguments[0]="--require-probe"
    probe_verifier_arguments+=(
        --candidate-chombo-revision "${probe_candidate_revision}"
    )
fi
CXX="${probe_cxx_command}" CHOMBO_FC="${probe_fc_real}" \
    "${probe_script_dir}/verify_grchombo_dependency.sh" \
    "${probe_verifier_arguments[@]}"

probe_directory="${probe_repo_root}/code/BlackStringToy/tests/chombo_header_probe"

make -C "${probe_directory}" clean \
    CHOMBO_HOME="${probe_chombo_home}" \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=FALSE \
    "${probe_make_overrides[@]}" >/dev/null
make -C "${probe_directory}" -j1 all \
    CHOMBO_HOME="${probe_chombo_home}" \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=FALSE \
    "${probe_make_overrides[@]}"
make -C "${probe_directory}" -j1 run-only \
    CHOMBO_HOME="${probe_chombo_home}" \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=FALSE \
    "${probe_make_overrides[@]}"

echo "TARGET_HEADER_PROBE=PASS"
echo "Macros: CH_SPACEDIM=2 GR_SPACEDIM=4 DEFAULT_TENSOR_DIM=4"
