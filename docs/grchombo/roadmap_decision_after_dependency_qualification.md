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

The exact next implementation substage is the thin `Cell`/`FArrayBox` storage
seam around the validated 18-slot reduced Vars and GP point initializer. It
must not duplicate physics or begin `BoxLoop`, RHS, cleanup, gauge-source,
periodicity, evolution, diagnostics, or AHFinder work.
