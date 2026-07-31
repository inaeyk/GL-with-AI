#!/usr/bin/env bash
set -euo pipefail

fixture_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${fixture_dir}/../../../.." && pwd)"
fortran_compiler="${CHOMBO_FC:-gfortran-15}"
serial_config=".M1C_SERIAL"
mpi_config=".M1C_MPI"
output_dir="${M1C_OUTPUT_DIR:-${repo_root}/.m1c-output}"

mkdir -p "${output_dir}"

make -C "${fixture_dir}" -j2 BlackStringM1CDistributedSeamTest \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=FALSE USE_HDF=FALSE \
    FC="${fortran_compiler}" CXX=g++ XTRACONFIG="${serial_config}"

serial_executable="$(
    find "${fixture_dir}" -maxdepth 1 -type f -perm -u+x \
        -name "BlackStringM1CDistributedSeamTest2d*${serial_config}.ex" \
        -print -quit
)"
if [[ -z "${serial_executable}" ]]; then
    echo "M1C_RUNNER_FAIL serial executable not found" >&2
    exit 1
fi

"${serial_executable}" serial \
    >"${output_dir}/serial.log" 2>&1

make -C "${fixture_dir}" -j2 BlackStringM1CDistributedSeamTest \
    DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=FALSE \
    FC="${fortran_compiler}" CXX=g++ MPICXX=mpicxx \
    XTRACONFIG="${mpi_config}"

mpi_executable="$(
    find "${fixture_dir}" -maxdepth 1 -type f -perm -u+x \
        -name "BlackStringM1CDistributedSeamTest2d*${mpi_config}.ex" \
        -print -quit
)"
if [[ -z "${mpi_executable}" ]]; then
    echo "M1C_RUNNER_FAIL MPI executable not found" >&2
    exit 1
fi

mpirun -np 2 "${mpi_executable}" z \
    >"${output_dir}/mpi2_z.log" 2>&1
mpirun -np 2 "${mpi_executable}" x \
    >"${output_dir}/mpi2_x.log" 2>&1
mpirun -np 4 "${mpi_executable}" both \
    >"${output_dir}/mpi4_both.log" 2>&1
for log in serial mpi2_z mpi2_x mpi4_both; do
    grep -E "M1C_(SERIAL_REFERENCE_PASS|LAYOUT_PASS|MUTATION_RUNNER_PASS|FIXTURE_PASS)" \
        "${output_dir}/${log}.log"
done
echo "M1C_RUNNER_PASS completed_launches=4 mutation_runner=serial output_dir=${output_dir}"
