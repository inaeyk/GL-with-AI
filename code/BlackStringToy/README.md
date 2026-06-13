# BlackStringToy

Stage 2A editable GRChombo scaffold.

Origin: derived from public GRChombo `Examples/BinaryBH`.

This directory is a project-controlled copy of the minimal files needed to
build a distinct `BlackStringToy` example inside a scratch GRChombo tree. The
current scaffold deliberately inherits the public 3+1D `BinaryBH` initial data
and standard CCZ4 evolution behavior for build/run validation only.

Warning: this is not physical black-string evolution. It does not implement 5D
black-string initial data, compact `z`, GL perturbations, modified-cartoon
source terms, `hww/Aww`, horizon diagnostics, radiation extraction, or a
physical `GR_SPACEDIM=4`, `CH_SPACEDIM=2` evolution.

Build through:

```bash
bash scripts/stage2_build_blackstringtoy_scratch.sh
```

The script creates and uses an ignored scratch tree under
`runs/stage2_blackstringtoy/`.
