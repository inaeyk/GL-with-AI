# Project Log

Chronological lab notebook.

Categories: 🔴 Physics, 🔵 Code, 🟡 Physics + Code

## 2026-05-12

Category: 🟡 Physics + Code

- Apple Silicon Mac entered the GRChombo Docker container, but the prebuilt `amd64` executable failed with `Illegal instruction`.
- GitHub Codespaces `x86_64` Docker run succeeded for the GRChombo `BinaryBH` very-cheap test.
- HDF5 output and `quick_plot` PNG plots were generated.
- `quick_plot` needed a `yt` API patch: `set_window_size` -> `set_figure_size`.
- Stable local development target: Windows PC + WSL2 + Docker.
- Repository scope: code, docs, scripts, logs, research plans, small plots, and metadata.

## 2026-05-13

Category: 🔵 Code

- GRChombo Docker image ran locally under Windows WSL2 Ubuntu.
- `BinaryBH` very-cheap example completed and produced HDF5 output in `runs/BinaryBH_verycheap/hdf5/`.

Category: 🟡 Physics + Code

- Windows WSL2 is validated as the stable local development and smoke-test platform for GRChombo BinaryBH work.
