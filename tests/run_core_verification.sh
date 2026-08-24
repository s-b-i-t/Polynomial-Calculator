#!/usr/bin/env bash

set -u

repo_root=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
build_root=/tmp/polycalc-core-verification
groups=(polynomial complex rational promotion api)
overall_status=0

mkdir -p "$build_root"
cd "$repo_root"

build_debug() {
    g++ \
      -std=c++17 \
      -O0 \
      -g \
      -Wall \
      -Wextra \
      -Wpedantic \
      -Iinclude \
      src/Polynomial.cpp \
      src/Complex.cpp \
      src/RationalComplex.cpp \
      tests/core_verification.cpp \
      -o "$build_root/core_verification"
}

build_optimized() {
    g++ \
      -std=c++17 \
      -O2 \
      -Wall \
      -Wextra \
      -Wpedantic \
      -Iinclude \
      src/Polynomial.cpp \
      src/Complex.cpp \
      src/RationalComplex.cpp \
      tests/core_verification.cpp \
      -o "$build_root/core_verification_o2"
}

build_sanitized() {
    g++ \
      -std=c++17 \
      -O1 \
      -g \
      -Wall \
      -Wextra \
      -Wpedantic \
      -fsanitize=address,undefined \
      -fno-omit-frame-pointer \
      -Iinclude \
      src/Polynomial.cpp \
      src/Complex.cpp \
      src/RationalComplex.cpp \
      tests/core_verification.cpp \
      -o "$build_root/core_verification_sanitized"
}

run_groups() {
    local label=$1
    local binary=$2
    local sanitized=$3
    local group
    local status
    for group in "${groups[@]}"; do
        printf '[ RUNNER ] %s group=%s\n' "$label" "$group"
        if [[ "$sanitized" == "yes" ]]; then
            ASAN_OPTIONS=halt_on_error=1:abort_on_error=1:detect_leaks=1 \
            UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 \
                "$binary" --group "$group"
        else
            "$binary" --group "$group"
        fi
        status=$?
        printf '[ RUNNER ] %s group=%s exit=%d\n' "$label" "$group" "$status"
        if [[ $status -ne 0 ]]; then
            overall_status=1
        fi
    done
}

printf '[ RUNNER ] building debug\n'
build_debug
debug_build_status=$?
printf '[ RUNNER ] debug build exit=%d\n' "$debug_build_status"
if [[ $debug_build_status -eq 0 ]]; then
    run_groups debug "$build_root/core_verification" no
else
    overall_status=1
fi

printf '[ RUNNER ] building optimized\n'
build_optimized
optimized_build_status=$?
printf '[ RUNNER ] optimized build exit=%d\n' "$optimized_build_status"
if [[ $optimized_build_status -eq 0 ]]; then
    run_groups optimized "$build_root/core_verification_o2" no
else
    overall_status=1
fi

printf '[ RUNNER ] building sanitized\n'
build_sanitized
sanitized_build_status=$?
printf '[ RUNNER ] sanitized build exit=%d\n' "$sanitized_build_status"
if [[ $sanitized_build_status -eq 0 ]]; then
    run_groups sanitized "$build_root/core_verification_sanitized" yes
else
    printf '[ RUNNER ] SANITIZERS: NOT AVAILABLE\n'
    overall_status=1
fi

printf '[ RUNNER ] compile-time API probe\n'
g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude \
    tests/probes/api_compile_probe.cpp \
    -c -o "$build_root/api_compile_probe.o"
api_compile_status=$?
printf '[ RUNNER ] API compile probe exit=%d\n' "$api_compile_status"
if [[ $api_compile_status -ne 0 ]]; then
    overall_status=1
fi

printf '[ RUNNER ] link probe ComplexPoly::Undefined\n'
g++ -std=c++17 -Iinclude \
    src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
    tests/probes/complex_undefined_link_probe.cpp \
    -o "$build_root/complex_undefined_link_probe"
undefined_link_status=$?
printf '[ RUNNER ] ComplexPoly::Undefined link exit=%d\n' "$undefined_link_status"
if [[ $undefined_link_status -ne 0 ]]; then
    overall_status=1
fi

printf '[ RUNNER ] link probe ComplexPoly::isUndefined\n'
g++ -std=c++17 -Iinclude \
    src/Polynomial.cpp src/Complex.cpp src/RationalComplex.cpp \
    tests/probes/complex_is_undefined_link_probe.cpp \
    -o "$build_root/complex_is_undefined_link_probe"
is_undefined_link_status=$?
printf '[ RUNNER ] ComplexPoly::isUndefined link exit=%d\n' "$is_undefined_link_status"
if [[ $is_undefined_link_status -ne 0 ]]; then
    overall_status=1
fi

printf '[ RUNNER ] aggregate exit=%d\n' "$overall_status"
exit "$overall_status"
