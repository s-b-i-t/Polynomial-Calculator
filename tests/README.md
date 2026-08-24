# Native mathematical-core verification

This directory is an independent Phase 1 test suite for `Polynomial`,
`ComplexPoly`, `RationalComplex`, and `make_rational`. It does not build or test
RootFinding, WebAssembly, JavaScript, TypeScript, React, or UI code.

The suite uses a self-contained non-`assert` harness and a separate
`std::complex<double>`/`std::vector` oracle. Expected arithmetic never calls the
production operation under test. Comparisons cover complete normalized
coefficient sequences and reject NaN and both infinities. The default tolerance
is `1e-9`; oracle-only trailing-zero normalization uses `1e-12` so the oracle
does not erase production-significant inputs.

Deterministic generated checks use seed `0x5EED1234` (`1592594996`), degrees 0
through 5, integer coefficients -3 through 3, and 100 cases per tested safe
operation. A failing generated case prints the seed, iteration, complete
operands, expected result, and actual result.

Groups are independently invokable:

```sh
/tmp/polycalc-core-verification/core_verification --group polynomial
/tmp/polycalc-core-verification/core_verification --group complex
/tmp/polycalc-core-verification/core_verification --group rational
/tmp/polycalc-core-verification/core_verification --group promotion
/tmp/polycalc-core-verification/core_verification --group api
```

`--list` lists cases, and `--test group.case_name` runs one case. High-risk
ComplexPoly multiplication regressions fork one child per minimal input so a
crash or sanitizer abort cannot prevent later inputs or other components from
running.

Run all requested debug, optimized, sanitizer, compile, and isolated link probes
with:

```sh
tests/run_core_verification.sh
```

All generated binaries and object files are written beneath
`/tmp/polycalc-core-verification/`. A nonzero runner exit means the current core
failed at least one verification or public-link check; failures are deliberately
preserved rather than weakened.
