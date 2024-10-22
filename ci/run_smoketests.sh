#!/bin/bash
set -eou pipefail

CMD_PATH=$(cd "$(dirname "$0")" && pwd)
BASE_DIR=${CMD_PATH%/*}

BUILD_TYPE="${1:-Debug}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
SMOKETEST_RUNNER="$BASE_DIR/test/smoketest/smoketest.sh"

export PREFIX_PATH="$BUILD_DIR/dist/usr/local"
export SMOKETEST_RESULT_DIR="${SMOKETEST_RESULT_DIR-$BUILD_DIR/result/smoketest}"
export LD_LIBRARY_PATH="${BASE_DIR}/build/deps/lib"

rm -rf "$SMOKETEST_RESULT_DIR"

time "$SMOKETEST_RUNNER" simple_config "$BUILD_TYPE"
time "$SMOKETEST_RUNNER" signed_config "$BUILD_TYPE"
time "$SMOKETEST_RUNNER" error_signed_config "$BUILD_TYPE"
time "$SMOKETEST_RUNNER" sign_config "$BUILD_TYPE"
time "$SMOKETEST_RUNNER" genkeys "$BUILD_TYPE"
time "$SMOKETEST_RUNNER" compile_program_using_libsamconf_test_utils "$BUILD_TYPE"

