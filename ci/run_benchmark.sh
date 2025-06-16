#!/bin/bash
# shellcheck disable=SC2317

SCRIPT_NAME=${0##*/}
###############################################################################
print_info() {
    echo "
    Run the benchmark test suite or parts of it against actual build. Default
    is to run all benchmark tests.

    Usage: ${SCRIPT_NAME} [option] [build type]

    Options:

    -h|--help:                  print this help

    Build Type:

    Debug   - Run benchmark test or interactive shell against the Debug Build
    Release - Run benchmark test or interactive shell against the Release Build
    ...     - Any build type located in build/ can be provided

    <Debug> is the default build type, if nothing is specified.

    Examples:

    $>${SCRIPT_NAME} # run all benchmark tests on Debug build

    $>${SCRIPT_NAME} Release # run all benchmark tests on Release build
    "
}
###############################################################################
CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"
BUILD_TYPE="${1:-Debug}"

. "$BASE_DIR/ci/common_names.sh"

BENCHMARK_RESULT_DIR=${BENCHMARK_RESULT_DIR:-"$RESULT_DIR/benchmark_results"}

export BUILD_TYPE
export BENCHMARK_RESULT_DIR
export BUILD_DIR

PARAM=""
while [ $# -gt 0 ]; do
    case ${1} in
        --help|-h)
            print_info
            exit 0;;
        -*)
            echo "error: unknown option: $1"
            echo "Use ${SCRIPT_NAME} --help"
            exit 1 ;;
        *)
            PARAM="$PARAM ${1}" ;;
    esac
    shift
done
set -- "$PARAM"

"$BASE_DIR"/test/benchmark/bench_test.sh

FAIL=0
find "${BENCHMARK_RESULT_DIR}" -name "*.csv" -type f > tmp
while IFS= read -r CSV_FILE
do
    LINE_COUNT=$(wc -l < "${CSV_FILE}")
    # disabling shellcheck 2086 since LINE_COUNT should be integer for comparison
    # shellcheck disable=SC2086
    if [ ${LINE_COUNT} -lt 2 ]; then
        echo "No results for ${CSV_FILE}"
        (( FAIL++ ))
    else
        echo "Generate plot for '$CSV_FILE' .."
        gnuplot -e report_file=\'"$CSV_FILE"\' "$CMD_PATH/plot_csv_report.p"
    fi
done < tmp

exit ${FAIL}
