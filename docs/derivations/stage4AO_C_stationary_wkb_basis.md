# Stage 4AO-C Stationary Full-Vector WKB Audit

Status: symbolic/design audit only.  This document is derived from the
committed, validated thirteen-row frozen-gauge interior assembler at commit
`6628f49`.  It does not define an outer endpoint operator.  In particular,
it does not promote the former scalar-profile characteristic diagnostic to a
WKB boundary condition.

## Conventions and radial matrix

The locked state is

```text
U=(chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,Gx,Gz)^T.
```

Let `sigma=+1` in `P_+` and `sigma=-1` in `P_-`.  Work in the dimensionless
coordinate `X=x/r0`, with `q=k r0` and `t=X^(-1/2)`.  Curvature and connection
variables are measured in the corresponding powers of `r0`; hence the table
below is the `r0=1` table used by the fixtures and `D_x=r0^(-1)D_X` restores
dimensions.  Direct application of every row to independent value, first-
derivative, and second-derivative jets gives

```text
0 = M0(t,q,sigma) U + M1(t,q,sigma) D_X U + M2 D_X^2 U.
```

The following sparse tables are explicit: omitted entries vanish.  An entry
`rc:a` means matrix row `r`, column `c`, value `a`; `q`-linear entries acquire
the displayed `sigma`.

```text
M2:
r6  c1:3 c2:-1/2 c4:-1/2 c5:-1
r7  c1:3/4 c2:-3/8 c4:1/8 c5:1/4
r8  c3:-1/2
r9  c1:-1/4 c2:1/8 c4:-3/8 c5:1/4
r10 c1:-1/4 c2:1/8 c4:1/8 c5:-1/4
r11 c1:3/2 c2:-1/4 c4:-1/4 c5:-1/2

M1[t^0]:
r6 c12:1
r7 c12:3/4
r8 c1:-sigma*q c13:1/2
r9 c12:-1/4
r10 c12:-1/4
r11 c12:1/2
r12 c2:1/20 c4:-1/20 c5:-1/10 c6:-3/2 c11:2
r13 c3:1/10

M1[t^1]:
r1 c1:1; r2 c2:1; r3 c3:1; r4 c4:1; r5 c5:1
r6 c6:1; r7 c7:1; r8 c8:1; r9 c9:1; r10 c10:1
r11 c11:1; r12 c12:1; r13 c13:1

M1[t^2]:
r6 c1:6 c2:-1 c4:-1 c5:-2
r7 c1:-1/2 c2:-3/4 c4:1/4 c5:1/2
r8 c3:-1
r9 c1:-1/2 c2:1/4 c4:-3/4 c5:1/2
r10 c1:1/2 c2:1/4 c4:1/4 c5:-1/2
r11 c1:3 c2:-1/2 c4:-1/2 c5:-1

M1[t^3]:
r12 c1:7/2 c2:-1/2 c5:-2
r13 c3:-1

M0[t^0]:
r1 c6:1/2
r2 c7:-2; r3 c8:-2; r4 c9:-2; r5 c10:-2
r6 c1:-3q^2 c2:q^2/2 c4:q^2/2 c5:q^2 c11:-2/5
   c13:sigma*q
r7 c1:q^2/4 c2:3q^2/8 c4:-q^2/8 c5:-q^2/4
   c13:-sigma*q/4
r8 c3:q^2/2 c12:-sigma*q/2
r9 c1:-3q^2/4 c2:-q^2/8 c4:3q^2/8 c5:-q^2/4
   c13:3sigma*q/4
r10 c1:q^2/4 c2:-q^2/8 c4:-q^2/8 c5:q^2/4
    c13:-sigma*q/4
r11 c1:-3q^2/2 c2:q^2/4 c4:q^2/4 c5:q^2/2 c11:-1/4
    c13:sigma*q/2
r12 c3:sigma*q/10 c12:-1/10
r13 c2:sigma*q/20 c4:-sigma*q/20 c5:sigma*q/10
    c6:3sigma*q/2 c11:-2sigma*q c13:-1/10

M0[t^2]:
r6 c12:2
r7 c12:-1/2; r9 c12:-1/2; r10 c12:1/2; r11 c12:1
r12 c2:1/5 c5:-1/5
r13 c3:1/5

M0[t^3]:
r2 c2:-7/4; r3 c3:-5/4; r4 c4:-3/4; r5 c5:5/4
r6 c6:3 c11:-3
r7 c6:-7/8 c7:13/4 c11:7/4
r8 c8:11/4
r9 c6:-3/8 c9:9/4 c11:3/4
r10 c6:5/8 c10:1/4 c11:-5/4
r11 c6:9/8 c7:7/8 c9:3/8 c10:-5/4 c11:-3/2
r12 c12:1/2
r13 c1:-3sigma*q/2 c2:-sigma*q/2 c5:2sigma*q

M0[t^4]:
r7 c2:2 c5:-2; r8 c3:1; r10 c2:-1 c5:1

M0[t^5]:
r12 c2:1 c5:-1; r13 c3:7/4

M0[t^6]:
r7 c2:49/32; r8 c3:21/32; r9 c4:9/32
r10 c5:25/32
r11 c2:49/64 c4:9/64 c5:25/32
```

Thus all `1/X`, `1/X^2`, encoded-Z, representative hidden-sphere, and Gamma
terms are present.  The `t^5` and `t^6` entries are retained because the
complete rows contain products of the spherical factors with
`lambda=O(t^3)`; dropping them while claiming the complete asymptotic system
would be inconsistent.

An independent source-versus-matrix comparison applied both representations
to the same nonzero jets at `q={0.31,0.73,1.11}` and `X={7,13,29}` in both
parity sectors.  The maximum componentwise mismatch was
`2.44e-16`.  This validates the sparse matrices through their documented
asymptotic order; it does not validate a determinant factorization or an outer
boundary condition.

For a local exponential `U=e^(sX)u`, define

```text
L(t,s)=M0(t)+s M1(t)+s^2 M2.
```

The previously claimed universal limit

```text
det L(t,s)/t^5 -> C(q) s^5 (s^2-q^2)^4
```

is false.  An exact-rational calculation at the single value `q=2/3` gives

```text
lim[t->0] det L(t,s)/t^5
  proportional to s^5 (s^2-q^2)^3 (s^2-q^2+1/200).
```

This is a counterexample to the fourfold light factorization.  It is not a
general-`q` factorization, which has not been derived.

## Leading right and left spaces

The displayed singular leading matrix is generically of nullity three at
`s=epsilon*q`, `epsilon=+/-1`.  Three verified right-kernel vectors are:

```text
r1=(0,-2 epsilon sigma,1,0,0,0,0,0,0,0,0,0,0)^T,
r2=(0,1,0,1,0,0,0,0,0,0,0,0,0)^T,
r3=(sigma/q,0,0,0,sigma/q,0,0,0,0,0,0,
    -epsilon sigma,1)^T.
```

A compatible (not biorthonormal) left kernel is

```text
l1=(0,0,0,0,0,0,-1,-2 epsilon sigma,1,0,0,0,0)^T,
l2=(0,0,0,0,0,0, 1, epsilon sigma,0,1,0,0,0)^T,
l3=(0,0,0,0,0,-5/8,1/2,epsilon sigma/2,0,0,1,0,0)^T.
```

Substitution verifies `L r_i=0` and `l_i^T L=0`.  These vectors are full
state vectors; they are not transformed-amplitude selectors.  They establish
only the right and left kernels of this singular leading symbol.

The kernel dimension cannot be interpreted directly as the geometric
multiplicity of a regular matrix polynomial.  In particular, it does not
prove a fourth generalized light vector, a polynomial prefactor
`X^m exp(+/-qX)`, singular powers of `t`, or a required state rescaling.
Higher-order solvability or a separately justified regularized first-order
radial system is needed to classify the actual branches.  Coincident leading
vectors with distinct subleading series have not been excluded.

At `s=0` the same singular leading matrix has a three-dimensional ordinary
kernel.  One convenient right-kernel basis is

```text
z1=(1,1,0,3,1,0,0,0,0,0,0,0,0)^T,
z2=(0,0,sigma/q,0,0,0,0,0,0,0,0,1,0)^T,
z3=(0,0,0,-2sigma/q,0,0,0,0,0,0,0,0,1)^T.
```

These vectors do not prove that two additional zero-sector branches are
generalized chains.  The old five labels `J,F,G,C_h,C_A` remain useful
coordinates on a diagnostic characteristic layout, but they are not five
proved eigenvectors or five exact unmixed annihilator rows.  Higher-order
solvability is needed to determine the number, form, and mixing of any
additional zero-exponential branches.

## Attempted WKB recursion and limitation

For the requested ansatz let

```text
F=exp(epsilon*q*X-gamma/t) X^(-p),
a(t)=u0+t u1+t^2 u2+t^3 u3+O(t^4),
U=F a.
```

Its derivative map is explicit:

```text
D_X U/F = [Q-(t^3/2)D_t]a,
D_X^2 U/F = [Q-(t^3/2)D_t]^2 a,
Q=epsilon*q-(gamma/2)t-p t^2.
```

Inserting the sparse matrices above and collecting powers of `t` starts with

```text
L(0,epsilon*q) u0=0.
```

Therefore every regular series of the requested form considered at
`s=epsilon*q` has `u0 in span(r1,r2,r3)`.  This leading-order statement does
not determine the number of full asymptotic solutions: distinct series may
share a leading vector and separate at subleading orders.  The necessary
higher-order solvability calculation has not been completed, so `gamma`,
`p`, and `u1` cannot yet be assigned to four classified branches.

Consequently this audit does not report invented values for
`u0,u1,u2,u3`.  It also withdraws the former scalar-only assertions

```text
gamma_T=gamma_TF=0, gamma_V=0.1, gamma_S=0.5,
p_b^-=1+q/2+gamma_b^2/(8q).
```

Those values solve a diagnostic scalar equation after discarding the mixing
shown in the matrices; they have not survived the full-vector solvability
test.  No componentwise logarithmic derivative is presently available.  The
only valid formula is the conditional identity

```text
D_X log F = epsilon*q-(gamma/2)t-p t^2,
```

augmented by the vector-series derivative above after a valid generalized
basis has been derived.

## Consequences for a future outer boundary

There is not yet enough asymptotic data to form

```text
A_-(q,Xout)=[Y_T^- Y_TF^- Y_V^- Y_S^-].
```

In particular, rank four, a nine-dimensional physical left annihilator, and
four retained-PDE dual covectors have not been derived.  No endpoint rows or
nullspace routines should be implemented from this audit.

Once four genuine columns exist, the linear-algebra statements are standard:
right multiplication `A_- -> A_- C` by nonsingular `C` preserves its column
space and left annihilator; nonzero column rescaling is the diagonal case;
and a four-row PDE dual `P` is admissible only if its row space is disjoint
from the nine-row annihilator and the stacked matrix has rank thirteen.
Those statements do not prove that the required columns currently exist.

The validated interior remains quadratic in `k`, but no polynomial, rational,
or genuinely nonlinear classification has been established for a future
boundary-bearing problem because no accepted outer operator exists.  No
polynomial or nonlinear eigensolver work is authorized.

## Required continuation and validation

If this deferred research problem is reopened, it must first:

1. carry the higher-order solvability calculation far enough to classify all
   branches, or derive an independently justified regularized first-order
   radial system;
2. determine whether distinct branches share leading kernel vectors;
3. derive the Frobenius solvability conditions and map every result back to
   the locked thirteen variables;
4. substitute each resulting series into all thirteen explicit rows through
   the first uncancelled order.

Independent future checks must separate continuum-series residual, radial
stencil error, and annihilator error; evaluate several `q` and `Xout`; test
decaying/growing separation, both parities, hidden multiplicity, branch
relations, left/right pairings, and the final `4+9=13`
endpoint rank.  Until those checks exist, every outer-boundary and downstream
Stage 4AO-C gate remains false.

Current Stage 4AO-C status is therefore:

- the full thirteen-variable interior operator is complete and validated;
- the inner pure-outflow boundary and endpoint stencils are validated;
- mirrored outer stencils and outer row-layout code are diagnostic scaffolding
  only;
- no accepted outer asymptotic condition, rank-four decaying basis, or
  rank-nine physical annihilator exists;
- no generalized-chain obstruction has been established;
- outer endpoint implementation/validation, aggregate boundary completion,
  the boundary-bearing complete operator, pencil representation,
  eigensolver/shift-invert, MOTS, threshold, production wiring, Stage 4AO-D,
  and Checkpoint G remain false;
- no polynomial or nonlinear eigensolver work is authorized.

## Project disposition

The custom stationary outer-boundary problem is frozen as deferred research.
The next project phase is to:

1. inventory overlap between the custom solver and GRChombo;
2. compare overlapping formulas and numerical outputs;
3. use those results to assess AI-agent capability;
4. adapt missing production functionality directly from GRChombo.
