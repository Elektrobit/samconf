#!/bin/bash

###############################################################################
print_info() {
    SCRIPT_NAME="${0##*/}"
    echo "
Run the smoketest tests suite or parts of it against the actual build.
Default is to run all smoketests.

Usage:  ${SCRIPT_NAME} [option] [build type]

Options:
   -i, --interactive    Launch an interactive shell with samconf in the PATH
   -t, --test TEST      Add test to the list of tests to run if none are provided all get run
   -l, --list           List all possible tests
   -h, --help           Print this help message
    "
}
###############################################################################
set -eou pipefail

CMD_PATH=$(cd "$(dirname "$0")" && pwd)
BASE_DIR=${CMD_PATH%/*}

DEFAULT_TESTS=(
    simple_config
    signed_config
    error_signed_config
    sign_config
    genkeys
    compile_program_using_libsamconf_test_utils
    compile_program_using_libmock_samconf
)
list_tests() {
    for test in "${DEFAULT_TESTS[@]}"; do
        echo "${test}"
    done
}
RUN_TESTS=( )
INTERACTIVE=0
PARAM=( )

while [ $# -gt 0 ]; do
    case "${1}" in
        --interactive|-i)
            INTERACTIVE=1 ;;
        --test|-t)
            RUN_TESTS+=( "${2}" )
            shift ;;
        --list|-l)
            list_tests
            exit 0 ;;
        --help|-h)
            print_info
            exit 0 ;;
        *)
            PARAM+=( "${1}" )
    esac
    shift
done
set -- ${PARAM[@]}
if [ ${#RUN_TESTS[*]} -eq 0 ] && [ ${INTERACTIVE} -eq 0 ]; then
    RUN_TESTS=( "${DEFAULT_TESTS[@]}" )
fi

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

for test in "${RUN_TESTS[@]}"; do
    run_test "${test}"
done

echo "################################################################################"
echo "# Summary"
echo "################################################################################"
echo -e "${RESULT}"

if [ ${INTERACTIVE} -eq 1 ]; then
    "$BASE_DIR/test/smoketest/smoketest_interactive.sh"
fi

exit "${FAIL}"
