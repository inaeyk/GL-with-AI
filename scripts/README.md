# Scripts

Reproducible wrappers for smoke tests, output collection, plotting, and cleanup.

Keep raw outputs in ignored scratch folders.

## Usage

Run the smoke-test workflow in this order:

1. `./scripts/run_binarybh_smoke.sh`
2. `./scripts/collect_binarybh_outputs.sh`
3. `./scripts/plot_binarybh_outputs.sh`
4. Optional cleanup: `./scripts/clean_binarybh_smoke.sh`

`plot_binarybh_outputs.sh` is currently a placeholder and does not yet generate plots.

- `run_binarybh_smoke.sh`: runs the GRChombo BinaryBH very-cheap test through Docker and writes raw HDF5 output under `runs/`.
- `collect_binarybh_outputs.sh`: records small metadata for the latest BinaryBH smoke test without copying raw HDF5 into `results/`.
- `plot_binarybh_outputs.sh`: creates the BinaryBH plots directory; plotting support will be added later.
- `clean_binarybh_smoke.sh`: asks for confirmation, then removes ignored local BinaryBH smoke-test outputs from `runs/`.
