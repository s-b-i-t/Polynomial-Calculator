# Web (React + WASM)

This folder is a fresh frontend scaffold meant to replace the old Vue GUI once the WASM bridge is solid.

## Run

```bash
npm install
npm run dev
```

## WASM note

The app tries to load `/poly.wasm` from the Vite dev server.

- Build it using `wasm/README.md`.
- Place output at `web/public/poly.wasm`.

Once that file exists, click “Run WASM smoke test” in the UI and you should see `WASM add(2, 3) = 5`.
