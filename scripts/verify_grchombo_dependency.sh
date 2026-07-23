#!/usr/bin/env bash

set -euo pipefail

verification_script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
verification_repo_root="$(cd "${verification_script_dir}/.." && pwd)"
verification_manifest="${verification_repo_root}/run_manifests/grchombo_dependency_lock.yaml"

verification_mode="metadata"
verification_with_petsc="false"
verification_chombo_override=""
verification_candidate_chombo_revision=""

usage()
{
    cat <<'USAGE'
Usage: scripts/verify_grchombo_dependency.sh [options]

Options:
  --metadata-only         Verify the locked GRChombo source and report build gaps
                          without requiring absent dependencies (default).
  --require-probe         Require a pinned, clean Chombo checkout with the
                          headers and make infrastructure needed by the target
                          header compile probe.
  --require-build         Require the project-qualified core Chombo build:
                          pinned clean checkout and the four serial DIM=2
                          libraries. Container and PETSc/AHFinder provenance
                          are reported separately.
  --with-petsc            Also require a discoverable PETSc installation.
  --chombo-root PATH      Override the manifest Chombo checkout path.
  --candidate-chombo-revision COMMIT
                          Permit --require-probe to validate this exact
                          candidate before it is accepted into the manifest.
                          This is recovery-only and never overrides a lock.
  -h, --help              Show this help.

This verifier is read-only. It never downloads, installs, checks out, or builds
dependencies.
USAGE
}

fail_verification()
{
    echo "GRChombo dependency verification failed: $1" >&2
    exit 1
}

while (($# > 0)); do
    case "$1" in
    --metadata-only)
        verification_mode="metadata"
        shift
        ;;
    --require-probe)
        verification_mode="probe"
        shift
        ;;
    --require-build)
        verification_mode="build"
        shift
        ;;
    --with-petsc)
        verification_with_petsc="true"
        shift
        ;;
    --chombo-root)
        (($# >= 2)) ||
            fail_verification "--chombo-root requires a path"
        verification_chombo_override="$2"
        shift 2
        ;;
    --candidate-chombo-revision)
        (($# >= 2)) ||
            fail_verification \
                "--candidate-chombo-revision requires a commit"
        verification_candidate_chombo_revision="$2"
        [[ "${verification_candidate_chombo_revision}" =~ ^[0-9a-f]{40}$ ]] ||
            fail_verification \
                "candidate Chombo revision must be a full 40-hex commit"
        shift 2
        ;;
    -h|--help)
        usage
        exit 0
        ;;
    *)
        fail_verification "unknown option $1"
        ;;
    esac
done

if [[ -n "${verification_candidate_chombo_revision}" ]] &&
    [[ "${verification_mode}" != "probe" ]]; then
    fail_verification \
        "--candidate-chombo-revision is valid only with --require-probe"
fi

[[ -f "${verification_manifest}" ]] ||
    fail_verification "missing lock manifest ${verification_manifest}"

manifest_value()
{
    local section="$1"
    local key="$2"
    awk -v wanted_section="${section}" -v wanted_key="${key}" '
        $0 == wanted_section ":" {
            in_section = 1
            next
        }
        /^[^[:space:]]/ {
            in_section = 0
        }
        in_section && $0 ~ "^  " wanted_key ":" {
            sub("^  " wanted_key ":[[:space:]]*", "")
            print
            exit
        }
    ' "${verification_manifest}"
}

manifest_top_value()
{
    local key="$1"
    sed -n "s/^${key}: //p" "${verification_manifest}"
}

is_unresolved()
{
    [[ -z "$1" || "$1" == "unresolved" ]]
}

resolve_repo_path()
{
    local configured_path="$1"
    if [[ "${configured_path}" = /* ]]; then
        printf '%s\n' "${configured_path}"
    else
        printf '%s\n' "${verification_repo_root}/${configured_path}"
    fi
}

locked_grchombo_remote="$(manifest_value grchombo remote_url)"
locked_grchombo_commit="$(manifest_value grchombo commit)"
configured_grchombo_checkout="$(manifest_value grchombo checkout_path)"
expected_grchombo_state="$(manifest_value grchombo expected_checkout_state)"

[[ -n "${locked_grchombo_remote}" ]] ||
    fail_verification "manifest has no grchombo.remote_url"
[[ -n "${locked_grchombo_commit}" ]] ||
    fail_verification "manifest has no grchombo.commit"
[[ "${expected_grchombo_state}" == "detached_clean" ]] ||
    fail_verification "manifest must require a detached_clean GRChombo checkout"

verification_grchombo_checkout="$(
    resolve_repo_path "${configured_grchombo_checkout}"
)"
[[ -d "${verification_grchombo_checkout}/.git" ]] ||
    fail_verification \
        "missing Git checkout ${verification_grchombo_checkout}"

actual_grchombo_remote="$(
    git -C "${verification_grchombo_checkout}" remote get-url origin
)" || fail_verification "GRChombo checkout has no readable origin remote"
[[ "${actual_grchombo_remote}" == "${locked_grchombo_remote}" ]] ||
    fail_verification \
        "GRChombo origin is ${actual_grchombo_remote}; expected ${locked_grchombo_remote}"

actual_grchombo_commit="$(
    git -C "${verification_grchombo_checkout}" rev-parse --verify HEAD
)" || fail_verification "cannot resolve GRChombo checkout HEAD"
[[ "${actual_grchombo_commit}" == "${locked_grchombo_commit}" ]] ||
    fail_verification \
        "GRChombo HEAD is ${actual_grchombo_commit}; expected ${locked_grchombo_commit}"

if git -C "${verification_grchombo_checkout}" symbolic-ref --quiet HEAD \
    >/dev/null; then
    fail_verification "GRChombo checkout is attached to a branch; detached HEAD required"
fi

grchombo_status="$(
    git -C "${verification_grchombo_checkout}" \
        status --porcelain --untracked-files=all
)"
[[ -z "${grchombo_status}" ]] ||
    fail_verification \
        "GRChombo checkout is dirty; preserve local work elsewhere"

required_ch_spacedim="$(manifest_value grchombo required_CH_SPACEDIM)"
required_gr_spacedim="$(manifest_value grchombo required_GR_SPACEDIM)"
required_tensor_dim="$(manifest_value grchombo required_DEFAULT_TENSOR_DIM)"
[[ "${required_ch_spacedim}" == "2" ]] ||
    fail_verification "manifest CH_SPACEDIM is not 2"
[[ "${required_gr_spacedim}" == "4" ]] ||
    fail_verification "manifest GR_SPACEDIM is not 4"
[[ "${required_tensor_dim}" == "4" ]] ||
    fail_verification "manifest DEFAULT_TENSOR_DIM is not 4"

echo "GRChombo source lock verified"
echo "  remote: ${actual_grchombo_remote}"
echo "  commit: ${actual_grchombo_commit}"
echo "  state: detached and clean"
echo "  target macros: CH_SPACEDIM=2 GR_SPACEDIM=4 DEFAULT_TENSOR_DIM=4"

historical_provenance_status="$(
    manifest_value historical_provenance status
)"
project_dependency_status="$(
    manifest_value project_dependency_lock status
)"
project_dependency_commit="$(
    manifest_value project_dependency_lock chombo_commit
)"
[[ "${historical_provenance_status}" == "exact" ||
   "${historical_provenance_status}" == "inferred" ||
   "${historical_provenance_status}" == "unavailable" ]] ||
    fail_verification \
        "historical_provenance.status must be exact, inferred, or unavailable"
[[ "${project_dependency_status}" == "qualified" ]] ||
    fail_verification \
        "project_dependency_lock.status is not qualified"
[[ "${project_dependency_commit}" =~ ^[0-9a-f]{40}$ ]] ||
    fail_verification \
        "project_dependency_lock.chombo_commit is not a full commit"

configured_chombo_checkout="$(manifest_value chombo checkout_path)"
locked_chombo_remote="$(manifest_value chombo remote_url)"
locked_chombo_revision="$(manifest_value chombo revision)"
required_chombo_header="$(manifest_value chombo required_header)"

if [[ -n "${verification_chombo_override}" ]]; then
    verification_chombo_checkout="${verification_chombo_override}"
else
    verification_chombo_checkout="$(
        resolve_repo_path "${configured_chombo_checkout}"
    )"
fi
verification_chombo_home="${verification_chombo_checkout}/lib"

chombo_available="false"
chombo_probe_ready="false"
if [[ -e "${verification_chombo_checkout}" ]]; then
    [[ -d "${verification_chombo_checkout}/.git" ]] ||
        fail_verification \
            "present Chombo path is not a Git checkout: ${verification_chombo_checkout}"

    actual_chombo_remote="$(
        git -C "${verification_chombo_checkout}" remote get-url origin
    )" || fail_verification "Chombo checkout has no readable origin remote"
    [[ "${actual_chombo_remote}" == "${locked_chombo_remote}" ]] ||
        fail_verification \
            "Chombo origin is ${actual_chombo_remote}; expected ${locked_chombo_remote}"

    actual_chombo_revision="$(
        git -C "${verification_chombo_checkout}" rev-parse --verify HEAD
    )" || fail_verification "cannot resolve Chombo checkout HEAD"
    if [[ -n "${verification_candidate_chombo_revision}" ]] &&
        [[ "${actual_chombo_revision}" != \
           "${verification_candidate_chombo_revision}" ]]; then
        fail_verification \
            "Chombo HEAD is ${actual_chombo_revision}; requested candidate is ${verification_candidate_chombo_revision}"
    fi

    chombo_status="$(
        git -C "${verification_chombo_checkout}" \
            status --porcelain --untracked-files=all
    )"
    [[ -z "${chombo_status}" ]] ||
        fail_verification \
            "Chombo checkout is dirty; preserve local work elsewhere"
    if git -C "${verification_chombo_checkout}" symbolic-ref --quiet HEAD \
        >/dev/null; then
        fail_verification \
            "Chombo checkout is attached to a branch; detached HEAD required"
    fi

    [[ -f "${verification_chombo_home}/mk/Make.test" ]] ||
        fail_verification \
            "Chombo is missing ${verification_chombo_home}/mk/Make.test"

    chombo_header_path="$(
        find "${verification_chombo_home}" -type f \
            -name "${required_chombo_header}" -print -quit
    )"
    [[ -n "${chombo_header_path}" ]] ||
        fail_verification \
            "Chombo is missing required header ${required_chombo_header}"

    chombo_available="true"
    echo "Chombo checkout inspected"
    echo "  remote: ${actual_chombo_remote}"
    echo "  revision: ${actual_chombo_revision}"
    echo "  CHOMBO_HOME: ${verification_chombo_home}"
    echo "  ${required_chombo_header}: ${chombo_header_path}"

    if is_unresolved "${locked_chombo_revision}"; then
        if [[ -n "${verification_candidate_chombo_revision}" ]]; then
            [[ "${actual_chombo_revision}" == "${verification_candidate_chombo_revision}" ]] ||
                fail_verification \
                    "Chombo HEAD is ${actual_chombo_revision}; requested candidate is ${verification_candidate_chombo_revision}"
            chombo_probe_ready="true"
            echo "  revision lock: unresolved"
            echo "  candidate validation: exact requested commit; not authoritative"
        else
            echo "  revision lock: unresolved; candidate is not accepted as authoritative"
        fi
    elif [[ "${actual_chombo_revision}" != "${locked_chombo_revision}" ]]; then
        fail_verification \
            "Chombo HEAD is ${actual_chombo_revision}; expected ${locked_chombo_revision}"
    else
        [[ "${actual_chombo_revision}" == "${project_dependency_commit}" ]] ||
            fail_verification \
                "Chombo manifest revision disagrees with project dependency lock"
        chombo_probe_ready="true"
        echo "  revision lock: verified"
    fi
else
    echo "Chombo checkout unavailable"
    echo "  expected checkout: ${verification_chombo_checkout}"
    echo "  expected CHOMBO_HOME: ${verification_chombo_home}"
    echo "  expected remote: ${locked_chombo_remote}"
    echo "  revision lock: ${locked_chombo_revision}"
fi

compiler_command="${CXX:-g++}"
if command -v "${compiler_command}" >/dev/null 2>&1; then
    compiler_version="$("${compiler_command}" --version | sed -n '1p')"
    echo "Compiler available: ${compiler_version}"
else
    echo "Compiler unavailable: ${compiler_command}"
    if [[ "${verification_mode}" != "metadata" ]]; then
        fail_verification "target compiler ${compiler_command} is unavailable"
    fi
fi

petsc_available="false"
if command -v pkg-config >/dev/null 2>&1 &&
    pkg-config --exists petsc; then
    petsc_available="true"
    echo "PETSc discovered by pkg-config: $(pkg-config --modversion petsc)"
else
    echo "PETSc not discovered by pkg-config"
fi

if [[ "${verification_with_petsc}" == "true" ]] &&
    [[ "${petsc_available}" != "true" ]]; then
    fail_verification \
        "--with-petsc requested but pkg-config cannot resolve petsc"
fi

if [[ "${verification_mode}" == "probe" ]]; then
    [[ "${chombo_available}" == "true" ]] ||
        fail_verification "target header probe requires a local Chombo checkout"
    [[ "${chombo_probe_ready}" == "true" ]] ||
        fail_verification \
            "target header probe requires a pinned Chombo revision"
fi

core_reproducibility_status="$(
    manifest_top_value reproducibility_status
)"
[[ "${core_reproducibility_status}" == \
   "project_qualified_core_dependency" ]] ||
    fail_verification \
        "manifest does not declare a project-qualified core dependency"

if [[ "${verification_mode}" == "build" ]]; then
    [[ -z "${verification_candidate_chombo_revision}" ]] ||
        fail_verification \
            "a recovery candidate cannot satisfy the project dependency lock"
    [[ "${chombo_probe_ready}" == "true" ]] ||
        fail_verification \
            "core build verification requires the pinned Chombo revision"

    qualification_compiler_version="$(
        manifest_value chombo qualification_compiler_version
    )"
    [[ -n "${qualification_compiler_version}" ]] ||
        fail_verification \
            "manifest has no Chombo qualification compiler version"
    [[ "${compiler_version}" == *"${qualification_compiler_version}"* ]] ||
        fail_verification \
            "C++ compiler does not match qualified version ${qualification_compiler_version}"

    fortran_compiler_command="${CHOMBO_FC:-gfortran}"
    if [[ "${fortran_compiler_command}" = */* ]]; then
        [[ -x "${fortran_compiler_command}" ]] ||
            fail_verification \
                "qualified Fortran compiler is unavailable: ${fortran_compiler_command}"
    else
        command -v "${fortran_compiler_command}" >/dev/null 2>&1 ||
            fail_verification \
                "qualified Fortran compiler is unavailable: ${fortran_compiler_command}"
    fi
    fortran_compiler_version="$(
        "${fortran_compiler_command}" --version | sed -n '1p'
    )"
    [[ "${fortran_compiler_version}" == \
       *"${qualification_compiler_version}"* ]] ||
        fail_verification \
            "Fortran compiler does not match qualified version ${qualification_compiler_version}"

    required_chombo_libraries=(
        "libbasetools2d_ch*.a"
        "libboxtools2d_ch*.a"
        "libamrtools2d_ch*.a"
        "libamrtimedependent2d_ch*.a"
    )
    for required_library_pattern in "${required_chombo_libraries[@]}"; do
        library_matches=(
            "${verification_chombo_home}"/${required_library_pattern}
        )
        [[ -f "${library_matches[0]}" ]] ||
            fail_verification \
                "missing qualified DIM=2 library ${required_library_pattern}"
    done
fi

echo "HISTORICAL_PROVENANCE=${historical_provenance_status^^}"
if [[ "${verification_mode}" == "build" ]]; then
    echo "CORE_DEPENDENCY_BUILD=VERIFIED"
else
    echo "CORE_DEPENDENCY_BUILD=PROJECT_QUALIFIED"
fi
echo "CONTAINER_REPRODUCIBILITY=UNRESOLVED"
echo "Reason: the former image tag/digest and recipe digest remain unavailable."
if [[ "${petsc_available}" != "true" ]]; then
    echo "PETSC_AHFINDER_REPRODUCIBILITY=UNRESOLVED"
    echo "Reason: PETSc is unavailable and is not required by the core/initializer seam."
else
    echo "PETSC_DISCOVERY=AVAILABLE_UNPINNED"
fi
