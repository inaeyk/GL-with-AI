# Physics Notes

Each physics-containing stage should have a polished physics note in this
directory.

- The `.tex` file is the editable source of truth for polished equations and
  explanatory discussion.
- The compiled `.pdf` is a human-readable review artifact.
- Markdown implementation notes remain operational design notes for code and
  workflow details.
- Generated PDFs should be small enough to track unless the project later
  decides otherwise.
- Build-only or infrastructure stages do not require full physics PDFs unless
  they contain nontrivial physics reasoning.

Build the notes with:

```bash
make -C docs/physics_notes all
```

If `make` is not available, use the shell helper:

```bash
bash scripts/build_physics_notes.sh all
```

Auxiliary LaTeX files are written under `.build/`, which is ignored.
