#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

if ! command -v em++ >/dev/null 2>&1; then
  if [ -f "/home/bhatt/emsdk/emsdk_env.sh" ]; then
    # shellcheck disable=SC1091
    source "/home/bhatt/emsdk/emsdk_env.sh" >/dev/null
  fi
fi

if ! command -v em++ >/dev/null 2>&1; then
  echo "Error: em++ was not found. Install or activate Emscripten before running this script." >&2
  exit 1
fi

mkdir -p "${REPO_ROOT}/web/public"

em++ \
  -std=c++17 \
  -O3 \
  -I"${REPO_ROOT}/include" \
  "${REPO_ROOT}/src/wasm_api.cpp" \
  "${REPO_ROOT}/src/Polynomial.cpp" \
  "${REPO_ROOT}/src/RootFinding.cpp" \
  "${REPO_ROOT}/src/Complex.cpp" \
  "${REPO_ROOT}/src/RationalComplex.cpp" \
  -sMODULARIZE=1 \
  -sEXPORT_NAME=createPolyWasmModule \
  -sALLOW_MEMORY_GROWTH=1 \
  -sNO_EXIT_RUNTIME=1 \
  -sENVIRONMENT=web,worker \
  -sEXPORTED_FUNCTIONS='["_poly_add","_poly_subtract","_poly_multiply","_poly_divide","_poly_roots","_poly_free"]' \
  -sEXPORTED_RUNTIME_METHODS='["cwrap","UTF8ToString"]' \
  -o "${REPO_ROOT}/web/public/poly_wasm.js"

echo "Built ${REPO_ROOT}/web/public/poly_wasm.js"
echo "Built ${REPO_ROOT}/web/public/poly_wasm.wasm"
