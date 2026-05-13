#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"
run_dir="${repo_root}/runs/BinaryBH_verycheap"
hdf5_dir="${run_dir}/hdf5"
image="grchombo/grchombo"
# Verified for the current grchombo/grchombo Docker smoke test; dynamic
# executable discovery is a future enhancement.
executable="./Main_BinaryBH3d_ch.Linux.64.mpicxx.gfortran.DEBUG.OPT.MPI.OPENMPCC.ex"

if ! command -v docker >/dev/null 2>&1; then
  echo "Error: docker is not available on PATH." >&2
  exit 1
fi

mkdir -p "${run_dir}"

if ! docker image inspect "${image}" >/dev/null 2>&1; then
  echo "Docker image ${image} not found locally; pulling it now."
  docker pull "${image}"
fi

docker run --rm \
  -v "${repo_root}:/settings" \
  "${image}" \
  bash -lc "set -euo pipefail
cd /GRChombo/Examples/BinaryBH
${executable} params_very_cheap.txt
mkdir -p /settings/runs/BinaryBH_verycheap
rm -rf /settings/runs/BinaryBH_verycheap/hdf5
cp -r hdf5 /settings/runs/BinaryBH_verycheap/"

if command -v sudo >/dev/null 2>&1; then
  sudo chown -R "$(whoami):$(whoami)" "${run_dir}" || \
    echo "Warning: could not change ownership of ${run_dir}; continuing." >&2
else
  echo "Warning: sudo is not available; skipping ownership fix for ${run_dir}." >&2
fi

if [[ ! -d "${hdf5_dir}" ]]; then
  echo "Error: expected output directory was not created: ${hdf5_dir}" >&2
  exit 1
fi

mapfile -t hdf5_files < <(find "${hdf5_dir}" -type f -name "*.hdf5" | sort)

if (( ${#hdf5_files[@]} == 0 )); then
  echo "Error: no .hdf5 files found in ${hdf5_dir}" >&2
  exit 1
fi

echo "First HDF5 files:"
for file in "${hdf5_files[@]:0:10}"; do
  printf '  %s\n' "${file#${repo_root}/}"
done

echo "Success: found ${#hdf5_files[@]} .hdf5 file(s) in runs/BinaryBH_verycheap/hdf5."
