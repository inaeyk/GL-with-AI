# Run Manifests

Structured records for validation and production runs live here. Do not
commit raw run output.

`grchombo_dependency_lock.yaml` separates two facts:

- the former CI checkout has only inferred Chombo provenance because its
  exact SHA was not exposed by official public logs or artifacts;
- official Chombo commit
  `8684f2e000106f1abadb72642e1d15351867f98f` is this project's qualified
  core dependency for locked GRChombo
  `37e659523830418b210acea1661dac0e00bb1b75`.

Verification levels:

```bash
# Check both source locks when present and disclose unresolved optional fields.
scripts/verify_grchombo_dependency.sh --metadata-only

# Require the pinned clean Chombo checkout and real header infrastructure.
scripts/verify_grchombo_dependency.sh --require-probe

# Additionally require the four qualified serial DIM=2 Chombo libraries.
scripts/verify_grchombo_dependency.sh --require-build

# Separate later AHFinder/PETSc discovery gate.
scripts/verify_grchombo_dependency.sh --require-probe --with-petsc
```

The real target executable probe is:

```bash
scripts/probe_grchombo_target_headers.sh
```

The verifier and probe never download, install, check out, or patch a
dependency. Container and PETSc/AHFinder provenance remain unresolved and are
reported separately from the qualified core source/build dependency.
