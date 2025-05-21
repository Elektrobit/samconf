#!/bin/sh

BASE_DIR=$(dirname "$0")/../..
export BUILD_TYPE="${BUILD_TYPE-Debug}"
export BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
export DIST_DIR="${DIST_DIR-"${BUILD_DIR}/dist"}"

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-"${DIST_DIR}/usr/local/lib/"}"
export PATH="${PATH}:${DIST_DIR}/usr/local/bin/"
export BENCHMARK_DIR="${BENCHMARK_DIR-"$(dirname "$0")"}"
export BENCHMARK_RESULT_DIR="${BENCHMARK_RESULT_DIR-"$BUILD_DIR/result/benchmark_results"}"
export SAMCONF_SIGNATURE_KEY="$PUBLIC_KEY"

rm -rvf "${BENCHMARK_RESULT_DIR}" >/dev/null 2>&1
mkdir -p "${BENCHMARK_RESULT_DIR}"

calculatetime() {
    loadtime=$1

    loadtime_s=$(echo "${loadtime}" | cut -f 2 -d ":" | cut -f 2 -d " ")
    loadtime_sms=$(LC_NUMERIC=C awk -v time_s="$loadtime_s" 'BEGIN { printf "%.6f", time_s/1000 }')

    loadtime_ns=$(echo "${loadtime}" | cut -f 2 -d ":" | cut -f 4 -d " ")
    loadtime_nsm=$(LC_NUMERIC=C awk -v time_ns="$loadtime_ns" 'BEGIN { printf "%.6f", time_ns/1000000 }')

    loadtime_ms=$(LC_NUMERIC=C awk -v time_sms="$loadtime_sms" -v time_nsm="$loadtime_nsm" 'BEGIN { printf "%.6f", time_sms + time_nsm }')

    echo $loadtime_ms
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

        CONFIG_FILES=$(find ${BENCHMARK_CONFIG_DIR} -maxdepth 1 | tail -n +2)

        FILE_SIZE=0

        for file in $CONFIG_FILES; do
            file_size=$(stat -c %s "${file}")
            FILE_SIZE=$((FILE_SIZE + file_size))
        done

        for _ in $(seq 1 10); do
            MERGE_LOG=$(dump_merged_config ${CONFIG_FILES})
            echo ${MERGE_LOG} | cut -f 1 -d '{' >>${BENCHMARK_LOG}
        done

        echo "Total Files Size : ${FILE_SIZE} bytes" >>${BENCHMARK_LOG}
    done
}

runBenchmark
createcsv
