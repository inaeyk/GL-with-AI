# Black-string Cell/FArrayBox storage seam fixture

This fixture compiles against the locked real Chombo and GRChombo headers and
libraries at:

```text
CH_SPACEDIM=2
GR_SPACEDIM=4
DEFAULT_TENSOR_DIM=4
```

It constructs real 18-component `FArrayBox` objects, uses GRChombo
`BoxPointers` and `Cell<double>` at one explicit `IntVect`, and exercises
`BlackStringCellStorage::load` and `BlackStringCellStorage::store`.

The fixture does not call `BoxLoop`, traverse a grid, initialize production
data, own ghosts, or evaluate any physics.
