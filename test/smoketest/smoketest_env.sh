export PREFIX_PATH="${PREFIX_PATH-"/usr"}"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-""}:${PREFIX_PATH}/lib"
export PATH="${PATH}:${PREFIX_PATH}/bin"

export SMOKETEST_DIR="${SMOKETEST_DIR-${CMDPATH}}"
export SMOKETEST_RESULT_DIR="${SMOKETEST_RESULT_DIR-"./results/smoketest"}"
export SMOKETEST_TMP_DIR="${SMOKETEST_TMP_DIR-"$(mktemp -d /tmp/samconf_smoketest_XXXXXX)"}"

export PRIVATE_KEY="$SMOKETEST_DIR/samconf.pem"
export PUBLIC_KEY="$SMOKETEST_DIR/samconf.pub"
export SAMCONF_SIGNATURE_KEY="$PUBLIC_KEY"
