#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"
hdf5_dir="${repo_root}/runs/BinaryBH_verycheap/hdf5"
results_dir="${repo_root}/results/BinaryBH_verycheap"
metadata_file="${results_dir}/metadata.md"
image="grchombo/grchombo"
example="GRChombo Examples/BinaryBH"
params="params_very_cheap.txt"

if [[ ! -d "${hdf5_dir}" ]]; then
  echo "Error: missing HDF5 directory: ${hdf5_dir}" >&2
  echo "Run scripts/run_binarybh_smoke.sh first." >&2
  exit 1
fi

mapfile -t hdf5_files < <(find "${hdf5_dir}" -type f -name "*.hdf5" | sort)

if (( ${#hdf5_files[@]} == 0 )); then
  echo "Error: no .hdf5 files found in ${hdf5_dir}" >&2
  exit 1
fi

platform="$(uname -s)"
if grep -qi microsoft /proc/version 2>/dev/null; then
  platform="Windows WSL2 Ubuntu"
fi

mkdir -p "${results_dir}"

cat > "${metadata_file}" <<EOF
# BinaryBH Very-Cheap Smoke Test

## Run Metadata
- Platform: ${platform}
- Architecture: $(uname -m)
- Date: $(date -Iseconds)
- Docker image: ${image}
- Example: ${example}
- Parameter file: ${params}
- HDF5 file count: ${#hdf5_files[@]}
- Raw output location: \`runs/BinaryBH_verycheap/hdf5/\`

## Notes
- Raw HDF5 output is ignored by Git.
- Raw HDF5 files are not copied into \`results/\`.
EOF

echo "Wrote ${metadata_file#${repo_root}/}"
