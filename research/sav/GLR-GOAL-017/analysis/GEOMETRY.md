# Directed GL and fixed-infinity mean on the late collar

The full causal graph reaches \(b(d)=1.0090210771\ldots\) with
\(u_\Sigma(r)=-r-2\log(r-1)\). GOAL-015's public coefficient
interface starts at \(25/16\), although its inner profile cells
start at \(9/8\). Neither bound alone covers this graph. Extend the
interface on \([1+1/112,25/16]\); directed substitution into
\(u_\Sigma\) places \(1+1/112\) below \(b(d)\).
The GOAL-015 caps on early strong error and packet interference apply
to their original intervals. [GOAL-015 source](https://github.com/inaeyk/GL-with-AI/pull/21)
· [Independent audit](https://github.com/inaeyk/GL-with-AI/pull/22).

Write \(H=e^{\Omega v}\widehat H(r)\) in the regular ingoing EF
basis and \((\mu,\beta)=e^{2\Omega v}(\widehat\mu,\widehat\beta)\).
Use the GOAL-011 regular GL normalization and the full spectral
bracket. Below \(9/8\), the degree-224 regular series cancels the
apparent factors of \(r-1\) before enclosing the metric. The independent
symbolic control explicitly checks the orders of the EF \(vr\) and
\(rr\) numerators. The fourth radial derivative is needed for a
second-order mean integrand quadrature. For each derivative of order
\(j\), the discarded analytic tail on a disk of radius \(R>\sup|r-1|\)
contributes at most \(j!\sup|{\rm tail}|/(R-\sup|r-1|)^j\).
Between \(9/8\) and \(25/16\), subdivide the pinned compact GL
profile cells and retain their radial Riccati bounds. At the join,
the independently evaluated normalized profile and all twelve EF
metric-jet entries overlap. Identity of the profiles uses the common
normalized regular solution of the pinned ODE.

At fixed \(v\), the two mean equations are
\[
 \widehat\mu_r=-\tfrac12 r^2\widehat{\mathcal T}^v{}_v,\qquad
 \widehat\beta_r=\tfrac12 r\widehat{\mathcal T}_{rr}.
\]
Initialize at \(25/16\) from the first GOAL-015 mean cell's
right endpoint and validated derivative. For a radial cell of length
\(h\) with midpoint \(m\), enclose its integral by
\(h\,q(m)\pm h^3\sup|q''|/24\), where \(q\) is the pair of
mean integrands. Subtract this integral when marching inward.
The local mass identity gives a separate enclosure for the mass
within each cell. Its use assumes the on-shell normalized GL mode,
\(\Omega\ne0\), and vanishing asymptotic mass correction. The lapse
uses the full fixed-infinity constraint integral.

The 3,072-cell 192-bit run encloses the regular zeroth and first EF
jets over the new collar. For example, the upper bounds on
\((|\widehat H_{vv}|,|\widehat H_{vr}|,|\widehat H_{rr}|,
|\widehat H_{\theta\theta}|)\) are \((1.625,16.700,13.800,16.937)\).
At the inner endpoint, the radial integral gives
\(\widehat\mu\in[7.28,7.32]\) and
\(\widehat\beta\in[74.33,74.47]\) with outward rounding.
The initial pointwise-algebra enclosure across the whole cells
was far wider near the horizon. Its \(rr\) second-jet diameter
exceeded \(2.5\times10^7\) from subtraction of singular expressions.
The regular series and subdivision improve it markedly, but the
combined collar's largest \(rr\) second-jet diameter still exceeds
\(5.7\times10^3\) in the compact-profile leg. These ranges are
whole-cell coefficient enclosures, with variation across a cell.
They do not measure the discrepancy from the binary waveform
coefficients or solve the weighted mixed residual problem.

The outer dependency is read-only. The check requires the exact
GOAL-015 source commit with clean coefficient directories, and
checks the two GOAL-015 coefficient-file SHA-256 digests.
[Source](geometry_extension.py) · [Coarse output](geometry_coarse.txt)
· [Refined output](geometry_extension.txt)
· [Initial wide enclosure](geometry_initial_failure.txt)
· [Independent horizon control](../verification/horizon_contacts.txt).
