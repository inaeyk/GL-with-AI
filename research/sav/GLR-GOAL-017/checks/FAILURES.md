# Failed checks

The cap-norm comparison below is between two numerical readouts of the
same physical equations. It is retained to show a refinement failure
of the shifted-current formula on the pilot mesh. The formula is exact
in the continuum. On the original coarse grid the executable emitted:

```text
late independent readout discrepancy (shared fields and shear) ||p_cap - derivative||2=6.1394009934
```

The middle and fine meshes gave $0.799597888748$ and $0.102589998872$.
The targeted clustered grid still gave $0.0284098294837$, far above
its $0.00043006$ shear-slope change compared with the preceding mesh.
The signed cancellation in the separately integrated stationary time
derivative is a limiting numerical effect. The outgoing-ray derivative
alternative avoids that particular cancellation, but lacks a
whole-volume physical input-error budget.

The first numerical graph assembly attempted to multiply the graph
slope column against a volume array before extracting the graph
diagonal. It stopped with the following error:

```text
ValueError: operands could not be broadcast together with shapes (161,1) (161,341,2)
```

Taking both graph traces first and then forming their tilted
combination corrected the shape error. The fixed physical tangential
graph constraint defect was at most $1.776\times10^{-15}$ on the tested
grids. It is a floating-point diagnostic.

The pinned GL/mean IVP routine emitted the following warning for every
radial run during the adaptive far numerical integration:

```text
RuntimeWarning: invalid value encountered in scalar divide
  return np.abs(h) * err5_norm_2 / np.sqrt(denom * len(scale))
```

This warning and the nonzero Riccati matching error stay in the
numerical uncertainty ledger. The numerical radial endpoint proxy
does not provide a verified physical coefficient tail bound.

The first bilinear readout check required the numerical weak shear to
vanish bitwise throughout the zero collar. It failed that assertion.
The maximal magnitude was $3.211331\times10^{-157}$, with all four
current arrays zero at those nodes. The bilinear candidate now
truncates the numerical shear there before defining its own candidate
$H+Vw$ model. This is an explicit change of the candidate only.

The all-cell coarse scalar reconstruction using a lift of the
unsubtracted baseline march gives the following output:

```text
directed baseline compact cap error <= [0.234610530172124564895092406549908 +/- 3.51e-34]
```

Subtracting the exact outgoing bump before the march, so that the
transported unknown vanishes to first order on the graph, reduces
the directed compact bound to
$0.005692654756765$. With the independently estimated no-transfer
exterior error, the coarse bound for the late norm still has zero
as its only nonnegative lower endpoint. The independent baseline
certificate in the spectral track closes that gate.
