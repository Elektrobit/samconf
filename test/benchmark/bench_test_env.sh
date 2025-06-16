#!/bin/sh
export DIST_DIR="${DIST_DIR-"${BUILD_DIR}/dist"}"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-""}:${BASE_DIR}/build/deps/lib:${DIST_DIR}/usr/local/lib"
export PATH="${PATH}:${DIST_DIR}/usr/local/bin/"
export BENCHMARK_DIR="${BENCHMARK_DIR-"$(dirname "$0")"}"
export BENCHMARK_RESULT_DIR="${BENCHMARK_RESULT_DIR-"$BUILD_DIR/result/benchmark_results"}"
export SAMCONF_SIGNATURE_KEY="${PUBLIC_KEY}"
