// SPDX-License-Identifier: MIT

#include <samconf/json_backend.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "cmocka_mocks/mock_libc.h"
#include "safu/mock_log.h"
#include "samconf/samconf_types.h"
#include "samconfJsonBackendOpen_utest.h"

int samconfTestSamconfJsonBackendOpenFileErrReadingFileSetup(UNUSED void **state) {
    return 0;
}

int samconfTestSamconfJsonBackendOpenFileErrReadingFileTeardown(UNUSED void **state) {
    return 0;
}

void samconfTestSamconfJsonBackendOpenFileErrReadingFile(UNUSED void **state) {
    samconfConfigStatusE_t result = SAMCONF_CONFIG_OK;
    char *path = "test.json";

    samconfConfigBackend_t backend = {
        .originalHandle = path,
        .backendHandle = NULL,
    };

    TEST("samconfJsonBackendOpen File Input with error");
    SHOULD("%s", "Return Error Status and log error message");

    MOCK_FUNC_AFTER_CALL(stat, 0);
    expect_string(__wrap_stat, pathname, path);
    expect_any(__wrap_stat, statbuf);
    will_return(__wrap_stat, 0);

    MOCK_FUNC_AFTER_CALL(json_object_from_file, 0);
    expect_not_value(__wrap_json_object_from_file, filename, NULL);
    will_return(__wrap_json_object_from_file, NULL);

    result = samconfJsonBackendOpen(path, &backend);

    assert_int_equal(result, SAMCONF_CONFIG_ERROR);
    assert_string_equal(backend.originalHandle, path);
    assert_null(backend.backendHandle);
}
