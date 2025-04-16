#!/bin/sh
set -eu 

CMDPATH="$(realpath "$(dirname "$0")")"
  
export PREFIX_PATH="${PREFIX_PATH-"/usr"}"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-""}:${PREFIX_PATH}/lib"
export PATH="${PATH}:${PREFIX_PATH}/bin"

export SMOKETEST_DIR="${SMOKETEST_DIR-${CMDPATH}}"
export SMOKETEST_RESULT_DIR="${SMOKETEST_RESULT_DIR-"./results/smoketest"}"
export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR-"$(mktemp -d /tmp/samconf_smoketest_XXXXXX)"}"

export PRIVATE_KEY="$SMOKETEST_DIR/samconf.pem"
export PUBLIC_KEY="$SMOKETEST_DIR/samconf.pub"
export SAMCONF_SIGNATURE_KEY="$PUBLIC_KEY"

smoketest_simple_config() {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/simple_config"
    rm -rvf "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"

    echo "Starting smoketest simple config ..."
    if ! samprobe "$SMOKETEST_DIR"/config.json > "${RESULT_DIR}/simple_config_output.txt" 2>&1; then
        error_exit "samprobe failed"
    fi

    cat "${SMOKETEST_DIR}/simple_config_output.txt" \
        >> "${RESULT_DIR}/simple_config_nosign_output.txt"
    
    echo "Smoketest comparing output ${RESULT_DIR}/simple_config_output.txt with ${RESULT_DIR}/simple_config_nosign_output.txt"
    output_diff=$(diff -w "${RESULT_DIR}/simple_config_output.txt" "${RESULT_DIR}/simple_config_nosign_output.txt" || echo "diff returned: $?")
    if [ -n "$output_diff" ]; then
        echo "Problems occurred while comparing the client output:"
        echo "$output_diff"
        error_exit "Test failed"
    fi

    echo "Finished smoketest."
    exit 0
}

smoketest_signed_config() {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/signed_config"
    rm -rvf "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"
    cp -a "$SMOKETEST_DIR/config.json" "${RESULT_DIR}/config.json"
    CONFIG_FILE="${RESULT_DIR}/config.json"

    echo "Starting smoketest signed config ..."
    samconf-sign "$CONFIG_FILE" "$PRIVATE_KEY"
    samprobe "$CONFIG_FILE" > "${RESULT_DIR}/signed_config_output.txt" 2>&1
    re=$?
    if [ $re -ne 0 ]; then
        error_exit "samprobe with signature failed"
    fi

    echo "Finished smoketest."
    exit 0
}

smoketest_error_signed_config() {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/error_signed_config"
    rm -rvf "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"
    cp -a "$SMOKETEST_DIR/config.json" "${RESULT_DIR}/config.json"
    CONFIG_FILE="${RESULT_DIR}/config.json"

    echo "Starting smoketest error signed config ..."
    echo "RSA-SHA2-256:///1234567890123456789012345678901234567890123456789012345678901234" > "$CONFIG_FILE.sig"
    set +e
    samprobe "$CONFIG_FILE" > "${RESULT_DIR}/error_signed_config_output.txt" 2>&1
    re=$?
    set -e
    if [ $re -eq 0 ]; then
        error_exit "samprobe with wrong signature succeeded"
    fi

    echo "Smoketest check output for expected error"
    
    if ! grep -q "ERR: Signature was invalid" "${RESULT_DIR}/error_signed_config_output.txt"; then
        echo "samprobe failed with wrong error:"
        cat "${RESULT_DIR}/error_signed_config_output.txt"
        error_exit "Test failed"
    fi

    echo "Finished smoketest."
    exit 0
}

smoketest_sign_config() {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/sign_config"
    rm -rvf "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"
    cp -a "$SMOKETEST_DIR/config.json" "${RESULT_DIR}/config.json"

    echo "Starting smoketest signing config ..."


    
    if ! samconf-sign "${RESULT_DIR}/config.json" "$PRIVATE_KEY"; then
        error_exit "signing failed: executable returned error code on exit"
    fi

    if [ ! -f "${RESULT_DIR}/config.json.sig" ]; then
        error_exit "signing failed: signature file doesnt exist"
    fi

    OUTPUT=$(tr -d "\n" < "${RESULT_DIR}/config.json.sig")
    EXPECTED_OUTPUT="RSA-SHA2-256:///vc7o5/wo8qPZ9aJiSYuJK/s7Ba4kZQA1luddm+YLozpuD1AT2YtkiasFKITbI3xJnWmeUFfv9k1mgJ0MEXNK/IF8T89lMSxk7eXze4zR32FwjKtjCrs7n4EU27+427QNE6C1tqmmxpwS07PUBA3ngRCa4n1LhrTciZWOEUMLTqAYsE72zd7yV4z0AMR5b1OhUuiKMpw2TLBypMRQZ9bQ63tXG4tBHpdCwq7c4C2OQLbbKB0OyrANg4MCSRYcghnx3Zks1I2NQCGoaK9ir8oYiHHM81UBREvbrT+1ghiu+Xf4O8NNfegSRM79CRsEyf0p23Qvil7+5RrxuspDBzvgnQ=="
    if [ "$OUTPUT" != "$EXPECTED_OUTPUT" ]; then
        echo "signing failed:"
        echo "$OUTPUT(generated) != $EXPECTED_OUTPUT(expected)"
        error_exit "Test failed"
    fi

    echo "Finished smoketest."
    exit 0
}

smoketest_genkeys() {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/genkeys"
    rm -rvf "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"

    echo "Starting smoketest signing config ..."
    
    if ! samconf-sign create_keys "${RESULT_DIR}" "samconf"; then
        error_exit "creating a key pair failed"
    fi

    if [ ! -f "${RESULT_DIR}/samconf.pem" ] || [ ! -s "${RESULT_DIR}/samconf.pem" ]; then
        error_exit "expected private key file does not exist or is empty : ${RESULT_DIR}/samconf.pem"
    fi

    if [ ! -f "${RESULT_DIR}/samconf.pub" ] || [ ! -s "${RESULT_DIR}/samconf.pub" ]; then
        error_exit "expected public key file does not exist or is empty : ${RESULT_DIR}/samconf.pub"
    fi

    echo "Finished smoketest."
    exit 0
}

run_in_source_tree() {
    [ -d "${SMOKETEST_DIR}/../../build/deps" ]
}

smoketest_compile_program_using_libsamconf_test_utils() {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/compile_program_using_libsamconf_test_utils"
    rm -rvf "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"

    echo "Try to compile simple program using libsamconf_test_utils"
    TEST_C_PROG="${SMOKETEST_DIR}/test_utils_example.c"
    cat "$TEST_C_PROG"

    if run_in_source_tree; then
        BUILD_DEPS_PREFIX="${SMOKETEST_DIR}/../../build/deps"
        EXTRA_FLAGS="-I ${BUILD_DEPS_PREFIX}/include/ -L ${BUILD_DEPS_PREFIX}/lib"
    fi

    # shellcheck disable=SC2086
    # reasoning: ${EXTRA_FLAGS} shall expand to " " separated flags
    printf '%s' "$TEST_C_PROG" \
        | gcc -v "${TEST_C_PROG}" \
        -Wl,--no-as-needed -xc -lsamconf_test_utils -lsamconf -lsafu \
        -I "${PREFIX_PATH}/include/" -L "${PREFIX_PATH}/lib" \
        ${EXTRA_FLAGS} \
        -o "${SMOKETEST_TMP_DIR}/testlibelos" \
        >> "$RESULT_DIR/libsamconf_test_utils.log" 2>&1
    # shellcheck disable=SC2181
    # reasoning: commandline is to long
    if [ $? -ne 0 ]; then
        error_exit "failed to compile test program for libsamconf_test_utils"
    fi
    echo "Finished smoketest."
    exit 0
}

smoketest_compile_program_using_libmock_samconf() {
    RESULT_DIR="$SMOKETEST_RESULT_DIR/compile_program_using_libmock_samconf"
    rm -rvf "${RESULT_DIR}"
    mkdir -p "${RESULT_DIR}"

    echo "Try to compile simple program using libmock_samconf"

    if run_in_source_tree; then
        BUILD_DEPS_PREFIX="${SMOKETEST_DIR}/../../build/deps"
        EXTRA_FLAGS="-I ${BUILD_DEPS_PREFIX}/include/ -L ${BUILD_DEPS_PREFIX}/lib"
    fi

    # shellcheck disable=SC2086
    # reasoning: ${EXTRA_FLAGS} shall expand to " " separated flags
    printf '#include <samconf/mock_samconf.h>\n int main(int argc, char* argv[]){return argc;}\n' | \
        gcc -v -xc \
        -Wl,--no-as-needed -lmock_samconf -lsamconf -lsafu \
        -I "${PREFIX_PATH}/include/" -L "${PREFIX_PATH}/lib" \
        ${EXTRA_FLAGS} \
        -o "${SMOKETEST_TMP_DIR}/testlibmock_samconf" - \
        >> "$RESULT_DIR/libmock_samconf.log" 2>&1
    # shellcheck disable=SC2181
    # reasoning: commandline is to long
    if [ $? -ne 0 ]; then
        error_exit "failed to compile test program for libmock_samconf"
    fi
    echo "Finished smoketest."
    exit 0
}

print_help() {
    echo
    echo "Usage: $0 <simple_config|sign_config|help> <Debug|Release>"
    echo
}

error_exit() {
    echo "Error : $1, terminating smoketest ..."
    exit 1
}

if [ $# -lt 1 ]; then
    print_help
    exit 1
fi

mkdir -p "$SMOKETEST_RESULT_DIR"

case $1 in
    simple_config)
        smoketest_simple_config
        ;;
    signed_config)
        smoketest_signed_config
        ;;
    error_signed_config)
        smoketest_error_signed_config
        ;;
    sign_config)
        smoketest_sign_config
        ;;
    genkeys)
        smoketest_genkeys
        ;;
    compile_program_using_libsamconf_test_utils)
        smoketest_compile_program_using_libsamconf_test_utils
        ;;
    compile_program_using_libmock_samconf)
        smoketest_compile_program_using_libmock_samconf
        ;;
    help)
        print_help
        ;;
    *)
        print_help
        exit 1
        ;;
esac
