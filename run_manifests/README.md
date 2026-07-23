# Run Manifests

Structured records for smoke tests, validation runs, and production runs.

Do not commit raw run outputs here.

`grchombo_dependency_lock.yaml` is the source-inspection lock for the
production-adaptation preflight. It deliberately records unresolved
Chombo/PETSc/container provenance rather than claiming a reproducible build.
