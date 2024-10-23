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

FAIL=0
RESULT=""
function run_test() {
    TEST_NAME="${1}"
    RESULT="${RESULT}${TEST_NAME} "
    if time "${SMOKETEST_RUNNER}" "${TEST_NAME}" "${BUILD_TYPE}"; then
        RESULT="${RESULT} OK\n"
    else
        RESULT="${RESULT} FAILED\n"
        FAIL=$((FAIL + 1))
    fi
}

run_test simple_config
run_test signed_config
run_test error_signed_config
run_test sign_config
run_test genkeys
run_test compile_program_using_libsamconf_test_utils

echo "################################################################################"
echo "# Summary"
echo "################################################################################"
echo -e "${RESULT}"

exit "${FAIL}"

