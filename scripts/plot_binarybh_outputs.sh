#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"
plots_dir="${repo_root}/results/BinaryBH_verycheap/plots"

mkdir -p "${plots_dir}"

echo "Plotting will be added later. No Python packages were installed and quick_plot was not cloned."
echo "Plots directory is ready: ${plots_dir#${repo_root}/}"
