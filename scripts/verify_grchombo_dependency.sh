#!/usr/bin/env bash

set -euo pipefail

verification_script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
verification_repo_root="$(cd "${verification_script_dir}/.." && pwd)"
verification_manifest="${verification_repo_root}/run_manifests/grchombo_dependency_lock.yaml"
verification_checkout="${1:-${verification_repo_root}/external/GRChombo}"

fail_verification()
{
    echo "GRChombo dependency verification failed: $1" >&2
    exit 1
}

[[ -f "${verification_manifest}" ]] ||
    fail_verification "missing lock manifest ${verification_manifest}"
[[ -d "${verification_checkout}/.git" ]] ||
    fail_verification "missing Git checkout ${verification_checkout}"

locked_remote="$(
    sed -n 's/^remote_url: //p' "${verification_manifest}"
)"
locked_commit="$(
    sed -n 's/^commit: //p' "${verification_manifest}"
)"

[[ -n "${locked_remote}" ]] ||
    fail_verification "manifest has no top-level remote_url"
[[ -n "${locked_commit}" ]] ||
    fail_verification "manifest has no top-level commit"

actual_remote="$(
    git -C "${verification_checkout}" remote get-url origin
)" || fail_verification "checkout has no readable origin remote"
[[ "${actual_remote}" == "${locked_remote}" ]] ||
    fail_verification \
        "origin is ${actual_remote}; expected ${locked_remote}"

actual_commit="$(
    git -C "${verification_checkout}" rev-parse --verify HEAD
)" || fail_verification "cannot resolve checkout HEAD"
[[ "${actual_commit}" == "${locked_commit}" ]] ||
    fail_verification \
        "HEAD is ${actual_commit}; expected ${locked_commit}"

if git -C "${verification_checkout}" symbolic-ref --quiet HEAD >/dev/null
then
    fail_verification "checkout is attached to a branch; detached HEAD required"
fi

checkout_status="$(
    git -C "${verification_checkout}" status --porcelain --untracked-files=all
)"
[[ -z "${checkout_status}" ]] ||
    fail_verification "checkout is dirty; commit, remove, or preserve local work elsewhere"

echo "GRChombo source lock verified"
echo "  remote: ${actual_remote}"
echo "  commit: ${actual_commit}"
echo "  state: detached and clean"
echo "Build reproducibility remains unresolved: Chombo, PETSc, and container digests are not locked."
