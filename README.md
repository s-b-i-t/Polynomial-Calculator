# Polynomial Calculator

This repo currently has:

- `cpp_code/`: the existing C++ polynomial implementation and tests.
- `web/`: a new Vite + React + TypeScript frontend (barebones UI).
- `wasm/`: minimal C++ entrypoints intended to compile to `web/public/poly.wasm`.

## Quick start (frontend)

```bash
cd web
npm install
npm run dev
```

## Build the WASM module

Follow `wasm/README.md` to produce `web/public/poly.wasm`, then refresh the page and run the smoke test.

