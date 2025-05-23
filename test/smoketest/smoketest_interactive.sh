#!/bin/bash
CMDPATH="$(realpath "$(dirname "$0")")"

. "${CMDPATH}/smoketest_env.sh"

mkdir -p "${SMOKETEST_TMP_DIR}"

#change PS1 for the interactive bash and start it
env PS1="[samconf-interactive]\$ " /bin/bash --norc -i

