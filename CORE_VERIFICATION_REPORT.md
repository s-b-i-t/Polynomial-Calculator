# Phase 1 Native Mathematical-Core Verification Report

## Live Completion Checklist

### Completed

- [x] **VERIFIED** — Read the existing verification suite, harness, oracle, runner, probes, supporting documentation, `src/test.cpp`, and `src/extra_test.cpp`.
- [x] **VERIFIED** — Recorded the branch, commit, worktree status, changed paths, and recent history.
- [x] **VERIFIED** — Confirmed all eight expected files beneath `tests/` are present.
- [x] **VERIFIED** — Confirmed that `CORE_VERIFICATION_REPORT.md` was absent and created this live checkpoint report.
- [x] **VERIFIED** — Captured initial protected-tree status, hashes, and a binary diff beneath `/tmp/polycalc-core-verification/`.

### Execution Completed

- [x] **VERIFIED** — Debug verification binary compiled successfully.
- [x] **VERIFIED** — Debug `polynomial` group executed independently (exit 1).
- [x] **VERIFIED** — Debug `complex` group executed independently (exit 1).
- [x] **VERIFIED** — Debug `rational` group executed independently (exit 1).
- [x] **VERIFIED** — Debug `promotion` group executed independently (exit 1).
- [x] **VERIFIED** — Debug `api` group executed independently (exit 0).
- [x] **VERIFIED** — All five debug groups executed independently.
- [x] **VERIFIED** — Optimized (`-O2`) verification binary compiled successfully.
- [x] **VERIFIED** — Optimized `polynomial` group executed independently (exit 1).
- [x] **VERIFIED** — Optimized `complex` group executed independently (exit 1).
- [x] **VERIFIED** — Optimized `rational` group executed independently (exit 1).
- [x] **VERIFIED** — Optimized `promotion` group executed independently (exit 1).
- [x] **VERIFIED** — Optimized `api` group executed independently (exit 0).
- [x] **VERIFIED** — All five optimized groups executed independently.
- [x] **VERIFIED** — ASan/UBSan verification binary compiled successfully.
- [x] **VERIFIED** — Sanitized `polynomial` group executed independently (exit 1; logical failures only).
- [x] **VERIFIED** — Sanitized `complex` group executed independently and terminated on ASan (exit 134).
- [x] **VERIFIED** — Existing minimal high-imaginary-degree sanitizer regression executed independently (exit 1; isolated children signaled).
- [x] **VERIFIED** — Sanitized deterministic complex property case executed independently and terminated on ASan (exit 134).
- [x] **VERIFIED** — Sanitized `rational` group executed independently (exit 1; logical failures only).
- [x] **VERIFIED** — Sanitized `promotion` group executed independently (exit 1; logical failures only).
- [x] **VERIFIED** — Sanitized `api` group executed independently (exit 0).
- [x] **VERIFIED** — All five ASan/UBSan groups attempted independently; terminated complex cases were continued through existing targeted test selection.
- [x] **VERIFIED** — Compile-time API expression probe compiled successfully (exit 0).
- [x] **VERIFIED** — `ComplexPoly::Undefined()` isolated link probe failed with unresolved reference (exit 1).
- [x] **VERIFIED** — `ComplexPoly::isUndefined() const` isolated link probe failed with unresolved reference (exit 1).

### Remaining

- [x] **VERIFIED** — Final protected-file hashes, status, HEAD diff, and parent diff are byte-for-byte identical to the takeover snapshot.
- [x] **VERIFIED** — Final report structure/scope audit completed and only permitted verification paths committed.

### Commands Completed at This Checkpoint

| Command | Exit | Result | Configuration |
|---|---:|---|---|
| `git branch --show-current` | 0 | **VERIFIED** — `portfolio-poly-cleanup` | Audit |
| `git rev-parse HEAD` | 0 | **VERIFIED** — `3021a17bce36f4ef05630f23d40fe9dc371cbb98` | Audit |
| `git status --short` | 0 | **VERIFIED** — dirty inherited worktree; `tests/` untracked | Audit |
| `git diff --name-only` | 0 | **VERIFIED** — 13 tracked worktree paths differ from HEAD | Audit |
| `git log -3 --oneline` | 0 | **VERIFIED** — `3021a17`, `d429185`, `67c546e` | Audit |
| `git rev-parse HEAD^` | 0 | **VERIFIED** — `d42918511bcd4067df06cbb43437f21947dc5883` | Audit |
| `git log --all --oneline -- tests CORE_VERIFICATION_REPORT.md` | 0 | **VERIFIED** — no output; no committed verification artifacts found | Audit |
| `g++ --version` | 0 | **VERIFIED** — GCC 13.3.0 | Environment |
| `uname -a` | 0 | **VERIFIED** — Linux x86-64 WSL2 host | Environment |
| `date --iso-8601=seconds` | 0 | **VERIFIED** — `2026-08-24T17:44:32-04:00` | Environment |
| `git diff --name-status HEAD -- include src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp src/extra_polynomials.cpp wasm web README.md src/readme.md makefile .gitignore .github` | 0 | **VERIFIED** — 11 protected paths already differ from HEAD | Audit |
| `git diff --name-status HEAD^ -- include src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp src/extra_polynomials.cpp wasm web README.md src/readme.md makefile .gitignore .github` | 0 | **VERIFIED** — at pre-commit audit time, 12 protected paths differed from starting HEAD's first parent | Audit |
| `g++ -std=c++17 -O0 -g -Wall -Wextra -Wpedantic -Iinclude src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp tests/core_verification.cpp -o /tmp/polycalc-core-verification/core_verification > /tmp/polycalc-core-verification/debug-build.log 2>&1` | 0 | **VERIFIED** — build passed; warnings only | Debug build |
| `/tmp/polycalc-core-verification/core_verification --group polynomial > /tmp/polycalc-core-verification/debug-polynomial.log 2>&1` | 1 | **VERIFIED** — 2 tests passed, 4 failed; 1,487 checks passed, 18 failed | Debug |
| `/tmp/polycalc-core-verification/core_verification --group complex > /tmp/polycalc-core-verification/debug-complex.log 2>&1` | 1 | **VERIFIED** — 3 tests passed, 3 failed; 1,166 checks passed, 8 failed | Debug |
| `/tmp/polycalc-core-verification/core_verification --group rational > /tmp/polycalc-core-verification/debug-rational.log 2>&1` | 1 | **VERIFIED** — 4 tests passed, 1 failed; 490 checks passed, 9 failed | Debug |
| `/tmp/polycalc-core-verification/core_verification --group promotion > /tmp/polycalc-core-verification/debug-promotion.log 2>&1` | 1 | **VERIFIED** — 1 test passed, 1 failed; 417 checks passed, 7 failed | Debug |
| `/tmp/polycalc-core-verification/core_verification --group api > /tmp/polycalc-core-verification/debug-api.log 2>&1` | 0 | **VERIFIED** — 3 tests passed, 0 failed; 87 checks passed, 0 failed | Debug |
| `g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp tests/core_verification.cpp -o /tmp/polycalc-core-verification/core_verification_o2 > /tmp/polycalc-core-verification/optimized-build.log 2>&1` | 0 | **VERIFIED** — build passed; warnings only | Optimized build |
| `/tmp/polycalc-core-verification/core_verification_o2 --group polynomial > /tmp/polycalc-core-verification/optimized-polynomial.log 2>&1` | 1 | **VERIFIED** — 2 tests passed, 4 failed; 1,487 checks passed, 18 failed | Optimized |
| `/tmp/polycalc-core-verification/core_verification_o2 --group complex > /tmp/polycalc-core-verification/optimized-complex.log 2>&1` | 1 | **VERIFIED** — 3 tests passed, 3 failed; 1,166 checks passed, 8 failed | Optimized |
| `/tmp/polycalc-core-verification/core_verification_o2 --group rational > /tmp/polycalc-core-verification/optimized-rational.log 2>&1` | 1 | **VERIFIED** — 4 tests passed, 1 failed; 490 checks passed, 9 failed | Optimized |
| `/tmp/polycalc-core-verification/core_verification_o2 --group promotion > /tmp/polycalc-core-verification/optimized-promotion.log 2>&1` | 1 | **VERIFIED** — 1 test passed, 1 failed; 417 checks passed, 7 failed | Optimized |
| `/tmp/polycalc-core-verification/core_verification_o2 --group api > /tmp/polycalc-core-verification/optimized-api.log 2>&1` | 0 | **VERIFIED** — 3 tests passed, 0 failed; 87 checks passed, 0 failed | Optimized |
| `g++ -std=c++17 -O1 -g -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp tests/core_verification.cpp -o /tmp/polycalc-core-verification/core_verification_sanitized > /tmp/polycalc-core-verification/sanitizer-build.log 2>&1` | 0 | **VERIFIED** — build passed; warnings only | ASan/UBSan build |
| `ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 /tmp/polycalc-core-verification/core_verification_sanitized --group polynomial > /tmp/polycalc-core-verification/sanitized-polynomial.log 2>&1` | 1 | **VERIFIED** — same 18 logical check failures; no sanitizer diagnostic | ASan/UBSan |
| `ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 /tmp/polycalc-core-verification/core_verification_sanitized --group complex > /tmp/polycalc-core-verification/sanitized-complex.log 2>&1` | 134 | **VERIFIED** — ASan heap-buffer-overflow; process aborted | ASan/UBSan |
| `ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 /tmp/polycalc-core-verification/core_verification_sanitized --test complex.high_imaginary_degree_memory_regressions > /tmp/polycalc-core-verification/sanitized-complex-high-imag-minimal.log 2>&1` | 1 | **VERIFIED** — all three isolated inputs triggered ASan heap-buffer-overflow at `src/Complex.cpp:125`; each child signal 6 | ASan/UBSan targeted |
| `ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 /tmp/polycalc-core-verification/core_verification_sanitized --test complex.deterministic_generated_safe_properties > /tmp/polycalc-core-verification/sanitized-complex-generated.log 2>&1` | 134 | **VERIFIED** — ASan heap-buffer-overflow at `src/Complex.cpp:125`, called by test line 905 | ASan/UBSan targeted |
| `ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 /tmp/polycalc-core-verification/core_verification_sanitized --group rational > /tmp/polycalc-core-verification/sanitized-rational.log 2>&1` | 1 | **VERIFIED** — same 9 logical check failures; no sanitizer diagnostic | ASan/UBSan |
| `ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 /tmp/polycalc-core-verification/core_verification_sanitized --group promotion > /tmp/polycalc-core-verification/sanitized-promotion.log 2>&1` | 1 | **VERIFIED** — same 7 logical check failures; no sanitizer diagnostic | ASan/UBSan |
| `ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 /tmp/polycalc-core-verification/core_verification_sanitized --group api > /tmp/polycalc-core-verification/sanitized-api.log 2>&1` | 0 | **VERIFIED** — 3 tests passed, 87 checks passed; no sanitizer diagnostic | ASan/UBSan |
| `g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude tests/probes/api_compile_probe.cpp -c -o /tmp/polycalc-core-verification/api_compile_probe.o > /tmp/polycalc-core-verification/api-compile-probe.log 2>&1` | 0 | **VERIFIED** — compile-only API probe passed; warning only | API compile probe |
| `g++ -std=c++17 -Iinclude src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp tests/probes/complex_undefined_link_probe.cpp -o /tmp/polycalc-core-verification/complex_undefined_link_probe > /tmp/polycalc-core-verification/complex-undefined-link-probe.log 2>&1` | 1 | **VERIFIED** — undefined reference to `ComplexPoly::Undefined()` | API link probe |
| `g++ -std=c++17 -Iinclude src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp tests/probes/complex_is_undefined_link_probe.cpp -o /tmp/polycalc-core-verification/complex_is_undefined_link_probe > /tmp/polycalc-core-verification/complex-is-undefined-link-probe.log 2>&1` | 1 | **VERIFIED** — undefined reference to `ComplexPoly::isUndefined() const` | API link probe |
| `g++ -std=c++17 -O0 -g -Wall -Wextra -Wpedantic -Iinclude src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp src/test.cpp -o /tmp/polycalc-core-verification/historical_test > /tmp/polycalc-core-verification/historical-build.log 2>&1` | 0 | **VERIFIED** — historical smoke binary compiled; warnings only | Historical debug build |
| `/tmp/polycalc-core-verification/historical_test > /tmp/polycalc-core-verification/historical-run.log 2>&1` | 0 | **VERIFIED** — historical smoke program printed `All tests passed!` | Historical debug run |
| `cmp -s /tmp/polycalc-core-verification/initial-protected-vs-head.patch /tmp/polycalc-core-verification/final-protected-vs-head.patch` | 0 | **VERIFIED** — protected diff against HEAD unchanged | Final protected audit |
| `cmp -s /tmp/polycalc-core-verification/initial-protected-status.txt /tmp/polycalc-core-verification/final-protected-status.txt` | 0 | **VERIFIED** — protected status unchanged | Final protected audit |
| `cmp -s /tmp/polycalc-core-verification/initial-protected-vs-parent.txt /tmp/polycalc-core-verification/final-protected-vs-parent.txt` | 0 | **VERIFIED** — protected diff against starting HEAD's first parent unchanged | Final protected audit |
| `cmp -s /tmp/polycalc-core-verification/initial-protected-hashes.txt /tmp/polycalc-core-verification/final-protected-hashes.txt` | 0 | **VERIFIED** — protected file hashes unchanged | Final protected audit |
| `git status --short` | 0 | **VERIFIED** — only inherited dirty paths plus permitted report/tests are present | Final pre-commit audit |
| `git diff --stat` | 0 | **VERIFIED** — inherited tracked diff plus permitted `src/test.cpp`; untracked report/tests are not shown by this command | Final pre-commit audit |
| `git diff --name-only` | 0 | **VERIFIED** — no new protected path relative to takeover | Final pre-commit audit |
| `git add -- CORE_VERIFICATION_REPORT.md src/test.cpp tests` | 0 | **VERIFIED** — staged only permitted verification paths | Commit preparation |
| `git diff --cached --name-only` | 0 | **VERIFIED** — report, `src/test.cpp`, and eight `tests/**` files only | Commit scope audit |
| `git diff --cached --check` | 0 | **VERIFIED** — no staged whitespace errors | Commit scope audit |
| `git commit -m "test: complete native polynomial core verification"` | 0 | **VERIFIED** — local verification commit created; no push or PR | Commit |

### Verdict Impact at This Checkpoint

**VERIFIED** — The native core builds in all three configurations, but reproducible mathematical failures, an ASan-confirmed heap-buffer-overflow, and two unresolved public API symbols require `CORE_FAIL`. Existing protected-path changes are an inherited repository condition; byte-for-byte final comparison confirms this continuation did not alter them.

## 1. Executive Summary

**VERIFIED** — PolyEngineer-2 completed all five groups in debug and optimized configurations, attempted all five ASan/UBSan groups independently, continued the sanitizer-terminated complex coverage through existing named tests, compiled the standalone API expression probe, and ran both isolated link probes.

**VERIFIED** — Debug and optimized execution each produced the same result: 13 of 22 tests passed, 9 failed; 3,647 checks passed, 42 failed. The sanitizer campaign confirmed a `heap-buffer-overflow` in `ComplexPoly::operator*` at `src/Complex.cpp:125`, with `(i*x)*(i*x)` as the smallest existing trigger. The compile-only API probe passed, while both public undefined-state symbol probes failed to link.

Final verdict: **CORE_FAIL**

WASM_READY: **NO**

## 2. Repository Baseline

- **VERIFIED** — Repository: `s-b-i-t/Polynomial-Calculator` local checkout.
- **VERIFIED** — Branch: `portfolio-poly-cleanup`.
- **VERIFIED** — Starting HEAD: `3021a17bce36f4ef05630f23d40fe9dc371cbb98` (`any change`).
- **VERIFIED** — First parent: `d42918511bcd4067df06cbb43437f21947dc5883` (`cleaned up`).
- **VERIFIED** — No committed verification checkpoint exists in reachable history; all eight `tests/**` files were untracked at takeover.
- **VERIFIED** — The expected checkpoint report was missing at takeover.
- **VERIFIED** — No staged changes existed at takeover.
- **VERIFIED** — Compiler: `g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`.
- **VERIFIED** — Host: Linux x86-64 under WSL2 (`6.6.87.2-microsoft-standard-WSL2`).
- **VERIFIED** — Execution timestamp: `2026-08-24T17:44:32-04:00`.

Starting `git status --short`:

```text
 M README.md
 M ec
 D include/RootFinding.h
 M makefile
 D src/RootFinding.cpp
 M src/test.cpp
 M src/wasm_api.cpp
 M wasm/build_wasm.sh
 M web/public/poly_wasm.js
 M web/public/poly_wasm.wasm
 M web/src/App.css
 M web/src/App.tsx
 M web/src/wasm/polyWasm.ts
?? tests/
```

## 3. Protected-File Verification

**VERIFIED** — Before this continuation executed any build or test, 11 protected paths already differed from HEAD:

```text
M  README.md
D  include/RootFinding.h
M  makefile
D  src/RootFinding.cpp
M  src/wasm_api.cpp
M  wasm/build_wasm.sh
M  web/public/poly_wasm.js
M  web/public/poly_wasm.wasm
M  web/src/App.css
M  web/src/App.tsx
M  web/src/wasm/polyWasm.ts
```

**VERIFIED** — Relative to the starting HEAD's first parent `d42918511bcd4067df06cbb43437f21947dc5883`, the takeover baseline differs in 12 protected paths: `README.md`, `include/Complex.h`, `include/Polynomial.h`, `makefile`, `src/Polynomial.cpp`, `src/wasm_api.cpp`, `wasm/build_wasm.sh`, both generated WebAssembly files, `web/src/App.css`, `web/src/App.tsx`, and `web/src/wasm/polyWasm.ts`.

**VERIFIED** — Initial protected snapshots are stored at:

- `/tmp/polycalc-core-verification/initial-protected-vs-head.patch`
- `/tmp/polycalc-core-verification/initial-protected-status.txt`
- `/tmp/polycalc-core-verification/initial-protected-vs-parent.txt`
- `/tmp/polycalc-core-verification/initial-protected-hashes.txt`

**VERIFIED** — Final comparison exit 0 for all four artifacts: protected file hashes, protected status relative to the starting HEAD, binary protected diff relative to the starting HEAD, and protected name/status diff relative to starting HEAD's first parent are identical to the takeover snapshots.

**VERIFIED** — `PROTECTED CORE CHANGED BY POLYENGINEER-2: NO`.

**VERIFIED** — `PROTECTED PATHS ALREADY DIRTY AT TAKEOVER: YES` (11 paths versus HEAD, documented above). This distinction prevents attributing inherited user/checkpoint changes to this campaign.

## 4. Historical-Test Audit

**VERIFIED** — `src/test.cpp` is an assertion-based historical smoke suite covering basic polynomial zero/divide-by-zero safety plus basic `RationalComplex` arithmetic. Its assertions are disabled by `NDEBUG`, it uses spot checks rather than a separate mathematical oracle, and it does not cover the complete Phase 1 surface.

**VERIFIED** — `src/extra_test.cpp` consists entirely of deprecated/commented-out test material and is not executable coverage.

**VERIFIED** — The active historical smoke program compiled in debug mode with exit 0 and ran with exit 0, printing `All tests passed!`. This does not override the independent suite failures because the historical checks omit the failing edge cases and memory instrumentation. Logs: `/tmp/polycalc-core-verification/historical-build.log`, `/tmp/polycalc-core-verification/historical-run.log`.

## 5. Test Infrastructure

**VERIFIED** — The checkpoint architecture was retained: `tests/test_harness.h`, `tests/oracle_math.h`, `tests/core_verification.cpp`, `tests/run_core_verification.sh`, three probes, and `tests/README.md`.

**VERIFIED** — The harness does not depend on `assert`, rejects non-finite numeric comparisons, uses a separate `std::complex<double>`/`std::vector` oracle, and supports independent group/test selection.

**VERIFIED** — Deterministic generated tests use seed `0x5EED1234` (`1592594996`), degrees 0 through 5, coefficients -3 through 3, and 100 generated cases per covered operation.

**VERIFIED** — High-imaginary-degree multiplication regressions already isolate each minimal input in a child process so one crash does not stop later checks.

**VERIFIED** — The suite contains 22 registered cases: 6 polynomial, 6 complex, 5 rational, 2 promotion, and 3 API cases.

**VERIFIED** — The existing architecture was not rewritten. Manual configuration/group invocations were used instead of one aggregate runner call so logs could be separated and this report could be checkpointed after every batch.

## 6. Debug Results

**VERIFIED** — The debug binary compiled with exit code 0. The 178-line compiler log contains warnings (including signed/unsigned comparisons, a multi-line comment, and deprecated implicit copy assignment) but no build error. Full output: `/tmp/polycalc-core-verification/debug-build.log`.

**VERIFIED** — `polynomial` exited 1: 2 tests passed, 4 failed; 1,487 checks passed, 18 failed. Failures occurred in `equality_and_inequality`, `division_and_remainder`, `remainder_state_regressions`, and `deterministic_generated_properties`. Full output: `/tmp/polycalc-core-verification/debug-polynomial.log`.

**VERIFIED** — `complex` exited 1: 3 tests passed, 3 failed; 1,166 checks passed, 8 failed. Failures occurred in `constant_and_nonconstant_division`, `deterministic_generated_safe_properties`, and `high_imaginary_degree_memory_regressions`. Full output: `/tmp/polycalc-core-verification/debug-complex.log`.

**VERIFIED** — `rational` exited 1: 4 tests passed, 1 failed; 490 checks passed, 9 failed. Only `scale_sensitive_zero_normalization` failed. Full output: `/tmp/polycalc-core-verification/debug-rational.log`.

**VERIFIED** — `promotion` exited 1: 1 test passed, 1 failed; 417 checks passed, 7 failed. `value_preservation_and_chains` failed only at the scale-sensitive later-arithmetic check. Full output: `/tmp/polycalc-core-verification/debug-promotion.log`.

**VERIFIED** — `api` exited 0: all 3 tests and all 87 checks passed. Full output: `/tmp/polycalc-core-verification/debug-api.log`.

**VERIFIED** — Debug aggregate from the five independent group summaries: 13 of 22 tests passed, 9 failed; 3,647 checks passed, 42 failed. Debug configuration result: FAIL.

## 7. Optimized Results

**VERIFIED** — The `-O2` binary compiled with exit code 0. Its 178-line log contains the same warning categories as the debug build and no build error. Full output: `/tmp/polycalc-core-verification/optimized-build.log`.

**VERIFIED** — Optimized `polynomial` exited 1 with the same test/check totals and failure set as debug: 2 tests passed, 4 failed; 1,487 checks passed, 18 failed. Full output: `/tmp/polycalc-core-verification/optimized-polynomial.log`.

**VERIFIED** — Optimized `complex` exited 1 with the same test/check totals and failure set as debug: 3 tests passed, 3 failed; 1,166 checks passed, 8 failed. All isolated children returned mathematical mismatch exit 3 rather than crashing. Full output: `/tmp/polycalc-core-verification/optimized-complex.log`.

**VERIFIED** — Optimized `rational` exited 1 with the same test/check totals and failure set as debug: 4 tests passed, 1 failed; 490 checks passed, 9 failed. Full output: `/tmp/polycalc-core-verification/optimized-rational.log`.

**VERIFIED** — Optimized `promotion` exited 1 with the same test/check totals and failure set as debug: 1 test passed, 1 failed; 417 checks passed, 7 failed. Full output: `/tmp/polycalc-core-verification/optimized-promotion.log`.

**VERIFIED** — Optimized `api` exited 0: all 3 tests and all 87 checks passed. Full output: `/tmp/polycalc-core-verification/optimized-api.log`.

**VERIFIED** — Optimized aggregate from the five independent group summaries: 13 of 22 tests passed, 9 failed; 3,647 checks passed, 42 failed. The failure set and reported values match debug. Optimized configuration result: FAIL.

## 8. Sanitizer Results

**VERIFIED** — The ASan/UBSan binary compiled with exit code 0. Its 178-line log contains warnings but no build error. Full output: `/tmp/polycalc-core-verification/sanitizer-build.log`.

**VERIFIED** — Sanitized `polynomial` exited 1 with the same 2/4 test and 1,487/18 check pass/fail totals as debug/optimized. The log contains no ASan, UBSan, or LeakSanitizer diagnostic; its failures are logical verification failures. Full output: `/tmp/polycalc-core-verification/sanitized-polynomial.log`.

**VERIFIED** — Sanitized `complex` aborted with exit 134. ASan category: heap-buffer-overflow READ; top fault location: `ComplexPoly::operator*(ComplexPoly const&) const` at `src/Complex.cpp:125`; immediate test caller: `complex.constant_and_nonconstant_division` at `tests/core_verification.cpp:870`. The first three complex cases passed and three exact division checks failed before the terminating multiplication. Full output: `/tmp/polycalc-core-verification/sanitized-complex.log`.

**VERIFIED** — The existing targeted regression `complex.high_imaginary_degree_memory_regressions` exited 1 after all three isolated child inputs triggered ASan `heap-buffer-overflow` at `src/Complex.cpp:125`. The smallest existing input is `(i*x)*(i*x)`, called from test line 925; its child ended by signal 6. Full output: `/tmp/polycalc-core-verification/sanitized-complex-high-imag-minimal.log`.

**VERIFIED** — Independently rerunning `complex.deterministic_generated_safe_properties`, which the full group had not reached, aborted with exit 134 on the same ASan category/location from test line 905. Full output: `/tmp/polycalc-core-verification/sanitized-complex-generated.log`.

**VERIFIED** — Sanitized `rational` exited 1 with the same 4/1 test and 490/9 check pass/fail totals as debug/optimized. The log contains no ASan, UBSan, or LeakSanitizer diagnostic. Full output: `/tmp/polycalc-core-verification/sanitized-rational.log`.

**VERIFIED** — Sanitized `promotion` exited 1 with the same 1/1 test and 417/7 check pass/fail totals as debug/optimized. The log contains no ASan, UBSan, or LeakSanitizer diagnostic. Full output: `/tmp/polycalc-core-verification/sanitized-promotion.log`.

**VERIFIED** — Sanitized `api` exited 0: all 3 tests and 87 checks passed, with no sanitizer diagnostic. Full output: `/tmp/polycalc-core-verification/sanitized-api.log`.

**VERIFIED** — Every sanitizer group was invoked independently. `polynomial`, `rational`, `promotion`, and `api` completed; `complex` terminated on ASan and was continued using the existing named deterministic and high-imaginary-degree tests. All six named complex cases were entered across the group and targeted reruns.

**VERIFIED** — The full complex case aborted at test line 870, so later checks in that case did not execute under sanitizers and no complete sanitized complex totals exist.

**VERIFIED** — No distinct UBSan or LeakSanitizer diagnostic appeared. ASan halt-on-error prevented complete UBSan/LeakSanitizer observation in affected processes, so absence of a separate diagnostic is not evidence that later issues are impossible. Sanitizer configuration result: FAIL.

## 9. Polynomial Results

**VERIFIED** — Debug equality is storage-sensitive. A value `[1,-2,3]` compares unequal to the mathematically identical value produced by expanding storage to index 7 with a zero coefficient. Both `left == expanded_zero` and its `!=` complement failed.

**VERIFIED** — Debug remainder state is missing or stale when the dividend degree is below the divisor degree. Fresh lower-degree cases returned zero instead of the dividend; results also varied with preceding division/remainder calls and leaked an incorrect zero into a second object case.

**VERIFIED** — Four deterministic generated lower-degree division cases (seed `1592594996`, iterations 8, 40, 65, and 91) returned zero remainder and consequently failed reconstruction.

**VERIFIED** — The debug polynomial group had 18 failed checks total; these comprise 2 equality checks, 8 targeted division/remainder checks, and 8 deterministic remainder/reconstruction checks.

**VERIFIED** — The identical polynomial failures reproduced under `-O2` and ASan/UBSan. No sanitizer diagnostic accompanied the polynomial failures.

**VERIFIED** — Source mapping is consistent with the output: the lower-degree division return at `src/Polynomial.cpp:142` occurs before remainder assignment, while `%` later consumes the stored remainder; equality at `src/Polynomial.cpp:217` compares stored vector size/content after `SetCoeffAt` can retain a trailing-zero expansion.

**VERIFIED** — No mathematical/tolerance equality contract was found in the public API documentation. Storage-sensitive equality is verified behavior and a suite failure.

**PRELIMINARY** — Whether exact stored-representation equality was intended cannot be resolved from the documentation.

## 10. ComplexPoly Results

**VERIFIED** — Debug constant division cases passed, while three exact nonconstant requirements failed. `(x^2-1)/(x-1)` returned coefficients `[1,-1,-1,1]` instead of `[1,1]`; an exact complex-coefficient dividend/divisor pair returned three coefficients instead of constant `2+i`; and `x/x` returned two NaNs and an infinity instead of finite polynomial one.

**VERIFIED** — The additional general nonconstant reconstruction check failed. Because that fixture may be non-exact under ordinary quotient-with-remainder semantics, it is retained as contract-sensitive supporting evidence and is not needed to establish the exact-division defect.

**VERIFIED** — One deterministic complex distributivity check failed at seed `1592594996`, iteration 44, with the highest expected coefficient omitted.

**VERIFIED** — All three isolated high-imaginary-degree multiplication inputs produced truncated/wrong products in the debug binary. Each child exited 3 after a mathematical mismatch; no debug child crashed.

**VERIFIED** — ASan confirms that the same public-input family causes `heap-buffer-overflow` at `src/Complex.cpp:125`. The minimal existing `(i*x)*(i*x)` child ended by signal 6; the two larger isolated inputs independently produced the same ASan category/location.

**VERIFIED** — Source mapping is consistent with the sanitizer output: result vectors are sized from real coefficient lengths at `src/Complex.cpp:109`–`113`, then indexed using potentially longer imaginary coefficient lengths at lines 122–139. The public two-vector constructor states no restriction requiring imaginary degree to be no greater than real degree.

**VERIFIED** — The exact `x/x` division failure follows the implementation path at `src/Complex.cpp:146`: the conjugate product is reduced by denominator coefficient index 0, which is zero for `x*x`, producing non-finite coefficients.

**VERIFIED** — No documented contract requires a non-exact `ComplexPoly` quotient to reconstruct the dividend without a remainder. The non-exact round-trip failure is reported as contract-sensitive only and is not counted as a standalone blocker; exact-division and sanitizer evidence establish the failures independently.

## 11. RationalComplex Results

**VERIFIED** — Debug construction, ordinary normalization/exceptions, structural/compound arithmetic, semantic checks, and all 100 deterministic generated cases passed.

**VERIFIED** — Scale-sensitive zero normalization failed. The nonzero value `5e-10 / 1e-8`, mathematically `0.05`, was canonicalized to `0/1` and evaluated as zero at all seven safe sample points. Constructing `1 / 5e-10` threw `RationalComplex: zero denominator`.

**VERIFIED** — The same nine checks failed under debug, `-O2`, and ASan/UBSan, with no sanitizer diagnostic. The behavior is consistent with the fixed absolute `1e-9` zero threshold used at `src/RationalComplex.cpp:28`.

**VERIFIED** — The public documentation does not define a normalization tolerance policy. The observed loss/rejection of nonzero rational values is verified.

**PRELIMINARY** — The intended numerical threshold policy requires a production design decision.

## 12. Promotion Matrix

**VERIFIED** — Direct value preservation passed for `Polynomial -> ComplexPoly`, explicit `ComplexPoly -> RationalComplex`, `make_rational(Polynomial)`, and `make_rational(ComplexPoly)`, including sparse/fractional values and 100 deterministic generated cases.

**NOT REPRODUCED** — Tiny direct-promotion preservation was not actually established. The structural assertion for `make_rational(Polynomial(5e-10))` uses the suite-wide absolute `1e-9` comparison tolerance, so it accepts actual zero as near expected `5e-10`.

**VERIFIED** — Source tracing shows `make_rational(Polynomial)` calls the normalizing numerator-only `RationalComplex` constructor, which classifies `5e-10` as zero at the fixed `1e-9` threshold. The subsequent semantic check exposes the effect: dividing the promoted value by promoted `Polynomial(1e-8)` returns zero rather than `0.05` at all seven sample points. Promotion therefore FAILS for the tiny value under the same `RATIONAL-SCALE-ZERO` root cause.

**VERIFIED** — The standalone compile-time expression/API probe compiled with exit 0, confirming the supported/unsupported expression and explicit-construction matrix at compile time.

**VERIFIED** — The runtime/compile-expression matrix inside the debug suite supports all four operations for `Polynomial` with `Polynomial`, `int`, `double`, and `ComplexPoly` in the declared operand orders; all four same-type `ComplexPoly` and `RationalComplex` operations also compile and were exercised. Direct `RationalComplex` expressions with `Polynomial` or `ComplexPoly` in either direction do not compile in the probed API; callers can use `make_rational`.

| Left | Right | `+ - * /` compile | Result | Runtime coverage | Classification |
|---|---|---|---|---|---|
| `Polynomial` | `Polynomial` | **VERIFIED** — YES | `Polynomial` | **VERIFIED** — YES | Same-type supported |
| `Polynomial` | `int` / `double` | **VERIFIED** — YES | `Polynomial` | **VERIFIED** — YES | RHS implicit scalar conversion |
| `int` / `double` | `Polynomial` | **VERIFIED** — YES | `Polynomial` | **VERIFIED** — YES | Scalar-left overloads |
| `Polynomial` | `ComplexPoly` | **VERIFIED** — YES | `ComplexPoly` | **VERIFIED** — YES | Direct mixed overloads |
| `ComplexPoly` | `Polynomial` | **VERIFIED** — YES | `ComplexPoly` | **VERIFIED** — YES | Direct mixed overloads |
| `ComplexPoly` | `ComplexPoly` | **VERIFIED** — YES | `ComplexPoly` | **VERIFIED** — YES | Same-type supported; nonconstant `/` has verified defects |
| `RationalComplex` | `RationalComplex` | **VERIFIED** — YES | `RationalComplex` | **VERIFIED** — YES | Same-type supported |
| `RationalComplex` | `Polynomial` / `ComplexPoly` (either direction) | **VERIFIED** — NO | — | **VERIFIED** — NO | Use `make_rational` |

| Construction/promotion | Constructible | Implicitly convertible | Verified result |
|---|---|---|---|
| `Polynomial -> ComplexPoly` | **VERIFIED** — YES | **VERIFIED** — NO | Value preserved |
| `vector<double> -> ComplexPoly` | **VERIFIED** — YES | **VERIFIED** — NO | Explicit construction supported |
| `ComplexPoly -> RationalComplex` | **VERIFIED** — YES | **VERIFIED** — NO | Ordinary fixtures preserved; sub-`1e-9` values are normalized as zero |
| `Polynomial -> RationalComplex` directly | **VERIFIED** — NO | **VERIFIED** — NO | Use helper |
| `make_rational(Polynomial)` | **VERIFIED** — YES | N/A | `RationalComplex` |
| `make_rational(ComplexPoly)` | **VERIFIED** — YES | N/A | `RationalComplex` |

## 13. API Compile/Link Completeness

**VERIFIED** — The in-suite debug API group passed its 87 compile-expression, construction, runtime, and declaration-inventory checks.

**VERIFIED** — The API group prints that `ComplexPoly::Undefined()` and `ComplexPoly::isUndefined() const` are unresolved, but that text is hardcoded inventory output; only the isolated probe diagnostics below are treated as link evidence.

**VERIFIED** — The standalone compile-only expression probe exited 0. Its 9-line log contains only the existing multi-line-comment warning. Full output: `/tmp/polycalc-core-verification/api-compile-probe.log`.

**VERIFIED** — The isolated `ComplexPoly::Undefined()` probe failed to link with exit 1: `undefined reference to ComplexPoly::Undefined()`. Full output: `/tmp/polycalc-core-verification/complex-undefined-link-probe.log`.

**VERIFIED** — The isolated `ComplexPoly::isUndefined() const` probe failed to link with exit 1: `undefined reference to ComplexPoly::isUndefined() const`. Full output: `/tmp/polycalc-core-verification/complex-is-undefined-link-probe.log`.

**VERIFIED** — API probe result: compile-time expression probe PASS; both public-symbol link probes FAIL. API compile/link completeness: FAIL.

## 14. Failure Inventory

1. **VERIFIED** — `POLY-EQUALITY-STORAGE`: mathematically equal polynomial values can compare unequal after zero-only storage expansion. Debug trigger: `polynomial.equality_and_inequality`.
2. **VERIFIED** — `POLY-REMAINDER-STATE`: lower-degree remainder can be missing or depend on stale operation/object state. Debug triggers: `polynomial.division_and_remainder`, `polynomial.remainder_state_regressions`, and generated seed iterations 8/40/65/91.
3. **VERIFIED** — `COMPLEX-DIVISION-NONCONSTANT`: exact nonconstant division is mathematically wrong; the minimal `x/x` result contains NaN and infinity. Debug trigger: `complex.constant_and_nonconstant_division`.
4. **VERIFIED** — `COMPLEX-MULTIPLICATION-HIGH-IMAG-CORRECTNESS`: all three isolated higher-imaginary-degree products are truncated or wrong, and deterministic complex distributivity loses its highest coefficient at seed iteration 44. Debug and optimized triggers: `complex.high_imaginary_degree_memory_regressions` and `complex.deterministic_generated_safe_properties`.
5. **VERIFIED** — `COMPLEX-MULTIPLICATION-HIGH-IMAG-MEMORY`: ASan reports `heap-buffer-overflow` at `src/Complex.cpp:125` in `ComplexPoly::operator*`. The full complex group aborted with exit 134. The smallest existing trigger, `(i*x)*(i*x)`, reproduced in an isolated child that ended by signal 6; both larger existing inputs reproduced the same category/location independently.
6. **VERIFIED** — `RATIONAL-SCALE-ZERO`: fixed-scale zero classification changes nonzero values, rejects a nonzero denominator, and defeats tiny-value promotion. Triggers: `rational.scale_sensitive_zero_normalization` and `promotion.value_preservation_and_chains`.
7. **VERIFIED** — `API-LINK-UNDEFINED-FACTORY`: public `ComplexPoly::Undefined()` is declared but an actual call fails to link with an undefined reference.
8. **VERIFIED** — `API-LINK-UNDEFINED-PREDICATE`: public `ComplexPoly::isUndefined() const` is declared but an actual call fails to link with an undefined reference.

**VERIFIED** — Items 1–4 and 6 reproduced in both debug and optimized execution; logical items 1, 2, and 6 also reproduced in completed sanitized groups. Item 5 is sanitizer-confirmed. Items 7–8 are isolated link failures independent of optimization mode.

## 15. Intentional Limitations

- **VERIFIED** — Phase 1 is limited to the native mathematical core: `Polynomial`, `ComplexPoly`, `RationalComplex`, promotion helpers, and their public native API surface.
- **VERIFIED** — Root finding, WebAssembly, JavaScript, TypeScript, React, UI behavior, performance benchmarking, fuzzing beyond the fixed deterministic properties, concurrency, and production repairs are outside scope.
- **VERIFIED** — `ComplexPoly` has no public remainder operator and documents no non-exact quotient semantics. Only exact division cases are counted as unambiguous division blockers; the non-exact reconstruction fixture is retained as contract-sensitive supporting output.
- **VERIFIED** — ASan halt-on-error necessarily prevented the full complex process from reaching checks after its first overflow. Existing named reruns reached the deterministic and high-imaginary-degree cases; debug and optimized runs completed every complex check.
- **VERIFIED** — The fixed deterministic properties are reproducible coverage, not exhaustive proof for all degrees, magnitudes, or floating-point conditions.
- **VERIFIED** — Rational semantic sampling skips a point when either expected or actual denominator is near zero; a spurious actual pole at a sample point could therefore be masked. Structural checks and the scale regression still establish the reported failure.
- **VERIFIED** — The harness `CHECK` macro evaluates its expression twice. Current `CHECK` expressions are side-effect-free, so no observed result is invalidated, but future stateful checks would need care.
- **VERIFIED** — Isolated child regressions have no timeout. No hang occurred in this campaign.
- **VERIFIED** — Oracle normalization uses an absolute `1e-12` zero/trim threshold. The scale regression uses `5e-10`, so the oracle did not erase its expected nonzero value.
- **VERIFIED** — Structural production-vs-oracle comparisons use absolute tolerance `1e-9`; therefore the tiny direct-promotion check cannot distinguish zero from `5e-10`. The seven semantic `0` versus `0.05` checks and the rational denominator/structure checks still verify the root defect.
- **VERIFIED** — Undefined-state representation/behavior could not be tested because both declared public symbols fail at link time.

## 16. WebAssembly Readiness

WASM_READY: **NO**

**VERIFIED** — Final native verdict is `CORE_FAIL`, so the required readiness rule sets `WASM_READY: NO`.

**NOT EXECUTED** — No WebAssembly build, binding, JavaScript, TypeScript, React, or UI work was started.

## 17. Project Manager Summary

**VERIFIED** — All required native configurations and API probes are complete. The core builds, and much of the ordinary arithmetic surface passes, but it is not release-ready: polynomial remainder/equality edge behavior fails, exact nonconstant complex division is wrong and can return NaN/infinity, valid complex multiplication inputs cause an ASan heap-buffer-overflow, scale-sensitive rational values are lost/rejected, and two declared public methods cannot link.

**VERIFIED** — Debug and optimized failures are stable and identical. The historical smoke test passes only because it does not cover the failing edge cases. Final byte comparison confirms the inherited dirty protected baseline was not changed by PolyEngineer-2.

## 18. Recommended Next Action

**VERIFIED** — Stop after Phase 1 with `CORE_FAIL` and `WASM_READY: NO`. In a separately authorized production-repair task, prioritize the ASan overflow, then exact `ComplexPoly` division, polynomial remainder state, scale-sensitive rational normalization, unresolved public symbols, and the intended equality contract. Rerun these exact native commands after repair before considering WebAssembly work.

## 19. Completed vs. Not Completed

Completed:

- **VERIFIED** — Initial artifact, repository, historical-test, and protected-file audits, including final byte-for-byte protected comparison.
- **VERIFIED** — Creation of the live report/checklist.
- **VERIFIED** — Debug, optimized, and ASan/UBSan builds.
- **VERIFIED** — All five debug and all five optimized groups in independent processes.
- **VERIFIED** — All five sanitizer groups attempted independently, plus existing targeted reruns after complex termination.
- **VERIFIED** — Standalone API compile probe and both isolated link probes.
- **VERIFIED** — Final report findings, failure inventory, verdict, and readiness decision.
- **VERIFIED** — Protected-tree byte comparison, staged-path whitelist, and permitted-files-only local commit.

Not completed:

- **NOT EXECUTED** — Production repairs and WebAssembly work are intentionally out of scope.

Committed paths:

- **VERIFIED** — `CORE_VERIFICATION_REPORT.md`.
- **VERIFIED** — `src/test.cpp` (checkpoint change, permitted by task scope).
- **VERIFIED** — `tests/README.md`, `tests/core_verification.cpp`, `tests/oracle_math.h`, `tests/test_harness.h`, and `tests/run_core_verification.sh`.
- **VERIFIED** — `tests/probes/api_compile_probe.cpp`, `tests/probes/complex_undefined_link_probe.cpp`, and `tests/probes/complex_is_undefined_link_probe.cpp`.
- **VERIFIED** — No `/tmp` logs, production implementation/header, WebAssembly, or web path was staged.

## 20. Exact Reproduction Commands

Required initial audit (all exit 0, **VERIFIED**):

```sh
git branch --show-current
git rev-parse HEAD
git status --short
git diff --name-only
git log -3 --oneline
```

Checkpoint-history and environment metadata (all exit 0, **VERIFIED**):

```sh
git rev-parse HEAD^
git log --all --oneline -- tests CORE_VERIFICATION_REPORT.md
g++ --version
uname -a
date --iso-8601=seconds
```

Initial protected snapshot (exit 0, **VERIFIED**):

**VERIFIED** — This command ran before the verification commit, while `HEAD` was the starting hash `3021a17bce36f4ef05630f23d40fe9dc371cbb98`; at that time `HEAD^` was `d42918511bcd4067df06cbb43437f21947dc5883`.

```sh
set -o pipefail
mkdir -p /tmp/polycalc-core-verification
git diff --binary HEAD -- include src/Polynomial.cpp src/Complex.cpp \
  src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp \
  src/extra_polynomials.cpp wasm web README.md src/readme.md makefile \
  .gitignore .github \
  > /tmp/polycalc-core-verification/initial-protected-vs-head.patch
git status --porcelain=v2 -- include src/Polynomial.cpp src/Complex.cpp \
  src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp \
  src/extra_polynomials.cpp wasm web README.md src/readme.md makefile \
  .gitignore .github \
  > /tmp/polycalc-core-verification/initial-protected-status.txt
git diff --name-status HEAD^ -- include src/Polynomial.cpp src/Complex.cpp \
  src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp \
  src/extra_polynomials.cpp wasm web README.md src/readme.md makefile \
  .gitignore .github \
  > /tmp/polycalc-core-verification/initial-protected-vs-parent.txt
find include wasm web .github -type f -print0 2>/dev/null \
  | sort -z | xargs -0 sha256sum \
  > /tmp/polycalc-core-verification/initial-protected-hashes.txt
for path in src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
  src/RootFinding.cpp src/wasm_api.cpp src/extra_polynomials.cpp README.md \
  src/readme.md makefile .gitignore; do
    if [ -f "$path" ]; then
        sha256sum "$path"
    else
        printf 'MISSING  %s\n' "$path"
    fi
done >> /tmp/polycalc-core-verification/initial-protected-hashes.txt
```

Debug build (exit 0, **VERIFIED**):

```sh
g++ -std=c++17 -O0 -g -Wall -Wextra -Wpedantic -Iinclude \
  src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
  tests/core_verification.cpp \
  -o /tmp/polycalc-core-verification/core_verification \
  > /tmp/polycalc-core-verification/debug-build.log 2>&1
```

Debug polynomial group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification --group polynomial \
  > /tmp/polycalc-core-verification/debug-polynomial.log 2>&1
```

Debug complex group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification --group complex \
  > /tmp/polycalc-core-verification/debug-complex.log 2>&1
```

Debug rational group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification --group rational \
  > /tmp/polycalc-core-verification/debug-rational.log 2>&1
```

Debug promotion group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification --group promotion \
  > /tmp/polycalc-core-verification/debug-promotion.log 2>&1
```

Debug API group (exit 0, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification --group api \
  > /tmp/polycalc-core-verification/debug-api.log 2>&1
```

Optimized build (exit 0, **VERIFIED**):

```sh
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude \
  src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
  tests/core_verification.cpp \
  -o /tmp/polycalc-core-verification/core_verification_o2 \
  > /tmp/polycalc-core-verification/optimized-build.log 2>&1
```

Optimized polynomial group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification_o2 --group polynomial \
  > /tmp/polycalc-core-verification/optimized-polynomial.log 2>&1
```

Optimized complex group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification_o2 --group complex \
  > /tmp/polycalc-core-verification/optimized-complex.log 2>&1
```

Optimized rational group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification_o2 --group rational \
  > /tmp/polycalc-core-verification/optimized-rational.log 2>&1
```

Optimized promotion group (exit 1, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification_o2 --group promotion \
  > /tmp/polycalc-core-verification/optimized-promotion.log 2>&1
```

Optimized API group (exit 0, **VERIFIED**):

```sh
/tmp/polycalc-core-verification/core_verification_o2 --group api \
  > /tmp/polycalc-core-verification/optimized-api.log 2>&1
```

ASan/UBSan build (exit 0, **VERIFIED**):

```sh
g++ -std=c++17 -O1 -g -Wall -Wextra -Wpedantic \
  -fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude \
  src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
  tests/core_verification.cpp \
  -o /tmp/polycalc-core-verification/core_verification_sanitized \
  > /tmp/polycalc-core-verification/sanitizer-build.log 2>&1
```

Sanitized polynomial group (exit 1, **VERIFIED**):

```sh
ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  /tmp/polycalc-core-verification/core_verification_sanitized --group polynomial \
  > /tmp/polycalc-core-verification/sanitized-polynomial.log 2>&1
```

Sanitized complex group (exit 134, **VERIFIED** sanitizer termination):

```sh
ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  /tmp/polycalc-core-verification/core_verification_sanitized --group complex \
  > /tmp/polycalc-core-verification/sanitized-complex.log 2>&1
```

Targeted existing high-imaginary-degree regression (exit 1, **VERIFIED**; each child signal 6):

```sh
ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  /tmp/polycalc-core-verification/core_verification_sanitized \
  --test complex.high_imaginary_degree_memory_regressions \
  > /tmp/polycalc-core-verification/sanitized-complex-high-imag-minimal.log 2>&1
```

Targeted deterministic complex properties (exit 134, **VERIFIED** sanitizer termination):

```sh
ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  /tmp/polycalc-core-verification/core_verification_sanitized \
  --test complex.deterministic_generated_safe_properties \
  > /tmp/polycalc-core-verification/sanitized-complex-generated.log 2>&1
```

Sanitized rational group (exit 1, **VERIFIED**):

```sh
ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  /tmp/polycalc-core-verification/core_verification_sanitized --group rational \
  > /tmp/polycalc-core-verification/sanitized-rational.log 2>&1
```

Sanitized promotion group (exit 1, **VERIFIED**):

```sh
ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  /tmp/polycalc-core-verification/core_verification_sanitized --group promotion \
  > /tmp/polycalc-core-verification/sanitized-promotion.log 2>&1
```

Sanitized API group (exit 0, **VERIFIED**):

```sh
ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
  /tmp/polycalc-core-verification/core_verification_sanitized --group api \
  > /tmp/polycalc-core-verification/sanitized-api.log 2>&1
```

Compile-time API expression probe (exit 0, **VERIFIED**):

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude \
  tests/probes/api_compile_probe.cpp -c \
  -o /tmp/polycalc-core-verification/api_compile_probe.o \
  > /tmp/polycalc-core-verification/api-compile-probe.log 2>&1
```

`ComplexPoly::Undefined()` link probe (exit 1, **VERIFIED** API failure):

```sh
g++ -std=c++17 -Iinclude \
  src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
  tests/probes/complex_undefined_link_probe.cpp \
  -o /tmp/polycalc-core-verification/complex_undefined_link_probe \
  > /tmp/polycalc-core-verification/complex-undefined-link-probe.log 2>&1
```

`ComplexPoly::isUndefined() const` link probe (exit 1, **VERIFIED** API failure):

```sh
g++ -std=c++17 -Iinclude \
  src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
  tests/probes/complex_is_undefined_link_probe.cpp \
  -o /tmp/polycalc-core-verification/complex_is_undefined_link_probe \
  > /tmp/polycalc-core-verification/complex-is-undefined-link-probe.log 2>&1
```

Historical smoke build and run (both exit 0, **VERIFIED**):

```sh
g++ -std=c++17 -O0 -g -Wall -Wextra -Wpedantic -Iinclude \
  src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp src/test.cpp \
  -o /tmp/polycalc-core-verification/historical_test \
  > /tmp/polycalc-core-verification/historical-build.log 2>&1

/tmp/polycalc-core-verification/historical_test \
  > /tmp/polycalc-core-verification/historical-run.log 2>&1
```

Initial/final protected-state artifacts were generated with the same path set and hashing order. Final regeneration and comparison (exit 0, **VERIFIED**):

**VERIFIED** — This comparison also ran before the verification commit, so its `HEAD`/`HEAD^` references have the same meanings as the initial snapshot.

```sh
set -o pipefail
git diff --binary HEAD -- include src/Polynomial.cpp src/Complex.cpp \
  src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp \
  src/extra_polynomials.cpp wasm web README.md src/readme.md makefile \
  .gitignore .github \
  > /tmp/polycalc-core-verification/final-protected-vs-head.patch
git status --porcelain=v2 -- include src/Polynomial.cpp src/Complex.cpp \
  src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp \
  src/extra_polynomials.cpp wasm web README.md src/readme.md makefile \
  .gitignore .github \
  > /tmp/polycalc-core-verification/final-protected-status.txt
git diff --name-status HEAD^ -- include src/Polynomial.cpp src/Complex.cpp \
  src/RationalComplex.cpp src/RootFinding.cpp src/wasm_api.cpp \
  src/extra_polynomials.cpp wasm web README.md src/readme.md makefile \
  .gitignore .github \
  > /tmp/polycalc-core-verification/final-protected-vs-parent.txt
find include wasm web .github -type f -print0 2>/dev/null \
  | sort -z | xargs -0 sha256sum \
  > /tmp/polycalc-core-verification/final-protected-hashes.txt
for path in src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
  src/RootFinding.cpp src/wasm_api.cpp src/extra_polynomials.cpp README.md \
  src/readme.md makefile .gitignore; do
    if [ -f "$path" ]; then
        sha256sum "$path"
    else
        printf 'MISSING  %s\n' "$path"
    fi
done >> /tmp/polycalc-core-verification/final-protected-hashes.txt

cmp -s /tmp/polycalc-core-verification/initial-protected-vs-head.patch \
  /tmp/polycalc-core-verification/final-protected-vs-head.patch
cmp -s /tmp/polycalc-core-verification/initial-protected-status.txt \
  /tmp/polycalc-core-verification/final-protected-status.txt
cmp -s /tmp/polycalc-core-verification/initial-protected-vs-parent.txt \
  /tmp/polycalc-core-verification/final-protected-vs-parent.txt
cmp -s /tmp/polycalc-core-verification/initial-protected-hashes.txt \
  /tmp/polycalc-core-verification/final-protected-hashes.txt
```

Required final pre-commit Git audit (all exit 0, **VERIFIED**):

```sh
git status --short
git diff --stat
git diff --name-only
```

Permitted-path staging, staged validation, and local commit (all exit 0, **VERIFIED**):

```sh
git add -- CORE_VERIFICATION_REPORT.md src/test.cpp tests
git diff --cached --name-only
git diff --cached --check

poly_scope_status=0
while IFS= read -r path; do
    case "$path" in
        CORE_VERIFICATION_REPORT.md|src/test.cpp|src/extra_test.cpp|tests/*) ;;
        *)
            printf 'OUT_OF_SCOPE %s\n' "$path"
            poly_scope_status=1
            ;;
    esac
done < <(git diff --cached --name-only)
test "$poly_scope_status" -eq 0

git commit -m "test: complete native polynomial core verification"
```

**VERIFIED** — All compiler/test output is retained only under `/tmp/polycalc-core-verification/` and is not committed.
