#!/bin/sh
# shellcheck disable=SC2317

SCRIPT_NAME=${0##*/}

###############################################################################
print_info() {
    echo "
    Run the benchmark tests

    Usage: ${SCRIPT_NAME} [-h|--help]

    -h|--help:        print this help

    The benchmark test can be configured through several environment variables. See
    bench_test_env.sh for more details about how to setup the test suite and
    about the defaults.

$(cat "$(realpath "$(dirname "${0}")/bench_test_env.sh")")
    "
}
###############################################################################

CMDPATH="$(realpath "$(dirname "$0")")"
. "${CMDPATH}/bench_test_env.sh"

rm -rvf "${BENCHMARK_RESULT_DIR}" >/dev/null 2>&1
mkdir -p "${BENCHMARK_RESULT_DIR}"

calculatetime() {
    loadtime=$1

    loadtime_s=$(echo "${loadtime}" | cut -f 2 -d ":" | cut -f 2 -d " ")
    loadtime_sms=$(LC_NUMERIC=C awk -v time_s="$loadtime_s" 'BEGIN { printf "%.6f", time_s/1000 }')

    loadtime_ns=$(echo "${loadtime}" | cut -f 2 -d ":" | cut -f 4 -d " ")
    loadtime_nsm=$(LC_NUMERIC=C awk -v time_ns="$loadtime_ns" 'BEGIN { printf "%.6f", time_ns/1000000 }')

    loadtime_ms=$(LC_NUMERIC=C awk -v time_sms="$loadtime_sms" -v time_nsm="$loadtime_nsm" 'BEGIN { printf "%.6f", time_sms + time_nsm }')

    echo "$loadtime_ms"
}

createcsv() {
    REPORT_CSV="${BENCHMARK_RESULT_DIR}/report_load_and_merge_config_time.csv"
    echo "Configs Merged, Size of Configs Merged, Avg.Time Load & Merge Time" >"${REPORT_CSV}"
    for i in 3 5 10 100; do
        BENCHMARK_LOG="$BENCHMARK_RESULT_DIR/benchmark_${i}_configs_merge.log"
        LOAD_TIME=0

        for j in $(seq 1 10); do
            LOAD_TIME_STR=$(head -n "${j}" "${BENCHMARK_LOG}" | tail -n -1)
            time=$(calculatetime "${LOAD_TIME_STR}")
            LOAD_TIME=$(LC_NUMERIC=C awk -v lt="$LOAD_TIME" -v t="$time" 'BEGIN { printf "%.6f", lt + t }')
        done

        AVG_LOAD_TIME=$(LC_NUMERIC=C awk -v lt="$LOAD_TIME" 'BEGIN { printf "%.6f", lt / 10 }')

        TOTAL_SIZE=$(head -n 11 "${BENCHMARK_LOG}" | tail -n -1 | cut -f 2 -d ":" | cut -f 2 -d " ")

        echo "${i}, ${TOTAL_SIZE}, ${AVG_LOAD_TIME}" >>"${REPORT_CSV}"
    done
}

runBenchmark() {
    for i in 3 5 10 100; do
        BENCHMARK_CONFIG_DIR="$BENCHMARK_DIR/benchmark_${i}_configs_to_merge"
        BENCHMARK_LOG="$BENCHMARK_RESULT_DIR/benchmark_${i}_configs_merge.log"
        rm -rf "${BENCHMARK_LOG}" >/dev/null 2>&1
        touch "${BENCHMARK_LOG}"

        CONFIG_FILES=$(find "${BENCHMARK_CONFIG_DIR}" -maxdepth 1 | tail -n +2)

        FILE_SIZE=0

        for file in $CONFIG_FILES; do
            file_size=$(stat -c %s "${file}")
            FILE_SIZE=$((FILE_SIZE + file_size))
        done

        for _ in $(seq 1 10); do
            # disabling shellcheck 2086 since CONFIG_FILES need to be passed as args and not string
            # shellcheck disable=SC2086
            MERGE_LOG=$(dump_merged_config -b ${CONFIG_FILES})
            # disabling shellcheck 2086 since MERGE_LOG needs to be echoed as is and not as string
            # shellcheck disable=SC2086
            echo ${MERGE_LOG} | cut -f 1 -d '{' >> "${BENCHMARK_LOG}"
        done

        echo "Total Files Size : ${FILE_SIZE} bytes" >> "${BENCHMARK_LOG}"
    done
}

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

runBenchmark
createcsv
