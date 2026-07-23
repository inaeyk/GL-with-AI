# Run Manifests

Structured records for smoke tests, validation runs, and production runs.

Do not commit raw run outputs here.

`grchombo_dependency_lock.yaml` is the dependency contract for the
production-adaptation preflight. Its GRChombo record is fully pinned. The
locked source verifies that GRChombo CI uses the `GRChombo/Chombo` fork with
`CHOMBO_HOME=<checkout>/lib`, but the CI checkout has no `ref`, so the Chombo
revision is deliberately `unresolved`. PETSc and container provenance are
recorded separately.

Verification levels are:

```bash
# Strict GRChombo source check; reports absent/unresolved build dependencies.
scripts/verify_grchombo_dependency.sh --metadata-only

# Available only after Chombo is pinned and present.
scripts/verify_grchombo_dependency.sh --require-probe

# Also require PETSc for an AHFinder candidate.
scripts/verify_grchombo_dependency.sh --require-probe --with-petsc

# Must not pass while any required build provenance remains unresolved.
scripts/verify_grchombo_dependency.sh --require-build
```

For recovery testing before a Chombo candidate is accepted into the manifest,
use its full commit explicitly:

```bash
scripts/probe_grchombo_target_headers.sh \
  --chombo-root external/Chombo \
  --candidate-revision <FULL_40_HEX_CHOMBO_COMMIT>
```

Candidate mode proves only that the exact requested checkout passes the
minimal target-header probe. It never satisfies full-build verification and
does not silently pin the candidate.

The verifier and probe are read-only with respect to dependency checkouts.
They never download, install, check out, or patch dependencies. A missing
Chombo checkout makes the probe print `TARGET_HEADER_PROBE=BLOCKED` and exit
with status 2; no header stubs are permitted.
