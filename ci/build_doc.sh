#!/bin/bash -eu
CMD_PATH="$(realpath "$(dirname "$0")")"
BASE_DIR="$(realpath "$CMD_PATH/..")"

function printHelp() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo -e "\t -c\t\tclean output directory and generated files before building"
    echo -e "\t -h\t\tprint this help and exit"
}

PARAM=""
OPTION_CLEAN=0
for element in $@; do
    case $element in
        --clean|-c)
            OPTION_CLEAN=1 ;;
        --help|-h)
            printHelp
			exit 0 ;;
        -*)
            echo "error: unknown option: ${element}"
            printHelp
            exit 1 ;;
        *)  PARAM="$PARAM $element" ;;
    esac
done

set -- $PARAM
BUILD_TYPE="${1:-Release}"
BUILD_DIR="$BASE_DIR/build/$BUILD_TYPE"
DIST_DIR="$BUILD_DIR/dist"

MD_DOCUMENTAION_DIR="${BASE_DIR}/documentation"
SAMCONF_SOURCE_SOURCE_DIR="${BASE_DIR}/src"

SPHINX_SOURCE_DIR=${BASE_DIR}
SPHINX_BUILD_DIR=${BUILD_DIR}/doc
SPHINX_GENERATED_SOURCE_DIR=${SPHINX_BUILD_DIR}/source_generated
SPHINX_HTML_OUTPUT_DIR=${SPHINX_BUILD_DIR}/html

. "${SPHINX_VENV-${BASE_DIR}/.venv/}/bin/activate"

rm -rf "${SPHINX_GENERATED_SOURCE_DIR}"
mkdir -p "${SPHINX_GENERATED_SOURCE_DIR}/ADRs" "${SPHINX_GENERATED_SOURCE_DIR}/developer"

function createApiDocu() {
    sphinx-c-apidoc --force \
	    -o "${SPHINX_GENERATED_SOURCE_DIR}/api/" \
	    --tocfile index \
	    "${SAMCONF_SOURCE_SOURCE_DIR}/samconf/"
}

function createDeveloperDocu() {
    sphinx-c-apidoc --force \
	    -o "${SPHINX_GENERATED_SOURCE_DIR}/developer/api/" \
	    --tocfile index \
	    "${BASE_DIR}/test/utest/utils/"
}

createApiDocu
createDeveloperDocu

export PATH="${PATH}:${BASE_DIR}/build/Release/dist/usr/local/bin"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH-"./"}:${BASE_DIR}/build/Release/dist/usr/local/lib"

sphinx-build -b html ${SPHINX_SOURCE_DIR} ${SPHINX_HTML_OUTPUT_DIR} 2> ${SPHINX_BUILD_DIR}/html_doc_error.log
if [ $? -ne 0 ]; then
    echo "Build failed, for details see ${SPHINX_BUILD_DIR}/html_doc_error.log"
    exit 1
fi

WARNINGS=$(grep -e ": WARNING:" -e ": ERROR:" ${SPHINX_BUILD_DIR}/html_doc_error.log | wc -l)
if [ ${WARNINGS} -ne 0 ]; then
    echo ""
    echo "Build warnings ${WARNINGS}"
    echo ""
    cat ${SPHINX_BUILD_DIR}/html_doc_error.log
    exit 1
fi
