#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"

image="${GRCHOMBO_DOCKER_IMAGE:-grchombo/grchombo}"
source_grchombo="${repo_root}/external/GRChombo"
source_example="${repo_root}/code/BlackStringToy"
scratch_root="${repo_root}/runs/stage2_blackstringtoy"
scratch_grchombo="${scratch_root}/GRChombo"
scratch_example="${scratch_grchombo}/Examples/BlackStringToy"
container_example="/settings/runs/stage2_blackstringtoy/GRChombo/Examples/BlackStringToy"
build_jobs="${BUILD_JOBS:-2}"
run_smoke="${RUN_SMOKE:-1}"
run_timeout_seconds="${RUN_TIMEOUT_SECONDS:-180}"
host_uid="$(id -u)"
host_gid="$(id -g)"

echo "Stage 2A BlackStringToy editable build scaffold"
echo "WARNING: this skeleton is not physical black-string evolution."
echo "It does not implement 5D initial data, GL perturbations, cartoon source terms, hww/Aww, or radiation extraction."

require_expected_scratch_root() {
  if [[ "${scratch_root}" != "${repo_root}/runs/stage2_blackstringtoy" ]]; then
    echo "Error: refusing to operate on unexpected scratch root: ${scratch_root}" >&2
    exit 1
  fi
}

fix_scratch_ownership() {
  if [[ ! -e "${scratch_root}" ]]; then
    return 0
  fi

  if ! find "${scratch_root}" -xdev -not -uid "${host_uid}" -print -quit 2>/dev/null | grep -q .; then
    return 0
  fi

  echo "Detected scratch files not owned by the current user."
  echo "Attempting ownership fix: chown -R ${host_uid}:${host_gid} ${scratch_root#${repo_root}/}"
  if command -v sudo >/dev/null 2>&1; then
    sudo chown -R "${host_uid}:${host_gid}" "${scratch_root}" || {
      echo "Error: sudo chown failed for ${scratch_root#${repo_root}/}." >&2
      echo "Run manually: sudo chown -R ${host_uid}:${host_gid} '${scratch_root}'" >&2
      exit 1
    }
  else
    echo "Error: scratch tree contains non-user-owned files and sudo is unavailable." >&2
    echo "Run manually: sudo chown -R ${host_uid}:${host_gid} '${scratch_root}'" >&2
    exit 1
  fi
}

clean_scratch_root() {
  require_expected_scratch_root
  fix_scratch_ownership
  rm -rf "${scratch_root}" || {
    echo "Error: could not remove ${scratch_root#${repo_root}/}." >&2
    echo "If Docker left root-owned files, run:" >&2
    echo "  sudo chown -R ${host_uid}:${host_gid} '${scratch_root}'" >&2
    echo "then rerun this script." >&2
    exit 1
  }
}

if ! command -v docker >/dev/null 2>&1; then
  echo "Error: docker is not available on PATH." >&2
  exit 1
fi

if ! docker info >/dev/null 2>&1; then
  echo "Error: docker is present but not usable from this shell." >&2
  echo "Run 'docker info' for details; Docker Desktop WSL integration may be disabled." >&2
  exit 1
fi

if [[ ! -d "${source_grchombo}" ]]; then
  echo "Error: missing external GRChombo source: ${source_grchombo}" >&2
  exit 1
fi

if [[ ! -d "${source_example}" ]]; then
  echo "Error: missing tracked BlackStringToy source: ${source_example}" >&2
  exit 1
fi

if ! docker image inspect "${image}" >/dev/null 2>&1; then
  echo "Error: Docker image ${image} is not available locally." >&2
  echo "Pull it separately, then rerun this script." >&2
  exit 1
fi

require_expected_scratch_root

echo "Creating scratch tree: ${scratch_grchombo#${repo_root}/}"
clean_scratch_root
mkdir -p "${scratch_root}"
cp -a "${source_grchombo}" "${scratch_grchombo}"

echo "Injecting tracked scaffold: ${source_example#${repo_root}/} -> ${scratch_example#${repo_root}/}"
rm -rf "${scratch_example}"
mkdir -p "$(dirname "${scratch_example}")"
cp -a "${source_example}" "${scratch_example}"

build_inner=$(cat <<EOF
set -euo pipefail
cd "${container_example}"
echo "Build command:"
echo "make -j${build_jobs} all DIM=3 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE"
make -j${build_jobs} all DIM=3 DEBUG=FALSE OPT=TRUE USE_PETSC=FALSE
exe="\$(find . -maxdepth 1 -type f -name 'Main_BlackStringToy*.ex' | sort | head -n 1)"
if [[ -z "\${exe}" ]]; then
  echo "Error: build completed but no Main_BlackStringToy*.ex executable was found." >&2
  exit 1
fi
echo "Final executable path: ${container_example}/\${exe#./}"
if [[ "${run_smoke}" == "1" ]]; then
  echo "Smoke run command:"
  echo "timeout ${run_timeout_seconds}s \${exe} params_stage2_smoke.txt"
  timeout "${run_timeout_seconds}s" "\${exe}" params_stage2_smoke.txt
  echo "Smoke output location: ${container_example}"
else
  echo "Smoke run skipped because RUN_SMOKE=${run_smoke}."
fi
EOF
)

echo "Docker command:"
printf 'docker run --rm -v %q:/settings %q bash -lc %q\n' "${repo_root}" "${image}" "${build_inner}"

docker run --rm \
  -v "${repo_root}:/settings" \
  "${image}" \
  bash -lc "${build_inner}"

fix_scratch_ownership

echo "Done. Generated files remain under ${scratch_root#${repo_root}/}."
