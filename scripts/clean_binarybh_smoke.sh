#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"
run_dir="${repo_root}/runs/BinaryBH_verycheap"

if [[ ! -e "${run_dir}" ]]; then
  echo "Nothing to clean: runs/BinaryBH_verycheap does not exist."
  exit 0
fi

printf 'Delete ignored smoke-test output directory %s? [y/N] ' "${run_dir#${repo_root}/}"
if ! read -r answer; then
  answer=""
fi

case "${answer}" in
  y|Y|yes|YES)
    rm -rf "${run_dir}"
    echo "Deleted runs/BinaryBH_verycheap."
    ;;
  *)
    echo "Clean canceled."
    ;;
esac
