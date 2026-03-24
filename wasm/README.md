# Npm 
## Output location

- `web/public/poly.wasm`

## Option A: Emscripten (recommended)

If you have `emcc` installed:

```bash
cd wasm
source ~/.emsdk/emsdk_env.sh
emcc poly_wasm.cpp -O2 -s STANDALONE_WASM=1 -Wl,--no-entry -s EXPORTED_FUNCTIONS='[_add]' -o ../web/public/poly.wasm
```

Notes:
- With Emscripten, C exports are typically prefixed (e.g. `_add`). The React loader accepts `add` or `_add`.

## Option B: Clang wasm32 (advanced)

If you have a wasm-enabled clang + lld:

```bash
cd wasm
clang++ --target=wasm32 -O2 -nostdlib -Wl,--no-entry -Wl,--export=add -o ../web/public/poly.wasm poly_wasm.cpp
```

## Inspect exports (optional)

```bash
cd wasm
node inspect_exports.mjs
```

