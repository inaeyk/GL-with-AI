# Workflow Overview

This file is the source of truth for the overall coding workflow.

## Development and Testing

Use local WSL/Docker or Codespaces/cloud VM for development and smoke testing.

- Windows WSL2 + Docker: preferred stable local target.
- Codespaces/cloud VM: useful for quick `x86_64` smoke tests.
- Apple Silicon Mac: useful for editing, Git, Codex, plotting, and post-processing.

## Repository Policy

The GitHub repository stores:

- Code
- Docs
- Scripts
- Logs
- Small plots
- Metadata
- Run manifests

Ignored raw-output locations:

- `runs/`
- `data/`
- `tmp/`

Do not commit raw HDF5 files, checkpoints, restarts, or production outputs.

## Documentation Size

- `README.md`: fewer than 100 lines.
- `TODO.md`: fewer than 80 lines.
- `PROJECT_LOG.md`: grows over time, but entries stay concise.
- `CODEX_LOG.md`: grows over time, task-based.
- Setup docs: one page each unless needed.
- Implementation docs: detailed only when we actually implement something.

## Production

Large production simulations will eventually move to HPC. Record production metadata in `run_manifests/` and keep raw outputs outside normal Git tracking.
