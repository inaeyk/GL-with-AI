# Early weak calculation failures

The first directed run used rectangular boxes for the entire principal phase and evaluated the bump's second derivative with interval division on every cell except the first. Its relevant output was

    chi_c^2 enclosure [1e+0 +/- 0.645]
    Bprime(d), Bsecond(d), integral Bsecond^2 upper [0.41378523 +/- 6.45e-9] [159.696705 +/- 7.67e-7] nan
    P lower nan
    AssertionError

Interval dependency in the bump denominator caused the nonfinite first positive cell. The corrected run handles the first 32 cells together using the inherited flat-cap envelope. The broad rectangular phase interval remains valid for intermediate upper bounds on the outgoing component. The decisive signed endpoint and bulk phase bounds instead use the pinned launch interval and the exact moving-graph transport. [Derivation](ADJOINT.md) · [Corrected replay](early_optical_certificate.txt).

The central-coefficient diagnostic emitted this SciPy warning in its upstream sampled IVP and completed:

    RuntimeWarning: invalid value encountered in scalar divide
      return np.abs(h) * err5_norm_2 / np.sqrt(denom * len(scale))

The coefficient-size diagnostic supplies no input to the directed sign bound.
