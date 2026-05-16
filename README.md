# Polynomial Calculator

Portfolio-ready polynomial calculator built around an original C++ polynomial engine compiled to WebAssembly and used by a Vite/React web interface.

## Overview

This project started as a C++ polynomial calculator and has been cleaned up for a focused web deployment. The deployed interface intentionally stays narrow: it accepts two real polynomials, `P(x)` and `Q(x)`, as coefficient lists and performs polynomial/rational polynomial operations through the compiled C++ WebAssembly engine.

Example input:

```text
1, 2, 3
```

means:

```text
1 + 2x + 3x^2
```

## Original C++ Polynomial Engine

The C++ implementation remains the source engine for the project. The core model lives in:

- `include/Polynomial.h`
- `include/Complex.h`
- `include/RationalComplex.h`
- `src/Polynomial.cpp`
- `src/Complex.cpp`
- `src/RationalComplex.cpp`

The current cleanup does not rewrite the existing C++ polynomial or `RationalComplex` logic. It preserves the tested master-branch behavior and keeps the C++ model as the future integration target.

## RationalComplex Design

The canonical internal rational type is `RationalComplex`, which stores a numerator and denominator as `ComplexPoly` values.

Plain polynomial inputs are promoted through the existing helper path:

```cpp
Polynomial -> ComplexPoly -> RationalComplex
```

The helper functions are declared in `include/RationalComplex.h`:

```cpp
make_rational(const Polynomial&)
make_rational(const ComplexPoly&)
```

This lets plain polynomial operations and rational polynomial operations share the same structural arithmetic model:

- `(a / b) + (c / d) = (a*d + c*b) / (b*d)`
- `(a / b) - (c / d) = (a*d - c*b) / (b*d)`
- `(a / b) * (c / d) = (a*c) / (b*d)`
- `(a / b) / (c / d) = (a*d) / (b*c)`

## Current Deployed Vite Web Interface

The `web/` directory contains the portfolio web app. It uses Vite, React, and TypeScript.

The deployed app loads `web/public/poly_wasm.js` and `web/public/poly_wasm.wasm`, then calls C-compatible functions exported from `src/wasm_api.cpp`. The TypeScript polynomial code in the UI is retained only as a visibly labeled fallback/test oracle if the WASM engine fails to load.

The WASM wrapper accepts coefficient-list strings, builds `Polynomial` values, promotes them with `make_rational(...)`, and returns formatted result strings to the UI.

## Operations Supported

The web interface supports:

- `P + Q`
- `P - Q`
- `P * Q`
- `P / Q`
- Remainder after polynomial division
- Derivative of `P`
- Evaluate `P` at `x = a`

The UI intentionally does not include matrices, graphing, roots, symbolic CAS features, complex-number controls, or general calculator buttons.

## Local Development

Build and run the C++ tests from the repository root:

```bash
make clean
make
./ec
```

Build the C++ WebAssembly engine:

```bash
./wasm/build_wasm.sh
```

Run the web app locally:

```bash
cd web
npm install
npm run dev
```

Create a production build:

```bash
cd web
npm run build
```

## Cloudflare Pages Deployment

Use these Cloudflare Pages settings:

- Root directory: `web`
- Build command: `npm run build`
- Output directory: `dist`

## Future Work

Future improvements can simplify rational outputs, broaden test coverage around the WASM wrapper, and add a CI job that builds both the native C++ tests and the WebAssembly artifact before deployment.
