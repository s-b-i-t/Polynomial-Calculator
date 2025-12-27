# Source code for calculator


***description***
- Polynomial.cpp
    - Creates Polynomial objects represented by a vector of coefficients
    - Arithmetic operators are overloaded to simplify operations between objects

- Complex.cpp
    - Similar to Polynomial but includes another vector to represent coefficients

- RationalComplex.cpp
    - All types get promoted to rationalcomplex in order to perform intertype operations
    - Maintains rational expressions when possible

***commands***
- activate emiscripten env 
    - source emsdk_env.sh

- compile to wasm
    - emcc \
  src/wasm_api.cpp \
  src/Polynomial.cpp \
  src/Complex.cpp \
  src/RationalComplex.cpp \
  -Iinclude \
  -std=c++17 \
  -O3 \
  -sWASM=1 \
  -sMODULARIZE=1 \
  -sEXPORT_ES6=1 \
  -sEXPORT_NAME=PolyModule \
  -sEXPORTED_FUNCTIONS='["_wasm_add_double","_wasm_cp_new","_malloc","_free"]' \
  -sEXPORTED_RUNTIME_METHODS='["cwrap","ccall"]' \
  -sALLOW_MEMORY_GROWTH=1 \
  -sNO_EXIT_RUNTIME=1 \
  -fexceptions \
  -sDISABLE_EXCEPTION_CATCHING=0 \
  -o web/poly_wasm.js


