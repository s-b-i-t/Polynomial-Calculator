# Polynomial Calculator

Portfolio-ready polynomial calculator built around an original C++ polynomial engine and a deployed Vite/React web interface.

## Overview

This project started as a C++ polynomial calculator and has been cleaned up for a focused web deployment. The deployed interface intentionally stays narrow: it accepts two real polynomials, `P(x)` and `Q(x)`, as coefficient lists and performs polynomial/rational polynomial operations.

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

The deployed app currently runs polynomial/rational polynomial operations in TypeScript so the portfolio build is not blocked by a browser WASM bridge. The TypeScript implementation mirrors the real-polynomial subset of the C++ `RationalComplex` arithmetic and keeps the user flow independent of missing `poly.wasm` files.

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

Complete the C++/WASM bridge so the deployed web app can call the original C++ `RationalComplex` engine directly. The current TypeScript implementation is a deployment-safe mirror for the real-polynomial interface, not a replacement for the source C++ model.
