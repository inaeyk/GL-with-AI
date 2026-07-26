# Roadmap Decision After Dependency Qualification

Date: 2026-07-24

The custom solver is retained as a validated reference oracle and
agent-capability benchmark. GRChombo is the production framework. Missing
production functionality will be adapted directly rather than independently
rebuilt. The unresolved custom outer spectral boundary is deferred and
nonblocking.

This decision supersedes the custom spectral gate as a prerequisite for the
active production path; it does not complete or erase that gate. Stage 4AO-D
and Checkpoint G remain incomplete. GRChombo RK, AMR, MPI/OpenMP,
checkpointing, reductions, interpolation, ghost exchange, and generic grid
infrastructure remain framework-owned.

Storage, GP `BoxLoop` initialization, the complete target-`d=4` RHS,
post-update cleanup, observational constraints, the fixed lapse source, and
periodic direction-1 ghost exchange are now assembled in the isolated E1
application. The exact next implementation substage is Stage 4AO-D-F: a
bounded unperturbed GP evolution plus radial-boundary qualification. It must
not yet claim AMR/MPI production, perturbation growth, horizons, or AHFinder.
